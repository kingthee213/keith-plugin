"""Build the guitar banks from CC0 multisamples.

    /work/.venv/bin/python tools/build_guitar_samples.py            # report only
    /work/.venv/bin/python tools/build_guitar_samples.py --write

## Source and licence

FreePats project banks, **CC0 1.0**, verified from the `LICENSE.txt` / `cc0.txt` that ships inside
each archive rather than from the web page alone:

    clean      FreePats FSBS Clean Electric Guitar      direct-sampled Fender
    jazz       FreePats FSBS Clean Electric (Jazz)      the same, warmer
    dist1      FreePats FSBS Distorted Electric 1       the same, driven
    dist2      FreePats FSBS Distorted Electric 2       the same, differently driven
    nylon      FreePats Spanish Classical Guitar        mic'd nylon-string

**Steel-string is deliberately absent.** The only FreePats steel-string bank is GPL-3.0-with-
exception, whose exception is written for mixing samples into a *composition* -- it does not
clearly grant redistributing the sample files inside a product other people play. FreePats have
also deprecated that licence. Not worth mixing one ambiguous grant into fourteen clean ones.

## Why FLAC directly

The plugin's loader already decodes FLAC through dr_flac (see `SampleLoader.cpp`), so these drop in
without conversion -- and a FLAC bank is a fraction of the WAV size for identical audio. Nothing is
re-encoded, so the audio that ships is the audio FreePats published.

## Velocity layers

The banks are velocity-layered: `_soft_` files and plain files, plus `lovel`/`hivel` ranges in the
SFZ. The plugin's sampler holds ONE sample per note, so a layer has to be chosen. The **plain**
(non-soft) layer is taken, which is the middle-and-up region and the one a played line normally
lands in. The soft layer is a different recording, not a volume change, so this is a real (small)
loss of dynamics rather than something that could be faked with velocity scaling.

## Round-robins

Four takes per note (`_01`..`_04`). One is kept rather than blending them: the sampler has no
round-robin machinery, and averaging takes would comb-filter against each other. The first is taken
deterministically so the bank is reproducible.
"""

import argparse
import re
import shutil
import sys
from pathlib import Path

SRC = Path("/tmp/guitars")
ROOT = Path("/work/keith-plugin")
OUT = ROOT / "resources" / "samples" / "guitars"

# bank key -> (source dir, human name). The dirs are the extracted archive roots.
BANKS = {
    "clean":  ("clean",  "FreePats FSBS Clean Electric Guitar"),
    "jazz":   ("jazz-small", "FreePats FSBS Clean Electric Guitar (Jazz)"),
    "dist1":  ("dist1",  "FreePats FSBS Distorted Electric Guitar 1"),
    "dist2":  ("dist2",  "FreePats FSBS Distorted Electric Guitar 2"),
    "nylon":  ("nylon",  "FreePats Spanish Classical Guitar"),
}

# Filenames carry the pitch: `C2_s1_01.flac`, `G#5_s6_soft_02.flac`, or bare `C2.flac` for the
# nylon bank which is not string-tagged.
NOTE_RE = re.compile(r"([A-G]#?)(-?\d)(?:_|\.)")

SEMITONE = {"C": 0, "D": 2, "E": 4, "F": 5, "G": 7, "A": 9, "B": 11}


def note_to_midi(letter, octave):
    """'C' + 2 -> 36, 'A#' + 5 -> 82. MIDI 60 is C4, so octave 4 starts at 60."""
    n = SEMITONE[letter[0]] + (1 if letter.endswith("#") else 0)
    return n + (int(octave) + 1) * 12


def find_samples(root):
    """{midi: path} for the plain (non-soft) layer, one round-robin take per note."""
    found = {}
    for path in sorted(root.rglob("*.flac")):
        if "soft" in path.name.lower():
            continue   # the soft layer is a separate recording; the plain layer is used
        m = NOTE_RE.search(path.name)
        if not m:
            continue
        letter, octave = m.group(1), m.group(2)
        midi = note_to_midi(letter, octave)
        # First take wins, deterministically, because sorted() fixes the order.
        found.setdefault(midi, path)
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    if not SRC.is_dir():
        raise SystemExit(f"source not found: {SRC}\n  run /work/fetch-guitars.sh and extract")

    print(f"  source: {SRC}")
    print()
    plans = {}
    for key, (sub, label) in BANKS.items():
        roots = list((SRC / sub).rglob("samples"))
        if not roots:
            print(f"  {key:7s} MISSING (no samples/ dir under {sub})")
            continue
        found = find_samples(roots[0].parent)
        if not found:
            print(f"  {key:7s} MISSING (no notes found)")
            continue
        lo, hi = min(found), max(found)
        print(f"  {key:7s} {len(found):>3} notes  MIDI {lo:>3}..{hi:<3}  {label}")
        plans[key] = found

    print()
    total = sum(len(v) for v in plans.values())
    print(f"  banks: {len(plans)}   samples: {total}")
    if not args.write:
        print("  (report only -- pass --write to copy)")
        return 0

    for key, found in plans.items():
        dest = OUT / key
        dest.mkdir(parents=True, exist_ok=True)
        for midi, path in sorted(found.items()):
            # Copied unaltered, as FLAC, under a name that states the note. No trim, no fade, no
            # normalisation: these are held notes with a real decay, and the whole point of
            # sampling them was to keep that. Level matching is the preset trim's job later.
            shutil.copyfile(path, dest / f"{midi:03d}.flac")

    # The licence travels with the audio.
    for key, (sub, _) in BANKS.items():
        for lic in ("LICENSE.txt", "cc0.txt"):
            for cand in (SRC / sub).rglob(lic):
                shutil.copyfile(cand, OUT / f"{key}-{lic}")
                break

    (OUT / "README.md").write_text(
        "# Guitar samples\n\n"
        "All five banks are **CC0 1.0** (public domain dedication, no attribution required),\n"
        "verified from the licence file shipped inside each FreePats archive.\n\n"
        "| bank | source | instrument |\n"
        "|---|---|---|\n"
        "| clean | FreePats FSBS Clean Electric | direct-sampled electric guitar |\n"
        "| jazz | FreePats FSBS Clean Electric (Jazz) | the same, warmer |\n"
        "| dist1 | FreePats FSBS Distorted Electric 1 | the same, driven |\n"
        "| dist2 | FreePats FSBS Distorted Electric 2 | the same, driven |\n"
        "| nylon | FreePats Spanish Classical Guitar | mic'd nylon-string |\n\n"
        "Files are `<midi note>.flac`, copied unaltered. FLAC because the plugin's loader decodes it\n"
        "through dr_flac, so no re-encode is needed and the audio that ships is the audio published.\n\n"
        "## What is not here\n\n"
        "**Steel-string acoustic.** The only FreePats steel-string bank is GPL-3.0-with-exception,\n"
        "and that exception is written for mixing samples into a composition -- it does not clearly\n"
        "grant redistributing the sample files inside a product that other people play. The licence\n"
        "is also deprecated by FreePats. One ambiguous grant is not worth mixing into fourteen clean\n"
        "ones, so that patch stays synthesised until a CC0 steel-string turns up.\n\n"
        "## Velocity and round-robin\n\n"
        "The banks are velocity-layered and carry four round-robin takes per note. One layer (the\n"
        "plain, non-soft) and one take are kept, because the plugin's sampler holds one sample per\n"
        "note. The soft layer is a separate recording rather than a volume change, so this is a\n"
        "small real loss of dynamics, not something velocity scaling could have reproduced.\n")
    print(f"  wrote {OUT}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
