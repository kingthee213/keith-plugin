#!/usr/bin/env python3
"""Procedural comic editor faces for MAGS SYNTH."""
from __future__ import annotations
import math
import random
import re
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
OUT = ROOT / "resources" / "images" / "chassis.png"
FONT = ROOT / "resources" / "fonts" / "Bangers-Regular.ttf"
SCALE = 3
W, H = 1280 * SCALE, 800 * SCALE
PALETTE = {
    "wood": (107, 74, 46), "wood_dark": (74, 50, 32), "charcoal": (42, 45, 48),
    "teal": (47, 92, 88), "olive": (107, 106, 69), "ink": (10, 10, 10), "paper": (234, 225, 205),
}

def parse_boxes(header: Path) -> dict[str, tuple[int, int, int, int]]:
    text = header.read_text()
    return {name: (int(l), int(t), int(r), int(b)) for name, l, t, r, b in re.findall(
        r"constexpr Box (k\w+) \{(\d+)f, (\d+)f, (\d+)f, (\d+)f\};", text)}

def s(v: float) -> int:
    return int(round(v * SCALE))

def wood_tile(size: int = 96) -> Image.Image:
    tile = Image.new("RGB", (size, size), PALETTE["wood"])
    draw = ImageDraw.Draw(tile)
    random.seed(17)
    for y in range(0, size, 6):
        shade = random.randint(-12, 12)
        c = tuple(max(0, min(255, ch + shade)) for ch in PALETTE["wood"])
        draw.line([(0, y), (size, y)], fill=c, width=2)
    for x in range(0, size, 18):
        draw.line([(x, 0), (x, size)], fill=PALETTE["wood_dark"], width=1)
    return tile

def tiled(img: Image.Image, tile: Image.Image, box: tuple[int, int, int, int]) -> None:
    l, t, r, b = box
    tw, th = tile.size
    for y in range(t, b, th):
        for x in range(l, r, tw):
            img.paste(tile, (x, y))

def draw_panel(img, draw, box, fill, title=""):
    l, t, r, b = box
    draw.rectangle((l + s(4), t + s(4), r + s(4), b + s(4)), fill=(0, 0, 0, 90))
    draw.rectangle((l, t, r, b), fill=fill, outline=PALETTE["ink"], width=s(4))
    brad = s(4)
    for px, py in ((l + s(6), t + s(6)), (r - s(6), t + s(6)), (l + s(6), b - s(6)), (r - s(6), b - s(6))):
        draw.ellipse((px - brad, py - brad, px + brad, py + brad), fill=(180, 180, 180), outline=PALETTE["ink"], width=s(1))
    if title:
        try: font = ImageFont.truetype(str(FONT), s(14))
        except OSError: font = ImageFont.load_default()
        draw.text((l + s(10), t + s(6)), title, fill=PALETTE["paper"], font=font)

def draw_cityscape(img, draw, box):
    l, t, r, b = box
    sky = Image.new("RGB", (r - l, b - t))
    sdraw = ImageDraw.Draw(sky)
    for y in range(b - t):
        u = y / max(1, b - t)
        color = (int(255 * (1 - u) + 120 * u), int(120 * (1 - u) + 60 * u), int(200 * (1 - u) + 180 * u))
        sdraw.line([(0, y), (r - l, y)], fill=color)
    img.paste(sky, (l, t))
    base = b - s(40)
    rng = random.Random(31)
    x = l + s(8)
    buildings = []
    while x < r - s(20):
        w = rng.randint(s(26), s(64)); h = rng.randint(s(70), s(180))
        buildings.append((x, base - h, x + w, base)); x += w + rng.randint(s(4), s(12))
    for bl, bt, br, bb in buildings:
        draw.rectangle((bl, bt, br, bb), fill=(24, 20, 34), outline=PALETTE["ink"], width=s(2))
        for wy in range(bt + s(10), bb - s(8), s(14)):
            for wx in range(bl + s(8), br - s(8), s(12)):
                if rng.random() > 0.45:
                    draw.rectangle((wx, wy, wx + s(6), wy + s(8)), fill=(255, 196, 96))

def draw_wordmark(img, box):
    l, t, r, b = box
    try: font = ImageFont.truetype(str(FONT), s(42))
    except OSError: font = ImageFont.load_default()
    layer = Image.new("RGBA", (r - l, b - t), (0, 0, 0, 0))
    draw = ImageDraw.Draw(layer)
    text = "MAGS\nSYNTH"
    for ox, oy in ((-2, 0), (2, 0), (0, -2), (0, 2)):
        draw.multiline_text((s(8) + ox, s(4) + oy), text, fill=(10, 10, 10, 255), font=font, spacing=s(4))
    draw.multiline_text((s(8), s(4)), text, fill=(255, 79, 161, 255), font=font, spacing=s(4))
    img.paste(layer, (l, t), layer)

def main():
    boxes = parse_boxes(GEOM)
    img = Image.new("RGBA", (W, H), (0, 0, 0, 255))
    tile = wood_tile()
    tiled(img, tile, (0, 0, W, H))
    draw = ImageDraw.Draw(img, "RGBA")
    styles = {
        "kPanelOsc": ("charcoal", "OSCILLATORS"), "kPanelFilter": ("teal", "FILTER"),
        "kPanelAmp": ("charcoal", "AMP"), "kPanelLfo": ("charcoal", "LFO"),
        "kPanelMod": ("charcoal", "MOD MATRIX"), "kPanelArt": ("art", ""),
        "kPanelSample": ("charcoal", "LOADED SOUNDS"), "kPanelPads": ("charcoal", "DRUM PADS"),
        "kPanelVintage": ("olive", "VINTAGE ANALOG"), "kPanelGlobal": ("charcoal", "GLOBAL"),
        "kPanelFx": ("olive", "EFFECTS"),
    }
    fills = {"charcoal": PALETTE["charcoal"], "teal": PALETTE["teal"], "olive": PALETTE["olive"]}
    for key, (style, title) in styles.items():
        if key not in boxes: continue
        box = tuple(s(v) for v in boxes[key])
        if style == "art":
            draw_cityscape(img, draw, box)
            draw.rectangle(box, outline=PALETTE["ink"], width=s(4))
        else:
            draw_panel(img, draw, box, fills[style], title)
    draw_wordmark(img, tuple(s(v) for v in boxes["kWordmarkBox"]))
    img = img.convert("RGB")
    OUT.parent.mkdir(parents=True, exist_ok=True)
    if img.size != (3840, 2400): raise SystemExit(f"bad size {img.size}")
    img.save(OUT)
    print(f"wrote {OUT}")

if __name__ == "__main__":
    main()
