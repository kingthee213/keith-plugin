#!/usr/bin/env python3
"""Measure reference recordings so the synth can be tuned to match them.

Analysis only. Nothing here is shipped; the audio stays out of the repository.
Emits docs/REFERENCE-ANALYSIS.md and a JSON target table.

Usage: python tools/analyse_references.py [reference-audio-dir]
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from measure_signal import NOTE_HZ, analyse

SRC = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "reference-audio"


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
