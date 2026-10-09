"""Throwaway mockup of the grouped-rows layout, drawn at the real 1280x800 proportions.

Colour blocks with labels, no styling. For looking at the structure, not the look. Renders the two
groupings still open: three rows with the drum strip pinned, and four rows with drums as their own row.

    ~/.venvs/keith-measure/bin/python tools/mockup_rows.py

Writes design/screenshots/mockup-rows.png
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "mockup-rows.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 200
M = 8
GAP = 8

BG = (14, 15, 17)
RAIL_FILL = (108, 59, 30)
PIN_FILL = (30, 34, 40)
ROW_FILL = (22, 25, 29)
BTN_FILL = (52, 58, 66)
BTN_ON = (232, 79, 163)
BLOCK = [
    (44, 96, 120), (96, 60, 110), (40, 110, 90),
    (120, 84, 40), (60, 70, 120), (110, 50, 60),
]
INK = (238, 232, 216)


def font(size):
    return ImageFont.truetype(str(FONT), size)


def label(draw, xy, text, size=13, fill=INK, anchor="la"):
    draw.text(xy, text, font=font(size), fill=fill, anchor=anchor)


def switcher(draw, x, y, names, on=0, h=26, w=104):
    for i, name in enumerate(names):
        bx = x + i * (w + 4)
        draw.rectangle([bx, y, bx + w, y + h], fill=BTN_ON if i == on else BTN_FILL)
        label(draw, (bx + 8, y + h // 2), name, 13, BG if i == on else INK, "lm")
    return x + len(names) * (w + 4)


def variant(three_rows):
    canvas = Image.new("RGB", (W, H), BG)
    draw = ImageDraw.Draw(canvas)

    draw.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    label(draw, (16, 20), "MAGS", 34, (232, 79, 163))
    label(draw, (16, 62), "SYNTH", 26, INK)
    label(draw, (16, H - 120), "BRAND RAIL", 13, (60, 34, 18))
    label(draw, (16, H - 100), "logo, icons", 11, (60, 34, 18))

    left = RAIL + GAP
    right = W - M
    y = M

    draw.rectangle([left, y, right, y + 44], fill=PIN_FILL)
    label(draw, (left + 10, y + 22), "PRESET  [ 008 Glass Keys v ]", 15, INK, "lm")
    label(draw, (right - 200, y + 22), "MASTER  0.0 dB   [meter]", 13, INK, "lm")
    y += 44 + GAP

    if three_rows:
        rows = [
            ("SOURCE", ["OSC", "FILTER", "AMP"]),
            ("SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"]),
            ("SPACE", ["INSERTS", "VINTAGE", "GLOBAL"]),
        ]
        row_h = (H - y - GAP - 168 - M - 2 * GAP) // 3
    else:
        rows = [
            ("SOURCE", ["OSC", "FILTER", "AMP"]),
            ("SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"]),
            ("PLAY", ["DRUM EDIT", "SAMPLED"]),
            ("SPACE", ["INSERTS", "VINTAGE", "GLOBAL"]),
        ]
        row_h = (H - y - GAP - 168 - M - 3 * GAP) // 4

    for i, (title, modules) in enumerate(rows):
        draw.rectangle([left, y, right, y + row_h], fill=ROW_FILL)
        label(draw, (left + 10, y + 14), title, 14, INK)
        switcher(draw, left + 10 + 90, y + 4, modules, 0, 24, 96)
        bx = left + 10
        bw = right - 10 - bx
        draw.rectangle([bx, y + 34, bx + bw, y + row_h - 8], fill=BLOCK[i % len(BLOCK)])
        label(draw, (bx + bw // 2, y + 34 + (row_h - 42) // 2),
              f"{modules[0]}  module controls", 16, INK, "mm")
        y += row_h + GAP

    draw.rectangle([left, y, right, y + 168], fill=PIN_FILL)
    pads_w = 230
    draw.rectangle([left + 10, y + 8, left + 10 + pads_w, y + 160], fill=BLOCK[2])
    label(draw, (left + 20, y + 16), "4x4 PADS", 14, INK)
    kb_x = left + 10 + pads_w + 10
    draw.rectangle([kb_x, y + 8, right - 10, y + 160], fill=(40, 44, 52))
    label(draw, ((kb_x + right - 10) // 2, y + 84), "KEYBOARD  (always)", 16, INK, "mm")

    header = "VARIANT 1 — three rows, drums pinned down with the keys" if three_rows else \
             "VARIANT 2 — four rows, drums get their own row"
    label(draw, (RAIL + GAP, H - 20), header, 13, (150, 150, 150))
    return canvas


three = variant(True)
four = variant(False)
sheet = Image.new("RGB", (W, H * 2 + 30), (0, 0, 0))
sheet.paste(three, (0, 0))
sheet.paste(four, (0, H + 30))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}")
