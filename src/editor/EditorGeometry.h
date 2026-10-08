#pragma once
#include <algorithm>
#include <cmath>

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
constexpr Box kPanelOsc {218.0f, 14.0f, 456.0f, 350.0f};
constexpr Box kPanelFilter {458.0f, 14.0f, 634.0f, 350.0f};
constexpr Box kPanelAmp {636.0f, 14.0f, 876.0f, 350.0f};
constexpr Box kPanelLfo {878.0f, 14.0f, 1054.0f, 350.0f};
constexpr Box kPanelMod {1056.0f, 14.0f, 1272.0f, 350.0f};
constexpr Box kPanelArt {218.0f, 356.0f, 578.0f, 542.0f};
constexpr Box kPanelSample {584.0f, 356.0f, 848.0f, 542.0f};
constexpr Box kPanelPads {854.0f, 356.0f, 1272.0f, 542.0f};
constexpr Box kPanelVintage {218.0f, 548.0f, 368.0f, 788.0f};
constexpr Box kPanelGlobal {374.0f, 548.0f, 664.0f, 788.0f};
constexpr Box kPanelFx {670.0f, 548.0f, 1272.0f, 788.0f};
constexpr Box kLeftRail {0.0f, 0.0f, 210.0f, 800.0f};
constexpr Box kWordmarkBox {18.0f, 28.0f, 192.0f, 118.0f};


constexpr PanelSpec kPanels[] = {
  {"OSCILLATORS", kPanelOsc, "panel"},
  {"FILTER", kPanelFilter, "panel"},
  {"AMP", kPanelAmp, "panel"},
  {"LFO", kPanelLfo, "panel"},
  {"MOD MATRIX", kPanelMod, "panel"},
  {"LOADED SOUNDS", kPanelSample, "panel"},
  {"DRUM PADS", kPanelPads, "panel"},
  {"VINTAGE ANALOG MODELING", kPanelVintage, "panel"},
  {"GLOBAL", kPanelGlobal, "panel"},
  {"EFFECTS", kPanelFx, "panel"},
};
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };
inline constexpr ControlRect kControlRects[] = {
  {0, {382.0f, 576.0f, 444.0f, 649.0f}, "VOL", 10.0f, 1836, ControlKind::Knob},
  {1, {330.0f, 659.5f, 374.0f, 732.5f}, "DIST", 10.0f, 2448, ControlKind::Knob},
  {2, {226.0f, 659.5f, 270.0f, 732.5f}, "TONE", 10.0f, 2448, ControlKind::Knob},
  {3, {678.0f, 576.0f, 756.6f, 649.0f}, "WOW", 10.0f, 1836, ControlKind::Knob},
  {4, {762.6f, 576.0f, 841.1f, 649.0f}, "FLUT", 10.0f, 2448, ControlKind::Knob},
  {5, {847.1f, 576.0f, 925.7f, 649.0f}, "HISS", 10.0f, 2448, ControlKind::Knob},
  {6, {931.7f, 576.0f, 1010.3f, 649.0f}, "PRINT", 10.0f, 3060, ControlKind::Knob},
  {7, {1016.3f, 576.0f, 1094.9f, 649.0f}, "MIX", 10.0f, 1836, ControlKind::Knob},
  {8, {1100.9f, 576.0f, 1179.4f, 604.0f}, "BYP", 10.0f, 1836, ControlKind::Tab},
  {9, {226.0f, 576.0f, 360.0f, 604.0f}, "TYPE", 10.0f, 2448, ControlKind::Tab},
  {10, {382.0f, 657.0f, 458.0f, 685.0f}, "POLY", 10.0f, 2448, ControlKind::Tab},
  {11, {452.0f, 576.0f, 514.0f, 649.0f}, "GLIDE", 10.0f, 3060, ControlKind::Knob},
  {12, {522.0f, 576.0f, 584.0f, 649.0f}, "UNI", 10.0f, 1836, ControlKind::Knob},
  {13, {592.0f, 576.0f, 654.0f, 649.0f}, "DET", 10.0f, 1836, ControlKind::Knob},
  {14, {226.0f, 42.0f, 280.0f, 115.0f}, "SHP", 10.0f, 1836, ControlKind::Knob},
  {15, {284.0f, 42.0f, 338.0f, 115.0f}, "TUNE", 10.0f, 2448, ControlKind::Knob},
  {16, {226.0f, 119.0f, 280.0f, 192.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {17, {284.0f, 119.0f, 338.0f, 192.0f}, "PW", 10.0f, 1224, ControlKind::Knob},
  {18, {342.0f, 42.0f, 396.0f, 115.0f}, "SHP", 10.0f, 1836, ControlKind::Knob},
  {19, {400.0f, 42.0f, 454.0f, 115.0f}, "TUNE", 10.0f, 2448, ControlKind::Knob},
  {20, {342.0f, 119.0f, 396.0f, 192.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {21, {400.0f, 119.0f, 454.0f, 192.0f}, "PW", 10.0f, 1224, ControlKind::Knob},
  {22, {226.0f, 200.0f, 296.0f, 273.0f}, "SUB", 10.0f, 1836, ControlKind::Knob},
  {23, {302.0f, 200.0f, 372.0f, 273.0f}, "NOISE", 10.0f, 3060, ControlKind::Knob},
  {24, {378.0f, 200.0f, 448.0f, 273.0f}, "FM", 10.0f, 1224, ControlKind::Knob},
  {25, {466.0f, 42.0f, 626.0f, 70.0f}, "TYPE", 10.0f, 2448, ControlKind::Tab},
  {26, {466.0f, 78.0f, 543.0f, 151.0f}, "CUT", 10.0f, 1836, ControlKind::Knob},
  {27, {549.0f, 78.0f, 626.0f, 151.0f}, "RES", 10.0f, 1836, ControlKind::Knob},
  {28, {466.0f, 157.0f, 543.0f, 230.0f}, "ENV", 10.0f, 1836, ControlKind::Knob},
  {29, {644.0f, 46.0f, 698.0f, 142.0f}, "A", 10.0f, 612, ControlKind::Fader},
  {30, {708.0f, 46.0f, 762.0f, 142.0f}, "D", 10.0f, 612, ControlKind::Fader},
  {31, {644.0f, 150.0f, 698.0f, 246.0f}, "S", 10.0f, 612, ControlKind::Fader},
  {32, {708.0f, 150.0f, 762.0f, 246.0f}, "R", 10.0f, 612, ControlKind::Fader},
  {33, {592.0f, 384.0f, 649.5f, 457.0f}, "PA", 10.0f, 1224, ControlKind::Knob},
  {34, {655.5f, 384.0f, 713.0f, 457.0f}, "PD", 10.0f, 1224, ControlKind::Knob},
  {35, {719.0f, 384.0f, 776.5f, 457.0f}, "PS", 10.0f, 1224, ControlKind::Knob},
  {36, {592.0f, 463.0f, 646.0f, 536.0f}, "PR", 10.0f, 1224, ControlKind::Knob},
  {37, {652.0f, 463.0f, 706.0f, 536.0f}, "AMT", 10.0f, 1836, ControlKind::Knob},
  {38, {382.0f, 693.0f, 446.0f, 766.0f}, "SYN", 10.0f, 1836, ControlKind::Knob},
  {39, {644.0f, 262.0f, 698.0f, 335.0f}, "FA", 10.0f, 1224, ControlKind::Knob},
  {40, {704.0f, 262.0f, 758.0f, 335.0f}, "FD", 10.0f, 1224, ControlKind::Knob},
  {41, {764.0f, 262.0f, 818.0f, 335.0f}, "FS", 10.0f, 1224, ControlKind::Knob},
  {42, {824.0f, 262.0f, 878.0f, 335.0f}, "FR", 10.0f, 1224, ControlKind::Knob},
  {43, {226.0f, 314.0f, 294.0f, 342.0f}, "SYNC", 10.0f, 2448, ControlKind::Tab},
  {44, {302.0f, 314.0f, 370.0f, 342.0f}, "RING", 10.0f, 2448, ControlKind::Tab},
  {45, {549.0f, 157.0f, 626.0f, 230.0f}, "KEY", 10.0f, 1836, ControlKind::Knob},
  {46, {278.0f, 659.5f, 322.0f, 732.5f}, "COMP", 10.0f, 2448, ControlKind::Knob},
  {47, {886.0f, 42.0f, 963.0f, 115.0f}, "RATE", 10.0f, 2448, ControlKind::Knob},
  {48, {886.0f, 121.0f, 963.0f, 173.0f}, "SHP", 10.0f, 1836, ControlKind::Dropdown},
  {49, {969.0f, 121.0f, 1046.0f, 173.0f}, "DST", 10.0f, 1836, ControlKind::Dropdown},
  {50, {969.0f, 42.0f, 1046.0f, 115.0f}, "DEP", 10.0f, 1836, ControlKind::Knob},
  {51, {452.0f, 693.0f, 532.0f, 745.0f}, "MWD", 10.0f, 1836, ControlKind::Dropdown},
  {52, {538.0f, 693.0f, 656.0f, 766.0f}, "MWA", 10.0f, 1836, ControlKind::Knob},
  {53, {466.0f, 236.0f, 543.0f, 309.0f}, "VA", 10.0f, 1224, ControlKind::Knob},
  {54, {549.0f, 236.0f, 626.0f, 309.0f}, "VF", 10.0f, 1224, ControlKind::Knob},
  {55, {464.0f, 657.0f, 540.0f, 685.0f}, "LEG", 10.0f, 1836, ControlKind::Tab},
  {56, {678.0f, 657.0f, 868.0f, 709.0f}, "I1", 10.0f, 1224, ControlKind::Dropdown},
  {57, {678.0f, 715.0f, 723.2f, 788.0f}, "A1", 10.0f, 1224, ControlKind::Knob},
  {58, {727.2f, 715.0f, 772.3f, 788.0f}, "B1", 10.0f, 1224, ControlKind::Knob},
  {59, {776.3f, 715.0f, 821.5f, 788.0f}, "C1", 10.0f, 1224, ControlKind::Knob},
  {60, {825.5f, 715.0f, 870.7f, 788.0f}, "M1", 10.0f, 1224, ControlKind::Knob},
  {62, {876.0f, 657.0f, 1066.0f, 709.0f}, "I2", 10.0f, 1224, ControlKind::Dropdown},
  {63, {874.7f, 715.0f, 919.8f, 788.0f}, "A2", 10.0f, 1224, ControlKind::Knob},
  {64, {923.8f, 715.0f, 969.0f, 788.0f}, "B2", 10.0f, 1224, ControlKind::Knob},
  {65, {973.0f, 715.0f, 1018.2f, 788.0f}, "C2", 10.0f, 1224, ControlKind::Knob},
  {66, {1022.2f, 715.0f, 1067.3f, 788.0f}, "M2", 10.0f, 1224, ControlKind::Knob},
  {68, {1074.0f, 657.0f, 1264.0f, 709.0f}, "MS", 10.0f, 1224, ControlKind::Dropdown},
  {69, {1071.3f, 715.0f, 1116.5f, 788.0f}, "MA", 10.0f, 1224, ControlKind::Knob},
  {70, {1120.5f, 715.0f, 1165.7f, 788.0f}, "MB", 10.0f, 1224, ControlKind::Knob},
  {71, {1169.7f, 715.0f, 1214.8f, 788.0f}, "MC", 10.0f, 1224, ControlKind::Knob},
  {72, {1218.8f, 715.0f, 1264.0f, 788.0f}, "MX", 10.0f, 1224, ControlKind::Knob},
  {74, {1185.4f, 576.0f, 1264.0f, 649.0f}, "OUT", 10.0f, 1836, ControlKind::Knob},
  {75, {862.0f, 384.0f, 1082.0f, 534.0f}, "PAD", 10.0f, 1836, ControlKind::Pad},
  {76, {1090.0f, 388.0f, 1144.0f, 461.0f}, "LVL", 10.0f, 1836, ControlKind::Knob},
  {77, {1150.0f, 388.0f, 1204.0f, 461.0f}, "PAN", 10.0f, 1836, ControlKind::Knob},
  {78, {1210.0f, 388.0f, 1264.0f, 461.0f}, "TUN", 10.0f, 1836, ControlKind::Knob},
  {79, {1090.0f, 465.0f, 1144.0f, 538.0f}, "DEC", 10.0f, 1836, ControlKind::Knob},
  {80, {1150.0f, 465.0f, 1204.0f, 538.0f}, "FLT", 10.0f, 1836, ControlKind::Knob},
  {81, {1210.0f, 465.0f, 1264.0f, 538.0f}, "DRV", 10.0f, 1836, ControlKind::Knob},
  {82, {712.0f, 485.5f, 776.0f, 513.5f}, "MUT", 10.0f, 1836, ControlKind::Tab},
  {83, {782.0f, 485.5f, 846.0f, 513.5f}, "SOL", 10.0f, 1836, ControlKind::Tab},
  {84, {782.5f, 384.0f, 840.0f, 457.0f}, "KIT", 10.0f, 1836, ControlKind::Knob},
  {85, {886.0f, 181.0f, 963.0f, 254.0f}, "RT", 10.0f, 1224, ControlKind::Knob},
  {86, {886.0f, 260.0f, 963.0f, 312.0f}, "SHP", 10.0f, 1836, ControlKind::Dropdown},
  {87, {969.0f, 260.0f, 1046.0f, 312.0f}, "DST", 10.0f, 1836, ControlKind::Dropdown},
  {88, {969.0f, 181.0f, 1046.0f, 254.0f}, "DEP", 10.0f, 1836, ControlKind::Knob},
  {89, {1064.0f, 42.0f, 1135.0f, 94.0f}, "S1", 10.0f, 1224, ControlKind::Dropdown},
  {90, {1139.0f, 42.0f, 1210.0f, 94.0f}, "D1", 10.0f, 1224, ControlKind::Dropdown},
  {91, {1210.0f, 42.0f, 1264.0f, 115.0f}, "A1", 10.0f, 1224, ControlKind::Knob},
  {92, {1064.0f, 119.0f, 1135.0f, 171.0f}, "S2", 10.0f, 1224, ControlKind::Dropdown},
  {93, {1139.0f, 119.0f, 1210.0f, 171.0f}, "D2", 10.0f, 1224, ControlKind::Dropdown},
  {94, {1210.0f, 119.0f, 1264.0f, 192.0f}, "A2", 10.0f, 1224, ControlKind::Knob},
  {95, {1064.0f, 196.0f, 1135.0f, 248.0f}, "S3", 10.0f, 1224, ControlKind::Dropdown},
  {96, {1139.0f, 196.0f, 1210.0f, 248.0f}, "D3", 10.0f, 1224, ControlKind::Dropdown},
  {97, {1210.0f, 196.0f, 1264.0f, 269.0f}, "A3", 10.0f, 1224, ControlKind::Knob},
  {98, {1064.0f, 273.0f, 1135.0f, 325.0f}, "S4", 10.0f, 1224, ControlKind::Dropdown},
  {99, {1139.0f, 273.0f, 1210.0f, 325.0f}, "D4", 10.0f, 1224, ControlKind::Dropdown},
  {100, {1210.0f, 273.0f, 1264.0f, 346.0f}, "A4", 10.0f, 1224, ControlKind::Knob},
  {101, {226.0f, 380.0f, 280.0f, 453.0f}, "NK", 10.0f, 1224, ControlKind::Knob},
  {102, {286.0f, 380.0f, 340.0f, 453.0f}, "AK", 10.0f, 1224, ControlKind::Knob},
  {103, {346.0f, 380.0f, 400.0f, 453.0f}, "AB", 10.0f, 1224, ControlKind::Knob},
  {104, {406.0f, 380.0f, 460.0f, 453.0f}, "DK", 10.0f, 1224, ControlKind::Knob},
  {105, {466.0f, 380.0f, 520.0f, 453.0f}, "DB", 10.0f, 1224, ControlKind::Knob},
  {106, {226.0f, 461.0f, 280.0f, 534.0f}, "SD", 10.0f, 1224, ControlKind::Knob},
  {107, {286.0f, 461.0f, 340.0f, 534.0f}, "ST", 10.0f, 1224, ControlKind::Knob},
  {108, {346.0f, 461.0f, 400.0f, 534.0f}, "SW", 10.0f, 1224, ControlKind::Knob},
  {109, {406.0f, 461.0f, 460.0f, 534.0f}, "DIR", 10.0f, 1836, ControlKind::Knob},
  {110, {466.0f, 461.0f, 520.0f, 534.0f}, "CB", 10.0f, 1224, ControlKind::Knob},
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
