#!/usr/bin/env python3
"""Render the FAMILY-03 (Korg M1) and FAMILY-04 (Korg Wavestation) reference
documents from the enumerated data.

Inputs are the artifacts of this pass:
  - family 04: the Wavestation Voice Name List transcription (page images read
    with a vision model, cross-checked against per-column crops) and the effect
    program list read from two independent scans of the Reference Guide.
  - family 03: the M1 Voice Name List transcription and the effect type list
    read from the M1 Owner's Manual.

These files are INTERNAL REFERENCE: the only place the reference machines' own
names may appear. Shipped labels stay neutral (Family 01 ... Family 04).

Usage:
    python write_m1_ws_docs.py --family 04 --ws-tsv <tsv> --fx-json <json> --out banks/
    python write_m1_ws_docs.py --family 03 --m1-programs <tsv> --m1-combinations <tsv> \
        --fx-json <json> --out banks/
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys

RULES = """
## Hard rules for this file

- **Internal reference only.** The reference machine's own names may appear in
  this file and in `banks/SOURCES.md`, and nowhere else: not in the UI, preset
  names, file names or plugin metadata.
- Every row comes from a public document listed under *Source documents*; the
  source column says which document and which page or column it was read from.
- Nothing here is a ROM asset. These are name lists and structure read from
  published documentation; no sample data and no factory program data.
- Where the document does not state something, the cell says `not documented`.
- **Shipped labels are positions, not document numbers.** The first row of a
  section is `<family>-P0001`, `<family>-C0001`, `<family>-W0001` and so on; the
  `Number` / `Bank` columns carry the document's own identifiers, which is what
  a later conversion unit should match on.
"""

WS_DOCS = [
    (
        "D1",
        "WAVESTATION Voice Name List (Korg, file WSVoiceNList.pdf, 6 pages, "
        "scanned - no text layer)",
        "official Korg download page, product WAVESTATION, document id 1564",
        "https://www.korg.com/us/support/download/manual/1/224/1564/",
        "waves, wave sequences, patches (programs) and performances",
    ),
    (
        "D2",
        "WAVESTATION Reference Guide (Korg, file WSReferenceGuide_E.pdf, 128 "
        "pages, scanned - no text layer) and the archive.org scan of the "
        "Wavestation A/D Reference Guide",
        "official Korg download page, product WAVESTATION, document id 1563",
        "https://www.korg.com/us/support/download/manual/1/224/1563/",
        "the MDE effect program list, the effect routing (FX1/FX2, serial or "
        "parallel, buses A-D) and the wave/wave-sequence numbering rules",
    ),
]

M1_DOCS = [
    (
        "D1",
        "M1 Voice Name List (Korg, 4-page factory name list; scan transcribed "
        "from page images)",
        "archive.org item korg-m-1-voice.name.list (scanned copy of the Korg "
        "factory name list)",
        "https://archive.org/details/korg-m-1-voice.name.list",
        "100 factory programs and 100 factory combinations",
    ),
    (
        "D2",
        "M1 Owner's Manual (Korg, archive.org scan with OCR text layer)",
        "archive.org item synthmanual_M1_owners_manual",
        "https://archive.org/details/synthmanual_M1_owners_manual",
        "effect types 1-33 with their names, effect routing (2 effect systems, "
        "serial or parallel, 4 inputs A-D, 4 outputs 1/L 2/R 3 4), instrument "
        "and drum-kit parameters, specifications",
    ),
]


def read_tsv(path):
    rows = []
    with open(path, encoding="utf-8") as fh:
        fh.readline()
        for ln in fh:
            ln = ln.rstrip("\n")
            if not ln:
                continue
            rows.append([c.strip() for c in re.split(r"\s*\|\s*", ln)])
    return rows


def marker_note(name):
    if any(ch in name for ch in "![]{}*<>?"):
        return "name printed with marker characters; recorded exactly as printed"
    return ""


def table(lines, title, intro, columns, rows):
    lines.append(f"## {title}")
    lines.append("")
    if intro:
        lines.append(intro)
        lines.append("")
    lines.append("| " + " | ".join(columns) + " |")
    lines.append("|" + "---|" * len(columns))
    for r in rows:
        lines.append("| " + " | ".join(str(c) for c in r) + " |")
    lines.append("")


def write_ws(args):
    fam = "04"
    prefix = "F04"
    rows = read_tsv(args.ws_tsv)
    sections = {}
    for sec, ident, name in rows:
        sections.setdefault(sec, []).append((ident, name))
    patches = sections.get("PATCHES", [])
    perfs = sections.get("PERFORMANCES", [])
    seqs = sections.get("WAVE SEQUENCES", [])
    waves = sections.get("WAVES", [])
    fx = json.load(open(args.fx_json, encoding="utf-8"))

    lines = []
    add = lines.append
    add(f"# FAMILY {fam} - factory patches, performances and waves (internal reference)")
    add("")
    add("Reference machine (internal name, never shipped): **Korg Wavestation**.")
    add(f"Shipped label: **Family {fam}**.")
    add("")
    add(
        "Every row below was read from scanned pages of the official Voice Name "
        "List: the pages have no text layer, so they were transcribed from the "
        "page images by a vision pass and independently re-read from per-column "
        "crops of the same pages. Rows where the two passes disagreed are listed "
        "with a note. Machine-readable copies of the transcription live outside "
        "the repository (`scratch/u2/ocr_ws_lists.tsv`); the page scans are not "
        "copied into the repository."
    )
    add(RULES)
    add("")
    add("## Source documents")
    add("")
    add("| Ref | Document | Where it came from | URL | What it supplies |")
    add("|---|---|---|---|---|")
    for ref, title, where, url, supplies in WS_DOCS:
        add(f"| {ref} | {title} | {where} | {url} | {supplies} |")
    add("")
    add("## Counts as documented")
    add("")
    add("| Section | Count | Documented banks |")
    add("|---|---|---|")
    banks = lambda xs: ", ".join(sorted({x[0].split(" ")[0] for x in xs})) or "-"
    add(f"| Patches (the machine's programs) | {len(patches)} | {banks(patches)} |")
    add(f"| Performances (the machine's combis) | {len(perfs)} | {banks(perfs)} |")
    add(f"| Wave sequences | {len(seqs)} | {banks(seqs)} |")
    add(f"| Waves (ROM wave list) | {len(waves)} | one continuous index |")
    add("")
    add(
        f"- Patches: **{len(patches)}**; performances: **{len(perfs)}**; wave "
        f"sequences: **{len(seqs)}**; waves: **{len(waves)}**."
    )
    add("")

    table(
        lines,
        "Patches",
        "The document prints three patch banks side by side (ROM, RAM1, RAM2), "
        "each numbered from 0 and each 35 rows long. The bank is a column heading "
        "in the document, not repeated on the row.",
        ["Shipped label", "Documented name", "Bank", "Number", "Source"],
        [
            [f"{prefix}-P{i:04d}", name, ident.split(" ")[0], ident.split(" ")[1], "D1 p5"]
            for i, (ident, name) in enumerate(patches, start=1)
        ],
    )
    table(
        lines,
        "Performances",
        "Three banks (ROM, RAM1, RAM2), 50 rows each, numbered from 0.",
        ["Shipped label", "Documented name", "Bank", "Number", "Source"],
        [
            [f"{prefix}-F{i:04d}", name, ident.split(" ")[0], ident.split(" ")[1], "D1 p6"]
            for i, (ident, name) in enumerate(perfs, start=1)
        ],
    )
    table(
        lines,
        "Wave sequences",
        "Two banks (ROM, RAM1), 32 rows each, numbered from 0. The document "
        "repeats the first 32 ROM wave-sequence names as the first 32 entries of "
        "its wave list.",
        ["Shipped label", "Documented name", "Bank", "Number", "Source"],
        [
            [f"{prefix}-S{i:04d}", name, ident.split(" ")[0], ident.split(" ")[1], "D1 p4"]
            for i, (ident, name) in enumerate(seqs, start=1)
        ],
    )
    table(
        lines,
        "Waves (ROM wave list)",
        "One continuous index printed across five columns per page. Per D2, waves "
        "0-31 in each bank are wave sequences and waves 32 and up are the ROM "
        "waves (multisamples, attack transients, single-cycle loops); the last "
        "two waves are the analog inputs. This is engine input, not content to "
        "ingest.",
        ["Shipped label", "Documented index", "Documented name", "Source", "Note"],
        [
            [f"{prefix}-W{i:04d}", ident, name, "D1 p2" if int(ident) < 200 else "D1 p3",
             marker_note(name)]
            for i, (ident, name) in enumerate(waves, start=1)
        ],
    )

    add("## Factory FX algorithms and routing")
    add("")
    add(
        "**Routing: documented (D2).** The instrument has an MDE (Multi Digital "
        "Effects) processor with two effect slots, FX1 and FX2, which can be "
        "routed **serial or parallel**; four buses (A-D) feed the effects and "
        "each wave is routed to an FX bus. The guide states the effect programs "
        "are 55 programs, variations on approximately 26 basic effect types, and "
        "that each program is a macro for one of those types."
    )
    add("")
    add(
        "**Per-patch FX chains: not documented.** No published document states "
        "which effect program each factory patch or performance uses. That only "
        "exists in the machine's own factory data, which is out of bounds here, "
        "so per-patch chains are reconstructed by ear."
    )
    add("")
    add(
        "**Effect program list: partially recovered.** The list is printed only "
        "in the Reference Guide's effects chapter, and both available copies are "
        "scans whose text layer is machine-OCR of uneven quality (D2). The table "
        "below is what two independent scans agree on; entries marked `?` could "
        "not be read from either scan and must be checked against a clean copy "
        "of the manual. The guide presents this list as the Wavestation A/D's "
        "effect programs, so confirm it against the original Wavestation manual "
        "before using it as Family 04's chain vocabulary."
    )
    add("")
    add("| # | Effect program as printed | Note |")
    add("|---|---|---|")
    for e in fx["entries"]:
        add(f"| {e['number']} | {e['name']} | {e['note']} |")
    add("")

    path = os.path.join(args.out, f"FAMILY-{fam}-PROGRAMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    print("wrote", path, "lines", len(lines) + 1)

    # ---------------- drums file ----------------
    d = []
    d.append(f"# FAMILY {fam} - drum kits, drum sounds and pad mapping (internal reference)")
    d.append("")
    d.append("Reference machine (internal name, never shipped): **Korg Wavestation**.")
    d.append(f"Shipped label: **Family {fam}**.")
    d.append("")
    d.append(RULES)
    d.append("")
    d.append("## Source documents")
    d.append("")
    d.append("| Ref | Document | Where it came from | URL |")
    d.append("|---|---|---|---|")
    for ref, title, where, url, _ in WS_DOCS:
        d.append(f"| {ref} | {title} | {where} | {url} |")
    d.append("")
    d.append("## Drum kits and drum sounds")
    d.append("")
    d.append(
        "- **Drum kits: not documented.** D1 has four sections - waves, wave "
        "sequences, patches and performances - and no drum-kit section. The "
        "machine has no drum-kit concept: percussion is built from waves and "
        "wave sequences inside ordinary patches."
    )
    d.append(
        "- **Drum sounds: not documented as a list.** D1 does not label any wave "
        "as a drum sound, and D2 does not publish one either. The drum-type "
        "waves therefore exist only as entries in the wave list in "
        f"`FAMILY-{fam}-PROGRAMS.md`, and which of them are percussion is not "
        "stated by any document we found. We do not classify them ourselves."
    )
    d.append(
        "- **Pad mapping: not documented.** With no drum-kit concept there is no "
        "key-to-sound map to record."
    )
    d.append("")
    d.append("## Where we looked for drum documentation")
    d.append("")
    d.append("| Document | What it contains for drums |")
    d.append("|---|---|")
    d.append(
        "| D1 Voice Name List (all 6 pages) | no drum section; four lists only |"
    )
    d.append(
        "| D2 Reference Guide (effects, wave and wave-sequence sections) | "
        "wave/wave-sequence structure only, no drum-sound list |"
    )
    d.append(
        "| D2 Player's Guide / Owner's Manual (archive.org copies) | searched for "
        "'drum', 'kit', 'percussion'; the guide describes waveform selection and "
        "wave sequencing, with no drum-sound list |"
    )
    d.append("")
    path = os.path.join(args.out, f"FAMILY-{fam}-DRUMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(d) + "\n")
    print("wrote", path, "lines", len(d) + 1)
    return 0


def write_m1(args):
    fam = "03"
    prefix = "F03"
    progs = read_tsv(args.m1_programs)
    combis = read_tsv(args.m1_combinations)
    fx = json.load(open(args.fx_json, encoding="utf-8"))

    lines = []
    add = lines.append
    add(f"# FAMILY {fam} - factory programs and combinations (internal reference)")
    add("")
    add("Reference machine (internal name, never shipped): **Korg M1**.")
    add(f"Shipped label: **Family {fam}**.")
    add("")
    add(
        "Rows were read from page images of the factory Voice Name List (the "
        "scans have no text layer) and cross-checked against an independent "
        "machine OCR of the same pages. The PDFs and page images are not copied "
        "into the repository."
    )
    add(RULES)
    add("")
    add("## Source documents")
    add("")
    add("| Ref | Document | Where it came from | URL | What it supplies |")
    add("|---|---|---|---|---|")
    for ref, title, where, url, supplies in M1_DOCS:
        add(f"| {ref} | {title} | {where} | {url} | {supplies} |")
    add("")
    add("## Counts as documented")
    add("")
    add("| Section | Count |")
    add("|---|---|")
    add(f"| Factory programs | {len(progs)} |")
    add(f"| Factory combinations | {len(combis)} |")
    add("")
    add(
        f"- Programs: **{len(progs)}**; combinations: **{len(combis)}**. D2's "
        "specification page states 100 programs and 100 combinations, which "
        "matches the name list."
    )
    add("")
    table(
        lines,
        "Programs",
        "Numbering is the document's own 00-99 grid.",
        ["Shipped label", "Documented name", "Number", "Source", "Note"],
        [
            [f"{prefix}-P{i:04d}", r[1], r[0], "D1 p1", marker_note(r[1])]
            for i, r in enumerate(progs, start=1)
        ],
    )
    table(
        lines,
        "Combinations",
        "Numbering is the document's own 00-99 grid. The document also prints the "
        "programs each combination is built from; that timbre list is in the "
        "transcription file outside the repository and is not repeated here.",
        ["Shipped label", "Documented name", "Number", "Source", "Note"],
        [
            [f"{prefix}-C{i:04d}", r[1], r[0], "D1 p1-p3", marker_note(r[1])]
            for i, r in enumerate(combis, start=1)
        ],
    )

    add("## Disputed spellings in the scan")
    add("")
    add(
        "The Voice Name List is a scan with no text layer, so two spellings were "
        "read two different ways. Both are recorded here with the evidence, and "
        "the family file keeps the better-supported reading."
    )
    add("")
    add("| Number | Reading kept | Other reading | Evidence |")
    add("|---|---|---|---|")
    add(
        "| 31 (program) | `Harpsicord` | `Harpsichord` | tesseract reads "
        "`Harpsicord` at all five occurrences, the full-page reads agree, and the "
        "printed word's width matches the 10-character column; two zoomed reads "
        "said `Harpsichord`, but the letters touch in the scan so they could not "
        "be split into glyphs to settle it. Kept as printed by the majority of "
        "readings. |"
    )
    add(
        "| 82 (combination) | `Atomosphere` | `Atmosphere` | splitting the printed "
        "word into its letter bitmaps shows a hollow oval then a three-stem "
        "glyph (A-t-o-m-o-s-p-h-e-r-e), and tesseract reads `Atomosphere`; the "
        "document appears to carry the typo. |"
    )
    add("")

    add("## Factory FX algorithms and routing")
    add("")
    add(
        "**Found in documentation: yes** (D2). The instrument has two effect "
        "systems plus two panpots, with four inputs (A, B, C, D) and four "
        "outputs (1/L, 2/R, 3, 4). The two effects can be placed in **series or "
        "in parallel**; all routing is digital inside the instrument. D2 states "
        "that effect types **1-25 are stereo effects and 26-33 are dual "
        "effects**, in which each channel carries a different effect - 33 types "
        "in total."
    )
    add("")
    add(
        "**Per-program FX assignments are not documented.** No public document "
        "states which effect type and parameter values each factory program "
        "uses; that is only in the instrument's factory data, which is out of "
        "bounds here."
    )
    add("")
    add("### Effect types as printed in D2")
    add("")
    add("| # | Effect type as printed |")
    add("|---|---|")
    for e in fx["entries"]:
        add(f"| {e['number']} | {e['name']} |")
    add("")

    path = os.path.join(args.out, f"FAMILY-{fam}-PROGRAMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    print("wrote", path, "lines", len(lines) + 1)

    d = []
    d.append(f"# FAMILY {fam} - drum kits, drum sounds and pad mapping (internal reference)")
    d.append("")
    d.append("Reference machine (internal name, never shipped): **Korg M1**.")
    d.append(f"Shipped label: **Family {fam}**.")
    d.append("")
    d.append(RULES)
    d.append("")
    d.append("## Source documents")
    d.append("")
    d.append("| Ref | Document | Where it came from | URL |")
    d.append("|---|---|---|---|")
    for ref, title, where, url, _ in M1_DOCS:
        d.append(f"| {ref} | {title} | {where} | {url} |")
    d.append("")
    d.append("## Drum kits, drum sounds and pad mapping")
    d.append("")
    d.append(
        "- **Drum kits: not documented as a list.** The machine has a drum-kit "
        "instrument setting (D2: 'Assigns drum sounds to the keys', one of four "
        "instrument settings). D2 shows **one example** key assignment to "
        "illustrate the parameter - `1 BASS DRUM 1, 2 SNARE 1, 3 MID TOM, "
        "4 CLOSED HI HAT, 5 OPEN HI HAT, 6 CRASH CYMBAL 1, 7 RIDE CYMBAL 1, "
        "8 HANDCLAPS, 9 HI CONGA (MUTED), 10 LO BONGO, 11 BASS DRUM 2, "
        "12 SNARE 2, 13 MID TOM, 14 LO TOM ...`. It is printed as an example, "
        "not as the factory mapping, so it is **not** recorded here as the "
        "factory key map."
    )
    d.append(
        "- **Drum sounds: not documented as a list.** Neither D1 nor D2 "
        "publishes a drum-sound list for this machine. D1 lists programs and "
        "combinations only."
    )
    d.append(
        "- **Pad mapping: not documented.** No published document gives the "
        "factory key-to-drum-sound map."
    )
    d.append("")
    d.append("## Where we looked for drum documentation")
    d.append("")
    d.append("| Document | What it contains for drums |")
    d.append("|---|---|")
    d.append(
        "| D1 Voice Name List (all 4 pages) | programs and combinations only; the "
        "program list includes drum-kit programs named `Drums #1` / `Drums #2` / "
        "`Drums #3`, but no drum-sound list |"
    )
    d.append(
        "| D2 Owner's Manual (OCR text, whole document) | the drum-kit parameter "
        "and its example assignment; no factory map, no drum-sound list |"
    )
    d.append(
        "| Korg's M1 download page (Owner's Manual, Super Guide) | searched for a "
        "drum-sound or multisound list; none published as text |"
    )
    d.append("")
    path = os.path.join(args.out, f"FAMILY-{fam}-DRUMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(d) + "\n")
    print("wrote", path, "lines", len(d) + 1)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--family", required=True, choices=["03", "04"])
    ap.add_argument("--ws-tsv")
    ap.add_argument("--m1-programs")
    ap.add_argument("--m1-combinations")
    ap.add_argument("--fx-json", required=True)
    ap.add_argument("--out", required=True)
    args = ap.parse_args(argv)
    if args.family == "04":
        return write_ws(args)
    return write_m1(args)


if __name__ == "__main__":
    sys.exit(main())
