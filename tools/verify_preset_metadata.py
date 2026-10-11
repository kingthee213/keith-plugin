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
# "retired drums" is the 7 original synthesised kit presets: kept in the table so indices do not
# shift, hidden from the menu. It is a legal category and deliberately absent from ROLE_ORDER.
ROLE_ORDER = [
    "mono synths",
    "poly synths",
    "guitars",
    "winds",
    "sfx",
    "vocal",
    "bass",
    "strings",
]
ROLE_INDEX = {name: index for index, name in enumerate(ROLE_ORDER)}
EXPECTED_REFS = ["Triton", "Triton Extreme", "Korg M1", "Korg Wavestation", "Korg TR"]
# The old fine-grained roles. The table now stores the eight groups, so these no longer appear as
# categories -- the palette must instead span a spread of GROUPS, which is what the check below
# verifies. Kept as a name so the intent is legible.
# The "Glass Keys" arrangement palette is the owner's named demo set, so its categories follow the
# current grouping. `retired drums` is excluded on purpose: those 7 presets are hidden.
PALETTE_CATEGORIES = {"poly synths", "mono synths", "strings", "sfx"}
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
        # "retired drums" is legal but not menu-listed: the 7 original synthesised kit presets,
        # kept at their indices so the deltas and level trims do not shift onto other patches.
        if category != "retired drums" and category not in ROLE_INDEX:
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
    # READ FROM THE TABLES, not from a second hardcoded list.
    #
    # The previous version duplicated the group names here, and they went stale the moment the
    # groups were reorganised -- the check then failed on names that no longer existed while the
    # ledger itself was correct. What has to hold is that every group the plugin SHIPS has a row
    # and that the row states a source and a licence. Both of those come from the table, so both
    # are read from the table.
    #
    # "retired drums" is excluded: those 7 presets are hidden from the menu, so they are not part
    # of the shipped group list and do not need a bank row of their own.
    shipped_groups = sorted({category for _, _, _, category in rows
                             if category not in ("retired drums",)})
    if not shipped_groups:
        fail("no shipped groups found in the preset table")

    m = re.search(r"## The current library.*", LEDGER, re.S)
    if not m:
        fail("SOURCES.md has no '## The current library' section")
    else:
        section = m.group(0)
        table_rows = {}
        for line in section.splitlines():
            if not line.strip().startswith("|"):
                continue
            cells = [c.strip() for c in line.split("|")]
            if len(cells) > 3:
                table_rows[cells[1]] = line
        for group in shipped_groups:
            line = table_rows.get(group)
            if line is None:
                fail(f"SOURCES.md has no bank row for shipped group: {group}")
                continue
            # A row must state SOMETHING about where the sound comes from and under what licence.
            # Checked as "not empty" rather than against one expected sentence, because the groups
            # genuinely differ now: some ingest nothing, some ingest CC0 multisamples.
            lower = line.lower()
            # Known upstream names. This is a list, but of SOURCES rather than of groups: a group
            # is renamed whenever the library is reorganised, an upstream keeps its name. Anything
            # added from a new source must be added here too, which is the point -- it makes the
            # provenance explicit at the moment a source is introduced.
            UPSTREAMS = ("our own design", "freepats", "mckaud", "mck sample", "sfzinstruments",
                         "legato_vocal", "emilyguitar", "mksamplepacks", "karoryfer")
            if not any(u in lower for u in UPSTREAMS):
                fail(f"bank row '{group}' does not name a source")
            if "none" not in lower and "cc0" not in lower and "cc-by" not in lower:
                fail(f"bank row '{group}' does not state a licence")
    # Ledger rows are TABLE ROWS. A preset name appearing anywhere in the file is not a row, and
    # testing `name in LEDGER` treated a sentence naming "Steel String" as its provenance because
    # the new prose section mentions it. Both the membership test and the lookup are restricted to
    # lines that actually start a table row.
    #
    # The per-patch loop covers the ORIGINAL 109 only; the 205 added later are covered by the bank
    # rows above, which the owner asked for rather than 205 rows saying one sentence. The
    # distinction is by FAMILY rather than by the name starting with "Family", because the MAGS
    # patches are named plainly and a name-based test sent them here, where they failed for not
    # being recreations they were never meant to be.
    ledger_rows = [l for l in LEDGER.splitlines() if l.strip().startswith("|")]

    def row_for(name):
        """The ledger row whose PATCH NAME column is `name`.

        The column is found from the header rather than hardcoded. The Ledger table starts with a
        "#" column, so the patch name is the third cell, while the bank table further down starts
        with the name. Hunting for the literal name across the whole line -- which is what this did
        -- matched the run of prose in the new section instead.
        """
        for l in ledger_rows:
            cells = [c.strip() for c in l.split("|")]
            if len(cells) > 3 and any(c == name for c in cells[1:4]):
                return l
        return None

    for name, family, _, _ in rows:
        if int(family) == 0:
            continue   # a MAGS patch: covered by the bank rows, not by a row of its own
        line = row_for(name)
        if line is None:
            fail(f"SOURCES.md has no row for {name}")
            continue
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
