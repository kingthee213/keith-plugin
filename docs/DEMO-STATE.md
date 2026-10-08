# Demo state, 2026-10-08

## What is installed and verified right now

- `~/Library/Audio/Plug-Ins/Components/MagsSynth.component`, ad-hoc signed, `auval -v aumu MgS1 Mags`
  reports AU VALIDATION SUCCEEDED.
- `~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3`, ad-hoc signed.
- `build/out/Release/MagsSynth.app`, the standalone, launches and stays running.
- No stale `Keith.*` bundle is left in either plugin folder, so a host shows one plugin.

Built from `mags-rebuild-and-levels` at `89674c4`.

## What the editor work changed

Five rounds, each ending in a check that would have caught the previous round's miss.

1. The geometry test checked a control's label but never its value or the body left over, so a layout
   that collided on screen reported clean. `KeithKnobControl` takes 19 px for the label and 20 px for
   the value off the bottom of its bounds, and the oscillator knobs were 46 to 52 px tall, so the body
   had 7 to 13 px. That was the whole "looks unfinished" effect.
2. The panels were rebalanced off a measured occupancy table. Effects was at 2,896 square pixels per
   control and vintage analog at 17,000. Every panel is now above 6,000, with no canvas change.
3. The 111 control rectangles were re-sized so each holds its body, label and value, and the effects
   panel lost controls instead of gaining space, by folding bypass into the algorithm row.
4. Panel titles got a reserved band and the keyboard its own strip, both enforced by new checks, plus a
   row-pitch check so two neighbours' text cannot merge.
5. Labels that the row-pitch rule had squeezed to a single letter were restored to declared short names,
   with a check that a one-character label fails unless the table lists it.

The wordmark was painted nine times over. `draw_wordmark` unioned nine offset copies of the letters into
its ink mask and then laid a noise-modulated highlight over the fill, which read as two overlapping
wordmarks. It now draws the letters once, dilates them symmetrically for the outline, and takes the
highlight from the letters' own top edge.

Gates at this commit: `keith-editor-tests` 636 passed 0 failed, `keith-synth-tests` 137 passed 0 failed,
`editor_surface_check.py` 17 passed 0 failed, `fit_budget.py` no panel below 6,000 square pixels per
control, synth recreation gate 40 passed 0 failed.

## Open, and visible in `design/screenshots/w5-verified.png` and the running window

- Fixed. The toggles read. `IVTabSwitchControl` draws one label colour for every tab, so a light
  selected fill under a cream label could never read. The tab style now fills light in both states with
  dark ink text, which is the idiom in the owner's reference, and the selected tab is a solid hot pink.
  The effects `ACT` indicator draws dark ink on its green fill instead of cream. Found on the way: the
  `kHotPink` constant had a typo, `0x4FA3` in a byte position, which resolved to a pale pink; it is now
  the intended `0xFF, 0x4F, 0xA3`.
- Fixed. The GLOBAL panel's green bars are gone. They were not a level meter, they read `kParamGain`
  and drew it as a bar, so the bars sat pegged at 100 percent whenever the synth was idle, with no
  label, and `kParamGain` already has a labelled VOL knob on the same panel. Removed as a duplicate.
- Fixed. The LFO panel drew a waveform box at `IRECT(872, 60, 950, 100)`, which is inside the rate
  knob's own rectangle of `{886, 40, 963, 113}`, so it covered the knob's label and value bands and the
  big RATE sat on top of the squiggle. That panel has no slack to move it into, so the LFO waveform
  decoration is removed. The two oscillator waveform squiggles stay, they are clear of their knobs.
- Both were invisible to the geometry gate because they were hand-placed `IRECT` decorations rather
  than entries in the geometry table. A decoration that overlaps a control is the gap that let them
  through twice. Worth closing by putting decorations in the table too.
- The GLOBAL panel now has an empty gap where the removed bars sat, which reads as breathing room.
- The bottom left rail's help text renders as `MWD/L/R PLAY` and `2/X OCTAVE` instead of `A-L PLAY`
  and `Z/X OCTAVE`, so those two lines overlap each other.
- The preset row in LOADED SOUNDS renders `Ld 01` and its octave label overlapping the adjacent buttons.
- `NOISE` in OSCILLATORS is legible as `NOTSE` at the current size because the glyphs touch. The string
  is correct, the size is not.
- The city artwork panel is now an 80 px wide column. It reads as a deliberate vertical slice, but it is
  far from the hero image in the owner's reference.
- 14 presets that are quiet percussive stabs sit 3 dB or more below the -12 LUFS target, because they
  are already at the peak ceiling and a level control cannot lift them without clipping. They need a
  tone pass and the owner's ear.
- The Windows build has not been regenerated since `resources/main.rc` changed.
- The drum sound-sourcing plan is still unwritten.
- The on-disk bundle names are now `MagsSynth.*`; the build script `tools/build-macos.sh` still looks
  for `Keith.vst3` and `Keith.app`, so its printed install paths are wrong even though the build itself
  is correct.

## Demo path

```
open build/out/Release/MagsSynth.app
```

Or from Live, in the browser under Mags, one entry reads MAGS SYNTH.
