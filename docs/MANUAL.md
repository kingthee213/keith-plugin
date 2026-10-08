# Keith — control and preset manual

Keith is a two-oscillator polyphonic instrument with a synthesized 16-slot drum engine, a tape bus,
two insert effects and one master effect. It ships as a Windows VST3 and a Windows standalone that
share one editor. The editor wordmark reads **Yote Machine** and the service plate reads
**The Youth Hostel DSP**; the plugin, the binary and the preset names are all **Keith**.

Every control described here is wired to audio. Nothing in this manual is aspirational — see
[What this manual does not claim](#what-this-manual-does-not-claim) for the boundary, and
[Where these claims come from](#where-these-claims-come-from) for the file behind each section.

- 111 host parameters, IDs 0–110. Full table with ranges and defaults:
  [`PARAM-MANIFEST.md`](PARAM-MANIFEST.md).
- 103 factory presets in five families of 20, 21, 21, 20 and 21.
- 16 synth voices, each able to stack up to 8 detuned unison copies.
- No audio files. The drum section is synthesized; so is everything else.

## Signal path

`src/Keith_DSP.h` runs one chain per block, in this order:

```
16 synth voices  ─┐
                  ├─▶ Insert 1 ─▶ Insert 2 ─▶ part tape ─▶ global tape ─▶ Master FX ─▶ Out Level ─▶ Gain ─▶ out
stereo drum bus  ─┘
```

Three things about that chain are worth knowing before you edit anything:

The drum bus is summed in **before** the inserts, so Insert 1, Insert 2, the tape and the master
effect all process the drums together with the synth. There is no separate drum output.

The **part tape** stage has no control and is inaudible. `Keith::SyncTapeParams` sets all four part
tape stages to bypassed with their mix at zero on every sync, and `ProcessBlock` only runs the
first of the four, so the only tape you can reach is the global one on page 05. The part stages are
scaffolding for the multi-part routing that is not built.

**Out Level** and **Gain** are two separate attenuators at the end. Out Level is the FX page's
post-master trim (−∞ to +12 dB, ID 74). Gain is the always-visible `OUTPUT` knob in the header
(0–100 %, ID 0) and is the last thing applied.

## The window

Four regions stay on screen whichever page you are on.

| Region | Contents |
|---|---|
| Header | `PATCH / FAMILY 01-05` caption, the preset menu, `<` and `>` step buttons, and the `OUTPUT` knob |
| Navigation | Seven page buttons: `01 SOURCE`, `02 MODULATION`, `03 FILTER`, `04 ENVELOPES`, `05 TAPE`, `06 LIBRARY`, `07 FX` |
| Channel strip (right) | `FILTER / VOICE`: the filter-type tabs, `CUTOFF`, `RESONANCE`, `ENV DEPTH`, the engine and poly tabs, `GLIDE`, `UNISON`, `DETUNE` |
| Footer | `MIDI IN / A-L PLAY`, `Z/X OCTAVE`, and an on-screen keyboard spanning MIDI notes 36–84 |

`<` and `>` step through the 109 presets and wrap at both ends.

Knobs take a vertical drag. Ctrl-drag is fine control and a double-click returns the default; the
editor prints that on the page 06 `CONTROL NOTES` list. Envelope values can also be typed by
clicking the number.

### Channel strip

| Control | Values | Notes |
|---|---|---|
| filter-type tabs | `LP24` `LP12` `BP` `HP` | 24 dB and 12 dB lowpass, bandpass, highpass |
| `CUTOFF` | 20 Hz – 20 kHz | clamped in the voice to 20 Hz … 0.45 × sample rate |
| `RESONANCE` | 0–100 % | |
| `ENV DEPTH` | −100…+100 % | bipolar; how much the filter envelope moves cutoff |
| engine tabs | `HYBRID` `ANALOG` | Analog softens the post-filter signal and biases shape and FM depth down; Hybrid biases them up |
| poly tabs | `POLY` `MONO` | |
| `GLIDE` | 0–1000 ms | portamento |
| `UNISON` | 1–8 | copies per voice |
| `DETUNE` | 0–100 % | spreads the unison copies across roughly 35 cents |

## 01 SOURCE

Two oscillator rows, then a mixer column.

Each row has `MORPH`, a live waveform display, a text readout, `TUNE`, `LEVEL` and `PULSE WIDTH`.
`MORPH` sweeps one continuous shape in three equal segments and the readout names where you are:
**SINE → TRI → SAW → PULSE**. Between two named shapes it shows the blend. `PULSE WIDTH` runs
5–95 % and only does anything in the top third of the morph, because that is the only segment the
pulse wave is blended into.

| Control | Range | Parameter |
|---|---|---|
| `MORPH` | 0–100 % | `synth_osc1_shape` / `synth_osc2_shape` |
| `TUNE` | −24…+24 semitones | `synth_osc1_pitch` / `synth_osc2_pitch` |
| `LEVEL` | 0–100 % | `synth_osc1_level` / `synth_osc2_level` |
| `PULSE WIDTH` | 5–95 % | `synth_osc1_pw` / `synth_osc2_pw` |

`SOURCE MIX` holds `SUB` (0–100 %), `NOISE` (0–100 %), `FM` (0–100 %) and `SYNTH LEVEL`
(−60…+6 dB, the synth bus trim before the tape and gain chain). `FM` is phase modulation from
oscillator 2 into oscillator 1's frequency, and the engine tab scales it: ×1.25 on `HYBRID`,
×0.85 on `ANALOG`.

Two switches sit below. `OSC SYNC` (`FREE` / `SYNC`) makes oscillator 2 the master — its cycle wrap
restarts oscillator 1. `RING` (`OFF` / `ON`) adds the product of the two oscillators and is a
separate path from `FM`, so you can run both.

## 02 MODULATION

The left half is a three-tab desk: `LFO 1`, `LFO 2`, `MATRIX`. The right half is always visible.

### LFO 1 and LFO 2

The two are independent and identical in layout.

| Control | Values |
|---|---|
| `RATE` | 0.01–30 Hz |
| `AMOUNT` | −100…+100 % (bipolar) |
| `SHAPE` | `SINE` `TRI` `SAW` `SQUARE` `S&H` |
| `DEST` | `OFF` `CUTOFF` `PITCH` `SHAPE` `LEVEL` `FM` |

### MATRIX

Four rows numbered 1–4. Each row is a `SOURCE` menu, a `DESTINATION` menu and a bipolar `AMOUNT`
knob.

- `SOURCE`: `OFF` `LFO 1` `LFO 2` `MOD WHEEL` `VELOCITY` `KEY`
- `DESTINATION`: `OFF` `CUTOFF` `PITCH` `SHAPE` `LEVEL` `FM`

Rows pointed at the same destination sum. The matrix total per destination is then clamped to one
full-scale route before it is added to the fixed LFO 1, LFO 2 and mod-wheel routes, so four rows
on `CUTOFF` cannot drive the filter four times as far as one row can. `OFF` and a zero amount are
exact no-ops.

All four matrix rows are inert in all 103 factory presets. That is deliberate and the
`preset-restore` test asserts it, so the matrix is always yours to use from a clean slate.

### Mod wheel, velocity, legato

| Control | Values | Notes |
|---|---|---|
| `MOD WHEEL` / `DEST` | the six-value destination list | fed from MIDI CC1 |
| `MOD WHEEL` / `AMOUNT` | −100…+100 % | |
| `VELOCITY` / `TO AMP` | 0–100 % | |
| `VELOCITY` / `TO FILTER` | 0–100 % | |
| `MONO LEGATO` | `RETRIG` / `LEGATO` | applies in mono mode |

The page prints `CC1` beside the mod-wheel header because that is the controller it reads.

## 03 FILTER

A `RESPONSE` plot of the current filter type, cutoff and resonance, plus two knobs that are not on
the channel strip:

| Control | Range |
|---|---|
| `KEY TRACK` | 0–200 % — how much the played note moves cutoff. 100 % is one octave of cutoff per octave |
| `DRIVE` | 0–100 % — drive into the filter |

## 04 ENVELOPES

One large envelope desk with three tabs: `VOLUME`, `PITCH`, `FILTER`. Each is a four-stage ADSR you
edit by dragging the handles, or by clicking a value and typing it.

| Tab | Attack | Decay | Sustain | Release | Extra |
|---|---|---|---|---|---|
| `VOLUME` | 0.1 ms – 10 s | 1 ms – 30 s | 0–100 % | 1 ms – 10 s | key and bend under the graph |
| `PITCH` | 0.1 ms – 10 s | 1 ms – 10 s | 0–100 % | 1 ms – 10 s | `PITCH DEPTH`, −24…+24 semitones |
| `FILTER` | 0.1 ms – 10 s | 1 ms – 10 s | 0–100 % | 1 ms – 10 s | depth is `ENV DEPTH` on the channel strip |

The pitch envelope does nothing until `PITCH DEPTH` is non-zero, and the filter envelope does
nothing until `ENV DEPTH` is non-zero. Both default to a shape that is ready and a depth that is
not, which is why a fresh init patch sounds static.

Under the volume graph: `ATK KEY`, `ATK BEND`, `DEC KEY`, `DEC BEND`, `NOISE KEY`, `DIRECT`,
`ST DELAY`, `ST TRACK` and `ST WIDTH`. The filter graph's tab carries `CUT BEND`. Key and bend
run −100 to +100 %, and 100 % is two octaves of change per octave away from middle C. Bend is
that amount times the square of the octave offset, so a time or a cutoff can peak in the middle
of the keyboard. `DIRECT` (0–100 %) adds the oscillators after the filter. `ST DELAY` is 0–40 ms
and `ST WIDTH` (−100 to +100 %) pans a delayed copy to the right; at zero the voice stays mono.
All of these default to zero.

## 05 TAPE

`TAPE / GLOBAL BUS`. One tape stage on the whole mix, after the inserts and before the master
effect. All seven knobs are 0–100 %.

| Control | What it does |
|---|---|
| `DRIVE` | tape saturation |
| `HEAD BUMP` | low-frequency head resonance |
| `WOW` | slow speed variation |
| `FLUTTER` | fast speed variation |
| `HISS` | tape noise floor |
| `PRINT` | print-through |
| `DRY / WET` | tape mix |
| `ACTIVE` / `BYPASS` | tape bypass |

The page is labelled `POST SYNTH` because that is where it sits.

## 06 LIBRARY

Two tabs: `PATCHES` and `DRUMS`.

`PATCHES` lists all 109 presets on the left and a `CONTROL NOTES` reference on the right. Clicking a
row recalls that preset.

`DRUMS` is the drum engine: a 4 × 4 pad grid, seven knobs and two switches. See
[The drum engine](#the-drum-engine).

## 07 FX

Three panels: `INSERT 1`, `INSERT 2`, `MASTER`. Each has an algorithm tab row, three knobs whose
labels change with the algorithm, a `DRY / WET` knob and an `ACTIVE` / `BYPASS` switch. `OUT LEVEL`
(−∞ to +12 dB, drawn from its 0 dB centre) sits below them.

Insert 1 and Insert 2 share one algorithm set and run in series, Insert 1 first.

### Insert algorithms

| Algorithm | Knob A | Knob B | Knob C |
|---|---|---|---|
| `OFF` | `NOT USED` | `NOT USED` | `NOT USED` |
| `DRIVE` | `DRIVE` — gain 1× to 25× | `TONE` — post-drive lowpass, 800 Hz to 18 kHz | `BIAS` — asymmetry, ±0.35 |
| `CHORUS` | `RATE` — 0.05 to 5 Hz | `DEPTH` — a 0.3 to 12 ms sweep added to a 16 ms tap | `SPREAD` — left/right LFO phase offset, 0 to π |
| `DELAY` | `TIME` — 20 ms to 1 s | `FEEDBACK` — up to 0.92, cross-coupled | `TONE` — feedback lowpass, 500 Hz to 18 kHz |
| `CRUSH` | `BITS` — 4 to 16 bits | `RATE` — sample-and-hold rate, 2 % to 100 % of the sample rate | `JITTER` — rate randomisation, up to ±45 % |

### Master algorithms

| Algorithm | Knob A | Knob B | Knob C |
|---|---|---|---|
| `OFF` | `NOT USED` | `NOT USED` | `NOT USED` |
| `COMP` | `THRESHOLD` — −48 to 0 dB | `RATIO` — 1:1 to 20:1 | `RELEASE` — 20 ms to 1 s (attack is fixed at 5 ms) |
| `LIMIT` | `CEILING` — −18 to 0 dB | `RELEASE` — 10 to 500 ms | `INPUT` — 0 to +24 dB of gain *into* the limiter |
| `WIDTH` | `WIDTH` — 0× to 2× on the side signal | `MONO FREQ` — 40 Hz to 500 Hz kept mono | `BALANCE` — left/right, centre at 50 % |
| `TILT` | `TILT` — −12 to +12 dB | `PIVOT` — 200 Hz to 5 kHz | `DRIVE` — 0 to +6 dB with soft clipping |

`LIMIT` / `INPUT` is the one control on this page that can get loud on its own: it is a straight
0 to +24 dB boost before the ceiling, and its parameter default is 50 % — that is +12 dB. Every
factory preset that selects `LIMIT` writes `INPUT` explicitly rather than inheriting that default,
and `tools/verify_patch_fx_chains.py` fails the build if any of them exceeds 40 %.

### Control taper

Six of the fifteen knobs above are squared in the DSP rather than taken straight: the insert
`DRIVE`, the chorus `RATE`, the delay `TIME`, the crush `RATE`, and both master `RELEASE` controls.
Those six move slowly at the bottom of their travel and quickly at the top. The other nine are
linear in whatever unit the table names.

### Switching algorithms

Changing an algorithm does not click. `src/fx/PatchEffects.h` ramps the stage's wet route down to
zero over 5 ms, swaps the algorithm at the zero crossing, then ramps back up. `BYPASS` uses the same
ramp, and every stage output is clamped to ±64 before it leaves.

## The drum engine

Keith has sixteen synthesized percussion slots. `src/drums/DrumSynth.h` builds each hit from
oscillators, deterministic noise, envelopes, a one-pole filter and soft clipping. **There is no
sample playback and no sample map** — nothing in that file reads a file or an asset.

### MIDI split

MIDI notes **36–51** trigger drum slots 01–16 and never reach the synth voice allocator. Notes
0–35 and 52–127 go to the synth only. Drum note-offs are consumed, because the percussion
envelopes are one-shot.

That keeps drum hits from stealing any of the 16 synth voices, and gives a controller one
contiguous 4 × 4 pad bank:

```
top:     P13  P14  P15  P16
         P09  P10  P11  P12
         P05  P06  P07  P08
bottom:  P01  P02  P03  P04
```

### Drum map

| Pad | Slot | Synthesized voice | MIDI | Note | Round robin | Default level | Default pan |
|---|---:|---|---:|---|---:|---:|---:|
| P01 | 01 | Kick | 36 | C1 | 3 | 0 dB | C |
| P02 | 02 | Snare | 37 | C#1 | 4 | −1 dB | C |
| P03 | 03 | Snare Alt / Rim | 38 | D1 | 2 | −3 dB | R10 |
| P04 | 04 | Clap | 39 | D#1 | 3 | −2 dB | L10 |
| P05 | 05 | Closed Hat | 40 | E1 | 4 | −4 dB | R25 |
| P06 | 06 | Open Hat | 41 | F1 | 2 | −4 dB | R30 |
| P07 | 07 | Pedal Hat | 42 | F#1 | 2 | −5 dB | L20 |
| P08 | 08 | Tom Lo | 43 | G1 | 2 | −2 dB | L18 |
| P09 | 09 | Tom Mid | 44 | G#1 | 2 | −2 dB | C |
| P10 | 10 | Tom Hi | 45 | A1 | 2 | −2 dB | R18 |
| P11 | 11 | Ride | 46 | A#1 | 2 | −5 dB | R20 |
| P12 | 12 | Crash | 47 | B1 | 2 | −4 dB | L20 |
| P13 | 13 | Perc 1 | 48 | C2 | 3 | −3 dB | L25 |
| P14 | 14 | Perc 2 | 49 | C#2 | 3 | −6 dB | R25 |
| P15 | 15 | Perc 3 | 50 | D2 | 2 | −5 dB | L30 |
| P16 | 16 | Perc 4 | 51 | D#2 | 2 | −4 dB | R30 |

No default slot is silent, and the level and pan spread leaves a fresh kit balanced without edits.
Kit level defaults to 0 dB.

### Velocity and round robin

Velocity always sets output level. On top of that the engine picks one of three synthesis bands:

| Band | Velocity | Synthesis change |
|---|---:|---|
| Soft | 1–42 | lower transient, darker noise balance |
| Medium | 43–95 | nominal voice |
| Hard | 96–127 | stronger transient, brighter noise balance |

Each slot cycles through the round-robin count in the drum map above. The alternates use small fixed
pitch and decay offsets plus a deterministic noise seed, so repeated hits vary but the same MIDI
phrase always renders the same way. Reset and preset recall restart the sequence.

### Slot controls

The pad grid **selects** a slot; it does not audition. Play MIDI 36–51 to hear a voice. The seven
knobs and two switches then edit the selected slot:

| Control | Range |
|---|---|
| `SLOT LEVEL` | −∞ to +6 dB |
| `PAN` | L100 to R100 |
| `TUNE` | −12 to +12 semitones |
| `DECAY` | 0–100 % |
| `FILTER` | 20 Hz – 20 kHz |
| `DRIVE` | 0–100 % |
| `KIT LEVEL` | −∞ to +6 dB (the whole kit) |
| `MUTE` | `OFF` / `MUTE` |
| `SOLO` | `OFF` / `SOLO` |

If any slot is soloed, every non-soloed slot is silent. `MUTE`, and a slot level of −∞, both give
exact silence. All 16 slot records are saved in the plugin state chunk, not just the selected one.

There is no swing control. Swing moves generated steps and Keith has no sequencer or pattern
player, so there are no steps to move.

## Preset library

109 presets in five families, ordered 01 → 05 and never reordered. A preset is a set of deltas over
the init defaults, baked into a state chunk at construction, so recalling one is the same code path
the host uses to load a saved session.

Each preset also carries a deliberate FX chain. Insert 1 and the master stage are engaged on all
109; Insert 2 is engaged where the role wants more space, and is `Off` on every Family 05 preset.
Mix percentages below are the stage's `DRY / WET`, so they tell you how hard each chain is working.

Family 01 / 021 Upright Bass, 022 Hammer Keys, 023 Swell Choir, 024 Bright Reed, 025 Snap Bass and
026 Short Bass are synthesis matched by analysis to Free Wave Samples recordings by Jason Champion
(freewavesamples.com): acoustic bass at C2, C3 and C4, acoustic piano at C1, C3 and C5, the slow
choir at C4, the harmonica at C5, and the short bass at C2 and C3. No audio from those recordings
was ingested or shipped.

<!-- BEGIN GENERATED PRESET TABLE -->

### Family 01

| # | Preset | Role | Insert 1 | Insert 2 | Master |
|---:|---|---|---|---|---|
| 0 | 001 Glass Keys | keys | Chorus 20% | Delay 14% | Width 24% |
| 1 | 002 Sparse Keys | keys | Delay 14% | Off | Width 20% |
| 2 | 003 Soft Tines | electric piano | Chorus 20% | Off | Tilt 18% |
| 3 | 004 Bright Clav | clav | Drive 28% | Off | Comp 26% |
| 4 | 005 Drawbar Organ | organ | Chorus 18% | Off | Comp 22% |
| 5 | 006 Glass Bells | bells | Delay 18% | Chorus 16% | Width 26% |
| 6 | 007 Crystal Pluck | pluck | Delay 16% | Off | Comp 20% |
| 7 | 008 Key Stab | stab | Delay 22% | Drive 20% | Comp 28% |
| 8 | 009 Dry Bass | bass | Drive 22% | Off | Comp 26% |
| 9 | 010 Smooth Lead | lead | Delay 24% | Chorus 14% | Comp 22% |
| 10 | 011 Warm Brass | brass | Chorus 22% | Off | Comp 35% |
| 11 | 012 Silk Strings | strings | Chorus 30% | Delay 16% | Width 28% |
| 12 | 013 Soft Choir | choir | Chorus 28% | Delay 18% | Width 26% |
| 13 | 014 Bell Pad | pad | Delay 24% | Chorus 22% | Width 30% |
| 14 | 015 Slow Motion | motion | Chorus 24% | Delay 30% | Width 40% |
| 15 | 016 Shimmer Texture | texture | Crush 18% | Delay 20% | Tilt 30% |
| 16 | 017 Low Drone | drone | Chorus 20% | Delay 18% | Tilt 22% |
| 17 | 018 Glass Arp | arp | Delay 16% | Off | Comp 20% |
| 18 | 019 Heavy Delay | fx | Delay 78% | Crush 40% | Comp 65% |
| 19 | 020 Room Kit | kit | Drive 25% | Crush 18% | Limit 50% |
| 20 | 021 Upright Bass | bass | Drive 5% | Off | Comp 5% |
| 21 | 022 Hammer Keys | keys | Chorus 1% | Off | Tilt 12% |
| 22 | 023 Swell Choir | choir | Chorus 5% | Off | Width 5% |
| 23 | 024 Bright Reed | lead | Drive 5% | Off | Tilt 12% |
| 24 | 025 Snap Bass | bass | Chorus 5% | Off | Comp 12% |
| 25 | 026 Short Bass | bass | Drive 5% | Off | Comp 12% |

### Family 02

| # | Preset | Role | Insert 1 | Insert 2 | Master |
|---:|---|---|---|---|---|
| 26 | 001 Neon Keys | keys | Drive 30% | Off | Comp 26% |
| 27 | 002 Thin Piano | electric piano | Chorus 12% | Off | Tilt 16% |
| 28 | 003 Thin Clav | clav | Drive 34% | Off | Comp 28% |
| 29 | 004 Perc Organ | organ | Drive 22% | Off | Comp 24% |
| 30 | 005 Metal Bells | bells | Delay 20% | Drive 18% | Width 24% |
| 31 | 006 Neon Pluck | pluck | Drive 24% | Off | Comp 22% |
| 32 | 007 Sparse Pluck | pluck | Delay 8% | Off | Comp 14% |
| 33 | 008 Thin Stab | stab | Drive 30% | Off | Comp 30% |
| 34 | 009 Hard Bass | bass | Drive 40% | Off | Limit 55% |
| 35 | 010 Sparse Bass | bass | Drive 14% | Off | Comp 18% |
| 36 | 011 Sync Lead | lead | Drive 32% | Delay 16% | Comp 26% |
| 37 | 012 Stab Brass | brass | Drive 26% | Off | Comp 30% |
| 38 | 013 Tight Strings | strings | Chorus 16% | Off | Comp 30% |
| 39 | 014 Breath Choir | choir | Chorus 24% | Delay 16% | Width 24% |
| 40 | 015 Poly Pad | pad | Chorus 28% | Delay 18% | Width 26% |
| 41 | 016 Sweep Motion | motion | Chorus 22% | Delay 36% | Width 32% |
| 42 | 017 Skew Texture | texture | Crush 45% | Drive 24% | Tilt 40% |
| 43 | 018 Pulse Drone | drone | Drive 20% | Delay 16% | Tilt 24% |
| 44 | 019 Wide Arp | arp | Delay 18% | Off | Width 24% |
| 45 | 020 Heavy Crush | fx | Crush 80% | Drive 50% | Limit 70% |
| 46 | 021 Punch Kit | kit | Drive 36% | Crush 20% | Limit 60% |

### Family 03

| # | Preset | Role | Insert 1 | Insert 2 | Master |
|---:|---|---|---|---|---|
| 47 | 001 Piano Stack | keys | Chorus 12% | Delay 12% | Tilt 16% |
| 48 | 002 Soft EP | electric piano | Chorus 18% | Off | Tilt 18% |
| 49 | 003 Soul Piano | electric piano | Chorus 26% | Delay 14% | Tilt 22% |
| 50 | 004 Clav Stab | clav | Drive 24% | Off | Comp 26% |
| 51 | 005 Dry Clav | clav | Drive 18% | Off | Comp 20% |
| 52 | 006 Warm Organ | organ | Chorus 20% | Off | Comp 22% |
| 53 | 007 Bottle Bells | bells | Delay 14% | Chorus 14% | Width 22% |
| 54 | 008 Muted Pluck | pluck | Delay 10% | Off | Comp 18% |
| 55 | 009 Reed Stab | stab | Chorus 18% | Delay 12% | Comp 26% |
| 56 | 010 Finger Bass | bass | Drive 20% | Off | Comp 24% |
| 57 | 011 Solo Lead | lead | Delay 16% | Chorus 12% | Comp 22% |
| 58 | 012 Ensemble Brass | brass | Chorus 20% | Off | Comp 30% |
| 59 | 013 Section Strings | strings | Chorus 24% | Delay 16% | Width 24% |
| 60 | 014 Ooh Choir | choir | Chorus 22% | Delay 16% | Width 22% |
| 61 | 015 Dream Pad | pad | Chorus 28% | Delay 16% | Width 26% |
| 62 | 016 Slow Sweep | motion | Chorus 22% | Delay 20% | Tilt 22% |
| 63 | 017 Hollow Texture | texture | Crush 12% | Delay 16% | Tilt 18% |
| 64 | 018 Warm Drone | drone | Chorus 18% | Delay 14% | Tilt 20% |
| 65 | 019 Mallet Arp | arp | Delay 10% | Off | Comp 18% |
| 66 | 020 Slap Delay | fx | Delay 48% | Crush 26% | Width 30% |
| 67 | 021 Dry Perc | kit | Drive 22% | Crush 16% | Limit 46% |

### Family 04

| # | Preset | Role | Insert 1 | Insert 2 | Master |
|---:|---|---|---|---|---|
| 68 | 001 Vector Keys | keys | Chorus 26% | Delay 14% | Width 24% |
| 69 | 002 Phase Piano | electric piano | Chorus 34% | Delay 12% | Width 22% |
| 70 | 003 Wave Clav | clav | Chorus 20% | Drive 18% | Comp 24% |
| 71 | 004 Drift Organ | organ | Chorus 32% | Delay 12% | Width 24% |
| 72 | 005 Chime Bells | bells | Delay 22% | Chorus 20% | Width 26% |
| 73 | 006 Harp Pluck | pluck | Chorus 18% | Delay 14% | Width 22% |
| 74 | 007 Soft Stab | stab | Chorus 16% | Delay 12% | Comp 24% |
| 75 | 008 Sub Bass | bass | Drive 16% | Off | Comp 26% |
| 76 | 009 Air Lead | lead | Delay 20% | Chorus 16% | Width 22% |
| 77 | 010 Swell Brass | brass | Chorus 24% | Delay 14% | Width 24% |
| 78 | 011 Bowed Strings | strings | Chorus 30% | Delay 16% | Width 24% |
| 79 | 012 Air Choir | choir | Chorus 28% | Delay 18% | Width 26% |
| 80 | 013 Cloud Pad | pad | Chorus 36% | Delay 22% | Width 30% |
| 81 | 014 Sparse Pad | pad | Chorus 12% | Delay 10% | Width 16% |
| 82 | 015 Motion Pad | motion | Chorus 35% | Delay 24% | Width 55% |
| 83 | 016 Tilt Texture | texture | Crush 16% | Delay 18% | Tilt 48% |
| 84 | 017 Slow Drone | drone | Chorus 20% | Delay 16% | Tilt 22% |
| 85 | 018 Seq Arp | arp | Delay 12% | Chorus 12% | Width 20% |
| 86 | 019 Wash Delay | fx | Delay 62% | Chorus 28% | Width 36% |
| 87 | 020 Metal Kit | kit | Crush 18% | Drive 22% | Limit 48% |

### Family 05

| # | Preset | Role | Insert 1 | Insert 2 | Master |
|---:|---|---|---|---|---|
| 88 | 001 Stage Keys | keys | Chorus 14% | Off | Comp 28% |
| 89 | 002 Reed Piano | electric piano | Chorus 10% | Off | Tilt 18% |
| 90 | 003 Funk Clav | clav | Drive 22% | Off | Comp 26% |
| 91 | 004 Rock Organ | organ | Drive 20% | Off | Comp 26% |
| 92 | 005 Ping Bells | bells | Delay 8% | Off | Width 20% |
| 93 | 006 Mute Pluck | pluck | Drive 10% | Off | Comp 18% |
| 94 | 007 Short Stab | stab | Drive 16% | Off | Comp 26% |
| 95 | 008 Round Bass | bass | Drive 18% | Off | Comp 26% |
| 96 | 009 Saw Lead | lead | Drive 24% | Off | Comp 24% |
| 97 | 010 Section Brass | brass | Chorus 12% | Off | Comp 24% |
| 98 | 011 Small Strings | strings | Chorus 14% | Off | Width 20% |
| 99 | 012 Close Choir | choir | Chorus 10% | Off | Width 18% |
| 100 | 013 Tape Pad | pad | Delay 14% | Off | Width 22% |
| 101 | 014 Filter Motion | motion | Chorus 18% | Off | Width 24% |
| 102 | 015 Broken Texture | texture | Crush 22% | Off | Tilt 26% |
| 103 | 016 Dark Drone | drone | Chorus 16% | Off | Tilt 20% |
| 104 | 017 Tight Arp | arp | Delay 6% | Off | Comp 16% |
| 105 | 018 Room Crush | fx | Crush 55% | Off | Limit 50% |
| 106 | 019 Dry Kit | kit | Drive 20% | Off | Limit 42% |
| 107 | 020 Hard Kit | kit | Drive 42% | Off | Comp 70% |
| 108 | 021 Tape Kit | kit | Drive 30% | Off | Limit 45% |

<!-- END GENERATED PRESET TABLE -->

### How the chains were chosen

The chains are our own design, informed by published algorithm names and routing for the five
reference machines — not copied from them. No document states which effects any factory program on
those machines used, and nothing was ingested. `banks/SOURCES.md` rows 107–111 record the
provenance per family, and `banks/FAMILY-0*-PROGRAMS.md` holds the documented catalogues the
reading is based on.

What that reading produced, family by family:

| Family | Documented structure | How Keith reads it |
|---|---|---|
| 01 | long algorithm catalogue, chained inserts into master send effects | two inserts wherever the role wants space; 12 of 20 |
| 02 | same catalogue plus a valve stage | drive-forward Insert 1 (11 of 21), compressor or limiter master |
| 03 | 33 types, the last eight of them dual `DELAY/x` pairs | modulation in Insert 1, delay in Insert 2 (9 of 21); tilt master for the catalogue's EQ and exciter types |
| 04 | two-slot processor, serial or parallel | two serial inserts on 19 of 20; width master on 14 of 20 |
| 05 | **one** insert effect plus two master sends | Insert 2 `Off` on all 21; the master stage carries the rest |

Three checks enforce all of this rather than trusting the prose.
`tools/verify_patch_fx_chains.py` checks the shape of the table — 14 checks, including that every
preset engages Insert 1 and a master stage, that no stage is half-written, that all four insert and
all four master algorithms get used, that no preset repeats its neighbour's chain, and that Family
05 keeps its single insert. `tools/verify_preset_metadata.py` checks that no preset resolves to the
init defaults or to the same state as another preset. The `preset-restore` test renders each preset
twice, once as it ships and once with all three FX bypass switches thrown, and requires the two to
differ by at least 1 % of signal RMS; the weakest chain in the library clears that by 2.6× and the
engines' own render-to-render noise variation stays under 0.26 %.

## State, presets and automation

All 111 parameters are host-automatable. The automation name of each one is in the `automatable`
column of [`PARAM-MANIFEST.md`](PARAM-MANIFEST.md); they are stable and should not be renamed.

`Keith::SerializeState` writes a magic word and version, the kit level, all 16 drum slot records,
and then all 111 parameter values. `Keith::UnserializeState` reads them back. If the magic or
version does not match it falls back to the pre-drum chunk layout — the first 75 doubles, with the
drum defaults supplied — rather than rejecting an older saved state outright.

The `preset-restore` test proves the round trip rather than asserting it: it drives every one of
the 111 parameters off its default, gives each of the 16 drum slots its own values, saves, loads
into an instance that has never seen that state, and requires all 111 parameters and all 128 drum
slot values to come back bit-identical. It then round-trips all 103 factory presets into a second
instance that is holding a different preset at the time, so the restore has to overwrite real state
rather than agree with whatever was already there.

Preset save, bank selection and favourites are not parameters and not implemented. The host's own
preset and session save is the save path.

## What this manual does not claim

- **No macOS binary has been built from this tree.** The AU, the macOS VST3 and the macOS
  standalone have not been run, and `auval` has not been run.
- Ableton Live and FL Studio have not loaded this build. The only host-side validation run against
  it is pluginval at strictness level 10; everything else is this repo's own test suite and
  checkers.
- The plugin is unsigned and not notarised.
- Multi-part synth routing is not built, so `synth_part` and the four part tape stages have no
  control.
- Preset save, bank and favourite actions are not parameters.
- There is no sequencer, and swing is not a parameter.
- The documented factory program lists from the reference machines are not in the plugin, and no
  sampled factory ROM content is either.

## Where these claims come from

| This manual's section | Source of truth |
|---|---|
| Signal path | `src/Keith_DSP.h` `ProcessBlock`, `Keith::SyncTapeParams` |
| Page and control labels | `src/editor/EditorialLayout.h`, `src/editor/EditorialControls.h` (`FxKnobLabel`) |
| Parameter ranges, defaults, automation names | `src/Keith.cpp` parameter init, [`PARAM-MANIFEST.md`](PARAM-MANIFEST.md) |
| Morph readout shape names | `src/editor/KeithControls.h` `KeithShapeReadoutControl` |
| Insert and master algorithm behaviour | `src/fx/PatchEffects.h` |
| Tape controls | `src/fx/TapeAnalog.h`, `Keith::SyncTapeParams` |
| Voice, filter, LFO and matrix behaviour | `src/synth/CoreSynth.h` |
| Drum engine and drum map | `src/drums/DrumSynth.h`, [`DRUM-MAP.md`](DRUM-MAP.md) |
| Preset list and FX chains | `kPresetSpecs` and `kPresetDeltas` in `src/Keith.cpp` |
| FX chain provenance | `banks/SOURCES.md` rows 107–111, `banks/FAMILY-0*-PROGRAMS.md` |
| State round-trip | `Keith::SerializeState` / `UnserializeState`, `tools/preset_restore_test.cpp` |
| Enforced properties | `tools/verify_patch_fx_chains.py`, `tools/verify_preset_metadata.py`, `tools/editor_surface_check.py` |

The preset table in this file is generated from `kPresetSpecs` and `kPresetDeltas` by
`tools/gen_manual_preset_table.py`. Run that script with `--check` to prove the table still matches
the shipped data, and without it to regenerate the table after changing a chain.
