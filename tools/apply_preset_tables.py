"""Write the 190 new patches into the preset tables, and regroup all 299 by the owner's eight.

    /work/.venv/bin/python /work/keith-plugin/tools/apply_preset_tables.py          # dry run
    /work/.venv/bin/python /work/keith-plugin/tools/apply_preset_tables.py --write

## What this does

Two changes to `src/Keith.cpp`, both driven from data rather than typed by hand:

1. **Appends the 190 generated patches** to `kPresetSpecs` and `kPresetDeltas` after the existing
   109. Families are gone: the owner asked for grouping by role, and the `Family NN / NNN` prefix
   was a family label, so the new names are plain. The existing 109 keep their names -- those are
   in the manual and the level comments.

2. **Rewrites the category of all 299** onto the owner's eight groups: polysynth, lead, keys,
   bass, drums, reeds/horn, pad, strings/fx. The menu groups by category, so this is what the
   dropdown shows.

## Levels are deliberately NOT written

The 190 go in without a `kParamSynthMasterLevel`. Measuring is what
tools/measure_preset_levels.py does, through the render binary, and that has to happen after
these are in the tables -- you cannot measure a preset that does not exist. So the order is:
apply this, build, measure, then emit the trims into the block the measurement tool already
knows how to rewrite.

Until that runs, the 190 will be uneven in volume against the 109. That is expected, not a bug.

## Idempotence

Running it twice must not append twice, so it looks for a marker comment it writes itself and
refuses to proceed if the patches are already present.
"""

import argparse
import importlib
import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
SOURCE = ROOT / "src" / "Keith.cpp"
sys.path.insert(0, str(ROOT / "tools"))

GENERATORS = [
    "build_poly_patches",
    "build_lead_patches",
    "build_remaining_patches",
    "build_keyboard_patches",
    "build_final_patches",
]

BEGIN_NEW = "// --- BEGIN the MAGS library"
END_NEW = "// --- END the MAGS library"

# The owner's eight groups, in menu order.
GROUP_ORDER = ["mono synths", "poly synths", "guitars", "winds", "sfx", "vocal", "bass",
               "strings"]

# Every category that exists, mapped onto those eight. Anything not listed is an error: a silent
# fall-through would put a preset in a group of its own and nobody would notice.
CATEGORY_MAP = {
    # --- mono synths -----------------------------------------------------------------------
    "lead": "mono synths", "lead saw": "mono synths", "lead pulse": "mono synths",
    "lead sync": "mono synths", "lead fm": "mono synths", "lead table": "mono synths",
    "mono": "mono synths", "bass lead": "mono synths",
    # --- poly synths -----------------------------------------------------------------------
    "poly": "poly synths", "polysynth": "poly synths", "keys": "poly synths",
    "poly keys": "poly synths", "clav": "poly synths", "organ": "poly synths",
    "key tine": "poly synths", "key organ": "poly synths", "key combo": "poly synths",
    "key tape": "poly synths", "bells": "poly synths", "stab": "poly synths",
    "pluck": "poly synths", "arp": "poly synths",
    "poly synth": "poly synths", "poly saw": "poly synths", "poly pluck": "poly synths",
    "poly pad": "strings", "poly warm keys": "poly synths",
    "lead keys": "mono synths", "poly lead keys": "poly synths",
    # --- guitars ---------------------------------------------------------------------------
    "guitars": "guitars",
    # --- winds -----------------------------------------------------------------------------
    "reeds": "winds", "brass": "winds", "reed": "winds", "horn": "winds",
    "winds": "winds",
    # --- sfx -------------------------------------------------------------------------------
    "fx": "sfx", "sfx": "sfx", "texture": "sfx", "motion": "sfx", "noise": "sfx",
    "drone": "sfx", "riser": "sfx",
    # --- vocal -----------------------------------------------------------------------------
    "choir": "vocal", "vocal": "vocal", "voice": "vocal",
    # --- bass -------------------------------------------------------------------------------
    "bass": "bass", "sub": "bass",
    # --- names already written into the table by the previous grouping pass -----------------
    # A re-apply reads the table as it stands, so it meets these. Without them the build stops on
    # 'unknown category', which is how this was found.
    "polysynth": "poly synths", "lead": "mono synths", "keys": "poly synths",
    "pad": "strings", "strings/fx": "strings", "reeds/horn": "winds",
    # The 7 ORIGINAL synthesised drum presets. They stay in the table at their existing indices
    # because kPresetDeltas and the level trims are keyed by index -- dropping them would shift
    # 205 presets' data onto the wrong patches. They are retagged to a category the menu does not
    # list, so they leave the dropdown while their rows stay exactly where they are.
    "drums": "retired drums",
    # --- strings ---------------------------------------------------------------------------
    "strings": "strings", "pad": "strings", "string": "strings",
}



def build_patches():
    """Every generated patch, with its FX stage applied.

    The generators describe the SOUND; the FX chain is decided here, by group, in tools/patch_fx.py.
    Keeping the two apart means a generator does not have to know which effects its group gets, and
    the FX decision lives in one readable table instead of five."""
    import patch_fx

    patches = []
    seen_in_group = {}
    for name in GENERATORS:
        for patch in importlib.import_module(name).build():
            group = CATEGORY_MAP.get(patch.category)
            if group is None:
                raise SystemExit(f"unmapped category '{patch.category}' on '{patch.name}'")
            # Position within the group decides which FX variant the patch gets, so consecutive
            # patches in the menu are processed differently.
            position = seen_in_group.get(group, 0)
            seen_in_group[group] = position + 1
            patch_fx.apply_fx(patch, group, position)
            patches.append(patch)
    return patches


def spec_block(text):
    m = re.search(r"(kPresetSpecs\[kNumPresets\]\s*=\s*\{)(.*?)(\n\};)", text, re.S)
    if not m:
        raise SystemExit("cannot find kPresetSpecs")
    return m


def delta_block(text):
    m = re.search(r"(kPresetDeltas\[\]\s*=\s*\{)(.*?)(\n\};)", text, re.S)
    if not m:
        raise SystemExit("cannot find kPresetDeltas")
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    text = SOURCE.read_text()

    if BEGIN_NEW in text:
        # Idempotent: strip both blocks (specs and deltas) rather than refusing, so re-applying
        # after a change to the FX table or the grouping is one command instead of a manual edit.
        # The two blocks are matched INDEPENDENTLY -- a single find() pair spanning the first
        # BEGIN to the last END swallows everything between them, which is most of the file.
        while BEGIN_NEW in text:
            i = text.find(BEGIN_NEW)
            j = text.find(END_NEW, i)
            if j == -1:
                raise SystemExit("a MAGS block has no END marker; refusing to guess its extent")
            end_of_line = text.find("\n", j)
            if end_of_line == -1:
                end_of_line = len(text)
            text = text[:i] + text[end_of_line + 1:]
        text = re.sub(r"\n{3,}", "\n\n", text)
        print("  removed the existing MAGS blocks; re-applying")

    patches = build_patches()
    unknown = sorted({p.category for p in patches} - set(CATEGORY_MAP))
    if unknown:
        raise SystemExit(f"unmapped new categories: {unknown}")

    # --- existing presets, and how many there are ------------------------------------------
    sm = spec_block(text)
    existing = re.findall(r'\{"([^"]+)",\s*(\d+),\s*"([^"]*)",\s*"([^"]*)"\}', sm.group(2))
    n_existing = len(existing)
    print(f"  existing presets: {n_existing}")

    # --- regroup the existing 109's categories ----------------------------------------------
    #
    # Preset names that are textures and effects rather than strings. The original library put both
    # under one "strings/fx" category, and once the groups separated the textures must follow the
    # FX side: a crushed texture is not a sustaining string, and the arrangement palette expects an
    # sfx entry it was not getting.
    FX_NAME_TOKENS = ("texture", "crush", "riser", "sweep", "noise", "drone", "motion", "swell")

    def is_fx_preset(preset_name):
        """True when a preset's name says it is a texture or effect rather than an instrument."""
        low = preset_name.lower()
        return any(tok in low for tok in FX_NAME_TOKENS)

    def regroup_spec(match):
        name, family, source, category = match.group(1), match.group(2), match.group(3), match.group(4)
        if category == "retired drums":
            return f'{{"{name}", {family}, "{source}", "retired drums"}}'
        # The FX test comes FIRST, and it has to.
        #
        # It was below the already-grouped return, which made it dead code: the original
        # "strings/fx" category held strings AND textures under one name, the first run mapped the
        # whole thing to "strings", and every later run then matched "strings" in GROUP_ORDER and
        # returned before reaching this test. So "Skew Texture" and "Heavy Crush" sat in strings
        # while two gates correctly said a crushed texture is not a sustaining string.
        #
        # Asking the NAME rather than the current category is also what makes the pass idempotent:
        # a texture is a texture whatever the row currently says.
        if category in ("strings/fx", "strings") and is_fx_preset(name):
            return f'{{"{name}", {family}, "{source}", "sfx"}}'
        # An already-grouped category maps to itself, so re-running after an interrupted apply is
        # safe. Without this, a second run fails on 'polysynth' because the first run already
        # renamed the row -- which is exactly what happened once.
        if category in GROUP_ORDER:
            return f'{{"{name}", {family}, "{source}", "{category}"}}'
        new = CATEGORY_MAP.get(category)
        if new is None:
            raise SystemExit(f"existing category '{category}' (preset '{name}') has no group")
        return f'{{"{name}", {family}, "{source}", "{new}"}}'

    new_specs_body = re.sub(r'\{"([^"]+)",\s*(\d+),\s*"([^"]*)",\s*"([^"]*)"\}',
                            regroup_spec, sm.group(2))
    changed = sum(1 for a, b in zip(re.findall(r'"([^"]*)"\}', sm.group(2)),
                                    re.findall(r'"([^"]*)"\}', new_specs_body)) if a != b)
    print(f"  existing categories regrouped: {changed}")

    # --- append the 190 specs ---------------------------------------------------------------
    lines = ["", BEGIN_NEW,
             f"  // The MAGS library: {len(patches)} patches, grouped by role rather than by",
             "  // emulated family. Measured level trims for these are emitted below once",
             "  // tools/measure_preset_levels.py has run against them.",
             "  //"]
    by_group = {}
    for p in patches:
        by_group.setdefault(CATEGORY_MAP[p.category], []).append(p)
    index = n_existing
    for group in GROUP_ORDER:
        for p in by_group.get(group, []):
            lines.append(f'  {{"{p.name}", 0, "mags", "{group}"}},')
            index += 1
    lines.append(f"  {END_NEW}")
    appended_specs = "\n".join(lines)

    # --- append the 190 deltas --------------------------------------------------------------
    dlines = ["", BEGIN_NEW]
    index = n_existing
    for group in GROUP_ORDER:
        if not by_group.get(group):
            continue
        dlines.append(f"  // {group}")
        for p in by_group[group]:
            items = []
            for key, value in p.deltas.items():
                v = value
                vs = str(int(v)) if float(v).is_integer() else str(v)
                items.append(f"{{{index}, {key}, {vs}}}")
            # three per line, matching the existing style
            for i in range(0, len(items), 3):
                dlines.append("  " + ",   ".join(items[i:i + 3]) + ",")
            index += 1
    dlines.append(f"  {END_NEW}")
    appended_deltas = "\n".join(dlines)

    print(f"  new specs:  {len(patches)}")
    print(f"  new deltas: {sum(len(p.deltas) for p in patches)}")
    print()
    print("  groups, as the menu will show them:")
    from collections import Counter
    total = Counter()
    for name, family, source, category in existing:
        # Already-grouped rows map to themselves, so a re-apply does not fail here. Same rule as
        # regroup_spec above -- this is the second place that needed it.
        # Third place in this file that has to accept an already-final group name, and the second
        # time it was missed. The rule is the same everywhere: a name the table already carries is
        # legal and maps to itself.
        if category in GROUP_ORDER or category in ("retired drums",):
            total[category] += 1
        else:
            total[CATEGORY_MAP[category]] += 1
    for p in patches:
        total[CATEGORY_MAP[p.category]] += 1
    for g in GROUP_ORDER:
        if total[g]:
            print(f"     {g:14s} {total[g]}")
    print(f"     {'TOTAL':14s} {sum(total.values())}")

    if not args.write:
        print()
        print("  dry run. pass --write to apply.")
        return 0

    text = (text[:sm.start(2)] + new_specs_body + appended_specs + text[sm.end(2):])
    # recompute the delta match, the earlier offsets moved
    dm = delta_block(text)
    text = text[:dm.start(2)] + dm.group(2) + appended_deltas + text[dm.end(2):]
    SOURCE.write_text(text)
    print()
    print(f"  wrote {SOURCE}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
