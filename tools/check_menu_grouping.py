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
    return re.findall(r'"(\w+)"', m.group(1))


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


def generated_categories():
    sys.path.insert(0, str(ROOT / "tools"))
    seen = []
    for name in GENERATORS:
        try:
            module = importlib.import_module(name)
        except Exception as exc:                       # noqa: BLE001
            print(f"  FAIL  could not import {name}: {exc}")
            continue
        for patch in module.build():
            seen.append(patch.category)
    return seen


def main():
    text = LAYOUT.read_text()
    order = read_kind_order(text)
    prefix, prefix_kind = read_key_prefix_rule(text)
    fallbacks = read_fallback_rules(text)

    print(f"  kinds, in menu order: {order}")
    print(f"  prefix rule: \"{prefix}\" -> \"{prefix_kind}\"")
    print(f"  fallback rules: {len(fallbacks)}")

    failures = 0

    baked = baked_categories()
    generated = generated_categories()
    all_cats = baked + generated
    print(f"  categories to classify: {len(baked)} baked + {len(generated)} generated")

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

    # 3. the keyboard patches must NOT each become their own heading. This is the regression
    #    this file was written for.
    key_cats = sorted({c for c in all_cats if c.startswith("key ")})
    key_kinds = {kind_of(c, order, prefix, prefix_kind, fallbacks) for c in key_cats}
    if key_cats and key_kinds != {prefix_kind}:
        print(f"  FAIL  keyboard categories {key_cats} did not all group under "
              f"'{prefix_kind}': {sorted(key_kinds)}")
        failures += 1
    else:
        print(f"  ok    {len(key_cats)} keyboard categor{'y' if len(key_cats)==1 else 'ies'} "
              f"group under one '{prefix_kind}' heading")

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
