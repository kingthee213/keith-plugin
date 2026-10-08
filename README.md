# Keith

Keith is a two-oscillator instrument with 111 automatable parameters (IDs 0-110), 109 presets in Family 01-05, a synthesized drum kit, a tape bus, and insert and master effects. The Windows VST3 and the standalone share one editor. The plugin name, the binary and the preset names stay Keith. The editor wordmark reads Yote Machine. The service plate reads The Youth Hostel DSP.

Locked product spec: [`BRIEF.md`](BRIEF.md). Control and preset manual, including the full drum
map and the FX chain of all 109 presets: [`docs/MANUAL.md`](docs/MANUAL.md).

## Pages

Seven pages. The right-hand channel stays up on all of them: filter type, cutoff, resonance, envelope depth, engine, poly or mono, glide, unison and detune. [`docs/MANUAL.md`](docs/MANUAL.md) documents every page and control with its real editor label.

1. **Source.** Two oscillators, each with morph, tune, level and pulse width, plus sub, noise, FM, sync and ring.
2. **Modulation.** LFO 1, LFO 2 and the four-slot matrix are sub-tabs of one full-size desk. Each matrix row has source, destination and bipolar amount controls. Mod wheel, velocity and mono legato sit beside that desk.
3. **Filter.** Response plot, key track and drive.
4. **Envelopes.** Volume, pitch and filter are sub-tabs of one graph.
5. **Tape.** Drive, head bump, wow, flutter, hiss, print, dry/wet, bypass.
6. **Library.** Patches and drums are sub-tabs.
7. **FX.** Insert 1, insert 2 and master.

## Drums

The drum kit is synthesized. `src/drums/DrumSynth.h` builds each hit from oscillators, deterministic noise, envelopes, a one-pole filter and soft clipping. The file has no sample playback and there is no sample map. MIDI notes 36-51 trigger the sixteen slots. A pad click selects a slot. The slot knobs edit that slot. Full map: [`docs/MANUAL.md`](docs/MANUAL.md#drum-map) and [`docs/DRUM-MAP.md`](docs/DRUM-MAP.md).

## Effects

`src/Keith_DSP.h` runs the chain in this order: synth voices plus the drum bus, insert 1, insert 2, tape, master, output level, then the Gain parameter.

Insert 1 and insert 2 each take Off, Drive, Chorus, Delay or Crush. Master takes Off, Comp, Limit, Width or Tilt.

Every one of the 109 presets preloads a deliberate chain: insert 1 and the master stage on all of them, insert 2 where the role wants more space. `docs/MANUAL.md` lists the chain per preset, `banks/SOURCES.md` records the provenance, and `tools/verify_patch_fx_chains.py` gates the table.

## Modulation matrix

The matrix has four additive slots. Sources are Off, LFO 1, LFO 2, Mod Wheel, Velocity and Key.
Destinations are the engine's existing Off, Cutoff, Pitch, Shape, Level and FM routes. Slots on one
destination sum, then the matrix contribution is clamped to one full-scale route. Off sources and
zero amounts are exact no-ops. The matrix defaults are inert in all 106 factory presets.

## What is not built

- No macOS binary has been compiled from this tree. The AU, the macOS VST3 and the macOS standalone have not been run. `auval` has not been run. Ableton and FL Studio have not loaded this build.
- The plugin is unsigned and not notarised.
- Multi-part synth routing is not built.
- Preset save, bank and favorites are not parameters.
- There is no sequencer, and swing is not a parameter.
- The documented factory program lists from the reference machines are not in the plugin.

## Pinned iPlug2 revision

```
d54f69050f517e43b941d88c2a170f0a840b9ee4
```

Fetched recursively into `third_party/iPlug2/` (gitignored) by `tools/bootstrap-deps`. CI uses
the same script and revision. **Do not float `main`.**

## Plugin identity (neutral)

| Field | Value |
|---|---|
| Name | Keith |
| Manufacturer | Private Audio |
| VST3 unique ID | `Kth1` |
| Manufacturer ID | `Prvt` |
| AU type | `aumu` (instrument) |
| AU subtype | `Kth1` |
| AU manufacturer | `Prvt` |

## Repo layout

```
CMakeLists.txt
cmake/KeithPlatform.cmake    # deployment floors (macOS 11.7.10, Win10 22H2)
config/                      # Keith.config, Keith-mac.xcconfig, Keith-win.props
config.h                     # iPlug2 plugin config (required at project root)
src/                         # instrument, drums, effects, editor
resources/                   # fonts, page art, Windows .rc, AU/VST3/APP plists
tools/bootstrap-deps         # clone iPlug2 at pinned SHA + fetch VST3 SDK
tools/build-macos.sh         # Xcode build of VST3, AU and APP (not yet run)
.github/workflows/build.yml
```

## Prerequisites

- **Git** (recursive submodules)
- **CMake** 3.14+
- **Windows:** Visual Studio 2022 or later with “Desktop development with C++”
- **macOS:** Xcode. `tools/build-macos.sh` uses the Xcode generator. That script has not been run.
- **pluginval** (validation only): [Tracktion/pluginval releases](https://github.com/Tracktion/pluginval/releases)

## Bootstrap dependencies

```bash
./tools/bootstrap-deps
```

Windows PowerShell:

```powershell
.\tools\bootstrap-deps.ps1
```

This clones iPlug2 at the pinned SHA and runs iPlug2’s SDK download scripts (VST3 SDK, etc.).

## Build — Windows (x64 VST3 + standalone)

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DIPLUG_DEPLOY_PLUGINS=OFF
cmake --build build --config Release --target Keith-vst3 Keith-app
```

Artefacts: `build/out/Keith.vst3` and `build/out/Keith.exe` (standalone).

Install: copy the `.vst3` bundle to `C:\Program Files\Common Files\VST3` (see
[`docs/INSTALL-WINDOWS.md`](docs/INSTALL-WINDOWS.md)).

## Build — macOS (universal AUv2 + VST3 + standalone)

Deployment floor **macOS 11.7.10**, universal **arm64 + x86_64**. `tools/build-macos.sh` calls `tools/bootstrap-deps` for the pinned iPlug2 checkout, configures with the Xcode generator, and builds `Keith-vst3`, `Keith-au` and `Keith-app`.

```bash
./tools/build-macos.sh
```

That script has not been run. No macOS binary has been produced or checked. See [`docs/INSTALL-MAC.md`](docs/INSTALL-MAC.md).

AU type `aumu`, subtype `Kth1`, manufacturer `Prvt` (`PLUG_UNIQUE_ID` / `PLUG_MFR_ID` in `config.h`, repeated in `resources/Keith-AU-Info.plist`).

## pluginval (strictness 10)

After a Release Windows build:

```powershell
pluginval --strictness-level 10 --validate-in-process^
  "build\out\Keith.vst3"
```

## Standalone

`build/out/Keith.exe` runs the same engine without a host. Click the on-screen
keyboard, or play from the computer keyboard: **A W S E D F T G Y H U J K O L**
(one octave), **Z / X** shift the octave. The editor faces
(`Bangers-Regular.ttf`, `Oswald-Bold.ttf`, `SpaceMono-Regular.ttf`,
`SpaceMono-Bold.ttf`) are embedded in the binary and also copied next to the
exe. On macOS the same files are staged into each bundle's `Contents/Resources`, and also beside `Keith.app`. The macOS app has not been built, so that copy is unverified. If you move the Windows exe alone, copy the four TTFs with it or the editor draws no text.

## Current engine and editor

The pages above are the live editor. Parameter IDs and defaults are in [`docs/PARAM-MANIFEST.md`](docs/PARAM-MANIFEST.md). The editor uses original baked Cycles materials and native parameter controls. See [UI design](docs/UI-DESIGN.md).

```powershell
cmake --build build --config Release --target keith-synth-tests keith-editor-tests
ctest --test-dir build -C Release --output-on-failure
python tools/editor_surface_check.py
python tools/verify_preset_metadata.py
python tools/verify_patch_fx_chains.py
python tools/verify_material_layout.py
```

Rendering the authored material assets is optional for a normal build. PNGs are checked into the
asset set and embedded in Windows binaries. To rebuild them with Blender 5.x:

```powershell
blender --background --factory-startup --python tools/render_materials.py
```

## CI

`.github/workflows/build.yml` requests this matrix. A green run of that workflow is not claimed from this tree. The macOS jobs in the file still use Ninja. The local Mac procedure is `tools/build-macos.sh`.

| Runner | Target requested by the workflow |
|---|---|
| `macos-14` | Apple Silicon, universal AU + VST3 |
| `macos-15-intel` | Intel Mac, universal AU + VST3 (`macos-13` retired; see BRIEF [A2]) |
| `windows-latest` | Windows x64 VST3 |

## Licence notes

- **Keith** plugin code: private handover; not published.
- **Analysis references.** The acoustic bass, acoustic piano, slow choir, harmonica and short bass
  recreations in Family 01 (Upright Bass, Hammer Keys, Swell Choir, Bright Reed, Snap Bass, Short
  Bass) were matched by analysis to recordings from Free Wave Samples by Jason Champion
  (freewavesamples.com). Those files were not copied into this repo or the build. The presets are
  synthesis.
- **iPlug2** — zlib-style licence. Attribution: *“This product uses the iPlug 2 library,
  Copyright (c) the iPlug 2 developers.”*
- **Bundled permissive dependencies** shipped with iPlug2: WDL, NanoVG, NanoSVG, RTAudio,
  RTMidi, Steinberg VST3 SDK (see iPlug2 `LICENSE.txt` and dependency folders).
- **Oswald Bold** (`resources/fonts/Oswald-Bold.ttf`) — product wordmark and section headings.
  SIL Open Font Licence 1.1, Copyright 2016 The Oswald Project Authors
  (https://github.com/googlefonts/OswaldFont). The file is the static Bold from
  `ofl/oswald/static/Oswald-Bold.ttf` at
  https://github.com/google/fonts/blob/e6298cc84f59/ofl/oswald/static/Oswald-Bold.ttf
  (google/fonts main no longer carries that `static/` directory). `resources/fonts/OFL.txt`
  sits beside the TTF. One face is registered, under the font id `Oswald`. Embedded in the
  plugin and standalone binaries.
- **Bangers** (`resources/fonts/Bangers-Regular.ttf`) — retained in the resource table. Headings
  use Oswald. SIL Open Font Licence 1.1, Copyright the Bangers project authors
  (github.com/googlefonts/bangers). Embedded in the plugin and standalone binaries.
- **Space Mono** (`resources/fonts/SpaceMono-Regular.ttf`, `SpaceMono-Bold.ttf`) — parameter
  labels, list rows, numeric readouts. SIL Open Font Licence 1.1, Copyright the Space Mono
  project authors (github.com/googlefonts/spacemono). Embedded in the plugin and standalone
  binaries.
- **Tape/analog DSP** (`src/fx/TapeAnalog.h`) — original Keith implementation. Algorithm ideas
  seeded from the Airwindows open-source library (MIT Licence, Copyright (c) 2018 Chris Johnson):
  modules **Tape**, **ToTape7**, **Flutter**, **Flutter2**, **Noise**, and **TapeDust**. No
  Airwindows source code is included in this repository. See `docs/ENGINE-SCOPE.md` §2.2.

Do not ingest GPL synth sources (Surge, OB-Xf), Freakshow code, or non-permissive sample
content — see `BRIEF.md`.
