"""Two structures that give a module vertical stacking, at the real 1280x800 proportions.

The oscillator module is drawn in place at true cell sizes so the knob size is visible rather
than described.

    ~/.venvs/keith-measure/bin/python tools/mockup_height.py

Writes design/screenshots/mockup-height.png
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "design" / "screenshots" / "mockup-height.png"
FONT = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

W, H = 1280, 800
RAIL = 200
M, GAP = 8, 8
TOP_H, BOT_H = 44, 140

BG = (14, 15, 17)
RAIL_FILL = (108, 59, 30)
PIN_FILL = (30, 34, 40)
ROW_FILL = (22, 25, 29)
EMPTY = (26, 29, 34)
BTN_FILL = (52, 58, 66)
BTN_ON = (232, 79, 163)
HERO = (150, 40, 90)
KNOB = (58, 108, 136)
KNOB2 = (96, 60, 110)
KNOB3 = (40, 110, 90)
TAB = (150, 130, 40)
PADS = (40, 110, 90)
KEYS = (40, 44, 52)
INK = (238, 232, 216)
DIM = (150, 150, 150)


def fnt(size):
    return ImageFont.truetype(str(FONT), size)


def text(d, xy, s, size=13, fill=INK, anchor="la"):
    d.text(xy, s, font=fnt(size), fill=fill, anchor=anchor)


def chip(d, x, y, name, on, w=96, h=24):
    d.rectangle([x, y, x + w, y + h], fill=BTN_ON if on else BTN_FILL)
    text(d, (x + 8, y + h // 2), name, 13, BG if on else INK, "lm")


def panel(d, x, y, w, h, title, fill=EMPTY):
    d.rectangle([x, y, x + w, y + h], fill=ROW_FILL)
    text(d, (x + 8, y + 8), title, 13)
    d.rectangle([x + 6, y + 26, x + w - 6, y + h - 6], fill=fill)


def osc_grid(d, x, y, w, h, cols, rows, title="OSC"):
    panel(d, x, y, w, h, title)
    gx, gy = x + 10, y + 32
    gw, gh = w - 20, h - 42
    cw = gw / cols
    chh = gh / rows
    labels = ["SHP", "TUNE", "LVL", "PW", "SUB", "NOISE", "FM", "SYNC", "RING"]
    i = 0
    for r in range(rows):
        for c in range(cols):
            if i >= 11:
                break
            px, py = gx + c * cw, gy + r * chh
            colour = KNOB if c < 2 else (KNOB2 if r < 2 else KNOB3)
            d.rectangle([px + 2, py + 3, px + cw - 4, py + chh - 4], fill=colour)
            text(d, (px + cw / 2, py + 14), labels[i] if i < len(labels) else "", 12, INK, "mm")
            text(d, (px + cw / 2, py + chh - 14), "0.0", 11, INK, "mm")
            i += 1
    d.rectangle([gx, gy + gh - 4, gx + gw, gy + gh + 14], fill=TAB)
    text(d, (gx + gw / 2, gy + gh + 5), "SYNC   RING", 12, BG, "mm")
    return cw, chh


def chrome(d):
    d.rectangle([0, 0, RAIL, H], fill=RAIL_FILL)
    text(d, (16, 20), "MAGS", 34, (232, 79, 163))
    text(d, (16, 62), "SYNTH", 26, INK)
    text(d, (16, H - 100), "BRAND RAIL", 12, (64, 36, 18))
    d.rectangle([RAIL + GAP, M, W - M, M + TOP_H], fill=PIN_FILL)
    text(d, (RAIL + GAP + 10, M + TOP_H // 2), "PRESET  [ 008 Glass Keys v ]", 15, INK, "lm")
    text(d, (W - M - 210, M + TOP_H // 2), "MASTER 0.0 dB  [meter]", 13, INK, "lm")


def pinned(d, x, y, w):
    d.rectangle([x, y, x + w, y + BOT_H], fill=PIN_FILL)
    d.rectangle([x + 8, y + 6, x + 8 + 200, y + BOT_H - 8], fill=PADS)
    text(d, (x + 16, y + 14), "4x4 PADS", 13)
    d.rectangle([x + 216, y + 6, x + w - 8, y + BOT_H - 8], fill=KEYS)
    text(d, ((x + 216 + x + w - 8) // 2, y + BOT_H // 2), "KEYBOARD", 15, INK, "mm")


def two_tall_rows():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x = RAIL + GAP
    w = W - M - x
    y0 = M + TOP_H + GAP
    rows_h = H - y0 - GAP - BOT_H - M
    rh = (rows_h - GAP) / 2

    widths = [.27, .16, .16, .16, .25]
    cx = x
    tw = 0
    cw = chh = 0.0
    for name, fr in zip(["OSC", "FILTER", "AMP", "LFO", "MOD"], widths):
        tw = w * fr
        if name == "OSC":
            cw, chh = osc_grid(d, cx, y0, tw - GAP, rh, 4, 3)
        else:
            panel(d, cx, y0, tw - GAP, rh, name)
        cx += tw

    y1 = y0 + rh + GAP
    hero_w = w * .28
    d.rectangle([x, y1, x + hero_w, y1 + rh], fill=HERO)
    text(d, (x + hero_w / 2, y1 + rh / 2), "HERO CITY", 20, INK, "mm")
    cx = x + hero_w + GAP
    for name in ["SAMPLED", "DRUM PADS"]:
        tw = w * .21
        panel(d, cx, y1, tw - GAP, rh, name)
        cx += tw
    tw = W - M - cx
    panel(d, cx, y1, tw, rh, "VINTAGE")
    text(d, (cx + 8, y1 + 32), "switcher: VINTAGE  GLOBAL  FX  AMP MODS", 11, DIM)
    pinned(d, x, H - M - BOT_H, w)
    text(d, (RAIL + GAP, H - 18), f"1  two tall rows, cells {cw:.0f} by {chh:.0f}", 13, DIM)
    return canvas


def one_big_module():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x = RAIL + GAP
    w = W - M - x
    y0 = M + TOP_H + GAP
    picker_h = 40
    d.rectangle([x, y0, x + w, y0 + picker_h], fill=ROW_FILL)
    cx = x + 8
    for i, name in enumerate(["SOURCE", "SHAPE", "SPACE", "DRUM", "ALL MODULES"]):
        chip(d, cx, y0 + 8, name, i == 2, w=118, h=24)
        cx += 124
    area_y = y0 + picker_h + GAP
    area_h = H - M - BOT_H - GAP - area_y
    cw, chh = osc_grid(d, x, area_y, w, area_h, 5, 3)
    pinned(d, x, H - M - BOT_H, w)
    text(d, (RAIL + GAP, H - 18),
         f"2  one module at a time, cells {cw:.0f} by {chh:.0f}, picker is a row of buttons", 13, DIM)
    return canvas


def source_tall():
    canvas = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(canvas)
    chrome(d)
    x = RAIL + GAP
    w = W - M - x
    y0 = M + TOP_H + GAP
    src_h = 258
    band_h = 148
    y1 = y0 + src_h + GAP
    y2 = y1 + band_h + GAP

    cx = x
    for name, fr in zip(["OSC", "FILTER", "AMP"], [.34, .30, .30]):
        tw = w * fr
        if name == "OSC":
            cw, chh = osc_grid(d, cx, y0, tw - GAP, src_h, 4, 3)
        else:
            panel(d, cx, y0, tw - GAP, src_h, name)
        cx += tw

    hero_w = w * .26
    d.rectangle([x, y1, x + hero_w, y1 + band_h], fill=HERO)
    text(d, (x + hero_w / 2, y1 + band_h / 2), "HERO CITY", 16, INK, "mm")
    cx = x + hero_w + GAP
    rest = w - hero_w - GAP
    for name, fr in zip(["LFO", "MOD MATRIX", "AMP MODS"], [.24, .30, .22]):
        tw = rest * fr
        panel(d, cx, y1, tw - GAP, band_h, name)
        cx += tw
    panel(d, cx, y1, W - M - cx, band_h, "SAMPLED / DRUM EDIT")

    cx = x
    for name, fr in zip(["INSERTS", "VINTAGE", "GLOBAL", "DRUM PADS"], [.30, .22, .24, .24]):
        tw = w * fr
        panel(d, cx, y2, tw - GAP, band_h, name)
        cx += tw

    pinned(d, x, H - M - BOT_H, w)
    text(d, (RAIL + GAP, H - 18),
         f"3  source row tall at 258, the other two bands at 148, cells {cw:.0f} by {chh:.0f}", 13, DIM)
    return canvas


sheet = Image.new("RGB", (W, H * 3 + 60), (0, 0, 0))
sheet.paste(two_tall_rows(), (0, 0))
sheet.paste(one_big_module(), (0, H + 30))
sheet.paste(source_tall(), (0, 2 * (H + 30)))
OUT.parent.mkdir(parents=True, exist_ok=True)
sheet.save(OUT)
print(f"wrote {OUT} {sheet.size}")
