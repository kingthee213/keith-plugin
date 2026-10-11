# Sampler and Wavetable pages: completion spec

**Status:** spec for implementation. Written after reading the engine; the UI design section is
pending the Ableton Simpler / Wavetable reference research.

## 1. Why this spec exists

Both pages shipped as scaffolding. The owner opened them and reported, correctly:

> "the sampler and wave table both have a bunch of stuff missing. theres literally no buttons. no
> graph for the sample to be seen and edited. i dont see any of the knobs."

What was actually there: page titles, mode buttons, tab switches, and a set of `KeithPagePanel`
boxes that draw a filled rectangle and a border and **nothing else**. `page_wt_view`,
`page_wt_frames`, `page_sample_browser` and `page_sample_wave` were all empty panels.

Since then a waveform view, a sample browser and a wavetable frame view have been added. This spec
covers finishing the job properly against the reference products the owner named:
**Ableton Simpler** for the sampler, **Ableton Wavetable** for the wavetable.

## 2. What the engine already provides

The UI is the gap, not the audio. These exist and are callable today.

### SamplerEngine (`src/sampler/SamplerEngine.h`)

| Call | What it does |
|---|---|
| `LoadError Load(const std::string& path)` | loads a file; message thread only |
| `bool HasSample() const` | is anything loaded |
| `const std::shared_ptr<const SampleData>& Sample() const` | the audio, for drawing |
| `double EngineSampleRate() const` | the host rate, for a time axis |
| `SampleMode Mode() / SetMode(...)` | `Play` (whole sample transposed) or `Slice` (one slice per key) |
| `bool Reversed() / SetReversed(...)` | play backwards; a parameter, saved with the patch |
| `std::shared_ptr<const std::vector<Slice>> Slices() const` | the slice list, for drawing markers |
| `SliceResult DetectSlices(const SliceSettings&)` | transient detection |
| `void MoveSliceBoundary(std::size_t index, int frame)` | drag a boundary |
| `void SetSlices(std::vector<Slice>)` | set them wholesale |
| `void TempoSyncFromSlices(double beatsPerSlice)` | tempo align |
| `int SliceForNote(int note) const` | which slice a key plays |
| `WarpEngine& Warp()` / `WarpMap& Map()` | the stretcher and its marker map |
| `SetVocalEnabled(bool)` | the choir layer |

### SliceSettings — the detection controls, already parameterised

```cpp
float sensitivity = 0.5f;   // 0..1, higher finds more onsets
float minSpacingMs = 40.f;  // below ~20 ms one hit reports as several
float attackMs = 1.f;
float releaseMs = 20.f;
float localMs = 250.f;
float hopMs = 5.f;
```

### Slice — note the absent end

```cpp
struct Slice {
  int startFrame = 0;
  int midiNote = 36;
  int EndFrame(const std::vector<Slice>&, std::size_t index, int totalFrames) const;
};
```

**A slice stores no end.** It derives one from the next slice's start, so a boundary drag cannot
leave a start and an end disagreeing. Any UI that draws or drags slices MUST use `EndFrame()` and
must never introduce a stored end.

### Wavetable (`src/synth/WavetableOsc.h`, `CoreSynth.h`)

- `WavetableFrame` holds `float harmonic[256]` — an amplitude spectrum, not samples.
- `WavetableBank` holds a `std::vector<WavetableFrame>`; `FrameCount()`, `Frames()`.
- `CoreSynth::MakeStarterBank()` builds **one bank of 8 frames**: frame 0 is a sine, each later
  frame adds upper harmonics while pulling the odd ones back — a sine-to-square sweep.
- `kParamOsc3Morph` (0..100 %) walks across the frames.
- `kParamOsc3Source` selects built-in shapes vs the bank; `kParamOsc3Shape` picks a shape.
- Reachable read-only via `Keith::WavetableBankForEditor()`.

**There is exactly one bank.** So any "table browser" has a single row, and the honest UI is a
frame view, not a list.

## 3. Geometry available

The page rects are generated into `src/editor/EditorGeometry.h` and read with `OverlayIRECT(id)`.

### Sampler — `page_sampler` is `{141, 14, 1028, 300}`

| id | rect | intended |
|---|---|---|
| `page_sample_play` | `{151, 36, 1018, 52}` | full-width mode button strip |
| `page_sample_slice` | `{151, 58, 1018, 74}` | |
| `page_sample_browser` | `{151, 84, 331, 290}` | the sample list |
| `page_sample_wave` | `{341, 84, 1018, 196}` | the waveform |
| `page_sample_slices` | `{341, 204, 1018, 215}` | a slice strip |
| `page_knob_start` | `{341, 226, 447, 262}` | knob row |
| `page_knob_end` | `{455, 226, 561, 262}` | |
| `page_knob_length` | `{569, 226, 675, 262}` | |
| `page_knob_root` | `{683, 226, 789, 262}` | |
| `page_knob_tune` | `{797, 226, 903, 262}` | |
| `page_knob_gain` | `{911, 226, 1018, 262}` | |
| `page_sample_warp` | `{341, 280, 431, 292}` | caption |
| `page_sample_temposync` | `{439, 280, 543, 292}` | caption |
| `page_sample_reverse` | `{555, 280, 643, 292}` | FWD/REV tab |

### Wavetable — `page_wavetable` is `{141, 310, 1028, 562}`

| id | rect | intended |
|---|---|---|
| `page_wt_banks` | `{151, 332, 331, 552}` | table selector |
| `page_wt_frames` | `{341, 332, 850, 392}` | frame strip |
| `page_wt_view` | `{341, 392, 850, 552}` | the frame view |
| `page_wt_controls` | `{860, 332, 1018, 552}` | controls column |
| `page_wt_morph` | `{869, 353, 1009, 399}` | morph knob |
| `page_wt_src_label` | — | source tab switch |

## 4. Hard constraints — these have each caused a real defect

1. **Guard every page control's mouse handlers on the page being open.** One unguarded surface
   swallowed clicks across most of the plugin and made eight controls dead. `tools/audit_editor_input.py`
   enforces this; run it.
2. **Page controls must not collide with each other.** Check rects after any geometry change.
3. **`DrawText` takes `const char*`.** Pass `.c_str()`; a `std::string` does not bind.
4. **Qualify sampler types**: `sampler::Slice`, `sampler::SampleData`. `synth::WavetableBank`,
   `synth::WavetableFrame`. These are not in `keith::ui`.
5. **Include `KeithPage.h` before anything else** in an editor header — it provides `IControl`, and
   reversing that order produces a cascade of "no matching member function" errors.
6. **Never store a slice end.** Derive via `EndFrame()`.
7. **Build envelopes and frame shapes once per load**, never per `Draw()`.
8. **Run `tools/check_includes.py` and `tools/check_const_assign.py`** before pushing.
9. **An empty state must say so.** A blank box is indistinguishable from a broken one — which is
   precisely what the owner reported.

## 5. Acceptance criteria

The pages are done when, with the plugin open:

- Every control on both pages responds to a click or drag.
- The sampler draws a waveform for the loaded sample, with numbered slice markers that can be
  dragged, and the markers line up with what is heard.
- The sampler lists the shipped samples and clicking one loads it.
- Slicing can be triggered from the UI (detect), and the mode buttons change behaviour audibly.
- The wavetable shows all 8 frames and a morph marker that tracks the morph knob.
- No control on either page is unreachable, and no two page controls overlap.
- `audit_editor_input.py`, `check_includes.py`, `check_const_assign.py` and the layout gates pass.

## 6. Not in scope

- Simpler's Warp modes and complex envelope shaping, unless the engine already has them.
- Multiple wavetable banks — the engine has one, and inventing a browser for it would be a lie.
- Anything requiring audio to be heard. That is the owner's machine, not CI.
