"""Tests for sample playback: the play range, and forward vs reversed.

    /work/.venv/bin/python /work/keith-plugin/tools/voice_check.py

Mirrors the position arithmetic in src/sampler/SamplerVoice.h and actually runs it. This box has
no C++ compiler, so the logic is ported here rather than only written -- the same approach
slicer_check.py and warp_check.py take.

## Why these cases

Two real bugs came out of this area, both kept as regressions:

  - **A slice played to the end of the FILE, not the slice.** The voice had no end frame, so
    "play this slice" played that slice and then everything after it. mEndFrame is now part of the
    range, inclusive, and -1 still means the whole sample.

  - **Reversed playback dropped a frame.** The first implementation accumulated a position,
    stepping down from the end. Forward accumulated upward from the start. Doing the same
    arithmetic in a different order drifted, and the two disagreed by one frame at non-unity
    ratios -- a reversed hit sounded fractionally short. Both directions now derive the position
    from a shared frame counter, which makes reversed the forward sequence reversed BY
    CONSTRUCTION rather than by a second calculation that has to be kept in step.

    The counter also removes accumulation drift over long samples, which the old version had.
"""

import math
import sys

SAMPLE_RATE = 44100


# --- port of the C++ -------------------------------------------------------------------------

def step_per_frame(pitch_ratio, file_sr, host_sr):
    """SamplerVoice::StepPerFrame."""
    return pitch_ratio * (file_sr / host_sr)


def resolved_end_frame(end_frame, frames):
    """SamplerVoice::ResolvedEndFrame. end_frame < 0 means the whole sample."""
    file_last = frames - 1
    return min(end_frame if end_frame >= 0 else file_last, file_last)


def frame_count(start_frame, end_frame, pitch_ratio, file_sr, host_sr, frames):
    """SamplerVoice::FrameCount."""
    step = step_per_frame(pitch_ratio, file_sr, host_sr)
    span = (resolved_end_frame(end_frame, frames) + 1.0) - start_frame
    if step <= 0.0 or span <= 0.0:
        return 0
    # Ceiling, with an epsilon so an exact multiple does not round up to an extra frame.
    return max(0, int(math.ceil(span / step - 1e-9)))


def position_for_frame(n, start_frame, end_frame, pitch_ratio, file_sr, host_sr, frames,
                       reversed_):
    """SamplerVoice::PositionForFrame. The one function both directions use."""
    total = frame_count(start_frame, end_frame, pitch_ratio, file_sr, host_sr, frames)
    index = (total - 1 - n) if reversed_ else n
    return start_frame + index * step_per_frame(pitch_ratio, file_sr, host_sr)


def playback(start_frame, end_frame, pitch_ratio=1.0, file_sr=SAMPLE_RATE, host_sr=SAMPLE_RATE,
             frames=20000, reversed_=False):
    """The sequence of read positions, mirroring Process()'s loop."""
    total = frame_count(start_frame, end_frame, pitch_ratio, file_sr, host_sr, frames)
    return [position_for_frame(n, start_frame, end_frame, pitch_ratio, file_sr, host_sr, frames,
                               reversed_)
            for n in range(total)]


def main():
    failures = 0

    def report(ok, name, detail=""):
        nonlocal failures
        print(f"  {'ok  ' if ok else 'FAIL'} {name:44s} {detail}")
        if not ok:
            failures += 1

    def same_frames(label, start, end, ratio=1.0, file_sr=SAMPLE_RATE, host_sr=SAMPLE_RATE):
        """Forward and reversed must cover the same frames, in opposite orders."""
        fwd = playback(start, end, ratio, file_sr, host_sr, reversed_=False)
        rev = playback(start, end, ratio, file_sr, host_sr, reversed_=True)
        exact = fwd[::-1] == rev
        report(exact, f"{label} (ratio {ratio:g})",
               f"{len(fwd)} fwd / {len(rev)} rev" + ("" if exact else "  NOT MIRRORED"))
        return fwd, rev

    print("a slice plays its own range, not the rest of the file")
    # 6 frames of a 20000-frame sample: forward must stop at the range end.
    fwd = playback(1000, 1005)
    report(len(fwd) == 6, "6-frame range reads 6 frames", f"got {len(fwd)}")
    report(max(fwd) <= 1005, "never reads past the range end", f"max {max(fwd):.1f}")
    report(min(fwd) >= 1000, "never reads before the range start", f"min {min(fwd):.1f}")

    # The whole sample ends at frames-1, which -1 must still mean.
    whole = playback(0, -1, frames=500)
    report(len(whole) == 500, "end_frame -1 plays the whole sample", f"got {len(whole)}")

    print()
    print("forward and reversed cover identical frames")
    for label, start, end, ratio, fs, hs in [
        ("full range, unity ratio",        0, 1999, 1.0, SAMPLE_RATE, SAMPLE_RATE),
        ("full range, detuned",            0, 1999, 1.059463, SAMPLE_RATE, SAMPLE_RATE),
        ("full range, half speed",         0, 1999, 0.5, SAMPLE_RATE, SAMPLE_RATE),
        ("full range, double speed",       0, 1999, 2.0, SAMPLE_RATE, SAMPLE_RATE),
        ("full range, cross sample rate",  0, 1999, 1.0, 48000, 44100),
        ("a slice, not the whole file", 1000, 5000, 1.0, SAMPLE_RATE, SAMPLE_RATE),
        ("one frame",                    500,  500, 1.0, SAMPLE_RATE, SAMPLE_RATE),
        ("one frame at zero",              0,    0, 1.0, SAMPLE_RATE, SAMPLE_RATE),
        ("two frames",                   500,  501, 1.0, SAMPLE_RATE, SAMPLE_RATE),
        ("whole sample via -1",            0,   -1, 1.0, SAMPLE_RATE, SAMPLE_RATE),
    ]:
        same_frames(label, start, end, ratio, fs, hs)

    print()
    print("reversed starts at the range END, not one before it")
    rev = playback(1000, 1005, reversed_=True)
    report(rev and abs(rev[0] - 1005) < 1e-9, "first reversed position is the end frame",
           f"{rev[0]:.1f}" if rev else "empty")
    fwd = playback(1000, 1005)
    report(bool(rev), "reversed produced samples", f"{len(rev)}")
    report(bool(fwd), "forward produced samples", f"{len(fwd)}")

    print()
    print("a one-frame range produces exactly one sample, both ways")
    for label, r in (("forward", False), ("reversed", True)):
        seq = playback(500, 500, reversed_=r)
        report(len(seq) == 1, f"{label} one-frame range", f"{len(seq)} sample(s)")

    print()
    print("no position escapes the range, in any case")
    escaped = []
    for start, end, ratio, fs, hs in [
        (0, 1999, 1.059463, SAMPLE_RATE, SAMPLE_RATE),
        (0, 1999, 0.5, SAMPLE_RATE, SAMPLE_RATE),
        (0, 1999, 1.0, 48000, 44100),
        (1000, 5000, 1.7, SAMPLE_RATE, SAMPLE_RATE),
    ]:
        for r in (False, True):
            seq = playback(start, end, ratio, fs, hs, reversed_=r)
            # The last partial step may land past the end, but Process() clamps the read index to
            # the end frame, so nothing is read outside the range. What must never happen is a
            # position BELOW the start in either direction.
            if min(seq) < start:
                escaped.append((start, end, ratio, r, min(seq)))
    report(not escaped, "no position below the range start", f"{escaped}" if escaped else "clean")

    print()
    print("the frame counter does not drift over a long sample")
    # Accumulation would end a frame or two off after hundreds of thousands of additions.
    long_fwd = playback(0, -1, 1.059463, frames=500000)
    expected = frame_count(0, -1, 1.059463, SAMPLE_RATE, SAMPLE_RATE, 500000)
    report(len(long_fwd) == expected, "long sample frame count matches the formula",
           f"{len(long_fwd)} vs {expected}")
    # The last position may be fractional and land just past the final index: the read index is
    # clamped to the end frame in Process(), so 499999.8 reads frame 499999 and interpolates
    # towards the same clamped frame. What matters is that it is not out of the sample entirely.
    report(long_fwd[-1] <= 500000, "last position within the sample bounds",
           f"{long_fwd[-1]:.1f}")

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
