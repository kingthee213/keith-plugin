# U9 — control-surface expansion: scopes and recommendation

Owner: keith-deepseek (worker lane). Authority: `briefs/U9-deepseek-control-surface.md`.
Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`, read-only git.
Written **before** any source edit, per the required process.

## 0. Live state at the ideas checkpoint (reproduced, not relayed)

| Item | Command | Result |
|---|---|---|
| Branch / dirty tree | `git status --short --branch` | `editorial-redesign`; U7/U8/U4 work uncommitted in tree |
| iPlug2 | `build/CMakeCache.txt` | `IPLUG2_DIR=D:/TYH_R10/dev/keith-plugin/third_party/iPlug2` (present) |
| Artefacts | `build/out` | `Keith.vst3`, `Keith.exe`, fonts copied |
| Tests | `ctest --test-dir build -C Release` | **2/2 passed** (`synth-offline`, `editor-geometry`) |
| `tools/editor_surface_check.py` | run | **12 passed, 0 failed** — 43 params, ids 0..42 contiguous, all bound |
| `tools/verify_preset_metadata.py` | run | **PASS** — 15 presets, emulated family order preserved |
| `tools/verify_material_layout.py` | run | **PASS** — 27 native/render placements agree |
| pluginval | `D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe` | present |

Everything the acceptance list names is currently green; U9 must not regress any of it.

## 1. Inventory — what is already real (no invented controls)

Engine, `src/synth/CoreSynth.h` (617 lines):

| Block | Implementation | Honest vocabulary |
|---|---|---|
| Oscillator | PolyBLEP sine→tri→saw→pulse morph, `pw` for pulse width | continuous morph; UI already prints `SINE>TRI` etc. |
| Voice | 2 osc + sub (osc instance) + LCG noise + osc2 frequency-modulates osc1 | `FM` is real FM, not PM dressing |
| Unison | 1..8 detuned copies, per-osc detune offsets, **mono output** | detune is real; there is no stereo field today |
| Filter | TPT SVF, one 12 dB section; LP24 = cascaded second section; BP/HP from one section | LP12/LP24/BP/HP all real; Q 0.5..10 |
| Envelopes | amp ADSR, pitch ADSR + bipolar amount, filter ADSR + bipolar env amount (±4 oct at 100 %) | all three real |
| Voice mgmt | poly/mono through `MidiSynth`, portamento through `SetNoteGlideTime`, engine Hybrid/Analog (FM scale, shape bias, one-pole soften) | real |
| Master | `masterLevel` dB, global `gain` smoother | real |
| Tape bus | global `TapeAnalog` (sat/bump/wow/flutter/hiss/print/mix/bypass) + 4 per-part instances (currently bypassed) | real, seeded from MIT Airwindows ideas |

Editor, `src/editor/*`: chassis + workspace materials, `01 SOURCE`, `02 ENVELOPES`, `03 TAPE`,
`04 LIBRARY`, permanent `FILTER / VOICE` channel, keyboard, preset menu, preset archive.
`kParamGain` + the channel-strip knobs are baked into `chassis.png`; the source/tape/pitch
knobs are baked into their `workspace-*.png` crops.

Manifest: 101 rows, **43 `live`**, 58 `not yet implemented`. Three rows the manifest already
anticipates and that this unit is allowed to flip: `synth_lfo_rate`, `synth_lfo_amount`,
`synth_mod_wheel` (Mod Dest).

## 2. Constraints discovered before choosing a scope

These decide where new controls may live. They are facts from the tree, not preferences.

1. **`tools/verify_material_layout.py` hard-asserts 27 baked control placements** and that every
   rendered control's page, centre and radius match `src/editor/ControlLayout.h` exactly.
   Re-rendering is a Blender/art pass — out of scope for U9 and forbidden by the brief
   ("Current chassis and material assets remain unchanged").
   ⇒ **`ControlLayout.h` and the four PNGs are frozen. No new knob may be added to a baked
   material page (source/tape/pitch) or to the shared channel strip**, because a live knob over
   material with no baked body reads as a broken instrument.
2. **Toggle/tab/dropdown controls are NOT baked** — the tape `ACTIVE/BYPASS` tabs already draw
   natively over `workspace-tape.png`. So new *switches and tab selectors* may sit on existing
   material pages; new *knobs* may not.
3. New knobs therefore go on **material-free pages** (the amp/filter envelope tabs and the
   library page are already material-free and show the chassis work area). A new page costs no
   art. New knobs on those pages still need a visible body, so `KeithKnobControl` gains an
   opt-in native body (flat `kPanelAlt` fill + `kLine` frame, the existing hard-panel vocabulary).
   No art asset, no texture, no new type face.
4. **CC1 is already forwarded.** `VoiceAllocator` sends every unhandled controller to
   `SynthVoice::SetControl(controlNumber, value)` (`VoiceAllocator.cpp:176`); `MidiSynth` maps
   `kControlChange` default → `kControllerAction` (`MidiSynth.cpp:82`). So mod wheel is a real,
   reachable source and must be read that way, not faked.
5. **`isRetrigger` is never true.** `VoiceAllocator.cpp:413-417, 436, 515` hard-code
   `retrig = false` (with a `TODO retrig / legato`). Legato cannot be driven from the framework
   flag; it needs our own held-note count, in `KeithDSP` and in `OfflineEngine`.
6. **The offline engine is the test surface.** `OfflineEngine` mirrors the voice allocation so
   DSP behaviour is testable without a host; `KeithDSP` is the same maths behind `MidiSynth`.
7. **Defaults must be no-ops.** `AddFactoryPresets` builds every preset from the init defaults,
   so a new parameter whose default is inert leaves all 15 presets sounding identical. Every new
   control in this plan defaults to inert.

## 3. Scope options

### A. Conservative — ~49–52 live params

Flip the three manifest rows already written and add the two cheapest filter controls.

- `synth_lfo_rate`, `synth_lfo_amount`, `synth_mod_wheel` (LFO → cutoff only, amount, CC1 depth).
- filter key tracking, filter drive.
- velocity → filter.

**Rejects:** oscillator sync, ring, LFO shape/dest selection, legato. Small, safe, and not much
of a "middle ground": it leaves the LFO with a hard-wired destination and no shape choice, which
is exactly the unlabeled-generic-knob smell the reference lessons warn against.

### B. Middle — **56 live params (recommended)**

13 appends, every one with real DSP, presets, and a focused test.

New real DSP:

| Feature | DSP (owner) | Why it is honest |
|---|---|---|
| Hard sync | `Oscillator` wrap flag; on wrap, reset osc1 phase before it is processed (`Voice`) | osc2 becomes the master for osc1; classic sync timbre, audible | 
| Ring modulation | `mix += osc1 · osc2 · ring` (`Voice`) | a distinct algorithm from the existing FM path; both may run |
| Filter key tracking | `cutoff *= 2^(kt·(note−60)/12)` (`Voice`) | standard 100 % = 1 octave per octave |
| Filter drive | `mix = tanh(mix·(1+7·drive))` pre-filter, with a level-compensation factor (`Voice`) | changes harmonic content, not just level |
| LFO | per-voice phase-retriggered LFO: rate, shape, one destination, bipolar amount (`Lfo` in `CoreSynth.h`) | one real source → one real destination → real amount |
| Mod wheel | CC1 (already forwarded) → shared runtime value → same destination set, bipolar amount (`Voice` + `KeithDSP::Voice::SetControl`) | no fake matrix: the value reaches the DSP |
| Velocity → amp | extra velocity depth on top of the envelope's built-in velocity scaling | audible, measured |
| Velocity → filter | velocity offsets cutoff, ±2 octaves at 100 % | audible, measured |
| Mono legato | key-held mask (bit per key, cleared exactly on note-off and on all-notes-off/reset) decides re-attack vs glide-on-new-key | rule is deterministic and testable; defined as "another key is currently held", so no drift against CC123/sustain |

Destination set for LFO and mod wheel, shared and named: `OFF / CUTOFF / PITCH / SHAPE / LEVEL`
(shape = oscillator morph, level = tremolo). `OFF` makes the two amount knobs provably inert.
LFO shape set: `SINE / TRI / SAW / SQUARE / S&H` (S&H draws from the existing noise generator).

**Not in the middle scope:** any second filter, any stereo field for unison, any tempo sync.

### C. Deep — later phase (does not ship in U9)

- Second filter with real serial/parallel routing and per-route mix; filter FM.
- True stereo voice output: per-unison-voice panning → `Unison Width`. Deferred here because it
  changes the voice output shape (`Voice::Process` → L/R) in both `KeithDSP` and `OfflineEngine`,
  and its knob has no legal home without new material. Recorded, not built.
- Two-slot source→destination matrix (multiple sources per destination), mod envelopes,
  tempo-synced LFO, aftertouch as a source.
- Wavetable oscillator bank and the Hybrid/Analog split becoming two full engines.
- Drum engine parameters (16 slots × level/pan/tune/decay/filter/drive + kit level/swing) and the
  insert/master FX parameter families already listed in the manifest.
- Pitch-bend range parameter (`MidiSynth::SetPitchBendRange` exists; not requested yet).

## 4. Recommended scope: **B (middle, 56 live params)**

Rationale: it is the smallest set that satisfies the reference lessons — named algorithm choices
(sync, ring, LFO shapes, named destinations), grouped by sound-design task, with one real
source→destination→amount router instead of a fake matrix, and no unbacked control. It stays
inside every hard constraint in §2, and it lands inside the requested 55–70 band with 13 appends
rather than 27. Scope approved by the coordinator as tabled.

## 5. Migration table (old ID → new ID)

IDs 0–42 are untouched. `kParamOscSync` (43) starts the append; `kNumParams` becomes **56**.

| Old ID | New ID | Enum | Label | Range / unit | Default | DSP owner | Test |
|---|---|---|---|---|---|---|---|
| — | `kParamOscSync` | 43 | Osc Sync | `Free` / `Sync` | Free | `Oscillator::Wrapped`, `Voice` | `osc_sync_changes_timbre` |
| — | `kParamOscRing` | 44 | Osc Ring | `Off` / `On` | Off | `Voice` (osc1·osc2) | `osc_ring_changes_spectrum` |
| — | `kParamFilterKeyTrack` | 45 | Filter Key Track | 0…100 % | 0 % | `Voice` (cutoff by note) | `key_track_tracks_note` |
| — | `kParamFilterDrive` | 46 | Filter Drive | 0…100 % | 0 % | `Voice` (pre-filter tanh) | `filter_drive_adds_harmonics` |
| `synth_lfo_rate` | `kParamLfoRate` | 47 | LFO Rate | 0.01…30 Hz | 1 Hz | `Lfo::Process` | `lfo_rate_sets_period` |
| — | `kParamLfoShape` | 48 | LFO Shape | `Sine/Tri/Saw/Square/S&H` | Sine | `Lfo::Process` | `lfo_shape_changes_waveform` |
| — | `kParamLfoDest` | 49 | LFO Dest | `Off/Cutoff/Pitch/Shape/Level` | Off | `Voice` | `lfo_dest_targets_only_its_destination` |
| `synth_lfo_amount` | `kParamLfoAmount` | 50 | LFO Amount | −100…+100 % | 0 % | `Voice` | `lfo_amount_sets_depth` |
| `synth_mod_wheel` | `kParamModWheelDest` | 51 | Mod Dest | `Off/Cutoff/Pitch/Shape/Level` | Off | `Voice` (CC1) | `mod_wheel_dest_targets_only_its_destination` |
| — | `kParamModWheelAmount` | 52 | Mod Amount | −100…+100 % | 0 % | `Voice` (CC1) | `mod_wheel_amount_sets_depth` |
| — | `kParamVelToAmp` | 53 | Vel To Amp | 0…100 % | 0 % | `Voice` (extra depth) | `vel_to_amp_changes_level` |
| — | `kParamVelToFilter` | 54 | Vel To Filter | 0…100 % | 0 % | `Voice` (cutoff offset) | `vel_to_filter_changes_cutoff` |
| — | `kParamMonoLegato` | 55 | Mono Legato | `Retrig` / `Legato` | Retrig | key-held mask + `Voice::GlideTo` | `legato_skips_reattack` |

Existing automatable names are never renamed (host automation in the owner's Live sets must keep
resolving). New host names are unique and descriptive: `Synth Osc Sync`, `Synth Osc Ring`,
`Synth Filter Key Track`, `Synth Filter Drive`, `Synth LFO Rate`, `Synth LFO Shape`,
`Synth LFO Dest`, `Synth LFO Amount`, `Synth Mod Dest`, `Synth Mod Amount`,
`Synth Vel To Amp`, `Synth Vel To Filter`, `Synth Mono Legato`.

Zero-cost honesty note: no filter type is added. A notch would be reachable from the existing
SVF (`in − k·BP`) but the current type list is already honest and adding an option to a live
enum is a change to a host parameter's option count for no requested benefit.

## 6. UI plan (no art changes)

Page order becomes six buttons in the existing nav bar (it holds six at the design size):

1. `01 SOURCE` — unchanged, plus two native tab switches under `SOURCE MIX` in the free band
   between the two mixer rows: `OSC SYNC` (`FREE/SYNC`) and `RING` (`OFF/ON`). Tabs draw natively
   over the material, exactly like the tape `BYPASS` tabs; **no knob is added to this page.**
2. `02 MODULATION` (new, material-free) — three readable groups, headers only, no panels:
   - `LFO`: RATE, SHAPE, DEST, AMOUNT
   - `MOD WHEEL (CC1)`: DEST, AMOUNT
   - `VELOCITY`: `→AMP`, `→FILTER`; plus `MONO LEGATO` (`RETRIG/LEGATO`)
3. `03 FILTER` (new, material-free) — `KEY TRACK`, `DRIVE` knobs plus a live filter response
   curve drawn from the real cutoff/reso/type values (same honesty rule as `KeithWaveform`).
4. `04 ENVELOPES`, 5. `05 TAPE`, 6. `06 LIBRARY` — unchanged behaviour, page indices shifted.
   Page choice stays UI state, not a parameter.

Permanent `FILTER / VOICE` channel, chassis and workspace materials: **untouched**.

Typography (final slice, coordinator decision 3): one OFL-1.1 condensed grotesque replaces Bangers
for the title/wordmark/heading voice only (`KeithTheme.h:kFontTitle` + `Title()` + the heading
uses in `KeithArtwork`/`DrawTitleStrip`/`KeithBadgeControl`); Space Mono keeps values, labels and
readouts. The face is embedded the same way the other two already are (`config.h` name +
`resources/main.rc` + `CMakeLists.txt` RESOURCES + the VST3/APP font copies), and the licence +
source URL are recorded next to the existing OFL notices. Bangers stays registered until the
substitute is proven loaded; headings are measured for clipping at 1280×800 and the measurement
is reported. `ControlLayout.h` and the material PNGs are not involved.

New knob visuals: `KeithKnobControl` gains an optional native body flag for material-free pages;
it keeps the same 270° track, ticks, label and value readout. No new colours, no new fonts.

## 7. Presets

- All 13 new parameters default to inert, so the 15 existing presets keep their exact sound;
  `AddFactoryPresets` starts from the defaults, so nothing else changes.
- Add deltas for the new controls to a few presets only where they are musically real, e.g.
  LFO→cutoff + rate in `Family 04 / 001 Motion Pad`, key tracking in `Family 02 / 001 Neon Pluck`,
  filter drive in `Family 05 / 003 Tape Kit`. This proves serialization through the host path.
- No new factory content, no renamed families, emulated-family-first order preserved.
- `docs/PARAM-MANIFEST.md` gets the 13 rows (3 flipped from `not yet implemented` to `live`, 10
  new), the U9 code surface table, and updated counts.

## 8. Tests planned

Offline (`tools/synth_offline_test.cpp`, target `keith-synth-tests`) — every new control gets a
measurable assertion plus at least one negative assertion that a control does not touch an
unrelated destination:

1. sync, ring, key track, drive — timbre/spectrum/zero-crossing evidence, all finite.
2. LFO rate (period), shape (waveform), dest (only its destination), amount (depth vs zero).
3. mod wheel: `SetControl(1,…)` → destination only; amount scaling.
4. velocity → amp, velocity → filter.
5. mono legato vs retrig (re-attack present/absent).
6. `new_defaults_are_inert`: a `Params` with only pre-U9 fields set renders identically to one
   with every new field explicitly at its documented default (catches a non-zero struct default).
7. Existing assertions (note-on audio, note-off silence, envelope shaping, filter modes,
   parameter sweep finite) kept unchanged.

MIDI path: planned `keith-dsp-tests` target linking `iPlug2::Extras::Synth`, driving the real
`MidiSynth` + `KeithDSP` with note-on/note-off and CC1 to prove the framework path (not only the
offline mirror). If the target proves disproportionately costly, the equivalent assertion is kept
in `keith-synth-tests` and the gap is reported honestly rather than papered over.

Editor: extend `tools/editor_geometry_test.cpp` with bounds/non-overlap assertions for the two new
pages, and extend the Windows message driver (`tools/editor_input_smoke.py`) to visit
`02 MODULATION` and `03 FILTER`, exercise the new knobs' numeric round-trip, and click the new
tabs with a captured window as evidence. Tab-selection evidence is a capture, and will be
reported as capture-verified, not host-tested.

## 9. Verification to run at the end (unpiped, output preserved)

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

`editor_surface_check.py` needs one addition to match the deliverable: it currently proves a
parameter is *referenced*, not that each is referenced exactly once. The mission asks for
"every live parameter exposed exactly once", so the count of `kParam…` occurrences per parameter in
the editor surface becomes an assertion (presets/`SyncSynthParams` are excluded from that count).

## 10. Explicitly rejected or deferred, with reasons

| Item | Decision | Reason |
|---|---|---|
| Named waveform mode selector | rejected | the morph is continuous and the readout already names the stages; a mode list would fight the morph |
| Second filter | deferred (deep) | would need real routing + CPU accounting before it may be drawn |
| Unison width / stereo spread | deferred (deep) | needs stereo voice output and has no legal knob home without new material |
| 12-LFO / full matrix | rejected | explicitly out of unit scope; a fake matrix is forbidden |
| Tempo-synced LFO | rejected | the brief asks for one tempo-independent LFO |
| Tape expansion | rejected | brief says finish the synth surface first |
| Mod wheel as an on-screen MIDI-learn | rejected | only the real CC1 route is honest here |
| Popup `list` type for the two destination enums | rejected in favour of tab selectors | iPlug2 popup menus need an on-screen list in a 1280×800 design; the tape page already proves tabs are the readable pattern |
| Typography / replacing Bangers | **in scope — bounded final slice** (coordinator decision 3) | owner request. Ship an OFL-1.1 condensed grotesque (Oswald or equivalent) for the title/wordmark/heading voice only; record licence + source URL beside the existing OFL notices; keep Space Mono for values and labels; `KeithArtwork`'s KEITH wordmark is live text so no material PNG or `ControlLayout.h` entry moves. Remove `Bangers-Regular.ttf` only after the substitute is embedded, loaded and proven; until then it stays registered |

## 11. Risks and open questions

- **Six nav buttons** compress the nav labels from 4×232 px to 6×150 px. Coordinator decision 2:
  keep the two pages separate; measure the label widths at the design size and widen the nav cells
  (or drop the numeric prefix) if any label clips.
- **Legato** cannot use the framework's `isRetrigger` (never true), so it is defined as *"another
  key is currently held"* and tracked with a 128-bit key-held mask that is cleared exactly on
  note-off and on all-notes-off/sound-off/reset. The rule is identical in `OfflineEngine` and
  `KeithDSP`, and the offline test proves it; no drift against sustain or CC123.
- New knobs draw a flat native body on material-free pages; if the owner rejects a non-baked knob
  body, the fallback is to keep those controls as tab selectors instead of knobs.
- No change is made to `ControlLayout.h` or to the material PNGs. The only `resources/` change in
  this unit is the optional final typography slice (add the OFL substitute; remove Bangers only
  once the substitute is proven).

## 12. Stop point

U9 stops at: implementation + tests + the four verification commands + pluginval, in this
worktree, with a report split into implemented-and-verified / implemented-not-host-tested /
deferred / blocked. **No merge, no install, no push, no deployment.**
