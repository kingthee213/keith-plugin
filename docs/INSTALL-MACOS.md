# Yote Machine — macOS build & demo instructions

**The Youth Hostel DSP — Yote Machine.** A synthesizer instrument (standalone app, VST3 and AU).

You have the source. It builds on your Mac with one command. Read "Read this first" honestly — this
build has not been run on a Mac yet, so treat the first run as a test.

## Read this first

This package builds from source. It has **not** been built or tested on macOS yet — the machine that
prepared it was a Windows box, and no Mac was reachable to verify against. The build script is written
to be careful (it checks your toolchain, matches macOS's architecture requirements, and signs the
result), but if something fails on the first run, send the output back rather than fighting it.

## 1. Install the tools (about 10 minutes, once)

You need Apple's developer tools and CMake. Open **Terminal** (in Applications → Utilities) and run:

```bash
xcode-select --install
```

A dialog appears — click Install and let it finish. Then install the full Xcode from the **App Store**
(this is needed for the build tools, not to write code), open it once, and run:

```bash
sudo xcode-select -s /Applications/Xcode.app
```

Install CMake — easiest is [Homebrew](https://brew.sh) (`brew install cmake`), or the installer from
[cmake.org/download](https://cmake.org/download/).

## 2. Build it (about 10 minutes, mostly waiting)

In Terminal, go to this folder and run:

```bash
cd /path/to/keith-editorial
./tools/build-macos.sh
```

That fetches the framework, builds, signs the result, and prints where everything landed. The last
lines tell you exactly which files you now have.

## 3. Demo it — the fastest way

**You do not need a DAW.** It builds a standalone application:

```bash
open build/out/Keith.app
```

It behaves like a small instrument — pick a preset, play with the on-screen keyboard or a connected
MIDI keyboard. This is the quickest way to hear it.

## 4. Using it in a DAW (optional)

Copy the plugin into your plug-in folders:

```bash
mkdir -p ~/Library/Audio/Plug-Ins/VST3 ~/Library/Audio/Plug-Ins/Components
cp -R build/out/Keith.vst3      ~/Library/Audio/Plug-Ins/VST3/
cp -R build/out/Keith.component ~/Library/Audio/Plug-Ins/Components/
```

- **VST3** works in Ableton, Reaper, Bitwig, Studio One and others.
- **AU** is what Logic and GarageBand want.
- Rescan plugins in your DAW's preferences if it does not appear the first time.

## If macOS blocks it

Because this came from another computer, macOS may quarantine it. If it refuses to open or a DAW will
not load it, clear the flag:

```bash
xattr -dr com.apple.quarantine build/out/Keith.app
xattr -dr com.apple.quarantine build/out/Keith.vst3
xattr -dr com.apple.quarantine build/out/Keith.component
```

The build script signs everything ad-hoc, which is what Apple Silicon Macs need in order to load an
unsigned plug-in. If a DAW still refuses it, that is a signing question, not a broken build — mention
it and we will sort it.

## What is inside

- 109 factory presets, organised by the hardware family each one is modelled on.
- A synth engine, drum pads, effects and a modulation matrix.
- An interactive interface.

## Notes

- Requires macOS 11.7 or newer. Builds for both Apple Silicon and Intel.
- Nothing here phones home, requires an account, or installs anything beyond the plug-in itself.
