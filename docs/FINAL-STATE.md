# Yote Machine — final state

**The Youth Hostel DSP.** Branch `editorial-redesign`, 8 commits. Working tree clean.
This is the hand-back record: what exists, what is proven, and what is not.

---

## The instrument

A Windows + macOS instrument plugin (VST3, AUv2, standalone) built on iPlug2. Two morphing
oscillators with sub, noise, FM, ring and hard sync; a key-tracked multimode filter with drive; amp,
filter and pitch envelopes; two LFOs, mod wheel, velocity, key tracking and a 4-slot modulation matrix;
a global tape bus; a per-patch insert/master FX chain; and a 16-voice synthesized drum engine.

**101 live parameters. 103 presets across five families.**

| | |
|---|---|
| Framework | iPlug2 (zlib-like licence; BRIEF §5) — not JUCE, deliberately |
| Formats | VST3 + standalone on Windows; VST3 + AUv2 + standalone on macOS (declared, see "not built") |
| Pages | 01 Source · 02 Modulation (LFO 1 / LFO 2 / Matrix) · 03 Filter · 04 Envelopes · 05 Tape · 06 Library (Patches / Drums) · 07 FX |
| Permanent | Filter/Voice channel strip and the playable keyboard |
| Typography | Oswald Bold (OFL 1.1) for headings; Space Mono (OFL 1.1) for values and labels |

### Signal path, exactly as implemented

```
voices → synth bus → Insert 1 → Insert 2 → tape bus → Master → output level → gain
```

### Preset library

103 presets, ordered **emulated family first, then neutral sound role**:

| Family | Count |
|---|---|
| Family 01 | 20 |
| Family 02 | 21 |
| Family 03 | 21 |
| Family 04 | 20 |
| Family 05 | 21 |

Plus a 14-patch arrangement bank covering the BRIEF §8 production palette. Every preset carries a
preloaded insert/master FX chain (§6.4).

**All content is our own parameter-level recreation.** No audio files exist in this repository and none
may be ingested (`banks/SOURCES.md` rules 1, 2, 5). The reference machines' factory data, ROM content and
sample material are not present in any form. Each preset has an own-design provenance row per rule 6.

---

## The package

`dist/` is current with the verified source (regenerate with the same steps after any source change):

| File | md5 |
|---|---|
| `Keith.vst3/Contents/x86_64-win/Keith.vst3` | `14d0a592e5c28bf2ee910186aea2eb10` |
| `Keith.exe` | `faa33795b27073ed659d748fea338cf3` |

Also included: the four font files, `OFL.txt`, `INSTALL-WINDOWS.md`, `INSTALL-MAC.md`, `MANUAL.md`,
`DRUM-MAP.md`.

**Install (Windows):** copy `Keith.vst3` (the whole bundle) to
`C:\Program Files\Common Files\VST3\`. It loads unsigned; no signing needed. Opening a `.vst3` from
Explorer does nothing — it is a DLL inside a bundle and only exists once a host loads it. The standalone
needs its font files beside the `.exe`.

**Nothing was installed on this machine.** `dist/` is gitignored by design; binaries are build output.

---

## Verified

All of the following were re-run independently by the coordinator, against the final commit:

| Gate | Result |
|---|---|
| Release build (4 targets) | clean, no errors or warnings |
| CTest | 3/3 |
| Offline DSP assertions | **126 passed, 0 failed** |
| Editor surface check | **17 passed, 0 failed** |
| Preset metadata check | PASS — 20/21/21/20/21, palette 14 |
| FX chain checker | **14 passed, 0 failed** |
| Material placement check | PASS — 27 placements, shared lighting receipt |
| pluginval strictness 10 | **SUCCESS** — Editor, Open-editor-whilst-processing, Editor Automation all completed |
| State round-trip | 101 parameters + 128 drum slot values through `SerializeState`/`UnserializeState` |
| FX chains engaged | all 103 alter rendered audio; weakest case 2.6% of signal RMS against a 1.0% floor |
| Parameter IDs | 0–100, never renumbered; each pass diffed against the prior commit |
| Frozen assets | `src/editor/ControlLayout.h` and `resources/images` untouched by every pass |

Live interaction was driven against the real Release standalone for each pass: page navigation, numeric
round-trips, knob drag, patch-list scrolling, pad selection and the matrix tab.

Records: `docs/U9-VERIFICATION.md`, `U10-VERIFICATION.md`, `U11-VERIFICATION.md`,
`U12-U14-VERIFICATION.md`, `U15-VERIFICATION.md`, `U16-VERIFICATION.md`,
`EDITOR-REBUILD-VERIFICATION.md`.

---

## NOT verified — read this before trusting anything

- **macOS, AUv2 and auval were never built or run.** The repo is Mac-ready and
  `tools/build-macos.sh` exists, but it has **never been executed** and says so in its own header.
  The Apple resource deployment, the deployment target (11.7.10), the universal build and the AU
  identity (`PLUG_UNIQUE_ID 'Kth1'`, `PLUG_MFR_ID 'Prvt'`) are all configured and none of them is
  proven.
- **FL Studio and Ableton Live have never loaded this build.** pluginval proves instantiation,
  processing and editor behaviour under a validator; a DAW's plugin slot is a different code path.
- **CC1 (mod wheel) and mono legato are proven offline only.** The iPlug2 voice allocator does not
  deliver controller messages to the sounding voice, so both are handled in `KeithDSP` and verified
  through the offline engine.
- **The drum engine is synthesized, not sampled**, and will not match the reference machines'
  percussion timbres. This is forced by the no-samples rule, not an oversight.
- **No CI is configured.** BRIEF §12 names a CI matrix; this repo is a local, private, hand-over
  artifact with no remote, so no CI was set up.

## Deliberately not built

- **Multi-part routing** (`synth_part`, parts 1–4, BRIEF §4.3). `docs/ENGINE-SCOPE.md` §2.4 scopes this
  as a later pass because per-part engine selection would multiply the parameter surface roughly
  fourfold. It is a documented deferral with a reason, not a gap nobody noticed.
- **The reference machines' own per-patch FX chains reproduced verbatim** — impossible without their
  ROM data, which may not be ingested. Our own chains are designed to match the documented character.

---

## Where things live

| | |
|---|---|
| Source | `src/` — `Keith.h` (params), `synth/CoreSynth.h`, `fx/PatchEffects.h`, `fx/TapeAnalog.h`, `drums/DrumSynth.h`, `editor/*` |
| Tools | `tools/` — checkers, offline tests, the Windows input harness, `build-macos.sh`, `render_materials.py` |
| Briefs | `briefs/` — one contract per pass, including the coordinator decisions file |
| Docs | `docs/` — `MANUAL.md`, `DRUM-MAP.md`, `PARAM-MANIFEST.md`, `ENGINE-SCOPE.md`, `UI-DESIGN.md`, install notes, per-unit verification records |
| Content provenance | `banks/SOURCES.md` and `banks/FAMILY-0*-*.md` |

The material textures are Blender/Cycles renders produced by `tools/render_materials.py`; the PNGs are
checked in so a normal build needs no Blender. `src/editor/ControlLayout.h` is the shared placement
table that the renderer and the native controls both read, which is why the material can never drift
away from the live controls.
