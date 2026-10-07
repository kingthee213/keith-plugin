# FAMILY 05 - factory programs and combinations (internal reference)

Reference machine (internal name, never shipped): **Korg TR (2005)**.
Shipped label: **Family 05**.

Machine-readable source of these rows: `tools/enumerate/triton_vnl.py` (`--profile tr`) run against the Voice Name List PDF; the PDF itself is not stored in this repository.

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
| D1 | TR Voice Name List (Korg, file TR_VoiceNameList_EFG.pdf, 34 pages, created 2005-08-26) | official Korg download page, product TR, document id 1646 | https://www.korg.com/us/support/download/manual/1/211/1646/ | combinations (banks A-C), programs (banks A-D, bank G with its g(d) variant rows), drum kits with the per-key drumsample mapping, GM drum kits, multisamples (Rom, EX), drumsamples (Rom, EX) |
| D2 | TR Parameter Guide (Korg, file TR_ParamGuide.pdf, 281 pages, created 2005-10-17) | official Korg download page, product TR, document id 1645 | https://www.korg.com/us/support/download/manual/1/211/1645/ | effect algorithm catalogue (00-89) and the insert/master effect routing architecture; the internal-memory statement (470 multisamples, 33 drum kits) |
| D3 | TR Operation Guide (Korg, file TR_OpGuide_E2, 136 pages, created 2005-10-17) | official Korg download page, product TR, document id 1644 | https://www.korg.com/us/support/download/manual/1/211/1644/ | cross-check of the modes and the effect architecture; supplies no factory list of its own |

## Counts as documented

| Section | Count | Documented banks |
|---|---|---|
| Factory combinations | 384 | A, B, C |
| Factory programs | 649 | A, B, C, D, G |
| Factory drum kits | 16 | n/a |
| GM drum kits | 9 | n/a |
| Rom + EX multisamples (PCM waves) | 470 | n/a |
| Rom + EX drumsamples (PCM waves) | 518 | n/a |

- Combinations: **384** documented entries (banks A, B, C).
- Programs: **649** documented entries (banks A, B, C, D, G).
- Drum kits: **16** factory kits plus **9** GM kits - see `FAMILY-05-DRUMS.md`.
- PCM multisamples: **470** documented rows (Rom 425 + EX 45).
- PCM drumsamples: **518** documented rows (Rom 417 + EX 101).

## What the document states about its own structure (D2)

D2's appendix *Bank/Number structure for programs, combinations, drum kits, and
arpeggio patterns* prints the TR's own ranges. They agree with the lists
enumerated here, which is the cross-check for the sample totals:

- Multisamples: Rom `000-424` (425) and `EX 000-044` (45) = **470** - the same
  figure the parameter text states: "The internal memory contains 470 different
  multisamples (preset multisamples) and 33 drum kits".
- Drumsamples: Rom `000-416` (417) and `EX 000-100` (101) = **518** rows.
- Combinations: banks A, B, C only. Programs: banks A-D, G and the `g(d)` rows.
- Drum kits: 33 in the machine - `00-15 (INT)` preload kits, `16-23 (User)` user
  kits and `24-32 (GM)` GM2-compatible ROM kits. D1 publishes the 16 preload
  kits and the 9 GM kits; the 8 user kits hold no factory data, so nothing is
  listed for them here.

## How this list differs from Family 01 (TRITON)

The TR is a later, smaller machine in the same family, and its preload is its
own, not a subset of the TRITON's. Measured over the two enumerated lists:

| | Family 05 (TR) | Family 01 (TRITON) |
|---|---|---|
| Combinations | **384** (banks A-C) | **512** (banks A-D) |
| Combination names shared with Family 01 | 120 of the 384 | - |
| Programs | **649** rows (A-D 512, G 137) | **777** rows (A-D 512, G 265) |
| GM variant rows in bank G | 9 (`g(d)001`-`g(d)057`) | 137 (`g(1)`-`g(9)` / `g(d)`) |
| Program names shared with Family 01 | 456 of the 635 distinct names | - |
| Preload starts at | combi `A000 StereophonicKeys`, program `A000 Portamento Pad` | combi `A000 Romance Layers`, program `A000 Noisy Stabber` |
| Factory drum kits | **16** | **16** |
| Kit names shared with Family 01 | 9 of 16 | - |
| Kit key rows | 88 (A0-C8); kit 05 adds C#8 and D8, kit 15 adds C#8-G#8 (96) | 88 per kit |
| GM drum kits | **9**, all 9 names shared with Family 01 | **9** |
| Multisamples | **470** - Rom 425 + EX 45 | **425** |
| Multisample names shared with Family 01 | 423 of 470 | - |
| Drumsamples | **518** - Rom 417 + EX 101 | **413** |
| Drumsample names shared with Family 01 | every one of Family 01's 412 distinct names appears in this list | - |
| Insert effects | **1** (IFX) | **5** (IFX1-IFX5) |
| Master effects | 2 (MFX1-MFX2, mono-in / stereo-out, wet only) | 2 (MFX1-MFX2) |
| FX algorithms | **90** entries (`00`-`89`) | **103** entries (`000`-`102`) |

Two consequences for a later conversion unit:

- The TR's effect architecture is not the TRITON's. One insert effect means a
  Family 01 patch that uses IFX2-IFX5 has nowhere to put them; the TR's own
  catalogue also stops at `89: Reverb-Gate` where Family 01 continues to
  `102: Hold Delay`.
- The preloads overlap only partly, so Family 05 is not a duplicate bank. It
  repeats about a third of the TRITON's combination names and roughly
  two-thirds of its program names, and adds 45 multisamples and 105 drumsamples
  the TRITON does not have.

## Combinations

Category is the document's own category column. Every row is one documented factory combination.

| Shipped label | Documented name | Bank | Number | Category | Source | Note |
|---|---|---|---|---|---|---|
| F05-C0001 | StereophonicKeys | A | A000 | Keyboard | D1 p3 |  |
| F05-C0002 | Archi Prime File | A | A001 | Strings | D1 p3 |  |
| F05-C0003 | Vintage Pad | A | A002 | Pads | D1 p3 |  |
| F05-C0004 | Wonkaville | A | A003 | BassSplits | D1 p3 |  |
| F05-C0005 | Epic Escape !! | A | A004 | BassSplits | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0006 | D'Ya Wanna Ride? | A | A005 | BassSplits | D1 p3 |  |
| F05-C0007 | The Fire Inside | A | A006 | BassSplits | D1 p3 |  |
| F05-C0008 | Real Steppers | A | A007 | BassSplits | D1 p3 |  |
| F05-C0009 | Song Of Africa | A | A008 | World | D1 p3 |  |
| F05-C0010 | Trumpets&Strings | A | A009 | Orchestral | D1 p3 |  |
| F05-C0011 | Analog 101 | A | A010 | Synth | D1 p3 |  |
| F05-C0012 | Triton Voices | A | A011 | Pads | D1 p3 |  |
| F05-C0013 | Bellagio | A | A012 | Bell/Mallet/Perc | D1 p3 |  |
| F05-C0014 | Scat Blatt Phatt | A | A013 | BrassReed | D1 p3 |  |
| F05-C0015 | Reasons Unclear | A | A014 | Complex & SE | D1 p3 |  |
| F05-C0016 | Heavy Organ | A | A015 | Organ | D1 p3 |  |
| F05-C0017 | Session E.P. | A | A016 | Keyboard | D1 p3 |  |
| F05-C0018 | Detailed Strings | A | A017 | Strings | D1 p3 |  |
| F05-C0019 | People Watching | A | A018 | Pads | D1 p3 |  |
| F05-C0020 | The White Box | A | A019 | LeadSplits | D1 p3 |  |
| F05-C0021 | Fantasmagoria | A | A020 | BassSplits | D1 p3 |  |
| F05-C0022 | Urban Nature RnB | A | A021 | BassSplits | D1 p3 |  |
| F05-C0023 | Late Night Jam | A | A022 | LeadSplits | D1 p3 |  |
| F05-C0024 | Chillfactor | A | A023 | LeadSplits | D1 p3 |  |
| F05-C0025 | Bass&WhirlySplit | A | A024 | BassSplits | D1 p3 |  |
| F05-C0026 | Vibrating Woods | A | A025 | Orchestral | D1 p3 |  |
| F05-C0027 | Waiting4Sunrise | A | A026 | Pads | D1 p3 |  |
| F05-C0028 | Pink Flutes | A | A027 | LeadSplits | D1 p3 |  |
| F05-C0029 | South Beach Gtr. | A | A028 | Guitar/Plucked | D1 p3 |  |
| F05-C0030 | Pro Brass | A | A029 | BrassReed | D1 p3 |  |
| F05-C0031 | "Yo,Wasup Dude!" | A | A030 | Ds/Hits | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0032 | Med.->Tutti/Knbs | A | A031 | Organ | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0033 | Icy River Piano | A | A032 | Keyboard | D1 p4 |  |
| F05-C0034 | DynOctaveStrings | A | A033 | Strings | D1 p4 |  |
| F05-C0035 | Phases Of Angels | A | A034 | MotionSynth | D1 p4 |  |
| F05-C0036 | Dark Element | A | A035 | BassSplits | D1 p4 |  |
| F05-C0037 | Progressive Code | A | A036 | BassSplits | D1 p4 |  |
| F05-C0038 | Wave Sequencer | A | A037 | MotionSynth | D1 p4 |  |
| F05-C0039 | Jazz Step DnB | A | A038 | BassSplits | D1 p4 |  |
| F05-C0040 | <T-nage-Dancing> | A | A039 | BassSplits | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0041 | Indian Ocean | A | A040 | World | D1 p4 |  |
| F05-C0042 | Delicate Winds | A | A041 | BrassReed | D1 p4 |  |
| F05-C0043 | Chord Memory | A | A042 | Synth | D1 p4 |  |
| F05-C0044 | Stereo Voices | A | A043 | Pads | D1 p4 |  |
| F05-C0045 | Vaderian Bells | A | A044 | Bell/Mallet/Perc | D1 p4 |  |
| F05-C0046 | Velo Orchestra | A | A045 | Orchestral | D1 p4 |  |
| F05-C0047 | Millennium Files | A | A046 | Complex & SE | D1 p4 |  |
| F05-C0048 | Dirty BX-3 Organ | A | A047 | Organ | D1 p4 |  |
| F05-C0049 | Lush Layers | A | A048 | Keyboard | D1 p4 |  |
| F05-C0050 | Bell Animations | A | A049 | Bell/Mallet/Perc | D1 p4 |  |
| F05-C0051 | /\Gods Bathtub/\ | A | A050 | MotionSynth | D1 p4 |  |
| F05-C0052 | Funky Keys | A | A051 | Keyboard | D1 p4 |  |
| F05-C0053 | Gimme a ring... | A | A052 | BassSplits | D1 p4 |  |
| F05-C0054 | Bunnies In Love | A | A053 | BassSplits | D1 p4 |  |
| F05-C0055 | Rap Bass Split | A | A054 | BassSplits | D1 p4 |  |
| F05-C0056 | Tremolo Guitar | A | A055 | BassSplits | D1 p4 |  |
| F05-C0057 | Mute Tp & More | A | A056 | LeadSplits | D1 p4 |  |
| F05-C0058 | Orchestral Brass | A | A057 | Orchestral | D1 p4 |  |
| F05-C0059 | REZ IS KING | A | A058 | Synth | D1 p4 |  |
| F05-C0060 | *Pad Tropolis* | A | A059 | Pads | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0061 | Pad Guitar SW1 | A | A060 | Guitar/Plucked | D1 p4 |  |
| F05-C0062 | Oboe Split | A | A061 | LeadSplits | D1 p4 |  |
| F05-C0063 | Speed Racer | A | A062 | Ds/Hits | D1 p4 |  |
| F05-C0064 | Jazz/GospelOrgan | A | A063 | Organ | D1 p4 |  |
| F05-C0065 | Piano/WideString | A | A064 | Keyboard | D1 p5 |  |
| F05-C0066 | Slow Strings + @ | A | A065 | Strings | D1 p5 |  |
| F05-C0067 | LargeSqwishiThag | A | A066 | MotionSynth | D1 p5 |  |
| F05-C0068 | Back & 4th | A | A067 | LeadSplits | D1 p5 |  |
| F05-C0069 | US Garage House | A | A068 | BassSplits | D1 p5 |  |
| F05-C0070 | Chic 2 Chic | A | A069 | BassSplits | D1 p5 |  |
| F05-C0071 | * Time Crisis * | A | A070 | BassSplits | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0072 | >AutoRave< Knobs | A | A071 | BassSplits | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0073 | Accordion (SW) | A | A072 | World | D1 p5 |  |
| F05-C0074 | Strings & Winds | A | A073 | Orchestral | D1 p5 |  |
| F05-C0075 | UltimateAnaPizz | A | A074 | Synth | D1 p5 |  |
| F05-C0076 | TheNightBefore.. | A | A075 | Pads | D1 p5 |  |
| F05-C0077 | Heaven's Bells | A | A076 | Bell/Mallet/Perc | D1 p5 |  |
| F05-C0078 | Warm Bars | A | A077 | Organ | D1 p5 |  |
| F05-C0079 | *Dying Star* | A | A078 | Complex & SE | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0080 | 3rd Harm.on Knob | A | A079 | Organ | D1 p5 |  |
| F05-C0081 | Cutting Clav | A | A080 | Keyboard | D1 p5 |  |
| F05-C0082 | Pro Strings | A | A081 | Strings | D1 p5 |  |
| F05-C0083 | Northern Stars | A | A082 | MotionSynth | D1 p5 |  |
| F05-C0084 | FutureJazztronic | A | A083 | BassSplits | D1 p5 |  |
| F05-C0085 | Killer Smile | A | A084 | BassSplits | D1 p5 |  |
| F05-C0086 | Space Wah Bass | A | A085 | BassSplits | D1 p5 |  |
| F05-C0087 | Missing Android | A | A086 | BassSplits | D1 p5 |  |
| F05-C0088 | Dry as Garage | A | A087 | BassSplits | D1 p5 |  |
| F05-C0089 | At The Copa! | A | A088 | BassSplits | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0090 | Flutambient | A | A089 | LeadSplits | D1 p5 |  |
| F05-C0091 | *Movin' Stars* | A | A090 | Synth | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0092 | Judgement Day | A | A091 | Pads | D1 p5 |  |
| F05-C0093 | BrothersAcoustic | A | A092 | Guitar/Plucked | D1 p5 |  |
| F05-C0094 | Cascade Bells | A | A093 | Bell/Mallet/Perc | D1 p5 |  |
| F05-C0095 | Talk to Galileo | A | A094 | MotionSynth | D1 p5 |  |
| F05-C0096 | Med.Mixture/Knbs | A | A095 | Organ | D1 p5 |  |
| F05-C0097 | Harmonic Whisper | A | A096 | Keyboard | D1 p6 |  |
| F05-C0098 | Detached Bows | A | A097 | Strings | D1 p6 |  |
| F05-C0099 | Shh-I'm Sweeping | A | A098 | MotionSynth | D1 p6 |  |
| F05-C0100 | Do U Funk? | A | A099 | BassSplits | D1 p6 |  |
| F05-C0101 | Jazz Bass/Gtr. | A | A100 | BassSplits | D1 p6 |  |
| F05-C0102 | Trash Aliens | A | A101 | Complex & SE | D1 p6 |  |
| F05-C0103 | Groove'n Soloman | A | A102 | BassSplits | D1 p6 |  |
| F05-C0104 | *ResoChord Pad* | A | A103 | Pads | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0105 | Magic Kingdom | A | A104 | World | D1 p6 |  |
| F05-C0106 | >Smaller Ensbl.< | A | A105 | Strings | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0107 | =Sync to Drum= | A | A106 | Synth | D1 p6 |  |
| F05-C0108 | Falling Leaves | A | A107 | MotionSynth | D1 p6 |  |
| F05-C0109 | Bell Amis | A | A108 | Bell/Mallet/Perc | D1 p6 |  |
| F05-C0110 | Ballad Organ | A | A109 | Organ | D1 p6 |  |
| F05-C0111 | Bass & PianoPad | A | A110 | BassSplits | D1 p6 |  |
| F05-C0112 | Upper&Lower Org. | A | A111 | Organ | D1 p6 |  |
| F05-C0113 | Dark Bell EP | A | A112 | Keyboard | D1 p6 |  |
| F05-C0114 | OrchestraStrings | A | A113 | Strings | D1 p6 |  |
| F05-C0115 | StreetCatScratch | A | A114 | LeadSplits | D1 p6 |  |
| F05-C0116 | ** 4:17 a.m. ** | A | A115 | BassSplits | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0117 | Talk 2 Me | A | A116 | BassSplits | D1 p6 |  |
| F05-C0118 | >Relax Pad< | A | A117 | Complex & SE | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0119 | Digital'n'Bass | A | A118 | Complex & SE | D1 p6 |  |
| F05-C0120 | ) Beat Boy ( | A | A119 | BassSplits | D1 p6 |  |
| F05-C0121 | Old Tubes! | A | A120 | Organ | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0122 | DynOrchestra 2 | A | A121 | Orchestral | D1 p6 |  |
| F05-C0123 | Huge Ober Sweep | A | A122 | Synth | D1 p6 |  |
| F05-C0124 | Essential Flute | A | A123 | LeadSplits | D1 p6 |  |
| F05-C0125 | 12-Strings (Arp) | A | A124 | Guitar/Plucked | D1 p6 |  |
| F05-C0126 | It's Sine Tine! | A | A125 | Synth | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0127 | DrumConstruction | A | A126 | Ds/Hits | D1 p6 |  |
| F05-C0128 | Gospel Organ | A | A127 | Organ | D1 p6 |  |
| F05-C0129 | Amp Driven Wurly | B | B000 | Keyboard | D1 p7 |  |
| F05-C0130 | Dynamic Strings | B | B001 | Strings | D1 p7 |  |
| F05-C0131 | //Feel ;-) Pad// | B | B002 | MotionSynth | D1 p7 |  |
| F05-C0132 | PartyReggaeMahn | B | B003 | BassSplits | D1 p7 |  |
| F05-C0133 | Trancy Euphoria | B | B004 | BassSplits | D1 p7 |  |
| F05-C0134 | SynthBass & Pitz | B | B005 | BassSplits | D1 p7 |  |
| F05-C0135 | Tribal Drum 'n.. | B | B006 | BassSplits | D1 p7 |  |
| F05-C0136 | GroovyGarage | B | B007 | BassSplits | D1 p7 |  |
| F05-C0137 | Isle Of Indigo | B | B008 | World | D1 p7 |  |
| F05-C0138 | Studio Orchestra | B | B009 | Orchestral | D1 p7 |  |
| F05-C0139 | Warm & Snappy | B | B010 | Synth | D1 p7 |  |
| F05-C0140 | Breath Monster | B | B011 | Pads | D1 p7 |  |
| F05-C0141 | ChristmasStrings | B | B012 | Bell/Mallet/Perc | D1 p7 |  |
| F05-C0142 | Big Jazz Band | B | B013 | BrassReed | D1 p7 |  |
| F05-C0143 | All Was Lost | B | B014 | Complex & SE | D1 p7 |  |
| F05-C0144 | Rock Organ | B | B015 | Organ | D1 p7 |  |
| F05-C0145 | Hip Hop Clav | B | B016 | Keyboard | D1 p7 |  |
| F05-C0146 | Bowed Strings | B | B017 | Strings | D1 p7 |  |
| F05-C0147 | Sailing | B | B018 | MotionSynth | D1 p7 |  |
| F05-C0148 | E.Piano/Brass | B | B019 | LeadSplits | D1 p7 |  |
| F05-C0149 | Galactic Artcore | B | B020 | BassSplits | D1 p7 |  |
| F05-C0150 | Sweet RnB Vibe | B | B021 | BassSplits | D1 p7 |  |
| F05-C0151 | HardHouse | B | B022 | BassSplits | D1 p7 |  |
| F05-C0152 | ClassicGarage | B | B023 | Synth | D1 p7 |  |
| F05-C0153 | You Will Love It | B | B024 | BassSplits | D1 p7 |  |
| F05-C0154 | Presto & Vivace | B | B025 | Orchestral | D1 p7 |  |
| F05-C0155 | Fuji-yama | B | B026 | Pads | D1 p7 |  |
| F05-C0156 | Night Reeds | B | B027 | LeadSplits | D1 p7 |  |
| F05-C0157 | The Guitarist | B | B028 | Guitar/Plucked | D1 p7 |  |
| F05-C0158 | Warm Brass Ens. | B | B029 | BrassReed | D1 p7 |  |
| F05-C0159 | Modern Orchestra | B | B030 | Orchestral | D1 p7 |  |
| F05-C0160 | Registrations | B | B031 | Organ | D1 p7 |  |
| F05-C0161 | Soft Piano Pad | B | B032 | Keyboard | D1 p8 |  |
| F05-C0162 | >Movie Strings< | B | B033 | Strings | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0163 | Forget-Me-Not | B | B034 | MotionSynth | D1 p8 |  |
| F05-C0164 | -SWEET GROOVE- | B | B035 | BassSplits | D1 p8 |  |
| F05-C0165 | The16th Strummer | B | B036 | BassSplits | D1 p8 |  |
| F05-C0166 | Retrogene | B | B037 | BassSplits | D1 p8 |  |
| F05-C0167 | Stationary Split | B | B038 | BassSplits | D1 p8 |  |
| F05-C0168 | / Nice & Easy / | B | B039 | Pads | D1 p8 |  |
| F05-C0169 | Delhicacy (vel) | B | B040 | World | D1 p8 |  |
| F05-C0170 | FrenchEnsemble | B | B041 | Orchestral | D1 p8 |  |
| F05-C0171 | 80's Analogues | B | B042 | Synth | D1 p8 |  |
| F05-C0172 | Universal Choir | B | B043 | Pads | D1 p8 |  |
| F05-C0173 | Deep Crystals | B | B044 | Bell/Mallet/Perc | D1 p8 |  |
| F05-C0174 | Polished Brass | B | B045 | BrassReed | D1 p8 |  |
| F05-C0175 | The Rain Forest | B | B046 | Complex & SE | D1 p8 |  |
| F05-C0176 | Super Jazz Organ | B | B047 | Organ | D1 p8 |  |
| F05-C0177 | Stream E.Piano | B | B048 | Keyboard | D1 p8 |  |
| F05-C0178 | Horns & Strings | B | B049 | LeadSplits | D1 p8 |  |
| F05-C0179 | Kromatica | B | B050 | MotionSynth | D1 p8 |  |
| F05-C0180 | Club Movement | B | B051 | Keyboard | D1 p8 |  |
| F05-C0181 | POP Princessa | B | B052 | BassSplits | D1 p8 |  |
| F05-C0182 | Breaks Step | B | B053 | Complex & SE | D1 p8 |  |
| F05-C0183 | Brass Jazz Funk | B | B054 | LeadSplits | D1 p8 |  |
| F05-C0184 | FunkedUpGarage | B | B055 | BassSplits | D1 p8 |  |
| F05-C0185 | Journey To Bali | B | B056 | World | D1 p8 |  |
| F05-C0186 | Gotham Snd.Track | B | B057 | Orchestral | D1 p8 |  |
| F05-C0187 | LushiousClavinus | B | B058 | Synth | D1 p8 |  |
| F05-C0188 | Classic Combi | B | B059 | Orchestral | D1 p8 |  |
| F05-C0189 | LargeScaleGuitar | B | B060 | Guitar/Plucked | D1 p8 |  |
| F05-C0190 | Velo Orch Winds | B | B061 | Orchestral | D1 p8 |  |
| F05-C0191 | Carnival Parade | B | B062 | Ds/Hits | D1 p8 |  |
| F05-C0192 | Pipes on SW&Knbs | B | B063 | Organ | D1 p8 |  |
| F05-C0193 | Layered A.Piano | B | B064 | Keyboard | D1 p9 |  |
| F05-C0194 | ClassicAnaString | B | B065 | Strings | D1 p9 |  |
| F05-C0195 | Tritium | B | B066 | MotionSynth | D1 p9 |  |
| F05-C0196 | > Movies Theme < | B | B067 | MotionSynth | D1 p9 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0197 | Club NYC | B | B068 | Complex & SE | D1 p9 |  |
| F05-C0198 | Organic Beats | B | B069 | BassSplits | D1 p9 |  |
| F05-C0199 | Ethereal Piano | B | B070 | Keyboard | D1 p9 |  |
| F05-C0200 | In The Pocket | B | B071 | Synth | D1 p9 |  |
| F05-C0201 | Fisatic Dreams | B | B072 | World | D1 p9 |  |
| F05-C0202 | Dry Chamber Ens. | B | B073 | Orchestral | D1 p9 |  |
| F05-C0203 | Sigma Strings | B | B074 | Synth | D1 p9 |  |
| F05-C0204 | Sequence Pad | B | B075 | MotionSynth | D1 p9 |  |
| F05-C0205 | -Fourier's Idea- | B | B076 | Bell/Mallet/Perc | D1 p9 |  |
| F05-C0206 | Knob as Drawbar1 | B | B077 | Organ | D1 p9 |  |
| F05-C0207 | Narita Airprt C4 | B | B078 | Complex & SE | D1 p9 |  |
| F05-C0208 | JazzyChords/Knbs | B | B079 | Organ | D1 p9 |  |
| F05-C0209 | Village Pump | B | B080 | Keyboard | D1 p9 |  |
| F05-C0210 | AfterRainStrings | B | B081 | Strings | D1 p9 |  |
| F05-C0211 | <Smoky Soft Pad> | B | B082 | MotionSynth | D1 p9 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0212 | Hidden Rhythm | B | B083 | LeadSplits | D1 p9 |  |
| F05-C0213 | Streetwalk | B | B084 | BassSplits | D1 p9 |  |
| F05-C0214 | Clavi-Jammer | B | B085 | BassSplits | D1 p9 |  |
| F05-C0215 | Slow Motion Man | B | B086 | MotionSynth | D1 p9 |  |
| F05-C0216 | Elektro Express | B | B087 | BassSplits | D1 p9 |  |
| F05-C0217 | Peasant Song | B | B088 | World | D1 p9 |  |
| F05-C0218 | Sweet Dyn-Orch. | B | B089 | Orchestral | D1 p9 |  |
| F05-C0219 | Miami Virtue | B | B090 | Synth | D1 p9 |  |
| F05-C0220 | Strings Of Silk | B | B091 | Strings | D1 p9 |  |
| F05-C0221 | My Baby's Asleep | B | B092 | Guitar/Plucked | D1 p9 |  |
| F05-C0222 | Going Home @ 3am | B | B093 | Bell/Mallet/Perc | D1 p9 |  |
| F05-C0223 | Arp Factory 1 | B | B094 | MotionSynth | D1 p9 |  |
| F05-C0224 | Pipes on Knob | B | B095 | Organ | D1 p9 |  |
| F05-C0225 | Layered M1 Piano | B | B096 | Keyboard | D1 p10 |  |
| F05-C0226 | CinematicStrings | B | B097 | Strings | D1 p10 |  |
| F05-C0227 | Spheoretical | B | B098 | MotionSynth | D1 p10 |  |
| F05-C0228 | Steely Keys | B | B099 | Synth | D1 p10 |  |
| F05-C0229 | Lonely Moon | B | B100 | BassSplits | D1 p10 |  |
| F05-C0230 | Freewheeling LFO | B | B101 | BassSplits | D1 p10 |  |
| F05-C0231 | Mystic Presence | B | B102 | MotionSynth | D1 p10 |  |
| F05-C0232 | > Housemade < | B | B103 | LeadSplits | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0233 | Snap!ReggaeOrgan | B | B104 | Organ | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0234 | Expressive Orch | B | B105 | Orchestral | D1 p10 |  |
| F05-C0235 | Anabell Rain New | B | B106 | Pads | D1 p10 |  |
| F05-C0236 | Simple Pad | B | B107 | Pads | D1 p10 |  |
| F05-C0237 | Bella Proach | B | B108 | Bell/Mallet/Perc | D1 p10 |  |
| F05-C0238 | Knob as Drawbar2 | B | B109 | Organ | D1 p10 |  |
| F05-C0239 | Living Cycles | B | B110 | BassSplits | D1 p10 |  |
| F05-C0240 | Velo*Perc*Organ | B | B111 | Organ | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0241 | LayerEP&Pad/Knbs | B | B112 | Keyboard | D1 p10 |  |
| F05-C0242 | Splittavox | B | B113 | BassSplits | D1 p10 |  |
| F05-C0243 | **Weightless** | B | B114 | MotionSynth | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0244 | Bass On Drums | B | B115 | Ds/Hits | D1 p10 |  |
| F05-C0245 | > Flimmer Line < | B | B116 | Pads | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0246 | Love to Play Pad | B | B117 | Complex & SE | D1 p10 |  |
| F05-C0247 | ErraJ si Gnivil | B | B118 | MotionSynth | D1 p10 |  |
| F05-C0248 | Dem is Well Hard | B | B119 | BassSplits | D1 p10 |  |
| F05-C0249 | Quidam | B | B120 | World | D1 p10 |  |
| F05-C0250 | Dream Orchestra | B | B121 | Orchestral | D1 p10 |  |
| F05-C0251 | Poly Sixual | B | B122 | Synth | D1 p10 |  |
| F05-C0252 | -> Solo @ Pad -> | B | B123 | LeadSplits | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0253 | Classical Guitar | B | B124 | Guitar/Plucked | D1 p10 |  |
| F05-C0254 | ] Glass Shop [ | B | B125 | Bell/Mallet/Perc | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0255 | "Dude,That's SICK" | B | B126 | Complex & SE | D1 p10 |  |
| F05-C0256 | Gospel Church | B | B127 | Organ | D1 p10 |  |
| F05-C0257 | Ballad Layer EP | C | C000 | Keyboard | D1 p11 |  |
| F05-C0258 | Super Strings | C | C001 | Strings | D1 p11 |  |
| F05-C0259 | Squarehead | C | C002 | MotionSynth | D1 p11 |  |
| F05-C0260 | Ballad Grand | C | C003 | Keyboard | D1 p11 |  |
| F05-C0261 | Play The Trance | C | C004 | LeadSplits | D1 p11 |  |
| F05-C0262 | The ILL Chill | C | C005 | BassSplits | D1 p11 |  |
| F05-C0263 | Nu Cyber Breaks | C | C006 | LeadSplits | D1 p11 |  |
| F05-C0264 | Strumin' 2step | C | C007 | BassSplits | D1 p11 |  |
| F05-C0265 | World Atmosphere | C | C008 | World | D1 p11 |  |
| F05-C0266 | Rhythms & Bows | C | C009 | Orchestral | D1 p11 |  |
| F05-C0267 | Analog Clavical | C | C010 | Synth | D1 p11 |  |
| F05-C0268 | =Liquified= | C | C011 | Pads | D1 p11 |  |
| F05-C0269 | Random Blocks | C | C012 | Bell/Mallet/Perc | D1 p11 |  |
| F05-C0270 | Sop/Alt/Tnr/Bari | C | C013 | BrassReed | D1 p11 |  |
| F05-C0271 | One Bad Monster | C | C014 | Complex & SE | D1 p11 |  |
| F05-C0272 | Comp Organ/Knobs | C | C015 | Organ | D1 p11 |  |
| F05-C0273 | Damper Wah Clav | C | C016 | Keyboard | D1 p11 |  |
| F05-C0274 | 3-V.Switch-Strgs | C | C017 | Strings | D1 p11 |  |
| F05-C0275 | (Where are You?) | C | C018 | MotionSynth | D1 p11 |  |
| F05-C0276 | Acid Groover... | C | C019 | BassSplits | D1 p11 |  |
| F05-C0277 | Paradise Cycle | C | C020 | BassSplits | D1 p11 |  |
| F05-C0278 | - Groovy Jam - | C | C021 | BassSplits | D1 p11 |  |
| F05-C0279 | < Sneakin > | C | C022 | BassSplits | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0280 | Elec Laboratory | C | C023 | BassSplits | D1 p11 |  |
| F05-C0281 | Velo Psychedelic | C | C024 | Synth | D1 p11 |  |
| F05-C0282 | DynOrchestra 1 | C | C025 | Orchestral | D1 p11 |  |
| F05-C0283 | WIDE StringPad | C | C026 | Strings | D1 p11 |  |
| F05-C0284 | PerformingGuitar | C | C027 | Guitar/Plucked | D1 p11 |  |
| F05-C0285 | Mountainside | C | C028 | Guitar/Plucked | D1 p11 |  |
| F05-C0286 | Pompus Brass | C | C029 | Orchestral | D1 p11 |  |
| F05-C0287 | Psycho Orchestra | C | C030 | Ds/Hits | D1 p11 |  |
| F05-C0288 | Orchestral Pipes | C | C031 | Organ | D1 p11 |  |
| F05-C0289 | Tiney Harmonic | C | C032 | Keyboard | D1 p12 |  |
| F05-C0290 | Violin Section | C | C033 | Strings | D1 p12 |  |
| F05-C0291 | What GEOS Around | C | C034 | MotionSynth | D1 p12 |  |
| F05-C0292 | * Blood Cooker * | C | C035 | BassSplits | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0293 | Manic 3AM | C | C036 | BassSplits | D1 p12 |  |
| F05-C0294 | OffToHollywood.. | C | C037 | BassSplits | D1 p12 |  |
| F05-C0295 | Spiral Design | C | C038 | BassSplits | D1 p12 |  |
| F05-C0296 | Garage'r'us! | C | C039 | BassSplits | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0297 | World Winds | C | C040 | World | D1 p12 |  |
| F05-C0298 | Film Tools | C | C041 | Orchestral | D1 p12 |  |
| F05-C0299 | BleepageForDays | C | C042 | Synth | D1 p12 |  |
| F05-C0300 | VoicesFromHeaven | C | C043 | Pads | D1 p12 |  |
| F05-C0301 | Emtim Bells | C | C044 | Bell/Mallet/Perc | D1 p12 |  |
| F05-C0302 | French Brass | C | C045 | BrassReed | D1 p12 |  |
| F05-C0303 | Sargasso Sea | C | C046 | Complex & SE | D1 p12 |  |
| F05-C0304 | BX-3 Click Organ | C | C047 | Organ | D1 p12 |  |
| F05-C0305 | Real EP & Pad | C | C048 | Keyboard | D1 p12 |  |
| F05-C0306 | Strings/Brass | C | C049 | LeadSplits | D1 p12 |  |
| F05-C0307 | isabellacoola | C | C050 | MotionSynth | D1 p12 |  |
| F05-C0308 | Session Piano | C | C051 | Keyboard | D1 p12 |  |
| F05-C0309 | >Mutant Hop<Knbs | C | C052 | Synth | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0310 | Echo Jamm | C | C053 | BassSplits | D1 p12 |  |
| F05-C0311 | <<Jenindaze>> | C | C054 | Synth | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0312 | -BAD Luck- | C | C055 | BassSplits | D1 p12 |  |
| F05-C0313 | Alchemy Layer | C | C056 | Guitar/Plucked | D1 p12 |  |
| F05-C0314 | Orchestra | C | C057 | Orchestral | D1 p12 |  |
| F05-C0315 | Hybrid BellPad | C | C058 | Synth | D1 p12 |  |
| F05-C0316 | Healing Pad | C | C059 | Pads | D1 p12 |  |
| F05-C0317 | Strumming Guitar | C | C060 | Guitar/Plucked | D1 p12 |  |
| F05-C0318 | RomanticSparkles | C | C061 | Pads | D1 p12 |  |
| F05-C0319 | Laid-Back-Split | C | C062 | BassSplits | D1 p12 |  |
| F05-C0320 | Holy Pipes | C | C063 | Organ | D1 p12 |  |
| F05-C0321 | Modern Piano | C | C064 | Keyboard | D1 p13 |  |
| F05-C0322 | Golden Strings | C | C065 | Strings | D1 p13 |  |
| F05-C0323 | FourStar*Choice | C | C066 | MotionSynth | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0324 | Abstract Hop | C | C067 | BassSplits | D1 p13 |  |
| F05-C0325 | Invisible Sun | C | C068 | BassSplits | D1 p13 |  |
| F05-C0326 | * `tude.com * | C | C069 | BassSplits | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0327 | D'nB 'n'AllMySon | C | C070 | Complex & SE | D1 p13 |  |
| F05-C0328 | ClubbinGarage | C | C071 | BassSplits | D1 p13 |  |
| F05-C0329 | CaribicSituation | C | C072 | World | D1 p13 |  |
| F05-C0330 | Small Orchestra | C | C073 | Orchestral | D1 p13 |  |
| F05-C0331 | Analogue Blatter | C | C074 | Synth | D1 p13 |  |
| F05-C0332 | Symphonic Voices | C | C075 | Pads | D1 p13 |  |
| F05-C0333 | Bellish Pad | C | C076 | Bell/Mallet/Perc | D1 p13 |  |
| F05-C0334 | Dirty Drawbars | C | C077 | Organ | D1 p13 |  |
| F05-C0335 | Trance Split | C | C078 | Ds/Hits | D1 p13 |  |
| F05-C0336 | Old Vox Organ | C | C079 | Organ | D1 p13 |  |
| F05-C0337 | Big Bottom Clav | C | C080 | Keyboard | D1 p13 |  |
| F05-C0338 | Smoothie Strings | C | C081 | Strings | D1 p13 |  |
| F05-C0339 | || TRANSIT || | C | C082 | MotionSynth | D1 p13 |  |
| F05-C0340 | Tranzation/Knobs | C | C083 | BassSplits | D1 p13 |  |
| F05-C0341 | Trancendental | C | C084 | BassSplits | D1 p13 |  |
| F05-C0342 | Trick House | C | C085 | BassSplits | D1 p13 |  |
| F05-C0343 | Petit Lounge | C | C086 | Organ | D1 p13 |  |
| F05-C0344 | Funky Moves | C | C087 | Complex & SE | D1 p13 |  |
| F05-C0345 | Shogun | C | C088 | World | D1 p13 |  |
| F05-C0346 | Serenade | C | C089 | Orchestral | D1 p13 |  |
| F05-C0347 | Syzzle Synth | C | C090 | Synth | D1 p13 |  |
| F05-C0348 | Solo+Arp &IcePad | C | C091 | LeadSplits | D1 p13 |  |
| F05-C0349 | -Hybr.Guit&Arp.- | C | C092 | Guitar/Plucked | D1 p13 |  |
| F05-C0350 | Angelic Bells | C | C093 | Bell/Mallet/Perc | D1 p13 |  |
| F05-C0351 | Arp Factory 2 | C | C094 | MotionSynth | D1 p13 |  |
| F05-C0352 | Pipes&Choir SW | C | C095 | Organ | D1 p13 |  |
| F05-C0353 | Piano &Strings | C | C096 | Keyboard | D1 p14 |  |
| F05-C0354 | {Octave-Strings} | C | C097 | Strings | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0355 | Respirator | C | C098 | MotionSynth | D1 p14 |  |
| F05-C0356 | Rock-tave Gtr | C | C099 | LeadSplits | D1 p14 |  |
| F05-C0357 | Zero Sleep | C | C100 | BassSplits | D1 p14 |  |
| F05-C0358 | Rhythm Of Asia | C | C101 | World | D1 p14 |  |
| F05-C0359 | Stutter Pad | C | C102 | MotionSynth | D1 p14 |  |
| F05-C0360 | Strut Break | C | C103 | BassSplits | D1 p14 |  |
| F05-C0361 | Delhi/gate | C | C104 | Synth | D1 p14 |  |
| F05-C0362 | OrchestraMaximus | C | C105 | Orchestral | D1 p14 |  |
| F05-C0363 | Alpha Mega Synth | C | C106 | Synth | D1 p14 |  |
| F05-C0364 | The Night Sky | C | C107 | Pads | D1 p14 |  |
| F05-C0365 | Stereo Mallets | C | C108 | Bell/Mallet/Perc | D1 p14 |  |
| F05-C0366 | Sax Banquet | C | C109 | BrassReed | D1 p14 |  |
| F05-C0367 | Grungie Vibe Jam | C | C110 | Complex & SE | D1 p14 |  |
| F05-C0368 | Melon Flute | C | C111 | BrassReed | D1 p14 |  |
| F05-C0369 | Shiny DigiPiano | C | C112 | Keyboard | D1 p14 |  |
| F05-C0370 | Handmade Jazz | C | C113 | BassSplits | D1 p14 |  |
| F05-C0371 | Millennium Pad | C | C114 | MotionSynth | D1 p14 |  |
| F05-C0372 | Chewing Gun | C | C115 | Synth | D1 p14 |  |
| F05-C0373 | OrganVel->Brass | C | C116 | Organ | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0374 | < Wah-Solo-Jam > | C | C117 | LeadSplits | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0375 | Acid Clav | C | C118 | Keyboard | D1 p14 |  |
| F05-C0376 | Kleen Guitah | C | C119 | Guitar/Plucked | D1 p14 |  |
| F05-C0377 | *Dance with me!* | C | C120 | MotionSynth | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F05-C0378 | Bows&KnobHarpsi | C | C121 | Orchestral | D1 p14 |  |
| F05-C0379 | Noisy Funk Synth | C | C122 | Synth | D1 p14 |  |
| F05-C0380 | Oboe & Strings | C | C123 | LeadSplits | D1 p14 |  |
| F05-C0381 | Vintage12 String | C | C124 | Guitar/Plucked | D1 p14 |  |
| F05-C0382 | Big Bad Brass | C | C125 | BrassReed | D1 p14 |  |
| F05-C0383 | Analog Baff -Y | C | C126 | Ds/Hits | D1 p14 |  |
| F05-C0384 | Init.Combination | C | C127 | Rhythmic Pattern | D1 p14 |  |

## Programs

Type is the document's own single/double program flag. Bank G holds the GM bank; its `g(1)`-`g(9)` / `g(d)` rows are the document's own variant entries and are listed here exactly as printed.

| Shipped label | Documented name | Bank | Number | Type | Category | Source | Note |
|---|---|---|---|---|---|---|---|
| F05-P0001 | Portamento Pad | A | A000 | double | SlowSynth | D1 p15 |  |
| F05-P0002 | Polysix Pad | A | A064 | double | FastSynth | D1 p15 |  |
| F05-P0003 | StereoPiano mf/f | A | A001 | double | Keyboard | D1 p15 |  |
| F05-P0004 | M1 Piano | A | A065 | single | Keyboard | D1 p15 |  |
| F05-P0005 | Centerfuge | A | A002 | double | Hit/Arpg | D1 p15 |  |
| F05-P0006 | Chord Trigger | A | A066 | double | Hit/Arpg | D1 p15 |  |
| F05-P0007 | Session Strings | A | A003 | double | Strings | D1 p15 |  |
| F05-P0008 | Hybrid Strings | A | A067 | double | Strings | D1 p15 |  |
| F05-P0009 | Mashed Kit | A | A004 | double | Drums | D1 p15 |  |
| F05-P0010 | Mega Drum Hit | A | A068 | double | Drums | D1 p15 |  |
| F05-P0011 | HardPulse Bass | A | A005 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0012 | M1 Organ Bass | A | A069 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0013 | Session Brass | A | A006 | double | Brass | D1 p15 |  |
| F05-P0014 | Velo Resonant | A | A070 | double | FastSynth | D1 p15 |  |
| F05-P0015 | Jazz Organ 1 | A | A007 | double | Organ | D1 p15 |  |
| F05-P0016 | Additive Organ | A | A071 | double | Organ | D1 p15 |  |
| F05-P0017 | Vibraphone 2 | A | A008 | double | Bell/Mallet | D1 p15 |  |
| F05-P0018 | Digital Bells 1 | A | A072 | double | Bell/Mallet | D1 p15 |  |
| F05-P0019 | Saw Strings | A | A009 | double | SlowSynth | D1 p15 |  |
| F05-P0020 | ClassicAnaPad | A | A073 | double | SlowSynth | D1 p15 |  |
| F05-P0021 | Saw OD Bass/Lead | A | A010 | single | LeadSynth | D1 p15 |  |
| F05-P0022 | DetunedSuperLead | A | A074 | double | LeadSynth | D1 p15 |  |
| F05-P0023 | SiniSTAR Bass | A | A011 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0024 | Bass-Depot | A | A075 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0025 | Nylon Guitar 2 | A | A012 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0026 | Processed E.Gtr. | A | A076 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0027 | Ping/Pong Synth | A | A013 | double | MotionSynth | D1 p15 |  |
| F05-P0028 | LightOfTheSource | A | A077 | double | MotionSynth | D1 p15 |  |
| F05-P0029 | Harmonica-Wah | A | A014 | single | Woodwind/Reed | D1 p15 |  |
| F05-P0030 | Classic Tines | A | A078 | double | Keyboard | D1 p15 |  |
| F05-P0031 | THEY were here | A | A015 | double | SE | D1 p15 |  |
| F05-P0032 | Big Break Down | A | A079 | double | SE | D1 p15 |  |
| F05-P0033 | Fast Saw Synth | A | A016 | double | FastSynth | D1 p15 |  |
| F05-P0034 | Paddy Whack | A | A080 | double | FastSynth | D1 p15 |  |
| F05-P0035 | EP Bright mf/f | A | A017 | single | Keyboard | D1 p15 |  |
| F05-P0036 | Analog E.Piano | A | A081 | double | Keyboard | D1 p15 |  |
| F05-P0037 | PunchBowl HIT | A | A018 | double | Hit/Arpg | D1 p15 |  |
| F05-P0038 | Chipper Dayglow | A | A082 | double | Hit/Arpg | D1 p15 |  |
| F05-P0039 | Tape Strings | A | A019 | double | Strings | D1 p15 |  |
| F05-P0040 | Acid Square Bass | A | A083 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0041 | Standard Kit 3 | A | A020 | double | Drums | D1 p15 |  |
| F05-P0042 | Filter Drums | A | A084 | double | Drums | D1 p15 |  |
| F05-P0043 | Velo Scat Voices | A | A021 | double | Vocal/Airy | D1 p15 |  |
| F05-P0044 | Digital Voice | A | A085 | double | Vocal/Airy | D1 p15 |  |
| F05-P0045 | Trumpet Duo | A | A022 | single | Brass | D1 p15 |  |
| F05-P0046 | Trumpets&Brass | A | A086 | double | Brass | D1 p15 |  |
| F05-P0047 | BX3 Velo Switch | A | A023 | double | Organ | D1 p15 |  |
| F05-P0048 | Velo Pulse Clav | A | A087 | double | FastSynth | D1 p15 |  |
| F05-P0049 | Analog Bells | A | A024 | double | Bell/Mallet | D1 p15 |  |
| F05-P0050 | Ensemble Bell | A | A088 | single | Bell/Mallet | D1 p15 |  |
| F05-P0051 | Overture Pad | A | A025 | double | SlowSynth | D1 p15 |  |
| F05-P0052 | ASineOfTheTimes | A | A089 | double | SlowSynth | D1 p15 |  |
| F05-P0053 | 2VCO Planet Lead | A | A026 | double | LeadSynth | D1 p15 |  |
| F05-P0054 | JuicyBleep-age | A | A090 | single | LeadSynth | D1 p15 |  |
| F05-P0055 | Club Ac Bass | A | A027 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0056 | DarkCore Bass | A | A091 | single | Bass/Synth Bass | D1 p15 |  |
| F05-P0057 | Mono Guitar Lead | A | A028 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0058 | Harp | A | A092 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0059 | SpikeyUnisyn | A | A029 | double | MotionSynth | D1 p15 |  |
| F05-P0060 | Swirling Dreams | A | A093 | double | MotionSynth | D1 p15 |  |
| F05-P0061 | Flute Vibrato | A | A030 | double | Woodwind/Reed | D1 p15 |  |
| F05-P0062 | Fisa Americana | A | A094 | double | Woodwind/Reed | D1 p15 |  |
| F05-P0063 | Cymbalis | A | A031 | double | SE | D1 p15 |  |
| F05-P0064 | Monster Island | A | A095 | double | SE | D1 p15 |  |
| F05-P0065 | Ghostly Popper | A | A032 | double | FastSynth | D1 p15 |  |
| F05-P0066 | Reso Comp | A | A096 | single | FastSynth | D1 p15 |  |
| F05-P0067 | Tremolo Wurly | A | A033 | single | Keyboard | D1 p15 |  |
| F05-P0068 | DarkTineStage EP | A | A097 | double | Keyboard | D1 p15 |  |
| F05-P0069 | Saw Arpeggiator | A | A034 | double | Hit/Arpg | D1 p15 |  |
| F05-P0070 | Progressv Trance | A | A098 | double | Hit/Arpg | D1 p15 |  |
| F05-P0071 | StringsQuartet 2 | A | A035 | single | Strings | D1 p15 |  |
| F05-P0072 | PizzAnsamble | A | A099 | double | Strings | D1 p15 |  |
| F05-P0073 | DWGS Sync2 Knob1 | A | A036 | double | LeadSynth | D1 p15 |  |
| F05-P0074 | ! {Tricky} Kit ! | A | A100 | double | Drums | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0075 | Nice&Expensive | A | A037 | double | Vocal/Airy | D1 p15 |  |
| F05-P0076 | Deep Detune Bass | A | A101 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0077 | 5th Stab | A | A038 | double | FastSynth | D1 p15 |  |
| F05-P0078 | Cosmic Furnace | A | A102 | double | FastSynth | D1 p15 |  |
| F05-P0079 | Distortion Organ | A | A039 | double | Organ | D1 p15 |  |
| F05-P0080 | Pipe Mixture | A | A103 | double | Organ | D1 p15 |  |
| F05-P0081 | Monkey Skulls | A | A040 | double | Bell/Mallet | D1 p15 |  |
| F05-P0082 | Krystal Bells | A | A104 | double | Bell/Mallet | D1 p15 |  |
| F05-P0083 | Korgmatose | A | A041 | double | SlowSynth | D1 p15 |  |
| F05-P0084 | Strings Synth | A | A105 | double | SlowSynth | D1 p15 |  |
| F05-P0085 | Dance Lead | A | A042 | single | LeadSynth | D1 p15 |  |
| F05-P0086 | Detox Lead | A | A106 | double | LeadSynth | D1 p15 |  |
| F05-P0087 | SuperSwitch Bass | A | A043 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0088 | Wah Space Bass | A | A107 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0089 | A.Guitar Gliss. | A | A044 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0090 | Da Mute...Joy -Y | A | A108 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0091 | Time Stood Still | A | A045 | double | MotionSynth | D1 p15 |  |
| F05-P0092 | Sinistar*Android | A | A109 | double | MotionSynth | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0093 | Sax Ensemble | A | A046 | double | Woodwind/Reed | D1 p15 |  |
| F05-P0094 | Piano Pad | A | A110 | double | Keyboard | D1 p15 |  |
| F05-P0095 | Orbiting Probes | A | A047 | double | SE | D1 p15 |  |
| F05-P0096 | JoyStick Machine | A | A111 | double | SE | D1 p15 |  |
| F05-P0097 | >Fast Reaction< | A | A048 | double | FastSynth | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0098 | Super Saw Brass | A | A112 | double | FastSynth | D1 p15 |  |
| F05-P0099 | Funky Clav | A | A049 | double | Keyboard | D1 p15 |  |
| F05-P0100 | Romance Piano | A | A113 | double | Keyboard | D1 p15 |  |
| F05-P0101 | NetherlandHit | A | A050 | double | Hit/Arpg | D1 p15 |  |
| F05-P0102 | Techno Organ Hit | A | A114 | double | Hit/Arpg | D1 p15 |  |
| F05-P0103 | D'n'B/GarageBass | A | A051 | single | Bass/Synth Bass | D1 p15 |  |
| F05-P0104 | Boost RnB Bass | A | A115 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0105 | Psycho Kit | A | A052 | double | Drums | D1 p15 |  |
| F05-P0106 | BD&SD Kit 1 | A | A116 | double | Drums | D1 p15 |  |
| F05-P0107 | Aah Voices | A | A053 | double | Vocal/Airy | D1 p15 |  |
| F05-P0108 | Talkin' Keys | A | A117 | double | Vocal/Airy | D1 p15 |  |
| F05-P0109 | Horns & Ensemble | A | A054 | double | Brass | D1 p15 |  |
| F05-P0110 | Trombone Ens. | A | A118 | double | Brass | D1 p15 |  |
| F05-P0111 | Transweep | A | A055 | double | FastSynth | D1 p15 |  |
| F05-P0112 | PowerRezSweep | A | A119 | double | FastSynth | D1 p15 |  |
| F05-P0113 | Chorus E.Guitar | A | A056 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0114 | Thin Bell-s-park | A | A120 | single | Bell/Mallet | D1 p15 |  |
| F05-P0115 | FlutaSonic Pad | A | A057 | double | SlowSynth | D1 p15 |  |
| F05-P0116 | Cross Sweeper | A | A121 | double | SlowSynth | D1 p15 |  |
| F05-P0117 | Ripper | A | A058 | double | LeadSynth | D1 p15 |  |
| F05-P0118 | Nu HPF Lead | A | A122 | double | LeadSynth | D1 p15 |  |
| F05-P0119 | Low Finger Bass | A | A059 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0120 | Motion Bass/Lead | A | A123 | double | Bass/Synth Bass | D1 p15 |  |
| F05-P0121 | Power-Chords&FX | A | A060 | double | Guitar/Plucked | D1 p15 |  |
| F05-P0122 | Jazz Guitar | A | A124 | single | Guitar/Plucked | D1 p15 |  |
| F05-P0123 | Motion Saw | A | A061 | double | MotionSynth | D1 p15 |  |
| F05-P0124 | Space Pod for 2 | A | A125 | double | MotionSynth | D1 p15 |  |
| F05-P0125 | Oboe Vibrato | A | A062 | single | Woodwind/Reed | D1 p15 |  |
| F05-P0126 | TenorSax Growl-Y | A | A126 | double | Woodwind/Reed | D1 p15 |  |
| F05-P0127 | Xpanded Voice | A | A063 | double | SE | D1 p15 |  |
| F05-P0128 | Steam Sweeps | A | A127 | double | SE | D1 p15 |  |
| F05-P0129 | Big Sweep Stab | B | B000 | double | FastSynth | D1 p16 |  |
| F05-P0130 | InverseSQwave | B | B064 | double | FastSynth | D1 p16 |  |
| F05-P0131 | Acoustic Piano | B | B001 | single | Keyboard | D1 p16 |  |
| F05-P0132 | Warm E.Grand | B | B065 | double | Keyboard | D1 p16 |  |
| F05-P0133 | Flip Blip | B | B002 | double | Hit/Arpg | D1 p16 |  |
| F05-P0134 | VCF Sample&Hold | B | B066 | double | Hit/Arpg | D1 p16 |  |
| F05-P0135 | Arco Strings | B | B003 | double | Strings | D1 p16 |  |
| F05-P0136 | AnalogStrings1&2 | B | B067 | double | Strings | D1 p16 |  |
| F05-P0137 | Trance/GarageKit | B | B004 | double | Drums | D1 p16 |  |
| F05-P0138 | Dragon Gong | B | B068 | double | Drums | D1 p16 |  |
| F05-P0139 | Bass 4 Da Phunk | B | B005 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0140 | Tekno Organ*Bass | B | B069 | double | Bass/Synth Bass | D1 p16 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0141 | Burnin' Brass | B | B006 | double | Brass | D1 p16 |  |
| F05-P0142 | 5th Sweeper | B | B070 | double | FastSynth | D1 p16 |  |
| F05-P0143 | Full Drawbars | B | B007 | double | Organ | D1 p16 |  |
| F05-P0144 | Dark Jazz-Organ | B | B071 | double | Organ | D1 p16 |  |
| F05-P0145 | Bali Gamelon | B | B008 | double | Bell/Mallet | D1 p16 |  |
| F05-P0146 | Hybrid Bell | B | B072 | double | Bell/Mallet | D1 p16 |  |
| F05-P0147 | Breakdown String | B | B009 | double | SlowSynth | D1 p16 |  |
| F05-P0148 | Digi Ice Pad | B | B073 | double | SlowSynth | D1 p16 |  |
| F05-P0149 | Phat Saw Lead | B | B010 | double | LeadSynth | D1 p16 |  |
| F05-P0150 | Detune Leader | B | B074 | double | LeadSynth | D1 p16 |  |
| F05-P0151 | Drive Bass | B | B011 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0152 | House Bass | B | B075 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0153 | AcousticGuitar 1 | B | B012 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0154 | SingleCoil+Piezo | B | B076 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0155 | VCF Modulation | B | B013 | single | MotionSynth | D1 p16 |  |
| F05-P0156 | BPF Sweep Pad | B | B077 | double | MotionSynth | D1 p16 |  |
| F05-P0157 | Harmonica | B | B014 | single | Woodwind/Reed | D1 p16 |  |
| F05-P0158 | Night Tines EP | B | B078 | double | Keyboard | D1 p16 |  |
| F05-P0159 | Soundscapes | B | B015 | double | SE | D1 p16 |  |
| F05-P0160 | Sonic Blast | B | B079 | double | SE | D1 p16 |  |
| F05-P0161 | Rezbo | B | B016 | double | FastSynth | D1 p16 |  |
| F05-P0162 | Future Syn Pad | B | B080 | double | FastSynth | D1 p16 |  |
| F05-P0163 | Stereo E.Piano | B | B017 | double | Keyboard | D1 p16 |  |
| F05-P0164 | SynPiano X | B | B081 | single | Keyboard | D1 p16 |  |
| F05-P0165 | BlastFromDaPast | B | B018 | double | Hit/Arpg | D1 p16 |  |
| F05-P0166 | Jungle Melody | B | B082 | double | Hit/Arpg | D1 p16 |  |
| F05-P0167 | Vintage Stringz | B | B019 | double | Strings | D1 p16 |  |
| F05-P0168 | Ana Bass 1 | B | B083 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0169 | Standard Kit 1 | B | B020 | double | Drums | D1 p16 |  |
| F05-P0170 | Drum'n'Bass Kit | B | B084 | double | Drums | D1 p16 |  |
| F05-P0171 | Dah Voices | B | B021 | double | Vocal/Airy | D1 p16 |  |
| F05-P0172 | Fresh Breath | B | B085 | double | Vocal/Airy | D1 p16 |  |
| F05-P0173 | Trombone Hard | B | B022 | single | Brass | D1 p16 |  |
| F05-P0174 | Fat Brass | B | B086 | double | Brass | D1 p16 |  |
| F05-P0175 | Classic Click | B | B023 | double | Organ | D1 p16 |  |
| F05-P0176 | Turn Back Stab | B | B087 | double | FastSynth | D1 p16 |  |
| F05-P0177 | VS Bell Boy | B | B024 | double | Bell/Mallet | D1 p16 |  |
| F05-P0178 | Tropico Bells | B | B088 | double | Bell/Mallet | D1 p16 |  |
| F05-P0179 | Reactor Pad | B | B025 | double | SlowSynth | D1 p16 |  |
| F05-P0180 | Simple Sine Pad | B | B089 | double | SlowSynth | D1 p16 |  |
| F05-P0181 | MonoSynGtr LEAD | B | B026 | double | LeadSynth | D1 p16 |  |
| F05-P0182 | Seq DDL Lead | B | B090 | double | LeadSynth | D1 p16 |  |
| F05-P0183 | Acoustic Bass | B | B027 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0184 | 2 Step Bass | B | B091 | single | Bass/Synth Bass | D1 p16 |  |
| F05-P0185 | Wet Dist. Guitar | B | B028 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0186 | Koto | B | B092 | single | Guitar/Plucked | D1 p16 |  |
| F05-P0187 | Crimson 5ths | B | B029 | double | MotionSynth | D1 p16 |  |
| F05-P0188 | Astral Dreams | B | B093 | double | MotionSynth | D1 p16 |  |
| F05-P0189 | Old Shakuhachi | B | B030 | double | Woodwind/Reed | D1 p16 |  |
| F05-P0190 | Musette | B | B094 | single | Woodwind/Reed | D1 p16 |  |
| F05-P0191 | Loop-Iteria | B | B031 | double | SE | D1 p16 |  |
| F05-P0192 | Dark Night | B | B095 | double | SE | D1 p16 |  |
| F05-P0193 | Perky Bleeper | B | B032 | double | FastSynth | D1 p16 |  |
| F05-P0194 | Metalic Rez | B | B096 | double | FastSynth | D1 p16 |  |
| F05-P0195 | Velo Whirly | B | B033 | double | Keyboard | D1 p16 |  |
| F05-P0196 | Dyno Tine EP | B | B097 | double | Keyboard | D1 p16 |  |
| F05-P0197 | Auto Pilot | B | B034 | single | Hit/Arpg | D1 p16 |  |
| F05-P0198 | Goa Message | B | B098 | double | Hit/Arpg | D1 p16 |  |
| F05-P0199 | Small StringsEns | B | B035 | double | Strings | D1 p16 |  |
| F05-P0200 | PizzicatoSection | B | B099 | double | Strings | D1 p16 |  |
| F05-P0201 | WellAardHouse | B | B036 | single | LeadSynth | D1 p16 |  |
| F05-P0202 | Orchestra&Ethnic | B | B100 | double | Drums | D1 p16 |  |
| F05-P0203 | Slow Choir ST | B | B037 | double | Vocal/Airy | D1 p16 |  |
| F05-P0204 | The Tea-Bea | B | B101 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0205 | Garage PWM | B | B038 | double | FastSynth | D1 p16 |  |
| F05-P0206 | Synth Sweeper | B | B102 | double | FastSynth | D1 p16 |  |
| F05-P0207 | Perc Short Decay | B | B039 | double | Organ | D1 p16 |  |
| F05-P0208 | Church Pipes | B | B103 | double | Organ | D1 p16 |  |
| F05-P0209 | Log Drum | B | B040 | double | Bell/Mallet | D1 p16 |  |
| F05-P0210 | Tinkle Bells | B | B104 | double | Bell/Mallet | D1 p16 |  |
| F05-P0211 | Rez. Down | B | B041 | double | SlowSynth | D1 p16 |  |
| F05-P0212 | Noble Brite Pad | B | B105 | double | SlowSynth | D1 p16 |  |
| F05-P0213 | Espress Lead | B | B042 | double | LeadSynth | D1 p16 |  |
| F05-P0214 | Old & Analog | B | B106 | double | LeadSynth | D1 p16 |  |
| F05-P0215 | Slap Bass v/s | B | B043 | single | Bass/Synth Bass | D1 p16 |  |
| F05-P0216 | Nu Skool Bass | B | B107 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0217 | Sitar Sitar | B | B044 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0218 | Funkin' Guitar | B | B108 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0219 | Tinklin' Pad | B | B045 | double | MotionSynth | D1 p16 |  |
| F05-P0220 | Spectrum Alloy | B | B109 | double | MotionSynth | D1 p16 |  |
| F05-P0221 | SopranoSax Br.-Y | B | B046 | double | Woodwind/Reed | D1 p16 |  |
| F05-P0222 | HarpsyKorg 8'+4' | B | B110 | double | Keyboard | D1 p16 |  |
| F05-P0223 | The Great Wall | B | B047 | double | SE | D1 p16 |  |
| F05-P0224 | VenusianStories | B | B111 | double | SE | D1 p16 |  |
| F05-P0225 | Mega Big Synth | B | B048 | double | FastSynth | D1 p16 |  |
| F05-P0226 | SynthBrass | B | B112 | double | FastSynth | D1 p16 |  |
| F05-P0227 | Clav | B | B049 | single | Keyboard | D1 p16 |  |
| F05-P0228 | Phantom Of Tine | B | B113 | double | Keyboard | D1 p16 |  |
| F05-P0229 | Percussive Hits | B | B050 | double | Hit/Arpg | D1 p16 |  |
| F05-P0230 | Rave Synth | B | B114 | double | Hit/Arpg | D1 p16 |  |
| F05-P0231 | Drum'n'Bass Bass | B | B051 | single | Bass/Synth Bass | D1 p16 |  |
| F05-P0232 | Dark R&B Bass | B | B115 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0233 | Psycho Pitch Kit | B | B052 | double | Drums | D1 p16 |  |
| F05-P0234 | BD&SD Kit 2 | B | B116 | double | Drums | D1 p16 |  |
| F05-P0235 | Woh Voices | B | B053 | double | Vocal/Airy | D1 p16 |  |
| F05-P0236 | Take Voices | B | B117 | double | Vocal/Airy | D1 p16 |  |
| F05-P0237 | Classic Fanfare | B | B054 | double | Brass | D1 p16 |  |
| F05-P0238 | Glen & The Boys | B | B118 | double | Brass | D1 p16 |  |
| F05-P0239 | Blasta Brasster | B | B055 | double | FastSynth | D1 p16 |  |
| F05-P0240 | Dual Filterz | B | B119 | double | FastSynth | D1 p16 |  |
| F05-P0241 | Strato-Chime | B | B056 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0242 | Future Bell | B | B120 | double | Bell/Mallet | D1 p16 |  |
| F05-P0243 | Clouds of Air | B | B057 | double | SlowSynth | D1 p16 |  |
| F05-P0244 | Ultra Res. Sweep | B | B121 | double | SlowSynth | D1 p16 |  |
| F05-P0245 | Nu Skool Bleeps | B | B058 | single | LeadSynth | D1 p16 |  |
| F05-P0246 | Glide Lead | B | B122 | double | LeadSynth | D1 p16 |  |
| F05-P0247 | Stein Bass | B | B059 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0248 | Nasty Bass | B | B123 | double | Bass/Synth Bass | D1 p16 |  |
| F05-P0249 | Joystick Gtr(-Y) | B | B060 | double | Guitar/Plucked | D1 p16 |  |
| F05-P0250 | Vintage Stratt | B | B124 | single | Guitar/Plucked | D1 p16 |  |
| F05-P0251 | Transformation | B | B061 | double | MotionSynth | D1 p16 |  |
| F05-P0252 | Symphonic Waves | B | B125 | double | MotionSynth | D1 p16 |  |
| F05-P0253 | DoubleReed | B | B062 | single | Woodwind/Reed | D1 p16 |  |
| F05-P0254 | AltoSax2Breath-Y | B | B126 | double | Woodwind/Reed | D1 p16 |  |
| F05-P0255 | <Techno Vox Box> | B | B063 | double | SE | D1 p16 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0256 | DJ Touch | B | B127 | double | SE | D1 p16 |  |
| F05-P0257 | EuphoricTrancer | C | C000 | double | FastSynth | D1 p17 |  |
| F05-P0258 | Square Snaps | C | C064 | double | FastSynth | D1 p17 |  |
| F05-P0259 | Attack Piano | C | C001 | double | Keyboard | D1 p17 |  |
| F05-P0260 | 90's Piano | C | C065 | double | Keyboard | D1 p17 |  |
| F05-P0261 | Scattersynth | C | C002 | double | MotionSynth | D1 p17 |  |
| F05-P0262 | Synchro Science | C | C066 | double | Hit/Arpg | D1 p17 |  |
| F05-P0263 | StSlowStringsEXP | C | C003 | double | Strings | D1 p17 |  |
| F05-P0264 | HybridStrg/Choir | C | C067 | double | Strings | D1 p17 |  |
| F05-P0265 | HipHop Kit | C | C004 | double | Drums | D1 p17 |  |
| F05-P0266 | Marc Tree | C | C068 | double | Drums | D1 p17 |  |
| F05-P0267 | Ana Bass 2 | C | C005 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0268 | Hard Velo Bass | C | C069 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0269 | SFZ Brass ST | C | C006 | double | Brass | D1 p17 |  |
| F05-P0270 | Cyber Choir | C | C070 | double | FastSynth | D1 p17 |  |
| F05-P0271 | Gospel PercOrgan | C | C007 | double | Organ | D1 p17 |  |
| F05-P0272 | Sine DWGS-Organ | C | C071 | double | Organ | D1 p17 |  |
| F05-P0273 | Marimba | C | C008 | single | Bell/Mallet | D1 p17 |  |
| F05-P0274 | Digital Bells 2 | C | C072 | single | Bell/Mallet | D1 p17 |  |
| F05-P0275 | Symphonic Ensemb | C | C009 | double | SlowSynth | D1 p17 |  |
| F05-P0276 | Harmonic Pad | C | C073 | double | SlowSynth | D1 p17 |  |
| F05-P0277 | BrightPulseLead | C | C010 | double | LeadSynth | D1 p17 |  |
| F05-P0278 | Digitized Raver | C | C074 | double | LeadSynth | D1 p17 |  |
| F05-P0279 | DistortedTeaBea | C | C011 | single | Bass/Synth Bass | D1 p17 |  |
| F05-P0280 | Feedback MG Bass | C | C075 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0281 | Nylon Guitar 1 | C | C012 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0282 | PedalSteelGuitar | C | C076 | single | Guitar/Plucked | D1 p17 |  |
| F05-P0283 | Gating Voice Pad | C | C013 | double | MotionSynth | D1 p17 |  |
| F05-P0284 | OXYGEN | C | C077 | double | MotionSynth | D1 p17 |  |
| F05-P0285 | Spitz Bottle | C | C014 | double | Woodwind/Reed | D1 p17 |  |
| F05-P0286 | Hybrid Digi EP | C | C078 | double | Keyboard | D1 p17 |  |
| F05-P0287 | MotionSoundTrack | C | C015 | double | SE | D1 p17 |  |
| F05-P0288 | Video Blaster | C | C079 | double | SE | D1 p17 |  |
| F05-P0289 | Rez. Sweep | C | C016 | double | FastSynth | D1 p17 |  |
| F05-P0290 | Pop Synth Pad | C | C080 | double | FastSynth | D1 p17 |  |
| F05-P0291 | EP Dark mf/f | C | C017 | single | Keyboard | D1 p17 |  |
| F05-P0292 | White Pad EP | C | C081 | double | Keyboard | D1 p17 |  |
| F05-P0293 | StaccatoPizzHit | C | C018 | double | Hit/Arpg | D1 p17 |  |
| F05-P0294 | Big Hit in India | C | C082 | double | Hit/Arpg | D1 p17 |  |
| F05-P0295 | WatcherOfTheSky | C | C019 | double | Strings | D1 p17 |  |
| F05-P0296 | Jungle Rez Bass | C | C083 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0297 | Standard Kit 2 | C | C020 | double | Drums | D1 p17 |  |
| F05-P0298 | D'n'B Gate Kit | C | C084 | double | Drums | D1 p17 |  |
| F05-P0299 | Choir of Light | C | C021 | double | Vocal/Airy | D1 p17 |  |
| F05-P0300 | The AirVox | C | C085 | double | Vocal/Airy | D1 p17 |  |
| F05-P0301 | Trumpet | C | C022 | single | Brass | D1 p17 |  |
| F05-P0302 | Tight Brass | C | C086 | double | Brass | D1 p17 |  |
| F05-P0303 | Percussion BX3 | C | C023 | double | Organ | D1 p17 |  |
| F05-P0304 | Thunderlog | C | C087 | double | FastSynth | D1 p17 |  |
| F05-P0305 | Moving Bellz | C | C024 | double | Bell/Mallet | D1 p17 |  |
| F05-P0306 | Bottle-Bell | C | C088 | double | Bell/Mallet | D1 p17 |  |
| F05-P0307 | Tsunami Waves | C | C025 | double | SlowSynth | D1 p17 |  |
| F05-P0308 | Smoothy Pad | C | C089 | double | SlowSynth | D1 p17 |  |
| F05-P0309 | HipHop Lead | C | C026 | double | LeadSynth | D1 p17 |  |
| F05-P0310 | Syncro City | C | C090 | double | LeadSynth | D1 p17 |  |
| F05-P0311 | Fretless Bass | C | C027 | single | Bass/Synth Bass | D1 p17 |  |
| F05-P0312 | Ramp Jungle Bass | C | C091 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0313 | Mute Monster | C | C028 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0314 | Old 12-String | C | C092 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0315 | Aqua Phonics | C | C029 | double | MotionSynth | D1 p17 |  |
| F05-P0316 | Flying Machines | C | C093 | double | MotionSynth | D1 p17 |  |
| F05-P0317 | Jazz Clarinet | C | C030 | single | Woodwind/Reed | D1 p17 |  |
| F05-P0318 | Fisa Cassotto | C | C094 | double | Woodwind/Reed | D1 p17 |  |
| F05-P0319 | Goa Lover | C | C031 | double | SE | D1 p17 |  |
| F05-P0320 | Compu-shift | C | C095 | double | SE | D1 p17 |  |
| F05-P0321 | Analog Piano | C | C032 | double | FastSynth | D1 p17 |  |
| F05-P0322 | S&H Reso Comp | C | C096 | double | FastSynth | D1 p17 |  |
| F05-P0323 | Classic Wurly | C | C033 | double | Keyboard | D1 p17 |  |
| F05-P0324 | Pro-Dyno EP | C | C097 | double | Keyboard | D1 p17 |  |
| F05-P0325 | Noise Shot | C | C034 | double | Hit/Arpg | D1 p17 |  |
| F05-P0326 | Motion Raver | C | C098 | double | Hit/Arpg | D1 p17 |  |
| F05-P0327 | StringsQuartet 1 | C | C035 | single | Strings | D1 p17 |  |
| F05-P0328 | Octave Strings | C | C099 | double | Strings | D1 p17 |  |
| F05-P0329 | X-Mod Raver | C | C036 | double | LeadSynth | D1 p17 |  |
| F05-P0330 | Percussion Kit | C | C100 | double | Drums | D1 p17 |  |
| F05-P0331 | Garage SQ Bass | C | C037 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0332 | Blaster Bass | C | C101 | single | Bass/Synth Bass | D1 p17 |  |
| F05-P0333 | Power Snap Synth | C | C038 | double | FastSynth | D1 p17 |  |
| F05-P0334 | Band Passed | C | C102 | double | FastSynth | D1 p17 |  |
| F05-P0335 | Old Tone-Wheel | C | C039 | double | Organ | D1 p17 |  |
| F05-P0336 | M1 Organ | C | C103 | double | Organ | D1 p17 |  |
| F05-P0337 | Velo Kalimba | C | C040 | double | Bell/Mallet | D1 p17 |  |
| F05-P0338 | GlassBell Bright | C | C104 | single | Bell/Mallet | D1 p17 |  |
| F05-P0339 | Antartic Wind | C | C041 | double | SlowSynth | D1 p17 |  |
| F05-P0340 | Analog Pad | C | C105 | double | SlowSynth | D1 p17 |  |
| F05-P0341 | Smooth Sine Lead | C | C042 | double | LeadSynth | D1 p17 |  |
| F05-P0342 | Octo Lead | C | C106 | double | LeadSynth | D1 p17 |  |
| F05-P0343 | E.Bass Pick 2 | C | C043 | single | Bass/Synth Bass | D1 p17 |  |
| F05-P0344 | Karma Sutra | C | C107 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0345 | A.Guitar NatATK | C | C044 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0346 | CleanMute-Guitar | C | C108 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0347 | Pods In The Pad | C | C045 | double | MotionSynth | D1 p17 |  |
| F05-P0348 | Stereo Rez Sweep | C | C109 | double | MotionSynth | D1 p17 |  |
| F05-P0349 | Reed Of Power | C | C046 | double | Woodwind/Reed | D1 p17 |  |
| F05-P0350 | Theatrical Pad | C | C110 | double | SlowSynth | D1 p17 |  |
| F05-P0351 | New Voyage | C | C047 | double | SE | D1 p17 |  |
| F05-P0352 | Distant Lights | C | C111 | double | SE | D1 p17 |  |
| F05-P0353 | Noisy Stabber | C | C048 | double | FastSynth | D1 p17 |  |
| F05-P0354 | Soft Synth Brass | C | C112 | double | FastSynth | D1 p17 |  |
| F05-P0355 | Sticky Rez Clav | C | C049 | double | Keyboard | D1 p17 |  |
| F05-P0356 | Sweeping EP | C | C113 | double | Keyboard | D1 p17 |  |
| F05-P0357 | Organ Impact | C | C050 | double | Hit/Arpg | D1 p17 |  |
| F05-P0358 | Techno Stat | C | C114 | double | Hit/Arpg | D1 p17 |  |
| F05-P0359 | UK Garage Bass | C | C051 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0360 | Phat Bass | C | C115 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0361 | UGLY HoUSe Kit | C | C052 | double | Drums | D1 p17 |  |
| F05-P0362 | Wild Wasp | C | C116 | double | Hit/Arpg | D1 p17 |  |
| F05-P0363 | Oooh Voices ST | C | C053 | double | Vocal/Airy | D1 p17 |  |
| F05-P0364 | Vocalscaping | C | C117 | double | Vocal/Airy | D1 p17 |  |
| F05-P0365 | Film Brass | C | C054 | double | Brass | D1 p17 |  |
| F05-P0366 | Big Band Plunger | C | C118 | double | Brass | D1 p17 |  |
| F05-P0367 | Polyphonic Line | C | C055 | double | FastSynth | D1 p17 |  |
| F05-P0368 | Filter Morpher | C | C119 | double | FastSynth | D1 p17 |  |
| F05-P0369 | Dynamic E.Guitar | C | C056 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0370 | Breathy Bells | C | C120 | double | Bell/Mallet | D1 p17 |  |
| F05-P0371 | Ravelian Pad | C | C057 | double | SlowSynth | D1 p17 |  |
| F05-P0372 | Cinema Pad | C | C121 | double | SlowSynth | D1 p17 |  |
| F05-P0373 | Fat Syn Sync | C | C058 | double | LeadSynth | D1 p17 |  |
| F05-P0374 | Electro Lead | C | C122 | double | LeadSynth | D1 p17 |  |
| F05-P0375 | E.Bass Finger | C | C059 | single | Bass/Synth Bass | D1 p17 |  |
| F05-P0376 | 30303 Mega Bass | C | C123 | double | Bass/Synth Bass | D1 p17 |  |
| F05-P0377 | Feedback D. Gtr. | C | C060 | double | Guitar/Plucked | D1 p17 |  |
| F05-P0378 | E.G. Harmonics | C | C124 | single | Guitar/Plucked | D1 p17 |  |
| F05-P0379 | Cyclic X-Fades | C | C061 | double | MotionSynth | D1 p17 |  |
| F05-P0380 | Moon Talker | C | C125 | double | MotionSynth | D1 p17 |  |
| F05-P0381 | English Horn | C | C062 | single | Woodwind/Reed | D1 p17 |  |
| F05-P0382 | TenorSax Brth.-Y | C | C126 | double | Woodwind/Reed | D1 p17 |  |
| F05-P0383 | Gliding Squares | C | C063 | double | FastSynth | D1 p17 |  |
| F05-P0384 | Wispy Dry Wind | C | C127 | double | SE | D1 p17 |  |
| F05-P0385 | Comp Trance Stab | D | D000 | double | FastSynth | D1 p18 |  |
| F05-P0386 | Syn Pizz | D | D064 | double | FastSynth | D1 p18 |  |
| F05-P0387 | PianoMezzoDark | D | D001 | double | Keyboard | D1 p18 |  |
| F05-P0388 | ST Piano mp/f | D | D065 | double | Keyboard | D1 p18 |  |
| F05-P0389 | Bounce Shot | D | D002 | double | MotionSynth | D1 p18 |  |
| F05-P0390 | Arp Angeles | D | D066 | double | Hit/Arpg | D1 p18 |  |
| F05-P0391 | Legato Strings | D | D003 | double | Strings | D1 p18 |  |
| F05-P0392 | Analog Velvet | D | D067 | double | Strings | D1 p18 |  |
| F05-P0393 | House Kit | D | D004 | double | Drums | D1 p18 |  |
| F05-P0394 | Clock It Out | D | D068 | double | MotionSynth | D1 p18 |  |
| F05-P0395 | Bazooka Bass | D | D005 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0396 | MG Pulse Bass | D | D069 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0397 | Brass Of Power | D | D006 | double | Brass | D1 p18 |  |
| F05-P0398 | Power Saw | D | D070 | double | FastSynth | D1 p18 |  |
| F05-P0399 | Jazz Organ 2 | D | D007 | double | Organ | D1 p18 |  |
| F05-P0400 | Formant Voices | D | D071 | double | Vocal/Airy | D1 p18 |  |
| F05-P0401 | Mondo'Rimba | D | D008 | double | Bell/Mallet | D1 p18 |  |
| F05-P0402 | Vibraphone 1 | D | D072 | double | Bell/Mallet | D1 p18 |  |
| F05-P0403 | Money Pad | D | D009 | double | SlowSynth | D1 p18 |  |
| F05-P0404 | StereoStringsEXP | D | D073 | double | Strings | D1 p18 |  |
| F05-P0405 | Square Wave Lead | D | D010 | double | LeadSynth | D1 p18 |  |
| F05-P0406 | Syn Pipes | D | D074 | double | LeadSynth | D1 p18 |  |
| F05-P0407 | The Growler | D | D011 | single | Bass/Synth Bass | D1 p18 |  |
| F05-P0408 | Digi Syn Bass | D | D075 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0409 | FingertipsGuitar | D | D012 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0410 | PiezoMix Guitar | D | D076 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0411 | Flashlight Pad | D | D013 | double | MotionSynth | D1 p18 |  |
| F05-P0412 | Dolphin Ride | D | D077 | double | MotionSynth | D1 p18 |  |
| F05-P0413 | Recorder | D | D014 | single | Woodwind/Reed | D1 p18 |  |
| F05-P0414 | Studio Stage EP | D | D078 | double | Keyboard | D1 p18 |  |
| F05-P0415 | One Note Stories | D | D015 | double | SE | D1 p18 |  |
| F05-P0416 | Glass Vox | D | D079 | double | MotionSynth | D1 p18 |  |
| F05-P0417 | Square Rez | D | D016 | double | FastSynth | D1 p18 |  |
| F05-P0418 | Ana Brass/Lead | D | D080 | double | FastSynth | D1 p18 |  |
| F05-P0419 | Vintage EP | D | D017 | double | Keyboard | D1 p18 |  |
| F05-P0420 | Reson Piano | D | D081 | double | Keyboard | D1 p18 |  |
| F05-P0421 | Brass Impact Hit | D | D018 | double | Hit/Arpg | D1 p18 |  |
| F05-P0422 | Hit Me 2 Times!! | D | D082 | double | Hit/Arpg | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0423 | The Pad | D | D019 | double | SlowSynth | D1 p18 |  |
| F05-P0424 | Euro 8va Bass | D | D083 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0425 | Jazz/Brush Kits | D | D020 | double | Drums | D1 p18 |  |
| F05-P0426 | WAcKy HiPHop Kit | D | D084 | double | Drums | D1 p18 |  |
| F05-P0427 | Ooh/Aah VoicesST | D | D021 | double | Vocal/Airy | D1 p18 |  |
| F05-P0428 | Vocalesque | D | D085 | double | Vocal/Airy | D1 p18 |  |
| F05-P0429 | Muted Trumpet | D | D022 | single | Brass | D1 p18 |  |
| F05-P0430 | Brass Expression | D | D086 | double | Brass | D1 p18 |  |
| F05-P0431 | Killer B | D | D023 | double | Organ | D1 p18 |  |
| F05-P0432 | Fire Wave | D | D087 | double | FastSynth | D1 p18 |  |
| F05-P0433 | Magical Bells | D | D024 | double | Bell/Mallet | D1 p18 |  |
| F05-P0434 | Mallet Clocker | D | D088 | double | Bell/Mallet | D1 p18 |  |
| F05-P0435 | {(Meditate}) | D | D025 | double | SlowSynth | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0436 | Stereo Piano mf | D | D089 | double | Keyboard | D1 p18 |  |
| F05-P0437 | Trancer Lead | D | D026 | double | LeadSynth | D1 p18 |  |
| F05-P0438 | Sync Kronicity | D | D090 | double | LeadSynth | D1 p18 |  |
| F05-P0439 | Sweet Fretless | D | D027 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0440 | Poinker Bass | D | D091 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0441 | Amp D. Guitar | D | D028 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0442 | AcousticGuitar 2 | D | D092 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0443 | Rezzo Release | D | D029 | double | MotionSynth | D1 p18 |  |
| F05-P0444 | Station Of Waves | D | D093 | double | MotionSynth | D1 p18 |  |
| F05-P0445 | BambuSilverFlute | D | D030 | double | Woodwind/Reed | D1 p18 |  |
| F05-P0446 | MixFisaMaster | D | D094 | double | Woodwind/Reed | D1 p18 |  |
| F05-P0447 | CyborgFactoryHit | D | D031 | double | SE | D1 p18 |  |
| F05-P0448 | Frozen Glaciers | D | D095 | double | SE | D1 p18 |  |
| F05-P0449 | The ANAPIZZ | D | D032 | double | FastSynth | D1 p18 |  |
| F05-P0450 | AStabInTheDark | D | D096 | double | MotionSynth | D1 p18 |  |
| F05-P0451 | R&B E.Piano | D | D033 | double | Keyboard | D1 p18 |  |
| F05-P0452 | Pro-Stage EP | D | D097 | double | Keyboard | D1 p18 |  |
| F05-P0453 | Arp Twins | D | D034 | double | Hit/Arpg | D1 p18 |  |
| F05-P0454 | Wild Arp | D | D098 | double | Hit/Arpg | D1 p18 |  |
| F05-P0455 | Camera Strings | D | D035 | double | Strings | D1 p18 |  |
| F05-P0456 | Stereo Strings | D | D099 | double | Strings | D1 p18 |  |
| F05-P0457 | Busy Sync | D | D036 | double | LeadSynth | D1 p18 |  |
| F05-P0458 | Processed Kit | D | D100 | double | Drums | D1 p18 |  |
| F05-P0459 | 30303 Square | D | D037 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0460 | Acid Dist Bass | D | D101 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0461 | Trumpet Ens. | D | D038 | double | Brass | D1 p18 |  |
| F05-P0462 | Few Bows Here | D | D102 | double | Strings | D1 p18 |  |
| F05-P0463 | "Dirty ""B""" | D | D039 | double | Organ | D1 p18 |  |
| F05-P0464 | Arctic Voices | D | D103 | double | Vocal/Airy | D1 p18 |  |
| F05-P0465 | Log Drum & Bells | D | D040 | double | Bell/Mallet | D1 p18 |  |
| F05-P0466 | Classic E.Piano | D | D104 | double | Keyboard | D1 p18 |  |
| F05-P0467 | Freedom Pad | D | D041 | double | SlowSynth | D1 p18 |  |
| F05-P0468 | Mellow Movie Pad | D | D105 | double | SlowSynth | D1 p18 |  |
| F05-P0469 | Thin AnaLead | D | D042 | double | LeadSynth | D1 p18 |  |
| F05-P0470 | AMSFeedbackLead | D | D106 | double | LeadSynth | D1 p18 |  |
| F05-P0471 | E.Bass Pick 1 | D | D043 | single | Bass/Synth Bass | D1 p18 |  |
| F05-P0472 | Classic F.Horn | D | D107 | double | Brass | D1 p18 |  |
| F05-P0473 | Indian Stars | D | D044 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0474 | Indian Frets Gtr | D | D108 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0475 | OutOfTheClouds | D | D045 | double | MotionSynth | D1 p18 |  |
| F05-P0476 | Virtual Traveler | D | D109 | double | MotionSynth | D1 p18 |  |
| F05-P0477 | BariSax Growl -Y | D | D046 | double | Woodwind/Reed | D1 p18 |  |
| F05-P0478 | Flute | D | D110 | single | Woodwind/Reed | D1 p18 |  |
| F05-P0479 | Hemispheres | D | D047 | double | SE | D1 p18 |  |
| F05-P0480 | Vanishing Planet | D | D111 | double | SE | D1 p18 |  |
| F05-P0481 | Techno Phonic | D | D048 | double | FastSynth | D1 p18 |  |
| F05-P0482 | Symphonic Bows | D | D112 | double | Strings | D1 p18 |  |
| F05-P0483 | Clav Snap | D | D049 | double | Keyboard | D1 p18 |  |
| F05-P0484 | Classic Piano | D | D113 | single | Keyboard | D1 p18 |  |
| F05-P0485 | Old Record Hit | D | D050 | double | Hit/Arpg | D1 p18 |  |
| F05-P0486 | Velo PowerStab | D | D114 | double | Hit/Arpg | D1 p18 |  |
| F05-P0487 | Cyber Tom | D | D051 | single | Bass/Synth Bass | D1 p18 |  |
| F05-P0488 | Happy Vibe Lead | D | D115 | double | LeadSynth | D1 p18 |  |
| F05-P0489 | That Heaven Vibe | D | D052 | double | SlowSynth | D1 p18 |  |
| F05-P0490 | Trumpet Section | D | D116 | double | Brass | D1 p18 |  |
| F05-P0491 | Aaah Voices ST | D | D053 | double | Vocal/Airy | D1 p18 |  |
| F05-P0492 | Dream Voices | D | D117 | double | Vocal/Airy | D1 p18 |  |
| F05-P0493 | French Horn Sect | D | D054 | double | Brass | D1 p18 |  |
| F05-P0494 | Octave Brass Exp | D | D118 | double | Brass | D1 p18 |  |
| F05-P0495 | Garage Organ | D | D055 | double | Organ | D1 p18 |  |
| F05-P0496 | Solo Violin | D | D119 | single | Strings | D1 p18 |  |
| F05-P0497 | Nu Strat | D | D056 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0498 | Santur | D | D120 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0499 | Brass Pad | D | D057 | double | SlowSynth | D1 p18 |  |
| F05-P0500 | Full Vox Pad | D | D121 | double | Vocal/Airy | D1 p18 |  |
| F05-P0501 | Brian's Sync | D | D058 | double | LeadSynth | D1 p18 |  |
| F05-P0502 | A leadload | D | D122 | double | LeadSynth | D1 p18 |  |
| F05-P0503 | SynBassRes | D | D059 | double | Bass/Synth Bass | D1 p18 |  |
| F05-P0504 | L/R Piano(Knob1) | D | D123 | double | Keyboard | D1 p18 |  |
| F05-P0505 | Vox Wah Chicks | D | D060 | single | Guitar/Plucked | D1 p18 |  |
| F05-P0506 | Spanish Guitar | D | D124 | double | Guitar/Plucked | D1 p18 |  |
| F05-P0507 | Stereo WaveSweep | D | D061 | double | MotionSynth | D1 p18 |  |
| F05-P0508 | Motion Ocean ! | D | D125 | double | MotionSynth | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F05-P0509 | Bassoon | D | D062 | single | Woodwind/Reed | D1 p18 |  |
| F05-P0510 | AltoSax1 Growl-Y | D | D126 | double | Woodwind/Reed | D1 p18 |  |
| F05-P0511 | Flute Pad | D | D063 | double | FastSynth | D1 p18 |  |
| F05-P0512 | Init.Program | D | D127 | single | Keyboard | D1 p18 |  |
| F05-P0513 | Acoustic Piano | G | G001 | single | Keyboard | D1 p19 |  |
| F05-P0514 | Trumpet | G | G057 | single | Brass | D1 p19 |  |
| F05-P0515 | Tinkle Bell | G | G113 | double | Bell/Mallet | D1 p19 |  |
| F05-P0516 | Bright Piano | G | G002 | single | Keyboard | D1 p19 |  |
| F05-P0517 | Trombone 1 | G | G058 | single | Brass | D1 p19 |  |
| F05-P0518 | Agogo | G | G114 | single | Bell/Mallet | D1 p19 |  |
| F05-P0519 | El.Grand Piano | G | G003 | single | Keyboard | D1 p19 |  |
| F05-P0520 | Tuba | G | G059 | single | Brass | D1 p19 |  |
| F05-P0521 | Steel Drums | G | G115 | double | Drums | D1 p19 |  |
| F05-P0522 | Honkey-Tonk | G | G004 | double | Keyboard | D1 p19 |  |
| F05-P0523 | Muted Trumpet 1 | G | G060 | single | Brass | D1 p19 |  |
| F05-P0524 | Woodblock | G | G116 | single | Drums | D1 p19 |  |
| F05-P0525 | Electric Piano 1 | G | G005 | single | Keyboard | D1 p19 |  |
| F05-P0526 | French Horn | G | G061 | single | Brass | D1 p19 |  |
| F05-P0527 | Taiko | G | G117 | single | Drums | D1 p19 |  |
| F05-P0528 | Electric Piano 2 | G | G006 | single | Keyboard | D1 p19 |  |
| F05-P0529 | Brass Section 1 | G | G062 | single | Brass | D1 p19 |  |
| F05-P0530 | Melodic Tom 1 | G | G118 | single | Drums | D1 p19 |  |
| F05-P0531 | Harpsichord | G | G007 | single | Keyboard | D1 p19 |  |
| F05-P0532 | Synth Brass 1 | G | G063 | double | FastSynth | D1 p19 |  |
| F05-P0533 | Synth Drum | G | G119 | single | Drums | D1 p19 |  |
| F05-P0534 | Clavi. | G | G008 | single | Keyboard | D1 p19 |  |
| F05-P0535 | Synth Brass 2 | G | G064 | single | FastSynth | D1 p19 |  |
| F05-P0536 | Reverse Cymbal | G | G120 | single | SE | D1 p19 |  |
| F05-P0537 | Celesta | G | G009 | single | Bell/Mallet | D1 p19 |  |
| F05-P0538 | Soprano Sax | G | G065 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0539 | Gtr.Fret Noise | G | G121 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0540 | Glockenspiel | G | G010 | double | Bell/Mallet | D1 p19 |  |
| F05-P0541 | Alto Sax | G | G066 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0542 | Breath Noise | G | G122 | single | SE | D1 p19 |  |
| F05-P0543 | Music Box | G | G011 | single | Bell/Mallet | D1 p19 |  |
| F05-P0544 | Tenor Sax | G | G067 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0545 | Seashore | G | G123 | double | SE | D1 p19 |  |
| F05-P0546 | Vibraphone | G | G012 | single | Bell/Mallet | D1 p19 |  |
| F05-P0547 | Baritone Sax | G | G068 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0548 | Bird Tweet 1 | G | G124 | double | SE | D1 p19 |  |
| F05-P0549 | Marimba | G | G013 | single | Bell/Mallet | D1 p19 |  |
| F05-P0550 | Oboe | G | G069 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0551 | Telephone Ring 1 | G | G125 | single | SE | D1 p19 |  |
| F05-P0552 | Xylophone | G | G014 | single | Bell/Mallet | D1 p19 |  |
| F05-P0553 | English Horn | G | G070 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0554 | Helicopter | G | G126 | double | SE | D1 p19 |  |
| F05-P0555 | Tubular Bells | G | G015 | single | Bell/Mallet | D1 p19 |  |
| F05-P0556 | Bassoon | G | G071 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0557 | Applause | G | G127 | double | SE | D1 p19 |  |
| F05-P0558 | Santur | G | G016 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0559 | Clarinet | G | G072 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0560 | Gun Shot | G | G128 | single | SE | D1 p19 |  |
| F05-P0561 | Drawbar Organ 1 | G | G017 | single | Organ | D1 p19 |  |
| F05-P0562 | Piccolo | G | G073 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0563 | STANDARD Kit | G | g(d)001 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0564 | PercussiveOrgan1 | G | G018 | single | Organ | D1 p19 |  |
| F05-P0565 | Flute | G | G074 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0566 | ROOM Kit | G | g(d)009 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0567 | Rock Organ | G | G019 | single | Organ | D1 p19 |  |
| F05-P0568 | Recorder | G | G075 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0569 | POWER Kit | G | g(d)017 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0570 | Church Organ | G | G020 | single | Organ | D1 p19 |  |
| F05-P0571 | Pan Flute | G | G076 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0572 | ELECTRONIC Kit | G | g(d)025 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0573 | Reed Organ | G | G021 | double | Organ | D1 p19 |  |
| F05-P0574 | Blown Bottle | G | G077 | double | Woodwind/Reed | D1 p19 |  |
| F05-P0575 | ANALOG Kit | G | g(d)026 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0576 | Accordion 1 | G | G022 | double | Woodwind/Reed | D1 p19 |  |
| F05-P0577 | Shakuhachi | G | G078 | double | Woodwind/Reed | D1 p19 |  |
| F05-P0578 | JAZZ Kit | G | g(d)033 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0579 | Harmonica | G | G023 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0580 | Whistle | G | G079 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0581 | BRUSH Kit | G | g(d)041 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0582 | Bandoneon | G | G024 | double | Woodwind/Reed | D1 p19 |  |
| F05-P0583 | Ocarina | G | G080 | single | Woodwind/Reed | D1 p19 |  |
| F05-P0584 | ORCHESTRA Kit | G | g(d)049 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0585 | Nylon Guitar 1 | G | G025 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0586 | Detuned Square | G | G081 | double | FastSynth | D1 p19 |  |
| F05-P0587 | SFX Kit | G | g(d)057 | double | Drums | D1 p19 | document's own GM variant row |
| F05-P0588 | Steel Guitar | G | G026 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0589 | Detuned Sawtooth | G | G082 | double | FastSynth | D1 p19 |  |
| F05-P0590 | Jazz Guitar | G | G027 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0591 | Synth Calliope | G | G083 | double | FastSynth | D1 p19 |  |
| F05-P0592 | Clean Guitar | G | G028 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0593 | Chiff Lead | G | G084 | double | FastSynth | D1 p19 |  |
| F05-P0594 | Muted Guitar | G | G029 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0595 | Charang | G | G085 | double | LeadSynth | D1 p19 |  |
| F05-P0596 | Overdrive Guitar | G | G030 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0597 | Air Voice | G | G086 | double | Vocal/Airy | D1 p19 |  |
| F05-P0598 | DistortionGuitar | G | G031 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0599 | 5th Sawtooth | G | G087 | double | LeadSynth | D1 p19 |  |
| F05-P0600 | Guitar Harmonics | G | G032 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0601 | Bass & Lead | G | G088 | single | LeadSynth | D1 p19 |  |
| F05-P0602 | Acoustic Bass | G | G033 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0603 | Fantasia | G | G089 | double | FastSynth | D1 p19 |  |
| F05-P0604 | Fingered Bass | G | G034 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0605 | Warm Pad | G | G090 | single | SlowSynth | D1 p19 |  |
| F05-P0606 | Picked Bass | G | G035 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0607 | Polyphonic Synth | G | G091 | double | FastSynth | D1 p19 |  |
| F05-P0608 | Fretless Bass | G | G036 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0609 | Space Voice | G | G092 | double | Vocal/Airy | D1 p19 |  |
| F05-P0610 | Slap Bass 1 | G | G037 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0611 | Bowed Glass | G | G093 | double | SlowSynth | D1 p19 |  |
| F05-P0612 | Slap Bass 2 | G | G038 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0613 | Metallic Pad | G | G094 | double | SlowSynth | D1 p19 |  |
| F05-P0614 | Synth Bass 1 | G | G039 | double | Bass/Synth Bass | D1 p19 |  |
| F05-P0615 | Halo Pad | G | G095 | double | Vocal/Airy | D1 p19 |  |
| F05-P0616 | Synth Bass 2 | G | G040 | single | Bass/Synth Bass | D1 p19 |  |
| F05-P0617 | Sweep Pad | G | G096 | single | MotionSynth | D1 p19 |  |
| F05-P0618 | Violin | G | G041 | single | Strings | D1 p19 |  |
| F05-P0619 | Ice Rain | G | G097 | double | FastSynth | D1 p19 |  |
| F05-P0620 | Viola | G | G042 | single | Strings | D1 p19 |  |
| F05-P0621 | Sound Track | G | G098 | double | SlowSynth | D1 p19 |  |
| F05-P0622 | Cello | G | G043 | single | Strings | D1 p19 |  |
| F05-P0623 | Crystal | G | G099 | double | Bell/Mallet | D1 p19 |  |
| F05-P0624 | Contrabass | G | G044 | single | Strings | D1 p19 |  |
| F05-P0625 | Atmosphere | G | G100 | double | FastSynth | D1 p19 |  |
| F05-P0626 | Tremolo Strings | G | G045 | single | Strings | D1 p19 |  |
| F05-P0627 | Brightness | G | G101 | double | FastSynth | D1 p19 |  |
| F05-P0628 | Pizzicato Str. | G | G046 | single | Strings | D1 p19 |  |
| F05-P0629 | Goblins | G | G102 | double | MotionSynth | D1 p19 |  |
| F05-P0630 | Orchestral Harp | G | G047 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0631 | Echo Drops | G | G103 | single | FastSynth | D1 p19 |  |
| F05-P0632 | Timpani | G | G048 | double | Drums | D1 p19 |  |
| F05-P0633 | Star Theme | G | G104 | double | FastSynth | D1 p19 |  |
| F05-P0634 | Strings | G | G049 | single | Strings | D1 p19 |  |
| F05-P0635 | Sitar 1 | G | G105 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0636 | Slow Strings | G | G050 | single | Strings | D1 p19 |  |
| F05-P0637 | Banjo | G | G106 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0638 | Synth Strings 1 | G | G051 | double | Strings | D1 p19 |  |
| F05-P0639 | Shamisen | G | G107 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0640 | Synth Strings 2 | G | G052 | single | Strings | D1 p19 |  |
| F05-P0641 | Koto | G | G108 | single | Guitar/Plucked | D1 p19 |  |
| F05-P0642 | Choir Aahs 1 | G | G053 | single | Vocal/Airy | D1 p19 |  |
| F05-P0643 | Kalimba | G | G109 | single | Bell/Mallet | D1 p19 |  |
| F05-P0644 | Voice Oohs | G | G054 | single | Vocal/Airy | D1 p19 |  |
| F05-P0645 | Bagpipe | G | G110 | double | Woodwind/Reed | D1 p19 |  |
| F05-P0646 | Synth Vox | G | G055 | single | Vocal/Airy | D1 p19 |  |
| F05-P0647 | Fiddle | G | G111 | single | Strings | D1 p19 |  |
| F05-P0648 | Orchestra Hit | G | G056 | single | Hit/Arpg | D1 p19 |  |
| F05-P0649 | Shanai | G | G112 | single | Woodwind/Reed | D1 p19 |  |

## Factory FX algorithms and routing

**Found in documentation: yes** - the algorithm catalogue and the slot routing are both published in D2. **Per-program FX assignments are not documented**: no public document states which insert algorithms, chain order, send levels or master effect a given factory program uses. That information exists only inside the instrument's own factory data, which is out of bounds for this project, so per-patch chains have to be reconstructed by ear against the catalogue below.

**Slots (D2).** The TR provides **one** insert effect (IFX) - D2's compatibility appendix states it directly ("The TR provides one insert effect (IFX)"; a TRITON file that uses five insert effects loads only IFX1). Alongside it sit two master effects MFX1-MFX2, whose I/Os are mono-in / stereo-out, which output wet signal only and whose output levels are set by Rtn1/Rtn2, and a Master EQ after them. D2's routing: oscillator / filter / amplifier -> BUS Select (L/R or IFX) -> IFX -> post-IFX BUS Select (L/R, 1, 2, 1/2, Off) -> sends S1/S2 to MFX1/MFX2 -> returns mixed into the L/R bus -> Master EQ -> output. Both the insert effect and the two master effects select from the same catalogue of 90 entries (00: No Effect plus the 89 effect types below).

**Selectable ranges (D2).**

| Slot (as the document names it) | Documented range | Source |
|---|---|---|
| Insert Effect | 00 - 89 | D2 |
| Master Effect 1, 2 | 00 - 89 | D2 |

Slot rules read from D2: first_type = St.Amp Sim, ifx_range = 00..89, insert_effects = 1, last_type_number = 89, master_effect_io = mono in / stereo out, master_effects = 2, mfx_range = 00..89, stated_type_count = 89.

**Algorithm catalogue (D2), 90 entries, read from the Parameter Guide's own effect index.**

| # | Algorithm as printed |
|---|---|
| 00 | No Effect |
| 01 | St.Amp Sim (Stereo Amp Simulation) |
| 02 | St.Compressor (Stereo Compressor) |
| 03 | St.Limiter (Stereo Limiter) |
| 04 | Mltband Limit (Multiband Limiter) |
| 05 | St.Gate (Stereo Gate) |
| 06 | OD/HiGain Wah (Overdrive/Hi.Gain Wah) |
| 07 | St.Para.4EQ (Stereo Parametric 4-Band EQ) |
| 08 | St.Graphic7EQ (Stereo Graphic 7-Band EQ) |
| 09 | St.Wah/AutoW (Stereo Wah/Auto Wah) |
| 10 | St.Rndm Filter (Stereo Random Filter) |
| 11 | St.Exct/Enhcr (Stereo Exciter/Enhancer) |
| 12 | St.Sub OSC (Stereo Sub Oscillator) |
| 13 | Talking Mod (Talking Modulator) |
| 14 | St.Decimator (Stereo Decimator) |
| 15 | St.AnalogRecd (Stereo Analog Record) |
| 16 | St.Chorus (Stereo Chorus) |
| 17 | St.HarmnicCho (Stereo Harmonic Chorus) |
| 18 | MltTap ChoDly (Multitap Chorus/Delay) |
| 19 | Ensemble |
| 20 | St.Flanger (Stereo Flanger) |
| 21 | St.Rndm Flang (Stereo Random Flanger) |
| 22 | St.Env.Flanger (Stereo Envelope Flanger) |
| 23 | St.Phaser (Stereo Phaser) |
| 24 | St.Rndm Phasr (Stereo Random Phaser) |
| 25 | St.Env.Phaser (Stereo Envelope Phaser) |
| 26 | St.BiphaseMod (Stereo Biphase Modulation) |
| 27 | St.Vibrato (Stereo Vibrato) |
| 28 | St.AutoFd Mod (Stereo Auto Fade Modulation) |
| 29 | 2Voice Reso (2Voice Resonator) |
| 30 | Doppler |
| 31 | Scratch |
| 32 | St.Tremolo (Stereo Tremolo) |
| 33 | St.Env.Tremlo (Stereo Envelope Tremolo) |
| 34 | St.Auto Pan (Stereo Auto Pan) |
| 35 | St.Phasr+Trml (Stereo Phaser + Tremolo) |
| 36 | St.Ring Mod (Stereo Ring Modulator) |
| 37 | Detune |
| 38 | Pitch Shifter |
| 39 | PitchShft Mod (Pitch Shift Modulation) |
| 40 | Rotary SP (Rotary Speaker) |
| 41 | Early Reflect (Early Reflections) |
| 42 | Auto Reverse |
| 43 | LCR Delay (L/C/R Delay) |
| 44 | St/Cross Dly (Stereo/Cross Delay) |
| 45 | St.MltTap Dly (Stereo Multitap Delay) |
| 46 | St.Mod. Delay (Stereo Modulation Delay) |
| 47 | St.DynamicDly (Stereo Dynamic Delay) |
| 48 | St.AutoPanDly (Stereo Auto Panning Delay) |
| 49 | LCR BPM Delay (L/C/R BPM Delay) |
| 50 | St.BPM Delay (Stereo BPM Delay) |
| 51 | Sequence Dly (Sequence Delay) |
| 52 | Rev Hall (Reverb Hall) |
| 53 | Rev Smth. Hall (Reverb Smooth Hall) |
| 54 | Rev Wet Plate (Reverb Wet Plate) |
| 55 | Rev Dry Plate (Reverb Dry Plate) |
| 56 | Rev Room (Reverb Room) |
| 57 | Rev Brt. Room (Reverb Bright Room) |
| 58 | P4EQ–Exciter (Parametric 4-Band EQ – Exciter) |
| 59 | P4EQ–Wah (Parametric 4-Band EQ – Wah/Auto Wah) |
| 60 | P4EQ–Cho/Fl (Parametric 4-Band EQ – Chorus/Flanger) |
| 61 | P4EQ–Phaser (Parametric 4-Band EQ – Phaser) |
| 62 | P4EQ–M.Dly (Parametric 4-Band EQ – Multitap Delay) |
| 63 | Comp–Wah (Compressor – Wah/Auto Wah) |
| 64 | Comp–AmpSim (Compressor – Amp Simulation) |
| 65 | Comp–OD/HG (Compressor – Overdrive/Hi.Gain) |
| 66 | Comp–P4EQ (Compressor – Parametric 4-Band EQ) |
| 67 | Comp–Cho/Fl (Compressor – Chorus/Flanger) |
| 68 | Comp–Phaser (Compressor – Phaser) |
| 69 | Comp–M.Dly (Compressor – Multitap Delay) |
| 70 | Limit–P4EQ (Limiter – Parametric 4-Band EQ) |
| 71 | Limit–Cho/Fl (Limiter – Chorus/Flanger) |
| 72 | Limit–Phaser (Limiter – Phaser) |
| 73 | Limit–M.Dly (Limiter – Multitap Delay) |
| 74 | Exct–Comp (Exciter – Compressor) |
| 75 | Exct–Limiter (Exciter – Limiter) |
| 76 | Exct–Cho/Fl (Exciter – Chorus/Flanger) |
| 77 | Exct–Phaser (Exciter – Phaser) |
| 78 | Exct–M.Dly (Exciter – Multitap Delay) |
| 79 | OD/HG–AmpSim (Overdrive/Hi.Gain – Amp Simulation) |
| 80 | OD/HG–Cho/Fl (Overdrive/Hi.Gain – Chorus/Flanger) |
| 81 | OD/HG–Phaser (Overdrive/Hi.Gain – Phaser) |
| 82 | OD/HG–M.Dly (Overdrive/Hi.Gain – Multitap Delay) |
| 83 | Wah–AmpSim (Wah/Auto Wah – Amp Simulation) |
| 84 | Deci–AmpSim (Decimator – Amp Simulation) |
| 85 | Deci–Comp (Decimator – Compressor) |
| 86 | AmpSim–Trml (Amp Simulation – Tremolo) |
| 87 | Cho/Fl–M.Dly (Chorus/Flanger – Multitap Delay) |
| 88 | Phasr–Cho/Fl (Phaser – Chorus/Flanger) |
| 89 | Reverb–Gate |

## PCM wave list (engine input, not content to ingest)

D1 publishes the ROM multisample and drumsample name lists. They are engine input: they say what the reference machine's waves are called and how they are numbered. They are not content we may copy, and the numbers are the document's own numbering.

### Multisamples

| Shipped label | Number | Documented name | Group | Source | Note |
|---|---|---|---|---|---|
| F05-W0001 | 0 | A.Piano | Rom | D1 p28 | |
| F05-W0002 | 0 | St.Piano mf -L | EX | D1 p29 | |
| F05-W0003 | 1 | A.Piano-M1 | Rom | D1 p28 | |
| F05-W0004 | 1 | St.Piano mf -R | EX | D1 p29 | |
| F05-W0005 | 2 | E.Grand Piano | Rom | D1 p28 | |
| F05-W0006 | 2 | St.Piano f -L | EX | D1 p29 | |
| F05-W0007 | 3 | E.P.-FM 1 | Rom | D1 p28 | |
| F05-W0008 | 3 | St.Piano f -R | EX | D1 p29 | |
| F05-W0009 | 4 | E.P.-FM 1 LP | Rom | D1 p28 | |
| F05-W0010 | 4 | E.P.Bright mf | EX | D1 p29 | |
| F05-W0011 | 5 | E.P.-FM 2 | Rom | D1 p28 | |
| F05-W0012 | 5 | E.P.Bright f | EX | D1 p29 | |
| F05-W0013 | 6 | E.P.-FM 3 | Rom | D1 p28 | |
| F05-W0014 | 6 | E.P.Dark mf | EX | D1 p29 | |
| F05-W0015 | 7 | E.P.-FM 3 LP | Rom | D1 p28 | |
| F05-W0016 | 7 | E.P.Dark f | EX | D1 p29 | |
| F05-W0017 | 8 | E.P.-Dyno Soft | Rom | D1 p28 | |
| F05-W0018 | 8 | Vibraphone 2 | EX | D1 p29 | |
| F05-W0019 | 9 | E.P.-Dyno Sft LP | Rom | D1 p28 | |
| F05-W0020 | 9 | Flute-Vibrato | EX | D1 p29 | |
| F05-W0021 | 10 | E.P.-Dyno Medium | Rom | D1 p28 | |
| F05-W0022 | 10 | Oboe-Vibrato | EX | D1 p29 | |
| F05-W0023 | 11 | E.P.-Dyno Med LP | Rom | D1 p28 | |
| F05-W0024 | 11 | Harmonica-Wah | EX | D1 p29 | |
| F05-W0025 | 12 | E.P.-Stage Soft | Rom | D1 p28 | |
| F05-W0026 | 12 | 2 Trumpets | EX | D1 p29 | |
| F05-W0027 | 13 | E.P.-Sta Soft LP | Rom | D1 p28 | |
| F05-W0028 | 13 | Voice Woh | EX | D1 p29 | |
| F05-W0029 | 14 | E.P.-Stage Hard | Rom | D1 p28 | |
| F05-W0030 | 14 | Voice Dah | EX | D1 p29 | |
| F05-W0031 | 15 | E.P.-Sta Hard LP | Rom | D1 p28 | |
| F05-W0032 | 15 | String Ens. -R | EX | D1 p29 | |
| F05-W0033 | 16 | E.P.-Wurly | Rom | D1 p28 | |
| F05-W0034 | 16 | String Ens.REV-R | EX | D1 p29 | |
| F05-W0035 | 17 | E.P.-Wurly LP | Rom | D1 p28 | |
| F05-W0036 | 17 | String Quartet 3 | EX | D1 p29 | |
| F05-W0037 | 18 | E.P. Pad 1 | Rom | D1 p28 | |
| F05-W0038 | 18 | String Qurt3 REV | EX | D1 p29 | |
| F05-W0039 | 19 | E.P. Pad 1 LP | Rom | D1 p28 | |
| F05-W0040 | 19 | Nylon Guitar p | EX | D1 p29 | |
| F05-W0041 | 20 | E.P. Pad 2 | Rom | D1 p28 | |
| F05-W0042 | 20 | Nylon Guitar mf | EX | D1 p29 | |
| F05-W0043 | 21 | E.P. Pad 3 | Rom | D1 p28 | |
| F05-W0044 | 21 | A.Gtr D-Pick | EX | D1 p29 | |
| F05-W0045 | 22 | E.P. Pad 3 LP | Rom | D1 p28 | |
| F05-W0046 | 22 | A.Gtr D-Gliss | EX | D1 p29 | |
| F05-W0047 | 23 | Clav 1 | Rom | D1 p28 | |
| F05-W0048 | 23 | Vocoder Voice | EX | D1 p29 | |
| F05-W0049 | 24 | Harpsichord-Sngl | Rom | D1 p28 | |
| F05-W0050 | 24 | Sword | EX | D1 p29 | |
| F05-W0051 | 25 | E.Organ-Perc 1 | Rom | D1 p28 | |
| F05-W0052 | 25 | Cannon | EX | D1 p29 | |
| F05-W0053 | 26 | E.Organ-Perc 2 | Rom | D1 p28 | |
| F05-W0054 | 26 | BD-amb | EX | D1 p29 | |
| F05-W0055 | 27 | E.Organ-Perc 3 | Rom | D1 p28 | |
| F05-W0056 | 27 | BD-Dance a | EX | D1 p29 | |
| F05-W0057 | 28 | E.Organ-2' Perc | Rom | D1 p28 | |
| F05-W0058 | 28 | BD-Dance b | EX | D1 p29 | |
| F05-W0059 | 29 | E.Organ-M1 1 | Rom | D1 p28 | |
| F05-W0060 | 29 | SD-Roll-Open | EX | D1 p29 | |
| F05-W0061 | 30 | E.Organ-M1 2 | Rom | D1 p28 | |
| F05-W0062 | 30 | SD-Roll-Closed | EX | D1 p29 | |
| F05-W0063 | 31 | E.Organ-N1 5 | Rom | D1 p28 | |
| F05-W0064 | 31 | SD-Dance a | EX | D1 p29 | |
| F05-W0065 | 32 | E.Organ-N1 3 | Rom | D1 p28 | |
| F05-W0066 | 32 | SD-Dance b | EX | D1 p29 | |
| F05-W0067 | 33 | E.Organ-N1 3 LP | Rom | D1 p28 | |
| F05-W0068 | 33 | SD-Dance c | EX | D1 p29 | |
| F05-W0069 | 34 | E.Organ-Jazz 1TR | Rom | D1 p28 | |
| F05-W0070 | 34 | SD-Dance d | EX | D1 p29 | |
| F05-W0071 | 35 | E.Organ-Jazz 3TR | Rom | D1 p28 | |
| F05-W0072 | 35 | Tom Low | EX | D1 p29 | |
| F05-W0073 | 36 | E.Organ-Perc 3rd | Rom | D1 p28 | |
| F05-W0074 | 36 | H.Hat-Dance a | EX | D1 p29 | |
| F05-W0075 | 37 | E.Organ-Vox | Rom | D1 p28 | |
| F05-W0076 | 37 | H.Hat-Dance b | EX | D1 p29 | |
| F05-W0077 | 38 | E.Organ-Vox 1TR | Rom | D1 p28 | |
| F05-W0078 | 38 | H.Hat-Dance c | EX | D1 p29 | |
| F05-W0079 | 39 | E.Organ-Vox 2TR | Rom | D1 p28 | |
| F05-W0080 | 39 | H.Hat-Dance d | EX | D1 p29 | |
| F05-W0081 | 40 | E.Organ-Vox 3TR | Rom | D1 p28 | |
| F05-W0082 | 40 | Ride-Edge 3 | EX | D1 p29 | |
| F05-W0083 | 41 | E.Organ-Soft | Rom | D1 p28 | |
| F05-W0084 | 41 | WD-Native Drum | EX | D1 p29 | |
| F05-W0085 | 42 | E.Organ-Full | Rom | D1 p28 | |
| F05-W0086 | 42 | WD-Ethno SD | EX | D1 p29 | |
| F05-W0087 | 43 | E.Organ-Dist | Rom | D1 p28 | |
| F05-W0088 | 43 | Dance Perc a | EX | D1 p29 | |
| F05-W0089 | 44 | Positive 1 | Rom | D1 p28 | |
| F05-W0090 | 44 | Dance Perc b | EX | D1 p29 | |
| F05-W0091 | 45 | Positive 2 | Rom | D1 p28 | |
| F05-W0092 | 46 | Pipe-Mixture 1 | Rom | D1 p28 | |
| F05-W0093 | 47 | Pipe-Mixture 2 | Rom | D1 p28 | |
| F05-W0094 | 48 | Pipe-Reed | Rom | D1 p28 | |
| F05-W0095 | 49 | Pipe-Full | Rom | D1 p28 | |
| F05-W0096 | 50 | Kalimba | Rom | D1 p28 | |
| F05-W0097 | 51 | Kalimba-Mute | Rom | D1 p28 | |
| F05-W0098 | 52 | Kalimba N1 | Rom | D1 p28 | |
| F05-W0099 | 53 | Music Box 1 | Rom | D1 p28 | |
| F05-W0100 | 54 | Music Box 1 LP | Rom | D1 p28 | |
| F05-W0101 | 55 | Music Box 2 | Rom | D1 p28 | |
| F05-W0102 | 56 | Music Box 2 LP | Rom | D1 p28 | |
| F05-W0103 | 57 | Marimba | Rom | D1 p28 | |
| F05-W0104 | 58 | Marimba LP | Rom | D1 p28 | |
| F05-W0105 | 59 | Xylophone | Rom | D1 p28 | |
| F05-W0106 | 60 | Vibraphone | Rom | D1 p28 | |
| F05-W0107 | 61 | Vibraphone LP | Rom | D1 p28 | |
| F05-W0108 | 62 | Celesta | Rom | D1 p28 | |
| F05-W0109 | 63 | Celesta LP | Rom | D1 p28 | |
| F05-W0110 | 64 | Glockenspiel | Rom | D1 p28 | |
| F05-W0111 | 65 | Glockenspiel LP | Rom | D1 p28 | |
| F05-W0112 | 66 | Tubular Bell | Rom | D1 p28 | |
| F05-W0113 | 67 | Log Drum | Rom | D1 p28 | |
| F05-W0114 | 68 | Steel Drum | Rom | D1 p28 | |
| F05-W0115 | 69 | Steel Drum LP | Rom | D1 p28 | |
| F05-W0116 | 70 | Gamelan | Rom | D1 p28 | |
| F05-W0117 | 71 | Metal FM Bell | Rom | D1 p28 | |
| F05-W0118 | 72 | FM Bell | Rom | D1 p28 | |
| F05-W0119 | 73 | FM Glass Bell | Rom | D1 p28 | |
| F05-W0120 | 74 | Ensemble Bell | Rom | D1 p28 | |
| F05-W0121 | 75 | Flute | Rom | D1 p28 | |
| F05-W0122 | 76 | Piccolo | Rom | D1 p28 | |
| F05-W0123 | 77 | Pan Flute | Rom | D1 p28 | |
| F05-W0124 | 78 | Shakuhachi | Rom | D1 p28 | |
| F05-W0125 | 79 | Shakuhachi-Atk | Rom | D1 p28 | |
| F05-W0126 | 80 | Bottle | Rom | D1 p28 | |
| F05-W0127 | 81 | Recorder | Rom | D1 p28 | |
| F05-W0128 | 82 | Ocarina | Rom | D1 p28 | |
| F05-W0129 | 83 | Clarinet | Rom | D1 p28 | |
| F05-W0130 | 84 | DoubleReed | Rom | D1 p28 | |
| F05-W0131 | 85 | Oboe | Rom | D1 p28 | |
| F05-W0132 | 86 | English Horn | Rom | D1 p28 | |
| F05-W0133 | 87 | Bassoon | Rom | D1 p28 | |
| F05-W0134 | 88 | Baritone Sax-T1 | Rom | D1 p28 | |
| F05-W0135 | 89 | Baritone Sax | Rom | D1 p28 | |
| F05-W0136 | 90 | Tenor Sax-M1 | Rom | D1 p28 | |
| F05-W0137 | 91 | TenorSax-Medium | Rom | D1 p28 | |
| F05-W0138 | 92 | Alto Sax-01/W | Rom | D1 p28 | |
| F05-W0139 | 93 | Alto Sax-Soft | Rom | D1 p28 | |
| F05-W0140 | 94 | SopranoSax-Soft | Rom | D1 p28 | |
| F05-W0141 | 95 | Musette | Rom | D1 p28 | |
| F05-W0142 | 96 | Musette LP | Rom | D1 p28 | |
| F05-W0143 | 97 | Accordion | Rom | D1 p28 | |
| F05-W0144 | 98 | Harmonica | Rom | D1 p28 | |
| F05-W0145 | 99 | Bagpipe | Rom | D1 p28 | |
| F05-W0146 | 100 | F.Horn-T1 | Rom | D1 p28 | |
| F05-W0147 | 101 | French HornEns. | Rom | D1 p28 | |
| F05-W0148 | 102 | Flugel Horn | Rom | D1 p28 | |
| F05-W0149 | 103 | Tuba | Rom | D1 p28 | |
| F05-W0150 | 104 | Trombone-Soft | Rom | D1 p28 | |
| F05-W0151 | 105 | Trombone-Bright | Rom | D1 p28 | |
| F05-W0152 | 106 | Trumpet-Medium | Rom | D1 p28 | |
| F05-W0153 | 107 | Trumpet-Muted | Rom | D1 p28 | |
| F05-W0154 | 108 | Brass Ens-Sharp | Rom | D1 p28 | |
| F05-W0155 | 109 | Brass Ens-1 | Rom | D1 p28 | |
| F05-W0156 | 110 | Brass Ens-1 LP | Rom | D1 p28 | |
| F05-W0157 | 111 | Brass Ens-2 | Rom | D1 p28 | |
| F05-W0158 | 112 | Brass Ens-2 LP | Rom | D1 p28 | |
| F05-W0159 | 113 | Voice-Choir | Rom | D1 p28 | |
| F05-W0160 | 114 | Voice-Choir REV | Rom | D1 p28 | |
| F05-W0161 | 115 | Voice-Pop Ooh | Rom | D1 p28 | |
| F05-W0162 | 116 | Voice-PopOoh REV | Rom | D1 p28 | |
| F05-W0163 | 117 | Voice-Pop Ah | Rom | D1 p28 | |
| F05-W0164 | 118 | Voice-Pop Ah REV | Rom | D1 p28 | |
| F05-W0165 | 119 | Voice-Doo | Rom | D1 p28 | |
| F05-W0166 | 120 | Voice-Doo LP | Rom | D1 p28 | |
| F05-W0167 | 121 | Voice Wave 1 | Rom | D1 p28 | |
| F05-W0168 | 122 | Voice Wave 2 | Rom | D1 p28 | |
| F05-W0169 | 123 | String Ens. -L | Rom | D1 p28 | |
| F05-W0170 | 124 | String Ens.REV-L | Rom | D1 p28 | |
| F05-W0171 | 125 | Pizzicato Ens. | Rom | D1 p28 | |
| F05-W0172 | 126 | Violin | Rom | D1 p28 | |
| F05-W0173 | 127 | Viola | Rom | D1 p28 | |
| F05-W0174 | 128 | Cello&Contrabass | Rom | D1 p28 | |
| F05-W0175 | 129 | String Quartet 1 | Rom | D1 p28 | |
| F05-W0176 | 130 | String Quartet 2 | Rom | D1 p28 | |
| F05-W0177 | 131 | String Qurt2 REV | Rom | D1 p28 | |
| F05-W0178 | 132 | Pizzicato | Rom | D1 p28 | |
| F05-W0179 | 133 | A.Guitar-Nylon | Rom | D1 p28 | |
| F05-W0180 | 134 | A.Guitar-Finger | Rom | D1 p28 | |
| F05-W0181 | 135 | A.Guitar-Pick | Rom | D1 p28 | |
| F05-W0182 | 136 | A.Gtr-Harmonics | Rom | D1 p28 | |
| F05-W0183 | 137 | Clean Gtr 1-Stra | Rom | D1 p28 | |
| F05-W0184 | 138 | Clean Gtr 2-Stra | Rom | D1 p28 | |
| F05-W0185 | 139 | Clean Gtr 3-Tele | Rom | D1 p28 | |
| F05-W0186 | 140 | Clean Gtr-Mute 1 | Rom | D1 p28 | |
| F05-W0187 | 141 | Clean Gtr-Mute 2 | Rom | D1 p28 | |
| F05-W0188 | 142 | Funky Gtr 1-Stra | Rom | D1 p28 | |
| F05-W0189 | 143 | Funky Gtr 2-Stra | Rom | D1 p28 | |
| F05-W0190 | 144 | Jazz Guitar | Rom | D1 p28 | |
| F05-W0191 | 145 | Pedal Steel Gtr. | Rom | D1 p28 | |
| F05-W0192 | 146 | E.Gtr-Harmonics | Rom | D1 p28 | |
| F05-W0193 | 147 | OverDrive Guitar | Rom | D1 p28 | |
| F05-W0194 | 148 | Dist. Guitar 1 | Rom | D1 p28 | |
| F05-W0195 | 149 | Dist. Guitar 2 | Rom | D1 p28 | |
| F05-W0196 | 150 | Dist.Gtr-Harmo | Rom | D1 p28 | |
| F05-W0197 | 151 | Dist.Gtr-Mute | Rom | D1 p28 | |
| F05-W0198 | 152 | Power Chord | Rom | D1 p28 | |
| F05-W0199 | 153 | Fret Noise | Rom | D1 p28 | |
| F05-W0200 | 154 | A.Bass 1 | Rom | D1 p28 | |
| F05-W0201 | 155 | A.Bass 2 | Rom | D1 p28 | |
| F05-W0202 | 156 | A.Bass 2 LP | Rom | D1 p28 | |
| F05-W0203 | 157 | FingerBass-Clear | Rom | D1 p28 | |
| F05-W0204 | 158 | Finger Bass 1 | Rom | D1 p28 | |
| F05-W0205 | 159 | Finger Bass 1 LP | Rom | D1 p28 | |
| F05-W0206 | 160 | Finger Bass 2 | Rom | D1 p28 | |
| F05-W0207 | 161 | Finger Bass 2 LP | Rom | D1 p28 | |
| F05-W0208 | 162 | Pick Bass-M1 | Rom | D1 p28 | |
| F05-W0209 | 163 | Pick Bass-Hard | Rom | D1 p28 | |
| F05-W0210 | 164 | Pick Bass | Rom | D1 p28 | |
| F05-W0211 | 165 | Pick Bass LP | Rom | D1 p28 | |
| F05-W0212 | 166 | SlapBass-ThumbJB | Rom | D1 p28 | |
| F05-W0213 | 167 | SlapBass-Thumb 1 | Rom | D1 p28 | |
| F05-W0214 | 168 | SlapBass-Thum1LP | Rom | D1 p28 | |
| F05-W0215 | 169 | SlapBass-Thumb 2 | Rom | D1 p28 | |
| F05-W0216 | 170 | SlapBass-Thum2LP | Rom | D1 p28 | |
| F05-W0217 | 171 | SlapBass-Pull JB | Rom | D1 p28 | |
| F05-W0218 | 172 | SlapBass-Pull | Rom | D1 p28 | |
| F05-W0219 | 173 | SlapBass-Mute | Rom | D1 p28 | |
| F05-W0220 | 174 | SlapBass-Mute LP | Rom | D1 p28 | |
| F05-W0221 | 175 | SlapBass-M1 | Rom | D1 p28 | |
| F05-W0222 | 176 | Fretless Bass | Rom | D1 p28 | |
| F05-W0223 | 177 | Fretless Bass-M1 | Rom | D1 p28 | |
| F05-W0224 | 178 | Bass Harmonics | Rom | D1 p28 | |
| F05-W0225 | 179 | Bass Harmo LP | Rom | D1 p28 | |
| F05-W0226 | 180 | Sitar | Rom | D1 p28 | |
| F05-W0227 | 181 | Sitar&Tambura | Rom | D1 p28 | |
| F05-W0228 | 182 | Sitar&Tambura LP | Rom | D1 p28 | |
| F05-W0229 | 183 | Santur | Rom | D1 p28 | |
| F05-W0230 | 184 | Santur LP | Rom | D1 p28 | |
| F05-W0231 | 185 | Mandolin | Rom | D1 p28 | |
| F05-W0232 | 186 | Mandolin LP | Rom | D1 p28 | |
| F05-W0233 | 187 | Banjo | Rom | D1 p28 | |
| F05-W0234 | 188 | Banjo LP | Rom | D1 p28 | |
| F05-W0235 | 189 | Shamisen | Rom | D1 p28 | |
| F05-W0236 | 190 | Koto | Rom | D1 p28 | |
| F05-W0237 | 191 | Harp | Rom | D1 p28 | |
| F05-W0238 | 192 | Harp LP | Rom | D1 p28 | |
| F05-W0239 | 193 | Harp-Glissando | Rom | D1 p28 | |
| F05-W0240 | 194 | Harp-Gliss REV | Rom | D1 p28 | |
| F05-W0241 | 195 | Sine Pop Bass | Rom | D1 p28 | |
| F05-W0242 | 196 | Syn Bass-Reso 1 | Rom | D1 p28 | |
| F05-W0243 | 197 | Syn Bass-Reso 2 | Rom | D1 p28 | |
| F05-W0244 | 198 | Syn Bass-Dirty | Rom | D1 p28 | |
| F05-W0245 | 199 | Syn Bass-FM 1 | Rom | D1 p28 | |
| F05-W0246 | 200 | Syn Bass-FM 1 LP | Rom | D1 p28 | |
| F05-W0247 | 201 | Syn Bass-FM 2 | Rom | D1 p28 | |
| F05-W0248 | 202 | Syn Bass-FM 2 LP | Rom | D1 p28 | |
| F05-W0249 | 203 | Syn Bass-TB | Rom | D1 p28 | |
| F05-W0250 | 204 | Syn Bass-RBSaw | Rom | D1 p28 | |
| F05-W0251 | 205 | Syn Bass-RBSqr | Rom | D1 p28 | |
| F05-W0252 | 206 | Sync | Rom | D1 p28 | |
| F05-W0253 | 207 | ChromRes | Rom | D1 p28 | |
| F05-W0254 | 208 | Syn-Pop | Rom | D1 p28 | |
| F05-W0255 | 209 | Detuned-Super | Rom | D1 p28 | |
| F05-W0256 | 210 | Detuned-SuperREV | Rom | D1 p28 | |
| F05-W0257 | 211 | Detuned-Super 2 | Rom | D1 p28 | |
| F05-W0258 | 212 | Detun-Super2 REV | Rom | D1 p28 | |
| F05-W0259 | 213 | Detuned-PWM | Rom | D1 p28 | |
| F05-W0260 | 214 | Detuned-PWM REV | Rom | D1 p28 | |
| F05-W0261 | 215 | Analog Strings 1 | Rom | D1 p28 | |
| F05-W0262 | 216 | AnaStrings 1 REV | Rom | D1 p28 | |
| F05-W0263 | 217 | Analog Strings 2 | Rom | D1 p28 | |
| F05-W0264 | 218 | AnaStrings 2 REV | Rom | D1 p28 | |
| F05-W0265 | 219 | White Pad | Rom | D1 p28 | |
| F05-W0266 | 220 | White Pad REV | Rom | D1 p28 | |
| F05-W0267 | 221 | Glass Vox | Rom | D1 p28 | |
| F05-W0268 | 222 | Glass Vox REV | Rom | D1 p28 | |
| F05-W0269 | 223 | Syn-Air Vox | Rom | D1 p28 | |
| F05-W0270 | 224 | Syn-Air Vox REV | Rom | D1 p28 | |
| F05-W0271 | 225 | Syn-Ghostly | Rom | D1 p28 | |
| F05-W0272 | 226 | Syn-Ghostly REV | Rom | D1 p28 | |
| F05-W0273 | 227 | Syn-Air Pad | Rom | D1 p28 | |
| F05-W0274 | 228 | Syn-Air Pad REV | Rom | D1 p28 | |
| F05-W0275 | 229 | Syn-Flute Pad | Rom | D1 p28 | |
| F05-W0276 | 230 | Syn-FlutePad REV | Rom | D1 p28 | |
| F05-W0277 | 231 | Syn-DreamStrings | Rom | D1 p28 | |
| F05-W0278 | 232 | Syn-DreamStr REV | Rom | D1 p28 | |
| F05-W0279 | 233 | Syn-Golithan | Rom | D1 p28 | |
| F05-W0280 | 234 | Syn-Golithan REV | Rom | D1 p28 | |
| F05-W0281 | 235 | Syn-Air Vortex | Rom | D1 p28 | |
| F05-W0282 | 236 | Syn-AirVortexREV | Rom | D1 p28 | |
| F05-W0283 | 237 | Syn-FX01Z | Rom | D1 p28 | |
| F05-W0284 | 238 | Syn-FX01Z REV | Rom | D1 p28 | |
| F05-W0285 | 239 | Syn-Palawan | Rom | D1 p28 | |
| F05-W0286 | 240 | Syn-Palawan REV | Rom | D1 p28 | |
| F05-W0287 | 241 | WaveSweep | Rom | D1 p28 | |
| F05-W0288 | 242 | WaveSweepA | Rom | D1 p28 | |
| F05-W0289 | 243 | WaveSweepA REV | Rom | D1 p28 | |
| F05-W0290 | 244 | WaveSweepB | Rom | D1 p28 | |
| F05-W0291 | 245 | Syn-Clicker | Rom | D1 p28 | |
| F05-W0292 | 246 | Syn-Clicker REV | Rom | D1 p28 | |
| F05-W0293 | 247 | Xylo-Spectrum | Rom | D1 p28 | |
| F05-W0294 | 248 | Cricket Spectrum | Rom | D1 p28 | |
| F05-W0295 | 249 | Syn-FX07Z | Rom | D1 p28 | |
| F05-W0296 | 250 | Swish Terra | Rom | D1 p28 | |
| F05-W0297 | 251 | Noise | Rom | D1 p28 | |
| F05-W0298 | 252 | Noise Spectrum 1 | Rom | D1 p28 | |
| F05-W0299 | 253 | Noise Spectrum 2 | Rom | D1 p28 | |
| F05-W0300 | 254 | Space Lore | Rom | D1 p28 | |
| F05-W0301 | 255 | Space Lore REV | Rom | D1 p28 | |
| F05-W0302 | 256 | Telephone Ring | Rom | D1 p28 | |
| F05-W0303 | 257 | Tele Ring REV | Rom | D1 p28 | |
| F05-W0304 | 258 | Syn-FX02Z | Rom | D1 p28 | |
| F05-W0305 | 259 | Syn-FX02Z REV | Rom | D1 p28 | |
| F05-W0306 | 260 | Pole | Rom | D1 p28 | |
| F05-W0307 | 261 | Pole LP | Rom | D1 p28 | |
| F05-W0308 | 262 | Gamelan Wave | Rom | D1 p28 | |
| F05-W0309 | 263 | Saw | Rom | D1 p28 | |
| F05-W0310 | 264 | Saw-MG3 | Rom | D1 p28 | |
| F05-W0311 | 265 | Saw-Chroma | Rom | D1 p28 | |
| F05-W0312 | 266 | Pulse-02% | Rom | D1 p28 | |
| F05-W0313 | 267 | Pulse-05% | Rom | D1 p28 | |
| F05-W0314 | 268 | Pulse-08% | Rom | D1 p28 | |
| F05-W0315 | 269 | Pulse-16% | Rom | D1 p28 | |
| F05-W0316 | 270 | Pulse-33% | Rom | D1 p28 | |
| F05-W0317 | 271 | Pulse-40% | Rom | D1 p28 | |
| F05-W0318 | 272 | Square | Rom | D1 p28 | |
| F05-W0319 | 273 | Sqr-mMG | Rom | D1 p28 | |
| F05-W0320 | 274 | Square-JP | Rom | D1 p28 | |
| F05-W0321 | 275 | Triangle-MG3 | Rom | D1 p28 | |
| F05-W0322 | 276 | Ramp | Rom | D1 p28 | |
| F05-W0323 | 277 | Ramp-mMG | Rom | D1 p28 | |
| F05-W0324 | 278 | Sine | Rom | D1 p28 | |
| F05-W0325 | 279 | Sine-JP | Rom | D1 p28 | |
| F05-W0326 | 280 | DWGS-Syn Sine 1 | Rom | D1 p28 | |
| F05-W0327 | 281 | DWGS-Syn Sine 2 | Rom | D1 p28 | |
| F05-W0328 | 282 | DWGS-E.P.1 | Rom | D1 p28 | |
| F05-W0329 | 283 | DWGS-E.P.2 | Rom | D1 p28 | |
| F05-W0330 | 284 | DWGS-E.P.3 | Rom | D1 p28 | |
| F05-W0331 | 285 | DWGS-E.P.4 | Rom | D1 p28 | |
| F05-W0332 | 286 | DWGS-E.P.5 | Rom | D1 p28 | |
| F05-W0333 | 287 | DWGS-Organ 1 | Rom | D1 p28 | |
| F05-W0334 | 288 | DWGS-Organ 2 | Rom | D1 p28 | |
| F05-W0335 | 289 | DWGS-Organ 3 | Rom | D1 p28 | |
| F05-W0336 | 290 | DWGS-Organ 4 | Rom | D1 p28 | |
| F05-W0337 | 291 | DWGS-Organ 5 | Rom | D1 p28 | |
| F05-W0338 | 292 | DWGS-Accordion | Rom | D1 p28 | |
| F05-W0339 | 293 | DWGS Guitar 1 | Rom | D1 p28 | |
| F05-W0340 | 294 | DWGS-Guitar 2 | Rom | D1 p28 | |
| F05-W0341 | 295 | DWGS-Bell 1 | Rom | D1 p28 | |
| F05-W0342 | 296 | DWGS-Bell 2 | Rom | D1 p28 | |
| F05-W0343 | 297 | DWGS-Bell 3 | Rom | D1 p28 | |
| F05-W0344 | 298 | DWGS-Bell 4 | Rom | D1 p28 | |
| F05-W0345 | 299 | DWGS-Clav 1 | Rom | D1 p28 | |
| F05-W0346 | 300 | DWGS-Clav 2 | Rom | D1 p28 | |
| F05-W0347 | 301 | DWGS-Clav 3 | Rom | D1 p28 | |
| F05-W0348 | 302 | DWGS-Digi 1 | Rom | D1 p28 | |
| F05-W0349 | 303 | DWGS-Digi 2 | Rom | D1 p28 | |
| F05-W0350 | 304 | DWGS-Digi 3 | Rom | D1 p28 | |
| F05-W0351 | 305 | DWGS-Wire 1 | Rom | D1 p28 | |
| F05-W0352 | 306 | DWGS-Wire 2 | Rom | D1 p28 | |
| F05-W0353 | 307 | DWGS-Sync 1 | Rom | D1 p28 | |
| F05-W0354 | 308 | DWGS-Sync 2 | Rom | D1 p28 | |
| F05-W0355 | 309 | DWGS-Sync 3 | Rom | D1 p28 | |
| F05-W0356 | 310 | Orchestra Hit | Rom | D1 p28 | |
| F05-W0357 | 311 | Pizz Hit | Rom | D1 p28 | |
| F05-W0358 | 312 | Band Hit | Rom | D1 p28 | |
| F05-W0359 | 313 | Impact Hit L | Rom | D1 p28 | |
| F05-W0360 | 314 | Impact Hit R | Rom | D1 p28 | |
| F05-W0361 | 315 | Philly Hit | Rom | D1 p28 | |
| F05-W0362 | 316 | Brass Fall | Rom | D1 p28 | |
| F05-W0363 | 317 | Organ Hit | Rom | D1 p28 | |
| F05-W0364 | 318 | FunkyGuitarHit | Rom | D1 p28 | |
| F05-W0365 | 319 | Pop Guitar Hit | Rom | D1 p28 | |
| F05-W0366 | 320 | ReggaeGuitar Hit | Rom | D1 p28 | |
| F05-W0367 | 321 | Vox-Wah Gtr | Rom | D1 p28 | |
| F05-W0368 | 322 | GtCutNois 1 | Rom | D1 p28 | |
| F05-W0369 | 323 | GtCutNois 2 | Rom | D1 p28 | |
| F05-W0370 | 324 | String Slap | Rom | D1 p28 | |
| F05-W0371 | 325 | Voice Set | Rom | D1 p28 | |
| F05-W0372 | 326 | Voice *Do it* | Rom | D1 p28 | |
| F05-W0373 | 327 | Voice *Do it* LP | Rom | D1 p28 | |
| F05-W0374 | 328 | VibeChord 1 | Rom | D1 p28 | |
| F05-W0375 | 329 | VibeChord 2 | Rom | D1 p28 | |
| F05-W0376 | 330 | VibeChord 1&2 | Rom | D1 p28 | |
| F05-W0377 | 331 | Cyber Drum | Rom | D1 p28 | |
| F05-W0378 | 332 | Cyber Drum RPT | Rom | D1 p28 | |
| F05-W0379 | 333 | Industry | Rom | D1 p28 | |
| F05-W0380 | 334 | Industry RPT | Rom | D1 p28 | |
| F05-W0381 | 335 | SD-Metal Striker | Rom | D1 p28 | |
| F05-W0382 | 336 | SD-M.Striker RPT | Rom | D1 p28 | |
| F05-W0383 | 337 | SD-Air Explosion | Rom | D1 p28 | |
| F05-W0384 | 338 | Metal Hit 4 | Rom | D1 p28 | |
| F05-W0385 | 339 | Metal Hit 4 RPT | Rom | D1 p28 | |
| F05-W0386 | 340 | Industry-66 | Rom | D1 p28 | |
| F05-W0387 | 341 | Industry-66 RPT | Rom | D1 p28 | |
| F05-W0388 | 342 | Industry-21 | Rom | D1 p28 | |
| F05-W0389 | 343 | BD-Cannon | Rom | D1 p28 | |
| F05-W0390 | 344 | Zap 1 | Rom | D1 p28 | |
| F05-W0391 | 345 | Zap 2 | Rom | D1 p28 | |
| F05-W0392 | 346 | Scratch Hi | Rom | D1 p28 | |
| F05-W0393 | 347 | DJ-Old Record | Rom | D1 p28 | |
| F05-W0394 | 348 | Stadium | Rom | D1 p28 | |
| F05-W0395 | 349 | Applause | Rom | D1 p28 | |
| F05-W0396 | 350 | Birds 1 | Rom | D1 p28 | |
| F05-W0397 | 351 | Birds 1 REV | Rom | D1 p28 | |
| F05-W0398 | 352 | Bird 2 | Rom | D1 p28 | |
| F05-W0399 | 353 | Crickets 1 | Rom | D1 p28 | |
| F05-W0400 | 354 | Crickets 1 REV | Rom | D1 p28 | |
| F05-W0401 | 355 | Crickets 2 | Rom | D1 p28 | |
| F05-W0402 | 356 | ChurchBell | Rom | D1 p28 | |
| F05-W0403 | 357 | ChurchBell REV | Rom | D1 p28 | |
| F05-W0404 | 358 | Thunder | Rom | D1 p28 | |
| F05-W0405 | 359 | Stream | Rom | D1 p28 | |
| F05-W0406 | 360 | Bubble | Rom | D1 p28 | |
| F05-W0407 | 361 | Bubble REV | Rom | D1 p28 | |
| F05-W0408 | 362 | Dog | Rom | D1 p28 | |
| F05-W0409 | 363 | Gallop | Rom | D1 p28 | |
| F05-W0410 | 364 | Gallop REV | Rom | D1 p28 | |
| F05-W0411 | 365 | Laughing | Rom | D1 p28 | |
| F05-W0412 | 366 | Scream | Rom | D1 p28 | |
| F05-W0413 | 367 | Punch | Rom | D1 p28 | |
| F05-W0414 | 368 | Heart Beat | Rom | D1 p28 | |
| F05-W0415 | 369 | Footstep 1 | Rom | D1 p28 | |
| F05-W0416 | 370 | Footstep 2 | Rom | D1 p28 | |
| F05-W0417 | 371 | Door Creak | Rom | D1 p28 | |
| F05-W0418 | 372 | Door Slam | Rom | D1 p28 | |
| F05-W0419 | 373 | Car Engine | Rom | D1 p28 | |
| F05-W0420 | 374 | Car Engine LP | Rom | D1 p28 | |
| F05-W0421 | 375 | Car Stop | Rom | D1 p28 | |
| F05-W0422 | 376 | Car Pass | Rom | D1 p28 | |
| F05-W0423 | 377 | Car Crash | Rom | D1 p28 | |
| F05-W0424 | 378 | Train | Rom | D1 p28 | |
| F05-W0425 | 379 | Train REV | Rom | D1 p28 | |
| F05-W0426 | 380 | Helicopter | Rom | D1 p28 | |
| F05-W0427 | 381 | Helicopter REV | Rom | D1 p28 | |
| F05-W0428 | 382 | Gun Shot 2 | Rom | D1 p28 | |
| F05-W0429 | 383 | Machine Gun | Rom | D1 p28 | |
| F05-W0430 | 384 | Machine Gun REV | Rom | D1 p28 | |
| F05-W0431 | 385 | Laser Gun | Rom | D1 p28 | |
| F05-W0432 | 386 | Explosion | Rom | D1 p28 | |
| F05-W0433 | 387 | Wind | Rom | D1 p28 | |
| F05-W0434 | 388 | Timpani | Rom | D1 p28 | |
| F05-W0435 | 389 | Crash 1 | Rom | D1 p28 | |
| F05-W0436 | 390 | Crash 2 | Rom | D1 p28 | |
| F05-W0437 | 391 | Cymbal Reverse | Rom | D1 p28 | |
| F05-W0438 | 392 | Orch. Cymbal | Rom | D1 p28 | |
| F05-W0439 | 393 | Ride-Jazz | Rom | D1 p28 | |
| F05-W0440 | 394 | Ride-Edge 1 | Rom | D1 p28 | |
| F05-W0441 | 395 | Ride-Edge 2 | Rom | D1 p28 | |
| F05-W0442 | 396 | H.Hat-Closed | Rom | D1 p28 | |
| F05-W0443 | 397 | 88-HHat Open | Rom | D1 p28 | |
| F05-W0444 | 398 | 88-Cowbell | Rom | D1 p28 | |
| F05-W0445 | 399 | 88-Tom | Rom | D1 p28 | |
| F05-W0446 | 400 | 88-Conga | Rom | D1 p28 | |
| F05-W0447 | 401 | 88-Crash | Rom | D1 p28 | |
| F05-W0448 | 402 | Tom | Rom | D1 p28 | |
| F05-W0449 | 403 | Tom-Brush | Rom | D1 p28 | |
| F05-W0450 | 404 | Tom-Processed | Rom | D1 p28 | |
| F05-W0451 | 405 | Real E.Tom | Rom | D1 p28 | |
| F05-W0452 | 406 | Flexatone | Rom | D1 p28 | |
| F05-W0453 | 407 | Tambourine | Rom | D1 p28 | |
| F05-W0454 | 408 | Tambourine-RPT | Rom | D1 p28 | |
| F05-W0455 | 409 | Agogo Bell | Rom | D1 p28 | |
| F05-W0456 | 410 | Cowbell | Rom | D1 p28 | |
| F05-W0457 | 411 | Marc Tree | Rom | D1 p28 | |
| F05-W0458 | 412 | Marc Tree LP | Rom | D1 p28 | |
| F05-W0459 | 413 | Bell Tree | Rom | D1 p28 | |
| F05-W0460 | 414 | Bell Tree LP | Rom | D1 p28 | |
| F05-W0461 | 415 | Rainstick | Rom | D1 p28 | |
| F05-W0462 | 416 | TempleBlocks | Rom | D1 p28 | |
| F05-W0463 | 417 | WoodBlock | Rom | D1 p28 | |
| F05-W0464 | 418 | Click | Rom | D1 p28 | |
| F05-W0465 | 419 | Castanet | Rom | D1 p28 | |
| F05-W0466 | 420 | Taiko | Rom | D1 p28 | |
| F05-W0467 | 421 | Orch BD | Rom | D1 p28 | |
| F05-W0468 | 422 | Djembe-Open | Rom | D1 p28 | |
| F05-W0469 | 423 | Djembe-Mute | Rom | D1 p28 | |
| F05-W0470 | 424 | Chinese Gong | Rom | D1 p28 | |

### Drumsamples

| Shipped label | Number | Documented name | Group | Source | Note |
|---|---|---|---|---|---|
| F05-W0001 | 0 | BD-Dry 1 | Rom | D1 p29 | |
| F05-W0002 | 0 | BD-amb 1 | EX | D1 p30 | |
| F05-W0003 | 1 | BD-Dry 2 | Rom | D1 p29 | |
| F05-W0004 | 1 | BD-amb 2 | EX | D1 p30 | |
| F05-W0005 | 2 | BD-Dry 3 | Rom | D1 p29 | |
| F05-W0006 | 2 | BD-Dance 5 | EX | D1 p30 | |
| F05-W0007 | 3 | BD-Dry 4 | Rom | D1 p29 | |
| F05-W0008 | 3 | BD-Dance 6 | EX | D1 p30 | |
| F05-W0009 | 4 | BD-Dry 5 | Rom | D1 p29 | |
| F05-W0010 | 4 | BD-Dance 7 | EX | D1 p30 | |
| F05-W0011 | 5 | BD-Dry 6 | Rom | D1 p29 | |
| F05-W0012 | 5 | BD-Dance 8 | EX | D1 p30 | |
| F05-W0013 | 6 | BD-Soft | Rom | D1 p29 | |
| F05-W0014 | 6 | BD-Dance 9 | EX | D1 p30 | |
| F05-W0015 | 7 | BD-Soft Room | Rom | D1 p29 | |
| F05-W0016 | 7 | BD-Dance 10 | EX | D1 p30 | |
| F05-W0017 | 8 | BD-Jazz 1 | Rom | D1 p29 | |
| F05-W0018 | 8 | BD-Dance 11 | EX | D1 p30 | |
| F05-W0019 | 9 | BD-Jazz 2 | Rom | D1 p29 | |
| F05-W0020 | 9 | BD-Dance 12 | EX | D1 p30 | |
| F05-W0021 | 10 | BD-Pillow | Rom | D1 p29 | |
| F05-W0022 | 10 | BD-Dance 13 | EX | D1 p30 | |
| F05-W0023 | 11 | BD-Woofer | Rom | D1 p29 | |
| F05-W0024 | 11 | BD-Dance 14 | EX | D1 p30 | |
| F05-W0025 | 12 | BD-Mondo Kill | Rom | D1 p29 | |
| F05-W0026 | 12 | BD-Dance 15 | EX | D1 p30 | |
| F05-W0027 | 13 | BD-Terminator | Rom | D1 p29 | |
| F05-W0028 | 13 | SD-Dry 4 | EX | D1 p30 | |
| F05-W0029 | 14 | BD-Tubby | Rom | D1 p29 | |
| F05-W0030 | 14 | SD-Dry 5 f | EX | D1 p30 | |
| F05-W0031 | 15 | BD-Gated | Rom | D1 p29 | |
| F05-W0032 | 15 | SD-Dry 5 ff | EX | D1 p30 | |
| F05-W0033 | 16 | BD-Tight | Rom | D1 p29 | |
| F05-W0034 | 16 | SD-Dry 6 | EX | D1 p30 | |
| F05-W0035 | 17 | BD-Squash | Rom | D1 p29 | |
| F05-W0036 | 17 | SD-Dry 7 | EX | D1 p30 | |
| F05-W0037 | 18 | BD-Dance 1 | Rom | D1 p29 | |
| F05-W0038 | 18 | SD-Dry 8 p | EX | D1 p30 | |
| F05-W0039 | 19 | BD-Dance 2 | Rom | D1 p29 | |
| F05-W0040 | 19 | SD-Dry 8 mp | EX | D1 p30 | |
| F05-W0041 | 20 | BD-Dance 3 | Rom | D1 p29 | |
| F05-W0042 | 20 | SD-Dry 8 f | EX | D1 p30 | |
| F05-W0043 | 21 | BD-Dance 4 | Rom | D1 p29 | |
| F05-W0044 | 21 | SD-Dry 8 ff | EX | D1 p30 | |
| F05-W0045 | 22 | BD-Hip 1 | Rom | D1 p29 | |
| F05-W0046 | 22 | SD-Dry 9 | EX | D1 p30 | |
| F05-W0047 | 23 | BD-Hip 2 | Rom | D1 p29 | |
| F05-W0048 | 23 | SD-Roll-Open | EX | D1 p30 | |
| F05-W0049 | 24 | BD-Hip 3 | Rom | D1 p29 | |
| F05-W0050 | 24 | SD-Roll-Closed | EX | D1 p30 | |
| F05-W0051 | 25 | BD-Hip 4 | Rom | D1 p29 | |
| F05-W0052 | 25 | Sidestick-Amb2mf | EX | D1 p30 | |
| F05-W0053 | 26 | BD-Dark | Rom | D1 p29 | |
| F05-W0054 | 26 | Sidestick-Amb2 f | EX | D1 p30 | |
| F05-W0055 | 27 | BD-Pop | Rom | D1 p29 | |
| F05-W0056 | 27 | SD-Dance 2 | EX | D1 p30 | |
| F05-W0057 | 28 | BD-Ringy | Rom | D1 p29 | |
| F05-W0058 | 28 | SD-Dance 3 | EX | D1 p30 | |
| F05-W0059 | 29 | BD-Short88 | Rom | D1 p29 | |
| F05-W0060 | 29 | SD-Dance 4 | EX | D1 p30 | |
| F05-W0061 | 30 | BD-Dance99 | Rom | D1 p29 | |
| F05-W0062 | 30 | SD-Dance 5 | EX | D1 p30 | |
| F05-W0063 | 31 | BD-Ambi | Rom | D1 p29 | |
| F05-W0064 | 31 | SD-Dance 6 | EX | D1 p30 | |
| F05-W0065 | 32 | BD-Ambi Crackle | Rom | D1 p29 | |
| F05-W0066 | 32 | SD-Dance 7 | EX | D1 p30 | |
| F05-W0067 | 33 | BD-Ambi Soft | Rom | D1 p29 | |
| F05-W0068 | 33 | SD-Dance 8 | EX | D1 p30 | |
| F05-W0069 | 34 | BD-Dance Soft | Rom | D1 p29 | |
| F05-W0070 | 34 | SD-Dance 9 | EX | D1 p30 | |
| F05-W0071 | 35 | BD-AmbiRocker | Rom | D1 p29 | |
| F05-W0072 | 35 | SD-Dance 10 | EX | D1 p30 | |
| F05-W0073 | 36 | BD-Chili | Rom | D1 p29 | |
| F05-W0074 | 36 | SD-Dance 11 | EX | D1 p30 | |
| F05-W0075 | 37 | BD-PhatJaz | Rom | D1 p29 | |
| F05-W0076 | 37 | SD-Dance 12 | EX | D1 p30 | |
| F05-W0077 | 38 | BD-Cannon | Rom | D1 p29 | |
| F05-W0078 | 38 | SD-Dance 13 | EX | D1 p30 | |
| F05-W0079 | 39 | BD-Klanger | Rom | D1 p29 | |
| F05-W0080 | 39 | SD-Dance 14 | EX | D1 p30 | |
| F05-W0081 | 40 | SD-Dry 1 | Rom | D1 p29 | |
| F05-W0082 | 40 | SD-Dance 15 | EX | D1 p30 | |
| F05-W0083 | 41 | SD-Dry 2 | Rom | D1 p29 | |
| F05-W0084 | 41 | SD-Dance 16 | EX | D1 p30 | |
| F05-W0085 | 42 | SD-Dry 3 | Rom | D1 p29 | |
| F05-W0086 | 42 | SD-Dance 17 | EX | D1 p30 | |
| F05-W0087 | 43 | SD-Normal | Rom | D1 p29 | |
| F05-W0088 | 43 | SD-Dance 18 | EX | D1 p30 | |
| F05-W0089 | 44 | SD-GhostSet roll | Rom | D1 p29 | |
| F05-W0090 | 44 | SD-Dance 19 | EX | D1 p30 | |
| F05-W0091 | 45 | SD-GhostSet f | Rom | D1 p29 | |
| F05-W0092 | 45 | Tom 4-Lo | EX | D1 p30 | |
| F05-W0093 | 46 | SD-GhostSet p1 | Rom | D1 p29 | |
| F05-W0094 | 46 | Tom 4-Mid | EX | D1 p30 | |
| F05-W0095 | 47 | SD-GhostSet p2 | Rom | D1 p29 | |
| F05-W0096 | 47 | Tom 4-Hi | EX | D1 p30 | |
| F05-W0097 | 48 | SD-Full Room | Rom | D1 p29 | |
| F05-W0098 | 48 | Tom 5-Lo mf | EX | D1 p30 | |
| F05-W0099 | 49 | SD-Off Center | Rom | D1 p29 | |
| F05-W0100 | 49 | Tom 5-Lo ff | EX | D1 p30 | |
| F05-W0101 | 50 | SD-Jazz Ring | Rom | D1 p29 | |
| F05-W0102 | 50 | Tom 5-Hi mf | EX | D1 p30 | |
| F05-W0103 | 51 | SD-Amb.Piccolo 1 | Rom | D1 p29 | |
| F05-W0104 | 51 | Tom 5-Hi ff | EX | D1 p30 | |
| F05-W0105 | 52 | SD-Amb.Piccolo 2 | Rom | D1 p29 | |
| F05-W0106 | 52 | H.Hat 3-Close mf | EX | D1 p30 | |
| F05-W0107 | 53 | SD-Paper | Rom | D1 p29 | |
| F05-W0108 | 53 | H.Hat 3-Close f | EX | D1 p30 | |
| F05-W0109 | 54 | SD-Classic Room | Rom | D1 p29 | |
| F05-W0110 | 54 | H.Hat 3-Foot | EX | D1 p30 | |
| F05-W0111 | 55 | SD-Brush Hit | Rom | D1 p29 | |
| F05-W0112 | 55 | H.Hat 3-Open | EX | D1 p30 | |
| F05-W0113 | 56 | SD-Brush Tap 1 | Rom | D1 p29 | |
| F05-W0114 | 56 | H.Hat 4-Close mf | EX | D1 p30 | |
| F05-W0115 | 57 | SD-Brush Tap 2 | Rom | D1 p29 | |
| F05-W0116 | 57 | H.Hat 4-Close f | EX | D1 p30 | |
| F05-W0117 | 58 | SD-Brush Swirl | Rom | D1 p29 | |
| F05-W0118 | 58 | H.Hat-BrushCl mp | EX | D1 p30 | |
| F05-W0119 | 59 | SD-Atomic | Rom | D1 p29 | |
| F05-W0120 | 59 | H.Hat-BrushCl mf | EX | D1 p30 | |
| F05-W0121 | 60 | SD-Big Rock | Rom | D1 p29 | |
| F05-W0122 | 60 | H.Hat-BrushOp mf | EX | D1 p30 | |
| F05-W0123 | 61 | SD-Yowie | Rom | D1 p29 | |
| F05-W0124 | 61 | H.Hat-BrushOp f | EX | D1 p30 | |
| F05-W0125 | 62 | SD-Processed 1 | Rom | D1 p29 | |
| F05-W0126 | 62 | H.Hat-Dance 1 | EX | D1 p30 | |
| F05-W0127 | 63 | SD-Processed 2 | Rom | D1 p29 | |
| F05-W0128 | 63 | H.Hat-Dance 2 | EX | D1 p30 | |
| F05-W0129 | 64 | SD-Cracker Room | Rom | D1 p29 | |
| F05-W0130 | 64 | H.Hat-Dance 3 | EX | D1 p30 | |
| F05-W0131 | 65 | Sidestick-Dry | Rom | D1 p29 | |
| F05-W0132 | 65 | H.Hat-Dance 4 | EX | D1 p30 | |
| F05-W0133 | 66 | Sidestick-Amb | Rom | D1 p29 | |
| F05-W0134 | 66 | H.Hat-Dance 5 | EX | D1 p30 | |
| F05-W0135 | 67 | SD-Amb.House | Rom | D1 p29 | |
| F05-W0136 | 67 | H.Hat-Dance 6 | EX | D1 p30 | |
| F05-W0137 | 68 | SD-Dance | Rom | D1 p29 | |
| F05-W0138 | 68 | H.Hat-Dance 7 | EX | D1 p30 | |
| F05-W0139 | 69 | SD-Rap 1 | Rom | D1 p29 | |
| F05-W0140 | 69 | H.Hat-Dance 8 | EX | D1 p30 | |
| F05-W0141 | 70 | SD-Rap 2 | Rom | D1 p29 | |
| F05-W0142 | 70 | H.Hat-Dance 9 | EX | D1 p30 | |
| F05-W0143 | 71 | SD-Noise | Rom | D1 p29 | |
| F05-W0144 | 71 | H.Hat-Dance 10 | EX | D1 p30 | |
| F05-W0145 | 72 | SD-Sizzle | Rom | D1 p29 | |
| F05-W0146 | 72 | H.Hat-Dance 11 | EX | D1 p30 | |
| F05-W0147 | 73 | SD-Reverse | Rom | D1 p29 | |
| F05-W0148 | 73 | H.Hat-Dance 12 | EX | D1 p30 | |
| F05-W0149 | 74 | SD-Hip 1 | Rom | D1 p29 | |
| F05-W0150 | 74 | H.Hat-Dance 13 | EX | D1 p30 | |
| F05-W0151 | 75 | SD-Hip 2 | Rom | D1 p29 | |
| F05-W0152 | 75 | H.Hat-Dance 14 | EX | D1 p30 | |
| F05-W0153 | 76 | SD-Hip 3 | Rom | D1 p29 | |
| F05-W0154 | 76 | H.Hat-Dance 15 | EX | D1 p30 | |
| F05-W0155 | 77 | SD-Hip 4 | Rom | D1 p29 | |
| F05-W0156 | 77 | H.Hat-Dance 16 | EX | D1 p30 | |
| F05-W0157 | 78 | SD-Hip 5 | Rom | D1 p29 | |
| F05-W0158 | 78 | Ride-Edge 3 | EX | D1 p30 | |
| F05-W0159 | 79 | SD-Hip 6 | Rom | D1 p29 | |
| F05-W0160 | 79 | Ride-Cup 2 | EX | D1 p30 | |
| F05-W0161 | 80 | SD-Ringy | Rom | D1 p29 | |
| F05-W0162 | 80 | Splash Cymbal 2 | EX | D1 p30 | |
| F05-W0163 | 81 | SD-Tiny | Rom | D1 p29 | |
| F05-W0164 | 81 | Crash-Dance | EX | D1 p30 | |
| F05-W0165 | 82 | SD-Vintage 1 | Rom | D1 p29 | |
| F05-W0166 | 82 | Djembe-CloseSlap | EX | D1 p30 | |
| F05-W0167 | 83 | SD-Vintage 2 | Rom | D1 p29 | |
| F05-W0168 | 83 | WD-Native Drum | EX | D1 p30 | |
| F05-W0169 | 84 | SD-Vintage 3 | Rom | D1 p29 | |
| F05-W0170 | 84 | WD-Ethno SD | EX | D1 p30 | |
| F05-W0171 | 85 | SD-Vintage 4 | Rom | D1 p29 | |
| F05-W0172 | 85 | Dance Claps | EX | D1 p30 | |
| F05-W0173 | 86 | SD-Vintage 5 | Rom | D1 p29 | |
| F05-W0174 | 86 | Dance Perc-1 a | EX | D1 p30 | |
| F05-W0175 | 87 | SD-Vintage 6 | Rom | D1 p29 | |
| F05-W0176 | 87 | Dance Perc-1 b | EX | D1 p30 | |
| F05-W0177 | 88 | SD-AmbiCrackle 1 | Rom | D1 p29 | |
| F05-W0178 | 88 | Dance Perc-2 a | EX | D1 p30 | |
| F05-W0179 | 89 | SD-AmbiCrackle 2 | Rom | D1 p29 | |
| F05-W0180 | 89 | Dance Perc-2 b | EX | D1 p30 | |
| F05-W0181 | 90 | SD-AmbiCrackle 3 | Rom | D1 p29 | |
| F05-W0182 | 90 | Dance Perc-3 | EX | D1 p30 | |
| F05-W0183 | 91 | SD-AmbiHop | Rom | D1 p29 | |
| F05-W0184 | 91 | DJ VinylSliced 1 | EX | D1 p30 | |
| F05-W0185 | 92 | SD-Brasser | Rom | D1 p29 | |
| F05-W0186 | 92 | DJ VinylSliced 2 | EX | D1 p30 | |
| F05-W0187 | 93 | SD-Chili | Rom | D1 p29 | |
| F05-W0188 | 93 | DJ VinylSliced 3 | EX | D1 p30 | |
| F05-W0189 | 94 | SD-Klanger | Rom | D1 p29 | |
| F05-W0190 | 94 | DJ VinylSliced 4 | EX | D1 p30 | |
| F05-W0191 | 95 | SD-Whopper | Rom | D1 p29 | |
| F05-W0192 | 95 | "Trrrra" | EX | D1 p30 | |
| F05-W0193 | 96 | SD-Dry Present | Rom | D1 p29 | |
| F05-W0194 | 96 | "Aow" | EX | D1 p30 | |
| F05-W0195 | 97 | SD-Ambi Pop | Rom | D1 p29 | |
| F05-W0196 | 97 | "TurnAround" | EX | D1 p30 | |
| F05-W0197 | 98 | SD-Ambi Hit 1 | Rom | D1 p29 | |
| F05-W0198 | 98 | Vocoder Voice | EX | D1 p30 | |
| F05-W0199 | 99 | SD-Ambi Hit 2 | Rom | D1 p29 | |
| F05-W0200 | 99 | Sword | EX | D1 p30 | |
| F05-W0201 | 100 | SD-Ambi Hit 3 | Rom | D1 p29 | |
| F05-W0202 | 100 | Cannon | EX | D1 p30 | |
| F05-W0203 | 101 | Tom 1-Hi | Rom | D1 p29 | |
| F05-W0204 | 102 | Tom 1-Lo | Rom | D1 p29 | |
| F05-W0205 | 103 | Tom 1-Floor | Rom | D1 p29 | |
| F05-W0206 | 104 | Tom 2-Hi | Rom | D1 p29 | |
| F05-W0207 | 105 | Tom 2-Lo | Rom | D1 p29 | |
| F05-W0208 | 106 | Tom 2-Floor | Rom | D1 p29 | |
| F05-W0209 | 107 | Tom 3-Hi | Rom | D1 p29 | |
| F05-W0210 | 108 | Tom 3-Lo | Rom | D1 p29 | |
| F05-W0211 | 109 | Tom 3-Floor | Rom | D1 p29 | |
| F05-W0212 | 110 | Tom-Processed | Rom | D1 p29 | |
| F05-W0213 | 111 | Tom-Jazz Hi | Rom | D1 p29 | |
| F05-W0214 | 112 | Tom-Jazz Hi Rim | Rom | D1 p29 | |
| F05-W0215 | 113 | Tom-Jazz Floor | Rom | D1 p29 | |
| F05-W0216 | 114 | Tom-Brush Hi | Rom | D1 p29 | |
| F05-W0217 | 115 | Tom-Brush Floor | Rom | D1 p29 | |
| F05-W0218 | 116 | Tom-Dirty Funk | Rom | D1 p29 | |
| F05-W0219 | 117 | Tom-Old R&B | Rom | D1 p29 | |
| F05-W0220 | 118 | H.Hat 1-Closed 1 | Rom | D1 p29 | |
| F05-W0221 | 119 | H.Hat 1-Closed 2 | Rom | D1 p29 | |
| F05-W0222 | 120 | H.Hat 1-Foot | Rom | D1 p29 | |
| F05-W0223 | 121 | H.Hat 1-FootOpen | Rom | D1 p29 | |
| F05-W0224 | 122 | H.Hat 1-Open 1 | Rom | D1 p29 | |
| F05-W0225 | 123 | H.Hat 1-Open 2 | Rom | D1 p29 | |
| F05-W0226 | 124 | H.Hat 1-Sizzle | Rom | D1 p29 | |
| F05-W0227 | 125 | H.Hat 2-Closed 1 | Rom | D1 p29 | |
| F05-W0228 | 126 | H.Hat 2-Closed 2 | Rom | D1 p29 | |
| F05-W0229 | 127 | H.Hat 2-Foot | Rom | D1 p29 | |
| F05-W0230 | 128 | H.Hat 2-FootOpen | Rom | D1 p29 | |
| F05-W0231 | 129 | H.Hat 2-Open 1 | Rom | D1 p29 | |
| F05-W0232 | 130 | H.Hat 2-Open 2 | Rom | D1 p29 | |
| F05-W0233 | 131 | H.Hat 2-Sizzle | Rom | D1 p29 | |
| F05-W0234 | 132 | H.Hat-Chili | Rom | D1 p29 | |
| F05-W0235 | 133 | H.Hat-Crisp Cl 1 | Rom | D1 p29 | |
| F05-W0236 | 134 | H.Hat-Crisp Cl 2 | Rom | D1 p29 | |
| F05-W0237 | 135 | H.Hat-Crisp Open | Rom | D1 p29 | |
| F05-W0238 | 136 | H.Hat-Grange | Rom | D1 p29 | |
| F05-W0239 | 137 | H.Hat-Vintage 1 | Rom | D1 p29 | |
| F05-W0240 | 138 | H.Hat-Vintage 2 | Rom | D1 p29 | |
| F05-W0241 | 139 | H.Hat-Tight 1 | Rom | D1 p29 | |
| F05-W0242 | 140 | H.Hat-Tight 2 | Rom | D1 p29 | |
| F05-W0243 | 141 | H.Hat-Old Cl 1 | Rom | D1 p29 | |
| F05-W0244 | 142 | H.Hat-Old Cl 2 | Rom | D1 p29 | |
| F05-W0245 | 143 | H.Hat-Old Cl 3 | Rom | D1 p29 | |
| F05-W0246 | 144 | H.Hat-Old Open | Rom | D1 p29 | |
| F05-W0247 | 145 | H.Hat-Hip 1 | Rom | D1 p29 | |
| F05-W0248 | 146 | H.Hat-Hip 2 | Rom | D1 p29 | |
| F05-W0249 | 147 | H.Hat-AmbCrackle | Rom | D1 p29 | |
| F05-W0250 | 148 | H.Hat-Ambi | Rom | D1 p29 | |
| F05-W0251 | 149 | H.Hat-ClangyOpen | Rom | D1 p29 | |
| F05-W0252 | 150 | H.Hat-Whispy | Rom | D1 p29 | |
| F05-W0253 | 151 | H.Hat-Alpo Close | Rom | D1 p29 | |
| F05-W0254 | 152 | Crash Cymbal 1 | Rom | D1 p29 | |
| F05-W0255 | 153 | Crash Cymbal 2 | Rom | D1 p29 | |
| F05-W0256 | 154 | Crash Cymbal 3 | Rom | D1 p29 | |
| F05-W0257 | 155 | China Cymbal | Rom | D1 p29 | |
| F05-W0258 | 156 | Splash Cymbal | Rom | D1 p29 | |
| F05-W0259 | 157 | Cymbal Reverse | Rom | D1 p29 | |
| F05-W0260 | 158 | Crash-Dance 99 | Rom | D1 p29 | |
| F05-W0261 | 159 | Crash-Dance DDD1 | Rom | D1 p29 | |
| F05-W0262 | 160 | Ride-Dance 99 | Rom | D1 p29 | |
| F05-W0263 | 161 | Ride-Dance DDD1 | Rom | D1 p29 | |
| F05-W0264 | 162 | Ride-Edge 1 | Rom | D1 p29 | |
| F05-W0265 | 163 | Ride-Edge 2 | Rom | D1 p29 | |
| F05-W0266 | 164 | Ride-Jazz | Rom | D1 p29 | |
| F05-W0267 | 165 | Ride-Cup | Rom | D1 p29 | |
| F05-W0268 | 166 | BD-Orchestral | Rom | D1 p29 | |
| F05-W0269 | 167 | Orch Cym.-Closed | Rom | D1 p29 | |
| F05-W0270 | 168 | Orch Cym.-Open | Rom | D1 p29 | |
| F05-W0271 | 169 | Timpani | Rom | D1 p29 | |
| F05-W0272 | 170 | SD-Orch.Roll 1 | Rom | D1 p29 | |
| F05-W0273 | 171 | SD-Orch.Roll 2 | Rom | D1 p29 | |
| F05-W0274 | 172 | SD-Orchestral | Rom | D1 p29 | |
| F05-W0275 | 173 | 88-BD 1(Rap) | Rom | D1 p29 | |
| F05-W0276 | 174 | 88-BD 2(R&B) | Rom | D1 p29 | |
| F05-W0277 | 175 | 88-BD 3 | Rom | D1 p29 | |
| F05-W0278 | 176 | 99-BD 1 | Rom | D1 p29 | |
| F05-W0279 | 177 | 99-BD 2 | Rom | D1 p29 | |
| F05-W0280 | 178 | 99-BD 3 | Rom | D1 p29 | |
| F05-W0281 | 179 | 99-BD 4 | Rom | D1 p29 | |
| F05-W0282 | 180 | 99-BD 5 | Rom | D1 p29 | |
| F05-W0283 | 181 | 88-SD 1(New) | Rom | D1 p29 | |
| F05-W0284 | 182 | 88-SD 2(Old) | Rom | D1 p29 | |
| F05-W0285 | 183 | 99-SD 1 | Rom | D1 p29 | |
| F05-W0286 | 184 | 99-SD 2 | Rom | D1 p29 | |
| F05-W0287 | 185 | 88-Rimshot | Rom | D1 p29 | |
| F05-W0288 | 186 | 88-Claps | Rom | D1 p29 | |
| F05-W0289 | 187 | 88-HHat Closed | Rom | D1 p29 | |
| F05-W0290 | 188 | 88-HHat Open | Rom | D1 p29 | |
| F05-W0291 | 189 | 99-HHat Closed | Rom | D1 p29 | |
| F05-W0292 | 190 | 99-HHat Open | Rom | D1 p29 | |
| F05-W0293 | 191 | 88-Crash | Rom | D1 p29 | |
| F05-W0294 | 192 | 88-Tom | Rom | D1 p29 | |
| F05-W0295 | 193 | 88-Conga | Rom | D1 p29 | |
| F05-W0296 | 194 | 88-Clave | Rom | D1 p29 | |
| F05-W0297 | 195 | 88-Cowbell | Rom | D1 p29 | |
| F05-W0298 | 196 | 88-Maraca | Rom | D1 p29 | |
| F05-W0299 | 197 | FM E.Tom | Rom | D1 p29 | |
| F05-W0300 | 198 | Real E.Tom | Rom | D1 p29 | |
| F05-W0301 | 199 | Vocal SD | Rom | D1 p29 | |
| F05-W0302 | 200 | Vocal Cowbell | Rom | D1 p29 | |
| F05-W0303 | 201 | Vocal Woodblock | Rom | D1 p29 | |
| F05-W0304 | 202 | Dance Perc-Hip | Rom | D1 p29 | |
| F05-W0305 | 203 | Dance Perc-Hous1 | Rom | D1 p29 | |
| F05-W0306 | 204 | Dance Perc-Hous2 | Rom | D1 p29 | |
| F05-W0307 | 205 | Zap 1 | Rom | D1 p29 | |
| F05-W0308 | 206 | Zap 2 | Rom | D1 p29 | |
| F05-W0309 | 207 | Zap 3 | Rom | D1 p29 | |
| F05-W0310 | 208 | DJ-Scratch 1 | Rom | D1 p29 | |
| F05-W0311 | 209 | DJ-Scratch 2 | Rom | D1 p29 | |
| F05-W0312 | 210 | DJ-Scratch 3a | Rom | D1 p29 | |
| F05-W0313 | 211 | DJ-Scratch 3b | Rom | D1 p29 | |
| F05-W0314 | 212 | DJ-Scratch 3c | Rom | D1 p29 | |
| F05-W0315 | 213 | DJ-Scratch 4 | Rom | D1 p29 | |
| F05-W0316 | 214 | DJ-Hit Rub | Rom | D1 p29 | |
| F05-W0317 | 215 | DJ-1c Vocal Rub | Rom | D1 p29 | |
| F05-W0318 | 216 | DJ-1c Vocal Rub | Rom | D1 p29 | |
| F05-W0319 | 217 | DJ-BD Rub | Rom | D1 p29 | |
| F05-W0320 | 218 | DJ-SD Rub | Rom | D1 p29 | |
| F05-W0321 | 219 | DJ-Record Stop | Rom | D1 p29 | |
| F05-W0322 | 220 | DJ-Reverse | Rom | D1 p29 | |
| F05-W0323 | 221 | DJ-Old Record | Rom | D1 p29 | |
| F05-W0324 | 222 | Orchestra Hit | Rom | D1 p29 | |
| F05-W0325 | 223 | Pizz Hit | Rom | D1 p29 | |
| F05-W0326 | 224 | Band Hit | Rom | D1 p29 | |
| F05-W0327 | 225 | ImpactHit | Rom | D1 p29 | |
| F05-W0328 | 226 | Philly Hit | Rom | D1 p29 | |
| F05-W0329 | 227 | Drumstick Hit | Rom | D1 p29 | |
| F05-W0330 | 228 | Finger Snap | Rom | D1 p29 | |
| F05-W0331 | 229 | Hand Claps | Rom | D1 p29 | |
| F05-W0332 | 230 | Hand Claps-Hip | Rom | D1 p29 | |
| F05-W0333 | 231 | Hand Claps-Happy | Rom | D1 p29 | |
| F05-W0334 | 232 | Conga Lo-Open | Rom | D1 p29 | |
| F05-W0335 | 233 | Conga Lo-MtSlap | Rom | D1 p29 | |
| F05-W0336 | 234 | Conga Lo-Slap | Rom | D1 p29 | |
| F05-W0337 | 235 | Conga Hi-Open | Rom | D1 p29 | |
| F05-W0338 | 236 | Conga Hi-Mute | Rom | D1 p29 | |
| F05-W0339 | 237 | Conga Hi-MtSlap | Rom | D1 p29 | |
| F05-W0340 | 238 | Conga Hi-Slap 1 | Rom | D1 p29 | |
| F05-W0341 | 239 | Conga Hi-Slap 2 | Rom | D1 p29 | |
| F05-W0342 | 240 | Conga-Heel | Rom | D1 p29 | |
| F05-W0343 | 241 | Conga-Toe | Rom | D1 p29 | |
| F05-W0344 | 242 | Bongo Lo-Open | Rom | D1 p29 | |
| F05-W0345 | 243 | Bongo Lo-Slap | Rom | D1 p29 | |
| F05-W0346 | 244 | Bongo Lo-Stick | Rom | D1 p29 | |
| F05-W0347 | 245 | Bongo Hi-Open | Rom | D1 p29 | |
| F05-W0348 | 246 | Bongo Hi-Slap | Rom | D1 p29 | |
| F05-W0349 | 247 | Bongo Hi-Stick 1 | Rom | D1 p29 | |
| F05-W0350 | 248 | Bongo Hi-Stick 2 | Rom | D1 p29 | |
| F05-W0351 | 249 | Djembe-Open | Rom | D1 p29 | |
| F05-W0352 | 250 | Djembe-Mute | Rom | D1 p29 | |
| F05-W0353 | 251 | Djembe-Slap | Rom | D1 p29 | |
| F05-W0354 | 252 | Castanet-Single | Rom | D1 p29 | |
| F05-W0355 | 253 | Castanet-Dble 1 | Rom | D1 p29 | |
| F05-W0356 | 254 | Castanet-Dble 2 | Rom | D1 p29 | |
| F05-W0357 | 255 | Guiro-Long | Rom | D1 p29 | |
| F05-W0358 | 256 | Guiro-Short | Rom | D1 p29 | |
| F05-W0359 | 257 | Guiro-Tap | Rom | D1 p29 | |
| F05-W0360 | 258 | Maracas-Push | Rom | D1 p29 | |
| F05-W0361 | 259 | Maracas-Pull | Rom | D1 p29 | |
| F05-W0362 | 260 | Baya-Open | Rom | D1 p29 | |
| F05-W0363 | 261 | Baya-Ghe | Rom | D1 p29 | |
| F05-W0364 | 262 | Baya-Mute 1 | Rom | D1 p29 | |
| F05-W0365 | 263 | Baya-Mute 2 | Rom | D1 p29 | |
| F05-W0366 | 264 | Baya-Mute 3 | Rom | D1 p29 | |
| F05-W0367 | 265 | Baya-Mute 4 | Rom | D1 p29 | |
| F05-W0368 | 266 | Baya-Mute 5 | Rom | D1 p29 | |
| F05-W0369 | 267 | Tabla-Na | Rom | D1 p29 | |
| F05-W0370 | 268 | Tabla-Open | Rom | D1 p29 | |
| F05-W0371 | 269 | Tabla-Tin | Rom | D1 p29 | |
| F05-W0372 | 270 | Tabla-Mute 1 | Rom | D1 p29 | |
| F05-W0373 | 271 | Tabla-Mute 2 | Rom | D1 p29 | |
| F05-W0374 | 272 | Tabla-Mute 3 | Rom | D1 p29 | |
| F05-W0375 | 273 | Tabla-Mute 4 | Rom | D1 p29 | |
| F05-W0376 | 274 | Taiko-Open | Rom | D1 p29 | |
| F05-W0377 | 275 | Taiko-Rim | Rom | D1 p29 | |
| F05-W0378 | 276 | Tsuzumi | Rom | D1 p29 | |
| F05-W0379 | 277 | Vibraslap | Rom | D1 p29 | |
| F05-W0380 | 278 | Claves | Rom | D1 p29 | |
| F05-W0381 | 279 | Woodblock 1 | Rom | D1 p29 | |
| F05-W0382 | 280 | Woodblock 2 | Rom | D1 p30 | |
| F05-W0383 | 281 | Timbale Lo-Open | Rom | D1 p30 | |
| F05-W0384 | 282 | Timbale Lo-Mute | Rom | D1 p30 | |
| F05-W0385 | 283 | Timbale Lo-Rim | Rom | D1 p30 | |
| F05-W0386 | 284 | Timbale Hi-Edge | Rom | D1 p30 | |
| F05-W0387 | 285 | Timbale Hi-Rim 1 | Rom | D1 p30 | |
| F05-W0388 | 286 | Timbale Hi-Rim 2 | Rom | D1 p30 | |
| F05-W0389 | 287 | Timbale-Paila | Rom | D1 p30 | |
| F05-W0390 | 288 | Tambourine-Push | Rom | D1 p30 | |
| F05-W0391 | 289 | Tambourine-Pull | Rom | D1 p30 | |
| F05-W0392 | 290 | Tambourine-Acc.1 | Rom | D1 p30 | |
| F05-W0393 | 291 | Tambourine-Acc.2 | Rom | D1 p30 | |
| F05-W0394 | 292 | Triangle-Open | Rom | D1 p30 | |
| F05-W0395 | 293 | Triangle-Mute | Rom | D1 p30 | |
| F05-W0396 | 294 | Triangle-Roll | Rom | D1 p30 | |
| F05-W0397 | 295 | Cuica-Hi | Rom | D1 p30 | |
| F05-W0398 | 296 | Cuica-Lo | Rom | D1 p30 | |
| F05-W0399 | 297 | Shaker 1 | Rom | D1 p30 | |
| F05-W0400 | 298 | Shaker 2 | Rom | D1 p30 | |
| F05-W0401 | 299 | Cabasa-Up | Rom | D1 p30 | |
| F05-W0402 | 300 | Cabasa-Down | Rom | D1 p30 | |
| F05-W0403 | 301 | Cabasa-Tap | Rom | D1 p30 | |
| F05-W0404 | 302 | Caxixi-Hard | Rom | D1 p30 | |
| F05-W0405 | 303 | Caxixi-Soft | Rom | D1 p30 | |
| F05-W0406 | 304 | Agogo Bell | Rom | D1 p30 | |
| F05-W0407 | 305 | Cowbell 1 | Rom | D1 p30 | |
| F05-W0408 | 306 | Cowbell 2 | Rom | D1 p30 | |
| F05-W0409 | 307 | Chacha Bell | Rom | D1 p30 | |
| F05-W0410 | 308 | Mambo Bell | Rom | D1 p30 | |
| F05-W0411 | 309 | Sleigh Bell | Rom | D1 p30 | |
| F05-W0412 | 310 | Rap Sleighbell | Rom | D1 p30 | |
| F05-W0413 | 311 | Finger Cymbal | Rom | D1 p30 | |
| F05-W0414 | 312 | Marc Tree | Rom | D1 p30 | |
| F05-W0415 | 313 | Marc Tree LP | Rom | D1 p30 | |
| F05-W0416 | 314 | Bell Tree | Rom | D1 p30 | |
| F05-W0417 | 315 | Flexatone | Rom | D1 p30 | |
| F05-W0418 | 316 | Samba Whistle | Rom | D1 p30 | |
| F05-W0419 | 317 | Chinese Gong | Rom | D1 p30 | |
| F05-W0420 | 318 | Industry | Rom | D1 p30 | |
| F05-W0421 | 319 | Cyber Drum | Rom | D1 p30 | |
| F05-W0422 | 320 | SD-Metal Striker | Rom | D1 p30 | |
| F05-W0423 | 321 | SD-Air Explosion | Rom | D1 p30 | |
| F05-W0424 | 322 | Industry Loop 1 | Rom | D1 p30 | |
| F05-W0425 | 323 | Industry Loop 2 | Rom | D1 p30 | |
| F05-W0426 | 324 | Metal Hit | Rom | D1 p30 | |
| F05-W0427 | 325 | Yea Solo | Rom | D1 p30 | |
| F05-W0428 | 326 | Year solo | Rom | D1 p30 | |
| F05-W0429 | 327 | Uhh Solo | Rom | D1 p30 | |
| F05-W0430 | 328 | Hit It | Rom | D1 p30 | |
| F05-W0431 | 329 | Uhhhh Solo | Rom | D1 p30 | |
| F05-W0432 | 330 | CompuVoice-Beep | Rom | D1 p30 | |
| F05-W0433 | 331 | CompuVoice-Noise | Rom | D1 p30 | |
| F05-W0434 | 332 | CompuVoice-Chi | Rom | D1 p30 | |
| F05-W0435 | 333 | CompuVoice-Test | Rom | D1 p30 | |
| F05-W0436 | 334 | CompuVoice-Do it | Rom | D1 p30 | |
| F05-W0437 | 335 | CompuVoice-When | Rom | D1 p30 | |
| F05-W0438 | 336 | CompuVoice-Toi | Rom | D1 p30 | |
| F05-W0439 | 337 | CompuVoice-Ti | Rom | D1 p30 | |
| F05-W0440 | 338 | "Four..." Solo | Rom | D1 p30 | |
| F05-W0441 | 339 | "Three..." Solo | Rom | D1 p30 | |
| F05-W0442 | 340 | "Two..." Solo | Rom | D1 p30 | |
| F05-W0443 | 341 | "One..." Solo | Rom | D1 p30 | |
| F05-W0444 | 342 | PC Vox-Sis | Rom | D1 p30 | |
| F05-W0445 | 343 | PC Vox-Dunc | Rom | D1 p30 | |
| F05-W0446 | 344 | PC Vox-Cks | Rom | D1 p30 | |
| F05-W0447 | 345 | PC Vox-For | Rom | D1 p30 | |
| F05-W0448 | 346 | PC Vox-One | Rom | D1 p30 | |
| F05-W0449 | 347 | PC Vox-If | Rom | D1 p30 | |
| F05-W0450 | 348 | PC Vox-Zee | Rom | D1 p30 | |
| F05-W0451 | 349 | PC Vox-Ae | Rom | D1 p30 | |
| F05-W0452 | 350 | PC Vox-Pa | Rom | D1 p30 | |
| F05-W0453 | 351 | PC Vox-Sh | Rom | D1 p30 | |
| F05-W0454 | 352 | PC Vox-O | Rom | D1 p30 | |
| F05-W0455 | 353 | PC Vox-Tu | Rom | D1 p30 | |
| F05-W0456 | 354 | Stadium | Rom | D1 p30 | |
| F05-W0457 | 355 | Applause | Rom | D1 p30 | |
| F05-W0458 | 356 | Scream | Rom | D1 p30 | |
| F05-W0459 | 357 | Laughing | Rom | D1 p30 | |
| F05-W0460 | 358 | Footstep 1 | Rom | D1 p30 | |
| F05-W0461 | 359 | Footstep 2 | Rom | D1 p30 | |
| F05-W0462 | 360 | Click | Rom | D1 p30 | |
| F05-W0463 | 361 | Bird 1 | Rom | D1 p30 | |
| F05-W0464 | 362 | Bird 2 | Rom | D1 p30 | |
| F05-W0465 | 363 | Dog | Rom | D1 p30 | |
| F05-W0466 | 364 | Gallop | Rom | D1 p30 | |
| F05-W0467 | 365 | Crickets 1 | Rom | D1 p30 | |
| F05-W0468 | 366 | Crickets 2 | Rom | D1 p30 | |
| F05-W0469 | 367 | Hart Beat | Rom | D1 p30 | |
| F05-W0470 | 368 | Punch | Rom | D1 p30 | |
| F05-W0471 | 369 | Tribe | Rom | D1 p30 | |
| F05-W0472 | 370 | GunShot | Rom | D1 p30 | |
| F05-W0473 | 371 | Rainstick | Rom | D1 p30 | |
| F05-W0474 | 372 | Door Creak | Rom | D1 p30 | |
| F05-W0475 | 373 | Door Slam | Rom | D1 p30 | |
| F05-W0476 | 374 | Car Engine | Rom | D1 p30 | |
| F05-W0477 | 375 | Car Stop | Rom | D1 p30 | |
| F05-W0478 | 376 | Car Pass | Rom | D1 p30 | |
| F05-W0479 | 377 | Car Crash | Rom | D1 p30 | |
| F05-W0480 | 378 | Train | Rom | D1 p30 | |
| F05-W0481 | 379 | Helicopter | Rom | D1 p30 | |
| F05-W0482 | 380 | Gun Shot 2 | Rom | D1 p30 | |
| F05-W0483 | 381 | MachineGun | Rom | D1 p30 | |
| F05-W0484 | 382 | Laser Gun | Rom | D1 p30 | |
| F05-W0485 | 383 | Explosion | Rom | D1 p30 | |
| F05-W0486 | 384 | Thunder | Rom | D1 p30 | |
| F05-W0487 | 385 | Wind | Rom | D1 p30 | |
| F05-W0488 | 386 | Stream | Rom | D1 p30 | |
| F05-W0489 | 387 | Bubble | Rom | D1 p30 | |
| F05-W0490 | 388 | ChurchBell | Rom | D1 p30 | |
| F05-W0491 | 389 | Telephone Ring | Rom | D1 p30 | |
| F05-W0492 | 390 | Space Lore | Rom | D1 p30 | |
| F05-W0493 | 391 | Swish Terra | Rom | D1 p30 | |
| F05-W0494 | 392 | Xylophone Spectr | Rom | D1 p30 | |
| F05-W0495 | 393 | Cricket Spectrum | Rom | D1 p30 | |
| F05-W0496 | 394 | Air Vortex | Rom | D1 p30 | |
| F05-W0497 | 395 | Noise-White | Rom | D1 p30 | |
| F05-W0498 | 396 | Noise-FM Mod | Rom | D1 p30 | |
| F05-W0499 | 397 | Tubular | Rom | D1 p30 | |
| F05-W0500 | 398 | Gamelan | Rom | D1 p30 | |
| F05-W0501 | 399 | Tambura | Rom | D1 p30 | |
| F05-W0502 | 400 | GtCutNois1 | Rom | D1 p30 | |
| F05-W0503 | 401 | GtCutNois2 | Rom | D1 p30 | |
| F05-W0504 | 402 | StringSlap | Rom | D1 p30 | |
| F05-W0505 | 403 | HarpGliss | Rom | D1 p30 | |
| F05-W0506 | 404 | Power Chord | Rom | D1 p30 | |
| F05-W0507 | 405 | Fret Noise | Rom | D1 p30 | |
| F05-W0508 | 406 | Dist.Slide 1 | Rom | D1 p30 | |
| F05-W0509 | 407 | Dist.Slide 2 | Rom | D1 p30 | |
| F05-W0510 | 408 | E.Guitar Pick 1 | Rom | D1 p30 | |
| F05-W0511 | 409 | E.Guitar Pick 2 | Rom | D1 p30 | |
| F05-W0512 | 410 | E.Guitar Pick 3 | Rom | D1 p30 | |
| F05-W0513 | 411 | Guitar Scratch | Rom | D1 p30 | |
| F05-W0514 | 412 | Amp Noise | Rom | D1 p30 | |
| F05-W0515 | 413 | Vox-Wah Gtr 1 | Rom | D1 p30 | |
| F05-W0516 | 414 | Vox-Wah Gtr 2 | Rom | D1 p30 | |
| F05-W0517 | 415 | Vox-Wah Gtr 3 | Rom | D1 p30 | |
| F05-W0518 | 416 | Vox-Wah Gtr 4 | Rom | D1 p30 | |

