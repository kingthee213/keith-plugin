#!/usr/bin/env python3
"""Extract the effect-algorithm catalogue and the effect slot rules from a Korg
TRITON-family Parameter Guide.

The Parameter Guide's effect chapter index lists every algorithm as
`NNN: Name`; this tool reads that index out of the document's own text layer.
Nothing is transcribed by hand and nothing is invented: if the index cannot be
read, the tool reports it instead of producing a list.

Usage:
    python korg_pg_fx.py <parameter-guide.pdf> <out.json> [--dump-text <txt>]
                         [--profile triton|tr]

Profiles:
    triton (default) - TRITON / TRITON Extreme: the index numbers algorithms
        `000`-`102` and the guide's text layer can be read as one stream.
    tr               - TR (2005): the index numbers algorithms `00`-`89` and sits
        in a two-column contents page, so each page is read column by column
        from the word geometry instead of from the merged text layer.
"""

from __future__ import annotations

import argparse
import json
import re
import sys

import pdfplumber

INDEX_START = "Filter and dynamics control effects"
RANGE_RE = re.compile(r"(\d{3})\s*:\s*No Effect\s*[-–]\s*(\d{3})\s*:\s*([^\.\(]+)")

TR_ENTRY_RE = re.compile(r"^(\d{2}):\s*(.*?)\.{2,}\s*(\d{1,3})\s*$")
TR_GLOSS_RE = re.compile(r"^\((.+)\)\s*\.{2,}\s*(\d{1,3})\s*$")
TR_BARE_RE = re.compile(r"^(\d{2}):\s*([A-Za-z].*[^\d\s\.])\s*$")
TR_RANGE_RE = re.compile(r"(Insert Effect|Master Effect 1, 2) \[(\d{2})\.\.\.(\d{2}): name\]")
TR_COUNT_RE = re.compile(r"select from (\d+) types of effect: 01:\s*([^–\-]+?)\s*[–\-]\s*(\d{2}):")


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


def tr_column_lines(pdf_path):
    """Read every page's text one column at a time.

    The TR guide prints its contents in two columns that share each physical
    line; the extracted text layer merges the two, which pairs an entry with the
    wrong neighbour. Splitting the words at the page midline first keeps each
    column's entries and their wrapped gloss lines together.
    """
    with pdfplumber.open(pdf_path) as pdf:
        for page in pdf.pages:
            words = page.extract_words()
            for side in (
                [w for w in words if w["x0"] < page.width / 2],
                [w for w in words if w["x0"] >= page.width / 2],
            ):
                side.sort(key=lambda w: (round(w["top"]), w["x0"]))
                line, top = [], None
                for w in side:
                    if top is None or abs(w["top"] - top) <= 3:
                        line.append(w["text"])
                        top = w["top"] if top is None else top
                    else:
                        yield " ".join(line)
                        line, top = [w["text"]], w["top"]
                if line:
                    yield " ".join(line)


def tr_index(lines):
    """Read `NN: Name` entries out of a column-wise line stream. Returns the
    contiguous run that starts at 00, in order."""
    entries, pending = {}, None
    for s in lines:
        m = TR_ENTRY_RE.match(s)
        if m:
            num, name, pg = m.group(1), m.group(2).strip(), int(m.group(3))
            if num not in entries:
                entries[num] = [name, pg]
            pending = num if name.count("(") != name.count(")") else None
            continue
        g = TR_GLOSS_RE.match(s)
        if g and pending is not None:
            entries[pending][0] = f"{entries[pending][0]} ({g.group(1).strip()})"
            entries[pending][1] = int(g.group(2))
            pending = None
            continue
        b = TR_BARE_RE.match(s)
        if b and b.group(1) not in entries:
            entries[b.group(1)] = [b.group(2).strip(), None]
            pending = b.group(1)
            continue
        if s and not s[0].isdigit():
            pending = None
    run, expect = [], 0
    while f"{expect:02d}" in entries:
        run.append((f"{expect:02d}", entries[f"{expect:02d}"][0]))
        expect += 1
    return run


def parse_tr_slot_rules(text):
    rules = {}
    for m in TR_RANGE_RE.finditer(text):
        key = "ifx" if m.group(1).startswith("Insert") else "mfx"
        rules[f"{key}_range"] = f"{m.group(2)}..{m.group(3)}"
    m = TR_COUNT_RE.search(text)
    if m:
        rules["stated_type_count"] = int(m.group(1))
        rules["first_type"] = m.group(2).strip()
        rules["last_type_number"] = m.group(3)
    if re.search(r"The TR provides one insert effect \(IFX\)", text):
        rules["insert_effects"] = 1
    if re.search(r"The I/Os of Master Effects MFX1 and MFX2 are mono-in/\s*stereo-out", text):
        rules["master_effects"] = 2
        rules["master_effect_io"] = "mono in / stereo out"
    return rules


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
    ap.add_argument(
        "--profile",
        choices=("triton", "tr"),
        default="triton",
        help="page layout of the guide being read (default: triton)",
    )
    args = ap.parse_args(argv)

    with pdfplumber.open(args.pdf) as pdf:
        pages = len(pdf.pages)
    if args.dump_text:
        print("pages dumped:", dump_text(args.pdf, args.dump_text))

    if args.profile == "tr":
        text = "\n".join(tr_column_lines(args.pdf))
        algorithms = tr_index(text.split("\n"))
        slot_rules = parse_tr_slot_rules(text)
        published_ranges = []
        for m in TR_RANGE_RE.finditer(text):
            row = {"from": m.group(2), "to": m.group(3), "name": m.group(1)}
            if row not in published_ranges:
                published_ranges.append(row)
    else:
        if not args.dump_text:
            raise SystemExit("--dump-text <txt> is required for this profile")
        with open(args.dump_text, encoding="utf-8") as fh:
            text = fh.read()
        algorithms = parse_index(text)
        slot_rules = parse_slot_rules(text)
        published_ranges = parse_ranges(text)

    result = {
        "source_pdf": args.pdf,
        "pages": pages,
        "text_dump": args.dump_text,
        "profile": args.profile,
        "algorithms": [{"number": n, "name": s} for n, s in algorithms],
        "algorithm_count": len(algorithms),
        "slot_rules": slot_rules,
        "published_ranges": published_ranges,
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
