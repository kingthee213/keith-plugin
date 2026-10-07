# INSTALL-WINDOWS.md — putting Keith on a Windows machine

You need Ableton Live 12 Suite and Windows 10 (22H2) or Windows 11 (22H2 or newer), 64-bit.

## 1. Unzip

Unzip the folder. It contains:

- `Keith.vst3` — the VST3 build (a folder, not a single file — that is normal for VST3)
- `INSTALL-MAC.md` if you are also installing on a Mac

## 2. Copy the plugin

Put `Keith.vst3` in the common VST3 folder:

```
C:\Program Files\Common Files\VST3\
```

This is the folder Live scans by default. Copying here needs admin rights; a user-level alternative
is `C:\Users\<you>\Documents\VST3\` if you add that folder in Live's preferences.

**No signing is needed on Windows.** An unsigned VST3 copied into the common folder loads normally.
You may see a SmartScreen warning on a zip or an installer downloaded from the internet — that is
about the download, not the plugin, and Windows lets you continue past it.

## 3. Turn it on in Live

1. Open Live 12 Suite.
2. **Preferences ▸ Plug-Ins** → make sure **Use VST3 Plug-In System Folders** is on.
3. Click **Rescan** (hold `Alt` for a deeper rescan if it does not appear).
4. The plugin appears in the browser under **Plug-Ins ▸ VST3**.

There is no AU build on Windows — Audio Units are macOS only. Use the VST3.

## 4. Playing it

Drop Keith on a MIDI track and arm it. It receives MIDI from Live like any instrument: play it from
your keyboard, or from a pad controller. Drum sounds are mapped across the keys — the drum map is in
the plugin's manual.

## 5. If it doesn't show up

- Check that `Keith.vst3` is directly inside `C:\Program Files\Common Files\VST3\` — a nested
  extra folder is the usual cause.
- Confirm Live is 64-bit (all Live 12 installs are) and that the plugin is the x64 build.
- Rescan again, then fully restart Live.
- Tell Noah which step you got to and what the screen said, word for word.

---

## Verification log (build side — filled in by the builder)

| Step | Machine (Windows version, build) | Result | Extra steps actually needed | Verified by |
|---|---|---|---|---|
| Copy to common VST3 folder | TBD | TBD | TBD | TBD |
| VST3 loads in Live 12 Suite | TBD | TBD | TBD | TBD |
| MIDI in from keyboard | TBD | TBD | TBD | TBD |
| MIDI in from pad controller + drum map | TBD | TBD | TBD | TBD |
