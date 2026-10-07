# Keith — DeepSeek session handoff

Updated: 2026-10-07

## Immediate instruction

Continue the Keith rebuild in `D:/TYH_R10/dev/keith-editorial` on branch `editorial-redesign`. Do not restart the art pass. solace wants DeepSeek to steer the implementation from this point because GPT usage is nearly exhausted.

The current priority is **control-surface depth**, not more texture or character art. Move Keith from its compact 43-parameter synth toward a deliberate middle ground between the current engine and Surge-level depth. Use Surge's public manuals/GitHub as clean-room UX and feature references only. Do not copy GPL code, coefficient tables, wavetables, factory patches or UI.

## Read first

1. `D:/TYH_R10/AGENTS.md`
2. `BRIEF.md`
3. `docs/ENGINE-SCOPE.md`
4. `docs/PARAM-MANIFEST.md`
5. `docs/UI-DESIGN.md`
6. `docs/EDITOR-REBUILD-VERIFICATION.md`
7. `src/Keith.h`
8. `src/Keith.cpp`
9. `src/synth/CoreSynth.h`
10. `src/editor/EditorialLayout.h`
11. `src/editor/EditorialControls.h`
12. `src/editor/KeithControls.h`
13. `tools/synth_offline_test.cpp`
14. `tools/editor_surface_check.py`
15. `tools/editor_geometry_test.cpp`
16. `briefs/U9-deepseek-control-surface.md`

## Current verified implementation

- Worktree: `D:/TYH_R10/dev/keith-editorial`
- Branch: `editorial-redesign`
- Windows x64 standalone and VST3 targets
- iPlug2 framework
- Live parameter IDs: `0–42`
- Engine: two morphing oscillators (sine → triangle → saw → pulse), sub, noise, simple FM, LP12/LP24/BP/HP filter, amp/filter/pitch envelopes, poly/mono, portamento, unison, detune, global tape bus
- UI pages: Source, Envelopes, Tape, Library; permanent Filter/Voice strip
- 15 neutral presets across Family 01–05
- Internal preset provenance/order: Triton → Triton Extreme → Korg M1 → Korg Wavestation → Korg TR, then neutral sound category
- Existing host parameter IDs must never be renumbered
- New parameters must append after ID 42

## Last verified gates

The identity-plate build was verified after the latest source changes:

- Release VST3 and standalone build: pass
- CTest: 2/2 pass
- Editor surface checker: 12/12 pass
- Preset provenance checker: pass
- Material/control placement checker: pass
- Strictness-10 in-process pluginval: `SUCCESS`
- Editor test: pass
- Editor Automation: pass
- External Steinberg VST3 validator: skipped because no validator path is configured

Latest pluginval log:
`D:/TYH_R10/dev/keith-editorial/build/verification/pluginval-identity-plate.log`

Do not treat those results as proof of the expanded control surface. Re-run all relevant gates after changes.

## User priorities

- Take UX lessons from The God Particle and Output Arcade without copying either.
- More meaningful controls are wanted; the current 43-parameter surface is too small.
- Labels must describe real DSP behavior. No invented or redundant controls.
- A useful middle ground is preferred over full Surge complexity.
- The synth must be functionally sound, not just visually convincing.
- Preset families must be organized by emulated synth family first, then instrument/sound category.
- Keep Family 01–05 visible in the UI; reference-machine names stay internal/documentary only.
- Leave the existing art/material/chassis work alone for this pass.
- The Adult Swim reference is typography only: use a license-permitting condensed grotesque substitute if changing the title face; do not copy the Adult Swim logo or ship proprietary Helvetica/brand assets. Space Mono remains the technical/value face.

## Required next sequence

1. Verify Git state and confirm no other writer is changing this worktree.
2. Write `D:/TYH_R10/dev/keith-editorial/briefs/U9-IDEAS.md` before editing source.
3. Compare three scopes:
   - Conservative: high-value additions that fit the current engine.
   - Middle: recommended target, approximately 55–70 live parameters.
   - Deep: later modern-hybrid-synth scope, explicitly deferred.
4. Choose the middle scope unless a concrete engine constraint says otherwise.
5. Create a parameter/DSP migration table before implementation.
6. Implement vertical slices: parameter definition → DSP behavior → preset serialization → UI control → focused test.
7. Prioritize high-value additions such as one real LFO/modulation route, key tracking, filter drive, velocity behavior, oscillator routing/mode controls, or other controls only when their DSP is real.
8. Do not add a fake full modulation matrix, 12-LFO interface, second filter, drum controls or insert-FX controls without actual implementation.
9. Build and test both targets.
10. Run CTest, surface checks, preset metadata checks, material checks and GUI-enabled strictness-10 pluginval.
11. Report implemented/verified, implemented but host-untested, deferred and blocked separately.
12. Stop before merge, install, push or deployment.

## Important boundaries

- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- Do not merge, commit, push, install or deploy without explicit solace approval.
- Do not claim FL Studio verification unless it is actually performed.
- Do not claim full instrument completion: factory ROM integration, working drum engine, modulation matrix, insert effects and broader planned parameter scope remain open.
- Do not include credentials, tokens, keys or connection strings in reports.
- Do not infer visual approval from screenshots or pluginval. solace owns the final taste decision.

## Existing detailed brief

The full implementation contract is in:
`D:/TYH_R10/dev/keith-editorial/briefs/U9-deepseek-control-surface.md`

Start with `U9-IDEAS.md`, then proceed in bounded, testable slices.
