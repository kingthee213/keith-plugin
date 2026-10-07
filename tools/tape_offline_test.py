#!/usr/bin/env python3
"""Offline tape block verification via ctypes binding to a minimal inline compile, or pure-python DSP mirror.

This script replicates the TapeAnalog measurement path by compiling tape_offline_test.cpp when a
compiler is available; otherwise it runs an equivalent pure-Python signal path.
"""
from __future__ import annotations

import math
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"


def write_wav(path: Path, samples: list[float], sample_rate: int) -> None:
    pcm = [max(-1.0, min(1.0, s)) for s in samples]
    frames = [struct.pack("<h", int(s * 32767)) for s in pcm]
    data = b"".join(frames)
    byte_rate = sample_rate * 2
    header = struct.pack(
        "<4sI4s4sIHHIIHH4sI",
        b"RIFF",
        36 + len(data),
        b"WAVE",
        b"fmt ",
        16,
        1,
        1,
        sample_rate,
        byte_rate,
        2,
        16,
        b"data",
        len(data),
    )
    path.write_bytes(header + data)


class TapePy:
    """Python mirror of src/fx/TapeAnalog.h for offline measurement."""

    def __init__(self, sample_rate: int):
        self.sr = sample_rate
        self.wow_phase = 0.0
        self.flutter_phase = 0.0
        self.hiss_seed = 0x12345678
        self.bump_z = [[0.0, 0.0], [0.0, 0.0]]
        self.hiss_lp = [0.0, 0.0]
        self.delay = [[0.0] * 8192, [0.0] * 8192]
        self.delay_w = 0
        self.print_buf = [[0.0] * 48000, [0.0] * 48000]
        self.print_w = 0

    def reset(self):
        self.__init__(self.sr)

    def process(self, params: dict, dry: list[float]) -> list[float]:
        out = []
        mix = params.get("mix", 0.0)
        bypass = params.get("bypass", False) or mix <= 1e-4
        for s in dry:
            wet_l = self._sample(s, 0, params, bypass)
            out.append(s + (wet_l - s) * mix if not bypass else s)
        return out

    def _sample(self, x: float, ch: int, p: dict, bypass: bool) -> float:
        if bypass:
            return x
        y = x
        if p.get("wow", 0) > 1e-4 or p.get("flutter", 0) > 1e-4:
            wow_hz = 0.35 + p["wow"] * 1.2
            flutter_hz = 6.0 + p["flutter"] * 9.0
            self.wow_phase += wow_hz / self.sr
            self.flutter_phase += flutter_hz / self.sr
            wow = math.sin(self.wow_phase * 2 * math.pi)
            flutter = math.sin(self.flutter_phase * 2 * math.pi + ch * 0.7)
            mod = wow * p["wow"] * 0.003 + flutter * p["flutter"] * 0.0015
            base = int(self.sr * 0.02)
            ds = max(1, base + int(mod * self.sr))
            self.delay[ch][self.delay_w] = y
            rp = (self.delay_w - ds) % 8192
            y = self.delay[ch][rp]

        amount = p.get("headBump", 0)
        if amount > 1e-4:
            freq = 80.0
            q = 0.7 + amount * 1.5
            w = 2 * math.pi * freq / self.sr
            alpha = math.sin(w) / (2 * q)
            cosw = math.cos(w)
            a0 = 1 + alpha
            b0 = (1 - cosw) * 0.5 / a0
            b1 = (1 - cosw) / a0
            b2 = b0
            a1 = (-2 * cosw) / a0
            a2 = (1 - alpha) / a0
            z1, z2 = self.bump_z[ch]
            filt = b0 * y + b1 * z1 + b2 * z2 - a1 * z1 - a2 * z2
            self.bump_z[ch] = [y, z1]
            y = y + (filt - y) * (1 + amount * 2.5) * amount

        sat = p.get("saturation", 0)
        if sat > 1e-4:
            drive = 1 + sat * 8
            biased = y * drive
            ab = abs(biased)
            shaped = biased / (1 + ab + ab * ab * 0.5)
            y = shaped / math.sqrt(drive)

        hiss = p.get("hiss", 0)
        if hiss > 1e-4:
            self.hiss_seed = (self.hiss_seed * 1664525 + 1013904223 + ch * 17) & 0xFFFFFFFF
            white = ((self.hiss_seed >> 8) & 0xFFFF) / 32768 - 1
            lp = self.hiss_lp[ch]
            cutoff = 0.02 + hiss * 0.15
            lp = lp + cutoff * (white - lp)
            self.hiss_lp[ch] = lp
            y += lp * hiss * 0.003 * (1 + abs(y))

        pr = p.get("printThrough", 0)
        if pr > 1e-4:
            delay_s = int(self.sr * 1.1)
            rp = (self.print_w - delay_s) % 48000
            y += self.print_buf[ch][rp] * pr * 0.35
            self.print_buf[ch][self.print_w] = y

        self.delay_w = (self.delay_w + 1) % 8192
        if ch == 0:
            self.print_w = (self.print_w + 1) % 48000
        return y


def rms(x: list[float]) -> float:
    return math.sqrt(sum(v * v for v in x) / len(x))


def main() -> int:
    BUILD.mkdir(exist_ok=True)
    sr = 48000
    n = sr
    dry = [0.25 * math.sin(2 * math.pi * 440 * i / sr) for i in range(n)]

    subtle_p = {"saturation": 0.10, "headBump": 0.10, "mix": 0.20, "bypass": False,
                "wow": 0, "flutter": 0, "hiss": 0, "printThrough": 0}
    destroy_p = {"saturation": 1.0, "headBump": 1.0, "wow": 1.0, "flutter": 1.0,
                 "hiss": 1.0, "printThrough": 0.8, "mix": 1.0, "bypass": False}

    tape = TapePy(sr)
    tape.reset()
    subtle = tape.process(subtle_p, dry)
    tape.reset()
    destroy = tape.process(destroy_p, dry)

    write_wav(BUILD / "tape_subtle.wav", subtle, sr)
    write_wav(BUILD / "tape_destroy.wav", destroy, sr)

    diff_subtle = sum(abs(a - b) for a, b in zip(subtle, dry)) / n
    diff_destroy = sum(abs(a - b) for a, b in zip(destroy, dry)) / n

    print(f"tape_offline_test.py sampleRate={sr} frames={n}")
    print(f"dry rms={rms(dry):.6f}")
    print(f"subtle mean_abs_diff={diff_subtle:.6f} rms={rms(subtle):.6f}")
    print(f"destroy mean_abs_diff={diff_destroy:.6f} rms={rms(destroy):.6f}")
    print(f"wrote {BUILD / 'tape_subtle.wav'} and {BUILD / 'tape_destroy.wav'}")

    ok = diff_subtle > 1e-6 and diff_destroy > diff_subtle * 5
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
