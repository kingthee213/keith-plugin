# Build MAGS SYNTH on your Mac (local)

**Note:** This guide was written before the first Mac run — treat step **c** as a test and send Sam any errors.

Build Keith/MAGS SYNTH locally so you can hear every edit without waiting on GitHub Actions.

---

## a. One-time toolchain

**Why:** macOS needs Apple’s compiler tools before anything can compile.

```bash
xcode-select --install
```

**Why:** The full **Xcode** app (App Store) includes `xcodebuild`, which this build needs — not just the small Command Line Tools. Install Xcode, open it once, then accept the license:

```bash
sudo xcodebuild -license accept
```

**Why:** Tell macOS to use the full Xcode app (not only Command Line Tools).

```bash
sudo xcode-select -s /Applications/Xcode.app
```

**Why:** **Homebrew** installs build tools with simple commands (`brew install …`).

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Why:** **CMake** drives the build; **Git** fetches the repo and iPlug2; **Python 3** is for optional quality gates (numpy via a venv — after you clone, `tools/setup-tools-venv.sh` creates `~/.venvs/keith-measure` if you need it).

```bash
brew install cmake git python3
```

---

## b. Get the source

**Why:** Download the project and open the `w7-composition` branch (current MAGS SYNTH work).

```bash
git clone NEEDS-REPO-URL keith-plugin && cd keith-plugin && git checkout w7-composition
```

---

## c. First full build

**Why:** One script downloads **iPlug2** + **Eigen**, configures, builds VST3, AU, and the standalone, and signs the bundles.

First run: **~10–20 minutes**. Later runs are much faster.

```bash
./tools/build-macos.sh .
```

**Harmless noise:** `sudo: a password is required` and `fatal: destination path '…' already exists` during SDK download — CLAP/AAX/VST2, which this project skips. Ignore if you see “Build complete.”

---

## d. Fast loop after the first build

From the `keith-plugin` folder, after each edit:

**Why:** Incremental rebuild (faster than the full script).

```bash
cmake --build build --config Release --target MagsSynth-vst3 MagsSynth-au MagsSynth-app
```

**Why:** **Ad-hoc codesign** so the DAW will load the plugin; copy into your user plug-in folders.

```bash
for n in MagsSynth.vst3 MagsSynth.component MagsSynth.app; do p="$(find build/out -type d -name "$n" -maxdepth 3 -print -quit)"; [[ -n "$p" ]] && codesign --force --deep --sign - "$p"; done && mkdir -p ~/Library/Audio/Plug-Ins/VST3 ~/Library/Audio/Plug-Ins/Components && cp -R "$(find build/out -type d -name MagsSynth.vst3 -maxdepth 3 -print -quit)" ~/Library/Audio/Plug-Ins/VST3/ && cp -R "$(find build/out -type d -name MagsSynth.component -maxdepth 3 -print -quit)" ~/Library/Audio/Plug-Ins/Components/
```

**Why:** Reload the new build — DAW: restart the session or **Rescan plug-ins**; standalone: quit, then open again.

```bash
open "$(find build/out -type d -name MagsSynth.app -maxdepth 3 -print -quit)"
```

**Just want to hear it:** open the standalone (no DAW):

```bash
open build/out/Release/MagsSynth.app
```

---

## e. Optional: offline tests

Not required to hear the plugin. `keith-synth-tests` = synth + preset-restore + recreation gate; `keith-editor-tests` = editor layout geometry.

**Why:** Build the test programs.

```bash
cmake --build build --config Release --target keith-synth-tests keith-editor-tests
```

**Why:** Run synth tests (recreation gate needs `tools/setup-tools-venv.sh` → `~/.venvs/keith-measure` with numpy/scipy/pillow).

```bash
./build/out/Release/keith-synth-tests
```

**Why:** Run editor layout tests.

```bash
./build/out/Release/keith-editor-tests
```

---

## f. Troubleshooting

| Problem | What to try |
|--------|-------------|
| `xcodebuild` not found | Install Xcode, open once, run `sudo xcode-select -s /Applications/Xcode.app` (**a**). |
| `cmake` not found | `brew install cmake`; open a new Terminal. |
| Plugin not in DAW | Sign + copy (**d**); rescan; use VST3 or AU as your DAW expects. |
| DAW won’t load plugin | Sign (**d**); clear quarantine: `xattr -dr com.apple.quarantine build/out`, then sign + copy again. |
| Build fails after `git pull` | `rm -rf build && ./tools/build-macos.sh .` |
| Send Sam the output | Copy the **last 30 lines** from Terminal into a message. |
