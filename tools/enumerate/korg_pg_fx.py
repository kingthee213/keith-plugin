#!/usr/bin/env python3
"""Extract the effect-algorithm catalogue and the effect slot rules from a Korg
TRITON-family Parameter Guide.

The Parameter Guide's effect chapter index lists every algorithm as
`NNN: Name`; this tool reads that index out of the document's own text layer.
Nothing is transcribed by hand and nothing is invented: if the index cannot be
read, the tool reports it instead of producing a list.

Usage:
    python korg_pg_fx.py <parameter-guide.pdf> <out.json> [--dump-text <txt>]
"""

from __future__ import annotations

import argparse
import json
import re
import sys

import pdfplumber

INDEX_START = "Filter and dynamics control effects"
RANGE_RE = re.compile(r"(\d{3})\s*:\s*No Effect\s*[-–]\s*(\d{3})\s*:\s*([^\.\(]+)")


def dump_text(pdf_path, txt_path):
    with pdfplumber.open(pdf_path) as pdf:
        parts = []
        for i, page in enumerate(pdf.pages, start=1):
            parts.append(f"@@@PAGE {i}@@@\n" + (page.extract_text() or ""))
    with open(txt_path, "w", encoding="utf-8") as fh:
        fh.write("\n".join(parts))
    return len(parts)


def clean_name(s):
    """Strip the index's dotted leaders and page number from an entry name."""
    s = re.sub(r"(?:\s*\.\s*){3,}\s*\d*\s*$", "", s)
    s = re.sub(r"\s{2,}\d+\s*$", "", s)
    return s.strip()


def parse_index(text):
    """Read `NNN: Name` rows out of the effect chapter index."""
    i = text.find(INDEX_START)
    if i < 0:
        return []
    out = []
    for ln in text[i:].split("\n"):
        s = ln.strip()
        m = re.match(r"^(\d{3}):\s*(.*)$", s)
        if m:
            out.append([m.group(1), clean_name(m.group(2))])
        elif (
            out
            and re.match(r"^\(.*\)[\s\.]*\d{1,3}\s*$", s)
            and not out[-1][1].endswith(")")
        ):
            # an index line whose parenthetical gloss wrapped to the next line
            out[-1][1] = (out[-1][1] + " " + clean_name(s)).strip()
    # keep the contiguous run that starts at 000
    run, expect = [], 0
    for num, name in out:
        if int(num) != expect:
            if run:
                break
            continue
        run.append((num, name))
        expect += 1
    return run


def parse_ranges(text):
    out = []
    for m in RANGE_RE.finditer(text):
        out.append({"from": m.group(1), "to": m.group(2), "name": m.group(3).strip()})
    return out


def parse_slot_rules(text):
    """Pull the slot -> selectable range sentences out of the document."""
    rules = {}
    for pat, key in (
        (r"IFX1, 5 \[000\.\.\.(\d{3})\]", "ifx1_5_max"),
        (r"IFX2, 3, 4 \[000\.\.\.(\d{3})\]", "ifx2_4_max"),
        (r"MFX1, 2 \[000\.\.\.(\d{3})\]", "mfx_max"),
    ):
        m = re.search(pat, text)
        if m:
            rules[key] = m.group(1)
    m = re.search(r"You can select from (\d+) types of effect:\s*000: No Effect", text)
    if m:
        rules["stated_type_count"] = int(m.group(1))
    m = re.search(r"select from (\d+) types\s*of effect: 000: No Effect", text)
    if m:
        rules["stated_type_count_2"] = int(m.group(1))
    return rules


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("pdf")
    ap.add_argument("out")
    ap.add_argument("--dump-text")
    args = ap.parse_args(argv)

    with pdfplumber.open(args.pdf) as pdf:
        pages = len(pdf.pages)
    if args.dump_text:
        print("pages dumped:", dump_text(args.pdf, args.dump_text))

    with open(args.dump_text or " ", encoding="utf-8") as fh:
        text = fh.read()

    algorithms = parse_index(text)
    result = {
        "source_pdf": args.pdf,
        "pages": pages,
        "text_dump": args.dump_text,
        "algorithms": [{"number": n, "name": s} for n, s in algorithms],
        "algorithm_count": len(algorithms),
        "slot_rules": parse_slot_rules(text),
        "published_ranges": parse_ranges(text),
    }
    with open(args.out, "w", encoding="utf-8") as fh:
        json.dump(result, fh, indent=1, ensure_ascii=False)
    print(
        "wrote %s | algorithms %d | first %s | last %s"
        % (
            args.out,
            len(algorithms),
            algorithms[0] if algorithms else None,
            algorithms[-1] if algorithms else None,
        )
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
