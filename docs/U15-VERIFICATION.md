# U15 verification record — modulation matrix

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Worker lane: `u15-matrix` (Cursor CLI, GPT-5.6 Sol Extra High), pane `w17:p4`.

**Every result below was re-run by the coordinator.** No merge, install, push or deployment.

## What changed

Keith goes from 89 to **101 live parameters**. IDs 0–88 are unchanged; 12 are appended:

`kParamMod1Source/Dest/Amount` (89–91), `kParamMod2*` (92–94), `kParamMod3*` (95–97),
`kParamMod4*` (98–100).

Four matrix slots, each **source → destination → amount**, all routed in the DSP. This implements the
modulation matrix `BRIEF.md` §4.1 names — the last named synthesis feature that was missing.

Design constraints held:

- The matrix reuses the engine's existing `ModDest` set and `ApplyModRoute` helper rather than
  introducing a second destination enum, so every offered destination is genuinely routable.
- Multiple slots on one destination sum, and the summed modulation is clamped so no combination can
  destabilise the filter or produce NaN/Inf.
- Amount 0 and Source `Off` are exact no-ops.
- The pre-existing LFO 1, LFO 2 and mod-wheel routings are unchanged; the matrix is additive.

The `02 MODULATION` page's LFO region is a three-way sub-deck — **LFO 1 / LFO 2 / MATRIX** — so the
matrix gets full-size controls rather than a cramped strip.

## Coordinator-verified results

| Gate | Result |
|---|---|
| Parameter IDs vs previous commit | prefix **byte-identical**; 12 appends; 101 live total |
| Release build (4 targets) | clean, no errors/warnings |
| CTest | 3/3 passed |
| Offline synth tests | **126 passed, 0 failed** (23 of them modulation-matrix assertions) |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS — 20/21/21/20/21, palette 14 |
| Material layout check | PASS — 27 placements agree |
| pluginval strictness 10 | exit 0, **SUCCESS**, Editor + Editor Automation completed |
| Frozen assets | `src/editor/ControlLayout.h` and `resources/images` **untouched** |

### Live interaction

`02 MODULATION` shows `LFO 1`, `LFO 2` and `MATRIX` sub-tabs; the MATRIX tab opens four
source/destination modulation rows. Capture: `computer_use_b47d1e723e1743f6bb57d7fd8c7b087a.png`

## Test honesty

Each slot has a positive assertion that its destination measurably moves **and** a negative assertion
that it does not move an unrelated destination. `mod_matrix_does_not_disturb_existing_routings` proves
the pre-existing paths behave identically with the matrix at default, and `mod_new_defaults_are_inert`
proves the 103 existing presets keep their exact sound.

## Not verified

- **macOS, AU and auval: never built, never run.** `tools/build-macos.sh` exists and states in its own
  header that it has never been executed. The owner is host-testing on Mac.
- **FL Studio and Ableton: never loaded this build.** pluginval is not a DAW.
- CC1 and mono legato remain offline-proven only.
- The drum engine is synthesized and will not match the reference machines' percussion timbres.

## Still open from the wider brief

Multi-part routing (`synth_part`, 1–4 parts) and the reference machines' own per-patch FX chains
preloaded per program (BRIEF §6.4). The macOS side of BRIEF §12 is unverified by definition here.
