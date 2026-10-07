# U9 verification record — control-surface expansion

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Worker lane: `keith-grok-u9` (Cursor CLI, Grok 4.7 Extra High), pane `w17:p1`,
workspace `keith-u9-deepseek`.

**Every result below was re-run by the coordinator, not taken from the worker's report.**
No merge, install, push or deployment was performed. Work is uncommitted.

## What changed

Keith goes from 43 to **56 live parameters**. IDs 0–42 are unchanged; 13 are appended:

| ID | Enum | Label |
|---|---|---|
| 43 | `kParamOscSync` | Osc Sync |
| 44 | `kParamOscRing` | Osc Ring |
| 45 | `kParamFilterKeyTrack` | Filter Key Track |
| 46 | `kParamFilterDrive` | Filter Drive |
| 47 | `kParamLfoRate` | LFO Rate |
| 48 | `kParamLfoShape` | LFO Shape |
| 49 | `kParamLfoDest` | LFO Dest |
| 50 | `kParamLfoAmount` | LFO Amount |
| 51 | `kParamModWheelDest` | Mod Dest |
| 52 | `kParamModWheelAmount` | Mod Amount |
| 53 | `kParamVelToAmp` | Vel To Amp |
| 54 | `kParamVelToFilter` | Vel To Filter |
| 55 | `kParamMonoLegato` | Mono Legato |

New DSP: oscillator hard sync, ring modulation, filter key tracking, filter drive,
one per-voice LFO (rate / shape / one destination / bipolar amount), mod-wheel
capture from CC1 with destination and amount, velocity to amp, velocity to filter,
and mono legato.

Pages are now `01 SOURCE`, `02 MODULATION`, `03 FILTER`, `04 ENVELOPES`,
`05 TAPE`, `06 LIBRARY`. The two new pages are material-free; new knobs carry a
flat native body. The permanent FILTER/VOICE channel is unchanged.

Typography: the title/heading voice is now **Oswald Bold**, a static OFL-1.1
condensed grotesque (`resources/fonts/Oswald-Bold.ttf`, notice in
`resources/fonts/OFL.txt`). Space Mono remains the face for values, labels and
readouts. Bangers stays embedded under its own font id but is no longer used for
the wordmark or headings.

## Coordinator-verified results

| Gate | Command | Result |
|---|---|---|
| Release build | `cmake --build build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests` | clean, 4 targets, no errors/warnings |
| CTest | `ctest --test-dir build -C Release --output-on-failure` | 2/2 passed |
| Offline synth tests | `build/Release/keith-synth-tests.exe` | **50 passed, 0 failed** |
| Editor surface | `python tools/editor_surface_check.py` | **14 passed, 0 failed** |
| Preset metadata | `python tools/verify_preset_metadata.py` | PASS, emulated-family order intact |
| Material layout | `python tools/verify_material_layout.py` | PASS, 27 placements agree |
| pluginval | strictness 10, `--validate-in-process` | exit 0, **SUCCESS**, Editor + Editor Automation + Open-editor-whilst-processing all completed |

### Parameter contract (independently diffed against the original checkout)

- IDs 0–42 are byte-identical to `D:/TYH_R10/dev/keith-plugin/src/Keith.h`. Verified by parsing both enums.
- 13 appends, contiguous, `kNumParams` = 56.
- `editor_surface_check.py` now also proves each live parameter is attached **exactly once**.

### Frozen assets

- `git diff --stat -- src/editor/ControlLayout.h resources/images` is **empty** — the render
  placements and every material PNG are untouched.
- Neither binary contains an `fvar` tag, confirming the shipped font is static, not variable.

### Live interaction (coordinator-driven, actual Release standalone)

Driven with targeted Win32 messages to the real `IPlugWndClass` child of the running exe:

- `02 MODULATION` opens and shows LFO (Rate 1.00 Hz, Amount 0.0%, SINE/TRI/SAW/SQUARE/S&H,
  OFF/CUTOFF/PITCH/SHAPE/LEVEL), MOD WHEEL (Amount, same destinations) and
  VELOCITY (TO AMP, TO FILTER, RETRIG/LEGATO). Capture:
  `computer_use_41ff7887d52b42239f6e6f9bce34fd32.png`
- `03 FILTER` opens and shows a live RESPONSE curve plus KEY TRACK and DRIVE. Capture:
  `computer_use_2669d2d152014ce7a0a984ef4af00972.png`
- LFO Rate readout opens a real numeric field and reads back `1.00`; Escape closes it.
- LFO Rate knob drag changed the value **1.00 → 30.00 Hz** (correct clamp at the 30 Hz ceiling).
- `tools/editor_input_smoke.py` ran end to end against the live six-page Release standalone,
  walking Source → Modulation → Filter → Envelopes and exercising both new U9 controls:

```json
{"status": "pass", "cutoff_drag": [9000.0, 4995.3], "cutoff_numeric_roundtrip": 5000.0,
 "lfo_rate_numeric_roundtrip": [1.0, 2.5], "filter_drive_numeric_roundtrip": [0.0, 25.0],
 "volume_attack_numeric_roundtrip": [8.0, 20.0]}
```

  This closes the editor-interaction gap the worker reported as untested. It proves numeric entry
  and drag on a new LFO control and a new filter control on their new pages, plus the pre-existing
  cutoff and envelope interactions.

### Test honesty

Every new control has a positive assertion plus at least one negative assertion that it does not
touch an unrelated destination — e.g. `lfo_pitch_does_not_tremolo`,
`vel_to_amp_does_not_change_pitch`, `mod_wheel_level_does_not_change_pitch`,
`key_track_does_not_move_middle_c`, `filter_drive_does_not_retune`.
`new_defaults_are_inert` proves all 13 defaults are no-ops, so the 15 existing presets keep
their sound.

## Branding pass (owner instruction, after U9)

UI display name changed to **Yote Machine** from **The Youth Hostel DSP**. Repo, binary, plugin name
and preset naming are deliberately unchanged.

- Wordmark is now `YOTE MACHINE` in Oswald; it fits the wordmark box with clear margin (verified by
  capture, final `E` unobstructed).
- The separate subtitle line was **removed** and merged into one stamped magenta service plate
  (`THE YOUTH HOSTEL DSP` / `FIVE SOURCE FAMILIES`). The old arrangement stacked a subtitle and the
  plate in a band with no room for both, and the plate clipped the subtitle's descenders — the
  overlap was found by inspecting a zoomed capture, not from source.
- Plate text was originally 9px/8px and unreadable at display scale; raised to 11px/10px with a
  wider plate, and re-verified legible from a fresh capture.
- All gates re-run after the change: build clean, CTest 2/2, surface check 14/14, preset and
  material checks pass, pluginval strictness 10 **SUCCESS** with Editor and Editor Automation.

## Not host-tested

- **CC1 and mono legato inside KeithDSP.** The iPlug2 voice allocator does not deliver controller
  messages to the sounding voice, so both are handled in `KeithDSP`. The offline engine proves the
  audible rule; pluginval never plays a note or sends CC1. Reaching these through a real DAW
  (automation lane or a controller) is still unverified.
- **FL Studio** has not loaded this build.

## Still open from the wider brief

Second filter, unison width / stereo field, a real modulation matrix, tempo-synced LFO, more tape
controls, pitch-bend range, the five-family factory content, the drum engine, and the insert/master
FX families. None of these are implemented and none are presented as working controls.
