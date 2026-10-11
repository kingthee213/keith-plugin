"""Check the preset menu's grouping: every category lands under a heading, and none is invented.

    /work/.venv/bin/python /work/keith-plugin/tools/check_menu_grouping.py

## Why this exists

The preset menu groups by KIND, derived from the category field. Two things can go wrong silently:

  - **A category with no matching kind becomes its own heading.** The keyboard patches use
    "key tine", "key organ", "key combo" and "key tape". Without a rule for the "key" prefix those
    became FOUR headings instead of one KEYS, which is not what anyone asked for. Nothing errors;
    the menu just quietly sprouts headings.

  - **A kind that no preset uses gets a heading anyway**, or a preset falls through to "other" and
    is grouped by its raw category string. Either way the menu stops describing the library.

The C++ side is the source of truth: this reads `kKindOrder` and the `kindOf` rules out of
`EditorialControls.h` and applies them to every category that actually exists, in both the baked
109 and the 190 new patches. If the two disagree, this fails.

## What it does not check

It does not run the menu. It checks the mapping, which is the part that breaks silently.
"""

import importlib
import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
LAYOUT = ROOT / "src" / "editor" / "EditorialControls.h"
KEITH_CPP = ROOT / "src" / "Keith.cpp"

GENERATORS = [
    "build_poly_patches",
    "build_lead_patches",
    "build_remaining_patches",
    "build_keyboard_patches",
    "build_final_patches",
]


def read_kind_order(text):
    """kKindOrder from the menu code, so this cannot drift from it."""
    m = re.search(r"kKindOrder\[\]\s*=\s*\{(.*?)\};", text, re.S)
    if not m:
        raise SystemExit("could not find kKindOrder in EditorialControls.h")
    return re.findall(r'"([^"]+)"', m.group(1))


def read_key_prefix_rule(text):
    """Does the menu collapse a prefix, and which one?"""
    m = re.search(r'if \(first == "(\w+)"\)\s*\n\s*return "(\w+)";', text)
    return (m.group(1), m.group(2)) if m else (None, None)


def read_fallback_rules(text):
    """The category -> kind fallbacks, as (categories, kind) pairs."""
    rules = []
    for m in re.finditer(r'if \((.*?)\)\s*\n\s*return "(\w+)";', text):
        cats = re.findall(r'cat == "([^"]+)"', m.group(1))
        if cats:
            rules.append((cats, m.group(2)))
    return rules


def kind_of(cat, order, prefix, prefix_kind, fallbacks):
    first = cat.split(" ")[0] if cat else ""
    if prefix is not None and first == prefix:
        return prefix_kind
    if first in order:
        return first
    for cats, kind in fallbacks:
        if cat in cats:
            return kind
    return cat or "other"


def baked_categories():
    """The categories in the 109 already in the preset table."""
    c = KEITH_CPP.read_text()
    i = c.find("const keith::PresetSpec kPresetSpecs[kNumPresets] = {")
    if i == -1:
        return []
    return re.findall(r'\{"[^"]+",\s*\d+,\s*"[^"]*",\s*"([^"]*)"\}', c[i:c.find("};", i)])


def table_categories():
    """Every category in kPresetSpecs -- the single source of truth for the menu.

    An earlier version also read the generators, because grouping used to be a property of the
    generated patches. It is not any more: the generators emit fine-grained roles ("poly keys",
    "key tine") and tools/apply_preset_tables.py maps them onto the owner's eight when it writes
    the table. Reading the generators here double-counted every patch and reported the
    pre-grouping names as unlisted.

    The table is what the plugin loads, so the table is what this checks.
    """
    c = KEITH_CPP.read_text()
    i = c.find("const keith::PresetSpec kPresetSpecs[kNumPresets] = {")
    if i == -1:
        return []
    return re.findall(r'\{"[^"]+",\s*\d+,\s*"[^"]*",\s*"([^"]*)"\}', c[i:c.find("};", i)])


def main():
    text = LAYOUT.read_text()
    order = read_kind_order(text)
    prefix, prefix_kind = read_key_prefix_rule(text)
    fallbacks = read_fallback_rules(text)

    print(f"  kinds, in menu order: {order}")
    if prefix:
        print("  prefix rule: %r -> %r" % (prefix, prefix_kind))
    else:
        print("  prefix rule: none (the categories are already the groups)")
    print(f"  fallback rules: {len(fallbacks)}")

    failures = 0

    all_cats = table_categories()
    print(f"  categories in the preset table: {len(all_cats)}")

    # 1. nothing may fall through to 'other'
    others = sorted({c for c in all_cats if kind_of(c, order, prefix, prefix_kind, fallbacks)
                     == "other"})
    if others:
        print(f"  FAIL  categories with no heading: {others}")
        failures += 1
    else:
        print("  ok    every category maps to a heading")

    # 2. every kind that appears must be in the menu order, or it gets appended unlisted
    kinds = {kind_of(c, order, prefix, prefix_kind, fallbacks) for c in all_cats}
    unlisted = sorted(kinds - set(order))
    if unlisted:
        print(f"  FAIL  kinds with no place in kKindOrder (they would be appended): {unlisted}")
        failures += 1
    else:
        print("  ok    every kind is in the menu order")

    # 3. every category must BE one of the eight. The old version of this check looked for the
    #    "key " prefix not collapsing into one heading; that can no longer happen, because the
    #    table stores the final group name. What can happen now is a category the menu does not
    #    know, which would show as its own unexpected heading.
    unlisted_cats = sorted(set(all_cats) - set(order))
    if unlisted_cats:
        print(f"  FAIL  categories that are not one of the eight groups: {unlisted_cats}")
        failures += 1
    else:
        print(f"  ok    all {len(set(all_cats))} categories are one of the eight groups")

    # 4. an empty kind must not claim a heading
    used = {kind_of(c, order, prefix, prefix_kind, fallbacks) for c in all_cats}
    empty = [k for k in order if k not in used]
    if empty:
        print(f"  note  kinds with no presets yet (no heading drawn): {empty}")

    print()
    from collections import Counter
    counts = Counter(kind_of(c, order, prefix, prefix_kind, fallbacks) for c in all_cats)
    print("  menu as it would render:")
    for k in order:
        if counts[k]:
            print(f"     {k:9s} ({counts[k]})")
    for k in sorted(set(counts) - set(order)):
        print(f"     {k:9s} ({counts[k]})   <- unlisted")
    print(f"     total {sum(counts.values())}")

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
