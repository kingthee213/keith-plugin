#!/usr/bin/env python3
"""Regenerate the preset-library table in docs/MANUAL.md from src/Keith.cpp.

The manual lists all 103 presets with the insert and master FX chain each one
preloads. Writing that by hand would rot the moment a chain changed, so it is
generated between the markers below and this script is idempotent: run it after
touching kPresetSpecs or kPresetDeltas and commit the diff.

Usage:
  python tools/gen_manual_preset_table.py          # rewrite the table
  python tools/gen_manual_preset_table.py --check  # fail if the table is stale
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SOURCE_PATH = ROOT / "src" / "Keith.cpp"
MANUAL_PATH = ROOT / "docs" / "MANUAL.md"

BEGIN = "<!-- BEGIN GENERATED PRESET TABLE -->"
END = "<!-- END GENERATED PRESET TABLE -->"

INSERT_ALGOS = ["Off", "Drive", "Chorus", "Delay", "Crush"]
MASTER_ALGOS = ["Off", "Comp", "Limit", "Width", "Tilt"]


def parse(source):
    deltas = {}
    body = re.search(r"kPresetDeltas\[\]\s*=\s*\{(.*?)\n\};", source, re.S)
    specs = re.search(r"kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};", source, re.S)
    if not body or not specs:
        sys.exit("could not find kPresetDeltas / kPresetSpecs in src/Keith.cpp")

    for preset, param, value in re.findall(
            r"\{\s*(\d+)\s*,\s*(kParam\w+)\s*,\s*(-?[\d.]+)\s*\}", body.group(1)):
        deltas.setdefault(int(preset), {})[param] = float(value)

    # The third spec field is the internal reference-machine name and must not be
    # published; banks/SOURCES.md is the only place it lives.
    rows = re.findall(r'\{\s*"([^"]+)"\s*,\s*(\d+)\s*,\s*"[^"]+"\s*,\s*"([^"]+)"\s*\}',
                      specs.group(1))
    return rows, deltas


def stage(written, prefix, algos):
    algo = int(written.get(f"kParam{prefix}Algo", 0))
    if algo == 0:
        return "Off"
    return f"{algos[algo]} {written.get(f'kParam{prefix}Mix', 0.):g}%"


def render(rows, deltas):
    lines = []
    for family in sorted({int(row[1]) for row in rows}):
        lines += ["", f"### Family 0{family}", "",
                  "| # | Preset | Role | Insert 1 | Insert 2 | Master |",
                  "|---:|---|---|---|---|---|"]
        for index, (name, spec_family, role) in enumerate(rows):
            if int(spec_family) != family:
                continue
            written = deltas.get(index, {})
            lines.append(
                f"| {index} | {name.split(' / ', 1)[1]} | {role} "
                f"| {stage(written, 'Ins1', INSERT_ALGOS)} "
                f"| {stage(written, 'Ins2', INSERT_ALGOS)} "
                f"| {stage(written, 'Mst', MASTER_ALGOS)} |")
    lines.append("")
    return "\n".join(lines)


def main(argv):
    rows, deltas = parse(SOURCE_PATH.read_text(encoding="utf-8"))
    manual = MANUAL_PATH.read_text(encoding="utf-8")
    pattern = re.compile(f"{re.escape(BEGIN)}.*?{re.escape(END)}", re.S)
    if not pattern.search(manual):
        sys.exit(f"docs/MANUAL.md is missing the {BEGIN} / {END} markers")

    block = f"{BEGIN}\n{render(rows, deltas)}\n{END}"
    updated = pattern.sub(lambda _: block, manual)

    if "--check" in argv:
        if updated != manual:
            print("FAIL docs/MANUAL.md preset table is stale; "
                  "run python tools/gen_manual_preset_table.py")
            return 1
        print(f"PASS docs/MANUAL.md preset table matches {len(rows)} presets in src/Keith.cpp")
        return 0

    MANUAL_PATH.write_text(updated, encoding="utf-8")
    print(f"wrote {len(rows)} preset rows into docs/MANUAL.md"
          + ("" if updated != manual else " (no change)"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
