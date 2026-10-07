# keith-plugin — gates and pane map

Herdr workspace **w16** ("keith"), repo `D:/TYH_R10/dev/keith-plugin`, branch `main`.

## Agent → pane map (this run)

| Agent | Lane | Model (from pane footer) | Pane id | Tab | Status |
|---|---|---|---|---|---|
| keith-composer | composer | Composer 2.5 | w16:p5 | w16:t5 | round 1 done (U1) |
| keith-grok | ui | Grok 4.5 High | w16:p3 | w16:t3 | round 1 done (U3) |
| keith-omp | content | DeepSeek-V4.1-Flash | w16:p4 | w16:t4 | round 1 done (U2); pane closed |
| keith-composer-2 | composer | Composer 2.5 | w16:p5 | w16:t5 | round 2, dispatched (U4) |
| keith-grok-2 | ui | Grok 4.5 High | w16:p3 | w16:t3 | round 2, dispatched (U5) |

`CURSOR_API_KEY` / `DEEPSEEK_API_KEY` are not in the Windows user environment; they are exported into
each pane from `%LOCALAPPDATA%\hermes\.env` before the agent starts. A fresh pane needs this again.

## Unit gates

| Unit | Owner | Gate | State |
|---|---|---|---|
| U1 iPlug2 skeleton + build system | keith-composer | Windows VST3 builds, pluginval strictness 10, CI matrix file, pinned iPlug2 revision | dispatched |
| U2 factory list enumeration | keith-omp | real totals per family, citable docs, per-family files with counted rows | dispatched |
| U3 editor shell + visual language | keith-grok | prototype renders (screenshots), param manifest with counted rows | dispatched |
| U4 engine scope + tape block | keith-composer-2 | ENGINE-SCOPE.md, MIT seed named, audible tape block | dispatched (round 2) |
| U5 editor fit + manifest reconciliation | keith-grok-2 | layout complete at design size (no clipping), reconciled manifest counts | dispatched (round 2) |

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
