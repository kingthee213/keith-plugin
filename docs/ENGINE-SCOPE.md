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

1. **U4 (this pass):** `docs/ENGINE-SCOPE.md` + tape/analog block (global + per-part).
2. **Next composer pass:** OB-XF analog voice (simpler subtractive path first).
3. **Then:** Surge-style hybrid engine + modulation matrix.
4. **Then:** Part routing, drum engine, Triton-family insert FX chains (data-driven from U2 lists).

---

## 7. Licence audit checklist (engine)

- [x] No Freakshow / Mishby code or assets
- [x] No Surge or OB-Xf source in tree
- [x] Airwindows attribution recorded
- [x] No paid DSP libraries
- [x] No Korg, Splice, Gorillaz, or Green Lantern strings in parameter names
