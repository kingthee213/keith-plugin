"""Quantify how far each editor panel is from fitting.

The geometry test checks that a control's rectangle holds its label. It passes. The running window
still collides, because what the plugin actually draws is the label AND the parameter value, and the
value is usually the wider of the two. The gate was measuring half the requirement, which is why a
layout that fails on screen reports clean.

This measures the whole requirement. For every control rectangle in EditorGeometry.h it takes the
label and the worst-case value string for that parameter's units, converts both to a text width with
Space Mono's 0.6 em advance, and reports where the wider of the two does not fit.

Output is one table per panel plus a total, so the fix has a size rather than a feeling.

Usage: python3 tools/fit_budget.py
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
MANIFEST = ROOT / "docs" / "PARAM-MANIFEST.md"

ADVANCE_EM = 0.60  # Space Mono and Space Mono Bold both advance 0.6 em per character.


def parse_geometry():
    text = GEOM.read_text(errors="replace")
    panels = {}
    for name, numbers in re.findall(r"constexpr Box (kPanel\w+|kWordmarkBox|kLeftRail) \{([^}]*)\};", text):
        panels[name] = tuple(float(v.replace("f", "").strip()) for v in numbers.split(","))
    items = []
    for param, l, t, r, b, label, font in re.findall(
            r"\{\s*(\d+),\s*\{([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f\},\s*\"([^\"]*)\",\s*([\d.]+)f",
            text):
        items.append({
            "param": int(param),
            "rect": (float(l), float(t), float(r), float(b)),
            "label": label,
            "font": float(font),
        })
    return panels, items


def parse_units():
    """param symbol -> (min, max, step, units), from the manifest's two tables."""
    text = MANIFEST.read_text(errors="replace")
    symbol_to_index = {}
    for symbol, index in re.findall(r"\|\s*`(kParam\w+)`\s*\|\s*(\d+)\s*\|", text):
        symbol_to_index[symbol] = int(index)

    enum_order = []
    for name in re.findall(r"^\s*(kParam\w+)\s*=\s*\d+,", (ROOT / "src" / "Keith.h").read_text(errors="replace"), re.M):
        enum_order.append(name)

    ranges = {}
    rows = re.findall(r"\|\s*`([a-z0-9_]+)`\s*\|[^|]*\|[^|]*\|\s*([^|]*?)\s*\|", text)
    id_to_range = {row[0]: row[1] for row in rows}

    # The manifest's second table carries id -> symbol -> automatable name. Join through it.
    symbol_to_id = {}
    for symbol, _index, ident in re.findall(r"\|\s*`(kParam\w+)`\s*\|\s*(\d+)\s*\|\s*[^|]*\|\s*`([a-z0-9_]+)`\s*\|", text):
        symbol_to_id[symbol] = ident

    for symbol, ident in symbol_to_id.items():
        raw = id_to_range.get(ident, "")
        head = raw.split()[0] if raw else ""
        head = head.replace("−", "-").replace("–", "-")
        numbers = [float(v) for v in re.findall(r"-?\d+\.?\d*", head)]
        units = "".join(ch for ch in raw if ch.isalpha() or ch == "%")
        if len(numbers) >= 2:
            ranges[symbol] = (numbers[0], numbers[1], units)
    return enum_order, ranges


def worst_value(symbol, table):
    entry = table.get(symbol)
    if not entry:
        return ""
    low, high, units = entry
    magnitude = max(abs(low), abs(high))
    decimals = 0 if magnitude >= 100 else 2
    text = f"{high:.{decimals}f}"
    return f"{text} {units}".strip()


def text_width(text, font_px):
    return ADVANCE_EM * font_px * len(text)


def main():
    panels, items = parse_geometry()
    enum_order, ranges = parse_units()
    if not items:
        sys.exit("no control items parsed from EditorGeometry.h")

    panel_boxes = {name: box for name, box in panels.items() if name.startswith("kPanel")}
    name_for = {f"kPanel{key}": key for key in
                ("Osc", "Filter", "Amp", "Lfo", "Mod", "Analyzer", "AmpMods", "Sample", "Pads",
                 "Vintage", "Global", "Fx")}

    grouped = {name: [] for name in panel_boxes}
    orphans = []
    for item in items:
        cx = (item["rect"][0] + item["rect"][2]) / 2
        cy = (item["rect"][1] + item["rect"][3]) / 2
        # Smallest containing panel wins. Some panels overlap by design (the analyzer and
        # AMP MODIFIERS abut), so first-match would file a control under the wrong parent
        # and report a density for a panel that does not own it.
        home = None
        home_area = None
        for name, (l, t, r, b) in panel_boxes.items():
            if l <= cx <= r and t <= cy <= b:
                area = (r - l) * (b - t)
                if home_area is None or area < home_area:
                    home, home_area = name, area
        if home is None:
            orphans.append(item)
        else:
            grouped[home].append(item)

    print(f"controls placed inside a panel: {sum(len(v) for v in grouped.values())}, "
          f"outside every panel: {len(orphans)}\n")

    total_overflow = 0
    for name in sorted(grouped, key=lambda n: -len(grouped[n])):
        bucket = grouped[name]
        if not bucket:
            continue
        l, t, r, b = panel_boxes[name]
        area = (r - l) * (b - t)
        worst = []
        for item in bucket:
            symbol = enum_order[item["param"]] if item["param"] < len(enum_order) else ""
            value = worst_value(symbol, ranges)
            need = max(text_width(item["label"], item["font"]), text_width(value, item["font"]))
            have = item["rect"][2] - item["rect"][0]
            if need > have:
                worst.append((need - have, item["label"], value, need, have))
        worst.sort(reverse=True)
        density = area / len(bucket)
        print(f"{name:16s} {len(bucket):3d} controls  {area:7.0f} px2 panel  "
              f"{density:6.0f} px2 per control  overflowing: {len(worst)}")
        for over, label, value, need, have in worst[:6]:
            print(f"    over by {over:5.1f}px  '{label}' value '{value}' "
                  f"needs {need:.1f} has {have:.0f}")
        total_overflow += len(worst)
    print(f"\ncontrols whose label or value cannot fit their own rectangle: {total_overflow}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
