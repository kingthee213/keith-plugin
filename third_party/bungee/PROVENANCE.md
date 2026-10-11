# third_party/bungee — provenance

Committed into the repository so the build needs no network for it. Bungee's other dependency,
Eigen, is **not** here — it is ~8 MB of headers and is fetched by `tools/bootstrap-deps` instead.

## What is here

38 files: Bungee's public headers (`bungee/*.h`), its implementation (`src/*.h`, `src/*.cpp`) and
`LICENSE`. 115,543 bytes total.

| | |
|---|---|
| **Written by** | Parabola Research Limited |
| **Upstream** | https://github.com/bungee-audio-stretch/bungee |
| **Pinned revision** | `052451eff35165dae16221092447be0ce521dd24` (main, 2026-09-27) |
| **Licence** | **MPL-2.0** (Mozilla Public License 2.0) |
| **Verified** | `LICENSE` is included in the vendored set; each source file carries an SPDX header reading `MPL-2.0` |

## What it does

Phase-vocoder time-stretching and pitch-shifting, in real time. It is the engine behind the
sampler's warp feature.

Its API takes a `Request` per audio grain, carrying a **frame position** and a speed and pitch
adjustment. That per-grain position is why it was chosen over alternatives: a warp map computes a
different stretch ratio for every segment between markers, so the library has to accept a rate
that changes continuously, not one global tempo.

## Licence reasoning, checked against the standing rules

- **MPL-2.0 is file-level copyleft.** Modifications to *Bungee's own files* must be published;
  files it is combined with are unaffected. We do not modify it, so nothing is triggered.
- **Not Splice, not Freakshow** (`banks/SOURCES.md` rules 2 and 3). Unaffected.
- **Not GPL-only.** Rule 4 excludes GPL and non-commercial sources. MPL is neither. Note D3
  already makes MagsSynth GPL-3.0 because of the Surge/OB-Xf reuse, so MPL is strictly less
  restrictive than what is already accepted.
- **Code licence vs content licence.** Rule 5 warns that an open-source code licence says nothing
  about a project's sample content. Bungee ships no audio, so this is not engaged.

## Why it is a separate CMake target

`keith-bungee` is its own static library rather than extra plugin sources. Two reasons:

1. It is third-party code under its own licence; a distinct target keeps the boundary visible.
2. The plugin reaches it through **Bungee's C API** — `extern "C" const Bungee::Functions`, a
   table of function pointers, with only plain C types crossing. Nothing of Bungee's internals
   appears in our headers.

The plugin talks to it only through `src/sampler/WarpStretcher.h`.

## Eigen and pffft

Both are required, and neither is inside Bungee's own tree:

- **Eigen** — `Eigen/Core` only, but `Eigen/Core` pulls in a large header closure. MIT licensed.
  ~8 MB, which is more than everything else in `third_party/` combined, so it is fetched by
  `tools/bootstrap-deps` at the 3.4.0 tag (`3147391d946bb4b6c68edd901f2add6ac1f31f8c`) and
  gitignored. This matches how iPlug2 is already handled.
- **pffft** — vendored at `third_party/pffft/`, 3 files. See the provenance note there.

## Updating

Deliberate version bumps only. Re-download, re-hash, and update the pinned revision above and in
`tools/bootstrap-deps` for Eigen. Do not float a branch — the point of pinning is that the build
cannot change under us.
