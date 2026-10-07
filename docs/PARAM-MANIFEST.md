# Keith — Parameter Manifest

Contract between the editor lane (U3) and the composer / engine lane.  
**Stable ids.** The engine and host automation read these names; do not rename casually.

Column meanings:

| Column | Meaning |
|---|---|
| `id` | Stable snake_case identifier |
| `label` | Display label in the editor |
| `type` | `knob` · `switch` · `pad` · `list` |
| `range` | Numeric range + units, or enum members |
| `default` | Power-on / init default |
| `group` | `drum` · `synth` · `fx` · `preset` |
| `automatable` | MIDI/host-automatable parameter name (VST3/AU) |
| `status` | `locked` (from BRIEF) or `proposed` (U3 default; not decided) |

Rows below are the full v0.1 visible surface implied by BRIEF §§2–7,10. Anything BRIEF does not fix is `proposed`.

---

## Manifest rows

| id | label | type | range | default | group | automatable | status |
|---|---|---|---|---|---|---|---|
| `preset_family` | Family | list | Family 01…04 | Family 01 | preset | Preset Family | proposed |
| `preset_bank` | Bank | list | bank index 0…N | 0 | preset | Preset Bank | proposed |
| `preset_patch` | Patch | list | patch index 0…N | 0 | preset | Preset Patch | proposed |
| `preset_prev` | Prev | switch | moment | — | preset | Preset Prev | proposed |
| `preset_next` | Next | switch | moment | — | preset | Preset Next | proposed |
| `preset_save` | Save | switch | moment | — | preset | Preset Save | proposed |
| `preset_init` | Init | switch | moment | — | preset | Preset Init | proposed |
| `preset_favorite` | Fav | switch | off/on | off | preset | Preset Favorite | proposed |
| `drum_pad_01` | Pad 01 | pad | trigger / select | — | drum | Drum Pad 01 | proposed |
| `drum_pad_02` | Pad 02 | pad | trigger / select | — | drum | Drum Pad 02 | proposed |
| `drum_pad_03` | Pad 03 | pad | trigger / select | — | drum | Drum Pad 03 | proposed |
| `drum_pad_04` | Pad 04 | pad | trigger / select | — | drum | Drum Pad 04 | proposed |
| `drum_pad_05` | Pad 05 | pad | trigger / select | — | drum | Drum Pad 05 | proposed |
| `drum_pad_06` | Pad 06 | pad | trigger / select | — | drum | Drum Pad 06 | proposed |
| `drum_pad_07` | Pad 07 | pad | trigger / select | — | drum | Drum Pad 07 | proposed |
| `drum_pad_08` | Pad 08 | pad | trigger / select | — | drum | Drum Pad 08 | proposed |
| `drum_pad_09` | Pad 09 | pad | trigger / select | — | drum | Drum Pad 09 | proposed |
| `drum_pad_10` | Pad 10 | pad | trigger / select | — | drum | Drum Pad 10 | proposed |
| `drum_pad_11` | Pad 11 | pad | trigger / select | — | drum | Drum Pad 11 | proposed |
| `drum_pad_12` | Pad 12 | pad | trigger / select | — | drum | Drum Pad 12 | proposed |
| `drum_pad_13` | Pad 13 | pad | trigger / select | — | drum | Drum Pad 13 | proposed |
| `drum_pad_14` | Pad 14 | pad | trigger / select | — | drum | Drum Pad 14 | proposed |
| `drum_pad_15` | Pad 15 | pad | trigger / select | — | drum | Drum Pad 15 | proposed |
| `drum_pad_16` | Pad 16 | pad | trigger / select | — | drum | Drum Pad 16 | proposed |
| `drum_slot_select` | Slot | list | 1…16 | 1 | drum | Drum Slot Select | proposed |
| `drum_slot_level` | Level | knob | -inf…+6 dB | 0 dB | drum | Drum Slot Level | proposed |
| `drum_slot_pan` | Pan | knob | L100…R100 | C | drum | Drum Slot Pan | proposed |
| `drum_slot_tune` | Tune | knob | -12…+12 st | 0 st | drum | Drum Slot Tune | proposed |
| `drum_slot_decay` | Decay | knob | 0…100 % | 70 % | drum | Drum Slot Decay | proposed |
| `drum_slot_filter` | Filter | knob | 20 Hz…20 kHz | 12 kHz | drum | Drum Slot Filter | proposed |
| `drum_slot_drive` | Drive | knob | 0…100 % | 0 % | drum | Drum Slot Drive | proposed |
| `drum_slot_mute` | Mute | switch | off/on | off | drum | Drum Slot Mute | proposed |
| `drum_slot_solo` | Solo | switch | off/on | off | drum | Drum Slot Solo | proposed |
| `drum_kit_level` | Kit Level | knob | -inf…+6 dB | 0 dB | drum | Drum Kit Level | proposed |
| `drum_kit_swing` | Swing | knob | 0…100 % | 0 % | drum | Drum Kit Swing | proposed |
| `synth_engine` | Engine | switch | Hybrid / Analog | Hybrid | synth | Synth Engine | proposed |
| `synth_part` | Part | list | 1…4 | 1 | synth | Synth Part | proposed |
| `synth_poly_mode` | Poly | switch | Poly / Mono | Poly | synth | Synth Poly Mode | proposed |
| `synth_porta` | Porta | knob | 0…1000 ms | 0 ms | synth | Synth Porta | proposed |
| `synth_unison_voices` | Unison | knob | 1…8 | 1 | synth | Synth Unison Voices | proposed |
| `synth_unison_detune` | Detune | knob | 0…100 % | 25 % | synth | Synth Unison Detune | proposed |
| `synth_osc1_shape` | Osc1 Shape | knob | 0…100 % | 0 % | synth | Synth Osc1 Shape | proposed |
| `synth_osc1_pitch` | Osc1 Pitch | knob | -24…+24 st | 0 st | synth | Synth Osc1 Pitch | proposed |
| `synth_osc1_level` | Osc1 Level | knob | 0…100 % | 100 % | synth | Synth Osc1 Level | proposed |
| `synth_osc2_shape` | Osc2 Shape | knob | 0…100 % | 0 % | synth | Synth Osc2 Shape | proposed |
| `synth_osc2_pitch` | Osc2 Pitch | knob | -24…+24 st | 0 st | synth | Synth Osc2 Pitch | proposed |
| `synth_osc2_level` | Osc2 Level | knob | 0…100 % | 80 % | synth | Synth Osc2 Level | proposed |
| `synth_sub_level` | Sub Level | knob | 0…100 % | 0 % | synth | Synth Sub Level | proposed |
| `synth_noise_level` | Noise | knob | 0…100 % | 0 % | synth | Synth Noise Level | proposed |
| `synth_fm_amount` | FM | knob | 0…100 % | 0 % | synth | Synth FM Amount | proposed |
| `synth_filter_cutoff` | Cutoff | knob | 20 Hz…20 kHz | 8 kHz | synth | Synth Filter Cutoff | proposed |
| `synth_filter_reso` | Reso | knob | 0…100 % | 10 % | synth | Synth Filter Reso | proposed |
| `synth_filter_type` | Filt Type | list | LP24 / LP12 / BP / HP | LP24 | synth | Synth Filter Type | proposed |
| `synth_filter_env` | Filt Env | knob | -100…+100 % | 40 % | synth | Synth Filter Env | proposed |
| `synth_amp_attack` | Attack | knob | 0.1 ms…10 s | 5 ms | synth | Synth Amp Attack | proposed |
| `synth_amp_decay` | Decay | knob | 1 ms…10 s | 200 ms | synth | Synth Amp Decay | proposed |
| `synth_amp_sustain` | Sustain | knob | 0…100 % | 70 % | synth | Synth Amp Sustain | proposed |
| `synth_amp_release` | Release | knob | 1 ms…10 s | 300 ms | synth | Synth Amp Release | proposed |
| `synth_filt_attack` | F Att | knob | 0.1 ms…10 s | 10 ms | synth | Synth Filt Attack | proposed |
| `synth_filt_decay` | F Dec | knob | 1 ms…10 s | 250 ms | synth | Synth Filt Decay | proposed |
| `synth_filt_sustain` | F Sus | knob | 0…100 % | 40 % | synth | Synth Filt Sustain | proposed |
| `synth_filt_release` | F Rel | knob | 1 ms…10 s | 300 ms | synth | Synth Filt Release | proposed |
| `synth_lfo_rate` | LFO Rate | knob | 0.01…30 Hz | 1 Hz | synth | Synth LFO Rate | proposed |
| `synth_lfo_amount` | LFO Amt | knob | 0…100 % | 0 % | synth | Synth LFO Amount | proposed |
| `synth_mod_wheel` | Mod Dest | list | Cutoff / Pitch / Shape / Level | Cutoff | synth | Synth Mod Dest | proposed |
| `synth_master_level` | Level | knob | -inf…+6 dB | 0 dB | synth | Synth Master Level | proposed |
| `fx_tape_sat` | Saturation | knob | 0…100 % | 25 % | fx | FX Tape Saturation | proposed |
| `fx_tape_bump` | Head Bump | knob | 0…100 % | 20 % | fx | FX Tape Head Bump | proposed |
| `fx_tape_wow` | Wow | knob | 0…100 % | 10 % | fx | FX Tape Wow | proposed |
| `fx_tape_flutter` | Flutter | knob | 0…100 % | 10 % | fx | FX Tape Flutter | proposed |
| `fx_tape_hiss` | Hiss | knob | 0…100 % | 5 % | fx | FX Tape Hiss | proposed |
| `fx_tape_print` | Print | knob | 0…100 % | 0 % | fx | FX Tape Print | proposed |
| `fx_tape_mix` | Tape Mix | knob | 0…100 % | 40 % | fx | FX Tape Mix | proposed |
| `fx_tape_bypass` | Tape Byp | switch | off/on | off | fx | FX Tape Bypass | proposed |
| `fx_ins1_algo` | Ins1 Algo | list | algo enum (TBD) | 0 | fx | FX Insert1 Algo | proposed |
| `fx_ins1_a` | Ins1 A | knob | 0…100 % | 50 % | fx | FX Insert1 A | proposed |
| `fx_ins1_b` | Ins1 B | knob | 0…100 % | 50 % | fx | FX Insert1 B | proposed |
| `fx_ins1_c` | Ins1 C | knob | 0…100 % | 50 % | fx | FX Insert1 C | proposed |
| `fx_ins1_mix` | Ins1 Mix | knob | 0…100 % | 100 % | fx | FX Insert1 Mix | proposed |
| `fx_ins1_bypass` | Ins1 Byp | switch | off/on | off | fx | FX Insert1 Bypass | proposed |
| `fx_ins2_algo` | Ins2 Algo | list | algo enum (TBD) | 0 | fx | FX Insert2 Algo | proposed |
| `fx_ins2_a` | Ins2 A | knob | 0…100 % | 50 % | fx | FX Insert2 A | proposed |
| `fx_ins2_b` | Ins2 B | knob | 0…100 % | 50 % | fx | FX Insert2 B | proposed |
| `fx_ins2_c` | Ins2 C | knob | 0…100 % | 50 % | fx | FX Insert2 C | proposed |
| `fx_ins2_mix` | Ins2 Mix | knob | 0…100 % | 100 % | fx | FX Insert2 Mix | proposed |
| `fx_ins2_bypass` | Ins2 Byp | switch | off/on | off | fx | FX Insert2 Bypass | proposed |
| `fx_mst_algo` | Master Algo | list | algo enum (TBD) | 0 | fx | FX Master Algo | proposed |
| `fx_mst_a` | Master A | knob | 0…100 % | 50 % | fx | FX Master A | proposed |
| `fx_mst_b` | Master B | knob | 0…100 % | 50 % | fx | FX Master B | proposed |
| `fx_mst_c` | Master C | knob | 0…100 % | 50 % | fx | FX Master C | proposed |
| `fx_mst_mix` | Master Mix | knob | 0…100 % | 40 % | fx | FX Master Mix | proposed |
| `fx_mst_bypass` | Master Byp | switch | off/on | off | fx | FX Master Bypass | proposed |
| `fx_out_level` | Out Level | knob | -inf…+12 dB | 0 dB | fx | FX Out Level | proposed |

---

## Notes for the engine lane

1. Pads are UI triggers / slot selectors; MIDI note → slot mapping lives in `DRUM-MAP.md`. Host automation of pads is optional; if the host surface cannot expose pads, keep `drum_slot_select` + slot knobs.
2. Momentary preset switches (`prev` / `next` / `save` / `init`) may be implemented as non-automatable UI actions; they stay in this manifest so the editor layout stays 1:1.
3. `fx_*_algo` enum members are filled when U2 / U4 publish the factory FX algorithm list. Until then the prototype shows placeholder names `Algo 01`… without claiming real algorithm identities.
4. No Korg, Splice, Freakshow, Surge, or OB-XF names appear in `label` or `automatable`.

## Proposed-but-undecided (summary)

Every row is currently `proposed` because U1 has not published a locked parameter surface yet. Highest-impact undecided clusters:

- Preset navigation / save / init / favourite behaviour
- Drum slot tone control set and kit swing
- Synth engine labelling (Hybrid / Analog), part count, filter enum
- Insert/Master FX algorithm catalogues and macro meanings
- Whether pads are host-automatable parameters or UI-only
