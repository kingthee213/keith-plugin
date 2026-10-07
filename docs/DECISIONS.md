# Keith — decisions record

Owner decisions that change the locked spec in `BRIEF.md`. Each entry names who decided and when.

## D1 — Family 05 added (Korg TR, 2005)

**Decided by the owner, 2026-10-06.** The content lane's U2 pass found a document titled "TR Voice Name
List" (`TR_VoiceNameList_EFG.pdf`, 34 pages, created 2005-08-26). That is a **different machine** from
the four named in `BRIEF.md` §6 — the Korg TR (2005) — so U2 deliberately did not enumerate it and
flagged the question instead.

The owner has now ruled: **include it.** The Korg TR (2005) becomes **Family 05**, enumerated in the same
form as Families 01–04.

Effect on the spec:
- `BRIEF.md` §6 (content) now covers **five** families, not four.
- `BRIEF.md` §6.5 (naming) extends: the neutral shipped label for the fifth family is **`Family 05`**;
  the name "TR" behaves like the other Korg names — internal reference files only, never in the UI,
  preset names, file names or plugin metadata.
- Build order is unchanged for Families 01–04 (Triton first, per §6.1). Family 05 is enumerated now and
  its bank data lands with the other post-v0.1 bank updates, not as a v0.1 blocker.
- Unit: **U6**, content lane, brief at `briefs/U6-omp-family-05-enumeration.md`.
