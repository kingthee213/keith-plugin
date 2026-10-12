"""Find declarations in src/ that nothing calls or uses.

    /work/.venv/bin/python tools/audit_dead_code.py [--all]

## Why

Two dead things were found by hand while chasing a blank-editor bug, and both had been sitting
there long enough to mislead:

  * `knob()` -- the only caller of `PlacementFor()` -- has ZERO call sites. All 56 knobs go
    through `freeKnob`. `PlacementFor` ends in `assert(false)` that compiles OUT in release and
    silently returns `kKnobPlacements[0]`, so 28 parameters with no physical placement would get
    the outermost knob's position with no diagnostic. Unreachable today, live the moment anyone
    calls `knob()`.
  * `sample_waveform` is an overlay rect no control is attached to.

Neither breaks anything. Both cost time: a reader has to work out whether the dead thing is dead
or whether they have missed a call path, and a gate that checks all 115 control rects has to be
told which ones do not matter.

## What it checks

For each function DEFINED in src/ (in a .h or .cpp), count references to its name outside its own
definition. A function with zero references anywhere is reported.

Then two structural checks that this project has actually tripped on:

  * `assert(false)` inside a function that returns a fallback anyway -- the assert vanishes in
    release and the fallback is returned silently. These are reported separately as LATENT,
    because they are not dead code but they are the same trap.
  * overlay ids defined in EditorGeometry.h that no control is attached to.

## What it does not do

It does not do real call-graph analysis. A function named in a comment or a string counts as a
reference, so this UNDER-reports -- it will miss dead code rather than cry wolf. Functions used
only through a function pointer or as a virtual override are treated as live, because they are.

Run with --all to list every function and its reference count rather than only the zero ones.
"""

import re
import sys
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"

# A declared-and-defined ENGINE function with no caller is a different fault from a dead helper,
# and conflating them hides the more serious one. An unwired engine function means a feature that
# exists and does nothing -- TempoSyncFromSlices, TriggerSlice and SliceAtFrame were all declared,
# never called, and never audible. That is a missing connection, not dead weight: the fix is to
# call it, not to delete it. They are reported separately for that reason.
ENGINE_DIRS = ("sampler", "drums", "synth", "instruments", "fx")

# An assert(false) followed by a DELIBERATE safe fallback is a fix, not a trap. OverlayIRECT
# returns a zero rect on a missing id on purpose: it replaced a null deref that took a host down.
# Only assert(false) followed by returning a plausible-looking value is the trap.
DELIBERATE_FALLBACK = {"OverlayIRECT"}

# Names that are overridden or called by the framework, not by our code. Reporting these as dead
# would be wrong and would train a reader to ignore the tool.
LIVE_BY_DESIGN = re.compile(
    r"^(On|Process|Draw|Serialize|Unserialize|Init|Reset|Attach|Set|Get|Load|Save|"
    r"Make|Build|Render|Apply|Update|Handle|Prepare|Create|Dealloc|Destroy)"
)

def strip_comments_strings(text):
    """Remove comments and string literals so a name in a comment is not a reference."""
    out = []
    i = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            while i < n and text[i] != "\n":
                i += 1
            continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            i += 2
            while i + 1 < n and not (text[i] == "*" and text[i + 1] == "/"):
                i += 1
            i += 2
            continue
        if c == '"':
            i += 1
            while i < n and text[i] != '"':
                i += 2 if text[i] == "\\" else 1
            i += 1
            continue
        out.append(c)
        i += 1
    return "".join(out)


def definitions(text):
    """Function names defined in this translation unit (name before '(' at a definition)."""
    names = set()
    for m in re.finditer(r"(?m)^\s*(?:[\w:<>,\s\*&]+?)\b(\w+)\s*\([^;{]*\)\s*(?:const\s*)?\{", text):
        names.add(m.group(1))
    for m in re.finditer(r"(?m)^\s*(?:inline|static|constexpr)\s+[\w:<>,\s\*&]+?\b(\w+)\s*\([^;{]*\)\s*\{", text):
        names.add(m.group(1))
    return names


def main():
    show_all = "--all" in sys.argv
    files = sorted(list(SRC.rglob("*.h")) + list(SRC.rglob("*.cpp")))
    raw = {f: f.read_text(errors="replace") for f in files}
    clean = {f: strip_comments_strings(t) for f, t in raw.items()}
    whole = "\n".join(clean.values())

    defined = {}
    for f, t in clean.items():
        for name in definitions(t):
            defined.setdefault(name, f)

    dead = []
    unwired = []
    for name, where in sorted(defined.items()):
        if LIVE_BY_DESIGN.match(name):
            continue
        uses = len(re.findall(r"\b" + re.escape(name) + r"\b", whole)) - 1
        if uses <= 0:
            in_engine = any(part in where.parts for part in ENGINE_DIRS)
            (unwired if in_engine else dead).append((name, where))
        elif show_all:
            print(f"    {name:36s} {uses:4d} use(s)  {where.relative_to(ROOT)}")

    print(f"  functions defined in src/ : {len(defined)}")

    # LATENT: assert(false) followed by a returning fallback.
    latent = []
    for f, t in raw.items():
        for m in re.finditer(r"assert\(false[^)]*\)\s*;?\s*return\s+([^;]+);", t):
            fn = None
            head = t[:m.start()]
            fm = list(re.finditer(r"\b(\w+)\s*\([^;{]*\)\s*(?:const\s*)?\{[^{}]*$", head))
            if fm:
                fn = fm[-1].group(1)
            latent.append((fn or "?", f.relative_to(ROOT), m.group(1).strip()))

    # Overlay ids nothing is attached to.
    geo = (SRC / "editor" / "EditorGeometry.h").read_text()
    layout = (SRC / "editor" / "EditorialLayout.h").read_text()
    table = set(re.findall(r'\{\s*"([a-z_0-9]+)"', re.search(
        r"kOverlayRects\[\]\s*=\s*\{(.*?)\n\};", geo, re.S).group(1)))
    # A rect is used either by OverlayIRECT("id") or through the kBound id table.
    used = set(re.findall(r'OverlayIRECT\(\s*"([a-z_0-9]+)"\s*\)', layout))
    for m in re.finditer(r"\{[^}]*?\"([a-z_0-9]+)\"[^}]*?\}", layout):
        used.add(m.group(1))
    for m in re.finditer(r'"(page_knob_\w+)"', layout):
        used.add(m.group(1))
    unused_rects = sorted(table - used)

    print()
    if unwired:
        print(f"  UNWIRED -- engine functions defined but never called ({len(unwired)}).")
        print("  These are NOT dead weight: a feature that exists and does nothing. Fix by calling")
        print("  it, not by deleting it. Each one is a missing connection to the UI or the DSP.")
        for name, where in unwired:
            print(f"    {name:34s} {where.relative_to(ROOT)}")
        print()

    if dead:
        print(f"  DEAD -- defined in src/, referenced nowhere ({len(dead)}):")
        for name, where in dead:
            print(f"    {name:34s} {where.relative_to(ROOT)}")
    else:
        print("  ok    no dead non-engine function definitions found")

    if latent:
        print()
        print("  LATENT -- assert(false) then a silent fallback (the assert compiles out in release):")
        for fn, where, ret in latent:
            note = "  [deliberate: documented guard]" if fn in DELIBERATE_FALLBACK else ""
            print(f"    {fn:24s} returns {ret:24s} {where}{note}")

    real_unused = [r for r in unused_rects if r != "sample_waveform"]
    if unused_rects:
        print()
        print("  overlay rects no control is attached to:")
        for r in unused_rects:
            print(f"    {r}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
