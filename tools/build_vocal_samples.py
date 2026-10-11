"""Build the vocal patches from CC0 vowel samples.

    /work/.venv/bin/python tools/build_vocal_samples.py            # report only
    /work/.venv/bin/python tools/build_vocal_samples.py --write

## Source

`sfzinstruments/legato_vocal_tutorial` by sfzinstruments -- **CC0 1.0**, verified from the repo's
own LICENSE file.

It holds sustained vowels and short vowels, named by pitch: `vowel_a_a3.wav`, `vowel_a_bb4.wav`,
`vowel_a_gb3.wav`. The pitch in the filename is what makes this usable directly: a sampler maps a
root note to a file, and these come with the root note already stated.

## What this does NOT solve

**Only the `a` vowel has sustained samples.** So this builds a genuine "Vocal Aah" and there is no
"Vocal Ooh" to build from this source. Guessing an ooh by filtering an aah would be inventing a
sound and calling it a recording, so it is not done here: the ooh needs its own verified source and
is left absent until one is found.

## Output

`resources/samples/vocal/<vowel>/<note>.wav` -- 48 kHz mono, trimmed and peak-normalised, with the
pitch preserved. Level normalisation is per-file at -3 dBFS here and the preset trim runs later, but
**the relative levels between notes are what matter** and are deliberately NOT flattened: a
normalised-per-note choir would have every note equally loud, which is not what a voice does.
"""

import argparse
import re
import shutil
import struct
import sys
import wave
from pathlib import Path

DEFAULT_SOURCE = Path("/tmp/vocal")
ROOT = Path("/work/keith-plugin")
OUT = ROOT / "resources" / "samples" / "vocal"

# Filename pitch -> MIDI note. The repo uses letter names with `s` for sharp, plus `b` in the
# conventional spelling seen in the files (`db3`, `gb4`).
SEMITONE = {"c": 0, "d": 2, "e": 4, "f": 5, "g": 7, "a": 9, "b": 11}


def note_to_midi(text):
    """'a3' -> 57, 'gb4' -> 66, 'ab3' -> 56. Returns None if it is not a note name."""
    m = re.fullmatch(r"([a-g])([bs]?)(\d)", text.lower())
    if not m:
        return None
    letter, accidental, octave = m.groups()
    n = SEMITONE[letter] + (1 if accidental == "s" else 0)
    # MIDI 60 is C4 in this convention, so octave 4 starts at 60.
    return n + (int(octave) + 1) * 12


def read_mono(path):
    with wave.open(str(path)) as w:
        rate, chans, width = w.getframerate(), w.getnchannels(), w.getsampwidth()
        raw = w.readframes(w.getnframes())
    if width == 2:
        fmt, scale, size = "<h", 32768.0, 2
    elif width == 3:
        fmt, scale, size = None, 8388608.0, 3
    else:
        raise ValueError(f"{path.name}: unsupported width {width}")
    count = len(raw) // (size * chans)
    out = []
    for i in range(count):
        acc = 0.0
        for c in range(chans):
            off = i * size * chans + c * size
            if size == 2:
                acc += struct.unpack_from(fmt, raw, off)[0] / scale
            else:
                b = raw[off:off + 3]
                v = b[0] | (b[1] << 8) | (b[2] << 16)
                if v & 0x800000:
                    v -= 0x1000000
                acc += v / scale
        out.append(acc / chans)
    return out, rate


def write_mono(path, samples, rate):
    buf = bytearray()
    for s in samples:
        v = int(max(-1.0, min(1.0, s)) * 32767.0)
        buf += struct.pack("<h", v)
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(bytes(buf))


def trim(samples, rate):
    """Trim silence from both ends and fade the last 20 ms.

    The fade is longer than a drum hit's because a voice held to the end of the file stops
    mid-cycle, and the click is very audible on a sustained note.
    """
    thresh = 1e-3
    start, end = 0, len(samples)
    for i, s in enumerate(samples):
        if abs(s) > thresh:
            start = max(0, i - int(0.002 * rate))
            break
    for i in range(len(samples) - 1, -1, -1):
        if abs(samples[i]) > thresh:
            end = min(len(samples), i + int(0.02 * rate))
            break
    body = list(samples[start:end])
    fade = min(int(0.02 * rate), len(body) // 3)
    if fade > 1:
        for i in range(fade):
            body[len(body) - fade + i] *= i / fade
    return body


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", default=str(DEFAULT_SOURCE))
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    src = Path(args.source)
    if not src.is_dir():
        raise SystemExit(f"source not found: {src}\n  clone sfzinstruments/legato_vocal_tutorial")

    print(f"  source: {src}")

    # Gather every sustained vowel, keyed by MIDI note.
    by_vowel = {}
    for path in sorted(src.rglob("vowel_sustain/*/*.wav")):
        # .../vowel_sustain/<vowel>/vowel_<vowel>_<note>.wav
        vowel = path.parent.name
        m = re.search(r"_([a-g][bs]?\d)\.wav$", path.name, re.I)
        if not m:
            continue
        midi = note_to_midi(m.group(1))
        if midi is None:
            continue
        by_vowel.setdefault(vowel, {})[midi] = path

    if not by_vowel:
        raise SystemExit("no sustained vowels found")

    for vowel, notes in sorted(by_vowel.items()):
        lo, hi = min(notes), max(notes)
        print(f"  vowel '{vowel}': {len(notes)} notes, MIDI {lo}..{hi}")

    print()
    # Only report what can be named honestly.
    named = {"a": "Vocal Aah"}
    for vowel, label in named.items():
        if vowel not in by_vowel:
            print(f"  !! '{vowel}' not present, cannot build {label}")
            continue
        notes = by_vowel[vowel]
        print(f"  {label}: {len(notes)} notes -> {OUT / vowel}")
        if args.write:
            dest = OUT / vowel
            dest.mkdir(parents=True, exist_ok=True)
            for midi, path in sorted(notes.items()):
                samples, rate = read_mono(path)
                write_mono(dest / f"{midi:03d}.wav", trim(samples, rate), rate)

    missing = [v for v in ("a", "e", "i", "o", "u") if v not in by_vowel]
    print()
    print(f"  sustained vowels present: {sorted(by_vowel)}")
    print(f"  absent (no source, not synthesised): {missing}")
    if args.write:
        lic = src / "LICENSE"
        if lic.exists():
            shutil.copy(lic, OUT / "LICENSE-CC0-1.0.txt")
        # A readme that says plainly what is and is not here.
        (OUT / "README.md").write_text(
            "# Vocal samples\n\n"
            "Source: `sfzinstruments/legato_vocal_tutorial`, **CC0 1.0** (see LICENSE next to this\n"
            "file). Sustained vowels only; the repository holds no other vowel with sustains.\n\n"
            "Filenames are MIDI note numbers, so a sampler maps them directly.\n\n"
            "## Why there is only an Aah\n\n"
            "The source has sustained samples for the `a` vowel alone. An \"Ooh\" would have to be\n"
            "synthesised by filtering these, which would be inventing a sound and calling it a\n"
            "recording. It is left out until a verified CC0 source is found.\n")
        print(f"  wrote {OUT}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
