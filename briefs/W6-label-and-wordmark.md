# W6 — the labels the layout shortened, and the wordmark

Work in `/Users/solace/YoteMachine-build`, branch `mags-rebuild-and-levels`. Read
`briefs/W5-editor-polish.md` first, it explains the title bands and the row-pitch rule.

W5 worked. Panel titles are clear, the keyboard has its own strip, and the geometry gate passes 523
checks including the new title-band and row-pitch ones. What is left is what the row-pitch rule did
to the labels once it started enforcing itself, plus one thing it did not touch at all.

Look at `design/screenshots/w5-verified.png`. That is a capture of the plugin window only, taken with
`screencapture -R` against the bounds from System Events. Use that same method. A full-screen capture
must never be committed again, because it collects whatever else is on the screen.

## 1. Labels got truncated instead of the row being made to fit

The row-pitch check now demands that a control's text fit between it and its neighbour. The layout
answered by shortening the text. Knob labels in LOADED SOUNDS read as single letters, `P`, `P`, `P`,
`KIT`. The knobs on the artwork panel read as two-letter fragments, `AB`, `SD`, `ST`, `XF`, `DET`, and
they sit on top of the painted skyline.

This is the wrong resolution of the constraint. A label reduced to one letter is not a label. Fix it
in this order:

- Every control's label must be a declared name. Add an abbreviation table beside the geometry, with
  real short forms like `LVL`, `PAN`, `TUNE`, `DECAY`, `PITCH`, and a check that every drawn label
  either appears in that table or is long enough to read on its own. A single character fails the
  gate unless it is in the table, so a future squeeze cannot silently do this again.
- Widen or restack the rows that need it. LOADED SOUNDS has eight controls in one row where the panel
  carries four pairs, so it should read as two rows of four.
- Get the knobs off the artwork. That panel holds ten controls sitting on the painted city, which is
  why their labels have nowhere to go. The art panel keeps the art, and its controls move into a
  strip with a title band of its own.

## 2. The wordmark is still drawn twice

W5 was meant to stop this and did not. In the capture it still shows two copies offset by a few
pixels, one light and one pink, so the strokes ghost. Find both sources. One is painted into
`resources/images/chassis.png` by `tools/render_faces.py`. The other is drawn live somewhere in
`src/editor/`. Delete the live one, re-render the face, and confirm the capture shows one wordmark.

## 3. An unlabelled button on the left rail

Halfway down the wood rail there is a white rounded button with a green edge and no text on it,
sitting where a `BYPASS` label appears above it. It reads as an unfinished control. Give it its label
or take it off the surface. Its parameter stays reachable either way.

## 4. The amp faders read as blank slabs

In AMP the four envelope faders draw as tall dark rectangles with their values far below and no label
visible between, so the eye sees an empty bar and a stranded number. Either label each fader at the
top of its own column, or make them knobs like the rest of the panel. Whichever you pick, the capture
must plainly show what each control is.

## How to verify

```
export PATH="$HOME/cmakebin:$PATH"
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
python3 tools/fit_budget.py
~/.venvs/keith-measure/bin/python tools/render_faces.py
open build/out/Release/MagsSynth.app
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
```

Take the two numbers that come back, position then size, and capture exactly that rectangle with
`screencapture -x -R<x>,<y>,<w>,<h> /tmp/w6.png`. Commit the crop to
`design/screenshots/w6-editor-fit.png` and delete `/tmp/w6.png`.

Then read the crop with your own eyes and check the four items above one at a time. If a label is
still a single letter, or the wordmark still ghosts, the work is not done and no passing gate
outweighs what the image shows. That mismatch is what put us four rounds in.

## Hard constraints

- Parameter ids 0 to 110 are frozen and every parameter stays reachable from the single window.
- Do not touch `src/synth/`, `src/fx/`, `src/drums/`, `src/Keith_DSP.h`, `config.h`, or the preset
  delta table in `src/Keith.cpp`.
- Do not change the plugin identity, the canvas size, or the maximum window size.
- No new dependency.

## Done means

`./build/Release/keith-editor-tests` exits 0 with the abbreviation check added and passing,
`tools/fit_budget.py` still reports every panel above 6,000 square pixels per control, the app builds
and launches, the face renders at 3840 x 2400, and `design/screenshots/w6-editor-fit.png` shows one
wordmark, no single-letter labels, no unlabelled control, and readable amp faders. Commit on
`mags-rebuild-and-levels`. Do not push.
