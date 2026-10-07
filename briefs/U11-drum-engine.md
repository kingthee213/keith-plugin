# U11 — drum engine (synthesized percussion)

Coordinator contract. Binding. Read `BRIEF.md` §2, `docs/DRUM-MAP.md`, `docs/PARAM-MANIFEST.md`,
`docs/ENGINE-SCOPE.md`, `src/Keith.h`, `src/Keith_DSP.h`, `src/synth/CoreSynth.h`,
`src/editor/KeithTheme.h`, `src/editor/EditorialLayout.h`, `src/editor/EditorGeometry.h`,
`tools/synth_offline_test.cpp`, `tools/editor_surface_check.py`.

## Mission

BRIEF §2 requires a working drum section: "multi-slot sample playback driven by MIDI, slots mapped
across the keyboard, with a documented drum map so pads and keys trigger them."

Today the drum page is a hatched **`MAP ONLY`** preview with no engine behind it and no parameters.
Make it real.

## The content constraint — read this before designing anything

**There are no audio files in this repo, and none may be brought in.** `banks/SOURCES.md` rule 1
forbids Korg ROM content, rule 5 forbids treating an open-source code licence as permission to reuse
its sample content, and the owner has no hardware to record. A sample-playback engine would
therefore ship with zero samples and be a lie in the UI.

So implement the drum section as **synthesized percussion**, built from the primitives this engine
already has (oscillators, noise, envelopes, filter, drive). Say so plainly in the UI and docs: this
is a synthesized drum engine, not sample playback. Do not draw a sample browser, a "load sample"
control, or any sample-name UI. The honest label is the whole point.

## Parameters to add (append only, IDs 72 onward)

Start after the highest existing ID at the time you begin — **read `src/Keith.h` and use what is
actually there**; U10 (FX) may have landed before you, in which case the append starts higher. Never
renumber any existing ID.

11 appends:

| Enum | Label | Range |
|---|---|---|
| `kParamDrumSlotSelect` | Slot | 1…16 |
| `kParamDrumSlotLevel` | Slot Level | −inf…+6 dB |
| `kParamDrumSlotPan` | Slot Pan | L100…R100 |
| `kParamDrumSlotTune` | Slot Tune | −12…+12 st |
| `kParamDrumSlotDecay` | Slot Decay | 0…100 % |
| `kParamDrumSlotFilter` | Slot Filter | 20 Hz…20 kHz |
| `kParamDrumSlotDrive` | Slot Drive | 0…100 % |
| `kParamDrumSlotMute` | Slot Mute | off/on |
| `kParamDrumSlotSolo` | Slot Solo | off/on |
| `kParamDrumKitLevel` | Kit Level | −inf…+6 dB |

Host names: `Drum Slot Select`, `Drum Slot Level`, … `Drum Kit Level`.

**`drum_kit_swing` is deliberately NOT implemented.** Swing only means something with a step
sequencer, and BRIEF §2 locks the instrument as "MIDI in, sound out — no internal step sequencer,
no pattern player". Mark that manifest row as `not applicable — instrument has no sequencer` rather
than leaving it looking pending. Explain this in `docs/DRUM-MAP.md`.

**Do not add 16 trigger parameters.** Pads are triggered by MIDI, which is what the brief actually
describes; a trigger is not an automatable value.

## Engine

- 16 percussion voices, one per slot, each a distinct synthesized drum voice matching the documented
  map in `docs/DRUM-MAP.md` (kick, snare, hats, toms, clap, rim, cymbal, perc — follow the existing
  document's own slot order and names; do not invent a different map).
- Voices are triggered by MIDI notes per the documented map, and must coexist with the synth voice
  on the same MIDI input without stealing a synth voice or a synth note. Define the split rule
  explicitly (which note range drives drums, and that the note also does or does not reach the
  synth) and document it.
- Per-voice: pitch/tune, decay, filter cutoff, drive, level, pan. Pan is per-slot and needs a stereo
  output path for the drum bus — if the main output is mono, convert only as far as is honest and
  say what you did.
- Mute/solo across 16 slots with standard solo semantics (any solo mutes all non-soloed slots).
- Velocity must affect level, and BRIEF §2 asks for round-robin variation so it does not
  machine-gun: synthesize a small deterministic variation per trigger (e.g. seeded jitter on pitch
  or decay). It must be deterministic enough to test.
- Bounded and finite: no NaN/Inf, no clicks on trigger, and a slot that is muted or at −inf level
  produces exact silence.
- No allocation in `ProcessBlock`; preallocate every voice's state and delay/noise buffers.

## Editor

- Replace the `MAP ONLY` hatched preview on the Library page (or wherever the pad grid currently
  lives — read the code) with a **live** pad grid: real slot names, real mute/solo state, the
  currently selected slot highlighted, and the slot parameters bound to the new parameters.
- Pads must be clickable to select a slot. If a pad click is expected to audition the voice, implement
  it truly or do not offer it — do not draw a click that does nothing.
- Remove the "drum sampler not built" style disclaimer text that is now false, and replace it with
  the accurate synthesized-engine wording.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
  Do not add controls to baked material pages.

## Presets and docs

- All 11 defaults must leave the existing 15 presets sounding exactly as they do now, and a test must
  prove it.
- Every slot's default level and pan must be musically sane so a fresh kit is playable without
  edits; if a default slot would be silent, say so and justify it.
- Update `docs/PARAM-MANIFEST.md`: flip every `drum_*` row to its true state (`live` or
  `not applicable`), add a U11 code surface table, update the counts.
- Update `docs/DRUM-MAP.md`: the real map, the MIDI split rule, the synthesized-voice description,
  the round-robin rule, and why swing is absent.
- Update `docs/ENGINE-SCOPE.md` with the drum engine section.

## Tests (all in `tools/synth_offline_test.cpp`)

- Per voice family: triggering produces finite, non-zero output, and the tail decays to silence.
- `drum_velocity_changes_level`, `drum_slot_decay_changes_length`, `drum_slot_tune_changes_pitch`,
  `drum_slot_filter_changes_brightness`, `drum_slot_drive_adds_harmonics`,
  `drum_slot_mute_is_silent`, `drum_solo_mutes_others`, `drum_pan_shifts_balance`.
- `drum_notes_do_not_steal_synth_voices` — the explicit split rule, proved.
- `drum_round_robin_varies_and_stays_deterministic`.
- `drum_all_slots_max_is_finite`.
- `drum_new_defaults_preserve_presets`.
- All existing tests keep passing.

## Verification — run unpiped, preserve the real output

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

`pluginval` must end in `SUCCESS` with Editor and Editor Automation included.

## Hard boundaries

- Append parameter IDs only. Never renumber an existing ID.
- No sample playback, no sample assets, no sample browser UI.
- Do not touch `src/editor/ControlLayout.h`, the material PNGs, or `D:/TYH_R10/dev/keith-plugin`.
- Do not implement factory ROM content or the modulation matrix here; separate stages.
- No commit, merge, push, install or deploy.
- Report separately: implemented and verified / implemented but not host-tested / deferred / blocked.
