# U16 — per-patch FX chains, control manual, round-trip proof

Coordinator contract. Binding. Read `BRIEF.md` §6.4, §7, §12, `docs/U10-VERIFICATION.md`,
`docs/U15-VERIFICATION.md`, `banks/SOURCES.md`, `banks/FAMILY-0*-PROGRAMS.md`,
`docs/DRUM-MAP.md`, `src/Keith.cpp`, `src/Keith.h`, `src/fx/PatchEffects.h`, `README.md`.

Three parts. All three are named deliverables in `BRIEF.md` §12 and are the last closed gaps in the
documented acceptance list.

---

## Part A — preload a real FX chain per preset (BRIEF §6.4)

BRIEF §6.4: *"Triton-family programs ship preloaded with the effects they have in the Tritons — the
factory program's insert + master FX chain is embedded per patch and reproduced on load, not left
dry."*

The mechanism exists: U10 shipped `src/fx/PatchEffects.h` with two insert stages and a master stage,
each with real algorithms (`Off / Drive / Chorus / Delay / Crush` and `Off / Comp / Limit / Width /
Tilt`), and presets already carry FX deltas. Today only a handful of presets set them.

Do this:

1. **Give every one of the 103 presets a deliberate FX chain** via `kPresetDeltas`, chosen to suit the
   patch's family character and role — not the same chain stamped everywhere. A dry clav should not
   carry a long delay; a pad should. A kit wants drive; a bell wants space.
2. **Use the documented family FX material as the design source.** The family bank files carry
   FX-algorithm catalogues and routing for the reference machines. Read them, and choose our own
   algorithms to match the *character* the documentation describes. Record that provenance.
3. **Do not ingest anything.** `banks/SOURCES.md` rules forbid the reference machines' ROM data and
   anything from a GPL or non-commercial source. The chain you write is our own design informed by
   published algorithm names and routing — say exactly that in the provenance row.
4. **Every preset must still differ from the defaults and from its neighbours.** The existing checkers
   and the restore test must keep passing.
5. Add a test that a preset with a non-`Off` chain actually routes through it (measurable difference
   in output with the chain engaged vs bypassed).

## Part B — the control and preset manual (BRIEF §12)

BRIEF §12 requires *"README, install steps for both OSes, and a preset/control manual including the
drum map."* Install steps exist. Write the manual as **`docs/MANUAL.md`**:

- What the instrument is (Yote Machine from The Youth Hostel DSP) and what it is not.
- The signal path, in order, exactly as implemented: voices → synth bus → Insert 1 → Insert 2 →
  tape bus → Master → output level → gain.
- Every page and every control, with what each one actually does. Use the real labels from the editor.
  No invented controls, and do not describe anything that is not implemented.
- The full **drum map** (MIDI 36–51 → slots 01–16, the voice each slot produces, the velocity and
  round-robin behaviour, and that the pads are synthesized rather than sampled).
- The modulation surface: LFO 1, LFO 2, mod wheel, velocity, key tracking, and the 4-slot matrix.
- The preset library: 103 presets, five families, how the families are ordered, and that the content is
  our own recreation rather than sampled factory data.
- A short, accurate **"not built"** section.
- Cross-link it from `README.md`.

**Accuracy rule:** every claim in the manual must be checkable against the source. If you cannot verify
a behaviour, do not write it down as a feature.

## Part C — preset save/load round-trip proof (BRIEF §12)

BRIEF §12 requires *"Preset save/load round-trips exactly: parameters + sample map + preloaded FX chain.
Live set save/reopen restores state exactly."*

There is no sample map, because the drum section is synthesized. Prove what is real:

- Add a test that serialises plugin state (all parameters including the FX and drum and matrix values),
  restores it into a fresh instance, and asserts **every parameter round-trips to the same value**.
  Use the plugin's own state save/restore path, not a hand-rolled copy.
- Extend the existing preset-restore test if that is cleaner than adding a second one.
- Report the exact parameter count covered.

---

## Hard boundaries

- Append parameter IDs only if you genuinely need a new one; **never renumber an existing ID**.
  Parameters are currently 0–100 with `kNumParams` = 101.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- Do not change preset names or their family ordering.
- **Do not commit, merge, push, install anything into `C:\Program Files\...`, or deploy.** Three
  earlier workers broke the commit rule; leave the tree for the coordinator.
- Do not claim macOS, AU, auval, Ableton or FL Studio verification. None were performed.
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
