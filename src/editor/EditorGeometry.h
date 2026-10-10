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
constexpr Box kKeyboardBox {192.0f, 751.5f, 1119.0f, 765.0f};
inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }
constexpr Box kPanelOsc {188.0f, 18.0f, 414.0f, 312.0f};
constexpr Box kPanelFilter {422.0f, 18.0f, 543.0f, 312.0f};
constexpr Box kPanelAmp {546.0f, 18.0f, 696.0f, 312.0f};
constexpr Box kPanelLfo {704.0f, 18.0f, 930.0f, 312.0f};
constexpr Box kPanelMod {938.0f, 18.0f, 1125.0f, 312.0f};
constexpr Box kPanelAmpMods {192.0f, 322.0f, 439.0f, 533.0f};
constexpr Box kPanelSample {445.0f, 322.0f, 634.0f, 533.0f};
constexpr Box kPanelPads {640.0f, 322.0f, 1125.0f, 533.0f};
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
  {"AMP MODIFIERS", kPanelAmpMods, "panel"},
  {"LOADED SOUNDS", kPanelSample, "panel"},
  {"DRUM PADS", kPanelPads, "panel"},
  {"VINTAGE ANALOG MODELING", kPanelVintage, "panel"},
  {"GLOBAL", kPanelGlobal, "panel"},
  {"EFFECTS", kPanelFx, "panel"},
};
struct OverlayRect { const char* id; Box rect; };
inline constexpr OverlayRect kOverlayRects[] = {
  {"lfo1_label", {717.5f, 39.0f, 916.5f, 49.5f}},
  {"lfo2_label", {717.5f, 125.5f, 916.5f, 136.0f}},
  {"midi_hint1", {13.5f, 708.0f, 170.5f, 722.0f}},
  {"midi_hint2", {13.5f, 722.0f, 170.5f, 735.0f}},
  {"osc1_waveform", {201.5f, 39.0f, 295.8f, 65.0f}},
  {"osc2_waveform", {306.2f, 39.0f, 400.5f, 65.0f}},
  {"preset_menu", {458.5f, 361.5f, 536.5f, 388.5f}},
  {"sample_waveform", {458.5f, 343.0f, 620.5f, 351.0f}},
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
  {1, {318.7f, 609.5f, 364.8f, 664.5f}, "DIST", 8.2f, 2448, ControlKind::Knob},
  {2, {205.5f, 609.5f, 251.6f, 664.5f}, "TONE", 8.2f, 2448, ControlKind::Knob},
  {3, {375.2f, 609.5f, 421.3f, 664.5f}, "WOW", 8.2f, 1836, ControlKind::Knob},
  {4, {431.8f, 609.5f, 477.9f, 664.5f}, "FLUT", 8.2f, 2448, ControlKind::Knob},
  {5, {488.4f, 609.5f, 534.5f, 664.5f}, "HISS", 8.2f, 2448, ControlKind::Knob},
  {6, {205.5f, 675.0f, 251.6f, 730.0f}, "PRINT", 8.2f, 3060, ControlKind::Knob},
  {7, {262.1f, 675.0f, 308.2f, 730.0f}, "MIX", 8.2f, 1836, ControlKind::Knob},
  {8, {328.0f, 578.0f, 410.0f, 599.0f}, "BYP", 8.2f, 1836, ControlKind::Tab},
  {9, {205.5f, 578.0f, 317.5f, 599.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {10, {692.0f, 582.5f, 734.0f, 621.5f}, "POLY", 8.2f, 2448, ControlKind::Dropdown},
  {11, {611.7f, 574.5f, 641.3f, 629.5f}, "GLIDE", 8.2f, 3060, ControlKind::Knob},
  {12, {651.8f, 574.5f, 681.5f, 629.5f}, "UNI", 8.2f, 1836, ControlKind::Knob},
  {13, {571.5f, 678.5f, 616.7f, 733.5f}, "DET", 8.2f, 1836, ControlKind::Knob},
  {14, {201.5f, 75.5f, 243.4f, 130.5f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {15, {253.9f, 75.5f, 295.8f, 130.5f}, "TUN", 8.2f, 2448, ControlKind::Knob},
  {16, {201.5f, 141.0f, 243.4f, 196.0f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {17, {253.9f, 141.0f, 295.8f, 196.0f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {18, {295.8f, 75.5f, 337.6f, 130.5f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {19, {348.1f, 75.5f, 390.0f, 130.5f}, "TUN", 8.2f, 2448, ControlKind::Knob},
  {20, {295.8f, 141.0f, 337.6f, 196.0f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {21, {348.1f, 141.0f, 390.0f, 196.0f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {22, {201.5f, 206.5f, 243.4f, 261.5f}, "SUB", 8.2f, 1836, ControlKind::Knob},
  {23, {253.9f, 206.5f, 295.8f, 261.5f}, "NOISE", 8.2f, 3060, ControlKind::Knob},
  {24, {306.2f, 206.5f, 348.1f, 261.5f}, "FM", 8.2f, 1224, ControlKind::Knob},
  {25, {435.5f, 39.0f, 529.5f, 60.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {26, {435.5f, 70.5f, 483.8f, 125.5f}, "CUT", 8.2f, 1836, ControlKind::Knob},
  {27, {494.3f, 70.5f, 542.6f, 125.5f}, "RES", 8.2f, 1836, ControlKind::Knob},
  {28, {435.5f, 136.0f, 483.8f, 191.0f}, "ENV", 8.2f, 1836, ControlKind::Knob},
  {29, {559.5f, 49.5f, 610.5f, 104.5f}, "ATT", 8.2f, 1836, ControlKind::Knob},
  {30, {621.0f, 49.5f, 672.0f, 104.5f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {31, {559.5f, 115.0f, 610.5f, 170.0f}, "SUS", 8.2f, 1836, ControlKind::Knob},
  {32, {621.0f, 115.0f, 672.0f, 170.0f}, "REL", 8.2f, 1836, ControlKind::Knob},
  {33, {458.5f, 399.0f, 506.8f, 454.0f}, "PATK", 8.2f, 1836, ControlKind::Knob},
  {34, {517.3f, 399.0f, 565.6f, 454.0f}, "PDEC", 8.2f, 1836, ControlKind::Knob},
  {35, {576.1f, 399.0f, 624.3f, 454.0f}, "PSUS", 8.2f, 1836, ControlKind::Knob},
  {36, {458.5f, 464.5f, 506.8f, 519.5f}, "PREL", 8.2f, 1836, ControlKind::Knob},
  {37, {517.3f, 464.5f, 565.6f, 519.5f}, "PAMT", 8.2f, 1836, ControlKind::Knob},
  {38, {627.2f, 678.5f, 672.3f, 733.5f}, "SYN", 8.2f, 1836, ControlKind::Knob},
  {39, {559.5f, 191.0f, 610.5f, 246.0f}, "FATT", 8.2f, 1836, ControlKind::Knob},
  {40, {621.0f, 191.0f, 672.0f, 246.0f}, "FDEC", 8.2f, 1836, ControlKind::Knob},
  {41, {559.5f, 256.5f, 610.5f, 311.5f}, "FSUS", 8.2f, 1836, ControlKind::Knob},
  {42, {621.0f, 256.5f, 672.0f, 311.5f}, "FREL", 8.2f, 1836, ControlKind::Knob},
  {43, {201.5f, 272.0f, 258.5f, 293.0f}, "SYNC", 8.2f, 2448, ControlKind::Tab},
  {44, {264.5f, 272.0f, 321.5f, 293.0f}, "RING", 8.2f, 2448, ControlKind::Tab},
  {45, {494.3f, 136.0f, 542.6f, 191.0f}, "KEY", 8.2f, 1836, ControlKind::Knob},
  {46, {262.1f, 609.5f, 308.2f, 664.5f}, "COMP", 8.2f, 2448, ControlKind::Knob},
  {47, {717.5f, 60.0f, 757.0f, 115.0f}, "RATE", 8.2f, 2448, ControlKind::Knob},
  {48, {817.5f, 74.0f, 856.5f, 101.0f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {49, {867.0f, 74.0f, 906.0f, 101.0f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {50, {767.5f, 60.0f, 807.0f, 115.0f}, "DEP", 8.2f, 1836, ControlKind::Knob},
  {51, {738.5f, 678.5f, 786.5f, 717.5f}, "MWD", 8.2f, 1836, ControlKind::Dropdown},
  {52, {682.8f, 678.5f, 728.0f, 733.5f}, "MWA", 8.2f, 1836, ControlKind::Knob},
  {53, {435.5f, 201.5f, 483.8f, 256.5f}, "VEL", 8.2f, 1836, ControlKind::Knob},
  {54, {494.3f, 201.5f, 542.6f, 256.5f}, "FILT", 8.2f, 1836, ControlKind::Knob},
  {55, {744.5f, 582.5f, 786.5f, 621.5f}, "LEG", 8.2f, 1836, ControlKind::Dropdown},
  {56, {823.5f, 574.5f, 912.5f, 601.5f}, "I1", 8.2f, 1224, ControlKind::Dropdown},
  {57, {823.5f, 612.0f, 855.0f, 667.0f}, "A1", 8.2f, 1224, ControlKind::Knob},
  {58, {865.5f, 612.0f, 897.0f, 667.0f}, "B1", 8.2f, 1224, ControlKind::Knob},
  {59, {907.5f, 612.0f, 939.0f, 667.0f}, "C1", 8.2f, 1224, ControlKind::Knob},
  {60, {949.5f, 612.0f, 981.0f, 667.0f}, "M1", 8.2f, 1224, ControlKind::Knob},
  {62, {923.0f, 574.5f, 1012.0f, 601.5f}, "I2", 8.2f, 1224, ControlKind::Dropdown},
  {63, {991.5f, 612.0f, 1023.0f, 667.0f}, "A2", 8.2f, 1224, ControlKind::Knob},
  {64, {1033.5f, 612.0f, 1065.0f, 667.0f}, "B2", 8.2f, 1224, ControlKind::Knob},
  {65, {823.5f, 677.5f, 855.0f, 732.5f}, "C2", 8.2f, 1224, ControlKind::Knob},
  {66, {865.5f, 677.5f, 897.0f, 732.5f}, "M2", 8.2f, 1224, ControlKind::Knob},
  {68, {1022.5f, 574.5f, 1111.5f, 601.5f}, "MS", 8.2f, 1224, ControlKind::Dropdown},
  {69, {907.5f, 677.5f, 939.0f, 732.5f}, "MA", 8.2f, 1224, ControlKind::Knob},
  {70, {949.5f, 677.5f, 981.0f, 732.5f}, "MB", 8.2f, 1224, ControlKind::Knob},
  {71, {991.5f, 677.5f, 1023.0f, 732.5f}, "MC", 8.2f, 1224, ControlKind::Knob},
  {72, {1033.5f, 677.5f, 1065.0f, 732.5f}, "MX", 8.2f, 1224, ControlKind::Knob},
  {74, {318.7f, 675.0f, 364.8f, 730.0f}, "OUT", 8.2f, 1836, ControlKind::Knob},
  {75, {653.5f, 353.5f, 930.6f, 519.5f}, "PAD", 8.2f, 1836, ControlKind::Pad},
  {76, {941.1f, 353.5f, 990.9f, 408.5f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {77, {1001.4f, 353.5f, 1051.2f, 408.5f}, "PAN", 8.2f, 1836, ControlKind::Knob},
  {78, {1061.7f, 353.5f, 1111.5f, 408.5f}, "TUN", 8.2f, 1836, ControlKind::Knob},
  {79, {941.1f, 419.0f, 990.9f, 474.0f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {80, {1001.4f, 419.0f, 1051.2f, 474.0f}, "FLT", 8.2f, 1836, ControlKind::Knob},
  {81, {1061.7f, 419.0f, 1111.5f, 474.0f}, "DRV", 8.2f, 1836, ControlKind::Knob},
  {82, {547.0f, 364.5f, 578.5f, 385.5f}, "MUT", 8.2f, 1836, ControlKind::Tab},
  {83, {589.0f, 364.5f, 620.5f, 385.5f}, "SOL", 8.2f, 1836, ControlKind::Tab},
  {84, {576.1f, 464.5f, 624.3f, 519.5f}, "KIT", 8.2f, 1836, ControlKind::Knob},
  {85, {717.5f, 146.5f, 757.0f, 201.5f}, "RT", 8.2f, 1224, ControlKind::Knob},
  {86, {817.5f, 160.5f, 856.5f, 187.5f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {87, {867.0f, 160.5f, 906.0f, 187.5f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {88, {767.5f, 146.5f, 807.0f, 201.5f}, "DEP", 8.2f, 1836, ControlKind::Knob},
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
  {101, {205.5f, 353.5f, 252.6f, 408.5f}, "NKEY", 8.2f, 1836, ControlKind::Knob},
  {102, {263.1f, 353.5f, 310.2f, 408.5f}, "AKEY", 8.2f, 1836, ControlKind::Knob},
  {103, {320.8f, 353.5f, 367.9f, 408.5f}, "ABND", 8.2f, 1836, ControlKind::Knob},
  {104, {378.4f, 353.5f, 425.5f, 408.5f}, "DKEY", 8.2f, 1836, ControlKind::Knob},
  {105, {205.5f, 419.0f, 252.6f, 474.0f}, "DBND", 8.2f, 1836, ControlKind::Knob},
  {106, {263.1f, 419.0f, 310.2f, 474.0f}, "SDLY", 8.2f, 1836, ControlKind::Knob},
  {107, {320.8f, 419.0f, 367.9f, 474.0f}, "STRK", 8.2f, 1836, ControlKind::Knob},
  {108, {378.4f, 419.0f, 425.5f, 474.0f}, "SWID", 8.2f, 1836, ControlKind::Knob},
  {109, {375.2f, 675.0f, 421.3f, 730.0f}, "DIR", 8.2f, 1836, ControlKind::Knob},
  {110, {431.8f, 675.0f, 477.9f, 730.0f}, "CBND", 8.2f, 1836, ControlKind::Knob},
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
