# Preset deltas: read before editing `kPresetDeltas`

## The duplicates are load-bearing. Do not "dedupe" them.

`src/Keith.cpp` contains 29 repeated `(preset, kParamSynthMasterLevel)` rows. They look like a
mistake. They are not, and removing them changes the sound of five presets.

The pattern is:

```cpp
{21, kParamSynthMasterLevel, -13.15},            // <- placeholder, written when the preset was authored
...
{21, kParamOutLevel, +12.00}, {21, kParamSynthMasterLevel, -12.55},  // measured -12.00 LUFS, applied -0.00 dB
                                                 // ^ the MEASURED trim, appended later
```

This is an **array of structs**, so a repeated pair is not a compile error — the **last write
wins**. The second row is the one that actually applies. The first is a placeholder left behind.

For presets 21, 22, 23, 39 and 81 the two values **differ**, and the later measured one is
authoritative:

| Preset | Placeholder | Measured (authoritative) |
|---:|---:|---:|
| 21 | −13.15 | **−12.55** |
| 22 | −10.46 | **−6.29** |
| 23 | −12.00 | **−11.69** |
| 39 | −4.00 | **−1.57** |
| 81 | −6.00 | **−4.61** |

For the other 24 presets both copies hold the same value, so either survives.

**A "keep the first occurrence" dedupe silently corrupts those five.** That was tried during the
2026-10-11 audit and had to be reverted.

## `verify_preset_metadata` already fails on this

```
FAIL duplicate delta for preset 1 kParamSynthMasterLevel
...
verify_preset_metadata: 0 passed, 29 failed
```

**This failure predates the sampler work** — it reproduces on the committed `w7-composition` HEAD
with the working tree stashed. It is not caused by anything in the sampler branch.

The gate is doing its job by reporting the duplication. The right fix is not to delete rows but to
**collapse each pair to its authoritative value**, then remove the duplicate — and the measurement
comments on the lines are the source of truth for which is which. That has not been done yet.

## Related

- `tools/verify_preset_metadata.py` — the gate that reports this.
- `tools/measure_preset_levels.py` — writes the measured trims. Needs a render binary.
- `tools/check_patch_ranges.py` — the range gate; all 5864 values pass.
