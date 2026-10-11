"""Find which patch parameters make a preset clip.

    /work/.venv/bin/python /work/keith-plugin/tools/diagnose_clipping.py

The level measurement found 65 of the 190 new patches peaking above 0 dBFS, the
worst at +21 dBFS. That is not a trim problem -- it is over 10x full scale, and a
level trim cannot rescue it. The cause has to be in the parameters.

This reads the measured peaks and correlates them against the generated deltas, so
the culprit is identified from data rather than guessed at from a sample of eight
presets.

## How it correlates

For each parameter, split the patches into those that set it above a threshold and
those that do not, and compare the mean peak of each half. A parameter that drives
clipping shows a large gap. That is crude but it points at the right places, and
the numbers are printed so the reasoning can be checked.
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

# Parameters that only ever route or switch, never add signal.
IGNORE = {"kParamEngine", "kParamPolyMode", "kParamMonoLegato", "kParamFilterType",
          "kParamIns1Algo", "kParamIns2Algo", "kParamMstAlgo", "kParamOsc3Enable",
          "kParamOsc3Source", "kParamLfoDest"}


def load_peaks():
    rows = {}
    for line in MEASUREMENTS.read_text().splitlines()[1:]:
        parts = line.split("\t")
        if len(parts) != 5:
            continue
        rows[int(parts[0])] = (parts[1], float(parts[2]), float(parts[3]))
    return rows


def patches_by_index():
    """index -> (group, deltas), matching the order apply_preset_tables wrote."""
    import patch_fx
    from apply_preset_tables import CATEGORY_MAP, GROUP_ORDER

    ordered = []
    for name in GENERATORS:
        for p in importlib.import_module(name).build():
            ordered.append((CATEGORY_MAP[p.category], p))
    by_group = {}
    for group, p in ordered:
        by_group.setdefault(group, []).append(p)
    out = {}
    index = 109
    for group in GROUP_ORDER:
        for p in by_group.get(group, []):
            out[index] = (group, p.deltas)
            index += 1
    return out


def main():
    peaks = load_peaks()
    patches = patches_by_index()
    print(f"  measured peaks: {len(peaks)}   indexed patches: {len(patches)}")

    new = {i: peaks[i] for i in peaks if i >= 109}
    print(f"  the 190 new:    {len(new)}")
    clipping = [i for i, v in new.items() if v[2] > 0.0]
    print(f"  clipping:       {len(clipping)}")
    print()

    # how much headroom each group has
    from collections import defaultdict
    by_group = defaultdict(list)
    for i, (name, lufs, peak) in new.items():
        group, _ = patches.get(i, ("?", {}))
        by_group[group].append(peak)
    print("=== peak by group ===")
    print(f"  {'group':14s} {'n':>4} {'mean peak':>10} {'worst':>8} {'>0dBFS':>8}")
    for g in sorted(by_group, key=lambda g: -max(by_group[g])):
        v = by_group[g]
        print(f"  {g:14s} {len(v):>4} {statistics.mean(v):>10.2f} {max(v):>8.2f} "
              f"{sum(1 for x in v if x > 0):>8}")

    # correlate parameters against peak
    print()
    print("=== parameters correlated with a high peak ===")
    values = defaultdict(list)
    for i, (group, deltas) in patches.items():
        if i not in new:
            continue
        peak = new[i][2]
        for k, v in deltas.items():
            if k in IGNORE:
                continue
            values[k].append((float(v), peak))

    scored = []
    for k, pairs in values.items():
        if len(pairs) < 30:
            continue
        hi = [p for v, p in pairs if v >= 60.0]
        lo = [p for v, p in pairs if v < 60.0]
        if len(hi) < 8 or len(lo) < 8:
            continue
        gap = statistics.mean(hi) - statistics.mean(lo)
        scored.append((gap, k, len(hi), statistics.mean(hi), statistics.mean(lo)))
    scored.sort(reverse=True)
    print(f"  {'parameter':26s} {'n high':>7} {'mean>60':>9} {'mean<60':>9} {'gap dB':>8}")
    for gap, k, n, mhi, mlo in scored[:14]:
        print(f"  {k:26s} {n:>7} {mhi:>9.2f} {mlo:>9.2f} {gap:>8.2f}")

    print()
    print("=== the sum of the level-ish controls, per patch ===")
    # A voice that sums oscillators, sub, noise and FX past unity will clip however the
    # individual values look, so this totals them.
    LEVELISH = ["kParamOsc1Level", "kParamOsc2Level", "kParamOsc3Level", "kParamSubLevel",
                "kParamNoiseLevel", "kParamFilterDrive", "kParamTapeSat"]
    tot = []
    for i, (group, deltas) in patches.items():
        if i not in new:
            continue
        s = sum(float(deltas.get(k, 0.0)) for k in LEVELISH)
        tot.append((s, new[i][2], new[i][0], group))
    tot.sort(reverse=True)
    print(f"  {'sum':>6} {'peak':>8}  preset")
    for s, peak, name, group in tot[:12]:
        print(f"  {s:>6.0f} {peak:>8.2f}  {name[:30]:30s} {group}")


if __name__ == "__main__":
    main()
