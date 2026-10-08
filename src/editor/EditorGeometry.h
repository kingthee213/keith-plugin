#pragma once
#include <algorithm>
#include <cmath>
#include <cstring>

namespace keith { namespace ui {

struct Box { float l,t,r,b; constexpr float W() const { return r-l; } constexpr float H() const { return b-t; } constexpr bool Contains(Box o) const { return o.l>=l&&o.t>=t&&o.r<=r&&o.b<=b; } constexpr bool Overlaps(Box o) const { return l<o.r&&o.l<r&&t<o.b&&o.t<b; } };

constexpr float kCanvasW = 1280.0f;
constexpr float kCanvasH = 800.0f;
constexpr float kLeftRailW = 210.0f;
constexpr bool kSingleWindowEditor = true;
constexpr Box kCanvas {0.f, 0.f, kCanvasW, kCanvasH};

// Knob vertical budget from KeithKnobControl::kLabelHeight / kValueHeight.
constexpr float kKnobLabelHeight = 19.f;
constexpr float kKnobValueHeight = 20.f;
constexpr float kKnobBodyMin = 34.f;
constexpr float kKnobMinHeight = kKnobBodyMin + kKnobLabelHeight + kKnobValueHeight;

enum class ControlKind { Knob, Tab, Dropdown, Fader, Pad };

struct PanelSpec { const char* title; Box bounds; const char* tint; };
constexpr float kPanelTitleBand = 26.0f;
constexpr Box kKeyboardBox {380.0f, 744.0f, 1248.0f, 789.0f};
inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }
constexpr Box kPanelOsc {218.0f, 14.0f, 456.0f, 350.0f};
constexpr Box kPanelFilter {458.0f, 14.0f, 634.0f, 350.0f};
constexpr Box kPanelAmp {636.0f, 14.0f, 876.0f, 350.0f};
constexpr Box kPanelLfo {878.0f, 14.0f, 1054.0f, 350.0f};
constexpr Box kPanelMod {1056.0f, 14.0f, 1272.0f, 350.0f};
constexpr Box kPanelArt {218.0f, 356.0f, 298.0f, 542.0f};
constexpr Box kPanelAmpMods {304.0f, 356.0f, 578.0f, 542.0f};
constexpr Box kPanelSample {584.0f, 356.0f, 848.0f, 542.0f};
constexpr Box kPanelPads {854.0f, 356.0f, 1272.0f, 542.0f};
constexpr Box kPanelVintage {218.0f, 548.0f, 368.0f, 743.0f};
constexpr Box kPanelGlobal {374.0f, 548.0f, 664.0f, 743.0f};
constexpr Box kPanelFx {670.0f, 548.0f, 1272.0f, 743.0f};
constexpr Box kLeftRail {0.0f, 0.0f, 210.0f, 800.0f};
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

constexpr Box kWordmarkBox {18.0f, 28.0f, 192.0f, 118.0f};

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
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };
inline constexpr ControlRect kControlRects[] = {
  {0, {382.0f, 574.0f, 433.3f, 647.0f}, "VOL", 10.0f, 1836, ControlKind::Knob},
  {1, {330.0f, 636.0f, 374.0f, 709.0f}, "DIST", 10.0f, 2448, ControlKind::Knob},
  {2, {226.0f, 636.0f, 270.0f, 709.0f}, "TONE", 10.0f, 2448, ControlKind::Knob},
  {3, {8.0f, 130.0f, 202.0f, 203.0f}, "WOW", 10.0f, 1836, ControlKind::Knob},
  {4, {8.0f, 207.0f, 202.0f, 280.0f}, "FLUT", 10.0f, 2448, ControlKind::Knob},
  {5, {8.0f, 284.0f, 202.0f, 357.0f}, "HISS", 10.0f, 2448, ControlKind::Knob},
  {6, {8.0f, 361.0f, 202.0f, 434.0f}, "PRINT", 10.0f, 3060, ControlKind::Knob},
  {7, {8.0f, 438.0f, 202.0f, 511.0f}, "MIX", 10.0f, 1836, ControlKind::Knob},
  {8, {8.0f, 594.0f, 202.0f, 622.0f}, "BYP", 10.0f, 1836, ControlKind::Tab},
  {9, {226.0f, 574.0f, 360.0f, 602.0f}, "TYPE", 10.0f, 2448, ControlKind::Tab},
  {10, {554.0f, 584.5f, 602.0f, 636.5f}, "POLY", 10.0f, 2448, ControlKind::Dropdown},
  {11, {439.3f, 574.0f, 490.7f, 647.0f}, "GLIDE", 10.0f, 3060, ControlKind::Knob},
  {12, {496.7f, 574.0f, 548.0f, 647.0f}, "UNI", 10.0f, 1836, ControlKind::Knob},
  {13, {382.0f, 662.0f, 443.3f, 735.0f}, "DET", 10.0f, 1836, ControlKind::Knob},
  {14, {226.0f, 40.0f, 280.0f, 113.0f}, "SHP", 10.0f, 1836, ControlKind::Knob},
  {15, {284.0f, 40.0f, 338.0f, 113.0f}, "TUNE", 10.0f, 2448, ControlKind::Knob},
  {16, {226.0f, 117.0f, 280.0f, 190.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {17, {284.0f, 117.0f, 338.0f, 190.0f}, "PW", 10.0f, 1224, ControlKind::Knob},
  {18, {342.0f, 40.0f, 396.0f, 113.0f}, "SHP", 10.0f, 1836, ControlKind::Knob},
  {19, {400.0f, 40.0f, 454.0f, 113.0f}, "TUNE", 10.0f, 2448, ControlKind::Knob},
  {20, {342.0f, 117.0f, 396.0f, 190.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {21, {400.0f, 117.0f, 454.0f, 190.0f}, "PW", 10.0f, 1224, ControlKind::Knob},
  {22, {226.0f, 198.0f, 296.0f, 271.0f}, "SUB", 10.0f, 1836, ControlKind::Knob},
  {23, {302.0f, 198.0f, 372.0f, 271.0f}, "NOISE", 10.0f, 3060, ControlKind::Knob},
  {24, {378.0f, 198.0f, 448.0f, 271.0f}, "FM", 10.0f, 1224, ControlKind::Knob},
  {25, {466.0f, 40.0f, 626.0f, 68.0f}, "TYPE", 10.0f, 2448, ControlKind::Tab},
  {26, {466.0f, 74.0f, 543.0f, 147.0f}, "CUT", 10.0f, 1836, ControlKind::Knob},
  {27, {549.0f, 74.0f, 626.0f, 147.0f}, "RES", 10.0f, 1836, ControlKind::Knob},
  {28, {466.0f, 153.0f, 543.0f, 226.0f}, "ENV", 10.0f, 1836, ControlKind::Knob},
  {29, {644.0f, 44.0f, 698.0f, 117.0f}, "ATT", 10.0f, 1836, ControlKind::Knob},
  {30, {704.0f, 44.0f, 758.0f, 117.0f}, "DEC", 10.0f, 1836, ControlKind::Knob},
  {31, {764.0f, 44.0f, 818.0f, 117.0f}, "SUS", 10.0f, 1836, ControlKind::Knob},
  {32, {824.0f, 44.0f, 878.0f, 117.0f}, "REL", 10.0f, 1836, ControlKind::Knob},
  {33, {592.0f, 382.0f, 649.5f, 455.0f}, "PATK", 10.0f, 1836, ControlKind::Knob},
  {34, {655.5f, 382.0f, 713.0f, 455.0f}, "PDEC", 10.0f, 1836, ControlKind::Knob},
  {35, {719.0f, 382.0f, 776.5f, 455.0f}, "PSUS", 10.0f, 1836, ControlKind::Knob},
  {36, {782.5f, 382.0f, 840.0f, 455.0f}, "PREL", 10.0f, 1836, ControlKind::Knob},
  {37, {592.0f, 461.0f, 649.5f, 534.0f}, "PAMT", 10.0f, 1836, ControlKind::Knob},
  {38, {449.3f, 662.0f, 510.7f, 735.0f}, "SYN", 10.0f, 1836, ControlKind::Knob},
  {39, {644.0f, 125.0f, 698.0f, 198.0f}, "FATT", 10.0f, 1836, ControlKind::Knob},
  {40, {704.0f, 125.0f, 758.0f, 198.0f}, "FDEC", 10.0f, 1836, ControlKind::Knob},
  {41, {764.0f, 125.0f, 818.0f, 198.0f}, "FSUS", 10.0f, 1836, ControlKind::Knob},
  {42, {824.0f, 125.0f, 878.0f, 198.0f}, "FREL", 10.0f, 1836, ControlKind::Knob},
  {43, {226.0f, 314.0f, 294.0f, 342.0f}, "SYNC", 10.0f, 2448, ControlKind::Tab},
  {44, {302.0f, 314.0f, 370.0f, 342.0f}, "RING", 10.0f, 2448, ControlKind::Tab},
  {45, {549.0f, 153.0f, 626.0f, 226.0f}, "KEY", 10.0f, 1836, ControlKind::Knob},
  {46, {278.0f, 636.0f, 322.0f, 709.0f}, "COMP", 10.0f, 2448, ControlKind::Knob},
  {47, {886.0f, 40.0f, 963.0f, 113.0f}, "RATE", 10.0f, 2448, ControlKind::Knob},
  {48, {886.0f, 119.0f, 963.0f, 171.0f}, "SHP", 10.0f, 1836, ControlKind::Dropdown},
  {49, {969.0f, 119.0f, 1046.0f, 171.0f}, "DST", 10.0f, 1836, ControlKind::Dropdown},
  {50, {969.0f, 40.0f, 1046.0f, 113.0f}, "DEP", 10.0f, 1836, ControlKind::Knob},
  {51, {584.0f, 662.0f, 656.0f, 714.0f}, "MWD", 10.0f, 1836, ControlKind::Dropdown},
  {52, {516.7f, 662.0f, 578.0f, 735.0f}, "MWA", 10.0f, 1836, ControlKind::Knob},
  {53, {466.0f, 232.0f, 543.0f, 305.0f}, "VEL", 10.0f, 1836, ControlKind::Knob},
  {54, {549.0f, 232.0f, 626.0f, 305.0f}, "FILT", 10.0f, 1836, ControlKind::Knob},
  {55, {608.0f, 584.5f, 656.0f, 636.5f}, "LEG", 10.0f, 1836, ControlKind::Dropdown},
  {56, {678.0f, 574.0f, 868.0f, 626.0f}, "I1", 10.0f, 1224, ControlKind::Dropdown},
  {57, {678.0f, 662.0f, 723.2f, 735.0f}, "A1", 10.0f, 1224, ControlKind::Knob},
  {58, {727.2f, 662.0f, 772.3f, 735.0f}, "B1", 10.0f, 1224, ControlKind::Knob},
  {59, {776.3f, 662.0f, 821.5f, 735.0f}, "C1", 10.0f, 1224, ControlKind::Knob},
  {60, {825.5f, 662.0f, 870.7f, 735.0f}, "M1", 10.0f, 1224, ControlKind::Knob},
  {62, {876.0f, 574.0f, 1066.0f, 626.0f}, "I2", 10.0f, 1224, ControlKind::Dropdown},
  {63, {874.7f, 662.0f, 919.8f, 735.0f}, "A2", 10.0f, 1224, ControlKind::Knob},
  {64, {923.8f, 662.0f, 969.0f, 735.0f}, "B2", 10.0f, 1224, ControlKind::Knob},
  {65, {973.0f, 662.0f, 1018.2f, 735.0f}, "C2", 10.0f, 1224, ControlKind::Knob},
  {66, {1022.2f, 662.0f, 1067.3f, 735.0f}, "M2", 10.0f, 1224, ControlKind::Knob},
  {68, {1074.0f, 574.0f, 1264.0f, 626.0f}, "MS", 10.0f, 1224, ControlKind::Dropdown},
  {69, {1071.3f, 662.0f, 1116.5f, 735.0f}, "MA", 10.0f, 1224, ControlKind::Knob},
  {70, {1120.5f, 662.0f, 1165.7f, 735.0f}, "MB", 10.0f, 1224, ControlKind::Knob},
  {71, {1169.7f, 662.0f, 1214.8f, 735.0f}, "MC", 10.0f, 1224, ControlKind::Knob},
  {72, {1218.8f, 662.0f, 1264.0f, 735.0f}, "MX", 10.0f, 1224, ControlKind::Knob},
  {74, {8.0f, 515.0f, 202.0f, 588.0f}, "OUT", 10.0f, 1836, ControlKind::Knob},
  {75, {862.0f, 382.0f, 1082.0f, 534.0f}, "PAD", 10.0f, 1836, ControlKind::Pad},
  {76, {1090.0f, 386.0f, 1144.0f, 459.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {77, {1150.0f, 386.0f, 1204.0f, 459.0f}, "PAN", 10.0f, 1836, ControlKind::Knob},
  {78, {1210.0f, 386.0f, 1264.0f, 459.0f}, "TUN", 10.0f, 1836, ControlKind::Knob},
  {79, {1090.0f, 463.0f, 1144.0f, 536.0f}, "DEC", 10.0f, 1836, ControlKind::Knob},
  {80, {1150.0f, 463.0f, 1204.0f, 536.0f}, "FLT", 10.0f, 1836, ControlKind::Knob},
  {81, {1210.0f, 463.0f, 1264.0f, 536.0f}, "DRV", 10.0f, 1836, ControlKind::Knob},
  {82, {719.0f, 483.5f, 772.5f, 511.5f}, "MUT", 10.0f, 1836, ControlKind::Tab},
  {83, {782.5f, 483.5f, 836.0f, 511.5f}, "SOL", 10.0f, 1836, ControlKind::Tab},
  {84, {655.5f, 461.0f, 713.0f, 534.0f}, "KIT", 10.0f, 1836, ControlKind::Knob},
  {85, {886.0f, 179.0f, 963.0f, 252.0f}, "RT", 10.0f, 1224, ControlKind::Knob},
  {86, {886.0f, 258.0f, 963.0f, 310.0f}, "SHP", 10.0f, 1836, ControlKind::Dropdown},
  {87, {969.0f, 258.0f, 1046.0f, 310.0f}, "DST", 10.0f, 1836, ControlKind::Dropdown},
  {88, {969.0f, 179.0f, 1046.0f, 252.0f}, "DEP", 10.0f, 1836, ControlKind::Knob},
  {89, {1064.0f, 40.0f, 1135.0f, 92.0f}, "S1", 10.0f, 1224, ControlKind::Dropdown},
  {90, {1139.0f, 40.0f, 1210.0f, 92.0f}, "D1", 10.0f, 1224, ControlKind::Dropdown},
  {91, {1210.0f, 40.0f, 1264.0f, 113.0f}, "A1", 10.0f, 1224, ControlKind::Knob},
  {92, {1064.0f, 117.0f, 1135.0f, 169.0f}, "S2", 10.0f, 1224, ControlKind::Dropdown},
  {93, {1139.0f, 117.0f, 1210.0f, 169.0f}, "D2", 10.0f, 1224, ControlKind::Dropdown},
  {94, {1210.0f, 117.0f, 1264.0f, 190.0f}, "A2", 10.0f, 1224, ControlKind::Knob},
  {95, {1064.0f, 194.0f, 1135.0f, 246.0f}, "S3", 10.0f, 1224, ControlKind::Dropdown},
  {96, {1139.0f, 194.0f, 1210.0f, 246.0f}, "D3", 10.0f, 1224, ControlKind::Dropdown},
  {97, {1210.0f, 194.0f, 1264.0f, 267.0f}, "A3", 10.0f, 1224, ControlKind::Knob},
  {98, {1064.0f, 271.0f, 1135.0f, 323.0f}, "S4", 10.0f, 1224, ControlKind::Dropdown},
  {99, {1139.0f, 271.0f, 1210.0f, 323.0f}, "D4", 10.0f, 1224, ControlKind::Dropdown},
  {100, {1210.0f, 271.0f, 1264.0f, 344.0f}, "A4", 10.0f, 1224, ControlKind::Knob},
  {101, {312.0f, 386.0f, 372.0f, 459.0f}, "NKEY", 10.0f, 1836, ControlKind::Knob},
  {102, {378.0f, 386.0f, 438.0f, 459.0f}, "AKEY", 10.0f, 1836, ControlKind::Knob},
  {103, {444.0f, 386.0f, 504.0f, 459.0f}, "ABND", 10.0f, 1836, ControlKind::Knob},
  {104, {510.0f, 386.0f, 570.0f, 459.0f}, "DKEY", 10.0f, 1836, ControlKind::Knob},
  {105, {312.0f, 465.0f, 372.0f, 538.0f}, "DBND", 10.0f, 1836, ControlKind::Knob},
  {106, {378.0f, 465.0f, 438.0f, 538.0f}, "SDLY", 10.0f, 1836, ControlKind::Knob},
  {107, {444.0f, 465.0f, 504.0f, 538.0f}, "STRK", 10.0f, 1836, ControlKind::Knob},
  {108, {510.0f, 465.0f, 570.0f, 538.0f}, "SWID", 10.0f, 1836, ControlKind::Knob},
  {109, {8.0f, 632.0f, 102.0f, 705.0f}, "DIR", 10.0f, 1836, ControlKind::Knob},
  {110, {108.0f, 632.0f, 202.0f, 705.0f}, "CBND", 10.0f, 1836, ControlKind::Knob},
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
