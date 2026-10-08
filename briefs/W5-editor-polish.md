# W5 — make the editor look finished

Work in `/Users/solace/YoteMachine-build`, branch `mags-rebuild-and-levels`. Read `briefs/W4-editor-fit.md`
first, it explains the control geometry.

W4 fixed the numbers. `tools/fit_budget.py` now reports every panel above 6,000 square pixels per
control, and the geometry test passes 520 checks. The problems left are the ones a rectangle check
cannot see, and they are all visible in `design/screenshots/w4-editor-fit.png`. Look at that
screenshot before changing anything. Six defects, in the order they hurt a demo.

## 1. The wordmark is drawn twice

Top left of the window, MAGS SYNTH appears twice at a slight offset, so the letters ghost. One copy
is painted into `resources/images/chassis.png` by `tools/render_faces.py`. Something is also drawing
it live. Exactly one thing may own it. The face owns it, so the live one goes.

## 2. Panel titles are covered by controls

In OSCILLATORS, LOADED SOUNDS, VINTAGE ANALOG, GLOBAL and EFFECTS, the panel title sits at the top
left of the panel and controls are placed on top of it. In OSCILLATORS the title runs into a `MIX`
label and a value. In VINTAGE ANALOG and GLOBAL a button row covers it. In EFFECTS `MASTER` sits over
it.

Fix it structurally, not by nudging. Each panel gets a reserved title band, declared next to the
panel boxes, and every control rectangle must clear it. Add that as a check in
`tools/editor_geometry_test.cpp` so it cannot regress, and then move the offending controls down.
`tools/generate_editor_geometry.py` is where placements come from, and it should honour the band
rather than having the offsets hand-corrected.

## 3. LOADED SOUNDS has a tab row over its title

Two small tabs sit on top of the panel title in that panel. They belong below the title band, in
their own row.

## 4. The amp modifier row is unreadable

The row of envelope modifiers reads as one run of truncated words, `N KEY A KEY A ADV KEY A D KEY D
BEND`, and beneath it a row of values `0.0 0.0 0.0 0.0` with nothing to tell them apart.

The existing check verifies each control's text fits its own rectangle. It does not verify that two
neighbours' text fits the *pitch* between them, which is what fails here. Add that check: for items
sharing a row, the widest of (label, value) must fit within the distance to the next item, with a
visible gap. Then either widen the row or show fewer of these modifiers at once. They are secondary
controls and losing one from the surface is acceptable as long as the parameter stays reachable.

## 5. The effects panel's top area is a mess

`MASTER` and a repeated `MASTER` collide with the title row, the knob labels sit on top of their own
values, and the yellow button column overlaps the knobs beside it. W4 folded bypass into the
algorithm menus, which is right. The panel now needs its rows spaced off the title band and the knob
grid given even pitch, using the new row-pitch check from item 4.

## 6. The keyboard is behind the bottom panels

The playable keyboard's octave is drawn underneath the bottom row of panels, so the white keys run
under GLOBAL and VINTAGE ANALOG. The keyboard belongs in the strip below the panel row. Check the
panel bottoms against the keyboard rectangle and fix whichever is wrong.

## How to verify

```
export PATH="$HOME/cmakebin:$PATH"
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
python3 tools/fit_budget.py
~/.venvs/keith-measure/bin/python tools/render_faces.py
open build/out/Release/MagsSynth.app
screencapture -x /tmp/w5.png
```

Then crop to the plugin window, commit the crop to `design/screenshots/w5-editor-fit.png`, and
delete `/tmp/w5.png`. A full-screen capture must not be committed or left on disk, because other
windows are on that screen. Ask for Accessibility permission if you need the window bounds; if you
cannot get them, crop by eye from the full capture before deleting it.

Read the crop with your own eyes and confirm all six defects are gone. Do not report done on the
strength of the checks alone. The whole reason these survived four passes is that everyone trusted a
check instead of looking.

## Hard constraints

- Parameter ids 0 to 110 are frozen and every parameter stays reachable from the single window.
- Do not touch `src/synth/`, `src/fx/`, `src/drums/`, `src/Keith_DSP.h`, `config.h`, or the preset
  delta table in `src/Keith.cpp`.
- Do not change the plugin identity or the panel boxes beyond what the title band needs.
- No new dependency.

## Done means

`./build/Release/keith-editor-tests` exits 0 with the title-band and row-pitch checks added and passing,
`tools/fit_budget.py` still reports every panel above 6,000 square pixels per control, the app builds
and launches, the face renders at 3840 x 2400, and `design/screenshots/w5-editor-fit.png` exists and
shows all six defects gone. Commit on `mags-rebuild-and-levels`. Do not push.
