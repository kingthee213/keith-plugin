"""Check the layout renders by measuring them, not by eye.

Two jobs.

1. Geometry. Every header the render scripts draw records its boxes in a HEADER_GEOM list (see
   tools/mockup_states.py row_header(), tools/mockup_rows.py, tools/wireframe.py row()/box()).
   This walks that trail and asserts, for every title:
     * the title's ink box clears the container's top edge by at least 8 px
     * no title ink box overlaps any switcher/chip box
     * the switcher starts to the right of the title's measured textlength
     * the body top equals container top + the one header band number
   It prints the numbers it measured.

2. Output. Each rendered sheet is opened and checked for size, for a real spread of colours and
   for not being a blank or black frame.

Also asserts that the oscillator trace really is generated from shape + PW, by comparing the
computed traces for different values.

    ~/.venvs/keith-measure/bin/python tools/check_layout_render.py

Exit status is non-zero if any assertion fails.
"""

import sys
from pathlib import Path

from PIL import Image, ImageStat

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
sys.path.insert(0, str(TOOLS))

import mockup_states  # noqa: E402  (renders at import)
import mockup_rows  # noqa: E402  (renders at import)
import wireframe  # noqa: E402

# wireframe.py only renders under __main__; drive its states here so the geometry trail is
# populated too (the file itself is written by running the script).
for _s in wireframe.STATES:
    wireframe.state(*_s)

import wireframe_osc  # noqa: E402  (renders at import)
import osc_wireframe_in_B  # noqa: E402  (renders at import)
from osc_wave import wave_points, wave_value  # noqa: E402

FAILURES = []


def check(ok, message):
    print(f"  {'PASS' if ok else 'FAIL'}  {message}")
    if not ok:
        FAILURES.append(message)


def geometry_trail():
    trail = []
    trail += mockup_states.HEADER_GEOM
    trail += mockup_rows.HEADER_GEOM
    trail += wireframe.HEADER_GEOM
    return trail


def check_geometry():
    trail = geometry_trail()
    print(f"\n== header geometry ({len(trail)} headers drawn)")
    print(f"   {'script':<24} {'title':<12} {'top':>5} {'band':>5} {'title_top-top':>14} "
          f"{'titles_right':>13} {'first_chip_x':>13}")
    seen_scripts = set()
    for e in trail:
        seen_scripts.add(e["script"])
        ct = e["container_top"]
        tb = e["title_box"]
        band = e["band"]
        gap_top = tb[1] - ct
        chips = e["chip_boxes"]
        first_chip_x = chips[0][0] if chips else float("nan")
        print(f"   {e['script']:<24} {e['title']:<12.12} {ct:>5.0f} {band:>5} "
              f"{gap_top:>14.1f} {tb[2]:>13.1f} {first_chip_x:>13.1f}")
        check(gap_top >= 8,
              f"{e['script']} {e['title']!r}: title clears container top by "
              f"{gap_top:.1f} px (need >= 8)")
        check(abs((e["body_top"] - ct) - band) < 1e-6,
              f"{e['script']} {e['title']!r}: body top is container top + band "
              f"({e['body_top'] - ct:.1f} vs {band})")
        for cb in chips:
            overlap = not (tb[2] <= cb[0] or cb[2] <= tb[0] or tb[3] <= cb[1] or cb[3] <= tb[1])
            check(not overlap,
                  f"{e['script']} {e['title']!r}: title box {tb} vs chip {cb} overlap={overlap}")
            check(cb[0] >= tb[2],
                  f"{e['script']} {e['title']!r}: chip starts at {cb[0]:.1f}, "
                  f"right of the title ink edge {tb[2]:.1f}")
    check(len(seen_scripts) >= 4,
          f"geometry came from {sorted(seen_scripts)}")


SHEETS = [
    ("mockup-states.png", ROOT / "design" / "screenshots" / "mockup-states.png", (1280, 2460), 3),
    ("mockup-rows.png", ROOT / "design" / "screenshots" / "mockup-rows.png", (1280, 1630), 2),
    ("osc-wireframe-B.png", ROOT / "design" / "screenshots" / "osc-wireframe-B.png", (1280, 800), 1),
    ("wireframe-rows.png", ROOT / "design" / "screenshots" / "wireframe-rows.png", (1280, 2472), 3),
    ("wireframe-osc-variants.png",
     ROOT / "design" / "screenshots" / "wireframe-osc-variants.png", (1280, 2472), 3),
]


def check_sheets():
    print("\n== rendered sheets")
    for name, path, size, frames in SHEETS:
        if not path.exists():
            check(False, f"{name}: missing at {path}")
            continue
        im = Image.open(path).convert("RGB")
        check(im.size == size, f"{name}: size {im.size} (expected {size})")
        stat = ImageStat.Stat(im)
        bright = sum(stat.mean) / 3.0
        spread = sum(stat.stddev) / 3.0
        hist = im.convert("L").histogram()
        total = im.size[0] * im.size[1]
        near_black = sum(hist[:8]) / total
        print(f"   {name:<26} {im.size[0]}x{im.size[1]}  mean={bright:5.1f}  "
              f"stddev={spread:5.1f}  near-black={near_black * 100:4.1f}%")
        check(spread > 5.0, f"{name}: colour spread {spread:.1f} (need > 5, not a blank frame)")
        check(near_black < 0.35, f"{name}: near-black {near_black * 100:.1f}% (< 35%)")
        frame_h = size[1] // frames if frames == 1 else 800
        gap = size[1] - frames * frame_h
        for i in range(frames):
            top = i * (frame_h + gap // frames) if frames > 1 else 0
            box = (0, top, size[0], min(size[1], top + frame_h))
            fr = im.crop(box)
            colours = len(fr.getcolors(maxcolors=1 << 20) or [])
            fstat = ImageStat.Stat(fr)
            fspread = sum(fstat.stddev) / 3.0
            check(colours > 100 and fspread > 5.0,
                  f"{name}: frame {i} {box} has {colours} colours, spread {fspread:.1f}")


def check_wave():
    print("\n== oscillator trace is generated from shape + PW (tools/osc_wave.py)")
    probe = [i / 200.0 for i in range(200)]
    diffs = [abs(wave_value(p, 0, 50) - wave_value(p, 100, 50)) for p in probe]
    best_p = probe[diffs.index(max(diffs))]
    print(f"   max |shape 0% - shape 100%| at pw 50% over one cycle: {max(diffs):.3f} "
          f"(largest at phase {best_p:.3f})")
    check(max(diffs) > 0.1, f"shape changes the trace (max delta {max(diffs):.3f})")

    def duty_fraction(pw):
        n = 1000
        pts = [wave_value(i / n, 100, pw) for i in range(n)]
        return sum(1 for v in pts if v > 0.0) / n

    high50, high25, high75 = duty_fraction(50), duty_fraction(25), duty_fraction(75)
    print(f"   hard square (shape 100%) high fraction: pw 25% -> {high25:.2f}, "
          f"pw 50% -> {high50:.2f}, pw 75% -> {high75:.2f}")
    check(high25 < high50 < high75, "pw narrows and widens the pulse monotonically")

    a = wave_points(0, 0, 120, 40, 0, 50)
    b = wave_points(0, 0, 120, 40, 100, 50)
    c = wave_points(0, 0, 120, 40, 100, 25)
    ys = [p[1] for p in a]
    print(f"   trace spread at shape 0 / pw 50: min y {min(ys):.1f}, max y {max(ys):.1f}")
    check(max(ys) - min(ys) > 1.0, "the trace is not a flat line")
    check(a != b, "shape 0% and shape 100% draw different traces")
    check(b != c, "pw 50% and pw 25% draw different traces")


if __name__ == "__main__":
    check_geometry()
    check_sheets()
    check_wave()
    print(f"\n{len(FAILURES)} failures")
    for f in FAILURES:
        print(f"  - {f}")
    sys.exit(1 if FAILURES else 0)
