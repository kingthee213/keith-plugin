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

from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageFont

ROOT = Path(__file__).resolve().parents[1]
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"
ASSETS = ROOT / "resources" / "reference"
OUT = ROOT / "resources" / "images" / "chassis.png"
FONT_HEAD = ROOT / "resources" / "fonts" / "Oswald-Bold.ttf"
# MAGS is set in blackletter (Pirata One, OFL 1.1) -- chosen for legibility at this size,
# where the more ornate faces tangle. SYNTH stays a bold grotesque beneath it.
FONT_OLDE = ROOT / "resources" / "fonts" / "PirataOne-Regular.ttf"

# The wordmark plaque in the rail asset. Measured from the artwork: the plaque's straight
# edges run wider than the lettering, and the fill colour is sampled from its own corner.
PLAQUE = (33, 15, 183, 166)

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
    "kPanelAmpMods": ("AMP MODIFIERS", (9, 24, 30)),
    "kPanelSample": ("LOADED SOUNDS", (9, 24, 30)),
    "kPanelPads": ("DRUMS", (11, 22, 26)),
    "kPanelVintage": ("VINTAGE ANALOG MODELING", (73, 77, 43)),
    "kPanelGlobal": ("GLOBAL", (19, 29, 26)),
    "kPanelFx": ("EFFECTS", (19, 29, 26)),
}

KEYBOARD_KEY = "kKeyboardBox"
WORDMARK_KEY = "kWordmarkBox"
# The plugin's accent, from KeithTheme.h:kHotPink. The wordmark uses the same value so the
# painted face and the live controls agree on what the brand colour is.
HOT_PINK = (255, 79, 163)
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


def clear_plaque(rail):
    """Blank the wordmark plaque in the rail so the wordmark can be redrawn.

    The rail asset ships MAGS SYNTH as artwork in the old spray face. Filling the plaque
    with its own background colour removes it without touching the wood around it.
    """
    l, t, r, b = PLAQUE
    fill = rail.getpixel((l + 1, b - 1))
    out = rail.copy()
    out.paste(Image.new("RGB", (r - l + 1, b - t + 1), fill), (l, t))
    return out


def draw_wordmark(canvas, box, scale=1):
    """Paint MAGS in pink blackletter and SYNTH in the white grotesque, centred in the box.

    `box` is in the canvas's own pixel coordinates and `scale` is how many output pixels make
    one design pixel, so the glyph sizes below are expressed in design units and multiplied
    up. Both words are fitted to the box width independently, so neither can overflow it.
    """
    if not box:
        return
    x0, y0, x1, y1 = (int(v) for v in box)
    draw = ImageDraw.Draw(canvas)
    max_w = (x1 - x0) - 4 * scale

    # Start generous and shrink only until each word fits, so the words are as large as the
    # box allows rather than pinned to a fixed ceiling.
    mags_px = 64 * scale
    f_olde = ImageFont.truetype(str(FONT_OLDE), mags_px)
    while mags_px > 12 * scale and draw.textlength("MAGS", font=f_olde) > max_w:
        mags_px -= 1
        f_olde = ImageFont.truetype(str(FONT_OLDE), mags_px)

    synth_px = 52 * scale
    f_synth = ImageFont.truetype(str(FONT_HEAD), synth_px)
    while synth_px > 10 * scale and draw.textlength("SYNTH", font=f_synth) > max_w:
        synth_px -= 1
        f_synth = ImageFont.truetype(str(FONT_HEAD), synth_px)

    def centred(text, font, top, colour, x_override=None):
        wpx = draw.textlength(text, font=font)
        x = x_override if x_override is not None else x0 + ((x1 - x0) - wpx) / 2
        draw.text((x, top), text, font=font, fill=colour)

    # Centre the two words as a block inside the box, using each word's INK bounds rather
    # than the font's line box. A line box carries ascender and descender padding that
    # differs between the blackletter face and the grotesque, so centring on it dropped the
    # pair below the middle of the plaque.
    def ink_box(font, text):
        return draw.textbbox((0, 0), text, font=font)

    gap = 4 * scale
    mags_l, mags_t, mags_r, mags_b = ink_box(f_olde, "MAGS")
    synth_l, synth_t, synth_r, synth_b = ink_box(f_synth, "SYNTH")
    mags_h = mags_b - mags_t
    synth_h = synth_b - synth_t
    mags_w = mags_r - mags_l
    synth_w = synth_r - synth_l
    block_h = mags_h + gap + synth_h
    top = y0 + ((y1 - y0) - block_h) // 2

    mags_x = x0 + ((x1 - x0) - mags_w) / 2
    synth_x = x0 + ((x1 - x0) - synth_w) / 2
    # Flat lettering, matching the reference: MAGS in pink, SYNTH in paper white beneath it.
    centred("MAGS", f_olde, top - mags_t, HOT_PINK, x_override=mags_x - mags_l)
    centred("SYNTH", f_synth, top + mags_h + gap - synth_t, (235, 227, 207),
            x_override=synth_x)


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

    # VINTAGE ANALOG MODELING carries no illustration. The vintage-rig plate used to be
    # pasted here, but the panel now holds nine knobs -- the engine tab and the six tape
    # controls moved up from the wood rail -- and the plate was taking the width they need.

    rail = Image.open(ASSETS / "left-column.png").convert("RGB")
    # The rail asset ships the wordmark as artwork, in the spray face. Clear it so the
    # wordmark can be redrawn below in the current faces -- MAGS in pink blackletter, SYNTH
    # in the white grotesque. Filling the plaque is what removes the old one.
    rail = clear_plaque(rail)
    canvas.paste(rail, (0, 0))
    # The wordmark is NOT drawn here. It is drawn after the upscale, so its glyphs are
    # rendered at full output resolution instead of being painted at 1x and then interpolated
    # up -- that interpolation is what made the letters look soft and pixelated.

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
    # Wordmark last, at output resolution. Text drawn into the 1x canvas and then scaled up
    # comes out soft; drawing it here keeps the glyph edges crisp.
    wm_box = boxes.get(WORDMARK_KEY)
    if wm_box:
        draw_wordmark(face, tuple(v * SCALE for v in wm_box), scale=SCALE)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    face.save(OUT, optimize=True)
    print(f"wrote {OUT} {face.size[0]}x{face.size[1]} {OUT.stat().st_size / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
