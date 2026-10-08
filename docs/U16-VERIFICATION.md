# U16 verification record — per-patch FX chains, manual, round-trip

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.
Worker lane: `u16-polish` (Cursor CLI, Claude Opus 5 Thinking High), pane `w17:p5`.

**Every result below was re-run by the coordinator.** No merge, install, push or deployment.

## Part A — preloaded FX chain per preset (BRIEF §6.4)

All **103 presets** now carry a deliberate insert/master chain chosen per family character and role,
rather than one chain stamped everywhere. Provenance recorded in `banks/SOURCES.md` as our own design
informed by the documented algorithm names and routing — nothing ingested, per rules 1/2/5.

New checker `tools/verify_patch_fx_chains.py` (14 passed, 0 failed) gates the chains.

Measured proof from the restore test:

```
PASS 103 preloaded FX chains alter the rendered audio
     (weakest Family 05 / 006 Mute Pluck at 2.6% of signal RMS, floor 1.0%, render drift 0.26%)
```

This is a real measurement, not an assertion: each chain is rendered and compared against bypassed.
The weakest case is disclosed rather than hidden behind the average.

## Part B — control and preset manual (BRIEF §12)

`docs/MANUAL.md` written (~28.5 KB), cross-linked from `README.md`. Covers: what the instrument is and
is not, the real signal path (voices → synth bus → Insert 1 → Insert 2 → tape bus → Master → out level
→ gain), every page and control under its real label, the modulation surface (LFO 1, LFO 2, mod wheel,
velocity, key tracking, 4-slot matrix), the full drum map (MIDI 36–51 → slots 01–16, synthesized voices,
velocity and round-robin), the 103-preset library and family ordering, and an accurate "not built"
section. The worker re-checked claims against `src/synth/CoreSynth.h` rather than writing from memory.

## Part C — save/load round-trip (BRIEF §12)

```
PASS 103 presets recall through RestorePreset into distinct states
PASS 103 factory presets round-trip all 101 parameters into a second instance
PASS probe state round-trips 101 parameters and 128 drum slot values exactly
     through SerializeState/UnserializeState (1236 byte chunk)
```

The round-trip uses the plugin's own `SerializeState`/`UnserializeState` path, not a hand-rolled copy,
and covers all 101 host parameters plus the 16 × 8 drum slot records. There is no sample map to test,
because the drum section is synthesized — the manual says so plainly.

## Coordinator-verified results

| Gate | Result |
|---|---|
| Parameter IDs vs previous commit | unchanged — **101 live**, prefix identical (no new params this pass) |
| Release build (4 targets) | clean, no errors/warnings |
| CTest | 3/3 passed |
| Offline synth tests | **126 passed, 0 failed** |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS — 20/21/21/20/21, palette 14 |
| **FX chain checker (new)** | **14 passed, 0 failed** |
| Material layout check | PASS — 27 placements agree |
| pluginval strictness 10 | exit 0, **SUCCESS**, Editor + Editor Automation completed |
| Frozen assets | `src/editor/ControlLayout.h` and `resources/images` **untouched** |

## Not verified

- **macOS, AU and auval: never built, never run.** `tools/build-macos.sh` says so in its own header.
  The owner is host-testing on Mac.
- **FL Studio and Ableton: never loaded this build.** pluginval is not a DAW.
- CC1 and mono legato remain offline-proven only.
- The drum engine is synthesized and will not match the reference machines' percussion timbres.

## Still open from the wider brief

Multi-part routing (`synth_part`, parts 1–4). This is deliberately not attempted: `docs/ENGINE-SCOPE.md`
§2.4 scopes it as a later pass because it would multiply the parameter surface roughly fourfold, and
BRIEF §4.3's per-part engine selection depends on it. The macOS side of BRIEF §12 is unverified by
definition on this machine.
