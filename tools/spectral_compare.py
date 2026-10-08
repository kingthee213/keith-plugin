#!/usr/bin/env python3
"""Spectral comparison probe: how far is a recreation from its reference, honestly?

PROBE, not a gate. This exists to answer one question with numbers: the recreation
gate compares four scalars (fundamental, attack, decay, spectral centroid) at 8 %
tolerance, and four scalars cannot tell two different-sounding instruments apart.
A time-frequency comparison can. This measures that, and reports the scale it lives on
so a threshold can be chosen defensibly instead of guessed.

Method, and why each part is here:

- Onset alignment. The recordings and our renders are not sample-aligned. Comparing
  unaligned signals measures the delay, not the timbre, so both are trimmed to their
  first onset before anything else.
- Loudness normalisation. Both are scaled to equal RMS over the compared window, so
  the result describes spectral shape and not gain.
- Log-mel spectrogram (64 bands). Mel spacing is perceptually motivated and the log
  compresses dynamics, which is what stops one loud band dominating the distance.
- Spectral convergence, ||A-B||_F / ||A||_F, reported as a percentage. This is the
  "% difference" number, on linear magnitudes.
- Log-mel RMSE in dB, reported beside it, because a percentage on linear magnitudes
  and an error in dB disagree in interesting ways and both are worth seeing.

The number is only meaningful against a scale, so this also computes the distance
between DIFFERENT reference instruments (what "clearly not the same sound" scores)
and between different notes of the SAME instrument (what "same instrument, different
pitch" scores). A recreation is good if it sits far closer to its own reference than
the cross-instrument baseline.

Nothing here is shipped and no audio is redistributed; the reference recordings are
analysis material in a gitignored directory.
"""

import argparse
import json
import subprocess
import sys
import wave
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parent.parent
REF_DIR = ROOT / "reference-audio"
# The plugin renders at 48 kHz and the reference recordings are 44.1 kHz, so one side
# must be resampled. References are converted up to the render rate.
SR = 48000
N_FFT = 2048
HOP = 512
N_MELS = 64
FMIN, FMAX = 40.0, 16000.0
WINDOW_S = 2.0  # compared window after the onset
MIN_WINDOW_S = 0.35  # short percussive references (e.g. the Stein basses) need less


def load_wav_mono(path: Path) -> np.ndarray:
    with wave.open(str(path), "rb") as w:
        nch, sw, sr, n = w.getnchannels(), w.getsampwidth(), w.getframerate(), w.getnframes()
        raw = w.readframes(n)
    if sw == 2:
        x = np.frombuffer(raw, dtype="<i2").astype(np.float64) / 32768.0
    elif sw == 3:
        b = np.frombuffer(raw, dtype=np.uint8).reshape(-1, 3)
        v = (b[:, 0].astype(np.int32) | (b[:, 1].astype(np.int32) << 8)
             | (b[:, 2].astype(np.int32) << 16))
        v = np.where(v & 0x800000, v - 0x1000000, v)
        x = v.astype(np.float64) / 8388608.0
    elif sw == 4:
        x = np.frombuffer(raw, dtype="<i4").astype(np.float64) / 2147483648.0
    else:
        raise ValueError(f"unsupported sample width {sw}")
    if nch > 1:
        x = x.reshape(-1, nch).mean(axis=1)
    return x, sr


def load_render_mono(path: Path) -> np.ndarray:
    """Read the render tool's format: int32 sample rate, int32 frames, interleaved f32."""
    with open(path, "rb") as f:
        head = np.frombuffer(f.read(8), dtype="<i4")
        sr, frames = int(head[0]), int(head[1])
        data = np.frombuffer(f.read(), dtype="<f4")
    if data.size < frames * 2:
        raise ValueError(f"{path.name}: expected {frames * 2} floats, got {data.size}")
    x = data[: frames * 2].reshape(-1, 2).mean(axis=1).astype(np.float64)
    return x, sr


def onset_index(x: np.ndarray, sr: int) -> int:
    """First sample that rises above a small fraction of the signal's peak envelope."""
    win = max(1, sr // 200)
    env = np.convolve(np.abs(x), np.ones(win) / win, mode="same")
    if not np.any(env):
        return 0
    thr = 0.05 * env.max()
    above = np.flatnonzero(env >= thr)
    return int(above[0]) if above.size else 0


def resample(x: np.ndarray, sr_in: int, sr_out: int) -> np.ndarray:
    """Band-limited resampling by zero-padding the spectrum.

    Linear interpolation would act as a lowpass and quietly attenuate the top octave,
    which is precisely the region these recreations are being judged on, so the
    spectrum is padded instead.
    """
    if sr_in == sr_out:
        return x
    n_in = x.size
    n_out = int(round(n_in * sr_out / sr_in))
    spec = np.fft.rfft(x)
    out_spec = np.zeros(n_out // 2 + 1, dtype=complex)
    m = min(spec.size, out_spec.size)
    out_spec[:m] = spec[:m]
    y = np.fft.irfft(out_spec, n=n_out)
    return y * (n_out / n_in)


def prepare(x: np.ndarray, sr: int) -> np.ndarray | None:
    """Onset-trim, resample-free window, loudness-normalise."""
    if sr != SR:
        x = resample(x, sr, SR)
        sr = SR
    start = onset_index(x, sr)
    x = x[start:]
    need = int(MIN_WINDOW_S * sr)
    if x.size < need:
        return None
    n = min(int(WINDOW_S * sr), x.size)
    x = x[:n].copy()
    rms = np.sqrt(np.mean(x ** 2))
    if rms <= 0:
        return None
    return x / rms


def mel_filterbank(sr: int, n_fft: int, n_mels: int, fmin: float, fmax: float) -> np.ndarray:
    def hz_to_mel(f):
        return 2595.0 * np.log10(1.0 + f / 700.0)

    def mel_to_hz(m):
        return 700.0 * (10.0 ** (m / 2595.0) - 1.0)

    pts = mel_to_hz(np.linspace(hz_to_mel(fmin), hz_to_mel(fmax), n_mels + 2))
    freqs = np.fft.rfftfreq(n_fft, 1.0 / sr)
    fb = np.zeros((n_mels, freqs.size))
    for m in range(n_mels):
        lo, ctr, hi = pts[m], pts[m + 1], pts[m + 2]
        left = (freqs - lo) / max(ctr - lo, 1e-9)
        right = (hi - freqs) / max(hi - ctr, 1e-9)
        fb[m] = np.clip(np.minimum(left, right), 0.0, None)
    return fb


def logmel(x: np.ndarray, fb: np.ndarray) -> np.ndarray:
    win = np.hanning(N_FFT)
    frames = 1 + (x.size - N_FFT) // HOP
    out = np.empty((frames, fb.shape[0]))
    for i in range(frames):
        seg = x[i * HOP: i * HOP + N_FFT] * win
        mag = np.abs(np.fft.rfft(seg))
        out[i] = fb @ mag
    return out


def spectral_convergence(a_lin: np.ndarray, b_lin: np.ndarray) -> float:
    num = np.linalg.norm(a_lin - b_lin)
    den = np.linalg.norm(a_lin)
    return float(100.0 * num / den) if den > 0 else float("nan")


def logmel_rmse_db(a_lin: np.ndarray, b_lin: np.ndarray) -> float:
    eps = 1e-10
    a_db = 20.0 * np.log10(a_lin + eps)
    b_db = 20.0 * np.log10(b_lin + eps)
    return float(np.sqrt(np.mean((a_db - b_db) ** 2)))


def compare(xa: np.ndarray, xb: np.ndarray, fb: np.ndarray) -> dict:
    A, B = logmel(xa, fb), logmel(xb, fb)
    r = min(A.shape[0], B.shape[0])
    A, B = A[:r], B[:r]
    return {
        "sc_pct": round(spectral_convergence(A, B), 2),
        "logmel_rmse_db": round(logmel_rmse_db(A, B), 2),
    }


RECREATIONS = [
    ("Family 01 / 021 Upright Bass", 36, 5.2, "Korg-Triton-Acoustic-Bass-C2.wav"),
    ("Family 01 / 022 Hammer Keys", 24, 3.4, "Korg-Triton-Acoustic-Piano-C1.wav"),
    ("Family 01 / 023 Swell Choir", 60, 5.6, "Korg-Triton-Slow-Choir-ST-C4.wav"),
    ("Family 01 / 024 Bright Reed", 72, 3.2, "Korg-Triton-Harmonica-C5.wav"),
    ("Family 01 / 025 Snap Bass", 36, 2.2, "Korg-Triton-Stein-Bass-C2.wav"),
    ("Family 01 / 026 Short Bass", 48, 2.4, "Korg-Triton-Stein-Bass-C3.wav"),
]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("renderer", help="path to keith-recreation-render.exe")
    ap.add_argument("--out", default=str(ROOT / "build" / "spectral-probe.json"))
    args = ap.parse_args()

    fb = mel_filterbank(SR, N_FFT, N_MELS, FMIN, FMAX)
    tmp = ROOT / "build" / "spectral-probe"
    tmp.mkdir(parents=True, exist_ok=True)

    refs: dict[str, np.ndarray] = {}
    prepared_refs: dict[str, np.ndarray] = {}
    for _, _, _, ref in RECREATIONS:
        if ref in refs:
            continue
        p = REF_DIR / ref
        if not p.exists():
            print(f"missing reference {ref}", file=sys.stderr)
            return 2
        x, sr = load_wav_mono(p)
        refs[ref] = x
        y = prepare(x, sr)
        if y is None:
            print(f"reference too short after trim: {ref}", file=sys.stderr)
            return 2
        prepared_refs[ref] = y

    rows = []
    for preset, note, secs, ref in RECREATIONS:
        out = tmp / f"{preset.split('/')[-1].strip().replace(' ', '_')}.f32"
        subprocess.run([args.renderer, preset, str(note), str(secs), str(out)],
                       check=True, capture_output=True)
        xr, sr = load_render_mono(out)
        yr = prepare(xr, sr)
        if yr is None:
            print(f"render too short after trim: {preset}", file=sys.stderr)
            return 2
        m = compare(prepared_refs[ref], yr, fb)
        rows.append({"preset": preset, "note": note, "reference": ref, **m})

    # Scale: what does "clearly a different instrument" score, and what does
    # "same instrument, different pitch" score? Without this the percentages are
    # meaningless.
    names = list(prepared_refs)
    cross = []
    for i, a in enumerate(names):
        for b in names[i + 1:]:
            cross.append({"a": a, "b": b, **compare(prepared_refs[a], prepared_refs[b], fb)})

    doc = {
        "method": {
            "window_s": WINDOW_S, "n_fft": N_FFT, "hop": HOP, "n_mels": N_MELS,
            "fmin": FMIN, "fmax": FMAX, "sr": SR,
            "alignment": "onset trim", "level": "RMS normalised",
        },
        "recreations": rows,
        "cross_reference_baseline": sorted(cross, key=lambda r: r["sc_pct"]),
    }
    Path(args.out).write_text(json.dumps(doc, indent=2), encoding="utf-8")

    print(f"{'preset':<30} {'note':>4} {'SC %':>7} {'logmel dB':>10}")
    for r in rows:
        print(f"{r['preset']:<30} {r['note']:>4} {r['sc_pct']:>7} {r['logmel_rmse_db']:>10}")
    print("\nscale - closest and furthest reference-to-reference pairs:")
    cr = doc["cross_reference_baseline"]
    for r in cr[:2]:
        print(f"  {r['sc_pct']:>7} %  {r['a']} vs {r['b']}")
    print("  ...")
    for r in cr[-2:]:
        print(f"  {r['sc_pct']:>7} %  {r['a']} vs {r['b']}")
    print(f"\nwrote {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
