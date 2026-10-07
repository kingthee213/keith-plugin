# U8 — Keith editor: full composer surface

**Lane:** `keith-editor-4` (Cursor/Composer). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first:** `BRIEF.md` §§2, 4, 5, 6.5, 7, 12; `docs/UI-DESIGN.md`; `docs/PARAM-MANIFEST.md`;
`design/editor-prototype.html`; `src/Keith.h`; `src/Keith.cpp`; `src/synth/CoreSynth.h`; and the
U7 report at `C:/Users/noaha/.agents/orchestrate/keith-plugin/U7-report-composer.md`.
Do not run git commands or commit. The coordinator owns integration.

## Purpose

The U7 core synth is now real and tested, but the shipped editor still exposes only Gain/tape and
looks like a generic dark test panel. Replace that preview with the first serious Keith instrument
surface. This is an editor implementation pass, not a decorative mock: every visible knob/switch
must bind to a real parameter or a real host preset action.

## Required outcome

### 1. Full-size themed shell

- Change the default editor to the brief's design size: 1280x800; minimum practical layout 1024x700.
- Use the actual UI design language from `docs/UI-DESIGN.md`: ink ground, thick near-black comic
  outlines, hard rectangular panels, flat saturated washes, cosmic green/magenta/gold/cyan accents,
  zero rounded glass cards and no soft shadows.
- Use the bundled original fonts already in `design/_fonttmp/`: copy/embed Bangers Regular and
  Space Mono Regular/Bold as resources. Do not use Roboto for the new shell.
- Make the UI resizable. Do not use tabs: preset strip, synth, FX and the initial drum panel are
  visible together at the design size. At smaller sizes, preserve readable geometry and keep the
  core synth + preset strip visible.

### 2. Preset picker at the top

- Add a prominent preset strip with KEITH wordmark, family chips labeled only `Family 01` through
  `Family 05`, bank/model grouping, and a working dropdown/pop-up picker.
- Use iPlug2's baked preset manager control or an equivalent real `RestorePreset` path. Add at least
  12 real baked presets with neutral names grouped by Family 01–05 (for example: `Family 01 /
  001 Glass Keys`, `Family 01 / 002 Dry Bass`, `Family 02 / 001 Neon Pluck`, `Family 03 / 001 Soft EP`,
  `Family 04 / 001 Motion Pad`, `Family 05 / 001 Hard Kit`). Names must not expose Korg/trademark names.
- Preset changes must change the actual parameter state, and Prev/Next must work. If the host preset
  API requires a different setup, use the simplest supported iPlug2 path and document it.
- Do not invent fake preset UI controls that have no action.

### 3. Synth / oscillator-first layout

The central panel must make the main oscillators unmistakable:

- `OSCILLATOR 1` and `OSCILLATOR 2` have separate framed blocks with shape, pitch, level and PW
  bound to the U7 IDs. Include visible shape text or a real enum control, not an unlabeled generic knob.
- A `SUB / NOISE / FM` block binds the real U7 IDs.
- A `FILTER` block binds type, cutoff, resonance, envelope amount, and filter ADSR.
- A `VOICE` block binds engine, poly/mono, portamento, unison voices and detune.
- A master/output block binds synth master level and the existing Gain.
- Do not leave the old tape-only eight-knob arrangement as the main UI.

### 4. Real envelope editors

- Add a clearly labelled graphical `AMP / VOLUME ENVELOPE` editor with Attack, Decay, Sustain,
  Release handles or an equivalent interactive graph. The four controls must bind exactly to
  `kParamAmpAttack`, `kParamAmpDecay`, `kParamAmpSustain`, `kParamAmpRelease` and changing them must
  redraw the curve.
- Add a clearly labelled graphical `PITCH ENVELOPE` editor with the four ADSR controls and Amount;
  bind to `kParamPitchAttack`, `kParamPitchDecay`, `kParamPitchSustain`, `kParamPitchRelease`,
  `kParamPitchAmount`. Curve direction must visibly reflect positive/negative amount.
- If custom controls are necessary, implement them in project source with no allocations in Draw or
  ProcessBlock. Use existing iPlug2 control patterns where possible. A graph that merely displays
  static art is not acceptable.

### 5. FX and drum surface

- Keep the existing global tape bus visible in a clearly labelled `TAPE BUS` panel and bind all eight
  existing tape/Gain IDs.
- Add a compact `DRUM KIT` panel and 4x4 pad grid as the next visible zone, styled as the brief
  specifies. It may be an honest preview/disabled panel only if no real drum params/engine exist yet;
  label it `DRUM KIT — NEXT ENGINE` rather than pretending pads trigger audio.
- Do not expose unimplemented insert FX, LFO, mod matrix, or drum knobs as working controls.

### 6. Verification

- Build `Keith-vst3` and `Keith-app` Release.
- Run `pluginval` strictness 10 with GUI tests enabled.
- Launch the standalone and capture a screenshot proving the full shell is actually visible, not just
  a successful compile. Inspect the screenshot for clipped text, invisible controls, and blank panels.
- Add/update a focused offline or source-level check if needed to prove the visible control IDs match
  real EParams and the preset count/names are non-empty.
- Write `U8-report-editor.md` under `C:/Users/noaha/.agents/orchestrate/keith-plugin/` with exact
  commands and output. Clearly separate measured results from NOT VERIFIED (host DAW load, macOS,
  ear match, full drum engine).

## Constraints

- Preserve U7 parameter IDs 0–42. Do not rename the existing host-visible params without documenting
  migration.
- Neutral family labels only in UI, preset names, plugin metadata, and screenshots.
- No GPL synth source, no samples, no external UI framework, no online auth.
- Keep Windows x64 VST3 + standalone buildable.
- Make the smallest coherent edit; no drive-by DSP changes in this lane.
