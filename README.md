# Keith

Private instrument project with a working two-oscillator synth, 43 automatable parameters,
15 original presets across Family 01-05, a tape bus, and shared Windows VST3/standalone editor.
The physical/editorial UI rebuild is in progress. The complete instrument brief is not finished.

Locked product spec: [`BRIEF.md`](BRIEF.md).

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
src/                         # Keith.cpp, Keith.h, Keith_DSP.h (test voice only)
resources/                   # AU/VST3 plists, resource.h (no art this pass)
tools/bootstrap-deps         # clone iPlug2 at pinned SHA + fetch VST3 SDK
.github/workflows/build.yml
```

## Prerequisites

- **Git** (recursive submodules)
- **CMake** 3.14+
- **Windows:** Visual Studio 2022 or later with “Desktop development with C++”
- **macOS:** Xcode + Ninja (`brew install ninja`)
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

## Build — macOS (universal AUv2 + VST3)

Deployment floor **macOS 11.7.10**, universal **arm64 + x86_64**:

```bash
./tools/bootstrap-deps
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DIPLUG_DEPLOY_PLUGINS=OFF
cmake --build build --target Keith-vst3 Keith-au
```

Artefacts:

- `build/out/Keith.vst3`
- `build/out/Keith.component`

macOS builds are CI-produced; load testing in Ableton Live requires a real Mac (see
[`docs/INSTALL-MAC.md`](docs/INSTALL-MAC.md)).

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
exe; if you move the exe alone, copy the four TTFs with it or the editor draws
no text.

## Current engine and editor

Source, Envelopes, Tape and Library pages share a permanent filter/voice channel. The editor uses
original baked Cycles materials and native parameter controls. See [UI design](docs/UI-DESIGN.md).
Factory ROMs, drum sampling, the modulation matrix and insert effects remain outside this slice.

```powershell
cmake --build build --config Release --target keith-synth-tests keith-editor-tests
ctest --test-dir build -C Release --output-on-failure
python tools/editor_surface_check.py
```

Rendering the authored material assets is optional for a normal build. PNGs are checked into the
asset set and embedded in Windows binaries. To rebuild them with Blender 5.x:

```powershell
blender --background --factory-startup --python tools/render_materials.py
```

## CI

`.github/workflows/build.yml` matrix:

| Runner | Target |
|---|---|
| `macos-14` | Apple Silicon — universal AU + VST3 |
| `macos-15-intel` | Intel Mac — universal AU + VST3 (`macos-13` retired; see BRIEF [A2]) |
| `windows-latest` | Windows x64 VST3 |

## Licence notes

- **Keith** plugin code: private handover; not published.
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
