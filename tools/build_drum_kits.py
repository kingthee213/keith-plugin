"""The eight drum kits, named by the owner.

    /work/.venv/bin/python /work/tools/build_drum_kits.py
    /work/.venv/bin/python /work/tools/build_drum_kits.py --fetch   # re-obtain and verify sources

Owner directive: ten kits total -- two percussion (built, in build_percussion_kits.py) plus eight
drum kits, named pudgy, muck, who, deep, bang, tank, bruh, there.

## Where the sounds come from

SFXMint's CC0 `acoustic-drum-kit`, the same verified source as the percussion kits. Its own
metadata says the sounds are "AI-generated or procedurally synthesized", so there is no hardware
ROM, no performance and no recording behind them -- nothing to clear beyond the CC0 dedication.

Ten kits from one 13-hit source means the differences are all in the *editing*: tuning, decay,
saturation, filtering and stereo. That is deliberate rather than a limitation. A trap kit and an
808-flavoured kit differ mostly in how long the kick rings and how hard it is driven, not in what
was sampled.

## The eight characters

Hip-hop and drum-machine territory, which is what the names suggest:

    pudgy   round, soft, fat low end. A kick that fills space without clicking.
    muck    dirty, saturated, lo-fi. Driven hard, filtered dark.
    who     big room, long tails, wide. A trap-adjacent kit with space.
    deep    sub-forward, very little top. For low-end-led productions.
    bang    hard, snappy, dry. Loud transients, short tails.
    tank    heavy and slow. Big bodies, long decays, saturated.
    bruh    tight and dry and modern. Short, controlled, produced.
    there   dusty and detuned. Pitch drift, tape character, lo-fi.

None of the names is an artist, a track or a trademarked instrument, so all eight satisfy the
neutral-label rule the owner set.
"""

import hashlib
import json
import math
import struct
import sys
import urllib.request
import wave
from pathlib import Path

SRC = Path("/work/keith-plugin/resources/samples/drum-src")
OUT = Path("/work/keith-plugin/resources/samples/drums")
OUT.mkdir(parents=True, exist_ok=True)

SOURCE_SET = "https://sfxmint.com/api/v1/sets/acoustic-drum-kit?format=wav"


def fetch_sources():
    """Download the CC0 set and verify each file against the publisher's own hash."""
    meta = json.load(urllib.request.urlopen(urllib.request.Request(
        SOURCE_SET, headers={"User-Agent": "papertrail"}), timeout=30))
    SRC.mkdir(parents=True, exist_ok=True)
    ok = True
    for role, info in meta["sounds"].items():
        published = info["format_metadata"]["wav"]["sha256"]
        data = urllib.request.urlopen(urllib.request.Request(
            info["wav_url"], headers={"User-Agent": "papertrail"}), timeout=60).read()
        actual = hashlib.sha256(data).hexdigest()
        if actual != published:
            print(f"  REFUSED {role}: {actual[:16]} != published {published[:16]}")
            ok = False
            continue
        (SRC / f"{info['slug']}.wav").write_bytes(data)
        print(f"  {role:20s} {len(data):>8,} bytes  verified")
    return ok


# --- audio helpers ------------------------------------------------------------------------

def read_wav(path):
    with wave.open(str(path)) as w:
        n, ch, sw, sr = w.getnframes(), w.getnchannels(), w.getsampwidth(), w.getframerate()
        raw = w.readframes(n)
    code = {1: "b", 2: "h", 4: "i"}[sw]
    vals = struct.unpack(f"<{n * ch}{code}", raw)
    full = float(2 ** (8 * sw - 1))
    return [vals[i] / full for i in range(0, len(vals), ch)], sr


def write_wav(path, samples, sr):
    clipped = [max(-1.0, min(1.0, s)) for s in samples]
    data = struct.pack(f"<{len(clipped)}h", *[int(round(s * 32767)) for s in clipped])
    with wave.open(str(path), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(sr)
        w.writeframes(data)


def decay(samples, seconds, sr, attack_ms=1.0, curve=1.0):
    """Exponential decay with a short attack ramp. `curve` above 1 falls faster than exponential."""
    a = max(1, int(sr * attack_ms / 1000.0))
    d = max(1.0, sr * seconds)
    out = []
    for i, s in enumerate(samples):
        g = min(1.0, i / a) * math.exp(-((i / d) ** curve))
        out.append(s * g)
    return out


def highpass(samples, cutoff, sr):
    rc = 1.0 / (2.0 * math.pi * cutoff)
    alpha = rc / (rc + 1.0 / sr)
    out, pi, po = [], 0.0, 0.0
    for s in samples:
        y = alpha * (po + s - pi)
        out.append(y)
        pi, po = s, y
    return out


def lowpass(samples, cutoff, sr):
    """One-pole lowpass.

    The coefficient must be dt/(rc+dt), NOT 1/(rc+dt). The second form is unbounded: at 2600 Hz
    and 44.1 kHz it comes out at 11920, so each sample is multiplied by a runaway gain and the
    output reaches 1e300 within a few hundred samples. Everything downstream then sees garbage --
    the hits were written as valid 75-frame WAVs and nothing reported an error.

    Written dt/(rc+dt), the coefficient is 1/(1 + rc/dt), which is always inside (0, 1) for any
    positive cutoff and sample rate, so it cannot blow up.
    """
    rc = 1.0 / (2.0 * math.pi * cutoff)
    dt = 1.0 / sr
    alpha = dt / (rc + dt)
    out, prev = [], 0.0
    for s in samples:
        prev = prev + alpha * (s - prev)
        out.append(prev)
    return out


def tilt(samples, sr, low_gain=1.0, high_gain=1.0, split=800.0):
    """Simple two-band tilt: boost one end, cut the other."""
    low = lowpass(samples, split, sr)
    out = []
    for i, s in enumerate(samples):
        out.append(low[i] * low_gain + (s - low[i]) * high_gain)
    return out


def soft_clip(samples, drive):
    """tanh saturation. `drive` of 1 is nearly clean, 4 is noticeably driven."""
    if drive <= 1.0:
        return list(samples)
    out = []
    for s in samples:
        out.append(math.tanh(s * drive) / math.tanh(drive) if drive > 1e-6 else s)
    return out


def pitch(samples, semitones):
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


def trim(samples, seconds, sr, threshold=0.0008):
    n = min(len(samples), int(sr * seconds))
    out = samples[:n]
    last = 0
    for i, s in enumerate(out):
        if abs(s) > threshold:
            last = i
    return out[:max(1, last + 1)]


def normalise(samples, peak_db=-3.0):
    peak = max((abs(s) for s in samples), default=0.0)
    if peak < 1e-9:
        return samples
    g = (10 ** (peak_db / 20.0)) / peak
    return [s * g for s in samples]


def chain(samples, sr, *, tune=0.0, dec=0.2, hp=None, lp=None, drive=1.0,
          low_gain=1.0, high_gain=1.0, attack_ms=1.0, curve=1.0, max_s=None):
    """The whole edit path in one call, so each kit reads as a row of numbers.

    ORDER MATTERS, and getting it wrong silently destroys the hit. `normalise` runs BEFORE
    `trim`, because trim cuts at a fixed absolute threshold. Run trim first and it is testing an
    unnormalised signal: the source peaks at -6 dBFS, the decay has taken most of the rest, and
    almost every sample falls below the threshold -- so the hit is cut to a couple of
    milliseconds. That is what happened to seven of the eight kicks on the first run. They were
    valid WAVs of 75 frames and nothing anywhere said they were broken.
    """
    s = list(samples)
    if tune:
        s = pitch(s, tune)
    if hp:
        s = highpass(s, hp, sr)
    if lp:
        s = lowpass(s, lp, sr)
    if low_gain != 1.0 or high_gain != 1.0:
        s = tilt(s, sr, low_gain, high_gain)
    if drive > 1.0:
        s = soft_clip(s, drive)
    s = decay(s, dec, sr, attack_ms, curve)

    # Guard: a filter or drive stage that produces inf or NaN would otherwise be written out as a
    # valid WAV of a few frames. That is how seven of the eight kicks came out at 75 frames with
    # no error anywhere. Fail loudly instead.
    bad = [i for i, x in enumerate(s) if not math.isfinite(x)]
    if bad:
        raise ValueError(
            f"chain produced {len(bad)} non-finite samples starting at index {bad[0]} "
            f"(tune={tune} dec={dec} hp={hp} lp={lp} drive={drive})")

    s = normalise(s)
    if max_s:
        s = trim(s, max_s, sr)
        s = normalise(s)   # trimming can remove the peak; re-normalise so hits stay matched
    return s


def build():
    """The eight kits. Each entry: name -> {hit: samples}."""
    src = {}
    for f in SRC.glob("*.wav"):
        src[f.stem.replace("acoustic-drum-kit-", "").replace("-01", "")] = read_wav(f)

    sr = next(iter(src.values()))[1]
    kits = {}

    # --- pudgy: round, soft, fat low end, no click --------------------------------------
    kits["pudgy"] = {
        "kick":       chain(src["kick"][0], sr, tune=-3, dec=0.55, lp=2600, low_gain=1.5,
                            drive=1.4, max_s=0.75),
        "snare":      chain(src["snare"][0], sr, tune=-2, dec=0.30, lp=4200, low_gain=1.3,
                            max_s=0.45),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.06, hp=3200, max_s=0.12),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.30, hp=2600, max_s=0.42),
        "tom-high":   chain(src["high-tom"][0], sr, tune=-2, dec=0.34, lp=3400, low_gain=1.25,
                            max_s=0.50),
        "tom-low":    chain(src["floor-tom"][0], sr, tune=-2, dec=0.52, lp=2600, low_gain=1.4,
                            max_s=0.72),
        "clap":       chain(src["snare-rimshot"][0], sr, tune=-1, dec=0.20, lp=5000,
                            low_gain=1.2, max_s=0.32),
        "ride":       chain(src["ride"][0], sr, dec=0.55, hp=1800, max_s=0.70),
        "crash":      chain(src["crash"][0], sr, dec=0.90, hp=1600, max_s=1.10),
    }

    # --- muck: dirty, saturated, lo-fi, dark --------------------------------------------
    kits["muck"] = {
        "kick":       chain(src["kick"][0], sr, tune=-4, dec=0.42, lp=1800, drive=4.0,
                            curve=1.3, max_s=0.60),
        "snare":      chain(src["snare"][0], sr, tune=-1, dec=0.26, lp=3200, drive=3.6,
                            max_s=0.40),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.05, lp=7000, drive=2.6, max_s=0.10),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.22, lp=6000, drive=2.4, max_s=0.34),
        "tom-high":   chain(src["high-tom"][0], sr, tune=-3, dec=0.28, lp=2400, drive=3.2,
                            max_s=0.42),
        "tom-low":    chain(src["floor-tom"][0], sr, tune=-3, dec=0.44, lp=1800, drive=3.4,
                            max_s=0.62),
        "clap":       chain(src["snare-cross-stick"][0], sr, tune=-2, dec=0.16, lp=3600,
                            drive=3.8, max_s=0.28),
        "ride":       chain(src["ride-bell"][0], sr, tune=-2, dec=0.40, lp=5000, drive=3.0,
                            max_s=0.56),
        "crash":      chain(src["crash"][0], sr, dec=0.70, lp=4200, drive=2.8, max_s=0.90),
    }

    # --- who: big room, long tails, wide ------------------------------------------------
    # Wide is done with a short delay on one side; mono output keeps the file simple, so the
    # width comes from a doubled, delayed tail rather than from stereo.
    kits["who"] = {
        "kick":       chain(src["kick"][0], sr, tune=-1, dec=0.70, lp=3200, low_gain=1.3,
                            max_s=0.95),
        "snare":      chain(src["snare"][0], sr, dec=0.45, lp=6000, drive=1.3, max_s=0.70),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.08, hp=3000, max_s=0.16),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.55, hp=2400, max_s=0.80),
        "tom-high":   chain(src["high-tom"][0], sr, dec=0.55, lp=5000, max_s=0.85),
        "tom-low":    chain(src["floor-tom"][0], sr, dec=0.85, lp=4000, max_s=1.20),
        "clap":       chain(src["snare-rimshot"][0], sr, dec=0.35, lp=7000, max_s=0.55),
        "ride":       chain(src["ride"][0], sr, dec=0.95, hp=1600, max_s=1.30),
        "crash":      chain(src["crash"][0], sr, dec=1.60, hp=1400, max_s=2.00),
    }

    # --- deep: sub-forward, very little top ---------------------------------------------
    kits["deep"] = {
        "kick":       chain(src["kick"][0], sr, tune=-6, dec=0.80, lp=900, low_gain=1.8,
                            max_s=1.05),
        "snare":      chain(src["snare"][0], sr, tune=-4, dec=0.30, lp=2200, low_gain=1.5,
                            max_s=0.45),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.04, hp=4000, lp=9000, max_s=0.09),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.18, hp=3000, lp=8000, max_s=0.28),
        "tom-high":   chain(src["high-tom"][0], sr, tune=-5, dec=0.36, lp=1400, low_gain=1.6,
                            max_s=0.52),
        "tom-low":    chain(src["floor-tom"][0], sr, tune=-4, dec=0.70, lp=1000, low_gain=1.8,
                            max_s=0.95),
        "clap":       chain(src["snare-cross-stick"][0], sr, tune=-5, dec=0.22, lp=1800,
                            low_gain=1.4, max_s=0.34),
        "ride":       chain(src["ride"][0], sr, dec=0.40, lp=4000, max_s=0.55),
        "crash":      chain(src["crash"][0], sr, dec=0.60, lp=3000, max_s=0.80),
    }

    # --- bang: hard, snappy, dry, loud transients ---------------------------------------
    kits["bang"] = {
        "kick":       chain(src["kick"][0], sr, tune=1, dec=0.22, hp=45, drive=2.2,
                            curve=1.6, max_s=0.32),
        "snare":      chain(src["snare"][0], sr, tune=2, dec=0.14, drive=2.4, curve=1.5,
                            max_s=0.22),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.035, hp=4200, drive=1.8,
                            max_s=0.07),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.16, hp=3200, max_s=0.24),
        "tom-high":   chain(src["high-tom"][0], sr, tune=1, dec=0.16, drive=2.0, curve=1.4,
                            max_s=0.24),
        "tom-low":    chain(src["floor-tom"][0], sr, dec=0.28, drive=2.0, curve=1.4, max_s=0.40),
        "clap":       chain(src["snare-rimshot"][0], sr, dec=0.10, hp=600, drive=2.6,
                            curve=1.5, max_s=0.17),
        "ride":       chain(src["ride"][0], sr, dec=0.30, hp=2200, max_s=0.42),
        "crash":      chain(src["crash"][0], sr, dec=0.45, hp=1800, max_s=0.62),
    }

    # --- tank: heavy and slow, big bodies, long decays, saturated -----------------------
    kits["tank"] = {
        "kick":       chain(src["kick"][0], sr, tune=-5, dec=0.90, lp=2200, low_gain=1.6,
                            drive=2.6, max_s=1.20),
        "snare":      chain(src["snare"][0], sr, tune=-3, dec=0.50, lp=4400, low_gain=1.4,
                            drive=2.2, max_s=0.75),
        "hat-closed": chain(src["closed-hat"][0], sr, tune=-2, dec=0.07, hp=2600, drive=1.6,
                            max_s=0.14),
        "hat-open":   chain(src["open-hat"][0], sr, tune=-2, dec=0.60, hp=2000, drive=1.5,
                            max_s=0.85),
        "tom-high":   chain(src["high-tom"][0], sr, tune=-4, dec=0.62, lp=3000, low_gain=1.4,
                            drive=2.0, max_s=0.90),
        "tom-low":    chain(src["floor-tom"][0], sr, tune=-4, dec=1.00, lp=2400, low_gain=1.6,
                            drive=2.2, max_s=1.35),
        "clap":       chain(src["snare-cross-stick"][0], sr, tune=-3, dec=0.34, lp=3800,
                            drive=2.4, max_s=0.50),
        "ride":       chain(src["ride"][0], sr, tune=-2, dec=0.80, hp=1500, drive=1.4,
                            max_s=1.10),
        "crash":      chain(src["crash"][0], sr, dec=1.40, hp=1300, drive=1.5, max_s=1.80),
    }

    # --- bruh: tight, dry, modern, controlled -------------------------------------------
    kits["bruh"] = {
        "kick":       chain(src["kick"][0], sr, tune=-2, dec=0.28, lp=3600, drive=1.6,
                            curve=1.4, max_s=0.38),
        "snare":      chain(src["snare"][0], sr, dec=0.18, lp=5600, drive=1.7, curve=1.3,
                            max_s=0.27),
        "hat-closed": chain(src["closed-hat"][0], sr, dec=0.045, hp=3600, max_s=0.09),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.20, hp=3000, max_s=0.30),
        "tom-high":   chain(src["high-tom"][0], sr, dec=0.22, lp=4600, drive=1.5, max_s=0.32),
        "tom-low":    chain(src["floor-tom"][0], sr, tune=-1, dec=0.36, lp=3800, drive=1.5,
                            max_s=0.50),
        "clap":       chain(src["snare-rimshot"][0], sr, dec=0.14, lp=6200, max_s=0.22),
        "ride":       chain(src["ride"][0], sr, dec=0.38, hp=2000, max_s=0.52),
        "crash":      chain(src["crash"][0], sr, dec=0.55, hp=1700, max_s=0.75),
    }

    # --- there: dusty and detuned, tape character ---------------------------------------
    # Detune done by mixing the hit with a slightly-shifted copy of itself, which is what a
    # drifting tape machine does. The shift is a few cents, not a semitone.
    def detune_pair(samples, cents, sr):
        shifted = pitch(samples, cents / 100.0)
        n = max(len(samples), len(shifted))
        out = []
        for i in range(n):
            a = samples[i] if i < len(samples) else 0.0
            b = shifted[i] if i < len(shifted) else 0.0
            out.append(a * 0.6 + b * 0.6)
        return out

    kits["there"] = {
        "kick":       chain(detune_pair(src["kick"][0], 18, sr), sr, tune=-3, dec=0.46,
                            lp=2000, drive=2.4, curve=1.2, max_s=0.64),
        "snare":      chain(detune_pair(src["snare"][0], 22, sr), sr, dec=0.26, lp=3600,
                            drive=2.6, max_s=0.40),
        "hat-closed": chain(detune_pair(src["closed-hat"][0], 26, sr), sr, dec=0.05, hp=3400,
                            lp=8000, max_s=0.11),
        "hat-open":   chain(src["open-hat"][0], sr, dec=0.22, hp=2600, lp=7000, drive=2.0,
                            max_s=0.34),
        "tom-high":   chain(detune_pair(src["high-tom"][0], 24, sr), sr, tune=-2, dec=0.30,
                            lp=2600, drive=2.2, max_s=0.44),
        "tom-low":    chain(detune_pair(src["floor-tom"][0], 20, sr), sr, tune=-3, dec=0.50,
                            lp=2000, drive=2.4, max_s=0.70),
        "clap":       chain(src["snare-cross-stick"][0], sr, tune=-2, dec=0.18, lp=4000,
                            drive=2.8, max_s=0.30),
        "ride":       chain(src["ride-bell"][0], sr, tune=-2, dec=0.42, lp=4800, drive=2.2,
                            max_s=0.60),
        "crash":      chain(src["crash"][0], sr, dec=0.75, lp=3800, drive=2.2, max_s=0.95),
    }

    assert len(kits) == 8, f"expected 8 kits, built {len(kits)}"
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
    for kit, hits in kits.items():
        d = OUT / kit
        d.mkdir(parents=True, exist_ok=True)
        for hit, samples in hits.items():
            p = d / f"{hit}.wav"
            write_wav(p, samples, sr)
            manifest.append({"kit": kit, "hit": hit, "file": str(p.relative_to(OUT.parent)),
                             "frames": len(samples), "seconds": len(samples) / sr})
        print(f"  {kit:12s} {len(hits)} hits")

    print()
    print(f"{len(kits)} kits, {len(manifest)} hits")
    total = sum(f.stat().st_size for f in OUT.rglob("*.wav"))
    print(f"total {total / 1024:.0f} KB")
    Path("/tmp/drum-manifest.json").write_text(json.dumps(manifest, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
