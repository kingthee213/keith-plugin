#pragma once
#include <algorithm>
#include <cmath>
#include <cstring>

namespace keith { namespace ui {

struct Box { float l,t,r,b; constexpr float W() const { return r-l; } constexpr float H() const { return b-t; } constexpr bool Contains(Box o) const { return o.l>=l&&o.t>=t&&o.r<=r&&o.b<=b; } constexpr bool Overlaps(Box o) const { return l<o.r&&o.l<r&&t<o.b&&o.t<b; } };

constexpr float kCanvasW = 1152.0f;
constexpr float kCanvasH = 768.0f;
constexpr float kLeftRailW = 184.0f;
constexpr float kPanelPadding = 13.5f;
constexpr float kControlGap = 10.5f;
constexpr bool kSingleWindowEditor = true;
constexpr Box kCanvas {0.f, 0.f, kCanvasW, kCanvasH};

// Knob vertical budget from KeithKnobControl::kLabelHeight / kValueHeight.
constexpr float kKnobLabelHeight = 14.2f;
constexpr float kKnobValueHeight = 15.0f;
constexpr float kKnobBodyMin = 25.5f;
constexpr float kKnobMinHeight = kKnobBodyMin + kKnobLabelHeight + kKnobValueHeight;

enum class ControlKind { Knob, Tab, Dropdown, Fader, Pad };

struct PanelSpec { const char* title; Box bounds; const char* tint; };
constexpr float kPanelTitleBand = 21.0f;
constexpr Box kKeyboardBox {268.5f, 751.5f, 1119.0f, 765.0f};
inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }
constexpr Box kPanelOsc {188.0f, 18.0f, 414.0f, 312.0f};
constexpr Box kPanelFilter {422.0f, 18.0f, 543.0f, 312.0f};
constexpr Box kPanelAmp {546.0f, 18.0f, 696.0f, 312.0f};
constexpr Box kPanelLfo {704.0f, 18.0f, 930.0f, 312.0f};
constexpr Box kPanelMod {938.0f, 18.0f, 1125.0f, 312.0f};
constexpr Box kPanelArt {194.0f, 322.0f, 606.0f, 549.0f};
constexpr Box kPanelAmpMods {550.0f, 324.0f, 735.0f, 484.0f};
constexpr Box kPanelSample {741.0f, 320.0f, 942.0f, 525.0f};
constexpr Box kPanelPads {948.0f, 320.0f, 1125.0f, 552.0f};
constexpr Box kPanelVintage {192.0f, 543.0f, 548.0f, 747.0f};
constexpr Box kPanelGlobal {558.0f, 543.0f, 800.0f, 747.0f};
constexpr Box kPanelFx {810.0f, 543.0f, 1125.0f, 750.0f};
constexpr Box kLeftRail {0.0f, 0.0f, 184.0f, 768.0f};
constexpr Box kWordmarkBox {14.0f, 21.0f, 171.0f, 88.0f};
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
  {"bypass_label", {13.5f, 519.0f, 170.5f, 531.0f}},
  {"lfo1_label", {717.5f, 39.0f, 916.5f, 49.5f}},
  {"lfo2_label", {717.5f, 125.5f, 916.5f, 136.0f}},
  {"midi_hint1", {13.5f, 708.0f, 170.5f, 722.0f}},
  {"midi_hint2", {13.5f, 722.0f, 170.5f, 735.0f}},
  {"osc1_waveform", {201.5f, 39.0f, 285.5f, 72.0f}},
  {"osc2_waveform", {289.5f, 39.0f, 373.5f, 72.0f}},
  {"preset_menu", {754.5f, 366.5f, 928.5f, 393.5f}},
  {"sample_waveform", {754.5f, 341.0f, 928.5f, 356.0f}},
};
constexpr int kNumOverlayRects = static_cast<int>(sizeof(kOverlayRects) / sizeof(OverlayRect));
inline const OverlayRect* OverlayFor(const char* id) {
  for (int i = 0; i < kNumOverlayRects; ++i)
    if (std::strcmp(id, kOverlayRects[i].id) == 0) return &kOverlayRects[i];
  return nullptr;
}
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };
inline constexpr ControlRect kControlRects[] = {
  {0, {571.5f, 574.5f, 601.2f, 629.5f}, "VOL", 8.2f, 1836, ControlKind::Knob},
  {1, {490.0f, 595.5f, 534.5f, 650.5f}, "DIST", 8.2f, 2448, ControlKind::Knob},
  {2, {380.0f, 595.5f, 424.5f, 650.5f}, "TONE", 8.2f, 2448, ControlKind::Knob},
  {3, {13.5f, 126.0f, 170.5f, 181.0f}, "WOW", 8.2f, 1836, ControlKind::Knob},
  {4, {13.5f, 191.5f, 170.5f, 246.5f}, "FLUT", 8.2f, 2448, ControlKind::Knob},
  {5, {13.5f, 257.0f, 170.5f, 312.0f}, "HISS", 8.2f, 2448, ControlKind::Knob},
  {6, {13.5f, 322.5f, 170.5f, 377.5f}, "PRINT", 8.2f, 3060, ControlKind::Knob},
  {7, {13.5f, 388.0f, 170.5f, 443.0f}, "MIX", 8.2f, 1836, ControlKind::Knob},
  {8, {13.5f, 532.5f, 170.5f, 553.5f}, "BYP", 8.2f, 1836, ControlKind::Tab},
  {9, {205.5f, 564.0f, 370.5f, 585.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {10, {692.0f, 582.5f, 734.0f, 621.5f}, "POLY", 8.2f, 2448, ControlKind::Dropdown},
  {11, {611.7f, 574.5f, 641.3f, 629.5f}, "GLIDE", 8.2f, 3060, ControlKind::Knob},
  {12, {651.8f, 574.5f, 681.5f, 629.5f}, "UNI", 8.2f, 1836, ControlKind::Knob},
  {13, {571.5f, 678.5f, 616.7f, 733.5f}, "DET", 8.2f, 1836, ControlKind::Knob},
  {14, {201.5f, 82.5f, 249.5f, 137.5f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {15, {260.0f, 82.5f, 308.0f, 137.5f}, "TUNE", 8.2f, 2448, ControlKind::Knob},
  {16, {201.5f, 148.0f, 249.5f, 203.0f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {17, {260.0f, 148.0f, 308.0f, 203.0f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {18, {308.0f, 82.5f, 356.0f, 137.5f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {19, {366.5f, 82.5f, 414.5f, 137.5f}, "TUNE", 8.2f, 2448, ControlKind::Knob},
  {20, {308.0f, 148.0f, 356.0f, 203.0f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {21, {366.5f, 148.0f, 414.5f, 203.0f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {22, {201.5f, 213.5f, 260.8f, 268.5f}, "SUB", 8.2f, 1836, ControlKind::Knob},
  {23, {271.3f, 213.5f, 330.7f, 268.5f}, "NOISE", 8.2f, 3060, ControlKind::Knob},
  {24, {341.2f, 213.5f, 400.5f, 268.5f}, "FM", 8.2f, 1224, ControlKind::Knob},
  {25, {435.5f, 39.0f, 529.5f, 60.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {26, {435.5f, 70.5f, 480.5f, 125.5f}, "CUT", 8.2f, 1836, ControlKind::Knob},
  {27, {491.0f, 70.5f, 536.0f, 125.5f}, "RES", 8.2f, 1836, ControlKind::Knob},
  {28, {435.5f, 136.0f, 480.5f, 191.0f}, "ENV", 8.2f, 1836, ControlKind::Knob},
  {29, {559.5f, 49.5f, 615.8f, 104.5f}, "ATT", 8.2f, 1836, ControlKind::Knob},
  {30, {626.2f, 49.5f, 682.5f, 104.5f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {31, {559.5f, 115.0f, 615.8f, 170.0f}, "SUS", 8.2f, 1836, ControlKind::Knob},
  {32, {626.2f, 115.0f, 682.5f, 170.0f}, "REL", 8.2f, 1836, ControlKind::Knob},
  {33, {754.5f, 404.0f, 790.1f, 459.0f}, "PATK", 8.2f, 1836, ControlKind::Knob},
  {34, {800.6f, 404.0f, 836.2f, 459.0f}, "PDEC", 8.2f, 1836, ControlKind::Knob},
  {35, {846.8f, 404.0f, 882.4f, 459.0f}, "PSUS", 8.2f, 1836, ControlKind::Knob},
  {36, {892.9f, 404.0f, 928.5f, 459.0f}, "PREL", 8.2f, 1836, ControlKind::Knob},
  {37, {754.5f, 469.5f, 790.1f, 524.5f}, "PAMT", 8.2f, 1836, ControlKind::Knob},
  {38, {627.2f, 678.5f, 672.3f, 733.5f}, "SYN", 8.2f, 1836, ControlKind::Knob},
  {39, {559.5f, 191.0f, 615.8f, 246.0f}, "FATT", 8.2f, 1836, ControlKind::Knob},
  {40, {626.2f, 191.0f, 682.5f, 246.0f}, "FDEC", 8.2f, 1836, ControlKind::Knob},
  {41, {559.5f, 256.5f, 615.8f, 311.5f}, "FSUS", 8.2f, 1836, ControlKind::Knob},
  {42, {626.2f, 256.5f, 682.5f, 311.5f}, "FREL", 8.2f, 1836, ControlKind::Knob},
  {43, {201.5f, 277.5f, 258.5f, 298.5f}, "SYNC", 8.2f, 2448, ControlKind::Tab},
  {44, {264.5f, 277.5f, 321.5f, 298.5f}, "RING", 8.2f, 2448, ControlKind::Tab},
  {45, {491.0f, 136.0f, 536.0f, 191.0f}, "KEY", 8.2f, 1836, ControlKind::Knob},
  {46, {435.0f, 595.5f, 479.5f, 650.5f}, "COMP", 8.2f, 2448, ControlKind::Knob},
  {47, {717.5f, 60.0f, 762.2f, 115.0f}, "RATE", 8.2f, 2448, ControlKind::Knob},
  {48, {828.0f, 74.0f, 867.0f, 101.0f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {49, {877.5f, 74.0f, 916.5f, 101.0f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {50, {772.8f, 60.0f, 817.5f, 115.0f}, "DEP", 8.2f, 1836, ControlKind::Knob},
  {51, {738.5f, 678.5f, 786.5f, 717.5f}, "MWD", 8.2f, 1836, ControlKind::Dropdown},
  {52, {682.8f, 678.5f, 728.0f, 733.5f}, "MWA", 8.2f, 1836, ControlKind::Knob},
  {53, {435.5f, 201.5f, 480.5f, 256.5f}, "VEL", 8.2f, 1836, ControlKind::Knob},
  {54, {491.0f, 201.5f, 536.0f, 256.5f}, "FILT", 8.2f, 1836, ControlKind::Knob},
  {55, {744.5f, 582.5f, 786.5f, 621.5f}, "LEG", 8.2f, 1836, ControlKind::Dropdown},
  {56, {823.5f, 574.5f, 912.5f, 601.5f}, "I1", 8.2f, 1224, ControlKind::Dropdown},
  {57, {823.5f, 612.0f, 862.8f, 667.0f}, "A1", 8.2f, 1224, ControlKind::Knob},
  {58, {873.2f, 612.0f, 912.5f, 667.0f}, "B1", 8.2f, 1224, ControlKind::Knob},
  {59, {923.0f, 612.0f, 962.2f, 667.0f}, "C1", 8.2f, 1224, ControlKind::Knob},
  {60, {972.8f, 612.0f, 1012.0f, 667.0f}, "M1", 8.2f, 1224, ControlKind::Knob},
  {62, {923.0f, 574.5f, 1012.0f, 601.5f}, "I2", 8.2f, 1224, ControlKind::Dropdown},
  {63, {1022.5f, 612.0f, 1061.8f, 667.0f}, "A2", 8.2f, 1224, ControlKind::Knob},
  {64, {1072.2f, 612.0f, 1111.5f, 667.0f}, "B2", 8.2f, 1224, ControlKind::Knob},
  {65, {823.5f, 677.5f, 862.8f, 732.5f}, "C2", 8.2f, 1224, ControlKind::Knob},
  {66, {873.2f, 677.5f, 912.5f, 732.5f}, "M2", 8.2f, 1224, ControlKind::Knob},
  {68, {1022.5f, 574.5f, 1111.5f, 601.5f}, "MS", 8.2f, 1224, ControlKind::Dropdown},
  {69, {923.0f, 677.5f, 962.2f, 732.5f}, "MA", 8.2f, 1224, ControlKind::Knob},
  {70, {972.8f, 677.5f, 1012.0f, 732.5f}, "MB", 8.2f, 1224, ControlKind::Knob},
  {71, {1022.5f, 677.5f, 1061.8f, 732.5f}, "MC", 8.2f, 1224, ControlKind::Knob},
  {72, {1072.2f, 677.5f, 1111.5f, 732.5f}, "MX", 8.2f, 1224, ControlKind::Knob},
  {74, {13.5f, 453.5f, 170.5f, 508.5f}, "OUT", 8.2f, 1836, ControlKind::Knob},
  {75, {961.5f, 341.0f, 990.0f, 538.5f}, "PAD", 8.2f, 1836, ControlKind::Pad},
  {76, {1000.5f, 351.5f, 1032.0f, 406.5f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {77, {1042.5f, 351.5f, 1074.0f, 406.5f}, "PAN", 8.2f, 1836, ControlKind::Knob},
  {78, {1084.5f, 351.5f, 1116.0f, 406.5f}, "TUN", 8.2f, 1836, ControlKind::Knob},
  {79, {1000.5f, 417.0f, 1032.0f, 472.0f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {80, {1042.5f, 417.0f, 1074.0f, 472.0f}, "FLT", 8.2f, 1836, ControlKind::Knob},
  {81, {1084.5f, 417.0f, 1116.0f, 472.0f}, "DRV", 8.2f, 1836, ControlKind::Knob},
  {82, {846.8f, 486.5f, 882.4f, 507.5f}, "MUT", 8.2f, 1836, ControlKind::Tab},
  {83, {892.9f, 486.5f, 928.5f, 507.5f}, "SOL", 8.2f, 1836, ControlKind::Tab},
  {84, {800.6f, 469.5f, 836.2f, 524.5f}, "KIT", 8.2f, 1836, ControlKind::Knob},
  {85, {717.5f, 146.5f, 762.2f, 201.5f}, "RT", 8.2f, 1224, ControlKind::Knob},
  {86, {828.0f, 160.5f, 867.0f, 187.5f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {87, {877.5f, 160.5f, 916.5f, 187.5f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {88, {772.8f, 146.5f, 817.5f, 201.5f}, "DEP", 8.2f, 1836, ControlKind::Knob},
  {89, {951.5f, 52.5f, 1010.5f, 91.5f}, "S1", 8.2f, 1224, ControlKind::Dropdown},
  {90, {1021.0f, 52.5f, 1080.0f, 91.5f}, "D1", 8.2f, 1224, ControlKind::Dropdown},
  {91, {1080.0f, 52.5f, 1111.5f, 107.5f}, "A1", 8.2f, 1224, ControlKind::Knob},
  {92, {951.5f, 117.4f, 1010.5f, 156.4f}, "S2", 8.2f, 1224, ControlKind::Dropdown},
  {93, {1021.0f, 117.4f, 1080.0f, 156.4f}, "D2", 8.2f, 1224, ControlKind::Dropdown},
  {94, {1080.0f, 117.4f, 1111.5f, 172.4f}, "A2", 8.2f, 1224, ControlKind::Knob},
  {95, {951.5f, 182.2f, 1010.5f, 221.2f}, "S3", 8.2f, 1224, ControlKind::Dropdown},
  {96, {1021.0f, 182.2f, 1080.0f, 221.2f}, "D3", 8.2f, 1224, ControlKind::Dropdown},
  {97, {1080.0f, 182.2f, 1111.5f, 237.2f}, "A3", 8.2f, 1224, ControlKind::Knob},
  {98, {951.5f, 247.1f, 1010.5f, 286.1f}, "S4", 8.2f, 1224, ControlKind::Dropdown},
  {99, {1021.0f, 247.1f, 1080.0f, 286.1f}, "D4", 8.2f, 1224, ControlKind::Dropdown},
  {100, {1080.0f, 247.1f, 1111.5f, 302.1f}, "A4", 8.2f, 1224, ControlKind::Knob},
  {101, {563.5f, 355.5f, 595.1f, 410.5f}, "NKEY", 8.2f, 1836, ControlKind::Knob},
  {102, {605.6f, 355.5f, 637.2f, 410.5f}, "AKEY", 8.2f, 1836, ControlKind::Knob},
  {103, {647.8f, 355.5f, 679.4f, 410.5f}, "ABND", 8.2f, 1836, ControlKind::Knob},
  {104, {689.9f, 355.5f, 721.5f, 410.5f}, "DKEY", 8.2f, 1836, ControlKind::Knob},
  {105, {563.5f, 421.0f, 595.1f, 476.0f}, "DBND", 8.2f, 1836, ControlKind::Knob},
  {106, {605.6f, 421.0f, 637.2f, 476.0f}, "SDLY", 8.2f, 1836, ControlKind::Knob},
  {107, {647.8f, 421.0f, 679.4f, 476.0f}, "STRK", 8.2f, 1836, ControlKind::Knob},
  {108, {689.9f, 421.0f, 721.5f, 476.0f}, "SWID", 8.2f, 1836, ControlKind::Knob},
  {109, {13.5f, 564.0f, 86.8f, 619.0f}, "DIR", 8.2f, 1836, ControlKind::Knob},
  {110, {97.2f, 564.0f, 170.5f, 619.0f}, "CBND", 8.2f, 1836, ControlKind::Knob},
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
