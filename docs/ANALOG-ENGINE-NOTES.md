# Analog engine verification notes (W1)

Measured on 2026-10-08 from `AnalogComparePatch` in `tools/synth_offline_test.cpp`
(note 48, velocity 0.9, 48 kHz, hold window samples `sr/8` .. `sr/8 + sr/2`).

## Command

```bash
cd /Users/solace/wt-analog
export PATH="$HOME/cmakebin:$PATH"
cmake -S . -B build -G Xcode
cmake --build build --config Release --target keith-synth-tests
./build/Release/keith-synth-tests
```

All W1 assertions are in the `// W1 analog engine verification contract` block at the
end of `tools/synth_offline_test.cpp`.

## Sub-120 Hz energy (margin ≥ 1.35×)

| Engine | Goertzel sum (35–115 Hz) |
|--------|--------------------------|
| Hybrid | 401.491 |
| Analog | 638.918 |
| Ratio  | **1.591×** |

## Idle noise floor (Analog > Hybrid, Analog > −90 dBFS)

| Engine | RMS dBFS (no notes, 0.5 s) |
|--------|----------------------------|
| Hybrid | −∞ (digital silence) |
| Analog | **−47.64 dBFS** |

## Spectral centroid (Analog darker = lower Hz)

| Engine | Centroid (Hz) |
|--------|---------------|
| Hybrid | 298.18 |
| Analog | **230.35** |

## Peak level (Analog < 0 dBFS at full master)

| Engine | Peak |
|--------|------|
| Analog | **0.4979** (−6.1 dBFS) |

## Hybrid regression hash (byte-identical proof)

Fixed patch: `HybridRegressionPatch()` in `tools/synth_offline_test.cpp`.

| Metric | Value |
|--------|-------|
| FNV-1a hash | `15578503562098217975` |
| Samples | 84000 |
| Assertion | `hybrid_regression_hash_unchanged` |

## OB-Xf adaptations (GPL-3.0)

| Keith symbol | OB-Xf source |
|--------------|--------------|
| `LadderFilter::Process` | `Filter::apply4Pole`, `Filter::resolveFeedback4Pole`, `AudioUtils.h` `tpt_process_scaled_cutoff` — `src/engine/Filter.h`, `src/engine/AudioUtils.h` |
| Per-voice pitch drift | `OscillatorBlock::ProcessSample` `oscPitchNoise * gen.noise.getWhite()` — `src/engine/OscillatorBlock.h` |
| Per-voice detune on note-on | `Voice` constructor `slop.*` random offsets — `src/engine/Voice.h` |
| HF rolloff (`AnalogBrightness`) | `Voice::setBrightness` / `tpt_process` — `src/engine/Voice.h`, `src/engine/AudioUtils.h` |

Hybrid path is unchanged: TPT SVF `MultiFilter` only, no analog ladder/sub/drift/floor.
