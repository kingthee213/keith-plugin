# third_party/pffft — provenance

Committed into the repository. Bungee's FFT, and its only one.

## What is here

| File | Size | sha256 (first 16) |
|---|---|---|
| `pffft.c` | 68,201 | `485f2c641b9bc943` |
| `pffft.h` | 6,848 | `d6ac7f26f7c3f87e` |
| `fftpack.c` | 88,320 | `d39a4165920c4e9e` |

## Source and licence

- **Written by:** Julien Pommier (pommier@modartt.com), 2013. Based on Fortran 77 FFTPACKv4 by
  Dr Paul Swarztrauber of NCAR, 1985.
- **Upstream:** https://bitbucket.org/jpommier/pffft — this is the revision Bungee's git submodule
  points at (`02fe7715a5bf`).
- **Licence:** the **FFTPACK licence** as set out in the file headers — BSD-style. Redistribution
  in source or binary form is permitted provided the copyright notice and conditions are
  retained, and provided NCAR's and UCAR's names are not used to endorse derived products. There
  is no copyleft.
- **Verified:** the licence block was read out of the vendored `pffft.h` after download, not taken
  from a repository description.

## Why it is required, not optional

`Bungee/src/Fourier.cpp` has this shape:

```c
#ifndef BUNGEE_USE_PFFFT
#  define BUNGEE_USE_PFFFT 1
#endif
#if BUNGEE_USE_PFFFT
... pffft implementation ...
#endif
```

There is nothing in the `#else`. So pffft is Bungee's only FFT and cannot be swapped for another
without editing Bungee, which we do not do.

## Licence reasoning, checked against the standing rules

- **Not Splice, not Freakshow** (`banks/SOURCES.md` rules 2 and 3). Unaffected — this is DSP code,
  not sample content.
- **Not GPL or non-commercial.** The FFTPACK licence is permissive. Rule 4 is not engaged.
- **Code licence vs content licence.** Rule 5: pffft ships no audio, so there is no content
  question.
- **Attribution.** The licence requires the notice be retained in source and reproduced in binary
  distributions. The files are committed unmodified with their headers intact, which satisfies
  the source case; the binary case is covered by this document shipping with the source tree.

## Why these three files and not a fork

Later forks (for example `marton78/pffft`) have restructured the tree into `src/` and `include/`
and added their own CMake. Bungee expects `pffft.c`, `pffft.h` and `fftpack.c` at the include
root, which is the original layout. Taking the original avoids patching Bungee's include.

## Updating

Deliberate bumps only. Re-download, re-hash, update the table above.
