"""Tests for transient detection and slice editing.

    /work/.venv/bin/python /work/tools/slicer_check.py

Mirrors src/sampler/SampleSlicer.h. This box has no C++ compiler, so the detection logic is
ported here and actually run rather than only written.

The cases below are the ones that found real bugs, kept as regressions:

  - A steady tone must not produce onsets. The first implementation reported 40 of them,
    because a fixed window is not an integer number of cycles and the per-window mean rippled.
    Fixed by using a one-pole envelope follower, which has no window to misalign.

  - A quiet hit after a loud one must still be found. The first fix used a threshold relative
    to the loudest transient, and an 8%-amplitude hit could never clear it. Then a purely local
    reference produced 35 false onsets on a flat tone, because near-zero flux compared against
    near-zero local level is noise. The working rule needs BOTH an absolute floor and a local
    reference.

  - A hit that begins after silence is two slices, not one: the silence is its own region. The
    first version of this test asserted one and was wrong, not the code.
"""

import math
import sys

SAMPLE_RATE = 44100


# --- port of the C++ ---------------------------------------------------------------------

def follow_envelope(sig, sr, attack_ms, release_ms):
    at = math.exp(-1.0 / max(1.0, sr * attack_ms / 1000.0))
    rt = math.exp(-1.0 / max(1.0, sr * release_ms / 1000.0))
    env = [0.0] * len(sig)
    prev = 0.0
    for i, v in enumerate(sig):
        x = abs(v)
        prev = x + (at if x > prev else rt) * (prev - x)
        env[i] = prev
    return env


def detect(sig, sr, sensitivity=0.5, min_spacing_ms=40.0, hop_ms=5.0,
           attack_ms=1.0, release_ms=20.0, local_ms=250.0):
    env = follow_envelope(sig, sr, attack_ms, release_ms)
    hop = max(1, int(sr * hop_ms / 1000.0))
    blocks = len(env) // hop
    if blocks < 3:
        return [0]

    level = [sum(env[b * hop:(b + 1) * hop]) / hop for b in range(blocks)]

    flux = [0.0] * blocks
    for b in range(1, blocks):
        flux[b] = max(0.0, level[b] - level[b - 1])

    peak_flux = max(flux) if flux else 0.0
    if peak_flux <= 0.0:
        return [0]

    win = max(1, int(sr * local_ms / 1000.0 / hop))
    local = [0.0] * blocks
    run = 0.0
    for b in range(blocks):
        run += flux[b]
        if b >= win:
            run -= flux[b - win]
        local[b] = run / min(b + 1, win)

    s = min(1.0, max(0.0, sensitivity))
    relative = 2.4 - 1.7 * s
    floor_level = peak_flux * 0.06

    min_spacing = max(1, int(sr * min_spacing_ms / 1000.0))
    onsets = [0]
    last = 0
    for b in range(1, blocks):
        f = flux[b]
        if f < floor_level:
            continue
        if f <= max(local[b] * relative, floor_level):
            continue
        nxt = flux[b + 1] if b + 1 < blocks else 0.0
        if f < nxt:
            continue
        frame = b * hop
        if frame - last < min_spacing:
            continue
        onsets.append(frame)
        last = frame
    return onsets


# --- signal builders ---------------------------------------------------------------------

def percussive(spec, dur=2.0, decay=0.06, freq=200.0):
    """spec: [(seconds, amplitude)]. Sharp attack, exponential decay."""
    n = int(SAMPLE_RATE * dur)
    buf = [0.0] * n
    for t0, amp in spec:
        i0 = int(t0 * SAMPLE_RATE)
        for i in range(i0, min(n, i0 + int(SAMPLE_RATE * 0.3))):
            t = (i - i0) / SAMPLE_RATE
            buf[i] += amp * math.exp(-t / decay) * math.sin(2 * math.pi * freq * t)
    return buf


def tone(freq, dur=2.0, amp=0.5):
    return [amp * math.sin(2 * math.pi * freq * i / SAMPLE_RATE)
            for i in range(int(SAMPLE_RATE * dur))]


# --- checks ------------------------------------------------------------------------------

def main():
    failures = 0

    def check(name, sig, want, tol_positions=None, expect=None):
        nonlocal failures
        got = detect(sig, SAMPLE_RATE)
        positions = [round(x / SAMPLE_RATE, 3) for x in got]
        ok = len(got) == want
        if ok and expect is not None:
            ok = all(abs(a - b) < 0.05 for a, b in zip(positions, expect))
        print(f"  {'ok  ' if ok else 'FAIL'} {name:26s} {len(got):2d} slice(s)"
              + ("" if ok else f"  want {want}"))
        if not ok:
            print(f"        got {positions}")
            failures += 1

    print("steady material must not produce onsets")
    check("steady 220 Hz tone", tone(220), 1)
    check("steady 100 Hz tone", tone(100), 1)
    check("steady 440 Hz tone", tone(440), 1)
    check("silence", [0.0] * SAMPLE_RATE, 1)

    print()
    print("percussive material must segment correctly")
    check("four on the floor", percussive([(0.0, 1.0), (0.5, 1.0), (1.0, 1.0), (1.5, 1.0)]),
          4, expect=[0.0, 0.5, 1.0, 1.5])
    check("uneven spacing",
          percussive([(0.0, 1.0), (0.13, 1.0), (0.5, 1.0), (0.55, 1.0), (1.4, 1.0)]),
          5, expect=[0.0, 0.13, 0.5, 0.55, 1.4])

    print()
    print("hits closer than the minimum spacing merge")
    check("20 ms pair", percussive([(0.0, 1.0), (0.02, 1.0), (0.5, 1.0)]), 2,
          expect=[0.0, 0.5])

    print()
    print("a quiet hit after a loud one is still found")
    check("quiet 8% second hit", percussive([(0.0, 1.0), (0.7, 0.08)]), 2,
          expect=[0.0, 0.7])
    check("quiet 25% second hit", percussive([(0.0, 1.0), (0.7, 0.25)]), 2,
          expect=[0.0, 0.7])

    print()
    print("a hit after silence is two slices: the silence is its own region")
    check("single hit at 0.1 s", percussive([(0.1, 1.0)]), 2, expect=[0.0, 0.1])

    print()
    print("sensitivity moves the bar")
    sig = percussive([(0.0, 1.0), (0.7, 0.02)])
    low = len(detect(sig, SAMPLE_RATE, sensitivity=0.1))
    high = len(detect(sig, SAMPLE_RATE, sensitivity=0.95))
    check_ok = high >= low
    print(f"  {'ok  ' if check_ok else 'FAIL'} higher sensitivity finds at least as many "
          f"({low} at 0.1, {high} at 0.95)")
    if not check_ok:
        failures += 1

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
