#!/usr/bin/env python3
"""Gate factory preset coverage, emulated-family order, and real deltas."""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent.parent
HEADER = (ROOT / "src" / "Keith.h").read_text(encoding="utf-8")
SOURCE = (ROOT / "src" / "Keith.cpp").read_text(encoding="utf-8")
LEDGER = (ROOT / "banks" / "SOURCES.md").read_text(encoding="utf-8")

ROLE_ORDER = [
    "keys",
    "electric piano",
    "clav",
    "organ",
    "bells",
    "pluck",
    "stab",
    "bass",
    "lead",
    "brass",
    "strings",
    "choir",
    "pad",
    "motion",
    "texture",
    "drone",
    "arp",
    "fx",
    "kit",
]
ROLE_INDEX = {name: index for index, name in enumerate(ROLE_ORDER)}
EXPECTED_REFS = ["Triton", "Triton Extreme", "Korg M1", "Korg Wavestation", "Korg TR"]
PALETTE_CATEGORIES = {"clav", "stab", "electric piano", "kit", "texture", "fx"}
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

    if not 80 <= len(rows) <= 120:
        fail(f"preset count {len(rows)} is outside 80-120")
    if len(rows) != declared:
        fail(f"kNumPresets is {declared} but the spec table has {len(rows)} rows")

    names = [row[0] for row in rows]
    if len(set(names)) != len(names):
        fail("preset names are not unique")
    titles = []
    for name, family, reference, category in rows:
        match = re.match(r"^Family 0([1-5]) / (\d{3}) ([A-Za-z0-9 ]+)$", name)
        if not match:
            fail(f"name breaks Family 0N / NNN <neutral role>: {name}")
            continue
        if int(match.group(1)) != int(family):
            fail(f"name family does not match the spec family: {name}")
        titles.append(match.group(3))
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

    families = [int(row[1]) for row in rows]
    if families != sorted(families):
        fail("presets are not grouped by emulated family")
    for family, reference in zip(range(1, 6), EXPECTED_REFS):
        selected = [row for row in rows if int(row[1]) == family]
        if not selected:
            fail(f"family {family} has no presets")
            continue
        if any(row[2] != reference for row in selected):
            fail(f"family {family} is not all {reference}")
        first = rows.index(selected[0])
        expected_first = sum(1 for row in rows if int(row[1]) < family)
        if first != expected_first:
            fail(f"family {family} does not start at index {expected_first}")
        present = {row[3] for row in selected}
        missing = [role for role in ROLE_ORDER if role not in present]
        if missing:
            fail(f"family {family} missing roles: {', '.join(missing)}")
        # The original catalogue stays in role order. Measured recreations are
        # appended after that catalogue, still inside the family, and do not
        # have to continue the role sequence.
        catalogue = {1: 20, 2: 21, 3: 21, 4: 20, 5: 21}[family]
        last_role = -1
        last_title = ""
        last_category = ""
        for offset, row in enumerate(selected):
            name, _, _, category = row
            title = re.match(r"^Family 0[1-5] / \d{3} (.+)$", name).group(1)
            number = int(re.search(r"/ (\d{3}) ", name).group(1))
            if number != offset + 1:
                fail(f"{name} is not numbered {offset + 1:03d} inside its family")
            if offset >= catalogue:
                continue
            role_index = ROLE_INDEX[category]
            if role_index < last_role:
                fail(f"{name} is out of sound-role order")
            if category == last_category and title < last_title:
                fail(f"{name} breaks alphabetical order inside {category}")
            last_role = role_index
            last_title = title
            last_category = category

    live = set(params)
    seen_delta = {preset: 0 for preset in range(len(rows))}
    seen_pair = set()
    for preset, param, _value in deltas:
        if param not in live:
            fail(f"delta targets unknown parameter {param}")
        if preset < 0 or preset >= len(rows):
            fail(f"delta targets missing preset {preset}")
        else:
            seen_delta[preset] += 1
        pair = (preset, param)
        if pair in seen_pair:
            fail(f"duplicate delta for preset {preset} {param}")
        seen_pair.add(pair)
    for preset, count in seen_delta.items():
        if count < 1:
            fail(f"preset {preset} has no deltas")

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

    for name in names:
        if name not in LEDGER:
            fail(f"SOURCES.md has no row for {name}")
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
