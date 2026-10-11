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
| 01 | Family 01 | Triton | 1st — establishes patch + FX-chain format | factory lists enumerated (U2); parameter-level recreations shipped (26 presets); no sample data |
| 02 | Family 02 | Triton Extreme | 2nd — reuses Family 01 structure | factory lists enumerated (U2); parameter-level recreations shipped (21 presets); no sample data |
| 03 | Family 03 | Korg M1 | 3rd — simpler PCM/AI engine | factory lists enumerated (U2); parameter-level recreations shipped (21 presets); no sample data |
| 04 | Family 04 | Korg Wavestation | 4th — wave-sequencing, its own engine | factory lists enumerated (U2); parameter-level recreations shipped (20 presets); no sample data |
| 05 | Family 05 | Korg TR | 5th — later, smaller machine in the Triton family (one insert effect, three combination banks) | factory lists enumerated (U6); parameter-level recreations shipped (21 presets); no sample data |

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
| 4 | 01 | Family 01 / 001 Glass Keys | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 5 | 01 | Family 01 / 002 Sparse Keys | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 6 | 01 | Family 01 / 003 Soft Tines | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 7 | 01 | Family 01 / 004 Bright Clav | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 8 | 01 | Family 01 / 005 Drawbar Organ | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 9 | 01 | Family 01 / 006 Glass Bells | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 10 | 01 | Family 01 / 007 Crystal Pluck | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 11 | 01 | Family 01 / 008 Key Stab | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 12 | 01 | Family 01 / 009 Dry Bass | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 13 | 01 | Family 01 / 010 Smooth Lead | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 14 | 01 | Family 01 / 011 Warm Brass | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 15 | 01 | Family 01 / 012 Silk Strings | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 16 | 01 | Family 01 / 013 Soft Choir | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 17 | 01 | Family 01 / 014 Bell Pad | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 18 | 01 | Family 01 / 015 Slow Motion | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 19 | 01 | Family 01 / 016 Shimmer Texture | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 20 | 01 | Family 01 / 017 Low Drone | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 21 | 01 | Family 01 / 018 Glass Arp | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 22 | 01 | Family 01 / 019 Heavy Delay | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 23 | 01 | Family 01 / 020 Room Kit | preset | our own recreation, matched to TRITON Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 24 | 01 | Family 01 / 021 Upright Bass | preset | our own synthesis, matched by analysis to the Free Wave Samples acoustic bass recording (C2, C3, C4) | none - no audio was ingested or shipped | N | U17: Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 25 | 01 | Family 01 / 022 Hammer Keys | preset | our own synthesis, matched by analysis to the Free Wave Samples acoustic piano recording (C1, C3, C5) | none - no audio was ingested or shipped | N | U17: Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 26 | 01 | Family 01 / 023 Swell Choir | preset | our own synthesis, matched by analysis to the Free Wave Samples slow choir recording (C4) | none - no audio was ingested or shipped | N | U17: Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 27 | 01 | Family 01 / 024 Bright Reed | preset | our own synthesis, matched by analysis to the Free Wave Samples harmonica recording (C5) | none - no audio was ingested or shipped | N | Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 28 | 01 | Family 01 / 025 Snap Bass | preset | our own synthesis, matched by analysis to the Free Wave Samples stein bass recording (C2) | none - no audio was ingested or shipped | N | Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 29 | 01 | Family 01 / 026 Short Bass | preset | our own synthesis, matched by analysis to the Free Wave Samples stein bass recording (C3) | none - no audio was ingested or shipped | N | Jason Champion / freewavesamples.com. Synthesis only. Gate: tools/recreation_gate.py |
| 30 | 02 | Family 02 / 001 Neon Keys | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 31 | 02 | Family 02 / 002 Thin Piano | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 32 | 02 | Family 02 / 003 Thin Clav | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 33 | 02 | Family 02 / 004 Perc Organ | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 34 | 02 | Family 02 / 005 Metal Bells | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 35 | 02 | Family 02 / 006 Neon Pluck | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 36 | 02 | Family 02 / 007 Sparse Pluck | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 37 | 02 | Family 02 / 008 Thin Stab | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 38 | 02 | Family 02 / 009 Hard Bass | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 39 | 02 | Family 02 / 010 Sparse Bass | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 40 | 02 | Family 02 / 011 Sync Lead | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 41 | 02 | Family 02 / 012 Stab Brass | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 42 | 02 | Family 02 / 013 Tight Strings | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 43 | 02 | Family 02 / 014 Breath Choir | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 44 | 02 | Family 02 / 015 Poly Pad | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 45 | 02 | Family 02 / 016 Sweep Motion | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 46 | 02 | Family 02 / 017 Skew Texture | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 47 | 02 | Family 02 / 018 Pulse Drone | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 48 | 02 | Family 02 / 019 Wide Arp | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 49 | 02 | Family 02 / 020 Heavy Crush | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 50 | 02 | Family 02 / 021 Punch Kit | preset | our own recreation, matched to TRITON Extreme Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 51 | 03 | Family 03 / 001 Piano Stack | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 52 | 03 | Family 03 / 002 Soft EP | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 53 | 03 | Family 03 / 003 Soul Piano | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 54 | 03 | Family 03 / 004 Clav Stab | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 55 | 03 | Family 03 / 005 Dry Clav | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 56 | 03 | Family 03 / 006 Warm Organ | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 57 | 03 | Family 03 / 007 Bottle Bells | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 58 | 03 | Family 03 / 008 Muted Pluck | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 59 | 03 | Family 03 / 009 Reed Stab | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 60 | 03 | Family 03 / 010 Finger Bass | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 61 | 03 | Family 03 / 011 Solo Lead | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 62 | 03 | Family 03 / 012 Ensemble Brass | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 63 | 03 | Family 03 / 013 Section Strings | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 64 | 03 | Family 03 / 014 Ooh Choir | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 65 | 03 | Family 03 / 015 Dream Pad | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 66 | 03 | Family 03 / 016 Slow Sweep | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 67 | 03 | Family 03 / 017 Hollow Texture | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 68 | 03 | Family 03 / 018 Warm Drone | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 69 | 03 | Family 03 / 019 Mallet Arp | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 70 | 03 | Family 03 / 020 Slap Delay | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 71 | 03 | Family 03 / 021 Dry Perc | preset | our own recreation, matched to M1 Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 72 | 04 | Family 04 / 001 Vector Keys | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 73 | 04 | Family 04 / 002 Phase Piano | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 74 | 04 | Family 04 / 003 Wave Clav | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 75 | 04 | Family 04 / 004 Drift Organ | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 76 | 04 | Family 04 / 005 Chime Bells | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 77 | 04 | Family 04 / 006 Harp Pluck | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 78 | 04 | Family 04 / 007 Soft Stab | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 79 | 04 | Family 04 / 008 Sub Bass | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 80 | 04 | Family 04 / 009 Air Lead | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 81 | 04 | Family 04 / 010 Swell Brass | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 82 | 04 | Family 04 / 011 Bowed Strings | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 83 | 04 | Family 04 / 012 Air Choir | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 84 | 04 | Family 04 / 013 Cloud Pad | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 85 | 04 | Family 04 / 014 Sparse Pad | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 86 | 04 | Family 04 / 015 Motion Pad | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 87 | 04 | Family 04 / 016 Tilt Texture | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 88 | 04 | Family 04 / 017 Slow Drone | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 89 | 04 | Family 04 / 018 Seq Arp | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 90 | 04 | Family 04 / 019 Wash Delay | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 91 | 04 | Family 04 / 020 Metal Kit | preset | our own recreation, matched to WAVESTATION Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 92 | 05 | Family 05 / 001 Stage Keys | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 93 | 05 | Family 05 / 002 Reed Piano | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 94 | 05 | Family 05 / 003 Funk Clav | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 95 | 05 | Family 05 / 004 Rock Organ | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 96 | 05 | Family 05 / 005 Ping Bells | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 97 | 05 | Family 05 / 006 Mute Pluck | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 98 | 05 | Family 05 / 007 Short Stab | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 99 | 05 | Family 05 / 008 Round Bass | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 100 | 05 | Family 05 / 009 Saw Lead | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 101 | 05 | Family 05 / 010 Section Brass | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 102 | 05 | Family 05 / 011 Small Strings | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 103 | 05 | Family 05 / 012 Close Choir | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 104 | 05 | Family 05 / 013 Tape Pad | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 105 | 05 | Family 05 / 014 Filter Motion | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 106 | 05 | Family 05 / 015 Broken Texture | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 107 | 05 | Family 05 / 016 Dark Drone | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 108 | 05 | Family 05 / 017 Tight Arp | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 109 | 05 | Family 05 / 018 Room Crush | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 110 | 05 | Family 05 / 019 Dry Kit | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 111 | 05 | Family 05 / 020 Hard Kit | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 112 | 05 | Family 05 / 021 Tape Kit | preset | our own recreation, matched to TR Voice Name List | none — our own design | N | U12: parameter deltas over init defaults; no sample ingested |
| 113 | 01 | Family 01 presets — per-preset insert and master FX chains (26 presets) | FX chain | our own design informed by published algorithm names and routing (TRITON Parameter Guide: 103 algorithms 000-102, IFX1-5 chained, MFX1-2 send/return, master EQ) | none — our own design | N | U16: the published document supplies the *shape* only — a chain of inserts into master send effects. Insert 1 is Chorus x11, Delay x8, Drive x6, Crush x1; 12 of 26 add an Insert 2; masters are Comp x12, Width x8, Tilt x5, Limit x1. Bright Reed runs drive at 5% and tilt at 12%. Snap Bass runs chorus at 5% and compression at 12%. Short Bass runs drive at 5% and compression at 12%. Upright Bass and Swell Choir run both chain stages at 5% mix. Hammer Keys runs chorus at 1% and tilt at 12%. Those levels move the audio past the routing check. The weakest chain in the library is Upright Bass, at 2.1% of signal RMS against the 1% floor. Every value is ours, written as `kPresetDeltas` rows in `src/Keith.cpp`. No per-program chain is published for the reference machine and none was ingested; `tools/verify_patch_fx_chains.py` gates the result |
| 114 | 02 | Family 02 presets — per-preset insert and master FX chains (21 presets) | FX chain | our own design informed by published algorithm names and routing (TRITON Extreme Parameter Guide: same 000-102 catalogue, IFX1/IFX5 and MFX select 000-089, plus a valve stage) | none — our own design | N | U16: the documented valve stage motivates a drive-forward reading, not a copied setting — Insert 1 is Drive x11, Chorus x5, Delay x3, Crush x2, with Comp x10 or Limit x3 on the master. 9 of 21 add an Insert 2. Values ours; no per-program chain published, none ingested |
| 115 | 03 | Family 03 presets — per-preset insert and master FX chains (21 presets) | FX chain | our own design informed by published algorithm names and routing (M1 Owner's Manual: 33 effect types, 1-25 stereo and 26-33 dual `DELAY/x`, two effect systems serial or parallel) | none — our own design | N | U16: the documented dual `DELAY/x` types motivate putting the modulation in Insert 1 and the delay in Insert 2 — Insert 1 is Chorus x11, Delay x5, Drive x4, Crush x1, Insert 2 is Delay x9 on 13 of 21; the catalogue's EQ and exciter types motivate Tilt x6 on the master. Values ours; no per-program chain published, none ingested |
| 116 | 04 | Family 04 presets — per-preset insert and master FX chains (20 presets) | FX chain | our own design informed by published algorithm names and routing (WAVESTATION Reference Guide: MDE processor, FX1/FX2 serial or parallel, 55 effect programs over ~26 basic types) | none — our own design | N | U16: the documented two-slot MDE motivates two serial inserts on 19 of 20 presets — Insert 1 is Chorus x13, Delay x4, Crush x2, Drive x1 into Insert 2 Delay x13 — and the wave-sequencing character motivates Width x14 on the master. Values ours; no per-program chain published, none ingested. The 8 OCR-disputed effect-program rows flagged in `banks/FAMILY-04-PROGRAMS.md` were not relied on |
| 117 | 05 | Family 05 presets — per-preset insert and master FX chains (21 presets) | FX chain | our own design informed by published algorithm names and routing (TR Parameter Guide: BUS Select -> **one** insert effect -> sends S1/S2 -> MFX1/MFX2 -> returns -> master EQ) | none — our own design | N | U16: the document publishes a single insert effect for this machine, so Insert 2 is `Off` on all 21 presets and the master stage carries the rest — Insert 1 is Drive x9, Chorus x7, Delay x3, Crush x2; masters are Comp x10, Width x5, Limit x3, Tilt x3. That structural difference is checked by `tools/verify_patch_fx_chains.py`. Values ours; no per-program chain published, none ingested |

Rows 1-3 above stay document enumerations from U6. They ingested no asset. The research PDFs stay outside the repository in
`C:/Users/noaha/.agents/orchestrate/keith-plugin/scratch/u6/`.

Rows 4 onward are the shipped parameter library. Each one is our own preset, a delta list over the init defaults, matched to the named voice list for its role. No sample was ingested, and the enumeration totals above are unchanged.

## Arrangement palette

Neutral bank for the BRIEF section 8 production palette. These are shipped presets already listed in the ledger.

- Family 01 / 002 Sparse Keys
- Family 01 / 008 Key Stab
- Family 01 / 019 Heavy Delay
- Family 02 / 002 Thin Piano
- Family 02 / 007 Sparse Pluck
- Family 02 / 010 Sparse Bass
- Family 02 / 017 Skew Texture
- Family 02 / 020 Heavy Crush
- Family 03 / 003 Soul Piano
- Family 03 / 004 Clav Stab
- Family 04 / 014 Sparse Pad
- Family 04 / 016 Tilt Texture
- Family 05 / 019 Dry Kit
- Family 05 / 020 Hard Kit


## The current library: nine groups, and where each sound comes from

The library was **rebuilt** since the table above was written. It is no longer all synthesis: the
drums, the guitars and the choir are sampled from CC0 and CC-BY sources, and the remaining synth
groups are still the plugin's own engine. This section is the current truth.

**314 presets in nine groups.** `retired drums` holds the 7 original synthesised kit presets, kept
in the table so their indices -- and the deltas and level trims keyed to them -- do not shift onto
other patches. They are hidden from the menu.

| Group | Patches | Type | Source of the sound | Licence of ingested asset | Attribution | Verified by |
|---|---:|---|---|---|---|---|
| mono synths | 61 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | `check_patch_ranges`, `verify_preset_metadata` |
| poly synths | 103 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | as above |
| guitars | 15 | **7 sampled banks, 8 synthesised** | 7 play FreePats / Emilyguitar multisamples; 8 have no clean source and stay synthesised | **CC0 1.0** for all 118 sampled files | **N** | licence file shipped inside each archive, checked; see `resources/samples/guitars/README.md` |
| winds | 25 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | as above |
| sfx | 28 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | as above |
| vocal | 4 | **sampled**, 1 layer | `sfzinstruments/legato_vocal_tutorial`, sustained `a` vowel, 14 recorded notes | **CC0 1.0**, LICENSE in the repo | **N** | `resources/samples/vocal/README.md` |
| bass | 14 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | as above |
| strings | 57 | preset bank, synthesised | our own design, built in the engine | none -- nothing ingested | N | as above |
| drum kits | 10 | drum kit, **sampled** | `MckAudio/MckSamplePacks` -- 90 one-shots, 9 slots per kit | **CC0 1.0**, LICENSE in the repo | **N** | `tools/build_sampled_drum_kits.py`; every hit normalised to -3.00 dBFS |
| percussion kits | 2 | percussion kit, **sampled** | the same collection: congas, timbales, agogo, claves, cowbell, cabasa, castanets, sleigh bell | **CC0 1.0** | **N** | `tools/build_percussion_kits.py` |

### The one source that is NOT CC0, and why it is not used

**Steel-string acoustic guitar.** The only FreePats steel-string bank is
**GPL-3.0-with-exception**, whose exception is written for *"a composition which uses these
sounds, and mix these sounds... into the composition."* Shipping sample files inside a plugin that
other people play is not that -- it is redistributing the files, and the exception does not grant
it. It also only covers *"unaltered"* portions, and the build trims and normalises. FreePats
themselves have deprecated the licence:

> *"This license has been deprecated and it won't be accepted anymore, please consider Creative
> Commons CC0 for your new contributions."*

So Steel String, and the other seven acoustics with no CC0 source, ship with `kParamGuitarBank = 0`
(Off) and play the synth voice. **That is a deliberate limit, not an oversight.**

### MckSamplePacks: what it actually is

Recordings of the author's own drum machines -- **Roland TR-8, RX5, DR-5, RD-6**. This was put to
the owner directly, because rule 1 forbids *"ROM content or factory program data ripped from
hardware"* and a recording of hardware is adjacent to that. **The owner ruled it acceptable**
(2026-10-11), on the basis that a CC0 recording someone makes of hardware they own is a different
thing from extracted ROM data. Recorded here as a judgement rather than something the file proves.

### The vocal layer, and the Ooh that is not there

The choir is the `a` vowel only -- 14 notes, MIDI 48..71. The source has no sustained samples for
any other vowel, so **there is no Ooh**. Deriving one by filtering the Aah would be inventing a
recording and calling it one, so it was left out rather than approximated.

### Attribution

**Nothing in this library requires attribution.** Every ingested asset is CC0 1.0, which is a
public-domain dedication with no attribution condition. The steel-string bank that WOULD have
required care is the one that was not used.

If a CC-BY source is ever added -- one acoustic guitar candidate was CC-BY 3.0 and was not taken --
attribution becomes a real obligation on the manual and the about screen, not just a line here.

### Levels

Measured by `tools/measure_preset_levels.py` through `keith-recreation-render`, which CI builds and
runs. **314 presets measured, zero clipping**, worst peak -2.55 dBFS. The trims are not yet written
back into the presets: the render has roughly 2 dB of run-to-run variance, which is larger than the
+/-1 dB the trim targets, so trimming against it would be fitting to noise.
