# Preset level alignment

Every factory preset now carries a measured level trim. This records what was measured,
what changed, and what is still wrong.

## What was wrong

Only 32 of 109 presets set a level at all, and the values that were set came from ear. The
measured result:

| | before | after |
|---|---|---|
| momentary loudness spread | **46.86 dB** | **7.92 dB** |
| loudest preset | +11.23 LUFS | -11.99 LUFS |
| quietest preset | -35.63 LUFS | -19.91 LUFS |
| presets clipping above 0 dBFS | **12** | **0** |
| worst sample peak | **+17.45 dBFS** | -1.50 dBFS |
| presets at the -12 LUFS target within 0.01 dB | 0 | **78** |

Four presets, all basses, were peaking between +12 and +17.5 dBFS. That is not a loud
patch, it is a clipped one, and it was the main reason switching patches by hand sounded
like the volume was jumping around.

## How it is measured

`tools/measure_preset_levels.py measure` renders every preset through the plugin itself
(the same path the recreation gate uses) at C2, C4 and C5, holds each note for three
seconds, and for each render reports the sample peak in dBFS and the loudest 100 ms
K-weighted window in LUFS per ITU-R BS.1770-4. The loudest of the three notes is that
preset's level.

Two choices worth stating, because they are not the textbook defaults:

- **A 100 ms loudness window, not the standard 400 ms.** BS.1770's 400 ms window is for
  programme loudness. A single held note is not a programme, and a 400 ms window averages
  a short percussive hit away. With a 400 ms window a kick drum that peaks at -9 dBFS
  reads about 22 dB quieter than it does through a 100 ms one, which makes a hit and a pad
  impossible to compare on one number.
- **Peak is a ceiling, not a target.** Each trim is the smaller of what loudness wants and
  what the peak ceiling allows, so no trim can push a preset into clipping. The ceiling is
  -1.5 dBFS sample peak, which is roughly -1 dBTP once inter-sample peaks are counted.

The K-weighting is checked against a known signal: a 1 kHz sine at -20 dBFS RMS measures
-19.99 LUFS, and at -12 dBFS RMS measures -11.99 LUFS.

## Where the trim is applied, and why there

The signal path is

```
voices -> synth bus -> Insert 1 -> Insert 2 -> tape bus -> Master -> output level -> gain
```

The trim goes on **output level**, because that stage sits after every nonlinear stage, so
a change there is a pure level change. Synth bus level is before the inserts, the tape bus
and the master block, so a change there also changes how hard those stages are driven: the
first attempt applied the whole trim on the synth bus and the measured result did not match
the arithmetic prediction at all. Synth bus level only carries whatever overflow the output
range cannot hold, which is needed for the handful of presets far below the pack.

`tools/level_iterate.sh` runs the loop, two passes by default. Each pass measures the state
the previous pass built and corrects the real measurement rather than trusting a prediction.
It resets the generated block first, so it is safe to re-run after any preset change.

## What is still wrong

28 presets sit at the -1.5 dBFS peak ceiling, which means they are as loud as they can be
without clipping. 14 of those land more than 3 dB below the -12 LUFS target, between -15.1
and -19.9 LUFS. They are all short percussive patches, stabs, plucks, arps and kits, whose
peak-to-loudness ratio is large: one hard transient and very little sustain.

That is not fixable with a level control. A higher level would clip. The fix is in the
patch, and it belongs to a content pass: less drive into the tape and master stages, or a
faster transient, so the peak and the body sit closer together. The list is in
`docs/level-measurements-after.tsv`, marked by a peak at -1.50.

## Reproducing

```
bash tools/level_iterate.sh 2
```

Files: `docs/level-measurements-before.tsv` is the state before any trim,
`docs/level-measurements-after.tsv` is the shipped state. Both are the raw per-preset
measurements, not summaries.
