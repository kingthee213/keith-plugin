# U7 — composer — core synth vertical slice

**Lane:** `keith-composer-3` (Cursor/Composer). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first:** `BRIEF.md` §§2–7 and §10–12, `docs/UI-DESIGN.md`, `docs/PARAM-MANIFEST.md`,
`docs/ENGINE-SCOPE.md`, `src/Keith.h`, `src/Keith.cpp`, `src/Keith_DSP.h`, and the current
`design/editor-prototype.html` only for the intended surface. Do not use GPL synth source or
Freakshow code. The coordinator owns git; do not run git commands and do not commit.

## Purpose

The current build is still a sine test voice. Build the first real sound-design vertical slice so
the editor can stop being decorative: two oscillators + sub/noise, selectable neutral engine mode,
filter, poly/mono voice behavior, and real volume and pitch envelopes. Keep the current tape block
working. This is not the full drum/sample/FX/preset bank pass yet; make the core synth correct and
extensible first.

## Required outcome

1. **Stable parameter contract.** Replace the nine-parameter test surface with a coherent enum and
   `Init*` registrations for the synth surface. Preserve existing IDs/names where practical; if IDs
   must move, document the migration in the report and update `docs/PARAM-MANIFEST.md`. At minimum:
   - engine mode: `Hybrid` / `Analog`
   - poly/mono, portamento, unison voices/detune
   - Osc 1 and Osc 2: shape, semitone pitch, level, pulse width where applicable
   - sub level, noise level, oscillator FM amount
   - filter type (LP24/LP12/BP/HP), cutoff, resonance, envelope amount
   - amp envelope attack/decay/sustain/release
   - pitch envelope attack/decay/sustain/release/amount
   - synth master level and existing global tape controls

2. **DSP.** Implement clean-room behavior in our own code:
   - polyphonic MIDI note on/off with no stuck notes;
   - oscillator shapes at minimum sine, saw, square/pulse and triangle/wavetable-like morph;
   - two oscillators, sub oscillator, noise, level mix and simple FM;
   - stable resonant filter with the four UI modes;
   - amp envelope actually controls volume;
   - pitch envelope actually controls oscillator pitch;
   - no clicks on note-off or reset; bounded parameter values; no allocations in ProcessBlock;
   - keep the global TapeAnalog block after the synth bus.

3. **Tests / evidence.** Add a small offline C++ or Python test harness under `tools/` if useful,
   and run it. Prove at least: note-on creates non-zero audio, note-off decays to silence, amp
   attack/release change the measured envelope, pitch envelope changes zero-crossing/frequency,
   all filter modes remain finite, and a parameter sweep has no NaN/Inf. Paste real command output
   in `U7-report-composer.md` under `C:/Users/noaha/.agents/orchestrate/keith-plugin/`.

4. **Do not fake the UI.** The editor pass will bind these controls after this lane. Do not leave
   new visible controls with no real DSP parameter. Keep the plugin buildable with the current
   editor while you work.

## Constraints

- No external synth source, no copied GPL code, no samples.
- No drum engine in this pass; no factory content ingestion; no preset browser implementation.
- Keep the current Windows VST3 + standalone targets building.
- Report measured vs NOT verified. Mention any parameter IDs that remain intentionally deferred.

## Gate

- `cmake --build build --config Release --target Keith-vst3 Keith-app` succeeds.
- Existing pluginval strictness 10 still passes with GUI tests enabled.
- The offline signal tests pass with their counted assertions.
- Report contains exact commands and output, plus explicit NOT verified list.
