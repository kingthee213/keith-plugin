# W2 — single-window comic editor

Worktree `/Users/solace/wt-ui`, branch `wt/ui`, based on `mags-rebuild-and-levels`.

## Why

The current editor is seven pages with a permanent side channel. The owner wants one
window with every control visible at once, in a painted comic style. The style was
directed with a reference image the owner generated himself. An earlier comic attempt was
rejected, and the reason matters: it looked cheap and flat, not because comic was wrong.

The reference images are at
`/Users/solace/TYH_M1/dev/mags-modulator/inbox/mags-synth-final-ui-reference.png` (the
target) and `mags-synth-rough-ui-reference.png` (same layout, plus a cartoon character
the owner does not want). Look at the final one if you can read images. The layout is
described in full below either way, and that description is the contract.

## The layout to build

Canvas stays 1280 x 800. Every panel has a heavy dark outline, a subtle paper grain, a
soft drop shadow, and small brads at the corners. The outer frame is a warm-brown vertical
wood plank. Panels sit on it as painted metal plates in three tints: charcoal, teal-green,
and olive-khaki.

Left rail, full height, about 210 px wide. Wood shows through. Top: the wordmark MAGS
SYNTH in hot pink spray-paint script with a dark offset outline. Bottom: three small line
icons, then a horizontal rule, then four stacked pink lines reading ANALOG, REPLICATION,
DRUMS + KEYS, LOOPS + MORE.

Top row, six panels across the remaining width.

1. OSCILLATORS, charcoal. Three columns, divided by thin vertical rules, headed OSC 1,
   OSC 2, OSC 3. Each column has a waveform glyph on top (sine, pulse, triangle) with a
   small vertical tick beside it, the shape name under it, then two knobs with numeric
   readouts. Under column one a small toggle labelled SYNC, filled magenta. Under column
   two a toggle labelled RING, unfilled. Under column three a toggle labelled FM, filled
   amber.
2. FILTER, teal-green. Two large knobs, CUTOFF and RESONANCE, then a TYPE dropdown, then
   three smaller knobs ENV, KEY, DRIVE.
3. AMP, charcoal. Four vertical faders, A D S R, with readouts beneath, and small red caps
   on the handles.
4. LFO, charcoal. A pink waveform plot on black, then RATE and DEPTH knobs and a SHAPE
   dropdown.
5. MOD MATRIX, charcoal. Column headers SOURCE and DESTINATION. Four rows, each a source
   menu, a destination menu and a small amount knob. A plus button bottom right.

Second row.

6. A large framed artwork panel on the left, about 45 percent of the width, about 300 px
   tall. In the reference it is a painted cityscape: pink and violet sky, dark building
   silhouettes, water towers, strung cables, warm lit windows. No characters, no people.
7. To its right, a panel with three tabs across the top: ANALOG REPLICA, DRUMS, LOADED
   SOUNDS. Under the tabs a waveform strip, then two rows each with a label and a left and
   right arrow, reading KIT and PRESET, then five knobs ATTACK, DECAY, TUNE, PAN, LEVEL.
8. Far right, DRUM PADS, charcoal. A four by four grid of rounded pads in alternating
   magenta, slate and grey, with the row numbers 1 to 4 to the right of each row.

Bottom row, three panels.

9. VINTAGE ANALOG MODELING, olive-khaki. A small illustration plate on the left, then a
   TYPE dropdown and four knobs TONE, NOISE, COMP, DIST.
10. GLOBAL, charcoal. VOLUME and PAN knobs, a VOICES dropdown, a LEGATO toggle, and a
    two-bar output meter.
11. EFFECTS, olive-khaki. Four knobs REVERB, DELAY, CHORUS, EQ, and beneath them three
    small stacked buttons reading SPACE, ANALOG, WARM.

## Binding rules, and these are not negotiable

Every control you draw must be bound to a real live parameter. Read
`docs/PARAM-MANIFEST.md` for the 111 parameters that exist. There is no third oscillator,
no global pan, and no reverb, chorus or EQ parameter. So:

- OSC 1 and OSC 2 are the two real oscillators: shape, tune, level, pulse width, plus the
  SYNC and RING toggles. The third column carries the mixer group instead, SUB, NOISE and
  FM, and must be labelled so it does not read as a third oscillator.
- The AMP panel holds the four real envelope faders. The extra amp modifiers, attack key,
  attack bend, decay key, decay bend, noise key, direct level, stereo delay, stereo track
  and stereo width, go in a compact secondary row inside that panel.
- LFO holds LFO 1 and LFO 2 stacked, each with rate, depth, shape and destination.
- THE VINTAGE ANALOG MODELING panel is the analog voice block. Bind it to real parameters:
  the engine switch, tape head bump, noise level, filter drive and tape saturation. Label
  each knob with the parameter it actually controls.
- EFFECTS is a re-layout of the existing insert 1, insert 2, master and tape bus controls.
  Keep their real algorithm lists and knob counts. Do not draw a reverb, chorus or EQ knob
  that has nothing behind it.
- GLOBAL holds gain, poly or mono, legato, glide, unison and detune. Do not draw a global
  pan knob.

## Assets

Write `tools/render_faces.py` that builds the painted faces with Pillow, procedurally, no
hand-drawn binary files and no downloaded images. Add Pillow to a venv you create at
`~/.venvs/keith-art`. It must produce:

- `resources/images/chassis.png` at exactly 3840 x 2400, which is 3x the design canvas.
  This holds the wood frame, the panel plates, the outlines, the grain, the shadows, the
  brads and the wordmark. The painted cityscape plate can be generated into the same image
  as a marked region, or as its own file, your call, as long as the geometry matches.
- Keep the existing file names working or update `config.h` and `CMakeLists.txt` together.

Rules for the faces:

- 3x scale, never upscaled. The window ceiling in `config.h` is 3840 x 2400, so a face at
  3x is exactly 1 to 1 at the largest window and is only ever downscaled.
- Structural edges, bevels and outlines must be drawn from the same geometry the layout
  uses, so a panel edge cannot drift away from a live control. `src/editor/ControlLayout.h`
  and `src/editor/EditorGeometry.h` are the shared tables; extend them rather than
  hardcoding rectangles in the renderer.
- Wood and grain are tiled at a fixed pixel scale, never stretched to fit a rectangle.
- No character, no person, no Gorillaz or Green Lantern artwork, logo, ring or corps mark.
  A city skyline, sky, cables and buildings are fine and wanted.
- The wordmark is ours. Render MAGS SYNTH as vector text with the loaded font plus a
  spray-paint edge treatment, not as a generated raster.

## Files

- `src/editor/EditorGeometry.h` — the layout table. This is the source of truth.
- `src/editor/EditorialLayout.h` — build one page holding everything. No `KeithPageDeck`,
  no nav bar, no page switching.
- `src/editor/EditorialControls.h`, `KeithControls.h`, `KeithTheme.h` — controls and palette.
- `src/editor/ControlLayout.h` — the knob placement table the renderer also reads.
- `resources/images/*` — the faces.
- `tools/render_faces.py` — new, the face generator.
- `tools/editor_geometry_test.cpp` — extend with the checks below.
- `CMakeLists.txt` and `config.h` — only if the face file list changes.

## Hard constraints

- Every one of the 111 host parameters must still be reachable from the single window.
  There must be no control that only exists on a page you removed.
- Host parameter ids 0 to 110 do not move. No new parameters in this task.
- Do not touch `src/synth/*`, `src/fx/*`, `src/drums/*`, `src/Keith_DSP.h`, or the preset
  table in `src/Keith.cpp`. Those are other lanes.
- Do not touch `tools/measure_preset_levels.py` or `tools/verify_plugin_identity.py`.
- Keep `docs/UI-DESIGN.md` truthful, or leave it for the docs lane to fix.

## Verification, and this is the contract

Extend `tools/editor_geometry_test.cpp` so it fails loudly on any of these:

- every parameter id from 0 to 110 has exactly one control rectangle
- no control rectangle overlaps another
- every control rectangle is inside the 1280 x 800 canvas
- every label fits inside its own rectangle at its own font size
- no page deck, nav button or tab is created in the editor build

Then build the real thing and look at it:

```
cd /Users/solace/wt-ui
export PATH="$HOME/cmakebin:$PATH"
cmake -S . -B build -G Xcode
cmake --build build --config Release --target keith-editor-tests Keith-app
./build/Release/keith-editor-tests
open build/out/Release/Keith.app
```

Take a screenshot of the running standalone and save it to
`design/screenshots/editor-single-window.png`. `screencapture -x -o <path>` works from the
terminal if screen recording permission is already granted.

## Done means

`./build/Release/keith-editor-tests` exits 0 with the new assertions included, the
standalone builds and launches, and the screenshot exists. Commit on `wt/ui`. Do not
merge, do not push. If you cannot finish the assets, commit the layout with a plain
placeholder face and say exactly what is missing in the commit message. A working layout
with an honest placeholder beats a beautiful face that does not build.
