"""Check that every static_assert guarding the parameter table still matches the table.

    /work/.venv/bin/python /work/keith-plugin/tools/check_param_asserts.py

## Why this exists

`src/Keith.cpp` guards the parameter numbering with static_asserts, one per block, so a
reordering or an insert in the middle cannot silently shift every later parameter. The gate is
good -- but the asserts are themselves numbers typed by hand, and appending a parameter without
updating the assert above it produces a build failure that reads like a code bug rather than a
stale comment.

That is exactly what happened on the first real compile of this branch: `kParamOsc3Source` was
asserted to be 118, but 118 is now `kParamSampleMode`. The compiler was right and the assert was
out of date.

This reads the enum from `src/Keith.h` and the asserts from `src/Keith.cpp` and evaluates them. A
mismatch is reported here rather than at the end of a four-minute CI run.
"""

import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
HEADER = ROOT / "src" / "Keith.h"
SOURCE = ROOT / "src" / "Keith.cpp"


def read_params(text):
    """Every `kParam... = N` in the enum."""
    return {name: int(value) for name, value in re.findall(r"(kParam\w+)\s*=\s*(\d+)", text)}


def read_asserts(text):
    """The static_asserts that mention kParam, as (expression, message) pairs."""
    out = []
    for m in re.finditer(r"static_assert\((.*?)\);", text, re.S):
        expr = " ".join(m.group(1).split())
        if "kParam" in expr:
            out.append(expr)
    return out


def main():
    if not HEADER.exists() or not SOURCE.exists():
        print(f"  FAIL  cannot find {HEADER} or {SOURCE}")
        return 1

    params = read_params(HEADER.read_text())
    if not params:
        print("  FAIL  no kParam values found in Keith.h")
        return 1

    highest = max(params.values())
    num_params = highest + 1

    # The numbering must be contiguous: a gap means a parameter was removed or misnumbered, and
    # every assert below it is then meaningless.
    present = set(params.values())
    missing = [n for n in range(min(params.values()), num_params) if n not in present]
    if missing:
        print(f"  FAIL  parameter numbers missing: {missing}")
        return 1
    print(f"  ok    {len(params)} parameters, {min(params.values())}..{highest}, no gaps")

    asserts = read_asserts(SOURCE.read_text())
    if not asserts:
        print("  FAIL  no parameter static_asserts found in Keith.cpp")
        return 1

    failures = 0
    for expr in asserts:
        detail = []
        ok = True
        for name, claimed in re.findall(r"(kParam\w+)\s*==\s*(\d+)", expr):
            actual = params.get(name)
            good = actual == int(claimed)
            if not good:
                ok = False
            detail.append(f"{name}={actual}" + ("" if good else f" ASSERT {claimed}"))
        for claimed in re.findall(r"kNumParams\s*==\s*(\d+)", expr):
            good = num_params == int(claimed)
            if not good:
                ok = False
            detail.append(f"kNumParams={num_params}" + ("" if good else f" ASSERT {claimed}"))
        print(f"  {'ok   ' if ok else 'FAIL '} " + ", ".join(detail))
        if not ok:
            failures += 1

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
