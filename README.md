# Keith

Working-name instrument plugin (private handover). This repo is the **U1 skeleton**: iPlug2
build system, Windows VST3, macOS universal AUv2 + VST3 targets, and a minimal test voice
(sine oscillator → gain) so builds are auditable. Engine, FX, UI, and content land in later
units.

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
(one octave), **Z / X** shift the octave. Keep `Roboto-Regular.ttf` next to the
exe, or the editor draws no text.

## Test voice (pre-engine)

Nine automatable parameters: **Gain** plus the global tape bus (saturation, head bump, wow,
flutter, hiss, print-through, mix, bypass). Polyphonic sine oscillator → smoothed gain → tape
bus → stereo out, with an editor that binds to those nine parameters and an on-screen keyboard.
Replaced by the real engine in a later unit — no banks, drum map or preset browser yet.

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
- **Roboto Regular** (`resources/fonts/Roboto-Regular.ttf`) — editor UI font, Apache License 2.0,
  Copyright the Roboto project authors. Shipped with the plugin bundle and the standalone.
- **Tape/analog DSP** (`src/fx/TapeAnalog.h`) — original Keith implementation. Algorithm ideas
  seeded from the Airwindows open-source library (MIT Licence, Copyright (c) 2018 Chris Johnson):
  modules **Tape**, **ToTape7**, **Flutter**, **Flutter2**, **Noise**, and **TapeDust**. No
  Airwindows source code is included in this repository. See `docs/ENGINE-SCOPE.md` §2.2.

Do not ingest GPL synth sources (Surge, OB-Xf), Freakshow code, or non-permissive sample
content — see `BRIEF.md`.
