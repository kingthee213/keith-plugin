# keith-plugin — gates and pane map

Herdr workspace **w16** ("keith"), repo `D:/TYH_R10/dev/keith-plugin`, branch `main`.

## Agent → pane map (this run)

| Agent | Lane | Model (from pane footer) | Pane id | Tab | Status |
|---|---|---|---|---|---|
| keith-composer | composer | Composer 2.5 | w16:p5 | w16:t5 | round 1 done (U1) |
| keith-grok | ui | Grok 4.5 High | w16:p3 | w16:t3 | round 1 done (U3) |
| keith-omp | content | DeepSeek-V4.1-Flash | w16:p4 | w16:t4 | round 1 done (U2); pane closed |
| keith-composer-2 | composer | Composer 2.5 | w16:p5 | w16:t5 | round 2 done (U4) |
| keith-grok-2 | ui | Grok 4.5 High | w16:p3 | w16:t3 | round 2 done (U5) |
| keith-omp-2 | content | DeepSeek-V4.1-Flash | w16:p6 | w16:t6 | round 2 done (U6); pane closed |

`CURSOR_API_KEY` / `DEEPSEEK_API_KEY` are not in the Windows user environment; they are exported into
each pane from `%LOCALAPPDATA%\hermes\.env` before the agent starts. A fresh pane needs this again.

## Unit gates

| Unit | Owner | Gate | State |
|---|---|---|---|
| U1 iPlug2 skeleton + build system | keith-composer | Windows VST3 builds, pluginval strictness 10, CI matrix file, pinned iPlug2 revision | done, verified (round 1) |
| U2 factory list enumeration | keith-omp | real totals per family, citable docs, per-family files with counted rows | done, verified (round 1) |
| U3 editor shell + visual language | keith-grok | prototype renders (screenshots), param manifest with counted rows | done, verified (round 1) |
| U4 engine scope + tape block | keith-composer-2 | ENGINE-SCOPE.md, MIT seed named, audible tape block | done (round 2) |
| U5 editor fit + manifest reconciliation | keith-grok-2 | layout complete at design size (no clipping), reconciled manifest counts | done (round 2) |
| U6 Family 05 enumeration (Korg TR) | keith-omp-2 | real totals, citable docs, counted rows, divergence from Family 01 recorded | done, verified (round 2) |

## Round 1 coordinator verification (reproduced, not relayed)

| Claim | Verified how | Result |
|---|---|---|
| Windows VST3 builds | artefact on disk | `build/out/Keith.vst3/Contents/x86_64-win/Keith.vst3`, 550912 bytes |
| pluginval strictness 10 | re-run by coordinator | `SUCCESS` (reproduced) |
| CI workflow present | file on disk | `.github/workflows/build.yml` |
| Pinned iPlug2 rev | grep | `d54f69050f517e43b941d88c2a170f0a840b9ee4` in README.md and tools/bootstrap-deps |
| U2 line counts | `wc -l` | 15941 total across the eight FAMILY files, matching the report exactly |
| No Korg docs in repo | `find` | only iPlug2's own PDFs; no Korg file anywhere |
| U3 prototype offline | `grep -c 'http(s)://'` | 0 |
| PARAM-MANIFEST rows | count | 101, matching the report |
| Rendered UI | screenshot inspection | neutral labels, no Korg/Gorillaz/GL marks; **content clipped at the bottom of all three columns at the design size** — the U5 defect |

## Round 2 coordinator verification — U6, reproduced 2026-10-07

Each row was re-run by the coordinator against the live tree, not relayed from the worker report.

| Claim (U6 report) | Verified how | Result |
|---|---|---|
| Line counts 2259 / 2206 / 68 | `wc -l` | exact match |
| Program rows 649, combi rows 384 | `grep -c "F05-P"` / `"F05-C"` | 649 / 384, exact |
| Drum key rows 1450 (−16 summary −16 headings = 1418), GM 603 (−9 −9 = 585) | `grep -c` | 1450 / 603, exact |
| 90 FX catalogue rows | `awk` over the algorithm table | 90 |
| Family 05 totals 384 / 649 / 16+9 / 470 / 518 | re-ran `triton_vnl.py --profile tr` on the VNL pdf | identical to `scratch/u6/tr.json` (only `source_pdf` path differs) |
| Sample totals corroborated by D2 | grep the Parameter Guide text | `"The internal memory contains 470 different multisamples … and 33 drum kits"`; `"ROM: 000–424, EX: 000–044"` (multisamples); `"ROM: 0000–0416, EX: 000–0100"` (drumsamples) — all present |
| No regression, Families 01/02 | re-ran `triton_vnl.py` (default profile) on the two Triton VNLs, `cmp` vs the U2 JSONs | **byte-identical** |
| No regression, FX tool | ran `parse_index`/`parse_slot_rules`/`parse_ranges` on the U2 text dump, compared to `fx_triton.json` | 103/103 algorithms, slot rules and ranges **identical** |
| Worker did not touch git | `git status --short` | only the expected `M` on three tools + `SOURCES.md` and the two new bank files |
| Korg names stay internal | `grep -rli` over `src/ resources/ config/ design/ config.h CMakeLists.txt` | none |
| No Korg asset in the tree | `find` for `*.pdf *.syx *.pcg *.kmp` | none |

Two caveats recorded rather than hidden:

- The FX **tool wrapper** overwrites `--dump-text` before reading it, so it cannot read a
  pre-existing `pdftotext` dump; the 103-entry Triton catalogue is only reachable by feeding the
  dump to the parse functions directly (as verified above). This is pre-existing behaviour at
  HEAD, not introduced by U6; noted as a follow-up, not fixed in this pass.
- `korg_pg_fx.py` now carries a duplicated identical `clean_name` definition (harmless).
