# Keith - Parameter Manifest

Contract between the editor lane and the composer / engine lane.
**Stable ids.** The engine and host automation read these names; do not rename casually.

Reconciled **U5 (2026-10-06)** against `src/Keith.cpp` / `src/Keith.h` as shipped by U1.
U1 exposes a single host parameter: `Gain` (`kParamGain`). All other rows remain intended
editor surface and are marked `not yet implemented` until the engine publishes them.

Column meanings:

| Column | Meaning |
|---|---|
| `id` | Stable snake_case identifier |
| `label` | Display label in the editor |
| `type` | `knob` / `switch` / `pad` / `list` |
| `range` | Numeric range + units, or enum members |
| `default` | Power-on / init default |
| `group` | `drum` / `synth` / `fx` / `preset` / `master` |
| `automatable` | MIDI/host-automatable parameter name (VST3/AU) |
| `status` | Taste / lock state: `locked` (from BRIEF) or `proposed` (not decided) |
| `impl` | Engine wiring: `live` (exists in code) / `not yet implemented` / `live - added U5` |

---

## Manifest rows

| id | label | type | range | default | group | automatable | status | impl |
|---|---|---|---|---|---|---|---|---|
| `gain` | Gain | knob | 0-100 % | 100 % | master | Gain | proposed | live - added U5 |
| `preset_family` | Family | list | Family 01-04 | Family 01 | preset | Preset Family | proposed | not yet implemented |
| `preset_bank` | Bank | list | bank index 0-N | 0 | preset | Preset Bank | proposed | not yet implemented |
| `preset_patch` | Patch | list | patch index 0-N | 0 | preset | Preset Patch | proposed | not yet implemented |
| `preset_prev` | Prev | switch | moment | - | preset | Preset Prev | proposed | not yet implemented |
| `preset_next` | Next | switch | moment | - | preset | Preset Next | proposed | not yet implemented |
| `preset_save` | Save | switch | moment | - | preset | Preset Save | proposed | not yet implemented |
| `preset_init` | Init | switch | moment | - | preset | Preset Init | proposed | not yet implemented |
| `preset_favorite` | Fav | switch | off/on | off | preset | Preset Favorite | proposed | not yet implemented |
| `drum_pad_01` | Pad 01 | pad | trigger / select | - | drum | Drum Pad 01 | proposed | not yet implemented |
| `drum_pad_02` | Pad 02 | pad | trigger / select | - | drum | Drum Pad 02 | proposed | not yet implemented |
| `drum_pad_03` | Pad 03 | pad | trigger / select | - | drum | Drum Pad 03 | proposed | not yet implemented |
| `drum_pad_04` | Pad 04 | pad | trigger / select | - | drum | Drum Pad 04 | proposed | not yet implemented |
| `drum_pad_05` | Pad 05 | pad | trigger / select | - | drum | Drum Pad 05 | proposed | not yet implemented |
| `drum_pad_06` | Pad 06 | pad | trigger / select | - | drum | Drum Pad 06 | proposed | not yet implemented |
| `drum_pad_07` | Pad 07 | pad | trigger / select | - | drum | Drum Pad 07 | proposed | not yet implemented |
| `drum_pad_08` | Pad 08 | pad | trigger / select | - | drum | Drum Pad 08 | proposed | not yet implemented |
| `drum_pad_09` | Pad 09 | pad | trigger / select | - | drum | Drum Pad 09 | proposed | not yet implemented |
| `drum_pad_10` | Pad 10 | pad | trigger / select | - | drum | Drum Pad 10 | proposed | not yet implemented |
| `drum_pad_11` | Pad 11 | pad | trigger / select | - | drum | Drum Pad 11 | proposed | not yet implemented |
| `drum_pad_12` | Pad 12 | pad | trigger / select | - | drum | Drum Pad 12 | proposed | not yet implemented |
| `drum_pad_13` | Pad 13 | pad | trigger / select | - | drum | Drum Pad 13 | proposed | not yet implemented |
| `drum_pad_14` | Pad 14 | pad | trigger / select | - | drum | Drum Pad 14 | proposed | not yet implemented |
| `drum_pad_15` | Pad 15 | pad | trigger / select | - | drum | Drum Pad 15 | proposed | not yet implemented |
| `drum_pad_16` | Pad 16 | pad | trigger / select | - | drum | Drum Pad 16 | proposed | not yet implemented |
| `drum_slot_select` | Slot | list | 1-16 | 1 | drum | Drum Slot Select | proposed | not yet implemented |
| `drum_slot_level` | Level | knob | -inf-+6 dB | 0 dB | drum | Drum Slot Level | proposed | not yet implemented |
| `drum_slot_pan` | Pan | knob | L100-R100 | C | drum | Drum Slot Pan | proposed | not yet implemented |
| `drum_slot_tune` | Tune | knob | -12-+12 st | 0 st | drum | Drum Slot Tune | proposed | not yet implemented |
| `drum_slot_decay` | Decay | knob | 0-100 % | 70 % | drum | Drum Slot Decay | proposed | not yet implemented |
| `drum_slot_filter` | Filter | knob | 20 Hz-20 kHz | 12 kHz | drum | Drum Slot Filter | proposed | not yet implemented |
| `drum_slot_drive` | Drive | knob | 0-100 % | 0 % | drum | Drum Slot Drive | proposed | not yet implemented |
| `drum_slot_mute` | Mute | switch | off/on | off | drum | Drum Slot Mute | proposed | not yet implemented |
| `drum_slot_solo` | Solo | switch | off/on | off | drum | Drum Slot Solo | proposed | not yet implemented |
| `drum_kit_level` | Kit Level | knob | -inf-+6 dB | 0 dB | drum | Drum Kit Level | proposed | not yet implemented |
| `drum_kit_swing` | Swing | knob | 0-100 % | 0 % | drum | Drum Kit Swing | proposed | not yet implemented |
| `synth_engine` | Engine | switch | Hybrid / Analog | Hybrid | synth | Synth Engine | proposed | not yet implemented |
| `synth_part` | Part | list | 1-4 | 1 | synth | Synth Part | proposed | not yet implemented |
| `synth_poly_mode` | Poly | switch | Poly / Mono | Poly | synth | Synth Poly Mode | proposed | not yet implemented |
| `synth_porta` | Porta | knob | 0-1000 ms | 0 ms | synth | Synth Porta | proposed | not yet implemented |
| `synth_unison_voices` | Unison | knob | 1-8 | 1 | synth | Synth Unison Voices | proposed | not yet implemented |
| `synth_unison_detune` | Detune | knob | 0-100 % | 25 % | synth | Synth Unison Detune | proposed | not yet implemented |
| `synth_osc1_shape` | Osc1 Shape | knob | 0-100 % | 0 % | synth | Synth Osc1 Shape | proposed | not yet implemented |
| `synth_osc1_pitch` | Osc1 Pitch | knob | -24-+24 st | 0 st | synth | Synth Osc1 Pitch | proposed | not yet implemented |
| `synth_osc1_level` | Osc1 Level | knob | 0-100 % | 100 % | synth | Synth Osc1 Level | proposed | not yet implemented |
| `synth_osc2_shape` | Osc2 Shape | knob | 0-100 % | 0 % | synth | Synth Osc2 Shape | proposed | not yet implemented |
| `synth_osc2_pitch` | Osc2 Pitch | knob | -24-+24 st | 0 st | synth | Synth Osc2 Pitch | proposed | not yet implemented |
| `synth_osc2_level` | Osc2 Level | knob | 0-100 % | 80 % | synth | Synth Osc2 Level | proposed | not yet implemented |
| `synth_sub_level` | Sub Level | knob | 0-100 % | 0 % | synth | Synth Sub Level | proposed | not yet implemented |
| `synth_noise_level` | Noise | knob | 0-100 % | 0 % | synth | Synth Noise Level | proposed | not yet implemented |
| `synth_fm_amount` | FM | knob | 0-100 % | 0 % | synth | Synth FM Amount | proposed | not yet implemented |
| `synth_filter_cutoff` | Cutoff | knob | 20 Hz-20 kHz | 8 kHz | synth | Synth Filter Cutoff | proposed | not yet implemented |
| `synth_filter_reso` | Reso | knob | 0-100 % | 10 % | synth | Synth Filter Reso | proposed | not yet implemented |
| `synth_filter_type` | Filt Type | list | LP24 / LP12 / BP / HP | LP24 | synth | Synth Filter Type | proposed | not yet implemented |
| `synth_filter_env` | Filt Env | knob | -100-+100 % | 40 % | synth | Synth Filter Env | proposed | not yet implemented |
| `synth_amp_attack` | Attack | knob | 0.1 ms-10 s | 5 ms | synth | Synth Amp Attack | proposed | not yet implemented |
| `synth_amp_decay` | Decay | knob | 1 ms-10 s | 200 ms | synth | Synth Amp Decay | proposed | not yet implemented |
| `synth_amp_sustain` | Sustain | knob | 0-100 % | 70 % | synth | Synth Amp Sustain | proposed | not yet implemented |
| `synth_amp_release` | Release | knob | 1 ms-10 s | 300 ms | synth | Synth Amp Release | proposed | not yet implemented |
| `synth_filt_attack` | F Att | knob | 0.1 ms-10 s | 10 ms | synth | Synth Filt Attack | proposed | not yet implemented |
| `synth_filt_decay` | F Dec | knob | 1 ms-10 s | 250 ms | synth | Synth Filt Decay | proposed | not yet implemented |
| `synth_filt_sustain` | F Sus | knob | 0-100 % | 40 % | synth | Synth Filt Sustain | proposed | not yet implemented |
| `synth_filt_release` | F Rel | knob | 1 ms-10 s | 300 ms | synth | Synth Filt Release | proposed | not yet implemented |
| `synth_lfo_rate` | LFO Rate | knob | 0.01-30 Hz | 1 Hz | synth | Synth LFO Rate | proposed | not yet implemented |
| `synth_lfo_amount` | LFO Amt | knob | 0-100 % | 0 % | synth | Synth LFO Amount | proposed | not yet implemented |
| `synth_mod_wheel` | Mod Dest | list | Cutoff / Pitch / Shape / Level | Cutoff | synth | Synth Mod Dest | proposed | not yet implemented |
| `synth_master_level` | Level | knob | -inf-+6 dB | 0 dB | synth | Synth Master Level | proposed | not yet implemented |
| `fx_tape_sat` | Saturation | knob | 0-100 % | 25 % | fx | FX Tape Saturation | proposed | not yet implemented |
| `fx_tape_bump` | Head Bump | knob | 0-100 % | 20 % | fx | FX Tape Head Bump | proposed | not yet implemented |
| `fx_tape_wow` | Wow | knob | 0-100 % | 10 % | fx | FX Tape Wow | proposed | not yet implemented |
| `fx_tape_flutter` | Flutter | knob | 0-100 % | 10 % | fx | FX Tape Flutter | proposed | not yet implemented |
| `fx_tape_hiss` | Hiss | knob | 0-100 % | 5 % | fx | FX Tape Hiss | proposed | not yet implemented |
| `fx_tape_print` | Print | knob | 0-100 % | 0 % | fx | FX Tape Print | proposed | not yet implemented |
| `fx_tape_mix` | Tape Mix | knob | 0-100 % | 40 % | fx | FX Tape Mix | proposed | not yet implemented |
| `fx_tape_bypass` | Tape Byp | switch | off/on | off | fx | FX Tape Bypass | proposed | not yet implemented |
| `fx_ins1_algo` | Ins1 Algo | list | algo enum (TBD) | 0 | fx | FX Insert1 Algo | proposed | not yet implemented |
| `fx_ins1_a` | Ins1 A | knob | 0-100 % | 50 % | fx | FX Insert1 A | proposed | not yet implemented |
| `fx_ins1_b` | Ins1 B | knob | 0-100 % | 50 % | fx | FX Insert1 B | proposed | not yet implemented |
| `fx_ins1_c` | Ins1 C | knob | 0-100 % | 50 % | fx | FX Insert1 C | proposed | not yet implemented |
| `fx_ins1_mix` | Ins1 Mix | knob | 0-100 % | 100 % | fx | FX Insert1 Mix | proposed | not yet implemented |
| `fx_ins1_bypass` | Ins1 Byp | switch | off/on | off | fx | FX Insert1 Bypass | proposed | not yet implemented |
| `fx_ins2_algo` | Ins2 Algo | list | algo enum (TBD) | 0 | fx | FX Insert2 Algo | proposed | not yet implemented |
| `fx_ins2_a` | Ins2 A | knob | 0-100 % | 50 % | fx | FX Insert2 A | proposed | not yet implemented |
| `fx_ins2_b` | Ins2 B | knob | 0-100 % | 50 % | fx | FX Insert2 B | proposed | not yet implemented |
| `fx_ins2_c` | Ins2 C | knob | 0-100 % | 50 % | fx | FX Insert2 C | proposed | not yet implemented |
| `fx_ins2_mix` | Ins2 Mix | knob | 0-100 % | 100 % | fx | FX Insert2 Mix | proposed | not yet implemented |
| `fx_ins2_bypass` | Ins2 Byp | switch | off/on | off | fx | FX Insert2 Bypass | proposed | not yet implemented |
| `fx_mst_algo` | Master Algo | list | algo enum (TBD) | 0 | fx | FX Master Algo | proposed | not yet implemented |
| `fx_mst_a` | Master A | knob | 0-100 % | 50 % | fx | FX Master A | proposed | not yet implemented |
| `fx_mst_b` | Master B | knob | 0-100 % | 50 % | fx | FX Master B | proposed | not yet implemented |
| `fx_mst_c` | Master C | knob | 0-100 % | 50 % | fx | FX Master C | proposed | not yet implemented |
| `fx_mst_mix` | Master Mix | knob | 0-100 % | 40 % | fx | FX Master Mix | proposed | not yet implemented |
| `fx_mst_bypass` | Master Byp | switch | off/on | off | fx | FX Master Bypass | proposed | not yet implemented |
| `fx_out_level` | Out Level | knob | -inf-+12 dB | 0 dB | fx | FX Out Level | proposed | not yet implemented |

---

## U1 code surface (source of truth for `live`)

From `src/Keith.h` / `src/Keith.cpp` (read-only for this lane):

```
enum EParams { kParamGain = 0, kNumParams };
GetParam(kParamGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");
```

| Code enum | Host name | Range | Default | Manifest `id` |
|---|---|---|---|---|
| `kParamGain` | Gain | 0-100 % step 0.01 | 100 % | `gain` |

No other `Init*` parameter registrations exist in U1.

---

## Row counts (U5 reconcile)

| Metric | Count |
|---|---|
| Total rows | **94** |
| Rows matching a real parameter (`impl` contains `live`) | **1** (`gain`) |
| Rows `not yet implemented` | **93** |
| Rows added this pass | **1** (`gain`) |

Note: U3 reported 101 rows; the table that was actually on disk had **93** data rows.
U5 keeps those 93 intent rows and adds `gain`.

---

## Notes for the engine lane

1. Pads are UI triggers / slot selectors; MIDI note -> slot mapping lives in `DRUM-MAP.md`. Host automation of pads is optional; if the host surface cannot expose pads, keep `drum_slot_select` + slot knobs.
2. Momentary preset switches (`prev` / `next` / `save` / `init`) may be implemented as non-automatable UI actions; they stay in this manifest so the editor layout stays 1:1.
3. `fx_*_algo` enum members are filled when U2 / U4 publish the factory FX algorithm list. Until then the prototype shows placeholder names `Algo 01` without claiming real algorithm identities.
4. No Korg, Splice, Freakshow, Surge, or OB-XF names appear in `label` or `automatable`.
5. `gain` is the U1 sine-voice output gain. It is distinct from the proposed `synth_master_level` and `fx_out_level` rows (those remain intended once the fuller engine lands). Do not silently merge them without a composer-lane decision.
6. When U4 / later engine passes add parameters, flip `impl` from `not yet implemented` to `live` and align `range` / `default` / `label` to the code.

## Proposed-but-undecided (summary)

All non-`gain` rows stay `proposed` (taste / product surface not locked). Highest-impact undecided clusters:

- Preset navigation / save / init / favourite behaviour
- Drum slot tone control set and kit swing
- Synth engine labelling (Hybrid / Analog), part count, filter enum
- Insert/Master FX algorithm catalogues and macro meanings
- Whether pads are host-automatable parameters or UI-only
- Whether U1 `gain` remains the long-term master level or is superseded by `synth_master_level` / `fx_out_level`
