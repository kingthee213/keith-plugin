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

## The single-window layout does not fit

This is the most important open item, and it was found by looking at a real screenshot of the
running editor rather than at the source.

Every control has a rectangle, the rectangles do not overlap, and each label fits its own box. All of
that is true and gated. What the gate cannot see is the text the plugin draws at runtime, which is
the parameter name plus its value, and that text is wider than the box the label check measured.

Observed in the running standalone at 1280x800:

- The OSCILLATORS panel carries two oscillators plus the sub, noise and FM mixer in 198 design
  pixels, and the value readouts collide near the bottom.
- The AMP panel packs nine envelope modifiers into a secondary row, and their names run into each
  other.
- The LFO panel's two dropdowns and their values spill past the panel edge into MOD MATRIX.
- The EFFECTS and GLOBAL panels have knob values that overlap their own labels.
- The LOADED SOUNDS panel is mostly empty space with the controls crowded along the bottom, so the
  area is available but not where it is needed.

The layout is not wrong in principle. It is over-packed in the panels that carry the most controls
and under-used in the one that carries the fewest. The fix is to move area from LOADED SOUNDS and
DRUM PADS to OSCILLATORS, AMP and LFO, then re-check. If that is not enough, the design canvas has
to grow beyond 1280x800, which is a one-way decision because the face render scale follows it.

Until it is fixed, the honest description of the editor is: one window, every control present, and
several of them unreadable at the default size.

## A live screenshot of the editor cannot be taken from an agent session

Screen Recording permission is granted now, so `screencapture` works. Accessibility permission is
not, so a capture cannot be scoped to one window. A full-screen capture therefore pulls in whatever
else is on the screen, which on this machine included another agent session's transcript. Do not
take full-screen captures here. Either grant Accessibility so the window bounds can be read, or
clear the screen first, or capture from a human session.

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
