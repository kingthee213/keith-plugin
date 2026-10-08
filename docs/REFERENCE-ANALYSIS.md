# Reference analysis — synthesis targets

Measured from reference recordings held in a scratch directory that is **not** part of the
build and **not** shipped. Only the measurements below are used; the audio never enters the
plugin, the repository, or the package.

Measured by `tools/analyse_references.py` (numpy, no external DSP dependency).

| File | Dur s | F0 Hz | Note | Attack s | Decay to -20 dB s | Centroid Hz | Rolloff85 Hz | Harmonic ratio | Stereo corr | Reliability |
|---|---|---|---|---|---|---|---|---|---|---|
| Korg-Triton-Acoustic-Bass-C2.wav | 10.79 | 65.4 | C2 | 0.116 | 3.48 | 402.0 | 130.0 | 0.941 | 0.999 | ok |
| Korg-Triton-Acoustic-Bass-C3.wav | 8.09 | 130.7 | C3 | 0.023 | 2.15 | 655.0 | 261.0 | 0.893 | 0.996 | ok |
| Korg-Triton-Acoustic-Bass-C4.wav | 6.93 | 261.5 | C4 | 0.029 | 1.59 | 1156.0 | 523.0 | 0.815 | 0.983 | ok |
| Korg-Triton-Acoustic-Piano-C1.wav | 13.81 | 32.6 | C1 | 0.099 | 1.75 | 683.0 | 1007.0 | 0.697 | 0.974 | ok |
| Korg-Triton-Acoustic-Piano-C3.wav | 12.87 | 130.8 | C3 | 0.017 | 4.69 | 1283.0 | 2118.0 | 0.648 | 0.953 | ok |
| Korg-Triton-Acoustic-Piano-C5.wav | 10.34 | 523.2 | C5 | 0.012 | 2.33 | 2793.0 | 4306.0 | 0.406 | 0.994 | ok |
| Korg-Triton-Harmonica-C5.wav | 2.27 | 174.3 | C5 | 0.07 | 1.45 | 5506.0 | 9414.0 | 0.38 | 0.987 | **SUSPECT** — octave/harmonic lock x3 vs named note (expected 523.25 Hz) |
| Korg-Triton-Slow-Choir-ST-C4.wav | 5.7 | 262.2 | C4 | 3.1 | 1.1 | 1477.0 | 2682.0 | 0.122 | 0.732 | ok |
| Korg-Triton-Stein-Bass-C2.wav | 1.3 | 65.4 | C2 | 0.012 | 0.62 | 285.0 | 328.0 | 0.529 | -0.16 | ok |
| Korg-Triton-Stein-Bass-C3.wav | 1.25 | 130.7 | C3 | 0.012 | 0.7 | 481.0 | 652.0 | 0.563 | 0.324 | ok |

## Entries that must not be trusted blindly

- `Korg-Triton-Harmonica-C5.wav` — octave/harmonic lock x3 vs named note (expected 523.25 Hz). Do not tune a recreation to a suspect
  measurement; verify by ear or re-analyse that file before using its numbers.

## How to read this

- **F0** should agree with the note in the file name; a large disagreement means the detector
  locked onto a harmonic rather than the fundamental.
- **Attack** is the time to peak RMS. Short = percussive/plucked, longer = bowed/blown.
- **Decay to -20 dB** is the body decay after the peak: short = plucked/keyed, long = sustained.
- **Centroid** is the spectral centre of mass: brightness. **Rolloff85** is the frequency below
  which 85% of the energy sits: useful for setting the filter cutoff target.
- **Harmonic ratio** near 1.0 = strongly tonal; low = noisy or percussive.
- **Stereo correlation** near 1.0 = essentially mono; lower = wide stereo content.

These are the targets a recreation has to hit. They are measurements, not opinions, and they
are the honest substitute for shipping the audio.
