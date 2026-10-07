# FAMILY 02 - drum kits, drum sounds and pad mapping (internal reference)

Reference machine (internal name, never shipped): **Triton Extreme**.
Shipped label: **Family 02**.


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
| D1 | TRITON Extreme Voice Name List (Korg, file TRITON Extreme Voice Name List, 40 pages) | official Korg download page, product TRITON Extreme | https://www.korg.com/us/support/download/product/1/215/ |
| D2 | TRITON Extreme Parameter Guide (Korg, file TNEX_ParameterGuide_E2, 354 pages, created 2004-02-23) | official Korg download page, product TRITON Extreme, document id 2423 | https://www.korg.com/us/support/download/manual/1/215/2423/ |

## Counts as documented

- Factory drum kits: **50**
- GM drum kits: **9**
- Distinct drumsample numbers named in the PCM list: **417** (1233 documented rows; the document re-lists samples in more than one group)
- Drum sounds: the document does not publish a separate drum-sound list for this machine beyond the drumsample names in the PCM list, which are listed in `FAMILY-02-PROGRAMS.md`.

The document prints each kit as a per-key table: the key label, the high drumsample, the low drumsample, the exclusive group and the bus select. Each cell below is the document's own text for that key, in that order. Where a key row is empty the kit does not assign that key.

## Factory drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F02-K01 | 000 | Standard Kit 1 | 90 | D1 p19 |
| F02-K02 | 001 | Standard Kit 2 | 90 | D1 p19 |
| F02-K03 | 002 | Processed Kit | 90 | D1 p19 |
| F02-K04 | 003 | Jazz/Brush Kit | 89 | D1 p20 |
| F02-K05 | 004 | House Kit | 89 | D1 p20 |
| F02-K06 | 005 | Tricky Kit | 89 | D1 p20 |
| F02-K07 | 006 | Drum'n'Bass Kit | 90 | D1 p21 |
| F02-K08 | 007 | Hip Hop Kit | 90 | D1 p21 |
| F02-K09 | 008 | Psycho Kit | 90 | D1 p21 |
| F02-K10 | 009 | Percussion Kit | 89 | D1 p22 |
| F02-K11 | 010 | Orchestra&Ethnic | 89 | D1 p22 |
| F02-K12 | 011 | Cymbals Catalog | 89 | D1 p22 |
| F02-K13 | 012 | BD&SD Catalog 1 | 90 | D1 p23 |
| F02-K14 | 013 | BD&SD Catalog 2 | 90 | D1 p23 |
| F02-K15 | 014 | 01/W Total Kit | 90 | D1 p23 |
| F02-K16 | 015 | 01/W Producer's | 89 | D1 p24 |
| F02-K17 | 016 | Hard Core Kit | 89 | D1 p24 |
| F02-K18 | 017 | ModernRockKit | 89 | D1 p24 |
| F02-K19 | 018 | Electro Kit 2 | 90 | D1 p25 |
| F02-K20 | 019 | Under Hard Kit | 90 | D1 p25 |
| F02-K21 | 020 | Nu Pop Kit | 90 | D1 p25 |
| F02-K22 | 021 | Voice Drum Set | 89 | D1 p26 |
| F02-K23 | 022 | Junkyard Kit | 89 | D1 p26 |
| F02-K24 | 023 | Voice Catalog | 89 | D1 p26 |
| F02-K25 | 024 | Beat Box Kit | 90 | D1 p27 |
| F02-K26 | 025 | Lounge Kit | 90 | D1 p27 |
| F02-K27 | 026 | TekNo Kit 1 | 90 | D1 p27 |
| F02-K28 | 027 | TekNo Kit 2 | 89 | D1 p28 |
| F02-K29 | 028 | Trance Kit 1 | 89 | D1 p28 |
| F02-K30 | 029 | Trance Kit 2 | 89 | D1 p28 |
| F02-K31 | 030 | Hard Kit 1 | 90 | D1 p29 |
| F02-K32 | 031 | Hard Kit 2 | 90 | D1 p29 |
| F02-K33 | 032 | Pumpin' Kit | 90 | D1 p29 |
| F02-K34 | 033 | PhatBoy Kit | 89 | D1 p30 |
| F02-K35 | 034 | Nu Trix Kit | 89 | D1 p30 |
| F02-K36 | 035 | Xtreme Kit | 89 | D1 p30 |
| F02-K37 | 036 | Grand Slam Kit | 90 | D1 p31 |
| F02-K38 | 037 | Techno Perc | 90 | D1 p31 |
| F02-K39 | 038 | EXB Orch. Kit 1 | 90 | D1 p31 |
| F02-K40 | 039 | EXB Orch. Kit 2 | 87 | D1 p32 |
| F02-K41 | 040 | ROM Orch. Kit | 87 | D1 p32 |
| F02-K42 | 041 | Harp Kit(maj) | 87 | D1 p32 |
| F02-K43 | 042 | Harp Kit(min) | 90 | D1 p33 |
| F02-K44 | 043 | Major Hit Kit | 90 | D1 p33 |
| F02-K45 | 044 | Minor Hit Kit | 90 | D1 p33 |
| F02-K46 | 045 | EXB Cymbals set | 89 | D1 p34 |
| F02-K47 | 128 | Trance/GarageKit | 89 | D1 p34 |
| F02-K48 | 129 | Big Trouble Kit | 89 | D1 p34 |
| F02-K49 | 130 | Electro Kit 1 | 90 | D1 p35 |
| F02-K50 | 131 | Wacky FX Kit | 90 | D1 p35 |

## GM drum kits

| Shipped label | Kit number | Documented kit name | Keys documented | Source |
|---|---|---|---|---|
| F02-GM01 | 144 | STANDARD | 65 | D1 p36 |
| F02-GM02 | 145 | ROOM | 65 | D1 p36 |
| F02-GM03 | 146 | POWER | 65 | D1 p36 |
| F02-GM04 | 147 | ELECTRONIC | 65 | D1 p36 |
| F02-GM05 | 148 | ANALOG | 65 | D1 p36 |
| F02-GM06 | 149 | JAZZ | 65 | D1 p37 |
| F02-GM07 | 150 | BRUSH | 65 | D1 p37 |
| F02-GM08 | 151 | ORCHESTRA | 65 | D1 p37 |
| F02-GM09 | 152 | SFX | 65 | D1 p37 |

### F02-K01 - Standard Kit 1 (kit 000, D1 p19)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K01-01 | A0 |  |
| F02-K01-02 | A#0 |  |
| F02-K01-03 | B0 |  |
| F02-K01-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K01-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K01-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K01-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K01-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K01-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K01-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K01-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K01-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K01-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K01-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K01-15 | B1 | ROM, 73 SD-Reverse |
| F02-K01-16 | C2 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K01-17 | C#2 | ROM, 3 BD-Dry 4 ROM, 3 BD-Dry 4 |
| F02-K01-18 | D2 | ROM, 6 BD-Soft ROM, 6 BD-Soft |
| F02-K01-19 | D#2 | ROM, 7 BD-Soft Room ROM, 7 BD-Soft Room |
| F02-K01-20 | E2 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K01-21 | F2 | ROM, 48 SD-Full Room ROM, 48 SD-Full Room |
| F02-K01-22 | F#2 | ROM, 54 SD-Classic Room ROM, 54 SD-Classic Room |
| F02-K01-23 | G2 | ROM, 51 SD-Amb.Piccolo 1 ROM, 51 SD-Amb.Piccolo 1 |
| F02-K01-24 | G#2 | ROM, 53 SD-Paper ROM, 53 SD-Paper |
| F02-K01-25 | A2 | ROM, 42 SD-Dry 3 ROM, 42 SD-Dry 3 |
| F02-K01-26 | A#2 | ROM, 40 SD-Dry 1 ROM, 40 SD-Dry 1 |
| F02-K01-27 | B2 | ROM, 2 BD-Dry 3 ROM, 2 BD-Dry 3 |
| F02-K01-28 | C3 | ROM, 1 BD-Dry 2 ROM, 1 BD-Dry 2 |
| F02-K01-29 | C#3 | ROM, 65 Sidestick-Dry ROM, 65 Sidestick-Dry |
| F02-K01-30 | D3 | ROM, 52 SD-Amb.Piccolo 2 ROM, 52 SD-Amb.Piccolo 2 |
| F02-K01-31 | D#3 | ROM, 229 Hand Claps ROM, 229 Hand Claps |
| F02-K01-32 | E3 | ROM, 60 SD-Big Rock ROM, 60 SD-Big Rock |
| F02-K01-33 | F3 | ROM, 106 Tom 2-Floor ROM, 106 Tom 2-Floor |
| F02-K01-34 | F#3 | ROM, 119 H.Hat 1-Closed 2 ROM, 119 H.Hat 1-Closed 2 |
| F02-K01-35 | G3 | ROM, 106 Tom 2-Floor ROM, 106 Tom 2-Floor |
| F02-K01-36 | G#3 | ROM, 121 H.Hat 1-FootOpen ROM, 120 H.Hat 1-Foot |
| F02-K01-37 | A3 | ROM, 105 Tom 2-Lo ROM, 105 Tom 2-Lo |
| F02-K01-38 | A#3 | ROM, 122 H.Hat 1-Open 1 ROM, 123 H.Hat 1-Open 2 |
| F02-K01-39 | B3 | ROM, 105 Tom 2-Lo ROM, 105 Tom 2-Lo |
| F02-K01-40 | C4 | ROM, 104 Tom 2-Hi ROM, 104 Tom 2-Hi |
| F02-K01-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K01-42 | D4 | ROM, 104 Tom 2-Hi ROM, 104 Tom 2-Hi |
| F02-K01-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K01-44 | E4 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K01-45 | F4 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K01-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K01-47 | G4 | ROM, 156 Splash Cymbal ROM, 156 Splash Cymbal |
| F02-K01-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K01-49 | A4 | ROM, 152 Crash Cymbal 1 |
| F02-K01-50 | A#4 | ROM, 277 Vibraslap |
| F02-K01-51 | B4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K01-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K01-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K01-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K01-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K01-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K01-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K01-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K01-59 | G5 | ROM, 304 Agogo Bell |
| F02-K01-60 | G#5 | ROM, 304 Agogo Bell |
| F02-K01-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K01-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K01-63 | B5 | ROM, 316 Samba Whistle |
| F02-K01-64 | C6 | ROM, 316 Samba Whistle |
| F02-K01-65 | C#6 | ROM, 256 Guiro-Short ROM, 257 Guiro-Tap |
| F02-K01-66 | D6 | ROM, 255 Guiro-Long |
| F02-K01-67 | D#6 | ROM, 278 Claves |
| F02-K01-68 | E6 | ROM, 279 Woodblock 1 |
| F02-K01-69 | F6 | ROM, 279 Woodblock 1 |
| F02-K01-70 | F#6 | ROM, 295 Cuica-Hi |
| F02-K01-71 | G6 | ROM, 296 Cuica-Lo |
| F02-K01-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K01-73 | A6 | ROM, 292 Triangle-Open |
| F02-K01-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K01-75 | B6 | ROM, 312 Marc Tree |
| F02-K01-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K01-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K01-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K01-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K01-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K01-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K01-82 | F#7 | ROM, 186 88-Claps |
| F02-K01-83 | G7 | ROM, 196 88-Maraca |
| F02-K01-84 | G#7 | ROM, 171 SD-Orch.Roll 2 |
| F02-K01-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K01-86 | A#7 | ROM, 314 Bell Tree |
| F02-K01-87 | B7 | ROM, 354 Stadium |
| F02-K01-88 | C8 | ROM, 355 Applause |
| F02-K01-89 | C#8 |  |
| F02-K01-90 | D8 |  |

### F02-K02 - Standard Kit 2 (kit 001, D1 p19)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K02-01 | A0 |  |
| F02-K02-02 | A#0 |  |
| F02-K02-03 | B0 |  |
| F02-K02-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K02-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K02-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K02-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K02-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K02-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K02-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K02-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K02-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K02-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K02-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K02-15 | B1 | ROM, 73 SD-Reverse |
| F02-K02-16 | C2 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K02-17 | C#2 | ROM, 15 BD-Gated ROM, 15 BD-Gated |
| F02-K02-18 | D2 | ROM, 36 BD-Chili ROM, 36 BD-Chili |
| F02-K02-19 | D#2 | ROM, 25 BD-Hip 4 ROM, 25 BD-Hip 4 |
| F02-K02-20 | E2 | ROM, 37 BD-PhatJaz ROM, 37 BD-PhatJaz |
| F02-K02-21 | F2 | ROM, 93 SD-Chili ROM, 50 SD-Jazz Ring |
| F02-K02-22 | F#2 | ROM, 99 SD-Ambi Hit 2 ROM, 54 SD-Classic Room |
| F02-K02-23 | G2 | ROM, 92 SD-Brasser ROM, 53 SD-Paper |
| F02-K02-24 | G#2 | ROM, 85 SD-Vintage 4 ROM, 56 SD-Brush Tap 1 |
| F02-K02-25 | A2 | ROM, 78 SD-Hip 5 ROM, 78 SD-Hip 5 |
| F02-K02-26 | A#2 | ROM, 44 SD-GhostSet roll ROM, 44 SD-GhostSet roll |
| F02-K02-27 | B2 | ROM, 5 BD-Dry 6 ROM, 5 BD-Dry 6 |
| F02-K02-28 | C3 | ROM, 4 BD-Dry 5 ROM, 4 BD-Dry 5 |
| F02-K02-29 | C#3 | ROM, 66 Sidestick-Amb ROM, 66 Sidestick-Amb |
| F02-K02-30 | D3 | ROM, 45 SD-GhostSet f ROM, 47 SD-GhostSet p2 |
| F02-K02-31 | D#3 | ROM, 229 Hand Claps ROM, 229 Hand Claps |
| F02-K02-32 | E3 | ROM, 94 SD-Klanger ROM, 46 SD-GhostSet p1 |
| F02-K02-33 | F3 | ROM, 103 Tom 1-Floor ROM, 103 Tom 1-Floor |
| F02-K02-34 | F#3 | ROM, 126 H.Hat 2-Closed 2 ROM, 125 H.Hat 2-Closed 1 |
| F02-K02-35 | G3 | ROM, 103 Tom 1-Floor ROM, 103 Tom 1-Floor |
| F02-K02-36 | G#3 | ROM, 128 H.Hat 2-FootOpen ROM, 127 H.Hat 2-Foot |
| F02-K02-37 | A3 | ROM, 102 Tom 1-Lo ROM, 102 Tom 1-Lo |
| F02-K02-38 | A#3 | ROM, 131 H.Hat 2-Sizzle ROM, 129 H.Hat 2-Open 1 |
| F02-K02-39 | B3 | ROM, 102 Tom 1-Lo ROM, 102 Tom 1-Lo |
| F02-K02-40 | C4 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K02-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K02-42 | D4 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K02-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K02-44 | E4 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K02-45 | F4 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K02-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K02-47 | G4 | ROM, 156 Splash Cymbal ROM, 156 Splash Cymbal |
| F02-K02-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K02-49 | A4 | ROM, 152 Crash Cymbal 1 |
| F02-K02-50 | A#4 | ROM, 277 Vibraslap |
| F02-K02-51 | B4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K02-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K02-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K02-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K02-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K02-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K02-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K02-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K02-59 | G5 | ROM, 304 Agogo Bell |
| F02-K02-60 | G#5 | ROM, 304 Agogo Bell |
| F02-K02-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K02-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K02-63 | B5 | ROM, 316 Samba Whistle |
| F02-K02-64 | C6 | ROM, 316 Samba Whistle |
| F02-K02-65 | C#6 | ROM, 256 Guiro-Short ROM, 257 Guiro-Tap |
| F02-K02-66 | D6 | ROM, 255 Guiro-Long |
| F02-K02-67 | D#6 | ROM, 278 Claves |
| F02-K02-68 | E6 | ROM, 279 Woodblock 1 |
| F02-K02-69 | F6 | ROM, 279 Woodblock 1 |
| F02-K02-70 | F#6 | ROM, 295 Cuica-Hi |
| F02-K02-71 | G6 | ROM, 296 Cuica-Lo |
| F02-K02-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K02-73 | A6 | ROM, 292 Triangle-Open |
| F02-K02-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K02-75 | B6 | ROM, 312 Marc Tree |
| F02-K02-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K02-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K02-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K02-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K02-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K02-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K02-82 | F#7 | ROM, 186 88-Claps |
| F02-K02-83 | G7 | ROM, 196 88-Maraca |
| F02-K02-84 | G#7 | ROM, 171 SD-Orch.Roll 2 |
| F02-K02-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K02-86 | A#7 | ROM, 314 Bell Tree |
| F02-K02-87 | B7 | ROM, 354 Stadium |
| F02-K02-88 | C8 | ROM, 355 Applause |
| F02-K02-89 | C#8 |  |
| F02-K02-90 | D8 |  |

### F02-K03 - Processed Kit (kit 002, D1 p19)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K03-01 | A0 |  |
| F02-K03-02 | A#0 |  |
| F02-K03-03 | B0 |  |
| F02-K03-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K03-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K03-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K03-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K03-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K03-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K03-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K03-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K03-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K03-13 | A1 | ROM, 227 Drumstick Hit ROM, 227 Drumstick Hit |
| F02-K03-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K03-15 | B1 | ROM, 73 SD-Reverse |
| F02-K03-16 | C2 | ROM, 16 BD-Tight ROM, 16 BD-Tight |
| F02-K03-17 | C#2 | ROM, 17 BD-Squash ROM, 17 BD-Squash |
| F02-K03-18 | D2 | ROM, 15 BD-Gated ROM, 15 BD-Gated |
| F02-K03-19 | D#2 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K03-20 | E2 | ROM, 11 BD-Woofer ROM, 11 BD-Woofer |
| F02-K03-21 | F2 | ROM, 48 SD-Full Room ROM, 48 SD-Full Room |
| F02-K03-22 | F#2 | ROM, 64 SD-Cracker Room ROM, 64 SD-Cracker Room |
| F02-K03-23 | G2 | ROM, 52 SD-Amb.Piccolo 2 ROM, 52 SD-Amb.Piccolo 2 |
| F02-K03-24 | G#2 | ROM, 62 SD-Processed 1 ROM, 62 SD-Processed 1 |
| F02-K03-25 | A2 | ROM, 61 SD-Yowie ROM, 61 SD-Yowie |
| F02-K03-26 | A#2 | ROM, 59 SD-Atomic ROM, 59 SD-Atomic |
| F02-K03-27 | B2 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K03-28 | C3 | ROM, 12 BD-Mondo Kill ROM, 12 BD-Mondo Kill |
| F02-K03-29 | C#3 | ROM, 66 Sidestick-Amb ROM, 66 Sidestick-Amb |
| F02-K03-30 | D3 | ROM, 60 SD-Big Rock ROM, 60 SD-Big Rock |
| F02-K03-31 | D#3 | ROM, 229 Hand Claps ROM, 229 Hand Claps |
| F02-K03-32 | E3 | ROM, 63 SD-Processed 2 ROM, 63 SD-Processed 2 |
| F02-K03-33 | F3 | ROM, 109 Tom 3-Floor ROM, 109 Tom 3-Floor |
| F02-K03-34 | F#3 | ROM, 126 H.Hat 2-Closed 2 ROM, 125 H.Hat 2-Closed 1 |
| F02-K03-35 | G3 | ROM, 108 Tom 3-Lo ROM, 108 Tom 3-Lo |
| F02-K03-36 | G#3 | ROM, 128 H.Hat 2-FootOpen ROM, 127 H.Hat 2-Foot |
| F02-K03-37 | A3 | ROM, 108 Tom 3-Lo ROM, 108 Tom 3-Lo |
| F02-K03-38 | A#3 | ROM, 129 H.Hat 2-Open 1 ROM, 130 H.Hat 2-Open 2 |
| F02-K03-39 | B3 | ROM, 108 Tom 3-Lo ROM, 108 Tom 3-Lo |
| F02-K03-40 | C4 | ROM, 107 Tom 3-Hi ROM, 107 Tom 3-Hi |
| F02-K03-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K03-42 | D4 | ROM, 107 Tom 3-Hi ROM, 107 Tom 3-Hi |
| F02-K03-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K03-44 | E4 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K03-45 | F4 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K03-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K03-47 | G4 | ROM, 156 Splash Cymbal ROM, 156 Splash Cymbal |
| F02-K03-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K03-49 | A4 | ROM, 152 Crash Cymbal 1 |
| F02-K03-50 | A#4 | ROM, 277 Vibraslap |
| F02-K03-51 | B4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K03-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K03-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K03-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K03-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K03-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K03-57 | F5 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-58 | F#5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K03-59 | G5 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-60 | G#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K03-61 | A5 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K03-63 | B5 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-64 | C6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-65 | C#6 | ROM, 301 Cabasa-Tap ROM, 299 Cabasa-Up |
| F02-K03-66 | D6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K03-67 | D#6 | ROM, 278 Claves ROM, 278 Claves |
| F02-K03-68 | E6 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K03-69 | F6 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K03-70 | F#6 | ROM, 295 Cuica-Hi |
| F02-K03-71 | G6 | ROM, 296 Cuica-Lo ROM, 296 Cuica-Lo |
| F02-K03-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K03-73 | A6 | ROM, 292 Triangle-Open ROM, 292 Triangle-Open |
| F02-K03-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K03-75 | B6 | ROM, 312 Marc Tree |
| F02-K03-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K03-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K03-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K03-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K03-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K03-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K03-82 | F#7 | ROM, 186 88-Claps |
| F02-K03-83 | G7 | ROM, 196 88-Maraca |
| F02-K03-84 | G#7 | ROM, 171 SD-Orch.Roll 2 ROM, 170 SD-Orch.Roll 1 |
| F02-K03-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K03-86 | A#7 | ROM, 314 Bell Tree |
| F02-K03-87 | B7 | ROM, 354 Stadium |
| F02-K03-88 | C8 | ROM, 355 Applause |
| F02-K03-89 | C#8 | 19 |
| F02-K03-90 | D8 |  |

### F02-K04 - Jazz/Brush Kit (kit 003, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K04-01 | A0 |  |
| F02-K04-02 | A#0 |  |
| F02-K04-03 | B0 |  |
| F02-K04-04 | C1 | ROM, 103 Tom 1-Floor ROM, 103 Tom 1-Floor |
| F02-K04-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K04-06 | D1 | ROM, 102 Tom 1-Lo ROM, 102 Tom 1-Lo |
| F02-K04-07 | D#1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K04-08 | E1 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K04-09 | F1 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K04-10 | F#1 | ROM, 307 Chacha Bell |
| F02-K04-11 | G1 | ROM, 228 Finger Snap ROM, 228 Finger Snap |
| F02-K04-12 | G#1 | ROM, 229 Hand Claps ROM, 229 Hand Claps |
| F02-K04-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K04-14 | A#1 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K04-15 | B1 | ROM, 2 BD-Dry 3 ROM, 2 BD-Dry 3 |
| F02-K04-16 | C2 | ROM, 11 BD-Woofer ROM, 11 BD-Woofer |
| F02-K04-17 | C#2 | ROM, 3 BD-Dry 4 ROM, 3 BD-Dry 4 |
| F02-K04-18 | D2 | ROM, 6 BD-Soft ROM, 6 BD-Soft |
| F02-K04-19 | D#2 | ROM, 8 BD-Jazz 1 ROM, 8 BD-Jazz 1 |
| F02-K04-20 | E2 | ROM, 9 BD-Jazz 2 ROM, 9 BD-Jazz 2 |
| F02-K04-21 | F2 | ROM, 53 SD-Paper ROM, 53 SD-Paper |
| F02-K04-22 | F#2 | ROM, 41 SD-Dry 2 ROM, 41 SD-Dry 2 |
| F02-K04-23 | G2 | ROM, 40 SD-Dry 1 ROM, 40 SD-Dry 1 |
| F02-K04-24 | G#2 | ROM, 51 SD-Amb.Piccolo 1 ROM, 51 SD-Amb.Piccolo 1 |
| F02-K04-25 | A2 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K04-26 | A#2 | ROM, 55 SD-Brush Hit |
| F02-K04-27 | B2 | ROM, 9 BD-Jazz 2 ROM, 9 BD-Jazz 2 |
| F02-K04-28 | C3 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K04-29 | C#3 | ROM, 65 Sidestick-Dry ROM, 65 Sidestick-Dry |
| F02-K04-30 | D3 | ROM, 57 SD-Brush Tap 2 |
| F02-K04-31 | D#3 | ROM, 56 SD-Brush Tap 1 |
| F02-K04-32 | E3 | ROM, 58 SD-Brush Swirl |
| F02-K04-33 | F3 | ROM, 115 Tom-Brush Floor ROM, 115 Tom-Brush Floor |
| F02-K04-34 | F#3 | ROM, 119 H.Hat 1-Closed 2 ROM, 118 H.Hat 1-Closed 1 |
| F02-K04-35 | G3 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K04-36 | G#3 | ROM, 120 H.Hat 1-Foot ROM, 120 H.Hat 1-Foot |
| F02-K04-37 | A3 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K04-38 | A#3 | ROM, 122 H.Hat 1-Open 1 ROM, 123 H.Hat 1-Open 2 |
| F02-K04-39 | B3 | ROM, 113 Tom-Jazz Floor ROM, 113 Tom-Jazz Floor |
| F02-K04-40 | C4 | ROM, 112 Tom-Jazz Hi Rim ROM, 111 Tom-Jazz Hi |
| F02-K04-41 | C#4 | ROM, 152 Crash Cymbal 1 |
| F02-K04-42 | D4 | ROM, 112 Tom-Jazz Hi Rim ROM, 111 Tom-Jazz Hi |
| F02-K04-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K04-44 | E4 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K04-45 | F4 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K04-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K04-47 | G4 | ROM, 156 Splash Cymbal ROM, 156 Splash Cymbal |
| F02-K04-48 | G#4 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K04-49 | A4 | ROM, 154 Crash Cymbal 3 ROM, 154 Crash Cymbal 3 |
| F02-K04-50 | A#4 | ROM, 277 Vibraslap |
| F02-K04-51 | B4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K04-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K04-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K04-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K04-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K04-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K04-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K04-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K04-59 | G5 | ROM, 304 Agogo Bell |
| F02-K04-60 | G#5 | ROM, 304 Agogo Bell |
| F02-K04-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K04-62 | A#5 | ROM, 258 Maracas-Push |
| F02-K04-63 | B5 | ROM, 316 Samba Whistle |
| F02-K04-64 | C6 | ROM, 316 Samba Whistle |
| F02-K04-65 | C#6 | ROM, 256 Guiro-Short ROM, 257 Guiro-Tap |
| F02-K04-66 | D6 | ROM, 255 Guiro-Long |
| F02-K04-67 | D#6 | ROM, 278 Claves |
| F02-K04-68 | E6 | ROM, 279 Woodblock 1 |
| F02-K04-69 | F6 | ROM, 279 Woodblock 1 |
| F02-K04-70 | F#6 | ROM, 295 Cuica-Hi |
| F02-K04-71 | G6 | ROM, 296 Cuica-Lo |
| F02-K04-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K04-73 | A6 | ROM, 292 Triangle-Open |
| F02-K04-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K04-75 | B6 | ROM, 312 Marc Tree |
| F02-K04-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K04-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K04-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K04-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K04-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K04-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K04-82 | F#7 | ROM, 186 88-Claps |
| F02-K04-83 | G7 | ROM, 196 88-Maraca |
| F02-K04-84 | G#7 | ROM, 171 SD-Orch.Roll 2 ROM, 170 SD-Orch.Roll 1 |
| F02-K04-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K04-86 | A#7 | ROM, 314 Bell Tree |
| F02-K04-87 | B7 | ROM, 354 Stadium |
| F02-K04-88 | C8 | ROM, 355 Applause |
| F02-K04-89 | D8 |  |

### F02-K05 - House Kit (kit 004, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K05-01 | A0 |  |
| F02-K05-02 | A#0 |  |
| F02-K05-03 | B0 |  |
| F02-K05-04 | C1 | ROM, 315 Flexatone |
| F02-K05-05 | C#1 | ROM, 257 Guiro-Tap ROM, 258 Maracas-Push |
| F02-K05-06 | D1 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K05-07 | D#1 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K05-08 | E1 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K05-09 | F1 | ROM, 291 Tambourine-Acc.2 |
| F02-K05-10 | F#1 | ROM, 288 Tambourine-Push ROM, 289 Tambourine-Pull |
| F02-K05-11 | G1 | ROM, 310 Rap Sleighbell |
| F02-K05-12 | G#1 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K05-13 | A1 | ROM, 312 Marc Tree ROM, 312 Marc Tree |
| F02-K05-14 | A#1 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K05-15 | B1 | ROM, 18 BD-Dance 1 ROM, 18 BD-Dance 1 |
| F02-K05-16 | C2 | ROM, 178 99-BD 3 ROM, 178 99-BD 3 |
| F02-K05-17 | C#2 | ROM, 21 BD-Dance 4 ROM, 21 BD-Dance 4 |
| F02-K05-18 | D2 | ROM, 29 BD-Short88 ROM, 29 BD-Short88 |
| F02-K05-19 | D#2 | ROM, 17 BD-Squash ROM, 17 BD-Squash |
| F02-K05-20 | E2 | ROM, 174 88-BD 2(R&B) ROM, 174 88-BD 2(R&B) |
| F02-K05-21 | F2 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K05-22 | F#2 | ROM, 183 99-SD 1 ROM, 183 99-SD 1 |
| F02-K05-23 | G2 | ROM, 77 SD-Hip 4 ROM, 77 SD-Hip 4 |
| F02-K05-24 | G#2 | ROM, 72 SD-Sizzle ROM, 72 SD-Sizzle |
| F02-K05-25 | A2 | ROM, 71 SD-Noise |
| F02-K05-26 | A#2 | ROM, 74 SD-Hip 1 |
| F02-K05-27 | B2 | ROM, 179 99-BD 4 ROM, 179 99-BD 4 |
| F02-K05-28 | C3 | ROM, 176 99-BD 1 ROM, 176 99-BD 1 |
| F02-K05-29 | C#3 | ROM, 185 88-Rimshot ROM, 185 88-Rimshot |
| F02-K05-30 | D3 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K05-31 | D#3 | ROM, 186 88-Claps ROM, 186 88-Claps |
| F02-K05-32 | E3 | ROM, 184 99-SD 2 ROM, 184 99-SD 2 |
| F02-K05-33 | F3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K05-34 | F#3 | ROM, 189 99-HHat Closed ROM, 189 99-HHat Closed |
| F02-K05-35 | G3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K05-36 | G#3 | ROM, 190 99-HHat Open ROM, 190 99-HHat Open |
| F02-K05-37 | A3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K05-38 | A#3 | ROM, 190 99-HHat Open ROM, 190 99-HHat Open |
| F02-K05-39 | B3 | ROM, 150 H.Hat-Whispy |
| F02-K05-40 | C4 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K05-41 | C#4 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K05-42 | D4 | ROM, 188 88-HHat Open ROM, 188 88-HHat Open |
| F02-K05-43 | D#4 | ROM, 160 Ride-Dance 99 ROM, 160 Ride-Dance 99 |
| F02-K05-44 | E4 | ROM, 153 Crash Cymbal 2 |
| F02-K05-45 | F4 | ROM, 161 Ride-Dance DDD1 ROM, 161 Ride-Dance DDD1 |
| F02-K05-46 | F#4 | ROM, 291 Tambourine-Acc.2 ROM, 290 Tambourine-Acc.1 |
| F02-K05-47 | G4 | ROM, 156 Splash Cymbal ROM, 156 Splash Cymbal |
| F02-K05-48 | G#4 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K05-49 | A4 | ROM, 158 Crash-Dance 99 ROM, 158 Crash-Dance 99 |
| F02-K05-50 | A#4 | ROM, 207 Zap 3 ROM, 207 Zap 3 |
| F02-K05-51 | B4 | ROM, 157 Cymbal Reverse ROM, 157 Cymbal Reverse |
| F02-K05-52 | C5 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K05-53 | C#5 | ROM, 206 Zap 2 ROM, 206 Zap 2 |
| F02-K05-54 | D5 | ROM, 194 88-Clave |
| F02-K05-55 | D#5 | ROM, 193 88-Conga ROM, 193 88-Conga |
| F02-K05-56 | E5 | ROM, 193 88-Conga ROM, 193 88-Conga |
| F02-K05-57 | F5 | ROM, 324 Metal Hit |
| F02-K05-58 | F#5 | ROM, 132 H.Hat-Chili |
| F02-K05-59 | G5 | ROM, 323 Industry Loop 2 |
| F02-K05-60 | G#5 | ROM, 127 H.Hat 2-Foot |
| F02-K05-61 | A5 | ROM, 204 Dance Perc-Hous2 |
| F02-K05-62 | A#5 | ROM, 196 88-Maraca |
| F02-K05-63 | B5 | ROM, 199 Vocal SD ROM, 199 Vocal SD |
| F02-K05-64 | C6 | ROM, 377 Car Crash |
| F02-K05-65 | C#6 | ROM, 293 Triangle-Mute |
| F02-K05-66 | D6 | ROM, 395 Noise-White |
| F02-K05-67 | D#6 | ROM, 292 Triangle-Open |
| F02-K05-68 | E6 | ROM, 400 GtCutNois1 |
| F02-K05-69 | F6 | ROM, 146 H.Hat-Hip 2 |
| F02-K05-70 | F#6 | ROM, 151 H.Hat-Alpo Close ROM, 151 H.Hat-Alpo Close |
| F02-K05-71 | G6 | ROM, 402 StringSlap |
| F02-K05-72 | G#6 | ROM, 147 H.Hat-AmbCrackle |
| F02-K05-73 | A6 | ROM, 406 Dist.Slide 1 |
| F02-K05-74 | A#6 | ROM, 135 H.Hat-Crisp Open |
| F02-K05-75 | B6 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K05-76 | C7 | ROM, 391 Swish Terra |
| F02-K05-77 | C#7 | ROM, 152 Crash Cymbal 1 ROM, 152 Crash Cymbal 1 |
| F02-K05-78 | D7 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K05-79 | D#7 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K05-80 | E7 | ROM, 238 Conga Hi-Slap 1 ROM, 242 Bongo Lo-Open |
| F02-K05-81 | F7 | ROM, 235 Conga Hi-Open ROM, 233 Conga Lo-MtSlap |
| F02-K05-82 | F#7 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K05-83 | G7 | ROM, 239 Conga Hi-Slap 2 ROM, 236 Conga Hi-Mute |
| F02-K05-84 | G#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K05-85 | A7 | ROM, 307 Chacha Bell ROM, 307 Chacha Bell |
| F02-K05-86 | A#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K05-87 | B7 | ROM, 256 Guiro-Short |
| F02-K05-88 | C8 | ROM, 257 Guiro-Tap ROM, 257 Guiro-Tap |
| F02-K05-89 | D8 | ROM, 202 Dance Perc-Hip |

### F02-K06 - Tricky Kit (kit 005, D1 p20)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K06-01 | A0 |  |
| F02-K06-02 | A#0 |  |
| F02-K06-03 | B0 |  |
| F02-K06-04 | C1 | ROM, 325 Yea Solo ROM, 326 Year solo |
| F02-K06-05 | C#1 | ROM, 329 Uhhhh Solo ROM, 328 Hit It |
| F02-K06-06 | D1 | ROM, 327 Uhh Solo |
| F02-K06-07 | D#1 | ROM, 335 CompuVoice-When ROM, 334 CompuVoice-Do it |
| F02-K06-08 | E1 | ROM, 331 CompuVoice-Noise ROM, 330 CompuVoice-Beep |
| F02-K06-09 | F1 | ROM, 332 CompuVoice-Chi ROM, 333 CompuVoice-Test |
| F02-K06-10 | F#1 | ROM, 337 CompuVoice-Ti ROM, 336 CompuVoice-Toi |
| F02-K06-11 | G1 | ROM, 343 PC Vox-Dunc ROM, 342 PC Vox-Sis |
| F02-K06-12 | G#1 | ROM, 348 PC Vox-Zee ROM, 345 PC Vox-For |
| F02-K06-13 | A1 | ROM, 349 PC Vox-Ae ROM, 350 PC Vox-Pa |
| F02-K06-14 | A#1 | ROM, 350 PC Vox-Pa ROM, 351 PC Vox-Sh |
| F02-K06-15 | B1 | ROM, 352 PC Vox-O ROM, 353 PC Vox-Tu |
| F02-K06-16 | C2 | ROM, 174 88-BD 2(R&B) ROM, 174 88-BD 2(R&B) |
| F02-K06-17 | C#2 | ROM, 26 BD-Dark ROM, 180 99-BD 5 |
| F02-K06-18 | D2 | ROM, 22 BD-Hip 1 ROM, 23 BD-Hip 2 |
| F02-K06-19 | D#2 | ROM, 180 99-BD 5 ROM, 180 99-BD 5 |
| F02-K06-20 | E2 | ROM, 34 BD-Dance Soft ROM, 29 BD-Short88 |
| F02-K06-21 | F2 | ROM, 91 SD-AmbiHop ROM, 91 SD-AmbiHop |
| F02-K06-22 | F#2 | ROM, 98 SD-Ambi Hit 1 |
| F02-K06-23 | G2 | ROM, 92 SD-Brasser |
| F02-K06-24 | G#2 | ROM, 97 SD-Ambi Pop ROM, 88 SD-AmbiCrackle 1 |
| F02-K06-25 | A2 | ROM, 96 SD-Dry Present |
| F02-K06-26 | A#2 | ROM, 87 SD-Vintage 6 ROM, 87 SD-Vintage 6 |
| F02-K06-27 | B2 | ROM, 5 BD-Dry 6 ROM, 26 BD-Dark |
| F02-K06-28 | C3 | ROM, 27 BD-Pop ROM, 29 BD-Short88 |
| F02-K06-29 | C#3 | ROM, 66 Sidestick-Amb |
| F02-K06-30 | D3 | ROM, 82 SD-Vintage 1 ROM, 82 SD-Vintage 1 |
| F02-K06-31 | D#3 | ROM, 230 Hand Claps-Hip ROM, 231 Hand Claps-Happy |
| F02-K06-32 | E3 | ROM, 79 SD-Hip 6 |
| F02-K06-33 | F3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K06-34 | F#3 | ROM, 139 H.Hat-Tight 1 ROM, 140 H.Hat-Tight 2 |
| F02-K06-35 | G3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K06-36 | G#3 | ROM, 142 H.Hat-Old Cl 2 ROM, 145 H.Hat-Hip 1 |
| F02-K06-37 | A3 | ROM, 192 88-Tom ROM, 192 88-Tom |
| F02-K06-38 | A#3 | ROM, 149 H.Hat-ClangyOpen ROM, 135 H.Hat-Crisp Open |
| F02-K06-39 | B3 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K06-40 | C4 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K06-41 | C#4 | ROM, 396 Noise-FM Mod |
| F02-K06-42 | D4 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K06-43 | D#4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K06-44 | E4 | ROM, 191 88-Crash |
| F02-K06-45 | F4 | ROM, 165 Ride-Cup |
| F02-K06-46 | F#4 | ROM, 290 Tambourine-Acc.1 |
| F02-K06-47 | G4 | ROM, 159 Crash-Dance DDD1 |
| F02-K06-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K06-49 | A4 | ROM, 383 Explosion |
| F02-K06-50 | A#4 | ROM, 388 ChurchBell |
| F02-K06-51 | B4 | ROM, 366 Crickets 2 ROM, 366 Crickets 2 |
| F02-K06-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K06-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K06-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K06-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K06-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K06-57 | F5 | ROM, 310 Rap Sleighbell ROM, 310 Rap Sleighbell |
| F02-K06-58 | F#5 | ROM, 217 DJ-BD Rub |
| F02-K06-59 | G5 | ROM, 218 DJ-SD Rub |
| F02-K06-60 | G#5 | ROM, 219 DJ-Record Stop ROM, 220 DJ-Reverse |
| F02-K06-61 | A5 | ROM, 299 Cabasa-Up |
| F02-K06-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K06-63 | B5 | ROM, 196 88-Maraca |
| F02-K06-64 | C6 | ROM, 320 SD-Metal Striker |
| F02-K06-65 | C#6 | ROM, 374 Car Engine |
| F02-K06-66 | D6 | ROM, 323 Industry Loop 2 |
| F02-K06-67 | D#6 | ROM, 323 Industry Loop 2 |
| F02-K06-68 | E6 | ROM, 375 Car Stop |
| F02-K06-69 | F6 | ROM, 291 Tambourine-Acc.2 |
| F02-K06-70 | F#6 | ROM, 323 Industry Loop 2 |
| F02-K06-71 | G6 | ROM, 402 StringSlap |
| F02-K06-72 | G#6 | ROM, 202 Dance Perc-Hip |
| F02-K06-73 | A6 | ROM, 205 Zap 1 |
| F02-K06-74 | A#6 | ROM, 387 Bubble |
| F02-K06-75 | B6 | ROM, 377 Car Crash |
| F02-K06-76 | C7 | ROM, 173 88-BD 1(Rap) |
| F02-K06-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K06-78 | D7 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K06-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K06-80 | E7 | ROM, 317 Chinese Gong |
| F02-K06-81 | F7 | ROM, 194 88-Clave ROM, 194 88-Clave |
| F02-K06-82 | F#7 | ROM, 385 Wind |
| F02-K06-83 | G7 | ROM, 386 Stream |
| F02-K06-84 | G#7 | ROM, 340 Two... Solo ROM, 341 One... Solo |
| F02-K06-85 | A7 | ROM, 338 Four... Solo ROM, 339 Three... Solo |
| F02-K06-86 | A#7 | ROM, 314 Bell Tree |
| F02-K06-87 | B7 | ROM, 390 Space Lore |
| F02-K06-88 | C8 | ROM, 367 Hart Beat |
| F02-K06-89 | D8 |  |

### F02-K07 - Drum'n'Bass Kit (kit 006, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K07-01 | A0 |  |
| F02-K07-02 | A#0 |  |
| F02-K07-03 | B0 |  |
| F02-K07-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K07-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K07-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K07-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K07-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K07-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K07-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K07-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K07-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K07-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K07-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K07-15 | B1 | ROM, 73 SD-Reverse |
| F02-K07-16 | C2 | ROM, 37 BD-PhatJaz ROM, 37 BD-PhatJaz |
| F02-K07-17 | C#2 | ROM, 14 BD-Tubby |
| F02-K07-18 | D2 | ROM, 5 BD-Dry 6 ROM, 5 BD-Dry 6 |
| F02-K07-19 | D#2 | ROM, 39 BD-Klanger ROM, 22 BD-Hip 1 |
| F02-K07-20 | E2 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K07-21 | F2 | ROM, 100 SD-Ambi Hit 3 ROM, 100 SD-Ambi Hit 3 |
| F02-K07-22 | F#2 | ROM, 76 SD-Hip 3 ROM, 76 SD-Hip 3 |
| F02-K07-23 | G2 | ROM, 94 SD-Klanger ROM, 51 SD-Amb.Piccolo 1 |
| F02-K07-24 | G#2 | ROM, 80 SD-Ringy ROM, 80 SD-Ringy |
| F02-K07-25 | A2 | ROM, 93 SD-Chili ROM, 93 SD-Chili |
| F02-K07-26 | A#2 | ROM, 40 SD-Dry 1 ROM, 40 SD-Dry 1 |
| F02-K07-27 | B2 | ROM, 28 BD-Ringy ROM, 29 BD-Short88 |
| F02-K07-28 | C3 | ROM, 32 BD-Ambi Crackle ROM, 31 BD-Ambi |
| F02-K07-29 | C#3 | ROM, 65 Sidestick-Dry ROM, 185 88-Rimshot |
| F02-K07-30 | D3 | ROM, 93 SD-Chili ROM, 93 SD-Chili |
| F02-K07-31 | D#3 | ROM, 231 Hand Claps-Happy ROM, 230 Hand Claps-Hip |
| F02-K07-32 | E3 | ROM, 90 SD-AmbiCrackle 3 ROM, 90 SD-AmbiCrackle 3 |
| F02-K07-33 | F3 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K07-34 | F#3 | ROM, 134 H.Hat-Crisp Cl 2 ROM, 134 H.Hat-Crisp Cl 2 |
| F02-K07-35 | G3 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K07-36 | G#3 | ROM, 119 H.Hat 1-Closed 2 ROM, 119 H.Hat 1-Closed 2 |
| F02-K07-37 | A3 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K07-38 | A#3 | ROM, 135 H.Hat-Crisp Open ROM, 123 H.Hat 1-Open 2 |
| F02-K07-39 | B3 | ROM, 137 H.Hat-Vintage 1 ROM, 137 H.Hat-Vintage 1 |
| F02-K07-40 | C4 | ROM, 147 H.Hat-AmbCrackle ROM, 147 H.Hat-AmbCrackle |
| F02-K07-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K07-42 | D4 | ROM, 144 H.Hat-Old Open ROM, 144 H.Hat-Old Open |
| F02-K07-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K07-44 | E4 | ROM, 396 Noise-FM Mod ROM, 393 Cricket Spectrum |
| F02-K07-45 | F4 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K07-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K07-47 | G4 | ROM, 291 Tambourine-Acc.2 ROM, 289 Tambourine-Pull |
| F02-K07-48 | G#4 | ROM, 146 H.Hat-Hip 2 |
| F02-K07-49 | A4 | ROM, 152 Crash Cymbal 1 |
| F02-K07-50 | A#4 | ROM, 381 MachineGun ROM, 381 MachineGun |
| F02-K07-51 | B4 | ROM, 161 Ride-Dance DDD1 ROM, 161 Ride-Dance DDD1 |
| F02-K07-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K07-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K07-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K07-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K07-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K07-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K07-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K07-59 | G5 | ROM, 202 Dance Perc-Hip |
| F02-K07-60 | G#5 | ROM, 202 Dance Perc-Hip |
| F02-K07-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K07-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K07-63 | B5 | ROM, 45 SD-GhostSet f ROM, 45 SD-GhostSet f |
| F02-K07-64 | C6 | ROM, 44 SD-GhostSet roll ROM, 44 SD-GhostSet roll |
| F02-K07-65 | C#6 | ROM, 378 Train |
| F02-K07-66 | D6 | ROM, 390 Space Lore ROM, 390 Space Lore |
| F02-K07-67 | D#6 | ROM, 396 Noise-FM Mod |
| F02-K07-68 | E6 | ROM, 394 Air Vortex |
| F02-K07-69 | F6 | ROM, 393 Cricket Spectrum ROM, 393 Cricket Spectrum |
| F02-K07-70 | F#6 | ROM, 374 Car Engine |
| F02-K07-71 | G6 | ROM, 373 Door Slam |
| F02-K07-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K07-73 | A6 | ROM, 292 Triangle-Open |
| F02-K07-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K07-75 | B6 | ROM, 314 Bell Tree |
| F02-K07-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K07-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K07-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K07-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K07-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K07-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K07-82 | F#7 | ROM, 186 88-Claps |
| F02-K07-83 | G7 | ROM, 196 88-Maraca |
| F02-K07-84 | G#7 | ROM, 171 SD-Orch.Roll 2 |
| F02-K07-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K07-86 | A#7 | ROM, 314 Bell Tree |
| F02-K07-87 | B7 | ROM, 354 Stadium |
| F02-K07-88 | C8 | ROM, 355 Applause |
| F02-K07-89 | C#8 |  |
| F02-K07-90 | D8 |  |

### F02-K08 - Hip Hop Kit (kit 007, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K08-01 | A0 |  |
| F02-K08-02 | A#0 |  |
| F02-K08-03 | B0 |  |
| F02-K08-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K08-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K08-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K08-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K08-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K08-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K08-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K08-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K08-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K08-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K08-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K08-15 | B1 | ROM, 73 SD-Reverse |
| F02-K08-16 | C2 | ROM, 37 BD-PhatJaz |
| F02-K08-17 | C#2 | ROM, 22 BD-Hip 1 |
| F02-K08-18 | D2 | ROM, 21 BD-Dance 4 ROM, 21 BD-Dance 4 |
| F02-K08-19 | D#2 | ROM, 35 BD-AmbiRocker ROM, 35 BD-AmbiRocker |
| F02-K08-20 | E2 | ROM, 180 99-BD 5 |
| F02-K08-21 | F2 | ROM, 86 SD-Vintage 5 |
| F02-K08-22 | F#2 | ROM, 98 SD-Ambi Hit 1 |
| F02-K08-23 | G2 | ROM, 89 SD-AmbiCrackle 2 |
| F02-K08-24 | G#2 | ROM, 83 SD-Vintage 2 |
| F02-K08-25 | A2 | ROM, 76 SD-Hip 3 |
| F02-K08-26 | A#2 | ROM, 67 SD-Amb.House |
| F02-K08-27 | B2 | ROM, 175 88-BD 3 |
| F02-K08-28 | C3 | ROM, 31 BD-Ambi |
| F02-K08-29 | C#3 | ROM, 65 Sidestick-Dry |
| F02-K08-30 | D3 | ROM, 95 SD-Whopper |
| F02-K08-31 | D#3 | ROM, 231 Hand Claps-Happy |
| F02-K08-32 | E3 | ROM, 82 SD-Vintage 1 |
| F02-K08-33 | F3 | ROM, 116 Tom-Dirty Funk ROM, 116 Tom-Dirty Funk |
| F02-K08-34 | F#3 | ROM, 147 H.Hat-AmbCrackle |
| F02-K08-35 | G3 | ROM, 116 Tom-Dirty Funk ROM, 116 Tom-Dirty Funk |
| F02-K08-36 | G#3 | ROM, 142 H.Hat-Old Cl 2 |
| F02-K08-37 | A3 | ROM, 116 Tom-Dirty Funk ROM, 116 Tom-Dirty Funk |
| F02-K08-38 | A#3 | ROM, 135 H.Hat-Crisp Open |
| F02-K08-39 | B3 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K08-40 | C4 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K08-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K08-42 | D4 | ROM, 117 Tom-Old R&B ROM, 117 Tom-Old R&B |
| F02-K08-43 | D#4 | ROM, 162 Ride-Edge 1 |
| F02-K08-44 | E4 | ROM, 155 China Cymbal |
| F02-K08-45 | F4 | ROM, 165 Ride-Cup |
| F02-K08-46 | F#4 | ROM, 290 Tambourine-Acc.1 |
| F02-K08-47 | G4 | ROM, 156 Splash Cymbal |
| F02-K08-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K08-49 | A4 | ROM, 152 Crash Cymbal 1 |
| F02-K08-50 | A#4 | ROM, 277 Vibraslap |
| F02-K08-51 | B4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K08-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K08-53 | C#5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K08-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K08-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K08-56 | E5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K08-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K08-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K08-59 | G5 | ROM, 304 Agogo Bell |
| F02-K08-60 | G#5 | ROM, 304 Agogo Bell |
| F02-K08-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K08-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K08-63 | B5 | ROM, 316 Samba Whistle |
| F02-K08-64 | C6 | ROM, 316 Samba Whistle |
| F02-K08-65 | C#6 | ROM, 256 Guiro-Short ROM, 257 Guiro-Tap |
| F02-K08-66 | D6 | ROM, 255 Guiro-Long |
| F02-K08-67 | D#6 | ROM, 278 Claves |
| F02-K08-68 | E6 | ROM, 279 Woodblock 1 |
| F02-K08-69 | F6 | ROM, 279 Woodblock 1 |
| F02-K08-70 | F#6 | ROM, 295 Cuica-Hi |
| F02-K08-71 | G6 | ROM, 296 Cuica-Lo |
| F02-K08-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K08-73 | A6 | ROM, 292 Triangle-Open |
| F02-K08-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K08-75 | B6 | ROM, 312 Marc Tree |
| F02-K08-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K08-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K08-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K08-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K08-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K08-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K08-82 | F#7 | ROM, 186 88-Claps |
| F02-K08-83 | G7 | ROM, 196 88-Maraca |
| F02-K08-84 | G#7 | ROM, 171 SD-Orch.Roll 2 |
| F02-K08-85 | A7 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K08-86 | A#7 | ROM, 314 Bell Tree |
| F02-K08-87 | B7 | ROM, 354 Stadium |
| F02-K08-88 | C8 | ROM, 355 Applause |
| F02-K08-89 | C#8 |  |
| F02-K08-90 | D8 |  |

### F02-K09 - Psycho Kit (kit 008, D1 p21)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K09-01 | A0 |  |
| F02-K09-02 | A#0 |  |
| F02-K09-03 | B0 |  |
| F02-K09-04 | C1 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K09-05 | C#1 | ROM, 294 Triangle-Roll ROM, 294 Triangle-Roll |
| F02-K09-06 | D1 | ROM, 315 Flexatone ROM, 315 Flexatone |
| F02-K09-07 | D#1 | ROM, 274 Taiko-Open ROM, 274 Taiko-Open |
| F02-K09-08 | E1 | ROM, 263 Baya-Mute 2 ROM, 263 Baya-Mute 2 |
| F02-K09-09 | F1 | ROM, 72 SD-Sizzle ROM, 72 SD-Sizzle |
| F02-K09-10 | F#1 | ROM, 58 SD-Brush Swirl ROM, 58 SD-Brush Swirl |
| F02-K09-11 | G1 | ROM, 70 SD-Rap 2 ROM, 70 SD-Rap 2 |
| F02-K09-12 | G#1 | ROM, 73 SD-Reverse ROM, 73 SD-Reverse |
| F02-K09-13 | A1 | ROM, 15 BD-Gated ROM, 15 BD-Gated |
| F02-K09-14 | A#1 | ROM, 157 Cymbal Reverse ROM, 157 Cymbal Reverse |
| F02-K09-15 | B1 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K09-16 | C2 | ROM, 17 BD-Squash ROM, 17 BD-Squash |
| F02-K09-17 | C#2 | ROM, 9 BD-Jazz 2 ROM, 9 BD-Jazz 2 |
| F02-K09-18 | D2 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K09-19 | D#2 | ROM, 358 Footstep 1 ROM, 359 Footstep 2 |
| F02-K09-20 | E2 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K09-21 | F2 | ROM, 267 Tabla-Na ROM, 268 Tabla-Open |
| F02-K09-22 | F#2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K09-23 | G2 | ROM, 70 SD-Rap 2 ROM, 70 SD-Rap 2 |
| F02-K09-24 | G#2 | ROM, 68 SD-Dance ROM, 68 SD-Dance |
| F02-K09-25 | A2 | ROM, 52 SD-Amb.Piccolo 2 ROM, 49 SD-Off Center |
| F02-K09-26 | A#2 | ROM, 59 SD-Atomic ROM, 59 SD-Atomic |
| F02-K09-27 | B2 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K09-28 | C3 | ROM, 19 BD-Dance 2 ROM, 19 BD-Dance 2 |
| F02-K09-29 | C#3 | ROM, 285 Timbale Hi-Rim 1 ROM, 285 Timbale Hi-Rim 1 |
| F02-K09-30 | D3 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K09-31 | D#3 | ROM, 207 Zap 3 ROM, 207 Zap 3 |
| F02-K09-32 | E3 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K09-33 | F3 | ROM, 105 Tom 2-Lo ROM, 105 Tom 2-Lo |
| F02-K09-34 | F#3 | ROM, 189 99-HHat Closed ROM, 189 99-HHat Closed |
| F02-K09-35 | G3 | ROM, 115 Tom-Brush Floor ROM, 115 Tom-Brush Floor |
| F02-K09-36 | G#3 | ROM, 188 88-HHat Open ROM, 188 88-HHat Open |
| F02-K09-37 | A3 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K09-38 | A#3 | ROM, 292 Triangle-Open ROM, 190 99-HHat Open |
| F02-K09-39 | B3 | ROM, 198 Real E.Tom |
| F02-K09-40 | C4 | ROM, 111 Tom-Jazz Hi |
| F02-K09-41 | C#4 | ROM, 168 Orch Cym.-Open ROM, 154 Crash Cymbal 3 |
| F02-K09-42 | D4 | ROM, 108 Tom 3-Lo |
| F02-K09-43 | D#4 | ROM, 164 Ride-Jazz ROM, 163 Ride-Edge 2 |
| F02-K09-44 | E4 | ROM, 394 Air Vortex ROM, 394 Air Vortex |
| F02-K09-45 | F4 | ROM, 219 DJ-Record Stop ROM, 208 DJ-Scratch 1 |
| F02-K09-46 | F#4 | ROM, 219 DJ-Record Stop ROM, 208 DJ-Scratch 1 |
| F02-K09-47 | G4 | ROM, 221 DJ-Old Record ROM, 221 DJ-Old Record |
| F02-K09-48 | G#4 | ROM, 406 Dist.Slide 1 ROM, 407 Dist.Slide 2 |
| F02-K09-49 | A4 | ROM, 168 Orch Cym.-Open ROM, 168 Orch Cym.-Open |
| F02-K09-50 | A#4 | ROM, 155 China Cymbal ROM, 154 Crash Cymbal 3 |
| F02-K09-51 | B4 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K09-52 | C5 | ROM, 277 Vibraslap ROM, 277 Vibraslap |
| F02-K09-53 | C#5 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K09-54 | D5 | ROM, 305 Cowbell 1 ROM, 308 Mambo Bell |
| F02-K09-55 | D#5 | ROM, 291 Tambourine-Acc.2 ROM, 302 Caxixi-Hard |
| F02-K09-56 | E5 | ROM, 390 Space Lore ROM, 390 Space Lore |
| F02-K09-57 | F5 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K09-58 | F#5 | ROM, 292 Triangle-Open ROM, 292 Triangle-Open |
| F02-K09-59 | G5 | ROM, 392 Xylophone Spectr ROM, 392 Xylophone Spectr |
| F02-K09-60 | G#5 | ROM, 290 Tambourine-Acc.1 ROM, 289 Tambourine-Pull |
| F02-K09-61 | A5 | ROM, 319 Cyber Drum ROM, 394 Air Vortex |
| F02-K09-62 | A#5 | ROM, 397 Tubular ROM, 397 Tubular |
| F02-K09-63 | B5 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K09-64 | C6 | ROM, 255 Guiro-Long ROM, 255 Guiro-Long |
| F02-K09-65 | C#6 | ROM, 199 Vocal SD ROM, 199 Vocal SD |
| F02-K09-66 | D6 | ROM, 274 Taiko-Open ROM, 398 Gamelan |
| F02-K09-67 | D#6 | ROM, 224 Band Hit ROM, 224 Band Hit |
| F02-K09-68 | E6 | ROM, 389 Telephone Ring ROM, 389 Telephone Ring |
| F02-K09-69 | F6 | ROM, 358 Footstep 1 ROM, 359 Footstep 2 |
| F02-K09-70 | F#6 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K09-71 | G6 | ROM, 393 Cricket Spectrum ROM, 393 Cricket Spectrum |
| F02-K09-72 | G#6 | ROM, 396 Noise-FM Mod ROM, 396 Noise-FM Mod |
| F02-K09-73 | A6 | ROM, 395 Noise-White ROM, 395 Noise-White |
| F02-K09-74 | A#6 | ROM, 396 Noise-FM Mod ROM, 396 Noise-FM Mod |
| F02-K09-75 | B6 | ROM, 304 Agogo Bell ROM, 304 Agogo Bell |
| F02-K09-76 | C7 | ROM, 370 GunShot ROM, 370 GunShot |
| F02-K09-77 | C#7 | ROM, 311 Finger Cymbal ROM, 312 Marc Tree |
| F02-K09-78 | D7 | ROM, 316 Samba Whistle ROM, 316 Samba Whistle |
| F02-K09-79 | D#7 | ROM, 323 Industry Loop 2 ROM, 320 SD-Metal Striker |
| F02-K09-80 | E7 | ROM, 217 DJ-BD Rub ROM, 217 DJ-BD Rub |
| F02-K09-81 | F7 | ROM, 318 Industry ROM, 318 Industry |
| F02-K09-82 | F#7 | ROM, 407 Dist.Slide 2 ROM, 406 Dist.Slide 1 |
| F02-K09-83 | G7 | ROM, 403 HarpGliss ROM, 371 Rainstick |
| F02-K09-84 | G#7 | ROM, 253 Castanet-Dble 1 ROM, 277 Vibraslap |
| F02-K09-85 | A7 | ROM, 219 DJ-Record Stop ROM, 220 DJ-Reverse |
| F02-K09-86 | A#7 | ROM, 209 DJ-Scratch 2 ROM, 208 DJ-Scratch 1 |
| F02-K09-87 | B7 | ROM, 369 Tribe ROM, 369 Tribe |
| F02-K09-88 | C8 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K09-89 | C#8 | 21 |
| F02-K09-90 | D8 |  |

### F02-K10 - Percussion Kit (kit 009, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K10-01 | A0 |  |
| F02-K10-02 | A#0 |  |
| F02-K10-03 | B0 |  |
| F02-K10-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K10-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K10-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K10-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K10-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K10-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K10-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K10-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K10-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K10-13 | A1 | ROM, 271 Tabla-Mute 2 ROM, 272 Tabla-Mute 3 |
| F02-K10-14 | A#1 | ROM, 267 Tabla-Na ROM, 270 Tabla-Mute 1 |
| F02-K10-15 | B1 | ROM, 270 Tabla-Mute 1 ROM, 271 Tabla-Mute 2 |
| F02-K10-16 | C2 | ROM, 166 BD-Orchestral |
| F02-K10-17 | C#2 | ROM, 369 Tribe |
| F02-K10-18 | D2 | ROM, 228 Finger Snap |
| F02-K10-19 | D#2 | ROM, 229 Hand Claps |
| F02-K10-20 | E2 | ROM, 293 Triangle-Mute |
| F02-K10-21 | F2 | ROM, 292 Triangle-Open |
| F02-K10-22 | F#2 | ROM, 254 Castanet-Dble 2 |
| F02-K10-23 | G2 | ROM, 252 Castanet-Single |
| F02-K10-24 | G#2 | ROM, 253 Castanet-Dble 1 |
| F02-K10-25 | A2 | ROM, 255 Guiro-Long |
| F02-K10-26 | A#2 | ROM, 256 Guiro-Short ROM, 256 Guiro-Short |
| F02-K10-27 | B2 | ROM, 257 Guiro-Tap |
| F02-K10-28 | C3 | ROM, 277 Vibraslap |
| F02-K10-29 | C#3 | ROM, 278 Claves |
| F02-K10-30 | D3 | ROM, 194 88-Clave |
| F02-K10-31 | D#3 | ROM, 296 Cuica-Lo |
| F02-K10-32 | E3 | ROM, 295 Cuica-Hi |
| F02-K10-33 | F3 | ROM, 281 Timbale Lo-Open |
| F02-K10-34 | F#3 | ROM, 279 Woodblock 1 |
| F02-K10-35 | G3 | ROM, 283 Timbale Lo-Rim |
| F02-K10-36 | G#3 | ROM, 279 Woodblock 1 |
| F02-K10-37 | A3 | ROM, 282 Timbale Lo-Mute |
| F02-K10-38 | A#3 | ROM, 201 Vocal Woodblock |
| F02-K10-39 | B3 | ROM, 284 Timbale Hi-Edge |
| F02-K10-40 | C4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-K10-41 | C#4 | ROM, 307 Chacha Bell |
| F02-K10-42 | D4 | ROM, 285 Timbale Hi-Rim 1 |
| F02-K10-43 | D#4 | ROM, 308 Mambo Bell |
| F02-K10-44 | E4 | ROM, 287 Timbale-Paila |
| F02-K10-45 | F4 | ROM, 242 Bongo Lo-Open |
| F02-K10-46 | F#4 | ROM, 305 Cowbell 1 |
| F02-K10-47 | G4 | ROM, 243 Bongo Lo-Slap |
| F02-K10-48 | G#4 | ROM, 306 Cowbell 2 |
| F02-K10-49 | A4 | ROM, 245 Bongo Hi-Open |
| F02-K10-50 | A#4 | ROM, 200 Vocal Cowbell |
| F02-K10-51 | B4 | ROM, 246 Bongo Hi-Slap ROM, 246 Bongo Hi-Slap |
| F02-K10-52 | C5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K10-53 | C#5 | ROM, 234 Conga Lo-Slap |
| F02-K10-54 | D5 | ROM, 233 Conga Lo-MtSlap |
| F02-K10-55 | D#5 | ROM, 237 Conga Hi-MtSlap |
| F02-K10-56 | E5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K10-57 | F5 | ROM, 236 Conga Hi-Mute |
| F02-K10-58 | F#5 | ROM, 240 Conga-Heel |
| F02-K10-59 | G5 | ROM, 238 Conga Hi-Slap 1 |
| F02-K10-60 | G#5 | ROM, 241 Conga-Toe |
| F02-K10-61 | A5 | ROM, 239 Conga Hi-Slap 2 |
| F02-K10-62 | A#5 | ROM, 304 Agogo Bell |
| F02-K10-63 | B5 | ROM, 304 Agogo Bell |
| F02-K10-64 | C6 | ROM, 259 Maracas-Pull ROM, 258 Maracas-Push |
| F02-K10-65 | C#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K10-66 | D6 | ROM, 298 Shaker 2 ROM, 298 Shaker 2 |
| F02-K10-67 | D#6 | ROM, 301 Cabasa-Tap ROM, 300 Cabasa-Down |
| F02-K10-68 | E6 | ROM, 301 Cabasa-Tap ROM, 299 Cabasa-Up |
| F02-K10-69 | F6 | ROM, 302 Caxixi-Hard ROM, 303 Caxixi-Soft |
| F02-K10-70 | F#6 | ROM, 288 Tambourine-Push |
| F02-K10-71 | G6 | ROM, 290 Tambourine-Acc.1 ROM, 290 Tambourine-Acc.1 |
| F02-K10-72 | G#6 | ROM, 289 Tambourine-Pull |
| F02-K10-73 | A6 | ROM, 291 Tambourine-Acc.2 ROM, 291 Tambourine-Acc.2 |
| F02-K10-74 | A#6 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K10-75 | B6 | ROM, 316 Samba Whistle |
| F02-K10-76 | C7 | ROM, 316 Samba Whistle |
| F02-K10-77 | C#7 | ROM, 314 Bell Tree |
| F02-K10-78 | D7 | ROM, 316 Samba Whistle |
| F02-K10-79 | D#7 | ROM, 312 Marc Tree |
| F02-K10-80 | E7 | ROM, 316 Samba Whistle |
| F02-K10-81 | F7 | ROM, 310 Rap Sleighbell ROM, 310 Rap Sleighbell |
| F02-K10-82 | F#7 | ROM, 371 Rainstick |
| F02-K10-83 | G7 | ROM, 317 Chinese Gong |
| F02-K10-84 | G#7 | ROM, 361 Bird 1 |
| F02-K10-85 | A7 | ROM, 362 Bird 2 |
| F02-K10-86 | A#7 | ROM, 361 Bird 1 |
| F02-K10-87 | B7 | ROM, 393 Cricket Spectrum |
| F02-K10-88 | C8 | ROM, 312 Marc Tree |
| F02-K10-89 | D8 |  |

### F02-K11 - Orchestra&Ethnic (kit 010, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K11-01 | A0 |  |
| F02-K11-02 | A#0 |  |
| F02-K11-03 | B0 |  |
| F02-K11-04 | C1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-05 | C#1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-06 | D1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-07 | D#1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-08 | E1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-09 | F1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-10 | F#1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-11 | G1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-12 | G#1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-13 | A1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-14 | A#1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-15 | B1 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K11-16 | C2 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K11-17 | C#2 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K11-18 | D2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K11-19 | D#2 | ROM, 171 SD-Orch.Roll 2 ROM, 171 SD-Orch.Roll 2 |
| F02-K11-20 | E2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K11-21 | F2 | ROM, 168 Orch Cym.-Open ROM, 168 Orch Cym.-Open |
| F02-K11-22 | F#2 | ROM, 167 Orch Cym.-Closed ROM, 167 Orch Cym.-Closed |
| F02-K11-23 | G2 | ROM, 274 Taiko-Open ROM, 274 Taiko-Open |
| F02-K11-24 | G#2 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K11-25 | A2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K11-26 | A#2 | ROM, 171 SD-Orch.Roll 2 ROM, 171 SD-Orch.Roll 2 |
| F02-K11-27 | B2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K11-28 | C3 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K11-29 | C#3 | ROM, 294 Triangle-Roll ROM, 294 Triangle-Roll |
| F02-K11-30 | D3 | ROM, 292 Triangle-Open ROM, 292 Triangle-Open |
| F02-K11-31 | D#3 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K11-32 | E3 | ROM, 291 Tambourine-Acc.2 ROM, 291 Tambourine-Acc.2 |
| F02-K11-33 | F3 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K11-34 | F#3 | ROM, 253 Castanet-Dble 1 ROM, 253 Castanet-Dble 1 |
| F02-K11-35 | G3 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K11-36 | G#3 | ROM, 254 Castanet-Dble 2 ROM, 254 Castanet-Dble 2 |
| F02-K11-37 | A3 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K11-38 | A#3 | ROM, 252 Castanet-Single ROM, 252 Castanet-Single |
| F02-K11-39 | B3 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K11-40 | C4 | ROM, 279 Woodblock 1 ROM, 279 Woodblock 1 |
| F02-K11-41 | C#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K11-42 | D4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K11-43 | D#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K11-44 | E4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K11-45 | F4 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K11-46 | F#4 | ROM, 250 Djembe-Mute ROM, 250 Djembe-Mute |
| F02-K11-47 | G4 | ROM, 251 Djembe-Slap ROM, 251 Djembe-Slap |
| F02-K11-48 | G#4 | ROM, 302 Caxixi-Hard ROM, 303 Caxixi-Soft |
| F02-K11-49 | A4 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K11-50 | A#4 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K11-51 | B4 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K11-52 | C5 | ROM, 261 Baya-Ghe ROM, 261 Baya-Ghe |
| F02-K11-53 | C#5 | ROM, 260 Baya-Open ROM, 260 Baya-Open |
| F02-K11-54 | D5 | ROM, 262 Baya-Mute 1 ROM, 262 Baya-Mute 1 |
| F02-K11-55 | D#5 | ROM, 263 Baya-Mute 2 ROM, 263 Baya-Mute 2 |
| F02-K11-56 | E5 | ROM, 264 Baya-Mute 3 ROM, 264 Baya-Mute 3 |
| F02-K11-57 | F5 | ROM, 265 Baya-Mute 4 ROM, 265 Baya-Mute 4 |
| F02-K11-58 | F#5 | ROM, 266 Baya-Mute 5 ROM, 266 Baya-Mute 5 |
| F02-K11-59 | G5 | ROM, 267 Tabla-Na ROM, 267 Tabla-Na |
| F02-K11-60 | G#5 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K11-61 | A5 | ROM, 269 Tabla-Tin ROM, 269 Tabla-Tin |
| F02-K11-62 | A#5 | ROM, 270 Tabla-Mute 1 ROM, 270 Tabla-Mute 1 |
| F02-K11-63 | B5 | ROM, 271 Tabla-Mute 2 ROM, 271 Tabla-Mute 2 |
| F02-K11-64 | C6 | ROM, 272 Tabla-Mute 3 ROM, 272 Tabla-Mute 3 |
| F02-K11-65 | C#6 | ROM, 273 Tabla-Mute 4 ROM, 273 Tabla-Mute 4 |
| F02-K11-66 | D6 | ROM, 274 Taiko-Open ROM, 274 Taiko-Open |
| F02-K11-67 | D#6 | ROM, 276 Tsuzumi |
| F02-K11-68 | E6 | ROM, 275 Taiko-Rim ROM, 275 Taiko-Rim |
| F02-K11-69 | F6 | ROM, 232 Conga Lo-Open |
| F02-K11-70 | F#6 | ROM, 233 Conga Lo-MtSlap ROM, 239 Conga Hi-Slap 2 |
| F02-K11-71 | G6 | ROM, 236 Conga Hi-Mute ROM, 246 Bongo Hi-Slap |
| F02-K11-72 | G#6 | ROM, 238 Conga Hi-Slap 1 |
| F02-K11-73 | A6 | ROM, 399 Tambura ROM, 399 Tambura |
| F02-K11-74 | A#6 | ROM, 371 Rainstick ROM, 371 Rainstick |
| F02-K11-75 | B6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K11-76 | C7 | ROM, 371 Rainstick ROM, 371 Rainstick |
| F02-K11-77 | C#7 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K11-78 | D7 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K11-79 | D#7 | ROM, 398 Gamelan ROM, 398 Gamelan |
| F02-K11-80 | E7 | ROM, 398 Gamelan ROM, 398 Gamelan |
| F02-K11-81 | F7 | ROM, 361 Bird 1 ROM, 361 Bird 1 |
| F02-K11-82 | F#7 | ROM, 393 Cricket Spectrum |
| F02-K11-83 | G7 | ROM, 369 Tribe ROM, 369 Tribe |
| F02-K11-84 | G#7 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K11-85 | A7 | ROM, 398 Gamelan ROM, 398 Gamelan |
| F02-K11-86 | A#7 | ROM, 398 Gamelan |
| F02-K11-87 | B7 | ROM, 398 Gamelan ROM, 398 Gamelan |
| F02-K11-88 | C8 | ROM, 398 Gamelan ROM, 398 Gamelan |
| F02-K11-89 | D8 |  |

### F02-K12 - Cymbals Catalog (kit 011, D1 p22)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K12-01 | A0 |  |
| F02-K12-02 | A#0 |  |
| F02-K12-03 | B0 |  |
| F02-K12-04 | C1 |  |
| F02-K12-05 | C#1 |  |
| F02-K12-06 | D1 |  |
| F02-K12-07 | D#1 |  |
| F02-K12-08 | E1 | ROM, 258 Maracas-Push |
| F02-K12-09 | F1 | ROM, 259 Maracas-Pull |
| F02-K12-10 | F#1 | ROM, 302 Caxixi-Hard |
| F02-K12-11 | G1 | ROM, 202 Dance Perc-Hip |
| F02-K12-12 | G#1 | ROM, 288 Tambourine-Push |
| F02-K12-13 | A1 | ROM, 289 Tambourine-Pull |
| F02-K12-14 | A#1 | ROM, 290 Tambourine-Acc.1 |
| F02-K12-15 | B1 | ROM, 291 Tambourine-Acc.2 |
| F02-K12-16 | C2 | ROM, 24 BD-Hip 3 ROM, 24 BD-Hip 3 |
| F02-K12-17 | C#2 | ROM, 125 H.Hat 2-Closed 1 |
| F02-K12-18 | D2 | ROM, 93 SD-Chili ROM, 93 SD-Chili |
| F02-K12-19 | D#2 | ROM, 126 H.Hat 2-Closed 2 |
| F02-K12-20 | E2 | ROM, 118 H.Hat 1-Closed 1 |
| F02-K12-21 | F2 | ROM, 119 H.Hat 1-Closed 2 |
| F02-K12-22 | F#2 | ROM, 130 H.Hat 2-Open 2 |
| F02-K12-23 | G2 | ROM, 123 H.Hat 1-Open 2 |
| F02-K12-24 | G#2 | ROM, 129 H.Hat 2-Open 1 |
| F02-K12-25 | A2 | ROM, 122 H.Hat 1-Open 1 |
| F02-K12-26 | A#2 | ROM, 131 H.Hat 2-Sizzle |
| F02-K12-27 | B2 | ROM, 124 H.Hat 1-Sizzle |
| F02-K12-28 | C3 | ROM, 120 H.Hat 1-Foot |
| F02-K12-29 | C#3 | ROM, 127 H.Hat 2-Foot |
| F02-K12-30 | D3 | ROM, 121 H.Hat 1-FootOpen |
| F02-K12-31 | D#3 | ROM, 128 H.Hat 2-FootOpen |
| F02-K12-32 | E3 | ROM, 132 H.Hat-Chili |
| F02-K12-33 | F3 | ROM, 136 H.Hat-Grange |
| F02-K12-34 | F#3 | ROM, 133 H.Hat-Crisp Cl 1 |
| F02-K12-35 | G3 | ROM, 137 H.Hat-Vintage 1 |
| F02-K12-36 | G#3 | ROM, 134 H.Hat-Crisp Cl 2 |
| F02-K12-37 | A3 | ROM, 138 H.Hat-Vintage 2 |
| F02-K12-38 | A#3 | ROM, 135 H.Hat-Crisp Open |
| F02-K12-39 | B3 | ROM, 139 H.Hat-Tight 1 |
| F02-K12-40 | C4 | ROM, 140 H.Hat-Tight 2 |
| F02-K12-41 | C#4 | ROM, 144 H.Hat-Old Open |
| F02-K12-42 | D4 | ROM, 141 H.Hat-Old Cl 1 |
| F02-K12-43 | D#4 | ROM, 149 H.Hat-ClangyOpen |
| F02-K12-44 | E4 | ROM, 142 H.Hat-Old Cl 2 |
| F02-K12-45 | F4 | ROM, 143 H.Hat-Old Cl 3 |
| F02-K12-46 | F#4 | ROM, 151 H.Hat-Alpo Close |
| F02-K12-47 | G4 | ROM, 147 H.Hat-AmbCrackle |
| F02-K12-48 | G#4 | ROM, 150 H.Hat-Whispy |
| F02-K12-49 | A4 | ROM, 148 H.Hat-Ambi |
| F02-K12-50 | A#4 | ROM, 146 H.Hat-Hip 2 |
| F02-K12-51 | B4 | ROM, 145 H.Hat-Hip 1 |
| F02-K12-52 | C5 | ROM, 189 99-HHat Closed |
| F02-K12-53 | C#5 | ROM, 309 Sleigh Bell |
| F02-K12-54 | D5 | ROM, 190 99-HHat Open |
| F02-K12-55 | D#5 | ROM, 310 Rap Sleighbell |
| F02-K12-56 | E5 | ROM, 187 88-HHat Closed |
| F02-K12-57 | F5 | ROM, 188 88-HHat Open |
| F02-K12-58 | F#5 | ROM, 154 Crash Cymbal 3 |
| F02-K12-59 | G5 | ROM, 191 88-Crash |
| F02-K12-60 | G#5 | ROM, 152 Crash Cymbal 1 |
| F02-K12-61 | A5 | ROM, 155 China Cymbal |
| F02-K12-62 | A#5 | ROM, 153 Crash Cymbal 2 |
| F02-K12-63 | B5 | ROM, 156 Splash Cymbal |
| F02-K12-64 | C6 | ROM, 160 Ride-Dance 99 |
| F02-K12-65 | C#6 | ROM, 158 Crash-Dance 99 |
| F02-K12-66 | D6 | ROM, 161 Ride-Dance DDD1 |
| F02-K12-67 | D#6 | ROM, 159 Crash-Dance DDD1 |
| F02-K12-68 | E6 | ROM, 162 Ride-Edge 1 |
| F02-K12-69 | F6 | ROM, 163 Ride-Edge 2 |
| F02-K12-70 | F#6 | ROM, 164 Ride-Jazz |
| F02-K12-71 | G6 | ROM, 165 Ride-Cup |
| F02-K12-72 | G#6 | ROM, 157 Cymbal Reverse |
| F02-K12-73 | A6 | ROM, 156 Splash Cymbal |
| F02-K12-74 | A#6 | ROM, 158 Crash-Dance 99 |
| F02-K12-75 | B6 | ROM, 160 Ride-Dance 99 |
| F02-K12-76 | C7 | ROM, 165 Ride-Cup |
| F02-K12-77 | C#7 | ROM, 190 99-HHat Open |
| F02-K12-78 | D7 | ROM, 142 H.Hat-Old Cl 2 |
| F02-K12-79 | D#7 | ROM, 143 H.Hat-Old Cl 3 |
| F02-K12-80 | E7 | ROM, 134 H.Hat-Crisp Cl 2 |
| F02-K12-81 | F7 | ROM, 189 99-HHat Closed |
| F02-K12-82 | F#7 | ROM, 125 H.Hat 2-Closed 1 |
| F02-K12-83 | G7 | ROM, 187 88-HHat Closed |
| F02-K12-84 | G#7 |  |
| F02-K12-85 | A7 |  |
| F02-K12-86 | A#7 |  |
| F02-K12-87 | B7 |  |
| F02-K12-88 | C8 |  |
| F02-K12-89 | D8 |  |

### F02-K13 - BD&SD Catalog 1 (kit 012, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K13-01 | A0 |  |
| F02-K13-02 | A#0 |  |
| F02-K13-03 | B0 |  |
| F02-K13-04 | C1 |  |
| F02-K13-05 | C#1 |  |
| F02-K13-06 | D1 |  |
| F02-K13-07 | D#1 |  |
| F02-K13-08 | E1 |  |
| F02-K13-09 | F1 |  |
| F02-K13-10 | F#1 |  |
| F02-K13-11 | G1 |  |
| F02-K13-12 | G#1 |  |
| F02-K13-13 | A1 |  |
| F02-K13-14 | A#1 |  |
| F02-K13-15 | B1 |  |
| F02-K13-16 | C2 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K13-17 | C#2 | ROM, 1 BD-Dry 2 ROM, 1 BD-Dry 2 |
| F02-K13-18 | D2 | ROM, 2 BD-Dry 3 ROM, 2 BD-Dry 3 |
| F02-K13-19 | D#2 | ROM, 3 BD-Dry 4 ROM, 3 BD-Dry 4 |
| F02-K13-20 | E2 | ROM, 4 BD-Dry 5 ROM, 4 BD-Dry 5 |
| F02-K13-21 | F2 | ROM, 6 BD-Soft ROM, 6 BD-Soft |
| F02-K13-22 | F#2 | ROM, 7 BD-Soft Room ROM, 7 BD-Soft Room |
| F02-K13-23 | G2 | ROM, 8 BD-Jazz 1 ROM, 8 BD-Jazz 1 |
| F02-K13-24 | G#2 | ROM, 9 BD-Jazz 2 ROM, 9 BD-Jazz 2 |
| F02-K13-25 | A2 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K13-26 | A#2 | ROM, 11 BD-Woofer ROM, 11 BD-Woofer |
| F02-K13-27 | B2 | ROM, 12 BD-Mondo Kill ROM, 12 BD-Mondo Kill |
| F02-K13-28 | C3 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K13-29 | C#3 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K13-30 | D3 | ROM, 15 BD-Gated ROM, 15 BD-Gated |
| F02-K13-31 | D#3 | ROM, 16 BD-Tight ROM, 16 BD-Tight |
| F02-K13-32 | E3 | ROM, 17 BD-Squash ROM, 17 BD-Squash |
| F02-K13-33 | F3 | ROM, 20 BD-Dance 3 ROM, 20 BD-Dance 3 |
| F02-K13-34 | F#3 | ROM, 26 BD-Dark ROM, 26 BD-Dark |
| F02-K13-35 | G3 | ROM, 28 BD-Ringy ROM, 28 BD-Ringy |
| F02-K13-36 | G#3 | ROM, 34 BD-Dance Soft ROM, 34 BD-Dance Soft |
| F02-K13-37 | A3 | ROM, 37 BD-PhatJaz ROM, 37 BD-PhatJaz |
| F02-K13-38 | A#3 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K13-39 | B3 | ROM, 40 SD-Dry 1 ROM, 40 SD-Dry 1 |
| F02-K13-40 | C4 | ROM, 41 SD-Dry 2 ROM, 41 SD-Dry 2 |
| F02-K13-41 | C#4 | ROM, 42 SD-Dry 3 ROM, 42 SD-Dry 3 |
| F02-K13-42 | D4 | ROM, 43 SD-Normal ROM, 43 SD-Normal |
| F02-K13-43 | D#4 | ROM, 44 SD-GhostSet roll ROM, 44 SD-GhostSet roll |
| F02-K13-44 | E4 | ROM, 47 SD-GhostSet p2 ROM, 47 SD-GhostSet p2 |
| F02-K13-45 | F4 | ROM, 46 SD-GhostSet p1 ROM, 46 SD-GhostSet p1 |
| F02-K13-46 | F#4 | ROM, 45 SD-GhostSet f ROM, 45 SD-GhostSet f |
| F02-K13-47 | G4 | ROM, 48 SD-Full Room ROM, 48 SD-Full Room |
| F02-K13-48 | G#4 | ROM, 49 SD-Off Center ROM, 49 SD-Off Center |
| F02-K13-49 | A4 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K13-50 | A#4 | ROM, 51 SD-Amb.Piccolo 1 ROM, 51 SD-Amb.Piccolo 1 |
| F02-K13-51 | B4 | ROM, 52 SD-Amb.Piccolo 2 ROM, 52 SD-Amb.Piccolo 2 |
| F02-K13-52 | C5 | ROM, 53 SD-Paper ROM, 53 SD-Paper |
| F02-K13-53 | C#5 | ROM, 54 SD-Classic Room ROM, 54 SD-Classic Room |
| F02-K13-54 | D5 | ROM, 55 SD-Brush Hit ROM, 55 SD-Brush Hit |
| F02-K13-55 | D#5 | ROM, 56 SD-Brush Tap 1 ROM, 56 SD-Brush Tap 1 |
| F02-K13-56 | E5 | ROM, 57 SD-Brush Tap 2 ROM, 57 SD-Brush Tap 2 |
| F02-K13-57 | F5 | ROM, 58 SD-Brush Swirl ROM, 58 SD-Brush Swirl |
| F02-K13-58 | F#5 | ROM, 59 SD-Atomic ROM, 59 SD-Atomic |
| F02-K13-59 | G5 | ROM, 60 SD-Big Rock ROM, 60 SD-Big Rock |
| F02-K13-60 | G#5 | ROM, 61 SD-Yowie ROM, 61 SD-Yowie |
| F02-K13-61 | A5 | ROM, 62 SD-Processed 1 ROM, 62 SD-Processed 1 |
| F02-K13-62 | A#5 | ROM, 63 SD-Processed 2 ROM, 63 SD-Processed 2 |
| F02-K13-63 | B5 | ROM, 64 SD-Cracker Room ROM, 64 SD-Cracker Room |
| F02-K13-64 | C6 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K13-65 | C#6 | ROM, 65 Sidestick-Dry ROM, 65 Sidestick-Dry |
| F02-K13-66 | D6 | ROM, 171 SD-Orch.Roll 2 ROM, 171 SD-Orch.Roll 2 |
| F02-K13-67 | D#6 | ROM, 152 Crash Cymbal 1 ROM, 152 Crash Cymbal 1 |
| F02-K13-68 | E6 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K13-69 | F6 | ROM, 73 SD-Reverse ROM, 73 SD-Reverse |
| F02-K13-70 | F#6 | ROM, 118 H.Hat 1-Closed 1 ROM, 118 H.Hat 1-Closed 1 |
| F02-K13-71 | G6 | ROM, 78 SD-Hip 5 ROM, 78 SD-Hip 5 |
| F02-K13-72 | G#6 | ROM, 120 H.Hat 1-Foot ROM, 120 H.Hat 1-Foot |
| F02-K13-73 | A6 | ROM, 91 SD-AmbiHop ROM, 91 SD-AmbiHop |
| F02-K13-74 | A#6 | ROM, 122 H.Hat 1-Open 1 ROM, 122 H.Hat 1-Open 1 |
| F02-K13-75 | B6 | ROM, 95 SD-Whopper ROM, 95 SD-Whopper |
| F02-K13-76 | C7 | ROM, 97 SD-Ambi Pop ROM, 97 SD-Ambi Pop |
| F02-K13-77 | C#7 |  |
| F02-K13-78 | D7 |  |
| F02-K13-79 | D#7 |  |
| F02-K13-80 | E7 |  |
| F02-K13-81 | F7 |  |
| F02-K13-82 | F#7 |  |
| F02-K13-83 | G7 |  |
| F02-K13-84 | G#7 |  |
| F02-K13-85 | A7 |  |
| F02-K13-86 | A#7 |  |
| F02-K13-87 | B7 |  |
| F02-K13-88 | C8 |  |
| F02-K13-89 | C#8 |  |
| F02-K13-90 | D8 |  |

### F02-K14 - BD&SD Catalog 2 (kit 013, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K14-01 | A0 |  |
| F02-K14-02 | A#0 |  |
| F02-K14-03 | B0 |  |
| F02-K14-04 | C1 |  |
| F02-K14-05 | C#1 |  |
| F02-K14-06 | D1 |  |
| F02-K14-07 | D#1 |  |
| F02-K14-08 | E1 |  |
| F02-K14-09 | F1 |  |
| F02-K14-10 | F#1 |  |
| F02-K14-11 | G1 |  |
| F02-K14-12 | G#1 |  |
| F02-K14-13 | A1 |  |
| F02-K14-14 | A#1 |  |
| F02-K14-15 | B1 |  |
| F02-K14-16 | C2 | ROM, 5 BD-Dry 6 ROM, 5 BD-Dry 6 |
| F02-K14-17 | C#2 | ROM, 18 BD-Dance 1 ROM, 18 BD-Dance 1 |
| F02-K14-18 | D2 | ROM, 19 BD-Dance 2 ROM, 19 BD-Dance 2 |
| F02-K14-19 | D#2 | ROM, 178 99-BD 3 ROM, 178 99-BD 3 |
| F02-K14-20 | E2 | ROM, 21 BD-Dance 4 ROM, 21 BD-Dance 4 |
| F02-K14-21 | F2 | ROM, 22 BD-Hip 1 ROM, 22 BD-Hip 1 |
| F02-K14-22 | F#2 | ROM, 23 BD-Hip 2 ROM, 23 BD-Hip 2 |
| F02-K14-23 | G2 | ROM, 24 BD-Hip 3 ROM, 24 BD-Hip 3 |
| F02-K14-24 | G#2 | ROM, 25 BD-Hip 4 ROM, 25 BD-Hip 4 |
| F02-K14-25 | A2 | ROM, 27 BD-Pop ROM, 27 BD-Pop |
| F02-K14-26 | A#2 | ROM, 29 BD-Short88 ROM, 29 BD-Short88 |
| F02-K14-27 | B2 | ROM, 30 BD-Dance99 ROM, 30 BD-Dance99 |
| F02-K14-28 | C3 | ROM, 31 BD-Ambi ROM, 31 BD-Ambi |
| F02-K14-29 | C#3 | ROM, 32 BD-Ambi Crackle ROM, 32 BD-Ambi Crackle |
| F02-K14-30 | D3 | ROM, 33 BD-Ambi Soft ROM, 33 BD-Ambi Soft |
| F02-K14-31 | D#3 | ROM, 35 BD-AmbiRocker ROM, 35 BD-AmbiRocker |
| F02-K14-32 | E3 | ROM, 36 BD-Chili ROM, 36 BD-Chili |
| F02-K14-33 | F3 | ROM, 180 99-BD 5 ROM, 180 99-BD 5 |
| F02-K14-34 | F#3 | ROM, 175 88-BD 3 ROM, 175 88-BD 3 |
| F02-K14-35 | G3 | ROM, 38 BD-Cannon ROM, 38 BD-Cannon |
| F02-K14-36 | G#3 | ROM, 39 BD-Klanger ROM, 39 BD-Klanger |
| F02-K14-37 | A3 | ROM, 173 88-BD 1(Rap) ROM, 173 88-BD 1(Rap) |
| F02-K14-38 | A#3 | ROM, 174 88-BD 2(R&B) ROM, 174 88-BD 2(R&B) |
| F02-K14-39 | B3 | ROM, 176 99-BD 1 ROM, 176 99-BD 1 |
| F02-K14-40 | C4 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K14-41 | C#4 | ROM, 179 99-BD 4 ROM, 179 99-BD 4 |
| F02-K14-42 | D4 | ROM, 67 SD-Amb.House ROM, 67 SD-Amb.House |
| F02-K14-43 | D#4 | ROM, 68 SD-Dance ROM, 68 SD-Dance |
| F02-K14-44 | E4 | ROM, 69 SD-Rap 1 ROM, 69 SD-Rap 1 |
| F02-K14-45 | F4 | ROM, 70 SD-Rap 2 ROM, 70 SD-Rap 2 |
| F02-K14-46 | F#4 | ROM, 71 SD-Noise ROM, 71 SD-Noise |
| F02-K14-47 | G4 | ROM, 72 SD-Sizzle ROM, 72 SD-Sizzle |
| F02-K14-48 | G#4 | ROM, 74 SD-Hip 1 ROM, 74 SD-Hip 1 |
| F02-K14-49 | A4 | ROM, 75 SD-Hip 2 ROM, 75 SD-Hip 2 |
| F02-K14-50 | A#4 | ROM, 76 SD-Hip 3 ROM, 76 SD-Hip 3 |
| F02-K14-51 | B4 | ROM, 77 SD-Hip 4 ROM, 77 SD-Hip 4 |
| F02-K14-52 | C5 | ROM, 79 SD-Hip 6 ROM, 79 SD-Hip 6 |
| F02-K14-53 | C#5 | ROM, 80 SD-Ringy ROM, 80 SD-Ringy |
| F02-K14-54 | D5 | ROM, 81 SD-Tiny ROM, 81 SD-Tiny |
| F02-K14-55 | D#5 | ROM, 82 SD-Vintage 1 ROM, 82 SD-Vintage 1 |
| F02-K14-56 | E5 | ROM, 83 SD-Vintage 2 ROM, 83 SD-Vintage 2 |
| F02-K14-57 | F5 | ROM, 84 SD-Vintage 3 ROM, 84 SD-Vintage 3 |
| F02-K14-58 | F#5 | ROM, 85 SD-Vintage 4 ROM, 85 SD-Vintage 4 |
| F02-K14-59 | G5 | ROM, 86 SD-Vintage 5 ROM, 86 SD-Vintage 5 |
| F02-K14-60 | G#5 | ROM, 87 SD-Vintage 6 ROM, 87 SD-Vintage 6 |
| F02-K14-61 | A5 | ROM, 88 SD-AmbiCrackle 1 ROM, 88 SD-AmbiCrackle 1 |
| F02-K14-62 | A#5 | ROM, 89 SD-AmbiCrackle 2 ROM, 89 SD-AmbiCrackle 2 |
| F02-K14-63 | B5 | ROM, 90 SD-AmbiCrackle 3 ROM, 90 SD-AmbiCrackle 3 |
| F02-K14-64 | C6 | ROM, 92 SD-Brasser ROM, 92 SD-Brasser |
| F02-K14-65 | C#6 | ROM, 93 SD-Chili ROM, 93 SD-Chili |
| F02-K14-66 | D6 | ROM, 94 SD-Klanger ROM, 94 SD-Klanger |
| F02-K14-67 | D#6 | ROM, 96 SD-Dry Present ROM, 96 SD-Dry Present |
| F02-K14-68 | E6 | ROM, 98 SD-Ambi Hit 1 ROM, 98 SD-Ambi Hit 1 |
| F02-K14-69 | F6 | ROM, 99 SD-Ambi Hit 2 ROM, 99 SD-Ambi Hit 2 |
| F02-K14-70 | F#6 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K14-71 | G6 | ROM, 100 SD-Ambi Hit 3 ROM, 100 SD-Ambi Hit 3 |
| F02-K14-72 | G#6 | ROM, 189 99-HHat Closed ROM, 189 99-HHat Closed |
| F02-K14-73 | A6 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K14-74 | A#6 | ROM, 190 99-HHat Open ROM, 190 99-HHat Open |
| F02-K14-75 | B6 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K14-76 | C7 | ROM, 183 99-SD 1 ROM, 183 99-SD 1 |
| F02-K14-77 | C#7 | ROM, 184 99-SD 2 ROM, 184 99-SD 2 |
| F02-K14-78 | D7 | ROM, 199 Vocal SD ROM, 199 Vocal SD |
| F02-K14-79 | D#7 |  |
| F02-K14-80 | E7 |  |
| F02-K14-81 | F7 |  |
| F02-K14-82 | F#7 |  |
| F02-K14-83 | G7 |  |
| F02-K14-84 | G#7 |  |
| F02-K14-85 | A7 |  |
| F02-K14-86 | A#7 |  |
| F02-K14-87 | B7 |  |
| F02-K14-88 | C8 |  |
| F02-K14-89 | C#8 |  |
| F02-K14-90 | D8 |  |

### F02-K15 - 01/W Total Kit (kit 014, D1 p23)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K15-01 | A0 | ROM, 311 Finger Cymbal |
| F02-K15-02 | A#0 | ROM, 295 Cuica-Hi ROM, 296 Cuica-Lo |
| F02-K15-03 | B0 | ROM, 291 Tambourine-Acc.2 |
| F02-K15-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K15-05 | C#1 | ROM, 252 Castanet-Single ROM, 253 Castanet-Dble 1 |
| F02-K15-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K15-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K15-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K15-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K15-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K15-11 | G1 | ROM, 263 Baya-Mute 2 ROM, 264 Baya-Mute 3 |
| F02-K15-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K15-13 | A1 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K15-14 | A#1 | ROM, 64 SD-Cracker Room ROM, 73 SD-Reverse |
| F02-K15-15 | B1 | ROM, 9 BD-Jazz 2 |
| F02-K15-16 | C2 | ROM, 11 BD-Woofer ROM, 11 BD-Woofer |
| F02-K15-17 | C#2 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K15-18 | D2 | ROM, 12 BD-Mondo Kill ROM, 12 BD-Mondo Kill |
| F02-K15-19 | D#2 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K15-20 | E2 | ROM, 174 88-BD 2(R&B) ROM, 173 88-BD 1(Rap) |
| F02-K15-21 | F2 | ROM, 48 SD-Full Room ROM, 48 SD-Full Room |
| F02-K15-22 | F#2 | ROM, 72 SD-Sizzle |
| F02-K15-23 | G2 | ROM, 59 SD-Atomic ROM, 59 SD-Atomic |
| F02-K15-24 | G#2 | ROM, 55 SD-Brush Hit ROM, 56 SD-Brush Tap 1 |
| F02-K15-25 | A2 | ROM, 183 99-SD 1 ROM, 183 99-SD 1 |
| F02-K15-26 | A#2 | ROM, 66 Sidestick-Amb ROM, 66 Sidestick-Amb |
| F02-K15-27 | B2 | ROM, 182 88-SD 2(Old) ROM, 181 88-SD 1(New) |
| F02-K15-28 | C3 | ROM, 103 Tom 1-Floor ROM, 103 Tom 1-Floor |
| F02-K15-29 | C#3 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K15-30 | D3 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K15-31 | D#3 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K15-32 | E3 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K15-33 | F3 | ROM, 119 H.Hat 1-Closed 2 ROM, 118 H.Hat 1-Closed 1 |
| F02-K15-34 | F#3 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K15-35 | G3 | ROM, 123 H.Hat 1-Open 2 ROM, 119 H.Hat 1-Closed 2 |
| F02-K15-36 | G#3 | ROM, 188 88-HHat Open ROM, 187 88-HHat Closed |
| F02-K15-37 | A3 | ROM, 122 H.Hat 1-Open 1 |
| F02-K15-38 | A#3 | ROM, 190 99-HHat Open |
| F02-K15-39 | B3 | ROM, 121 H.Hat 1-FootOpen ROM, 120 H.Hat 1-Foot |
| F02-K15-40 | C4 | ROM, 152 Crash Cymbal 1 |
| F02-K15-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K15-42 | D4 | ROM, 155 China Cymbal |
| F02-K15-43 | D#4 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K15-44 | E4 | ROM, 165 Ride-Cup |
| F02-K15-45 | F4 | ROM, 297 Shaker 1 ROM, 298 Shaker 2 |
| F02-K15-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 288 Tambourine-Push |
| F02-K15-47 | G4 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K15-48 | G#4 | ROM, 293 Triangle-Mute |
| F02-K15-49 | A4 | ROM, 292 Triangle-Open |
| F02-K15-50 | A#4 | ROM, 286 Timbale Hi-Rim 2 ROM, 284 Timbale Hi-Edge |
| F02-K15-51 | B4 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K15-52 | C5 | ROM, 242 Bongo Lo-Open ROM, 242 Bongo Lo-Open |
| F02-K15-53 | C#5 | ROM, 246 Bongo Hi-Slap ROM, 245 Bongo Hi-Open |
| F02-K15-54 | D5 | ROM, 232 Conga Lo-Open ROM, 232 Conga Lo-Open |
| F02-K15-55 | D#5 | ROM, 235 Conga Hi-Open ROM, 235 Conga Hi-Open |
| F02-K15-56 | E5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K15-57 | F5 | ROM, 305 Cowbell 1 |
| F02-K15-58 | F#5 | ROM, 229 Hand Claps |
| F02-K15-59 | G5 | ROM, 166 BD-Orchestral |
| F02-K15-60 | G#5 | ROM, 73 SD-Reverse |
| F02-K15-61 | A5 | ROM, 61 SD-Yowie |
| F02-K15-62 | A#5 | ROM, 171 SD-Orch.Roll 2 ROM, 170 SD-Orch.Roll 1 |
| F02-K15-63 | B5 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K15-64 | C6 | ROM, 168 Orch Cym.-Open ROM, 167 Orch Cym.-Closed |
| F02-K15-65 | C#6 | ROM, 309 Sleigh Bell |
| F02-K15-66 | D6 | ROM, 217 DJ-BD Rub ROM, 218 DJ-SD Rub |
| F02-K15-67 | D#6 | ROM, 219 DJ-Record Stop ROM, 220 DJ-Reverse |
| F02-K15-68 | E6 | ROM, 221 DJ-Old Record |
| F02-K15-69 | F6 | ROM, 391 Swish Terra ROM, 318 Industry |
| F02-K15-70 | F#6 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K15-71 | G6 | ROM, 223 Pizz Hit ROM, 223 Pizz Hit |
| F02-K15-72 | G#6 | ROM, 224 Band Hit ROM, 224 Band Hit |
| F02-K15-73 | A6 | ROM, 396 Noise-FM Mod ROM, 394 Air Vortex |
| F02-K15-74 | A#6 | ROM, 317 Chinese Gong |
| F02-K15-75 | B6 | ROM, 314 Bell Tree |
| F02-K15-76 | C7 | ROM, 312 Marc Tree ROM, 312 Marc Tree |
| F02-K15-77 | C#7 | ROM, 354 Stadium |
| F02-K15-78 | D7 | ROM, 355 Applause |
| F02-K15-79 | D#7 | ROM, 389 Telephone Ring ROM, 315 Flexatone |
| F02-K15-80 | E7 | ROM, 358 Footstep 1 ROM, 358 Footstep 1 |
| F02-K15-81 | F7 | ROM, 359 Footstep 2 ROM, 359 Footstep 2 |
| F02-K15-82 | F#7 | ROM, 369 Tribe |
| F02-K15-83 | G7 | ROM, 361 Bird 1 ROM, 361 Bird 1 |
| F02-K15-84 | G#7 | ROM, 362 Bird 2 ROM, 362 Bird 2 |
| F02-K15-85 | A7 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K15-86 | A#7 | ROM, 395 Noise-White ROM, 370 GunShot |
| F02-K15-87 | B7 | ROM, 393 Cricket Spectrum ROM, 393 Cricket Spectrum |
| F02-K15-88 | C8 | ROM, 390 Space Lore ROM, 390 Space Lore |
| F02-K15-89 | C#8 | 23 |
| F02-K15-90 | D8 |  |

### F02-K16 - 01/W Producer's (kit 015, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K16-01 | A0 | ROM, 311 Finger Cymbal |
| F02-K16-02 | A#0 | ROM, 295 Cuica-Hi ROM, 296 Cuica-Lo |
| F02-K16-03 | B0 | ROM, 291 Tambourine-Acc.2 |
| F02-K16-04 | C1 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K16-05 | C#1 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K16-06 | D1 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K16-07 | D#1 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K16-08 | E1 | ROM, 179 99-BD 4 ROM, 179 99-BD 4 |
| F02-K16-09 | F1 | ROM, 176 99-BD 1 ROM, 176 99-BD 1 |
| F02-K16-10 | F#1 | ROM, 173 88-BD 1(Rap) ROM, 173 88-BD 1(Rap) |
| F02-K16-11 | G1 | ROM, 20 BD-Dance 3 ROM, 20 BD-Dance 3 |
| F02-K16-12 | G#1 | ROM, 19 BD-Dance 2 ROM, 19 BD-Dance 2 |
| F02-K16-13 | A1 | ROM, 18 BD-Dance 1 ROM, 18 BD-Dance 1 |
| F02-K16-14 | A#1 | ROM, 11 BD-Woofer ROM, 11 BD-Woofer |
| F02-K16-15 | B1 | ROM, 8 BD-Jazz 1 ROM, 8 BD-Jazz 1 |
| F02-K16-16 | C2 | ROM, 16 BD-Tight ROM, 16 BD-Tight |
| F02-K16-17 | C#2 | ROM, 7 BD-Soft Room ROM, 7 BD-Soft Room |
| F02-K16-18 | D2 | ROM, 17 BD-Squash ROM, 17 BD-Squash |
| F02-K16-19 | D#2 | ROM, 1 BD-Dry 2 ROM, 1 BD-Dry 2 |
| F02-K16-20 | E2 | ROM, 6 BD-Soft ROM, 6 BD-Soft |
| F02-K16-21 | F2 | ROM, 3 BD-Dry 4 ROM, 3 BD-Dry 4 |
| F02-K16-22 | F#2 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K16-23 | G2 | ROM, 15 BD-Gated ROM, 15 BD-Gated |
| F02-K16-24 | G#2 | ROM, 0 BD-Dry 1 ROM, 0 BD-Dry 1 |
| F02-K16-25 | A2 | ROM, 10 BD-Pillow ROM, 10 BD-Pillow |
| F02-K16-26 | A#2 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K16-27 | B2 | ROM, 2 BD-Dry 3 ROM, 2 BD-Dry 3 |
| F02-K16-28 | C3 | ROM, 12 BD-Mondo Kill ROM, 12 BD-Mondo Kill |
| F02-K16-29 | C#3 | ROM, 9 BD-Jazz 2 ROM, 9 BD-Jazz 2 |
| F02-K16-30 | D3 | ROM, 174 88-BD 2(R&B) ROM, 174 88-BD 2(R&B) |
| F02-K16-31 | D#3 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K16-32 | E3 | ROM, 51 SD-Amb.Piccolo 1 ROM, 51 SD-Amb.Piccolo 1 |
| F02-K16-33 | F3 | ROM, 41 SD-Dry 2 ROM, 41 SD-Dry 2 |
| F02-K16-34 | F#3 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K16-35 | G3 | ROM, 40 SD-Dry 1 ROM, 40 SD-Dry 1 |
| F02-K16-36 | G#3 | ROM, 42 SD-Dry 3 ROM, 42 SD-Dry 3 |
| F02-K16-37 | A3 | ROM, 60 SD-Big Rock ROM, 60 SD-Big Rock |
| F02-K16-38 | A#3 | ROM, 62 SD-Processed 1 ROM, 62 SD-Processed 1 |
| F02-K16-39 | B3 | ROM, 49 SD-Off Center ROM, 49 SD-Off Center |
| F02-K16-40 | C4 | ROM, 65 Sidestick-Dry ROM, 65 Sidestick-Dry |
| F02-K16-41 | C#4 | ROM, 73 SD-Reverse |
| F02-K16-42 | D4 | ROM, 61 SD-Yowie ROM, 61 SD-Yowie |
| F02-K16-43 | D#4 | ROM, 170 SD-Orch.Roll 1 |
| F02-K16-44 | E4 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K16-45 | F4 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K16-46 | F#4 | ROM, 184 99-SD 2 ROM, 184 99-SD 2 |
| F02-K16-47 | G4 | ROM, 52 SD-Amb.Piccolo 2 ROM, 52 SD-Amb.Piccolo 2 |
| F02-K16-48 | G#4 | ROM, 63 SD-Processed 2 ROM, 63 SD-Processed 2 |
| F02-K16-49 | A4 | ROM, 48 SD-Full Room ROM, 48 SD-Full Room |
| F02-K16-50 | A#4 | ROM, 218 DJ-SD Rub ROM, 218 DJ-SD Rub |
| F02-K16-51 | B4 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K16-52 | C5 | ROM, 126 H.Hat 2-Closed 2 ROM, 125 H.Hat 2-Closed 1 |
| F02-K16-53 | C#5 | ROM, 126 H.Hat 2-Closed 2 ROM, 125 H.Hat 2-Closed 1 |
| F02-K16-54 | D5 | ROM, 129 H.Hat 2-Open 1 ROM, 130 H.Hat 2-Open 2 |
| F02-K16-55 | D#5 | ROM, 128 H.Hat 2-FootOpen ROM, 127 H.Hat 2-Foot |
| F02-K16-56 | E5 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K16-57 | F5 | ROM, 188 88-HHat Open |
| F02-K16-58 | F#5 | ROM, 152 Crash Cymbal 1 |
| F02-K16-59 | G5 | ROM, 153 Crash Cymbal 2 |
| F02-K16-60 | G#5 | ROM, 153 Crash Cymbal 2 |
| F02-K16-61 | A5 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K16-62 | A#5 | ROM, 156 Splash Cymbal |
| F02-K16-63 | B5 | ROM, 163 Ride-Edge 2 ROM, 163 Ride-Edge 2 |
| F02-K16-64 | C6 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K16-65 | C#6 | ROM, 103 Tom 1-Floor ROM, 103 Tom 1-Floor |
| F02-K16-66 | D6 | ROM, 102 Tom 1-Lo ROM, 102 Tom 1-Lo |
| F02-K16-67 | D#6 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K16-68 | E6 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K16-69 | F6 | ROM, 101 Tom 1-Hi ROM, 101 Tom 1-Hi |
| F02-K16-70 | F#6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K16-71 | G6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K16-72 | G#6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K16-73 | A6 | ROM, 110 Tom-Processed ROM, 110 Tom-Processed |
| F02-K16-74 | A#6 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-75 | B6 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-76 | C7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-77 | C#7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-78 | D7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-79 | D#7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-80 | E7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-81 | F7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-82 | F#7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-83 | G7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-84 | G#7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-85 | A7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-86 | A#7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-87 | B7 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-88 | C8 | ROM, 198 Real E.Tom ROM, 198 Real E.Tom |
| F02-K16-89 | D8 |  |

### F02-K17 - Hard Core Kit (kit 016, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K17-01 | A0 |  |
| F02-K17-02 | A#0 |  |
| F02-K17-03 | B0 |  |
| F02-K17-04 | C1 | ROM, 30 BD-Dance99 |
| F02-K17-05 | C#1 | ROM, 19 BD-Dance 2 |
| F02-K17-06 | D1 | ROM, 32 BD-Ambi Crackle |
| F02-K17-07 | D#1 | ROM, 28 BD-Ringy |
| F02-K17-08 | E1 | ROM, 5 BD-Dry 6 |
| F02-K17-09 | F1 | ROM, 13 BD-Terminator |
| F02-K17-10 | F#1 | ROM, 24 BD-Hip 3 |
| F02-K17-11 | G1 | ROM, 74 SD-Hip 1 |
| F02-K17-12 | G#1 | ROM, 79 SD-Hip 6 |
| F02-K17-13 | A1 | ROM, 81 SD-Tiny |
| F02-K17-14 | A#1 | Synth, 48 SD-EXB09 09 |
| F02-K17-15 | B1 | ROM, 87 SD-Vintage 6 |
| F02-K17-16 | C2 | Synth, 37 BD-Jungle-EXB09 Synth, 37 BD-Jungle-EXB09 |
| F02-K17-17 | C#2 | ROM, 2 BD-Dry 3 |
| F02-K17-18 | D2 | Synth, 26 BD-EXB09 27 Synth, 26 BD-EXB09 27 |
| F02-K17-19 | D#2 | Synth, 9 BD-EXB09 10 Synth, 9 BD-EXB09 10 |
| F02-K17-20 | E2 | Best, 43 BD-EXB04 043 Best, 43 BD-EXB04 043 |
| F02-K17-21 | F2 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K17-22 | F#2 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K17-23 | G2 | ROM, 96 SD-Dry Present |
| F02-K17-24 | G#2 | ROM, 84 SD-Vintage 3 |
| F02-K17-25 | A2 | ROM, 44 SD-GhostSet roll ROM, 44 SD-GhostSet roll |
| F02-K17-26 | A#2 | Best, 67 BD-EXB04 067 Best, 67 BD-EXB04 067 |
| F02-K17-27 | B2 | ROM, 28 BD-Ringy Best, 92 BD-EXB04 096 |
| F02-K17-28 | C3 | Best, 81 BD-EXB04 082 Best, 81 BD-EXB04 082 |
| F02-K17-29 | C#3 | ROM, 50 SD-Jazz Ring |
| F02-K17-30 | D3 | Best, 105 SD-EXB04 04 Best, 105 SD-EXB04 04 |
| F02-K17-31 | D#3 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K17-32 | E3 | Best, 130 SD-EXB04 29 Best, 131 SD-EXB04 30 |
| F02-K17-33 | F3 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K17-34 | F#3 | Best, 205 H.H.Cl-EXB04 34 Best, 205 H.H.Cl-EXB04 34 |
| F02-K17-35 | G3 | ROM, 250 Djembe-Mute ROM, 250 Djembe-Mute |
| F02-K17-36 | G#3 | Synth, 79 HH05-Ho-EXB09 |
| F02-K17-37 | A3 | ROM, 251 Djembe-Slap ROM, 251 Djembe-Slap |
| F02-K17-38 | A#3 | Synth, 80 HH05-Op-EXB09 |
| F02-K17-39 | B3 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K17-40 | C4 | ROM, 282 Timbale Lo-Mute ROM, 282 Timbale Lo-Mute |
| F02-K17-41 | C#4 | Synth, 88 CrashJungle-EXB9 Synth, 88 CrashJungle-EXB9 |
| F02-K17-42 | D4 | ROM, 273 Tabla-Mute 4 |
| F02-K17-43 | D#4 | ROM, 161 Ride-Dance DDD1 |
| F02-K17-44 | E4 | ROM, 154 Crash Cymbal 3 |
| F02-K17-45 | F4 | Best, 279 Cabasa-EXB04 01 Best, 279 Cabasa-EXB04 01 |
| F02-K17-46 | F#4 | Synth, 77 HH04-Op-EXB09 Synth, 75 HH04-Ho-EXB09 01 |
| F02-K17-47 | G4 | Best, 278 Cabasa-EXB04 00 Best, 278 Cabasa-EXB04 00 |
| F02-K17-48 | G#4 | Best, 280 Cabasa-EXB04 02 Best, 280 Cabasa-EXB04 02 |
| F02-K17-49 | A4 | Best, 268 Tamb-EXB04 00 |
| F02-K17-50 | A#4 | Best, 272 Tamb-EXB04 04 |
| F02-K17-51 | B4 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K17-52 | C5 | ROM, 271 Tabla-Mute 2 |
| F02-K17-53 | C#5 | ROM, 267 Tabla-Na |
| F02-K17-54 | D5 | ROM, 267 Tabla-Na |
| F02-K17-55 | D#5 | ROM, 269 Tabla-Tin |
| F02-K17-56 | E5 | ROM, 269 Tabla-Tin |
| F02-K17-57 | F5 | ROM, 261 Baya-Ghe |
| F02-K17-58 | F#5 | ROM, 260 Baya-Open |
| F02-K17-59 | G5 | ROM, 262 Baya-Mute 1 |
| F02-K17-60 | G#5 | ROM, 263 Baya-Mute 2 |
| F02-K17-61 | A5 | ROM, 264 Baya-Mute 3 |
| F02-K17-62 | A#5 | ROM, 265 Baya-Mute 4 |
| F02-K17-63 | B5 | ROM, 266 Baya-Mute 5 |
| F02-K17-64 | C6 | Best, 6 BD-EXB04 006 |
| F02-K17-65 | C#6 | ROM, 27 BD-Pop |
| F02-K17-66 | D6 | Best, 14 BD-EXB04 014 |
| F02-K17-67 | D#6 | Best, 7 BD-EXB04 007 |
| F02-K17-68 | E6 | Best, 79 BD-EXB04 079 |
| F02-K17-69 | F6 | Best, 135 SD-EXB04 34 |
| F02-K17-70 | F#6 | Best, 217 H.H.Ho-EXB04 04 |
| F02-K17-71 | G6 | ROM, 79 SD-Hip 6 |
| F02-K17-72 | G#6 | Synth, 88 CrashJungle-EXB9 |
| F02-K17-73 | A6 | ROM, 71 SD-Noise |
| F02-K17-74 | A#6 | Best, 234 H.H.Op-EXB04 04 |
| F02-K17-75 | B6 | Best, 171 H.H.Cl-EXB04 00 |
| F02-K17-76 | C7 | Synth, 160Vo-09 Ahhhh |
| F02-K17-77 | C#7 | Synth, 84 Ride-EXB09 02 Synth, 84 Ride-EXB09 02 |
| F02-K17-78 | D7 | Synth, 161Vo-09 Ohhhh |
| F02-K17-79 | D#7 | Synth, 206Vo-09 Oohh |
| F02-K17-80 | E7 | Synth, 210Vo-09 Uuh |
| F02-K17-81 | F7 | Synth, 234Vo-09 Be |
| F02-K17-82 | F#7 | Synth, 211Vo-09 Uh-a-ha |
| F02-K17-83 | G7 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K17-84 | G#7 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K17-85 | A7 | OrchS, 20 ViEns.RunUp-mi 3 |
| F02-K17-86 | A#7 | OrchS, 21 ViEns.RunUp-mi 4 |
| F02-K17-87 | B7 | ROM, 404 Power Chord |
| F02-K17-88 | C8 | ROM, 405 Fret Noise |
| F02-K17-89 | D8 |  |

### F02-K18 - ModernRockKit (kit 017, D1 p24)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K18-01 | A0 |  |
| F02-K18-02 | A#0 |  |
| F02-K18-03 | B0 |  |
| F02-K18-04 | C1 | ROM, 28 BD-Ringy |
| F02-K18-05 | C#1 | ROM, 15 BD-Gated |
| F02-K18-06 | D1 | Best, 43 BD-EXB04 043 |
| F02-K18-07 | D#1 | Best, 15 BD-EXB04 015 |
| F02-K18-08 | E1 | ROM, 28 BD-Ringy |
| F02-K18-09 | F1 | Synth, 13 BD-EXB09 14 |
| F02-K18-10 | F#1 | ROM, 35 BD-AmbiRocker |
| F02-K18-11 | G1 | Synth, 16 BD-EXB09 17 |
| F02-K18-12 | G#1 | ROM, 79 SD-Hip 6 ROM, 79 SD-Hip 6 |
| F02-K18-13 | A1 | ROM, 90 SD-AmbiCrackle 3 ROM, 90 SD-AmbiCrackle 3 |
| F02-K18-14 | A#1 | Best, 1 BD-EXB04 001 |
| F02-K18-15 | B1 | Synth, 47 SD-EXB09 08 |
| F02-K18-16 | C2 | Synth, 32 BD-EXB09 33 |
| F02-K18-17 | C#2 | ROM, 12 BD-Mondo Kill |
| F02-K18-18 | D2 | ROM, 13 BD-Terminator ROM, 13 BD-Terminator |
| F02-K18-19 | D#2 | Best, 7 BD-EXB04 007 |
| F02-K18-20 | E2 | Best, 14 BD-EXB04 014 |
| F02-K18-21 | F2 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K18-22 | F#2 | Best, 125 SD-EXB04 24 Best, 125 SD-EXB04 24 |
| F02-K18-23 | G2 | ROM, 98 SD-Ambi Hit 1 ROM, 98 SD-Ambi Hit 1 |
| F02-K18-24 | G#2 | ROM, 62 SD-Processed 1 ROM, 62 SD-Processed 1 |
| F02-K18-25 | A2 | ROM, 91 SD-AmbiHop |
| F02-K18-26 | A#2 | ROM, 50 SD-Jazz Ring ROM, 50 SD-Jazz Ring |
| F02-K18-27 | B2 | Best, 6 BD-EXB04 006 Best, 6 BD-EXB04 006 |
| F02-K18-28 | C3 | ROM, 24 BD-Hip 3 ROM, 24 BD-Hip 3 |
| F02-K18-29 | C#3 | ROM, 66 Sidestick-Amb |
| F02-K18-30 | D3 | ROM, 54 SD-Classic Room |
| F02-K18-31 | D#3 | ROM, 80 SD-Ringy |
| F02-K18-32 | E3 | Best, 117 SD-EXB04 16 Best, 117 SD-EXB04 16 |
| F02-K18-33 | F3 | ROM, 109 Tom 3-Floor |
| F02-K18-34 | F#3 | ROM, 125 H.Hat 2-Closed 1 |
| F02-K18-35 | G3 | ROM, 108 Tom 3-Lo |
| F02-K18-36 | G#3 | ROM, 126 H.Hat 2-Closed 2 |
| F02-K18-37 | A3 | ROM, 107 Tom 3-Hi |
| F02-K18-38 | A#3 | ROM, 142 H.Hat-Old Cl 2 ROM, 142 H.Hat-Old Cl 2 |
| F02-K18-39 | B3 | ROM, 137 H.Hat-Vintage 1 |
| F02-K18-40 | C4 | ROM, 147 H.Hat-AmbCrackle |
| F02-K18-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K18-42 | D4 | ROM, 142 H.Hat-Old Cl 2 |
| F02-K18-43 | D#4 | ROM, 165 Ride-Cup |
| F02-K18-44 | E4 | ROM, 155 China Cymbal |
| F02-K18-45 | F4 | Best, 389 Industry-142 |
| F02-K18-46 | F#4 | Best, 205 H.H.Cl-EXB04 34 |
| F02-K18-47 | G4 | Best, 379 Industry-Comp 2 |
| F02-K18-48 | G#4 | Best, 378 Industry-Comp 1 |
| F02-K18-49 | A4 | Best, 384 Hall Bound |
| F02-K18-50 | A#4 | Best, 372 Industry-Iron |
| F02-K18-51 | B4 | Best, 373 Industry-Press |
| F02-K18-52 | C5 | Best, 370 Industry-Tblsow |
| F02-K18-53 | C#5 | Best, 365 Industry-Motor 1 |
| F02-K18-54 | D5 | Best, 366 Industry-Motor 2 |
| F02-K18-55 | D#5 | Best, 361 Metal Hit 4 |
| F02-K18-56 | E5 | Best, 361 Metal Hit 4 |
| F02-K18-57 | F5 | Best, 206 H.H.Cl-EXB04 35 |
| F02-K18-58 | F#5 | Best, 235 H.H.Op-EXB04 05 |
| F02-K18-59 | G5 | Best, 210 H.H.Cl-EXB04 39 |
| F02-K18-60 | G#5 | Best, 254 H.H.SE-EXB04 01 |
| F02-K18-61 | A5 | Best, 214 H.H.Ho-EXB04 01 |
| F02-K18-62 | A#5 | Best, 238 H.H.Op-EXB04 08 |
| F02-K18-63 | B5 | Best, 234 H.H.Op-EXB04 04 |
| F02-K18-64 | C6 | Best, 7 BD-EXB04 007 |
| F02-K18-65 | C#6 | ROM, 28 BD-Ringy |
| F02-K18-66 | D6 | Best, 6 BD-EXB04 006 Best, 6 BD-EXB04 006 |
| F02-K18-67 | D#6 | Best, 31 BD-EXB04 031 |
| F02-K18-68 | E6 | Best, 1 BD-EXB04 001 |
| F02-K18-69 | F6 | ROM, 62 SD-Processed 1 |
| F02-K18-70 | F#6 | Best, 179 H.H.Cl-EXB04 08 |
| F02-K18-71 | G6 | ROM, 60 SD-Big Rock |
| F02-K18-72 | G#6 | Best, 180 H.H.Cl-EXB04 09 |
| F02-K18-73 | A6 | ROM, 74 SD-Hip 1 |
| F02-K18-74 | A#6 | Best, 182 H.H.Cl-EXB04 11 |
| F02-K18-75 | B6 | Best, 162 SD-EXB04 61 |
| F02-K18-76 | C7 | Synth, 126Perc-Junk-EXB9 6 |
| F02-K18-77 | C#7 | ROM, 148 H.Hat-Ambi ROM, 148 H.Hat-Ambi |
| F02-K18-78 | D7 | Synth, 120Perc-DDD-EXB09 4 |
| F02-K18-79 | D#7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K18-80 | E7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K18-81 | F7 | ROM, 162 Ride-Edge 1 |
| F02-K18-82 | F#7 | Synth, 140Tombourine-EXB09 |
| F02-K18-83 | G7 | ROM, 319 Cyber Drum |
| F02-K18-84 | G#7 | Best, 384 Hall Bound |
| F02-K18-85 | A7 | Synth, 85 Crash-EXB09 01 |
| F02-K18-86 | A#7 | ROM, 356 Scream |
| F02-K18-87 | B7 | ROM, 111 Tom-Jazz Hi |
| F02-K18-88 | C8 | ROM, 395 Noise-White |
| F02-K18-89 | D8 |  |

### F02-K19 - Electro Kit 2 (kit 018, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K19-01 | A0 |  |
| F02-K19-02 | A#0 |  |
| F02-K19-03 | B0 |  |
| F02-K19-04 | C1 | Vint, 2 BDEX-EXB05 00 |
| F02-K19-05 | C#1 | Best, 23 BD-EXB04 023 |
| F02-K19-06 | D1 | Synth, 17 BD-EXB09 18 |
| F02-K19-07 | D#1 | Synth, 13 BD-EXB09 14 |
| F02-K19-08 | E1 | ROM, 29 BD-Short88 |
| F02-K19-09 | F1 | ROM, 206 Zap 2 |
| F02-K19-10 | F#1 | ROM, 33 BD-Ambi Soft |
| F02-K19-11 | G1 | ROM, 82 SD-Vintage 1 |
| F02-K19-12 | G#1 | ROM, 82 SD-Vintage 1 |
| F02-K19-13 | A1 | ROM, 91 SD-AmbiHop |
| F02-K19-14 | A#1 | ROM, 205 Zap 1 |
| F02-K19-15 | B1 | Synth, 35 BD-EXB09 36 Synth, 35 BD-EXB09 36 |
| F02-K19-16 | C2 | Vint, 0 BD-EXB05 00 |
| F02-K19-17 | C#2 | Synth, 2 BD-EXB09 03 |
| F02-K19-18 | D2 | Vint, 0 BD-EXB05 00 |
| F02-K19-19 | D#2 | Synth, 1 BD-EXB09 02 |
| F02-K19-20 | E2 | Synth, 4 BD-EXB09 05 |
| F02-K19-21 | F2 | Vint, 20 SDEX-EXB05 03 |
| F02-K19-22 | F#2 | Best, 157 SD-EXB04 56 |
| F02-K19-23 | G2 | Vint, 17 SDEX-EXB05 00 |
| F02-K19-24 | G#2 | Vint, 23 SDEX-EXB05 07 |
| F02-K19-25 | A2 | Best, 120 SD-EXB04 19 |
| F02-K19-26 | A#2 | Best, 138 SD-EXB04 37 |
| F02-K19-27 | B2 | Best, 83 BD-EXB04 087 |
| F02-K19-28 | C3 | ROM, 19 BD-Dance 2 |
| F02-K19-29 | C#3 | Best, 169 SD-EXB04 68 |
| F02-K19-30 | D3 | Best, 114 SD-EXB04 13 |
| F02-K19-31 | D#3 | Best, 156 SD-EXB04 55 |
| F02-K19-32 | E3 | Synth, 45 SD-EXB09 06 |
| F02-K19-33 | F3 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K19-34 | F#3 | Synth, 74 HH04-Cl-EXB09 |
| F02-K19-35 | G3 | Synth, 111Perc-Tom-EXB09 2 |
| F02-K19-36 | G#3 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K19-37 | A3 | Synth, 111Perc-Tom-EXB09 2 |
| F02-K19-38 | A#3 | Synth, 77 HH04-Op-EXB09 |
| F02-K19-39 | B3 | Vint, 28 H.H.Cl-EXB05 02 |
| F02-K19-40 | C4 | Vint, 31 H.H.Cl-EXB05 05 |
| F02-K19-41 | C#4 | Vint, 39 H.H.Ho-EXB05 03 |
| F02-K19-42 | D4 | Vint, 28 H.H.Cl-EXB05 02 |
| F02-K19-43 | D#4 | Synth, 76 HH04-Ho-EXB09 02 |
| F02-K19-44 | E4 | Synth, 86 Crash-EXB09 02 |
| F02-K19-45 | F4 | Vint, 32 H.H.Cl-EXB05 06 |
| F02-K19-46 | F#4 | Best, 207 H.H.Cl-EXB04 36 |
| F02-K19-47 | G4 | Synth, 61 Clap-EXB09 05 |
| F02-K19-48 | G#4 | Best, 289 PercSE-EXB04 00 |
| F02-K19-49 | A4 | Synth, 78 HH05-Cl-EXB09 |
| F02-K19-50 | A#4 | Best, 300 SynthDrum-SD2 H |
| F02-K19-51 | B4 | Synth, 104Perc05-Cl-EXB09 |
| F02-K19-52 | C5 | Best, 285 Clap-EXB04 01 |
| F02-K19-53 | C#5 | Best, 246 H.H.Ana-EXB04 07 |
| F02-K19-54 | D5 | Vint, 53 Metal-EXB05 00 |
| F02-K19-55 | D#5 | Best, 246 H.H.Ana-EXB04 07 |
| F02-K19-56 | E5 | ROM, 206 Zap 2 |
| F02-K19-57 | F5 | Best, 298 SynthDrum-SD1 H |
| F02-K19-58 | F#5 | Best, 241 H.H.Ana-EXB04 02 |
| F02-K19-59 | G5 | Best, 271 Tamb-EXB04 03 |
| F02-K19-60 | G#5 | Best, 241 H.H.Ana-EXB04 02 |
| F02-K19-61 | A5 | Best, 205 H.H.Cl-EXB04 34 |
| F02-K19-62 | A#5 | ROM, 194 88-Clave |
| F02-K19-63 | B5 | Best, 281 Maracas-EXB04 |
| F02-K19-64 | C6 | Best, 297 SynthDrum-BD |
| F02-K19-65 | C#6 | Best, 66 BD-EXB04 066 |
| F02-K19-66 | D6 | Best, 88 BD-EXB04 092 |
| F02-K19-67 | D#6 | Best, 68 BD-EXB04 068 |
| F02-K19-68 | E6 | Best, 81 BD-EXB04 082 |
| F02-K19-69 | F6 | Best, 156 SD-EXB04 55 |
| F02-K19-70 | F#6 | Best, 215 H.H.Ho-EXB04 02 |
| F02-K19-71 | G6 | Best, 160 SD-EXB04 59 |
| F02-K19-72 | G#6 | Best, 364 Metal Hit 7 |
| F02-K19-73 | A6 | Best, 169 SD-EXB04 68 |
| F02-K19-74 | A#6 | Best, 226 H.H.Ho-EXB04 13 |
| F02-K19-75 | B6 | Best, 167 SD-EXB04 66 |
| F02-K19-76 | C7 | Vint, 1 BD-EXB05 01 |
| F02-K19-77 | C#7 | Synth, 106Perc06-Cl-EXB09 |
| F02-K19-78 | D7 | Synth, 117Perc-DDD-EXB09 1 |
| F02-K19-79 | D#7 | Vint, 33 H.H.Cl-EXB05 07 |
| F02-K19-80 | E7 | Synth, 116Perc-Mute-EXB09 |
| F02-K19-81 | F7 | Synth, 129Perc-Junk-EXB9 9 |
| F02-K19-82 | F#7 | Synth, 105Perc05-Op-EXB09 |
| F02-K19-83 | G7 | Synth, 74 HH04-Cl-EXB09 |
| F02-K19-84 | G#7 | Best, 274 Tamb-EXB04 06 |
| F02-K19-85 | A7 | Synth, 62 Clap-EXB09 06 |
| F02-K19-86 | A#7 | Best, 171 H.H.Cl-EXB04 00 |
| F02-K19-87 | B7 | ROM, 177 99-BD 2 |
| F02-K19-88 | C8 | Synth, 94 Perc01-Op-EXB09 |
| F02-K19-89 | C#8 |  |
| F02-K19-90 | D8 |  |

### F02-K20 - Under Hard Kit (kit 019, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K20-01 | A0 |  |
| F02-K20-02 | A#0 |  |
| F02-K20-03 | B0 |  |
| F02-K20-04 | C1 | Best, 7 BD-EXB04 007 |
| F02-K20-05 | C#1 | Best, 18 BD-EXB04 018 |
| F02-K20-06 | D1 | Best, 23 BD-EXB04 023 |
| F02-K20-07 | D#1 | Best, 14 BD-EXB04 014 |
| F02-K20-08 | E1 | Best, 31 BD-EXB04 031 |
| F02-K20-09 | F1 | Synth, 29 BD-EXB09 30 |
| F02-K20-10 | F#1 | ROM, 22 BD-Hip 1 |
| F02-K20-11 | G1 | Best, 134 SD-EXB04 33 |
| F02-K20-12 | G#1 | Best, 125 SD-EXB04 24 |
| F02-K20-13 | A1 | Best, 120 SD-EXB04 19 |
| F02-K20-14 | A#1 | Best, 123 SD-EXB04 22 |
| F02-K20-15 | B1 | Best, 135 SD-EXB04 34 |
| F02-K20-16 | C2 | Synth, 2 BD-EXB09 03 Synth, 2 BD-EXB09 03 |
| F02-K20-17 | C#2 | Synth, 30 BD-EXB09 31 Synth, 30 BD-EXB09 31 |
| F02-K20-18 | D2 | Synth, 1 BD-EXB09 02 |
| F02-K20-19 | D#2 | Synth, 32 BD-EXB09 33 Synth, 32 BD-EXB09 33 |
| F02-K20-20 | E2 | Synth, 3 BD-EXB09 04 Synth, 3 BD-EXB09 04 |
| F02-K20-21 | F2 | Best, 165 SD-EXB04 64 |
| F02-K20-22 | F#2 | Synth, 42 SD-EXB09 03 |
| F02-K20-23 | G2 | Synth, 47 SD-EXB09 08 |
| F02-K20-24 | G#2 | Best, 106 SD-EXB04 05 |
| F02-K20-25 | A2 | Synth, 50 SD-EXB09 11 |
| F02-K20-26 | A#2 | Synth, 51 SD-EXB09 12 |
| F02-K20-27 | B2 | Synth, 31 BD-EXB09 32 |
| F02-K20-28 | C3 | Synth, 16 BD-EXB09 17 Synth, 16 BD-EXB09 17 |
| F02-K20-29 | C#3 | Synth, 29 BD-EXB09 30 |
| F02-K20-30 | D3 | ROM, 77 SD-Hip 4 |
| F02-K20-31 | D#3 | Best, 162 SD-EXB04 61 |
| F02-K20-32 | E3 | Synth, 55 SD-EXB09 16 |
| F02-K20-33 | F3 | Synth, 74 HH04-Cl-EXB09 |
| F02-K20-34 | F#3 | Synth, 64 HH01-Cl-EXB09 |
| F02-K20-35 | G3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K20-36 | G#3 | Synth, 65 HH01-Ho-EXB09 Synth, 65 HH01-Ho-EXB09 |
| F02-K20-37 | A3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K20-38 | A#3 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K20-39 | B3 | Best, 313 Rap Voice 7 |
| F02-K20-40 | C4 | Synth, 76 HH04-Ho-EXB09 02 |
| F02-K20-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K20-42 | D4 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K20-43 | D#4 | Synth, 84 Ride-EXB09 02 |
| F02-K20-44 | E4 | Synth, 87 Crash-EXB09 03 |
| F02-K20-45 | F4 | Best, 279 Cabasa-EXB04 01 Best, 279 Cabasa-EXB04 01 |
| F02-K20-46 | F#4 | Best, 275 Tamb-EXB04 07 |
| F02-K20-47 | G4 | Best, 236 H.H.Op-EXB04 06 |
| F02-K20-48 | G#4 | Best, 290 PercSE-EXB04 01 |
| F02-K20-49 | A4 | Best, 177 H.H.Cl-EXB04 06 |
| F02-K20-50 | A#4 | Best, 291 PercSE-EXB04 02 |
| F02-K20-51 | B4 | Best, 283 Caxixi-EXB04 01 |
| F02-K20-52 | C5 | Best, 284 Clap-EXB04 00 Best, 284 Clap-EXB04 00 |
| F02-K20-53 | C#5 | Synth, 67 HH01-Op-EXB09 02 |
| F02-K20-54 | D5 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K20-55 | D#5 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K20-56 | E5 | Synth, 73 HH03-Op-EXB09 |
| F02-K20-57 | F5 | Synth, 73 HH03-Op-EXB09 |
| F02-K20-58 | F#5 | Synth, 239Vo-09 Go Synth, 239Vo-09 Go |
| F02-K20-59 | G5 | Synth, 72 HH03-Cl-EXB09 |
| F02-K20-60 | G#5 | Synth, 71 HH02-Op-EXB09 02 |
| F02-K20-61 | A5 | Synth, 125Perc-Junk-EXB9 5 |
| F02-K20-62 | A#5 | Synth, 84 Ride-EXB09 02 |
| F02-K20-63 | B5 | ROM, 206 Zap 2 |
| F02-K20-64 | C6 | Best, 84 BD-EXB04 088 |
| F02-K20-65 | C#6 | Best, 88 BD-EXB04 092 |
| F02-K20-66 | D6 | Best, 94 BD-EXB04 098 |
| F02-K20-67 | D#6 | Best, 68 BD-EXB04 068 |
| F02-K20-68 | E6 | Best, 100 BD-EXB04 104 |
| F02-K20-69 | F6 | Best, 110 SD-EXB04 09 |
| F02-K20-70 | F#6 | Best, 224 H.H.Ho-EXB04 11 |
| F02-K20-71 | G6 | Best, 110 SD-EXB04 09 |
| F02-K20-72 | G#6 | Best, 225 H.H.Ho-EXB04 12 |
| F02-K20-73 | A6 | Best, 162 SD-EXB04 61 |
| F02-K20-74 | A#6 | Best, 228 H.H.Ho-EXB04 15 |
| F02-K20-75 | B6 | Best, 167 SD-EXB04 66 |
| F02-K20-76 | C7 | Synth, 213Vo-09 Hey F2 |
| F02-K20-77 | C#7 | ROM, 118 H.Hat 1-Closed 1 |
| F02-K20-78 | D7 | Synth, 238Vo-09 Get Down |
| F02-K20-79 | D#7 | Synth, 210Vo-09 Uuh |
| F02-K20-80 | E7 | Synth, 224Vo-09 Owww |
| F02-K20-81 | F7 | Best, 353 Guitar Wah 5 |
| F02-K20-82 | F#7 | Best, 292 PercSE-EXB04 03 |
| F02-K20-83 | G7 | ROM, 139 H.Hat-Tight 1 |
| F02-K20-84 | G#7 | Best, 153 SD-EXB04 52 |
| F02-K20-85 | A7 | Vint, 50 Perc B-EXB05 |
| F02-K20-86 | A#7 | Best, 358 Metal Hit 1 |
| F02-K20-87 | B7 | Best, 359 Metal Hit 2 |
| F02-K20-88 | C8 | Best, 360 Metal Hit 3 |
| F02-K20-89 | C#8 |  |
| F02-K20-90 | D8 |  |

### F02-K21 - Nu Pop Kit (kit 020, D1 p25)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K21-01 | A0 |  |
| F02-K21-02 | A#0 |  |
| F02-K21-03 | B0 |  |
| F02-K21-04 | C1 | ROM, 13 BD-Terminator |
| F02-K21-05 | C#1 | ROM, 19 BD-Dance 2 |
| F02-K21-06 | D1 | ROM, 22 BD-Hip 1 |
| F02-K21-07 | D#1 | ROM, 30 BD-Dance99 |
| F02-K21-08 | E1 | ROM, 31 BD-Ambi |
| F02-K21-09 | F1 | Synth, 29 BD-EXB09 30 |
| F02-K21-10 | F#1 | ROM, 28 BD-Ringy |
| F02-K21-11 | G1 | ROM, 83 SD-Vintage 2 |
| F02-K21-12 | G#1 | ROM, 84 SD-Vintage 3 |
| F02-K21-13 | A1 | ROM, 92 SD-Brasser |
| F02-K21-14 | A#1 | Vint, 2 BDEX-EXB05 00 |
| F02-K21-15 | B1 | Best, 25 BD-EXB04 025 |
| F02-K21-16 | C2 | Best, 75 BD-EXB04 075 |
| F02-K21-17 | C#2 | Best, 78 BD-EXB04 078 |
| F02-K21-18 | D2 | Best, 70 BD-EXB04 070 |
| F02-K21-19 | D#2 | Best, 69 BD-EXB04 069 |
| F02-K21-20 | E2 | Synth, 26 BD-EXB09 27 |
| F02-K21-21 | F2 | Best, 135 SD-EXB04 34 |
| F02-K21-22 | F#2 | Best, 125 SD-EXB04 24 |
| F02-K21-23 | G2 | Best, 140 SD-EXB04 39 |
| F02-K21-24 | G#2 | Best, 132 SD-EXB04 31 |
| F02-K21-25 | A2 | Best, 113 SD-EXB04 12 |
| F02-K21-26 | A#2 | Best, 142 SD-EXB04 41 |
| F02-K21-27 | B2 | Synth, 22 BD-EXB09 23 |
| F02-K21-28 | C3 | ROM, 23 BD-Hip 2 |
| F02-K21-29 | C#3 | Synth, 18 BD-EXB09 19 |
| F02-K21-30 | D3 | Synth, 55 SD-EXB09 16 |
| F02-K21-31 | D#3 | Best, 138 SD-EXB04 37 |
| F02-K21-32 | E3 | Synth, 56 SD-Jungle-EXB09 |
| F02-K21-33 | F3 | Vint, 35 H.H.Cl-EXB05 09 |
| F02-K21-34 | F#3 | ROM, 126 H.Hat 2-Closed 2 |
| F02-K21-35 | G3 | ROM, 109 Tom 3-Floor |
| F02-K21-36 | G#3 | ROM, 127 H.Hat 2-Foot |
| F02-K21-37 | A3 | ROM, 109 Tom 3-Floor |
| F02-K21-38 | A#3 | ROM, 129 H.Hat 2-Open 1 |
| F02-K21-39 | B3 | Best, 208 H.H.Cl-EXB04 37 |
| F02-K21-40 | C4 | Best, 207 H.H.Cl-EXB04 36 |
| F02-K21-41 | C#4 | ROM, 152 Crash Cymbal 1 |
| F02-K21-42 | D4 | Best, 226 H.H.Ho-EXB04 13 |
| F02-K21-43 | D#4 | ROM, 162 Ride-Edge 1 |
| F02-K21-44 | E4 | ROM, 156 Splash Cymbal |
| F02-K21-45 | F4 | ROM, 292 Triangle-Open ROM, 293 Triangle-Mute |
| F02-K21-46 | F#4 | Synth, 78 HH05-Cl-EXB09 |
| F02-K21-47 | G4 | ROM, 289 Tambourine-Pull ROM, 288 Tambourine-Push |
| F02-K21-48 | G#4 | Synth, 79 HH05-Ho-EXB09 |
| F02-K21-49 | A4 | ROM, 299 Cabasa-Up ROM, 301 Cabasa-Tap |
| F02-K21-50 | A#4 | Synth, 80 HH05-Op-EXB09 |
| F02-K21-51 | B4 | ROM, 258 Maracas-Push ROM, 259 Maracas-Pull |
| F02-K21-52 | C5 | Best, 284 Clap-EXB04 00 |
| F02-K21-53 | C#5 | OrchB, 21 Triangle-Mute |
| F02-K21-54 | D5 | Vint, 53 Metal-EXB05 00 |
| F02-K21-55 | D#5 | Best, 287 88Tom-EXB04 |
| F02-K21-56 | E5 | Best, 289 PercSE-EXB04 00 |
| F02-K21-57 | F5 | Vint, 49 Perc A-EXB05 |
| F02-K21-58 | F#5 | Best, 246 H.H.Ana-EXB04 07 |
| F02-K21-59 | G5 | Vint, 46 Perc Lo-EXB05 00 |
| F02-K21-60 | G#5 | Best, 240 H.H.Ana-EXB04 01 |
| F02-K21-61 | A5 | Best, 302 SynthDrum-OHH 1 |
| F02-K21-62 | A#5 | Best, 245 H.H.Ana-EXB04 06 |
| F02-K21-63 | B5 | Best, 304 SynthDrum-OHH 3 |
| F02-K21-64 | C6 | Best, 7 BD-EXB04 007 |
| F02-K21-65 | C#6 | Best, 80 BD-EXB04 080 |
| F02-K21-66 | D6 | Best, 85 BD-EXB04 089 |
| F02-K21-67 | D#6 | Synth, 0 BD-EXB09 01 |
| F02-K21-68 | E6 | Best, 91 BD-EXB04 095 |
| F02-K21-69 | F6 | Best, 143 SD-EXB04 42 |
| F02-K21-70 | F#6 | Best, 245 H.H.Ana-EXB04 06 |
| F02-K21-71 | G6 | Best, 106 SD-EXB04 05 |
| F02-K21-72 | G#6 | Best, 241 H.H.Ana-EXB04 02 |
| F02-K21-73 | A6 | Best, 143 SD-EXB04 42 |
| F02-K21-74 | A#6 | Best, 226 H.H.Ho-EXB04 13 |
| F02-K21-75 | B6 | Synth, 52 SD-EXB09 13 |
| F02-K21-76 | C7 | Synth, 100Perc04-Cl-EXB09 |
| F02-K21-77 | C#7 | Vint, 31 H.H.Cl-EXB05 05 |
| F02-K21-78 | D7 | Synth, 125Perc-Junk-EXB9 5 |
| F02-K21-79 | D#7 | Synth, 146Vo-09 Hey M |
| F02-K21-80 | E7 | Synth, 127Perc-Junk-EXB9 7 |
| F02-K21-81 | F7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K21-82 | F#7 | Synth, 109Perc07-Op-EXB09 |
| F02-K21-83 | G7 | Vint, 36 H.H.Ho-EXB05 00 |
| F02-K21-84 | G#7 | Best, 264 Hit-EXB04 02 |
| F02-K21-85 | A7 | Best, 267 Hit-EXB04 05 |
| F02-K21-86 | A#7 | ROM, 346 PC Vox-One |
| F02-K21-87 | B7 | ROM, 331 CompuVoice-Noise |
| F02-K21-88 | C8 | Synth, 96 Perc02-Op-EXB09 |
| F02-K21-89 | C#8 | 25 |
| F02-K21-90 | D8 |  |

### F02-K22 - Voice Drum Set (kit 021, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K22-01 | A0 |  |
| F02-K22-02 | A#0 |  |
| F02-K22-03 | B0 |  |
| F02-K22-04 | C1 | ROM, 330 CompuVoice-Beep |
| F02-K22-05 | C#1 | ROM, 341 One... Solo |
| F02-K22-06 | D1 | ROM, 340 Two... Solo |
| F02-K22-07 | D#1 | ROM, 339 Three... Solo |
| F02-K22-08 | E1 | ROM, 338 Four... Solo |
| F02-K22-09 | F1 | ROM, 342 PC Vox-Sis |
| F02-K22-10 | F#1 | ROM, 345 PC Vox-For |
| F02-K22-11 | G1 | ROM, 343 PC Vox-Dunc |
| F02-K22-12 | G#1 | ROM, 347 PC Vox-If |
| F02-K22-13 | A1 | ROM, 349 PC Vox-Ae |
| F02-K22-14 | A#1 | ROM, 352 PC Vox-O |
| F02-K22-15 | B1 | ROM, 353 PC Vox-Tu |
| F02-K22-16 | C2 | Best, 311 Rap Voice 5 |
| F02-K22-17 | C#2 | Synth, 207Vo-09 Ahoo Synth, 207Vo-09 Ahoo |
| F02-K22-18 | D2 | Synth, 206Vo-09 Oohh |
| F02-K22-19 | D#2 | Best, 313 Rap Voice 7 |
| F02-K22-20 | E2 | Synth, 148Vo-09 Love Synth, 148Vo-09 Love |
| F02-K22-21 | F2 | Synth, 156Vo-09 Yeah M1 |
| F02-K22-22 | F#2 | Synth, 155Vo-09 Bass |
| F02-K22-23 | G2 | Best, 312 Rap Voice 6 Best, 312 Rap Voice 6 |
| F02-K22-24 | G#2 | ROM, 199 Vocal SD |
| F02-K22-25 | A2 | Synth, 199Vo-09 Well |
| F02-K22-26 | A#2 | Best, 316 Gospel Voice 12 |
| F02-K22-27 | B2 | Synth, 162Vo-09 Ooooh |
| F02-K22-28 | C3 | Synth, 161Vo-09 Ohhhh Synth, 161Vo-09 Ohhhh |
| F02-K22-29 | C#3 | Synth, 169Vo-09 Click |
| F02-K22-30 | D3 | Synth, 167Vo-09 Baaaa |
| F02-K22-31 | D#3 | Synth, 168Vo-09 Hrrrr |
| F02-K22-32 | E3 | Synth, 160Vo-09 Ahhhh |
| F02-K22-33 | F3 | Synth, 166Vo-09 Kaaaa |
| F02-K22-34 | F#3 | ROM, 344 PC Vox-Cks |
| F02-K22-35 | G3 | Best, 317 Gospel Voice 13 |
| F02-K22-36 | G#3 | Synth, 170Vo-09 Trrrr |
| F02-K22-37 | A3 | Best, 318 Gospel Voice 14 |
| F02-K22-38 | A#3 | Synth, 174Vo-09 Hah |
| F02-K22-39 | B3 | Synth, 205Vo-09 Turnin' Up |
| F02-K22-40 | C4 | Synth, 172Vo-09 Oh-Yeah M Synth, 172Vo-09 Oh-Yeah M |
| F02-K22-41 | C#4 | Best, 321 Gospel Voice 18 |
| F02-K22-42 | D4 | Synth, 182Vo-09 Ay-ay-he Synth, 182Vo-09 Ay-ay-he |
| F02-K22-43 | D#4 | Synth, 211Vo-09 Uh-a-ha |
| F02-K22-44 | E4 | Synth, 208Vo-09 Mmmm |
| F02-K22-45 | F4 | Synth, 233Vo-09 I wanna |
| F02-K22-46 | F#4 | Best, 317 Gospel Voice 13 |
| F02-K22-47 | G4 | Synth, 201Vo-09 Dance |
| F02-K22-48 | G#4 | Best, 329 Gospel Voice 29 |
| F02-K22-49 | A4 | Best, 327 Gospel Voice 26 |
| F02-K22-50 | A#4 | Best, 326 Gospel Voice 25 |
| F02-K22-51 | B4 | Best, 317 Gospel Voice 13 |
| F02-K22-52 | C5 | Synth, 222Vo-09 Oh-ya-yah |
| F02-K22-53 | C#5 | Synth, 171Vo-09 Club Synth, 171Vo-09 Club |
| F02-K22-54 | D5 | ROM, 201 Vocal Woodblock |
| F02-K22-55 | D#5 | Best, 317 Gospel Voice 13 |
| F02-K22-56 | E5 | ROM, 200 Vocal Cowbell |
| F02-K22-57 | F5 | Synth, 179Vo-09 Ya-ee-ha Synth, 179Vo-09 Ya-ee-ha |
| F02-K22-58 | F#5 | Synth, 153Vo-09 KeepRockin |
| F02-K22-59 | G5 | Best, 323 Gospel Voice 21 |
| F02-K22-60 | G#5 | Synth, 178Vo-09 Ya-ee Synth, 178Vo-09 Ya-ee |
| F02-K22-61 | A5 | Synth, 194Vo-09 Take Up Synth, 194Vo-09 Take Up |
| F02-K22-62 | A#5 | Synth, 210Vo-09 Uuh Synth, 210Vo-09 Uuh |
| F02-K22-63 | B5 | Best, 316 Gospel Voice 12 Best, 316 Gospel Voice 12 |
| F02-K22-64 | C6 | Synth, 207Vo-09 Ahoo Synth, 207Vo-09 Ahoo |
| F02-K22-65 | C#6 | Synth, 180Vo-09 Ye-ah Synth, 180Vo-09 Ye-ah |
| F02-K22-66 | D6 | Synth, 146Vo-09 Hey M |
| F02-K22-67 | D#6 | Synth, 154Vo-09 Break itUp Synth, 154Vo-09 Break itUp |
| F02-K22-68 | E6 | Synth, 172Vo-09 Oh-Yeah M Synth, 172Vo-09 Oh-Yeah M |
| F02-K22-69 | F6 | Synth, 163Vo-09 Aaaah Synth, 163Vo-09 Aaaah |
| F02-K22-70 | F#6 | Synth, 192Vo-09 TurnAround Synth, 192Vo-09 TurnAround |
| F02-K22-71 | G6 | Synth, 163Vo-09 Aaaah |
| F02-K22-72 | G#6 | Synth, 210Vo-09 Uuh |
| F02-K22-73 | A6 | Synth, 160Vo-09 Ahhhh |
| F02-K22-74 | A#6 | Synth, 163Vo-09 Aaaah |
| F02-K22-75 | B6 | Best, 331 Gospel Voice 31 Best, 331 Gospel Voice 31 |
| F02-K22-76 | C7 | Synth, 204Vo-09 Good Synth, 204Vo-09 Good |
| F02-K22-77 | C#7 | Synth, 210Vo-09 Uuh |
| F02-K22-78 | D7 | ROM, 344 PC Vox-Cks |
| F02-K22-79 | D#7 | ROM, 344 PC Vox-Cks |
| F02-K22-80 | E7 | ROM, 327 Uhh Solo |
| F02-K22-81 | F7 | ROM, 332 CompuVoice-Chi |
| F02-K22-82 | F#7 | Best, 326 Gospel Voice 25 |
| F02-K22-83 | G7 | Synth, 212Vo-09 Hey F1 |
| F02-K22-84 | G#7 | ROM, 332 CompuVoice-Chi |
| F02-K22-85 | A7 | Synth, 215Vo-09 C'mon |
| F02-K22-86 | A#7 | Best, 331 Gospel Voice 31 |
| F02-K22-87 | B7 | Best, 322 Gospel Voice 19 |
| F02-K22-88 | C8 | Best, 325 Gospel Voice 24 |
| F02-K22-89 | D8 |  |

### F02-K23 - Junkyard Kit (kit 022, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K23-01 | A0 |  |
| F02-K23-02 | A#0 |  |
| F02-K23-03 | B0 |  |
| F02-K23-04 | C1 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K23-05 | C#1 | Best, 385 Glass Hit Best, 385 Glass Hit |
| F02-K23-06 | D1 | Synth, 102Perc-Metal-EXB09 Synth, 102Perc-Metal-EXB09 |
| F02-K23-07 | D#1 | Best, 381 Industry-TAIHOU Best, 381 Industry-TAIHOU |
| F02-K23-08 | E1 | Best, 292 PercSE-EXB04 03 Best, 292 PercSE-EXB04 03 |
| F02-K23-09 | F1 | Best, 301 SynthDrum-SD2 L Best, 301 SynthDrum-SD2 L |
| F02-K23-10 | F#1 | Best, 366 Industry-Motor 2 Best, 366 Industry-Motor 2 |
| F02-K23-11 | G1 | Best, 105 SD-EXB04 04 Best, 105 SD-EXB04 04 |
| F02-K23-12 | G#1 | Synth, 145Revers-EXB09 Synth, 145Revers-EXB09 |
| F02-K23-13 | A1 | Best, 14 BD-EXB04 014 Best, 14 BD-EXB04 014 |
| F02-K23-14 | A#1 | Synth, 107Perc06-Op-EXB09 Synth, 107Perc06-Op-EXB09 |
| F02-K23-15 | B1 | Best, 382 Industry-Voice Best, 382 Industry-Voice |
| F02-K23-16 | C2 | Best, 381 Industry-TAIHOU Best, 381 Industry-TAIHOU |
| F02-K23-17 | C#2 | Best, 46 BD-EXB04 046 Best, 46 BD-EXB04 046 |
| F02-K23-18 | D2 | Best, 301 SynthDrum-SD2 L Best, 301 SynthDrum-SD2 L |
| F02-K23-19 | D#2 | Best, 363 Metal Hit 6 Best, 363 Metal Hit 6 |
| F02-K23-20 | E2 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K23-21 | F2 | ROM, 320 SD-Metal Striker ROM, 320 SD-Metal Striker |
| F02-K23-22 | F#2 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K23-23 | G2 | Best, 119 SD-EXB04 18 Best, 119 SD-EXB04 18 |
| F02-K23-24 | G#2 | OrchB, 16 Piatti ff OrchB, 17 Piatti pp |
| F02-K23-25 | A2 | OrchB, 6 Timpani-Roll OrchB, 6 Timpani-Roll |
| F02-K23-26 | A#2 | Synth, 39 BBD-EXB09 02 Synth, 39 BBD-EXB09 02 |
| F02-K23-27 | B2 | Best, 383 Industry-BD Best, 383 Industry-BD |
| F02-K23-28 | C3 | Best, 384 Hall Bound Best, 384 Hall Bound |
| F02-K23-29 | C#3 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K23-30 | D3 | Best, 372 Industry-Iron Best, 372 Industry-Iron |
| F02-K23-31 | D#3 | Synth, 103Perc-Ambi-EXB09 Synth, 103Perc-Ambi-EXB09 |
| F02-K23-32 | E3 | Best, 373 Industry-Press Best, 373 Industry-Press |
| F02-K23-33 | F3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K23-34 | F#3 | Best, 380 Industry-Pump Best, 380 Industry-Pump |
| F02-K23-35 | G3 | Synth, 110Perc-Tom-EXB09 1 Best, 305 SynthDrum-Tom H |
| F02-K23-36 | G#3 | Best, 379 Industry-Comp 2 Best, 379 Industry-Comp 2 |
| F02-K23-37 | A3 | Synth, 111Perc-Tom-EXB09 2 Best, 306 SynthDrum-Tom M |
| F02-K23-38 | A#3 | Best, 387 Industry-69 Best, 387 Industry-69 |
| F02-K23-39 | B3 | Synth, 112Perc-Tom-EXB09 3 Best, 310 SynthDrum-Perc L |
| F02-K23-40 | C4 | Best, 360 Metal Hit 3 |
| F02-K23-41 | C#4 | Best, 376 Industry-GIIN. Best, 376 Industry-GIIN. |
| F02-K23-42 | D4 | Best, 365 Industry-Motor 1 |
| F02-K23-43 | D#4 | Best, 367 Industry-Drill 1 Best, 367 Industry-Drill 1 |
| F02-K23-44 | E4 | Best, 377 Industry-Machine Best, 377 Industry-Machine |
| F02-K23-45 | F4 | Best, 368 Industry-Drill 2 Best, 368 Industry-Drill 2 |
| F02-K23-46 | F#4 | Best, 369 Industry-Drill 3 Best, 369 Industry-Drill 3 |
| F02-K23-47 | G4 | Best, 374 Industry-Metal Best, 374 Industry-Metal |
| F02-K23-48 | G#4 | Best, 380 Industry-Pump Best, 380 Industry-Pump |
| F02-K23-49 | A4 | Best, 389 Industry-142 Best, 389 Industry-142 |
| F02-K23-50 | A#4 | Best, 359 Metal Hit 2 Best, 359 Metal Hit 2 |
| F02-K23-51 | B4 | Synth, 143Soda-EXB09 Synth, 143Soda-EXB09 |
| F02-K23-52 | C5 | Best, 388 Industry-114 Best, 388 Industry-114 |
| F02-K23-53 | C#5 | Best, 378 Industry-Comp 1 Best, 378 Industry-Comp 1 |
| F02-K23-54 | D5 | Best, 361 Metal Hit 4 Best, 361 Metal Hit 4 |
| F02-K23-55 | D#5 | Best, 375 Industry-Regist Best, 375 Industry-Regist |
| F02-K23-56 | E5 | Synth, 144Glass-EXB09 Synth, 144Glass-EXB09 |
| F02-K23-57 | F5 | Best, 362 Metal Hit 5 Best, 362 Metal Hit 5 |
| F02-K23-58 | F#5 | Synth, 102Perc-Metal-EXB09 Synth, 102Perc-Metal-EXB09 |
| F02-K23-59 | G5 | Best, 371 Industry-Bowing Best, 371 Industry-Bowing |
| F02-K23-60 | G#5 | Best, 358 Metal Hit 1 Best, 358 Metal Hit 1 |
| F02-K23-61 | A5 | ROM, 322 Industry Loop 1 ROM, 322 Industry Loop 1 |
| F02-K23-62 | A#5 | Best, 364 Metal Hit 7 Best, 364 Metal Hit 7 |
| F02-K23-63 | B5 | Synth, 100Perc04-Cl-EXB09 Synth, 100Perc04-Cl-EXB09 |
| F02-K23-64 | C6 | ROM, 323 Industry Loop 2 ROM, 323 Industry Loop 2 |
| F02-K23-65 | C#6 | Best, 386 Industry-52 Best, 386 Industry-52 |
| F02-K23-66 | D6 | Synth, 99 Perc03-Op-EXB09 Synth, 98 Perc03-Cl-EXB09 |
| F02-K23-67 | D#6 | Best, 358 Metal Hit 1 Best, 358 Metal Hit 1 |
| F02-K23-68 | E6 | Synth, 142Car Horn-EXB09 Synth, 142Car Horn-EXB09 |
| F02-K23-69 | F6 | Synth, 97 Perc02-Rev-EXB09 Synth, 97 Perc02-Rev-EXB09 |
| F02-K23-70 | F#6 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K23-71 | G6 | Synth, 105Perc05-Op-EXB09 Synth, 105Perc05-Op-EXB09 |
| F02-K23-72 | G#6 | Synth, 80 HH05-Op-EXB09 Synth, 80 HH05-Op-EXB09 |
| F02-K23-73 | A6 | ROM, 395 Noise-White ROM, 395 Noise-White |
| F02-K23-74 | A#6 | ROM, 396 Noise-FM Mod ROM, 396 Noise-FM Mod |
| F02-K23-75 | B6 | Best, 364 Metal Hit 7 Best, 364 Metal Hit 7 |
| F02-K23-76 | C7 | Synth, 38 BBD-EXB09 01 Synth, 38 BBD-EXB09 01 |
| F02-K23-77 | C#7 | Synth, 122Perc-Junk-EXB9 2 Synth, 122Perc-Junk-EXB9 2 |
| F02-K23-78 | D7 | ROM, 316 Samba Whistle ROM, 316 Samba Whistle |
| F02-K23-79 | D#7 | ROM, 323 Industry Loop 2 ROM, 320 SD-Metal Striker |
| F02-K23-80 | E7 | ROM, 217 DJ-BD Rub ROM, 217 DJ-BD Rub |
| F02-K23-81 | F7 | ROM, 318 Industry ROM, 318 Industry |
| F02-K23-82 | F#7 | ROM, 407 Dist.Slide 2 ROM, 406 Dist.Slide 1 |
| F02-K23-83 | G7 | ROM, 403 HarpGliss ROM, 371 Rainstick |
| F02-K23-84 | G#7 | ROM, 253 Castanet-Dble 1 ROM, 277 Vibraslap |
| F02-K23-85 | A7 | ROM, 219 DJ-Record Stop ROM, 220 DJ-Reverse |
| F02-K23-86 | A#7 | ROM, 209 DJ-Scratch 2 ROM, 208 DJ-Scratch 1 |
| F02-K23-87 | B7 | ROM, 369 Tribe ROM, 369 Tribe |
| F02-K23-88 | C8 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K23-89 | D8 |  |

### F02-K24 - Voice Catalog (kit 023, D1 p26)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K24-01 | A0 |  |
| F02-K24-02 | A#0 |  |
| F02-K24-03 | B0 |  |
| F02-K24-04 | C1 | Best, 314 Gospel Voice 1 |
| F02-K24-05 | C#1 | Best, 319 Gospel Voice 15 |
| F02-K24-06 | D1 | Best, 321 Gospel Voice 18 |
| F02-K24-07 | D#1 | Synth, 179Vo-09 Ya-ee-ha Synth, 178Vo-09 Ya-ee |
| F02-K24-08 | E1 | Synth, 180Vo-09 Ye-ah |
| F02-K24-09 | F1 | Synth, 157Vo-09 Yeah M2 Synth, 156Vo-09 Yeah M1 |
| F02-K24-10 | F#1 | Synth, 172Vo-09 Oh-Yeah M Synth, 172Vo-09 Oh-Yeah M |
| F02-K24-11 | G1 | Best, 331 Gospel Voice 31 Best, 331 Gospel Voice 31 |
| F02-K24-12 | G#1 | Best, 328 Gospel Voice 28 |
| F02-K24-13 | A1 | Synth, 221Vo-09 Yah Synth, 221Vo-09 Yah |
| F02-K24-14 | A#1 | ROM, 326 Year solo ROM, 326 Year solo |
| F02-K24-15 | B1 | ROM, 325 Yea Solo ROM, 325 Yea Solo |
| F02-K24-16 | C2 | Synth, 232Vo-09 Don't Stop Synth, 232Vo-09 Don't Stop |
| F02-K24-17 | C#2 | Synth, 183Vo-09 Aow Synth, 183Vo-09 Aow |
| F02-K24-18 | D2 | Synth, 175Vo-09 Baby Synth, 175Vo-09 Baby |
| F02-K24-19 | D#2 | Synth, 158Vo-09 Bang Synth, 158Vo-09 Bang |
| F02-K24-20 | E2 | Synth, 214Vo-09 Haaay Synth, 214Vo-09 Haaay |
| F02-K24-21 | F2 | Synth, 213Vo-09 Hey F2 Synth, 212Vo-09 Hey F1 |
| F02-K24-22 | F#2 | Synth, 146Vo-09 Hey M Synth, 146Vo-09 Hey M |
| F02-K24-23 | G2 | Synth, 216Vo-09 Com'On Synth, 215Vo-09 C'mon |
| F02-K24-24 | G#2 | Synth, 219Vo-09 PartyNight Synth, 218Vo-09 Pardy |
| F02-K24-25 | A2 | Synth, 237Vo-09 Crazy Synth, 237Vo-09 Crazy |
| F02-K24-26 | A#2 | Synth, 239Vo-09 Go Synth, 239Vo-09 Go |
| F02-K24-27 | B2 | Synth, 155Vo-09 Bass Synth, 155Vo-09 Bass |
| F02-K24-28 | C3 | Synth, 162Vo-09 Ooooh Synth, 161Vo-09 Ohhhh |
| F02-K24-29 | C#3 | Synth, 166Vo-09 Kaaaa Synth, 166Vo-09 Kaaaa |
| F02-K24-30 | D3 | Synth, 224Vo-09 Owww Synth, 225Vo-09 Aaa! |
| F02-K24-31 | D#3 | Synth, 165Vo-09 Kkkkk Synth, 165Vo-09 Kkkkk |
| F02-K24-32 | E3 | Synth, 160Vo-09 Ahhhh Synth, 163Vo-09 Aaaah |
| F02-K24-33 | F3 | Best, 311 Rap Voice 5 Best, 311 Rap Voice 5 |
| F02-K24-34 | F#3 | Synth, 169Vo-09 Click Synth, 169Vo-09 Click |
| F02-K24-35 | G3 | Best, 312 Rap Voice 6 Best, 312 Rap Voice 6 |
| F02-K24-36 | G#3 | Synth, 170Vo-09 Trrrr |
| F02-K24-37 | A3 | ROM, 329 Uhhhh Solo ROM, 329 Uhhhh Solo |
| F02-K24-38 | A#3 | Synth, 168Vo-09 Hrrrr Synth, 168Vo-09 Hrrrr |
| F02-K24-39 | B3 | ROM, 327 Uhh Solo ROM, 327 Uhh Solo |
| F02-K24-40 | C4 | Best, 313 Rap Voice 7 Best, 313 Rap Voice 7 |
| F02-K24-41 | C#4 | Best, 327 Gospel Voice 26 Best, 326 Gospel Voice 25 |
| F02-K24-42 | D4 | Best, 324 Gospel Voice 23 Best, 324 Gospel Voice 23 |
| F02-K24-43 | D#4 | Synth, 223Vo-09 Oohh Synth, 223Vo-09 Oohh |
| F02-K24-44 | E4 | Best, 330 Gospel Voice 30 |
| F02-K24-45 | F4 | Synth, 187Vo-09 Oooua Synth, 187Vo-09 Oooua |
| F02-K24-46 | F#4 | Synth, 211Vo-09 Uh-a-ha Synth, 211Vo-09 Uh-a-ha |
| F02-K24-47 | G4 | Synth, 208Vo-09 Mmmm Synth, 208Vo-09 Mmmm |
| F02-K24-48 | G#4 | Best, 320 Gospel Voice 17 Best, 320 Gospel Voice 17 |
| F02-K24-49 | A4 | Best, 325 Gospel Voice 24 Best, 325 Gospel Voice 24 |
| F02-K24-50 | A#4 | Synth, 228Vo-09 Oh-Yeah F Synth, 228Vo-09 Oh-Yeah F |
| F02-K24-51 | B4 | ROM, 328 Hit It ROM, 328 Hit It |
| F02-K24-52 | C5 | Synth, 174Vo-09 Hah Synth, 173Vo-09 Uagh |
| F02-K24-53 | C#5 | Synth, 171Vo-09 Club |
| F02-K24-54 | D5 | Synth, 238Vo-09 Get Down Synth, 238Vo-09 Get Down |
| F02-K24-55 | D#5 | Synth, 164Vo-09 Love |
| F02-K24-56 | E5 | Synth, 147Vo-09 Wild |
| F02-K24-57 | F5 | Synth, 150Vo-09 Ready |
| F02-K24-58 | F#5 | Synth, 153Vo-09 KeepRockin Synth, 153Vo-09 KeepRockin |
| F02-K24-59 | G5 | Synth, 159Vo-09 Let's Go Synth, 159Vo-09 Let's Go |
| F02-K24-60 | G#5 | Synth, 154Vo-09 Break itUp Synth, 154Vo-09 Break itUp |
| F02-K24-61 | A5 | Synth, 220Vo-09 Yaeh F Synth, 220Vo-09 Yaeh F |
| F02-K24-62 | A#5 | Synth, 210Vo-09 Uuh Synth, 210Vo-09 Uuh |
| F02-K24-63 | B5 | Synth, 207Vo-09 Ahoo Synth, 207Vo-09 Ahoo |
| F02-K24-64 | C6 | Synth, 227Vo-09 Uhwoah Synth, 227Vo-09 Uhwoah |
| F02-K24-65 | C#6 | Synth, 198Vo-09 Oh-o-owu |
| F02-K24-66 | D6 | Synth, 229Vo-09 Alright |
| F02-K24-67 | D#6 | Best, 329 Gospel Voice 29 |
| F02-K24-68 | E6 | Synth, 230Vo-09 I Love It Synth, 230Vo-09 I Love It |
| F02-K24-69 | F6 | Synth, 231Vo-09 I believe |
| F02-K24-70 | F#6 | Synth, 234Vo-09 Be Synth, 233Vo-09 I wanna |
| F02-K24-71 | G6 | Synth, 236Vo-09 You Synth, 235Vo-09 I Need |
| F02-K24-72 | G#6 | Synth, 217Vo-09 Come On Synth, 217Vo-09 Come On |
| F02-K24-73 | A6 | Synth, 194Vo-09 Take Up Synth, 194Vo-09 Take Up |
| F02-K24-74 | A#6 | Synth, 186Vo-09 Ever Synth, 185Vo-09 What |
| F02-K24-75 | B6 | Synth, 204Vo-09 Good Synth, 203Vo-09 I feel |
| F02-K24-76 | C7 | Synth, 177Vo-09It'sAllGood Synth, 177Vo-09It'sAllGood |
| F02-K24-77 | C#7 | Synth, 190Vo-09 Do It Synth, 189Vo-09 CaMon |
| F02-K24-78 | D7 | Synth, 201Vo-09 Dance Synth, 200Vo-09 Ho-o |
| F02-K24-79 | D#7 | Synth, 193Vo-09 Wo'I dance Synth, 193Vo-09 Wo'I dance |
| F02-K24-80 | E7 | Synth, 192Vo-09 TurnAround Synth, 199Vo-09 Well |
| F02-K24-81 | F7 | Synth, 195Vo-09 n Round Synth, 195Vo-09 n Round |
| F02-K24-82 | F#7 | Synth, 196Vo-09 Ye-ah-ar |
| F02-K24-83 | G7 | Synth, 197Vo-09 Go-o |
| F02-K24-84 | G#7 | Synth, 202Vo-09 Ha-ooo-wa |
| F02-K24-85 | A7 | Best, 323 Gospel Voice 21 |
| F02-K24-86 | A#7 | Best, 315 Gospel Voice 2 |
| F02-K24-87 | B7 | Best, 332 Gospel Voice 34 |
| F02-K24-88 | C8 | Best, 333 Gospel Voice 35 |
| F02-K24-89 | D8 |  |

### F02-K25 - Beat Box Kit (kit 024, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K25-01 | A0 |  |
| F02-K25-02 | A#0 |  |
| F02-K25-03 | B0 |  |
| F02-K25-04 | C1 | ROM, 315 Flexatone |
| F02-K25-05 | C#1 | ROM, 257 Guiro-Tap ROM, 258 Maracas-Push |
| F02-K25-06 | D1 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K25-07 | D#1 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K25-08 | E1 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K25-09 | F1 | ROM, 291 Tambourine-Acc.2 |
| F02-K25-10 | F#1 | ROM, 288 Tambourine-Push ROM, 289 Tambourine-Pull |
| F02-K25-11 | G1 | ROM, 310 Rap Sleighbell |
| F02-K25-12 | G#1 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K25-13 | A1 | ROM, 312 Marc Tree ROM, 312 Marc Tree |
| F02-K25-14 | A#1 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K25-15 | B1 | Vint, 48 Perc BD-EXB05 Vint, 48 Perc BD-EXB05 |
| F02-K25-16 | C2 | Vint, 6 BDEX-EXB05 04 Vint, 6 BDEX-EXB05 04 |
| F02-K25-17 | C#2 | Vint, 3 BDEX-EXB05 01 Vint, 3 BDEX-EXB05 01 |
| F02-K25-18 | D2 | Vint, 1 BD-EXB05 01 Vint, 1 BD-EXB05 01 |
| F02-K25-19 | D#2 | Vint, 0 BD-EXB05 00 Vint, 0 BD-EXB05 00 |
| F02-K25-20 | E2 | Vint, 9 BDEX-EXB05 07 Vint, 9 BDEX-EXB05 07 |
| F02-K25-21 | F2 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K25-22 | F#2 | Vint, 13 SD-EXB05 02 Vint, 13 SD-EXB05 02 |
| F02-K25-23 | G2 | Vint, 18 SDEX-EXB05 01 Vint, 18 SDEX-EXB05 01 |
| F02-K25-24 | G#2 | Vint, 12 SD-EXB05 01 Vint, 12 SD-EXB05 01 |
| F02-K25-25 | A2 | Vint, 16 SD-EXB05 05 |
| F02-K25-26 | A#2 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K25-27 | B2 | Vint, 2 BDEX-EXB05 00 Vint, 2 BDEX-EXB05 00 |
| F02-K25-28 | C3 | ROM, 175 88-BD 3 ROM, 175 88-BD 3 |
| F02-K25-29 | C#3 | Vint, 17 SDEX-EXB05 00 Vint, 17 SDEX-EXB05 00 |
| F02-K25-30 | D3 | Vint, 21 SDEX-EXB05 04 Vint, 21 SDEX-EXB05 04 |
| F02-K25-31 | D#3 | ROM, 186 88-Claps ROM, 186 88-Claps |
| F02-K25-32 | E3 | Vint, 17 SDEX-EXB05 00 Vint, 17 SDEX-EXB05 00 |
| F02-K25-33 | F3 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K25-34 | F#3 | Vint, 28 H.H.Cl-EXB05 02 Vint, 28 H.H.Cl-EXB05 02 |
| F02-K25-35 | G3 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K25-36 | G#3 | ROM, 188 88-HHat Open ROM, 188 88-HHat Open |
| F02-K25-37 | A3 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K25-38 | A#3 | Vint, 39 H.H.Ho-EXB05 03 Vint, 39 H.H.Ho-EXB05 03 |
| F02-K25-39 | B3 | ROM, 150 H.Hat-Whispy |
| F02-K25-40 | C4 | Vint, 38 H.H.Ho-EXB05 02 Vint, 38 H.H.Ho-EXB05 02 |
| F02-K25-41 | C#4 | Vint, 36 H.H.Ho-EXB05 00 Vint, 36 H.H.Ho-EXB05 00 |
| F02-K25-42 | D4 | ROM, 188 88-HHat Open ROM, 188 88-HHat Open |
| F02-K25-43 | D#4 | ROM, 160 Ride-Dance 99 ROM, 160 Ride-Dance 99 |
| F02-K25-44 | E4 | ROM, 153 Crash Cymbal 2 |
| F02-K25-45 | F4 | ROM, 161 Ride-Dance DDD1 ROM, 161 Ride-Dance DDD1 |
| F02-K25-46 | F#4 | Vint, 39 H.H.Ho-EXB05 03 Vint, 39 H.H.Ho-EXB05 03 |
| F02-K25-47 | G4 | ROM, 157 Cymbal Reverse ROM, 157 Cymbal Reverse |
| F02-K25-48 | G#4 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K25-49 | A4 | ROM, 158 Crash-Dance 99 ROM, 158 Crash-Dance 99 |
| F02-K25-50 | A#4 | Vint, 43 Perc Hi-EXB05 00 Vint, 42 Perc MH-EXB05 00 |
| F02-K25-51 | B4 | Vint, 39 H.H.Ho-EXB05 03 Vint, 39 H.H.Ho-EXB05 03 |
| F02-K25-52 | C5 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K25-53 | C#5 | Vint, 40 Perc Hi-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K25-54 | D5 | Vint, 55 Clave-EXB05 00 |
| F02-K25-55 | D#5 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K25-56 | E5 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K25-57 | F5 | Vint, 54 Metal-EXB05 01 Vint, 53 Metal-EXB05 00 |
| F02-K25-58 | F#5 | Vint, 29 H.H.Cl-EXB05 03 |
| F02-K25-59 | G5 | ROM, 311 Finger Cymbal |
| F02-K25-60 | G#5 | ROM, 127 H.Hat 2-Foot |
| F02-K25-61 | A5 | ROM, 204 Dance Perc-Hous2 |
| F02-K25-62 | A#5 | ROM, 196 88-Maraca |
| F02-K25-63 | B5 | ROM, 199 Vocal SD ROM, 199 Vocal SD |
| F02-K25-64 | C6 | ROM, 377 Car Crash |
| F02-K25-65 | C#6 | ROM, 293 Triangle-Mute |
| F02-K25-66 | D6 | Vint, 46 Perc Lo-EXB05 00 Vint, 47 Perc Lo-EXB05 01 |
| F02-K25-67 | D#6 | ROM, 292 Triangle-Open |
| F02-K25-68 | E6 | ROM, 400 GtCutNois1 |
| F02-K25-69 | F6 | ROM, 146 H.Hat-Hip 2 |
| F02-K25-70 | F#6 | ROM, 151 H.Hat-Alpo Close ROM, 151 H.Hat-Alpo Close |
| F02-K25-71 | G6 | ROM, 402 StringSlap |
| F02-K25-72 | G#6 | ROM, 147 H.Hat-AmbCrackle |
| F02-K25-73 | A6 | ROM, 406 Dist.Slide 1 |
| F02-K25-74 | A#6 | Vint, 39 H.H.Ho-EXB05 03 |
| F02-K25-75 | B6 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K25-76 | C7 | ROM, 391 Swish Terra |
| F02-K25-77 | C#7 | Vint, 36 H.H.Ho-EXB05 00 Vint, 36 H.H.Ho-EXB05 00 |
| F02-K25-78 | D7 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K25-79 | D#7 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K25-80 | E7 | ROM, 238 Conga Hi-Slap 1 ROM, 242 Bongo Lo-Open |
| F02-K25-81 | F7 | ROM, 235 Conga Hi-Open ROM, 233 Conga Lo-MtSlap |
| F02-K25-82 | F#7 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K25-83 | G7 | ROM, 239 Conga Hi-Slap 2 ROM, 236 Conga Hi-Mute |
| F02-K25-84 | G#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K25-85 | A7 | ROM, 307 Chacha Bell ROM, 307 Chacha Bell |
| F02-K25-86 | A#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K25-87 | B7 | ROM, 256 Guiro-Short |
| F02-K25-88 | C8 | ROM, 257 Guiro-Tap ROM, 257 Guiro-Tap |
| F02-K25-89 | C#8 | ROM, 324 Metal Hit |
| F02-K25-90 | D8 | ROM, 202 Dance Perc-Hip |

### F02-K26 - Lounge Kit (kit 025, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K26-01 | A0 |  |
| F02-K26-02 | A#0 |  |
| F02-K26-03 | B0 |  |
| F02-K26-04 | C1 | ROM, 275 Taiko-Rim ROM, 274 Taiko-Open |
| F02-K26-05 | C#1 | ROM, 276 Tsuzumi ROM, 276 Tsuzumi |
| F02-K26-06 | D1 | ROM, 249 Djembe-Open ROM, 249 Djembe-Open |
| F02-K26-07 | D#1 | ROM, 251 Djembe-Slap ROM, 250 Djembe-Mute |
| F02-K26-08 | E1 | ROM, 261 Baya-Ghe ROM, 260 Baya-Open |
| F02-K26-09 | F1 | ROM, 262 Baya-Mute 1 ROM, 264 Baya-Mute 3 |
| F02-K26-10 | F#1 | ROM, 266 Baya-Mute 5 ROM, 265 Baya-Mute 4 |
| F02-K26-11 | G1 | ROM, 268 Tabla-Open ROM, 268 Tabla-Open |
| F02-K26-12 | G#1 | ROM, 269 Tabla-Tin ROM, 273 Tabla-Mute 4 |
| F02-K26-13 | A1 | ROM, 227 Drumstick Hit |
| F02-K26-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K26-15 | B1 | ROM, 73 SD-Reverse |
| F02-K26-16 | C2 | Vint, 9 BDEX-EXB05 07 Vint, 9 BDEX-EXB05 07 |
| F02-K26-17 | C#2 | Vint, 8 BDEX-EXB05 06 Vint, 8 BDEX-EXB05 06 |
| F02-K26-18 | D2 | Vint, 0 BD-EXB05 00 Vint, 0 BD-EXB05 00 |
| F02-K26-19 | D#2 | Vint, 5 BDEX-EXB05 03 Vint, 5 BDEX-EXB05 03 |
| F02-K26-20 | E2 | ROM, 173 88-BD 1(Rap) |
| F02-K26-21 | F2 | Vint, 12 SD-EXB05 01 Vint, 11 SD-EXB05 00 |
| F02-K26-22 | F#2 | Vint, 20 SDEX-EXB05 03 Vint, 20 SDEX-EXB05 03 |
| F02-K26-23 | G2 | Vint, 25 SDEX-EXB05 09 |
| F02-K26-24 | G#2 | Vint, 16 SD-EXB05 05 |
| F02-K26-25 | A2 | Vint, 18 SDEX-EXB05 01 |
| F02-K26-26 | A#2 | Vint, 21 SDEX-EXB05 04 Vint, 21 SDEX-EXB05 04 |
| F02-K26-27 | B2 | Vint, 46 Perc Lo-EXB05 00 |
| F02-K26-28 | C3 | Vint, 48 Perc BD-EXB05 Vint, 48 Perc BD-EXB05 |
| F02-K26-29 | C#3 | Vint, 55 Clave-EXB05 00 |
| F02-K26-30 | D3 | Vint, 25 SDEX-EXB05 09 |
| F02-K26-31 | D#3 | Vint, 11 SD-EXB05 00 Vint, 11 SD-EXB05 00 |
| F02-K26-32 | E3 | ROM, 182 88-SD 2(Old) ROM, 182 88-SD 2(Old) |
| F02-K26-33 | F3 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K26-34 | F#3 | Vint, 35 H.H.Cl-EXB05 09 Vint, 35 H.H.Cl-EXB05 09 |
| F02-K26-35 | G3 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K26-36 | G#3 | Vint, 27 H.H.Cl-EXB05 01 Vint, 27 H.H.Cl-EXB05 01 |
| F02-K26-37 | A3 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K26-38 | A#3 | Vint, 36 H.H.Ho-EXB05 00 |
| F02-K26-39 | B3 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K26-40 | C4 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K26-41 | C#4 | ROM, 152 Crash Cymbal 1 |
| F02-K26-42 | D4 | Vint, 41 Perc MH-EXB05 01 Vint, 41 Perc MH-EXB05 01 |
| F02-K26-43 | D#4 | ROM, 162 Ride-Edge 1 |
| F02-K26-44 | E4 | ROM, 153 Crash Cymbal 2 |
| F02-K26-45 | F4 | ROM, 157 Cymbal Reverse |
| F02-K26-46 | F#4 | ROM, 164 Ride-Jazz |
| F02-K26-47 | G4 | ROM, 156 Splash Cymbal |
| F02-K26-48 | G#4 | ROM, 195 88-Cowbell |
| F02-K26-49 | A4 | ROM, 191 88-Crash |
| F02-K26-50 | A#4 | ROM, 229 Hand Claps ROM, 229 Hand Claps |
| F02-K26-51 | B4 | Vint, 28 H.H.Cl-EXB05 02 Vint, 28 H.H.Cl-EXB05 02 |
| F02-K26-52 | C5 | ROM, 245 Bongo Hi-Open ROM, 245 Bongo Hi-Open |
| F02-K26-53 | C#5 | Vint, 40 Perc Hi-EXB05 01 Vint, 40 Perc Hi-EXB05 01 |
| F02-K26-54 | D5 | ROM, 237 Conga Hi-MtSlap ROM, 241 Conga-Toe |
| F02-K26-55 | D#5 | Vint, 44 Perc Hi-EXB05 02 Vint, 44 Perc Hi-EXB05 02 |
| F02-K26-56 | E5 | Vint, 45 Perc ML-EXB05 00 Vint, 45 Perc ML-EXB05 00 |
| F02-K26-57 | F5 | Vint, 50 Perc B-EXB05 Vint, 50 Perc B-EXB05 |
| F02-K26-58 | F#5 | Vint, 51 Perc C-EXB05 Vint, 51 Perc C-EXB05 |
| F02-K26-59 | G5 | Vint, 54 Metal-EXB05 01 |
| F02-K26-60 | G#5 | Vint, 53 Metal-EXB05 00 |
| F02-K26-61 | A5 | ROM, 299 Cabasa-Up ROM, 300 Cabasa-Down |
| F02-K26-62 | A#5 | ROM, 258 Maracas-Push ROM, 258 Maracas-Push |
| F02-K26-63 | B5 | ROM, 311 Finger Cymbal |
| F02-K26-64 | C6 | ROM, 310 Rap Sleighbell |
| F02-K26-65 | C#6 | ROM, 256 Guiro-Short ROM, 257 Guiro-Tap |
| F02-K26-66 | D6 | ROM, 257 Guiro-Tap |
| F02-K26-67 | D#6 | ROM, 278 Claves |
| F02-K26-68 | E6 | ROM, 279 Woodblock 1 |
| F02-K26-69 | F6 | ROM, 279 Woodblock 1 |
| F02-K26-70 | F#6 | ROM, 280 Woodblock 2 |
| F02-K26-71 | G6 | ROM, 279 Woodblock 1 |
| F02-K26-72 | G#6 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |
| F02-K26-73 | A6 | ROM, 292 Triangle-Open |
| F02-K26-74 | A#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K26-75 | B6 | ROM, 312 Marc Tree |
| F02-K26-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K26-77 | C#7 | ROM, 187 88-HHat Closed ROM, 187 88-HHat Closed |
| F02-K26-78 | D7 | ROM, 181 88-SD 1(New) ROM, 181 88-SD 1(New) |
| F02-K26-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K26-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K26-81 | F7 | ROM, 195 88-Cowbell ROM, 195 88-Cowbell |
| F02-K26-82 | F#7 | ROM, 186 88-Claps |
| F02-K26-83 | G7 | ROM, 196 88-Maraca |
| F02-K26-84 | G#7 | Vint, 3 BDEX-EXB05 01 |
| F02-K26-85 | A7 | Vint, 39 H.H.Ho-EXB05 03 Vint, 39 H.H.Ho-EXB05 03 |
| F02-K26-86 | A#7 | ROM, 314 Bell Tree |
| F02-K26-87 | B7 | ROM, 354 Stadium |
| F02-K26-88 | C8 | ROM, 355 Applause |
| F02-K26-89 | C#8 |  |
| F02-K26-90 | D8 |  |

### F02-K27 - TekNo Kit 1 (kit 026, D1 p27)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K27-01 | A0 |  |
| F02-K27-02 | A#0 |  |
| F02-K27-03 | B0 |  |
| F02-K27-04 | C1 | Synth, 29 BD-EXB09 30 Synth, 29 BD-EXB09 30 |
| F02-K27-05 | C#1 | Synth, 3 BD-EXB09 04 Synth, 3 BD-EXB09 04 |
| F02-K27-06 | D1 | Synth, 4 BD-EXB09 05 Synth, 4 BD-EXB09 05 |
| F02-K27-07 | D#1 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K27-08 | E1 | Synth, 49 SD-EXB09 10 Synth, 49 SD-EXB09 10 |
| F02-K27-09 | F1 | Synth, 167Vo-09 Baaaa Synth, 167Vo-09 Baaaa |
| F02-K27-10 | F#1 | Synth, 166Vo-09 Kaaaa Synth, 169Vo-09 Click |
| F02-K27-11 | G1 | Synth, 160Vo-09 Ahhhh Synth, 160Vo-09 Ahhhh |
| F02-K27-12 | G#1 | Synth, 168Vo-09 Hrrrr Synth, 166Vo-09 Kaaaa |
| F02-K27-13 | A1 | Synth, 161Vo-09 Ohhhh |
| F02-K27-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K27-15 | B1 | Synth, 162Vo-09 Ooooh |
| F02-K27-16 | C2 | Synth, 11 BD-EXB09 12 Synth, 0 BD-EXB09 01 |
| F02-K27-17 | C#2 | Synth, 32 BD-EXB09 33 Synth, 27 BD-EXB09 28 |
| F02-K27-18 | D2 | Synth, 17 BD-EXB09 18 Synth, 8 BD-EXB09 09 |
| F02-K27-19 | D#2 | Synth, 31 BD-EXB09 32 Synth, 5 BD-EXB09 06 |
| F02-K27-20 | E2 | Synth, 11 BD-EXB09 12 Synth, 8 BD-EXB09 09 |
| F02-K27-21 | F2 | Synth, 51 SD-EXB09 12 Synth, 51 SD-EXB09 12 |
| F02-K27-22 | F#2 | Synth, 53 SD-EXB09 14 |
| F02-K27-23 | G2 | Synth, 61 Clap-EXB09 05 |
| F02-K27-24 | G#2 | Synth, 62 Clap-EXB09 06 |
| F02-K27-25 | A2 | Synth, 166Vo-09 Kaaaa Synth, 165Vo-09 Kkkkk |
| F02-K27-26 | A#2 | Synth, 168Vo-09 Hrrrr |
| F02-K27-27 | B2 | Synth, 3 BD-EXB09 04 Synth, 18 BD-EXB09 19 |
| F02-K27-28 | C3 | Synth, 25 BD-EXB09 26 Synth, 23 BD-EXB09 24 |
| F02-K27-29 | C#3 | Synth, 139Sidestick-EXB09 |
| F02-K27-30 | D3 | Synth, 45 SD-EXB09 06 |
| F02-K27-31 | D#3 | Synth, 57 Clap-EXB09 01 Synth, 63 Clap-EXB09 07 |
| F02-K27-32 | E3 | Synth, 40 SD-EXB09 01 Synth, 40 SD-EXB09 01 |
| F02-K27-33 | F3 | Synth, 112Perc-Tom-EXB09 3 |
| F02-K27-34 | F#3 | Synth, 74 HH04-Cl-EXB09 Synth, 74 HH04-Cl-EXB09 |
| F02-K27-35 | G3 | Synth, 112Perc-Tom-EXB09 3 |
| F02-K27-36 | G#3 | Synth, 70 HH02-Op-EXB09 01 Synth, 70 HH02-Op-EXB09 01 |
| F02-K27-37 | A3 | Synth, 112Perc-Tom-EXB09 3 |
| F02-K27-38 | A#3 | Synth, 66 HH01-Op-EXB09 01 Synth, 66 HH01-Op-EXB09 01 |
| F02-K27-39 | B3 | Synth, 137Conga02-Hs-EX9 1 ROM, 117 Tom-Old R&B |
| F02-K27-40 | C4 | Synth, 137Conga02-Hs-EX9 1 ROM, 117 Tom-Old R&B |
| F02-K27-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K27-42 | D4 | Synth, 137Conga02-Hs-EX9 1 ROM, 117 Tom-Old R&B |
| F02-K27-43 | D#4 | Synth, 83 Ride-EXB09 01 Synth, 84 Ride-EXB09 02 |
| F02-K27-44 | E4 | Synth, 86 Crash-EXB09 02 |
| F02-K27-45 | F4 | Synth, 86 Crash-EXB09 02 Synth, 86 Crash-EXB09 02 |
| F02-K27-46 | F#4 | Synth, 128Perc-Junk-EXB9 8 Synth, 128Perc-Junk-EXB9 8 |
| F02-K27-47 | G4 | Synth, 162Vo-09 Ooooh |
| F02-K27-48 | G#4 | Synth, 216Vo-09 Com'On Synth, 218Vo-09 Pardy |
| F02-K27-49 | A4 | Synth, 202Vo-09 Ha-ooo-wa |
| F02-K27-50 | A#4 | Synth, 215Vo-09 C'mon |
| F02-K27-51 | B4 | Synth, 234Vo-09 Be |
| F02-K27-52 | C5 | Synth, 185Vo-09 What |
| F02-K27-53 | C#5 | Synth, 186Vo-09 Ever |
| F02-K27-54 | D5 | Synth, 184Vo-09 You-are-h |
| F02-K27-55 | D#5 | Synth, 172Vo-09 Oh-Yeah M |
| F02-K27-56 | E5 | Synth, 157Vo-09 Yeah M2 |
| F02-K27-57 | F5 | Synth, 155Vo-09 Bass |
| F02-K27-58 | F#5 | Synth, 149Vo-09 Ha |
| F02-K27-59 | G5 | Synth, 210Vo-09 Uuh |
| F02-K27-60 | G#5 | Synth, 162Vo-09 Ooooh |
| F02-K27-61 | A5 | Synth, 144Glass-EXB09 Synth, 144Glass-EXB09 |
| F02-K27-62 | A#5 | Synth, 108Perc07-Cl-EXB09 |
| F02-K27-63 | B5 | Synth, 149Vo-09 Ha |
| F02-K27-64 | C6 | Synth, 159Vo-09 Let's Go |
| F02-K27-65 | C#6 | Synth, 148Vo-09 Love |
| F02-K27-66 | D6 | Synth, 193Vo-09 Wo'I dance |
| F02-K27-67 | D#6 | Synth, 208Vo-09 Mmmm |
| F02-K27-68 | E6 | Synth, 226Vo-09 Aa-ow |
| F02-K27-69 | F6 | Synth, 178Vo-09 Ya-ee |
| F02-K27-70 | F#6 | Synth, 157Vo-09 Yeah M2 |
| F02-K27-71 | G6 | Synth, 159Vo-09 Let's Go |
| F02-K27-72 | G#6 | Synth, 210Vo-09 Uuh Synth, 210Vo-09 Uuh |
| F02-K27-73 | A6 | Synth, 239Vo-09 Go Synth, 239Vo-09 Go |
| F02-K27-74 | A#6 | Synth, 226Vo-09 Aa-ow |
| F02-K27-75 | B6 | Synth, 211Vo-09 Uh-a-ha |
| F02-K27-76 | C7 | Synth, 157Vo-09 Yeah M2 |
| F02-K27-77 | C#7 | Synth, 214Vo-09 Haaay |
| F02-K27-78 | D7 | Synth, 233Vo-09 I wanna |
| F02-K27-79 | D#7 | Synth, 234Vo-09 Be |
| F02-K27-80 | E7 | Synth, 197Vo-09 Go-o |
| F02-K27-81 | F7 | Synth, 189Vo-09 CaMon |
| F02-K27-82 | F#7 | Synth, 190Vo-09 Do It |
| F02-K27-83 | G7 | Synth, 147Vo-09 Wild |
| F02-K27-84 | G#7 | Synth, 173Vo-09 Uagh |
| F02-K27-85 | A7 | Synth, 154Vo-09 Break itUp |
| F02-K27-86 | A#7 | Synth, 218Vo-09 Pardy |
| F02-K27-87 | B7 | Synth, 156Vo-09 Yeah M1 |
| F02-K27-88 | C8 | Synth, 152Vo-09 Funky 2 |
| F02-K27-89 | C#8 | 27 |
| F02-K27-90 | D8 |  |

### F02-K28 - TekNo Kit 2 (kit 027, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K28-01 | A0 |  |
| F02-K28-02 | A#0 |  |
| F02-K28-03 | B0 |  |
| F02-K28-04 | C1 | Synth, 29 BD-EXB09 30 Synth, 29 BD-EXB09 30 |
| F02-K28-05 | C#1 | Synth, 3 BD-EXB09 04 Synth, 3 BD-EXB09 04 |
| F02-K28-06 | D1 | Synth, 4 BD-EXB09 05 Synth, 4 BD-EXB09 05 |
| F02-K28-07 | D#1 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K28-08 | E1 | Synth, 49 SD-EXB09 10 Synth, 49 SD-EXB09 10 |
| F02-K28-09 | F1 | Synth, 167Vo-09 Baaaa Synth, 167Vo-09 Baaaa |
| F02-K28-10 | F#1 | Synth, 166Vo-09 Kaaaa Synth, 169Vo-09 Click |
| F02-K28-11 | G1 | Synth, 160Vo-09 Ahhhh Synth, 160Vo-09 Ahhhh |
| F02-K28-12 | G#1 | Synth, 168Vo-09 Hrrrr Synth, 166Vo-09 Kaaaa |
| F02-K28-13 | A1 | Synth, 161Vo-09 Ohhhh |
| F02-K28-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K28-15 | B1 | Synth, 162Vo-09 Ooooh |
| F02-K28-16 | C2 | Synth, 17 BD-EXB09 18 |
| F02-K28-17 | C#2 | Synth, 29 BD-EXB09 30 Synth, 28 BD-EXB09 29 |
| F02-K28-18 | D2 | Synth, 15 BD-EXB09 16 Synth, 7 BD-EXB09 08 |
| F02-K28-19 | D#2 | Synth, 21 BD-EXB09 22 Synth, 21 BD-EXB09 22 |
| F02-K28-20 | E2 | Synth, 24 BD-EXB09 25 Synth, 24 BD-EXB09 25 |
| F02-K28-21 | F2 | Synth, 50 SD-EXB09 11 |
| F02-K28-22 | F#2 | Synth, 47 SD-EXB09 08 |
| F02-K28-23 | G2 | Synth, 56 SD-Jungle-EXB09 |
| F02-K28-24 | G#2 | Synth, 40 SD-EXB09 01 |
| F02-K28-25 | A2 | Synth, 55 SD-EXB09 16 |
| F02-K28-26 | A#2 | Synth, 54 SD-EXB09 15 |
| F02-K28-27 | B2 | Synth, 18 BD-EXB09 19 Synth, 18 BD-EXB09 19 |
| F02-K28-28 | C3 | Synth, 0 BD-EXB09 01 Synth, 25 BD-EXB09 26 |
| F02-K28-29 | C#3 | Synth, 52 SD-EXB09 13 |
| F02-K28-30 | D3 | Synth, 42 SD-EXB09 03 |
| F02-K28-31 | D#3 | Synth, 60 Clap-EXB09 04 Synth, 62 Clap-EXB09 06 |
| F02-K28-32 | E3 | Synth, 48 SD-EXB09 09 Synth, 48 SD-EXB09 09 |
| F02-K28-33 | F3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K28-34 | F#3 | Synth, 68 HH02-Cl-EXB09 Synth, 64 HH01-Cl-EXB09 |
| F02-K28-35 | G3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K28-36 | G#3 | Synth, 73 HH03-Op-EXB09 Synth, 73 HH03-Op-EXB09 |
| F02-K28-37 | A3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K28-38 | A#3 | Synth, 66 HH01-Op-EXB09 01 Synth, 66 HH01-Op-EXB09 01 |
| F02-K28-39 | B3 | Synth, 113Perc-Tom-EXB09 4 ROM, 117 Tom-Old R&B |
| F02-K28-40 | C4 | Synth, 113Perc-Tom-EXB09 4 ROM, 117 Tom-Old R&B |
| F02-K28-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K28-42 | D4 | Synth, 113Perc-Tom-EXB09 4 ROM, 117 Tom-Old R&B |
| F02-K28-43 | D#4 | Synth, 83 Ride-EXB09 01 Synth, 84 Ride-EXB09 02 |
| F02-K28-44 | E4 | Synth, 87 Crash-EXB09 03 |
| F02-K28-45 | F4 | Synth, 86 Crash-EXB09 02 Synth, 96 Perc02-Op-EXB09 |
| F02-K28-46 | F#4 | Synth, 127Perc-Junk-EXB9 7 Synth, 127Perc-Junk-EXB9 7 |
| F02-K28-47 | G4 | Synth, 88 CrashJungle-EXB9 |
| F02-K28-48 | G#4 | ROM, 305 Cowbell 1 |
| F02-K28-49 | A4 | Synth, 39 BBD-EXB09 02 Synth, 38 BBD-EXB09 01 |
| F02-K28-50 | A#4 | Synth, 107Perc06-Op-EXB09 Synth, 107Perc06-Op-EXB09 |
| F02-K28-51 | B4 | Synth, 90 Cym-DDD1-L-EXB09 Synth, 90 Cym-DDD1-L-EXB09 |
| F02-K28-52 | C5 | Synth, 132Conga01-L-EXB09 |
| F02-K28-53 | C#5 | Synth, 133Conga01-H-EXB09 |
| F02-K28-54 | D5 | Synth, 137 Conga02-Hs-EX9 1 Synth, 135 Conga02-Ho-EX9 1 |
| F02-K28-55 | D#5 | Synth, 134 Conga01-Hm-EXB09 |
| F02-K28-56 | E5 | Synth, 138Conga02-Hs-EX9 2 |
| F02-K28-57 | F5 | ROM, 285 Timbale Hi-Rim 1 ROM, 284 Timbale Hi-Edge |
| F02-K28-58 | F#5 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K28-59 | G5 | Synth, 126Perc-Junk-EXB9 6 |
| F02-K28-60 | G#5 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K28-61 | A5 | Synth, 129Perc-Junk-EXB9 9 Synth, 129Perc-Junk-EXB9 9 |
| F02-K28-62 | A#5 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K28-63 | B5 | Synth, 146Vo-09 Hey M |
| F02-K28-64 | C6 | Synth, 158Vo-09 Bang |
| F02-K28-65 | C#6 | Synth, 151Vo-09 Funky 1 |
| F02-K28-66 | D6 | Synth, 182Vo-09 Ay-ay-he |
| F02-K28-67 | D#6 | Synth, 201Vo-09 Dance |
| F02-K28-68 | E6 | Synth, 232Vo-09 Don't Stop |
| F02-K28-69 | F6 | Synth, 171Vo-09 Club |
| F02-K28-70 | F#6 | Synth, 149Vo-09 Ha Synth, 150Vo-09 Ready |
| F02-K28-71 | G6 | Synth, 148Vo-09 Love |
| F02-K28-72 | G#6 | Synth, 174Vo-09 Hah |
| F02-K28-73 | A6 | Synth, 183Vo-09 Aow |
| F02-K28-74 | A#6 | Synth, 239Vo-09 Go Synth, 224Vo-09 Owww |
| F02-K28-75 | B6 | Synth, 179Vo-09 Ya-ee-ha |
| F02-K28-76 | C7 | Synth, 155Vo-09 Bass |
| F02-K28-77 | C#7 | Synth, 214Vo-09 Haaay |
| F02-K28-78 | D7 | Synth, 233Vo-09 I wanna |
| F02-K28-79 | D#7 | Synth, 234Vo-09 Be |
| F02-K28-80 | E7 | Synth, 197Vo-09 Go-o |
| F02-K28-81 | F7 | Synth, 189Vo-09 CaMon |
| F02-K28-82 | F#7 | Synth, 190Vo-09 Do It |
| F02-K28-83 | G7 | Synth, 147Vo-09 Wild |
| F02-K28-84 | G#7 | Synth, 173Vo-09 Uagh |
| F02-K28-85 | A7 | Synth, 154Vo-09 Break itUp |
| F02-K28-86 | A#7 | Synth, 218Vo-09 Pardy |
| F02-K28-87 | B7 | Synth, 156Vo-09 Yeah M1 |
| F02-K28-88 | C8 | Synth, 152Vo-09 Funky 2 |
| F02-K28-89 | D8 |  |

### F02-K29 - Trance Kit 1 (kit 028, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K29-01 | A0 |  |
| F02-K29-02 | A#0 |  |
| F02-K29-03 | B0 |  |
| F02-K29-04 | C1 | Synth, 129Perc-Junk-EXB9 9 |
| F02-K29-05 | C#1 | Synth, 114Perc-Rim-EXB09 1 |
| F02-K29-06 | D1 | ROM, 317 Chinese Gong |
| F02-K29-07 | D#1 | ROM, 166 BD-Orchestral |
| F02-K29-08 | E1 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K29-09 | F1 | ROM, 291 Tambourine-Acc.2 |
| F02-K29-10 | F#1 | ROM, 289 Tambourine-Pull |
| F02-K29-11 | G1 | ROM, 288 Tambourine-Push |
| F02-K29-12 | G#1 | Synth, 60 Clap-EXB09 04 |
| F02-K29-13 | A1 | ROM, 312 Marc Tree |
| F02-K29-14 | A#1 | Synth, 10 BD-EXB09 11 |
| F02-K29-15 | B1 | Synth, 20 BD-EXB09 21 |
| F02-K29-16 | C2 | Synth, 38 BBD-EXB09 01 |
| F02-K29-17 | C#2 | Synth, 39 BBD-EXB09 02 |
| F02-K29-18 | D2 | Synth, 0 BD-EXB09 01 |
| F02-K29-19 | D#2 | Synth, 1 BD-EXB09 02 |
| F02-K29-20 | E2 | Synth, 2 BD-EXB09 03 |
| F02-K29-21 | F2 | Synth, 42 SD-EXB09 03 |
| F02-K29-22 | F#2 | Synth, 44 SD-EXB09 05 |
| F02-K29-23 | G2 | Synth, 45 SD-EXB09 06 |
| F02-K29-24 | G#2 | Synth, 46 SD-EXB09 07 |
| F02-K29-25 | A2 | Synth, 55 SD-EXB09 16 |
| F02-K29-26 | A#2 | Synth, 40 SD-EXB09 01 |
| F02-K29-27 | B2 | Synth, 9 BD-EXB09 10 |
| F02-K29-28 | C3 | Synth, 0 BD-EXB09 01 |
| F02-K29-29 | C#3 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K29-30 | D3 | ROM, 182 88-SD 2(Old) |
| F02-K29-31 | D#3 | Synth, 59 Clap-EXB09 03 |
| F02-K29-32 | E3 | ROM, 184 99-SD 2 |
| F02-K29-33 | F3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K29-34 | F#3 | Synth, 64 HH01-Cl-EXB09 |
| F02-K29-35 | G3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K29-36 | G#3 | Synth, 65 HH01-Ho-EXB09 |
| F02-K29-37 | A3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K29-38 | A#3 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K29-39 | B3 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K29-40 | C4 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K29-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K29-42 | D4 | Synth, 83 Ride-EXB09 01 |
| F02-K29-43 | D#4 | ROM, 161 Ride-Dance DDD1 |
| F02-K29-44 | E4 | ROM, 158 Crash-Dance 99 |
| F02-K29-45 | F4 | ROM, 164 Ride-Jazz |
| F02-K29-46 | F#4 | ROM, 291 Tambourine-Acc.2 |
| F02-K29-47 | G4 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K29-48 | G#4 | Synth, 160Vo-09 Ahhhh Synth, 161Vo-09 Ohhhh |
| F02-K29-49 | A4 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K29-50 | A#4 | Synth, 162Vo-09 Ooooh Synth, 163Vo-09 Aaaah |
| F02-K29-51 | B4 | ROM, 157 Cymbal Reverse ROM, 157 Cymbal Reverse |
| F02-K29-52 | C5 | Synth, 1 BD-EXB09 02 |
| F02-K29-53 | C#5 | Synth, 107Perc06-Op-EXB09 Synth, 107Perc06-Op-EXB09 |
| F02-K29-54 | D5 | Synth, 103Perc-Ambi-EXB09 |
| F02-K29-55 | D#5 | ROM, 267 Tabla-Na |
| F02-K29-56 | E5 | ROM, 268 Tabla-Open |
| F02-K29-57 | F5 | ROM, 261 Baya-Ghe |
| F02-K29-58 | F#5 | Synth, 78 HH05-Cl-EXB09 |
| F02-K29-59 | G5 | ROM, 260 Baya-Open |
| F02-K29-60 | G#5 | Synth, 80 HH05-Op-EXB09 |
| F02-K29-61 | A5 | Synth, 94 Perc01-Op-EXB09 |
| F02-K29-62 | A#5 | Synth, 104Perc05-Cl-EXB09 |
| F02-K29-63 | B5 | Synth, 105Perc05-Op-EXB09 |
| F02-K29-64 | C6 | Synth, 62 Clap-EXB09 06 |
| F02-K29-65 | C#6 | ROM, 293 Triangle-Mute |
| F02-K29-66 | D6 | Synth, 132Conga01-L-EXB09 |
| F02-K29-67 | D#6 | ROM, 292 Triangle-Open |
| F02-K29-68 | E6 | Synth, 133Conga01-H-EXB09 |
| F02-K29-69 | F6 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K29-70 | F#6 | Synth, 74 HH04-Cl-EXB09 |
| F02-K29-71 | G6 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K29-72 | G#6 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K29-73 | A6 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K29-74 | A#6 | Synth, 76 HH04-Ho-EXB09 02 |
| F02-K29-75 | B6 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K29-76 | C7 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K29-77 | C#7 | Synth, 85 Crash-EXB09 01 |
| F02-K29-78 | D7 | ROM, 207 Zap 3 ROM, 207 Zap 3 |
| F02-K29-79 | D#7 | ROM, 206 Zap 2 ROM, 206 Zap 2 |
| F02-K29-80 | E7 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K29-81 | F7 | Synth, 136Conga02-Ho-EX9 2 |
| F02-K29-82 | F#7 | Synth, 138Conga02-Hs-EX9 2 |
| F02-K29-83 | G7 | Synth, 134 Conga01-Hm-EXB09 |
| F02-K29-84 | G#7 | Synth, 133Conga01-H-EXB09 |
| F02-K29-85 | A7 | Synth, 132Conga01-L-EXB09 |
| F02-K29-86 | A#7 | Synth, 131Woodblock-H-EXB9 |
| F02-K29-87 | B7 | Synth, 137Conga02-Hs-EX9 1 |
| F02-K29-88 | C8 | Synth, 135Conga02-Ho-EX9 1 |
| F02-K29-89 | D8 | ROM, 202 Dance Perc-Hip |

### F02-K30 - Trance Kit 2 (kit 029, D1 p28)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K30-01 | A0 |  |
| F02-K30-02 | A#0 |  |
| F02-K30-03 | B0 |  |
| F02-K30-04 | C1 | Synth, 129Perc-Junk-EXB9 9 |
| F02-K30-05 | C#1 | Synth, 114Perc-Rim-EXB09 1 |
| F02-K30-06 | D1 | ROM, 317 Chinese Gong |
| F02-K30-07 | D#1 | ROM, 166 BD-Orchestral |
| F02-K30-08 | E1 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K30-09 | F1 | ROM, 291 Tambourine-Acc.2 |
| F02-K30-10 | F#1 | ROM, 289 Tambourine-Pull |
| F02-K30-11 | G1 | ROM, 288 Tambourine-Push |
| F02-K30-12 | G#1 | Synth, 60 Clap-EXB09 04 |
| F02-K30-13 | A1 | ROM, 312 Marc Tree |
| F02-K30-14 | A#1 | Synth, 10 BD-EXB09 11 |
| F02-K30-15 | B1 | Synth, 20 BD-EXB09 21 |
| F02-K30-16 | C2 | Synth, 38 BBD-EXB09 01 |
| F02-K30-17 | C#2 | Synth, 39 BBD-EXB09 02 |
| F02-K30-18 | D2 | Synth, 3 BD-EXB09 04 |
| F02-K30-19 | D#2 | Synth, 5 BD-EXB09 06 |
| F02-K30-20 | E2 | Synth, 2 BD-EXB09 03 |
| F02-K30-21 | F2 | Synth, 42 SD-EXB09 03 |
| F02-K30-22 | F#2 | Synth, 41 SD-EXB09 02 |
| F02-K30-23 | G2 | Synth, 45 SD-EXB09 06 |
| F02-K30-24 | G#2 | Synth, 44 SD-EXB09 05 |
| F02-K30-25 | A2 | Synth, 55 SD-EXB09 16 |
| F02-K30-26 | A#2 | Synth, 61 Clap-EXB09 05 |
| F02-K30-27 | B2 | Synth, 9 BD-EXB09 10 |
| F02-K30-28 | C3 | Synth, 0 BD-EXB09 01 |
| F02-K30-29 | C#3 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K30-30 | D3 | ROM, 182 88-SD 2(Old) |
| F02-K30-31 | D#3 | Synth, 59 Clap-EXB09 03 |
| F02-K30-32 | E3 | ROM, 184 99-SD 2 |
| F02-K30-33 | F3 | ROM, 198 Real E.Tom |
| F02-K30-34 | F#3 | Synth, 74 HH04-Cl-EXB09 |
| F02-K30-35 | G3 | ROM, 198 Real E.Tom |
| F02-K30-36 | G#3 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K30-37 | A3 | ROM, 198 Real E.Tom |
| F02-K30-38 | A#3 | Synth, 77 HH04-Op-EXB09 |
| F02-K30-39 | B3 | ROM, 198 Real E.Tom |
| F02-K30-40 | C4 | ROM, 198 Real E.Tom |
| F02-K30-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K30-42 | D4 | Synth, 144Glass-EXB09 |
| F02-K30-43 | D#4 | ROM, 161 Ride-Dance DDD1 |
| F02-K30-44 | E4 | ROM, 158 Crash-Dance 99 |
| F02-K30-45 | F4 | ROM, 164 Ride-Jazz |
| F02-K30-46 | F#4 | ROM, 291 Tambourine-Acc.2 |
| F02-K30-47 | G4 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K30-48 | G#4 | Synth, 210Vo-09 Uuh |
| F02-K30-49 | A4 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K30-50 | A#4 | Synth, 211Vo-09 Uh-a-ha |
| F02-K30-51 | B4 | Synth, 85 Crash-EXB09 01 |
| F02-K30-52 | C5 | Synth, 1 BD-EXB09 02 |
| F02-K30-53 | C#5 | ROM, 206 Zap 2 ROM, 206 Zap 2 |
| F02-K30-54 | D5 | Synth, 103Perc-Ambi-EXB09 |
| F02-K30-55 | D#5 | ROM, 235 Conga Hi-Open |
| F02-K30-56 | E5 | ROM, 239 Conga Hi-Slap 2 |
| F02-K30-57 | F5 | ROM, 232 Conga Lo-Open |
| F02-K30-58 | F#5 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K30-59 | G5 | ROM, 234 Conga Lo-Slap |
| F02-K30-60 | G#5 | Synth, 77 HH04-Op-EXB09 |
| F02-K30-61 | A5 | Synth, 94 Perc01-Op-EXB09 |
| F02-K30-62 | A#5 | Synth, 104Perc05-Cl-EXB09 |
| F02-K30-63 | B5 | Synth, 105Perc05-Op-EXB09 |
| F02-K30-64 | C6 | Synth, 57 Clap-EXB09 01 |
| F02-K30-65 | C#6 | ROM, 293 Triangle-Mute |
| F02-K30-66 | D6 | Synth, 164Vo-09 Love |
| F02-K30-67 | D#6 | ROM, 292 Triangle-Open |
| F02-K30-68 | E6 | Synth, 171Vo-09 Club |
| F02-K30-69 | F6 | Synth, 202Vo-09 Ha-ooo-wa |
| F02-K30-70 | F#6 | Synth, 64 HH01-Cl-EXB09 |
| F02-K30-71 | G6 | Synth, 204Vo-09 Good |
| F02-K30-72 | G#6 | Synth, 65 HH01-Ho-EXB09 |
| F02-K30-73 | A6 | Synth, 217Vo-09 Come On |
| F02-K30-74 | A#6 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K30-75 | B6 | Synth, 220Vo-09 Yaeh F |
| F02-K30-76 | C7 | Synth, 222Vo-09 Oh-ya-yah |
| F02-K30-77 | C#7 | ROM, 157 Cymbal Reverse |
| F02-K30-78 | D7 | ROM, 207 Zap 3 ROM, 207 Zap 3 |
| F02-K30-79 | D#7 | ROM, 206 Zap 2 ROM, 206 Zap 2 |
| F02-K30-80 | E7 | ROM, 205 Zap 1 ROM, 205 Zap 1 |
| F02-K30-81 | F7 | Synth, 136Conga02-Ho-EX9 2 |
| F02-K30-82 | F#7 | Synth, 138Conga02-Hs-EX9 2 |
| F02-K30-83 | G7 | Synth, 134 Conga01-Hm-EXB09 |
| F02-K30-84 | G#7 | Synth, 133Conga01-H-EXB09 |
| F02-K30-85 | A7 | Synth, 132Conga01-L-EXB09 |
| F02-K30-86 | A#7 | Synth, 131Woodblock-H-EXB9 |
| F02-K30-87 | B7 | Synth, 137Conga02-Hs-EX9 1 |
| F02-K30-88 | C8 | Synth, 135Conga02-Ho-EX9 1 |
| F02-K30-89 | D8 | ROM, 202 Dance Perc-Hip |

### F02-K31 - Hard Kit 1 (kit 030, D1 p29)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K31-01 | A0 |  |
| F02-K31-02 | A#0 |  |
| F02-K31-03 | B0 |  |
| F02-K31-04 | C1 | Synth, 14 BD-EXB09 15 |
| F02-K31-05 | C#1 | Synth, 15 BD-EXB09 16 |
| F02-K31-06 | D1 | Synth, 7 BD-EXB09 08 |
| F02-K31-07 | D#1 | Synth, 6 BD-EXB09 07 |
| F02-K31-08 | E1 | Synth, 13 BD-EXB09 14 |
| F02-K31-09 | F1 | Synth, 12 BD-EXB09 13 |
| F02-K31-10 | F#1 | Synth, 16 BD-EXB09 17 |
| F02-K31-11 | G1 | Synth, 63 Clap-EXB09 07 |
| F02-K31-12 | G#1 | Synth, 38 BBD-EXB09 01 |
| F02-K31-13 | A1 | Synth, 39 BBD-EXB09 02 |
| F02-K31-14 | A#1 | Synth, 48 SD-EXB09 09 |
| F02-K31-15 | B1 | Synth, 0 BD-EXB09 01 |
| F02-K31-16 | C2 | Synth, 0 BD-EXB09 01 |
| F02-K31-17 | C#2 | Synth, 9 BD-EXB09 10 |
| F02-K31-18 | D2 | Synth, 11 BD-EXB09 12 |
| F02-K31-19 | D#2 | Synth, 29 BD-EXB09 30 |
| F02-K31-20 | E2 | Synth, 18 BD-EXB09 19 |
| F02-K31-21 | F2 | Synth, 44 SD-EXB09 05 |
| F02-K31-22 | F#2 | Synth, 40 SD-EXB09 01 |
| F02-K31-23 | G2 | Synth, 47 SD-EXB09 08 |
| F02-K31-24 | G#2 | Synth, 42 SD-EXB09 03 |
| F02-K31-25 | A2 | Synth, 43 SD-EXB09 04 |
| F02-K31-26 | A#2 | Synth, 56 SD-Jungle-EXB09 |
| F02-K31-27 | B2 | Synth, 6 BD-EXB09 07 |
| F02-K31-28 | C3 | Synth, 20 BD-EXB09 21 |
| F02-K31-29 | C#3 | Synth, 114Perc-Rim-EXB09 1 |
| F02-K31-30 | D3 | Synth, 55 SD-EXB09 16 |
| F02-K31-31 | D#3 | Synth, 59 Clap-EXB09 03 |
| F02-K31-32 | E3 | Synth, 47 SD-EXB09 08 |
| F02-K31-33 | F3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-34 | F#3 | Synth, 65 HH01-Ho-EXB09 |
| F02-K31-35 | G3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-36 | G#3 | Synth, 69 HH02-Ho-EXB09 |
| F02-K31-37 | A3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-38 | A#3 | Synth, 70 HH02-Op-EXB09 01 Synth, 70 HH02-Op-EXB09 01 |
| F02-K31-39 | B3 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-40 | C4 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K31-42 | D4 | Synth, 110Perc-Tom-EXB09 1 |
| F02-K31-43 | D#4 | Synth, 84 Ride-EXB09 02 |
| F02-K31-44 | E4 | Synth, 85 Crash-EXB09 01 |
| F02-K31-45 | F4 | Synth, 121Perc-Junk-EXB9 1 |
| F02-K31-46 | F#4 | Synth, 75 HH04-Ho-EXB09 01 |
| F02-K31-47 | G4 | Synth, 126Perc-Junk-EXB9 6 |
| F02-K31-48 | G#4 | Synth, 76 HH04-Ho-EXB09 02 |
| F02-K31-49 | A4 | Synth, 124Perc-Junk-EXB9 4 |
| F02-K31-50 | A#4 | Synth, 77 HH04-Op-EXB09 |
| F02-K31-51 | B4 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K31-52 | C5 | Synth, 135Conga02-Ho-EX9 1 |
| F02-K31-53 | C#5 | Synth, 130Woodblock-L-EXB9 |
| F02-K31-54 | D5 | Synth, 137Conga02-Hs-EX9 1 |
| F02-K31-55 | D#5 | Synth, 131Woodblock-H-EXB9 |
| F02-K31-56 | E5 | Synth, 138Conga02-Hs-EX9 2 |
| F02-K31-57 | F5 | Synth, 51 SD-EXB09 12 |
| F02-K31-58 | F#5 | Synth, 92 Cym-DDD1-H-EXB09Synth, 101 Perc04-Op-EXB09 |
| F02-K31-59 | G5 | Synth, 52 SD-EXB09 13 |
| F02-K31-60 | G#5 | Synth, 94 Perc01-Op-EXB09 Synth, 93 Perc01-Cl-EXB09 |
| F02-K31-61 | A5 | Synth, 109Perc07-Op-EXB09 |
| F02-K31-62 | A#5 | Synth, 96 Perc02-Op-EXB09 Synth, 95 Perc02-Cl-EXB09 |
| F02-K31-63 | B5 | Synth, 109Perc07-Op-EXB09 |
| F02-K31-64 | C6 | Synth, 53 SD-EXB09 14 |
| F02-K31-65 | C#6 | Synth, 117Perc-DDD-EXB09 1 |
| F02-K31-66 | D6 | Synth, 53 SD-EXB09 14 |
| F02-K31-67 | D#6 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K31-68 | E6 | Synth, 87 Crash-EXB09 03 |
| F02-K31-69 | F6 | Synth, 141Camera-EXB09 |
| F02-K31-70 | F#6 | Synth, 119Perc-DDD-EXB09 3 |
| F02-K31-71 | G6 | Synth, 142Car Horn-EXB09 |
| F02-K31-72 | G#6 | Synth, 120Perc-DDD-EXB09 4 |
| F02-K31-73 | A6 | Synth, 144Glass-EXB09 |
| F02-K31-74 | A#6 | Synth, 120Perc-DDD-EXB09 4 |
| F02-K31-75 | B6 | Synth, 87 Crash-EXB09 03 |
| F02-K31-76 | C7 | Synth, 3 BD-EXB09 04 |
| F02-K31-77 | C#7 | Synth, 81 HH06-Ho-EXB09 |
| F02-K31-78 | D7 | Synth, 45 SD-EXB09 06 |
| F02-K31-79 | D#7 | Synth, 82 HH06-Op-EXB09 |
| F02-K31-80 | E7 | Synth, 61 Clap-EXB09 05 |
| F02-K31-81 | F7 | Synth, 163Vo-09 Aaaah |
| F02-K31-82 | F#7 | Synth, 156Vo-09 Yeah M1 |
| F02-K31-83 | G7 | Synth, 161Vo-09 Ohhhh |
| F02-K31-84 | G#7 | Synth, 183Vo-09 Aow |
| F02-K31-85 | A7 | Synth, 148Vo-09 Love |
| F02-K31-86 | A#7 | Synth, 174Vo-09 Hah |
| F02-K31-87 | B7 | Synth, 153Vo-09 KeepRockin |
| F02-K31-88 | C8 | Synth, 151Vo-09 Funky 1 |
| F02-K31-89 | C#8 |  |
| F02-K31-90 | D8 |  |

### F02-K32 - Hard Kit 2 (kit 031, D1 p29)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K32-01 | A0 |  |
| F02-K32-02 | A#0 |  |
| F02-K32-03 | B0 |  |
| F02-K32-04 | C1 | ROM, 325 Yea Solo ROM, 326 Year solo |
| F02-K32-05 | C#1 | ROM, 329 Uhhhh Solo ROM, 328 Hit It |
| F02-K32-06 | D1 | ROM, 327 Uhh Solo |
| F02-K32-07 | D#1 | ROM, 335 CompuVoice-When ROM, 334 CompuVoice-Do it |
| F02-K32-08 | E1 | ROM, 331 CompuVoice-Noise ROM, 330 CompuVoice-Beep |
| F02-K32-09 | F1 | ROM, 332 CompuVoice-Chi ROM, 333 CompuVoice-Test |
| F02-K32-10 | F#1 | ROM, 337 CompuVoice-Ti ROM, 336 CompuVoice-Toi |
| F02-K32-11 | G1 | ROM, 343 PC Vox-Dunc ROM, 342 PC Vox-Sis |
| F02-K32-12 | G#1 | ROM, 348 PC Vox-Zee ROM, 345 PC Vox-For |
| F02-K32-13 | A1 | ROM, 349 PC Vox-Ae ROM, 350 PC Vox-Pa |
| F02-K32-14 | A#1 | ROM, 350 PC Vox-Pa ROM, 351 PC Vox-Sh |
| F02-K32-15 | B1 | ROM, 352 PC Vox-O ROM, 353 PC Vox-Tu |
| F02-K32-16 | C2 | Synth, 17 BD-EXB09 18 Synth, 17 BD-EXB09 18 |
| F02-K32-17 | C#2 | Synth, 13 BD-EXB09 14 Synth, 13 BD-EXB09 14 |
| F02-K32-18 | D2 | Synth, 18 BD-EXB09 19 Synth, 18 BD-EXB09 19 |
| F02-K32-19 | D#2 | Synth, 18 BD-EXB09 19 Synth, 18 BD-EXB09 19 |
| F02-K32-20 | E2 | Synth, 36 BD-EXB09 37 Synth, 36 BD-EXB09 37 |
| F02-K32-21 | F2 | Synth, 46 SD-EXB09 07 Synth, 46 SD-EXB09 07 |
| F02-K32-22 | F#2 | Synth, 41 SD-EXB09 02 Synth, 41 SD-EXB09 02 |
| F02-K32-23 | G2 | Synth, 173Vo-09 Uagh |
| F02-K32-24 | G#2 | Synth, 44 SD-EXB09 05 Synth, 44 SD-EXB09 05 |
| F02-K32-25 | A2 | Synth, 52 SD-EXB09 13 |
| F02-K32-26 | A#2 | Synth, 51 SD-EXB09 12 Synth, 51 SD-EXB09 12 |
| F02-K32-27 | B2 | Synth, 16 BD-EXB09 17 |
| F02-K32-28 | C3 | Synth, 16 BD-EXB09 17 Synth, 16 BD-EXB09 17 |
| F02-K32-29 | C#3 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K32-30 | D3 | Synth, 40 SD-EXB09 01 Synth, 40 SD-EXB09 01 |
| F02-K32-31 | D#3 | Synth, 57 Clap-EXB09 01 Synth, 57 Clap-EXB09 01 |
| F02-K32-32 | E3 | Synth, 44 SD-EXB09 05 |
| F02-K32-33 | F3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K32-34 | F#3 | Synth, 65 HH01-Ho-EXB09 Synth, 64 HH01-Cl-EXB09 |
| F02-K32-35 | G3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K32-36 | G#3 | Synth, 71 HH02-Op-EXB09 02 Synth, 71 HH02-Op-EXB09 02 |
| F02-K32-37 | A3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K32-38 | A#3 | Synth, 66 HH01-Op-EXB09 01 Synth, 66 HH01-Op-EXB09 01 |
| F02-K32-39 | B3 | Synth, 130Woodblock-L-EXB9 Synth, 130Woodblock-L-EXB9 |
| F02-K32-40 | C4 | Synth, 130Woodblock-L-EXB9 Synth, 130Woodblock-L-EXB9 |
| F02-K32-41 | C#4 | Synth, 103Perc-Ambi-EXB09 |
| F02-K32-42 | D4 | Synth, 131Woodblock-H-EXB9Synth, 131Woodblock-H-EXB9 |
| F02-K32-43 | D#4 | Synth, 84 Ride-EXB09 02 Synth, 84 Ride-EXB09 02 |
| F02-K32-44 | E4 | Synth, 99 Perc03-Op-EXB09 |
| F02-K32-45 | F4 | Synth, 71 HH02-Op-EXB09 02 Synth, 71 HH02-Op-EXB09 02 |
| F02-K32-46 | F#4 | Synth, 117Perc-DDD-EXB09 1 |
| F02-K32-47 | G4 | Synth, 85 Crash-EXB09 01 |
| F02-K32-48 | G#4 | Synth, 103Perc-Ambi-EXB09 |
| F02-K32-49 | A4 | ROM, 388 ChurchBell |
| F02-K32-50 | A#4 | Synth, 96 Perc02-Op-EXB09 |
| F02-K32-51 | B4 | Synth, 45 SD-EXB09 06 Synth, 45 SD-EXB09 06 |
| F02-K32-52 | C5 | Synth, 134Conga01-Hm-EXB09 |
| F02-K32-53 | C#5 | Synth, 138Conga02-Hs-EX9 2 Synth, 138Conga02-Hs-EX9 2 |
| F02-K32-54 | D5 | Synth, 137Conga02-Hs-EX9 1 Synth, 137Conga02-Hs-EX9 1 |
| F02-K32-55 | D#5 | Synth, 58 Clap-EXB09 02 |
| F02-K32-56 | E5 | Synth, 135Conga02-Ho-EX9 1 Synth, 135Conga02-Ho-EX9 1 |
| F02-K32-57 | F5 | Synth, 124Perc-Junk-EXB9 4 Synth, 124Perc-Junk-EXB9 4 |
| F02-K32-58 | F#5 | Synth, 37 BD-Jungle-EXB09 |
| F02-K32-59 | G5 | Synth, 88 CrashJungle-EXB9 |
| F02-K32-60 | G#5 | Synth, 56 SD-Jungle-EXB09 |
| F02-K32-61 | A5 | Synth, 97 Perc02-Rev-EXB09 |
| F02-K32-62 | A#5 | Synth, 8 BD-EXB09 09 |
| F02-K32-63 | B5 | Synth, 95 Perc02-Cl-EXB09 |
| F02-K32-64 | C6 | Synth, 165Vo-09 Kkkkk |
| F02-K32-65 | C#6 | Synth, 168Vo-09 Hrrrr |
| F02-K32-66 | D6 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K32-67 | D#6 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K32-68 | E6 | Synth, 163Vo-09 Aaaah |
| F02-K32-69 | F6 | Synth, 160Vo-09 Ahhhh |
| F02-K32-70 | F#6 | Synth, 99 Perc03-Op-EXB09 |
| F02-K32-71 | G6 | Synth, 169Vo-09 Click |
| F02-K32-72 | G#6 | Synth, 58 Clap-EXB09 02 Synth, 58 Clap-EXB09 02 |
| F02-K32-73 | A6 | Synth, 66 HH01-Op-EXB09 01 |
| F02-K32-74 | A#6 | Synth, 70 HH02-Op-EXB09 01 Synth, 70 HH02-Op-EXB09 01 |
| F02-K32-75 | B6 | Synth, 47 SD-EXB09 08 |
| F02-K32-76 | C7 | Synth, 4 BD-EXB09 05 |
| F02-K32-77 | C#7 | Synth, 95 Perc02-Cl-EXB09 Synth, 95 Perc02-Cl-EXB09 |
| F02-K32-78 | D7 | Synth, 61 Clap-EXB09 05 Synth, 61 Clap-EXB09 05 |
| F02-K32-79 | D#7 | Synth, 96 Perc02-Op-EXB09 |
| F02-K32-80 | E7 | Synth, 39 BBD-EXB09 02 Synth, 38 BBD-EXB09 01 |
| F02-K32-81 | F7 | ROM, 194 88-Clave ROM, 194 88-Clave |
| F02-K32-82 | F#7 | ROM, 385 Wind |
| F02-K32-83 | G7 | ROM, 386 Stream |
| F02-K32-84 | G#7 | ROM, 340 Two... Solo ROM, 341 One... Solo |
| F02-K32-85 | A7 | ROM, 338 Four... Solo ROM, 339 Three... Solo |
| F02-K32-86 | A#7 | ROM, 314 Bell Tree |
| F02-K32-87 | B7 | ROM, 390 Space Lore |
| F02-K32-88 | C8 | ROM, 367 Hart Beat |
| F02-K32-89 | C#8 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K32-90 | D8 |  |

### F02-K33 - Pumpin' Kit (kit 032, D1 p29)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K33-01 | A0 |  |
| F02-K33-02 | A#0 |  |
| F02-K33-03 | B0 |  |
| F02-K33-04 | C1 | ROM, 315 Flexatone |
| F02-K33-05 | C#1 | ROM, 257 Guiro-Tap ROM, 258 Maracas-Push |
| F02-K33-06 | D1 | ROM, 391 Swish Terra ROM, 391 Swish Terra |
| F02-K33-07 | D#1 | ROM, 319 Cyber Drum ROM, 319 Cyber Drum |
| F02-K33-08 | E1 | ROM, 311 Finger Cymbal ROM, 311 Finger Cymbal |
| F02-K33-09 | F1 | ROM, 291 Tambourine-Acc.2 |
| F02-K33-10 | F#1 | ROM, 288 Tambourine-Push ROM, 289 Tambourine-Pull |
| F02-K33-11 | G1 | ROM, 310 Rap Sleighbell |
| F02-K33-12 | G#1 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K33-13 | A1 | ROM, 312 Marc Tree ROM, 312 Marc Tree |
| F02-K33-14 | A#1 | Synth, 0 BD-EXB09 01 ROM, 177 99-BD 2 |
| F02-K33-15 | B1 | Synth, 32 BD-EXB09 33 Synth, 32 BD-EXB09 33 |
| F02-K33-16 | C2 | Synth, 18 BD-EXB09 19 Synth, 18 BD-EXB09 19 |
| F02-K33-17 | C#2 | Synth, 1 BD-EXB09 02 Synth, 1 BD-EXB09 02 |
| F02-K33-18 | D2 | Synth, 17 BD-EXB09 18 Synth, 17 BD-EXB09 18 |
| F02-K33-19 | D#2 | Synth, 6 BD-EXB09 07 Synth, 6 BD-EXB09 07 |
| F02-K33-20 | E2 | Synth, 39 BBD-EXB09 02 Synth, 38 BBD-EXB09 01 |
| F02-K33-21 | F2 | Synth, 45 SD-EXB09 06 Synth, 45 SD-EXB09 06 |
| F02-K33-22 | F#2 | Synth, 40 SD-EXB09 01 Synth, 40 SD-EXB09 01 |
| F02-K33-23 | G2 | Synth, 41 SD-EXB09 02 Synth, 41 SD-EXB09 02 |
| F02-K33-24 | G#2 | Synth, 61 Clap-EXB09 05 |
| F02-K33-25 | A2 | Synth, 103Perc-Ambi-EXB09 |
| F02-K33-26 | A#2 | Synth, 56 SD-Jungle-EXB09 |
| F02-K33-27 | B2 | Synth, 37 BD-Jungle-EXB09 |
| F02-K33-28 | C3 | Synth, 6 BD-EXB09 07 Synth, 6 BD-EXB09 07 |
| F02-K33-29 | C#3 | ROM, 185 88-Rimshot ROM, 185 88-Rimshot |
| F02-K33-30 | D3 | Synth, 42 SD-EXB09 03 Synth, 42 SD-EXB09 03 |
| F02-K33-31 | D#3 | Synth, 57 Clap-EXB09 01 Synth, 57 Clap-EXB09 01 |
| F02-K33-32 | E3 | Synth, 55 SD-EXB09 16 Synth, 55 SD-EXB09 16 |
| F02-K33-33 | F3 | Synth, 216Vo-09 Com'On |
| F02-K33-34 | F#3 | Synth, 72 HH03-Cl-EXB09 Synth, 72 HH03-Cl-EXB09 |
| F02-K33-35 | G3 | Synth, 239Vo-09 Go |
| F02-K33-36 | G#3 | Synth, 65 HH01-Ho-EXB09 Synth, 65 HH01-Ho-EXB09 |
| F02-K33-37 | A3 | Synth, 183Vo-09 Aow |
| F02-K33-38 | A#3 | Synth, 73 HH03-Op-EXB09 Synth, 73 HH03-Op-EXB09 |
| F02-K33-39 | B3 | Synth, 65 HH01-Ho-EXB09 |
| F02-K33-40 | C4 | Synth, 75 HH04-Ho-EXB09 01 Synth, 75 HH04-Ho-EXB09 01 |
| F02-K33-41 | C#4 | Synth, 75 HH04-Ho-EXB09 01 Synth, 76 HH04-Ho-EXB09 02 |
| F02-K33-42 | D4 | Synth, 77 HH04-Op-EXB09 Synth, 77 HH04-Op-EXB09 |
| F02-K33-43 | D#4 | Synth, 84 Ride-EXB09 02 Synth, 84 Ride-EXB09 02 |
| F02-K33-44 | E4 | Synth, 85 Crash-EXB09 01 |
| F02-K33-45 | F4 | Synth, 138Conga02-Hs-EX9 2 Synth, 138Conga02-Hs-EX9 2 |
| F02-K33-46 | F#4 | Synth, 74 HH04-Cl-EXB09 |
| F02-K33-47 | G4 | Synth, 87 Crash-EXB09 03 Synth, 87 Crash-EXB09 03 |
| F02-K33-48 | G#4 | Synth, 94 Perc01-Op-EXB09 Synth, 94 Perc01-Op-EXB09 |
| F02-K33-49 | A4 | ROM, 158 Crash-Dance 99 ROM, 158 Crash-Dance 99 |
| F02-K33-50 | A#4 | Synth, 105Perc05-Op-EXB09 Synth, 105Perc05-Op-EXB09 |
| F02-K33-51 | B4 | Synth, 135 Conga02-Ho-EX9 1 Synth, 134 Conga01-Hm-EXB09 |
| F02-K33-52 | C5 | Synth, 6 BD-EXB09 07 Synth, 6 BD-EXB09 07 |
| F02-K33-53 | C#5 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K33-54 | D5 | Synth, 163Vo-09 Aaaah |
| F02-K33-55 | D#5 | Synth, 160Vo-09 Ahhhh |
| F02-K33-56 | E5 | Synth, 210Vo-09 Uuh |
| F02-K33-57 | F5 | Synth, 223Vo-09 Oohh |
| F02-K33-58 | F#5 | Synth, 172Vo-09 Oh-Yeah M |
| F02-K33-59 | G5 | Synth, 124Perc-Junk-EXB9 4 |
| F02-K33-60 | G#5 | Synth, 8 BD-EXB09 09 |
| F02-K33-61 | A5 | Synth, 64 HH01-Cl-EXB09 Synth, 213Vo-09 Hey F2 |
| F02-K33-62 | A#5 | Synth, 106Perc06-Cl-EXB09 |
| F02-K33-63 | B5 | Synth, 164Vo-09 Love |
| F02-K33-64 | C6 | Synth, 144Glass-EXB09 |
| F02-K33-65 | C#6 | Synth, 146Vo-09 Hey M |
| F02-K33-66 | D6 | Synth, 150Vo-09 Ready |
| F02-K33-67 | D#6 | Synth, 193Vo-09 Wo'I dance |
| F02-K33-68 | E6 | Synth, 201Vo-09 Dance Synth, 200Vo-09 Ho-o |
| F02-K33-69 | F6 | Synth, 191Vo-09 Party |
| F02-K33-70 | F#6 | Synth, 194Vo-09 Take Up |
| F02-K33-71 | G6 | Synth, 211Vo-09 Uh-a-ha |
| F02-K33-72 | G#6 | Synth, 210Vo-09 Uuh |
| F02-K33-73 | A6 | Synth, 179Vo-09 Ya-ee-ha |
| F02-K33-74 | A#6 | Synth, 225Vo-09 Aaa! |
| F02-K33-75 | B6 | Synth, 116Perc-Mute-EXB09 |
| F02-K33-76 | C7 | Synth, 145Revers-EXB09 |
| F02-K33-77 | C#7 | ROM, 152 Crash Cymbal 1 ROM, 152 Crash Cymbal 1 |
| F02-K33-78 | D7 | ROM, 155 China Cymbal ROM, 155 China Cymbal |
| F02-K33-79 | D#7 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K33-80 | E7 | ROM, 238 Conga Hi-Slap 1 ROM, 242 Bongo Lo-Open |
| F02-K33-81 | F7 | ROM, 235 Conga Hi-Open ROM, 233 Conga Lo-MtSlap |
| F02-K33-82 | F#7 | ROM, 165 Ride-Cup ROM, 165 Ride-Cup |
| F02-K33-83 | G7 | ROM, 239 Conga Hi-Slap 2 ROM, 236 Conga Hi-Mute |
| F02-K33-84 | G#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K33-85 | A7 | ROM, 307 Chacha Bell ROM, 307 Chacha Bell |
| F02-K33-86 | A#7 | ROM, 305 Cowbell 1 ROM, 305 Cowbell 1 |
| F02-K33-87 | B7 | ROM, 256 Guiro-Short |
| F02-K33-88 | C8 | Synth, 93 Perc01-Cl-EXB09 |
| F02-K33-89 | C#8 | Synth, 75 HH04-Ho-EXB09 01 29 |
| F02-K33-90 | D8 | ROM, 202 Dance Perc-Hip |

### F02-K34 - PhatBoy Kit (kit 033, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K34-01 | A0 |  |
| F02-K34-02 | A#0 |  |
| F02-K34-03 | B0 |  |
| F02-K34-04 | C1 | Synth, 5 BD-EXB09 06 Synth, 0 BD-EXB09 01 |
| F02-K34-05 | C#1 | Synth, 132Conga01-L-EXB09 Synth, 132Conga01-L-EXB09 |
| F02-K34-06 | D1 | Synth, 38 BBD-EXB09 01 Synth, 38 BBD-EXB09 01 |
| F02-K34-07 | D#1 | Synth, 138Conga02-Hs-EX9 2 Synth, 138Conga02-Hs-EX9 2 |
| F02-K34-08 | E1 | Synth, 9 BD-EXB09 10 Synth, 9 BD-EXB09 10 |
| F02-K34-09 | F1 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K34-10 | F#1 | Synth, 62 Clap-EXB09 06 Synth, 62 Clap-EXB09 06 |
| F02-K34-11 | G1 | Synth, 58 Clap-EXB09 02 Synth, 58 Clap-EXB09 02 |
| F02-K34-12 | G#1 | Synth, 112 Perc-Tom-EXB09 3 Synth, 112 Perc-Tom-EXB09 3 |
| F02-K34-13 | A1 | Synth, 71 HH02-Op-EXB09 02 |
| F02-K34-14 | A#1 | Synth, 145Revers-EXB09 |
| F02-K34-15 | B1 | Synth, 31 BD-EXB09 32 |
| F02-K34-16 | C2 | Synth, 22 BD-EXB09 23 Synth, 22 BD-EXB09 23 |
| F02-K34-17 | C#2 | Synth, 35 BD-EXB09 36 Synth, 35 BD-EXB09 36 |
| F02-K34-18 | D2 | Synth, 32 BD-EXB09 33 Synth, 32 BD-EXB09 33 |
| F02-K34-19 | D#2 | Synth, 36 BD-EXB09 37 Synth, 36 BD-EXB09 37 |
| F02-K34-20 | E2 | Synth, 37 BD-Jungle-EXB09 Synth, 37 BD-Jungle-EXB09 |
| F02-K34-21 | F2 | Synth, 46 SD-EXB09 07 Synth, 46 SD-EXB09 07 |
| F02-K34-22 | F#2 | Synth, 54 SD-EXB09 15 Synth, 54 SD-EXB09 15 |
| F02-K34-23 | G2 | Synth, 54 SD-EXB09 15 Synth, 54 SD-EXB09 15 |
| F02-K34-24 | G#2 | Synth, 44 SD-EXB09 05 Synth, 43 SD-EXB09 04 |
| F02-K34-25 | A2 | Synth, 45 SD-EXB09 06 Synth, 44 SD-EXB09 05 |
| F02-K34-26 | A#2 | Synth, 45 SD-EXB09 06 Synth, 45 SD-EXB09 06 |
| F02-K34-27 | B2 | Synth, 6 BD-EXB09 07 Synth, 3 BD-EXB09 04 |
| F02-K34-28 | C3 | Synth, 9 BD-EXB09 10 Synth, 11 BD-EXB09 12 |
| F02-K34-29 | C#3 | Synth, 115Perc-Rim-EXB09 2 Synth, 115Perc-Rim-EXB09 2 |
| F02-K34-30 | D3 | Synth, 55 SD-EXB09 16 Synth, 55 SD-EXB09 16 |
| F02-K34-31 | D#3 | Synth, 60 Clap-EXB09 04 Synth, 60 Clap-EXB09 04 |
| F02-K34-32 | E3 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K34-33 | F3 | Synth, 113 Perc-Tom-EXB09 4 Synth, 113 Perc-Tom-EXB09 4 |
| F02-K34-34 | F#3 | Synth, 68 HH02-Cl-EXB09 Synth, 68 HH02-Cl-EXB09 |
| F02-K34-35 | G3 | Synth, 113 Perc-Tom-EXB09 4 Synth, 113 Perc-Tom-EXB09 4 |
| F02-K34-36 | G#3 | Synth, 69 HH02-Ho-EXB09 Synth, 69 HH02-Ho-EXB09 |
| F02-K34-37 | A3 | Synth, 113 Perc-Tom-EXB09 4 Synth, 113 Perc-Tom-EXB09 4 |
| F02-K34-38 | A#3 | Synth, 70 HH02-Op-EXB09 01 Synth, 70 HH02-Op-EXB09 01 |
| F02-K34-39 | B3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K34-40 | C4 | Synth, 113 Perc-Tom-EXB09 4 Synth, 113 Perc-Tom-EXB09 4 |
| F02-K34-41 | C#4 | Synth, 86 Crash-EXB09 02 |
| F02-K34-42 | D4 | Synth, 113 Perc-Tom-EXB09 4 Synth, 113 Perc-Tom-EXB09 4 |
| F02-K34-43 | D#4 | Synth, 83 Ride-EXB09 01 Synth, 83 Ride-EXB09 01 |
| F02-K34-44 | E4 | Synth, 87 Crash-EXB09 03 Synth, 87 Crash-EXB09 03 |
| F02-K34-45 | F4 | Synth, 105Perc05-Op-EXB09 Synth, 105Perc05-Op-EXB09 |
| F02-K34-46 | F#4 | Synth, 140 Tombourine-EXB09 Synth, 140 Tombourine-EXB09 |
| F02-K34-47 | G4 | Synth, 128Perc-Junk-EXB9 8 Synth, 128Perc-Junk-EXB9 8 |
| F02-K34-48 | G#4 | Synth, 103Perc-Ambi-EXB09 Synth, 103Perc-Ambi-EXB09 |
| F02-K34-49 | A4 | Synth, 141Camera-EXB09 |
| F02-K34-50 | A#4 | Synth, 104Perc05-Cl-EXB09 |
| F02-K34-51 | B4 | Synth, 80 HH05-Op-EXB09 Synth, 80 HH05-Op-EXB09 |
| F02-K34-52 | C5 | Synth, 136 Conga02-Ho-EX9 2 Synth, 136 Conga02-Ho-EX9 2 |
| F02-K34-53 | C#5 | Synth, 137 Conga02-Hs-EX9 1 Synth, 137 Conga02-Hs-EX9 1 |
| F02-K34-54 | D5 | Synth, 139Sidestick-EXB09 Synth, 139Sidestick-EXB09 |
| F02-K34-55 | D#5 | Synth, 165Vo-09 Kkkkk Synth, 165Vo-09 Kkkkk |
| F02-K34-56 | E5 | Synth, 132Conga01-L-EXB09 Synth, 132Conga01-L-EXB09 |
| F02-K34-57 | F5 | Synth, 140 Tombourine-EXB09 Synth, 140 Tombourine-EXB09 |
| F02-K34-58 | F#5 | Synth, 141Camera-EXB09 Synth, 141Camera-EXB09 |
| F02-K34-59 | G5 | Synth, 131 Woodblock-H-EXB9 |
| F02-K34-60 | G#5 | ROM, 206 Zap 2 |
| F02-K34-61 | A5 | Synth, 140 Tombourine-EXB09 Synth, 140 Tombourine-EXB09 |
| F02-K34-62 | A#5 | Synth, 129Perc-Junk-EXB9 9 Synth, 129Perc-Junk-EXB9 9 |
| F02-K34-63 | B5 | Synth, 98 Perc03-Cl-EXB09 |
| F02-K34-64 | C6 | Synth, 109Perc07-Op-EXB09 |
| F02-K34-65 | C#6 | Synth, 112 Perc-Tom-EXB09 3 Synth, 112 Perc-Tom-EXB09 3 |
| F02-K34-66 | D6 | Synth, 142Car Horn-EXB09 |
| F02-K34-67 | D#6 | Synth, 124Perc-Junk-EXB9 4 Synth, 124Perc-Junk-EXB9 4 |
| F02-K34-68 | E6 | Synth, 102Perc-Metal-EXB09 |
| F02-K34-69 | F6 | Synth, 96 Perc02-Op-EXB09 |
| F02-K34-70 | F#6 | Synth, 234Vo-09 Be |
| F02-K34-71 | G6 | Synth, 174Vo-09 Hah |
| F02-K34-72 | G#6 | Synth, 126Perc-Junk-EXB9 6 Synth, 126Perc-Junk-EXB9 6 |
| F02-K34-73 | A6 | Synth, 144Glass-EXB09 Synth, 144Glass-EXB09 |
| F02-K34-74 | A#6 | Synth, 128Perc-Junk-EXB9 8 Synth, 123Perc-Junk-EXB9 3 |
| F02-K34-75 | B6 | ROM, 312 Marc Tree |
| F02-K34-76 | C7 | Synth, 39 BBD-EXB09 02 |
| F02-K34-77 | C#7 | Synth, 98 Perc03-Cl-EXB09 Synth, 98 Perc03-Cl-EXB09 |
| F02-K34-78 | D7 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K34-79 | D#7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K34-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K34-81 | F7 | Synth, 102Perc-Metal-EXB09 Synth, 102Perc-Metal-EXB09 |
| F02-K34-82 | F#7 | Synth, 62 Clap-EXB09 06 |
| F02-K34-83 | G7 | Synth, 127Perc-Junk-EXB9 7 |
| F02-K34-84 | G#7 | Synth, 162Vo-09 Ooooh |
| F02-K34-85 | A7 | Synth, 168Vo-09 Hrrrr ROM, 172 SD-Orchestral |
| F02-K34-86 | A#7 | Synth, 107Perc06-Op-EXB09 |
| F02-K34-87 | B7 | ROM, 354 Stadium |
| F02-K34-88 | C8 | ROM, 355 Applause |
| F02-K34-89 | D8 |  |

### F02-K35 - Nu Trix Kit (kit 034, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K35-01 | A0 |  |
| F02-K35-02 | A#0 |  |
| F02-K35-03 | B0 |  |
| F02-K35-04 | C1 | Synth, 170Vo-09 Trrrr Synth, 170Vo-09 Trrrr |
| F02-K35-05 | C#1 | ROM, 328 Hit It ROM, 328 Hit It |
| F02-K35-06 | D1 | Synth, 164Vo-09 Love |
| F02-K35-07 | D#1 | Synth, 208Vo-09 Mmmm Synth, 208Vo-09 Mmmm |
| F02-K35-08 | E1 | Synth, 141Camera-EXB09 Synth, 141Camera-EXB09 |
| F02-K35-09 | F1 | Synth, 105Perc05-Op-EXB09 Synth, 105Perc05-Op-EXB09 |
| F02-K35-10 | F#1 | Synth, 233Vo-09 I wanna Synth, 233Vo-09 I wanna |
| F02-K35-11 | G1 | Synth, 234Vo-09 Be Synth, 234Vo-09 Be |
| F02-K35-12 | G#1 | Synth, 163Vo-09 Aaaah Synth, 163Vo-09 Aaaah |
| F02-K35-13 | A1 | Synth, 160Vo-09 Ahhhh Synth, 160Vo-09 Ahhhh |
| F02-K35-14 | A#1 | Synth, 161Vo-09 Ohhhh Synth, 161Vo-09 Ohhhh |
| F02-K35-15 | B1 | Synth, 3 BD-EXB09 04 Synth, 3 BD-EXB09 04 |
| F02-K35-16 | C2 | Synth, 0 BD-EXB09 01 Synth, 0 BD-EXB09 01 |
| F02-K35-17 | C#2 | Synth, 7 BD-EXB09 08 Synth, 7 BD-EXB09 08 |
| F02-K35-18 | D2 | Synth, 36 BD-EXB09 37 Synth, 36 BD-EXB09 37 |
| F02-K35-19 | D#2 | Synth, 16 BD-EXB09 17 Synth, 16 BD-EXB09 17 |
| F02-K35-20 | E2 | Synth, 2 BD-EXB09 03 Synth, 2 BD-EXB09 03 |
| F02-K35-21 | F2 | Synth, 40 SD-EXB09 01 Synth, 40 SD-EXB09 01 |
| F02-K35-22 | F#2 | Synth, 62 Clap-EXB09 06 |
| F02-K35-23 | G2 | Synth, 47 SD-EXB09 08 |
| F02-K35-24 | G#2 | Synth, 55 SD-EXB09 16 Synth, 55 SD-EXB09 16 |
| F02-K35-25 | A2 | Synth, 50 SD-EXB09 11 |
| F02-K35-26 | A#2 | Synth, 48 SD-EXB09 09 Synth, 48 SD-EXB09 09 |
| F02-K35-27 | B2 | Synth, 16 BD-EXB09 17 Synth, 13 BD-EXB09 14 |
| F02-K35-28 | C3 | Synth, 14 BD-EXB09 15 Synth, 14 BD-EXB09 15 |
| F02-K35-29 | C#3 | ROM, 206 Zap 2 |
| F02-K35-30 | D3 | Synth, 49 SD-EXB09 10 Synth, 49 SD-EXB09 10 |
| F02-K35-31 | D#3 | Synth, 58 Clap-EXB09 02 Synth, 60 Clap-EXB09 04 |
| F02-K35-32 | E3 | Synth, 41 SD-EXB09 02 Synth, 41 SD-EXB09 02 |
| F02-K35-33 | F3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-34 | F#3 | Synth, 78 HH05-Cl-EXB09 Synth, 78 HH05-Cl-EXB09 |
| F02-K35-35 | G3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-36 | G#3 | Synth, 79 HH05-Ho-EXB09 Synth, 79 HH05-Ho-EXB09 |
| F02-K35-37 | A3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-38 | A#3 | Synth, 80 HH05-Op-EXB09 Synth, 80 HH05-Op-EXB09 |
| F02-K35-39 | B3 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-40 | C4 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K35-42 | D4 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K35-43 | D#4 | Synth, 86 Crash-EXB09 02 Synth, 86 Crash-EXB09 02 |
| F02-K35-44 | E4 | Synth, 84 Ride-EXB09 02 |
| F02-K35-45 | F4 | ROM, 165 Ride-Cup |
| F02-K35-46 | F#4 | Synth, 140Tombourine-EXB09 |
| F02-K35-47 | G4 | Synth, 85 Crash-EXB09 01 |
| F02-K35-48 | G#4 | Synth, 105Perc05-Op-EXB09 |
| F02-K35-49 | A4 | ROM, 354 Stadium Synth, 107Perc06-Op-EXB09 |
| F02-K35-50 | A#4 | Synth, 107Perc06-Op-EXB09 |
| F02-K35-51 | B4 | Synth, 96 Perc02-Op-EXB09 Synth, 96 Perc02-Op-EXB09 |
| F02-K35-52 | C5 | Synth, 38 BBD-EXB09 01 Synth, 38 BBD-EXB09 01 |
| F02-K35-53 | C#5 | Synth, 34 BD-EXB09 35 Synth, 134 Conga01-Hm-EXB09 |
| F02-K35-54 | D5 | Synth, 125Perc-Junk-EXB9 5 Synth, 125Perc-Junk-EXB9 5 |
| F02-K35-55 | D#5 | Synth, 215Vo-09 C'mon Synth, 215Vo-09 C'mon |
| F02-K35-56 | E5 | Synth, 135Conga02-Ho-EX9 1 Synth, 135Conga02-Ho-EX9 1 |
| F02-K35-57 | F5 | Synth, 99 Perc03-Op-EXB09 Synth, 99 Perc03-Op-EXB09 |
| F02-K35-58 | F#5 | Synth, 141Camera-EXB09 |
| F02-K35-59 | G5 | ROM, 218 DJ-SD Rub |
| F02-K35-60 | G#5 | Synth, 104Perc05-Cl-EXB09 Synth, 104Perc05-Cl-EXB09 |
| F02-K35-61 | A5 | Synth, 95 Perc02-Cl-EXB09 |
| F02-K35-62 | A#5 | Synth, 99 Perc03-Op-EXB09 Synth, 99 Perc03-Op-EXB09 |
| F02-K35-63 | B5 | Synth, 108Perc07-Cl-EXB09 |
| F02-K35-64 | C6 | Synth, 92 Cym-DDD1-H-EXB09 |
| F02-K35-65 | C#6 | Synth, 141Camera-EXB09 |
| F02-K35-66 | D6 | Synth, 140Tombourine-EXB09 |
| F02-K35-67 | D#6 | Synth, 127Perc-Junk-EXB9 7 Synth, 127Perc-Junk-EXB9 7 |
| F02-K35-68 | E6 | Synth, 143Soda-EXB09 |
| F02-K35-69 | F6 | Synth, 127Perc-Junk-EXB9 7 |
| F02-K35-70 | F#6 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K35-71 | G6 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K35-72 | G#6 | Synth, 120Perc-DDD-EXB09 4 |
| F02-K35-73 | A6 | ROM, 206 Zap 2 |
| F02-K35-74 | A#6 | Synth, 132Conga01-L-EXB09 |
| F02-K35-75 | B6 | Synth, 144Glass-EXB09 |
| F02-K35-76 | C7 | Synth, 4 BD-EXB09 05 |
| F02-K35-77 | C#7 | Synth, 75 HH04-Ho-EXB09 01 Synth, 75 HH04-Ho-EXB09 01 |
| F02-K35-78 | D7 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K35-79 | D#7 | Synth, 77 HH04-Op-EXB09 |
| F02-K35-80 | E7 | Synth, 130Woodblock-L-EXB9 |
| F02-K35-81 | F7 | Synth, 103Perc-Ambi-EXB09 Synth, 103Perc-Ambi-EXB09 |
| F02-K35-82 | F#7 | Synth, 107Perc06-Op-EXB09 |
| F02-K35-83 | G7 | Synth, 143Soda-EXB09 |
| F02-K35-84 | G#7 | Synth, 169Vo-09 Click Synth, 169Vo-09 Click |
| F02-K35-85 | A7 | Synth, 170Vo-09 Trrrr Synth, 170Vo-09 Trrrr |
| F02-K35-86 | A#7 | ROM, 314 Bell Tree |
| F02-K35-87 | B7 | ROM, 390 Space Lore |
| F02-K35-88 | C8 | Synth, 39 BBD-EXB09 02 |
| F02-K35-89 | D8 |  |

### F02-K36 - Xtreme Kit (kit 035, D1 p30)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K36-01 | A0 |  |
| F02-K36-02 | A#0 |  |
| F02-K36-03 | B0 |  |
| F02-K36-04 | C1 | Synth, 113Perc-Tom-EXB09 4 Synth, 113Perc-Tom-EXB09 4 |
| F02-K36-05 | C#1 | Synth, 132Conga01-L-EXB09 Synth, 132Conga01-L-EXB09 |
| F02-K36-06 | D1 | Synth, 39 BBD-EXB09 02 Synth, 39 BBD-EXB09 02 |
| F02-K36-07 | D#1 | Synth, 138Conga02-Hs-EX9 2 Synth, 138Conga02-Hs-EX9 2 |
| F02-K36-08 | E1 | Synth, 9 BD-EXB09 10 Synth, 9 BD-EXB09 10 |
| F02-K36-09 | F1 | Synth, 134 Conga01-Hm-EXB09 Synth, 134 Conga01-Hm-EXB09 |
| F02-K36-10 | F#1 | Synth, 134 Conga01-Hm-EXB09 Synth, 134 Conga01-Hm-EXB09 |
| F02-K36-11 | G1 | Synth, 135Conga02-Ho-EX9 1 Synth, 135Conga02-Ho-EX9 1 |
| F02-K36-12 | G#1 | Synth, 116Perc-Mute-EXB09 Synth, 116Perc-Mute-EXB09 |
| F02-K36-13 | A1 | Synth, 120Perc-DDD-EXB09 4 |
| F02-K36-14 | A#1 | Synth, 145Revers-EXB09 |
| F02-K36-15 | B1 | Synth, 6 BD-EXB09 07 |
| F02-K36-16 | C2 | Synth, 15 BD-EXB09 16 Synth, 15 BD-EXB09 16 |
| F02-K36-17 | C#2 | Synth, 14 BD-EXB09 15 Synth, 14 BD-EXB09 15 |
| F02-K36-18 | D2 | Synth, 7 BD-EXB09 08 Synth, 7 BD-EXB09 08 |
| F02-K36-19 | D#2 | Synth, 24 BD-EXB09 25 Synth, 24 BD-EXB09 25 |
| F02-K36-20 | E2 | Synth, 30 BD-EXB09 31 Synth, 30 BD-EXB09 31 |
| F02-K36-21 | F2 | Synth, 40 SD-EXB09 01 Synth, 40 SD-EXB09 01 |
| F02-K36-22 | F#2 | Synth, 41 SD-EXB09 02 Synth, 41 SD-EXB09 02 |
| F02-K36-23 | G2 | Synth, 42 SD-EXB09 03 Synth, 42 SD-EXB09 03 |
| F02-K36-24 | G#2 | Synth, 43 SD-EXB09 04 Synth, 43 SD-EXB09 04 |
| F02-K36-25 | A2 | Synth, 44 SD-EXB09 05 Synth, 44 SD-EXB09 05 |
| F02-K36-26 | A#2 | Synth, 45 SD-EXB09 06 Synth, 45 SD-EXB09 06 |
| F02-K36-27 | B2 | Synth, 0 BD-EXB09 01 Synth, 0 BD-EXB09 01 |
| F02-K36-28 | C3 | Synth, 1 BD-EXB09 02 Synth, 1 BD-EXB09 02 |
| F02-K36-29 | C#3 | Synth, 139Sidestick-EXB09 Synth, 139Sidestick-EXB09 |
| F02-K36-30 | D3 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K36-31 | D#3 | Synth, 57 Clap-EXB09 01 Synth, 57 Clap-EXB09 01 |
| F02-K36-32 | E3 | Synth, 55 SD-EXB09 16 Synth, 55 SD-EXB09 16 |
| F02-K36-33 | F3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-34 | F#3 | Synth, 75 HH04-Ho-EXB09 01 Synth, 75 HH04-Ho-EXB09 01 |
| F02-K36-35 | G3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-36 | G#3 | Synth, 76 HH04-Ho-EXB09 02 Synth, 76 HH04-Ho-EXB09 02 |
| F02-K36-37 | A3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-38 | A#3 | Synth, 77 HH04-Op-EXB09 Synth, 77 HH04-Op-EXB09 |
| F02-K36-39 | B3 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-40 | C4 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-41 | C#4 | Synth, 85 Crash-EXB09 01 |
| F02-K36-42 | D4 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K36-43 | D#4 | Synth, 86 Crash-EXB09 02 Synth, 86 Crash-EXB09 02 |
| F02-K36-44 | E4 | Synth, 87 Crash-EXB09 03 Synth, 87 Crash-EXB09 03 |
| F02-K36-45 | F4 | Synth, 99 Perc03-Op-EXB09 Synth, 99 Perc03-Op-EXB09 |
| F02-K36-46 | F#4 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K36-47 | G4 | Synth, 124Perc-Junk-EXB9 4 Synth, 124Perc-Junk-EXB9 4 |
| F02-K36-48 | G#4 | Synth, 208Vo-09 Mmmm Synth, 168Vo-09 Hrrrr |
| F02-K36-49 | A4 | Synth, 144Glass-EXB09 |
| F02-K36-50 | A#4 | Synth, 106Perc06-Cl-EXB09 |
| F02-K36-51 | B4 | Synth, 80 HH05-Op-EXB09 Synth, 80 HH05-Op-EXB09 |
| F02-K36-52 | C5 | Synth, 5 BD-EXB09 06 Synth, 5 BD-EXB09 06 |
| F02-K36-53 | C#5 | Synth, 134 Conga01-Hm-EXB09 Synth, 134 Conga01-Hm-EXB09 |
| F02-K36-54 | D5 | Synth, 136Conga02-Ho-EX9 2 Synth, 136Conga02-Ho-EX9 2 |
| F02-K36-55 | D#5 | Synth, 160Vo-09 Ahhhh Synth, 160Vo-09 Ahhhh |
| F02-K36-56 | E5 | Synth, 132Conga01-L-EXB09 Synth, 132Conga01-L-EXB09 |
| F02-K36-57 | F5 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K36-58 | F#5 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K36-59 | G5 | Synth, 131Woodblock-H-EXB9 |
| F02-K36-60 | G#5 | ROM, 206 Zap 2 |
| F02-K36-61 | A5 | Synth, 143Soda-EXB09 Synth, 143Soda-EXB09 |
| F02-K36-62 | A#5 | Synth, 129Perc-Junk-EXB9 9 Synth, 129Perc-Junk-EXB9 9 |
| F02-K36-63 | B5 | Synth, 106Perc06-Cl-EXB09 |
| F02-K36-64 | C6 | Synth, 107Perc06-Op-EXB09 |
| F02-K36-65 | C#6 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K36-66 | D6 | Synth, 108Perc07-Cl-EXB09 |
| F02-K36-67 | D#6 | Synth, 106Perc06-Cl-EXB09 |
| F02-K36-68 | E6 | Synth, 103Perc-Ambi-EXB09 |
| F02-K36-69 | F6 | Synth, 103Perc-Ambi-EXB09 |
| F02-K36-70 | F#6 | Synth, 224Vo-09 Owww |
| F02-K36-71 | G6 | Synth, 210Vo-09 Uuh Synth, 170Vo-09 Trrrr |
| F02-K36-72 | G#6 | Synth, 120Perc-DDD-EXB09 4Synth, 120Perc-DDD-EXB09 4 |
| F02-K36-73 | A6 | Synth, 140Tombourine-EXB09 |
| F02-K36-74 | A#6 | Synth, 118Perc-DDD-EXB09 2Synth, 118Perc-DDD-EXB09 2 |
| F02-K36-75 | B6 | ROM, 312 Marc Tree |
| F02-K36-76 | C7 | Synth, 2 BD-EXB09 03 |
| F02-K36-77 | C#7 | Synth, 98 Perc03-Cl-EXB09 Synth, 98 Perc03-Cl-EXB09 |
| F02-K36-78 | D7 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K36-79 | D#7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K36-80 | E7 | ROM, 191 88-Crash ROM, 191 88-Crash |
| F02-K36-81 | F7 | Synth, 102Perc-Metal-EXB09 Synth, 102Perc-Metal-EXB09 |
| F02-K36-82 | F#7 | Synth, 62 Clap-EXB09 06 |
| F02-K36-83 | G7 | Synth, 127Perc-Junk-EXB9 7 |
| F02-K36-84 | G#7 | Synth, 162Vo-09 Ooooh |
| F02-K36-85 | A7 | Synth, 168Vo-09 Hrrrr ROM, 172 SD-Orchestral |
| F02-K36-86 | A#7 | Synth, 107Perc06-Op-EXB09 |
| F02-K36-87 | B7 | ROM, 354 Stadium |
| F02-K36-88 | C8 | ROM, 355 Applause |
| F02-K36-89 | D8 |  |

### F02-K37 - Grand Slam Kit (kit 036, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K37-01 | A0 |  |
| F02-K37-02 | A#0 |  |
| F02-K37-03 | B0 |  |
| F02-K37-04 | C1 | Synth, 13 BD-EXB09 14 Synth, 13 BD-EXB09 14 |
| F02-K37-05 | C#1 | Synth, 61 Clap-EXB09 05 Synth, 61 Clap-EXB09 05 |
| F02-K37-06 | D1 | Synth, 38 BBD-EXB09 01 Synth, 38 BBD-EXB09 01 |
| F02-K37-07 | D#1 | Synth, 130Woodblock-L-EXB9 Synth, 130Woodblock-L-EXB9 |
| F02-K37-08 | E1 | Synth, 11 BD-EXB09 12 Synth, 11 BD-EXB09 12 |
| F02-K37-09 | F1 | Synth, 120Perc-DDD-EXB09 4Synth, 120Perc-DDD-EXB09 4 |
| F02-K37-10 | F#1 | Synth, 139Sidestick-EXB09 Synth, 139Sidestick-EXB09 |
| F02-K37-11 | G1 | Synth, 114Perc-Rim-EXB09 1 Synth, 114Perc-Rim-EXB09 1 |
| F02-K37-12 | G#1 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K37-13 | A1 | Synth, 117Perc-DDD-EXB09 1 |
| F02-K37-14 | A#1 | ROM, 215 DJ-1c Vocal Rub |
| F02-K37-15 | B1 | Synth, 28 BD-EXB09 29 |
| F02-K37-16 | C2 | Synth, 15 BD-EXB09 16 Synth, 15 BD-EXB09 16 |
| F02-K37-17 | C#2 | Synth, 13 BD-EXB09 14 Synth, 13 BD-EXB09 14 |
| F02-K37-18 | D2 | Synth, 7 BD-EXB09 08 Synth, 7 BD-EXB09 08 |
| F02-K37-19 | D#2 | Synth, 25 BD-EXB09 26 Synth, 25 BD-EXB09 26 |
| F02-K37-20 | E2 | Synth, 31 BD-EXB09 32 Synth, 31 BD-EXB09 32 |
| F02-K37-21 | F2 | Synth, 45 SD-EXB09 06 Synth, 45 SD-EXB09 06 |
| F02-K37-22 | F#2 | Synth, 52 SD-EXB09 13 Synth, 52 SD-EXB09 13 |
| F02-K37-23 | G2 | Synth, 47 SD-EXB09 08 Synth, 47 SD-EXB09 08 |
| F02-K37-24 | G#2 | Synth, 49 SD-EXB09 10 Synth, 49 SD-EXB09 10 |
| F02-K37-25 | A2 | Synth, 50 SD-EXB09 11 Synth, 50 SD-EXB09 11 |
| F02-K37-26 | A#2 | Synth, 51 SD-EXB09 12 Synth, 51 SD-EXB09 12 |
| F02-K37-27 | B2 | Synth, 3 BD-EXB09 04 Synth, 3 BD-EXB09 04 |
| F02-K37-28 | C3 | Synth, 36 BD-EXB09 37 Synth, 36 BD-EXB09 37 |
| F02-K37-29 | C#3 | Synth, 134 Conga01-Hm-EXB09 Synth, 134 Conga01-Hm-EXB09 |
| F02-K37-30 | D3 | Synth, 52 SD-EXB09 13 Synth, 52 SD-EXB09 13 |
| F02-K37-31 | D#3 | Synth, 62 Clap-EXB09 06 Synth, 62 Clap-EXB09 06 |
| F02-K37-32 | E3 | Synth, 53 SD-EXB09 14 Synth, 53 SD-EXB09 14 |
| F02-K37-33 | F3 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K37-34 | F#3 | Synth, 78 HH05-Cl-EXB09 Synth, 78 HH05-Cl-EXB09 |
| F02-K37-35 | G3 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K37-36 | G#3 | Synth, 79 HH05-Ho-EXB09 Synth, 79 HH05-Ho-EXB09 |
| F02-K37-37 | A3 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K37-38 | A#3 | Synth, 80 HH05-Op-EXB09 Synth, 80 HH05-Op-EXB09 |
| F02-K37-39 | B3 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K37-40 | C4 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K37-41 | C#4 | Synth, 86 Crash-EXB09 02 |
| F02-K37-42 | D4 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K37-43 | D#4 | Synth, 87 Crash-EXB09 03 Synth, 87 Crash-EXB09 03 |
| F02-K37-44 | E4 | Synth, 88 CrashJungle-EXB9 Synth, 88 CrashJungle-EXB9 |
| F02-K37-45 | F4 | Synth, 102Perc-Metal-EXB09 Synth, 102Perc-Metal-EXB09 |
| F02-K37-46 | F#4 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K37-47 | G4 | Synth, 126Perc-Junk-EXB9 6 Synth, 126Perc-Junk-EXB9 6 |
| F02-K37-48 | G#4 | Synth, 131Woodblock-H-EXB9 |
| F02-K37-49 | A4 | Synth, 141Camera-EXB09 |
| F02-K37-50 | A#4 | Synth, 107Perc06-Op-EXB09 Synth, 107Perc06-Op-EXB09 |
| F02-K37-51 | B4 | Synth, 82 HH06-Op-EXB09 Synth, 82 HH06-Op-EXB09 |
| F02-K37-52 | C5 | Synth, 23 BD-EXB09 24 Synth, 23 BD-EXB09 24 |
| F02-K37-53 | C#5 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K37-54 | D5 | Synth, 138Conga02-Hs-EX9 2 Synth, 138Conga02-Hs-EX9 2 |
| F02-K37-55 | D#5 | Synth, 163Vo-09 Aaaah Synth, 163Vo-09 Aaaah |
| F02-K37-56 | E5 | Synth, 134 Conga01-Hm-EXB09 Synth, 134 Conga01-Hm-EXB09 |
| F02-K37-57 | F5 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K37-58 | F#5 | Synth, 127Perc-Junk-EXB9 7 Synth, 127Perc-Junk-EXB9 7 |
| F02-K37-59 | G5 | Synth, 130Woodblock-L-EXB9 |
| F02-K37-60 | G#5 | ROM, 205 Zap 1 |
| F02-K37-61 | A5 | Synth, 108Perc07-Cl-EXB09 Synth, 106Perc06-Cl-EXB09 |
| F02-K37-62 | A#5 | Synth, 128Perc-Junk-EXB9 8 Synth, 128Perc-Junk-EXB9 8 |
| F02-K37-63 | B5 | Synth, 163Vo-09 Aaaah |
| F02-K37-64 | C6 | Synth, 106Perc06-Cl-EXB09 |
| F02-K37-65 | C#6 | Synth, 110Perc-Tom-EXB09 1 Synth, 110Perc-Tom-EXB09 1 |
| F02-K37-66 | D6 | Synth, 140Tombourine-EXB09 |
| F02-K37-67 | D#6 | Synth, 108Perc07-Cl-EXB09 |
| F02-K37-68 | E6 | Synth, 101Perc04-Op-EXB09 |
| F02-K37-69 | F6 | Synth, 99 Perc03-Op-EXB09 |
| F02-K37-70 | F#6 | Synth, 210Vo-09 Uuh |
| F02-K37-71 | G6 | Synth, 166Vo-09 Kaaaa |
| F02-K37-72 | G#6 | Synth, 121Perc-Junk-EXB9 1 Synth, 120Perc-DDD-EXB09 4 |
| F02-K37-73 | A6 | Synth, 141Camera-EXB09 |
| F02-K37-74 | A#6 | Synth, 119Perc-DDD-EXB09 3Synth, 119Perc-DDD-EXB09 3 |
| F02-K37-75 | B6 | ROM, 313 Marc Tree LP |
| F02-K37-76 | C7 | Synth, 12 BD-EXB09 13 Synth, 12 BD-EXB09 13 |
| F02-K37-77 | C#7 | Synth, 99 Perc03-Op-EXB09 Synth, 99 Perc03-Op-EXB09 |
| F02-K37-78 | D7 | Synth, 56 SD-Jungle-EXB09 Synth, 56 SD-Jungle-EXB09 |
| F02-K37-79 | D#7 | Synth, 129Perc-Junk-EXB9 9 |
| F02-K37-80 | E7 | ROM, 190 99-HHat Open ROM, 190 99-HHat Open |
| F02-K37-81 | F7 | Synth, 101Perc04-Op-EXB09 Synth, 101Perc04-Op-EXB09 |
| F02-K37-82 | F#7 | Synth, 63 Clap-EXB09 07 |
| F02-K37-83 | G7 | Synth, 128Perc-Junk-EXB9 8 |
| F02-K37-84 | G#7 | Synth, 164Vo-09 Love |
| F02-K37-85 | A7 | Synth, 170Vo-09 Trrrr ROM, 172 SD-Orchestral |
| F02-K37-86 | A#7 | Synth, 109Perc07-Op-EXB09 |
| F02-K37-87 | B7 | ROM, 355 Applause |
| F02-K37-88 | C8 | ROM, 354 Stadium |
| F02-K37-89 | C#8 |  |
| F02-K37-90 | D8 |  |

### F02-K38 - Techno Perc (kit 037, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K38-01 | A0 |  |
| F02-K38-02 | A#0 |  |
| F02-K38-03 | B0 |  |
| F02-K38-04 | C1 | Synth, 39 BBD-EXB09 02 Synth, 39 BBD-EXB09 02 |
| F02-K38-05 | C#1 | Synth, 143Soda-EXB09 Synth, 143Soda-EXB09 |
| F02-K38-06 | D1 | Synth, 162Vo-09 Ooooh Synth, 162Vo-09 Ooooh |
| F02-K38-07 | D#1 | Synth, 161Vo-09 Ohhhh Synth, 161Vo-09 Ohhhh |
| F02-K38-08 | E1 | Synth, 160Vo-09 Ahhhh Synth, 160Vo-09 Ahhhh |
| F02-K38-09 | F1 | Synth, 173Vo-09 Uagh Synth, 173Vo-09 Uagh |
| F02-K38-10 | F#1 | Synth, 170Vo-09 Trrrr Synth, 170Vo-09 Trrrr |
| F02-K38-11 | G1 | Synth, 169Vo-09 Click Synth, 169Vo-09 Click |
| F02-K38-12 | G#1 | Synth, 143Soda-EXB09 Synth, 143Soda-EXB09 |
| F02-K38-13 | A1 | ROM, 206 Zap 2 ROM, 206 Zap 2 |
| F02-K38-14 | A#1 | Synth, 112Perc-Tom-EXB09 3 Synth, 116Perc-Mute-EXB09 |
| F02-K38-15 | B1 | Synth, 111Perc-Tom-EXB09 2 Synth, 112Perc-Tom-EXB09 3 |
| F02-K38-16 | C2 | ROM, 205 Zap 1 |
| F02-K38-17 | C#2 | Synth, 162Vo-09 Ooooh |
| F02-K38-18 | D2 | Synth, 38 BBD-EXB09 01 |
| F02-K38-19 | D#2 | Synth, 59 Clap-EXB09 03 |
| F02-K38-20 | E2 | Synth, 104Perc05-Cl-EXB09 |
| F02-K38-21 | F2 | Synth, 105Perc05-Op-EXB09 |
| F02-K38-22 | F#2 | Synth, 25 BD-EXB09 26 |
| F02-K38-23 | G2 | Synth, 102Perc-Metal-EXB09 |
| F02-K38-24 | G#2 | Synth, 105Perc05-Op-EXB09 |
| F02-K38-25 | A2 | Synth, 141Camera-EXB09 |
| F02-K38-26 | A#2 | Synth, 107Perc06-Op-EXB09 Synth, 107Perc06-Op-EXB09 |
| F02-K38-27 | B2 | Synth, 143Soda-EXB09 |
| F02-K38-28 | C3 | Synth, 36 BD-EXB09 37 |
| F02-K38-29 | C#3 | Synth, 104Perc05-Cl-EXB09 |
| F02-K38-30 | D3 | Synth, 103Perc-Ambi-EXB09 |
| F02-K38-31 | D#3 | Synth, 107Perc06-Op-EXB09 |
| F02-K38-32 | E3 | Synth, 58 Clap-EXB09 02 |
| F02-K38-33 | F3 | Synth, 111Perc-Tom-EXB09 2 |
| F02-K38-34 | F#3 | ROM, 207 Zap 3 |
| F02-K38-35 | G3 | Synth, 112Perc-Tom-EXB09 3 |
| F02-K38-36 | G#3 | Synth, 54 SD-EXB09 15 |
| F02-K38-37 | A3 | Synth, 116Perc-Mute-EXB09 |
| F02-K38-38 | A#3 | Synth, 142Car Horn-EXB09 |
| F02-K38-39 | B3 | Synth, 56 SD-Jungle-EXB09 |
| F02-K38-40 | C4 | Synth, 63 Clap-EXB09 07 |
| F02-K38-41 | C#4 | Synth, 103Perc-Ambi-EXB09 |
| F02-K38-42 | D4 | Synth, 161Vo-09 Ohhhh |
| F02-K38-43 | D#4 | Synth, 20 BD-EXB09 21 |
| F02-K38-44 | E4 | Synth, 108Perc07-Cl-EXB09 |
| F02-K38-45 | F4 | Synth, 136Conga02-Ho-EX9 2 |
| F02-K38-46 | F#4 | Synth, 137Conga02-Hs-EX9 1 |
| F02-K38-47 | G4 | Synth, 131Woodblock-H-EXB9 |
| F02-K38-48 | G#4 | Synth, 130Woodblock-L-EXB9 |
| F02-K38-49 | A4 | Synth, 103Perc-Ambi-EXB09 |
| F02-K38-50 | A#4 | Synth, 113Perc-Tom-EXB09 4 |
| F02-K38-51 | B4 | Synth, 111Perc-Tom-EXB09 2 Synth, 111Perc-Tom-EXB09 2 |
| F02-K38-52 | C5 | Synth, 141Camera-EXB09 Synth, 141Camera-EXB09 |
| F02-K38-53 | C#5 | Synth, 137Conga02-Hs-EX9 1 |
| F02-K38-54 | D5 | ROM, 205 Zap 1 |
| F02-K38-55 | D#5 | ROM, 207 Zap 3 |
| F02-K38-56 | E5 | Synth, 116Perc-Mute-EXB09 Synth, 116Perc-Mute-EXB09 |
| F02-K38-57 | F5 | Synth, 134 Conga01-Hm-EXB09 |
| F02-K38-58 | F#5 | Synth, 138Conga02-Hs-EX9 2 |
| F02-K38-59 | G5 | Synth, 116Perc-Mute-EXB09 |
| F02-K38-60 | G#5 | Synth, 115Perc-Rim-EXB09 2 |
| F02-K38-61 | A5 | Synth, 132Conga01-L-EXB09 |
| F02-K38-62 | A#5 | Synth, 100Perc04-Cl-EXB09 |
| F02-K38-63 | B5 | Synth, 100Perc04-Cl-EXB09 |
| F02-K38-64 | C6 | ROM, 259 Maracas-Pull ROM, 258 Maracas-Push |
| F02-K38-65 | C#6 | ROM, 297 Shaker 1 ROM, 297 Shaker 1 |
| F02-K38-66 | D6 | Synth, 97 Perc02-Rev-EXB09 Synth, 97 Perc02-Rev-EXB09 |
| F02-K38-67 | D#6 | Synth, 125Perc-Junk-EXB9 5 Synth, 125Perc-Junk-EXB9 5 |
| F02-K38-68 | E6 | Synth, 129Perc-Junk-EXB9 9 Synth, 129Perc-Junk-EXB9 9 |
| F02-K38-69 | F6 | Synth, 106Perc06-Cl-EXB09 Synth, 106Perc06-Cl-EXB09 |
| F02-K38-70 | F#6 | Synth, 140Tombourine-EXB09 |
| F02-K38-71 | G6 | Synth, 140Tombourine-EXB09 Synth, 140Tombourine-EXB09 |
| F02-K38-72 | G#6 | Synth, 118Perc-DDD-EXB09 2 |
| F02-K38-73 | A6 | Synth, 99 Perc03-Op-EXB09 Synth, 99 Perc03-Op-EXB09 |
| F02-K38-74 | A#6 | Synth, 130Woodblock-L-EXB9 Synth, 130Woodblock-L-EXB9 |
| F02-K38-75 | B6 | Synth, 107Perc06-Op-EXB09 |
| F02-K38-76 | C7 | Synth, 97 Perc02-Rev-EXB09 |
| F02-K38-77 | C#7 | Synth, 96 Perc02-Op-EXB09 |
| F02-K38-78 | D7 | Synth, 107Perc06-Op-EXB09 |
| F02-K38-79 | D#7 | Synth, 61 Clap-EXB09 05 |
| F02-K38-80 | E7 | Synth, 107Perc06-Op-EXB09 |
| F02-K38-81 | F7 | Synth, 127Perc-Junk-EXB9 7 Synth, 127Perc-Junk-EXB9 7 |
| F02-K38-82 | F#7 | Synth, 131Woodblock-H-EXB9 |
| F02-K38-83 | G7 | Synth, 39 BBD-EXB09 02 |
| F02-K38-84 | G#7 | Synth, 140Tombourine-EXB09 |
| F02-K38-85 | A7 | Synth, 143Soda-EXB09 |
| F02-K38-86 | A#7 | ROM, 361 Bird 1 |
| F02-K38-87 | B7 | Synth, 130Woodblock-L-EXB9 |
| F02-K38-88 | C8 | ROM, 310 Rap Sleighbell |
| F02-K38-89 | C#8 |  |
| F02-K38-90 | D8 |  |

### F02-K39 - EXB Orch. Kit 1 (kit 038, D1 p31)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K39-01 | A0 |  |
| F02-K39-02 | A#0 |  |
| F02-K39-03 | B0 |  |
| F02-K39-04 | C1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-05 | C#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-06 | D1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-07 | D#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-08 | E1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-09 | F1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-10 | F#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-11 | G1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-12 | G#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-13 | A1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-14 | A#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-15 | B1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K39-16 | C2 | OrchB, 7 BD-Orch. ff OrchB, 7 BD-Orch. ff |
| F02-K39-17 | C#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K39-18 | D2 | OrchB, 9 SD-Orch. ff 1 OrchB, 9 SD-Orch. ff 1 |
| F02-K39-19 | D#2 | OrchB, 10 SD-Orch. ff 2 OrchB, 10 SD-Orch. ff 2 |
| F02-K39-20 | E2 | OrchB, 14 SD-Orch. pp 2 OrchB, 14 SD-Orch. pp 2 |
| F02-K39-21 | F2 | OrchB, 16 Piatti ff OrchB, 16 Piatti ff |
| F02-K39-22 | F#2 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K39-23 | G2 | OrchB, 8 BD-Orch. pp OrchB, 8 BD-Orch. pp |
| F02-K39-24 | G#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K39-25 | A2 | OrchB, 13 SD-Orch. pp 1 OrchB, 13 SD-Orch. pp 1 |
| F02-K39-26 | A#2 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K39-27 | B2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K39-28 | C3 | OrchB, 21 Triangle-Mute OrchB, 21 Triangle-Mute |
| F02-K39-29 | C#3 | OrchB, 22 Triangle-Roll OrchB, 22 Triangle-Roll |
| F02-K39-30 | D3 | OrchB, 20 Triangle OrchB, 20 Triangle |
| F02-K39-31 | D#3 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K39-32 | E3 | ROM, 291 Tambourine-Acc.2 ROM, 291 Tambourine-Acc.2 |
| F02-K39-33 | F3 | ROM, 280 Woodblock 2 ROM, 280 Woodblock 2 |
| F02-K39-34 | F#3 | ROM, 253 Castanet-Dble 1 ROM, 253 Castanet-Dble 1 |
| F02-K39-35 | G3 | ROM, 280 Woodblock 2 ROM, 280 Woodblock 2 |
| F02-K39-36 | G#3 | ROM, 254 Castanet-Dble 2 ROM, 254 Castanet-Dble 2 |
| F02-K39-37 | A3 | ROM, 280 Woodblock 2 ROM, 280 Woodblock 2 |
| F02-K39-38 | A#3 | ROM, 252 Castanet-Single ROM, 252 Castanet-Single |
| F02-K39-39 | B3 | ROM, 280 Woodblock 2 ROM, 280 Woodblock 2 |
| F02-K39-40 | C4 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K39-41 | C#4 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K39-42 | D4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K39-43 | D#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K39-44 | E4 | OrchB, 3 Timpani mf Lo OrchB, 5 Timpani pp Lo |
| F02-K39-45 | F4 | OrchB, 2 Timpani mf Hi OrchB, 4 Timpani pp Hi |
| F02-K39-46 | F#4 | OrchB, 2 Timpani mf Hi OrchB, 4 Timpani pp Hi |
| F02-K39-47 | G4 | OrchB, 6 Timpani-Roll OrchB, 6 Timpani-Roll |
| F02-K39-48 | G#4 | OrchB, 6 Timpani-Roll OrchB, 6 Timpani-Roll |
| F02-K39-49 | A4 | OrchB, 6 Timpani-Roll OrchB, 6 Timpani-Roll |
| F02-K39-50 | A#4 | ROM, 168 Orch Cym.-Open ROM, 168 Orch Cym.-Open |
| F02-K39-51 | B4 | ROM, 167 Orch Cym.-Closed ROM, 167 Orch Cym.-Closed |
| F02-K39-52 | C5 | OrchB, 31 Harp-Chord-min 1 OrchB, 31 Harp-Chord-min 1 |
| F02-K39-53 | C#5 | OrchB, 32 Harp-Chord-min 2 OrchB, 32 Harp-Chord-min 2 |
| F02-K39-54 | D5 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K39-55 | D#5 | OrchB, 33 Harp-Chord-maj 1 OrchB, 33 Harp-Chord-maj 1 |
| F02-K39-56 | E5 | OrchB, 34 Harp-Chord-maj 2 OrchB, 34 Harp-Chord-maj 2 |
| F02-K39-57 | F5 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K39-58 | F#5 | OrchS, 15 Vi Ens.Hit-min 2 OrchS, 15 Vi Ens.Hit-min 2 |
| F02-K39-59 | G5 | OrchS, 14 Vi Ens.Hit-min 1 OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K39-60 | G#5 | OrchS, 12 Vi Ens.Hit-dim 1 OrchS, 12 Vi Ens.Hit-dim 1 |
| F02-K39-61 | A5 | OrchS, 17 Vi Ens.Hit-maj 2 OrchS, 17 Vi Ens.Hit-maj 2 |
| F02-K39-62 | A#5 | OrchS, 16 Vi Ens.Hit-maj 1 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K39-63 | B5 | OrchS, 13 Vi Ens.Hit-dim 2 OrchS, 13 Vi Ens.Hit-dim 2 |
| F02-K39-64 | C6 | OrchS, 18 ViEns.RunUp-mi 1 OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K39-65 | C#6 | OrchS, 19 ViEns.RunUp-mi 2 OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K39-66 | D6 | OrchS, 20 ViEns.RunUp-mi 3 OrchS, 20 ViEns.RunUp-mi 3 |
| F02-K39-67 | D#6 | OrchS, 22 ViEns.RunUp-mj 1 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K39-68 | E6 | OrchS, 23 ViEns.RunUp-mj 2 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K39-69 | F6 | OrchS, 24 ViEns.RunUp-mj 3 OrchS, 24 ViEns.RunUp-mj 3 |
| F02-K39-70 | F#6 | OrchB, 23 Harp-Glissando 1 |
| F02-K39-71 | G6 | OrchB, 23 Harp-Glissando 1 |
| F02-K39-72 | G#6 | OrchB, 25 Harp-Glissando 3 |
| F02-K39-73 | A6 | OrchB, 25 Harp-Glissando 3 |
| F02-K39-74 | A#6 | OrchB, 26 Harp-Glissando 4 |
| F02-K39-75 | B6 | OrchB, 24 Harp-Glissando 2 |
| F02-K39-76 | C7 | ROM, 403 HarpGliss |
| F02-K39-77 | C#7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-78 | D7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-79 | D#7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-80 | E7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-81 | F7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-82 | F#7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-83 | G7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-84 | G#7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-85 | A7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-86 | A#7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-87 | B7 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-88 | C8 | ROM, 222 Orchestra Hit ROM, 222 Orchestra Hit |
| F02-K39-89 | C#8 | 31 |
| F02-K39-90 | D8 |  |

### F02-K40 - EXB Orch. Kit 2 (kit 039, D1 p32)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K40-01 | A0 |  |
| F02-K40-02 | A#0 |  |
| F02-K40-03 | B0 |  |
| F02-K40-04 | C1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-05 | C#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-06 | D1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-07 | D#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-08 | E1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-09 | F1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-10 | F#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-11 | G1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-12 | G#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-13 | A1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-14 | A#1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-15 | B1 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-16 | C2 | OrchB, 7 BD-Orch. ff OrchB, 8 BD-Orch. pp |
| F02-K40-17 | C#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K40-18 | D2 | OrchB, 14 SD-Orch. pp 2 OrchB, 13 SD-Orch. pp 1 |
| F02-K40-19 | D#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K40-20 | E2 | OrchB, 12 SD-Orch. mf 2 OrchB, 11 SD-Orch. mf 1 |
| F02-K40-21 | F2 | OrchB, 16 Piatti ff OrchB, 16 Piatti ff |
| F02-K40-22 | F#2 | OrchB, 17 Piatti pp OrchB, 18 Piatti Short |
| F02-K40-23 | G2 | ROM, 281 Timbale Lo-Open ROM, 281 Timbale Lo-Open |
| F02-K40-24 | G#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K40-25 | A2 | OrchB, 12 SD-Orch. mf 2 OrchB, 14 SD-Orch. pp 2 |
| F02-K40-26 | A#2 | OrchB, 15 SD-Orch.-Roll OrchB, 15 SD-Orch.-Roll |
| F02-K40-27 | B2 | OrchB, 10 SD-Orch. ff 2 OrchB, 9 SD-Orch. ff 1 |
| F02-K40-28 | C3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-29 | C#3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-30 | D3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-31 | D#3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-32 | E3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-33 | F3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-34 | F#3 | OrchB, 1 Timpani ff Lo OrchB, 3 Timpani mf Lo |
| F02-K40-35 | G3 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-36 | G#3 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-37 | A3 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-38 | A#3 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-39 | B3 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-40 | C4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-41 | C#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-42 | D4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-43 | D#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-44 | E4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-45 | F4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-46 | F#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-47 | G4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-48 | G#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-49 | A4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-50 | A#4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-51 | B4 | OrchB, 0 Timpani ff Hi OrchB, 2 Timpani mf Hi |
| F02-K40-52 | C5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-53 | C#5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-54 | D5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-55 | D#5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-56 | E5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-57 | F5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-58 | F#5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-59 | G5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-60 | G#5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-61 | A5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-62 | A#5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-63 | B5 | OrchB, 19 Gong OrchB, 19 Gong |
| F02-K40-64 | C6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-65 | C#6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-66 | D6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-67 | D#6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-68 | E6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-69 | F6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-70 | F#6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-71 | G6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-72 | G#6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-73 | A6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-74 | A#6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-75 | B6 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K40-76 | C7 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K40-77 | C#7 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K40-78 | D7 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K40-79 | D#7 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K40-80 | E7 | ROM, 291 Tambourine-Acc.2 ROM, 291 Tambourine-Acc.2 |
| F02-K40-81 | F7 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K40-82 | F#7 | ROM, 252 Castanet-Single ROM, 252 Castanet-Single |
| F02-K40-83 | G7 | ROM, 253 Castanet-Dble 1 ROM, 253 Castanet-Dble 1 |
| F02-K40-84 | G#7 | ROM, 254 Castanet-Dble 2 ROM, 254 Castanet-Dble 2 |
| F02-K40-85 | A7 | OrchB, 20 Triangle OrchB, 20 Triangle |
| F02-K40-86 | A#7 | OrchB, 22 Triangle-Roll OrchB, 22 Triangle-Roll |
| F02-K40-87 | B7 | OrchB, 21 Triangle-Mute OrchB, 21 Triangle-Mute |

### F02-K41 - ROM Orch. Kit (kit 040, D1 p32)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K41-01 | A0 |  |
| F02-K41-02 | A#0 |  |
| F02-K41-03 | B0 |  |
| F02-K41-04 | C1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-05 | C#1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-06 | D1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-07 | D#1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-08 | E1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-09 | F1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-10 | F#1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-11 | G1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-12 | G#1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-13 | A1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-14 | A#1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-15 | B1 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-16 | C2 | ROM, 166 BD-Orchestral ROM, 166 BD-Orchestral |
| F02-K41-17 | C#2 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K41-18 | D2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K41-19 | D#2 | ROM, 171 SD-Orch.Roll 2 ROM, 171 SD-Orch.Roll 2 |
| F02-K41-20 | E2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K41-21 | F2 | ROM, 168 Orch Cym.-Open ROM, 168 Orch Cym.-Open |
| F02-K41-22 | F#2 | ROM, 167 Orch Cym.-Closed ROM, 167 Orch Cym.-Closed |
| F02-K41-23 | G2 | ROM, 274 Taiko-Open ROM, 274 Taiko-Open |
| F02-K41-24 | G#2 | ROM, 170 SD-Orch.Roll 1 ROM, 170 SD-Orch.Roll 1 |
| F02-K41-25 | A2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K41-26 | A#2 | ROM, 171 SD-Orch.Roll 2 ROM, 171 SD-Orch.Roll 2 |
| F02-K41-27 | B2 | ROM, 172 SD-Orchestral ROM, 172 SD-Orchestral |
| F02-K41-28 | C3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-29 | C#3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-30 | D3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-31 | D#3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-32 | E3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-33 | F3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-34 | F#3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-35 | G3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-36 | G#3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-37 | A3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-38 | A#3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-39 | B3 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-40 | C4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-41 | C#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-42 | D4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-43 | D#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-44 | E4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-45 | F4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-46 | F#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-47 | G4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-48 | G#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-49 | A4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-50 | A#4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-51 | B4 | ROM, 169 Timpani ROM, 169 Timpani |
| F02-K41-52 | C5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-53 | C#5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-54 | D5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-55 | D#5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-56 | E5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-57 | F5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-58 | F#5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-59 | G5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-60 | G#5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-61 | A5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-62 | A#5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-63 | B5 | ROM, 317 Chinese Gong ROM, 317 Chinese Gong |
| F02-K41-64 | C6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-65 | C#6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-66 | D6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-67 | D#6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-68 | E6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-69 | F6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-70 | F#6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-71 | G6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-72 | G#6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-73 | A6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-74 | A#6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-75 | B6 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-76 | C7 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-77 | C#7 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-78 | D7 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-79 | D#7 | ROM, 403 HarpGliss ROM, 403 HarpGliss |
| F02-K41-80 | E7 | ROM, 291 Tambourine-Acc.2 ROM, 291 Tambourine-Acc.2 |
| F02-K41-81 | F7 | ROM, 309 Sleigh Bell ROM, 309 Sleigh Bell |
| F02-K41-82 | F#7 | ROM, 252 Castanet-Single ROM, 252 Castanet-Single |
| F02-K41-83 | G7 | ROM, 253 Castanet-Dble 1 ROM, 253 Castanet-Dble 1 |
| F02-K41-84 | G#7 | ROM, 254 Castanet-Dble 2 ROM, 254 Castanet-Dble 2 |
| F02-K41-85 | A7 | ROM, 292 Triangle-Open ROM, 292 Triangle-Open |
| F02-K41-86 | A#7 | ROM, 294 Triangle-Roll ROM, 294 Triangle-Roll |
| F02-K41-87 | B7 | ROM, 293 Triangle-Mute ROM, 293 Triangle-Mute |

### F02-K42 - Harp Kit(maj) (kit 041, D1 p32)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K42-01 | A0 |  |
| F02-K42-02 | A#0 |  |
| F02-K42-03 | B0 |  |
| F02-K42-04 | C1 |  |
| F02-K42-05 | C#1 |  |
| F02-K42-06 | D1 |  |
| F02-K42-07 | D#1 |  |
| F02-K42-08 | E1 |  |
| F02-K42-09 | F1 |  |
| F02-K42-10 | F#1 |  |
| F02-K42-11 | G1 |  |
| F02-K42-12 | G#1 |  |
| F02-K42-13 | A1 |  |
| F02-K42-14 | A#1 |  |
| F02-K42-15 | B1 |  |
| F02-K42-16 | C2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-17 | C#2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-18 | D2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-19 | D#2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-20 | E2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-21 | F2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-22 | F#2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-23 | G2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-24 | G#2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-25 | A2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-26 | A#2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-27 | B2 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-28 | C3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-29 | C#3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-30 | D3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-31 | D#3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-32 | E3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-33 | F3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-34 | F#3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-35 | G3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-36 | G#3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-37 | A3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-38 | A#3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-39 | B3 | OrchB, 34 Harp-Chord-maj 2 OrchB, 33 Harp-Chord-maj 1 |
| F02-K42-40 | C4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-41 | C#4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-42 | D4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-43 | D#4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-44 | E4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-45 | F4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-46 | F#4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-47 | G4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-48 | G#4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-49 | A4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-50 | A#4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-51 | B4 | OrchB, 29 Harp-GlissUp-mj1 OrchB, 29 Harp-GlissUp-mj1 |
| F02-K42-52 | C5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-53 | C#5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-54 | D5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-55 | D#5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-56 | E5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-57 | F5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-58 | F#5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-59 | G5 | OrchB, 30 Harp-GlissUp-mj2 OrchB, 30 Harp-GlissUp-mj2 |
| F02-K42-60 | G#5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K42-61 | A5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K42-62 | A#5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K42-63 | B5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K42-64 | C6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K42-65 | C#6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K42-66 | D6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K42-67 | D#6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K42-68 | E6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K42-69 | F6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K42-70 | F#6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K42-71 | G6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K42-72 | G#6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K42-73 | A6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K42-74 | A#6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K42-75 | B6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K42-76 | C7 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K42-77 | C#7 |  |
| F02-K42-78 | D7 |  |
| F02-K42-79 | D#7 |  |
| F02-K42-80 | E7 |  |
| F02-K42-81 | F7 |  |
| F02-K42-82 | F#7 |  |
| F02-K42-83 | G7 |  |
| F02-K42-84 | G#7 |  |
| F02-K42-85 | A7 |  |
| F02-K42-86 | A#7 |  |
| F02-K42-87 | B7 |  |

### F02-K43 - Harp Kit(min) (kit 042, D1 p33)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K43-01 | A0 |  |
| F02-K43-02 | A#0 |  |
| F02-K43-03 | B0 |  |
| F02-K43-04 | C1 |  |
| F02-K43-05 | C#1 |  |
| F02-K43-06 | D1 |  |
| F02-K43-07 | D#1 |  |
| F02-K43-08 | E1 |  |
| F02-K43-09 | F1 |  |
| F02-K43-10 | F#1 |  |
| F02-K43-11 | G1 |  |
| F02-K43-12 | G#1 |  |
| F02-K43-13 | A1 |  |
| F02-K43-14 | A#1 |  |
| F02-K43-15 | B1 |  |
| F02-K43-16 | C2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-17 | C#2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-18 | D2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-19 | D#2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-20 | E2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-21 | F2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-22 | F#2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-23 | G2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-24 | G#2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-25 | A2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-26 | A#2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-27 | B2 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-28 | C3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-29 | C#3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-30 | D3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-31 | D#3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-32 | E3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-33 | F3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-34 | F#3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-35 | G3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-36 | G#3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-37 | A3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-38 | A#3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-39 | B3 | OrchB, 32 Harp-Chord-min 2 OrchB, 31 Harp-Chord-min 1 |
| F02-K43-40 | C4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-41 | C#4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-42 | D4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-43 | D#4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-44 | E4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-45 | F4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-46 | F#4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-47 | G4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-48 | G#4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-49 | A4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-50 | A#4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-51 | B4 | OrchB, 27 Harp-GlissUp-mi1 OrchB, 27 Harp-GlissUp-mi1 |
| F02-K43-52 | C5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-53 | C#5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-54 | D5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-55 | D#5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-56 | E5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-57 | F5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-58 | F#5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-59 | G5 | OrchB, 28 Harp-GlissUp-mi2 OrchB, 28 Harp-GlissUp-mi2 |
| F02-K43-60 | G#5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K43-61 | A5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K43-62 | A#5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K43-63 | B5 | OrchB, 23 Harp-Glissando 1 OrchB, 23 Harp-Glissando 1 |
| F02-K43-64 | C6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K43-65 | C#6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K43-66 | D6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K43-67 | D#6 | OrchB, 24 Harp-Glissando 2 OrchB, 24 Harp-Glissando 2 |
| F02-K43-68 | E6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K43-69 | F6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K43-70 | F#6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K43-71 | G6 | OrchB, 25 Harp-Glissando 3 OrchB, 25 Harp-Glissando 3 |
| F02-K43-72 | G#6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K43-73 | A6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K43-74 | A#6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K43-75 | B6 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K43-76 | C7 | OrchB, 26 Harp-Glissando 4 OrchB, 26 Harp-Glissando 4 |
| F02-K43-77 | C#7 |  |
| F02-K43-78 | D7 |  |
| F02-K43-79 | D#7 |  |
| F02-K43-80 | E7 |  |
| F02-K43-81 | F7 |  |
| F02-K43-82 | F#7 |  |
| F02-K43-83 | G7 |  |
| F02-K43-84 | G#7 |  |
| F02-K43-85 | A7 |  |
| F02-K43-86 | A#7 |  |
| F02-K43-87 | B7 |  |
| F02-K43-88 | C8 |  |
| F02-K43-89 | C#8 |  |
| F02-K43-90 | D8 |  |

### F02-K44 - Major Hit Kit (kit 043, D1 p33)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K44-01 | A0 | C-1 |
| F02-K44-02 | A#0 | … ROM, 0 BD-Dry 1 |
| F02-K44-03 | B0 | B-1 |
| F02-K44-04 | C1 | AC00 |
| F02-K44-05 | C#1 | …A#0 ROM, 223 Pizz Hit |
| F02-K44-06 | D1 | BB00 |
| F02-K44-07 | D#1 | C1 ROM, 223 Pizz Hit |
| F02-K44-08 | E1 | C#1 OrchS, 11 Cl Ens.Chord-maj |
| F02-K44-09 | F1 | D1 OrchS, 11 Cl Ens.Chord-maj |
| F02-K44-10 | F#1 | D#1 OrchS, 11 Cl Ens.Chord-maj |
| F02-K44-11 | G1 | E1 OrchS, 11 Cl Ens.Chord-maj |
| F02-K44-12 | G#1 | F1 OrchS, 5 Cl Ens. Arp-maj |
| F02-K44-13 | A1 | F#1 OrchS, 5 Cl Ens. Arp-maj |
| F02-K44-14 | A#1 | G1 OrchS, 5 Cl Ens. Arp-maj |
| F02-K44-15 | B1 | G#1 ROM, 223 Pizz Hit |
| F02-K44-16 | C2 | A1 ROM, 223 Pizz Hit |
| F02-K44-17 | C#2 | A#1 OrchS, 8 Vi Ens.Chord-maj |
| F02-K44-18 | D2 | B1 OrchS, 8 Vi Ens.Chord-maj |
| F02-K44-19 | D#2 | C2 OrchS, 8 Vi Ens.Chord-maj |
| F02-K44-20 | E2 | C#2 OrchS, 8 Vi Ens.Chord-maj |
| F02-K44-21 | F2 | D2 ROM, 222 Orchestra Hit |
| F02-K44-22 | F#2 | D#2 ROM, 222 Orchestra Hit |
| F02-K44-23 | G2 | E2 ROM, 222 Orchestra Hit |
| F02-K44-24 | G#2 | F2 ROM, 222 Orchestra Hit |
| F02-K44-25 | A2 | F#2 ROM, 222 Orchestra Hit |
| F02-K44-26 | A#2 | G2 ROM, 222 Orchestra Hit |
| F02-K44-27 | B2 | G#2 ROM, 224 Band Hit |
| F02-K44-28 | C3 | A2 ROM, 224 Band Hit |
| F02-K44-29 | C#3 | A#2 OrchS, 2 Vi Ens. Arp-maj |
| F02-K44-30 | D3 | B2 OrchS, 2 Vi Ens. Arp-maj |
| F02-K44-31 | D#3 | C3 OrchS, 2 Vi Ens. Arp-maj |
| F02-K44-32 | E3 | C#3 OrchS, 2 Vi Ens. Arp-maj |
| F02-K44-33 | F3 | D3 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K44-34 | F#3 | D#3 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K44-35 | G3 | E3 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K44-36 | G#3 | F3 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K44-37 | A3 | F#3 OrchS, 16 Vi Ens.Hit-maj 1 |
| F02-K44-38 | A#3 | G3 OrchS, 17 Vi Ens.Hit-maj 2 |
| F02-K44-39 | B3 | G#3 OrchS, 17 Vi Ens.Hit-maj 2 |
| F02-K44-40 | C4 | A3 OrchS, 17 Vi Ens.Hit-maj 2 |
| F02-K44-41 | C#4 | A#3 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-42 | D4 | B3 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-43 | D#4 | C4 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-44 | E4 | C#4 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-45 | F4 | D4 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-46 | F#4 | D#4 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-47 | G4 | E4 OrchS, 22 ViEns.RunUp-mj 1 |
| F02-K44-48 | G#4 | F4 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K44-49 | A4 | F#4 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K44-50 | A#4 | G4 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K44-51 | B4 | G#4 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K44-52 | C5 | A4 OrchS, 23 ViEns.RunUp-mj 2 |
| F02-K44-53 | C#5 | A#4 OrchS, 24 ViEns.RunUp-mj 3 |
| F02-K44-54 | D5 | B4 OrchS, 24 ViEns.RunUp-mj 3 |
| F02-K44-55 | D#5 | C5 OrchS, 24 ViEns.RunUp-mj 3 |
| F02-K44-56 | E5 | C#5 OrchS, 25 ViEns.RunUp-mj 4 |
| F02-K44-57 | F5 | D5 OrchS, 25 ViEns.RunUp-mj 4 |
| F02-K44-58 | F#5 | D#5 OrchS, 25 ViEns.RunUp-mj 4 |
| F02-K44-59 | G5 | E5 OrchS, 28 Vi Solo Arp-maj |
| F02-K44-60 | G#5 | F5 OrchS, 28 Vi Solo Arp-maj |
| F02-K44-61 | A5 | F#5 OrchS, 28 Vi Solo Arp-maj |
| F02-K44-62 | A#5 | G5 OrchS, 33 Vi SoloChord-maj |
| F02-K44-63 | B5 | G#5 OrchS, 33 Vi SoloChord-maj |
| F02-K44-64 | C6 | A5 OrchS, 33 Vi SoloChord-maj |
| F02-K44-65 | C#6 | A#5 ROM, 222 Orchestra Hit |
| F02-K44-66 | D6 | B5 ROM, 222 Orchestra Hit |
| F02-K44-67 | D#6 | C6 ROM, 222 Orchestra Hit |
| F02-K44-68 | E6 | C#6 ROM, 222 Orchestra Hit |
| F02-K44-69 | F6 | D6 ROM, 222 Orchestra Hit |
| F02-K44-70 | F#6 | D#6 ROM, 222 Orchestra Hit |
| F02-K44-71 | G6 | E6 ROM, 222 Orchestra Hit |
| F02-K44-72 | G#6 | F6 ROM, 222 Orchestra Hit |
| F02-K44-73 | A6 | F#6 ROM, 222 Orchestra Hit |
| F02-K44-74 | A#6 | G6 ROM, 222 Orchestra Hit |
| F02-K44-75 | B6 | G#6 ROM, 222 Orchestra Hit |
| F02-K44-76 | C7 | A6 ROM, 222 Orchestra Hit |
| F02-K44-77 | C#7 | A#6 ROM, 222 Orchestra Hit |
| F02-K44-78 | D7 | B6 ROM, 222 Orchestra Hit |
| F02-K44-79 | D#7 | C7 ROM, 222 Orchestra Hit |
| F02-K44-80 | E7 | C#7 ROM, 225 ImpactHit |
| F02-K44-81 | F7 | D7 ROM, 225 ImpactHit |
| F02-K44-82 | F#7 | D#7 ROM, 225 ImpactHit |
| F02-K44-83 | G7 | E7 ROM, 225 ImpactHit |
| F02-K44-84 | G#7 | FF77 |
| F02-K44-85 | A7 | …F#7 ROM, 225 ImpactHit |
| F02-K44-86 | A#7 | GC87 |
| F02-K44-87 | B7 | C#G8#7 |
| F02-K44-88 | C8 | A…7 ROM, 0 BD-Dry 1 |
| F02-K44-89 | C#8 | G9A#7 |
| F02-K44-90 | D8 |  |

### F02-K45 - Minor Hit Kit (kit 044, D1 p33)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K45-01 | A0 |  |
| F02-K45-02 | A#0 | ROM, 0 BD-Dry 1 |
| F02-K45-03 | B0 |  |
| F02-K45-04 | C1 |  |
| F02-K45-05 | C#1 | ROM, 222 Orchestra Hit |
| F02-K45-06 | D1 |  |
| F02-K45-07 | D#1 | ROM, 222 Orchestra Hit |
| F02-K45-08 | E1 | OrchS, 10 Cl Ens.Chord-min |
| F02-K45-09 | F1 | OrchS, 10 Cl Ens.Chord-min |
| F02-K45-10 | F#1 | OrchS, 10 Cl Ens.Chord-min |
| F02-K45-11 | G1 | OrchS, 10 Cl Ens.Chord-min |
| F02-K45-12 | G#1 | OrchS, 4 Cl Ens. Arp-min |
| F02-K45-13 | A1 | OrchS, 4 Cl Ens. Arp-min |
| F02-K45-14 | A#1 | OrchS, 4 Cl Ens. Arp-min |
| F02-K45-15 | B1 | ROM, 223 Pizz Hit |
| F02-K45-16 | C2 | ROM, 223 Pizz Hit |
| F02-K45-17 | C#2 | OrchS, 7 Vi Ens.Chord-min |
| F02-K45-18 | D2 | OrchS, 7 Vi Ens.Chord-min |
| F02-K45-19 | D#2 | OrchS, 7 Vi Ens.Chord-min |
| F02-K45-20 | E2 | OrchS, 7 Vi Ens.Chord-min |
| F02-K45-21 | F2 | ROM, 222 Orchestra Hit |
| F02-K45-22 | F#2 | ROM, 222 Orchestra Hit |
| F02-K45-23 | G2 | ROM, 222 Orchestra Hit |
| F02-K45-24 | G#2 | ROM, 222 Orchestra Hit |
| F02-K45-25 | A2 | ROM, 222 Orchestra Hit |
| F02-K45-26 | A#2 | ROM, 222 Orchestra Hit |
| F02-K45-27 | B2 | ROM, 224 Band Hit |
| F02-K45-28 | C3 | ROM, 224 Band Hit |
| F02-K45-29 | C#3 | OrchS, 1 Vi Ens. Arp-min |
| F02-K45-30 | D3 | OrchS, 1 Vi Ens. Arp-min |
| F02-K45-31 | D#3 | OrchS, 1 Vi Ens. Arp-min |
| F02-K45-32 | E3 | OrchS, 1 Vi Ens. Arp-min |
| F02-K45-33 | F3 | OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K45-34 | F#3 | OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K45-35 | G3 | OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K45-36 | G#3 | OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K45-37 | A3 | OrchS, 14 Vi Ens.Hit-min 1 |
| F02-K45-38 | A#3 | OrchS, 15 Vi Ens.Hit-min 2 |
| F02-K45-39 | B3 | OrchS, 15 Vi Ens.Hit-min 2 |
| F02-K45-40 | C4 | OrchS, 15 Vi Ens.Hit-min 2 |
| F02-K45-41 | C#4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-42 | D4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-43 | D#4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-44 | E4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-45 | F4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-46 | F#4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-47 | G4 | OrchS, 18 ViEns.RunUp-mi 1 |
| F02-K45-48 | G#4 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K45-49 | A4 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K45-50 | A#4 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K45-51 | B4 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K45-52 | C5 | OrchS, 19 ViEns.RunUp-mi 2 |
| F02-K45-53 | C#5 | OrchS, 20 ViEns.RunUp-mi 3 |
| F02-K45-54 | D5 | OrchS, 20 ViEns.RunUp-mi 3 |
| F02-K45-55 | D#5 | OrchS, 20 ViEns.RunUp-mi 3 |
| F02-K45-56 | E5 | OrchS, 21 ViEns.RunUp-mi 4 |
| F02-K45-57 | F5 | OrchS, 21 ViEns.RunUp-mi 4 |
| F02-K45-58 | F#5 | OrchS, 21 ViEns.RunUp-mi 4 |
| F02-K45-59 | G5 | OrchS, 27 Vi Solo Arp-min |
| F02-K45-60 | G#5 | OrchS, 27 Vi Solo Arp-min |
| F02-K45-61 | A5 | OrchS, 27 Vi Solo Arp-min |
| F02-K45-62 | A#5 | OrchS, 32 Vi SoloChord-min |
| F02-K45-63 | B5 | OrchS, 32 Vi SoloChord-min |
| F02-K45-64 | C6 | OrchS, 32 Vi SoloChord-min |
| F02-K45-65 | C#6 | ROM, 223 Pizz Hit |
| F02-K45-66 | D6 | ROM, 223 Pizz Hit |
| F02-K45-67 | D#6 | ROM, 223 Pizz Hit |
| F02-K45-68 | E6 | ROM, 223 Pizz Hit |
| F02-K45-69 | F6 | ROM, 223 Pizz Hit |
| F02-K45-70 | F#6 | ROM, 223 Pizz Hit |
| F02-K45-71 | G6 | ROM, 223 Pizz Hit |
| F02-K45-72 | G#6 | ROM, 223 Pizz Hit |
| F02-K45-73 | A6 | ROM, 223 Pizz Hit |
| F02-K45-74 | A#6 | ROM, 223 Pizz Hit |
| F02-K45-75 | B6 | ROM, 223 Pizz Hit |
| F02-K45-76 | C7 | ROM, 223 Pizz Hit |
| F02-K45-77 | C#7 | ROM, 223 Pizz Hit |
| F02-K45-78 | D7 | ROM, 223 Pizz Hit |
| F02-K45-79 | D#7 | ROM, 223 Pizz Hit |
| F02-K45-80 | E7 | ROM, 225 ImpactHit |
| F02-K45-81 | F7 | ROM, 225 ImpactHit |
| F02-K45-82 | F#7 | ROM, 225 ImpactHit |
| F02-K45-83 | G7 | ROM, 225 ImpactHit |
| F02-K45-84 | G#7 |  |
| F02-K45-85 | A7 | ROM, 225 ImpactHit |
| F02-K45-86 | A#7 |  |
| F02-K45-87 | B7 |  |
| F02-K45-88 | C8 | ROM, 0 BD-Dry 1 |
| F02-K45-89 | C#8 | 33 |
| F02-K45-90 | D8 |  |

### F02-K46 - EXB Cymbals set (kit 045, D1 p34)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K46-01 | A0 |  |
| F02-K46-02 | A#0 |  |
| F02-K46-03 | B0 |  |
| F02-K46-04 | C1 |  |
| F02-K46-05 | C#1 |  |
| F02-K46-06 | D1 |  |
| F02-K46-07 | D#1 |  |
| F02-K46-08 | E1 | ROM, 169 Timpani |
| F02-K46-09 | F1 | ROM, 169 Timpani |
| F02-K46-10 | F#1 | ROM, 169 Timpani |
| F02-K46-11 | G1 | ROM, 169 Timpani |
| F02-K46-12 | G#1 | ROM, 169 Timpani |
| F02-K46-13 | A1 | ROM, 169 Timpani |
| F02-K46-14 | A#1 | ROM, 169 Timpani |
| F02-K46-15 | B1 | ROM, 169 Timpani |
| F02-K46-16 | C2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-17 | C#2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-18 | D2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-19 | D#2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-20 | E2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-21 | F2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-22 | F#2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-23 | G2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-24 | G#2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-25 | A2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-26 | A#2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-27 | B2 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-28 | C3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-29 | C#3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-30 | D3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-31 | D#3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-32 | E3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-33 | F3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-34 | F#3 | OrchB, 17 Piatti pp OrchB, 17 Piatti pp |
| F02-K46-35 | G3 | OrchB, 16 Piatti ff |
| F02-K46-36 | G#3 | OrchB, 16 Piatti ff |
| F02-K46-37 | A3 | OrchB, 16 Piatti ff |
| F02-K46-38 | A#3 | OrchB, 16 Piatti ff |
| F02-K46-39 | B3 | OrchB, 16 Piatti ff |
| F02-K46-40 | C4 | OrchB, 16 Piatti ff |
| F02-K46-41 | C#4 | OrchB, 16 Piatti ff |
| F02-K46-42 | D4 | OrchB, 16 Piatti ff |
| F02-K46-43 | D#4 | OrchB, 16 Piatti ff |
| F02-K46-44 | E4 | OrchB, 16 Piatti ff |
| F02-K46-45 | F4 | OrchB, 16 Piatti ff |
| F02-K46-46 | F#4 | OrchB, 16 Piatti ff |
| F02-K46-47 | G4 | OrchB, 16 Piatti ff |
| F02-K46-48 | G#4 | OrchB, 16 Piatti ff |
| F02-K46-49 | A4 | OrchB, 16 Piatti ff |
| F02-K46-50 | A#4 | OrchB, 16 Piatti ff |
| F02-K46-51 | B4 | OrchB, 16 Piatti ff |
| F02-K46-52 | C5 | OrchB, 16 Piatti ff |
| F02-K46-53 | C#5 | OrchB, 16 Piatti ff |
| F02-K46-54 | D5 | OrchB, 16 Piatti ff |
| F02-K46-55 | D#5 | OrchB, 16 Piatti ff |
| F02-K46-56 | E5 | OrchB, 16 Piatti ff |
| F02-K46-57 | F5 | OrchB, 16 Piatti ff |
| F02-K46-58 | F#5 | OrchB, 16 Piatti ff |
| F02-K46-59 | G5 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-60 | G#5 | OrchB, 16 Piatti ff OrchB, 16 Piatti ff |
| F02-K46-61 | A5 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-62 | A#5 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-63 | B5 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-64 | C6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-65 | C#6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-66 | D6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-67 | D#6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-68 | E6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-69 | F6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-70 | F#6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-71 | G6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-72 | G#6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-73 | A6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-74 | A#6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-75 | B6 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-76 | C7 | OrchB, 18 Piatti Short OrchB, 18 Piatti Short |
| F02-K46-77 | C#7 | ROM, 168 Orch Cym.-Open |
| F02-K46-78 | D7 | ROM, 168 Orch Cym.-Open |
| F02-K46-79 | D#7 | ROM, 168 Orch Cym.-Open |
| F02-K46-80 | E7 | ROM, 168 Orch Cym.-Open |
| F02-K46-81 | F7 | ROM, 168 Orch Cym.-Open |
| F02-K46-82 | F#7 | ROM, 168 Orch Cym.-Open |
| F02-K46-83 | G7 | ROM, 168 Orch Cym.-Open |
| F02-K46-84 | G#7 |  |
| F02-K46-85 | A7 |  |
| F02-K46-86 | A#7 |  |
| F02-K46-87 | B7 |  |
| F02-K46-88 | C8 |  |
| F02-K46-89 | D8 |  |

### F02-K47 - Trance/GarageKit (kit 128, D1 p34)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K47-01 | A0 |  |
| F02-K47-02 | A#0 |  |
| F02-K47-03 | B0 |  |
| F02-K47-04 | C1 | ROM, 338 Four... Solo |
| F02-K47-05 | C#1 | ROM, 325 Yea Solo |
| F02-K47-06 | D1 | ROM, 339 Three... Solo |
| F02-K47-07 | D#1 | ROM, 326 Year solo |
| F02-K47-08 | E1 | ROM, 340 Two... Solo |
| F02-K47-09 | F1 | ROM, 341 One... Solo |
| F02-K47-10 | F#1 | ROM, 329 Uhhhh Solo |
| F02-K47-11 | G1 | ROM, 207 Zap 3 |
| F02-K47-12 | G#1 | ROM, 228 Finger Snap |
| F02-K47-13 | A1 | ROM, 206 Zap 2 |
| F02-K47-14 | A#1 | ROM, 157 Cymbal Reverse |
| F02-K47-15 | B1 | ROM, 157 Cymbal Reverse |
| F02-K47-16 | C2 | ROM, 178 99-BD 3 |
| F02-K47-17 | C#2 | ROM, 12 BD-Mondo Kill |
| F02-K47-18 | D2 | ROM, 178 99-BD 3 |
| F02-K47-19 | D#2 | ROM, 17 BD-Squash |
| F02-K47-20 | E2 | ROM, 21 BD-Dance 4 |
| F02-K47-21 | F2 | ROM, 48 SD-Full Room |
| F02-K47-22 | F#2 | ROM, 64 SD-Cracker Room |
| F02-K47-23 | G2 | ROM, 78 SD-Hip 5 |
| F02-K47-24 | G#2 | ROM, 85 SD-Vintage 4 |
| F02-K47-25 | A2 | ROM, 99 SD-Ambi Hit 2 |
| F02-K47-26 | A#2 | ROM, 81 SD-Tiny |
| F02-K47-27 | B2 | ROM, 180 99-BD 5 |
| F02-K47-28 | C3 | ROM, 178 99-BD 3 |
| F02-K47-29 | C#3 | ROM, 203 Dance Perc-Hous1 |
| F02-K47-30 | D3 | ROM, 183 99-SD 1 |
| F02-K47-31 | D#3 | ROM, 186 88-Claps |
| F02-K47-32 | E3 | ROM, 184 99-SD 2 |
| F02-K47-33 | F3 | ROM, 198 Real E.Tom |
| F02-K47-34 | F#3 | ROM, 189 99-HHat Closed |
| F02-K47-35 | G3 | ROM, 198 Real E.Tom |
| F02-K47-36 | G#3 | ROM, 135 H.Hat-Crisp Open |
| F02-K47-37 | A3 | ROM, 198 Real E.Tom |
| F02-K47-38 | A#3 | ROM, 190 99-HHat Open |
| F02-K47-39 | B3 | ROM, 198 Real E.Tom |
| F02-K47-40 | C4 | ROM, 198 Real E.Tom |
| F02-K47-41 | C#4 | ROM, 158 Crash-Dance 99 |
| F02-K47-42 | D4 | ROM, 198 Real E.Tom |
| F02-K47-43 | D#4 | ROM, 160 Ride-Dance 99 |
| F02-K47-44 | E4 | ROM, 153 Crash Cymbal 2 |
| F02-K47-45 | F4 | ROM, 70 SD-Rap 2 |
| F02-K47-46 | F#4 | ROM, 288 Tambourine-Push |
| F02-K47-47 | G4 | ROM, 87 SD-Vintage 6 |
| F02-K47-48 | G#4 | ROM, 289 Tambourine-Pull |
| F02-K47-49 | A4 | ROM, 69 SD-Rap 1 |
| F02-K47-50 | A#4 | ROM, 290 Tambourine-Acc.1 |
| F02-K47-51 | B4 | ROM, 291 Tambourine-Acc.2 |
| F02-K47-52 | C5 | ROM, 245 Bongo Hi-Open |
| F02-K47-53 | C#5 | ROM, 242 Bongo Lo-Open |
| F02-K47-54 | D5 | ROM, 237 Conga Hi-MtSlap |
| F02-K47-55 | D#5 | ROM, 235 Conga Hi-Open |
| F02-K47-56 | E5 | ROM, 232 Conga Lo-Open |
| F02-K47-57 | F5 | ROM, 233 Conga Lo-MtSlap |
| F02-K47-58 | F#5 | ROM, 234 Conga Lo-Slap |
| F02-K47-59 | G5 | ROM, 233 Conga Lo-MtSlap |
| F02-K47-60 | G#5 | ROM, 239 Conga Hi-Slap 2 |
| F02-K47-61 | A5 | ROM, 238 Conga Hi-Slap 1 |
| F02-K47-62 | A#5 | ROM, 243 Bongo Lo-Slap |
| F02-K47-63 | B5 | ROM, 246 Bongo Hi-Slap |
| F02-K47-64 | C6 | ROM, 220 DJ-Reverse |
| F02-K47-65 | C#6 | ROM, 215 DJ-1c Vocal Rub |
| F02-K47-66 | D6 | ROM, 214 DJ-Hit Rub |
| F02-K47-67 | D#6 | ROM, 219 DJ-Record Stop |
| F02-K47-68 | E6 | ROM, 216 DJ-1c Vocal Rub |
| F02-K47-69 | F6 | ROM, 212 DJ-Scratch 3c |
| F02-K47-70 | F#6 | ROM, 218 DJ-SD Rub |
| F02-K47-71 | G6 | ROM, 217 DJ-BD Rub |
| F02-K47-72 | G#6 | ROM, 293 Triangle-Mute |
| F02-K47-73 | A6 | ROM, 292 Triangle-Open |
| F02-K47-74 | A#6 | ROM, 230 Hand Claps-Hip |
| F02-K47-75 | B6 | ROM, 312 Marc Tree |
| F02-K47-76 | C7 | ROM, 174 88-BD 2(R&B) |
| F02-K47-77 | C#7 | ROM, 187 88-HHat Closed |
| F02-K47-78 | D7 | ROM, 182 88-SD 2(Old) |
| F02-K47-79 | D#7 | ROM, 188 88-HHat Open |
| F02-K47-80 | E7 | ROM, 191 88-Crash |
| F02-K47-81 | F7 | ROM, 195 88-Cowbell |
| F02-K47-82 | F#7 | ROM, 186 88-Claps |
| F02-K47-83 | G7 | ROM, 196 88-Maraca |
| F02-K47-84 | G#7 | ROM, 185 88-Rimshot |
| F02-K47-85 | A7 | ROM, 181 88-SD 1(New) |
| F02-K47-86 | A#7 | ROM, 194 88-Clave |
| F02-K47-87 | B7 | ROM, 354 Stadium |
| F02-K47-88 | C8 | ROM, 311 Finger Cymbal |
| F02-K47-89 | D8 |  |

### F02-K48 - Big Trouble Kit (kit 129, D1 p34)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K48-01 | A0 |  |
| F02-K48-02 | A#0 |  |
| F02-K48-03 | B0 |  |
| F02-K48-04 | C1 | ROM, 260 Baya-Open |
| F02-K48-05 | C#1 | ROM, 261 Baya-Ghe |
| F02-K48-06 | D1 | ROM, 263 Baya-Mute 2 |
| F02-K48-07 | D#1 | ROM, 265 Baya-Mute 4 |
| F02-K48-08 | E1 | ROM, 268 Tabla-Open |
| F02-K48-09 | F1 | ROM, 269 Tabla-Tin |
| F02-K48-10 | F#1 | ROM, 270 Tabla-Mute 1 |
| F02-K48-11 | G1 | ROM, 271 Tabla-Mute 2 |
| F02-K48-12 | G#1 | ROM, 248 Bongo Hi-Stick 2 ROM, 247 Bongo Hi-Stick 1 |
| F02-K48-13 | A1 | ROM, 251 Djembe-Slap ROM, 251 Djembe-Slap |
| F02-K48-14 | A#1 | ROM, 179 99-BD 4 ROM, 179 99-BD 4 |
| F02-K48-15 | B1 | ROM, 2 BD-Dry 3 |
| F02-K48-16 | C2 | ROM, 25 BD-Hip 4 |
| F02-K48-17 | C#2 | ROM, 174 88-BD 2(R&B) ROM, 174 88-BD 2(R&B) |
| F02-K48-18 | D2 | ROM, 175 88-BD 3 ROM, 6 BD-Soft |
| F02-K48-19 | D#2 | ROM, 28 BD-Ringy ROM, 30 BD-Dance99 |
| F02-K48-20 | E2 | ROM, 34 BD-Dance Soft ROM, 34 BD-Dance Soft |
| F02-K48-21 | F2 | ROM, 100 SD-Ambi Hit 3 ROM, 100 SD-Ambi Hit 3 |
| F02-K48-22 | F#2 | ROM, 94 SD-Klanger ROM, 94 SD-Klanger |
| F02-K48-23 | G2 | ROM, 93 SD-Chili ROM, 93 SD-Chili |
| F02-K48-24 | G#2 | ROM, 86 SD-Vintage 5 ROM, 83 SD-Vintage 2 |
| F02-K48-25 | A2 | ROM, 75 SD-Hip 2 ROM, 74 SD-Hip 1 |
| F02-K48-26 | A#2 | ROM, 70 SD-Rap 2 |
| F02-K48-27 | B2 | ROM, 14 BD-Tubby ROM, 18 BD-Dance 1 |
| F02-K48-28 | C3 | ROM, 8 BD-Jazz 1 ROM, 8 BD-Jazz 1 |
| F02-K48-29 | C#3 | ROM, 65 Sidestick-Dry ROM, 65 Sidestick-Dry |
| F02-K48-30 | D3 | ROM, 79 SD-Hip 6 ROM, 79 SD-Hip 6 |
| F02-K48-31 | D#3 | ROM, 230 Hand Claps-Hip |
| F02-K48-32 | E3 | ROM, 56 SD-Brush Tap 1 ROM, 56 SD-Brush Tap 1 |
| F02-K48-33 | F3 | ROM, 115 Tom-Brush Floor ROM, 115 Tom-Brush Floor |
| F02-K48-34 | F#3 | ROM, 148 H.Hat-Ambi ROM, 132 H.Hat-Chili |
| F02-K48-35 | G3 | ROM, 115 Tom-Brush Floor ROM, 115 Tom-Brush Floor |
| F02-K48-36 | G#3 | ROM, 123 H.Hat 1-Open 2 ROM, 118 H.Hat 1-Closed 1 |
| F02-K48-37 | A3 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K48-38 | A#3 | ROM, 130 H.Hat 2-Open 2 ROM, 129 H.Hat 2-Open 1 |
| F02-K48-39 | B3 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K48-40 | C4 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K48-41 | C#4 | ROM, 153 Crash Cymbal 2 |
| F02-K48-42 | D4 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K48-43 | D#4 | ROM, 162 Ride-Edge 1 ROM, 162 Ride-Edge 1 |
| F02-K48-44 | E4 | ROM, 155 China Cymbal |
| F02-K48-45 | F4 | ROM, 165 Ride-Cup |
| F02-K48-46 | F#4 | ROM, 290 Tambourine-Acc.1 ROM, 289 Tambourine-Pull |
| F02-K48-47 | G4 | ROM, 156 Splash Cymbal |
| F02-K48-48 | G#4 | ROM, 360 Click |
| F02-K48-49 | A4 | ROM, 191 88-Crash |
| F02-K48-50 | A#4 | ROM, 207 Zap 3 ROM, 205 Zap 1 |
| F02-K48-51 | B4 | ROM, 393 Cricket Spectrum ROM, 393 Cricket Spectrum |
| F02-K48-52 | C5 | ROM, 270 Tabla-Mute 1 ROM, 272 Tabla-Mute 3 |
| F02-K48-53 | C#5 | ROM, 273 Tabla-Mute 4 ROM, 271 Tabla-Mute 2 |
| F02-K48-54 | D5 | ROM, 367 Hart Beat ROM, 367 Hart Beat |
| F02-K48-55 | D#5 | ROM, 365 Crickets 1 ROM, 365 Crickets 1 |
| F02-K48-56 | E5 | ROM, 314 Bell Tree |
| F02-K48-57 | F5 | ROM, 331 CompuVoice-Noise |
| F02-K48-58 | F#5 | ROM, 332 CompuVoice-Chi |
| F02-K48-59 | G5 | ROM, 333 CompuVoice-Test |
| F02-K48-60 | G#5 | ROM, 336 CompuVoice-Toi |
| F02-K48-61 | A5 | ROM, 352 PC Vox-O |
| F02-K48-62 | A#5 | ROM, 353 PC Vox-Tu |
| F02-K48-63 | B5 | ROM, 343 PC Vox-Dunc |
| F02-K48-64 | C6 | ROM, 347 PC Vox-If |
| F02-K48-65 | C#6 | ROM, 408 E.Guitar Pick 1 |
| F02-K48-66 | D6 | ROM, 409 E.Guitar Pick 2 |
| F02-K48-67 | D#6 | ROM, 410 E.Guitar Pick 3 |
| F02-K48-68 | E6 | ROM, 321 SD-Air Explosion |
| F02-K48-69 | F6 | ROM, 323 Industry Loop 2 |
| F02-K48-70 | F#6 | ROM, 354 Stadium |
| F02-K48-71 | G6 | ROM, 356 Scream |
| F02-K48-72 | G#6 | ROM, 391 Swish Terra |
| F02-K48-73 | A6 | ROM, 319 Cyber Drum |
| F02-K48-74 | A#6 | ROM, 326 Year solo |
| F02-K48-75 | B6 | ROM, 357 Laughing |
| F02-K48-76 | C7 | ROM, 372 Door Creak |
| F02-K48-77 | C#7 | ROM, 316 Samba Whistle |
| F02-K48-78 | D7 | ROM, 317 Chinese Gong |
| F02-K48-79 | D#7 | ROM, 318 Industry |
| F02-K48-80 | E7 | ROM, 323 Industry Loop 2 |
| F02-K48-81 | F7 | ROM, 324 Metal Hit |
| F02-K48-82 | F#7 | ROM, 354 Stadium |
| F02-K48-83 | G7 | ROM, 362 Bird 2 ROM, 362 Bird 2 |
| F02-K48-84 | G#7 | ROM, 374 Car Engine |
| F02-K48-85 | A7 | ROM, 375 Car Stop |
| F02-K48-86 | A#7 | ROM, 378 Train |
| F02-K48-87 | B7 | ROM, 414 ROM, 413 |
| F02-K48-88 | C8 | ROM, 416 ROM, 415 |
| F02-K48-89 | D8 |  |

### F02-K49 - Electro Kit 1 (kit 130, D1 p35)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K49-01 | A0 |  |
| F02-K49-02 | A#0 |  |
| F02-K49-03 | B0 |  |
| F02-K49-04 | C1 | ROM, 26 BD-Dark ROM, 26 BD-Dark |
| F02-K49-05 | C#1 | ROM, 32 BD-Ambi Crackle ROM, 32 BD-Ambi Crackle |
| F02-K49-06 | D1 | ROM, 24 BD-Hip 3 ROM, 20 BD-Dance 3 |
| F02-K49-07 | D#1 | ROM, 175 88-BD 3 ROM, 175 88-BD 3 |
| F02-K49-08 | E1 | ROM, 178 99-BD 3 |
| F02-K49-09 | F1 | ROM, 100 SD-Ambi Hit 3 ROM, 100 SD-Ambi Hit 3 |
| F02-K49-10 | F#1 | ROM, 77 SD-Hip 4 |
| F02-K49-11 | G1 | ROM, 45 SD-GhostSet f |
| F02-K49-12 | G#1 | ROM, 44 SD-GhostSet roll |
| F02-K49-13 | A1 | ROM, 46 SD-GhostSet p1 ROM, 47 SD-GhostSet p2 |
| F02-K49-14 | A#1 | ROM, 88 SD-AmbiCrackle 1 |
| F02-K49-15 | B1 | ROM, 90 SD-AmbiCrackle 3 |
| F02-K49-16 | C2 | ROM, 14 BD-Tubby ROM, 14 BD-Tubby |
| F02-K49-17 | C#2 | ROM, 20 BD-Dance 3 ROM, 20 BD-Dance 3 |
| F02-K49-18 | D2 | ROM, 16 BD-Tight ROM, 16 BD-Tight |
| F02-K49-19 | D#2 | ROM, 177 99-BD 2 ROM, 177 99-BD 2 |
| F02-K49-20 | E2 | ROM, 27 BD-Pop ROM, 27 BD-Pop |
| F02-K49-21 | F2 | ROM, 52 SD-Amb.Piccolo 2 |
| F02-K49-22 | F#2 | ROM, 83 SD-Vintage 2 |
| F02-K49-23 | G2 | ROM, 396 Noise-FM Mod ROM, 396 Noise-FM Mod |
| F02-K49-24 | G#2 | ROM, 78 SD-Hip 5 |
| F02-K49-25 | A2 | ROM, 84 SD-Vintage 3 |
| F02-K49-26 | A#2 | ROM, 182 88-SD 2(Old) |
| F02-K49-27 | B2 | ROM, 175 88-BD 3 ROM, 175 88-BD 3 |
| F02-K49-28 | C3 | ROM, 180 99-BD 5 ROM, 180 99-BD 5 |
| F02-K49-29 | C#3 | ROM, 320 SD-Metal Striker |
| F02-K49-30 | D3 | ROM, 85 SD-Vintage 4 ROM, 85 SD-Vintage 4 |
| F02-K49-31 | D#3 | ROM, 231 Hand Claps-Happy |
| F02-K49-32 | E3 | ROM, 395 Noise-White |
| F02-K49-33 | F3 | ROM, 197 FM E.Tom ROM, 197 FM E.Tom |
| F02-K49-34 | F#3 | ROM, 119 H.Hat 1-Closed 2 ROM, 118 H.Hat 1-Closed 1 |
| F02-K49-35 | G3 | ROM, 197 FM E.Tom ROM, 197 FM E.Tom |
| F02-K49-36 | G#3 | ROM, 122 H.Hat 1-Open 1 ROM, 119 H.Hat 1-Closed 2 |
| F02-K49-37 | A3 | ROM, 197 FM E.Tom |
| F02-K49-38 | A#3 | ROM, 130 H.Hat 2-Open 2 |
| F02-K49-39 | B3 | ROM, 197 FM E.Tom ROM, 197 FM E.Tom |
| F02-K49-40 | C4 | ROM, 197 FM E.Tom ROM, 197 FM E.Tom |
| F02-K49-41 | C#4 | ROM, 158 Crash-Dance 99 |
| F02-K49-42 | D4 | ROM, 197 FM E.Tom ROM, 197 FM E.Tom |
| F02-K49-43 | D#4 | ROM, 160 Ride-Dance 99 |
| F02-K49-44 | E4 | ROM, 159 Crash-Dance DDD1 |
| F02-K49-45 | F4 | ROM, 163 Ride-Edge 2 |
| F02-K49-46 | F#4 | ROM, 322 Industry Loop 1 ROM, 322 Industry Loop 1 |
| F02-K49-47 | G4 | ROM, 156 Splash Cymbal |
| F02-K49-48 | G#4 | ROM, 321 SD-Air Explosion ROM, 321 SD-Air Explosion |
| F02-K49-49 | A4 | ROM, 153 Crash Cymbal 2 |
| F02-K49-50 | A#4 | ROM, 385 Wind ROM, 385 Wind |
| F02-K49-51 | B4 | ROM, 199 Vocal SD |
| F02-K49-52 | C5 | ROM, 247 Bongo Hi-Stick 1 ROM, 247 Bongo Hi-Stick 1 |
| F02-K49-53 | C#5 | ROM, 244 Bongo Lo-Stick ROM, 244 Bongo Lo-Stick |
| F02-K49-54 | D5 | ROM, 251 Djembe-Slap |
| F02-K49-55 | D#5 | ROM, 239 Conga Hi-Slap 2 |
| F02-K49-56 | E5 | ROM, 234 Conga Lo-Slap |
| F02-K49-57 | F5 | ROM, 408 E.Guitar Pick 1 |
| F02-K49-58 | F#5 | ROM, 410 E.Guitar Pick 3 |
| F02-K49-59 | G5 | ROM, 375 Car Stop |
| F02-K49-60 | G#5 | ROM, 375 Car Stop |
| F02-K49-61 | A5 | ROM, 354 Stadium ROM, 354 Stadium |
| F02-K49-62 | A#5 | ROM, 371 Rainstick |
| F02-K49-63 | B5 | ROM, 414 ROM, 413 |
| F02-K49-64 | C6 | ROM, 416 ROM, 415 |
| F02-K49-65 | C#6 | ROM, 335 CompuVoice-When |
| F02-K49-66 | D6 | ROM, 336 CompuVoice-Toi |
| F02-K49-67 | D#6 | ROM, 331 CompuVoice-Noise |
| F02-K49-68 | E6 | ROM, 333 CompuVoice-Test |
| F02-K49-69 | F6 | ROM, 345 PC Vox-For ROM, 343 PC Vox-Dunc |
| F02-K49-70 | F#6 | ROM, 356 Scream |
| F02-K49-71 | G6 | ROM, 357 Laughing |
| F02-K49-72 | G#6 | ROM, 374 Car Engine |
| F02-K49-73 | A6 | ROM, 376 Car Pass |
| F02-K49-74 | A#6 | ROM, 377 Car Crash |
| F02-K49-75 | B6 | ROM, 380 Gun Shot 2 |
| F02-K49-76 | C7 | ROM, 395 Noise-White |
| F02-K49-77 | C#7 | ROM, 189 99-HHat Closed |
| F02-K49-78 | D7 | ROM, 190 99-HHat Open |
| F02-K49-79 | D#7 | ROM, 140 H.Hat-Tight 2 |
| F02-K49-80 | E7 | ROM, 149 H.Hat-ClangyOpen |
| F02-K49-81 | F7 | ROM, 143 H.Hat-Old Cl 3 |
| F02-K49-82 | F#7 | ROM, 144 H.Hat-Old Open |
| F02-K49-83 | G7 | ROM, 228 Finger Snap |
| F02-K49-84 | G#7 | ROM, 274 Taiko-Open |
| F02-K49-85 | A7 | ROM, 227 Drumstick Hit |
| F02-K49-86 | A#7 | ROM, 387 Bubble |
| F02-K49-87 | B7 | ROM, 382 Laser Gun |
| F02-K49-88 | C8 | ROM, 312 Marc Tree |
| F02-K49-89 | C#8 |  |
| F02-K49-90 | D8 |  |

### F02-K50 - Wacky FX Kit (kit 131, D1 p35)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-K50-01 | A0 |  |
| F02-K50-02 | A#0 |  |
| F02-K50-03 | B0 |  |
| F02-K50-04 | C1 | ROM, 341 One... Solo |
| F02-K50-05 | C#1 | ROM, 340 Two... Solo |
| F02-K50-06 | D1 | ROM, 339 Three... Solo |
| F02-K50-07 | D#1 | ROM, 338 Four... Solo |
| F02-K50-08 | E1 | ROM, 317 Chinese Gong |
| F02-K50-09 | F1 | ROM, 202 Dance Perc-Hip |
| F02-K50-10 | F#1 | ROM, 201 Vocal Woodblock |
| F02-K50-11 | G1 | ROM, 200 Vocal Cowbell |
| F02-K50-12 | G#1 | ROM, 209 DJ-Scratch 2 |
| F02-K50-13 | A1 | ROM, 210 DJ-Scratch 3a |
| F02-K50-14 | A#1 | ROM, 34 BD-Dance Soft |
| F02-K50-15 | B1 | ROM, 35 BD-AmbiRocker |
| F02-K50-16 | C2 | ROM, 249 Djembe-Open ROM, 319 Cyber Drum |
| F02-K50-17 | C#2 | ROM, 352 PC Vox-O ROM, 352 PC Vox-O |
| F02-K50-18 | D2 | ROM, 373 Door Slam ROM, 23 BD-Hip 2 |
| F02-K50-19 | D#2 | ROM, 260 Baya-Open ROM, 260 Baya-Open |
| F02-K50-20 | E2 | ROM, 402 StringSlap ROM, 402 StringSlap |
| F02-K50-21 | F2 | ROM, 114 Tom-Brush Hi ROM, 114 Tom-Brush Hi |
| F02-K50-22 | F#2 | ROM, 402 StringSlap ROM, 402 StringSlap |
| F02-K50-23 | G2 | ROM, 395 Noise-White |
| F02-K50-24 | G#2 | ROM, 378 Train ROM, 378 Train |
| F02-K50-25 | A2 | ROM, 331 CompuVoice-Noise ROM, 331 CompuVoice-Noise |
| F02-K50-26 | A#2 | ROM, 320 SD-Metal Striker ROM, 320 SD-Metal Striker |
| F02-K50-27 | B2 | ROM, 383 Explosion ROM, 27 BD-Pop |
| F02-K50-28 | C3 | ROM, 319 Cyber Drum ROM, 205 Zap 1 |
| F02-K50-29 | C#3 | ROM, 370 GunShot ROM, 370 GunShot |
| F02-K50-30 | D3 | ROM, 98 SD-Ambi Hit 1 ROM, 98 SD-Ambi Hit 1 |
| F02-K50-31 | D#3 | ROM, 231 Hand Claps-Happy |
| F02-K50-32 | E3 | ROM, 383 Explosion |
| F02-K50-33 | F3 | ROM, 219 DJ-Record Stop |
| F02-K50-34 | F#3 | ROM, 139 H.Hat-Tight 1 |
| F02-K50-35 | G3 | ROM, 219 DJ-Record Stop |
| F02-K50-36 | G#3 | ROM, 310 Rap Sleighbell ROM, 310 Rap Sleighbell |
| F02-K50-37 | A3 | ROM, 219 DJ-Record Stop |
| F02-K50-38 | A#3 | ROM, 149 H.Hat-ClangyOpen |
| F02-K50-39 | B3 | ROM, 219 DJ-Record Stop |
| F02-K50-40 | C4 | ROM, 219 DJ-Record Stop |
| F02-K50-41 | C#4 | ROM, 371 Rainstick |
| F02-K50-42 | D4 | ROM, 219 DJ-Record Stop |
| F02-K50-43 | D#4 | ROM, 164 Ride-Jazz |
| F02-K50-44 | E4 | ROM, 356 Scream |
| F02-K50-45 | F4 | ROM, 164 Ride-Jazz |
| F02-K50-46 | F#4 | ROM, 403 HarpGliss |
| F02-K50-47 | G4 | ROM, 158 Crash-Dance 99 |
| F02-K50-48 | G#4 | ROM, 310 Rap Sleighbell ROM, 310 Rap Sleighbell |
| F02-K50-49 | A4 | ROM, 378 Train |
| F02-K50-50 | A#4 | ROM, 386 Stream ROM, 385 Wind |
| F02-K50-51 | B4 | ROM, 323 Industry Loop 2 |
| F02-K50-52 | C5 | ROM, 217 DJ-BD Rub ROM, 217 DJ-BD Rub |
| F02-K50-53 | C#5 | ROM, 214 DJ-Hit Rub ROM, 214 DJ-Hit Rub |
| F02-K50-54 | D5 | ROM, 220 DJ-Reverse |
| F02-K50-55 | D#5 | ROM, 210 DJ-Scratch 3a ROM, 211 DJ-Scratch 3b |
| F02-K50-56 | E5 | ROM, 208 DJ-Scratch 1 |
| F02-K50-57 | F5 | ROM, 411 Guitar Scratch |
| F02-K50-58 | F#5 | ROM, 406 Dist.Slide 1 |
| F02-K50-59 | G5 | ROM, 384 Thunder |
| F02-K50-60 | G#5 | ROM, 385 Wind |
| F02-K50-61 | A5 | ROM, 371 Rainstick ROM, 354 Stadium |
| F02-K50-62 | A#5 | ROM, 372 Door Creak |
| F02-K50-63 | B5 | ROM, 391 Swish Terra ROM, 390 Space Lore |
| F02-K50-64 | C6 | ROM, 359 Footstep 2 ROM, 358 Footstep 1 |
| F02-K50-65 | C#6 | ROM, 335 CompuVoice-When |
| F02-K50-66 | D6 | ROM, 336 CompuVoice-Toi |
| F02-K50-67 | D#6 | ROM, 331 CompuVoice-Noise |
| F02-K50-68 | E6 | ROM, 348 PC Vox-Zee |
| F02-K50-69 | F6 | ROM, 345 PC Vox-For ROM, 343 PC Vox-Dunc |
| F02-K50-70 | F#6 | ROM, 364 Gallop |
| F02-K50-71 | G6 | ROM, 361 Bird 1 |
| F02-K50-72 | G#6 | ROM, 380 Gun Shot 2 |
| F02-K50-73 | A6 | ROM, 375 Car Stop |
| F02-K50-74 | A#6 | ROM, 372 Door Creak |
| F02-K50-75 | B6 | ROM, 382 Laser Gun |
| F02-K50-76 | C7 | ROM, 396 Noise-FM Mod |
| F02-K50-77 | C#7 | ROM, 377 Car Crash |
| F02-K50-78 | D7 | ROM, 321 SD-Air Explosion |
| F02-K50-79 | D#7 | ROM, 362 Bird 2 |
| F02-K50-80 | E7 | ROM, 170 SD-Orch.Roll 1 |
| F02-K50-81 | F7 | ROM, 372 Door Creak |
| F02-K50-82 | F#7 | ROM, 314 Bell Tree |
| F02-K50-83 | G7 | ROM, 239 Conga Hi-Slap 2 |
| F02-K50-84 | G#7 | ROM, 310 Rap Sleighbell |
| F02-K50-85 | A7 | ROM, 222 Orchestra Hit |
| F02-K50-86 | A#7 | ROM, 223 Pizz Hit |
| F02-K50-87 | B7 | ROM, 363 Dog |
| F02-K50-88 | C8 | ROM, 0 BD-Dry 1 |
| F02-K50-89 | C#8 |  |
| F02-K50-90 | D8 |  |

### F02-GM01 - STANDARD (GM kit 144, D1 p36)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM01-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM01-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM01-03 | D1 | ROM, 228 Finger Snap |
| F02-GM01-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM01-05 | E1 | ROM, 395 Noise-White |
| F02-GM01-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM01-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM01-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM01-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM01-10 | A1 | ROM, 360 Click |
| F02-GM01-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM01-12 | B1 | ROM, 16 BD-Tight |
| F02-GM01-13 | C2 | ROM, 2 BD-Dry 3 |
| F02-GM01-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM01-15 | D2 | ROM, 48 SD-Full Room |
| F02-GM01-16 | D#2 | ROM, 186 88-Claps |
| F02-GM01-17 | E2 | ROM, 48 SD-Full Room |
| F02-GM01-18 | F2 | ROM, 106 Tom 2-Floor |
| F02-GM01-19 | F#2 | ROM, 123 H.Hat 1-Open 2 |
| F02-GM01-20 | G2 | ROM, 106 Tom 2-Floor |
| F02-GM01-21 | G#2 | ROM, 127 H.Hat 2-Foot |
| F02-GM01-22 | A2 | ROM, 105 Tom 2-Lo |
| F02-GM01-23 | A#2 | ROM, 130 H.Hat 2-Open 2 |
| F02-GM01-24 | B2 | ROM, 105 Tom 2-Lo |
| F02-GM01-25 | C3 | ROM, 104 Tom 2-Hi |
| F02-GM01-26 | C#3 | ROM, 153 Crash Cymbal 2 |
| F02-GM01-27 | D3 | ROM, 104 Tom 2-Hi |
| F02-GM01-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM01-29 | E3 | ROM, 155 China Cymbal |
| F02-GM01-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM01-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM01-32 | G3 | ROM, 155 China Cymbal |
| F02-GM01-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM01-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM01-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM01-36 | B3 | ROM, 161 Ride-Dance DDD1 |
| F02-GM01-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM01-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM01-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM01-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM01-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM01-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM01-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM01-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM01-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM01-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM01-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM01-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM01-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM01-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM01-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM01-52 | D#5 | ROM, 278 Claves |
| F02-GM01-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM01-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM01-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM01-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM01-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM01-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM01-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM01-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM01-61 | C6 | ROM, 312 Marc Tree |
| F02-GM01-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM01-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM01-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM01-65 | E6 | ROM, 354 Stadium |

### F02-GM02 - ROOM (GM kit 145, D1 p36)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM02-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM02-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM02-03 | D1 | ROM, 228 Finger Snap |
| F02-GM02-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM02-05 | E1 | ROM, 395 Noise-White |
| F02-GM02-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM02-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM02-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM02-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM02-10 | A1 | ROM, 360 Click |
| F02-GM02-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM02-12 | B1 | ROM, 1 BD-Dry 2 |
| F02-GM02-13 | C2 | ROM, 2 BD-Dry 3 |
| F02-GM02-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM02-15 | D2 | ROM, 61 SD-Yowie |
| F02-GM02-16 | D#2 | ROM, 186 88-Claps |
| F02-GM02-17 | E2 | ROM, 61 SD-Yowie |
| F02-GM02-18 | F2 | ROM, 109 Tom 3-Floor |
| F02-GM02-19 | F#2 | ROM, 123 H.Hat 1-Open 2 |
| F02-GM02-20 | G2 | ROM, 109 Tom 3-Floor |
| F02-GM02-21 | G#2 | ROM, 127 H.Hat 2-Foot |
| F02-GM02-22 | A2 | ROM, 108 Tom 3-Lo |
| F02-GM02-23 | A#2 | ROM, 130 H.Hat 2-Open 2 |
| F02-GM02-24 | B2 | ROM, 108 Tom 3-Lo |
| F02-GM02-25 | C3 | ROM, 107 Tom 3-Hi |
| F02-GM02-26 | C#3 | ROM, 153 Crash Cymbal 2 |
| F02-GM02-27 | D3 | ROM, 107 Tom 3-Hi |
| F02-GM02-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM02-29 | E3 | ROM, 155 China Cymbal |
| F02-GM02-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM02-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM02-32 | G3 | ROM, 155 China Cymbal |
| F02-GM02-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM02-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM02-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM02-36 | B3 | ROM, 161 Ride-Dance DDD1 |
| F02-GM02-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM02-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM02-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM02-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM02-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM02-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM02-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM02-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM02-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM02-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM02-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM02-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM02-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM02-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM02-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM02-52 | D#5 | ROM, 278 Claves |
| F02-GM02-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM02-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM02-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM02-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM02-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM02-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM02-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM02-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM02-61 | C6 | ROM, 312 Marc Tree |
| F02-GM02-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM02-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM02-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM02-65 | E6 | ROM, 354 Stadium |

### F02-GM03 - POWER (GM kit 146, D1 p36)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM03-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM03-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM03-03 | D1 | ROM, 228 Finger Snap |
| F02-GM03-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM03-05 | E1 | ROM, 395 Noise-White |
| F02-GM03-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM03-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM03-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM03-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM03-10 | A1 | ROM, 360 Click |
| F02-GM03-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM03-12 | B1 | ROM, 15 BD-Gated |
| F02-GM03-13 | C2 | ROM, 12 BD-Mondo Kill |
| F02-GM03-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM03-15 | D2 | ROM, 60 SD-Big Rock |
| F02-GM03-16 | D#2 | ROM, 186 88-Claps |
| F02-GM03-17 | E2 | ROM, 60 SD-Big Rock |
| F02-GM03-18 | F2 | ROM, 109 Tom 3-Floor |
| F02-GM03-19 | F#2 | ROM, 123 H.Hat 1-Open 2 |
| F02-GM03-20 | G2 | ROM, 109 Tom 3-Floor |
| F02-GM03-21 | G#2 | ROM, 127 H.Hat 2-Foot |
| F02-GM03-22 | A2 | ROM, 108 Tom 3-Lo |
| F02-GM03-23 | A#2 | ROM, 130 H.Hat 2-Open 2 |
| F02-GM03-24 | B2 | ROM, 108 Tom 3-Lo |
| F02-GM03-25 | C3 | ROM, 107 Tom 3-Hi |
| F02-GM03-26 | C#3 | ROM, 153 Crash Cymbal 2 |
| F02-GM03-27 | D3 | ROM, 107 Tom 3-Hi |
| F02-GM03-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM03-29 | E3 | ROM, 155 China Cymbal |
| F02-GM03-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM03-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM03-32 | G3 | ROM, 155 China Cymbal |
| F02-GM03-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM03-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM03-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM03-36 | B3 | ROM, 161 Ride-Dance DDD1 |
| F02-GM03-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM03-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM03-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM03-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM03-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM03-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM03-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM03-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM03-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM03-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM03-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM03-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM03-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM03-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM03-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM03-52 | D#5 | ROM, 278 Claves |
| F02-GM03-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM03-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM03-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM03-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM03-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM03-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM03-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM03-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM03-61 | C6 | ROM, 312 Marc Tree |
| F02-GM03-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM03-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM03-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM03-65 | E6 | ROM, 354 Stadium |

### F02-GM04 - ELECTRONIC (GM kit 147, D1 p36)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM04-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM04-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM04-03 | D1 | ROM, 228 Finger Snap |
| F02-GM04-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM04-05 | E1 | ROM, 395 Noise-White |
| F02-GM04-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM04-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM04-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM04-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM04-10 | A1 | ROM, 360 Click |
| F02-GM04-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM04-12 | B1 | ROM, 22 BD-Hip 1 |
| F02-GM04-13 | C2 | ROM, 30 BD-Dance99 |
| F02-GM04-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM04-15 | D2 | ROM, 198 Real E.Tom |
| F02-GM04-16 | D#2 | ROM, 186 88-Claps |
| F02-GM04-17 | E2 | ROM, 98 SD-Ambi Hit 1 |
| F02-GM04-18 | F2 | ROM, 198 Real E.Tom |
| F02-GM04-19 | F#2 | ROM, 131 H.Hat 2-Sizzle |
| F02-GM04-20 | G2 | ROM, 198 Real E.Tom |
| F02-GM04-21 | G#2 | ROM, 127 H.Hat 2-Foot |
| F02-GM04-22 | A2 | ROM, 198 Real E.Tom |
| F02-GM04-23 | A#2 | ROM, 131 H.Hat 2-Sizzle |
| F02-GM04-24 | B2 | ROM, 198 Real E.Tom |
| F02-GM04-25 | C3 | ROM, 198 Real E.Tom |
| F02-GM04-26 | C#3 | ROM, 153 Crash Cymbal 2 |
| F02-GM04-27 | D3 | ROM, 198 Real E.Tom |
| F02-GM04-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM04-29 | E3 | ROM, 152 Crash Cymbal 1 |
| F02-GM04-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM04-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM04-32 | G3 | ROM, 155 China Cymbal |
| F02-GM04-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM04-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM04-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM04-36 | B3 | ROM, 161 Ride-Dance DDD1 |
| F02-GM04-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM04-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM04-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM04-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM04-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM04-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM04-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM04-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM04-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM04-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM04-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM04-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM04-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM04-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM04-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM04-52 | D#5 | ROM, 278 Claves |
| F02-GM04-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM04-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM04-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM04-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM04-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM04-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM04-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM04-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM04-61 | C6 | ROM, 312 Marc Tree |
| F02-GM04-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM04-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM04-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM04-65 | E6 | ROM, 354 Stadium |

### F02-GM05 - ANALOG (GM kit 148, D1 p36)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM05-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM05-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM05-03 | D1 | ROM, 228 Finger Snap |
| F02-GM05-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM05-05 | E1 | ROM, 395 Noise-White |
| F02-GM05-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM05-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM05-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM05-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM05-10 | A1 | ROM, 360 Click |
| F02-GM05-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM05-12 | B1 | ROM, 174 88-BD 2(R&B) |
| F02-GM05-13 | C2 | ROM, 176 99-BD 1 |
| F02-GM05-14 | C#2 | ROM, 185 88-Rimshot |
| F02-GM05-15 | D2 | ROM, 181 88-SD 1(New) |
| F02-GM05-16 | D#2 | ROM, 186 88-Claps |
| F02-GM05-17 | E2 | ROM, 182 88-SD 2(Old) |
| F02-GM05-18 | F2 | ROM, 192 88-Tom |
| F02-GM05-19 | F#2 | ROM, 188 88-HHat Open |
| F02-GM05-20 | G2 | ROM, 192 88-Tom |
| F02-GM05-21 | G#2 | ROM, 188 88-HHat Open |
| F02-GM05-22 | A2 | ROM, 192 88-Tom |
| F02-GM05-23 | A#2 | ROM, 188 88-HHat Open |
| F02-GM05-24 | B2 | ROM, 192 88-Tom |
| F02-GM05-25 | C3 | ROM, 192 88-Tom |
| F02-GM05-26 | C#3 | ROM, 160 Ride-Dance 99 |
| F02-GM05-27 | D3 | ROM, 192 88-Tom |
| F02-GM05-28 | D#3 | ROM, 191 88-Crash |
| F02-GM05-29 | E3 | ROM, 155 China Cymbal |
| F02-GM05-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM05-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM05-32 | G3 | ROM, 155 China Cymbal |
| F02-GM05-33 | G#3 | ROM, 195 88-Cowbell |
| F02-GM05-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM05-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM05-36 | B3 | ROM, 161 Ride-Dance DDD1 |
| F02-GM05-37 | C4 | ROM, 193 88-Conga |
| F02-GM05-38 | C#4 | ROM, 193 88-Conga |
| F02-GM05-39 | D4 | ROM, 192 88-Tom |
| F02-GM05-40 | D#4 | ROM, 192 88-Tom |
| F02-GM05-41 | E4 | ROM, 192 88-Tom |
| F02-GM05-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM05-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM05-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM05-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM05-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM05-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM05-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM05-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM05-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM05-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM05-52 | D#5 | ROM, 278 Claves |
| F02-GM05-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM05-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM05-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM05-56 | G5 | ROM, 295 Cuica-Hi |
| F02-GM05-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM05-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM05-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM05-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM05-61 | C6 | ROM, 312 Marc Tree |
| F02-GM05-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM05-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM05-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM05-65 | E6 | ROM, 354 Stadium |

### F02-GM06 - JAZZ (GM kit 149, D1 p37)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM06-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM06-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM06-03 | D1 | ROM, 228 Finger Snap |
| F02-GM06-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM06-05 | E1 | ROM, 395 Noise-White |
| F02-GM06-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM06-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM06-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM06-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM06-10 | A1 | ROM, 360 Click |
| F02-GM06-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM06-12 | B1 | ROM, 3 BD-Dry 4 |
| F02-GM06-13 | C2 | ROM, 9 BD-Jazz 2 |
| F02-GM06-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM06-15 | D2 | ROM, 53 SD-Paper |
| F02-GM06-16 | D#2 | ROM, 186 88-Claps |
| F02-GM06-17 | E2 | ROM, 50 SD-Jazz Ring |
| F02-GM06-18 | F2 | ROM, 113 Tom-Jazz Floor |
| F02-GM06-19 | F#2 | ROM, 118 H.Hat 1-Closed 1 |
| F02-GM06-20 | G2 | ROM, 113 Tom-Jazz Floor |
| F02-GM06-21 | G#2 | ROM, 120 H.Hat 1-Foot |
| F02-GM06-22 | A2 | ROM, 111 Tom-Jazz Hi |
| F02-GM06-23 | A#2 | ROM, 122 H.Hat 1-Open 1 |
| F02-GM06-24 | B2 | ROM, 111 Tom-Jazz Hi |
| F02-GM06-25 | C3 | ROM, 111 Tom-Jazz Hi |
| F02-GM06-26 | C#3 | ROM, 152 Crash Cymbal 1 |
| F02-GM06-27 | D3 | ROM, 111 Tom-Jazz Hi |
| F02-GM06-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM06-29 | E3 | ROM, 155 China Cymbal |
| F02-GM06-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM06-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM06-32 | G3 | ROM, 155 China Cymbal |
| F02-GM06-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM06-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM06-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM06-36 | B3 | ROM, 163 Ride-Edge 2 |
| F02-GM06-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM06-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM06-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM06-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM06-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM06-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM06-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM06-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM06-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM06-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM06-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM06-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM06-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM06-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM06-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM06-52 | D#5 | ROM, 278 Claves |
| F02-GM06-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM06-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM06-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM06-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM06-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM06-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM06-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM06-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM06-61 | C6 | ROM, 312 Marc Tree |
| F02-GM06-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM06-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM06-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM06-65 | E6 | ROM, 355 Applause |

### F02-GM07 - BRUSH (GM kit 150, D1 p37)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM07-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM07-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM07-03 | D1 | ROM, 228 Finger Snap |
| F02-GM07-04 | D#1 | ROM, 206 Zap 2 |
| F02-GM07-05 | E1 | ROM, 395 Noise-White |
| F02-GM07-06 | F1 | ROM, 209 DJ-Scratch 2 |
| F02-GM07-07 | F#1 | ROM, 209 DJ-Scratch 2 |
| F02-GM07-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM07-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM07-10 | A1 | ROM, 360 Click |
| F02-GM07-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM07-12 | B1 | ROM, 3 BD-Dry 4 |
| F02-GM07-13 | C2 | ROM, 9 BD-Jazz 2 |
| F02-GM07-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM07-15 | D2 | ROM, 56 SD-Brush Tap 1 |
| F02-GM07-16 | D#2 | ROM, 55 SD-Brush Hit |
| F02-GM07-17 | E2 | ROM, 58 SD-Brush Swirl |
| F02-GM07-18 | F2 | ROM, 113 Tom-Jazz Floor |
| F02-GM07-19 | F#2 | ROM, 118 H.Hat 1-Closed 1 |
| F02-GM07-20 | G2 | ROM, 113 Tom-Jazz Floor |
| F02-GM07-21 | G#2 | ROM, 120 H.Hat 1-Foot |
| F02-GM07-22 | A2 | ROM, 111 Tom-Jazz Hi |
| F02-GM07-23 | A#2 | ROM, 122 H.Hat 1-Open 1 |
| F02-GM07-24 | B2 | ROM, 111 Tom-Jazz Hi |
| F02-GM07-25 | C3 | ROM, 111 Tom-Jazz Hi |
| F02-GM07-26 | C#3 | ROM, 152 Crash Cymbal 1 |
| F02-GM07-27 | D3 | ROM, 111 Tom-Jazz Hi |
| F02-GM07-28 | D#3 | ROM, 162 Ride-Edge 1 |
| F02-GM07-29 | E3 | ROM, 155 China Cymbal |
| F02-GM07-30 | F3 | ROM, 164 Ride-Jazz |
| F02-GM07-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM07-32 | G3 | ROM, 155 China Cymbal |
| F02-GM07-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM07-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM07-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM07-36 | B3 | ROM, 163 Ride-Edge 2 |
| F02-GM07-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM07-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM07-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM07-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM07-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM07-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM07-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM07-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM07-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM07-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM07-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM07-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM07-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM07-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM07-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM07-52 | D#5 | ROM, 278 Claves |
| F02-GM07-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM07-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM07-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM07-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM07-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM07-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM07-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM07-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM07-61 | C6 | ROM, 312 Marc Tree |
| F02-GM07-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM07-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM07-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM07-65 | E6 | ROM, 355 Applause |

### F02-GM08 - ORCHESTRA (GM kit 151, D1 p37)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM08-01 | C1 | ROM, 172 SD-Orchestral |
| F02-GM08-02 | C#1 | ROM, 171 SD-Orch.Roll 2 |
| F02-GM08-03 | D1 | ROM, 228 Finger Snap |
| F02-GM08-04 | D#1 | ROM, 118 H.Hat 1-Closed 1 |
| F02-GM08-05 | E1 | ROM, 120 H.Hat 1-Foot |
| F02-GM08-06 | F1 | ROM, 122 H.Hat 1-Open 1 |
| F02-GM08-07 | F#1 | ROM, 162 Ride-Edge 1 |
| F02-GM08-08 | G1 | ROM, 227 Drumstick Hit |
| F02-GM08-09 | G#1 | ROM, 205 Zap 1 |
| F02-GM08-10 | A1 | ROM, 360 Click |
| F02-GM08-11 | A#1 | ROM, 292 Triangle-Open |
| F02-GM08-12 | B1 | ROM, 7 BD-Soft Room |
| F02-GM08-13 | C2 | ROM, 166 BD-Orchestral |
| F02-GM08-14 | C#2 | ROM, 66 Sidestick-Amb |
| F02-GM08-15 | D2 | ROM, 172 SD-Orchestral |
| F02-GM08-16 | D#2 | ROM, 252 Castanet-Single |
| F02-GM08-17 | E2 | ROM, 172 SD-Orchestral |
| F02-GM08-18 | F2 | ROM, 169 Timpani |
| F02-GM08-19 | F#2 | ROM, 169 Timpani |
| F02-GM08-20 | G2 | ROM, 169 Timpani |
| F02-GM08-21 | G#2 | ROM, 169 Timpani |
| F02-GM08-22 | A2 | ROM, 169 Timpani |
| F02-GM08-23 | A#2 | ROM, 169 Timpani |
| F02-GM08-24 | B2 | ROM, 169 Timpani |
| F02-GM08-25 | C3 | ROM, 169 Timpani |
| F02-GM08-26 | C#3 | ROM, 169 Timpani |
| F02-GM08-27 | D3 | ROM, 169 Timpani |
| F02-GM08-28 | D#3 | ROM, 169 Timpani |
| F02-GM08-29 | E3 | ROM, 169 Timpani |
| F02-GM08-30 | F3 | ROM, 169 Timpani |
| F02-GM08-31 | F#3 | ROM, 291 Tambourine-Acc.2 |
| F02-GM08-32 | G3 | ROM, 155 China Cymbal |
| F02-GM08-33 | G#3 | ROM, 306 Cowbell 2 |
| F02-GM08-34 | A3 | ROM, 153 Crash Cymbal 2 |
| F02-GM08-35 | A#3 | ROM, 277 Vibraslap |
| F02-GM08-36 | B3 | ROM, 168 Orch Cym.-Open |
| F02-GM08-37 | C4 | ROM, 245 Bongo Hi-Open |
| F02-GM08-38 | C#4 | ROM, 242 Bongo Lo-Open |
| F02-GM08-39 | D4 | ROM, 237 Conga Hi-MtSlap |
| F02-GM08-40 | D#4 | ROM, 235 Conga Hi-Open |
| F02-GM08-41 | E4 | ROM, 232 Conga Lo-Open |
| F02-GM08-42 | F4 | ROM, 286 Timbale Hi-Rim 2 |
| F02-GM08-43 | F#4 | ROM, 281 Timbale Lo-Open |
| F02-GM08-44 | G4 | ROM, 304 Agogo Bell |
| F02-GM08-45 | G#4 | ROM, 304 Agogo Bell |
| F02-GM08-46 | A4 | ROM, 299 Cabasa-Up |
| F02-GM08-47 | A#4 | ROM, 258 Maracas-Push |
| F02-GM08-48 | B4 | ROM, 316 Samba Whistle |
| F02-GM08-49 | C5 | ROM, 316 Samba Whistle |
| F02-GM08-50 | C#5 | ROM, 256 Guiro-Short |
| F02-GM08-51 | D5 | ROM, 255 Guiro-Long |
| F02-GM08-52 | D#5 | ROM, 278 Claves |
| F02-GM08-53 | E5 | ROM, 279 Woodblock 1 |
| F02-GM08-54 | F5 | ROM, 279 Woodblock 1 |
| F02-GM08-55 | F#5 | ROM, 295 Cuica-Hi |
| F02-GM08-56 | G5 | ROM, 296 Cuica-Lo |
| F02-GM08-57 | G#5 | ROM, 293 Triangle-Mute |
| F02-GM08-58 | A5 | ROM, 292 Triangle-Open |
| F02-GM08-59 | A#5 | ROM, 300 Cabasa-Down |
| F02-GM08-60 | B5 | ROM, 309 Sleigh Bell |
| F02-GM08-61 | C6 | ROM, 312 Marc Tree |
| F02-GM08-62 | C#6 | ROM, 252 Castanet-Single |
| F02-GM08-63 | D6 | ROM, 282 Timbale Lo-Mute |
| F02-GM08-64 | D#6 | ROM, 281 Timbale Lo-Open |
| F02-GM08-65 | E6 | ROM, 355 Applause |

### F02-GM09 - SFX (GM kit 152, D1 p37)

| Shipped label | Key | Documented assignment |
|---|---|---|
| F02-GM09-01 | C1 |  |
| F02-GM09-02 | C#1 |  |
| F02-GM09-03 | D1 |  |
| F02-GM09-04 | D#1 |  |
| F02-GM09-05 | E1 |  |
| F02-GM09-06 | F1 |  |
| F02-GM09-07 | F#1 |  |
| F02-GM09-08 | G1 |  |
| F02-GM09-09 | G#1 |  |
| F02-GM09-10 | A1 |  |
| F02-GM09-11 | A#1 |  |
| F02-GM09-12 | B1 |  |
| F02-GM09-13 | C2 |  |
| F02-GM09-14 | C#2 |  |
| F02-GM09-15 | D2 |  |
| F02-GM09-16 | D#2 | ROM, 206 Zap 2 |
| F02-GM09-17 | E2 | ROM, 370 GunShot |
| F02-GM09-18 | F2 | ROM, 208 DJ-Scratch 1 |
| F02-GM09-19 | F#2 | ROM, 209 DJ-Scratch 2 |
| F02-GM09-20 | G2 | ROM, 227 Drumstick Hit |
| F02-GM09-21 | G#2 | ROM, 228 Finger Snap |
| F02-GM09-22 | A2 | ROM, 360 Click |
| F02-GM09-23 | A#2 | ROM, 397 Tubular |
| F02-GM09-24 | B2 | ROM, 405 Fret Noise |
| F02-GM09-25 | C3 | ROM, 400 GtCutNois1 |
| F02-GM09-26 | C#3 | ROM, 401 GtCutNois2 |
| F02-GM09-27 | D3 | ROM, 402 StringSlap |
| F02-GM09-28 | D#3 | ROM, 228 Finger Snap |
| F02-GM09-29 | E3 | ROM, 357 Laughing |
| F02-GM09-30 | F3 | ROM, 356 Scream |
| F02-GM09-31 | F#3 | ROM, 368 Punch |
| F02-GM09-32 | G3 | ROM, 367 Hart Beat |
| F02-GM09-33 | G#3 | ROM, 359 Footstep 2 |
| F02-GM09-34 | A3 | ROM, 358 Footstep 1 |
| F02-GM09-35 | A#3 | ROM, 355 Applause |
| F02-GM09-36 | B3 | ROM, 372 Door Creak |
| F02-GM09-37 | C4 | ROM, 373 Door Slam |
| F02-GM09-38 | C#4 | ROM, 209 DJ-Scratch 2 |
| F02-GM09-39 | D4 | ROM, 312 Marc Tree |
| F02-GM09-40 | D#4 | ROM, 374 Car Engine |
| F02-GM09-41 | E4 | ROM, 375 Car Stop |
| F02-GM09-42 | F4 | ROM, 376 Car Pass |
| F02-GM09-43 | F#4 | ROM, 377 Car Crash |
| F02-GM09-44 | G4 | ROM, 366 Crickets 2 |
| F02-GM09-45 | G#4 | ROM, 378 Train |
| F02-GM09-46 | A4 | ROM, 395 Noise-White |
| F02-GM09-47 | A#4 | ROM, 379 Helicopter |
| F02-GM09-48 | B4 | ROM, 391 Swish Terra |
| F02-GM09-49 | C5 | ROM, 380 Gun Shot 2 |
| F02-GM09-50 | C#5 | ROM, 381 MachineGun |
| F02-GM09-51 | D5 | ROM, 382 Laser Gun |
| F02-GM09-52 | D#5 | ROM, 383 Explosion |
| F02-GM09-53 | E5 | ROM, 363 Dog |
| F02-GM09-54 | F5 | ROM, 364 Gallop |
| F02-GM09-55 | F#5 | ROM, 361 Bird 1 |
| F02-GM09-56 | G5 | ROM, 371 Rainstick |
| F02-GM09-57 | G#5 | ROM, 384 Thunder |
| F02-GM09-58 | A5 | ROM, 385 Wind |
| F02-GM09-59 | A#5 | ROM, 395 Noise-White |
| F02-GM09-60 | B5 | ROM, 386 Stream |
| F02-GM09-61 | C6 | ROM, 387 Bubble |
| F02-GM09-62 | C#6 |  |
| F02-GM09-63 | D6 |  |
| F02-GM09-64 | D#6 |  |
| F02-GM09-65 | E6 |  |

