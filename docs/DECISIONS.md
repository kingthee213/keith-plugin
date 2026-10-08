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

---

## D2 — the Korg-IP prohibition is RETRACTED for this hand-over

**Decided by the owner, 2026-10-07.**

`banks/SOURCES.md` rule 1 ("No Korg ROM content. No Korg factory program data ripped from hardware or
from Korg's own software") and the corresponding lock in `BRIEF.md` [L3] were **not authored by the
owner** — they were written into the brief by an agent lane and presented as a locked decision. The
owner has reviewed that and **retracted the rule for this build.**

**Owner's rationale, in his words:** this is only for personal use — the plugin is handed to one
friend, never sold and never published. He accepts the residual risk of including recordings of the
reference machine's programs, and has directed that sample content from a third-party free sample
library be used to broaden the sound library.

**What this changes:**

- `banks/SOURCES.md` rule 1 and `BRIEF.md` [L3] no longer forbid reference-machine sample content
  **for this private hand-over**. The remaining rules stand: no Splice content (rule 2), no Freakshow
  code or assets (rule 3), no GPL or non-commercial synth source (rule 4), and rule 5's point that a
  code licence says nothing about sample content still applies to any *code* we reference.
- **Provenance stays truthful.** Sample content is recorded in `banks/SOURCES.md` with its actual
  source and the licence as published by that source — it is **not** labelled "our own design".
  Rule 6's own-design category still applies only to content we actually synthesized.
- **Second, separate licence layer.** The recording site's terms are its owner's, not Korg's. As read
  on 2026-10-07, Free Wave Samples states its samples are freeware and royalty-free, asks for a link or
  a mention, and prohibits placing copies for download — with a carve-out whose wording names *web*
  applications specifically. Embedding them in a VST3 handed to one person by the owner is his call and
  he has made it. **Recommended, not required:** credit Jason Champion / freewavesamples.com in
  `README.md` and the manual, which is what the site asks for in return.
- Anything published or streamed publicly would need this re-reviewed. The retraction is scoped to the
  private hand-over the owner described.

**Effect on the engine:** the plugin previously had no sample playback at all (the drum section is
synthesized, because there was no permissible content). This decision is what makes a sampler
worth building, so a sample-playback engine becomes in scope.
