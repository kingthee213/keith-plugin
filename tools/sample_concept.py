"""Pull the palette and texture tiles out of the owner's concept render.

The concept render is 1536x1024. The face is 3840x2400. Stretching the render to fit would be a
2.5x upscale, which is exactly the softness this project already rejected once. So the render is used
as a source of materials at its own resolution: colour sampled from it, and texture patches cropped
at 1:1 and tiled. A tiled patch keeps every pixel it started with.

Writes:
  design/concept-palette.json      sampled colours, for render_faces.py to read
  design/concept-tex-wood.png      a wood patch, cropped 1:1
  design/concept-tex-panel.png     a painted panel patch, cropped 1:1
  design/concept-art.png           the cityscape plate, cropped 1:1
  design/concept-swatch.png        the above, laid out for a human to check

Usage: ~/.venvs/keith-measure/bin/python tools/sample_concept.py
"""
from __future__ import annotations

import json
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "design" / "mags-synth-concept.png"

# Normalised boxes, chosen by looking at the render. Everything here is a patch that is
# unambiguously one material: the rail is wood, the big right panel is charcoal, the plate is the
# skyline, and the wordmark corner is paint.
BOXES = {
    "wood": (0.00, 0.30, 0.10, 0.72),
    "charcoal": (0.63, 0.03, 0.80, 0.17),
    "teal": (0.300, 0.04, 0.365, 0.12),
    "olive": (0.15, 0.82, 0.30, 0.95),
    "art": (0.13, 0.555, 0.49, 0.72),
}


def find_paint(render: Image.Image) -> tuple[int, int, int]:
    """The paint colour is the most saturated magenta anywhere in the render. Scanning for it
    beats guessing a box, which landed on bare wood twice, and beats averaging a patch, which
    mixed the dark outline into the median."""
    width, height = render.size
    best, best_score = (232, 48, 132), -1
    for y in range(0, height, 2):
        for x in range(0, width, 2):
            r, g, b = render.getpixel((x, y))
            score = min(r, b) - g
            if score > best_score:
                best_score, best = score, (r, g, b)
    return best


def median_colour(image: Image.Image) -> tuple[int, int, int]:
    pixels = sorted(image.getdata(), key=lambda c: sum(c))
    return pixels[len(pixels) // 2]


def main() -> None:
    if not SOURCE.exists():
        raise SystemExit(f"concept render not found at {SOURCE}")
    render = Image.open(SOURCE).convert("RGB")
    width, height = render.size

    palette = {}
    crops = {}
    for name, (l, t, r, b) in BOXES.items():
        box = (int(l * width), int(t * height), int(r * width), int(b * height))
        region = render.crop(box)
        crops[name] = region
        palette[name] = list(median_colour(region))
        print(f"{name:9s} box={box} size={region.size} median={tuple(palette[name])}")

    palette["paint"] = list(find_paint(render))
    print(f"{'paint':9s} auto-detected median={tuple(palette['paint'])}")

    quantised = render.quantize(colors=10, method=Image.MEDIANCUT).convert("RGB")
    counts = {}
    for pixel in quantised.getdata():
        counts[pixel] = counts.get(pixel, 0) + 1
    top = sorted(counts.items(), key=lambda kv: -kv[1])[:10]
    total = width * height
    print("\ntop ten colours in the render, by share:")
    for colour, count in top:
        print(f"  {colour}  {100.0 * count / total:5.1f}%")
    palette["dominant"] = [[list(colour), round(count / total, 4)] for colour, count in top]

    for name in ("wood", "charcoal", "art"):
        out = ROOT / "design" / f"concept-tex-{name}.png"
        crops[name].save(out)
        print(f"wrote {out} {crops[name].size}")

    # A repeatable tile has to be square, so the panel and wood patches get their largest square.
    for name in ("wood", "charcoal"):
        region = crops[name]
        side = min(region.size)
        region.crop((0, 0, side, side)).save(ROOT / "design" / f"concept-tile-{name}.png")

    crops["art"].save(ROOT / "design" / "concept-art.png")

    swatch = Image.new("RGB", (1500, 420), (24, 24, 24))
    draw = ImageDraw.Draw(swatch)
    x = 8
    for name in ("wood", "charcoal", "teal", "olive", "paint"):
        colour = tuple(palette[name])
        draw.rectangle((x, 8, x + 120, 128), fill=colour, outline=(255, 255, 255))
        draw.text((x, 134), name, fill=(255, 255, 255))
        x += 130
    small = crops["art"].resize((600, int(600 * crops["art"].size[1] / crops["art"].size[0])))
    swatch.paste(small, (8, 170))
    tile = Image.open(ROOT / "design" / "concept-tile-wood.png")
    shown = tile.crop((0, 0, min(300, tile.size[0]), min(240, tile.size[1])))
    swatch.paste(shown, (620, 170))
    panel = Image.open(ROOT / "design" / "concept-tile-charcoal.png")
    swatch.paste(panel.crop((0, 0, min(300, panel.size[0]), min(240, panel.size[1]))), (930, 170))
    swatch.save(ROOT / "design" / "concept-swatch.png")
    print(f"wrote {ROOT / 'design' / 'concept-swatch.png'}")

    (ROOT / "design" / "concept-palette.json").write_text(json.dumps(palette, indent=2) + "\n")
    print(f"wrote {ROOT / 'design' / 'concept-palette.json'}")


if __name__ == "__main__":
    main()
