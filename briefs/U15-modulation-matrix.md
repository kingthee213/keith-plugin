# U15 — modulation matrix

Coordinator contract. Binding. Read `BRIEF.md` §4.1, `docs/PARAM-MANIFEST.md`,
`docs/ENGINE-SCOPE.md`, `docs/U12-U14-VERIFICATION.md`, `src/Keith.h`, `src/Keith.cpp`,
`src/synth/CoreSynth.h`, `src/editor/EditorialLayout.h`, `src/editor/EditorGeometry.h`,
`tools/synth_offline_test.cpp`, `tools/editor_surface_check.py`.

## Mission

`BRIEF.md` §4.1 names a **modulation matrix** as part of the modelled engine. Today the modulation
surface is fixed wiring: LFO 1 → one destination, LFO 2 → one destination, mod wheel → one destination,
plus velocity and key tracking. Build the real matrix: several slots, each **source → destination →
amount**, all genuinely routed in the DSP.

This is the last named synthesis feature in the brief that is not implemented.

## Parameters to add (append only, starting at ID 89)

The current top ID is **88** (`kParamLfo2Amount`); `kNumParams` is 89. **Read `src/Keith.h` and start
after whatever the highest ID actually is.** Never renumber an existing ID.

**4 slots × 3 = 12 appends:**

| Enum | Label | Values |
|---|---|---|
| `kParamMod1Source` | Mod1 Src | `Off / LFO 1 / LFO 2 / Mod Wheel / Velocity / Key` |
| `kParamMod1Dest` | Mod1 Dest | the real destination set — see below |
| `kParamMod1Amount` | Mod1 Amt | −100…+100 % |
| …then `kParamMod2*`, `kParamMod3*`, `kParamMod4*` | | |

Host names: `Mod1 Src`, `Mod1 Dest`, `Mod1 Amt`, … `Mod4 Amt`.

### Destinations must be real — no dead options

The engine already has a `ModDest` enum in `src/synth/CoreSynth.h` used by the LFOs and mod wheel, with
an existing `ApplyModRoute` helper. **Reuse that set rather than inventing a second one**, and remove any
option you cannot genuinely route. Do not ship a destination that does nothing.

- **Multiple slots targeting the same destination must sum**, and the summed modulation must stay
  bounded so no combination can produce NaN/Inf or destabilise the filter.
- Amount `0` and Source `Off` must be exact no-ops.
- The existing per-source routings (LFO 1, LFO 2, mod wheel) must keep working exactly as they do now;
  the matrix is additive, not a replacement. Prove that with a test.

## Editor

The `02 MODULATION` page's LFO region is already a nested sub-deck with `LFO 1` / `LFO 2` sub-tabs.
**Extend that deck to a third sub-tab, `MATRIX`**, so the matrix gets the region
`(58,218)-(500,540)` and readable controls rather than a cramped strip.

- Four rows, one per slot, each with its source selector, destination selector and amount knob.
- Every selector's option list must match its parameter's real option count.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- `tools/editor_surface_check.py` must still pass **17/17**, and every new parameter must be exposed
  exactly once.

## Tests (in `tools/synth_offline_test.cpp`)

- Per slot: a positive assertion that its destination measurably moves, and a **negative** assertion
  that the slot does not move an unrelated destination.
- `mod_slots_sum_on_shared_destination`.
- `mod_off_and_zero_amount_are_inert`.
- `mod_max_all_slots_is_finite`.
- `mod_matrix_does_not_disturb_existing_routings` — the pre-existing LFO/mod-wheel paths behave
  identically with the matrix at default.
- `mod_new_defaults_are_inert` — the 103 existing presets must keep their exact sound.
- All existing tests keep passing (currently 111 offline assertions).

## Docs

- `docs/PARAM-MANIFEST.md`: add the 12 rows and a U15 code surface table; update the counts.
- `docs/ENGINE-SCOPE.md`: document the matrix, its sources, its real destination set, and the summing
  and clamping rule.
- `README.md`: reflect the real parameter count and note the matrix.

## Hard boundaries

- Append parameter IDs only. Never renumber an existing ID.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- **Do not commit, merge, push, install anything into `C:\Program Files\...`, or deploy.** Two earlier
  workers broke the commit rule; leave the tree for the coordinator.
- Do not claim macOS, AU, auval, Ableton or FL Studio verification. None were performed.
- Do not change the preset library's names or ordering.
- Report separately: implemented and verified / implemented but not host-tested / deferred / blocked.

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
`editor_surface_check.py` must end 17 passed, 0 failed.
