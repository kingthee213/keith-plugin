# W7b — finish the composition fit

Work in `/Users/solace/YoteMachine-build`, branch **`w7-composition`** (not `mags-rebuild-and-levels`).
Read `briefs/W7-composition.md` first; its "Blocker found part way through" section now carries the
owner's settled rule. Read the reference at
`/Users/solace/TYH_M1/dev/mags-modulator/inbox/mags-synth-final-ui-reference.png` and the crops in
`resources/reference/` before changing anything.

## The rule

The reference is a **style guide, not a final design**. Match it as closely as possible and lay out
whatever obviously has to differ differently. Take from it: the near-black canvas, wood only as the left
column and the thin frame, the cityscape as a hero, the hardware graphic, the panel fill colours, the
type scale, the pink accent. Panel rectangles are a guide. Where our control count does not fit, the
panel grows or changes shape. All 111 controls stay visible and nothing shrinks below the label plus
value minimum, which is 73 px of height and `MIN_W` of width.

## Where it stands

The canvas is already on the reference's 1536 x 1024 grid, the panel boxes in
`tools/generate_editor_geometry.py` are on the reference's coordinates, and `tools/compose_face.py`
composes the face from the owner's crops. The generator has its own validator and **it refuses to write
`src/editor/EditorGeometry.h` until every conflict it reports is resolved**. `src/editor/EditorGeometry.h`
is therefore still stale, and the face and the controls are being read from different truths. Making the
validator pass is the job.

Run it and you get every conflict at once:

```
python3 tools/generate_editor_geometry.py --write
```

Do not weaken the validator, do not delete a check, and do not make it skip. Fix the placements.

## What is known to be wrong

- **LFO, the hard one.** The reference panel is 124 wide by 392 tall and carries four controls. Ours
  needs eight plus a label row per block. A full knob is 73 px tall, so two stacked blocks need 418 px of
  content in the 346 px available, and it cannot fit at any spacing. Lay each LFO as **one row across the
  panel**: label, then its two knobs and two dropdowns side by side. That needs about 230 px of panel
  width and about 103 px of height per LFO, so two fit in 220 px of the 346. The top row then has to
  rebalance; the oscillators and the filter can give up width for it.
- **Amp modifiers.** Eight controls at two columns of `MIN_W` need 134 px of width. That is already
  widened and resolved.
- More will surface as you go. Take them one at a time and keep re-running until the generator writes.

## Also in this pass

- **Type scale.** The owner's "smallest text is way too small" is still unaddressed. Read the reference's
  sizes and match them. At 1536 wide that is about 13 to 14 px for panel titles in caps, 11 px for knob
  labels, 12 to 13 px for values, and nothing below 10 px anywhere. The current labels are 9 to 10 px.
- **Spacing rhythm.** `PAD` and `ROW_GAP` are already 18 and 14 in the generator. Use them everywhere
  instead of per-placement numbers, and make the LFO and modifier strips follow the same rhythm as the
  rest so the gaps read as one system.
- **Rectangles placed outside the geometry table.** The preset selector and the waveform strips are
  placed with hand-built rectangles, which is why the gate cannot see them. Put them in the table as
  overlays and make `tools/editor_geometry_test.cpp` check them, so the next one is caught.

## Verify

```
export PATH="$HOME/cmakebin:$PATH"
python3 tools/generate_editor_geometry.py --write
~/.venvs/keith-measure/bin/python tools/compose_face.py
~/.venvs/keith-measure/bin/python tools/render_faces.py     # only if something still calls it
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
python3 tools/editor_surface_check.py
python3 tools/fit_budget.py
open build/out/Release/MagsSynth.app
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
```

Capture the window with `screencapture -x -R<x>,<y>,<w>,<h>` from the bounds that come back, then build a
side by side of the reference and the capture at the same size and look at it. Write it to
`design/screenshots/w7b-vs-reference.png` and list in your reply what still differs.

Looking is the acceptance test. Seven rounds of passing gates produced a build the owner called a mess,
and every real defect was found by looking at the window.

## Done means

`python3 tools/generate_editor_geometry.py --write` exits 0 and prints nothing but the generated blocks,
`src/editor/EditorGeometry.h` and `ControlLayout.h` are updated, `keith-editor-tests` exits 0 with the
overlay checks passing, `editor_surface_check.py` is 17 of 17, `fit_budget.py` reports no overflow, the
app builds and launches, and the side by side exists. Commit on `w7-composition`. Do not push.
