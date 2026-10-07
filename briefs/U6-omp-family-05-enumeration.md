# U6 — omp — Family 05 enumeration (Korg TR, 2005)

**Lane:** `keith-omp-2` (OMP, `deepseek-flash`). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first:** `BRIEF.md` §6, `banks/SOURCES.md`, `C:/Users/noaha/.agents/orchestrate/keith-plugin/preferences.md`,
`banks/FAMILY-01-PROGRAMS.md` (the format you are matching), your own `U2-report-omp.md`.

## Scope change, authorised

In your U2 report you flagged `TR_VoiceNameList_EFG.pdf` (title "TR Voice Name List", 34 pages, created
2005-08-26) as a **different machine's** list and left it out on scope grounds — correct call at the time.
The owner has now decided: **include it.** The Korg TR (2005) becomes **Family 05**, enumerated in the same
form as Families 01–04. This is an owner decision, recorded in `docs/DECISIONS.md`; the four-family
naming rule in `BRIEF.md` §6.5 extends to `Family 05` for the shipped label.

## Deliverable

Same shape as your U2 output, for this one machine:

1. **`banks/FAMILY-05-PROGRAMS.md`** — combis, programs (bank by bank, with the printed name + number),
   the FX algorithm catalogue and routing as documented, and the PCM/multisample and drumsample lists if
   the documents publish them.
2. **`banks/FAMILY-05-DRUMS.md`** — the factory drum kits and their per-key map, if documented. If the
   documents do not publish a drum list, the same honest "not documented" statement and where you looked.
3. **`banks/SOURCES.md`** — add the Family 05 row to the family status table and to the factory-list
   enumeration table, fill the ledger's source columns, and list every document used.

## Rules are unchanged — they are the point of the pass

- Enumerate from **public documentation** only. No invented names, no guessed counts, no Korg ROM or
  factory data. No Splice content. Nothing sampled or downloaded into the repo — the PDFs stay in
  `C:/Users/noaha/.agents/orchestrate/keith-plugin/scratch/`, never in the tree.
- **Neutral shipped label is `Family 05`.** The names Triton / TR / Korg may appear in these `banks/`
  internal-reference files and nowhere else — not in the UI, preset names, file names or plugin metadata.
- One row per entry, with the document it came from. Anything the documents do not state says
  "not documented" with the searches you ran.
- **Do not run any git command.** The coordinator commits.
- No patch, preset or bank is built in this pass. Enumeration only.

## Gate

1. Totals for Family 05 stated plainly: programs, combis, drum kits, samples, FX algorithms.
2. Row counts of the new files pasted as produced by a command (`wc -l`), not by hand.
3. Every source document cited with a URL or exact citation.
4. Where the TR's list differs from the Triton's, say so — it is a different machine and the overlap or
   divergence is worth recording.

## Report

`C:/Users/noaha/.agents/orchestrate/keith-plugin/U6-report-omp.md`: status · the real totals · documents
used · commands with real pasted output · what could **NOT** be found · what needs the coordinator.
