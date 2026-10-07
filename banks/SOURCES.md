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

- **Type** — drum kit / factory program / combi / FX chain / preset bank; for an enumeration-only
  pass, `factory-list enumeration`, `FX algorithm catalogue and routing` or `reference / cross-check only`.
- **Source of the sound** — exactly what it was built from: a named public document, a named
  openly licensed recreation, or our own design.
- **Licence of ingested asset** — the licence of anything actually copied in. `none — our own
  design` where nothing was ingested.
- **Attribution required** — Y/N, and the exact attribution string where Y is.
- **Verified** — who checked it against the reference documentation, and how.

## Family status

| Family | Shipped label | Reference machine (internal only) | Order | Status |
|---|---|---|---|---|
| 01 | Family 01 | Triton | 1st — establishes patch + FX-chain format | factory lists enumerated (U2); no bank data built |
| 02 | Family 02 | Triton Extreme | 2nd — reuses Family 01 structure | factory lists enumerated (U2); no bank data built |
| 03 | Family 03 | Korg M1 | 3rd — simpler PCM/AI engine | factory lists enumerated (U2); no bank data built |
| 04 | Family 04 | Korg Wavestation | 4th — wave-sequencing, its own engine | factory lists enumerated (U2); no bank data built |
| 05 | Family 05 | Korg TR | 5th — later, smaller machine in the Triton family (one insert effect, three combination banks) | factory lists enumerated (U6); no bank data built |

## Factory list enumeration (content lane fills this in first)

Enumerated from public documentation — manuals, patch lists, published program banks. **Report the
real totals before building any patch. Do not invent names or counts.**

| Family | Factory programs (count) | Drum kits / drum sounds (count) | FX chain data found? | Documents used | Reported by |
|---|---|---|---|---|---|
| 01 | **777 documented program rows** - banks A-D **512**, plus GM bank G **265** (128 GM programs `G001`-`G128` and 137 `g(1)`-`g(9)`/`g(d)` variant rows the document itself prints). Combinations: **512** (banks A-D). | **16** factory drum kits (000-015) **+ 9** GM kits (00-08). The per-key high/low drumsample mapping is printed for every kit (88 key rows each). PCM: **425** multisamples, **413** drumsamples. | **Yes for algorithms and routing** - 103 effect algorithms (000-102) and the slot routing (IFX1-5 chain -> MFX1-2 send/return -> master EQ) are published. **No for per-program chains** - no document states which insert/master effects a given factory program uses. | `TRITON Voice Name List` (korg.com, product TRITON, doc id 1652); `TRITON Parameter Guide` (doc id 1648) | keith-omp (U2) |
| 02 | **1,609 documented program rows** - banks A-D **512**, GM bank G **265** (128 GM + 137 variant rows), banks H-M **768**, bank N **64** (`N000`-`N063`; the document prints only that half of bank N). Combinations: **1,280** (banks A-D and H-M). | **50** factory drum kits (000-045, 128-131) **+ 9** GM kits (144-152). Per-key mapping printed for every kit (87-90 key rows each, as the document prints them). PCM: **442** multisample rows (425 distinct numbers, listed in named groups) and **1,233** drumsample rows (417 distinct numbers, grouped). | **Yes for algorithms and routing** - same catalogue (000-102); IFX1/IFX5 and MFX select 000-089 (90 types), IFX2-4 select 000-102 (103, double-size effects), plus Valve Force. **No for per-program chains.** | `TRITON Extreme Voice Name List` (korg.com, product TRITON Extreme); `TRITON Extreme Parameter Guide` (doc id 2423) | keith-omp (U2) |
| 03 | **100 programs** and **100 combinations** (the document's 00-99 grids). | **Not documented.** The owner's manual shows one *example* drum-kit key assignment to illustrate the parameter; no factory kit list or drum-sound list is published in any document found. No pad mapping documented. | **Yes for algorithms and routing** - 33 effect types with names, 2 effect systems, serial or parallel placement, 4 inputs (A-D) / 4 outputs. **No for per-program chains.** | `M1 Voice Name List` (archive.org korg-m-1-voice.name.list); `M1 Owner's Manual` (archive.org synthmanual_M1_owners_manual) | keith-omp (U2) |
| 04 | **105 patches** (banks ROM/RAM1/RAM2, 35 each) and **150 performances** (same three banks, 50 each). | **Not documented.** The Voice Name List has no drum section and the machine has no drum-kit concept; no document labels any wave as a drum sound. No pad mapping documented. | **Partially.** Routing is documented (MDE processor, FX1/FX2 serial or parallel, buses A-D, 55 effect programs over ~26 basic types). The 56-row effect program list was recovered from two independent scans; 8 rows read differently between them and are flagged in the family file. **No per-patch chains.** | `WAVESTATION Voice Name List` (korg.com, doc id 1564); `WAVESTATION Reference Guide` (doc id 1563) plus the archive.org Wavestation A/D Reference Guide scan | keith-omp (U2) |
| 05 | **649 documented program rows** - banks A-D **512**, plus GM bank G **137** (128 GM programs `G001`-`G128` and 9 `g(d)` drum-program variant rows the document prints). Combinations: **384** (banks A, B, C - this machine has three combination banks, not four). | **16** factory drum kits (00-15) **+ 9** GM kits (00-08). The per-key high/low drumsample mapping is printed for every kit: 88 key rows (A0-C8) each, plus 2 extra rows for kit 05 and 8 for kit 15 (96 rows). GM kits print 65 key rows each. PCM: **470** multisamples (Rom 425 + EX 45) and **518** drumsamples (Rom 417 + EX 101). | **Yes for algorithms and routing** - 90 documented effect entries (`00: No Effect` + 89 types `01`-`89`), and the routing is published: BUS Select -> **one** insert effect (IFX) -> post-IFX BUS Select -> sends S1/S2 to MFX1/MFX2 -> returns -> Master EQ. **No for per-program chains** - no document states which insert/master effects a given factory program uses. | `TR Voice Name List` (korg.com, product TR, doc id 1646); `TR Parameter Guide` (doc id 1645); `TR Operation Guide` (doc id 1644) | keith-omp-2 (U6) |

## Ledger

| # | Family | Patch / bank name (shipped label) | Type | Source of the sound | Licence of ingested asset | Attribution required | Verified by / how |
|---|---|---|---|---|---|---|---|
| 1 | 05 | — (no bank, patch or preset built in this pass) | factory-list enumeration (combinations, programs, drum kits, multisamples, drumsamples) | `TR Voice Name List` (Korg, doc id 1646, https://www.korg.com/us/support/download/manual/1/211/1646/) | none — public documentation only, no asset ingested | N | keith-omp-2 (U6): every enumerated row matched against the document's own text layer — 2,021 rows, 0 misses |
| 2 | 05 | — (no bank, patch or preset built in this pass) | FX algorithm catalogue and routing | `TR Parameter Guide` (Korg, doc id 1645, https://www.korg.com/us/support/download/manual/1/211/1645/) | none — public documentation only, no asset ingested | N | keith-omp-2 (U6): 90 of 90 entries recovered, each cross-checked by number and leading word against the guide's own effect chapter; the slot rules and the 470-multisample / 33-drum-kit statement are read from the same document |
| 3 | 05 | — (no bank, patch or preset built in this pass) | reference / cross-check only | `TR Operation Guide` (Korg, doc id 1644, https://www.korg.com/us/support/download/manual/1/211/1644/) | none — public documentation only, no asset ingested | N | keith-omp-2 (U6): read for the mode and effect architecture only; it publishes no factory list of its own |

No row for a Family 05 patch, bank or preset exists yet: this pass enumerated the factory lists and
built nothing (BRIEF.md §6.3). Every Family 05 row above is a *document*, not an ingested asset, and
the research PDFs stay outside the repository in
`C:/Users/noaha/.agents/orchestrate/keith-plugin/scratch/u6/`.
