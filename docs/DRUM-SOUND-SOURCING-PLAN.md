# Drum sound sourcing plan

Status: proposal. No drum work is in flight. The drum section stays synthesized until this is
settled, which is what the owner asked for.

## What exists today

`src/drums/DrumSynth.h` synthesizes sixteen kits worth of percussion. Sixteen slots are mapped to
MIDI 36 through 51, velocity is applied, and round-robin is deterministic. `docs/DRUM-MAP.md` is the
documented map. There are no audio files in the repo and none are shipped.

`BRIEF.md` §2 asks for sampled multi-slot playback with velocity layers and round-robin. That is not
built, and building it was blocked until `docs/DECISIONS.md` D2 retracted the no-samples rule for
this private hand-over. D2 also states what still stands: no Splice content, no Freakshow code or
assets, no GPL or non-commercial synth source, and provenance stays truthful in `banks/SOURCES.md`.

## The decision that gates everything

Every later phase depends on the answer to one question: **which sources may be harvested?** It is a
licensing decision, not a technical one, and no amount of measurement settles it. The three
categories, cheapest first:

1. **Synthesized in-house.** Our own code, no provenance question at all, already shipped. The
   ceiling is timbre: `docs/SPECTRAL-EVAL-PROPOSAL.md` measured the existing synthesized
   recreations between 49 and 111 percent from their reference recordings, where a deliberately
   wrong instrument scores 75 to 178 percent. Two of six sit as far from the reference as an
   unrelated instrument does.
2. **CC0 and public-domain sample libraries.** Free to ship, still requires a provenance row per
   file. Free Wave Samples is the source D2 already read and cleared for this hand-over; its terms
   are freeware and royalty-free, it asks for attribution, and it prohibits re-hosting copies.
3. **Derived from reference recordings.** Wavetable or one-shot resynthesis of the machines named in
   §6. The recording's licence governs the derived result, not ours. This is the only route that
   reaches a real machine's timbre, and it is the route with the most risk.

## Phases

### Phase 0. Gate first, then content

Nothing gets sourced or processed until there is a gate to judge it. Three pieces:

- **The provenance manifest.** `banks/drums/MANIFEST.tsv`, one row per shipped file: slot, layer,
  round-robin index, source name, source URL, licence as published, retrieval date, sha256, and the
  processing steps applied. A checker reads it and fails when a shipped file has no row, when a row
  names a file that is not shipped, or when a file's hash differs from its row.
- **The audio gate.** Per file: no sample at or above 0 dBFS after processing, no DC offset above a
  stated floor, and a loudness reading from the same 100 ms K-weighted measure
  `tools/measure_preset_levels.py` already uses. Then a per-kit check that the sixteen slots sit
  inside a stated band of each other, for the same reason the presets do.
- **Calibration.** `docs/SPECTRAL-EVAL-PROPOSAL.md` says in its own words that the spectral metric is
  not yet fit to gate on. Phase 0 ends by either calibrating it against a set of known-good and
  known-wrong samples, or by explicitly deciding not to gate on timbre and to judge by ear. Both are
  acceptable; leaving it uncalibrated and pretending is not.

### Phase 1. One kit, end to end

Pick one kit and carry it all the way through: source, clear, process, map, measure, load in Live,
play from a pad controller by hand. Sixteen slots, three velocity layers, four round-robin variants
is 192 files, which is the realistic shape. Ship it as the second kit alongside the synthesized one,
so a regression is visible side by side. Nothing scales until this one kit is real and listened to.

### Phase 2. The family banks

Only after Phase 1 passes. Scale per family in the §6 build order, with one commit per kit and the
manifest updated in the same commit.

## The engine side

Independent of sourcing, and sequenced after it because the format follows the content:

- **A sample slot alongside the synthesized voice**, not instead of it. The synthesized voice stays
  as slot 17 and as the fallback when no file is present, so a missing file degrades rather than
  crashes.
- **Velocity layers and round-robin** as data in the manifest, not as code branches.
- **One-shot import**, which the owner asked for. This is the one feature that lets a user drop their
  own file into a slot. It needs a file dialog, a path policy, and a decision about whether imported
  files are copied into the plugin's folder or referenced in place. Referencing in place breaks
  portability, which §2 forbids for presets, so copying is the presumable answer.
- **Storage.** 192 files per kit at roughly 200 KB each is about 38 MB per kit, and half a dozen kits
  is 230 MB. That is a decision about shipping size, and it is the owner's.

## What is not in scope here

No drum UI work, no drum preset content, no change to the drum map, and no touch of the synthesized
voices, until the sourcing question is answered. This document exists so that when the answer comes,
the work starts from a plan instead of from discovery.

## Open questions for the owner

1. Which of the three source categories is authorized, in order of preference?
2. For any derived-from-recording content, is that authorized at all, given it is the route that
   reaches a real machine's timbre and the route D2 flagged as needing a re-review if anything is
   ever published?
3. Does attribution for Free Wave Samples go in `README.md` and the manual, which is what the site
   asks for, and is that acceptable inside a handed-over plugin?
4. Ship the samples inside the plugin bundle, or beside it in a folder the plugin reads?
5. Is a 230 MB ceiling for six kits acceptable?
