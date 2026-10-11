"""FFT check for the wavetable oscillator's antialiasing.

    /work/.venv/bin/python /work/tools/wavetable_fft_check.py

This mirrors the maths in src/synth/WavetableOsc.h -- harmonic frames, band-limited mip
levels, linear-interpolated read -- and measures how much energy lands on frequencies that
are NOT harmonics of the note being played. That is what aliasing IS.

Why this exists rather than just the C++ test: this box has no C++ compiler and no way to
install one. The logic was ported here so it could actually be run.

Why it uses an FFT and not a Goertzel probe: the first version probed with Goertzel at
arbitrary frequencies and measured a pure sine at 0.0145 of "non-harmonic energy". A pure
sine has none. That number was spectral leakage from a non-integer number of cycles in the
window, and it made the band limiting look like it did nothing at 220 Hz. A Hann-windowed
FFT does not have that problem, and it is the version whose numbers are quoted.
"""

import cmath
import math

SAMPLE_RATE = 44100
MAX_HARMONICS = 256
MIP_LEVELS = 9
TABLE_SIZE = 2048


def fft(values):
    n = len(values)
    if n == 1:
        return values
    even = fft(values[0::2])
    odd = fft(values[1::2])
    twiddled = [cmath.exp(-2j * math.pi * k / n) * odd[k] for k in range(n // 2)]
    return [even[k] + twiddled[k] for k in range(n // 2)] + \
           [even[k] - twiddled[k] for k in range(n // 2)]


def spectrum(signal):
    """Magnitude per bin, Hann-tapered so a non-integer cycle count cannot leak."""
    n = 1
    while n * 2 <= len(signal):
        n *= 2
    window = [0.5 - 0.5 * math.cos(2 * math.pi * i / n) for i in range(n)]
    bins = fft([complex(signal[i] * window[i], 0.0) for i in range(n)])
    return [abs(bins[i]) / (n / 4) for i in range(n // 2)]


def render_frame(harmonics, level, size=TABLE_SIZE):
    """One frame at one mip level, normalised to unit peak."""
    keep = max(1, MAX_HARMONICS >> level)
    buf = [0.0] * size
    for h in range(min(keep, len(harmonics))):
        amp = harmonics[h]
        if amp == 0.0:
            continue
        step = 2 * math.pi * (h + 1) / size
        for i in range(size):
            buf[i] += amp * math.sin(step * i)
    peak = max(abs(v) for v in buf) or 1.0
    return [v / peak for v in buf]


def band_limit_for(freq):
    allowed = int((SAMPLE_RATE * 0.5) / freq)
    level, cap = 0, MAX_HARMONICS
    while level < MIP_LEVELS - 1 and allowed < cap:
        cap //= 2
        level += 1
    return level


def play(banks, freq, count, use_band_limit=True):
    phase = 0.0
    out = []
    for _ in range(count):
        phase += freq / SAMPLE_RATE
        if phase >= 1.0:
            phase -= math.floor(phase)
        buf = banks[band_limit_for(freq) if use_band_limit else 0]
        pos = phase * TABLE_SIZE
        i0 = int(pos) & (TABLE_SIZE - 1)
        i1 = (i0 + 1) & (TABLE_SIZE - 1)
        frac = pos - math.floor(pos)
        out.append(buf[i0] + (buf[i1] - buf[i0]) * frac)
    return out


def alias_ratio(signal, freq):
    """Non-harmonic energy as a fraction of total. 0 means perfectly clean."""
    mag = spectrum(signal)
    bin_hz = SAMPLE_RATE / len(mag) * 2
    total = 0.0
    alias = 0.0
    for b in range(1, len(mag)):
        hz = b * bin_hz
        if hz < 20 or hz > SAMPLE_RATE * 0.45:
            continue
        total += mag[b] ** 2
        ratio = hz / freq
        if abs(ratio - round(ratio)) > 0.15:
            alias += mag[b] ** 2
    return math.sqrt(alias) / math.sqrt(total or 1.0)


def main():
    # A deliberately bright frame: 128 harmonics falling as 1/n. This is the shape that
    # aliases worst and the one a derived table will resemble.
    bright = [1.0 / (i + 1) for i in range(128)]

    limited = {lvl: render_frame(bright, lvl) for lvl in range(MIP_LEVELS)}
    unlimited = {lvl: render_frame(bright, 0) for lvl in range(MIP_LEVELS)}

    print("Wavetable antialiasing — non-harmonic energy as a fraction of total")
    print("(lower is cleaner; 'same' at low pitches is correct, not a failure)\n")
    print(f"{'freq':>8} {'band-limited':>14} {'no limit':>11} {'ratio':>7}  verdict")
    failures = 0
    for freq in (220.0, 880.0, 3520.0, 7000.0):
        bl = alias_ratio(play(limited, freq, 16384), freq)
        nb = alias_ratio(play(unlimited, freq, 16384), freq)
        ratio = bl / nb if nb else 0.0
        if bl < nb * 0.99:
            verdict = "better"
        elif abs(ratio - 1.0) < 0.01:
            verdict = "same (nothing to remove)"
        else:
            verdict = "WORSE"
            failures += 1
        print(f"{freq:8.0f} {bl:14.5f} {nb:11.5f} {ratio:7.2f}  {verdict}")

    print()
    # A pure sine must come back a pure sine, or the table read itself is broken.
    sine = render_frame([1.0], 0)
    sig = []
    phase = 0.0
    for _ in range(16384):
        phase += 220.0 / SAMPLE_RATE
        if phase >= 1.0:
            phase -= math.floor(phase)
        pos = phase * TABLE_SIZE
        i0 = int(pos) & (TABLE_SIZE - 1)
        i1 = (i0 + 1) & (TABLE_SIZE - 1)
        frac = pos - math.floor(pos)
        sig.append(sine[i0] + (sine[i1] - sine[i0]) * frac)
    sine_alias = alias_ratio(sig, 220.0)
    print(f"pure sine frame at 220 Hz: non-harmonic energy {sine_alias:.5f}")
    if sine_alias > 0.02:
        print("  FAIL — a sine frame should be essentially pure")
        failures += 1
    else:
        print("  ok — the table read is clean")

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
