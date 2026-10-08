# Open items

What is deliberately not done, with the reason and the first step. Nothing here is a surprise
nobody noticed; each one was found by a check or by inspection.

## The on-disk bundle is still named `Keith`

A host shows **MAGS SYNTH** and that is verified. The file on disk is `Keith.vst3` /
`Keith.component` / `Keith.app`, because those names come from the CMake target names.

Renaming is not just an output rename. `CMakeLists.txt` hardcodes the Windows staging path
`${CMAKE_BINARY_DIR}/out/Keith.vst3/Contents/Resources`, and no Windows toolchain was available
to test a change there. The step, when a Windows machine is in reach:

1. Set `OUTPUT_NAME MagsSynth` on `Keith-vst3`, `Keith-au` and `Keith-app` in `CMakeLists.txt`.
2. Update `CFBundleExecutable` in the three `resources/*-Info.plist` files to `MagsSynth`.
3. Update the Windows staging path in `CMakeLists.txt` to match.
4. Update `tools/build-macos.sh`, `tools/verify_plugin_identity.py` and both install notes.
5. Rebuild, re-run every gate, then confirm Live still finds it.

Do **not** change `PLUG_UNIQUE_ID` or `PLUG_MFR_ID` while doing it. Those are what a saved set
stores. The file name is cosmetic; the identity is not.

## Windows was not rebuilt after the identity change

`resources/main.rc` changed with the identity. Nothing on this machine can build or load a
Windows VST3. The Windows build needs a pass on the machine that has the toolchain.

## Fourteen presets still sit below the loudness target

They peak at the -1.5 dBFS ceiling and land between -15.1 and -19.9 LUFS instead of -12. They
are all short percussive patches: stabs, plucks, arps and kits, with one hard transient and very
little sustain. A higher level would clip them.

This is not a level problem, it is a patch problem. The fix is in the patch: less drive into the
tape and master stages, or a faster transient, so the peak and the body sit closer together.
The named list is `docs/level-measurements-after.tsv`, marked by a peak at -1.50.

## The face is a first pass

Wood, panels and the skyline read as painted. The wordmark reads as chunky novelty lettering
rather than spray script, and the two olive panels are greyer than the reference. Neither can be
judged properly without the owner looking at the running plugin, and the art is explicitly his to
approve.

## A live screenshot cannot be taken from an agent session

macOS denies screen capture to a terminal-launched process unless Screen Recording is granted by
hand in System Settings, Privacy and Security. Until then, editor review needs a human session or
a real screen capture.

## Content still to come

- The 300-preset library and its calibrated spectral gate: `docs/V2-LIBRARY-PLAN.md`.
- The wavetable question in `docs/V2-LIBRARY-PLAN.md` is now simpler: GPL-3.0 reuse is approved
  (`docs/DECISIONS.md` D3), so the wavetable oscillator can be borrowed from Surge rather than
  rebuilt. The owner still has to say yes to shipping derived wavetable data.
- One-shot import for the drum pads. Sanctioned by the D2 retraction, not yet built.
- Every capability in `docs/SURGE-GAP-ANALYSIS.md` marked `borrow`, ranked with its cost.

## Verify on the owner's host

Keith runs Ableton Live 12. This machine verified Live 11.3.43, whose plugin scanner accepted the
plugin as MAGS SYNTH. Live 12 has not been exercised, and no DAW has had a MIDI keyboard play the
drum map.
