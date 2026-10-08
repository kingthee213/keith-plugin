"""Shared measurements for reference recordings and rendered recreations.

tools/analyse_references.py and the recreation gate both call this module.
There is one implementation, so the two cannot drift.
"""
import numpy as np

NOTE_HZ = {
    "C1": 32.70, "C2": 65.41, "C3": 130.81, "C4": 261.63, "C5": 523.25,
}

# Stated before any recreation was tuned. They follow the analyser, not a miss.
# RMS hop is 256 samples (5.3 ms at 48 kHz). Attack tolerance covers two hops,
# or 8% when the attack is long. Decay uses the same 8% with a 40 ms floor.
# Centroid uses 8% with a 40 Hz floor. Fundamental stays inside 1.5%, under the
# 6% octave-lock threshold the reference tool already treats as suspect.
F0_REL = 0.015
ATTACK_ABS_S = 0.012
ATTACK_REL = 0.08
DECAY_ABS_S = 0.040
DECAY_REL = 0.08
CENTROID_ABS_HZ = 40.0
CENTROID_REL = 0.08


def within(measured, target, abs_tol, rel_tol):
    limit = max(abs_tol, abs(target) * rel_tol)
    return abs(measured - target) <= limit, limit


def read_wav(path):
    import wave
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
        denom = (np.sqrt(np.sum(left ** 2)) * np.sqrt(np.sum(right ** 2))) or 1.0
        corr = float(np.sum(left * right) / denom)
    else:
        mono, corr = a, 1.0
        left = right = mono
    return mono, sr, corr, left, right


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


def measure_mono(mono, sr, corr=1.0):
    """Same body the reference tool uses, on an in-memory buffer."""
    x = np.asarray(mono, dtype=np.float64)
    dur = len(x) / sr
    env, env_sr = rms_envelope(x, sr)
    peak = int(np.argmax(env)) if env.size else 0
    peak_val = float(env[peak]) if env.size else 0.0
    attack_s = peak / env_sr if env_sr else 0.0
    target = peak_val * 0.1
    after = np.where(env[peak:] <= target)[0]
    decay_s = float(after[0] / env_sr) if after.size else float(dur - attack_s)

    body_start = int(min(len(x) - 1, max(attack_s, 0.02) * sr))
    body = x[body_start:]
    if body.size < 64:
        body = x[: min(len(x), sr)]

    win = min(len(body), sr)
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
    return {
        "duration_s": dur,
        "detected_f0_hz": f0,
        "attack_s": attack_s,
        "decay_to_minus20db_s": decay_s,
        "spectral_centroid_hz": centroid,
        "spectral_rolloff85_hz": rolloff,
        "harmonic_ratio": harm,
        "stereo_correlation": corr,
    }


def stereo_correlation(left, right):
    left = np.asarray(left, dtype=np.float64)
    right = np.asarray(right, dtype=np.float64)
    denom = (np.sqrt(np.sum(left ** 2)) * np.sqrt(np.sum(right ** 2))) or 1.0
    return float(np.sum(left * right) / denom)


def measure_stereo(left, right, sr):
    left = np.asarray(left, dtype=np.float64)
    right = np.asarray(right, dtype=np.float64)
    mono = 0.5 * (left + right)
    corr = stereo_correlation(left, right)
    return measure_mono(mono, sr, corr)


def analyse(path):
    import wave
    from pathlib import Path
    path = Path(path)
    mono, sr, corr, _left, _right = read_wav(path)
    dur = len(mono) / sr
    name = path.stem
    guess = next((k for k in NOTE_HZ if name.upper().endswith(k)), None)
    row = measure_mono(mono, sr, corr)
    f0 = row["detected_f0_hz"]
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
    harm = row["harmonic_ratio"]
    if harm < 0.10 and not suspect:
        suspect = "harmonic ratio implausibly low for a keyed instrument - treat as unreliable"
    return {
        "file": path.name,
        "duration_s": round(dur, 2),
        "detected_f0_hz": round(f0, 1),
        "named_note": guess,
        "named_note_hz": NOTE_HZ.get(guess) if guess else None,
        "attack_s": round(row["attack_s"], 3),
        "decay_to_minus20db_s": round(row["decay_to_minus20db_s"], 2),
        "spectral_centroid_hz": round(row["spectral_centroid_hz"], 0),
        "spectral_rolloff85_hz": round(row["spectral_rolloff85_hz"], 0),
        "harmonic_ratio": round(harm, 3),
        "stereo_correlation": round(corr, 3),
        "suspect": suspect,
    }
