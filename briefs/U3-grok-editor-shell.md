# U3 — grok — editor / UI shell and visual language

**Lane:** `keith-grok` (Cursor, `cursor-grok-4.5-high`). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first, in this order:** `BRIEF.md` §10 (interface) and §7 (FX), `../preferences.md` run rules,
`docs/UNIT-PLAN.md`.

## Scope of this pass — the shell and the language. Not the DSP, not the banks.

The composer lane (U1) is building the iPlug2 skeleton **in parallel**. Do not wait on it and do not
edit its files. Your pass produces the editor as a **self-contained, standalone-runnable design** plus
the mapping manifest the composer lane will wire into the real parameter surface:

1. **`docs/UI-DESIGN.md`** — the visual language, written so a second person could draw it:
   - the school (thick ink lines, flat saturated colour, comic-panel energy, cosmic green / magenta /
     gold), stated as *original art in that school*; no character, costume, ring, corps symbol, logo or
     mark from either referenced property, ever;
   - the palette as hex values with a role for each;
   - type: which faces, which sizes, which weights — and every face must be freely redistributable
     (record its licence) or drawn by us;
   - the panel/box vocabulary (how a module is framed, how a knob reads, how state is shown);
   - **resizable, one window**; every visible control maps 1:1 to a real parameter; drum kit, synth/
     bass/keys, FX and preset browser all reachable **without a tab hunt**.
2. **`docs/PARAM-MANIFEST.md`** — the contract with the composer lane. One row per control:
   `id` · display label · type (knob / switch / pad / list) · range and units · default · group
   (drum / synth / fx / preset) · the MIDI/host-automatable name. Keep it stable; the engine reads it.
   Where `BRIEF.md` leaves a parameter undecided, list it as a proposed default and mark it proposed —
   do not silently choose something that changes what gets built.
3. **A runnable editor prototype** — a **single self-contained HTML file** (`design/editor-prototype.html`)
   with no external network requests, no CDN, no build step, that renders the full layout at the real
   window size and is resizable. It is a *prototype of the editor*, not the plugin UI; the composer lane
   ports the layout and the manifest into iPlug2. Use only freely licensed or self-drawn assets. It must
   open from the local filesystem and look correct with no network.
4. **`docs/DRUM-MAP.md`** — the drum map the manual will print: which key and which pad triggers which
   slot, velocity layers and round-robin. If the mapping is not yet decided by the documentation, state
   the proposed map and mark it proposed.
5. **Original art only.** If you draw a mark, a mascot, a texture or an icon, it is new. Do not
   reproduce anything recognisable from Gorillaz or Green Lantern, and do not use their names, logos,
   rings, corps symbols or costume marks anywhere in the UI, file names or metadata.

## Hard rules for this pass

- **Do not run any git command.** Leave files on disk; the coordinator commits.
- Original art only. No third-party art or font without a recorded, redistributable licence.
- No Korg names (Triton, Triton Extreme, Korg M1, Korg Wavestation) anywhere in the UI, labels, file
  names or metadata. Neutral `Family 01`–`Family 04` labels only.
- No Splice assets. No Freakshow assets. No Gorillaz or Green Lantern assets.
- No network at runtime; the prototype opens offline.

## Gate — what must be true and pasted

1. `design/editor-prototype.html` exists and **renders** — take a real screenshot from a headless
   browser at the intended window size and at one resized size, and paste both. A build log is not a
   render. State the window size and the browser used.
2. `docs/PARAM-MANIFEST.md` exists; paste its **row count** as produced by a command.
3. `docs/UI-DESIGN.md` and `docs/DRUM-MAP.md` exist and are complete enough for a second person to
   implement from.
4. State explicitly which items are **proposed** and not yet decided.

## Report

Write `C:/Users/noaha/.agents/orchestrate/keith-plugin/U3-report-grok.md`:
status · what you built · the screenshot paths · the rendered reading (what the screenshot actually
shows) · every proposed-but-undecided item · **NOT verified** (what you did not judge — a taste call is
never reported as verified) · what needs the coordinator. Finish with the files left on disk ready to
port.
