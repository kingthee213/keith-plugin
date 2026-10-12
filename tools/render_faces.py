#!/usr/bin/env python3
"""Procedural comic editor faces for MAGS SYNTH.

The look is inked, not vector-clean: wobbly edges drawn as jittered paths, thick black outlines
twice over, cross-hatching in the shadowed thirds, drawn wood grain with grime and scratches, a
torn black frame, and paper grain over the whole board. Panels are never perfect rectangles.

Draws the single chassis face the plugin loads, at exactly 3x the 1280x800 design canvas. Every
panel rectangle comes from src/editor/EditorGeometry.h, the same table the live controls are placed
from, so a painted panel edge cannot drift away from the control it frames.

Pillow only. No image files, no downloads, no generated raster whose licence we cannot name. The
wordmark is drawn from the Bangers face and the panel titles from Oswald Bold, both OFL 1.1 and both
already embedded in the plugin.

Needs Pillow, which the system python does not have. Run it from the shared tools venv:

    bash tools/setup-tools-venv.sh
    ~/.venvs/keith-measure/bin/python tools/render_faces.py

Writes resources/images/chassis.png at 3840x2400 and fails if the size is wrong.
"""
from __future__ import annotations

import math
import json
import random
import re
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageFont

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
OUT = ROOT / "resources" / "images" / "chassis.png"
FONT_HEAD = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"
FONT_SPRAY = ROOT / "resources" / "fonts" / "Bangers-Regular.ttf"
# The MAGS wordmark is set in blackletter (Pirata One, OFL 1.1). SYNTH stays a bold
# grotesque -- blackletter for the whole thing is too heavy to read at this size.
FONT_OLDE = ROOT / "resources" / "fonts" / "PirataOne-Regular.ttf"

# 2, NOT 3. This is the fix for the editor that would not appear.
#
# The face was raised to 3x (4200x2304) while chasing a gate that claimed the canvas was
# 1280x800. That made it WIDER THAN 4096, which is the maximum texture dimension on macOS GPUs.
# nvgCreateImageRGBA fails above that and returns image id 0; IGraphicsNanoVG::Bitmap stores it
# anyway; IBitmap::IsValid() is only `mAPIBitmap != nullptr`, so it reports TRUE; and
# DrawFittedBitmap then computes PathTransformScale(bounds.W()/bitmap.W(), ...) with bitmap.W()
# of 0. The inf/NaN poisons the transform for everything painted after it, so the window opened
# and NOTHING PAINTED -- no exception, no crash, and the whole editor gone.
#
# 2x of 1400x768 is 2800x1536, which is what the LAST BUILD THAT OPENED bundled (commit
# ed8fc216, run 100). It was correct all along. verify_material_layout.py was the thing that was
# wrong: it asserted (cw * 2, ch * 2) and then I changed it to read 3x off the generator, so the
# gate agreed with the bug. That is the second time in this work a gate was adjusted to match
# broken output instead of the output being fixed.
#
# The ceiling is now enforced rather than asserted, so this cannot recur.
SCALE = 2

# The maximum texture dimension this face may be uploaded at. Every GPU macOS ships supports at
# least 4096; going over means the upload fails and the editor goes blank. Checked below.
MAX_TEXTURE_DIM = 4096

# The design canvas, read from the one place that defines it rather than copied.
#
# This was hardcoded to 1280x800 while config.h and EditorGeometry.h both say 1400x768, so the
# face was generated at the wrong size for every panel drawn on top of it. It shipped that way:
# the bundled chassis.png is 3840x2400 (1280x800 at 3x) against a 1400x768 editor, and
# tools/verify_material_layout.py had been reporting the mismatch all along.
#
# Read from EditorGeometry.h so the face and the controls cannot disagree again. That file is
# already the source of every panel rectangle this script draws, so the size belongs there too.
def _design_canvas():
    text = GEOM.read_text()
    def value(name):
        m = re.search(r"constexpr float " + name + r"\s*=\s*([\d.]+)f", text)
        if not m:
            raise SystemExit(f"render_faces: could not read {name} from {GEOM}")
        return int(round(float(m.group(1))))
    w, h = value("kCanvasW"), value("kCanvasH")
    # Cross-check against config.h, which is what the editor is actually created at. If these two
    # ever disagree the face would be drawn at one size and laid out at another, which is the bug
    # this function exists to prevent.
    cfg = (ROOT / "config.h").read_text()
    cw = re.search(r"#define PLUG_WIDTH\s+(\d+)", cfg)
    ch = re.search(r"#define PLUG_HEIGHT\s+(\d+)", cfg)
    if cw and ch and (int(cw.group(1)), int(ch.group(1))) != (w, h):
        raise SystemExit(
            f"render_faces: EditorGeometry says {w}x{h} but config.h says "
            f"{cw.group(1)}x{ch.group(1)} -- fix one of them before generating the face")
    return w, h


DESIGN_W, DESIGN_H = _design_canvas()
W, H = DESIGN_W * SCALE, DESIGN_H * SCALE

# Refuse to emit a face too large to upload. This is the check whose absence cost five builds: the
# bitmap silently failed to upload, and the failure surfaced as an empty editor rather than as an
# error anywhere.
if max(W, H) > MAX_TEXTURE_DIM:
    raise SystemExit(
        f"the face would be {W}x{H}, and {max(W, H)} exceeds the {MAX_TEXTURE_DIM} maximum "
        f"texture dimension. The upload would fail and the editor would draw nothing. Lower "
        f"SCALE (currently {SCALE}) or the canvas.")


INK = (10, 9, 11)
INK_SOFT = (26, 22, 26)
PAPER = (236, 228, 208)
MAGENTA = (232, 48, 132)
WOOD = (150, 86, 40)
WOOD_LIGHT = (196, 128, 66)
WOOD_DARK = (74, 38, 18)

PANEL_FILL = {
    "charcoal": (46, 54, 62),
    "teal": (52, 104, 108),
    "olive": (116, 112, 62),
}
JITTER = 0.6

CONCEPT = ROOT / "design"
CONCEPT_PALETTE = CONCEPT / "concept-palette.json"
CONCEPT_TILE = {"wood": CONCEPT / "concept-tile-wood.png", "panel": CONCEPT / "concept-tile-charcoal.png"}


def load_concept():
    """Colours and texture patches from the owner's concept render, sampled by
    tools/sample_concept.py. Returns empty structures when the render is absent, so the face
    still renders from the built-in palette."""
    palette, tiles = {}, {}
    if CONCEPT_PALETTE.exists():
        raw = json.loads(CONCEPT_PALETTE.read_text())
        palette = {key: tuple(value) for key, value in raw.items()
                   if isinstance(value, list) and len(value) == 3}
    for key, path in CONCEPT_TILE.items():
        if path.exists():
            tiles[key] = Image.open(path).convert("RGB")
    return palette, tiles


def tiled(image: Image.Image, tile: Image.Image, alpha: float) -> Image.Image:
    """Multiply a patch over an image, tiled at 1:1. Tiling is the point: every pixel of the
    concept render stays at its own resolution instead of being stretched to fit."""
    width, height = image.size
    layer = Image.new("RGB", (width, height))
    for y in range(0, height, tile.size[1]):
        for x in range(0, width, tile.size[0]):
            layer.paste(tile, (x, y))
    flat = Image.new("RGB", (width, height), (255, 255, 255))
    return ImageChops.multiply(image, Image.blend(flat, layer, alpha))


def texture(canvas: Image.Image, tile: Image.Image, box, alpha: float) -> None:
    l, t, r, b = box
    if r - l <= 0 or b - t <= 0:
        return
    canvas.paste(tiled(canvas.crop((l, t, r, b)), tile, alpha), (l, t))


def s(v: float) -> int:
    return int(round(v * SCALE))


def shade(color, factor):
    return tuple(max(0, min(255, int(ch * factor))) for ch in color)


def parse_boxes(header: Path):
    text = header.read_text()
    found = re.findall(r"constexpr Box (k[A-Za-z]+) \{([\d.]+)f, ([\d.]+)f, ([\d.]+)f, ([\d.]+)f\};", text)
    return {name: (float(l), float(t), float(r), float(b)) for name, l, t, r, b in found}


def vertical_gradient(size, top, bottom):
    image = Image.new("RGB", size)
    draw = ImageDraw.Draw(image)
    for y in range(size[1]):
        u = y / max(1, size[1] - 1)
        draw.line([(0, y), (size[0], y)],
                  fill=tuple(int(top[i] * (1 - u) + bottom[i] * u) for i in range(3)))
    return image


def wobble(box, rng, amount=JITTER):
    """A rectangle as a hand-drawn path. Borderlands panels are never straight."""
    l, t, r, b = box
    step = max(14.0, (r - l) / 22.0)
    points = []

    def walk(x0, y0, x1, y1):
        length = math.hypot(x1 - x0, y1 - y0)
        count = max(2, int(length / step))
        for i in range(count):
            u = i / count
            x = x0 + (x1 - x0) * u
            y = y0 + (y1 - y0) * u
            points.append((x + rng.uniform(-amount, amount) * SCALE,
                           y + rng.uniform(-amount, amount) * SCALE))

    walk(l, t, r, t)
    walk(r, t, r, b)
    walk(r, b, l, b)
    walk(l, b, l, t)
    return points


def hatch(layer_box, rng, spacing, angle_deg, alpha, width=1):
    """Diagonal pen shading, drawn on its own layer so it can be masked to a panel."""
    l, t, r, b = layer_box
    layer = Image.new("L", (r - l, b - t), 0)
    draw = ImageDraw.Draw(layer)
    span = r - l + b - t
    rad = math.radians(angle_deg)
    dx, dy = math.cos(rad), math.sin(rad)
    for i in range(int(span / spacing)):
        offset = i * spacing
        points = []
        for step in range(0, span, 18):
            px = offset + step * dx - (b - t) * 0.5
            py = step * dy
            if -40 <= px <= (r - l) + 40 and -40 <= py <= (b - t) + 40:
                points.append((px, py))
        if len(points) > 1:
            wobbled = [(x + rng.uniform(-1.2, 1.2), y + rng.uniform(-1.2, 1.2)) for x, y in points]
            draw.line(wobbled, fill=alpha, width=width)
    return layer


def wood(canvas: Image.Image, rng: random.Random, tiles: dict) -> None:
    canvas.paste(vertical_gradient((W, H), WOOD_LIGHT, WOOD), (0, 0))
    draw = ImageDraw.Draw(canvas, "RGBA")

    plank = s(52)
    for index, x in enumerate(range(-plank, W + plank, plank)):
        tone = rng.uniform(0.88, 1.1)
        draw.polygon(wobble((x, 0, x + plank, H), rng, 0.9), fill=shade(WOOD, tone) + (135,))

        for _ in range(34):
            gx = rng.uniform(x + s(3), x + plank - s(3))
            points = [(gx, -s(8))]
            drift = rng.uniform(-s(4), s(4))
            for y in range(0, H + s(20), s(48)):
                points.append((gx + drift * math.sin(y / float(H) * math.pi * rng.uniform(1.2, 3.2)) + rng.uniform(-1.5, 1.5), y))
            draw.line(points, fill=shade(WOOD_DARK, rng.uniform(0.45, 0.95)) + (rng.randint(30, 90),),
                      width=rng.choice((1, 1, 2)))

        if rng.random() < 0.4:
            kx, ky = rng.uniform(x + s(10), x + plank - s(10)), rng.uniform(0, H)
            for ring in range(5, 0, -1):
                rx = s(ring * 2.2)
                draw.ellipse((kx - rx, ky - rx * 1.6, kx + rx, ky + rx * 1.6),
                             outline=shade(WOOD_DARK, 0.5) + (85,), width=s(1))

        seam = wobble((x + plank, 0, x + plank, H), rng, 1.1)
        draw.line(seam, fill=INK + (170,), width=s(1) + 1)
        draw.line([(p[0] - s(1), p[1]) for p in seam], fill=shade(WOOD_LIGHT, 1.05) + (90,), width=s(1))

    wood_hatch = hatch((0, 0, W, H), rng, s(9), 68, 34, width=1)
    canvas.paste(Image.new("RGB", (W, H), shade(WOOD_DARK, 0.5)), (0, 0),
                 wood_hatch.filter(ImageFilter.GaussianBlur(1)))

    if "wood" in tiles:
        texture(canvas, tiles["wood"], (0, 0, W, H), 0.55)

    grime = Image.new("L", (W, H), 0)
    gdraw = ImageDraw.Draw(grime)
    for _ in range(220):
        gx, gy = rng.uniform(0, W), rng.uniform(0, H)
        rad = rng.uniform(s(8), s(60))
        gdraw.ellipse((gx - rad, gy - rad * 0.6, gx + rad, gy + rad * 0.6),
                      fill=rng.randint(20, 70))
    grime = grime.filter(ImageFilter.GaussianBlur(s(10)))
    canvas.paste(Image.new("RGB", (W, H), (18, 12, 8)), (0, 0), grime)

    scratches = ImageDraw.Draw(canvas, "RGBA")
    for _ in range(320):
        x0, y0 = rng.uniform(0, W), rng.uniform(0, H)
        length = rng.uniform(s(6), s(70))
        angle = rng.uniform(-0.4, 0.4) + (math.pi if rng.random() < 0.5 else 0)
        light = rng.random() < 0.55
        color = shade(WOOD_LIGHT, 1.1) + (rng.randint(30, 80),) if light else INK + (rng.randint(20, 60),)
        scratches.line((x0, y0, x0 + math.cos(angle) * length, y0 + math.sin(angle) * length),
                       fill=color, width=1)

    vignette = Image.new("L", (W, H), 0)
    vdraw = ImageDraw.Draw(vignette)
    for i in range(70):
        vdraw.rectangle((s(70 - i), s(70 - i), W - s(70 - i), H - s(70 - i)),
                        outline=int(110 * (i / 70.0) ** 2))
    canvas.paste(Image.new("RGB", (W, H), (14, 10, 8)), (0, 0),
                 vignette.filter(ImageFilter.GaussianBlur(s(22))))


def torn_frame(canvas: Image.Image, rng: random.Random) -> None:
    """The black ragged border the reference uses around the whole board."""
    mask = Image.new("L", (W, H), 0)
    draw = ImageDraw.Draw(mask)
    depth = s(7)
    outer = [(0, 0), (W, 0), (W, H), (0, H)]
    inner_points = []
    for x in range(0, W, 9):
        inner_points.append((x, depth * rng.uniform(0.5, 1.3)))
    for y in range(0, H, 9):
        inner_points.append((W - depth * rng.uniform(0.5, 1.3), y))
    for x in range(W, 0, -9):
        inner_points.append((x, H - depth * rng.uniform(0.5, 1.3)))
    for y in range(H, 0, -9):
        inner_points.append((depth * rng.uniform(0.5, 1.3), y))
    draw.polygon([(0, 0), (W, 0), (W, H), (0, H)], fill=0)
    draw.polygon(outer + inner_points, fill=255)
    canvas.paste(Image.new("RGB", (W, H), INK), (0, 0), mask)


def screws(draw, box, rng) -> None:
    l, t, r, b = box
    radius = s(5)
    for px, py in ((l + s(9), t + s(9)), (r - s(9), t + s(9)), (l + s(9), b - s(9)), (r - s(9), b - s(9))):
        px += rng.uniform(-2, 2)
        py += rng.uniform(-2, 2)
        draw.ellipse((px - radius, py - radius, px + radius, py + radius), fill=(0, 0, 0, 150))
        draw.ellipse((px - radius * 0.8, py - radius * 0.8, px + radius * 0.8, py + radius * 0.8),
                     fill=(150, 148, 142), outline=INK, width=s(1))
        draw.line((px - radius * 0.5, py, px + radius * 0.5, py), fill=INK + (220,), width=s(1))


def draw_panel(canvas: Image.Image, draw, box, fill, title: str, rng: random.Random,
               tiles: dict | None = None) -> None:
    l, t, r, b = box
    pw, ph = r - l + 1, b - t + 1

    shadow = Image.new("L", (W, H), 0)
    ImageDraw.Draw(shadow).polygon(wobble((l, t + s(3), r, b), rng, 0.5), fill=210)
    canvas.paste(Image.new("RGB", (W, H), INK), (0, 0), shadow.filter(ImageFilter.GaussianBlur(s(8))))

    plate = vertical_gradient((pw, ph), shade(fill, 1.24), shade(fill, 0.86))
    if tiles and "panel" in tiles:
        plate = tiled(plate, tiles["panel"], 0.5)
    mask = Image.new("L", (pw, ph), 0)
    ImageDraw.Draw(mask).polygon([(x - l, y - t) for x, y in wobble(box, rng)], fill=255)
    canvas.paste(plate, (l, t), mask)

    shading = ImageChops.multiply(hatch((l, t, r + 1, b + 1), rng, s(5), 70, 20), mask)
    canvas.paste(Image.new("RGB", (pw, ph), shade(fill, 0.62)), (l, t), shading)

    grime = Image.new("L", (pw, ph), 0)
    gdraw = ImageDraw.Draw(grime)
    for _ in range(26):
        gx, gy = rng.uniform(0, pw), rng.uniform(0, ph)
        rad = rng.uniform(s(4), s(20))
        gdraw.ellipse((gx - rad, gy - rad * 0.55, gx + rad, gy + rad * 0.55), fill=rng.randint(20, 55))
    canvas.paste(Image.new("RGB", (pw, ph), (16, 11, 9)), (l, t),
                 ImageChops.multiply(grime.filter(ImageFilter.GaussianBlur(s(8))), mask))

    for _ in range(rng.randint(30, 55)):
        cx = rng.uniform(l, r)
        cy = rng.choice((rng.uniform(t, t + (b - t) * 0.2), rng.uniform(b - (b - t) * 0.2, b)))
        size = rng.uniform(s(2), s(7))
        draw.rectangle((cx, cy, cx + size, cy + size * 0.7),
                       fill=shade(fill, rng.uniform(0.5, 0.75)) + (rng.randint(90, 170),))

    for _ in range(rng.randint(20, 40)):
        x0, y0 = rng.uniform(l, r), rng.uniform(t, b)
        length = rng.uniform(s(4), s(34))
        angle = rng.uniform(0, math.pi)
        draw.line((x0, y0, x0 + math.cos(angle) * length, y0 + math.sin(angle) * length),
                  fill=(PAPER if rng.random() < 0.5 else (0, 0, 0)) + (rng.randint(25, 70),), width=1)

    for width, offset in ((s(4), 0.0), (s(2), 2.6)):
        draw.line(wobble((l + s(offset), t + s(offset), r - s(offset), b - s(offset)), rng), fill=INK,
                  width=width, joint="curve")

    draw.line([(p[0], p[1]) for p in wobble((l + s(3), t + s(3), r - s(3), t + s(3)), rng)],
              fill=shade(fill, 1.9) + (130,), width=s(2))

    screws(draw, box, rng)

    if title:
        font = ImageFont.truetype(str(FONT_HEAD), s(15))
        position = (l + s(12), t + s(8))
        draw.text((position[0] + s(1), position[1] + s(1)), title, fill=(0, 0, 0, 200), font=font)
        draw.text(position, title, fill=PAPER, font=font)


def draw_cityscape(canvas: Image.Image, box, rng: random.Random) -> None:
    l, t, r, b = box
    width, height = r - l, b - t

    sky = Image.blend(vertical_gradient((width, height), (238, 58, 126), (74, 32, 110)),
                      vertical_gradient((width, height), (252, 138, 178), (44, 20, 78)), 0.42)
    draw = ImageDraw.Draw(sky, "RGBA")
    for _ in range(160):
        y = rng.gauss(height * 0.28, height * 0.10)
        if 0 <= y < height:
            draw.line([(0, y), (width, y)], fill=(255, 196, 226, rng.randint(8, 20)), width=s(2))
    draw.ellipse((-width * 0.3, height * 0.12, width * 0.8, height * 0.5), fill=(255, 224, 240, 30))

    layers = [
        ((92, 62, 128), height * 0.44, s(56), s(124), 0.55),
        ((58, 38, 88), height * 0.56, s(66), s(164), 0.8),
        ((20, 14, 28), height * 0.68, s(72), s(214), 1.0),
    ]
    glow = Image.new("L", (width, height), 0)
    glow_draw = ImageDraw.Draw(glow)

    for color, roof, min_h, max_h, brightness in layers:
        x = -s(20)
        tops = []
        while x < width + s(20):
            bw = rng.randint(s(38), s(96))
            bh = rng.randint(min_h, max_h)
            top = roof - bh * 0.45
            draw.polygon([(x, top), (x + bw, top), (x + bw, height), (x, height)], fill=color + (255,))
            draw.line([(x, top), (x + bw, top)], fill=INK + (200,), width=s(2))
            tops.append((x, top, x + bw))
            if rng.random() < 0.3:
                tw = bw * rng.uniform(0.22, 0.34)
                tx = x + bw * 0.5 - tw * 0.5
                draw.rectangle((tx, top - height * 0.10, tx + tw, top), fill=color + (255,))
                draw.polygon([(tx - s(5), top - height * 0.10), (tx + tw + s(5), top - height * 0.10),
                              (tx + tw * 0.5, top - height * 0.14)], fill=color + (255,))
                draw.line([(tx, top), (tx, top - height * 0.10)], fill=INK + (170,), width=s(1))
            if rng.random() < 0.2:
                draw.line([(x + bw * 0.5, top), (x + bw * 0.5, top - height * 0.09)], fill=INK + (200,), width=s(2))
            x += bw + rng.randint(s(3), s(14))

        for bx, btop, bright in tops:
            for wy in range(int(btop + s(16)), height - s(10), s(15)):
                for wxx in range(int(bx + s(9)), int(bright - s(9)), s(13)):
                    if rng.random() > (0.42 if brightness > 0.9 else 0.58):
                        box_pts = (wxx, wy, wxx + s(6), wy + s(9))
                        alpha = 255 if brightness > 0.9 else int(160 * brightness)
                        draw.rectangle(box_pts, fill=(255, 200, 104, alpha), outline=INK + (140,), width=1)
                        if brightness > 0.9:
                            glow_draw.rectangle(box_pts, fill=120)

    canvas.paste(sky, (l, t))
    canvas.paste(Image.new("RGB", (width, height), (255, 214, 150)), (l, t),
                 glow.filter(ImageFilter.GaussianBlur(s(7))).point(lambda v: int(v * 0.7)))

    cable = ImageDraw.Draw(canvas, "RGBA")
    for _ in range(8):
        x0 = l + rng.uniform(0, width * 0.3)
        x1 = l + rng.uniform(width * 0.65, width)
        y0 = t + rng.uniform(height * 0.32, height * 0.6)
        y1 = t + rng.uniform(height * 0.32, height * 0.6)
        sag = rng.uniform(s(18), s(50))
        points = [(x0 + (x1 - x0) * (i / 24.0), y0 + (y1 - y0) * (i / 24.0)
                   + math.sin((i / 24.0) * math.pi) * sag) for i in range(25)]
        cable.line(points, fill=INK + (200,), width=s(1) + 1)

    city_hatch = hatch((l, t, r, b), rng, s(9), 62, 22)
    canvas.paste(Image.new("RGB", (width, height), (40, 20, 48)), (l, t), city_hatch)

    ImageDraw.Draw(canvas, "RGBA").rectangle((l, t, r, b), outline=INK, width=s(2))
    draw_ink_border(canvas, box, rng)


def draw_ink_border(canvas, box, rng) -> None:
    draw = ImageDraw.Draw(canvas, "RGBA")
    for width, offset in ((s(5), 0.0), (s(2), 3.0)):
        l, t, r, b = box
        draw.line(wobble((l + s(offset), t + s(offset), r - s(offset), b - s(offset)), rng),
                  fill=INK, width=width, joint="curve")


def draw_wordmark(canvas: Image.Image, box, rng: random.Random) -> None:
    """Hand-painted letters: a rough painted edge, then a heavy ink outline behind them."""
    l, t, r, b = box
    size = (r - l, b - t)
    font = ImageFont.truetype(str(FONT_SPRAY), s(46))
    text = "MAGS\nSYNTH"
    origin = (s(8), s(0))

    body = Image.new("L", size, 0)
    ImageDraw.Draw(body).multiline_text(origin, text, fill=255, font=font, spacing=s(1))
    paint = body.point(lambda v: 255 if v > 100 else 0)

    layer = Image.new("RGBA", size, (0, 0, 0, 0))
    outline = Image.new("L", size, 0)
    for dx, dy in ((s(3), s(3)), (-s(3), -s(3)), (s(3), -s(3)), (-s(3), s(3))):
        outline = ImageChops.lighter(outline, ImageChops.offset(paint, dx, dy))
    outline = ImageChops.lighter(outline, paint)
    layer.paste(Image.new("RGBA", size, INK + (255,)), (0, 0), outline)
    layer.paste(Image.new("RGBA", size, MAGENTA + (255,)), (0, 0), paint)

    top_edge = ImageChops.offset(paint, 0, -s(2))
    layer.paste(Image.new("RGBA", size, (255, 158, 204, 255)), (0, 0),
                ImageChops.subtract(paint, top_edge).point(lambda v: int(v * 0.7)))

    speck = ImageDraw.Draw(layer)
    for _ in range(80):
        sx, sy = rng.uniform(0, size[0]), rng.uniform(0, size[1])
        if paint.getpixel((int(min(sx, size[0] - 1)), int(min(sy, size[1] - 1)))) > 60:
            rad = rng.uniform(s(0.4), s(1.8))
            speck.ellipse((sx - rad, sy - rad, sx + rad, sy + rad), fill=MAGENTA + (rng.randint(100, 200),))

    layer = layer.rotate(rng.uniform(-3.0, -1.2), resample=Image.BICUBIC, expand=False)
    canvas.paste(layer, (l, t), layer)


def paper_grain(canvas: Image.Image, rng: random.Random) -> None:
    grain = Image.effect_noise((W, H), 26).convert("L").point(lambda v: int(v * 0.16))
    canvas.paste(Image.new("RGB", (W, H), (255, 252, 240)), (0, 0), grain)

    halftone = Image.new("L", (W, H), 0)
    hdraw = ImageDraw.Draw(halftone)
    step = s(6)
    for y in range(0, H, step):
        for x in range(0, W, step):
            if (x // step + y // step) % 2 == 0:
                hdraw.ellipse((x, y, x + s(1), y + s(1)), fill=rng.randint(18, 42))
    canvas.paste(Image.new("RGB", (W, H), INK), (0, 0), halftone)

    flecks = ImageDraw.Draw(canvas, "RGBA")
    for _ in range(900):
        fx, fy = rng.uniform(0, W), rng.uniform(0, H)
        rad = rng.uniform(0.6, 2.4)
        flecks.ellipse((fx - rad, fy - rad, fx + rad, fy + rad),
                       fill=(INK if rng.random() < 0.7 else PAPER) + (rng.randint(30, 90),))


def main() -> None:
    global WOOD, WOOD_LIGHT, WOOD_DARK, MAGENTA, INK
    rng = random.Random(20261008)
    boxes = parse_boxes(GEOM)
    missing = [k for k in ("kPanelOsc", "kPanelAmpMods", "kWordmarkBox", "kPanelFx") if k not in boxes]
    if missing:
        raise SystemExit(f"EditorGeometry.h is missing {missing}")

    palette, tiles = load_concept()
    if palette:
        WOOD = palette.get("wood", WOOD)
        WOOD_LIGHT = shade(WOOD, 1.35)
        WOOD_DARK = shade(WOOD, 0.42)
        MAGENTA = palette.get("paint", MAGENTA)
        INK = shade(palette.get("charcoal", INK), 0.4)
        for key in PANEL_FILL:
            if key in palette:
                PANEL_FILL[key] = palette[key]
        print(f"concept render sampled: wood {WOOD} panels {PANEL_FILL} paint {MAGENTA}")
    else:
        print("concept render absent, using the built-in palette")

    canvas = Image.new("RGB", (W, H), INK)
    wood(canvas, rng, tiles)
    draw = ImageDraw.Draw(canvas, "RGBA")

    styles = [
        ("kPanelOsc", "charcoal", "OSCILLATORS"), ("kPanelFilter", "teal", "FILTER"),
        ("kPanelAmp", "charcoal", "AMP"), ("kPanelLfo", "charcoal", "LFO"),
        ("kPanelMod", "charcoal", "MOD MATRIX"),
        ("kPanelAmpMods", "charcoal", "AMP MODIFIERS"),
        ("kPanelSample", "charcoal", "LOADED SOUNDS"), ("kPanelPads", "charcoal", "DRUMS"),
        ("kPanelVintage", "olive", "VINTAGE ANALOG"), ("kPanelGlobal", "charcoal", "GLOBAL"),
        ("kPanelFx", "olive", "EFFECTS"),
    ]
    for key, style, title in styles:
        if key not in boxes:
            continue
        box = tuple(s(v) for v in boxes[key])
        if style is None:
            draw_cityscape(canvas, box, rng)
        else:
            draw_panel(canvas, draw, box, PANEL_FILL[style], title, rng, tiles)

    draw_wordmark(canvas, tuple(s(v) for v in boxes["kWordmarkBox"]), rng)
    paper_grain(canvas, rng)
    torn_frame(canvas, rng)

    # Checked against the canvas this run actually used, not against the number that was hardcoded
    # here before. The old literal (3840, 2400) was the 1280x800 face and it is what let the wrong
    # size ship: the assertion agreed with the generator because both were wrong the same way.
    if canvas.size != (W, H):
        raise SystemExit(f"bad size {canvas.size}, expected {(W, H)}")
    OUT.parent.mkdir(parents=True, exist_ok=True)
    canvas.save(OUT, optimize=True)
    print(f"wrote {OUT} {canvas.size[0]}x{canvas.size[1]} {OUT.stat().st_size / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
