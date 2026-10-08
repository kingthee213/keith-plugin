# Keith editor design

## Owner direction and status

**Current direction (D4, 2026-10-08):** comic, painted, single-window UI — every control visible, no
page tabs. See `BRIEF.md` §10 and `docs/DECISIONS.md` D4. Gorillaz and Green Lantern inform
attitude and green/red contrast only; every asset is original.

**What this document still describes:** the **outgoing** seven-page editorial build (realistic
materials, `EditorialLayout.h`, Blender renders). That layout is **not** the current contract. It
remains here as implementation history and verification reference until the comic single-window
replacement lands.

**New layout work:** the editor lane rebuilds from `BRIEF.md` §10 — one painted panel per functional
group (source, modulation, filter, envelopes, tape, library, FX) arranged in one frame, not hidden
behind tabs. Parameter bindings and host IDs in `docs/PARAM-MANIFEST.md` do not change. No new
brief is required beyond D4; track progress in editor commits and owner review captures.

The original Gorillaz and Green Lantern references inform attitude and green/red contrast only.
Every rendered asset in either generation is original. The old `design/editor-prototype.html` and
U3/U8 briefs are historical references, not the visual contract.

## Instrument model (outgoing editorial build)

Product name (UI): **Yote Machine**, from **The Youth Hostel DSP**. This is a display-name change only:
the repo, binary, plugin name and preset naming are unchanged, per owner instruction. The wordmark
carries the product name and the stamped magenta service plate carries the studio name beneath it.

One worn green-enamel instrument with a stamped red nameplate, exposed metal at the seams, charcoal work area, machined controls and permanent filter/voice channel. Physical light defines the bevels and reflections. Artwork occupies the chassis, while parameter graphics remain live native controls.

Default design canvas: 1280 x 800, scaled through iPlug2's corner resizer. This is uniform scaling, not responsive reflow. Small-size legibility needs owner validation; do not claim the old 1024 x 700 layout guarantee.

## Workflow and layout (outgoing — seven pages)

This section documents the **superseded** tabbed editorial UI. Do not implement new features against
it unless explicitly rolling back D4.

- Header: wordmark, grouped family/preset dropdown, previous/next preset, master output.
- Source: two horizontal oscillator work areas. Oversized morph dials, DSP-derived waveform, tune, level and pulse width. Side mixer holds sub, noise, FM and synth level.
- Envelopes: volume, pitch and filter tabs share a large editable graph. Fixed normalized time lanes keep handles stable while dragging. Pitch depth is separate from the normalized contour, which stays editable at zero depth. Readouts accept exact numeric input.
- Tape: signal order runs from large drive control on the left through head bump, wow/flutter, hiss and print to large dry/wet on the right. Bypass is explicit.
- Library: independent scrollbar-free patch and control-note windows inside the page. Scroll position belongs to each window. Only the visible row can receive a click.
- Permanent right channel: filter type, cutoff, resonance, envelope depth, engine mode, polyphony, glide, unison and detune remain accessible across pages.
- Footer: playable keyboard and typing-key hints.

Page choices are UI state, not extra automation parameters. Existing host IDs 0 through 110 remain unchanged. The family menu and patch rows restore real baked presets.

## Materials and assets (outgoing editorial build)

`tools/render_materials.py` builds the original asset scene in Blender 5.x. Cycles runs on CPU with six threads and 40 samples. One fixed orthographic camera and one upper-left area light illuminate the complete instrument. Uniform world fill is nondirectional. The camera and light do not move for page crops.

`src/editor/ControlLayout.h` is the shared placement table for native knobs and rendered physical controls. The renderer reads the same bounds and radii that the C++ layout uses. Knobs, control bearings, contact wear and shadows are rendered in their actual positions. At runtime the pointer and value remain live, while the rotationally symmetric body stays correctly lit.

Material noise uses world-space Geometry.Position instead of per-object Generated coordinates. This keeps grain scale independent of the shape or size of a panel. Enamel stays mostly intact; exposed material follows case seams and knob bearings. Flat regions behind text remain quiet.

- `resources/images/chassis.png`: 2560 x 1600 shell including permanently visible physical controls.
- `resources/images/workspace-source.png`, `workspace-pitch.png`, `workspace-tape.png`: exact 2x crops for the corresponding page regions, all rendered with the same camera and lighting.
- `resources/images/physical-layout.json`: render receipt with crop bounds, light setup and physical control centres/radii.
- `design/physical-editor.blend`: saved authored common scene.

The unused generic module and dial sprite files are historical assets; they are no longer embedded or drawn. Page images are exact 2x crops of their native bounds, not textures stretched to arbitrary containers.

These are baked images. No Blender installation, ray tracer or external image service is needed at runtime. Windows resources embed the active PNGs into both VST3 and standalone.

Oswald Bold is the wordmark and section-heading face. Space Mono Regular and Bold stay on values, labels, tabs and readouts. Bangers stays embedded and is not used for headings. OFL notices accompany the distribution.

## Reference study

Owner-added references: The God Particle and Output Arcade. These are control/UX references, not assets to copy.

- [The God Particle official product page](https://cradle.app/products/the-god-particle): the inspected official Add Mass illustration makes the primary amount control and its value dominant, with smaller grouped tone controls. Keith uses that hierarchy principle for morph, cutoff, tape drive and mix. It does not reproduce the central sphere or source artwork.
- [Arcade official product page](https://output.com/products/arcade): the inspected Get Inspired image exposes identifiable named content grouped for discovery. Keith keeps patch selection distinct from editing and retains the instrument channel while browsing. The selected marketing images do not prove detailed Arcade interactions, so those are not claimed as verified behavior.
- [Arcade macro documentation](https://support.output.com/en/articles/10297593-macro-sliders): the inspected Play-page reference separates four named macro sliders, a central performance graphic and a persistent keyboard. Keith borrows the separation of immediate sound shaping from deeper editing, not Arcade's assets or identical arrangement. Current Keith controls retain their actual parameter names and bindings rather than suggesting unimplemented macros.
- Official source media stays link-only. No reference imagery is included in the plugin or asset set.

## Interaction contract

- Knob drag up increases value. iPlug2 gearing is 4.0; fine-control modifier follows the host/framework. Double-click resets to the parameter default.
- Click a numeric knob readout for exact entry. Knob and envelope prompts use the already-loaded Space Mono font, not iPlug2's unloaded default font. Native Return/Escape completion is asynchronous; runtime probes wait for the field to close before continuing.
- Envelope time handles use the same forward and inverse mapping. Sustain moves vertically. Readout clicks prompt numeric entry. Double-click a handle resets that parameter.
- Library scrolling clamps to its own content, including short content. Scrolling one list does not move its neighbour.
- Returning to a page preserves its internal selected envelope tab (outgoing tabbed UI only).
- Family labels remain Family 01 through Family 05. The factory library is 103 original parameter-level patches. They are not sampled factory ROM programs, and they are not the documented program lists.
- Internally, preset rows are grouped by emulated source family first — Triton, Triton Extreme, Korg M1, Korg Wavestation, then Korg TR — and only then by neutral sound role. Those reference names are metadata/documentation only; the editor exposes Family 01–05 and neutral role names.

## Verification and boundaries

Compile Release VST3 and standalone. Run CTest, `tools/editor_surface_check.py`, and strictness-10 pluginval with GUI tests. Inspect the actual standalone and test tab visibility, preset selection, knob sensitivity, all envelope interactions and independent list scrolling.

The source checker verifies references, not actual hit testing or sound. The geometry test verifies forward/inverse mappings and scroll clamping. Neither is visual approval. Record captures and test evidence separately, and ask the owner to approve the rendered screen.

The wider BRIEF remains open. This library does not ingest the documented factory program lists, and it does not add a drum sampler or a modulation matrix. Insert effects and the synthesized drum kit are live. Do not treat this as the finished instrument.
