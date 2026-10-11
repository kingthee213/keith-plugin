"""Report the library size, in total and by group.

    /work/.venv/bin/python tools/library_count.py

Counts the generated patches (regrouped) and the presets already in the table, so the answer comes
from the data rather than from arithmetic done by hand. The two halves matter separately: the
generated set is what the generators produce right now, and the table is what the plugin actually
ships.
"""

import importlib
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

GENERATORS = ("build_poly_patches", "build_lead_patches", "build_remaining_patches",
              "build_keyboard_patches", "build_final_patches")


def config():
    ns = {}
    exec((ROOT / "tools" / "apply_preset_tables.py").read_text().split("def ")[0], ns)
    return ns["CATEGORY_MAP"], ns["GROUP_ORDER"]


def main():
    cmap, order = config()

    generated = Counter()
    for name in GENERATORS:
        for patch in importlib.import_module(name).build():
            generated[cmap[patch.category]] += 1

    table = (ROOT / "src" / "Keith.cpp").read_text()
    specs = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", table, re.S).group(1)
    rows = re.findall(r'\{"([^"]+)",\s*(\d+),\s*"([^"]*)",\s*"([^"]*)"\}', specs)
    committed = Counter()
    for name, family, source, category in rows:
        if int(family) != 0:
            committed[cmap.get(category, category)] += 1

    combined = Counter()
    for group in order:
        combined[group] = generated.get(group, 0) + committed.get(group, 0)

    def show(title, counter):
        print(f"  {title}")
        for group in order:
            n = counter.get(group, 0)
            if n:
                print(f"    {group:14s} {n:>4}")
        print(f"    {'':14s} {'-' * 4}")
        print(f"    {'total':14s} {sum(counter.values()):>4}")
        print()

    show("THE 205 GENERATED, regrouped", generated)
    show("THE 109 ALREADY IN THE TABLE, regrouped", committed)
    show("COMBINED, as the plugin will ship", combined)

    print(f"  kits and layers that are NOT presets:")
    print(f"    10 drum kits        (selected by the Drum Kit parameter)")
    print(f"     2 percussion kits  (selected by the Percussion Kit parameter)")
    print(f"     1 vocal layer      (Vocal Aah, 14 notes -- a toggle, not a preset)")
    print()
    print(f"  presets:     {sum(combined.values())}")
    print(f"  + 10 kits    = {sum(combined.values()) + 10}")
    print(f"  + 2 perc     = {sum(combined.values()) + 12}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
