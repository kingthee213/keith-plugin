"""Option B from mockup-states.png with one change: the OSC block is drawn in detail.

Every other block stays flat exactly as B has it, because only the oscillator module is being
worked on. Geometry, colours and the two flat rows come from tools/mockup_states.py.

    ~/.venvs/keith-measure/bin/python tools/osc_wireframe_in_B.py

Writes design/screenshots/osc-wireframe-B.png

The oscillator block is drawn to the wiring facts, verified against docs/PARAM-MANIFEST.md:

  * The engine has TWO oscillators, ids 14-17 (OSC 1) and 18-21 (OSC 2). There is no third.
  * ids 22-24 (SUB / NOISE / FM) are the mixer group. The column is labelled MIXER and says it
    is not an oscillator, so it cannot be read as OSC 3.
  * ids 43 (SYNC) and 44 (RING) are one switch each for the whole module. They are drawn in one
    module-level strip, not inside an oscillator column.
  * SHAPE (ids 14, 18) is a continuous 0-100 % knob, not a mode selector, so the readout is a
    percentage and the trace is generated from shape + PW by tools/osc_wave.py. Nothing about
    the picture is fixed.
  * FM (id 24) has exactly one control: the knob in the MIXER column. No duplicate switch.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mockup_states as ms  # noqa: E402
from osc_wave import wave_points  # noqa: E402

OUT = ms.ROOT / "design" / "screenshots" / "osc-wireframe-B.png"
SESSION_TAG = "osc_wireframe_in_B"
ms._TAG = SESSION_TAG          # tag the header-geometry audit trail for this render

PANEL = (22, 25, 29)
CELL = (18, 20, 24)
BODY = (62, 68, 78)
BODY_HI = (98, 108, 122)
CAP = (206, 90, 140)
SCOPE = (16, 34, 40)
CURVE = (120, 220, 200)
INK = (236, 231, 216)
DIM = (140, 146, 156)
WARN = (214, 168, 84)
SW_ON = (232, 79, 163)
SW_OFF = (48, 53, 61)
BG = ms.BG

# (column, shape %, pw %, tune, level, id note) - the two real oscillators only.
OSC_COLUMNS = [
    ("OSC 1", 0, 50, "0.0", "100", "ids 14-17"),
    ("OSC 2", 85, 35, "-12.0", "75", "ids 18-21"),
]
MIX_COLUMNS = [("SUB", "0%"), ("NOISE", "0%"), ("FM", "0%")]
MIX_IDS = "ids 22-24"
MODULE_SWITCHES = [("SYNC", True), ("RING", False)]
MODULE_SWITCH_IDS = "ids 43-44"


def fnt(s):
    return ms.fnt(s)


def text(d, xy, s, size=12, fill=INK, anchor="la"):
    ms.text(d, xy, s, size, fill, anchor)


def knob(d, cx, cy, r, label, value):
    import math
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=BODY, outline=BODY_HI, width=2)
    a = math.radians(-55)
    d.line([cx, cy, cx + r * 0.62 * math.cos(a), cy + r * 0.62 * math.sin(a)], fill=CAP, width=3)
    text(d, (cx, cy + r + 6), f"{label} {value}", 10, INK, "ma")


def wave(d, x, y, w, h, shape, pw):
    """Scope tile whose trace is computed from the shape and pulse-width values."""
    w, h = int(w), int(h)
    d.rectangle([x, y, x + w, y + h], fill=SCOPE, outline=BODY_HI)
    d.line(wave_points(x, y, w, h, shape, pw), fill=CURVE, width=2)


def band(d, x, y, w, title, names):
    """A row band with the shared header rule, returning the body region."""
    d.rectangle([x, y, x + w, y + ms.ROW_H], fill=ms.ROW_FILL)
    body_top = ms.row_header(d, x, y, w, title, names, tag=SESSION_TAG)
    return x + ms.HEADER_INSET, body_top, w - 2 * ms.HEADER_INSET, y + ms.ROW_H - 8 - body_top


def osc_module(d, x, y, w, h):
    d.rounded_rectangle([x, y, x + w, y + h], radius=5, fill=PANEL, outline=(40, 44, 52))
    gut = 12
    colw = (w - 20 - 2 * gut) / 3.0
    tile_y, tile_h = y + 22, 32
    knob_cy, knob_r = y + 68, 13

    for i, (name, shape, pw, tune, lvl, ids) in enumerate(OSC_COLUMNS):
        cx = x + 10 + i * (colw + gut)
        text(d, (cx, y + 4), name, 12, INK)
        text(d, (cx + colw - 4, y + 4), ids, 9, DIM, "ra")
        wave(d, cx, tile_y, colw, tile_h, shape, pw)
        step = colw / 4.0
        for j, (lab, val) in enumerate((("SHAPE", f"{shape}%"), ("TUNE", tune),
                                        ("LEVEL", lvl), ("PW", f"{pw}%"))):
            knob(d, cx + step * (j + 0.5), knob_cy, knob_r, lab, val)

    # The mixer group. Labelled MIXER with an explicit note so it cannot read as OSC 3.
    cx = x + 10 + 2 * (colw + gut)
    text(d, (cx, y + 4), "MIXER", 12, WARN)
    text(d, (cx + colw - 4, y + 4), MIX_IDS, 9, DIM, "ra")
    d.rectangle([cx, tile_y, cx + colw, tile_y + tile_h], fill=CELL, outline=BODY_HI)
    text(d, (cx + colw / 2, tile_y + 12), "SUB / NOISE / FM", 11, INK, "mm")
    text(d, (cx + colw / 2, tile_y + 25), "not a third oscillator", 9, WARN, "mm")
    step = colw / 3.0
    for j, (lab, val) in enumerate(MIX_COLUMNS):
        knob(d, cx + step * (j + 0.5), knob_cy, knob_r, lab, val)

    # One module-level strip for SYNC and RING: one switch each, for the whole module.
    sy = y + h - 34
    d.rounded_rectangle([x + 10, sy, x + w - 10, sy + 28], radius=4, fill=CELL, outline=BODY_HI)
    text(d, (x + 18, sy + 14), "MODULE   SYNC / RING   one each, module-wide", 10, DIM, "lm")
    text(d, (x + w - 10 - 2 * 74 - 12 - 4, sy + 14), MODULE_SWITCH_IDS, 9, DIM, "rm")
    sx = x + w - 10 - 2 * 74 - 8
    for k, (name, on) in enumerate(MODULE_SWITCHES):
        bx = sx + k * 78
        d.rounded_rectangle([bx, sy + 4, bx + 74, sy + 24], radius=3, fill=SW_ON if on else SW_OFF)
        text(d, (bx + 37, sy + 14), name, 11, BG if on else INK, "mm")


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
body_top = ms.row_header(d, x, y, w, "SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"], tag=SESSION_TAG)
hw = int(w * 0.42)
d.rectangle([x + ms.HEADER_INSET, body_top, x + ms.HEADER_INSET + hw, y + ms.ROW_H - 8], fill=ms.HERO)
text(d, (x + ms.HEADER_INSET + hw // 2, (body_top + y + ms.ROW_H - 8) // 2), "HERO CITY", 20, INK, "mm")
text(d, (x + ms.HEADER_INSET + hw // 2, (body_top + y + ms.ROW_H - 8) // 2 + 26), "in the row", 13, DIM, "mm")
bx = x + ms.HEADER_INSET + hw + ms.GAP
d.rectangle([bx, body_top, x + w - ms.HEADER_INSET, y + ms.ROW_H - 8], fill=ms.MODULE2)
text(d, ((bx + x + w - ms.HEADER_INSET) // 2, (body_top + y + ms.ROW_H - 8) // 2),
     "LFO  module controls", 16, INK, "mm")

y += ms.ROW_H + ms.GAP
ms.row(d, x, y, w, "SPACE", ["INSERTS", "VINTAGE", "GLOBAL"], ms.MODULE3)

ms.pads_and_keys(d, x, ms.H - ms.M - ms.BOT_H, w, ms.BOT_H)
text(d, (x, ms.H - 22), "B, with the OSC block drawn in detail and every other block left flat", 13, DIM)

OUT.parent.mkdir(parents=True, exist_ok=True)
canvas.save(OUT)
osc_row = canvas.crop((ms.RAIL, 60, ms.W, 240))
osc_row.save("/tmp/oscB-row.png")
print(f"wrote {OUT} {canvas.size}")
