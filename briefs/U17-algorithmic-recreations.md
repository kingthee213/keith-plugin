# U17 — algorithmic recreations of the measured reference instruments

Coordinator contract. Binding. Read `docs/DECISIONS.md` (especially **D2**),
`docs/REFERENCE-ANALYSIS.md`, `build/reference-targets.json`, `BRIEF.md` §4, §6.4, §10,
`docs/ENGINE-SCOPE.md`, `docs/PARAM-MANIFEST.md`, `src/synth/CoreSynth.h`, `src/Keith.cpp`,
`src/Keith.h`, `src/editor/EditorialLayout.h`, `tools/synth_offline_test.cpp`,
`tools/analyse_references.py`, `tools/editor_surface_check.py`.

## Mission

The owner wants the sound library to be genuinely varied rather than 103 patches sharing one timbre
palette. He has supplied reference recordings and given a specific direction, quoted verbatim:

> **"but we dont need a sampler, just use the audio to recreate the sounds algorithmically"**

So: **recreate the reference instruments as synthesis, and ship no audio.** There is to be no sampler
engine, no embedded samples, no sample browser and no bundle growth from audio. The reference
recordings are analysis material only and are not in the repository (they live in `reference-audio/`,
which is gitignored).

This also resolves the recording site's redistribution question completely, because nothing is
redistributed.

## Part A — prepare the engine for the targets

Read `docs/REFERENCE-ANALYSIS.md` first. The measured targets demand capabilities the engine may not
have today. Every gap you find must be **implemented, not faked** — if a target cannot be reached
honestly, say so and leave that recreation out rather than shipping a preset that claims to match.

Known tension points to check against the real measurements:

- **Brightness must track pitch.** The piano targets show the spectral centroid rising with pitch
  (roughly 683 Hz at C1 → 2793 Hz at C5) while the harmonic ratio *falls* (0.697 → 0.406). Check
  whether the current oscillator/filter set can do that per note.
- **The choir target is formant-rich and wide**: harmonic ratio 0.122, stereo correlation 0.732, and a
  3.1 s attack. The engine's voice output may be effectively mono — if so, that is a real capability gap
  for this target.
- **Stein Bass C2 has a negative stereo correlation (−0.16)**, i.e. genuinely out-of-phase stereo.
- **The harmonica entry is flagged SUSPECT** in the analysis (a x3 harmonic lock). Do not tune to its
  numbers. If you want that recreation, re-analyse or judge it by ear and say which you did.

Add only the parameters and DSP that the targets actually require. Existing parameters are 0–100 with
`kNumParams` = 101; **append only and never renumber an existing ID.**

## Part B — author the recreations

Add presets that recreate each reference instrument in the correct family, following the existing
neutral naming rules (`Family 0N / NNN <neutral role>`, no reference-machine or brand names in shipped
names — D2 retracted the *content* rule, not the naming rule, and neutral names still keep the owner
safe if he streams with it on screen).

Use the measured targets as the design specification: fundamental, attack, decay to −20 dB, spectral
centroid, rolloff85, harmonic ratio and stereo correlation are all things a preset must actually hit.

The ten references, from `docs/REFERENCE-ANALYSIS.md`:

| Reference | F0 Hz | Attack s | Decay s | Centroid Hz | Harmonic | Corr |
|---|---|---|---|---|---|---|
| Acoustic Bass C2 | 65.4 | 0.116 | 3.48 | 402 | 0.941 | 0.999 |
| Acoustic Bass C3 | 130.7 | 0.023 | 2.15 | 655 | 0.893 | 0.996 |
| Acoustic Bass C4 | 261.5 | 0.029 | 1.59 | 1156 | 0.815 | 0.983 |
| Acoustic Piano C1 | 32.6 | 0.099 | 1.75 | 683 | 0.697 | 0.974 |
| Acoustic Piano C3 | 130.8 | 0.017 | 4.69 | 1283 | 0.648 | 0.953 |
| Acoustic Piano C5 | 523.2 | 0.012 | 2.33 | 2793 | 0.406 | 0.994 |
| Harmonica C5 | **SUSPECT** | 0.07 | 1.45 | 5506 | 0.38 | 0.987 |
| Slow Choir ST C4 | 262.2 | 3.1 | 1.1 | 1477 | 0.122 | 0.732 |
| Stein Bass C2 | 65.4 | 0.012 | 0.62 | 285 | 0.529 | −0.16 |
| Stein Bass C3 | 130.7 | 0.012 | 0.7 | 481 | 0.563 | 0.324 |

## Part C — prove each recreation by re-measuring it

This is the acceptance gate and it must be objective:

1. Extend the offline test harness so it can **render a preset's output to a buffer** for a given
   note, using the plugin's own engine.
2. Run the **same measurements** `tools/analyse_references.py` uses against that rendered buffer.
3. Assert each recreation lands within a stated tolerance of its target on at least: fundamental,
   attack, decay to −20 dB, and spectral centroid.

Refactor the measurement code so the Python tool and the C++ test cannot drift apart — either share one
implementation or explicitly cross-check the two against the same fixed buffer, and say which you did.

**A recreation that cannot be shown to match is not a recreation.** If one misses, either fix it or
report it as unmet with the measured numbers. Do not loosen a tolerance to make a miss pass, and do not
delete a target to shrink the work.

## Part D — provenance stays truthful

The sample-content prohibition is retired (D2), but the provenance rules are not. In
`banks/SOURCES.md`, record each recreation with its real source:

- Source: our own synthesis, **matched by analysis** to the named reference recording.
- Licence of ingested asset: **none — no audio was ingested or shipped.**
- Attribution: name Free Wave Samples / Jason Champion as the reference source. The site asks for a
  link or a mention in return, so add that credit to `README.md` and `docs/MANUAL.md`.

Never label a recreation "our own design" as though no reference existed — the measurement is exactly
what the provenance is for.

## Hard boundaries

- **No audio assets enter the repository or the build. No sampler. No sample loading at runtime.**
- Append parameter IDs only; never renumber an existing ID.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- Do not change existing preset names or the family ordering.
- **Do not commit, merge, push, install anything into `C:\Program Files\...`, or deploy.** Earlier
  workers broke this rule; leave the tree for the coordinator.
- Do not claim macOS, AU, auval, Ableton or FL Studio verification.
- Report separately: implemented and verified / implemented but not host-tested / deferred / blocked.

## Verification — run unpiped, preserve the real output

```text
cmake --build D:/TYH_R10/dev/keith-editorial/build --config Release --target Keith-vst3 Keith-app keith-synth-tests keith-editor-tests
ctest --test-dir D:/TYH_R10/dev/keith-editorial/build -C Release --output-on-failure
python D:/TYH_R10/dev/keith-editorial/tools/editor_surface_check.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_preset_metadata.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_patch_fx_chains.py
python D:/TYH_R10/dev/keith-editorial/tools/verify_material_layout.py
D:/TYH_R10/dev/keith-plugin/tools/pluginval/pluginval.exe --strictness-level 10 --validate-in-process D:/TYH_R10/dev/keith-editorial/build/out/Keith.vst3
```

`pluginval` must end in `SUCCESS` with Editor and Editor Automation included.
`editor_surface_check.py` must end 17 passed, 0 failed.
Report the measured-vs-target table for every recreation.
