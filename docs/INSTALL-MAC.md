# INSTALL-MAC.md: putting Keith on your Mac

**The macOS binary has not been built. It has not been verified on a Mac.** Nothing in this file is a report of a successful Mac build, an AU validation, or a host load. The install steps below are what to do once a binary exists. The build steps are the procedure to run on a Mac. They have not been executed.

**This build is not signed or notarised by Apple.** That is deliberate: it would need a paid Apple Developer account, and this plugin is not being sold. Gatekeeper treats an unsigned plugin downloaded from the internet as quarantined.

You need Ableton Live 12 Suite and macOS 11.7.10 (Big Sur) or newer.

## Build from source

These commands have not been run. They are the steps a Mac should run from a checkout of this repo.

Requirements: git, CMake 3.14 or newer, and Xcode. `tools/bootstrap-deps` clones iPlug2 at the pinned revision in `README.md` and downloads the plugin SDKs. `tools/build-macos.sh` calls that script, then configures with the Xcode generator and builds three targets.

```bash
./tools/build-macos.sh
```

The script asks for deployment target **11.7.10** and a universal binary (`arm64` and `x86_64`). Whether the compiler honoured that is unchecked, because this tree has not been configured on macOS.

Expected products, printed by the script if the build finishes:

- `build/out/MagsSynth.vst3`
- `build/out/MagsSynth.component`
- `build/out/Keith.app`

Xcode sometimes nests those under `build/out/Release/`. The script lists the directories it actually finds.

The AU identity compiled into `config.h` and `resources/Keith-AU-Info.plist` is:

| Field | Code |
|---|---|
| Type | `aumu` |
| Subtype (`PLUG_UNIQUE_ID`) | `Kth1` |
| Manufacturer (`PLUG_MFR_ID`) | `Prvt` |

`auval -v aumu Kth1 Prvt` is the command that would look the AU up. It has not been run.

Editor fonts are copied into each bundle's `Contents/Resources`, which is where `LoadFont` looks on macOS. The same four TTFs and `OFL.txt` are also copied beside `Keith.app`. That copy has not been confirmed on a Mac.

## 1. Unzip

Unzip the folder somewhere you can find it. A finished Mac package would contain:

- `Keith.vst3`
- `MagsSynth.component`
- this note

That package does not exist yet.

## 2. Put the plugin files in place

Either `Library` folder works. The user-level one does not need your admin password, so use it unless you have a reason not to.

- VST3 → `~/Library/Audio/Plug-Ins/VST3/`
- AU → `~/Library/Audio/Plug-Ins/Components/`

(In Finder: hold `Option` and use Go ▸ Library to see your hidden `Library` folder. The system-wide alternatives are `/Library/Audio/Plug-Ins/VST3/` and `/Library/Audio/Plug-Ins/Components/`.)

## 3. Clear the quarantine flag. This is the step people miss.

When a plugin arrives from the internet, macOS tags it as quarantined. Gatekeeper then blocks it and reports **"damaged and can't be opened"** or that it can't be verified. The file is fine; the tag is the problem.

Open Terminal (Applications ▸ Utilities ▸ Terminal) and paste these two lines:

```
xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/VST3/Keith.vst3
xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/Components/MagsSynth.component
```

Nothing printed back means it worked. (If you copied the plugins to the system-wide `Library` folders instead, swap in `/Library/Audio/Plug-Ins/...`.)

## 4. If macOS still asks

Open **System Settings ▸ Privacy & Security**. If you see a message about a blocked item from the developer, click **Allow Anyway**. Then continue. Leave the file in place. Do not click "Move to Trash".

## 5. Rescan in Live

1. Open Live 12 Suite.
2. **Preferences ▸ Plug-Ins** → make sure **Use VST3 Plug-In System Folders** is on (and **Use Audio Units** if you are using the AU) → turn **Use Audio Units**/**VST3** on if they are off.
3. Hold `Option` and click **Rescan** for a deep rescan, which is the one that picks up new plugins.
4. In the browser: VST3 lives under **Plug-Ins ▸ VST3**, the AU under **Plug-Ins ▸ Audio Units**.

Live has not loaded this plugin. The steps above are the ones to try when a binary exists.

## 6. Playing it

Drop Keith on a MIDI track and arm it. It receives MIDI from Live like any instrument: play it from your keyboard, or from a pad controller. Drum sounds are mapped across MIDI notes 36-51. The drum map is in `docs/DRUM-MAP.md`. The hits are synthesized in the plugin. They are not samples.

## 7. If it doesn't show up

- Confirm the plugin file is in the folder from step 2, not sitting in Downloads.
- Re-run the `xattr` line for that file, then fully quit and reopen Live.
- The build script requests a universal binary. That request has not been verified, so a missing plugin is not evidence about which chip it runs on.
- Tell Noah which step you got to and what the screen said, word for word.

---

## Verification log (build side)

No row below has been filled. There has been no Mac build and no Mac install.

| Step | Machine (model, chip, macOS version) | Result | Extra steps actually needed | Verified by |
|---|---|---|---|---|
| Build with `tools/build-macos.sh` | not run | not built | | |
| Unzip + copy | not run | not built | | |
| xattr clear | not run | not built | | |
| AU loads in Live 12 Suite | not run | not built | | |
| VST3 loads in Live 12 Suite | not run | not built | | |
| MIDI in from keyboard | not run | not built | | |
| MIDI in from pad controller + drum map | not run | not built | | |
