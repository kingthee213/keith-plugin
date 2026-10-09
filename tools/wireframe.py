"""Block out every control of every module as itself, at real sizes, in the full interface.

A wireframe, not the design: knobs are circles with a pointer, faders are slots with a cap,
switches and menus are boxes, waveform displays are boxes with a curve. Sizes carry meaning, so a
primary control is drawn big and a trim small. One image per tab state so the tab behaviour is
visible.

    ~/.venvs/keith-measure/bin/python tools/wireframe.py

Writes design/screenshots/wireframe-all.png
"""

import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "wireframe-all.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 176
M, GAP = 8, 8
TOP_H = 42
BOT_H = 132
TAB_H = 34

BG = (13, 14, 16)
RAIL_FILL = (104, 56, 28)
RAIL_INK = (58, 30, 14)
STRIP = (28, 31, 36)
PANEL = (22, 25, 29)
EMPTY = (18, 20, 24)
BODY = (62, 68, 78)
BODY_HI = (96, 106, 120)
CAP = (206, 90, 140)
SW_ON = (232, 79, 163)
SW_OFF = (48, 53, 61)
MENU = (44, 49, 57)
SCOPE = (16, 34, 40)
CURVE = (120, 220, 200)
PAD = (38, 92, 78)
KEY_W = (198, 196, 190)
KEY_B = (20, 21, 24)
INK = (236, 231, 216)
DIM = (140, 146, 156)


def fnt(s):
    return ImageFont.truetype(str(FONT), s)


def txt(d, xy, s, size=13, fill=INK, anchor="la"):
    d.text(xy, s, font=fnt(size), fill=fill, anchor=anchor)


def knob(d, cx, cy, r, label, value="0.0", size=12):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=BODY, outline=BODY_HI, width=2)
    d.line([cx, cy, cx + r * 0.62 * math.cos(math.radians(-55)),
            cy + r * 0.62 * math.sin(math.radians(-55))], fill=CAP, width=3)
    txt(d, (cx, cy - r - 13), label, size, INK, "mm")
    txt(d, (cx, cy + r + 5), value, size - 1, DIM, "ma")


def fader(d, cx, y, h, label, value="0.0"):
    d.rounded_rectangle([cx - 7, y, cx + 7, y + h], radius=4, fill=(30, 33, 39), outline=BODY_HI)
    cy = y + h * 0.42
    d.rounded_rectangle([cx - 15, cy - 7, cx + 15, cy + 7], radius=4, fill=CAP)
    txt(d, (cx, y - 15), label, 13, INK, "mm")
    txt(d, (cx, y + h + 6), value, 12, DIM, "ma")


def switch(d, x, y, w, h, label, on=False):
    d.rounded_rectangle([x, y, x + w, y + h], radius=4, fill=SW_ON if on else SW_OFF)
    txt(d, (x + w / 2, y + h / 2), label, 12, BG if on else INK, "mm")


def menu(d, x, y, w, h, label, value):
    d.rounded_rectangle([x, y, x + w, y + h], radius=3, fill=MENU, outline=BODY_HI)
    txt(d, (x + 7, y + h / 2), label, 11, DIM, "lm")
    if w > 70:
        d.polygon([(x + w - 16, y + h / 2 - 3), (x + w - 8, y + h / 2 - 3), (x + w - 12, y + h / 2 + 3)],
                  fill=INK)
    txt(d, (x + w - 22, y + h / 2), value, 12, INK, "rm")


def scope(d, x, y, w, h, label, frac=0.6):
    w, h = int(w), int(h)
    d.rectangle([x, y, x + w, y + h], fill=SCOPE, outline=BODY_HI)
    pts = []
    for i in range(0, w - 4):
        t = i / max(1, w - 4)
        pts.append((x + 2 + i, y + h / 2 - math.sin(t * math.pi * 2 * max(1, int(frac * 4))) * h * 0.32))
    d.line(pts, fill=CURVE, width=2)
    txt(d, (x + 5, y + 3), label, 10, DIM)


def pads(d, x, y, w, h, rows=4, cols=4):
    size = min(w, h)
    cw, ch = size / cols, size / rows
    for r in range(rows):
        for c in range(cols):
            px, py = x + c * cw, y + r * ch
            d.rounded_rectangle([px + 2, py + 2, px + cw - 4, py + ch - 4], radius=3,
                                fill=PAD if (r + c) % 3 else (52, 46, 84))
    txt(d, (x + 4, y + 4), "4x4", 10, DIM)
    return size


def keyboard(d, x, y, w, h):
    d.rectangle([x, y, x + w, y + h], fill=KEY_B)
    n = 22
    kw = w / n
    for i in range(n):
        d.rectangle([x + i * kw + 1, y, x + (i + 1) * kw - 1, y + h], fill=KEY_W)
    for i in range(n - 1):
        if i % 7 in (0, 1, 3, 4, 5):
            d.rectangle([x + (i + 1) * kw - kw * 0.3, y, x + (i + 1) * kw + kw * 0.3, y + h * 0.6],
                        fill=KEY_B)


def group_box(d, x, y, w, h, title):
    d.rounded_rectangle([x, y, x + w, y + h], radius=5, fill=PANEL, outline=(40, 44, 52))
    txt(d, (x + 8, y + 6), title, 12, DIM)


def chrome(d, tab, tabs):
    d.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    txt(d, (14, 22), "MAGS", 30, (232, 79, 163))
    txt(d, (14, 58), "SYNTH", 22, INK)
    txt(d, (14, H - 92), "ANALOG", 11, RAIL_INK)
    txt(d, (14, H - 76), "REPLICATION", 11, RAIL_INK)
    d.rectangle([RAIL + GAP, M, W - M, M + TOP_H], fill=STRIP)
    menu(d, RAIL + GAP + 8, M + 8, 250, 26, "PRESET", "008 Glass Keys")
    txt(d, (RAIL + GAP + 280, M + TOP_H / 2), "PREV", 12, DIM, "lm")
    txt(d, (RAIL + GAP + 330, M + TOP_H / 2), "NEXT", 12, DIM, "lm")
    menu(d, W - M - 300, M + 8, 150, 26, "MONO", "POLY")
    d.rectangle([W - M - 140, M + 8, W - M - 8, M + 34], fill=STRIP, outline=BODY_HI)
    txt(d, (W - M - 74, M + 21), "OUT 0.0 dB", 12, INK, "mm")
    y = M + TOP_H + GAP
    d.rectangle([RAIL + GAP, y, W - M, y + TAB_H], fill=STRIP)
    cx = RAIL + GAP + 8
    for name in tabs:
        w = 14 + len(name) * 8
        on = name == tab
        switch(d, cx, y + 5, w, TAB_H - 10, name, on)
        cx += w + 6


def pin_row(d):
    y = H - M - BOT_H
    x = RAIL + GAP
    w = W - M - x
    d.rectangle([x, y, x + w, y + BOT_H], fill=STRIP)
    pads(d, x + 8, y + 8, 150, BOT_H - 16, 4, 4)
    menu(d, x + 168, y + 8, 190, 24, "KIT", "Neptunes")
    menu(d, x + 168, y + 38, 190, 24, "PICK", "008 Glass Keys")
    switch(d, x + 366, y + 8, 74, 24, "MUTE")
    switch(d, x + 366, y + 38, 74, 24, "SOLO")
    knob(d, x + 470, y + 46, 20, "KIT LVL", "0.0")
    knob(d, x + 520, y + 46, 20, "SLOT", "0.0")
    keyboard(d, x + 566, y + 14, w - 578, BOT_H - 28)


TABS = ["OSC", "FILTER", "AMP", "LFO", "MOD", "AMP MODS", "FX", "VINTAGE", "GLOBAL"]


def module_area():
    x = RAIL + GAP
    y = M + TOP_H + GAP + TAB_H + GAP
    return x, y, W - M - x, H - M - BOT_H - GAP - y


def draw_osc(d, x, y, w, h):
    group_box(d, x, y, w, h, "OSCILLATORS")
    for i in range(2):
        oy = y + 30 + i * 158
        scope(d, x + 10, oy, 330, 88, f"OSC {i+1} WAVE", 0.35 + 0.3 * i)
        knob(d, x + 430, oy + 62, 38, "SHAPE", "SINE", 14)
        knob(d, x + 530, oy + 62, 28, "TUNE", "0.0")
        knob(d, x + 610, oy + 62, 28, "LEVEL", "80")
        knob(d, x + 680, oy + 62, 21, "PW", "50")
        switch(d, x + 740, oy + 14, 70, 26, "SYNC", True)
        switch(d, x + 740, oy + 48, 70, 26, "RING")
    group_box(d, x + 830, y + 30, 230, 150, "MIX  (shared)")
    knob(d, x + 872, y + 118, 24, "FM", "0.0")
    knob(d, x + 942, y + 118, 24, "SUB", "0.0")
    knob(d, x + 1012, y + 118, 24, "NOISE", "0.0")


def draw_filter(d, x, y, w, h):
    group_box(d, x, y, w, h, "FILTER")
    knob(d, x + 90, y + 120, 52, "CUTOFF", "900 Hz", 14)
    knob(d, x + 230, y + 120, 38, "RESONANCE", "0.0", 13)
    menu(d, x + 300, y + 40, 150, 26, "TYPE", "LP 24dB")
    for i, (lab, val) in enumerate([("ENV", "0.0"), ("KEY", "0.0"), ("VEL", "0.0"), ("DRIVE", "0.0")]):
        knob(d, x + 340 + i * 64, y + 150, 20, lab, val)


def draw_amp(d, x, y, w, h):
    group_box(d, x, y, w, h, "AMP ENVELOPE")
    for i, lab in enumerate(["ATTACK", "DECAY", "SUSTAIN", "RELEASE"]):
        fader(d, x + 80 + i * 92, y + 46, h - 100, lab, "0.0")
    group_box(d, x + 8, y + h - 96, w - 16, 88, "FILTER ENVELOPE")
    for i, lab in enumerate(["ATTACK", "DECAY", "SUSTAIN", "RELEASE"]):
        knob(d, x + 90 + i * 100, y + h - 42, 20, lab, "0.0")


def draw_lfo(d, x, y, w, h):
    group_box(d, x, y, w, h, "LFO")
    for i in range(2):
        ox = x + 8 + i * ((w - 16) / 2)
        cw = (w - 16) / 2 - 16
        group_box(d, ox, y + 26, cw, h - 40, f"LFO {i+1}")
        scope(d, ox + 8, y + 50, cw - 16, 76, "WAVE", 0.25 + 0.4 * i)
        knob(d, ox + 60, y + 190, 34, "RATE", "1.00 Hz", 13)
        knob(d, ox + 150, y + 190, 26, "DEPTH", "0.0")
        menu(d, ox + 8, y + 250, cw - 16, 26, "SHAPE", "SINE")
        menu(d, ox + 8, y + 282, cw - 16, 26, "DEST", "CUTOFF")


def draw_mod(d, x, y, w, h):
    group_box(d, x, y, w, h, "MOD MATRIX")
    for i in range(4):
        ry = y + 30 + i * 46
        txt(d, (x + 10, ry + 12), str(i + 1), 12, DIM, "lm")
        menu(d, x + 34, ry, 210, 26, "SOURCE", "LFO 1")
        menu(d, x + 254, ry, 210, 26, "DEST", "CUTOFF")
        knob(d, x + 520, ry + 13, 20, "AMT", "0.0")
        switch(d, x + 560, ry + 1, 56, 24, "INV")


def draw_fx(d, x, y, w, h):
    group_box(d, x, y, w, h, "EFFECTS")
    for i in range(3):
        ox = x + 8 + i * ((w - 16) / 3)
        cw = (w - 16) / 3 - 12
        group_box(d, ox, y + 26, cw, h - 40, ["INSERT 1", "INSERT 2", "MASTER"][i])
        menu(d, ox + 8, y + 50, cw - 16, 26, "ALGO", ["CHORUS", "DELAY", "REVERB"][i])
        knob(d, ox + 42, y + 124, 26, "A", "0.0")
        knob(d, ox + 112, y + 124, 26, "B", "0.0")
        knob(d, ox + 182, y + 124, 26, "C", "0.0")
        knob(d, ox + 252, y + 124, 22, "MIX", "0.0")
        switch(d, ox + 8, y + 170, cw - 16, 24, "ACT", True)
        scope(d, ox + 8, y + 204, cw - 16, 54, "RESPONSE", 0.6)


def draw_drums(d, x, y, w, h):
    group_box(d, x, y, w, h, "DRUM PADS  (pads stay pinned below, this is the selected pad)")
    size = pads(d, x + 10, y + 30, 300, h - 44, 4, 4)
    kx = x + 10 + size + 30
    for i, lab in enumerate(["LEVEL", "PAN", "TUNE", "DECAY", "FILTER", "DRIVE"]):
        knob(d, kx + (i % 3) * 84, y + 70 + (i // 3) * 96, 24, lab, "0.0")
    mx = kx + 3 * 84 + 20
    menu(d, mx, y + 40, 190, 26, "SAMPLE SLOT", "KICK 3")
    switch(d, mx, y + 76, 190, 24, "FROM KIT")
    switch(d, mx, y + 106, 190, 24, "ONE SHOT")
    scope(d, mx, y + 142, 190, 70, "SAMPLE", 0.9)


def draw_vintage(d, x, y, w, h):
    group_box(d, x, y, w, h, "VINTAGE ANALOG  (tape and console character, the Hybrid / Analog switch)")
    switch(d, x + 12, y + 30, 110, 26, "HYBRID", True)
    switch(d, x + 130, y + 30, 110, 26, "ANALOG")
    photo = [("TONE", "0.0"), ("NOISE", "0.0"), ("COMP", "0.0"), ("DIST", "0.0"),
             ("WOW", "0.0"), ("FLUT", "0.0"), ("HISS", "0.0"), ("PRINT", "0.0")]
    for i, (lab, val) in enumerate(photo):
        knob(d, x + 60 + (i % 4) * 92, y + 110 + (i // 4) * 120, 24, lab, val)
    menu(d, x + 480, y + 60, 150, 26, "TYPE", "808")


def draw_global(d, x, y, w, h):
    group_box(d, x, y, w, h, "GLOBAL")
    knob(d, x + 70, y + 90, 34, "VOLUME", "0.0")
    knob(d, x + 190, y + 90, 26, "GLIDE", "0.0")
    knob(d, x + 300, y + 90, 26, "VOICES", "8")
    knob(d, x + 410, y + 90, 26, "DETUNE", "0.0")
    knob(d, x + 520, y + 90, 26, "SYNC", "0.0")
    menu(d, x + 60, y + 190, 170, 26, "VOICE MODE", "POLY")
    menu(d, x + 250, y + 190, 170, 26, "LEGATO", "RETRIG")
    menu(d, x + 440, y + 190, 200, 26, "MOD WHEEL", "CUTOFF")
    knob(d, x + 660, y + 100, 22, "MW AMT", "0.0")
    d.rectangle([x + 560, y + 140, x + 760, y + 178], fill=(16, 40, 24), outline=BODY_HI)
    txt(d, (x + 660, y + 159), "OUTPUT METER", 12, CURVE, "mm")


DRAW = {"OSC": draw_osc, "FILTER": draw_filter, "AMP": draw_amp, "LFO": draw_lfo,
        "MOD": draw_mod, "FX": draw_fx, "GLOBAL": draw_global}


def state(tab):
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d, tab, TABS)
    x, y, w, h = module_area()
    if tab == "DRUMS":
        draw_drums(d, x, y, w, h)
    elif tab == "VINTAGE":
        draw_vintage(d, x, y, w, h)
    else:
        DRAW[tab](d, x, y, w, h)
    pin_row(d)
    txt(d, (W - M - 8, H - 16), f"state: {tab}", 13, DIM, "ra")
    return canvas


states = ["OSC", "FILTER", "AMP", "LFO", "MOD", "FX", "VINTAGE", "GLOBAL", "DRUMS"]
sheet = Image.new("RGB", (W, len(states) * (H + 24)), (0, 0, 0))
for i, s in enumerate(states):
    sheet.paste(state(s), (0, i * (H + 24)))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}  states: {', '.join(states)}")
