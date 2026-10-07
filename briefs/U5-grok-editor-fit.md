# U5 — grok — editor fit pass and parameter-surface reconciliation

**Lane:** `keith-grok` (Cursor, `cursor-grok-4.5-high`). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first:** `BRIEF.md` §10, `C:/Users/noaha/.agents/orchestrate/keith-plugin/preferences.md` run
rules, your own `U3-report-grok.md`, `docs/PARAM-MANIFEST.md`, `docs/UI-DESIGN.md`, `docs/UNIT-PLAN.md`.

U1 has landed: the plugin now builds and exposes a real parameter surface in `src/Keith.cpp` (read it —
read-only, do not edit it; it is the composer lane's file).

## Defect to fix — the layout does not fit the window it is designed for

The coordinator reproduced this from your own screenshot: at the **intended 1280 × 800** size, content
is **clipped at the bottom of all three columns** — the Kit block below `SLOT - 01 KICK` is cut off, the
mod/voice row and the lower knob edges in the synth column are cut, and `INSERT 2` in the FX column shows
only its header and the tips of its knobs. At 1024 × 700 it is worse. Your report recorded this as
"below the fold inside scrollable columns"; that is exactly what is not acceptable here. `BRIEF.md` §10
requires drum kit, synth/bass/keys, FX and preset browser all reachable **without a tab hunt**, and
scrolling a column to reach a knob is a hunt.

Fix it so the complete layout fits inside the window at its design size and degrades sensibly when
smaller:

- **Fit at 1280 × 800 first** — nothing clipped, no column needs scrolling at the design size. Reduce
  chrome, tighten vertical rhythm, or re-group blocks; do not simply shrink everything to illegibility.
- **Graceful at smaller sizes** — pick and state a real minimum size, and at that minimum make the
  layout still complete (re-flow / re-group / collapse secondary rows) rather than clipped.
- **No visible scrollbars** on any column at the design size or above.
- The parameter surface is unchanged: every control on screen still maps 1:1 to a manifest row.

## Second job — reconcile the manifest with the real parameter surface

`docs/PARAM-MANIFEST.md` was written before U1 existed; all 101 rows are marked proposed. Read
`src/Keith.cpp`'s parameter definitions and produce `docs/PARAM-MANIFEST.md` reconciled:

- one row per **real** parameter that exists in the plugin, with the id/label/range/default the code
  actually uses;
- any manifest row with **no** parameter behind it marked `not yet implemented` — do not delete it
  silently, the intent is still wanted;
- any parameter in the code **absent** from the manifest added, marked as added by this pass;
- the proposed/undecided items stay flagged as proposed. Do not decide a taste call.

## Hard rules

- **Do not run any git command.** The coordinator commits.
- **Do not edit** `src/`, `CMakeLists.txt`, `config/`, `config.h`, `resources/` — composer lane's files.
  Your pass touches `design/` and `docs/` only.
- Original art only. No Korg names, no Gorillaz or Green Lantern characters, marks, rings or logos.
- No network at runtime; the prototype stays a self-contained offline file.

## Gate — what must be true and pasted

1. Fresh screenshots at the **design size** and at the **stated minimum size**, from the same headless
   Chrome, with real file sizes pasted.
2. A **rendered reading** of each screenshot that answers directly: is anything clipped at the bottom,
   and is any column scrolled? Answer it plainly — yes or no, per column. The coordinator will check
   this against the pixels, so a claim that does not hold is worse than an honest "still clipped here".
3. The reconciled `docs/PARAM-MANIFEST.md` with its row counts pasted: total rows, rows matching a real
   parameter, rows `not yet implemented`, rows added this pass.

## Report

`C:/Users/noaha/.agents/orchestrate/keith-plugin/U5-report-grok.md`, same shape as U3: status · what
changed · the screenshots and the rendered reading · counts · **NOT verified** (taste is never verified)
· what needs the coordinator.
