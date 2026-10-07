# U10 — insert and master FX (real DSP)

Coordinator contract. Binding. Read `BRIEF.md` §7, `docs/PARAM-MANIFEST.md`,
`docs/ENGINE-SCOPE.md`, `docs/U9-VERIFICATION.md`, `briefs/U9-COORDINATOR-DECISIONS.md`,
`src/Keith.h`, `src/Keith_DSP.h`, `src/synth/CoreSynth.h`, `src/editor/EditorialLayout.h`,
`src/editor/EditorGeometry.h`, `tools/synth_offline_test.cpp`, `tools/editor_surface_check.py`.

## Mission

Keith has a full synth voice and a tape bus, but the per-patch FX chain that BRIEF §7 requires does
not exist. Implement it for real: two insert effects and one master effect, each a small set of
genuinely different algorithms, all automatable, all serialized in presets.

This is the Triton-family architecture BRIEF §6.4 describes — "the factory program's insert +
master FX chain is embedded per patch". Build the mechanism; the per-patch data comes later.

## Current state

- 56 live parameters, IDs 0–55. `kNumParams` = 56.
- IDs 0–42 are frozen and must never be renumbered (host automation).
- Existing: 2 morphing oscillators, sub, noise, FM, ring, hard sync, LP12/LP24/BP/HP filter,
  key tracking, drive, amp/pitch/filter envelopes, LFO, mod wheel, velocity routing, mono legato,
  and a global tape bus (`src/fx/TapeAnalog.h`, seeded from MIT Airwindows ideas).
- Editor pages: 01 Source, 02 Modulation, 03 Filter, 04 Envelopes, 05 Tape, 06 Library.
- `docs/PARAM-MANIFEST.md` already lists the target rows for this unit as `not yet implemented`:
  `fx_ins1_*`, `fx_ins2_*`, `fx_mst_*`, `fx_out_level`.

## Parameters to add (append only, IDs 56 onward)

19 appends, `kNumParams` = 75:

| Enum | Label | Range |
|---|---|---|
| `kParamIns1Algo` | Ins1 Algo | `Off / Drive / Chorus / Delay / Crush` |
| `kParamIns1A` | Ins1 A | 0…100 % |
| `kParamIns1B` | Ins1 B | 0…100 % |
| `kParamIns1C` | Ins1 C | 0…100 % |
| `kParamIns1Mix` | Ins1 Mix | 0…100 % |
| `kParamIns1Bypass` | Ins1 Byp | `Active / Bypass` |
| `kParamIns2Algo` … `kParamIns2Bypass` | Ins2 * | same shape |
| `kParamMstAlgo` | Mst Algo | `Off / Comp / Limit / Width / Tilt` |
| `kParamMstA` | Mst A | 0…100 % |
| `kParamMstB` | Mst B | 0…100 % |
| `kParamMstC` | Mst C | 0…100 % |
| `kParamMstMix` | Mst Mix | 0…100 % |
| `kParamMstBypass` | Mst Byp | `Active / Bypass` |
| `kParamOutLevel` | Out Level | −inf…+12 dB |

Host automatable names: `FX Insert1 Algo`, `FX Insert1 A`…`FX Insert1 Bypass`, likewise `Insert2`,
and `FX Master Algo`…`FX Master Bypass`, `FX Out Level`. Match `docs/PARAM-MANIFEST.md` row ids.

**A/B/C must be honestly labelled per algorithm.** A knob that changes meaning when the algorithm
changes is fine, but the editor must show the current meaning, not a fixed guess. Label the three
knobs from the selected algorithm (e.g. Drive: `A=Drive B=Tone C=Bias`; Chorus: `A=Rate B=Depth
C=Spread`; Delay: `A=Time B=Feedback C=Tone`; Crush: `A=Bits B=Rate C=Jitter`). Choose names that
actually describe what the DSP does — do not invent a control the code ignores.

## DSP requirements

- Chain order: voices → synth bus → **Insert 1 → Insert 2** → tape bus → **Master FX** → output
  level → existing gain. Say so in the docs; do not leave the order implied.
- Every algorithm must be a real, distinct DSP effect. No stub that returns its input.
- Parameter changes must be click-free during playback (smooth the coefficients; a per-sample
  one-pole on changed values is enough).
- All processing stays finite: no NaN/Inf, and self-oscillating feedback must be bounded.
- Mst Mix and each insert Mix must be a true dry/wet, and `Off`/`Bypass` must be an exact pass-through
  (prove it in a test).
- Keep it CPU-sane: the delay buffers must be preallocated, no allocation in `ProcessBlock`.

## Editor

- Add an `07 FX` page, material-free, following the existing pattern for new pages.
  The page already has precedent: new knobs get a flat native body; nav is six cells in
  `kNavBar`, so **verify the nav still fits at seven** and adjust width/gap in `EditorGeometry.h`
  if the labels clip — the nav was already sized for six.
- Group as three named regions: `INSERT 1`, `INSERT 2`, `MASTER`. Each shows its algorithm selector,
  the three per-algorithm knobs with their live labels, mix, and a bypass tab.
- `OUT LEVEL` is a master-stage control and belongs on this page.
- Nothing new may be drawn on a baked material page, and `src/editor/ControlLayout.h` plus the
  material PNGs stay untouched.

## Presets and docs

- All 19 defaults must be inert: `Off` algorithms, `Active`/`Bypass` as chosen so the 15 existing
  presets keep their sound exactly. **Prove it** with a test like U9's `new_defaults_are_inert`.
- Add deltas to a few presets where a real chain is musically meaningful (e.g. a delay on a pad, a
  drive on a kit) to prove serialization through the host preset path.
- Update `docs/PARAM-MANIFEST.md`: flip the `fx_ins1_*`, `fx_ins2_*`, `fx_mst_*` and `fx_out_level`
  rows to `live`, add a U10 code surface table, update the counts.
- Update `docs/ENGINE-SCOPE.md` §7 with the implemented algorithm list and the real chain order.

## Tests (all in `tools/synth_offline_test.cpp`)

Per algorithm, a measurable assertion plus a negative one: e.g. `drive_adds_harmonics`,
`chorus_modulates_delay`, `delay_repeats_at_set_time`, `crush_quantises_levels`,
`comp_reduces_dynamic_range`, `limit_bounds_peak`, `width_changes_stereo_correlation`,
`tilt_changes_spectral_balance`. Plus:

- `fx_off_is_exact_passthrough` and `fx_bypass_is_exact_passthrough`
- `fx_chain_all_max_is_finite`
- `fx_new_defaults_are_inert`
- existing tests keep passing (currently 50 passing; the count will rise)

## Verification — run unpiped, preserve the real output

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

`pluginval` must end in `SUCCESS` with Editor and Editor Automation included.

## Hard boundaries

- Append parameter IDs only. Never renumber 0–55.
- Do not touch `src/editor/ControlLayout.h`, the material PNGs under `resources/images/`, or
  `D:/TYH_R10/dev/keith-plugin`.
- Replace the tape bus? **No.** Leave `TapeAnalog` alone; this unit adds the insert/master stages.
- Do not add drum or factory-content controls in this unit; they are separate stages.
- No commit, merge, push, install or deploy.
- Report separately: implemented and verified / implemented but not host-tested / deferred / blocked.
