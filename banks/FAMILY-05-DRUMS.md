# FAMILY 05 - drum kits, drum sounds and pad mapping (internal reference)

Reference machine (internal name, never shipped): **Korg TR (2005)**.
Shipped label: **Family 05**.


## Hard rules for this file

- **Internal reference only.** The reference machine's own names (above) may
  appear in this file and in `banks/SOURCES.md`, and nowhere else: not in the UI,
  preset names, file names or plugin metadata.
- Every row is read from a public document listed under *Source documents*; the
  source column names the document and its page.
- Nothing here is a Korg ROM asset. These are name lists and structure taken
  from published documentation; no sample data, no factory PC data.
- Where the document does not state something, the cell says `not documented`.
- **Shipped labels are positions, not document numbers.** The first row of a
  section is `<family>-P0001`, `<family>-C0001`, `<family>-W0001` and so on; the
  `Bank` and `Number` columns carry the document's own bank and number, which is
  what a later conversion unit should match on.


## Source documents

| Ref | Document | Where it came from | URL |
|---|---|---|---|
| D1 | TR Voice Name List (Korg, file TR_VoiceNameList_EFG.pdf, 34 pages, created 2005-08-26) | official Korg download page, product TR, document id 1646 | https://www.korg.com/us/support/download/manual/1/211/1646/ |
| D2 | TR Parameter Guide (Korg, file TR_ParamGuide.pdf, 281 pages, created 2005-10-17) | official Korg download page, product TR, document id 1645 | https://www.korg.com/us/support/download/manual/1/211/1645/ |
| D3 | TR Operation Guide (Korg, file TR_OpGuide_E2, 136 pages, created 2005-10-17) | official Korg download page, product TR, document id 1644 | https://www.korg.com/us/support/download/manual/1/211/1644/ |

## Counts as documented

- Factory drum kits: **16**
- GM drum kits: **9**
- Drumsample rows: **518** - 417 in the Rom group (numbers 0-416) + 101 in the EX group (numbers 0-100). The document restarts its numbering inside each group.
- Drum sounds: the document does not publish a separate drum-sound list for this machine beyond the drumsample names in the PCM list, which are listed in `FAMILY-05-PROGRAMS.md`.

The document prints each kit as a per-key table: the key label, the high drumsample, the low drumsample, the exclusive group and the bus select. Each cell below is the document's own text for that key, in that order. Where a key row is empty the kit does not assign that key.

**Key range as printed.** Every kit page carries one shared key column of 88 keys (A0-C8). Two kits print keys above C8: kit 05 assigns C#8 and D8 in its own column on its page, and kit 15's table continues into a second half-page column that carries the keys C#8-G#8 (96 key rows in total). Both are recorded here as the document prints them. The GM kit pages carry a shared key column of 65 keys (C1-E6).

**Kit slots without factory content.** The machine has 33 kit slots - these 16 preload kits (`00-15 INT`), 8 user kits (`16-23 User`) and 9 GM kits (`24-32 GM`). The user slots carry no factory data in any document, so nothing is listed for them.

## Factory drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F05-K01 | 00 | Standard Kit 1 | 88 | D1 p20 |
| F05-K02 | 01 | Standard Kit 2 | 88 | D1 p20 |
| F05-K03 | 02 | Standard Kit 3 | 88 | D1 p20 |
| F05-K04 | 03 | Processed Kit | 88 | D1 p21 |
| F05-K05 | 04 | Jazz/Brush Kit | 88 | D1 p21 |
| F05-K06 | 05 | House Kit | 90 | D1 p21 |
| F05-K07 | 06 | Tricky Kit | 88 | D1 p22 |
| F05-K08 | 07 | Drum'n'Bass Kit | 88 | D1 p22 |
| F05-K09 | 08 | Hip Hop Kit | 88 | D1 p22 |
| F05-K10 | 09 | Psycho Kit | 88 | D1 p23 |
| F05-K11 | 10 | Trance/GarageKit | 88 | D1 p23 |
| F05-K12 | 11 | Mashed Kit | 88 | D1 p23 |
| F05-K13 | 12 | Precussion Kit | 88 | D1 p24 |
| F05-K14 | 13 | Orchestra&Ethnic | 88 | D1 p24 |
| F05-K15 | 14 | BD/SD Catalog 1 | 88 | D1 p24 |
| F05-K16 | 15 | BD/SD Catalog 2 | 96 | D1 p25 |

## GM drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F05-GM01 | 00 | STANDARD | 65 | D1 p26 |
| F05-GM02 | 01 | ROOM | 65 | D1 p26 |
| F05-GM03 | 02 | POWER | 65 | D1 p26 |
| F05-GM04 | 03 | ELECTRONIC | 65 | D1 p26 |
| F05-GM05 | 04 | ANALOG | 65 | D1 p26 |
| F05-GM06 | 05 | JAZZ | 65 | D1 p27 |
| F05-GM07 | 06 | BRUSH | 65 | D1 p27 |
| F05-GM08 | 07 | ORCHESTRA | 65 | D1 p27 |
| F05-GM09 | 08 | SFX | 65 | D1 p27 |

### F05-K01 - Standard Kit 1 (kit 00, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K01-01 | A0 |  |
| F05-K01-02 | A#0 |  |
| F05-K01-03 | B0 |  |
| F05-K01-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K01-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K01-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K01-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off L/R |
| F05-K01-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 9 L/R |
| F05-K01-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K01-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K01-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K01-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K01-13 | A1 | ROM 227: Drumstick Hit Off L/R |
| F05-K01-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K01-15 | B1 | ROM 073: SD-Reverse Off IFX1 |
| F05-K01-16 | C2 | ROM 000: BD-Dry 1 ROM 000: BD-Dry 1 Off IFX1 |
| F05-K01-17 | C#2 | ROM 003: BD-Dry 4 ROM 003: BD-Dry 4 Off IFX1 |
| F05-K01-18 | D2 | ROM 006: BD-Soft ROM 006: BD-Soft Off IFX1 |
| F05-K01-19 | D#2 | ROM 007: BD-Soft Room ROM 007: BD-Soft Room Off IFX1 |
| F05-K01-20 | E2 | ROM 010: BD-Pillow ROM 010: BD-Pillow Off IFX1 |
| F05-K01-21 | F2 | ROM 048: SD-Full Room ROM 048: SD-Full Room Off IFX1 |
| F05-K01-22 | F#2 | ROM 054: SD-Classic Room ROM 054: SD-Classic Room Off IFX1 |
| F05-K01-23 | G2 | ROM 051: SD-Amb.Piccolo 1ROM 051: SD-Amb.Piccolo 1 Off IFX1 |
| F05-K01-24 | G#2 | ROM 053: SD-Paper ROM 053: SD-Paper Off IFX1 |
| F05-K01-25 | A2 | ROM 042: SD-Dry 3 ROM 042: SD-Dry 3 Off IFX1 |
| F05-K01-26 | A#2 | ROM 040: SD-Dry 1 ROM 040: SD-Dry 1 Off IFX1 |
| F05-K01-27 | B2 | ROM 002: BD-Dry 3 ROM 002: BD-Dry 3 Off IFX1 |
| F05-K01-28 | C3 | ROM 001: BD-Dry 2 ROM 001: BD-Dry 2 Off IFX1 |
| F05-K01-29 | C#3 | ROM 065: Sidestick-Dry ROM 065: Sidestick-Dry Off L/R |
| F05-K01-30 | D3 | ROM 052: SD-Amb.Piccolo 2ROM 052: SD-Amb.Piccolo 2 Off IFX1 |
| F05-K01-31 | D#3 | ROM 229: Hand Claps ROM 229: Hand Claps Off L/R |
| F05-K01-32 | E3 | ROM 060: SD-Big Rock ROM 060: SD-Big Rock Off IFX1 |
| F05-K01-33 | F3 | ROM 106: Tom 2-Floor ROM 106: Tom 2-Floor Off L/R |
| F05-K01-34 | F#3 | ROM 119: H.Hat 1-Closed 2 ROM 119: H.Hat 1-Closed 2 1 L/R |
| F05-K01-35 | G3 | ROM 106: Tom 2-Floor ROM 106: Tom 2-Floor Off L/R |
| F05-K01-36 | G#3 | ROM 121: H.Hat 1-FootOpenROM 120: H.Hat 1-Foot 1 L/R |
| F05-K01-37 | A3 | ROM 105: Tom 2-Lo ROM 105: Tom 2-Lo Off L/R |
| F05-K01-38 | A#3 | ROM 122: H.Hat 1-Open 1 ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-K01-39 | B3 | ROM 105: Tom 2-Lo ROM 105: Tom 2-Lo Off L/R |
| F05-K01-40 | C4 | ROM 104: Tom 2-Hi ROM 104: Tom 2-Hi Off L/R |
| F05-K01-41 | C#4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K01-42 | D4 | ROM 104: Tom 2-Hi ROM 104: Tom 2-Hi Off L/R |
| F05-K01-43 | D#4 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K01-44 | E4 | ROM 155: China Cymbal ROM 155: China Cymbal Off L/R |
| F05-K01-45 | F4 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K01-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K01-47 | G4 | ROM 156: Splash Cymbal ROM 156: Splash Cymbal Off L/R |
| F05-K01-48 | G#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K01-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K01-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K01-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K01-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K01-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K01-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K01-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K01-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K01-57 | F5 | ROM 285: Timbale Hi-Rim 1ROM 284: Timbale Hi-Edge Off L/R |
| F05-K01-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K01-59 | G5 | ROM 304: Agogo Bell Off L/R |
| F05-K01-60 | G#5 | ROM 304: Agogo Bell Off L/R |
| F05-K01-61 | A5 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K01-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K01-63 | B5 | ROM 316: Samba Whistle 2 L/R |
| F05-K01-64 | C6 | ROM 316: Samba Whistle 2 L/R |
| F05-K01-65 | C#6 | ROM 256: Guiro-Short ROM 257: Guiro-Tap 3 L/R |
| F05-K01-66 | D6 | ROM 255: Guiro-Long 3 L/R |
| F05-K01-67 | D#6 | ROM 278: Claves Off L/R |
| F05-K01-68 | E6 | ROM 279: Woodblock 1 Off L/R |
| F05-K01-69 | F6 | ROM 279: Woodblock 1 Off L/R |
| F05-K01-70 | F#6 | ROM 295: Cuica-Hi 4 L/R |
| F05-K01-71 | G6 | ROM 296: Cuica-Lo 4 L/R |
| F05-K01-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 5 L/R |
| F05-K01-73 | A6 | ROM 292: Triangle-Open 5 L/R |
| F05-K01-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K01-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K01-76 | C7 | ROM 174: 88-BD 2(R&B) 6 L/R |
| F05-K01-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 7 L/R |
| F05-K01-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K01-79 | D#7 | ROM 188: 88-HHat Open 7 L/R |
| F05-K01-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K01-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K01-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K01-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K01-84 | G#7 | ROM 171: SD-Orch.Roll 2 8 L/R |
| F05-K01-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 8 L/R |
| F05-K01-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K01-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K01-88 | C8 | ROM 355: Applause Off L/R |

### F05-K02 - Standard Kit 2 (kit 01, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K02-01 | A0 |  |
| F05-K02-02 | A#0 |  |
| F05-K02-03 | B0 |  |
| F05-K02-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K02-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K02-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K02-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off L/R |
| F05-K02-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 9 L/R |
| F05-K02-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K02-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K02-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K02-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K02-13 | A1 | ROM 227: Drumstick Hit Off L/R |
| F05-K02-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K02-15 | B1 | ROM 073: SD-Reverse Off IFX 1 |
| F05-K02-16 | C2 | ROM 014: BD-Tubby ROM 014: BD-Tubby Off IFX 1 |
| F05-K02-17 | C#2 | ROM 015: BD-Gated ROM 015: BD-Gated Off IFX 1 |
| F05-K02-18 | D2 | ROM 036: BD-Chili ROM 036: BD-Chili Off IFX 1 |
| F05-K02-19 | D#2 | ROM 025: BD-Hip 4 ROM 025: BD-Hip 4 Off IFX 1 |
| F05-K02-20 | E2 | ROM 037: BD-PhatJaz ROM 037: BD-PhatJaz Off IFX 1 |
| F05-K02-21 | F2 | ROM 093: SD-Chili ROM 050: SD-Jazz Ring Off IFX 1 |
| F05-K02-22 | F#2 | ROM 099: SD-Ambi Hit 2 ROM 054: SD-Classic Room Off IFX 1 |
| F05-K02-23 | G2 | ROM 092: SD-Brasser ROM 053: SD-Paper Off IFX 1 |
| F05-K02-24 | G#2 | ROM 085: SD-Vintage 4 ROM 056: SD-Brush Tap 1 Off IFX 1 |
| F05-K02-25 | A2 | ROM 078: SD-Hip 5 ROM 078: SD-Hip 5 Off IFX 1 |
| F05-K02-26 | A#2 | ROM 044: SD-GhostSet roll ROM 044: SD-GhostSet roll Off IFX 1 |
| F05-K02-27 | B2 | ROM 005: BD-Dry 6 ROM 005: BD-Dry 6 Off IFX 1 |
| F05-K02-28 | C3 | ROM 004: BD-Dry 5 ROM 004: BD-Dry 5 Off IFX 1 |
| F05-K02-29 | C#3 | ROM 066: Sidestick-Amb ROM 066: Sidestick-Amb Off L/R |
| F05-K02-30 | D3 | ROM 045: SD-GhostSet f ROM 047: SD-GhostSet p2 Off IFX 1 |
| F05-K02-31 | D#3 | ROM 229: Hand Claps ROM 229: Hand Claps Off L/R |
| F05-K02-32 | E3 | ROM 094: SD-Klanger ROM 046: SD-GhostSet p1 Off IFX 1 |
| F05-K02-33 | F3 | ROM 103: Tom 1-Floor ROM 103: Tom 1-Floor Off L/R |
| F05-K02-34 | F#3 | ROM 126: H.Hat 2-Closed 2 ROM 125: H.Hat 2-Closed 1 1 L/R |
| F05-K02-35 | G3 | ROM 103: Tom 1-Floor ROM 103: Tom 1-Floor Off L/R |
| F05-K02-36 | G#3 | ROM 128: H.Hat 2-FootOpen ROM 127: H.Hat 2-Foot 1 L/R |
| F05-K02-37 | A3 | ROM 102: Tom 1-Lo ROM 102: Tom 1-Lo Off L/R |
| F05-K02-38 | A#3 | ROM 131: H.Hat 2-Sizzle ROM 129: H.Hat 2-Open 1 1 L/R |
| F05-K02-39 | B3 | ROM 102: Tom 1-Lo ROM 102: Tom 1-Lo Off L/R |
| F05-K02-40 | C4 | ROM 101: Tom 1-Hi ROM 101: Tom 1-Hi Off L/R |
| F05-K02-41 | C#4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K02-42 | D4 | ROM 101: Tom 1-Hi ROM 101: Tom 1-Hi Off L/R |
| F05-K02-43 | D#4 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K02-44 | E4 | ROM 155: China Cymbal ROM 155: China Cymbal Off L/R |
| F05-K02-45 | F4 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K02-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K02-47 | G4 | ROM 156: Splash Cymbal ROM 156: Splash Cymbal Off L/R |
| F05-K02-48 | G#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K02-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K02-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K02-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K02-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K02-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K02-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K02-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K02-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K02-57 | F5 | ROM 285: Timbale Hi-Rim 1 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K02-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K02-59 | G5 | ROM 304: Agogo Bell Off L/R |
| F05-K02-60 | G#5 | ROM 304: Agogo Bell Off L/R |
| F05-K02-61 | A5 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K02-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K02-63 | B5 | ROM 316: Samba Whistle 2 L/R |
| F05-K02-64 | C6 | ROM 316: Samba Whistle 2 L/R |
| F05-K02-65 | C#6 | ROM 256: Guiro-Short ROM 257: Guiro-Tap 3 L/R |
| F05-K02-66 | D6 | ROM 255: Guiro-Long 3 L/R |
| F05-K02-67 | D#6 | ROM 278: Claves Off L/R |
| F05-K02-68 | E6 | ROM 279: Woodblock 1 Off L/R |
| F05-K02-69 | F6 | ROM 279: Woodblock 1 Off L/R |
| F05-K02-70 | F#6 | ROM 295: Cuica-Hi 4 L/R |
| F05-K02-71 | G6 | ROM 296: Cuica-Lo 4 L/R |
| F05-K02-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 5 L/R |
| F05-K02-73 | A6 | ROM 292: Triangle-Open 5 L/R |
| F05-K02-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K02-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K02-76 | C7 | ROM 174: 88-BD 2(R&B) 6 L/R |
| F05-K02-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 7 L/R |
| F05-K02-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K02-79 | D#7 | ROM 188: 88-HHat Open 7 L/R |
| F05-K02-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K02-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K02-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K02-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K02-84 | G#7 | ROM 171: SD-Orch.Roll 2 8 L/R |
| F05-K02-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 8 L/R |
| F05-K02-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K02-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K02-88 | C8 | ROM 355: Applause Off L/R |

### F05-K03 - Standard Kit 3 (kit 02, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K03-01 | A0 |  |
| F05-K03-02 | A#0 |  |
| F05-K03-03 | B0 |  |
| F05-K03-04 | C1 | EX 009: BD-Dance 12 Off IFX1 |
| F05-K03-05 | C#1 | EX 004: BD-Dance 7 Off IFX1 |
| F05-K03-06 | D1 | EX 007: BD-Dance 10 Off IFX1 |
| F05-K03-07 | D#1 | ROM 173: 88-BD 1(Rap) Off IFX1 |
| F05-K03-08 | E1 | ROM 177: 99-BD 2 Off IFX1 |
| F05-K03-09 | F1 | ROM 061: SD-Yowie ROM 061: SD-Yowie Off IFX1 |
| F05-K03-10 | F#1 | ROM 041: SD-Dry 2 ROM 041: SD-Dry 2 Off IFX1 |
| F05-K03-11 | G1 | ROM 055: SD-Brush Hit ROM 055: SD-Brush Hit Off IFX1 |
| F05-K03-12 | G#1 | ROM 059: SD-Atomic ROM 059: SD-Atomic Off IFX1 |
| F05-K03-13 | A1 | ROM 227: Drumstick Hit Off IFX1 |
| F05-K03-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K03-15 | B1 | EX 029: SD-Dance 4 Off L/R |
| F05-K03-16 | C2 | ROM 009: BD-Jazz 2 ROM 009: BD-Jazz 2 Off IFX1 |
| F05-K03-17 | C#2 | ROM 007: BD-Soft Room ROM 007: BD-Soft Room Off IFX1 |
| F05-K03-18 | D2 | ROM 008: BD-Jazz 1 ROM 008: BD-Jazz 1 Off IFX1 |
| F05-K03-19 | D#2 | ROM 011: BD-Woofer ROM 011: BD-Woofer Off IFX1 |
| F05-K03-20 | E2 | ROM 006: BD-Soft ROM 006: BD-Soft Off IFX1 |
| F05-K03-21 | F2 | EX 013: SD-Dry 4 EX 013: SD-Dry 4 Off IFX1 |
| F05-K03-22 | F#2 | EX 022: SD-Dry 9 EX 022: SD-Dry 9 Off IFX1 |
| F05-K03-23 | G2 | ROM 040: SD-Dry 1 ROM 040: SD-Dry 1 Off IFX1 |
| F05-K03-24 | G#2 | EX 017: SD-Dry 7 EX 017: SD-Dry 7 Off IFX1 |
| F05-K03-25 | A2 | EX 015: SD-Dry 5 ff EX 015: SD-Dry 5 ff Off IFX1 |
| F05-K03-26 | A#2 | EX 016: SD-Dry 6 EX 016: SD-Dry 6 Off IFX1 |
| F05-K03-27 | B2 | EX 001: BD-amb 2 EX 001: BD-amb 2 Off IFX1 |
| F05-K03-28 | C3 | ROM 003: BD-Dry 4 ROM00 2: BD-Dry 3 Off IFX1 |
| F05-K03-29 | C#3 | EX 026: Sidestick-Amb2 f EX 025: Sidestick-Amb2mf Off IFX1 |
| F05-K03-30 | D3 | EX 021: SD-Dry 8 ff EX 020: SD-Dry 8 f Off IFX1 |
| F05-K03-31 | D#3 | ROM 229: Hand Claps ROM 229: Hand Claps Off IFX1 |
| F05-K03-32 | E3 | EX 020: SD-Dry 8 f EX 018: SD-Dry 8 p Off IFX1 |
| F05-K03-33 | F3 | EX 045: Tom 4-Lo EX 045: Tom 4-Lo Off IFX1 |
| F05-K03-34 | F#3 | EX 053: H.Hat 3-Close f EX 052: H.Hat 3-Close mf 1 L/R |
| F05-K03-35 | G3 | EX 045: Tom 4-Lo EX 045: Tom 4-Lo Off IFX1 |
| F05-K03-36 | G#3 | EX 054: H.Hat 3-Foot 1 L/R |
| F05-K03-37 | A3 | EX 045: Tom 4-Lo EX 045: Tom 4-Lo Off IFX1 |
| F05-K03-38 | A#3 | EX 055: H.Hat 3-Open EX 055: H.Hat 3-Open 1 L/R |
| F05-K03-39 | B3 | EX 046: Tom 4-Mid EX 046: Tom 4-Mid Off IFX1 |
| F05-K03-40 | C4 | EX 047: Tom 4-Hi EX 047: Tom 4-Hi Off IFX1 |
| F05-K03-41 | C#4 | ROM 153: Crash Cymbal 2 ROM 153: Crash Cymbal 2 Off L/R |
| F05-K03-42 | D4 | EX 047: Tom 4-Hi EX 047: Tom 4-Hi Off IFX1 |
| F05-K03-43 | D#4 | EX 078: Ride-Edge 3 EX 078: Ride-Edge 3 Off L/R |
| F05-K03-44 | E4 | ROM 155: China Cymbal Off L/R |
| F05-K03-45 | F4 | EX 079: Ride-Cup 2 EX 079: Ride-Cup 2 Off L/R |
| F05-K03-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K03-47 | G4 | EX 080: Splash Cymbal 2 EX 080: Splash Cymbal 2 Off IFX1 |
| F05-K03-48 | G#4 | ROM 306: Cowbell 2 ROM 305: Cowbell 1 Off L/R |
| F05-K03-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K03-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K03-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K03-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K03-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K03-54 | D5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K03-55 | D#5 | ROM 239: Conga Hi-Slap 2 ROM 237: Conga Hi-MtSlap Off L/R |
| F05-K03-56 | E5 | ROM 234: Conga Lo-Slap ROM 232: Conga Lo-Open Off L/R |
| F05-K03-57 | F5 | ROM 286: Timbale Hi-Rim 2 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K03-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K03-59 | G5 | ROM 304: Agogo Bell Off L/R |
| F05-K03-60 | G#5 | ROM 304: Agogo Bell Off L/R |
| F05-K03-61 | A5 | ROM 299: Cabasa-Up Off L/R |
| F05-K03-62 | A#5 | ROM 258: Maracas-Push ROM 259: Maracas-Pull Off L/R |
| F05-K03-63 | B5 | ROM 316: Samba Whistle 5 L/R |
| F05-K03-64 | C6 | ROM 316: Samba Whistle 5 L/R |
| F05-K03-65 | C#6 | ROM 256: Guiro-Short Off L/R |
| F05-K03-66 | D6 | ROM 255: Guiro-Long Off L/R |
| F05-K03-67 | D#6 | ROM 278: Claves Off L/R |
| F05-K03-68 | E6 | ROM 279: Woodblock 1 Off L/R |
| F05-K03-69 | F6 | ROM 280: Woodblock 2 Off L/R |
| F05-K03-70 | F#6 | ROM 295: Cuica-Hi Off L/R |
| F05-K03-71 | G6 | ROM 296: Cuica-Lo Off L/R |
| F05-K03-72 | G#6 | ROM 293: Triangle-Mute 2 L/R |
| F05-K03-73 | A6 | ROM 292: Triangle-Open ROM 292: Triangle-Open 2 L/R |
| F05-K03-74 | A#6 | ROM 298: Shaker 2 ROM 298: Shaker 2 Off L/R |
| F05-K03-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K03-76 | C7 | ROM 174: 88-BD 2(R&B) Off IFX1 |
| F05-K03-77 | C#7 | ROM 187: 88-HHat Closed 3 L/R |
| F05-K03-78 | D7 | ROM 181: 88-SD 1(New) Off IFX1 |
| F05-K03-79 | D#7 | ROM 188: 88-HHat Open 3 L/R |
| F05-K03-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K03-81 | F7 | ROM 195: 88-Cowbell Off IFX1 |
| F05-K03-82 | F#7 | ROM 186: 88-Claps Off IFX1 |
| F05-K03-83 | G7 | ROM 196: 88-Maraca: Off IFX1 |
| F05-K03-84 | G#7 | EX 024: SD-Roll-Closed EX 023: SD-Roll-Open 4 IFX1 |
| F05-K03-85 | A7 | EX 014: SD-Dry 5 f 4 IFX1 |
| F05-K03-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K03-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K03-88 | C8 | ROM 355: Applause Off L/R |

### F05-K04 - Processed Kit (kit 03, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K04-01 | A0 |  |
| F05-K04-02 | A#0 |  |
| F05-K04-03 | B0 |  |
| F05-K04-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K04-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K04-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K04-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off L/R |
| F05-K04-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 7 L/R |
| F05-K04-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K04-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K04-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K04-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K04-13 | A1 | ROM 227: Drumstick Hit ROM 227: Drumstick Hit Off L/R |
| F05-K04-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K04-15 | B1 | ROM 073: SD-Reverse Off IFX 1 |
| F05-K04-16 | C2 | ROM 016: BD-Tight ROM 016: BD-Tight Off IFX 1 |
| F05-K04-17 | C#2 | ROM 017: BD-Squash ROM 017: BD-Squash Off IFX 1 |
| F05-K04-18 | D2 | ROM 015: BD-Gated ROM 015: BD-Gated Off IFX 1 |
| F05-K04-19 | D#2 | ROM 014: BD-Tubby ROM 014: BD-Tubby Off IFX 1 |
| F05-K04-20 | E2 | ROM 011: BD-Woofer ROM 011: BD-Woofer Off IFX 1 |
| F05-K04-21 | F2 | ROM 048: SD-Full Room ROM 048: SD-Full Room Off IFX 1 |
| F05-K04-22 | F#2 | ROM 064: SD-Cracker Room ROM 064: SD-Cracker Room Off IFX 1 |
| F05-K04-23 | G2 | ROM 052: SD-Amb.Piccolo 2 ROM 052: SD-Amb.Piccolo 2 Off IFX 1 |
| F05-K04-24 | G#2 | ROM 062: SD-Processed 1 ROM 062: SD-Processed 1 Off IFX 1 |
| F05-K04-25 | A2 | ROM 061: SD-Yowie ROM 061: SD-Yowie Off IFX 1 |
| F05-K04-26 | A#2 | ROM 059: SD-Atomic ROM 059: SD-Atomic Off IFX 1 |
| F05-K04-27 | B2 | ROM 013: BD-Terminator ROM 013: BD-Terminator Off IFX 1 |
| F05-K04-28 | C3 | ROM 012: BD-Mondo Kill ROM 012: BD-Mondo Kill Off IFX 1 |
| F05-K04-29 | C#3 | ROM 066: Sidestick-Amb ROM 066: Sidestick-Amb Off L/R |
| F05-K04-30 | D3 | ROM 060: SD-Big Rock ROM 060: SD-Big Rock Off IFX 1 |
| F05-K04-31 | D#3 | ROM 229: Hand Claps ROM 229: Hand Claps Off L/R |
| F05-K04-32 | E3 | ROM 063: SD-Processed 2 ROM 063: SD-Processed 2 Off IFX 1 |
| F05-K04-33 | F3 | ROM 109: Tom 3-Floor ROM 109: Tom 3-Floor Off L/R |
| F05-K04-34 | F#3 | ROM 126: H.Hat 2-Closed 2 ROM 125: H.Hat 2-Closed 1 1 L/R |
| F05-K04-35 | G3 | ROM 108: Tom 3-Lo ROM 108: Tom 3-Lo Off L/R |
| F05-K04-36 | G#3 | ROM 128: H.Hat 2-FootOpen ROM 127: H.Hat 2-Foot 1 L/R |
| F05-K04-37 | A3 | ROM 108: Tom 3-Lo ROM 108: Tom 3-Lo Off L/R |
| F05-K04-38 | A#3 | ROM 129: H.Hat 2-Open 1 ROM 130: H.Hat 2-Open 2 1 L/R |
| F05-K04-39 | B3 | ROM 108: Tom 3-Lo ROM 108: Tom 3-Lo Off L/R |
| F05-K04-40 | C4 | ROM 107: Tom 3-Hi ROM 107: Tom 3-Hi Off L/R |
| F05-K04-41 | C#4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K04-42 | D4 | ROM 107: Tom 3-Hi ROM 107: Tom 3-Hi Off L/R |
| F05-K04-43 | D#4 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K04-44 | E4 | ROM 155: China Cymbal ROM 155: China Cymbal Off L/R |
| F05-K04-45 | F4 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K04-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K04-47 | G4 | ROM 156: Splash Cymbal ROM 156: Splash Cymbal Off L/R |
| F05-K04-48 | G#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K04-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K04-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K04-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K04-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K04-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K04-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K04-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K04-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K04-57 | F5 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-58 | F#5 | ROM 285: Timbale Hi-Rim 1 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K04-59 | G5 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-60 | G#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K04-61 | A5 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K04-63 | B5 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-64 | C6 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-65 | C#6 | ROM 301: Cabasa-Tap ROM 299: Cabasa-Up Off L/R |
| F05-K04-66 | D6 | ROM 110: Tom-Processed ROM 110: Tom-Processed Off L/R |
| F05-K04-67 | D#6 | ROM 278: Claves ROM 278: Claves Off L/R |
| F05-K04-68 | E6 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off L/R |
| F05-K04-69 | F6 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off L/R |
| F05-K04-70 | F#6 | ROM 295: Cuica-Hi 2 L/R |
| F05-K04-71 | G6 | ROM 296: Cuica-Lo ROM 296: Cuica-Lo 2 L/R |
| F05-K04-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 3 L/R |
| F05-K04-73 | A6 | ROM 292: Triangle-Open ROM 292: Triangle-Open 3 L/R |
| F05-K04-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K04-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K04-76 | C7 | ROM 174: 88-BD 2(R&B) 4 L/R |
| F05-K04-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 5 L/R |
| F05-K04-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K04-79 | D#7 | ROM 188: 88-HHat Open 5 L/R |
| F05-K04-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K04-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K04-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K04-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K04-84 | G#7 | ROM 171: SD-Orch.Roll 2 ROM 170: SD-Orch.Roll 1 6 L/R |
| F05-K04-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 6 L/R |
| F05-K04-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K04-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K04-88 | C8 | ROM 355: Applause Off L/R |

### F05-K05 - Jazz/Brush Kit (kit 04, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K05-01 | A0 |  |
| F05-K05-02 | A#0 |  |
| F05-K05-03 | B0 |  |
| F05-K05-04 | C1 | ROM 103: Tom 1-Floor ROM 103: Tom 1-Floor Off L/R |
| F05-K05-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K05-06 | D1 | ROM 102: Tom 1-Lo ROM 102: Tom 1-Lo Off L/R |
| F05-K05-07 | D#1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K05-08 | E1 | ROM 101: Tom 1-Hi ROM 101: Tom 1-Hi Off L/R |
| F05-K05-09 | F1 | ROM 101: Tom 1-Hi ROM 101: Tom 1-Hi Off L/R |
| F05-K05-10 | F#1 | ROM 307: Chacha Bell Off L/R |
| F05-K05-11 | G1 | ROM 228: Finger Snap ROM 228: Finger Snap Off L/R |
| F05-K05-12 | G#1 | ROM 229: Hand Claps ROM 229: Hand Claps Off L/R |
| F05-K05-13 | A1 | ROM 227: Drumstick Hit Off L/R |
| F05-K05-14 | A#1 | ROM 014: BD-Tubby ROM 014: BD-Tubby Off IFX 1 |
| F05-K05-15 | B1 | ROM 002: BD-Dry 3 ROM 002: BD-Dry 3 Off IFX 1 |
| F05-K05-16 | C2 | ROM 011: BD-Woofer ROM 011: BD-Woofer Off IFX 1 |
| F05-K05-17 | C#2 | ROM 003: BD-Dry 4 ROM 003: BD-Dry 4 Off IFX 1 |
| F05-K05-18 | D2 | ROM 006: BD-Soft ROM 006: BD-Soft Off IFX 1 |
| F05-K05-19 | D#2 | ROM 008: BD-Jazz 1 ROM 008: BD-Jazz 1 Off IFX 1 |
| F05-K05-20 | E2 | ROM 009: BD-Jazz 2 ROM 009: BD-Jazz 2 Off IFX 1 |
| F05-K05-21 | F2 | ROM 053: SD-Paper ROM 053: SD-Paper Off IFX 1 |
| F05-K05-22 | F#2 | ROM 041: SD-Dry 2 ROM 041: SD-Dry 2 Off IFX 1 |
| F05-K05-23 | G2 | ROM 040: SD-Dry 1 ROM 040: SD-Dry 1 Off IFX 1 |
| F05-K05-24 | G#2 | ROM 051: SD-Amb.Piccolo 1ROM 051: SD-Amb.Piccolo 1 Off IFX 1 |
| F05-K05-25 | A2 | ROM 050: SD-Jazz Ring ROM 050: SD-Jazz Ring Off IFX 1 |
| F05-K05-26 | A#2 | ROM 055: SD-Brush Hit Off IFX 1 |
| F05-K05-27 | B2 | ROM 009: BD-Jazz 2 ROM 009: BD-Jazz 2 Off IFX 1 |
| F05-K05-28 | C3 | ROM 010: BD-Pillow ROM 010: BD-Pillow Off IFX 1 |
| F05-K05-29 | C#3 | ROM 065: Sidestick-Dry ROM 065: Sidestick-Dry Off L/R |
| F05-K05-30 | D3 | ROM 057: SD-Brush Tap 2 Off IFX 1 |
| F05-K05-31 | D#3 | ROM 056: SD-Brush Tap 1 Off IFX 1 |
| F05-K05-32 | E3 | ROM 058: SD-Brush Swirl Off IFX 1 |
| F05-K05-33 | F3 | ROM 115: Tom-Brush Floor ROM 115: Tom-Brush Floor Off L/R |
| F05-K05-34 | F#3 | ROM 119: H.Hat 1-Closed 2 ROM 118: H.Hat 1-Closed 1 1 L/R |
| F05-K05-35 | G3 | ROM 114: Tom-Brush Hi ROM 114: Tom-Brush Hi Off L/R |
| F05-K05-36 | G#3 | ROM 120: H.Hat 1-Foot ROM 120: H.Hat 1-Foot 1 L/R |
| F05-K05-37 | A3 | ROM 114: Tom-Brush Hi ROM 114: Tom-Brush Hi Off L/R |
| F05-K05-38 | A#3 | ROM 122: H.Hat 1-Open 1 ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-K05-39 | B3 | ROM 113: Tom-Jazz Floor ROM 113: Tom-Jazz Floor Off L/R |
| F05-K05-40 | C4 | ROM 112: Tom-Jazz Hi Rim ROM 111: Tom-Jazz Hi Off L/R |
| F05-K05-41 | C#4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K05-42 | D4 | ROM 112: Tom-Jazz Hi Rim ROM 111: Tom-Jazz Hi Off L/R |
| F05-K05-43 | D#4 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K05-44 | E4 | ROM 155: China Cymbal ROM 155: China Cymbal Off L/R |
| F05-K05-45 | F4 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K05-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K05-47 | G4 | ROM 156: Splash Cymbal ROM 156: Splash Cymbal Off L/R |
| F05-K05-48 | G#4 | ROM 305: Cowbell 1 ROM 305: Cowbell 1 Off L/R |
| F05-K05-49 | A4 | ROM 154: Crash Cymbal 3 ROM 154: Crash Cymbal 3 Off L/R |
| F05-K05-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K05-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K05-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K05-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K05-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K05-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K05-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K05-57 | F5 | ROM 285: Timbale Hi-Rim 1 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K05-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K05-59 | G5 | ROM 304: Agogo Bell Off L/R |
| F05-K05-60 | G#5 | ROM 304: Agogo Bell Off L/R |
| F05-K05-61 | A5 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K05-62 | A#5 | ROM 258: Maracas-Push Off L/R |
| F05-K05-63 | B5 | ROM 316: Samba Whistle 2 L/R |
| F05-K05-64 | C6 | ROM 316: Samba Whistle 2 L/R |
| F05-K05-65 | C#6 | ROM 256: Guiro-Short ROM 257: Guiro-Tap 3 L/R |
| F05-K05-66 | D6 | ROM 255: Guiro-Long 3 L/R |
| F05-K05-67 | D#6 | ROM 278: Claves Off L/R |
| F05-K05-68 | E6 | ROM 279: Woodblock 1 Off L/R |
| F05-K05-69 | F6 | ROM 279: Woodblock 1 Off L/R |
| F05-K05-70 | F#6 | ROM 295: Cuica-Hi 4 L/R |
| F05-K05-71 | G6 | ROM 296: Cuica-Lo 4 L/R |
| F05-K05-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 5 L/R |
| F05-K05-73 | A6 | ROM 292: Triangle-Open 5 L/R |
| F05-K05-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K05-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K05-76 | C7 | ROM 174: 88-BD 2(R&B) 6 L/R |
| F05-K05-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 7 L/R |
| F05-K05-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K05-79 | D#7 | ROM 188: 88-HHat Open 7 L/R |
| F05-K05-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K05-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K05-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K05-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K05-84 | G#7 | ROM 171: SD-Orch.Roll 2 ROM 170: SD-Orch.Roll 1 8 L/R |
| F05-K05-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 8 L/R |
| F05-K05-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K05-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K05-88 | C8 | ROM 355: Applause Off L/R |

### F05-K06 - House Kit (kit 05, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K06-01 | A0 |  |
| F05-K06-02 | A#0 |  |
| F05-K06-03 | B0 |  |
| F05-K06-04 | C1 | ROM 315: Flexatone Off L/R |
| F05-K06-05 | C#1 | ROM 257: Guiro-Tap ROM 258: Maracas-Push 6 L/R |
| F05-K06-06 | D1 | ROM 391: Swish Terra ROM 391: Swish Terra Off L/R |
| F05-K06-07 | D#1 | ROM 319: Cyber Drum ROM 319: Cyber Drum Off L/R |
| F05-K06-08 | E1 | ROM 311: Finger Cymbal ROM 311: Finger Cymbal Off L/R |
| F05-K06-09 | F1 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-K06-10 | F#1 | ROM 288: Tambourine-Push ROM 289: Tambourine-Pull Off L/R |
| F05-K06-11 | G1 | ROM 310: Rap Sleighbell Off L/R |
| F05-K06-12 | G#1 | ROM 309: Sleigh Bell ROM 309: Sleigh Bell Off L/R |
| F05-K06-13 | A1 | ROM 312: Marc Tree ROM 312: Marc Tree Off L/R |
| F05-K06-14 | A#1 | ROM 177: 99-BD 2 ROM 177: 99-BD 2 Off IFX 1 |
| F05-K06-15 | B1 | ROM 018: BD-Dance 1 ROM 018: BD-Dance 1 Off IFX 1 |
| F05-K06-16 | C2 | ROM 178: 99-BD 3 ROM 178: 99-BD 3 Off IFX 1 |
| F05-K06-17 | C#2 | ROM 021: BD-Dance 4 ROM 021: BD-Dance 4 Off IFX 1 |
| F05-K06-18 | D2 | ROM 029: BD-Short88 ROM 029: BD-Short88 Off IFX 1 |
| F05-K06-19 | D#2 | ROM 017: BD-Squash ROM 017: BD-Squash Off IFX 1 |
| F05-K06-20 | E2 | ROM 174: 88-BD 2(R&B) ROM 174: 88-BD 2(R&B) Off IFX 1 |
| F05-K06-21 | F2 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off IFX 1 |
| F05-K06-22 | F#2 | ROM 183: 99-SD 1 ROM 183: 99-SD 1 Off IFX 1 |
| F05-K06-23 | G2 | ROM 077: SD-Hip 4 ROM 077: SD-Hip 4 Off IFX 1 |
| F05-K06-24 | G#2 | ROM 072: SD-Sizzle ROM 072: SD-Sizzle Off IFX 1 |
| F05-K06-25 | A2 | ROM 071: SD-Noise Off IFX 1 |
| F05-K06-26 | A#2 | ROM 074: SD-Hip 1 Off IFX 1 |
| F05-K06-27 | B2 | ROM 179: 99-BD 4 ROM 179: 99-BD 4 Off IFX 1 |
| F05-K06-28 | C3 | ROM 176: 99-BD 1 ROM 176: 99-BD 1 Off IFX 1 |
| F05-K06-29 | C#3 | ROM 185: 88-Rimshot ROM 185: 88-Rimshot Off L/R |
| F05-K06-30 | D3 | ROM 182: 88-SD 2(Old) ROM 182: 88-SD 2(Old) Off IFX 1 |
| F05-K06-31 | D#3 | ROM 186: 88-Claps ROM 186: 88-Claps Off L/R |
| F05-K06-32 | E3 | ROM 184: 99-SD 2 ROM 184: 99-SD 2 Off IFX 1 |
| F05-K06-33 | F3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K06-34 | F#3 | ROM 189: 99-HHat Closed ROM 189: 99-HHat Closed 1 L/R |
| F05-K06-35 | G3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K06-36 | G#3 | ROM 190: 99-HHat Open ROM 190: 99-HHat Open 1 L/R |
| F05-K06-37 | A3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K06-38 | A#3 | ROM 190: 99-HHat Open ROM 190: 99-HHat Open 1 L/R |
| F05-K06-39 | B3 | ROM 150: H.Hat-Whispy Off L/R |
| F05-K06-40 | C4 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 2 L/R |
| F05-K06-41 | C#4 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K06-42 | D4 | ROM 188: 88-HHat Open ROM 188: 88-HHat Open 2 L/R |
| F05-K06-43 | D#4 | ROM 160: Ride-Dance 99 ROM 160: Ride-Dance 99 Off L/R |
| F05-K06-44 | E4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K06-45 | F4 | ROM 161: Ride-Dance DDD1 ROM 161: Ride-Dance DDD1 Off L/R |
| F05-K06-46 | F#4 | ROM 291: Tambourine-Acc.2 ROM 290: Tambourine-Acc.1 Off L/R |
| F05-K06-47 | G4 | ROM 156: Splash Cymbal ROM 156: Splash Cymbal Off L/R |
| F05-K06-48 | G#4 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K06-49 | A4 | ROM 158: Crash-Dance 99 ROM 158: Crash-Dance 99 Off L/R |
| F05-K06-50 | A#4 | ROM 207: Zap 3 ROM 207: Zap 3 Off L/R |
| F05-K06-51 | B4 | ROM 157: Cymbal Reverse ROM 157: Cymbal Reverse Off L/R |
| F05-K06-52 | C5 | ROM 205: Zap 1 ROM 205: Zap 1 Off L/R |
| F05-K06-53 | C#5 | ROM 206: Zap 2 ROM 206: Zap 2 Off L/R |
| F05-K06-54 | D5 | ROM 194: 88-Clave Off L/R |
| F05-K06-55 | D#5 | ROM 193: 88-Conga ROM 193: 88-Conga Off L/R |
| F05-K06-56 | E5 | ROM 193: 88-Conga ROM 193: 88-Conga Off L/R |
| F05-K06-57 | F5 | ROM 324: Metal Hit Off L/R |
| F05-K06-58 | F#5 | ROM 132: H.Hat-Chili Off L/R |
| F05-K06-59 | G5 | ROM 323: Industry Loop 2 Off L/R |
| F05-K06-60 | G#5 | ROM 127: H.Hat 2-Foot Off L/R |
| F05-K06-61 | A5 | ROM 204: Dance Perc-Hous2 Off L/R |
| F05-K06-62 | A#5 | ROM 196: 88-Maraca Off L/R |
| F05-K06-63 | B5 | ROM 199: Vocal SD ROM 199: Vocal SD Off L/R |
| F05-K06-64 | C6 | ROM 377: Car Crash Off L/R |
| F05-K06-65 | C#6 | ROM 293: Triangle-Mute 3 L/R |
| F05-K06-66 | D6 | ROM 395: Noise-White Off L/R |
| F05-K06-67 | D#6 | ROM 292: Triangle-Open 3 L/R |
| F05-K06-68 | E6 | ROM 400: GtCutNois1 Off L/R |
| F05-K06-69 | F6 | ROM 146: H.Hat-Hip 2 Off L/R |
| F05-K06-70 | F#6 | ROM 151: H.Hat-Alpo CloseROM 151: H.Hat-Alpo Close 4 L/R |
| F05-K06-71 | G6 | ROM 402: StringSlap Off L/R |
| F05-K06-72 | G#6 | ROM 147: H.Hat-AmbCrackle Off L/R |
| F05-K06-73 | A6 | ROM 406: Dist.Slide 1 Off L/R |
| F05-K06-74 | A#6 | ROM 135: H.Hat-Crisp Open 4 L/R |
| F05-K06-75 | B6 | ROM 391: Swish Terra ROM 391: Swish Terra Off L/R |
| F05-K06-76 | C7 | ROM 391: Swish Terra Off L/R |
| F05-K06-77 | C#7 | ROM 152: Crash Cymbal 1 ROM 152: Crash Cymbal 1 Off L/R |
| F05-K06-78 | D7 | ROM 155: China Cymbal ROM 155: China Cymbal Off L/R |
| F05-K06-79 | D#7 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K06-80 | E7 | ROM 238: Conga Hi-Slap 1 ROM 242: Bongo Lo-Open Off L/R |
| F05-K06-81 | F7 | ROM 235: Conga Hi-Open ROM 233: Conga Lo-MtSlap Off L/R |
| F05-K06-82 | F#7 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K06-83 | G7 | ROM 239: Conga Hi-Slap 2 ROM 236: Conga Hi-Mute Off L/R |
| F05-K06-84 | G#7 | ROM 305: Cowbell 1 ROM 305: Cowbell 1 Off L/R |
| F05-K06-85 | A7 | ROM 307: Chacha Bell ROM 307: Chacha Bell Off L/R |
| F05-K06-86 | A#7 | ROM 305: Cowbell 1 ROM 305: Cowbell 1 Off L/R |
| F05-K06-87 | B7 | ROM 256: Guiro-Short 6 L/R |
| F05-K06-88 | C8 | ROM 257: Guiro-Tap ROM 257: Guiro-Tap 6 L/R |
| F05-K06-89 | C#8 | ROM 324: Metal Hit Off L/R |
| F05-K06-90 | D8 | ROM 202: Dance Perc-Hip Off L/R |

### F05-K07 - Tricky Kit (kit 06, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K07-01 | A0 |  |
| F05-K07-02 | A#0 |  |
| F05-K07-03 | B0 |  |
| F05-K07-04 | C1 | ROM 325: Yea Solo ROM 326: Year solo Off L/R |
| F05-K07-05 | C#1 | ROM 329: Uhhhh Solo ROM 328: Hit It Off L/R |
| F05-K07-06 | D1 | ROM 327: Uhh Solo Off L/R |
| F05-K07-07 | D#1 | ROM 335: CompuVoice-WhenROM 334: CompuVoice-Do it Off L/R |
| F05-K07-08 | E1 | ROM 331: CompuVoice-NoiseROM 330: CompuVoice-Beep 9 L/R |
| F05-K07-09 | F1 | ROM 332: CompuVoice-Chi ROM 333: CompuVoice-Test Off L/R |
| F05-K07-10 | F#1 | ROM 337: CompuVoice-Ti ROM 336: CompuVoice-Toi Off L/R |
| F05-K07-11 | G1 | ROM 343: PC Vox-Dunc ROM 342: PC Vox-Sis Off L/R |
| F05-K07-12 | G#1 | ROM 348: PC Vox-Zee ROM 345: PC Vox-For Off L/R |
| F05-K07-13 | A1 | ROM 349: PC Vox-Ae ROM 350: PC Vox-Pa Off L/R |
| F05-K07-14 | A#1 | ROM 350: PC Vox-Pa ROM 351: PC Vox-Sh Off L/R |
| F05-K07-15 | B1 | ROM 352: PC Vox-O ROM 353: PC Vox-Tu Off L/R |
| F05-K07-16 | C2 | ROM 174: 88-BD 2(R&B) ROM 174: 88-BD 2(R&B) Off IFX 1 |
| F05-K07-17 | C#2 | ROM 026: BD-Dark ROM 180: 99-BD 5 Off IFX 1 |
| F05-K07-18 | D2 | ROM 022: BD-Hip 1 ROM 023: BD-Hip 2 Off IFX 1 |
| F05-K07-19 | D#2 | ROM 180: 99-BD 5 ROM 180: 99-BD 5 Off IFX 1 |
| F05-K07-20 | E2 | ROM 034: BD-Dance Soft ROM 029: BD-Short88 Off IFX 1 |
| F05-K07-21 | F2 | ROM 091: SD-AmbiHop ROM 091: SD-AmbiHop Off IFX 1 |
| F05-K07-22 | F#2 | ROM 098: SD-Ambi Hit 1 Off IFX 1 |
| F05-K07-23 | G2 | ROM 092: SD-Brasser Off IFX 1 |
| F05-K07-24 | G#2 | ROM 097: SD-Ambi Pop ROM 088: SD-AmbiCrackle 1 Off IFX 1 |
| F05-K07-25 | A2 | ROM 096: SD-Dry Present Off IFX 1 |
| F05-K07-26 | A#2 | ROM 087: SD-Vintage 6 ROM 087: SD-Vintage 6 Off IFX 1 |
| F05-K07-27 | B2 | ROM 005: BD-Dry 6 ROM 026: BD-Dark Off IFX 1 |
| F05-K07-28 | C3 | ROM 027: BD-Pop ROM 029: BD-Short88 Off IFX 1 |
| F05-K07-29 | C#3 | ROM 066: Sidestick-Amb Off IFX 1 |
| F05-K07-30 | D3 | ROM 082: SD-Vintage 1 ROM 082: SD-Vintage 1 Off IFX 1 |
| F05-K07-31 | D#3 | ROM 230: Hand Claps-Hip ROM 231: Hand Claps-Happy Off L/R |
| F05-K07-32 | E3 | ROM 079: SD-Hip 6 Off IFX 1 |
| F05-K07-33 | F3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K07-34 | F#3 | ROM 139: H.Hat-Tight 1 ROM 140: H.Hat-Tight 2 1 L/R |
| F05-K07-35 | G3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K07-36 | G#3 | ROM 142: H.Hat-Old Cl 2 ROM 145: H.Hat-Hip 1 1 L/R |
| F05-K07-37 | A3 | ROM 192: 88-Tom ROM 192: 88-Tom Off L/R |
| F05-K07-38 | A#3 | ROM 149: H.Hat-ClangyOpenROM 135: H.Hat-Crisp Open 1 L/R |
| F05-K07-39 | B3 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K07-40 | C4 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K07-41 | C#4 | ROM 396: Noise-FM Mod Off L/R |
| F05-K07-42 | D4 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K07-43 | D#4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K07-44 | E4 | ROM 191: 88-Crash Off L/R |
| F05-K07-45 | F4 | ROM 165: Ride-Cup Off L/R |
| F05-K07-46 | F#4 | ROM 290: Tambourine-Acc.1 Off L/R |
| F05-K07-47 | G4 | ROM 159: Crash-Dance DDD1 Off L/R |
| F05-K07-48 | G#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K07-49 | A4 | ROM 383: Explosion Off L/R |
| F05-K07-50 | A#4 | ROM 388: ChurchBell Off L/R |
| F05-K07-51 | B4 | ROM 366: Crickets 2 ROM 366: Crickets 2 Off L/R |
| F05-K07-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K07-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K07-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K07-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K07-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K07-57 | F5 | ROM 310: Rap Sleighbell ROM 310: Rap Sleighbell Off L/R |
| F05-K07-58 | F#5 | ROM 217: DJ-BD Rub Off L/R |
| F05-K07-59 | G5 | ROM 218: DJ-SD Rub Off L/R |
| F05-K07-60 | G#5 | ROM 219: DJ-Record Stop ROM 220: DJ-Reverse Off L/R |
| F05-K07-61 | A5 | ROM 299: Cabasa-Up Off L/R |
| F05-K07-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K07-63 | B5 | ROM 196: 88-Maraca Off L/R |
| F05-K07-64 | C6 | ROM 320: SD-Metal Striker Off L/R |
| F05-K07-65 | C#6 | ROM 374: Car Engine Off L/R |
| F05-K07-66 | D6 | ROM 323: Industry Loop 2 Off L/R |
| F05-K07-67 | D#6 | ROM 323: Industry Loop 2 Off L/R |
| F05-K07-68 | E6 | ROM 375: Car Stop Off L/R |
| F05-K07-69 | F6 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-K07-70 | F#6 | ROM 323: Industry Loop 2 Off L/R |
| F05-K07-71 | G6 | ROM 402: StringSlap 4 L/R |
| F05-K07-72 | G#6 | ROM 202: Dance Perc-Hip 5 L/R |
| F05-K07-73 | A6 | ROM 205: Zap 1 Off L/R |
| F05-K07-74 | A#6 | ROM 387: Bubble Off L/R |
| F05-K07-75 | B6 | ROM 377: Car Crash Off L/R |
| F05-K07-76 | C7 | ROM 173: 88-BD 1(Rap) Off L/R |
| F05-K07-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed Off L/R |
| F05-K07-78 | D7 | ROM 182: 88-SD 2(Old) ROM 182: 88-SD 2(Old) Off L/R |
| F05-K07-79 | D#7 | ROM 188: 88-HHat Open Off L/R |
| F05-K07-80 | E7 | ROM 317: Chinese Gong Off L/R |
| F05-K07-81 | F7 | ROM 194: 88-Clave ROM 194: 88-Clave Off L/R |
| F05-K07-82 | F#7 | ROM 385: Wind Off L/R |
| F05-K07-83 | G7 | ROM 386: Stream Off L/R |
| F05-K07-84 | G#7 | ROM 340: Two... Solo ROM 341: One... Solo Off L/R |
| F05-K07-85 | A7 | ROM 338: Four... Solo ROM 339: Three... Solo Off L/R |
| F05-K07-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K07-87 | B7 | ROM 390: Space Lore Off L/R |
| F05-K07-88 | C8 | ROM 367: Hart Beat Off L/R |

### F05-K08 - Drum'n'Bass Kit (kit 07, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K08-01 | A0 |  |
| F05-K08-02 | A#0 |  |
| F05-K08-03 | B0 |  |
| F05-K08-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K08-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K08-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K08-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off L/R |
| F05-K08-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 10 L/R |
| F05-K08-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K08-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K08-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K08-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K08-13 | A1 | ROM 227: Drumstick Hit Off L/R |
| F05-K08-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K08-15 | B1 | ROM 073: SD-Reverse Off IFX 1 |
| F05-K08-16 | C2 | ROM 037: BD-PhatJaz ROM 037: BD-PhatJaz Off IFX 1 |
| F05-K08-17 | C#2 | ROM 014: BD-Tubby Off IFX 1 |
| F05-K08-18 | D2 | ROM 005: BD-Dry 6 ROM 005: BD-Dry 6 Off IFX 1 |
| F05-K08-19 | D#2 | ROM 039: BD-Klanger ROM 022: BD-Hip 1 Off IFX 1 |
| F05-K08-20 | E2 | ROM 010: BD-Pillow ROM 010: BD-Pillow Off IFX 1 |
| F05-K08-21 | F2 | ROM 100: SD-Ambi Hit 3 ROM 100: SD-Ambi Hit 3 Off IFX 1 |
| F05-K08-22 | F#2 | ROM 076: SD-Hip 3 ROM 076: SD-Hip 3 Off IFX 1 |
| F05-K08-23 | G2 | ROM 094: SD-Klanger ROM 051: SD-Amb.Piccolo 1 Off IFX 1 |
| F05-K08-24 | G#2 | ROM 080: SD-Ringy ROM 080: SD-Ringy Off IFX 1 |
| F05-K08-25 | A2 | ROM 093: SD-Chili ROM 093: SD-Chili Off IFX 1 |
| F05-K08-26 | A#2 | ROM 040: SD-Dry 1 ROM 040: SD-Dry 1 Off IFX 1 |
| F05-K08-27 | B2 | ROM 028: BD-Ringy ROM 029: BD-Short88 Off IFX 1 |
| F05-K08-28 | C3 | ROM 032: BD-Ambi Crackle ROM 031: BD-Ambi Off IFX 1 |
| F05-K08-29 | C#3 | ROM 065: Sidestick-Dry ROM 185: 88-Rimshot Off L/R |
| F05-K08-30 | D3 | ROM 093: SD-Chili ROM 093: SD-Chili Off IFX 1 |
| F05-K08-31 | D#3 | ROM 231: Hand Claps-HappyROM 230: Hand Claps-Hip Off L/R |
| F05-K08-32 | E3 | ROM 090: SD-AmbiCrackle 3 ROM 090: SD-AmbiCrackle 3 Off IFX 1 |
| F05-K08-33 | F3 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K08-34 | F#3 | ROM 134: H.Hat-Crisp Cl 2 ROM 134: H.Hat-Crisp Cl 2 1 L/R |
| F05-K08-35 | G3 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K08-36 | G#3 | ROM 119: H.Hat 1-Closed 2 ROM 119: H.Hat 1-Closed 2 1 L/R |
| F05-K08-37 | A3 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K08-38 | A#3 | ROM 135: H.Hat-Crisp Open ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-K08-39 | B3 | ROM 137: H.Hat-Vintage 1 ROM 137: H.Hat-Vintage 1 Off L/R |
| F05-K08-40 | C4 | ROM 147: H.Hat-AmbCrackleROM 147: H.Hat-AmbCrackle Off L/R |
| F05-K08-41 | C#4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K08-42 | D4 | ROM 144: H.Hat-Old Open ROM 144: H.Hat-Old Open Off L/R |
| F05-K08-43 | D#4 | ROM 162: Ride-Edge 1 ROM 162: Ride-Edge 1 Off L/R |
| F05-K08-44 | E4 | ROM 396: Noise-FM Mod ROM 393: Cricket Spectrum Off L/R |
| F05-K08-45 | F4 | ROM 165: Ride-Cup ROM 165: Ride-Cup Off L/R |
| F05-K08-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K08-47 | G4 | ROM 291: Tambourine-Acc.2 ROM 289: Tambourine-Pull Off L/R |
| F05-K08-48 | G#4 | ROM 146: H.Hat-Hip 2 Off L/R |
| F05-K08-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K08-50 | A#4 | ROM 381: MachineGun ROM 381: MachineGun Off L/R |
| F05-K08-51 | B4 | ROM 161: Ride-Dance DDD1ROM 161: Ride-Dance DDD1 Off L/R |
| F05-K08-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K08-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K08-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K08-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K08-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K08-57 | F5 | ROM 285: Timbale Hi-Rim 1 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K08-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K08-59 | G5 | ROM 202: Dance Perc-Hip Off L/R |
| F05-K08-60 | G#5 | ROM 202: Dance Perc-Hip Off L/R |
| F05-K08-61 | A5 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K08-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K08-63 | B5 | ROM 045: SD-GhostSet f ROM 045: SD-GhostSet f 9 IFX 1 |
| F05-K08-64 | C6 | ROM 044: SD-GhostSet roll ROM 044: SD-GhostSet roll 9 IFX 1 |
| F05-K08-65 | C#6 | ROM 378: Train 2 L/R |
| F05-K08-66 | D6 | ROM 390: Space Lore ROM 390: Space Lore 3 L/R |
| F05-K08-67 | D#6 | ROM 396: Noise-FM Mod Off L/R |
| F05-K08-68 | E6 | ROM 394: Air Vortex Off L/R |
| F05-K08-69 | F6 | ROM 393: Cricket Spectrum ROM 393: Cricket Spectrum Off L/R |
| F05-K08-70 | F#6 | ROM 374: Car Engine 4 L/R |
| F05-K08-71 | G6 | ROM 373: Door Slam 4 L/R |
| F05-K08-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 5 L/R |
| F05-K08-73 | A6 | ROM 292: Triangle-Open 5 L/R |
| F05-K08-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K08-75 | B6 | ROM 314: Bell Tree Off L/R |
| F05-K08-76 | C7 | ROM 174: 88-BD 2(R&B) 6 L/R |
| F05-K08-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 7 L/R |
| F05-K08-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K08-79 | D#7 | ROM 188: 88-HHat Open 7 L/R |
| F05-K08-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K08-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K08-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K08-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K08-84 | G#7 | ROM 171: SD-Orch.Roll 2 8 L/R |
| F05-K08-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 8 L/R |
| F05-K08-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K08-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K08-88 | C8 | ROM 355: Applause Off L/R |

### F05-K09 - Hip Hop Kit (kit 08, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K09-01 | A0 |  |
| F05-K09-02 | A#0 |  |
| F05-K09-03 | B0 |  |
| F05-K09-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K09-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K09-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K09-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off L/R |
| F05-K09-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 9 L/R |
| F05-K09-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K09-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K09-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K09-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K09-13 | A1 | ROM 227: Drumstick Hit Off L/R |
| F05-K09-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K09-15 | B1 | ROM 073: SD-Reverse Off IFX 1 |
| F05-K09-16 | C2 | ROM 037: BD-PhatJaz Off IFX 1 |
| F05-K09-17 | C#2 | ROM 022: BD-Hip 1 Off IFX 1 |
| F05-K09-18 | D2 | ROM 021: BD-Dance 4 ROM 021: BD-Dance 4 Off IFX 1 |
| F05-K09-19 | D#2 | ROM 035: BD-AmbiRocker ROM 035: BD-AmbiRocker Off IFX 1 |
| F05-K09-20 | E2 | ROM 180: 99-BD 5 Off IFX 1 |
| F05-K09-21 | F2 | ROM 086: SD-Vintage 5 Off IFX 1 |
| F05-K09-22 | F#2 | ROM 098: SD-Ambi Hit 1 Off IFX 1 |
| F05-K09-23 | G2 | ROM 089: SD-AmbiCrackle 2 Off IFX 1 |
| F05-K09-24 | G#2 | ROM 083: SD-Vintage 2 Off IFX 1 |
| F05-K09-25 | A2 | ROM 076: SD-Hip 3 Off IFX 1 |
| F05-K09-26 | A#2 | ROM 067: SD-Amb.House Off IFX 1 |
| F05-K09-27 | B2 | ROM 175: 88-BD 3 Off IFX 1 |
| F05-K09-28 | C3 | ROM 031: BD-Ambi Off IFX 1 |
| F05-K09-29 | C#3 | ROM 065: Sidestick-Dry Off L/R |
| F05-K09-30 | D3 | ROM 095: SD-Whopper Off IFX 1 |
| F05-K09-31 | D#3 | ROM 231: Hand Claps-Happy Off L/R |
| F05-K09-32 | E3 | ROM 082: SD-Vintage 1 Off IFX 1 |
| F05-K09-33 | F3 | ROM 116: Tom-Dirty Funk ROM 116: Tom-Dirty Funk Off L/R |
| F05-K09-34 | F#3 | ROM 147: H.Hat-AmbCrackle 1 L/R |
| F05-K09-35 | G3 | ROM 116: Tom-Dirty Funk ROM 116: Tom-Dirty Funk Off L/R |
| F05-K09-36 | G#3 | ROM 142: H.Hat-Old Cl 2 1 L/R |
| F05-K09-37 | A3 | ROM 116: Tom-Dirty Funk ROM 116: Tom-Dirty Funk Off L/R |
| F05-K09-38 | A#3 | ROM 135: H.Hat-Crisp Open 1 L/R |
| F05-K09-39 | B3 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K09-40 | C4 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K09-41 | C#4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K09-42 | D4 | ROM 117: Tom-Old R&B ROM 117: Tom-Old R&B Off L/R |
| F05-K09-43 | D#4 | ROM 162: Ride-Edge 1 Off L/R |
| F05-K09-44 | E4 | ROM 155: China Cymbal Off L/R |
| F05-K09-45 | F4 | ROM 165: Ride-Cup Off L/R |
| F05-K09-46 | F#4 | ROM 290: Tambourine-Acc.1 Off L/R |
| F05-K09-47 | G4 | ROM 156: Splash Cymbal Off L/R |
| F05-K09-48 | G#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K09-49 | A4 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-K09-50 | A#4 | ROM 277: Vibraslap Off L/R |
| F05-K09-51 | B4 | ROM 163: Ride-Edge 2 ROM 163: Ride-Edge 2 Off L/R |
| F05-K09-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K09-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K09-54 | D5 | ROM 237: Conga Hi-MtSlap ROM 241: Conga-Toe Off L/R |
| F05-K09-55 | D#5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K09-56 | E5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K09-57 | F5 | ROM 285: Timbale Hi-Rim 1 ROM 284: Timbale Hi-Edge Off L/R |
| F05-K09-58 | F#5 | ROM 281: Timbale Lo-Open ROM 281: Timbale Lo-Open Off L/R |
| F05-K09-59 | G5 | ROM 304: Agogo Bell Off L/R |
| F05-K09-60 | G#5 | ROM 304: Agogo Bell Off L/R |
| F05-K09-61 | A5 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K09-62 | A#5 | ROM 258: Maracas-Push ROM 258: Maracas-Push Off L/R |
| F05-K09-63 | B5 | ROM 316: Samba Whistle 2 L/R |
| F05-K09-64 | C6 | ROM 316: Samba Whistle 2 L/R |
| F05-K09-65 | C#6 | ROM 256: Guiro-Short ROM 257: Guiro-Tap 3 L/R |
| F05-K09-66 | D6 | ROM 255: Guiro-Long 3 L/R |
| F05-K09-67 | D#6 | ROM 278: Claves Off L/R |
| F05-K09-68 | E6 | ROM 279: Woodblock 1 Off L/R |
| F05-K09-69 | F6 | ROM 279: Woodblock 1 Off L/R |
| F05-K09-70 | F#6 | ROM 295: Cuica-Hi 4 L/R |
| F05-K09-71 | G6 | ROM 296: Cuica-Lo 4 L/R |
| F05-K09-72 | G#6 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 5 L/R |
| F05-K09-73 | A6 | ROM 292: Triangle-Open 5 L/R |
| F05-K09-74 | A#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K09-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K09-76 | C7 | ROM 174: 88-BD 2(R&B) 6 L/R |
| F05-K09-77 | C#7 | ROM 187: 88-HHat Closed ROM 187: 88-HHat Closed 7 L/R |
| F05-K09-78 | D7 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off L/R |
| F05-K09-79 | D#7 | ROM 188: 88-HHat Open 7 L/R |
| F05-K09-80 | E7 | ROM 191: 88-Crash ROM 191: 88-Crash Off L/R |
| F05-K09-81 | F7 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off L/R |
| F05-K09-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K09-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K09-84 | G#7 | ROM 171: SD-Orch.Roll 2 8 L/R |
| F05-K09-85 | A7 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 8 L/R |
| F05-K09-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K09-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K09-88 | C8 | ROM 355: Applause Off L/R |

### F05-K10 - Psycho Kit (kit 09, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K10-01 | A0 |  |
| F05-K10-02 | A#0 |  |
| F05-K10-03 | B0 |  |
| F05-K10-04 | C1 | ROM 317: Chinese Gong ROM 317: Chinese Gong Off L/R |
| F05-K10-05 | C#1 | ROM 294: Triangle-Roll ROM 294: Triangle-Roll Off IFX 1 |
| F05-K10-06 | D1 | ROM 315: Flexatone ROM 315: Flexatone Off IFX 1 |
| F05-K10-07 | D#1 | ROM 274: Taiko-Open ROM 274: Taiko-Open Off L/R |
| F05-K10-08 | E1 | ROM 263: Baya-Mute 2 ROM 263: Baya-Mute 2 Off L/R |
| F05-K10-09 | F1 | ROM 072: SD-Sizzle ROM 072: SD-Sizzle Off IFX 1 |
| F05-K10-10 | F#1 | ROM 058: SD-Brush Swirl ROM 058: SD-Brush Swirl Off IFX 1 |
| F05-K10-11 | G1 | ROM 070: SD-Rap 2 ROM 070: SD-Rap 2 Off IFX 1 |
| F05-K10-12 | G#1 | ROM 073: SD-Reverse ROM 073: SD-Reverse 4 IFX 1 |
| F05-K10-13 | A1 | ROM 015: BD-Gated ROM 015: BD-Gated Off L/R |
| F05-K10-14 | A#1 | ROM 157: Cymbal Reverse ROM 157: Cymbal Reverse 5 IFX 1 |
| F05-K10-15 | B1 | ROM 013: BD-Terminator ROM 013: BD-Terminator Off L/R |
| F05-K10-16 | C2 | ROM 017: BD-Squash ROM 017: BD-Squash Off L/R |
| F05-K10-17 | C#2 | ROM 009: BD-Jazz 2 ROM 009: BD-Jazz 2 Off L/R |
| F05-K10-18 | D2 | ROM 205: Zap 1 ROM 205: Zap 1 Off L/R |
| F05-K10-19 | D#2 | ROM 358: Footstep 1 ROM 359: Footstep 2 Off L/R |
| F05-K10-20 | E2 | ROM 319: Cyber Drum ROM 319: Cyber Drum Off L/R |
| F05-K10-21 | F2 | ROM 267: Tabla-Na ROM 268: Tabla-Open Off IFX 1 |
| F05-K10-22 | F#2 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 1 IFX 1 |
| F05-K10-23 | G2 | ROM 070: SD-Rap 2 ROM 070: SD-Rap 2 Off IFX 1 |
| F05-K10-24 | G#2 | ROM 068: SD-Dance ROM 068: SD-Dance 1 IFX 1 |
| F05-K10-25 | A2 | ROM 052: SD-Amb.Piccolo 2ROM 049: SD-Off Center Off IFX 1 |
| F05-K10-26 | A#2 | ROM 059: SD-Atomic ROM 059: SD-Atomic Off IFX 1 |
| F05-K10-27 | B2 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off L/R |
| F05-K10-28 | C3 | ROM 019: BD-Dance 2 ROM 019: BD-Dance 2 Off L/R |
| F05-K10-29 | C#3 | ROM 285: Timbale Hi-Rim 1ROM 285: Timbale Hi-Rim 1 Off IFX 1 |
| F05-K10-30 | D3 | ROM 050: SD-Jazz Ring ROM 050: SD-Jazz Ring Off IFX 1 |
| F05-K10-31 | D#3 | ROM 207: Zap 3 ROM 207: Zap 3 Off IFX 1 |
| F05-K10-32 | E3 | ROM 182: 88-SD 2(Old) ROM 182: 88-SD 2(Old) Off IFX 1 |
| F05-K10-33 | F3 | ROM 105: Tom 2-Lo ROM 105: Tom 2-Lo Off IFX 1 |
| F05-K10-34 | F#3 | ROM 189: 99-HHat Closed ROM 189: 99-HHat Closed 2 L/R |
| F05-K10-35 | G3 | ROM 115: Tom-Brush Floor ROM 115: Tom-Brush Floor Off IFX 1 |
| F05-K10-36 | G#3 | ROM 188: 88-HHat Open ROM 188: 88-HHat Open 2 L/R |
| F05-K10-37 | A3 | ROM 198: Real E.Tom ROM 198: Real E.Tom Off IFX 1 |
| F05-K10-38 | A#3 | ROM 292: Triangle-Open ROM 190: 99-HHat Open 2 L/R |
| F05-K10-39 | B3 | ROM 198: Real E.Tom Off IFX 1 |
| F05-K10-40 | C4 | ROM 111: Tom-Jazz Hi Off IFX 1 |
| F05-K10-41 | C#4 | ROM 168: Orch Cym.-Open ROM 154: Crash Cymbal 3 Off IFX 1 |
| F05-K10-42 | D4 | ROM 108: Tom 3-Lo Off IFX 1 |
| F05-K10-43 | D#4 | ROM 164: Ride-Jazz ROM 163: Ride-Edge 2 Off IFX 1 |
| F05-K10-44 | E4 | ROM 394: Air Vortex ROM 394: Air Vortex Off IFX 1 |
| F05-K10-45 | F4 | ROM 219: DJ-Record Stop ROM 208: DJ-Scratch 1 Off IFX 1 |
| F05-K10-46 | F#4 | ROM 219: DJ-Record Stop ROM 208: DJ-Scratch 1 Off L/R |
| F05-K10-47 | G4 | ROM 221: DJ-Old Record ROM 221: DJ-Old Record Off L/R |
| F05-K10-48 | G#4 | ROM 406: Dist.Slide 1 ROM 407: Dist.Slide 2 Off IFX 1 |
| F05-K10-49 | A4 | ROM 168: Orch Cym.-Open ROM 168: Orch Cym.-Open Off L/R |
| F05-K10-50 | A#4 | ROM 155: China Cymbal ROM 154: Crash Cymbal 3 Off IFX 1 |
| F05-K10-51 | B4 | ROM 309: Sleigh Bell ROM 309: Sleigh Bell Off L/R |
| F05-K10-52 | C5 | ROM 277: Vibraslap ROM 277: Vibraslap Off IFX 1 |
| F05-K10-53 | C#5 | ROM 170: SD-Orch.Roll 1 ROM 170: SD-Orch.Roll 1 Off IFX 1 |
| F05-K10-54 | D5 | ROM 305: Cowbell 1 ROM 308: Mambo Bell Off IFX 1 |
| F05-K10-55 | D#5 | ROM 291: Tambourine-Acc.2 ROM 302: Caxixi-Hard Off L/R |
| F05-K10-56 | E5 | ROM 390: Space Lore ROM 390: Space Lore Off IFX 1 |
| F05-K10-57 | F5 | ROM 391: Swish Terra ROM 391: Swish Terra Off IFX 1 |
| F05-K10-58 | F#5 | ROM 292: Triangle-Open ROM 292: Triangle-Open Off IFX 1 |
| F05-K10-59 | G5 | ROM 392: Xylophone Spectr ROM 392: Xylophone Spectr Off IFX 1 |
| F05-K10-60 | G#5 | ROM 290: Tambourine-Acc.1 ROM 289: Tambourine-Pull Off L/R |
| F05-K10-61 | A5 | ROM 319: Cyber Drum ROM 394: Air Vortex Off IFX 1 |
| F05-K10-62 | A#5 | ROM 397: Tubular ROM 397: Tubular Off IFX 1 |
| F05-K10-63 | B5 | ROM 311: Finger Cymbal ROM 311: Finger Cymbal Off IFX 1 |
| F05-K10-64 | C6 | ROM 255: Guiro-Long ROM 255: Guiro-Long Off IFX 1 |
| F05-K10-65 | C#6 | ROM 199: Vocal SD ROM 199: Vocal SD Off L/R |
| F05-K10-66 | D6 | ROM 274: Taiko-Open ROM 398: Gamelan Off IFX 1 |
| F05-K10-67 | D#6 | ROM 224: Band Hit ROM 224: Band Hit Off IFX 1 |
| F05-K10-68 | E6 | ROM 389: Telephone Ring ROM 389: Telephone Ring Off IFX 1 |
| F05-K10-69 | F6 | ROM 358: Footstep 1 ROM 359: Footstep 2 Off L/R |
| F05-K10-70 | F#6 | ROM 205: Zap 1 ROM 205: Zap 1 Off IFX 1 |
| F05-K10-71 | G6 | ROM 393: Cricket Spectrum ROM 393: Cricket Spectrum Off IFX 1 |
| F05-K10-72 | G#6 | ROM 396: Noise-FM Mod ROM 396: Noise-FM Mod Off L/R |
| F05-K10-73 | A6 | ROM 395: Noise-White ROM 395: Noise-White Off IFX 1 |
| F05-K10-74 | A#6 | ROM 396: Noise-FM Mod ROM 396: Noise-FM Mod Off L/R |
| F05-K10-75 | B6 | ROM 304: Agogo Bell ROM 304: Agogo Bell Off IFX 1 |
| F05-K10-76 | C7 | ROM 370: GunShot ROM 370: GunShot Off IFX 1 |
| F05-K10-77 | C#7 | ROM 311: Finger Cymbal ROM 312: Marc Tree Off IFX 1 |
| F05-K10-78 | D7 | ROM 316: Samba Whistle ROM 316: Samba Whistle 3 IFX 1 |
| F05-K10-79 | D#7 | ROM 323: Industry Loop 2 ROM 320: SD-Metal Striker Off IFX 1 |
| F05-K10-80 | E7 | ROM 217: DJ-BD Rub ROM 217: DJ-BD Rub Off L/R |
| F05-K10-81 | F7 | ROM 318: Industry ROM 318: Industry Off IFX 1 |
| F05-K10-82 | F#7 | ROM 407: Dist.Slide 2 ROM 406: Dist.Slide 1 Off L/R |
| F05-K10-83 | G7 | ROM 403: HarpGliss ROM 371: Rainstick Off L/R |
| F05-K10-84 | G#7 | ROM 253: Castanet-Dble 1 ROM 277: Vibraslap Off L/R |
| F05-K10-85 | A7 | ROM 219: DJ-Record Stop ROM 220: DJ-Reverse Off L/R |
| F05-K10-86 | A#7 | ROM 209: DJ-Scratch 2 ROM 208: DJ-Scratch 1 Off L/R |
| F05-K10-87 | B7 | ROM 369: Tribe ROM 369: Tribe Off L/R |
| F05-K10-88 | C8 | ROM 195: 88-Cowbell ROM 195: 88-Cowbell Off IFX 1 |

### F05-K11 - Trance/GarageKit (kit 10, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K11-01 | A0 |  |
| F05-K11-02 | A#0 |  |
| F05-K11-03 | B0 |  |
| F05-K11-04 | C1 | ROM 338: Four... Solo Off L/R |
| F05-K11-05 | C#1 | ROM 325: Yea Solo Off L/R |
| F05-K11-06 | D1 | ROM 339: Three... Solo Off L/R |
| F05-K11-07 | D#1 | ROM 326: Year solo Off L/R |
| F05-K11-08 | E1 | ROM 340: Two... Solo Off L/R |
| F05-K11-09 | F1 | ROM 341: One... Solo Off L/R |
| F05-K11-10 | F#1 | ROM 329: Uhhhh Solo Off L/R |
| F05-K11-11 | G1 | ROM 207: Zap 3 Off L/R |
| F05-K11-12 | G#1 | ROM 228: Finger Snap Off L/R |
| F05-K11-13 | A1 | ROM 206: Zap 2 Off L/R |
| F05-K11-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K11-15 | B1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K11-16 | C2 | ROM 178: 99-BD 3 Off L/R |
| F05-K11-17 | C#2 | ROM 012: BD-Mondo Kill Off L/R |
| F05-K11-18 | D2 | ROM 178: 99-BD 3 Off IFX 1 |
| F05-K11-19 | D#2 | ROM 017: BD-Squash Off L/R |
| F05-K11-20 | E2 | ROM 021: BD-Dance 4 Off L/R |
| F05-K11-21 | F2 | ROM 048: SD-Full Room Off L/R |
| F05-K11-22 | F#2 | ROM 064: SD-Cracker Room Off L/R |
| F05-K11-23 | G2 | ROM 078: SD-Hip 5 Off L/R |
| F05-K11-24 | G#2 | ROM 085: SD-Vintage 4 Off L/R |
| F05-K11-25 | A2 | ROM 099: SD-Ambi Hit 2 Off L/R |
| F05-K11-26 | A#2 | ROM 081: SD-Tiny Off L/R |
| F05-K11-27 | B2 | ROM 180: 99-BD 5 Off L/R |
| F05-K11-28 | C3 | ROM 178: 99-BD 3 Off L/R |
| F05-K11-29 | C#3 | ROM 203: Dance Perc-Hous1 Off L/R |
| F05-K11-30 | D3 | ROM 183: 99-SD 1 Off L/R |
| F05-K11-31 | D#3 | ROM 186: 88-Claps Off L/R |
| F05-K11-32 | E3 | ROM 184: 99-SD 2 Off L/R |
| F05-K11-33 | F3 | ROM 198: Real E.Tom Off L/R |
| F05-K11-34 | F#3 | ROM 189: 99-HHat Closed Off L/R |
| F05-K11-35 | G3 | ROM 198: Real E.Tom Off L/R |
| F05-K11-36 | G#3 | ROM 135: H.Hat-Crisp Open Off L/R |
| F05-K11-37 | A3 | ROM 198: Real E.Tom Off L/R |
| F05-K11-38 | A#3 | ROM 190: 99-HHat Open Off L/R |
| F05-K11-39 | B3 | ROM 198: Real E.Tom Off L/R |
| F05-K11-40 | C4 | ROM 198: Real E.Tom Off L/R |
| F05-K11-41 | C#4 | ROM 158: Crash-Dance 99 Off L/R |
| F05-K11-42 | D4 | ROM 198: Real E.Tom Off L/R |
| F05-K11-43 | D#4 | ROM 160: Ride-Dance 99 Off L/R |
| F05-K11-44 | E4 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-K11-45 | F4 | ROM 070: SD-Rap 2 Off L/R |
| F05-K11-46 | F#4 | ROM 288: Tambourine-Push 1 L/R |
| F05-K11-47 | G4 | ROM 087: SD-Vintage 6 Off L/R |
| F05-K11-48 | G#4 | ROM 289: Tambourine-Pull 1 L/R |
| F05-K11-49 | A4 | ROM 069: SD-Rap 1 Off L/R |
| F05-K11-50 | A#4 | ROM 290: Tambourine-Acc.1 1 L/R |
| F05-K11-51 | B4 | ROM 291: Tambourine-Acc.2 1 L/R |
| F05-K11-52 | C5 | ROM 245: Bongo Hi-Open Off L/R |
| F05-K11-53 | C#5 | ROM 242: Bongo Lo-Open Off L/R |
| F05-K11-54 | D5 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-K11-55 | D#5 | ROM 235: Conga Hi-Open Off L/R |
| F05-K11-56 | E5 | ROM 232: Conga Lo-Open Off L/R |
| F05-K11-57 | F5 | ROM 233: Conga Lo-MtSlap Off L/R |
| F05-K11-58 | F#5 | ROM 234: Conga Lo-Slap Off L/R |
| F05-K11-59 | G5 | ROM 233: Conga Lo-MtSlap Off L/R |
| F05-K11-60 | G#5 | ROM 239: Conga Hi-Slap 2 Off L/R |
| F05-K11-61 | A5 | ROM 238: Conga Hi-Slap 1 Off L/R |
| F05-K11-62 | A#5 | ROM 243: Bongo Lo-Slap Off L/R |
| F05-K11-63 | B5 | ROM 246: Bongo Hi-Slap Off L/R |
| F05-K11-64 | C6 | ROM 220: DJ-Reverse Off L/R |
| F05-K11-65 | C#6 | ROM 215: DJ-1c Vocal Rub Off L/R |
| F05-K11-66 | D6 | ROM 214: DJ-Hit Rub Off L/R |
| F05-K11-67 | D#6 | ROM 219: DJ-Record Stop Off L/R |
| F05-K11-68 | E6 | ROM 216: DJ-1c Vocal Rub Off L/R |
| F05-K11-69 | F6 | ROM 212: DJ-Scratch 3c Off L/R |
| F05-K11-70 | F#6 | ROM 218: DJ-SD Rub Off L/R |
| F05-K11-71 | G6 | ROM 217: DJ-BD Rub Off L/R |
| F05-K11-72 | G#6 | ROM 293: Triangle-Mute Off L/R |
| F05-K11-73 | A6 | ROM 292: Triangle-Open Off L/R |
| F05-K11-74 | A#6 | ROM 230: Hand Claps-Hip Off L/R |
| F05-K11-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K11-76 | C7 | ROM 174: 88-BD 2(R&B) Off L/R |
| F05-K11-77 | C#7 | ROM 187: 88-HHat Closed Off L/R |
| F05-K11-78 | D7 | ROM 182: 88-SD 2(Old) Off L/R |
| F05-K11-79 | D#7 | ROM 188: 88-HHat Open Off L/R |
| F05-K11-80 | E7 | ROM 191: 88-Crash Off L/R |
| F05-K11-81 | F7 | ROM 195: 88-Cowbell Off L/R |
| F05-K11-82 | F#7 | ROM 186: 88-Claps Off L/R |
| F05-K11-83 | G7 | ROM 196: 88-Maraca Off L/R |
| F05-K11-84 | G#7 | ROM 185: 88-Rimshot Off L/R |
| F05-K11-85 | A7 | ROM 181: 88-SD 1(New) Off L/R |
| F05-K11-86 | A#7 | ROM 194: 88-Clave Off L/R |
| F05-K11-87 | B7 | ROM 354: Stadium Off L/R |
| F05-K11-88 | C8 | ROM 311: Finger Cymbal Off L/R |

### F05-K12 - Mashed Kit (kit 11, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K12-01 | A0 |  |
| F05-K12-02 | A#0 |  |
| F05-K12-03 | B0 |  |
| F05-K12-04 | C1 | EX 011: BD-Dance 14 EX 011: BD-Dance 14 Off IFX 1 |
| F05-K12-05 | C#1 | EX 004: BD-Dance 7 Off IFX 1 |
| F05-K12-06 | D1 | EX 006: BD-Dance 9 EX 006: BD-Dance 9 Off IFX 1 |
| F05-K12-07 | D#1 | EX 009: BD-Dance 12 EX 009: BD-Dance 12 Off IFX 1 |
| F05-K12-08 | E1 | EX 007: BD-Dance 10 Off IFX 1 |
| F05-K12-09 | F1 | EX 041: SD-Dance 16 EX 039: SD-Dance 14 Off IFX 1 |
| F05-K12-10 | F#1 | EX 037: SD-Dance 12 EX 036: SD-Dance 11 Off IFX 1 |
| F05-K12-11 | G1 | EX 022: SD-Dry 9 EX 016: SD-Dry 6 Off IFX 1 |
| F05-K12-12 | G#1 | EX 013: SD-Dry 4 EX 015: SD-Dry 5 ff Off IFX 1 |
| F05-K12-13 | A1 | EX 014: SD-Dry 5 f EX 014: SD-Dry 5 f Off IFX 1 |
| F05-K12-14 | A#1 | ROM 157: Cymbal Reverse Off L/R |
| F05-K12-15 | B1 | EX 029: SD-Dance 4 Off L/R |
| F05-K12-16 | C2 | ROM 024: BD-Hip 3 ROM 034: BD-Dance Soft Off IFX 1 |
| F05-K12-17 | C#2 | EX 005: BD-Dance 8 Off IFX 1 |
| F05-K12-18 | D2 | EX 007: BD-Dance 10 EX 003: BD-Dance 6 Off IFX 1 |
| F05-K12-19 | D#2 | EX 012: BD-Dance 15 EX 012: BD-Dance 15 Off IFX 1 |
| F05-K12-20 | E2 | EX 010: BD-Dance 13 EX 011: BD-Dance 14 Off IFX 1 |
| F05-K12-21 | F2 | EX 030: SD-Dance 5 EX 031: SD-Dance 6 Off IFX 1 |
| F05-K12-22 | F#2 | EX 042: SD-Dance 17 EX 038: SD-Dance 13 Off IFX 1 |
| F05-K12-23 | G2 | EX 043: SD-Dance 18 EX 043: SD-Dance 18 Off IFX 1 |
| F05-K12-24 | G#2 | EX 035: SD-Dance 10 EX 035: SD-Dance 10 Off IFX 1 |
| F05-K12-25 | A2 | EX 034: SD-Dance 9 EX 033: SD-Dance 8 Off IFX 1 |
| F05-K12-26 | A#2 | EX 044: SD-Dance 19 EX 027: SD-Dance 2 Off IFX 1 |
| F05-K12-27 | B2 | ROM 034: BD-Dance Soft EX 002: BD-Dance 5 Off IFX 1 |
| F05-K12-28 | C3 | EX 08: BD-Dance 11 EX 008: BD-Dance 11 Off IFX 1 |
| F05-K12-29 | C#3 | EX 026: Sidestick-Amb2 f Off IFX 1 |
| F05-K12-30 | D3 | EX 029: SD-Dance 4 EX 029: SD-Dance 4 Off IFX 1 |
| F05-K12-31 | D#3 | EX 085: Dance Claps EX 085: Dance Claps Off IFX 1 |
| F05-K12-32 | E3 | EX 032: SD-Dance 7 EX 031: SD-Dance 6 Off IFX 1 |
| F05-K12-33 | F3 | EX 049: Tom 5-Lo ff EX 048: Tom 5-Lo mf Off IFX 1 |
| F05-K12-34 | F#3 | EX 069: H.Hat-Dance 8 EX 069: H.Hat-Dance 8 1 L/R |
| F05-K12-35 | G3 | EX 051: Tom 5-Hi ff EX 050: Tom 5-Hi mf Off IFX 1 |
| F05-K12-36 | G#3 | EX 071: H.Hat-Dance 10 1 L/R |
| F05-K12-37 | A3 | EX 051: Tom 5-Hi ff EX 050: Tom 5-Hi mf Off IFX 1 |
| F05-K12-38 | A#3 | EX 077: H.Hat-Dance 16 EX 070: H.Hat-Dance 9 1 L/R |
| F05-K12-39 | B3 | ROM 249: Djembe-Open Off IFX 1 |
| F05-K12-40 | C4 | ROM 249: Djembe-Open Off IFX 1 |
| F05-K12-41 | C#4 | EX 081: Crash-Dance Off L/R |
| F05-K12-42 | D4 | ROM 249: Djembe-Open Off IFX 1 |
| F05-K12-43 | D#4 | EX 078: Ride-Edge 3 EX 078: Ride-Edge 3 Off L/R |
| F05-K12-44 | E4 | EX 081: Crash-Dance Off L/R |
| F05-K12-45 | F4 | ROM 160: Ride-Dance 99 Off L/R |
| F05-K12-46 | F#4 | ROM 290: Tambourine-Acc.1 ROM 288: Tambourine-Push Off L/R |
| F05-K12-47 | G4 | EX 094: DJ VinylSliced 4 EX 093: DJ VinylSliced 3 Off IFX 1 |
| F05-K12-48 | G#4 | ROM 306: Cowbell 2 ROM 305: Cowbell 1 Off L/R |
| F05-K12-49 | A4 | ROM 158: Crash-Dance 99 Off L/R |
| F05-K12-50 | A#4 | EX 098: Vocoder Voice Off L/R |
| F05-K12-51 | B4 | ROM 311: Finger Cymbal ROM 311: Finger Cymbal Off L/R |
| F05-K12-52 | C5 | ROM 245: Bongo Hi-Open ROM 245: Bongo Hi-Open Off L/R |
| F05-K12-53 | C#5 | ROM 242: Bongo Lo-Open ROM 242: Bongo Lo-Open Off L/R |
| F05-K12-54 | D5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K12-55 | D#5 | ROM 239: Conga Hi-Slap 2 ROM 237: Conga Hi-MtSlap Off L/R |
| F05-K12-56 | E5 | ROM 234: Conga Lo-Slap ROM 232: Conga Lo-Open Off L/R |
| F05-K12-57 | F5 | EX 063: H.Hat-Dance 2 EX 062: H.Hat-Dance 1 Off L/R |
| F05-K12-58 | F#5 | EX 074: H.Hat-Dance 13 EX 072: H.Hat-Dance 11 Off L/R |
| F05-K12-59 | G5 | ROM 288: Tambourine-Push Off L/R |
| F05-K12-60 | G#5 | ROM 289: Tambourine-Pull Off L/R |
| F05-K12-61 | A5 | ROM 301: Cabasa-Tap Off L/R |
| F05-K12-62 | A#5 | ROM 258: Maracas-Push ROM 259: Maracas-Pull Off L/R |
| F05-K12-63 | B5 | EX 100: Cannon Off L/R |
| F05-K12-64 | C6 | ROM 061: SD-Yowie Off IFX 1 |
| F05-K12-65 | C#6 | EX 064: H.Hat-Dance 3 EX 073: H.Hat-Dance 12 Off L/R |
| F05-K12-66 | D6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K12-67 | D#6 | EX 084: WD-Ethno SD EX 084: WD-Ethno SD Off L/R |
| F05-K12-68 | E6 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off L/R |
| F05-K12-69 | F6 | EX 088: Dance Perc-2 a EX 089: Dance Perc-2 b Off L/R |
| F05-K12-70 | F#6 | EX 099: Sword EX 099: Sword Off L/R |
| F05-K12-71 | G6 | EX 090: Dance Perc-3 EX 090: Dance Perc-3 Off L/R |
| F05-K12-72 | G#6 | ROM 293: Triangle-Mute 2 L/R |
| F05-K12-73 | A6 | ROM 292: Triangle-Open 2 L/R |
| F05-K12-74 | A#6 | ROM 298: Shaker 2 ROM 298: Shaker 2 Off L/R |
| F05-K12-75 | B6 | ROM 312: Marc Tree Off L/R |
| F05-K12-76 | C7 | ROM 163: Ride-Edge 2 Off L/R |
| F05-K12-77 | C#7 | ROM 164: Ride-Jazz Off L/R |
| F05-K12-78 | D7 | ROM 162: Ride-Edge 1 Off L/R |
| F05-K12-79 | D#7 | EX 078: Ride-Edge 3 Off L/R |
| F05-K12-80 | E7 | EX 096: Aow Off L/R |
| F05-K12-81 | F7 | EX 091: DJ VinylSliced 1 EX 092: DJ VinylSliced 2 Off L/R |
| F05-K12-82 | F#7 | ROM 320: SD-Metal Striker ROM 321: SD-Air Explosion Off L/R |
| F05-K12-83 | G7 | ROM 300: Cabasa-Down ROM 300: Cabasa-Down Off L/R |
| F05-K12-84 | G#7 | ROM 064: SD-Cracker Room 3 IFX 1 |
| F05-K12-85 | A7 | ROM 064: SD-Cracker Room 3 IFX 1 |
| F05-K12-86 | A#7 | ROM 314: Bell Tree Off L/R |
| F05-K12-87 | B7 | ROM 377: Car Crash Off L/R |
| F05-K12-88 | C8 | ROM 297: Shaker 1 Off L/R |

### F05-K13 - Precussion Kit (kit 12, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K13-01 | A0 |  |
| F05-K13-02 | A#0 |  |
| F05-K13-03 | B0 |  |
| F05-K13-04 | C1 | ROM 275: Taiko-Rim ROM 274: Taiko-Open Off L/R |
| F05-K13-05 | C#1 | ROM 276: Tsuzumi ROM 276: Tsuzumi Off L/R |
| F05-K13-06 | D1 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off IFX 1 |
| F05-K13-07 | D#1 | ROM 251: Djembe-Slap ROM 250: Djembe-Mute Off IFX 1 |
| F05-K13-08 | E1 | ROM 261: Baya-Ghe ROM 260: Baya-Open 5 L/R |
| F05-K13-09 | F1 | ROM 262: Baya-Mute 1 ROM 264: Baya-Mute 3 Off L/R |
| F05-K13-10 | F#1 | ROM 266: Baya-Mute 5 ROM 265: Baya-Mute 4 Off L/R |
| F05-K13-11 | G1 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off L/R |
| F05-K13-12 | G#1 | ROM 269: Tabla-Tin ROM 273: Tabla-Mute 4 Off L/R |
| F05-K13-13 | A1 | ROM 271: Tabla-Mute 2 ROM 272: Tabla-Mute 3 Off L/R |
| F05-K13-14 | A#1 | ROM 267: Tabla-Na ROM 270: Tabla-Mute 1 6 L/R |
| F05-K13-15 | B1 | ROM 270: Tabla-Mute 1 ROM 271: Tabla-Mute 2 Off L/R |
| F05-K13-16 | C2 | ROM 166: BD-Orchestral Off L/R |
| F05-K13-17 | C#2 | ROM 369: Tribe Off L/R |
| F05-K13-18 | D2 | ROM 228: Finger Snap Off L/R |
| F05-K13-19 | D#2 | ROM 229: Hand Claps Off L/R |
| F05-K13-20 | E2 | ROM 293: Triangle-Mute 1 L/R |
| F05-K13-21 | F2 | ROM 292: Triangle-Open 1 L/R |
| F05-K13-22 | F#2 | ROM 254: Castanet-Dble 2 Off IFX 1 |
| F05-K13-23 | G2 | ROM 252: Castanet-Single Off IFX 1 |
| F05-K13-24 | G#2 | ROM 253: Castanet-Dble 1 Off IFX 1 |
| F05-K13-25 | A2 | ROM 255: Guiro-Long 2 L/R |
| F05-K13-26 | A#2 | ROM 256: Guiro-Short ROM 256: Guiro-Short 2 L/R |
| F05-K13-27 | B2 | ROM 257: Guiro-Tap 2 L/R |
| F05-K13-28 | C3 | ROM 277: Vibraslap Off IFX 1 |
| F05-K13-29 | C#3 | ROM 278: Claves Off IFX 1 |
| F05-K13-30 | D3 | ROM 194: 88-Clave Off IFX 1 |
| F05-K13-31 | D#3 | ROM 296: Cuica-Lo 3 IFX 1 |
| F05-K13-32 | E3 | ROM 295: Cuica-Hi 3 IFX 1 |
| F05-K13-33 | F3 | ROM 281: Timbale Lo-Open Off IFX 1 |
| F05-K13-34 | F#3 | ROM 279: Woodblock 1 Off IFX 1 |
| F05-K13-35 | G3 | ROM 283: Timbale Lo-Rim Off IFX 1 |
| F05-K13-36 | G#3 | ROM 279: Woodblock 1 Off IFX 1 |
| F05-K13-37 | A3 | ROM 282: Timbale Lo-Mute Off IFX 1 |
| F05-K13-38 | A#3 | ROM 201: Vocal Woodblock Off IFX 1 |
| F05-K13-39 | B3 | ROM 284: Timbale Hi-Edge Off IFX 1 |
| F05-K13-40 | C4 | ROM 286: Timbale Hi-Rim 2 Off IFX 1 |
| F05-K13-41 | C#4 | ROM 307: Chacha Bell Off IFX 1 |
| F05-K13-42 | D4 | ROM 285: Timbale Hi-Rim 1 Off IFX 1 |
| F05-K13-43 | D#4 | ROM 308: Mambo Bell Off IFX 1 |
| F05-K13-44 | E4 | ROM 287: Timbale-Paila Off IFX 1 |
| F05-K13-45 | F4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-K13-46 | F#4 | ROM 305: Cowbell 1 Off L/R |
| F05-K13-47 | G4 | ROM 243: Bongo Lo-Slap Off L/R |
| F05-K13-48 | G#4 | ROM 306: Cowbell 2 Off L/R |
| F05-K13-49 | A4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-K13-50 | A#4 | ROM 200: Vocal Cowbell Off L/R |
| F05-K13-51 | B4 | ROM 246: Bongo Hi-Slap ROM 246: Bongo Hi-Slap Off L/R |
| F05-K13-52 | C5 | ROM 232: Conga Lo-Open ROM 232: Conga Lo-Open Off L/R |
| F05-K13-53 | C#5 | ROM 234: Conga Lo-Slap Off L/R |
| F05-K13-54 | D5 | ROM 233: Conga Lo-MtSlap Off L/R |
| F05-K13-55 | D#5 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-K13-56 | E5 | ROM 235: Conga Hi-Open ROM 235: Conga Hi-Open Off L/R |
| F05-K13-57 | F5 | ROM 236: Conga Hi-Mute Off L/R |
| F05-K13-58 | F#5 | ROM 240: Conga-Heel Off L/R |
| F05-K13-59 | G5 | ROM 238: Conga Hi-Slap 1 Off L/R |
| F05-K13-60 | G#5 | ROM 241: Conga-Toe Off L/R |
| F05-K13-61 | A5 | ROM 239: Conga Hi-Slap 2 Off L/R |
| F05-K13-62 | A#5 | ROM 304: Agogo Bell Off IFX 1 |
| F05-K13-63 | B5 | ROM 304: Agogo Bell Off IFX 1 |
| F05-K13-64 | C6 | ROM 259: Maracas-Pull ROM 258: Maracas-Push Off IFX 1 |
| F05-K13-65 | C#6 | ROM 297: Shaker 1 ROM 297: Shaker 1 Off L/R |
| F05-K13-66 | D6 | ROM 298: Shaker 2 ROM 298: Shaker 2 Off L/R |
| F05-K13-67 | D#6 | ROM 301: Cabasa-Tap ROM 300: Cabasa-Down Off L/R |
| F05-K13-68 | E6 | ROM 301: Cabasa-Tap ROM 299: Cabasa-Up Off L/R |
| F05-K13-69 | F6 | ROM 302: Caxixi-Hard ROM 303: Caxixi-Soft Off L/R |
| F05-K13-70 | F#6 | ROM 288: Tambourine-Push Off L/R |
| F05-K13-71 | G6 | ROM 290: Tambourine-Acc.1 ROM 290: Tambourine-Acc.1 Off L/R |
| F05-K13-72 | G#6 | ROM 289: Tambourine-Pull Off L/R |
| F05-K13-73 | A6 | ROM 291: Tambourine-Acc.2 ROM 291: Tambourine-Acc.2 Off IFX 1 |
| F05-K13-74 | A#6 | ROM 309: Sleigh Bell ROM 309: Sleigh Bell Off L/R |
| F05-K13-75 | B6 | ROM 316: Samba Whistle 4 L/R |
| F05-K13-76 | C7 | ROM 316: Samba Whistle 4 L/R |
| F05-K13-77 | C#7 | ROM 314: Bell Tree Off L/R |
| F05-K13-78 | D7 | ROM 316: Samba Whistle 4 L/R |
| F05-K13-79 | D#7 | ROM 312: Marc Tree Off L/R |
| F05-K13-80 | E7 | ROM 316: Samba Whistle 4 L/R |
| F05-K13-81 | F7 | ROM 310: Rap Sleighbell ROM 310: Rap Sleighbell Off L/R |
| F05-K13-82 | F#7 | ROM 371: Rainstick Off L/R |
| F05-K13-83 | G7 | ROM 317: Chinese Gong Off L/R |
| F05-K13-84 | G#7 | ROM 361: Bird 1 Off L/R |
| F05-K13-85 | A7 | ROM 362: Bird 2 Off L/R |
| F05-K13-86 | A#7 | ROM 361: Bird 1 Off L/R |
| F05-K13-87 | B7 | ROM 393: Cricket Spectrum Off L/R |
| F05-K13-88 | C8 | ROM 312: Marc Tree Off L/R |

### F05-K14 - Orchestra&Ethnic (kit 13, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K14-01 | A0 |  |
| F05-K14-02 | A#0 |  |
| F05-K14-03 | B0 |  |
| F05-K14-04 | C1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-05 | C#1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-06 | D1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-07 | D#1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-08 | E1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-09 | F1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-10 | F#1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-11 | G1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-12 | G#1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-13 | A1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-14 | A#1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-15 | B1 | ROM 222: Orchestra Hit ROM 222: Orchestra Hit Off L/R |
| F05-K14-16 | C2 | ROM 166: BD-Orchestral ROM 166: BD-Orchestral Off L/R |
| F05-K14-17 | C#2 | ROM 170: SD-Orch.Roll 1 ROM 170: SD-Orch.Roll 1 1 L/R |
| F05-K14-18 | D2 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral Off L/R |
| F05-K14-19 | D#2 | ROM 171: SD-Orch.Roll 2 ROM 171: SD-Orch.Roll 2 1 L/R |
| F05-K14-20 | E2 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 1 L/R |
| F05-K14-21 | F2 | ROM 168: Orch Cym.-Open ROM 168: Orch Cym.-Open 2 L/R |
| F05-K14-22 | F#2 | ROM 167: Orch Cym.-Closed ROM 167: Orch Cym.-Closed 2 L/R |
| F05-K14-23 | G2 | ROM 274: Taiko-Open ROM 274: Taiko-Open Off L/R |
| F05-K14-24 | G#2 | ROM 170: SD-Orch.Roll 1 ROM 170: SD-Orch.Roll 1 3 L/R |
| F05-K14-25 | A2 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral Off L/R |
| F05-K14-26 | A#2 | ROM 171: SD-Orch.Roll 2 ROM 171: SD-Orch.Roll 2 3 L/R |
| F05-K14-27 | B2 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral 3 L/R |
| F05-K14-28 | C3 | ROM 293: Triangle-Mute ROM 293: Triangle-Mute 4 L/R |
| F05-K14-29 | C#3 | ROM 294: Triangle-Roll ROM 294: Triangle-Roll 4 L/R |
| F05-K14-30 | D3 | ROM 292: Triangle-Open ROM 292: Triangle-Open 4 L/R |
| F05-K14-31 | D#3 | ROM 317: Chinese Gong ROM 317: Chinese Gong Off IFX 1 |
| F05-K14-32 | E3 | ROM 291: Tambourine-Acc.2 ROM 291: Tambourine-Acc.2 Off L/R |
| F05-K14-33 | F3 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off IFX 1 |
| F05-K14-34 | F#3 | ROM 253: Castanet-Dble 1 ROM 253: Castanet-Dble 1 Off L/R |
| F05-K14-35 | G3 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off IFX 1 |
| F05-K14-36 | G#3 | ROM 254: Castanet-Dble 2 ROM 254: Castanet-Dble 2 Off L/R |
| F05-K14-37 | A3 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off IFX 1 |
| F05-K14-38 | A#3 | ROM 252: Castanet-Single ROM 252: Castanet-Single Off L/R |
| F05-K14-39 | B3 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off IFX 1 |
| F05-K14-40 | C4 | ROM 279: Woodblock 1 ROM 279: Woodblock 1 Off IFX 1 |
| F05-K14-41 | C#4 | ROM 169: Timpani ROM 169: Timpani Off L/R |
| F05-K14-42 | D4 | ROM 169: Timpani ROM 169: Timpani Off L/R |
| F05-K14-43 | D#4 | ROM 169: Timpani ROM 169: Timpani Off L/R |
| F05-K14-44 | E4 | ROM 169: Timpani ROM 169: Timpani Off L/R |
| F05-K14-45 | F4 | ROM 249: Djembe-Open ROM 249: Djembe-Open Off IFX 1 |
| F05-K14-46 | F#4 | ROM 250: Djembe-Mute ROM 250: Djembe-Mute Off IFX 1 |
| F05-K14-47 | G4 | ROM 251: Djembe-Slap ROM 251: Djembe-Slap Off IFX 1 |
| F05-K14-48 | G#4 | ROM 302: Caxixi-Hard ROM 303: Caxixi-Soft Off L/R |
| F05-K14-49 | A4 | ROM 309: Sleigh Bell ROM 309: Sleigh Bell Off IFX 1 |
| F05-K14-50 | A#4 | ROM 299: Cabasa-Up ROM 300: Cabasa-Down Off L/R |
| F05-K14-51 | B4 | ROM 311: Finger Cymbal ROM 311: Finger Cymbal Off L/R |
| F05-K14-52 | C5 | ROM 261: Baya-Ghe ROM 261: Baya-Ghe Off IFX 1 |
| F05-K14-53 | C#5 | ROM 260: Baya-Open ROM 260: Baya-Open Off IFX 1 |
| F05-K14-54 | D5 | ROM 262: Baya-Mute 1 ROM 262: Baya-Mute 1 Off IFX 1 |
| F05-K14-55 | D#5 | ROM 263: Baya-Mute 2 ROM 263: Baya-Mute 2 Off IFX 1 |
| F05-K14-56 | E5 | ROM 264: Baya-Mute 3 ROM 264: Baya-Mute 3 Off IFX 1 |
| F05-K14-57 | F5 | ROM 265: Baya-Mute 4 ROM 265: Baya-Mute 4 Off IFX 1 |
| F05-K14-58 | F#5 | ROM 266: Baya-Mute 5 ROM 266: Baya-Mute 5 Off IFX 1 |
| F05-K14-59 | G5 | ROM 267: Tabla-Na ROM 267: Tabla-Na Off IFX 1 |
| F05-K14-60 | G#5 | ROM 268: Tabla-Open ROM 268: Tabla-Open Off IFX 1 |
| F05-K14-61 | A5 | ROM 269: Tabla-Tin ROM 269: Tabla-Tin Off IFX 1 |
| F05-K14-62 | A#5 | ROM 270: Tabla-Mute 1 ROM 270: Tabla-Mute 1 Off IFX 1 |
| F05-K14-63 | B5 | ROM 271: Tabla-Mute 2 ROM 271: Tabla-Mute 2 Off IFX 1 |
| F05-K14-64 | C6 | ROM 272: Tabla-Mute 3 ROM 272: Tabla-Mute 3 Off IFX 1 |
| F05-K14-65 | C#6 | ROM 273: Tabla-Mute 4 ROM 273: Tabla-Mute 4 Off IFX 1 |
| F05-K14-66 | D6 | ROM 274: Taiko-Open ROM 274: Taiko-Open Off IFX 1 |
| F05-K14-67 | D#6 | ROM 276: Tsuzumi Off IFX 1 |
| F05-K14-68 | E6 | ROM 275: Taiko-Rim ROM 275: Taiko-Rim Off IFX 1 |
| F05-K14-69 | F6 | ROM 232: Conga Lo-Open Off IFX 1 |
| F05-K14-70 | F#6 | ROM 233: Conga Lo-MtSlap ROM 239: Conga Hi-Slap 2 Off IFX 1 |
| F05-K14-71 | G6 | ROM 236: Conga Hi-Mute ROM 246: Bongo Hi-Slap Off IFX 1 |
| F05-K14-72 | G#6 | ROM 238: Conga Hi-Slap 1 Off IFX 1 |
| F05-K14-73 | A6 | ROM 399: Tambura ROM 399: Tambura Off L/R |
| F05-K14-74 | A#6 | ROM 371: Rainstick ROM 371: Rainstick Off L/R |
| F05-K14-75 | B6 | ROM 403: HarpGliss ROM 403: HarpGliss Off L/R |
| F05-K14-76 | C7 | ROM 371: Rainstick ROM 371: Rainstick Off L/R |
| F05-K14-77 | C#7 | ROM 000: BD-Dry 1 ROM 000: BD-Dry 1 Off IFX 1 |
| F05-K14-78 | D7 | ROM 000: BD-Dry 1 ROM 000: BD-Dry 1 Off L/R |
| F05-K14-79 | D#7 | ROM 398: Gamelan ROM 398: Gamelan Off L/R |
| F05-K14-80 | E7 | ROM 398: Gamelan ROM 398: Gamelan Off L/R |
| F05-K14-81 | F7 | ROM 361: Bird 1 ROM 361: Bird 1 11 L/R |
| F05-K14-82 | F#7 | ROM 393: Cricket Spectrum Off L/R |
| F05-K14-83 | G7 | ROM 369: Tribe ROM 369: Tribe Off L/R |
| F05-K14-84 | G#7 | ROM 000: BD-Dry 1 ROM 000: BD-Dry 1 Off L/R |
| F05-K14-85 | A7 | ROM 398: Gamelan ROM 398: Gamelan Off L/R |
| F05-K14-86 | A#7 | ROM 398: Gamelan Off L/R |
| F05-K14-87 | B7 | ROM 398: Gamelan ROM 398: Gamelan Off L/R |
| F05-K14-88 | C8 | ROM 398: Gamelan ROM 398: Gamelan Off L/R |

### F05-K15 - BD/SD Catalog 1 (kit 14, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K15-01 | A0 |  |
| F05-K15-02 | A#0 |  |
| F05-K15-03 | B0 |  |
| F05-K15-04 | C1 |  |
| F05-K15-05 | C#1 |  |
| F05-K15-06 | D1 |  |
| F05-K15-07 | D#1 |  |
| F05-K15-08 | E1 |  |
| F05-K15-09 | F1 |  |
| F05-K15-10 | F#1 |  |
| F05-K15-11 | G1 |  |
| F05-K15-12 | G#1 |  |
| F05-K15-13 | A1 |  |
| F05-K15-14 | A#1 | ROM 013: BD-Terminator ROM 013: BD-Terminator Off IFX 1 |
| F05-K15-15 | B1 | ROM 012: BD-Mondo Kill ROM 012: BD-Mondo Kill Off IFX 1 |
| F05-K15-16 | C2 | ROM 000: BD-Dry 1 ROM 000: BD-Dry 1 Off IFX 1 |
| F05-K15-17 | C#2 | ROM 001: BD-Dry 2 ROM 001: BD-Dry 2 Off IFX 1 |
| F05-K15-18 | D2 | ROM 002: BD-Dry 3 ROM 002: BD-Dry 3 Off IFX 1 |
| F05-K15-19 | D#2 | ROM 003: BD-Dry 4 ROM 003: BD-Dry 4 Off IFX 1 |
| F05-K15-20 | E2 | ROM 004: BD-Dry 5 ROM 004: BD-Dry 5 Off IFX 1 |
| F05-K15-21 | F2 | ROM 006: BD-Soft ROM 006: BD-Soft Off IFX 1 |
| F05-K15-22 | F#2 | ROM 007: BD-Soft Room ROM 007: BD-Soft Room Off IFX 1 |
| F05-K15-23 | G2 | ROM 008: BD-Jazz 1 ROM 008: BD-Jazz 1 Off IFX 1 |
| F05-K15-24 | G#2 | ROM 009: BD-Jazz 2 ROM 009: BD-Jazz 2 Off IFX 1 |
| F05-K15-25 | A2 | ROM 010: BD-Pillow ROM 010: BD-Pillow Off IFX 1 |
| F05-K15-26 | A#2 | ROM 011: BD-Woofer ROM 011: BD-Woofer Off IFX 1 |
| F05-K15-27 | B2 | EX 000: BD-amb 1 EX 000: BD-amb 1 Off IFX 1 |
| F05-K15-28 | C3 | EX 001: BD-amb 2 EX 001: BD-amb 2 Off IFX 1 |
| F05-K15-29 | C#3 | ROM 014: BD-Tubby ROM 014: BD-Tubby Off IFX 1 |
| F05-K15-30 | D3 | ROM 015: BD-Gated ROM 015: BD-Gated Off IFX 1 |
| F05-K15-31 | D#3 | ROM 016: BD-Tight ROM 016: BD-Tight Off IFX 1 |
| F05-K15-32 | E3 | ROM 017: BD-Squash ROM 017: BD-Squash Off IFX 1 |
| F05-K15-33 | F3 | ROM 020: BD-Dance 3 ROM 020: BD-Dance 3 Off IFX 1 |
| F05-K15-34 | F#3 | ROM 026: BD-Dark ROM 026: BD-Dark Off IFX 1 |
| F05-K15-35 | G3 | ROM 028: BD-Ringy ROM 028: BD-Ringy Off IFX 1 |
| F05-K15-36 | G#3 | ROM 034: BD-Dance Soft ROM 034: BD-Dance Soft Off IFX 1 |
| F05-K15-37 | A3 | ROM 037: BD-PhatJaz ROM 037: BD-PhatJaz Off IFX 1 |
| F05-K15-38 | A#3 | ROM 166: BD-Orchestral ROM 166: BD-Orchestral Off IFX 1 |
| F05-K15-39 | B3 | ROM 040: SD-Dry 1 ROM 040: SD-Dry 1 Off IFX 1 |
| F05-K15-40 | C4 | EX 013: SD-Dry 4 EX 013: SD-Dry 4 Off IFX 1 |
| F05-K15-41 | C#4 | EX 015: SD-Dry 5 ff EX 014: SD-Dry 5 f Off IFX 1 |
| F05-K15-42 | D4 | EX 016: SD-Dry 6 EX 016: SD-Dry 6 Off IFX 1 |
| F05-K15-43 | D#4 | EX 017: SD-Dry 7 EX 017: SD-Dry 7 Off IFX 1 |
| F05-K15-44 | E4 | EX 020: SD-Dry 8 f EX 018: SD-Dry 8 p Off IFX 1 |
| F05-K15-45 | F4 | EX 021: SD-Dry 8 ff EX 019: SD-Dry 8 mp Off IFX 1 |
| F05-K15-46 | F#4 | EX 022: SD-Dry 9 EX 022: SD-Dry 9 Off IFX 1 |
| F05-K15-47 | G4 | EX 024: SD-Roll-Closed EX 023: SD-Roll-Open Off IFX 1 |
| F05-K15-48 | G#4 | ROM 049: SD-Off Center ROM 049: SD-Off Center Off IFX 1 |
| F05-K15-49 | A4 | ROM 050: SD-Jazz Ring ROM 050: SD-Jazz Ring Off IFX 1 |
| F05-K15-50 | A#4 | ROM 051: SD-Amb.Piccolo 1 ROM 051: SD-Amb.Piccolo 1 Off IFX 1 |
| F05-K15-51 | B4 | ROM 052: SD-Amb.Piccolo 2 ROM 052: SD-Amb.Piccolo 2 Off IFX 1 |
| F05-K15-52 | C5 | ROM 053: SD-Paper ROM 053: SD-Paper Off IFX 1 |
| F05-K15-53 | C#5 | ROM 054: SD-Classic Room ROM 054: SD-Classic Room Off IFX 1 |
| F05-K15-54 | D5 | ROM 055: SD-Brush Hit ROM 055: SD-Brush Hit Off IFX 1 |
| F05-K15-55 | D#5 | ROM 056: SD-Brush Tap 1 ROM 056: SD-Brush Tap 1 Off IFX 1 |
| F05-K15-56 | E5 | ROM 057: SD-Brush Tap 2 ROM 057: SD-Brush Tap 2 Off IFX 1 |
| F05-K15-57 | F5 | ROM 058: SD-Brush Swirl ROM 058: SD-Brush Swirl Off IFX 1 |
| F05-K15-58 | F#5 | ROM 059: SD-Atomic ROM 059: SD-Atomic Off IFX 1 |
| F05-K15-59 | G5 | ROM 060: SD-Big Rock ROM 060: SD-Big Rock Off IFX 1 |
| F05-K15-60 | G#5 | ROM 061: SD-Yowie ROM 061: SD-Yowie Off IFX 1 |
| F05-K15-61 | A5 | ROM 062: SD-Processed 1 ROM 062: SD-Processed 1 Off IFX 1 |
| F05-K15-62 | A#5 | ROM 063: SD-Processed 2 ROM 063: SD-Processed 2 Off IFX 1 |
| F05-K15-63 | B5 | ROM 064: SD-Cracker Room ROM 064: SD-Cracker Room Off IFX 1 |
| F05-K15-64 | C6 | EX 025: Sidestick-Amb2mf EX 025: Sidestick-Amb2mf Off IFX 1 |
| F05-K15-65 | C#6 | ROM 065: Sidestick-Dry ROM 65: Sidestick-Dry Off IFX 1 |
| F05-K15-66 | D6 | ROM 171: SD-Orch.Roll 2 ROM 170: SD-Orch.Roll 1 Off IFX 1 |
| F05-K15-67 | D#6 | ROM 152: Crash Cymbal 1 ROM 152: Crash Cymbal 1 Off L/R |
| F05-K15-68 | E6 | ROM 172: SD-Orchestral ROM 172: SD-Orchestral Off IFX 1 |
| F05-K15-69 | F6 | ROM 073: SD-Reverse ROM 073: SD-Reverse Off IFX 1 |
| F05-K15-70 | F#6 | EX 053: H.Hat 3-Close f EX 052: H.Hat 3-Close mf 1 L/R |
| F05-K15-71 | G6 | ROM 078: SD-Hip 5 ROM 078: SD-Hip 5 Off IFX 1 |
| F05-K15-72 | G#6 | EX 054: H.Hat 3-Foot EX 054: H.Hat 3-Foot 1 L/R |
| F05-K15-73 | A6 | ROM 091: SD-AmbiHop ROM 091: SD-AmbiHop Off IFX 1 |
| F05-K15-74 | A#6 | EX 055: H.Hat 3-Open EX 055: H.Hat 3-Open 1 L/R |
| F05-K15-75 | B6 | ROM 095: SD-Whopper ROM 095: SD-Whopper Off IFX 1 |
| F05-K15-76 | C7 | ROM 097: SD-Ambi Pop ROM 097: SD-Ambi Pop Off IFX 1 |
| F05-K15-77 | C#7 | ROM 041: SD-Dry 2 ROM 041: SD-Dry 2 Off IFX 1 |
| F05-K15-78 | D7 | ROM 042: SD-Dry 3 ROM 042: SD-Dry 3 Off IFX 1 |
| F05-K15-79 | D#7 | ROM 043: SD-Normal ROM 043: SD-Normal Off IFX 1 |
| F05-K15-80 | E7 | ROM 044: SD-GhostSet roll ROM 044: SD-GhostSet roll Off IFX 1 |
| F05-K15-81 | F7 | ROM 047: SD-GhostSet p2 ROM 047: SD-GhostSet p2 Off IFX 1 |
| F05-K15-82 | F#7 | ROM 046: SD-GhostSet p1 ROM 046: SD-GhostSet p1 Off IFX 1 |
| F05-K15-83 | G7 | ROM 045: SD-GhostSet f ROM 045: SD-GhostSet f Off IFX 1 |
| F05-K15-84 | G#7 | ROM 048: SD-Full Room ROM 048: SD-Full Room Off IFX 1 |
| F05-K15-85 | A7 |  |
| F05-K15-86 | A#7 |  |
| F05-K15-87 | B7 |  |
| F05-K15-88 | C8 |  |

### F05-K16 - BD/SD Catalog 2 (kit 15, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-K16-01 | A0 |  |
| F05-K16-02 | A#0 |  |
| F05-K16-03 | B0 |  |
| F05-K16-04 | C1 | ROM 022: BD-Hip 1 ROM 022: BD-Hip 1 Off IFX 1 |
| F05-K16-05 | C#1 | ROM 174: 88-BD 2(R&B) ROM 174: 88-BD 2(R&B) Off IFX 1 |
| F05-K16-06 | D1 | ROM 173: 88-BD 1(Rap) ROM 173: 88-BD 1(Rap) Off IFX 1 |
| F05-K16-07 | D#1 | ROM 039: BD-Klanger ROM 039: BD-Klanger Off IFX 1 |
| F05-K16-08 | E1 | ROM 038: BD-Cannon ROM 038: BD-Cannon Off IFX 1 |
| F05-K16-09 | F1 | ROM 175: 88-BD 3 ROM 175: 88-BD 3 Off IFX 1 |
| F05-K16-10 | F#1 | ROM 180: 99-BD 5 ROM 180: 99-BD 5 Off IFX 1 |
| F05-K16-11 | G1 | ROM 036: BD-Chili ROM 036: BD-Chili Off IFX 1 |
| F05-K16-12 | G#1 | ROM 035: BD-AmbiRocker ROM 035: BD-AmbiRocker Off IFX 1 |
| F05-K16-13 | A1 | ROM 033: BD-Ambi Soft ROM 033: BD-Ambi Soft Off IFX 1 |
| F05-K16-14 | A#1 | ROM 032: BD-Ambi Crackle ROM 032: BD-Ambi Crackle Off IFX 1 |
| F05-K16-15 | B1 | ROM 031: BD-Ambi ROM 031: BD-Ambi Off IFX 1 |
| F05-K16-16 | C2 | ROM 005: BD-Dry 6 ROM 005: BD-Dry 6 Off IFX 1 |
| F05-K16-17 | C#2 | ROM 018: BD-Dance 1 ROM 018: BD-Dance 1 Off IFX 1 |
| F05-K16-18 | D2 | ROM 019: BD-Dance 2 ROM 019: BD-Dance 2 Off IFX 1 |
| F05-K16-19 | D#2 | ROM 178: 99-BD 3 ROM 178: 99-BD 3 Off IFX 1 |
| F05-K16-20 | E2 | ROM 021: BD-Dance 4 ROM 021: BD-Dance 4 Off IFX 1 |
| F05-K16-21 | F2 | ROM 174: 88-BD 2(R&B) ROM 174: 88-BD 2(R&B) Off IFX 1 |
| F05-K16-22 | F#2 | ROM 023: BD-Hip 2 ROM 023: BD-Hip 2 Off IFX 1 |
| F05-K16-23 | G2 | ROM 024: BD-Hip 3 ROM 024: BD-Hip 3 Off IFX 1 |
| F05-K16-24 | G#2 | ROM 025: BD-Hip 4 ROM 025: BD-Hip 4 Off IFX 1 |
| F05-K16-25 | A2 | ROM 027: BD-Pop ROM 027: BD-Pop Off IFX 1 |
| F05-K16-26 | A#2 | ROM 029: BD-Short88 ROM 029: BD-Short88 Off IFX 1 |
| F05-K16-27 | B2 | ROM 030: BD-Dance99 ROM 030: BD-Dance99 Off IFX 1 |
| F05-K16-28 | C3 | EX 002: BD-Dance 5 EX 002: BD-Dance 5 Off IFX 1 |
| F05-K16-29 | C#3 | EX 003: BD-Dance 6 EX 003: BD-Dance 6 Off IFX 1 |
| F05-K16-30 | D3 | EX 004: BD-Dance 7 EX 004: BD-Dance 7 Off IFX 1 |
| F05-K16-31 | D#3 | EX 005: BD-Dance 8 EX 005: BD-Dance 8 Off IFX 1 |
| F05-K16-32 | E3 | EX 006: BD-Dance 9 EX 006: BD-Dance 9 Off IFX 1 |
| F05-K16-33 | F3 | EX 007: BD-Dance 10 EX 007: BD-Dance 10 Off IFX 1 |
| F05-K16-34 | F#3 | EX 008: BD-Dance 11 EX 008: BD-Dance 11 Off IFX 1 |
| F05-K16-35 | G3 | EX 009: BD-Dance 12 EX 009: BD-Dance 12 Off IFX 1 |
| F05-K16-36 | G#3 | EX 010: BD-Dance 13 EX 010: BD-Dance 13 Off IFX 1 |
| F05-K16-37 | A3 | EX 011: BD-Dance 14 EX 011: BD-Dance 14 Off IFX 1 |
| F05-K16-38 | A#3 | EX 012: BD-Dance 15 EX 012: BD-Dance 15 Off IFX 1 |
| F05-K16-39 | B3 | ROM 176: 99-BD 1 ROM 176: 99-BD 1 Off IFX 1 |
| F05-K16-40 | C4 | ROM 177: 99-BD 2 ROM 177: 99-BD 2 Off IFX 1 |
| F05-K16-41 | C#4 | ROM 179: 99-BD 4 ROM 179: 99-BD 4 Off IFX 1 |
| F05-K16-42 | D4 | ROM 067: SD-Amb.House ROM 067: SD-Amb.House Off IFX 1 |
| F05-K16-43 | D#4 | ROM 068: SD-Dance ROM 068: SD-Dance Off IFX 1 |
| F05-K16-44 | E4 | ROM 069: SD-Rap 1 ROM 069: SD-Rap 1 Off IFX 1 |
| F05-K16-45 | F4 | ROM 070: SD-Rap 2 ROM 070: SD-Rap 2 Off IFX 1 |
| F05-K16-46 | F#4 | ROM 071: SD-Noise ROM 071: SD-Noise Off IFX 1 |
| F05-K16-47 | G4 | ROM 072: SD-Sizzle ROM 072: SD-Sizzle Off IFX 1 |
| F05-K16-48 | G#4 | ROM 074: SD-Hip 1 ROM 074: SD-Hip 1 Off IFX 1 |
| F05-K16-49 | A4 | ROM 075: SD-Hip 2 ROM 075: SD-Hip 2 Off IFX 1 |
| F05-K16-50 | A#4 | ROM 076: SD-Hip 3 ROM 076: SD-Hip 3 Off IFX 1 |
| F05-K16-51 | B4 | ROM 077: SD-Hip 4 ROM 077: SD-Hip 4 Off IFX 1 |
| F05-K16-52 | C5 | EX 028: SD-Dance 3 EX 028: SD-Dance 3 Off IFX 1 |
| F05-K16-53 | C#5 | EX 027: SD-Dance 2 EX 027: SD-Dance 2 Off IFX 1 |
| F05-K16-54 | D5 | EX 029: SD-Dance 4 EX 029: SD-Dance 4 Off IFX 1 |
| F05-K16-55 | D#5 | EX 030: SD-Dance 5 EX 030: SD-Dance 5 Off IFX 1 |
| F05-K16-56 | E5 | EX 031: SD-Dance 6 EX 031: SD-Dance 6 Off IFX 1 |
| F05-K16-57 | F5 | EX 032: SD-Dance 7 EX 032: SD-Dance 7 Off IFX 1 |
| F05-K16-58 | F#5 | EX 033: SD-Dance 8 EX 033: SD-Dance 8 Off IFX 1 |
| F05-K16-59 | G5 | EX 034: SD-Dance 9 EX 034: SD-Dance 9 Off IFX 1 |
| F05-K16-60 | G#5 | EX 035: SD-Dance 10 EX 035: SD-Dance 10 Off IFX 1 |
| F05-K16-61 | A5 | EX 036: SD-Dance 11 EX 036: SD-Dance 11 Off IFX 1 |
| F05-K16-62 | A#5 | EX 037: SD-Dance 12 EX 037: SD-Dance 12 Off IFX 1 |
| F05-K16-63 | B5 | EX 038: SD-Dance 13 EX 038: SD-Dance 13 Off IFX 1 |
| F05-K16-64 | C6 | EX 039: SD-Dance 14 EX 039: SD-Dance 14 Off IFX 1 |
| F05-K16-65 | C#6 | EX 040: SD-Dance 15 EX 040: SD-Dance 15 Off IFX 1 |
| F05-K16-66 | D6 | EX 041: SD-Dance 16 EX 041: SD-Dance 16 Off IFX 1 |
| F05-K16-67 | D#6 | EX 042: SD-Dance 17 EX 042: SD-Dance 17 Off IFX 1 |
| F05-K16-68 | E6 | EX 043: SD-Dance 18 EX 043: SD-Dance 18 Off IFX 1 |
| F05-K16-69 | F6 | EX 044: SD-Dance 19 EX 044: SD-Dance 19 Off IFX 1 |
| F05-K16-70 | F#6 | EX 062: H.Hat-Dance 1 EX 062: H.Hat-Dance 1 1 L/R |
| F05-K16-71 | G6 | ROM 100: SD-Ambi Hit 3 ROM 100: SD-Ambi Hit 3 Off IFX 1 |
| F05-K16-72 | G#6 | EX 066: H.Hat-Dance 5 EX 066: H.Hat-Dance 5 1 L/R |
| F05-K16-73 | A6 | ROM 181: 88-SD 1(New) ROM 181: 88-SD 1(New) Off IFX 1 |
| F05-K16-74 | A#6 | EX 067: H.Hat-Dance 6 EX 067: H.Hat-Dance 6 1 L/R |
| F05-K16-75 | B6 | ROM 182: 88-SD 2(Old) ROM 182: 88-SD 2(Old) Off IFX 1 |
| F05-K16-76 | C7 | ROM 183: 99-SD 1 ROM 183: 99-SD 1 Off IFX 1 |
| F05-K16-77 | C#7 | ROM 184: 99-SD 2 ROM 184: 99-SD 2 Off IFX 1 |
| F05-K16-78 | D7 | ROM 199: Vocal SD ROM 199: Vocal SD Off IFX 1 |
| F05-K16-79 | D#7 | ROM 079: SD-Hip 6 ROM 079: SD-Hip 6 Off IFX 1 |
| F05-K16-80 | E7 | ROM 080: SD-Ringy ROM 080: SD-Ringy Off IFX 1 |
| F05-K16-81 | F7 | ROM 081: SD-Tiny ROM 081: SD-Tiny Off IFX 1 |
| F05-K16-82 | F#7 | ROM 082: SD-Vintage 1 ROM 082: SD-Vintage 1 Off IFX 1 |
| F05-K16-83 | G7 | ROM 083: SD-Vintage 2 ROM 083: SD-Vintage 2 Off IFX 1 |
| F05-K16-84 | G#7 | ROM 084: SD-Vintage 3 ROM 084: SD-Vintage 3 Off IFX 1 |
| F05-K16-85 | A7 | ROM 085: SD-Vintage 4 ROM 085: SD-Vintage 4 Off IFX 1 |
| F05-K16-86 | A#7 | ROM 086: SD-Vintage 5 ROM 086: SD-Vintage 5 Off IFX 1 |
| F05-K16-87 | B7 | ROM 087: SD-Vintage 6 ROM 087: SD-Vintage 6 Off IFX 1 |
| F05-K16-88 | C8 | ROM 088: SD-AmbiCrackle 1 ROM 088: SD-AmbiCrackle 1 Off IFX 1 |
| F05-K16-89 | C#8 | ROM 089: SD-AmbiCrackle 2 ROM 089: SD-AmbiCrackle 2 Off IFX 1 |
| F05-K16-90 | D8 | ROM 090: SD-AmbiCrackle 3 ROM 090: SD-AmbiCrackle 3 Off IFX 1 |
| F05-K16-91 | D#8 | ROM 092: SD-Brasser ROM 092: SD-Brasser Off IFX 1 |
| F05-K16-92 | E8 | ROM 093: SD-Chili ROM 093: SD-Chili Off IFX 1 |
| F05-K16-93 | F8 | ROM 094: SD-Klanger ROM 094: SD-Klanger Off IFX 1 |
| F05-K16-94 | F#8 | ROM 096: SD-Dry Present ROM 096: SD-Dry Present Off IFX 1 |
| F05-K16-95 | G8 | ROM 098: SD-Ambi Hit 1 ROM 098: SD-Ambi Hit 1 Off IFX 1 |
| F05-K16-96 | G#8 | ROM 099: SD-Ambi Hit 2 ROM 099: SD-Ambi Hit 2 Off IFX 1 |

### F05-GM01 - STANDARD (GM kit 00, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM01-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM01-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM01-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM01-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM01-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM01-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM01-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM01-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM01-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM01-10 | A1 | ROM 360: Click Off L/R |
| F05-GM01-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM01-12 | B1 | ROM 016: BD-Tight Off L/R |
| F05-GM01-13 | C2 | ROM 002: BD-Dry 3 Off L/R |
| F05-GM01-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM01-15 | D2 | ROM 048: SD-Full Room Off L/R |
| F05-GM01-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM01-17 | E2 | ROM 048: SD-Full Room Off L/R |
| F05-GM01-18 | F2 | ROM 106: Tom 2-Floor Off L/R |
| F05-GM01-19 | F#2 | ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-GM01-20 | G2 | ROM 106: Tom 2-Floor Off L/R |
| F05-GM01-21 | G#2 | ROM 127: H.Hat 2-Foot 1 L/R |
| F05-GM01-22 | A2 | ROM 105: Tom 2-Lo Off L/R |
| F05-GM01-23 | A#2 | ROM 130: H.Hat 2-Open 2 1 L/R |
| F05-GM01-24 | B2 | ROM 105: Tom 2-Lo Off L/R |
| F05-GM01-25 | C3 | ROM 104: Tom 2-Hi Off L/R |
| F05-GM01-26 | C#3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM01-27 | D3 | ROM 104: Tom 2-Hi Off L/R |
| F05-GM01-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM01-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM01-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM01-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM01-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM01-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM01-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM01-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM01-36 | B3 | ROM 161: Ride-Dance DDD1 Off L/R |
| F05-GM01-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM01-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM01-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM01-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM01-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM01-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM01-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM01-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM01-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM01-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM01-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM01-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM01-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM01-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM01-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM01-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM01-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM01-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM01-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM01-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM01-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM01-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM01-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM01-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM01-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM01-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM01-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM01-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM01-65 | E6 | ROM 354: Stadium Off L/R |

### F05-GM02 - ROOM (GM kit 01, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM02-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM02-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM02-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM02-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM02-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM02-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM02-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM02-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM02-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM02-10 | A1 | ROM 360: Click Off L/R |
| F05-GM02-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM02-12 | B1 | ROM 001: BD-Dry 2 Off L/R |
| F05-GM02-13 | C2 | ROM 002: BD-Dry 3 Off L/R |
| F05-GM02-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM02-15 | D2 | ROM 061: SD-Yowie Off L/R |
| F05-GM02-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM02-17 | E2 | ROM 061: SD-Yowie Off L/R |
| F05-GM02-18 | F2 | ROM 109: Tom 3-Floor Off L/R |
| F05-GM02-19 | F#2 | ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-GM02-20 | G2 | ROM 109: Tom 3-Floor Off L/R |
| F05-GM02-21 | G#2 | ROM 127: H.Hat 2-Foot 1 L/R |
| F05-GM02-22 | A2 | ROM 108: Tom 3-Lo Off L/R |
| F05-GM02-23 | A#2 | ROM 130: H.Hat 2-Open 2 1 L/R |
| F05-GM02-24 | B2 | ROM 108: Tom 3-Lo Off L/R |
| F05-GM02-25 | C3 | ROM 107: Tom 3-Hi Off L/R |
| F05-GM02-26 | C#3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM02-27 | D3 | ROM 107: Tom 3-Hi Off L/R |
| F05-GM02-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM02-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM02-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM02-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM02-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM02-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM02-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM02-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM02-36 | B3 | ROM 161: Ride-Dance DDD1 Off L/R |
| F05-GM02-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM02-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM02-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM02-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM02-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM02-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM02-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM02-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM02-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM02-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM02-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM02-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM02-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM02-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM02-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM02-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM02-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM02-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM02-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM02-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM02-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM02-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM02-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM02-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM02-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM02-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM02-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM02-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM02-65 | E6 | ROM 354: Stadium Off L/R |

### F05-GM03 - POWER (GM kit 02, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM03-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM03-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM03-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM03-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM03-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM03-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM03-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM03-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM03-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM03-10 | A1 | ROM 360: Click Off L/R |
| F05-GM03-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM03-12 | B1 | ROM 015: BD-Gated Off L/R |
| F05-GM03-13 | C2 | ROM 012: BD-Mondo Kill Off L/R |
| F05-GM03-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM03-15 | D2 | ROM 060: SD-Big Rock Off L/R |
| F05-GM03-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM03-17 | E2 | ROM 060: SD-Big Rock Off L/R |
| F05-GM03-18 | F2 | ROM 109: Tom 3-Floor Off L/R |
| F05-GM03-19 | F#2 | ROM 123: H.Hat 1-Open 2 1 L/R |
| F05-GM03-20 | G2 | ROM 109: Tom 3-Floor Off L/R |
| F05-GM03-21 | G#2 | ROM 127: H.Hat 2-Foot 1 L/R |
| F05-GM03-22 | A2 | ROM 108: Tom 3-Lo Off L/R |
| F05-GM03-23 | A#2 | ROM 130: H.Hat 2-Open 2 1 L/R |
| F05-GM03-24 | B2 | ROM 108: Tom 3-Lo Off L/R |
| F05-GM03-25 | C3 | ROM 107: Tom 3-Hi Off L/R |
| F05-GM03-26 | C#3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM03-27 | D3 | ROM 107: Tom 3-Hi Off L/R |
| F05-GM03-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM03-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM03-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM03-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM03-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM03-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM03-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM03-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM03-36 | B3 | ROM 161: Ride-Dance DDD1 Off L/R |
| F05-GM03-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM03-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM03-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM03-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM03-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM03-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM03-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM03-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM03-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM03-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM03-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM03-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM03-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM03-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM03-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM03-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM03-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM03-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM03-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM03-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM03-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM03-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM03-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM03-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM03-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM03-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM03-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM03-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM03-65 | E6 | ROM 354: Stadium Off L/R |

### F05-GM04 - ELECTRONIC (GM kit 03, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM04-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM04-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM04-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM04-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM04-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM04-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM04-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM04-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM04-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM04-10 | A1 | ROM 360: Click Off L/R |
| F05-GM04-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM04-12 | B1 | ROM 022: BD-Hip 1 Off L/R |
| F05-GM04-13 | C2 | ROM 030: BD-Dance99 Off L/R |
| F05-GM04-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM04-15 | D2 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM04-17 | E2 | ROM 098: SD-Ambi Hit 1 Off L/R |
| F05-GM04-18 | F2 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-19 | F#2 | ROM 131: H.Hat 2-Sizzle 1 L/R |
| F05-GM04-20 | G2 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-21 | G#2 | ROM 127: H.Hat 2-Foot 1 L/R |
| F05-GM04-22 | A2 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-23 | A#2 | ROM 131: H.Hat 2-Sizzle 1 L/R |
| F05-GM04-24 | B2 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-25 | C3 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-26 | C#3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM04-27 | D3 | ROM 198: Real E.Tom Off L/R |
| F05-GM04-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM04-29 | E3 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-GM04-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM04-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM04-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM04-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM04-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM04-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM04-36 | B3 | ROM 161: Ride-Dance DDD1 Off L/R |
| F05-GM04-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM04-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM04-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM04-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM04-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM04-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM04-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM04-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM04-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM04-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM04-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM04-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM04-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM04-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM04-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM04-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM04-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM04-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM04-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM04-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM04-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM04-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM04-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM04-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM04-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM04-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM04-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM04-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM04-65 | E6 | ROM 354: Stadium Off L/R |

### F05-GM05 - ANALOG (GM kit 04, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM05-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM05-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM05-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM05-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM05-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM05-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM05-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM05-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM05-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM05-10 | A1 | ROM 360: Click Off L/R |
| F05-GM05-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM05-12 | B1 | ROM 174: 88-BD 2(R&B) Off L/R |
| F05-GM05-13 | C2 | ROM 176: 99-BD 1 Off L/R |
| F05-GM05-14 | C#2 | ROM 185: 88-Rimshot Off L/R |
| F05-GM05-15 | D2 | ROM 181: 88-SD 1(New) Off L/R |
| F05-GM05-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM05-17 | E2 | ROM 182: 88-SD 2(Old) Off L/R |
| F05-GM05-18 | F2 | ROM 192: 88-Tom Off L/R |
| F05-GM05-19 | F#2 | ROM 188: 88-HHat Open 1 L/R |
| F05-GM05-20 | G2 | ROM 192: 88-Tom Off L/R |
| F05-GM05-21 | G#2 | ROM 188: 88-HHat Open 1 L/R |
| F05-GM05-22 | A2 | ROM 192: 88-Tom Off L/R |
| F05-GM05-23 | A#2 | ROM 188: 88-HHat Open 1 L/R |
| F05-GM05-24 | B2 | ROM 192: 88-Tom Off L/R |
| F05-GM05-25 | C3 | ROM 192: 88-Tom Off L/R |
| F05-GM05-26 | C#3 | ROM 160: Ride-Dance 99 Off L/R |
| F05-GM05-27 | D3 | ROM 192: 88-Tom Off L/R |
| F05-GM05-28 | D#3 | ROM 191: 88-Crash Off L/R |
| F05-GM05-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM05-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM05-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM05-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM05-33 | G#3 | ROM 195: 88-Cowbell Off L/R |
| F05-GM05-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM05-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM05-36 | B3 | ROM 161: Ride-Dance DDD1 Off L/R |
| F05-GM05-37 | C4 | ROM 193: 88-Conga Off L/R |
| F05-GM05-38 | C#4 | ROM 193: 88-Conga Off L/R |
| F05-GM05-39 | D4 | ROM 192: 88-Tom Off L/R |
| F05-GM05-40 | D#4 | ROM 192: 88-Tom Off L/R |
| F05-GM05-41 | E4 | ROM 192: 88-Tom Off L/R |
| F05-GM05-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM05-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM05-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM05-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM05-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM05-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM05-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM05-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM05-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM05-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM05-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM05-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM05-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM05-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM05-56 | G5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM05-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM05-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM05-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM05-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM05-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM05-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM05-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM05-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM05-65 | E6 | ROM 354: Stadium Off L/R |

### F05-GM06 - JAZZ (GM kit 05, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM06-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM06-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM06-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM06-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM06-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM06-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM06-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM06-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM06-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM06-10 | A1 | ROM 360: Click Off L/R |
| F05-GM06-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM06-12 | B1 | ROM 003: BD-Dry 4 Off L/R |
| F05-GM06-13 | C2 | ROM 009: BD-Jazz 2 Off L/R |
| F05-GM06-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM06-15 | D2 | ROM 053: SD-Paper Off L/R |
| F05-GM06-16 | D#2 | ROM 186: 88-Claps Off L/R |
| F05-GM06-17 | E2 | ROM 050: SD-Jazz Ring Off L/R |
| F05-GM06-18 | F2 | ROM 113: Tom-Jazz Floor Off L/R |
| F05-GM06-19 | F#2 | ROM 118: H.Hat 1-Closed 1 1 L/R |
| F05-GM06-20 | G2 | ROM 113: Tom-Jazz Floor Off L/R |
| F05-GM06-21 | G#2 | ROM 120: H.Hat 1-Foot 1 L/R |
| F05-GM06-22 | A2 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM06-23 | A#2 | ROM 122: H.Hat 1-Open 1 1 L/R |
| F05-GM06-24 | B2 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM06-25 | C3 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM06-26 | C#3 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-GM06-27 | D3 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM06-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM06-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM06-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM06-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM06-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM06-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM06-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM06-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM06-36 | B3 | ROM 163: Ride-Edge 2 Off L/R |
| F05-GM06-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM06-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM06-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM06-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM06-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM06-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM06-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM06-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM06-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM06-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM06-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM06-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM06-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM06-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM06-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM06-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM06-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM06-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM06-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM06-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM06-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM06-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM06-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM06-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM06-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM06-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM06-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM06-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM06-65 | E6 | ROM 355: Applause Off L/R |

### F05-GM07 - BRUSH (GM kit 06, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM07-01 | C1 | ROM 172: SD-Orchestral 7 L/R |
| F05-GM07-02 | C#1 | ROM 171: SD-Orch.Roll 2 7 L/R |
| F05-GM07-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM07-04 | D#1 | ROM 206: Zap 2 Off L/R |
| F05-GM07-05 | E1 | ROM 395: Noise-White Off L/R |
| F05-GM07-06 | F1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM07-07 | F#1 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM07-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM07-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM07-10 | A1 | ROM 360: Click Off L/R |
| F05-GM07-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM07-12 | B1 | ROM 003: BD-Dry 4 Off L/R |
| F05-GM07-13 | C2 | ROM 009: BD-Jazz 2 Off L/R |
| F05-GM07-14 | C#2 | ROM 066: Sidestick-Amb Off L/R: |
| F05-GM07-15 | D2 | ROM 056: SD-Brush Tap 1 Off L/R |
| F05-GM07-16 | D#2 | ROM 055: SD-Brush Hit Off L/R |
| F05-GM07-17 | E2 | ROM 058: SD-Brush Swirl Off L/R |
| F05-GM07-18 | F2 | ROM 113: Tom-Jazz Floor Off L/R |
| F05-GM07-19 | F#2 | ROM 118: H.Hat 1-Closed 1 1 L/R |
| F05-GM07-20 | G2 | ROM 113: Tom-Jazz Floor Off L/R |
| F05-GM07-21 | G#2 | ROM 120: H.Hat 1-Foot 1 L/R |
| F05-GM07-22 | A2 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM07-23 | A#2 | ROM 122: H.Hat 1-Open 1 1 L/R |
| F05-GM07-24 | B2 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM07-25 | C3 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM07-26 | C#3 | ROM 152: Crash Cymbal 1 Off L/R |
| F05-GM07-27 | D3 | ROM 111: Tom-Jazz Hi Off L/R |
| F05-GM07-28 | D#3 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM07-29 | E3 | ROM 155: China Cymbal Off L/R |
| F05-GM07-30 | F3 | ROM 164: Ride-Jazz Off L/R |
| F05-GM07-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM07-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM07-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM07-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM07-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM07-36 | B3 | ROM 163: Ride-Edge 2 Off L/R |
| F05-GM07-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM07-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM07-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM07-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM07-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM07-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM07-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM07-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM07-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM07-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM07-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM07-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM07-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM07-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM07-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM07-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM07-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM07-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM07-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM07-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM07-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM07-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM07-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM07-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM07-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM07-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM07-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM07-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM07-65 | E6 | ROM 355: Applause Off L/R |

### F05-GM08 - ORCHESTRA (GM kit 07, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM08-01 | C1 | ROM 172: SD-Orchestral Off L/R |
| F05-GM08-02 | C#1 | ROM 171: SD-Orch.Roll 2 Off L/R |
| F05-GM08-03 | D1 | ROM 228: Finger Snap Off L/R |
| F05-GM08-04 | D#1 | ROM 118: H.Hat 1-Closed 1 1 L/R |
| F05-GM08-05 | E1 | ROM 120: H.Hat 1-Foot 1 L/R |
| F05-GM08-06 | F1 | ROM 122: H.Hat 1-Open 1 1 L/R |
| F05-GM08-07 | F#1 | ROM 162: Ride-Edge 1 Off L/R |
| F05-GM08-08 | G1 | ROM 227: Drumstick Hit Off L/R |
| F05-GM08-09 | G#1 | ROM 205: Zap 1 Off L/R |
| F05-GM08-10 | A1 | ROM 360: Click Off L/R |
| F05-GM08-11 | A#1 | ROM 292: Triangle-Open Off L/R |
| F05-GM08-12 | B1 | ROM 007: BD-Soft Room Off L/R |
| F05-GM08-13 | C2 | ROM 166: BD-Orchestral Off L/R |
| F05-GM08-14 | C#2 | ROM 066: Sidestick-Amb Off L/R |
| F05-GM08-15 | D2 | ROM 172: SD-Orchestral Off L/R |
| F05-GM08-16 | D#2 | ROM 252: Castanet-Single Off L/R |
| F05-GM08-17 | E2 | ROM 172: SD-Orchestral Off L/R |
| F05-GM08-18 | F2 | ROM 169: Timpani Off L/R |
| F05-GM08-19 | F#2 | ROM 169: Timpani Off L/R |
| F05-GM08-20 | G2 | ROM 169: Timpani Off L/R |
| F05-GM08-21 | G#2 | ROM 169: Timpani Off L/R |
| F05-GM08-22 | A2 | ROM 169: Timpani Off L/R |
| F05-GM08-23 | A#2 | ROM 169: Timpani Off L/R |
| F05-GM08-24 | B2 | ROM 169: Timpani Off L/R |
| F05-GM08-25 | C3 | ROM 169: Timpani Off L/R |
| F05-GM08-26 | C#3 | ROM 169: Timpani Off L/R |
| F05-GM08-27 | D3 | ROM 169: Timpani Off L/R |
| F05-GM08-28 | D#3 | ROM 169: Timpani Off L/R |
| F05-GM08-29 | E3 | ROM 169: Timpani Off L/R |
| F05-GM08-30 | F3 | ROM 169: Timpani Off L/R |
| F05-GM08-31 | F#3 | ROM 291: Tambourine-Acc.2 Off L/R |
| F05-GM08-32 | G3 | ROM 155: China Cymbal Off L/R |
| F05-GM08-33 | G#3 | ROM 306: Cowbell 2 Off L/R |
| F05-GM08-34 | A3 | ROM 153: Crash Cymbal 2 Off L/R |
| F05-GM08-35 | A#3 | ROM 277: Vibraslap Off L/R |
| F05-GM08-36 | B3 | ROM 168: Orch Cym.-Open Off L/R |
| F05-GM08-37 | C4 | ROM 245: Bongo Hi-Open Off L/R |
| F05-GM08-38 | C#4 | ROM 242: Bongo Lo-Open Off L/R |
| F05-GM08-39 | D4 | ROM 237: Conga Hi-MtSlap Off L/R |
| F05-GM08-40 | D#4 | ROM 235: Conga Hi-Open Off L/R |
| F05-GM08-41 | E4 | ROM 232: Conga Lo-Open Off L/R |
| F05-GM08-42 | F4 | ROM 286: Timbale Hi-Rim 2 Off L/R |
| F05-GM08-43 | F#4 | ROM 281: Timbale Lo-Open Off L/R |
| F05-GM08-44 | G4 | ROM 304: Agogo Bell Off L/R |
| F05-GM08-45 | G#4 | ROM 304: Agogo Bell Off L/R |
| F05-GM08-46 | A4 | ROM 299: Cabasa-Up Off L/R |
| F05-GM08-47 | A#4 | ROM 258: Maracas-Push Off L/R |
| F05-GM08-48 | B4 | ROM 316: Samba Whistle 2 L/R |
| F05-GM08-49 | C5 | ROM 316: Samba Whistle 2 L/R |
| F05-GM08-50 | C#5 | ROM 256: Guiro-Short 3 L/R |
| F05-GM08-51 | D5 | ROM 255: Guiro-Long 3 L/R |
| F05-GM08-52 | D#5 | ROM 278: Claves Off L/R |
| F05-GM08-53 | E5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM08-54 | F5 | ROM 279: Woodblock 1 Off L/R |
| F05-GM08-55 | F#5 | ROM 295: Cuica-Hi 4 L/R |
| F05-GM08-56 | G5 | ROM 296: Cuica-Lo 4 L/R |
| F05-GM08-57 | G#5 | ROM 293: Triangle-Mute 5 L/R |
| F05-GM08-58 | A5 | ROM 292: Triangle-Open 5 L/R |
| F05-GM08-59 | A#5 | ROM 300: Cabasa-Down Off L/R |
| F05-GM08-60 | B5 | ROM 309: Sleigh Bell Off L/R |
| F05-GM08-61 | C6 | ROM 312: Marc Tree Off L/R |
| F05-GM08-62 | C#6 | ROM 252: Castanet-Single Off L/R |
| F05-GM08-63 | D6 | ROM 282: Timbale Lo-Mute 6 L/R |
| F05-GM08-64 | D#6 | ROM 281: Timbale Lo-Open 6 L/R |
| F05-GM08-65 | E6 | ROM 355: Applause Off L/R |

### F05-GM09 - SFX (GM kit 08, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F05-GM09-01 | C1 |  |
| F05-GM09-02 | C#1 |  |
| F05-GM09-03 | D1 |  |
| F05-GM09-04 | D#1 |  |
| F05-GM09-05 | E1 |  |
| F05-GM09-06 | F1 |  |
| F05-GM09-07 | F#1 |  |
| F05-GM09-08 | G1 |  |
| F05-GM09-09 | G#1 |  |
| F05-GM09-10 | A1 |  |
| F05-GM09-11 | A#1 |  |
| F05-GM09-12 | B1 |  |
| F05-GM09-13 | C2 |  |
| F05-GM09-14 | C#2 |  |
| F05-GM09-15 | D2 |  |
| F05-GM09-16 | D#2 | ROM 206: Zap 2 Off L/R |
| F05-GM09-17 | E2 | ROM 370: GunShot Off L/R |
| F05-GM09-18 | F2 | ROM 208: DJ-Scratch 1 7 L/R |
| F05-GM09-19 | F#2 | ROM 209: DJ-Scratch 2 7 L/R |
| F05-GM09-20 | G2 | ROM 227: Drumstick Hit Off L/R |
| F05-GM09-21 | G#2 | ROM 228: Finger Snap Off L/R |
| F05-GM09-22 | A2 | ROM 360: Click Off L/R |
| F05-GM09-23 | A#2 | ROM 397: Tubular Off L/R |
| F05-GM09-24 | B2 | ROM 405: Fret Noise Off L/R |
| F05-GM09-25 | C3 | ROM 400: GtCutNois1 Off L/R |
| F05-GM09-26 | C#3 | ROM 401: GtCutNois2 Off L/R |
| F05-GM09-27 | D3 | ROM 402: StringSlap Off L/R |
| F05-GM09-28 | D#3 | ROM 228: Finger Snap Off L/R |
| F05-GM09-29 | E3 | ROM 357: Laughing Off L/R |
| F05-GM09-30 | F3 | ROM 356: Scream Off L/R |
| F05-GM09-31 | F#3 | ROM 368: Punch Off L/R |
| F05-GM09-32 | G3 | ROM 367: Hart Beat Off L/R |
| F05-GM09-33 | G#3 | ROM 359: Footstep 2 Off L/R |
| F05-GM09-34 | A3 | ROM 358: Footstep 1 Off L/R |
| F05-GM09-35 | A#3 | ROM 355: Applause Off L/R |
| F05-GM09-36 | B3 | ROM 372: Door Creak Off L/R |
| F05-GM09-37 | C4 | ROM 373: Door Slam Off L/R |
| F05-GM09-38 | C#4 | ROM 209: DJ-Scratch 2 Off L/R |
| F05-GM09-39 | D4 | ROM 312: Marc Tree Off L/R |
| F05-GM09-40 | D#4 | ROM 374: Car Engine Off L/R |
| F05-GM09-41 | E4 | ROM 375: Car Stop Off L/R |
| F05-GM09-42 | F4 | ROM 376: Car Pass Off L/R |
| F05-GM09-43 | F#4 | ROM 377: Car Crash Off L/R |
| F05-GM09-44 | G4 | ROM 366: Crickets 2 Off L/R |
| F05-GM09-45 | G#4 | ROM 378: Train Off L/R |
| F05-GM09-46 | A4 | ROM 395: Noise-White Off L/R |
| F05-GM09-47 | A#4 | ROM 379: Helicopter Off L/R |
| F05-GM09-48 | B4 | ROM 391: Swish Terra Off L/R |
| F05-GM09-49 | C5 | ROM 380: Gun Shot 2 Off L/R |
| F05-GM09-50 | C#5 | ROM 381: MachineGun Off L/R |
| F05-GM09-51 | D5 | ROM 382: Laser Gun Off L/R |
| F05-GM09-52 | D#5 | ROM 383: Explosion Off L/R |
| F05-GM09-53 | E5 | ROM 363: Dog Off L/R |
| F05-GM09-54 | F5 | ROM 364: Gallop Off L/R |
| F05-GM09-55 | F#5 | ROM 361: Bird 1 Off L/R |
| F05-GM09-56 | G5 | ROM 371: Rainstick Off L/R |
| F05-GM09-57 | G#5 | ROM 384: Thunder Off L/R |
| F05-GM09-58 | A5 | ROM 385: Wind Off L/R |
| F05-GM09-59 | A#5 | ROM 395: Noise-White Off L/R |
| F05-GM09-60 | B5 | ROM 386: Stream Off L/R |
| F05-GM09-61 | C6 | ROM 387: Bubble Off L/R |
| F05-GM09-62 | C#6 |  |
| F05-GM09-63 | D6 |  |
| F05-GM09-64 | D#6 |  |
| F05-GM09-65 | E6 |  |

