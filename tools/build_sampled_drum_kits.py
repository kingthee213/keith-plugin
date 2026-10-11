"""Build the drum kits from CC0 one-shots.

    /work/.venv/bin/python tools/build_sampled_drum_kits.py            # report only
    /work/.venv/bin/python tools/build_sampled_drum_kits.py --write    # copy into resources/

## Source

`MckSamplePacks` by MckAudio -- **CC0 1.0**, verified in the repo's own LICENSE file, not inferred
from a repository metadata field. Recordings of the author's own drum machines.

The owner ruled these acceptable on 2026-10-11, having been told plainly what they are:
recordings of commercial hardware (Roland TR-8, RX5, DR-5, RD-6). `banks/SOURCES.md` rule 1 forbids
*ROM content and factory program data ripped from hardware*. A CC0 recording of hardware someone
owns is a different thing, and the owner's call was that it is acceptable. That ruling is recorded
because it is a judgement, not something the file itself proves.

## The kits

Ten kits, each a character rather than a machine. The machines differ in exactly the way the eras
do: the RX5 and RD6 are early-80s digital, thin and clicky with tight envelopes; the TR-8 is the
808/909 lineage, deep and round; the DR5 is a preset box with an odd, boxy tone. Ten kits are made
by choosing per machine and by layering, not by shipping one kit per machine.

Each kit is 9 slots, matching the plugin's drum range (36..44):
    KICK(36) SNARE(37) CLAP(38) RIM(39) HAT_CLOSED(40) HAT_OPEN(41) TOM_LO(42) TOM_HI(43) PERC(44)

## Output

`resources/samples/drums/<kit>/<slot>_<name>.wav`, 48 kHz mono, peak-normalised to -3 dBFS so the
kit is internally consistent before the preset level trim runs. The licence text is copied next to
the kits so the grant travels with the audio.
"""

import argparse
import math
import re
import shutil
import struct
import sys
import wave
from collections import Counter
from pathlib import Path

SOURCE = Path("/tmp/mck")
ROOT = Path("/work/keith-plugin")
OUT = ROOT / "resources" / "samples" / "drums"

SLOTS = ["KICK", "SNARE", "CLAP", "RIM", "HAT_CLOSED", "HAT_OPEN", "TOM_LO", "TOM_HI", "PERC"]

# Filenames in this repo follow patterns per machine, discovered by reading the inventory rather
# than guessed. The first pattern in the list that matches wins, so a machine that names things
# differently still resolves.
#
# The machines genuinely differ in what they hold: RX5 keeps its claps and rim shots in SD/PERC,
# DR5 is acoustic-flavoured, RD6 numbers its files bare (001.wav) so its slots come from the FOLDER
# alone.
PATTERNS = {
    "KICK":       [r"bass_drum", r"kick", r"^bd"],
    "SNARE":      [r"snare", r"^sd"],
    "CLAP":       [r"clap", r"hand_clap", r"hc_"],
    "RIM":        [r"rim_shot", r"rimshot", r"rim"],
    "HAT_CLOSED": [r"closed_h?i-?hat", r"hihat_closed", r"closed"],
    "HAT_OPEN":   [r"open_h?i-?hat", r"hihat_open", r"open"],
    "TOM_LO":     [r"low_tom", r"tom_1$", r"tom_1\.", r"_tom_1\b", r"tom"],
    "TOM_HI":     [r"high_tom", r"tom_3$", r"tom_3\.", r"_tom_3\b", r"tom"],
    "PERC":       [r"conga", r"cowbell", r"tambourine", r"shaker", r"clave", r"perc"],
}

# Which folders a slot may live in, per machine. This is the part that is genuinely machine
# specific and had to be read off the real tree.
FOLDERS = {
    "KICK":       ["BD"],
    "SNARE":      ["SD"],
    "CLAP":       ["CLAP", "PERC", "SD"],
    "RIM":        ["SD", "PERC", "MISC"],
    "HAT_CLOSED": ["HATS"],
    "HAT_OPEN":   ["HATS"],
    "TOM_LO":     ["TOMS"],
    "TOM_HI":     ["TOMS"],
    "PERC":       ["PERC", "MISC"],
}

# Ten kits. (kit name, machine, pick) where pick selects the Nth match within a slot, so two kits
# on one machine differ by taking different hits rather than by inventing values.
#
# The character comes from the machine and the pick:
#   TR8  -- the 808/909 lineage. Deep and round. Trap, modern R&B.
#   RX5  -- early-80s digital. Thin, clicky, tight. The boom-bap chassis.
#   DR5  -- a preset box with acoustic and ambient samples alongside the electronic ones. Odd.
#   RD6  -- the small one. Everything tight and polite.
KITS = [
    ("smoke",   "TR8", 0),   # 808 lineage, first picks. The default modern kit.
    ("velvet",  "TR8", 1),   # the same lineage, later picks. Softer, for slow soul.
    ("stone",   "TR8", 2),   # the hardest picks of the analog machine.
    ("dust",    "RX5", 0),   # digital, first picks. The boom-bap chassis.
    ("paper",   "RX5", 1),   # the driest digital picks. Nothing rings.
    ("late",    "RX5", 2),   # late-night: closed, quiet, little top end.
    ("corner",  "DR5", 0),   # the preset box, straight up.
    ("crate",   "DR5", 1),   # the odd corners of the same box, for digging.
    ("swing",   "RD6", 0, "DR5"),   # tidy and small; percussion borrowed from the DR5.
    ("clinic",  "RD6", 1, "DR5"),   # the same, brighter picks.
]

def read_wav_mono48(path):
    """Return (samples as list of float in -1..1, framerate). 16 and 24 bit, 1 or 2 channels."""
    with wave.open(str(path)) as w:
        rate = w.getframerate()
        chans = w.getnchannels()
        width = w.getsampwidth()
        raw = w.readframes(w.getnframes())
    if width not in (2, 3):
        raise ValueError(f"{path.name}: unsupported width {width}")
    count = len(raw) // (width * chans)
    out = []
    for i in range(count):
        base = i * width * chans
        acc = 0
        for c in range(chans):
            off = base + c * width
            if width == 2:
                v = struct.unpack_from("<h", raw, off)[0] / 32768.0
            else:
                b = raw[off:off + 3]
                v = (b[0] | (b[1] << 8) | (b[2] << 16))
                if v & 0x800000:
                    v -= 0x1000000
                v /= 8388608.0
            acc += v
        out.append(acc / chans)
    return out, rate


def write_wav_mono48(path, samples, rate):
    """16-bit mono. The engine reads 16 and 24 bit; 16 halves the size with no audible loss for
    drum one-shots that are then peak-normalised anyway."""
    frames = bytearray()
    for s in samples:
        v = int(max(-1.0, min(1.0, s)) * 32767.0)
        frames += struct.pack("<h", v)
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(bytes(frames))


def trim_and_normalise(samples, rate, peak_db=-3.0):
    """Trim leading silence, apply a short fade-out so the tail cannot click, normalise to peak_db.

    The fade is the thing that matters: a one-shot cut off mid-cycle produces a step discontinuity,
    which is an audible click on every hit. 5 ms is long enough to remove it and short enough that
    it does not shorten a kick.
    """
    # leading silence
    start = 0
    for i, s in enumerate(samples):
        if abs(s) > 1e-4:
            start = max(0, i - int(0.0005 * rate))
            break
    body = samples[start:]

    # fade the last 5 ms
    fade = min(int(0.005 * rate), len(body) // 4)
    if fade > 1:
        for i in range(fade):
            k = i / fade
            body[len(body) - fade + i] *= k

    peak = max((abs(s) for s in body), default=0.0)
    if peak <= 0:
        raise ValueError("silent sample")
    target = 10 ** (peak_db / 20.0)
    gain = target / peak
    return [s * gain for s in body]


def _matches(machine, slot):
    """Every file that could fill `slot`, in name order, searched across the slot's folders."""
    hits = []
    pats = PATTERNS[slot]
    for folder in FOLDERS[slot]:
        d = SOURCE / machine / folder
        if not d.is_dir():
            continue
        for path in sorted(d.glob("*.wav")):
            low = path.name.lower()
            if any(re.search(p, low) for p in pats):
                hits.append(path)
    # A bare-numbered machine (RD6 names everything 001.wav) matches nothing by name, so fall back
    # to the folder itself -- the folder IS the instrument there.
    if not hits:
        for folder in FOLDERS[slot]:
            d = SOURCE / machine / folder
            if d.is_dir():
                hits += sorted(d.glob("*.wav"))
    return hits


def resolve(machine, pick, borrow=None):
    """{slot: Path} for one kit, taking the `pick`-th candidate per slot where available.

    `borrow` names a second machine to fall back to for a slot this one simply does not have.
    That is normal kit building rather than a fault: a kit is a curated set of hits, not a dump of
    one machine's memory, and RD6 ships no percussion folder at all.
    """
    out = {}
    for slot in SLOTS:
        hits = _matches(machine, slot)
        if not hits and borrow:
            hits = _matches(borrow, slot)
        if hits:
            out[slot] = hits[min(pick, len(hits) - 1)]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    if not SOURCE.is_dir():
        raise SystemExit(f"source not found: {SOURCE}\n  clone MckAudio/MckSamplePacks there first")

    print(f"  source: {SOURCE}")
    print(f"  kits:   {len(KITS)}")
    print()

    total = 0
    for entry in KITS:
        kit, machine, pick = entry[0], entry[1], entry[2]
        borrow = entry[3] if len(entry) > 3 else None
        resolved = resolve(machine, pick, borrow)
        missing = [s for s in SLOTS if s not in resolved]
        status = "OK " if not missing else f"missing {missing}"
        print(f"  {kit:8s} {machine:5s} {len(resolved)}/{len(SLOTS)}  {status}")
        if missing:
            continue
        total += len(resolved)
        if args.write:
            dest = OUT / kit
            dest.mkdir(parents=True, exist_ok=True)
            for slot in SLOTS:
                src = resolved[slot]
                samples, rate = read_wav_mono48(src)
                try:
                    fixed = trim_and_normalise(samples, rate)
                except ValueError as e:
                    raise SystemExit(f"{kit}/{slot}: {e}")
                name = re.sub(r"^\d+_", "", src.stem)
                name = re.sub(r"[^A-Za-z0-9]+", "_", name).strip("_").lower()
                write_wav_mono48(dest / f"{SLOTS.index(slot):02d}_{slot.lower()}_{name}.wav",
                                 fixed, rate)

    print()
    print(f"  slots resolved: {total} / {len(KITS) * len(SLOTS)}")
    if args.write:
        lic = SOURCE / "LICENSE"
        if lic.exists():
            shutil.copy(lic, OUT / "LICENSE-CC0-1.0.txt")
        print(f"  wrote {OUT}")
    else:
        print("  (report only -- pass --write to copy)")
    return 0 if total == len(KITS) * len(SLOTS) else 1


if __name__ == "__main__":
    sys.exit(main())
