# Keith — UNIT PLAN (phase 1)

Written by Sam, 2026-10-06, before any dispatch. This is the first artifact of the run, posted rather
than waited on. Locked spec: `BRIEF.md`. Run rules: `preferences.md`.

## What phase 1 has to produce

A usable **v0.1** the friend can open in **Ableton Live 12 Suite on macOS** and make a beat with:
Family 01 (Triton family) banks + the synth engine + the tape/analog character + the editor. The other
three families and Windows-package polish land later as bank updates — banks are data, no rebuild.

## Lanes and their first tasks

| Lane | Agent / model | First task (unit) |
|---|---|---|
| composer | `keith-composer` / Cursor `composer-2.5` | **U1** — iPlug2 skeleton + build system: repo layout, CMake, Windows x64 VST3 target, macOS universal AUv2+VST3 target, one sounding sine/gain voice, pluginval wired, CI matrix (apple-silicon, intel, windows). No DSP work yet. |
| omp | `keith-omp` / OMP `deepseek-flash` | **U2** — content lane: enumerate the real factory program / drum / factory-FX lists of the four reference machines from public documentation and report the real totals into `banks/SOURCES.md`. No invented names, no counts guessed, no patch built before the totals are reported. |
| grok | `keith-grok` / Cursor `cursor-grok-4.5-high` | **U3** — editor: original cartoon-language UI shell, panel layout for drum kit / synth / FX / preset browser all reachable without a tab hunt, param mapping manifest. Original art only. |

Engine scoping (**U4**, the Mishby tape/analog character seeded from Airwindows MIT tape code, plus the
clean-room Surge / OB-XF behaviour mimic) is a **read-and-write-scope** unit for `keith-composer` after
U1's build system lands — it needs a building plugin to put DSP into. It is briefed now so its scope is
recorded, and dispatched as the composer lane's second pass.

## Order

1. `git init` + commit the four handed-over files. **(done)**
2. Run rules + this plan written. **(done)**
3. Dispatch U1, U2, U3 in three visible panes.
4. U2's real totals come back **before** any bank data is written.
5. U1's build system comes back green **before** U4 (engine) is dispatched into it.
6. U3's UI shell lands against U1's parameter manifest once U1 publishes one; until then it builds the
   shell and the mapping manifest from `BRIEF.md` §10.
7. Windows build verified locally on this host. macOS build is a CI product; a real-Mac load test is the
   part of the finish line this run cannot complete from here — recorded as parked, never claimed.

## Gate (per code pass)

- Build green for the committed target.
- The relevant check runs and its counted total is pasted (pluginval, test suite, or the enumeration's
  own row count).
- UI claims carry a rendered reading, not a build log.
- Every report splits measured from NOT judged.

## Stop condition

Work until the phase-1 slice is proven or the run fails. If failing, stop early and report rather than
drifting. Leave the tree green and committed. Anything needing Noah is parked in the final report.

## Parked for Noah (not a stall)

- **Product name**: "Keith" is working-only. Confirm or replace — then it changes in `BRIEF.md`'s header
  first and nowhere else.
- **A real Mac** for the `INSTALL-MAC.md` end-to-end walk and the AU/VST3 load test in Live 12 Suite.
  The brief requires this to be walked on a real machine before handover; this host is Windows.
- **Intel-mac CI runner availability** (BRIEF [A2]) if the provider no longer offers one.
- **`auval` headless reliability** (BRIEF [A3]) if it proves flaky in CI.
