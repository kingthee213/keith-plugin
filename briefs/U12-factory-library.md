# U12 — factory content library

Coordinator contract. Binding. Read `BRIEF.md` §6 and §8, `banks/SOURCES.md`,
`banks/FAMILY-0*-PROGRAMS.md`, `docs/PARAM-MANIFEST.md`, `src/Keith.cpp`,
`src/Keith.h`, `tools/editor_surface_check.py`, `tools/verify_preset_metadata.py`.

## Mission

Keith ships **15** presets. BRIEF §6 requires a full library: five families, each with factory-program
coverage, and §8 requires a bank that proves the production palette. Build a real, playable library.

## What "factory content" can honestly mean here — read this first

**There are no audio files in this repo and none may be ingested.** `banks/SOURCES.md` forbids Korg
ROM content (rule 1), Splice samples (rule 2), and treats an open-source code licence as irrelevant to
its sample content (rule 5). The owner has no hardware to record.

`banks/SOURCES.md` **rule 6 already sanctions the correct path**:

> "If a patch is synthesised from scratch to match a documented factory sound, that is its Source:
> 'our own recreation, matched to \<document\>'."

So every preset you add is **our own parameter-level recreation**, designed from the engine's own
primitives to sit in the documented role. The `banks/FAMILY-0*-PROGRAMS.md` files are name and
structure references — use them to decide *what kinds of sound belong in each family and category*.
Do not invent counts, do not claim to have ingested sample data, and do not present a name list as
shipped content.

Each new preset needs a row in `banks/SOURCES.md` with `Source = our own recreation` naming the
reference document, and `Licence of ingested asset = none — our own design`.

## Naming rules (non-negotiable)

- Shipped names follow the existing pattern exactly: `Family 0N / NNN <neutral role>`
  (checked by `tools/editor_surface_check.py`).
- **No Korg, Triton, M1, Wavestation, TR, Surge, OB-X, Splice, Neptune/Pharrell/Chad Hugo, or any
  other reference-machine, brand or living-artist name** in a preset name, a family name, a file name
  or plugin metadata. The reference machines are internal documentation only.
- Family labels stay `Family 01`…`Family 05`, ordered by **emulated family first** (Triton, Triton
  Extreme, Korg M1, Korg Wavestation, Korg TR — internal names, docs only), then by neutral sound role.

## Scope

Target **80–120 presets** total, distributed across all five families, covering the roles the family
documents actually list. Suggested role coverage per family (adapt to what the bank files show):

keys, bass, pad, lead, pluck, stab, arp, bells, electric piano, clav, organ, brass, strings, choir,
motion/texture, drone, FX/riser, and percussion/kit.

Also include a coherent bank of roughly 12–16 presets that demonstrates the §8 palette end to end:
clav and key stabs, thin soulful electric piano, hard dry drums, off-kilter synth textures, heavy
per-part FX, sparse arrangement-friendly patches. Name it neutrally — it is a bank, not a person.

## Implementation

- Presets stay **delta lists over the init defaults** in the existing `kPresetDeltas` /
  `kPresetSpecs` tables, using the same parameter serialisation the host uses. Keep that mechanism;
  do not invent a second one.
- Every delta must target a real `EParams` id (checked by the surface checker).
- Every preset must have at least one delta and must actually differ from the defaults.
- New parameters from U9/U10/U11 (LFO, mod wheel, velocity, sync/ring, key track, drive, insert and
  master FX, drum slots) are fair game and should be used where they make the patch real — a pad with
  the delay insert, a kit with drive, a pluck with key tracking.
- Preset count is now data-driven: **update the tools that hardcode 15.** In particular
  `tools/verify_preset_metadata.py` currently asserts exactly 15 rows — rework it to assert
  *coverage and ordering* rather than a fixed count, and keep its emulated-family-first assertion
  meaningful at the new size. Do not weaken it into a no-op.
- `tools/editor_surface_check.py` must keep passing; if it encodes the preset count, update it the
  same way.

## Documentation

- `banks/SOURCES.md`: a row per added preset (or an honest per-batch row naming the family, the
  reference document, and `none — our own design`), plus an updated family status table.
- `docs/PARAM-MANIFEST.md`: update the counts if it tracks preset totals.
- `docs/UI-DESIGN.md`: it currently states 15 original patches; make it accurate.
- `README.md`: update any preset-count claims.

## Tests

- A test or checker that every preset index has at least one delta, every delta targets a real id,
  names are unique, and every family 01–05 is represented — extend the existing checkers rather than
  adding a parallel one.
- Prove the library loads: apply each preset through the plugin's own restore path in a test and
  assert the resulting parameter state differs from the previous preset's state (catches duplicate
  presets and no-op entries).
- All existing tests keep passing (76 offline + 2 CTest + the checkers as they stand).

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

- No sample assets, no sample ingestion, no invented source rows.
- No reference-machine or artist names in anything shipped.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- Do not change the parameter set in this unit; content only.
- No commit, merge, push, install or deploy.
- Report separately: implemented and verified / implemented but not host-tested / deferred / blocked.
- Report the exact preset count you shipped, per family.
