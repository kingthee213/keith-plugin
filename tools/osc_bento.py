"""Oscillator block portrait over the first two bands, green row left exactly as B has it.

The block cannot pass 286 tall without eating the green SPACE band, so its height is fixed at the
two bands above it. At 286 tall, three knob columns need about 350 wide to stay legible, which
makes each column 102 x 286. That is the reference's column shape, its own being 135 x 392.

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
OSC_W = 350
OSC_H = 2 * CONTENT_H + ms.GAP
HERO_W = 446

COLUMNS = [
    ("OSC 1", "SINE", 1.0, [("SHAPE", "SINE"), ("TUNE", "0.00"), ("LEVEL", "100"), ("PW", "50")],
     "SYNC", True),
    ("OSC 2", "PULSE", 0.0, [("SHAPE", "PULSE"), ("TUNE", "-12.0"), ("LEVEL", "75"), ("PW", "35")],
     "RING", False),
    ("MIX", None, 0.0, [("SUB", "0.0"), ("NOISE", "0.0"), ("FM", "0.0")], "FM", True),
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
    text(d, (x + 8, y + 8), "OSCILLATORS", 11, DIM)
    gut = 12
    colw = (w - 20 - 2 * gut) / 3
    r = max(12, min(14, colw * 0.14))
    for i, (name, shape, cycles, knobs, sw, on) in enumerate(COLUMNS):
        cx = x + 10 + i * (colw + gut)
        text(d, (cx, y + 28), name, 12, INK)
        if shape:
            wave(d, cx, y + 46, colw, 60, cycles)
            text(d, (cx + colw / 2, y + 76), shape, 11, INK, "mm")
        else:
            d.rectangle([cx, y + 46, cx + colw, y + 106], fill=CELL, outline=BODY_HI)
            text(d, (cx + colw / 2, y + 71), "SUB /", 10, DIM, "mm")
            text(d, (cx + colw / 2, y + 83), "NOISE / FM", 10, DIM, "mm")
        for row_i, row in enumerate([knobs[i:i + 2] for i in range(0, len(knobs), 2)]):
            cy = y + 130 + row_i * 74
            for j, (lab, val) in enumerate(row):
                kx = cx + colw / 4 + j * (colw / 2)
                knob(d, kx, cy, r)
                text(d, (kx, cy + r + 8), lab, 10, INK, "mm")
                d.rounded_rectangle([kx - 23, cy + r + 20, kx + 23, cy + r + 36], radius=3,
                                    fill=CELL, outline=(40, 44, 52))
                text(d, (kx, cy + r + 28), val, 10, INK, "mm")
        d.rounded_rectangle([cx, y + 262, cx + 24, y + 282], radius=3,
                            fill=SW_ON if on else SW_OFF)
        text(d, (cx + 32, y + 272), sw, 11, INK, "lm")


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
rx = x + 10 + OSC_W + ms.GAP
rw = x + w - 10 - rx
lw = rw - 20 - HERO_W - ms.GAP

d.rectangle([x, CONTENT_Y - 33, x + w, CONTENT_Y - 33 + 2 * ms.ROW_H + ms.GAP], fill=ms.ROW_FILL)
osc_cell(d, x + 10, CONTENT_Y, OSC_W, OSC_H)

by = band(d, rx, CONTENT_Y - 33, rw, "SOURCE", ["FILTER", "AMP"])
flat(d, rx + 10, by, rw - 20, CONTENT_H, "FILTER / AMP  module controls", ms.MODULE)

by = band(d, rx, CONTENT_Y - 33 + ms.ROW_H + ms.GAP, rw, "SHAPE",
          ["LFO", "MOD MATRIX", "AMP MODS"])
d.rectangle([rx + 10, by, rx + 10 + HERO_W, by + CONTENT_H], fill=ms.HERO)
text(d, (rx + 10 + HERO_W / 2, by + 48), "HERO CITY", 20, INK, "mm")
text(d, (rx + 10 + HERO_W / 2, by + 76), "same size, moved over", 13, DIM, "mm")
flat(d, rx + 10 + HERO_W + ms.GAP, by, lw, CONTENT_H, f"LFO\n{int(lw)} wide", ms.MODULE2)

ms.row(d, x, CONTENT_Y - 33 + 2 * (ms.ROW_H + ms.GAP), w, "SPACE",
       ["INSERTS", "VINTAGE", "GLOBAL"], ms.MODULE3)

ms.pads_and_keys(d, x, ms.H - ms.M - ms.BOT_H, w, ms.BOT_H)
text(d, (x, ms.H - 20),
     f"oscillator block {OSC_W} x {int(OSC_H)} over the first two bands, columns "
     f"{int((OSC_W - 44) / 3)} wide, hero unchanged at {HERO_W} x {CONTENT_H} and moved over, "
     f"LFO down to {int(lw)}, green row untouched", 13, DIM)

OUT.parent.mkdir(parents=True, exist_ok=True)
canvas.save(OUT)
print(f"wrote {OUT} {canvas.size}")
