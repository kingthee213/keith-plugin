"""The oscillator module mirrored from the owner's reference, inside the agreed structure.

Reference layout, read off the crop: equal columns, each with the name at the top, a waveform
picture with the shape name inside it, knobs beneath with their labels UNDER them and the values
in boxes below that, and one switch at the foot of each column with its label to the right.

    ~/.venvs/keith-measure/bin/python tools/wireframe_osc_ref.py

Writes design/screenshots/wireframe-osc-reference.png
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import wireframe as wf  # noqa: E402

OUT = wf.ROOT / "design" / "screenshots" / "wireframe-osc-reference.png"

COLUMNS = [
    ("OSC 1", "SINE", 1.0, [("SHAPE", "SINE"), ("TUNE", "0.00"), ("LEVEL", "100"), ("PW", "50")],
     "SYNC", True),
    ("OSC 2", "PULSE", 0.0, [("SHAPE", "PULSE"), ("TUNE", "-12.0"), ("LEVEL", "75"), ("PW", "35")],
     "RING", False),
    ("MIX", None, 0.0, [("SUB", "0.0"), ("NOISE", "0.0"), ("FM", "0.0")], "FM", True),
]


def osc_reference(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS")
    gut = 12
    colw = (w - 20 - 2 * gut) / 3
    for i, (name, shape, cycles, knobs, sw, on) in enumerate(COLUMNS):
        cx = x + 10 + i * (colw + gut)
        wf.txt(d, (cx, y + 24), name, 12, wf.INK)
        if shape:
            wf.scope(d, cx, y + 38, colw, 40, "", cycles)
            wf.txt(d, (cx + colw / 2, y + 58), shape, 11, wf.INK, "mm")
        else:
            d.rounded_rectangle([cx, y + 38, cx + colw, y + 78], radius=3, fill=wf.CELL,
                                outline=wf.BODY_HI)
            wf.txt(d, (cx + colw / 2, y + 58), "SUB / NOISE / FM", 10, wf.DIM, "mm")
        step = colw / len(knobs)
        for j, (lab, val) in enumerate(knobs):
            kx = cx + step * (j + 0.5)
            wf.knob(d, kx, y + 100, 15, "", "")
            wf.txt(d, (kx, y + 120), lab, 10, wf.INK, "mm")
        wf.switch(d, cx, y + h - 22, 24, 20, "", on)
        wf.txt(d, (cx + 32, y + h - 12), sw, 11, wf.INK, "lm")


def state():
    canvas = wf.Image.new("RGB", (wf.W, wf.H), wf.BG)
    d = wf.ImageDraw.Draw(canvas)
    wf.chrome(d)
    x0 = wf.RAIL + wf.GAP
    w0 = wf.W - wf.M - x0
    y = wf.M + wf.TOP_H + wf.GAP
    rows_h = wf.H - wf.M - wf.BOT_H - wf.GAP - y
    rh = (rows_h - 2 * wf.GAP) / 3

    cx, cy, cw, ch = wf.row(d, x0, y, w0, rh, "SOURCE", wf.ROW1, "OSC")
    osc_reference(d, cx, cy, cw, ch)

    y2 = y + rh + wf.GAP
    cx, cy, cw, ch = wf.row(d, x0, y2, w0, rh, "SHAPE", wf.ROW2, "LFO")
    hero_w = cw * 0.34
    d.rectangle([cx, cy, cx + hero_w, cy + ch], fill=wf.HERO)
    wf.txt(d, (cx + hero_w / 2, cy + ch / 2), "HERO CITY", 16, wf.INK, "mm")
    wf.lfo_module(d, cx + hero_w + wf.GAP, cy, cw - hero_w - wf.GAP, ch)

    y3 = y2 + rh + wf.GAP
    cx, cy, cw, ch = wf.row(d, x0, y3, w0, rh, "SPACE", wf.ROW3, "INSERTS")
    wf.fx_module(d, cx, cy, cw, ch)

    wf.pin(d)
    return canvas


img = state()
img.save(OUT)
row_crop = img.crop((wf.RAIL, 58, wf.W, 250))
row_crop.save("/tmp/osc-ref-row.png")
print(f"wrote {OUT} {img.size}")
