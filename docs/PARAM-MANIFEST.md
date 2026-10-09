# Keith - Parameter Manifest

Contract between the editor lane and the composer / engine lane.
**Stable ids.** The engine and host automation read these names; do not rename casually.

Reconciled **U15 (2026-10-07)** against `src/Keith.h` / `src/Keith.cpp`.
U1/U4 host IDs **0-8**, U7 IDs **9-42**, U9 IDs **43-55**, U10 IDs **56-74** and
U11 IDs **75-84** are preserved. U13 appends the second LFO at IDs **85-88**. U15 appends
the four-slot modulation matrix at IDs **89-100**. U17 appends pitch-tracked noise,
envelope curves, a stereo path, direct level and filter-key bend at IDs **101-110**.
Current `kNumParams` is **111**.

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
| `impl` | Engine wiring: `live` / `not yet implemented` / `not applicable` / `live - added U#` |

---

## Manifest rows

| id | label | type | range | default | group | automatable | status | impl |
|---|---|---|---|---|---|---|---|---|
| `gain` | Gain | knob | 0-100 % | 100 % | master | Gain | proposed | live - added U5 |
| `preset_family` | Family | list | Family 01-05 | Family 01 | preset | Preset Family | proposed | live - host preset action |
| `preset_bank` | Bank | list | bank index 0-N | 0 | preset | Preset Bank | proposed | not yet implemented |
| `preset_patch` | Patch | list | patch index 0-N | 0 | preset | Preset Patch | proposed | not yet implemented |
| `preset_prev` | Prev | switch | moment | - | preset | Preset Prev | proposed | not yet implemented |
| `preset_next` | Next | switch | moment | - | preset | Preset Next | proposed | not yet implemented |
| `preset_save` | Save | switch | moment | - | preset | Preset Save | proposed | not yet implemented |
| `preset_init` | Init | switch | moment | - | preset | Preset Init | proposed | not yet implemented |
| `preset_favorite` | Fav | switch | off/on | off | preset | Preset Favorite | proposed | not yet implemented |
| `drum_pad_01` | Kick | pad | MIDI 36 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_02` | Snare | pad | MIDI 37 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_03` | Snare Alt / Rim | pad | MIDI 38 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_04` | Clap | pad | MIDI 39 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_05` | Closed Hat | pad | MIDI 40 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_06` | Open Hat | pad | MIDI 41 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_07` | Pedal Hat | pad | MIDI 42 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_08` | Tom Lo | pad | MIDI 43 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_09` | Tom Mid | pad | MIDI 44 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_10` | Tom Hi | pad | MIDI 45 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_11` | Ride | pad | MIDI 46 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_12` | Crash | pad | MIDI 47 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_13` | Perc 1 | pad | MIDI 48 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_14` | Perc 2 | pad | MIDI 49 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_15` | Perc 3 | pad | MIDI 50 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_pad_16` | Perc 4 | pad | MIDI 51 / select | - | drum | not automatable | locked | live - MIDI trigger / editor select U11 |
| `drum_slot_select` | Slot | list | 1-16 | 1 | drum | Drum Slot Select | locked | live - added U11 |
| `drum_slot_level` | Slot Level | knob | -inf-+6 dB | 0 dB | drum | Drum Slot Level | locked | live - added U11 |
| `drum_slot_pan` | Slot Pan | knob | L100-R100 | C | drum | Drum Slot Pan | locked | live - added U11 |
| `drum_slot_tune` | Slot Tune | knob | -12-+12 st | 0 st | drum | Drum Slot Tune | locked | live - added U11 |
| `drum_slot_decay` | Slot Decay | knob | 0-100 % | 70 % | drum | Drum Slot Decay | locked | live - added U11 |
| `drum_slot_filter` | Slot Filter | knob | 20 Hz-20 kHz | 12 kHz | drum | Drum Slot Filter | locked | live - added U11 |
| `drum_slot_drive` | Slot Drive | knob | 0-100 % | 0 % | drum | Drum Slot Drive | locked | live - added U11 |
| `drum_slot_mute` | Slot Mute | switch | off/on | off | drum | Drum Slot Mute | locked | live - added U11 |
| `drum_slot_solo` | Slot Solo | switch | off/on | off | drum | Drum Slot Solo | locked | live - added U11 |
| `drum_kit_level` | Kit Level | knob | -inf-+6 dB | 0 dB | drum | Drum Kit Level | locked | live - added U11 |
| `drum_kit_swing` | Swing | knob | not applicable | - | drum | not applicable | locked | not applicable - instrument has no sequencer |
| `synth_engine` | Engine | switch | Hybrid / Analog | Hybrid | synth | Synth Engine | proposed | live - added U7 |
| `synth_part` | Part | list | 1-4 | 1 | synth | Synth Part | proposed | not yet implemented |
| `synth_poly_mode` | Poly | switch | Poly / Mono | Poly | synth | Synth Poly Mode | proposed | live - added U7 |
| `synth_porta` | Porta | knob | 0-1000 ms | 0 ms | synth | Synth Porta | proposed | live - added U7 |
| `synth_unison_voices` | Unison | knob | 1-8 | 1 | synth | Synth Unison Voices | proposed | live - added U7 |
| `synth_unison_detune` | Detune | knob | 0-100 % | 25 % | synth | Synth Unison Detune | proposed | live - added U7 |
| `synth_osc1_shape` | Osc1 Shape | knob | 0-100 % | 0 % | synth | Synth Osc1 Shape | proposed | live - added U7 |
| `synth_osc1_pitch` | Osc1 Pitch | knob | -24-+24 st | 0 st | synth | Synth Osc1 Pitch | proposed | live - added U7 |
| `synth_osc1_level` | Osc1 Level | knob | 0-100 % | 100 % | synth | Synth Osc1 Level | proposed | live - added U7 |
| `synth_osc1_pw` | Osc1 PW | knob | 5-95 % | 50 % | synth | Synth Osc1 PW | proposed | live - added U7 |
| `synth_osc2_shape` | Osc2 Shape | knob | 0-100 % | 0 % | synth | Synth Osc2 Shape | proposed | live - added U7 |
| `synth_osc2_pitch` | Osc2 Pitch | knob | -24-+24 st | 0 st | synth | Synth Osc2 Pitch | proposed | live - added U7 |
| `synth_osc2_level` | Osc2 Level | knob | 0-100 % | 80 % | synth | Synth Osc2 Level | proposed | live - added U7 |
| `synth_osc2_pw` | Osc2 PW | knob | 5-95 % | 50 % | synth | Synth Osc2 PW | proposed | live - added U7 |
| `synth_sub_level` | Sub Level | knob | 0-100 % | 0 % | synth | Synth Sub Level | proposed | live - added U7 |
| `synth_noise_level` | Noise | knob | 0-100 % | 0 % | synth | Synth Noise Level | proposed | live - added U7 |
| `synth_fm_amount` | FM | knob | 0-100 % | 0 % | synth | Synth FM Amount | proposed | live - added U7 |
| `synth_filter_cutoff` | Cutoff | knob | 20 Hz-20 kHz | 8 kHz | synth | Synth Filter Cutoff | proposed | live - added U7 |
| `synth_filter_reso` | Reso | knob | 0-100 % | 10 % | synth | Synth Filter Reso | proposed | live - added U7 |
| `synth_filter_type` | Filt Type | list | LP24 / LP12 / BP / HP | LP24 | synth | Synth Filter Type | proposed | live - added U7 |
| `synth_filter_env` | Filt Env | knob | -100-+100 % | 40 % | synth | Synth Filter Env | proposed | live - added U7 |
| `synth_amp_attack` | Attack | knob | 0.1 ms-10 s | 5 ms | synth | Synth Amp Attack | proposed | live - added U7 |
| `synth_amp_decay` | Decay | knob | 1 ms-30 s | 200 ms | synth | Synth Amp Decay | proposed | live - added U7 |
| `synth_amp_sustain` | Sustain | knob | 0-100 % | 70 % | synth | Synth Amp Sustain | proposed | live - added U7 |
| `synth_amp_release` | Release | knob | 1 ms-10 s | 300 ms | synth | Synth Amp Release | proposed | live - added U7 |
| `synth_pitch_attack` | P Att | knob | 0.1 ms-10 s | 10 ms | synth | Synth Pitch Attack | proposed | live - added U7 |
| `synth_pitch_decay` | P Dec | knob | 1 ms-10 s | 250 ms | synth | Synth Pitch Decay | proposed | live - added U7 |
| `synth_pitch_sustain` | P Sus | knob | 0-100 % | 0 % | synth | Synth Pitch Sustain | proposed | live - added U7 |
| `synth_pitch_release` | P Rel | knob | 1 ms-10 s | 300 ms | synth | Synth Pitch Release | proposed | live - added U7 |
| `synth_pitch_amount` | P Amt | knob | -24-+24 st | 0 st | synth | Synth Pitch Amount | proposed | live - added U7 |
| `synth_filt_attack` | F Att | knob | 0.1 ms-10 s | 10 ms | synth | Synth Filt Attack | proposed | live - added U7 |
| `synth_filt_decay` | F Dec | knob | 1 ms-10 s | 250 ms | synth | Synth Filt Decay | proposed | live - added U7 |
| `synth_filt_sustain` | F Sus | knob | 0-100 % | 40 % | synth | Synth Filt Sustain | proposed | live - added U7 |
| `synth_filt_release` | F Rel | knob | 1 ms-10 s | 300 ms | synth | Synth Filt Release | proposed | live - added U7 |
| `synth_osc_sync` | Osc Sync | switch | Free / Sync | Free | synth | Synth Osc Sync | proposed | live - added U9 |
| `synth_osc_ring` | Osc Ring | switch | Off / On | Off | synth | Synth Osc Ring | proposed | live - added U9 |
| `synth_filter_key_track` | Key Track | knob | 0-200 % | 0 % | synth | Synth Filter Key Track | proposed | live - added U9 |
| `synth_filter_drive` | Drive | knob | 0-100 % | 0 % | synth | Synth Filter Drive | proposed | live - added U9 |
| `synth_lfo_rate` | LFO Rate | knob | 0.01-30 Hz | 1 Hz | synth | Synth LFO Rate | proposed | live - added U9 |
| `synth_lfo_shape` | LFO Shape | list | Sine / Tri / Saw / Square / S&H | Sine | synth | Synth LFO Shape | proposed | live - added U9 |
| `synth_lfo_dest` | LFO Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Synth LFO Dest | proposed | live - added U9 |
| `synth_lfo_amount` | LFO Amt | knob | -100-+100 % | 0 % | synth | Synth LFO Amount | proposed | live - added U9 |
| `synth_lfo2_rate` | LFO2 Rate | knob | 0.01-30 Hz | 1 Hz | synth | Synth LFO2 Rate | proposed | live - added U13 |
| `synth_lfo2_shape` | LFO2 Shape | list | Sine / Tri / Saw / Square / S&H | Sine | synth | Synth LFO2 Shape | proposed | live - added U13 |
| `synth_lfo2_dest` | LFO2 Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Synth LFO2 Dest | proposed | live - added U13 |
| `synth_lfo2_amount` | LFO2 Amt | knob | -100-+100 % | 0 % | synth | Synth LFO2 Amount | proposed | live - added U13 |
| `synth_mod1_source` | Mod1 Src | list | Off / LFO 1 / LFO 2 / Mod Wheel / Velocity / Key | Off | synth | Mod1 Src | proposed | live - added U15 |
| `synth_mod1_dest` | Mod1 Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Mod1 Dest | proposed | live - added U15 |
| `synth_mod1_amount` | Mod1 Amt | knob | -100-+100 % | 0 % | synth | Mod1 Amt | proposed | live - added U15 |
| `synth_mod2_source` | Mod2 Src | list | Off / LFO 1 / LFO 2 / Mod Wheel / Velocity / Key | Off | synth | Mod2 Src | proposed | live - added U15 |
| `synth_mod2_dest` | Mod2 Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Mod2 Dest | proposed | live - added U15 |
| `synth_mod2_amount` | Mod2 Amt | knob | -100-+100 % | 0 % | synth | Mod2 Amt | proposed | live - added U15 |
| `synth_mod3_source` | Mod3 Src | list | Off / LFO 1 / LFO 2 / Mod Wheel / Velocity / Key | Off | synth | Mod3 Src | proposed | live - added U15 |
| `synth_mod3_dest` | Mod3 Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Mod3 Dest | proposed | live - added U15 |
| `synth_mod3_amount` | Mod3 Amt | knob | -100-+100 % | 0 % | synth | Mod3 Amt | proposed | live - added U15 |
| `synth_mod4_source` | Mod4 Src | list | Off / LFO 1 / LFO 2 / Mod Wheel / Velocity / Key | Off | synth | Mod4 Src | proposed | live - added U15 |
| `synth_mod4_dest` | Mod4 Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Mod4 Dest | proposed | live - added U15 |
| `synth_mod4_amount` | Mod4 Amt | knob | -100-+100 % | 0 % | synth | Mod4 Amt | proposed | live - added U15 |
| `synth_noise_key` | Noise Key | knob | -100-+100 % | 0 % | synth | Synth Noise Key | proposed | live - added U17 |
| `synth_amp_attack_key` | Atk Key | knob | -100-+100 % | 0 % | synth | Synth Amp Attack Key | proposed | live - added U17 |
| `synth_amp_attack_curve` | Atk Bend | knob | -100-+100 % | 0 % | synth | Synth Amp Attack Curve | proposed | live - added U17 |
| `synth_amp_decay_key` | Dec Key | knob | -100-+100 % | 0 % | synth | Synth Amp Decay Key | proposed | live - added U17 |
| `synth_amp_decay_curve` | Dec Bend | knob | -100-+100 % | 0 % | synth | Synth Amp Decay Curve | proposed | live - added U17 |
| `synth_stereo_delay` | St Delay | knob | 0-40 ms | 0 ms | synth | Synth Stereo Delay | proposed | live - added U17 |
| `synth_stereo_delay_key` | St Track | knob | -100-+100 % | 0 % | synth | Synth Stereo Delay Key | proposed | live - added U17 |
| `synth_stereo_spread` | St Width | knob | -100-+100 % | 0 % | synth | Synth Stereo Spread | proposed | live - added U17 |
| `synth_direct_level` | Direct | knob | 0-100 % | 0 % | synth | Synth Direct Level | proposed | live - added U17 |
| `synth_filter_key_curve` | Cut Bend | knob | -100-+100 % | 0 % | synth | Synth Filter Key Curve | proposed | live - added U17 |
| `synth_mod_wheel` | Mod Dest | list | Off / Cutoff / Pitch / Shape / Level / FM | Off | synth | Synth Mod Dest | proposed | live - added U9 |
| `synth_mod_amount` | Mod Amt | knob | -100-+100 % | 0 % | synth | Synth Mod Amount | proposed | live - added U9 |
| `synth_vel_to_amp` | Vel To Amp | knob | 0-100 % | 0 % | synth | Synth Vel To Amp | proposed | live - added U9 |
| `synth_vel_to_filter` | Vel To Filter | knob | 0-100 % | 0 % | synth | Synth Vel To Filter | proposed | live - added U9 |
| `synth_mono_legato` | Mono Legato | switch | Retrig / Legato | Retrig | synth | Synth Mono Legato | proposed | live - added U9 |
| `synth_master_level` | Level | knob | -60-+18 dB | 0 dB | synth | Synth Master Level | proposed | live - added U7 |
| `fx_tape_sat` | Saturation | knob | 0-100 % | 25 % | fx | FX Tape Saturation | proposed | live - added U4 |
| `fx_tape_bump` | Head Bump | knob | 0-100 % | 20 % | fx | FX Tape Head Bump | proposed | live - added U4 |
| `fx_tape_wow` | Wow | knob | 0-100 % | 10 % | fx | FX Tape Wow | proposed | live - added U4 |
| `fx_tape_flutter` | Flutter | knob | 0-100 % | 10 % | fx | FX Tape Flutter | proposed | live - added U4 |
| `fx_tape_hiss` | Hiss | knob | 0-100 % | 5 % | fx | FX Tape Hiss | proposed | live - added U4 |
| `fx_tape_print` | Print | knob | 0-100 % | 0 % | fx | FX Tape Print | proposed | live - added U4 |
| `fx_tape_mix` | Tape Mix | knob | 0-100 % | 40 % | fx | FX Tape Mix | proposed | live - added U4 |
| `fx_tape_bypass` | Tape Byp | switch | off/on | off | fx | FX Tape Bypass | proposed | live - added U4 |
| `fx_ins1_algo` | Ins1 Algo | list | Off / Drive / Chorus / Delay / Crush | Off | fx | FX Insert1 Algo | proposed | live - added U10 |
| `fx_ins1_a` | Ins1 A | knob | 0-100 % | 50 % | fx | FX Insert1 A | proposed | live - added U10 |
| `fx_ins1_b` | Ins1 B | knob | 0-100 % | 50 % | fx | FX Insert1 B | proposed | live - added U10 |
| `fx_ins1_c` | Ins1 C | knob | 0-100 % | 50 % | fx | FX Insert1 C | proposed | live - added U10 |
| `fx_ins1_mix` | Ins1 Mix | knob | 0-100 % | 100 % | fx | FX Insert1 Mix | proposed | live - added U10 |
| `fx_ins1_bypass` | Ins1 Byp | switch | Active / Bypass | Active | fx | FX Insert1 Bypass | proposed | live - added U10 |
| `fx_ins2_algo` | Ins2 Algo | list | Off / Drive / Chorus / Delay / Crush | Off | fx | FX Insert2 Algo | proposed | live - added U10 |
| `fx_ins2_a` | Ins2 A | knob | 0-100 % | 50 % | fx | FX Insert2 A | proposed | live - added U10 |
| `fx_ins2_b` | Ins2 B | knob | 0-100 % | 50 % | fx | FX Insert2 B | proposed | live - added U10 |
| `fx_ins2_c` | Ins2 C | knob | 0-100 % | 50 % | fx | FX Insert2 C | proposed | live - added U10 |
| `fx_ins2_mix` | Ins2 Mix | knob | 0-100 % | 100 % | fx | FX Insert2 Mix | proposed | live - added U10 |
| `fx_ins2_bypass` | Ins2 Byp | switch | Active / Bypass | Active | fx | FX Insert2 Bypass | proposed | live - added U10 |
| `fx_mst_algo` | Mst Algo | list | Off / Comp / Limit / Width / Tilt | Off | fx | FX Master Algo | proposed | live - added U10 |
| `fx_mst_a` | Mst A | knob | 0-100 % | 50 % | fx | FX Master A | proposed | live - added U10 |
| `fx_mst_b` | Mst B | knob | 0-100 % | 50 % | fx | FX Master B | proposed | live - added U10 |
| `fx_mst_c` | Mst C | knob | 0-100 % | 50 % | fx | FX Master C | proposed | live - added U10 |
| `fx_mst_mix` | Mst Mix | knob | 0-100 % | 40 % | fx | FX Master Mix | proposed | live - added U10 |
| `fx_mst_bypass` | Mst Byp | switch | Active / Bypass | Active | fx | FX Master Bypass | proposed | live - added U10 |
| `fx_out_level` | Out Level | knob | -inf-+12 dB | 0 dB | fx | FX Out Level | proposed | live - added U10 |

---

## U7 code surface (source of truth for `live`)

From `src/Keith.h` / `src/Keith.cpp`:

| Code enum | ID | Host name | Manifest `id` |
|---|---|---|---|
| `kParamGain` | 0 | Gain | `gain` |
| `kParamTapeSat` … `kParamTapeBypass` | 1–8 | FX Tape * | `fx_tape_*` |
| `kParamEngine` | 9 | Synth Engine | `synth_engine` |
| `kParamPolyMode` | 10 | Synth Poly Mode | `synth_poly_mode` |
| `kParamPorta` | 11 | Synth Porta | `synth_porta` |
| `kParamUnisonVoices` | 12 | Synth Unison Voices | `synth_unison_voices` |
| `kParamUnisonDetune` | 13 | Synth Unison Detune | `synth_unison_detune` |
| `kParamOsc1Shape` … `kParamOsc1PW` | 14–17 | Synth Osc1 * | `synth_osc1_*` |
| `kParamOsc2Shape` … `kParamOsc2PW` | 18–21 | Synth Osc2 * | `synth_osc2_*` |
| `kParamSubLevel` | 22 | Synth Sub Level | `synth_sub_level` |
| `kParamNoiseLevel` | 23 | Synth Noise Level | `synth_noise_level` |
| `kParamFmAmount` | 24 | Synth FM Amount | `synth_fm_amount` |
| `kParamFilterType` … `kParamFilterEnv` | 25–28 | Synth Filter * | `synth_filter_*` |
| `kParamAmpAttack` … `kParamAmpRelease` | 29–32 | Synth Amp * | `synth_amp_*` |
| `kParamPitchAttack` … `kParamPitchAmount` | 33–37 | Synth Pitch * | `synth_pitch_*` |
| `kParamSynthMasterLevel` | 38 | Synth Master Level | `synth_master_level` |
| `kParamFiltAttack` … `kParamFiltRelease` | 39–42 | Synth Filt * | `synth_filt_*` |

**Migration note:** No existing IDs moved. Synth params are new appends. Pitch envelope rows (`synth_pitch_*`) and pulse-width rows (`synth_osc1_pw`, `synth_osc2_pw`) are new vs the U5 intent table (U5 listed filter ADSR but not pitch ADSR / PW).

`kNumParams` at the U7 checkpoint was **43**. U9 below records the next append checkpoint.

---

## U9 code surface (source of truth for `live`)

Appended after ID 42. Host names are the `Init*` strings in `src/Keith.cpp`. Defaults are inert.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---|---|---|---|
| `kParamOscSync` | 43 | Synth Osc Sync | `synth_osc_sync` | Free |
| `kParamOscRing` | 44 | Synth Osc Ring | `synth_osc_ring` | Off |
| `kParamFilterKeyTrack` | 45 | Synth Filter Key Track | `synth_filter_key_track` | 0 % |
| `kParamFilterDrive` | 46 | Synth Filter Drive | `synth_filter_drive` | 0 % |
| `kParamLfoRate` | 47 | Synth LFO Rate | `synth_lfo_rate` | 1 Hz |
| `kParamLfoShape` | 48 | Synth LFO Shape | `synth_lfo_shape` | Sine |
| `kParamLfoDest` | 49 | Synth LFO Dest | `synth_lfo_dest` | Off |
| `kParamLfoAmount` | 50 | Synth LFO Amount | `synth_lfo_amount` | 0 % |
| `kParamModWheelDest` | 51 | Synth Mod Dest | `synth_mod_wheel` | Off |
| `kParamModWheelAmount` | 52 | Synth Mod Amount | `synth_mod_amount` | 0 % |
| `kParamVelToAmp` | 53 | Synth Vel To Amp | `synth_vel_to_amp` | 0 % |
| `kParamVelToFilter` | 54 | Synth Vel To Filter | `synth_vel_to_filter` | 0 % |
| `kParamMonoLegato` | 55 | Synth Mono Legato | `synth_mono_legato` | Retrig |

DSP owner is `src/synth/CoreSynth.h` (`Voice::Process`, `Lfo`, `HeldKeyTracker`). CC1 is stored from the MIDI message in `KeithDSP::TrackPerformanceMidi` because `VoiceAllocator` addresses a controller by its CC number, so `SetControl` does not reach a voice playing any other note. `SetControl` still writes the same runtime value when it does fire.

`kNumParams` = **56**.

---

## U10 code surface (source of truth for `live`)

Appended after ID 55. Insert 1 and Insert 2 share the same algorithm set. Every algorithm defaults
to Off, both bypass controls default to Active, and output level defaults to 0 dB.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---|---|---|---|
| `kParamIns1Algo` | 56 | FX Insert1 Algo | `fx_ins1_algo` | Off |
| `kParamIns1A` | 57 | FX Insert1 A | `fx_ins1_a` | 50 % |
| `kParamIns1B` | 58 | FX Insert1 B | `fx_ins1_b` | 50 % |
| `kParamIns1C` | 59 | FX Insert1 C | `fx_ins1_c` | 50 % |
| `kParamIns1Mix` | 60 | FX Insert1 Mix | `fx_ins1_mix` | 100 % |
| `kParamIns1Bypass` | 61 | FX Insert1 Bypass | `fx_ins1_bypass` | Active |
| `kParamIns2Algo` | 62 | FX Insert2 Algo | `fx_ins2_algo` | Off |
| `kParamIns2A` | 63 | FX Insert2 A | `fx_ins2_a` | 50 % |
| `kParamIns2B` | 64 | FX Insert2 B | `fx_ins2_b` | 50 % |
| `kParamIns2C` | 65 | FX Insert2 C | `fx_ins2_c` | 50 % |
| `kParamIns2Mix` | 66 | FX Insert2 Mix | `fx_ins2_mix` | 100 % |
| `kParamIns2Bypass` | 67 | FX Insert2 Bypass | `fx_ins2_bypass` | Active |
| `kParamMstAlgo` | 68 | FX Master Algo | `fx_mst_algo` | Off |
| `kParamMstA` | 69 | FX Master A | `fx_mst_a` | 50 % |
| `kParamMstB` | 70 | FX Master B | `fx_mst_b` | 50 % |
| `kParamMstC` | 71 | FX Master C | `fx_mst_c` | 50 % |
| `kParamMstMix` | 72 | FX Master Mix | `fx_mst_mix` | 40 % |
| `kParamMstBypass` | 73 | FX Master Bypass | `fx_mst_bypass` | Active |
| `kParamOutLevel` | 74 | FX Out Level | `fx_out_level` | 0 dB |

`src/fx/PatchEffects.h` owns the insert and master algorithms. `src/Keith_DSP.h` owns the chain
order. `src/Keith.cpp` maps host values to DSP parameters and stores all 75 values in each baked
preset chunk. `src/editor/EditorialLayout.h` exposes the material-free `07 FX` page.

`kNumParams` = **75**.

---

## U11 code surface (source of truth for `live`)

Appended after ID 74. The slot controls edit the slot selected by `kParamDrumSlotSelect`. All 16
slot records are saved in the plugin state chunk.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---:|---|---|---|
| `kParamDrumSlotSelect` | 75 | Drum Slot Select | `drum_slot_select` | 1 |
| `kParamDrumSlotLevel` | 76 | Drum Slot Level | `drum_slot_level` | 0 dB |
| `kParamDrumSlotPan` | 77 | Drum Slot Pan | `drum_slot_pan` | C |
| `kParamDrumSlotTune` | 78 | Drum Slot Tune | `drum_slot_tune` | 0 st |
| `kParamDrumSlotDecay` | 79 | Drum Slot Decay | `drum_slot_decay` | 70 % |
| `kParamDrumSlotFilter` | 80 | Drum Slot Filter | `drum_slot_filter` | 12 kHz |
| `kParamDrumSlotDrive` | 81 | Drum Slot Drive | `drum_slot_drive` | 0 % |
| `kParamDrumSlotMute` | 82 | Drum Slot Mute | `drum_slot_mute` | off |
| `kParamDrumSlotSolo` | 83 | Drum Slot Solo | `drum_slot_solo` | off |
| `kParamDrumKitLevel` | 84 | Drum Kit Level | `drum_kit_level` | 0 dB |

`src/drums/DrumSynth.h` owns the 16 synthesized voices and deterministic round-robin state.
`src/Keith_DSP.h` owns MIDI splitting and sums the stereo drum bus before patch effects.
`src/editor/EditorialLayout.h` exposes the live grid and all ten host parameters.

The brief says "11 appends" but lists ten host parameters. Swing is explicitly not implemented and
pads are MIDI events, not parameters. The code therefore appends the ten named parameters and does
not invent an eleventh ID.

`kNumParams` at the U11 checkpoint was **85**.

---

## U13 code surface (source of truth for `live`)

Appended after ID 84. LFO 2 is independent of LFO 1. Destination and shape use the same enums. Defaults are inert: destination Off, amount 0. The host rate default is 1 Hz, matching LFO 1 (`InitDouble` in `src/Keith.cpp`).

`ModDest` is Off, Cutoff, Pitch, Shape, Level, FM. FM adds to the FM oscillator depth. `LfoShape` is Sine, Tri, Saw, Square, S&H. The modulation page lists all six destinations and all five shapes for both LFOs. The mod wheel destination list is the same six.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---:|---|---|---|
| `kParamLfo2Rate` | 85 | Synth LFO2 Rate | `synth_lfo2_rate` | 1 Hz |
| `kParamLfo2Shape` | 86 | Synth LFO2 Shape | `synth_lfo2_shape` | Sine |
| `kParamLfo2Dest` | 87 | Synth LFO2 Dest | `synth_lfo2_dest` | Off |
| `kParamLfo2Amount` | 88 | Synth LFO2 Amount | `synth_lfo2_amount` | 0 % |

`src/synth/CoreSynth.h` owns `mLfo2` and routes it with `ApplyModRoute`. `src/editor/EditorialLayout.h` binds both LFOs as sub-tabs on page 02 MODULATION. IDs 0-84 are unchanged.

`kNumParams` = **89**.

---

## U15 code surface (source of truth for `live`)

Appended after ID 88. Each slot uses the existing `ModDest` set and has an independent source,
destination and bipolar amount. Source Off, destination Off and amount 0 are inert defaults.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---:|---|---|---|
| `kParamMod1Source` | 89 | Mod1 Src | `synth_mod1_source` | Off |
| `kParamMod1Dest` | 90 | Mod1 Dest | `synth_mod1_dest` | Off |
| `kParamMod1Amount` | 91 | Mod1 Amt | `synth_mod1_amount` | 0 % |
| `kParamMod2Source` | 92 | Mod2 Src | `synth_mod2_source` | Off |
| `kParamMod2Dest` | 93 | Mod2 Dest | `synth_mod2_dest` | Off |
| `kParamMod2Amount` | 94 | Mod2 Amt | `synth_mod2_amount` | 0 % |
| `kParamMod3Source` | 95 | Mod3 Src | `synth_mod3_source` | Off |
| `kParamMod3Dest` | 96 | Mod3 Dest | `synth_mod3_dest` | Off |
| `kParamMod3Amount` | 97 | Mod3 Amt | `synth_mod3_amount` | 0 % |
| `kParamMod4Source` | 98 | Mod4 Src | `synth_mod4_source` | Off |
| `kParamMod4Dest` | 99 | Mod4 Dest | `synth_mod4_dest` | Off |
| `kParamMod4Amount` | 100 | Mod4 Amt | `synth_mod4_amount` | 0 % |

`src/synth/CoreSynth.h` owns the matrix source values and sends every slot through the existing
`ApplyModRoute` helper. Slots on the same destination sum. The matrix sum is clamped to one
full-scale route per destination before it is added to the fixed LFO 1, LFO 2 and mod-wheel routes.
`src/editor/EditorialLayout.h` exposes all four rows on the `MATRIX` sub-tab. IDs 0-88 are unchanged.

`kNumParams` = **101** at the end of U15. U17 below takes it to **111**.

---

## U17 code surface (source of truth for `live`)

Appended after ID 100. Every default is 0, so earlier presets stay on the previous voice.
Key and bend knobs use ±100% as ±2 octaves of change per octave from MIDI note 60.
Bend is that amount times octaves squared. Filter key track itself now runs to 200%
(two octaves of cutoff per octave). Stereo width 0 leaves the voice mono and bit-identical.

| Code enum | ID | Host name | Manifest `id` | Default |
|---|---:|---|---|---|
| `kParamNoiseKey` | 101 | Synth Noise Key | `synth_noise_key` | 0 % |
| `kParamAmpAttackKey` | 102 | Synth Amp Attack Key | `synth_amp_attack_key` | 0 % |
| `kParamAmpAttackCurve` | 103 | Synth Amp Attack Curve | `synth_amp_attack_curve` | 0 % |
| `kParamAmpDecayKey` | 104 | Synth Amp Decay Key | `synth_amp_decay_key` | 0 % |
| `kParamAmpDecayCurve` | 105 | Synth Amp Decay Curve | `synth_amp_decay_curve` | 0 % |
| `kParamStereoDelay` | 106 | Synth Stereo Delay | `synth_stereo_delay` | 0 ms |
| `kParamStereoDelayKey` | 107 | Synth Stereo Delay Key | `synth_stereo_delay_key` | 0 % |
| `kParamStereoSpread` | 108 | Synth Stereo Spread | `synth_stereo_spread` | 0 % |
| `kParamDirectLevel` | 109 | Synth Direct Level | `synth_direct_level` | 0 % |
| `kParamFilterKeyCurve` | 110 | Synth Filter Key Curve | `synth_filter_key_curve` | 0 % |

`kNumParams` = **111**.

---

## Row counts (U17 reconcile)

| Metric | Count |
|---|---|
| Total rows | **137** |
| Real parameters (`kNumParams`, IDs 0-110) | **111** |
| Host preset action, not a parameter (`preset_family`) | **1** |
| MIDI pad rows, live but not parameters | **16** |
| Rows `not yet implemented` | **8** |
| Rows `not applicable` | **1** (`drum_kit_swing`) |
| Rows added for LFO 2 | **4** (`synth_lfo2_rate`, `synth_lfo2_shape`, `synth_lfo2_dest`, `synth_lfo2_amount`) |
| Rows added for the modulation matrix | **12** (`synth_mod1_*` through `synth_mod4_*`) |
| Rows added for U17 | **10** (noise key, amp attack/decay key and bend, stereo delay/track/width, direct, filter key bend) |

---

## Notes for the editor lane

1. Visible editor controls stay 1:1 with real params. The editor exposes live IDs 0-110. Preset families and baked preset selection are host preset actions, not automatable parameter rows.
2. `gain` remains overall output gain (smoothed). `synth_master_level` is synth-bus level in dB before that gain/tape chain.
3. Deferred intentionally: `synth_part` and the remaining `preset_*` actions. Two LFOs, CC1 and the four-slot modulation matrix are live.
4. No Korg, Splice, Freakshow, Surge, or OB-XF names appear in `label` or `automatable`.
