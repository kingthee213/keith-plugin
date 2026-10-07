# U10 verification record — insert and master FX

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Worker lane: `u10-fx` (Cursor CLI, GPT-5.6 Sol Extra High), pane `w17:p2`.

**Every result below was re-run by the coordinator, not taken from the worker's report.**
No commit of the worker's own, no merge, install, push or deployment.

## What changed

Keith goes from 56 to **75 live parameters**. IDs 0–55 are unchanged; 19 are appended:

`kParamIns1Algo/A/B/C/Mix/Bypass` (56–61), `kParamIns2Algo/A/B/C/Mix/Bypass` (62–67),
`kParamMstAlgo/A/B/C/Mix/Bypass` (68–73), `kParamOutLevel` (74).

New `src/fx/PatchEffects.h` (~22 KB) implements the per-patch chain BRIEF §7 requires.

- Insert algorithms: `Off / Drive / Chorus / Delay / Crush`
- Master algorithms: `Off / Comp / Limit / Width / Tilt`
- Real chain order: voices → synth bus → Insert 1 → Insert 2 → tape bus → Master → out level → gain.
- A/B/C knob labels follow the selected algorithm, so a knob never claims a job the DSP is not doing.
- Parameter changes are smoothed; `Off` and `Bypass` are exact pass-throughs (asserted).

Editor gains a material-free `07 FX` page with `INSERT 1`, `INSERT 2` and `MASTER` regions, algorithm
selectors, live A/B/C labels, mix, bypass and `OUT LEVEL`. Nav widened from six cells to seven
(`kNavCount` 7, `kNavGap` 4).

## Coordinator-verified results

| Gate | Result |
|---|---|
| Parameter IDs 0–55 vs previous commit | **byte-identical prefix**, 19 appended |
| Release build (4 targets) | clean, no errors/warnings |
| CTest | 2/2 passed |
| Offline synth tests | **76 passed, 0 failed** |
| Editor surface check | **15 passed, 0 failed** |
| Preset metadata check | PASS, emulated-family order intact |
| Material layout check | PASS, 27 placements agree |
| pluginval strictness 10 | exit 0, **SUCCESS**, Editor + Editor Automation + Open-editor-while-processing all completed |
| Protected files | `src/editor/ControlLayout.h` and `resources/images` **untouched** (`git diff` empty) |

### Live interaction

`07 FX` opens with three regions and the real algorithm sets (verified from a capture:
`computer_use_0cce2337d15540d58c30263f2875d431.png`). All seven nav labels render without clipping.

`tools/editor_input_smoke.py` was updated for seven nav cells and extended to cover the new page, then
run against the live Release standalone:

```json
{"status": "pass", "cutoff_drag": [9000.0, 4995.3], "cutoff_numeric_roundtrip": 5000.0,
 "lfo_rate_numeric_roundtrip": [1.0, 2.5], "filter_drive_numeric_roundtrip": [0.0, 25.0],
 "fx_out_level_numeric_roundtrip": [0.0, 3.0], "volume_attack_numeric_roundtrip": [8.0, 20.0]}
```

## Not host-tested

- The FX chain has never been auditioned in a DAW. pluginval proves instantiation, processing and
  editor behaviour; it does not judge how a delay or a compressor sounds.
- CC1 and mono legato remain offline-proven only (unchanged from U9).

## Still open from the wider brief

Factory content, the drum engine and the modulation matrix. These are separate stages; nothing here
claims them.
