"""Cut the owner's artwork out of his reference render.

The reference is the spec, and it already contains the cityscape, the hardware graphic, the wood rail
with the wordmark plaque, and the wood frame edges. Re-deriving those in code is what produced a
recreation he called a mess. These crops are the assets.

Coordinates are in reference pixels and are recorded here so the crops are reproducible rather than
hand-tuned once. Run from the repo root:

    ~/.venvs/keith-measure/bin/python tools/extract_reference_assets.py
"""

from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
REFERENCE = Path("/Users/solace/TYH_M1/dev/mags-modulator/inbox/mags-synth-final-ui-reference.png")
OUT = ROOT / "resources" / "reference"

CROPS = {
    "left-column.png": (0, 0, 246, 1024),
    "wood-frame-top.png": (0, 0, 1536, 16),
    "wood-frame-right.png": (1496, 0, 1536, 1024),
    "wood-frame-bottom.png": (0, 1006, 1536, 1024),
    "hero-city.png": (258, 430, 808, 732),
    "vintage-rig.png": (262, 800, 465, 908),
}

PANEL_FILLS = {
    "kPanelOsc": (16, 29, 34),
    "kPanelFilter": (37, 88, 103),
    "kPanelAmp": (13, 28, 34),
    "kPanelLfo": (4, 11, 14),
    "kPanelMod": (11, 29, 36),
    "kPanelSample": (9, 24, 30),
    "kPanelPads": (11, 22, 26),
    "kPanelVintage": (73, 77, 43),
    "kPanelGlobal": (19, 29, 26),
    "kPanelFx": (19, 29, 26),
}


def main():
    if not REFERENCE.exists():
        raise SystemExit(f"reference not found at {REFERENCE}")
    reference = Image.open(REFERENCE).convert("RGB")
    if reference.size != (1536, 1024):
        raise SystemExit(f"reference is {reference.size}, expected 1536x1024")

    OUT.mkdir(parents=True, exist_ok=True)
    for name, box in CROPS.items():
        reference.crop(box).save(OUT / name)
        print(f"{name:24s} {box} -> {reference.crop(box).size}")

    print("\npanel fills, for the generator:")
    for key, value in PANEL_FILLS.items():
        print(f"  {key:16s} {value}")


if __name__ == "__main__":
    main()
