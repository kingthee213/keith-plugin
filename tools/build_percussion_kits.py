"""Build the two percussion kits from the CC0 acoustic drum kit.

    /work/.venv/bin/python /work/tools/build_percussion_kits.py

The owner asked for two of the ten kits to be percussion, searching based on Neptunes production
and Korg ROM character, then chose: both built from the SFXMint CC0 acoustic kit, re-edited into
percussion, one of each character.

## Why re-editing rather than sourcing percussion directly

SFXMint has no percussion set -- its 18 sets are one acoustic drum kit plus UI, game, animal and
foley sounds. So percussion is *derived* from the drum kit, which the owner explicitly allowed:
"we can edit if we need to."

Every hit here is one of the 13 CC0 sources, transformed. Nothing new is ingested, so the
provenance chain stays short: CC0 source, edited by us.

## The two characters

**Neptunes** -- thin, dry, tight, high-tuned, lots of space. The trick is avery short decay, a
high-pass to remove body, and a bright top. Percussion in that production sits above the mix with
air around it rather than thumping.

**Korg ROM** -- the M1/Triton percussion flavour: bright, punchy, slightly synthetic, with a
harder attack and a more even, "produced" tail. Achieved by adding pitch and a short bright
transient, and by shortening the tail so hits sound triggered rather than played.

Both are fully derived, so both are recorded as "our own design, derived from CC0 source" rather
than as ingested samples.
"""

import hashlib
import json
import math
import struct
import sys
import urllib.request
import wave
from pathlib import Path

SRC = Path("/work/keith-plugin/resources/samples/percussion-src")
OUT = Path("/work/keith-plugin/resources/samples/percussion")
OUT.mkdir(parents=True, exist_ok=True)

# The CC0 source set. Not committed: 5.4 MB of third-party files we would be redistributing for
# no need, when the provenance record names them and gives every hash. Fetch and verify instead.
SOURCE_SET = "https://sfxmint.com/api/v1/sets/acoustic-drum-kit?format=wav"


def fetch_sources():
    """Download the CC0 set and verify each file against the publisher's own hash.

    Refuses a file whose hash differs. A substituted or truncated download would silently change
    every derived hit, and the point of recording hashes is to catch exactly that.
    """
    meta = json.load(urllib.request.urlopen(urllib.request.Request(
        SOURCE_SET, headers={"User-Agent": "papertrail"}), timeout=30))
    SRC.mkdir(parents=True, exist_ok=True)
    ok = True
    for role, info in meta["sounds"].items():
        url = info["wav_url"]
        published = info["format_metadata"]["wav"]["sha256"]
        data = urllib.request.urlopen(urllib.request.Request(
            url, headers={"User-Agent": "papertrail"}), timeout=60).read()
        actual = hashlib.sha256(data).hexdigest()
        if actual != published:
            print(f"  REFUSED {role}: hash {actual[:16]} does not match the published "
                  f"{published[:16]}")
            ok = False
            continue
        (SRC / f"{info['slug']}.wav").write_bytes(data)
        print(f"  {role:20s} {len(data):>8,} bytes  verified")
    return ok


def read_wav(path):
    with wave.open(str(path)) as w:
        n, ch, sw, sr = w.getnframes(), w.getnchannels(), w.getsampwidth(), w.getframerate()
        raw = w.readframes(n)
    fmt = {1: "b", 2: "h", 4: "i"}[sw]
    vals = struct.unpack(f"<{n * ch}{fmt}", raw)
    full = float(2 ** (8 * sw - 1))
    # mono for percussion: a single hit does not need stereo, and mono halves the memory
    mono = [vals[i] / full for i in range(0, len(vals), ch)]
    return mono, sr


def write_wav(path, samples, sr):
    clipped = [max(-1.0, min(1.0, s)) for s in samples]
    data = struct.pack(f"<{len(clipped)}h", *[int(round(s * 32767)) for s in clipped])
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(sr)
        w.writeframes(data)


def envelope(samples, decay_s, sr, attack_ms=1.0):
    """Apply an exponential decay with a short attack ramp.

    Shortening a hit is what turns a tom into a conga or a block: the pitch stays, the body goes.
    """
    a = max(1, int(sr * attack_ms / 1000.0))
    d = max(1.0, sr * decay_s)
    out = []
    for i, s in enumerate(samples):
        g = min(1.0, i / a) * math.exp(-i / d)
        out.append(s * g)
    return out


def highpass(samples, cutoff, sr):
    """One-pole high-pass. Removes the body, which is what makes a hit read as thin and dry."""
    rc = 1.0 / (2.0 * math.pi * cutoff)
    alpha = rc / (rc + 1.0 / sr)
    out, prev_in, prev_out = [], 0.0, 0.0
    for s in samples:
        y = alpha * (prev_out + s - prev_in)
        out.append(y)
        prev_in, prev_out = s, y
    return out


def lowpass(samples, cutoff, sr):
    rc = 1.0 / (2.0 * math.pi * cutoff)
    alpha = 1.0 / (rc + 1.0 / sr)
    out, prev = [], 0.0
    for s in samples:
        prev = prev + alpha * (s - prev)
        out.append(prev)
    return out


def trim(samples, seconds, sr, threshold=0.0008):
    """Cut to `seconds`, then trim trailing silence so the file is only as long as it sounds."""
    n = min(len(samples), int(sr * seconds))
    out = samples[:n]
    last = 0
    for i, s in enumerate(out):
        if abs(s) > threshold:
            last = i
    return out[:max(1, last + 1)]


def resample_pitch(samples, semitones):
    """Crude but effective: read the buffer at a different rate, which shifts pitch and length.

    For a single percussion hit that is exactly what is wanted -- shorter and higher together is
    how a tom becomes a high block. Linear interpolation avoids the worst of the aliasing.
    """
    if semitones == 0:
        return samples
    ratio = 2.0 ** (semitones / 12.0)
    n_out = max(1, int(len(samples) / ratio))
    out = []
    for i in range(n_out):
        src = i * ratio
        i0 = int(src)
        i1 = min(i0 + 1, len(samples) - 1)
        f = src - i0
        out.append(samples[i0] + (samples[i1] - samples[i0]) * f)
    return out


def normalise(samples, peak_db=-3.0):
    peak = max((abs(s) for s in samples), default=0.0)
    if peak < 1e-9:
        return samples
    target = 10 ** (peak_db / 20.0)
    g = target / peak
    return [s * g for s in samples]


def click(sr, freq, ms, amp=0.25):
    """A short bright transient. The Korg-ROM kits have a hard, produced attack."""
    n = int(sr * ms / 1000.0)
    return [amp * math.exp(-i / (sr * ms / 1000.0 / 4.0)) * math.sin(2 * math.pi * freq * i / sr)
            for i in range(n)]


def mix(a, b):
    n = max(len(a), len(b))
    out = [0.0] * n
    for i in range(n):
        out[i] = (a[i] if i < len(a) else 0.0) + (b[i] if i < len(b) else 0.0)
    return out


def build():
    """The two kits: name -> (source file, recipe)."""
    src = {}
    for f in SRC.glob("*.wav"):
        short = f.stem.replace("acoustic-drum-kit-", "").replace("-01", "")
        src[short] = read_wav(f)

    kits = {}

    # --- NEPTUNES: thin, dry, tight, high-tuned, space around it --------------------------
    # Every hit: high-passed to strip body, very short decay, tuned up. The name of the game is
    # that a percussion layer in this production floats above the arrangement.
    n = {}
    s, sr = src["high-tom"]
    n["neptunes-tamb"] = normalise(trim(highpass(envelope(s, 0.10, sr), 900, sr), 0.18, sr))
    s2, _ = src["closed-hat"]
    n["neptunes-shaker"] = normalise(trim(highpass(envelope(s2, 0.07, sr, 3.0), 2600, sr),
                                           0.14, sr))
    s3, _ = src["snare-cross-stick"]
    n["neptunes-clave"] = normalise(trim(highpass(envelope(
        resample_pitch(s3, 14), 0.04, sr), 1400, sr), 0.10, sr))
    s4, _ = src["mid-tom"]
    n["neptunes-conga-high"] = normalise(trim(highpass(envelope(
        resample_pitch(s4, 9), 0.16, sr), 260, sr), 0.26, sr))
    s5, _ = src["floor-tom"]
    n["neptunes-conga-low"] = normalise(trim(highpass(envelope(
        resample_pitch(s5, 7), 0.22, sr), 180, sr), 0.34, sr))
    s6, _ = src["snare-rimshot"]
    n["neptunes-block"] = normalise(trim(highpass(envelope(
        resample_pitch(s6, 12), 0.05, sr), 1100, sr), 0.12, sr))
    s7, _ = src["ride-bell"]
    n["neptunes-bell-tick"] = normalise(trim(highpass(envelope(s7, 0.09, sr), 3400, sr),
                                             0.16, sr))
    s8, _ = src["pedal-hat"]
    n["neptunes-cabasa"] = normalise(trim(highpass(envelope(s8, 0.06, sr, 6.0), 2200, sr),
                                          0.13, sr))
    s9, _ = src["kick"]
    n["neptunes-tom-thin"] = normalise(trim(highpass(envelope(
        resample_pitch(s9, 10), 0.13, sr), 400, sr), 0.22, sr))
    s10, _ = src["snare"]
    n["neptunes-snap"] = normalise(trim(highpass(envelope(s10, 0.05, sr, 0.5), 1600, sr),
                                        0.11, sr))
    s11, _ = src["open-hat"]
    n["neptunes-rattle"] = normalise(trim(highpass(envelope(s11, 0.12, sr, 4.0), 2000, sr),
                                          0.20, sr))
    s12, _ = src["ride"]
    n["neptunes-ting"] = normalise(trim(highpass(envelope(s12, 0.11, sr), 3000, sr),
                                        0.18, sr))
    s13, _ = src["crash"]
    n["neptunes-sizzle"] = normalise(trim(highpass(envelope(s13, 0.16, sr, 2.0), 2800, sr),
                                          0.26, sr))

    # --- KORG ROM: bright, punchy, slightly synthetic, hard produced attack ---------------
    # Every hit gets a click transient and a short even tail. Less high-pass than the Neptunes
    # kit, so more body survives; the character is punch rather than air.
    k = {}
    s, sr = src["high-tom"]
    k["rom-timbale"] = normalise(trim(mix(envelope(resample_pitch(s, 5), 0.13, sr),
                                          click(sr, 4200, 4, 0.35)), 0.20, sr))
    s2, _ = src["mid-tom"]
    k["rom-conga"] = normalise(trim(mix(envelope(resample_pitch(s2, 3), 0.20, sr),
                                        click(sr, 3600, 5, 0.30)), 0.30, sr))
    s3, _ = src["floor-tom"]
    k["rom-conga-low"] = normalise(trim(mix(envelope(resample_pitch(s3, 1), 0.28, sr),
                                            click(sr, 3000, 6, 0.26)), 0.42, sr))
    s4, _ = src["snare-cross-stick"]
    k["rom-clave"] = normalise(trim(mix(envelope(resample_pitch(s4, 16), 0.05, sr),
                                        click(sr, 5200, 3, 0.40)), 0.12, sr))
    s5, _ = src["snare-rimshot"]
    k["rom-block"] = normalise(trim(mix(envelope(resample_pitch(s5, 11), 0.06, sr),
                                        click(sr, 4600, 3, 0.38)), 0.14, sr))
    s6, _ = src["closed-hat"]
    k["rom-shaker"] = normalise(trim(mix(envelope(s6, 0.08, sr, 2.0),
                                         click(sr, 6400, 2, 0.30)), 0.15, sr))
    s7, _ = src["pedal-hat"]
    k["rom-cabasa"] = normalise(trim(mix(envelope(s7, 0.07, sr, 4.0),
                                         click(sr, 5600, 2, 0.32)), 0.14, sr))
    s8, _ = src["ride-bell"]
    k["rom-bell"] = normalise(trim(mix(envelope(resample_pitch(s8, -2), 0.30, sr),
                                       click(sr, 6800, 4, 0.34)), 0.48, sr))
    s9, _ = src["snare"]
    k["rom-snap"] = normalise(trim(mix(envelope(s9, 0.07, sr, 0.5),
                                       click(sr, 5000, 3, 0.36)), 0.15, sr))
    s10, _ = src["open-hat"]
    k["rom-rattle"] = normalise(trim(mix(envelope(s10, 0.14, sr, 3.0),
                                         click(sr, 5800, 3, 0.30)), 0.24, sr))
    s11, _ = src["ride"]
    k["rom-ting"] = normalise(trim(mix(envelope(s11, 0.16, sr),
                                       click(sr, 7200, 3, 0.32)), 0.26, sr))
    s12, _ = src["crash"]
    k["rom-sizzle"] = normalise(trim(mix(envelope(s12, 0.20, sr, 1.5),
                                         click(sr, 6000, 3, 0.28)), 0.34, sr))
    s13, _ = src["kick"]
    k["rom-tom-high"] = normalise(trim(mix(envelope(resample_pitch(s13, 12), 0.11, sr),
                                           click(sr, 4400, 4, 0.34)), 0.18, sr))

    kits["percussion-neptunes"] = n
    kits["percussion-rom"] = k
    return kits, sr


def main():
    if "--fetch" in sys.argv:
        print("fetching and verifying the CC0 source set:")
        if not fetch_sources():
            print()
            print("one or more files failed verification -- refusing to continue")
            return 1
        print()

    if not SRC.exists() or not any(SRC.glob("*.wav")):
        print(f"no sources in {SRC}")
        print("run with --fetch to download and verify them from the CC0 set")
        return 1

    kits, sr = build()
    manifest = []
    for kit_name, hits in kits.items():
        d = OUT / kit_name
        d.mkdir(parents=True, exist_ok=True)
        for name, samples in hits.items():
            p = d / f"{name}.wav"
            write_wav(p, samples, sr)
            manifest.append((kit_name, name, p.name, len(samples), len(samples) / sr))
        print(f"{kit_name}: {len(hits)} hits")

    print()
    print(f"{'kit':22s} {'hit':22s} {'frames':>7} {'sec':>6}")
    for kit, name, fn, n, sec in manifest[:8]:
        print(f"{kit:22s} {name:22s} {n:7d} {sec:6.2f}")
    print(f"... {len(manifest)} files total")
    print()
    total = sum(f.stat().st_size for f in OUT.rglob("*.wav"))
    print(f"total size: {total/1024:.0f} KB")
    Path("/tmp/perc-manifest.json").write_text(json.dumps(manifest, indent=2))


if __name__ == "__main__":
    main()
