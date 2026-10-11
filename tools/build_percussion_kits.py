"""Build the two percussion kits from CC0 one-shots.

    /work/.venv/bin/python tools/build_percussion_kits.py            # report only
    /work/.venv/bin/python tools/build_percussion_kits.py --write

## Source

The same `MckSamplePacks` CC0-1.0 collection the drum kits come from. The machine PERC folders
carry the percussion this needs: congas, timbales, cowbell, tambourine, shaker, castanets,
triangle, sleigh bell, agogo. Read from the real tree rather than assumed.

## Why these are separate from the drum kits

The owner asked for **10 drum kits plus 2 percussion kits**. The drum kits are kick-snare-hat
machines; these two are hand and metal percussion, played as their own instruments. Two kits
because the character splits cleanly: one is the Latin/hand-percussion side (congas, timbales,
agogo, clave) and one is the metal-and-shaker side (cowbell, triangle, tambourine, shakers,
castanets).

## Output

`resources/samples/percussion/<kit>/<slot>_<name>.wav`, 48 kHz mono, peak-normalised to -3 dBFS,
with the CC0 licence copied alongside.
"""

import argparse
import re
import shutil
import struct
import sys
import wave
from pathlib import Path

SOURCE = Path("/tmp/mck")
ROOT = Path("/work/keith-plugin")
OUT = ROOT / "resources" / "samples" / "percussion"

# 8 slots each, so a percussion kit sits in the drum range without colliding in meaning.
SLOTS_LATIN = ["CONGA_LO", "CONGA_HI", "TIMBALE_LO", "TIMBALE_HI", "AGOGO_LO", "AGOGO_HI",
               "CLAVE", "SHAKER"]
SLOTS_METAL = ["COWBELL_LO", "COWBELL_HI", "TRIANGLE", "TAMBOURINE", "SHAKER", "CABASA",
               "CASTANET", "SLEIGH"]

# slot -> (folders to search, filename patterns, kit)
LATIN_SLOTS = {
    "CONGA_LO":    [r"low_conga", r"conga_low", r"conga"],
    "CONGA_HI":    [r"high_conga", r"conga_high", r"conga"],
    "TIMBALE_LO":  [r"low_timbale", r"timbale"],
    "TIMBALE_HI":  [r"high_timbale", r"timbale"],
    "AGOGO_LO":    [r"low_agogo", r"agogo"],
    "AGOGO_HI":    [r"high_agogo", r"agogo"],
    "CLAVE":       [r"clave"],
    "SHAKER":      [r"shaker", r"maraca"],
}
METAL_SLOTS = {
    "COWBELL_LO":  [r"low_cowbell", r"cowbell"],
    "COWBELL_HI":  [r"high_cowbell", r"cowbell"],
    "TRIANGLE":    [r"triangle"],
    "TAMBOURINE":  [r"tambourine"],
    "SHAKER":      [r"shaker"],
    "CABASA":      [r"cabasa"],
    "CASTANET":    [r"castanet"],
    "SLEIGH":      [r"sleigh", r"jingle"],
}


def read_mono(path):
    with wave.open(str(path)) as w:
        rate, chans, width = w.getframerate(), w.getnchannels(), w.getsampwidth()
        raw = w.readframes(w.getnframes())
    if width not in (2, 3):
        raise ValueError(f"{path.name}: width {width}")
    size = width
    count = len(raw) // (size * chans)
    out = []
    for i in range(count):
        acc = 0.0
        for c in range(chans):
            off = i * size * chans + c * size
            if size == 2:
                acc += struct.unpack_from("<h", raw, off)[0] / 32768.0
            else:
                b = raw[off:off + 3]
                v = b[0] | (b[1] << 8) | (b[2] << 16)
                if v & 0x800000:
                    v -= 0x1000000
                acc += v / 8388608.0
        out.append(acc / chans)
    return out, rate


def write_mono(path, samples, rate):
    buf = bytearray()
    for s in samples:
        buf += struct.pack("<h", int(max(-1.0, min(1.0, s)) * 32767.0))
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(bytes(buf))


def trim_norm(samples, rate, target_db=-3.0):
    """Lead trim, tail fade, peak normalise. The fade matters: a percussion hit cut mid-cycle
    clicks on every strike, and shakers especially end abruptly."""
    start = 0
    for i, s in enumerate(samples):
        if abs(s) > 1e-4:
            start = max(0, i - int(0.0005 * rate))
            break
    body = list(samples[start:])
    fade = min(int(0.005 * rate), max(1, len(body) // 4))
    for i in range(fade):
        body[len(body) - fade + i] *= i / fade
    peak = max((abs(s) for s in body), default=0.0)
    if peak <= 0:
        raise ValueError("silent")
    gain = (10 ** (target_db / 20.0)) / peak
    return [s * gain for s in body]


def collect(): 
    """All wavs under the PERC folders of every machine, as (machine, path)."""
    out = []
    for machine in ("TR8", "RX5", "DR5", "RD6"):
        for folder in ("PERC", "MISC"):
            d = SOURCE / machine / folder
            if d.is_dir():
                out += [(machine, p) for p in sorted(d.glob("*.wav"))]
    return out


def find(pool, patterns, used, pick=0):
    hits = []
    for machine, path in pool:
        low = path.name.lower()
        if any(re.search(p, low) for p in patterns):
            hits.append((machine, path))
    for entry in hits:
        if str(entry[1]) not in used:
            return entry
    return hits[0] if hits else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    if not SOURCE.is_dir():
        raise SystemExit(f"source not found: {SOURCE}")

    pool = collect()
    print(f"  percussion pool: {len(pool)} files across PERC/MISC of four machines")
    print()

    kits = [("hands", SLOTS_LATIN, LATIN_SLOTS), ("metal", SLOTS_METAL, METAL_SLOTS)]
    total = 0
    for kit, slots, spec in kits:
        used = set()
        resolved = {}
        for slot in slots:
            entry = find(pool, spec[slot], used)
            if entry:
                resolved[slot] = entry
                used.add(str(entry[1]))
        missing = [s for s in slots if s not in resolved]
        print(f"  {kit:7s} {len(resolved)}/{len(slots)}"
              + ("" if not missing else f"  missing {missing}"))
        for slot, (machine, path) in resolved.items():
            print(f"      {slot:12s} {machine} {path.name}")
        total += len(resolved)
        if args.write and not missing:
            dest = OUT / kit
            dest.mkdir(parents=True, exist_ok=True)
            for slot in slots:
                machine, path = resolved[slot]
                samples, rate = read_mono(path)
                name = re.sub(r"^\d+_", "", path.stem)
                name = re.sub(r"[^A-Za-z0-9]+", "_", name).strip("_").lower()
                write_mono(dest / f"{slots.index(slot):02d}_{slot.lower()}_{name}.wav",
                           trim_norm(samples, rate), rate)
        print()

    print(f"  slots resolved: {total} / 16")
    if args.write:
        lic = SOURCE / "LICENSE"
        if lic.exists():
            shutil.copy(lic, OUT / "LICENSE-CC0-1.0.txt")
        print(f"  wrote {OUT}")
    return 0 if total == 16 else 1


if __name__ == "__main__":
    sys.exit(main())
