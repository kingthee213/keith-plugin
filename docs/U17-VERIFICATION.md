# U17 verification record — algorithmic recreations

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Lane: `u17-grok` (Cursor CLI, Grok 4.7 Extra High), pane `w17:p7`.

**Every result below was re-run by the coordinator.** No merge, install, push or deployment.

## What the owner asked for, and what was built

Owner direction, verbatim:

> "but we dont need a sampler, just use the audio to recreate the sounds algorithmically"

So the reference recordings are **measurement targets**, not shipped content. **No sampler was built,
no audio is in the repository, and nothing is redistributed** — which also dissolves the recording
site's licence question entirely, because nothing is redistributed. `reference-audio/` is gitignored,
`git ls-files` contains **zero** audio files.

## Reference analysis (coordinator-owned)

`tools/analyse_references.py` measures each reference into synthesis targets: fundamental, attack,
decay to −20 dB, spectral centroid, rolloff85, harmonic ratio, stereo correlation — averaged across the
sustain rather than one window. Results in `docs/REFERENCE-ANALYSIS.md`.

The tool reports its own unreliable measurements rather than presenting them as fact. Single-window
analysis produced three wrong numbers, caught and corrected by going multi-window:

- Stein Bass C3 detected as 43.6 Hz — actually **130.7 Hz** (a detector error).
- Piano C5 harmonic ratio 0.017 — actually **0.406**.
- Harmonica C5 still flagged: **an x3 harmonic lock**, so nobody tunes to its numbers.

## Engine capability added (10 appends, IDs 101–110)

The measured targets required capability the engine did not have, so it was implemented rather than
faked:

`kParamNoiseKey`, `kParamAmpAttackKey`, `kParamAmpAttackCurve`, `kParamAmpDecayKey`,
`kParamAmpDecayCurve`, `kParamStereoDelay`, `kParamStereoDelayKey`, `kParamStereoSpread`,
`kParamDirectLevel`, `kParamFilterKeyCurve`.

These give per-pitch brightness (the piano target needs the centroid to rise ~683 Hz → 2793 Hz while
the harmonic ratio falls 0.697 → 0.406) and a stereo path (the choir target has correlation 0.732 and
Stein Bass C2 is −0.16, i.e. genuinely out of phase).

## Recreations shipped

Three, each gated, rather than ten unverified ones:

| Preset | Reference | Notes gated |
|---|---|---|
| `Family 01 / 021 Upright Bass` | acoustic bass C2/C3/C4 | 3 |
| `Family 01 / 022 Hammer Keys` | acoustic piano C1/C3/C5 | 3 |
| `Family 01 / 023 Swell Choir` | slow choir C4 | 1 |

Preset library: **106** (was 103).

## The acceptance gate — objective, and tight

`tools/recreation_gate.py` renders each recreation through the plugin's own engine, then runs **the same
measurement code** the analysis tool uses (`tools/measure_signal.py`), so the two cannot drift. It
asserts each recreation lands within tolerance of its measured target.

Tolerances are **not** loosened to force a pass:

| Metric | Absolute | Relative |
|---|---|---|
| Fundamental | — | **1.5 %** |
| Attack | 0.012 s | 8 % |
| Decay to −20 dB | 0.040 s | 8 % |
| Spectral centroid | 40 Hz | 8 % |

Harmonic ratio and stereo correlation are printed beside the gated columns but **not** given a looser
pass, exactly as the brief required.

Measured evidence, from the coordinator's own run:

```
Family 01 / 021 Upright Bass   36 f0_hz     65.410   65.400   0.981 PASS
Family 01 / 021 Upright Bass   36 attack_s   0.123    0.116   0.012 PASS
Family 01 / 021 Upright Bass   36 decay_s    3.552    3.480   0.278 PASS
Family 01 / 021 Upright Bass   36 centroid 423.940  402.000  40.000 PASS
Family 01 / 021 Upright Bass   48 f0_hz    130.820  130.700   1.960 PASS
Family 01 / 021 Upright Bass   60 f0_hz    261.637  261.500   3.922 PASS
Family 01 / 021 Upright Bass   60 decay_s    1.637    1.590   0.127 PASS
```

Zero FAIL rows. `Family 01 / 021` matches the acoustic bass reference to **0.015 %** on fundamental.

## Coordinator-verified results

| Gate | Result |
|---|---|
| Parameter IDs vs previous commit | prefix **byte-identical**; 10 appends; **111 live** |
| Release build (4 targets) | clean |
| CTest | **4/4**, including `recreation-gate` (695 s) |
| Offline synth tests | **131 passed, 0 failed** |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS — 23/21/21/20/21 |
| FX chain checker | **14 passed, 0 failed** |
| Material layout check | PASS |
| Audio files tracked in git | **0** |
| `reference-audio/` ignored | yes |
| Frozen assets | `ControlLayout.h`, `resources/images` **untouched** |

## Not done

- **Seven of the ten references are not yet recreated.** Reported as deferred, not silently dropped.
- **The harmonica cannot be gated** until its measurement is resolved (flagged suspect).
- macOS/AU/auval: still never built or run. FL Studio and Ableton: still never loaded this build.
- CC1 and mono legato remain offline-proven only.

## Usage note

GPT-5.6 Sol hit its monthly model cap mid-run (resets 10/12) having written **zero files**; the pass was
restarted on Grok 4.7 xhigh. Nothing was lost or half-applied.
