# U9 — DeepSeek control-surface expansion

## Mission

Expand Keith from the current compact 43-parameter subtractive instrument toward a useful middle ground between the current UI and Surge's depth. Do not redesign the visual identity. The chassis/material pass is good enough for now and art is explicitly out of scope for this unit.

The target is a synth that has more meaningful sound-design control without becoming a copy of Surge or a wall of controls.

## Authority and current state

- Worktree: `D:/TYH_R10/dev/keith-editorial`
- Branch: `editorial-redesign`
- Framework: iPlug2
- Targets: Windows x64 VST3 and standalone
- Current live parameter IDs: 0–42, all surfaced by the editor
- Current engine: two morphing oscillators (sine → triangle → saw → pulse), sub, noise, simple FM, LP12/LP24/BP/HP filter, filter envelope, amp envelope, pitch envelope, poly/mono, portamento, unison, detune, global tape bus
- Current UI pages: Source, Envelopes, Tape, Library; permanent Filter/Voice channel
- Current preset families: neutral Family 01–05, internally ordered by emulated source family first: Triton, Triton Extreme, Korg M1, Korg Wavestation, Korg TR; then neutral sound role
- Current visual/art work is not the task. Do not add textures, character art, generated references, new fonts, or layout theatrics.
- Typography exception: solace wants an Adult Swim-style condensed grotesque title treatment. Do not copy or ship the Adult Swim logo, Helvetica Neue, or any proprietary brand asset. Replace the current Bangers title face only if a license-permitting substitute is sourced and recorded (for example an OFL-licensed condensed grotesque such as Oswald); keep Space Mono for values and technical labels. Verify the substitute is embedded and loaded before removing Bangers.

## Clean-room reference boundary

Surge is a study reference only. Use its public manual/GitHub documentation for feature vocabulary and UX grouping. Do not clone the Surge repository, copy GPL code, copy coefficient tables, copy wavetables, copy factory patches, or reproduce its UI.

Reference lessons to use:

- Group controls by sound-design task: oscillator, mixer, filter, amp envelope, filter envelope, modulation.
- Make the primary oscillator/filter controls obvious, then put deeper controls behind coherent tabs or expandable regions.
- Named filter types and meaningful waveform/algorithm choices are better than unlabeled generic knobs.
- Modulation should show source, destination and amount clearly; do not add a fake modulation matrix with no DSP.
- Preset browsing remains separate from editing.

## Required process

1. Read `BRIEF.md`, `docs/ENGINE-SCOPE.md`, `docs/PARAM-MANIFEST.md`, `docs/UI-DESIGN.md`, `src/Keith.h`, `src/synth/CoreSynth.h`, `src/editor/EditorialLayout.h`, and the current tests.
2. Inventory what is already real before proposing additions. No invented controls.
3. Write `U9-IDEAS.md` before editing. Propose three scopes:
   - Conservative: add only high-value controls that fit the existing engine.
   - Middle: recommended target, roughly 55–70 live parameters.
   - Deep: closer to a modern hybrid synth, clearly marked as a later phase.
4. Choose the middle scope unless a real engine constraint proves it unsafe.
5. Extend the DSP and parameters vertically. Every new visible control must have a real parameter, DSP effect, preset serialization, and a focused test.
6. Preserve IDs 0–42. Append new IDs only. Never renumber existing host automation.
7. Keep UI structure coherent. Prefer adding an `02 MODULATION` or `03 FILTER` sub-page only if the current page cannot fit the new real controls. Do not create miniature unreadable knobs.
8. Keep neutral family labels in the UI. Internal provenance metadata may remain in source/docs only.
9. Do not claim Surge parity. The deliverable is a deliberate middle ground.

## Recommended middle-scope candidates

These are proposals, not permission to add unbacked controls. Verify each against the engine design before implementation:

### Oscillators

- Per-oscillator sync toggle or mode, only if implemented in DSP.
- Per-oscillator ring/FM routing choice, only if the route is real and named accurately.
- Per-oscillator stereo/unison spread, if it can be implemented without misleading mono output.
- Explicit waveform mode/algorithm selector if the current shape morph needs a named mode distinction.

### Filter / voice

- Key tracking amount.
- Filter drive or input gain.
- A second filter only if routing and CPU behavior are actually implemented; otherwise do not draw one.
- Filter slope/type choices must stay honest to the DSP.

### Modulation

- One tempo-independent LFO with rate, shape, amount and one or two explicit destinations; or a small real modulation router with source → destination → amount.
- Mod wheel destination/amount only if MIDI CC1 is handled and tested.
- Do not add a 12-LFO or full matrix UI in this unit.

### Performance

- Velocity-to-level and/or velocity-to-filter amount if MIDI velocity is already available in the voice path.
- Legato/retrigger behavior only if mono/portamento semantics are implemented and tested.

### Tape

- Keep current tape controls. Do not expand tape before the synth control surface is complete.

## Acceptance criteria

### Parameter contract

- Existing IDs 0–42 unchanged.
- New IDs appended contiguously.
- Parameter labels describe real DSP behavior and include units where applicable.
- No duplicate or redundant visible labels.
- `editor_surface_check.py` passes with every live parameter exposed exactly once.
- `docs/PARAM-MANIFEST.md` matches the implementation and marks deferred rows honestly.

### DSP

- Offline tests cover every new control with a measurable behavior assertion.
- Note-on produces finite non-zero audio.
- Note-off reaches silence.
- Parameter sweeps remain finite and bounded.
- New modulation/performance controls demonstrably alter the intended destination and no unrelated destination.
- No stuck voices, crashes, NaN or Inf.

### Presets

- Existing 15 presets continue to load and sound.
- New controls have sensible defaults and are serialized in preset deltas.
- Family ordering remains emulated-family-first, then neutral category.
- Do not silently add fake factory content.

### UI

- Current chassis and material assets remain unchanged.
- Existing pages continue to work.
- New controls use readable groups, not pixel-dense filler.
- All controls are native/live and have numeric entry or named selection where appropriate.
- Add interaction tests for any new page/tab, dropdown, modulation routing or MIDI behavior.

### Verification

Run unpiped and preserve real output:

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

Pluginval must end in `SUCCESS`, with Editor and Editor Automation included. Do not treat a screenshot or a successful compile as proof of sound behavior.

## Deliverables from DeepSeek

1. `briefs/U9-IDEAS.md` with three scopes and the recommended middle scope.
2. A short parameter/DSP migration table: old ID, new ID, label, range/unit, DSP owner, test.
3. Implementation in the isolated worktree only.
4. Focused offline and editor interaction tests.
5. A report that separates:
   - implemented and verified,
   - implemented but not host-tested,
   - deferred,
   - blocked.

Stop before merge, install, push, or deployment. The coordinator retains acceptance authority.
