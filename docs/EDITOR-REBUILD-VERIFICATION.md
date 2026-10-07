# Editor rebuild verification checkpoint

Branch: `editorial-redesign`, isolated worktree `D:/TYH_R10/dev/keith-editorial`.
Status: material/control-placement pass built and exercised. Not final design acceptance.
No merge, installation, push or deployment performed for this rebuild.

## Current material pass

- Chassis and all page skins rendered by `tools/render_materials.py` with a fixed orthographic camera and upper-left area light. Uniform world fill adds no second directional light.
- `ControlLayout.h` supplies the same placement bounds and radii to the native controls and renderer. Physical bodies, bearings, contact wear and shadows are positioned around those controls.
- World-space material coordinates keep grain scale independent of panel dimensions.
- Source, pitch and tape images are exact 1940 x 1028 crops at 2x resolution, not stretched generic module textures. Generic dial/module sprites are no longer embedded or drawn.
- `tools/verify_material_layout.py` passes all three crop-size checks and the 27-control placement comparison. This verifies geometry and the render receipt, not artistic quality.
- Official The God Particle and Arcade control references were inspected. Primary-control hierarchy and separation of playing, editing and browsing inform this design; no reference artwork is shipped. See `docs/UI-DESIGN.md` for source links and limitations.

## Functional correction found during this pass

Native numeric prompts were disabled in the custom controls. Enabling them exposed an unloaded default-font dereference in the Windows platform entry path. Knobs and envelope readouts now explicitly enable prompts and use loaded Space Mono text styling. Readout clicks open the input over the readout; knob dragging and double-click default reset remain separate interactions.

`tools/editor_input_smoke.py --pid 51444` exercised the actual Release standalone:

- Cutoff drag: 5000.0 -> 2775.2 Hz.
- Entered 5000 Hz, closed the native field, reopened it and read back 5000.0.
- Volume attack: 20.0 -> 21.0 ms via numeric entry, then reopened and read back 21.0.
- Source -> Envelopes -> Source navigation survived the entry tests.

The earlier first successful run also changed volume attack from 8.0 to 20.0 ms. The repeatable probe alternates 20/21 ms to prove a real change on every run. It waits for native input completion instead of sending subsequent actions while Return/Escape is still pending.

## Verification of this build

- Release `Keith-vst3` and `Keith-app`: built successfully.
- CTest: synth-offline and editor-geometry both pass (2/2).
- Source surface checker: 12 checks pass; all 43 real parameter names referenced, 15 neutral presets across five families.
- Material placement/crop checker: pass.
- Strictness-10 in-process pluginval, GUI tests enabled: exit 0 and final SUCCESS. Its optional external Steinberg validator was not configured and was skipped.
- Actual numeric-entry/drag probe: pass. Evidence: `build/verification/numeric-input.json`.
- SHA-256 confirms the running material-preview executable equals `build/out/Keith.exe`. Hashes for both and the VST3 binary are in `build/verification/material-build-hashes.json`.
- Validator output: `build/verification/pluginval-materials.log`.
- `git diff --check`: pass.

## Current inspection target

- Standalone: `build/material-preview/Keith.exe`, with fonts adjacent.
- Test PID: 51444; window: 4263468. Verify these identifiers before future input.
- Actual-window capture: `C:/Users/noaha/AppData/Local/hermes/cache/images/computer_use_dee21b1ea19e46b58d8ca00699f4dd25.png`.

## Still open

- Owner visual approval, especially consistent perceived lighting and restrained, deliberate wear. A renderer setup is not proof of an award-winning result.
- Small-size legibility and a full current-build interaction sweep beyond the tested controls.
- FL Studio verification of this exact build. pluginval is not FL Studio.
- Generated concept side task: blocked on its checked lane; no image-model concept output was produced. See `design/concepts/BLOCKED.md`. Existing Blender assets are authored physical renders, not claimed image-model references.
- Wider instrument scope: factory ROM integration, drum sampler, modulation matrix and insert effects remain unfinished. This UI pass does not implement or claim them.
