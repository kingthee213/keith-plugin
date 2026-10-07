# FAMILY 01 - factory programs and combinations (internal reference)

Reference machine (internal name, never shipped): **Triton**.
Shipped label: **Family 01**.

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
| D1 | TRITON Voice Name List (Korg, file TRITON_VNL_EFG2.pdf, 409.5 KB) | official Korg download page, product TRITON, document id 1652 | https://www.korg.com/us/support/download/manual/1/214/1652/ | combinations (banks A-D), programs (banks A-D and GM bank G with its g(1)-g(9)/g(d) variant rows), drum kits with the per-key drumsample mapping, GM drum kits, multisamples, drumsamples |
| D2 | TRITON Parameter Guide (Korg, file TRITON_PG_E.pdf, 255 pages, created 1999-12-16) | official Korg download page, product TRITON, document id 1648 | https://www.korg.com/us/support/download/manual/1/214/1648/ | effect algorithm catalogue (000-102) and the insert/master effect routing architecture |

## Counts as documented

| Section | Count | Documented banks |
|---|---|---|
| Factory combinations | 512 | A, B, C, D |
| Factory programs | 777 | A, B, C, D, G |
| Factory drum kits | 16 | n/a |
| GM drum kits | 9 | n/a |
| ROM multisamples (PCM waves) | 425 | n/a |
| ROM drumsamples (PCM waves) | 413 | n/a |

- Combinations: **512** documented entries (banks A, B, C, D).
- Programs: **777** documented entries (banks A, B, C, D, G).
- Drum kits: **16** factory kits plus **9** GM kits - see `FAMILY-01-DRUMS.md`.
- PCM multisamples: **425** documented rows.
- PCM drumsamples: **413** documented rows.

## Combinations

Category is the document's own category column. Every row is one documented factory combination.

| Shipped label | Documented name | Bank | Number | Category | Source | Note |
|---|---|---|---|---|---|---|
| F01-C0001 | Romance Layers | A | A000 | Keyboard | D1 p2 |  |
| F01-C0002 | Lonely Moon | A | A001 | BassSplits | D1 p2 |  |
| F01-C0003 | DynOrchestra 1 | A | A002 | Orchestral | D1 p2 |  |
| F01-C0004 | Drum'n'Bass Kit | A | A003 | Rhythmic Pattern | D1 p2 |  |
| F01-C0005 | Vocoderhythm | A | A004 | MotionSynth | D1 p2 |  |
| F01-C0006 | [(Goliath )] | A | A005 | Synth | D1 p2 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0007 | Ugly Momma D.Kit | A | A006 | Ds/Hits | D1 p2 |  |
| F01-C0008 | My Baby's Asleep | A | A007 | Guitar/Plucked | D1 p2 |  |
| F01-C0009 | Knob as Drawbar1 | A | A008 | Organ | D1 p2 |  |
| F01-C0010 | Indian Ocean | A | A009 | Rhythmic Pattern | D1 p2 |  |
| F01-C0011 | *Pad Tropolis* | A | A010 | Pads | D1 p2 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0012 | Big Jazz Band | A | A011 | BrassReed | D1 p2 |  |
| F01-C0013 | Bell Amis | A | A012 | Bell/Mallet/Perc | D1 p2 |  |
| F01-C0014 | Golden Strings | A | A013 | Strings | D1 p2 |  |
| F01-C0015 | Leading Lady | A | A014 | LeadSplits | D1 p2 |  |
| F01-C0016 | Barter Town | A | A015 | Complex & SE | D1 p2 |  |
| F01-C0017 | Digi SynPiano 1 | A | A016 | Keyboard | D1 p2 |  |
| F01-C0018 | [Ribbon Rappin'] | A | A017 | BassSplits | D1 p2 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0019 | Rhythms & Bows | A | A018 | Orchestral | D1 p2 |  |
| F01-C0020 | Stabbin' Split | A | A019 | Rhythmic Pattern | D1 p2 |  |
| F01-C0021 | Falling Leaves | A | A020 | MotionSynth | D1 p2 |  |
| F01-C0022 | Noisy Funk Synth | A | A021 | Synth | D1 p2 |  |
| F01-C0023 | Drumfest | A | A022 | Ds/Hits | D1 p2 |  |
| F01-C0024 | Strumming Guitar | A | A023 | Guitar/Plucked | D1 p2 |  |
| F01-C0025 | Ballad Organ | A | A024 | Organ | D1 p2 |  |
| F01-C0026 | World Winds | A | A025 | World | D1 p2 |  |
| F01-C0027 | Universal Choir | A | A026 | Pads | D1 p2 |  |
| F01-C0028 | The Power Trio | A | A027 | BrassReed | D1 p2 |  |
| F01-C0029 | Random Blocks | A | A028 | Bell/Mallet/Perc | D1 p2 |  |
| F01-C0030 | Bows&KnobHarpsi | A | A029 | Strings | D1 p2 |  |
| F01-C0031 | Home Suite Home | A | A030 | LeadSplits | D1 p2 |  |
| F01-C0032 | Meteor Shower | A | A031 | Complex & SE | D1 p2 |  |
| F01-C0033 | Tiney Harmonic | A | A032 | Keyboard | D1 p3 |  |
| F01-C0034 | after-T-house | A | A033 | BassSplits | D1 p3 |  |
| F01-C0035 | Small Orchestra | A | A034 | Orchestral | D1 p3 |  |
| F01-C0036 | Organic Beats | A | A035 | Rhythmic Pattern | D1 p3 |  |
| F01-C0037 | /\Gods Bathtub/\ | A | A036 | MotionSynth | D1 p3 |  |
| F01-C0038 | Talkin'RaveSynth | A | A037 | Synth | D1 p3 |  |
| F01-C0039 | Arp Session Kit | A | A038 | Ds/Hits | D1 p3 |  |
| F01-C0040 | Wavetable Guitar | A | A039 | Guitar/Plucked | D1 p3 |  |
| F01-C0041 | 2nd Harm.on Knob | A | A040 | Organ | D1 p3 |  |
| F01-C0042 | Fisatic Dreams | A | A041 | World | D1 p3 |  |
| F01-C0043 | Simple Pad | A | A042 | Pads | D1 p3 |  |
| F01-C0044 | Big Brassed | A | A043 | BrassReed | D1 p3 |  |
| F01-C0045 | Angelic Bells | A | A044 | Bell/Mallet/Perc | D1 p3 |  |
| F01-C0046 | CinematicStrings | A | A045 | Strings | D1 p3 |  |
| F01-C0047 | Hidden Rhythm | A | A046 | LeadSplits | D1 p3 |  |
| F01-C0048 | The Rain Forest | A | A047 | Complex & SE | D1 p3 |  |
| F01-C0049 | Big Bottom Clav | A | A048 | Keyboard | D1 p3 |  |
| F01-C0050 | Bass & PianoPad | A | A049 | BassSplits | D1 p3 |  |
| F01-C0051 | French Ensemble | A | A050 | Orchestral | D1 p3 |  |
| F01-C0052 | : Crusin'Compton: | A | A051 | Rhythmic Pattern | D1 p3 |  |
| F01-C0053 | ***Ice*Rain*** | A | A052 | MotionSynth | D1 p3 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0054 | Spiky & Tight | A | A053 | Synth | D1 p3 |  |
| F01-C0055 | Crazy Hit Tricks | A | A054 | Ds/Hits | D1 p3 |  |
| F01-C0056 | Sofia's Place | A | A055 | Guitar/Plucked | D1 p3 |  |
| F01-C0057 | Upper&Lower Org. | A | A056 | Organ | D1 p3 |  |
| F01-C0058 | Irish Ballade | A | A057 | World | D1 p3 |  |
| F01-C0059 | The BeeG Pad | A | A058 | Pads | D1 p3 |  |
| F01-C0060 | MutedButSpeakin' | A | A059 | BrassReed | D1 p3 |  |
| F01-C0061 | Bellish Pad | A | A060 | Bell/Mallet/Perc | D1 p3 |  |
| F01-C0062 | AfterRainStrings | A | A061 | Strings | D1 p3 |  |
| F01-C0063 | DreamingForMiles | A | A062 | LeadSplits | D1 p3 |  |
| F01-C0064 | Melanco | A | A063 | Complex & SE | D1 p3 |  |
| F01-C0065 | ModernPiano | A | A064 | Keyboard | D1 p4 |  |
| F01-C0066 | Gig Split | A | A065 | BassSplits | D1 p4 |  |
| F01-C0067 | Woodwinds | A | A066 | Orchestral | D1 p4 |  |
| F01-C0068 | Steely Keys | A | A067 | Rhythmic Pattern | D1 p4 |  |
| F01-C0069 | **Weightless** | A | A068 | MotionSynth | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0070 | BIG Wash Synth | A | A069 | Synth | D1 p4 |  |
| F01-C0071 | Super Hip Kit | A | A070 | Ds/Hits | D1 p4 |  |
| F01-C0072 | Harmonic Whisper | A | A071 | Guitar/Plucked | D1 p4 |  |
| F01-C0073 | Rock Organ | A | A072 | Organ | D1 p4 |  |
| F01-C0074 | Peace On Earth | A | A073 | World | D1 p4 |  |
| F01-C0075 | I'm sorry... | A | A074 | Pads | D1 p4 |  |
| F01-C0076 | Sop/Alt/Tnr/Bari | A | A075 | BrassReed | D1 p4 |  |
| F01-C0077 | Emtim Bells | A | A076 | Bell/Mallet/Perc | D1 p4 |  |
| F01-C0078 | Smoothie Strings | A | A077 | Strings | D1 p4 |  |
| F01-C0079 | Pad & ShakuLead | A | A078 | LeadSplits | D1 p4 |  |
| F01-C0080 | All Was Lost | A | A079 | Complex & SE | D1 p4 |  |
| F01-C0081 | EP Stack (Knobs) | A | A080 | Keyboard | D1 p4 |  |
| F01-C0082 | DanceSynthSplit | A | A081 | BassSplits | D1 p4 |  |
| F01-C0083 | Bows 'n' Brass | A | A082 | Orchestral | D1 p4 |  |
| F01-C0084 | DoItToYaFeet (C) | A | A083 | Rhythmic Pattern | D1 p4 |  |
| F01-C0085 | <<SYNERGY>> | A | A084 | MotionSynth | D1 p4 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0086 | Cosmic Sweeper | A | A085 | Synth | D1 p4 |  |
| F01-C0087 | SciFi ChaseScene | A | A086 | Rhythmic Pattern | D1 p4 |  |
| F01-C0088 | Mountainside | A | A087 | Guitar/Plucked | D1 p4 |  |
| F01-C0089 | Gospel Church | A | A088 | Organ | D1 p4 |  |
| F01-C0090 | Koto Environment | A | A089 | World | D1 p4 |  |
| F01-C0091 | BeeG Pad2 Bright | A | A090 | Pads | D1 p4 |  |
| F01-C0092 | Spittin' Brass | A | A091 | BrassReed | D1 p4 |  |
| F01-C0093 | Rotary Bellz | A | A092 | Bell/Mallet/Perc | D1 p4 |  |
| F01-C0094 | Strings Of Silk | A | A093 | Strings | D1 p4 |  |
| F01-C0095 | SweetBabyBlues | A | A094 | LeadSplits | D1 p4 |  |
| F01-C0096 | Jungle Stalker | A | A095 | Complex & SE | D1 p4 |  |
| F01-C0097 | Comp-o-Net | A | A096 | Keyboard | D1 p5 |  |
| F01-C0098 | Happily Split | A | A097 | BassSplits | D1 p5 |  |
| F01-C0099 | Dry Chamber Ens. | A | A098 | Orchestral | D1 p5 |  |
| F01-C0100 | 70's Mirror Ball | A | A099 | Rhythmic Pattern | D1 p5 |  |
| F01-C0101 | Dust Devil | A | A100 | MotionSynth | D1 p5 |  |
| F01-C0102 | Power Synth | A | A101 | Synth | D1 p5 |  |
| F01-C0103 | The16th Strummer | A | A102 | Rhythmic Pattern | D1 p5 |  |
| F01-C0104 | New Breed of Gtr | A | A103 | Guitar/Plucked | D1 p5 |  |
| F01-C0105 | Pipes on SW&Knbs | A | A104 | Organ | D1 p5 |  |
| F01-C0106 | Tropic Of Cancer | A | A105 | World | D1 p5 |  |
| F01-C0107 | Please stay calm | A | A106 | Pads | D1 p5 |  |
| F01-C0108 | Warm Brass Ens. | A | A107 | BrassReed | D1 p5 |  |
| F01-C0109 | Comfort Bell | A | A108 | Bell/Mallet/Perc | D1 p5 |  |
| F01-C0110 | Sugar Plums | A | A109 | Strings | D1 p5 |  |
| F01-C0111 | Ambient House | A | A110 | LeadSplits | D1 p5 |  |
| F01-C0112 | Alien Invasion | A | A111 | Complex & SE | D1 p5 |  |
| F01-C0113 | Acoustic Mix | A | A112 | Keyboard | D1 p5 |  |
| F01-C0114 | 5th Synth Split | A | A113 | BassSplits | D1 p5 |  |
| F01-C0115 | Sweet Dyn-Orch. | A | A114 | Orchestral | D1 p5 |  |
| F01-C0116 | Prodisynth Split | A | A115 | Rhythmic Pattern | D1 p5 |  |
| F01-C0117 | Planet Pad | A | A116 | MotionSynth | D1 p5 |  |
| F01-C0118 | Square Pad Lead | A | A117 | Synth | D1 p5 |  |
| F01-C0119 | Atmosphere Hit! | A | A118 | Ds/Hits | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0120 | Guitar Sings | A | A119 | Guitar/Plucked | D1 p5 |  |
| F01-C0121 | Holy Pipes | A | A120 | Organ | D1 p5 |  |
| F01-C0122 | Gamelan Band | A | A121 | World | D1 p5 |  |
| F01-C0123 | **The Toy Box** | A | A122 | Pads | D1 p5 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0124 | FunkyBrass Sect. | A | A123 | BrassReed | D1 p5 |  |
| F01-C0125 | Little Princess | A | A124 | Bell/Mallet/Perc | D1 p5 |  |
| F01-C0126 | Bowed Strings | A | A125 | Strings | D1 p5 |  |
| F01-C0127 | Life Together | A | A126 | LeadSplits | D1 p5 |  |
| F01-C0128 | One FingerTVShow | A | A127 | Complex & SE | D1 p5 |  |
| F01-C0129 | Foster Layer EP | B | B000 | Keyboard | D1 p6 |  |
| F01-C0130 | Phatt Bass Split | B | B001 | BassSplits | D1 p6 |  |
| F01-C0131 | DynOrchestra 2 | B | B002 | Orchestral | D1 p6 |  |
| F01-C0132 | Formant & Lead | B | B003 | Rhythmic Pattern | D1 p6 |  |
| F01-C0133 | Beat Meditation | B | B004 | MotionSynth | D1 p6 |  |
| F01-C0134 | BIG Power Synth | B | B005 | Synth | D1 p6 |  |
| F01-C0135 | Velo HiFi Kit | B | B006 | Ds/Hits | D1 p6 |  |
| F01-C0136 | Alchemy Layer | B | B007 | Guitar/Plucked | D1 p6 |  |
| F01-C0137 | Jazz Organ 2 | B | B008 | Organ | D1 p6 |  |
| F01-C0138 | Prince of Egypt | B | B009 | Rhythmic Pattern | D1 p6 |  |
| F01-C0139 | 5th.Dimension | B | B010 | Synth | D1 p6 |  |
| F01-C0140 | HybridBrassStab | B | B011 | BrassReed | D1 p6 |  |
| F01-C0141 | Cascade Bells | B | B012 | Bell/Mallet/Perc | D1 p6 |  |
| F01-C0142 | ClassicAnaString | B | B013 | Strings | D1 p6 |  |
| F01-C0143 | SimplePad&SopSax | B | B014 | LeadSplits | D1 p6 |  |
| F01-C0144 | {Jealous Robots} | B | B015 | Complex & SE | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0145 | Digi SynPiano 2 | B | B016 | Keyboard | D1 p6 |  |
| F01-C0146 | < The Trio> | B | B017 | BassSplits | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0147 | Orchestra | B | B018 | Orchestral | D1 p6 |  |
| F01-C0148 | Waterboy Split | B | B019 | Rhythmic Pattern | D1 p6 |  |
| F01-C0149 | Dawn Arp. Pad | B | B020 | MotionSynth | D1 p6 |  |
| F01-C0150 | Mr. Rezzocomp | B | B021 | Synth | D1 p6 |  |
| F01-C0151 | Percfest!! | B | B022 | Ds/Hits | D1 p6 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0152 | 12-Strings (Arp) | B | B023 | Guitar/Plucked | D1 p6 |  |
| F01-C0153 | Gospel Organ | B | B024 | Organ | D1 p6 |  |
| F01-C0154 | Beyond Nepal... | B | B025 | World | D1 p6 |  |
| F01-C0155 | ...Water to Wine | B | B026 | MotionSynth | D1 p6 |  |
| F01-C0156 | Trp/Tb/Alto/Tenr | B | B027 | BrassReed | D1 p6 |  |
| F01-C0157 | Stereo Mallets | B | B028 | Bell/Mallet/Perc | D1 p6 |  |
| F01-C0158 | Strings & Wings | B | B029 | Strings | D1 p6 |  |
| F01-C0159 | Night Reeds | B | B030 | LeadSplits | D1 p6 |  |
| F01-C0160 | Puff Comper | B | B031 | Rhythmic Pattern | D1 p6 |  |
| F01-C0161 | Dark Bell EP | B | B032 | Keyboard | D1 p7 |  |
| F01-C0162 | Bass/EP Pad Beat | B | B033 | BassSplits | D1 p7 |  |
| F01-C0163 | Brash Tpts&Choir | B | B034 | Orchestral | D1 p7 |  |
| F01-C0164 | Slammin'Bs&Piano | B | B035 | Rhythmic Pattern | D1 p7 |  |
| F01-C0165 | Whopper Pad | B | B036 | MotionSynth | D1 p7 |  |
| F01-C0166 | Ana Logger | B | B037 | Synth | D1 p7 |  |
| F01-C0167 | Mineshaft | B | B038 | Ds/Hits | D1 p7 |  |
| F01-C0168 | Magic Guitar | B | B039 | Guitar/Plucked | D1 p7 |  |
| F01-C0169 | 3rd Harm.on Knob | B | B040 | Organ | D1 p7 |  |
| F01-C0170 | Peasant Song | B | B041 | World | D1 p7 |  |
| F01-C0171 | Real EP & Pad | B | B042 | Keyboard | D1 p7 |  |
| F01-C0172 | Dry & Funky Keys | B | B043 | Keyboard | D1 p7 |  |
| F01-C0173 | Happy Bells | B | B044 | Bell/Mallet/Perc | D1 p7 |  |
| F01-C0174 | Oct. Strings | B | B045 | Strings | D1 p7 |  |
| F01-C0175 | Warrior Princess | B | B046 | LeadSplits | D1 p7 |  |
| F01-C0176 | Garage Nite | B | B047 | Rhythmic Pattern | D1 p7 |  |
| F01-C0177 | Cutting Clav | B | B048 | Keyboard | D1 p7 |  |
| F01-C0178 | Jazz Bass/Gtr. | B | B049 | BassSplits | D1 p7 |  |
| F01-C0179 | Orchestral Brass | B | B050 | Orchestral | D1 p7 |  |
| F01-C0180 | slow... crawl... | B | B051 | Rhythmic Pattern | D1 p7 |  |
| F01-C0181 | Forget-Me-Not | B | B052 | MotionSynth | D1 p7 |  |
| F01-C0182 | Buzz-A-Log | B | B053 | Synth | D1 p7 |  |
| F01-C0183 | * Instant Euro* | B | B054 | Rhythmic Pattern | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0184 | Harp Strings | B | B055 | Guitar/Plucked | D1 p7 |  |
| F01-C0185 | Split ClickOrgan | B | B056 | Organ | D1 p7 |  |
| F01-C0186 | Super Stereo EP | B | B057 | Keyboard | D1 p7 |  |
| F01-C0187 | << Godchild >> | B | B058 | MotionSynth | D1 p7 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0188 | Fat Horn Section | B | B059 | BrassReed | D1 p7 |  |
| F01-C0189 | Pad & Logs | B | B060 | Bell/Mallet/Perc | D1 p7 |  |
| F01-C0190 | Orchestral Pipes | B | B061 | Organ | D1 p7 |  |
| F01-C0191 | Oboe Split | B | B062 | LeadSplits | D1 p7 |  |
| F01-C0192 | Echo Jamm | B | B063 | Rhythmic Pattern | D1 p7 |  |
| F01-C0193 | Wide L/R Piano | B | B064 | Keyboard | D1 p8 |  |
| F01-C0194 | Bass / Vibe | B | B065 | BassSplits | D1 p8 |  |
| F01-C0195 | Knob as Drawbar2 | B | B066 | Organ | D1 p8 |  |
| F01-C0196 | *HipTrip Split* | B | B067 | Rhythmic Pattern | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0197 | Haunted Waters | B | B068 | MotionSynth | D1 p8 |  |
| F01-C0198 | Mutant SoftSynth | B | B069 | Synth | D1 p8 |  |
| F01-C0199 | Snap!ReggaeOrgan | B | B070 | Rhythmic Pattern | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0200 | Time For Bed | B | B071 | Synth | D1 p8 |  |
| F01-C0201 | Velo Perc Organ | B | B072 | Organ | D1 p8 |  |
| F01-C0202 | Quidam | B | B073 | World | D1 p8 |  |
| F01-C0203 | Hi Key-Beauty-EP | B | B074 | Keyboard | D1 p8 |  |
| F01-C0204 | Sax Banquet | B | B075 | BrassReed | D1 p8 |  |
| F01-C0205 | Vaderian Bells | B | B076 | Bell/Mallet/Perc | D1 p8 |  |
| F01-C0206 | *Arctic Strings* | B | B077 | Pads | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0207 | Recorder & Orch. | B | B078 | LeadSplits | D1 p8 |  |
| F01-C0208 | Dirty Beat | B | B079 | Rhythmic Pattern | D1 p8 |  |
| F01-C0209 | Piano Nu-agey | B | B080 | Keyboard | D1 p8 |  |
| F01-C0210 | Rock Bass/Gtr. | B | B081 | BassSplits | D1 p8 |  |
| F01-C0211 | Gotham Snd.Track | B | B082 | Orchestral | D1 p8 |  |
| F01-C0212 | Junga-jam | B | B083 | Rhythmic Pattern | D1 p8 |  |
| F01-C0213 | || TRANSIT || | B | B084 | MotionSynth | D1 p8 |  |
| F01-C0214 | Res-Inator | B | B085 | Synth | D1 p8 |  |
| F01-C0215 | [<Moon Jam>] | B | B086 | Rhythmic Pattern | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0216 | Rock Is King! | B | B087 | Guitar/Plucked | D1 p8 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0217 | Knob as Drawbar3 | B | B088 | Organ | D1 p8 |  |
| F01-C0218 | Indian Layers | B | B089 | World | D1 p8 |  |
| F01-C0219 | Imaginary PizPad | B | B090 | MotionSynth | D1 p8 |  |
| F01-C0220 | SynBrass Tapper | B | B091 | Rhythmic Pattern | D1 p8 |  |
| F01-C0221 | Mallet Piano | B | B092 | Bell/Mallet/Perc | D1 p8 |  |
| F01-C0222 | LifeSigns EP | B | B093 | Synth | D1 p8 |  |
| F01-C0223 | SplitHornSection | B | B094 | LeadSplits | D1 p8 |  |
| F01-C0224 | Saucy Squares | B | B095 | Synth | D1 p8 |  |
| F01-C0225 | Synthetic EP's | B | B096 | Keyboard | D1 p9 |  |
| F01-C0226 | The Sneaky 3 | B | B097 | BassSplits | D1 p9 |  |
| F01-C0227 | Nu Vintage Orch | B | B098 | Orchestral | D1 p9 |  |
| F01-C0228 | Way Smooth Grv. | B | B099 | Rhythmic Pattern | D1 p9 |  |
| F01-C0229 | Wooshboard | B | B100 | MotionSynth | D1 p9 |  |
| F01-C0230 | Dynamic Change | B | B101 | Synth | D1 p9 |  |
| F01-C0231 | Knob Mixer Kit | B | B102 | Rhythmic Pattern | D1 p9 |  |
| F01-C0232 | Arp-A-Listic | B | B103 | BassSplits | D1 p9 |  |
| F01-C0233 | Pipes on Knobs | B | B104 | Organ | D1 p9 |  |
| F01-C0234 | Rain Festival | B | B105 | World | D1 p9 |  |
| F01-C0235 | Wave Of Time | B | B106 | MotionSynth | D1 p9 |  |
| F01-C0236 | Warm & Horny | B | B107 | BrassReed | D1 p9 |  |
| F01-C0237 | Bell and Tine | B | B108 | Keyboard | D1 p9 |  |
| F01-C0238 | Piano &Strings | B | B109 | Keyboard | D1 p9 |  |
| F01-C0239 | Riddem & EP | B | B110 | Rhythmic Pattern | D1 p9 |  |
| F01-C0240 | Suspected Hit | B | B111 | Ds/Hits | D1 p9 |  |
| F01-C0241 | MegaBrass&Piano | B | B112 | Keyboard | D1 p9 |  |
| F01-C0242 | Tough to say | B | B113 | BassSplits | D1 p9 |  |
| F01-C0243 | Pipes&Choir SW | B | B114 | Organ | D1 p9 |  |
| F01-C0244 | Bad Girl Shuffle | B | B115 | Rhythmic Pattern | D1 p9 |  |
| F01-C0245 | The digital sea | B | B116 | MotionSynth | D1 p9 |  |
| F01-C0246 | Digitalian | B | B117 | Synth | D1 p9 |  |
| F01-C0247 | Mr. Trendy | B | B118 | Ds/Hits | D1 p9 |  |
| F01-C0248 | Amby Plucked | B | B119 | MotionSynth | D1 p9 |  |
| F01-C0249 | Soft/Tutti on SW | B | B120 | Organ | D1 p9 |  |
| F01-C0250 | Steel Pan Band | B | B121 | World | D1 p9 |  |
| F01-C0251 | Future Japan | B | B122 | MotionSynth | D1 p9 |  |
| F01-C0252 | Wackadoo | B | B123 | LeadSplits | D1 p9 |  |
| F01-C0253 | Soft Piano Pad | B | B124 | Keyboard | D1 p9 |  |
| F01-C0254 | Universal Orch. | B | B125 | Pads | D1 p9 |  |
| F01-C0255 | Rock it ship | B | B126 | LeadSplits | D1 p9 |  |
| F01-C0256 | BX-3 Click Organ | B | B127 | Organ | D1 p9 |  |
| F01-C0257 | Layered A.Piano | C | C000 | Keyboard | D1 p10 |  |
| F01-C0258 | In The Room | C | C001 | BassSplits | D1 p10 |  |
| F01-C0259 | Film Tools | C | C002 | Orchestral | D1 p10 |  |
| F01-C0260 | Invisible Sun | C | C003 | Rhythmic Pattern | D1 p10 |  |
| F01-C0261 | Chromium Pad | C | C004 | MotionSynth | D1 p10 |  |
| F01-C0262 | Huge Ober Sweeps | C | C005 | Synth | D1 p10 |  |
| F01-C0263 | Alien Jungle Jam | C | C006 | Ds/Hits | D1 p10 |  |
| F01-C0264 | Encore (High C) | C | C007 | Guitar/Plucked | D1 p10 |  |
| F01-C0265 | Dirty Drawbars | C | C008 | Organ | D1 p10 |  |
| F01-C0266 | Synthitar Ens. | C | C009 | World | D1 p10 |  |
| F01-C0267 | Movie Pad | C | C010 | Pads | D1 p10 |  |
| F01-C0268 | Earth Brass | C | C011 | BrassReed | D1 p10 |  |
| F01-C0269 | RandomPanninBelz | C | C012 | Bell/Mallet/Perc | D1 p10 |  |
| F01-C0270 | Lush String Pad | C | C013 | Pads | D1 p10 |  |
| F01-C0271 | Tenor & Pad | C | C014 | LeadSplits | D1 p10 |  |
| F01-C0272 | Millennium Files | C | C015 | Complex & SE | D1 p10 |  |
| F01-C0273 | Sympathetic EP | C | C016 | Keyboard | D1 p10 |  |
| F01-C0274 | Bass &Elec.Grand | C | C017 | BassSplits | D1 p10 |  |
| F01-C0275 | ThickStringBrass | C | C018 | Orchestral | D1 p10 |  |
| F01-C0276 | Rez Stab Split | C | C019 | Rhythmic Pattern | D1 p10 |  |
| F01-C0277 | Plasma Beings | C | C020 | MotionSynth | D1 p10 |  |
| F01-C0278 | ModulaTech*Synth | C | C021 | Synth | D1 p10 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0279 | %%Orgambient%% | C | C022 | Rhythmic Pattern | D1 p10 |  |
| F01-C0280 | LastChanceToSee | C | C023 | Rhythmic Pattern | D1 p10 |  |
| F01-C0281 | Jazz Perc (Knob) | C | C024 | Organ | D1 p10 |  |
| F01-C0282 | SlowDance Piano | C | C025 | Keyboard | D1 p10 |  |
| F01-C0283 | Opera"Wind Free" | C | C026 | Pads | D1 p10 |  |
| F01-C0284 | Future Brass | C | C027 | BrassReed | D1 p10 |  |
| F01-C0285 | Wood+Steel Bars | C | C028 | Bell/Mallet/Perc | D1 p10 |  |
| F01-C0286 | Super Strings | C | C029 | Strings | D1 p10 |  |
| F01-C0287 | Mystic Presence | C | C030 | LeadSplits | D1 p10 |  |
| F01-C0288 | Next Millennium | C | C031 | Ds/Hits | D1 p10 |  |
| F01-C0289 | EP Bell Split | C | C032 | Keyboard | D1 p11 |  |
| F01-C0290 | Smooth Split | C | C033 | BassSplits | D1 p11 |  |
| F01-C0291 | DblReeds&Strings | C | C034 | Orchestral | D1 p11 |  |
| F01-C0292 | The Compin' Man | C | C035 | Rhythmic Pattern | D1 p11 |  |
| F01-C0293 | Inhabited World | C | C036 | MotionSynth | D1 p11 |  |
| F01-C0294 | Analog Punch | C | C037 | Synth | D1 p11 |  |
| F01-C0295 | Speed Racer | C | C038 | Ds/Hits | D1 p11 |  |
| F01-C0296 | Warm&Fuzzy Gtr | C | C039 | Guitar/Plucked | D1 p11 |  |
| F01-C0297 | JazzOrgan/Split | C | C040 | Organ | D1 p11 |  |
| F01-C0298 | VoicyPianoLayer | C | C041 | Keyboard | D1 p11 |  |
| F01-C0299 | TaiChi Dawn | C | C042 | World | D1 p11 |  |
| F01-C0300 | Brass n' Saxes | C | C043 | BrassReed | D1 p11 |  |
| F01-C0301 | -Miracle Bells- | C | C044 | Bell/Mallet/Perc | D1 p11 |  |
| F01-C0302 | SmoothOctaveStr | C | C045 | Strings | D1 p11 |  |
| F01-C0303 | Moon's Glow | C | C046 | LeadSplits | D1 p11 |  |
| F01-C0304 | M A T R I X | C | C047 | Complex & SE | D1 p11 |  |
| F01-C0305 | Clav.usFunkus | C | C048 | Keyboard | D1 p11 |  |
| F01-C0306 | Soft Split-Pad | C | C049 | BassSplits | D1 p11 |  |
| F01-C0307 | *Warm Orchestra* | C | C050 | Orchestral | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0308 | Velo Bass Stack | C | C051 | Rhythmic Pattern | D1 p11 |  |
| F01-C0309 | Gone Forever | C | C052 | Bell/Mallet/Perc | D1 p11 |  |
| F01-C0310 | Big and Buzzy | C | C053 | Synth | D1 p11 |  |
| F01-C0311 | Stratosphere | C | C054 | Rhythmic Pattern | D1 p11 |  |
| F01-C0312 | E.Grand Bell Pad | C | C055 | Keyboard | D1 p11 |  |
| F01-C0313 | OrganVel->Brass | C | C056 | Organ | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0314 | Highland Tines | C | C057 | Keyboard | D1 p11 |  |
| F01-C0315 | Triton Voices | C | C058 | Pads | D1 p11 |  |
| F01-C0316 | Muted BigBand | C | C059 | BrassReed | D1 p11 |  |
| F01-C0317 | Isle Of Indigo | C | C060 | Bell/Mallet/Perc | D1 p11 |  |
| F01-C0318 | * Epic Cinema * | C | C061 | MotionSynth | D1 p11 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0319 | TV Detectv Drive | C | C062 | LeadSplits | D1 p11 |  |
| F01-C0320 | AimForTheMonster | C | C063 | Complex & SE | D1 p11 |  |
| F01-C0321 | Ballad Layer EP | C | C064 | Keyboard | D1 p12 |  |
| F01-C0322 | Hybrid Split | C | C065 | BassSplits | D1 p12 |  |
| F01-C0323 | Piano Concerto | C | C066 | Orchestral | D1 p12 |  |
| F01-C0324 | Happy Feet !! | C | C067 | Rhythmic Pattern | D1 p12 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0325 | Panner-Emma | C | C068 | MotionSynth | D1 p12 |  |
| F01-C0326 | Analog 101 | C | C069 | Synth | D1 p12 |  |
| F01-C0327 | Re-Mix-Maven | C | C070 | Ds/Hits | D1 p12 |  |
| F01-C0328 | Hybrid Gtr Pad | C | C071 | Guitar/Plucked | D1 p12 |  |
| F01-C0329 | Vital Organ | C | C072 | Organ | D1 p12 |  |
| F01-C0330 | BellyPianoLayer | C | C073 | Keyboard | D1 p12 |  |
| F01-C0331 | Good Luck EP Pad | C | C074 | Keyboard | D1 p12 |  |
| F01-C0332 | Saxobrass | C | C075 | BrassReed | D1 p12 |  |
| F01-C0333 | Triple Bell | C | C076 | Bell/Mallet/Perc | D1 p12 |  |
| F01-C0334 | Dirty Robbers | C | C077 | Rhythmic Pattern | D1 p12 |  |
| F01-C0335 | Alien Lovers | C | C078 | LeadSplits | D1 p12 |  |
| F01-C0336 | ... Zero G's ... | C | C079 | Complex & SE | D1 p12 |  |
| F01-C0337 | 'round the Piano | C | C080 | Keyboard | D1 p12 |  |
| F01-C0338 | Stab-In-A-Stab | C | C081 | Synth | D1 p12 |  |
| F01-C0339 | Poly Sixual | C | C082 | Synth | D1 p12 |  |
| F01-C0340 | korg@groove.comp | C | C083 | Rhythmic Pattern | D1 p12 |  |
| F01-C0341 | Ice Castle | C | C084 | MotionSynth | D1 p12 |  |
| F01-C0342 | Analog SquareWah | C | C085 | LeadSplits | D1 p12 |  |
| F01-C0343 | Something Wicked | C | C086 | Rhythmic Pattern | D1 p12 |  |
| F01-C0344 | Layered M1 Piano | C | C087 | Keyboard | D1 p12 |  |
| F01-C0345 | ReggaeAlvinBlues | C | C088 | Organ | D1 p12 |  |
| F01-C0346 | Clicky E.Piano | C | C089 | Keyboard | D1 p12 |  |
| F01-C0347 | Clean Aire | C | C090 | Pads | D1 p12 |  |
| F01-C0348 | Aggessive Horns | C | C091 | BrassReed | D1 p12 |  |
| F01-C0349 | Venus Percs | C | C092 | Bell/Mallet/Perc | D1 p12 |  |
| F01-C0350 | Solo & Ensemble | C | C093 | Strings | D1 p12 |  |
| F01-C0351 | Hold Me... EP | C | C094 | Keyboard | D1 p12 |  |
| F01-C0352 | TrancenDance | C | C095 | Complex & SE | D1 p12 |  |
| F01-C0353 | Wurly Dream EP | C | C096 | Keyboard | D1 p13 |  |
| F01-C0354 | Bumpin'&Thumpin' | C | C097 | BassSplits | D1 p13 |  |
| F01-C0355 | The Ascension | C | C098 | Synth | D1 p13 |  |
| F01-C0356 | Bad*Street Split | C | C099 | Guitar/Plucked | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0357 | Nebular Motion | C | C100 | MotionSynth | D1 p13 |  |
| F01-C0358 | Soft Blip*Synth | C | C101 | Synth | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0359 | "...MY toys!" | C | C102 | Ds/Hits | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0360 | Toro,Toro,Ole!!! | C | C103 | Guitar/Plucked | D1 p13 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0361 | Positive@SW&Knbs | C | C104 | Organ | D1 p13 |  |
| F01-C0362 | World Atmosphere | C | C105 | World | D1 p13 |  |
| F01-C0363 | Powerful Throats | C | C106 | Pads | D1 p13 |  |
| F01-C0364 | Warm Low Brass | C | C107 | BrassReed | D1 p13 |  |
| F01-C0365 | Bell Animations | C | C108 | Bell/Mallet/Perc | D1 p13 |  |
| F01-C0366 | Burble Swirl | C | C109 | Synth | D1 p13 |  |
| F01-C0367 | E.Pianic Layers | C | C110 | Keyboard | D1 p13 |  |
| F01-C0368 | Digital Diver | C | C111 | Complex & SE | D1 p13 |  |
| F01-C0369 | Blaster Keys | C | C112 | Keyboard | D1 p13 |  |
| F01-C0370 | Square Roots | C | C113 | Synth | D1 p13 |  |
| F01-C0371 | Warm Bars | C | C114 | Organ | D1 p13 |  |
| F01-C0372 | Snowboard Hop | C | C115 | Rhythmic Pattern | D1 p13 |  |
| F01-C0373 | Shh-I'm Sweeping | C | C116 | MotionSynth | D1 p13 |  |
| F01-C0374 | Mir Appears | C | C117 | Synth | D1 p13 |  |
| F01-C0375 | SYNCing Feeling | C | C118 | Synth | D1 p13 |  |
| F01-C0376 | Complex EP Grand | C | C119 | Keyboard | D1 p13 |  |
| F01-C0377 | Organ Principle | C | C120 | Organ | D1 p13 |  |
| F01-C0378 | Manhattan Chase | C | C121 | Rhythmic Pattern | D1 p13 |  |
| F01-C0379 | Musical Clowns | C | C122 | World | D1 p13 |  |
| F01-C0380 | Mustang Saxes | C | C123 | BrassReed | D1 p13 |  |
| F01-C0381 | Belle | C | C124 | Bell/Mallet/Perc | D1 p13 |  |
| F01-C0382 | Digimidgets | C | C125 | Pads | D1 p13 |  |
| F01-C0383 | Vacancy... | C | C126 | Rhythmic Pattern | D1 p13 |  |
| F01-C0384 | PerihelionSweep | C | C127 | Complex & SE | D1 p13 |  |
| F01-C0385 | Ballad Grand | D | D000 | Keyboard | D1 p14 |  |
| F01-C0386 | R&B Lead Split | D | D001 | BassSplits | D1 p14 |  |
| F01-C0387 | Pompus Brass | D | D002 | Orchestral | D1 p14 |  |
| F01-C0388 | Mr Happy Arp ! | D | D003 | Rhythmic Pattern | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0389 | Phases Of Angels | D | D004 | MotionSynth | D1 p14 |  |
| F01-C0390 | Syzzle Synth | D | D005 | Synth | D1 p14 |  |
| F01-C0391 | Sonic FX Mixer | D | D006 | Ds/Hits | D1 p14 |  |
| F01-C0392 | The Guitarist | D | D007 | Guitar/Plucked | D1 p14 |  |
| F01-C0393 | Straight Wheels | D | D008 | Organ | D1 p14 |  |
| F01-C0394 | Marching Pipes | D | D009 | World | D1 p14 |  |
| F01-C0395 | Pearly-Gates Gig | D | D010 | Pads | D1 p14 |  |
| F01-C0396 | French Brass | D | D011 | BrassReed | D1 p14 |  |
| F01-C0397 | Modern Monastery | D | D012 | Bell/Mallet/Perc | D1 p14 |  |
| F01-C0398 | String Concerto | D | D013 | Strings | D1 p14 |  |
| F01-C0399 | Leading to Peace | D | D014 | LeadSplits | D1 p14 |  |
| F01-C0400 | Alien Translator | D | D015 | Complex & SE | D1 p14 |  |
| F01-C0401 | GlassyEP&Strings | D | D016 | Keyboard | D1 p14 |  |
| F01-C0402 | Stationary Split | D | D017 | BassSplits | D1 p14 |  |
| F01-C0403 | Epic Orchestra | D | D018 | Orchestral | D1 p14 |  |
| F01-C0404 | Prog Rock Fest | D | D019 | Rhythmic Pattern | D1 p14 |  |
| F01-C0405 | Metamorphoses | D | D020 | MotionSynth | D1 p14 |  |
| F01-C0406 | Ravalogue | D | D021 | Synth | D1 p14 |  |
| F01-C0407 | Carnival Parade | D | D022 | Ds/Hits | D1 p14 |  |
| F01-C0408 | Pastoral Guitar | D | D023 | Guitar/Plucked | D1 p14 |  |
| F01-C0409 | <Standard Bars> | D | D024 | Organ | D1 p14 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0410 | Shogun | D | D025 | World | D1 p14 |  |
| F01-C0411 | Symphonic Voices | D | D026 | Pads | D1 p14 |  |
| F01-C0412 | Small Sax Ens | D | D027 | BrassReed | D1 p14 |  |
| F01-C0413 | Lincoln Logs | D | D028 | Bell/Mallet/Perc | D1 p14 |  |
| F01-C0414 | Violin Sect. | D | D029 | Strings | D1 p14 |  |
| F01-C0415 | Bass-n-Bellz | D | D030 | BassSplits | D1 p14 |  |
| F01-C0416 | Nunavut Lights | D | D031 | MotionSynth | D1 p14 |  |
| F01-C0417 | Breathy Bell EP | D | D032 | Keyboard | D1 p15 |  |
| F01-C0418 | Hot Tub Split | D | D033 | BassSplits | D1 p15 |  |
| F01-C0419 | Velo Orch Winds | D | D034 | Orchestral | D1 p15 |  |
| F01-C0420 | The Organ Groove | D | D035 | Rhythmic Pattern | D1 p15 |  |
| F01-C0421 | Respirator | D | D036 | MotionSynth | D1 p15 |  |
| F01-C0422 | TalkBoxBass<Rbn> | D | D037 | Synth | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0423 | Cyborg Scratch | D | D038 | Ds/Hits | D1 p15 |  |
| F01-C0424 | Rich Guitarist | D | D039 | Guitar/Plucked | D1 p15 |  |
| F01-C0425 | Jazz Overdrive | D | D040 | Organ | D1 p15 |  |
| F01-C0426 | Accordion (SW) 1 | D | D041 | World | D1 p15 |  |
| F01-C0427 | *Dying Star* | D | D042 | Pads | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0428 | PowrBrassSession | D | D043 | BrassReed | D1 p15 |  |
| F01-C0429 | *)BarbieBellz(* | D | D044 | Bell/Mallet/Perc | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0430 | 3-Octave Strings | D | D045 | Strings | D1 p15 |  |
| F01-C0431 | Splatsburg | D | D046 | LeadSplits | D1 p15 |  |
| F01-C0432 | Deadly Gasses | D | D047 | Complex & SE | D1 p15 |  |
| F01-C0433 | Village Pump | D | D048 | Keyboard | D1 p15 |  |
| F01-C0434 | Old Friends | D | D049 | BassSplits | D1 p15 |  |
| F01-C0435 | Jet Stream | D | D050 | MotionSynth | D1 p15 |  |
| F01-C0436 | RibbonREZ JAM!! | D | D051 | Rhythmic Pattern | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0437 | Spheoretical | D | D052 | MotionSynth | D1 p15 |  |
| F01-C0438 | DiscoLivesHERE!! | D | D053 | Synth | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0439 | Streetwalk | D | D054 | Rhythmic Pattern | D1 p15 |  |
| F01-C0440 | Heaven's Bells | D | D055 | Bell/Mallet/Perc | D1 p15 |  |
| F01-C0441 | Jazz A Bee | D | D056 | Organ | D1 p15 |  |
| F01-C0442 | October Fest | D | D057 | World | D1 p15 |  |
| F01-C0443 | Pulsing 5 Pad | D | D058 | Pads | D1 p15 |  |
| F01-C0444 | Technified | D | D059 | Rhythmic Pattern | D1 p15 |  |
| F01-C0445 | AIRY Bellz | D | D060 | Bell/Mallet/Perc | D1 p15 |  |
| F01-C0446 | SyrupySynthStrs | D | D061 | Strings | D1 p15 |  |
| F01-C0447 | Pad & Knob1 Lead | D | D062 | LeadSplits | D1 p15 |  |
| F01-C0448 | OpenPodBayDoor! | D | D063 | Complex & SE | D1 p15 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0449 | Heaven Layers | D | D064 | Keyboard | D1 p16 |  |
| F01-C0450 | Titanical Split | D | D065 | LeadSplits | D1 p16 |  |
| F01-C0451 | Dbl. Reed Gang | D | D066 | Orchestral | D1 p16 |  |
| F01-C0452 | Bass & LFO-er | D | D067 | Rhythmic Pattern | D1 p16 |  |
| F01-C0453 | Northern Stars | D | D068 | MotionSynth | D1 p16 |  |
| F01-C0454 | The Pitz | D | D069 | Synth | D1 p16 |  |
| F01-C0455 | 2Hand Beats&Bass | D | D070 | Ds/Hits | D1 p16 |  |
| F01-C0456 | *Slinky* | D | D071 | Rhythmic Pattern | D1 p16 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0457 | Lovely Old VOX | D | D072 | Organ | D1 p16 |  |
| F01-C0458 | Accordion (SW) 2 | D | D073 | World | D1 p16 |  |
| F01-C0459 | SecretGroove Pad | D | D074 | Pads | D1 p16 |  |
| F01-C0460 | Techno Fugue | D | D075 | Rhythmic Pattern | D1 p16 |  |
| F01-C0461 | Linear Synthesis | D | D076 | Bell/Mallet/Perc | D1 p16 |  |
| F01-C0462 | Antarctica | D | D077 | Pads | D1 p16 |  |
| F01-C0463 | Electric Pad | D | D078 | Pads | D1 p16 |  |
| F01-C0464 | Don'tLandTheShip | D | D079 | Complex & SE | D1 p16 |  |
| F01-C0465 | The Key Elements | D | D080 | Keyboard | D1 p16 |  |
| F01-C0466 | Jungle Split | D | D081 | BassSplits | D1 p16 |  |
| F01-C0467 | King Cobras | D | D082 | World | D1 p16 |  |
| F01-C0468 | Street Cats | D | D083 | Rhythmic Pattern | D1 p16 |  |
| F01-C0469 | Land Of Dreams | D | D084 | MotionSynth | D1 p16 |  |
| F01-C0470 | BellZarra 6/8 | D | D085 | Synth | D1 p16 |  |
| F01-C0471 | BIG Gnarly Lead | D | D086 | Synth | D1 p16 |  |
| F01-C0472 | Rhythm Mysters | D | D087 | Guitar/Plucked | D1 p16 |  |
| F01-C0473 | Glacier Pad | D | D088 | MotionSynth | D1 p16 |  |
| F01-C0474 | African Festival | D | D089 | World | D1 p16 |  |
| F01-C0475 | WideOpenSpaces | D | D090 | Pads | D1 p16 |  |
| F01-C0476 | Ghosttown Cornet | D | D091 | BrassReed | D1 p16 |  |
| F01-C0477 | ClockmakersDream | D | D092 | Bell/Mallet/Perc | D1 p16 |  |
| F01-C0478 | Serene Strings | D | D093 | Strings | D1 p16 |  |
| F01-C0479 | BigB.Sect.-Split | D | D094 | BassSplits | D1 p16 |  |
| F01-C0480 | PortaMenthos | D | D095 | Synth | D1 p16 |  |
| F01-C0481 | The Ballad EP | D | D096 | Keyboard | D1 p17 |  |
| F01-C0482 | Bass&RezKey<99> | D | D097 | BassSplits | D1 p17 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0483 | Harpsicord Suite | D | D098 | Orchestral | D1 p17 |  |
| F01-C0484 | Bass & Comp Jam | D | D099 | Rhythmic Pattern | D1 p17 |  |
| F01-C0485 | Juicy Reflection | D | D100 | MotionSynth | D1 p17 |  |
| F01-C0486 | BottlePowerPad | D | D101 | Synth | D1 p17 |  |
| F01-C0487 | Tranquil~Stream~ | D | D102 | BrassReed | D1 p17 |  |
| F01-C0488 | Steam Factory | D | D103 | Rhythmic Pattern | D1 p17 |  |
| F01-C0489 | Grand Procession | D | D104 | Organ | D1 p17 |  |
| F01-C0490 | Svengali Dusk | D | D105 | World | D1 p17 |  |
| F01-C0491 | Alpha Mega Synth | D | D106 | MotionSynth | D1 p17 |  |
| F01-C0492 | Smooth DynoBrass | D | D107 | BrassReed | D1 p17 |  |
| F01-C0493 | Dreamy*Lillies | D | D108 | MotionSynth | D1 p17 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0494 | BellAsian | D | D109 | World | D1 p17 |  |
| F01-C0495 | Sweetpad & LEADZ | D | D110 | LeadSplits | D1 p17 |  |
| F01-C0496 | Da Bomb Rap | D | D111 | Complex & SE | D1 p17 |  |
| F01-C0497 | E. Pierre | D | D112 | Keyboard | D1 p17 |  |
| F01-C0498 | Fat Analog Split | D | D113 | BassSplits | D1 p17 |  |
| F01-C0499 | PhunkyWahWahClav | D | D114 | Keyboard | D1 p17 |  |
| F01-C0500 | Crazy Wah Cafe | D | D115 | Rhythmic Pattern | D1 p17 |  |
| F01-C0501 | Virtual Journey | D | D116 | MotionSynth | D1 p17 |  |
| F01-C0502 | Jazzy BottleLead | D | D117 | Synth | D1 p17 |  |
| F01-C0503 | TalkBoxGtr <Rbn> | D | D118 | Guitar/Plucked | D1 p17 | name printed with marker characters; recorded exactly as the document prints it |
| F01-C0504 | Mandolin Strums | D | D119 | Guitar/Plucked | D1 p17 |  |
| F01-C0505 | Sunday Sermon | D | D120 | Organ | D1 p17 |  |
| F01-C0506 | Triton Bay | D | D121 | World | D1 p17 |  |
| F01-C0507 | Morning Doves | D | D122 | BrassReed | D1 p17 |  |
| F01-C0508 | Radiologizer | D | D123 | Complex & SE | D1 p17 |  |
| F01-C0509 | West Indian Way | D | D124 | Bell/Mallet/Perc | D1 p17 |  |
| F01-C0510 | Orchestr.Strings | D | D125 | Strings | D1 p17 |  |
| F01-C0511 | Any Clock'll Do | D | D126 | MotionSynth | D1 p17 |  |
| F01-C0512 | * User's Slot * | D | D127 | Keyboard | D1 p17 | document prints this row as a placeholder, not a factory sound; name printed with marker characters; recorded exactly as the document prints it |

## Programs

Type is the document's own single/double program flag. Bank G holds the GM bank; its `g(1)`-`g(9)` / `g(d)` rows are the document's own variant entries and are listed here exactly as printed.

| Shipped label | Documented name | Bank | Number | Type | Category | Source | Note |
|---|---|---|---|---|---|---|---|
| F01-P0001 | Noisy Stabber | A | A000 | double | FastSynth | D1 p18 |  |
| F01-P0002 | Ana Brass/Lead | A | A064 | double | FastSynth | D1 p18 |  |
| F01-P0003 | Acoustic Piano | A | A001 | single | Keyboard | D1 p18 |  |
| F01-P0004 | 90's Piano | A | A065 | double | Keyboard | D1 p18 |  |
| F01-P0005 | Chipper Dayglow | A | A002 | double | Hit/Arpg | D1 p18 |  |
| F01-P0006 | Wild Arp | A | A066 | double | Hit/Arpg | D1 p18 |  |
| F01-P0007 | Legato Strings | A | A003 | double | Strings | D1 p18 |  |
| F01-P0008 | Stereo Strings | A | A067 | double | Strings | D1 p18 |  |
| F01-P0009 | ! {Tricky} Kit ! | A | A004 | double | Drums | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0010 | HipHop Kit | A | A068 | double | Drums | D1 p18 |  |
| F01-P0011 | Acoustic Guitar | A | A005 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0012 | Jazz Guitar | A | A069 | single | Guitar/Plucked | D1 p18 |  |
| F01-P0013 | Nasty Bass | A | A006 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0014 | Euro 8va Bass | A | A070 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0015 | BX3 Vel Switch | A | A007 | double | Organ | D1 p18 |  |
| F01-P0016 | Distortion Organ | A | A071 | double | Organ | D1 p18 |  |
| F01-P0017 | Rez. Down | A | A008 | double | SlowSynth | D1 p18 |  |
| F01-P0018 | Tsunami Waves | A | A072 | double | SlowSynth | D1 p18 |  |
| F01-P0019 | Fat Brass | A | A009 | double | Brass | D1 p18 |  |
| F01-P0020 | Glen & The Boys | A | A073 | double | Brass | D1 p18 |  |
| F01-P0021 | <Techno Vox Box> | A | A010 | double | SE | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0022 | Steam Sweeps | A | A074 | double | SE | D1 p18 |  |
| F01-P0023 | Fresh Breath | A | A011 | double | Vocal/Airy | D1 p18 |  |
| F01-P0024 | White Pad EP | A | A075 | double | Keyboard | D1 p18 |  |
| F01-P0025 | Smooth Sine Lead | A | A012 | double | LeadSynth | D1 p18 |  |
| F01-P0026 | Old & Analog | A | A076 | double | LeadSynth | D1 p18 |  |
| F01-P0027 | Sax Ensemble | A | A013 | double | Woodwind/Reed | D1 p18 |  |
| F01-P0028 | DoubleReed | A | A077 | single | Woodwind/Reed | D1 p18 |  |
| F01-P0029 | Swirling Dreams | A | A014 | double | MotionSynth | D1 p18 |  |
| F01-P0030 | Aqua Phonics | A | A078 | double | MotionSynth | D1 p18 |  |
| F01-P0031 | Monkey Skulls | A | A015 | double | Bell/Mallet | D1 p18 |  |
| F01-P0032 | Ensemble Bell | A | A079 | single | Bell/Mallet | D1 p18 |  |
| F01-P0033 | Metalic Rez | A | A016 | double | FastSynth | D1 p18 |  |
| F01-P0034 | Cosmic Furnace | A | A080 | double | FastSynth | D1 p18 |  |
| F01-P0035 | Studio Stage EP | A | A017 | double | Keyboard | D1 p18 |  |
| F01-P0036 | Velo Whirly | A | A081 | double | Keyboard | D1 p18 |  |
| F01-P0037 | Flip Blip | A | A018 | double | Hit/Arpg | D1 p18 |  |
| F01-P0038 | Techno Organ Hit | A | A082 | double | Hit/Arpg | D1 p18 |  |
| F01-P0039 | Camera Strings | A | A019 | double | Strings | D1 p18 |  |
| F01-P0040 | Few Bows Here | A | A083 | double | Strings | D1 p18 |  |
| F01-P0041 | House Kit | A | A020 | double | Drums | D1 p18 |  |
| F01-P0042 | Mega Drum Hit | A | A084 | double | Drums | D1 p18 |  |
| F01-P0043 | Chorus E.Guitar | A | A021 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0044 | Mute Monster | A | A085 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0045 | Acoustic Bass | A | A022 | single | Bass/Synth Bass | D1 p18 |  |
| F01-P0046 | Stein Bass | A | A086 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0047 | M1 Organ | A | A023 | double | Organ | D1 p18 |  |
| F01-P0048 | Perc Short Decay | A | A087 | double | Organ | D1 p18 |  |
| F01-P0049 | Digi Ice Pad | A | A024 | double | SlowSynth | D1 p18 |  |
| F01-P0050 | Antartic Wind | A | A088 | double | SlowSynth | D1 p18 |  |
| F01-P0051 | Trumpet | A | A025 | single | Brass | D1 p18 |  |
| F01-P0052 | Trombone Hard | A | A089 | single | Brass | D1 p18 |  |
| F01-P0053 | Sonic Blast | A | A026 | double | SE | D1 p18 |  |
| F01-P0054 | Orbiting Probes | A | A090 | double | SE | D1 p18 |  |
| F01-P0055 | Choir of Light | A | A027 | double | Vocal/Airy | D1 p18 |  |
| F01-P0056 | Phantom Of Tine | A | A091 | double | Keyboard | D1 p18 |  |
| F01-P0057 | Rezbo | A | A028 | double | FastSynth | D1 p18 |  |
| F01-P0058 | Seq DDL Lead | A | A092 | double | LeadSynth | D1 p18 |  |
| F01-P0059 | TenorSax Growl-Y | A | A029 | double | Woodwind/Reed | D1 p18 |  |
| F01-P0060 | BariSax Growl -Y | A | A093 | double | Woodwind/Reed | D1 p18 |  |
| F01-P0061 | Tinklin' Pad | A | A030 | double | MotionSynth | D1 p18 |  |
| F01-P0062 | Percussive Hits | A | A094 | double | Hit/Arpg | D1 p18 |  |
| F01-P0063 | VS Bell Boy | A | A031 | double | Bell/Mallet | D1 p18 |  |
| F01-P0064 | Vibraphone | A | A095 | double | Bell/Mallet | D1 p18 |  |
| F01-P0065 | Power Snap Synth | A | A032 | double | FastSynth | D1 p18 |  |
| F01-P0066 | Digital PolySix | A | A096 | double | FastSynth | D1 p18 |  |
| F01-P0067 | Romance Piano | A | A033 | double | Keyboard | D1 p18 |  |
| F01-P0068 | Clav | A | A097 | single | Keyboard | D1 p18 |  |
| F01-P0069 | Arp Angeles | A | A034 | double | Hit/Arpg | D1 p18 |  |
| F01-P0070 | Goa Lover | A | A098 | double | SE | D1 p18 |  |
| F01-P0071 | Arco Strings | A | A035 | double | Strings | D1 p18 |  |
| F01-P0072 | AnalogStrings1&2 | A | A099 | double | Strings | D1 p18 |  |
| F01-P0073 | Standard Kit | A | A036 | double | Drums | D1 p18 |  |
| F01-P0074 | BD&SD Kit 1 | A | A100 | double | Drums | D1 p18 |  |
| F01-P0075 | Wet Dist. Guitar | A | A037 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0076 | Nylon Guitar | A | A101 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0077 | 30303 Mega Bass | A | A038 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0078 | Jungle Rez Bass | A | A102 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0079 | Dark Jazz-Organ | A | A039 | double | Organ | D1 p18 |  |
| F01-P0080 | Full Drawbars | A | A103 | double | Organ | D1 p18 |  |
| F01-P0081 | Clouds of Air | A | A040 | double | SlowSynth | D1 p18 |  |
| F01-P0082 | Money Pad | A | A104 | double | SlowSynth | D1 p18 |  |
| F01-P0083 | SFZ Brass ST | A | A041 | double | Brass | D1 p18 |  |
| F01-P0084 | Burnin' Brass | A | A105 | double | Brass | D1 p18 |  |
| F01-P0085 | Loop-Iteria | A | A042 | double | SE | D1 p18 |  |
| F01-P0086 | VenusianStories | A | A106 | double | SE | D1 p18 |  |
| F01-P0087 | Oooh Voices ST | A | A043 | double | Vocal/Airy | D1 p18 |  |
| F01-P0088 | SynPiano X | A | A107 | single | Keyboard | D1 p18 |  |
| F01-P0089 | HipHop Lead | A | A044 | double | LeadSynth | D1 p18 |  |
| F01-P0090 | Syncro City | A | A108 | double | LeadSynth | D1 p18 |  |
| F01-P0091 | Flute | A | A045 | single | Woodwind/Reed | D1 p18 |  |
| F01-P0092 | Recorder | A | A109 | single | Woodwind/Reed | D1 p18 |  |
| F01-P0093 | Sinistar*Android | A | A046 | double | MotionSynth | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0094 | Astral Dreams | A | A110 | double | MotionSynth | D1 p18 |  |
| F01-P0095 | Log Drum | A | A047 | double | Bell/Mallet | D1 p18 |  |
| F01-P0096 | Bali Gamelon | A | A111 | double | Bell/Mallet | D1 p18 |  |
| F01-P0097 | Rez. Sweep | A | A048 | double | FastSynth | D1 p18 |  |
| F01-P0098 | Super Saw Brass | A | A112 | double | FastSynth | D1 p18 |  |
| F01-P0099 | R&B E.Piano | A | A049 | double | Keyboard | D1 p18 |  |
| F01-P0100 | Sweeping EP | A | A113 | double | Keyboard | D1 p18 |  |
| F01-P0101 | Arp Twins | A | A050 | double | Hit/Arpg | D1 p18 |  |
| F01-P0102 | Jungle Melody | A | A114 | double | Hit/Arpg | D1 p18 |  |
| F01-P0103 | PizzAnsamble | A | A051 | double | Strings | D1 p18 |  |
| F01-P0104 | WatcherOfTheSky | A | A115 | double | Strings | D1 p18 |  |
| F01-P0105 | Psycho Kit | A | A052 | double | Drums | D1 p18 |  |
| F01-P0106 | Orchestra&Ethnic | A | A116 | double | Drums | D1 p18 |  |
| F01-P0107 | Vox Wah Chicks | A | A053 | single | Guitar/Plucked | D1 p18 |  |
| F01-P0108 | Power-Chords&FX | A | A117 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0109 | SuperSwitch Bass | A | A054 | double | Bass/Synth Bass | D1 p18 |  |
| F01-P0110 | E.Bass Finger | A | A118 | single | Bass/Synth Bass | D1 p18 |  |
| F01-P0111 | Jazz Organ 1 | A | A055 | double | Organ | D1 p18 |  |
| F01-P0112 | Pipe Mixture | A | A119 | double | Organ | D1 p18 |  |
| F01-P0113 | Ravelian Pad | A | A056 | double | SlowSynth | D1 p18 |  |
| F01-P0114 | {(Meditate}) | A | A120 | double | SlowSynth | D1 p18 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0115 | Classic F.Horn | A | A057 | double | Brass | D1 p18 |  |
| F01-P0116 | Tight Brass | A | A121 | double | Brass | D1 p18 |  |
| F01-P0117 | Monster Island | A | A058 | double | SE | D1 p18 |  |
| F01-P0118 | Soundscapes | A | A122 | double | SE | D1 p18 |  |
| F01-P0119 | Aaah Voices ST | A | A059 | double | Vocal/Airy | D1 p18 |  |
| F01-P0120 | Sitar Sitar | A | A123 | double | Guitar/Plucked | D1 p18 |  |
| F01-P0121 | Trancer Lead | A | A060 | double | LeadSynth | D1 p18 |  |
| F01-P0122 | Espress Lead | A | A124 | double | LeadSynth | D1 p18 |  |
| F01-P0123 | AltoSax2Breath-Y | A | A061 | double | Woodwind/Reed | D1 p18 |  |
| F01-P0124 | SopranoSax Br.-Y | A | A125 | double | Woodwind/Reed | D1 p18 |  |
| F01-P0125 | Crimson 5ths | A | A062 | double | MotionSynth | D1 p18 |  |
| F01-P0126 | Flying Machines | A | A126 | double | MotionSynth | D1 p18 |  |
| F01-P0127 | Moving Bellz | A | A063 | double | Bell/Mallet | D1 p18 |  |
| F01-P0128 | Krystal Bells | A | A127 | double | Bell/Mallet | D1 p18 |  |
| F01-P0129 | Synth Sweeper | B | B000 | double | FastSynth | D1 p19 |  |
| F01-P0130 | SynthBrass | B | B064 | double | FastSynth | D1 p19 |  |
| F01-P0131 | Attack Piano | B | B001 | double | Keyboard | D1 p19 |  |
| F01-P0132 | Classic Piano | B | B065 | single | Keyboard | D1 p19 |  |
| F01-P0133 | StaccatoPizzHit | B | B002 | double | Hit/Arpg | D1 p19 |  |
| F01-P0134 | Big Hit in India | B | B066 | double | Hit/Arpg | D1 p19 |  |
| F01-P0135 | Octave Strings | B | B003 | double | Strings | D1 p19 |  |
| F01-P0136 | Solo Violin | B | B067 | single | Strings | D1 p19 |  |
| F01-P0137 | Jazz/Brush Kits | B | B004 | double | Drums | D1 p19 |  |
| F01-P0138 | Drum'n'Bass Kit | B | B068 | double | Drums | D1 p19 |  |
| F01-P0139 | FingertipsGuitar | B | B005 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0140 | Strato-Chime | B | B069 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0141 | Dark R&B Bass | B | B006 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0142 | Digi Syn Bass | B | B070 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0143 | Old Tone-Wheel | B | B007 | double | Organ | D1 p19 |  |
| F01-P0144 | Classic Click | B | B071 | double | Organ | D1 p19 |  |
| F01-P0145 | The Pad | B | B008 | double | SlowSynth | D1 p19 |  |
| F01-P0146 | DJ Touch | B | B072 | double | SE | D1 p19 |  |
| F01-P0147 | Brass Expression | B | B009 | double | Brass | D1 p19 |  |
| F01-P0148 | Big Band Plunger | B | B073 | double | Brass | D1 p19 |  |
| F01-P0149 | PiezoMix Guitar | B | B010 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0150 | New Voyage | B | B074 | double | SE | D1 p19 |  |
| F01-P0151 | Slow Choir ST | B | B011 | double | Vocal/Airy | D1 p19 |  |
| F01-P0152 | Arctic Voices | B | B075 | double | Vocal/Airy | D1 p19 |  |
| F01-P0153 | Phat Saw Lead | B | B012 | double | LeadSynth | D1 p19 |  |
| F01-P0154 | Goa Message | B | B076 | double | Hit/Arpg | D1 p19 |  |
| F01-P0155 | Old Shakuhachi | B | B013 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0156 | English Horn | B | B077 | single | Woodwind/Reed | D1 p19 |  |
| F01-P0157 | OXYGEN | B | B014 | double | MotionSynth | D1 p19 |  |
| F01-P0158 | E.Bass Pick 2 | B | B078 | single | Bass/Synth Bass | D1 p19 |  |
| F01-P0159 | Velo Kalimba | B | B015 | double | Bell/Mallet | D1 p19 |  |
| F01-P0160 | Tropico Bells | B | B079 | double | Bell/Mallet | D1 p19 |  |
| F01-P0161 | Future Syn Pad | B | B016 | double | FastSynth | D1 p19 |  |
| F01-P0162 | Mega Big Synth | B | B080 | double | FastSynth | D1 p19 |  |
| F01-P0163 | Dyno Tine EP | B | B017 | double | Keyboard | D1 p19 |  |
| F01-P0164 | Vintage EP | B | B081 | double | Keyboard | D1 p19 |  |
| F01-P0165 | Brass Impact Hit | B | B018 | double | Hit/Arpg | D1 p19 |  |
| F01-P0166 | Space Pod for 2 | B | B082 | double | MotionSynth | D1 p19 |  |
| F01-P0167 | String Quartet | B | B019 | single | Strings | D1 p19 |  |
| F01-P0168 | Indian Stars | B | B083 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0169 | Processed Kit | B | B020 | double | Drums | D1 p19 |  |
| F01-P0170 | Dragon Gong | B | B084 | double | Drums | D1 p19 |  |
| F01-P0171 | Vintage Stratt | B | B021 | single | Guitar/Plucked | D1 p19 |  |
| F01-P0172 | Feedback D. Gtr. | B | B085 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0173 | Fretless Bass | B | B022 | single | Bass/Synth Bass | D1 p19 |  |
| F01-P0174 | Sweet Fretless | B | B086 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0175 | Tekno Organ*Bass | B | B023 | double | Bass/Synth Bass | D1 p19 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0176 | Percussion BX3 | B | B087 | double | Organ | D1 p19 |  |
| F01-P0177 | Brass Pad | B | B024 | double | SlowSynth | D1 p19 |  |
| F01-P0178 | Dark Night | B | B088 | double | SE | D1 p19 |  |
| F01-P0179 | Muted Trumpet | B | B025 | single | Brass | D1 p19 |  |
| F01-P0180 | Trombone Ens. | B | B089 | double | Brass | D1 p19 |  |
| F01-P0181 | Fisa Americana | B | B026 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0182 | Marc Tree | B | B090 | double | Drums | D1 p19 |  |
| F01-P0183 | Vocalesque | B | B027 | double | Vocal/Airy | D1 p19 |  |
| F01-P0184 | Take Voices | B | B091 | double | Vocal/Airy | D1 p19 |  |
| F01-P0185 | Glide Lead | B | B028 | double | LeadSynth | D1 p19 |  |
| F01-P0186 | Fisa Cassotto | B | B092 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0187 | TenorSax Brth.-Y | B | B029 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0188 | Bassoon | B | B093 | single | Woodwind/Reed | D1 p19 |  |
| F01-P0189 | Transformation | B | B030 | double | MotionSynth | D1 p19 |  |
| F01-P0190 | PedalSteelGuitar | B | B094 | single | Guitar/Plucked | D1 p19 |  |
| F01-P0191 | Thin Bell-s-park | B | B031 | single | Bell/Mallet | D1 p19 |  |
| F01-P0192 | Koto | B | B095 | single | Guitar/Plucked | D1 p19 |  |
| F01-P0193 | Pop Synth Pad | B | B032 | double | FastSynth | D1 p19 |  |
| F01-P0194 | Rezzo Release | B | B096 | double | MotionSynth | D1 p19 |  |
| F01-P0195 | Piano Pad | B | B033 | double | Keyboard | D1 p19 |  |
| F01-P0196 | Sticky Rez Clav | B | B097 | double | Keyboard | D1 p19 |  |
| F01-P0197 | Karma Sutra | B | B034 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0198 | Hit Me 2 Times!! | B | B098 | double | Hit/Arpg | D1 p19 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0199 | Symphonic Bows | B | B035 | double | Strings | D1 p19 |  |
| F01-P0200 | Santur | B | B099 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0201 | Standard Kit 2 | B | B036 | double | Drums | D1 p19 |  |
| F01-P0202 | BD&SD Kit 2 | B | B100 | double | Drums | D1 p19 |  |
| F01-P0203 | Amp D. Guitar | B | B037 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0204 | Old 12-String | B | B101 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0205 | 30303 Square | B | B038 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0206 | Ramp Jungle Bass | B | B102 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0207 | Sine DWGS-Organ | B | B039 | double | Organ | D1 p19 |  |
| F01-P0208 | Killer B | B | B103 | double | Organ | D1 p19 |  |
| F01-P0209 | Simple Sine Pad | B | B040 | double | SlowSynth | D1 p19 |  |
| F01-P0210 | L/R Piano(Knob1) | B | B104 | double | Keyboard | D1 p19 |  |
| F01-P0211 | Octave Brass Exp | B | B041 | double | Brass | D1 p19 |  |
| F01-P0212 | Trumpet Ens. | B | B105 | double | Brass | D1 p19 |  |
| F01-P0213 | CyborgFactoryHit | B | B042 | double | SE | D1 p19 |  |
| F01-P0214 | MotionSoundTrack | B | B106 | double | SE | D1 p19 |  |
| F01-P0215 | E.G. Harmonics | B | B043 | single | Guitar/Plucked | D1 p19 |  |
| F01-P0216 | Processed E.Gtr. | B | B107 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0217 | Auto Pilot | B | B044 | single | Hit/Arpg | D1 p19 |  |
| F01-P0218 | Fat Syn Sync | B | B108 | double | LeadSynth | D1 p19 |  |
| F01-P0219 | BambuSilverFlute | B | B045 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0220 | Spitz Bottle | B | B109 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0221 | Dolphin Ride | B | B046 | double | MotionSynth | D1 p19 |  |
| F01-P0222 | MixFisaMaster 3 | B | B110 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0223 | Marimba | B | B047 | single | Bell/Mallet | D1 p19 |  |
| F01-P0224 | Future Bell | B | B111 | double | Bell/Mallet | D1 p19 |  |
| F01-P0225 | Gliding Squares | B | B048 | double | FastSynth | D1 p19 |  |
| F01-P0226 | Flute Pad | B | B112 | double | FastSynth | D1 p19 |  |
| F01-P0227 | Night Tines EP | B | B049 | double | Keyboard | D1 p19 |  |
| F01-P0228 | Hybrid Digi EP | B | B113 | double | Keyboard | D1 p19 |  |
| F01-P0229 | Mondo'Rimba | B | B050 | double | Bell/Mallet | D1 p19 |  |
| F01-P0230 | Harp | B | B114 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0231 | HybridStrg/Choir | B | B051 | double | Strings | D1 p19 |  |
| F01-P0232 | Harmonica | B | B115 | single | Woodwind/Reed | D1 p19 |  |
| F01-P0233 | Cymbals Kit | B | B052 | double | Drums | D1 p19 |  |
| F01-P0234 | Percussion Kit | B | B116 | double | Drums | D1 p19 |  |
| F01-P0235 | Funkin' Guitar | B | B053 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0236 | Dynamic E.Guitar | B | B117 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0237 | Poinker Bass | B | B054 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0238 | Decayed Bass | B | B118 | double | Bass/Synth Bass | D1 p19 |  |
| F01-P0239 | Gospel Organ | B | B055 | double | Organ | D1 p19 |  |
| F01-P0240 | Church Pipes | B | B119 | double | Organ | D1 p19 |  |
| F01-P0241 | HarpsyKorg 8'+4' | B | B056 | double | Keyboard | D1 p19 |  |
| F01-P0242 | Digital Bells | B | B120 | double | Bell/Mallet | D1 p19 |  |
| F01-P0243 | Horns & Ensemble | B | B057 | double | Brass | D1 p19 |  |
| F01-P0244 | Classic Fanfare | B | B121 | double | Brass | D1 p19 |  |
| F01-P0245 | The Great Wall | B | B058 | double | SE | D1 p19 |  |
| F01-P0246 | Hemispheres | B | B122 | double | SE | D1 p19 |  |
| F01-P0247 | Full Vox Pad | B | B059 | double | Vocal/Airy | D1 p19 |  |
| F01-P0248 | Indian Frets Gtr | B | B123 | double | Guitar/Plucked | D1 p19 |  |
| F01-P0249 | Musette | B | B060 | single | Woodwind/Reed | D1 p19 |  |
| F01-P0250 | Film Brass | B | B124 | double | Brass | D1 p19 |  |
| F01-P0251 | AltoSax1 Growl-Y | B | B061 | double | Woodwind/Reed | D1 p19 |  |
| F01-P0252 | Jazz Clarinet | B | B125 | single | Woodwind/Reed | D1 p19 |  |
| F01-P0253 | E.Bass Pick 1 | B | B062 | single | Bass/Synth Bass | D1 p19 |  |
| F01-P0254 | Pro-Dyno EP | B | B126 | double | Keyboard | D1 p19 |  |
| F01-P0255 | Magical Bells | B | B063 | double | Bell/Mallet | D1 p19 |  |
| F01-P0256 | Tinkle Bells | B | B127 | double | Bell/Mallet | D1 p19 |  |
| F01-P0257 | Techno Phonic | C | C000 | double | FastSynth | D1 p20 |  |
| F01-P0258 | Raunchy Brass | C | C064 | double | FastSynth | D1 p20 |  |
| F01-P0259 | Warm E.Grand | C | C001 | double | Keyboard | D1 p20 |  |
| F01-P0260 | Piano&String/Pad | C | C065 | double | Keyboard | D1 p20 |  |
| F01-P0261 | Rave <Ribbon> | C | C002 | double | Hit/Arpg | D1 p20 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0262 | Old Record Hit 2 | C | C066 | double | Hit/Arpg | D1 p20 |  |
| F01-P0263 | Stereo Strings 2 | C | C003 | double | Strings | D1 p20 |  |
| F01-P0264 | String World | C | C067 | double | Strings | D1 p20 |  |
| F01-P0265 | WAcKy HiPHop Kit | C | C004 | double | Drums | D1 p20 |  |
| F01-P0266 | Gamelan Gong | C | C068 | single | Drums | D1 p20 |  |
| F01-P0267 | Spanish Guitar | C | C005 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0268 | Super Clean Gtr | C | C069 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0269 | Phat Bass | C | C006 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0270 | Digi Bass 1 | C | C070 | single | Bass/Synth Bass | D1 p20 |  |
| F01-P0271 | Dirty "B" | C | C007 | double | Organ | D1 p20 |  |
| F01-P0272 | Perc-2ndHarmonic | C | C071 | double | Organ | D1 p20 |  |
| F01-P0273 | Super Sweeper | C | C008 | double | SlowSynth | D1 p20 |  |
| F01-P0274 | Sweeper Strings | C | C072 | double | SlowSynth | D1 p20 |  |
| F01-P0275 | DynaBrassStereo1 | C | C009 | double | Brass | D1 p20 |  |
| F01-P0276 | BrassSlow Stereo | C | C073 | double | Brass | D1 p20 |  |
| F01-P0277 | <ReverseVoxBox> | C | C010 | double | SE | D1 p20 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0278 | Dirty Wounds | C | C074 | double | FastSynth | D1 p20 |  |
| F01-P0279 | Vocalscaping | C | C011 | double | Vocal/Airy | D1 p20 |  |
| F01-P0280 | Choir Mmh | C | C075 | single | Vocal/Airy | D1 p20 |  |
| F01-P0281 | Sine Switcher | C | C012 | double | LeadSynth | D1 p20 |  |
| F01-P0282 | Sync Kronicity | C | C076 | double | LeadSynth | D1 p20 |  |
| F01-P0283 | Breathy BariSax | C | C013 | double | Woodwind/Reed | D1 p20 |  |
| F01-P0284 | Classic Flute | C | C077 | double | Woodwind/Reed | D1 p20 |  |
| F01-P0285 | Stereo WaveSweep | C | C014 | double | MotionSynth | D1 p20 |  |
| F01-P0286 | Water is Deep | C | C078 | double | MotionSynth | D1 p20 |  |
| F01-P0287 | Mallet Clocker | C | C015 | double | Bell/Mallet | D1 p20 |  |
| F01-P0288 | Digital Bells 2 | C | C079 | single | Bell/Mallet | D1 p20 |  |
| F01-P0289 | PowerRezSweep | C | C016 | double | FastSynth | D1 p20 |  |
| F01-P0290 | Dual Filterz | C | C080 | double | FastSynth | D1 p20 |  |
| F01-P0291 | Pro-Stage EP | C | C017 | double | Keyboard | D1 p20 |  |
| F01-P0292 | Classic Wurly | C | C081 | double | Keyboard | D1 p20 |  |
| F01-P0293 | Motion Raver | C | C018 | double | Hit/Arpg | D1 p20 |  |
| F01-P0294 | Organ Impact | C | C082 | double | Hit/Arpg | D1 p20 |  |
| F01-P0295 | Rosin Strings | C | C019 | single | Strings | D1 p20 |  |
| F01-P0296 | Violin & Viola | C | C083 | double | Strings | D1 p20 |  |
| F01-P0297 | Nasty TRicKy Kit | C | C020 | double | Drums | D1 p20 |  |
| F01-P0298 | Psycho Pitch Kit | C | C084 | double | Drums | D1 p20 |  |
| F01-P0299 | Nu Strat | C | C021 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0300 | CleanMute-Guitar | C | C085 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0301 | A.Bass Buzzing | C | C022 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0302 | Fretless Switch | C | C086 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0303 | Perky Tonewheels | C | C023 | double | Organ | D1 p20 |  |
| F01-P0304 | BX3 Short Decay | C | C087 | double | Organ | D1 p20 |  |
| F01-P0305 | DigitalThroats 1 | C | C024 | double | SlowSynth | D1 p20 |  |
| F01-P0306 | Cross Sweeper | C | C088 | double | SlowSynth | D1 p20 |  |
| F01-P0307 | Trombone Soft | C | C025 | single | Brass | D1 p20 |  |
| F01-P0308 | Trombones | C | C089 | double | Brass | D1 p20 |  |
| F01-P0309 | Video Blaster | C | C026 | double | SE | D1 p20 |  |
| F01-P0310 | Cosmic Toys | C | C090 | double | SE | D1 p20 |  |
| F01-P0311 | Grand Choir | C | C027 | double | Vocal/Airy | D1 p20 |  |
| F01-P0312 | DigitalThroats 2 | C | C091 | double | Vocal/Airy | D1 p20 |  |
| F01-P0313 | A leadload | C | C028 | double | LeadSynth | D1 p20 |  |
| F01-P0314 | Fire Wave | C | C092 | double | FastSynth | D1 p20 |  |
| F01-P0315 | Soft TenorSax | C | C029 | double | Woodwind/Reed | D1 p20 |  |
| F01-P0316 | Wooden Flute | C | C093 | single | Woodwind/Reed | D1 p20 |  |
| F01-P0317 | Cyclic X-Fades | C | C030 | double | MotionSynth | D1 p20 |  |
| F01-P0318 | Smoothy Pad | C | C094 | double | SlowSynth | D1 p20 |  |
| F01-P0319 | Breathy Bells | C | C031 | double | Bell/Mallet | D1 p20 |  |
| F01-P0320 | Hybrid Bell | C | C095 | double | Bell/Mallet | D1 p20 |  |
| F01-P0321 | Auto Groove | C | C032 | double | Hit/Arpg | D1 p20 |  |
| F01-P0322 | Square Snaps | C | C096 | double | FastSynth | D1 p20 |  |
| F01-P0323 | Piano Pad 2 | C | C033 | double | Keyboard | D1 p20 |  |
| F01-P0324 | Clav Snap | C | C097 | double | Keyboard | D1 p20 |  |
| F01-P0325 | Techno Stat | C | C034 | double | Hit/Arpg | D1 p20 |  |
| F01-P0326 | Golithanic Stab | C | C098 | double | Hit/Arpg | D1 p20 |  |
| F01-P0327 | Viola Solo | C | C035 | single | Strings | D1 p20 |  |
| F01-P0328 | ComponentStrings | C | C099 | double | Strings | D1 p20 |  |
| F01-P0329 | D'n'B Gate Kit | C | C036 | double | Drums | D1 p20 |  |
| F01-P0330 | Jazzy Pitch Kit | C | C100 | double | Drums | D1 p20 |  |
| F01-P0331 | SingleCoil+Piezo | C | C037 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0332 | Fingertips12 Gtr | C | C101 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0333 | SynBassRes | C | C038 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0334 | Reso Bass | C | C102 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0335 | Superdark Organ | C | C039 | double | Organ | D1 p20 |  |
| F01-P0336 | Super BX Perc | C | C103 | double | Organ | D1 p20 |  |
| F01-P0337 | Goliath's Waves | C | C040 | double | SlowSynth | D1 p20 |  |
| F01-P0338 | Ghostly Strings | C | C104 | double | SlowSynth | D1 p20 |  |
| F01-P0339 | Blind as a Bat | C | C041 | double | Bass/Synth Bass | D1 p20 |  |
| F01-P0340 | Attack Brass ST | C | C105 | double | Brass | D1 p20 |  |
| F01-P0341 | Rave Stabz | C | C042 | double | Hit/Arpg | D1 p20 |  |
| F01-P0342 | Compu-shift | C | C106 | double | SE | D1 p20 |  |
| F01-P0343 | Oooh Choir | C | C043 | double | Vocal/Airy | D1 p20 |  |
| F01-P0344 | Ooh/Aah VoicesST | C | C107 | double | Vocal/Airy | D1 p20 |  |
| F01-P0345 | X-Mod Raver | C | C044 | double | LeadSynth | D1 p20 |  |
| F01-P0346 | Grain Board | C | C108 | double | FastSynth | D1 p20 |  |
| F01-P0347 | Breathy AltoSax | C | C045 | double | Woodwind/Reed | D1 p20 |  |
| F01-P0348 | Bari Blaster | C | C109 | double | Woodwind/Reed | D1 p20 |  |
| F01-P0349 | Spectrum Alloy | C | C046 | double | MotionSynth | D1 p20 |  |
| F01-P0350 | JS/Ribbon Pad | C | C110 | double | MotionSynth | D1 p20 |  |
| F01-P0351 | Log Drum & Bells | C | C047 | double | Bell/Mallet | D1 p20 |  |
| F01-P0352 | Ribbon Rhythm | C | C111 | double | Bell/Mallet | D1 p20 |  |
| F01-P0353 | Square Rez | C | C048 | double | FastSynth | D1 p20 |  |
| F01-P0354 | Power Saw | C | C112 | double | FastSynth | D1 p20 |  |
| F01-P0355 | Dark and Warm | C | C049 | double | Keyboard | D1 p20 |  |
| F01-P0356 | Reson Piano | C | C113 | double | Keyboard | D1 p20 |  |
| F01-P0357 | Old Record Hit 1 | C | C050 | double | Hit/Arpg | D1 p20 |  |
| F01-P0358 | Pop Guitar Hit | C | C114 | double | Hit/Arpg | D1 p20 |  |
| F01-P0359 | Ensemble & Solo | C | C051 | double | Strings | D1 p20 |  |
| F01-P0360 | Analog Velvet | C | C115 | double | Strings | D1 p20 |  |
| F01-P0361 | Tiny Perc. Kit | C | C052 | double | Drums | D1 p20 |  |
| F01-P0362 | KING<>KONG Kit | C | C116 | double | Drums | D1 p20 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0363 | CountryNuStrings | C | C053 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0364 | Arpeggio Guitar | C | C117 | double | Guitar/Plucked | D1 p20 |  |
| F01-P0365 | Slap Bass v/s | C | C054 | single | Bass/Synth Bass | D1 p20 |  |
| F01-P0366 | E.Bass Finger 2 | C | C118 | single | Bass/Synth Bass | D1 p20 |  |
| F01-P0367 | Jazz Organ 2 | C | C055 | double | Organ | D1 p20 |  |
| F01-P0368 | Organ Positive | C | C119 | double | Organ | D1 p20 |  |
| F01-P0369 | Harmonic Pad | C | C056 | double | SlowSynth | D1 p20 |  |
| F01-P0370 | Mellow Movie Pad | C | C120 | double | SlowSynth | D1 p20 |  |
| F01-P0371 | Tubas Gold | C | C057 | double | Brass | D1 p20 |  |
| F01-P0372 | Movie Brass | C | C121 | double | Brass | D1 p20 |  |
| F01-P0373 | Birds & Bugs | C | C058 | double | SE | D1 p20 |  |
| F01-P0374 | One Note Stories | C | C122 | double | SE | D1 p20 |  |
| F01-P0375 | Aah Choir | C | C059 | double | Vocal/Airy | D1 p20 |  |
| F01-P0376 | Ether Voices | C | C123 | double | Vocal/Airy | D1 p20 |  |
| F01-P0377 | Busy Sync | C | C060 | double | LeadSynth | D1 p20 |  |
| F01-P0378 | Brian's Sync | C | C124 | double | LeadSynth | D1 p20 |  |
| F01-P0379 | Warm Oboe | C | C061 | single | Woodwind/Reed | D1 p20 |  |
| F01-P0380 | PanFlute | C | C125 | single | Woodwind/Reed | D1 p20 |  |
| F01-P0381 | Stereo Rez Sweep | C | C062 | double | MotionSynth | D1 p20 |  |
| F01-P0382 | Glass Vox | C | C126 | double | MotionSynth | D1 p20 |  |
| F01-P0383 | GlassBell Bright | C | C063 | single | Bell/Mallet | D1 p20 |  |
| F01-P0384 | Bottle-Bell | C | C127 | double | Bell/Mallet | D1 p20 |  |
| F01-P0385 | Ribbon Morpher | D | D000 | double | FastSynth | D1 p21 |  |
| F01-P0386 | Cat Lead (-Y) | D | D064 | double | LeadSynth | D1 p21 |  |
| F01-P0387 | Super Grand | D | D001 | double | Keyboard | D1 p21 |  |
| F01-P0388 | Classic Tines | D | D065 | double | Keyboard | D1 p21 |  |
| F01-P0389 | Raggae Gtr Hit | D | D002 | double | Hit/Arpg | D1 p21 |  |
| F01-P0390 | SharpBottleBlow | D | D066 | double | FastSynth | D1 p21 |  |
| F01-P0391 | PizzicatoSection | D | D003 | double | Strings | D1 p21 |  |
| F01-P0392 | Dance Lead | D | D067 | single | LeadSynth | D1 p21 |  |
| F01-P0393 | UGLY HoUSe Kit | D | D004 | double | Drums | D1 p21 |  |
| F01-P0394 | Piano Layers | D | D068 | double | Keyboard | D1 p21 |  |
| F01-P0395 | Rhythmic E.Gtr | D | D005 | single | Guitar/Plucked | D1 p21 |  |
| F01-P0396 | Filter Gate | D | D069 | double | MotionSynth | D1 p21 |  |
| F01-P0397 | Dirty Man Bass | D | D006 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0398 | Digi Bass 2 | D | D070 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0399 | Killer BX-3 | D | D007 | double | Organ | D1 p21 |  |
| F01-P0400 | Rotary Organ | D | D071 | double | Organ | D1 p21 |  |
| F01-P0401 | Ultra Res. Sweep | D | D008 | double | SlowSynth | D1 p21 |  |
| F01-P0402 | Electrik Brass | D | D072 | double | SlowSynth | D1 p21 |  |
| F01-P0403 | Power Brass | D | D009 | double | Brass | D1 p21 |  |
| F01-P0404 | Frozen Glaciers | D | D073 | double | SE | D1 p21 |  |
| F01-P0405 | Just Do It!!! | D | D010 | double | SE | D1 p21 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0406 | Warm Flugel Horn | D | D074 | double | Brass | D1 p21 |  |
| F01-P0407 | Dream Voices | D | D011 | double | Vocal/Airy | D1 p21 |  |
| F01-P0408 | Wispy Dry Wind | D | D075 | double | SE | D1 p21 |  |
| F01-P0409 | Octo Lead | D | D012 | double | LeadSynth | D1 p21 |  |
| F01-P0410 | RibbonYoHeadOff! | D | D076 | double | LeadSynth | D1 p21 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0411 | Warm Shakuhachi | D | D013 | single | Woodwind/Reed | D1 p21 |  |
| F01-P0412 | Pipe Tutti | D | D077 | double | Organ | D1 p21 |  |
| F01-P0413 | Cinema Pad | D | D014 | double | SlowSynth | D1 p21 |  |
| F01-P0414 | Syn Ghostly | D | D078 | double | MotionSynth | D1 p21 |  |
| F01-P0415 | Kalimba Soft | D | D015 | single | Bell/Mallet | D1 p21 |  |
| F01-P0416 | Warm Steel Drums | D | D079 | double | Bell/Mallet | D1 p21 |  |
| F01-P0417 | Thunderlog | D | D016 | double | FastSynth | D1 p21 |  |
| F01-P0418 | Solar Surfing | D | D080 | double | MotionSynth | D1 p21 |  |
| F01-P0419 | Club E.Piano | D | D017 | double | Keyboard | D1 p21 |  |
| F01-P0420 | Splat Slap | D | D081 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0421 | Symphonic Waves | D | D018 | double | MotionSynth | D1 p21 |  |
| F01-P0422 | Rhythmnosis | D | D082 | double | MotionSynth | D1 p21 |  |
| F01-P0423 | X-Synced | D | D019 | double | LeadSynth | D1 p21 |  |
| F01-P0424 | The Mandolin | D | D083 | double | Guitar/Plucked | D1 p21 |  |
| F01-P0425 | Tamborine Dream | D | D020 | double | Drums | D1 p21 |  |
| F01-P0426 | Crazy Wah Wah | D | D084 | double | MotionSynth | D1 p21 |  |
| F01-P0427 | Joystick Gtr(-Y) | D | D021 | double | Guitar/Plucked | D1 p21 |  |
| F01-P0428 | Square Wave Lead | D | D085 | double | LeadSynth | D1 p21 |  |
| F01-P0429 | Fretless Snap | D | D022 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0430 | Fretless Bass 2 | D | D086 | single | Bass/Synth Bass | D1 p21 |  |
| F01-P0431 | Overdrive BX3 | D | D023 | double | Organ | D1 p21 |  |
| F01-P0432 | Old VOX Legend | D | D087 | double | Organ | D1 p21 |  |
| F01-P0433 | That Heaven Vibe | D | D024 | double | SlowSynth | D1 p21 |  |
| F01-P0434 | Freedom Pad | D | D088 | double | SlowSynth | D1 p21 |  |
| F01-P0435 | DynaBrassStereo2 | D | D025 | double | Brass | D1 p21 |  |
| F01-P0436 | Classic Digi EP | D | D089 | double | Keyboard | D1 p21 |  |
| F01-P0437 | Distant Lights | D | D026 | double | SE | D1 p21 |  |
| F01-P0438 | Loonie Bin | D | D090 | double | SE | D1 p21 |  |
| F01-P0439 | OohSlow VoicesST | D | D027 | double | Vocal/Airy | D1 p21 |  |
| F01-P0440 | ResRes One | D | D091 | double | LeadSynth | D1 p21 |  |
| F01-P0441 | Electro Lead | D | D028 | double | LeadSynth | D1 p21 |  |
| F01-P0442 | Unholy Water | D | D092 | double | SE | D1 p21 |  |
| F01-P0443 | WarPipes | D | D029 | double | Woodwind/Reed | D1 p21 |  |
| F01-P0444 | Shirk Hit | D | D093 | single | SE | D1 p21 |  |
| F01-P0445 | Pods In The Pad | D | D030 | double | MotionSynth | D1 p21 |  |
| F01-P0446 | Lava Lakes | D | D094 | double | MotionSynth | D1 p21 |  |
| F01-P0447 | Finger Cymbal | D | D031 | double | Bell/Mallet | D1 p21 |  |
| F01-P0448 | E.Bass Finger 3 | D | D095 | single | Bass/Synth Bass | D1 p21 |  |
| F01-P0449 | SynthPianoid | D | D032 | double | FastSynth | D1 p21 |  |
| F01-P0450 | Old Portamento | D | D096 | double | LeadSynth | D1 p21 |  |
| F01-P0451 | Chorus Piano | D | D033 | double | Keyboard | D1 p21 |  |
| F01-P0452 | Mutant Clav | D | D097 | double | Keyboard | D1 p21 |  |
| F01-P0453 | Cyber Choir | D | D034 | double | FastSynth | D1 p21 |  |
| F01-P0454 | Random Pulsator | D | D098 | double | MotionSynth | D1 p21 |  |
| F01-P0455 | Rich Lead | D | D035 | double | LeadSynth | D1 p21 |  |
| F01-P0456 | Mini Lead | D | D099 | double | LeadSynth | D1 p21 |  |
| F01-P0457 | Djembe/Log Drum | D | D036 | double | Drums | D1 p21 |  |
| F01-P0458 | DWGS E.Piano | D | D100 | double | Keyboard | D1 p21 |  |
| F01-P0459 | Guitarish | D | D037 | double | Guitar/Plucked | D1 p21 |  |
| F01-P0460 | Rhythm Of Life | D | D101 | double | MotionSynth | D1 p21 |  |
| F01-P0461 | Hybrid Bass | D | D038 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0462 | SuperSwtch Bass2 | D | D102 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0463 | Good Old "B" | D | D039 | double | Organ | D1 p21 |  |
| F01-P0464 | Full Pipes | D | D103 | double | Organ | D1 p21 |  |
| F01-P0465 | Analog Pad | D | D040 | double | SlowSynth | D1 p21 |  |
| F01-P0466 | Metal SlowPad | D | D104 | single | SlowSynth | D1 p21 |  |
| F01-P0467 | French Horn Sect | D | D041 | double | Brass | D1 p21 |  |
| F01-P0468 | BeBop Cornet | D | D105 | single | Brass | D1 p21 |  |
| F01-P0469 | Vanishing Planet | D | D042 | double | SE | D1 p21 |  |
| F01-P0470 | Cosmic Waves | D | D106 | double | SE | D1 p21 |  |
| F01-P0471 | Classic Vox | D | D043 | double | Vocal/Airy | D1 p21 |  |
| F01-P0472 | Fluttering EP | D | D107 | double | Keyboard | D1 p21 |  |
| F01-P0473 | BrightPulseLead | D | D044 | double | LeadSynth | D1 p21 |  |
| F01-P0474 | AMSFeedbackLead | D | D108 | double | LeadSynth | D1 p21 |  |
| F01-P0475 | Digi Layer Keys | D | D045 | double | Keyboard | D1 p21 |  |
| F01-P0476 | Tomorrow's Pad | D | D109 | double | MotionSynth | D1 p21 |  |
| F01-P0477 | OutOfTheClouds | D | D046 | double | MotionSynth | D1 p21 |  |
| F01-P0478 | BPM Filter Sweep | D | D110 | double | MotionSynth | D1 p21 |  |
| F01-P0479 | Garbage Mallets | D | D047 | double | Bell/Mallet | D1 p21 |  |
| F01-P0480 | Small Harp | D | D111 | double | Guitar/Plucked | D1 p21 |  |
| F01-P0481 | Band Passed | D | D048 | double | FastSynth | D1 p21 |  |
| F01-P0482 | Dark Element | D | D112 | double | FastSynth | D1 p21 |  |
| F01-P0483 | Stereo Digi EP | D | D049 | double | Keyboard | D1 p21 |  |
| F01-P0484 | Silky Digi EP | D | D113 | double | Keyboard | D1 p21 |  |
| F01-P0485 | Rhythm Rezonator | D | D050 | double | MotionSynth | D1 p21 |  |
| F01-P0486 | Moon Talker | D | D114 | double | MotionSynth | D1 p21 |  |
| F01-P0487 | Syn Pipes | D | D051 | double | LeadSynth | D1 p21 |  |
| F01-P0488 | Sitar/Tamboura | D | D115 | double | Guitar/Plucked | D1 p21 |  |
| F01-P0489 | *SpaceShip* | D | D052 | double | Drums | D1 p21 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0490 | Rave Volutionary | D | D116 | double | Hit/Arpg | D1 p21 |  |
| F01-P0491 | Time Machines | D | D053 | double | MotionSynth | D1 p21 |  |
| F01-P0492 | Hubble Pad | D | D117 | double | MotionSynth | D1 p21 |  |
| F01-P0493 | Thumb Bass | D | D054 | single | Bass/Synth Bass | D1 p21 |  |
| F01-P0494 | DynaThumb Bass | D | D118 | single | Bass/Synth Bass | D1 p21 |  |
| F01-P0495 | Dirty Jazz Organ | D | D055 | double | Organ | D1 p21 |  |
| F01-P0496 | "Flauto" Pipes | D | D119 | double | Organ | D1 p21 |  |
| F01-P0497 | Lonley Sphinx | D | D056 | double | SlowSynth | D1 p21 |  |
| F01-P0498 | Noble Brite Pad | D | D120 | double | SlowSynth | D1 p21 |  |
| F01-P0499 | The Avenger | D | D057 | double | Hit/Arpg | D1 p21 |  |
| F01-P0500 | Fugue In L.F.O. | D | D121 | double | MotionSynth | D1 p21 |  |
| F01-P0501 | Bad Weather | D | D058 | double | SE | D1 p21 |  |
| F01-P0502 | Black Box | D | D122 | double | SE | D1 p21 |  |
| F01-P0503 | Cript-on | D | D059 | double | SE | D1 p21 |  |
| F01-P0504 | Station Of Waves | D | D123 | double | MotionSynth | D1 p21 |  |
| F01-P0505 | Thin AnaLead | D | D060 | double | LeadSynth | D1 p21 |  |
| F01-P0506 | R&B Lead | D | D124 | double | LeadSynth | D1 p21 |  |
| F01-P0507 | Wacky A.Bass | D | D061 | double | Bass/Synth Bass | D1 p21 |  |
| F01-P0508 | Motion Ocean ! | D | D125 | double | MotionSynth | D1 p21 | name printed with marker characters; recorded exactly as the document prints it |
| F01-P0509 | Virtual Traveler | D | D062 | double | MotionSynth | D1 p21 |  |
| F01-P0510 | Tight Brass 2 | D | D126 | double | Brass | D1 p21 |  |
| F01-P0511 | Digi-Bell | D | D063 | double | Bell/Mallet | D1 p21 |  |
| F01-P0512 | Moon Cycles | D | D127 | double | MotionSynth | D1 p21 |  |
| F01-P0513 | Acoustic Piano | G | G001 | single | Keyboard | D1 p22 |  |
| F01-P0514 | Steel Guitar | G | G026 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0515 | Synth Vox | G | G055 | single | Vocal/Airy | D1 p22 |  |
| F01-P0516 | Acoustic Piano w | G | g(1)001 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0517 | 12-StringsGuitar | G | g(1)026 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0518 | Analog Voice | G | g(1)055 | single | Vocal/Airy | D1 p22 | document's own GM variant row |
| F01-P0519 | Acoustic Piano d | G | g(2)001 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0520 | Mandolin | G | g(2)026 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0521 | Orchestra Hit | G | G056 | single | Hit/Arpg | D1 p22 |  |
| F01-P0522 | Bright Piano | G | G002 | single | Keyboard | D1 p22 |  |
| F01-P0523 | Steel Gtr. Body | G | g(3)026 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0524 | Bass Hit Plus | G | g(1)056 | double | Hit/Arpg | D1 p22 | document's own GM variant row |
| F01-P0525 | Bright Piano w | G | g(1)002 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0526 | Jazz Guitar | G | G027 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0527 | 6th Hit | G | g(2)056 | double | Hit/Arpg | D1 p22 | document's own GM variant row |
| F01-P0528 | El.Grand Piano | G | G003 | single | Keyboard | D1 p22 |  |
| F01-P0529 | Pedal Steel Gtr. | G | g(1)027 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0530 | Euro Hit | G | g(3)056 | double | Hit/Arpg | D1 p22 | document's own GM variant row |
| F01-P0531 | El.Grand Piano w | G | g(1)003 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0532 | Clean Guitar | G | G028 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0533 | Trumpet | G | G057 | single | Brass | D1 p22 |  |
| F01-P0534 | Honkey-Tonk | G | G004 | double | Keyboard | D1 p22 |  |
| F01-P0535 | Detuned Clean Gt | G | g(1)028 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0536 | Soft Trumpet | G | g(1)057 | single | Brass | D1 p22 | document's own GM variant row |
| F01-P0537 | Honkey-Tonk w | G | g(1)004 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0538 | Mid Tone Guitar | G | g(2)028 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0539 | Trombone 1 | G | G058 | single | Brass | D1 p22 |  |
| F01-P0540 | Electric Piano 1 | G | G005 | single | Keyboard | D1 p22 |  |
| F01-P0541 | Muted Guitar | G | G029 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0542 | Trombone 2 | G | g(1)058 | double | Brass | D1 p22 | document's own GM variant row |
| F01-P0543 | Detuned E.Piano1 | G | g(1)005 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0544 | Funky Guitar | G | g(1)029 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0545 | Bright Trombone | G | g(2)058 | single | Brass | D1 p22 | document's own GM variant row |
| F01-P0546 | ElectricPiano1 v | G | g(2)005 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0547 | Muted Guitar v | G | g(2)029 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0548 | Tuba | G | G059 | single | Brass | D1 p22 |  |
| F01-P0549 | 60’s E.Piano | G | g(3)005 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0550 | Jazz Man | G | g(3)029 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0551 | Muted Trumpet 1 | G | G060 | single | Brass | D1 p22 |  |
| F01-P0552 | Electric Piano 2 | G | G006 | single | Keyboard | D1 p22 |  |
| F01-P0553 | Overdrive Guitar | G | G030 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0554 | Muted Trumpet 2 | G | g(1)060 | single | Brass | D1 p22 | document's own GM variant row |
| F01-P0555 | Detuned E.Piano2 | G | g(1)006 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0556 | Guitar Pinch | G | g(1)030 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0557 | French Horn | G | G061 | single | Brass | D1 p22 |  |
| F01-P0558 | ElectricPiano2 v | G | g(2)006 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0559 | DistortionGuitar | G | G031 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0560 | Warm French Horn | G | g(1)061 | double | Brass | D1 p22 | document's own GM variant row |
| F01-P0561 | E.Piano Legend | G | g(3)006 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0562 | Feedback Guitar | G | g(1)031 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0563 | Brass Section 1 | G | G062 | single | Brass | D1 p22 |  |
| F01-P0564 | E.Piano Phase | G | g(4)006 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0565 | Dist. Rhythm Gtr | G | g(2)031 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0566 | Brass Section 2 | G | g(1)062 | double | Brass | D1 p22 | document's own GM variant row |
| F01-P0567 | Harpsichord | G | G007 | single | Keyboard | D1 p22 |  |
| F01-P0568 | Guitar Harmonics | G | G032 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0569 | Synth Brass 1 | G | G063 | double | FastSynth | D1 p22 |  |
| F01-P0570 | Harpsi. oct-mix | G | g(1)007 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0571 | Guitar Feedback | G | g(1)032 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0572 | Synth Brass 3 | G | g(1)063 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0573 | Harpsichord w | G | g(2)007 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0574 | Acoustic Bass | G | G033 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0575 | Analog Brass 1 | G | g(2)063 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0576 | Harpsi. key-off | G | g(3)007 | double | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0577 | Fingered Bass | G | G034 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0578 | Jump Brass | G | g(3)063 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0579 | Clavi. | G | G008 | single | Keyboard | D1 p22 |  |
| F01-P0580 | Finger Slap Bass | G | g(1)034 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0581 | Synth Brass 2 | G | G064 | single | FastSynth | D1 p22 |  |
| F01-P0582 | Pulse Clavi. | G | g(1)008 | single | Keyboard | D1 p22 | document's own GM variant row |
| F01-P0583 | Picked Bass | G | G035 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0584 | Synth Brass 4 | G | g(1)064 | single | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0585 | Celesta | G | G009 | single | Bell/Mallet | D1 p22 |  |
| F01-P0586 | Fretless Bass | G | G036 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0587 | Analog Brass 2 | G | g(2)064 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0588 | Glockenspiel | G | G010 | double | Bell/Mallet | D1 p22 |  |
| F01-P0589 | Slap Bass 1 | G | G037 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0590 | Soprano Sax | G | G065 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0591 | Music Box | G | G011 | single | Bell/Mallet | D1 p22 |  |
| F01-P0592 | Slap Bass 2 | G | G038 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0593 | Alto Sax | G | G066 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0594 | Vibraphone | G | G012 | single | Bell/Mallet | D1 p22 |  |
| F01-P0595 | Synth Bass 1 | G | G039 | double | Bass/Synth Bass | D1 p22 |  |
| F01-P0596 | Tenor Sax | G | G067 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0597 | Vibraphone w | G | g(1)012 | single | Bell/Mallet | D1 p22 | document's own GM variant row |
| F01-P0598 | Warm Synth Bass | G | g(1)039 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0599 | Baritone Sax | G | G068 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0600 | Marimba | G | G013 | single | Bell/Mallet | D1 p22 |  |
| F01-P0601 | Synth Bass 3 | G | g(2)039 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0602 | Oboe | G | G069 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0603 | Marimba w | G | g(1)013 | single | Bell/Mallet | D1 p22 | document's own GM variant row |
| F01-P0604 | Clavi Bass | G | g(3)039 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0605 | English Horn | G | G070 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0606 | Xylophone | G | G014 | single | Bell/Mallet | D1 p22 |  |
| F01-P0607 | Hammer | G | g(4)039 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0608 | Bassoon | G | G071 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0609 | Tubular Bells | G | G015 | single | Bell/Mallet | D1 p22 |  |
| F01-P0610 | Synth Bass 2 | G | G040 | single | Bass/Synth Bass | D1 p22 |  |
| F01-P0611 | Clarinet | G | G072 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0612 | Church Bell | G | g(1)015 | double | Bell/Mallet | D1 p22 | document's own GM variant row |
| F01-P0613 | Synth Bass 4 | G | g(1)040 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0614 | Piccolo | G | G073 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0615 | Carillon | G | g(2)015 | double | Bell/Mallet | D1 p22 | document's own GM variant row |
| F01-P0616 | Rubber Bass | G | g(2)040 | double | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0617 | Flute | G | G074 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0618 | Santur | G | G016 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0619 | Attack Pulse | G | g(3)040 | single | Bass/Synth Bass | D1 p22 | document's own GM variant row |
| F01-P0620 | Recorder | G | G075 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0621 | Drawbar Organ 1 | G | G017 | single | Organ | D1 p22 |  |
| F01-P0622 | Violin | G | G041 | single | Strings | D1 p22 |  |
| F01-P0623 | Pan Flute | G | G076 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0624 | Det.DrawbarOrgan | G | g(1)017 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0625 | Slow Violin | G | g(1)041 | single | Strings | D1 p22 | document's own GM variant row |
| F01-P0626 | Blown Bottle | G | G077 | double | Woodwind/Reed | D1 p22 |  |
| F01-P0627 | Italian60’sOrgan | G | g(2)017 | single | Organ | D1 p22 | document's own GM variant row |
| F01-P0628 | Viola | G | G042 | single | Strings | D1 p22 |  |
| F01-P0629 | Shakuhachi | G | G078 | double | Woodwind/Reed | D1 p22 |  |
| F01-P0630 | Drawbar Organ 2 | G | g(3)017 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0631 | Cello | G | G043 | single | Strings | D1 p22 |  |
| F01-P0632 | Whistle | G | G079 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0633 | PercussiveOrgan1 | G | G018 | single | Organ | D1 p22 |  |
| F01-P0634 | Contrabass | G | G044 | single | Strings | D1 p22 |  |
| F01-P0635 | Ocarina | G | G080 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0636 | Det.Perc-Organ | G | g(1)018 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0637 | Tremolo Strings | G | G045 | single | Strings | D1 p22 |  |
| F01-P0638 | Detuned Square | G | G081 | double | FastSynth | D1 p22 |  |
| F01-P0639 | PercussiveOrgan2 | G | g(2)018 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0640 | Pizzicato Str. | G | G046 | single | Strings | D1 p22 |  |
| F01-P0641 | Simple Square | G | g(1)081 | single | LeadSynth | D1 p22 | document's own GM variant row |
| F01-P0642 | Rock Organ | G | G019 | single | Organ | D1 p22 |  |
| F01-P0643 | Orchestral Harp | G | G047 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0644 | Simple Sine | G | g(2)081 | single | LeadSynth | D1 p22 | document's own GM variant row |
| F01-P0645 | Church Organ | G | G020 | single | Organ | D1 p22 |  |
| F01-P0646 | Yang Chin | G | g(1)047 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0647 | Detuned Sawtooth | G | G082 | double | FastSynth | D1 p22 |  |
| F01-P0648 | Church Org Oct. | G | g(1)020 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0649 | Timpani | G | G048 | double | Drums | D1 p22 |  |
| F01-P0650 | Simple Sawtooth | G | g(1)082 | single | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0651 | Det.Church Org | G | g(2)020 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0652 | Strings | G | G049 | single | Strings | D1 p22 |  |
| F01-P0653 | Sawtooth + Pulse | G | g(2)082 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0654 | Reed Organ | G | G021 | double | Organ | D1 p22 |  |
| F01-P0655 | Strings & Brass | G | g(1)049 | double | Strings | D1 p22 | document's own GM variant row |
| F01-P0656 | Double Sawtooth | G | g(3)082 | double | FastSynth | D1 p22 | document's own GM variant row |
| F01-P0657 | Puff Organ | G | g(1)021 | double | Organ | D1 p22 | document's own GM variant row |
| F01-P0658 | 60’s Strings | G | g(2)049 | double | Strings | D1 p22 | document's own GM variant row |
| F01-P0659 | Sequenced Analog | G | g(4)082 | double | Hit/Arpg | D1 p22 | document's own GM variant row |
| F01-P0660 | Accordion 1 | G | G022 | double | Woodwind/Reed | D1 p22 |  |
| F01-P0661 | Slow Strings | G | G050 | single | Strings | D1 p22 |  |
| F01-P0662 | Synth Calliope | G | G083 | double | FastSynth | D1 p22 |  |
| F01-P0663 | Accordion 2 | G | g(1)022 | single | Woodwind/Reed | D1 p22 | document's own GM variant row |
| F01-P0664 | Synth Strings 1 | G | G051 | double | Strings | D1 p22 |  |
| F01-P0665 | Chiff Lead | G | G084 | double | FastSynth | D1 p22 |  |
| F01-P0666 | Harmonica | G | G023 | single | Woodwind/Reed | D1 p22 |  |
| F01-P0667 | Synth Strings 3 | G | g(1)051 | double | Strings | D1 p22 | document's own GM variant row |
| F01-P0668 | Charang | G | G085 | double | LeadSynth | D1 p22 |  |
| F01-P0669 | Bandoneon | G | G024 | double | Woodwind/Reed | D1 p22 |  |
| F01-P0670 | Synth Strings 2 | G | G052 | single | Strings | D1 p22 |  |
| F01-P0671 | Wire Lead | G | g(1)085 | double | LeadSynth | D1 p22 | document's own GM variant row |
| F01-P0672 | Nylon Guitar 1 | G | G025 | single | Guitar/Plucked | D1 p22 |  |
| F01-P0673 | Choir Aahs 1 | G | G053 | single | Vocal/Airy | D1 p22 |  |
| F01-P0674 | Air Voice | G | G086 | double | Vocal/Airy | D1 p22 |  |
| F01-P0675 | Ukulele | G | g(1)025 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0676 | Choir Aahs 2 | G | g(1)053 | single | Vocal/Airy | D1 p22 | document's own GM variant row |
| F01-P0677 | 5th Sawtooth | G | G087 | double | LeadSynth | D1 p22 |  |
| F01-P0678 | NylonGtr.key-off | G | g(2)025 | double | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0679 | Voice Oohs | G | G054 | single | Vocal/Airy | D1 p22 |  |
| F01-P0680 | Bass & Lead | G | G088 | single | LeadSynth | D1 p22 |  |
| F01-P0681 | Nylon Guitar 2 | G | g(3)025 | single | Guitar/Plucked | D1 p22 | document's own GM variant row |
| F01-P0682 | Humming | G | g(1)054 | double | Vocal/Airy | D1 p22 | document's own GM variant row |
| F01-P0683 | Soft Wurl | G | g(1)088 | double | LeadSynth | D1 p22 | document's own GM variant row |
| F01-P0684 | Fantasia | G | G089 | double | FastSynth | D1 p23 |  |
| F01-P0685 | Horse Gallop | G | g(2)124 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0686 | Warm Pad | G | G090 | single | SlowSynth | D1 p23 |  |
| F01-P0687 | Bird Tweet 2 | G | g(3)124 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0688 | Sine Pad | G | g(1)090 | double | SlowSynth | D1 p23 | document's own GM variant row |
| F01-P0689 | Telephone Ring 1 | G | G125 | single | SE | D1 p23 |  |
| F01-P0690 | Polyphonic Synth | G | G091 | double | FastSynth | D1 p23 |  |
| F01-P0691 | Telephone Ring 2 | G | g(1)125 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0692 | Space Voice | G | G092 | double | Vocal/Airy | D1 p23 |  |
| F01-P0693 | Door Creaking | G | g(2)125 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0694 | Itopia | G | g(1)092 | double | Vocal/Airy | D1 p23 | document's own GM variant row |
| F01-P0695 | Door Slamming | G | g(3)125 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0696 | Bowed Glass | G | G093 | double | SlowSynth | D1 p23 |  |
| F01-P0697 | Scratch | G | g(4)125 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0698 | Metallic Pad | G | G094 | double | SlowSynth | D1 p23 |  |
| F01-P0699 | Wind Chime | G | g(5)125 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0700 | Halo Pad | G | G095 | double | Vocal/Airy | D1 p23 |  |
| F01-P0701 | Helicopter | G | G126 | double | SE | D1 p23 |  |
| F01-P0702 | Sweep Pad | G | G096 | single | MotionSynth | D1 p23 |  |
| F01-P0703 | Car Engine | G | g(1)126 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0704 | Ice Rain | G | G097 | double | FastSynth | D1 p23 |  |
| F01-P0705 | Car Stop | G | g(2)126 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0706 | Sound Track | G | G098 | double | SlowSynth | D1 p23 |  |
| F01-P0707 | Car Pass | G | g(3)126 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0708 | Crystal | G | G099 | double | Bell/Mallet | D1 p23 |  |
| F01-P0709 | Car Crash | G | g(4)126 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0710 | Synth Mallet | G | g(1)099 | single | Bell/Mallet | D1 p23 | document's own GM variant row |
| F01-P0711 | Siren | G | g(5)126 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0712 | Atmosphere | G | G100 | double | FastSynth | D1 p23 |  |
| F01-P0713 | Train | G | g(6)126 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0714 | Brightness | G | G101 | double | FastSynth | D1 p23 |  |
| F01-P0715 | Jetplane | G | g(7)126 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0716 | Goblins | G | G102 | double | MotionSynth | D1 p23 |  |
| F01-P0717 | Starship | G | g(8)126 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0718 | Echo Drops | G | G103 | single | FastSynth | D1 p23 |  |
| F01-P0719 | Burst Noise | G | g(9)126 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0720 | Echo Bell | G | g(1)103 | double | FastSynth | D1 p23 | document's own GM variant row |
| F01-P0721 | Applause | G | G127 | double | SE | D1 p23 |  |
| F01-P0722 | Echo Pan | G | g(2)103 | double | FastSynth | D1 p23 | document's own GM variant row |
| F01-P0723 | Laughing | G | g(1)127 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0724 | Star Theme | G | G104 | double | FastSynth | D1 p23 |  |
| F01-P0725 | Screaming | G | g(2)127 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0726 | Sitar 1 | G | G105 | single | Guitar/Plucked | D1 p23 |  |
| F01-P0727 | Punch | G | g(3)127 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0728 | Sitar 2 | G | g(1)105 | double | Guitar/Plucked | D1 p23 | document's own GM variant row |
| F01-P0729 | Heart Beat | G | g(4)127 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0730 | Banjo | G | G106 | single | Guitar/Plucked | D1 p23 |  |
| F01-P0731 | Footsteps | G | g(5)127 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0732 | Shamisen | G | G107 | single | Guitar/Plucked | D1 p23 |  |
| F01-P0733 | Gun Shot | G | G128 | single | SE | D1 p23 |  |
| F01-P0734 | Koto | G | G108 | single | Guitar/Plucked | D1 p23 |  |
| F01-P0735 | Machine Gun | G | g(1)128 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0736 | Taisho Koto | G | g(1)108 | double | Guitar/Plucked | D1 p23 | document's own GM variant row |
| F01-P0737 | Lasergun | G | g(2)128 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0738 | Kalimba | G | G109 | single | Bell/Mallet | D1 p23 |  |
| F01-P0739 | Explosion | G | g(3)128 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0740 | Bagpipe | G | G110 | double | Woodwind/Reed | D1 p23 |  |
| F01-P0741 | STANDARD Kit | G | g(d)001 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0742 | Fiddle | G | G111 | single | Strings | D1 p23 |  |
| F01-P0743 | ROOM Kit | G | g(d)009 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0744 | Shanai | G | G112 | single | Woodwind/Reed | D1 p23 |  |
| F01-P0745 | POWER Kit | G | g(d)017 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0746 | Tinkle Bell | G | G113 | double | Bell/Mallet | D1 p23 |  |
| F01-P0747 | ELECTRONIC Kit | G | g(d)025 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0748 | Agogo | G | G114 | single | Bell/Mallet | D1 p23 |  |
| F01-P0749 | ANALOG Kit | G | g(d)026 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0750 | Steel Drums | G | G115 | double | Drums | D1 p23 |  |
| F01-P0751 | JAZZ Kit | G | g(d)033 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0752 | Woodblock | G | G116 | single | Drums | D1 p23 |  |
| F01-P0753 | BRUSH Kit | G | g(d)041 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0754 | Castanets | G | g(1)116 | single | Drums | D1 p23 | document's own GM variant row |
| F01-P0755 | ORCHESTRA Kit | G | g(d)049 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0756 | Taiko | G | G117 | single | Drums | D1 p23 |  |
| F01-P0757 | SFX Kit | G | g(d)057 | double | Drums | D1 p23 | document's own GM variant row |
| F01-P0758 | Concert BassDrum | G | g(1)117 | single | Drums | D1 p23 | document's own GM variant row |
| F01-P0759 | Melodic Tom 1 | G | G118 | single | Drums | D1 p23 |  |
| F01-P0760 | Melodic Tom 2 | G | g(1)118 | single | Drums | D1 p23 | document's own GM variant row |
| F01-P0761 | Synth Drum | G | G119 | single | Drums | D1 p23 |  |
| F01-P0762 | Analog Tom | G | g(1)119 | single | Drums | D1 p23 | document's own GM variant row |
| F01-P0763 | Electric Drum | G | g(2)119 | single | Drums | D1 p23 | document's own GM variant row |
| F01-P0764 | Reverse Cymbal | G | G120 | single | SE | D1 p23 |  |
| F01-P0765 | Gtr.Fret Noise | G | G121 | single | Guitar/Plucked | D1 p23 |  |
| F01-P0766 | Gtr.CuttingNoise | G | g(1)121 | single | Guitar/Plucked | D1 p23 | document's own GM variant row |
| F01-P0767 | Bass String Slap | G | g(2)121 | single | Bass/Synth Bass | D1 p23 | document's own GM variant row |
| F01-P0768 | Breath Noise | G | G122 | single | SE | D1 p23 |  |
| F01-P0769 | Flute Key Click | G | g(1)122 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0770 | Seashore | G | G123 | double | SE | D1 p23 |  |
| F01-P0771 | Rain | G | g(1)123 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0772 | Thunder | G | g(2)123 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0773 | Wind | G | g(3)123 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0774 | Stream | G | g(4)123 | single | SE | D1 p23 | document's own GM variant row |
| F01-P0775 | Bubble | G | g(5)123 | double | SE | D1 p23 | document's own GM variant row |
| F01-P0776 | Bird Tweet 1 | G | G124 | double | SE | D1 p23 |  |
| F01-P0777 | Dog | G | g(1)124 | single | SE | D1 p23 | document's own GM variant row |

## Factory FX algorithms and routing

**Found in documentation: yes** - the algorithm catalogue and the slot routing are both published in D2. **Per-program FX assignments are not documented**: no public document states which insert algorithms, chain order, send levels or master effect a given factory program uses. That information exists only inside the instrument's own factory data, which is out of bounds for this project, so per-patch chains have to be reconstructed by ear against the catalogue below.

**Slots (D2).** Five insert effects IFX1-IFX5 (chainable), two master effects MFX1-MFX2 (send/return, mono-in stereo-out), and a Master EQ after them. D2's routing diagrams place the chain as: oscillator / filter / amplifier -> IFX1-5 -> MFX send and return -> Master EQ -> output.

**Selectable ranges (D2).**

| Slot | Documented range | Document's wording |
|---|---|---|
| see D2 | 000 - 089 | "Reverb-Gate" |
| see D2 | 000 - 102 | "Hold Delay" |
| see D2 | 000 - 089 | "Reverb-Gate" |

Slot rules read from D2: stated_type_count_2 = 103.

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
| 041 | Early Reflections |
| 042 | Auto Reverse |
| 043 | L/C/R Delay |
| 044 | Stereo/Cross Delay |
| 045 | St. Multitap Delay (Stereo Multitap Delay) |
| 046 | St. Modulation Delay (Stereo Modulation Delay) |
| 047 | St. Dynamic Delay (Stereo Dynamic Delay) |
| 048 | St. Auto Panning Dly (Stereo Auto Panning Delay) |
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
| 059 | P4EQ – Wah (Parametric 4-Band EQ – Wah/Auto Wah) .186 |
| 060 | P4EQ – Cho/Flng (Parametric 4-Band EQ – Chorus/Flanger) .186 |
| 061 | P4EQ – Phaser (Parametric 4-Band EQ – Phaser) |
| 062 | P4EQ – Mt. Delay (Parametric 4-Band EQ – Multitap Delay) . .187 |
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
| 097 | Early Reflections |
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
| F01-W0001 | 0 | A.Piano | - | D1 p32 | |
| F01-W0002 | 1 | A.Piano-M1 | - | D1 p32 | |
| F01-W0003 | 2 | E.Grand Piano | - | D1 p32 | |
| F01-W0004 | 3 | E.P.-FM 1 | - | D1 p32 | |
| F01-W0005 | 4 | E.P.-FM 1 LP | - | D1 p32 | |
| F01-W0006 | 5 | E.P.-FM 2 | - | D1 p32 | |
| F01-W0007 | 6 | E.P.-FM 3 | - | D1 p32 | |
| F01-W0008 | 7 | E.P.-FM 3 LP | - | D1 p32 | |
| F01-W0009 | 8 | E.P.-Dyno Soft | - | D1 p32 | |
| F01-W0010 | 9 | E.P.-Dyno Sft LP | - | D1 p32 | |
| F01-W0011 | 10 | E.P.-Dyno Medium | - | D1 p32 | |
| F01-W0012 | 11 | E.P.-Dyno Med LP | - | D1 p32 | |
| F01-W0013 | 12 | E.P.-Stage Soft | - | D1 p32 | |
| F01-W0014 | 13 | E.P.-Sta Soft LP | - | D1 p32 | |
| F01-W0015 | 14 | E.P.-Stage Hard | - | D1 p32 | |
| F01-W0016 | 15 | E.P.-Sta Hard LP | - | D1 p32 | |
| F01-W0017 | 16 | E.P.-Wurly | - | D1 p32 | |
| F01-W0018 | 17 | E.P.-Wurly LP | - | D1 p32 | |
| F01-W0019 | 18 | E.P. Pad 1 | - | D1 p32 | |
| F01-W0020 | 19 | E.P. Pad 1 LP | - | D1 p32 | |
| F01-W0021 | 20 | E.P. Pad 2 | - | D1 p32 | |
| F01-W0022 | 21 | E.P. Pad 3 | - | D1 p32 | |
| F01-W0023 | 22 | E.P. Pad 3 LP | - | D1 p32 | |
| F01-W0024 | 23 | Clav 1 | - | D1 p32 | |
| F01-W0025 | 24 | Harpsichord-Sngl | - | D1 p32 | |
| F01-W0026 | 25 | E.Organ-Perc 1 | - | D1 p32 | |
| F01-W0027 | 26 | E.Organ-Perc 2 | - | D1 p32 | |
| F01-W0028 | 27 | E.Organ-Perc 3 | - | D1 p32 | |
| F01-W0029 | 28 | E.Organ-2' Perc | - | D1 p32 | |
| F01-W0030 | 29 | E.Organ-M1 1 | - | D1 p32 | |
| F01-W0031 | 30 | E.Organ-M1 2 | - | D1 p32 | |
| F01-W0032 | 31 | E.Organ-N1 5 | - | D1 p32 | |
| F01-W0033 | 32 | E.Organ-N1 3 | - | D1 p32 | |
| F01-W0034 | 33 | E.Organ-N1 3 LP | - | D1 p32 | |
| F01-W0035 | 34 | E.Organ-Jazz 1TR | - | D1 p32 | |
| F01-W0036 | 35 | E.Organ-Jazz 3TR | - | D1 p32 | |
| F01-W0037 | 36 | E.Organ-Perc 3rd | - | D1 p32 | |
| F01-W0038 | 37 | E.Organ-Vox | - | D1 p32 | |
| F01-W0039 | 38 | E.Organ-Vox 1TR | - | D1 p32 | |
| F01-W0040 | 39 | E.Organ-Vox 2TR | - | D1 p32 | |
| F01-W0041 | 40 | E.Organ-Vox 3TR | - | D1 p32 | |
| F01-W0042 | 41 | E.Organ-Soft | - | D1 p32 | |
| F01-W0043 | 42 | E.Organ-Full | - | D1 p32 | |
| F01-W0044 | 43 | E.Organ-Dist | - | D1 p32 | |
| F01-W0045 | 44 | Positive 1 | - | D1 p32 | |
| F01-W0046 | 45 | Positive 2 | - | D1 p32 | |
| F01-W0047 | 46 | Pipe-Mixture 1 | - | D1 p32 | |
| F01-W0048 | 47 | Pipe-Mixture 2 | - | D1 p32 | |
| F01-W0049 | 48 | Pipe-Reed | - | D1 p32 | |
| F01-W0050 | 49 | Pipe-Full | - | D1 p32 | |
| F01-W0051 | 50 | Kalimba | - | D1 p32 | |
| F01-W0052 | 51 | Kalimba-Mute | - | D1 p32 | |
| F01-W0053 | 52 | Kalimba N1 | - | D1 p32 | |
| F01-W0054 | 53 | Music Box 1 | - | D1 p32 | |
| F01-W0055 | 54 | Music Box 1 LP | - | D1 p32 | |
| F01-W0056 | 55 | Music Box 2 | - | D1 p32 | |
| F01-W0057 | 56 | Music Box 2 LP | - | D1 p32 | |
| F01-W0058 | 57 | Marimba | - | D1 p32 | |
| F01-W0059 | 58 | Marimba LP | - | D1 p32 | |
| F01-W0060 | 59 | Xylophone | - | D1 p32 | |
| F01-W0061 | 60 | Vibraphone | - | D1 p32 | |
| F01-W0062 | 61 | Vibraphone LP | - | D1 p32 | |
| F01-W0063 | 62 | Celesta | - | D1 p32 | |
| F01-W0064 | 63 | Celesta LP | - | D1 p32 | |
| F01-W0065 | 64 | Glockenspiel | - | D1 p32 | |
| F01-W0066 | 65 | Glockenspiel LP | - | D1 p32 | |
| F01-W0067 | 66 | Tubular Bell | - | D1 p32 | |
| F01-W0068 | 67 | Log Drum | - | D1 p32 | |
| F01-W0069 | 68 | Steel Drum | - | D1 p32 | |
| F01-W0070 | 69 | Steel Drum LP | - | D1 p32 | |
| F01-W0071 | 70 | Gamelan | - | D1 p32 | |
| F01-W0072 | 71 | Metal FM Bell | - | D1 p32 | |
| F01-W0073 | 72 | FM Bell | - | D1 p32 | |
| F01-W0074 | 73 | FM Glass Bell | - | D1 p32 | |
| F01-W0075 | 74 | Ensemble Bell | - | D1 p32 | |
| F01-W0076 | 75 | Flute | - | D1 p32 | |
| F01-W0077 | 76 | Piccolo | - | D1 p32 | |
| F01-W0078 | 77 | Pan Flute | - | D1 p32 | |
| F01-W0079 | 78 | Shakuhachi | - | D1 p32 | |
| F01-W0080 | 79 | Shakuhachi-Atk | - | D1 p32 | |
| F01-W0081 | 80 | Bottle | - | D1 p32 | |
| F01-W0082 | 81 | Recorder | - | D1 p32 | |
| F01-W0083 | 82 | Ocarina | - | D1 p32 | |
| F01-W0084 | 83 | Clarinet | - | D1 p32 | |
| F01-W0085 | 84 | DoubleReed | - | D1 p32 | |
| F01-W0086 | 85 | Oboe | - | D1 p32 | |
| F01-W0087 | 86 | English Horn | - | D1 p32 | |
| F01-W0088 | 87 | Bassoon | - | D1 p32 | |
| F01-W0089 | 88 | Baritone Sax-T1 | - | D1 p32 | |
| F01-W0090 | 89 | Baritone Sax | - | D1 p32 | |
| F01-W0091 | 90 | Tenor Sax-M1 | - | D1 p32 | |
| F01-W0092 | 91 | TenorSax-Medium | - | D1 p32 | |
| F01-W0093 | 92 | Alto Sax-01/W | - | D1 p32 | |
| F01-W0094 | 93 | Alto Sax-Soft | - | D1 p32 | |
| F01-W0095 | 94 | SopranoSax-Soft | - | D1 p32 | |
| F01-W0096 | 95 | Musette | - | D1 p32 | |
| F01-W0097 | 96 | Musette LP | - | D1 p32 | |
| F01-W0098 | 97 | Accordion | - | D1 p32 | |
| F01-W0099 | 98 | Harmonica | - | D1 p32 | |
| F01-W0100 | 99 | Bagpipe | - | D1 p32 | |
| F01-W0101 | 100 | F.Horn-T1 | - | D1 p32 | |
| F01-W0102 | 101 | French HornEns. | - | D1 p32 | |
| F01-W0103 | 102 | Flugel Horn | - | D1 p32 | |
| F01-W0104 | 103 | Tuba | - | D1 p32 | |
| F01-W0105 | 104 | Trombone-Soft | - | D1 p32 | |
| F01-W0106 | 105 | Trombone-Bright | - | D1 p32 | |
| F01-W0107 | 106 | Trumpet-Medium | - | D1 p32 | |
| F01-W0108 | 107 | Trumpet-Muted | - | D1 p32 | |
| F01-W0109 | 108 | Brass Ens-Sharp | - | D1 p32 | |
| F01-W0110 | 109 | Brass Ens-1 | - | D1 p32 | |
| F01-W0111 | 110 | Brass Ens-1 LP | - | D1 p32 | |
| F01-W0112 | 111 | Brass Ens-2 | - | D1 p32 | |
| F01-W0113 | 112 | Brass Ens-2 LP | - | D1 p32 | |
| F01-W0114 | 113 | Voice-Choir | - | D1 p32 | |
| F01-W0115 | 114 | Voice-Choir REV | - | D1 p32 | |
| F01-W0116 | 115 | Voice-Pop Ooh | - | D1 p32 | |
| F01-W0117 | 116 | Voice-PopOoh REV | - | D1 p32 | |
| F01-W0118 | 117 | Voice-Pop Ah | - | D1 p32 | |
| F01-W0119 | 118 | Voice-Pop Ah REV | - | D1 p32 | |
| F01-W0120 | 119 | Voice-Doo | - | D1 p32 | |
| F01-W0121 | 120 | Voice-Doo LP | - | D1 p32 | |
| F01-W0122 | 121 | Voice Wave 1 | - | D1 p32 | |
| F01-W0123 | 122 | Voice Wave 2 | - | D1 p32 | |
| F01-W0124 | 123 | String Ens. | - | D1 p32 | |
| F01-W0125 | 124 | String Ens. REV | - | D1 p32 | |
| F01-W0126 | 125 | Pizzicato Ens. | - | D1 p32 | |
| F01-W0127 | 126 | Violin | - | D1 p32 | |
| F01-W0128 | 127 | Viola | - | D1 p32 | |
| F01-W0129 | 128 | Cello&Contrabass | - | D1 p32 | |
| F01-W0130 | 129 | String Quartet 1 | - | D1 p32 | |
| F01-W0131 | 130 | String Quartet 2 | - | D1 p32 | |
| F01-W0132 | 131 | String Qurt2 REV | - | D1 p32 | |
| F01-W0133 | 132 | Pizzicato | - | D1 p32 | |
| F01-W0134 | 133 | A.Guitar-Nylon | - | D1 p32 | |
| F01-W0135 | 134 | A.Guitar-Finger | - | D1 p32 | |
| F01-W0136 | 135 | A.Guitar-Pick | - | D1 p32 | |
| F01-W0137 | 136 | A.Gtr-Harmonics | - | D1 p32 | |
| F01-W0138 | 137 | Clean Gtr 1-Stra | - | D1 p32 | |
| F01-W0139 | 138 | Clean Gtr 2-Stra | - | D1 p32 | |
| F01-W0140 | 139 | Clean Gtr 3-Tele | - | D1 p32 | |
| F01-W0141 | 140 | Clean Gtr-Mute 1 | - | D1 p32 | |
| F01-W0142 | 141 | Clean Gtr-Mute 2 | - | D1 p32 | |
| F01-W0143 | 142 | Funky Gtr 1-Stra | - | D1 p32 | |
| F01-W0144 | 143 | Funky Gtr 2-Stra | - | D1 p32 | |
| F01-W0145 | 144 | Jazz Guitar | - | D1 p32 | |
| F01-W0146 | 145 | Pedal Steel Gtr. | - | D1 p32 | |
| F01-W0147 | 146 | E.Gtr-Harmonics | - | D1 p32 | |
| F01-W0148 | 147 | OverDrive Guitar | - | D1 p32 | |
| F01-W0149 | 148 | Dist. Guitar 1 | - | D1 p32 | |
| F01-W0150 | 149 | Dist. Guitar 2 | - | D1 p32 | |
| F01-W0151 | 150 | Dist.Gtr-Harmo | - | D1 p32 | |
| F01-W0152 | 151 | Dist.Gtr-Mute | - | D1 p32 | |
| F01-W0153 | 152 | Power Chord | - | D1 p32 | |
| F01-W0154 | 153 | Fret Noise | - | D1 p32 | |
| F01-W0155 | 154 | A.Bass 1 | - | D1 p32 | |
| F01-W0156 | 155 | A.Bass 2 | - | D1 p32 | |
| F01-W0157 | 156 | A.Bass 2 LP | - | D1 p32 | |
| F01-W0158 | 157 | FingerBass-Clear | - | D1 p32 | |
| F01-W0159 | 158 | Finger Bass 1 | - | D1 p32 | |
| F01-W0160 | 159 | Finger Bass 1 LP | - | D1 p32 | |
| F01-W0161 | 160 | Finger Bass 2 | - | D1 p32 | |
| F01-W0162 | 161 | Finger Bass 2 LP | - | D1 p32 | |
| F01-W0163 | 162 | Pick Bass-M1 | - | D1 p32 | |
| F01-W0164 | 163 | Pick Bass-Hard | - | D1 p32 | |
| F01-W0165 | 164 | Pick Bass | - | D1 p32 | |
| F01-W0166 | 165 | Pick Bass LP | - | D1 p32 | |
| F01-W0167 | 166 | SlapBass-ThumbJB | - | D1 p32 | |
| F01-W0168 | 167 | SlapBass-Thumb 1 | - | D1 p32 | |
| F01-W0169 | 168 | SlapBass-Thum1LP | - | D1 p32 | |
| F01-W0170 | 169 | SlapBass-Thumb 2 | - | D1 p32 | |
| F01-W0171 | 170 | SlapBass-Thum2LP | - | D1 p32 | |
| F01-W0172 | 171 | SlapBass-Pull JB | - | D1 p32 | |
| F01-W0173 | 172 | SlapBass-Pull | - | D1 p32 | |
| F01-W0174 | 173 | SlapBass-Mute | - | D1 p32 | |
| F01-W0175 | 174 | SlapBass-Mute LP | - | D1 p32 | |
| F01-W0176 | 175 | SlapBass-M1 | - | D1 p32 | |
| F01-W0177 | 176 | Fretless Bass | - | D1 p32 | |
| F01-W0178 | 177 | Fretless Bass-M1 | - | D1 p32 | |
| F01-W0179 | 178 | Bass Harmonics | - | D1 p32 | |
| F01-W0180 | 179 | Bass Harmo LP | - | D1 p32 | |
| F01-W0181 | 180 | Sitar | - | D1 p32 | |
| F01-W0182 | 181 | Sitar&Tambura | - | D1 p32 | |
| F01-W0183 | 182 | Sitar&Tambura LP | - | D1 p32 | |
| F01-W0184 | 183 | Santur | - | D1 p32 | |
| F01-W0185 | 184 | Santur LP | - | D1 p32 | |
| F01-W0186 | 185 | Mandolin | - | D1 p32 | |
| F01-W0187 | 186 | Mandolin LP | - | D1 p32 | |
| F01-W0188 | 187 | Banjo | - | D1 p32 | |
| F01-W0189 | 188 | Banjo LP | - | D1 p32 | |
| F01-W0190 | 189 | Shamisen | - | D1 p32 | |
| F01-W0191 | 190 | Koto | - | D1 p32 | |
| F01-W0192 | 191 | Harp | - | D1 p32 | |
| F01-W0193 | 192 | Harp LP | - | D1 p32 | |
| F01-W0194 | 193 | Harp-Glissando | - | D1 p32 | |
| F01-W0195 | 194 | Harp-Gliss REV | - | D1 p32 | |
| F01-W0196 | 195 | Sine Pop Bass | - | D1 p32 | |
| F01-W0197 | 196 | Syn Bass-Reso 1 | - | D1 p32 | |
| F01-W0198 | 197 | Syn Bass-Reso 2 | - | D1 p32 | |
| F01-W0199 | 198 | Syn Bass-Dirty | - | D1 p32 | |
| F01-W0200 | 199 | Syn Bass-FM 1 | - | D1 p32 | |
| F01-W0201 | 200 | Syn Bass-FM 1 LP | - | D1 p32 | |
| F01-W0202 | 201 | Syn Bass-FM 2 | - | D1 p32 | |
| F01-W0203 | 202 | Syn Bass-FM 2 LP | - | D1 p32 | |
| F01-W0204 | 203 | Syn Bass-TB | - | D1 p32 | |
| F01-W0205 | 204 | Syn Bass-RBSaw | - | D1 p32 | |
| F01-W0206 | 205 | Syn Bass-RBSqr | - | D1 p32 | |
| F01-W0207 | 206 | Sync | - | D1 p32 | |
| F01-W0208 | 207 | ChromRes | - | D1 p32 | |
| F01-W0209 | 208 | Syn-Pop | - | D1 p32 | |
| F01-W0210 | 209 | Detuned-Super | - | D1 p32 | |
| F01-W0211 | 210 | Detuned-SuperREV | - | D1 p32 | |
| F01-W0212 | 211 | Detuned-Super 2 | - | D1 p32 | |
| F01-W0213 | 212 | Detun-Super2 REV | - | D1 p32 | |
| F01-W0214 | 213 | Detuned-PWM | - | D1 p32 | |
| F01-W0215 | 214 | Detuned-PWM REV | - | D1 p32 | |
| F01-W0216 | 215 | Analog Strings 1 | - | D1 p32 | |
| F01-W0217 | 216 | AnaStrings 1 REV | - | D1 p32 | |
| F01-W0218 | 217 | Analog Strings 2 | - | D1 p32 | |
| F01-W0219 | 218 | AnaStrings 2 REV | - | D1 p32 | |
| F01-W0220 | 219 | White Pad | - | D1 p32 | |
| F01-W0221 | 220 | White Pad REV | - | D1 p32 | |
| F01-W0222 | 221 | Glass Vox | - | D1 p32 | |
| F01-W0223 | 222 | Glass Vox REV | - | D1 p32 | |
| F01-W0224 | 223 | Syn-Air Vox | - | D1 p32 | |
| F01-W0225 | 224 | Syn-Air Vox REV | - | D1 p32 | |
| F01-W0226 | 225 | Syn-Ghostly | - | D1 p32 | |
| F01-W0227 | 226 | Syn-Ghostly REV | - | D1 p32 | |
| F01-W0228 | 227 | Syn-Air Pad | - | D1 p32 | |
| F01-W0229 | 228 | Syn-Air Pad REV | - | D1 p32 | |
| F01-W0230 | 229 | Syn-Flute Pad | - | D1 p32 | |
| F01-W0231 | 230 | Syn-FlutePad REV | - | D1 p32 | |
| F01-W0232 | 231 | Syn-DreamStrings | - | D1 p32 | |
| F01-W0233 | 232 | Syn-DreamStr REV | - | D1 p32 | |
| F01-W0234 | 233 | Syn-Golithan | - | D1 p32 | |
| F01-W0235 | 234 | Syn-Golithan REV | - | D1 p32 | |
| F01-W0236 | 235 | Syn-Air Vortex | - | D1 p32 | |
| F01-W0237 | 236 | Syn-AirVortexREV | - | D1 p32 | |
| F01-W0238 | 237 | Syn-FX01Z | - | D1 p32 | |
| F01-W0239 | 238 | Syn-FX01Z REV | - | D1 p32 | |
| F01-W0240 | 239 | Syn-Palawan | - | D1 p32 | |
| F01-W0241 | 240 | Syn-Palawan REV | - | D1 p32 | |
| F01-W0242 | 241 | WaveSweep | - | D1 p32 | |
| F01-W0243 | 242 | WaveSweepA | - | D1 p32 | |
| F01-W0244 | 243 | WaveSweepA REV | - | D1 p32 | |
| F01-W0245 | 244 | WaveSweepB | - | D1 p32 | |
| F01-W0246 | 245 | Syn-Clicker | - | D1 p32 | |
| F01-W0247 | 246 | Syn-Clicker REV | - | D1 p32 | |
| F01-W0248 | 247 | Xylo-Spectrum | - | D1 p32 | |
| F01-W0249 | 248 | Cricket Spectrum | - | D1 p32 | |
| F01-W0250 | 249 | Syn-FX07Z | - | D1 p32 | |
| F01-W0251 | 250 | Swish Terra | - | D1 p32 | |
| F01-W0252 | 251 | Noise | - | D1 p32 | |
| F01-W0253 | 252 | Noise Spectrum 1 | - | D1 p32 | |
| F01-W0254 | 253 | Noise Spectrum 2 | - | D1 p32 | |
| F01-W0255 | 254 | Space Lore | - | D1 p32 | |
| F01-W0256 | 255 | Space Lore REV | - | D1 p32 | |
| F01-W0257 | 256 | Telephone Ring | - | D1 p32 | |
| F01-W0258 | 257 | Tele Ring REV | - | D1 p32 | |
| F01-W0259 | 258 | Syn-FX02Z | - | D1 p32 | |
| F01-W0260 | 259 | Syn-FX02Z REV | - | D1 p32 | |
| F01-W0261 | 260 | Pole | - | D1 p32 | |
| F01-W0262 | 261 | Pole LP | - | D1 p32 | |
| F01-W0263 | 262 | Gamelan Wave | - | D1 p32 | |
| F01-W0264 | 263 | Saw | - | D1 p32 | |
| F01-W0265 | 264 | Saw-MG3 | - | D1 p32 | |
| F01-W0266 | 265 | Saw-Chroma | - | D1 p32 | |
| F01-W0267 | 266 | Pulse-02% | - | D1 p32 | |
| F01-W0268 | 267 | Pulse-05% | - | D1 p32 | |
| F01-W0269 | 268 | Pulse-08% | - | D1 p32 | |
| F01-W0270 | 269 | Pulse-16% | - | D1 p32 | |
| F01-W0271 | 270 | Pulse-33% | - | D1 p32 | |
| F01-W0272 | 271 | Pulse-40% | - | D1 p32 | |
| F01-W0273 | 272 | Square | - | D1 p32 | |
| F01-W0274 | 273 | Sqr-mMG | - | D1 p32 | |
| F01-W0275 | 274 | Square-JP | - | D1 p32 | |
| F01-W0276 | 275 | Triangle-MG3 | - | D1 p32 | |
| F01-W0277 | 276 | Ramp | - | D1 p32 | |
| F01-W0278 | 277 | Ramp-mMG | - | D1 p32 | |
| F01-W0279 | 278 | Sine | - | D1 p32 | |
| F01-W0280 | 279 | Sine-JP | - | D1 p32 | |
| F01-W0281 | 280 | DWGS-Syn Sine 1 | - | D1 p32 | |
| F01-W0282 | 281 | DWGS-Syn Sine 2 | - | D1 p32 | |
| F01-W0283 | 282 | DWGS-E.P.1 | - | D1 p32 | |
| F01-W0284 | 283 | DWGS-E.P.2 | - | D1 p32 | |
| F01-W0285 | 284 | DWGS-E.P.3 | - | D1 p32 | |
| F01-W0286 | 285 | DWGS-E.P.4 | - | D1 p32 | |
| F01-W0287 | 286 | DWGS-E.P.5 | - | D1 p32 | |
| F01-W0288 | 287 | DWGS-Organ 1 | - | D1 p32 | |
| F01-W0289 | 288 | DWGS-Organ 2 | - | D1 p32 | |
| F01-W0290 | 289 | DWGS-Organ 3 | - | D1 p32 | |
| F01-W0291 | 290 | DWGS-Organ 4 | - | D1 p32 | |
| F01-W0292 | 291 | DWGS-Organ 5 | - | D1 p32 | |
| F01-W0293 | 292 | DWGS-Accordion | - | D1 p32 | |
| F01-W0294 | 293 | DWGS Guitar 1 | - | D1 p32 | |
| F01-W0295 | 294 | DWGS-Guitar 2 | - | D1 p32 | |
| F01-W0296 | 295 | DWGS-Bell 1 | - | D1 p32 | |
| F01-W0297 | 296 | DWGS-Bell 2 | - | D1 p32 | |
| F01-W0298 | 297 | DWGS-Bell 3 | - | D1 p32 | |
| F01-W0299 | 298 | DWGS-Bell 4 | - | D1 p32 | |
| F01-W0300 | 299 | DWGS-Clav 1 | - | D1 p32 | |
| F01-W0301 | 300 | DWGS-Clav 2 | - | D1 p32 | |
| F01-W0302 | 301 | DWGS-Clav 3 | - | D1 p32 | |
| F01-W0303 | 302 | DWGS-Digi 1 | - | D1 p32 | |
| F01-W0304 | 303 | DWGS-Digi 2 | - | D1 p32 | |
| F01-W0305 | 304 | DWGS-Digi 3 | - | D1 p32 | |
| F01-W0306 | 305 | DWGS-Wire 1 | - | D1 p32 | |
| F01-W0307 | 306 | DWGS-Wire 2 | - | D1 p32 | |
| F01-W0308 | 307 | DWGS-Sync 1 | - | D1 p32 | |
| F01-W0309 | 308 | DWGS-Sync 2 | - | D1 p32 | |
| F01-W0310 | 309 | DWGS-Sync 3 | - | D1 p32 | |
| F01-W0311 | 310 | Orchestra Hit | - | D1 p32 | |
| F01-W0312 | 311 | Pizz Hit | - | D1 p32 | |
| F01-W0313 | 312 | Band Hit | - | D1 p32 | |
| F01-W0314 | 313 | Impact Hit L | - | D1 p32 | |
| F01-W0315 | 314 | Impact Hit R | - | D1 p32 | |
| F01-W0316 | 315 | Philly Hit | - | D1 p32 | |
| F01-W0317 | 316 | Brass Fall | - | D1 p32 | |
| F01-W0318 | 317 | Organ Hit | - | D1 p32 | |
| F01-W0319 | 318 | FunkyGuitarHit | - | D1 p32 | |
| F01-W0320 | 319 | Pop Guitar Hit | - | D1 p32 | |
| F01-W0321 | 320 | ReggaeGuitar Hit | - | D1 p32 | |
| F01-W0322 | 321 | Vox-Wah Gtr | - | D1 p32 | |
| F01-W0323 | 322 | GtCutNois 1 | - | D1 p32 | |
| F01-W0324 | 323 | GtCutNois 2 | - | D1 p32 | |
| F01-W0325 | 324 | String Slap | - | D1 p32 | |
| F01-W0326 | 325 | Voice Set | - | D1 p32 | |
| F01-W0327 | 326 | Voice *Do it* | - | D1 p32 | |
| F01-W0328 | 327 | Voice *Do it* LP | - | D1 p32 | |
| F01-W0329 | 328 | VibeChord 1 | - | D1 p32 | |
| F01-W0330 | 329 | VibeChord 2 | - | D1 p32 | |
| F01-W0331 | 330 | VibeChord 1&2 | - | D1 p32 | |
| F01-W0332 | 331 | Cyber Drum | - | D1 p32 | |
| F01-W0333 | 332 | Cyber Drum RPT | - | D1 p32 | |
| F01-W0334 | 333 | Industry | - | D1 p32 | |
| F01-W0335 | 334 | Industry RPT | - | D1 p32 | |
| F01-W0336 | 335 | SD-Metal Striker | - | D1 p32 | |
| F01-W0337 | 336 | SD-M.Striker RPT | - | D1 p32 | |
| F01-W0338 | 337 | SD-Air Explosion | - | D1 p32 | |
| F01-W0339 | 338 | Metal Hit 4 | - | D1 p32 | |
| F01-W0340 | 339 | Metal Hit 4 RPT | - | D1 p32 | |
| F01-W0341 | 340 | Industry-66 | - | D1 p32 | |
| F01-W0342 | 341 | Industry-66 RPT | - | D1 p32 | |
| F01-W0343 | 342 | Industry-21 | - | D1 p32 | |
| F01-W0344 | 343 | BD-Cannon | - | D1 p32 | |
| F01-W0345 | 344 | Zap 1 | - | D1 p32 | |
| F01-W0346 | 345 | Zap 2 | - | D1 p32 | |
| F01-W0347 | 346 | Scratch Hi | - | D1 p32 | |
| F01-W0348 | 347 | DJ-Old Record | - | D1 p32 | |
| F01-W0349 | 348 | Stadium | - | D1 p32 | |
| F01-W0350 | 349 | Applause | - | D1 p32 | |
| F01-W0351 | 350 | Birds 1 | - | D1 p32 | |
| F01-W0352 | 351 | Birds 1 REV | - | D1 p32 | |
| F01-W0353 | 352 | Bird 2 | - | D1 p32 | |
| F01-W0354 | 353 | Crickets 1 | - | D1 p32 | |
| F01-W0355 | 354 | Crickets 1 REV | - | D1 p32 | |
| F01-W0356 | 355 | Crickets 2 | - | D1 p32 | |
| F01-W0357 | 356 | ChurchBell | - | D1 p32 | |
| F01-W0358 | 357 | ChurchBell REV | - | D1 p32 | |
| F01-W0359 | 358 | Thunder | - | D1 p32 | |
| F01-W0360 | 359 | Stream | - | D1 p32 | |
| F01-W0361 | 360 | Bubble | - | D1 p32 | |
| F01-W0362 | 361 | Bubble REV | - | D1 p32 | |
| F01-W0363 | 362 | Dog | - | D1 p32 | |
| F01-W0364 | 363 | Gallop | - | D1 p32 | |
| F01-W0365 | 364 | Gallop REV | - | D1 p32 | |
| F01-W0366 | 365 | Laughing | - | D1 p32 | |
| F01-W0367 | 366 | Scream | - | D1 p32 | |
| F01-W0368 | 367 | Punch | - | D1 p32 | |
| F01-W0369 | 368 | Heart Beat | - | D1 p32 | |
| F01-W0370 | 369 | Footstep 1 | - | D1 p32 | |
| F01-W0371 | 370 | Footstep 2 | - | D1 p32 | |
| F01-W0372 | 371 | Door Creak | - | D1 p32 | |
| F01-W0373 | 372 | Door Slam | - | D1 p32 | |
| F01-W0374 | 373 | Car Engine | - | D1 p32 | |
| F01-W0375 | 374 | Car Engine LP | - | D1 p32 | |
| F01-W0376 | 375 | Car Stop | - | D1 p32 | |
| F01-W0377 | 376 | Car Pass | - | D1 p32 | |
| F01-W0378 | 377 | Car Crash | - | D1 p32 | |
| F01-W0379 | 378 | Train | - | D1 p32 | |
| F01-W0380 | 379 | Train REV | - | D1 p32 | |
| F01-W0381 | 380 | Helicopter | - | D1 p32 | |
| F01-W0382 | 381 | Helicopter REV | - | D1 p32 | |
| F01-W0383 | 382 | Gun Shot 2 | - | D1 p32 | |
| F01-W0384 | 383 | Machine Gun | - | D1 p32 | |
| F01-W0385 | 384 | Machine Gun REV | - | D1 p32 | |
| F01-W0386 | 385 | Laser Gun | - | D1 p32 | |
| F01-W0387 | 386 | Explosion | - | D1 p32 | |
| F01-W0388 | 387 | Wind | - | D1 p32 | |
| F01-W0389 | 388 | Timpani | - | D1 p32 | |
| F01-W0390 | 389 | Crash 1 | - | D1 p32 | |
| F01-W0391 | 390 | Crash 2 | - | D1 p32 | |
| F01-W0392 | 391 | Cymbal Reverse | - | D1 p32 | |
| F01-W0393 | 392 | Orch. Cymbal | - | D1 p32 | |
| F01-W0394 | 393 | Ride-Jazz | - | D1 p32 | |
| F01-W0395 | 394 | Ride-Edge 1 | - | D1 p32 | |
| F01-W0396 | 395 | Ride-Edge 2 | - | D1 p32 | |
| F01-W0397 | 396 | H.Hat-Closed | - | D1 p32 | |
| F01-W0398 | 397 | 88-HHat Open | - | D1 p32 | |
| F01-W0399 | 398 | 88-Cowbell | - | D1 p32 | |
| F01-W0400 | 399 | 88-Tom | - | D1 p32 | |
| F01-W0401 | 400 | 88-Conga | - | D1 p32 | |
| F01-W0402 | 401 | 88-Crash | - | D1 p32 | |
| F01-W0403 | 402 | Tom | - | D1 p32 | |
| F01-W0404 | 403 | Tom-Brush | - | D1 p32 | |
| F01-W0405 | 404 | Tom-Processed | - | D1 p32 | |
| F01-W0406 | 405 | Real E.Tom | - | D1 p32 | |
| F01-W0407 | 406 | Flexatone | - | D1 p32 | |
| F01-W0408 | 407 | Tambourine | - | D1 p32 | |
| F01-W0409 | 408 | Tambourine-RPT | - | D1 p32 | |
| F01-W0410 | 409 | Agogo Bell | - | D1 p32 | |
| F01-W0411 | 410 | Cowbell | - | D1 p32 | |
| F01-W0412 | 411 | Marc Tree | - | D1 p32 | |
| F01-W0413 | 412 | Marc Tree LP | - | D1 p32 | |
| F01-W0414 | 413 | Bell Tree | - | D1 p32 | |
| F01-W0415 | 414 | Bell Tree LP | - | D1 p32 | |
| F01-W0416 | 415 | Rainstick | - | D1 p32 | |
| F01-W0417 | 416 | TempleBlocks | - | D1 p32 | |
| F01-W0418 | 417 | WoodBlock | - | D1 p32 | |
| F01-W0419 | 418 | Click | - | D1 p32 | |
| F01-W0420 | 419 | Castanet | - | D1 p32 | |
| F01-W0421 | 420 | Taiko | - | D1 p32 | |
| F01-W0422 | 421 | Orch BD | - | D1 p32 | |
| F01-W0423 | 422 | Djembe-Open | - | D1 p32 | |
| F01-W0424 | 423 | Djembe-Mute | - | D1 p32 | |
| F01-W0425 | 424 | Chinese Gong | - | D1 p32 | |

### Drumsamples

| Shipped label | Number | Documented name | Group | Source | Note |
|---|---|---|---|---|---|
| F01-W0001 | 0 | BD-Dry 1 | - | D1 p33 | |
| F01-W0002 | 1 | BD-Dry 2 | - | D1 p33 | |
| F01-W0003 | 2 | BD-Dry 3 | - | D1 p33 | |
| F01-W0004 | 3 | BD-Dry 4 | - | D1 p33 | |
| F01-W0005 | 4 | BD-Dry 5 | - | D1 p33 | |
| F01-W0006 | 5 | BD-Dry 6 | - | D1 p33 | |
| F01-W0007 | 6 | BD-Soft | - | D1 p33 | |
| F01-W0008 | 7 | BD-Soft Room | - | D1 p33 | |
| F01-W0009 | 8 | BD-Jazz 1 | - | D1 p33 | |
| F01-W0010 | 9 | BD-Jazz 2 | - | D1 p33 | |
| F01-W0011 | 10 | BD-Pillow | - | D1 p33 | |
| F01-W0012 | 11 | BD-Woofer | - | D1 p33 | |
| F01-W0013 | 12 | BD-Mondo Kill | - | D1 p33 | |
| F01-W0014 | 13 | BD-Terminator | - | D1 p33 | |
| F01-W0015 | 14 | BD-Tubby | - | D1 p33 | |
| F01-W0016 | 15 | BD-Gated | - | D1 p33 | |
| F01-W0017 | 16 | BD-Tight | - | D1 p33 | |
| F01-W0018 | 17 | BD-Squash | - | D1 p33 | |
| F01-W0019 | 18 | BD-Dance 1 | - | D1 p33 | |
| F01-W0020 | 19 | BD-Dance 2 | - | D1 p33 | |
| F01-W0021 | 20 | BD-Dance 3 | - | D1 p33 | |
| F01-W0022 | 21 | BD-Dance 4 | - | D1 p33 | |
| F01-W0023 | 22 | BD-Hip 1 | - | D1 p33 | |
| F01-W0024 | 23 | BD-Hip 2 | - | D1 p33 | |
| F01-W0025 | 24 | BD-Hip 3 | - | D1 p33 | |
| F01-W0026 | 25 | BD-Hip 4 | - | D1 p33 | |
| F01-W0027 | 26 | BD-Dark | - | D1 p33 | |
| F01-W0028 | 27 | BD-Pop | - | D1 p33 | |
| F01-W0029 | 28 | BD-Ringy | - | D1 p33 | |
| F01-W0030 | 29 | BD-Short88 | - | D1 p33 | |
| F01-W0031 | 30 | BD-Dance99 | - | D1 p33 | |
| F01-W0032 | 31 | BD-Ambi | - | D1 p33 | |
| F01-W0033 | 32 | BD-Ambi Crackle | - | D1 p33 | |
| F01-W0034 | 33 | BD-Ambi Soft | - | D1 p33 | |
| F01-W0035 | 34 | BD-Dance Soft | - | D1 p33 | |
| F01-W0036 | 35 | BD-AmbiRocker | - | D1 p33 | |
| F01-W0037 | 36 | BD-Chili | - | D1 p33 | |
| F01-W0038 | 37 | BD-PhatJaz | - | D1 p33 | |
| F01-W0039 | 38 | BD-Cannon | - | D1 p33 | |
| F01-W0040 | 39 | BD-Klanger | - | D1 p33 | |
| F01-W0041 | 40 | SD-Dry 1 | - | D1 p33 | |
| F01-W0042 | 41 | SD-Dry 2 | - | D1 p33 | |
| F01-W0043 | 42 | SD-Dry 3 | - | D1 p33 | |
| F01-W0044 | 43 | SD-Normal | - | D1 p33 | |
| F01-W0045 | 44 | SD-GhostSet roll | - | D1 p33 | |
| F01-W0046 | 45 | SD-GhostSet f | - | D1 p33 | |
| F01-W0047 | 46 | SD-GhostSet p1 | - | D1 p33 | |
| F01-W0048 | 47 | SD-GhostSet p2 | - | D1 p33 | |
| F01-W0049 | 48 | SD-Full Room | - | D1 p33 | |
| F01-W0050 | 49 | SD-Off Center | - | D1 p33 | |
| F01-W0051 | 50 | SD-Jazz Ring | - | D1 p33 | |
| F01-W0052 | 51 | SD-Amb.Piccolo 1 | - | D1 p33 | |
| F01-W0053 | 52 | SD-Amb.Piccolo 2 | - | D1 p33 | |
| F01-W0054 | 53 | SD-Paper | - | D1 p33 | |
| F01-W0055 | 54 | SD-Classic Room | - | D1 p33 | |
| F01-W0056 | 55 | SD-Brush Hit | - | D1 p33 | |
| F01-W0057 | 56 | SD-Brush Tap 1 | - | D1 p33 | |
| F01-W0058 | 57 | SD-Brush Tap 2 | - | D1 p33 | |
| F01-W0059 | 58 | SD-Brush Swirl | - | D1 p33 | |
| F01-W0060 | 59 | SD-Atomic | - | D1 p33 | |
| F01-W0061 | 60 | SD-Big Rock | - | D1 p33 | |
| F01-W0062 | 61 | SD-Yowie | - | D1 p33 | |
| F01-W0063 | 62 | SD-Processed 1 | - | D1 p33 | |
| F01-W0064 | 63 | SD-Processed 2 | - | D1 p33 | |
| F01-W0065 | 64 | SD-Cracker Room | - | D1 p33 | |
| F01-W0066 | 65 | Sidestick-Dry | - | D1 p33 | |
| F01-W0067 | 66 | Sidestick-Amb | - | D1 p33 | |
| F01-W0068 | 67 | SD-Amb.House | - | D1 p33 | |
| F01-W0069 | 68 | SD-Dance | - | D1 p33 | |
| F01-W0070 | 69 | SD-Rap 1 | - | D1 p33 | |
| F01-W0071 | 70 | SD-Rap 2 | - | D1 p33 | |
| F01-W0072 | 71 | SD-Noise | - | D1 p33 | |
| F01-W0073 | 72 | SD-Sizzle | - | D1 p33 | |
| F01-W0074 | 73 | SD-Reverse | - | D1 p33 | |
| F01-W0075 | 74 | SD-Hip 1 | - | D1 p33 | |
| F01-W0076 | 75 | SD-Hip 2 | - | D1 p33 | |
| F01-W0077 | 76 | SD-Hip 3 | - | D1 p33 | |
| F01-W0078 | 77 | SD-Hip 4 | - | D1 p33 | |
| F01-W0079 | 78 | SD-Hip 5 | - | D1 p33 | |
| F01-W0080 | 79 | SD-Hip 6 | - | D1 p33 | |
| F01-W0081 | 80 | SD-Ringy | - | D1 p33 | |
| F01-W0082 | 81 | SD-Tiny | - | D1 p33 | |
| F01-W0083 | 82 | SD-Vintage 1 | - | D1 p33 | |
| F01-W0084 | 83 | SD-Vintage 2 | - | D1 p33 | |
| F01-W0085 | 84 | SD-Vintage 3 | - | D1 p33 | |
| F01-W0086 | 85 | SD-Vintage 4 | - | D1 p33 | |
| F01-W0087 | 86 | SD-Vintage 5 | - | D1 p33 | |
| F01-W0088 | 87 | SD-Vintage 6 | - | D1 p33 | |
| F01-W0089 | 88 | SD-AmbiCrackle 1 | - | D1 p33 | |
| F01-W0090 | 89 | SD-AmbiCrackle 2 | - | D1 p33 | |
| F01-W0091 | 90 | SD-AmbiCrackle 3 | - | D1 p33 | |
| F01-W0092 | 91 | SD-AmbiHop | - | D1 p33 | |
| F01-W0093 | 92 | SD-Brasser | - | D1 p33 | |
| F01-W0094 | 93 | SD-Chili | - | D1 p33 | |
| F01-W0095 | 94 | SD-Klanger | - | D1 p33 | |
| F01-W0096 | 95 | SD-Whopper | - | D1 p33 | |
| F01-W0097 | 96 | SD-Dry Present | - | D1 p33 | |
| F01-W0098 | 97 | SD-Ambi Pop | - | D1 p33 | |
| F01-W0099 | 98 | SD-Ambi Hit 1 | - | D1 p33 | |
| F01-W0100 | 99 | SD-Ambi Hit 2 | - | D1 p33 | |
| F01-W0101 | 100 | SD-Ambi Hit 3 | - | D1 p33 | |
| F01-W0102 | 101 | Tom 1-Hi | - | D1 p33 | |
| F01-W0103 | 102 | Tom 1-Lo | - | D1 p33 | |
| F01-W0104 | 103 | Tom 1-Floor | - | D1 p33 | |
| F01-W0105 | 104 | Tom 2-Hi | - | D1 p33 | |
| F01-W0106 | 105 | Tom 2-Lo | - | D1 p33 | |
| F01-W0107 | 106 | Tom 2-Floor | - | D1 p33 | |
| F01-W0108 | 107 | Tom 3-Hi | - | D1 p33 | |
| F01-W0109 | 108 | Tom 3-Lo | - | D1 p33 | |
| F01-W0110 | 109 | Tom 3-Floor | - | D1 p33 | |
| F01-W0111 | 110 | Tom-Processed | - | D1 p33 | |
| F01-W0112 | 111 | Tom-Jazz Hi | - | D1 p33 | |
| F01-W0113 | 112 | Tom-Jazz Hi Rim | - | D1 p33 | |
| F01-W0114 | 113 | Tom-Jazz Floor | - | D1 p33 | |
| F01-W0115 | 114 | Tom-Brush Hi | - | D1 p33 | |
| F01-W0116 | 115 | Tom-Brush Floor | - | D1 p33 | |
| F01-W0117 | 116 | Tom-Dirty Funk | - | D1 p33 | |
| F01-W0118 | 117 | Tom-Old R&B | - | D1 p33 | |
| F01-W0119 | 118 | H.Hat 1-Closed 1 | - | D1 p33 | |
| F01-W0120 | 119 | H.Hat 1-Closed 2 | - | D1 p33 | |
| F01-W0121 | 120 | H.Hat 1-Foot | - | D1 p33 | |
| F01-W0122 | 121 | H.Hat 1-FootOpen | - | D1 p33 | |
| F01-W0123 | 122 | H.Hat 1-Open 1 | - | D1 p33 | |
| F01-W0124 | 123 | H.Hat 1-Open 2 | - | D1 p33 | |
| F01-W0125 | 124 | H.Hat 1-Sizzle | - | D1 p33 | |
| F01-W0126 | 125 | H.Hat 2-Closed 1 | - | D1 p33 | |
| F01-W0127 | 126 | H.Hat 2-Closed 2 | - | D1 p33 | |
| F01-W0128 | 127 | H.Hat 2-Foot | - | D1 p33 | |
| F01-W0129 | 128 | H.Hat 2-FootOpen | - | D1 p33 | |
| F01-W0130 | 129 | H.Hat 2-Open 1 | - | D1 p33 | |
| F01-W0131 | 130 | H.Hat 2-Open 2 | - | D1 p33 | |
| F01-W0132 | 131 | H.Hat 2-Sizzle | - | D1 p33 | |
| F01-W0133 | 132 | H.Hat-Chili | - | D1 p33 | |
| F01-W0134 | 133 | H.Hat-Crisp Cl 1 | - | D1 p33 | |
| F01-W0135 | 134 | H.Hat-Crisp Cl 2 | - | D1 p33 | |
| F01-W0136 | 135 | H.Hat-Crisp Open | - | D1 p33 | |
| F01-W0137 | 136 | H.Hat-Grange | - | D1 p33 | |
| F01-W0138 | 137 | H.Hat-Vintage 1 | - | D1 p33 | |
| F01-W0139 | 138 | H.Hat-Vintage 2 | - | D1 p33 | |
| F01-W0140 | 139 | H.Hat-Tight 1 | - | D1 p33 | |
| F01-W0141 | 140 | H.Hat-Tight 2 | - | D1 p33 | |
| F01-W0142 | 141 | H.Hat-Old Cl 1 | - | D1 p33 | |
| F01-W0143 | 142 | H.Hat-Old Cl 2 | - | D1 p33 | |
| F01-W0144 | 143 | H.Hat-Old Cl 3 | - | D1 p33 | |
| F01-W0145 | 144 | H.Hat-Old Open | - | D1 p33 | |
| F01-W0146 | 145 | H.Hat-Hip 1 | - | D1 p33 | |
| F01-W0147 | 146 | H.Hat-Hip 2 | - | D1 p33 | |
| F01-W0148 | 147 | H.Hat-AmbCrackle | - | D1 p33 | |
| F01-W0149 | 148 | H.Hat-Ambi | - | D1 p33 | |
| F01-W0150 | 149 | H.Hat-ClangyOpen | - | D1 p33 | |
| F01-W0151 | 150 | H.Hat-Whispy | - | D1 p33 | |
| F01-W0152 | 151 | H.Hat-Alpo Close | - | D1 p33 | |
| F01-W0153 | 152 | Crash Cymbal 1 | - | D1 p33 | |
| F01-W0154 | 153 | Crash Cymbal 2 | - | D1 p33 | |
| F01-W0155 | 154 | Crash Cymbal 3 | - | D1 p33 | |
| F01-W0156 | 155 | China Cymbal | - | D1 p33 | |
| F01-W0157 | 156 | Splash Cymbal | - | D1 p33 | |
| F01-W0158 | 157 | Cymbal Reverse | - | D1 p33 | |
| F01-W0159 | 158 | Crash-Dance 99 | - | D1 p33 | |
| F01-W0160 | 159 | Crash-Dance DDD1 | - | D1 p33 | |
| F01-W0161 | 160 | Ride-Dance 99 | - | D1 p33 | |
| F01-W0162 | 161 | Ride-Dance DDD1 | - | D1 p33 | |
| F01-W0163 | 162 | Ride-Edge 1 | - | D1 p33 | |
| F01-W0164 | 163 | Ride-Edge 2 | - | D1 p33 | |
| F01-W0165 | 164 | Ride-Jazz | - | D1 p33 | |
| F01-W0166 | 165 | Ride-Cup | - | D1 p33 | |
| F01-W0167 | 166 | BD-Orchestral | - | D1 p33 | |
| F01-W0168 | 167 | Orch Cym.-Closed | - | D1 p33 | |
| F01-W0169 | 168 | Orch Cym.-Open | - | D1 p33 | |
| F01-W0170 | 169 | Timpani | - | D1 p33 | |
| F01-W0171 | 170 | SD-Orch.Roll 1 | - | D1 p33 | |
| F01-W0172 | 171 | SD-Orch.Roll 2 | - | D1 p33 | |
| F01-W0173 | 172 | SD-Orchestral | - | D1 p33 | |
| F01-W0174 | 173 | 88-BD 1(Rap) | - | D1 p33 | |
| F01-W0175 | 174 | 88-BD 2(R&B) | - | D1 p33 | |
| F01-W0176 | 175 | 88-BD 3 | - | D1 p33 | |
| F01-W0177 | 176 | 99-BD 1 | - | D1 p33 | |
| F01-W0178 | 177 | 99-BD 2 | - | D1 p33 | |
| F01-W0179 | 178 | 99-BD 3 | - | D1 p33 | |
| F01-W0180 | 179 | 99-BD 4 | - | D1 p33 | |
| F01-W0181 | 180 | 99-BD 5 | - | D1 p33 | |
| F01-W0182 | 181 | 88-SD 1(New) | - | D1 p33 | |
| F01-W0183 | 182 | 88-SD 2(Old) | - | D1 p33 | |
| F01-W0184 | 183 | 99-SD 1 | - | D1 p33 | |
| F01-W0185 | 184 | 99-SD 2 | - | D1 p33 | |
| F01-W0186 | 185 | 88-Rimshot | - | D1 p33 | |
| F01-W0187 | 186 | 88-Claps | - | D1 p33 | |
| F01-W0188 | 187 | 88-HHat Closed | - | D1 p33 | |
| F01-W0189 | 188 | 88-HHat Open | - | D1 p33 | |
| F01-W0190 | 189 | 99-HHat Closed | - | D1 p33 | |
| F01-W0191 | 190 | 99-HHat Open | - | D1 p33 | |
| F01-W0192 | 191 | 88-Crash | - | D1 p33 | |
| F01-W0193 | 192 | 88-Tom | - | D1 p33 | |
| F01-W0194 | 193 | 88-Conga | - | D1 p33 | |
| F01-W0195 | 194 | 88-Clave | - | D1 p33 | |
| F01-W0196 | 195 | 88-Cowbell | - | D1 p33 | |
| F01-W0197 | 196 | 88-Maraca | - | D1 p33 | |
| F01-W0198 | 197 | FM E.Tom | - | D1 p33 | |
| F01-W0199 | 198 | Real E.Tom | - | D1 p33 | |
| F01-W0200 | 199 | Vocal SD | - | D1 p33 | |
| F01-W0201 | 200 | Vocal Cowbell | - | D1 p33 | |
| F01-W0202 | 201 | Vocal Woodblock | - | D1 p33 | |
| F01-W0203 | 202 | Dance Perc-Hip | - | D1 p33 | |
| F01-W0204 | 203 | Dance Perc-Hous1 | - | D1 p33 | |
| F01-W0205 | 204 | Dance Perc-Hous2 | - | D1 p33 | |
| F01-W0206 | 205 | Zap 1 | - | D1 p33 | |
| F01-W0207 | 206 | Zap 2 | - | D1 p33 | |
| F01-W0208 | 207 | Zap 3 | - | D1 p33 | |
| F01-W0209 | 208 | DJ-Scratch 1 | - | D1 p33 | |
| F01-W0210 | 209 | DJ-Scratch 2 | - | D1 p33 | |
| F01-W0211 | 210 | DJ-Scratch 3a | - | D1 p33 | |
| F01-W0212 | 211 | DJ-Scratch 3b | - | D1 p33 | |
| F01-W0213 | 212 | DJ-Scratch 3c | - | D1 p33 | |
| F01-W0214 | 213 | DJ-Scratch 4 | - | D1 p33 | |
| F01-W0215 | 214 | DJ-Hit Rub | - | D1 p33 | |
| F01-W0216 | 215 | DJ-1c Vocal Rub | - | D1 p33 | |
| F01-W0217 | 216 | DJ-1c Vocal Rub | - | D1 p33 | |
| F01-W0218 | 217 | DJ-BD Rub | - | D1 p33 | |
| F01-W0219 | 218 | DJ-SD Rub | - | D1 p33 | |
| F01-W0220 | 219 | DJ-Record Stop | - | D1 p33 | |
| F01-W0221 | 220 | DJ-Reverse | - | D1 p33 | |
| F01-W0222 | 221 | DJ-Old Record | - | D1 p33 | |
| F01-W0223 | 222 | Orchestra Hit | - | D1 p33 | |
| F01-W0224 | 223 | Pizz Hit | - | D1 p33 | |
| F01-W0225 | 224 | Band Hit | - | D1 p33 | |
| F01-W0226 | 225 | ImpactHit | - | D1 p33 | |
| F01-W0227 | 226 | Philly Hit | - | D1 p33 | |
| F01-W0228 | 227 | Drumstick Hit | - | D1 p33 | |
| F01-W0229 | 228 | Finger Snap | - | D1 p33 | |
| F01-W0230 | 229 | Hand Claps | - | D1 p33 | |
| F01-W0231 | 230 | Hand Claps-Hip | - | D1 p33 | |
| F01-W0232 | 231 | Hand Claps-Happy | - | D1 p33 | |
| F01-W0233 | 232 | Conga Lo-Open | - | D1 p33 | |
| F01-W0234 | 233 | Conga Lo-MtSlap | - | D1 p33 | |
| F01-W0235 | 234 | Conga Lo-Slap | - | D1 p33 | |
| F01-W0236 | 235 | Conga Hi-Open | - | D1 p33 | |
| F01-W0237 | 236 | Conga Hi-Mute | - | D1 p33 | |
| F01-W0238 | 237 | Conga Hi-MtSlap | - | D1 p33 | |
| F01-W0239 | 238 | Conga Hi-Slap 1 | - | D1 p33 | |
| F01-W0240 | 239 | Conga Hi-Slap 2 | - | D1 p33 | |
| F01-W0241 | 240 | Conga-Heel | - | D1 p33 | |
| F01-W0242 | 241 | Conga-Toe | - | D1 p33 | |
| F01-W0243 | 242 | Bongo Lo-Open | - | D1 p33 | |
| F01-W0244 | 243 | Bongo Lo-Slap | - | D1 p33 | |
| F01-W0245 | 244 | Bongo Lo-Stick | - | D1 p33 | |
| F01-W0246 | 245 | Bongo Hi-Open | - | D1 p33 | |
| F01-W0247 | 246 | Bongo Hi-Slap | - | D1 p33 | |
| F01-W0248 | 247 | Bongo Hi-Stick 1 | - | D1 p33 | |
| F01-W0249 | 248 | Bongo Hi-Stick 2 | - | D1 p33 | |
| F01-W0250 | 249 | Djembe-Open | - | D1 p33 | |
| F01-W0251 | 250 | Djembe-Mute | - | D1 p33 | |
| F01-W0252 | 251 | Djembe-Slap | - | D1 p33 | |
| F01-W0253 | 252 | Castanet-Single | - | D1 p33 | |
| F01-W0254 | 253 | Castanet-Dble 1 | - | D1 p33 | |
| F01-W0255 | 254 | Castanet-Dble 2 | - | D1 p33 | |
| F01-W0256 | 255 | Guiro-Long | - | D1 p33 | |
| F01-W0257 | 256 | Guiro-Short | - | D1 p33 | |
| F01-W0258 | 257 | Guiro-Tap | - | D1 p33 | |
| F01-W0259 | 258 | Maracas-Push | - | D1 p33 | |
| F01-W0260 | 259 | Maracas-Pull | - | D1 p33 | |
| F01-W0261 | 260 | Baya-Open | - | D1 p33 | |
| F01-W0262 | 261 | Baya-Ghe | - | D1 p33 | |
| F01-W0263 | 262 | Baya-Mute 1 | - | D1 p33 | |
| F01-W0264 | 263 | Baya-Mute 2 | - | D1 p33 | |
| F01-W0265 | 264 | Baya-Mute 3 | - | D1 p33 | |
| F01-W0266 | 265 | Baya-Mute 4 | - | D1 p33 | |
| F01-W0267 | 266 | Baya-Mute 5 | - | D1 p33 | |
| F01-W0268 | 267 | Tabla-Na | - | D1 p33 | |
| F01-W0269 | 268 | Tabla-Open | - | D1 p33 | |
| F01-W0270 | 269 | Tabla-Tin | - | D1 p33 | |
| F01-W0271 | 270 | Tabla-Mute 1 | - | D1 p33 | |
| F01-W0272 | 271 | Tabla-Mute 2 | - | D1 p33 | |
| F01-W0273 | 272 | Tabla-Mute 3 | - | D1 p33 | |
| F01-W0274 | 273 | Tabla-Mute 4 | - | D1 p33 | |
| F01-W0275 | 274 | Taiko-Open | - | D1 p33 | |
| F01-W0276 | 275 | Taiko-Rim | - | D1 p33 | |
| F01-W0277 | 276 | Tsuzumi | - | D1 p33 | |
| F01-W0278 | 277 | Vibraslap | - | D1 p33 | |
| F01-W0279 | 278 | Claves | - | D1 p33 | |
| F01-W0280 | 279 | Woodblock 1 | - | D1 p33 | |
| F01-W0281 | 280 | Woodblock 2 | - | D1 p33 | |
| F01-W0282 | 281 | Timbale Lo-Open | - | D1 p33 | |
| F01-W0283 | 282 | Timbale Lo-Mute | - | D1 p33 | |
| F01-W0284 | 283 | Timbale Lo-Rim | - | D1 p33 | |
| F01-W0285 | 284 | Timbale Hi-Edge | - | D1 p33 | |
| F01-W0286 | 285 | Timbale Hi-Rim 1 | - | D1 p33 | |
| F01-W0287 | 286 | Timbale Hi-Rim 2 | - | D1 p33 | |
| F01-W0288 | 287 | Timbale-Paila | - | D1 p33 | |
| F01-W0289 | 288 | Tambourine-Push | - | D1 p33 | |
| F01-W0290 | 289 | Tambourine-Pull | - | D1 p33 | |
| F01-W0291 | 290 | Tambourine-Acc.1 | - | D1 p33 | |
| F01-W0292 | 291 | Tambourine-Acc.2 | - | D1 p33 | |
| F01-W0293 | 292 | Triangle-Open | - | D1 p33 | |
| F01-W0294 | 293 | Triangle-Mute | - | D1 p33 | |
| F01-W0295 | 294 | Triangle-Roll | - | D1 p33 | |
| F01-W0296 | 295 | Cuica-Hi | - | D1 p33 | |
| F01-W0297 | 296 | Cuica-Lo | - | D1 p33 | |
| F01-W0298 | 297 | Shaker 1 | - | D1 p33 | |
| F01-W0299 | 298 | Shaker 2 | - | D1 p33 | |
| F01-W0300 | 299 | Cabasa-Up | - | D1 p33 | |
| F01-W0301 | 300 | Cabasa-Down | - | D1 p33 | |
| F01-W0302 | 301 | Cabasa-Tap | - | D1 p33 | |
| F01-W0303 | 302 | Caxixi-Hard | - | D1 p33 | |
| F01-W0304 | 303 | Caxixi-Soft | - | D1 p33 | |
| F01-W0305 | 304 | Agogo Bell | - | D1 p33 | |
| F01-W0306 | 305 | Cowbell 1 | - | D1 p33 | |
| F01-W0307 | 306 | Cowbell 2 | - | D1 p33 | |
| F01-W0308 | 307 | Chacha Bell | - | D1 p33 | |
| F01-W0309 | 308 | Mambo Bell | - | D1 p33 | |
| F01-W0310 | 309 | Sleigh Bell | - | D1 p33 | |
| F01-W0311 | 310 | Rap Sleighbell | - | D1 p33 | |
| F01-W0312 | 311 | Finger Cymbal | - | D1 p33 | |
| F01-W0313 | 312 | Marc Tree | - | D1 p33 | |
| F01-W0314 | 313 | Marc Tree LP | - | D1 p33 | |
| F01-W0315 | 314 | Bell Tree | - | D1 p33 | |
| F01-W0316 | 315 | Flexatone | - | D1 p33 | |
| F01-W0317 | 316 | Samba Whistle | - | D1 p33 | |
| F01-W0318 | 317 | Chinese Gong | - | D1 p33 | |
| F01-W0319 | 318 | Industry | - | D1 p33 | |
| F01-W0320 | 319 | Cyber Drum | - | D1 p33 | |
| F01-W0321 | 320 | SD-Metal Striker | - | D1 p33 | |
| F01-W0322 | 321 | SD-Air Explosion | - | D1 p33 | |
| F01-W0323 | 322 | Industry Loop 1 | - | D1 p33 | |
| F01-W0324 | 323 | Industry Loop 2 | - | D1 p33 | |
| F01-W0325 | 324 | Metal Hit | - | D1 p33 | |
| F01-W0326 | 325 | Yea Solo | - | D1 p33 | |
| F01-W0327 | 326 | Year solo | - | D1 p33 | |
| F01-W0328 | 327 | Uhh Solo | - | D1 p33 | |
| F01-W0329 | 328 | Hit It | - | D1 p33 | |
| F01-W0330 | 329 | Uhhhh Solo | - | D1 p33 | |
| F01-W0331 | 330 | CompuVoice-Beep | - | D1 p33 | |
| F01-W0332 | 331 | CompuVoice-Noise | - | D1 p33 | |
| F01-W0333 | 332 | CompuVoice-Chi | - | D1 p33 | |
| F01-W0334 | 333 | CompuVoice-Test | - | D1 p33 | |
| F01-W0335 | 334 | CompuVoice-Do it | - | D1 p33 | |
| F01-W0336 | 335 | CompuVoice-When | - | D1 p33 | |
| F01-W0337 | 336 | CompuVoice-Toi | - | D1 p33 | |
| F01-W0338 | 337 | CompuVoice-Ti | - | D1 p33 | |
| F01-W0339 | 338 | "Four..." Solo | - | D1 p33 | |
| F01-W0340 | 339 | "Three..." Solo | - | D1 p33 | |
| F01-W0341 | 340 | "Two..." Solo | - | D1 p33 | |
| F01-W0342 | 341 | "One..." Solo | - | D1 p33 | |
| F01-W0343 | 342 | PC Vox-Sis | - | D1 p33 | |
| F01-W0344 | 343 | PC Vox-Dunc | - | D1 p33 | |
| F01-W0345 | 344 | PC Vox-Cks | - | D1 p33 | |
| F01-W0346 | 345 | PC Vox-For | - | D1 p33 | |
| F01-W0347 | 346 | PC Vox-One | - | D1 p33 | |
| F01-W0348 | 347 | PC Vox-If | - | D1 p33 | |
| F01-W0349 | 348 | PC Vox-Zee | - | D1 p33 | |
| F01-W0350 | 349 | PC Vox-Ae | - | D1 p33 | |
| F01-W0351 | 350 | PC Vox-Pa | - | D1 p33 | |
| F01-W0352 | 351 | PC Vox-Sh | - | D1 p33 | |
| F01-W0353 | 352 | PC Vox-O | - | D1 p33 | |
| F01-W0354 | 353 | PC Vox-Tu | - | D1 p33 | |
| F01-W0355 | 354 | Stadium | - | D1 p33 | |
| F01-W0356 | 355 | Applause | - | D1 p33 | |
| F01-W0357 | 356 | Scream | - | D1 p33 | |
| F01-W0358 | 357 | Laughing | - | D1 p33 | |
| F01-W0359 | 358 | Footstep 1 | - | D1 p33 | |
| F01-W0360 | 359 | Footstep 2 | - | D1 p33 | |
| F01-W0361 | 360 | Click | - | D1 p33 | |
| F01-W0362 | 361 | Bird 1 | - | D1 p33 | |
| F01-W0363 | 362 | Bird 2 | - | D1 p33 | |
| F01-W0364 | 363 | Dog | - | D1 p33 | |
| F01-W0365 | 364 | Gallop | - | D1 p33 | |
| F01-W0366 | 365 | Crickets 1 | - | D1 p33 | |
| F01-W0367 | 366 | Crickets 2 | - | D1 p33 | |
| F01-W0368 | 367 | Hart Beat | - | D1 p33 | |
| F01-W0369 | 368 | Punch | - | D1 p33 | |
| F01-W0370 | 369 | Tribe | - | D1 p33 | |
| F01-W0371 | 370 | GunShot | - | D1 p33 | |
| F01-W0372 | 371 | Rainstick | - | D1 p33 | |
| F01-W0373 | 372 | Door Creak | - | D1 p33 | |
| F01-W0374 | 373 | Door Slam | - | D1 p33 | |
| F01-W0375 | 374 | Car Engine | - | D1 p33 | |
| F01-W0376 | 375 | Car Stop | - | D1 p33 | |
| F01-W0377 | 376 | Car Pass | - | D1 p33 | |
| F01-W0378 | 377 | Car Crash | - | D1 p33 | |
| F01-W0379 | 378 | Train | - | D1 p33 | |
| F01-W0380 | 379 | Helicopter | - | D1 p33 | |
| F01-W0381 | 380 | Gun Shot 2 | - | D1 p33 | |
| F01-W0382 | 381 | MachineGun | - | D1 p33 | |
| F01-W0383 | 382 | Laser Gun | - | D1 p33 | |
| F01-W0384 | 383 | Explosion | - | D1 p33 | |
| F01-W0385 | 384 | Thunder | - | D1 p33 | |
| F01-W0386 | 385 | Wind | - | D1 p33 | |
| F01-W0387 | 386 | Stream | - | D1 p33 | |
| F01-W0388 | 387 | Bubble | - | D1 p33 | |
| F01-W0389 | 388 | ChurchBell | - | D1 p33 | |
| F01-W0390 | 389 | Telephone Ring | - | D1 p33 | |
| F01-W0391 | 390 | Space Lore | - | D1 p33 | |
| F01-W0392 | 391 | Swish Terra | - | D1 p33 | |
| F01-W0393 | 392 | Xylophone Spectr | - | D1 p33 | |
| F01-W0394 | 393 | Cricket Spectrum | - | D1 p33 | |
| F01-W0395 | 394 | Air Vortex | - | D1 p33 | |
| F01-W0396 | 395 | Noise-White | - | D1 p33 | |
| F01-W0397 | 396 | Noise-FM Mod | - | D1 p33 | |
| F01-W0398 | 397 | Tubular | - | D1 p33 | |
| F01-W0399 | 398 | Gamelan | - | D1 p33 | |
| F01-W0400 | 399 | Tambura | - | D1 p33 | |
| F01-W0401 | 400 | GtCutNois1 | - | D1 p33 | |
| F01-W0402 | 401 | GtCutNois2 | - | D1 p33 | |
| F01-W0403 | 402 | StringSlap | - | D1 p33 | |
| F01-W0404 | 403 | HarpGliss | - | D1 p33 | |
| F01-W0405 | 404 | Power Chord | - | D1 p33 | |
| F01-W0406 | 405 | Fret Noise | - | D1 p33 | |
| F01-W0407 | 406 | Dist.Slide 1 | - | D1 p33 | |
| F01-W0408 | 407 | Dist.Slide 2 | - | D1 p33 | |
| F01-W0409 | 408 | E.Guitar Pick 1 | - | D1 p33 | |
| F01-W0410 | 409 | E.Guitar Pick 2 | - | D1 p33 | |
| F01-W0411 | 410 | E.Guitar Pick 3 | - | D1 p33 | |
| F01-W0412 | 411 | Guitar Scratch | - | D1 p33 | |
| F01-W0413 | 412 | Amp Noise | - | D1 p33 | |

