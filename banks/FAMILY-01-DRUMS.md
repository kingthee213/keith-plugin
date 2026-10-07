# FAMILY 01 - drum kits, drum sounds and pad mapping (internal reference)

Reference machine (internal name, never shipped): **Triton**.
Shipped label: **Family 01**.


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
| D1 | TRITON Voice Name List (Korg, file TRITON_VNL_EFG2.pdf, 409.5 KB) | official Korg download page, product TRITON, document id 1652 | https://www.korg.com/us/support/download/manual/1/214/1652/ |
| D2 | TRITON Parameter Guide (Korg, file TRITON_PG_E.pdf, 255 pages, created 1999-12-16) | official Korg download page, product TRITON, document id 1648 | https://www.korg.com/us/support/download/manual/1/214/1648/ |

## Counts as documented

- Factory drum kits: **16**
- GM drum kits: **9**
- Distinct drumsample numbers named in the PCM list: **413** (413 documented rows; the document re-lists samples in more than one group)
- Drum sounds: the document does not publish a separate drum-sound list for this machine beyond the drumsample names in the PCM list, which are listed in `FAMILY-01-PROGRAMS.md`.

The document prints each kit as a per-key table: the key label, the high drumsample, the low drumsample, the exclusive group and the bus select. Each cell below is the document's own text for that key, in that order. Where a key row is empty the kit does not assign that key.

## Factory drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F01-K01 | 000 | Standard Kit | 88 | D1 p24 |
| F01-K02 | 001 | Standard Kit 2 | 88 | D1 p24 |
| F01-K03 | 002 | Processed Kit | 88 | D1 p24 |
| F01-K04 | 003 | Jazz/Brush Kit | 88 | D1 p25 |
| F01-K05 | 004 | House Kit | 88 | D1 p25 |
| F01-K06 | 005 | Tricky Kit | 88 | D1 p25 |
| F01-K07 | 006 | Drum'n'Bass Kit | 88 | D1 p26 |
| F01-K08 | 007 | Hip Hop Kit | 88 | D1 p26 |
| F01-K09 | 008 | Psycho Kit | 88 | D1 p26 |
| F01-K10 | 009 | Percussion Kit | 88 | D1 p27 |
| F01-K11 | 010 | Orchestra&Ethnic | 88 | D1 p27 |
| F01-K12 | 011 | Cymbals Catalog | 88 | D1 p27 |
| F01-K13 | 012 | BD&SD Catalog 1 | 88 | D1 p28 |
| F01-K14 | 013 | BD&SD Catalog 2 | 88 | D1 p28 |
| F01-K15 | 014 | 01/W Total Kit | 88 | D1 p28 |
| F01-K16 | 015 | 01/W Producer's | 88 | D1 p29 |

## GM drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F01-GM01 | 00 | STANDARD | 65 | D1 p30 |
| F01-GM02 | 01 | ROOM | 65 | D1 p30 |
| F01-GM03 | 02 | POWER | 65 | D1 p30 |
| F01-GM04 | 03 | ELECTRONIC | 65 | D1 p30 |
| F01-GM05 | 04 | ANALOG | 65 | D1 p30 |
| F01-GM06 | 05 | JAZZ | 65 | D1 p31 |
| F01-GM07 | 06 | BRUSH | 65 | D1 p31 |
| F01-GM08 | 07 | ORCHESTRA | 65 | D1 p31 |
| F01-GM09 | 08 | SFX | 65 | D1 p31 |

### F01-K01 - Standard Kit (kit 000, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K01-01 | A0 |  |
| F01-K01-02 | A#0 |  |
| F01-K01-03 | B0 |  |
| F01-K01-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K01-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K01-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off L/R |
| F01-K01-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off L/R |
| F01-K01-08 | E1 | 261:Baya-Ghe 260:Baya-Open 9 L/R |
| F01-K01-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K01-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K01-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K01-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K01-13 | A1 | 227:Drumstick Hit Off L/R |
| F01-K01-14 | A#1 | 157:Cymbal Reverse Off L/R |
| F01-K01-15 | B1 | 073:SD-Reverse Off IFX1 |
| F01-K01-16 | C2 | 000:BD-Dry 1 000:BD-Dry 1 Off IFX2 |
| F01-K01-17 | C#2 | 003:BD-Dry 4 003:BD-Dry 4 Off IFX2 |
| F01-K01-18 | D2 | 006:BD-Soft 006:BD-Soft Off IFX2 |
| F01-K01-19 | D#2 | 007:BD-Soft Room 007:BD-Soft Room Off IFX2 |
| F01-K01-20 | E2 | 010:BD-Pillow 010:BD-Pillow Off IFX2 |
| F01-K01-21 | F2 | 048:SD-Full Room 048:SD-Full Room Off IFX1 |
| F01-K01-22 | F#2 | 054:SD-Classic Room 054:SD-Classic Room Off IFX1 |
| F01-K01-23 | G2 | 051:SD-Amb.Piccolo 1 051:SD-Amb.Piccolo 1 Off IFX1 |
| F01-K01-24 | G#2 | 053:SD-Paper 053:SD-Paper Off IFX1 |
| F01-K01-25 | A2 | 042:SD-Dry 3 042:SD-Dry 3 Off IFX1 |
| F01-K01-26 | A#2 | 040:SD-Dry 1 040:SD-Dry 1 Off IFX1 |
| F01-K01-27 | B2 | 002:BD-Dry 3 002:BD-Dry 3 Off IFX2 |
| F01-K01-28 | C3 | 001:BD-Dry 2 001:BD-Dry 2 Off IFX2 |
| F01-K01-29 | C#3 | 065:Sidestick-Dry 065:Sidestick-Dry Off L/R |
| F01-K01-30 | D3 | 052:SD-Amb.Piccolo 2 052:SD-Amb.Piccolo 2 Off IFX1 |
| F01-K01-31 | D#3 | 229:Hand Claps 229:Hand Claps Off L/R |
| F01-K01-32 | E3 | 060:SD-Big Rock 060:SD-Big Rock Off IFX1 |
| F01-K01-33 | F3 | 106:Tom 2-Floor 106:Tom 2-Floor Off L/R |
| F01-K01-34 | F#3 | 119:H.Hat 1-Closed 2 119:H.Hat 1-Closed 2 1 L/R |
| F01-K01-35 | G3 | 106:Tom 2-Floor 106:Tom 2-Floor Off L/R |
| F01-K01-36 | G#3 | 121:H.Hat 1-FootOpen 120:H.Hat 1-Foot 1 L/R |
| F01-K01-37 | A3 | 105:Tom 2-Lo 105:Tom 2-Lo Off L/R |
| F01-K01-38 | A#3 | 122:H.Hat 1-Open 1 123:H.Hat 1-Open 2 1 L/R |
| F01-K01-39 | B3 | 105:Tom 2-Lo 105:Tom 2-Lo Off L/R |
| F01-K01-40 | C4 | 104:Tom 2-Hi 104:Tom 2-Hi Off L/R |
| F01-K01-41 | C#4 | 153:Crash Cymbal 2 Off L/R |
| F01-K01-42 | D4 | 104:Tom 2-Hi 104:Tom 2-Hi Off L/R |
| F01-K01-43 | D#4 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K01-44 | E4 | 155:China Cymbal 155:China Cymbal Off L/R |
| F01-K01-45 | F4 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K01-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off L/R |
| F01-K01-47 | G4 | 156:Splash Cymbal 156:Splash Cymbal Off L/R |
| F01-K01-48 | G#4 | 305:Cowbell 1 Off L/R |
| F01-K01-49 | A4 | 152:Crash Cymbal 1 Off L/R |
| F01-K01-50 | A#4 | 277:Vibraslap Off L/R |
| F01-K01-51 | B4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K01-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K01-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K01-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K01-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K01-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K01-57 | F5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K01-58 | F#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K01-59 | G5 | 304:Agogo Bell Off L/R |
| F01-K01-60 | G#5 | 304:Agogo Bell Off L/R |
| F01-K01-61 | A5 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K01-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K01-63 | B5 | 316:Samba Whistle 2 L/R |
| F01-K01-64 | C6 | 316:Samba Whistle 2 L/R |
| F01-K01-65 | C#6 | 256:Guiro-Short 257:Guiro-Tap 3 L/R |
| F01-K01-66 | D6 | 255:Guiro-Long 3 L/R |
| F01-K01-67 | D#6 | 278:Claves Off L/R |
| F01-K01-68 | E6 | 279:Woodblock 1 Off L/R |
| F01-K01-69 | F6 | 279:Woodblock 1 Off L/R |
| F01-K01-70 | F#6 | 295:Cuica-Hi 4 L/R |
| F01-K01-71 | G6 | 296:Cuica-Lo 4 L/R |
| F01-K01-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 5 L/R |
| F01-K01-73 | A6 | 292:Triangle-Open 5 L/R |
| F01-K01-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K01-75 | B6 | 312:Marc Tree Off L/R |
| F01-K01-76 | C7 | 174:88-BD 2(R&B) 6 L/R |
| F01-K01-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 7 L/R |
| F01-K01-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K01-79 | D#7 | 188:88-HHat Open 7 L/R |
| F01-K01-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K01-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K01-82 | F#7 | 186:88-Claps Off L/R |
| F01-K01-83 | G7 | 196:88-Maraca Off L/R |
| F01-K01-84 | G#7 | 171:SD-Orch.Roll 2 8 L/R |
| F01-K01-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 8 L/R |
| F01-K01-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K01-87 | B7 | 354:Stadium Off L/R |
| F01-K01-88 | C8 | 355:Applause Off L/R |

### F01-K02 - Standard Kit 2 (kit 001, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K02-01 | A0 |  |
| F01-K02-02 | A#0 |  |
| F01-K02-03 | B0 |  |
| F01-K02-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K02-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K02-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off L/R |
| F01-K02-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off L/R |
| F01-K02-08 | E1 | 261:Baya-Ghe 260:Baya-Open 9 L/R |
| F01-K02-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K02-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K02-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K02-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K02-13 | A1 | 227:Drumstick Hit Off L/R |
| F01-K02-14 | A#1 | 157:Cymbal Reverse Off L/R |
| F01-K02-15 | B1 | 073:SD-Reverse Off IFX1 |
| F01-K02-16 | C2 | 014:BD-Tubby 014:BD-Tubby Off IFX2 |
| F01-K02-17 | C#2 | 015:BD-Gated 015:BD-Gated Off IFX2 |
| F01-K02-18 | D2 | 036:BD-Chili 036:BD-Chili Off IFX2 |
| F01-K02-19 | D#2 | 025:BD-Hip 4 025:BD-Hip 4 Off IFX2 |
| F01-K02-20 | E2 | 037:BD-PhatJaz 037:BD-PhatJaz Off IFX2 |
| F01-K02-21 | F2 | 093:SD-Chili 050:SD-Jazz Ring Off IFX1 |
| F01-K02-22 | F#2 | 099:SD-Ambi Hit 2 054:SD-Classic Room Off IFX1 |
| F01-K02-23 | G2 | 092:SD-Brasser 053:SD-Paper Off IFX1 |
| F01-K02-24 | G#2 | 085:SD-Vintage 4 056:SD-Brush Tap 1 Off IFX1 |
| F01-K02-25 | A2 | 078:SD-Hip 5 078:SD-Hip 5 Off IFX1 |
| F01-K02-26 | A#2 | 044:SD-GhostSet roll 044:SD-GhostSet roll Off IFX1 |
| F01-K02-27 | B2 | 005:BD-Dry 6 005:BD-Dry 6 Off IFX2 |
| F01-K02-28 | C3 | 004:BD-Dry 5 004:BD-Dry 5 Off IFX2 |
| F01-K02-29 | C#3 | 066:Sidestick-Amb 066:Sidestick-Amb Off L/R |
| F01-K02-30 | D3 | 045:SD-GhostSet f 047:SD-GhostSet p2 Off IFX1 |
| F01-K02-31 | D#3 | 229:Hand Claps 229:Hand Claps Off L/R |
| F01-K02-32 | E3 | 094:SD-Klanger 046:SD-GhostSet p1 Off IFX1 |
| F01-K02-33 | F3 | 103:Tom 1-Floor 103:Tom 1-Floor Off L/R |
| F01-K02-34 | F#3 | 126:H.Hat 2-Closed 2 125:H.Hat 2-Closed 1 1 L/R |
| F01-K02-35 | G3 | 103:Tom 1-Floor 103:Tom 1-Floor Off L/R |
| F01-K02-36 | G#3 | 128:H.Hat 2-FootOpen 127:H.Hat 2-Foot 1 L/R |
| F01-K02-37 | A3 | 102:Tom 1-Lo 102:Tom 1-Lo Off L/R |
| F01-K02-38 | A#3 | 131:H.Hat 2-Sizzle 129:H.Hat 2-Open 1 1 L/R |
| F01-K02-39 | B3 | 102:Tom 1-Lo 102:Tom 1-Lo Off L/R |
| F01-K02-40 | C4 | 101:Tom 1-Hi 101:Tom 1-Hi Off L/R |
| F01-K02-41 | C#4 | 153:Crash Cymbal 2 Off L/R |
| F01-K02-42 | D4 | 101:Tom 1-Hi 101:Tom 1-Hi Off L/R |
| F01-K02-43 | D#4 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K02-44 | E4 | 155:China Cymbal 155:China Cymbal Off L/R |
| F01-K02-45 | F4 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K02-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off L/R |
| F01-K02-47 | G4 | 156:Splash Cymbal 156:Splash Cymbal Off L/R |
| F01-K02-48 | G#4 | 305:Cowbell 1 Off L/R |
| F01-K02-49 | A4 | 152:Crash Cymbal 1 Off L/R |
| F01-K02-50 | A#4 | 277:Vibraslap Off L/R |
| F01-K02-51 | B4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K02-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K02-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K02-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K02-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K02-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K02-57 | F5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K02-58 | F#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K02-59 | G5 | 304:Agogo Bell Off L/R |
| F01-K02-60 | G#5 | 304:Agogo Bell Off L/R |
| F01-K02-61 | A5 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K02-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K02-63 | B5 | 316:Samba Whistle 2 L/R |
| F01-K02-64 | C6 | 316:Samba Whistle 2 L/R |
| F01-K02-65 | C#6 | 256:Guiro-Short 257:Guiro-Tap 3 L/R |
| F01-K02-66 | D6 | 255:Guiro-Long 3 L/R |
| F01-K02-67 | D#6 | 278:Claves Off L/R |
| F01-K02-68 | E6 | 279:Woodblock 1 Off L/R |
| F01-K02-69 | F6 | 279:Woodblock 1 Off L/R |
| F01-K02-70 | F#6 | 295:Cuica-Hi 4 L/R |
| F01-K02-71 | G6 | 296:Cuica-Lo 4 L/R |
| F01-K02-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 5 L/R |
| F01-K02-73 | A6 | 292:Triangle-Open 5 L/R |
| F01-K02-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K02-75 | B6 | 312:Marc Tree Off L/R |
| F01-K02-76 | C7 | 174:88-BD 2(R&B) 6 L/R |
| F01-K02-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 7 L/R |
| F01-K02-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K02-79 | D#7 | 188:88-HHat Open 7 L/R |
| F01-K02-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K02-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K02-82 | F#7 | 186:88-Claps Off L/R |
| F01-K02-83 | G7 | 196:88-Maraca Off L/R |
| F01-K02-84 | G#7 | 171:SD-Orch.Roll 2 8 L/R |
| F01-K02-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 8 L/R |
| F01-K02-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K02-87 | B7 | 354:Stadium Off L/R |
| F01-K02-88 | C8 | 355:Applause Off L/R |

### F01-K03 - Processed Kit (kit 002, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K03-01 | A0 |  |
| F01-K03-02 | A#0 |  |
| F01-K03-03 | B0 |  |
| F01-K03-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K03-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K03-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off L/R |
| F01-K03-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off L/R |
| F01-K03-08 | E1 | 261:Baya-Ghe 260:Baya-Open 7 L/R |
| F01-K03-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K03-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K03-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K03-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K03-13 | A1 | 227:Drumstick Hit 227:Drumstick Hit Off L/R |
| F01-K03-14 | A#1 | 157:Cymbal Reverse Off L/R |
| F01-K03-15 | B1 | 073:SD-Reverse Off IFX1 |
| F01-K03-16 | C2 | 016:BD-Tight 016:BD-Tight Off IFX2 |
| F01-K03-17 | C#2 | 017:BD-Squash 017:BD-Squash Off IFX2 |
| F01-K03-18 | D2 | 015:BD-Gated 015:BD-Gated Off IFX2 |
| F01-K03-19 | D#2 | 014:BD-Tubby 014:BD-Tubby Off IFX2 |
| F01-K03-20 | E2 | 011:BD-Woofer 011:BD-Woofer Off IFX2 |
| F01-K03-21 | F2 | 048:SD-Full Room 048:SD-Full Room Off IFX1 |
| F01-K03-22 | F#2 | 064:SD-Cracker Room 064:SD-Cracker Room Off IFX1 |
| F01-K03-23 | G2 | 052:SD-Amb.Piccolo 2 052:SD-Amb.Piccolo 2 Off IFX1 |
| F01-K03-24 | G#2 | 062:SD-Processed 1 062:SD-Processed 1 Off IFX1 |
| F01-K03-25 | A2 | 061:SD-Yowie 061:SD-Yowie Off IFX1 |
| F01-K03-26 | A#2 | 059:SD-Atomic 059:SD-Atomic Off IFX1 |
| F01-K03-27 | B2 | 013:BD-Terminator 013:BD-Terminator Off IFX2 |
| F01-K03-28 | C3 | 012:BD-Mondo Kill 012:BD-Mondo Kill Off IFX2 |
| F01-K03-29 | C#3 | 066:Sidestick-Amb 066:Sidestick-Amb Off L/R |
| F01-K03-30 | D3 | 060:SD-Big Rock 060:SD-Big Rock Off IFX1 |
| F01-K03-31 | D#3 | 229:Hand Claps 229:Hand Claps Off L/R |
| F01-K03-32 | E3 | 063:SD-Processed 2 063:SD-Processed 2 Off IFX1 |
| F01-K03-33 | F3 | 109:Tom 3-Floor 109:Tom 3-Floor Off L/R |
| F01-K03-34 | F#3 | 126:H.Hat 2-Closed 2 125:H.Hat 2-Closed 1 1 L/R |
| F01-K03-35 | G3 | 108:Tom 3-Lo 108:Tom 3-Lo Off L/R |
| F01-K03-36 | G#3 | 128:H.Hat 2-FootOpen 127:H.Hat 2-Foot 1 L/R |
| F01-K03-37 | A3 | 108:Tom 3-Lo 108:Tom 3-Lo Off L/R |
| F01-K03-38 | A#3 | 129:H.Hat 2-Open 1 130:H.Hat 2-Open 2 1 L/R |
| F01-K03-39 | B3 | 108:Tom 3-Lo 108:Tom 3-Lo Off L/R |
| F01-K03-40 | C4 | 107:Tom 3-Hi 107:Tom 3-Hi Off L/R |
| F01-K03-41 | C#4 | 153:Crash Cymbal 2 Off L/R |
| F01-K03-42 | D4 | 107:Tom 3-Hi 107:Tom 3-Hi Off L/R |
| F01-K03-43 | D#4 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K03-44 | E4 | 155:China Cymbal 155:China Cymbal Off L/R |
| F01-K03-45 | F4 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K03-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off L/R |
| F01-K03-47 | G4 | 156:Splash Cymbal 156:Splash Cymbal Off L/R |
| F01-K03-48 | G#4 | 305:Cowbell 1 Off L/R |
| F01-K03-49 | A4 | 152:Crash Cymbal 1 Off L/R |
| F01-K03-50 | A#4 | 277:Vibraslap Off L/R |
| F01-K03-51 | B4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K03-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K03-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K03-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K03-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K03-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K03-57 | F5 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-58 | F#5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K03-59 | G5 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-60 | G#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K03-61 | A5 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K03-63 | B5 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-64 | C6 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-65 | C#6 | 301:Cabasa-Tap 299:Cabasa-Up Off L/R |
| F01-K03-66 | D6 | 110:Tom-Processed 110:Tom-Processed Off L/R |
| F01-K03-67 | D#6 | 278:Claves 278:Claves Off L/R |
| F01-K03-68 | E6 | 279:Woodblock 1 279:Woodblock 1 Off L/R |
| F01-K03-69 | F6 | 279:Woodblock 1 279:Woodblock 1 Off L/R |
| F01-K03-70 | F#6 | 295:Cuica-Hi 2 L/R |
| F01-K03-71 | G6 | 296:Cuica-Lo 296:Cuica-Lo 2 L/R |
| F01-K03-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 3 L/R |
| F01-K03-73 | A6 | 292:Triangle-Open 292:Triangle-Open 3 L/R |
| F01-K03-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K03-75 | B6 | 312:Marc Tree Off L/R |
| F01-K03-76 | C7 | 174:88-BD 2(R&B) 4 L/R |
| F01-K03-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 5 L/R |
| F01-K03-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K03-79 | D#7 | 188:88-HHat Open 5 L/R |
| F01-K03-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K03-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K03-82 | F#7 | 186:88-Claps Off L/R |
| F01-K03-83 | G7 | 196:88-Maraca Off L/R |
| F01-K03-84 | G#7 | 171:SD-Orch.Roll 2 170:SD-Orch.Roll 1 6 L/R |
| F01-K03-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 6 L/R |
| F01-K03-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K03-87 | B7 | 354:Stadium Off L/R |
| F01-K03-88 | C8 | 355:Applause Off L/R |

### F01-K04 - Jazz/Brush Kit (kit 003, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K04-01 | A0 |  |
| F01-K04-02 | A#0 |  |
| F01-K04-03 | B0 |  |
| F01-K04-04 | C1 | 103:Tom 1-Floor 103:Tom 1-Floor Off L/R |
| F01-K04-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K04-06 | D1 | 102:Tom 1-Lo 102:Tom 1-Lo Off L/R |
| F01-K04-07 | D#1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K04-08 | E1 | 101:Tom 1-Hi 101:Tom 1-Hi Off L/R |
| F01-K04-09 | F1 | 101:Tom 1-Hi 101:Tom 1-Hi Off L/R |
| F01-K04-10 | F#1 | 307:Chacha Bell Off L/R |
| F01-K04-11 | G1 | 228:Finger Snap 228:Finger Snap Off L/R |
| F01-K04-12 | G#1 | 229:Hand Claps 229:Hand Claps Off L/R |
| F01-K04-13 | A1 | 227:Drumstick Hit Off L/R |
| F01-K04-14 | A#1 | 014:BD-Tubby 014:BD-Tubby Off IFX2 |
| F01-K04-15 | B1 | 002:BD-Dry 3 002:BD-Dry 3 Off IFX2 |
| F01-K04-16 | C2 | 011:BD-Woofer 011:BD-Woofer Off IFX2 |
| F01-K04-17 | C#2 | 003:BD-Dry 4 003:BD-Dry 4 Off IFX2 |
| F01-K04-18 | D2 | 006:BD-Soft 006:BD-Soft Off IFX2 |
| F01-K04-19 | D#2 | 008:BD-Jazz 1 008:BD-Jazz 1 Off IFX2 |
| F01-K04-20 | E2 | 009:BD-Jazz 2 009:BD-Jazz 2 Off IFX2 |
| F01-K04-21 | F2 | 053:SD-Paper 053:SD-Paper Off IFX1 |
| F01-K04-22 | F#2 | 041:SD-Dry 2 041:SD-Dry 2 Off IFX1 |
| F01-K04-23 | G2 | 040:SD-Dry 1 040:SD-Dry 1 Off IFX1 |
| F01-K04-24 | G#2 | 051:SD-Amb.Piccolo 1 051:SD-Amb.Piccolo 1 Off IFX1 |
| F01-K04-25 | A2 | 050:SD-Jazz Ring 050:SD-Jazz Ring Off IFX1 |
| F01-K04-26 | A#2 | 055:SD-Brush Hit Off IFX1 |
| F01-K04-27 | B2 | 009:BD-Jazz 2 009:BD-Jazz 2 Off IFX2 |
| F01-K04-28 | C3 | 010:BD-Pillow 010:BD-Pillow Off IFX2 |
| F01-K04-29 | C#3 | 065:Sidestick-Dry 065:Sidestick-Dry Off L/R |
| F01-K04-30 | D3 | 057:SD-Brush Tap 2 Off IFX1 |
| F01-K04-31 | D#3 | 056:SD-Brush Tap 1 Off IFX1 |
| F01-K04-32 | E3 | 058:SD-Brush Swirl Off IFX1 |
| F01-K04-33 | F3 | 115:Tom-Brush Floor 115:Tom-Brush Floor Off L/R |
| F01-K04-34 | F#3 | 119:H.Hat 1-Closed 2 118:H.Hat 1-Closed 1 1 L/R |
| F01-K04-35 | G3 | 114:Tom-Brush Hi 114:Tom-Brush Hi Off L/R |
| F01-K04-36 | G#3 | 120:H.Hat 1-Foot 120:H.Hat 1-Foot 1 L/R |
| F01-K04-37 | A3 | 114:Tom-Brush Hi 114:Tom-Brush Hi Off L/R |
| F01-K04-38 | A#3 | 122:H.Hat 1-Open 1 123:H.Hat 1-Open 2 1 L/R |
| F01-K04-39 | B3 | 113:Tom-Jazz Floor 113:Tom-Jazz Floor Off L/R |
| F01-K04-40 | C4 | 112:Tom-Jazz Hi Rim 111:Tom-Jazz Hi Off L/R |
| F01-K04-41 | C#4 | 152:Crash Cymbal 1 Off L/R |
| F01-K04-42 | D4 | 112:Tom-Jazz Hi Rim 111:Tom-Jazz Hi Off L/R |
| F01-K04-43 | D#4 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K04-44 | E4 | 155:China Cymbal 155:China Cymbal Off L/R |
| F01-K04-45 | F4 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K04-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off L/R |
| F01-K04-47 | G4 | 156:Splash Cymbal 156:Splash Cymbal Off L/R |
| F01-K04-48 | G#4 | 305:Cowbell 1 305:Cowbell 1 Off L/R |
| F01-K04-49 | A4 | 154:Crash Cymbal 3 154:Crash Cymbal 3 Off L/R |
| F01-K04-50 | A#4 | 277:Vibraslap Off L/R |
| F01-K04-51 | B4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K04-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K04-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K04-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K04-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K04-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K04-57 | F5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K04-58 | F#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K04-59 | G5 | 304:Agogo Bell Off L/R |
| F01-K04-60 | G#5 | 304:Agogo Bell Off L/R |
| F01-K04-61 | A5 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K04-62 | A#5 | 258:Maracas-Push Off L/R |
| F01-K04-63 | B5 | 316:Samba Whistle 2 L/R |
| F01-K04-64 | C6 | 316:Samba Whistle 2 L/R |
| F01-K04-65 | C#6 | 256:Guiro-Short 257:Guiro-Tap 3 L/R |
| F01-K04-66 | D6 | 255:Guiro-Long 3 L/R |
| F01-K04-67 | D#6 | 278:Claves Off L/R |
| F01-K04-68 | E6 | 279:Woodblock 1 Off L/R |
| F01-K04-69 | F6 | 279:Woodblock 1 Off L/R |
| F01-K04-70 | F#6 | 295:Cuica-Hi 4 L/R |
| F01-K04-71 | G6 | 296:Cuica-Lo 4 L/R |
| F01-K04-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 5 L/R |
| F01-K04-73 | A6 | 292:Triangle-Open 5 L/R |
| F01-K04-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K04-75 | B6 | 312:Marc Tree Off L/R |
| F01-K04-76 | C7 | 174:88-BD 2(R&B) 6 L/R |
| F01-K04-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 7 L/R |
| F01-K04-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K04-79 | D#7 | 188:88-HHat Open 7 L/R |
| F01-K04-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K04-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K04-82 | F#7 | 186:88-Claps Off L/R |
| F01-K04-83 | G7 | 196:88-Maraca Off L/R |
| F01-K04-84 | G#7 | 171:SD-Orch.Roll 2 170:SD-Orch.Roll 1 8 L/R |
| F01-K04-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 8 L/R |
| F01-K04-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K04-87 | B7 | 354:Stadium Off L/R |
| F01-K04-88 | C8 | 355:Applause Off L/R |

### F01-K05 - House Kit (kit 004, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K05-01 | A0 |  |
| F01-K05-02 | A#0 |  |
| F01-K05-03 | B0 |  |
| F01-K05-04 | C1 | 315:Flexatone Off L/R |
| F01-K05-05 | C#1 | 257:Guiro-Tap 258:Maracas-Push 6 L/R |
| F01-K05-06 | D1 | 391:Swish Terra 391:Swish Terra Off L/R |
| F01-K05-07 | D#1 | 319:Cyber Drum 319:Cyber Drum Off L/R |
| F01-K05-08 | E1 | 311:Finger Cymbal 311:Finger Cymbal Off L/R |
| F01-K05-09 | F1 | 291:Tambourine-Acc.2 Off L/R |
| F01-K05-10 | F#1 | 288:Tambourine-Push 289:Tambourine-Pull Off L/R |
| F01-K05-11 | G1 | 310:Rap Sleighbell Off L/R |
| F01-K05-12 | G#1 | 309:Sleigh Bell 309:Sleigh Bell Off L/R |
| F01-K05-13 | A1 | 312:Marc Tree 312:Marc Tree Off L/R |
| F01-K05-14 | A#1 | 177:99-BD 2 177:99-BD 2 Off IFX2 |
| F01-K05-15 | B1 | 18: BD-Dance 1 18: BD-Dance 1 Off IFX2 |
| F01-K05-16 | C2 | 178:99-BD 3 178:99-BD 3 Off IFX2 |
| F01-K05-17 | C#2 | 21: BD-Dance 4 21: BD-Dance 4 Off IFX2 |
| F01-K05-18 | D2 | 29: BD-Short88 29: BD-Short88 Off IFX2 |
| F01-K05-19 | D#2 | 17: BD-Squash 17: BD-Squash Off IFX2 |
| F01-K05-20 | E2 | 174:88-BD 2(R&B) 174:88-BD 2(R&B) Off IFX2 |
| F01-K05-21 | F2 | 181:88-SD 1(New) 181:88-SD 1(New) Off IFX1 |
| F01-K05-22 | F#2 | 183:99-SD 1 183:99-SD 1 Off IFX1 |
| F01-K05-23 | G2 | 77: SD-Hip 4 77: SD-Hip 4 Off IFX1 |
| F01-K05-24 | G#2 | 72: SD-Sizzle 72: SD-Sizzle Off IFX1 |
| F01-K05-25 | A2 | 71: SD-Noise Off IFX1 |
| F01-K05-26 | A#2 | 74: SD-Hip 1 Off IFX1 |
| F01-K05-27 | B2 | 179:99-BD 4 179:99-BD 4 Off IFX2 |
| F01-K05-28 | C3 | 176:99-BD 1 176:99-BD 1 Off IFX2 |
| F01-K05-29 | C#3 | 185:88-Rimshot 185:88-Rimshot Off L/R |
| F01-K05-30 | D3 | 182:88-SD 2(Old) 182:88-SD 2(Old) Off IFX1 |
| F01-K05-31 | D#3 | 186:88-Claps 186:88-Claps Off L/R |
| F01-K05-32 | E3 | 184:99-SD 2 184:99-SD 2 Off IFX1 |
| F01-K05-33 | F3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K05-34 | F#3 | 189:99-HHat Closed 189:99-HHat Closed 1 L/R |
| F01-K05-35 | G3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K05-36 | G#3 | 190:99-HHat Open 190:99-HHat Open 1 L/R |
| F01-K05-37 | A3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K05-38 | A#3 | 190:99-HHat Open 190:99-HHat Open 1 L/R |
| F01-K05-39 | B3 | 150:H.Hat-Whispy Off L/R |
| F01-K05-40 | C4 | 187:88-HHat Closed 187:88-HHat Closed 2 L/R |
| F01-K05-41 | C#4 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K05-42 | D4 | 188:88-HHat Open 188:88-HHat Open 2 L/R |
| F01-K05-43 | D#4 | 160:Ride-Dance 99 160:Ride-Dance 99 Off L/R |
| F01-K05-44 | E4 | 153:Crash Cymbal 2 Off L/R |
| F01-K05-45 | F4 | 161:Ride-Dance DDD1 161:Ride-Dance DDD1 Off L/R |
| F01-K05-46 | F#4 | 291:Tambourine-Acc.2 290:Tambourine-Acc.1 Off L/R |
| F01-K05-47 | G4 | 156:Splash Cymbal 156:Splash Cymbal Off L/R |
| F01-K05-48 | G#4 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K05-49 | A4 | 158:Crash-Dance 99 158:Crash-Dance 99 Off L/R |
| F01-K05-50 | A#4 | 207:Zap 3 207:Zap 3 Off L/R |
| F01-K05-51 | B4 | 157:Cymbal Reverse 157:Cymbal Reverse Off L/R |
| F01-K05-52 | C5 | 205:Zap 1 205:Zap 1 Off L/R |
| F01-K05-53 | C#5 | 206:Zap 2 206:Zap 2 Off L/R |
| F01-K05-54 | D5 | 194:88-Clave Off L/R |
| F01-K05-55 | D#5 | 193:88-Conga 193:88-Conga Off L/R |
| F01-K05-56 | E5 | 193:88-Conga 193:88-Conga Off L/R |
| F01-K05-57 | F5 | 324:Metal Hit Off L/R |
| F01-K05-58 | F#5 | 132:H.Hat-Chili Off L/R |
| F01-K05-59 | G5 | 323:Industry Loop 2 Off L/R |
| F01-K05-60 | G#5 | 127:H.Hat 2-Foot Off L/R |
| F01-K05-61 | A5 | 204:Dance Perc-Hous2 Off L/R |
| F01-K05-62 | A#5 | 196:88-Maraca Off L/R |
| F01-K05-63 | B5 | 199:Vocal SD 199:Vocal SD Off L/R |
| F01-K05-64 | C6 | 377:Car Crash Off L/R |
| F01-K05-65 | C#6 | 293:Triangle-Mute 3 L/R |
| F01-K05-66 | D6 | 395:Noise-White Off L/R |
| F01-K05-67 | D#6 | 292:Triangle-Open 3 L/R |
| F01-K05-68 | E6 | 400:GtCutNois1 Off L/R |
| F01-K05-69 | F6 | 146:H.Hat-Hip 2 Off L/R |
| F01-K05-70 | F#6 | 151:H.Hat-Alpo Close 151:H.Hat-Alpo Close 4 L/R |
| F01-K05-71 | G6 | 402:StringSlap Off L/R |
| F01-K05-72 | G#6 | 147:H.Hat-AmbCrackle Off L/R |
| F01-K05-73 | A6 | 406:Dist.Slide 1 Off L/R |
| F01-K05-74 | A#6 | 135:H.Hat-Crisp Open 4 L/R |
| F01-K05-75 | B6 | 391:Swish Terra 391:Swish Terra Off L/R |
| F01-K05-76 | C7 | 391:Swish Terra Off L/R |
| F01-K05-77 | C#7 | 152:Crash Cymbal 1 152:Crash Cymbal 1 Off L/R |
| F01-K05-78 | D7 | 155:China Cymbal 155:China Cymbal Off L/R |
| F01-K05-79 | D#7 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K05-80 | E7 | 238:Conga Hi-Slap 1 242:Bongo Lo-Open Off L/R |
| F01-K05-81 | F7 | 235:Conga Hi-Open 233:Conga Lo-MtSlap Off L/R |
| F01-K05-82 | F#7 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K05-83 | G7 | 239:Conga Hi-Slap 2 236:Conga Hi-Mute Off L/R |
| F01-K05-84 | G#7 | 305:Cowbell 1 305:Cowbell 1 Off L/R |
| F01-K05-85 | A7 | 307:Chacha Bell 307:Chacha Bell Off L/R |
| F01-K05-86 | A#7 | 305:Cowbell 1 305:Cowbell 1 Off L/R |
| F01-K05-87 | B7 | 256:Guiro-Short 6 L/R |
| F01-K05-88 | C8 | 257:Guiro-Tap 257:Guiro-Tap 6 L/R |

### F01-K06 - Tricky Kit (kit 005, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K06-01 | A0 |  |
| F01-K06-02 | A#0 |  |
| F01-K06-03 | B0 |  |
| F01-K06-04 | C1 | 325:Yea Solo 326:Year solo Off L/R |
| F01-K06-05 | C#1 | 329:Uhhhh Solo 328:Hit It Off L/R |
| F01-K06-06 | D1 | 327:Uhh Solo Off L/R |
| F01-K06-07 | D#1 | 335:CompuVoice-When 334:CompuVoice-Do it Off L/R |
| F01-K06-08 | E1 | 331:CompuVoice-Noise 330:CompuVoice-Beep 9 L/R |
| F01-K06-09 | F1 | 332:CompuVoice-Chi 333:CompuVoice-Test Off L/R |
| F01-K06-10 | F#1 | 337:CompuVoice-Ti 336:CompuVoice-Toi Off L/R |
| F01-K06-11 | G1 | 343:PC Vox-Dunc 342:PC Vox-Sis Off L/R |
| F01-K06-12 | G#1 | 348:PC Vox-Zee 345:PC Vox-For Off L/R |
| F01-K06-13 | A1 | 349:PC Vox-Ae 350:PC Vox-Pa Off L/R |
| F01-K06-14 | A#1 | 350:PC Vox-Pa 351:PC Vox-Sh Off L/R |
| F01-K06-15 | B1 | 352:PC Vox-O 353:PC Vox-Tu Off L/R |
| F01-K06-16 | C2 | 174:88-BD 2(R&B) 174:88-BD 2(R&B) Off IFX2 |
| F01-K06-17 | C#2 | 026:BD-Dark 180:99-BD 5 Off IFX2 |
| F01-K06-18 | D2 | 022:BD-Hip 1 023:BD-Hip 2 Off IFX2 |
| F01-K06-19 | D#2 | 180:99-BD 5 180:99-BD 5 Off IFX2 |
| F01-K06-20 | E2 | 034:BD-Dance Soft 029:BD-Short88 Off IFX2 |
| F01-K06-21 | F2 | 091:SD-AmbiHop 091:SD-AmbiHop Off IFX1 |
| F01-K06-22 | F#2 | 098:SD-Ambi Hit 1 Off IFX1 |
| F01-K06-23 | G2 | 092:SD-Brasser Off IFX1 |
| F01-K06-24 | G#2 | 097:SD-Ambi Pop 088:SD-AmbiCrackle 1 Off IFX1 |
| F01-K06-25 | A2 | 096:SD-Dry Present Off IFX1 |
| F01-K06-26 | A#2 | 087:SD-Vintage 6 087:SD-Vintage 6 Off IFX1 |
| F01-K06-27 | B2 | 005:BD-Dry 6 026:BD-Dark Off IFX2 |
| F01-K06-28 | C3 | 027:BD-Pop 029:BD-Short88 Off IFX2 |
| F01-K06-29 | C#3 | 066:Sidestick-Amb Off IFX1 |
| F01-K06-30 | D3 | 082:SD-Vintage 1 082:SD-Vintage 1 Off IFX1 |
| F01-K06-31 | D#3 | 230:Hand Claps-Hip 231:Hand Claps-Happy Off L/R |
| F01-K06-32 | E3 | 079:SD-Hip 6 Off IFX1 |
| F01-K06-33 | F3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K06-34 | F#3 | 139:H.Hat-Tight 1 140:H.Hat-Tight 2 1 L/R |
| F01-K06-35 | G3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K06-36 | G#3 | 142:H.Hat-Old Cl 2 145:H.Hat-Hip 1 1 L/R |
| F01-K06-37 | A3 | 192:88-Tom 192:88-Tom Off L/R |
| F01-K06-38 | A#3 | 149:H.Hat-ClangyOpen 135:H.Hat-Crisp Open 1 L/R |
| F01-K06-39 | B3 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K06-40 | C4 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K06-41 | C#4 | 396:Noise-FM Mod Off L/R |
| F01-K06-42 | D4 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K06-43 | D#4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K06-44 | E4 | 191:88-Crash Off L/R |
| F01-K06-45 | F4 | 165:Ride-Cup Off L/R |
| F01-K06-46 | F#4 | 290:Tambourine-Acc.1 Off L/R |
| F01-K06-47 | G4 | 159:Crash-Dance DDD1 Off L/R |
| F01-K06-48 | G#4 | 305:Cowbell 1 Off L/R |
| F01-K06-49 | A4 | 383:Explosion Off L/R |
| F01-K06-50 | A#4 | 388:ChurchBell Off L/R |
| F01-K06-51 | B4 | 366:Crickets 2 366:Crickets 2 Off L/R |
| F01-K06-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K06-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K06-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K06-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K06-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K06-57 | F5 | 310:Rap Sleighbell 310:Rap Sleighbell Off L/R |
| F01-K06-58 | F#5 | 217:DJ-BD Rub Off L/R |
| F01-K06-59 | G5 | 218:DJ-SD Rub Off L/R |
| F01-K06-60 | G#5 | 219:DJ-Record Stop 220:DJ-Reverse Off L/R |
| F01-K06-61 | A5 | 299:Cabasa-Up Off L/R |
| F01-K06-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K06-63 | B5 | 196:88-Maraca Off L/R |
| F01-K06-64 | C6 | 320:SD-Metal Striker Off L/R |
| F01-K06-65 | C#6 | 374:Car Engine Off L/R |
| F01-K06-66 | D6 | 323:Industry Loop 2 Off L/R |
| F01-K06-67 | D#6 | 323:Industry Loop 2 Off L/R |
| F01-K06-68 | E6 | 375:Car Stop Off L/R |
| F01-K06-69 | F6 | 291:Tambourine-Acc.2 Off L/R |
| F01-K06-70 | F#6 | 323:Industry Loop 2 Off L/R |
| F01-K06-71 | G6 | 402:StringSlap 4 L/R |
| F01-K06-72 | G#6 | 202:Dance Perc-Hip 5 L/R |
| F01-K06-73 | A6 | 205:Zap 1 Off L/R |
| F01-K06-74 | A#6 | 387:Bubble Off L/R |
| F01-K06-75 | B6 | 377:Car Crash Off L/R |
| F01-K06-76 | C7 | 173:88-BD 1(Rap) Off L/R |
| F01-K06-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed Off L/R |
| F01-K06-78 | D7 | 182:88-SD 2(Old) 182:88-SD 2(Old) Off L/R |
| F01-K06-79 | D#7 | 188:88-HHat Open Off L/R |
| F01-K06-80 | E7 | 317:Chinese Gong Off L/R |
| F01-K06-81 | F7 | 194:88-Clave 194:88-Clave Off L/R |
| F01-K06-82 | F#7 | 385:Wind Off L/R |
| F01-K06-83 | G7 | 386:Stream Off L/R |
| F01-K06-84 | G#7 | 340:Two... Solo 341:One... Solo Off L/R |
| F01-K06-85 | A7 | 338:Four... Solo 339:Three... Solo Off L/R |
| F01-K06-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K06-87 | B7 | 390:Space Lore Off L/R |
| F01-K06-88 | C8 | 367:Hart Beat Off L/R |

### F01-K07 - Drum'n'Bass Kit (kit 006, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K07-01 | A0 |  |
| F01-K07-02 | A#0 |  |
| F01-K07-03 | B0 |  |
| F01-K07-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K07-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K07-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off L/R |
| F01-K07-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off L/R |
| F01-K07-08 | E1 | 261:Baya-Ghe 260:Baya-Open 10 L/R |
| F01-K07-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K07-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K07-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K07-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K07-13 | A1 | 227:Drumstick Hit Off L/R |
| F01-K07-14 | A#1 | 157:Cymbal Reverse Off L/R |
| F01-K07-15 | B1 | 073:SD-Reverse Off IFX1 |
| F01-K07-16 | C2 | 037:BD-PhatJaz 037:BD-PhatJaz Off IFX2 |
| F01-K07-17 | C#2 | 014:BD-Tubby Off IFX2 |
| F01-K07-18 | D2 | 005:BD-Dry 6 005:BD-Dry 6 Off IFX2 |
| F01-K07-19 | D#2 | 039:BD-Klanger 022:BD-Hip 1 Off IFX2 |
| F01-K07-20 | E2 | 010:BD-Pillow 010:BD-Pillow Off IFX2 |
| F01-K07-21 | F2 | 100:SD-Ambi Hit 3 100:SD-Ambi Hit 3 Off IFX1 |
| F01-K07-22 | F#2 | 076:SD-Hip 3 076:SD-Hip 3 Off IFX1 |
| F01-K07-23 | G2 | 094:SD-Klanger 051:SD-Amb.Piccolo 1 Off IFX1 |
| F01-K07-24 | G#2 | 080:SD-Ringy 080:SD-Ringy Off IFX1 |
| F01-K07-25 | A2 | 093:SD-Chili 093:SD-Chili Off IFX1 |
| F01-K07-26 | A#2 | 040:SD-Dry 1 040:SD-Dry 1 Off IFX1 |
| F01-K07-27 | B2 | 028:BD-Ringy 029:BD-Short88 Off IFX2 |
| F01-K07-28 | C3 | 032:BD-Ambi Crackle 031:BD-Ambi Off IFX2 |
| F01-K07-29 | C#3 | 065:Sidestick-Dry 185:88-Rimshot Off L/R |
| F01-K07-30 | D3 | 093:SD-Chili 093:SD-Chili Off IFX1 |
| F01-K07-31 | D#3 | 231:Hand Claps-Happy 230:Hand Claps-Hip Off L/R |
| F01-K07-32 | E3 | 090:SD-AmbiCrackle 3 090:SD-AmbiCrackle 3 Off IFX1 |
| F01-K07-33 | F3 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K07-34 | F#3 | 134:H.Hat-Crisp Cl 2 134:H.Hat-Crisp Cl 2 1 L/R |
| F01-K07-35 | G3 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K07-36 | G#3 | 119:H.Hat 1-Closed 2 119:H.Hat 1-Closed 2 1 L/R |
| F01-K07-37 | A3 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K07-38 | A#3 | 135:H.Hat-Crisp Open 123:H.Hat 1-Open 2 1 L/R |
| F01-K07-39 | B3 | 137:H.Hat-Vintage 1 137:H.Hat-Vintage 1 Off L/R |
| F01-K07-40 | C4 | 147:H.Hat-AmbCrackle 147:H.Hat-AmbCrackle Off L/R |
| F01-K07-41 | C#4 | 153:Crash Cymbal 2 Off L/R |
| F01-K07-42 | D4 | 144:H.Hat-Old Open 144:H.Hat-Old Open Off L/R |
| F01-K07-43 | D#4 | 162:Ride-Edge 1 162:Ride-Edge 1 Off L/R |
| F01-K07-44 | E4 | 396:Noise-FM Mod 393:Cricket Spectrum Off L/R |
| F01-K07-45 | F4 | 165:Ride-Cup 165:Ride-Cup Off L/R |
| F01-K07-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off L/R |
| F01-K07-47 | G4 | 291:Tambourine-Acc.2 289:Tambourine-Pull Off L/R |
| F01-K07-48 | G#4 | 146:H.Hat-Hip 2 Off L/R |
| F01-K07-49 | A4 | 152:Crash Cymbal 1 Off L/R |
| F01-K07-50 | A#4 | 381:MachineGun 381:MachineGun Off L/R |
| F01-K07-51 | B4 | 161:Ride-Dance DDD1 161:Ride-Dance DDD1 Off L/R |
| F01-K07-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K07-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K07-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K07-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K07-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K07-57 | F5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K07-58 | F#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K07-59 | G5 | 202:Dance Perc-Hip Off L/R |
| F01-K07-60 | G#5 | 202:Dance Perc-Hip Off L/R |
| F01-K07-61 | A5 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K07-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K07-63 | B5 | 045:SD-GhostSet f 045:SD-GhostSet f 9 IFX1 |
| F01-K07-64 | C6 | 044:SD-GhostSet roll 044:SD-GhostSet roll 9 IFX1 |
| F01-K07-65 | C#6 | 378:Train 2 L/R |
| F01-K07-66 | D6 | 390:Space Lore 390:Space Lore 3 L/R |
| F01-K07-67 | D#6 | 396:Noise-FM Mod Off L/R |
| F01-K07-68 | E6 | 394:Air Vortex Off L/R |
| F01-K07-69 | F6 | 393:Cricket Spectrum 393:Cricket Spectrum Off L/R |
| F01-K07-70 | F#6 | 374:Car Engine 4 L/R |
| F01-K07-71 | G6 | 373:Door Slam 4 L/R |
| F01-K07-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 5 L/R |
| F01-K07-73 | A6 | 292:Triangle-Open 5 L/R |
| F01-K07-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K07-75 | B6 | 314:Bell Tree Off L/R |
| F01-K07-76 | C7 | 174:88-BD 2(R&B) 6 L/R |
| F01-K07-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 7 L/R |
| F01-K07-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K07-79 | D#7 | 188:88-HHat Open 7 L/R |
| F01-K07-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K07-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K07-82 | F#7 | 186:88-Claps Off L/R |
| F01-K07-83 | G7 | 196:88-Maraca Off L/R |
| F01-K07-84 | G#7 | 171:SD-Orch.Roll 2 8 L/R |
| F01-K07-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 8 L/R |
| F01-K07-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K07-87 | B7 | 354:Stadium Off L/R |
| F01-K07-88 | C8 | 355:Applause Off L/R |

### F01-K08 - Hip Hop Kit (kit 007, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K08-01 | A0 |  |
| F01-K08-02 | A#0 |  |
| F01-K08-03 | B0 |  |
| F01-K08-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K08-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K08-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off L/R |
| F01-K08-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off L/R |
| F01-K08-08 | E1 | 261:Baya-Ghe 260:Baya-Open 9 L/R |
| F01-K08-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K08-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K08-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K08-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K08-13 | A1 | 227:Drumstick Hit Off L/R |
| F01-K08-14 | A#1 | 157:Cymbal Reverse Off L/R |
| F01-K08-15 | B1 | 073:SD-Reverse Off IFX1 |
| F01-K08-16 | C2 | 037:BD-PhatJaz Off IFX2 |
| F01-K08-17 | C#2 | 022:BD-Hip 1 Off IFX2 |
| F01-K08-18 | D2 | 021:BD-Dance 4 021:BD-Dance 4 Off IFX2 |
| F01-K08-19 | D#2 | 035:BD-AmbiRocker 035:BD-AmbiRocker Off IFX2 |
| F01-K08-20 | E2 | 180:99-BD 5 Off IFX2 |
| F01-K08-21 | F2 | 086:SD-Vintage 5 Off IFX1 |
| F01-K08-22 | F#2 | 098:SD-Ambi Hit 1 Off IFX1 |
| F01-K08-23 | G2 | 089:SD-AmbiCrackle 2 Off IFX1 |
| F01-K08-24 | G#2 | 083:SD-Vintage 2 Off IFX1 |
| F01-K08-25 | A2 | 076:SD-Hip 3 Off IFX1 |
| F01-K08-26 | A#2 | 067:SD-Amb.House Off IFX1 |
| F01-K08-27 | B2 | 175:88-BD 3 Off IFX2 |
| F01-K08-28 | C3 | 031:BD-Ambi Off IFX2 |
| F01-K08-29 | C#3 | 065:Sidestick-Dry Off L/R |
| F01-K08-30 | D3 | 095:SD-Whopper Off IFX1 |
| F01-K08-31 | D#3 | 231:Hand Claps-Happy Off L/R |
| F01-K08-32 | E3 | 082:SD-Vintage 1 Off IFX1 |
| F01-K08-33 | F3 | 116:Tom-Dirty Funk 116:Tom-Dirty Funk Off L/R |
| F01-K08-34 | F#3 | 147:H.Hat-AmbCrackle 1 L/R |
| F01-K08-35 | G3 | 116:Tom-Dirty Funk 116:Tom-Dirty Funk Off L/R |
| F01-K08-36 | G#3 | 142:H.Hat-Old Cl 2 1 L/R |
| F01-K08-37 | A3 | 116:Tom-Dirty Funk 116:Tom-Dirty Funk Off L/R |
| F01-K08-38 | A#3 | 135:H.Hat-Crisp Open 1 L/R |
| F01-K08-39 | B3 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K08-40 | C4 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K08-41 | C#4 | 153:Crash Cymbal 2 Off L/R |
| F01-K08-42 | D4 | 117:Tom-Old R&B 117:Tom-Old R&B Off L/R |
| F01-K08-43 | D#4 | 162:Ride-Edge 1 Off L/R |
| F01-K08-44 | E4 | 155:China Cymbal Off L/R |
| F01-K08-45 | F4 | 165:Ride-Cup Off L/R |
| F01-K08-46 | F#4 | 290:Tambourine-Acc.1 Off L/R |
| F01-K08-47 | G4 | 156:Splash Cymbal Off L/R |
| F01-K08-48 | G#4 | 305:Cowbell 1 Off L/R |
| F01-K08-49 | A4 | 152:Crash Cymbal 1 Off L/R |
| F01-K08-50 | A#4 | 277:Vibraslap Off L/R |
| F01-K08-51 | B4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off L/R |
| F01-K08-52 | C5 | 245:Bongo Hi-Open 245:Bongo Hi-Open Off L/R |
| F01-K08-53 | C#5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off L/R |
| F01-K08-54 | D5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off L/R |
| F01-K08-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K08-56 | E5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K08-57 | F5 | 285:Timbale Hi-Rim 1 284:Timbale Hi-Edge Off L/R |
| F01-K08-58 | F#5 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off L/R |
| F01-K08-59 | G5 | 304:Agogo Bell Off L/R |
| F01-K08-60 | G#5 | 304:Agogo Bell Off L/R |
| F01-K08-61 | A5 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K08-62 | A#5 | 258:Maracas-Push 258:Maracas-Push Off L/R |
| F01-K08-63 | B5 | 316:Samba Whistle 2 L/R |
| F01-K08-64 | C6 | 316:Samba Whistle 2 L/R |
| F01-K08-65 | C#6 | 256:Guiro-Short 257:Guiro-Tap 3 L/R |
| F01-K08-66 | D6 | 255:Guiro-Long 3 L/R |
| F01-K08-67 | D#6 | 278:Claves Off L/R |
| F01-K08-68 | E6 | 279:Woodblock 1 Off L/R |
| F01-K08-69 | F6 | 279:Woodblock 1 Off L/R |
| F01-K08-70 | F#6 | 295:Cuica-Hi 4 L/R |
| F01-K08-71 | G6 | 296:Cuica-Lo 4 L/R |
| F01-K08-72 | G#6 | 293:Triangle-Mute 293:Triangle-Mute 5 L/R |
| F01-K08-73 | A6 | 292:Triangle-Open 5 L/R |
| F01-K08-74 | A#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K08-75 | B6 | 312:Marc Tree Off L/R |
| F01-K08-76 | C7 | 174:88-BD 2(R&B) 6 L/R |
| F01-K08-77 | C#7 | 187:88-HHat Closed 187:88-HHat Closed 7 L/R |
| F01-K08-78 | D7 | 181:88-SD 1(New) 181:88-SD 1(New) Off L/R |
| F01-K08-79 | D#7 | 188:88-HHat Open 7 L/R |
| F01-K08-80 | E7 | 191:88-Crash 191:88-Crash Off L/R |
| F01-K08-81 | F7 | 195:88-Cowbell 195:88-Cowbell Off L/R |
| F01-K08-82 | F#7 | 186:88-Claps Off L/R |
| F01-K08-83 | G7 | 196:88-Maraca Off L/R |
| F01-K08-84 | G#7 | 171:SD-Orch.Roll 2 8 L/R |
| F01-K08-85 | A7 | 172:SD-Orchestral 172:SD-Orchestral 8 L/R |
| F01-K08-86 | A#7 | 314:Bell Tree Off L/R |
| F01-K08-87 | B7 | 354:Stadium Off L/R |
| F01-K08-88 | C8 | 355:Applause Off L/R |

### F01-K09 - Psycho Kit (kit 008, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K09-01 | A0 |  |
| F01-K09-02 | A#0 |  |
| F01-K09-03 | B0 |  |
| F01-K09-04 | C1 | 317:Chinese Gong 317:Chinese Gong Off L/R |
| F01-K09-05 | C#1 | 294:Triangle-Roll 294:Triangle-Roll Off IFX1 |
| F01-K09-06 | D1 | 315:Flexatone 315:Flexatone Off IFX1 |
| F01-K09-07 | D#1 | 274:Taiko-Open 274:Taiko-Open Off IFX2 |
| F01-K09-08 | E1 | 263:Baya-Mute 2 263:Baya-Mute 2 Off IFX2 |
| F01-K09-09 | F1 | 072:SD-Sizzle 072:SD-Sizzle Off IFX1 |
| F01-K09-10 | F#1 | 058:SD-Brush Swirl 058:SD-Brush Swirl Off IFX1 |
| F01-K09-11 | G1 | 070:SD-Rap 2 070:SD-Rap 2 Off IFX1 |
| F01-K09-12 | G#1 | 073:SD-Reverse 073:SD-Reverse 4 IFX1 |
| F01-K09-13 | A1 | 015:BD-Gated 015:BD-Gated Off IFX2 |
| F01-K09-14 | A#1 | 157:Cymbal Reverse 157:Cymbal Reverse 5 IFX1 |
| F01-K09-15 | B1 | 013:BD-Terminator 013:BD-Terminator Off IFX2 |
| F01-K09-16 | C2 | 017:BD-Squash 017:BD-Squash Off IFX2 |
| F01-K09-17 | C#2 | 009:BD-Jazz 2 009:BD-Jazz 2 Off IFX2 |
| F01-K09-18 | D2 | 205:Zap 1 205:Zap 1 Off IFX2 |
| F01-K09-19 | D#2 | 358:Footstep 1 359:Footstep 2 Off IFX2 |
| F01-K09-20 | E2 | 319:Cyber Drum 319:Cyber Drum Off IFX2 |
| F01-K09-21 | F2 | 267:Tabla-Na 268:Tabla-Open Off IFX1 |
| F01-K09-22 | F#2 | 172:SD-Orchestral 172:SD-Orchestral 1 IFX1 |
| F01-K09-23 | G2 | 070:SD-Rap 2 070:SD-Rap 2 Off IFX1 |
| F01-K09-24 | G#2 | 068:SD-Dance 068:SD-Dance 1 IFX1 |
| F01-K09-25 | A2 | 052:SD-Amb.Piccolo 2 049:SD-Off Center Off IFX1 |
| F01-K09-26 | A#2 | 059:SD-Atomic 059:SD-Atomic Off IFX1 |
| F01-K09-27 | B2 | 249:Djembe-Open 249:Djembe-Open Off IFX2 |
| F01-K09-28 | C3 | 019:BD-Dance 2 019:BD-Dance 2 Off IFX2 |
| F01-K09-29 | C#3 | 285:Timbale Hi-Rim 1 285:Timbale Hi-Rim 1 Off IFX1 |
| F01-K09-30 | D3 | 050:SD-Jazz Ring 050:SD-Jazz Ring Off IFX1 |
| F01-K09-31 | D#3 | 207:Zap 3 207:Zap 3 Off IFX1 |
| F01-K09-32 | E3 | 182:88-SD 2(Old) 182:88-SD 2(Old) Off IFX1 |
| F01-K09-33 | F3 | 105:Tom 2-Lo 105:Tom 2-Lo Off IFX1 |
| F01-K09-34 | F#3 | 189:99-HHat Closed 189:99-HHat Closed 2 L/R |
| F01-K09-35 | G3 | 115:Tom-Brush Floor 115:Tom-Brush Floor Off IFX1 |
| F01-K09-36 | G#3 | 188:88-HHat Open 188:88-HHat Open 2 L/R |
| F01-K09-37 | A3 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K09-38 | A#3 | 292:Triangle-Open 190:99-HHat Open 2 L/R |
| F01-K09-39 | B3 | 198:Real E.Tom Off IFX1 |
| F01-K09-40 | C4 | 111:Tom-Jazz Hi Off IFX1 |
| F01-K09-41 | C#4 | 168:Orch Cym.-Open 154:Crash Cymbal 3 Off IFX1 |
| F01-K09-42 | D4 | 108:Tom 3-Lo Off IFX1 |
| F01-K09-43 | D#4 | 164:Ride-Jazz 163:Ride-Edge 2 Off IFX1 |
| F01-K09-44 | E4 | 394:Air Vortex 394:Air Vortex Off IFX1 |
| F01-K09-45 | F4 | 219:DJ-Record Stop 208:DJ-Scratch 1 Off IFX1 |
| F01-K09-46 | F#4 | 219:DJ-Record Stop 208:DJ-Scratch 1 Off L/R |
| F01-K09-47 | G4 | 221:DJ-Old Record 221:DJ-Old Record Off L/R |
| F01-K09-48 | G#4 | 406:Dist.Slide 1 407:Dist.Slide 2 Off IFX1 |
| F01-K09-49 | A4 | 168:Orch Cym.-Open 168:Orch Cym.-Open Off L/R |
| F01-K09-50 | A#4 | 155:China Cymbal 154:Crash Cymbal 3 Off IFX1 |
| F01-K09-51 | B4 | 309:Sleigh Bell 309:Sleigh Bell Off L/R |
| F01-K09-52 | C5 | 277:Vibraslap 277:Vibraslap Off IFX1 |
| F01-K09-53 | C#5 | 170:SD-Orch.Roll 1 170:SD-Orch.Roll 1 Off IFX1 |
| F01-K09-54 | D5 | 305:Cowbell 1 308:Mambo Bell Off IFX1 |
| F01-K09-55 | D#5 | 291:Tambourine-Acc.2 302:Caxixi-Hard Off L/R |
| F01-K09-56 | E5 | 390:Space Lore 390:Space Lore Off IFX1 |
| F01-K09-57 | F5 | 391:Swish Terra 391:Swish Terra Off IFX1 |
| F01-K09-58 | F#5 | 292:Triangle-Open 292:Triangle-Open Off IFX1 |
| F01-K09-59 | G5 | 392:Xylophone Spectr 392:Xylophone Spectr Off IFX1 |
| F01-K09-60 | G#5 | 290:Tambourine-Acc.1 289:Tambourine-Pull Off L/R |
| F01-K09-61 | A5 | 319:Cyber Drum 394:Air Vortex Off IFX1 |
| F01-K09-62 | A#5 | 397:Tubular 397:Tubular Off IFX1 |
| F01-K09-63 | B5 | 311:Finger Cymbal 311:Finger Cymbal Off IFX1 |
| F01-K09-64 | C6 | 255:Guiro-Long 255:Guiro-Long Off IFX1 |
| F01-K09-65 | C#6 | 199:Vocal SD 199:Vocal SD Off L/R |
| F01-K09-66 | D6 | 274:Taiko-Open 398:Gamelan Off IFX1 |
| F01-K09-67 | D#6 | 224:Band Hit 224:Band Hit Off IFX1 |
| F01-K09-68 | E6 | 389:Telephone Ring 389:Telephone Ring Off IFX1 |
| F01-K09-69 | F6 | 358:Footstep 1 359:Footstep 2 Off IFX2 |
| F01-K09-70 | F#6 | 205:Zap 1 205:Zap 1 Off IFX1 |
| F01-K09-71 | G6 | 393:Cricket Spectrum 393:Cricket Spectrum Off IFX1 |
| F01-K09-72 | G#6 | 396:Noise-FM Mod 396:Noise-FM Mod Off L/R |
| F01-K09-73 | A6 | 395:Noise-White 395:Noise-White Off IFX1 |
| F01-K09-74 | A#6 | 396:Noise-FM Mod 396:Noise-FM Mod Off L/R |
| F01-K09-75 | B6 | 304:Agogo Bell 304:Agogo Bell Off IFX1 |
| F01-K09-76 | C7 | 370:GunShot 370:GunShot Off IFX1 |
| F01-K09-77 | C#7 | 311:Finger Cymbal 312:Marc Tree Off IFX1 |
| F01-K09-78 | D7 | 316:Samba Whistle 316:Samba Whistle 3 IFX1 |
| F01-K09-79 | D#7 | 323:Industry Loop 2 320:SD-Metal Striker Off IFX1 |
| F01-K09-80 | E7 | 217:DJ-BD Rub 217:DJ-BD Rub Off L/R |
| F01-K09-81 | F7 | 318:Industry 318:Industry Off IFX1 |
| F01-K09-82 | F#7 | 407:Dist.Slide 2 406:Dist.Slide 1 Off IFX2 |
| F01-K09-83 | G7 | 403:HarpGliss 371:Rainstick Off L/R |
| F01-K09-84 | G#7 | 253:Castanet-Dble 1 277:Vibraslap Off L/R |
| F01-K09-85 | A7 | 219:DJ-Record Stop 220:DJ-Reverse Off IFX2 |
| F01-K09-86 | A#7 | 209:DJ-Scratch 2 208:DJ-Scratch 1 Off IFX2 |
| F01-K09-87 | B7 | 369:Tribe 369:Tribe Off IFX2 |
| F01-K09-88 | C8 | 195:88-Cowbell 195:88-Cowbell Off IFX1 |

### F01-K10 - Percussion Kit (kit 009, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K10-01 | A0 |  |
| F01-K10-02 | A#0 |  |
| F01-K10-03 | B0 |  |
| F01-K10-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off L/R |
| F01-K10-05 | C#1 | 276:Tsuzumi 276:Tsuzumi Off L/R |
| F01-K10-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off IFX1 |
| F01-K10-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off IFX1 |
| F01-K10-08 | E1 | 261:Baya-Ghe 260:Baya-Open 5 L/R |
| F01-K10-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off L/R |
| F01-K10-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off L/R |
| F01-K10-11 | G1 | 268:Tabla-Open 268:Tabla-Open Off L/R |
| F01-K10-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off L/R |
| F01-K10-13 | A1 | 271:Tabla-Mute 2 272:Tabla-Mute 3 Off L/R |
| F01-K10-14 | A#1 | 267:Tabla-Na 270:Tabla-Mute 1 6 L/R |
| F01-K10-15 | B1 | 270:Tabla-Mute 1 271:Tabla-Mute 2 Off L/R |
| F01-K10-16 | C2 | 166:BD-Orchestral Off L/R |
| F01-K10-17 | C#2 | 369:Tribe Off L/R |
| F01-K10-18 | D2 | 228:Finger Snap Off L/R |
| F01-K10-19 | D#2 | 229:Hand Claps Off L/R |
| F01-K10-20 | E2 | 293:Triangle-Mute 1 L/R |
| F01-K10-21 | F2 | 292:Triangle-Open 1 L/R |
| F01-K10-22 | F#2 | 254:Castanet-Dble 2 Off IFX1 |
| F01-K10-23 | G2 | 252:Castanet-Single Off IFX1 |
| F01-K10-24 | G#2 | 253:Castanet-Dble 1 Off IFX1 |
| F01-K10-25 | A2 | 255:Guiro-Long 2 L/R |
| F01-K10-26 | A#2 | 256:Guiro-Short 256:Guiro-Short 2 L/R |
| F01-K10-27 | B2 | 257:Guiro-Tap 2 L/R |
| F01-K10-28 | C3 | 277:Vibraslap Off IFX1 |
| F01-K10-29 | C#3 | 278:Claves Off IFX1 |
| F01-K10-30 | D3 | 194:88-Clave Off IFX1 |
| F01-K10-31 | D#3 | 296:Cuica-Lo 3 IFX1 |
| F01-K10-32 | E3 | 295:Cuica-Hi 3 IFX1 |
| F01-K10-33 | F3 | 281:Timbale Lo-Open Off IFX1 |
| F01-K10-34 | F#3 | 279:Woodblock 1 Off IFX1 |
| F01-K10-35 | G3 | 283:Timbale Lo-Rim Off IFX1 |
| F01-K10-36 | G#3 | 279:Woodblock 1 Off IFX1 |
| F01-K10-37 | A3 | 282:Timbale Lo-Mute Off IFX1 |
| F01-K10-38 | A#3 | 201:Vocal Woodblock Off IFX1 |
| F01-K10-39 | B3 | 284:Timbale Hi-Edge Off IFX1 |
| F01-K10-40 | C4 | 286:Timbale Hi-Rim 2 Off IFX1 |
| F01-K10-41 | C#4 | 307:Chacha Bell Off IFX1 |
| F01-K10-42 | D4 | 285:Timbale Hi-Rim 1 Off IFX1 |
| F01-K10-43 | D#4 | 308:Mambo Bell Off IFX1 |
| F01-K10-44 | E4 | 287:Timbale-Paila Off IFX1 |
| F01-K10-45 | F4 | 242:Bongo Lo-Open Off L/R |
| F01-K10-46 | F#4 | 305:Cowbell 1 Off L/R |
| F01-K10-47 | G4 | 243:Bongo Lo-Slap Off L/R |
| F01-K10-48 | G#4 | 306:Cowbell 2 Off L/R |
| F01-K10-49 | A4 | 245:Bongo Hi-Open Off L/R |
| F01-K10-50 | A#4 | 200:Vocal Cowbell Off L/R |
| F01-K10-51 | B4 | 246:Bongo Hi-Slap 246:Bongo Hi-Slap Off L/R |
| F01-K10-52 | C5 | 232:Conga Lo-Open 232:Conga Lo-Open Off L/R |
| F01-K10-53 | C#5 | 234:Conga Lo-Slap Off L/R |
| F01-K10-54 | D5 | 233:Conga Lo-MtSlap Off L/R |
| F01-K10-55 | D#5 | 237:Conga Hi-MtSlap Off L/R |
| F01-K10-56 | E5 | 235:Conga Hi-Open 235:Conga Hi-Open Off L/R |
| F01-K10-57 | F5 | 236:Conga Hi-Mute Off L/R |
| F01-K10-58 | F#5 | 240:Conga-Heel Off L/R |
| F01-K10-59 | G5 | 238:Conga Hi-Slap 1 Off L/R |
| F01-K10-60 | G#5 | 241:Conga-Toe Off L/R |
| F01-K10-61 | A5 | 239:Conga Hi-Slap 2 Off L/R |
| F01-K10-62 | A#5 | 304:Agogo Bell Off IFX1 |
| F01-K10-63 | B5 | 304:Agogo Bell Off IFX1 |
| F01-K10-64 | C6 | 259:Maracas-Pull 258:Maracas-Push Off IFX1 |
| F01-K10-65 | C#6 | 297:Shaker 1 297:Shaker 1 Off L/R |
| F01-K10-66 | D6 | 298:Shaker 2 298:Shaker 2 Off L/R |
| F01-K10-67 | D#6 | 301:Cabasa-Tap 300:Cabasa-Down Off L/R |
| F01-K10-68 | E6 | 301:Cabasa-Tap 299:Cabasa-Up Off L/R |
| F01-K10-69 | F6 | 302:Caxixi-Hard 303:Caxixi-Soft Off L/R |
| F01-K10-70 | F#6 | 288:Tambourine-Push Off L/R |
| F01-K10-71 | G6 | 290:Tambourine-Acc.1 290:Tambourine-Acc.1 Off L/R |
| F01-K10-72 | G#6 | 289:Tambourine-Pull Off L/R |
| F01-K10-73 | A6 | 291:Tambourine-Acc.2 291:Tambourine-Acc.2 Off IFX1 |
| F01-K10-74 | A#6 | 309:Sleigh Bell 309:Sleigh Bell Off L/R |
| F01-K10-75 | B6 | 316:Samba Whistle 4 L/R |
| F01-K10-76 | C7 | 316:Samba Whistle 4 L/R |
| F01-K10-77 | C#7 | 314:Bell Tree Off L/R |
| F01-K10-78 | D7 | 316:Samba Whistle 4 L/R |
| F01-K10-79 | D#7 | 312:Marc Tree Off L/R |
| F01-K10-80 | E7 | 316:Samba Whistle 4 L/R |
| F01-K10-81 | F7 | 310:Rap Sleighbell 310:Rap Sleighbell Off L/R |
| F01-K10-82 | F#7 | 371:Rainstick Off L/R |
| F01-K10-83 | G7 | 317:Chinese Gong Off L/R |
| F01-K10-84 | G#7 | 361:Bird 1 Off L/R |
| F01-K10-85 | A7 | 362:Bird 2 Off L/R |
| F01-K10-86 | A#7 | 361:Bird 1 Off L/R |
| F01-K10-87 | B7 | 393:Cricket Spectrum Off L/R |
| F01-K10-88 | C8 | 312:Marc Tree Off L/R |

### F01-K11 - Orchestra&Ethnic (kit 010, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K11-01 | A0 |  |
| F01-K11-02 | A#0 |  |
| F01-K11-03 | B0 |  |
| F01-K11-04 | C1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-05 | C#1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-06 | D1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-07 | D#1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-08 | E1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-09 | F1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-10 | F#1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-11 | G1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-12 | G#1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-13 | A1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-14 | A#1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-15 | B1 | 222:Orchestra Hit 222:Orchestra Hit Off L/R |
| F01-K11-16 | C2 | 166:BD-Orchestral 166:BD-Orchestral Off IFX2 |
| F01-K11-17 | C#2 | 170:SD-Orch.Roll 1 170:SD-Orch.Roll 1 1 L/R |
| F01-K11-18 | D2 | 172:SD-Orchestral 172:SD-Orchestral Off L/R |
| F01-K11-19 | D#2 | 171:SD-Orch.Roll 2 171:SD-Orch.Roll 2 1 L/R |
| F01-K11-20 | E2 | 172:SD-Orchestral 172:SD-Orchestral 1 L/R |
| F01-K11-21 | F2 | 168:Orch Cym.-Open 168:Orch Cym.-Open 2 L/R |
| F01-K11-22 | F#2 | 167:Orch Cym.-Closed 167:Orch Cym.-Closed 2 L/R |
| F01-K11-23 | G2 | 274:Taiko-Open 274:Taiko-Open Off IFX2 |
| F01-K11-24 | G#2 | 170:SD-Orch.Roll 1 170:SD-Orch.Roll 1 3 L/R |
| F01-K11-25 | A2 | 172:SD-Orchestral 172:SD-Orchestral Off L/R |
| F01-K11-26 | A#2 | 171:SD-Orch.Roll 2 171:SD-Orch.Roll 2 3 L/R |
| F01-K11-27 | B2 | 172:SD-Orchestral 172:SD-Orchestral 3 L/R |
| F01-K11-28 | C3 | 293:Triangle-Mute 293:Triangle-Mute 4 L/R |
| F01-K11-29 | C#3 | 294:Triangle-Roll 294:Triangle-Roll 4 L/R |
| F01-K11-30 | D3 | 292:Triangle-Open 292:Triangle-Open 4 L/R |
| F01-K11-31 | D#3 | 317:Chinese Gong 317:Chinese Gong Off IFX1 |
| F01-K11-32 | E3 | 291:Tambourine-Acc.2 291:Tambourine-Acc.2 Off L/R |
| F01-K11-33 | F3 | 279:Woodblock 1 279:Woodblock 1 Off IFX1 |
| F01-K11-34 | F#3 | 253:Castanet-Dble 1 253:Castanet-Dble 1 Off L/R |
| F01-K11-35 | G3 | 279:Woodblock 1 279:Woodblock 1 Off IFX1 |
| F01-K11-36 | G#3 | 254:Castanet-Dble 2 254:Castanet-Dble 2 Off L/R |
| F01-K11-37 | A3 | 279:Woodblock 1 279:Woodblock 1 Off IFX1 |
| F01-K11-38 | A#3 | 252:Castanet-Single 252:Castanet-Single Off L/R |
| F01-K11-39 | B3 | 279:Woodblock 1 279:Woodblock 1 Off IFX1 |
| F01-K11-40 | C4 | 279:Woodblock 1 279:Woodblock 1 Off IFX1 |
| F01-K11-41 | C#4 | 169:Timpani 169:Timpani Off IFX2 |
| F01-K11-42 | D4 | 169:Timpani 169:Timpani Off IFX2 |
| F01-K11-43 | D#4 | 169:Timpani 169:Timpani Off IFX2 |
| F01-K11-44 | E4 | 169:Timpani 169:Timpani Off IFX2 |
| F01-K11-45 | F4 | 249:Djembe-Open 249:Djembe-Open Off IFX1 |
| F01-K11-46 | F#4 | 250:Djembe-Mute 250:Djembe-Mute Off IFX1 |
| F01-K11-47 | G4 | 251:Djembe-Slap 251:Djembe-Slap Off IFX1 |
| F01-K11-48 | G#4 | 302:Caxixi-Hard 303:Caxixi-Soft Off L/R |
| F01-K11-49 | A4 | 309:Sleigh Bell 309:Sleigh Bell Off IFX1 |
| F01-K11-50 | A#4 | 299:Cabasa-Up 300:Cabasa-Down Off L/R |
| F01-K11-51 | B4 | 311:Finger Cymbal 311:Finger Cymbal Off L/R |
| F01-K11-52 | C5 | 261:Baya-Ghe 261:Baya-Ghe Off IFX1 |
| F01-K11-53 | C#5 | 260:Baya-Open 260:Baya-Open Off IFX1 |
| F01-K11-54 | D5 | 262:Baya-Mute 1 262:Baya-Mute 1 Off IFX1 |
| F01-K11-55 | D#5 | 263:Baya-Mute 2 263:Baya-Mute 2 Off IFX1 |
| F01-K11-56 | E5 | 264:Baya-Mute 3 264:Baya-Mute 3 Off IFX1 |
| F01-K11-57 | F5 | 265:Baya-Mute 4 265:Baya-Mute 4 Off IFX1 |
| F01-K11-58 | F#5 | 266:Baya-Mute 5 266:Baya-Mute 5 Off IFX1 |
| F01-K11-59 | G5 | 267:Tabla-Na 267:Tabla-Na Off IFX1 |
| F01-K11-60 | G#5 | 268:Tabla-Open 268:Tabla-Open Off IFX1 |
| F01-K11-61 | A5 | 269:Tabla-Tin 269:Tabla-Tin Off IFX1 |
| F01-K11-62 | A#5 | 270:Tabla-Mute 1 270:Tabla-Mute 1 Off IFX1 |
| F01-K11-63 | B5 | 271:Tabla-Mute 2 271:Tabla-Mute 2 Off IFX1 |
| F01-K11-64 | C6 | 272:Tabla-Mute 3 272:Tabla-Mute 3 Off IFX1 |
| F01-K11-65 | C#6 | 273:Tabla-Mute 4 273:Tabla-Mute 4 Off IFX1 |
| F01-K11-66 | D6 | 274:Taiko-Open 274:Taiko-Open Off IFX1 |
| F01-K11-67 | D#6 | 276:Tsuzumi Off IFX1 |
| F01-K11-68 | E6 | 275:Taiko-Rim 275:Taiko-Rim Off IFX1 |
| F01-K11-69 | F6 | 232:Conga Lo-Open Off IFX1 |
| F01-K11-70 | F#6 | 233:Conga Lo-MtSlap 239:Conga Hi-Slap 2 Off IFX1 |
| F01-K11-71 | G6 | 236:Conga Hi-Mute 246:Bongo Hi-Slap Off IFX1 |
| F01-K11-72 | G#6 | 238:Conga Hi-Slap 1 Off IFX1 |
| F01-K11-73 | A6 | 399:Tambura 399:Tambura Off L/R |
| F01-K11-74 | A#6 | 371:Rainstick 371:Rainstick Off L/R |
| F01-K11-75 | B6 | 403:HarpGliss 403:HarpGliss Off L/R |
| F01-K11-76 | C7 | 371:Rainstick 371:Rainstick Off L/R |
| F01-K11-77 | C#7 | 000:BD-Dry 1 000:BD-Dry 1 Off IFX1 |
| F01-K11-78 | D7 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K11-79 | D#7 | 398:Gamelan 398:Gamelan Off L/R |
| F01-K11-80 | E7 | 398:Gamelan 398:Gamelan Off L/R |
| F01-K11-81 | F7 | 361:Bird 1 361:Bird 1 11 L/R |
| F01-K11-82 | F#7 | 393:Cricket Spectrum Off L/R |
| F01-K11-83 | G7 | 369:Tribe 369:Tribe Off L/R |
| F01-K11-84 | G#7 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K11-85 | A7 | 398:Gamelan 398:Gamelan Off L/R |
| F01-K11-86 | A#7 | 398:Gamelan Off L/R |
| F01-K11-87 | B7 | 398:Gamelan 398:Gamelan Off L/R |
| F01-K11-88 | C8 | 398:Gamelan 398:Gamelan Off L/R |

### F01-K12 - Cymbals Catalog (kit 011, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K12-01 | A0 |  |
| F01-K12-02 | A#0 |  |
| F01-K12-03 | B0 |  |
| F01-K12-04 | C1 |  |
| F01-K12-05 | C#1 |  |
| F01-K12-06 | D1 |  |
| F01-K12-07 | D#1 |  |
| F01-K12-08 | E1 | 258:Maracas-Push Off L/R |
| F01-K12-09 | F1 | 259:Maracas-Pull Off L/R |
| F01-K12-10 | F#1 | 302:Caxixi-Hard Off L/R |
| F01-K12-11 | G1 | 202:Dance Perc-Hip Off L/R |
| F01-K12-12 | G#1 | 288:Tambourine-Push 6 L/R |
| F01-K12-13 | A1 | 289:Tambourine-Pull 6 L/R |
| F01-K12-14 | A#1 | 290:Tambourine-Acc.1 6 L/R |
| F01-K12-15 | B1 | 291:Tambourine-Acc.2 6 L/R |
| F01-K12-16 | C2 | 024:BD-Hip 3 24: BD-Hip 3 Off L/R |
| F01-K12-17 | C#2 | 125:H.Hat 2-Closed 1 1 L/R |
| F01-K12-18 | D2 | 093:SD-Chili 93: SD-Chili Off L/R |
| F01-K12-19 | D#2 | 126:H.Hat 2-Closed 2 1 L/R |
| F01-K12-20 | E2 | 118:H.Hat 1-Closed 1 1 L/R |
| F01-K12-21 | F2 | 119:H.Hat 1-Closed 2 1 L/R |
| F01-K12-22 | F#2 | 130:H.Hat 2-Open 2 1 L/R |
| F01-K12-23 | G2 | 123:H.Hat 1-Open 2 1 L/R |
| F01-K12-24 | G#2 | 129:H.Hat 2-Open 1 1 L/R |
| F01-K12-25 | A2 | 122:H.Hat 1-Open 1 1 L/R |
| F01-K12-26 | A#2 | 131:H.Hat 2-Sizzle 1 L/R |
| F01-K12-27 | B2 | 124:H.Hat 1-Sizzle 1 L/R |
| F01-K12-28 | C3 | 120:H.Hat 1-Foot 1 L/R |
| F01-K12-29 | C#3 | 127:H.Hat 2-Foot 1 L/R |
| F01-K12-30 | D3 | 121:H.Hat 1-FootOpen 1 L/R |
| F01-K12-31 | D#3 | 128:H.Hat 2-FootOpen 1 L/R |
| F01-K12-32 | E3 | 132:H.Hat-Chili Off L/R |
| F01-K12-33 | F3 | 136:H.Hat-Grange Off L/R |
| F01-K12-34 | F#3 | 133:H.Hat-Crisp Cl 1 Off L/R |
| F01-K12-35 | G3 | 137:H.Hat-Vintage 1 Off L/R |
| F01-K12-36 | G#3 | 134:H.Hat-Crisp Cl 2 3 L/R |
| F01-K12-37 | A3 | 138:H.Hat-Vintage 2 Off L/R |
| F01-K12-38 | A#3 | 135:H.Hat-Crisp Open 3 L/R |
| F01-K12-39 | B3 | 139:H.Hat-Tight 1 Off L/R |
| F01-K12-40 | C4 | 140:H.Hat-Tight 2 3 L/R |
| F01-K12-41 | C#4 | 144:H.Hat-Old Open 3 L/R |
| F01-K12-42 | D4 | 141:H.Hat-Old Cl 1 Off L/R |
| F01-K12-43 | D#4 | 149:H.Hat-ClangyOpen 3 L/R |
| F01-K12-44 | E4 | 142:H.Hat-Old Cl 2 Off L/R |
| F01-K12-45 | F4 | 143:H.Hat-Old Cl 3 Off L/R |
| F01-K12-46 | F#4 | 151:H.Hat-Alpo Close Off L/R |
| F01-K12-47 | G4 | 147:H.Hat-AmbCrackle Off L/R |
| F01-K12-48 | G#4 | 150:H.Hat-Whispy Off L/R |
| F01-K12-49 | A4 | 148:H.Hat-Ambi Off L/R |
| F01-K12-50 | A#4 | 146:H.Hat-Hip 2 Off L/R |
| F01-K12-51 | B4 | 145:H.Hat-Hip 1 Off L/R |
| F01-K12-52 | C5 | 189:99-HHat Closed 4 L/R |
| F01-K12-53 | C#5 | 309:Sleigh Bell Off L/R |
| F01-K12-54 | D5 | 190:99-HHat Open 4 L/R |
| F01-K12-55 | D#5 | 310:Rap Sleighbell Off L/R |
| F01-K12-56 | E5 | 187:88-HHat Closed 5 L/R |
| F01-K12-57 | F5 | 188:88-HHat Open 5 L/R |
| F01-K12-58 | F#5 | 154:Crash Cymbal 3 Off L/R |
| F01-K12-59 | G5 | 191:88-Crash Off L/R |
| F01-K12-60 | G#5 | 152:Crash Cymbal 1 Off L/R |
| F01-K12-61 | A5 | 155:China Cymbal Off L/R |
| F01-K12-62 | A#5 | 153:Crash Cymbal 2 Off L/R |
| F01-K12-63 | B5 | 156:Splash Cymbal Off L/R |
| F01-K12-64 | C6 | 160:Ride-Dance 99 Off L/R |
| F01-K12-65 | C#6 | 158:Crash-Dance 99 Off L/R |
| F01-K12-66 | D6 | 161:Ride-Dance DDD1 Off L/R |
| F01-K12-67 | D#6 | 159:Crash-Dance DDD1 Off L/R |
| F01-K12-68 | E6 | 162:Ride-Edge 1 Off L/R |
| F01-K12-69 | F6 | 163:Ride-Edge 2 Off L/R |
| F01-K12-70 | F#6 | 164:Ride-Jazz Off L/R |
| F01-K12-71 | G6 | 165:Ride-Cup Off L/R |
| F01-K12-72 | G#6 | 157:Cymbal Reverse Off L/R |
| F01-K12-73 | A6 | 156:Splash Cymbal Off L/R |
| F01-K12-74 | A#6 | 158:Crash-Dance 99 Off L/R |
| F01-K12-75 | B6 | 160:Ride-Dance 99 Off L/R |
| F01-K12-76 | C7 | 165:Ride-Cup Off L/R |
| F01-K12-77 | C#7 | 190:99-HHat Open Off L/R |
| F01-K12-78 | D7 | 142:H.Hat-Old Cl 2 Off L/R |
| F01-K12-79 | D#7 | 143:H.Hat-Old Cl 3 Off L/R |
| F01-K12-80 | E7 | 134:H.Hat-Crisp Cl 2 Off L/R |
| F01-K12-81 | F7 | 189:99-HHat Closed Off L/R |
| F01-K12-82 | F#7 | 125:H.Hat 2-Closed 1 Off L/R |
| F01-K12-83 | G7 | 187:88-HHat Closed Off L/R |
| F01-K12-84 | G#7 |  |
| F01-K12-85 | A7 |  |
| F01-K12-86 | A#7 |  |
| F01-K12-87 | B7 |  |
| F01-K12-88 | C8 |  |

### F01-K13 - BD&SD Catalog 1 (kit 012, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K13-01 | A0 |  |
| F01-K13-02 | A#0 |  |
| F01-K13-03 | B0 |  |
| F01-K13-04 | C1 |  |
| F01-K13-05 | C#1 |  |
| F01-K13-06 | D1 |  |
| F01-K13-07 | D#1 |  |
| F01-K13-08 | E1 |  |
| F01-K13-09 | F1 |  |
| F01-K13-10 | F#1 |  |
| F01-K13-11 | G1 |  |
| F01-K13-12 | G#1 |  |
| F01-K13-13 | A1 |  |
| F01-K13-14 | A#1 |  |
| F01-K13-15 | B1 |  |
| F01-K13-16 | C2 | 000:BD-Dry 1 000:BD-Dry 1 Off IFX2 |
| F01-K13-17 | C#2 | 001:BD-Dry 2 001:BD-Dry 2 Off IFX2 |
| F01-K13-18 | D2 | 002:BD-Dry 3 002:BD-Dry 3 Off IFX2 |
| F01-K13-19 | D#2 | 003:BD-Dry 4 003:BD-Dry 4 Off IFX2 |
| F01-K13-20 | E2 | 004:BD-Dry 5 004:BD-Dry 5 Off IFX2 |
| F01-K13-21 | F2 | 006:BD-Soft 006:BD-Soft Off IFX2 |
| F01-K13-22 | F#2 | 007:BD-Soft Room 007:BD-Soft Room Off IFX2 |
| F01-K13-23 | G2 | 008:BD-Jazz 1 008:BD-Jazz 1 Off IFX2 |
| F01-K13-24 | G#2 | 009:BD-Jazz 2 009:BD-Jazz 2 Off IFX2 |
| F01-K13-25 | A2 | 010:BD-Pillow 010:BD-Pillow Off IFX2 |
| F01-K13-26 | A#2 | 011:BD-Woofer 011:BD-Woofer Off IFX2 |
| F01-K13-27 | B2 | 012:BD-Mondo Kill 012:BD-Mondo Kill Off IFX2 |
| F01-K13-28 | C3 | 013:BD-Terminator 013:BD-Terminator Off IFX2 |
| F01-K13-29 | C#3 | 014:BD-Tubby 014:BD-Tubby Off IFX2 |
| F01-K13-30 | D3 | 015:BD-Gated 015:BD-Gated Off IFX2 |
| F01-K13-31 | D#3 | 016:BD-Tight 016:BD-Tight Off IFX2 |
| F01-K13-32 | E3 | 017:BD-Squash 017:BD-Squash Off IFX2 |
| F01-K13-33 | F3 | 020:BD-Dance 3 020:BD-Dance 3 Off IFX2 |
| F01-K13-34 | F#3 | 026:BD-Dark 026:BD-Dark Off IFX2 |
| F01-K13-35 | G3 | 028:BD-Ringy 028:BD-Ringy Off IFX2 |
| F01-K13-36 | G#3 | 034:BD-Dance Soft 034:BD-Dance Soft Off IFX2 |
| F01-K13-37 | A3 | 037:BD-PhatJaz 037:BD-PhatJaz Off IFX2 |
| F01-K13-38 | A#3 | 166:BD-Orchestral 166:BD-Orchestral Off IFX2 |
| F01-K13-39 | B3 | 040:SD-Dry 1 040:SD-Dry 1 Off IFX1 |
| F01-K13-40 | C4 | 041:SD-Dry 2 041:SD-Dry 2 Off IFX1 |
| F01-K13-41 | C#4 | 042:SD-Dry 3 042:SD-Dry 3 Off IFX1 |
| F01-K13-42 | D4 | 043:SD-Normal 043:SD-Normal Off IFX1 |
| F01-K13-43 | D#4 | 044:SD-GhostSet roll 044:SD-GhostSet roll Off IFX1 |
| F01-K13-44 | E4 | 047:SD-GhostSet p2 047:SD-GhostSet p2 Off IFX1 |
| F01-K13-45 | F4 | 046:SD-GhostSet p1 046:SD-GhostSet p1 Off IFX1 |
| F01-K13-46 | F#4 | 045:SD-GhostSet f 045:SD-GhostSet f Off IFX1 |
| F01-K13-47 | G4 | 048:SD-Full Room 048:SD-Full Room Off IFX1 |
| F01-K13-48 | G#4 | 049:SD-Off Center 049:SD-Off Center Off IFX1 |
| F01-K13-49 | A4 | 050:SD-Jazz Ring 050:SD-Jazz Ring Off IFX1 |
| F01-K13-50 | A#4 | 051:SD-Amb.Piccolo 1 051:SD-Amb.Piccolo 1 Off IFX1 |
| F01-K13-51 | B4 | 052:SD-Amb.Piccolo 2 052:SD-Amb.Piccolo 2 Off IFX1 |
| F01-K13-52 | C5 | 053:SD-Paper 053:SD-Paper Off IFX1 |
| F01-K13-53 | C#5 | 054:SD-Classic Room 054:SD-Classic Room Off IFX1 |
| F01-K13-54 | D5 | 055:SD-Brush Hit 055:SD-Brush Hit Off IFX1 |
| F01-K13-55 | D#5 | 056:SD-Brush Tap 1 056:SD-Brush Tap 1 Off IFX1 |
| F01-K13-56 | E5 | 057:SD-Brush Tap 2 057:SD-Brush Tap 2 Off IFX1 |
| F01-K13-57 | F5 | 058:SD-Brush Swirl 058:SD-Brush Swirl Off IFX1 |
| F01-K13-58 | F#5 | 059:SD-Atomic 059:SD-Atomic Off IFX1 |
| F01-K13-59 | G5 | 060:SD-Big Rock 060:SD-Big Rock Off IFX1 |
| F01-K13-60 | G#5 | 061:SD-Yowie 061:SD-Yowie Off IFX1 |
| F01-K13-61 | A5 | 062:SD-Processed 1 062:SD-Processed 1 Off IFX1 |
| F01-K13-62 | A#5 | 063:SD-Processed 2 063:SD-Processed 2 Off IFX1 |
| F01-K13-63 | B5 | 064:SD-Cracker Room 064:SD-Cracker Room Off IFX1 |
| F01-K13-64 | C6 | 170:SD-Orch.Roll 1 170:SD-Orch.Roll 1 Off IFX1 |
| F01-K13-65 | C#6 | 065:Sidestick-Dry 065:Sidestick-Dry Off IFX1 |
| F01-K13-66 | D6 | 171:SD-Orch.Roll 2 171:SD-Orch.Roll 2 Off IFX1 |
| F01-K13-67 | D#6 | 152:Crash Cymbal 1 152:Crash Cymbal 1 Off L/R |
| F01-K13-68 | E6 | 172:SD-Orchestral 172:SD-Orchestral Off IFX1 |
| F01-K13-69 | F6 | 073:SD-Reverse 073:SD-Reverse Off IFX1 |
| F01-K13-70 | F#6 | 118:H.Hat 1-Closed 1 118:H.Hat 1-Closed 1 1 L/R |
| F01-K13-71 | G6 | 078:SD-Hip 5 078:SD-Hip 5 Off IFX1 |
| F01-K13-72 | G#6 | 120:H.Hat 1-Foot 120:H.Hat 1-Foot 1 L/R |
| F01-K13-73 | A6 | 091:SD-AmbiHop 091:SD-AmbiHop Off IFX1 |
| F01-K13-74 | A#6 | 122:H.Hat 1-Open 1 122:H.Hat 1-Open 1 1 L/R |
| F01-K13-75 | B6 | 095:SD-Whopper 095:SD-Whopper Off IFX1 |
| F01-K13-76 | C7 | 097:SD-Ambi Pop 097:SD-Ambi Pop Off IFX1 |
| F01-K13-77 | C#7 |  |
| F01-K13-78 | D7 |  |
| F01-K13-79 | D#7 |  |
| F01-K13-80 | E7 |  |
| F01-K13-81 | F7 |  |
| F01-K13-82 | F#7 |  |
| F01-K13-83 | G7 |  |
| F01-K13-84 | G#7 |  |
| F01-K13-85 | A7 |  |
| F01-K13-86 | A#7 |  |
| F01-K13-87 | B7 |  |
| F01-K13-88 | C8 |  |

### F01-K14 - BD&SD Catalog 2 (kit 013, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K14-01 | A0 |  |
| F01-K14-02 | A#0 |  |
| F01-K14-03 | B0 |  |
| F01-K14-04 | C1 |  |
| F01-K14-05 | C#1 |  |
| F01-K14-06 | D1 |  |
| F01-K14-07 | D#1 |  |
| F01-K14-08 | E1 |  |
| F01-K14-09 | F1 |  |
| F01-K14-10 | F#1 |  |
| F01-K14-11 | G1 |  |
| F01-K14-12 | G#1 |  |
| F01-K14-13 | A1 |  |
| F01-K14-14 | A#1 |  |
| F01-K14-15 | B1 |  |
| F01-K14-16 | C2 | 005:BD-Dry 6 005:BD-Dry 6 Off IFX2 |
| F01-K14-17 | C#2 | 018:BD-Dance 1 018:BD-Dance 1 Off IFX2 |
| F01-K14-18 | D2 | 019:BD-Dance 2 019:BD-Dance 2 Off IFX2 |
| F01-K14-19 | D#2 | 178:99-BD 3 178:99-BD 3 Off IFX2 |
| F01-K14-20 | E2 | 021:BD-Dance 4 021:BD-Dance 4 Off IFX2 |
| F01-K14-21 | F2 | 022:BD-Hip 1 022:BD-Hip 1 Off IFX2 |
| F01-K14-22 | F#2 | 023:BD-Hip 2 023:BD-Hip 2 Off IFX2 |
| F01-K14-23 | G2 | 024:BD-Hip 3 024:BD-Hip 3 Off IFX2 |
| F01-K14-24 | G#2 | 025:BD-Hip 4 025:BD-Hip 4 Off IFX2 |
| F01-K14-25 | A2 | 027:BD-Pop 027:BD-Pop Off IFX2 |
| F01-K14-26 | A#2 | 029:BD-Short88 029:BD-Short88 Off IFX2 |
| F01-K14-27 | B2 | 030:BD-Dance99 030:BD-Dance99 Off IFX2 |
| F01-K14-28 | C3 | 031:BD-Ambi 031:BD-Ambi Off IFX2 |
| F01-K14-29 | C#3 | 032:BD-Ambi Crackle 032:BD-Ambi Crackle Off IFX2 |
| F01-K14-30 | D3 | 033:BD-Ambi Soft 033:BD-Ambi Soft Off IFX2 |
| F01-K14-31 | D#3 | 035:BD-AmbiRocker 035:BD-AmbiRocker Off IFX2 |
| F01-K14-32 | E3 | 036:BD-Chili 036:BD-Chili Off IFX2 |
| F01-K14-33 | F3 | 180:99-BD 5 180:99-BD 5 Off IFX2 |
| F01-K14-34 | F#3 | 175:88-BD 3 175:88-BD 3 Off IFX2 |
| F01-K14-35 | G3 | 038:BD-Cannon 038:BD-Cannon Off IFX2 |
| F01-K14-36 | G#3 | 039:BD-Klanger 039:BD-Klanger Off IFX2 |
| F01-K14-37 | A3 | 173:88-BD 1(Rap) 173:88-BD 1(Rap) Off IFX2 |
| F01-K14-38 | A#3 | 174:88-BD 2(R&B) 174:88-BD 2(R&B) Off IFX2 |
| F01-K14-39 | B3 | 176:99-BD 1 176:99-BD 1 Off IFX2 |
| F01-K14-40 | C4 | 177:99-BD 2 177:99-BD 2 Off IFX2 |
| F01-K14-41 | C#4 | 179:99-BD 4 179:99-BD 4 Off IFX2 |
| F01-K14-42 | D4 | 067:SD-Amb.House 067:SD-Amb.House Off IFX1 |
| F01-K14-43 | D#4 | 068:SD-Dance 068:SD-Dance Off IFX1 |
| F01-K14-44 | E4 | 069:SD-Rap 1 069:SD-Rap 1 Off IFX1 |
| F01-K14-45 | F4 | 070:SD-Rap 2 070:SD-Rap 2 Off IFX1 |
| F01-K14-46 | F#4 | 071:SD-Noise 071:SD-Noise Off IFX1 |
| F01-K14-47 | G4 | 072:SD-Sizzle 072:SD-Sizzle Off IFX1 |
| F01-K14-48 | G#4 | 074:SD-Hip 1 074:SD-Hip 1 Off IFX1 |
| F01-K14-49 | A4 | 075:SD-Hip 2 075:SD-Hip 2 Off IFX1 |
| F01-K14-50 | A#4 | 076:SD-Hip 3 076:SD-Hip 3 Off IFX1 |
| F01-K14-51 | B4 | 077:SD-Hip 4 077:SD-Hip 4 Off IFX1 |
| F01-K14-52 | C5 | 079:SD-Hip 6 079:SD-Hip 6 Off IFX1 |
| F01-K14-53 | C#5 | 080:SD-Ringy 080:SD-Ringy Off IFX1 |
| F01-K14-54 | D5 | 081:SD-Tiny 081:SD-Tiny Off IFX1 |
| F01-K14-55 | D#5 | 082:SD-Vintage 1 082:SD-Vintage 1 Off IFX1 |
| F01-K14-56 | E5 | 083:SD-Vintage 2 083:SD-Vintage 2 Off IFX1 |
| F01-K14-57 | F5 | 084:SD-Vintage 3 084:SD-Vintage 3 Off IFX1 |
| F01-K14-58 | F#5 | 085:SD-Vintage 4 085:SD-Vintage 4 Off IFX1 |
| F01-K14-59 | G5 | 086:SD-Vintage 5 086:SD-Vintage 5 Off IFX1 |
| F01-K14-60 | G#5 | 087:SD-Vintage 6 087:SD-Vintage 6 Off IFX1 |
| F01-K14-61 | A5 | 088:SD-AmbiCrackle 1 088:SD-AmbiCrackle 1 Off IFX1 |
| F01-K14-62 | A#5 | 089:SD-AmbiCrackle 2 089:SD-AmbiCrackle 2 Off IFX1 |
| F01-K14-63 | B5 | 090:SD-AmbiCrackle 3 090:SD-AmbiCrackle 3 Off IFX1 |
| F01-K14-64 | C6 | 092:SD-Brasser 092:SD-Brasser Off IFX1 |
| F01-K14-65 | C#6 | 093:SD-Chili 093:SD-Chili Off IFX1 |
| F01-K14-66 | D6 | 094:SD-Klanger 094:SD-Klanger Off IFX1 |
| F01-K14-67 | D#6 | 096:SD-Dry Present 096:SD-Dry Present Off IFX1 |
| F01-K14-68 | E6 | 098:SD-Ambi Hit 1 098:SD-Ambi Hit 1 Off IFX1 |
| F01-K14-69 | F6 | 099:SD-Ambi Hit 2 099:SD-Ambi Hit 2 Off IFX1 |
| F01-K14-70 | F#6 | 187:88-HHat Closed 187:88-HHat Closed 1 L/R |
| F01-K14-71 | G6 | 100:SD-Ambi Hit 3 100:SD-Ambi Hit 3 Off IFX1 |
| F01-K14-72 | G#6 | 189:99-HHat Closed 189:99-HHat Closed 1 L/R |
| F01-K14-73 | A6 | 181:88-SD 1(New) 181:88-SD 1(New) Off IFX1 |
| F01-K14-74 | A#6 | 190:99-HHat Open 190:99-HHat Open 1 L/R |
| F01-K14-75 | B6 | 182:88-SD 2(Old) 182:88-SD 2(Old) Off IFX1 |
| F01-K14-76 | C7 | 183:99-SD 1 183:99-SD 1 Off IFX1 |
| F01-K14-77 | C#7 | 184:99-SD 2 184:99-SD 2 Off IFX1 |
| F01-K14-78 | D7 | 199:Vocal SD 199:Vocal SD Off IFX1 |
| F01-K14-79 | D#7 |  |
| F01-K14-80 | E7 |  |
| F01-K14-81 | F7 |  |
| F01-K14-82 | F#7 |  |
| F01-K14-83 | G7 |  |
| F01-K14-84 | G#7 |  |
| F01-K14-85 | A7 |  |
| F01-K14-86 | A#7 |  |
| F01-K14-87 | B7 |  |
| F01-K14-88 | C8 |  |

### F01-K15 - 01/W Total Kit (kit 014, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K15-01 | A0 | 311:Finger Cymbal Off IFX1 |
| F01-K15-02 | A#0 | 295:Cuica-Hi 296:Cuica-Lo Off IFX1 |
| F01-K15-03 | B0 | 291:Tambourine-Acc.2 Off IFX1 |
| F01-K15-04 | C1 | 275:Taiko-Rim 274:Taiko-Open Off IFX1 |
| F01-K15-05 | C#1 | 252:Castanet-Single 253:Castanet-Dble 1 Off IFX1 |
| F01-K15-06 | D1 | 249:Djembe-Open 249:Djembe-Open Off IFX1 |
| F01-K15-07 | D#1 | 251:Djembe-Slap 250:Djembe-Mute Off IFX1 |
| F01-K15-08 | E1 | 261:Baya-Ghe 260:Baya-Open 8 IFX1 |
| F01-K15-09 | F1 | 262:Baya-Mute 1 264:Baya-Mute 3 Off IFX1 |
| F01-K15-10 | F#1 | 266:Baya-Mute 5 265:Baya-Mute 4 Off IFX1 |
| F01-K15-11 | G1 | 263:Baya-Mute 2 264:Baya-Mute 3 Off IFX1 |
| F01-K15-12 | G#1 | 269:Tabla-Tin 273:Tabla-Mute 4 Off IFX1 |
| F01-K15-13 | A1 | 013:BD-Terminator 013:BD-Terminator Off IFX1 |
| F01-K15-14 | A#1 | 064:SD-Cracker Room 073:SD-Reverse 9 IFX1 |
| F01-K15-15 | B1 | 009:BD-Jazz 2 Off IFX1 |
| F01-K15-16 | C2 | 011:BD-Woofer 011:BD-Woofer Off IFX1 |
| F01-K15-17 | C#2 | 010:BD-Pillow 010:BD-Pillow Off IFX1 |
| F01-K15-18 | D2 | 012:BD-Mondo Kill 012:BD-Mondo Kill Off IFX1 |
| F01-K15-19 | D#2 | 177:99-BD 2 177:99-BD 2 Off IFX1 |
| F01-K15-20 | E2 | 174:88-BD 2(R&B) 173:88-BD 1(Rap) 10 IFX1 |
| F01-K15-21 | F2 | 048:SD-Full Room 048:SD-Full Room Off IFX1 |
| F01-K15-22 | F#2 | 072:SD-Sizzle Off IFX1 |
| F01-K15-23 | G2 | 059:SD-Atomic 059:SD-Atomic Off IFX1 |
| F01-K15-24 | G#2 | 055:SD-Brush Hit 056:SD-Brush Tap 1 Off IFX1 |
| F01-K15-25 | A2 | 183:99-SD 1 183:99-SD 1 Off IFX1 |
| F01-K15-26 | A#2 | 066:Sidestick-Amb 066:Sidestick-Amb Off IFX1 |
| F01-K15-27 | B2 | 182:88-SD 2(Old) 181:88-SD 1(New) Off IFX1 |
| F01-K15-28 | C3 | 103:Tom 1-Floor 103:Tom 1-Floor Off IFX1 |
| F01-K15-29 | C#3 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K15-30 | D3 | 101:Tom 1-Hi 101:Tom 1-Hi Off IFX1 |
| F01-K15-31 | D#3 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K15-32 | E3 | 101:Tom 1-Hi 101:Tom 1-Hi Off IFX1 |
| F01-K15-33 | F3 | 119:H.Hat 1-Closed 2 118:H.Hat 1-Closed 1 1 IFX1 |
| F01-K15-34 | F#3 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K15-35 | G3 | 123:H.Hat 1-Open 2 119:H.Hat 1-Closed 2 1 IFX1 |
| F01-K15-36 | G#3 | 188:88-HHat Open 187:88-HHat Closed 2 IFX1 |
| F01-K15-37 | A3 | 122:H.Hat 1-Open 1 1 IFX1 |
| F01-K15-38 | A#3 | 190:99-HHat Open Off IFX1 |
| F01-K15-39 | B3 | 121:H.Hat 1-FootOpen 120:H.Hat 1-Foot 1 IFX1 |
| F01-K15-40 | C4 | 152:Crash Cymbal 1 Off IFX1 |
| F01-K15-41 | C#4 | 153:Crash Cymbal 2 Off IFX1 |
| F01-K15-42 | D4 | 155:China Cymbal Off IFX1 |
| F01-K15-43 | D#4 | 163:Ride-Edge 2 163:Ride-Edge 2 Off IFX1 |
| F01-K15-44 | E4 | 165:Ride-Cup Off IFX1 |
| F01-K15-45 | F4 | 297:Shaker 1 298:Shaker 2 Off IFX1 |
| F01-K15-46 | F#4 | 290:Tambourine-Acc.1 288:Tambourine-Push Off IFX1 |
| F01-K15-47 | G4 | 299:Cabasa-Up 300:Cabasa-Down Off IFX1 |
| F01-K15-48 | G#4 | 293:Triangle-Mute 3 IFX1 |
| F01-K15-49 | A4 | 292:Triangle-Open 3 IFX1 |
| F01-K15-50 | A#4 | 286:Timbale Hi-Rim 2 284:Timbale Hi-Edge Off IFX1 |
| F01-K15-51 | B4 | 281:Timbale Lo-Open 281:Timbale Lo-Open Off IFX1 |
| F01-K15-52 | C5 | 242:Bongo Lo-Open 242:Bongo Lo-Open Off IFX1 |
| F01-K15-53 | C#5 | 246:Bongo Hi-Slap 245:Bongo Hi-Open Off IFX1 |
| F01-K15-54 | D5 | 232:Conga Lo-Open 232:Conga Lo-Open Off IFX1 |
| F01-K15-55 | D#5 | 235:Conga Hi-Open 235:Conga Hi-Open Off IFX1 |
| F01-K15-56 | E5 | 237:Conga Hi-MtSlap 241:Conga-Toe Off IFX1 |
| F01-K15-57 | F5 | 305:Cowbell 1 Off IFX1 |
| F01-K15-58 | F#5 | 229:Hand Claps Off IFX1 |
| F01-K15-59 | G5 | 166:BD-Orchestral Off IFX1 |
| F01-K15-60 | G#5 | 073:SD-Reverse 4 IFX1 |
| F01-K15-61 | A5 | 061:SD-Yowie 4 IFX1 |
| F01-K15-62 | A#5 | 171:SD-Orch.Roll 2 170:SD-Orch.Roll 1 5 IFX1 |
| F01-K15-63 | B5 | 172:SD-Orchestral 172:SD-Orchestral 5 IFX1 |
| F01-K15-64 | C6 | 168:Orch Cym.-Open 167:Orch Cym.-Closed 6 IFX1 |
| F01-K15-65 | C#6 | 309:Sleigh Bell Off IFX1 |
| F01-K15-66 | D6 | 217:DJ-BD Rub 218:DJ-SD Rub 7 IFX1 |
| F01-K15-67 | D#6 | 219:DJ-Record Stop 220:DJ-Reverse 7 IFX1 |
| F01-K15-68 | E6 | 221:DJ-Old Record 7 IFX1 |
| F01-K15-69 | F6 | 391:Swish Terra 318:Industry Off IFX1 |
| F01-K15-70 | F#6 | 222:Orchestra Hit 222:Orchestra Hit Off IFX1 |
| F01-K15-71 | G6 | 223:Pizz Hit 223:Pizz Hit Off IFX1 |
| F01-K15-72 | G#6 | 224:Band Hit 224:Band Hit Off IFX1 |
| F01-K15-73 | A6 | 396:Noise-FM Mod 394:Air Vortex Off IFX1 |
| F01-K15-74 | A#6 | 317:Chinese Gong Off IFX1 |
| F01-K15-75 | B6 | 314:Bell Tree Off IFX1 |
| F01-K15-76 | C7 | 312:Marc Tree 312:Marc Tree Off IFX1 |
| F01-K15-77 | C#7 | 354:Stadium Off IFX1 |
| F01-K15-78 | D7 | 355:Applause Off IFX1 |
| F01-K15-79 | D#7 | 389:Telephone Ring 315:Flexatone 11 IFX1 |
| F01-K15-80 | E7 | 358:Footstep 1 358:Footstep 1 Off IFX1 |
| F01-K15-81 | F7 | 359:Footstep 2 359:Footstep 2 Off IFX1 |
| F01-K15-82 | F#7 | 369:Tribe Off IFX1 |
| F01-K15-83 | G7 | 361:Bird 1 361:Bird 1 Off IFX1 |
| F01-K15-84 | G#7 | 362:Bird 2 362:Bird 2 Off IFX1 |
| F01-K15-85 | A7 | 319:Cyber Drum 319:Cyber Drum Off IFX1 |
| F01-K15-86 | A#7 | 395:Noise-White 370:GunShot Off IFX1 |
| F01-K15-87 | B7 | 393:Cricket Spectrum 393:Cricket Spectrum Off IFX1 |
| F01-K15-88 | C8 | 390:Space Lore 390 Space Lore Off IFX1 |

### F01-K16 - 01/W Producer's (kit 015, D1 p29)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-K16-01 | A0 | 311:Finger Cymbal Off L/R |
| F01-K16-02 | A#0 | 295:Cuica-Hi 296:Cuica-Lo Off L/R |
| F01-K16-03 | B0 | 291:Tambourine-Acc.2 Off L/R |
| F01-K16-04 | C1 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K16-05 | C#1 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K16-06 | D1 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K16-07 | D#1 | 000:BD-Dry 1 000:BD-Dry 1 Off L/R |
| F01-K16-08 | E1 | 179:99-BD 4 179:99-BD 4 Off IFX1 |
| F01-K16-09 | F1 | 176:99-BD 1 176:99-BD 1 Off IFX1 |
| F01-K16-10 | F#1 | 173:88-BD 1(Rap) 173:88-BD 1(Rap) Off IFX1 |
| F01-K16-11 | G1 | 020:BD-Dance 3 020:BD-Dance 3 Off IFX1 |
| F01-K16-12 | G#1 | 019:BD-Dance 2 019:BD-Dance 2 Off IFX1 |
| F01-K16-13 | A1 | 018:BD-Dance 1 018:BD-Dance 1 Off IFX1 |
| F01-K16-14 | A#1 | 011:BD-Woofer 011:BD-Woofer Off IFX1 |
| F01-K16-15 | B1 | 008:BD-Jazz 1 008:BD-Jazz 1 Off IFX1 |
| F01-K16-16 | C2 | 016:BD-Tight 016:BD-Tight Off IFX1 |
| F01-K16-17 | C#2 | 007:BD-Soft Room 007:BD-Soft Room Off IFX1 |
| F01-K16-18 | D2 | 017:BD-Squash 017:BD-Squash Off IFX1 |
| F01-K16-19 | D#2 | 001:BD-Dry 2 001:BD-Dry 2 Off IFX1 |
| F01-K16-20 | E2 | 006:BD-Soft 006:BD-Soft Off IFX1 |
| F01-K16-21 | F2 | 003:BD-Dry 4 003:BD-Dry 4 Off IFX1 |
| F01-K16-22 | F#2 | 013:BD-Terminator 013:BD-Terminator Off IFX1 |
| F01-K16-23 | G2 | 015:BD-Gated 015:BD-Gated Off IFX1 |
| F01-K16-24 | G#2 | 000:BD-Dry 1 000:BD-Dry 1 Off IFX1 |
| F01-K16-25 | A2 | 010:BD-Pillow 010:BD-Pillow Off IFX1 |
| F01-K16-26 | A#2 | 014:BD-Tubby 014:BD-Tubby Off IFX1 |
| F01-K16-27 | B2 | 002:BD-Dry 3 002:BD-Dry 3 Off IFX1 |
| F01-K16-28 | C3 | 012:BD-Mondo Kill 012:BD-Mondo Kill Off IFX1 |
| F01-K16-29 | C#3 | 009:BD-Jazz 2 009:BD-Jazz 2 Off IFX1 |
| F01-K16-30 | D3 | 174:88-BD 2(R&B) 174:88-BD 2(R&B) 4 IFX1 |
| F01-K16-31 | D#3 | 177:99-BD 2 177:99-BD 2 Off IFX1 |
| F01-K16-32 | E3 | 051:SD-Amb.Piccolo 1 051:SD-Amb.Piccolo 1 Off IFX1 |
| F01-K16-33 | F3 | 041:SD-Dry 2 041:SD-Dry 2 Off IFX1 |
| F01-K16-34 | F#3 | 050:SD-Jazz Ring 050:SD-Jazz Ring Off IFX1 |
| F01-K16-35 | G3 | 040:SD-Dry 1 040:SD-Dry 1 Off IFX1 |
| F01-K16-36 | G#3 | 042:SD-Dry 3 042:SD-Dry 3 Off IFX1 |
| F01-K16-37 | A3 | 060:SD-Big Rock 060:SD-Big Rock Off IFX1 |
| F01-K16-38 | A#3 | 062:SD-Processed 1 062:SD-Processed 1 Off IFX1 |
| F01-K16-39 | B3 | 049:SD-Off Center 049:SD-Off Center Off IFX1 |
| F01-K16-40 | C4 | 065:Sidestick-Dry 065:Sidestick-Dry Off IFX1 |
| F01-K16-41 | C#4 | 073:SD-Reverse 5 IFX1 |
| F01-K16-42 | D4 | 061:SD-Yowie 061:SD-Yowie 5 IFX1 |
| F01-K16-43 | D#4 | 170:SD-Orch.Roll 1 1 IFX1 |
| F01-K16-44 | E4 | 172:SD-Orchestral 172:SD-Orchestral 1 IFX1 |
| F01-K16-45 | F4 | 181:88-SD 1(New) 181:88-SD 1(New) Off IFX1 |
| F01-K16-46 | F#4 | 184:99-SD 2 184:99-SD 2 Off IFX1 |
| F01-K16-47 | G4 | 052:SD-Amb.Piccolo 2 052:SD-Amb.Piccolo 2 Off IFX1 |
| F01-K16-48 | G#4 | 063:SD-Processed 2 063:SD-Processed 2 Off IFX1 |
| F01-K16-49 | A4 | 048:SD-Full Room 048:SD-Full Room Off IFX1 |
| F01-K16-50 | A#4 | 218:DJ-SD Rub 218:DJ-SD Rub Off IFX1 |
| F01-K16-51 | B4 | 182:88-SD 2(Old) 182:88-SD 2(Old) Off IFX1 |
| F01-K16-52 | C5 | 126:H.Hat 2-Closed 2 125:H.Hat 2-Closed 1 2 IFX1 |
| F01-K16-53 | C#5 | 126:H.Hat 2-Closed 2 125:H.Hat 2-Closed 1 2 IFX1 |
| F01-K16-54 | D5 | 129:H.Hat 2-Open 1 130:H.Hat 2-Open 2 2 IFX1 |
| F01-K16-55 | D#5 | 128:H.Hat 2-FootOpen 127:H.Hat 2-Foot 2 IFX1 |
| F01-K16-56 | E5 | 187:88-HHat Closed 187:88-HHat Closed 3 IFX1 |
| F01-K16-57 | F5 | 188:88-HHat Open 3 IFX1 |
| F01-K16-58 | F#5 | 152:Crash Cymbal 1 Off IFX1 |
| F01-K16-59 | G5 | 153:Crash Cymbal 2 Off IFX1 |
| F01-K16-60 | G#5 | 153:Crash Cymbal 2 Off IFX1 |
| F01-K16-61 | A5 | 155:China Cymbal 155:China Cymbal Off IFX1 |
| F01-K16-62 | A#5 | 156:Splash Cymbal Off IFX1 |
| F01-K16-63 | B5 | 163:Ride-Edge 2 163:Ride-Edge 2 Off IFX1 |
| F01-K16-64 | C6 | 165:Ride-Cup 165:Ride-Cup Off IFX1 |
| F01-K16-65 | C#6 | 103:Tom 1-Floor 103:Tom 1-Floor Off IFX1 |
| F01-K16-66 | D6 | 102:Tom 1-Lo 102:Tom 1-Lo Off IFX1 |
| F01-K16-67 | D#6 | 101:Tom 1-Hi 101:Tom 1-Hi Off IFX1 |
| F01-K16-68 | E6 | 101:Tom 1-Hi 101:Tom 1-Hi Off IFX1 |
| F01-K16-69 | F6 | 101:Tom 1-Hi 101:Tom 1-Hi Off IFX1 |
| F01-K16-70 | F#6 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K16-71 | G6 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K16-72 | G#6 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K16-73 | A6 | 110:Tom-Processed 110:Tom-Processed Off IFX1 |
| F01-K16-74 | A#6 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-75 | B6 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-76 | C7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-77 | C#7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-78 | D7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-79 | D#7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-80 | E7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-81 | F7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-82 | F#7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-83 | G7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-84 | G#7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-85 | A7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-86 | A#7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-87 | B7 | 198:Real E.Tom 198:Real E.Tom Off IFX1 |
| F01-K16-88 | C8 |  |

### F01-GM01 - STANDARD (GM kit 00, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM01-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM01-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM01-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM01-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM01-05 | E1 | 395:Noise-White Off L/R |
| F01-GM01-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM01-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM01-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM01-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM01-10 | A1 | 360:Click Off L/R |
| F01-GM01-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM01-12 | B1 | 016:BD-Tight Off L/R |
| F01-GM01-13 | C2 | 002:BD-Dry 3 Off L/R |
| F01-GM01-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM01-15 | D2 | 048:SD-Full Room Off L/R |
| F01-GM01-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM01-17 | E2 | 048:SD-Full Room Off L/R |
| F01-GM01-18 | F2 | 106:Tom 2-Floor Off L/R |
| F01-GM01-19 | F#2 | 123:H.Hat 1-Open 2 1 L/R |
| F01-GM01-20 | G2 | 106:Tom 2-Floor Off L/R |
| F01-GM01-21 | G#2 | 127:H.Hat 2-Foot 1 L/R |
| F01-GM01-22 | A2 | 105:Tom 2-Lo Off L/R |
| F01-GM01-23 | A#2 | 130:H.Hat 2-Open 2 1 L/R |
| F01-GM01-24 | B2 | 105:Tom 2-Lo Off L/R |
| F01-GM01-25 | C3 | 104:Tom 2-Hi Off L/R |
| F01-GM01-26 | C#3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM01-27 | D3 | 104:Tom 2-Hi Off L/R |
| F01-GM01-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM01-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM01-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM01-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM01-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM01-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM01-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM01-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM01-36 | B3 | 161:Ride-Dance DDD1 Off L/R |
| F01-GM01-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM01-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM01-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM01-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM01-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM01-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM01-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM01-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM01-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM01-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM01-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM01-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM01-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM01-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM01-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM01-52 | D#5 | 278:Claves Off L/R |
| F01-GM01-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM01-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM01-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM01-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM01-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM01-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM01-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM01-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM01-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM01-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM01-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM01-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM01-65 | E6 | 354:Stadium Off L/R |

### F01-GM02 - ROOM (GM kit 01, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM02-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM02-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM02-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM02-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM02-05 | E1 | 395:Noise-White Off L/R |
| F01-GM02-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM02-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM02-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM02-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM02-10 | A1 | 360:Click Off L/R |
| F01-GM02-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM02-12 | B1 | 001:BD-Dry 2 Off L/R |
| F01-GM02-13 | C2 | 002:BD-Dry 3 Off L/R |
| F01-GM02-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM02-15 | D2 | 061:SD-Yowie Off L/R |
| F01-GM02-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM02-17 | E2 | 061:SD-Yowie Off L/R |
| F01-GM02-18 | F2 | 109:Tom 3-Floor Off L/R |
| F01-GM02-19 | F#2 | 123:H.Hat 1-Open 2 1 L/R |
| F01-GM02-20 | G2 | 109:Tom 3-Floor Off L/R |
| F01-GM02-21 | G#2 | 127:H.Hat 2-Foot 1 L/R |
| F01-GM02-22 | A2 | 108:Tom 3-Lo Off L/R |
| F01-GM02-23 | A#2 | 130:H.Hat 2-Open 2 1 L/R |
| F01-GM02-24 | B2 | 108:Tom 3-Lo Off L/R |
| F01-GM02-25 | C3 | 107:Tom 3-Hi Off L/R |
| F01-GM02-26 | C#3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM02-27 | D3 | 107:Tom 3-Hi Off L/R |
| F01-GM02-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM02-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM02-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM02-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM02-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM02-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM02-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM02-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM02-36 | B3 | 161:Ride-Dance DDD1 Off L/R |
| F01-GM02-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM02-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM02-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM02-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM02-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM02-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM02-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM02-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM02-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM02-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM02-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM02-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM02-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM02-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM02-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM02-52 | D#5 | 278:Claves Off L/R |
| F01-GM02-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM02-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM02-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM02-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM02-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM02-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM02-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM02-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM02-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM02-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM02-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM02-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM02-65 | E6 | 354:Stadium Off L/R |

### F01-GM03 - POWER (GM kit 02, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM03-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM03-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM03-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM03-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM03-05 | E1 | 395:Noise-White Off L/R |
| F01-GM03-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM03-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM03-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM03-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM03-10 | A1 | 360:Click Off L/R |
| F01-GM03-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM03-12 | B1 | 015:BD-Gated Off L/R |
| F01-GM03-13 | C2 | 012:BD-Mondo Kill Off L/R |
| F01-GM03-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM03-15 | D2 | 060:SD-Big Rock Off L/R |
| F01-GM03-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM03-17 | E2 | 060:SD-Big Rock Off L/R |
| F01-GM03-18 | F2 | 109:Tom 3-Floor Off L/R |
| F01-GM03-19 | F#2 | 123:H.Hat 1-Open 2 1 L/R |
| F01-GM03-20 | G2 | 109:Tom 3-Floor Off L/R |
| F01-GM03-21 | G#2 | 127:H.Hat 2-Foot 1 L/R |
| F01-GM03-22 | A2 | 108:Tom 3-Lo Off L/R |
| F01-GM03-23 | A#2 | 130:H.Hat 2-Open 2 1 L/R |
| F01-GM03-24 | B2 | 108:Tom 3-Lo Off L/R |
| F01-GM03-25 | C3 | 107:Tom 3-Hi Off L/R |
| F01-GM03-26 | C#3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM03-27 | D3 | 107:Tom 3-Hi Off L/R |
| F01-GM03-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM03-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM03-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM03-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM03-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM03-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM03-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM03-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM03-36 | B3 | 161:Ride-Dance DDD1 Off L/R |
| F01-GM03-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM03-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM03-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM03-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM03-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM03-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM03-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM03-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM03-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM03-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM03-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM03-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM03-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM03-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM03-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM03-52 | D#5 | 278:Claves Off L/R |
| F01-GM03-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM03-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM03-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM03-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM03-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM03-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM03-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM03-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM03-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM03-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM03-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM03-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM03-65 | E6 | 354:Stadium Off L/R |

### F01-GM04 - ELECTRONIC (GM kit 03, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM04-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM04-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM04-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM04-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM04-05 | E1 | 395:Noise-White Off L/R |
| F01-GM04-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM04-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM04-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM04-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM04-10 | A1 | 360:Click Off L/R |
| F01-GM04-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM04-12 | B1 | 022:BD-Hip 1 Off L/R |
| F01-GM04-13 | C2 | 030:BD-Dance99 Off L/R |
| F01-GM04-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM04-15 | D2 | 198:Real E.Tom Off L/R |
| F01-GM04-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM04-17 | E2 | 098:SD-Ambi Hit 1 Off L/R |
| F01-GM04-18 | F2 | 198:Real E.Tom Off L/R |
| F01-GM04-19 | F#2 | 131:H.Hat 2-Sizzle 1 L/R |
| F01-GM04-20 | G2 | 198:Real E.Tom Off L/R |
| F01-GM04-21 | G#2 | 127:H.Hat 2-Foot 1 L/R |
| F01-GM04-22 | A2 | 198:Real E.Tom Off L/R |
| F01-GM04-23 | A#2 | 131:H.Hat 2-Sizzle 1 L/R |
| F01-GM04-24 | B2 | 198:Real E.Tom Off L/R |
| F01-GM04-25 | C3 | 198:Real E.Tom Off L/R |
| F01-GM04-26 | C#3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM04-27 | D3 | 198:Real E.Tom Off L/R |
| F01-GM04-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM04-29 | E3 | 152:Crash Cymbal 1 Off L/R |
| F01-GM04-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM04-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM04-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM04-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM04-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM04-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM04-36 | B3 | 161:Ride-Dance DDD1 Off L/R |
| F01-GM04-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM04-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM04-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM04-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM04-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM04-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM04-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM04-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM04-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM04-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM04-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM04-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM04-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM04-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM04-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM04-52 | D#5 | 278:Claves Off L/R |
| F01-GM04-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM04-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM04-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM04-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM04-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM04-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM04-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM04-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM04-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM04-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM04-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM04-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM04-65 | E6 | 354:Stadium Off L/R |

### F01-GM05 - ANALOG (GM kit 04, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM05-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM05-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM05-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM05-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM05-05 | E1 | 395:Noise-White Off L/R |
| F01-GM05-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM05-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM05-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM05-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM05-10 | A1 | 360:Click Off L/R |
| F01-GM05-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM05-12 | B1 | 174:88-BD 2(R&B) Off L/R |
| F01-GM05-13 | C2 | 176:99-BD 1 Off L/R |
| F01-GM05-14 | C#2 | 185:88-Rimshot Off L/R |
| F01-GM05-15 | D2 | 181:88-SD 1(New) Off L/R |
| F01-GM05-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM05-17 | E2 | 182:88-SD 2(Old) Off L/R |
| F01-GM05-18 | F2 | 192:88-Tom Off L/R |
| F01-GM05-19 | F#2 | 188:88-HHat Open 1 L/R |
| F01-GM05-20 | G2 | 192:88-Tom Off L/R |
| F01-GM05-21 | G#2 | 188:88-HHat Open 1 L/R |
| F01-GM05-22 | A2 | 192:88-Tom Off L/R |
| F01-GM05-23 | A#2 | 188:88-HHat Open 1 L/R |
| F01-GM05-24 | B2 | 192:88-Tom Off L/R |
| F01-GM05-25 | C3 | 192:88-Tom Off L/R |
| F01-GM05-26 | C#3 | 160:Ride-Dance 99 Off L/R |
| F01-GM05-27 | D3 | 192:88-Tom Off L/R |
| F01-GM05-28 | D#3 | 191:88-Crash Off L/R |
| F01-GM05-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM05-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM05-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM05-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM05-33 | G#3 | 195:88-Cowbell Off L/R |
| F01-GM05-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM05-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM05-36 | B3 | 161:Ride-Dance DDD1 Off L/R |
| F01-GM05-37 | C4 | 193:88-Conga Off L/R |
| F01-GM05-38 | C#4 | 193:88-Conga Off L/R |
| F01-GM05-39 | D4 | 192:88-Tom Off L/R |
| F01-GM05-40 | D#4 | 192:88-Tom Off L/R |
| F01-GM05-41 | E4 | 192:88-Tom Off L/R |
| F01-GM05-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM05-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM05-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM05-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM05-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM05-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM05-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM05-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM05-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM05-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM05-52 | D#5 | 278:Claves Off L/R |
| F01-GM05-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM05-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM05-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM05-56 | G5 | 295:Cuica-Hi 4 L/R |
| F01-GM05-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM05-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM05-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM05-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM05-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM05-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM05-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM05-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM05-65 | E6 | 354:Stadium Off L/R |

### F01-GM06 - JAZZ (GM kit 05, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM06-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM06-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM06-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM06-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM06-05 | E1 | 395:Noise-White Off L/R |
| F01-GM06-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM06-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM06-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM06-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM06-10 | A1 | 360:Click Off L/R |
| F01-GM06-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM06-12 | B1 | 003:BD-Dry 4 Off L/R |
| F01-GM06-13 | C2 | 009:BD-Jazz 2 Off L/R |
| F01-GM06-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM06-15 | D2 | 053:SD-Paper Off L/R |
| F01-GM06-16 | D#2 | 186:88-Claps Off L/R |
| F01-GM06-17 | E2 | 050:SD-Jazz Ring Off L/R |
| F01-GM06-18 | F2 | 113:Tom-Jazz Floor Off L/R |
| F01-GM06-19 | F#2 | 118:H.Hat 1-Closed 1 1 L/R |
| F01-GM06-20 | G2 | 113:Tom-Jazz Floor Off L/R |
| F01-GM06-21 | G#2 | 120:H.Hat 1-Foot 1 L/R |
| F01-GM06-22 | A2 | 111:Tom-Jazz Hi Off L/R |
| F01-GM06-23 | A#2 | 122:H.Hat 1-Open 1 1 L/R |
| F01-GM06-24 | B2 | 111:Tom-Jazz Hi Off L/R |
| F01-GM06-25 | C3 | 111:Tom-Jazz Hi Off L/R |
| F01-GM06-26 | C#3 | 152:Crash Cymbal 1 Off L/R |
| F01-GM06-27 | D3 | 111:Tom-Jazz Hi Off L/R |
| F01-GM06-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM06-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM06-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM06-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM06-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM06-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM06-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM06-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM06-36 | B3 | 163:Ride-Edge 2 Off L/R |
| F01-GM06-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM06-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM06-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM06-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM06-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM06-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM06-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM06-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM06-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM06-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM06-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM06-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM06-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM06-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM06-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM06-52 | D#5 | 278:Claves Off L/R |
| F01-GM06-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM06-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM06-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM06-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM06-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM06-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM06-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM06-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM06-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM06-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM06-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM06-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM06-65 | E6 | 355:Applause Off L/R |

### F01-GM07 - BRUSH (GM kit 06, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM07-01 | C1 | 172:SD-Orchestral 7 L/R |
| F01-GM07-02 | C#1 | 171:SD-Orch.Roll 2 7 L/R |
| F01-GM07-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM07-04 | D#1 | 206:Zap 2 Off L/R |
| F01-GM07-05 | E1 | 395:Noise-White Off L/R |
| F01-GM07-06 | F1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM07-07 | F#1 | 209:DJ-Scratch 2 Off L/R |
| F01-GM07-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM07-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM07-10 | A1 | 360:Click Off L/R |
| F01-GM07-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM07-12 | B1 | 003:BD-Dry 4 Off L/R |
| F01-GM07-13 | C2 | 009:BD-Jazz 2 Off L/R |
| F01-GM07-14 | C#2 | 066:Sidestick-Amb Off L/R: |
| F01-GM07-15 | D2 | 056:SD-Brush Tap 1 Off L/R |
| F01-GM07-16 | D#2 | 055:SD-Brush Hit Off L/R |
| F01-GM07-17 | E2 | 058:SD-Brush Swirl Off L/R |
| F01-GM07-18 | F2 | 113:Tom-Jazz Floor Off L/R |
| F01-GM07-19 | F#2 | 118:H.Hat 1-Closed 1 1 L/R |
| F01-GM07-20 | G2 | 113:Tom-Jazz Floor Off L/R |
| F01-GM07-21 | G#2 | 120:H.Hat 1-Foot 1 L/R |
| F01-GM07-22 | A2 | 111:Tom-Jazz Hi Off L/R |
| F01-GM07-23 | A#2 | 122:H.Hat 1-Open 1 1 L/R |
| F01-GM07-24 | B2 | 111:Tom-Jazz Hi Off L/R |
| F01-GM07-25 | C3 | 111:Tom-Jazz Hi Off L/R |
| F01-GM07-26 | C#3 | 152:Crash Cymbal 1 Off L/R |
| F01-GM07-27 | D3 | 111:Tom-Jazz Hi Off L/R |
| F01-GM07-28 | D#3 | 162:Ride-Edge 1 Off L/R |
| F01-GM07-29 | E3 | 155:China Cymbal Off L/R |
| F01-GM07-30 | F3 | 164:Ride-Jazz Off L/R |
| F01-GM07-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM07-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM07-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM07-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM07-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM07-36 | B3 | 163:Ride-Edge 2 Off L/R |
| F01-GM07-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM07-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM07-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM07-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM07-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM07-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM07-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM07-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM07-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM07-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM07-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM07-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM07-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM07-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM07-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM07-52 | D#5 | 278:Claves Off L/R |
| F01-GM07-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM07-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM07-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM07-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM07-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM07-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM07-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM07-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM07-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM07-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM07-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM07-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM07-65 | E6 | 355:Applause Off L/R |

### F01-GM08 - ORCHESTRA (GM kit 07, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM08-01 | C1 | 172:SD-Orchestral Off L/R |
| F01-GM08-02 | C#1 | 171:SD-Orch.Roll 2 Off L/R |
| F01-GM08-03 | D1 | 228:Finger Snap Off L/R |
| F01-GM08-04 | D#1 | 118:H.Hat 1-Closed 1 1 L/R |
| F01-GM08-05 | E1 | 120:H.Hat 1-Foot 1 L/R |
| F01-GM08-06 | F1 | 122:H.Hat 1-Open 1 1 L/R |
| F01-GM08-07 | F#1 | 162:Ride-Edge 1 Off L/R |
| F01-GM08-08 | G1 | 227:Drumstick Hit Off L/R |
| F01-GM08-09 | G#1 | 205:Zap 1 Off L/R |
| F01-GM08-10 | A1 | 360:Click Off L/R |
| F01-GM08-11 | A#1 | 292:Triangle-Open Off L/R |
| F01-GM08-12 | B1 | 007:BD-Soft Room Off L/R |
| F01-GM08-13 | C2 | 166:BD-Orchestral Off L/R |
| F01-GM08-14 | C#2 | 066:Sidestick-Amb Off L/R |
| F01-GM08-15 | D2 | 172:SD-Orchestral Off L/R |
| F01-GM08-16 | D#2 | 252:Castanet-Single Off L/R |
| F01-GM08-17 | E2 | 172:SD-Orchestral Off L/R |
| F01-GM08-18 | F2 | 169:Timpani Off L/R |
| F01-GM08-19 | F#2 | 169:Timpani Off L/R |
| F01-GM08-20 | G2 | 169:Timpani Off L/R |
| F01-GM08-21 | G#2 | 169:Timpani Off L/R |
| F01-GM08-22 | A2 | 169:Timpani Off L/R |
| F01-GM08-23 | A#2 | 169:Timpani Off L/R |
| F01-GM08-24 | B2 | 169:Timpani Off L/R |
| F01-GM08-25 | C3 | 169:Timpani Off L/R |
| F01-GM08-26 | C#3 | 169:Timpani Off L/R |
| F01-GM08-27 | D3 | 169:Timpani Off L/R |
| F01-GM08-28 | D#3 | 169:Timpani Off L/R |
| F01-GM08-29 | E3 | 169:Timpani Off L/R |
| F01-GM08-30 | F3 | 169:Timpani Off L/R |
| F01-GM08-31 | F#3 | 291:Tambourine-Acc.2 Off L/R |
| F01-GM08-32 | G3 | 155:China Cymbal Off L/R |
| F01-GM08-33 | G#3 | 306:Cowbell 2 Off L/R |
| F01-GM08-34 | A3 | 153:Crash Cymbal 2 Off L/R |
| F01-GM08-35 | A#3 | 277:Vibraslap Off L/R |
| F01-GM08-36 | B3 | 168:Orch Cym.-Open Off L/R |
| F01-GM08-37 | C4 | 245:Bongo Hi-Open Off L/R |
| F01-GM08-38 | C#4 | 242:Bongo Lo-Open Off L/R |
| F01-GM08-39 | D4 | 237:Conga Hi-MtSlap Off L/R |
| F01-GM08-40 | D#4 | 235:Conga Hi-Open Off L/R |
| F01-GM08-41 | E4 | 232:Conga Lo-Open Off L/R |
| F01-GM08-42 | F4 | 286:Timbale Hi-Rim 2 Off L/R |
| F01-GM08-43 | F#4 | 281:Timbale Lo-Open Off L/R |
| F01-GM08-44 | G4 | 304:Agogo Bell Off L/R |
| F01-GM08-45 | G#4 | 304:Agogo Bell Off L/R |
| F01-GM08-46 | A4 | 299:Cabasa-Up Off L/R |
| F01-GM08-47 | A#4 | 258:Maracas-Push Off L/R |
| F01-GM08-48 | B4 | 316:Samba Whistle 2 L/R |
| F01-GM08-49 | C5 | 316:Samba Whistle 2 L/R |
| F01-GM08-50 | C#5 | 256:Guiro-Short 3 L/R |
| F01-GM08-51 | D5 | 255:Guiro-Long 3 L/R |
| F01-GM08-52 | D#5 | 278:Claves Off L/R |
| F01-GM08-53 | E5 | 279:Woodblock 1 Off L/R |
| F01-GM08-54 | F5 | 279:Woodblock 1 Off L/R |
| F01-GM08-55 | F#5 | 295:Cuica-Hi 4 L/R |
| F01-GM08-56 | G5 | 296:Cuica-Lo 4 L/R |
| F01-GM08-57 | G#5 | 293:Triangle-Mute 5 L/R |
| F01-GM08-58 | A5 | 292:Triangle-Open 5 L/R |
| F01-GM08-59 | A#5 | 300:Cabasa-Down Off L/R |
| F01-GM08-60 | B5 | 309:Sleigh Bell Off L/R |
| F01-GM08-61 | C6 | 312:Marc Tree Off L/R |
| F01-GM08-62 | C#6 | 252:Castanet-Single Off L/R |
| F01-GM08-63 | D6 | 282:Timbale Lo-Mute 6 L/R |
| F01-GM08-64 | D#6 | 281:Timbale Lo-Open 6 L/R |
| F01-GM08-65 | E6 | 355:Applause Off L/R |

### F01-GM09 - SFX (GM kit 08, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F01-GM09-01 | C1 |  |
| F01-GM09-02 | C#1 |  |
| F01-GM09-03 | D1 |  |
| F01-GM09-04 | D#1 |  |
| F01-GM09-05 | E1 |  |
| F01-GM09-06 | F1 |  |
| F01-GM09-07 | F#1 |  |
| F01-GM09-08 | G1 |  |
| F01-GM09-09 | G#1 |  |
| F01-GM09-10 | A1 |  |
| F01-GM09-11 | A#1 |  |
| F01-GM09-12 | B1 |  |
| F01-GM09-13 | C2 |  |
| F01-GM09-14 | C#2 |  |
| F01-GM09-15 | D2 |  |
| F01-GM09-16 | D#2 | 206:Zap 2 Off L/R |
| F01-GM09-17 | E2 | 370:GunShot Off L/R |
| F01-GM09-18 | F2 | 208:DJ-Scratch 1 7 L/R |
| F01-GM09-19 | F#2 | 209:DJ-Scratch 2 7 L/R |
| F01-GM09-20 | G2 | 227:Drumstick Hit Off L/R |
| F01-GM09-21 | G#2 | 228:Finger Snap Off L/R |
| F01-GM09-22 | A2 | 360:Click Off L/R |
| F01-GM09-23 | A#2 | 397:Tubular Off L/R |
| F01-GM09-24 | B2 | 405:Fret Noise Off L/R |
| F01-GM09-25 | C3 | 400:GtCutNois1 Off L/R |
| F01-GM09-26 | C#3 | 401:GtCutNois2 Off L/R |
| F01-GM09-27 | D3 | 402:StringSlap Off L/R |
| F01-GM09-28 | D#3 | 228:Finger Snap Off L/R |
| F01-GM09-29 | E3 | 357:Laughing Off L/R |
| F01-GM09-30 | F3 | 356:Scream Off L/R |
| F01-GM09-31 | F#3 | 368:Punch Off L/R |
| F01-GM09-32 | G3 | 367:Hart Beat Off L/R |
| F01-GM09-33 | G#3 | 359:Footstep 2 Off L/R |
| F01-GM09-34 | A3 | 358:Footstep 1 Off L/R |
| F01-GM09-35 | A#3 | 355:Applause Off L/R |
| F01-GM09-36 | B3 | 372:Door Creak Off L/R |
| F01-GM09-37 | C4 | 373:Door Slam Off L/R |
| F01-GM09-38 | C#4 | 209:DJ-Scratch 2 Off L/R |
| F01-GM09-39 | D4 | 312:Marc Tree Off L/R |
| F01-GM09-40 | D#4 | 374:Car Engine Off L/R |
| F01-GM09-41 | E4 | 375:Car Stop Off L/R |
| F01-GM09-42 | F4 | 376:Car Pass Off L/R |
| F01-GM09-43 | F#4 | 377:Car Crash Off L/R |
| F01-GM09-44 | G4 | 366:Crickets 2 Off L/R |
| F01-GM09-45 | G#4 | 378:Train Off L/R |
| F01-GM09-46 | A4 | 395:Noise-White Off L/R |
| F01-GM09-47 | A#4 | 379:Helicopter Off L/R |
| F01-GM09-48 | B4 | 391:Swish Terra Off L/R |
| F01-GM09-49 | C5 | 380:Gun Shot 2 Off L/R |
| F01-GM09-50 | C#5 | 381:MachineGun Off L/R |
| F01-GM09-51 | D5 | 382:Laser Gun Off L/R |
| F01-GM09-52 | D#5 | 383:Explosion Off L/R |
| F01-GM09-53 | E5 | 363:Dog Off L/R |
| F01-GM09-54 | F5 | 364:Gallop Off L/R |
| F01-GM09-55 | F#5 | 361:Bird 1 Off L/R |
| F01-GM09-56 | G5 | 371:Rainstick Off L/R |
| F01-GM09-57 | G#5 | 384:Thunder Off L/R |
| F01-GM09-58 | A5 | 385:Wind Off L/R |
| F01-GM09-59 | A#5 | 395:Noise-White Off L/R |
| F01-GM09-60 | B5 | 386:Stream Off L/R |
| F01-GM09-61 | C6 | 387:Bubble Off L/R |
| F01-GM09-62 | C#6 |  |
| F01-GM09-63 | D6 |  |
| F01-GM09-64 | D#6 |  |
| F01-GM09-65 | E6 |  |

