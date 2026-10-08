#pragma once
#include "IControl.h"
#include "EditorGeometry.h"
#include <cassert>

namespace keith { namespace ui {
struct KnobPlacement { int parameter; const char* page; float left, top, right, bottom, radius; IRECT Bounds() const { return IRECT(left, top, right, bottom); } };
inline const KnobPlacement kKnobPlacements[] = {
  {0, "single", 572.0f, 640.0f, 636.0f, 704.0f, 32.0f},
  {1, "single", 450.0f, 660.0f, 510.0f, 720.0f, 30.0f},
  {2, "single", 322.0f, 660.0f, 382.0f, 720.0f, 30.0f},
  {11, "single", 642.0f, 640.0f, 706.0f, 704.0f, 32.0f},
  {12, "single", 712.0f, 640.0f, 776.0f, 704.0f, 32.0f},
  {13, "single", 782.0f, 640.0f, 846.0f, 704.0f, 32.0f},
  {14, "single", 222.0f, 48.0f, 274.0f, 96.0f, 24.0f},
  {15, "single", 222.0f, 104.0f, 268.0f, 152.0f, 23.0f},
  {16, "single", 222.0f, 158.0f, 268.0f, 206.0f, 23.0f},
  {17, "single", 222.0f, 212.0f, 268.0f, 252.0f, 20.0f},
  {18, "single", 278.0f, 48.0f, 330.0f, 96.0f, 24.0f},
  {19, "single", 278.0f, 104.0f, 324.0f, 152.0f, 23.0f},
  {20, "single", 278.0f, 158.0f, 324.0f, 206.0f, 23.0f},
  {21, "single", 278.0f, 212.0f, 324.0f, 252.0f, 20.0f},
  {22, "single", 340.0f, 72.0f, 388.0f, 120.0f, 24.0f},
  {23, "single", 340.0f, 126.0f, 388.0f, 174.0f, 24.0f},
  {24, "single", 340.0f, 180.0f, 388.0f, 228.0f, 24.0f},
  {26, "single", 426.0f, 72.0f, 488.0f, 140.0f, 31.0f},
  {27, "single", 492.0f, 72.0f, 554.0f, 140.0f, 31.0f},
  {28, "single", 426.0f, 148.0f, 472.0f, 206.0f, 23.0f},
  {29, "single", 600.0f, 36.0f, 636.0f, 172.0f, 18.0f},
  {30, "single", 640.0f, 36.0f, 676.0f, 172.0f, 18.0f},
  {31, "single", 680.0f, 36.0f, 716.0f, 172.0f, 18.0f},
  {32, "single", 720.0f, 36.0f, 756.0f, 172.0f, 18.0f},
  {33, "single", 710.0f, 286.0f, 988.0f, 318.0f, 16.0f},
  {34, "single", 710.0f, 320.0f, 988.0f, 352.0f, 16.0f},
  {35, "single", 710.0f, 354.0f, 988.0f, 386.0f, 16.0f},
  {36, "single", 710.0f, 388.0f, 988.0f, 420.0f, 16.0f},
  {37, "single", 710.0f, 422.0f, 988.0f, 454.0f, 16.0f},
  {38, "single", 712.0f, 708.0f, 796.0f, 772.0f, 32.0f},
  {45, "single", 476.0f, 148.0f, 522.0f, 206.0f, 23.0f},
  {46, "single", 386.0f, 660.0f, 446.0f, 720.0f, 30.0f},
  {47, "single", 776.0f, 40.0f, 820.0f, 84.0f, 22.0f},
  {50, "single", 824.0f, 40.0f, 868.0f, 84.0f, 22.0f},
  {75, "single", 1006.0f, 286.0f, 1266.0f, 570.0f, 130.0f},
  {76, "single", 712.0f, 460.0f, 764.0f, 508.0f, 24.0f},
  {77, "single", 768.0f, 460.0f, 820.0f, 508.0f, 24.0f},
  {78, "single", 824.0f, 460.0f, 876.0f, 508.0f, 24.0f},
  {79, "single", 880.0f, 460.0f, 932.0f, 508.0f, 24.0f},
  {80, "single", 712.0f, 512.0f, 764.0f, 560.0f, 24.0f},
  {81, "single", 768.0f, 512.0f, 820.0f, 560.0f, 24.0f},
  {84, "single", 880.0f, 540.0f, 964.0f, 576.0f, 18.0f},
  {85, "single", 776.0f, 150.0f, 820.0f, 194.0f, 22.0f},
  {88, "single", 824.0f, 150.0f, 868.0f, 194.0f, 22.0f},
  {91, "single", 1144.0f, 36.0f, 1266.0f, 84.0f, 24.0f},
  {94, "single", 1144.0f, 92.0f, 1266.0f, 140.0f, 24.0f},
  {97, "single", 1144.0f, 148.0f, 1266.0f, 196.0f, 24.0f},
  {100, "single", 1144.0f, 204.0f, 1266.0f, 252.0f, 24.0f},
};
inline const KnobPlacement& PlacementFor(int parameter){for(const auto& item:kKnobPlacements)if(item.parameter==parameter)return item;assert(false&&"Knob must have a physical placement");return kKnobPlacements[0];}
} }
