# U2 — omp — factory list enumeration (content lane, first pass)

**Lane:** `keith-omp` (OMP, `deepseek-flash`). **Repo:** `D:/TYH_R10/dev/keith-plugin`.
**Read first, in this order:** `BRIEF.md` §6 (content) and §6.5 (naming), `banks/SOURCES.md` (the
ledger and its hard rules), `C:/Users/noaha/.agents/orchestrate/keith-plugin/preferences.md` run rules, `docs/UNIT-PLAN.md`.

## Scope of this pass — enumerate and count. Build nothing.

The single deliverable is the **real factory lists** of the four reference machines, enumerated from
**public documentation**, with the **real totals reported before any patch is built**. Nothing gets
synthesised, sampled, converted or named-invented in this pass.

The four reference machines (internal names only, never shipped — see naming rule below), in build
order:

1. **Family 01 — Triton**
2. **Family 02 — Triton Extreme**
3. **Family 03 — Korg M1**
4. **Family 04 — Korg Wavestation**

For **each** family, from public documentation (factory voice-name lists, manuals, published program
bank listings, official product spec sheets), enumerate:

- the **factory programs / combis / patches** — count them and list them (name + bank + number);
- the **drum kits and drum sounds** — count them and list the kit names, and where a document gives the
  pad-to-sound mapping, record it;
- the **factory FX algorithm names and routing information** — enough for a later unit to reproduce a
  per-patch insert + master chain. If a document does not give the FX chain data, say so explicitly.
- the **PCM/wave list** if the documentation publishes one (this is engine input, not content to
  ingest).

## Where the results go

Fill the **"Factory list enumeration"** table in `banks/SOURCES.md` for Families 01–04:
program count · drum kit/sound count · FX chain data found? · the documents used (title + where it came
from) · reported by.

Then write the full enumerations to **`banks/FAMILY-01-PROGRAMS.md`** … `banks/FAMILY-04-PROGRAMS.md`
(one file per family) so the lists are on disk and countable, and so a later unit converts them without
re-deriving anything. Each row: shipped-neutral label placeholder, the documented original name (this
file is the internal reference doc, so the original names live **here** and nowhere else), source
document, and a one-line note on anything ambiguous in the source.

Also create `banks/FAMILY-01-DRUMS.md` … `FAMILY-04-DRUMS.md` with the drum kits, their sounds and any
documented pad mapping.

## Hard rules — these are the whole point of the pass

- **Do not invent a name, a count or an FX chain.** If a document does not state it, the cell says
  "not documented" and the report says where you looked. An honest gap beats a plausible list.
- **No Korg ROM content**, no data ripped from hardware or from Korg's own software. The machines are
  the *target*, not the source. Documentation only.
- **Neutral labels in shipped names.** The families ship as `Family 01`–`Family 04`. The Korg names may
  appear in these `banks/` internal-reference files (that is what they are for) and **nowhere else** —
  not in the UI, preset names, file names or plugin metadata.
- **No Splice content, ever** — not in the repo, not in a bank, in any format. The Chad Hugo kit is a
  listening reference only.
- **Do not run any git command.** Leave files on disk; the coordinator commits.
- Record a source URL or an exact citation for every document used. A row whose source nobody can name
  does not enter the build.

## Gate — what must be true and pasted

1. The four enumeration tables in `banks/SOURCES.md` are filled, with the counts stated plainly.
2. The per-family files exist; paste their **row counts** as produced by a command (e.g. `wc -l`), not
   by hand.
3. Every family has at least one named, citable source document. Where a number could not be found,
   that is stated as "not documented" with the searches you ran, not filled in with a guess.
4. A short **totals summary**: programs per family, drum kits per family, and which families have FX
   chain data and PCM lists.

## Report

Write `C:/Users/noaha/.agents/orchestrate/keith-plugin/U2-report-omp.md`:
status · the **real totals per family** up front · every source document cited · the commands you ran
to count, with **real pasted output** · what could **NOT** be found in public documentation · what needs
the coordinator. **Building any patch is out of scope for this pass and would fail the review.**
