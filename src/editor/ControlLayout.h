#pragma once
#include <cassert>

namespace keith { namespace ui {
// Shared by the native controls and tools/render_materials.py. Pixel units.
struct KnobPlacement
{
  int parameter;
  const char* page;
  float left, top, right, bottom, radius;
  IRECT Bounds() const { return IRECT(left, top, right, bottom); }
};
inline const KnobPlacement kKnobPlacements[] = {
  {kParamGain, "shared", 1100.000000f, 28.000000f, 1240.000000f, 125.000000f, 24.000000f},
  {kParamOsc1Shape, "source", 56.000000f, 270.000000f, 222.000000f, 430.000000f, 51.000000f},
  {kParamOsc1Pitch, "source", 250.000000f, 343.000000f, 393.333333f, 435.000000f, 24.000000f},
  {kParamOsc1Level, "source", 413.333333f, 343.000000f, 556.666667f, 435.000000f, 24.000000f},
  {kParamOsc1PW, "source", 576.666667f, 343.000000f, 720.000000f, 435.000000f, 24.000000f},
  {kParamOsc2Shape, "source", 56.000000f, 506.000000f, 222.000000f, 666.000000f, 51.000000f},
  {kParamOsc2Pitch, "source", 250.000000f, 579.000000f, 393.333333f, 671.000000f, 24.000000f},
  {kParamOsc2Level, "source", 413.333333f, 579.000000f, 556.666667f, 671.000000f, 24.000000f},
  {kParamOsc2PW, "source", 576.666667f, 579.000000f, 720.000000f, 671.000000f, 24.000000f},
  {kParamSubLevel, "source", 784.000000f, 292.000000f, 876.000000f, 414.000000f, 24.000000f},
  {kParamNoiseLevel, "source", 888.000000f, 292.000000f, 980.000000f, 414.000000f, 24.000000f},
  {kParamFmAmount, "source", 784.000000f, 505.000000f, 876.000000f, 627.000000f, 24.000000f},
  {kParamSynthMasterLevel, "source", 888.000000f, 505.000000f, 980.000000f, 627.000000f, 24.000000f},
  {kParamPitchAmount, "pitch", 824.000000f, 350.000000f, 976.000000f, 528.000000f, 45.000000f},
  {kParamTapeSat, "tape", 58.000000f, 318.000000f, 228.000000f, 531.000000f, 61.000000f},
  {kParamTapeMix, "tape", 794.000000f, 318.000000f, 972.000000f, 531.000000f, 61.000000f},
  {kParamTapeBump, "tape", 280.000000f, 310.000000f, 421.333333f, 432.000000f, 24.000000f},
  {kParamTapeWow, "tape", 445.333333f, 310.000000f, 586.666667f, 432.000000f, 24.000000f},
  {kParamTapeFlutter, "tape", 610.666667f, 310.000000f, 752.000000f, 432.000000f, 24.000000f},
  {kParamTapeHiss, "tape", 308.000000f, 490.000000f, 470.000000f, 612.000000f, 24.000000f},
  {kParamTapePrint, "tape", 550.000000f, 490.000000f, 712.000000f, 612.000000f, 24.000000f},
  {kParamFilterCutoff, "shared", 1056.000000f, 312.000000f, 1230.000000f, 439.000000f, 38.000000f},
  {kParamFilterReso, "shared", 1043.000000f, 453.000000f, 1140.000000f, 548.000000f, 24.000000f},
  {kParamFilterEnv, "shared", 1148.000000f, 453.000000f, 1245.000000f, 548.000000f, 24.000000f},
  {kParamPorta, "shared", 1038.000000f, 648.000000f, 1104.000000f, 713.000000f, 24.000000f},
  {kParamUnisonVoices, "shared", 1110.000000f, 648.000000f, 1176.000000f, 713.000000f, 24.000000f},
  {kParamUnisonDetune, "shared", 1182.000000f, 648.000000f, 1248.000000f, 713.000000f, 24.000000f},
};
inline const KnobPlacement& PlacementFor(int parameter)
{
  for (const auto& item : kKnobPlacements)
    if (item.parameter == parameter) return item;
  assert(false && "Knob must have a physical placement");
  return kKnobPlacements[0];
}
} }
