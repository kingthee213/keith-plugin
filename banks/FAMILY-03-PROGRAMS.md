# FAMILY 03 - factory programs and combinations (internal reference)

Reference machine (internal name, never shipped): **Korg M1**.
Shipped label: **Family 03**.

Rows were read from page images of the factory Voice Name List (the scans have no text layer) and cross-checked against an independent machine OCR of the same pages. The PDFs and page images are not copied into the repository.

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

| Ref | Document | Where it came from | URL | What it supplies |
|---|---|---|---|---|
| D1 | M1 Voice Name List (Korg, 4-page factory name list; scan transcribed from page images) | archive.org item korg-m-1-voice.name.list (scanned copy of the Korg factory name list) | https://archive.org/details/korg-m-1-voice.name.list | 100 factory programs and 100 factory combinations |
| D2 | M1 Owner's Manual (Korg, archive.org scan with OCR text layer) | archive.org item synthmanual_M1_owners_manual | https://archive.org/details/synthmanual_M1_owners_manual | effect types 1-33 with their names, effect routing (2 effect systems, serial or parallel, 4 inputs A-D, 4 outputs 1/L 2/R 3 4), instrument and drum-kit parameters, specifications |

## Counts as documented

| Section | Count |
|---|---|
| Factory programs | 100 |
| Factory combinations | 100 |

- Programs: **100**; combinations: **100**. D2's specification page states 100 programs and 100 combinations, which matches the name list.

## Programs

Numbering is the document's own 00-99 grid.

| Shipped label | Documented name | Number | Source | Note |
|---|---|---|---|---|
| F03-P0001 | Univers | 00 | D1 p1 |  |
| F03-P0002 | Piano 16' | 01 | D1 p1 |  |
| F03-P0003 | Brass 1 | 02 | D1 p1 |  |
| F03-P0004 | Ooh/Ahh | 03 | D1 p1 |  |
| F03-P0005 | Guitar 1 | 04 | D1 p1 |  |
| F03-P0006 | BottleBell | 05 | D1 p1 |  |
| F03-P0007 | Fretless | 06 | D1 p1 |  |
| F03-P0008 | Symphonic | 07 | D1 p1 |  |
| F03-P0009 | Pan Flute | 08 | D1 p1 |  |
| F03-P0010 | Drums #1 | 09 | D1 p1 |  |
| F03-P0011 | PanMallet | 10 | D1 p1 |  |
| F03-P0012 | E.Piano 1 | 11 | D1 p1 |  |
| F03-P0013 | Trumpet | 12 | D1 p1 |  |
| F03-P0014 | Nimbus | 13 | D1 p1 |  |
| F03-P0015 | DistGuitar | 14 | D1 p1 |  |
| F03-P0016 | Vibes | 15 | D1 p1 |  |
| F03-P0017 | Pick Bass | 16 | D1 p1 |  |
| F03-P0018 | Organ 2 | 17 | D1 p1 |  |
| F03-P0019 | Flute | 18 | D1 p1 |  |
| F03-P0020 | Pole | 19 | D1 p1 |  |
| F03-P0021 | Dream Pad | 20 | D1 p1 |  |
| F03-P0022 | MagicPiano | 21 | D1 p1 |  |
| F03-P0023 | Solo Sax | 22 | D1 p1 |  |
| F03-P0024 | Choir | 23 | D1 p1 |  |
| F03-P0025 | 12-String | 24 | D1 p1 |  |
| F03-P0026 | Kalimba | 25 | D1 p1 |  |
| F03-P0027 | A.Bass | 26 | D1 p1 |  |
| F03-P0028 | Strings | 27 | D1 p1 |  |
| F03-P0029 | SynMallet | 28 | D1 p1 |  |
| F03-P0030 | Drums #2 | 29 | D1 p1 |  |
| F03-P0031 | Lore | 30 | D1 p1 |  |
| F03-P0032 | Harpsicord | 31 | D1 p1 |  |
| F03-P0033 | DoubleReed | 32 | D1 p1 |  |
| F03-P0034 | Bottles | 33 | D1 p1 |  |
| F03-P0035 | Koto Trem | 34 | D1 p1 |  |
| F03-P0036 | Bell Ring | 35 | D1 p1 |  |
| F03-P0037 | SynthBass1 | 36 | D1 p1 |  |
| F03-P0038 | Timp&Bells | 37 | D1 p1 |  |
| F03-P0039 | Solo Synth | 38 | D1 p1 |  |
| F03-P0040 | Pop | 39 | D1 p1 |  |
| F03-P0041 | Magician | 40 | D1 p1 |  |
| F03-P0042 | Piano 8' | 41 | D1 p1 |  |
| F03-P0043 | Overture | 42 | D1 p1 |  |
| F03-P0044 | Angels | 43 | D1 p1 |  |
| F03-P0045 | Sitar 1 | 44 | D1 p1 |  |
| F03-P0046 | Tubular | 45 | D1 p1 |  |
| F03-P0047 | Slap Bass | 46 | D1 p1 |  |
| F03-P0048 | Pipe Organ | 47 | D1 p1 |  |
| F03-P0049 | Wire | 48 | D1 p1 |  |
| F03-P0050 | Drums #3 | 49 | D1 p1 |  |
| F03-P0051 | Bambu Trem | 50 | D1 p1 |  |
| F03-P0052 | E.Piano 4 | 51 | D1 p1 |  |
| F03-P0053 | TubaFlugel | 52 | D1 p1 |  |
| F03-P0054 | Voice Wave | 53 | D1 p1 |  |
| F03-P0055 | Guitar 2 | 54 | D1 p1 |  |
| F03-P0056 | Metal Hit | 55 | D1 p1 |  |
| F03-P0057 | SynthBass2 | 56 | D1 p1 |  |
| F03-P0058 | StringRise | 57 | D1 p1 |  |
| F03-P0059 | Pan Wave | 58 | D1 p1 |  |
| F03-P0060 | Hammer | 59 | D1 p1 |  |
| F03-P0061 | Cloud Nine | 60 | D1 p1 |  |
| F03-P0062 | Clav | 61 | D1 p1 |  |
| F03-P0063 | TENOR SAX | 62 | D1 p1 |  |
| F03-P0064 | Voices | 63 | D1 p1 |  |
| F03-P0065 | RockGuitar | 64 | D1 p1 |  |
| F03-P0066 | WindBells | 65 | D1 p1 |  |
| F03-P0067 | SynthBass3 | 66 | D1 p1 |  |
| F03-P0068 | Organ 1 | 67 | D1 p1 |  |
| F03-P0069 | Block | 68 | D1 p1 |  |
| F03-P0070 | FingerSnap | 69 | D1 p1 |  |
| F03-P0071 | MagicOrgan | 70 | D1 p1 |  |
| F03-P0072 | E.Piano 2 | 71 | D1 p1 |  |
| F03-P0073 | Brass 2 | 72 | D1 p1 |  |
| F03-P0074 | FV Wave | 73 | D1 p1 |  |
| F03-P0075 | PickGuitar | 74 | D1 p1 |  |
| F03-P0076 | Digi-Bells | 75 | D1 p1 |  |
| F03-P0077 | AnalogBass | 76 | D1 p1 |  |
| F03-P0078 | Ping Wave | 77 | D1 p1 |  |
| F03-P0079 | Vibe Hit | 78 | D1 p1 |  |
| F03-P0080 | Pluck | 79 | D1 p1 |  |
| F03-P0081 | Good & Bad | 80 | D1 p1 |  |
| F03-P0082 | Digital 2 | 81 | D1 p1 |  |
| F03-P0083 | Mute Trp. | 82 | D1 p1 |  |
| F03-P0084 | Stratos | 83 | D1 p1 |  |
| F03-P0085 | Sitar 2 | 84 | D1 p1 |  |
| F03-P0086 | Flexatone | 85 | D1 p1 |  |
| F03-P0087 | Digital 4 | 86 | D1 p1 |  |
| F03-P0088 | Soft Horns | 87 | D1 p1 |  |
| F03-P0089 | HellsBells | 88 | D1 p1 |  |
| F03-P0090 | Drop | 89 | D1 p1 |  |
| F03-P0091 | Zephyr | 90 | D1 p1 |  |
| F03-P0092 | E.Piano 3 | 91 | D1 p1 |  |
| F03-P0093 | SynthBrass | 92 | D1 p1 |  |
| F03-P0094 | Digital 5 | 93 | D1 p1 |  |
| F03-P0095 | E.Guitar 1 | 94 | D1 p1 |  |
| F03-P0096 | Rhythm | 95 | D1 p1 |  |
| F03-P0097 | Mono Synth | 96 | D1 p1 |  |
| F03-P0098 | Hold ...... | 97 | D1 p1 |  |
| F03-P0099 | Wait ...... | 98 | D1 p1 |  |
| F03-P0100 | Surprise!! | 99 | D1 p1 | name printed with marker characters; recorded exactly as printed |

## Combinations

Numbering is the document's own 00-99 grid. The document also prints the programs each combination is built from; that timbre list is in the transcription file outside the repository and is not repeated here.

| Shipped label | Documented name | Number | Source | Note |
|---|---|---|---|---|
| F03-C0001 | FilmScore | 00 | D1 p1-p3 |  |
| F03-C0002 | Pankala | 01 | D1 p1-p3 |  |
| F03-C0003 | Rondo' | 02 | D1 p1-p3 |  |
| F03-C0004 | Brass 1&2 | 03 | D1 p1-p3 |  |
| F03-C0005 | Fuji-san | 04 | D1 p1-p3 |  |
| F03-C0006 | 1-Man-Band | 05 | D1 p1-p3 |  |
| F03-C0007 | Orchestral | 06 | D1 p1-p3 |  |
| F03-C0008 | 12 String | 07 | D1 p1-p3 |  |
| F03-C0009 | Pyramids | 08 | D1 p1-p3 |  |
| F03-C0010 | Bass&Piano | 09 | D1 p1-p3 |  |
| F03-C0011 | MIDIStack1 | 10 | D1 p1-p3 |  |
| F03-C0012 | BellVoices | 11 | D1 p1-p3 |  |
| F03-C0013 | Ensemble 1 | 12 | D1 p1-p3 |  |
| F03-C0014 | SunSection | 13 | D1 p1-p3 |  |
| F03-C0015 | Atrantis | 14 | D1 p1-p3 |  |
| F03-C0016 | WeatherMan | 15 | D1 p1-p3 |  |
| F03-C0017 | Orchestra2 | 16 | D1 p1-p3 |  |
| F03-C0018 | Perc-Organ | 17 | D1 p1-p3 |  |
| F03-C0019 | RhythmLore | 18 | D1 p1-p3 |  |
| F03-C0020 | Bass&Brass | 19 | D1 p1-p3 |  |
| F03-C0021 | Christmas | 20 | D1 p1-p3 |  |
| F03-C0022 | Air Mallet | 21 | D1 p1-p3 |  |
| F03-C0023 | Baroque | 22 | D1 p1-p3 |  |
| F03-C0024 | DynaFusion | 23 | D1 p1-p3 |  |
| F03-C0025 | Montezuma | 24 | D1 p1-p3 |  |
| F03-C0026 | Power Play | 25 | D1 p1-p3 |  |
| F03-C0027 | Orchestra3 | 26 | D1 p1-p3 |  |
| F03-C0028 | Clik Piano | 27 | D1 p1-p3 |  |
| F03-C0029 | Clock Shop | 28 | D1 p1-p3 |  |
| F03-C0030 | Bass&Vibe | 29 | D1 p1-p3 |  |
| F03-C0031 | Westerns | 30 | D1 p1-p3 |  |
| F03-C0032 | Breathy | 31 | D1 p1-p3 |  |
| F03-C0033 | Strings | 32 | D1 p1-p3 |  |
| F03-C0034 | Big City | 33 | D1 p1-p3 |  |
| F03-C0035 | Venice | 34 | D1 p1-p3 |  |
| F03-C0036 | MultiSound | 35 | D1 p1-p3 |  |
| F03-C0037 | Orchestra4 | 36 | D1 p1-p3 |  |
| F03-C0038 | MultiBass | 37 | D1 p1-p3 |  |
| F03-C0039 | PastTime | 38 | D1 p1-p3 |  |
| F03-C0040 | Bass&Reed | 39 | D1 p1-p3 |  |
| F03-C0041 | MIDIStack2 | 40 | D1 p1-p3 |  |
| F03-C0042 | VoiceChoir | 41 | D1 p1-p3 |  |
| F03-C0043 | String Mix | 42 | D1 p1-p3 |  |
| F03-C0044 | SuperBrass | 43 | D1 p1-p3 |  |
| F03-C0045 | Bombay | 44 | D1 p1-p3 |  |
| F03-C0046 | Acoustic | 45 | D1 p1-p3 |  |
| F03-C0047 | Wind 5th | 46 | D1 p1-p3 |  |
| F03-C0048 | Trumpet&FH | 47 | D1 p1-p3 |  |
| F03-C0049 | Caverns | 48 | D1 p1-p3 |  |
| F03-C0050 | Bass&Sax | 49 | D1 p1-p3 |  |
| F03-C0051 | PianoVoice | 50 | D1 p1-p3 |  |
| F03-C0052 | VoiceSlaps | 51 | D1 p1-p3 |  |
| F03-C0053 | 5 Strings | 52 | D1 p1-p3 |  |
| F03-C0054 | TechnoFunk | 53 | D1 p1-p3 |  |
| F03-C0055 | Jamaica | 54 | D1 p1-p3 |  |
| F03-C0056 | Club Date | 55 | D1 p1-p3 |  |
| F03-C0057 | Brass-Orch | 56 | D1 p1-p3 |  |
| F03-C0058 | ElecGuitar | 57 | D1 p1-p3 |  |
| F03-C0059 | Gothica | 58 | D1 p1-p3 |  |
| F03-C0060 | Bass&Horn | 59 | D1 p1-p3 |  |
| F03-C0061 | SaintPeter | 60 | D1 p1-p3 |  |
| F03-C0062 | Celestial | 61 | D1 p1-p3 |  |
| F03-C0063 | StringBell | 62 | D1 p1-p3 |  |
| F03-C0064 | Madness | 63 | D1 p1-p3 |  |
| F03-C0065 | China | 64 | D1 p1-p3 |  |
| F03-C0066 | LayerPad | 65 | D1 p1-p3 |  |
| F03-C0067 | The Hunter | 66 | D1 p1-p3 |  |
| F03-C0068 | TP&Sax | 67 | D1 p1-p3 |  |
| F03-C0069 | DeepHeart | 68 | D1 p1-p3 |  |
| F03-C0070 | Sax & Orch | 69 | D1 p1-p3 |  |
| F03-C0071 | Bambu Bell | 70 | D1 p1-p3 |  |
| F03-C0072 | AirHorns | 71 | D1 p1-p3 |  |
| F03-C0073 | Music Box | 72 | D1 p1-p3 |  |
| F03-C0074 | Electric | 73 | D1 p1-p3 |  |
| F03-C0075 | Animation | 74 | D1 p1-p3 |  |
| F03-C0076 | DirtyBrass | 75 | D1 p1-p3 |  |
| F03-C0077 | Str&Piano | 76 | D1 p1-p3 |  |
| F03-C0078 | SaxSection | 77 | D1 p1-p3 |  |
| F03-C0079 | DigitalBox | 78 | D1 p1-p3 |  |
| F03-C0080 | Piano&EP1 | 79 | D1 p1-p3 |  |
| F03-C0081 | ThePlanets | 80 | D1 p1-p3 |  |
| F03-C0082 | Barbarians | 81 | D1 p1-p3 |  |
| F03-C0083 | Atomosphere | 82 | D1 p1-p3 |  |
| F03-C0084 | Metal Clav | 83 | D1 p1-p3 |  |
| F03-C0085 | HammerHead | 84 | D1 p1-p3 |  |
| F03-C0086 | Nuts&Bolts | 85 | D1 p1-p3 |  |
| F03-C0087 | OrganChoir | 86 | D1 p1-p3 |  |
| F03-C0088 | Piano(L+R) | 87 | D1 p1-p3 |  |
| F03-C0089 | Space Race | 88 | D1 p1-p3 |  |
| F03-C0090 | LUNA-PAD | 89 | D1 p1-p3 |  |
| F03-C0091 | Beauty | 90 | D1 p1-p3 |  |
| F03-C0092 | WaveMallet | 91 | D1 p1-p3 |  |
| F03-C0093 | Mallets | 92 | D1 p1-p3 |  |
| F03-C0094 | PluckOrgan | 93 | D1 p1-p3 |  |
| F03-C0095 | E.PianoMix | 94 | D1 p1-p3 |  |
| F03-C0096 | SuperSynth | 95 | D1 p1-p3 |  |
| F03-C0097 | Passages | 96 | D1 p1-p3 |  |
| F03-C0098 | OctaveBass | 97 | D1 p1-p3 |  |
| F03-C0099 | Sub-Space | 98 | D1 p1-p3 |  |
| F03-C0100 | Please→→→→ | 99 | D1 p1-p3 |  |

## Disputed spellings in the scan

The Voice Name List is a scan with no text layer, so two spellings were read two different ways. Both are recorded here with the evidence, and the family file keeps the better-supported reading.

| Number | Reading kept | Other reading | Evidence |
|---|---|---|---|
| 31 (program) | `Harpsicord` | `Harpsichord` | tesseract reads `Harpsicord` at all five occurrences, the full-page reads agree, and the printed word's width matches the 10-character column; two zoomed reads said `Harpsichord`, but the letters touch in the scan so they could not be split into glyphs to settle it. Kept as printed by the majority of readings. |
| 82 (combination) | `Atomosphere` | `Atmosphere` | splitting the printed word into its letter bitmaps shows a hollow oval then a three-stem glyph (A-t-o-m-o-s-p-h-e-r-e), and tesseract reads `Atomosphere`; the document appears to carry the typo. |

## Factory FX algorithms and routing

**Found in documentation: yes** (D2). The instrument has two effect systems plus two panpots, with four inputs (A, B, C, D) and four outputs (1/L, 2/R, 3, 4). The two effects can be placed in **series or in parallel**; all routing is digital inside the instrument. D2 states that effect types **1-25 are stereo effects and 26-33 are dual effects**, in which each channel carries a different effect - 33 types in total.

**Per-program FX assignments are not documented.** No public document states which effect type and parameter values each factory program uses; that is only in the instrument's factory data, which is out of bounds here.

### Effect types as printed in D2

| # | Effect type as printed |
|---|---|
| 1 | HALL |
| 2 | ENSEMBLE HALL |
| 3 | CONCERT HALL |
| 4 | ROOM |
| 5 | LARGE ROOM |
| 6 | LIVE STAGE |
| 7 | EARLY REFLECTION I |
| 8 | EARLY REFLECTION II |
| 9 | EARLY REFLECTION III |
| 10 | STEREO DELAY |
| 11 | CROSS DELAY |
| 12 | STEREO CHORUS I |
| 13 | STEREO CHORUS II |
| 14 | STEREO FLANGER |
| 15 | CROSS FLANGER |
| 16 | PHASER I |
| 17 | PHASER II |
| 18 | STEREO TREMOLO I |
| 19 | STEREO TREMOLO II |
| 20 | EQUALIZER |
| 21 | OVERDRIVE |
| 22 | DISTORTION |
| 23 | EXCITER |
| 24 | SYMPHONIC ENSEMBLE |
| 25 | ROTARY SPEAKER |
| 26 | DELAY/HALL |
| 27 | DELAY/ROOM |
| 28 | DELAY/EARLY REFLECTION |
| 29 | DELAY/DELAY |
| 30 | DELAY/CHORUS |
| 31 | DELAY/FLANGER |
| 32 | DELAY/PHASER |
| 33 | DELAY/TREMOLO |

