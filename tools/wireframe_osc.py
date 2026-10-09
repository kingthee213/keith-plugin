"""Three versions of the oscillator module only, inside the agreed structure, nothing else changed.

The other two rows and the pinned strip are identical in all three images so the only thing that
differs is the oscillator arrangement.

    ~/.venvs/keith-measure/bin/python tools/wireframe_osc.py

Writes design/screenshots/wireframe-osc-variants.png
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import wireframe as wf  # noqa: E402

OUT = wf.ROOT / "design" / "screenshots" / "wireframe-osc-variants.png"


def osc_a(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   A  two oscillators side by side, mix and switches on the right")
    blk = (w - 16) * 0.38
    for i in range(2):
        ox = x + 8 + i * blk
        wf.scope(d, ox, y + 24, blk - 170, h - 34, f"OSC {i+1} WAVE", 1.0 + i)
        cy = y + h / 2 + 6
        kx = ox + blk - 162
        wf.knob(d, kx + 24, cy, 24, "SHAPE", "SINE")
        wf.knob(d, kx + 70, cy, 18, "TUNE", "0.0")
        wf.knob(d, kx + 108, cy, 18, "LVL", "80")
        wf.knob(d, kx + 142, cy, 13, "PW", "50")
    mx = x + 8 + 2 * blk
    wf.box(d, mx, y + 22, x + w - 8 - mx, h - 32, "MIX")
    cy = y + h / 2 + 6
    wf.knob(d, mx + 32, cy, 17, "SUB", "0.0")
    wf.knob(d, mx + 76, cy, 17, "NOISE", "0.0")
    wf.knob(d, mx + 120, cy, 17, "FM", "0.0")
    wf.switch(d, mx + 156, cy - 26, 62, 22, "SYNC", True)
    wf.switch(d, mx + 156, cy + 2, 62, 22, "RING")


def osc_b(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   B  one oscillator per line, stacked")
    for i, oy in enumerate((y + 22, y + 90)):
        wf.scope(d, x + 8, oy, 260, 60, f"OSC {i+1}", 1.0 + i)
        cy = oy + 30
        wf.knob(d, x + 320, cy, 17, "SHAPE", "SINE")
        wf.knob(d, x + 366, cy, 14, "TUNE", "0.0")
        wf.knob(d, x + 402, cy, 14, "LVL", "80")
        wf.knob(d, x + 434, cy, 11, "PW", "50")
        wf.switch(d, x + 456, oy + 3, 60, 20, "SYNC", True)
        wf.switch(d, x + 456, oy + 29, 60, 20, "RING")
    xm = x + 540
    wf.box(d, xm, y + 22, x + w - 8 - xm, h - 32, "MIX  (shared)")
    cy = y + h / 2 + 2
    wf.knob(d, xm + 34, cy, 17, "SUB", "0.0")
    wf.knob(d, xm + 78, cy, 17, "NOISE", "0.0")
    wf.knob(d, xm + 122, cy, 17, "FM", "0.0")


def osc_c(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   C  a column each, oscillator, oscillator, mix")
    colw = (w - 24) / 3
    for i in range(2):
        ox = x + 8 + i * colw
        wf.box(d, ox, y + 22, colw - 10, h - 32, f"OSC {i+1}")
        wf.scope(d, ox + 6, y + 36, colw - 22, 46, "WAVE", 1.0 + i)
        cy = y + h - 34
        wf.knob(d, ox + 32, cy, 20, "SHAPE", "SINE")
        wf.knob(d, ox + 76, cy, 15, "TUNE", "0.0")
        wf.knob(d, ox + 112, cy, 15, "LVL", "80")
        wf.knob(d, ox + 144, cy, 11, "PW", "50")
    ox = x + 8 + 2 * colw
    wf.box(d, ox, y + 22, colw - 10, h - 32, "MIX / SWITCHES")
    wf.knob(d, ox + 32, y + h - 34, 17, "SUB", "0.0")
    wf.knob(d, ox + 76, y + h - 34, 17, "NOISE", "0.0")
    wf.knob(d, ox + 120, y + h - 34, 17, "FM", "0.0")
    wf.switch(d, ox + 148, y + 40, 62, 22, "SYNC", True)
    wf.switch(d, ox + 148, y + 68, 62, 22, "RING")


def state(osc_draw):
    canvas = wf.Image.new("RGB", (wf.W, wf.H), wf.BG)
    d = wf.ImageDraw.Draw(canvas)
    wf.chrome(d)
    x0 = wf.RAIL + wf.GAP
    w0 = wf.W - wf.M - x0
    y = wf.M + wf.TOP_H + wf.GAP
    rows_h = wf.H - wf.M - wf.BOT_H - wf.GAP - y
    rh = (rows_h - 2 * wf.GAP) / 3

    cx, cy, cw, ch = wf.row(d, x0, y, w0, rh, "SOURCE", wf.ROW1, "OSC")
    osc_draw(d, cx, cy, cw, ch)

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


images = [state(fn) for fn in (osc_a, osc_b, osc_c)]
sheet = wf.Image.new("RGB", (wf.W, len(images) * (wf.H + 24)), (0, 0, 0))
for i, im in enumerate(images):
    sheet.paste(im, (0, i * (wf.H + 24)))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}")
