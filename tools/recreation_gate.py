#!/usr/bin/env python3
"""Render each analysis recreation through the plugin and remeasure it.

The measurements are tools/measure_signal.py, the same module
tools/analyse_references.py imports. There is no second implementation.
Tolerances are the constants in that module. This script does not widen them.

Usage:
  python tools/recreation_gate.py <path-to-recreation_render>
"""
import subprocess
import sys
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from measure_signal import (  # noqa: E402
    ATTACK_ABS_S,
    ATTACK_REL,
    CENTROID_ABS_HZ,
    CENTROID_REL,
    DECAY_ABS_S,
    DECAY_REL,
    F0_REL,
    measure_stereo,
    within,
)

# note, fundamental Hz, attack s, decay-to--20 dB s, centroid Hz, render seconds
# Numbers are the measured references. Harmonic ratio and stereo correlation are
# printed beside the four gated columns; they are not given a looser pass.
RECREATIONS = [
    ("Family 01 / 021 Upright Bass", [
        (36, 65.4, 0.116, 3.48, 402.0, 5.2),
        (48, 130.7, 0.023, 2.15, 655.0, 3.6),
        (60, 261.5, 0.029, 1.59, 1156.0, 3.6),
    ]),
    ("Family 01 / 022 Hammer Keys", [
        (24, 32.6, 0.099, 1.75, 683.0, 3.4),
        (48, 130.8, 0.017, 4.69, 1283.0, 6.4),
        (72, 523.2, 0.012, 2.33, 2793.0, 4.2),
    ]),
    ("Family 01 / 023 Swell Choir", [
        (60, 262.2, 3.1, 1.1, 1477.0, 5.6),
    ]),
    ("Family 01 / 024 Bright Reed", [
        (72, 523.0, 0.070, 1.45, 5506.0, 3.2),
    ]),
    ("Family 01 / 025 Snap Bass", [
        (36, 65.4, 0.012, 0.62, 285.0, 2.2),
    ]),
    ("Family 01 / 026 Short Bass", [
        (48, 130.7, 0.012, 0.70, 481.0, 2.4),
    ]),
]


def read_f32(path):
    raw = Path(path).read_bytes()
    sr = int(np.frombuffer(raw[:4], dtype="<i4")[0])
    frames = int(np.frombuffer(raw[4:8], dtype="<i4")[0])
    audio = np.frombuffer(raw[8:], dtype="<f4").reshape(-1, 2)[:frames]
    return audio[:, 0].astype(np.float64), audio[:, 1].astype(np.float64), sr


def main():
    if len(sys.argv) != 2:
        print("usage: recreation_gate.py <recreation_render>", file=sys.stderr)
        return 2
    exe = Path(sys.argv[1])
    if not exe.is_file():
        print(f"renderer missing: {exe}", file=sys.stderr)
        return 2

    work = ROOT / "build" / "recreation-gate"
    work.mkdir(parents=True, exist_ok=True)
    failed = 0
    rows = 0
    print(f"{'preset':<28} {'note':>4} {'metric':<8} {'measured':>10} {'target':>10} {'tol':>8} result")
    for name, notes in RECREATIONS:
        for note, f0, attack, decay, centroid, seconds in notes:
            out = work / f"{name.split('/')[-1].strip().replace(' ', '_')}_{note}.f32"
            subprocess.check_call(
                [str(exe), name, str(note), str(seconds), str(out)],
                stdout=subprocess.DEVNULL,
            )
            left, right, sr = read_f32(out)
            measured = measure_stereo(left, right, sr)
            checks = (
                ("f0_hz", measured["detected_f0_hz"], f0, 0.0, F0_REL),
                ("attack_s", measured["attack_s"], attack, ATTACK_ABS_S, ATTACK_REL),
                ("decay_s", measured["decay_to_minus20db_s"], decay, DECAY_ABS_S, DECAY_REL),
                ("centroid", measured["spectral_centroid_hz"], centroid, CENTROID_ABS_HZ, CENTROID_REL),
            )
            for metric, got, target, abs_tol, rel_tol in checks:
                ok, limit = within(got, target, abs_tol, rel_tol)
                rows += 1
                failed += not ok
                print(
                    f"{name:<28} {note:4d} {metric:<8} {got:10.3f} {target:10.3f} {limit:8.3f} "
                    f"{'PASS' if ok else 'FAIL'}",
                    flush=True,
                )
            print(
                f"{name:<28} {note:4d} {'harm':<8} {measured['harmonic_ratio']:10.3f} {'':10} {'':8} report",
                flush=True,
            )
            print(
                f"{name:<28} {note:4d} {'corr':<8} {measured['stereo_correlation']:10.3f} {'':10} {'':8} report",
                flush=True,
            )
    print(f"recreation_gate: {rows - failed} passed, {failed} failed (total {rows})")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
