"""Three versions of the oscillator module only, inside the agreed structure, nothing else changed.

The other two rows and the pinned strip are identical in all three images so the only thing that
differs is the oscillator arrangement.

    ~/.venvs/keith-measure/bin/python tools/wireframe_osc.py

Writes design/screenshots/wireframe-osc-variants.png

All three are drawn to the wiring facts (`docs/PARAM-MANIFEST.md`): two oscillators (ids 14-17
and 18-21), a MIXER group (22-24) labelled so it cannot read as OSC 3, one SYNC and one RING
switch for the whole module (43, 44) in a module-level strip, SHAPE as a continuous percentage
whose trace is generated from shape + PW, and FM as a single knob in the mixer.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import wireframe as wf  # noqa: E402

wf._TAG = "wireframe_osc"     # tag this file's entries in the header-geometry audit trail

OUT = wf.ROOT / "design" / "screenshots" / "wireframe-osc-variants.png"

# (shape %, pw %) per oscillator, and the other two real oscillator values.
OSC_PARAMS = [(0, 50), (85, 35)]
TUNES = ("0.0", "-12.0")
LEVELS = ("100", "75")
MIX_KNOBS = ["SUB", "NOISE", "FM"]
STRIP_H = 24


def geometry(y, h):
    """The module interior, given that the header band and the switch strip own their space."""
    top = y + wf.BOX_BAND
    body_bottom = y + h - 6 - STRIP_H - 4
    return top, body_bottom, body_bottom - top


def mix_panel(d, x, y, w, h, note):
    """The mixer panel. The label says MIXER and what is in it, so it cannot read as OSC 3."""
    d.rounded_rectangle([x, y, x + w, y + h], radius=4, fill=wf.CELL, outline=wf.BODY_HI)
    wf.txt(d, (x + w / 2, y + 12), f"MIXER   {note}", 10, wf.INK, "mm")


def osc_a(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   A  two oscillators side by side, mixer on the right")
    top, body_bottom, region = geometry(y, h)
    blk = (w - 16) * 0.36
    for i, (shape, pw) in enumerate(OSC_PARAMS):
        ox = x + 8 + i * blk
        wf.wave_scope(d, ox, top + 2, 150, region - 4, f"OSC {i+1}", shape, pw)
        ax, aw = ox + 156, blk - 164
        cy = top + region / 2
        for j, (lab, val) in enumerate((("SHAPE", f"{shape}%"), ("TUNE", TUNES[i]),
                                        ("LEVEL", LEVELS[i]), ("PW", f"{pw}%"))):
            wf.knob(d, ax + aw * (j + 0.5) / 4, cy, 13, lab, val)
    mx = x + 8 + 2 * blk
    mw = x + w - 8 - mx
    mix_panel(d, mx, top + 2, mw, region - 4, "SUB  NOISE  FM   not an oscillator")
    for j, lab in enumerate(MIX_KNOBS):
        wf.knob(d, mx + mw * (j + 0.5) / 3, top + 54, 15, lab, "0%")
    wf.module_switch_strip(d, x + 8, y + h - 6 - STRIP_H, w - 16, STRIP_H)


def osc_b(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   B  one oscillator per line, stacked")
    top, body_bottom, region = geometry(y, h)
    pitch = region / 2.0
    for i, (shape, pw) in enumerate(OSC_PARAMS):
        oy = top + i * pitch
        wf.wave_scope(d, x + 8, oy + 2, 220, pitch - 4, f"OSC {i+1}", shape, pw)
        cy = oy + pitch / 2
        for j, (lab, val) in enumerate((("SHAPE", f"{shape}%"), ("TUNE", TUNES[i]),
                                        ("LEVEL", LEVELS[i]), ("PW", f"{pw}%"))):
            wf.knob_compact(d, x + 246 + j * 46, cy, 14, lab, val)
    mx = x + 8 + 220 + 4 * 46 + 12
    mw = x + w - 8 - mx
    mix_panel(d, mx, top + 2, mw, region - 4, "SUB  NOISE  FM   not an oscillator")
    for j, lab in enumerate(MIX_KNOBS):
        wf.knob(d, mx + mw * (j + 0.5) / 3, top + 54, 15, lab, "0%")
    wf.module_switch_strip(d, x + 8, y + h - 6 - STRIP_H, w - 16, STRIP_H)


def osc_c(d, x, y, w, h):
    wf.box(d, x, y, w, h, "OSCILLATORS   C  a column each: oscillator, oscillator, mixer")
    top, body_bottom, region = geometry(y, h)
    colw = (w - 16 - 2 * 4) / 3.0
    for i, (shape, pw) in enumerate(OSC_PARAMS):
        ox = x + 8 + i * (colw + 4)
        wf.txt(d, (ox + colw / 2, top + 10), f"OSC {i+1}", 12, wf.INK, "mm")
        wf.wave_scope(d, ox, top + 22, colw - 8, 26, "", shape, pw)
        for j, (lab, val) in enumerate((("SHAPE", f"{shape}%"), ("TUNE", TUNES[i]),
                                        ("LEVEL", LEVELS[i]), ("PW", f"{pw}%"))):
            wf.knob_compact(d, ox + (colw - 8) * (j + 0.5) / 4, top + 62, 10, lab, val)
    ox = x + 8 + 2 * (colw + 4)
    d.rounded_rectangle([ox, top + 6, ox + colw, body_bottom - 2], radius=4,
                        fill=wf.CELL, outline=wf.BODY_HI)
    wf.txt(d, (ox + colw / 2, top + 20), "MIXER   SUB / NOISE / FM", 10, wf.INK, "mm")
    wf.txt(d, (ox + colw / 2, top + 32), "not an oscillator", 9, wf.DIM, "mm")
    for j, lab in enumerate(MIX_KNOBS):
        wf.knob_compact(d, ox + colw * (j + 0.5) / 3, top + 62, 10, lab, "0%")
    wf.module_switch_strip(d, x + 8, y + h - 6 - STRIP_H, w - 16, STRIP_H)


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
