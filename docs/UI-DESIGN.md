# MAGS SYNTH editor design

## Current direction

Comic, painted, single window. Every control is visible at once; there are no page tabs and no
scrolling surfaces. Owner decision `docs/DECISIONS.md` D4, `BRIEF.md` §10.

The Gorillaz and Green Lantern references inform attitude and palette only. Every asset is original.
No character, likeness, logo, ring or corps mark is drawn, and no reference image is included in the
plugin or in the asset set.

## Instrument model

The wordmark on the face reads **MAGS SYNTH**. The registered identity a host stores is subtype
`MgS1` with maker `Mags`, and it is frozen: changing it after a host saves a set breaks that set.
See `config.h` and `tools/verify_plugin_identity.py`.

The face is one painted board. Parameter graphics are live native controls drawn on top; nothing
about a control is baked into the image except its panel.

## Canvas and scale

- Design canvas 1280 x 800. Uniform scaling through iPlug2's corner resizer, not responsive reflow.
- Floor 1024 x 700, ceiling 3840 x 2400. The ceiling is 3x the design canvas.
- Every face raster is rendered at exactly 3x, so at the largest window it is drawn 1 to 1 and is
  otherwise only downscaled. A face is never upscaled.

## Layout

`src/editor/EditorGeometry.h` is the single source of truth. It holds the canvas, the eleven panel
boxes, the wordmark box, and one entry per parameter giving its rectangle, its label, its label font
size and its rotation. `tools/render_faces.py` reads that same table to paint the panels, so a
painted edge cannot drift away from the control it frames.

Panels, left to right and top to bottom:

- Wood rail down the left, carrying the wordmark.
- Top row: OSCILLATORS, FILTER, AMP, LFO, MOD MATRIX.
- Middle row: the cityscape plate, the sample deck, DRUM PADS.
- Bottom row: VINTAGE ANALOG, GLOBAL, EFFECTS.

## Binding rule

Every control drawn on the face is bound to one of the 111 live parameters in
`docs/PARAM-MANIFEST.md`. Nothing decorative is drawn where a control appears to be.

That rule is why the second oscillator column carries the SUB, NOISE and FM mixer rather than a third
oscillator, and why the VINTAGE ANALOG panel drives the real engine switch, head bump, noise, drive
and saturation. It is also why there is no global pan knob and no reverb, chorus or EQ knob: no
parameter exists behind them. Adding those is a separate pass, and
`docs/SURGE-GAP-ANALYSIS.md` ranks what is missing.

The EFFECTS panel is a re-layout of the existing insert 1, insert 2, master and tape bus controls,
with their real algorithm lists.

## Assets

`tools/render_faces.py` draws the face procedurally with Pillow, at 3840 x 2400, and is the only
source of `resources/images/chassis.png`. There are no image files to source, no downloads, and no
generated raster whose licence we cannot name. The wordmark is drawn from the Bangers face and the
panel titles from Oswald Bold, both OFL 1.1 and both already embedded in the plugin.

It needs Pillow, which the system python does not have:

```
~/.venvs/keith-art/bin/python tools/render_faces.py
```

Wood is sawn planks with grain, wander, knots, a light edge and a shadow edge, plank seams and a
vignette. Panels carry a drop shadow, a vertical gradient, a bevel highlight along the top and left
with a dark bottom and right, paint chips along the outer edge, corner brads and a noise grain. The
cityscape is three depth layers separated by haze with water towers, antennas, strung cables and
warm windows that bloom. The wordmark uses a ragged spray edge, a dark offset outline, a bloom and
thrown specks.

The outgoing Blender material pipeline (`tools/render_materials.py`, `design/physical-editor.blend`)
and its page crops are no longer embedded. They are kept on disk as history, not as the contract.

## Type

Oswald Bold for panel titles. Space Mono Regular and Bold for values, labels and readouts. Bangers
for the wordmark only. OFL notices ship with the distribution.

## Interaction contract

- Knob drag up increases value. iPlug2 gearing is 4.0; the fine-control modifier follows the host.
- Double-click resets a knob to its parameter default.
- Clicking a numeric readout opens exact entry, using the already-loaded mono face rather than
  iPlug2's unloaded default.
- Envelope handles use matching forward and inverse time mapping. Sustain moves vertically.
- Family labels stay Family 01 through Family 05. Preset rows are grouped internally by emulated
  source family and then by neutral sound role; those reference names never reach the UI.
- The wordmark, panel titles and every label are original text. No Korg name reaches the face.

## Verification, and what each check can and cannot prove

- `./build/Release/keith-editor-tests` proves one control rectangle per parameter id, that no two
  rectangles overlap, that every rectangle is inside the canvas, that every label fits its own box
  at its own font size, and that no page deck or nav control is constructed. It does not prove that
  a host draws them, and it is not visual approval.
- `python3 tools/editor_surface_check.py` proves every live parameter is reachable from the editor
  source, that the drum panel is the live pad grid and not a sample browser, and that preset deltas
  target real indices. Counting text mentions does not prove single binding, so it does not claim to;
  the single-binding proof is the geometry test.
- pluginval at strictness 10 covers instantiation, buses, parameter fuzzing, state, editor open and
  editor automation on macOS for both VST3 and AU.
- Real visual approval is the owner opening the standalone and the plugin in Live.

A live screenshot cannot be captured from an agent session on this machine: macOS denies screen
capture unless Screen Recording is granted to the process by hand. Review captures come from a human
session.

## Open

- Face art is a first pass. The wordmark reads as chunky novelty lettering rather than true spray
  script, and the two olive panels are greyer than the reference.
- The reference's left-rail icons are not drawn: there is no control behind them.
- The panel titles name panels, not parameters. Per-parameter labelling lives on the controls.
