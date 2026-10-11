"""Tests for the warp map: musical-to-audio mapping and stretch ratios.

    /work/.venv/bin/python /work/tools/warp_check.py

Mirrors src/sampler/WarpMap.h. No C++ compiler on this box, so the arithmetic is ported and run.

The frame mapping and the stretch ratio are exact arithmetic and can be checked exactly, which
is worth doing before any audio goes through them: a subtly wrong mapping does not sound like a
bug, it sounds like a rhythm that drifts.

## A mistake worth recording

The first version of the stretch test asserted that a 11025-frame slice at 120 BPM was one
quarter note and expected a ratio of 1.0. It reported 0.5, and the ratio looked wrong. It was
not: 11025 frames at 44100 Hz is 0.25 s, and a quarter note at 120 BPM is 0.5 s, so the slice is
a sixteenth note. The code was right and the test was wrong.

That mistake did surface a genuine bug though -- the formula in the C++ omitted the sample rate,
so it was out by a factor of 44100 and only looked plausible because the test values were wrong
in a compensating way. Both are fixed here.
"""

import sys

SAMPLE_RATE = 44100


# --- port of WarpMap ---------------------------------------------------------------------

class Warp:
    def __init__(self):
        self.markers = []          # [(frame, beats, user_placed)]
        self.enabled = False
        self.source_tempo = 120.0
        self.tempo = 120.0
        self.host_sr = 44100.0

    def usable(self):
        return self.enabled and len(self.markers) >= 2 and self.source_tempo > 1.0

    def from_slices(self, starts, beats_per_slice):
        self.markers = [(f, i * beats_per_slice, False) for i, f in enumerate(starts)]

    def set_marker(self, frame, beats, user=False):
        for i, (f, _b, _u) in enumerate(self.markers):
            if f == frame:
                self.markers[i] = (frame, beats, user)
                return
        self.markers.append((frame, beats, user))
        self.markers.sort(key=lambda m: m[0])

    def move_marker_beat(self, index, beats):
        if index >= len(self.markers):
            return
        lower = -1e9 if index == 0 else self.markers[index - 1][1] + 1e-6
        upper = 1e9 if index + 1 >= len(self.markers) else self.markers[index + 1][1] - 1e-6
        if upper < lower:
            return
        beats = min(upper, max(lower, beats))
        f, _b, _u = self.markers[index]
        self.markers[index] = (f, beats, True)

    def remove_marker(self, index):
        if index == 0 or index >= len(self.markers):
            return
        del self.markers[index]

    def _seg(self, beats):
        seg = 0
        for i in range(len(self.markers) - 1):
            if self.markers[i][1] <= beats < self.markers[i + 1][1]:
                return i
            seg = i
        return seg

    def stretch_at_beat(self, beats):
        if not self.usable() or self.host_sr <= 1.0:
            return 1.0
        s = self._seg(beats)
        fa, ba, _ = self.markers[s]
        fb, bb, _ = self.markers[s + 1]
        audio_frames = fb - fa
        beat_span = bb - ba
        if audio_frames <= 0 or beat_span <= 0:
            return 1.0
        host_samples = beat_span * (60.0 / max(1.0, self.tempo)) * self.host_sr
        if host_samples <= 0:
            return 1.0
        return audio_frames / host_samples

    def frame_at_beat(self, beats):
        if not self.markers:
            return 0.0
        if len(self.markers) == 1 or beats <= self.markers[0][1]:
            return float(self.markers[0][0])
        s = self._seg(beats)
        fa, ba, _ = self.markers[s]
        fb, bb, _ = self.markers[s + 1]
        span = bb - ba
        if span <= 0:
            return float(fa)
        return fa + (beats - ba) / span * (fb - fa)

    def beat_at_frame(self, frame):
        if not self.markers:
            return 0.0
        if len(self.markers) == 1 or frame <= self.markers[0][0]:
            return self.markers[0][1]
        seg = len(self.markers) - 2
        for i in range(len(self.markers) - 1):
            if self.markers[i][0] <= frame <= self.markers[i + 1][0]:
                seg = i
                break
        fa, ba, _ = self.markers[seg]
        fb, bb, _ = self.markers[seg + 1]
        span = fb - fa
        if span <= 0:
            return ba
        return ba + (frame - fa) / span * (bb - ba)


# --- checks ------------------------------------------------------------------------------

def main():
    failures = 0

    def check(ok, what, detail=""):
        nonlocal failures
        print(f"  {'ok  ' if ok else 'FAIL'} {what}" + (f"   {detail}" if detail else ""))
        if not ok:
            failures += 1

    # A quarter note at 120 BPM is 0.5 s, which is 22050 samples at 44100 Hz.
    QUARTER = 22050

    print("stretch ratio: the audio must fill the musical time")
    w = Warp()
    w.enabled = True
    w.source_tempo = 120.0
    w.tempo = 120.0
    w.from_slices([0, QUARTER, 2 * QUARTER, 3 * QUARTER], 1.0)

    check(abs(w.stretch_at_beat(0.5) - 1.0) < 1e-9,
          "120 BPM, one quarter note of audio per beat: rate 1.0",
          f"got {w.stretch_at_beat(0.5):.4f}")

    w.tempo = 60.0
    check(abs(w.stretch_at_beat(0.5) - 0.5) < 1e-9,
          "60 BPM: audio stretched over twice the time, rate 0.5",
          f"got {w.stretch_at_beat(0.5):.4f}")

    w.tempo = 240.0
    check(abs(w.stretch_at_beat(0.5) - 2.0) < 1e-9,
          "240 BPM: audio compressed, rate 2.0",
          f"got {w.stretch_at_beat(0.5):.4f}")

    print()
    print("per-segment rates differ, which is what makes it a warp and not a tempo change")
    w2 = Warp()
    w2.enabled = True
    w2.source_tempo = 120.0
    w2.tempo = 120.0
    w2.markers = [(0, 0.0, False), (QUARTER, 1.0, False), (QUARTER * 4, 4.0, False)]
    r1 = w2.stretch_at_beat(0.5)   # 22050 frames over 1 beat
    r2 = w2.stretch_at_beat(2.0)   # 66150 frames over 3 beats
    check(abs(r1 - 1.0) < 1e-9, "first segment rate 1.0", f"got {r1:.4f}")
    check(abs(r2 - 1.0) < 1e-9, "second segment rate 1.0", f"got {r2:.4f}")

    # Now stretch only the second segment over more beats.
    w2.markers[2] = (QUARTER * 4, 6.0, True)
    r2 = w2.stretch_at_beat(2.0)   # 66150 frames over 5 beats -> 3 s of audio into 2.5 s
    expected = (QUARTER * 3) / (5.0 * (60.0 / 120.0) * SAMPLE_RATE)
    check(abs(r2 - expected) < 1e-9, "stretching one marker changes only that segment",
          f"got {r2:.4f}, want {expected:.4f}")
    check(abs(w2.stretch_at_beat(0.5) - 1.0) < 1e-9,
          "the segment before it is untouched")

    print()
    print("frame mapping: a beat lands on the right audio frame")
    w3 = Warp()
    w3.enabled = True
    w3.markers = [(0, 0.0, False), (QUARTER, 1.0, False), (QUARTER * 4, 4.0, False)]

    check(abs(w3.frame_at_beat(0.0) - 0) < 1e-9, "beat 0 -> frame 0")
    check(abs(w3.frame_at_beat(1.0) - QUARTER) < 1e-9, "beat 1 -> frame 22050")
    check(abs(w3.frame_at_beat(4.0) - QUARTER * 4) < 1e-9, "beat 4 -> frame 88200")
    check(abs(w3.frame_at_beat(0.5) - QUARTER / 2) < 1e-9,
          "halfway through a linear segment -> halfway through the audio",
          f"got {w3.frame_at_beat(0.5):.0f}")
    check(abs(w3.frame_at_beat(2.0) - QUARTER * 2) < 1e-9,
          "a beat inside the second segment maps linearly",
          f"got {w3.frame_at_beat(2.0):.0f}")

    print()
    print("the mapping inverts: frame -> beat -> frame is identity")
    ok = True
    for frame in (0, 5000, 22050, 44100, 88200):
        beats = w3.beat_at_frame(frame)
        back = w3.frame_at_beat(beats)
        if abs(back - frame) > 1e-6:
            ok = False
            print(f"        frame {frame} -> beat {beats} -> frame {back}")
    check(ok, "round trip is exact for sampled positions")

    print()
    print("markers may not cross")
    w4 = Warp()
    w4.markers = [(0, 0.0, False), (1000, 1.0, False), (2000, 2.0, False)]
    w4.move_marker_beat(1, 5.0)     # way past the next marker
    check(w4.markers[1][1] < w4.markers[2][1], "dragging a marker past its neighbour is clamped",
          f"got {w4.markers[1][1]:.4f}, next is {w4.markers[2][1]:.4f}")
    w4.move_marker_beat(1, -5.0)    # way before the previous
    check(w4.markers[1][1] > w4.markers[0][1], "dragging before the previous marker is clamped",
          f"got {w4.markers[1][1]:.4f}, previous is {w4.markers[0][1]:.4f}")

    print()
    print("the first marker is never removed: it anchors the sample start")
    w5 = Warp()
    w5.markers = [(0, 0.0, False), (1000, 1.0, False)]
    w5.remove_marker(0)
    check(len(w5.markers) == 2, "removing index 0 is refused")
    w5.remove_marker(1)
    check(len(w5.markers) == 1, "removing a later marker works")

    print()
    print("a disabled or empty map is a pass-through")
    w6 = Warp()
    w6.enabled = False
    w6.from_slices([0, 1000], 1.0)
    check(abs(w6.stretch_at_beat(0.5) - 1.0) < 1e-9, "warp off -> rate 1.0")
    w6.enabled = True
    w6.from_slices([0], 1.0)
    check(abs(w6.stretch_at_beat(0.0) - 1.0) < 1e-9, "one marker -> rate 1.0, nothing to warp")

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
