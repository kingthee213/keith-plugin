"""Bento readjustment: the oscillator block goes portrait, the hero shifts over and the LFO squeezes.

Three knob columns need about 400px of width, so the block only becomes taller than wide when it
spans all three bands. It ends up 400 x 433. The hero keeps its 446 x 139 and only moves; the LFO
takes what is left of the SHAPE band.

    ~/.venvs/keith-measure/bin/python tools/osc_bento.py

Writes design/screenshots/osc-bento.png
"""

import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mockup_states as ms  # noqa: E402

OUT = ms.ROOT / "design" / "screenshots" / "osc-bento.png"

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

CONTENT_Y = 93
CONTENT_H = 139
OSC_W = 400
HERO_W = 446
BANDS = 3

COLUMNS = [
    ("OSC 1", "SINE", 1.0, [("SHAPE", "SINE"), ("TUNE", "0.00"), ("LEVEL", "100"), ("PW", "50")],
     "SYNC", True),
    ("OSC 2", "PULSE", 0.0, [("SHAPE", "PULSE"), ("TUNE", "-12.0"), ("LEVEL", "75"), ("PW", "35")],
     "RING", False),
    ("MIX", None, 0.0, [("SUB", "0.0"), ("NOISE", "0.0"), ("FM", "0.0")], "FM", True),
]

RIGHT_BANDS = [
    ("SOURCE", ["FILTER", "AMP"], "FILTER / AMP  module controls", ms.MODULE),
    ("SPACE", ["VINTAGE", "GLOBAL"], "VINTAGE / GLOBAL  module controls", ms.MODULE3),
]


def text(d, xy, s, size=12, fill=INK, anchor="la"):
    ms.text(d, xy, s, size, fill, anchor)


def knob(d, cx, cy, r):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=BODY, outline=BODY_HI, width=2)
    a = math.radians(-55)
    d.line([cx, cy, cx + r * 0.62 * math.cos(a), cy + r * 0.62 * math.sin(a)], fill=CAP, width=3)


def wave(d, x, y, w, h, cycles):
    d.rectangle([x, y, x + w, y + h], fill=SCOPE, outline=BODY_HI)
    pts = [(x + 2 + i, y + h / 2 - math.sin(i / max(1, w - 4) * math.pi * 2 * cycles) * h * 0.32)
           for i in range(0, int(w) - 4)]
    d.line(pts, fill=CURVE, width=2)


def osc_cell(d, x, y, w, h):
    d.rounded_rectangle([x, y, x + w, y + h], radius=5, fill=PANEL, outline=(40, 44, 52))
    text(d, (x + 8, y + 7), "OSCILLATORS", 11, DIM)
    gut = 12
    colw = (w - 20 - 2 * gut) / 3
    wh = h * 0.34
    r = max(16, min(26, h * 0.06))
    for i, (name, shape, cycles, knobs, sw, on) in enumerate(COLUMNS):
        cx = x + 10 + i * (colw + gut)
        text(d, (cx, y + h * 0.06), name, 12, INK)
        wy = y + h * 0.10
        if shape:
            wave(d, cx, wy, colw, wh, cycles)
            text(d, (cx + colw / 2, wy + wh / 2), shape, 11, INK, "mm")
        else:
            d.rectangle([cx, wy, cx + colw, wy + wh], fill=CELL, outline=BODY_HI)
            text(d, (cx + colw / 2, wy + wh / 2), "SUB / NOISE / FM", 10, DIM, "mm")
        rows = [knobs[i:i + 2] for i in range(0, len(knobs), 2)]
        r = max(12, min(22, colw / 2 * 0.38))
        for row_i, row in enumerate(rows):
            for j, (lab, val) in enumerate(row):
                kx = cx + colw / 4 + j * (colw / 2)
                ky = y + h * (0.52 + 0.20 * row_i)
                knob(d, kx, ky, r)
                text(d, (kx, ky + r + 10), lab, 10, INK, "mm")
                d.rounded_rectangle([kx - 26, ky + r + 22, kx + 26, ky + r + 40], radius=3,
                                    fill=CELL, outline=(40, 44, 52))
                text(d, (kx, ky + r + 31), val, 10, INK, "mm")
        d.rounded_rectangle([cx, y + h * 0.92, cx + 24, y + h * 0.92 + 20], radius=3,
                            fill=SW_ON if on else SW_OFF)
        text(d, (cx + 32, y + h * 0.92 + 10), sw, 11, INK, "lm")


def band(d, x, y, w, title, names):
    d.rectangle([x, y, x + w, y + ms.ROW_H], fill=ms.ROW_FILL)
    text(d, (x + 10, y + 12), title, 14)
    cx = x + 98
    for i, name in enumerate(names):
        ms.chip(d, cx, y + 3, name, i == 0)
        cx += 100
    return y + 33


def flat(d, x, y, w, h, label, colour):
    d.rectangle([x, y, x + w, y + h], fill=colour)
    text(d, (x + w / 2, y + h / 2), label, 15, INK, "mm")


canvas = ms.Image.new("RGB", (ms.W, ms.H), ms.BG)
d = ms.ImageDraw.Draw(canvas)
ms.chrome(d)
x = ms.RAIL + ms.GAP
w = ms.W - ms.M - x
cell_h = BANDS * CONTENT_H + (BANDS - 1) * ms.GAP
rx = x + 10 + OSC_W + ms.GAP
rw = x + w - 10 - rx
lw = rw - 20 - HERO_W - ms.GAP

d.rectangle([x, CONTENT_Y - 33, x + w, ms.H - ms.M - ms.BOT_H - ms.GAP], fill=ms.ROW_FILL)
osc_cell(d, x + 10, CONTENT_Y, OSC_W, cell_h)

by = band(d, rx, CONTENT_Y - 33, rw, RIGHT_BANDS[0][0], RIGHT_BANDS[0][1])
flat(d, rx + 10, by, rw - 20, CONTENT_H, RIGHT_BANDS[0][2], RIGHT_BANDS[0][3])

by = band(d, rx, CONTENT_Y - 33 + ms.ROW_H + ms.GAP, rw, "SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"])
d.rectangle([rx + 10, by, rx + 10 + HERO_W, by + CONTENT_H], fill=ms.HERO)
text(d, (rx + 10 + HERO_W / 2, by + 48), "HERO CITY", 20, INK, "mm")
text(d, (rx + 10 + HERO_W / 2, by + 76), "same size, moved over", 13, DIM, "mm")
flat(d, rx + 10 + HERO_W + ms.GAP, by, lw, CONTENT_H, f"LFO\n{int(lw)} wide", ms.MODULE2)

by = band(d, rx, CONTENT_Y - 33 + 2 * (ms.ROW_H + ms.GAP), rw, RIGHT_BANDS[1][0], RIGHT_BANDS[1][1])
flat(d, rx + 10, by, rw - 20, CONTENT_H, RIGHT_BANDS[1][2], RIGHT_BANDS[1][3])

ms.pads_and_keys(d, x, ms.H - ms.M - ms.BOT_H, w, ms.BOT_H)
text(d, (x, ms.H - 20),
     f"oscillator block {OSC_W} x {int(cell_h)} portrait on the left, hero unchanged at "
     f"{HERO_W} x {CONTENT_H} and moved over, LFO down to {int(lw)}", 13, DIM)

OUT.parent.mkdir(parents=True, exist_ok=True)
canvas.save(OUT)
print(f"wrote {OUT} {canvas.size}")
