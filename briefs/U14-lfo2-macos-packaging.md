# U14 — bind LFO 2, macOS/AU readiness, and packaging

Coordinator contract. Binding. Read `BRIEF.md` §5, §11, §12, `docs/INSTALL-MAC.md`,
`docs/INSTALL-WINDOWS.md`, `README.md`, `docs/PARAM-MANIFEST.md`, `src/Keith.h`, `src/Keith.cpp`,
`src/synth/CoreSynth.h`, `src/editor/EditorialLayout.h`, `src/editor/EditorGeometry.h`,
`config.h`, `CMakeLists.txt`, `tools/editor_surface_check.py`.

Three parts, in this order. **Part A first — it is a live failing gate.**

---

## Part A — bind LFO 2 (fixes a failing check)

A previous pass added a second LFO to the DSP: parameters `kParamLfo2Rate` (85), `kParamLfo2Shape`
(86), `kParamLfo2Dest` (87), `kParamLfo2Amount` (88), wired in `src/synth/CoreSynth.h` and covered by
the offline tests. **But no editor control was added for them**, so `tools/editor_surface_check.py`
currently fails two checks:

```
FAIL every live parameter is exposed by the editor surface  [missing Lfo2Rate, Lfo2Shape, Lfo2Dest, Lfo2Amount]
FAIL every live parameter is attached exactly once          [x0 for each]
```

The DSP already works, so do **not** delete it. Expose it.

- The `02 MODULATION` page currently gives LFO 1 the region `(58,218)-(500,540)` with RATE and AMOUNT
  knobs plus SHAPE and DEST tab rows, and the page is already crowded with MOD WHEEL, VELOCITY and
  LEGATO regions. Two full LFOs will not fit side by side at readable size.
- **Use the nested-deck idiom already used by the envelopes page and the Library page**: make the LFO
  region a sub-deck with two sub-tabs, `LFO 1` and `LFO 2`, each sub-page holding the same full-size
  control layout. Both LFOs then get real, readable controls and nothing is hidden behind a cramped
  half-width layout.
- **Read the real parameter definitions before choosing the destination/shape option lists.** LFO 2
  shares `ModDest` and `LfoShape` with LFO 1, but the tab option lists in the editor must match the
  parameter's actual option count exactly. If a destination that exists in the enum is not offered by
  the existing tabs, say so in your report rather than guessing.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- `tools/editor_surface_check.py` must pass 17/17 afterwards.

---

## Part B — macOS / AU build readiness

**This is the owner's stated top priority: he needs a macOS build more than Windows.** There is no
Mac available to build on right now, so this part is about making the repo genuinely Mac-ready and
removing every Windows-only assumption, so the build works the first time it runs on a Mac.

Verified facts to build on:

- Our own `src/` contains **zero** `_WIN32` / `windows.h` / Win32 API usage. The plugin code is already
  cross-platform; only the build glue is Windows-flavoured.
- `CMakeLists.txt` already branches: `if(APPLE) set(KEITH_FORMATS VST3 AU APP)` else `VST3 APP`.
- `resources/main.rc` is a Windows resource script and must only be used on Windows.

Do these:

1. **Resource deployment on macOS.** The current POST_BUILD copy steps that place the fonts where the
   editor can load them are written for Windows only. On macOS the faces must land in each bundle's
   `Contents/Resources` for the VST3 and AU, and beside the APP. Add the Apple path so
   `LoadFont(...)` resolves on macOS. Guard every Windows-only step (the `.rc` compile, the `.exe`
   font copy) behind `WIN32`, and add the Apple equivalent behind `APPLE`.
2. **AU identity.** Make sure the AU has a real four-character subtype and manufacturer code set
   (`PLUG_UNIQUE_ID` / `PLUG_MFR_ID` or the CMake equivalents) so `auval` can find it. Report the exact
   codes you set. If they are missing today, that is a live macOS blocker — say so.
3. **Deployment target.** BRIEF §11 names macOS 11.7.10 as the floor. Set the Apple deployment target
   accordingly and report it.
4. **A build script.** Add `tools/build-macos.sh` that configures with the Xcode generator and builds
   the VST3, AU and APP targets, then prints where the bundles landed. It must bootstrap or locate
   iPlug2 the same way the Windows path does — read `tools/bootstrap-deps` and match its behaviour
   rather than inventing a second scheme.
5. **`docs/INSTALL-MAC.md`.** Keep the existing Gatekeeper / quarantine instructions
   (`xattr -dr com.apple.quarantine`). Add the real build steps. **The status must be honest: the
   macOS binary has NOT been built and has NOT been verified on a Mac.** Do not imply otherwise
   anywhere in the file.

**You cannot build or test any of Part B on this Windows machine.** That is expected. Make the changes,
verify only what is verifiable here (that the Windows build still works and that your CMake guards are
correct by reading), and report clearly and separately exactly what is unverified and why.

---

## Part C — package and documentation

1. **Produce `dist/`** containing the Windows VST3 bundle with its structure preserved
   (`Keith.vst3/Contents/...`) and the standalone with its font files beside it, plus the install
   notes. Report the file list and sizes.
2. **Report the md5 of the built VST3 binary and of the standalone**, so the owner can confirm that
   what he installs matches what was verified.
3. **`README.md`** must be accurate: what the instrument is now, the live parameter count, the page
   list, that the drum section is synthesized rather than sampled and why, the FX chain, the font
   licences (Oswald and Space Mono, OFL 1.1), how to build on each platform, and a short
   **"what is not built"** list that does not overstate.
4. **`docs/PARAM-MANIFEST.md`**: make sure it matches the real parameter set, including the LFO 2 rows.

---

## Hard boundaries — all parts

- Append or expose parameter IDs only. Never renumber an existing ID.
- Do not touch `src/editor/ControlLayout.h` or the material PNGs under `resources/images/`.
- Do not edit `D:/TYH_R10/dev/keith-plugin`.
- **Do not commit, merge, push, install into `C:\Program Files\...`, or deploy.** A previous worker
  broke this rule by committing; do not repeat it. Leave the tree for the coordinator to commit.
- Do not claim macOS, AU, auval, Ableton or FL Studio verification. None of those were performed.
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
