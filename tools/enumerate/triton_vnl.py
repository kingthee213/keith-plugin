#!/usr/bin/env python3
"""Parse a Korg TRITON-family Voice Name List PDF into structured JSON.

Keith content lane (U2) - our own enumeration tool. It reads a publicly
published Korg Voice Name List PDF (supplied by the caller) and emits the
factory lists that document contains: combinations, programs, drum kits with
the documented per-key drumsample mapping, multisamples and drumsamples.

No Korg content is embedded in this file; it only reads a document the caller
supplies. The raw PDF is never copied into the repository.

Usage:
    python triton_vnl.py <voice-name-list.pdf> <out.json> [--profile triton|extreme]

Requires: pdfplumber  (pip install pdfplumber)
"""

from __future__ import annotations

import argparse
import json
import re
import sys

import pdfplumber

NUM_RE = re.compile(r"^([A-N]\d{3}|g\(\w+\)\d{3})([sd]?)$")
NOTE_RE = re.compile(r"^[A-G]#?\d$")
HEAD_RE = re.compile(r"^#+$")
BANK_RE = re.compile(r"^[A-Z]$")

# section heading -> kind; matched on the leading words of a page row
SECTIONS = [
    ("Combinations", "combination"),
    ("Programs", "program"),
    ("Drum kits", "kit"),
    ("GM Drum kits", "gm_kit"),
    ("Multisamples", "multisample"),
    ("Drumsamples", "drumsample"),
]

# headings that end a list section but carry no factory list of their own
IGNORED_HEADINGS = (
    "Preset Arpeggio Patterns",
    "User Arpeggio Patterns",
    "Preset Patterns",
    "Template Songs",
    "Demo Songs",
    "If the EXB-MOSS",
    "Table of contents",
    "Arpeggio Patterns",
)


def page_rows(page, tol=4.0):
    """Group a page's words into visual rows (top -> bottom, x -> right)."""
    words = sorted(page.extract_words(), key=lambda w: (w["top"], w["x0"]))
    rows = []
    for w in words:
        if rows and abs(w["top"] - rows[-1]["top"]) <= tol:
            rows[-1]["words"].append(w)
            rows[-1]["top"] = min(rows[-1]["top"], w["top"])
        else:
            rows.append({"top": w["top"], "words": [w]})
    for r in rows:
        r["words"].sort(key=lambda w: w["x0"])
    return rows


def bounds_from(starts):
    """Column spans. Each block owns every token from its start up to the next
    block's start, so right-aligned cells (category codes, bus columns) stay
    with the block they belong to."""
    out = []
    for i, s in enumerate(starts):
        lo = s - 6 if i == 0 else s - 6
        hi = starts[i + 1] - 6 if i + 1 < len(starts) else 10_000.0
        out.append((lo, hi))
    return out


def align_starts(rows, starts):
    """Header markers sit a few points right of the data they label. Find the
    data column x for the first block and shift every start by the same delta."""
    first = None
    for row in rows:
        for w in row["words"]:
            if NUM_RE.match(w["text"]) or re.fullmatch(r"\d{3}", w["text"]):
                first = w["x0"]
                break
        if first is not None:
            break
    if first is None:
        return starts
    delta = first - starts[0]
    if not (-25 <= delta <= 5):
        return starts
    return [round(s + delta, 1) for s in starts]


def row_text(row):
    return " ".join(w["text"] for w in row["words"])


def section_of(row):
    """Return the section kind if this row is a section heading, else None.

    Returns the string "none" for a heading that ends a list without starting
    another one."""
    ws = row["words"]
    if not ws or len(ws) > 8:
        return None
    text = row_text(row)
    for label in IGNORED_HEADINGS + tuple(l for l, _ in SECTIONS):
        if not text.startswith(label):
            continue
        if ".." in text[len(label):]:
            return None  # table-of-contents line
        for name, kind in SECTIONS:
            if text.startswith(name):
                return kind
        return "none"
    return None


def cluster(xs, tol=10.0):
    out = []
    for x in sorted(xs):
        if out and x - out[-1][-1] <= tol:
            out[-1].append(x)
        else:
            out.append([x])
    return [sum(g) / len(g) for g in out]


def column_starts(rows, kind):
    """Column starts for a block.

    List sections (combinations / programs / multisamples / drumsamples) are
    keyed off the `#` markers in the heading row; drum-kit sections are keyed
    off the kit-number and column markers in their heading row (kit numbers are
    sometimes printed run together with the kit name)."""
    if kind in ("kit", "gm_kit"):
        marker = "High" if kind == "kit" else "Drumsample"
        num_xs, mark_xs, low_xs = [], [], []
        for row in rows[:2]:
            for w in row["words"]:
                if re.fullmatch(r"\d{1,3}:?", w["text"]):
                    num_xs.append(w["x0"])
                elif w["text"] == marker:
                    mark_xs.append(w["x0"])
                elif w["text"] == "Low":
                    low_xs.append(w["x0"])
        if mark_xs and low_xs:
            # the Low column sits a fixed distance right of each High column,
            # so it recovers a column whose marker is damaged in the text layer
            offset = min(low_xs) - min(mark_xs)
            mark_xs = mark_xs + [x - offset for x in low_xs]
        starts = cluster(mark_xs) if mark_xs else cluster(num_xs)
        if num_xs and starts:
            delta = num_xs[0] - starts[0]
            if -25 <= delta <= 5:
                starts = [s + delta for s in starts]
        return starts
    for row in rows:
        hits = [w for w in row["words"] if HEAD_RE.match(w["text"])]
        if hits and any(w["text"] == "Name" for w in row["words"]):
            starts = sorted({round(w["x0"], 1) for w in hits})
            return align_starts(rows, starts)
    return []


def sub_columns(rows, bounds):
    """Per column block: x of the Name / Category / SW sub-headings."""
    sub = {round(lo, 1): {} for lo, _ in bounds}
    for row in rows:
        for w in row["words"]:
            for lo, hi in bounds:
                if not (lo <= w["x0"] < hi):
                    continue
                key = round(lo, 1)
                t = w["text"]
                if t == "Name" and "name" not in sub[key]:
                    sub[key]["name"] = w["x0"]
                elif t == "Category" and "category" not in sub[key]:
                    sub[key]["category"] = w["x0"]
                elif t in ("SW", "Arp.SW") and "arp" not in sub[key]:
                    sub[key]["arp"] = w["x0"]
    return sub


def block_index(x, starts):
    """Index of the column block that owns x (largest start <= x)."""
    idx = 0
    for i, s in enumerate(starts):
        if x >= s - 6:
            idx = i
    return idx


def banks_by_block(rows, starts):
    """Per-block bank letters from `Bank X` labels; -1 where none is printed."""
    out = {}
    for row in rows:
        ws = row["words"]
        for i, w in enumerate(ws):
            if w["text"] == "Bank" and i + 1 < len(ws) and BANK_RE.match(ws[i + 1]["text"]):
                out[block_index(w["x0"], starts)] = ws[i + 1]["text"]
    return out


def groups_by_block(rows, starts, header_top):
    """Sample-list group labels (ROM, Piano, New1, ...) printed above the
    column headers, mapped to the block they sit over."""
    out = {}
    label_row = None
    for row in rows:
        if row["top"] >= header_top - 1:
            continue
        if any(re.search(r"[A-Za-z]", w["text"]) for w in row["words"]):
            label_row = row
    if label_row is not None:
        for w in label_row["words"]:
            if re.search(r"[A-Za-z]", w["text"]):
                out[block_index(w["x0"], starts)] = w["text"]
    return out


def split_cell(row, lo, hi):
    return [w for w in row["words"] if lo <= w["x0"] < hi]


def col_of(x, cat_x, arp_x, hi, tol=2.0):
    """Which sub-column a token belongs to: name, category or arp."""
    if arp_x is not None and x >= arp_x - tol:
        return "arp"
    if cat_x is not None and x >= cat_x - tol:
        return "category"
    return "name"


def parse_combination(rows, starts, sub):
    bounds = bounds_from(starts)
    out = []
    pending = {}
    for row in rows:
        if section_of(row):
            continue
        for bi, (lo, hi) in enumerate(bounds):
            cell = split_cell(row, lo, hi)
            if not cell:
                continue
            key = round(lo, 1)
            head = cell[0]
            if re.fullmatch(r"[A-Z]\d{3}", head["text"]):
                rest = cell[1:]
                cat_x = sub[key].get("category")
                arp_x = sub[key].get("arp")
                # Classic layout: name and category share one x on two rows, so
                # the category column only bounds the name when there is no arp
                # column to the right of it (TRITON Extreme).
                eff_cat_x = None if arp_x is not None else cat_x
                cat = ""
                if arp_x is None and cat_x is not None and rest and re.fullmatch(
                    r"\d{2}", rest[-1]["text"]
                ):
                    cat = rest[-1]["text"]
                    rest = rest[:-1]
                name = " ".join(
                    w["text"]
                    for w in rest
                    if col_of(w["x0"], eff_cat_x, arp_x, hi) == "name"
                ).strip()
                entry = {"number": head["text"], "name": name, "category": cat, "col": bi}
                out.append(entry)
                pending[key] = entry
            elif head["x0"] > lo + 18 and key in pending:
                # classic layout: category text on the continuation row
                cat_x = sub[key].get("category")
                arp_x = sub[key].get("arp")
                cat = " ".join(
                    w["text"]
                    for w in cell
                    if col_of(w["x0"], cat_x, arp_x, hi) == "category"
                ).strip()
                if cat and not pending[key]["category"]:
                    pending[key]["category"] = cat
    return out


def parse_program(rows, starts, sub, allow_trailing_code=False):
    bounds = bounds_from(starts)
    out = []
    for row in rows:
        if section_of(row):
            continue
        for bi, (lo, hi) in enumerate(bounds):
            cell = split_cell(row, lo, hi)
            if not cell:
                continue
            key = round(lo, 1)
            m = NUM_RE.match(cell[0]["text"])
            if not m:
                continue
            number, osc = m.group(1), m.group(2)
            rest = cell[1:]
            cat_x = sub[key].get("category")
            arp_x = sub[key].get("arp")
            # a separate OSC/drum flag can follow the number
            if not osc and rest and re.fullmatch(r"[sd]", rest[0]["text"]):
                osc = rest[0]["text"]
                rest = rest[1:]
            cat = ""
            use_code = allow_trailing_code or (arp_x is None and cat_x is not None)
            if use_code and rest and re.fullmatch(r"\d{2}", rest[-1]["text"]):
                # TRITON Extreme prints a 2-digit category code
                cat = rest[-1]["text"]
                rest = rest[:-1]
            else:
                cat_tokens = [
                    w for w in rest
                    if col_of(w["x0"], cat_x, arp_x, hi) == "category"
                ]
                if cat_tokens:
                    cat = " ".join(w["text"] for w in cat_tokens).strip()
                    rest = [w for w in rest if w not in cat_tokens]
            name = " ".join(
                w["text"] for w in rest if col_of(w["x0"], cat_x, arp_x, hi) == "name"
            ).strip()
            out.append(
                {
                    "number": number,
                    "osc": osc,
                    "name": name,
                    "category": cat,
                    "col": bi,
                }
            )
    return out


def parse_kit(rows, starts):
    bounds = bounds_from(starts)
    headers = {}
    for row in rows[:3]:
        for w in row["words"]:
            inline = ""
            m = re.match(r"^(\d{1,3}):(.*)$", w["text"])
            if m:
                bi = min(range(len(starts)), key=lambda i: abs(starts[i] - w["x0"]))
                inline = m.group(2)
            elif re.fullmatch(r"\d{1,3}", w["text"]):
                bi = min(range(len(starts)), key=lambda i: abs(starts[i] - w["x0"]))
                if abs(w["x0"] - starts[bi]) > 8:
                    continue  # a name fragment that happens to be a number
                m = re.match(r"^(\d{1,3})", w["text"])
            else:
                continue
            if bi in headers:
                continue
            lo, hi = bounds[bi]
            parts = [inline] if inline else []
            parts += [
                t["text"]
                for t in row["words"]
                if t["x0"] > w["x1"] and lo - 40 <= t["x0"] < hi
            ]
            name = " ".join(parts).lstrip(": ").strip()
            headers[bi] = {"number": m.group(1), "name": name}
    keys = []
    for row in rows:
        first = row["words"][0]
        if first["x0"] < bounds[0][0] and NOTE_RE.match(first["text"]):
            keys.append((row["top"], first["text"]))
    if keys:
        lo_top, hi_top = keys[0][0] - 2, keys[-1][0] + 6
        # rows in the key band that carry data but no printed key label
        for row in rows:
            if not (lo_top <= row["top"] <= hi_top):
                continue
            if any(NOTE_RE.match(w["text"]) and w["x0"] < bounds[0][0] for w in row["words"]):
                continue
            if any(lo <= w["x0"] < hi for w in row["words"] for lo, hi in bounds):
                keys.append((row["top"], ""))
        keys.sort()
    kits = []
    for bi in sorted(headers):
        lo, hi = bounds[bi]
        mapping = []
        for top, note in keys:
            cell = [
                w
                for row in rows
                if abs(row["top"] - top) <= 2.5
                for w in row["words"]
                if lo <= w["x0"] < hi
            ]
            cell.sort(key=lambda w: w["x0"])
            mapping.append({"note": note, "text": " ".join(w["text"] for w in cell)})
        kit = dict(headers[bi])
        kit["col"] = bi
        kit["keys"] = mapping
        kits.append(kit)
    return kits


def parse_numbered(rows, starts):
    bounds = bounds_from(starts)
    out = []
    for row in rows:
        if section_of(row):
            continue
        for bi, (lo, hi) in enumerate(bounds):
            cell = split_cell(row, lo, hi)
            if not cell:
                continue
            if not re.fullmatch(r"\d{3}", cell[0]["text"]):
                continue
            name = " ".join(w["text"] for w in cell[1:]).strip()
            out.append({"number": int(cell[0]["text"]), "name": name, "col": bi})
    return out


def parse_pdf(path, verbose=True):
    data = {
        "source_pdf": path,
        "combinations": [],
        "programs": [],
        "kits": [],
        "gm_kits": [],
        "multisamples": [],
        "drumsamples": [],
        "page_map": [],
    }
    with pdfplumber.open(path) as pdf:
        section = None
        bank_by_idx = {}      # column-block index -> bank letter (carried on)
        group_by_idx = {}     # column-block index -> sample-list group label
        last_bank = None
        last_group = None
        for pno, page in enumerate(pdf.pages, start=1):
            rows = page_rows(page)
            cuts = [i for i, r in enumerate(rows) if section_of(r)]
            segments = []
            start = 0
            for i in cuts:
                if i > start:
                    segments.append((start, i, section))
                new = section_of(rows[i])
                section = None if new == "none" else new
                start = i + 1
            if start < len(rows):
                segments.append((start, len(rows), section))

            page_info = {"page": pno, "blocks": []}
            for s, e, kind in segments:
                block = rows[s:e]
                if kind is None or not block:
                    continue
                starts = column_starts(block, kind)
                if not starts:
                    if verbose:
                        print(f"  p{pno} {kind}: no columns found")
                    continue
                bounds = bounds_from(starts)
                sub = sub_columns(block, bounds) if kind in ("combination", "program") else {}
                banks = {}
                groups = {}
                if kind in ("combination", "program", "multisample", "drumsample"):
                    banks = banks_by_block(block, starts)
                    for i in range(len(starts)):
                        if i in banks:
                            last_bank = banks[i]
                        bank_by_idx[i] = last_bank
                if kind in ("multisample", "drumsample"):
                    head_row = next(
                        (r["top"] for r in block if any(HEAD_RE.match(w["text"]) for w in r["words"])),
                        0.0,
                    )
                    groups = groups_by_block(block, starts, head_row)
                    for i in range(len(starts)):
                        if i in groups:
                            last_group = groups[i]
                        group_by_idx[i] = last_group
                page_info["blocks"].append(
                    {
                        "section": kind,
                        "banks": {str(k): v for k, v in banks.items()},
                        "groups": {str(k): v for k, v in groups.items()},
                        "rows": len(block),
                        "cols": len(starts),
                    }
                )

                if kind == "combination":
                    for x in parse_combination(block, starts, sub):
                        bi = x.pop("col")
                        data["combinations"].append(
                            {**x, "bank": bank_by_idx.get(bi), "page": pno}
                        )
                elif kind == "program":
                    for x in parse_program(block, starts, sub):
                        bi = x.pop("col")
                        data["programs"].append(
                            {**x, "bank": bank_by_idx.get(bi), "page": pno}
                        )
                elif kind in ("kit", "gm_kit"):
                    for k in parse_kit(block, starts):
                        data["kits" if kind == "kit" else "gm_kits"].append(
                            {**k, "page": pno}
                        )
                elif kind in ("multisample", "drumsample"):
                    key = "multisamples" if kind == "multisample" else "drumsamples"
                    # a program column can continue onto a page whose heading is
                    # another section (TRITON Extreme bank N); its cells start
                    # with a program number, so they are separable from samples
                    for x in parse_program(
                        block,
                        starts,
                        {round(lo, 1): {} for lo, _ in bounds_from(starts)},
                        allow_trailing_code=True,
                    ):
                        bi = x.pop("col")
                        data["programs"].append(
                            {**x, "bank": bank_by_idx.get(bi), "page": pno}
                        )
                    for x in parse_numbered(block, starts):
                        bi = x.pop("col")
                        data[key].append(
                            {**x, "group": group_by_idx.get(bi), "page": pno}
                        )
            data["page_map"].append(page_info)
            if verbose:
                print(f"p{pno:>3} blocks={page_info['blocks']}")
    return data


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("pdf")
    ap.add_argument("out")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args(argv)
    data = parse_pdf(args.pdf, verbose=not args.quiet)
    with open(args.out, "w", encoding="utf-8") as fh:
        json.dump(data, fh, indent=1, ensure_ascii=False)
    print(
        "wrote %s | combis %d | programs %d | kits %d | multisamples %d | drumsamples %d"
        % (
            args.out,
            len(data["combinations"]),
            len(data["programs"]),
            len(data["kits"]),
            len(data["multisamples"]),
            len(data["drumsamples"]),
        )
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
