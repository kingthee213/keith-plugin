# W7d — size the columns from the text, so the gate goes green

Work in `/Users/solace/YoteMachine-build`, branch **`w7-composition`**. The previous round got the window
to 1152 x 768, which fits the owner's 1280 x 800 screen, and removed the 47 px carve-out. Good. But it
committed with the geometry gate **red**, so the round is not finished.

```
./build/Release/keith-editor-tests
```

reports eight failures:

```
FAIL parameter 33 value "-10.00 ms" fits width (advance 44.3 in 36)
FAIL parameter 34 value "-10.00 ms" fits width (advance 44.3 in 36)
FAIL parameter 36 value "-10.00 ms" fits width (advance 44.3 in 36)
FAIL parameter 37 value "-24.00 st" fits width (advance 44.3 in 36)
FAIL row pitch parameter 15 -> 18 needs 48.3 has 48.0
FAIL row pitch parameter 33 -> 34 needs 48.3 has 46.1
FAIL row pitch parameter 34 -> 35 needs 48.3 has 46.2
FAIL row pitch parameter 37 -> 84 needs 48.3 has 46.1
```

## The fix, and do it as a rule rather than by hand

The columns are sized from a constant, `MIN_W`, and the string that has to fit is not consulted.
`"-10.00 ms"` needs 44.3 px and the rectangle gives it 36.

So make the width a function of the content:

- The generator already knows each parameter's widest value string, and the geometry test already
  computes the advance for a string. Reuse both.
- Add a floor to the grid helpers: a control's width is at least the advance of its widest value string
  plus its horizontal padding. `tools/generate_editor_geometry.py` holds the helpers, and the pitch
  between neighbours follows from the same number, which fixes the row-pitch failures too.
- Then let the affected panels reflow, and adjust their boxes only if the wider cells no longer fit.
  Panels may grow or change shape; that is settled.

Do not widen the eight rectangles by hand. The next parameter with a long value would fail again, and
this same defect has now been reintroduced twice. The fix is the rule, and the gate is what proves it.

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

`keith-editor-tests` must exit 0 with zero failures before you commit. That is the whole point of this
round.

Then crop the title bar off the capture, put it under the reference at the same size, save as
`design/screenshots/w7d-vs-reference.png`, and look at it. Every label and value must read, nothing may
overlap, and no knob may be a dot.

## Done means

`keith-editor-tests` exits 0 with zero failures and no carve-out, `editor_surface_check.py` is 17 of 17,
`fit_budget.py` reports no overflow, the window opens at most 1200 x 780 points, and
`design/screenshots/w7d-vs-reference.png` exists. Commit on `w7-composition`. Do not push.
