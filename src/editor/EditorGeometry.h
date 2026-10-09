#pragma once
#include <algorithm>
#include <cmath>
#include <cstring>

namespace keith { namespace ui {

struct Box { float l,t,r,b; constexpr float W() const { return r-l; } constexpr float H() const { return b-t; } constexpr bool Contains(Box o) const { return o.l>=l&&o.t>=t&&o.r<=r&&o.b<=b; } constexpr bool Overlaps(Box o) const { return l<o.r&&o.l<r&&t<o.b&&o.t<b; } };

constexpr float kCanvasW = 1536.0f;
constexpr float kCanvasH = 1024.0f;
constexpr float kLeftRailW = 246.0f;
constexpr float kPanelPadding = 18.0f;
constexpr float kControlGap = 14.0f;
constexpr bool kSingleWindowEditor = true;
constexpr Box kCanvas {0.f, 0.f, kCanvasW, kCanvasH};

// Knob vertical budget from KeithKnobControl::kLabelHeight / kValueHeight.
constexpr float kKnobLabelHeight = 19.f;
constexpr float kKnobValueHeight = 20.f;
constexpr float kKnobBodyMin = 34.f;
constexpr float kKnobMinHeight = kKnobBodyMin + kKnobLabelHeight + kKnobValueHeight;

enum class ControlKind { Knob, Tab, Dropdown, Fader, Pad };

struct PanelSpec { const char* title; Box bounds; const char* tint; };
constexpr float kPanelTitleBand = 28.0f;
constexpr Box kKeyboardBox {358.0f, 1002.0f, 1492.0f, 1020.0f};
inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }
constexpr Box kPanelOsc {250.0f, 24.0f, 552.0f, 416.0f};
constexpr Box kPanelFilter {562.0f, 24.0f, 724.0f, 416.0f};
constexpr Box kPanelAmp {728.0f, 24.0f, 928.0f, 416.0f};
constexpr Box kPanelLfo {938.0f, 24.0f, 1240.0f, 416.0f};
constexpr Box kPanelMod {1250.0f, 24.0f, 1500.0f, 416.0f};
constexpr Box kPanelArt {258.0f, 430.0f, 808.0f, 732.0f};
constexpr Box kPanelAmpMods {734.0f, 432.0f, 868.0f, 728.0f};
constexpr Box kPanelSample {884.0f, 426.0f, 1256.0f, 736.0f};
constexpr Box kPanelPads {1264.0f, 426.0f, 1500.0f, 736.0f};
constexpr Box kPanelVintage {256.0f, 756.0f, 730.0f, 996.0f};
constexpr Box kPanelGlobal {744.0f, 756.0f, 1066.0f, 996.0f};
constexpr Box kPanelFx {1080.0f, 756.0f, 1500.0f, 996.0f};
constexpr Box kLeftRail {0.0f, 0.0f, 246.0f, 1024.0f};
constexpr Box kWordmarkBox {18.0f, 28.0f, 228.0f, 118.0f};
constexpr const char* kLabelAbbreviations[] = {
  "A1",
  "A2",
  "A3",
  "A4",
  "ABND",
  "AKEY",
  "ATT",
  "B1",
  "B2",
  "BYP",
  "C1",
  "C2",
  "CBND",
  "COMP",
  "CUT",
  "D1",
  "D2",
  "D3",
  "D4",
  "DBND",
  "DEC",
  "DEP",
  "DET",
  "DIR",
  "DIST",
  "DKEY",
  "DRV",
  "DST",
  "ENV",
  "FATT",
  "FDEC",
  "FILT",
  "FLT",
  "FLUT",
  "FM",
  "FREL",
  "FSUS",
  "GLIDE",
  "HISS",
  "I1",
  "I2",
  "KEY",
  "KIT",
  "LEG",
  "LVL",
  "M1",
  "M2",
  "MA",
  "MB",
  "MC",
  "MIX",
  "MS",
  "MUT",
  "MWA",
  "MWD",
  "MX",
  "NKEY",
  "NOISE",
  "OUT",
  "PAD",
  "PAMT",
  "PAN",
  "PATK",
  "PDEC",
  "POLY",
  "PREL",
  "PRINT",
  "PSUS",
  "PW",
  "RATE",
  "REL",
  "RES",
  "RING",
  "RT",
  "S1",
  "S2",
  "S3",
  "S4",
  "SDLY",
  "SHP",
  "SOL",
  "STRK",
  "SUB",
  "SUS",
  "SWID",
  "SYN",
  "SYNC",
  "TONE",
  "TUN",
  "TUNE",
  "TYPE",
  "UNI",
  "VEL",
  "VOL",
  "WOW",
};
constexpr int kNumLabelAbbreviations = static_cast<int>(sizeof(kLabelAbbreviations) / sizeof(const char*));
inline bool LabelIsDeclaredAbbrev(const char* label) {
  for (int i = 0; i < kNumLabelAbbreviations; ++i)
    if (std::strcmp(label, kLabelAbbreviations[i]) == 0) return true;
  return false;
}
inline bool LabelPassesAbbrevGate(const char* label) {
  const std::size_t n = std::strlen(label);
  if (n == 0) return false;
  if (n == 1) return LabelIsDeclaredAbbrev(label);
  if (n >= 4) return true;
  return LabelIsDeclaredAbbrev(label);
}

constexpr PanelSpec kPanels[] = {
  {"OSCILLATORS", kPanelOsc, "panel"},
  {"FILTER", kPanelFilter, "panel"},
  {"AMP", kPanelAmp, "panel"},
  {"LFO", kPanelLfo, "panel"},
  {"MOD MATRIX", kPanelMod, "panel"},
  {"ART", kPanelArt, "panel"},
  {"AMP MODIFIERS", kPanelAmpMods, "panel"},
  {"LOADED SOUNDS", kPanelSample, "panel"},
  {"DRUM PADS", kPanelPads, "panel"},
  {"VINTAGE ANALOG MODELING", kPanelVintage, "panel"},
  {"GLOBAL", kPanelGlobal, "panel"},
  {"EFFECTS", kPanelFx, "panel"},
};
struct OverlayRect { const char* id; Box rect; };
inline constexpr OverlayRect kOverlayRects[] = {
  {"bypass_label", {18.0f, 690.0f, 228.0f, 706.0f}},
  {"lfo1_label", {956.0f, 52.0f, 1222.0f, 66.0f}},
  {"lfo2_label", {956.0f, 167.0f, 1222.0f, 181.0f}},
  {"midi_hint1", {18.0f, 944.0f, 228.0f, 962.0f}},
  {"midi_hint2", {18.0f, 962.0f, 228.0f, 980.0f}},
  {"osc1_waveform", {268.0f, 52.0f, 380.0f, 96.0f}},
  {"osc2_waveform", {386.0f, 52.0f, 498.0f, 96.0f}},
  {"preset_menu", {902.0f, 508.0f, 1238.0f, 552.0f}},
  {"sample_waveform", {902.0f, 454.0f, 1238.0f, 494.0f}},
};
constexpr int kNumOverlayRects = static_cast<int>(sizeof(kOverlayRects) / sizeof(OverlayRect));
inline const OverlayRect* OverlayFor(const char* id) {
  for (int i = 0; i < kNumOverlayRects; ++i)
    if (std::strcmp(id, kOverlayRects[i].id) == 0) return &kOverlayRects[i];
  return nullptr;
}
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };
inline constexpr ControlRect kControlRects[] = {
  {0, {762.0f, 798.0f, 801.3f, 871.0f}, "VOL", 11.0f, 1836, ControlKind::Knob},
  {1, {652.7f, 826.0f, 712.0f, 899.0f}, "DIST", 11.0f, 2448, ControlKind::Knob},
  {2, {506.0f, 826.0f, 565.3f, 899.0f}, "TONE", 11.0f, 2448, ControlKind::Knob},
  {3, {18.0f, 168.0f, 228.0f, 241.0f}, "WOW", 11.0f, 1836, ControlKind::Knob},
  {4, {18.0f, 255.0f, 228.0f, 328.0f}, "FLUT", 11.0f, 2448, ControlKind::Knob},
  {5, {18.0f, 342.0f, 228.0f, 415.0f}, "HISS", 11.0f, 2448, ControlKind::Knob},
  {6, {18.0f, 429.0f, 228.0f, 502.0f}, "PRINT", 11.0f, 3060, ControlKind::Knob},
  {7, {18.0f, 516.0f, 228.0f, 589.0f}, "MIX", 11.0f, 1836, ControlKind::Knob},
  {8, {18.0f, 708.0f, 228.0f, 736.0f}, "BYP", 11.0f, 1836, ControlKind::Tab},
  {9, {274.0f, 784.0f, 494.0f, 812.0f}, "TYPE", 11.0f, 2448, ControlKind::Tab},
  {10, {922.0f, 808.5f, 978.0f, 860.5f}, "POLY", 11.0f, 2448, ControlKind::Dropdown},
  {11, {815.3f, 798.0f, 854.7f, 871.0f}, "GLIDE", 11.0f, 3060, ControlKind::Knob},
  {12, {868.7f, 798.0f, 908.0f, 871.0f}, "UNI", 11.0f, 1836, ControlKind::Knob},
  {13, {762.0f, 905.0f, 822.0f, 978.0f}, "DET", 11.0f, 1836, ControlKind::Knob},
  {14, {268.0f, 110.0f, 332.0f, 183.0f}, "SHP", 11.0f, 1836, ControlKind::Knob},
  {15, {346.0f, 110.0f, 410.0f, 183.0f}, "TUNE", 11.0f, 2448, ControlKind::Knob},
  {16, {268.0f, 197.0f, 332.0f, 270.0f}, "LVL", 11.0f, 1836, ControlKind::Knob},
  {17, {346.0f, 197.0f, 410.0f, 270.0f}, "PW", 11.0f, 1224, ControlKind::Knob},
  {18, {410.0f, 110.0f, 474.0f, 183.0f}, "SHP", 11.0f, 1836, ControlKind::Knob},
  {19, {488.0f, 110.0f, 552.0f, 183.0f}, "TUNE", 11.0f, 2448, ControlKind::Knob},
  {20, {410.0f, 197.0f, 474.0f, 270.0f}, "LVL", 11.0f, 1836, ControlKind::Knob},
  {21, {488.0f, 197.0f, 552.0f, 270.0f}, "PW", 11.0f, 1224, ControlKind::Knob},
  {22, {268.0f, 284.0f, 347.3f, 357.0f}, "SUB", 11.0f, 1836, ControlKind::Knob},
  {23, {361.3f, 284.0f, 440.7f, 357.0f}, "NOISE", 11.0f, 3060, ControlKind::Knob},
  {24, {454.7f, 284.0f, 534.0f, 357.0f}, "FM", 11.0f, 1224, ControlKind::Knob},
  {25, {580.0f, 52.0f, 706.0f, 80.0f}, "TYPE", 11.0f, 2448, ControlKind::Tab},
  {26, {580.0f, 94.0f, 640.0f, 167.0f}, "CUT", 11.0f, 1836, ControlKind::Knob},
  {27, {654.0f, 94.0f, 714.0f, 167.0f}, "RES", 11.0f, 1836, ControlKind::Knob},
  {28, {580.0f, 181.0f, 640.0f, 254.0f}, "ENV", 11.0f, 1836, ControlKind::Knob},
  {29, {746.0f, 66.0f, 821.0f, 139.0f}, "ATT", 11.0f, 1836, ControlKind::Knob},
  {30, {835.0f, 66.0f, 910.0f, 139.0f}, "DEC", 11.0f, 1836, ControlKind::Knob},
  {31, {746.0f, 153.0f, 821.0f, 226.0f}, "SUS", 11.0f, 1836, ControlKind::Knob},
  {32, {835.0f, 153.0f, 910.0f, 226.0f}, "REL", 11.0f, 1836, ControlKind::Knob},
  {33, {902.0f, 566.0f, 975.5f, 639.0f}, "PATK", 11.0f, 1836, ControlKind::Knob},
  {34, {989.5f, 566.0f, 1063.0f, 639.0f}, "PDEC", 11.0f, 1836, ControlKind::Knob},
  {35, {1077.0f, 566.0f, 1150.5f, 639.0f}, "PSUS", 11.0f, 1836, ControlKind::Knob},
  {36, {1164.5f, 566.0f, 1238.0f, 639.0f}, "PREL", 11.0f, 1836, ControlKind::Knob},
  {37, {902.0f, 653.0f, 975.5f, 726.0f}, "PAMT", 11.0f, 1836, ControlKind::Knob},
  {38, {836.0f, 905.0f, 896.0f, 978.0f}, "SYN", 11.0f, 1836, ControlKind::Knob},
  {39, {746.0f, 254.0f, 821.0f, 327.0f}, "FATT", 11.0f, 1836, ControlKind::Knob},
  {40, {835.0f, 254.0f, 910.0f, 327.0f}, "FDEC", 11.0f, 1836, ControlKind::Knob},
  {41, {746.0f, 341.0f, 821.0f, 414.0f}, "FSUS", 11.0f, 1836, ControlKind::Knob},
  {42, {835.0f, 341.0f, 910.0f, 414.0f}, "FREL", 11.0f, 1836, ControlKind::Knob},
  {43, {268.0f, 370.0f, 344.0f, 398.0f}, "SYNC", 11.0f, 2448, ControlKind::Tab},
  {44, {352.0f, 370.0f, 428.0f, 398.0f}, "RING", 11.0f, 2448, ControlKind::Tab},
  {45, {654.0f, 181.0f, 714.0f, 254.0f}, "KEY", 11.0f, 1836, ControlKind::Knob},
  {46, {579.3f, 826.0f, 638.7f, 899.0f}, "COMP", 11.0f, 2448, ControlKind::Knob},
  {47, {956.0f, 80.0f, 1016.0f, 153.0f}, "RATE", 11.0f, 2448, ControlKind::Knob},
  {48, {1104.0f, 98.5f, 1156.0f, 134.5f}, "SHP", 11.0f, 1836, ControlKind::Dropdown},
  {49, {1170.0f, 98.5f, 1222.0f, 134.5f}, "DST", 11.0f, 1836, ControlKind::Dropdown},
  {50, {1030.0f, 80.0f, 1090.0f, 153.0f}, "DEP", 11.0f, 1836, ControlKind::Knob},
  {51, {984.0f, 905.0f, 1048.0f, 957.0f}, "MWD", 11.0f, 1836, ControlKind::Dropdown},
  {52, {910.0f, 905.0f, 970.0f, 978.0f}, "MWA", 11.0f, 1836, ControlKind::Knob},
  {53, {580.0f, 268.0f, 640.0f, 341.0f}, "VEL", 11.0f, 1836, ControlKind::Knob},
  {54, {654.0f, 268.0f, 714.0f, 341.0f}, "FILT", 11.0f, 1836, ControlKind::Knob},
  {55, {992.0f, 808.5f, 1048.0f, 860.5f}, "LEG", 11.0f, 1836, ControlKind::Dropdown},
  {56, {1098.0f, 798.0f, 1216.7f, 850.0f}, "I1", 11.0f, 1224, ControlKind::Dropdown},
  {57, {1098.0f, 870.0f, 1150.3f, 917.0f}, "A1", 11.0f, 1224, ControlKind::Knob},
  {58, {1164.3f, 870.0f, 1216.7f, 917.0f}, "B1", 11.0f, 1224, ControlKind::Knob},
  {59, {1230.7f, 870.0f, 1283.0f, 917.0f}, "C1", 11.0f, 1224, ControlKind::Knob},
  {60, {1297.0f, 870.0f, 1349.3f, 917.0f}, "M1", 11.0f, 1224, ControlKind::Knob},
  {62, {1230.7f, 798.0f, 1349.3f, 850.0f}, "I2", 11.0f, 1224, ControlKind::Dropdown},
  {63, {1363.3f, 870.0f, 1415.7f, 917.0f}, "A2", 11.0f, 1224, ControlKind::Knob},
  {64, {1429.7f, 870.0f, 1482.0f, 917.0f}, "B2", 11.0f, 1224, ControlKind::Knob},
  {65, {1098.0f, 931.0f, 1150.3f, 978.0f}, "C2", 11.0f, 1224, ControlKind::Knob},
  {66, {1164.3f, 931.0f, 1216.7f, 978.0f}, "M2", 11.0f, 1224, ControlKind::Knob},
  {68, {1363.3f, 798.0f, 1482.0f, 850.0f}, "MS", 11.0f, 1224, ControlKind::Dropdown},
  {69, {1230.7f, 931.0f, 1283.0f, 978.0f}, "MA", 11.0f, 1224, ControlKind::Knob},
  {70, {1297.0f, 931.0f, 1349.3f, 978.0f}, "MB", 11.0f, 1224, ControlKind::Knob},
  {71, {1363.3f, 931.0f, 1415.7f, 978.0f}, "MC", 11.0f, 1224, ControlKind::Knob},
  {72, {1429.7f, 931.0f, 1482.0f, 978.0f}, "MX", 11.0f, 1224, ControlKind::Knob},
  {74, {18.0f, 603.0f, 228.0f, 676.0f}, "OUT", 11.0f, 1836, ControlKind::Knob},
  {75, {1282.0f, 454.0f, 1320.0f, 718.0f}, "PAD", 11.0f, 1836, ControlKind::Pad},
  {76, {1334.0f, 468.0f, 1376.0f, 541.0f}, "LVL", 11.0f, 1836, ControlKind::Knob},
  {77, {1390.0f, 468.0f, 1432.0f, 541.0f}, "PAN", 11.0f, 1836, ControlKind::Knob},
  {78, {1446.0f, 468.0f, 1488.0f, 541.0f}, "TUN", 11.0f, 1836, ControlKind::Knob},
  {79, {1334.0f, 555.0f, 1376.0f, 628.0f}, "DEC", 11.0f, 1836, ControlKind::Knob},
  {80, {1390.0f, 555.0f, 1432.0f, 628.0f}, "FLT", 11.0f, 1836, ControlKind::Knob},
  {81, {1446.0f, 555.0f, 1488.0f, 628.0f}, "DRV", 11.0f, 1836, ControlKind::Knob},
  {82, {1077.0f, 675.5f, 1149.0f, 703.5f}, "MUT", 11.0f, 1836, ControlKind::Tab},
  {83, {1164.5f, 675.5f, 1236.5f, 703.5f}, "SOL", 11.0f, 1836, ControlKind::Tab},
  {84, {989.5f, 653.0f, 1063.0f, 726.0f}, "KIT", 11.0f, 1836, ControlKind::Knob},
  {85, {956.0f, 195.0f, 1016.0f, 268.0f}, "RT", 11.0f, 1224, ControlKind::Knob},
  {86, {1104.0f, 213.5f, 1156.0f, 249.5f}, "SHP", 11.0f, 1836, ControlKind::Dropdown},
  {87, {1170.0f, 213.5f, 1222.0f, 249.5f}, "DST", 11.0f, 1836, ControlKind::Dropdown},
  {88, {1030.0f, 195.0f, 1090.0f, 268.0f}, "DEP", 11.0f, 1836, ControlKind::Knob},
  {89, {1268.0f, 70.0f, 1347.0f, 122.0f}, "S1", 11.0f, 1224, ControlKind::Dropdown},
  {90, {1361.0f, 70.0f, 1440.0f, 122.0f}, "D1", 11.0f, 1224, ControlKind::Dropdown},
  {91, {1440.0f, 70.0f, 1482.0f, 143.0f}, "A1", 11.0f, 1224, ControlKind::Knob},
  {92, {1268.0f, 156.5f, 1347.0f, 208.5f}, "S2", 11.0f, 1224, ControlKind::Dropdown},
  {93, {1361.0f, 156.5f, 1440.0f, 208.5f}, "D2", 11.0f, 1224, ControlKind::Dropdown},
  {94, {1440.0f, 156.5f, 1482.0f, 229.5f}, "A2", 11.0f, 1224, ControlKind::Knob},
  {95, {1268.0f, 243.0f, 1347.0f, 295.0f}, "S3", 11.0f, 1224, ControlKind::Dropdown},
  {96, {1361.0f, 243.0f, 1440.0f, 295.0f}, "D3", 11.0f, 1224, ControlKind::Dropdown},
  {97, {1440.0f, 243.0f, 1482.0f, 316.0f}, "A3", 11.0f, 1224, ControlKind::Knob},
  {98, {1268.0f, 329.5f, 1347.0f, 381.5f}, "S4", 11.0f, 1224, ControlKind::Dropdown},
  {99, {1361.0f, 329.5f, 1440.0f, 381.5f}, "D4", 11.0f, 1224, ControlKind::Dropdown},
  {100, {1440.0f, 329.5f, 1482.0f, 402.5f}, "A4", 11.0f, 1224, ControlKind::Knob},
  {101, {752.0f, 474.0f, 794.0f, 521.0f}, "NKEY", 11.0f, 1836, ControlKind::Knob},
  {102, {808.0f, 474.0f, 850.0f, 521.0f}, "AKEY", 11.0f, 1836, ControlKind::Knob},
  {103, {752.0f, 535.0f, 794.0f, 582.0f}, "ABND", 11.0f, 1836, ControlKind::Knob},
  {104, {808.0f, 535.0f, 850.0f, 582.0f}, "DKEY", 11.0f, 1836, ControlKind::Knob},
  {105, {752.0f, 596.0f, 794.0f, 643.0f}, "DBND", 11.0f, 1836, ControlKind::Knob},
  {106, {808.0f, 596.0f, 850.0f, 643.0f}, "SDLY", 11.0f, 1836, ControlKind::Knob},
  {107, {752.0f, 657.0f, 794.0f, 704.0f}, "STRK", 11.0f, 1836, ControlKind::Knob},
  {108, {808.0f, 657.0f, 850.0f, 704.0f}, "SWID", 11.0f, 1836, ControlKind::Knob},
  {109, {18.0f, 750.0f, 116.0f, 823.0f}, "DIR", 11.0f, 1836, ControlKind::Knob},
  {110, {130.0f, 750.0f, 228.0f, 823.0f}, "CBND", 11.0f, 1836, ControlKind::Knob},
};

constexpr int kNumControlRects = static_cast<int>(sizeof(kControlRects) / sizeof(ControlRect));
constexpr int kBypassViaAlgoMenu[] = {61, 67, 73};
constexpr int kNumBypassViaAlgoMenu = static_cast<int>(sizeof(kBypassViaAlgoMenu) / sizeof(int));
inline bool BypassViaAlgoMenu(int parameter)
{
  for (int id : kBypassViaAlgoMenu)
    if (id == parameter)
      return true;
  return false;
}
inline const ControlRect& ControlFor(int parameter) { for (const auto& item : kControlRects) if (item.parameter == parameter) return item; return kControlRects[0]; }
inline float EnvelopeTimeX(int segment, double value){const float v=static_cast<float>(std::clamp(value,0.,1.));if(segment==0)return 0.04f+0.22f*v;if(segment==1)return 0.30f+0.22f*v;return 0.70f+0.27f*v;}
inline double EnvelopeTimeValue(int segment,float x){if(segment==0)return std::clamp((x-0.04)/0.22,0.,1.);if(segment==1)return std::clamp((x-0.30)/0.22,0.,1.);return std::clamp((x-0.70)/0.27,0.,1.);}
inline float ScrollOffset(float offset,float delta,float content,float viewport){return std::clamp(offset-delta*32.f,0.f,std::max(0.f,content-viewport));}
constexpr float kSpaceMonoEm=0.612f; constexpr float kWordmarkPx=42.f;
inline float MonoAdvance(int units,float px){return static_cast<float>(units)*px/1000.f;}
inline float OswaldMetricHeight(float px){return px*1.482f;}
} }
