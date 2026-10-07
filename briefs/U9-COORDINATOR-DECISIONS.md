# U9 — coordinator decisions (binding)

Approved by the coordinator after review of `briefs/U9-IDEAS.md`. This file overrides anything
in the IDEAS document that contradicts it.

## 1. Scope B is APPROVED as tabled

- 13 appends, `kNumParams` becomes **56**, `kParamOscSync` = 43 starts the append.
- Existing IDs 0–42 are untouched and must never be renumbered.
- Implement exactly the 13 rows in section 5 of `briefs/U9-IDEAS.md`. Do not add a 14th.
- The constraint analysis in section 2 of that document is accepted and is binding:
  - `src/editor/ControlLayout.h` and the material PNGs are **frozen**. Do not re-render, do not
    change placements.
  - New **knobs** may only go on material-free pages. New **tabs/switches** may sit on existing
    material pages.
  - Every new parameter's default must be inert so the 15 existing presets keep their sound.

## 2. Two new pages are APPROVED

- Keep `02 MODULATION` and `03 FILTER` as **separate** pages. Do not merge them.
  Reason: a dense tool reads as undesigned when regions compete for attention, so FILTER gets its
  own room.
- Six nav buttons in the existing nav bar is acceptable. Verify the labels do not clip at the
  design size, and fix the button width if they do.
- Page choice stays UI state, not a parameter.

## 3. Typography SHIPS in this unit (not deferred)

The owner explicitly requested an Adult Swim-style condensed title treatment. This is an owner
request, so it is in scope — as a bounded FINAL slice, after the control surface is green, so it
cannot destabilise the DSP work.

- Source an **OFL-1.1 condensed grotesque** substitute (Oswald or equivalent).
- Do **not** ship Helvetica Neue, the Adult Swim logo/wordmark, or any proprietary or trademarked
  asset. Recreating a brand mark is out of scope; only the typographic register is in scope.
- Record the licence and the source URL in the repo, in the same form as the existing Bangers and
  Space Mono OFL notices.
- The substitute is used **only** for the title/wordmark/heading voice. **Space Mono stays** for
  values, labels and technical readouts.
- The KEITH wordmark in `KeithArtwork` is drawn as live text, not baked art, so this does not touch
  the material PNGs or `ControlLayout.h`. **Confirm that before and after** the change.
- Verify text metrics: no clipped or overflowing headings at the design size; every heading still
  fits its bounds. Report the measured result.
- Do **not** remove `resources/fonts/Bangers-Regular.ttf` unless the substitute is embedded, loaded
  and visibly proven. Keep the resource registered until then.

## 4. Unchanged hard rules

- Every visible control must have real DSP, a preset delta, and a focused test. No unbacked control.
- Build each vertical slice as it goes: parameter → DSP → serialization → UI → test. Do not
  big-bang the whole unit and compile once.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- No commit, merge, push, install or deploy.
- Report honestly and separately: implemented-and-verified / implemented-not-host-tested /
  deferred / blocked.

## 5. Acceptance gates

Run unpiped and preserve the real output:

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

`pluginval` must end in `SUCCESS` with Editor and Editor Automation included. A green compile is not
proof of sound behaviour.
