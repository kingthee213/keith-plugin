# Sampler and Wavetable pages: build spec

Sources: Ableton Reference Manual v12, §30.11 (Simpler) and §31.13 (Wavetable), plus the
supporting pages listed in `docs/reference/`. Labels quoted below are Ableton's; **our labels must
differ** (naming rule: no trademarked gear names, no near-misses), and the "our label" column gives
the substitute.

## 0. The governing finding

**Simpler separates the waveform from the controls, and the controls sit in two tabs: «Sample» and
«Controls».** The Sample tab is the waveform plus a single row of horizontal controls below it. The
Controls tab holds Filter, Envelopes, LFO, Global.

**Our pages should follow that split**, because it solves a problem we already have: the sampler
page currently mixes the waveform, the browser and six knob labels into one 286-unit band, and the
knobs never got drawn because there was no room and no decision.

**Wavetable's display is a stack of frames, linear, bottom-to-top, with the position control
draggable both as a separate slider and directly on the display.** Ours draws top-to-bottom and is
not draggable. Both need changing.

## 1. Sampler page

### 1.1 Waveform display — the dominant element

Ableton draws the waveform full width at the top with:

- **Two draggable flags**: left = where playback may start, right = where it may end.
- **Start** and **Length** shown as **percentages of the flag-defined region**.
- **Slice markers as vertical lines**: **automatic slices blue, manual slices white.**
- **Zoom**: vertical drag or wheel with Ctrl/Cmd. **Pan**: horizontal drag.
- Colour beyond the slice markers is not documented — do not invent a legend.

Our implementation:

| Our label | Behaviour |
|---|---|
| `START` / `LEN` | percentages of the flag region, drawn under the waveform |
| slice markers | vertical lines; **auto = one colour, manual = another** — the two-colour distinction is the point, it tells you what detection did versus what you moved |
| zoom / pan | drag to pan; wheel or drag with a modifier to zoom |
| region flags | draggable left and right edges of the playable region |

**Replace the existing `KeithWaveView` envelope-only draw** with: waveform + region flags + slice
markers + auto/manual colouring. The envelope code stays; the interaction is new.

### 1.2 The mode switch

Three modes, and the manual names them **Classic**, **One-Shot**, **Slice**.

| Ours | Ableton | What it does |
|---|---|---|
| `PLAY` | Classic | whole sample, transposed across the keyboard; loops |
| `ONE` | One-Shot | full sample plays regardless of note length; no loop |
| `SLICE` | Slice | one slice per key, at recorded pitch |

**We already have `SampleMode::Play` and `SampleMode::Slice`.** One-Shot is **not implemented in the
engine** — it is a third behaviour, not a rename. Either implement it or ship two modes; **do not
label a two-mode control with three names.**

### 1.3 Slicing — the `Slice By` chooser

Ableton offers four methods, and this is the most useful thing in the whole reference:

| Ableton | Ours | Engine support |
|---|---|---|
| **Transient** + **Sensitivity** | onset detection + sensitivity | **have it** — `DetectSlices()` with `sensitivity` |
| **Beat** + **Division** | slice at musical divisions | `TempoSyncFromSlices()` |
| **Region** + **Regions** (2–64) | N evenly spaced slices | trivial — build the vector |
| **Manual** | double-click to add | `SetSlices()` |

Slice affordances, all manual-cited:

- **Double-click between slices creates one.**
- **Double-click a slice deletes it.**
- **Drag a slice moves it.**
- **Alt/Option-click toggles a slice between automatic and manual.** Manual slices **survive a
  sensitivity change** — that is why the distinction matters.
- Max **64** slices. Ours: `kMaxSliceKeys` is 60 and slices start at note 36, which matches.

**Sensitivity is already a parameter** (`SliceSettings::sensitivity`, 0..1). Wire it to a knob.

**There is NO per-slice parameter panel.** The agent checked specifically, because it would be easy
to assume one: slice behaviour is global, governed by the Playback chooser. **Do not build a
per-slice grid.**

### 1.4 Controls below the waveform

Classic row: `Gain`, `Start`, `Length`, `Loop` slider, `Fade`, `Loop` switch, `Snap`, `Voices`,
`Retrig`.
One-Shot/Slice row: `Gain`, `Trigger`/`Gate`, `Snap`, `Fade In`, `Fade Out`, `Transp`, `Vol<Vel`,
`Volume`.
Slicing adds: `Slice By`, `Sensitivity`/`Division`/`Regions`, `Playback` (Mono/Poly/Thru).

We have six knob slots in the geometry (`page_knob_start/end/length/root/tune/gain`). Map:

| Ours | Ableton | Notes |
|---|---|---|
| `START` | Start | % of region |
| `END` | Length | ours is END; Ableton uses Length — keep END, same idea |
| `LENGTH` | (Loop) | |
| `ROOT` | (root key) | Ableton does not expose root on the Sample tab; ours is useful, keep |
| `TUNE` | Transp | |
| `GAIN` | Gain | |

**`Snap`** (snap markers to zero crossings to avoid clicks) is worth having and is cheap. It is in
both Simpler and the manual.

### 1.5 The browser

**Simpler has no internal browser.** Samples load by dragging from Live's Browser or via
**Hot-Swap** (bottom-right of the waveform display, hotkey Q).

**So our `SAMPLES` list is a departure, and a good one** — we have no host browser to lean on. Keep
it, and add a **Hot-Swap-style button** on the waveform that focuses the list. Name it something
ours: `SWAP`.

## 2. Wavetable page

### 2.1 The display — this is what is wrong today

The manual (§31.13.2), quoted:

> "two types of wavetable visualizations available... The **linear view** arranges the waveforms
> **from bottom to top**, with time running from left to right. The polar view displays the
> waveforms as loops from inside to outside, with time running clockwise."

**Our frame view draws top-to-bottom. Ableton's linear view is bottom-to-top.** Flip it — the
reference direction puts frame 0 at the bottom and the table opening upward, which reads as
"building up".

**Two view modes exists in the reference: linear and polar.** Polar (nested loops, radius = frame
index, angle = phase) is genuinely better for a table whose frames differ mainly in harmonic
content, which is exactly ours. **Implement both, with a switch**, since the manual describes the
switch as a first-class control.

### 2.2 The display is INTERACTIVE

> "Clicking and dragging within the visualization will move to a different position within the
> wavetable."

**Ours is not draggable.** A drag on the frame view must set the morph. The frame view and the
morph knob are two views of one parameter — so dragging vertical (linear) or radial (polar) sets
`kParamOsc3Morph`.

### 2.3 Table selection

Two choosers (Category, then Table) plus **left/right arrow buttons** that roll over into the next
category, "so you can continuously move through the wavetables using the arrows alone."

**We have ONE bank of 8 frames.** A two-chooser browser would be a lie. **But: we can implement the
same control shape honestly by making the CHOOSER useful** — the categories can be *frame groups*
within our bank, or, better, the chooser selects the **oscillator source** which already exists
(`kParamOsc3Source`: shapes vs table) and the arrow buttons step through frames.

**Do not fabricate a table list.** Instead: show the real frame count, name the one bank, and let
the arrows step the morph frame by frame. Say so in the label — `TABLE 8 FRAMES`.

### 2.4 The morph is called `Wave Position` in the manual

Ours is `kParamOsc3Morph`, labelled `MORPH`. Keep `MORPH` — it is accurate and it is ours.

### 2.5 What not to build

The reference has two oscillators, two filters, a sub, FM/Classic/Modern oscillator effects, a mod
matrix, three envelopes, two LFOs and unison modes. **Almost none of that is in our engine.**

**Build only what the engine can honour:** the frame view (linear + polar), the morph, the
draggable position, the frame stepping, and the source switch. Everything else would be a control
that does nothing — which is precisely the complaint that started this.

## 3. Acceptance criteria

- Sampler: waveform with region flags, slice markers coloured by auto/manual, zoom and pan,
  draggable markers, double-click to add/delete, sensitivity knob, `Slice By` chooser with at least
  Transient and Region, mode buttons that change behaviour.
- Wavetable: frames drawn **bottom-to-top**, linear **and** polar, **draggable to set morph**,
  arrows to step frames.
- Every control reachable; no two page controls overlap; `audit_editor_input.py` passes.
- Nothing labelled that is not implemented.

## 4. Explicitly out of scope, with reasons

| Not building | Why |
|---|---|
| One-Shot mode (if engine work is too large) | needs a third engine behaviour, not a label |
| Warp modes (`Beats`/`Tones`/`Texture`/`Complex`) | the engine has one stretcher |
| Per-slice parameter panel | **does not exist in the reference** |
| Wavetable filters, envelopes, LFOs, unison | not in our engine |
| A multi-bank table browser | we have one bank; a list of one is a lie |
