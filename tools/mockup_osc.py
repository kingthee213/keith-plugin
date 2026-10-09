"""Wireframe one module inside its row, in the full interface context, at the real proportions.

Module one is the oscillator section, thirteen controls. Two ways to arrange it in the row band.

    ~/.venvs/keith-measure/bin/python tools/mockup_osc.py

Writes design/screenshots/mockup-osc.png
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "mockup-osc.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 200
M, GAP = 8, 8
TOP_H, ROW_H, BOT_H = 44, 180, 168

BG = (14, 15, 17)
RAIL_FILL = (108, 59, 30)
PIN_FILL = (30, 34, 40)
ROW_FILL = (22, 25, 29)
EMPTY = (26, 29, 34)
BTN_FILL = (52, 58, 66)
BTN_ON = (232, 79, 163)
KNOB = (58, 108, 136)
KNOB2 = (96, 60, 110)
KNOB3 = (40, 110, 90)
TAB = (150, 130, 40)
PADS = (40, 110, 90)
KEYS = (40, 44, 52)
INK = (238, 232, 216)
DIM = (150, 150, 150)

OSC1 = [("SHP", KNOB), ("TUNE", KNOB), ("LVL", KNOB), ("PW", KNOB)]
OSC2 = [("SHP", KNOB2), ("TUNE", KNOB2), ("LVL", KNOB2), ("PW", KNOB2)]
MIX = [("SUB", KNOB3), ("NOISE", KNOB3), ("FM", KNOB3)]
SW = [("SYNC", TAB), ("RING", TAB)]


def fnt(size):
    return ImageFont.truetype(str(FONT), size)


def text(d, xy, s, size=13, fill=INK, anchor="la"):
    d.text(xy, s, font=fnt(size), fill=fill, anchor=anchor)


def chip(d, x, y, name, on, w=96, h=24):
    d.rectangle([x, y, x + w, y + h], fill=BTN_ON if on else BTN_FILL)
    text(d, (x + 8, y + h // 2), name, 13, BG if on else INK, "lm")


def cells(d, x, y, w, h, groups, gap=12, label_size=15):
    total = sum(len(g[1]) for g in groups)
    cell = (w - gap * (len(groups) - 1)) / total
    cx = x
    for name, controls in groups:
        for i, (label, colour) in enumerate(controls):
            px = cx + i * cell
            d.rectangle([px + 2, y, px + cell - 2, y + h], fill=colour)
            text(d, (px + cell / 2, y + h / 2), label, label_size, INK, "mm")
        cx += cell * len(controls) + gap
    return cell


def chrome(d):
    d.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    text(d, (16, 20), "MAGS", 34, (232, 79, 163))
    text(d, (16, 62), "SYNTH", 26, INK)
    text(d, (16, H - 110), "BRAND RAIL", 12, (64, 36, 18))
    d.rectangle([RAIL + GAP, M, W - M, M + TOP_H], fill=PIN_FILL)
    text(d, (RAIL + GAP + 10, M + TOP_H // 2), "PRESET  [ 008 Glass Keys v ]", 15, INK, "lm")
    text(d, (W - M - 210, M + TOP_H // 2), "MASTER 0.0 dB  [meter]", 13, INK, "lm")


def empty_row(d, x, y, w, title, modules):
    d.rectangle([x, y, x + w, y + ROW_H], fill=ROW_FILL)
    text(d, (x + 10, y + 12), title, 14)
    cx = x + 10 + 88
    for i, name in enumerate(modules):
        chip(d, cx, y + 3, name, i == 0)
        cx += 100
    d.rectangle([x + 10, y + 33, x + w - 10, y + ROW_H - 8], fill=EMPTY)
    text(d, (x + w // 2, y + 33 + (ROW_H - 41) // 2), modules[0], 15, DIM, "mm")


def source_row(d, x, y, w, variant):
    d.rectangle([x, y, x + w, y + ROW_H], fill=ROW_FILL)
    text(d, (x + 10, y + 12), "SOURCE", 14)
    chip(d, x + 98, y + 3, "OSC", True)
    chip(d, x + 198, y + 3, "FILTER", False)
    chip(d, x + 298, y + 3, "AMP", False)
    bx, by = x + 10, y + 33
    bw, bh = w - 20, ROW_H - 41

    if variant == 1:
        cell = cells(d, bx, by, bw, bh, [("OSC 1", OSC1), ("OSC 2", OSC2), ("MIX", MIX), ("SW", SW)])
        return cell, bh
    half = (bh - GAP) / 2
    cell = cells(d, bx, by, bw, half, [("OSC 1", OSC1), ("OSC 2", OSC2)], label_size=17)
    cells(d, bx, by + half + GAP, bw, half, [("MIX", MIX), ("SW", SW)], label_size=17)
    return cell, half


def interface(variant):
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x = RAIL + GAP
    w = W - M - x
    y = M + TOP_H + GAP
    cell, cellh = source_row(d, x, y, w, variant)
    y += ROW_H + GAP
    empty_row(d, x, y, w, "SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"])
    y += ROW_H + GAP
    empty_row(d, x, y, w, "SPACE", ["INSERTS", "VINTAGE", "GLOBAL"])
    y = H - M - BOT_H
    d.rectangle([x, y, x + w, y + BOT_H], fill=PIN_FILL)
    d.rectangle([x + 10, y + 8, x + 10 + 230, y + BOT_H - 10], fill=PADS)
    text(d, (x + 20, y + 16), "4x4 PADS", 14)
    d.rectangle([x + 250, y + 8, x + w - 10, y + BOT_H - 10], fill=KEYS)
    text(d, ((x + 250 + x + w - 10) // 2, y + BOT_H // 2), "KEYBOARD", 16, INK, "mm")
    label = (f"1  one row, thirteen cells of {cell:.0f} by {cellh:.0f}, grouped 4 / 4 / 3 / 2"
             if variant == 1 else
             f"2  two rows of {cell:.0f} by {cellh:.0f}, oscillators on top, mix and switches below")
    text(d, (RAIL + GAP, H - 20), label, 13, DIM)
    return canvas


sheet = Image.new("RGB", (W, H * 2 + 30), (0, 0, 0))
sheet.paste(interface(1), (0, 0))
sheet.paste(interface(2), (0, H + 30))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}")
