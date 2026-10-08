#!/usr/bin/env python3
"""Measure reference recordings so the synth can be tuned to match them.

Analysis only. Nothing here is shipped; the audio stays out of the repository.
Emits docs/REFERENCE-ANALYSIS.md and a JSON target table.

Usage: python tools/analyse_references.py [reference-audio-dir]
"""
import json
import sys
import wave
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parent.parent
SRC = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "reference-audio"

NOTE_HZ = {  # used to sanity-check the detected fundamental against the file name
    "C1": 32.70, "C2": 65.41, "C3": 130.81, "C4": 261.63, "C5": 523.25,
}


def read_wav(path):
    with wave.open(str(path), "rb") as w:
        ch, sw, sr, nf = w.getnchannels(), w.getsampwidth(), w.getframerate(), w.getnframes()
        raw = w.readframes(nf)
    if sw != 2:
        raise SystemExit(f"{path.name}: expected 16-bit, got {sw*8}-bit")
    a = np.frombuffer(raw, dtype="<i2").astype(np.float32) / 32768.0
    if ch == 2:
        a = a.reshape(-1, 2)
        left, right = a[:, 0], a[:, 1]
        mono = 0.5 * (left + right)
        # stereo correlation: how mono-compatible the recording is
        denom = (np.sqrt(np.sum(left ** 2)) * np.sqrt(np.sum(right ** 2))) or 1.0
        corr = float(np.sum(left * right) / denom)
    else:
        mono, corr = a, 1.0
    return mono, sr, corr


def rms_envelope(x, sr, hop=256):
    n = len(x) // hop * hop
    frames = x[:n].reshape(-1, hop)
    return np.sqrt(np.mean(frames ** 2, axis=1)), sr / hop


def detect_f0(x, sr, lo=25.0, hi=2000.0):
    """Autocorrelation pitch detection, restricted to the instrument's range."""
    if len(x) > sr:
        x = x[:sr]
    x = x - x.mean()
    if not np.any(x):
        return 0.0
    ac = np.correlate(x, x, mode="full")[len(x) - 1:]
    lo_lag, hi_lag = max(1, int(sr / hi)), min(len(ac) - 1, int(sr / lo))
    if hi_lag <= lo_lag:
        return 0.0
    seg = ac[lo_lag:hi_lag]
    lag = lo_lag + int(np.argmax(seg))
    # parabolic refinement for sub-bin accuracy
    if 0 < lag < len(ac) - 1:
        a, b, c = ac[lag - 1], ac[lag], ac[lag + 1]
        d = a - 2 * b + c
        if d != 0:
            lag = lag + 0.5 * (a - c) / d
    return float(sr / lag) if lag else 0.0


def spectrum_features(x, sr):
    w = np.hanning(len(x))
    spec = np.abs(np.fft.rfft(x * w))
    freqs = np.fft.rfftfreq(len(x), 1.0 / sr)
    total = spec.sum() or 1.0
    centroid = float((spec * freqs).sum() / total)
    cumsum = np.cumsum(spec)
    rolloff = float(freqs[np.searchsorted(cumsum, 0.85 * cumsum[-1])])
    return centroid, rolloff, spec, freqs


def harmonic_ratio(spec, freqs, f0):
    """Energy in harmonic partials vs total: 1.0 = tonal, low = noisy/percussive."""
    if f0 <= 0:
        return 0.0
    total = spec.sum() or 1.0
    harm = 0.0
    for k in range(1, 40):
        f = f0 * k
        if f > freqs[-1]:
            break
        i = int(np.argmin(np.abs(freqs - f)))
        lo, hi = max(0, i - 2), min(len(spec), i + 3)
        harm += spec[lo:hi].sum()
    return float(min(1.0, harm / total))


def analyse(path):
    x, sr, corr = read_wav(path)
    dur = len(x) / sr
    name = path.stem
    guess = next((k for k in NOTE_HZ if name.upper().endswith(k)), None)
    env, env_sr = rms_envelope(x, sr)
    peak = int(np.argmax(env)) if env.size else 0
    peak_val = float(env[peak]) if env.size else 0.0
    attack_s = peak / env_sr if env_sr else 0.0
    # decay: from peak to -20 dB
    target = peak_val * 0.1
    after = np.where(env[peak:] <= target)[0]
    decay_s = float(after[0] / env_sr) if after.size else float(dur - attack_s)

    # Analyse across the whole sustain, not one arbitrary window: a single slice can
    # land where the fundamental is weak and report a bogus pitch or a bogus harmonic
    # ratio. Averaging several windows across the body is far more stable.
    body_start = int(min(len(x) - 1, max(attack_s, 0.02) * sr))
    body = x[body_start:]
    if body.size < 64:
        body = x[: min(len(x), sr)]

    win = min(len(body), sr)                     # up to 1 s per window
    max_windows = 8
    starts = np.linspace(0, max(0, len(body) - win), num=max_windows).astype(int)
    spectra, freqs = [], None
    f0s = []
    for s in starts:
        seg = body[s:s + win]
        if seg.size < 64:
            continue
        _, _, spec_i, freqs = spectrum_features(seg, sr)
        spectra.append(spec_i)
        f = detect_f0(seg, sr)
        if f > 0:
            f0s.append(f)
    if not spectra:
        centroid, rolloff, spec, freqs = spectrum_features(body, sr)
        f0 = detect_f0(body, sr)
    else:
        spec = np.mean(spectra, axis=0)
        centroid = float((spec * freqs).sum() / (spec.sum() or 1.0))
        cumsum = np.cumsum(spec)
        rolloff = float(freqs[np.searchsorted(cumsum, 0.85 * cumsum[-1])])
        f0 = float(np.median(f0s)) if f0s else 0.0

    harm = harmonic_ratio(spec, freqs, f0)

    # Octave sanity: autocorrelation happily locks onto a strong harmonic. If the
    # detected pitch is a near-exact multiple or divisor of the named note, the
    # detector is reporting a harmonic, so say so instead of shipping a wrong target.
    named = NOTE_HZ.get(guess) if guess else None
    suspect = None
    if named:
        ratios = [f0 / named, named / f0] if f0 else [0]
        best = min(ratios, key=lambda r: abs(r - round(r)) if r else 9)
        if abs(f0 - named) / named > 0.06:
            k = round(f0 / named) if f0 > named else round(named / f0)
            if k and abs(best - k) < 0.05:
                suspect = f"octave/harmonic lock x{k} vs named note (expected {named:.2f} Hz)"
            else:
                suspect = f"disagrees with named note (expected {named:.2f} Hz)"

    # A very low harmonic ratio on a clearly keyed instrument usually means the
    # fundamental fell outside the analysed band rather than that the sound is noise.
    if harm < 0.10 and not suspect:
        suspect = "harmonic ratio implausibly low for a keyed instrument - treat as unreliable"

    return {
        "file": path.name,
        "duration_s": round(dur, 2),
        "detected_f0_hz": round(f0, 1),
        "named_note": guess,
        "named_note_hz": NOTE_HZ.get(guess) if guess else None,
        "attack_s": round(attack_s, 3),
        "decay_to_minus20db_s": round(decay_s, 2),
        "spectral_centroid_hz": round(centroid, 0),
        "spectral_rolloff85_hz": round(rolloff, 0),
        "harmonic_ratio": round(harm, 3),
        "stereo_correlation": round(corr, 3),
        "suspect": suspect,
    }


def main():
    wavs = sorted(SRC.glob("*.wav"))
    if not wavs:
        raise SystemExit(f"no .wav files in {SRC}")
    rows = [analyse(w) for w in wavs]
    (ROOT / "docs").mkdir(exist_ok=True)
    (ROOT / "build").mkdir(exist_ok=True)
    (ROOT / "build" / "reference-targets.json").write_text(json.dumps(rows, indent=2))

    lines = [
        "# Reference analysis — synthesis targets",
        "",
        "Measured from reference recordings held in a scratch directory that is **not** part of the",
        "build and **not** shipped. Only the measurements below are used; the audio never enters the",
        "plugin, the repository, or the package.",
        "",
        "Measured by `tools/analyse_references.py` (numpy, no external DSP dependency).",
        "",
        "| File | Dur s | F0 Hz | Note | Attack s | Decay to -20 dB s | Centroid Hz | Rolloff85 Hz | Harmonic ratio | Stereo corr | Reliability |",
        "|---|---|---|---|---|---|---|---|---|---|---|",
    ]
    for r in rows:
        lines.append(
            f"| {r['file']} | {r['duration_s']} | {r['detected_f0_hz']} | {r['named_note'] or '-'} "
            f"| {r['attack_s']} | {r['decay_to_minus20db_s']} | {r['spectral_centroid_hz']} "
            f"| {r['spectral_rolloff85_hz']} | {r['harmonic_ratio']} | {r['stereo_correlation']} "
            f"| {'**SUSPECT** — ' + r['suspect'] if r['suspect'] else 'ok'} |"
        )
    bad = [r for r in rows if r["suspect"]]
    lines += [
        "",
        "## Entries that must not be trusted blindly",
        "",
    ]
    if bad:
        for r in bad:
            lines.append(f"- `{r['file']}` — {r['suspect']}. Do not tune a recreation to a suspect")
            lines.append("  measurement; verify by ear or re-analyse that file before using its numbers.")
    else:
        lines.append("- None: every measurement agreed with its named note.")
    lines += [
        "",
        "## How to read this",
        "",
        "- **F0** should agree with the note in the file name; a large disagreement means the detector",
        "  locked onto a harmonic rather than the fundamental.",
        "- **Attack** is the time to peak RMS. Short = percussive/plucked, longer = bowed/blown.",
        "- **Decay to -20 dB** is the body decay after the peak: short = plucked/keyed, long = sustained.",
        "- **Centroid** is the spectral centre of mass: brightness. **Rolloff85** is the frequency below",
        "  which 85% of the energy sits: useful for setting the filter cutoff target.",
        "- **Harmonic ratio** near 1.0 = strongly tonal; low = noisy or percussive.",
        "- **Stereo correlation** near 1.0 = essentially mono; lower = wide stereo content.",
        "",
        "These are the targets a recreation has to hit. They are measurements, not opinions, and they",
        "are the honest substitute for shipping the audio.",
    ]
    (ROOT / "docs" / "REFERENCE-ANALYSIS.md").write_text("\n".join(lines) + "\n")
    print(f"{len(rows)} references analysed")
    for r in rows:
        print(f"  {r['file']}: f0={r['detected_f0_hz']}Hz attack={r['attack_s']}s "
              f"decay={r['decay_to_minus20db_s']}s centroid={r['spectral_centroid_hz']}Hz "
              f"harm={r['harmonic_ratio']} corr={r['stereo_correlation']}")


if __name__ == "__main__":
    main()
