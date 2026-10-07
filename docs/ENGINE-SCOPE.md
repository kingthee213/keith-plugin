# Keith — Engine Scope (clean-room plan)

Written before DSP implementation, U4 pass. Locked spec: `BRIEF.md` §§3–4, §7.
Run rules: `C:/Users/noaha/.agents/orchestrate/keith-plugin/preferences.md`.

This document is the permission and behaviour contract for the Keith engine. Nothing enters the tree
unless its origin and licence are named here or in `banks/SOURCES.md`.

---

## 1. Clean-room rule (all engines)

| Source | Licence | Ingestion |
|---|---|---|
| Mishby (Freakshow Industries) | Personal, non-transferable EULA | **Never.** Behaviour only; no code, binaries, or byte-level imitation. |
| Surge (`surge-synthesizer/surge`) | GPL-3.0 | **Never.** Study reference: manuals, listening, A/B by ear. |
| OB-Xf (`surge-synthesizer/OB-Xf`) | GPL-3.0 | **Never.** Study reference: manuals, listening, A/B by ear. |
| Airwindows tape family (`airwindows/airwindows`) | MIT | **Seed algorithms only.** Concepts reimplemented in our own code; attribution in licence notes. |
| iPlug2 / WDL | zlib-like | Framework; already in tree via bootstrap. |

**Distribution context:** this plugin is handed to another person. GPL synth source cannot be
vendored, forked, or line-ported. Filter coefficients, wavetables, and LUTs from GPL projects are
out of scope.

---

## 2. Mishby character — tape / analog degradation block

### 2.1 Target behaviour (BRIEF §3)

Tape machine / analog degradation usable from subtle glue to full destruction:

| Behaviour | Description | Routing |
|---|---|---|
| Saturation | Tape-style soft clipping / harmonic thickening | Global bus + per-part FX |
| Head bump | Low-mid resonant lift (~60–120 Hz) | Global bus + per-part FX |
| Wow | Slow pitch drift (≈0.1–2 Hz) | Global bus + per-part FX |
| Flutter | Faster pitch wobble (≈3–15 Hz) | Global bus + per-part FX |
| Hiss | Broadband noise floor, brighter at higher drive | Global bus + per-part FX |
| Print-through | Delayed, attenuated ghost of prior audio | Global bus + per-part FX |
| Unstable pitch | Combined wow/flutter depth → obvious detune | Emerges from wow + flutter at high settings |
| Mix / bypass | Wet/dry and hard bypass | Global bus + per-part FX |

Parameters (locked ids from `docs/PARAM-MANIFEST.md`):

| id | Host automatable name | Range | Default |
|---|---|---|---|
| `fx_tape_sat` | FX Tape Saturation | 0…100 % | 25 % |
| `fx_tape_bump` | FX Tape Head Bump | 0…100 % | 20 % |
| `fx_tape_wow` | FX Tape Wow | 0…100 % | 10 % |
| `fx_tape_flutter` | FX Tape Flutter | 0…100 % | 10 % |
| `fx_tape_hiss` | FX Tape Hiss | 0…100 % | 5 % |
| `fx_tape_print` | FX Tape Print | 0…100 % | 0 % |
| `fx_tape_mix` | FX Tape Mix | 0…100 % | 40 % |
| `fx_tape_bypass` | FX Tape Bypass | off/on | off |

Per-part instances mirror the same parameter set (scoped to synth part busses). Global bus runs on
the master sum after all parts.

### 2.2 Airwindows MIT seed modules

Algorithms are **reimplemented** in `src/fx/` from published behaviour descriptions. No Airwindows
source files are copied into this repo.

| Airwindows module | MIT seed for | Repo path (reference only) |
|---|---|---|
| **Tape** | Spiral-style saturation curve; head-bump tone lift | `plugins/WinVST/Tape/` |
| **ToTape7** | Drive staging; bias/dubly-style HF emphasis around saturation | `plugins/WinVST/ToTape7/` |
| **Flutter** | Slow wow / wide pitch drift (ToTape6 flutter, standalone) | `plugins/WinVST/Flutter/` |
| **Flutter2** | Faster flutter; stereo de-correlation (ToTape7 flutter) | `plugins/WinVST/Flutter2/` |
| **Noise** | Input-triggered brownian hiss reinforcement | `plugins/WinVST/Noise/` |
| **TapeDust** | Treble-erode slew noise layered on hiss | `plugins/WinVST/TapeDust/` |

**Print-through** has no named Airwindows counterpart; implemented from first principles (delayed,
attenuated feedback of the wet chain).

**MIT licence and attribution** (Chris Johnson / Airwindows, 2018):

```
MIT License

Copyright (c) 2018 Chris Johnson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

**Attribution line** (recorded in `README.md` licence notes):

> Tape/analog DSP in Keith is an original implementation seeded from algorithm ideas in the Airwindows
> open-source library (MIT Licence, Copyright (c) 2018 Chris Johnson). No Airwindows source code is
> included in this repository.

### 2.3 Listening tests (tape block)

| Setting | Reference | Pass criterion (ear) |
|---|---|---|
| Sat 10 %, Mix 20 %, rest 0 | Clean sine → Keith subtle | Barely thicker; level stable; no obvious pitch wobble |
| Sat 80 %, Bump 60 %, Mix 100 % | Airwindows **Tape** @ moderate drive | Audible harmonics + low-mid weight; not brickwall clipped |
| Wow 40 %, Flutter 20 % | Airwindows **Flutter** + **Flutter2** @ 0.3–0.5 | Slow pitch drift; mono compatibility acceptable |
| Hiss 30 % | Airwindows **Noise** @ low wet | Noise floor rises with material; not pure white hiss |
| Print 50 % | Prior bar ghost on steady tone | Delayed quiet repeat audible after ~1 s sustain |
| All max, Mix 100 % | Mishby-style destruction (ear only) | Obvious degradation: detune, noise, saturation; still outputs signal |

### 2.4 Implementation layout (U4)

```
src/fx/TapeAnalog.h    — header-only tape/analog processor (global + per-part instances)
```

U4 wires **global bus** on master output and **one per-part instance** on the synth bus (parts 2–4
gain separate parameter sets when the part engine lands).

---

## 3. Surge-style engine (BRIEF §4.1) — future pass

Clean-room mimic. No Surge source, wavetables, or filter coefficient tables.

### 3.1 Behaviours and references

| Behaviour | Reference (study only) | Listening test |
|---|---|---|
| Wavetable oscillator | Surge 1.x User Manual — Oscillator / WT section | Single WT morph: bright → harsh sweep matches manual examples by ear |
| Sine / alias-free shapes | Same manual — Classic oscillators | Sine purity; saw brightness without obvious aliasing at 44.1 kHz |
| FM / RM / PD hybrid modes | Surge manual — FM, Ring Mod, Phase Distortion | FM index 0→max: timbre darkens then grits; RM adds sidebands |
| Modulation matrix | Surge manual — Mod Matrix | LFO → cutoff: depth 0→100 % sweeps filter audibly |
| Filter set (LP/HP/BP, 12/24 dB) | Surge manual — Filter block | LP24 resonance self-oscillates near max; LP12 gentler slope |
| Scene / part routing | Surge manual — Scenes & Parts | Per-part engine select does not bleed voices across parts |

**GPL boundary:** `github.com/surge-synthesizer/surge` is never cloned into `third_party/`. Manual
PDFs and factory demo recordings are the only inputs.

---

## 4. OB-XF style engine (BRIEF §4.2) — future pass

Clean-room mimic. No OB-Xf source.

### 4.1 Behaviours and references

| Behaviour | Reference (study only) | Listening test |
|---|---|---|
| Dual oscillator + sub | OB-Xf manual — Oscillators | Two detuned saws + sub: classic stack; sub one octave down |
| Poly portamento | OB-Xf manual — Glide / Portamento | Legato retrigger glides pitch; staccato does not |
| OB-X ladder / sem filter | OB-Xf manual — Filter (24 dB ladder, SEM modes) | Ladder: resonance bump at cutoff; SEM: brighter, less aggressive resonance |
| Unison | OB-Xf manual — Unison | 2–8 voices: widen stereo; detune increases beating |
| Envelope | OB-Xf manual — Amp / Filter env | Filter env opens/closes timbre on pad |

**GPL boundary:** `github.com/surge-synthesizer/OB-Xf` is never vendored.

---

## 5. Dual-engine architecture (BRIEF §4.3) — future pass

| Behaviour | Reference | Listening test |
|---|---|---|
| Engine select per part | `synth_engine` param (Hybrid / Analog) | Part 1 Hybrid WT+FM; Part 2 Analog 2-osc: timbres differ, no crash |
| Shared master FX | BRIEF §7 | Global tape bus affects all parts equally |
| Per-part insert FX | BRIEF §7 | Part tape bypass independent of global tape bypass |

Voice allocator: 16 voices shared across parts (exact voice budget TBD in implementation pass).

---

## 6. Build order after U4

1. **U4:** `docs/ENGINE-SCOPE.md` + tape/analog block (global + per-part).
2. **U7 (landed):** Core subtractive vertical slice in `src/synth/CoreSynth.h` — 2 osc + sub/noise,
   Hybrid/Analog select, multimode filter, amp/pitch/filter envs, poly/mono/porta/unison. Not a full
   OB-Xf ladder or Surge WT/matrix yet.
3. **Next composer pass:** Deeper Analog ladder / SEM behaviour + Hybrid wavetable morph fidelity.
4. **Then:** Modulation matrix / LFO destinations.
5. **U10 (landed):** Two patch inserts plus one master effect, with preset serialization and the
   material-free `07 FX` page.
6. **U11 (landed):** 16-voice synthesized percussion engine, contiguous MIDI map and live pad
   editor. No sample content entered the repository.
7. **Then:** Part routing and data-driven Family-01 chain assignments from bank lists.

---

## 7. Patch insert and master FX (U10)

The real processing order is:

**voices -> synth bus -> Insert 1 -> Insert 2 -> tape bus -> Master FX -> output level -> existing gain**

Insert 1 and Insert 2 each select one of these algorithms:

| Algorithm | A | B | C | DSP |
|---|---|---|---|---|
| Off | Not used | Not used | Not used | Exact pass-through |
| Drive | Drive | Tone | Bias | Biased soft saturation followed by a variable low-pass tone stage |
| Chorus | Rate | Depth | Spread | Stereo modulated delay with independent phase on each side |
| Delay | Time | Feedback | Tone | Bounded ping-pong feedback delay with a filtered repeat path |
| Crush | Bits | Rate | Jitter | Amplitude quantization plus sample-rate reduction with bounded clock jitter |

The master stage selects one of these algorithms:

| Algorithm | A | B | C | DSP |
|---|---|---|---|---|
| Off | Not used | Not used | Not used | Exact pass-through |
| Comp | Threshold | Ratio | Release | Linked-stereo downward compressor with fixed 5 ms attack |
| Limit | Ceiling | Release | Input | Linked-stereo peak limiter with instantaneous gain reduction |
| Width | Width | Mono Freq | Balance | Mid/side width, low-side removal below the selected frequency, and stereo balance |
| Tilt | Tilt | Pivot | Drive | Complementary low/high tilt around a variable pivot, followed by optional saturation |

Each stage has a true dry/wet control. Off and settled bypass states copy the input exactly.
Continuous parameters and route transitions smooth over 5 ms. Delay memory is allocated during
reset, never inside `ProcessBlock`; feedback is capped and each stage rejects non-finite output.

All 19 host parameters live at IDs 56-74. Baked presets store all 75 host values. Bell Pad,
Motion Pad, and Hard Kit carry explicit U10 chain deltas; every other preset inherits the inert
Off defaults.

---

## 8. Synthesized percussion engine (U11)

The drum section contains no audio files and has no sample loader. `src/drums/DrumSynth.h`
generates 16 distinct voices from sine partials, deterministic noise, exponential one-shot
envelopes, a low-pass filter and soft clipping.

MIDI notes 36-51 go only to the drum engine. Notes outside that range go only to the synth voice
allocator. This fixed split prevents a drum hit from taking, retriggering or releasing a synth
voice. The drum engine keeps one preallocated voice per slot and a fixed 128-event MIDI queue.
Neither `ProcessBlock` nor the per-sample drum path allocates memory.

Each slot stores level, pan, tune, decay, filter, drive, mute and solo. Any soloed slot suppresses
all non-soloed slots. Muted slots and slots at `-inf` level produce exact silence while their
one-shot state continues to advance. Velocity controls level and one of three transient/brightness
bands. Fixed pitch, decay and noise-seed alternates provide deterministic round-robin variation.

The drum bus is stereo. Constant-power pan runs per slot, then the kit level and a bounded bus
stage feed the existing Insert 1 -> Insert 2 -> tape -> Master FX chain. All 16 slots at maximum
settings remain finite.

Host IDs 75-84 expose the ten parameters named by the U11 table. The brief's "11 appends" count is
inconsistent with its ten-row table. Swing is not an ID because this instrument has no sequencer,
and pads are MIDI events rather than automatable parameters. Complete per-slot records are stored
in versioned plugin state chunks. Pre-U11 75-parameter chunks load with default drum records.

---

## 9. Licence audit checklist (engine)

- [x] No Freakshow / Mishby code or assets
- [x] No Surge or OB-Xf source in tree
- [x] Airwindows attribution recorded
- [x] No paid DSP libraries
- [x] No Korg, Splice, Gorillaz, or Green Lantern strings in parameter names
- [x] Drum voices are original synthesis code; no sample assets or sample playback
