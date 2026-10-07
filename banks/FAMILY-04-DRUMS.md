# FAMILY 04 - drum kits, drum sounds and pad mapping (internal reference)

Reference machine (internal name, never shipped): **Korg Wavestation**.
Shipped label: **Family 04**.


## Hard rules for this file

- **Internal reference only.** The reference machine's own names may appear in
  this file and in `banks/SOURCES.md`, and nowhere else: not in the UI, preset
  names, file names or plugin metadata.
- Every row comes from a public document listed under *Source documents*; the
  source column says which document and which page or column it was read from.
- Nothing here is a ROM asset. These are name lists and structure read from
  published documentation; no sample data and no factory program data.
- Where the document does not state something, the cell says `not documented`.
- **Shipped labels are positions, not document numbers.** The first row of a
  section is `<family>-P0001`, `<family>-C0001`, `<family>-W0001` and so on; the
  `Number` / `Bank` columns carry the document's own identifiers, which is what
  a later conversion unit should match on.


## Source documents

| Ref | Document | Where it came from | URL |
|---|---|---|---|
| D1 | WAVESTATION Voice Name List (Korg, file WSVoiceNList.pdf, 6 pages, scanned - no text layer) | official Korg download page, product WAVESTATION, document id 1564 | https://www.korg.com/us/support/download/manual/1/224/1564/ |
| D2 | WAVESTATION Reference Guide (Korg, file WSReferenceGuide_E.pdf, 128 pages, scanned - no text layer) and the archive.org scan of the Wavestation A/D Reference Guide | official Korg download page, product WAVESTATION, document id 1563 | https://www.korg.com/us/support/download/manual/1/224/1563/ |

## Drum kits and drum sounds

- **Drum kits: not documented.** D1 has four sections - waves, wave sequences, patches and performances - and no drum-kit section. The machine has no drum-kit concept: percussion is built from waves and wave sequences inside ordinary patches.
- **Drum sounds: not documented as a list.** D1 does not label any wave as a drum sound, and D2 does not publish one either. The drum-type waves therefore exist only as entries in the wave list in `FAMILY-04-PROGRAMS.md`, and which of them are percussion is not stated by any document we found. We do not classify them ourselves.
- **Pad mapping: not documented.** With no drum-kit concept there is no key-to-sound map to record.

## Where we looked for drum documentation

| Document | What it contains for drums |
|---|---|
| D1 Voice Name List (all 6 pages) | no drum section; four lists only |
| D2 Reference Guide (effects, wave and wave-sequence sections) | wave/wave-sequence structure only, no drum-sound list |
| D2 Player's Guide / Owner's Manual (archive.org copies) | searched for 'drum', 'kit', 'percussion'; the guide describes waveform selection and wave sequencing, with no drum-sound list |

