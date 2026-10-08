# W7 — rebuild the composition to match the owner's reference

## Blocker found part way through, read this first

The reference's own panel rectangles are too small for our control count. Measured on its coordinates:

- **Amp modifiers.** Eight controls need two columns of the 42 px minimum plus padding and a gap, which
  is 134 px of width. The reference grid leaves 72 px there. It has been widened to 134, taking 68 px
  from the sampled-sounds panel, and that conflict is resolved.
- **LFO.** The reference panel is 124 by 392 and holds four controls. Ours needs eight, plus a label row
  per block. A full-size knob needs 73 px of height, which is 34 px of body plus 19 px of label plus
  20 px of value. Two LFO blocks need 418 px of content in the 346 px available. It does not fit, and no
  amount of re-spacing makes it fit.
- The same shape of problem sits wherever we show more than the reference does. GLOBAL carries nine
  controls where the reference shows three. EFFECTS carries twelve where the reference shows four.

So the composition cannot match the reference exactly **and** keep all 111 controls visible at the
reference's control size. Three ways out, and it is a product call rather than a technical one.

1. Let the busy panels be larger than the reference's rectangles, keeping every control visible. The
   panel grid then stops matching the reference in those places, while materials, colours, the hero and
   the overall proportions still do.
2. Keep the reference's grid and put the extra controls behind per-section tabs, LFO 1 and LFO 2 for
   instance, or insert A, B and master. Some controls then stop being visible at once, which cuts
   against the owner's instruction that everything be presented at once.
3. Keep the grid and shrink those panels' controls below the label-plus-value minimum. That is the
   defect this whole line of work started from, so it is not a real option.

Do not start this round until that is settled.


Work in `/Users/solace/YoteMachine-build`, branch `mags-rebuild-and-levels`.

The owner's feedback on the current build, in his words: the preset selector is entirely covered and it is
not the only element like that, the smallest text is too small, the spacing is terrible, every portion is
being hyperfocused on without considering the overall composition, the whole background is wood when the
reference only had wood edges, and the art is far off now that it is not a hero.

He is right on every point. Six rounds of work went into individual controls and none into the whole.
This round is composition first.

## The reference is the spec

`/Users/solace/TYH_M1/dev/mags-modulator/inbox/mags-synth-final-ui-reference.png` is 1536 x 1024. It is the
target, and every coordinate below is in its pixels. Read it before writing code.

Assets already extracted from it into `resources/reference/`, at these exact rectangles.

| file | crop | what it is |
| --- | --- | --- |
| `left-column.png` | 0, 0, 246, 1024 | the wood rail, with the MAGS SYNTH plaque, the icons and the tagline already on it |
| `wood-frame-top.png` | 0, 0, 1536, 16 | the top wood edge |
| `wood-frame-right.png` | 1496, 0, 1536, 1024 | the right wood edge |
| `wood-frame-bottom.png` | 0, 1006, 1536, 1024 | the bottom wood edge |
| `hero-city.png` | 258, 430, 808, 732 | the cityscape, the hero of the whole thing |
| `vintage-rig.png` | 262, 800, 465, 908 | the hardware graphic in the vintage panel |

## What is wrong now, precisely

- The face fills the entire canvas with wood. In the reference the canvas behind the panels is near black
  and wood appears only as the left column and a thin frame on the top, right and bottom. Use `(10, 11, 13)`
  for the canvas.
- The cityscape is an 80 px column. In the reference it is 550 x 302 and it is the visual anchor.
- The design canvas is 1280 x 800, which is 8:5. The reference is 3:2. Every coordinate has been scaled
  and re-derived by hand, which is where the drift came from.

## The work

### 1. Put the canvas on the reference's grid

Set the design canvas to **1536 x 1024** so reference coordinates map one to one with no scaling. Render the
face at **2x, so 3072 x 2048**, and set the maximum editor scale to 2.0 so the face is never upscaled.
Editor opens at scale 1.0.

`EditorGeometry.h` holds the canvas constants, `config.h` holds the editor size, and
`tools/generate_editor_geometry.py` holds the panel boxes. All three move to the new grid together.

### 2. Compose the face from the reference, not from code

Rewrite the composition half of `tools/render_faces.py` to assemble the reference layout.

- Fill the canvas with the dark background.
- Paste `left-column.png` at 0, 0.
- Paste the three frame strips at their edges.
- Draw each panel as a rectangle with a 2 px darker border and the fill sampled from the reference.
- Paste `hero-city.png` at 258, 430 with a 2 px border.
- Paste `vintage-rig.png` at 262, 800.
- Draw the panel titles in the reference's style, and nothing else.

Keep the existing procedural panel drawing only if it still renders something the reference has. Anything it
invents, including the cityscape it currently draws from scratch, goes. The hand-painted look now comes from
the owner's own artwork.

Panel rectangles and fills, read off the reference.

| panel | rect | fill |
| --- | --- | --- |
| OSCILLATORS | 250, 24, 654, 416 | 16, 29, 34 |
| FILTER | 664, 24, 896, 416 | 37, 88, 103 |
| AMP | 906, 24, 1106, 416 | 13, 28, 34 |
| LFO | 1116, 24, 1240, 416 | 4, 11, 14 |
| MOD MATRIX | 1250, 24, 1500, 416 | 11, 29, 36 |
| sampled sounds block | 816, 426, 1256, 736 | 9, 24, 30 |
| DRUM PADS | 1264, 426, 1500, 736 | 11, 22, 26 |
| VINTAGE ANALOG MODELING | 256, 756, 730, 996 | 73, 77, 43 |
| GLOBAL | 744, 756, 1066, 996 | 19, 29, 26 |
| EFFECTS | 1080, 756, 1500, 996 | 19, 29, 26 |

The cityscape occupies the middle-left with no panel frame around it, bordered only.

### 3. Fix the type scale and the spacing

Read the reference's text sizes off the image and match them. As a guide at 1536 wide: panel titles about
13 to 14 px in caps with letter spacing, knob labels about 11 px, values about 12 to 13 px, and nothing
smaller than 10 px anywhere. The current build's smallest text is 9 px and its labels are 9 to 10 px, which
is the "way too small" he is describing.

The reference has generous, even padding. Roughly 18 to 20 px inside a panel edge, and about 14 px between
neighbouring controls. Its spacing reads as a rhythm because the gaps are all the same size. Set the panel
padding and the control gap as named constants in the generator and use them everywhere, so the rhythm is a
property of the code rather than of each placement.

The reference carries about sixty controls. We have 111 and every one must stay reachable, so our panels will
be tighter than his. Match his padding and his gap size, accept that our grids have more cells, and do not
shrink the text to make a cell fit. If a cell cannot hold its control at the matched type scale, the panel is
over-subscribed and something has to move out of it, which is item 4.

### 4. Fix the collisions, and stop placing controls outside the geometry table

Two known collisions, both caused by a control placed with a hand-built `IRECT` instead of a rectangle from
the geometry table, which is why the gate cannot see them:

- The preset selector at `EditorialLayout.h:132`, built as `IRECT(kPanelSample.l + 8, kPanelSample.t + 108, ...)`,
  sits inside the panel's second knob row and the knobs cover the preset name. In the reference the preset
  selector is a full-width row of its own under a waveform strip.
- The same pattern removed two decorations in the GLOBAL and LFO panels last round.

Put every control's rectangle in the geometry table, including the preset selector, the waveform strips and
any decoration that can overlap a control. Extend `tools/editor_geometry_test.cpp` so it checks those
rectangles too, which is the only way the gate can catch the next one.

### 5. Verify against the reference, not against a passing gate

```
export PATH="$HOME/cmakebin:$PATH"
python3 tools/generate_editor_geometry.py --write
~/.venvs/keith-measure/bin/python tools/render_faces.py
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
python3 tools/editor_surface_check.py
python3 tools/fit_budget.py
open build/out/Release/MagsSynth.app
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
```

Capture the window with `screencapture -x -R<x>,<y>,<w>,<h>` using the bounds that come back, then build a
side by side image of the reference and the capture at the same size and look at it. Put that side by side in
`design/screenshots/w7-vs-reference.png`. Judge materials, panel positions, the hero's size, the type scale
and the spacing rhythm against the reference, one at a time, and write down what still differs.

A passing gate is not evidence that it looks like the reference. Six rounds of passing gates produced
something the owner called a mess, and the only thing that ever caught a defect was looking at the window.

## Hard constraints

- Parameter ids 0 to 110 are frozen and every parameter stays reachable from the single window.
- Do not touch `src/synth/`, `src/fx/`, `src/drums/`, `src/Keith_DSP.h`, `config.h`'s plugin identity
  values, or the preset delta table in `src/Keith.cpp`.
- The assets in `resources/reference/` are the owner's artwork. Use them as they are. Do not upscale,
  recolour or redraw them, and do not regenerate them.
- No new dependency.

## Done means

The design canvas is 1536 x 1024 and the face renders at 3072 x 2048. `keith-editor-tests` exits 0 with the
new decoration checks passing, `editor_surface_check.py` is 17 of 17, `fit_budget.py` reports no overflow, the
app builds and launches, and `design/screenshots/w7-vs-reference.png` exists with a written list of what still
differs. Commit on `mags-rebuild-and-levels`. Do not push.
