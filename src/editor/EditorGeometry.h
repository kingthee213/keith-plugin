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
constexpr Box kKeyboardBox {192.0f, 751.5f, 1122.0f, 765.0f};
inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }
constexpr Box kPanelOsc {188.0f, 18.0f, 414.0f, 312.0f};
constexpr Box kPanelFilter {422.0f, 18.0f, 543.0f, 312.0f};
constexpr Box kPanelAmp {546.0f, 18.0f, 696.0f, 312.0f};
constexpr Box kPanelLfo {704.0f, 18.0f, 930.0f, 312.0f};
constexpr Box kPanelMod {938.0f, 18.0f, 1125.0f, 312.0f};
constexpr Box kPanelAmpMods {192.0f, 322.0f, 439.0f, 533.0f};
constexpr Box kPanelSample {445.0f, 322.0f, 679.0f, 533.0f};
constexpr Box kPanelPads {684.0f, 322.0f, 1122.0f, 533.0f};
constexpr Box kPanelVintage {192.0f, 543.0f, 548.0f, 747.0f};
constexpr Box kPanelGlobal {558.0f, 543.0f, 800.0f, 747.0f};
constexpr Box kPanelFx {810.0f, 543.0f, 1122.0f, 750.0f};
constexpr Box kLeftRail {0.0f, 0.0f, 184.0f, 768.0f};
constexpr Box kWordmarkBox {43.0f, 17.0f, 183.0f, 167.0f};
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
  "MASTER",
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
  {"DRUMS", kPanelPads, "panel"},
  {"VINTAGE ANALOG MODELING", kPanelVintage, "panel"},
  {"GLOBAL", kPanelGlobal, "panel"},
  {"EFFECTS", kPanelFx, "panel"},
};
struct OverlayRect { const char* id; Box rect; };
inline constexpr OverlayRect kOverlayRects[] = {
  {"lfo1_label", {717.5f, 79.2f, 916.5f, 89.7f}},
  {"lfo2_label", {717.5f, 188.8f, 916.5f, 199.3f}},
  {"midi_hint1", {13.5f, 708.0f, 170.5f, 722.0f}},
  {"midi_hint2", {13.5f, 722.0f, 170.5f, 735.0f}},
  {"osc1_waveform", {201.5f, 48.2f, 295.8f, 66.6f}},
  {"osc2_waveform", {306.2f, 48.2f, 400.5f, 66.6f}},
  {"pad_midi_switch", {948.9f, 496.8f, 1108.5f, 507.2f}},
  {"preset_menu", {458.5f, 362.7f, 665.5f, 389.7f}},
  {"sample_waveform", {458.5f, 349.6f, 665.5f, 356.2f}},
};
constexpr int kNumOverlayRects = static_cast<int>(sizeof(kOverlayRects) / sizeof(OverlayRect));
inline const OverlayRect* OverlayFor(const char* id) {
  for (int i = 0; i < kNumOverlayRects; ++i)
    if (std::strcmp(id, kOverlayRects[i].id) == 0) return &kOverlayRects[i];
  return nullptr;
}
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };
inline constexpr ControlRect kControlRects[] = {
  {0, {571.5f, 592.2f, 606.1f, 647.2f}, "VOL", 8.2f, 1836, ControlKind::Knob},
  {1, {318.7f, 606.3f, 364.8f, 661.3f}, "DIST", 8.2f, 2448, ControlKind::Knob},
  {2, {205.5f, 606.3f, 251.6f, 661.3f}, "TONE", 8.2f, 2448, ControlKind::Knob},
  {3, {375.2f, 606.3f, 421.3f, 661.3f}, "WOW", 8.2f, 1836, ControlKind::Knob},
  {4, {431.8f, 606.3f, 477.9f, 661.3f}, "FLUT", 8.2f, 2448, ControlKind::Knob},
  {5, {488.4f, 606.3f, 534.5f, 661.3f}, "HISS", 8.2f, 2448, ControlKind::Knob},
  {6, {205.5f, 668.6f, 262.9f, 723.6f}, "PRINT", 8.2f, 3060, ControlKind::Knob},
  {7, {273.4f, 668.6f, 330.8f, 723.6f}, "MIX", 8.2f, 1836, ControlKind::Knob},
  {8, {375.2f, 578.0f, 534.5f, 599.0f}, "BYP", 8.2f, 1836, ControlKind::Tab},
  {9, {205.5f, 578.0f, 364.8f, 599.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {10, {706.8f, 600.2f, 741.4f, 639.2f}, "POLY", 8.2f, 2448, ControlKind::Dropdown},
  {11, {616.6f, 592.2f, 651.2f, 647.2f}, "GLIDE", 8.2f, 3060, ControlKind::Knob},
  {12, {661.7f, 592.2f, 696.3f, 647.2f}, "UNI", 8.2f, 1836, ControlKind::Knob},
  {13, {571.5f, 662.8f, 636.2f, 717.8f}, "DET", 8.2f, 1836, ControlKind::Knob},
  {14, {201.5f, 75.8f, 243.4f, 130.8f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {15, {253.9f, 75.8f, 295.8f, 130.8f}, "TUN", 8.2f, 2448, ControlKind::Knob},
  {16, {201.5f, 141.2f, 243.4f, 196.2f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {17, {253.9f, 141.2f, 295.8f, 196.2f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {18, {306.2f, 75.8f, 348.1f, 130.8f}, "SHP", 8.2f, 1836, ControlKind::Knob},
  {19, {358.6f, 75.8f, 400.5f, 130.8f}, "TUN", 8.2f, 2448, ControlKind::Knob},
  {20, {306.2f, 141.2f, 348.1f, 196.2f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {21, {358.6f, 141.2f, 400.5f, 196.2f}, "PW", 8.2f, 1224, ControlKind::Knob},
  {22, {201.5f, 206.8f, 260.8f, 261.8f}, "SUB", 8.2f, 1836, ControlKind::Knob},
  {23, {271.3f, 206.8f, 330.7f, 261.8f}, "NOISE", 8.2f, 3060, ControlKind::Knob},
  {24, {341.2f, 206.8f, 400.5f, 261.8f}, "FM", 8.2f, 1224, ControlKind::Knob},
  {25, {435.5f, 55.0f, 529.5f, 76.0f}, "TYPE", 8.2f, 2448, ControlKind::Tab},
  {26, {435.5f, 90.4f, 477.2f, 145.4f}, "CUT", 8.2f, 1836, ControlKind::Knob},
  {27, {487.8f, 90.4f, 529.5f, 145.4f}, "RES", 8.2f, 1836, ControlKind::Knob},
  {28, {435.5f, 159.8f, 477.2f, 214.8f}, "ENV", 8.2f, 1836, ControlKind::Knob},
  {29, {559.5f, 46.9f, 615.8f, 101.9f}, "ATT", 8.2f, 1836, ControlKind::Knob},
  {30, {626.2f, 46.9f, 682.5f, 101.9f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {31, {559.5f, 109.8f, 615.8f, 164.8f}, "SUS", 8.2f, 1836, ControlKind::Knob},
  {32, {626.2f, 109.8f, 682.5f, 164.8f}, "REL", 8.2f, 1836, ControlKind::Knob},
  {33, {458.5f, 396.3f, 520.5f, 451.3f}, "PATK", 8.2f, 1836, ControlKind::Knob},
  {34, {531.0f, 396.3f, 593.0f, 451.3f}, "PDEC", 8.2f, 1836, ControlKind::Knob},
  {35, {603.5f, 396.3f, 665.5f, 451.3f}, "PSUS", 8.2f, 1836, ControlKind::Knob},
  {36, {458.5f, 457.9f, 520.5f, 512.9f}, "PREL", 8.2f, 1836, ControlKind::Knob},
  {37, {531.0f, 457.9f, 593.0f, 512.9f}, "PAMT", 8.2f, 1836, ControlKind::Knob},
  {38, {62.3f, 248.0f, 121.7f, 322.2f}, "MASTER", 8.2f, 1836, ControlKind::Knob},
  {39, {559.5f, 172.7f, 615.8f, 227.7f}, "FATT", 8.2f, 1836, ControlKind::Knob},
  {40, {626.2f, 172.7f, 682.5f, 227.7f}, "FDEC", 8.2f, 1836, ControlKind::Knob},
  {41, {559.5f, 235.6f, 615.8f, 290.6f}, "FSUS", 8.2f, 1836, ControlKind::Knob},
  {42, {626.2f, 235.6f, 682.5f, 290.6f}, "FREL", 8.2f, 1836, ControlKind::Knob},
  {43, {201.5f, 272.2f, 295.8f, 293.2f}, "SYNC", 8.2f, 2448, ControlKind::Tab},
  {44, {306.2f, 272.2f, 400.5f, 293.2f}, "RING", 8.2f, 2448, ControlKind::Tab},
  {45, {487.8f, 159.8f, 529.5f, 214.8f}, "KEY", 8.2f, 1836, ControlKind::Knob},
  {46, {262.1f, 606.3f, 308.2f, 661.3f}, "COMP", 8.2f, 2448, ControlKind::Knob},
  {47, {717.5f, 105.7f, 759.4f, 160.7f}, "RATE", 8.2f, 2448, ControlKind::Knob},
  {48, {822.2f, 119.7f, 864.1f, 146.7f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {49, {874.6f, 119.7f, 916.5f, 146.7f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {50, {769.9f, 105.7f, 811.8f, 160.7f}, "DEP", 8.2f, 1836, ControlKind::Knob},
  {51, {721.8f, 670.8f, 786.5f, 709.8f}, "MWD", 8.2f, 1836, ControlKind::Dropdown},
  {52, {646.7f, 662.8f, 711.3f, 717.8f}, "MWA", 8.2f, 1836, ControlKind::Knob},
  {53, {435.5f, 229.1f, 477.2f, 284.1f}, "VEL", 8.2f, 1836, ControlKind::Knob},
  {54, {487.8f, 229.1f, 529.5f, 284.1f}, "FILT", 8.2f, 1836, ControlKind::Knob},
  {55, {751.9f, 600.2f, 786.5f, 639.2f}, "LEG", 8.2f, 1836, ControlKind::Dropdown},
  {56, {823.5f, 572.9f, 912.6f, 599.9f}, "I1", 8.2f, 1224, ControlKind::Dropdown},
  {57, {823.5f, 608.8f, 863.6f, 663.8f}, "A1", 8.2f, 1224, ControlKind::Knob},
  {58, {872.5f, 608.8f, 912.6f, 663.8f}, "B1", 8.2f, 1224, ControlKind::Knob},
  {59, {921.5f, 608.8f, 961.6f, 663.8f}, "C1", 8.2f, 1224, ControlKind::Knob},
  {60, {970.4f, 608.8f, 1010.5f, 663.8f}, "M1", 8.2f, 1224, ControlKind::Knob},
  {62, {921.5f, 572.9f, 1010.5f, 599.9f}, "I2", 8.2f, 1224, ControlKind::Dropdown},
  {63, {1019.4f, 608.8f, 1059.5f, 663.8f}, "A2", 8.2f, 1224, ControlKind::Knob},
  {64, {1068.4f, 608.8f, 1108.5f, 663.8f}, "B2", 8.2f, 1224, ControlKind::Knob},
  {65, {823.5f, 672.6f, 863.6f, 727.6f}, "C2", 8.2f, 1224, ControlKind::Knob},
  {66, {872.5f, 672.6f, 912.6f, 727.6f}, "M2", 8.2f, 1224, ControlKind::Knob},
  {68, {1019.4f, 572.9f, 1108.5f, 599.9f}, "MS", 8.2f, 1224, ControlKind::Dropdown},
  {69, {921.5f, 672.6f, 961.6f, 727.6f}, "MA", 8.2f, 1224, ControlKind::Knob},
  {70, {970.4f, 672.6f, 1010.5f, 727.6f}, "MB", 8.2f, 1224, ControlKind::Knob},
  {71, {1019.4f, 672.6f, 1059.5f, 727.6f}, "MC", 8.2f, 1224, ControlKind::Knob},
  {72, {1068.4f, 672.6f, 1108.5f, 727.6f}, "MX", 8.2f, 1224, ControlKind::Knob},
  {74, {341.3f, 668.6f, 398.7f, 723.6f}, "OUT", 8.2f, 1836, ControlKind::Knob},
  {75, {697.5f, 355.3f, 938.4f, 507.2f}, "PAD", 8.2f, 1836, ControlKind::Pad},
  {76, {948.9f, 355.3f, 995.1f, 410.3f}, "LVL", 8.2f, 1836, ControlKind::Knob},
  {77, {1005.6f, 355.3f, 1051.8f, 410.3f}, "PAN", 8.2f, 1836, ControlKind::Knob},
  {78, {1062.3f, 355.3f, 1108.5f, 410.3f}, "TUN", 8.2f, 1836, ControlKind::Knob},
  {79, {948.9f, 420.8f, 995.1f, 475.8f}, "DEC", 8.2f, 1836, ControlKind::Knob},
  {80, {1005.6f, 420.8f, 1051.8f, 475.8f}, "FLT", 8.2f, 1836, ControlKind::Knob},
  {81, {1062.3f, 420.8f, 1108.5f, 475.8f}, "DRV", 8.2f, 1836, ControlKind::Knob},
  {82, {1035.0f, 322.0f, 1066.5f, 343.0f}, "MUT", 8.2f, 1836, ControlKind::Tab},
  {83, {1077.0f, 322.0f, 1108.5f, 343.0f}, "SOL", 8.2f, 1836, ControlKind::Tab},
  {84, {603.5f, 457.9f, 665.5f, 512.9f}, "KIT", 8.2f, 1836, ControlKind::Knob},
  {85, {717.5f, 215.3f, 759.4f, 270.3f}, "RT", 8.2f, 1224, ControlKind::Knob},
  {86, {822.2f, 229.3f, 864.1f, 256.3f}, "SHP", 8.2f, 1836, ControlKind::Dropdown},
  {87, {874.6f, 229.3f, 916.5f, 256.3f}, "DST", 8.2f, 1836, ControlKind::Dropdown},
  {88, {769.9f, 215.3f, 811.8f, 270.3f}, "DEP", 8.2f, 1836, ControlKind::Knob},
  {89, {951.5f, 48.0f, 997.8f, 87.0f}, "S1", 8.2f, 1224, ControlKind::Dropdown},
  {90, {1008.3f, 48.0f, 1054.7f, 87.0f}, "D1", 8.2f, 1224, ControlKind::Dropdown},
  {91, {1065.2f, 40.0f, 1111.5f, 95.0f}, "A1", 8.2f, 1224, ControlKind::Knob},
  {92, {951.5f, 115.5f, 997.8f, 154.5f}, "S2", 8.2f, 1224, ControlKind::Dropdown},
  {93, {1008.3f, 115.5f, 1054.7f, 154.5f}, "D2", 8.2f, 1224, ControlKind::Dropdown},
  {94, {1065.2f, 107.5f, 1111.5f, 162.5f}, "A2", 8.2f, 1224, ControlKind::Knob},
  {95, {951.5f, 183.0f, 997.8f, 222.0f}, "S3", 8.2f, 1224, ControlKind::Dropdown},
  {96, {1008.3f, 183.0f, 1054.7f, 222.0f}, "D3", 8.2f, 1224, ControlKind::Dropdown},
  {97, {1065.2f, 175.0f, 1111.5f, 230.0f}, "A3", 8.2f, 1224, ControlKind::Knob},
  {98, {951.5f, 250.5f, 997.8f, 289.5f}, "S4", 8.2f, 1224, ControlKind::Dropdown},
  {99, {1008.3f, 250.5f, 1054.7f, 289.5f}, "D4", 8.2f, 1224, ControlKind::Dropdown},
  {100, {1065.2f, 242.5f, 1111.5f, 297.5f}, "A4", 8.2f, 1224, ControlKind::Knob},
  {101, {205.5f, 365.2f, 252.6f, 420.2f}, "NKEY", 8.2f, 1836, ControlKind::Knob},
  {102, {263.1f, 365.2f, 310.2f, 420.2f}, "AKEY", 8.2f, 1836, ControlKind::Knob},
  {103, {320.8f, 365.2f, 367.9f, 420.2f}, "ABND", 8.2f, 1836, ControlKind::Knob},
  {104, {378.4f, 365.2f, 425.5f, 420.2f}, "DKEY", 8.2f, 1836, ControlKind::Knob},
  {105, {205.5f, 442.3f, 252.6f, 497.3f}, "DBND", 8.2f, 1836, ControlKind::Knob},
  {106, {263.1f, 442.3f, 310.2f, 497.3f}, "SDLY", 8.2f, 1836, ControlKind::Knob},
  {107, {320.8f, 442.3f, 367.9f, 497.3f}, "STRK", 8.2f, 1836, ControlKind::Knob},
  {108, {378.4f, 442.3f, 425.5f, 497.3f}, "SWID", 8.2f, 1836, ControlKind::Knob},
  {109, {409.2f, 668.6f, 466.6f, 723.6f}, "DIR", 8.2f, 1836, ControlKind::Knob},
  {110, {477.1f, 668.6f, 534.5f, 723.6f}, "CBND", 8.2f, 1836, ControlKind::Knob},
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
