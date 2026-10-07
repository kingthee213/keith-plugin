# U11 verification record — synthesized drum engine

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Worker lane: `u11-drums` (Cursor CLI, GPT-5.6 Sol Extra High), pane `w17:p2`.

**Every result below was re-run by the coordinator, not taken from the worker's report.**
No merge, install, push or deployment.

## Why this is synthesized, not sampled

`BRIEF.md` §2 asks for multi-slot sample playback, but **no audio files exist in this repo and none
may be ingested**: `banks/SOURCES.md` rule 1 forbids Korg ROM content, rule 2 forbids Splice samples,
rule 5 makes an open-source code licence irrelevant to its sample content, and the owner has no
hardware to record. A sample engine would have shipped with zero samples and a UI that lied.

So the drum section is a **synthesized percussion engine** built from oscillators, deterministic
noise, envelopes, filtering and drive. The UI says `SYNTHESIZED DRUM ENGINE` and the previous
`MAP ONLY` hatched preview is gone.

## What changed

Keith goes from 75 to **85 live parameters**. IDs 0–74 are unchanged; 10 are appended:

`kParamDrumSlotSelect/Level/Pan/Tune/Decay/Filter/Drive/Mute/Solo` (75–83), `kParamDrumKitLevel` (84).

New `src/drums/DrumSynth.h` (~13 KB): 16 voices, one per documented slot.

- **MIDI split:** notes 36–51 trigger slots 01–16 and are consumed — they never reach the synth
  voice allocator, so a drum hit cannot steal or retrigger a synth voice. Notes 0–35 and 52–127 go to
  the synth only. Documented in `docs/DRUM-MAP.md`.
- Per slot: level, pan, tune, decay, filter cutoff, drive, mute, solo. Stereo pan on the drum bus.
- Velocity affects level; a deterministic round-robin variation per trigger stops machine-gunning.
- Mute/solo with standard solo semantics; a muted or −inf slot is exact silence.

The drum preview on the Library page is replaced by a **live pad grid** bound to `kParamDrumSlotSelect`,
with the real slot names from the map.

`drum_kit_swing` is deliberately **not implemented** and is marked `not applicable` — swing requires
a step sequencer, and BRIEF §2 locks this instrument as MIDI-in/sound-out with no sequencer.

**Coordinator note:** the brief's prose said "11 parameters" while its own table listed 10 rows, because
`drum_kit_swing` was removed mid-draft. The worker implemented the 10 tabled rows and flagged the
discrepancy instead of inventing an 11th. That is the correct behaviour.

## Coordinator-verified results

| Gate | Result |
|---|---|
| Parameter IDs 0–74 vs previous commit | **byte-identical prefix**, 10 appended |
| Release build (4 targets) | clean, no errors/warnings |
| CTest | 2/2 passed |
| Offline synth tests | **104 passed, 0 failed** (28 of them drum assertions) |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS, emulated-family order intact |
| Material layout check | PASS, 27 placements agree |
| pluginval strictness 10 | exit 0, **SUCCESS**, Editor + Editor Automation completed |
| Protected files | `src/editor/ControlLayout.h` and `resources/images` **untouched** |

### Live interaction

- Library page shows `SYNTHESIZED DRUM ENGINE` with a live 4×4 grid and all 16 real slot names
  (Perc 1–4, Tom Mid, Tom Hi, Ride, Crash, Closed Hat, Open Hat, Pedal Hat, Tom Lo, Kick, Snare,
  Snare Alt, Clap). Capture: `computer_use_5b548494ffd04075bbdd1d7da5cf659d.png`
- Clicking the top-left pad moved the selection highlight from **Kick** to **Perc 1**, proving the grid
  writes `kParamDrumSlotSelect` rather than only drawing state. Capture:
  `computer_use_2f4c66e8b4b64c5aa5f8be39e872326b.png`
- Slot knobs (level, pan, tune, decay, filter, drive), mute/solo and kit level render on the page.

## Not host-tested

- The drum engine has never been auditioned in a DAW or played from a physical pad controller.
  The offline assertions prove each voice triggers, is finite and non-zero, decays to silence, and
  responds to velocity/decay/tune/filter/drive/pan/mute/solo — they do not judge whether it sounds good.
- The engine is synthesized, so it will not match the reference machines' percussion timbres. That is a
  content decision forced by the no-samples constraint, not a defect.

## Still open from the wider brief

Factory content library and the modulation matrix. Separate stages.
