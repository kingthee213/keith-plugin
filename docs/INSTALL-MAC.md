# INSTALL-MAC.md — putting Keith on your Mac

**This build is not signed or notarised by Apple.** That is deliberate: it would need a paid Apple
Developer account, and this plugin is not being sold. Everything below is the normal, expected
consequence of an unsigned plugin — nothing here is broken.

You need Ableton Live 12 Suite and macOS 11.7.10 (Big Sur) or newer.

## 1. Unzip

Unzip the folder somewhere you can find it. It contains:

- `Keith.vst3` — the VST3 build
- `Keith.component` — the Audio Unit (AU) build
- this note

## 2. Put the plugin files in place

Either `Library` folder works. The user-level one does not need your admin password, so use it
unless you have a reason not to.

- VST3 → `~/Library/Audio/Plug-Ins/VST3/`
- AU → `~/Library/Audio/Plug-Ins/Components/`

(In Finder: hold `Option` and use Go ▸ Library to see your hidden `Library` folder. The system-wide
alternatives are `/Library/Audio/Plug-Ins/VST3/` and `/Library/Audio/Plug-Ins/Components/`.)

## 3. Clear the quarantine flag — this is the step people miss

When a plugin arrives from the internet, macOS tags it as quarantined. Gatekeeper then blocks it and
reports **"damaged and can't be opened"** or that it can't be verified. The file is fine; the tag is
the problem.

Open Terminal (Applications ▸ Utilities ▸ Terminal) and paste these two lines:

```
xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/VST3/Keith.vst3
xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/Components/Keith.component
```

Nothing printed back means it worked. (If you copied the plugins to the system-wide `Library`
folders instead, swap in `/Library/Audio/Plug-Ins/...`.)

## 4. If macOS still asks

Open **System Settings ▸ Privacy & Security**. If you see a message about a blocked item from the
developer, click **Allow Anyway**. Do this, then continue — do not click "Move to Trash".

## 5. Rescan in Live

1. Open Live 12 Suite.
2. **Preferences ▸ Plug-Ins** → make sure **Use VST3 Plug-In System Folders** is on (and
   **Use Audio Units** if you are using the AU) → turn **Use Audio Units**/**VST3** on if they are
   off.
3. Hold `Option` and click **Rescan** for a deep rescan, which is the one that picks up new
   plugins.
4. In the browser: VST3 lives under **Plug-Ins ▸ VST3**, the AU under **Plug-Ins ▸ Audio Units**.
   Both are the same plugin — use whichever you prefer, they sound identical.

## 6. Playing it

Drop Keith on a MIDI track and arm it. It receives MIDI from Live like any instrument: play it from
your keyboard, or from a pad controller. Drum sounds are mapped across the keys — the drum map is in
the plugin's manual.

## 7. If it doesn't show up

- Confirm the plugin file is in the folder from step 2, not sitting in Downloads.
- Re-run the `xattr` line for that file, then fully quit and reopen Live.
- Check that the build matches your Mac: this is a universal build (Apple silicon + Intel), so it
  runs on both.
- Tell Noah which step you got to and what the screen said, word for word.

---

## Verification log (build side — filled in by the builder, not the friend)

This note is only done when it has been walked end to end on a real Mac.

| Step | Machine (model, chip, macOS version) | Result | Extra steps actually needed | Verified by |
|---|---|---|---|---|
| Unzip + copy | TBD | TBD | TBD | TBD |
| xattr clear | TBD | TBD | TBD | TBD |
| AU loads in Live 12 Suite | TBD | TBD | TBD | TBD |
| VST3 loads in Live 12 Suite | TBD | TBD | TBD | TBD |
| MIDI in from keyboard | TBD | TBD | TBD | TBD |
| MIDI in from pad controller + drum map | TBD | TBD | TBD | TBD |
