# U12–U14 verification record — content library, LFO 2, macOS readiness, packaging

Worktree `D:/TYH_R10/dev/keith-editorial`, branch `editorial-redesign`.

**Every result below was re-run by the coordinator.** No merge, install, push or deployment.

## U12 — factory content library

| Gate | Result |
|---|---|
| Presets | **103** (`kNumPresets = 103`), names all unique |
| Per family | Family 01: 20, 02: 21, 03: 21, 04: 20, 05: 21 |
| Arrangement palette | 14 presets covering the BRIEF §8 palette |
| Naming | `Family 0N / NNN <neutral role>`, no reference-machine or artist names |
| Ordering | emulated family first (Triton → Triton Extreme → Korg M1 → Korg Wavestation → Korg TR), then neutral role |
| Provenance | one own-design row per preset in `banks/SOURCES.md`, per rule 6 |

Content is **our own parameter-level recreation**, not sampled factory data — no audio files exist in
this repo and `SOURCES.md` forbids ingesting any.

`tools/verify_preset_metadata.py` was reworked from a hardcoded "exactly 15" assertion to coverage and
ordering assertions at the new size.

## U13 / regression — the patch archive was deleted, and LFO 2 was added without a UI

Two real defects, both found by verification rather than by a worker report:

1. **The U11 drum pass silently deleted the patch archive.** `KeithArchive` became dead code and the
   drum grid took its place, removing the scrollable nested patch list the owner asked for.
   **Fix:** the Library page is now a nested `PATCHES / DRUMS` deck (the same idiom the envelopes page
   uses), so both surfaces live. Verified live: the 103-row list is back and the mouse wheel scrolls it.
2. **A second LFO was added to the DSP with no editor controls** (`kParamLfo2Rate/Shape/Dest/Amount`,
   IDs 85–88), which broke the project's own rule that every live parameter is exposed.
   **Fix:** the modulation page's LFO region is now an `LFO 1 / LFO 2` sub-deck, so each LFO keeps
   full-size controls. The destination tabs also now expose `FM`, which the DSP supported but the UI
   had never offered.

The second defect was caught by `tools/editor_surface_check.py`, which failed 15/17 before the fix and
passes 17/17 after.

## U14 — macOS/AU readiness and packaging

- `CMakeLists.txt` now stages editor fonts into each Apple bundle's `Contents/Resources` (VST3 and AU)
  and beside the APP, with every Windows-only step guarded behind `WIN32`.
- macOS deployment target set to **11.7.10** per BRIEF §11; `IPLUG2_UNIVERSAL=ON` for arm64 + x86_64.
- AU identity present: `PLUG_UNIQUE_ID 'Kth1'`, `PLUG_MFR_ID 'Prvt'`.
- `tools/build-macos.sh` added: bootstraps iPlug2 the same way the Windows path does, configures with
  the Xcode generator, builds VST3 + AU + APP, and prints where the bundles landed.
- `dist/` produced: the VST3 bundle with its structure preserved, the standalone, the four fonts and
  the OFL notice, plus both install notes.

## Coordinator-verified results (final state)

| Gate | Result |
|---|---|
| Parameter IDs vs previous commit | prefix **byte-identical**; 4 appends (LFO 2), 89 live total |
| Release build (4 targets) | clean, no errors/warnings |
| CTest | 3/3 passed |
| Offline synth tests | **111 passed, 0 failed** |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS — 20/21/21/20/21, palette 14 |
| Material layout check | PASS — 27 placements agree |
| pluginval strictness 10 | exit 0, **SUCCESS**, Editor + Editor Automation completed |
| Frozen assets | `src/editor/ControlLayout.h` and `resources/images` **untouched** |

Live checks: `02 MODULATION` shows LFO 1 / LFO 2 / MOD WHEEL / VELOCITY with FM among the destination
options; `06 LIBRARY` shows PATCHES / DRUMS sub-tabs with the full patch list, and the wheel scrolls it.

## NOT verified — stated plainly

- **macOS, AU and auval: not built, not run, not verified.** All Part B work is unexecuted; there is no
  Mac in this workspace and `tools/build-macos.sh` says so in its own header. The owner will host-test.
- **FL Studio and Ableton: never loaded this build.** pluginval is not a DAW.
- CC1 and mono legato remain offline-proven only.
- The drum engine is synthesized, so it will not match the reference machines' percussion timbres.

## Still open from the wider brief

The modulation matrix (§4.1) is not implemented. Multi-part routing (`synth_part`), insert-FX
per-patch presets of the reference machines' own chains, and the macOS side of BRIEF §12 remain open.
