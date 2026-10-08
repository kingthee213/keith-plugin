#!/usr/bin/env python3
"""Gate the per-patch FX chains BRIEF §6.4 requires (U16 Part A).

Every baked preset must carry a deliberate insert + master chain in
`kPresetDeltas`, not a leftover default. This checks the properties of the
shipped table rather than restating the table, so it still catches a preset
that loses its chain, half-writes a stage, or inherits a dangerous default.

Usage: python tools/verify_patch_fx_chains.py
Exit code 0 = pass, 1 = fail.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEADER = (ROOT / "src" / "Keith.h").read_text(encoding="utf-8")
SOURCE = (ROOT / "src" / "Keith.cpp").read_text(encoding="utf-8")

INSERT_ALGOS = ["Off", "Drive", "Chorus", "Delay", "Crush"]
MASTER_ALGOS = ["Off", "Comp", "Limit", "Width", "Tilt"]

# Stage name -> (algo param, A, B, C, Mix)
STAGES = {
    "insert 1": ("kParamIns1Algo", "kParamIns1A", "kParamIns1B", "kParamIns1C", "kParamIns1Mix"),
    "insert 2": ("kParamIns2Algo", "kParamIns2A", "kParamIns2B", "kParamIns2C", "kParamIns2Mix"),
    "master": ("kParamMstAlgo", "kParamMstA", "kParamMstB", "kParamMstC", "kParamMstMix"),
}

# Roles the brief names explicitly. A dry role may carry a slap but not a tail;
# a sustaining role must carry some space.
DRY_ROLES = {"clav", "bass", "stab"}
SPACIOUS_ROLES = {"pad", "motion", "drone", "strings", "choir", "bells"}

# Insert Delay A maps to 0.02 + 0.98 * x^2 seconds, so 25% is about 81 ms.
DRY_ROLE_MAX_DELAY_TIME = 25.0
# Master Limit C is make-up gain over 0-24 dB. 40% (9.6 dB) is the shipped ceiling.
MASTER_LIMIT_MAX_INPUT = 40.0

results = []


def check(name, ok, detail=""):
    results.append(ok)
    print(f"{'PASS' if ok else 'FAIL'}  {name}" + (f"  [{detail}]" if detail else ""))


def parse():
    delta_body = re.search(r"kPresetDeltas\[\]\s*=\s*\{(.*?)\n\};", SOURCE, re.S)
    spec_body = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", SOURCE, re.S)
    count = re.search(r"const int kNumPresets = (\d+);", HEADER)
    if not delta_body or not spec_body or not count:
        return None

    rows = re.findall(r"\{\s*(\d+)\s*,\s*(kParam\w+)\s*,\s*(-?[\d.]+)\s*\}", delta_body.group(1))
    specs = re.findall(
        r'\{\s*"([^"]+)"\s*,\s*(\d+)\s*,\s*"[^"]+"\s*,\s*"([^"]+)"\s*\}', spec_body.group(1))
    deltas = {}
    for preset, param, value in rows:
        deltas.setdefault(int(preset), {})[param] = float(value)
    return int(count.group(1)), specs, deltas


def names(specs, indices):
    return "; ".join(specs[index][0] for index in indices)


def chain_of(written, stage):
    """Return (algo, [a, b, c, mix]) for a stage, (None, missing) if half-written,
    or None if the preset leaves the stage entirely at its default."""
    algo_param, *value_params = STAGES[stage]
    missing = [name for name in STAGES[stage] if name not in written]
    if len(missing) == len(STAGES[stage]):
        return None
    if missing:
        return None, missing
    return int(written[algo_param]), [written[name] for name in value_params]


def main():
    parsed = parse()
    if parsed is None:
        check("preset tables parsed", False, "kPresetDeltas / kPresetSpecs / kNumPresets")
        return 1
    count, specs, deltas = parsed

    if len(specs) != count:
        check("preset spec table matches kNumPresets", False, f"{len(specs)} vs {count}")
        return 1
    check("preset tables parsed", True, f"{count} presets")

    chains = {}
    partial = []
    dead = []
    for preset in range(count):
        written = deltas.get(preset, {})
        chains[preset] = {}
        for stage in STAGES:
            found = chain_of(written, stage)
            if found is None:
                continue
            algo, values = found
            if algo is None:
                partial.append(f"preset {preset} {stage} missing {', '.join(values)}")
                continue
            if algo == 0:
                dead.append(f"preset {preset} {stage}")
            chains[preset][stage] = (algo, values)

    check("every written stage writes algo, A, B, C and mix", not partial,
          "; ".join(partial) if partial else "no half-written stage")
    check("no stage is written with its algorithm Off", not dead,
          "; ".join(dead) if dead else "every written stage is live")

    no_insert = [p for p in range(count) if "insert 1" not in chains[p]]
    check("every preset preloads an insert 1 chain", not no_insert,
          names(specs, no_insert) or f"{count} of {count}")

    no_master = [p for p in range(count) if "master" not in chains[p]]
    check("every preset preloads a master stage", not no_master,
          names(specs, no_master) or f"{count} of {count}")

    silent = [f"preset {p} {stage}" for p in range(count) for stage, (_, v) in chains[p].items()
              if v[3] <= 0.]
    check("no preloaded stage ships at zero mix", not silent,
          "; ".join(silent) if silent else "every stage is audible")

    insert_used = {chains[p][stage][0] for p in range(count)
                   for stage in ("insert 1", "insert 2") if stage in chains[p]}
    master_used = {chains[p]["master"][0] for p in range(count) if "master" in chains[p]}
    check("all four insert algorithms are used by the library",
          insert_used == {1, 2, 3, 4},
          ", ".join(INSERT_ALGOS[a] for a in sorted(insert_used)))
    check("all four master algorithms are used by the library",
          master_used == {1, 2, 3, 4},
          ", ".join(MASTER_ALGOS[a] for a in sorted(master_used)))

    # Family 05's bank file documents one insert effect, not two. Honour that.
    family_five = [index for index, spec in enumerate(specs) if int(spec[1]) == 5]
    second_insert = [p for p in family_five if "insert 2" in chains[p]]
    check("family 05 keeps the single documented insert effect", not second_insert,
          names(specs, second_insert) or f"{len(family_five)} presets, insert 2 off")

    # Families 01-04 all document a second effect slot, so each must actually use one.
    unused_slot = [family for family in (1, 2, 3, 4)
                   if not any("insert 2" in chains[index] for index, spec in enumerate(specs)
                              if int(spec[1]) == family)]
    check("families 01-04 use their second insert slot", not unused_slot,
          ", ".join(f"family 0{family}" for family in unused_slot)
          or "all four families chain two inserts")

    stamped = [preset for preset in range(1, count)
               if chains[preset] == chains[preset - 1]]
    check("no preset repeats its neighbour's chain", not stamped,
          names(specs, stamped) or "every chain differs from the one before it")

    long_tails = []
    for index, spec in enumerate(specs):
        role = spec[2]
        if role not in DRY_ROLES:
            continue
        for stage, (algo, values) in chains[index].items():
            if stage != "master" and algo == 3 and values[0] > DRY_ROLE_MAX_DELAY_TIME:
                long_tails.append(f"{spec[0]} {stage} time {values[0]:g}%")
    check("dry roles carry no long delay", not long_tails,
          "; ".join(long_tails) if long_tails else
          f"clav, bass and stab stay under {DRY_ROLE_MAX_DELAY_TIME:g}% delay time")

    no_space = []
    for index, spec in enumerate(specs):
        if spec[2] not in SPACIOUS_ROLES:
            continue
        algos = {algo for stage, (algo, _) in chains[index].items() if stage != "master"}
        if not algos & {2, 3}:
            no_space.append(spec[0])
    check("sustaining roles carry chorus or delay", not no_space,
          "; ".join(no_space) if no_space else
          "pads, motion, drones, strings, choirs and bells all have space")

    hot = [f"{specs[p][0]} input {chains[p]['master'][1][2]:g}%" for p in range(count)
           if "master" in chains[p] and chains[p]["master"][0] == 2
           and chains[p]["master"][1][2] > MASTER_LIMIT_MAX_INPUT]
    check("master limiter make-up gain stays bounded", not hot,
          "; ".join(hot) if hot else f"every limiter input is <= {MASTER_LIMIT_MAX_INPUT:g}%")

    failed = results.count(False)
    print()
    print(f"verify_patch_fx_chains: {results.count(True)} passed, {failed} failed "
          f"(total {len(results)})")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
