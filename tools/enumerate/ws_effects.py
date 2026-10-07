#!/usr/bin/env python3
"""Extract the WAVESTATION effect-program list from the Reference Guide OCR
texts (two independent scans of the same manual, downloaded from archive.org).

The Wavestation's effect list is only published in the manual's effects chapter,
and both available copies of that manual are image scans whose OCR text is
imperfect. This tool therefore reads BOTH scans, keeps every candidate reading
for each number, and marks an entry as uncertain when the two scans disagree or
when the reading contains text that is plainly a page fragment.

Nothing is invented: entries that cannot be read are emitted as `?` with the raw
readings attached.

Usage:
    python ws_effects.py <refguide.txt> <ad_refguide.txt> <out.json>
"""

from __future__ import annotations

import argparse
import json
import re
import sys

HEADINGS = ("WAVESTATION MDE EFFECTS", "MDE EFFECTS")
NOISE = re.compile(
    r"(?i)\b(EDIT|PATHS|KEYS|SECTION|BANK|COPY|TABLE|MIDI|PAGE|LEVELS|SOURCE)\b"
)
MIX_LINE = re.compile(r"^(DRY|WET)\b[\s,0-9/]*$", re.I)
ENTRY = re.compile(r"(?m)^.{0,8}?(\d{1,2})[\.\s]\s*([^\n]{3,60})$")


BAD_WORDS = re.compile(
    r"(?i)\b(receive|transmit|dump|message|sysex|exclusive|error|request|"
    r"parameter|memory|card|bank|song|track|tempo|sequence dump|no data|"
    r"hours|minutes|seconds|date|version|status)\b"
)


def candidates(path):
    text = open(path, encoding="utf-8", errors="replace").read()
    start = -1
    for h in HEADINGS:
        start = text.find(h)
        if start >= 0:
            break
    region = text[max(0, start - 2500) : start + 90_000] if start >= 0 else text
    out = {}
    for m in ENTRY.finditer(region):
        num = int(m.group(1))
        if not 0 <= num <= 55:
            continue
        name = re.sub(r"\s+", " ", m.group(2)).strip()
        if NOISE.search(name) or BAD_WORDS.search(name) or MIX_LINE.match(name):
            continue
        if not name[0].isupper():
            continue
        if not re.search(r"[a-z]", name):
            continue
        out.setdefault(num, []).append(name)
    return out


def clean(name):
    """Drop obvious trailing scan debris; returns (name, damaged)."""
    dotted = re.search(r"\s*\.\s*\..*$", name)
    if dotted:
        name, damaged = name[: dotted.start()].strip(), True
    else:
        damaged = False
    bad = re.search(r"\s(?:sei|Bos|TT|ED|XP|Wave\.?)$", name) or re.search(
        r"[a-z]{2,}[A-Z]", name
    ) or re.search(r"\s+[a-z]{2,4}\s+[A-Z][a-z]$", name)
    if bad:
        name = name[: bad.start()].strip()
        damaged = True
    return name, damaged


UNIT = re.compile(r"\b\d+\s*(?:ms|Hz|kHz|dB|%)", re.I)


def score(name):
    """Higher is better: effect names are short phrases with lower-case letters."""
    s = 0
    if re.search(r"[a-z]", name):
        s += 3
    if re.search(r"(?i)\b(reverb|delay|chorus|flanger|phaser|vocoder|speaker|"
                 r"eq|pan|comp|shifter|gated|exciter|distortion|overdrive|"
                 r"reflections|room|hall|plate|stage|enhancer|parametric|"
                 r"rotary|ping-pong|mono|stereo|quadrature|crossover)\b", name):
        s += 4
    if UNIT.search(name):
        s -= 5
    if len(name) > 40:
        s -= 2
    if name.lower().startswith(("to ", "and ", "or ")):
        s -= 4
    return s


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("refguide")
    ap.add_argument("ad_refguide")
    ap.add_argument("out")
    args = ap.parse_args(argv)

    a = candidates(args.refguide)
    b = candidates(args.ad_refguide)
    entries = []
    for num in range(0, 56):
        ca, cb = a.get(num, []), b.get(num, [])
        cands = list(dict.fromkeys(ca + cb))[:6]
        if not cands:
            entries.append(
                {"number": f"{num:02d}", "name": "?", "note": "not readable in either scan"}
            )
            continue
        best = max(
            cands,
            key=lambda c: (
                score(c) + (2 if c in ca and c in cb else 0),
                -cands.index(c),
            ),
        )
        name, damaged = clean(best)
        note = ""
        if damaged:
            note = "scan OCR carries trailing page text; name trimmed at the last clean word"
        if ca and cb and ca[0] != cb[0]:
            note = (note + "; " if note else "") + (
                "the two scans read this entry differently: " + " / ".join([ca[0], cb[0]])
            )
        entries.append({"number": f"{num:02d}", "name": name, "note": note})
    with open(args.out, "w", encoding="utf-8") as fh:
        json.dump(
            {"sources": [args.refguide, args.ad_refguide], "entries": entries},
            fh,
            indent=1,
            ensure_ascii=False,
        )
    print("wrote", args.out, "entries", len(entries))
    for e in entries:
        print(f"  {e['number']} {e['name']:<34} {e['note'][:70]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
