"""Throwaway wireframes for the two open layout states: where the hero art sits, and the drum
pad module telescoping open.

Plain colour blocks with labels at the real 1280x800 proportions, same as tools/mockup_rows.py.
For agreeing on structure, not for judging the look.

    ~/.venvs/keith-measure/bin/python tools/mockup_states.py

Writes design/screenshots/mockup-states.png

Header rule (see docs/LAYOUT-DIRECTION.md and the handoff): one number owns the header band.
The title is centred vertically inside HEADER_BAND and the switcher chips are centred on the
same band, placed to the right of the title's measured textlength. The body rectangle's top is
`y + HEADER_BAND`, derived from the same number, so a body can never be drawn up into a title.
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

import osc_wave

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "mockup-states.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 200
M, GAP = 8, 8
TOP_H, ROW_H, BOT_H = 44, 180, 168

# --- the header band, one number ---------------------------------------------------------
HEADER_BAND = 34          # fits a 14 px title with real margin above and below
HEADER_INSET = 10         # left/right horizontal margin
TITLE_SIZE = 14
CHIP_W, CHIP_H, CHIP_GAP = 96, 24, 4

# Audit trail. tools/check_layout_render.py walks this and asserts that no title box
# overlaps a chip box and that every title clears the container's top edge by >= 8 px.
HEADER_GEOM = []
_TAG = "mockup_states"

BG = (14, 15, 17)
RAIL_FILL = (108, 59, 30)
PIN_FILL = (30, 34, 40)
ROW_FILL = (22, 25, 29)
BTN_FILL = (52, 58, 66)
BTN_ON = (232, 79, 163)
HERO = (150, 40, 90)
MODULE = (44, 96, 120)
MODULE2 = (96, 60, 110)
MODULE3 = (40, 110, 90)
PADS = (40, 110, 90)
KEYS = (40, 44, 52)
INK = (238, 232, 216)
DIM = (150, 150, 150)

ROWS = [("SOURCE", ["OSC", "FILTER", "AMP"]),
        ("SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"]),
        ("SPACE", ["INSERTS", "VINTAGE", "GLOBAL"])]


def fnt(size):
    return ImageFont.truetype(str(FONT), size)


def text(d, xy, s, size=13, fill=INK, anchor="la"):
    d.text(xy, s, font=fnt(size), fill=fill, anchor=anchor)


def chip(d, x, y, name, on, w=CHIP_W, h=CHIP_H, boxes=None):
    d.rectangle([x, y, x + w, y + h], fill=BTN_ON if on else BTN_FILL)
    text(d, (x + 8, y + h // 2), name, 13, BG if on else INK, "lm")
    box = (x, y, x + w, y + h)
    if boxes is not None:
        boxes.append(box)
    return box


def row_header(d, x, y, w, title, modules, tag=None):
    """Draw a row's header and return the body top.

    The title is centred vertically inside a band of HEADER_BAND; the chips are centred on
    the same band and placed to the right of the title's measured textlength, not at a fixed
    pixel offset. Returns `y + HEADER_BAND`, which is where the body must start.
    """
    f = fnt(TITLE_SIZE)
    bx0, by0, bx1, by1 = d.textbbox((0, 0), title, font=f, anchor="la")
    ink_h = by1 - by0
    ink_top = y + (HEADER_BAND - ink_h) / 2.0
    tx = x + HEADER_INSET
    d.text((tx, ink_top - by0), title, font=f, fill=INK, anchor="la")
    title_box = (tx + bx0, ink_top, tx + bx1, ink_top + ink_h)

    chip_top = y + (HEADER_BAND - CHIP_H) / 2.0
    chip_boxes = []
    cx = tx + d.textlength(title, font=f) + CHIP_GAP
    for i, name in enumerate(modules):
        chip(d, cx, chip_top, name, i == 0, boxes=chip_boxes)
        cx += CHIP_W + CHIP_GAP

    body_top = y + HEADER_BAND
    HEADER_GEOM.append({"script": tag or _TAG, "container_top": y, "title": title,
                        "title_box": title_box, "chip_boxes": chip_boxes,
                        "band": HEADER_BAND, "body_top": body_top})
    return body_top


def chrome(d, rail_label="BRAND RAIL"):
    d.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    text(d, (16, 20), "MAGS", 34, (232, 79, 163))
    text(d, (16, 62), "SYNTH", 26, INK)
    text(d, (16, H - 110), rail_label, 12, (64, 36, 18))
    d.rectangle([RAIL + GAP, M, W - M, M + TOP_H], fill=PIN_FILL)
    text(d, (RAIL + GAP + 10, M + TOP_H // 2), "PRESET  [ 008 Glass Keys v ]", 15, INK, "lm")
    text(d, (W - M - 210, M + TOP_H // 2), "MASTER 0.0 dB  [meter]", 13, INK, "lm")


def row(d, x, y, w, title, modules, fill):
    d.rectangle([x, y, x + w, y + ROW_H], fill=ROW_FILL)
    body_top = row_header(d, x, y, w, title, modules)
    body_bot = y + ROW_H - 8
    d.rectangle([x + HEADER_INSET, body_top, x + w - HEADER_INSET, body_bot], fill=fill)
    text(d, (x + w // 2, (body_top + body_bot) // 2), f"{modules[0]}  module controls", 16, INK, "mm")


def columns_with_hero_band():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    hx = RAIL + GAP
    hy = M + TOP_H + GAP
    hw = 260
    d.rectangle([hx, hy, hx + hw, H - M - BOT_H - GAP], fill=HERO)
    text(d, (hx + hw // 2, hy + 60), "HERO CITY", 20, INK, "mm")
    text(d, (hx + hw // 2, hy + 88), "full height band", 13, DIM, "mm")
    rx = hx + hw + GAP
    rw = W - M - rx
    y = hy
    for title, mods in ROWS:
        row(d, rx, y, rw, title, mods, [MODULE, MODULE2, MODULE3][ROWS.index((title, mods))])
        y += ROW_H + GAP
    pads_and_keys(d, hx, H - M - BOT_H, W - M - hx, BOT_H)
    text(d, (RAIL + GAP, H - 22), "A  hero as a full height band, rows narrower", 13, DIM)
    return canvas


def hero_in_row():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x = RAIL + GAP
    w = W - M - x
    y = M + TOP_H + GAP
    row(d, x, y, w, "SOURCE", ["OSC", "FILTER", "AMP"], MODULE)
    y += ROW_H + GAP
    d.rectangle([x, y, x + w, y + ROW_H], fill=ROW_FILL)
    body_top = row_header(d, x, y, w, "SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"])
    body_bot = y + ROW_H - 8
    hw = int(w * 0.42)
    d.rectangle([x + HEADER_INSET, body_top, x + HEADER_INSET + hw, body_bot], fill=HERO)
    text(d, (x + HEADER_INSET + hw // 2, (body_top + body_bot) // 2), "HERO CITY", 20, INK, "mm")
    text(d, (x + HEADER_INSET + hw // 2, (body_top + body_bot) // 2 + 26), "in the row", 13, DIM, "mm")
    bx = x + HEADER_INSET + hw + GAP
    d.rectangle([bx, body_top, x + w - HEADER_INSET, body_bot], fill=MODULE2)
    text(d, ((bx + x + w - HEADER_INSET) // 2, (body_top + body_bot) // 2),
         "LFO  module controls", 16, INK, "mm")
    y += ROW_H + GAP
    row(d, x, y, w, "SPACE", ["INSERTS", "VINTAGE", "GLOBAL"], MODULE3)
    pads_and_keys(d, x, H - M - BOT_H, w, BOT_H)
    text(d, (RAIL + GAP, H - 22), "B  hero inside the SHAPE row, other rows full width", 13, DIM)
    return canvas


def pads_and_keys(d, x, y, w, h, pads_w=230, big=False, keys=True):
    d.rectangle([x, y, x + w, y + h], fill=PIN_FILL)
    pw = pads_w if not big else int(w * 0.46)
    d.rectangle([x + 10, y + 8, x + 10 + pw, y + h - 10], fill=PADS)
    text(d, (x + 20, y + 16), "4x4 PADS", 14, INK)
    if keys:
        kx = x + 10 + pw + 10
        d.rectangle([kx, y + 8, x + w - 10, y + h - 10], fill=KEYS)
        text(d, ((kx + x + w - 10) // 2, y + h // 2), "KEYBOARD  (always)", 16, INK, "mm")


def drum_open():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d, "BRAND RAIL")
    x = RAIL + GAP
    w = W - M - x
    y = M + TOP_H + GAP
    row(d, x, y, w, "SOURCE", ["OSC", "FILTER", "AMP"], MODULE)
    y += ROW_H + GAP
    row(d, x, y, w, "SHAPE", ["LFO", "MOD MATRIX", "AMP MODS"], MODULE2)
    y += ROW_H + GAP
    d.rectangle([x, y, x + w, y + ROW_H], fill=ROW_FILL)
    body_top = row_header(d, x, y, w, "SPACE", ["INSERTS", "VINTAGE", "GLOBAL"])
    d.rectangle([x + HEADER_INSET, body_top, x + w - HEADER_INSET, y + ROW_H - 8], fill=MODULE3)

    open_h = H - M - (y + ROW_H + GAP) - 44
    oy = y + ROW_H + GAP
    d.rectangle([x, oy, x + w, oy + open_h], fill=PIN_FILL)
    d.rectangle([x, oy, x + w, oy + 30], fill=BTN_ON)
    text(d, (x + 10, oy + 15), "DRUM MODULE OPEN  [ DRUM EDIT | SAMPLED ]   tap the caret to collapse", 14, BG, "lm")
    pw = int(w * 0.34)
    d.rectangle([x + 10, oy + 40, x + 10 + pw, oy + open_h - 10], fill=PADS)
    text(d, (x + 20, oy + 48), "4x4 PADS (bigger)", 14, INK)
    ex = x + 10 + pw + GAP
    d.rectangle([ex, oy + 40, x + w - 10, oy + open_h - 10], fill=MODULE2)
    text(d, ((ex + x + w - 10) // 2, oy + 40 + (open_h - 50) // 2),
         "SELECTED PAD: level pan tune decay filter drive\nkit level   mute  solo   sample slot", 15, INK, "mm")
    ky = oy + open_h + GAP
    d.rectangle([x, ky, x + w, H - M], fill=KEYS)
    text(d, (x + w // 2, ky + (H - M - ky) // 2), "KEYBOARD  (stays, thinner)", 13, INK, "mm")
    text(d, (RAIL + GAP, H - 22), "C  drum module telescoped open over the row above", 13, DIM)
    return canvas


states = [columns_with_hero_band(), hero_in_row(), drum_open()]
sheet = Image.new("RGB", (W, H * 3 + 60), (0, 0, 0))
for i, s in enumerate(states):
    sheet.paste(s, (0, i * (H + 30)))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}")
