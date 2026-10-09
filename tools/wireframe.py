"""Wireframe of the agreed structure: three rows with a switcher each, hero in the middle row,
pads and keyboard pinned. Every control drawn as itself and sized by importance.

Structure B, as agreed. Not one module filling the window.

    ~/.venvs/keith-measure/bin/python tools/wireframe.py

Writes design/screenshots/wireframe-rows.png, one image per switcher state.
"""

import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "wireframe-rows.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 176
M, GAP, TAB_H = 8, 8, 30
TOP_H, BOT_H = 42, 132

BG = (13, 14, 16)
RAIL_FILL = (104, 56, 28)
RAIL_INK = (58, 30, 14)
STRIP = (28, 31, 36)
PANEL = (22, 25, 29)
CELL = (18, 20, 24)
BODY = (62, 68, 78)
BODY_HI = (98, 108, 122)
CAP = (206, 90, 140)
SW_ON = (232, 79, 163)
SW_OFF = (48, 53, 61)
MENU = (44, 49, 57)
SCOPE = (16, 34, 40)
CURVE = (120, 220, 200)
PAD = (38, 92, 78)
HERO = (150, 40, 90)
KEY_W = (198, 196, 190)
KEY_B = (20, 21, 24)
INK = (236, 231, 216)
DIM = (140, 146, 156)


def fnt(s):
    return ImageFont.truetype(str(FONT), s)


def txt(d, xy, s, size=12, fill=INK, anchor="la"):
    d.text(xy, s, font=fnt(size), fill=fill, anchor=anchor)


def knob(d, cx, cy, r, label, value="0.0"):
    d.ellipse([cx - r, cy - r, cx + r, cy + r], fill=BODY, outline=BODY_HI, width=2)
    a = math.radians(-55)
    d.line([cx, cy, cx + r * 0.62 * math.cos(a), cy + r * 0.62 * math.sin(a)], fill=CAP, width=3)
    txt(d, (cx, cy - r - 11), label, int(max(9, min(12, r * 0.44))), INK, "mm")
    txt(d, (cx, cy + r + 3), value, int(max(8, min(11, r * 0.4))), DIM, "ma")


def fader(d, cx, y, h, label, value="0.0"):
    d.rounded_rectangle([cx - 6, y, cx + 6, y + h], radius=3, fill=(30, 33, 39), outline=BODY_HI)
    cy = y + h * 0.42
    d.rounded_rectangle([cx - 14, cy - 6, cx + 14, cy + 6], radius=3, fill=CAP)
    txt(d, (cx, y - 13), label, 11, INK, "mm")
    txt(d, (cx, y + h + 4), value, 9, DIM, "ma")


def switch(d, x, y, w, h, label, on=False):
    d.rounded_rectangle([x, y, x + w, y + h], radius=3, fill=SW_ON if on else SW_OFF)
    txt(d, (x + w / 2, y + h / 2), label, 11, BG if on else INK, "mm")


def menu(d, x, y, w, h, label, value=""):
    d.rounded_rectangle([x, y, x + w, y + h], radius=3, fill=MENU, outline=BODY_HI)
    txt(d, (x + 6, y + h / 2), label, 10, DIM, "lm")
    d.polygon([(x + w - 14, y + h / 2 - 3), (x + w - 7, y + h / 2 - 3), (x + w - 10, y + h / 2 + 3)],
              fill=INK)
    if value:
        txt(d, (x + w - 18, y + h / 2), value, 11, INK, "rm")


def scope(d, x, y, w, h, label, cycles=1.0):
    w, h = int(w), int(h)
    d.rectangle([x, y, x + w, y + h], fill=SCOPE, outline=BODY_HI)
    pts = [(x + 2 + i, y + h / 2 - math.sin(i / max(1, w - 4) * math.pi * 2 * cycles) * h * 0.3)
           for i in range(0, w - 4)]
    d.line(pts, fill=CURVE, width=2)
    txt(d, (x + 4, y + 2), label, 9, DIM)


def pads(d, x, y, size, rows=4, cols=4):
    cw, ch = size / cols, size / rows
    for r in range(rows):
        for c in range(cols):
            px, py = x + c * cw, y + r * ch
            d.rounded_rectangle([px + 2, py + 2, px + cw - 4, py + ch - 4], radius=3,
                                fill=PAD if (r + c) % 3 else (52, 46, 84))


def keyboard(d, x, y, w, h):
    d.rectangle([x, y, x + w, y + h], fill=KEY_B)
    n = 20
    kw = w / n
    for i in range(n):
        d.rectangle([x + i * kw + 1, y, x + (i + 1) * kw - 1, y + h], fill=KEY_W)
    for i in range(n - 1):
        if i % 7 in (0, 1, 3, 4, 5):
            d.rectangle([x + (i + 1) * kw - kw * 0.3, y, x + (i + 1) * kw + kw * 0.3, y + h * 0.6],
                        fill=KEY_B)


def box(d, x, y, w, h, title=None):
    d.rounded_rectangle([x, y, x + w, y + h], radius=5, fill=PANEL, outline=(40, 44, 52))
    if title:
        txt(d, (x + 7, y + 5), title, 11, DIM)


def chrome(d):
    d.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    txt(d, (14, 22), "MAGS", 30, (232, 79, 163))
    txt(d, (14, 58), "SYNTH", 22, INK)
    txt(d, (14, H - 92), "ANALOG", 11, RAIL_INK)
    txt(d, (14, H - 76), "REPLICATION", 11, RAIL_INK)
    d.rectangle([RAIL + GAP, M, W - M, M + TOP_H], fill=STRIP)
    menu(d, RAIL + GAP + 8, M + 8, 240, 26, "PRESET", "008 Glass Keys")
    switch(d, RAIL + GAP + 258, M + 8, 46, 26, "PREV")
    switch(d, RAIL + GAP + 310, M + 8, 46, 26, "NEXT")
    menu(d, W - M - 320, M + 8, 140, 26, "MODE", "POLY")
    d.rectangle([W - M - 170, M + 8, W - M - 8, M + 34], fill=CELL, outline=BODY_HI)
    txt(d, (W - M - 89, M + 21), "OUT 0.0 dB", 11, INK, "mm")


def pin(d):
    y = H - M - BOT_H
    x = RAIL + GAP
    w = W - M - x
    d.rectangle([x, y, x + w, y + BOT_H], fill=STRIP)
    pads(d, x + 8, y + 10, BOT_H - 20)
    txt(d, (x + 8, y + BOT_H - 6), "4x4 PADS", 9, DIM)
    menu(d, x + 140, y + 8, 170, 22, "KIT", "Neptunes")
    menu(d, x + 140, y + 34, 170, 22, "PICK", "Glass Keys")
    switch(d, x + 318, y + 8, 64, 22, "MUTE")
    switch(d, x + 318, y + 34, 64, 22, "SOLO")
    knob(d, x + 408, y + 52, 17, "KIT LVL", "0.0")
    knob(d, x + 452, y + 52, 17, "SLOT", "0.0")
    keyboard(d, x + 486, y + 14, w - 496, BOT_H - 28)


def switcher(d, x, y, names, on):
    cx = x
    for name in names:
        w = 14 + len(name) * 7
        switch(d, cx, y, w, TAB_H - 8, name, name == on)
        cx += w + 5
    return cx


def row(d, x, y, w, h, title, names, on):
    d.rectangle([x, y, x + w, y + h], fill=STRIP)
    txt(d, (x + 8, y + h // 2), title, 12, DIM, "lm")
    cx = switcher(d, x + 74, y + 4, names, on)
    return x + 74 + (cx - (x + 74)) + 6, y + TAB_H, x + w - (cx + 6), h - TAB_H - 6


def osc_module(d, x, y, w, h):
    box(d, x, y, w, h, "OSCILLATORS")
    blk = (w - 16) / 2
    for i in range(2):
        ox = x + 8 + i * blk
        kx = ox + blk - 170
        scope(d, ox, y + 24, kx - ox - 8, h - 34, f"OSC {i+1} WAVE", 1.0 + i)
        cy = y + h / 2 + 6
        knob(d, kx + 26, cy, 25, "SHAPE", "SINE")
        knob(d, kx + 74, cy, 19, "TUNE", "0.0")
        knob(d, kx + 114, cy, 19, "LVL", "80")
        knob(d, kx + 148, cy, 14, "PW", "50")
    return


def filter_module(d, x, y, w, h):
    box(d, x, y, w, h, "FILTER")
    knob(d, x + 52, y + h / 2 + 2, 38, "CUTOFF", "900 Hz")
    knob(d, x + 140, y + h / 2 + 2, 27, "RESO", "0.0")
    menu(d, x + 180, y + 22, 120, 24, "TYPE", "LP24")
    for i, (lab, val) in enumerate([("ENV", "0.0"), ("KEY", "0.0"), ("VEL", "0.0"), ("DRIVE", "0.0")]):
        knob(d, x + 220 + i * 54, y + h - 34, 16, lab, val)


def amp_module(d, x, y, w, h):
    box(d, x, y, w, h, "AMP")
    for i, lab in enumerate(["ATT", "DEC", "SUS", "REL"]):
        fader(d, x + 54 + i * 78, y + 26, h - 52, lab, "0.0")


def lfo_module(d, x, y, w, h):
    box(d, x, y, w, h, "LFO")
    cy = y + h / 2 + 8
    blk = (w - 16) / 2
    for i in range(2):
        ox = x + 8 + i * blk
        scope(d, ox, cy - 30, 76, 60, f"LFO {i+1}", 0.5 + i)
        knob(d, ox + 118, cy, 22, "RATE", "1.0Hz")
        knob(d, ox + 166, cy, 17, "DEPTH", "0.0")
        mw = (blk - 200) / 2 - 5
        menu(d, ox + 188, cy - 11, mw, 22, "SHAPE", "SINE")
        menu(d, ox + 190 + mw, cy - 11, mw, 22, "DEST", "CUT")


def mod_module(d, x, y, w, h):
    box(d, x, y, w, h, "MOD MATRIX")
    for i in range(4):
        ry = y + 24 + i * 30
        menu(d, x + 8, ry, (w - 120) / 2 - 10, 24, "SRC", "LFO 1")
        menu(d, x + 8 + (w - 120) / 2, ry, (w - 120) / 2 - 10, 24, "DST", "CUTOFF")
        knob(d, x + w - 74, ry + 20, 15, "AMT", "0.0")
        switch(d, x + w - 46, ry + 2, 38, 22, "INV")


def ampmods_module(d, x, y, w, h):
    box(d, x, y, w, h, "AMP MODIFIERS")
    for i, lab in enumerate(["NKEY", "AKEY", "AEND", "DKEY", "DRND", "SDLY", "STRK", "SWID"]):
        knob(d, x + 40 + i * ((w - 60) / 8), y + h / 2 + 2, 19, lab, "0.0")


def fx_module(d, x, y, w, h):
    box(d, x, y, w, h, "EFFECTS")
    blk = (w - 16) / 3
    for i in range(3):
        ox = x + 8 + i * blk
        box(d, ox, y + 22, blk - 10, h - 32, ["INSERT 1", "INSERT 2", "MASTER"][i])
        menu(d, ox + 8, y + 44, blk - 26, 22, "ALGO", ["CHORUS", "DELAY", "REVERB"][i])
        knob(d, ox + 32, y + h - 32, 18, "A", "0.0")
        knob(d, ox + 78, y + h - 32, 18, "B", "0.0")
        knob(d, ox + 124, y + h - 32, 18, "C", "0.0")
        knob(d, ox + 164, y + h - 32, 15, "MIX", "0.0")
        switch(d, ox + 8, y + 68, 60, 22, "ACT", True)


def vintage_module(d, x, y, w, h):
    box(d, x, y, w, h, "VINTAGE  (tape and console character)")
    switch(d, x + 10, y + 24, 78, 24, "HYBRID", True)
    switch(d, x + 94, y + 24, 78, 24, "ANALOG")
    for i, lab in enumerate(["TONE", "NOISE", "COMP", "DIST", "WOW", "FLUT"]):
        knob(d, x + 200 + i * 56, y + h / 2 + 2, 18, lab, "0.0")


def global_module(d, x, y, w, h):
    box(d, x, y, w, h, "GLOBAL")
    knob(d, x + 46, y + h / 2 + 2, 30, "VOLUME", "0.0")
    for i, lab in enumerate(["GLIDE", "VOICES", "DETUNE", "SYNC"]):
        knob(d, x + 122 + i * 58, y + h / 2 + 2, 19, lab, "0.0")
    menu(d, x + 372, y + 24, 120, 24, "MODE", "POLY")
    menu(d, x + 372, y + 54, 120, 24, "LEGATO", "RETRIG")
    menu(d, x + 502, y + 24, 130, 24, "MOD WHEEL", "CUTOFF")
    knob(d, x + 524, y + h - 32, 16, "MW AMT", "0.0")
    d.rectangle([x + 560, y + h - 48, x + 700, y + h - 20], fill=(16, 40, 24), outline=BODY_HI)
    txt(d, (x + 630, y + h - 34), "OUTPUT METER", 10, CURVE, "mm")


def drums_module(d, x, y, w, h):
    box(d, x, y, w, h, "DRUM SELECTED PAD  (pads stay pinned below)")
    knob(d, x + 40, y + h / 2 + 2, 20, "LEVEL", "0.0")
    knob(d, x + 88, y + h / 2 + 2, 20, "PAN", "0.0")
    knob(d, x + 136, y + h / 2 + 2, 20, "TUNE", "0.0")
    knob(d, x + 184, y + h / 2 + 2, 20, "DECAY", "0.0")
    knob(d, x + 232, y + h / 2 + 2, 20, "FILTER", "0.0")
    knob(d, x + 280, y + h / 2 + 2, 20, "DRIVE", "0.0")
    menu(d, x + 330, y + 24, 150, 22, "SAMPLE", "KICK 3")
    switch(d, x + 330, y + 54, 150, 22, "ONE SHOT")
    knob(d, x + 510, y + h / 2 + 2, 20, "PATK", "0.0")
    knob(d, x + 558, y + h / 2 + 2, 20, "PDEC", "0.0")
    knob(d, x + 606, y + h / 2 + 2, 20, "PSUS", "0.0")
    knob(d, x + 654, y + h / 2 + 2, 20, "PREL", "0.0")


MODULES = {
    "OSC": osc_module, "FILTER": filter_module, "AMP": amp_module,
    "LFO": lfo_module, "MOD MATRIX": mod_module, "AMP MODS": ampmods_module,
    "INSERTS": fx_module, "VINTAGE": vintage_module, "GLOBAL": global_module,
    "DRUMS": drums_module,
}

ROW1 = ["OSC", "FILTER", "AMP"]
ROW2 = ["LFO", "MOD MATRIX", "AMP MODS"]
ROW3 = ["INSERTS", "VINTAGE", "GLOBAL"]


def state(r1, r2, r3):
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x0 = RAIL + GAP
    w0 = W - M - x0
    y = M + TOP_H + GAP
    rows_h = H - M - BOT_H - GAP - y
    rh = (rows_h - 2 * GAP) / 3

    cx, cy, cw, ch = row(d, x0, y, w0, rh, "SOURCE", ROW1, r1)
    MODULES[r1](d, cx, cy, cw, ch)

    y2 = y + rh + GAP
    cx, cy, cw, ch = row(d, x0, y2, w0, rh, "SHAPE", ROW2, r2)
    hero_w = cw * 0.34
    d.rectangle([cx, cy, cx + hero_w, cy + ch], fill=HERO)
    txt(d, (cx + hero_w / 2, cy + ch / 2), "HERO CITY", 16, INK, "mm")
    MODULES[r2](d, cx + hero_w + GAP, cy, cw - hero_w - GAP, ch)

    y3 = y2 + rh + GAP
    cx, cy, cw, ch = row(d, x0, y3, w0, rh, "SPACE", ROW3, r3)
    MODULES[r3](d, cx, cy, cw, ch)

    pin(d)
    txt(d, (W - M - 8, H - 16), f"SOURCE {r1}   SHAPE {r2}   SPACE {r3}", 12, DIM, "ra")
    return canvas


STATES = [("OSC", "LFO", "INSERTS"), ("FILTER", "MOD MATRIX", "VINTAGE"),
          ("AMP", "AMP MODS", "GLOBAL")]

if __name__ == "__main__":
    images = [state(*s) for s in STATES]
    sheet = Image.new("RGB", (W, len(images) * (H + 24)), (0, 0, 0))
    for i, im in enumerate(images):
        sheet.paste(im, (0, i * (H + 24)))
    OUT.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUT)
    print(f"wrote {OUT} {sheet.size}")
