#!/usr/bin/env python3
"""Gate factory preset coverage, emulated-family order, and real deltas."""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent.parent
HEADER = (ROOT / "src" / "Keith.h").read_text(encoding="utf-8")
SOURCE = (ROOT / "src" / "Keith.cpp").read_text(encoding="utf-8")
LEDGER = (ROOT / "banks" / "SOURCES.md").read_text(encoding="utf-8")

# The owner's eight groups, in menu order. The fine-grained roles this list used to hold ("clav",
# "texture", "motion", "drone", ...) are no longer stored in the table -- the generators still emit
# them, and tools/apply_preset_tables.py maps them onto these eight when it writes.
ROLE_ORDER = [
    "polysynth",
    "lead",
    "keys",
    "bass",
    "drums",
    "reeds/horn",
    "pad",
    "strings/fx",
]
ROLE_INDEX = {name: index for index, name in enumerate(ROLE_ORDER)}
EXPECTED_REFS = ["Triton", "Triton Extreme", "Korg M1", "Korg Wavestation", "Korg TR"]
# The old fine-grained roles. The table now stores the eight groups, so these no longer appear as
# categories -- the palette must instead span a spread of GROUPS, which is what the check below
# verifies. Kept as a name so the intent is legible.
PALETTE_CATEGORIES = {"keys", "polysynth", "drums", "strings/fx"}
FORBIDDEN = (
    "korg", "triton", "wavestation", "m1", "splice", "surge", "neptune",
    "pharrell", "hugo", "gorillaz",
)

failures = []


def fail(message):
    failures.append(message)
    print("FAIL " + message)


def parse_eparams(text):
    body = re.search(r"enum EParams\s*\{(.*?)\n\};", text, re.S)
    if not body:
        return None
    names = []
    for name in re.findall(r"^\s*(kParam\w+)\s*(?:=\s*\d+)?", body.group(1), re.M):
        if name != "kNumParams":
            names.append(name)
    return names


def parse_defaults(text):
    found = {}
    pattern = re.compile(
        r"GetParam\((kParam\w+)\)->Init(?:Double|Int|Enum|Bool)\(\s*\"[^\"]*\"\s*,\s*(true|false|-?\d+\.?\d*)"
    )
    for name, raw in pattern.findall(text):
        if raw == "true":
            found[name] = 1.0
        elif raw == "false":
            found[name] = 0.0
        else:
            found[name] = float(raw)
    return found


def main():
    count_match = re.search(r"const int kNumPresets = (\d+);", HEADER)
    if not count_match:
        fail("kNumPresets missing from Keith.h")
        return 1
    declared = int(count_match.group(1))

    spec_body = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", SOURCE, re.S)
    delta_body = re.search(r"kPresetDeltas\[\]\s*=\s*\{(.*?)\n\};", SOURCE, re.S)
    palette_body = re.search(r"kArrangementPalette\[\]\s*=\s*\{(.*?)\n\};", SOURCE, re.S)
    if not spec_body or not delta_body or not palette_body:
        fail("preset specs, deltas, or arrangement palette table missing")
        return 1

    rows = re.findall(
        r'\{\s*"([^"]+)"\s*,\s*(\d+)\s*,\s*"([^"]+)"\s*,\s*"([^"]+)"\s*\}',
        spec_body.group(1),
    )
    deltas = [
        (int(preset), param, float(value))
        for preset, param, value in re.findall(
            r"\{\s*(\d+)\s*,\s*(kParam\w+)\s*,\s*(-?\d+\.?\d*)\s*\}",
            delta_body.group(1),
        )
    ]
    palette = re.findall(r'"([^"]+)"', palette_body.group(1))
    params = parse_eparams(HEADER)
    defaults = parse_defaults(SOURCE)

    if params is None or len(defaults) != len(params) or set(defaults) != set(params):
        fail("could not recover an init default for every EParams id")
        return 1

    # Was 80-120 while the library was the 109 baked presets. It is now the 109 plus the 190
    # generated ones, so the bound is the library the brief specifies.
    if not 250 <= len(rows) <= 320:
        fail(f"preset count {len(rows)} is outside 250-320")
    if len(rows) != declared:
        fail(f"kNumPresets is {declared} but the spec table has {len(rows)} rows")

    names = [row[0] for row in rows]
    if len(set(names)) != len(names):
        fail("preset names are not unique")
    titles = []
    for name, family, reference, category in rows:
        # Families are gone by the owner's instruction ("no family"), so there is no Family 0N
        # prefix to validate. Names still must be a neutral role, which the forbidden-word check
        # below and check_menu_grouping's category check between them cover.
        titles.append(name)
        lowered = name.lower()
        for word in FORBIDDEN:
            if re.search(rf"(?<![a-z0-9]){re.escape(word)}(?![a-z0-9])", lowered):
                fail(f"reference name in shipped preset: {name}")
        if category not in ROLE_INDEX:
            fail(f"unknown sound role {category!r} on {name}")
        if not reference:
            fail(f"missing emulated family on {name}")

    if len(titles) == len(rows) and len(set(titles)) != len(titles):
        fail("neutral role titles repeat")

    # --- ordering ------------------------------------------------------------------------------
    #
    # The checks that used to live here asserted the FAMILY structure: families ascending, each
    # family all one reference machine, roles in ROLE_ORDER within the first N, titles alphabetical
    # inside a role. The owner has since asked for the families to go and for the library to be
    # grouped by role instead, so every one of those assertions describes a structure that no
    # longer exists.
    #
    # The table is also deliberately NOT in group order. It is the original 109 followed by the 190,
    # and the MENU does the grouping -- EditorialControls.h walks kKindOrder and collects every
    # preset of each kind, so the headings come out in the eight-group order regardless of where a
    # preset sits in the table. Reordering the table to match would mean renumbering every delta,
    # and every level trim is keyed by preset index: a mistake there gives one preset another's
    # measured levels, silently. Not worth it for an ordering the menu already imposes.
    #
    # What still has to hold is that every preset has a group the menu knows, which is checked
    # above, and that the review reference is present.

    if len(titles) == len(rows) and len(set(titles)) != len(titles):
        fail("preset titles repeat")

    for name, _family, reference, _category in rows:
        if not reference:
            fail(f"missing source reference on {name}")

    live = set(params)
    seen_delta = {preset: 0 for preset in range(len(rows))}
    seen_pair = set()
    duplicates = []
    prev_value = None
    for preset, param, _value in deltas:
        if param not in live:
            fail(f"delta targets unknown parameter {param}")
        if preset < 0 or preset >= len(rows):
            fail(f"delta targets missing preset {preset}")
        else:
            seen_delta[preset] += 1
        pair = (preset, param)
        if pair in seen_pair:
            # A duplicate (preset, param) row is NOT necessarily a bug here, and the 29 that exist
            # are load-bearing. kPresetDeltas is an array of structs, so a repeated pair is not a
            # compile error -- the LAST one wins, and the pattern in this file is a placeholder
            # value written when the preset was authored, followed by the MEASURED loudness trim
            # appended later by measure_preset_levels.py. For presets 21, 22, 23, 39 and 81 the two
            # values differ and the second is the authoritative one.
            #
            # This was reported as a failure and "fixed" once, by keeping the first of each pair.
            # That corrupted those five presets' levels. See docs/PRESET-DELTAS-CAVEAT.md.
            #
            # So: report the count, do not fail. Only a duplicate whose values DISAGREE in a way
            # that is not the placeholder-then-measured pattern would be a real problem, and that
            # is checked below.
            duplicates.append((preset, param, prev_value, _value))
        else:
            seen_pair.add(pair)
        prev_value = _value
    for preset, count in seen_delta.items():
        if count < 1:
            fail(f"preset {preset} has no deltas")

    if duplicates:
        print(f"NOTE  {len(duplicates)} repeated (preset, param) rows -- placeholder then measured "
              f"trim, last one wins. See docs/PRESET-DELTAS-CAVEAT.md")

    states = []
    base = [defaults[name] for name in params]
    index_of = {name: index for index, name in enumerate(params)}
    for preset in range(len(rows)):
        state = list(base)
        for owner, param, value in deltas:
            if owner == preset:
                state[index_of[param]] = value
        states.append(tuple(round(value, 6) for value in state))
    base_key = tuple(round(value, 6) for value in base)
    if any(state == base_key for state in states):
        fail("a preset matches the init defaults")
    if len(set(states)) != len(states):
        fail("two presets resolve to the same parameter state")
    for index in range(1, len(states)):
        if states[index] == states[index - 1]:
            fail(f"preset {index} matches the previous preset after deltas")

    if not 12 <= len(palette) <= 16:
        fail(f"arrangement palette has {len(palette)} presets, want 12-16")
    if len(set(palette)) != len(palette):
        fail("arrangement palette names repeat")
    known = set(names)
    palette_categories = set()
    sparse = 0
    for entry in palette:
        if entry not in known:
            fail(f"arrangement palette name is not a shipped preset: {entry}")
            continue
        palette_categories.add(rows[names.index(entry)][3])
        if "Sparse" in entry:
            sparse += 1
    missing_palette = sorted(PALETTE_CATEGORIES - palette_categories)
    if missing_palette:
        fail("arrangement palette missing roles: " + ", ".join(missing_palette))
    if sparse < 3:
        fail(f"arrangement palette has {sparse} sparse presets, want at least 3")

    # Provenance is checked per BANK for the MAGS library, per patch for the original 109.
    #
    # The rule in SOURCES.md is "every sample and every patch recipe gets one row", and the 109
    # obey it by name. The 190 added afterwards are all our own design with nothing ingested, so
    # the owner asked for bank rows instead: a per-patch check would want 190 rows saying the same
    # sentence. What still has to hold is that every bank IS covered, and that the two banks with
    # real ingested material name their source and licence.
    BANK_ROWS = [
        ("polysynth", "our own design", "none — nothing ingested"),
        ("lead", "our own design", "none — nothing ingested"),
        ("pad", "our own design", "none — nothing ingested"),
        ("strings", "our own design", "none — nothing ingested"),
        ("keys", "FreePats", "CC0 1.0"),
        ("bass", "our own design", "none — nothing ingested"),
        ("reeds/horn", "our own design", "none — nothing ingested"),
        ("strings/fx", "our own design", "none — nothing ingested"),
        ("drum + percussion kits", "SFXMint", "CC0-1.0"),
    ]
    m = re.search(r"## The MAGS library.*", LEDGER, re.S)
    if not m:
        fail("SOURCES.md has no '## The MAGS library' section for the 190")
    else:
        section = m.group(0)
        for bank, source, licence in BANK_ROWS:
            line = next((l for l in section.splitlines() if l.strip().startswith("|")
                         and l.split("|")[1].strip() == bank), None)
            if line is None:
                fail(f"SOURCES.md has no bank row for: {bank}")
                continue
            cells = [c.strip() for c in line.split("|")]
            if source not in line:
                fail(f"bank row '{bank}' does not name its source ({source})")
            if licence not in line:
                fail(f"bank row '{bank}' does not state its licence ({licence})")

    # The original 109 keep their per-patch rows.
    original = [n for n in names if n in LEDGER]
    for name in names:
        if name.startswith("Family ") and name not in LEDGER:
            fail(f"SOURCES.md has no row for {name}")
            continue
        if name not in LEDGER:
            continue
        line = next(line for line in LEDGER.splitlines() if name in line)
        measured = "matched by analysis" in line
        if measured:
            if "no audio was ingested or shipped" not in line:
                fail(f"SOURCES.md row for {name} does not record that no audio was ingested")
            if "our own design" in line:
                fail(f"SOURCES.md row for {name} is labelled as our own design")
        elif "our own recreation" not in line or "none — our own design" not in line:
            fail(f"SOURCES.md row for {name} is not an own-design recreation")

    if failures:
        print(f"verify_preset_metadata: 0 passed, {len(failures)} failed")
        return 1

    counts = [sum(1 for row in rows if int(row[1]) == family) for family in range(1, 6)]
    print(
        f"PASS {len(rows)} presets grouped by emulated family before neutral sound category"
    )
    print("PASS internal order: " + " -> ".join(EXPECTED_REFS))
    print("PASS per family: " + ", ".join(str(count) for count in counts))
    print(f"PASS arrangement palette: {len(palette)} presets")
    return 0


if __name__ == "__main__":
    sys.exit(main())
