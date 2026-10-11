"""Audit the editor for controls the page layers can make unreachable.

    /work/.venv/bin/python tools/audit_editor_input.py

## Why

Two faults were reported from the built plugin and both were of this shape:

  - "i cant click any of the drums pads ... most of the knobs and buttons arent allowing
    me to select them" -- KeithPageSurface swallowed clicks unconditionally, and it spans
    essentially the whole plugin, so eight controls underneath it were dead whether or not
    a page was open.

  - "the drop down list wont let me click it" -- the preset menu sits inside the
    page_wavetable rect, and the surface was attached after it, so it won the hit-test.

Neither was visible to any existing gate. The layout gates check that things FIT; none of
them check that things can be CLICKED. This does.

## What it checks

  1. Overlap. Every non-page control whose rect intersects a page surface's rect. Every one
     of these is a control that a page can intercept, so every one needs a guard on the
     page being open -- or needs to not be under the page at all.

  2. Attachment order. iPlug2 hit-tests in reverse attachment order, so a control attached
     LATER sits on top. A page surface attached after a control it overlaps will win.

  3. Guard presence. Whether the classes that can intercept input return early when their
     page is closed. Missing guard + overlap = dead control.

## What it does not do

It does not run the editor. It reads geometry and source, so it cannot know what a control
does when clicked, only whether a click can reach it. And the overlap test is
rectangle-based: two controls may overlap on paper without visual conflict if one is
transparent, which this cannot see.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GEOMETRY = ROOT / "src" / "editor" / "EditorGeometry.h"
LAYOUT = ROOT / "src" / "editor" / "EditorialLayout.h"
PAGE = ROOT / "src" / "editor" / "KeithPage.h"


def rects(path):
    """id -> (l, t, r, b) from the generated geometry."""
    out = {}
    text = path.read_text()
    for m in re.finditer(r'"([a-z_0-9]+)",\s*\{([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f\}',
                         text):
        out[m.group(1)] = tuple(float(m.group(i)) for i in range(2, 6))
    return out


def overlaps(a, b):
    return not (a[2] <= b[0] or b[2] <= a[0] or a[3] <= b[1] or b[3] <= a[1])


def attach_order(text):
    """Every AttachControl, in source order, with the overlay id it uses if any."""
    order = []
    for m in re.finditer(r"AttachControl\(new\s+([\w:]+)\s*\(", text):
        cls = m.group(1).split("::")[-1]
        # The id is the first OverlayIRECT("...") after this attach, if there is one.
        tail = text[m.end():m.end() + 260]
        idm = re.search(r'OverlayIRECT\("([a-z_0-9]+)"\)', tail)
        order.append((cls, idm.group(1) if idm else None))
    return order


def guarded_classes(page_text):
    """Class name -> does its OnMouseDown return early when the page is closed?"""
    out = {}
    for m in re.finditer(r"class\s+(\w+)\s*:\s*public\s+(\w+)", page_text):
        name, base = m.group(1), m.group(2)
        body = page_text[m.end():m.end() + 6000]
        mouse = re.search(r"void OnMouseDown\([^)]*\)\s*override\s*\{(.*?)\n  \}", body, re.S)
        if not mouse:
            out[name] = None      # does not handle mouse at all
            continue
        # Either an explicit PageIsOpen() test, or a local Open() helper, or a state check.
        guarded = ("PageIsOpen" in mouse.group(1) or "Open()" in mouse.group(1))
        out[name] = guarded
    return out


def main():
    geo = rects(GEOMETRY)
    layout = LAYOUT.read_text()
    page = PAGE.read_text()

    page_ids = {k: v for k, v in geo.items() if k.startswith("page_") and
                "_" not in k[len("page_"):] or k in ("page_sampler", "page_wavetable")}
    page_ids = {k: v for k, v in geo.items() if k in ("page_sampler", "page_wavetable")}
    if not page_ids:
        print("  FAIL  no page surface rects found -- has the geometry changed?")
        return 1

    guards = guarded_classes(page)
    order = attach_order(layout)
    pos = {}
    for i, (cls, oid) in enumerate(order):
        if oid:
            pos[oid] = i

    problems = []
    covered_total = 0

    for pid, prect in sorted(page_ids.items()):
        covered = []
        for cid, crect in geo.items():
            if cid.startswith("page_") or cid == pid:
                continue
            if overlaps(crect, prect):
                covered.append(cid)
        covered_total += len(covered)

        print(f"  {pid}: {len(covered)} non-page control(s) under it")
        for cid in sorted(covered):
            # Did a page surface attach after this control?
            later = [oid for oid, i in pos.items()
                     if oid in page_ids and i > pos.get(cid, 10 ** 6)]
            notes = []
            if later:
                notes.append("page surface attached AFTER it")
            print(f"      {cid:22s} {'; '.join(notes) if notes else 'under the page rect'}")

    # Are the intercepting classes guarded?
    #
    # KeithPageButton is deliberately EXEMPT. It is the SAMPLER / WAVETABLE tab, and a tab that
    # only answered while its own page was already open could never be used to open it. Every other
    # page class guards, because every other one can only intercept a click meant for something
    # underneath.
    EXEMPT = {"KeithPageButton"}
    interceptors = [c for c, g in guards.items() if g is not None and "Page" in c]
    unguarded = [c for c in interceptors if guards[c] is False and c not in EXEMPT]
    print()
    print(f"  page classes that handle mouse: {len(interceptors)}")
    for c in sorted(interceptors):
        exempt = c in EXEMPT
        mark = "n/a" if exempt else ("ok " if guards[c] else "FAIL")
        why = "  (a page TAB: must work while its page is closed)" if exempt else ""
        print(f"      {mark} {c:26s} guarded={guards[c]}{why}")
    if unguarded:
        problems.append(f"unguarded page classes that swallow input: {', '.join(sorted(unguarded))}")

    if covered_total == 0:
        print("  ok    no non-page control sits under a page surface")
    else:
        # Overlap alone is only a problem if the interceptor is unguarded, which is checked above.
        print(f"  note  {covered_total} control(s) are under a page rect, which is fine only"
              " because the page controls all guard on the page being open")

    if problems:
        print()
        for p in problems:
            print(f"  FAIL  {p}")
        return 1

    print()
    print("  ok    every page control that handles mouse guards on the page being open,")
    print("        so a closed page cannot intercept a click meant for what is under it")
    return 0


if __name__ == "__main__":
    sys.exit(main())
