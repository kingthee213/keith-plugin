# W7c — undo the height carve-out, and make the window fit a laptop screen

Work in `/Users/solace/YoteMachine-build`, branch **`w7-composition`**. Read
`briefs/W7b-composition-fit.md` and `briefs/W7-composition.md` first, and look at
`design/screenshots/w7b-vs-reference.png`.

The composition is right now. These are the last two things.

## 1. The height rule was relaxed instead of the layout being made to fit

`tools/editor_geometry_test.cpp:82` adds a carve-out:

```cpp
const bool compactStrip = (item.parameter >= 57 && item.parameter <= 72) || ...
const float minKnobH = compactStrip ? 47.f : keith::ui::kKnobMinHeight;
```

`kKnobMinHeight` is `kKnobBodyMin + kKnobLabelHeight + kKnobValueHeight`, which is 34 + 19 + 20 = 73.
`KeithKnobControl::OnResize` takes a 20 px value band and a 19 px label band off the bottom of whatever
it is given and draws the body in what is left. At 47 px that leaves an **8 px body**, which is the
defect this entire line of work started from.

Measured: 16 controls in the effects strip sit at 47 px and 52 px, and all 8 amp-modifier controls sit
at 47 px. That is 24 of the 111 controls drawn as an 8 px dot with a label and a value crammed around it.

Fix it by making those two strips fit rather than by permitting them not to:

- Delete the carve-out. The test goes back to one minimum for every knob.
- Give the effects panel and the amp-modifier strip the height a knob actually needs. The owner's rule
  applies: where the reference's rectangles do not fit our control count, the panel grows or changes
  shape. Every one of the 111 controls stays visible.
- Rebalance the surrounding panels as needed to pay for that height. Nothing else shrinks below 73 px.

The font reductions already made are correct and stay. Labels at 11 px and values at 12 px match the
reference's type scale.

## 2. The window does not fit the owner's screen

His display is 1280 x 800 points. The design canvas is 1536 x 1024, so the window is 1536 x 1056 points
with its title bar and runs off the bottom of his screen. He will see this the moment he opens it.

Build it at **1152 x 768**, which is 0.75 of the reference and the same 3:2 shape, so nothing distorts.
Scale the panel boxes, the control gaps, the type sizes and the pads by 0.75, and resize the crops in
`resources/reference/` by 0.75 with a uniform resize so the artwork keeps its proportions.

Verify the window opens no larger than 1200 x 780 points. If it does, the scale is wrong.

## Verify

```
export PATH="$HOME/cmakebin:$PATH"
python3 tools/generate_editor_geometry.py --write
~/.venvs/keith-measure/bin/python tools/compose_face.py
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
python3 tools/editor_surface_check.py
python3 tools/fit_budget.py
bash tools/capture-window.sh
```

`tools/capture-window.sh` activates the app, reads its bounds from System Events, and captures only that
rectangle. Use it. A full-screen capture is never acceptable, because other windows are on that screen.

Then crop the title bar off the capture, put it under the reference at the same size, save as
`design/screenshots/w7c-vs-reference.png`, and look at it. Confirm no control is an 8 px dot, every label
reads, and nothing overlaps.

## Done means

`keith-editor-tests` exits 0 with no compact-strip carve-out and no control under 73 px, the window opens
at most 1200 x 780 points, `editor_surface_check.py` is 17 of 17, `fit_budget.py` reports no overflow,
and `design/screenshots/w7c-vs-reference.png` exists and shows no squeezed control. Commit on
`w7-composition`. Do not push.
