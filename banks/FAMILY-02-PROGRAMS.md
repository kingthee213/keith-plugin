# FAMILY 02 - factory programs and combinations (internal reference)

Reference machine (internal name, never shipped): **Triton Extreme**.
Shipped label: **Family 02**.

Machine-readable source of these rows: `tools/enumerate/triton_vnl.py` run against the Voice Name List PDF; the PDF itself is not stored in this repository.

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

| Ref | Document | Where it came from | URL | What it supplies |
|---|---|---|---|---|
| D1 | TRITON Extreme Voice Name List (Korg, file TRITON Extreme Voice Name List, 40 pages) | official Korg download page, product TRITON Extreme | https://www.korg.com/us/support/download/product/1/215/ | combinations (banks A-D, H-M), programs (banks A-D, G, H-N), drum kits with the per-key drumsample mapping, GM drum kits, multisamples, drumsamples |
| D2 | TRITON Extreme Parameter Guide (Korg, file TNEX_ParameterGuide_E2, 354 pages, created 2004-02-23) | official Korg download page, product TRITON Extreme, document id 2423 | https://www.korg.com/us/support/download/manual/1/215/2423/ | effect algorithm catalogue (000-102) and the insert/master effect routing architecture |

## Counts as documented

| Section | Count | Documented banks |
|---|---|---|
| Factory combinations | 1280 | A, B, C, D, H, I, J, K, L, M |
| Factory programs | 1609 | A, B, C, D, G, H, I, J, K, L, M, N |
| Factory drum kits | 50 | n/a |
| GM drum kits | 9 | n/a |
| ROM multisamples (PCM waves) | 442 | n/a |
| ROM drumsamples (PCM waves) | 1233 | n/a |

- Combinations: **1280** documented entries (banks A, B, C, D, H, I, J, K, L, M).
- Programs: **1609** documented entries (banks A, B, C, D, G, H, I, J, K, L, M, N).
- Drum kits: **50** factory kits plus **9** GM kits - see `FAMILY-02-DRUMS.md`.
- PCM multisamples: **442** documented rows.
- PCM drumsamples: **1233** documented rows.

## Combinations

Category is the document's own category column. Every row is one documented factory combination.

| Shipped label | Documented name | Bank | Number | Category | Source | Note |
|---|---|---|---|---|---|---|
| F02-C0001 |  | A | A000 | 00 | D1 p3 |  |
| F02-C0002 | Mellow Piano Pad | A | A064 | 00 | D1 p3 |  |
| F02-C0003 | Stereo Piano | B | B000 | 00 | D1 p3 |  |
| F02-C0004 | Soft Piano Pad | B | B064 |  | D1 p3 |  |
| F02-C0005 | The Ballade | C | C000 | 00 | D1 p3 |  |
| F02-C0006 |  | A | A001 | 05 | D1 p3 |  |
| F02-C0007 | Strings & Reeds | A | A065 | 05 | D1 p3 |  |
| F02-C0008 | Velo Orch. SW1 | B | B001 | 05 | D1 p3 |  |
| F02-C0009 | From pp To ff | B | B065 |  | D1 p3 |  |
| F02-C0010 | EXP Power Orch. | C | C001 | 05 | D1 p3 |  |
| F02-C0011 |  | A | A002 | 09 | D1 p3 |  |
| F02-C0012 | Millennium Pad | A | A066 | 09 | D1 p3 |  |
| F02-C0013 | RHYTHMWAVE | B | B002 | 09 | D1 p3 |  |
| F02-C0014 | Arp Factory 2 | B | B066 |  | D1 p3 |  |
| F02-C0015 | Analog Theme | C | C002 | 09 | D1 p3 |  |
| F02-C0016 |  | A | A003 | 14 | D1 p3 |  |
| F02-C0017 | Paradise Cycle | A | A067 | 14 | D1 p3 |  |
| F02-C0018 | Psy-Trance | B | B003 | 14 | D1 p3 |  |
| F02-C0019 | Trancy Euphoria | B | B067 |  | D1 p3 |  |
| F02-C0020 | IBIZA Midnight | C | C003 | 14 | D1 p3 |  |
| F02-C0021 |  | A | A004 | 12 | D1 p3 |  |
| F02-C0022 | }Future Child{ | A | A068 | 12 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0023 | Security Code | B | B004 | 12 | D1 p3 |  |
| F02-C0024 | OAB Techno | B | B068 |  | D1 p3 |  |
| F02-C0025 | Filter Muzik | C | C004 | 12 | D1 p3 |  |
| F02-C0026 |  | A | A005 | 12 | D1 p3 |  |
| F02-C0027 | Starlight | A | A069 | 12 | D1 p3 |  |
| F02-C0028 | Groovin'A'Bass | B | B005 | 12 | D1 p3 |  |
| F02-C0029 | 70's FM Station | B | B069 |  | D1 p3 |  |
| F02-C0030 | Swing Flavor | C | C005 | 12 | D1 p3 |  |
| F02-C0031 |  | A | A006 | 06 | D1 p3 |  |
| F02-C0032 | Song Of Angels | A | A070 | 06 | D1 p3 |  |
| F02-C0033 | Rhythm Of Asia | B | B006 | 06 | D1 p3 |  |
| F02-C0034 | Isle Of Indigo | B | B070 |  | D1 p3 |  |
| F02-C0035 | East Wind | C | C006 | 06 | D1 p3 |  |
| F02-C0036 |  | A | A007 | 03 | D1 p3 |  |
| F02-C0037 | All Strings Sect | A | A071 | 03 | D1 p3 |  |
| F02-C0038 | Dynamic Strings | B | B007 | 03 | D1 p3 |  |
| F02-C0039 | String Concerto | B | B071 |  | D1 p3 |  |
| F02-C0040 | Attack & Thread | C | C007 | 03 | D1 p3 |  |
| F02-C0041 |  | A | A008 | 10 | D1 p3 |  |
| F02-C0042 | POLY E.D.G KEY | A | A072 | 10 | D1 p3 |  |
| F02-C0043 | Cosmic Sweeper | B | B008 | 10 | D1 p3 |  |
| F02-C0044 | Vintage Strings | B | B072 |  | D1 p3 |  |
| F02-C0045 | Analog 101 | C | C008 | 10 | D1 p3 |  |
| F02-C0046 |  | A | A009 | 08 | D1 p3 |  |
| F02-C0047 | VocalPadLand | A | A073 | 08 | D1 p3 |  |
| F02-C0048 | *Pad Tropolis* | B | B009 | 08 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0049 | Electronic Dream | B | B073 |  | D1 p3 |  |
| F02-C0050 | Deep Aqua Pad | C | C009 | 08 | D1 p3 |  |
| F02-C0051 |  | A | A010 | 02 | D1 p3 |  |
| F02-C0052 | Vaderian Bells | A | A074 | 02 | D1 p3 |  |
| F02-C0053 | Bellidescope | B | B010 | 02 | D1 p3 |  |
| F02-C0054 | Luna Lumina | B | B074 |  | D1 p3 |  |
| F02-C0055 | Vibe & StringBed | C | C010 | 02 | D1 p3 |  |
| F02-C0056 |  | A | A011 | 04 | D1 p3 |  |
| F02-C0057 | SFZ Brass Ens. | A | A075 | 04 | D1 p3 |  |
| F02-C0058 | Big Bad Brass 1 | B | B011 | 04 | D1 p3 |  |
| F02-C0059 | Full mf Brass | B | B075 |  | D1 p3 |  |
| F02-C0060 | Big Jazz Band | C | C011 | 04 | D1 p3 |  |
| F02-C0061 |  | A | A012 | 11 | D1 p3 |  |
| F02-C0062 | SoprSax & AcBass | A | A076 | 11 | D1 p3 |  |
| F02-C0063 | Samba D's Accent | B | B012 | 11 | D1 p3 |  |
| F02-C0064 | Milestro | B | B076 |  | D1 p3 |  |
| F02-C0065 | Who Loves You? | C | C012 | 11 | D1 p3 |  |
| F02-C0066 |  | A | A013 | 07 | D1 p3 |  |
| F02-C0067 | Strumming Guitar | A | A077 | 07 | D1 p3 |  |
| F02-C0068 | My Sassy Guitar | B | B013 | 07 | D1 p3 |  |
| F02-C0069 | 12-Strings | B | B077 |  | D1 p3 |  |
| F02-C0070 | C.Guitar Knobs | C | C013 | 07 | D1 p3 |  |
| F02-C0071 |  | A | A014 | 01 | D1 p3 |  |
| F02-C0072 | 60sOrgan(K1-4) | A | A078 | 01 | D1 p3 |  |
| F02-C0073 | Old Wheelin` CX3 | B | B014 | 01 | D1 p3 |  |
| F02-C0074 | Super Jazz Organ | B | B078 |  | D1 p3 |  |
| F02-C0075 | Manual-Split | C | C014 | 01 | D1 p3 |  |
| F02-C0076 |  | A | A015 | 15 | D1 p3 |  |
| F02-C0077 | Yo,Wasup Dude! | A | A079 | 15 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0078 | Massiv KAOS Kit | B | B015 | 15 | D1 p3 |  |
| F02-C0079 | Trance Split | B | B079 |  | D1 p3 |  |
| F02-C0080 | E054d | C | C015 | 13 | D1 p3 |  |
| F02-C0081 |  | A | A016 | 00 | D1 p3 |  |
| F02-C0082 | Studio E.Piano | A | A080 | 00 | D1 p3 |  |
| F02-C0083 | Hip Hop Clav 2 | B | B016 | 00 | D1 p3 |  |
| F02-C0084 | Clavicoder | B | B080 |  | D1 p3 |  |
| F02-C0085 | Damper Wah Clav | C | C016 | 00 | D1 p3 |  |
| F02-C0086 |  | A | A017 | 05 | D1 p3 |  |
| F02-C0087 | Eng. Horn & Bows | A | A081 | 05 | D1 p3 |  |
| F02-C0088 | DramaticSndTrack | B | B017 | 05 | D1 p3 |  |
| F02-C0089 | Film Tools | B | B081 |  | D1 p3 |  |
| F02-C0090 | Dynamic Orch. | C | C017 | 05 | D1 p3 |  |
| F02-C0091 |  | A | A018 | 09 | D1 p3 |  |
| F02-C0092 | **Weightless** | A | A082 | 09 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0093 | Xover Frequency | B | B018 | 09 | D1 p3 |  |
| F02-C0094 | Polyaction | B | B082 |  | D1 p3 |  |
| F02-C0095 | ErraJ si Gnivil | C | C018 | 09 | D1 p3 |  |
| F02-C0096 |  | A | A019 | 14 | D1 p3 |  |
| F02-C0097 | Funky Lump | A | A083 | 14 | D1 p3 |  |
| F02-C0098 | Real Steppers | B | B019 | 14 | D1 p3 |  |
| F02-C0099 | Funky Moves | B | B083 |  | D1 p3 |  |
| F02-C0100 | Garage'r'us! | C | C019 | 14 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0101 |  | A | A020 | 12 | D1 p3 |  |
| F02-C0102 | Deep Jungle | A | A084 | 12 | D1 p3 |  |
| F02-C0103 | Nu Cyber Breaks | B | B020 | 12 | D1 p3 |  |
| F02-C0104 | Stationary Split | B | B084 |  | D1 p3 |  |
| F02-C0105 | Motion@D'n'B | C | C020 | 12 | D1 p3 |  |
| F02-C0106 |  | A | A021 | 12 | D1 p3 |  |
| F02-C0107 | - Groovy Jam - | A | A085 | 12 | D1 p3 |  |
| F02-C0108 | Goa Abduction | B | B021 | 12 | D1 p3 |  |
| F02-C0109 | >GROOVE.COM< | B | B085 |  | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0110 | Bs&Whirly Split1 | C | C021 | 12 | D1 p3 |  |
| F02-C0111 |  | A | A022 | 06 | D1 p3 |  |
| F02-C0112 | Irish Rover | A | A086 | 06 | D1 p3 |  |
| F02-C0113 | Tribal Dancer | B | B022 | 06 | D1 p3 |  |
| F02-C0114 | Accordion (SW) | B | B086 |  | D1 p3 |  |
| F02-C0115 | African Nights | C | C022 | 06 | D1 p3 |  |
| F02-C0116 |  | A | A023 | 03 | D1 p3 |  |
| F02-C0117 | Spicc.Camera Str | A | A087 | 03 | D1 p3 |  |
| F02-C0118 | AfterRainStrings | B | B023 | 03 | D1 p3 |  |
| F02-C0119 | Double Concerto2 | B | B087 |  | D1 p3 |  |
| F02-C0120 | Small Str Ens. 1 | C | C023 | 03 | D1 p3 |  |
| F02-C0121 |  | A | A024 | 10 | D1 p3 |  |
| F02-C0122 | Analog Clavical | A | A088 | 10 | D1 p3 |  |
| F02-C0123 | Mr.Trancer | B | B024 | 10 | D1 p3 |  |
| F02-C0124 | LFO Bit Poly | B | B088 |  | D1 p3 |  |
| F02-C0125 | UltimateAnaPizz | C | C024 | 10 | D1 p3 |  |
| F02-C0126 |  | A | A025 | 08 | D1 p3 |  |
| F02-C0127 | True Alien Love | A | A089 | 08 | D1 p3 |  |
| F02-C0128 | Universal Choir | B | B025 | 08 | D1 p3 |  |
| F02-C0129 | Love to Play | B | B089 |  | D1 p3 |  |
| F02-C0130 | MassTrance Pad | C | C025 | 08 | D1 p3 |  |
| F02-C0131 |  | A | A026 | 02 | D1 p3 |  |
| F02-C0132 | ChristmasStrings | A | A090 | 02 | D1 p3 |  |
| F02-C0133 | Orch.Pad & Bells | B | B026 | 02 | D1 p3 |  |
| F02-C0134 | Bells Of Glass | B | B090 |  | D1 p3 |  |
| F02-C0135 | FirstSnowFlakes | C | C026 | 02 | D1 p3 |  |
| F02-C0136 |  | A | A027 | 04 | D1 p3 |  |
| F02-C0137 | Lots o' Flutes | A | A091 | 04 | D1 p3 |  |
| F02-C0138 | DoubledOrchBrass | B | B027 | 04 | D1 p3 |  |
| F02-C0139 | HybridOrch.Brass | B | B091 |  | D1 p3 |  |
| F02-C0140 | Woods Soloists | C | C027 | 04 | D1 p3 |  |
| F02-C0141 |  | A | A028 | 11 | D1 p3 |  |
| F02-C0142 | Underwork | A | A092 | 11 | D1 p3 |  |
| F02-C0143 | Psycho Electro | B | B028 | 11 | D1 p3 |  |
| F02-C0144 | Hidden Rhythm | B | B092 |  | D1 p3 |  |
| F02-C0145 | Modern Machine | C | C028 | 11 | D1 p3 |  |
| F02-C0146 |  | A | A029 | 07 | D1 p3 |  |
| F02-C0147 | Acoustics & Str | A | A093 | 07 | D1 p3 |  |
| F02-C0148 | WichitaFalls Gtr | B | B029 | 07 | D1 p3 |  |
| F02-C0149 | Alchemy Layer | B | B093 |  | D1 p3 |  |
| F02-C0150 | Classical Guitar | C | C029 | 07 | D1 p3 |  |
| F02-C0151 |  | A | A030 | 01 | D1 p3 |  |
| F02-C0152 | Old Jazz CX3 | A | A094 | 01 | D1 p3 |  |
| F02-C0153 | The Big Opera | B | B030 | 01 | D1 p3 |  |
| F02-C0154 | Pipes&Choir | B | B094 |  | D1 p3 |  |
| F02-C0155 | Upper&Lower Org. | C | C030 | 01 | D1 p3 |  |
| F02-C0156 |  | A | A031 | 13 | D1 p3 |  |
| F02-C0157 | Trash Aliens | A | A095 | 13 | D1 p3 |  |
| F02-C0158 | Chord Hold | B | B031 | 13 | D1 p3 |  |
| F02-C0159 | Haunted House | B | B095 |  | D1 p3 |  |
| F02-C0160 | Major Fantasy | C | C031 | 15 | D1 p3 |  |
| F02-C0161 |  | A | A032 | 00 | D1 p3 |  |
| F02-C0162 | Steel&HumanCords | A | A096 | 00 | D1 p3 |  |
| F02-C0163 | GrandPiano n Str | B | B032 | 00 | D1 p3 |  |
| F02-C0164 | Mega Keys | B | B096 |  | D1 p3 |  |
| F02-C0165 | Piano&Strings 2 | C | C032 | 00 | D1 p3 |  |
| F02-C0166 |  | A | A033 | 05 | D1 p3 |  |
| F02-C0167 | RedCanyon Split | A | A097 | 05 | D1 p3 |  |
| F02-C0168 | Choral Symphony | B | B033 | 05 | D1 p3 |  |
| F02-C0169 | DynOrchestra | B | B097 |  | D1 p3 |  |
| F02-C0170 | Orchestra ff | C | C033 | 05 | D1 p3 |  |
| F02-C0171 |  | A | A034 | 09 | D1 p3 |  |
| F02-C0172 | The Lost Sea | A | A098 | 09 | D1 p3 |  |
| F02-C0173 | Sunrise @ IBIZA | B | B034 | 09 | D1 p3 |  |
| F02-C0174 | AssimilatedArps | B | B098 |  | D1 p3 |  |
| F02-C0175 | The Orion Nebula | C | C034 | 09 | D1 p3 |  |
| F02-C0176 |  | A | A035 | 14 | D1 p3 |  |
| F02-C0177 | Trance Anthem | A | A099 | 14 | D1 p3 |  |
| F02-C0178 | Urban Sweet RnB | B | B035 | 12 | D1 p3 |  |
| F02-C0179 | Goanna... | B | B099 |  | D1 p3 |  |
| F02-C0180 | ] Electrified [ | C | C035 | 14 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0181 |  | A | A036 | 12 | D1 p3 |  |
| F02-C0182 | Village Queen | A | A100 | 12 | D1 p3 |  |
| F02-C0183 | AC Gtr Pops | B | B036 | 12 | D1 p3 |  |
| F02-C0184 | Trance Power | B | B100 |  | D1 p3 |  |
| F02-C0185 | Century Elektro | C | C036 | 12 | D1 p3 |  |
| F02-C0186 |  | A | A037 | 12 | D1 p3 |  |
| F02-C0187 | Jazzy CX3 Split | A | A101 | 12 | D1 p3 |  |
| F02-C0188 | Gig Split | B | B037 | 12 | D1 p3 |  |
| F02-C0189 | Clavi-Jammer | B | B101 |  | D1 p3 |  |
| F02-C0190 | Radio Ribbonizer | C | C037 | 12 | D1 p3 |  |
| F02-C0191 |  | A | A038 | 06 | D1 p3 |  |
| F02-C0192 | World Atmosphere | A | A102 | 06 | D1 p3 |  |
| F02-C0193 | New Age Orch. | B | B038 | 05 | D1 p3 |  |
| F02-C0194 | Classic Combi | B | B102 |  | D1 p3 |  |
| F02-C0195 | Adventure | C | C038 | 05 | D1 p3 |  |
| F02-C0196 |  | A | A039 | 03 | D1 p3 |  |
| F02-C0197 | Bowed Strings | A | A103 | 03 | D1 p3 |  |
| F02-C0198 | 30 Strings | B | B039 | 03 | D1 p3 |  |
| F02-C0199 | Allegro Strings | B | B103 |  | D1 p3 |  |
| F02-C0200 | Trem Bows <vel> | C | C039 | 03 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0201 |  | A | A040 | 10 | D1 p3 |  |
| F02-C0202 | Huge Ober Sweep | A | A104 | 10 | D1 p3 |  |
| F02-C0203 | Ghost Planet | B | B040 | 10 | D1 p3 |  |
| F02-C0204 | Syzzle Synth | B | B104 |  | D1 p3 |  |
| F02-C0205 | High Sync Lead | C | C040 | 10 | D1 p3 |  |
| F02-C0206 |  | A | A041 | 08 | D1 p3 |  |
| F02-C0207 | Breath Monster | A | A105 | 08 | D1 p3 |  |
| F02-C0208 | Harp&Voice Pad | B | B041 | 08 | D1 p3 |  |
| F02-C0209 | <Analog World> | B | B105 |  | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0210 | Box Office | C | C041 | 05 | D1 p3 |  |
| F02-C0211 |  | A | A042 | 02 | D1 p3 |  |
| F02-C0212 | VelocityMallets | A | A106 | 02 | D1 p3 |  |
| F02-C0213 | Emtim Bells | B | B042 | 02 | D1 p3 |  |
| F02-C0214 | Elemental | B | B106 |  | D1 p3 |  |
| F02-C0215 | PizzPiano n Xylo | C | C042 | 05 | D1 p3 |  |
| F02-C0216 |  | A | A043 | 04 | D1 p3 |  |
| F02-C0217 | Full mf Brass 2 | A | A107 | 04 | D1 p3 |  |
| F02-C0218 | Brass/3-Octaves | B | B043 | 04 | D1 p3 |  |
| F02-C0219 | Brash Tpts&Choir | B | B107 |  | D1 p3 |  |
| F02-C0220 | CuttingSax&Brass | C | C043 | 04 | D1 p3 |  |
| F02-C0221 |  | A | A044 | 11 | D1 p3 |  |
| F02-C0222 | Walking On Mars | A | A108 | 11 | D1 p3 |  |
| F02-C0223 | Jazz Step DnB | B | B044 | 11 | D1 p3 |  |
| F02-C0224 | Brass All Stars | B | B108 |  | D1 p3 |  |
| F02-C0225 | Tape Machine | C | C044 | 11 | D1 p3 |  |
| F02-C0226 |  | A | A045 | 07 | D1 p3 |  |
| F02-C0227 | Mellowed Out... | A | A109 | 07 | D1 p3 |  |
| F02-C0228 | FilterREZ JAM!! | B | B045 | 12 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0229 | Hollow Men | B | B109 |  | D1 p3 |  |
| F02-C0230 | Jazz Gtr. Trio | C | C045 | 11 | D1 p3 |  |
| F02-C0231 |  | A | A046 | 12 | D1 p3 |  |
| F02-C0232 | DirtyBX-3 Organ2 | A | A110 | 01 | D1 p3 |  |
| F02-C0233 | StPno&Org(Knbs)1 | B | B046 | 01 | D1 p3 |  |
| F02-C0234 | Comp Organ/Knobs | B | B110 |  | D1 p3 |  |
| F02-C0235 | Dirty Drawbars | C | C046 | 01 | D1 p3 |  |
| F02-C0236 |  | A | A047 | 15 | D1 p3 |  |
| F02-C0237 | Wild Orchestra | A | A111 | 15 | D1 p3 |  |
| F02-C0238 | Velocity SfzCue1 | B | B047 | 15 | D1 p3 |  |
| F02-C0239 | Mad Orchestra | B | B111 |  | D1 p3 |  |
| F02-C0240 | ChordBlocker | C | C047 | 14 | D1 p3 |  |
| F02-C0241 |  | A | A048 | 00 | D1 p3 |  |
| F02-C0242 | Delux EP Pad | A | A112 | 00 | D1 p3 |  |
| F02-C0243 | Amp Driven Wurly | B | B048 | 00 | D1 p3 |  |
| F02-C0244 | Real EP & Pad | B | B112 |  | D1 p3 |  |
| F02-C0245 | [Jam with Me!] | C | C048 | 00 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0246 |  | A | A049 | 05 | D1 p3 |  |
| F02-C0247 | SymphonicMixture | A | A113 | 05 | D1 p3 |  |
| F02-C0248 | Harpsichord&Orch | B | B049 | 05 | D1 p3 |  |
| F02-C0249 | Orchestral Brass | B | B113 |  | D1 p3 |  |
| F02-C0250 | OrchestraMaximus | C | C049 | 05 | D1 p3 |  |
| F02-C0251 |  | A | A050 | 09 | D1 p3 |  |
| F02-C0252 | Poppah | A | A114 | 09 | D1 p3 |  |
| F02-C0253 | Orbit The Galaxy | B | B050 | 09 | D1 p3 |  |
| F02-C0254 | ARP vrs. LFO | B | B114 |  | D1 p3 |  |
| F02-C0255 | Slow Motion Man | C | C050 | 09 | D1 p3 |  |
| F02-C0256 |  | A | A051 | 14 | D1 p3 |  |
| F02-C0257 | Acid Groover... | A | A115 | 14 | D1 p3 |  |
| F02-C0258 | Vocodah | B | B051 | 14 | D1 p3 |  |
| F02-C0259 | 90's House | B | B115 |  | D1 p3 |  |
| F02-C0260 | ResonaceParty | C | C051 | 14 | D1 p3 |  |
| F02-C0261 |  | A | A052 | 11 | D1 p3 |  |
| F02-C0262 | Jungle Cowboy | A | A116 | 12 | D1 p3 |  |
| F02-C0263 | Try Some Tekno | B | B052 | 12 | D1 p3 |  |
| F02-C0264 | Future Funk | B | B116 |  | D1 p3 |  |
| F02-C0265 | Micro Calculator | C | C052 | 12 | D1 p3 |  |
| F02-C0266 |  | A | A053 | 12 | D1 p3 |  |
| F02-C0267 | Phillydelic | A | A117 | 12 | D1 p3 |  |
| F02-C0268 | Organ Bass | B | B053 | 12 | D1 p3 |  |
| F02-C0269 | <Dream of Me> | B | B117 |  | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0270 | ~~Storm~Riders~~ | C | C053 | 12 | D1 p3 |  |
| F02-C0271 |  | A | A054 | 06 | D1 p3 |  |
| F02-C0272 | Uilleann Pipes | A | A118 | 06 | D1 p3 |  |
| F02-C0273 | Tropical Forrest | B | B054 | 06 | D1 p3 |  |
| F02-C0274 | Paris Springtime | B | B118 |  | D1 p3 |  |
| F02-C0275 | Heroics | C | C054 | 05 | D1 p3 |  |
| F02-C0276 |  | A | A055 | 03 | D1 p3 |  |
| F02-C0277 | >Smaller Ensbl< | A | A119 | 03 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0278 | Mixed StringBows | B | B055 | 03 | D1 p3 |  |
| F02-C0279 | Hidden Winds | B | B119 |  | D1 p3 |  |
| F02-C0280 | Small Organic | C | C055 | 05 | D1 p3 |  |
| F02-C0281 |  | A | A056 | 10 | D1 p3 |  |
| F02-C0282 | 80's Analogues | A | A120 | 10 | D1 p3 |  |
| F02-C0283 | Delay PadDance | B | B056 | 10 | D1 p3 |  |
| F02-C0284 | Interdic Pad | B | B120 |  | D1 p3 |  |
| F02-C0285 | SMASH-HIT! | C | C056 | 10 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0286 |  | A | A057 | 08 | D1 p3 |  |
| F02-C0287 | Simple Pad | A | A121 | 08 | D1 p3 |  |
| F02-C0288 | |Sailing to You| | B | B057 | 08 | D1 p3 |  |
| F02-C0289 | / Nice & Easy | B | B121 |  | D1 p3 |  |
| F02-C0290 | Kromatica | C | C057 | 09 | D1 p3 |  |
| F02-C0291 |  | A | A058 | 02 | D1 p3 |  |
| F02-C0292 | OrchBell&Strings | A | A122 | 02 | D1 p3 |  |
| F02-C0293 | Staccato! | B | B058 | 02 | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0294 | Pizzicato Xylo | B | B122 |  | D1 p3 |  |
| F02-C0295 | Bella Proach | C | C058 | 02 | D1 p3 |  |
| F02-C0296 |  | A | A059 | 04 | D1 p3 |  |
| F02-C0297 | FlutesClarinets1 | A | A123 | 04 | D1 p3 |  |
| F02-C0298 | Dynamic Orch. | B | B059 | 05 | D1 p3 |  |
| F02-C0299 | War Flick | B | B123 |  | D1 p3 |  |
| F02-C0300 | Double Winds | C | C059 | 04 | D1 p3 |  |
| F02-C0301 |  | A | A060 | 11 | D1 p3 |  |
| F02-C0302 | Cyber Dance | A | A124 | 11 | D1 p3 |  |
| F02-C0303 | Chill Factor | B | B060 | 11 | D1 p3 |  |
| F02-C0304 | Modern Ethnic | B | B124 |  | D1 p3 |  |
| F02-C0305 | Progre-Ballad | C | C060 | 11 | D1 p3 |  |
| F02-C0306 |  | A | A061 | 07 | D1 p3 |  |
| F02-C0307 | Guitars n Pads | A | A125 | 07 | D1 p3 |  |
| F02-C0308 | Harmonic12String | B | B061 | 07 | D1 p3 |  |
| F02-C0309 | Vintage12 String | B | B125 |  | D1 p3 |  |
| F02-C0310 | Bass/Gtrs n Pads | C | C061 | 12 | D1 p3 |  |
| F02-C0311 |  | A | A062 | 01 | D1 p3 |  |
| F02-C0312 | Mods Combo | A | A126 | 01 | D1 p3 |  |
| F02-C0313 | Pipes on SW&Knbs | B | B062 | 01 | D1 p3 |  |
| F02-C0314 | Pipes on Knob | B | B126 |  | D1 p3 |  |
| F02-C0315 | DirtyBX-3 Organ1 | C | C062 | 01 | D1 p3 |  |
| F02-C0316 |  | A | A063 | 13 | D1 p3 |  |
| F02-C0317 | Female Xpansions | A | A127 | 13 | D1 p3 |  |
| F02-C0318 | Tempo Seq. 1 | B | B063 | 13 | D1 p3 |  |
| F02-C0319 | Tempo Seq. | B | B127 |  | D1 p3 |  |
| F02-C0320 | Speed Racer | C | C063 | 15 | D1 p3 |  |
| F02-C0321 | ClassicPiano&Str | C | C064 | 00 | D1 p4 |  |
| F02-C0322 | Dynamic Stinger | D | D000 | 00 | D1 p4 |  |
| F02-C0323 | Hero's Piano | D | D064 | 00 | D1 p4 |  |
| F02-C0324 | Romance Layers | H | H000 | 00 | D1 p4 |  |
| F02-C0325 | The Rememberance | H | H064 | 00 | D1 p4 |  |
| F02-C0326 | DynOrchestra 2 | C | C065 | 05 | D1 p4 |  |
| F02-C0327 | Piano&Orchestra | D | D001 | 05 | D1 p4 |  |
| F02-C0328 | New Symphonia | D | D065 | 05 | D1 p4 |  |
| F02-C0329 | Heavy Atmosphere | H | H001 | 05 | D1 p4 |  |
| F02-C0330 | EXPStudioOrchSW1 | H | H065 | 05 | D1 p4 |  |
| F02-C0331 | Flashin' Cycle | C | C066 | 09 | D1 p4 |  |
| F02-C0332 | Transcend | D | D002 | 09 | D1 p4 |  |
| F02-C0333 | Nebular Motion | D | D066 | 09 | D1 p4 |  |
| F02-C0334 | Vocalist | H | H002 | 09 | D1 p4 |  |
| F02-C0335 | Squarehead | H | H066 | 09 | D1 p4 |  |
| F02-C0336 | Killer Tune | C | C067 | 14 | D1 p4 |  |
| F02-C0337 | Velo Psychedelic | D | D003 | 14 | D1 p4 |  |
| F02-C0338 | Digital Hardcore | D | D067 | 14 | D1 p4 |  |
| F02-C0339 | Goa-in For It | H | H003 | 14 | D1 p4 |  |
| F02-C0340 | 5th Movement | H | H067 | 14 | D1 p4 |  |
| F02-C0341 | Hit The Road | C | C068 | 12 | D1 p4 |  |
| F02-C0342 | LSI Electronics | D | D004 | 12 | D1 p4 |  |
| F02-C0343 | Elec Laboratory | D | D068 | 12 | D1 p4 |  |
| F02-C0344 | PsychedelicStomp | H | H004 | 12 | D1 p4 |  |
| F02-C0345 | Cell Phone City | H | H068 | 14 | D1 p4 |  |
| F02-C0346 | >Dr.Tude`sTribe< | C | C069 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0347 | Vocoder Garage | D | D005 | 12 | D1 p4 |  |
| F02-C0348 | The ILL Chill | D | D069 | 12 | D1 p4 |  |
| F02-C0349 | POP Princessa | H | H005 | 12 | D1 p4 |  |
| F02-C0350 | Future Days | H | H069 | 12 | D1 p4 |  |
| F02-C0351 | World Winds | C | C070 | 06 | D1 p4 |  |
| F02-C0352 | Indian Ocean | D | D006 | 06 | D1 p4 |  |
| F02-C0353 | Shogun | D | D070 | 06 | D1 p4 |  |
| F02-C0354 | Journey To Bali | H | H006 | 06 | D1 p4 |  |
| F02-C0355 | Cleopatra | H | H070 | 06 | D1 p4 |  |
| F02-C0356 | Double Concerto1 | C | C071 | 03 | D1 p4 |  |
| F02-C0357 | Stacc.StringOrch | D | D007 | 03 | D1 p4 |  |
| F02-C0358 | Plucked Strings | D | D071 | 03 | D1 p4 |  |
| F02-C0359 | Tinkly Strings | H | H007 | 03 | D1 p4 |  |
| F02-C0360 | Golden Strings | H | H071 | 03 | D1 p4 |  |
| F02-C0361 | Layered SynBrass | C | C072 | 10 | D1 p4 |  |
| F02-C0362 | Vintage Archive | D | D008 | 10 | D1 p4 |  |
| F02-C0363 | BleepageForDays | D | D072 | 10 | D1 p4 |  |
| F02-C0364 | Chord Memory | H | H008 | 10 | D1 p4 |  |
| F02-C0365 | Alpha Mega Synth | H | H072 | 10 | D1 p4 |  |
| F02-C0366 | =Liquified= | C | C073 | 08 | D1 p4 |  |
| F02-C0367 | Delay Up Pad | D | D009 | 08 | D1 p4 |  |
| F02-C0368 | The Night Sky | D | D073 | 08 | D1 p4 |  |
| F02-C0369 | Syn Choir w/bell | H | H009 | 08 | D1 p4 |  |
| F02-C0370 | > Nice'n Thin < | H | H073 | 08 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0371 | ConcertXylophone | C | C074 | 02 | D1 p4 |  |
| F02-C0372 | Random Blocks | D | D010 | 02 | D1 p4 |  |
| F02-C0373 | Bell Stories 1 | D | D074 | 02 | D1 p4 |  |
| F02-C0374 | Bell Amis | H | H010 | 02 | D1 p4 |  |
| F02-C0375 | Bellish Pad | H | H074 | 02 | D1 p4 |  |
| F02-C0376 | DynaHorns&Bones | C | C075 | 04 | D1 p4 |  |
| F02-C0377 | Sop/Alt/Tnr/Bari | D | D011 | 04 | D1 p4 |  |
| F02-C0378 | All Doublereeds | D | D075 | 04 | D1 p4 |  |
| F02-C0379 | Fat Horn Section | H | H011 | 04 | D1 p4 |  |
| F02-C0380 | EXPWoodwindGroup | H | H075 | 04 | D1 p4 |  |
| F02-C0381 | Morning Dove | C | C076 | 11 | D1 p4 |  |
| F02-C0382 | Mondo Waltz(Kb1) | D | D012 | 11 | D1 p4 |  |
| F02-C0383 | Tenor KG split | D | D076 | 11 | D1 p4 |  |
| F02-C0384 | Hey!<SW>Brothers | H | H012 | 11 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0385 | Midnight Town | H | H076 | 11 | D1 p4 |  |
| F02-C0386 | Country Walk | C | C077 | 07 | D1 p4 |  |
| F02-C0387 | The Harp | D | D013 | 07 | D1 p4 |  |
| F02-C0388 | Harp Dreams | D | D077 | 07 | D1 p4 |  |
| F02-C0389 | 4th GTR Lead | H | H013 | 07 | D1 p4 |  |
| F02-C0390 | Streetwalk | H | H077 | 12 | D1 p4 |  |
| F02-C0391 | Split ClickOrgan | C | C078 | 01 | D1 p4 |  |
| F02-C0392 | Funky CX3 Split | D | D014 | 01 | D1 p4 |  |
| F02-C0393 | Gospel Organ | D | D078 | 01 | D1 p4 |  |
| F02-C0394 | Jazzy "B" | H | H014 | 01 | D1 p4 |  |
| F02-C0395 | Organic Beats | H | H078 | 12 | D1 p4 |  |
| F02-C0396 | Vox-N-Jams | C | C079 | 13 | D1 p4 |  |
| F02-C0397 | Drums on SW&Knbs | D | D015 | 15 | D1 p4 |  |
| F02-C0398 | Talkin'House | D | D079 | 13 | D1 p4 |  |
| F02-C0399 | Hip <SW> Hop | H | H015 | 15 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0400 | Hit onC4 | H | H079 | 15 | D1 p4 |  |
| F02-C0401 | TouchFunk | C | C080 | 00 | D1 p4 |  |
| F02-C0402 | Clav Reversal | D | D016 | 00 | D1 p4 |  |
| F02-C0403 | SKOOL of FUNK | D | D080 | 00 | D1 p4 |  |
| F02-C0404 | M1 Layer Keys | H | H016 | 00 | D1 p4 |  |
| F02-C0405 | Star Gazers | H | H080 | 00 | D1 p4 |  |
| F02-C0406 | Dyna Orch.Brass | C | C081 | 05 | D1 p4 |  |
| F02-C0407 | Pro-Orchestra | D | D017 | 05 | D1 p4 |  |
| F02-C0408 | Orchestra | D | D081 | 05 | D1 p4 |  |
| F02-C0409 | Velo Symphony | H | H017 | 05 | D1 p4 |  |
| F02-C0410 | Gotham Snd.Track | H | H081 | 05 | D1 p4 |  |
| F02-C0411 | What GEOS Around | C | C082 | 09 | D1 p4 |  |
| F02-C0412 | Digital Stream | D | D018 | 09 | D1 p4 |  |
| F02-C0413 | Counterpoint Arp | D | D082 | 09 | D1 p4 |  |
| F02-C0414 | Chorvoker | H | H018 | 09 | D1 p4 |  |
| F02-C0415 | Introspection | H | H082 | 09 | D1 p4 |  |
| F02-C0416 | Dry as Garage | C | C083 | 14 | D1 p4 |  |
| F02-C0417 | Groovy Garage | D | D019 | 14 | D1 p4 |  |
| F02-C0418 | ClubbinGarage | D | D083 | 14 | D1 p4 |  |
| F02-C0419 | Wave Sequencer | H | H019 | 14 | D1 p4 |  |
| F02-C0420 | The Rising Sun | H | H083 | 11 | D1 p4 |  |
| F02-C0421 | Red Zap Box | C | C084 | 12 | D1 p4 |  |
| F02-C0422 | Drive'n'Bass | D | D020 | 12 | D1 p4 |  |
| F02-C0423 | In The Room | D | D084 | 12 | D1 p4 |  |
| F02-C0424 | In the Sky | H | H020 | 12 | D1 p4 |  |
| F02-C0425 | Are U really U ? | H | H084 | 12 | D1 p4 |  |
| F02-C0426 | FutureJazztronic | C | C085 | 12 | D1 p4 |  |
| F02-C0427 | You Will Love It | D | D021 | 12 | D1 p4 |  |
| F02-C0428 | Latin Jazz (vel) | D | D085 | 12 | D1 p4 |  |
| F02-C0429 | Hard Groove | H | H021 | 12 | D1 p4 |  |
| F02-C0430 | PrgressivMonster | H | H085 | 12 | D1 p4 |  |
| F02-C0431 | Brassy Full Orch | C | C086 | 05 | D1 p4 |  |
| F02-C0432 | Quidam | D | D022 | 06 | D1 p4 |  |
| F02-C0433 | Guitar&Orchestra | D | D086 | 11 | D1 p4 |  |
| F02-C0434 | Magic Kingdom | H | H022 | 06 | D1 p4 |  |
| F02-C0435 | Oboe Split | H | H086 | 11 | D1 p4 |  |
| F02-C0436 | Smooth Strings2 | C | C087 | 03 | D1 p4 |  |
| F02-C0437 | Arc/TrmSplit- Va | D | D023 | 03 | D1 p4 |  |
| F02-C0438 | Arc/TrmSplit- Cl | D | D087 | 03 | D1 p4 |  |
| F02-C0439 | Smooth Str&Brs | H | H023 | 03 | D1 p4 |  |
| F02-C0440 | Bows and Bows | H | H087 | 03 | D1 p4 |  |
| F02-C0441 | Pop Pizza | C | C088 | 14 | D1 p4 |  |
| F02-C0442 | Infinity Pad | D | D024 | 10 | D1 p4 |  |
| F02-C0443 | Detune Massive | D | D088 | 10 | D1 p4 |  |
| F02-C0444 | Tape Mix Orch | H | H024 | 10 | D1 p4 |  |
| F02-C0445 | High Rizer | H | H088 | 10 | D1 p4 |  |
| F02-C0446 | SecretGroovePad | C | C089 | 08 | D1 p4 |  |
| F02-C0447 | Fly on bycicle! | D | D025 | 08 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0448 | Majestic Voice | D | D089 | 08 | D1 p4 |  |
| F02-C0449 | 3oct Tape Flute | H | H025 | 08 | D1 p4 |  |
| F02-C0450 | \Early Warning/ | H | H089 | 08 | D1 p4 |  |
| F02-C0451 | Deep Crystals | C | C090 | 02 | D1 p4 |  |
| F02-C0452 | Bella Releasimo | D | D026 | 02 | D1 p4 |  |
| F02-C0453 | -Fourier's Idea- | D | D090 | 02 | D1 p4 |  |
| F02-C0454 | Auto Mallets 1 | H | H026 | 02 | D1 p4 |  |
| F02-C0455 | Marimba Band | H | H090 | 02 | D1 p4 |  |
| F02-C0456 | EXP ReedEnsemble | C | C091 | 05 | D1 p4 |  |
| F02-C0457 | ClarinetEnsemble | D | D027 | 04 | D1 p4 |  |
| F02-C0458 | Moderato | D | D091 | 05 | D1 p4 |  |
| F02-C0459 | Muted Brass | H | H027 | 04 | D1 p4 |  |
| F02-C0460 | Band Brass Oct. | H | H091 | 04 | D1 p4 |  |
| F02-C0461 | Modern Rock <SW> | C | C092 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0462 | Free Way | D | D028 | 11 | D1 p4 |  |
| F02-C0463 | Back & 4th | D | D092 | 11 | D1 p4 |  |
| F02-C0464 | Taking Lead RY | H | H028 | 11 | D1 p4 |  |
| F02-C0465 | Good Old Days | H | H092 | 11 | D1 p4 |  |
| F02-C0466 | Teenage Poetry | C | C093 | 07 | D1 p4 |  |
| F02-C0467 | Harp Strum | D | D029 | 07 | D1 p4 |  |
| F02-C0468 | Second Movement | D | D093 | 05 | D1 p4 |  |
| F02-C0469 | HarmonrichGuitar | H | H029 | 07 | D1 p4 |  |
| F02-C0470 | Guitah Snappah | H | H093 | 07 | D1 p4 |  |
| F02-C0471 | Velo Perc Organ | C | C094 | 01 | D1 p4 |  |
| F02-C0472 | Warm Jazzy "B" | D | D030 | 01 | D1 p4 |  |
| F02-C0473 | Dirty Old B | D | D094 | 01 | D1 p4 |  |
| F02-C0474 | Dark2Bright CX3 | H | H030 | 01 | D1 p4 |  |
| F02-C0475 | Jazzy"B"Split | H | H094 | 12 | D1 p4 |  |
| F02-C0476 | Sfz&MinorPhrases | C | C095 | 15 | D1 p4 |  |
| F02-C0477 | DimensionalPiano | D | D031 | 13 | D1 p4 |  |
| F02-C0478 | Green Eyes | D | D095 | 12 | D1 p4 |  |
| F02-C0479 | Racing the Clock | H | H031 | 13 | D1 p4 |  |
| F02-C0480 | Dach Groove | H | H095 | 15 | D1 p4 |  |
| F02-C0481 | Piano&Strings 1 | C | C096 | 00 | D1 p4 |  |
| F02-C0482 | <Heavenly Boesy> | D | D032 | 00 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0483 | Ac.Piano Pad | D | D096 | 00 | D1 p4 |  |
| F02-C0484 | Ballad Layer EP | H | H032 | 00 | D1 p4 |  |
| F02-C0485 | Comp Stack | H | H096 | 00 | D1 p4 |  |
| F02-C0486 | Epic Orchestra | C | C097 | 05 | D1 p4 |  |
| F02-C0487 | Piano Symphonica | D | D033 | 05 | D1 p4 |  |
| F02-C0488 | VelocityConcerto | D | D097 | 05 | D1 p4 |  |
| F02-C0489 | SFZ Brass Octave | H | H033 | 04 | D1 p4 |  |
| F02-C0490 | Bows 'n Brass | H | H097 | 05 | D1 p4 |  |
| F02-C0491 | Talk to Galileo | C | C098 | 09 | D1 p4 |  |
| F02-C0492 | Shh-I'm Sweeping | D | D034 | 09 | D1 p4 |  |
| F02-C0493 | Vector Mod Pad | D | D098 | 09 | D1 p4 |  |
| F02-C0494 | Respirator | H | H034 | 09 | D1 p4 |  |
| F02-C0495 | Transtabbin | H | H098 | 09 | D1 p4 |  |
| F02-C0496 | Beyond the World | C | C099 | 14 | D1 p4 |  |
| F02-C0497 | TranceBuildSW1/2 | D | D035 | 14 | D1 p4 |  |
| F02-C0498 | >AutoRave< Knobs | D | D099 | 14 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0499 | Intro... | H | H035 | 14 | D1 p4 |  |
| F02-C0500 | DryPuffyTrance | H | H099 | 14 | D1 p4 |  |
| F02-C0501 | >ARAGone-Tribe<< | C | C100 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0502 | Muted Space Pad | D | D036 | 12 | D1 p4 |  |
| F02-C0503 | Reactant Groove | D | D100 | 12 | D1 p4 |  |
| F02-C0504 | U no U Want It | H | H036 | 12 | D1 p4 |  |
| F02-C0505 | U DO Tekno ? | H | H100 | 12 | D1 p4 |  |
| F02-C0506 | Straight Ahead | C | C101 | 11 | D1 p4 |  |
| F02-C0507 | Smooth Split | D | D037 | 12 | D1 p4 |  |
| F02-C0508 | DJ Scratch-Knob1 | D | D101 | 12 | D1 p4 |  |
| F02-C0509 | Laid-Back-Split | H | H037 | 12 | D1 p4 |  |
| F02-C0510 | Bass & PianoPad | H | H101 | 12 | D1 p4 |  |
| F02-C0511 | Record Recorder | C | C102 | 05 | D1 p4 |  |
| F02-C0512 | Symphony & Choir | D | D038 | 05 | D1 p4 |  |
| F02-C0513 | Horns & Strings2 | D | D102 | 05 | D1 p4 |  |
| F02-C0514 | Funky Goa<SW2> | H | H038 | 11 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0515 | Wind of Asia/SW1 | H | H102 | 11 | D1 p4 |  |
| F02-C0516 | Renaissance | C | C103 | 05 | D1 p4 |  |
| F02-C0517 | VeloPizzPluckSW1 | D | D039 | 03 | D1 p4 |  |
| F02-C0518 | Pizz Octaves | D | D103 | 03 | D1 p4 |  |
| F02-C0519 | Cinematic Love | H | H039 | 05 | D1 p4 |  |
| F02-C0520 | Full Orch | H | H103 | 05 | D1 p4 |  |
| F02-C0521 | Split_UH | C | C104 | 10 | D1 p4 |  |
| F02-C0522 | Mystic Presence | D | D040 | 09 | D1 p4 |  |
| F02-C0523 | Rhythmatic | D | D104 | 09 | D1 p4 |  |
| F02-C0524 | Beauty/Beast | H | H040 | 11 | D1 p4 |  |
| F02-C0525 | Out On The Floor | H | H104 | 12 | D1 p4 |  |
| F02-C0526 | =Dark MoviePad= | C | C105 | 08 | D1 p4 |  |
| F02-C0527 | Ocean Traveler | D | D041 | 09 | D1 p4 |  |
| F02-C0528 | <Deep in Space> | D | D105 | 09 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0529 | Marianis | H | H041 | 09 | D1 p4 |  |
| F02-C0530 | Carl's Cosmos | H | H105 | 09 | D1 p4 |  |
| F02-C0531 | Pizz & Timpani | C | C106 | 05 | D1 p4 |  |
| F02-C0532 | Behind Beyond | D | D042 | 00 | D1 p4 |  |
| F02-C0533 | Wine & Cheese | D | D106 | 00 | D1 p4 |  |
| F02-C0534 | <[Mystic Night]> | H | H042 | 09 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0535 | Battle Field | H | H106 | 05 | D1 p4 |  |
| F02-C0536 | TouchOrch w/flt | C | C107 | 05 | D1 p4 |  |
| F02-C0537 | Brass/2-Octaves | D | D043 | 04 | D1 p4 |  |
| F02-C0538 | Woodwind&Strings | D | D107 | 05 | D1 p4 |  |
| F02-C0539 | Bad Brass Band | H | H043 | 04 | D1 p4 |  |
| F02-C0540 | Big Bad Brass 2 | H | H107 | 04 | D1 p4 |  |
| F02-C0541 | Sax Club | C | C108 | 11 | D1 p4 |  |
| F02-C0542 | Jazz't Arrived | D | D044 | 12 | D1 p4 |  |
| F02-C0543 | Instant Harmony | D | D108 | 12 | D1 p4 |  |
| F02-C0544 | YoureShaftedMate | H | H044 | 12 | D1 p4 |  |
| F02-C0545 | Club NYC | H | H108 | 12 | D1 p4 |  |
| F02-C0546 | -Hybr.Guit&Arp.- | C | C109 | 07 | D1 p4 |  |
| F02-C0547 | Sugar Plums | D | D045 | 05 | D1 p4 |  |
| F02-C0548 | Pompus Brass | D | D109 | 05 | D1 p4 |  |
| F02-C0549 | Tremolo Guitar | H | H045 | 12 | D1 p4 |  |
| F02-C0550 | Sand Dunes | H | H109 | 14 | D1 p4 |  |
| F02-C0551 | BX-3 Click Organ | C | C110 | 01 | D1 p4 |  |
| F02-C0552 | EXP Knob Organ | D | D046 | 01 | D1 p4 |  |
| F02-C0553 | Old Tubes! | D | D110 | 01 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0554 | RockOrgan Splits | H | H046 | 12 | D1 p4 |  |
| F02-C0555 | Digital'n'Bass | H | H110 | 12 | D1 p4 |  |
| F02-C0556 | Trancendental 2 | C | C111 | 14 | D1 p4 |  |
| F02-C0557 | ClassicOrchHits | D | D047 | 15 | D1 p4 |  |
| F02-C0558 | Dual Orch. Kits | D | D111 | 15 | D1 p4 |  |
| F02-C0559 | < Wah-Solo-Jam | H | H047 | 11 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0560 | Lost Soles... ow | H | H111 | 14 | D1 p4 |  |
| F02-C0561 | Hip Hop Clav 1 | C | C112 | 00 | D1 p4 |  |
| F02-C0562 | Big Bottom Clav | D | D048 | 00 | D1 p4 |  |
| F02-C0563 | Hangin' Out | D | D112 | 00 | D1 p4 |  |
| F02-C0564 | Ballad Grand | H | H048 | 00 | D1 p4 |  |
| F02-C0565 | Flying Piano | H | H112 | 00 | D1 p4 |  |
| F02-C0566 | Dragon Orchestra | C | C113 | 05 | D1 p4 |  |
| F02-C0567 | Studio Orchestra | D | D049 | 05 | D1 p4 |  |
| F02-C0568 | Ether Woodwinds | D | D113 | 05 | D1 p4 |  |
| F02-C0569 | Rhythms & Bows | H | H049 | 05 | D1 p4 |  |
| F02-C0570 | WestPhilharmonic | H | H113 | 05 | D1 p4 |  |
| F02-C0571 | Quadrangle | C | C114 | 09 | D1 p4 |  |
| F02-C0572 | ThrobbingFingers | D | D050 | 09 | D1 p4 |  |
| F02-C0573 | TimeOut... | D | D114 | 09 | D1 p4 |  |
| F02-C0574 | Phases Of Angels | H | H050 | 09 | D1 p4 |  |
| F02-C0575 | (Where are You?) | H | H114 | 09 | D1 p4 |  |
| F02-C0576 | Manic 3AM | C | C115 | 14 | D1 p4 |  |
| F02-C0577 | Real Big Beat | D | D051 | 14 | D1 p4 |  |
| F02-C0578 | >>MYZone-Tribe<< | D | D115 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0579 | Wavestation | H | H051 | 09 | D1 p4 |  |
| F02-C0580 | -> Solo @ Pad | H | H115 | 11 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0581 | Remix Factory | C | C116 | 12 | D1 p4 |  |
| F02-C0582 | *ResoChord Pad* | D | D052 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0583 | Mouth Tracker | D | D116 | 12 | D1 p4 |  |
| F02-C0584 | Invisible Sun | H | H052 | 12 | D1 p4 |  |
| F02-C0585 | [BIGBANG] | H | H116 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0586 | Rap Bass Split | C | C117 | 12 | D1 p4 |  |
| F02-C0587 | When It Rains... | D | D053 | 12 | D1 p4 |  |
| F02-C0588 | Vocal Transit | D | D117 | 12 | D1 p4 |  |
| F02-C0589 | 2 Finger Magik | H | H053 | 12 | D1 p4 |  |
| F02-C0590 | Confirmation | H | H117 | 12 | D1 p4 |  |
| F02-C0591 | Dark March | C | C118 | 05 | D1 p4 |  |
| F02-C0592 | My Island Girl | D | D054 | 11 | D1 p4 |  |
| F02-C0593 | Pad & Saxine | D | D118 | 11 | D1 p4 |  |
| F02-C0594 | Home Suite Home | H | H054 | 11 | D1 p4 |  |
| F02-C0595 | SweetBabyBlues | H | H118 | 11 | D1 p4 |  |
| F02-C0596 | Strings Of Silk | C | C119 | 03 | D1 p4 |  |
| F02-C0597 | Arc/TrmSplit- Vi | D | D055 | 03 | D1 p4 |  |
| F02-C0598 | Arc/TrmSplit- Bs | D | D119 | 03 | D1 p4 |  |
| F02-C0599 | EXP Strings 1 | H | H055 | 03 | D1 p4 |  |
| F02-C0600 | Smoothie Strings | H | H119 | 03 | D1 p4 |  |
| F02-C0601 | SlowGroover | C | C120 | 14 | D1 p4 |  |
| F02-C0602 | Trance Blade | D | D056 | 10 | D1 p4 |  |
| F02-C0603 | SMASH-HIT! | D | D120 | 10 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0604 | Fire Ant Parade | H | H056 | 10 | D1 p4 |  |
| F02-C0605 | Gaters | H | H120 | 10 | D1 p4 |  |
| F02-C0606 | Know Trance | C | C121 | 14 | D1 p4 |  |
| F02-C0607 | Forget-Me-Not | D | D057 | 09 | D1 p4 |  |
| F02-C0608 | Northern Stars | D | D121 | 09 | D1 p4 |  |
| F02-C0609 | Ambient World | H | H057 | 12 | D1 p4 |  |
| F02-C0610 | Gloomy Night | H | H121 | 12 | D1 p4 |  |
| F02-C0611 | Angelic Bells | C | C122 | 02 | D1 p4 |  |
| F02-C0612 | Viotory is Nigh! | D | D058 | 09 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0613 | Starlight Bell | D | D122 | 02 | D1 p4 |  |
| F02-C0614 | Stereo Mallets | H | H058 | 02 | D1 p4 |  |
| F02-C0615 | Mallet Happy | H | H122 | 02 | D1 p4 |  |
| F02-C0616 | Flutes & Bassoon | C | C123 | 04 | D1 p4 |  |
| F02-C0617 | Film Mix | D | D059 | 05 | D1 p4 |  |
| F02-C0618 | Strings&Winds | D | D123 | 05 | D1 p4 |  |
| F02-C0619 | Velo Brass&Reeds | H | H059 | 04 | D1 p4 |  |
| F02-C0620 | ModernBrassSect. | H | H123 | 04 | D1 p4 |  |
| F02-C0621 | Cyber Gunmen | C | C124 | 12 | D1 p4 |  |
| F02-C0622 | The Horizon | D | D060 | 11 | D1 p4 |  |
| F02-C0623 | Overture Split | D | D124 | 11 | D1 p4 |  |
| F02-C0624 | Split in yer eye | H | H060 | 11 | D1 p4 |  |
| F02-C0625 | Mystic Lead | H | H124 | 11 | D1 p4 |  |
| F02-C0626 | * GiTronica * | C | C125 | 12 | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0627 | Plucked Glass | D | D061 | 07 | D1 p4 |  |
| F02-C0628 | Third Movement | D | D125 | 05 | D1 p4 |  |
| F02-C0629 | Harmonic Whisper | H | H061 | 07 | D1 p4 |  |
| F02-C0630 | Guitarbell | H | H125 | 07 | D1 p4 |  |
| F02-C0631 | Rock Organ | C | C126 | 01 | D1 p4 |  |
| F02-C0632 | Old Vox Organ | D | D062 | 01 | D1 p4 |  |
| F02-C0633 | Knob as Drawbar1 | D | D126 | 01 | D1 p4 |  |
| F02-C0634 | Space Wah Bass | H | H062 | 12 | D1 p4 |  |
| F02-C0635 | Gimme a ring... | H | H126 | 12 | D1 p4 |  |
| F02-C0636 | Drumfest | C | C127 | 15 | D1 p4 |  |
| F02-C0637 | Floor Acid | D | D063 | 14 | D1 p4 |  |
| F02-C0638 | Robot Factory | D | D127 | 13 | D1 p4 |  |
| F02-C0639 | Delayed Film Cue | H | H063 | 15 | D1 p4 |  |
| F02-C0640 | Orch. Film Cue | H | H127 | 15 | D1 p4 |  |
| F02-C0641 | Wide L/R Piano | I | I000 | 00 | D1 p5 |  |
| F02-C0642 | Pianocean | I | I064 | 00 | D1 p5 |  |
| F02-C0643 | Famous Layer EP | J | J000 | 00 | D1 p5 |  |
| F02-C0644 | Liquidpiano | J | J064 | 00 | D1 p5 |  |
| F02-C0645 | Glassy Layers | K | K000 | 00 | D1 p5 |  |
| F02-C0646 | Full Orchestra | I | I001 | 05 | D1 p5 |  |
| F02-C0647 | Movie Tool Orch. | I | I065 | 05 | D1 p5 |  |
| F02-C0648 | ClassicOrchestra | J | J001 | 05 | D1 p5 |  |
| F02-C0649 | Orchestral Pad | J | J065 | 05 | D1 p5 |  |
| F02-C0650 | Orchestral Blend | K | K001 | 05 | D1 p5 |  |
| F02-C0651 | Swept Away | I | I002 | 09 | D1 p5 |  |
| F02-C0652 | Splashdown | I | I066 | 09 | D1 p5 |  |
| F02-C0653 | } I SAW GOD { | J | J002 | 09 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0654 | Gated Padding | J | J066 | 09 | D1 p5 |  |
| F02-C0655 | PerkyVibeThang | K | K002 | 09 | D1 p5 |  |
| F02-C0656 | Night Flight | I | I003 | 14 | D1 p5 |  |
| F02-C0657 | OOD Techno | I | I067 | 14 | D1 p5 |  |
| F02-C0658 | Power Bar | J | J003 | 14 | D1 p5 |  |
| F02-C0659 | Nights Of Fire | J | J067 | 14 | D1 p5 |  |
| F02-C0660 | Sea Wave | K | K003 | 14 | D1 p5 |  |
| F02-C0661 | 5th EP House | I | I004 | 12 | D1 p5 |  |
| F02-C0662 | < Sneakin > | I | I068 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0663 | Capuirea Core | J | J004 | 12 | D1 p5 |  |
| F02-C0664 | Snap!ReggaeOrgan | J | J068 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0665 | Galactic Artcore | K | K004 | 12 | D1 p5 |  |
| F02-C0666 | SynthBass & Pitz | I | I005 | 12 | D1 p5 |  |
| F02-C0667 | Getcha Pimpin | I | I069 | 12 | D1 p5 |  |
| F02-C0668 | Strumin' 2step | J | J005 | 12 | D1 p5 |  |
| F02-C0669 | Wack Attack | J | J069 | 12 | D1 p5 |  |
| F02-C0670 | Trick House | K | K005 | 12 | D1 p5 |  |
| F02-C0671 | Fisatic Dreams | I | I006 | 06 | D1 p5 |  |
| F02-C0672 | Night Reeds | I | I070 | 11 | D1 p5 |  |
| F02-C0673 | Spanish Trip | J | J006 | 06 | D1 p5 |  |
| F02-C0674 | Woody Split | J | J070 | 05 | D1 p5 |  |
| F02-C0675 | Peasant Song | K | K006 | 06 | D1 p5 |  |
| F02-C0676 | Orch. Pizz (SW1) | I | I007 | 03 | D1 p5 |  |
| F02-C0677 | SuperOct Str ST | I | I071 | 03 | D1 p5 |  |
| F02-C0678 | Low Bows | J | J007 | 03 | D1 p5 |  |
| F02-C0679 | String Orch. | J | J071 | 03 | D1 p5 |  |
| F02-C0680 | Marcato/Trem Orc | K | K007 | 03 | D1 p5 |  |
| F02-C0681 | Analoge Story | I | I008 | 10 | D1 p5 |  |
| F02-C0682 | The Compin' Man | I | I072 | 10 | D1 p5 |  |
| F02-C0683 | RAIZZI | J | J008 | 10 | D1 p5 |  |
| F02-C0684 | Hybrid BellPad | J | J072 | 10 | D1 p5 |  |
| F02-C0685 | ResonatorVox | K | K008 | 10 | D1 p5 |  |
| F02-C0686 | Symphonica Vox | I | I009 | 08 | D1 p5 |  |
| F02-C0687 | Soft, yet smooth | I | I073 | 08 | D1 p5 |  |
| F02-C0688 | Symphonica | J | J009 | 08 | D1 p5 |  |
| F02-C0689 | Med Orch w/Choir | J | J073 | 05 | D1 p5 |  |
| F02-C0690 | Lost In Love | K | K009 | 08 | D1 p5 |  |
| F02-C0691 | MIDI Mallets | I | I010 | 02 | D1 p5 |  |
| F02-C0692 | UltraAnoString | I | I074 | 09 | D1 p5 |  |
| F02-C0693 | Christmas Eve | J | J010 | 02 | D1 p5 |  |
| F02-C0694 | Cascade Bells | J | J074 | 02 | D1 p5 |  |
| F02-C0695 | Christmas | K | K010 | 02 | D1 p5 |  |
| F02-C0696 | Sax Banquet | I | I011 | 04 | D1 p5 |  |
| F02-C0697 | Strings & Brass1 | I | I075 | 05 | D1 p5 |  |
| F02-C0698 | Full mf Brass 3 | J | J011 | 04 | D1 p5 |  |
| F02-C0699 | French Brass | J | J075 | 04 | D1 p5 |  |
| F02-C0700 | The Power Trio | K | K011 | 04 | D1 p5 |  |
| F02-C0701 | Brass Jazz Funk | I | I012 | 11 | D1 p5 |  |
| F02-C0702 | OrganVel->Brass | I | I076 | 11 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0703 | Easy Mondo Night | J | J012 | 11 | D1 p5 |  |
| F02-C0704 | Analogsteroid | J | J076 | 11 | D1 p5 |  |
| F02-C0705 | Mute Tp & More | K | K012 | 11 | D1 p5 |  |
| F02-C0706 | Rich Deep Wet | I | I013 | 07 | D1 p5 |  |
| F02-C0707 | Killer Smile | I | I077 | 12 | D1 p5 |  |
| F02-C0708 | Band Of Power | J | J013 | 07 | D1 p5 |  |
| F02-C0709 | Dark Bell EP | J | J077 | 00 | D1 p5 |  |
| F02-C0710 | ClassicGtr&Orch. | K | K013 | 07 | D1 p5 |  |
| F02-C0711 | Med.->Tutti/Knbs | I | I014 | 01 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0712 | Med.Mixture/Knbs | I | I078 | 01 | D1 p5 |  |
| F02-C0713 | 3rd Harm.on Knob | J | J014 | 01 | D1 p5 |  |
| F02-C0714 | XF-Split | J | J078 | 05 | D1 p5 |  |
| F02-C0715 | Gospel Church | K | K014 | 01 | D1 p5 |  |
| F02-C0716 | Hard Core Party | I | I015 | 15 | D1 p5 |  |
| F02-C0717 | Scratch Street | I | I079 | 15 | D1 p5 |  |
| F02-C0718 | VoiceRhythmBox | J | J015 | 15 | D1 p5 |  |
| F02-C0719 | PursuitOfJustice | J | J079 | 14 | D1 p5 |  |
| F02-C0720 | Velocity SfzCue2 | K | K015 | 15 | D1 p5 |  |
| F02-C0721 | Standard EP | I | I016 | 00 | D1 p5 |  |
| F02-C0722 | Tiney Harmonic | I | I080 | 00 | D1 p5 |  |
| F02-C0723 | >Ballade Stack< | J | J016 | 00 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0724 | Mellow Keys | J | J080 | 00 | D1 p5 |  |
| F02-C0725 | Wurli+Pad | K | K016 | 00 | D1 p5 |  |
| F02-C0726 | Presto & Vivace | I | I017 | 05 | D1 p5 |  |
| F02-C0727 | Stream E.Piano | I | I081 | 00 | D1 p5 |  |
| F02-C0728 | EXPSymphony | J | J017 | 05 | D1 p5 |  |
| F02-C0729 | Orch. Dynamics | J | J081 | 05 | D1 p5 |  |
| F02-C0730 | Dream Orchestra | K | K017 | 05 | D1 p5 |  |
| F02-C0731 | PS6 Gated Pad | I | I018 | 09 | D1 p5 |  |
| F02-C0732 | Galactic Travels | I | I082 | 09 | D1 p5 |  |
| F02-C0733 | Scooby's Doom | J | J018 | 09 | D1 p5 |  |
| F02-C0734 | =(Empty Train)= | J | J082 | 09 | D1 p5 |  |
| F02-C0735 | Horror Film | K | K018 | 09 | D1 p5 |  |
| F02-C0736 | Sweep-phonic | I | I019 | 14 | D1 p5 |  |
| F02-C0737 | Cloud Travel | I | I083 | 14 | D1 p5 |  |
| F02-C0738 | Vocoder MS2K | J | J019 | 14 | D1 p5 |  |
| F02-C0739 | >ElectroMusic< | J | J083 | 14 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0740 | Elektro Express | K | K019 | 14 | D1 p5 |  |
| F02-C0741 | HardHouse | I | I020 | 12 | D1 p5 |  |
| F02-C0742 | Acid House | I | I084 | 12 | D1 p5 |  |
| F02-C0743 | Hard House | J | J020 | 12 | D1 p5 |  |
| F02-C0744 | Xperiment YZ52 | J | J084 | 12 | D1 p5 |  |
| F02-C0745 | Heavy Groove | K | K020 | 12 | D1 p5 |  |
| F02-C0746 | Spring Wind | I | I021 | 12 | D1 p5 |  |
| F02-C0747 | Dark Element | I | I085 | 12 | D1 p5 |  |
| F02-C0748 | Trance Classics | J | J021 | 12 | D1 p5 |  |
| F02-C0749 | Late Night Mood | J | J085 | 12 | D1 p5 |  |
| F02-C0750 | Dream Trance | K | K021 | 12 | D1 p5 |  |
| F02-C0751 | SplitFrThoseDays | I | I022 | 06 | D1 p5 |  |
| F02-C0752 | Dry Chamber Ens. | I | I086 | 05 | D1 p5 |  |
| F02-C0753 | Delhicacy (vel) | J | J022 | 06 | D1 p5 |  |
| F02-C0754 | VeloReed Strings | J | J086 | 05 | D1 p5 |  |
| F02-C0755 | The Waltz | K | K022 | 06 | D1 p5 |  |
| F02-C0756 | Flute/Pizz/Bells | I | I023 | 03 | D1 p5 |  |
| F02-C0757 | Detached Bows | I | I087 | 03 | D1 p5 |  |
| F02-C0758 | Smooth Str&Brs | J | J023 | 03 | D1 p5 |  |
| F02-C0759 | Smooth Strings3 | J | J087 | 03 | D1 p5 |  |
| F02-C0760 | Bows&KnobHarpsi | K | K023 | 03 | D1 p5 |  |
| F02-C0761 | Cosmic Dust | I | I024 | 10 | D1 p5 |  |
| F02-C0762 | Petit Lounge | I | I088 | 10 | D1 p5 |  |
| F02-C0763 | All StringSynths | J | J024 | 10 | D1 p5 |  |
| F02-C0764 | Hot House Pad | J | J088 | 10 | D1 p5 |  |
| F02-C0765 | Twilight Strings | K | K024 | 10 | D1 p5 |  |
| F02-C0766 | Voyager | I | I025 | 08 | D1 p5 |  |
| F02-C0767 | Small Movie | I | I089 | 05 | D1 p5 |  |
| F02-C0768 | Synth Bell Pad | J | J025 | 08 | D1 p5 |  |
| F02-C0769 | TheNightBefore.. | J | J089 | 08 | D1 p5 |  |
| F02-C0770 | Choir Ensemble | K | K025 | 08 | D1 p5 |  |
| F02-C0771 | Z1 Bells | I | I026 | 02 | D1 p5 |  |
| F02-C0772 | Clarity | I | I090 | 02 | D1 p5 |  |
| F02-C0773 | Bell Animations | J | J026 | 02 | D1 p5 |  |
| F02-C0774 | OFF >>> BELL | J | J090 | 13 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0775 | ] Glass Shop [ | K | K026 | 02 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0776 | Tp/Tb/Asax/Tsax | I | I027 | 04 | D1 p5 |  |
| F02-C0777 | Brass Cresc. | I | I091 | 04 | D1 p5 |  |
| F02-C0778 | F.Horn Ensemble | J | J027 | 04 | D1 p5 |  |
| F02-C0779 | Smooth Brass | J | J091 | 04 | D1 p5 |  |
| F02-C0780 | WWind Solo Insts | K | K027 | 04 | D1 p5 |  |
| F02-C0781 | Voodoo SynthLead | I | I028 | 11 | D1 p5 |  |
| F02-C0782 | Chill Ending | I | I092 | 11 | D1 p5 |  |
| F02-C0783 | Comical Alien | J | J028 | 11 | D1 p5 |  |
| F02-C0784 | Syn Brass Attack | J | J092 | 11 | D1 p5 |  |
| F02-C0785 | TRITON C.I.D. | K | K028 | 11 | D1 p5 |  |
| F02-C0786 | The Guitarist | I | I029 | 07 | D1 p5 |  |
| F02-C0787 | Str&Harpsichord | I | I093 | 05 | D1 p5 |  |
| F02-C0788 | Get With It(sw1) | J | J029 | 07 | D1 p5 |  |
| F02-C0789 | Hardwired Mood | J | J093 | 11 | D1 p5 |  |
| F02-C0790 | Harp & Orchestra | K | K029 | 07 | D1 p5 |  |
| F02-C0791 | Wedding Day | I | I030 | 01 | D1 p5 |  |
| F02-C0792 | Registrations | I | I094 | 01 | D1 p5 |  |
| F02-C0793 | Jazz/GospelOrgan | J | J030 | 01 | D1 p5 |  |
| F02-C0794 | Delta Dawn | J | J094 | 09 | D1 p5 |  |
| F02-C0795 | Heavy Organ | K | K030 | 01 | D1 p5 |  |
| F02-C0796 | PsychedelicAmb | I | I031 | 13 | D1 p5 |  |
| F02-C0797 | Fellow Humans, | I | I095 | 13 | D1 p5 |  |
| F02-C0798 | Breaks Step | J | J031 | 13 | D1 p5 |  |
| F02-C0799 | Hard Hitting | J | J095 | 15 | D1 p5 |  |
| F02-C0800 | Feel Wide Bass | K | K031 | 13 | D1 p5 |  |
| F02-C0801 | Irie PianoMan | I | I032 | 00 | D1 p5 |  |
| F02-C0802 | Reversible Piano | I | I096 | 00 | D1 p5 |  |
| F02-C0803 | C.Piano & O.Pad | J | J032 | 00 | D1 p5 |  |
| F02-C0804 | Movie PianoPad | J | J096 | 00 | D1 p5 |  |
| F02-C0805 | Shiny DigiPiano | K | K032 | 00 | D1 p5 |  |
| F02-C0806 | Xpanded Overture | I | I033 | 05 | D1 p5 |  |
| F02-C0807 | Movie HornsSplit | I | I097 | 05 | D1 p5 |  |
| F02-C0808 | Orchestral Bells | J | J033 | 05 | D1 p5 |  |
| F02-C0809 | SuperSmoothOrch. | J | J097 | 05 | D1 p5 |  |
| F02-C0810 | Expressive Orch | K | K033 | 05 | D1 p5 |  |
| F02-C0811 | Sailing | I | I034 | 09 | D1 p5 |  |
| F02-C0812 | SQ Heart | I | I098 | 09 | D1 p5 |  |
| F02-C0813 | * Time Crisis * | J | J034 | 09 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0814 | Klik Padd | J | J098 | 09 | D1 p5 |  |
| F02-C0815 | //Feel ;-) Pad// | K | K034 | 09 | D1 p5 |  |
| F02-C0816 | >TRIZone-Tribe<< | I | I035 | 14 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0817 | Syncro Sonic | I | I099 | 14 | D1 p5 |  |
| F02-C0818 | Hard at it | J | J035 | 14 | D1 p5 |  |
| F02-C0819 | SW1/2 Breakdown | J | J099 | 14 | D1 p5 |  |
| F02-C0820 | Patho's Song | K | K035 | 14 | D1 p5 |  |
| F02-C0821 | Garage AP House | I | I036 | 12 | D1 p5 |  |
| F02-C0822 | Str&EP House | I | I100 | 12 | D1 p5 |  |
| F02-C0823 | Spicy Split | J | J036 | 12 | D1 p5 |  |
| F02-C0824 | Yo Quiero Salsa! | J | J100 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0825 | D'nB 'n'AllMySon | K | K036 | 14 | D1 p5 |  |
| F02-C0826 | Spiral Design | I | I037 | 12 | D1 p5 |  |
| F02-C0827 | Prog. Lounge Act | I | I101 | 12 | D1 p5 |  |
| F02-C0828 | <Bellezza> | J | J037 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0829 | Future/Retro | J | J101 | 12 | D1 p5 |  |
| F02-C0830 | <T-nage-Dancing> | K | K037 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0831 | Split For Oboe | I | I038 | 11 | D1 p5 |  |
| F02-C0832 | Ethno Board | I | I102 | 11 | D1 p5 |  |
| F02-C0833 | J-Taste Split | J | J038 | 06 | D1 p5 |  |
| F02-C0834 | Delhi Gates | J | J102 | 10 | D1 p5 |  |
| F02-C0835 | Military Salute | K | K038 | 05 | D1 p5 |  |
| F02-C0836 | Vlins.Vlas.Celli | I | I039 | 03 | D1 p5 |  |
| F02-C0837 | Oct Marcato Str | I | I103 | 03 | D1 p5 |  |
| F02-C0838 | OrchestraStrings | J | J039 | 03 | D1 p5 |  |
| F02-C0839 | SyrupySynthStrs | J | J103 | 03 | D1 p5 |  |
| F02-C0840 | Virtual Strings | K | K039 | 03 | D1 p5 |  |
| F02-C0841 | Steely Keys | I | I040 | 10 | D1 p5 |  |
| F02-C0842 | Green Strings | I | I104 | 10 | D1 p5 |  |
| F02-C0843 | REZ IS KING | J | J040 | 10 | D1 p5 |  |
| F02-C0844 | Chewing Gun | J | J104 | 10 | D1 p5 |  |
| F02-C0845 | Tritonsphere | K | K040 | 10 | D1 p5 |  |
| F02-C0846 | Vision | I | I041 | 08 | D1 p5 |  |
| F02-C0847 | Please stay calm | I | I105 | 08 | D1 p5 |  |
| F02-C0848 | Strings & Voices | J | J041 | 05 | D1 p5 |  |
| F02-C0849 | Healing Pad | J | J105 | 08 | D1 p5 |  |
| F02-C0850 | >Relax Pad< | K | K041 | 08 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0851 | <Anabell Rain> | I | I042 | 08 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0852 | RomanticSparkles | I | I106 | 02 | D1 p5 |  |
| F02-C0853 | Holiday Bells | J | J042 | 02 | D1 p5 |  |
| F02-C0854 | Modern Monastery | J | J106 | 02 | D1 p5 |  |
| F02-C0855 | Deep In Dream... | K | K042 | 00 | D1 p5 |  |
| F02-C0856 | Wooden Brass | I | I043 | 04 | D1 p5 |  |
| F02-C0857 | Big BrassBoysOct | I | I107 | 04 | D1 p5 |  |
| F02-C0858 | LargeSpaceBrass | J | J043 | 04 | D1 p5 |  |
| F02-C0859 | Velo Orch Winds | J | J107 | 05 | D1 p5 |  |
| F02-C0860 | Bones&TrumpetsST | K | K043 | 04 | D1 p5 |  |
| F02-C0861 | Gimme Funk <SW1> | I | I044 | 11 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0862 | Future Funk 2 | I | I108 | 11 | D1 p5 |  |
| F02-C0863 | Space Lounge | J | J044 | 11 | D1 p5 |  |
| F02-C0864 | Big Boy | J | J108 | 11 | D1 p5 |  |
| F02-C0865 | Blues Bandits | K | K044 | 11 | D1 p5 |  |
| F02-C0866 | Panner&Sweeper | I | I045 | 12 | D1 p5 |  |
| F02-C0867 | Medica| Sp|it | I | I109 | 12 | D1 p5 |  |
| F02-C0868 | Vari-Textures | J | J045 | 09 | D1 p5 |  |
| F02-C0869 | Ethereal Piano | J | J109 | 00 | D1 p5 |  |
| F02-C0870 | Small Orchestra | K | K045 | 05 | D1 p5 |  |
| F02-C0871 | 3 Pipes On SW1/2 | I | I046 | 01 | D1 p5 |  |
| F02-C0872 | Orchestral Pipes | I | I110 | 01 | D1 p5 |  |
| F02-C0873 | JazzyChords/Knbs | J | J046 | 01 | D1 p5 |  |
| F02-C0874 | OldReeds&Strings | J | J110 | 05 | D1 p5 |  |
| F02-C0875 | Digi-EP-Choir | K | K046 | 00 | D1 p5 |  |
| F02-C0876 | Alien Jungle Jam | I | I047 | 15 | D1 p5 |  |
| F02-C0877 | Majority Rules | I | I111 | 15 | D1 p5 |  |
| F02-C0878 | Evolution | J | J047 | 14 | D1 p5 |  |
| F02-C0879 | Trippy Trance | J | J111 | 14 | D1 p5 |  |
| F02-C0880 | Major Power Hits | K | K047 | 15 | D1 p5 |  |
| F02-C0881 | Dream Time EP | I | I048 | 00 | D1 p5 |  |
| F02-C0882 | Wave Motion EP | I | I112 | 00 | D1 p5 |  |
| F02-C0883 | Synthy Keys | J | J048 | 00 | D1 p5 |  |
| F02-C0884 | LayerEP&Pad/Knbs | J | J112 | 00 | D1 p5 |  |
| F02-C0885 | Clavitizer | K | K048 | 00 | D1 p5 |  |
| F02-C0886 | Sonatina(Harpsi) | I | I049 | 05 | D1 p5 |  |
| F02-C0887 | Harpsi Ensemble | I | I113 | 05 | D1 p5 |  |
| F02-C0888 | Winner Orchestra | J | J049 | 05 | D1 p5 |  |
| F02-C0889 | Symphonic Sect.1 | J | J113 | 05 | D1 p5 |  |
| F02-C0890 | Classical Dreams | K | K049 | 05 | D1 p5 |  |
| F02-C0891 | Tritium | I | I050 | 09 | D1 p5 |  |
| F02-C0892 | Plasma Beings | I | I114 | 09 | D1 p5 |  |
| F02-C0893 | <<SYNERGY>> | J | J050 | 09 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0894 | ***Ice*Rain*** | J | J114 | 09 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0895 | Nefarious | K | K050 | 09 | D1 p5 |  |
| F02-C0896 | Voco Loco | I | I051 | 14 | D1 p5 |  |
| F02-C0897 | K A M P U S | I | I115 | 14 | D1 p5 |  |
| F02-C0898 | Modular System | J | J051 | 12 | D1 p5 |  |
| F02-C0899 | Surrender Now | J | J115 | 14 | D1 p5 |  |
| F02-C0900 | *Counter|Strike* | K | K051 | 14 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0901 | > Housemade < | I | I052 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0902 | Funky Jam | I | I116 | 12 | D1 p5 |  |
| F02-C0903 | Komplicate | J | J052 | 12 | D1 p5 |  |
| F02-C0904 | Radio Ctrl>Rbn | J | J116 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0905 | Reflexer | K | K052 | 12 | D1 p5 |  |
| F02-C0906 | Joy RIDE | I | I053 | 12 | D1 p5 |  |
| F02-C0907 | >End|of|Time< | I | I117 | 12 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0908 | Voice-Piano&Bass | J | J053 | 12 | D1 p5 |  |
| F02-C0909 | Blew Note (vel) | J | J117 | 12 | D1 p5 |  |
| F02-C0910 | Blood Pressure | K | K053 | 12 | D1 p5 |  |
| F02-C0911 | Love Theme | I | I054 | 05 | D1 p5 |  |
| F02-C0912 | Horns & Strings1 | I | I118 | 05 | D1 p5 |  |
| F02-C0913 | 8 Beat Split | J | J054 | 12 | D1 p5 |  |
| F02-C0914 | EXPVelocity Orch | J | J118 | 05 | D1 p5 |  |
| F02-C0915 | SectionClarinets | K | K054 | 05 | D1 p5 |  |
| F02-C0916 | Small Str Ens. 2 | I | I055 | 03 | D1 p5 |  |
| F02-C0917 | Deep Velvet | I | I119 | 03 | D1 p5 |  |
| F02-C0918 | Smooth Strings1 | J | J055 | 03 | D1 p5 |  |
| F02-C0919 | CinemaScore Str | J | J119 | 03 | D1 p5 |  |
| F02-C0920 | HeavenlyHarpArps | K | K055 | 07 | D1 p5 |  |
| F02-C0921 | +*VOYAGE*+ | I | I056 | 11 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0922 | Silk Road | I | I120 | 11 | D1 p5 |  |
| F02-C0923 | In The Pocket | J | J056 | 10 | D1 p5 |  |
| F02-C0924 | Cyber Stomp | J | J120 | 10 | D1 p5 |  |
| F02-C0925 | >Mutant Hop<Knbs | K | K056 | 10 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0926 | Bass/Airy Orch. | I | I057 | 05 | D1 p5 |  |
| F02-C0927 | Symphonic Voices | I | I121 | 08 | D1 p5 |  |
| F02-C0928 | Pad To-Die-For | J | J057 | 08 | D1 p5 |  |
| F02-C0929 | Triton Voices | J | J121 | 08 | D1 p5 |  |
| F02-C0930 | The Procedure | K | K057 | 08 | D1 p5 |  |
| F02-C0931 | Heaven's Bells | I | I058 | 02 | D1 p5 |  |
| F02-C0932 | Malletimbres | I | I122 | 02 | D1 p5 |  |
| F02-C0933 | Str, Brs & Bells | J | J058 | 05 | D1 p5 |  |
| F02-C0934 | Bellacoola | J | J122 | 09 | D1 p5 |  |
| F02-C0935 | Pegasus Piano | K | K058 | 00 | D1 p5 |  |
| F02-C0936 | Impact Brass | I | I059 | 04 | D1 p5 |  |
| F02-C0937 | Large Brass | I | I123 | 04 | D1 p5 |  |
| F02-C0938 | Dyna BrassOrk | J | J059 | 04 | D1 p5 |  |
| F02-C0939 | Warm Brass Ens. | J | J123 | 04 | D1 p5 |  |
| F02-C0940 | World of Winds | K | K059 | 04 | D1 p5 |  |
| F02-C0941 | Cyber Shaman | I | I060 | 11 | D1 p5 |  |
| F02-C0942 | _Black|Rain_ | I | I124 | 11 | D1 p5 |  |
| F02-C0943 | .....&I Runaway | J | J060 | 11 | D1 p5 |  |
| F02-C0944 | Groove Wah Bass | J | J124 | 11 | D1 p5 |  |
| F02-C0945 | StreetCatScratch | K | K060 | 11 | D1 p5 |  |
| F02-C0946 | Baroque Ensemble | I | I061 | 05 | D1 p5 |  |
| F02-C0947 | VeloPizz & Bells | I | I125 | 05 | D1 p5 |  |
| F02-C0948 | <Love the 80`s> | J | J061 | 00 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0949 | MisterY | J | J125 | 09 | D1 p5 |  |
| F02-C0950 | Silky Way | K | K061 | 03 | D1 p5 |  |
| F02-C0951 | Holy Pipes | I | I062 | 01 | D1 p5 |  |
| F02-C0952 | Winds & Voices | I | I126 | 05 | D1 p5 |  |
| F02-C0953 | Spheoretical | J | J062 | 09 | D1 p5 |  |
| F02-C0954 | Flanged Pad | J | J126 | 09 | D1 p5 |  |
| F02-C0955 | StPno&Org(Knbs)2 | K | K062 | 00 | D1 p5 |  |
| F02-C0956 | *Dying Star* | I | I063 | 13 | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0957 | The Rain Forest | I | I127 | 13 | D1 p5 |  |
| F02-C0958 | Re-Mix-Maven | J | J063 | 15 | D1 p5 |  |
| F02-C0959 | Hard Floor | J | J127 | 15 | D1 p5 |  |
| F02-C0960 | Floor Venus | K | K063 | 14 | D1 p5 |  |
| F02-C0961 | Double Keys | K | K064 |  | D1 p6 |  |
| F02-C0962 | Electroacustic | L | L000 |  | D1 p6 |  |
| F02-C0963 | Wait 4 Sunrise.. | L | L064 | 00 | D1 p6 |  |
| F02-C0964 | Perfect Partners | M | M000 | 00 | D1 p6 |  |
| F02-C0965 | Synthetic EP's | M | M064 | 00 | D1 p6 |  |
| F02-C0966 | Movie in a box | K | K065 |  | D1 p6 |  |
| F02-C0967 | ThunderOrchestra | L | L001 |  | D1 p6 |  |
| F02-C0968 | Harpsi Sinfonia | L | L065 | 05 | D1 p6 |  |
| F02-C0969 | VeloPhilharmonic | M | M001 | 05 | D1 p6 |  |
| F02-C0970 | Piano Concerto | M | M065 | 05 | D1 p6 |  |
| F02-C0971 | *Dance with | K | K066 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0972 | Wave Sequencer | L | L002 |  | D1 p6 |  |
| F02-C0973 | Autumn Chill | L | L066 | 09 | D1 p6 |  |
| F02-C0974 | Arp Factory 1 | M | M002 | 09 | D1 p6 |  |
| F02-C0975 | * Sunrize * | M | M066 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0976 | Misty Morning | K | K067 |  | D1 p6 |  |
| F02-C0977 | Epic House | L | L003 |  | D1 p6 |  |
| F02-C0978 | Nasal Passage | L | L067 | 14 | D1 p6 |  |
| F02-C0979 | DaDaDaa | M | M003 | 14 | D1 p6 |  |
| F02-C0980 | Lookin'4 Trouble | M | M067 | 14 | D1 p6 |  |
| F02-C0981 | Heart To Heart | K | K068 |  | D1 p6 |  |
| F02-C0982 | Two Step Garage | L | L004 |  | D1 p6 |  |
| F02-C0983 | DEEP in da House | L | L068 | 12 | D1 p6 |  |
| F02-C0984 | Progressive Code | M | M004 | 12 | D1 p6 |  |
| F02-C0985 | ARPG Player | M | M068 | 12 | D1 p6 |  |
| F02-C0986 | Funk Splits | K | K069 |  | D1 p6 |  |
| F02-C0987 | Sweet RnB Vibe | L | L005 |  | D1 p6 |  |
| F02-C0988 | Bunnies In Love | L | L069 | 12 | D1 p6 |  |
| F02-C0989 | Way/\Chillin | M | M005 | 12 | D1 p6 |  |
| F02-C0990 | Lonely Moon | M | M069 | 12 | D1 p6 |  |
| F02-C0991 | Big Movie | K | K070 |  | D1 p6 |  |
| F02-C0992 | Caribbean Mood | L | L006 |  | D1 p6 |  |
| F02-C0993 | Str Brass Winds | L | L070 | 05 | D1 p6 |  |
| F02-C0994 | Song Of Africa | M | M006 | 06 | D1 p6 |  |
| F02-C0995 | 2nd Movement | M | M070 | 05 | D1 p6 |  |
| F02-C0996 | Andante Strings | K | K071 |  | D1 p6 |  |
| F02-C0997 | Amazing Strings | L | L007 |  | D1 p6 |  |
| F02-C0998 | {Octave-Strings} | L | L071 | 03 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C0999 | WWW.ide.Bows | M | M007 | 03 | D1 p6 |  |
| F02-C1000 | StringsDetached1 | M | M071 | 03 | D1 p6 |  |
| F02-C1001 | It's Sine Tine! | K | K072 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1002 | =Sync to Drum | L | L008 |  | D1 p6 |  |
| F02-C1003 | The Green Mile | L | L072 | 10 | D1 p6 |  |
| F02-C1004 | =Sync to Drum | M | M008 | 10 | D1 p6 |  |
| F02-C1005 | InYourFaceHouse | M | M072 | 14 | D1 p6 |  |
| F02-C1006 | The BIG Pad | K | K073 |  | D1 p6 |  |
| F02-C1007 | Chillout Pad | L | L009 |  | D1 p6 |  |
| F02-C1008 | Medium Orch | L | L073 | 05 | D1 p6 |  |
| F02-C1009 | Syn Choir | M | M009 | 08 | D1 p6 |  |
| F02-C1010 | Calm Times | M | M073 | 05 | D1 p6 |  |
| F02-C1011 | Velocity Bell | K | K074 |  | D1 p6 |  |
| F02-C1012 | X-mas Alone | L | L010 |  | D1 p6 |  |
| F02-C1013 | Steam Factory | L | L074 | 09 | D1 p6 |  |
| F02-C1014 | Bell Stories 2 | M | M010 | 02 | D1 p6 |  |
| F02-C1015 | low~down~load~ | M | M074 | 12 | D1 p6 |  |
| F02-C1016 | Majestic Brass | K | K075 |  | D1 p6 |  |
| F02-C1017 | Soft Jazz Winds | L | L011 |  | D1 p6 |  |
| F02-C1018 | SpiccatoBrs(Arp) | L | L075 | 04 | D1 p6 |  |
| F02-C1019 | Saxpected =) | M | M011 | 04 | D1 p6 |  |
| F02-C1020 | Concert Brass | M | M075 | 04 | D1 p6 |  |
| F02-C1021 | Split Whiner | K | K076 |  | D1 p6 |  |
| F02-C1022 | Trip Hop Funk | L | L012 |  | D1 p6 |  |
| F02-C1023 | Awake All Night | L | L076 | 11 | D1 p6 |  |
| F02-C1024 | Jazz Electronica | M | M012 | 11 | D1 p6 |  |
| F02-C1025 | SmoothStr Winds1 | M | M076 | 05 | D1 p6 |  |
| F02-C1026 | EXP Strings | K | K077 |  | D1 p6 |  |
| F02-C1027 | Country Beat | L | L013 |  | D1 p6 |  |
| F02-C1028 | EXPReeds & Bows | L | L077 | 05 | D1 p6 |  |
| F02-C1029 | Mountainside | M | M013 | 07 | D1 p6 |  |
| F02-C1030 | <Martyrium> | M | M077 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1031 | Rotary EP | K | K078 |  | D1 p6 |  |
| F02-C1032 | Draw The Bars | L | L014 |  | D1 p6 |  |
| F02-C1033 | FonkyStomp | L | L078 | 12 | D1 p6 |  |
| F02-C1034 | Knob as Drawbar2 | M | M014 | 01 | D1 p6 |  |
| F02-C1035 | AfterBattleSplit | M | M078 | 05 | D1 p6 |  |
| F02-C1036 | Psycho Orchestra | K | K079 |  | D1 p6 |  |
| F02-C1037 | Bermuda Triangle | L | L015 |  | D1 p6 |  |
| F02-C1038 | All Was Lost | L | L079 | 13 | D1 p6 |  |
| F02-C1039 | EXPOrchPerc&Hits | M | M015 | 15 | D1 p6 |  |
| F02-C1040 | Eerie Things | M | M079 | 13 | D1 p6 |  |
| F02-C1041 | }Whirly<>Pad{ | K | K080 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1042 | Village Pump | L | L016 |  | D1 p6 |  |
| F02-C1043 | Cutting Clav | L | L080 | 00 | D1 p6 |  |
| F02-C1044 | Ambient Piano | M | M016 | 00 | D1 p6 |  |
| F02-C1045 | Latin Piano | M | M080 | 00 | D1 p6 |  |
| F02-C1046 | Orch. Film Drama | K | K081 |  | D1 p6 |  |
| F02-C1047 | Symphonic X-Fade | L | L017 |  | D1 p6 |  |
| F02-C1048 | The Final Round | L | L081 | 05 | D1 p6 |  |
| F02-C1049 | Dyn mf Orch. | M | M017 | 05 | D1 p6 |  |
| F02-C1050 | Modern Orchestra | M | M081 | 05 | D1 p6 |  |
| F02-C1051 | <Smoky Soft | K | K082 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1052 | Minimum System | L | L018 |  | D1 p6 |  |
| F02-C1053 | > Movies Theme | L | L082 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1054 | #<#BACTERIUM#># | M | M018 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1055 | Eternity | M | M082 | 09 | D1 p6 |  |
| F02-C1056 | Neo Dance | K | K083 |  | D1 p6 |  |
| F02-C1057 | HardStabbyTrance | L | L019 |  | D1 p6 |  |
| F02-C1058 | Electronic Era | L | L083 | 14 | D1 p6 |  |
| F02-C1059 | ArpegConsumption | M | M019 | 14 | D1 p6 |  |
| F02-C1060 | Swinging Party | M | M083 | 14 | D1 p6 |  |
| F02-C1061 | * Blood Cooker | K | K084 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1062 | Happy Groove! | L | L020 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1063 | Dawn Dancer | L | L084 | 12 | D1 p6 |  |
| F02-C1064 | Hard Project | M | M020 | 12 | D1 p6 |  |
| F02-C1065 | Sloppy Joe | M | M084 | 12 | D1 p6 |  |
| F02-C1066 | -SWEET GROOVE- | K | K085 |  | D1 p6 |  |
| F02-C1067 | Play The Trance | L | L021 |  | D1 p6 |  |
| F02-C1068 | >SlowTone-Tribe< | L | L085 | 12 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1069 | Fantasmagoria | M | M021 | 12 | D1 p6 |  |
| F02-C1070 | Wondermint | M | M085 | 12 | D1 p6 |  |
| F02-C1071 | Soundstage | K | K086 |  | D1 p6 |  |
| F02-C1072 | African Wind | L | L022 |  | D1 p6 |  |
| F02-C1073 | Brass & Choir | L | L086 | 05 | D1 p6 |  |
| F02-C1074 | Old World Split | M | M022 | 06 | D1 p6 |  |
| F02-C1075 | Baroque Manners | M | M086 | 05 | D1 p6 |  |
| F02-C1076 | Harp & Strings | K | K087 |  | D1 p6 |  |
| F02-C1077 | Smooth Str&Brs | L | L023 |  | D1 p6 |  |
| F02-C1078 | ROM PCM Strings | L | L087 | 03 | D1 p6 |  |
| F02-C1079 | Lo Pizz Pad | M | M023 | 03 | D1 p6 |  |
| F02-C1080 | Agitato Strings | M | M087 | 03 | D1 p6 |  |
| F02-C1081 | Digitalian | K | K088 |  | D1 p6 |  |
| F02-C1082 | Real OB | L | L024 |  | D1 p6 |  |
| F02-C1083 | Miami Virtue | L | L088 | 10 | D1 p6 |  |
| F02-C1084 | Sigma Strings | M | M024 | 10 | D1 p6 |  |
| F02-C1085 | FlutyLeadStack | M | M088 | 10 | D1 p6 |  |
| F02-C1086 | New Age Ragga | K | K089 |  | D1 p6 |  |
| F02-C1087 | Fuji-yama | L | L025 |  | D1 p6 |  |
| F02-C1088 | UniversalStrings | L | L089 | 03 | D1 p6 |  |
| F02-C1089 | Sacred Place | M | M025 | 08 | D1 p6 |  |
| F02-C1090 | > Flimmer Line | M | M089 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1091 | Brasspiano | K | K090 |  | D1 p6 |  |
| F02-C1092 | Going Home @ | L | L026 |  | D1 p6 |  |
| F02-C1093 | Majestic Moment | L | L090 | 11 | D1 p6 |  |
| F02-C1094 | Time limit !! | M | M026 | 02 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1095 | All Your Bass | M | M090 | 05 | D1 p6 |  |
| F02-C1096 | Woods Velo | K | K091 |  | D1 p6 |  |
| F02-C1097 | Bassoon & Oboe | L | L027 |  | D1 p6 |  |
| F02-C1098 | Alto Flute Split | L | L091 | 04 | D1 p6 |  |
| F02-C1099 | Oboe & SoftBrass | M | M027 | 04 | D1 p6 |  |
| F02-C1100 | SymphonicFanfare | M | M091 | 04 | D1 p6 |  |
| F02-C1101 | Hard House | K | K092 |  | D1 p6 |  |
| F02-C1102 | OLD Club Jazz | L | L028 |  | D1 p6 |  |
| F02-C1103 | [Fiordalisolo] | L | L092 | 11 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1104 | Mad Trip Hop | M | M028 | 11 | D1 p6 |  |
| F02-C1105 | Suspense Orch. | M | M092 | 05 | D1 p6 |  |
| F02-C1106 | CinematicStrings | K | K093 |  | D1 p6 |  |
| F02-C1107 | Wavetable Guitar | L | L029 |  | D1 p6 |  |
| F02-C1108 | Trancendental 1 | L | L093 | 14 | D1 p6 |  |
| F02-C1109 | My Baby's Asleep | M | M029 | 07 | D1 p6 |  |
| F02-C1110 | ATGater(SWBuild) | M | M093 | 14 | D1 p6 |  |
| F02-C1111 | Liquid E.Piano | K | K094 |  | D1 p6 |  |
| F02-C1112 | Rock Of Organ | L | L030 |  | D1 p6 |  |
| F02-C1113 | Sweet Dyn-Orch. | L | L094 | 05 | D1 p6 |  |
| F02-C1114 | 70sOrgan(K1-4) | M | M030 | 01 | D1 p6 |  |
| F02-C1115 | ArpeggioOrchTrem | M | M094 | 05 | D1 p6 |  |
| F02-C1116 | Floor Trance | K | K095 |  | D1 p6 |  |
| F02-C1117 | Male Xpansions | L | L031 |  | D1 p6 |  |
| F02-C1118 | 7th Contact | L | L095 | 13 | D1 p6 |  |
| F02-C1119 | Psycho Horrer | M | M031 | 13 | D1 p6 |  |
| F02-C1120 | Sargasso Sea | M | M095 | 13 | D1 p6 |  |
| F02-C1121 | Peace Songs | K | K096 |  | D1 p6 |  |
| F02-C1122 | Magic Pno Layer | L | L032 |  | D1 p6 |  |
| F02-C1123 | Big Torch Ballad | L | L096 | 00 | D1 p6 |  |
| F02-C1124 | <ElektroStatika> | M | M032 | 00 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1125 | Session Piano | M | M096 | 00 | D1 p6 |  |
| F02-C1126 | Orchestral Split | K | K097 |  | D1 p6 |  |
| F02-C1127 | Piano Exploit | L | L033 |  | D1 p6 |  |
| F02-C1128 | Pf Concerto | L | L097 | 05 | D1 p6 |  |
| F02-C1129 | Symphonic Sect.2 | M | M033 | 05 | D1 p6 |  |
| F02-C1130 | Brass Orchestra | M | M097 | 05 | D1 p6 |  |
| F02-C1131 | A New Dawn | K | K098 |  | D1 p6 |  |
| F02-C1132 | FourStar*Choice | L | L034 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1133 | DreamState... | L | L098 | 09 | D1 p6 |  |
| F02-C1134 | Crowd Pleaser | M | M034 | 09 | D1 p6 |  |
| F02-C1135 | In The rOund | M | M098 | 09 | D1 p6 |  |
| F02-C1136 | Planet of Love | K | K099 |  | D1 p6 |  |
| F02-C1137 | Jumpin' n Pumpin | L | L035 |  | D1 p6 |  |
| F02-C1138 | Pickled | L | L099 | 14 | D1 p6 |  |
| F02-C1139 | Love Me Tonight | M | M035 | 14 | D1 p6 |  |
| F02-C1140 | Crop Circling | M | M099 | 14 | D1 p6 |  |
| F02-C1141 | Missing Android | K | K100 |  | D1 p6 |  |
| F02-C1142 | Funkin... | L | L036 |  | D1 p6 |  |
| F02-C1143 | SeriousBleeper | L | L100 | 14 | D1 p6 |  |
| F02-C1144 | Pest Kontrol | M | M036 | 14 | D1 p6 |  |
| F02-C1145 | Bass &Silver Pad | M | M100 | 12 | D1 p6 |  |
| F02-C1146 | Freewheeling | K | K101 |  | D1 p6 |  |
| F02-C1147 | Tranzation/Knobs | L | L037 |  | D1 p6 |  |
| F02-C1148 | Talk 2 Me | L | L101 | 12 | D1 p6 |  |
| F02-C1149 | The Next Phase | M | M037 | 12 | D1 p6 |  |
| F02-C1150 | Rush Flower | M | M101 | 12 | D1 p6 |  |
| F02-C1151 | Horn String Ens. | K | K102 |  | D1 p6 |  |
| F02-C1152 | Str&Wood Velo | L | L038 |  | D1 p6 |  |
| F02-C1153 | A Few Winds | L | L102 | 05 | D1 p6 |  |
| F02-C1154 | NewReeds&Strings | M | M038 | 05 | D1 p6 |  |
| F02-C1155 | FrenchEnsemble | M | M102 | 05 | D1 p6 |  |
| F02-C1156 | 3-V.Switch-Strgs | K | K103 |  | D1 p6 |  |
| F02-C1157 | DivisiStringOrch | L | L039 |  | D1 p6 |  |
| F02-C1158 | Divisi Strings | L | L103 | 03 | D1 p6 |  |
| F02-C1159 | StringsDetached2 | M | M039 | 03 | D1 p6 |  |
| F02-C1160 | >Movie Strings< | M | M103 | 03 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1161 | Syn Brass Layer | K | K104 |  | D1 p6 |  |
| F02-C1162 | *Movin' Stars* | L | L040 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1163 | Neurological | L | L104 | 10 | D1 p6 |  |
| F02-C1164 | Edition ARP | M | M040 | 10 | D1 p6 |  |
| F02-C1165 | Did U See God | M | M104 | 10 | D1 p6 |  |
| F02-C1166 | Pad-a-mento | K | K105 |  | D1 p6 |  |
| F02-C1167 | Judgement Day | L | L041 |  | D1 p6 |  |
| F02-C1168 | Brass & Voices | L | L105 | 05 | D1 p6 |  |
| F02-C1169 | Digi SynPiano | M | M041 | 00 | D1 p6 |  |
| F02-C1170 | SmallXpandedOrch | M | M105 | 05 | D1 p6 |  |
| F02-C1171 | 80's Movie Theme | K | K106 |  | D1 p6 |  |
| F02-C1172 | The Oort Cloud | L | L042 |  | D1 p6 |  |
| F02-C1173 | StereoNewMallets | L | L106 | 02 | D1 p6 |  |
| F02-C1174 | VoxTron Trip | M | M042 | 12 | D1 p6 |  |
| F02-C1175 | Living Cycles | M | M106 | 12 | D1 p6 |  |
| F02-C1176 | Spiccato Brass | K | K107 |  | D1 p6 |  |
| F02-C1177 | Orch. Brass | L | L043 |  | D1 p6 |  |
| F02-C1178 | Brass Ens. mf | L | L107 | 04 | D1 p6 |  |
| F02-C1179 | 3 Brass mp (Arp) | M | M043 | 04 | D1 p6 |  |
| F02-C1180 | Warm Orch. Brass | M | M107 | 04 | D1 p6 |  |
| F02-C1181 | Electro Funk | K | K108 |  | D1 p6 |  |
| F02-C1182 | Hard Core Funk | L | L044 |  | D1 p6 |  |
| F02-C1183 | Modulator VOX | L | L108 | 11 | D1 p6 |  |
| F02-C1184 | Winds-n-Bows | M | M044 | 05 | D1 p6 |  |
| F02-C1185 | RhapsodyOnATheme | M | M108 | 05 | D1 p6 |  |
| F02-C1186 | ClassicAnaString | K | K109 |  | D1 p6 |  |
| F02-C1187 | XpandedPadString | L | L045 |  | D1 p6 |  |
| F02-C1188 | AllegroModerato | L | L109 | 03 | D1 p6 |  |
| F02-C1189 | <Lost in Sphere> | M | M045 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1190 | <<Dreamworld>> | M | M109 | 09 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1191 | SynPiano+SynStrg | K | K110 |  | D1 p6 |  |
| F02-C1192 | Moon Light | L | L046 |  | D1 p6 |  |
| F02-C1193 | TheMissingBleep | L | L110 | 14 | D1 p6 |  |
| F02-C1194 | String&Woodwinds | M | M046 | 05 | D1 p6 |  |
| F02-C1195 | Strings & Brass3 | M | M110 | 05 | D1 p6 |  |
| F02-C1196 | Suspense FilmCue | K | K111 |  | D1 p6 |  |
| F02-C1197 | Orch. Percussion | L | L047 |  | D1 p6 |  |
| F02-C1198 | AimForTheMonster | L | L111 | 13 | D1 p6 |  |
| F02-C1199 | Millennium Files | M | M047 | 13 | D1 p6 |  |
| F02-C1200 | Endless Loomy | M | M111 | 13 | D1 p6 |  |
| F02-C1201 | Chill w/my Piano | K | K112 |  | D1 p6 |  |
| F02-C1202 | =>Syn Clav<= | L | L048 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1203 | Acid Clav | L | L112 | 00 | D1 p6 |  |
| F02-C1204 | My Sassy Piano | M | M048 | 00 | D1 p6 |  |
| F02-C1205 | Layered A.Piano | M | M112 | 00 | D1 p6 |  |
| F02-C1206 | Velo Orch.&Piano | K | K113 |  | D1 p6 |  |
| F02-C1207 | EXP Dyno Orch. | L | L049 |  | D1 p6 |  |
| F02-C1208 | Hollywood Orch | L | L113 | 05 | D1 p6 |  |
| F02-C1209 | DramaticOrch.Cue | M | M049 | 05 | D1 p6 |  |
| F02-C1210 | Orch Perc & Hit | M | M113 | 05 | D1 p6 |  |
| F02-C1211 | Sky Line | K | K114 |  | D1 p6 |  |
| F02-C1212 | Millennium Pad | L | L050 |  | D1 p6 |  |
| F02-C1213 | Mad at the World | L | L114 | 09 | D1 p6 |  |
| F02-C1214 | National Pulse | M | M050 | 09 | D1 p6 |  |
| F02-C1215 | Midi Clock | M | M114 | 09 | D1 p6 |  |
| F02-C1216 | Dirty Stabbah | K | K115 |  | D1 p6 |  |
| F02-C1217 | Dem is Well Hard | L | L051 |  | D1 p6 |  |
| F02-C1218 | Zero Sleep | L | L115 | 14 | D1 p6 |  |
| F02-C1219 | The Architect | M | M051 | 14 | D1 p6 |  |
| F02-C1220 | Euphorious | M | M115 | 14 | D1 p6 |  |
| F02-C1221 | Can't Remember | K | K116 |  | D1 p6 |  |
| F02-C1222 | SquaredOrCubed? | L | L052 |  | D1 p6 |  |
| F02-C1223 | Digital Sun | L | L116 | 12 | D1 p6 |  |
| F02-C1224 | Network | M | M052 | 12 | D1 p6 |  |
| F02-C1225 | Healing True Man | M | M116 | 12 | D1 p6 |  |
| F02-C1226 | Mouse Paradiso! | K | K117 |  | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1227 | ) Beat Boy ( | L | L053 |  | D1 p6 |  |
| F02-C1228 | TheHeartHasIt... | L | L117 | 12 | D1 p6 |  |
| F02-C1229 | Temporal Lobe | M | M053 | 12 | D1 p6 |  |
| F02-C1230 | Strut Break | M | M117 | 12 | D1 p6 |  |
| F02-C1231 | Spiccato Orch. | K | K118 |  | D1 p6 |  |
| F02-C1232 | Ethno Sweeper | L | L054 |  | D1 p6 |  |
| F02-C1233 | Romantica | L | L118 | 05 | D1 p6 |  |
| F02-C1234 | Largo Orchestra | M | M054 | 05 | D1 p6 |  |
| F02-C1235 | Serenade | M | M118 | 05 | D1 p6 |  |
| F02-C1236 | Harpsi Velo | K | K119 |  | D1 p6 |  |
| F02-C1237 | Largo OctStrings | L | L055 |  | D1 p6 |  |
| F02-C1238 | Supreme Strings | L | L119 | 03 | D1 p6 |  |
| F02-C1239 | Dyna Marcato Str | M | M055 | 03 | D1 p6 |  |
| F02-C1240 | Super Strings | M | M119 | 03 | D1 p6 |  |
| F02-C1241 | LusciousClavinus | K | K120 |  | D1 p6 |  |
| F02-C1242 | Vintage Memory | L | L056 |  | D1 p6 |  |
| F02-C1243 | Perc.Synth Horns | L | L120 | 10 | D1 p6 |  |
| F02-C1244 | Noisy Funk Synth | M | M056 | 10 | D1 p6 |  |
| F02-C1245 | Comp-Pewter | M | M120 | 10 | D1 p6 |  |
| F02-C1246 | Large one | K | K121 |  | D1 p6 |  |
| F02-C1247 | WIDE StringPad | L | L057 |  | D1 p6 |  |
| F02-C1248 | Strings & Brass2 | L | L121 | 05 | D1 p6 |  |
| F02-C1249 | Stutter Pad | M | M057 | 09 | D1 p6 |  |
| F02-C1250 | SQ Jack | M | M121 | 09 | D1 p6 |  |
| F02-C1251 | Gate Away... | K | K122 |  | D1 p6 |  |
| F02-C1252 | PrayingForPeace | L | L058 |  | D1 p6 |  |
| F02-C1253 | Splittah | L | L122 | 11 | D1 p6 |  |
| F02-C1254 | Clarinet&Strings | M | M058 | 05 | D1 p6 |  |
| F02-C1255 | Strings & Flutes | M | M122 | 05 | D1 p6 |  |
| F02-C1256 | FlutesClarinets2 | K | K123 |  | D1 p6 |  |
| F02-C1257 | Flutes&Dbl Reeds | L | L059 |  | D1 p6 |  |
| F02-C1258 | Clarinets & Bows | L | L123 | 05 | D1 p6 |  |
| F02-C1259 | Largo Brass | M | M059 | 04 | D1 p6 |  |
| F02-C1260 | Wild Nights | M | M123 | 14 | D1 p6 |  |
| F02-C1261 | Death Groove | K | K124 |  | D1 p6 |  |
| F02-C1262 | Kaleidophonic | L | L060 |  | D1 p6 |  |
| F02-C1263 | Aquarium Gaze~~~ | L | L124 | 11 | D1 p6 |  |
| F02-C1264 | SpaceFlyOrkestra | M | M060 | 05 | D1 p6 |  |
| F02-C1265 | SplitForSlowSolo | M | M124 | 05 | D1 p6 |  |
| F02-C1266 | Doubled Strings | K | K125 |  | D1 p6 |  |
| F02-C1267 | LC Nagger | L | L061 |  | D1 p6 |  |
| F02-C1268 | >>Dancing-Zone<< | L | L125 | 14 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1269 | Hard Bleeper | M | M061 | 14 | D1 p6 |  |
| F02-C1270 | ClassicGarage | M | M125 | 14 | D1 p6 |  |
| F02-C1271 | Cosmic Frontiers | K | K126 |  | D1 p6 |  |
| F02-C1272 | SmoothStr Winds2 | L | L062 |  | D1 p6 |  |
| F02-C1273 | Velo Orchestra | L | L126 | 05 | D1 p6 |  |
| F02-C1274 | R.BladeDigiOrch. | M | M062 | 05 | D1 p6 |  |
| F02-C1275 | Orch.Ending>C4 | M | M126 | 05 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F02-C1276 | Floor Analog | K | K127 |  | D1 p6 |  |
| F02-C1277 | um, Where am | L | L063 |  | D1 p6 |  |
| F02-C1278 | Other Dimension | L | L127 | 13 | D1 p6 |  |
| F02-C1279 | / Smelly Oooz \ | M | M063 | 13 | D1 p6 |  |
| F02-C1280 | ===>>Finale<<=== | M | M127 | 13 | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |

## Programs

Type is the document's own single/double program flag. Bank G holds the GM bank; its `g(1)`-`g(9)` / `g(d)` rows are the document's own variant entries and are listed here exactly as printed.

| Shipped label | Documented name | Bank | Number | Type | Category | Source | Note |
|---|---|---|---|---|---|---|---|
| F02-P0001 | BD Grand Concert | A | A000 | double | 00 | D1 p7 |  |
| F02-P0002 | Stage Bell EP | A | A064 | double | 00 | D1 p7 |  |
| F02-P0003 | A.Gtr Vel Gliss. | B | B000 | single | 07 | D1 p7 |  |
| F02-P0004 | E.Bass Ray | B | B064 | double | 08 | D1 p7 |  |
| F02-P0005 | Virtual Electro | C | C000 | double | 10 | D1 p7 |  |
| F02-P0006 | Concert Grand | A | A001 | double | 00 | D1 p7 |  |
| F02-P0007 | Hard Dyno EP | A | A065 | double | 00 | D1 p7 |  |
| F02-P0008 | Nu N.Guitar 1 | B | B001 | double | 07 | D1 p7 |  |
| F02-P0009 | Live Bass | B | B065 | double | 08 | D1 p7 |  |
| F02-P0010 | BuzzzWorthy | C | C001 | double | 10 | D1 p7 |  |
| F02-P0011 | Jazz/Rock Piano | A | A002 | double | 00 | D1 p7 |  |
| F02-P0012 | Suit.&Stage EP | A | A066 | double | 00 | D1 p7 |  |
| F02-P0013 | Hotel Unplugged | B | B002 | single | 07 | D1 p7 |  |
| F02-P0014 | Power Slap Bs | B | B066 | single | 08 | D1 p7 |  |
| F02-P0015 | Invert Direction | C | C002 | double | 10 | D1 p7 |  |
| F02-P0016 | Rockin' Piano | A | A003 | double | 00 | D1 p7 |  |
| F02-P0017 | Velo ST Stage | A | A067 | double | 00 | D1 p7 |  |
| F02-P0018 | Chiming12-String | B | B003 | double | 07 | D1 p7 |  |
| F02-P0019 | Vel Slap Bass | B | B067 | single | 08 | D1 p7 |  |
| F02-P0020 | MMG Brass | C | C003 | double | 10 | D1 p7 |  |
| F02-P0021 | Power Piano | A | A004 | double | 00 | D1 p7 |  |
| F02-P0022 | Hollow Tine EP | A | A068 | double | 00 | D1 p7 |  |
| F02-P0023 | Velo E.Guitar | B | B004 | double | 07 | D1 p7 |  |
| F02-P0024 | PhatBassHarmonic | B | B068 | single | 08 | D1 p7 |  |
| F02-P0025 | Brass Oct Synth | C | C004 | double | 10 | D1 p7 |  |
| F02-P0026 | PopBallad Piano1 | A | A005 | double | 00 | D1 p7 |  |
| F02-P0027 | Phaser Vint.EP | A | A069 | double | 00 | D1 p7 |  |
| F02-P0028 | Old E.Guitar | B | B005 | double | 07 | D1 p7 |  |
| F02-P0029 | Stereo Blue Sax | B | B069 | double | 06 | D1 p7 |  |
| F02-P0030 | Nu Goa Synth | C | C005 | double | 10 | D1 p7 |  |
| F02-P0031 | Piano & Strings | A | A006 | double | 00 | D1 p7 |  |
| F02-P0032 | Wah Wurly 1(-Y) | A | A070 | double | 00 | D1 p7 |  |
| F02-P0033 | Heavy Lead Gt. | B | B006 | double | 07 | D1 p7 |  |
| F02-P0034 | AltoSax X-fade | B | B070 | double | 06 | D1 p7 |  |
| F02-P0035 | 6oow Bass | C | C006 | double | 08 | D1 p7 |  |
| F02-P0036 | Snowy Piano | A | A007 | double | 00 | D1 p7 |  |
| F02-P0037 | Nu Wurli | A | A071 | double | 00 | D1 p7 |  |
| F02-P0038 | Celtic Harp 2 | B | B007 | double | 07 | D1 p7 |  |
| F02-P0039 | Alto Sax Hard | B | B071 | double | 06 | D1 p7 |  |
| F02-P0040 | Pulse Bass | C | C007 | double | 08 | D1 p7 |  |
| F02-P0041 | 70's E. Grand | A | A008 | double | 00 | D1 p7 |  |
| F02-P0042 | Rockin' Whirly | A | A072 | double | 00 | D1 p7 |  |
| F02-P0043 | Nu 'coustic Bass | B | B008 | single | 08 | D1 p7 |  |
| F02-P0044 | Alto Sax Duo | B | B072 | double | 06 | D1 p7 |  |
| F02-P0045 | Saw Bass | C | C008 | double | 08 | D1 p7 |  |
| F02-P0046 | SG-1D Piano | A | A009 | single | 00 | D1 p7 |  |
| F02-P0047 | Velo Clav 2 | A | A073 | single | 00 | D1 p7 |  |
| F02-P0048 | Nu'Finger Bs 1 | B | B009 | single | 08 | D1 p7 |  |
| F02-P0049 | Nu TenorSax | B | B073 | double | 06 | D1 p7 |  |
| F02-P0050 | Mondo Sync Bass | C | C009 | double | 08 | D1 p7 |  |
| F02-P0051 | Aggressive Pad | A | A010 | double | 00 | D1 p7 |  |
| F02-P0052 | Clav 3 Fonky | A | A074 | double | 00 | D1 p7 |  |
| F02-P0053 | Picked Bass | B | B010 | single | 08 | D1 p7 |  |
| F02-P0054 | Joystick SAX | B | B074 | single | 06 | D1 p7 |  |
| F02-P0055 | Bass-Depot | C | C010 | double | 08 | D1 p7 |  |
| F02-P0056 | St Trem EP | A | A011 | single | 00 | D1 p7 |  |
| F02-P0057 | Clav 3 Muted | A | A075 | single | 00 | D1 p7 |  |
| F02-P0058 | Open Slap Bs | B | B011 | single | 08 | D1 p7 |  |
| F02-P0059 | TenorSax Soft | B | B075 | double | 06 | D1 p7 |  |
| F02-P0060 | DetuneTranceLead | C | C011 | double | 11 | D1 p7 |  |
| F02-P0061 | Suit.&Stage EP | A | A012 | double | 00 | D1 p7 |  |
| F02-P0062 | Velo Clav 1 | A | A076 | single | 00 | D1 p7 |  |
| F02-P0063 | Blue Alto Sax | B | B012 | single | 06 | D1 p7 |  |
| F02-P0064 | TenorSaxX-fade | B | B076 | double | 06 | D1 p7 |  |
| F02-P0065 | Saw Lead/Bass | C | C012 | double | 11 | D1 p7 |  |
| F02-P0066 | Velo ST Suit. EP | A | A013 | double | 00 | D1 p7 |  |
| F02-P0067 | Clav 2 Velo-Wah | A | A077 | single | 00 | D1 p7 |  |
| F02-P0068 | Soul Tenor Sax | B | B013 | single | 06 | D1 p7 |  |
| F02-P0069 | TenorSax Hard | B | B077 | double | 06 | D1 p7 |  |
| F02-P0070 | MiniKORG700 | C | C013 | single | 11 | D1 p7 |  |
| F02-P0071 | EP Pad Sweep | A | A014 | double | 00 | D1 p7 |  |
| F02-P0072 | Clav 1 Mute SW#1 | A | A078 | single | 00 | D1 p7 |  |
| F02-P0073 | FascinateSoprano | B | B014 | single | 06 | D1 p7 |  |
| F02-P0074 | Tenor Sax Duo | B | B078 | double | 06 | D1 p7 |  |
| F02-P0075 | 800DV Lead | C | C014 | double | 11 | D1 p7 |  |
| F02-P0076 | Wah Wurly 2(SW1) | A | A015 | double | 00 | D1 p7 |  |
| F02-P0077 | Funk Vel Clav | A | A079 | double | 00 | D1 p7 |  |
| F02-P0078 | Saxes in Band | B | B015 | single | 06 | D1 p7 |  |
| F02-P0079 | Soprano Sax | B | B079 | single | 06 | D1 p7 |  |
| F02-P0080 | MS-2000 Sync | C | C015 | double | 11 | D1 p7 |  |
| F02-P0081 | Whirling Whirly | A | A016 | double | 00 | D1 p7 |  |
| F02-P0082 | DblTracked Clav1 | A | A080 | double | 00 | D1 p7 |  |
| F02-P0083 | A Real Flute | B | B016 | single | 06 | D1 p7 |  |
| F02-P0084 | SopranoSaxX-fade | B | B080 | double | 06 | D1 p7 |  |
| F02-P0085 | OB Classcial ST | C | C016 | single | 09 | D1 p7 |  |
| F02-P0086 | Clav 3 w/KeyOff | A | A017 | double | 00 | D1 p7 |  |
| F02-P0087 | DblTracked Clav2 | A | A081 | double | 00 | D1 p7 |  |
| F02-P0088 | Flugel Air & Vib | B | B017 | single | 05 | D1 p7 |  |
| F02-P0089 | SopranoSax FFF | B | B081 | double | 06 | D1 p7 |  |
| F02-P0090 | [ Angelis ] | C | C017 | double | 09 | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0091 | Processed Clav | A | A018 | single | 00 | D1 p7 |  |
| F02-P0092 | Clav Synth Wah | A | A082 | double | 00 | D1 p7 |  |
| F02-P0093 | TrueStereoBrass | B | B018 | double | 05 | D1 p7 |  |
| F02-P0094 | Big Band Saxes | B | B082 | double | 06 | D1 p7 |  |
| F02-P0095 | Strawberry Flute | C | C018 | single | 09 | D1 p7 |  |
| F02-P0096 | Vox Wah Clav | A | A019 | double | 00 | D1 p7 |  |
| F02-P0097 | Dynamic Duo | A | A083 | double | 00 | D1 p7 |  |
| F02-P0098 | RnB Horns | B | B019 | double | 05 | D1 p7 |  |
| F02-P0099 | Alto Flute | B | B083 | double | 06 | D1 p7 |  |
| F02-P0100 | NostalgicStrings | C | C019 | double | 09 | D1 p7 |  |
| F02-P0101 | Clav 2 Fonky | A | A020 | double | 00 | D1 p7 |  |
| F02-P0102 | Clav 3 Clear | A | A084 | double | 00 | D1 p7 |  |
| F02-P0103 | Classical Music | B | B020 | double | 03 | D1 p7 |  |
| F02-P0104 | Flute Duo | B | B084 | double | 06 | D1 p7 |  |
| F02-P0105 | Lush Ana Strings | C | C020 | double | 09 | D1 p7 |  |
| F02-P0106 | Funky Clav Mute | A | A021 | double | 00 | D1 p7 |  |
| F02-P0107 | Clav 1 w/Synth | A | A085 | double | 00 | D1 p7 |  |
| F02-P0108 | Irish Fiddle | B | B021 | double | 03 | D1 p7 |  |
| F02-P0109 | Solo AFlt Canyon | B | B085 | single | 06 | D1 p7 |  |
| F02-P0110 | 2600 Strings | C | C021 | double | 09 | D1 p7 |  |
| F02-P0111 | Digi EP & Pad | A | A022 | double | 00 | D1 p7 |  |
| F02-P0112 | Clav & EP Blend | A | A086 | double | 00 | D1 p7 |  |
| F02-P0113 | Squeeze Box | B | B022 | single | 06 | D1 p7 |  |
| F02-P0114 | Jazz Clarinet 2 | B | B086 | single | 06 | D1 p7 |  |
| F02-P0115 | Slow MG Synth | C | C022 | single | 12 | D1 p7 |  |
| F02-P0116 | VS Tine E. Piano | A | A023 | double | 00 | D1 p7 |  |
| F02-P0117 | Dyn Digi EP | A | A087 | double | 00 | D1 p7 |  |
| F02-P0118 | Uilleann Pipes | B | B023 | single | 06 | D1 p7 |  |
| F02-P0119 | Egyptian Oboe | B | B087 | double | 06 | D1 p7 |  |
| F02-P0120 | Heliosphere | C | C023 | double | 12 | D1 p7 |  |
| F02-P0121 | Wavetable EP | A | A024 | double | 00 | D1 p7 |  |
| F02-P0122 | Digi EP & Pad | A | A088 | double | 00 | D1 p7 |  |
| F02-P0123 | Vocal Dreams | B | B024 | double | 04 | D1 p7 |  |
| F02-P0124 | Solo E.Horn | B | B088 | single | 06 | D1 p7 |  |
| F02-P0125 | Butterfly Sweep | C | C024 | double | 12 | D1 p7 |  |
| F02-P0126 | Smooth Jazzorgan | A | A025 | double | 01 | D1 p7 |  |
| F02-P0127 | Digital Tine EP | A | A089 | single | 00 | D1 p7 |  |
| F02-P0128 | Lush Vocals | B | B025 | double | 04 | D1 p7 |  |
| F02-P0129 | Medieval Jig | B | B089 | double | 06 | D1 p7 |  |
| F02-P0130 | Haze Pad | C | C025 | double | 12 | D1 p7 |  |
| F02-P0131 | CX Jazz Organ | A | A026 | double | 01 | D1 p7 |  |
| F02-P0132 | CX Jazz Organ | A | A090 | double | 01 | D1 p7 |  |
| F02-P0133 | Extreme Choir | B | B026 | double | 04 | D1 p7 |  |
| F02-P0134 | Highland Pipes | B | B090 | double | 06 | D1 p7 |  |
| F02-P0135 | Vocoder Choir | C | C026 | single | 04 | D1 p7 |  |
| F02-P0136 | Nu Jazz Organ | A | A027 | double | 01 | D1 p7 |  |
| F02-P0137 | Jazz Perc Organ1 | A | A091 | double | 01 | D1 p7 |  |
| F02-P0138 | Ohh Hmm Huu | B | B027 | double | 04 | D1 p7 |  |
| F02-P0139 | Scottish Clan | B | B091 | double | 06 | D1 p7 |  |
| F02-P0140 | Tiny Androids | C | C027 | double | 04 | D1 p7 |  |
| F02-P0141 | Rock Dist | A | A028 | double | 01 | D1 p7 |  |
| F02-P0142 | Jazz Perc Organ2 | A | A092 | double | 01 | D1 p7 |  |
| F02-P0143 | Delayed Voices | B | B028 | double | 04 | D1 p7 |  |
| F02-P0144 | Bandoneon 1 | B | B092 | double | 06 | D1 p7 |  |
| F02-P0145 | Sub Arp | C | C028 | double | 14 | D1 p7 |  |
| F02-P0146 | Hard CX-3 | A | A029 | double | 01 | D1 p7 |  |
| F02-P0147 | Jazz Perc Organ3 | A | A093 | double | 01 | D1 p7 |  |
| F02-P0148 | Modern Rock Kit | B | B029 | double | 15 | D1 p7 |  |
| F02-P0149 | Bandoneon 2 | B | B093 | double | 06 | D1 p7 |  |
| F02-P0150 | ProgressvTrance2 | C | C029 | double | 14 | D1 p7 |  |
| F02-P0151 | CX-3 Chorale | A | A030 | double | 01 | D1 p7 |  |
| F02-P0152 | Mods 60's Organ | A | A094 | double | 01 | D1 p7 |  |
| F02-P0153 | Under Hard Kit | B | B030 | double | 15 | D1 p7 |  |
| F02-P0154 | Accordion Player | B | B094 | double | 06 | D1 p7 |  |
| F02-P0155 | PitchSlamKit-SW | C | C030 | double | 15 | D1 p7 |  |
| F02-P0156 | Vintage B | A | A031 | double | 01 | D1 p7 |  |
| F02-P0157 | L.Hand Jazz Comp | A | A095 | double | 01 | D1 p7 |  |
| F02-P0158 | Digi-H.Core Kit | B | B031 | double | 15 | D1 p7 |  |
| F02-P0159 | Clean L+R Brass | B | B095 | double | 05 | D1 p7 |  |
| F02-P0160 | She's a GOA! | C | C031 | double | 15 | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0161 | Hymnal Organ | A | A032 | single | 01 | D1 p7 |  |
| F02-P0162 | Down 'n Dirty B | A | A096 | double | 01 | D1 p7 |  |
| F02-P0163 | Electro Kit 2 | B | B032 | double | 15 | D1 p7 |  |
| F02-P0164 | Trumpet Section3 | B | B096 | double | 05 | D1 p7 |  |
| F02-P0165 | Lounge Kit | C | C032 | double | 15 | D1 p7 |  |
| F02-P0166 | Pipe Full | A | A033 | double | 01 | D1 p7 |  |
| F02-P0167 | Classic BX-3 | A | A097 | single | 01 | D1 p7 |  |
| F02-P0168 | Voice Catalog | B | B033 | double | 15 | D1 p7 |  |
| F02-P0169 | Brass Real&Synth | B | B097 | double | 05 | D1 p7 |  |
| F02-P0170 | Wormhole Piano | C | C033 | double | 13 | D1 p7 |  |
| F02-P0171 | BD Grand more | A | A034 | double | 00 | D1 p7 |  |
| F02-P0172 | Solo Jazz B | A | A098 | double | 01 | D1 p7 |  |
| F02-P0173 | Nu A.Guitar | B | B034 | double | 07 | D1 p7 |  |
| F02-P0174 | Stereo Trumpet | B | B098 | double | 05 | D1 p7 |  |
| F02-P0175 | Circuit S/H | C | C034 | double | 13 | D1 p7 |  |
| F02-P0176 | Nu'Coustic Grand | A | A035 | double | 00 | D1 p7 |  |
| F02-P0177 | Traffic Organ | A | A099 | double | 01 | D1 p7 |  |
| F02-P0178 | Steel String Gtr | B | B035 | single | 07 | D1 p7 |  |
| F02-P0179 | Trumpet X-fade | B | B099 | double | 05 | D1 p7 |  |
| F02-P0180 | The EEC Groove | C | C035 | double | 10 | D1 p7 |  |
| F02-P0181 | Soft Grand | A | A036 | double | 00 | D1 p7 |  |
| F02-P0182 | Gospel Hour w/OD | A | A100 | double | 01 | D1 p7 |  |
| F02-P0183 | Reso-G PickVEL! | B | B036 | double | 07 | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0184 | Piccolo Trumpet2 | B | B100 | single | 05 | D1 p7 |  |
| F02-P0185 | Auto LPF Tech | C | C036 | double | 10 | D1 p7 |  |
| F02-P0186 | Nu Piano | A | A037 | double | 00 | D1 p7 |  |
| F02-P0187 | Gospel 1-JS|Kn | A | A101 | double | 01 | D1 p7 |  |
| F02-P0188 | Expensive Guitar | B | B037 | double | 07 | D1 p7 |  |
| F02-P0189 | Jazz Trumpeteer | B | B101 | single | 05 | D1 p7 |  |
| F02-P0190 | MassiveKillerPad | C | C037 | double | 10 | D1 p7 |  |
| F02-P0191 | S.Grand Piano | A | A038 | double | 00 | D1 p7 |  |
| F02-P0192 | Gospel 2-JS|Kn | A | A102 | double | 01 | D1 p7 |  |
| F02-P0193 | Metal Plectrum | B | B038 | double | 07 | D1 p7 |  |
| F02-P0194 | TubaFlugelhorn | B | B102 | single | 05 | D1 p7 |  |
| F02-P0195 | Nu Furnace | C | C038 | double | 10 | D1 p7 |  |
| F02-P0196 | [Big Pop-Grand] | A | A039 | double | 00 | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0197 | Nu Jazz Organ | A | A103 | double | 01 | D1 p7 |  |
| F02-P0198 | Guitar Duo | B | B039 | double | 07 | D1 p7 |  |
| F02-P0199 | Tuba | B | B103 | single | 05 | D1 p7 |  |
| F02-P0200 | Epic Trance Stab | C | C039 | double | 10 | D1 p7 |  |
| F02-P0201 | GrandJazzPiano | A | A040 | double | 00 | D1 p7 |  |
| F02-P0202 | Jazz Rotary | A | A104 | single | 01 | D1 p7 |  |
| F02-P0203 | Angel Guitar | B | B040 | double | 07 | D1 p7 |  |
| F02-P0204 | Flugel Horn Duo | B | B104 | double | 05 | D1 p7 |  |
| F02-P0205 | Killer DetunSyn | C | C040 | double | 10 | D1 p7 |  |
| F02-P0206 | BrightLongPiano | A | A041 | double | 00 | D1 p7 |  |
| F02-P0207 | Swish Chords | A | A105 | double | 01 | D1 p7 |  |
| F02-P0208 | Nu N.Guitar 2 | B | B041 | double | 07 | D1 p7 |  |
| F02-P0209 | Trumpets Duo | B | B105 | double | 05 | D1 p7 |  |
| F02-P0210 | Synth Brass | C | C041 | double | 10 | D1 p7 |  |
| F02-P0211 | Nu'Coustic [VS] | A | A042 | double | 00 | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0212 | Full-JS|Kn | A | A106 | double | 01 | D1 p7 |  |
| F02-P0213 | Nylon String Gtr | B | B042 | single | 07 | D1 p7 |  |
| F02-P0214 | Trumpet & FrHorn | B | B106 | double | 05 | D1 p7 |  |
| F02-P0215 | TotalPower Synth | C | C042 | double | 10 | D1 p7 |  |
| F02-P0216 | Velo Piano ST | A | A043 | double | 00 | D1 p7 |  |
| F02-P0217 | Full Drawbars | A | A107 | double | 01 | D1 p7 |  |
| F02-P0218 | Flamingo Guitar | B | B043 | double | 07 | D1 p7 |  |
| F02-P0219 | Violin Section | B | B107 | double | 03 | D1 p7 |  |
| F02-P0220 | Stereo M12 | C | C043 | double | 10 | D1 p7 |  |
| F02-P0221 | Euro Grand | A | A044 | double | 00 | D1 p7 |  |
| F02-P0222 | Electric Organ | A | A108 | double | 01 | D1 p7 |  |
| F02-P0223 | Nylon 12-String | B | B044 | double | 07 | D1 p7 |  |
| F02-P0224 | Agressiv Bows | B | B108 | double | 03 | D1 p7 |  |
| F02-P0225 | Anthem Tool | C | C044 | double | 10 | D1 p7 |  |
| F02-P0226 | Studio Piano | A | A045 | double | 00 | D1 p7 |  |
| F02-P0227 | Organ 888 | A | A109 | double | 01 | D1 p7 |  |
| F02-P0228 | Classic12-String | B | B045 | double | 07 | D1 p7 |  |
| F02-P0229 | Rain Drops | B | B109 | double | 03 | D1 p7 |  |
| F02-P0230 | Puncher | C | C045 | double | 10 | D1 p7 |  |
| F02-P0231 | The Piano Man | A | A046 | double | 00 | D1 p7 |  |
| F02-P0232 | Burnin' B Organ | A | A110 | double | 01 | D1 p7 |  |
| F02-P0233 | Mr 12 String | B | B046 | double | 07 | D1 p7 |  |
| F02-P0234 | Strings Real&Syn | B | B110 | double | 03 | D1 p7 |  |
| F02-P0235 | Dipsy Squares | C | C046 | double | 10 | D1 p7 |  |
| F02-P0236 | A Piano Moment | A | A047 | double | 00 | D1 p7 |  |
| F02-P0237 | Art Rock Organ | A | A111 | double | 01 | D1 p7 |  |
| F02-P0238 | Etreme12-String | B | B047 | double | 07 | D1 p7 |  |
| F02-P0239 | Digital Strings | B | B111 | double | 03 | D1 p7 |  |
| F02-P0240 | M12 Poly | C | C047 | double | 10 | D1 p7 |  |
| F02-P0241 | AI SynPiano | A | A048 | double | 00 | D1 p7 |  |
| F02-P0242 | Low.Man. 1-JS|Kn | A | A112 | double | 01 | D1 p7 |  |
| F02-P0243 | Indian 12-String | B | B048 | double | 07 | D1 p7 |  |
| F02-P0244 | Voice Of EIRE | B | B112 | double | 04 | D1 p7 |  |
| F02-P0245 | Warm Clavi Pad | C | C048 | double | 10 | D1 p7 |  |
| F02-P0246 | PopBallad Piano2 | A | A049 | double | 00 | D1 p7 |  |
| F02-P0247 | Low.Man. 2-JS|Kn | A | A113 | double | 01 | D1 p7 |  |
| F02-P0248 | Mellow 12-String | B | B049 | double | 07 | D1 p7 |  |
| F02-P0249 | The Choir | B | B113 | double | 04 | D1 p7 |  |
| F02-P0250 | ProSyn ClDarkKey | C | C049 | single | 10 | D1 p7 |  |
| F02-P0251 | Piano&SubtleOne | A | A050 | double | 00 | D1 p7 |  |
| F02-P0252 | CX-3 Bright | A | A114 | double | 01 | D1 p7 |  |
| F02-P0253 | Gentle 12-String | B | B050 | double | 07 | D1 p7 |  |
| F02-P0254 | Anima Voice Pad | B | B114 | double | 04 | D1 p7 |  |
| F02-P0255 | 2VCO Saw Organ | C | C050 | double | 10 | D1 p7 |  |
| F02-P0256 | Euro Pop Piano | A | A051 | double | 00 | D1 p7 |  |
| F02-P0257 | 16' + 2' | A | A115 | double | 01 | D1 p7 |  |
| F02-P0258 | 24-String Guitar | B | B051 | double | 07 | D1 p7 |  |
| F02-P0259 | Oohs&Aahs (Vel) | B | B115 | double | 04 | D1 p7 |  |
| F02-P0260 | Polyphet Line | C | C051 | double | 10 | D1 p7 |  |
| F02-P0261 | SGProXRockPiano | A | A052 | double | 00 | D1 p7 |  |
| F02-P0262 | Continental Vox | A | A116 | single | 01 | D1 p7 |  |
| F02-P0263 | Velo E Gtr SW1 | B | B052 | double | 07 | D1 p7 |  |
| F02-P0264 | Male Wuh Choir | B | B116 | double | 04 | D1 p7 |  |
| F02-P0265 | J-Pad Bell | C | C052 | double | 10 | D1 p7 |  |
| F02-P0266 | Rock Piano | A | A053 | double | 00 | D1 p7 |  |
| F02-P0267 | Vel.Change Organ | A | A117 | double | 01 | D1 p7 |  |
| F02-P0268 | Dyna E.Guitar | B | B053 | double | 07 | D1 p7 |  |
| F02-P0269 | Female Wuu Choir | B | B117 | double | 04 | D1 p7 |  |
| F02-P0270 | Tremolo Pad | C | C053 | double | 10 | D1 p7 |  |
| F02-P0271 | Rock & Roll | A | A054 | double | 00 | D1 p7 |  |
| F02-P0272 | Electric Pipe 1 | A | A118 | double | 01 | D1 p7 |  |
| F02-P0273 | Dyna E.Guitar | B | B054 | double | 07 | D1 p7 |  |
| F02-P0274 | Siren Voices | B | B118 | double | 04 | D1 p7 |  |
| F02-P0275 | Ice Piano | C | C054 | double | 10 | D1 p7 |  |
| F02-P0276 | Salsa Keys | A | A055 | double | 00 | D1 p7 |  |
| F02-P0277 | Electric Pipe 2 | A | A119 | double | 01 | D1 p7 |  |
| F02-P0278 | Dyna E.Guitar | B | B055 | double | 07 | D1 p7 |  |
| F02-P0279 | Vocal Harp | B | B119 | double | 04 | D1 p7 |  |
| F02-P0280 | Starlight Night | C | C055 | double | 10 | D1 p7 |  |
| F02-P0281 | Power Grand | A | A056 | double | 00 | D1 p7 |  |
| F02-P0282 | Church Organ | A | A120 | double | 01 | D1 p7 |  |
| F02-P0283 | TremoloE.Guitar | B | B056 | double | 07 | D1 p7 |  |
| F02-P0284 | Voices-Wuh Wuu | B | B120 | double | 04 | D1 p7 |  |
| F02-P0285 | Watermark | C | C056 | double | 10 | D1 p7 |  |
| F02-P0286 | Stereo E. Grand | A | A057 | double | 00 | D1 p7 |  |
| F02-P0287 | Recessional Org | A | A121 | double | 01 | D1 p7 |  |
| F02-P0288 | LP Guitar/Bass | B | B057 | double | 07 | D1 p7 |  |
| F02-P0289 | Aah Men | B | B121 | double | 04 | D1 p7 |  |
| F02-P0290 | Keys To My Heart | C | C057 | single | 10 | D1 p7 |  |
| F02-P0291 | Magic E. Grand | A | A058 | double | 00 | D1 p7 |  |
| F02-P0292 | Tuentina Octaves | A | A122 | double | 01 | D1 p7 |  |
| F02-P0293 | Muted Gtr Strum | B | B058 | double | 07 | D1 p7 |  |
| F02-P0294 | Gospel&M1 Choir | B | B122 | double | 04 | D1 p7 |  |
| F02-P0295 | E-Piano VPM Type | C | C058 | double | 10 | D1 p7 |  |
| F02-P0296 | Piano&SynthPal | A | A059 | double | 00 | D1 p7 |  |
| F02-P0297 | Tuentina + Flute | A | A123 | double | 01 | D1 p7 |  |
| F02-P0298 | Sitar Strum | B | B059 | single | 07 | D1 p7 |  |
| F02-P0299 | Gospel&VocoderVS | B | B123 | double | 04 | D1 p7 |  |
| F02-P0300 | Mega Piano Pad | C | C059 | double | 10 | D1 p7 |  |
| F02-P0301 | Piano&AnaStrings | A | A060 | double | 00 | D1 p7 |  |
| F02-P0302 | Positive SW#1 | A | A124 | double | 01 | D1 p7 |  |
| F02-P0303 | Growling Upright | B | B060 | double | 08 | D1 p7 |  |
| F02-P0304 | SynVoice M1Choir | B | B124 | double | 04 | D1 p7 |  |
| F02-P0305 | ProSynRev2 Brass | C | C060 | single | 10 | D1 p7 |  |
| F02-P0306 | Piano&Syn Comp | A | A061 | double | 00 | D1 p7 |  |
| F02-P0307 | Pipe Reeds 1 | A | A125 | double | 01 | D1 p7 |  |
| F02-P0308 | Walking A. Bass | B | B061 | double | 08 | D1 p7 |  |
| F02-P0309 | Its TV Time Kids | B | B125 | double | 02 | D1 p7 |  |
| F02-P0310 | M12 Brass | C | C061 | double | 10 | D1 p7 |  |
| F02-P0311 | Pf&Synth Detuned | A | A062 | double | 00 | D1 p7 |  |
| F02-P0312 | Pipe Reeds 2 | A | A126 | double | 01 | D1 p7 |  |
| F02-P0313 | New P.Bass | B | B062 | single | 08 | D1 p7 |  |
| F02-P0314 | Nu-Pop Kit | B | B126 | double | 15 | D1 p7 |  |
| F02-P0315 | Soft Brass Phive | C | C062 | double | 10 | D1 p7 |  |
| F02-P0316 | Tine E.Piano | A | A063 | single | 00 | D1 p7 |  |
| F02-P0317 | Pipe Flauto | A | A127 | double | 01 | D1 p7 |  |
| F02-P0318 | Nu'Finger Bs 2 | B | B063 | single | 08 | D1 p7 |  |
| F02-P0319 | Psycho Junk Kit | B | B127 | double | 15 | D1 p7 |  |
| F02-P0320 | Real-Cheap Brass | C | C063 | double | 10 | D1 p7 |  |
| F02-P0321 | Harry Bass | C | C064 | double | 08 | D1 p8 |  |
| F02-P0322 | Marcato Str Orch | D | D000 | double | 03 | D1 p8 |  |
| F02-P0323 | String Quartet | D | D064 | double | 03 | D1 p8 |  |
| F02-P0324 | Acoustic Piano | G | G001 | single | 00 | D1 p8 |  |
| F02-P0325 | Detuned Clean | G | g(1)028 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0326 | ProSynth Bass | C | C065 | double | 08 | D1 p8 |  |
| F02-P0327 | Maestro Violin | D | D001 | single | 03 | D1 p8 |  |
| F02-P0328 | Solace Strings | D | D065 | double | 03 | D1 p8 |  |
| F02-P0329 | Acoustic Piano | G | g(1)001 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0330 | Mid Tone Guitar | G | g(2)028 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0331 | MG Tri Bass | C | C066 | double | 08 | D1 p8 |  |
| F02-P0332 | Largo Strings | D | D002 | double | 03 | D1 p8 |  |
| F02-P0333 | Two Violins | D | D066 | double | 03 | D1 p8 |  |
| F02-P0334 | Acoustic Piano | G | g(2)001 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0335 | Muted Guitar | G | G029 | single | 07 | D1 p8 |  |
| F02-P0336 | PWM Lead/Bass | C | C067 | double | 08 | D1 p8 |  |
| F02-P0337 | StrOrch. Trem | D | D003 | double | 03 | D1 p8 |  |
| F02-P0338 | Two Violas | D | D067 | double | 03 | D1 p8 |  |
| F02-P0339 | Bright Piano | G | G002 | single | 00 | D1 p8 |  |
| F02-P0340 | Funky Guitar | G | g(1)029 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0341 | Serious Bass | C | C068 | double | 08 | D1 p8 |  |
| F02-P0342 | Medium Strings | D | D004 | double | 03 | D1 p8 |  |
| F02-P0343 | Two Celli | D | D068 | double | 03 | D1 p8 |  |
| F02-P0344 | Bright Piano | G | g(1)002 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0345 | Muted Guitar | G | g(2)029 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0346 | Deeeep Bass | C | C069 | single | 08 | D1 p8 |  |
| F02-P0347 | Pizz Str Section | D | D005 | double | 03 | D1 p8 |  |
| F02-P0348 | Viola & Cello | D | D069 | double | 03 | D1 p8 |  |
| F02-P0349 | El.Grand Piano | G | G003 | single | 00 | D1 p8 |  |
| F02-P0350 | Jazz Man | G | g(3)029 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0351 | PolySix Bass | C | C070 | double | 08 | D1 p8 |  |
| F02-P0352 | SFZ String Orch. | D | D006 | double | 03 | D1 p8 |  |
| F02-P0353 | Pizz Solo Violin | D | D070 | single | 03 | D1 p8 |  |
| F02-P0354 | El.Grand Piano | G | g(1)003 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0355 | Overdrive Guitar | G | G030 | single | 07 | D1 p8 |  |
| F02-P0356 | Comp MG Bass | C | C071 | double | 08 | D1 p8 |  |
| F02-P0357 | Orch.Flute Vib | D | D007 | single | 06 | D1 p8 |  |
| F02-P0358 | Pizz Solo Viola | D | D071 | single | 03 | D1 p8 |  |
| F02-P0359 | Honkey-Tonk | G | G004 | double | 00 | D1 p8 |  |
| F02-P0360 | Guitar Pinch | G | g(1)030 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0361 | Monopoly Bass | C | C072 | double | 08 | D1 p8 |  |
| F02-P0362 | Orch. Oboe Vib | D | D008 | single | 06 | D1 p8 |  |
| F02-P0363 | Pizz Solo Cello | D | D072 | single | 03 | D1 p8 |  |
| F02-P0364 | Honkey-Tonk | G | g(1)004 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0365 | DistortionGuitar | G | G031 | single | 07 | D1 p8 |  |
| F02-P0366 | Name Bass | C | C073 | single | 08 | D1 p8 |  |
| F02-P0367 | Clarinet | D | D009 | single | 06 | D1 p8 |  |
| F02-P0368 | Pizz Solo Bass | D | D073 | double | 03 | D1 p8 |  |
| F02-P0369 | Electric Piano | G | G005 | single | 00 | D1 p8 |  |
| F02-P0370 | Feedback Guitar | G | g(1)031 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0371 | Infected Bass | C | C074 | double | 08 | D1 p8 |  |
| F02-P0372 | 4FrenchHorns | D | D010 | double | 05 | D1 p8 |  |
| F02-P0373 | Pizz Str Quartet | D | D074 | single | 03 | D1 p8 |  |
| F02-P0374 | Detuned E.Piano1 | G | g(1)005 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0375 | Dist. Rhythm | G | g(2)031 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0376 | Deep Organ Bass | C | C075 | double | 08 | D1 p8 |  |
| F02-P0377 | Bright Trombones | D | D011 | double | 05 | D1 p8 |  |
| F02-P0378 | Alto Flute Vib. | D | D075 | single | 06 | D1 p8 |  |
| F02-P0379 | ElectricPiano1 | G | g(2)005 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0380 | Guitar Harmonics | G | G032 | single | 07 | D1 p8 |  |
| F02-P0381 | Antic Bass | C | C076 | double | 08 | D1 p8 |  |
| F02-P0382 | Solo Trumpet | D | D012 | double | 05 | D1 p8 |  |
| F02-P0383 | 3 Flutes | D | D076 | double | 06 | D1 p8 |  |
| F02-P0384 | 60’s E.Piano | G | g(3)005 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0385 | Guitar Feedback | G | g(1)032 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0386 | Effect Bass | C | C077 | double | 08 | D1 p8 |  |
| F02-P0387 | EXP Vibraphone | D | D013 | double | 02 | D1 p8 |  |
| F02-P0388 | Orchestral Oboe1 | D | D077 | single | 06 | D1 p8 |  |
| F02-P0389 | Electric Piano | G | G006 | single | 00 | D1 p8 |  |
| F02-P0390 | Acoustic Bass | G | G033 | double | 08 | D1 p8 |  |
| F02-P0391 | Reso Stereo Bass | C | C078 | double | 08 | D1 p8 |  |
| F02-P0392 | Concert Harp | D | D014 | double | 07 | D1 p8 |  |
| F02-P0393 | OrchEnglishHorn1 | D | D078 | single | 06 | D1 p8 |  |
| F02-P0394 | Detuned E.Piano2 | G | g(1)006 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0395 | Fingered Bass | G | G034 | double | 08 | D1 p8 |  |
| F02-P0396 | Dark Rez Bass | C | C079 | double | 08 | D1 p8 |  |
| F02-P0397 | EXP Velo Timpani | D | D015 | double | 15 | D1 p8 |  |
| F02-P0398 | EnglishHorn Vib1 | D | D079 | single | 06 | D1 p8 |  |
| F02-P0399 | ElectricPiano2 | G | g(2)006 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0400 | Finger Slap | G | g(1)034 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0401 | Square Tea Bass | C | C080 | double | 08 | D1 p8 |  |
| F02-P0402 | EXB Orch. Kit | D | D016 | double | 15 | D1 p8 |  |
| F02-P0403 | OrchBs.Clarinet1 | D | D080 | single | 06 | D1 p8 |  |
| F02-P0404 | E.Piano Legend | G | g(3)006 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0405 | Picked Bass | G | G035 | double | 08 | D1 p8 |  |
| F02-P0406 | Tigger the Bass | C | C081 | double | 08 | D1 p8 |  |
| F02-P0407 | Harp Phrase Set | D | D017 | single | 14 | D1 p8 |  |
| F02-P0408 | 2 Clarinets | D | D081 | double | 06 | D1 p8 |  |
| F02-P0409 | E.Piano Phase | G | g(4)006 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0410 | Fretless Bass | G | G036 | double | 08 | D1 p8 |  |
| F02-P0411 | Dist Tea Bass | C | C082 | double | 08 | D1 p8 |  |
| F02-P0412 | Xpressiv Violins | D | D018 | double | 03 | D1 p8 |  |
| F02-P0413 | Clarinets ST | D | D082 | double | 06 | D1 p8 |  |
| F02-P0414 | Harpsichord | G | G007 | single | 00 | D1 p8 |  |
| F02-P0415 | Slap Bass 1 | G | G037 | double | 08 | D1 p8 |  |
| F02-P0416 | Reverse Bass | C | C083 | double | 08 | D1 p8 |  |
| F02-P0417 | 14 Violins ST | D | D019 | double | 03 | D1 p8 |  |
| F02-P0418 | Orch. Bassoon | D | D083 | single | 06 | D1 p8 |  |
| F02-P0419 | Harpsi. oct-mix | G | g(1)007 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0420 | Slap Bass 2 | G | G038 | double | 08 | D1 p8 |  |
| F02-P0421 | Hip-Timp Bass | C | C084 | double | 08 | D1 p8 |  |
| F02-P0422 | Marcato Violins | D | D020 | double | 03 | D1 p8 |  |
| F02-P0423 | Contra Bassoon | D | D084 | single | 06 | D1 p8 |  |
| F02-P0424 | Harpsichord | G | g(2)007 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0425 | Synth Bass 1 | G | G039 | double | 08 | D1 p8 |  |
| F02-P0426 | 2VCO PlanetLead2 | C | C085 | double | 11 | D1 p8 |  |
| F02-P0427 | Largo Violins | D | D021 | double | 03 | D1 p8 |  |
| F02-P0428 | 2 Double Reeds | D | D085 | double | 06 | D1 p8 |  |
| F02-P0429 | Harpsi. key-off | G | g(3)007 | double | 00 | D1 p8 | document's own GM variant row |
| F02-P0430 | Warm Synth | G | g(1)039 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0431 | ProSynth SqrLead | C | C086 | single | 11 | D1 p8 |  |
| F02-P0432 | Largo RosinVlins | D | D022 | double | 03 | D1 p8 |  |
| F02-P0433 | Stereo Trumpets | D | D086 | double | 05 | D1 p8 |  |
| F02-P0434 | Clavi. | G | G008 | single | 00 | D1 p8 |  |
| F02-P0435 | Synth Bass 3 | G | g(2)039 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0436 | Detune Sqr Lead | C | C087 | single | 11 | D1 p8 |  |
| F02-P0437 | Rosin Violins | D | D023 | double | 03 | D1 p8 |  |
| F02-P0438 | Trumpet Section1 | D | D087 | double | 05 | D1 p8 |  |
| F02-P0439 | Pulse Clavi. | G | g(1)008 | single | 00 | D1 p8 | document's own GM variant row |
| F02-P0440 | Clavi Bass | G | g(3)039 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0441 | MiniKORG700 | C | C088 | single | 11 | D1 p8 |  |
| F02-P0442 | Xpressiv Violas | D | D024 | double | 03 | D1 p8 |  |
| F02-P0443 | Trumpet Section2 | D | D088 | double | 05 | D1 p8 |  |
| F02-P0444 | Celesta | G | G009 | single | 02 | D1 p8 |  |
| F02-P0445 | Hammer | G | g(4)039 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0446 | MS20 Lead | C | C089 | double | 11 | D1 p8 |  |
| F02-P0447 | 10 Violas ST | D | D025 | double | 03 | D1 p8 |  |
| F02-P0448 | Largo Trumpets | D | D089 | double | 05 | D1 p8 |  |
| F02-P0449 | Glockenspiel | G | G010 | double | 02 | D1 p8 |  |
| F02-P0450 | Synth Bass 2 | G | G040 | single | 08 | D1 p8 |  |
| F02-P0451 | Game Pulse Box | C | C090 | single | 11 | D1 p8 |  |
| F02-P0452 | Marcato Violas | D | D026 | double | 03 | D1 p8 |  |
| F02-P0453 | French Horns | D | D090 | double | 05 | D1 p8 |  |
| F02-P0454 | Music Box | G | G011 | single | 02 | D1 p8 |  |
| F02-P0455 | Synth Bass 4 | G | g(1)040 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0456 | MS-2000 Sync | C | C091 | double | 11 | D1 p8 |  |
| F02-P0457 | ExpressViolas | D | D027 | double | 03 | D1 p8 |  |
| F02-P0458 | F.Horns Mix | D | D091 | double | 05 | D1 p8 |  |
| F02-P0459 | Vibraphone | G | G012 | single | 02 | D1 p8 |  |
| F02-P0460 | Rubber Bass | G | g(2)040 | double | 08 | D1 p8 | document's own GM variant row |
| F02-P0461 | Rebel Lead | C | C092 | double | 11 | D1 p8 |  |
| F02-P0462 | Largo Violas | D | D028 | double | 03 | D1 p8 |  |
| F02-P0463 | Largo F.Horns | D | D092 | double | 05 | D1 p8 |  |
| F02-P0464 | Vibraphone | G | g(1)012 | single | 02 | D1 p8 | document's own GM variant row |
| F02-P0465 | Attack Pulse | G | g(3)040 | single | 08 | D1 p8 | document's own GM variant row |
| F02-P0466 | Strings Machine | C | C093 | double | 09 | D1 p8 |  |
| F02-P0467 | Rosin Violas | D | D029 | double | 03 | D1 p8 |  |
| F02-P0468 | Horns of France | D | D093 | double | 05 | D1 p8 |  |
| F02-P0469 | Marimba | G | G013 | single | 02 | D1 p8 |  |
| F02-P0470 | Violin | G | G041 | single | 03 | D1 p8 |  |
| F02-P0471 | OB Strings | C | C094 | double | 09 | D1 p8 |  |
| F02-P0472 | Xpressiv Celli | D | D030 | double | 03 | D1 p8 |  |
| F02-P0473 | TromboneSection1 | D | D094 | double | 05 | D1 p8 |  |
| F02-P0474 | Marimba w | G | g(1)013 | single | 02 | D1 p8 | document's own GM variant row |
| F02-P0475 | Slow Violin | G | g(1)041 | single | 03 | D1 p8 | document's own GM variant row |
| F02-P0476 | Delta StrngSynth | C | C095 | double | 09 | D1 p8 |  |
| F02-P0477 | 8 Celli ST | D | D031 | double | 03 | D1 p8 |  |
| F02-P0478 | ff Trombones | D | D095 | double | 05 | D1 p8 |  |
| F02-P0479 | Xylophone | G | G014 | single | 02 | D1 p8 |  |
| F02-P0480 | Viola | G | G042 | single | 03 | D1 p8 |  |
| F02-P0481 | Phasing Strings | C | C096 | double | 09 | D1 p8 |  |
| F02-P0482 | Marcato Celli | D | D032 | double | 03 | D1 p8 |  |
| F02-P0483 | TromboneSection2 | D | D096 | double | 05 | D1 p8 |  |
| F02-P0484 | Tubular Bells | G | G015 | single | 02 | D1 p8 |  |
| F02-P0485 | Cello | G | G043 | single | 03 | D1 p8 |  |
| F02-P0486 | Ana Voc -10ATK | C | C097 | double | 09 | D1 p8 |  |
| F02-P0487 | Largo Celli | D | D033 | double | 03 | D1 p8 |  |
| F02-P0488 | Largo Trombones | D | D097 | double | 05 | D1 p8 |  |
| F02-P0489 | Church Bell | G | g(1)015 | double | 02 | D1 p8 | document's own GM variant row |
| F02-P0490 | Contrabass | G | G044 | single | 03 | D1 p8 |  |
| F02-P0491 | Thin HPF Pad | C | C098 | double | 09 | D1 p8 |  |
| F02-P0492 | Largo RosinCelli | D | D034 | double | 03 | D1 p8 |  |
| F02-P0493 | SFZ Brass Sect. | D | D098 | double | 05 | D1 p8 |  |
| F02-P0494 | Carillon | G | g(2)015 | double | 02 | D1 p8 | document's own GM variant row |
| F02-P0495 | Tremolo Strings | G | G045 | single | 03 | D1 p8 |  |
| F02-P0496 | Glider Pad (SW1) | C | C099 | double | 09 | D1 p8 |  |
| F02-P0497 | Xpressiv Basses | D | D035 | double | 03 | D1 p8 |  |
| F02-P0498 | Symphonic Brass | D | D099 | double | 05 | D1 p8 |  |
| F02-P0499 | Santur | G | G016 | single | 07 | D1 p8 |  |
| F02-P0500 | Pizzicato Str. | G | G046 | single | 03 | D1 p8 |  |
| F02-P0501 | ReversePad | C | C100 | double | 09 | D1 p8 |  |
| F02-P0502 | 6 Basses ST | D | D036 | double | 03 | D1 p8 |  |
| F02-P0503 | ff Brass Section | D | D100 | double | 05 | D1 p8 |  |
| F02-P0504 | Drawbar Organ | G | G017 | single | 01 | D1 p8 |  |
| F02-P0505 | Orchestral Harp | G | G047 | single | 07 | D1 p8 |  |
| F02-P0506 | 4 by 4ths | C | C101 | double | 12 | D1 p8 |  |
| F02-P0507 | Marcato Basses | D | D037 | double | 03 | D1 p8 |  |
| F02-P0508 | BrightPiccoloTrp | D | D101 | single | 05 | D1 p8 |  |
| F02-P0509 | Det.DrawbarOrgan | G | g(1)017 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0510 | Yang Chin | G | g(1)047 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0511 | LightOfTheSource | C | C102 | double | 12 | D1 p8 |  |
| F02-P0512 | Largo Basses | D | D038 | double | 03 | D1 p8 |  |
| F02-P0513 | Piccolo Trumpet1 | D | D102 | double | 05 | D1 p8 |  |
| F02-P0514 | Italian60’sOrgan | G | g(2)017 | single | 01 | D1 p8 | document's own GM variant row |
| F02-P0515 | Timpani | G | G048 | double | 15 | D1 p8 |  |
| F02-P0516 | Padulus SW1 | C | C103 | double | 12 | D1 p8 |  |
| F02-P0517 | String Orchestra | D | D039 | double | 03 | D1 p8 |  |
| F02-P0518 | Express Trumpet | D | D103 | single | 05 | D1 p8 |  |
| F02-P0519 | Drawbar Organ | G | g(3)017 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0520 | Strings | G | G049 | single | 03 | D1 p8 |  |
| F02-P0521 | Honor & Love | C | C104 | double | 12 | D1 p8 |  |
| F02-P0522 | Rosin Strings | D | D040 | double | 03 | D1 p8 |  |
| F02-P0523 | Largo Trumpet | D | D104 | single | 05 | D1 p8 |  |
| F02-P0524 | PercussiveOrgan1 | G | G018 | single | 01 | D1 p8 |  |
| F02-P0525 | Strings & Brass | G | g(1)049 | double | 03 | D1 p8 | document's own GM variant row |
| F02-P0526 | Trance Scream | C | C105 | single | 12 | D1 p8 |  |
| F02-P0527 | Cinemagic Str | D | D041 | double | 03 | D1 p8 |  |
| F02-P0528 | Flugelhorn | D | D105 | single | 05 | D1 p8 |  |
| F02-P0529 | Det.Perc-Organ | G | g(1)018 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0530 | 60’s Strings | G | g(2)049 | double | 03 | D1 p8 | document's own GM variant row |
| F02-P0531 | Almost Asleep | C | C106 | double | 12 | D1 p8 |  |
| F02-P0532 | Philharmonic Str | D | D042 | double | 03 | D1 p8 |  |
| F02-P0533 | Express F.Horn | D | D106 | single | 05 | D1 p8 |  |
| F02-P0534 | PercussiveOrgan2 | G | g(2)018 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0535 | Slow Strings | G | G050 | single | 03 | D1 p8 |  |
| F02-P0536 | Underwater Cave | C | C107 | double | 12 | D1 p8 |  |
| F02-P0537 | Clear Strings | D | D043 | double | 03 | D1 p8 |  |
| F02-P0538 | Largo F.Horn | D | D107 | single | 05 | D1 p8 |  |
| F02-P0539 | Rock Organ | G | G019 | single | 01 | D1 p8 |  |
| F02-P0540 | Synth Strings | G | G051 | double | 03 | D1 p8 |  |
| F02-P0541 | VS Voice Pad | C | C108 | double | 04 | D1 p8 |  |
| F02-P0542 | Power Strings | D | D044 | double | 03 | D1 p8 |  |
| F02-P0543 | Bright Trombone | D | D108 | single | 05 | D1 p8 |  |
| F02-P0544 | Church Organ | G | G020 | single | 01 | D1 p8 |  |
| F02-P0545 | Synth Strings | G | g(1)051 | double | 03 | D1 p8 | document's own GM variant row |
| F02-P0546 | Chrome Choir | C | C109 | double | 04 | D1 p8 |  |
| F02-P0547 | Slow EXP Strings | D | D045 | double | 03 | D1 p8 |  |
| F02-P0548 | Solo Trombone | D | D109 | double | 05 | D1 p8 |  |
| F02-P0549 | Church Org | G | g(1)020 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0550 | Synth Strings | G | G052 | single | 03 | D1 p8 |  |
| F02-P0551 | Voxic | C | C110 | double | 04 | D1 p8 |  |
| F02-P0552 | Pad Strings | D | D046 | double | 03 | D1 p8 |  |
| F02-P0553 | Largo Trombone | D | D110 | single | 05 | D1 p8 |  |
| F02-P0554 | Det.Church Org | G | g(2)020 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0555 | Choir Aahs 1 | G | G053 | single | 04 | D1 p8 |  |
| F02-P0556 | Hardy Loop | C | C111 | double | 14 | D1 p8 |  |
| F02-P0557 | Octave Str Orch. | D | D047 | double | 03 | D1 p8 |  |
| F02-P0558 | Bright Tuba | D | D111 | single | 05 | D1 p8 |  |
| F02-P0559 | Reed Organ | G | G021 | double | 01 | D1 p8 |  |
| F02-P0560 | Choir Aahs 2 | G | g(1)053 | single | 04 | D1 p8 | document's own GM variant row |
| F02-P0561 | Wake Up Now! | C | C112 | double | 14 | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0562 | Producer Strings | D | D048 | double | 03 | D1 p8 |  |
| F02-P0563 | Solo Tuba | D | D112 | single | 05 | D1 p8 |  |
| F02-P0564 | Puff Organ | G | g(1)021 | double | 01 | D1 p8 | document's own GM variant row |
| F02-P0565 | Voice Oohs | G | G054 | single | 04 | D1 p8 |  |
| F02-P0566 | Muted Trance | C | C113 | double | 14 | D1 p8 |  |
| F02-P0567 | Str & Brass<vel> | D | D049 | double | 03 | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0568 | Largo Tuba | D | D113 | single | 05 | D1 p8 |  |
| F02-P0569 | Accordion 1 | G | G022 | double | 06 | D1 p8 |  |
| F02-P0570 | Humming | G | g(1)054 | double | 04 | D1 p8 | document's own GM variant row |
| F02-P0571 | Prophetic Pulsar | C | C114 | double | 14 | D1 p8 |  |
| F02-P0572 | Violins Trem ST | D | D050 | double | 03 | D1 p8 |  |
| F02-P0573 | Xylophone | D | D114 | double | 02 | D1 p8 |  |
| F02-P0574 | Accordion 2 | G | g(1)022 | single | 06 | D1 p8 | document's own GM variant row |
| F02-P0575 | Synth Vox | G | G055 | single | 04 | D1 p8 |  |
| F02-P0576 | Gymnastic ARP | C | C115 | double | 14 | D1 p8 |  |
| F02-P0577 | Violas Trem ST | D | D051 | double | 03 | D1 p8 |  |
| F02-P0578 | EXP Marimba | D | D115 | double | 02 | D1 p8 |  |
| F02-P0579 | Harmonica | G | G023 | single | 06 | D1 p8 |  |
| F02-P0580 | Analog Voice | G | g(1)055 | single | 04 | D1 p8 | document's own GM variant row |
| F02-P0581 | Analog Hit | C | C116 | double | 14 | D1 p8 |  |
| F02-P0582 | Celli Trem ST | D | D052 | double | 03 | D1 p8 |  |
| F02-P0583 | Orch. Marimba | D | D116 | double | 02 | D1 p8 |  |
| F02-P0584 | Bandoneon | G | G024 | double | 06 | D1 p8 |  |
| F02-P0585 | Orchestra Hit | G | G056 | single | 14 | D1 p8 |  |
| F02-P0586 | Toxic Buzz | C | C117 | double | 14 | D1 p8 |  |
| F02-P0587 | Basses Trem ST | D | D053 | double | 03 | D1 p8 |  |
| F02-P0588 | Soft Vibraphone | D | D117 | double | 02 | D1 p8 |  |
| F02-P0589 | Nylon Guitar | G | G025 | single | 07 | D1 p8 |  |
| F02-P0590 | Bass Hit Plus | G | g(1)056 | double | 14 | D1 p8 | document's own GM variant row |
| F02-P0591 | Sleep My Love | C | C118 | double | 02 | D1 p8 |  |
| F02-P0592 | SFZ String Trem | D | D054 | double | 03 | D1 p8 |  |
| F02-P0593 | Orchestra Bell | D | D118 | single | 02 | D1 p8 |  |
| F02-P0594 | Ukulele | G | g(1)025 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0595 | 6th Hit | G | g(2)056 | double | 14 | D1 p8 | document's own GM variant row |
| F02-P0596 | Fine Crystal | C | C119 | double | 02 | D1 p8 |  |
| F02-P0597 | Pizz Violins | D | D055 | double | 03 | D1 p8 |  |
| F02-P0598 | Tubular Bells | D | D119 | single | 02 | D1 p8 |  |
| F02-P0599 | NylonGtr.key-off | G | g(2)025 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0600 | Euro Hit | G | g(3)056 | double | 14 | D1 p8 | document's own GM variant row |
| F02-P0601 | Pumpin' Kit | C | C120 | double | 15 | D1 p8 |  |
| F02-P0602 | Pizz Violas | D | D056 | double | 03 | D1 p8 |  |
| F02-P0603 | Tuned Timpani | D | D120 | double | 15 | D1 p8 |  |
| F02-P0604 | Nylon Guitar | G | g(3)025 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0605 | Trumpet | G | G057 | single | 05 | D1 p8 |  |
| F02-P0606 | TEK NO Kit 1 | C | C121 | double | 15 | D1 p8 |  |
| F02-P0607 | Pizz Celli | D | D057 | double | 03 | D1 p8 |  |
| F02-P0608 | SFZ Timp Roll | D | D121 | double | 15 | D1 p8 |  |
| F02-P0609 | Steel Guitar | G | G026 | single | 07 | D1 p8 |  |
| F02-P0610 | Soft Trumpet | G | g(1)057 | single | 05 | D1 p8 | document's own GM variant row |
| F02-P0611 | Hard Kit 1 | C | C122 | double | 15 | D1 p8 |  |
| F02-P0612 | Pizz Basses | D | D058 | double | 03 | D1 p8 |  |
| F02-P0613 | Orch. Bass Drum | D | D122 | double | 15 | D1 p8 |  |
| F02-P0614 | 12-StringsGuitar | G | g(1)026 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0615 | Trombone 1 | G | G058 | single | 05 | D1 p8 |  |
| F02-P0616 | Hard Kit 2 | C | C123 | double | 15 | D1 p8 |  |
| F02-P0617 | EXPVeloPizzicato | D | D059 | double | 03 | D1 p8 |  |
| F02-P0618 | Cymbal SFX Swell | D | D123 | double | 15 | D1 p8 |  |
| F02-P0619 | Mandolin | G | g(2)026 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0620 | Trombone 2 | G | g(1)058 | double | 05 | D1 p8 | document's own GM variant row |
| F02-P0621 | Beat Box Kit | C | C124 | double | 15 | D1 p8 |  |
| F02-P0622 | Solo Violin Vib. | D | D060 | single | 03 | D1 p8 |  |
| F02-P0623 | Concert Gong | D | D124 | double | 15 | D1 p8 |  |
| F02-P0624 | Steel Gtr. Body | G | g(3)026 | double | 07 | D1 p8 | document's own GM variant row |
| F02-P0625 | Bright Trombone | G | g(2)058 | single | 05 | D1 p8 | document's own GM variant row |
| F02-P0626 | Antic Voice Kit | C | C125 | double | 15 | D1 p8 |  |
| F02-P0627 | Solo Viola Vib. | D | D061 | single | 03 | D1 p8 |  |
| F02-P0628 | Classic Harp | D | D125 | double | 07 | D1 p8 |  |
| F02-P0629 | Jazz Guitar | G | G027 | single | 07 | D1 p8 |  |
| F02-P0630 | Tuba | G | G059 | single | 05 | D1 p8 |  |
| F02-P0631 | Modulator MS20 | C | C126 | single | 13 | D1 p8 |  |
| F02-P0632 | Solo Cello Vib. | D | D062 | single | 03 | D1 p8 |  |
| F02-P0633 | String PhraseSet | D | D126 | single | 14 | D1 p8 |  |
| F02-P0634 | Pedal Steel | G | g(1)027 | single | 07 | D1 p8 | document's own GM variant row |
| F02-P0635 | Muted Trumpet | G | G060 | single | 05 | D1 p8 |  |
| F02-P0636 | Bkwrds/Frwrds | C | C127 | double | 13 | D1 p8 |  |
| F02-P0637 | Solo Bass Vib. | D | D063 | double | 03 | D1 p8 |  |
| F02-P0638 | Mixed Harp Gliss | D | D127 | double | 14 | D1 p8 |  |
| F02-P0639 | Clean Guitar | G | G028 | single | 07 | D1 p8 |  |
| F02-P0640 | Muted Trumpet | G | g(1)060 | single | 05 | D1 p8 | document's own GM variant row |
| F02-P0641 | French Horn | G | G061 | single | 05 | D1 p9 |  |
| F02-P0642 | Sitar 1 | G | G105 | single | 07 | D1 p9 |  |
| F02-P0643 | STANDARD | G | g(d)001 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0644 | Transonic Grand | H | H000 | double | 00 | D1 p9 |  |
| F02-P0645 | Snappy Bass/Lead | H | H064 | double | 08 | D1 p9 |  |
| F02-P0646 | Warm French | G | g(1)061 | double | Horn05 | D1 p9 | document's own GM variant row |
| F02-P0647 | Sitar 2 | G | g(1)105 | double | 07 | D1 p9 | document's own GM variant row |
| F02-P0648 | ROOM Kit | G | g(d)009 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0649 | Vintage EP200 | H | H001 | single | 00 | D1 p9 |  |
| F02-P0650 | Digilog Bass | H | H065 | double | 08 | D1 p9 |  |
| F02-P0651 | Brass Section | G | G062 | single | 05 | D1 p9 |  |
| F02-P0652 | Banjo | G | G106 | single | 07 | D1 p9 |  |
| F02-P0653 | POWER Kit | G | g(d)017 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0654 | Phunky Ana Piana | H | H002 | single | 00 | D1 p9 |  |
| F02-P0655 | Square Face Bass | H | H066 | double | 08 | D1 p9 |  |
| F02-P0656 | Brass Section | G | g(1)062 | double | 05 | D1 p9 | document's own GM variant row |
| F02-P0657 | Shamisen | G | G107 | single | 07 | D1 p9 |  |
| F02-P0658 | ELECTRONIC | G | g(d)025 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0659 | Organish Pad | H | H003 | single | 10 | D1 p9 |  |
| F02-P0660 | BaBaBass | H | H067 | double | 08 | D1 p9 |  |
| F02-P0661 | Synth Brass | G | G063 | double | 10 | D1 p9 |  |
| F02-P0662 | Koto | G | G108 | single | 07 | D1 p9 |  |
| F02-P0663 | ANALOG Kit | G | g(d)026 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0664 | Ethereal Pad | H | H004 | double | 10 | D1 p9 |  |
| F02-P0665 | Phone Bass | H | H068 | double | 08 | D1 p9 |  |
| F02-P0666 | Synth Brass | G | g(1)063 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0667 | Taisho Koto | G | g(1)108 | double | 07 | D1 p9 | document's own GM variant row |
| F02-P0668 | JAZZ Kit | G | g(d)033 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0669 | Distant Analog | H | H005 | single | 10 | D1 p9 |  |
| F02-P0670 | Plus Pulse | H | H069 | single | 08 | D1 p9 |  |
| F02-P0671 | Analog Brass | G | g(2)063 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0672 | Kalimba | G | G109 | single | 02 | D1 p9 |  |
| F02-P0673 | BRUSH Kit | G | g(d)041 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0674 | Pure Stab | H | H006 | double | 10 | D1 p9 |  |
| F02-P0675 | Tunnel Bass | H | H070 | double | 08 | D1 p9 |  |
| F02-P0676 | Jump Brass | G | g(3)063 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0677 | Bagpipe | G | G110 | double | 06 | D1 p9 |  |
| F02-P0678 | ORCHESTRA | G | g(d)049 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0679 | Vantage Point | H | H007 | double | 10 | D1 p9 |  |
| F02-P0680 | Chump Bass | H | H071 | double | 08 | D1 p9 |  |
| F02-P0681 | Synth Brass | G | G064 | single | 10 | D1 p9 |  |
| F02-P0682 | Fiddle | G | G111 | single | 03 | D1 p9 |  |
| F02-P0683 | SFX Kit | G | g(d)057 | double | 15 | D1 p9 | document's own GM variant row |
| F02-P0684 | Pulse Down Pad | H | H008 | double | 10 | D1 p9 |  |
| F02-P0685 | Attack Sub Bass | H | H072 | double | 08 | D1 p9 |  |
| F02-P0686 | Synth Brass | G | g(1)064 | single | 10 | D1 p9 | document's own GM variant row |
| F02-P0687 | Shanai | G | G112 | single | 06 | D1 p9 |  |
| F02-P0688 | Reso Comp 2 | H | H009 | single | 10 | D1 p9 |  |
| F02-P0689 | www.hard.ne.jp | H | H073 | double | 08 | D1 p9 |  |
| F02-P0690 | Analog Brass | G | g(2)064 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0691 | Tinkle Bell | G | G113 | double | 02 | D1 p9 |  |
| F02-P0692 | Deep'nDark Bass | H | H010 | double | 08 | D1 p9 |  |
| F02-P0693 | Breaker Bass | H | H074 | single | 08 | D1 p9 |  |
| F02-P0694 | Soprano Sax | G | G065 | single | 06 | D1 p9 |  |
| F02-P0695 | Agogo | G | G114 | single | 02 | D1 p9 |  |
| F02-P0696 | Big Butt Bass | H | H011 | double | 08 | D1 p9 |  |
| F02-P0697 | Mother ship Bass | H | H075 | double | 08 | D1 p9 |  |
| F02-P0698 | Alto Sax | G | G066 | single | 06 | D1 p9 |  |
| F02-P0699 | Steel Drums | G | G115 | double | 15 | D1 p9 |  |
| F02-P0700 | Mondo Saw Bass | H | H012 | single | 08 | D1 p9 |  |
| F02-P0701 | Crank Da Bass | H | H076 | double | 08 | D1 p9 |  |
| F02-P0702 | Tenor Sax | G | G067 | single | 06 | D1 p9 |  |
| F02-P0703 | Woodblock | G | G116 | single | 15 | D1 p9 |  |
| F02-P0704 | Grawkr Bass | H | H013 | double | 08 | D1 p9 |  |
| F02-P0705 | Unison Bass | H | H077 | double | 08 | D1 p9 |  |
| F02-P0706 | Baritone Sax | G | G068 | single | 06 | D1 p9 |  |
| F02-P0707 | Castanets | G | g(1)116 | single | 15 | D1 p9 | document's own GM variant row |
| F02-P0708 | Warm Low Bass | H | H014 | single | 08 | D1 p9 |  |
| F02-P0709 | Roboto Basssss | H | H078 | double | 08 | D1 p9 |  |
| F02-P0710 | Oboe | G | G069 | single | 06 | D1 p9 |  |
| F02-P0711 | Taiko | G | G117 | single | 15 | D1 p9 |  |
| F02-P0712 | Lovers Lead | H | H015 | double | 11 | D1 p9 |  |
| F02-P0713 | Orbitone | H | H079 | single | 08 | D1 p9 |  |
| F02-P0714 | English Horn | G | G070 | single | 06 | D1 p9 |  |
| F02-P0715 | Concert BassDrum15 | G | g(1)117 | single |  | D1 p9 | document's own GM variant row |
| F02-P0716 | HipPop Lead | H | H016 | double | 11 | D1 p9 |  |
| F02-P0717 | Blaster Bass 2 | H | H080 | single | 08 | D1 p9 |  |
| F02-P0718 | Bassoon | G | G071 | single | 06 | D1 p9 |  |
| F02-P0719 | Melodic Tom | G | G118 | single | 15 | D1 p9 |  |
| F02-P0720 | Wacky Vox Lead | H | H017 | double | 11 | D1 p9 |  |
| F02-P0721 | Lonely Lead (-Y) | H | H081 | double | 11 | D1 p9 |  |
| F02-P0722 | Clarinet | G | G072 | single | 06 | D1 p9 |  |
| F02-P0723 | Melodic Tom | G | g(1)118 | single | 15 | D1 p9 | document's own GM variant row |
| F02-P0724 | LoungeSpace | H | H018 | double | 11 | D1 p9 |  |
| F02-P0725 | ClaviLead Synth | H | H082 | double | 11 | D1 p9 |  |
| F02-P0726 | Piccolo | G | G073 | single | 06 | D1 p9 |  |
| F02-P0727 | Synth Drum | G | G119 | single | 15 | D1 p9 |  |
| F02-P0728 | Wide Detune Lead | H | H019 | double | 11 | D1 p9 |  |
| F02-P0729 | PWM Lead | H | H083 | double | 11 | D1 p9 |  |
| F02-P0730 | Flute | G | G074 | single | 06 | D1 p9 |  |
| F02-P0731 | Analog Tom | G | g(1)119 | single | 15 | D1 p9 | document's own GM variant row |
| F02-P0732 | Big Fog Horn | H | H020 | double | 11 | D1 p9 |  |
| F02-P0733 | Fly Like An..... | H | H084 | single | 11 | D1 p9 |  |
| F02-P0734 | Recorder | G | G075 | single | 06 | D1 p9 |  |
| F02-P0735 | Electric Drum | G | g(2)119 | single | 15 | D1 p9 | document's own GM variant row |
| F02-P0736 | HardBody Lead | H | H021 | single | 11 | D1 p9 |  |
| F02-P0737 | Psychedelic Lead | H | H085 | double | 11 | D1 p9 |  |
| F02-P0738 | Pan Flute | G | G076 | single | 06 | D1 p9 |  |
| F02-P0739 | Reverse Cymbal | G | G120 | single | 13 | D1 p9 |  |
| F02-P0740 | KillerPadSW1Ribb | H | H022 | double | 09 | D1 p9 |  |
| F02-P0741 | Romantic Cluster | H | H086 | double | 11 | D1 p9 |  |
| F02-P0742 | Blown Bottle | G | G077 | double | 06 | D1 p9 |  |
| F02-P0743 | Gtr.Fret Noise | G | G121 | single | 07 | D1 p9 |  |
| F02-P0744 | Portamento Pad | H | H023 | double | 09 | D1 p9 |  |
| F02-P0745 | Free Detox PWM | H | H087 | double | 11 | D1 p9 |  |
| F02-P0746 | Shakuhachi | G | G078 | double | 06 | D1 p9 |  |
| F02-P0747 | Gtr.CuttingNoise | G | g(1)121 | single | 07 | D1 p9 | document's own GM variant row |
| F02-P0748 | Nebula Church | H | H024 | double | 09 | D1 p9 |  |
| F02-P0749 | ) HARD Liner ( | H | H088 | double | 11 | D1 p9 |  |
| F02-P0750 | Whistle | G | G079 | single | 06 | D1 p9 |  |
| F02-P0751 | Bass String Slap | G | g(2)121 | single | 08 | D1 p9 | document's own GM variant row |
| F02-P0752 | Flute Voices | H | H025 | double | 09 | D1 p9 |  |
| F02-P0753 | Sharp Blade | H | H089 | double | 11 | D1 p9 |  |
| F02-P0754 | Ocarina | G | G080 | single | 06 | D1 p9 |  |
| F02-P0755 | Breath Noise | G | G122 | single | 13 | D1 p9 |  |
| F02-P0756 | Pretty Pad | H | H026 | double | 09 | D1 p9 |  |
| F02-P0757 | Morpher Lead | H | H090 | double | 11 | D1 p9 |  |
| F02-P0758 | Detuned Square | G | G081 | double | 10 | D1 p9 |  |
| F02-P0759 | Flute Key Click | G | g(1)122 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0760 | RezzoVox Sweep | H | H027 | double | 12 | D1 p9 |  |
| F02-P0761 | HYPER BRAT | H | H091 | double | 11 | D1 p9 |  |
| F02-P0762 | Simple Square | G | g(1)081 | single | 11 | D1 p9 | document's own GM variant row |
| F02-P0763 | Seashore | G | G123 | double | 13 | D1 p9 |  |
| F02-P0764 | 5th Sweeper 2 | H | H028 | double | 12 | D1 p9 |  |
| F02-P0765 | Dialated | H | H092 | double | 11 | D1 p9 |  |
| F02-P0766 | Simple Sine | G | g(2)081 | single | 11 | D1 p9 | document's own GM variant row |
| F02-P0767 | Rain | G | g(1)123 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0768 | Heavy Image | H | H029 | double | 12 | D1 p9 |  |
| F02-P0769 | Monster Sync | H | H093 | double | 11 | D1 p9 |  |
| F02-P0770 | Detuned Sawtooth | G | G082 | double | 10 | D1 p9 |  |
| F02-P0771 | Thunder | G | g(2)123 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0772 | Chromy BPF Sweep | H | H030 | double | 12 | D1 p9 |  |
| F02-P0773 | Dist-Lead | H | H094 | single | 11 | D1 p9 |  |
| F02-P0774 | Simple Sawtooth | G | g(1)082 | single | 10 | D1 p9 | document's own GM variant row |
| F02-P0775 | Wind | G | g(3)123 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0776 | Xtreme Kit | H | H031 | double | 15 | D1 p9 |  |
| F02-P0777 | Pulse Padding | H | H095 | double | 09 | D1 p9 |  |
| F02-P0778 | Sawtooth + Pulse | G | g(2)082 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0779 | Stream | G | g(4)123 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0780 | PhatBoy Kit | H | H032 | double | 15 | D1 p9 |  |
| F02-P0781 | Thick Pad | H | H096 | double | 09 | D1 p9 |  |
| F02-P0782 | Double Sawtooth | G | g(3)082 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0783 | Bubble | G | g(5)123 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0784 | Dissolve Hit | H | H033 | double | 14 | D1 p9 |  |
| F02-P0785 | Cloudy Pad | H | H097 | double | 09 | D1 p9 |  |
| F02-P0786 | Sequenced Analog14 | G | g(4)082 | double |  | D1 p9 | document's own GM variant row |
| F02-P0787 | Bird Tweet 1 | G | G124 | double | 13 | D1 p9 |  |
| F02-P0788 | 70s Analog Piano | H | H034 | double | 00 | D1 p9 |  |
| F02-P0789 | London's Pride | H | H098 | double | 09 | D1 p9 |  |
| F02-P0790 | Synth Calliope | G | G083 | double | 10 | D1 p9 |  |
| F02-P0791 | Dog | G | g(1)124 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0792 | Analog Piano 2 | H | H035 | double | 10 | D1 p9 |  |
| F02-P0793 | No Exit Eire | H | H099 | double | 09 | D1 p9 |  |
| F02-P0794 | Chiff Lead | G | G084 | double | 10 | D1 p9 |  |
| F02-P0795 | Horse Gallop | G | g(2)124 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0796 | Tingles | H | H036 | double | 10 | D1 p9 |  |
| F02-P0797 | ambr:sea | H | H100 | double | 09 | D1 p9 |  |
| F02-P0798 | Charang | G | G085 | double | 11 | D1 p9 |  |
| F02-P0799 | Bird Tweet 2 | G | g(3)124 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0800 | Glassy & Gritty | H | H037 | double | 10 | D1 p9 |  |
| F02-P0801 | <EVEREST> | H | H101 | double | 09 | D1 p9 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0802 | Wire Lead | G | g(1)085 | double | 11 | D1 p9 | document's own GM variant row |
| F02-P0803 | Telephone Ring | G | G125 | single | 13 | D1 p9 |  |
| F02-P0804 | Boinker Synth | H | H038 | double | 10 | D1 p9 |  |
| F02-P0805 | Ahh..Real Osc's! | H | H102 | double | 09 | D1 p9 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0806 | Air Voice | G | G086 | double | 04 | D1 p9 |  |
| F02-P0807 | Telephone Ring | G | g(1)125 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0808 | Old Hi Pad | H | H039 | double | 10 | D1 p9 |  |
| F02-P0809 | Fond Memories | H | H103 | double | 09 | D1 p9 |  |
| F02-P0810 | 5th Sawtooth | G | G087 | double | 11 | D1 p9 |  |
| F02-P0811 | Door Creaking | G | g(2)125 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0812 | Detuned SynBrass | H | H040 | double | 10 | D1 p9 |  |
| F02-P0813 | Flautistic Ens | H | H104 | double | 12 | D1 p9 |  |
| F02-P0814 | Bass & Lead | G | G088 | single | 11 | D1 p9 |  |
| F02-P0815 | Door Slamming | G | g(3)125 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0816 | Massive Pad | H | H041 | double | 10 | D1 p9 |  |
| F02-P0817 | Bombers Pad | H | H105 | double | 12 | D1 p9 |  |
| F02-P0818 | Soft Wurl | G | g(1)088 | double | 11 | D1 p9 | document's own GM variant row |
| F02-P0819 | Scratch | G | g(4)125 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0820 | Eurosynth | H | H042 | double | 10 | D1 p9 |  |
| F02-P0821 | Dream Catcher | H | H106 | double | 12 | D1 p9 |  |
| F02-P0822 | Fantasia | G | G089 | double | 10 | D1 p9 |  |
| F02-P0823 | Wind Chime | G | g(5)125 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0824 | Big Dancer | H | H043 | double | 10 | D1 p9 |  |
| F02-P0825 | Spinning Pad | H | H107 | double | 12 | D1 p9 |  |
| F02-P0826 | Warm Pad | G | G090 | single | 09 | D1 p9 |  |
| F02-P0827 | Helicopter | G | G126 | double | 13 | D1 p9 |  |
| F02-P0828 | Fat Fogger | H | H044 | double | 10 | D1 p9 |  |
| F02-P0829 | Introduction | H | H108 | double | 12 | D1 p9 |  |
| F02-P0830 | Sine Pad | G | g(1)090 | double | 09 | D1 p9 | document's own GM variant row |
| F02-P0831 | Car Engine | G | g(1)126 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0832 | Blade Run | H | H045 | double | 10 | D1 p9 |  |
| F02-P0833 | AStabInTheDark | H | H109 | double | 12 | D1 p9 |  |
| F02-P0834 | Polyphonic Synth | G | G091 | double | 10 | D1 p9 |  |
| F02-P0835 | Car Stop | G | g(2)126 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0836 | Xtreme Edge | H | H046 | double | 10 | D1 p9 |  |
| F02-P0837 | Ghost Train | H | H110 | double | 12 | D1 p9 |  |
| F02-P0838 | Space Voice | G | G092 | double | 04 | D1 p9 |  |
| F02-P0839 | Car Pass | G | g(3)126 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0840 | Space Pad | H | H047 | double | 10 | D1 p9 |  |
| F02-P0841 | Breath of God | H | H111 | double | 04 | D1 p9 |  |
| F02-P0842 | Itopia | G | g(1)092 | double | 04 | D1 p9 | document's own GM variant row |
| F02-P0843 | Car Crash | G | g(4)126 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0844 | Dreamy Punk | H | H048 | double | 10 | D1 p9 |  |
| F02-P0845 | < OpenMind > | H | H112 | double | 04 | D1 p9 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0846 | Bowed Glass | G | G093 | double | 09 | D1 p9 |  |
| F02-P0847 | Siren | G | g(5)126 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0848 | Pulse Width Mod | H | H049 | double | 10 | D1 p9 |  |
| F02-P0849 | DemBeatsMan | H | H113 | double | 15 | D1 p9 |  |
| F02-P0850 | Metallic Pad | G | G094 | double | 09 | D1 p9 |  |
| F02-P0851 | Train | G | g(6)126 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0852 | Dyna-Saw-Rus-Rex | H | H050 | double | 10 | D1 p9 |  |
| F02-P0853 | Techno Perc | H | H114 | double | 15 | D1 p9 |  |
| F02-P0854 | Halo Pad | G | G095 | double | 04 | D1 p9 |  |
| F02-P0855 | Jetplane | G | g(7)126 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0856 | Pure Pulse | H | H051 | double | 10 | D1 p9 |  |
| F02-P0857 | Nu Trix Kit | H | H115 | double | 15 | D1 p9 |  |
| F02-P0858 | Sweep Pad | G | G096 | single | 12 | D1 p9 |  |
| F02-P0859 | Starship | G | g(8)126 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0860 | Pure Saw | H | H052 | double | 10 | D1 p9 |  |
| F02-P0861 | ISaw&ISquared | H | H116 | double | 14 | D1 p9 |  |
| F02-P0862 | Ice Rain | G | G097 | double | 10 | D1 p9 |  |
| F02-P0863 | Burst Noise | G | g(9)126 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0864 | Phone Boy | H | H053 | double | 10 | D1 p9 |  |
| F02-P0865 | Tremolo Sine | H | H117 | double | 14 | D1 p9 |  |
| F02-P0866 | Sound Track | G | G098 | double | 09 | D1 p9 |  |
| F02-P0867 | Applause | G | G127 | double | 13 | D1 p9 |  |
| F02-P0868 | Minority | H | H054 | double | 10 | D1 p9 |  |
| F02-P0869 | SQ Synth | H | H118 | double | 14 | D1 p9 |  |
| F02-P0870 | Crystal | G | G099 | double | 02 | D1 p9 |  |
| F02-P0871 | Laughing | G | g(1)127 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0872 | RUDE Puncher | H | H055 | double | 10 | D1 p9 |  |
| F02-P0873 | Industrial Stab | H | H119 | double | 14 | D1 p9 |  |
| F02-P0874 | Synth Mallet | G | g(1)099 | single | 02 | D1 p9 | document's own GM variant row |
| F02-P0875 | Screaming | G | g(2)127 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0876 | Circus | H | H056 | double | 10 | D1 p9 |  |
| F02-P0877 | ArppHEAD | H | H120 | double | 14 | D1 p9 |  |
| F02-P0878 | Atmosphere | G | G100 | double | 10 | D1 p9 |  |
| F02-P0879 | Punch | G | g(3)127 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0880 | Rezzo Chrome | H | H057 | double | 10 | D1 p9 |  |
| F02-P0881 | SimmerReso | H | H121 | double | 14 | D1 p9 |  |
| F02-P0882 | Brightness | G | G101 | double | 10 | D1 p9 |  |
| F02-P0883 | Heart Beat | G | g(4)127 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0884 | Arp Rez Poly | H | H058 | double | 10 | D1 p9 |  |
| F02-P0885 | Trance Hit | H | H122 | double | 14 | D1 p9 |  |
| F02-P0886 | Goblins | G | G102 | double | 12 | D1 p9 |  |
| F02-P0887 | Footsteps | G | g(5)127 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0888 | Tube Vox | H | H059 | double | 10 | D1 p9 |  |
| F02-P0889 | Mega Pulse Hit | H | H123 | double | 14 | D1 p9 |  |
| F02-P0890 | Echo Drops | G | G103 | single | 10 | D1 p9 |  |
| F02-P0891 | Gun Shot | G | G128 | single | 13 | D1 p9 |  |
| F02-P0892 | Hip Hop Sub | H | H060 | double | 08 | D1 p9 |  |
| F02-P0893 | Modal Hit | H | H124 | double | 14 | D1 p9 |  |
| F02-P0894 | Echo Bell | G | g(1)103 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0895 | Machine Gun | G | g(1)128 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0896 | Wiked SINE Bass | H | H061 | double | 08 | D1 p9 |  |
| F02-P0897 | Cookin Oil | H | H125 | double | 14 | D1 p9 |  |
| F02-P0898 | Echo Pan | G | g(2)103 | double | 10 | D1 p9 | document's own GM variant row |
| F02-P0899 | Lasergun | G | g(2)128 | double | 13 | D1 p9 | document's own GM variant row |
| F02-P0900 | Black Low Bass | H | H062 | double | 08 | D1 p9 |  |
| F02-P0901 | 7th House Hit | H | H126 | double | 14 | D1 p9 |  |
| F02-P0902 | Star Theme | G | G104 | double | 10 | D1 p9 |  |
| F02-P0903 | Explosion | G | g(3)128 | single | 13 | D1 p9 | document's own GM variant row |
| F02-P0904 | Nu Decay BASS | H | H063 | double | 08 | D1 p9 |  |
| F02-P0905 | Sync Slider | H | H127 | double | 14 | D1 p9 |  |
| F02-P0906 | EXP Strings Vib | I | I000 | double | 03 | D1 p10 |  |
| F02-P0907 | Orch.OboeD'Amore | I | I064 | single | 06 | D1 p10 |  |
| F02-P0908 | Noisy Stabber | J | J000 | double | 10 | D1 p10 |  |
| F02-P0909 | Ana Brass/Lead | J | J064 | double | 10 | D1 p10 |  |
| F02-P0910 | Synth Sweeper | K | K000 | double | 10 | D1 p10 |  |
| F02-P0911 | QuartetStr D.Vib | I | I001 | double | 03 | D1 p10 |  |
| F02-P0912 | Orch. Oboe Vib | I | I065 | single | 06 | D1 p10 |  |
| F02-P0913 | Acoustic Piano | J | J001 | single | 00 | D1 p10 |  |
| F02-P0914 | 90's Piano | J | J065 | double | 00 | D1 p10 |  |
| F02-P0915 | Attack Piano | K | K001 | double | 00 | D1 p10 |  |
| F02-P0916 | Wide Oct Strings | I | I002 | double | 03 | D1 p10 |  |
| F02-P0917 | OrchEnglishHorn2 | I | I066 | single | 06 | D1 p10 |  |
| F02-P0918 | Chipper Dayglow | J | J002 | double | 14 | D1 p10 |  |
| F02-P0919 | Wild Arp | J | J066 | double | 14 | D1 p10 |  |
| F02-P0920 | StaccatoPizzHit | K | K002 | double | 14 | D1 p10 |  |
| F02-P0921 | StringsAtk->Trem | I | I003 | double | 03 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0922 | BeBop Clarinet | I | I067 | single | 06 | D1 p10 |  |
| F02-P0923 | Legato Strings | J | J003 | double | 03 | D1 p10 |  |
| F02-P0924 | Stereo Strings | J | J067 | double | 03 | D1 p10 |  |
| F02-P0925 | Octave Strings | K | K003 | double | 03 | D1 p10 |  |
| F02-P0926 | Staccato Strings | I | I004 | double | 03 | D1 p10 |  |
| F02-P0927 | BsClarnt+Trl<SW> | I | I068 | single | 06 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0928 | ! {Tricky} Kit ! | J | J004 | double | 15 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0929 | HipHop Kit | J | J068 | double | 15 | D1 p10 |  |
| F02-P0930 | Jazz/Brush Kits | K | K004 | double | 15 | D1 p10 |  |
| F02-P0931 | EnglishHorn Vib2 | I | I005 | single | 06 | D1 p10 |  |
| F02-P0932 | ClarinetsSpiccat | I | I069 | double | 06 | D1 p10 |  |
| F02-P0933 | Acoustic Guitar | J | J005 | double | 07 | D1 p10 |  |
| F02-P0934 | Jazz Guitar | J | J069 | single | 07 | D1 p10 |  |
| F02-P0935 | FingertipsGuitar | K | K005 | double | 07 | D1 p10 |  |
| F02-P0936 | Flute Section | I | I006 | double | 06 | D1 p10 |  |
| F02-P0937 | OrchContraBasson | I | I070 | single | 06 | D1 p10 |  |
| F02-P0938 | Nasty Bass | J | J006 | double | 08 | D1 p10 |  |
| F02-P0939 | Euro 8va Bass | J | J070 | double | 08 | D1 p10 |  |
| F02-P0940 | Dark R&B Bass | K | K006 | double | 08 | D1 p10 |  |
| F02-P0941 | OrchBs.Clarinet2 | I | I007 | single | 06 | D1 p10 |  |
| F02-P0942 | Flute & Eng.Horn | I | I071 | double | 06 | D1 p10 |  |
| F02-P0943 | BX3 Velo Switch | J | J007 | double | 01 | D1 p10 |  |
| F02-P0944 | Distortion Organ | J | J071 | double | 01 | D1 p10 |  |
| F02-P0945 | Old Tone-Wheel | K | K007 | double | 01 | D1 p10 |  |
| F02-P0946 | Clarinet Section | I | I008 | double | 06 | D1 p10 |  |
| F02-P0947 | EXP WindEnsemble | I | I072 | double | 06 | D1 p10 |  |
| F02-P0948 | Rez. Down | J | J008 | double | 09 | D1 p10 |  |
| F02-P0949 | Tsunami Waves | J | J072 | double | 09 | D1 p10 |  |
| F02-P0950 | The Pad | K | K008 | double | 09 | D1 p10 |  |
| F02-P0951 | Orch. Bassoon | I | I009 | single | 06 | D1 p10 |  |
| F02-P0952 | Wind Mixture | I | I073 | double | 06 | D1 p10 |  |
| F02-P0953 | Fat Brass | J | J009 | double | 05 | D1 p10 |  |
| F02-P0954 | Glen & The Boys | J | J073 | double | 05 | D1 p10 |  |
| F02-P0955 | Brass Expression | K | K009 | double | 05 | D1 p10 |  |
| F02-P0956 | F.Horns SFZ | I | I010 | double | 05 | D1 p10 |  |
| F02-P0957 | TrumpetsSpiccato | I | I074 | double | 05 | D1 p10 |  |
| F02-P0958 | <Techno Vox Box> | J | J010 | double | 13 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P0959 | Steam Sweeps | J | J074 | double | 13 | D1 p10 |  |
| F02-P0960 | PiezoMix Guitar | K | K010 | double | 07 | D1 p10 |  |
| F02-P0961 | StereoTrombones | I | I011 | double | 05 | D1 p10 |  |
| F02-P0962 | Trumpets SFZ | I | I075 | double | 05 | D1 p10 |  |
| F02-P0963 | Fresh Breath | J | J011 | double | 04 | D1 p10 |  |
| F02-P0964 | White Pad EP | J | J075 | double | 00 | D1 p10 |  |
| F02-P0965 | Slow Choir ST | K | K011 | double | 04 | D1 p10 |  |
| F02-P0966 | PiccoloTrp Spicc | I | I012 | single | 05 | D1 p10 |  |
| F02-P0967 | St. Trumpets mf | I | I076 | double | 05 | D1 p10 |  |
| F02-P0968 | Smooth Sine Lead | J | J012 | double | 11 | D1 p10 |  |
| F02-P0969 | Old & Analog | J | J076 | double | 11 | D1 p10 |  |
| F02-P0970 | Phat Saw Lead | K | K012 | double | 11 | D1 p10 |  |
| F02-P0971 | Bright Trumpets | I | I013 | double | 05 | D1 p10 |  |
| F02-P0972 | Stereo F.Horns | I | I077 | double | 05 | D1 p10 |  |
| F02-P0973 | Sax Ensemble | J | J013 | double | 06 | D1 p10 |  |
| F02-P0974 | DoubleReed | J | J077 | single | 06 | D1 p10 |  |
| F02-P0975 | Old Shakuhachi | K | K013 | double | 06 | D1 p10 |  |
| F02-P0976 | SoftTipsMarimba | I | I014 | double | 02 | D1 p10 |  |
| F02-P0977 | F.Horns Spiccato | I | I078 | double | 05 | D1 p10 |  |
| F02-P0978 | Swirling Dreams | J | J014 | double | 12 | D1 p10 |  |
| F02-P0979 | Aqua Phonics | J | J078 | double | 12 | D1 p10 |  |
| F02-P0980 | OXYGEN | K | K014 | double | 12 | D1 p10 |  |
| F02-P0981 | EXP TubularBells | I | I015 | double | 02 | D1 p10 |  |
| F02-P0982 | Stereo DarkHorns | I | I079 | double | 05 | D1 p10 |  |
| F02-P0983 | Monkey Skulls | J | J015 | double | 02 | D1 p10 |  |
| F02-P0984 | Ensemble Bell | J | J079 | single | 02 | D1 p10 |  |
| F02-P0985 | Velo Kalimba | K | K015 | double | 02 | D1 p10 |  |
| F02-P0986 | Going to Gong | I | I016 | double | 15 | D1 p10 |  |
| F02-P0987 | F.Horns D Spicc. | I | I080 | double | 05 | D1 p10 |  |
| F02-P0988 | Metalic Rez | J | J016 | double | 10 | D1 p10 |  |
| F02-P0989 | Cosmic Furnace | J | J080 | double | 10 | D1 p10 |  |
| F02-P0990 | Future Syn Pad | K | K016 | double | 10 | D1 p10 |  |
| F02-P0991 | StringPhraseMix1 | I | I017 | double | 14 | D1 p10 |  |
| F02-P0992 | F.Horns D SFZ | I | I081 | double | 05 | D1 p10 |  |
| F02-P0993 | Studio Stage EP | J | J017 | double | 00 | D1 p10 |  |
| F02-P0994 | Velo Whirly | J | J081 | double | 00 | D1 p10 |  |
| F02-P0995 | Dyno Tine EP | K | K017 | double | 00 | D1 p10 |  |
| F02-P0996 | Legato Violins | I | I018 | double | 03 | D1 p10 |  |
| F02-P0997 | Largo F.Horns | I | I082 | double | 05 | D1 p10 |  |
| F02-P0998 | Flip Blip | J | J018 | double | 14 | D1 p10 |  |
| F02-P0999 | Techno Organ | J | J082 | double | 14 | D1 p10 |  |
| F02-P1000 | Brass Impact Hit | K | K018 | double | 14 | D1 p10 |  |
| F02-P1001 | ExpressViolns | I | I019 | double | 03 | D1 p10 |  |
| F02-P1002 | Trombones Spicc. | I | I083 | double | 05 | D1 p10 |  |
| F02-P1003 | Camera Strings | J | J019 | double | 03 | D1 p10 |  |
| F02-P1004 | Few Bows Here | J | J083 | double | 03 | D1 p10 |  |
| F02-P1005 | String Quartet | K | K019 | single | 03 | D1 p10 |  |
| F02-P1006 | Staccato Violins | I | I020 | double | 03 | D1 p10 |  |
| F02-P1007 | Trombones SFZ | I | I084 | double | 05 | D1 p10 |  |
| F02-P1008 | House Kit | J | J020 | double | 15 | D1 p10 |  |
| F02-P1009 | Mega Drum Hit | J | J084 | double | 15 | D1 p10 |  |
| F02-P1010 | Processed Kit | K | K020 | double | 15 | D1 p10 |  |
| F02-P1011 | Legato Violas | I | I021 | double | 03 | D1 p10 |  |
| F02-P1012 | St. Trombones | I | I085 | double | 05 | D1 p10 |  |
| F02-P1013 | Chorus E.Guitar | J | J021 | double | 07 | D1 p10 |  |
| F02-P1014 | Mute Monster | J | J085 | double | 07 | D1 p10 |  |
| F02-P1015 | Vintage Stratt | K | K021 | single | 07 | D1 p10 |  |
| F02-P1016 | Staccato Violas | I | I022 | double | 03 | D1 p10 |  |
| F02-P1017 | TBones&Brass | I | I086 | double | 05 | D1 p10 |  |
| F02-P1018 | Acoustic Bass | J | J022 | single | 08 | D1 p10 |  |
| F02-P1019 | Stein Bass | J | J086 | double | 08 | D1 p10 |  |
| F02-P1020 | Fretless Bass | K | K022 | single | 08 | D1 p10 |  |
| F02-P1021 | LargoRosinViolas | I | I023 | double | 03 | D1 p10 |  |
| F02-P1022 | TBones&TPets | I | I087 | double | 05 | D1 p10 |  |
| F02-P1023 | M1 Organ | J | J023 | double | 01 | D1 p10 |  |
| F02-P1024 | Perc Short Decay | J | J087 | double | 01 | D1 p10 |  |
| F02-P1025 | Tekno Organ*Bass | K | K023 | double | 08 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1026 | ExpressCelli ST | I | I024 | double | 03 | D1 p10 |  |
| F02-P1027 | TBones & FHorns | I | I088 | double | 05 | D1 p10 |  |
| F02-P1028 | Digi Ice Pad | J | J024 | double | 09 | D1 p10 |  |
| F02-P1029 | Antartic Wind | J | J088 | double | 09 | D1 p10 |  |
| F02-P1030 | Brass Pad | K | K024 | double | 09 | D1 p10 |  |
| F02-P1031 | Staccato Celli | I | I025 | double | 03 | D1 p10 |  |
| F02-P1032 | Orchestral Brass | I | I089 | double | 05 | D1 p10 |  |
| F02-P1033 | Trumpet | J | J025 | single | 05 | D1 p10 |  |
| F02-P1034 | Trombone Hard | J | J089 | single | 05 | D1 p10 |  |
| F02-P1035 | Muted Trumpet | K | K025 | single | 05 | D1 p10 |  |
| F02-P1036 | ExpressBasses | I | I026 | double | 03 | D1 p10 |  |
| F02-P1037 | EXP PiccoloTrp | I | I090 | single | 05 | D1 p10 |  |
| F02-P1038 | Sonic Blast | J | J026 | double | 13 | D1 p10 |  |
| F02-P1039 | Orbiting Probes | J | J090 | double | 13 | D1 p10 |  |
| F02-P1040 | Fisa Americana | K | K026 | double | 06 | D1 p10 |  |
| F02-P1041 | Staccato Basses | I | I027 | double | 03 | D1 p10 |  |
| F02-P1042 | Largo PiccoloTrp | I | I091 | single | 05 | D1 p10 |  |
| F02-P1043 | Choir of Light | J | J027 | double | 04 | D1 p10 |  |
| F02-P1044 | Phantom Of Tine | J | J091 | double | 00 | D1 p10 |  |
| F02-P1045 | Vocalesque | K | K027 | double | 04 | D1 p10 |  |
| F02-P1046 | String Orch. ST | I | I028 | double | 03 | D1 p10 |  |
| F02-P1047 | Bright Trumpet | I | I092 | single | 05 | D1 p10 |  |
| F02-P1048 | Rezbo | J | J028 | double | 10 | D1 p10 |  |
| F02-P1049 | Seq DDL Lead | J | J092 | double | 11 | D1 p10 |  |
| F02-P1050 | Glide Lead | K | K028 | double | 11 | D1 p10 |  |
| F02-P1051 | Wide Strings | I | I029 | double | 03 | D1 p10 |  |
| F02-P1052 | Solo Trumpet | I | I093 | single | 05 | D1 p10 |  |
| F02-P1053 | TenorSax Growl-Y | J | J029 | double | 06 | D1 p10 |  |
| F02-P1054 | BariSax Growl | J | J093 | double | 06 | D1 p10 |  |
| F02-P1055 | TenorSax Brth.-Y | K | K029 | double | 06 | D1 p10 |  |
| F02-P1056 | EXP Strings ST | I | I030 | double | 03 | D1 p10 |  |
| F02-P1057 | Trumpet Spiccato | I | I094 | single | 05 | D1 p10 |  |
| F02-P1058 | Tinklin' Pad | J | J030 | double | 12 | D1 p10 |  |
| F02-P1059 | Percussive Hits | J | J094 | double | 14 | D1 p10 |  |
| F02-P1060 | Transformation | K | K030 | double | 12 | D1 p10 |  |
| F02-P1061 | EXP LiteStrings | I | I031 | double | 03 | D1 p10 |  |
| F02-P1062 | Trumpet pp | I | I095 | single | 05 | D1 p10 |  |
| F02-P1063 | VS Bell Boy | J | J031 | double | 02 | D1 p10 |  |
| F02-P1064 | Vibraphone | J | J095 | double | 02 | D1 p10 |  |
| F02-P1065 | Thin Bell-s-park | K | K031 | single | 02 | D1 p10 |  |
| F02-P1066 | LargoWideStrings | I | I032 | double | 03 | D1 p10 |  |
| F02-P1067 | Express Trombone | I | I096 | single | 05 | D1 p10 |  |
| F02-P1068 | Power Snap Synth | J | J032 | double | 10 | D1 p10 |  |
| F02-P1069 | Digital PolySix | J | J096 | double | 10 | D1 p10 |  |
| F02-P1070 | Pop Synth Pad | K | K032 | double | 10 | D1 p10 |  |
| F02-P1071 | Few Camera Str | I | I033 | double | 03 | D1 p10 |  |
| F02-P1072 | TromboneSpiccato | I | I097 | single | 05 | D1 p10 |  |
| F02-P1073 | Romance Piano | J | J033 | double | 00 | D1 p10 |  |
| F02-P1074 | Clav | J | J097 | single | 00 | D1 p10 |  |
| F02-P1075 | Piano Pad 1 | K | K033 | double | 00 | D1 p10 |  |
| F02-P1076 | 1st LinesStrings | I | I034 | double | 03 | D1 p10 |  |
| F02-P1077 | Trombone mp | I | I098 | single | 05 | D1 p10 |  |
| F02-P1078 | Arp Angeles | J | J034 | double | 14 | D1 p10 |  |
| F02-P1079 | Goa Lover | J | J098 | double | 13 | D1 p10 |  |
| F02-P1080 | Karma Sutra | K | K034 | double | 08 | D1 p10 |  |
| F02-P1081 | EXP Vlins&Vlas | I | I035 | double | 03 | D1 p10 |  |
| F02-P1082 | Trombone pp | I | I099 | single | 05 | D1 p10 |  |
| F02-P1083 | Arco Strings | J | J035 | double | 03 | D1 p10 |  |
| F02-P1084 | AnalogStrings1&2 | J | J099 | double | 03 | D1 p10 |  |
| F02-P1085 | Symphonic Bows | K | K035 | double | 03 | D1 p10 |  |
| F02-P1086 | EXP Vlins&Celli | I | I036 | double | 03 | D1 p10 |  |
| F02-P1087 | Slow Trombone | I | I100 | single | 05 | D1 p10 |  |
| F02-P1088 | Standard Kit 1 | J | J036 | double | 15 | D1 p10 |  |
| F02-P1089 | BD&SD Kit 1 | J | J100 | double | 15 | D1 p10 |  |
| F02-P1090 | Standard Kit 2 | K | K036 | double | 15 | D1 p10 |  |
| F02-P1091 | EXP Vlas&Celli | I | I037 | double | 03 | D1 p10 |  |
| F02-P1092 | Open F.Horn | I | I101 | single | 05 | D1 p10 |  |
| F02-P1093 | Wet Dist. Guitar | J | J037 | double | 07 | D1 p10 |  |
| F02-P1094 | Nylon Guitar | J | J101 | double | 07 | D1 p10 |  |
| F02-P1095 | Amp D. Guitar | K | K037 | double | 07 | D1 p10 |  |
| F02-P1096 | EXP Celli&Bass | I | I038 | double | 03 | D1 p10 |  |
| F02-P1097 | F.Horn Spiccato | I | I102 | single | 05 | D1 p10 |  |
| F02-P1098 | 30303 Mega Bass | J | J038 | double | 08 | D1 p10 |  |
| F02-P1099 | Jungle Rez Bass | J | J102 | double | 08 | D1 p10 |  |
| F02-P1100 | 30303 Square | K | K038 | double | 08 | D1 p10 |  |
| F02-P1101 | EXP Oct Vlin&Cl | I | I039 | double | 03 | D1 p10 |  |
| F02-P1102 | Closed F.Horn | I | I103 | single | 05 | D1 p10 |  |
| F02-P1103 | Dark Jazz-Organ | J | J039 | double | 01 | D1 p10 |  |
| F02-P1104 | Full Drawbars | J | J103 | double | 01 | D1 p10 |  |
| F02-P1105 | Sine DWGS-Organ | K | K039 | double | 01 | D1 p10 |  |
| F02-P1106 | EXP Oct Cl&Bs | I | I040 | double | 03 | D1 p10 |  |
| F02-P1107 | Expressive Tuba | I | I104 | single | 05 | D1 p10 |  |
| F02-P1108 | Clouds of Air | J | J040 | double | 09 | D1 p10 |  |
| F02-P1109 | Money Pad | J | J104 | double | 09 | D1 p10 |  |
| F02-P1110 | Simple Sine Pad | K | K040 | double | 09 | D1 p10 |  |
| F02-P1111 | Strings&Brass1-Y | I | I041 | double | 03 | D1 p10 |  |
| F02-P1112 | Tuba Spiccato | I | I105 | single | 05 | D1 p10 |  |
| F02-P1113 | SFZ Brass ST | J | J041 | double | 05 | D1 p10 |  |
| F02-P1114 | Burnin' Brass | J | J105 | double | 05 | D1 p10 |  |
| F02-P1115 | Octave Brass | K | K041 | double | 05 | D1 p10 |  |
| F02-P1116 | Strings&Brass2-Y | I | I042 | double | 03 | D1 p10 |  |
| F02-P1117 | Tuba mp | I | I106 | single | 05 | D1 p10 |  |
| F02-P1118 | Loop-Iteria | J | J042 | double | 13 | D1 p10 |  |
| F02-P1119 | VenusianStories | J | J106 | double | 13 | D1 p10 |  |
| F02-P1120 | CyborgFactoryHit | K | K042 | double | 13 | D1 p10 |  |
| F02-P1121 | Largo Brs&Str | I | I043 | double | 03 | D1 p10 |  |
| F02-P1122 | Orch. Xylophone | I | I107 | single | 02 | D1 p10 |  |
| F02-P1123 | Oooh Voices ST | J | J043 | double | 04 | D1 p10 |  |
| F02-P1124 | SynPiano X | J | J107 | single | 00 | D1 p10 |  |
| F02-P1125 | E.G. Harmonics | K | K043 | single | 07 | D1 p10 |  |
| F02-P1126 | Marc/Trem Violns | I | I044 | double | 03 | D1 p10 |  |
| F02-P1127 | HardTipsMarimba | I | I108 | double | 02 | D1 p10 |  |
| F02-P1128 | HipHop Lead | J | J044 | double | 11 | D1 p10 |  |
| F02-P1129 | Syncro City | J | J108 | double | 11 | D1 p10 |  |
| F02-P1130 | Auto Pilot | K | K044 | single | 14 | D1 p10 |  |
| F02-P1131 | Marc/Trem Violas | I | I045 | double | 03 | D1 p10 |  |
| F02-P1132 | Vibraphone <Sw1> | I | I109 | double | 02 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1133 | Flute | J | J045 | single | 06 | D1 p10 |  |
| F02-P1134 | Recorder | J | J109 | single | 06 | D1 p10 |  |
| F02-P1135 | BambuSilverFlute | K | K045 | double | 06 | D1 p10 |  |
| F02-P1136 | Marc/Trem Celli | I | I046 | double | 03 | D1 p10 |  |
| F02-P1137 | Orchestra Bell | I | I110 | single | 02 | D1 p10 |  |
| F02-P1138 | Sinistar*Android | J | J046 | double | 12 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1139 | Astral Dreams | J | J110 | double | 12 | D1 p10 |  |
| F02-P1140 | Dolphin Ride | K | K046 | double | 12 | D1 p10 |  |
| F02-P1141 | Marc/Trem Basses | I | I047 | double | 03 | D1 p10 |  |
| F02-P1142 | Orch. Timpani | I | I111 | double | 15 | D1 p10 |  |
| F02-P1143 | Log Drum | J | J047 | double | 02 | D1 p10 |  |
| F02-P1144 | Bali Gamelon | J | J111 | double | 02 | D1 p10 |  |
| F02-P1145 | Marimba | K | K047 | single | 02 | D1 p10 |  |
| F02-P1146 | Marc/TremStrings | I | I048 | double | 03 | D1 p10 |  |
| F02-P1147 | Tuned Timpani | I | I112 | single | 15 | D1 p10 |  |
| F02-P1148 | Rez. Sweep | J | J048 | double | 10 | D1 p10 |  |
| F02-P1149 | Super Saw Brass | J | J112 | double | 10 | D1 p10 |  |
| F02-P1150 | Gliding Squares | K | K048 | double | 10 | D1 p10 |  |
| F02-P1151 | TremoloToStrings | I | I049 | double | 03 | D1 p10 |  |
| F02-P1152 | Gran Cassa ff | I | I113 | double | 15 | D1 p10 |  |
| F02-P1153 | R&B E.Piano | J | J049 | double | 00 | D1 p10 |  |
| F02-P1154 | Sweeping EP | J | J113 | double | 00 | D1 p10 |  |
| F02-P1155 | Night Tines EP | K | K049 | double | 00 | D1 p10 |  |
| F02-P1156 | StringsToTremolo | I | I050 | double | 03 | D1 p10 |  |
| F02-P1157 | Gran Cassa mf | I | I114 | double | 15 | D1 p10 |  |
| F02-P1158 | Arp Twins | J | J050 | double | 14 | D1 p10 |  |
| F02-P1159 | Jungle Melody | J | J114 | double | 14 | D1 p10 |  |
| F02-P1160 | Mondo'Rimba | K | K050 | double | 02 | D1 p10 |  |
| F02-P1161 | Violins Pizz ST | I | I051 | double | 03 | D1 p10 |  |
| F02-P1162 | Orch. Cymbal | I | I115 | single | 15 | D1 p10 |  |
| F02-P1163 | PizzAnsamble | J | J051 | double | 03 | D1 p10 |  |
| F02-P1164 | WatcherOfTheSky | J | J115 | double | 03 | D1 p10 |  |
| F02-P1165 | HybridStrg/Choir | K | K051 | double | 03 | D1 p10 |  |
| F02-P1166 | Violas Pizz ST | I | I052 | double | 03 | D1 p10 |  |
| F02-P1167 | Orch. Harp | I | I116 | double | 07 | D1 p10 |  |
| F02-P1168 | Psycho Kit | J | J052 | double | 15 | D1 p10 |  |
| F02-P1169 | Orchestra&Ethnic | J | J116 | double | 15 | D1 p10 |  |
| F02-P1170 | Cymbals Kit | K | K052 | double | 15 | D1 p10 |  |
| F02-P1171 | Celli Pizz ST | I | I053 | double | 03 | D1 p10 |  |
| F02-P1172 | Celtic Harp 1 | I | I117 | double | 07 | D1 p10 |  |
| F02-P1173 | Vox Wah Chicks | J | J053 | single | 07 | D1 p10 |  |
| F02-P1174 | Power-Chords&FX | J | J117 | double | 07 | D1 p10 |  |
| F02-P1175 | Funkin' Guitar | K | K053 | double | 07 | D1 p10 |  |
| F02-P1176 | Basses Pizz ST | I | I054 | double | 03 | D1 p10 |  |
| F02-P1177 | StringPhraseMix2 | I | I118 | double | 14 | D1 p10 |  |
| F02-P1178 | SuperSwitch Bass | J | J054 | double | 08 | D1 p10 |  |
| F02-P1179 | E.Bass Finger | J | J118 | single | 08 | D1 p10 |  |
| F02-P1180 | Poinker Bass | K | K054 | double | 08 | D1 p10 |  |
| F02-P1181 | Str Orch.Pizz ST | I | I055 | double | 03 | D1 p10 |  |
| F02-P1182 | StringPhrase Hit | I | I119 | double | 14 | D1 p10 |  |
| F02-P1183 | Jazz Organ 1 | J | J055 | double | 01 | D1 p10 |  |
| F02-P1184 | Pipe Mixture | J | J119 | double | 01 | D1 p10 |  |
| F02-P1185 | Gospel PercOrgan | K | K055 | double | 01 | D1 p10 |  |
| F02-P1186 | String Quartet | I | I056 | single | 03 | D1 p10 |  |
| F02-P1187 | Velocity HarpSFX | I | I120 | double | 14 | D1 p10 |  |
| F02-P1188 | Ravelian Pad | J | J056 | double | 09 | D1 p10 |  |
| F02-P1189 | {(Meditate}) | J | J120 | double | 09 | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1190 | HarpsyKorg 8'+4' | K | K056 | double | 00 | D1 p10 |  |
| F02-P1191 | ViolinSolo D.Vib | I | I057 | double | 03 | D1 p10 |  |
| F02-P1192 | Velo Angel Harps | I | I121 | double | 14 | D1 p10 |  |
| F02-P1193 | Classic F.Horn | J | J057 | double | 05 | D1 p10 |  |
| F02-P1194 | Tight Brass | J | J121 | double | 05 | D1 p10 |  |
| F02-P1195 | Horns & Ensemble | K | K057 | double | 05 | D1 p10 |  |
| F02-P1196 | Viola Solo D.Vib | I | I058 | double | 03 | D1 p10 |  |
| F02-P1197 | ROM-OrchestraKit | I | I122 | double | 15 | D1 p10 |  |
| F02-P1198 | Monster Island | J | J058 | double | 13 | D1 p10 |  |
| F02-P1199 | Soundscapes | J | J122 | double | 13 | D1 p10 |  |
| F02-P1200 | The Great Wall | K | K058 | double | 13 | D1 p10 |  |
| F02-P1201 | Cello Solo D.Vib | I | I059 | double | 03 | D1 p10 |  |
| F02-P1202 | EXB Orch. Kit | I | I123 | double | 15 | D1 p10 |  |
| F02-P1203 | Aaah Voices ST | J | J059 | double | 04 | D1 p10 |  |
| F02-P1204 | Sitar Sitar | J | J123 | double | 07 | D1 p10 |  |
| F02-P1205 | Full Vox Pad | K | K059 | double | 04 | D1 p10 |  |
| F02-P1206 | Alto Flute Attak | I | I060 | double | 06 | D1 p10 |  |
| F02-P1207 | Major Hit Kit | I | I124 | double | 14 | D1 p10 |  |
| F02-P1208 | Trancer Lead | J | J060 | double | 11 | D1 p10 |  |
| F02-P1209 | Espress Lead | J | J124 | double | 11 | D1 p10 |  |
| F02-P1210 | Musette | K | K060 | single | 06 | D1 p10 |  |
| F02-P1211 | Orch.Flute Vib | I | I061 | double | 06 | D1 p10 |  |
| F02-P1212 | Minor Hit Kit | I | I125 | double | 14 | D1 p10 |  |
| F02-P1213 | AltoSax2Breath-Y | J | J061 | double | 06 | D1 p10 |  |
| F02-P1214 | SopranoSax Br.-Y | J | J125 | double | 06 | D1 p10 |  |
| F02-P1215 | AltoSax1 Growl-Y | K | K061 | double | 06 | D1 p10 |  |
| F02-P1216 | Flutes Spiccato | I | I062 | double | 06 | D1 p10 |  |
| F02-P1217 | Major Harp Kit | I | I126 | double | 14 | D1 p10 |  |
| F02-P1218 | Crimson 5ths | J | J062 | double | 12 | D1 p10 |  |
| F02-P1219 | Flying Machines | J | J126 | double | 12 | D1 p10 |  |
| F02-P1220 | E.Bass Pick 1 | K | K062 | single | 08 | D1 p10 |  |
| F02-P1221 | Orchestral Oboe2 | I | I063 | single | 06 | D1 p10 |  |
| F02-P1222 | Minor Harp Kit | I | I127 | double | 14 | D1 p10 |  |
| F02-P1223 | Moving Bellz | J | J063 | double | 02 | D1 p10 |  |
| F02-P1224 | Krystal Bells | J | J127 | double | 02 | D1 p10 |  |
| F02-P1225 | Magical Bells | K | K063 | double | 02 | D1 p10 |  |
| F02-P1226 | SynthBrass | K | K064 | double | 10 | D1 p11 |  |
| F02-P1227 | Big Sweep Stab | L | L000 | double | 10 | D1 p11 |  |
| F02-P1228 | BrightPulseLead | L | L064 | double | 11 | D1 p11 |  |
| F02-P1229 | Techno Phonic | M | M000 | double | 10 | D1 p11 |  |
| F02-P1230 | ResRes One | M | M064 | double | 11 | D1 p11 |  |
| F02-P1231 | Classic Piano | K | K065 | single | 00 | D1 p11 |  |
| F02-P1232 | C. Grand Piano | L | L001 | double | 00 | D1 p11 |  |
| F02-P1233 | M1 Piano | L | L065 | single | 00 | D1 p11 |  |
| F02-P1234 | Euro Grand [VS] | M | M001 | double | 00 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1235 | ClassySwtchPiano | M | M065 | double | 00 | D1 p11 |  |
| F02-P1236 | Big Hit in India | K | K066 | double | 14 | D1 p11 |  |
| F02-P1237 | VCF Sample&Hold | L | L002 | double | 14 | D1 p11 |  |
| F02-P1238 | Noise Shot | L | L066 | double | 14 | D1 p11 |  |
| F02-P1239 | Organ Impact | M | M002 | double | 14 | D1 p11 |  |
| F02-P1240 | Old Record Hit | M | M066 | double | 14 | D1 p11 |  |
| F02-P1241 | Solo Violin | K | K067 | single | 03 | D1 p11 |  |
| F02-P1242 | String World | L | L003 | double | 03 | D1 p11 |  |
| F02-P1243 | Strings Synth | L | L067 | double | 09 | D1 p11 |  |
| F02-P1244 | Stereo Strings | M | M003 | double | 03 | D1 p11 |  |
| F02-P1245 | Sweeper Strings | M | M067 | double | 09 | D1 p11 |  |
| F02-P1246 | Drum'n'Bass Kit | K | K068 | double | 15 | D1 p11 |  |
| F02-P1247 | Trance/GarageKit | L | L004 | double | 15 | D1 p11 |  |
| F02-P1248 | WAcKy HiPHop | L | L068 | double | 15 | D1 p11 |  |
| F02-P1249 | D'n'B Gate Kit | M | M004 | double | 15 | D1 p11 |  |
| F02-P1250 | Wacky FX Kit | M | M068 | double | 15 | D1 p11 |  |
| F02-P1251 | Strato-Chime | K | K069 | double | 07 | D1 p11 |  |
| F02-P1252 | Fingertips12 Gtr | L | L005 | double | 07 | D1 p11 |  |
| F02-P1253 | Perky Bleeper | L | L069 | double | 10 | D1 p11 |  |
| F02-P1254 | Spanish Guitar | M | M005 | double | 07 | D1 p11 |  |
| F02-P1255 | Analog Piano 1 | M | M069 | double | 10 | D1 p11 |  |
| F02-P1256 | Digi Syn Bass | K | K070 | double | 08 | D1 p11 |  |
| F02-P1257 | Bass 4 Da Phunk | L | L006 | double | 08 | D1 p11 |  |
| F02-P1258 | Ana Bass 2 | L | L070 | double | 08 | D1 p11 |  |
| F02-P1259 | House Bass | M | M006 | double | 08 | D1 p11 |  |
| F02-P1260 | Ana Bass 1 | M | M070 | double | 08 | D1 p11 |  |
| F02-P1261 | Classic Click | K | K071 | double | 01 | D1 p11 |  |
| F02-P1262 | Good Old "B" | L | L007 | double | 01 | D1 p11 |  |
| F02-P1263 | Detune Leader | L | L071 | double | 11 | D1 p11 |  |
| F02-P1264 | Rotary Organ | M | M007 | double | 01 | D1 p11 |  |
| F02-P1265 | JuicyBleep-age | M | M071 | single | 11 | D1 p11 |  |
| F02-P1266 | DJ Touch | K | K072 | double | 13 | D1 p11 |  |
| F02-P1267 | Cross Sweeper | L | L008 | double | 09 | D1 p11 |  |
| F02-P1268 | Virtual Traveler | L | L072 | double | 12 | D1 p11 |  |
| F02-P1269 | ClassicAnaPad | M | M008 | double | 09 | D1 p11 |  |
| F02-P1270 | Cosmic Waves | M | M072 | double | 13 | D1 p11 |  |
| F02-P1271 | Big Band Plunger | K | K073 | double | 05 | D1 p11 |  |
| F02-P1272 | Brass Of Power | L | L009 | double | 05 | D1 p11 |  |
| F02-P1273 | Transweep | L | L073 | double | 10 | D1 p11 |  |
| F02-P1274 | Attack Brass ST | M | M009 | double | 05 | D1 p11 |  |
| F02-P1275 | Blasta Brasster | M | M073 | double | 10 | D1 p11 |  |
| F02-P1276 | New Voyage | K | K074 | double | 13 | D1 p11 |  |
| F02-P1277 | Xpanded Voice | L | L010 | double | 13 | D1 p11 |  |
| F02-P1278 | UK Garage Bass | L | L074 | double | 08 | D1 p11 |  |
| F02-P1279 | SingleCoil+Piezo | M | M010 | double | 07 | D1 p11 |  |
| F02-P1280 | Drive Bass | M | M074 | double | 08 | D1 p11 |  |
| F02-P1281 | Arctic Voices | K | K075 | double | 04 | D1 p11 |  |
| F02-P1282 | Nice&Expensive | L | L011 | double | 04 | D1 p11 |  |
| F02-P1283 | Classic E.Piano | L | L075 | double | 00 | D1 p11 |  |
| F02-P1284 | Ooh/Aah VoicesST | M | M011 | double | 04 | D1 p11 |  |
| F02-P1285 | Harmonic Pad | M | M075 | double | 09 | D1 p11 |  |
| F02-P1286 | Goa Message | K | K076 | double | 14 | D1 p11 |  |
| F02-P1287 | Thin AnaLead | L | L012 | double | 11 | D1 p11 |  |
| F02-P1288 | RibbonYoHeadOff! | L | L076 | double | 11 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1289 | AMSFeedbackLead | M | M012 | double | 11 | D1 p11 |  |
| F02-P1290 | Ripper | M | M076 | double | 11 | D1 p11 |  |
| F02-P1291 | English Horn | K | K077 | single | 06 | D1 p11 |  |
| F02-P1292 | Reed Of Power | L | L013 | double | 06 | D1 p11 |  |
| F02-P1293 | 2VCO Planet Lead | L | L077 | double | 11 | D1 p11 |  |
| F02-P1294 | Breathy AltoSax | M | M013 | double | 06 | D1 p11 |  |
| F02-P1295 | Electro Lead | M | M077 | double | 11 | D1 p11 |  |
| F02-P1296 | E.Bass Pick 2 | K | K078 | single | 08 | D1 p11 |  |
| F02-P1297 | Gating Voice Pad | L | L014 | double | 12 | D1 p11 |  |
| F02-P1298 | Time Machines | L | L078 | double | 12 | D1 p11 |  |
| F02-P1299 | Stereo Rez Sweep | M | M014 | double | 12 | D1 p11 |  |
| F02-P1300 | Bazooka Bass | M | M078 | double | 08 | D1 p11 |  |
| F02-P1301 | Tropico Bells | K | K079 | double | 02 | D1 p11 |  |
| F02-P1302 | Log Drum & Bells | L | L015 | double | 02 | D1 p11 |  |
| F02-P1303 | Polyphonic Line | L | L079 | double | 10 | D1 p11 |  |
| F02-P1304 | Warm Steel Drums | M | M015 | double | 02 | D1 p11 |  |
| F02-P1305 | Velo Resonant | M | M079 | double | 10 | D1 p11 |  |
| F02-P1306 | Mega Big Synth | K | K080 | double | 10 | D1 p11 |  |
| F02-P1307 | S&H Reso Comp | L | L016 | double | 10 | D1 p11 |  |
| F02-P1308 | 5th Sweeper | L | L080 | double | 10 | D1 p11 |  |
| F02-P1309 | Paddy Whack | M | M016 | double | 10 | D1 p11 |  |
| F02-P1310 | Square Rez | M | M080 | double | 10 | D1 p11 |  |
| F02-P1311 | Vintage EP | K | K081 | double | 00 | D1 p11 |  |
| F02-P1312 | Stereo E.Piano | L | L017 | double | 00 | D1 p11 |  |
| F02-P1313 | Tremolo Wurly | L | L081 | single | 00 | D1 p11 |  |
| F02-P1314 | Pro-Stage EP | M | M017 | double | 00 | D1 p11 |  |
| F02-P1315 | Classic Wurly | M | M081 | double | 00 | D1 p11 |  |
| F02-P1316 | Space Pod for | K | K082 | double | 12 | D1 p11 |  |
| F02-P1317 | ProgressvTrance1 | L | L018 | double | 14 | D1 p11 |  |
| F02-P1318 | Rave <Ribbon> | L | L082 | double | 14 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1319 | BlastFromDaPast | M | M018 | double | 14 | D1 p11 |  |
| F02-P1320 | Motion Ocean | M | M082 | double | 12 | D1 p11 |  |
| F02-P1321 | Indian Stars | K | K083 | double | 07 | D1 p11 |  |
| F02-P1322 | Ensemble & Solo | L | L019 | double | 03 | D1 p11 |  |
| F02-P1323 | Garage SQ Bass | L | L083 | double | 08 | D1 p11 |  |
| F02-P1324 | Analog Velvet | M | M019 | double | 03 | D1 p11 |  |
| F02-P1325 | Acid Square Bass | M | M083 | double | 08 | D1 p11 |  |
| F02-P1326 | Dragon Gong | K | K084 | double | 15 | D1 p11 |  |
| F02-P1327 | Big Trouble Kit | L | L020 | double | 15 | D1 p11 |  |
| F02-P1328 | Filter Drums | L | L084 | double | 15 | D1 p11 |  |
| F02-P1329 | Electro Kit 1 | M | M020 | double | 15 | D1 p11 |  |
| F02-P1330 | Big Break Down | M | M084 | double | 13 | D1 p11 |  |
| F02-P1331 | Feedback D. Gtr. | K | K085 | double | 07 | D1 p11 |  |
| F02-P1332 | Nu Strat | L | L021 | double | 07 | D1 p11 |  |
| F02-P1333 | Nu Skool Bleeps | L | L085 | single | 11 | D1 p11 |  |
| F02-P1334 | Rhythmic E.Gtr | M | M021 | single | 07 | D1 p11 |  |
| F02-P1335 | WellAardHouse | M | M085 | single | 11 | D1 p11 |  |
| F02-P1336 | Sweet Fretless | K | K086 | double | 08 | D1 p11 |  |
| F02-P1337 | Club Ac Bass | L | L022 | double | 08 | D1 p11 |  |
| F02-P1338 | Acid Dist Bass | L | L086 | double | 08 | D1 p11 |  |
| F02-P1339 | Low Finger Bass | M | M022 | double | 08 | D1 p11 |  |
| F02-P1340 | The Growler | M | M086 | double | 08 | D1 p11 |  |
| F02-P1341 | Percussion BX3 | K | K087 | double | 01 | D1 p11 |  |
| F02-P1342 | Additive Organ | L | L023 | double | 01 | D1 p11 |  |
| F02-P1343 | Bounce Shot | L | L087 | double | 12 | D1 p11 |  |
| F02-P1344 | M1 Organ Bass | M | M023 | double | 08 | D1 p11 |  |
| F02-P1345 | Overture Pad | M | M087 | double | 09 | D1 p11 |  |
| F02-P1346 | Dark Night | K | K088 | double | 13 | D1 p11 |  |
| F02-P1347 | Smoothy Pad | L | L024 | double | 09 | D1 p11 |  |
| F02-P1348 | Freedom Pad | L | L088 | double | 09 | D1 p11 |  |
| F02-P1349 | Scattersynth | M | M024 | double | 12 | D1 p11 |  |
| F02-P1350 | Flauto Pipes | M | M088 | double | 01 | D1 p11 |  |
| F02-P1351 | Trombone Ens. | K | K089 | double | 05 | D1 p11 |  |
| F02-P1352 | Trombone Soft | L | L025 | single | 05 | D1 p11 |  |
| F02-P1353 | Thunderlog | L | L089 | double | 10 | D1 p11 |  |
| F02-P1354 | French Horn Sect | M | M025 | double | 05 | D1 p11 |  |
| F02-P1355 | Power Saw | M | M089 | double | 10 | D1 p11 |  |
| F02-P1356 | Marc Tree | K | K090 | double | 15 | D1 p11 |  |
| F02-P1357 | Video Blaster | L | L026 | double | 13 | D1 p11 |  |
| F02-P1358 | Vanishing Planet | L | L090 | double | 13 | D1 p11 |  |
| F02-P1359 | JoyStick Machine | M | M026 | double | 13 | D1 p11 |  |
| F02-P1360 | Unholy Water | M | M090 | double | 13 | D1 p11 |  |
| F02-P1361 | Take Voices | K | K091 | double | 04 | D1 p11 |  |
| F02-P1362 | Dream Voices | L | L027 | double | 04 | D1 p11 |  |
| F02-P1363 | Piano Pad 2 | L | L091 | double | 00 | D1 p11 |  |
| F02-P1364 | Vocalscaping | M | M027 | double | 04 | D1 p11 |  |
| F02-P1365 | Tomorrow's Pad | M | M091 | double | 12 | D1 p11 |  |
| F02-P1366 | Fisa Cassotto | K | K092 | double | 06 | D1 p11 |  |
| F02-P1367 | Turn Back Stab | L | L028 | double | 10 | D1 p11 |  |
| F02-P1368 | Nu HPF Lead | L | L092 | double | 11 | D1 p11 |  |
| F02-P1369 | Octo Lead | M | M028 | double | 11 | D1 p11 |  |
| F02-P1370 | Old VOX Legend | M | M092 | double | 01 | D1 p11 |  |
| F02-P1371 | Bassoon | K | K093 | single | 06 | D1 p11 |  |
| F02-P1372 | Bari Blaster | L | L029 | double | 06 | D1 p11 |  |
| F02-P1373 | D'n'B/GarageBass | L | L093 | single | 08 | D1 p11 |  |
| F02-P1374 | Soft TenorSax | M | M029 | double | 06 | D1 p11 |  |
| F02-P1375 | Drum'n'Bass Bass | M | M093 | single | 08 | D1 p11 |  |
| F02-P1376 | PedalSteelGuitar | K | K094 | single | 07 | D1 p11 |  |
| F02-P1377 | Pods In The Pad | L | L030 | double | 12 | D1 p11 |  |
| F02-P1378 | VCF Modulation | L | L094 | single | 12 | D1 p11 |  |
| F02-P1379 | Water is Deep | M | M030 | double | 12 | D1 p11 |  |
| F02-P1380 | Flashlight Pad | M | M094 | double | 12 | D1 p11 |  |
| F02-P1381 | Koto | K | K095 | single | 07 | D1 p11 |  |
| F02-P1382 | Bottle-Bell | L | L031 | double | 02 | D1 p11 |  |
| F02-P1383 | Digital Bells 2 | L | L095 | single | 02 | D1 p11 |  |
| F02-P1384 | GlassBell Bright | M | M031 | single | 02 | D1 p11 |  |
| F02-P1385 | The Mandolin | M | M095 | double | 07 | D1 p11 |  |
| F02-P1386 | Rezzo Release | K | K096 | double | 12 | D1 p11 |  |
| F02-P1387 | Garage PWM | L | L032 | double | 10 | D1 p11 |  |
| F02-P1388 | PowerRezSweep | L | L096 | double | 10 | D1 p11 |  |
| F02-P1389 | Comp Trance | M | M032 | double | 10 | D1 p11 |  |
| F02-P1390 | BPF Sweep Pad | M | M096 | double | 12 | D1 p11 |  |
| F02-P1391 | Sticky Rez Clav | K | K097 | double | 00 | D1 p11 |  |
| F02-P1392 | Star Piano | L | L033 | double | 00 | D1 p11 |  |
| F02-P1393 | Funky Clav | L | L097 | double | 00 | D1 p11 |  |
| F02-P1394 | <Glassy Stack> | M | M033 | double | 00 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1395 | Velo Pulse Clav | M | M097 | double | 10 | D1 p11 |  |
| F02-P1396 | Hit Me 2 Times!! | K | K098 | double | 14 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1397 | Techno Stat | L | L034 | double | 14 | D1 p11 |  |
| F02-P1398 | Random Pulsator | L | L098 | double | 12 | D1 p11 |  |
| F02-P1399 | Chord Trigger | M | M034 | double | 14 | D1 p11 |  |
| F02-P1400 | NetherlandHit | M | M098 | double | 14 | D1 p11 |  |
| F02-P1401 | Santur | K | K099 | double | 07 | D1 p11 |  |
| F02-P1402 | ComponentStrings | L | L035 | double | 03 | D1 p11 |  |
| F02-P1403 | Boost RnB Bass | L | L099 | double | 08 | D1 p11 |  |
| F02-P1404 | Symphonic Ensemb | M | M035 | double | 09 | D1 p11 |  |
| F02-P1405 | The Tea-Bea | M | M099 | double | 08 | D1 p11 |  |
| F02-P1406 | BD&SD Kit 2 | K | K100 | double | 15 | D1 p11 |  |
| F02-P1407 | Nasty TRicKy | L | L036 | double | 15 | D1 p11 |  |
| F02-P1408 | 5th Stab | L | L100 | double | 10 | D1 p11 |  |
| F02-P1409 | UGLY HoUSe | M | M036 | double | 15 | D1 p11 |  |
| F02-P1410 | Ribbon Morpher | M | M100 | double | 10 | D1 p11 |  |
| F02-P1411 | Old 12-String | K | K101 | double | 07 | D1 p11 |  |
| F02-P1412 | Joystick Gtr(-Y) | L | L037 | double | 07 | D1 p11 |  |
| F02-P1413 | Rhythm Of Life | L | L101 | double | 12 | D1 p11 |  |
| F02-P1414 | Busy Sync | M | M037 | double | 11 | D1 p11 |  |
| F02-P1415 | Moon Talker | M | M101 | double | 12 | D1 p11 |  |
| F02-P1416 | Ramp Jungle Bass | K | K102 | double | 08 | D1 p11 |  |
| F02-P1417 | DistortedTeaBea | L | L038 | single | 08 | D1 p11 |  |
| F02-P1418 | Cyber Tom | L | L102 | single | 08 | D1 p11 |  |
| F02-P1419 | 2 Step Bass | M | M038 | single | 08 | D1 p11 |  |
| F02-P1420 | Blaster Bass 1 | M | M102 | single | 08 | D1 p11 |  |
| F02-P1421 | Killer B | K | K103 | double | 01 | D1 p11 |  |
| F02-P1422 | Garage Organ | L | L039 | double | 01 | D1 p11 |  |
| F02-P1423 | Jazz Organ 2 | L | L103 | double | 01 | D1 p11 |  |
| F02-P1424 | Superdark Organ | M | M039 | double | 01 | D1 p11 |  |
| F02-P1425 | Theatrical Pad | M | M103 | double | 09 | D1 p11 |  |
| F02-P1426 | L/R Piano(Knob1) | K | K104 | double | 00 | D1 p11 |  |
| F02-P1427 | Portamento Pad | L | L040 | double | 09 | D1 p11 |  |
| F02-P1428 | Syn Ghostly | L | L104 | double | 12 | D1 p11 |  |
| F02-P1429 | Metal SlowPad | M | M040 | single | 09 | D1 p11 |  |
| F02-P1430 | Warm E.Grand | M | M104 | double | 00 | D1 p11 |  |
| F02-P1431 | Trumpet Ens. | K | K105 | double | 05 | D1 p11 |  |
| F02-P1432 | BrassSlow Stereo | L | L041 | double | 05 | D1 p11 |  |
| F02-P1433 | Korgmatose | L | L105 | double | 09 | D1 p11 |  |
| F02-P1434 | DynaBrassStereo | M | M041 | double | 05 | D1 p11 |  |
| F02-P1435 | Super Sweeper | M | M105 | double | 09 | D1 p11 |  |
| F02-P1436 | MotionSoundTrack | K | K106 | double | 13 | D1 p11 |  |
| F02-P1437 | Distant Lights | L | L042 | double | 13 | D1 p11 |  |
| F02-P1438 | Frozen Glaciers | L | L106 | double | 13 | D1 p11 |  |
| F02-P1439 | Compu-shift | M | M042 | double | 13 | D1 p11 |  |
| F02-P1440 | Cosmic Toys | M | M106 | double | 13 | D1 p11 |  |
| F02-P1441 | Processed E.Gtr. | K | K107 | double | 07 | D1 p11 |  |
| F02-P1442 | The AirVox | L | L043 | double | 04 | D1 p11 |  |
| F02-P1443 | The ANAPIZZ | L | L107 | double | 10 | D1 p11 |  |
| F02-P1444 | Da Mute...Joy | M | M043 | double | 07 | D1 p11 |  |
| F02-P1445 | Syn Pizz | M | M107 | double | 10 | D1 p11 |  |
| F02-P1446 | Fat Syn Sync | K | K108 | double | 11 | D1 p11 |  |
| F02-P1447 | R&B Lead | L | L044 | double | 11 | D1 p11 |  |
| F02-P1448 | Sync Kronicity | L | L108 | double | 11 | D1 p11 |  |
| F02-P1449 | Motion Raver | M | M044 | double | 14 | D1 p11 |  |
| F02-P1450 | Brian's Sync | M | M108 | double | 11 | D1 p11 |  |
| F02-P1451 | Spitz Bottle | K | K109 | double | 06 | D1 p11 |  |
| F02-P1452 | InverseSQwave | L | L045 | double | 10 | D1 p11 |  |
| F02-P1453 | Syn Pipes | L | L109 | double | 11 | D1 p11 |  |
| F02-P1454 | Square Snaps | M | M045 | double | 10 | D1 p11 |  |
| F02-P1455 | Deep Detune Bass | M | M109 | double | 08 | D1 p11 |  |
| F02-P1456 | MixFisaMaster | K | K110 | double | 06 | D1 p11 |  |
| F02-P1457 | Glass Vox | L | L046 | double | 12 | D1 p11 |  |
| F02-P1458 | Stereo WaveSweep | L | L110 | double | 12 | D1 p11 |  |
| F02-P1459 | Spectrum Alloy | M | M046 | double | 12 | D1 p11 |  |
| F02-P1460 | Rich Lead | M | M110 | double | 11 | D1 p11 |  |
| F02-P1461 | Future Bell | K | K111 | double | 02 | D1 p11 |  |
| F02-P1462 | Mallet Clocker | L | L047 | double | 02 | D1 p11 |  |
| F02-P1463 | Reactor Pad | L | L111 | double | 09 | D1 p11 |  |
| F02-P1464 | Breathy Bells | M | M047 | double | 02 | D1 p11 |  |
| F02-P1465 | DanceFloor Grand | M | M111 | double | 00 | D1 p11 |  |
| F02-P1466 | Flute Pad | K | K112 | double | 10 | D1 p11 |  |
| F02-P1467 | Dual Filterz | L | L048 | double | 10 | D1 p11 |  |
| F02-P1468 | MG Pulse Bass | L | L112 | double | 08 | D1 p11 |  |
| F02-P1469 | Band Passed | M | M048 | double | 10 | D1 p11 |  |
| F02-P1470 | Feedback MG | M | M112 | double | 08 | D1 p11 |  |
| F02-P1471 | Hybrid Digi EP | K | K113 | double | 00 | D1 p11 |  |
| F02-P1472 | Reson Piano | L | L049 | double | 00 | D1 p11 |  |
| F02-P1473 | Fire Wave | L | L113 | double | 10 | D1 p11 |  |
| F02-P1474 | Classic Tines | M | M049 | double | 00 | D1 p11 |  |
| F02-P1475 | Digitized Raver | M | M113 | double | 11 | D1 p11 |  |
| F02-P1476 | Harp | K | K114 | double | 07 | D1 p11 |  |
| F02-P1477 | Cyber Choir | L | L050 | double | 10 | D1 p11 |  |
| F02-P1478 | One Note Stories | L | L114 | double | 13 | D1 p11 |  |
| F02-P1479 | Djembe/Log Drum | M | M050 | double | 15 | D1 p11 |  |
| F02-P1480 | Filter Gate | M | M114 | double | 12 | D1 p11 |  |
| F02-P1481 | Harmonica | K | K115 | single | 06 | D1 p11 |  |
| F02-P1482 | PizzicatoSection | L | L051 | double | 03 | D1 p11 |  |
| F02-P1483 | Mellow Movie | L | L115 | double | 09 | D1 p11 |  |
| F02-P1484 | Breakdown String | M | M051 | double | 09 | D1 p11 |  |
| F02-P1485 | Detox Lead | M | M115 | double | 11 | D1 p11 |  |
| F02-P1486 | Percussion Kit | K | K116 | double | 15 | D1 p11 |  |
| F02-P1487 | Psycho Pitch Kit | L | L052 | double | 15 | D1 p11 |  |
| F02-P1488 | Symphonic Waves | L | L116 | double | 12 | D1 p11 |  |
| F02-P1489 | KING<>KONG | M | M052 | double | 15 | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1490 | Cyclic X-Fades | M | M116 | double | 12 | D1 p11 |  |
| F02-P1491 | Dynamic E.Guitar | K | K117 | double | 07 | D1 p11 |  |
| F02-P1492 | CleanMute-Guitar | L | L053 | double | 07 | D1 p11 |  |
| F02-P1493 | Reso Comp 1 | L | L117 | single | 10 | D1 p11 |  |
| F02-P1494 | Super Clean Gtr | M | M053 | double | 07 | D1 p11 |  |
| F02-P1495 | Cat Lead (-Y) | M | M117 | double | 11 | D1 p11 |  |
| F02-P1496 | Decayed Bass | K | K118 | double | 08 | D1 p11 |  |
| F02-P1497 | Wah Space Bass | L | L054 | double | 08 | D1 p11 |  |
| F02-P1498 | Nu Skool Bass | L | L118 | double | 08 | D1 p11 |  |
| F02-P1499 | Phat Bass | M | M054 | double | 08 | D1 p11 |  |
| F02-P1500 | Motion Bass/Lead | M | M118 | double | 08 | D1 p11 |  |
| F02-P1501 | Church Pipes | K | K119 | double | 01 | D1 p11 |  |
| F02-P1502 | Dirty Jazz Organ | L | L055 | double | 01 | D1 p11 |  |
| F02-P1503 | Pipe Tutti | L | L119 | double | 01 | D1 p11 |  |
| F02-P1504 | Dirty "B" | M | M055 | double | 01 | D1 p11 |  |
| F02-P1505 | Organ Positive | M | M119 | double | 01 | D1 p11 |  |
| F02-P1506 | Digital Bells 1 | K | K120 | double | 02 | D1 p11 |  |
| F02-P1507 | ASineOfTheTimes | L | L056 | double | 09 | D1 p11 |  |
| F02-P1508 | OutOfTheClouds | L | L120 | double | 12 | D1 p11 |  |
| F02-P1509 | Clav Snap | M | M056 | double | 00 | D1 p11 |  |
| F02-P1510 | JS/Ribbon Pad | M | M120 | double | 12 | D1 p11 |  |
| F02-P1511 | Classic Fanfare | K | K121 | double | 05 | D1 p11 |  |
| F02-P1512 | Soft Synth Brass | L | L057 | double | 10 | D1 p11 |  |
| F02-P1513 | Dance Lead | L | L121 | single | 11 | D1 p11 |  |
| F02-P1514 | Analog Pad | M | M057 | double | 09 | D1 p11 |  |
| F02-P1515 | X-Mod Raver | M | M121 | double | 11 | D1 p11 |  |
| F02-P1516 | Hemispheres | K | K122 | double | 13 | D1 p11 |  |
| F02-P1517 | Bad Weather | L | L058 | double | 13 | D1 p11 |  |
| F02-P1518 | Synchro Science | L | L122 | double | 14 | D1 p11 |  |
| F02-P1519 | Cript-on | M | M058 | double | 13 | D1 p11 |  |
| F02-P1520 | Rave Stabz | M | M122 | double | 14 | D1 p11 |  |
| F02-P1521 | Indian Frets Gtr | K | K123 | double | 07 | D1 p11 |  |
| F02-P1522 | Cinema Pad | L | L059 | double | 09 | D1 p11 |  |
| F02-P1523 | Ultra Res. Sweep | L | L123 | double | 09 | D1 p11 |  |
| F02-P1524 | Goliath's Waves | M | M059 | double | 09 | D1 p11 |  |
| F02-P1525 | Noble Brite Pad | M | M123 | double | 09 | D1 p11 |  |
| F02-P1526 | Film Brass | K | K124 | double | 05 | D1 p11 |  |
| F02-P1527 | Square Wave | L | L060 | double | 11 | D1 p11 |  |
| F02-P1528 | Hard Velo Bass | L | L124 | double | 08 | D1 p11 |  |
| F02-P1529 | A leadload | M | M060 | double | 11 | D1 p11 |  |
| F02-P1530 | SynBassRes | M | M124 | double | 08 | D1 p11 |  |
| F02-P1531 | Jazz Clarinet 1 | K | K125 | single | 06 | D1 p11 |  |
| F02-P1532 | Digital Voice | L | L061 | double | 04 | D1 p11 |  |
| F02-P1533 | Rhythmnosis | L | L125 | double | 12 | D1 p11 |  |
| F02-P1534 | Talkin' Keys | M | M061 | double | 04 | D1 p11 |  |
| F02-P1535 | Solar Surfing | M | M125 | double | 12 | D1 p11 |  |
| F02-P1536 | Pro-Dyno EP | K | K126 | double | 00 | D1 p11 |  |
| F02-P1537 | Moon Cycles | L | L062 | double | 12 | D1 p11 |  |
| F02-P1538 | Wispy Dry Wind | L | L126 | double | 13 | D1 p11 |  |
| F02-P1539 | Slap Bass v/s | M | M062 | single | 08 | D1 p11 |  |
| F02-P1540 | EuphoricTrancer | M | M126 | double | 10 | D1 p11 |  |
| F02-P1541 | Tinkle Bells | K | K127 | double | 02 | D1 p11 |  |
| F02-P1542 | Hybrid Bell | L | L063 | double | 02 | D1 p11 |  |
| F02-P1543 | Station Of Waves | L | L127 | double | 12 | D1 p11 |  |
| F02-P1544 | That Heaven Vibe | M | M063 | double | 09 | D1 p11 |  |
| F02-P1545 | BPM Filter Sweep | M | M127 | double | 12 | D1 p11 |  |
| F02-P1546 | Strato Grand | N | N000 | double | 00 | D1 p12 |  |
| F02-P1547 | Velocity PWM | N | N001 | double | 00 | D1 p12 |  |
| F02-P1548 | The 10% Solution | N | N002 | double | 10 | D1 p12 |  |
| F02-P1549 | Analog Bell | N | N003 | double | 10 | D1 p12 |  |
| F02-P1550 | 2VCO Decay Pad | N | N004 | double | 10 | D1 p12 |  |
| F02-P1551 | Hommie's Piana | N | N005 | double | 10 | D1 p12 |  |
| F02-P1552 | ProSynth Organ | N | N006 | single | 01 | D1 p12 |  |
| F02-P1553 | Green Organ | N | N007 | single | 01 | D1 p12 |  |
| F02-P1554 | Canterbury Organ | N | N008 | double | 01 | D1 p12 |  |
| F02-P1555 | Mondo Organ | N | N009 | double | 01 | D1 p12 |  |
| F02-P1556 | BladeBoard | N | N010 | double | 10 | D1 p12 |  |
| F02-P1557 | Widow Maker | N | N011 | double | 10 | D1 p12 |  |
| F02-P1558 | Boww Boww Synth | N | N012 | single | 10 | D1 p12 |  |
| F02-P1559 | Filter Bass | N | N013 | double | 08 | D1 p12 |  |
| F02-P1560 | [ VeloBASS ] | N | N014 | double | 08 | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1561 | Angry Tea Bee | N | N015 | single | 08 | D1 p12 |  |
| F02-P1562 | Breakbeat Bass | N | N016 | single | 08 | D1 p12 |  |
| F02-P1563 | Viral Lead | N | N017 | single | 11 | D1 p12 |  |
| F02-P1564 | Psycho Stab | N | N018 | double | 11 | D1 p12 |  |
| F02-P1565 | LFO Trance Lead | N | N019 | double | 11 | D1 p12 |  |
| F02-P1566 | Toxic Lead | N | N020 | double | 11 | D1 p12 |  |
| F02-P1567 | Lil'Space Men | N | N021 | double | 12 | D1 p12 |  |
| F02-P1568 | DeepPadw/Sprnkls | N | N022 | double | 12 | D1 p12 |  |
| F02-P1569 | Radio Angels | N | N023 | double | 12 | D1 p12 |  |
| F02-P1570 | Robotica | N | N024 | double | 12 | D1 p12 |  |
| F02-P1571 | All on Knob | N | N025 | double | 12 | D1 p12 |  |
| F02-P1572 | Euphorisim | N | N026 | double | 12 | D1 p12 |  |
| F02-P1573 | Red Rain | N | N027 | double | 12 | D1 p12 |  |
| F02-P1574 | Trance in Motion | N | N028 | double | 12 | D1 p12 |  |
| F02-P1575 | Europhonic | N | N029 | double | 12 | D1 p12 |  |
| F02-P1576 | Storm Chaser | N | N030 | double | 12 | D1 p12 |  |
| F02-P1577 | Sonic Throbber | N | N031 | double | 12 | D1 p12 |  |
| F02-P1578 | MonolithicMotion | N | N032 | double | 12 | D1 p12 |  |
| F02-P1579 | Wave Hunter | N | N033 | double | 12 | D1 p12 |  |
| F02-P1580 | WelcomeToMyWorld | N | N034 | double | 12 | D1 p12 |  |
| F02-P1581 | Man Of War | N | N035 | double | 12 | D1 p12 |  |
| F02-P1582 | Club L.F.O. | N | N036 | double | 12 | D1 p12 |  |
| F02-P1583 | Multi-Modular | N | N037 | double | 12 | D1 p12 |  |
| F02-P1584 | Trance Bells | N | N038 | double | 02 | D1 p12 |  |
| F02-P1585 | VCF S&H Arpg | N | N039 | double | 14 | D1 p12 |  |
| F02-P1586 | Synchro Pulse | N | N040 | double | 14 | D1 p12 |  |
| F02-P1587 | Clocks Running | N | N041 | double | 14 | D1 p12 |  |
| F02-P1588 | Irish Fairytale | N | N042 | single | 14 | D1 p12 |  |
| F02-P1589 | Elec Laboratory | N | N043 | double | 14 | D1 p12 |  |
| F02-P1590 | Dub Panner | N | N044 | double | 14 | D1 p12 |  |
| F02-P1591 | ARP Yer TRANCE | N | N045 | double | 14 | D1 p12 |  |
| F02-P1592 | Synpathic | N | N046 | double | 14 | D1 p12 |  |
| F02-P1593 | Trance Bleeper | N | N047 | single | 14 | D1 p12 |  |
| F02-P1594 | CyberWaveStation | N | N048 | double | 14 | D1 p12 |  |
| F02-P1595 | ModulatedMadness | N | N049 | double | 14 | D1 p12 |  |
| F02-P1596 | CombRibbBeats:-) | N | N050 | double | 15 | D1 p12 |  |
| F02-P1597 | TranceFilter Kit | N | N051 | double | 15 | D1 p12 |  |
| F02-P1598 | NuTrix Pitch Kit | N | N052 | double | 15 | D1 p12 |  |
| F02-P1599 | TEK NO Kit 2 | N | N053 | double | 15 | D1 p12 |  |
| F02-P1600 | Digger | N | N054 | double | 13 | D1 p12 |  |
| F02-P1601 | Noir Blood | N | N055 | double | 13 | D1 p12 |  |
| F02-P1602 | An Avalanche | N | N056 | double | 13 | D1 p12 |  |
| F02-P1603 | Droner>SlowClimb | N | N057 | single | 13 | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F02-P1604 | Ultra-Low Osc's | N | N058 | single | 13 | D1 p12 |  |
| F02-P1605 | SweepingAllAside | N | N059 | double | 13 | D1 p12 |  |
| F02-P1606 | Male Voice SW1 | N | N060 | double | 13 | D1 p12 |  |
| F02-P1607 | FemaleVoice1 SW1 | N | N061 | double | 13 | D1 p12 |  |
| F02-P1608 | FemaleVoice2 SW1 | N | N062 | double | 13 | D1 p12 |  |
| F02-P1609 | FemaleVoice3 SW1 | N | N063 | double | 13 | D1 p12 |  |

## Factory FX algorithms and routing

**Found in documentation: yes** - the algorithm catalogue and the slot routing are both published in D2. **Per-program FX assignments are not documented**: no public document states which insert algorithms, chain order, send levels or master effect a given factory program uses. That information exists only inside the instrument's own factory data, which is out of bounds for this project, so per-patch chains have to be reconstructed by ear against the catalogue below.

**Slots (D2).** Five insert effects IFX1-IFX5 (chainable), two master effects MFX1-MFX2 (send/return, mono-in stereo-out), and a Master EQ after them. D2's routing diagrams place the chain as: oscillator / filter / amplifier -> IFX1-5 -> MFX send and return -> Master EQ -> output.

**Selectable ranges (D2).**

| Slot | Documented range | Document's wording |
|---|---|---|
| see D2 | 000 - 089 | "Reverb-Gate" |
| see D2 | 000 - 102 | "Hold Delay" |
| see D2 | 000 - 089 | "Reverb-
Gate" |

Slot rules read from D2: ifx1_5_max = 089, ifx2_4_max = 102, mfx_max = 089, stated_type_count_2 = 90.

**Algorithm catalogue (D2), 103 entries, read from the Parameter Guide's own effect index.**

| # | Algorithm as printed |
|---|---|
| 000 | No Effect |
| 001 | St. Amp Simulation (Stereo Amp Simulation) |
| 002 | Stereo Compressor |
| 003 | Stereo Limiter |
| 004 | Multiband Limiter |
| 005 | Stereo Gate |
| 006 | OD/Hi.Gain Wah (Overdrive/Hi.Gain Wah) |
| 007 | St. Parametric 4EQ (Stereo Parametric 4-Band EQ) |
| 008 | St. Graphic 7EQ (Stereo Graphic 7 Band EQ) |
| 009 | St. Wah/Auto Wah (Stereo Wah/Auto Wah) |
| 010 | St. Random Filter (Stereo Random Filter) |
| 011 | St. Exciter/Enhncr (Stereo Exciter/Enhancer) |
| 012 | St. Sub Oscillator (Stereo Sub Oscillator) |
| 013 | Talking Modulator |
| 014 | Stereo Decimator |
| 015 | St. Analog Record (Stereo Analog Record) |
| 016 | Stereo Chorus |
| 017 | St. Harmonic Chorus (Stereo Harmonic Chorus) |
| 018 | Multitap Cho/Delay (Multitap Chorus/Delay) |
| 019 | Ensemble |
| 020 | Stereo Flanger |
| 021 | St. Random Flanger (Stereo Random Flanger) |
| 022 | St. Env. Flanger (Stereo Envelope Flanger) |
| 023 | Stereo Phaser |
| 024 | St. Random Phaser (Stereo Random Phaser) |
| 025 | St. Env. Phaser (Stereo Envelope Phaser) |
| 026 | St. Biphase Mod. (Stereo Biphase Modulation) |
| 027 | Stereo Vibrato |
| 028 | St. Auto Fade Mod. (Stereo Auto Fade Modulation) |
| 029 | 2Voice Resonator |
| 030 | Doppler |
| 031 | Scratch |
| 032 | Stereo Tremolo |
| 033 | St. Env. Tremolo (Stereo Envelope Tremolo) |
| 034 | Stereo Auto Pan |
| 035 | St. Phaser + Trml (Stereo Phaser + Tremolo) |
| 036 | St. Ring Modulator (Stereo Ring Modulator) |
| 037 | Detune |
| 038 | Pitch Shifter |
| 039 | Pitch Shift Mod. (Pitch Shift Modulation) |
| 040 | Rotary Speaker |
| 041 | Early Reﬂections |
| 042 | Auto Reverse |
| 043 | L/C/R Delay |
| 044 | Stereo/Cross Delay |
| 045 | St. Multitap Delay (Stereo Multitap Delay) |
| 046 | St. Modulation Delay (Stereo Modulation Delay) |
| 047 | St. Dynamic Delay (Stereo Dynamic Delay) |
| 048 | St. Auto Panning Dly(Stereo Auto Panning Delay) |
| 049 | L/C/R BPM Delay |
| 050 | St. BPM Delay (Stereo BPM Delay) |
| 051 | Sequence Delay |
| 052 | Reverb Hall |
| 053 | Reverb SmoothHall |
| 054 | Reverb Wet Plate |
| 055 | Reverb Dry Plate |
| 056 | Reverb Room |
| 057 | Reverb BrightRoom |
| 058 | P4EQ – Exciter (Parametric 4-Band EQ – Exciter) |
| 059 | P4EQ – Wah (Parametric 4-Band EQ – Wah/Auto Wah) |
| 060 | P4EQ – Cho/Flng (Parametric 4-Band EQ – Chorus/Flanger).. 248 |
| 061 | P4EQ – Phaser (Parametric 4-Band EQ – Phaser) |
| 062 | P4EQ – Mt. Delay (Parametric 4-Band EQ – Multitap Delay) |
| 063 | Comp – Wah (Compressor – Wah/Auto Wah) |
| 064 | Comp – Amp Sim (Compressor – Amp Simulation) |
| 065 | Comp – OD/HiGain (Compressor – Overdrive/Hi.Gain) |
| 066 | Comp – Param4EQ (Compressor – Parametric 4-Band EQ) |
| 067 | Comp – Cho/Flng (Compressor – Chorus/Flanger) |
| 068 | Comp – Phaser (Compressor – Phaser) |
| 069 | Comp – Mt. Delay (Compressor – Multitap Delay) |
| 070 | Limiter – P4EQ (Limiter – Parametric 4-Band EQ) |
| 071 | Limiter – Cho/Flng (Limiter – Chorus/Flanger) |
| 072 | Limiter – Phaser |
| 073 | Limiter – Mt. Delay (Limiter – Multitap Delay) |
| 074 | Exciter – Comp (Exciter – Compressor) |
| 075 | Exciter – Limiter |
| 076 | Exciter – Cho/Flng (Exciter – Chorus/Flanger) |
| 077 | Exciter – Phaser |
| 078 | Exciter – Mt. Delay (Exciter – Multitap Delay) |
| 079 | OD/HG – Amp Sim (Overdrive/Hi.Gain – Amp Simulation) |
| 080 | OD/HG – Cho/Flng (Overdrive/Hi.Gain – Chorus/Flanger) |
| 081 | OD/HG – Phaser (Overdrive/Hi.Gain – Phaser) |
| 082 | OD/HG – Mt. Delay (Overdrive/Hi.Gain – Multitap Delay) |
| 083 | Wah – Amp Sim (Wah/Auto Wah – Amp Simulation) |
| 084 | Decimator – Amp (Decimator – Amp Simulation) |
| 085 | Decimator – Comp (Decimator – Compressor) |
| 086 | Amp Sim – Tremolo (Amp Simulation – Tremolo) |
| 087 | Cho/Flng – Mt. Dly (Chorus/Flanger – Multitap Delay) |
| 088 | Phaser – Cho/Flng (Phaser – Chorus/Flanger) |
| 089 | Reverb – Gate |
| 090 | Piano Body/Damper (Piano Body/Damper Simulation) |
| 091 | St. Mltband Limiter (Stereo Multiband Limiter) |
| 092 | OD/HyperGain Wah (Overdrive/Hyper Gain Wah) |
| 093 | Vocoder |
| 094 | Multitap Cho/Delay (Multitap Chorus/Delay) |
| 095 | St. Pitch Shifter (Stereo Pitch Shifter) |
| 096 | Rotary Speaker OD (Rotary Speaker Overdrive) |
| 097 | Early Reﬂections |
| 098 | L/C/R Long Delay |
| 099 | St/Cross Long Dly (Stereo/Cross Long Delay) |
| 100 | LCR BPM Long Dly (L/C/R BPM Long Delay) |
| 101 | St. BPM Long Delay (Stereo BPM Long Delay) |
| 102 | Hold Delay |

## PCM wave list (engine input, not content to ingest)

D1 publishes the ROM multisample and drumsample name lists. They are engine input: they say what the reference machine's waves are called and how they are numbered. They are not content we may copy, and the numbers are the document's own numbering.

### Multisamples

| Shipped label | Number | Documented name | Group | Source | Note |
|---|---|---|---|---|---|
| F02-W0001 | 0 | A.Piano | ROM | D1 p12 | |
| F02-W0002 | 0 | A.Piano-BD ff L | Piano | D1 p13 | |
| F02-W0003 | 1 | A.Piano-M1 | ROM | D1 p12 | |
| F02-W0004 | 1 | A.Piano-BD ff R | Piano | D1 p13 | |
| F02-W0005 | 2 | E.Grand Piano | ROM | D1 p12 | |
| F02-W0006 | 2 | A.Piano-BD mf L | Piano | D1 p13 | |
| F02-W0007 | 3 | E.P.-FM 1 | ROM | D1 p12 | |
| F02-W0008 | 3 | A.Piano-BD mf R | Piano | D1 p13 | |
| F02-W0009 | 4 | E.P.-FM 1 LP | ROM | D1 p12 | |
| F02-W0010 | 5 | E.P.-FM 2 | ROM | D1 p12 | |
| F02-W0011 | 6 | E.P.-FM 3 | ROM | D1 p12 | |
| F02-W0012 | 7 | E.P.-FM 3 LP | ROM | D1 p12 | |
| F02-W0013 | 8 | E.P.-Dyno Soft | ROM | D1 p12 | |
| F02-W0014 | 9 | E.P.-Dyno Sft LP | ROM | D1 p12 | |
| F02-W0015 | 10 | E.P.-Dyno Medium | ROM | D1 p12 | |
| F02-W0016 | 11 | E.P.-Dyno Med LP | ROM | D1 p12 | |
| F02-W0017 | 12 | E.P.-Stage Soft | ROM | D1 p12 | |
| F02-W0018 | 13 | E.P.-Sta Soft LP | ROM | D1 p12 | |
| F02-W0019 | 14 | E.P.-Stage Hard | ROM | D1 p12 | |
| F02-W0020 | 15 | E.P.-Sta Hard LP | ROM | D1 p12 | |
| F02-W0021 | 16 | E.P.-Wurly | ROM | D1 p12 | |
| F02-W0022 | 17 | E.P.-Wurly LP | ROM | D1 p12 | |
| F02-W0023 | 18 | E.P. Pad 1 | ROM | D1 p12 | |
| F02-W0024 | 19 | E.P. Pad 1 LP | ROM | D1 p12 | |
| F02-W0025 | 20 | E.P. Pad 2 | ROM | D1 p12 | |
| F02-W0026 | 21 | E.P. Pad 3 | ROM | D1 p12 | |
| F02-W0027 | 22 | E.P. Pad 3 LP | ROM | D1 p12 | |
| F02-W0028 | 23 | Clav 1 | ROM | D1 p12 | |
| F02-W0029 | 24 | Harpsichord-Sngl | ROM | D1 p12 | |
| F02-W0030 | 25 | E.Organ-Perc 1 | ROM | D1 p12 | |
| F02-W0031 | 26 | E.Organ-Perc 2 | ROM | D1 p12 | |
| F02-W0032 | 27 | E.Organ-Perc 3 | ROM | D1 p12 | |
| F02-W0033 | 28 | E.Organ-2' Perc | ROM | D1 p12 | |
| F02-W0034 | 29 | E.Organ-M1 1 | ROM | D1 p12 | |
| F02-W0035 | 30 | E.Organ-M1 2 | ROM | D1 p12 | |
| F02-W0036 | 31 | E.Organ-N1 5 | ROM | D1 p12 | |
| F02-W0037 | 32 | E.Organ-N1 3 | ROM | D1 p12 | |
| F02-W0038 | 33 | E.Organ-N1 3 LP | ROM | D1 p12 | |
| F02-W0039 | 34 | E.Organ-Jazz 1TR | ROM | D1 p12 | |
| F02-W0040 | 35 | E.Organ-Jazz 3TR | ROM | D1 p12 | |
| F02-W0041 | 36 | E.Organ-Perc 3rd | ROM | D1 p12 | |
| F02-W0042 | 37 | E.Organ-Vox | ROM | D1 p12 | |
| F02-W0043 | 38 | E.Organ-Vox 1TR | ROM | D1 p12 | |
| F02-W0044 | 39 | E.Organ-Vox 2TR | ROM | D1 p12 | |
| F02-W0045 | 40 | E.Organ-Vox 3TR | ROM | D1 p12 | |
| F02-W0046 | 41 | E.Organ-Soft | ROM | D1 p12 | |
| F02-W0047 | 42 | E.Organ-Full | ROM | D1 p12 | |
| F02-W0048 | 43 | E.Organ-Dist | ROM | D1 p12 | |
| F02-W0049 | 44 | Positive 1 | ROM | D1 p12 | |
| F02-W0050 | 45 | Positive 2 | ROM | D1 p12 | |
| F02-W0051 | 46 | Pipe-Mixture 1 | ROM | D1 p12 | |
| F02-W0052 | 47 | Pipe-Mixture 2 | ROM | D1 p12 | |
| F02-W0053 | 48 | Pipe-Reed | ROM | D1 p12 | |
| F02-W0054 | 49 | Pipe-Full | ROM | D1 p12 | |
| F02-W0055 | 50 | Kalimba | ROM | D1 p12 | |
| F02-W0056 | 51 | Kalimba-Mute | ROM | D1 p12 | |
| F02-W0057 | 52 | Kalimba N1 | ROM | D1 p12 | |
| F02-W0058 | 53 | Music Box 1 | ROM | D1 p12 | |
| F02-W0059 | 54 | Music Box 1 LP | ROM | D1 p12 | |
| F02-W0060 | 55 | Music Box 2 | ROM | D1 p12 | |
| F02-W0061 | 56 | Music Box 2 LP | ROM | D1 p12 | |
| F02-W0062 | 57 | Marimba | ROM | D1 p12 | |
| F02-W0063 | 58 | Marimba LP | ROM | D1 p12 | |
| F02-W0064 | 59 | Xylophone | ROM | D1 p12 | |
| F02-W0065 | 60 | Vibraphone | ROM | D1 p12 | |
| F02-W0066 | 61 | Vibraphone LP | ROM | D1 p12 | |
| F02-W0067 | 62 | Celesta | ROM | D1 p12 | |
| F02-W0068 | 63 | Celesta LP | ROM | D1 p12 | |
| F02-W0069 | 64 | Glockenspiel | ROM | D1 p12 | |
| F02-W0070 | 65 | Glockenspiel LP | ROM | D1 p12 | |
| F02-W0071 | 66 | Tubular Bell | ROM | D1 p12 | |
| F02-W0072 | 67 | Log Drum | ROM | D1 p12 | |
| F02-W0073 | 68 | Steel Drum | ROM | D1 p12 | |
| F02-W0074 | 69 | Steel Drum LP | ROM | D1 p12 | |
| F02-W0075 | 70 | Gamelan | ROM | D1 p12 | |
| F02-W0076 | 71 | Metal FM Bell | ROM | D1 p12 | |
| F02-W0077 | 72 | FM Bell | ROM | D1 p12 | |
| F02-W0078 | 73 | FM Glass Bell | ROM | D1 p12 | |
| F02-W0079 | 74 | Ensemble Bell | ROM | D1 p12 | |
| F02-W0080 | 75 | Flute | ROM | D1 p12 | |
| F02-W0081 | 76 | Piccolo | ROM | D1 p12 | |
| F02-W0082 | 77 | Pan Flute | ROM | D1 p12 | |
| F02-W0083 | 78 | Shakuhachi | ROM | D1 p12 | |
| F02-W0084 | 79 | Shakuhachi-Atk | ROM | D1 p12 | |
| F02-W0085 | 80 | Bottle | ROM | D1 p12 | |
| F02-W0086 | 81 | Recorder | ROM | D1 p12 | |
| F02-W0087 | 82 | Ocarina | ROM | D1 p12 | |
| F02-W0088 | 83 | Clarinet | ROM | D1 p12 | |
| F02-W0089 | 84 | DoubleReed | ROM | D1 p12 | |
| F02-W0090 | 85 | Oboe | ROM | D1 p12 | |
| F02-W0091 | 86 | English Horn | ROM | D1 p12 | |
| F02-W0092 | 87 | Bassoon | ROM | D1 p12 | |
| F02-W0093 | 88 | Baritone Sax-T1 | ROM | D1 p12 | |
| F02-W0094 | 89 | Baritone Sax | ROM | D1 p12 | |
| F02-W0095 | 90 | Tenor Sax-M1 | ROM | D1 p12 | |
| F02-W0096 | 91 | TenorSax-Medium | ROM | D1 p12 | |
| F02-W0097 | 92 | Alto Sax-01/W | ROM | D1 p12 | |
| F02-W0098 | 93 | Alto Sax-Soft | ROM | D1 p12 | |
| F02-W0099 | 94 | SopranoSax-Soft | ROM | D1 p12 | |
| F02-W0100 | 95 | Musette | ROM | D1 p12 | |
| F02-W0101 | 96 | Musette LP | ROM | D1 p12 | |
| F02-W0102 | 97 | Accordion | ROM | D1 p12 | |
| F02-W0103 | 98 | Harmonica | ROM | D1 p12 | |
| F02-W0104 | 99 | Bagpipe | ROM | D1 p12 | |
| F02-W0105 | 100 | F.Horn-T1 | ROM | D1 p12 | |
| F02-W0106 | 100 | Cl Ens.Chord-min | OrchS | D1 p14 | |
| F02-W0107 | 101 | French HornEns. | ROM | D1 p12 | |
| F02-W0108 | 101 | Cl Ens.Chord-maj | OrchS | D1 p14 | |
| F02-W0109 | 102 | Flugel Horn | ROM | D1 p12 | |
| F02-W0110 | 102 | Vi Solo Arp-dim | OrchS | D1 p14 | |
| F02-W0111 | 103 | Tuba | ROM | D1 p12 | |
| F02-W0112 | 103 | Vi Solo Arp-min | OrchS | D1 p14 | |
| F02-W0113 | 104 | Trombone-Soft | ROM | D1 p12 | |
| F02-W0114 | 104 | Vi Solo Arp-maj | OrchS | D1 p14 | |
| F02-W0115 | 105 | Trombone-Bright | ROM | D1 p12 | |
| F02-W0116 | 105 | Vi SoloChord-dim | OrchS | D1 p14 | |
| F02-W0117 | 106 | Trumpet-Medium | ROM | D1 p12 | |
| F02-W0118 | 106 | Vi SoloChord-min | OrchS | D1 p14 | |
| F02-W0119 | 107 | Trumpet-Muted | ROM | D1 p12 | |
| F02-W0120 | 107 | Vi SoloChord-maj | OrchS | D1 p14 | |
| F02-W0121 | 108 | Brass Ens-Sharp | ROM | D1 p12 | |
| F02-W0122 | 108 | Vi Solo Hit-dim | OrchS | D1 p14 | |
| F02-W0123 | 109 | Brass Ens-1 | ROM | D1 p12 | |
| F02-W0124 | 109 | Vi Solo Hit-min | OrchS | D1 p14 | |
| F02-W0125 | 110 | Brass Ens-1 LP | ROM | D1 p12 | |
| F02-W0126 | 110 | Vi Solo Hit-maj | OrchS | D1 p14 | |
| F02-W0127 | 111 | Brass Ens-2 | ROM | D1 p12 | |
| F02-W0128 | 111 | Cl Solo Arp-dim | OrchS | D1 p14 | |
| F02-W0129 | 112 | Brass Ens-2 LP | ROM | D1 p12 | |
| F02-W0130 | 112 | Cl Solo Arp-maj | OrchS | D1 p14 | |
| F02-W0131 | 113 | Voice-Choir | ROM | D1 p12 | |
| F02-W0132 | 114 | Voice-Choir REV | ROM | D1 p12 | |
| F02-W0133 | 115 | Voice-Pop Ooh | ROM | D1 p12 | |
| F02-W0134 | 116 | Voice-PopOoh REV | ROM | D1 p12 | |
| F02-W0135 | 117 | Voice-Pop Ah | ROM | D1 p12 | |
| F02-W0136 | 118 | Voice-Pop Ah REV | ROM | D1 p12 | |
| F02-W0137 | 119 | Voice-Doo | ROM | D1 p12 | |
| F02-W0138 | 120 | Voice-Doo LP | ROM | D1 p12 | |
| F02-W0139 | 121 | Voice Wave 1 | ROM | D1 p12 | |
| F02-W0140 | 122 | Voice Wave 2 | ROM | D1 p12 | |
| F02-W0141 | 123 | String Ens. | ROM | D1 p12 | |
| F02-W0142 | 124 | String Ens. REV | ROM | D1 p12 | |
| F02-W0143 | 125 | Pizzicato Ens. | ROM | D1 p12 | |
| F02-W0144 | 126 | Violin | ROM | D1 p12 | |
| F02-W0145 | 127 | Viola | ROM | D1 p12 | |
| F02-W0146 | 128 | Cello&Contrabass | ROM | D1 p12 | |
| F02-W0147 | 129 | String Quartet 1 | ROM | D1 p12 | |
| F02-W0148 | 130 | String Quartet 2 | ROM | D1 p12 | |
| F02-W0149 | 131 | String Qurt2 REV | ROM | D1 p12 | |
| F02-W0150 | 132 | Pizzicato | ROM | D1 p12 | |
| F02-W0151 | 133 | A.Guitar-Nylon | ROM | D1 p12 | |
| F02-W0152 | 134 | A.Guitar-Finger | ROM | D1 p12 | |
| F02-W0153 | 135 | A.Guitar-Pick | ROM | D1 p12 | |
| F02-W0154 | 136 | A.Gtr-Harmonics | ROM | D1 p12 | |
| F02-W0155 | 137 | Clean Gtr 1-Stra | ROM | D1 p12 | |
| F02-W0156 | 138 | Clean Gtr 2-Stra | ROM | D1 p12 | |
| F02-W0157 | 139 | Clean Gtr 3-Tele | ROM | D1 p12 | |
| F02-W0158 | 140 | Clean Gtr-Mute 1 | ROM | D1 p12 | |
| F02-W0159 | 141 | Clean Gtr-Mute 2 | ROM | D1 p12 | |
| F02-W0160 | 142 | Funky Gtr 1-Stra | ROM | D1 p12 | |
| F02-W0161 | 143 | Funky Gtr 2-Stra | ROM | D1 p12 | |
| F02-W0162 | 144 | Jazz Guitar | ROM | D1 p12 | |
| F02-W0163 | 145 | Pedal Steel Gtr. | ROM | D1 p12 | |
| F02-W0164 | 146 | E.Gtr-Harmonics | ROM | D1 p12 | |
| F02-W0165 | 147 | OverDrive Guitar | ROM | D1 p12 | |
| F02-W0166 | 148 | Dist. Guitar 1 | ROM | D1 p12 | |
| F02-W0167 | 149 | Dist. Guitar 2 | ROM | D1 p12 | |
| F02-W0168 | 150 | Dist.Gtr-Harmo | ROM | D1 p12 | |
| F02-W0169 | 151 | Dist.Gtr-Mute | ROM | D1 p12 | |
| F02-W0170 | 152 | Power Chord | ROM | D1 p12 | |
| F02-W0171 | 153 | Fret Noise | ROM | D1 p12 | |
| F02-W0172 | 154 | A.Bass 1 | ROM | D1 p12 | |
| F02-W0173 | 155 | A.Bass 2 | ROM | D1 p12 | |
| F02-W0174 | 156 | A.Bass 2 LP | ROM | D1 p12 | |
| F02-W0175 | 157 | FingerBass-Clear | ROM | D1 p12 | |
| F02-W0176 | 158 | Finger Bass 1 | ROM | D1 p12 | |
| F02-W0177 | 159 | Finger Bass 1 LP | ROM | D1 p12 | |
| F02-W0178 | 160 | Finger Bass 2 | ROM | D1 p12 | |
| F02-W0179 | 161 | Finger Bass 2 LP | ROM | D1 p12 | |
| F02-W0180 | 162 | Pick Bass-M1 | ROM | D1 p12 | |
| F02-W0181 | 163 | Pick Bass-Hard | ROM | D1 p12 | |
| F02-W0182 | 164 | Pick Bass | ROM | D1 p12 | |
| F02-W0183 | 165 | Pick Bass LP | ROM | D1 p12 | |
| F02-W0184 | 166 | SlapBass-ThumbJB | ROM | D1 p12 | |
| F02-W0185 | 167 | SlapBass-Thumb 1 | ROM | D1 p12 | |
| F02-W0186 | 168 | SlapBass-Thum1LP | ROM | D1 p12 | |
| F02-W0187 | 169 | SlapBass-Thumb 2 | ROM | D1 p12 | |
| F02-W0188 | 170 | SlapBass-Thum2LP | ROM | D1 p12 | |
| F02-W0189 | 171 | SlapBass-Pull JB | ROM | D1 p12 | |
| F02-W0190 | 172 | SlapBass-Pull | ROM | D1 p12 | |
| F02-W0191 | 173 | SlapBass-Mute | ROM | D1 p12 | |
| F02-W0192 | 174 | SlapBass-Mute LP | ROM | D1 p12 | |
| F02-W0193 | 175 | SlapBass-M1 | ROM | D1 p12 | |
| F02-W0194 | 176 | Fretless Bass | ROM | D1 p12 | |
| F02-W0195 | 177 | Fretless Bass-M1 | ROM | D1 p12 | |
| F02-W0196 | 178 | Bass Harmonics | ROM | D1 p12 | |
| F02-W0197 | 179 | Bass Harmo LP | ROM | D1 p12 | |
| F02-W0198 | 180 | Sitar | ROM | D1 p12 | |
| F02-W0199 | 181 | Sitar&Tambura | ROM | D1 p12 | |
| F02-W0200 | 182 | Sitar&Tambura LP | ROM | D1 p12 | |
| F02-W0201 | 183 | Santur | ROM | D1 p12 | |
| F02-W0202 | 184 | Santur LP | ROM | D1 p12 | |
| F02-W0203 | 185 | Mandolin | ROM | D1 p12 | |
| F02-W0204 | 186 | Mandolin LP | ROM | D1 p12 | |
| F02-W0205 | 187 | Banjo | ROM | D1 p12 | |
| F02-W0206 | 188 | Banjo LP | ROM | D1 p12 | |
| F02-W0207 | 189 | Shamisen | ROM | D1 p12 | |
| F02-W0208 | 190 | Koto | ROM | D1 p12 | |
| F02-W0209 | 191 | Harp | ROM | D1 p12 | |
| F02-W0210 | 192 | Harp LP | ROM | D1 p12 | |
| F02-W0211 | 193 | Harp-Glissando | ROM | D1 p12 | |
| F02-W0212 | 194 | Harp-Gliss REV | ROM | D1 p12 | |
| F02-W0213 | 195 | Sine Pop Bass | ROM | D1 p12 | |
| F02-W0214 | 196 | Syn Bass-Reso 1 | ROM | D1 p12 | |
| F02-W0215 | 197 | Syn Bass-Reso 2 | ROM | D1 p12 | |
| F02-W0216 | 198 | Syn Bass-Dirty | ROM | D1 p12 | |
| F02-W0217 | 199 | Syn Bass-FM 1 | ROM | D1 p12 | |
| F02-W0218 | 200 | Syn Bass-FM 1 LP | ROM | D1 p12 | |
| F02-W0219 | 201 | Syn Bass-FM 2 | ROM | D1 p12 | |
| F02-W0220 | 202 | Syn Bass-FM 2 LP | ROM | D1 p12 | |
| F02-W0221 | 203 | Syn Bass-TB | ROM | D1 p12 | |
| F02-W0222 | 204 | Syn Bass-RBSaw | ROM | D1 p12 | |
| F02-W0223 | 205 | Syn Bass-RBSqr | ROM | D1 p12 | |
| F02-W0224 | 206 | Sync | ROM | D1 p12 | |
| F02-W0225 | 207 | ChromRes | ROM | D1 p12 | |
| F02-W0226 | 208 | Syn-Pop | ROM | D1 p12 | |
| F02-W0227 | 209 | Detuned-Super | ROM | D1 p12 | |
| F02-W0228 | 210 | Detuned-SuperREV | ROM | D1 p12 | |
| F02-W0229 | 211 | Detuned-Super 2 | ROM | D1 p12 | |
| F02-W0230 | 212 | Detun-Super2 REV | ROM | D1 p12 | |
| F02-W0231 | 213 | Detuned-PWM | ROM | D1 p12 | |
| F02-W0232 | 214 | Detuned-PWM REV | ROM | D1 p12 | |
| F02-W0233 | 215 | Analog Strings 1 | ROM | D1 p12 | |
| F02-W0234 | 216 | AnaStrings 1 REV | ROM | D1 p12 | |
| F02-W0235 | 217 | Analog Strings 2 | ROM | D1 p12 | |
| F02-W0236 | 218 | AnaStrings 2 REV | ROM | D1 p12 | |
| F02-W0237 | 219 | White Pad | ROM | D1 p12 | |
| F02-W0238 | 220 | White Pad REV | ROM | D1 p12 | |
| F02-W0239 | 221 | Glass Vox | ROM | D1 p12 | |
| F02-W0240 | 222 | Glass Vox REV | ROM | D1 p12 | |
| F02-W0241 | 223 | Syn-Air Vox | ROM | D1 p12 | |
| F02-W0242 | 224 | Syn-Air Vox REV | ROM | D1 p12 | |
| F02-W0243 | 225 | Syn-Ghostly | ROM | D1 p12 | |
| F02-W0244 | 226 | Syn-Ghostly REV | ROM | D1 p12 | |
| F02-W0245 | 227 | Syn-Air Pad | ROM | D1 p12 | |
| F02-W0246 | 228 | Syn-Air Pad REV | ROM | D1 p12 | |
| F02-W0247 | 229 | Syn-Flute Pad | ROM | D1 p12 | |
| F02-W0248 | 230 | Syn-FlutePad REV | ROM | D1 p12 | |
| F02-W0249 | 231 | Syn-DreamStrings | ROM | D1 p12 | |
| F02-W0250 | 232 | Syn-DreamStr REV | ROM | D1 p12 | |
| F02-W0251 | 233 | Syn-Golithan | ROM | D1 p12 | |
| F02-W0252 | 234 | Syn-Golithan REV | ROM | D1 p12 | |
| F02-W0253 | 235 | Syn-Air Vortex | ROM | D1 p12 | |
| F02-W0254 | 236 | Syn-AirVortexREV | ROM | D1 p12 | |
| F02-W0255 | 237 | Syn-FX01Z | ROM | D1 p12 | |
| F02-W0256 | 238 | Syn-FX01Z REV | ROM | D1 p12 | |
| F02-W0257 | 239 | Syn-Palawan | ROM | D1 p12 | |
| F02-W0258 | 240 | Syn-Palawan REV | ROM | D1 p12 | |
| F02-W0259 | 241 | WaveSweep | ROM | D1 p12 | |
| F02-W0260 | 242 | WaveSweepA | ROM | D1 p12 | |
| F02-W0261 | 243 | WaveSweepA REV | ROM | D1 p12 | |
| F02-W0262 | 244 | WaveSweepB | ROM | D1 p12 | |
| F02-W0263 | 245 | Syn-Clicker | ROM | D1 p12 | |
| F02-W0264 | 246 | Syn-Clicker REV | ROM | D1 p12 | |
| F02-W0265 | 247 | Xylo-Spectrum | ROM | D1 p12 | |
| F02-W0266 | 248 | Cricket Spectrum | ROM | D1 p12 | |
| F02-W0267 | 249 | Syn-FX07Z | ROM | D1 p12 | |
| F02-W0268 | 250 | Swish Terra | ROM | D1 p12 | |
| F02-W0269 | 251 | Noise | ROM | D1 p12 | |
| F02-W0270 | 252 | Noise Spectrum 1 | ROM | D1 p12 | |
| F02-W0271 | 253 | Noise Spectrum 2 | ROM | D1 p12 | |
| F02-W0272 | 254 | Space Lore | ROM | D1 p12 | |
| F02-W0273 | 255 | Space Lore REV | ROM | D1 p12 | |
| F02-W0274 | 256 | Telephone Ring | ROM | D1 p12 | |
| F02-W0275 | 257 | Tele Ring REV | ROM | D1 p12 | |
| F02-W0276 | 258 | Syn-FX02Z | ROM | D1 p12 | |
| F02-W0277 | 259 | Syn-FX02Z REV | ROM | D1 p12 | |
| F02-W0278 | 260 | Pole | ROM | D1 p13 | |
| F02-W0279 | 261 | Pole LP | ROM | D1 p13 | |
| F02-W0280 | 262 | Gamelan Wave | ROM | D1 p13 | |
| F02-W0281 | 263 | Saw | ROM | D1 p13 | |
| F02-W0282 | 264 | Saw-MG3 | ROM | D1 p13 | |
| F02-W0283 | 265 | Saw-Chroma | ROM | D1 p13 | |
| F02-W0284 | 266 | Pulse-02% | ROM | D1 p13 | |
| F02-W0285 | 267 | Pulse-05% | ROM | D1 p13 | |
| F02-W0286 | 268 | Pulse-08% | ROM | D1 p13 | |
| F02-W0287 | 269 | Pulse-16% | ROM | D1 p13 | |
| F02-W0288 | 270 | Pulse-33% | ROM | D1 p13 | |
| F02-W0289 | 271 | Pulse-40% | ROM | D1 p13 | |
| F02-W0290 | 272 | Square | ROM | D1 p13 | |
| F02-W0291 | 273 | Sqr-mMG | ROM | D1 p13 | |
| F02-W0292 | 274 | Square-JP | ROM | D1 p13 | |
| F02-W0293 | 275 | Triangle-MG3 | ROM | D1 p13 | |
| F02-W0294 | 276 | Ramp | ROM | D1 p13 | |
| F02-W0295 | 277 | Ramp-mMG | ROM | D1 p13 | |
| F02-W0296 | 278 | Sine | ROM | D1 p13 | |
| F02-W0297 | 279 | Sine-JP | ROM | D1 p13 | |
| F02-W0298 | 280 | DWGS-Syn Sine 1 | ROM | D1 p13 | |
| F02-W0299 | 281 | DWGS-Syn Sine 2 | ROM | D1 p13 | |
| F02-W0300 | 282 | DWGS-E.P.1 | ROM | D1 p13 | |
| F02-W0301 | 283 | DWGS-E.P.2 | ROM | D1 p13 | |
| F02-W0302 | 284 | DWGS-E.P.3 | ROM | D1 p13 | |
| F02-W0303 | 285 | DWGS-E.P.4 | ROM | D1 p13 | |
| F02-W0304 | 286 | DWGS-E.P.5 | ROM | D1 p13 | |
| F02-W0305 | 287 | DWGS-Organ 1 | ROM | D1 p13 | |
| F02-W0306 | 288 | DWGS-Organ 2 | ROM | D1 p13 | |
| F02-W0307 | 289 | DWGS-Organ 3 | ROM | D1 p13 | |
| F02-W0308 | 290 | DWGS-Organ 4 | ROM | D1 p13 | |
| F02-W0309 | 291 | DWGS-Organ 5 | ROM | D1 p13 | |
| F02-W0310 | 292 | DWGS-Accordion | ROM | D1 p13 | |
| F02-W0311 | 293 | DWGS Guitar 1 | ROM | D1 p13 | |
| F02-W0312 | 294 | DWGS-Guitar 2 | ROM | D1 p13 | |
| F02-W0313 | 295 | DWGS-Bell 1 | ROM | D1 p13 | |
| F02-W0314 | 296 | DWGS-Bell 2 | ROM | D1 p13 | |
| F02-W0315 | 297 | DWGS-Bell 3 | ROM | D1 p13 | |
| F02-W0316 | 298 | DWGS-Bell 4 | ROM | D1 p13 | |
| F02-W0317 | 299 | DWGS-Clav 1 | ROM | D1 p13 | |
| F02-W0318 | 300 | DWGS-Clav 2 | ROM | D1 p13 | |
| F02-W0319 | 301 | DWGS-Clav 3 | ROM | D1 p13 | |
| F02-W0320 | 302 | DWGS-Digi 1 | ROM | D1 p13 | |
| F02-W0321 | 303 | DWGS-Digi 2 | ROM | D1 p13 | |
| F02-W0322 | 304 | DWGS-Digi 3 | ROM | D1 p13 | |
| F02-W0323 | 305 | DWGS-Wire 1 | ROM | D1 p13 | |
| F02-W0324 | 306 | DWGS-Wire 2 | ROM | D1 p13 | |
| F02-W0325 | 307 | DWGS-Sync 1 | ROM | D1 p13 | |
| F02-W0326 | 308 | DWGS-Sync 2 | ROM | D1 p13 | |
| F02-W0327 | 309 | DWGS-Sync 3 | ROM | D1 p13 | |
| F02-W0328 | 310 | Orchestra Hit | ROM | D1 p13 | |
| F02-W0329 | 311 | Pizz Hit | ROM | D1 p13 | |
| F02-W0330 | 312 | Band Hit | ROM | D1 p13 | |
| F02-W0331 | 313 | Impact Hit L | ROM | D1 p13 | |
| F02-W0332 | 314 | Impact Hit R | ROM | D1 p13 | |
| F02-W0333 | 315 | Philly Hit | ROM | D1 p13 | |
| F02-W0334 | 316 | Brass Fall | ROM | D1 p13 | |
| F02-W0335 | 317 | Organ Hit | ROM | D1 p13 | |
| F02-W0336 | 318 | FunkyGuitarHit | ROM | D1 p13 | |
| F02-W0337 | 319 | Pop Guitar Hit | ROM | D1 p13 | |
| F02-W0338 | 320 | ReggaeGuitar Hit | ROM | D1 p13 | |
| F02-W0339 | 321 | Vox-Wah Gtr | ROM | D1 p13 | |
| F02-W0340 | 322 | GtCutNois 1 | ROM | D1 p13 | |
| F02-W0341 | 323 | GtCutNois 2 | ROM | D1 p13 | |
| F02-W0342 | 324 | String Slap | ROM | D1 p13 | |
| F02-W0343 | 325 | Voice Set | ROM | D1 p13 | |
| F02-W0344 | 326 | Voice *Do it* | ROM | D1 p13 | |
| F02-W0345 | 327 | Voice *Do it* LP | ROM | D1 p13 | |
| F02-W0346 | 328 | VibeChord 1 | ROM | D1 p13 | |
| F02-W0347 | 329 | VibeChord 2 | ROM | D1 p13 | |
| F02-W0348 | 330 | VibeChord 1&2 | ROM | D1 p13 | |
| F02-W0349 | 331 | Cyber Drum | ROM | D1 p13 | |
| F02-W0350 | 332 | Cyber Drum RPT | ROM | D1 p13 | |
| F02-W0351 | 333 | Industry | ROM | D1 p13 | |
| F02-W0352 | 334 | Industry RPT | ROM | D1 p13 | |
| F02-W0353 | 335 | SD-Metal Striker | ROM | D1 p13 | |
| F02-W0354 | 336 | SD-M.Striker RPT | ROM | D1 p13 | |
| F02-W0355 | 337 | SD-Air Explosion | ROM | D1 p13 | |
| F02-W0356 | 338 | Metal Hit 4 | ROM | D1 p13 | |
| F02-W0357 | 339 | Metal Hit 4 RPT | ROM | D1 p13 | |
| F02-W0358 | 340 | Industry-66 | ROM | D1 p13 | |
| F02-W0359 | 341 | Industry-66 RPT | ROM | D1 p13 | |
| F02-W0360 | 342 | Industry-21 | ROM | D1 p13 | |
| F02-W0361 | 343 | BD-Cannon | ROM | D1 p13 | |
| F02-W0362 | 344 | Zap 1 | ROM | D1 p13 | |
| F02-W0363 | 345 | Zap 2 | ROM | D1 p13 | |
| F02-W0364 | 346 | Scratch Hi | ROM | D1 p13 | |
| F02-W0365 | 347 | DJ-Old Record | ROM | D1 p13 | |
| F02-W0366 | 348 | Stadium | ROM | D1 p13 | |
| F02-W0367 | 349 | Applause | ROM | D1 p13 | |
| F02-W0368 | 350 | Birds 1 | ROM | D1 p13 | |
| F02-W0369 | 351 | Birds 1 REV | ROM | D1 p13 | |
| F02-W0370 | 352 | Bird 2 | ROM | D1 p13 | |
| F02-W0371 | 353 | Crickets 1 | ROM | D1 p13 | |
| F02-W0372 | 354 | Crickets 1 REV | ROM | D1 p13 | |
| F02-W0373 | 355 | Crickets 2 | ROM | D1 p13 | |
| F02-W0374 | 356 | ChurchBell | ROM | D1 p13 | |
| F02-W0375 | 357 | ChurchBell REV | ROM | D1 p13 | |
| F02-W0376 | 358 | Thunder | ROM | D1 p13 | |
| F02-W0377 | 359 | Stream | ROM | D1 p13 | |
| F02-W0378 | 360 | Bubble | ROM | D1 p13 | |
| F02-W0379 | 361 | Bubble REV | ROM | D1 p13 | |
| F02-W0380 | 362 | Dog | ROM | D1 p13 | |
| F02-W0381 | 363 | Gallop | ROM | D1 p13 | |
| F02-W0382 | 364 | Gallop REV | ROM | D1 p13 | |
| F02-W0383 | 365 | Laughing | ROM | D1 p13 | |
| F02-W0384 | 366 | Scream | ROM | D1 p13 | |
| F02-W0385 | 367 | Punch | ROM | D1 p13 | |
| F02-W0386 | 368 | Heart Beat | ROM | D1 p13 | |
| F02-W0387 | 369 | Footstep 1 | ROM | D1 p13 | |
| F02-W0388 | 370 | Footstep 2 | ROM | D1 p13 | |
| F02-W0389 | 371 | Door Creak | ROM | D1 p13 | |
| F02-W0390 | 372 | Door Slam | ROM | D1 p13 | |
| F02-W0391 | 373 | Car Engine | ROM | D1 p13 | |
| F02-W0392 | 374 | Car Engine LP | ROM | D1 p13 | |
| F02-W0393 | 375 | Car Stop | ROM | D1 p13 | |
| F02-W0394 | 376 | Car Pass | ROM | D1 p13 | |
| F02-W0395 | 377 | Car Crash | ROM | D1 p13 | |
| F02-W0396 | 378 | Train | ROM | D1 p13 | |
| F02-W0397 | 379 | Train REV | ROM | D1 p13 | |
| F02-W0398 | 380 | Helicopter | ROM | D1 p13 | |
| F02-W0399 | 381 | Helicopter REV | ROM | D1 p13 | |
| F02-W0400 | 382 | Gun Shot 2 | ROM | D1 p13 | |
| F02-W0401 | 383 | Machine Gun | ROM | D1 p13 | |
| F02-W0402 | 384 | Machine Gun REV | ROM | D1 p13 | |
| F02-W0403 | 385 | Laser Gun | ROM | D1 p13 | |
| F02-W0404 | 386 | Explosion | ROM | D1 p13 | |
| F02-W0405 | 387 | Wind | ROM | D1 p13 | |
| F02-W0406 | 388 | Timpani | ROM | D1 p13 | |
| F02-W0407 | 389 | Crash 1 | ROM | D1 p13 | |
| F02-W0408 | 390 | Crash 2 | ROM | D1 p13 | |
| F02-W0409 | 391 | Cymbal Reverse | ROM | D1 p13 | |
| F02-W0410 | 392 | Orch. Cymbal | ROM | D1 p13 | |
| F02-W0411 | 393 | Ride-Jazz | ROM | D1 p13 | |
| F02-W0412 | 394 | Ride-Edge 1 | ROM | D1 p13 | |
| F02-W0413 | 395 | Ride-Edge 2 | ROM | D1 p13 | |
| F02-W0414 | 396 | H.Hat-Closed | ROM | D1 p13 | |
| F02-W0415 | 397 | 88-HHat Open | ROM | D1 p13 | |
| F02-W0416 | 398 | 88-Cowbell | ROM | D1 p13 | |
| F02-W0417 | 399 | 88-Tom | ROM | D1 p13 | |
| F02-W0418 | 400 | 88-Conga | ROM | D1 p13 | |
| F02-W0419 | 401 | 88-Crash | ROM | D1 p13 | |
| F02-W0420 | 402 | Tom | ROM | D1 p13 | |
| F02-W0421 | 403 | Tom-Brush | ROM | D1 p13 | |
| F02-W0422 | 404 | Tom-Processed | ROM | D1 p13 | |
| F02-W0423 | 405 | Real E.Tom | ROM | D1 p13 | |
| F02-W0424 | 406 | Flexatone | ROM | D1 p13 | |
| F02-W0425 | 407 | Tambourine | ROM | D1 p13 | |
| F02-W0426 | 408 | Tambourine-RPT | ROM | D1 p13 | |
| F02-W0427 | 409 | Agogo Bell | ROM | D1 p13 | |
| F02-W0428 | 410 | Cowbell | ROM | D1 p13 | |
| F02-W0429 | 411 | Marc Tree | ROM | D1 p13 | |
| F02-W0430 | 412 | Marc Tree LP | ROM | D1 p13 | |
| F02-W0431 | 413 | Bell Tree | ROM | D1 p13 | |
| F02-W0432 | 414 | Bell Tree LP | ROM | D1 p13 | |
| F02-W0433 | 415 | Rainstick | ROM | D1 p13 | |
| F02-W0434 | 416 | TempleBlocks | ROM | D1 p13 | |
| F02-W0435 | 417 | WoodBlock | ROM | D1 p13 | |
| F02-W0436 | 418 | Click | ROM | D1 p13 | |
| F02-W0437 | 419 | Castanet | ROM | D1 p13 | |
| F02-W0438 | 420 | Taiko | ROM | D1 p13 | |
| F02-W0439 | 421 | Orch BD | ROM | D1 p13 | |
| F02-W0440 | 422 | Djembe-Open | ROM | D1 p13 | |
| F02-W0441 | 423 | Djembe-Mute | ROM | D1 p13 | |
| F02-W0442 | 424 | Chinese Gong | ROM | D1 p13 | |

### Drumsamples

| Shipped label | Number | Documented name | Group | Source | Note |
|---|---|---|---|---|---|
| F02-W0001 | 0 | BD-Dry 1 | ROM | D1 p15 | |
| F02-W0002 | 0 | BD-EXB04 000 | Best | D1 p16 | |
| F02-W0003 | 0 | Vi Ens. Arp-dim | OrchS | D1 p17 | |
| F02-W0004 | 0 | Timpani ff Hi | OrchB | D1 p18 | |
| F02-W0005 | 0 | BD-EXB05 00 | Vint | D1 p18 | |
| F02-W0006 | 0 | BD-EXB09 01 | Synth | D1 p18 | |
| F02-W0007 | 1 | BD-Dry 2 | ROM | D1 p15 | |
| F02-W0008 | 1 | BD-EXB04 001 | Best | D1 p16 | |
| F02-W0009 | 1 | Vi Ens. Arp-min | OrchS | D1 p17 | |
| F02-W0010 | 1 | Timpani ff Lo | OrchB | D1 p18 | |
| F02-W0011 | 1 | BD-EXB05 01 | Vint | D1 p18 | |
| F02-W0012 | 1 | BD-EXB09 02 | Synth | D1 p18 | |
| F02-W0013 | 2 | BD-Dry 3 | ROM | D1 p15 | |
| F02-W0014 | 2 | BD-EXB04 002 | Best | D1 p16 | |
| F02-W0015 | 2 | Vi Ens. Arp-maj | OrchS | D1 p17 | |
| F02-W0016 | 2 | Timpani mf Hi | OrchB | D1 p18 | |
| F02-W0017 | 2 | BDEX-EXB05 00 | Vint | D1 p18 | |
| F02-W0018 | 2 | BD-EXB09 03 | Synth | D1 p18 | |
| F02-W0019 | 3 | BD-Dry 4 | ROM | D1 p15 | |
| F02-W0020 | 3 | BD-EXB04 003 | Best | D1 p16 | |
| F02-W0021 | 3 | Cl Ens. Arp-dim | OrchS | D1 p17 | |
| F02-W0022 | 3 | Timpani mf Lo | OrchB | D1 p18 | |
| F02-W0023 | 3 | BDEX-EXB05 01 | Vint | D1 p18 | |
| F02-W0024 | 3 | BD-EXB09 04 | Synth | D1 p18 | |
| F02-W0025 | 4 | BD-Dry 5 | ROM | D1 p15 | |
| F02-W0026 | 4 | BD-EXB04 004 | Best | D1 p16 | |
| F02-W0027 | 4 | Cl Ens. Arp-min | OrchS | D1 p17 | |
| F02-W0028 | 4 | Timpani pp Hi | OrchB | D1 p18 | |
| F02-W0029 | 4 | BDEX-EXB05 02 | Vint | D1 p18 | |
| F02-W0030 | 4 | BD-EXB09 05 | Synth | D1 p18 | |
| F02-W0031 | 5 | BD-Dry 6 | ROM | D1 p15 | |
| F02-W0032 | 5 | BD-EXB04 005 | Best | D1 p16 | |
| F02-W0033 | 5 | Cl Ens. Arp-maj | OrchS | D1 p17 | |
| F02-W0034 | 5 | Timpani pp Lo | OrchB | D1 p18 | |
| F02-W0035 | 5 | BDEX-EXB05 03 | Vint | D1 p18 | |
| F02-W0036 | 5 | BD-EXB09 06 | Synth | D1 p18 | |
| F02-W0037 | 6 | BD-Soft | ROM | D1 p15 | |
| F02-W0038 | 6 | BD-EXB04 006 | Best | D1 p16 | |
| F02-W0039 | 6 | Vi Ens.Chord-dim | OrchS | D1 p17 | |
| F02-W0040 | 6 | Timpani-Roll | OrchB | D1 p18 | |
| F02-W0041 | 6 | BDEX-EXB05 04 | Vint | D1 p18 | |
| F02-W0042 | 6 | BD-EXB09 07 | Synth | D1 p18 | |
| F02-W0043 | 7 | BD-Soft Room | ROM | D1 p15 | |
| F02-W0044 | 7 | BD-EXB04 007 | Best | D1 p16 | |
| F02-W0045 | 7 | Vi Ens.Chord-min | OrchS | D1 p17 | |
| F02-W0046 | 7 | BD-Orch. ff | OrchB | D1 p18 | |
| F02-W0047 | 7 | BDEX-EXB05 05 | Vint | D1 p18 | |
| F02-W0048 | 7 | BD-EXB09 08 | Synth | D1 p18 | |
| F02-W0049 | 8 | BD-Jazz 1 | ROM | D1 p15 | |
| F02-W0050 | 8 | BD-EXB04 008 | Best | D1 p16 | |
| F02-W0051 | 8 | Vi Ens.Chord-maj | OrchS | D1 p17 | |
| F02-W0052 | 8 | BD-Orch. pp | OrchB | D1 p18 | |
| F02-W0053 | 8 | BDEX-EXB05 06 | Vint | D1 p18 | |
| F02-W0054 | 8 | BD-EXB09 09 | Synth | D1 p18 | |
| F02-W0055 | 9 | BD-Jazz 2 | ROM | D1 p15 | |
| F02-W0056 | 9 | BD-EXB04 009 | Best | D1 p16 | |
| F02-W0057 | 9 | Cl Ens.Chord-dim | OrchS | D1 p17 | |
| F02-W0058 | 9 | SD-Orch. ff 1 | OrchB | D1 p18 | |
| F02-W0059 | 9 | BDEX-EXB05 07 | Vint | D1 p18 | |
| F02-W0060 | 9 | BD-EXB09 10 | Synth | D1 p18 | |
| F02-W0061 | 10 | BD-Pillow | ROM | D1 p15 | |
| F02-W0062 | 10 | BD-EXB04 010 | Best | D1 p16 | |
| F02-W0063 | 10 | Cl Ens.Chord-min | OrchS | D1 p17 | |
| F02-W0064 | 10 | SD-Orch. ff 2 | OrchB | D1 p18 | |
| F02-W0065 | 10 | BDEX-EXB05 08 | Vint | D1 p18 | |
| F02-W0066 | 10 | BD-EXB09 11 | Synth | D1 p18 | |
| F02-W0067 | 11 | BD-Woofer | ROM | D1 p15 | |
| F02-W0068 | 11 | BD-EXB04 011 | Best | D1 p16 | |
| F02-W0069 | 11 | Cl Ens.Chord-maj | OrchS | D1 p17 | |
| F02-W0070 | 11 | SD-Orch. mf 1 | OrchB | D1 p18 | |
| F02-W0071 | 11 | SD-EXB05 00 | Vint | D1 p18 | |
| F02-W0072 | 11 | BD-EXB09 12 | Synth | D1 p18 | |
| F02-W0073 | 12 | BD-Mondo Kill | ROM | D1 p15 | |
| F02-W0074 | 12 | BD-EXB04 012 | Best | D1 p16 | |
| F02-W0075 | 12 | Vi Ens.Hit-dim 1 | OrchS | D1 p17 | |
| F02-W0076 | 12 | SD-Orch. mf 2 | OrchB | D1 p18 | |
| F02-W0077 | 12 | SD-EXB05 01 | Vint | D1 p18 | |
| F02-W0078 | 12 | BD-EXB09 13 | Synth | D1 p18 | |
| F02-W0079 | 13 | BD-Terminator | ROM | D1 p15 | |
| F02-W0080 | 13 | BD-EXB04 013 | Best | D1 p16 | |
| F02-W0081 | 13 | Vi Ens.Hit-dim 2 | OrchS | D1 p17 | |
| F02-W0082 | 13 | SD-Orch. pp 1 | OrchB | D1 p18 | |
| F02-W0083 | 13 | SD-EXB05 02 | Vint | D1 p18 | |
| F02-W0084 | 13 | BD-EXB09 14 | Synth | D1 p18 | |
| F02-W0085 | 14 | BD-Tubby | ROM | D1 p15 | |
| F02-W0086 | 14 | BD-EXB04 014 | Best | D1 p16 | |
| F02-W0087 | 14 | Vi Ens.Hit-min 1 | OrchS | D1 p17 | |
| F02-W0088 | 14 | SD-Orch. pp 2 | OrchB | D1 p18 | |
| F02-W0089 | 14 | SD-EXB05 03 | Vint | D1 p18 | |
| F02-W0090 | 14 | BD-EXB09 15 | Synth | D1 p18 | |
| F02-W0091 | 15 | BD-Gated | ROM | D1 p15 | |
| F02-W0092 | 15 | BD-EXB04 015 | Best | D1 p16 | |
| F02-W0093 | 15 | Vi Ens.Hit-min 2 | OrchS | D1 p17 | |
| F02-W0094 | 15 | SD-Orch.-Roll | OrchB | D1 p18 | |
| F02-W0095 | 15 | SD-EXB05 04 | Vint | D1 p18 | |
| F02-W0096 | 15 | BD-EXB09 16 | Synth | D1 p18 | |
| F02-W0097 | 16 | BD-Tight | ROM | D1 p15 | |
| F02-W0098 | 16 | BD-EXB04 016 | Best | D1 p16 | |
| F02-W0099 | 16 | Vi Ens.Hit-maj 1 | OrchS | D1 p17 | |
| F02-W0100 | 16 | Piatti ff | OrchB | D1 p18 | |
| F02-W0101 | 16 | SD-EXB05 05 | Vint | D1 p18 | |
| F02-W0102 | 16 | BD-EXB09 17 | Synth | D1 p18 | |
| F02-W0103 | 17 | BD-Squash | ROM | D1 p15 | |
| F02-W0104 | 17 | BD-EXB04 017 | Best | D1 p16 | |
| F02-W0105 | 17 | Vi Ens.Hit-maj 2 | OrchS | D1 p17 | |
| F02-W0106 | 17 | Piatti pp | OrchB | D1 p18 | |
| F02-W0107 | 17 | SDEX-EXB05 00 | Vint | D1 p18 | |
| F02-W0108 | 17 | BD-EXB09 18 | Synth | D1 p18 | |
| F02-W0109 | 18 | BD-Dance 1 | ROM | D1 p15 | |
| F02-W0110 | 18 | BD-EXB04 018 | Best | D1 p16 | |
| F02-W0111 | 18 | ViEns.RunUp-mi 1 | OrchS | D1 p17 | |
| F02-W0112 | 18 | Piatti Short | OrchB | D1 p18 | |
| F02-W0113 | 18 | SDEX-EXB05 01 | Vint | D1 p18 | |
| F02-W0114 | 18 | BD-EXB09 19 | Synth | D1 p18 | |
| F02-W0115 | 19 | BD-Dance 2 | ROM | D1 p15 | |
| F02-W0116 | 19 | BD-EXB04 019 | Best | D1 p16 | |
| F02-W0117 | 19 | ViEns.RunUp-mi 2 | OrchS | D1 p17 | |
| F02-W0118 | 19 | Gong | OrchB | D1 p18 | |
| F02-W0119 | 19 | SDEX-EXB05 02 | Vint | D1 p18 | |
| F02-W0120 | 19 | BD-EXB09 20 | Synth | D1 p18 | |
| F02-W0121 | 20 | BD-Dance 3 | ROM | D1 p15 | |
| F02-W0122 | 20 | BD-EXB04 020 | Best | D1 p16 | |
| F02-W0123 | 20 | ViEns.RunUp-mi 3 | OrchS | D1 p17 | |
| F02-W0124 | 20 | Triangle | OrchB | D1 p18 | |
| F02-W0125 | 20 | SDEX-EXB05 03 | Vint | D1 p18 | |
| F02-W0126 | 20 | BD-EXB09 21 | Synth | D1 p18 | |
| F02-W0127 | 21 | BD-Dance 4 | ROM | D1 p15 | |
| F02-W0128 | 21 | BD-EXB04 021 | Best | D1 p16 | |
| F02-W0129 | 21 | ViEns.RunUp-mi 4 | OrchS | D1 p17 | |
| F02-W0130 | 21 | Triangle-Mute | OrchB | D1 p18 | |
| F02-W0131 | 21 | SDEX-EXB05 04 | Vint | D1 p18 | |
| F02-W0132 | 21 | BD-EXB09 22 | Synth | D1 p18 | |
| F02-W0133 | 22 | BD-Hip 1 | ROM | D1 p15 | |
| F02-W0134 | 22 | BD-EXB04 022 | Best | D1 p16 | |
| F02-W0135 | 22 | ViEns.RunUp-mj 1 | OrchS | D1 p17 | |
| F02-W0136 | 22 | Triangle-Roll | OrchB | D1 p18 | |
| F02-W0137 | 22 | SDEX-EXB05 06 | Vint | D1 p18 | |
| F02-W0138 | 22 | BD-EXB09 23 | Synth | D1 p18 | |
| F02-W0139 | 23 | BD-Hip 2 | ROM | D1 p15 | |
| F02-W0140 | 23 | BD-EXB04 023 | Best | D1 p16 | |
| F02-W0141 | 23 | ViEns.RunUp-mj 2 | OrchS | D1 p17 | |
| F02-W0142 | 23 | Harp-Glissando 1 | OrchB | D1 p18 | |
| F02-W0143 | 23 | SDEX-EXB05 07 | Vint | D1 p18 | |
| F02-W0144 | 23 | BD-EXB09 24 | Synth | D1 p18 | |
| F02-W0145 | 24 | BD-Hip 3 | ROM | D1 p15 | |
| F02-W0146 | 24 | BD-EXB04 024 | Best | D1 p16 | |
| F02-W0147 | 24 | ViEns.RunUp-mj 3 | OrchS | D1 p17 | |
| F02-W0148 | 24 | Harp-Glissando 2 | OrchB | D1 p18 | |
| F02-W0149 | 24 | SDEX-EXB05 08 | Vint | D1 p18 | |
| F02-W0150 | 24 | BD-EXB09 25 | Synth | D1 p18 | |
| F02-W0151 | 25 | BD-Hip 4 | ROM | D1 p15 | |
| F02-W0152 | 25 | BD-EXB04 025 | Best | D1 p16 | |
| F02-W0153 | 25 | ViEns.RunUp-mj 4 | OrchS | D1 p17 | |
| F02-W0154 | 25 | Harp-Glissando 3 | OrchB | D1 p18 | |
| F02-W0155 | 25 | SDEX-EXB05 09 | Vint | D1 p18 | |
| F02-W0156 | 25 | BD-EXB09 26 | Synth | D1 p18 | |
| F02-W0157 | 26 | BD-Dark | ROM | D1 p15 | |
| F02-W0158 | 26 | BD-EXB04 026 | Best | D1 p16 | |
| F02-W0159 | 26 | Vi Solo Arp-dim | OrchS | D1 p17 | |
| F02-W0160 | 26 | Harp-Glissando 4 | OrchB | D1 p18 | |
| F02-W0161 | 26 | H.H.Cl-EXB05 00 | Vint | D1 p18 | |
| F02-W0162 | 26 | BD-EXB09 27 | Synth | D1 p18 | |
| F02-W0163 | 27 | BD-Pop | ROM | D1 p15 | |
| F02-W0164 | 27 | BD-EXB04 027 | Best | D1 p16 | |
| F02-W0165 | 27 | Vi Solo Arp-min | OrchS | D1 p17 | |
| F02-W0166 | 27 | Harp-GlissUp-mi1 | OrchB | D1 p18 | |
| F02-W0167 | 27 | H.H.Cl-EXB05 01 | Vint | D1 p18 | |
| F02-W0168 | 27 | BD-EXB09 28 | Synth | D1 p18 | |
| F02-W0169 | 28 | BD-Ringy | ROM | D1 p15 | |
| F02-W0170 | 28 | BD-EXB04 028 | Best | D1 p16 | |
| F02-W0171 | 28 | Vi Solo Arp-maj | OrchS | D1 p17 | |
| F02-W0172 | 28 | Harp-GlissUp-mi2 | OrchB | D1 p18 | |
| F02-W0173 | 28 | H.H.Cl-EXB05 02 | Vint | D1 p18 | |
| F02-W0174 | 28 | BD-EXB09 29 | Synth | D1 p18 | |
| F02-W0175 | 29 | BD-Short88 | ROM | D1 p15 | |
| F02-W0176 | 29 | BD-EXB04 029 | Best | D1 p16 | |
| F02-W0177 | 29 | Cl Solo Arp-dim | OrchS | D1 p17 | |
| F02-W0178 | 29 | Harp-GlissUp-mj1 | OrchB | D1 p18 | |
| F02-W0179 | 29 | H.H.Cl-EXB05 03 | Vint | D1 p18 | |
| F02-W0180 | 29 | BD-EXB09 30 | Synth | D1 p18 | |
| F02-W0181 | 30 | BD-Dance99 | ROM | D1 p15 | |
| F02-W0182 | 30 | BD-EXB04 030 | Best | D1 p16 | |
| F02-W0183 | 30 | Cl Solo Arp-maj | OrchS | D1 p17 | |
| F02-W0184 | 30 | Harp-GlissUp-mj2 | OrchB | D1 p18 | |
| F02-W0185 | 30 | H.H.Cl-EXB05 04 | Vint | D1 p18 | |
| F02-W0186 | 30 | BD-EXB09 31 | Synth | D1 p18 | |
| F02-W0187 | 31 | BD-Ambi | ROM | D1 p15 | |
| F02-W0188 | 31 | BD-EXB04 031 | Best | D1 p16 | |
| F02-W0189 | 31 | Vi SoloChord-dim | OrchS | D1 p17 | |
| F02-W0190 | 31 | Harp-Chord-min 1 | OrchB | D1 p18 | |
| F02-W0191 | 31 | H.H.Cl-EXB05 05 | Vint | D1 p18 | |
| F02-W0192 | 31 | BD-EXB09 32 | Synth | D1 p18 | |
| F02-W0193 | 32 | BD-Ambi Crackle | ROM | D1 p15 | |
| F02-W0194 | 32 | BD-EXB04 032 | Best | D1 p16 | |
| F02-W0195 | 32 | Vi SoloChord-min | OrchS | D1 p17 | |
| F02-W0196 | 32 | Harp-Chord-min 2 | OrchB | D1 p18 | |
| F02-W0197 | 32 | H.H.Cl-EXB05 06 | Vint | D1 p18 | |
| F02-W0198 | 32 | BD-EXB09 33 | Synth | D1 p18 | |
| F02-W0199 | 33 | BD-Ambi Soft | ROM | D1 p15 | |
| F02-W0200 | 33 | BD-EXB04 033 | Best | D1 p16 | |
| F02-W0201 | 33 | Vi SoloChord-maj | OrchS | D1 p17 | |
| F02-W0202 | 33 | Harp-Chord-maj 1 | OrchB | D1 p18 | |
| F02-W0203 | 33 | H.H.Cl-EXB05 07 | Vint | D1 p18 | |
| F02-W0204 | 33 | BD-EXB09 34 | Synth | D1 p18 | |
| F02-W0205 | 34 | BD-Dance Soft | ROM | D1 p15 | |
| F02-W0206 | 34 | BD-EXB04 034 | Best | D1 p16 | |
| F02-W0207 | 34 | Vi Solo Hit-dim | OrchS | D1 p17 | |
| F02-W0208 | 34 | Harp-Chord-maj 2 | OrchB | D1 p18 | |
| F02-W0209 | 34 | H.H.Cl-EXB05 08 | Vint | D1 p18 | |
| F02-W0210 | 34 | BD-EXB09 35 | Synth | D1 p18 | |
| F02-W0211 | 35 | BD-AmbiRocker | ROM | D1 p15 | |
| F02-W0212 | 35 | BD-EXB04 035 | Best | D1 p16 | |
| F02-W0213 | 35 | Vi Solo Hit-min | OrchS | D1 p17 | |
| F02-W0214 | 35 | H.H.Cl-EXB05 09 | Vint | D1 p18 | |
| F02-W0215 | 35 | BD-EXB09 36 | Synth | D1 p18 | |
| F02-W0216 | 36 | BD-Chili | ROM | D1 p15 | |
| F02-W0217 | 36 | BD-EXB04 036 | Best | D1 p16 | |
| F02-W0218 | 36 | Vi Solo Hit-maj | OrchS | D1 p17 | |
| F02-W0219 | 36 | H.H.Ho-EXB05 00 | Vint | D1 p18 | |
| F02-W0220 | 36 | BD-EXB09 37 | Synth | D1 p18 | |
| F02-W0221 | 37 | BD-PhatJaz | ROM | D1 p15 | |
| F02-W0222 | 37 | BD-EXB04 037 | Best | D1 p16 | |
| F02-W0223 | 37 | H.H.Ho-EXB05 01 | Vint | D1 p18 | |
| F02-W0224 | 37 | BD-Jungle-EXB09 | Synth | D1 p18 | |
| F02-W0225 | 38 | BD-Cannon | ROM | D1 p15 | |
| F02-W0226 | 38 | BD-EXB04 038 | Best | D1 p16 | |
| F02-W0227 | 38 | H.H.Ho-EXB05 02 | Vint | D1 p18 | |
| F02-W0228 | 38 | BBD-EXB09 01 | Synth | D1 p18 | |
| F02-W0229 | 39 | BD-Klanger | ROM | D1 p15 | |
| F02-W0230 | 39 | BD-EXB04 039 | Best | D1 p16 | |
| F02-W0231 | 39 | H.H.Ho-EXB05 03 | Vint | D1 p18 | |
| F02-W0232 | 39 | BBD-EXB09 02 | Synth | D1 p18 | |
| F02-W0233 | 40 | SD-Dry 1 | ROM | D1 p15 | |
| F02-W0234 | 40 | BD-EXB04 040 | Best | D1 p16 | |
| F02-W0235 | 40 | Perc Hi-EXB05 01 | Vint | D1 p18 | |
| F02-W0236 | 40 | SD-EXB09 01 | Synth | D1 p18 | |
| F02-W0237 | 41 | SD-Dry 2 | ROM | D1 p15 | |
| F02-W0238 | 41 | BD-EXB04 041 | Best | D1 p16 | |
| F02-W0239 | 41 | Perc MH-EXB05 01 | Vint | D1 p18 | |
| F02-W0240 | 41 | SD-EXB09 02 | Synth | D1 p18 | |
| F02-W0241 | 42 | SD-Dry 3 | ROM | D1 p15 | |
| F02-W0242 | 42 | BD-EXB04 042 | Best | D1 p16 | |
| F02-W0243 | 42 | Perc MH-EXB05 00 | Vint | D1 p18 | |
| F02-W0244 | 42 | SD-EXB09 03 | Synth | D1 p18 | |
| F02-W0245 | 43 | SD-Normal | ROM | D1 p15 | |
| F02-W0246 | 43 | BD-EXB04 043 | Best | D1 p16 | |
| F02-W0247 | 43 | Perc Hi-EXB05 00 | Vint | D1 p18 | |
| F02-W0248 | 43 | SD-EXB09 04 | Synth | D1 p18 | |
| F02-W0249 | 44 | SD-GhostSet roll | ROM | D1 p15 | |
| F02-W0250 | 44 | BD-EXB04 044 | Best | D1 p16 | |
| F02-W0251 | 44 | Perc Hi-EXB05 02 | Vint | D1 p18 | |
| F02-W0252 | 44 | SD-EXB09 05 | Synth | D1 p18 | |
| F02-W0253 | 45 | SD-GhostSet f | ROM | D1 p15 | |
| F02-W0254 | 45 | BD-EXB04 045 | Best | D1 p16 | |
| F02-W0255 | 45 | Perc ML-EXB05 00 | Vint | D1 p18 | |
| F02-W0256 | 45 | SD-EXB09 06 | Synth | D1 p18 | |
| F02-W0257 | 46 | SD-GhostSet p1 | ROM | D1 p15 | |
| F02-W0258 | 46 | BD-EXB04 046 | Best | D1 p16 | |
| F02-W0259 | 46 | Perc Lo-EXB05 00 | Vint | D1 p18 | |
| F02-W0260 | 46 | SD-EXB09 07 | Synth | D1 p18 | |
| F02-W0261 | 47 | SD-GhostSet p2 | ROM | D1 p15 | |
| F02-W0262 | 47 | BD-EXB04 047 | Best | D1 p16 | |
| F02-W0263 | 47 | Perc Lo-EXB05 01 | Vint | D1 p18 | |
| F02-W0264 | 47 | SD-EXB09 08 | Synth | D1 p18 | |
| F02-W0265 | 48 | SD-Full Room | ROM | D1 p15 | |
| F02-W0266 | 48 | BD-EXB04 048 | Best | D1 p16 | |
| F02-W0267 | 48 | Perc BD-EXB05 | Vint | D1 p18 | |
| F02-W0268 | 48 | SD-EXB09 09 | Synth | D1 p18 | |
| F02-W0269 | 49 | SD-Off Center | ROM | D1 p15 | |
| F02-W0270 | 49 | BD-EXB04 049 | Best | D1 p16 | |
| F02-W0271 | 49 | Perc A-EXB05 | Vint | D1 p18 | |
| F02-W0272 | 49 | SD-EXB09 10 | Synth | D1 p18 | |
| F02-W0273 | 50 | SD-Jazz Ring | ROM | D1 p15 | |
| F02-W0274 | 50 | BD-EXB04 050 | Best | D1 p16 | |
| F02-W0275 | 50 | Perc B-EXB05 | Vint | D1 p18 | |
| F02-W0276 | 50 | SD-EXB09 11 | Synth | D1 p18 | |
| F02-W0277 | 51 | SD-Amb.Piccolo 1 | ROM | D1 p15 | |
| F02-W0278 | 51 | BD-EXB04 051 | Best | D1 p16 | |
| F02-W0279 | 51 | Perc C-EXB05 | Vint | D1 p18 | |
| F02-W0280 | 51 | SD-EXB09 12 | Synth | D1 p18 | |
| F02-W0281 | 52 | SD-Amb.Piccolo 2 | ROM | D1 p15 | |
| F02-W0282 | 52 | BD-EXB04 052 | Best | D1 p16 | |
| F02-W0283 | 52 | Perc D-EXB05 | Vint | D1 p18 | |
| F02-W0284 | 52 | SD-EXB09 13 | Synth | D1 p18 | |
| F02-W0285 | 53 | SD-Paper | ROM | D1 p15 | |
| F02-W0286 | 53 | BD-EXB04 053 | Best | D1 p16 | |
| F02-W0287 | 53 | Metal-EXB05 00 | Vint | D1 p18 | |
| F02-W0288 | 53 | SD-EXB09 14 | Synth | D1 p18 | |
| F02-W0289 | 54 | SD-Classic Room | ROM | D1 p15 | |
| F02-W0290 | 54 | BD-EXB04 054 | Best | D1 p16 | |
| F02-W0291 | 54 | Metal-EXB05 01 | Vint | D1 p18 | |
| F02-W0292 | 54 | SD-EXB09 15 | Synth | D1 p18 | |
| F02-W0293 | 55 | SD-Brush Hit | ROM | D1 p15 | |
| F02-W0294 | 55 | BD-EXB04 055 | Best | D1 p16 | |
| F02-W0295 | 55 | Clave-EXB05 00 | Vint | D1 p18 | |
| F02-W0296 | 55 | SD-EXB09 16 | Synth | D1 p18 | |
| F02-W0297 | 56 | SD-Brush Tap 1 | ROM | D1 p15 | |
| F02-W0298 | 56 | BD-EXB04 056 | Best | D1 p16 | |
| F02-W0299 | 56 | SD-Jungle-EXB09 | Synth | D1 p18 | |
| F02-W0300 | 57 | SD-Brush Tap 2 | ROM | D1 p15 | |
| F02-W0301 | 57 | BD-EXB04 057 | Best | D1 p16 | |
| F02-W0302 | 57 | Clap-EXB09 01 | Synth | D1 p18 | |
| F02-W0303 | 58 | SD-Brush Swirl | ROM | D1 p15 | |
| F02-W0304 | 58 | BD-EXB04 058 | Best | D1 p16 | |
| F02-W0305 | 58 | Clap-EXB09 02 | Synth | D1 p18 | |
| F02-W0306 | 59 | SD-Atomic | ROM | D1 p15 | |
| F02-W0307 | 59 | BD-EXB04 059 | Best | D1 p16 | |
| F02-W0308 | 59 | Clap-EXB09 03 | Synth | D1 p18 | |
| F02-W0309 | 60 | SD-Big Rock | ROM | D1 p15 | |
| F02-W0310 | 60 | BD-EXB04 060 | Best | D1 p16 | |
| F02-W0311 | 60 | Clap-EXB09 04 | Synth | D1 p18 | |
| F02-W0312 | 61 | SD-Yowie | ROM | D1 p15 | |
| F02-W0313 | 61 | BD-EXB04 061 | Best | D1 p16 | |
| F02-W0314 | 61 | Clap-EXB09 05 | Synth | D1 p18 | |
| F02-W0315 | 62 | SD-Processed 1 | ROM | D1 p15 | |
| F02-W0316 | 62 | BD-EXB04 062 | Best | D1 p16 | |
| F02-W0317 | 62 | Clap-EXB09 06 | Synth | D1 p18 | |
| F02-W0318 | 63 | SD-Processed 2 | ROM | D1 p15 | |
| F02-W0319 | 63 | BD-EXB04 063 | Best | D1 p16 | |
| F02-W0320 | 63 | Clap-EXB09 07 | Synth | D1 p18 | |
| F02-W0321 | 64 | SD-Cracker Room | ROM | D1 p15 | |
| F02-W0322 | 64 | BD-EXB04 064 | Best | D1 p16 | |
| F02-W0323 | 64 | HH01-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0324 | 65 | Sidestick-Dry | ROM | D1 p15 | |
| F02-W0325 | 65 | BD-EXB04 065 | Best | D1 p17 | |
| F02-W0326 | 65 | HH01-Ho-EXB09 | Synth | D1 p18 | |
| F02-W0327 | 66 | Sidestick-Amb | ROM | D1 p15 | |
| F02-W0328 | 66 | BD-EXB04 066 | Best | D1 p17 | |
| F02-W0329 | 66 | HH01-Op-EXB09 01 | Synth | D1 p18 | |
| F02-W0330 | 67 | SD-Amb.House | ROM | D1 p15 | |
| F02-W0331 | 67 | BD-EXB04 067 | Best | D1 p17 | |
| F02-W0332 | 67 | HH01-Op-EXB09 02 | Synth | D1 p18 | |
| F02-W0333 | 68 | SD-Dance | ROM | D1 p15 | |
| F02-W0334 | 68 | BD-EXB04 068 | Best | D1 p17 | |
| F02-W0335 | 68 | HH02-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0336 | 69 | SD-Rap 1 | ROM | D1 p15 | |
| F02-W0337 | 69 | BD-EXB04 069 | Best | D1 p17 | |
| F02-W0338 | 69 | HH02-Ho-EXB09 | Synth | D1 p18 | |
| F02-W0339 | 70 | SD-Rap 2 | ROM | D1 p15 | |
| F02-W0340 | 70 | BD-EXB04 070 | Best | D1 p17 | |
| F02-W0341 | 70 | HH02-Op-EXB09 01 | Synth | D1 p18 | |
| F02-W0342 | 71 | SD-Noise | ROM | D1 p15 | |
| F02-W0343 | 71 | BD-EXB04 071 | Best | D1 p17 | |
| F02-W0344 | 71 | HH02-Op-EXB09 02 | Synth | D1 p18 | |
| F02-W0345 | 72 | SD-Sizzle | ROM | D1 p15 | |
| F02-W0346 | 72 | BD-EXB04 072 | Best | D1 p17 | |
| F02-W0347 | 72 | HH03-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0348 | 73 | SD-Reverse | ROM | D1 p15 | |
| F02-W0349 | 73 | BD-EXB04 073 | Best | D1 p17 | |
| F02-W0350 | 73 | HH03-Op-EXB09 | Synth | D1 p18 | |
| F02-W0351 | 74 | SD-Hip 1 | ROM | D1 p15 | |
| F02-W0352 | 74 | BD-EXB04 074 | Best | D1 p17 | |
| F02-W0353 | 74 | HH04-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0354 | 75 | SD-Hip 2 | ROM | D1 p15 | |
| F02-W0355 | 75 | BD-EXB04 075 | Best | D1 p17 | |
| F02-W0356 | 75 | HH04-Ho-EXB09 01 | Synth | D1 p18 | |
| F02-W0357 | 76 | SD-Hip 3 | ROM | D1 p15 | |
| F02-W0358 | 76 | BD-EXB04 076 | Best | D1 p17 | |
| F02-W0359 | 76 | HH04-Ho-EXB09 02 | Synth | D1 p18 | |
| F02-W0360 | 77 | SD-Hip 4 | ROM | D1 p15 | |
| F02-W0361 | 77 | BD-EXB04 077 | Best | D1 p17 | |
| F02-W0362 | 77 | HH04-Op-EXB09 | Synth | D1 p18 | |
| F02-W0363 | 78 | SD-Hip 5 | ROM | D1 p15 | |
| F02-W0364 | 78 | BD-EXB04 078 | Best | D1 p17 | |
| F02-W0365 | 78 | HH05-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0366 | 79 | SD-Hip 6 | ROM | D1 p15 | |
| F02-W0367 | 79 | BD-EXB04 079 | Best | D1 p17 | |
| F02-W0368 | 79 | HH05-Ho-EXB09 | Synth | D1 p18 | |
| F02-W0369 | 80 | SD-Ringy | ROM | D1 p15 | |
| F02-W0370 | 80 | BD-EXB04 080 | Best | D1 p17 | |
| F02-W0371 | 80 | HH05-Op-EXB09 | Synth | D1 p18 | |
| F02-W0372 | 81 | SD-Tiny | ROM | D1 p15 | |
| F02-W0373 | 81 | BD-EXB04 082 | Best | D1 p17 | |
| F02-W0374 | 81 | HH06-Ho-EXB09 | Synth | D1 p18 | |
| F02-W0375 | 82 | SD-Vintage 1 | ROM | D1 p15 | |
| F02-W0376 | 82 | BD-EXB04 086 | Best | D1 p17 | |
| F02-W0377 | 82 | HH06-Op-EXB09 | Synth | D1 p18 | |
| F02-W0378 | 83 | SD-Vintage 2 | ROM | D1 p15 | |
| F02-W0379 | 83 | BD-EXB04 087 | Best | D1 p17 | |
| F02-W0380 | 83 | Ride-EXB09 01 | Synth | D1 p18 | |
| F02-W0381 | 84 | SD-Vintage 3 | ROM | D1 p15 | |
| F02-W0382 | 84 | BD-EXB04 088 | Best | D1 p17 | |
| F02-W0383 | 84 | Ride-EXB09 02 | Synth | D1 p18 | |
| F02-W0384 | 85 | SD-Vintage 4 | ROM | D1 p15 | |
| F02-W0385 | 85 | BD-EXB04 089 | Best | D1 p17 | |
| F02-W0386 | 85 | Crash-EXB09 01 | Synth | D1 p18 | |
| F02-W0387 | 86 | SD-Vintage 5 | ROM | D1 p15 | |
| F02-W0388 | 86 | BD-EXB04 090 | Best | D1 p17 | |
| F02-W0389 | 86 | Crash-EXB09 02 | Synth | D1 p18 | |
| F02-W0390 | 87 | SD-Vintage 6 | ROM | D1 p15 | |
| F02-W0391 | 87 | BD-EXB04 091 | Best | D1 p17 | |
| F02-W0392 | 87 | Crash-EXB09 03 | Synth | D1 p18 | |
| F02-W0393 | 88 | SD-AmbiCrackle 1 | ROM | D1 p15 | |
| F02-W0394 | 88 | BD-EXB04 092 | Best | D1 p17 | |
| F02-W0395 | 88 | CrashJungle-EXB9 | Synth | D1 p18 | |
| F02-W0396 | 89 | SD-AmbiCrackle 2 | ROM | D1 p15 | |
| F02-W0397 | 89 | BD-EXB04 093 | Best | D1 p17 | |
| F02-W0398 | 89 | HH-DDD1-EXB09 | Synth | D1 p18 | |
| F02-W0399 | 90 | SD-AmbiCrackle 3 | ROM | D1 p15 | |
| F02-W0400 | 90 | BD-EXB04 094 | Best | D1 p17 | |
| F02-W0401 | 90 | Cym-DDD1-L-EXB09 | Synth | D1 p18 | |
| F02-W0402 | 91 | SD-AmbiHop | ROM | D1 p15 | |
| F02-W0403 | 91 | BD-EXB04 095 | Best | D1 p17 | |
| F02-W0404 | 91 | Cym-DDD1-M-EXB09 | Synth | D1 p18 | |
| F02-W0405 | 92 | SD-Brasser | ROM | D1 p15 | |
| F02-W0406 | 92 | BD-EXB04 096 | Best | D1 p17 | |
| F02-W0407 | 92 | Cym-DDD1-H-EXB09 | Synth | D1 p18 | |
| F02-W0408 | 93 | SD-Chili | ROM | D1 p15 | |
| F02-W0409 | 93 | BD-EXB04 097 | Best | D1 p17 | |
| F02-W0410 | 93 | Perc01-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0411 | 94 | SD-Klanger | ROM | D1 p15 | |
| F02-W0412 | 94 | BD-EXB04 098 | Best | D1 p17 | |
| F02-W0413 | 94 | Perc01-Op-EXB09 | Synth | D1 p18 | |
| F02-W0414 | 95 | SD-Whopper | ROM | D1 p15 | |
| F02-W0415 | 95 | BD-EXB04 099 | Best | D1 p17 | |
| F02-W0416 | 95 | Perc02-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0417 | 96 | SD-Dry Present | ROM | D1 p15 | |
| F02-W0418 | 96 | BD-EXB04 100 | Best | D1 p17 | |
| F02-W0419 | 96 | Perc02-Op-EXB09 | Synth | D1 p18 | |
| F02-W0420 | 97 | SD-Ambi Pop | ROM | D1 p15 | |
| F02-W0421 | 97 | BD-EXB04 101 | Best | D1 p17 | |
| F02-W0422 | 97 | Perc02-Rev-EXB09 | Synth | D1 p18 | |
| F02-W0423 | 98 | SD-Ambi Hit 1 | ROM | D1 p15 | |
| F02-W0424 | 98 | BD-EXB04 102 | Best | D1 p17 | |
| F02-W0425 | 98 | Perc03-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0426 | 99 | SD-Ambi Hit 2 | ROM | D1 p15 | |
| F02-W0427 | 99 | BD-EXB04 103 | Best | D1 p17 | |
| F02-W0428 | 99 | Perc03-Op-EXB09 | Synth | D1 p18 | |
| F02-W0429 | 100 | SynComp-Poly6_2 | Vint | D1 p15 | |
| F02-W0430 | 100 | SD-Ambi Hit 3 | ROM | D1 p15 | |
| F02-W0431 | 100 | BD-EXB04 104 | Best | D1 p17 | |
| F02-W0432 | 100 | Perc04-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0433 | 101 | Noisy Syn-Pro5 | Vint | D1 p15 | |
| F02-W0434 | 101 | Tom 1-Hi | ROM | D1 p15 | |
| F02-W0435 | 101 | SD-EXB04 00 | Best | D1 p17 | |
| F02-W0436 | 101 | Perc04-Op-EXB09 | Synth | D1 p18 | |
| F02-W0437 | 102 | Noisy Syn-P5 Rev | Vint | D1 p15 | |
| F02-W0438 | 102 | Tom 1-Lo | ROM | D1 p15 | |
| F02-W0439 | 102 | SD-EXB04 01 | Best | D1 p17 | |
| F02-W0440 | 102 | Perc-Metal-EXB09 | Synth | D1 p18 | |
| F02-W0441 | 103 | Oct Syn-Pro5 | Vint | D1 p15 | |
| F02-W0442 | 103 | Tom 1-Floor | ROM | D1 p15 | |
| F02-W0443 | 103 | SD-EXB04 02 | Best | D1 p17 | |
| F02-W0444 | 103 | Perc-Ambi-EXB09 | Synth | D1 p18 | |
| F02-W0445 | 104 | Oct Syn-P5 Rev | Vint | D1 p15 | |
| F02-W0446 | 104 | Tom 2-Hi | ROM | D1 p15 | |
| F02-W0447 | 104 | SD-EXB04 03 | Best | D1 p17 | |
| F02-W0448 | 104 | Perc05-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0449 | 105 | 5th Syn-Pro5 | Vint | D1 p15 | |
| F02-W0450 | 105 | Tom 2-Lo | ROM | D1 p15 | |
| F02-W0451 | 105 | SD-EXB04 04 | Best | D1 p17 | |
| F02-W0452 | 105 | Perc05-Op-EXB09 | Synth | D1 p18 | |
| F02-W0453 | 106 | 5th Syn-P5 Rev | Vint | D1 p15 | |
| F02-W0454 | 106 | Tom 2-Floor | ROM | D1 p15 | |
| F02-W0455 | 106 | SD-EXB04 05 | Best | D1 p17 | |
| F02-W0456 | 106 | Perc06-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0457 | 107 | Sync-MonoPoly | Vint | D1 p15 | |
| F02-W0458 | 107 | Tom 3-Hi | ROM | D1 p15 | |
| F02-W0459 | 107 | SD-EXB04 06 | Best | D1 p17 | |
| F02-W0460 | 107 | Perc06-Op-EXB09 | Synth | D1 p18 | |
| F02-W0461 | 108 | Sync-MonoPoly LP | Vint | D1 p15 | |
| F02-W0462 | 108 | Tom 3-Lo | ROM | D1 p15 | |
| F02-W0463 | 108 | SD-EXB04 07 | Best | D1 p17 | |
| F02-W0464 | 108 | Perc07-Cl-EXB09 | Synth | D1 p18 | |
| F02-W0465 | 109 | Sync-MonoPolyRev | Vint | D1 p15 | |
| F02-W0466 | 109 | Tom 3-Floor | ROM | D1 p15 | |
| F02-W0467 | 109 | SD-EXB04 08 | Best | D1 p17 | |
| F02-W0468 | 109 | Perc07-Op-EXB09 | Synth | D1 p18 | |
| F02-W0469 | 110 | Sync-MKS 1 | Vint | D1 p15 | |
| F02-W0470 | 110 | Tom-Processed | ROM | D1 p15 | |
| F02-W0471 | 110 | SD-EXB04 09 | Best | D1 p17 | |
| F02-W0472 | 110 | Perc-Tom-EXB09 1 | Synth | D1 p18 | |
| F02-W0473 | 111 | Sync-MKS 2 | Vint | D1 p15 | |
| F02-W0474 | 111 | Tom-Jazz Hi | ROM | D1 p15 | |
| F02-W0475 | 111 | SD-EXB04 10 | Best | D1 p17 | |
| F02-W0476 | 111 | Perc-Tom-EXB09 2 | Synth | D1 p18 | |
| F02-W0477 | 112 | SynBrass-MG 1 | Vint | D1 p15 | |
| F02-W0478 | 112 | Tom-Jazz Hi Rim | ROM | D1 p15 | |
| F02-W0479 | 112 | SD-EXB04 11 | Best | D1 p17 | |
| F02-W0480 | 112 | Perc-Tom-EXB09 3 | Synth | D1 p18 | |
| F02-W0481 | 113 | SynBrass-MG 1 LP | Vint | D1 p15 | |
| F02-W0482 | 113 | Tom-Jazz Floor | ROM | D1 p15 | |
| F02-W0483 | 113 | SD-EXB04 12 | Best | D1 p17 | |
| F02-W0484 | 113 | Perc-Tom-EXB09 4 | Synth | D1 p18 | |
| F02-W0485 | 114 | SynBrass-MG1 Rev | Vint | D1 p15 | |
| F02-W0486 | 114 | Tom-Brush Hi | ROM | D1 p15 | |
| F02-W0487 | 114 | SD-EXB04 13 | Best | D1 p17 | |
| F02-W0488 | 114 | Perc-Rim-EXB09 1 | Synth | D1 p18 | |
| F02-W0489 | 115 | SynBrass-MG 2 | Vint | D1 p15 | |
| F02-W0490 | 115 | Tom-Brush Floor | ROM | D1 p15 | |
| F02-W0491 | 115 | SD-EXB04 14 | Best | D1 p17 | |
| F02-W0492 | 115 | Perc-Rim-EXB09 2 | Synth | D1 p18 | |
| F02-W0493 | 116 | SynBrass-Pro5_1 | Vint | D1 p15 | |
| F02-W0494 | 116 | Tom-Dirty Funk | ROM | D1 p15 | |
| F02-W0495 | 116 | SD-EXB04 15 | Best | D1 p17 | |
| F02-W0496 | 116 | Perc-Mute-EXB09 | Synth | D1 p18 | |
| F02-W0497 | 117 | SynBrass-P5_1 LP | Vint | D1 p15 | |
| F02-W0498 | 117 | Tom-Old R&B | ROM | D1 p15 | |
| F02-W0499 | 117 | SD-EXB04 16 | Best | D1 p17 | |
| F02-W0500 | 117 | Perc-DDD-EXB09 1 | Synth | D1 p18 | |
| F02-W0501 | 118 | SynBrass-P5_1Rev | Vint | D1 p15 | |
| F02-W0502 | 118 | H.Hat 1-Closed 1 | ROM | D1 p15 | |
| F02-W0503 | 118 | SD-EXB04 17 | Best | D1 p17 | |
| F02-W0504 | 118 | Perc-DDD-EXB09 2 | Synth | D1 p18 | |
| F02-W0505 | 119 | SynBrass-Pro5_2 | Vint | D1 p15 | |
| F02-W0506 | 119 | H.Hat 1-Closed 2 | ROM | D1 p15 | |
| F02-W0507 | 119 | SD-EXB04 18 | Best | D1 p17 | |
| F02-W0508 | 119 | Perc-DDD-EXB09 3 | Synth | D1 p18 | |
| F02-W0509 | 120 | SynEns-MG 1 | Vint | D1 p15 | |
| F02-W0510 | 120 | H.Hat 1-Foot | ROM | D1 p15 | |
| F02-W0511 | 120 | SD-EXB04 19 | Best | D1 p17 | |
| F02-W0512 | 120 | Perc-DDD-EXB09 4 | Synth | D1 p18 | |
| F02-W0513 | 121 | SynEns-MG 1 Rev | Vint | D1 p15 | |
| F02-W0514 | 121 | H.Hat 1-FootOpen | ROM | D1 p15 | |
| F02-W0515 | 121 | SD-EXB04 20 | Best | D1 p17 | |
| F02-W0516 | 121 | Perc-Junk-EXB9 1 | Synth | D1 p18 | |
| F02-W0517 | 122 | SynEns-MG 2 | Vint | D1 p15 | |
| F02-W0518 | 122 | H.Hat 1-Open 1 | ROM | D1 p15 | |
| F02-W0519 | 122 | SD-EXB04 21 | Best | D1 p17 | |
| F02-W0520 | 122 | Perc-Junk-EXB9 2 | Synth | D1 p18 | |
| F02-W0521 | 123 | SynEns-MG 2 Rev | Vint | D1 p15 | |
| F02-W0522 | 123 | H.Hat 1-Open 2 | ROM | D1 p15 | |
| F02-W0523 | 123 | SD-EXB04 22 | Best | D1 p17 | |
| F02-W0524 | 123 | Perc-Junk-EXB9 3 | Synth | D1 p18 | |
| F02-W0525 | 124 | SynEns-A2600 | Vint | D1 p15 | |
| F02-W0526 | 124 | H.Hat 1-Sizzle | ROM | D1 p15 | |
| F02-W0527 | 124 | SD-EXB04 23 | Best | D1 p17 | |
| F02-W0528 | 124 | Perc-Junk-EXB9 4 | Synth | D1 p18 | |
| F02-W0529 | 125 | SynEns-A2600 Rev | Vint | D1 p15 | |
| F02-W0530 | 125 | H.Hat 2-Closed 1 | ROM | D1 p15 | |
| F02-W0531 | 125 | SD-EXB04 24 | Best | D1 p17 | |
| F02-W0532 | 125 | Perc-Junk-EXB9 5 | Synth | D1 p18 | |
| F02-W0533 | 126 | SynEns-MX12_1 | Vint | D1 p15 | |
| F02-W0534 | 126 | H.Hat 2-Closed 2 | ROM | D1 p15 | |
| F02-W0535 | 126 | SD-EXB04 25 | Best | D1 p17 | |
| F02-W0536 | 126 | Perc-Junk-EXB9 6 | Synth | D1 p18 | |
| F02-W0537 | 127 | SynEns-MX12_1Rev | Vint | D1 p15 | |
| F02-W0538 | 127 | H.Hat 2-Foot | ROM | D1 p15 | |
| F02-W0539 | 127 | SD-EXB04 26 | Best | D1 p17 | |
| F02-W0540 | 127 | Perc-Junk-EXB9 7 | Synth | D1 p18 | |
| F02-W0541 | 128 | SynEns-MX12_2 | Vint | D1 p15 | |
| F02-W0542 | 128 | H.Hat 2-FootOpen | ROM | D1 p15 | |
| F02-W0543 | 128 | SD-EXB04 27 | Best | D1 p17 | |
| F02-W0544 | 128 | Perc-Junk-EXB9 8 | Synth | D1 p18 | |
| F02-W0545 | 129 | SynEns-MX12_2Rev | Vint | D1 p15 | |
| F02-W0546 | 129 | H.Hat 2-Open 1 | ROM | D1 p15 | |
| F02-W0547 | 129 | SD-EXB04 28 | Best | D1 p17 | |
| F02-W0548 | 129 | Perc-Junk-EXB9 9 | Synth | D1 p18 | |
| F02-W0549 | 130 | SynEns-OB | Vint | D1 p15 | |
| F02-W0550 | 130 | H.Hat 2-Open 2 | ROM | D1 p16 | |
| F02-W0551 | 130 | SD-EXB04 29 | Best | D1 p17 | |
| F02-W0552 | 130 | Woodblock-L-EXB9 | Synth | D1 p18 | |
| F02-W0553 | 131 | SynEns-OB Rev | Vint | D1 p15 | |
| F02-W0554 | 131 | H.Hat 2-Sizzle | ROM | D1 p16 | |
| F02-W0555 | 131 | SD-EXB04 30 | Best | D1 p17 | |
| F02-W0556 | 131 | Woodblock-H-EXB9 | Synth | D1 p18 | |
| F02-W0557 | 132 | StrEns-Slina | Vint | D1 p15 | |
| F02-W0558 | 132 | H.Hat-Chili | ROM | D1 p16 | |
| F02-W0559 | 132 | SD-EXB04 31 | Best | D1 p17 | |
| F02-W0560 | 132 | Conga01-L-EXB09 | Synth | D1 p18 | |
| F02-W0561 | 133 | StrEns-Slina Rev | Vint | D1 p15 | |
| F02-W0562 | 133 | H.Hat-Crisp Cl 1 | ROM | D1 p16 | |
| F02-W0563 | 133 | SD-EXB04 32 | Best | D1 p17 | |
| F02-W0564 | 133 | Conga01-H-EXB09 | Synth | D1 p18 | |
| F02-W0565 | 134 | StrEns-Delta | Vint | D1 p15 | |
| F02-W0566 | 134 | H.Hat-Crisp Cl 2 | ROM | D1 p16 | |
| F02-W0567 | 134 | SD-EXB04 33 | Best | D1 p17 | |
| F02-W0568 | 134 | Conga01-Hm-EXB09 | Synth | D1 p18 | |
| F02-W0569 | 135 | StrEns-Delta Rev | Vint | D1 p15 | |
| F02-W0570 | 135 | H.Hat-Crisp Open | ROM | D1 p16 | |
| F02-W0571 | 135 | SD-EXB04 34 | Best | D1 p17 | |
| F02-W0572 | 135 | Conga02-Ho-EX9 1 | Synth | D1 p18 | |
| F02-W0573 | 136 | SynEns-MKS | Vint | D1 p15 | |
| F02-W0574 | 136 | H.Hat-Grange | ROM | D1 p16 | |
| F02-W0575 | 136 | SD-EXB04 35 | Best | D1 p17 | |
| F02-W0576 | 136 | Conga02-Ho-EX9 2 | Synth | D1 p18 | |
| F02-W0577 | 137 | SynEns-MKS Rev | Vint | D1 p15 | |
| F02-W0578 | 137 | H.Hat-Vintage 1 | ROM | D1 p16 | |
| F02-W0579 | 137 | SD-EXB04 36 | Best | D1 p17 | |
| F02-W0580 | 137 | Conga02-Hs-EX9 1 | Synth | D1 p18 | |
| F02-W0581 | 138 | Vocoder-VP | Vint | D1 p15 | |
| F02-W0582 | 138 | H.Hat-Vintage 2 | ROM | D1 p16 | |
| F02-W0583 | 138 | SD-EXB04 37 | Best | D1 p17 | |
| F02-W0584 | 138 | Conga02-Hs-EX9 2 | Synth | D1 p18 | |
| F02-W0585 | 139 | Vocoder-VP Rev | Vint | D1 p15 | |
| F02-W0586 | 139 | H.Hat-Tight 1 | ROM | D1 p16 | |
| F02-W0587 | 139 | SD-EXB04 38 | Best | D1 p17 | |
| F02-W0588 | 139 | Sidestick-EXB09 | Synth | D1 p18 | |
| F02-W0589 | 140 | SynVoice-VS | Vint | D1 p15 | |
| F02-W0590 | 140 | H.Hat-Tight 2 | ROM | D1 p16 | |
| F02-W0591 | 140 | SD-EXB04 39 | Best | D1 p17 | |
| F02-W0592 | 140 | Tombourine-EXB09 | Synth | D1 p18 | |
| F02-W0593 | 141 | SynVoice-VS Rev | Vint | D1 p15 | |
| F02-W0594 | 141 | H.Hat-Old Cl 1 | ROM | D1 p16 | |
| F02-W0595 | 141 | SD-EXB04 40 | Best | D1 p17 | |
| F02-W0596 | 141 | Camera-EXB09 | Synth | D1 p18 | |
| F02-W0597 | 142 | Tape Choir | Vint | D1 p15 | |
| F02-W0598 | 142 | H.Hat-Old Cl 2 | ROM | D1 p16 | |
| F02-W0599 | 142 | SD-EXB04 41 | Best | D1 p17 | |
| F02-W0600 | 142 | Car Horn-EXB09 | Synth | D1 p18 | |
| F02-W0601 | 143 | Tape Choir Rev | Vint | D1 p15 | |
| F02-W0602 | 143 | H.Hat-Old Cl 3 | ROM | D1 p16 | |
| F02-W0603 | 143 | SD-EXB04 42 | Best | D1 p17 | |
| F02-W0604 | 143 | Soda-EXB09 | Synth | D1 p18 | |
| F02-W0605 | 144 | Tape Strings | Vint | D1 p15 | |
| F02-W0606 | 144 | H.Hat-Old Open | ROM | D1 p16 | |
| F02-W0607 | 144 | SD-EXB04 43 | Best | D1 p17 | |
| F02-W0608 | 144 | Glass-EXB09 | Synth | D1 p18 | |
| F02-W0609 | 145 | Tape Strings Rev | Vint | D1 p15 | |
| F02-W0610 | 145 | H.Hat-Hip 1 | ROM | D1 p16 | |
| F02-W0611 | 145 | SD-EXB04 44 | Best | D1 p17 | |
| F02-W0612 | 145 | Revers-EXB09 | Synth | D1 p18 | |
| F02-W0613 | 146 | Tape Flute | Vint | D1 p15 | |
| F02-W0614 | 146 | H.Hat-Hip 2 | ROM | D1 p16 | |
| F02-W0615 | 146 | SD-EXB04 45 | Best | D1 p17 | |
| F02-W0616 | 146 | Vo-09 Hey M | Synth | D1 p18 | |
| F02-W0617 | 147 | Tape Flute Rev | Vint | D1 p15 | |
| F02-W0618 | 147 | H.Hat-AmbCrackle | ROM | D1 p16 | |
| F02-W0619 | 147 | SD-EXB04 46 | Best | D1 p17 | |
| F02-W0620 | 147 | Vo-09 Wild | Synth | D1 p18 | |
| F02-W0621 | 148 | Tape Brass | Vint | D1 p15 | |
| F02-W0622 | 148 | H.Hat-Ambi | ROM | D1 p16 | |
| F02-W0623 | 148 | SD-EXB04 47 | Best | D1 p17 | |
| F02-W0624 | 148 | Vo-09 Love | Synth | D1 p18 | |
| F02-W0625 | 149 | Tape Brass Rev | Vint | D1 p15 | |
| F02-W0626 | 149 | H.Hat-ClangyOpen | ROM | D1 p16 | |
| F02-W0627 | 149 | SD-EXB04 48 | Best | D1 p17 | |
| F02-W0628 | 149 | Vo-09 Ha | Synth | D1 p18 | |
| F02-W0629 | 150 | Syn Organ-MG | Vint | D1 p15 | |
| F02-W0630 | 150 | H.Hat-Whispy | ROM | D1 p16 | |
| F02-W0631 | 150 | SD-EXB04 49 | Best | D1 p17 | |
| F02-W0632 | 150 | Vo-09 Ready | Synth | D1 p18 | |
| F02-W0633 | 151 | Syn Organ-VS 1 | Vint | D1 p15 | |
| F02-W0634 | 151 | H.Hat-Alpo Close | ROM | D1 p16 | |
| F02-W0635 | 151 | SD-EXB04 50 | Best | D1 p17 | |
| F02-W0636 | 151 | Vo-09 Funky 1 | Synth | D1 p18 | |
| F02-W0637 | 152 | Syn Organ-VS1Rev | Vint | D1 p15 | |
| F02-W0638 | 152 | Crash Cymbal 1 | ROM | D1 p16 | |
| F02-W0639 | 152 | SD-EXB04 51 | Best | D1 p17 | |
| F02-W0640 | 152 | Vo-09 Funky 2 | Synth | D1 p18 | |
| F02-W0641 | 153 | Syn Organ-VS 2 | Vint | D1 p15 | |
| F02-W0642 | 153 | Crash Cymbal 2 | ROM | D1 p16 | |
| F02-W0643 | 153 | SD-EXB04 52 | Best | D1 p17 | |
| F02-W0644 | 153 | Vo-09 KeepRockin | Synth | D1 p18 | |
| F02-W0645 | 154 | Syn Organ-VS 3 | Vint | D1 p15 | |
| F02-W0646 | 154 | Crash Cymbal 3 | ROM | D1 p16 | |
| F02-W0647 | 154 | SD-EXB04 53 | Best | D1 p17 | |
| F02-W0648 | 154 | Vo-09 Break itUp | Synth | D1 p18 | |
| F02-W0649 | 155 | OSC Sync-Pro5_1 | Vint | D1 p15 | |
| F02-W0650 | 155 | China Cymbal | ROM | D1 p16 | |
| F02-W0651 | 155 | SD-EXB04 54 | Best | D1 p17 | |
| F02-W0652 | 155 | Vo-09 Bass | Synth | D1 p18 | |
| F02-W0653 | 156 | OSC Sync-Pro5_2 | Vint | D1 p15 | |
| F02-W0654 | 156 | Splash Cymbal | ROM | D1 p16 | |
| F02-W0655 | 156 | SD-EXB04 55 | Best | D1 p17 | |
| F02-W0656 | 156 | Vo-09 Yeah M1 | Synth | D1 p18 | |
| F02-W0657 | 157 | SynArpg-Pro5 | Vint | D1 p15 | |
| F02-W0658 | 157 | Cymbal Reverse | ROM | D1 p16 | |
| F02-W0659 | 157 | SD-EXB04 56 | Best | D1 p17 | |
| F02-W0660 | 157 | Vo-09 Yeah M2 | Synth | D1 p18 | |
| F02-W0661 | 158 | Crash-Dance 99 | ROM | D1 p16 | |
| F02-W0662 | 158 | SD-EXB04 57 | Best | D1 p17 | |
| F02-W0663 | 158 | Vo-09 Bang | Synth | D1 p18 | |
| F02-W0664 | 159 | Crash-Dance DDD1 | ROM | D1 p16 | |
| F02-W0665 | 159 | SD-EXB04 58 | Best | D1 p17 | |
| F02-W0666 | 159 | Vo-09 Let's Go | Synth | D1 p18 | |
| F02-W0667 | 160 | Ride-Dance 99 | ROM | D1 p16 | |
| F02-W0668 | 160 | SD-EXB04 59 | Best | D1 p17 | |
| F02-W0669 | 160 | Vo-09 Ahhhh | Synth | D1 p18 | |
| F02-W0670 | 161 | Ride-Dance DDD1 | ROM | D1 p16 | |
| F02-W0671 | 161 | SD-EXB04 60 | Best | D1 p17 | |
| F02-W0672 | 161 | Vo-09 Ohhhh | Synth | D1 p18 | |
| F02-W0673 | 162 | Ride-Edge 1 | ROM | D1 p16 | |
| F02-W0674 | 162 | SD-EXB04 61 | Best | D1 p17 | |
| F02-W0675 | 162 | Vo-09 Ooooh | Synth | D1 p18 | |
| F02-W0676 | 163 | Ride-Edge 2 | ROM | D1 p16 | |
| F02-W0677 | 163 | SD-EXB04 62 | Best | D1 p17 | |
| F02-W0678 | 163 | Vo-09 Aaaah | Synth | D1 p18 | |
| F02-W0679 | 164 | Ride-Jazz | ROM | D1 p16 | |
| F02-W0680 | 164 | SD-EXB04 63 | Best | D1 p17 | |
| F02-W0681 | 164 | Vo-09 Love | Synth | D1 p18 | |
| F02-W0682 | 165 | Ride-Cup | ROM | D1 p16 | |
| F02-W0683 | 165 | SD-EXB04 64 | Best | D1 p17 | |
| F02-W0684 | 165 | Vo-09 Kkkkk | Synth | D1 p18 | |
| F02-W0685 | 166 | BD-Orchestral | ROM | D1 p16 | |
| F02-W0686 | 166 | SD-EXB04 65 | Best | D1 p17 | |
| F02-W0687 | 166 | Vo-09 Kaaaa | Synth | D1 p18 | |
| F02-W0688 | 167 | Orch Cym.-Closed | ROM | D1 p16 | |
| F02-W0689 | 167 | SD-EXB04 66 | Best | D1 p17 | |
| F02-W0690 | 167 | Vo-09 Baaaa | Synth | D1 p18 | |
| F02-W0691 | 168 | Orch Cym.-Open | ROM | D1 p16 | |
| F02-W0692 | 168 | SD-EXB04 67 | Best | D1 p17 | |
| F02-W0693 | 168 | Vo-09 Hrrrr | Synth | D1 p18 | |
| F02-W0694 | 169 | Timpani | ROM | D1 p16 | |
| F02-W0695 | 169 | SD-EXB04 68 | Best | D1 p17 | |
| F02-W0696 | 169 | Vo-09 Click | Synth | D1 p18 | |
| F02-W0697 | 170 | SD-Orch.Roll 1 | ROM | D1 p16 | |
| F02-W0698 | 170 | SD-EXB04 69 | Best | D1 p17 | |
| F02-W0699 | 170 | Vo-09 Trrrr | Synth | D1 p18 | |
| F02-W0700 | 171 | SD-Orch.Roll 2 | ROM | D1 p16 | |
| F02-W0701 | 171 | H.H.Cl-EXB04 00 | Best | D1 p17 | |
| F02-W0702 | 171 | Vo-09 Club | Synth | D1 p18 | |
| F02-W0703 | 172 | SD-Orchestral | ROM | D1 p16 | |
| F02-W0704 | 172 | H.H.Cl-EXB04 01 | Best | D1 p17 | |
| F02-W0705 | 172 | Vo-09 Oh-Yeah M | Synth | D1 p18 | |
| F02-W0706 | 173 | 88-BD 1(Rap) | ROM | D1 p16 | |
| F02-W0707 | 173 | H.H.Cl-EXB04 02 | Best | D1 p17 | |
| F02-W0708 | 173 | Vo-09 Uagh | Synth | D1 p18 | |
| F02-W0709 | 174 | 88-BD 2(R&B) | ROM | D1 p16 | |
| F02-W0710 | 174 | H.H.Cl-EXB04 03 | Best | D1 p17 | |
| F02-W0711 | 174 | Vo-09 Hah | Synth | D1 p18 | |
| F02-W0712 | 175 | 88-BD 3 | ROM | D1 p16 | |
| F02-W0713 | 175 | H.H.Cl-EXB04 04 | Best | D1 p17 | |
| F02-W0714 | 175 | Vo-09 Baby | Synth | D1 p18 | |
| F02-W0715 | 176 | 99-BD 1 | ROM | D1 p16 | |
| F02-W0716 | 176 | H.H.Cl-EXB04 05 | Best | D1 p17 | |
| F02-W0717 | 176 | Vo-09 Vavy | Synth | D1 p18 | |
| F02-W0718 | 177 | 99-BD 2 | ROM | D1 p16 | |
| F02-W0719 | 177 | H.H.Cl-EXB04 06 | Best | D1 p17 | |
| F02-W0720 | 177 | Vo-09It'sAllGood | Synth | D1 p18 | |
| F02-W0721 | 178 | 99-BD 3 | ROM | D1 p16 | |
| F02-W0722 | 178 | H.H.Cl-EXB04 07 | Best | D1 p17 | |
| F02-W0723 | 178 | Vo-09 Ya-ee | Synth | D1 p18 | |
| F02-W0724 | 179 | 99-BD 4 | ROM | D1 p16 | |
| F02-W0725 | 179 | H.H.Cl-EXB04 08 | Best | D1 p17 | |
| F02-W0726 | 179 | Vo-09 Ya-ee-ha | Synth | D1 p18 | |
| F02-W0727 | 180 | 99-BD 5 | ROM | D1 p16 | |
| F02-W0728 | 180 | H.H.Cl-EXB04 09 | Best | D1 p17 | |
| F02-W0729 | 180 | Vo-09 Ye-ah | Synth | D1 p18 | |
| F02-W0730 | 181 | 88-SD 1(New) | ROM | D1 p16 | |
| F02-W0731 | 181 | H.H.Cl-EXB04 10 | Best | D1 p17 | |
| F02-W0732 | 181 | Vo-09 Ya-ee-ye-e | Synth | D1 p18 | |
| F02-W0733 | 182 | 88-SD 2(Old) | ROM | D1 p16 | |
| F02-W0734 | 182 | H.H.Cl-EXB04 11 | Best | D1 p17 | |
| F02-W0735 | 182 | Vo-09 Ay-ay-he | Synth | D1 p18 | |
| F02-W0736 | 183 | 99-SD 1 | ROM | D1 p16 | |
| F02-W0737 | 183 | H.H.Cl-EXB04 12 | Best | D1 p17 | |
| F02-W0738 | 183 | Vo-09 Aow | Synth | D1 p18 | |
| F02-W0739 | 184 | 99-SD 2 | ROM | D1 p16 | |
| F02-W0740 | 184 | H.H.Cl-EXB04 13 | Best | D1 p17 | |
| F02-W0741 | 184 | Vo-09 You-are-h | Synth | D1 p18 | |
| F02-W0742 | 185 | 88-Rimshot | ROM | D1 p16 | |
| F02-W0743 | 185 | H.H.Cl-EXB04 14 | Best | D1 p17 | |
| F02-W0744 | 185 | Vo-09 What | Synth | D1 p18 | |
| F02-W0745 | 186 | 88-Claps | ROM | D1 p16 | |
| F02-W0746 | 186 | H.H.Cl-EXB04 15 | Best | D1 p17 | |
| F02-W0747 | 186 | Vo-09 Ever | Synth | D1 p18 | |
| F02-W0748 | 187 | 88-HHat Closed | ROM | D1 p16 | |
| F02-W0749 | 187 | H.H.Cl-EXB04 16 | Best | D1 p17 | |
| F02-W0750 | 187 | Vo-09 Oooua | Synth | D1 p18 | |
| F02-W0751 | 188 | 88-HHat Open | ROM | D1 p16 | |
| F02-W0752 | 188 | H.H.Cl-EXB04 17 | Best | D1 p17 | |
| F02-W0753 | 188 | Vo09It'sWhatEver | Synth | D1 p18 | |
| F02-W0754 | 189 | 99-HHat Closed | ROM | D1 p16 | |
| F02-W0755 | 189 | H.H.Cl-EXB04 18 | Best | D1 p17 | |
| F02-W0756 | 189 | Vo-09 CaMon | Synth | D1 p18 | |
| F02-W0757 | 190 | 99-HHat Open | ROM | D1 p16 | |
| F02-W0758 | 190 | H.H.Cl-EXB04 19 | Best | D1 p17 | |
| F02-W0759 | 190 | Vo-09 Do It | Synth | D1 p18 | |
| F02-W0760 | 191 | 88-Crash | ROM | D1 p16 | |
| F02-W0761 | 191 | H.H.Cl-EXB04 20 | Best | D1 p17 | |
| F02-W0762 | 191 | Vo-09 Party | Synth | D1 p18 | |
| F02-W0763 | 192 | 88-Tom | ROM | D1 p16 | |
| F02-W0764 | 192 | H.H.Cl-EXB04 21 | Best | D1 p17 | |
| F02-W0765 | 192 | Vo-09 TurnAround | Synth | D1 p18 | |
| F02-W0766 | 193 | 88-Conga | ROM | D1 p16 | |
| F02-W0767 | 193 | H.H.Cl-EXB04 22 | Best | D1 p17 | |
| F02-W0768 | 193 | Vo-09 Wo'I dance | Synth | D1 p18 | |
| F02-W0769 | 194 | 88-Clave | ROM | D1 p16 | |
| F02-W0770 | 194 | H.H.Cl-EXB04 23 | Best | D1 p17 | |
| F02-W0771 | 194 | Vo-09 Take Up | Synth | D1 p18 | |
| F02-W0772 | 195 | 88-Cowbell | ROM | D1 p16 | |
| F02-W0773 | 195 | H.H.Cl-EXB04 24 | Best | D1 p17 | |
| F02-W0774 | 195 | Vo-09 n Round | Synth | D1 p18 | |
| F02-W0775 | 196 | 88-Maraca | ROM | D1 p16 | |
| F02-W0776 | 196 | H.H.Cl-EXB04 25 | Best | D1 p17 | |
| F02-W0777 | 196 | Vo-09 Ye-ah-ar | Synth | D1 p18 | |
| F02-W0778 | 197 | FM E.Tom | ROM | D1 p16 | |
| F02-W0779 | 197 | H.H.Cl-EXB04 26 | Best | D1 p17 | |
| F02-W0780 | 197 | Vo-09 Go-o | Synth | D1 p18 | |
| F02-W0781 | 198 | Real E.Tom | ROM | D1 p16 | |
| F02-W0782 | 198 | H.H.Cl-EXB04 27 | Best | D1 p17 | |
| F02-W0783 | 198 | Vo-09 Oh-o-owu | Synth | D1 p18 | |
| F02-W0784 | 199 | Vocal SD | ROM | D1 p16 | |
| F02-W0785 | 199 | H.H.Cl-EXB04 28 | Best | D1 p17 | |
| F02-W0786 | 199 | Vo-09 Well | Synth | D1 p18 | |
| F02-W0787 | 200 | Vocal Cowbell | ROM | D1 p16 | |
| F02-W0788 | 200 | H.H.Cl-EXB04 29 | Best | D1 p17 | |
| F02-W0789 | 200 | Vo-09 Ho-o | Synth | D1 p18 | |
| F02-W0790 | 201 | Vocal Woodblock | ROM | D1 p16 | |
| F02-W0791 | 201 | H.H.Cl-EXB04 30 | Best | D1 p17 | |
| F02-W0792 | 201 | Vo-09 Dance | Synth | D1 p18 | |
| F02-W0793 | 202 | Dance Perc-Hip | ROM | D1 p16 | |
| F02-W0794 | 202 | H.H.Cl-EXB04 31 | Best | D1 p17 | |
| F02-W0795 | 202 | Vo-09 Ha-ooo-wa | Synth | D1 p18 | |
| F02-W0796 | 203 | Dance Perc-Hous1 | ROM | D1 p16 | |
| F02-W0797 | 203 | H.H.Cl-EXB04 32 | Best | D1 p17 | |
| F02-W0798 | 203 | Vo-09 I feel | Synth | D1 p18 | |
| F02-W0799 | 204 | Dance Perc-Hous2 | ROM | D1 p16 | |
| F02-W0800 | 204 | H.H.Cl-EXB04 33 | Best | D1 p17 | |
| F02-W0801 | 204 | Vo-09 Good | Synth | D1 p18 | |
| F02-W0802 | 205 | Zap 1 | ROM | D1 p16 | |
| F02-W0803 | 205 | H.H.Cl-EXB04 34 | Best | D1 p17 | |
| F02-W0804 | 205 | Vo-09 Turnin' Up | Synth | D1 p18 | |
| F02-W0805 | 206 | Zap 2 | ROM | D1 p16 | |
| F02-W0806 | 206 | H.H.Cl-EXB04 35 | Best | D1 p17 | |
| F02-W0807 | 206 | Vo-09 Oohh | Synth | D1 p18 | |
| F02-W0808 | 207 | Zap 3 | ROM | D1 p16 | |
| F02-W0809 | 207 | H.H.Cl-EXB04 36 | Best | D1 p17 | |
| F02-W0810 | 207 | Vo-09 Ahoo | Synth | D1 p18 | |
| F02-W0811 | 208 | DJ-Scratch 1 | ROM | D1 p16 | |
| F02-W0812 | 208 | H.H.Cl-EXB04 37 | Best | D1 p17 | |
| F02-W0813 | 208 | Vo-09 Mmmm | Synth | D1 p18 | |
| F02-W0814 | 209 | DJ-Scratch 2 | ROM | D1 p16 | |
| F02-W0815 | 209 | H.H.Cl-EXB04 38 | Best | D1 p17 | |
| F02-W0816 | 209 | Vo-09 Ooh (wet) | Synth | D1 p18 | |
| F02-W0817 | 210 | DJ-Scratch 3a | ROM | D1 p16 | |
| F02-W0818 | 210 | H.H.Cl-EXB04 39 | Best | D1 p17 | |
| F02-W0819 | 210 | Vo-09 Uuh | Synth | D1 p18 | |
| F02-W0820 | 211 | DJ-Scratch 3b | ROM | D1 p16 | |
| F02-W0821 | 211 | H.H.Cl-EXB04 40 | Best | D1 p17 | |
| F02-W0822 | 211 | Vo-09 Uh-a-ha | Synth | D1 p18 | |
| F02-W0823 | 212 | DJ-Scratch 3c | ROM | D1 p16 | |
| F02-W0824 | 212 | H.H.Cl-EXB04 41 | Best | D1 p17 | |
| F02-W0825 | 212 | Vo-09 Hey F1 | Synth | D1 p18 | |
| F02-W0826 | 213 | DJ-Scratch 4 | ROM | D1 p16 | |
| F02-W0827 | 213 | H.H.Ho-EXB04 00 | Best | D1 p17 | |
| F02-W0828 | 213 | Vo-09 Hey F2 | Synth | D1 p18 | |
| F02-W0829 | 214 | DJ-Hit Rub | ROM | D1 p16 | |
| F02-W0830 | 214 | H.H.Ho-EXB04 01 | Best | D1 p17 | |
| F02-W0831 | 214 | Vo-09 Haaay | Synth | D1 p18 | |
| F02-W0832 | 215 | DJ-1c Vocal Rub | ROM | D1 p16 | |
| F02-W0833 | 215 | H.H.Ho-EXB04 02 | Best | D1 p17 | |
| F02-W0834 | 215 | Vo-09 C'mon | Synth | D1 p18 | |
| F02-W0835 | 216 | DJ-1c Vocal Rub | ROM | D1 p16 | |
| F02-W0836 | 216 | H.H.Ho-EXB04 03 | Best | D1 p17 | |
| F02-W0837 | 216 | Vo-09 Com'On | Synth | D1 p18 | |
| F02-W0838 | 217 | DJ-BD Rub | ROM | D1 p16 | |
| F02-W0839 | 217 | H.H.Ho-EXB04 04 | Best | D1 p17 | |
| F02-W0840 | 217 | Vo-09 Come On | Synth | D1 p18 | |
| F02-W0841 | 218 | DJ-SD Rub | ROM | D1 p16 | |
| F02-W0842 | 218 | H.H.Ho-EXB04 05 | Best | D1 p17 | |
| F02-W0843 | 218 | Vo-09 Pardy | Synth | D1 p18 | |
| F02-W0844 | 219 | DJ-Record Stop | ROM | D1 p16 | |
| F02-W0845 | 219 | H.H.Ho-EXB04 06 | Best | D1 p17 | |
| F02-W0846 | 219 | Vo-09 PartyNight | Synth | D1 p18 | |
| F02-W0847 | 220 | DJ-Reverse | ROM | D1 p16 | |
| F02-W0848 | 220 | H.H.Ho-EXB04 07 | Best | D1 p17 | |
| F02-W0849 | 220 | Vo-09 Yaeh F | Synth | D1 p18 | |
| F02-W0850 | 221 | DJ-Old Record | ROM | D1 p16 | |
| F02-W0851 | 221 | H.H.Ho-EXB04 08 | Best | D1 p17 | |
| F02-W0852 | 221 | Vo-09 Yah | Synth | D1 p18 | |
| F02-W0853 | 222 | Orchestra Hit | ROM | D1 p16 | |
| F02-W0854 | 222 | H.H.Ho-EXB04 09 | Best | D1 p17 | |
| F02-W0855 | 222 | Vo-09 Oh-ya-yah | Synth | D1 p18 | |
| F02-W0856 | 223 | Pizz Hit | ROM | D1 p16 | |
| F02-W0857 | 223 | H.H.Ho-EXB04 10 | Best | D1 p17 | |
| F02-W0858 | 223 | Vo-09 Oohh | Synth | D1 p18 | |
| F02-W0859 | 224 | Band Hit | ROM | D1 p16 | |
| F02-W0860 | 224 | H.H.Ho-EXB04 11 | Best | D1 p17 | |
| F02-W0861 | 224 | Vo-09 Owww | Synth | D1 p18 | |
| F02-W0862 | 225 | ImpactHit | ROM | D1 p16 | |
| F02-W0863 | 225 | H.H.Ho-EXB04 12 | Best | D1 p17 | |
| F02-W0864 | 225 | Vo-09 Aaa! | Synth | D1 p18 | |
| F02-W0865 | 226 | Philly Hit | ROM | D1 p16 | |
| F02-W0866 | 226 | H.H.Ho-EXB04 13 | Best | D1 p17 | |
| F02-W0867 | 226 | Vo-09 Aa-ow | Synth | D1 p18 | |
| F02-W0868 | 227 | Drumstick Hit | ROM | D1 p16 | |
| F02-W0869 | 227 | H.H.Ho-EXB04 14 | Best | D1 p17 | |
| F02-W0870 | 227 | Vo-09 Uhwoah | Synth | D1 p18 | |
| F02-W0871 | 228 | Finger Snap | ROM | D1 p16 | |
| F02-W0872 | 228 | H.H.Ho-EXB04 15 | Best | D1 p17 | |
| F02-W0873 | 228 | Vo-09 Oh-Yeah F | Synth | D1 p18 | |
| F02-W0874 | 229 | Hand Claps | ROM | D1 p16 | |
| F02-W0875 | 229 | H.H.Ho-EXB04 16 | Best | D1 p17 | |
| F02-W0876 | 229 | Vo-09 Alright | Synth | D1 p18 | |
| F02-W0877 | 230 | Hand Claps-Hip | ROM | D1 p16 | |
| F02-W0878 | 230 | H.H.Op-EXB04 00 | Best | D1 p17 | |
| F02-W0879 | 230 | Vo-09 I Love It | Synth | D1 p18 | |
| F02-W0880 | 231 | Hand Claps-Happy | ROM | D1 p16 | |
| F02-W0881 | 231 | H.H.Op-EXB04 01 | Best | D1 p17 | |
| F02-W0882 | 231 | Vo-09 I believe | Synth | D1 p18 | |
| F02-W0883 | 232 | Conga Lo-Open | ROM | D1 p16 | |
| F02-W0884 | 232 | H.H.Op-EXB04 02 | Best | D1 p17 | |
| F02-W0885 | 232 | Vo-09 Don't Stop | Synth | D1 p18 | |
| F02-W0886 | 233 | Conga Lo-MtSlap | ROM | D1 p16 | |
| F02-W0887 | 233 | H.H.Op-EXB04 03 | Best | D1 p17 | |
| F02-W0888 | 233 | Vo-09 I wanna | Synth | D1 p18 | |
| F02-W0889 | 234 | Conga Lo-Slap | ROM | D1 p16 | |
| F02-W0890 | 234 | H.H.Op-EXB04 04 | Best | D1 p17 | |
| F02-W0891 | 234 | Vo-09 Be | Synth | D1 p18 | |
| F02-W0892 | 235 | Conga Hi-Open | ROM | D1 p16 | |
| F02-W0893 | 235 | H.H.Op-EXB04 05 | Best | D1 p17 | |
| F02-W0894 | 235 | Vo-09 I Need | Synth | D1 p18 | |
| F02-W0895 | 236 | Conga Hi-Mute | ROM | D1 p16 | |
| F02-W0896 | 236 | H.H.Op-EXB04 06 | Best | D1 p17 | |
| F02-W0897 | 236 | Vo-09 You | Synth | D1 p18 | |
| F02-W0898 | 237 | Conga Hi-MtSlap | ROM | D1 p16 | |
| F02-W0899 | 237 | H.H.Op-EXB04 07 | Best | D1 p17 | |
| F02-W0900 | 237 | Vo-09 Crazy | Synth | D1 p18 | |
| F02-W0901 | 238 | Conga Hi-Slap 1 | ROM | D1 p16 | |
| F02-W0902 | 238 | H.H.Op-EXB04 08 | Best | D1 p17 | |
| F02-W0903 | 238 | Vo-09 Get Down | Synth | D1 p18 | |
| F02-W0904 | 239 | Conga Hi-Slap 2 | ROM | D1 p16 | |
| F02-W0905 | 239 | H.H.Ana-EXB04 00 | Best | D1 p17 | |
| F02-W0906 | 239 | Vo-09 Go | Synth | D1 p18 | |
| F02-W0907 | 240 | Conga-Heel | ROM | D1 p16 | |
| F02-W0908 | 240 | H.H.Ana-EXB04 01 | Best | D1 p17 | |
| F02-W0909 | 241 | Conga-Toe | ROM | D1 p16 | |
| F02-W0910 | 241 | H.H.Ana-EXB04 02 | Best | D1 p17 | |
| F02-W0911 | 242 | Bongo Lo-Open | ROM | D1 p16 | |
| F02-W0912 | 242 | H.H.Ana-EXB04 03 | Best | D1 p17 | |
| F02-W0913 | 243 | Bongo Lo-Slap | ROM | D1 p16 | |
| F02-W0914 | 243 | H.H.Ana-EXB04 04 | Best | D1 p17 | |
| F02-W0915 | 244 | Bongo Lo-Stick | ROM | D1 p16 | |
| F02-W0916 | 244 | H.H.Ana-EXB04 05 | Best | D1 p17 | |
| F02-W0917 | 245 | Bongo Hi-Open | ROM | D1 p16 | |
| F02-W0918 | 245 | H.H.Ana-EXB04 06 | Best | D1 p17 | |
| F02-W0919 | 246 | Bongo Hi-Slap | ROM | D1 p16 | |
| F02-W0920 | 246 | H.H.Ana-EXB04 07 | Best | D1 p17 | |
| F02-W0921 | 247 | Bongo Hi-Stick 1 | ROM | D1 p16 | |
| F02-W0922 | 247 | H.H.Ana-EXB04 08 | Best | D1 p17 | |
| F02-W0923 | 248 | Bongo Hi-Stick 2 | ROM | D1 p16 | |
| F02-W0924 | 248 | H.H.Ana-EXB04 09 | Best | D1 p17 | |
| F02-W0925 | 249 | Djembe-Open | ROM | D1 p16 | |
| F02-W0926 | 249 | H.H.Ana-EXB04 10 | Best | D1 p17 | |
| F02-W0927 | 250 | Djembe-Mute | ROM | D1 p16 | |
| F02-W0928 | 250 | H.H.Ana-EXB04 11 | Best | D1 p17 | |
| F02-W0929 | 251 | Djembe-Slap | ROM | D1 p16 | |
| F02-W0930 | 251 | H.H.Ana-EXB04 12 | Best | D1 p17 | |
| F02-W0931 | 252 | Castanet-Single | ROM | D1 p16 | |
| F02-W0932 | 252 | H.H.Ana-EXB04 13 | Best | D1 p17 | |
| F02-W0933 | 253 | Castanet-Dble 1 | ROM | D1 p16 | |
| F02-W0934 | 253 | H.H.SE-EXB04 00 | Best | D1 p17 | |
| F02-W0935 | 254 | Castanet-Dble 2 | ROM | D1 p16 | |
| F02-W0936 | 254 | H.H.SE-EXB04 01 | Best | D1 p17 | |
| F02-W0937 | 255 | Guiro-Long | ROM | D1 p16 | |
| F02-W0938 | 255 | H.H.SE-EXB04 02 | Best | D1 p17 | |
| F02-W0939 | 256 | Guiro-Short | ROM | D1 p16 | |
| F02-W0940 | 256 | Crash-EXB04 | Best | D1 p17 | |
| F02-W0941 | 257 | Guiro-Tap | ROM | D1 p16 | |
| F02-W0942 | 257 | Ride-EXB04 00 | Best | D1 p17 | |
| F02-W0943 | 258 | Maracas-Push | ROM | D1 p16 | |
| F02-W0944 | 258 | Ride-EXB04 01 | Best | D1 p17 | |
| F02-W0945 | 259 | Maracas-Pull | ROM | D1 p16 | |
| F02-W0946 | 259 | Ride-EXB04 02 | Best | D1 p17 | |
| F02-W0947 | 260 | Baya-Open | ROM | D1 p16 | |
| F02-W0948 | 260 | RideCup-EXB04 00 | Best | D1 p17 | |
| F02-W0949 | 261 | Baya-Ghe | ROM | D1 p16 | |
| F02-W0950 | 261 | RideCup-EXB04 01 | Best | D1 p17 | |
| F02-W0951 | 262 | Baya-Mute 1 | ROM | D1 p16 | |
| F02-W0952 | 262 | Hit-EXB04 00 | Best | D1 p17 | |
| F02-W0953 | 263 | Baya-Mute 2 | ROM | D1 p16 | |
| F02-W0954 | 263 | Hit-EXB04 01 | Best | D1 p17 | |
| F02-W0955 | 264 | Baya-Mute 3 | ROM | D1 p16 | |
| F02-W0956 | 264 | Hit-EXB04 02 | Best | D1 p17 | |
| F02-W0957 | 265 | Baya-Mute 4 | ROM | D1 p16 | |
| F02-W0958 | 265 | Hit-EXB04 03 | Best | D1 p17 | |
| F02-W0959 | 266 | Baya-Mute 5 | ROM | D1 p16 | |
| F02-W0960 | 266 | Hit-EXB04 04 | Best | D1 p17 | |
| F02-W0961 | 267 | Tabla-Na | ROM | D1 p16 | |
| F02-W0962 | 267 | Hit-EXB04 05 | Best | D1 p17 | |
| F02-W0963 | 268 | Tabla-Open | ROM | D1 p16 | |
| F02-W0964 | 268 | Tamb-EXB04 00 | Best | D1 p17 | |
| F02-W0965 | 269 | Tabla-Tin | ROM | D1 p16 | |
| F02-W0966 | 269 | Tamb-EXB04 01 | Best | D1 p17 | |
| F02-W0967 | 270 | Tabla-Mute 1 | ROM | D1 p16 | |
| F02-W0968 | 270 | Tamb-EXB04 02 | Best | D1 p17 | |
| F02-W0969 | 271 | Tabla-Mute 2 | ROM | D1 p16 | |
| F02-W0970 | 271 | Tamb-EXB04 03 | Best | D1 p17 | |
| F02-W0971 | 272 | Tabla-Mute 3 | ROM | D1 p16 | |
| F02-W0972 | 272 | Tamb-EXB04 04 | Best | D1 p17 | |
| F02-W0973 | 273 | Tabla-Mute 4 | ROM | D1 p16 | |
| F02-W0974 | 273 | Tamb-EXB04 05 | Best | D1 p17 | |
| F02-W0975 | 274 | Taiko-Open | ROM | D1 p16 | |
| F02-W0976 | 274 | Tamb-EXB04 06 | Best | D1 p17 | |
| F02-W0977 | 275 | Taiko-Rim | ROM | D1 p16 | |
| F02-W0978 | 275 | Tamb-EXB04 07 | Best | D1 p17 | |
| F02-W0979 | 276 | Tsuzumi | ROM | D1 p16 | |
| F02-W0980 | 276 | Tamb-EXB04 08 | Best | D1 p17 | |
| F02-W0981 | 277 | Vibraslap | ROM | D1 p16 | |
| F02-W0982 | 277 | Shaker-EXB04 | Best | D1 p17 | |
| F02-W0983 | 278 | Claves | ROM | D1 p16 | |
| F02-W0984 | 278 | Cabasa-EXB04 00 | Best | D1 p17 | |
| F02-W0985 | 279 | Woodblock 1 | ROM | D1 p16 | |
| F02-W0986 | 279 | Cabasa-EXB04 01 | Best | D1 p17 | |
| F02-W0987 | 280 | Woodblock 2 | ROM | D1 p16 | |
| F02-W0988 | 280 | Cabasa-EXB04 02 | Best | D1 p17 | |
| F02-W0989 | 281 | Timbale Lo-Open | ROM | D1 p16 | |
| F02-W0990 | 281 | Maracas-EXB04 | Best | D1 p17 | |
| F02-W0991 | 282 | Timbale Lo-Mute | ROM | D1 p16 | |
| F02-W0992 | 282 | Caxixi-EXB04 00 | Best | D1 p17 | |
| F02-W0993 | 283 | Timbale Lo-Rim | ROM | D1 p16 | |
| F02-W0994 | 283 | Caxixi-EXB04 01 | Best | D1 p17 | |
| F02-W0995 | 284 | Timbale Hi-Edge | ROM | D1 p16 | |
| F02-W0996 | 284 | Clap-EXB04 00 | Best | D1 p17 | |
| F02-W0997 | 285 | Timbale Hi-Rim 1 | ROM | D1 p16 | |
| F02-W0998 | 285 | Clap-EXB04 01 | Best | D1 p17 | |
| F02-W0999 | 286 | Timbale Hi-Rim 2 | ROM | D1 p16 | |
| F02-W1000 | 286 | Clap-EXB04 02 | Best | D1 p17 | |
| F02-W1001 | 287 | Timbale-Paila | ROM | D1 p16 | |
| F02-W1002 | 287 | 88Tom-EXB04 | Best | D1 p17 | |
| F02-W1003 | 288 | Tambourine-Push | ROM | D1 p16 | |
| F02-W1004 | 288 | 88Cowbell-EXB04 | Best | D1 p17 | |
| F02-W1005 | 289 | Tambourine-Pull | ROM | D1 p16 | |
| F02-W1006 | 289 | PercSE-EXB04 00 | Best | D1 p17 | |
| F02-W1007 | 290 | Tambourine-Acc.1 | ROM | D1 p16 | |
| F02-W1008 | 290 | PercSE-EXB04 01 | Best | D1 p17 | |
| F02-W1009 | 291 | Tambourine-Acc.2 | ROM | D1 p16 | |
| F02-W1010 | 291 | PercSE-EXB04 02 | Best | D1 p17 | |
| F02-W1011 | 292 | Triangle-Open | ROM | D1 p16 | |
| F02-W1012 | 292 | PercSE-EXB04 03 | Best | D1 p17 | |
| F02-W1013 | 293 | Triangle-Mute | ROM | D1 p16 | |
| F02-W1014 | 293 | PercSE-EXB04 04 | Best | D1 p17 | |
| F02-W1015 | 294 | Triangle-Roll | ROM | D1 p16 | |
| F02-W1016 | 294 | F.Bell-EXB04 | Best | D1 p17 | |
| F02-W1017 | 295 | Cuica-Hi | ROM | D1 p16 | |
| F02-W1018 | 295 | Industry-EXB04 | Best | D1 p17 | |
| F02-W1019 | 296 | Cuica-Lo | ROM | D1 p16 | |
| F02-W1020 | 296 | Silent-EXB04 | Best | D1 p17 | |
| F02-W1021 | 297 | Shaker 1 | ROM | D1 p16 | |
| F02-W1022 | 297 | SynthDrum-BD | Best | D1 p17 | |
| F02-W1023 | 298 | Shaker 2 | ROM | D1 p16 | |
| F02-W1024 | 298 | SynthDrum-SD1 H | Best | D1 p17 | |
| F02-W1025 | 299 | Cabasa-Up | ROM | D1 p16 | |
| F02-W1026 | 299 | SynthDrum-SD1 L | Best | D1 p17 | |
| F02-W1027 | 300 | Cabasa-Down | ROM | D1 p16 | |
| F02-W1028 | 300 | SynthDrum-SD2 H | Best | D1 p17 | |
| F02-W1029 | 301 | Cabasa-Tap | ROM | D1 p16 | |
| F02-W1030 | 301 | SynthDrum-SD2 L | Best | D1 p17 | |
| F02-W1031 | 302 | Caxixi-Hard | ROM | D1 p16 | |
| F02-W1032 | 302 | SynthDrum-OHH 1 | Best | D1 p17 | |
| F02-W1033 | 303 | Caxixi-Soft | ROM | D1 p16 | |
| F02-W1034 | 303 | SynthDrum-OHH 2 | Best | D1 p17 | |
| F02-W1035 | 304 | Agogo Bell | ROM | D1 p16 | |
| F02-W1036 | 304 | SynthDrum-OHH 3 | Best | D1 p17 | |
| F02-W1037 | 305 | Cowbell 1 | ROM | D1 p16 | |
| F02-W1038 | 305 | SynthDrum-Tom H | Best | D1 p17 | |
| F02-W1039 | 306 | Cowbell 2 | ROM | D1 p16 | |
| F02-W1040 | 306 | SynthDrum-Tom M | Best | D1 p17 | |
| F02-W1041 | 307 | Chacha Bell | ROM | D1 p16 | |
| F02-W1042 | 307 | SynthDrum-Tom L | Best | D1 p17 | |
| F02-W1043 | 308 | Mambo Bell | ROM | D1 p16 | |
| F02-W1044 | 308 | SynthDrum-Perc H | Best | D1 p17 | |
| F02-W1045 | 309 | Sleigh Bell | ROM | D1 p16 | |
| F02-W1046 | 309 | SynthDrum-Perc M | Best | D1 p17 | |
| F02-W1047 | 310 | Rap Sleighbell | ROM | D1 p16 | |
| F02-W1048 | 310 | SynthDrum-Perc L | Best | D1 p17 | |
| F02-W1049 | 311 | Finger Cymbal | ROM | D1 p16 | |
| F02-W1050 | 311 | Rap Voice 5 | Best | D1 p17 | |
| F02-W1051 | 312 | Marc Tree | ROM | D1 p16 | |
| F02-W1052 | 312 | Rap Voice 6 | Best | D1 p17 | |
| F02-W1053 | 313 | Marc Tree LP | ROM | D1 p16 | |
| F02-W1054 | 313 | Rap Voice 7 | Best | D1 p17 | |
| F02-W1055 | 314 | Bell Tree | ROM | D1 p16 | |
| F02-W1056 | 314 | Gospel Voice 1 | Best | D1 p17 | |
| F02-W1057 | 315 | Flexatone | ROM | D1 p16 | |
| F02-W1058 | 315 | Gospel Voice 2 | Best | D1 p17 | |
| F02-W1059 | 316 | Samba Whistle | ROM | D1 p16 | |
| F02-W1060 | 316 | Gospel Voice 12 | Best | D1 p17 | |
| F02-W1061 | 317 | Chinese Gong | ROM | D1 p16 | |
| F02-W1062 | 317 | Gospel Voice 13 | Best | D1 p17 | |
| F02-W1063 | 318 | Industry | ROM | D1 p16 | |
| F02-W1064 | 318 | Gospel Voice 14 | Best | D1 p17 | |
| F02-W1065 | 319 | Cyber Drum | ROM | D1 p16 | |
| F02-W1066 | 319 | Gospel Voice 15 | Best | D1 p17 | |
| F02-W1067 | 320 | SD-Metal Striker | ROM | D1 p16 | |
| F02-W1068 | 320 | Gospel Voice 17 | Best | D1 p17 | |
| F02-W1069 | 321 | SD-Air Explosion | ROM | D1 p16 | |
| F02-W1070 | 321 | Gospel Voice 18 | Best | D1 p17 | |
| F02-W1071 | 322 | Industry Loop 1 | ROM | D1 p16 | |
| F02-W1072 | 322 | Gospel Voice 19 | Best | D1 p17 | |
| F02-W1073 | 323 | Industry Loop 2 | ROM | D1 p16 | |
| F02-W1074 | 323 | Gospel Voice 21 | Best | D1 p17 | |
| F02-W1075 | 324 | Metal Hit | ROM | D1 p16 | |
| F02-W1076 | 324 | Gospel Voice 23 | Best | D1 p17 | |
| F02-W1077 | 325 | Yea Solo | ROM | D1 p16 | |
| F02-W1078 | 325 | Gospel Voice 24 | Best | D1 p17 | |
| F02-W1079 | 326 | Year solo | ROM | D1 p16 | |
| F02-W1080 | 326 | Gospel Voice 25 | Best | D1 p17 | |
| F02-W1081 | 327 | Uhh Solo | ROM | D1 p16 | |
| F02-W1082 | 327 | Gospel Voice 26 | Best | D1 p17 | |
| F02-W1083 | 328 | Hit It | ROM | D1 p16 | |
| F02-W1084 | 328 | Gospel Voice 28 | Best | D1 p17 | |
| F02-W1085 | 329 | Uhhhh Solo | ROM | D1 p16 | |
| F02-W1086 | 329 | Gospel Voice 29 | Best | D1 p17 | |
| F02-W1087 | 330 | CompuVoice-Beep | ROM | D1 p16 | |
| F02-W1088 | 330 | Gospel Voice 30 | Best | D1 p17 | |
| F02-W1089 | 331 | CompuVoice-Noise | ROM | D1 p16 | |
| F02-W1090 | 331 | Gospel Voice 31 | Best | D1 p17 | |
| F02-W1091 | 332 | CompuVoice-Chi | ROM | D1 p16 | |
| F02-W1092 | 332 | Gospel Voice 34 | Best | D1 p17 | |
| F02-W1093 | 333 | CompuVoice-Test | ROM | D1 p16 | |
| F02-W1094 | 333 | Gospel Voice 35 | Best | D1 p17 | |
| F02-W1095 | 334 | CompuVoice-Do it | ROM | D1 p16 | |
| F02-W1096 | 334 | Guitar Chord 1 | Best | D1 p17 | |
| F02-W1097 | 335 | CompuVoice-When | ROM | D1 p16 | |
| F02-W1098 | 335 | Guitar Chord 2 | Best | D1 p17 | |
| F02-W1099 | 336 | CompuVoice-Toi | ROM | D1 p16 | |
| F02-W1100 | 336 | Guitar Chord 3 | Best | D1 p17 | |
| F02-W1101 | 337 | CompuVoice-Ti | ROM | D1 p16 | |
| F02-W1102 | 337 | Guitar Chord 4 | Best | D1 p17 | |
| F02-W1103 | 338 | "Four..." Solo | ROM | D1 p16 | |
| F02-W1104 | 338 | Guitar Chord 5 | Best | D1 p17 | |
| F02-W1105 | 339 | "Three..." Solo | ROM | D1 p16 | |
| F02-W1106 | 339 | Guitar Chord 6 | Best | D1 p17 | |
| F02-W1107 | 340 | "Two..." Solo | ROM | D1 p16 | |
| F02-W1108 | 340 | Guitar Chord 7 | Best | D1 p17 | |
| F02-W1109 | 341 | "One..." Solo | ROM | D1 p16 | |
| F02-W1110 | 341 | Guitar Chord 8 | Best | D1 p17 | |
| F02-W1111 | 342 | PC Vox-Sis | ROM | D1 p16 | |
| F02-W1112 | 342 | Guitar Chord 9 | Best | D1 p17 | |
| F02-W1113 | 343 | PC Vox-Dunc | ROM | D1 p16 | |
| F02-W1114 | 343 | Guitar Chord 10 | Best | D1 p17 | |
| F02-W1115 | 344 | PC Vox-Cks | ROM | D1 p16 | |
| F02-W1116 | 344 | Guitar Chord 11 | Best | D1 p17 | |
| F02-W1117 | 345 | PC Vox-For | ROM | D1 p16 | |
| F02-W1118 | 345 | Guitar Chord 12 | Best | D1 p17 | |
| F02-W1119 | 346 | PC Vox-One | ROM | D1 p16 | |
| F02-W1120 | 346 | Guitar Chord 13 | Best | D1 p17 | |
| F02-W1121 | 347 | PC Vox-If | ROM | D1 p16 | |
| F02-W1122 | 347 | Guitar Chord 14 | Best | D1 p17 | |
| F02-W1123 | 348 | PC Vox-Zee | ROM | D1 p16 | |
| F02-W1124 | 348 | Guitar Chord 15 | Best | D1 p17 | |
| F02-W1125 | 349 | PC Vox-Ae | ROM | D1 p16 | |
| F02-W1126 | 349 | Guitar Wah 1 | Best | D1 p17 | |
| F02-W1127 | 350 | PC Vox-Pa | ROM | D1 p16 | |
| F02-W1128 | 350 | Guitar Wah 2 | Best | D1 p17 | |
| F02-W1129 | 351 | PC Vox-Sh | ROM | D1 p16 | |
| F02-W1130 | 351 | Guitar Wah 3 | Best | D1 p17 | |
| F02-W1131 | 352 | PC Vox-O | ROM | D1 p16 | |
| F02-W1132 | 352 | Guitar Wah 4 | Best | D1 p17 | |
| F02-W1133 | 353 | PC Vox-Tu | ROM | D1 p16 | |
| F02-W1134 | 353 | Guitar Wah 5 | Best | D1 p17 | |
| F02-W1135 | 354 | Stadium | ROM | D1 p16 | |
| F02-W1136 | 354 | Band Hit 1 | Best | D1 p17 | |
| F02-W1137 | 355 | Applause | ROM | D1 p16 | |
| F02-W1138 | 355 | Band Hit 11 | Best | D1 p17 | |
| F02-W1139 | 356 | Scream | ROM | D1 p16 | |
| F02-W1140 | 356 | Choir 5th. Hit | Best | D1 p17 | |
| F02-W1141 | 357 | Laughing | ROM | D1 p16 | |
| F02-W1142 | 357 | Rave Choir Hit | Best | D1 p17 | |
| F02-W1143 | 358 | Footstep 1 | ROM | D1 p16 | |
| F02-W1144 | 358 | Metal Hit 1 | Best | D1 p17 | |
| F02-W1145 | 359 | Footstep 2 | ROM | D1 p16 | |
| F02-W1146 | 359 | Metal Hit 2 | Best | D1 p17 | |
| F02-W1147 | 360 | Click | ROM | D1 p16 | |
| F02-W1148 | 360 | Metal Hit 3 | Best | D1 p17 | |
| F02-W1149 | 361 | Bird 1 | ROM | D1 p16 | |
| F02-W1150 | 361 | Metal Hit 4 | Best | D1 p17 | |
| F02-W1151 | 362 | Bird 2 | ROM | D1 p16 | |
| F02-W1152 | 362 | Metal Hit 5 | Best | D1 p17 | |
| F02-W1153 | 363 | Dog | ROM | D1 p16 | |
| F02-W1154 | 363 | Metal Hit 6 | Best | D1 p17 | |
| F02-W1155 | 364 | Gallop | ROM | D1 p16 | |
| F02-W1156 | 364 | Metal Hit 7 | Best | D1 p17 | |
| F02-W1157 | 365 | Crickets 1 | ROM | D1 p16 | |
| F02-W1158 | 365 | Industry-Motor 1 | Best | D1 p17 | |
| F02-W1159 | 366 | Crickets 2 | ROM | D1 p16 | |
| F02-W1160 | 366 | Industry-Motor 2 | Best | D1 p17 | |
| F02-W1161 | 367 | Hart Beat | ROM | D1 p16 | |
| F02-W1162 | 367 | Industry-Drill 1 | Best | D1 p17 | |
| F02-W1163 | 368 | Punch | ROM | D1 p16 | |
| F02-W1164 | 368 | Industry-Drill 2 | Best | D1 p17 | |
| F02-W1165 | 369 | Tribe | ROM | D1 p16 | |
| F02-W1166 | 369 | Industry-Drill 3 | Best | D1 p17 | |
| F02-W1167 | 370 | GunShot | ROM | D1 p16 | |
| F02-W1168 | 370 | Industry-Tblsow | Best | D1 p17 | |
| F02-W1169 | 371 | Rainstick | ROM | D1 p16 | |
| F02-W1170 | 371 | Industry-Bowing | Best | D1 p17 | |
| F02-W1171 | 372 | Door Creak | ROM | D1 p16 | |
| F02-W1172 | 372 | Industry-Iron | Best | D1 p17 | |
| F02-W1173 | 373 | Door Slam | ROM | D1 p16 | |
| F02-W1174 | 373 | Industry-Press | Best | D1 p17 | |
| F02-W1175 | 374 | Car Engine | ROM | D1 p16 | |
| F02-W1176 | 374 | Industry-Metal | Best | D1 p17 | |
| F02-W1177 | 375 | Car Stop | ROM | D1 p16 | |
| F02-W1178 | 375 | Industry-Regist | Best | D1 p17 | |
| F02-W1179 | 376 | Car Pass | ROM | D1 p16 | |
| F02-W1180 | 376 | Industry-GIIN. | Best | D1 p17 | |
| F02-W1181 | 377 | Car Crash | ROM | D1 p16 | |
| F02-W1182 | 377 | Industry-Machine | Best | D1 p17 | |
| F02-W1183 | 378 | Train | ROM | D1 p16 | |
| F02-W1184 | 378 | Industry-Comp 1 | Best | D1 p17 | |
| F02-W1185 | 379 | Helicopter | ROM | D1 p16 | |
| F02-W1186 | 379 | Industry-Comp 2 | Best | D1 p17 | |
| F02-W1187 | 380 | Gun Shot 2 | ROM | D1 p16 | |
| F02-W1188 | 380 | Industry-Pump | Best | D1 p17 | |
| F02-W1189 | 381 | MachineGun | ROM | D1 p16 | |
| F02-W1190 | 381 | Industry-TAIHOU | Best | D1 p17 | |
| F02-W1191 | 382 | Laser Gun | ROM | D1 p16 | |
| F02-W1192 | 382 | Industry-Voice | Best | D1 p17 | |
| F02-W1193 | 383 | Explosion | ROM | D1 p16 | |
| F02-W1194 | 383 | Industry-BD | Best | D1 p17 | |
| F02-W1195 | 384 | Thunder | ROM | D1 p16 | |
| F02-W1196 | 384 | Hall Bound | Best | D1 p17 | |
| F02-W1197 | 385 | Wind | ROM | D1 p16 | |
| F02-W1198 | 385 | Glass Hit | Best | D1 p17 | |
| F02-W1199 | 386 | Stream | ROM | D1 p16 | |
| F02-W1200 | 386 | Industry-52 | Best | D1 p17 | |
| F02-W1201 | 387 | Bubble | ROM | D1 p16 | |
| F02-W1202 | 387 | Industry-69 | Best | D1 p17 | |
| F02-W1203 | 388 | ChurchBell | ROM | D1 p16 | |
| F02-W1204 | 388 | Industry-114 | Best | D1 p17 | |
| F02-W1205 | 389 | Telephone Ring | ROM | D1 p16 | |
| F02-W1206 | 389 | Industry-142 | Best | D1 p17 | |
| F02-W1207 | 390 | Space Lore | ROM | D1 p16 | |
| F02-W1208 | 391 | Swish Terra | ROM | D1 p16 | |
| F02-W1209 | 392 | Xylophone Spectr | ROM | D1 p16 | |
| F02-W1210 | 393 | Cricket Spectrum | ROM | D1 p16 | |
| F02-W1211 | 394 | Air Vortex | ROM | D1 p16 | |
| F02-W1212 | 395 | Noise-White | ROM | D1 p16 | |
| F02-W1213 | 396 | Noise-FM Mod | ROM | D1 p16 | |
| F02-W1214 | 397 | Tubular | ROM | D1 p16 | |
| F02-W1215 | 398 | Gamelan | ROM | D1 p16 | |
| F02-W1216 | 399 | Tambura | ROM | D1 p16 | |
| F02-W1217 | 400 | GtCutNois1 | ROM | D1 p16 | |
| F02-W1218 | 401 | GtCutNois2 | ROM | D1 p16 | |
| F02-W1219 | 402 | StringSlap | ROM | D1 p16 | |
| F02-W1220 | 403 | HarpGliss | ROM | D1 p16 | |
| F02-W1221 | 404 | Power Chord | ROM | D1 p16 | |
| F02-W1222 | 405 | Fret Noise | ROM | D1 p16 | |
| F02-W1223 | 406 | Dist.Slide 1 | ROM | D1 p16 | |
| F02-W1224 | 407 | Dist.Slide 2 | ROM | D1 p16 | |
| F02-W1225 | 408 | E.Guitar Pick 1 | ROM | D1 p16 | |
| F02-W1226 | 409 | E.Guitar Pick 2 | ROM | D1 p16 | |
| F02-W1227 | 410 | E.Guitar Pick 3 | ROM | D1 p16 | |
| F02-W1228 | 411 | Guitar Scratch | ROM | D1 p16 | |
| F02-W1229 | 412 | Amp Noise | ROM | D1 p16 | |
| F02-W1230 | 413 | Vox-Wah Gtr 1 | ROM | D1 p16 | |
| F02-W1231 | 414 | Vox-Wah Gtr 2 | ROM | D1 p16 | |
| F02-W1232 | 415 | Vox-Wah Gtr 3 | ROM | D1 p16 | |
| F02-W1233 | 416 | Vox-Wah Gtr 4 | ROM | D1 p16 | |

