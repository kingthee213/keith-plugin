#!/usr/bin/env python3
"""Source-level gate for the Keith editor surface (U8).

Proves three things without a host or a GUI:

1. Editor source references only parameter names that exist in EParams.
2. Every live parameter appears in the editor source. This is not a runtime
   binding, visibility, hit-testing or duplicate-control proof.
3. Baked preset names and deltas are complete and use neutral family names.

Usage: python tools/editor_surface_check.py
Exit code 0 = pass, 1 = fail.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEADER = ROOT / "src" / "Keith.h"
SOURCE = ROOT / "src" / "Keith.cpp"
EDITOR_DIR = ROOT / "src" / "editor"

# Neutral-label rule (BRIEF §6.5, §9, §10 / docs/UI-DESIGN.md §1): none of these may appear in the
# editor source, the preset names, or the parameter identifiers the surface binds.
FORBIDDEN = [
    "korg", "triton", "wavestation", "m1", "tr ", "tr/", "surge", "ob-x", "obxf", "ob-xf",
    "splice", "freakshow", "mishby", "gorillaz", "lantern", "hewlett",
]

results = []


def check(name, ok, detail=""):
    results.append((name, ok, detail))
    print(f"{'PASS' if ok else 'FAIL'}  {name}" + (f"  [{detail}]" if detail else ""))


def read(path):
    return path.read_text(encoding="utf-8", errors="replace")


def parse_eparams(text):
    """Return the ordered parameter enumerators from the EParams enum."""
    body = re.search(r"enum EParams\s*\{(.*?)\n\};", text, re.S)
    if not body:
        return None

    names = re.findall(r"^\s*(kParam\w+)\s*(?:=\s*(\d+))?", body.group(1), re.M)
    out = []
    value = 0
    for name, literal in names:
        if literal:
            value = int(literal)
        out.append((name, value))
        value += 1
    return out


def parse_presets(text):
    body = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", text, re.S)
    if not body:
        return None
    return re.findall(r'\{\s*"([^"]*)"\s*,\s*(\d+)(?:\s*,\s*"[^"]*"\s*,\s*"[^"]*")?\s*\}', body.group(1))


def parse_deltas(text):
    """Return [(presetIdx, paramIdx)] for every row of the preset delta table."""
    body = re.search(r"kPresetDeltas\[\]\s*=\s*\{(.*?)\n\};", text, re.S)
    if not body:
        return None
    return [(int(p), name) for p, name in
            re.findall(r"\{\s*(\d+)\s*,\s*(kParam\w+)\s*,", body.group(1))]


def main():
    header = read(HEADER)
    source = read(SOURCE)

    params = parse_eparams(header)
    if not params:
        check("EParams enum parsed", False, "src/Keith.h")
        return 1

    # The enum ends with kNumParams (the count sentinel, not a parameter).
    live = {name: idx for name, idx in params if name != "kNumParams"}
    check("EParams parsed", bool(live), f"{len(live)} parameters")
    check("parameter ids are 0..N contiguous", sorted(live.values()) == list(range(len(live))),
          f"max id {max(live.values())}")

    # --- editor region of Keith.cpp (layout + controls), plus the editor headers ------------------
    editor_region = source
    start = source.find("mMakeGraphicsFunc")
    if start > -1:
        end = source.find("#endif", start)
        editor_region = source[start:end if end > -1 else len(source)]

    editor_text = editor_region + "\n" + "\n".join(
        read(p) for p in sorted(EDITOR_DIR.glob("*.h")))

    referenced = set(re.findall(r"\bkParam\w+", editor_text))
    unknown = sorted(name for name in referenced if name not in live)

    check("editor references only real EParams ids", not unknown, ", ".join(unknown) or "none")
    check("every live parameter is exposed by the editor surface",
          set(live) == referenced, f"missing {sorted(set(live) - referenced)}" if set(live) != referenced else "all bound")

    # --- baked presets ---------------------------------------------------------------------------
    names = parse_presets(source)
    if names is None:
        check("baked preset table parsed", False, "kPresetSpecs")
        return 1

    check("at least 12 baked presets", len(names) >= 12, f"{len(names)} presets")
    check("preset names unique and non-empty",
          len({n for n, _ in names}) == len(names) and all(n.strip() for n, _ in names))

    bad_names = [n for n, _ in names if not re.match(r"^Family 0[1-5] / \d{3} [A-Za-z0-9 ]+$", n)]
    check("preset names follow 'Family 0N / NNN <neutral role>'", not bad_names,
          "; ".join(bad_names))

    families = {int(f) for _, f in names}
    check("families 01-05 all represented", families == {1, 2, 3, 4, 5}, f"families {sorted(families)}")

    haystack = " ".join([n for n, _ in names] + list(referenced)).lower()
    hits = [word for word in FORBIDDEN if re.search(rf"(?<![a-z]){re.escape(word.strip())}(?![a-z0-9])", haystack)]
    check("no reference-machine or trademark names in preset names", not hits, ", ".join(hits) or "clean")

    # --- delta table sanity ----------------------------------------------------------------------
    deltas = parse_deltas(source)
    if deltas is None:
        check("preset delta table parsed", False, "kPresetDeltas")
        return 1

    bad_params = sorted({name for _, name in deltas if name not in live})
    check("preset deltas target real parameter ids", not bad_params,
          f"bad ids {bad_params}" if bad_params else f"{len(deltas)} rows")

    bad_presets = sorted({p for p, _ in deltas if p >= len(names)})
    check("preset deltas target existing preset indices", not bad_presets,
          f"bad indices {bad_presets}" if bad_presets else "ok")
    check("every preset has at least one delta",
          {p for p, _ in deltas} == set(range(len(names))),
          "all presets differ from init defaults")

    # One attaching mention in the layout. ControlLayout.h repeats baked knobs as
    # placement data, and the pitch graph shares kParamPitchAmount with its knob.
    layout = read(EDITOR_DIR / "EditorialLayout.h")
    counts = {}
    for name in re.findall(r"\bkParam\w+", layout):
        counts[name] = counts.get(name, 0) + 1

    def allowed(name):
        return 2 if name == "kParamPitchAmount" else 1

    wrong = sorted(f"{name} x{counts.get(name, 0)}" for name in live if counts.get(name, 0) != allowed(name))
    check("every live parameter is attached exactly once",
          not wrong, ", ".join(wrong) if wrong else f"{len(live)} parameters")

    serialized = {name for _, name in deltas}
    u9_deltas = ["kParamFilterKeyTrack", "kParamLfoRate", "kParamLfoDest", "kParamLfoAmount", "kParamFilterDrive"]
    missing_deltas = [name for name in u9_deltas if name not in serialized]
    check("U9 preset deltas serialize key track, LFO and drive",
          not missing_deltas, ", ".join(missing_deltas) if missing_deltas else "present")

    failed = [name for name, ok, _ in results if not ok]
    print()
    print(f"editor_surface_check: {len(results) - len(failed)} passed, {len(failed)} failed "
          f"(total {len(results)})")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
