# FAMILY 03 - drum kits, drum sounds and pad mapping (internal reference)

Reference machine (internal name, never shipped): **Korg M1**.
Shipped label: **Family 03**.


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
| D1 | M1 Voice Name List (Korg, 4-page factory name list; scan transcribed from page images) | archive.org item korg-m-1-voice.name.list (scanned copy of the Korg factory name list) | https://archive.org/details/korg-m-1-voice.name.list |
| D2 | M1 Owner's Manual (Korg, archive.org scan with OCR text layer) | archive.org item synthmanual_M1_owners_manual | https://archive.org/details/synthmanual_M1_owners_manual |

## Drum kits, drum sounds and pad mapping

- **Drum kits: not documented as a list.** The machine has a drum-kit instrument setting (D2: 'Assigns drum sounds to the keys', one of four instrument settings). D2 shows **one example** key assignment to illustrate the parameter - `1 BASS DRUM 1, 2 SNARE 1, 3 MID TOM, 4 CLOSED HI HAT, 5 OPEN HI HAT, 6 CRASH CYMBAL 1, 7 RIDE CYMBAL 1, 8 HANDCLAPS, 9 HI CONGA (MUTED), 10 LO BONGO, 11 BASS DRUM 2, 12 SNARE 2, 13 MID TOM, 14 LO TOM ...`. It is printed as an example, not as the factory mapping, so it is **not** recorded here as the factory key map.
- **Drum sounds: not documented as a list.** Neither D1 nor D2 publishes a drum-sound list for this machine. D1 lists programs and combinations only.
- **Pad mapping: not documented.** No published document gives the factory key-to-drum-sound map.

## Where we looked for drum documentation

| Document | What it contains for drums |
|---|---|
| D1 Voice Name List (all 4 pages) | programs and combinations only; the program list includes drum-kit programs named `Drums #1` / `Drums #2` / `Drums #3`, but no drum-sound list |
| D2 Owner's Manual (OCR text, whole document) | the drum-kit parameter and its example assignment; no factory map, no drum-sound list |
| Korg's M1 download page (Owner's Manual, Super Guide) | searched for a drum-sound or multisound list; none published as text |

