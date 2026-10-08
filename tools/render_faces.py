#!/usr/bin/env python3
"""Procedural comic editor faces for MAGS SYNTH.

Draws the single chassis face the plugin loads, at exactly 3x the 1280x800 design canvas.
Every panel rectangle comes from src/editor/EditorGeometry.h, the same table the live
controls are placed from, so a painted panel edge cannot drift away from the control it
frames.

Pillow only, no image files, no downloads, no generated raster the licence of which we
cannot name. The wordmark is drawn from the Oswald and Bangers faces already embedded in
the plugin (both OFL 1.1).

Needs Pillow, which the system python does not have:

    ~/.venvs/keith-art/bin/python tools/render_faces.py

Writes resources/images/chassis.png at 3840x2400 and fails if the size is wrong.
"""
from __future__ import annotations

import math
import random
import re
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageFont

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
OUT = ROOT / "resources" / "images" / "chassis.png"
FONT_HEAD = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"
FONT_SPRAY = ROOT / "resources" / "fonts" / "Bangers-Regular.ttf"

SCALE = 3
W, H = 1280 * SCALE, 800 * SCALE

WOOD = (128, 88, 54)
WOOD_LIGHT = (168, 124, 82)
WOOD_DARK = (70, 45, 26)
INK = (12, 11, 12)
PAPER = (238, 231, 213)
MAGENTA = (255, 62, 158)
PANEL_FILL = {
    "charcoal": (38, 42, 47),
    "teal": (42, 92, 88),
    "olive": (104, 104, 66),
}


def s(v: float) -> int:
    return int(round(v * SCALE))


def shade(color, factor):
    return tuple(max(0, min(255, int(ch * factor))) for ch in color)


def parse_boxes(header: Path) -> dict[str, tuple[float, float, float, float]]:
    text = header.read_text()
    found = re.findall(r"constexpr Box (k[A-Za-z]+) \{([\d.]+)f, ([\d.]+)f, ([\d.]+)f, ([\d.]+)f\};", text)
    return {name: (float(l), float(t), float(r), float(b)) for name, l, t, r, b in found}


def vertical_gradient(size, top, bottom):
    image = Image.new("RGB", size)
    draw = ImageDraw.Draw(image)
    height = size[1]
    for y in range(height):
        u = y / max(1, height - 1)
        draw.line([(0, y), (size[0], y)],
                  fill=tuple(int(top[i] * (1 - u) + bottom[i] * u) for i in range(3)))
    return image


# --------------------------------------------------------------------------- wood frame

def draw_wood(canvas: Image.Image, rng: random.Random) -> None:
    """Vertical planks with grain, seams, knots and a vignette. The reference frame is a
    sawn board, so the grain runs down the page, not across it."""
    canvas.paste(vertical_gradient((W, H), WOOD_LIGHT, WOOD_DARK), (0, 0))
    draw = ImageDraw.Draw(canvas, "RGBA")

    plank = s(46)
    for index, x in enumerate(range(-plank, W + plank, plank)):
        tone = rng.uniform(0.86, 1.12)
        base = shade(WOOD, tone)
        draw.rectangle((x, 0, x + plank, H), fill=base + (150,))

        # A highlight down the left edge of each board and a shadow down the right.
        draw.line([(x + s(1), 0), (x + s(1), H)], fill=shade(WOOD_LIGHT, tone) + (120,), width=s(2))
        draw.line([(x + plank - s(2), 0), (x + plank - s(2), H)], fill=shade(WOOD_DARK, 0.7) + (150,), width=s(3))

        # Grain: long wandering lines, darkest in the middle of the board.
        for _ in range(26):
            gx = rng.uniform(x + s(2), x + plank - s(2))
            alpha = rng.randint(18, 46)
            width = rng.choice((1, 1, 2))
            drift = rng.uniform(-s(3), s(3))
            points = [(gx, -s(10))]
            for y in range(0, H + s(20), s(60)):
                points.append((gx + drift * math.sin(y / float(H) * math.pi * rng.uniform(1.5, 3.5)), y))
            draw.line(points, fill=shade(WOOD_DARK, rng.uniform(0.55, 0.85)) + (alpha,), width=width)

        if rng.random() < 0.35:
            kx, ky = rng.uniform(x + s(8), x + plank - s(8)), rng.uniform(0, H)
            for ring in range(4, 0, -1):
                radius = s(ring * 2.4)
                draw.ellipse((kx - radius, ky - radius * 1.5, kx + radius, ky + radius * 1.5),
                             outline=shade(WOOD_DARK, 0.6) + (70,), width=s(1))

    # A gentle darkening at the edges so the frame reads as one lit object.
    vignette = Image.new("L", (W, H), 0)
    vdraw = ImageDraw.Draw(vignette)
    for i in range(60):
        alpha = int(70 * (i / 60.0) ** 2)
        inset = s(60 - i)
        vdraw.rectangle((inset, inset, W - inset, H - inset), outline=alpha)
    vignette = vignette.filter(ImageFilter.GaussianBlur(s(18)))
    canvas.paste(Image.new("RGB", (W, H), (0, 0, 0)), (0, 0), vignette)


# ------------------------------------------------------------------------------- panel

def panel_shadow(canvas: Image.Image, box, radius: int = 10, spread: int = 7) -> None:
    l, t, r, b = box
    mask = Image.new("L", (W, H), 0)
    ImageDraw.Draw(mask).rectangle((l, t + s(2), r, b), fill=200)
    mask = mask.filter(ImageFilter.GaussianBlur(s(spread)))
    canvas.paste(Image.new("RGB", (W, H), (0, 0, 0)), (0, 0), mask)


def draw_panel(canvas: Image.Image, draw, box, fill, title: str, rng: random.Random) -> None:
    l, t, r, b = box
    panel_shadow(canvas, box)

    plate = vertical_gradient((r - l, b - t), shade(fill, 1.16), shade(fill, 0.84))
    canvas.paste(plate, (l, t))

    grain = Image.effect_noise((r - l, b - t), 14).convert("L").point(lambda v: int(v * 0.16))
    canvas.paste(Image.new("RGB", (r - l, b - t), (255, 255, 255)), (l, t), grain)

    draw.rectangle((l, t, r, b), outline=INK, width=s(3))
    draw.line([(l + s(2), t + s(2)), (r - s(2), t + s(2))], fill=shade(fill, 1.6) + (110,), width=s(2))
    draw.line([(l + s(2), t + s(2)), (l + s(2), b - s(2))], fill=shade(fill, 1.45) + (85,), width=s(2))
    draw.line([(l + s(2), b - s(2)), (r - s(2), b - s(2))], fill=(0, 0, 0, 110), width=s(3))
    draw.line([(r - s(2), t + s(2)), (r - s(2), b - s(2))], fill=(0, 0, 0, 90), width=s(2))

    # Chipped paint along the outer edge, so the plate reads as worn metal.
    for _ in range(rng.randint(10, 22)):
        if rng.random() < 0.5:
            cx, cy = rng.uniform(l, r), rng.choice((t, b))
        else:
            cx, cy = rng.choice((l, r)), rng.uniform(t, b)
        size = rng.uniform(s(2), s(6))
        draw.rectangle((cx, cy, cx + size, cy + size), fill=shade(fill, 0.72) + (rng.randint(70, 130),))

    brad = s(4)
    for px, py in ((l + s(7), t + s(7)), (r - s(7), t + s(7)), (l + s(7), b - s(7)), (r - s(7), b - s(7))):
        draw.ellipse((px - brad, py - brad + s(1), px + brad, py + brad + s(1)), fill=(0, 0, 0, 120))
        draw.ellipse((px - brad, py - brad, px + brad, py + brad), fill=(176, 178, 174), outline=INK, width=s(1))
        draw.ellipse((px - brad + s(1), py - brad + s(1), px, py), fill=(228, 230, 226))

    if title:
        font = ImageFont.truetype(str(FONT_HEAD), s(15))
        position = (l + s(11), t + s(7))
        draw.text((position[0] + s(1), position[1] + s(1)), title, fill=(0, 0, 0, 190), font=font)
        draw.text(position, title, fill=PAPER, font=font)


# --------------------------------------------------------------------------- cityscape

def draw_cityscape(canvas: Image.Image, box, rng: random.Random) -> None:
    """The reference plate: a magenta sky over a silhouetted skyline with water towers and
    strung cables. Drawn as three depth layers with a haze between them."""
    l, t, r, b = box
    width, height = r - l, b - t

    sky = Image.new("RGB", (width, height))
    sky = Image.blend(vertical_gradient((width, height), (255, 92, 168), (86, 44, 132)),
                      vertical_gradient((width, height), (255, 150, 196), (58, 30, 96)), 0.45)
    draw = ImageDraw.Draw(sky, "RGBA")

    for i in range(140):
        y = rng.gauss(height * 0.30, height * 0.10)
        if 0 <= y < height:
            draw.line([(0, y), (width, y)], fill=(255, 190, 222, rng.randint(6, 16)), width=s(2))
    draw.ellipse((-width * 0.3, height * 0.14, width * 0.75, height * 0.52), fill=(255, 214, 236, 34))

    layers = [
        ((78, 58, 118), height * 0.46, s(56), s(120), 0.86),
        ((52, 38, 84), height * 0.58, s(64), s(160), 0.92),
        ((22, 17, 32), height * 0.70, s(70), s(210), 1.0),
    ]
    glow = Image.new("L", (width, height), 0)
    glow_draw = ImageDraw.Draw(glow)

    for color, roof, min_h, max_h, glow_amount in layers:
        x = -s(20)
        tops = []
        while x < width + s(20):
            building_w = rng.randint(s(38), s(96))
            building_h = rng.randint(min_h, max_h)
            top = roof - building_h * 0.45
            draw.rectangle((x, top, x + building_w, height), fill=color + (255,))
            tops.append((x, top, x + building_w))
            if rng.random() < 0.3:
                tower_w = building_w * rng.uniform(0.22, 0.34)
                tower_x = x + building_w * 0.5 - tower_w * 0.5
                draw.rectangle((tower_x, top - height * 0.10, tower_x + tower_w, top), fill=color + (255,))
                draw.polygon([(tower_x - s(4), top - height * 0.10), (tower_x + tower_w + s(4), top - height * 0.10),
                              (tower_x + tower_w * 0.5, top - height * 0.14)], fill=color + (255,))
            if rng.random() < 0.18:
                draw.line([(x + building_w * 0.5, top), (x + building_w * 0.5, top - height * 0.09)],
                          fill=color + (255,), width=s(2))
            x += building_w + rng.randint(s(3), s(14))

        if glow_amount > 0.9:
            for bx, btop, bright in tops:
                for wy in range(int(btop + s(16)), height - s(10), s(15)):
                    for wxx in range(int(bx + s(9)), int(bright - s(9)), s(13)):
                        if rng.random() > 0.42:
                            draw.rectangle((wxx, wy, wxx + s(6), wy + s(9)), fill=(255, 198, 104, 255))
                            glow_draw.rectangle((wxx, wy, wxx + s(6), wy + s(9)), fill=110)
        else:
            for bx, btop, bright in tops:
                for wy in range(int(btop + s(16)), height - s(10), s(15)):
                    for wxx in range(int(bx + s(9)), int(bright - s(9)), s(13)):
                        if rng.random() > 0.55:
                            draw.rectangle((wxx, wy, wxx + s(5), wy + s(7)),
                                           fill=(255, 214, 150, int(150 * glow_amount)))

    halo = Image.new("RGB", (width, height), (255, 214, 150))
    canvas.paste(sky, (l, t))
    canvas.paste(halo, (l, t), glow.filter(ImageFilter.GaussianBlur(s(7))).point(lambda v: int(v * 0.75)))

    # Cables between the rooftops, drawn last so they sit in front of the skyline.
    cable = ImageDraw.Draw(canvas, "RGBA")
    for _ in range(7):
        x0 = l + rng.uniform(0, width * 0.3)
        x1 = l + rng.uniform(width * 0.65, width)
        y0 = t + rng.uniform(height * 0.35, height * 0.6)
        y1 = t + rng.uniform(height * 0.35, height * 0.6)
        sag = rng.uniform(s(18), s(46))
        points = []
        for i in range(25):
            u = i / 24.0
            px = x0 + (x1 - x0) * u
            py = y0 + (y1 - y0) * u + math.sin(u * math.pi) * sag
            points.append((px, py))
        cable.line(points, fill=(14, 10, 20, 210), width=s(1) + 1)

    canvas.paste(Image.new("RGB", (width, height), (0, 0, 0)), (l, t),
                 Image.effect_noise((width, height), 8).convert("L").point(lambda v: int(v * 0.10)))
    ImageDraw.Draw(canvas, "RGBA").rectangle((l, t, r, b), outline=INK, width=s(3))


# ---------------------------------------------------------------------------- wordmark

def draw_wordmark(canvas: Image.Image, box, rng: random.Random) -> None:
    """Spray lettering in the owner's direction: two stacked words with a ragged edge, a
    bloom around the paint and a few specks thrown off the nozzle."""
    l, t, r, b = box
    layer = Image.new("RGBA", (r - l, b - t), (0, 0, 0, 0))
    draw = ImageDraw.Draw(layer)
    font = ImageFont.truetype(str(FONT_SPRAY), s(44))
    text = "MAGS\nSYNTH"
    origin = (s(10), s(2))

    body = Image.new("L", layer.size, 0)
    ImageDraw.Draw(body).multiline_text(origin, text, fill=255, font=font, spacing=s(2))
    body = body.point(lambda v: 255 if v > 110 else 0)

    edge = Image.new("L", layer.size, 0)
    edge_draw = ImageDraw.Draw(edge)
    for _ in range(14):
        edge_draw.multiline_text((origin[0] + rng.randint(-s(2), s(2)), origin[1] + rng.randint(-s(2), s(2))),
                                 text, fill=rng.randint(120, 255), font=font, spacing=s(2))
    edge = edge.point(lambda v: 255 if v > 90 else 0)
    mask = ImageChops.lighter(ImageChops.lighter(body, edge).filter(ImageFilter.GaussianBlur(s(1))), body)

    bloom = Image.new("RGBA", layer.size, (0, 0, 0, 0))
    bloom.paste(Image.new("RGBA", layer.size, MAGENTA + (255,)), (0, 0),
                mask.filter(ImageFilter.GaussianBlur(s(9))).point(lambda v: int(v * 0.85)))
    layer = Image.alpha_composite(layer, bloom)

    dark = Image.new("RGBA", layer.size, (0, 0, 0, 0))
    dark.paste(Image.new("RGBA", layer.size, (16, 8, 14, 255)), (s(2), s(3)), mask)
    dark.paste(Image.new("RGBA", layer.size, (16, 8, 14, 255)), (-s(1), -s(1)), mask)
    layer = Image.alpha_composite(layer, dark)

    paint = Image.new("RGBA", layer.size, (0, 0, 0, 0))
    paint.paste(Image.new("RGBA", layer.size, MAGENTA + (255,)), (0, 0), mask)
    paint.paste(Image.new("RGBA", layer.size, (255, 132, 196, 255)), (0, 0),
                mask.filter(ImageFilter.GaussianBlur(s(4))).point(lambda v: int(v * 0.35)))
    layer = Image.alpha_composite(layer, paint)

    for _ in range(70):
        sx, sy = rng.uniform(0, layer.size[0]), rng.uniform(0, layer.size[1])
        if mask.getpixel((int(min(sx, layer.size[0] - 1)), int(min(sy, layer.size[1] - 1)))) > 60:
            rad = rng.uniform(s(0.4), s(1.8))
            draw.ellipse((sx - rad, sy - rad, sx + rad, sy + rad), fill=MAGENTA + (rng.randint(90, 200),))

    layer = layer.rotate(rng.uniform(-3.0, -1.0), resample=Image.BICUBIC, expand=False)
    canvas.paste(layer, (l, t), layer)


# --------------------------------------------------------------------------------- main

def main() -> None:
    rng = random.Random(20261008)
    boxes = parse_boxes(GEOM)
    missing = [k for k in ("kPanelOsc", "kPanelArt", "kWordmarkBox", "kPanelFx") if k not in boxes]
    if missing:
        raise SystemExit(f"EditorGeometry.h is missing {missing}")

    canvas = Image.new("RGB", (W, H), INK)
    draw_wood(canvas, rng)
    draw = ImageDraw.Draw(canvas, "RGBA")

    styles = [
        ("kPanelOsc", "charcoal", "OSCILLATORS"), ("kPanelFilter", "teal", "FILTER"),
        ("kPanelAmp", "charcoal", "AMP"), ("kPanelLfo", "charcoal", "LFO"),
        ("kPanelMod", "charcoal", "MOD MATRIX"), ("kPanelArt", None, ""),
        ("kPanelSample", "charcoal", "LOADED SOUNDS"), ("kPanelPads", "charcoal", "DRUM PADS"),
        ("kPanelVintage", "olive", "VINTAGE ANALOG"), ("kPanelGlobal", "charcoal", "GLOBAL"),
        ("kPanelFx", "olive", "EFFECTS"),
    ]
    for key, style, title in styles:
        if key not in boxes:
            continue
        box = tuple(s(v) for v in boxes[key])
        if style is None:
            panel_shadow(canvas, box)
            draw_cityscape(canvas, box, rng)
        else:
            draw_panel(canvas, draw, box, PANEL_FILL[style], title, rng)

    draw_wordmark(canvas, tuple(s(v) for v in boxes["kWordmarkBox"]), rng)

    # One pass of paper grain over the whole instrument, then a soft bloom off the paint.
    grain = Image.effect_noise((W, H), 12).convert("L").point(lambda v: int(v * 0.09))
    canvas.paste(Image.new("RGB", (W, H), (255, 255, 255)), (0, 0), grain)

    if canvas.size != (3840, 2400):
        raise SystemExit(f"bad size {canvas.size}")
    OUT.parent.mkdir(parents=True, exist_ok=True)
    canvas.save(OUT, optimize=True)
    print(f"wrote {OUT} {canvas.size[0]}x{canvas.size[1]} {OUT.stat().st_size / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
