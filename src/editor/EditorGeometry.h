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

struct PanelSpec { const char* title; Box bounds; const char* tint; };
constexpr Box kPanelOsc {218.0f, 14.0f, 416.0f, 270.0f};
constexpr Box kPanelFilter {422.0f, 14.0f, 592.0f, 270.0f};
constexpr Box kPanelAmp {598.0f, 14.0f, 764.0f, 270.0f};
constexpr Box kPanelLfo {770.0f, 14.0f, 956.0f, 270.0f};
constexpr Box kPanelMod {962.0f, 14.0f, 1272.0f, 270.0f};
constexpr Box kPanelArt {218.0f, 276.0f, 698.0f, 580.0f};
constexpr Box kPanelSample {704.0f, 276.0f, 994.0f, 580.0f};
constexpr Box kPanelPads {1000.0f, 276.0f, 1272.0f, 580.0f};
constexpr Box kPanelVintage {218.0f, 586.0f, 558.0f, 786.0f};
constexpr Box kPanelGlobal {564.0f, 586.0f, 904.0f, 786.0f};
constexpr Box kPanelFx {910.0f, 586.0f, 1272.0f, 786.0f};
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
struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; };
inline constexpr ControlRect kControlRects[] = {
  {0, {572.0f, 640.0f, 636.0f, 704.0f}, "VOL", 10.0f, 1836},
  {1, {450.0f, 660.0f, 510.0f, 720.0f}, "DIST", 10.0f, 2448},
  {2, {322.0f, 660.0f, 382.0f, 720.0f}, "TONE", 10.0f, 2448},
  {3, {916.0f, 592.0f, 968.0f, 622.0f}, "WOW", 10.0f, 1836},
  {4, {974.0f, 592.0f, 1026.0f, 622.0f}, "FLUT", 10.0f, 2448},
  {5, {1032.0f, 592.0f, 1084.0f, 622.0f}, "HISS", 10.0f, 2448},
  {6, {1090.0f, 592.0f, 1142.0f, 622.0f}, "PRINT", 10.0f, 3060},
  {7, {1148.0f, 592.0f, 1200.0f, 622.0f}, "MIX", 10.0f, 1836},
  {8, {1206.0f, 592.0f, 1258.0f, 622.0f}, "BYP", 10.0f, 1836},
  {9, {226.0f, 620.0f, 316.0f, 648.0f}, "TYPE", 10.0f, 2448},
  {10, {642.0f, 600.0f, 732.0f, 628.0f}, "POLY", 10.0f, 2448},
  {11, {642.0f, 640.0f, 706.0f, 704.0f}, "GLIDE", 10.0f, 3060},
  {12, {712.0f, 640.0f, 776.0f, 704.0f}, "UNI", 10.0f, 1836},
  {13, {782.0f, 640.0f, 846.0f, 704.0f}, "DET", 10.0f, 1836},
  {14, {222.0f, 48.0f, 274.0f, 96.0f}, "SHP", 10.0f, 1836},
  {15, {222.0f, 104.0f, 268.0f, 152.0f}, "TUNE", 10.0f, 2448},
  {16, {222.0f, 158.0f, 268.0f, 206.0f}, "LVL", 10.0f, 1836},
  {17, {222.0f, 212.0f, 268.0f, 252.0f}, "PW", 10.0f, 1224},
  {18, {278.0f, 48.0f, 330.0f, 96.0f}, "SHP", 10.0f, 1836},
  {19, {278.0f, 104.0f, 324.0f, 152.0f}, "TUNE", 10.0f, 2448},
  {20, {278.0f, 158.0f, 324.0f, 206.0f}, "LVL", 10.0f, 1836},
  {21, {278.0f, 212.0f, 324.0f, 252.0f}, "PW", 10.0f, 1224},
  {22, {340.0f, 72.0f, 388.0f, 120.0f}, "SUB", 10.0f, 1836},
  {23, {340.0f, 126.0f, 388.0f, 174.0f}, "NOISE", 10.0f, 3060},
  {24, {340.0f, 180.0f, 388.0f, 228.0f}, "FM", 10.0f, 1224},
  {25, {426.0f, 36.0f, 586.0f, 62.0f}, "TYPE", 10.0f, 2448},
  {26, {426.0f, 72.0f, 488.0f, 140.0f}, "CUT", 10.0f, 1836},
  {27, {492.0f, 72.0f, 554.0f, 140.0f}, "RES", 10.0f, 1836},
  {28, {426.0f, 148.0f, 472.0f, 206.0f}, "ENV", 10.0f, 1836},
  {29, {600.0f, 36.0f, 636.0f, 172.0f}, "A", 10.0f, 612},
  {30, {640.0f, 36.0f, 676.0f, 172.0f}, "D", 10.0f, 612},
  {31, {680.0f, 36.0f, 716.0f, 172.0f}, "S", 10.0f, 612},
  {32, {720.0f, 36.0f, 756.0f, 172.0f}, "R", 10.0f, 612},
  {33, {710.0f, 286.0f, 988.0f, 318.0f}, "PA", 10.0f, 1224},
  {34, {710.0f, 320.0f, 988.0f, 352.0f}, "PD", 10.0f, 1224},
  {35, {710.0f, 354.0f, 988.0f, 386.0f}, "PS", 10.0f, 1224},
  {36, {710.0f, 388.0f, 988.0f, 420.0f}, "PR", 10.0f, 1224},
  {37, {710.0f, 422.0f, 988.0f, 454.0f}, "AMT", 10.0f, 1836},
  {38, {712.0f, 708.0f, 796.0f, 772.0f}, "SYN", 10.0f, 1836},
  {39, {556.0f, 148.0f, 590.0f, 174.0f}, "FA", 10.0f, 1224},
  {40, {556.0f, 178.0f, 590.0f, 204.0f}, "FD", 10.0f, 1224},
  {41, {556.0f, 208.0f, 590.0f, 234.0f}, "FS", 10.0f, 1224},
  {42, {556.0f, 238.0f, 590.0f, 264.0f}, "FR", 10.0f, 1224},
  {43, {222.0f, 254.0f, 290.0f, 268.0f}, "SYNC", 10.0f, 2448},
  {44, {296.0f, 254.0f, 364.0f, 268.0f}, "RING", 10.0f, 2448},
  {45, {476.0f, 148.0f, 522.0f, 206.0f}, "KEY", 10.0f, 1836},
  {46, {386.0f, 660.0f, 446.0f, 720.0f}, "COMP", 10.0f, 2448},
  {47, {776.0f, 40.0f, 820.0f, 84.0f}, "RATE", 10.0f, 2448},
  {48, {872.0f, 40.0f, 950.0f, 66.0f}, "SHP", 10.0f, 1836},
  {49, {872.0f, 70.0f, 950.0f, 96.0f}, "DST", 10.0f, 1836},
  {50, {824.0f, 40.0f, 868.0f, 84.0f}, "DEP", 10.0f, 1836},
  {51, {776.0f, 220.0f, 858.0f, 248.0f}, "MWD", 10.0f, 1836},
  {52, {776.0f, 252.0f, 834.0f, 270.0f}, "MWA", 10.0f, 1836},
  {53, {850.0f, 600.0f, 898.0f, 628.0f}, "VA", 10.0f, 1224},
  {54, {850.0f, 636.0f, 898.0f, 664.0f}, "VF", 10.0f, 1224},
  {55, {736.0f, 600.0f, 826.0f, 628.0f}, "LEG", 10.0f, 1836},
  {56, {916.0f, 626.0f, 968.0f, 656.0f}, "I1", 10.0f, 1224},
  {57, {974.0f, 626.0f, 1026.0f, 656.0f}, "A1", 10.0f, 1224},
  {58, {1032.0f, 626.0f, 1084.0f, 656.0f}, "B1", 10.0f, 1224},
  {59, {1090.0f, 626.0f, 1142.0f, 656.0f}, "C1", 10.0f, 1224},
  {60, {1148.0f, 626.0f, 1200.0f, 656.0f}, "M1", 10.0f, 1224},
  {61, {1206.0f, 626.0f, 1258.0f, 656.0f}, "B1", 10.0f, 1224},
  {62, {916.0f, 660.0f, 968.0f, 690.0f}, "I2", 10.0f, 1224},
  {63, {974.0f, 660.0f, 1026.0f, 690.0f}, "A2", 10.0f, 1224},
  {64, {1032.0f, 660.0f, 1084.0f, 690.0f}, "B2", 10.0f, 1224},
  {65, {1090.0f, 660.0f, 1142.0f, 690.0f}, "C2", 10.0f, 1224},
  {66, {1148.0f, 660.0f, 1200.0f, 690.0f}, "M2", 10.0f, 1224},
  {67, {1206.0f, 660.0f, 1258.0f, 690.0f}, "B2", 10.0f, 1224},
  {68, {916.0f, 694.0f, 968.0f, 724.0f}, "MS", 10.0f, 1224},
  {69, {974.0f, 694.0f, 1026.0f, 724.0f}, "MA", 10.0f, 1224},
  {70, {1032.0f, 694.0f, 1084.0f, 724.0f}, "MB", 10.0f, 1224},
  {71, {1090.0f, 694.0f, 1142.0f, 724.0f}, "MC", 10.0f, 1224},
  {72, {1148.0f, 694.0f, 1200.0f, 724.0f}, "MX", 10.0f, 1224},
  {73, {1206.0f, 694.0f, 1258.0f, 724.0f}, "BP", 10.0f, 1224},
  {74, {916.0f, 728.0f, 968.0f, 758.0f}, "OUT", 10.0f, 1836},
  {75, {1006.0f, 286.0f, 1266.0f, 570.0f}, "PAD", 10.0f, 1836},
  {76, {712.0f, 460.0f, 764.0f, 508.0f}, "LVL", 10.0f, 1836},
  {77, {768.0f, 460.0f, 820.0f, 508.0f}, "PAN", 10.0f, 1836},
  {78, {824.0f, 460.0f, 876.0f, 508.0f}, "TUN", 10.0f, 1836},
  {79, {880.0f, 460.0f, 932.0f, 508.0f}, "DEC", 10.0f, 1836},
  {80, {712.0f, 512.0f, 764.0f, 560.0f}, "FLT", 10.0f, 1836},
  {81, {768.0f, 512.0f, 820.0f, 560.0f}, "DRV", 10.0f, 1836},
  {82, {824.0f, 512.0f, 900.0f, 536.0f}, "MUT", 10.0f, 1836},
  {83, {904.0f, 512.0f, 980.0f, 536.0f}, "SOL", 10.0f, 1836},
  {84, {880.0f, 540.0f, 964.0f, 576.0f}, "KIT", 10.0f, 1836},
  {85, {776.0f, 150.0f, 820.0f, 194.0f}, "RT", 10.0f, 1224},
  {86, {872.0f, 150.0f, 950.0f, 176.0f}, "SHP", 10.0f, 1836},
  {87, {872.0f, 180.0f, 950.0f, 206.0f}, "DST", 10.0f, 1836},
  {88, {824.0f, 150.0f, 868.0f, 194.0f}, "DEP", 10.0f, 1836},
  {89, {968.0f, 36.0f, 1052.0f, 60.0f}, "S1", 10.0f, 1224},
  {90, {1056.0f, 36.0f, 1140.0f, 60.0f}, "D1", 10.0f, 1224},
  {91, {1144.0f, 36.0f, 1266.0f, 84.0f}, "A1", 10.0f, 1224},
  {92, {968.0f, 92.0f, 1052.0f, 116.0f}, "S2", 10.0f, 1224},
  {93, {1056.0f, 92.0f, 1140.0f, 116.0f}, "D2", 10.0f, 1224},
  {94, {1144.0f, 92.0f, 1266.0f, 140.0f}, "A2", 10.0f, 1224},
  {95, {968.0f, 148.0f, 1052.0f, 172.0f}, "S3", 10.0f, 1224},
  {96, {1056.0f, 148.0f, 1140.0f, 172.0f}, "D3", 10.0f, 1224},
  {97, {1144.0f, 148.0f, 1266.0f, 196.0f}, "A3", 10.0f, 1224},
  {98, {968.0f, 204.0f, 1052.0f, 228.0f}, "S4", 10.0f, 1224},
  {99, {1056.0f, 204.0f, 1140.0f, 228.0f}, "D4", 10.0f, 1224},
  {100, {1144.0f, 204.0f, 1266.0f, 252.0f}, "A4", 10.0f, 1224},
  {101, {600.0f, 176.0f, 630.0f, 214.0f}, "NK", 10.0f, 1224},
  {102, {632.0f, 176.0f, 662.0f, 214.0f}, "AK", 10.0f, 1224},
  {103, {664.0f, 176.0f, 694.0f, 214.0f}, "AB", 10.0f, 1224},
  {104, {696.0f, 176.0f, 726.0f, 214.0f}, "DK", 10.0f, 1224},
  {105, {728.0f, 176.0f, 758.0f, 214.0f}, "DB", 10.0f, 1224},
  {106, {600.0f, 220.0f, 630.0f, 258.0f}, "SD", 10.0f, 1224},
  {107, {632.0f, 220.0f, 662.0f, 258.0f}, "ST", 10.0f, 1224},
  {108, {664.0f, 220.0f, 694.0f, 258.0f}, "SW", 10.0f, 1224},
  {109, {696.0f, 220.0f, 726.0f, 258.0f}, "DIR", 10.0f, 1836},
  {110, {728.0f, 220.0f, 758.0f, 258.0f}, "CB", 10.0f, 1224},
};
constexpr int kNumControlRects = static_cast<int>(sizeof(kControlRects) / sizeof(ControlRect));
inline const ControlRect& ControlFor(int parameter) { for (const auto& item : kControlRects) if (item.parameter == parameter) return item; return kControlRects[0]; }
inline float EnvelopeTimeX(int segment, double value){const float v=static_cast<float>(std::clamp(value,0.,1.));if(segment==0)return 0.04f+0.22f*v;if(segment==1)return 0.30f+0.22f*v;return 0.70f+0.27f*v;}
inline double EnvelopeTimeValue(int segment,float x){if(segment==0)return std::clamp((x-0.04)/0.22,0.,1.);if(segment==1)return std::clamp((x-0.30)/0.22,0.,1.);return std::clamp((x-0.70)/0.27,0.,1.);}
inline float ScrollOffset(float offset,float delta,float content,float viewport){return std::clamp(offset-delta*32.f,0.f,std::max(0.f,content-viewport));}
constexpr float kSpaceMonoEm=0.612f; constexpr float kWordmarkPx=42.f;
inline float MonoAdvance(int units,float px){return static_cast<float>(units)*px/1000.f;}
inline float OswaldMetricHeight(float px){return px*1.482f;}
} }
