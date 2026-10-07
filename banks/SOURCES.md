# banks/SOURCES.md — every bank, every patch, where it came from

**A row without a filled `Source` and `Licence` may not enter the build.**
No exceptions, no "we'll fill it in later", no patch whose origin nobody can name.

## Hard rules

1. No Korg ROM content. No Korg factory program data ripped from hardware or from Korg's own
   software. The named machines are the *target*, not the source.
2. No Splice samples anywhere in this repo, in any bank, in any format (Section 9 of BRIEF.md).
3. No Freakshow Industries code or assets (Mishby is closed source, personal non-transferable
   EULA). Character is matched by ear, not copied.
4. No source from Surge, OB-Xf or any other GPL / non-commercial synth (Section 4 of BRIEF.md,
   clean-room only).
5. Open-source code is **not** permission to reuse its sample content. Code licence and content
   licence are separate questions. Check both, record both.
6. Every sample and every patch recipe gets one row. If a patch is synthesised from scratch to
   match a documented factory sound, that is its Source: "our own recreation, matched to
   <document>".
7. Neutral labels only in shipped names and metadata. Korg names live in the internal reference
   table below and nowhere else.

## Column meanings

- **Type** — drum kit / factory program / combi / FX chain / preset bank.
- **Source of the sound** — exactly what it was built from: a named public document, a named
  openly licensed recreation, or our own design.
- **Licence of ingested asset** — the licence of anything actually copied in. `none — our own
  design` where nothing was ingested.
- **Attribution required** — Y/N, and the exact attribution string where Y is.
- **Verified** — who checked it against the reference documentation, and how.

## Family status

| Family | Shipped label | Reference machine (internal only) | Order | Status |
|---|---|---|---|---|
| 01 | Family 01 | Triton | 1st — establishes patch + FX-chain format | not started |
| 02 | Family 02 | Triton Extreme | 2nd — reuses Family 01 structure | not started |
| 03 | Family 03 | Korg M1 | 3rd — simpler PCM/AI engine | not started |
| 04 | Family 04 | Korg Wavestation | 4th — wave-sequencing, its own engine | not started |

## Factory list enumeration (content lane fills this in first)

Enumerated from public documentation — manuals, patch lists, published program banks. **Report the
real totals before building any patch. Do not invent names or counts.**

| Family | Factory programs (count) | Drum kits / drum sounds (count) | FX chain data found? | Documents used | Reported by |
|---|---|---|---|---|---|
| 01 | TBD | TBD | TBD | TBD | TBD |
| 02 | TBD | TBD | TBD | TBD | TBD |
| 03 | TBD | TBD | TBD | TBD | TBD |
| 04 | TBD | TBD | TBD | TBD | TBD |

## Ledger

| # | Family | Patch / bank name (shipped label) | Type | Source of the sound | Licence of ingested asset | Attribution required | Verified by / how |
|---|---|---|---|---|---|---|---|
|  |  |  |  |  |  |  |  |
