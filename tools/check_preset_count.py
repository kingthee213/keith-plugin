"""Check that kNumPresets matches the number of rows in the preset table.

    /work/.venv/bin/python tools/check_preset_count.py

## Why

`const int kNumPresets = 321;` while `kPresetSpecs` held **314** rows. The array is declared
`kPresetSpecs[kNumPresets]`, so the seven extra slots were zero-initialised -- null `name`
pointers -- and `NPresets()` returned 321. Anything looping to `NPresets()` read entry 314, found
a null name, and died.

That is exactly what the offline renderer did: killed by a signal on preset 1 with an EMPTY
stderr, because the crash came from reading a null pointer rather than from a C++ exception.

Nothing checked this. `check_patch_ranges`, `check_menu_grouping` and `verify_preset_metadata` all
read the table's CONTENTS; not one compared its length against the declared count. A count that is
larger than the data is the worst shape of this bug: it compiles cleanly, every content check
passes, and the failure is a segfault in whichever loop happens to run first.

## What it checks

  - rows in `kPresetSpecs` == `kNumPresets`
  - every delta row references a preset index below the count
  - the highest index referenced by `kPresetDeltas` is a real row

## What it does not do

It counts `{` ... `"` at the start of a row, so a style with a brace then a quote inside a preset
NAME would be miscounted. Preset names here are plain words, and the gate that would matter is the
count itself -- if this ever misreads, the count will disagree loudly rather than silently pass.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEADER = ROOT / "src" / "Keith.h"
SOURCE = ROOT / "src" / "Keith.cpp"


def main():
    h = HEADER.read_text()
    c = SOURCE.read_text()

    m = re.search(r"const int kNumPresets = (\d+);", h)
    if not m:
        print("  FAIL  could not find `const int kNumPresets = <n>;` in Keith.h")
        return 1
    declared = int(m.group(1))

    spec = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", c, re.S)
    if not spec:
        print("  FAIL  could not find the kPresetSpecs table")
        return 1
    rows = len(re.findall(r"\{\s*\"", spec.group(1)))

    problems = []
    print(f"  kNumPresets declared : {declared}")
    print(f"  rows in kPresetSpecs : {rows}")

    if declared != rows:
        problems.append(
            f"kNumPresets is {declared} but the table has {rows} rows"
            + (f" -- {declared - rows} entries past the end are null" if declared > rows
               else f" -- {rows - declared} rows are unreachable"))

    # Every delta row must point at a real preset.
    deltas = re.search(r"kPresetDeltas\[\]\s*=\s*\{(.*?)\n\};", c, re.S)
    if deltas:
        idx = [int(x) for x in re.findall(r"\{\s*(\d+)\s*,", deltas.group(1))]
        if idx:
            high = max(idx)
            print(f"  highest preset index used by deltas : {high}")
            if high >= rows:
                problems.append(f"kPresetDeltas references preset {high}, but only {rows} exist")

    print()
    if problems:
        for p in problems:
            print(f"  FAIL  {p}")
        print()
        print("  A count LARGER than the table means the array's tail is zero-initialised and any")
        print("  loop to NPresets() reads a null name. That is a segfault, not an exception, so it")
        print("  arrives with no message at all -- fix the count, do not fix the loop.")
        return 1

    print("  ok    kNumPresets matches the table, and every delta index is real")
    return 0


if __name__ == "__main__":
    sys.exit(main())
