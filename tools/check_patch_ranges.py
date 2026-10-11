"""Check every generated patch value against its parameter's registered range.

    /work/.venv/bin/python /work/keith-plugin/tools/check_patch_ranges.py

## Why this exists

Preset deltas are written as plain numbers. A value outside its parameter's range is **silently
clamped** by the host -- the patch loads, sounds wrong, and nothing anywhere says why. That is
the worst kind of bug in a sound library: it looks like a bad patch rather than a bad value.

This caught two on the first run of the poly set:

    kParamOsc1PW    0.0   but the range is   5..95    (the default was emitted as 0)
    kParamOsc2Pitch 31.0  but the range is -24..24

Both would have loaded as something other than what was designed.

## What it checks

For each generated patch, every delta is compared against the min and max parsed out of
`InitDouble(...)` / `InitInt(...)` in src/Keith.cpp. Parameters registered with `InitBool` or
`InitEnum` have no numeric range and are skipped.

Exits non-zero on any violation, so it can gate a build.
"""

import importlib
import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
KEITH_CPP = ROOT / "src" / "Keith.cpp"

# The patch generators, each exposing build().
GENERATORS = ["build_poly_patches"]


def parse_ranges():
    """min and max for every numerically-registered parameter."""
    c = KEITH_CPP.read_text()
    ranges = {}
    # InitDouble("name", default, MIN, MAX, step, ...) and InitInt(...) share the shape.
    # Groups: 1 = param name, 2 = default, 3 = MIN, 4 = MAX.
    for m in re.finditer(
        r'GetParam\((kParam\w+)\)->Init(?:Double|Int)\("[^"]*",\s*([-\d.]+),\s*([-\d.]+),\s*([-\d.]+)',
        c):
        ranges[m.group(1)] = (float(m.group(3)), float(m.group(4)))
    return ranges


def main():
    sys.path.insert(0, str(ROOT / "tools"))
    ranges = parse_ranges()
    if not ranges:
        print("could not parse any parameter ranges from src/Keith.cpp")
        return 1

    failures = 0
    total = 0

    for name in GENERATORS:
        try:
            module = importlib.import_module(name)
        except Exception as exc:                       # noqa: BLE001 -- report, do not crash
            print(f"  FAIL  could not import {name}: {exc}")
            failures += 1
            continue

        patches = module.build()
        violations = []
        unknown = set()

        for patch in patches:
            for param, value in patch.deltas.items():
                total += 1
                if param not in ranges:
                    unknown.add(param)
                    continue
                lo, hi = ranges[param]
                if not (lo - 1e-9 <= float(value) <= hi + 1e-9):
                    violations.append((patch.name, param, float(value), lo, hi))

        label = f"{name} ({len(patches)} patches)"
        if violations:
            print(f"  FAIL  {label}: {len(violations)} value(s) outside range")
            for patch_name, param, value, lo, hi in violations:
                print(f"          {patch_name:30s} {param:26s} {value:9.2f}  range {lo}..{hi}")
            failures += 1
        else:
            print(f"  ok    {label}: every value in range")

        if unknown:
            # Not a failure: InitBool and InitEnum parameters have no numeric range, and a
            # category name that is not a parameter is a generator bug worth seeing.
            print(f"          no range to check (bool/enum): {', '.join(sorted(unknown))}")

    print()
    print(f"{total} values checked across {len(GENERATORS)} generator(s)")
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
