#!/usr/bin/env python3
"""Turn a parsed Triton-family Voice Name List JSON into the Keith content-lane
reference documents: banks/FAMILY-0x-PROGRAMS.md and banks/FAMILY-0x-DRUMS.md.

These files are INTERNAL REFERENCE. They are the only place in the repository
where the reference machine's own names appear; shipped labels stay neutral
(Family 01 ... Family 04).

Usage:
    python write_family_docs.py --family 01 --machine Triton \
        --json scratch/triton_classic.json --out banks/
"""

from __future__ import annotations

import argparse
import json
import os
import re

# Per family: the documents the rows were read from.
DOCS = {
    "01": [
        (
            "D1",
            "TRITON Voice Name List (Korg, file TRITON_VNL_EFG2.pdf, 409.5 KB)",
            "official Korg download page, product TRITON, document id 1652",
            "https://www.korg.com/us/support/download/manual/1/214/1652/",
            "combinations (banks A-D), programs (banks A-D and GM bank G with its "
            "g(1)-g(9)/g(d) variant rows), drum kits with the per-key drumsample "
            "mapping, GM drum kits, multisamples, drumsamples",
        ),
        (
            "D2",
            "TRITON Parameter Guide (Korg, file TRITON_PG_E.pdf, 255 pages, "
            "created 1999-12-16)",
            "official Korg download page, product TRITON, document id 1648",
            "https://www.korg.com/us/support/download/manual/1/214/1648/",
            "effect algorithm catalogue (000-102) and the insert/master effect "
            "routing architecture",
        ),
    ],
    "02": [
        (
            "D1",
            "TRITON Extreme Voice Name List (Korg, file TRITON Extreme Voice Name "
            "List, 40 pages)",
            "official Korg download page, product TRITON Extreme",
            "https://www.korg.com/us/support/download/product/1/215/",
            "combinations (banks A-D, H-M), programs (banks A-D, G, H-N), drum "
            "kits with the per-key drumsample mapping, GM drum kits, multisamples, "
            "drumsamples",
        ),
        (
            "D2",
            "TRITON Extreme Parameter Guide (Korg, file TNEX_ParameterGuide_E2, "
            "354 pages, created 2004-02-23)",
            "official Korg download page, product TRITON Extreme, document id 2423",
            "https://www.korg.com/us/support/download/manual/1/215/2423/",
            "effect algorithm catalogue (000-102) and the insert/master effect "
            "routing architecture",
        ),
    ],
    "05": [
        (
            "D1",
            "TR Voice Name List (Korg, file TR_VoiceNameList_EFG.pdf, 34 pages, "
            "created 2005-08-26)",
            "official Korg download page, product TR, document id 1646",
            "https://www.korg.com/us/support/download/manual/1/211/1646/",
            "combinations (banks A-C), programs (banks A-D, bank G with its "
            "g(d) variant rows), drum kits with the per-key drumsample mapping, "
            "GM drum kits, multisamples (Rom, EX), drumsamples (Rom, EX)",
        ),
        (
            "D2",
            "TR Parameter Guide (Korg, file TR_ParamGuide.pdf, 281 pages, created "
            "2005-10-17)",
            "official Korg download page, product TR, document id 1645",
            "https://www.korg.com/us/support/download/manual/1/211/1645/",
            "effect algorithm catalogue (00-89) and the insert/master effect "
            "routing architecture; the internal-memory statement (470 "
            "multisamples, 33 drum kits)",
        ),
        (
            "D3",
            "TR Operation Guide (Korg, file TR_OpGuide_E2, 136 pages, created "
            "2005-10-17)",
            "official Korg download page, product TR, document id 1644",
            "https://www.korg.com/us/support/download/manual/1/211/1644/",
            "cross-check of the modes and the effect architecture; supplies no "
            "factory list of its own",
        ),
    ],
}

# Per family: how the PCM sections are labelled in the counts table.
SAMPLE_LABELS = {
    "05": (
        "Rom + EX multisamples (PCM waves)",
        "Rom + EX drumsamples (PCM waves)",
    ),
}
DEFAULT_SAMPLE_LABELS = (
    "ROM multisamples (PCM waves)",
    "ROM drumsamples (PCM waves)",
)

# Per family: the slot paragraph of the FX section.
SLOT_PROSE = {
    "05": (
        "**Slots (D2).** The TR provides **one** insert effect (IFX) - D2's "
        "compatibility appendix states it directly (\"The TR provides one insert "
        "effect (IFX)\"; a TRITON file that uses five insert effects loads only "
        "IFX1). Alongside it sit two master effects MFX1-MFX2, whose I/Os are "
        "mono-in / stereo-out, which output wet signal only and whose output "
        "levels are set by Rtn1/Rtn2, and a Master EQ after them. D2's routing: "
        "oscillator / filter / amplifier -> BUS Select (L/R or IFX) -> IFX -> "
        "post-IFX BUS Select (L/R, 1, 2, 1/2, Off) -> sends S1/S2 to MFX1/MFX2 "
        "-> returns mixed into the L/R bus -> Master EQ -> output. Both the "
        "insert effect and the two master effects select from the same catalogue "
        "of 90 entries (00: No Effect plus the 89 effect types below)."
    ),
}
DEFAULT_SLOT_PROSE = (
    "**Slots (D2).** Five insert effects IFX1-IFX5 (chainable), two master "
    "effects MFX1-MFX2 (send/return, mono-in stereo-out), and a Master EQ "
    "after them. D2's routing diagrams place the chain as: oscillator / "
    "filter / amplifier -> IFX1-5 -> MFX send and return -> Master EQ -> "
    "output."
)

# Per family: a note for the drum file where the kit pages need one.
DRUM_NOTES = {
    "05": (
        "**Key range as printed.** Every kit page carries one shared key column "
        "of 88 keys (A0-C8). Two kits print keys above C8: kit 05 assigns C#8 and "
        "D8 in its own column on its page, and kit 15's table continues into a "
        "second half-page column that carries the keys C#8-G#8 (96 key rows in "
        "total). Both are recorded here as the document prints them. The GM kit "
        "pages carry a shared key column of 65 keys (C1-E6).\n\n"
        "**Kit slots without factory content.** The machine has 33 kit slots - these "
        "16 preload kits (`00-15 INT`), 8 user kits (`16-23 User`) and 9 GM kits "
        "(`24-32 GM`). The user slots carry no factory data in any document, so "
        "nothing is listed for them."
    ),
}

# Per family: markdown inserted after the counts bullets.
EXTRA_SECTIONS = {
    "05": """
## What the document states about its own structure (D2)

D2's appendix *Bank/Number structure for programs, combinations, drum kits, and
arpeggio patterns* prints the TR's own ranges. They agree with the lists
enumerated here, which is the cross-check for the sample totals:

- Multisamples: Rom `000-424` (425) and `EX 000-044` (45) = **470** - the same
  figure the parameter text states: "The internal memory contains 470 different
  multisamples (preset multisamples) and 33 drum kits".
- Drumsamples: Rom `000-416` (417) and `EX 000-100` (101) = **518** rows.
- Combinations: banks A, B, C only. Programs: banks A-D, G and the `g(d)` rows.
- Drum kits: 33 in the machine - `00-15 (INT)` preload kits, `16-23 (User)` user
  kits and `24-32 (GM)` GM2-compatible ROM kits. D1 publishes the 16 preload
  kits and the 9 GM kits; the 8 user kits hold no factory data, so nothing is
  listed for them here.

## How this list differs from Family 01 (TRITON)

The TR is a later, smaller machine in the same family, and its preload is its
own, not a subset of the TRITON's. Measured over the two enumerated lists:

| | Family 05 (TR) | Family 01 (TRITON) |
|---|---|---|
| Combinations | **384** (banks A-C) | **512** (banks A-D) |
| Combination names shared with Family 01 | 120 of the 384 | - |
| Programs | **649** rows (A-D 512, G 137) | **777** rows (A-D 512, G 265) |
| GM variant rows in bank G | 9 (`g(d)001`-`g(d)057`) | 137 (`g(1)`-`g(9)` / `g(d)`) |
| Program names shared with Family 01 | 456 of the 635 distinct names | - |
| Preload starts at | combi `A000 StereophonicKeys`, program `A000 Portamento Pad` | combi `A000 Romance Layers`, program `A000 Noisy Stabber` |
| Factory drum kits | **16** | **16** |
| Kit names shared with Family 01 | 9 of 16 | - |
| Kit key rows | 88 (A0-C8); kit 05 adds C#8 and D8, kit 15 adds C#8-G#8 (96) | 88 per kit |
| GM drum kits | **9**, all 9 names shared with Family 01 | **9** |
| Multisamples | **470** - Rom 425 + EX 45 | **425** |
| Multisample names shared with Family 01 | 423 of 470 | - |
| Drumsamples | **518** - Rom 417 + EX 101 | **413** |
| Drumsample names shared with Family 01 | every one of Family 01's 412 distinct names appears in this list | - |
| Insert effects | **1** (IFX) | **5** (IFX1-IFX5) |
| Master effects | 2 (MFX1-MFX2, mono-in / stereo-out, wet only) | 2 (MFX1-MFX2) |
| FX algorithms | **90** entries (`00`-`89`) | **103** entries (`000`-`102`) |

Two consequences for a later conversion unit:

- The TR's effect architecture is not the TRITON's. One insert effect means a
  Family 01 patch that uses IFX2-IFX5 has nowhere to put them; the TR's own
  catalogue also stops at `89: Reverb-Gate` where Family 01 continues to
  `102: Hold Delay`.
- The preloads overlap only partly, so Family 05 is not a duplicate bank. It
  repeats about a third of the TRITON's combination names and roughly
  two-thirds of its program names, and adds 45 multisamples and 105 drumsamples
  the TRITON does not have.
""",
}

def load(path):
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)

HEADER_RULES = """
## Hard rules for this file

- **Internal reference only.** The reference machine's own names (above) may
  appear in this file and in `banks/SOURCES.md`, and nowhere else: not in the UI,
  preset names, file names or plugin metadata.
- Every row is read from a public document listed under *Source documents*; the
  source column names the document and its page.
- Nothing here is a Korg ROM asset. These are name lists and structure taken
  from published documentation; no sample data, no factory PC data.
- Where the document does not state something, the cell says `not documented`.
- **Shipped labels are positions, not document numbers.** The first row of a
  section is `<family>-P0001`, `<family>-C0001`, `<family>-W0001` and so on; the
  `Bank` and `Number` columns carry the document's own bank and number, which is
  what a later conversion unit should match on.
"""


def load(path):
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)


def count_table(sections):
    rows = ["| Section | Count | Documented banks |", "|---|---|---|"]
    for label, items, banks in sections:
        rows.append(f"| {label} | {len(items)} | {banks} |")
    return "\n".join(rows)


def marker_note(name):
    if any(ch in name for ch in "![]{}*<>"):
        return ("name printed with marker characters; recorded exactly as the "
                "document prints it")
    return ""


def group_order(items):
    """Group labels in the order the document first prints them."""
    seen = []
    for i in items:
        g = i.get("group")
        if g and g not in seen:
            seen.append(g)
    return seen


def group_suffix(items):
    """' (Rom 425 + EX 45)' where the document groups its sample list."""
    groups = group_order(items)
    if not groups:
        return ""
    return " (" + " + ".join(
        f"{g} {sum(1 for i in items if i.get('group') == g)}" for g in groups
    ) + ")"


def program_rows(items, prefix, docs_ref="D1"):
    out = ["| Shipped label | Documented name | Bank | Number | Type | Category | Source | Note |",
           "|---|---|---|---|---|---|---|---|"]
    for i, p in enumerate(items, start=1):
        osc = {"s": "single", "d": "double"}.get(p.get("osc") or "", p.get("osc") or "")
        note = marker_note(p["name"])
        if p["number"].startswith("g("):
            note = "document's own GM variant row" + (("; " + note) if note else "")
        out.append(
            f"| {prefix}-P{i:04d} | {p['name']} | {p['bank']} | {p['number']} | "
            f"{osc} | {p['category']} | {docs_ref} p{p['page']} | {note} |"
        )
    return out


def combi_rows(items, prefix, docs_ref="D1"):
    out = ["| Shipped label | Documented name | Bank | Number | Category | Source | Note |",
           "|---|---|---|---|---|---|---|"]
    for i, c in enumerate(items, start=1):
        note = marker_note(c["name"])
        if "slot" in c["name"].lower():
            note = "document prints this row as a placeholder, not a factory sound" + (
                ("; " + note) if note else ""
            )
        out.append(
            f"| {prefix}-C{i:04d} | {c['name']} | {c['bank']} | {c['number']} | "
            f"{c['category']} | {docs_ref} p{c['page']} | {note} |"
        )
    return out


def sample_table(items, prefix, docs_ref="D1"):
    out = ["| Shipped label | Number | Documented name | Group | Source | Note |",
           "|---|---|---|---|---|---|"]
    for i, s in enumerate(sorted(items, key=lambda m: (m["number"], m["page"])), start=1):
        out.append(
            f"| {prefix}-W{i:04d} | {s['number']} | {s['name']} | "
            f"{s.get('group') or '-'} | {docs_ref} p{s['page']} | |"
        )
    return out


def write_drums(args, fam, prefix, data, docs):
    lines = []
    add = lines.append
    add(f"# FAMILY {fam} - drum kits, drum sounds and pad mapping (internal reference)")
    add("")
    add(f"Reference machine (internal name, never shipped): **{args.machine}**.")
    add(f"Shipped label: **Family {fam}**.")
    add("")
    add(HEADER_RULES)
    add("")
    add("## Source documents")
    add("")
    add("| Ref | Document | Where it came from | URL |")
    add("|---|---|---|---|")
    for ref, title, where, url, _ in docs:
        add(f"| {ref} | {title} | {where} | {url} |")
    add("")
    add("## Counts as documented")
    add("")
    add(f"- Factory drum kits: **{len(data['kits'])}**")
    add(f"- GM drum kits: **{len(data['gm_kits'])}**")
    groups = group_order(data["drumsamples"])
    if groups:
        add(
            f"- Drumsample rows: **{len(data['drumsamples'])}** - "
            + " + ".join(
                "%d in the %s group (numbers %d-%d)"
                % (
                    sum(1 for s in data["drumsamples"] if s.get("group") == g),
                    g,
                    min(
                        s["number"]
                        for s in data["drumsamples"]
                        if s.get("group") == g
                    ),
                    max(
                        s["number"]
                        for s in data["drumsamples"]
                        if s.get("group") == g
                    ),
                )
                for g in groups
            )
            + ". The document restarts its numbering inside each group."
        )
    else:
        add(
            "- Distinct drumsample numbers named in the PCM list: "
            f"**{len({s['number'] for s in data['drumsamples']})}** "
            f"({len(data['drumsamples'])} documented rows; the document re-lists "
            f"samples in more than one group)"
        )
    add(
        "- Drum sounds: the document does not publish a separate drum-sound "
        "list for this machine beyond the drumsample names in the PCM list, "
        "which are listed in `FAMILY-%s-PROGRAMS.md`." % fam
    )
    add("")
    add(
        "The document prints each kit as a per-key table: the key label, the "
        "high drumsample, the low drumsample, the exclusive group and the bus "
        "select. Each cell below is the document's own text for that key, in that "
        "order. Where a key row is empty the kit does not assign that key."
    )
    if DRUM_NOTES.get(fam):
        add("")
        add(DRUM_NOTES[fam])
    add("")
    add("## Factory drum kits")
    add("")
    add("| Shipped label | Kit number | Documented kit name | Keys documented | Source |")
    add("|---|---|---|---|---|")
    kit_index = {}
    for i, k in enumerate(data["kits"], start=1):
        label = f"{prefix}-K{i:02d}"
        kit_index[k["number"]] = label
        add(f"| {label} | {k['number']} | {k['name']} | {len(k['keys'])} | D1 p{k['page']} |")
    add("")
    add("## GM drum kits")
    add("")
    add("| Shipped label | Kit number | Documented kit name | Keys documented | Source |")
    add("|---|---|---|---|---|")
    for i, k in enumerate(data["gm_kits"], start=1):
        label = f"{prefix}-GM{i:02d}"
        kit_index[k["number"]] = label
        add(f"| {label} | {k['number']} | {k['name']} | {len(k['keys'])} | D1 p{k['page']} |")
    add("")
    for i, k in enumerate(data["kits"], start=1):
        label = f"{prefix}-K{i:02d}"
        add(f"### {label} - {k['name']} (kit {k['number']}, D1 p{k['page']})")
        add("")
        add("| Shipped label | Key | Documented assignment |")
        add("|---|---|---|")
        for j, e in enumerate(k["keys"], start=1):
            note = e["note"] or "not printed"
            add(f"| {prefix}-K{i:02d}-{j:02d} | {note} | {e['text']} |")
        add("")
    for i, k in enumerate(data["gm_kits"], start=1):
        label = f"{prefix}-GM{i:02d}"
        add(f"### {label} - {k['name']} (GM kit {k['number']}, D1 p{k['page']})")
        add("")
        add("| Shipped label | Key | Documented assignment |")
        add("|---|---|---|")
        for j, e in enumerate(k["keys"], start=1):
            note = e["note"] or "not printed"
            add(f"| {prefix}-GM{i:02d}-{j:02d} | {note} | {e['text']} |")
        add("")
    path = os.path.join(args.out, f"FAMILY-{fam}-DRUMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    print("wrote", path, "lines", len(lines) + 1)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--family", required=True, help="01..05")
    ap.add_argument("--machine", required=True)
    ap.add_argument("--json", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--fx-json", required=True)
    ap.add_argument("--drums-only", action="store_true")
    args = ap.parse_args(argv)

    fam = args.family
    prefix = f"F{fam}"
    data = load(args.json)
    docs = DOCS[fam]
    write_drums(args, fam, prefix, data, docs)
    if args.drums_only:
        return 0

    lines = []
    add = lines.append
    add(f"# FAMILY {fam} - factory programs and combinations (internal reference)")
    add("")
    add(f"Reference machine (internal name, never shipped): **{args.machine}**.")
    add("Shipped label: **Family {0}**.".format(fam))
    add("")
    add("Machine-readable source of these rows: "
        f"`tools/enumerate/triton_vnl.py`"
        + (" (`--profile tr`)" if fam == "05" else "")
        + " run against the Voice Name List PDF; "
        "the PDF itself is not stored in this repository.")
    add(HEADER_RULES)
    add("")
    add("## Source documents")
    add("")
    add("| Ref | Document | Where it came from | URL | What it supplies |")
    add("|---|---|---|---|---|")
    for ref, title, where, url, supplies in docs:
        add(f"| {ref} | {title} | {where} | {url} | {supplies} |")
    add("")
    add("## Counts as documented")
    add("")
    banks_c = ", ".join(sorted({c["bank"] for c in data["combinations"] if c["bank"]}))
    banks_p = ", ".join(sorted({p["bank"] for p in data["programs"] if p["bank"]}))
    multi_label, drum_label = SAMPLE_LABELS.get(fam, DEFAULT_SAMPLE_LABELS)
    add(count_table([
        ("Factory combinations", data["combinations"], banks_c),
        ("Factory programs", data["programs"], banks_p),
        ("Factory drum kits", data["kits"], "n/a"),
        ("GM drum kits", data["gm_kits"], "n/a"),
        (multi_label, data["multisamples"], "n/a"),
        (drum_label, data["drumsamples"], "n/a"),
    ]))
    add("")
    add(f"- Combinations: **{len(data['combinations'])}** documented entries "
        f"(banks {banks_c}).")
    add(f"- Programs: **{len(data['programs'])}** documented entries "
        f"(banks {banks_p}).")
    add(f"- Drum kits: **{len(data['kits'])}** factory kits plus "
        f"**{len(data['gm_kits'])}** GM kits - see `FAMILY-{fam}-DRUMS.md`.")
    add(f"- PCM multisamples: **{len(data['multisamples'])}** documented rows"
        + group_suffix(data["multisamples"]) + ".")
    add(f"- PCM drumsamples: **{len(data['drumsamples'])}** documented rows"
        + group_suffix(data["drumsamples"]) + ".")
    add("")
    if EXTRA_SECTIONS.get(fam):
        add(EXTRA_SECTIONS[fam].strip())
        add("")

    add("## Combinations")
    add("")
    add("Category is the document's own category column. Every row is one "
        "documented factory combination.")
    add("")
    lines.extend(combi_rows(data["combinations"], prefix))
    add("")

    add("## Programs")
    add("")
    add("Type is the document's own single/double program flag. Bank G holds the "
        "GM bank; its `g(1)`-`g(9)` / `g(d)` rows are the document's own variant "
        "entries and are listed here exactly as printed.")
    add("")
    lines.extend(program_rows(data["programs"], prefix))
    add("")

    fx = load(args.fx_json)
    add("## Factory FX algorithms and routing")
    add("")
    add(
        "**Found in documentation: yes** - the algorithm catalogue and the slot "
        "routing are both published in D2. **Per-program FX assignments are not "
        "documented**: no public document states which insert algorithms, chain "
        "order, send levels or master effect a given factory program uses. That "
        "information exists only inside the instrument's own factory data, which "
        "is out of bounds for this project, so per-patch chains have to be "
        "reconstructed by ear against the catalogue below."
    )
    add("")
    add(SLOT_PROSE.get(fam, DEFAULT_SLOT_PROSE))
    add("")
    add("**Selectable ranges (D2).**")
    add("")
    if fam == "05":
        add("| Slot (as the document names it) | Documented range | Source |")
        add("|---|---|---|")
        for r in fx["published_ranges"]:
            add(f"| {r['name']} | {r['from']} - {r['to']} | D2 |")
    else:
        add("| Slot | Documented range | Document's wording |")
        add("|---|---|---|")
        for r in fx["published_ranges"]:
            add(
                f"| see D2 | {r['from']} - {r['to']} | \"{r['name']}\" |"
            )
    add("")
    add(
        "Slot rules read from D2: "
        + ", ".join(f"{k} = {v}" for k, v in sorted(fx["slot_rules"].items()))
        + "."
    )
    add("")
    add(
        f"**Algorithm catalogue (D2), {fx['algorithm_count']} entries, read "
        "from the Parameter Guide's own effect index.**"
    )
    add("")
    add("| # | Algorithm as printed |")
    add("|---|---|")
    for a in fx["algorithms"]:
        add(f"| {a['number']} | {a['name']} |")
    add("")

    add("## PCM wave list (engine input, not content to ingest)")
    add("")
    add(
        "D1 publishes the ROM multisample and drumsample name lists. They are "
        "engine input: they say what the reference machine's waves are called and "
        "how they are numbered. They are not content we may copy, and the numbers "
        "are the document's own numbering."
    )
    add("")
    add("### Multisamples")
    add("")
    lines.extend(sample_table(data["multisamples"], prefix))
    add("")
    add("### Drumsamples")
    add("")
    lines.extend(sample_table(data["drumsamples"], prefix))
    add("")

    path = os.path.join(args.out, f"FAMILY-{fam}-PROGRAMS.md")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    print("wrote", path, "lines", len(lines) + 1)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
