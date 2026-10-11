# third_party/dr_libs — provenance

These headers are committed into the repository rather than fetched at build time, so the
build needs no network access for them. `third_party/iPlug2/` and the other SDK checkouts
stay gitignored and are fetched by `tools/bootstrap-deps`; these are small enough to carry.

## What is here

| File | Size | sha256 (first 16) | Provides |
|---|---|---|---|
| `dr_wav.h` | 369,705 | `03e70c1a2d9787cd` | WAV decode/encode |
| `dr_flac.h` | 532,768 | `111144e778f55738` | FLAC decode |
| `dr_mp3.h` | 209,166 | `997b7ee18de6e6b8` | MP3 decode |

## Source and licence

- **Written by:** David Reid (mackron)
- **Upstream:** https://github.com/mackron/dr_libs
- **Retrieved:** 2026-10-11, from the `master` branch of the upstream repository
- **Licence as published:** dual-licensed. The header offers a choice between **MIT No
  Attribution (MIT-0)** and **the Unlicense (public domain)**. Both are permissive; MIT-0
  requires no attribution and carries no copyleft.
- **Verified in the files themselves:** all three headers carry the "MIT No Attribution"
  block, "Copyright 2023 David Reid", and the Unlicense alternative. This was checked by
  reading the files after download, not taken on trust from the repository page.

## Why the build needs these

iPlug2, at the revision this project pins (`d54f6905`), provides a WAV **writer**
(`WDL/wavwrite.h`) and **no reader at all**. Its full file tree was checked — 4,159 paths,
no audio-file reader. So loading a sample file needs a decoder from somewhere.

The owner asked for WAV, AIFF, MP3 and FLAC. WAV and AIFF are container formats that could
be parsed here in a few hundred lines. MP3 and FLAC are **codecs**, not header formats —
each is thousands of lines of real signal processing, and writing them would be a project of
its own with a high chance of subtle bugs. dr_libs covers all four behind one small API.

## Why this does not conflict with the standing rules

Checked against `banks/SOURCES.md` hard rules and `docs/DECISIONS.md` D2:

- **Not Splice content.** Rule 2. Unaffected.
- **Not Freakshow code or assets.** Rule 3. Unaffected.
- **Not GPL or non-commercial.** Rule 4 / D3. dr_libs is **MIT-0 or Unlicense** — permissive,
  no copyleft. It does not make the plugin GPL. Note that D3 already makes MagsSynth GPL-3.0
  because of the Surge/OB-Xf reuse, so a permissive dependency is strictly less restrictive
  than what is already accepted.
- **Code licence vs content licence.** Rule 5 warns that an open-source *code* licence says
  nothing about that project's *sample content*. This applies to code only — dr_libs ships
  no audio at all, so there is no content question. Rule 5 is not engaged.
- **Provenance recorded.** Rule 6 requires a row for every sample and patch; this document is
  that record for a code dependency, which the rule's spirit covers — the origin is named,
  the licence is stated as published, and the hashes are recorded so a swapped file is
  detectable.

## Using them

Each is a single-header library. Exactly one translation unit per library defines the
implementation:

```c
#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.h"
```

Every other translation unit includes the header without the define. Defining the
implementation macro in more than one place produces duplicate symbols at link time.

## Updating

Change the pinned revision deliberately, re-download, re-hash, and update the table and the
retrieval date above. Do not float `master` in a build script — the whole point of committing
them is that the build cannot change under us.
