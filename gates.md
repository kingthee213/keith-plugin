# keith-plugin — gates and pane map

Herdr workspace **w16** ("keith"), repo `D:/TYH_R10/dev/keith-plugin`, branch `main`.

## Agent → pane map (this run)

| Agent | Lane | Model (from pane footer) | Pane id | Tab | Status |
|---|---|---|---|---|---|
| keith-composer | composer | Composer 2.5 | w16:p5 | w16:t5 | dispatched, working |
| keith-grok | ui | Grok 4.5 High | w16:p3 | w16:t3 | dispatched, working |
| keith-omp | content | DeepSeek-V4.1-Flash | w16:p4 | w16:t4 | dispatched, working |

`CURSOR_API_KEY` / `DEEPSEEK_API_KEY` are not in the Windows user environment; they are exported into
each pane from `%LOCALAPPDATA%\hermes\.env` before the agent starts. A fresh pane needs this again.

## Unit gates

| Unit | Owner | Gate | State |
|---|---|---|---|
| U1 iPlug2 skeleton + build system | keith-composer | Windows VST3 builds, pluginval strictness 10, CI matrix file, pinned iPlug2 revision | dispatched |
| U2 factory list enumeration | keith-omp | real totals per family, citable docs, per-family files with counted rows | dispatched |
| U3 editor shell + visual language | keith-grok | prototype renders (screenshots), param manifest with counted rows | dispatched |
| U4 engine scope + tape block | keith-composer | ENGINE-SCOPE.md, MIT seed named, audible tape block | not dispatched (waits on U1) |
