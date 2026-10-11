"""Tests for the sampler's decode and playback maths.

    /work/.venv/bin/python /work/tools/sampler_check.py

Mirrors src/sampler/SamplerVoice.h and src/sampler/SampleLoader.h. This box has no C++
compiler and no way to install one, so the logic is ported here to be actually run rather
than only written.

Covered:
  - AIFF header parsing, including the 80-bit extended sample-rate field
  - AIFF sample decoding (big-endian, signed, 16-bit)
  - fractional playback position and linear interpolation
  - pitch ratios up and down, and the file-rate/host-rate adjustment

NOT covered: the dr_libs decoders for wav/flac/mp3. Those are third-party and only exist in
the C++ build, so their behaviour is asserted from the build, not from here.
"""

import math
import os
import struct
import sys
import tempfile
import wave

SAMPLE_RATE = 44100


# --- helpers to build real files ---------------------------------------------------------

def write_wav(path, frames, rate=SAMPLE_RATE, channels=2, bits=16):
    with wave.open(path, "wb") as w:
        w.setnchannels(channels)
        w.setsampwidth(bits // 8)
        w.setframerate(rate)
        w.writeframes(b"".join(struct.pack("<hh", l, r) for l, r in frames))


def write_aiff(path, frames, rate=SAMPLE_RATE, channels=2, bits=16, form=b"AIFF"):
    def ext80(x):
        if x == 0:
            return b"\x00" * 10
        e = 0
        while x >= 2.0:
            x /= 2.0
            e += 1
        while x < 1.0:
            x *= 2.0
            e -= 1
        return struct.pack(">H", e + 16383) + struct.pack(">Q", int(x * (1 << 63)))

    n = len(frames)
    data = b"".join(struct.pack(">h", s) for fr in frames for s in fr)
    comm = struct.pack(">hIh", channels, n, bits) + ext80(float(rate))
    ssnd_body = struct.pack(">II", 0, 0) + data
    body = (b"COMM" + struct.pack(">I", len(comm)) + comm +
            b"SSND" + struct.pack(">I", len(ssnd_body)) + ssnd_body)
    with open(path, "wb") as f:
        f.write(b"FORM" + struct.pack(">I", 4 + len(body)) + form + body)


# --- ports of the C++ logic --------------------------------------------------------------

def ext80_to_double(b):
    """The 80-bit IEEE extended float AIFF uses for its sample rate."""
    exp = (struct.unpack(">H", b[0:2])[0] & 0x7FFF) - 16383
    hi = struct.unpack(">I", b[2:6])[0]
    lo = struct.unpack(">I", b[6:10])[0]
    return (hi * 4294967296.0 + lo) * (2.0 ** (exp - 63))


def parse_aiff(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != b"FORM":
        return {"error": "UnsupportedFormat"}
    form = data[8:12]
    if form not in (b"AIFF", b"AIFC"):
        return {"error": "UnsupportedFormat"}
    if form == b"AIFC":
        return {"error": "UnsupportedFormat"}  # compressed: refused, not guessed
    pos, info = 12, {"error": None}
    while pos + 8 <= len(data):
        cid = data[pos:pos + 4]
        size = struct.unpack(">I", data[pos + 4:pos + 8])[0]
        body = pos + 8
        if cid == b"COMM" and size >= 18:
            info["channels"] = struct.unpack(">h", data[body:body + 2])[0]
            info["frames"] = struct.unpack(">I", data[body + 2:body + 6])[0]
            info["bits"] = struct.unpack(">h", data[body + 6:body + 8])[0]
            info["rate"] = ext80_to_double(data[body + 8:body + 18])
        elif cid == b"SSND" and size >= 8:
            info["ssnd"] = body + 8 + struct.unpack(">I", data[body:body + 4])[0]
            break
        pos = body + ((size + 1) & ~1)   # chunks are even-padded
    return info


def decode_aiff_16(path, info, count):
    with open(path, "rb") as f:
        data = f.read()
    out = []
    for i in range(count):
        off = info["ssnd"] + i * 4
        out.append(struct.unpack(">h", data[off:off + 2])[0])
    return out


def play(frames, file_sr, host_sr, pitch_ratio, count):
    """Port of SamplerVoice::Process."""
    pos, last, out = 0.0, len(frames) - 1, []
    for _ in range(count):
        if pos >= last:
            break
        i0 = int(pos)
        i1 = min(i0 + 1, last)
        frac = pos - i0
        out.append(frames[i0] + (frames[i1] - frames[i0]) * frac)
        pos += pitch_ratio * (file_sr / host_sr)
    return out


# --- the checks --------------------------------------------------------------------------

def main():
    failures = 0

    def check(ok, what, detail=""):
        nonlocal failures
        print(f"  {'ok  ' if ok else 'FAIL'} {what}" + (f"   {detail}" if detail else ""))
        if not ok:
            failures += 1

    tmp = tempfile.mkdtemp(prefix="sampler-check-")

    # a real signal: 440 Hz at half amplitude, 0.1 s, stereo
    n = int(SAMPLE_RATE * 0.1)
    frames = []
    for i in range(n):
        v = int(32767 * 0.5 * math.sin(2 * math.pi * 440 * i / SAMPLE_RATE))
        frames.append((v, v // 2))

    wav_path = os.path.join(tmp, "tone.wav")
    aiff_path = os.path.join(tmp, "tone.aiff")
    write_wav(wav_path, frames)
    write_aiff(aiff_path, frames)

    print("AIFF header parsing")
    info = parse_aiff(aiff_path)
    check(info.get("error") is None, "a plain AIFF parses")
    check(info.get("channels") == 2, "channel count", f"got {info.get('channels')}")
    check(info.get("frames") == n, "frame count", f"got {info.get('frames')} of {n}")
    check(info.get("bits") == 16, "bit depth", f"got {info.get('bits')}")
    check(abs(info.get("rate", 0) - SAMPLE_RATE) < 0.5,
          "sample rate via the 80-bit extended field", f"got {info.get('rate')}")

    print()
    print("AIFF sample decoding")
    got = decode_aiff_16(aiff_path, info, 6)
    want = [int(32767 * 0.5 * math.sin(2 * math.pi * 440 * i / SAMPLE_RATE)) for i in range(6)]
    check(all(abs(a - b) <= 1 for a, b in zip(got, want)),
          "big-endian signed samples decode to the written signal",
          f"got {got} want {want}")

    print()
    print("AIFC is refused rather than guessed at")
    aifc = os.path.join(tmp, "comp.aiff")
    write_aiff(aifc, frames, form=b"AIFC")
    check(parse_aiff(aifc).get("error") == "UnsupportedFormat",
          "a compressed AIFF-C returns UnsupportedFormat")

    print()
    print("playback: fractional position and interpolation")
    ramp = [i / 99 for i in range(100)]

    got = play(ramp, SAMPLE_RATE, SAMPLE_RATE, 1.0, 5)
    check(all(abs(g - ramp[i]) < 1e-9 for i, g in enumerate(got)),
          "pitch 1.0 steps one frame at a time")

    got = play(ramp, SAMPLE_RATE, SAMPLE_RATE, 2.0, 5)
    check(all(abs(g - ramp[i * 2]) < 1e-9 for i, g in enumerate(got)),
          "pitch 2.0 skips every other frame")

    got = play(ramp, SAMPLE_RATE, SAMPLE_RATE, 0.5, 5)
    check(all(abs(g - ramp[i] * 0.5) < 1e-9 for i, g in enumerate(got)),
          "pitch 0.5 interpolates the midpoints")

    got = play(ramp, 48000, 44100, 1.0, 4)
    step = (got[1] - got[0]) / (ramp[1] - ramp[0])
    check(abs(step - 48000 / 44100) < 1e-9,
          "a 48 kHz file advances slower in a 44.1 kHz host", f"step {step:.5f}")

    print()
    print("runoff")
    got = play(ramp, SAMPLE_RATE, SAMPLE_RATE, 1.0, 200)
    check(len(got) == 99, "playback stops at the end rather than reading past it",
          f"{len(got)} samples")

    for p in (wav_path, aiff_path, aifc):
        if os.path.exists(p):
            os.remove(p)
    os.rmdir(tmp)

    print()
    print("PASS" if failures == 0 else f"FAIL ({failures})")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
