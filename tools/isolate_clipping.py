"""Which single parameter drives the clipping, isolated by group.

    /work/.venv/bin/python /work/keith-plugin/tools/isolate_clipping.py

diagnose_clipping.py showed lead and bass clip on EVERY preset. That means the cause
is common to those generators rather than the choice of values, so this prints the
parameters the generators set that the working groups do not. The difference between
a group that clips entirely and one that never does is the signal.
"""

import importlib
import statistics
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
sys.path.insert(0, str(ROOT / "tools"))

MEASUREMENTS = Path("/tmp/lm/level-measurements.tsv")

GENERATORS = [
    "build_poly_patches",
    "build_lead_patches",
    "build_remaining_patches",
    "build_keyboard_patches",
    "build_final_patches",
]


def load():
    peaks = {}
    for line in MEASUREMENTS.read_text().splitlines()[1:]:
        p = line.split("\t")
        if len(p) == 5:
            peaks[int(p[0])] = (p[1], float(p[2]), float(p[3]))

    import patch_fx
    from apply_preset_tables import CATEGORY_MAP, GROUP_ORDER

    by_group = {}
    for name in GENERATORS:
        for patch in importlib.import_module(name).build():
            by_group.setdefault(CATEGORY_MAP[patch.category], []).append(patch)

    indexed = {}
    index = 109
    for group in GROUP_ORDER:
        for patch in by_group.get(group, []):
            indexed[index] = (group, patch)
            index += 1
    return peaks, indexed


def main():
    peaks, indexed = load()

    # Compare the parameter KEYS a always-clipping group sets against a never-clipping one.
    groups = {}
    for i, (group, patch) in indexed.items():
        if i in peaks:
            groups.setdefault(group, []).append(patch)

    print("=== parameters present in EVERY patch of each group ===")
    print("  (a parameter in all leads but no pad is a candidate)")
    print()
    common = {}
    for g, patches in groups.items():
        sets = [set(p.deltas) for p in patches]
        shared = set.intersection(*sets) if sets else set()
        common[g] = shared

    clean = ["polysynth", "pad", "keys"]
    dirty = ["lead", "bass", "reeds/horn"]

    for g in dirty:
        extra = common[g] - set().union(*(common[c] for c in clean)) if common[g] else set()
        print(f"  {g:12s} sets {len(common[g]):3d} params; {len(extra):3d} not in any clean group:")
        for k in sorted(extra):
            print(f"        {k}")

    print()
    print("=== for the suspect parameters, the value each group uses ===")
    suspects = ["kParamOsc1Level", "kParamOsc2Level", "kParamSubLevel", "kParamSubShape",
                "kParamNoiseLevel", "kParamFilterDrive", "kParamTapeSat", "kParamOutLevel",
                "kParamSynthMasterLevel", "kParamAmpSustain", "kParamFilterReso"]
    present = [k for k in suspects if any(any(k in p.deltas for p in ps) for ps in groups.values())]
    print(f"  {'parameter':26s} " + " ".join(f"{g[:9]:>9}" for g in groups))
    for k in present:
        cells = []
        for g in groups:
            vs = [p.deltas[k] for p in groups[g] if k in p.deltas]
            cells.append(f"{statistics.mean(vs):9.1f}" if vs else f"{'-':>9}")
        print(f"  {k:26s} " + " ".join(cells))

    print()
    print("=== GroupVal: does kParamOutLevel exist in any generator? ===")
    allkeys = set()
    for ps in groups.values():
        for p in ps:
            allkeys |= set(p.deltas)
    for k in sorted(allkeys):
        if "Out" in k or "Mst" in k or "MstAlgo" in k:
            print("   ", k)


if __name__ == "__main__":
    main()
