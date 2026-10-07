# U4 — composer — engine scoping (RECORDED, dispatch after U1 is green)

**Lane:** `keith-composer` (Cursor, `composer-2.5`). **Status: not yet dispatched.** It is briefed here
so its scope is on the record; it starts only after U1's build system is green and committed, because
there is nothing to put DSP into until then.

**Read first:** `BRIEF.md` §3 (sonic foundation), §4 (synth engine), §7 (FX), `C:/Users/noaha/.agents/orchestrate/keith-plugin/preferences.md` run
rules, `docs/UNIT-PLAN.md`.

## Deliverable of the first engine pass — the scoping document, then the tape/analog block

1. **`docs/ENGINE-SCOPE.md`** — the clean-room plan, written before a line of DSP:
   - **Mishby character** (BRIEF §3): the tape machine / analog degradation block — saturation, head
     bump, wow/flutter drift, hiss, print-through, unstable pitch — usable from subtle glue to full
     destruction, as a **global bus and as per-part FX**. Mishby is closed source under a personal,
     non-transferable EULA: **no Freakshow code, no decompiled binaries, no byte-level imitation.**
     The behaviour is seeded from **Airwindows tape code, which is MIT and explicitly fine in
     closed-source code**, and tuned by ear. Name the exact Airwindows modules you are seeding from and
     record their MIT licence and attribution line in the licence notes.
   - **Surge-style** engine (BRIEF §4.1): wavetable + hybrid oscillators (FM/RM/PD), a modulation
     matrix, its filter set.
   - **OB-XF style** engine (BRIEF §4.2): classic subtractive voice — 2 osc + sub, poly portamento,
     OB-X ladder/sem filter behaviour, unison.
   - **Both in one plugin** (BRIEF §4.3): selectable engine/voice architecture per part.
   - **The clean-room rule, honoured**: Surge (`surge-synthesizer/surge`) and OB-Xf
     (`surge-synthesizer/OB-Xf`) are **GPL-3.0**, and this plugin is handed to another person —
     distribution. Therefore **behaviour is matched by ear against manuals and listening only**:
     no forks, no vendored files, no line-by-line porting, no copying their filter coefficients or
     wavetables. `docs/ENGINE-SCOPE.md` lists, per behaviour, the manual/reference it is being matched
     against and the test you will listen for.
2. **The tape/analog block itself**, implemented in our own DSP in `src/fx/`, with a bypass and a set
   of the parameters named in `docs/ENGINE-SCOPE.md`. Global-bus instance first; per-part instance
   second. It must be automatable and saved inside presets.

## Hard rules

- **Do not run any git command.** The coordinator commits.
- GPL synth source is **study reference, never ingested**. If a file's origin and licence cannot be
  named, it does not enter the tree.
- No Freakshow code or assets. No Splash/Splice assets. No Korg content.
- No new recurring cost: no paid libraries, no subscription DSP.

## Gate

- `docs/ENGINE-SCOPE.md` exists and names, per behaviour, the reference it is matched against and the
  MIT Airwindows seed modules with their attribution.
- The tape/analog block builds, is audible, and its parameters appear in the host.
- A real measurement or a real rendered/bounced audio sample proving the block does something at each
  end of its range (subtle glue → destruction). Paste what you ran; do not describe it.

## Report

`C:/Users/noaha/.agents/orchestrate/keith-plugin/U4-report-composer.md`, same shape as U1: status ·
real commands and pasted output · verdict · deviations · **NOT verified** · what needs the coordinator.
