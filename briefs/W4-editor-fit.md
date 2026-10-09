# W4 — make the single-window editor actually fit

Work in `/Users/solace/YoteMachine-build`, branch `mags-rebuild-and-levels`. The tree is clean at
`12a8a2a`.

## The defect, and its root cause

The editor collides on screen while every check passes. The checks are wrong, not the layout.

`KeithKnobControl::OnResize` in `src/editor/KeithControls.h` takes its bounds and does this:

```
mValueBounds = r.GetFromBottom(kValueHeight);   // 20px
r.ReduceFromBottom(kValueHeight);
mLabelBounds = r.GetFromBottom(kLabelHeight);   // 19px
r.ReduceFromBottom(kLabelHeight);
```

So a knob control needs, at minimum, 19 px of label plus 20 px of value plus a usable body. The
tabulated rectangles are far shorter than that. Measured examples from `src/editor/EditorGeometry.h`:
the oscillator knobs are 46 to 52 px tall, the amp faders 48 px. With 39 px spoken for by the text,
the body gets 7 to 13 px and everything crams together. That is exactly what the running window
shows.

The geometry test checks that the label fits its own box. It never checked the value, never checked
that a body was left over, and never knew the difference between a knob and a tab.

## What to build

### 1. Give the geometry table a kind, and gate the real requirement

`ControlRect` in `src/editor/EditorGeometry.h` gains a kind, defaulting to knob so the existing 111
entries keep working. Tabs, dropdowns and the pad grid get marked as what they are. Then
`tools/editor_geometry_test.cpp` gains, for every item:

- a knob is at least `kKnobBodyMin + kKnobLabelHeight + kKnobValueHeight` tall, with those three
  declared next to `ControlRect` and the values cited from `KeithControls.h`
- the label fits the rectangle width, as now
- the widest value string that parameter can produce also fits the rectangle width

For the value strings, take the range and units from `docs/PARAM-MANIFEST.md` and model the widest
case, for example `-60.00 dB` or `20000.00 Hz`. Space Mono advances 0.6 em per character at any
size, which is how `MonoAdvance` is already used for the label check.

`tools/fit_budget.py` already does a version of the value-width analysis in Python, and reports
occupancy per panel. Read it, then make the C++ check the authority. The Python tool stays for the
occupancy table.

### 2. Rebalance the panels with the numbers

`tools/fit_budget.py` currently reports, in square pixels of panel per control:

```
kPanelFx       25 controls   2,896     crowded
kPanelAmp      14 controls   3,035     crowded
kPanelOsc      13 controls   3,899     crowded
kPanelLfo      10 controls   4,762
kPanelFilter    9 controls   4,836
kPanelSample   14 controls   6,297     roomy
kPanelMod      12 controls   6,613     roomy
kPanelGlobal    9 controls   7,556     roomy
kPanelVintage   4 controls  17,000     very roomy
kPanelPads      1 controls  82,688     the pad grid
```

Move area from the roomy panels to the crowded ones. Vintage analog carries four controls in a third
of the bottom row. Do not grow the canvas: 1280 x 800 stays, because the face render scale and the
maximum window size both follow it and that is a one-way door.

### 3. Then enlarge and re-space the control rectangles

This is the bulk of the work and it is 111 rectangles. Every knob needs its body, its label and its
value, and neighbours need a visible gap so two stacked values do not read as one block. Group the
controls in each panel so the rows are even.

### 4. Simplify the effects panel by hiding what is not selected

The effects panel is the worst at 25 controls, and its three A/B/C knobs already relabel themselves
from the selected algorithm, which is why one set of rectangles serves every algorithm.

Apply the same idea harder. Keep three knob rectangles per slot and reuse them, and fold the two
bypass toggles into the algorithm tab row so bypass becomes a property of the selected algorithm
rather than a separate control. That removes controls from the panel rather than finding space for
them. `kParamIns1Bypass`, `kParamIns2Bypass` and `kParamMstBypass` must stay reachable and
automatable, so if the toggles go, the bypass has to be reachable some other way from the surface.

Be careful here: hiding a control is fine, orphaning a parameter is not. Every id from 0 to 110 stays
reachable from the single window and no id moves.

### 5. Re-render the face and look at the result

The panel boxes changed, so the face must be re-rendered. `tools/render_faces.py` reads the panel
boxes from the geometry header, so it follows automatically.

```
export PATH="$HOME/cmakebin:$PATH"
cmake --build build --config Release --target keith-editor-tests MagsSynth-app
./build/Release/keith-editor-tests
~/.venvs/keith-measure/bin/python tools/fit_budget.py
~/.venvs/keith-measure/bin/python tools/render_faces.py
open build/out/Release/MagsSynth.app
```

Then take a screenshot and actually look at it. Screen Recording is granted, so `screencapture -x
<path>` works. Do not leave a full-screen capture on disk, because other windows are on that screen
and it would capture someone else's work. Crop to the plugin window, keep the crop, delete the
original.

Judging by eye at this stage is not optional. The whole reason this defect survived three passes is
that everyone believed a check instead of looking.

## Hard constraints

- Host parameter ids 0 to 110 are frozen. No id moves, no parameter is added.
- Every parameter stays reachable from the single window.
- Do not touch `src/synth/`, `src/fx/`, `src/drums/`, `src/Keith_DSP.h`, `config.h`, or the preset
  delta table in `src/Keith.cpp`.
- Do not change `PLUG_UNIQUE_ID` or `PLUG_MFR_ID`.
- Keep `resources/images/chassis.png` generated by `tools/render_faces.py` rather than hand-edited.
- No new dependency.

## Done means

`./build/Release/keith-editor-tests` exits 0 with the new checks passing, `tools/fit_budget.py`
reports no panel under 5,000 square pixels per control, the app builds and launches, the face
re-renders at 3840 x 2400, and a cropped screenshot of the running window is committed to
`design/screenshots/`. Commit on `mags-rebuild-and-levels`. Do not push.
