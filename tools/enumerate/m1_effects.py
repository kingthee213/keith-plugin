#!/usr/bin/env python3
"""Extract the M1 effect-type list from the M1 Owner's Manual OCR text.

The M1 Owner's Manual describes each effect type in its own chapter section,
headed by the number and name (`23. EXCITER`, `25. ROTARY SPEAKER`, ...). This
tool reads those headings out of the manual's OCR text and keeps the contiguous
run that starts at 1.

Usage:
    python m1_effects.py <manual-ocr.txt> <out.json>
"""

from __future__ import annotations

import argparse
import json
import re
import sys

HEAD_RE = re.compile(r"(?m)^\s*(\d{1,2})\.\s*([A-Z][A-Z0-9 /\-\.&\']{2,44})\s*$")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("manual_text")
    ap.add_argument("out")
    args = ap.parse_args(argv)

    text = open(args.manual_text, encoding="utf-8", errors="replace").read()
    found = {}
    for m in HEAD_RE.finditer(text):
        num = int(m.group(1))
        name = re.sub(r"\s+", " ", m.group(2)).strip()
        if 1 <= num <= 40:
            found.setdefault(num, [])
            if name not in found[num]:
                found[num].append(name)

    entries = []
    expected = 1
    while expected in found:
        variants = found[expected]
        entries.append(
            {
                "number": str(expected),
                "name": variants[0],
                "ocr_variants": variants[1:],
            }
        )
        expected += 1

    with open(args.out, "w", encoding="utf-8") as fh:
        json.dump(
            {
                "source": args.manual_text,
                "entries": entries,
                "count": len(entries),
                "next_missing": None if expected > 33 else expected,
            },
            fh,
            indent=1,
            ensure_ascii=False,
        )
    print("wrote", args.out, "effect types", len(entries), "| first missing number", expected)
    for e in entries:
        print(f"  {e['number']:>2} {e['name']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
