"""Compose the MAGS SYNTH face from the owner's reference artwork.

Supersedes the procedural painter in render_faces.py. That one drew wood over the whole canvas and
re-invented the cityscape, which is the "recreation looks like dookie" the owner called out. The
reference render already contains the cityscape, the hardware graphic, the wood rail with the
wordmark plaque and the wood frame edges, and this assembles those crops over the panel rectangles
the controls are placed from.

Composes at 1152x768 (0.75 of the reference grid) with artwork crops scaled to match, then upscales once
to 2x for the hi-dpi face. At the editor's default scale of 1.0 the face maps back to exactly 1152x768.

    ~/.venvs/keith-measure/bin/python tools/compose_face.py
"""

from __future__ import annotations

import re
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
ASSETS = ROOT / "resources" / "reference"
OUT = ROOT / "resources" / "images" / "chassis.png"
FONT_HEAD = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"

DESIGN = (1152, 768)
SCALE = 2
CANVAS_BG = (10, 11, 13)
TITLE_INK = (236, 228, 208)
TITLE_PX = 11
TITLE_PAD_X = 14
TITLE_PAD_Y = 6

PANELS = {
    "kPanelOsc": ("OSCILLATORS", (16, 29, 34)),
    "kPanelFilter": ("FILTER", (37, 88, 103)),
    "kPanelAmp": ("AMP", (13, 28, 34)),
    "kPanelLfo": ("LFO", (4, 11, 14)),
    "kPanelMod": ("MOD MATRIX", (11, 29, 36)),
    "kPanelAnalyzer": ("SPECTRAL ANALYZER", (7, 20, 26)),
    "kPanelAmpMods": ("AMP MODIFIERS", (9, 24, 30)),
    "kPanelSample": ("LOADED SOUNDS", (9, 24, 30)),
    "kPanelPads": ("DRUM PADS", (11, 22, 26)),
    "kPanelVintage": ("VINTAGE ANALOG MODELING", (73, 77, 43)),
    "kPanelGlobal": ("GLOBAL", (19, 29, 26)),
    "kPanelFx": ("EFFECTS", (19, 29, 26)),
}

KEYBOARD_KEY = "kKeyboardBox"
BORDER = (6, 7, 9)


def parse_boxes():
    text = GEOM.read_text(errors="replace")
    boxes = {}
    for name, numbers in re.findall(r"constexpr Box (k\w+) \{([^}]*)\};", text):
        parts = [v.strip().rstrip("f") for v in numbers.split(",") if v.strip()]
        if len(parts) != 4 or not all(re.fullmatch(r"-?\d+(\.\d+)?", p) for p in parts):
            continue
        boxes[name] = tuple(float(p) for p in parts)
    return boxes


def frame(canvas, box, ink=BORDER):
    ImageDraw.Draw(canvas, "RGBA").rectangle(
        [box[0], box[1], box[2] - 1, box[3] - 1], outline=ink, width=2)


def title(draw, box, text):
    font = ImageFont.truetype(str(FONT_HEAD), TITLE_PX)
    spaced = " ".join(text)
    draw.text((box[0] + TITLE_PAD_X, box[1] + TITLE_PAD_Y), spaced, fill=TITLE_INK, font=font)


def main():
    boxes = parse_boxes()
    canvas = Image.new("RGB", DESIGN, CANVAS_BG)
    draw = ImageDraw.Draw(canvas, "RGBA")

    for key, (name, fill) in PANELS.items():
        if key not in boxes:
            print(f"  {key} missing from the geometry, skipped")
            continue
        box = [int(v) for v in boxes[key]]
        draw.rectangle(box, fill=fill, outline=BORDER, width=2)
        title(draw, box, name)

    # The analyzer replaced the city plate. Its well is painted by the panel loop below,
    # since it now has an entry in PANELS; nothing extra to paste.
    vintage = boxes.get("kPanelVintage")
    if vintage:
        rig = Image.open(ASSETS / "vintage-rig.png").convert("RGB")
        slot = (int(vintage[0]) + 6, int(vintage[1]) + 44)
        canvas.paste(rig, slot)

    rail = Image.open(ASSETS / "left-column.png").convert("RGB")
    canvas.paste(rail, (0, 0))

    top = Image.open(ASSETS / "wood-frame-top.png").convert("RGB")
    canvas.paste(top, (0, 0))
    right = Image.open(ASSETS / "wood-frame-right.png").convert("RGB")
    canvas.paste(right, (DESIGN[0] - right.width, 0))

    bottom = Image.open(ASSETS / "wood-frame-bottom.png").convert("RGB")
    keyboard = boxes.get(KEYBOARD_KEY)
    if keyboard:
        left_corner = bottom.crop((0, 0, int(keyboard[0]), bottom.height))
        canvas.paste(left_corner, (0, DESIGN[1] - bottom.height))
        right_corner = bottom.crop((int(keyboard[2]), 0, bottom.width, bottom.height))
        canvas.paste(right_corner, (int(keyboard[2]), DESIGN[1] - bottom.height))
    else:
        canvas.paste(bottom, (0, DESIGN[1] - bottom.height))

    if canvas.size != DESIGN:
        raise SystemExit(f"composition is {canvas.size}, expected {DESIGN}")
    face = canvas.resize((DESIGN[0] * SCALE, DESIGN[1] * SCALE), Image.LANCZOS)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    face.save(OUT, optimize=True)
    print(f"wrote {OUT} {face.size[0]}x{face.size[1]} {OUT.stat().st_size / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
