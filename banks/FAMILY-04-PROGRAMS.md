# FAMILY 04 - factory patches, performances and waves (internal reference)

Reference machine (internal name, never shipped): **Korg Wavestation**.
Shipped label: **Family 04**.

Every row below was read from scanned pages of the official Voice Name List: the pages have no text layer, so they were transcribed from the page images by a vision pass and independently re-read from per-column crops of the same pages. Rows where the two passes disagreed are listed with a note. Machine-readable copies of the transcription live outside the repository (`scratch/u2/ocr_ws_lists.tsv`); the page scans are not copied into the repository.

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
| D1 | WAVESTATION Voice Name List (Korg, file WSVoiceNList.pdf, 6 pages, scanned - no text layer) | official Korg download page, product WAVESTATION, document id 1564 | https://www.korg.com/us/support/download/manual/1/224/1564/ | waves, wave sequences, patches (programs) and performances |
| D2 | WAVESTATION Reference Guide (Korg, file WSReferenceGuide_E.pdf, 128 pages, scanned - no text layer) and the archive.org scan of the Wavestation A/D Reference Guide | official Korg download page, product WAVESTATION, document id 1563 | https://www.korg.com/us/support/download/manual/1/224/1563/ | the MDE effect program list, the effect routing (FX1/FX2, serial or parallel, buses A-D) and the wave/wave-sequence numbering rules |

## Counts as documented

| Section | Count | Documented banks |
|---|---|---|
| Patches (the machine's programs) | 105 | RAM1, RAM2, ROM |
| Performances (the machine's combis) | 150 | RAM1, RAM2, ROM |
| Wave sequences | 64 | RAM1, ROM |
| Waves (ROM wave list) | 397 | one continuous index |

- Patches: **105**; performances: **150**; wave sequences: **64**; waves: **397**.

## Patches

The document prints three patch banks side by side (ROM, RAM1, RAM2), each numbered from 0 and each 35 rows long. The bank is a column heading in the document, not repeated on the row.

| Shipped label | Documented name | Bank | Number | Source |
|---|---|---|---|---|
| F04-P0001 | Touch Tone | ROM | 0 | D1 p5 |
| F04-P0002 | Deep Waves | ROM | 1 | D1 p5 |
| F04-P0003 | Quarks | ROM | 2 | D1 p5 |
| F04-P0004 | DigitalResWave | ROM | 3 | D1 p5 |
| F04-P0005 | WS Strings | ROM | 4 | D1 p5 |
| F04-P0006 | Motion | ROM | 5 | D1 p5 |
| F04-P0007 | WS Metal | ROM | 6 | D1 p5 |
| F04-P0008 | WS S&H | ROM | 7 | D1 p5 |
| F04-P0009 | WS Table | ROM | 8 | D1 p5 |
| F04-P0010 | Vocalise | ROM | 9 | D1 p5 |
| F04-P0011 | Voices | ROM | 10 | D1 p5 |
| F04-P0012 | Air Vox | ROM | 11 | D1 p5 |
| F04-P0013 | Glass Vox | ROM | 12 | D1 p5 |
| F04-P0014 | Glass Bottle | ROM | 13 | D1 p5 |
| F04-P0015 | Softwaves | ROM | 14 | D1 p5 |
| F04-P0016 | SynOrch | ROM | 15 | D1 p5 |
| F04-P0017 | PWM Strings | ROM | 16 | D1 p5 |
| F04-P0018 | SynString | ROM | 17 | D1 p5 |
| F04-P0019 | Ravel By Number | ROM | 18 | D1 p5 |
| F04-P0020 | EP Body 1 | ROM | 19 | D1 p5 |
| F04-P0021 | EP Tine | ROM | 20 | D1 p5 |
| F04-P0022 | E. Piano | ROM | 21 | D1 p5 |
| F04-P0023 | Mini | ROM | 22 | D1 p5 |
| F04-P0024 | Super Clav | ROM | 23 | D1 p5 |
| F04-P0025 | Pluck 3 | ROM | 24 | D1 p5 |
| F04-P0026 | Digi Harp | ROM | 25 | D1 p5 |
| F04-P0027 | Tambotack | ROM | 26 | D1 p5 |
| F04-P0028 | Syn Bass 1 | ROM | 27 | D1 p5 |
| F04-P0029 | Vox Bass | ROM | 28 | D1 p5 |
| F04-P0030 | VS Bell | ROM | 29 | D1 p5 |
| F04-P0031 | Sweet Bells | ROM | 30 | D1 p5 |
| F04-P0032 | Soft Horn | ROM | 31 | D1 p5 |
| F04-P0033 | Syn Brass | ROM | 32 | D1 p5 |
| F04-P0034 | Wave Song | ROM | 33 | D1 p5 |
| F04-P0035 | Industrial | ROM | 34 | D1 p5 |
| F04-P0036 | Wave Dance | RAM1 | 0 | D1 p5 |
| F04-P0037 | Wave Rhythm | RAM1 | 1 | D1 p5 |
| F04-P0038 | Slow Waves | RAM1 | 2 | D1 p5 |
| F04-P0039 | Tap Drops | RAM1 | 3 | D1 p5 |
| F04-P0040 | ChromeTrans | RAM1 | 4 | D1 p5 |
| F04-P0041 | Box Waves | RAM1 | 5 | D1 p5 |
| F04-P0042 | Changin'Times | RAM1 | 6 | D1 p5 |
| F04-P0043 | Analog Sequence | RAM1 | 7 | D1 p5 |
| F04-P0044 | Sea Glass | RAM1 | 8 | D1 p5 |
| F04-P0045 | Arpeggio | RAM1 | 9 | D1 p5 |
| F04-P0046 | Env Sweep | RAM1 | 10 | D1 p5 |
| F04-P0047 | Will I Dream? | RAM1 | 11 | D1 p5 |
| F04-P0048 | Dist. Guitar | RAM1 | 12 | D1 p5 |
| F04-P0049 | Boom Bass | RAM1 | 13 | D1 p5 |
| F04-P0050 | Time Slice | RAM1 | 14 | D1 p5 |
| F04-P0051 | Entropy | RAM1 | 15 | D1 p5 |
| F04-P0052 | Glider | RAM1 | 16 | D1 p5 |
| F04-P0053 | Debussy VS | RAM1 | 17 | D1 p5 |
| F04-P0054 | Pressure Play | RAM1 | 18 | D1 p5 |
| F04-P0055 | Impressionist | RAM1 | 19 | D1 p5 |
| F04-P0056 | AlteronInvasion | RAM1 | 20 | D1 p5 |
| F04-P0057 | Cascade Falls | RAM1 | 21 | D1 p5 |
| F04-P0058 | Vectorism | RAM1 | 22 | D1 p5 |
| F04-P0059 | African Sunset | RAM1 | 23 | D1 p5 |
| F04-P0060 | Vektor Organ | RAM1 | 24 | D1 p5 |
| F04-P0061 | Organ 1 | RAM1 | 25 | D1 p5 |
| F04-P0062 | Organ 2 | RAM1 | 26 | D1 p5 |
| F04-P0063 | Organ Perc | RAM1 | 27 | D1 p5 |
| F04-P0064 | Bowing | RAM1 | 28 | D1 p5 |
| F04-P0065 | Slow Strings | RAM1 | 29 | D1 p5 |
| F04-P0066 | Doublepad | RAM1 | 30 | D1 p5 |
| F04-P0067 | Pitch Pad | RAM1 | 31 | D1 p5 |
| F04-P0068 | Sine Vox | RAM1 | 32 | D1 p5 |
| F04-P0069 | Vox Breath | RAM1 | 33 | D1 p5 |
| F04-P0070 | Choir | RAM1 | 34 | D1 p5 |
| F04-P0071 | Soft Marimba | RAM2 | 0 | D1 p5 |
| F04-P0072 | Tubular Bells | RAM2 | 1 | D1 p5 |
| F04-P0073 | Wack 1 | RAM2 | 2 | D1 p5 |
| F04-P0074 | Tamborine | RAM2 | 3 | D1 p5 |
| F04-P0075 | Bell Chimes | RAM2 | 4 | D1 p5 |
| F04-P0076 | New Metal Hit | RAM2 | 5 | D1 p5 |
| F04-P0077 | Crystals | RAM2 | 6 | D1 p5 |
| F04-P0078 | Soft EP | RAM2 | 7 | D1 p5 |
| F04-P0079 | Digi Piano | RAM2 | 8 | D1 p5 |
| F04-P0080 | VS Clav | RAM2 | 9 | D1 p5 |
| F04-P0081 | Electric Guitar | RAM2 | 10 | D1 p5 |
| F04-P0082 | Acoustic Guitar | RAM2 | 11 | D1 p5 |
| F04-P0083 | Mute Guitar | RAM2 | 12 | D1 p5 |
| F04-P0084 | Pluck 1 | RAM2 | 13 | D1 p5 |
| F04-P0085 | Pluck 2 | RAM2 | 14 | D1 p5 |
| F04-P0086 | Marbles | RAM2 | 15 | D1 p5 |
| F04-P0087 | Upright Bass | RAM2 | 16 | D1 p5 |
| F04-P0088 | Shock Bass | RAM2 | 17 | D1 p5 |
| F04-P0089 | Kick Bass | RAM2 | 18 | D1 p5 |
| F04-P0090 | TriSquare | RAM2 | 19 | D1 p5 |
| F04-P0091 | Hard Sync | RAM2 | 20 | D1 p5 |
| F04-P0092 | Nasty Waves | RAM2 | 21 | D1 p5 |
| F04-P0093 | Pop Waves | RAM2 | 22 | D1 p5 |
| F04-P0094 | Analog Brass | RAM2 | 23 | D1 p5 |
| F04-P0095 | Brass Mix | RAM2 | 24 | D1 p5 |
| F04-P0096 | Fanfare | RAM2 | 25 | D1 p5 |
| F04-P0097 | Bassoon/Oboe | RAM2 | 26 | D1 p5 |
| F04-P0098 | Mr. Alto | RAM2 | 27 | D1 p5 |
| F04-P0099 | Flute | RAM2 | 28 | D1 p5 |
| F04-P0100 | Spitty | RAM2 | 29 | D1 p5 |
| F04-P0101 | Neon Air | RAM2 | 30 | D1 p5 |
| F04-P0102 | Sonar | RAM2 | 31 | D1 p5 |
| F04-P0103 | Spectrum 1 | RAM2 | 32 | D1 p5 |
| F04-P0104 | Spectrum 2 | RAM2 | 33 | D1 p5 |
| F04-P0105 | Spectrum 3 | RAM2 | 34 | D1 p5 |

## Performances

Three banks (ROM, RAM1, RAM2), 50 rows each, numbered from 0.

| Shipped label | Documented name | Bank | Number | Source |
|---|---|---|---|---|
| F04-F0001 | The Wave Song | ROM | 0 | D1 p6 |
| F04-F0002 | Deep Atmosphere | ROM | 1 | D1 p6 |
| F04-F0003 | Sting Waves | ROM | 2 | D1 p6 |
| F04-F0004 | Metropolitan | ROM | 3 | D1 p6 |
| F04-F0005 | Mini Lead | ROM | 4 | D1 p6 |
| F04-F0006 | Tack Horns | ROM | 5 | D1 p6 |
| F04-F0007 | Guardians | ROM | 6 | D1 p6 |
| F04-F0008 | DigitalResWave | ROM | 7 | D1 p6 |
| F04-F0009 | Sandman | ROM | 8 | D1 p6 |
| F04-F0010 | Time Traveler | ROM | 9 | D1 p6 |
| F04-F0011 | Song Bells | ROM | 10 | D1 p6 |
| F04-F0012 | Analog Punch | ROM | 11 | D1 p6 |
| F04-F0013 | Cosmic Zone | ROM | 12 | D1 p6 |
| F04-F0014 | Super Clav | ROM | 13 | D1 p6 |
| F04-F0015 | Toy Box | ROM | 14 | D1 p6 |
| F04-F0016 | Analog Brass | ROM | 15 | D1 p6 |
| F04-F0017 | Modernesque | ROM | 16 | D1 p6 |
| F04-F0018 | Octave Strings | ROM | 17 | D1 p6 |
| F04-F0019 | Glass Tambu | ROM | 18 | D1 p6 |
| F04-F0020 | Vektar | ROM | 19 | D1 p6 |
| F04-F0021 | Whisper Voices | ROM | 20 | D1 p6 |
| F04-F0022 | Vulcan Harp | ROM | 21 | D1 p6 |
| F04-F0023 | Quarks | ROM | 22 | D1 p6 |
| F04-F0024 | Vocalise | ROM | 23 | D1 p6 |
| F04-F0025 | Gig Split | ROM | 24 | D1 p6 |
| F04-F0026 | Touch Brass | ROM | 25 | D1 p6 |
| F04-F0027 | Tine Piano | ROM | 26 | D1 p6 |
| F04-F0028 | Warm Strings | ROM | 27 | D1 p6 |
| F04-F0029 | Chiffy Kalimba | ROM | 28 | D1 p6 |
| F04-F0030 | Northern Lights | ROM | 29 | D1 p6 |
| F04-F0031 | Bottled Air | ROM | 30 | D1 p6 |
| F04-F0032 | Rock Stack | ROM | 31 | D1 p6 |
| F04-F0033 | Excalibur | ROM | 32 | D1 p6 |
| F04-F0034 | Wave Tables | ROM | 33 | D1 p6 |
| F04-F0035 | Bells | ROM | 34 | D1 p6 |
| F04-F0036 | Prophet Horn | ROM | 35 | D1 p6 |
| F04-F0037 | Mahogany | ROM | 36 | D1 p6 |
| F04-F0038 | Round Wound | ROM | 37 | D1 p6 |
| F04-F0039 | Digi Harp | ROM | 38 | D1 p6 |
| F04-F0040 | Motion Mix | ROM | 39 | D1 p6 |
| F04-F0041 | Stereo Waves | ROM | 40 | D1 p6 |
| F04-F0042 | Screamer | ROM | 41 | D1 p6 |
| F04-F0043 | Paradise | ROM | 42 | D1 p6 |
| F04-F0044 | Digital Touch | ROM | 43 | D1 p6 |
| F04-F0045 | Voice & Bell | ROM | 44 | D1 p6 |
| F04-F0046 | Resonant Synth | ROM | 45 | D1 p6 |
| F04-F0047 | Rhythm of the Wave | ROM | 46 | D1 p6 |
| F04-F0048 | Introspective | ROM | 47 | D1 p6 |
| F04-F0049 | Wave Mallet | ROM | 48 | D1 p6 |
| F04-F0050 | Station Platform | ROM | 49 | D1 p6 |
| F04-F0051 | Ski Jam | RAM1 | 0 | D1 p6 |
| F04-F0052 | Entropy | RAM1 | 1 | D1 p6 |
| F04-F0053 | Pinger | RAM1 | 2 | D1 p6 |
| F04-F0054 | Reswacker | RAM1 | 3 | D1 p6 |
| F04-F0055 | Lead Rock Guitar | RAM1 | 4 | D1 p6 |
| F04-F0056 | Softwaves | RAM1 | 5 | D1 p6 |
| F04-F0057 | Cascade Falls | RAM1 | 6 | D1 p6 |
| F04-F0058 | Blow the Bottle | RAM1 | 7 | D1 p6 |
| F04-F0059 | Magic Guitar | RAM1 | 8 | D1 p6 |
| F04-F0060 | Will I Dream? | RAM1 | 9 | D1 p6 |
| F04-F0061 | Fire Dance | RAM1 | 10 | D1 p6 |
| F04-F0062 | Analog Love Thang | RAM1 | 11 | D1 p6 |
| F04-F0063 | Panned Waves | RAM1 | 12 | D1 p6 |
| F04-F0064 | Super Res | RAM1 | 13 | D1 p6 |
| F04-F0065 | Ballerina Bells | RAM1 | 14 | D1 p6 |
| F04-F0066 | Soft Analog | RAM1 | 15 | D1 p6 |
| F04-F0067 | Mod Wheel Air | RAM1 | 16 | D1 p6 |
| F04-F0068 | Bowed Strings | RAM1 | 17 | D1 p6 |
| F04-F0069 | Pluckrimba | RAM1 | 18 | D1 p6 |
| F04-F0070 | Vector Guitar | RAM1 | 19 | D1 p6 |
| F04-F0071 | Midnight Run | RAM1 | 20 | D1 p6 |
| F04-F0072 | African Sunset | RAM1 | 21 | D1 p6 |
| F04-F0073 | Harmonic Motion | RAM1 | 22 | D1 p6 |
| F04-F0074 | Air Chorus & Bell | RAM1 | 23 | D1 p6 |
| F04-F0075 | SunGlasses Kid | RAM1 | 24 | D1 p6 |
| F04-F0076 | Stabby Horns | RAM1 | 25 | D1 p6 |
| F04-F0077 | Soft EP w/Tine | RAM1 | 26 | D1 p6 |
| F04-F0078 | Artificial Strg | RAM1 | 27 | D1 p6 |
| F04-F0079 | The Pied Piper | RAM1 | 28 | D1 p6 |
| F04-F0080 | Vox Concrete | RAM1 | 29 | D1 p6 |
| F04-F0081 | Snake Charmer | RAM1 | 30 | D1 p6 |
| F04-F0082 | Rock Tine Piano | RAM1 | 31 | D1 p6 |
| F04-F0083 | Pressure Glass | RAM1 | 32 | D1 p6 |
| F04-F0084 | Vox Arpeggios | RAM1 | 33 | D1 p6 |
| F04-F0085 | Struck Bell | RAM1 | 34 | D1 p6 |
| F04-F0086 | Upright & Oboe | RAM1 | 35 | D1 p6 |
| F04-F0087 | Refinery | RAM1 | 36 | D1 p6 |
| F04-F0088 | Kick up da Bass | RAM1 | 37 | D1 p6 |
| F04-F0089 | Syn Vox | RAM1 | 38 | D1 p6 |
| F04-F0090 | Kingdom Come | RAM1 | 39 | D1 p6 |
| F04-F0091 | Cat's Eye | RAM1 | 40 | D1 p6 |
| F04-F0092 | Jazz Mutes | RAM1 | 41 | D1 p6 |
| F04-F0093 | VS Bell Pad | RAM1 | 42 | D1 p6 |
| F04-F0094 | Spectra | RAM1 | 43 | D1 p6 |
| F04-F0095 | New Sparkle | RAM1 | 44 | D1 p6 |
| F04-F0096 | Vektor Organ | RAM1 | 45 | D1 p6 |
| F04-F0097 | Alien Dreams | RAM1 | 46 | D1 p6 |
| F04-F0098 | End of Voltaire | RAM1 | 47 | D1 p6 |
| F04-F0099 | Kilimanjaro | RAM1 | 48 | D1 p6 |
| F04-F0100 | Debussy On Wheels | RAM1 | 49 | D1 p6 |
| F04-F0101 | Pharoah's Jig | RAM2 | 0 | D1 p6 |
| F04-F0102 | City of Tomorrow | RAM2 | 1 | D1 p6 |
| F04-F0103 | Spectrumize | RAM2 | 2 | D1 p6 |
| F04-F0104 | Fuzzy Pop Clav | RAM2 | 3 | D1 p6 |
| F04-F0105 | ZZ Lead | RAM2 | 4 | D1 p6 |
| F04-F0106 | Wack Flute | RAM2 | 5 | D1 p6 |
| F04-F0107 | Glitter Vox | RAM2 | 6 | D1 p6 |
| F04-F0108 | Bee Hive | RAM2 | 7 | D1 p6 |
| F04-F0109 | Waves On Wheels | RAM2 | 8 | D1 p6 |
| F04-F0110 | 21st Century | RAM2 | 9 | D1 p6 |
| F04-F0111 | Sustain Pedal Jam | RAM2 | 10 | D1 p6 |
| F04-F0112 | Nasty Harmonics | RAM2 | 11 | D1 p6 |
| F04-F0113 | Glider | RAM2 | 12 | D1 p6 |
| F04-F0114 | Mr. Wave Table | RAM2 | 13 | D1 p6 |
| F04-F0115 | Alpine Bells | RAM2 | 14 | D1 p6 |
| F04-F0116 | The Big Brass | RAM2 | 15 | D1 p6 |
| F04-F0117 | Gentle Winds | RAM2 | 16 | D1 p6 |
| F04-F0118 | String & Woodwind | RAM2 | 17 | D1 p6 |
| F04-F0119 | Rain Chiffs | RAM2 | 18 | D1 p6 |
| F04-F0120 | Guitar Rez | RAM2 | 19 | D1 p6 |
| F04-F0121 | ScrittiFunk | RAM2 | 20 | D1 p6 |
| F04-F0122 | Sonar Bell String | RAM2 | 21 | D1 p6 |
| F04-F0123 | New Zealand Vice | RAM2 | 22 | D1 p6 |
| F04-F0124 | Sunday Morning | RAM2 | 23 | D1 p6 |
| F04-F0125 | Split on Sunset | RAM2 | 24 | D1 p6 |
| F04-F0126 | Brass Orchestra | RAM2 | 25 | D1 p6 |
| F04-F0127 | Digipno & Breath | RAM2 | 26 | D1 p6 |
| F04-F0128 | Lassie Come Home | RAM2 | 27 | D1 p6 |
| F04-F0129 | Mellow Square Pad | RAM2 | 28 | D1 p6 |
| F04-F0130 | Antarctica | RAM2 | 29 | D1 p6 |
| F04-F0131 | Echo Hunters | RAM2 | 30 | D1 p6 |
| F04-F0132 | Organomics | RAM2 | 31 | D1 p6 |
| F04-F0133 | Trans Atlantic | RAM2 | 32 | D1 p6 |
| F04-F0134 | The Wave Guitar | RAM2 | 33 | D1 p6 |
| F04-F0135 | Bell Tree | RAM2 | 34 | D1 p6 |
| F04-F0136 | Palo Alto Pad | RAM2 | 35 | D1 p6 |
| F04-F0137 | Thick Pick | RAM2 | 36 | D1 p6 |
| F04-F0138 | Skip's Boom Bass | RAM2 | 37 | D1 p6 |
| F04-F0139 | Folk Guitar | RAM2 | 38 | D1 p6 |
| F04-F0140 | Ivesian Split | RAM2 | 39 | D1 p6 |
| F04-F0141 | Saturn Rings | RAM2 | 40 | D1 p6 |
| F04-F0142 | Rotary Organ | RAM2 | 41 | D1 p6 |
| F04-F0143 | Star Bell Sweep | RAM2 | 42 | D1 p6 |
| F04-F0144 | Pop Box | RAM2 | 43 | D1 p6 |
| F04-F0145 | Xnaos Split | RAM2 | 44 | D1 p6 |
| F04-F0146 | Rock Steady | RAM2 | 45 | D1 p6 |
| F04-F0147 | 20 Sec. Invasion | RAM2 | 46 | D1 p6 |
| F04-F0148 | Chronos | RAM2 | 47 | D1 p6 |
| F04-F0149 | Mambo Marimba! | RAM2 | 48 | D1 p6 |
| F04-F0150 | The Big Pad | RAM2 | 49 | D1 p6 |

## Wave sequences

Two banks (ROM, RAM1), 32 rows each, numbered from 0. The document repeats the first 32 ROM wave-sequence names as the first 32 entries of its wave list.

| Shipped label | Documented name | Bank | Number | Source |
|---|---|---|---|---|
| F04-S0001 | WSTouch | ROM | 0 | D1 p4 |
| F04-S0002 | DeepWav | ROM | 1 | D1 p4 |
| F04-S0003 | Quarks | ROM | 2 | D1 p4 |
| F04-S0004 | ResXwav | ROM | 3 | D1 p4 |
| F04-S0005 | Strings | ROM | 4 | D1 p4 |
| F04-S0006 | Unison | ROM | 5 | D1 p4 |
| F04-S0007 | WSMetal | ROM | 6 | D1 p4 |
| F04-S0008 | WS S&H | ROM | 7 | D1 p4 |
| F04-S0009 | WSTable | ROM | 8 | D1 p4 |
| F04-S0010 | WSVoice | ROM | 9 | D1 p4 |
| F04-S0011 | ResMove | ROM | 10 | D1 p4 |
| F04-S0012 | WSNoise | ROM | 11 | D1 p4 |
| F04-S0013 | LobWave | ROM | 12 | D1 p4 |
| F04-S0014 | FolowMe | ROM | 13 | D1 p4 |
| F04-S0015 | P5 Res | ROM | 14 | D1 p4 |
| F04-S0016 | Complex | ROM | 15 | D1 p4 |
| F04-S0017 | WS Fade | ROM | 16 | D1 p4 |
| F04-S0018 | VelHarm | ROM | 17 | D1 p4 |
| F04-S0019 | Mini | ROM | 18 | D1 p4 |
| F04-S0020 | SoftWav | ROM | 19 | D1 p4 |
| F04-S0021 | Spectra | ROM | 20 | D1 p4 |
| F04-S0022 | WSGrowl | ROM | 21 | D1 p4 |
| F04-S0023 | SynWav1 | ROM | 22 | D1 p4 |
| F04-S0024 | EnSweep | ROM | 23 | D1 p4 |
| F04-S0025 | GateRez | ROM | 24 | D1 p4 |
| F04-S0026 | Marbles | ROM | 25 | D1 p4 |
| F04-S0027 | Ostinat | ROM | 26 | D1 p4 |
| F04-S0028 | Drops | ROM | 27 | D1 p4 |
| F04-S0029 | SloWave | ROM | 28 | D1 p4 |
| F04-S0030 | WavRytm | ROM | 29 | D1 p4 |
| F04-S0031 | Ski Jam | ROM | 30 | D1 p4 |
| F04-S0032 | WavSong | ROM | 31 | D1 p4 |
| F04-S0033 | Partial | RAM1 | 0 | D1 p4 |
| F04-S0034 | Invashn | RAM1 | 1 | D1 p4 |
| F04-S0035 | PulsMod | RAM1 | 2 | D1 p4 |
| F04-S0036 | OB Res1 | RAM1 | 3 | D1 p4 |
| F04-S0037 | OB Res2 | RAM1 | 4 | D1 p4 |
| F04-S0038 | Wave 13 | RAM1 | 5 | D1 p4 |
| F04-S0039 | Sparks | RAM1 | 6 | D1 p4 |
| F04-S0040 | SonaNoi | RAM1 | 7 | D1 p4 |
| F04-S0041 | Whisper | RAM1 | 8 | D1 p4 |
| F04-S0042 | WhSweep | RAM1 | 9 | D1 p4 |
| F04-S0043 | WavTabl | RAM1 | 10 | D1 p4 |
| F04-S0044 | Chrome | RAM1 | 11 | D1 p4 |
| F04-S0045 | DreamSq | RAM1 | 12 | D1 p4 |
| F04-S0046 | BellSwp | RAM1 | 13 | D1 p4 |
| F04-S0047 | XWind 1 | RAM1 | 14 | D1 p4 |
| F04-S0048 | TineVel | RAM1 | 15 | D1 p4 |
| F04-S0049 | Str Oct | RAM1 | 16 | D1 p4 |
| F04-S0050 | Breath | RAM1 | 17 | D1 p4 |
| F04-S0051 | OB Sax | RAM1 | 18 | D1 p4 |
| F04-S0052 | VelTran | RAM1 | 19 | D1 p4 |
| F04-S0053 | SynWav2 | RAM1 | 20 | D1 p4 |
| F04-S0054 | PlukRez | RAM1 | 21 | D1 p4 |
| F04-S0055 | TineRez | RAM1 | 22 | D1 p4 |
| F04-S0056 | RezStep | RAM1 | 23 | D1 p4 |
| F04-S0057 | AtSweep | RAM1 | 24 | D1 p4 |
| F04-S0058 | Haitian | RAM1 | 25 | D1 p4 |
| F04-S0059 | Rez Seq | RAM1 | 26 | D1 p4 |
| F04-S0060 | 3/4 Jam | RAM1 | 27 | D1 p4 |
| F04-S0061 | SpecJam | RAM1 | 28 | D1 p4 |
| F04-S0062 | RaspRap | RAM1 | 29 | D1 p4 |
| F04-S0063 | MIDISki | RAM1 | 30 | D1 p4 |
| F04-S0064 | MIDsong | RAM1 | 31 | D1 p4 |

## Waves (ROM wave list)

One continuous index printed across five columns per page. Per D2, waves 0-31 in each bank are wave sequences and waves 32 and up are the ROM waves (multisamples, attack transients, single-cycle loops); the last two waves are the analog inputs. This is engine input, not content to ingest.

| Shipped label | Documented index | Documented name | Source | Note |
|---|---|---|---|---|
| F04-W0001 | 00 | WSTouch | D1 p2 |  |
| F04-W0002 | 01 | DeepWav | D1 p2 |  |
| F04-W0003 | 02 | Quarks | D1 p2 |  |
| F04-W0004 | 03 | ResXwav | D1 p2 |  |
| F04-W0005 | 04 | Strings | D1 p2 |  |
| F04-W0006 | 05 | Unison | D1 p2 |  |
| F04-W0007 | 06 | WSMetal | D1 p2 |  |
| F04-W0008 | 07 | WS S&H | D1 p2 |  |
| F04-W0009 | 08 | WSTable | D1 p2 |  |
| F04-W0010 | 09 | WSVoice | D1 p2 |  |
| F04-W0011 | 10 | ResMove | D1 p2 |  |
| F04-W0012 | 11 | WSNoise | D1 p2 |  |
| F04-W0013 | 12 | LobWave | D1 p2 |  |
| F04-W0014 | 13 | FolowMe | D1 p2 |  |
| F04-W0015 | 14 | P5 Res | D1 p2 |  |
| F04-W0016 | 15 | Complex | D1 p2 |  |
| F04-W0017 | 16 | WS Fade | D1 p2 |  |
| F04-W0018 | 17 | VelHarm | D1 p2 |  |
| F04-W0019 | 18 | Mini | D1 p2 |  |
| F04-W0020 | 19 | SoftWav | D1 p2 |  |
| F04-W0021 | 20 | Spectra | D1 p2 |  |
| F04-W0022 | 21 | WSGrowl | D1 p2 |  |
| F04-W0023 | 22 | SynWav1 | D1 p2 |  |
| F04-W0024 | 23 | EnSweep | D1 p2 |  |
| F04-W0025 | 24 | GateRez | D1 p2 |  |
| F04-W0026 | 25 | Marbles | D1 p2 |  |
| F04-W0027 | 26 | Ostinat | D1 p2 |  |
| F04-W0028 | 27 | Drops | D1 p2 |  |
| F04-W0029 | 28 | SloWave | D1 p2 |  |
| F04-W0030 | 29 | WavRytm | D1 p2 |  |
| F04-W0031 | 30 | Ski Jam | D1 p2 |  |
| F04-W0032 | 31 | WavSong | D1 p2 |  |
| F04-W0033 | 32 | Soft EP | D1 p2 |  |
| F04-W0034 | 33 | Hard EP | D1 p2 |  |
| F04-W0035 | 34 | EP Tine | D1 p2 |  |
| F04-W0036 | 35 | EP Body1 | D1 p2 |  |
| F04-W0037 | 36 | EP Body2 | D1 p2 |  |
| F04-W0038 | 37 | EP Body3 | D1 p2 |  |
| F04-W0039 | 38 | Digi EP | D1 p2 |  |
| F04-W0040 | 39 | E PIANO3 | D1 p2 |  |
| F04-W0041 | 40 | CLAV DSM | D1 p2 |  |
| F04-W0042 | 41 | Organ 1 | D1 p2 |  |
| F04-W0043 | 42 | Organ 2 | D1 p2 |  |
| F04-W0044 | 43 | Organ 3 | D1 p2 |  |
| F04-W0045 | 44 | PipeOrg1 | D1 p2 |  |
| F04-W0046 | 45 | PipeOrg2 | D1 p2 |  |
| F04-W0047 | 46 | Pluck 1 | D1 p2 |  |
| F04-W0048 | 47 | Pluck 2 | D1 p2 |  |
| F04-W0049 | 48 | Pluck 3 | D1 p2 |  |
| F04-W0050 | 49 | A. Guitar | D1 p2 |  |
| F04-W0051 | 50 | E. Guitar | D1 p2 |  |
| F04-W0052 | 51 | Dist. Gtr | D1 p2 |  |
| F04-W0053 | 52 | EGuitChime | D1 p2 |  |
| F04-W0054 | 53 | MuteGtr1 | D1 p2 |  |
| F04-W0055 | 54 | MuteGtr2 | D1 p2 |  |
| F04-W0056 | 55 | MuteGtr3 | D1 p2 |  |
| F04-W0057 | 56 | Koto | D1 p2 |  |
| F04-W0058 | 57 | harmonic | D1 p2 |  |
| F04-W0059 | 58 | Stick | D1 p2 |  |
| F04-W0060 | 59 | E. Bass | D1 p2 |  |
| F04-W0061 | 60 | Synbass1 | D1 p2 |  |
| F04-W0062 | 61 | Synbass2 | D1 p2 |  |
| F04-W0063 | 62 | BassHarm | D1 p2 |  |
| F04-W0064 | 63 | Vibes | D1 p2 |  |
| F04-W0065 | 64 | Hi Bell | D1 p2 |  |
| F04-W0066 | 65 | Jar | D1 p2 |  |
| F04-W0067 | 66 | TinCup | D1 p2 |  |
| F04-W0068 | 67 | Agogo | D1 p2 |  |
| F04-W0069 | 68 | Gender | D1 p2 |  |
| F04-W0070 | 69 | Tubular | D1 p2 |  |
| F04-W0071 | 70 | New Pole | D1 p2 |  |
| F04-W0072 | 71 | Soft Mrmba | D1 p2 |  |
| F04-W0073 | 72 | Thai Mrmba | D1 p2 |  |
| F04-W0074 | 73 | Glass Hit | D1 p2 |  |
| F04-W0075 | 74 | Crystal | D1 p2 |  |
| F04-W0076 | 75 | Flute | D1 p2 |  |
| F04-W0077 | 76 | FluteTrans | D1 p2 |  |
| F04-W0078 | 77 | Overblown | D1 p2 |  |
| F04-W0079 | 78 | Bottle | D1 p2 |  |
| F04-W0080 | 79 | BassnOboe | D1 p2 |  |
| F04-W0081 | 80 | Clarinet | D1 p2 |  |
| F04-W0082 | 81 | BariSax | D1 p2 |  |
| F04-W0083 | 82 | TenorSax | D1 p2 |  |
| F04-W0084 | 83 | AltoSax | D1 p2 |  |
| F04-W0085 | 84 | BrassEns | D1 p2 |  |
| F04-W0086 | 85 | TromTrp | D1 p2 |  |
| F04-W0087 | 86 | Tuba&Flu | D1 p2 |  |
| F04-W0088 | 87 | Bowing | D1 p2 |  |
| F04-W0089 | 88 | Synorch | D1 p2 |  |
| F04-W0090 | 89 | PWM String | D1 p2 |  |
| F04-W0091 | 90 | SynString | D1 p2 |  |
| F04-W0092 | 91 | Airvox | D1 p2 |  |
| F04-W0093 | 92 | Voices | D1 p2 |  |
| F04-W0094 | 93 | Choir | D1 p2 |  |
| F04-W0095 | 94 | Glass Vox | D1 p2 |  |
| F04-W0096 | 95 | "OO" Vox | D1 p2 |  |
| F04-W0097 | 96 | "AH" Vox | D1 p2 |  |
| F04-W0098 | 97 | MV Wave | D1 p2 |  |
| F04-W0099 | 98 | FV Wave | D1 p2 |  |
| F04-W0100 | 99 | DW Voice | D1 p2 |  |
| F04-W0101 | 100 | SynthPad | D1 p2 |  |
| F04-W0102 | 101 | Birdland | D1 p2 |  |
| F04-W0103 | 102 | ChromRes | D1 p2 |  |
| F04-W0104 | 103 | ProSync | D1 p2 |  |
| F04-W0105 | 104 | SuperSaw | D1 p2 |  |
| F04-W0106 | 105 | Ping Wave | D1 p2 |  |
| F04-W0107 | 106 | Digital1 | D1 p2 |  |
| F04-W0108 | 107 | Digital2 | D1 p2 |  |
| F04-W0109 | 108 | Digital3 | D1 p2 |  |
| F04-W0110 | 109 | Bellwave | D1 p2 |  |
| F04-W0111 | 110 | PercWave | D1 p2 |  |
| F04-W0112 | 111 | ShellDrum | D1 p2 |  |
| F04-W0113 | 112 | BD head | D1 p2 |  |
| F04-W0114 | 113 | Tambourine | D1 p2 |  |
| F04-W0115 | 114 | Cabasa | D1 p2 |  |
| F04-W0116 | 115 | Woodblock | D1 p2 |  |
| F04-W0117 | 116 | HH Loop | D1 p2 |  |
| F04-W0118 | 117 | WhiteNoi | D1 p2 |  |
| F04-W0119 | 118 | Spectrm1 | D1 p2 |  |
| F04-W0120 | 119 | Spectrm2 | D1 p2 |  |
| F04-W0121 | 120 | Spectrm3 | D1 p2 |  |
| F04-W0122 | 121 | Spectrm4 | D1 p2 |  |
| F04-W0123 | 122 | Sonar | D1 p2 |  |
| F04-W0124 | 123 | Metal 1TR | D1 p2 |  |
| F04-W0125 | 124 | Metal 2TR | D1 p2 |  |
| F04-W0126 | 125 | KalimbaTR | D1 p2 |  |
| F04-W0127 | 126 | GamelaTR | D1 p2 |  |
| F04-W0128 | 127 | MarimbaTR | D1 p2 |  |
| F04-W0129 | 128 | Potnoise | D1 p2 |  |
| F04-W0130 | 129 | Ticker | D1 p2 |  |
| F04-W0131 | 130 | VibeHit | D1 p2 |  |
| F04-W0132 | 131 | Whack 1 | D1 p2 |  |
| F04-W0133 | 132 | Whack 2 | D1 p2 |  |
| F04-W0134 | 133 | HDulciTR | D1 p2 |  |
| F04-W0135 | 134 | HoseHit1 | D1 p2 |  |
| F04-W0136 | 135 | HoseHit2 | D1 p2 |  |
| F04-W0137 | 136 | SynbassTR | D1 p2 |  |
| F04-W0138 | 137 | A.BassTR | D1 p2 |  |
| F04-W0139 | 138 | "ch" | D1 p2 |  |
| F04-W0140 | 139 | "hhh" | D1 p2 |  |
| F04-W0141 | 140 | "kkk" | D1 p2 |  |
| F04-W0142 | 141 | "puh" | D1 p2 |  |
| F04-W0143 | 142 | "sss" | D1 p2 |  |
| F04-W0144 | 143 | "tnn" | D1 p2 |  |
| F04-W0145 | 144 | Inharm1 | D1 p2 |  |
| F04-W0146 | 145 | Inharm2 | D1 p2 |  |
| F04-W0147 | 146 | Inharm3 | D1 p2 |  |
| F04-W0148 | 147 | Inharm4 | D1 p2 |  |
| F04-W0149 | 148 | Inharm5 | D1 p2 |  |
| F04-W0150 | 149 | Inharm6 | D1 p2 |  |
| F04-W0151 | 150 | Inharm7 | D1 p2 |  |
| F04-W0152 | 151 | Inharm8 | D1 p2 |  |
| F04-W0153 | 152 | Inharm9 | D1 p2 |  |
| F04-W0154 | 153 | Inharm10 | D1 p2 |  |
| F04-W0155 | 154 | Formant1 | D1 p2 |  |
| F04-W0156 | 155 | Formant2 | D1 p2 |  |
| F04-W0157 | 156 | Formant3 | D1 p2 |  |
| F04-W0158 | 157 | Formant4 | D1 p2 |  |
| F04-W0159 | 158 | Formant5 | D1 p2 |  |
| F04-W0160 | 159 | Formant6 | D1 p2 |  |
| F04-W0161 | 160 | Formant7 | D1 p2 |  |
| F04-W0162 | 161 | Sine | D1 p2 |  |
| F04-W0163 | 162 | Triangle | D1 p2 |  |
| F04-W0164 | 163 | VS 35 | D1 p2 |  |
| F04-W0165 | 164 | VS 36 | D1 p2 |  |
| F04-W0166 | 165 | VS 37 | D1 p2 |  |
| F04-W0167 | 166 | VS 38 | D1 p2 |  |
| F04-W0168 | 167 | VS 39 | D1 p2 |  |
| F04-W0169 | 168 | VS 40 | D1 p2 |  |
| F04-W0170 | 169 | VS 41 | D1 p2 |  |
| F04-W0171 | 170 | VS 42 | D1 p2 |  |
| F04-W0172 | 171 | VS 43 | D1 p2 |  |
| F04-W0173 | 172 | VS 44 | D1 p2 |  |
| F04-W0174 | 173 | VS 45 | D1 p2 |  |
| F04-W0175 | 174 | VS 46 | D1 p2 |  |
| F04-W0176 | 175 | VS 47 | D1 p2 |  |
| F04-W0177 | 176 | VS 48 | D1 p2 |  |
| F04-W0178 | 177 | VS 49 | D1 p2 |  |
| F04-W0179 | 178 | VS 50 | D1 p2 |  |
| F04-W0180 | 179 | VS 51 | D1 p2 |  |
| F04-W0181 | 180 | VS 52 | D1 p2 |  |
| F04-W0182 | 181 | VS 53 | D1 p2 |  |
| F04-W0183 | 182 | VS 54 | D1 p2 |  |
| F04-W0184 | 183 | VS 55 | D1 p2 |  |
| F04-W0185 | 184 | VS 56 | D1 p2 |  |
| F04-W0186 | 185 | VS 57 | D1 p2 |  |
| F04-W0187 | 186 | VS 58 | D1 p2 |  |
| F04-W0188 | 187 | VS 59 | D1 p2 |  |
| F04-W0189 | 188 | VS 60 | D1 p2 |  |
| F04-W0190 | 189 | VS 61 | D1 p2 |  |
| F04-W0191 | 190 | VS 62 | D1 p2 |  |
| F04-W0192 | 191 | VS 63 | D1 p2 |  |
| F04-W0193 | 192 | VS 64 | D1 p2 |  |
| F04-W0194 | 193 | VS 65 | D1 p2 |  |
| F04-W0195 | 194 | VS 66 | D1 p2 |  |
| F04-W0196 | 195 | VS 67 | D1 p2 |  |
| F04-W0197 | 196 | VS 68 | D1 p2 |  |
| F04-W0198 | 197 | VS 69 | D1 p2 |  |
| F04-W0199 | 198 | VS 70 | D1 p2 |  |
| F04-W0200 | 199 | VS 71 | D1 p2 |  |
| F04-W0201 | 200 | VS 72 | D1 p3 |  |
| F04-W0202 | 201 | VS 73 | D1 p3 |  |
| F04-W0203 | 202 | VS 74 | D1 p3 |  |
| F04-W0204 | 203 | VS 75 | D1 p3 |  |
| F04-W0205 | 204 | VS 76 | D1 p3 |  |
| F04-W0206 | 205 | VS 77 | D1 p3 |  |
| F04-W0207 | 206 | VS 78 | D1 p3 |  |
| F04-W0208 | 207 | VS 79 | D1 p3 |  |
| F04-W0209 | 208 | VS 80 | D1 p3 |  |
| F04-W0210 | 209 | VS 81 | D1 p3 |  |
| F04-W0211 | 210 | VS 82 | D1 p3 |  |
| F04-W0212 | 211 | VS 83 | D1 p3 |  |
| F04-W0213 | 212 | VS 84 | D1 p3 |  |
| F04-W0214 | 213 | VS 85 | D1 p3 |  |
| F04-W0215 | 214 | VS 86 | D1 p3 |  |
| F04-W0216 | 215 | VS 87 | D1 p3 |  |
| F04-W0217 | 216 | VS 88 | D1 p3 |  |
| F04-W0218 | 217 | VS 89 | D1 p3 |  |
| F04-W0219 | 218 | VS 90 | D1 p3 |  |
| F04-W0220 | 219 | VS 91 | D1 p3 |  |
| F04-W0221 | 220 | VS 92 | D1 p3 |  |
| F04-W0222 | 221 | VS 93 | D1 p3 |  |
| F04-W0223 | 222 | VS 94 | D1 p3 |  |
| F04-W0224 | 223 | VS 95 | D1 p3 |  |
| F04-W0225 | 224 | VS 96 | D1 p3 |  |
| F04-W0226 | 225 | VS 97 | D1 p3 |  |
| F04-W0227 | 226 | VS 98 | D1 p3 |  |
| F04-W0228 | 227 | VS 99 | D1 p3 |  |
| F04-W0229 | 228 | VS 100 | D1 p3 |  |
| F04-W0230 | 229 | VS 101 | D1 p3 |  |
| F04-W0231 | 230 | VS 102 | D1 p3 |  |
| F04-W0232 | 231 | VS 103 | D1 p3 |  |
| F04-W0233 | 232 | VS 104 | D1 p3 |  |
| F04-W0234 | 233 | VS 105 | D1 p3 |  |
| F04-W0235 | 234 | VS 106 | D1 p3 |  |
| F04-W0236 | 235 | VS 107 | D1 p3 |  |
| F04-W0237 | 236 | VS 108 | D1 p3 |  |
| F04-W0238 | 237 | VS 109 | D1 p3 |  |
| F04-W0239 | 238 | VS 110 | D1 p3 |  |
| F04-W0240 | 239 | VS 111 | D1 p3 |  |
| F04-W0241 | 240 | VS 112 | D1 p3 |  |
| F04-W0242 | 241 | VS 113 | D1 p3 |  |
| F04-W0243 | 242 | VS 114 | D1 p3 |  |
| F04-W0244 | 243 | VS 115 | D1 p3 |  |
| F04-W0245 | 244 | VS 116 | D1 p3 |  |
| F04-W0246 | 245 | VS 117 | D1 p3 |  |
| F04-W0247 | 246 | VS 118 | D1 p3 |  |
| F04-W0248 | 247 | VS 119 | D1 p3 |  |
| F04-W0249 | 248 | VS 120 | D1 p3 |  |
| F04-W0250 | 249 | VS 121 | D1 p3 |  |
| F04-W0251 | 250 | VS 122 | D1 p3 |  |
| F04-W0252 | 251 | VS 123 | D1 p3 |  |
| F04-W0253 | 252 | VS 124 | D1 p3 |  |
| F04-W0254 | 253 | VS 125 | D1 p3 |  |
| F04-W0255 | 254 | saw | D1 p3 |  |
| F04-W0256 | 255 | OBPUL1 | D1 p3 |  |
| F04-W0257 | 256 | OBPUL3 | D1 p3 |  |
| F04-W0258 | 257 | OBPUL4 | D1 p3 |  |
| F04-W0259 | 258 | OBPUL5 | D1 p3 |  |
| F04-W0260 | 259 | OBPUL6 | D1 p3 |  |
| F04-W0261 | 260 | OBPUL7 | D1 p3 |  |
| F04-W0262 | 261 | OBRES1 | D1 p3 |  |
| F04-W0263 | 262 | OBRES2 | D1 p3 |  |
| F04-W0264 | 263 | OBRES3 | D1 p3 |  |
| F04-W0265 | 264 | OBSAW3 | D1 p3 |  |
| F04-W0266 | 265 | OBTRESB | D1 p3 |  |
| F04-W0267 | 266 | OBTRESD | D1 p3 |  |
| F04-W0268 | 267 | OBTRESF | D1 p3 |  |
| F04-W0269 | 268 | OBTRESH | D1 p3 |  |
| F04-W0270 | 269 | OBTRESJ | D1 p3 |  |
| F04-W0271 | 270 | OBTRESL | D1 p3 |  |
| F04-W0272 | 271 | OBTRESN | D1 p3 |  |
| F04-W0273 | 272 | PPUL2 | D1 p3 |  |
| F04-W0274 | 273 | PPUL3 | D1 p3 |  |
| F04-W0275 | 274 | PPUL4 | D1 p3 |  |
| F04-W0276 | 275 | PPUL5 | D1 p3 |  |
| F04-W0277 | 276 | PPUL6 | D1 p3 |  |
| F04-W0278 | 277 | PSAW2 | D1 p3 |  |
| F04-W0279 | 278 | 13 - 01 | D1 p3 |  |
| F04-W0280 | 279 | 13 - 03 | D1 p3 |  |
| F04-W0281 | 280 | 13 - 05 | D1 p3 |  |
| F04-W0282 | 281 | 13 - 07 | D1 p3 |  |
| F04-W0283 | 282 | 13 - 09 | D1 p3 |  |
| F04-W0284 | 283 | 13 - 11 | D1 p3 |  |
| F04-W0285 | 284 | 13 - 13 | D1 p3 |  |
| F04-W0286 | 285 | 13 - 15 | D1 p3 |  |
| F04-W0287 | 286 | 13 - 17 | D1 p3 |  |
| F04-W0288 | 287 | 13 - 19 | D1 p3 |  |
| F04-W0289 | 288 | 13 - 21 | D1 p3 |  |
| F04-W0290 | 289 | 13 - 23 | D1 p3 |  |
| F04-W0291 | 290 | 13 - 25 | D1 p3 |  |
| F04-W0292 | 291 | 13 - 27 | D1 p3 |  |
| F04-W0293 | 292 | 13 - 29 | D1 p3 |  |
| F04-W0294 | 293 | 13 - 31 | D1 p3 |  |
| F04-W0295 | 294 | 13 - 33 | D1 p3 |  |
| F04-W0296 | 295 | 13 - 35 | D1 p3 |  |
| F04-W0297 | 296 | 13 - 37 | D1 p3 |  |
| F04-W0298 | 297 | 13 - 39 | D1 p3 |  |
| F04-W0299 | 298 | 13 - 41 | D1 p3 |  |
| F04-W0300 | 299 | 13 - 43 | D1 p3 |  |
| F04-W0301 | 300 | 13 - 45 | D1 p3 |  |
| F04-W0302 | 301 | 13 - 47 | D1 p3 |  |
| F04-W0303 | 302 | 13 - 49 | D1 p3 |  |
| F04-W0304 | 303 | 13 - 51 | D1 p3 |  |
| F04-W0305 | 304 | 13 - 53 | D1 p3 |  |
| F04-W0306 | 305 | 13 - 55 | D1 p3 |  |
| F04-W0307 | 306 | 13 - 57 | D1 p3 |  |
| F04-W0308 | 307 | 13 - 59 | D1 p3 |  |
| F04-W0309 | 308 | 13 - 61 | D1 p3 |  |
| F04-W0310 | 309 | 13 - 63 | D1 p3 |  |
| F04-W0311 | 310 | resx - 001 | D1 p3 |  |
| F04-W0312 | 311 | resx - 002 | D1 p3 |  |
| F04-W0313 | 312 | resx - 003 | D1 p3 |  |
| F04-W0314 | 313 | resx - 004 | D1 p3 |  |
| F04-W0315 | 314 | resx - 005 | D1 p3 |  |
| F04-W0316 | 315 | resx - 006 | D1 p3 |  |
| F04-W0317 | 316 | resx - 007 | D1 p3 |  |
| F04-W0318 | 317 | resx - 008 | D1 p3 |  |
| F04-W0319 | 318 | resx - 009 | D1 p3 |  |
| F04-W0320 | 319 | resx - 010 | D1 p3 |  |
| F04-W0321 | 320 | resx - 011 | D1 p3 |  |
| F04-W0322 | 321 | resx - 012 | D1 p3 |  |
| F04-W0323 | 322 | resx - 013 | D1 p3 |  |
| F04-W0324 | 323 | resx - 014 | D1 p3 |  |
| F04-W0325 | 324 | resx - 015 | D1 p3 |  |
| F04-W0326 | 325 | resx - 016 | D1 p3 |  |
| F04-W0327 | 326 | resx - 017 | D1 p3 |  |
| F04-W0328 | 327 | resx - 018 | D1 p3 |  |
| F04-W0329 | 328 | resx - 019 | D1 p3 |  |
| F04-W0330 | 329 | resx - 020 | D1 p3 |  |
| F04-W0331 | 330 | resx - 021 | D1 p3 |  |
| F04-W0332 | 331 | resx - 022 | D1 p3 |  |
| F04-W0333 | 332 | resx - 023 | D1 p3 |  |
| F04-W0334 | 333 | resx - 024 | D1 p3 |  |
| F04-W0335 | 334 | resx - 025 | D1 p3 |  |
| F04-W0336 | 335 | resx - 026 | D1 p3 |  |
| F04-W0337 | 336 | resx - 027 | D1 p3 |  |
| F04-W0338 | 337 | resx - 028 | D1 p3 |  |
| F04-W0339 | 338 | resx - 029 | D1 p3 |  |
| F04-W0340 | 339 | resx - 030 | D1 p3 |  |
| F04-W0341 | 340 | resx - 031 | D1 p3 |  |
| F04-W0342 | 341 | resx - 032 | D1 p3 |  |
| F04-W0343 | 342 | Min1 - 01a | D1 p3 |  |
| F04-W0344 | 343 | Min1 - 02a | D1 p3 |  |
| F04-W0345 | 344 | Min1 - 04a | D1 p3 |  |
| F04-W0346 | 345 | Min1 - 05a | D1 p3 |  |
| F04-W0347 | 346 | Min1 - 06a | D1 p3 |  |
| F04-W0348 | 347 | Min1 - 07a | D1 p3 |  |
| F04-W0349 | 348 | Min1 - 08a | D1 p3 |  |
| F04-W0350 | 349 | Min1 - 09a | D1 p3 |  |
| F04-W0351 | 350 | Min1 - 12a | D1 p3 |  |
| F04-W0352 | 351 | Min1 - 13a | D1 p3 |  |
| F04-W0353 | 352 | Pres - 321 | D1 p3 |  |
| F04-W0354 | 353 | Pres - 335 | D1 p3 |  |
| F04-W0355 | 354 | Pres - 349 | D1 p3 |  |
| F04-W0356 | 355 | Pres - 363 | D1 p3 |  |
| F04-W0357 | 356 | Pres - 377 | D1 p3 |  |
| F04-W0358 | 357 | Pres - 384 | D1 p3 |  |
| F04-W0359 | 358 | Pres - 391 | D1 p3 |  |
| F04-W0360 | 359 | Pres - 398 | D1 p3 |  |
| F04-W0361 | 360 | Pres - 110 | D1 p3 |  |
| F04-W0362 | 361 | Pres - 3112 | D1 p3 |  |
| F04-W0363 | 362 | Pres - 3119 | D1 p3 |  |
| F04-W0364 | 363 | Pres - 3126 | D1 p3 |  |
| F04-W0365 | 364 | Sax .1sec | D1 p3 |  |
| F04-W0366 | 365 | Sax 1 sec | D1 p3 |  |
| F04-W0367 | 366 | Sax 1.3sec | D1 p3 |  |
| F04-W0368 | 367 | Sax 1.5sec | D1 p3 |  |
| F04-W0369 | 368 | Sax 1.7sec | D1 p3 |  |
| F04-W0370 | 369 | Sax 2 sec | D1 p3 |  |
| F04-W0371 | 370 | Sax 2.2sec | D1 p3 |  |
| F04-W0372 | 371 | Sax 2.4sec | D1 p3 |  |
| F04-W0373 | 372 | Sax 2.7sec | D1 p3 |  |
| F04-W0374 | 373 | Sax 2.9sec | D1 p3 |  |
| F04-W0375 | 374 | Sax 3 sec | D1 p3 |  |
| F04-W0376 | 375 | Sax 3.4sec | D1 p3 |  |
| F04-W0377 | 376 | Sax 3.6sec | D1 p3 |  |
| F04-W0378 | 377 | Sax 4.3sec | D1 p3 |  |
| F04-W0379 | 378 | Sax 4.7sec | D1 p3 |  |
| F04-W0380 | 379 | Sax 5 sec | D1 p3 |  |
| F04-W0381 | 380 | Square | D1 p3 |  |
| F04-W0382 | 381 | Pulse02 | D1 p3 |  |
| F04-W0383 | 382 | Pulse04 | D1 p3 |  |
| F04-W0384 | 383 | Pulse06 | D1 p3 |  |
| F04-W0385 | 384 | Pulse08 | D1 p3 |  |
| F04-W0386 | 385 | Pulse10 | D1 p3 |  |
| F04-W0387 | 386 | Pulse12 | D1 p3 |  |
| F04-W0388 | 387 | Pulse14 | D1 p3 |  |
| F04-W0389 | 388 | Pulse16 | D1 p3 |  |
| F04-W0390 | 389 | Pulse18 | D1 p3 |  |
| F04-W0391 | 390 | Pulse20 | D1 p3 |  |
| F04-W0392 | 391 | Pulse22 | D1 p3 |  |
| F04-W0393 | 392 | Pulse24 | D1 p3 |  |
| F04-W0394 | 393 | Pulse26 | D1 p3 |  |
| F04-W0395 | 394 | Pulse28 | D1 p3 |  |
| F04-W0396 | 395 | Pulse30 | D1 p3 |  |
| F04-W0397 | 396 | Pulse31 | D1 p3 |  |

## Factory FX algorithms and routing

**Routing: documented (D2).** The instrument has an MDE (Multi Digital Effects) processor with two effect slots, FX1 and FX2, which can be routed **serial or parallel**; four buses (A-D) feed the effects and each wave is routed to an FX bus. The guide states the effect programs are 55 programs, variations on approximately 26 basic effect types, and that each program is a macro for one of those types.

**Per-patch FX chains: not documented.** No published document states which effect program each factory patch or performance uses. That only exists in the machine's own factory data, which is out of bounds here, so per-patch chains are reconstructed by ear.

**Effect program list: partially recovered.** The list is printed only in the Reference Guide's effects chapter, and both available copies are scans whose text layer is machine-OCR of uneven quality (D2). The table below is what two independent scans agree on; entries marked `?` could not be read from either scan and must be checked against a clean copy of the manual. The guide presents this list as the Wavestation A/D's effect programs, so confirm it against the original Wavestation manual before using it as Family 04's chain vocabulary.

| # | Effect program as printed | Note |
|---|---|---|
| 00 | No Effect |  |
| 01 | Small hall reverb - EQ |  |
| 02 | Medium hall reverb - EQ |  |
| 03 | Large hall reverb - EQ |  |
| 04 | Small room reverb - EQ | the two scans read this entry differently: Small room reverb - EQ / Small roo |
| 05 | Large room reverb - EQ |  |
| 06 | Live stage - EQ |  |
| 07 | Wet plate reverb - EQ |  |
| 08 | Dry plate reverb - EQ |  |
| 09 | Spring reverb - EQ |  |
| 10 | Early reflections - EQ 1 |  |
| 11 | Early reflections - EQ 2 |  |
| 12 | Early reflections - EQ 3 |  |
| 13 | Forward gated reverb | the two scans read this entry differently: Trombone. / Forward gated reverb |
| 14 | Reverse gated reverb |  |
| 15 | Stereo delay |  |
| 16 | Ping-pong delay |  |
| 17 | Dual mono delay |  |
| 18 | Multi-tap delay - EQ 1 | the two scans read this entry differently: Multi-tap delay - EQ 1 / Multl-tap delay - EQ 1 |
| 19 | Multi-tap delay - EQ 2 |  |
| 20 | Multi-tap delay - EQ 3 |  |
| 21 | Stereo chorus - EQ |  |
| 22 | Quadrature chorus - EQ |  |
| 23 | Crossover chorus-EQ | scan OCR carries trailing page text; name trimmed at the last clean word |
| 24 | Harmonic chorus |  |
| 25 | Stereo flanger - EQ 1 |  |
| 26 | Stereo flanger - EQ 2 |  |
| 27 | Crossover flanger - EQ | the two scans read this entry differently: A value of 0 means that velocity has no effect on pan. / Crossover flanger - EQ |
| 28 | Enhancer - Exciter - EQ |  |
| 29 | Distortion - Filter - EQ |  |
| 30 | Overdrive - Filter - EQ |  |
| 31 | Stereo phaser 1 |  |
| 32 | Stereo phaser 2 |  |
| 33 | Rotary speaker |  |
| 34 | Stereo mod - pan - EQ |  |
| 35 | Quadrature mod - pan - EQ |  |
| 36 | Stereo parametric EQ |  |
| 37 | Chorus - Stereo delay - EQ |  |
| 38 | Flanger -Stereo delay - EQ |  |
| 39 | Delay/hall |  |
| 40 | Delay/room |  |
| 41 | Delay/Chorus |  |
| 42 | Delay/Flanger |  |
| 43 | Delay/Distortion-filter |  |
| 44 | Delay/Overdrive-filter |  |
| 45 | Delay/Phaser | scan OCR carries trailing page text; name trimmed at the last clean word |
| 46 | Delay/Rotary speaker |  |
| 47 | Stereo pitch shifter |  |
| 48 | Mod pitch shift-Dly |  |
| 49 | Stereo comp-lim/Gate | the two scans read this entry differently: Stereo comp-lim/Gate / Multi-tap delay - EQ 2 |
| 50 | Small vocoder 1 | the two scans read this entry differently: Small vocoder 1 / Small vocoder1 __ de |
| 51 | Small vocoder 2 |  |
| 52 | Small vocoder 3 |  |
| 53 | Small vocoder 4 |  |
| 54 | Stereo vocoder - Delay 1 |  |
| 55 | Stereo vocoder - Delay 2 | the two scans read this entry differently: Stereo vocoder - Delay 2 / Stereo vocoder : Delay 2 |

