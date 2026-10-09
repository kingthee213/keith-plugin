"""Option B from mockup-states.png with one change: the OSC block is drawn in detail.

Every other block stays flat exactly as B has it, because only the oscillator module is being
worked on. Geometry, colours and the two flat rows come from tools/mockup_states.py.

    ~/.venvs/keith-measure/bin/python tools/osc_wireframe_in_B.py

Writes design/screenshots/osc-wireframe-B.png
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mockup_states as ms  # noqa: E402

OUT = ms.ROOT / "design" / "screenshots" / "osc-wireframe-B.png"

PANEL = (22, 25, 29)
CELL = (18, 20, 24)
BODY = (62, 68, 78)
BODY_HI = (98, 108, 122)
CAP = (206, 90, 140)
SCOPE = (16, 34, 40)
CURVE = (120, 220, 200)
INK = (236, 231, 216)
DIM = (140, 146, 156)
SW_ON = (232, 79, 163)
SW_OFF = (48, 53, 61)

COLUMNS = [
    ("OSC 1", "SINE", 1.0, [("SHAPE", "SINE"), ("TUNE", "0.00"), ("LEVEL", "100"), ("PW", "50")],
     "SYNC", True),
    ("OSC 2", "PULSE", 0.0, [("SHAPE", "PULSE"), ("TUNE", "-12.0"), ("LEVEL", "75"), ("PW", "35")],
     "RING", False),
    ("MIX", None, 0.0, [("SUB", "0.0"), ("NOISE", "0.0"), ("FM", "0.0")], "FM", True),
]


def fnt(s):
    return ms.fnt(s)


def text(d, xy, s, size=12, fill=INK, anchor="la"):
    ms.text(d, xy, s, size, fill, anchor)


def knob(d, cx, cy, r):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=BODY, outline=BODY_HI, width=2)
    import math
    a = math.radians(-55)
    d.line([cx, cy, cx + r * 0.62 * math.cos(a), cy + r * 0.62 * math.sin(a)], fill=CAP, width=3)


def wave(d, x, y, w, h, cycles):
    d.rectangle([x, y, x + w, y + h], fill=SCOPE, outline=BODY_HI)
    import math
    pts = [(x + 2 + i, y + h / 2 - math.sin(i / max(1, w - 4) * math.pi * 2 * cycles) * h * 0.32)
           for i in range(0, int(w) - 4)]
    d.line(pts, fill=CURVE, width=2)


def osc_module(d, x, y, w, h):
    d.rounded_rectangle([x, y, x + w, y + h], radius=5, fill=PANEL, outline=(40, 44, 52))
    gut = 12
    colw = (w - 20 - 2 * gut) / 3
    for i, (name, shape, cycles, knobs, sw, on) in enumerate(COLUMNS):
        cx = x + 10 + i * (colw + gut)
        text(d, (cx, y + 6), name, 12, INK)
        if shape:
            wave(d, cx, y + 22, colw, 38, cycles)
            text(d, (cx + colw / 2, y + 41), shape, 11, INK, "mm")
        else:
            d.rectangle([cx, y + 22, cx + colw, y + 60], fill=CELL, outline=BODY_HI)
            text(d, (cx + colw / 2, y + 41), "SUB / NOISE / FM", 10, DIM, "mm")
        step = colw / len(knobs)
        for j, (lab, _val) in enumerate(knobs):
            kx = cx + step * (j + 0.5)
            knob(d, kx, y + 84, 15)
            text(d, (kx, y + 103), lab, 10, INK, "mm")
        d.rounded_rectangle([cx, y + h - 22, cx + 24, y + h - 2], radius=3,
                            fill=SW_ON if on else SW_OFF)
        text(d, (cx + 32, y + h - 12), sw, 11, INK, "lm")


def band(d, x, y, w, title, names):
    d.rectangle([x, y, x + w, y + ms.ROW_H], fill=ms.ROW_FILL)
    text(d, (x + 10, y + 12), title, 14)
    cx = x + 98
    for i, name in enumerate(names):
        ms.chip(d, cx, y + 3, name, i == 0)
        cx += 100
    return x + 10, y + 33, w - 20, ms.ROW_H - 41


canvas = ms.Image.new("RGB", (ms.W, ms.H), ms.BG)
d = ms.ImageDraw.Draw(canvas)
ms.chrome(d)
x = ms.RAIL + ms.GAP
w = ms.W - ms.M - x
y = ms.M + ms.TOP_H + ms.GAP

mx, my, mw, mh = band(d, x, y, w, "SOURCE", ["OSC", "FILTER", "AMP"])
osc_module(d, mx, my, mw, mh)

y += ms.ROW_H + ms.GAP
d.rectangle([x, y, x + w, y + ms.ROW_H], fill=ms.ROW_FILL)
text(d, (x + 10, y + 12), "SHAPE", 14)
ms.chip(d, x + 98, y + 3, "LFO", True)
ms.chip(d, x + 198, y + 3, "MOD MATRIX", False)
ms.chip(d, x + 298, y + 3, "AMP MODS", False)
hw = int(w * 0.42)
d.rectangle([x + 10, y + 33, x + 10 + hw, y + ms.ROW_H - 8], fill=ms.HERO)
text(d, (x + 10 + hw // 2, y + 33 + (ms.ROW_H - 41) // 2), "HERO CITY", 20, INK, "mm")
text(d, (x + 10 + hw // 2, y + 33 + (ms.ROW_H - 41) // 2 + 26), "in the row", 13, DIM, "mm")
bx = x + 10 + hw + ms.GAP
d.rectangle([bx, y + 33, x + w - 10, y + ms.ROW_H - 8], fill=ms.MODULE2)
text(d, ((bx + x + w - 10) // 2, y + 33 + (ms.ROW_H - 41) // 2), "LFO  module controls", 16, INK, "mm")

y += ms.ROW_H + ms.GAP
ms.row(d, x, y, w, "SPACE", ["INSERTS", "VINTAGE", "GLOBAL"], ms.MODULE3)

ms.pads_and_keys(d, x, ms.H - ms.M - ms.BOT_H, w, ms.BOT_H)
text(d, (x, ms.H - 22), "B, with the OSC block drawn in detail and every other block left flat", 13, DIM)

OUT.parent.mkdir(parents=True, exist_ok=True)
canvas.save(OUT)
osc_row = canvas.crop((ms.RAIL, 60, ms.W, 240))
osc_row.save("/tmp/oscB-row.png")
print(f"wrote {OUT} {canvas.size}")
