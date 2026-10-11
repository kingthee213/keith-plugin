"""Isolate the clipping cause by sweeping one parameter on a real clipping patch.

    /work/.venv/bin/python /work/keith-plugin/tools/clip_probe.py           # write the probe table
    /work/.venv/bin/python /work/keith-plugin/tools/clip_probe.py --read <tsv>

## Why

65 of the 190 new patches clip, peaking up to +21 dBFS. The measurement showed the split
is exactly mono (65/65 clip) versus poly (0/125), but two attempted explanations were
wrong: unison is not the cause (patches without it clip worse), and the engine does not
sum sixteen voices in mono (`Keith_DSP.h` batchSize only batches the GLIDE; CoreSynth
allocates `mVoices[0]` alone for mono).

Comparing my mono patches against the 109's mono ones leaves four candidates. Rather
than rebuild 65 patches on a guess -- having already been wrong twice -- this puts the
candidates into a small number of probe presets so ONE CI round separates them.

## The design

A single deep-dive patch ("Probe Base") that clips, then one variant per candidate with
that parameter alone set to the 109's mono average. Plus a control that is the patch
unchanged, so the comparison is against a measured baseline in the same run.

The candidates, from the diff against the 109's mono presets:

    kParamFmAmount      109  10.0   mine 44.2    ~4x
    kParamLfoAmount     109  11.0   mine 46.0    ~4x
    kParamFilterReso    109  18.8   mine 30.7
    kParamPorta         109  82.0   mine  2.6

If one variant drops the peak by ~20 dB, that parameter is the cause and the rebuild is
mechanical. If none does, the cause is a combination or something not in this list, and
the answer is to build the mono patches from the 109's mono envelope and glide settings
rather than to keep guessing.
"""

import argparse
import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
SOURCE = ROOT / "src" / "Keith.cpp"
HEADER = ROOT / "src" / "Keith.h"

BEGIN = "  // --- BEGIN clipping probes"
END = "  // --- END clipping probes"

# The patch to copy. Thin Whistle measured +10.71 LUFS / peak +20.05 dBFS, one of the worst,
# and it is a simple reed voice with nothing exotic in it.
BASE = "Thin Whistle"

# (label, parameter, value) -- the 109's mono average for each.
CANDIDATES = [
    ("control",   None,                 None),
    ("fm",        "kParamFmAmount",     10.0),
    ("lfo",       "kParamLfoAmount",    11.0),
    ("reso",      "kParamFilterReso",   18.8),
    ("porta",     "kParamPorta",        82.0),
    ("envelope",  None,                 None),   # special: Amp times from the 109's mono mean
    ("all",       None,                 None),   # every candidate at once
]

# The 109's mono averages, for the "all" and "envelope" probes.
ENV_109 = {
    "kParamAmpAttack": 7.9,
    "kParamAmpDecay": 346.7,
    "kParamAmpSustain": 68.6,
    "kParamAmpRelease": 233.3,
    "kParamFilterCutoff": 2780.0,
    "kParamFilterEnv": 39.4,
    "kParamVelToAmp": 35.7,
}


def read_base():
    """The generated deltas for BASE, as {param: value}."""
    sys.path.insert(0, str(ROOT / "tools"))
    import importlib
    from apply_preset_tables import CATEGORY_MAP

    for name in ("build_poly_patches", "build_lead_patches", "build_remaining_patches",
                 "build_keyboard_patches", "build_final_patches"):
        for patch in importlib.import_module(name).build():
            if patch.name == BASE:
                return dict(patch.deltas)
    raise SystemExit(f"{BASE} not found in the generators")


def current_count():
    c = SOURCE.read_text()
    m = re.search(r"const int kNumPresets = (\d+);", HEADER.read_text())
    base = int(m.group(1))
    if BEGIN in c:
        raise SystemExit("probe block already present; strip it before re-writing")
    return base


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--read")
    args = ap.parse_args()

    if args.read:
        return read_results(Path(args.read))

    deltas = read_base()
    base_index = current_count()
    print(f"  base patch: {BASE}  ({len(deltas)} params)")
    print(f"  appending {len(CANDIDATES)} probe presets at index {base_index}")

    specs, rows = [], []
    for offset, (label, param, value) in enumerate(CANDIDATES):
        index = base_index + offset
        name = f"Probe {label}"
        specs.append(f'  {{"{name}", 0, "mags", "keys"}},')
        d = dict(deltas)
        if label == "fm":
            d[param] = value
        elif label == "lfo":
            d[param] = value
        elif label == "reso":
            d[param] = value
        elif label == "porta":
            d[param] = value
        elif label == "envelope":
            d.update(ENV_109)
        elif label == "all":
            d.update({k: v for k, v in ENV_109.items()})
            d["kParamFmAmount"] = 10.0
            d["kParamLfoAmount"] = 11.0
            d["kParamFilterReso"] = 18.8
            d["kParamPorta"] = 82.0
        items = [f"{{{index}, {k}, {int(v) if float(v).is_integer() else v}}}"
                 for k, v in d.items()]
        for i in range(0, len(items), 3):
            rows.append("  " + ",   ".join(items[i:i + 3]) + ",")

    c = SOURCE.read_text()
    h = HEADER.read_text()

    # specs: insert the probe rows just before the closing brace of kPresetSpecs
    m = re.search(r"(kPresetSpecs\[kNumPresets\]\s*=\s*\{)(.*?)(\n\};)", c, re.S)
    block = "\n" + BEGIN + "\n" + "\n".join(specs) + "\n" + END
    c = c[:m.end(2)] + block + c[m.end(2):]

    # deltas: same, on kPresetDeltas
    m2 = re.search(r"(kPresetDeltas\[\]\s*=\s*\{)(.*?)(\n\};)", c, re.S)
    block2 = "\n" + BEGIN + "\n" + "\n".join(rows) + "\n" + END
    c = c[:m2.end(2)] + block2 + c[m2.end(2):]

    SOURCE.write_text(c)
    HEADER.write_text(h.replace(f"const int kNumPresets = {base_index};",
                                f"const int kNumPresets = {base_index + len(CANDIDATES)};"))
    print(f"  wrote. kNumPresets {base_index} -> {base_index + len(CANDIDATES)}")
    print("  build, then: clip_probe.py --read <measurements.tsv>")
    return 0


def read_results(path):
    rows = {}
    for line in path.read_text().splitlines()[1:]:
        p = line.split("\t")
        if len(p) == 5:
            rows[p[1]] = (float(p[2]), float(p[3]))
    base = rows.get(BASE)
    print(f"  baseline {BASE}: "
          + (f"{base[0]:.2f} LUFS  peak {base[1]:+.2f} dBFS" if base else "NOT MEASURED"))
    print()
    print(f"  {'probe':14s} {'LUFS':>9} {'peak':>9} {'peak vs base':>14}")
    for label, _, _ in CANDIDATES:
        key = f"Probe {label}"
        if key not in rows:
            print(f"  {label:14s} {'-':>9} {'-':>9}")
            continue
        lufs, peak = rows[key]
        delta = f"{peak - base[1]:+.2f} dB" if base else "-"
        print(f"  {label:14s} {lufs:>9.2f} {peak:>9.2f} {delta:>14}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
