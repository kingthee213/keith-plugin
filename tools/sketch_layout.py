"""Draw a panel, or the whole canvas, as text so a layout can be argued about before it is built.

Reads the real rectangles from src/editor/EditorGeometry.h, so a sketch is the truth rather than a
drawing of an intention. Every control becomes a cell of its true size on a character grid, filled
with its own label.

    ~/.venvs/keith-measure/bin/python tools/sketch_layout.py                 # the whole canvas
    ~/.venvs/keith-measure/bin/python tools/sketch_layout.py kPanelPads      # one panel

Options:
    --scale 6,12     pixels per character column and row (default 6 by 12)
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"

PANEL_TITLES = {
    "kPanelOsc": "OSCILLATORS",
    "kPanelFilter": "FILTER",
    "kPanelAmp": "AMP",
    "kPanelLfo": "LFO",
    "kPanelMod": "MOD MATRIX",
    "kPanelAmpMods": "AMP MODS",
    "kPanelSample": "SAMPLED",
    "kPanelPads": "DRUMS",
    "kPanelVintage": "VINTAGE",
    "kPanelGlobal": "GLOBAL",
    "kPanelFx": "EFFECTS",
}

KIND_MARK = {"knob": "o", "tab": "=", "dropdown": "v", "fader": "|", "pad": "#"}


def parse_boxes():
    text = GEOM.read_text(errors="replace")
    boxes = {}
    for name, numbers in re.findall(r"constexpr Box (k\w+) \{([^}]*)\};", text):
        parts = [v.strip().rstrip("f") for v in numbers.split(",") if v.strip()]
        if len(parts) != 4 or not all(re.fullmatch(r"-?\d+(\.\d+)?", p) for p in parts):
            continue
        boxes[name] = tuple(float(p) for p in parts)
    return boxes


def parse_controls():
    text = GEOM.read_text(errors="replace")
    match = re.search(r"inline constexpr ControlRect kControlRects\[\] = \{(.*?)\n\};", text, re.S)
    out = []
    pattern = (r"\{\s*(\d+),\s*\{([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f\},\s*"
               r"\"([^\"]*)\",\s*([\d.]+)f,\s*\d+,\s*ControlKind::(\w+)\s*\}")
    for m in re.finditer(pattern, match.group(1)):
        out.append({
            "id": int(m.group(1)),
            "box": (float(m.group(2)), float(m.group(3)), float(m.group(4)), float(m.group(5))),
            "label": m.group(6),
            "font": float(m.group(7)),
            "kind": m.group(8).lower(),
        })
    return out


def draw(panel_name, controls, panel_name_filter, cols, rows):
    grid = [[" "] * cols for _ in range(rows)]
    ox, oy = 0.0, 0.0
    if panel_name_filter:
        b = panels[panel_name_filter]
        ox, oy = b[0] - 1, b[1] - 1

    def put(x, y, ch):
        gx, gy = int(round(x - ox)), int(round(y - oy))
        if 0 <= gy < rows and 0 <= gx < cols:
            grid[gy][gx] = ch

    for name, b in panels.items():
        if panel_name_filter and name != panel_name_filter:
            continue
        for x in range(int(b[0]), int(b[2]) + 1):
            put(x, int(b[1]), "-")
            put(x, int(b[3]), "-")
        for y in range(int(b[1]), int(b[3]) + 1):
            put(int(b[0]), y, "|")
            put(int(b[2]), y, "|")

    for c in controls:
        l, t, r, b = c["box"]
        if panel_name:
            pb = panels[panel_name]
            cx, cy = (l + r) / 2, (t + b) / 2
            if not (pb[0] <= cx <= pb[2] and pb[1] <= cy <= pb[3]):
                if c["kind"] != "pad":
                    continue
        mark = KIND_MARK.get(c["kind"], "?")
        for x in range(int(l), int(r) + 1):
            put(x, int(t), "-")
            put(x, int(b), "-")
        for y in range(int(t), int(b) + 1):
            put(int(l), y, "|")
            put(int(r), y, "|")
        label = c["label"][: max(1, int(r) - int(l) - 1)]
        start = int(l) + 1
        put(start, int(t) + 1, mark)
        for i, ch in enumerate(label):
            put(start + 1 + i, int(t) + 1, ch)
    return "\n".join("".join(row).rstrip() for row in grid)


scale_x, scale_y = 6, 12
args = [a for a in sys.argv[1:] if not a.startswith("--")]
if "--scale" in sys.argv:
    raw = sys.argv[sys.argv.index("--scale") + 1]
    scale_x, scale_y = (int(v) for v in raw.split(","))

panel = args[0] if args else None
boxes = parse_boxes()
panels = {}
for name, b in boxes.items():
    if not name.startswith("kPanel"):
        continue
    panels[name] = (b[0] / scale_x, b[1] / scale_y, b[2] / scale_x, b[3] / scale_y)

canvas_w = 1152 / scale_x
canvas_h = 768 / scale_y
controls = []
for c in parse_controls():
    l, t, r, b = c["box"]
    controls.append({**c, "box": (l / scale_x, t / scale_y, r / scale_x, b / scale_y)})

if panel:
    b = panels[panel]
    cols = int(b[2] - b[0]) + 1
    rows = int(b[3] - b[1]) + 1
else:
    cols, rows = int(canvas_w) + 1, int(canvas_h) + 1

title = panel or "whole canvas"
print(f"# {title}  cell = {scale_x}x{scale_y} px")
print(draw(panel, controls, panel, cols, rows))
