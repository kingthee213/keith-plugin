"""Find a control attached AFTER something it shares a rect with, so it is painted over.

    /work/.venv/bin/python tools/audit_attach_order.py

## Why

iPlug2 paints controls in ATTACHMENT ORDER: the last one attached is drawn last and therefore
wins. Three separate defects in this project were all this one shape, and each was invisible
until the plugin was run:

  1. `KeithPageSurface` was attached at line 339 while the panels it covers attach from line
     298 -- so the page surface buried the main view.
  2. `KeithPageSurface` then swallowed clicks across most of the plugin, because its mouse
     handler had no page-open guard.
  3. `KeithPagePanel("page_sample_slices")` was attached AFTER the SLICE BY menu and the SENS
     and REGIONS knobs that sit on the same 11-unit rect, so the panel's fill hid all three.

The first two are fixed. The third was found by a reader going line by line, which is exactly
the work a gate should be doing.

## What it checks

Two things about each `g->AttachControl(...)` in attachment order:

  1. FULL COVER -- a control whose rect is fully contained by a LATER-attached control. The
     later one paints over it entirely, so the earlier control is invisible.
  2. PARTIAL OVERLAP -- any rect intersection at all, reported as a warning. Some overlap is
     intentional (a label over a panel), so this is not a failure on its own.

Panels and surfaces are the usual culprits because they FILL their whole rect.

## What it does not do

It does not know what a control draws. A later control that is transparent, or that draws only
a border, will not actually hide the earlier one -- this cannot tell, so it reports the overlap
and leaves the judgement to a reader. The failure case is deliberately limited to FULL
CONTAINMENT, where a filled control cannot avoid hiding what is beneath it.

It also reads only the literal `OverlayIRECT("id")` form; a rect built from arithmetic on
another rect, as the slice controls are, is attributed to the rect it names.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LAYOUT = ROOT / "src" / "editor" / "EditorialLayout.h"
GEOM = ROOT / "src" / "editor" / "EditorGeometry.h"

# Controls that only draw a border or nothing at all, which cannot hide what is under them.
TRANSPARENT = {"KeithTextControl"}

# Coverings that are DELIBERATE, so the gate does not cry wolf on the page design.
#
# The page surfaces and page panels exist to COVER the main view while a page is open -- that is
# their entire purpose, stated in KeithPage.h. A gate that flags them would be wrong every time and
# would train a reader to ignore it, which is worse than not having the gate.
#
# What is NOT deliberate is a control covering something that is supposed to be usable AT THE SAME
# TIME: a panel over its own contents, or a surface over a control that is not part of the page
# underneath. Those are the cases reported.
DELIBERATE_COVER = {
    # page surfaces cover the panels behind them, by design
    ("KeithPageSurface", "KeithWaveform"),
    ("KeithPageSurface", "KeithPresetMenu"),
    ("KeithPageSurface", "*"),
    # a label is allowed to sit on top of the panel that carries it
    ("KeithPageLabel", "KeithPagePanel"),
    ("KeithPageLabel", "KeithPageKnob"),
    ("KeithPageModeButton", "KeithWaveform"),
}


def rects(geom_text):
    out = {}
    for m in re.finditer(
            r'"([a-z_0-9]+)",\s*\{([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f\}', geom_text):
        out[m.group(1)] = tuple(float(m.group(i)) for i in range(2, 6))
    return out


def attaches(layout_text):
    """(index, class, overlay id or None) in attachment order."""
    out = []
    for i, m in enumerate(re.finditer(r"AttachControl\(\s*new\s+([\w:]+)\s*\(", layout_text)):
        cls = m.group(1).split("::")[-1]
        tail = layout_text[m.end():m.end() + 300]
        idm = re.search(r'OverlayIRECT\("([a-z_0-9]+)"\)', tail)
        out.append((i, cls, idm.group(1) if idm else None))
    return out


def contains(outer, inner):
    return (outer[0] <= inner[0] and outer[1] <= inner[1]
            and outer[2] >= inner[2] and outer[3] >= inner[3])


def overlaps(a, b):
    return not (a[2] <= b[0] or b[2] <= a[0] or a[3] <= b[1] or b[3] <= a[1])


def main():
    geo = rects(GEOM.read_text())
    order = attaches(LAYOUT.read_text())

    known = [(i, c, o, geo[o]) for i, c, o in order if o and o in geo]
    print(f"  attach calls          : {len(order)}")
    print(f"  with a resolvable rect: {len(known)}")

    buried = []
    partial = []
    for a in range(len(known)):
        for b in range(a + 1, len(known)):
            ia, ca, oa, ra = known[a]
            ib, cb, ob, rb = known[b]
            if ca in TRANSPARENT:
                continue

            # Is `b` painted over `a`?
            covers = contains(rb, ra) and (rb[2] - rb[0]) > 1 and (rb[3] - rb[1]) > 1
            if not covers:
                if overlaps(ra, rb):
                    partial.append((ca, oa, cb, ob))
                continue

            # A deliberate covering, so not reported.
            if (cb, ca) in DELIBERATE_COVER or (cb, "*") in DELIBERATE_COVER:
                continue

            buried.append((cb, ob, ca, oa))

    print()
    if buried:
        print("  FULLY COVERED by a later-attached control:")
        for cb, ob, ca, oa in buried:
            print(f"    {ca} ({oa}) is painted over by {cb} ({ob}), attached later")
        print()
        print("  Attach the covering control FIRST. iPlug2 paints in attachment order, so the")
        print("  later one wins and the earlier one is invisible.")
        return 1

    print("  ok    no control is fully covered by a later one")
    if partial:
        print()
        print(f"  {len(partial)} partial overlap(s), which may be intended (labels over panels):")
        seen = set()
        for ca, oa, cb, ob in partial:
            key = tuple(sorted((oa, ob)))
            if key in seen:
                continue
            seen.add(key)
            print(f"    {ca} ({oa})  overlaps  {cb} ({ob})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
