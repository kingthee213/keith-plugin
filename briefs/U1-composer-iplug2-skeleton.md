# U1 — composer — iPlug2 skeleton and build system

**Lane:** `keith-composer` (Cursor, `composer-2.5`). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first, in this order:** `BRIEF.md` (locked spec), `../preferences.md` run rules,
`docs/UNIT-PLAN.md`.

## Scope of this pass — the build system, nothing else

You are building the container the rest of the plugin goes into. **No DSP, no synth voice, no FX, no
UI** in this pass. One sounding test voice is allowed and required so the build is provably audible.

Deliverables:

1. **Repo layout** that the later units will fill. Suggested and open to your judgement, but record
   what you chose:
   ```
   CMakeLists.txt
   cmake/                     (toolchain + platform helpers)
   config/                    (iPlug2 config: <Product>.config, <Product>-mac.xcconfig, -win.vcxproj)
   src/                       (plugin entry, later engine/ and fx/)
   resources/                 (fonts, art — empty this pass)
   tools/bootstrap-deps       (fetches the iPlug2 SDK at a PINNED revision)
   .github/workflows/build.yml
   ```
2. **iPlug2 dependency, pinned, and never floated.** iPlug2 is zlib-like licensed — free in closed
   source. It ships its own deps (WDL, NanoVG, NanoSVG, RTAudio, RTMidi, Steinberg VST3 SDK) as nested
   submodules, so it must be fetched **recursively**. Two acceptable forms:
   - a `tools/bootstrap-deps` script that clones iPlug2 **recursively at a pinned tag or commit SHA**
     into `third_party/iPlug2/` (already gitignored), and that CI runs too; or
   - CMake `FetchContent` with `GIT_REPOSITORY` + `GIT_TAG <pinned sha>` + submodules enabled.
   Either way: **record the exact pinned revision in `README.md`** and make it the same revision CI
   uses. A floating `main` is not acceptable.
3. **Targets that configure and build:**
   - Windows x64 **VST3**, built and **verified on this host** (see gate).
   - macOS **universal (arm64 + x86_64) AUv2 + VST3** targets defined and configured. You cannot build
     these on Windows — define them correctly and prove the configure step is well-formed; build
     correctness on macOS is a CI + real-Mac question, and you say so in the report.
   - Deployment floors: macOS 11.7.10, Windows 10 22H2. Set them in the build config, don't assume.
4. **One sounding voice.** A single oscillator (any waveform) → gain → plugin output, with the gain
   exposed as one automatable parameter. This exists only so pluginval and Live can prove audio flows.
   It is explicitly **not** the engine — a later unit replaces it.
5. **Identity, neutral.** Plugin name `Keith` for now (working name; the coordinator changes it in
   `BRIEF.md` first if it changes). Manufacturer string neutral. **No Korg names, no Triton/M1/
   Wavestation strings, no Gorillaz or Green Lantern marks** in plugin metadata, bundle name, file
   names or the AU subtype/manufacturer codes. Pick codes that are neutral and record them.
6. **`pluginval` wired** and runnable against the Windows VST3 with a documented command. It must pass
   at strictness 10; if it fails on something outside your scope, report the exact failing check.
7. **CI matrix** in `.github/workflows/`: an **apple-silicon** mac runner, an **Intel** mac runner, and
   a **Windows** runner. Build + the AU/VST3 artefacts on each. Free-tier runners only — no paid CI, no
   new recurring cost. If the provider no longer offers an Intel mac runner (BRIEF [A2]), **report that
   fact** and move that leg to a documented local step — never silently drop it.
8. **`README.md`**: what this repo is, the pinned iPlug2 revision, how to configure and build on
   Windows and macOS, how to run pluginval, and the iPlug2 attribution line plus its bundled
   permissive deps (WDL, NanoVG, NanoSVG, RTAudio, RTMidi, VST3 SDK) in the licence notes.

## Hard rules for this pass

- **Do not run any git command.** No add, commit, branch, checkout, stash, push, rebase. The
  coordinator owns git. Leave files on disk.
- Do not ingest Surge or OB-Xf source (GPL-3.0). Do not add Freakshow code. Do not add a paid JUCE tier.
- No network requirement at plugin runtime. No telemetry, no licence server, no analytics.
- Do not commit a secret or an API key anywhere.

## Gate — what must be true and pasted

1. `cmake --build` (or the platform equivalent) **green on this host** for the Windows x64 VST3, with
   the build tail pasted.
2. The built `.vst3` exists on disk — paste `ls -la` of the actual artefact path.
3. `pluginval` run against it, with its real output pasted: strictness level, pass/fail, and the exact
   failing check if any.
4. The CI workflow file exists and is syntactically valid (`actionlint` if available, otherwise your
   own read-through stated as such).
5. Which iPlug2 revision was pinned, quoted exactly.

## Report

Write `C:/Users/noaha/.agents/orchestrate/keith-plugin/U1-report-composer.md`:
status · branch/HEAD (read-only `git rev-parse HEAD` is allowed, mutation is not) · every command you
actually ran with **real pasted output** · verdict · deviations from this brief · **NOT verified** (the
honest list — what you did not build, could not build here, or did not measure) · what needs the
coordinator. Finish the pass with the tree left in a state that builds.
