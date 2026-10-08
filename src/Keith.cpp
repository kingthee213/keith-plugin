#include "Keith.h"
#include "IPlug_include_in_plug_src.h"

#include <initializer_list>

#if IPLUG_EDITOR
#include "IControls.h"
#include "editor/EditorialLayout.h"
#endif

namespace
{
constexpr uint32_t kDrumStateMagic = 0x4B445231u; // "KDR1"
constexpr uint32_t kDrumStateVersion = 1u;

// ---------------------------------------------------------------------------
// Baked factory presets.
//
// Neutral family labels only (BRIEF §6.5): every name is "Family 0N / NNN <neutral role>".
// Each preset is a delta list over the init defaults, stored through the same parameter
// serialisation the host uses, so RestorePreset moves real parameter state.
// ---------------------------------------------------------------------------
struct PresetDelta
{
  int preset;
  int param;
  double value;
};

const PresetDelta kPresetDeltas[] = {
  // Family 01 / 001 Glass Keys
  {0, kParamEngine, 0},       {0, kParamOsc1Shape, 20},   {0, kParamOsc1Level, 90},
  {0, kParamOsc2Shape, 10},   {0, kParamOsc2Pitch, 12},   {0, kParamOsc2Level, 55},
  {0, kParamFilterCutoff, 9000}, {0, kParamFilterReso, 8},   {0, kParamFilterEnv, 30},
  {0, kParamAmpAttack, 8},    {0, kParamAmpDecay, 800},   {0, kParamAmpSustain, 35},
  {0, kParamAmpRelease, 900}, {0, kParamUnisonVoices, 2}, {0, kParamUnisonDetune, 18},
  {0, kParamTapeSat, 20},     {0, kParamTapeMix, 30},
  {0, kParamIns1Algo, 2},     {0, kParamIns1A, 16},       {0, kParamIns1B, 26},
  {0, kParamIns1C, 55},       {0, kParamIns1Mix, 20},     {0, kParamIns2Algo, 3},
  {0, kParamIns2A, 24},       {0, kParamIns2B, 18},       {0, kParamIns2C, 62},
  {0, kParamIns2Mix, 14},     {0, kParamMstAlgo, 3},      {0, kParamMstA, 60},
  {0, kParamMstB, 32},        {0, kParamMstC, 50},        {0, kParamMstMix, 24},

  // Family 01 / 002 Sparse Keys
  {1, kParamOsc1Shape, 18},   {1, kParamOsc1Level, 75},   {1, kParamOsc2Shape, 22},
  {1, kParamOsc2Pitch, 12},   {1, kParamOsc2Level, 22},   {1, kParamFilterCutoff, 5200},
  {1, kParamFilterReso, 6},   {1, kParamFilterEnv, 15},   {1, kParamAmpAttack, 15},
  {1, kParamAmpDecay, 600},   {1, kParamAmpSustain, 30},  {1, kParamAmpRelease, 500},
  {1, kParamVelToAmp, 45},    {1, kParamTapeSat, 12},     {1, kParamTapeMix, 18},
  {1, kParamSynthMasterLevel, -4},
  {1, kParamIns1Algo, 3},     {1, kParamIns1A, 22},       {1, kParamIns1B, 18},
  {1, kParamIns1C, 60},       {1, kParamIns1Mix, 14},     {1, kParamMstAlgo, 3},
  {1, kParamMstA, 55},        {1, kParamMstB, 35},        {1, kParamMstC, 50},
  {1, kParamMstMix, 20},

  // Family 01 / 003 Soft Tines
  {2, kParamOsc1Shape, 22},   {2, kParamOsc2Shape, 30},   {2, kParamOsc2Pitch, 12},
  {2, kParamOsc2Level, 40},   {2, kParamFmAmount, 16},    {2, kParamFilterCutoff, 3800},
  {2, kParamFilterEnv, 25},   {2, kParamAmpAttack, 6},    {2, kParamAmpDecay, 1200},
  {2, kParamAmpSustain, 40},  {2, kParamAmpRelease, 700}, {2, kParamVelToAmp, 40},
  {2, kParamVelToFilter, 25}, {2, kParamTapeSat, 16},     {2, kParamTapeMix, 32},
  {2, kParamIns1Algo, 2},     {2, kParamIns1A, 18},       {2, kParamIns1B, 30},
  {2, kParamIns1C, 45},       {2, kParamIns1Mix, 20},     {2, kParamMstAlgo, 4},
  {2, kParamMstA, 56},        {2, kParamMstB, 42},        {2, kParamMstC, 12},
  {2, kParamMstMix, 18},

  // Family 01 / 004 Bright Clav
  {3, kParamOsc1Shape, 76},   {3, kParamOsc1PW, 28},      {3, kParamOsc2Shape, 88},
  {3, kParamOsc2Pitch, 12},   {3, kParamOsc2Level, 50},   {3, kParamOsc2PW, 22},
  {3, kParamFilterCutoff, 3400}, {3, kParamFilterReso, 34},  {3, kParamFilterEnv, 62},
  {3, kParamFiltAttack, 1},   {3, kParamFiltDecay, 160},  {3, kParamFiltSustain, 8},
  {3, kParamFiltRelease, 140}, {3, kParamAmpAttack, 1},    {3, kParamAmpDecay, 240},
  {3, kParamAmpSustain, 0},   {3, kParamAmpRelease, 150}, {3, kParamFilterDrive, 22},
  {3, kParamFilterKeyTrack, 30}, {3, kParamVelToAmp, 60},    {3, kParamVelToFilter, 45},
  {3, kParamTapeSat, 26},     {3, kParamTapeMix, 18},
  {3, kParamIns1Algo, 1},     {3, kParamIns1A, 40},       {3, kParamIns1B, 55},
  {3, kParamIns1C, 35},       {3, kParamIns1Mix, 28},     {3, kParamMstAlgo, 1},
  {3, kParamMstA, 58},        {3, kParamMstB, 38},        {3, kParamMstC, 34},
  {3, kParamMstMix, 26},

  // Family 01 / 005 Drawbar Organ
  {4, kParamOsc1Shape, 33},   {4, kParamOsc1Level, 85},   {4, kParamOsc2Shape, 33},
  {4, kParamOsc2Pitch, 12},   {4, kParamOsc2Level, 80},   {4, kParamSubLevel, 45},
  {4, kParamFilterCutoff, 7500}, {4, kParamFilterReso, 5},   {4, kParamFilterEnv, 0},
  {4, kParamAmpAttack, 4},    {4, kParamAmpDecay, 40},    {4, kParamAmpSustain, 100},
  {4, kParamAmpRelease, 80},  {4, kParamLfoRate, 5.5},    {4, kParamLfoDest, 1},
  {4, kParamLfoAmount, 8},    {4, kParamModWheelDest, 1}, {4, kParamModWheelAmount, 25},
  {4, kParamIns1Algo, 2},     {4, kParamIns1A, 12},       {4, kParamIns1B, 22},
  {4, kParamIns1C, 40},       {4, kParamIns1Mix, 18},     {4, kParamMstAlgo, 1},
  {4, kParamMstA, 55},        {4, kParamMstB, 30},        {4, kParamMstC, 40},
  {4, kParamMstMix, 22},

  // Family 01 / 006 Glass Bells
  {5, kParamOsc1Shape, 6},    {5, kParamOsc2Shape, 12},   {5, kParamOsc2Pitch, 19},
  {5, kParamOsc2Level, 48},   {5, kParamFmAmount, 32},    {5, kParamFilterCutoff, 11000},
  {5, kParamFilterReso, 6},   {5, kParamFilterEnv, 20},   {5, kParamFilterKeyTrack, 55},
  {5, kParamAmpAttack, 2},    {5, kParamAmpDecay, 2200},  {5, kParamAmpSustain, 0},
  {5, kParamAmpRelease, 2400}, {5, kParamPitchAmount, 2},  {5, kParamPitchDecay, 400},
  {5, kParamSynthMasterLevel, -4},
  {5, kParamIns1Algo, 3},     {5, kParamIns1A, 28},       {5, kParamIns1B, 22},
  {5, kParamIns1C, 70},       {5, kParamIns1Mix, 18},     {5, kParamIns2Algo, 2},
  {5, kParamIns2A, 14},       {5, kParamIns2B, 20},       {5, kParamIns2C, 64},
  {5, kParamIns2Mix, 16},     {5, kParamMstAlgo, 3},      {5, kParamMstA, 66},
  {5, kParamMstB, 28},        {5, kParamMstC, 50},        {5, kParamMstMix, 26},

  // Family 01 / 007 Crystal Pluck
  {6, kParamOsc1Shape, 14},   {6, kParamOsc2Shape, 40},   {6, kParamOsc2Pitch, 12},
  {6, kParamOsc2Level, 60},   {6, kParamFmAmount, 12},    {6, kParamFilterCutoff, 7200},
  {6, kParamFilterReso, 22},  {6, kParamFilterEnv, 68},   {6, kParamFilterKeyTrack, 78},
  {6, kParamFiltAttack, 1},   {6, kParamFiltDecay, 140},  {6, kParamFiltSustain, 0},
  {6, kParamFiltRelease, 160}, {6, kParamAmpAttack, 1},    {6, kParamAmpDecay, 200},
  {6, kParamAmpSustain, 0},   {6, kParamAmpRelease, 220}, {6, kParamVelToFilter, 30},
  {6, kParamIns1Algo, 3},     {6, kParamIns1A, 16},       {6, kParamIns1B, 14},
  {6, kParamIns1C, 68},       {6, kParamIns1Mix, 16},     {6, kParamMstAlgo, 1},
  {6, kParamMstA, 56},        {6, kParamMstB, 32},        {6, kParamMstC, 30},
  {6, kParamMstMix, 20},

  // Family 01 / 008 Key Stab
  {7, kParamOsc1Shape, 48},   {7, kParamOsc1Level, 90},   {7, kParamOsc1PW, 40},
  {7, kParamOsc2Shape, 70},   {7, kParamOsc2Pitch, 12},   {7, kParamOsc2Level, 65},
  {7, kParamOsc2PW, 35},      {7, kParamFilterCutoff, 6400}, {7, kParamFilterReso, 32},
  {7, kParamFilterEnv, 72},   {7, kParamFiltAttack, 1},   {7, kParamFiltDecay, 90},
  {7, kParamFiltSustain, 0},  {7, kParamFiltRelease, 80}, {7, kParamAmpAttack, 1},
  {7, kParamAmpDecay, 110},   {7, kParamAmpSustain, 0},   {7, kParamAmpRelease, 90},
  {7, kParamVelToAmp, 80},    {7, kParamVelToFilter, 65}, {7, kParamFilterKeyTrack, 35},
  {7, kParamFilterDrive, 18}, {7, kParamTapeSat, 28},     {7, kParamTapeMix, 22},
  {7, kParamIns1Algo, 3},     {7, kParamIns1A, 12},       {7, kParamIns1B, 10},
  {7, kParamIns1C, 55},       {7, kParamIns1Mix, 22},     {7, kParamIns2Algo, 1},
  {7, kParamIns2A, 34},       {7, kParamIns2B, 52},       {7, kParamIns2C, 38},
  {7, kParamIns2Mix, 20},     {7, kParamMstAlgo, 1},      {7, kParamMstA, 52},
  {7, kParamMstB, 44},        {7, kParamMstC, 24},        {7, kParamMstMix, 28},

  // Family 01 / 009 Dry Bass
  {8, kParamPolyMode, 1},     {8, kParamOsc1Shape, 62},   {8, kParamOsc1Pitch, -12},
  {8, kParamOsc1Level, 100},  {8, kParamOsc2Shape, 66},   {8, kParamOsc2Pitch, -12},
  {8, kParamOsc2Level, 70},   {8, kParamSubLevel, 70},    {8, kParamFilterCutoff, 1400},
  {8, kParamFilterReso, 22},  {8, kParamFilterEnv, 55},   {8, kParamAmpAttack, 2},
  {8, kParamAmpDecay, 300},   {8, kParamAmpSustain, 80},  {8, kParamAmpRelease, 180},
  {8, kParamTapeSat, 35},     {8, kParamTapeMix, 25},
  {8, kParamIns1Algo, 1},     {8, kParamIns1A, 32},       {8, kParamIns1B, 40},
  {8, kParamIns1C, 46},       {8, kParamIns1Mix, 22},     {8, kParamMstAlgo, 1},
  {8, kParamMstA, 62},        {8, kParamMstB, 42},        {8, kParamMstC, 22},
  {8, kParamMstMix, 26},

  // Family 01 / 010 Smooth Lead
  {9, kParamPolyMode, 1},     {9, kParamPorta, 90},       {9, kParamMonoLegato, 1},
  {9, kParamOsc1Shape, 58},   {9, kParamOsc2Shape, 48},   {9, kParamOsc2Pitch, 12},
  {9, kParamOsc2Level, 55},   {9, kParamUnisonVoices, 2}, {9, kParamUnisonDetune, 12},
  {9, kParamFilterCutoff, 5600}, {9, kParamFilterReso, 18},  {9, kParamFilterEnv, 35},
  {9, kParamFilterKeyTrack, 40}, {9, kParamAmpAttack, 12},   {9, kParamAmpDecay, 400},
  {9, kParamAmpSustain, 75},  {9, kParamAmpRelease, 280}, {9, kParamModWheelDest, 1},
  {9, kParamModWheelAmount, 40}, {9, kParamVelToAmp, 35},    {9, kParamFilterDrive, 12},
  {9, kParamIns1Algo, 3},     {9, kParamIns1A, 35},       {9, kParamIns1B, 30},
  {9, kParamIns1C, 62},       {9, kParamIns1Mix, 24},     {9, kParamIns2Algo, 2},
  {9, kParamIns2A, 12},       {9, kParamIns2B, 18},       {9, kParamIns2C, 58},
  {9, kParamIns2Mix, 14},     {9, kParamMstAlgo, 1},      {9, kParamMstA, 58},
  {9, kParamMstB, 34},        {9, kParamMstC, 36},        {9, kParamMstMix, 22},

  // Family 01 / 011 Warm Brass
  {10, kParamOsc1Shape, 52},  {10, kParamOsc2Shape, 46},  {10, kParamOsc2Pitch, 7},
  {10, kParamOsc2Level, 70},  {10, kParamUnisonVoices, 3}, {10, kParamUnisonDetune, 16},
  {10, kParamFilterCutoff, 2400}, {10, kParamFilterReso, 14}, {10, kParamFilterEnv, 58},
  {10, kParamFiltAttack, 30}, {10, kParamFiltDecay, 280}, {10, kParamFiltSustain, 55},
  {10, kParamFiltRelease, 400}, {10, kParamAmpAttack, 35},  {10, kParamAmpDecay, 500},
  {10, kParamAmpSustain, 78}, {10, kParamAmpRelease, 350}, {10, kParamVelToAmp, 40},
  {10, kParamVelToFilter, 55}, {10, kParamModWheelDest, 1}, {10, kParamModWheelAmount, 30},
  {10, kParamIns1Algo, 2},    {10, kParamIns1A, 16},      {10, kParamIns1B, 28},
  {10, kParamIns1C, 50},      {10, kParamIns1Mix, 22},    {10, kParamMstAlgo, 1},
  {10, kParamMstA, 58},       {10, kParamMstB, 40},       {10, kParamMstC, 45},
  {10, kParamMstMix, 35},

  // Family 01 / 012 Silk Strings
  {11, kParamOsc1Shape, 42},  {11, kParamOsc2Shape, 38},  {11, kParamOsc2Pitch, 12},
  {11, kParamOsc2Level, 75},  {11, kParamUnisonVoices, 5}, {11, kParamUnisonDetune, 22},
  {11, kParamFilterCutoff, 3200}, {11, kParamFilterReso, 8},  {11, kParamFilterEnv, 25},
  {11, kParamAmpAttack, 480}, {11, kParamAmpDecay, 1600}, {11, kParamAmpSustain, 82},
  {11, kParamAmpRelease, 1800}, {11, kParamFiltAttack, 600}, {11, kParamFiltDecay, 1400},
  {11, kParamFiltSustain, 60}, {11, kParamFiltRelease, 1600}, {11, kParamLfoRate, 0.18},
  {11, kParamLfoDest, 1},     {11, kParamLfoAmount, 12},  {11, kParamSynthMasterLevel, -3},
  {11, kParamIns1Algo, 2},    {11, kParamIns1A, 14},      {11, kParamIns1B, 36},
  {11, kParamIns1C, 48},      {11, kParamIns1Mix, 30},    {11, kParamIns2Algo, 3},
  {11, kParamIns2A, 42},      {11, kParamIns2B, 26},      {11, kParamIns2C, 52},
  {11, kParamIns2Mix, 16},    {11, kParamMstAlgo, 3},     {11, kParamMstA, 62},
  {11, kParamMstB, 30},       {11, kParamMstC, 50},       {11, kParamMstMix, 28},

  // Family 01 / 013 Soft Choir
  {12, kParamOsc1Shape, 26},  {12, kParamOsc2Shape, 34},  {12, kParamOsc2Pitch, 12},
  {12, kParamOsc2Level, 60},  {12, kParamNoiseLevel, 8},  {12, kParamUnisonVoices, 6},
  {12, kParamUnisonDetune, 20}, {12, kParamFilterCutoff, 4800}, {12, kParamFilterReso, 12},
  {12, kParamFilterEnv, 15},  {12, kParamAmpAttack, 620}, {12, kParamAmpDecay, 1800},
  {12, kParamAmpSustain, 80}, {12, kParamAmpRelease, 2200}, {12, kParamLfoRate, 0.22},
  {12, kParamLfoDest, 3},     {12, kParamLfoAmount, 10},  {12, kParamTapeHiss, 10},
  {12, kParamTapeMix, 34},    {12, kParamSynthMasterLevel, -3},
  {12, kParamIns1Algo, 2},    {12, kParamIns1A, 20},      {12, kParamIns1B, 40},
  {12, kParamIns1C, 55},      {12, kParamIns1Mix, 28},    {12, kParamIns2Algo, 3},
  {12, kParamIns2A, 46},      {12, kParamIns2B, 22},      {12, kParamIns2C, 48},
  {12, kParamIns2Mix, 18},    {12, kParamMstAlgo, 3},     {12, kParamMstA, 64},
  {12, kParamMstB, 26},       {12, kParamMstC, 50},       {12, kParamMstMix, 26},

  // Family 01 / 014 Bell Pad
  {13, kParamOsc1Shape, 8},   {13, kParamOsc2Shape, 88},  {13, kParamOsc2Pitch, 19},
  {13, kParamFmAmount, 25},   {13, kParamFilterCutoff, 6000}, {13, kParamAmpAttack, 400},
  {13, kParamAmpDecay, 2500}, {13, kParamAmpSustain, 60}, {13, kParamAmpRelease, 2500},
  {13, kParamUnisonVoices, 3}, {13, kParamUnisonDetune, 22}, {13, kParamSynthMasterLevel, -3},
  {13, kParamIns1Algo, 3},    {13, kParamIns1A, 45},      {13, kParamIns1B, 32},
  {13, kParamIns1C, 68},      {13, kParamIns1Mix, 24},    {13, kParamIns2Algo, 2},
  {13, kParamIns2A, 10},      {13, kParamIns2B, 34},      {13, kParamIns2C, 70},
  {13, kParamIns2Mix, 22},    {13, kParamMstAlgo, 3},     {13, kParamMstA, 68},
  {13, kParamMstB, 24},       {13, kParamMstC, 50},       {13, kParamMstMix, 30},

  // Family 01 / 015 Slow Motion
  {14, kParamOsc1Shape, 15},  {14, kParamOsc2Shape, 55},  {14, kParamOsc2Pitch, 7},
  {14, kParamOsc2Level, 65},  {14, kParamUnisonVoices, 4}, {14, kParamUnisonDetune, 36},
  {14, kParamFilterCutoff, 2200}, {14, kParamFilterReso, 24}, {14, kParamFilterEnv, 55},
  {14, kParamAmpAttack, 1100}, {14, kParamAmpDecay, 2800}, {14, kParamAmpSustain, 72},
  {14, kParamAmpRelease, 3200}, {14, kParamFiltAttack, 1400}, {14, kParamFiltDecay, 2600},
  {14, kParamFiltSustain, 50}, {14, kParamFiltRelease, 2800}, {14, kParamLfoRate, 0.08},
  {14, kParamLfoShape, 1},    {14, kParamLfoDest, 1},     {14, kParamLfoAmount, 48},
  {14, kParamSynthMasterLevel, -4},
  {14, kParamIns1Algo, 2},    {14, kParamIns1A, 8},       {14, kParamIns1B, 42},
  {14, kParamIns1C, 62},      {14, kParamIns1Mix, 24},    {14, kParamIns2Algo, 3},
  {14, kParamIns2A, 55},      {14, kParamIns2B, 40},      {14, kParamIns2C, 48},
  {14, kParamIns2Mix, 30},    {14, kParamMstAlgo, 3},     {14, kParamMstA, 70},
  {14, kParamMstB, 25},       {14, kParamMstC, 50},       {14, kParamMstMix, 40},

  // Family 01 / 016 Shimmer Texture
  {15, kParamOsc1Shape, 30},  {15, kParamOsc2Shape, 72},  {15, kParamOsc2Pitch, 7},
  {15, kParamOsc2Level, 50},  {15, kParamOscRing, 1},     {15, kParamNoiseLevel, 14},
  {15, kParamFilterType, 2},  {15, kParamFilterCutoff, 1800}, {15, kParamFilterReso, 40},
  {15, kParamFilterEnv, 30},  {15, kParamUnisonVoices, 3}, {15, kParamUnisonDetune, 40},
  {15, kParamAmpAttack, 700}, {15, kParamAmpDecay, 3000}, {15, kParamAmpSustain, 65},
  {15, kParamAmpRelease, 3000}, {15, kParamLfoRate, 0.35},  {15, kParamLfoShape, 4},
  {15, kParamLfoDest, 1},     {15, kParamLfoAmount, 35},
  {15, kParamIns1Algo, 4},    {15, kParamIns1A, 30},      {15, kParamIns1B, 25},
  {15, kParamIns1C, 20},      {15, kParamIns1Mix, 18},    {15, kParamIns2Algo, 3},
  {15, kParamIns2A, 38},      {15, kParamIns2B, 30},      {15, kParamIns2C, 58},
  {15, kParamIns2Mix, 20},    {15, kParamMstAlgo, 4},     {15, kParamMstA, 62},
  {15, kParamMstB, 40},       {15, kParamMstC, 15},       {15, kParamMstMix, 30},

  // Family 01 / 017 Low Drone
  {16, kParamOsc1Shape, 8},   {16, kParamOsc2Shape, 20},  {16, kParamOsc2Pitch, -12},
  {16, kParamOsc2Level, 70},  {16, kParamSubLevel, 55},   {16, kParamNoiseLevel, 10},
  {16, kParamFilterCutoff, 900}, {16, kParamFilterReso, 28}, {16, kParamFilterEnv, 10},
  {16, kParamAmpAttack, 1600}, {16, kParamAmpDecay, 4000}, {16, kParamAmpSustain, 95},
  {16, kParamAmpRelease, 4000}, {16, kParamLfoRate, 0.06},  {16, kParamLfoDest, 1},
  {16, kParamLfoAmount, 22},  {16, kParamFilterDrive, 15}, {16, kParamSynthMasterLevel, -6},
  {16, kParamIns1Algo, 2},    {16, kParamIns1A, 6},       {16, kParamIns1B, 30},
  {16, kParamIns1C, 44},      {16, kParamIns1Mix, 20},    {16, kParamIns2Algo, 3},
  {16, kParamIns2A, 62},      {16, kParamIns2B, 36},      {16, kParamIns2C, 30},
  {16, kParamIns2Mix, 18},    {16, kParamMstAlgo, 4},     {16, kParamMstA, 40},
  {16, kParamMstB, 28},       {16, kParamMstC, 10},       {16, kParamMstMix, 22},

  // Family 01 / 018 Glass Arp
  {17, kParamOsc1Shape, 18},  {17, kParamOsc2Shape, 24},  {17, kParamOsc2Pitch, 12},
  {17, kParamOsc2Level, 55},  {17, kParamUnisonVoices, 2}, {17, kParamUnisonDetune, 14},
  {17, kParamFilterCutoff, 6800}, {17, kParamFilterReso, 20}, {17, kParamFilterEnv, 50},
  {17, kParamFilterKeyTrack, 45}, {17, kParamAmpAttack, 1},   {17, kParamAmpDecay, 80},
  {17, kParamAmpSustain, 0},  {17, kParamAmpRelease, 120}, {17, kParamFiltAttack, 1},
  {17, kParamFiltDecay, 70},  {17, kParamFiltSustain, 0}, {17, kParamFiltRelease, 90},
  {17, kParamIns1Algo, 3},    {17, kParamIns1A, 18},      {17, kParamIns1B, 20},
  {17, kParamIns1C, 65},      {17, kParamIns1Mix, 16},    {17, kParamMstAlgo, 1},
  {17, kParamMstA, 54},       {17, kParamMstB, 30},       {17, kParamMstC, 26},
  {17, kParamMstMix, 20},

  // Family 01 / 019 Heavy Delay
  {18, kParamOsc1Shape, 40},  {18, kParamOsc2Shape, 60},  {18, kParamOsc2Pitch, 7},
  {18, kParamOsc2Level, 60},  {18, kParamFilterCutoff, 4000}, {18, kParamFilterReso, 16},
  {18, kParamFilterEnv, 40},  {18, kParamFilterDrive, 25}, {18, kParamAmpAttack, 20},
  {18, kParamAmpDecay, 900},  {18, kParamAmpSustain, 50}, {18, kParamAmpRelease, 800},
  {18, kParamTapeSat, 40},    {18, kParamTapeMix, 45},    {18, kParamOutLevel, -3},
  {18, kParamIns1Algo, 3},    {18, kParamIns1A, 72},      {18, kParamIns1B, 68},
  {18, kParamIns1C, 40},      {18, kParamIns1Mix, 78},    {18, kParamIns2Algo, 4},
  {18, kParamIns2A, 45},      {18, kParamIns2B, 35},      {18, kParamIns2C, 30},
  {18, kParamIns2Mix, 40},    {18, kParamMstAlgo, 1},     {18, kParamMstA, 48},
  {18, kParamMstB, 70},       {18, kParamMstC, 35},       {18, kParamMstMix, 65},

  // Family 01 / 020 Room Kit
  {19, kParamEngine, 1},      {19, kParamOsc1Shape, 100}, {19, kParamOsc1PW, 20},
  {19, kParamOsc2Shape, 90},  {19, kParamOsc2Pitch, -12}, {19, kParamOsc2Level, 75},
  {19, kParamNoiseLevel, 35}, {19, kParamFilterCutoff, 9000}, {19, kParamFilterReso, 8},
  {19, kParamFilterEnv, 10},  {19, kParamFilterDrive, 20}, {19, kParamAmpAttack, 0.5},
  {19, kParamAmpDecay, 150},  {19, kParamAmpSustain, 0},  {19, kParamAmpRelease, 120},
  {19, kParamPitchAmount, -14}, {19, kParamPitchAttack, 0.5}, {19, kParamPitchDecay, 80},
  {19, kParamPitchSustain, 0}, {19, kParamPitchRelease, 80}, {19, kParamTapeSat, 18},
  {19, kParamTapeMix, 16},    {19, kParamDrumSlotDecay, 48}, {19, kParamDrumSlotDrive, 40},
  {19, kParamDrumSlotFilter, 13000}, {19, kParamDrumKitLevel, -1},
  {19, kParamIns1Algo, 1},    {19, kParamIns1A, 35},      {19, kParamIns1B, 50},
  {19, kParamIns1C, 40},      {19, kParamIns1Mix, 25},    {19, kParamIns2Algo, 4},
  {19, kParamIns2A, 62},      {19, kParamIns2B, 42},      {19, kParamIns2C, 26},
  {19, kParamIns2Mix, 18},    {19, kParamMstAlgo, 2},     {19, kParamMstA, 30},
  {19, kParamMstB, 40},       {19, kParamMstC, 20},       {19, kParamMstMix, 50},


  // Family 01 / 021 Upright Bass. Analysis match of the acoustic bass references.
  {20, kParamOsc1Shape, 95},      {20, kParamOsc1Level, 55},  {20, kParamOsc2Level, 0},
  {20, kParamFilterCutoff, 6800}, {20, kParamFilterReso, 4},  {20, kParamFilterEnv, 0},
  {20, kParamFilterKeyTrack, 105},{20, kParamFilterKeyCurve, 10},
  {20, kParamAmpAttack, 30},      {20, kParamAmpAttackKey, 78.75}, {20, kParamAmpAttackCurve, 63.75},
  {20, kParamAmpDecay, 4862},     {20, kParamAmpDecayKey, -15.3}, {20, kParamAmpDecayCurve, 6.5},
  {20, kParamAmpSustain, 0},      {20, kParamAmpRelease, 40},
  {20, kParamSynthMasterLevel, -11.06}, {20, kParamTapeBypass, 1},
  {20, kParamDrumKitLevel, -60},
  {20, kParamIns1Algo, 1},        {20, kParamIns1A, 18},      {20, kParamIns1B, 22},
  {20, kParamIns1C, 14},          {20, kParamIns1Mix, 5},
  {20, kParamMstAlgo, 1},         {20, kParamMstA, 42},       {20, kParamMstB, 28},
  {20, kParamMstC, 18},           {20, kParamMstMix, 5},

  // Family 01 / 022 Hammer Keys. Analysis match of the acoustic piano references.
  {21, kParamOsc1Shape, 90},      {21, kParamOsc1Level, 55},  {21, kParamOsc2Level, 0},
  {21, kParamFilterCutoff, 12500},{21, kParamFilterReso, 4},  {21, kParamFilterEnv, 0},
  {21, kParamFilterKeyTrack, 48}, {21, kParamFilterKeyCurve, 2},
  {21, kParamAmpAttack, 4.5},     {21, kParamAmpAttackKey, 5.55}, {21, kParamAmpAttackCurve, 26.85},
  {21, kParamAmpDecay, 12500},    {21, kParamAmpDecayKey, -25.25}, {21, kParamAmpDecayCurve, -15.2},
  {21, kParamAmpSustain, 0},      {21, kParamAmpRelease, 80},
  {21, kParamSynthMasterLevel, -13.15}, {21, kParamTapeBypass, 1},
  {21, kParamDrumKitLevel, -60},
  {21, kParamIns1Algo, 2},        {21, kParamIns1A, 10},      {21, kParamIns1B, 8},
  {21, kParamIns1C, 16},          {21, kParamIns1Mix, 1},
  {21, kParamMstAlgo, 4},         {21, kParamMstA, 55},       {21, kParamMstB, 22},
  {21, kParamMstC, 12},           {21, kParamMstMix, 12},

  // Family 01 / 023 Swell Choir. Analysis match of the slow choir reference.
  {22, kParamOsc1Shape, 0},       {22, kParamOsc1Level, 40},  {22, kParamOsc2Level, 0},
  {22, kParamNoiseLevel, 22},     {22, kParamDirectLevel, 16},
  {22, kParamFilterCutoff, 2300}, {22, kParamFilterReso, 20}, {22, kParamFilterEnv, 0},
  {22, kParamAmpAttack, 3105},    {22, kParamAmpDecay, 3366},
  {22, kParamAmpSustain, 0},      {22, kParamAmpRelease, 200},
  {22, kParamStereoDelay, 2.6},   {22, kParamStereoSpread, 50},
  {22, kParamSynthMasterLevel, -10.46}, {22, kParamTapeBypass, 1},
  {22, kParamDrumKitLevel, -60},
  {22, kParamIns1Algo, 2},        {22, kParamIns1A, 6},       {22, kParamIns1B, 14},
  {22, kParamIns1C, 28},          {22, kParamIns1Mix, 5},
  {22, kParamMstAlgo, 3},         {22, kParamMstA, 46},       {22, kParamMstB, 24},
  {22, kParamMstC, 18},           {22, kParamMstMix, 5},

  // Family 02 / 001 Neon Keys
  {23, kParamEngine, 1},      {23, kParamOsc1Shape, 36},  {23, kParamOsc1Level, 88},
  {23, kParamOsc2Shape, 58},  {23, kParamOsc2Pitch, 12},  {23, kParamOsc2Level, 62},
  {23, kParamFilterCutoff, 7800}, {23, kParamFilterReso, 16}, {23, kParamFilterEnv, 28},
  {23, kParamFilterDrive, 24}, {23, kParamAmpAttack, 4},   {23, kParamAmpDecay, 700},
  {23, kParamAmpSustain, 42}, {23, kParamAmpRelease, 420}, {23, kParamUnisonVoices, 2},
  {23, kParamUnisonDetune, 26}, {23, kParamVelToAmp, 55},   {23, kParamTapeSat, 38},
  {23, kParamTapeMix, 22},
  {23, kParamIns1Algo, 1},    {23, kParamIns1A, 48},      {23, kParamIns1B, 42},
  {23, kParamIns1C, 38},      {23, kParamIns1Mix, 30},    {23, kParamMstAlgo, 1},
  {23, kParamMstA, 56},       {23, kParamMstB, 36},       {23, kParamMstC, 32},
  {23, kParamMstMix, 26},

  // Family 02 / 002 Thin Piano
  {24, kParamEngine, 1},      {24, kParamOsc1Shape, 16},  {24, kParamOsc2Shape, 28},
  {24, kParamOsc2Pitch, 12},  {24, kParamOsc2Level, 18},  {24, kParamFmAmount, 8},
  {24, kParamFilterCutoff, 2600}, {24, kParamFilterReso, 8},  {24, kParamFilterEnv, 18},
  {24, kParamAmpAttack, 4},   {24, kParamAmpDecay, 700},  {24, kParamAmpSustain, 22},
  {24, kParamAmpRelease, 480}, {24, kParamVelToAmp, 62},   {24, kParamVelToFilter, 20},
  {24, kParamTapeSat, 14},    {24, kParamTapeMix, 16},    {24, kParamSynthMasterLevel, -2},
  {24, kParamIns1Algo, 2},    {24, kParamIns1A, 12},      {24, kParamIns1B, 18},
  {24, kParamIns1C, 35},      {24, kParamIns1Mix, 12},    {24, kParamMstAlgo, 4},
  {24, kParamMstA, 58},       {24, kParamMstB, 46},       {24, kParamMstC, 10},
  {24, kParamMstMix, 16},

  // Family 02 / 003 Thin Clav
  {25, kParamEngine, 1},      {25, kParamOsc1Shape, 82},  {25, kParamOsc1PW, 18},
  {25, kParamOsc2Shape, 94},  {25, kParamOsc2Pitch, 12},  {25, kParamOsc2Level, 28},
  {25, kParamOsc2PW, 16},     {25, kParamFilterCutoff, 4100}, {25, kParamFilterReso, 38},
  {25, kParamFilterEnv, 70},  {25, kParamFilterDrive, 30}, {25, kParamFilterKeyTrack, 48},
  {25, kParamFiltAttack, 1},  {25, kParamFiltDecay, 120}, {25, kParamFiltSustain, 0},
  {25, kParamFiltRelease, 100}, {25, kParamAmpAttack, 1},   {25, kParamAmpDecay, 180},
  {25, kParamAmpSustain, 0},  {25, kParamAmpRelease, 130}, {25, kParamVelToAmp, 70},
  {25, kParamIns1Algo, 1},    {25, kParamIns1A, 55},      {25, kParamIns1B, 40},
  {25, kParamIns1C, 32},      {25, kParamIns1Mix, 34},    {25, kParamMstAlgo, 1},
  {25, kParamMstA, 54},       {25, kParamMstB, 46},       {25, kParamMstC, 22},
  {25, kParamMstMix, 28},

  // Family 02 / 004 Perc Organ
  {26, kParamEngine, 1},      {26, kParamOsc1Shape, 40},  {26, kParamOsc1Level, 95},
  {26, kParamOsc2Shape, 33},  {26, kParamOsc2Pitch, 19},  {26, kParamOsc2Level, 55},
  {26, kParamSubLevel, 25},   {26, kParamFilterCutoff, 6200}, {26, kParamFilterReso, 18},
  {26, kParamFilterEnv, 45},  {26, kParamAmpAttack, 1},   {26, kParamAmpDecay, 180},
  {26, kParamAmpSustain, 85}, {26, kParamAmpRelease, 70}, {26, kParamFiltAttack, 1},
  {26, kParamFiltDecay, 140}, {26, kParamFiltSustain, 40}, {26, kParamFiltRelease, 120},
  {26, kParamVelToAmp, 35},   {26, kParamLfoRate, 6.5},   {26, kParamLfoDest, 1},
  {26, kParamLfoAmount, 12},
  {26, kParamIns1Algo, 1},    {26, kParamIns1A, 36},      {26, kParamIns1B, 48},
  {26, kParamIns1C, 42},      {26, kParamIns1Mix, 22},    {26, kParamMstAlgo, 1},
  {26, kParamMstA, 52},       {26, kParamMstB, 38},       {26, kParamMstC, 30},
  {26, kParamMstMix, 24},

  // Family 02 / 005 Metal Bells
  {27, kParamEngine, 1},      {27, kParamOsc1Shape, 10},  {27, kParamOsc2Shape, 80},
  {27, kParamOsc2Pitch, 19},  {27, kParamOsc2Level, 70},  {27, kParamFmAmount, 40},
  {27, kParamOscRing, 1},     {27, kParamFilterCutoff, 9800}, {27, kParamFilterReso, 20},
  {27, kParamFilterKeyTrack, 60}, {27, kParamAmpAttack, 1},   {27, kParamAmpDecay, 1600},
  {27, kParamAmpSustain, 0},  {27, kParamAmpRelease, 1800}, {27, kParamFilterDrive, 18},
  {27, kParamSynthMasterLevel, -5},
  {27, kParamIns1Algo, 3},    {27, kParamIns1A, 24},      {27, kParamIns1B, 26},
  {27, kParamIns1C, 58},      {27, kParamIns1Mix, 20},    {27, kParamIns2Algo, 1},
  {27, kParamIns2A, 42},      {27, kParamIns2B, 56},      {27, kParamIns2C, 40},
  {27, kParamIns2Mix, 18},    {27, kParamMstAlgo, 3},     {27, kParamMstA, 62},
  {27, kParamMstB, 30},       {27, kParamMstC, 50},       {27, kParamMstMix, 24},

  // Family 02 / 006 Neon Pluck
  {28, kParamOsc1Shape, 70},  {28, kParamOsc2Shape, 40},  {28, kParamOsc2Pitch, 12},
  {28, kParamFilterCutoff, 3500}, {28, kParamFilterReso, 30}, {28, kParamFilterEnv, 75},
  {28, kParamFiltAttack, 1},  {28, kParamFiltDecay, 180}, {28, kParamFiltSustain, 0},
  {28, kParamFiltRelease, 200}, {28, kParamAmpAttack, 2},   {28, kParamAmpDecay, 220},
  {28, kParamAmpSustain, 0},  {28, kParamAmpRelease, 250}, {28, kParamFilterKeyTrack, 75},
  {28, kParamIns1Algo, 1},    {28, kParamIns1A, 44},      {28, kParamIns1B, 50},
  {28, kParamIns1C, 36},      {28, kParamIns1Mix, 24},    {28, kParamMstAlgo, 1},
  {28, kParamMstA, 55},       {28, kParamMstB, 34},       {28, kParamMstC, 26},
  {28, kParamMstMix, 22},

  // Family 02 / 007 Sparse Pluck
  {29, kParamEngine, 1},      {29, kParamOsc1Shape, 64},  {29, kParamOsc1Level, 80},
  {29, kParamOsc2Level, 12},  {29, kParamFilterCutoff, 4800}, {29, kParamFilterReso, 14},
  {29, kParamFilterEnv, 60},  {29, kParamFilterKeyTrack, 85}, {29, kParamFiltAttack, 1},
  {29, kParamFiltDecay, 90},  {29, kParamFiltSustain, 0}, {29, kParamFiltRelease, 80},
  {29, kParamAmpAttack, 1},   {29, kParamAmpDecay, 140},  {29, kParamAmpSustain, 0},
  {29, kParamAmpRelease, 160}, {29, kParamTapeMix, 12},    {29, kParamSynthMasterLevel, -3},
  {29, kParamIns1Algo, 3},    {29, kParamIns1A, 14},      {29, kParamIns1B, 10},
  {29, kParamIns1C, 72},      {29, kParamIns1Mix, 8},     {29, kParamMstAlgo, 1},
  {29, kParamMstA, 58},       {29, kParamMstB, 28},       {29, kParamMstC, 24},
  {29, kParamMstMix, 14},

  // Family 02 / 008 Thin Stab
  {30, kParamPolyMode, 1},    {30, kParamOsc1Shape, 55},  {30, kParamOsc1Level, 85},
  {30, kParamOsc2Shape, 100}, {30, kParamOsc2PW, 20},     {30, kParamOsc2Level, 60},
  {30, kParamFilterCutoff, 5200}, {30, kParamFilterReso, 18}, {30, kParamFilterEnv, 50},
  {30, kParamAmpAttack, 1},   {30, kParamAmpDecay, 120},  {30, kParamAmpSustain, 0},
  {30, kParamAmpRelease, 120}, {30, kParamTapeSat, 40},
  {30, kParamIns1Algo, 1},    {30, kParamIns1A, 52},      {30, kParamIns1B, 46},
  {30, kParamIns1C, 34},      {30, kParamIns1Mix, 30},    {30, kParamMstAlgo, 1},
  {30, kParamMstA, 50},       {30, kParamMstB, 48},       {30, kParamMstC, 20},
  {30, kParamMstMix, 30},

  // Family 02 / 009 Hard Bass
  {31, kParamEngine, 1},      {31, kParamPolyMode, 1},    {31, kParamOscSync, 1},
  {31, kParamOsc1Shape, 74},  {31, kParamOsc1Pitch, -12}, {31, kParamOsc2Shape, 80},
  {31, kParamOsc2Pitch, -12}, {31, kParamOsc2Level, 85},  {31, kParamSubLevel, 80},
  {31, kParamFilterCutoff, 900}, {31, kParamFilterReso, 28}, {31, kParamFilterEnv, 65},
  {31, kParamFilterDrive, 42}, {31, kParamFilterKeyTrack, 25}, {31, kParamAmpAttack, 1},
  {31, kParamAmpDecay, 260},  {31, kParamAmpSustain, 70}, {31, kParamAmpRelease, 140},
  {31, kParamVelToAmp, 45},   {31, kParamTapeSat, 48},    {31, kParamTapeMix, 20},
  {31, kParamIns1Algo, 1},    {31, kParamIns1A, 70},      {31, kParamIns1B, 35},
  {31, kParamIns1C, 28},      {31, kParamIns1Mix, 40},    {31, kParamMstAlgo, 2},
  {31, kParamMstA, 36},       {31, kParamMstB, 30},       {31, kParamMstC, 18},
  {31, kParamMstMix, 55},

  // Family 02 / 010 Sparse Bass
  {32, kParamEngine, 1},      {32, kParamPolyMode, 1},    {32, kParamMonoLegato, 1},
  {32, kParamOsc1Shape, 60},  {32, kParamOsc1Pitch, -12}, {32, kParamOsc2Level, 16},
  {32, kParamSubLevel, 85},   {32, kParamFilterCutoff, 700}, {32, kParamFilterReso, 12},
  {32, kParamFilterEnv, 30},  {32, kParamAmpAttack, 2},   {32, kParamAmpDecay, 220},
  {32, kParamAmpSustain, 55}, {32, kParamAmpRelease, 160}, {32, kParamVelToAmp, 30},
  {32, kParamTapeSat, 8},     {32, kParamTapeMix, 8},     {32, kParamSynthMasterLevel, -2},
  {32, kParamIns1Algo, 1},    {32, kParamIns1A, 22},      {32, kParamIns1B, 34},
  {32, kParamIns1C, 48},      {32, kParamIns1Mix, 14},    {32, kParamMstAlgo, 1},
  {32, kParamMstA, 60},       {32, kParamMstB, 30},       {32, kParamMstC, 24},
  {32, kParamMstMix, 18},

  // Family 02 / 011 Sync Lead
  {33, kParamEngine, 1},      {33, kParamPolyMode, 1},    {33, kParamOscSync, 1},
  {33, kParamPorta, 140},     {33, kParamOsc1Shape, 68},  {33, kParamOsc2Shape, 72},
  {33, kParamOsc2Pitch, 7},   {33, kParamOsc2Level, 75},  {33, kParamFilterCutoff, 2400},
  {33, kParamFilterReso, 36}, {33, kParamFilterEnv, 48},  {33, kParamFilterDrive, 35},
  {33, kParamFilterKeyTrack, 50}, {33, kParamAmpAttack, 6},   {33, kParamAmpDecay, 280},
  {33, kParamAmpSustain, 80}, {33, kParamAmpRelease, 200}, {33, kParamModWheelDest, 1},
  {33, kParamModWheelAmount, 55}, {33, kParamVelToFilter, 40},
  {33, kParamIns1Algo, 1},    {33, kParamIns1A, 58},      {33, kParamIns1B, 44},
  {33, kParamIns1C, 36},      {33, kParamIns1Mix, 32},    {33, kParamIns2Algo, 3},
  {33, kParamIns2A, 28},      {33, kParamIns2B, 24},      {33, kParamIns2C, 60},
  {33, kParamIns2Mix, 16},    {33, kParamMstAlgo, 1},     {33, kParamMstA, 54},
  {33, kParamMstB, 40},       {33, kParamMstC, 30},       {33, kParamMstMix, 26},

  // Family 02 / 012 Stab Brass
  {34, kParamEngine, 1},      {34, kParamOsc1Shape, 64},  {34, kParamOsc2Shape, 58},
  {34, kParamOsc2Pitch, 7},   {34, kParamOsc2Level, 80},  {34, kParamUnisonVoices, 2},
  {34, kParamUnisonDetune, 18}, {34, kParamFilterCutoff, 1800}, {34, kParamFilterReso, 26},
  {34, kParamFilterEnv, 78},  {34, kParamFilterDrive, 20}, {34, kParamFiltAttack, 8},
  {34, kParamFiltDecay, 160}, {34, kParamFiltSustain, 20}, {34, kParamFiltRelease, 180},
  {34, kParamAmpAttack, 8},   {34, kParamAmpDecay, 220},  {34, kParamAmpSustain, 35},
  {34, kParamAmpRelease, 180}, {34, kParamVelToAmp, 65},   {34, kParamVelToFilter, 70},
  {34, kParamIns1Algo, 1},    {34, kParamIns1A, 44},      {34, kParamIns1B, 50},
  {34, kParamIns1C, 40},      {34, kParamIns1Mix, 26},    {34, kParamMstAlgo, 1},
  {34, kParamMstA, 50},       {34, kParamMstB, 46},       {34, kParamMstC, 22},
  {34, kParamMstMix, 30},

  // Family 02 / 013 Tight Strings
  {35, kParamEngine, 1},      {35, kParamOsc1Shape, 48},  {35, kParamOsc2Shape, 44},
  {35, kParamOsc2Pitch, 12},  {35, kParamOsc2Level, 60},  {35, kParamUnisonVoices, 3},
  {35, kParamUnisonDetune, 12}, {35, kParamFilterCutoff, 3600}, {35, kParamFilterReso, 10},
  {35, kParamFilterEnv, 20},  {35, kParamAmpAttack, 180}, {35, kParamAmpDecay, 900},
  {35, kParamAmpSustain, 70}, {35, kParamAmpRelease, 700}, {35, kParamVelToAmp, 25},
  {35, kParamIns1Algo, 2},    {35, kParamIns1A, 22},      {35, kParamIns1B, 20},
  {35, kParamIns1C, 42},      {35, kParamIns1Mix, 16},    {35, kParamMstAlgo, 1},
  {35, kParamMstA, 52},       {35, kParamMstB, 48},       {35, kParamMstC, 34},
  {35, kParamMstMix, 30},

  // Family 02 / 014 Breath Choir
  {36, kParamEngine, 1},      {36, kParamOsc1Shape, 24},  {36, kParamOsc2Shape, 50},
  {36, kParamOsc2Pitch, 12},  {36, kParamOsc2Level, 45},  {36, kParamNoiseLevel, 16},
  {36, kParamUnisonVoices, 4}, {36, kParamUnisonDetune, 28}, {36, kParamFilterType, 2},
  {36, kParamFilterCutoff, 1400}, {36, kParamFilterReso, 22}, {36, kParamAmpAttack, 500},
  {36, kParamAmpDecay, 1600}, {36, kParamAmpSustain, 75}, {36, kParamAmpRelease, 1800},
  {36, kParamLfoRate, 0.4},   {36, kParamLfoDest, 4},     {36, kParamLfoAmount, 18},
  {36, kParamSynthMasterLevel, -4},
  {36, kParamIns1Algo, 2},    {36, kParamIns1A, 18},      {36, kParamIns1B, 32},
  {36, kParamIns1C, 58},      {36, kParamIns1Mix, 24},    {36, kParamIns2Algo, 3},
  {36, kParamIns2A, 40},      {36, kParamIns2B, 24},      {36, kParamIns2C, 50},
  {36, kParamIns2Mix, 16},    {36, kParamMstAlgo, 3},     {36, kParamMstA, 60},
  {36, kParamMstB, 28},       {36, kParamMstC, 50},       {36, kParamMstMix, 24},

  // Family 02 / 015 Poly Pad
  {37, kParamEngine, 1},      {37, kParamOsc1Shape, 44},  {37, kParamOsc2Shape, 66},
  {37, kParamOsc2Pitch, 7},   {37, kParamOsc2Level, 70},  {37, kParamUnisonVoices, 3},
  {37, kParamUnisonDetune, 24}, {37, kParamFilterCutoff, 2400}, {37, kParamFilterReso, 18},
  {37, kParamFilterEnv, 35},  {37, kParamFilterDrive, 12}, {37, kParamAmpAttack, 350},
  {37, kParamAmpDecay, 2000}, {37, kParamAmpSustain, 68}, {37, kParamAmpRelease, 2200},
  {37, kParamLfoRate, 0.3},   {37, kParamLfoDest, 1},     {37, kParamLfoAmount, 22},
  {37, kParamIns1Algo, 2},    {37, kParamIns1A, 26},      {37, kParamIns1B, 34},
  {37, kParamIns1C, 56},      {37, kParamIns1Mix, 28},    {37, kParamIns2Algo, 3},
  {37, kParamIns2A, 40},      {37, kParamIns2B, 30},      {37, kParamIns2C, 54},
  {37, kParamIns2Mix, 18},    {37, kParamMstAlgo, 3},     {37, kParamMstA, 64},
  {37, kParamMstB, 26},       {37, kParamMstC, 50},       {37, kParamMstMix, 26},

  // Family 02 / 016 Sweep Motion
  {38, kParamEngine, 1},      {38, kParamOsc1Shape, 50},  {38, kParamOsc2Shape, 62},
  {38, kParamOsc2Pitch, -5},  {38, kParamOsc2Level, 65},  {38, kParamUnisonVoices, 4},
  {38, kParamUnisonDetune, 44}, {38, kParamFilterCutoff, 800}, {38, kParamFilterReso, 32},
  {38, kParamFilterEnv, 80},  {38, kParamFiltAttack, 1800}, {38, kParamFiltDecay, 2400},
  {38, kParamFiltSustain, 30}, {38, kParamFiltRelease, 2000}, {38, kParamAmpAttack, 200},
  {38, kParamAmpDecay, 2600}, {38, kParamAmpSustain, 60}, {38, kParamAmpRelease, 2400},
  {38, kParamLfoRate, 0.12},  {38, kParamLfoShape, 2},    {38, kParamLfoDest, 1},
  {38, kParamLfoAmount, 55},
  {38, kParamIns1Algo, 2},    {38, kParamIns1A, 10},      {38, kParamIns1B, 38},
  {38, kParamIns1C, 60},      {38, kParamIns1Mix, 22},    {38, kParamIns2Algo, 3},
  {38, kParamIns2A, 62},      {38, kParamIns2B, 48},      {38, kParamIns2C, 46},
  {38, kParamIns2Mix, 36},    {38, kParamMstAlgo, 3},     {38, kParamMstA, 66},
  {38, kParamMstB, 24},       {38, kParamMstC, 50},       {38, kParamMstMix, 32},

  // Family 02 / 017 Skew Texture
  {39, kParamEngine, 1},      {39, kParamOsc1Shape, 58},  {39, kParamOsc2Shape, 90},
  {39, kParamOsc2Pitch, 6},   {39, kParamOsc2Level, 55},  {39, kParamOscRing, 1},
  {39, kParamNoiseLevel, 20}, {39, kParamFilterType, 2},  {39, kParamFilterCutoff, 900},
  {39, kParamFilterReso, 48}, {39, kParamFilterDrive, 28}, {39, kParamUnisonVoices, 2},
  {39, kParamUnisonDetune, 50}, {39, kParamAmpAttack, 80},  {39, kParamAmpDecay, 1800},
  {39, kParamAmpSustain, 55}, {39, kParamAmpRelease, 1600}, {39, kParamLfoRate, 3.5},
  {39, kParamLfoShape, 4},    {39, kParamLfoDest, 1},     {39, kParamLfoAmount, 60},
  {39, kParamIns1Algo, 4},    {39, kParamIns1A, 55},      {39, kParamIns1B, 40},
  {39, kParamIns1C, 35},      {39, kParamIns1Mix, 45},    {39, kParamIns2Algo, 1},
  {39, kParamIns2A, 48},      {39, kParamIns2B, 38},      {39, kParamIns2C, 44},
  {39, kParamIns2Mix, 24},    {39, kParamMstAlgo, 4},     {39, kParamMstA, 30},
  {39, kParamMstB, 55},       {39, kParamMstC, 18},       {39, kParamMstMix, 40},

  // Family 02 / 018 Pulse Drone
  {40, kParamEngine, 1},      {40, kParamOsc1Shape, 70},  {40, kParamOsc1PW, 30},
  {40, kParamOsc2Shape, 66},  {40, kParamOsc2Pitch, -12}, {40, kParamOsc2Level, 50},
  {40, kParamSubLevel, 70},   {40, kParamFilterCutoff, 600}, {40, kParamFilterReso, 42},
  {40, kParamFilterEnv, 15},  {40, kParamFilterDrive, 25}, {40, kParamAmpAttack, 900},
  {40, kParamAmpDecay, 3000}, {40, kParamAmpSustain, 90}, {40, kParamAmpRelease, 3200},
  {40, kParamLfoRate, 0.2},   {40, kParamLfoShape, 3},    {40, kParamLfoDest, 4},
  {40, kParamLfoAmount, 30},  {40, kParamSynthMasterLevel, -6},
  {40, kParamIns1Algo, 1},    {40, kParamIns1A, 30},      {40, kParamIns1B, 28},
  {40, kParamIns1C, 50},      {40, kParamIns1Mix, 20},    {40, kParamIns2Algo, 3},
  {40, kParamIns2A, 58},      {40, kParamIns2B, 34},      {40, kParamIns2C, 34},
  {40, kParamIns2Mix, 16},    {40, kParamMstAlgo, 4},     {40, kParamMstA, 38},
  {40, kParamMstB, 30},       {40, kParamMstC, 12},       {40, kParamMstMix, 24},

  // Family 02 / 019 Wide Arp
  {41, kParamOsc1Shape, 66},  {41, kParamOsc2Shape, 66},  {41, kParamOsc2Pitch, 7},
  {41, kParamOsc2Level, 70},  {41, kParamUnisonVoices, 4}, {41, kParamUnisonDetune, 40},
  {41, kParamFilterCutoff, 7000}, {41, kParamAmpAttack, 1},   {41, kParamAmpDecay, 90},
  {41, kParamAmpSustain, 0},  {41, kParamAmpRelease, 140},
  {41, kParamIns1Algo, 3},    {41, kParamIns1A, 20},      {41, kParamIns1B, 22},
  {41, kParamIns1C, 66},      {41, kParamIns1Mix, 18},    {41, kParamMstAlgo, 3},
  {41, kParamMstA, 66},       {41, kParamMstB, 28},       {41, kParamMstC, 50},
  {41, kParamMstMix, 24},

  // Family 02 / 020 Heavy Crush
  {42, kParamEngine, 1},      {42, kParamOsc1Shape, 78},  {42, kParamOsc2Shape, 84},
  {42, kParamOsc2Pitch, 12},  {42, kParamOsc2Level, 70},  {42, kParamFilterCutoff, 3200},
  {42, kParamFilterReso, 24}, {42, kParamFilterDrive, 55}, {42, kParamAmpAttack, 5},
  {42, kParamAmpDecay, 600},  {42, kParamAmpSustain, 40}, {42, kParamAmpRelease, 400},
  {42, kParamOutLevel, -4},
  {42, kParamIns1Algo, 4},    {42, kParamIns1A, 72},      {42, kParamIns1B, 64},
  {42, kParamIns1C, 48},      {42, kParamIns1Mix, 80},    {42, kParamIns2Algo, 1},
  {42, kParamIns2A, 66},      {42, kParamIns2B, 30},      {42, kParamIns2C, 44},
  {42, kParamIns2Mix, 50},    {42, kParamMstAlgo, 2},     {42, kParamMstA, 28},
  {42, kParamMstB, 55},       {42, kParamMstC, 40},       {42, kParamMstMix, 70},

  // Family 02 / 021 Punch Kit
  {43, kParamEngine, 1},      {43, kParamOsc1Shape, 100}, {43, kParamOsc1PW, 15},
  {43, kParamOsc2Shape, 100}, {43, kParamOsc2Pitch, -12}, {43, kParamOsc2Level, 90},
  {43, kParamNoiseLevel, 25}, {43, kParamSubLevel, 40},   {43, kParamFilterCutoff, 11000},
  {43, kParamFilterReso, 6},  {43, kParamFilterEnv, 0},   {43, kParamFilterDrive, 48},
  {43, kParamAmpAttack, 0.5}, {43, kParamAmpDecay, 90},   {43, kParamAmpSustain, 0},
  {43, kParamAmpRelease, 70}, {43, kParamPitchAmount, -20}, {43, kParamPitchAttack, 0.5},
  {43, kParamPitchDecay, 50}, {43, kParamPitchRelease, 50}, {43, kParamTapeSat, 30},
  {43, kParamTapeMix, 12},    {43, kParamDrumSlotTune, 2}, {43, kParamDrumSlotDecay, 28},
  {43, kParamDrumSlotDrive, 70}, {43, kParamDrumSlotFilter, 15000}, {43, kParamDrumKitLevel, -2},
  {43, kParamIns1Algo, 1},    {43, kParamIns1A, 62},      {43, kParamIns1B, 45},
  {43, kParamIns1C, 40},      {43, kParamIns1Mix, 36},    {43, kParamIns2Algo, 4},
  {43, kParamIns2A, 66},      {43, kParamIns2B, 50},      {43, kParamIns2C, 30},
  {43, kParamIns2Mix, 20},    {43, kParamMstAlgo, 2},     {43, kParamMstA, 24},
  {43, kParamMstB, 46},       {43, kParamMstC, 18},       {43, kParamMstMix, 60},

  // Family 03 / 001 Piano Stack
  {44, kParamOsc1Shape, 12},  {44, kParamOsc1Level, 95},  {44, kParamOsc2Shape, 18},
  {44, kParamOsc2Pitch, 12},  {44, kParamOsc2Level, 48},  {44, kParamFmAmount, 6},
  {44, kParamFilterCutoff, 6400}, {44, kParamFilterReso, 4},  {44, kParamFilterEnv, 12},
  {44, kParamAmpAttack, 6},   {44, kParamAmpDecay, 1100}, {44, kParamAmpSustain, 38},
  {44, kParamAmpRelease, 900}, {44, kParamVelToAmp, 35},   {44, kParamTapeSat, 18},
  {44, kParamTapeMix, 28},
  {44, kParamIns1Algo, 2},    {44, kParamIns1A, 10},      {44, kParamIns1B, 16},
  {44, kParamIns1C, 38},      {44, kParamIns1Mix, 12},    {44, kParamIns2Algo, 3},
  {44, kParamIns2A, 30},      {44, kParamIns2B, 18},      {44, kParamIns2C, 50},
  {44, kParamIns2Mix, 12},    {44, kParamMstAlgo, 4},     {44, kParamMstA, 56},
  {44, kParamMstB, 44},       {44, kParamMstC, 10},       {44, kParamMstMix, 16},

  // Family 03 / 002 Soft EP
  {45, kParamOsc1Shape, 25},  {45, kParamOsc2Shape, 33},  {45, kParamOsc2Pitch, 12},
  {45, kParamOsc2Level, 45},  {45, kParamFmAmount, 18},   {45, kParamFilterCutoff, 4200},
  {45, kParamFilterReso, 12}, {45, kParamAmpAttack, 6},   {45, kParamAmpDecay, 1400},
  {45, kParamAmpSustain, 45}, {45, kParamAmpRelease, 800}, {45, kParamTapeMix, 35},
  {45, kParamIns1Algo, 2},    {45, kParamIns1A, 16},      {45, kParamIns1B, 24},
  {45, kParamIns1C, 46},      {45, kParamIns1Mix, 18},    {45, kParamMstAlgo, 4},
  {45, kParamMstA, 58},       {45, kParamMstB, 40},       {45, kParamMstC, 12},
  {45, kParamMstMix, 18},

  // Family 03 / 003 Soul Piano
  {46, kParamOsc1Shape, 20},  {46, kParamOsc2Shape, 36},  {46, kParamOsc2Pitch, 12},
  {46, kParamOsc2Level, 52},  {46, kParamFmAmount, 22},   {46, kParamFilterCutoff, 3400},
  {46, kParamFilterReso, 14}, {46, kParamFilterEnv, 22},  {46, kParamAmpAttack, 8},
  {46, kParamAmpDecay, 1600}, {46, kParamAmpSustain, 50}, {46, kParamAmpRelease, 1100},
  {46, kParamVelToAmp, 58},   {46, kParamVelToFilter, 30}, {46, kParamTapeSat, 30},
  {46, kParamTapeMix, 38},    {46, kParamTapeHiss, 6},
  {46, kParamIns1Algo, 2},    {46, kParamIns1A, 14},      {46, kParamIns1B, 28},
  {46, kParamIns1C, 42},      {46, kParamIns1Mix, 26},    {46, kParamIns2Algo, 3},
  {46, kParamIns2A, 26},      {46, kParamIns2B, 20},      {46, kParamIns2C, 46},
  {46, kParamIns2Mix, 14},    {46, kParamMstAlgo, 4},     {46, kParamMstA, 58},
  {46, kParamMstB, 45},       {46, kParamMstC, 14},       {46, kParamMstMix, 22},

  // Family 03 / 004 Clav Stab
  {47, kParamOsc1Shape, 84},  {47, kParamOsc1PW, 24},     {47, kParamOsc2Shape, 92},
  {47, kParamOsc2Pitch, 12},  {47, kParamOsc2Level, 62},  {47, kParamOsc2PW, 18},
  {47, kParamFilterCutoff, 2800}, {47, kParamFilterReso, 45}, {47, kParamFilterEnv, 80},
  {47, kParamFilterDrive, 26}, {47, kParamFilterKeyTrack, 40}, {47, kParamFiltAttack, 1},
  {47, kParamFiltDecay, 100}, {47, kParamFiltSustain, 0}, {47, kParamFiltRelease, 90},
  {47, kParamAmpAttack, 1},   {47, kParamAmpDecay, 140},  {47, kParamAmpSustain, 0},
  {47, kParamAmpRelease, 110}, {47, kParamVelToAmp, 78},   {47, kParamVelToFilter, 60},
  {47, kParamTapeSat, 22},    {47, kParamTapeMix, 16},
  {47, kParamIns1Algo, 1},    {47, kParamIns1A, 38},      {47, kParamIns1B, 50},
  {47, kParamIns1C, 44},      {47, kParamIns1Mix, 24},    {47, kParamMstAlgo, 1},
  {47, kParamMstA, 54},       {47, kParamMstB, 44},       {47, kParamMstC, 24},
  {47, kParamMstMix, 26},

  // Family 03 / 005 Dry Clav
  {48, kParamOsc1Shape, 78},  {48, kParamOsc1PW, 30},     {48, kParamOsc2Shape, 90},
  {48, kParamOsc2Pitch, 12},  {48, kParamOsc2Level, 55},  {48, kParamFilterCutoff, 3000},
  {48, kParamFilterReso, 28}, {48, kParamFilterEnv, 60},  {48, kParamAmpAttack, 1},
  {48, kParamAmpDecay, 260},  {48, kParamAmpSustain, 0},  {48, kParamAmpRelease, 160},
  {48, kParamTapeSat, 30},    {48, kParamTapeMix, 20},
  {48, kParamIns1Algo, 1},    {48, kParamIns1A, 28},      {48, kParamIns1B, 46},
  {48, kParamIns1C, 48},      {48, kParamIns1Mix, 18},    {48, kParamMstAlgo, 1},
  {48, kParamMstA, 58},       {48, kParamMstB, 36},       {48, kParamMstC, 22},
  {48, kParamMstMix, 20},

  // Family 03 / 006 Warm Organ
  {49, kParamOsc1Shape, 33},  {49, kParamOsc1Level, 90},  {49, kParamOsc2Shape, 33},
  {49, kParamOsc2Pitch, 12},  {49, kParamOsc2Level, 90},  {49, kParamSubLevel, 40},
  {49, kParamFilterCutoff, 8000}, {49, kParamFilterReso, 6},  {49, kParamAmpAttack, 3},
  {49, kParamAmpDecay, 60},   {49, kParamAmpSustain, 100}, {49, kParamAmpRelease, 90},
  {49, kParamIns1Algo, 2},    {49, kParamIns1A, 14},      {49, kParamIns1B, 26},
  {49, kParamIns1C, 48},      {49, kParamIns1Mix, 20},    {49, kParamMstAlgo, 1},
  {49, kParamMstA, 56},       {49, kParamMstB, 32},       {49, kParamMstC, 38},
  {49, kParamMstMix, 22},

  // Family 03 / 007 Bottle Bells
  {50, kParamOsc1Shape, 4},   {50, kParamOsc2Shape, 8},   {50, kParamOsc2Pitch, 12},
  {50, kParamOsc2Level, 40},  {50, kParamFmAmount, 14},   {50, kParamFilterCutoff, 7000},
  {50, kParamFilterReso, 4},  {50, kParamFilterKeyTrack, 35}, {50, kParamAmpAttack, 3},
  {50, kParamAmpDecay, 1800}, {50, kParamAmpSustain, 10}, {50, kParamAmpRelease, 1600},
  {50, kParamTapeMix, 24},
  {50, kParamIns1Algo, 3},    {50, kParamIns1A, 30},      {50, kParamIns1B, 18},
  {50, kParamIns1C, 75},      {50, kParamIns1Mix, 14},    {50, kParamIns2Algo, 2},
  {50, kParamIns2A, 12},      {50, kParamIns2B, 22},      {50, kParamIns2C, 62},
  {50, kParamIns2Mix, 14},    {50, kParamMstAlgo, 3},     {50, kParamMstA, 62},
  {50, kParamMstB, 28},       {50, kParamMstC, 50},       {50, kParamMstMix, 22},

  // Family 03 / 008 Muted Pluck
  {51, kParamOsc1Shape, 46},  {51, kParamOsc1PW, 35},     {51, kParamOsc2Shape, 30},
  {51, kParamOsc2Pitch, 12},  {51, kParamOsc2Level, 35},  {51, kParamFilterCutoff, 2200},
  {51, kParamFilterReso, 16}, {51, kParamFilterEnv, 40},  {51, kParamFilterKeyTrack, 50},
  {51, kParamFiltAttack, 2},  {51, kParamFiltDecay, 200}, {51, kParamFiltSustain, 0},
  {51, kParamFiltRelease, 180}, {51, kParamAmpAttack, 2},   {51, kParamAmpDecay, 280},
  {51, kParamAmpSustain, 0},  {51, kParamAmpRelease, 200}, {51, kParamTapeSat, 20},
  {51, kParamTapeMix, 22},
  {51, kParamIns1Algo, 3},    {51, kParamIns1A, 12},      {51, kParamIns1B, 12},
  {51, kParamIns1C, 52},      {51, kParamIns1Mix, 10},    {51, kParamMstAlgo, 1},
  {51, kParamMstA, 56},       {51, kParamMstB, 30},       {51, kParamMstC, 24},
  {51, kParamMstMix, 18},

  // Family 03 / 009 Reed Stab
  {52, kParamOsc1Shape, 38},  {52, kParamOsc2Shape, 42},  {52, kParamOsc2Pitch, 7},
  {52, kParamOsc2Level, 55},  {52, kParamFilterCutoff, 2600}, {52, kParamFilterReso, 24},
  {52, kParamFilterEnv, 55},  {52, kParamFiltAttack, 4},  {52, kParamFiltDecay, 150},
  {52, kParamFiltSustain, 0}, {52, kParamFiltRelease, 140}, {52, kParamAmpAttack, 4},
  {52, kParamAmpDecay, 200},  {52, kParamAmpSustain, 0},  {52, kParamAmpRelease, 160},
  {52, kParamVelToAmp, 50},   {52, kParamVelToFilter, 40},
  {52, kParamIns1Algo, 2},    {52, kParamIns1A, 16},      {52, kParamIns1B, 22},
  {52, kParamIns1C, 44},      {52, kParamIns1Mix, 18},    {52, kParamIns2Algo, 3},
  {52, kParamIns2A, 20},      {52, kParamIns2B, 16},      {52, kParamIns2C, 48},
  {52, kParamIns2Mix, 12},    {52, kParamMstAlgo, 1},     {52, kParamMstA, 52},
  {52, kParamMstB, 44},       {52, kParamMstC, 22},       {52, kParamMstMix, 26},

  // Family 03 / 010 Finger Bass
  {53, kParamPolyMode, 1},    {53, kParamMonoLegato, 1},  {53, kParamOsc1Shape, 28},
  {53, kParamOsc1Pitch, -12}, {53, kParamOsc2Shape, 22},  {53, kParamOsc2Pitch, -12},
  {53, kParamOsc2Level, 40},  {53, kParamSubLevel, 55},   {53, kParamFilterCutoff, 1100},
  {53, kParamFilterReso, 14}, {53, kParamFilterEnv, 35},  {53, kParamAmpAttack, 4},
  {53, kParamAmpDecay, 400},  {53, kParamAmpSustain, 72}, {53, kParamAmpRelease, 220},
  {53, kParamVelToAmp, 40},   {53, kParamTapeSat, 24},    {53, kParamTapeMix, 30},
  {53, kParamIns1Algo, 1},    {53, kParamIns1A, 26},      {53, kParamIns1B, 38},
  {53, kParamIns1C, 46},      {53, kParamIns1Mix, 20},    {53, kParamMstAlgo, 1},
  {53, kParamMstA, 60},       {53, kParamMstB, 40},       {53, kParamMstC, 22},
  {53, kParamMstMix, 24},

  // Family 03 / 011 Solo Lead
  {54, kParamPolyMode, 1},    {54, kParamPorta, 60},      {54, kParamOsc1Shape, 34},
  {54, kParamOsc2Shape, 40},  {54, kParamOsc2Pitch, 12},  {54, kParamOsc2Level, 40},
  {54, kParamFmAmount, 10},   {54, kParamFilterCutoff, 4500}, {54, kParamFilterReso, 12},
  {54, kParamFilterEnv, 28},  {54, kParamFilterKeyTrack, 30}, {54, kParamAmpAttack, 18},
  {54, kParamAmpDecay, 500},  {54, kParamAmpSustain, 76}, {54, kParamAmpRelease, 320},
  {54, kParamModWheelDest, 2}, {54, kParamModWheelAmount, 18}, {54, kParamVelToAmp, 28},
  {54, kParamIns1Algo, 3},    {54, kParamIns1A, 28},      {54, kParamIns1B, 22},
  {54, kParamIns1C, 58},      {54, kParamIns1Mix, 16},    {54, kParamIns2Algo, 2},
  {54, kParamIns2A, 14},      {54, kParamIns2B, 18},      {54, kParamIns2C, 54},
  {54, kParamIns2Mix, 12},    {54, kParamMstAlgo, 1},     {54, kParamMstA, 56},
  {54, kParamMstB, 36},       {54, kParamMstC, 32},       {54, kParamMstMix, 22},

  // Family 03 / 012 Ensemble Brass
  {55, kParamOsc1Shape, 46},  {55, kParamOsc2Shape, 50},  {55, kParamOsc2Pitch, 7},
  {55, kParamOsc2Level, 75},  {55, kParamUnisonVoices, 3}, {55, kParamUnisonDetune, 14},
  {55, kParamFilterCutoff, 2000}, {55, kParamFilterReso, 10}, {55, kParamFilterEnv, 48},
  {55, kParamFiltAttack, 40}, {55, kParamFiltDecay, 400}, {55, kParamFiltSustain, 60},
  {55, kParamFiltRelease, 500}, {55, kParamAmpAttack, 45},  {55, kParamAmpDecay, 700},
  {55, kParamAmpSustain, 80}, {55, kParamAmpRelease, 400}, {55, kParamVelToFilter, 45},
  {55, kParamTapeSat, 20},    {55, kParamTapeMix, 32},
  {55, kParamIns1Algo, 2},    {55, kParamIns1A, 12},      {55, kParamIns1B, 24},
  {55, kParamIns1C, 52},      {55, kParamIns1Mix, 20},    {55, kParamMstAlgo, 1},
  {55, kParamMstA, 56},       {55, kParamMstB, 42},       {55, kParamMstC, 42},
  {55, kParamMstMix, 30},

  // Family 03 / 013 Section Strings
  {56, kParamOsc1Shape, 36},  {56, kParamOsc2Shape, 32},  {56, kParamOsc2Pitch, 12},
  {56, kParamOsc2Level, 80},  {56, kParamUnisonVoices, 4}, {56, kParamUnisonDetune, 18},
  {56, kParamFilterCutoff, 2800}, {56, kParamFilterReso, 6},  {56, kParamFilterEnv, 18},
  {56, kParamAmpAttack, 700}, {56, kParamAmpDecay, 2000}, {56, kParamAmpSustain, 85},
  {56, kParamAmpRelease, 2000}, {56, kParamFiltAttack, 800}, {56, kParamFiltDecay, 1800},
  {56, kParamFiltSustain, 70}, {56, kParamFiltRelease, 1800}, {56, kParamTapeMix, 36},
  {56, kParamSynthMasterLevel, -2},
  {56, kParamIns1Algo, 2},    {56, kParamIns1A, 8},       {56, kParamIns1B, 30},
  {56, kParamIns1C, 50},      {56, kParamIns1Mix, 24},    {56, kParamIns2Algo, 3},
  {56, kParamIns2A, 44},      {56, kParamIns2B, 24},      {56, kParamIns2C, 46},
  {56, kParamIns2Mix, 16},    {56, kParamMstAlgo, 3},     {56, kParamMstA, 60},
  {56, kParamMstB, 30},       {56, kParamMstC, 50},       {56, kParamMstMix, 24},

  // Family 03 / 014 Ooh Choir
  {57, kParamOsc1Shape, 18},  {57, kParamOsc2Shape, 22},  {57, kParamOsc2Pitch, 12},
  {57, kParamOsc2Level, 70},  {57, kParamNoiseLevel, 6},  {57, kParamUnisonVoices, 5},
  {57, kParamUnisonDetune, 16}, {57, kParamFilterCutoff, 3600}, {57, kParamFilterReso, 8},
  {57, kParamAmpAttack, 800}, {57, kParamAmpDecay, 2200}, {57, kParamAmpSustain, 88},
  {57, kParamAmpRelease, 2400}, {57, kParamLfoRate, 0.15},  {57, kParamLfoDest, 3},
  {57, kParamLfoAmount, 8},   {57, kParamTapeHiss, 8},    {57, kParamTapeMix, 30},
  {57, kParamIns1Algo, 2},    {57, kParamIns1A, 10},      {57, kParamIns1B, 26},
  {57, kParamIns1C, 56},      {57, kParamIns1Mix, 22},    {57, kParamIns2Algo, 3},
  {57, kParamIns2A, 48},      {57, kParamIns2B, 20},      {57, kParamIns2C, 44},
  {57, kParamIns2Mix, 16},    {57, kParamMstAlgo, 3},     {57, kParamMstA, 62},
  {57, kParamMstB, 28},       {57, kParamMstC, 50},       {57, kParamMstMix, 22},

  // Family 03 / 015 Dream Pad
  {58, kParamOsc1Shape, 14},  {58, kParamOsc2Shape, 26},  {58, kParamOsc2Pitch, 7},
  {58, kParamOsc2Level, 55},  {58, kParamUnisonVoices, 3}, {58, kParamUnisonDetune, 20},
  {58, kParamFilterCutoff, 3000}, {58, kParamFilterReso, 12}, {58, kParamFilterEnv, 20},
  {58, kParamAmpAttack, 900}, {58, kParamAmpDecay, 2400}, {58, kParamAmpSustain, 70},
  {58, kParamAmpRelease, 2800}, {58, kParamLfoRate, 0.1},   {58, kParamLfoDest, 1},
  {58, kParamLfoAmount, 16},  {58, kParamSynthMasterLevel, -3},
  {58, kParamIns1Algo, 2},    {58, kParamIns1A, 12},      {58, kParamIns1B, 34},
  {58, kParamIns1C, 58},      {58, kParamIns1Mix, 28},    {58, kParamIns2Algo, 3},
  {58, kParamIns2A, 48},      {58, kParamIns2B, 26},      {58, kParamIns2C, 52},
  {58, kParamIns2Mix, 16},    {58, kParamMstAlgo, 3},     {58, kParamMstA, 64},
  {58, kParamMstB, 26},       {58, kParamMstC, 50},       {58, kParamMstMix, 26},

  // Family 03 / 016 Slow Sweep
  {59, kParamOsc1Shape, 10},  {59, kParamOsc2Shape, 48},  {59, kParamOsc2Pitch, 5},
  {59, kParamOsc2Level, 45},  {59, kParamFilterCutoff, 1400}, {59, kParamFilterReso, 18},
  {59, kParamFilterEnv, 40},  {59, kParamFiltAttack, 2000}, {59, kParamFiltDecay, 3000},
  {59, kParamFiltSustain, 45}, {59, kParamFiltRelease, 2500}, {59, kParamAmpAttack, 1400},
  {59, kParamAmpDecay, 3200}, {59, kParamAmpSustain, 75}, {59, kParamAmpRelease, 3000},
  {59, kParamLfoRate, 0.07},  {59, kParamLfoShape, 1},    {59, kParamLfoDest, 1},
  {59, kParamLfoAmount, 28},
  {59, kParamIns1Algo, 2},    {59, kParamIns1A, 8},       {59, kParamIns1B, 40},
  {59, kParamIns1C, 54},      {59, kParamIns1Mix, 22},    {59, kParamIns2Algo, 3},
  {59, kParamIns2A, 56},      {59, kParamIns2B, 34},      {59, kParamIns2C, 44},
  {59, kParamIns2Mix, 20},    {59, kParamMstAlgo, 4},     {59, kParamMstA, 46},
  {59, kParamMstB, 32},       {59, kParamMstC, 12},       {59, kParamMstMix, 22},

  // Family 03 / 017 Hollow Texture
  {60, kParamOsc1Shape, 22},  {60, kParamOsc2Shape, 55},  {60, kParamOsc2Pitch, 7},
  {60, kParamOsc2Level, 35},  {60, kParamNoiseLevel, 12}, {60, kParamFilterType, 2},
  {60, kParamFilterCutoff, 1600}, {60, kParamFilterReso, 30}, {60, kParamUnisonVoices, 2},
  {60, kParamUnisonDetune, 30}, {60, kParamAmpAttack, 600}, {60, kParamAmpDecay, 2400},
  {60, kParamAmpSustain, 60}, {60, kParamAmpRelease, 2200}, {60, kParamLfoRate, 0.55},
  {60, kParamLfoShape, 4},    {60, kParamLfoDest, 1},     {60, kParamLfoAmount, 24},
  {60, kParamIns1Algo, 4},    {60, kParamIns1A, 20},      {60, kParamIns1B, 18},
  {60, kParamIns1C, 26},      {60, kParamIns1Mix, 12},    {60, kParamIns2Algo, 3},
  {60, kParamIns2A, 36},      {60, kParamIns2B, 28},      {60, kParamIns2C, 42},
  {60, kParamIns2Mix, 16},    {60, kParamMstAlgo, 4},     {60, kParamMstA, 40},
  {60, kParamMstB, 40},       {60, kParamMstC, 14},       {60, kParamMstMix, 18},

  // Family 03 / 018 Warm Drone
  {61, kParamOsc1Shape, 6},   {61, kParamOsc2Shape, 14},  {61, kParamOsc2Pitch, -12},
  {61, kParamOsc2Level, 60},  {61, kParamSubLevel, 45},   {61, kParamFilterCutoff, 700},
  {61, kParamFilterReso, 22}, {61, kParamFilterEnv, 8},   {61, kParamAmpAttack, 2200},
  {61, kParamAmpDecay, 4000}, {61, kParamAmpSustain, 92}, {61, kParamAmpRelease, 3800},
  {61, kParamLfoRate, 0.05},  {61, kParamLfoDest, 1},     {61, kParamLfoAmount, 14},
  {61, kParamTapeSat, 28},    {61, kParamTapeMix, 40},    {61, kParamSynthMasterLevel, -5},
  {61, kParamIns1Algo, 2},    {61, kParamIns1A, 6},       {61, kParamIns1B, 28},
  {61, kParamIns1C, 42},      {61, kParamIns1Mix, 18},    {61, kParamIns2Algo, 3},
  {61, kParamIns2A, 60},      {61, kParamIns2B, 32},      {61, kParamIns2C, 28},
  {61, kParamIns2Mix, 14},    {61, kParamMstAlgo, 4},     {61, kParamMstA, 38},
  {61, kParamMstB, 26},       {61, kParamMstC, 10},       {61, kParamMstMix, 20},

  // Family 03 / 019 Mallet Arp
  {62, kParamOsc1Shape, 8},   {62, kParamOsc2Shape, 16},  {62, kParamOsc2Pitch, 12},
  {62, kParamOsc2Level, 50},  {62, kParamFmAmount, 8},    {62, kParamFilterCutoff, 5500},
  {62, kParamFilterReso, 10}, {62, kParamFilterEnv, 35},  {62, kParamFilterKeyTrack, 40},
  {62, kParamAmpAttack, 2},   {62, kParamAmpDecay, 160},  {62, kParamAmpSustain, 0},
  {62, kParamAmpRelease, 180}, {62, kParamFiltAttack, 1},  {62, kParamFiltDecay, 120},
  {62, kParamFiltSustain, 0}, {62, kParamFiltRelease, 140},
  {62, kParamIns1Algo, 3},    {62, kParamIns1A, 20},      {62, kParamIns1B, 12},
  {62, kParamIns1C, 60},      {62, kParamIns1Mix, 10},    {62, kParamMstAlgo, 1},
  {62, kParamMstA, 54},       {62, kParamMstB, 28},       {62, kParamMstC, 24},
  {62, kParamMstMix, 18},

  // Family 03 / 020 Slap Delay
  {63, kParamOsc1Shape, 70},  {63, kParamOsc1PW, 40},     {63, kParamOsc2Shape, 60},
  {63, kParamOsc2Pitch, 12},  {63, kParamOsc2Level, 40},  {63, kParamFilterCutoff, 3600},
  {63, kParamFilterReso, 20}, {63, kParamFilterEnv, 45},  {63, kParamAmpAttack, 1},
  {63, kParamAmpDecay, 180},  {63, kParamAmpSustain, 0},  {63, kParamAmpRelease, 140},
  {63, kParamTapeSat, 26},
  {63, kParamIns1Algo, 3},    {63, kParamIns1A, 18},      {63, kParamIns1B, 42},
  {63, kParamIns1C, 60},      {63, kParamIns1Mix, 48},    {63, kParamIns2Algo, 4},
  {63, kParamIns2A, 52},      {63, kParamIns2B, 44},      {63, kParamIns2C, 34},
  {63, kParamIns2Mix, 26},    {63, kParamMstAlgo, 3},     {63, kParamMstA, 64},
  {63, kParamMstB, 30},       {63, kParamMstC, 50},       {63, kParamMstMix, 30},

  // Family 03 / 021 Dry Perc
  {64, kParamEngine, 1},      {64, kParamOsc1Shape, 88},  {64, kParamOsc1PW, 45},
  {64, kParamOsc2Shape, 70},  {64, kParamOsc2Level, 40},  {64, kParamNoiseLevel, 22},
  {64, kParamFilterCutoff, 8000}, {64, kParamFilterReso, 8},  {64, kParamFilterEnv, 15},
  {64, kParamAmpAttack, 0.5}, {64, kParamAmpDecay, 100},  {64, kParamAmpSustain, 0},
  {64, kParamAmpRelease, 80}, {64, kParamPitchAmount, -8}, {64, kParamPitchDecay, 60},
  {64, kParamPitchRelease, 60}, {64, kParamTapeSat, 10},    {64, kParamTapeMix, 10},
  {64, kParamDrumSlotDecay, 36}, {64, kParamDrumSlotDrive, 18}, {64, kParamDrumSlotFilter, 11000},
  {64, kParamDrumKitLevel, -3}, {64, kParamFilterDrive, 12},
  {64, kParamIns1Algo, 1},    {64, kParamIns1A, 30},      {64, kParamIns1B, 52},
  {64, kParamIns1C, 44},      {64, kParamIns1Mix, 22},    {64, kParamIns2Algo, 4},
  {64, kParamIns2A, 58},      {64, kParamIns2B, 46},      {64, kParamIns2C, 28},
  {64, kParamIns2Mix, 16},    {64, kParamMstAlgo, 2},     {64, kParamMstA, 34},
  {64, kParamMstB, 42},       {64, kParamMstC, 16},       {64, kParamMstMix, 46},

  // Family 04 / 001 Vector Keys
  {65, kParamOsc1Shape, 28},  {65, kParamOsc2Shape, 64},  {65, kParamOsc2Pitch, 7},
  {65, kParamOsc2Level, 58},  {65, kParamUnisonVoices, 3}, {65, kParamUnisonDetune, 32},
  {65, kParamFilterCutoff, 5000}, {65, kParamFilterReso, 14}, {65, kParamFilterEnv, 22},
  {65, kParamAmpAttack, 20},  {65, kParamAmpDecay, 900},  {65, kParamAmpSustain, 48},
  {65, kParamAmpRelease, 1100}, {65, kParamLfoRate, 0.18},  {65, kParamLfoDest, 1},
  {65, kParamLfoAmount, 18},  {65, kParamVelToAmp, 30},
  {65, kParamIns1Algo, 2},    {65, kParamIns1A, 16},      {65, kParamIns1B, 48},
  {65, kParamIns1C, 62},      {65, kParamIns1Mix, 26},    {65, kParamIns2Algo, 3},
  {65, kParamIns2A, 34},      {65, kParamIns2B, 22},      {65, kParamIns2C, 54},
  {65, kParamIns2Mix, 14},    {65, kParamMstAlgo, 3},     {65, kParamMstA, 62},
  {65, kParamMstB, 30},       {65, kParamMstC, 50},       {65, kParamMstMix, 24},

  // Family 04 / 002 Phase Piano
  {66, kParamOsc1Shape, 18},  {66, kParamOsc2Shape, 44},  {66, kParamOsc2Pitch, 12},
  {66, kParamOsc2Level, 36},  {66, kParamFmAmount, 12},   {66, kParamFilterCutoff, 3900},
  {66, kParamFilterReso, 16}, {66, kParamAmpAttack, 10},  {66, kParamAmpDecay, 1300},
  {66, kParamAmpSustain, 36}, {66, kParamAmpRelease, 900}, {66, kParamLfoRate, 0.45},
  {66, kParamLfoDest, 3},     {66, kParamLfoAmount, 22},  {66, kParamVelToAmp, 42},
  {66, kParamIns1Algo, 2},    {66, kParamIns1A, 28},      {66, kParamIns1B, 55},
  {66, kParamIns1C, 66},      {66, kParamIns1Mix, 34},    {66, kParamIns2Algo, 3},
  {66, kParamIns2A, 28},      {66, kParamIns2B, 18},      {66, kParamIns2C, 50},
  {66, kParamIns2Mix, 12},    {66, kParamMstAlgo, 3},     {66, kParamMstA, 60},
  {66, kParamMstB, 32},       {66, kParamMstC, 50},       {66, kParamMstMix, 22},

  // Family 04 / 003 Wave Clav
  {67, kParamOsc1Shape, 72},  {67, kParamOsc1PW, 42},     {67, kParamOsc2Shape, 80},
  {67, kParamOsc2Pitch, 12},  {67, kParamOsc2Level, 48},  {67, kParamFilterCutoff, 2600},
  {67, kParamFilterReso, 26}, {67, kParamFilterEnv, 50},  {67, kParamFilterKeyTrack, 35},
  {67, kParamAmpAttack, 2},   {67, kParamAmpDecay, 220},  {67, kParamAmpSustain, 0},
  {67, kParamAmpRelease, 180}, {67, kParamLfoRate, 0.7},   {67, kParamLfoDest, 1},
  {67, kParamLfoAmount, 16},  {67, kParamVelToAmp, 48},
  {67, kParamIns1Algo, 2},    {67, kParamIns1A, 20},      {67, kParamIns1B, 36},
  {67, kParamIns1C, 48},      {67, kParamIns1Mix, 20},    {67, kParamIns2Algo, 1},
  {67, kParamIns2A, 34},      {67, kParamIns2B, 48},      {67, kParamIns2C, 42},
  {67, kParamIns2Mix, 18},    {67, kParamMstAlgo, 1},     {67, kParamMstA, 54},
  {67, kParamMstB, 42},       {67, kParamMstC, 24},       {67, kParamMstMix, 24},

  // Family 04 / 004 Drift Organ
  {68, kParamOsc1Shape, 33},  {68, kParamOsc1Level, 80},  {68, kParamOsc2Shape, 40},
  {68, kParamOsc2Pitch, 7},   {68, kParamOsc2Level, 70},  {68, kParamSubLevel, 30},
  {68, kParamUnisonVoices, 2}, {68, kParamUnisonDetune, 18}, {68, kParamFilterCutoff, 5400},
  {68, kParamFilterReso, 12}, {68, kParamAmpAttack, 8},   {68, kParamAmpSustain, 100},
  {68, kParamAmpRelease, 200}, {68, kParamLfoRate, 0.25},  {68, kParamLfoDest, 1},
  {68, kParamLfoAmount, 20},
  {68, kParamIns1Algo, 2},    {68, kParamIns1A, 14},      {68, kParamIns1B, 60},
  {68, kParamIns1C, 70},      {68, kParamIns1Mix, 32},    {68, kParamIns2Algo, 3},
  {68, kParamIns2A, 24},      {68, kParamIns2B, 16},      {68, kParamIns2C, 46},
  {68, kParamIns2Mix, 12},    {68, kParamMstAlgo, 3},     {68, kParamMstA, 62},
  {68, kParamMstB, 28},       {68, kParamMstC, 50},       {68, kParamMstMix, 24},

  // Family 04 / 005 Chime Bells
  {69, kParamOsc1Shape, 8},   {69, kParamOsc2Shape, 20},  {69, kParamOsc2Pitch, 19},
  {69, kParamOsc2Level, 55},  {69, kParamFmAmount, 18},   {69, kParamFilterCutoff, 8600},
  {69, kParamFilterKeyTrack, 45}, {69, kParamAmpAttack, 4},   {69, kParamAmpDecay, 2600},
  {69, kParamAmpSustain, 15}, {69, kParamAmpRelease, 2800}, {69, kParamLfoRate, 0.12},
  {69, kParamLfoDest, 2},     {69, kParamLfoAmount, 8},   {69, kParamSynthMasterLevel, -4},
  {69, kParamIns1Algo, 3},    {69, kParamIns1A, 36},      {69, kParamIns1B, 28},
  {69, kParamIns1C, 72},      {69, kParamIns1Mix, 22},    {69, kParamIns2Algo, 2},
  {69, kParamIns2A, 12},      {69, kParamIns2B, 30},      {69, kParamIns2C, 74},
  {69, kParamIns2Mix, 20},    {69, kParamMstAlgo, 3},     {69, kParamMstA, 66},
  {69, kParamMstB, 26},       {69, kParamMstC, 50},       {69, kParamMstMix, 26},

  // Family 04 / 006 Harp Pluck
  {70, kParamOsc1Shape, 12},  {70, kParamOsc2Shape, 18},  {70, kParamOsc2Pitch, 12},
  {70, kParamOsc2Level, 42},  {70, kParamFmAmount, 6},    {70, kParamFilterCutoff, 6400},
  {70, kParamFilterReso, 8},  {70, kParamFilterEnv, 30},  {70, kParamFilterKeyTrack, 65},
  {70, kParamAmpAttack, 1},   {70, kParamAmpDecay, 500},  {70, kParamAmpSustain, 0},
  {70, kParamAmpRelease, 700},
  {70, kParamIns1Algo, 2},    {70, kParamIns1A, 18},      {70, kParamIns1B, 24},
  {70, kParamIns1C, 58},      {70, kParamIns1Mix, 18},    {70, kParamIns2Algo, 3},
  {70, kParamIns2A, 26},      {70, kParamIns2B, 16},      {70, kParamIns2C, 62},
  {70, kParamIns2Mix, 14},    {70, kParamMstAlgo, 3},     {70, kParamMstA, 60},
  {70, kParamMstB, 30},       {70, kParamMstC, 50},       {70, kParamMstMix, 22},

  // Family 04 / 007 Soft Stab
  {71, kParamOsc1Shape, 30},  {71, kParamOsc2Shape, 48},  {71, kParamOsc2Pitch, 7},
  {71, kParamOsc2Level, 50},  {71, kParamFilterCutoff, 4200}, {71, kParamFilterReso, 14},
  {71, kParamFilterEnv, 40},  {71, kParamAmpAttack, 6},   {71, kParamAmpDecay, 240},
  {71, kParamAmpSustain, 0},  {71, kParamAmpRelease, 280}, {71, kParamUnisonVoices, 2},
  {71, kParamUnisonDetune, 16}, {71, kParamVelToAmp, 40},
  {71, kParamIns1Algo, 2},    {71, kParamIns1A, 12},      {71, kParamIns1B, 30},
  {71, kParamIns1C, 52},      {71, kParamIns1Mix, 16},    {71, kParamIns2Algo, 3},
  {71, kParamIns2A, 18},      {71, kParamIns2B, 14},      {71, kParamIns2C, 50},
  {71, kParamIns2Mix, 12},    {71, kParamMstAlgo, 1},     {71, kParamMstA, 52},
  {71, kParamMstB, 40},       {71, kParamMstC, 22},       {71, kParamMstMix, 24},

  // Family 04 / 008 Sub Bass
  {72, kParamPolyMode, 1},    {72, kParamOsc1Shape, 8},   {72, kParamOsc1Pitch, -12},
  {72, kParamOsc2Shape, 14},  {72, kParamOsc2Pitch, -12}, {72, kParamOsc2Level, 30},
  {72, kParamSubLevel, 90},   {72, kParamFilterCutoff, 480}, {72, kParamFilterReso, 18},
  {72, kParamFilterEnv, 25},  {72, kParamAmpAttack, 8},   {72, kParamAmpDecay, 500},
  {72, kParamAmpSustain, 85}, {72, kParamAmpRelease, 300}, {72, kParamLfoRate, 0.2},
  {72, kParamLfoDest, 1},     {72, kParamLfoAmount, 10},  {72, kParamFilterDrive, 8},
  {72, kParamIns1Algo, 1},    {72, kParamIns1A, 24},      {72, kParamIns1B, 30},
  {72, kParamIns1C, 48},      {72, kParamIns1Mix, 16},    {72, kParamMstAlgo, 1},
  {72, kParamMstA, 62},       {72, kParamMstB, 44},       {72, kParamMstC, 20},
  {72, kParamMstMix, 26},

  // Family 04 / 009 Air Lead
  {73, kParamPolyMode, 1},    {73, kParamPorta, 80},      {73, kParamOsc1Shape, 22},
  {73, kParamOsc2Shape, 36},  {73, kParamOsc2Pitch, 12},  {73, kParamOsc2Level, 48},
  {73, kParamNoiseLevel, 8},  {73, kParamUnisonVoices, 2}, {73, kParamUnisonDetune, 14},
  {73, kParamFilterCutoff, 6200}, {73, kParamFilterReso, 10}, {73, kParamFilterKeyTrack, 25},
  {73, kParamAmpAttack, 30},  {73, kParamAmpDecay, 600},  {73, kParamAmpSustain, 70},
  {73, kParamAmpRelease, 500}, {73, kParamLfoRate, 0.35},  {73, kParamLfoDest, 2},
  {73, kParamLfoAmount, 12},  {73, kParamModWheelDest, 1}, {73, kParamModWheelAmount, 28},
  {73, kParamIns1Algo, 3},    {73, kParamIns1A, 32},      {73, kParamIns1B, 24},
  {73, kParamIns1C, 60},      {73, kParamIns1Mix, 20},    {73, kParamIns2Algo, 2},
  {73, kParamIns2A, 16},      {73, kParamIns2B, 26},      {73, kParamIns2C, 66},
  {73, kParamIns2Mix, 16},    {73, kParamMstAlgo, 3},     {73, kParamMstA, 58},
  {73, kParamMstB, 32},       {73, kParamMstC, 50},       {73, kParamMstMix, 22},

  // Family 04 / 010 Swell Brass
  {74, kParamOsc1Shape, 40},  {74, kParamOsc2Shape, 52},  {74, kParamOsc2Pitch, 7},
  {74, kParamOsc2Level, 72},  {74, kParamUnisonVoices, 4}, {74, kParamUnisonDetune, 26},
  {74, kParamFilterCutoff, 1600}, {74, kParamFilterReso, 16}, {74, kParamFilterEnv, 60},
  {74, kParamFiltAttack, 500}, {74, kParamFiltDecay, 1200}, {74, kParamFiltSustain, 55},
  {74, kParamFiltRelease, 1400}, {74, kParamAmpAttack, 280}, {74, kParamAmpDecay, 1400},
  {74, kParamAmpSustain, 78}, {74, kParamAmpRelease, 900}, {74, kParamLfoRate, 0.15},
  {74, kParamLfoDest, 1},     {74, kParamLfoAmount, 14},  {74, kParamVelToFilter, 35},
  {74, kParamIns1Algo, 2},    {74, kParamIns1A, 10},      {74, kParamIns1B, 40},
  {74, kParamIns1C, 56},      {74, kParamIns1Mix, 24},    {74, kParamIns2Algo, 3},
  {74, kParamIns2A, 40},      {74, kParamIns2B, 22},      {74, kParamIns2C, 48},
  {74, kParamIns2Mix, 14},    {74, kParamMstAlgo, 3},     {74, kParamMstA, 60},
  {74, kParamMstB, 30},       {74, kParamMstC, 50},       {74, kParamMstMix, 24},

  // Family 04 / 011 Bowed Strings
  {75, kParamOsc1Shape, 34},  {75, kParamOsc2Shape, 28},  {75, kParamOsc2Pitch, 12},
  {75, kParamOsc2Level, 78},  {75, kParamUnisonVoices, 6}, {75, kParamUnisonDetune, 34},
  {75, kParamFilterCutoff, 2400}, {75, kParamFilterReso, 8},  {75, kParamFilterEnv, 15},
  {75, kParamAmpAttack, 1100}, {75, kParamAmpDecay, 2600}, {75, kParamAmpSustain, 86},
  {75, kParamAmpRelease, 2600}, {75, kParamLfoRate, 0.08},  {75, kParamLfoDest, 1},
  {75, kParamLfoAmount, 10},  {75, kParamSynthMasterLevel, -3},
  {75, kParamIns1Algo, 2},    {75, kParamIns1A, 8},       {75, kParamIns1B, 50},
  {75, kParamIns1C, 60},      {75, kParamIns1Mix, 30},    {75, kParamIns2Algo, 3},
  {75, kParamIns2A, 46},      {75, kParamIns2B, 26},      {75, kParamIns2C, 46},
  {75, kParamIns2Mix, 16},    {75, kParamMstAlgo, 3},     {75, kParamMstA, 68},
  {75, kParamMstB, 28},       {75, kParamMstC, 50},       {75, kParamMstMix, 24},

  // Family 04 / 012 Air Choir
  {76, kParamOsc1Shape, 28},  {76, kParamOsc2Shape, 45},  {76, kParamOsc2Pitch, 12},
  {76, kParamUnisonVoices, 6}, {76, kParamUnisonDetune, 30}, {76, kParamFilterCutoff, 5500},
  {76, kParamFilterReso, 14}, {76, kParamAmpAttack, 700}, {76, kParamAmpDecay, 2000},
  {76, kParamAmpSustain, 85}, {76, kParamAmpRelease, 2600}, {76, kParamTapeHiss, 18},
  {76, kParamTapeMix, 40},
  {76, kParamIns1Algo, 2},    {76, kParamIns1A, 12},      {76, kParamIns1B, 44},
  {76, kParamIns1C, 68},      {76, kParamIns1Mix, 28},    {76, kParamIns2Algo, 3},
  {76, kParamIns2A, 50},      {76, kParamIns2B, 24},      {76, kParamIns2C, 44},
  {76, kParamIns2Mix, 18},    {76, kParamMstAlgo, 3},     {76, kParamMstA, 66},
  {76, kParamMstB, 26},       {76, kParamMstC, 50},       {76, kParamMstMix, 26},

  // Family 04 / 013 Cloud Pad
  {77, kParamOsc1Shape, 16},  {77, kParamOsc2Shape, 58},  {77, kParamOsc2Pitch, 5},
  {77, kParamOsc2Level, 62},  {77, kParamUnisonVoices, 5}, {77, kParamUnisonDetune, 42},
  {77, kParamFilterCutoff, 2800}, {77, kParamFilterReso, 18}, {77, kParamFilterEnv, 30},
  {77, kParamAmpAttack, 1600}, {77, kParamAmpDecay, 3200}, {77, kParamAmpSustain, 74},
  {77, kParamAmpRelease, 3600}, {77, kParamLfoRate, 0.06},  {77, kParamLfoShape, 1},
  {77, kParamLfoDest, 1},     {77, kParamLfoAmount, 26},  {77, kParamSynthMasterLevel, -4},
  {77, kParamIns1Algo, 2},    {77, kParamIns1A, 12},      {77, kParamIns1B, 62},
  {77, kParamIns1C, 72},      {77, kParamIns1Mix, 36},    {77, kParamIns2Algo, 3},
  {77, kParamIns2A, 58},      {77, kParamIns2B, 34},      {77, kParamIns2C, 52},
  {77, kParamIns2Mix, 22},    {77, kParamMstAlgo, 3},     {77, kParamMstA, 70},
  {77, kParamMstB, 24},       {77, kParamMstC, 50},       {77, kParamMstMix, 30},

  // Family 04 / 014 Sparse Pad
  {78, kParamOsc1Shape, 12},  {78, kParamOsc1Level, 70},  {78, kParamOsc2Shape, 20},
  {78, kParamOsc2Pitch, 12},  {78, kParamOsc2Level, 18},  {78, kParamUnisonVoices, 2},
  {78, kParamUnisonDetune, 10}, {78, kParamFilterCutoff, 4000}, {78, kParamFilterReso, 6},
  {78, kParamAmpAttack, 1500}, {78, kParamAmpDecay, 2000}, {78, kParamAmpSustain, 45},
  {78, kParamAmpRelease, 2400}, {78, kParamTapeMix, 14},    {78, kParamSynthMasterLevel, -6},
  {78, kParamIns1Algo, 2},    {78, kParamIns1A, 8},       {78, kParamIns1B, 18},
  {78, kParamIns1C, 46},      {78, kParamIns1Mix, 12},    {78, kParamIns2Algo, 3},
  {78, kParamIns2A, 42},      {78, kParamIns2B, 16},      {78, kParamIns2C, 40},
  {78, kParamIns2Mix, 10},    {78, kParamMstAlgo, 3},     {78, kParamMstA, 56},
  {78, kParamMstB, 30},       {78, kParamMstC, 50},       {78, kParamMstMix, 16},

  // Family 04 / 015 Motion Pad
  {79, kParamOsc1Shape, 12},  {79, kParamOsc2Shape, 60},  {79, kParamOsc2Pitch, 7},
  {79, kParamUnisonVoices, 5}, {79, kParamUnisonDetune, 55}, {79, kParamFilterCutoff, 2600},
  {79, kParamFilterReso, 20}, {79, kParamFilterEnv, 65},  {79, kParamAmpAttack, 900},
  {79, kParamAmpDecay, 3000}, {79, kParamAmpSustain, 70}, {79, kParamAmpRelease, 3500},
  {79, kParamFiltAttack, 1200}, {79, kParamFiltDecay, 3000}, {79, kParamFiltSustain, 60},
  {79, kParamFiltRelease, 3000}, {79, kParamLfoRate, 0.25},  {79, kParamLfoDest, 1},
  {79, kParamLfoAmount, 40},
  {79, kParamIns1Algo, 2},    {79, kParamIns1A, 24},      {79, kParamIns1B, 58},
  {79, kParamIns1C, 82},      {79, kParamIns1Mix, 35},    {79, kParamIns2Algo, 3},
  {79, kParamIns2A, 60},      {79, kParamIns2B, 38},      {79, kParamIns2C, 48},
  {79, kParamIns2Mix, 24},    {79, kParamMstAlgo, 3},     {79, kParamMstA, 72},
  {79, kParamMstB, 28},       {79, kParamMstC, 50},       {79, kParamMstMix, 55},

  // Family 04 / 016 Tilt Texture
  {80, kParamOsc1Shape, 46},  {80, kParamOsc2Shape, 74},  {80, kParamOsc2Pitch, 7},
  {80, kParamOsc2Level, 58},  {80, kParamOscRing, 1},     {80, kParamNoiseLevel, 10},
  {80, kParamFilterType, 2},  {80, kParamFilterCutoff, 1100}, {80, kParamFilterReso, 36},
  {80, kParamUnisonVoices, 3}, {80, kParamUnisonDetune, 38}, {80, kParamAmpAttack, 400},
  {80, kParamAmpDecay, 2200}, {80, kParamAmpSustain, 58}, {80, kParamAmpRelease, 2000},
  {80, kParamLfoRate, 0.9},   {80, kParamLfoShape, 2},    {80, kParamLfoDest, 2},
  {80, kParamLfoAmount, 18},
  {80, kParamIns1Algo, 4},    {80, kParamIns1A, 28},      {80, kParamIns1B, 22},
  {80, kParamIns1C, 30},      {80, kParamIns1Mix, 16},    {80, kParamIns2Algo, 3},
  {80, kParamIns2A, 40},      {80, kParamIns2B, 30},      {80, kParamIns2C, 44},
  {80, kParamIns2Mix, 18},    {80, kParamMstAlgo, 4},     {80, kParamMstA, 72},
  {80, kParamMstB, 35},       {80, kParamMstC, 20},       {80, kParamMstMix, 48},

  // Family 04 / 017 Slow Drone
  {81, kParamOsc1Shape, 5},   {81, kParamOsc2Shape, 50},  {81, kParamOsc2Pitch, -12},
  {81, kParamSubLevel, 60},   {81, kParamNoiseLevel, 12}, {81, kParamFilterCutoff, 1200},
  {81, kParamFilterReso, 35}, {81, kParamFilterEnv, 20},  {81, kParamAmpAttack, 2000},
  {81, kParamAmpDecay, 4000}, {81, kParamAmpSustain, 95}, {81, kParamAmpRelease, 4000},
  {81, kParamIns1Algo, 2},    {81, kParamIns1A, 6},       {81, kParamIns1B, 34},
  {81, kParamIns1C, 50},      {81, kParamIns1Mix, 20},    {81, kParamIns2Algo, 3},
  {81, kParamIns2A, 64},      {81, kParamIns2B, 36},      {81, kParamIns2C, 30},
  {81, kParamIns2Mix, 16},    {81, kParamMstAlgo, 4},     {81, kParamMstA, 42},
  {81, kParamMstB, 28},       {81, kParamMstC, 10},       {81, kParamMstMix, 22},

  // Family 04 / 018 Seq Arp
  {82, kParamOsc1Shape, 54},  {82, kParamOsc2Shape, 60},  {82, kParamOsc2Pitch, 12},
  {82, kParamOsc2Level, 45},  {82, kParamUnisonVoices, 2}, {82, kParamUnisonDetune, 20},
  {82, kParamFilterCutoff, 5200}, {82, kParamFilterReso, 22}, {82, kParamFilterEnv, 55},
  {82, kParamFilterKeyTrack, 55}, {82, kParamAmpAttack, 1},   {82, kParamAmpDecay, 70},
  {82, kParamAmpSustain, 0},  {82, kParamAmpRelease, 100}, {82, kParamLfoRate, 4},
  {82, kParamLfoShape, 3},    {82, kParamLfoDest, 1},     {82, kParamLfoAmount, 20},
  {82, kParamIns1Algo, 3},    {82, kParamIns1A, 16},      {82, kParamIns1B, 18},
  {82, kParamIns1C, 62},      {82, kParamIns1Mix, 12},    {82, kParamIns2Algo, 2},
  {82, kParamIns2A, 20},      {82, kParamIns2B, 20},      {82, kParamIns2C, 60},
  {82, kParamIns2Mix, 12},    {82, kParamMstAlgo, 3},     {82, kParamMstA, 58},
  {82, kParamMstB, 30},       {82, kParamMstC, 50},       {82, kParamMstMix, 20},

  // Family 04 / 019 Wash Delay
  {83, kParamOsc1Shape, 20},  {83, kParamOsc2Shape, 40},  {83, kParamOsc2Pitch, 7},
  {83, kParamOsc2Level, 55},  {83, kParamFilterCutoff, 3600}, {83, kParamFilterReso, 12},
  {83, kParamAmpAttack, 200}, {83, kParamAmpDecay, 1600}, {83, kParamAmpSustain, 40},
  {83, kParamAmpRelease, 2000}, {83, kParamSynthMasterLevel, -3},
  {83, kParamIns1Algo, 3},    {83, kParamIns1A, 68},      {83, kParamIns1B, 55},
  {83, kParamIns1C, 50},      {83, kParamIns1Mix, 62},    {83, kParamIns2Algo, 2},
  {83, kParamIns2A, 14},      {83, kParamIns2B, 46},      {83, kParamIns2C, 70},
  {83, kParamIns2Mix, 28},    {83, kParamMstAlgo, 3},     {83, kParamMstA, 74},
  {83, kParamMstB, 28},       {83, kParamMstC, 50},       {83, kParamMstMix, 36},

  // Family 04 / 020 Metal Kit
  {84, kParamEngine, 1},      {84, kParamOsc1Shape, 96},  {84, kParamOsc1PW, 22},
  {84, kParamOsc2Shape, 88},  {84, kParamOsc2Pitch, 7},   {84, kParamOsc2Level, 55},
  {84, kParamNoiseLevel, 50}, {84, kParamFilterType, 3},  {84, kParamFilterCutoff, 4000},
  {84, kParamFilterReso, 18}, {84, kParamAmpAttack, 0.5}, {84, kParamAmpDecay, 200},
  {84, kParamAmpSustain, 0},  {84, kParamAmpRelease, 180}, {84, kParamPitchAmount, -6},
  {84, kParamPitchDecay, 40}, {84, kParamPitchRelease, 40}, {84, kParamDrumSlotTune, -3},
  {84, kParamDrumSlotDecay, 62}, {84, kParamDrumSlotDrive, 35}, {84, kParamDrumSlotFilter, 16000},
  {84, kParamDrumKitLevel, -2}, {84, kParamFilterDrive, 22},
  {84, kParamIns1Algo, 4},    {84, kParamIns1A, 24},      {84, kParamIns1B, 20},
  {84, kParamIns1C, 34},      {84, kParamIns1Mix, 18},    {84, kParamIns2Algo, 1},
  {84, kParamIns2A, 46},      {84, kParamIns2B, 56},      {84, kParamIns2C, 40},
  {84, kParamIns2Mix, 22},    {84, kParamMstAlgo, 2},     {84, kParamMstA, 32},
  {84, kParamMstB, 44},       {84, kParamMstC, 18},       {84, kParamMstMix, 48},

  // Family 05 / 001 Stage Keys
  {85, kParamOsc1Shape, 24},  {85, kParamOsc2Shape, 16},  {85, kParamOsc2Pitch, 12},
  {85, kParamOsc2Level, 42},  {85, kParamFilterCutoff, 7000}, {85, kParamFilterReso, 6},
  {85, kParamFilterEnv, 16},  {85, kParamAmpAttack, 5},   {85, kParamAmpDecay, 850},
  {85, kParamAmpSustain, 36}, {85, kParamAmpRelease, 550}, {85, kParamVelToAmp, 32},
  {85, kParamTapeSat, 16},    {85, kParamTapeMix, 18},
  {85, kParamIns1Algo, 2},    {85, kParamIns1A, 11},      {85, kParamIns1B, 18},
  {85, kParamIns1C, 40},      {85, kParamIns1Mix, 14},    {85, kParamMstAlgo, 1},
  {85, kParamMstA, 60},       {85, kParamMstB, 36},       {85, kParamMstC, 34},
  {85, kParamMstMix, 28},

  // Family 05 / 002 Reed Piano
  {86, kParamOsc1Shape, 30},  {86, kParamOsc2Shape, 24},  {86, kParamOsc2Pitch, 19},
  {86, kParamOsc2Level, 28},  {86, kParamFmAmount, 14},   {86, kParamFilterCutoff, 3000},
  {86, kParamFilterReso, 9},  {86, kParamFilterEnv, 16},  {86, kParamAmpAttack, 5},
  {86, kParamAmpDecay, 1000}, {86, kParamAmpSustain, 32}, {86, kParamAmpRelease, 640},
  {86, kParamVelToAmp, 48},   {86, kParamVelToFilter, 18}, {86, kParamTapeMix, 20},
  {86, kParamIns1Algo, 2},    {86, kParamIns1A, 9},       {86, kParamIns1B, 14},
  {86, kParamIns1C, 36},      {86, kParamIns1Mix, 10},    {86, kParamMstAlgo, 4},
  {86, kParamMstA, 58},       {86, kParamMstB, 42},       {86, kParamMstC, 12},
  {86, kParamMstMix, 18},

  // Family 05 / 003 Funk Clav
  {87, kParamOsc1Shape, 80},  {87, kParamOsc1PW, 26},     {87, kParamOsc2Shape, 86},
  {87, kParamOsc2Pitch, 12},  {87, kParamOsc2Level, 58},  {87, kParamOsc2PW, 20},
  {87, kParamFilterCutoff, 2500}, {87, kParamFilterReso, 36}, {87, kParamFilterEnv, 66},
  {87, kParamFilterDrive, 16}, {87, kParamFilterKeyTrack, 28}, {87, kParamFiltAttack, 1},
  {87, kParamFiltDecay, 130}, {87, kParamFiltSustain, 0}, {87, kParamFiltRelease, 110},
  {87, kParamAmpAttack, 1},   {87, kParamAmpDecay, 190},  {87, kParamAmpSustain, 0},
  {87, kParamAmpRelease, 140}, {87, kParamVelToAmp, 72},   {87, kParamTapeSat, 18},
  {87, kParamTapeMix, 14},
  {87, kParamIns1Algo, 1},    {87, kParamIns1A, 34},      {87, kParamIns1B, 46},
  {87, kParamIns1C, 42},      {87, kParamIns1Mix, 22},    {87, kParamMstAlgo, 1},
  {87, kParamMstA, 56},       {87, kParamMstB, 44},       {87, kParamMstC, 24},
  {87, kParamMstMix, 26},

  // Family 05 / 004 Rock Organ
  {88, kParamOsc1Shape, 36},  {88, kParamOsc1Level, 100}, {88, kParamOsc2Shape, 33},
  {88, kParamOsc2Pitch, 12},  {88, kParamOsc2Level, 78},  {88, kParamSubLevel, 28},
  {88, kParamFilterCutoff, 6800}, {88, kParamFilterReso, 14}, {88, kParamFilterEnv, 35},
  {88, kParamFiltAttack, 1},  {88, kParamFiltDecay, 90},  {88, kParamFiltSustain, 30},
  {88, kParamFiltRelease, 80}, {88, kParamAmpAttack, 2},   {88, kParamAmpDecay, 40},
  {88, kParamAmpSustain, 100}, {88, kParamAmpRelease, 70}, {88, kParamFilterDrive, 18},
  {88, kParamLfoRate, 6},     {88, kParamLfoDest, 1},     {88, kParamLfoAmount, 6},
  {88, kParamIns1Algo, 1},    {88, kParamIns1A, 42},      {88, kParamIns1B, 38},
  {88, kParamIns1C, 46},      {88, kParamIns1Mix, 20},    {88, kParamMstAlgo, 1},
  {88, kParamMstA, 54},       {88, kParamMstB, 40},       {88, kParamMstC, 32},
  {88, kParamMstMix, 26},

  // Family 05 / 005 Ping Bells
  {89, kParamOsc1Shape, 9},   {89, kParamOsc2Shape, 14},  {89, kParamOsc2Pitch, 24},
  {89, kParamOsc2Level, 36},  {89, kParamFmAmount, 20},   {89, kParamFilterCutoff, 12000},
  {89, kParamFilterKeyTrack, 70}, {89, kParamAmpAttack, 1},   {89, kParamAmpDecay, 900},
  {89, kParamAmpSustain, 0},  {89, kParamAmpRelease, 1100}, {89, kParamSynthMasterLevel, -3},
  {89, kParamIns1Algo, 3},    {89, kParamIns1A, 12},      {89, kParamIns1B, 8},
  {89, kParamIns1C, 80},      {89, kParamIns1Mix, 8},     {89, kParamMstAlgo, 3},
  {89, kParamMstA, 60},       {89, kParamMstB, 28},       {89, kParamMstC, 50},
  {89, kParamMstMix, 20},

  // Family 05 / 006 Mute Pluck
  {90, kParamOsc1Shape, 52},  {90, kParamOsc1PW, 38},     {90, kParamOsc2Level, 10},
  {90, kParamFilterCutoff, 1800}, {90, kParamFilterReso, 12}, {90, kParamFilterEnv, 48},
  {90, kParamFilterKeyTrack, 80}, {90, kParamFiltAttack, 1},  {90, kParamFiltDecay, 60},
  {90, kParamFiltSustain, 0}, {90, kParamFiltRelease, 50}, {90, kParamAmpAttack, 1},
  {90, kParamAmpDecay, 90},   {90, kParamAmpSustain, 0},  {90, kParamAmpRelease, 70},
  {90, kParamTapeMix, 8},
  {90, kParamIns1Algo, 1},    {90, kParamIns1A, 20},      {90, kParamIns1B, 44},
  {90, kParamIns1C, 50},      {90, kParamIns1Mix, 10},    {90, kParamMstAlgo, 1},
  {90, kParamMstA, 58},       {90, kParamMstB, 30},       {90, kParamMstC, 22},
  {90, kParamMstMix, 18},

  // Family 05 / 007 Short Stab
  {91, kParamOsc1Shape, 58},  {91, kParamOsc2Shape, 62},  {91, kParamOsc2Pitch, 12},
  {91, kParamOsc2Level, 48},  {91, kParamFilterCutoff, 4600}, {91, kParamFilterReso, 20},
  {91, kParamFilterEnv, 58},  {91, kParamAmpAttack, 1},   {91, kParamAmpDecay, 80},
  {91, kParamAmpSustain, 0},  {91, kParamAmpRelease, 60}, {91, kParamVelToAmp, 68},
  {91, kParamVelToFilter, 50}, {91, kParamFilterDrive, 14}, {91, kParamTapeSat, 12},
  {91, kParamTapeMix, 10},
  {91, kParamIns1Algo, 1},    {91, kParamIns1A, 28},      {91, kParamIns1B, 48},
  {91, kParamIns1C, 44},      {91, kParamIns1Mix, 16},    {91, kParamMstAlgo, 1},
  {91, kParamMstA, 52},       {91, kParamMstB, 46},       {91, kParamMstC, 20},
  {91, kParamMstMix, 26},

  // Family 05 / 008 Round Bass
  {92, kParamPolyMode, 1},    {92, kParamMonoLegato, 1},  {92, kParamOsc1Shape, 18},
  {92, kParamOsc1Pitch, -12}, {92, kParamOsc2Shape, 12},  {92, kParamOsc2Pitch, -12},
  {92, kParamOsc2Level, 35},  {92, kParamSubLevel, 65},   {92, kParamFilterCutoff, 980},
  {92, kParamFilterReso, 16}, {92, kParamFilterEnv, 32},  {92, kParamAmpAttack, 3},
  {92, kParamAmpDecay, 340},  {92, kParamAmpSustain, 78}, {92, kParamAmpRelease, 200},
  {92, kParamVelToAmp, 36},   {92, kParamTapeSat, 14},    {92, kParamTapeMix, 12},
  {92, kParamIns1Algo, 1},    {92, kParamIns1A, 24},      {92, kParamIns1B, 55},
  {92, kParamIns1C, 48},      {92, kParamIns1Mix, 18},    {92, kParamMstAlgo, 1},
  {92, kParamMstA, 62},       {92, kParamMstB, 40},       {92, kParamMstC, 22},
  {92, kParamMstMix, 26},

  // Family 05 / 009 Saw Lead
  {93, kParamEngine, 1},      {93, kParamPolyMode, 1},    {93, kParamPorta, 40},
  {93, kParamOsc1Shape, 70},  {93, kParamOsc2Shape, 66},  {93, kParamOsc2Pitch, 0.01},
  {93, kParamOsc2Level, 60},  {93, kParamUnisonVoices, 2}, {93, kParamUnisonDetune, 8},
  {93, kParamFilterCutoff, 3900}, {93, kParamFilterReso, 22}, {93, kParamFilterEnv, 30},
  {93, kParamFilterDrive, 20}, {93, kParamFilterKeyTrack, 35}, {93, kParamAmpAttack, 8},
  {93, kParamAmpDecay, 240},  {93, kParamAmpSustain, 82}, {93, kParamAmpRelease, 180},
  {93, kParamModWheelDest, 1}, {93, kParamModWheelAmount, 32},
  {93, kParamIns1Algo, 1},    {93, kParamIns1A, 46},      {93, kParamIns1B, 40},
  {93, kParamIns1C, 38},      {93, kParamIns1Mix, 24},    {93, kParamMstAlgo, 1},
  {93, kParamMstA, 56},       {93, kParamMstB, 38},       {93, kParamMstC, 30},
  {93, kParamMstMix, 24},

  // Family 05 / 010 Section Brass
  {94, kParamOsc1Shape, 50},  {94, kParamOsc2Shape, 54},  {94, kParamOsc2Pitch, 12},
  {94, kParamOsc2Level, 64},  {94, kParamUnisonVoices, 2}, {94, kParamUnisonDetune, 10},
  {94, kParamFilterCutoff, 2200}, {94, kParamFilterReso, 12}, {94, kParamFilterEnv, 52},
  {94, kParamFiltAttack, 25}, {94, kParamFiltDecay, 220}, {94, kParamFiltSustain, 50},
  {94, kParamFiltRelease, 300}, {94, kParamAmpAttack, 22},  {94, kParamAmpDecay, 400},
  {94, kParamAmpSustain, 74}, {94, kParamAmpRelease, 280}, {94, kParamVelToFilter, 42},
  {94, kParamIns1Algo, 2},    {94, kParamIns1A, 8},       {94, kParamIns1B, 16},
  {94, kParamIns1C, 44},      {94, kParamIns1Mix, 12},    {94, kParamMstAlgo, 1},
  {94, kParamMstA, 56},       {94, kParamMstB, 42},       {94, kParamMstC, 38},
  {94, kParamMstMix, 24},

  // Family 05 / 011 Small Strings
  {95, kParamOsc1Shape, 38},  {95, kParamOsc2Shape, 34},  {95, kParamOsc2Pitch, 12},
  {95, kParamOsc2Level, 55},  {95, kParamUnisonVoices, 2}, {95, kParamUnisonDetune, 9},
  {95, kParamFilterCutoff, 3400}, {95, kParamFilterReso, 5},  {95, kParamAmpAttack, 240},
  {95, kParamAmpDecay, 900},  {95, kParamAmpSustain, 72}, {95, kParamAmpRelease, 800},
  {95, kParamTapeMix, 16},    {95, kParamSynthMasterLevel, -2},
  {95, kParamIns1Algo, 2},    {95, kParamIns1A, 7},       {95, kParamIns1B, 20},
  {95, kParamIns1C, 42},      {95, kParamIns1Mix, 14},    {95, kParamMstAlgo, 3},
  {95, kParamMstA, 58},       {95, kParamMstB, 30},       {95, kParamMstC, 50},
  {95, kParamMstMix, 20},

  // Family 05 / 012 Close Choir
  {96, kParamOsc1Shape, 21},  {96, kParamOsc2Shape, 27},  {96, kParamOsc2Pitch, 12},
  {96, kParamOsc2Level, 40},  {96, kParamNoiseLevel, 4},  {96, kParamUnisonVoices, 2},
  {96, kParamUnisonDetune, 8}, {96, kParamFilterCutoff, 4200}, {96, kParamFilterReso, 7},
  {96, kParamAmpAttack, 280}, {96, kParamAmpDecay, 1200}, {96, kParamAmpSustain, 80},
  {96, kParamAmpRelease, 900}, {96, kParamTapeHiss, 4},    {96, kParamTapeMix, 18},
  {96, kParamIns1Algo, 2},    {96, kParamIns1A, 6},       {96, kParamIns1B, 14},
  {96, kParamIns1C, 40},      {96, kParamIns1Mix, 10},    {96, kParamMstAlgo, 3},
  {96, kParamMstA, 56},       {96, kParamMstB, 30},       {96, kParamMstC, 50},
  {96, kParamMstMix, 18},

  // Family 05 / 013 Tape Pad
  {97, kParamOsc1Shape, 26},  {97, kParamOsc2Shape, 32},  {97, kParamOsc2Pitch, 7},
  {97, kParamOsc2Level, 50},  {97, kParamUnisonVoices, 2}, {97, kParamUnisonDetune, 12},
  {97, kParamFilterCutoff, 2600}, {97, kParamFilterReso, 10}, {97, kParamAmpAttack, 600},
  {97, kParamAmpDecay, 1800}, {97, kParamAmpSustain, 66}, {97, kParamAmpRelease, 2000},
  {97, kParamTapeSat, 32},    {97, kParamTapeBump, 28},   {97, kParamTapeWow, 18},
  {97, kParamTapeFlutter, 14}, {97, kParamTapeMix, 42},
  {97, kParamIns1Algo, 3},    {97, kParamIns1A, 30},      {97, kParamIns1B, 20},
  {97, kParamIns1C, 46},      {97, kParamIns1Mix, 14},    {97, kParamMstAlgo, 3},
  {97, kParamMstA, 62},       {97, kParamMstB, 28},       {97, kParamMstC, 50},
  {97, kParamMstMix, 22},

  // Family 05 / 014 Filter Motion
  {98, kParamOsc1Shape, 48},  {98, kParamOsc2Shape, 56},  {98, kParamOsc2Pitch, 5},
  {98, kParamOsc2Level, 60},  {98, kParamFilterCutoff, 700}, {98, kParamFilterReso, 28},
  {98, kParamFilterEnv, 70},  {98, kParamFiltAttack, 800}, {98, kParamFiltDecay, 1600},
  {98, kParamFiltSustain, 20}, {98, kParamFiltRelease, 1400}, {98, kParamAmpAttack, 120},
  {98, kParamAmpDecay, 2000}, {98, kParamAmpSustain, 64}, {98, kParamAmpRelease, 1800},
  {98, kParamLfoRate, 0.16},  {98, kParamLfoShape, 2},    {98, kParamLfoDest, 1},
  {98, kParamLfoAmount, 36},
  {98, kParamIns1Algo, 2},    {98, kParamIns1A, 18},      {98, kParamIns1B, 28},
  {98, kParamIns1C, 52},      {98, kParamIns1Mix, 18},    {98, kParamMstAlgo, 3},
  {98, kParamMstA, 60},       {98, kParamMstB, 30},       {98, kParamMstC, 50},
  {98, kParamMstMix, 24},

  // Family 05 / 015 Broken Texture
  {99, kParamEngine, 1},      {99, kParamOsc1Shape, 62},  {99, kParamOsc2Shape, 96},
  {99, kParamOsc2Pitch, 6},   {99, kParamOsc2Level, 44},  {99, kParamOscRing, 1},
  {99, kParamNoiseLevel, 18}, {99, kParamFilterType, 3},  {99, kParamFilterCutoff, 2400},
  {99, kParamFilterReso, 24}, {99, kParamAmpAttack, 40},  {99, kParamAmpDecay, 900},
  {99, kParamAmpSustain, 40}, {99, kParamAmpRelease, 700}, {99, kParamLfoRate, 7},
  {99, kParamLfoShape, 4},    {99, kParamLfoDest, 3},     {99, kParamLfoAmount, 28},
  {99, kParamIns1Algo, 4},    {99, kParamIns1A, 40},      {99, kParamIns1B, 30},
  {99, kParamIns1C, 36},      {99, kParamIns1Mix, 22},    {99, kParamMstAlgo, 4},
  {99, kParamMstA, 22},       {99, kParamMstB, 55},       {99, kParamMstC, 16},
  {99, kParamMstMix, 26},

  // Family 05 / 016 Dark Drone
  {100, kParamOsc1Shape, 4},   {100, kParamOsc2Shape, 10},  {100, kParamOsc2Pitch, -24},
  {100, kParamOsc2Level, 55},  {100, kParamSubLevel, 75},   {100, kParamNoiseLevel, 8},
  {100, kParamFilterCutoff, 420}, {100, kParamFilterReso, 30}, {100, kParamFilterEnv, 6},
  {100, kParamAmpAttack, 2400}, {100, kParamAmpDecay, 4000}, {100, kParamAmpSustain, 98},
  {100, kParamAmpRelease, 4000}, {100, kParamLfoRate, 0.04},  {100, kParamLfoDest, 1},
  {100, kParamLfoAmount, 12},  {100, kParamFilterDrive, 10}, {100, kParamSynthMasterLevel, -8},
  {100, kParamTapeSat, 20},    {100, kParamTapeMix, 24},
  {100, kParamIns1Algo, 2},    {100, kParamIns1A, 5},       {100, kParamIns1B, 24},
  {100, kParamIns1C, 38},      {100, kParamIns1Mix, 16},    {100, kParamMstAlgo, 4},
  {100, kParamMstA, 34},       {100, kParamMstB, 24},       {100, kParamMstC, 10},
  {100, kParamMstMix, 20},

  // Family 05 / 017 Tight Arp
  {101, kParamOsc1Shape, 60},  {101, kParamOsc2Shape, 48},  {101, kParamOsc2Pitch, 12},
  {101, kParamOsc2Level, 30},  {101, kParamFilterCutoff, 4400}, {101, kParamFilterReso, 18},
  {101, kParamFilterEnv, 62},  {101, kParamFilterKeyTrack, 72}, {101, kParamAmpAttack, 1},
  {101, kParamAmpDecay, 55},   {101, kParamAmpSustain, 0},  {101, kParamAmpRelease, 70},
  {101, kParamFiltAttack, 1},  {101, kParamFiltDecay, 40},  {101, kParamFiltSustain, 0},
  {101, kParamFiltRelease, 40}, {101, kParamTapeMix, 8},
  {101, kParamIns1Algo, 3},    {101, kParamIns1A, 10},      {101, kParamIns1B, 8},
  {101, kParamIns1C, 56},      {101, kParamIns1Mix, 6},     {101, kParamMstAlgo, 1},
  {101, kParamMstA, 52},       {101, kParamMstB, 30},       {101, kParamMstC, 20},
  {101, kParamMstMix, 16},

  // Family 05 / 018 Room Crush
  {102, kParamEngine, 1},      {102, kParamOsc1Shape, 66},  {102, kParamOsc2Shape, 74},
  {102, kParamOsc2Pitch, -12}, {102, kParamOsc2Level, 50},  {102, kParamFilterCutoff, 2800},
  {102, kParamFilterReso, 18}, {102, kParamFilterDrive, 36}, {102, kParamAmpAttack, 2},
  {102, kParamAmpDecay, 320},  {102, kParamAmpSustain, 25}, {102, kParamAmpRelease, 240},
  {102, kParamOutLevel, -2},   {102, kParamTapeSat, 22},    {102, kParamTapeMix, 12},
  {102, kParamIns1Algo, 4},    {102, kParamIns1A, 58},      {102, kParamIns1B, 46},
  {102, kParamIns1C, 32},      {102, kParamIns1Mix, 55},    {102, kParamMstAlgo, 2},
  {102, kParamMstA, 32},       {102, kParamMstB, 48},       {102, kParamMstC, 22},
  {102, kParamMstMix, 50},

  // Family 05 / 019 Dry Kit
  {103, kParamOsc1Shape, 92}, {103, kParamOsc1PW, 40},    {103, kParamOsc2Shape, 88},
  {103, kParamOsc2Level, 60}, {103, kParamSubLevel, 35},  {103, kParamNoiseLevel, 30},
  {103, kParamFilterCutoff, 7000}, {103, kParamFilterReso, 15}, {103, kParamFilterEnv, 40},
  {103, kParamAmpAttack, 0.5}, {103, kParamAmpDecay, 110}, {103, kParamAmpSustain, 0},
  {103, kParamAmpRelease, 90}, {103, kParamPitchAmount, -12}, {103, kParamPitchDecay, 70},
  {103, kParamTapeSat, 15},   {103, kParamTapeMix, 15},   {103, kParamDrumSlotDecay, 42},
  {103, kParamDrumSlotDrive, 22}, {103, kParamDrumSlotFilter, 10000}, {103, kParamDrumKitLevel, -1},
  {103, kParamIns1Algo, 1},   {103, kParamIns1A, 28},     {103, kParamIns1B, 50},
  {103, kParamIns1C, 46},     {103, kParamIns1Mix, 20},   {103, kParamMstAlgo, 2},
  {103, kParamMstA, 38},      {103, kParamMstB, 40},      {103, kParamMstC, 14},
  {103, kParamMstMix, 42},

  // Family 05 / 020 Hard Kit
  {104, kParamEngine, 1},     {104, kParamOsc1Shape, 100}, {104, kParamOsc1PW, 12},
  {104, kParamOsc1Level, 100}, {104, kParamOsc2Shape, 100}, {104, kParamOsc2Pitch, -12},
  {104, kParamOsc2Level, 85}, {104, kParamNoiseLevel, 45}, {104, kParamFilterCutoff, 12000},
  {104, kParamFilterReso, 10}, {104, kParamFilterEnv, 0},  {104, kParamAmpAttack, 0.5},
  {104, kParamAmpDecay, 160}, {104, kParamAmpSustain, 0}, {104, kParamAmpRelease, 120},
  {104, kParamPitchAmount, -18}, {104, kParamPitchAttack, 0.5}, {104, kParamPitchDecay, 90},
  {104, kParamPitchSustain, 0}, {104, kParamPitchRelease, 90}, {104, kParamTapeSat, 45},
  {104, kParamTapeMix, 12},   {104, kParamOutLevel, -1.5}, {104, kParamDrumSlotTune, -2},
  {104, kParamDrumSlotDecay, 26}, {104, kParamDrumSlotDrive, 78}, {104, kParamDrumSlotFilter, 17000},
  {104, kParamIns1Algo, 1},   {104, kParamIns1A, 62},     {104, kParamIns1B, 68},
  {104, kParamIns1C, 54},     {104, kParamIns1Mix, 42},   {104, kParamMstAlgo, 1},
  {104, kParamMstA, 38},      {104, kParamMstB, 65},      {104, kParamMstC, 32},
  {104, kParamMstMix, 70},

  // Family 05 / 021 Tape Kit
  {105, kParamOsc1Shape, 100}, {105, kParamOsc1PW, 50},    {105, kParamOsc2Shape, 55},
  {105, kParamOsc2Pitch, -24}, {105, kParamOsc2Level, 70}, {105, kParamNoiseLevel, 60},
  {105, kParamFilterCutoff, 5000}, {105, kParamFilterReso, 20}, {105, kParamFilterEnv, 60},
  {105, kParamAmpAttack, 0.5}, {105, kParamAmpDecay, 240}, {105, kParamAmpSustain, 0},
  {105, kParamAmpRelease, 260}, {105, kParamPitchAmount, -24}, {105, kParamPitchDecay, 140},
  {105, kParamTapeSat, 60},   {105, kParamTapeBump, 45},  {105, kParamTapeWow, 25},
  {105, kParamTapeFlutter, 30}, {105, kParamTapeMix, 55},   {105, kParamFilterDrive, 45},
  {105, kParamDrumSlotDecay, 75}, {105, kParamDrumSlotDrive, 50}, {105, kParamDrumSlotFilter, 5500},
  {105, kParamDrumKitLevel, -3},
  {105, kParamIns1Algo, 1},   {105, kParamIns1A, 48},     {105, kParamIns1B, 36},
  {105, kParamIns1C, 52},     {105, kParamIns1Mix, 30},   {105, kParamMstAlgo, 2},
  {105, kParamMstA, 30},      {105, kParamMstB, 44},      {105, kParamMstC, 20},
  {105, kParamMstMix, 45},
};

const keith::PresetSpec kPresetSpecs[kNumPresets] = {
  {"Family 01 / 001 Glass Keys", 1, "Triton", "keys"},
  {"Family 01 / 002 Sparse Keys", 1, "Triton", "keys"},
  {"Family 01 / 003 Soft Tines", 1, "Triton", "electric piano"},
  {"Family 01 / 004 Bright Clav", 1, "Triton", "clav"},
  {"Family 01 / 005 Drawbar Organ", 1, "Triton", "organ"},
  {"Family 01 / 006 Glass Bells", 1, "Triton", "bells"},
  {"Family 01 / 007 Crystal Pluck", 1, "Triton", "pluck"},
  {"Family 01 / 008 Key Stab", 1, "Triton", "stab"},
  {"Family 01 / 009 Dry Bass", 1, "Triton", "bass"},
  {"Family 01 / 010 Smooth Lead", 1, "Triton", "lead"},
  {"Family 01 / 011 Warm Brass", 1, "Triton", "brass"},
  {"Family 01 / 012 Silk Strings", 1, "Triton", "strings"},
  {"Family 01 / 013 Soft Choir", 1, "Triton", "choir"},
  {"Family 01 / 014 Bell Pad", 1, "Triton", "pad"},
  {"Family 01 / 015 Slow Motion", 1, "Triton", "motion"},
  {"Family 01 / 016 Shimmer Texture", 1, "Triton", "texture"},
  {"Family 01 / 017 Low Drone", 1, "Triton", "drone"},
  {"Family 01 / 018 Glass Arp", 1, "Triton", "arp"},
  {"Family 01 / 019 Heavy Delay", 1, "Triton", "fx"},
  {"Family 01 / 020 Room Kit", 1, "Triton", "kit"},
  {"Family 01 / 021 Upright Bass", 1, "Triton", "bass"},
  {"Family 01 / 022 Hammer Keys", 1, "Triton", "keys"},
  {"Family 01 / 023 Swell Choir", 1, "Triton", "choir"},
  {"Family 02 / 001 Neon Keys", 2, "Triton Extreme", "keys"},
  {"Family 02 / 002 Thin Piano", 2, "Triton Extreme", "electric piano"},
  {"Family 02 / 003 Thin Clav", 2, "Triton Extreme", "clav"},
  {"Family 02 / 004 Perc Organ", 2, "Triton Extreme", "organ"},
  {"Family 02 / 005 Metal Bells", 2, "Triton Extreme", "bells"},
  {"Family 02 / 006 Neon Pluck", 2, "Triton Extreme", "pluck"},
  {"Family 02 / 007 Sparse Pluck", 2, "Triton Extreme", "pluck"},
  {"Family 02 / 008 Thin Stab", 2, "Triton Extreme", "stab"},
  {"Family 02 / 009 Hard Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 010 Sparse Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 011 Sync Lead", 2, "Triton Extreme", "lead"},
  {"Family 02 / 012 Stab Brass", 2, "Triton Extreme", "brass"},
  {"Family 02 / 013 Tight Strings", 2, "Triton Extreme", "strings"},
  {"Family 02 / 014 Breath Choir", 2, "Triton Extreme", "choir"},
  {"Family 02 / 015 Poly Pad", 2, "Triton Extreme", "pad"},
  {"Family 02 / 016 Sweep Motion", 2, "Triton Extreme", "motion"},
  {"Family 02 / 017 Skew Texture", 2, "Triton Extreme", "texture"},
  {"Family 02 / 018 Pulse Drone", 2, "Triton Extreme", "drone"},
  {"Family 02 / 019 Wide Arp", 2, "Triton Extreme", "arp"},
  {"Family 02 / 020 Heavy Crush", 2, "Triton Extreme", "fx"},
  {"Family 02 / 021 Punch Kit", 2, "Triton Extreme", "kit"},
  {"Family 03 / 001 Piano Stack", 3, "Korg M1", "keys"},
  {"Family 03 / 002 Soft EP", 3, "Korg M1", "electric piano"},
  {"Family 03 / 003 Soul Piano", 3, "Korg M1", "electric piano"},
  {"Family 03 / 004 Clav Stab", 3, "Korg M1", "clav"},
  {"Family 03 / 005 Dry Clav", 3, "Korg M1", "clav"},
  {"Family 03 / 006 Warm Organ", 3, "Korg M1", "organ"},
  {"Family 03 / 007 Bottle Bells", 3, "Korg M1", "bells"},
  {"Family 03 / 008 Muted Pluck", 3, "Korg M1", "pluck"},
  {"Family 03 / 009 Reed Stab", 3, "Korg M1", "stab"},
  {"Family 03 / 010 Finger Bass", 3, "Korg M1", "bass"},
  {"Family 03 / 011 Solo Lead", 3, "Korg M1", "lead"},
  {"Family 03 / 012 Ensemble Brass", 3, "Korg M1", "brass"},
  {"Family 03 / 013 Section Strings", 3, "Korg M1", "strings"},
  {"Family 03 / 014 Ooh Choir", 3, "Korg M1", "choir"},
  {"Family 03 / 015 Dream Pad", 3, "Korg M1", "pad"},
  {"Family 03 / 016 Slow Sweep", 3, "Korg M1", "motion"},
  {"Family 03 / 017 Hollow Texture", 3, "Korg M1", "texture"},
  {"Family 03 / 018 Warm Drone", 3, "Korg M1", "drone"},
  {"Family 03 / 019 Mallet Arp", 3, "Korg M1", "arp"},
  {"Family 03 / 020 Slap Delay", 3, "Korg M1", "fx"},
  {"Family 03 / 021 Dry Perc", 3, "Korg M1", "kit"},
  {"Family 04 / 001 Vector Keys", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 002 Phase Piano", 4, "Korg Wavestation", "electric piano"},
  {"Family 04 / 003 Wave Clav", 4, "Korg Wavestation", "clav"},
  {"Family 04 / 004 Drift Organ", 4, "Korg Wavestation", "organ"},
  {"Family 04 / 005 Chime Bells", 4, "Korg Wavestation", "bells"},
  {"Family 04 / 006 Harp Pluck", 4, "Korg Wavestation", "pluck"},
  {"Family 04 / 007 Soft Stab", 4, "Korg Wavestation", "stab"},
  {"Family 04 / 008 Sub Bass", 4, "Korg Wavestation", "bass"},
  {"Family 04 / 009 Air Lead", 4, "Korg Wavestation", "lead"},
  {"Family 04 / 010 Swell Brass", 4, "Korg Wavestation", "brass"},
  {"Family 04 / 011 Bowed Strings", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 012 Air Choir", 4, "Korg Wavestation", "choir"},
  {"Family 04 / 013 Cloud Pad", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 014 Sparse Pad", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 015 Motion Pad", 4, "Korg Wavestation", "motion"},
  {"Family 04 / 016 Tilt Texture", 4, "Korg Wavestation", "texture"},
  {"Family 04 / 017 Slow Drone", 4, "Korg Wavestation", "drone"},
  {"Family 04 / 018 Seq Arp", 4, "Korg Wavestation", "arp"},
  {"Family 04 / 019 Wash Delay", 4, "Korg Wavestation", "fx"},
  {"Family 04 / 020 Metal Kit", 4, "Korg Wavestation", "kit"},
  {"Family 05 / 001 Stage Keys", 5, "Korg TR", "keys"},
  {"Family 05 / 002 Reed Piano", 5, "Korg TR", "electric piano"},
  {"Family 05 / 003 Funk Clav", 5, "Korg TR", "clav"},
  {"Family 05 / 004 Rock Organ", 5, "Korg TR", "organ"},
  {"Family 05 / 005 Ping Bells", 5, "Korg TR", "bells"},
  {"Family 05 / 006 Mute Pluck", 5, "Korg TR", "pluck"},
  {"Family 05 / 007 Short Stab", 5, "Korg TR", "stab"},
  {"Family 05 / 008 Round Bass", 5, "Korg TR", "bass"},
  {"Family 05 / 009 Saw Lead", 5, "Korg TR", "lead"},
  {"Family 05 / 010 Section Brass", 5, "Korg TR", "brass"},
  {"Family 05 / 011 Small Strings", 5, "Korg TR", "strings"},
  {"Family 05 / 012 Close Choir", 5, "Korg TR", "choir"},
  {"Family 05 / 013 Tape Pad", 5, "Korg TR", "pad"},
  {"Family 05 / 014 Filter Motion", 5, "Korg TR", "motion"},
  {"Family 05 / 015 Broken Texture", 5, "Korg TR", "texture"},
  {"Family 05 / 016 Dark Drone", 5, "Korg TR", "drone"},
  {"Family 05 / 017 Tight Arp", 5, "Korg TR", "arp"},
  {"Family 05 / 018 Room Crush", 5, "Korg TR", "fx"},
  {"Family 05 / 019 Dry Kit", 5, "Korg TR", "kit"},
  {"Family 05 / 020 Hard Kit", 5, "Korg TR", "kit"},
  {"Family 05 / 021 Tape Kit", 5, "Korg TR", "kit"},
};

const char* kArrangementPalette[] = {
  "Family 01 / 002 Sparse Keys",
  "Family 01 / 008 Key Stab",
  "Family 01 / 019 Heavy Delay",
  "Family 02 / 002 Thin Piano",
  "Family 02 / 007 Sparse Pluck",
  "Family 02 / 010 Sparse Bass",
  "Family 02 / 017 Skew Texture",
  "Family 02 / 020 Heavy Crush",
  "Family 03 / 003 Soul Piano",
  "Family 03 / 004 Clav Stab",
  "Family 04 / 014 Sparse Pad",
  "Family 04 / 016 Tilt Texture",
  "Family 05 / 019 Dry Kit",
  "Family 05 / 020 Hard Kit",
};

} // namespace

namespace keith
{
const PresetSpec* BakedPresets(int& count)
{
  count = kNumPresets;
  return kPresetSpecs;
}

int FirstPresetInFamily(int family)
{
  for (int i = 0; i < kNumPresets; ++i)
    if (kPresetSpecs[i].family == family)
      return i;

  return -1;
}

int FamilyForPreset(int presetIdx)
{
  if (presetIdx < 0 || presetIdx >= kNumPresets)
    return -1;

  return kPresetSpecs[presetIdx].family;
}
} // namespace keith

void Keith::SerializeDrumKit(IByteChunk& chunk, const keith::drums::KitParams& kit)
{
  const uint32_t magic = kDrumStateMagic;
  const uint32_t version = kDrumStateVersion;
  chunk.Put(&magic);
  chunk.Put(&version);
  chunk.Put(&kit.levelDb);

  for (const auto& slot : kit.slot)
  {
    chunk.Put(&slot.levelDb);
    chunk.Put(&slot.pan);
    chunk.Put(&slot.tuneSemitones);
    chunk.Put(&slot.decay);
    chunk.Put(&slot.filterHz);
    chunk.Put(&slot.drive);
    const uint8_t mute = slot.mute ? 1u : 0u;
    const uint8_t solo = slot.solo ? 1u : 0u;
    chunk.Put(&mute);
    chunk.Put(&solo);
  }
}

int Keith::UnserializeDrumKit(const IByteChunk& chunk, int startPos,
                             keith::drums::KitParams& kit)
{
  uint32_t magic = 0;
  uint32_t version = 0;
  int pos = chunk.Get(&magic, startPos);
  pos = pos >= 0 ? chunk.Get(&version, pos) : -1;
  if (pos < 0 || magic != kDrumStateMagic || version != kDrumStateVersion)
    return -1;

  keith::drums::KitParams restored = keith::drums::DefaultKitParams();
  pos = chunk.Get(&restored.levelDb, pos);
  for (int i = 0; i < keith::drums::kNumSlots && pos >= 0; ++i)
  {
    auto& slot = restored.slot[i];
    pos = chunk.Get(&slot.levelDb, pos);
    pos = pos >= 0 ? chunk.Get(&slot.pan, pos) : -1;
    pos = pos >= 0 ? chunk.Get(&slot.tuneSemitones, pos) : -1;
    pos = pos >= 0 ? chunk.Get(&slot.decay, pos) : -1;
    pos = pos >= 0 ? chunk.Get(&slot.filterHz, pos) : -1;
    pos = pos >= 0 ? chunk.Get(&slot.drive, pos) : -1;
    uint8_t mute = 0;
    uint8_t solo = 0;
    pos = pos >= 0 ? chunk.Get(&mute, pos) : -1;
    pos = pos >= 0 ? chunk.Get(&solo, pos) : -1;
    slot.mute = mute != 0;
    slot.solo = solo != 0;
  }

  if (pos < 0)
    return -1;

  kit = restored;
  return pos;
}

bool Keith::SerializeState(IByteChunk& chunk) const
{
  SerializeDrumKit(chunk, mDrumKit);
  return SerializeParams(chunk);
}

int Keith::UnserializeState(const IByteChunk& chunk, int startPos)
{
  keith::drums::KitParams restored;
  const int paramsPos = UnserializeDrumKit(chunk, startPos, restored);
  if (paramsPos >= 0)
  {
    mDrumKit = restored;
    return UnserializeParams(chunk, paramsPos);
  }

  // Pre-U11 chunks contain only the old 75 doubles. Preserve those values and supply
  // the new synthesized-drum defaults rather than rejecting the entire saved state.
  ResetDrumKitToDefaults();
  int pos = startPos;
  const int available = std::max(0, (chunk.Size() - startPos) / static_cast<int>(sizeof(double)));
  const int oldParamCount = std::min(available, kParamOutLevel + 1);
  for (int i = 0; i < oldParamCount && pos >= 0; ++i)
  {
    double value = 0.;
    pos = chunk.Get(&value, pos);
    if (pos >= 0)
      GetParam(i)->Set(value);
  }
  for (int i = oldParamCount; i < kNumParams; ++i)
    GetParam(i)->SetToDefault();

  OnParamReset(kPresetRecall);
  return pos;
}

void Keith::OnRestoreState()
{
  const int selected = std::clamp(GetParam(kParamDrumSlotSelect)->Int() - 1, 0,
                                  keith::drums::kNumSlots - 1);
  mSelectedDrumSlot.store(selected);
  LoadSelectedDrumSlotParams(selected);
#if IPLUG_DSP
  mDSP.SetDrumParams(mDrumKit);
#endif
  mDrumUiDirty.store(true);
  SendCurrentParamValuesFromDelegate();
}

void Keith::ResetDrumKitToDefaults()
{
  mDrumKit = keith::drums::DefaultKitParams();
  mSelectedDrumSlot.store(0);
}

void Keith::LoadSelectedDrumSlotParams(int slot)
{
  const int safeSlot = std::clamp(slot, 0, keith::drums::kNumSlots - 1);
  const auto& values = mDrumKit.slot[safeSlot];
  mSelectedDrumSlot.store(safeSlot);
  GetParam(kParamDrumSlotLevel)->Set(values.levelDb);
  GetParam(kParamDrumSlotPan)->Set(values.pan * 100.f);
  GetParam(kParamDrumSlotTune)->Set(values.tuneSemitones);
  GetParam(kParamDrumSlotDecay)->Set(values.decay * 100.f);
  GetParam(kParamDrumSlotFilter)->Set(values.filterHz);
  GetParam(kParamDrumSlotDrive)->Set(values.drive * 100.f);
  GetParam(kParamDrumSlotMute)->Set(values.mute ? 1. : 0.);
  GetParam(kParamDrumSlotSolo)->Set(values.solo ? 1. : 0.);
  GetParam(kParamDrumKitLevel)->Set(mDrumKit.levelDb);
}

void Keith::StoreSelectedDrumSlotParam(int paramIdx)
{
  auto& slot = mDrumKit.slot[mSelectedDrumSlot.load()];
  switch (paramIdx)
  {
    case kParamDrumSlotLevel: slot.levelDb = static_cast<float>(GetParam(paramIdx)->Value()); break;
    case kParamDrumSlotPan: slot.pan = static_cast<float>(GetParam(paramIdx)->Value() / 100.); break;
    case kParamDrumSlotTune: slot.tuneSemitones = static_cast<float>(GetParam(paramIdx)->Value()); break;
    case kParamDrumSlotDecay: slot.decay = static_cast<float>(GetParam(paramIdx)->Value() / 100.); break;
    case kParamDrumSlotFilter: slot.filterHz = static_cast<float>(GetParam(paramIdx)->Value()); break;
    case kParamDrumSlotDrive: slot.drive = static_cast<float>(GetParam(paramIdx)->Value() / 100.); break;
    case kParamDrumSlotMute: slot.mute = GetParam(paramIdx)->Bool(); break;
    case kParamDrumSlotSolo: slot.solo = GetParam(paramIdx)->Bool(); break;
    default: break;
  }
}

void Keith::UpdateDrumUi()
{
  if (!mDrumUiDirty.exchange(false))
    return;

  for (int param = kParamDrumSlotSelect; param <= kParamDrumKitLevel; ++param)
    SendParameterValueFromDelegate(param, GetParam(param)->GetNormalized(), true);

#if IPLUG_EDITOR
  if (IGraphics* graphics = GetUI())
    graphics->SetAllControlsDirty();
#endif
}

void Keith::AddFactoryPresets()
{
  double defaults[kNumParams];

  for (int i = 0; i < kNumParams; ++i)
    defaults[i] = GetParam(i)->Value();

  double values[kNumParams];

  for (int preset = 0; preset < kNumPresets; ++preset)
  {
    for (int i = 0; i < kNumParams; ++i)
      values[i] = defaults[i];

    for (const PresetDelta& delta : kPresetDeltas)
      if (delta.preset == preset)
        values[delta.param] = delta.value;

    // Drum-slot deltas are host parameters, and OnRestoreState reloads the selected
    // slot from the kit chunk. Write those deltas into the kit so they survive recall.
    keith::drums::KitParams kit = keith::drums::DefaultKitParams();
    const int selected = std::clamp(static_cast<int>(values[kParamDrumSlotSelect] + 0.5) - 1, 0,
                                    keith::drums::kNumSlots - 1);
    auto& drumSlot = kit.slot[selected];
    drumSlot.levelDb = static_cast<float>(values[kParamDrumSlotLevel]);
    drumSlot.pan = static_cast<float>(values[kParamDrumSlotPan] / 100.);
    drumSlot.tuneSemitones = static_cast<float>(values[kParamDrumSlotTune]);
    drumSlot.decay = static_cast<float>(values[kParamDrumSlotDecay] / 100.);
    drumSlot.filterHz = static_cast<float>(values[kParamDrumSlotFilter]);
    drumSlot.drive = static_cast<float>(values[kParamDrumSlotDrive] / 100.);
    drumSlot.mute = values[kParamDrumSlotMute] >= 0.5;
    drumSlot.solo = values[kParamDrumSlotSolo] >= 0.5;
    kit.levelDb = static_cast<float>(values[kParamDrumKitLevel]);

    // Factory chunks use the same custom-state layout as host save/restore.
    IByteChunk chunk;
    Keith::SerializeDrumKit(chunk, kit);
    for (int i = 0; i < kNumParams; ++i)
      chunk.Put(&values[i]);

    MakePresetFromChunk(kPresetSpecs[preset].name, chunk);

    // Preset index 0 is what the editor shows on open, so the init state must be that preset.
    if (preset == 0)
      for (int i = 0; i < kNumParams; ++i)
        GetParam(i)->Set(values[i]);
  }
}

static_assert(kParamFilterType == 25 && kParamFilterCutoff == 26 && kParamFilterReso == 27,
              "KeithFilterResponse is bound to these host ids");
static_assert(kParamOscSync == 43 && kParamMonoLegato == 55,
              "U9 appends 43..55");
static_assert(kParamIns1Algo == 56 && kParamOutLevel == 74,
              "U10 appends 56..74");
static_assert(kParamDrumSlotSelect == 75 && kParamDrumKitLevel == 84,
              "U11 appends 75..84");
static_assert(kParamLfo2Rate == 85 && kParamLfo2Amount == 88,
              "U13 appends 85..88");
static_assert(kParamMod1Source == 89 && kParamMod4Amount == 100,
              "U15 appends 89..100");
static_assert(kParamNoiseKey == 101 && kParamFilterKeyCurve == 110 && kNumParams == 111,
              "U17 appends 101..110");

Keith::Keith(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  // --- U1/U4 preserved IDs ---
  GetParam(kParamGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");

  GetParam(kParamTapeSat)->InitDouble("FX Tape Saturation", 25., 0., 100., 0.1, "%");
  GetParam(kParamTapeBump)->InitDouble("FX Tape Head Bump", 20., 0., 100., 0.1, "%");
  GetParam(kParamTapeWow)->InitDouble("FX Tape Wow", 10., 0., 100., 0.1, "%");
  GetParam(kParamTapeFlutter)->InitDouble("FX Tape Flutter", 10., 0., 100., 0.1, "%");
  GetParam(kParamTapeHiss)->InitDouble("FX Tape Hiss", 5., 0., 100., 0.1, "%");
  GetParam(kParamTapePrint)->InitDouble("FX Tape Print", 0., 0., 100., 0.1, "%");
  GetParam(kParamTapeMix)->InitDouble("FX Tape Mix", 40., 0., 100., 0.1, "%");
  GetParam(kParamTapeBypass)->InitBool("FX Tape Bypass", false);

  // --- Synth surface (U7) ---
  GetParam(kParamEngine)->InitEnum("Synth Engine", 0, {"Hybrid", "Analog"});
  GetParam(kParamPolyMode)->InitEnum("Synth Poly Mode", 0, {"Poly", "Mono"});
  GetParam(kParamPorta)->InitDouble("Synth Porta", 0., 0., 1000., 0.1, "ms");
  GetParam(kParamUnisonVoices)->InitInt("Synth Unison Voices", 1, 1, 8);
  GetParam(kParamUnisonDetune)->InitDouble("Synth Unison Detune", 25., 0., 100., 0.1, "%");

  GetParam(kParamOsc1Shape)->InitDouble("Synth Osc1 Shape", 0., 0., 100., 0.1, "%");
  GetParam(kParamOsc1Pitch)->InitDouble("Synth Osc1 Pitch", 0., -24., 24., 0.01, "st");
  GetParam(kParamOsc1Level)->InitDouble("Synth Osc1 Level", 100., 0., 100., 0.1, "%");
  GetParam(kParamOsc1PW)->InitDouble("Synth Osc1 PW", 50., 5., 95., 0.1, "%");

  GetParam(kParamOsc2Shape)->InitDouble("Synth Osc2 Shape", 0., 0., 100., 0.1, "%");
  GetParam(kParamOsc2Pitch)->InitDouble("Synth Osc2 Pitch", 0., -24., 24., 0.01, "st");
  GetParam(kParamOsc2Level)->InitDouble("Synth Osc2 Level", 80., 0., 100., 0.1, "%");
  GetParam(kParamOsc2PW)->InitDouble("Synth Osc2 PW", 50., 5., 95., 0.1, "%");

  GetParam(kParamSubLevel)->InitDouble("Synth Sub Level", 0., 0., 100., 0.1, "%");
  GetParam(kParamNoiseLevel)->InitDouble("Synth Noise Level", 0., 0., 100., 0.1, "%");
  GetParam(kParamFmAmount)->InitDouble("Synth FM Amount", 0., 0., 100., 0.1, "%");

  GetParam(kParamFilterType)->InitEnum("Synth Filter Type", 0, {"LP24", "LP12", "BP", "HP"});
  GetParam(kParamFilterCutoff)->InitDouble("Synth Filter Cutoff", 8000., 20., 20000., 0.1, "Hz",
                                           IParam::kFlagsNone, "", IParam::ShapeExp(), IParam::kUnitFrequency);
  GetParam(kParamFilterReso)->InitDouble("Synth Filter Reso", 10., 0., 100., 0.1, "%");
  GetParam(kParamFilterEnv)->InitDouble("Synth Filter Env", 40., -100., 100., 0.1, "%");

  GetParam(kParamAmpAttack)->InitDouble("Synth Amp Attack", 5., 0.1, 10000., 0.01, "ms",
                                        IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamAmpDecay)->InitDouble("Synth Amp Decay", 200., 1., 30000., 0.01, "ms",
                                       IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamAmpSustain)->InitDouble("Synth Amp Sustain", 70., 0., 100., 0.1, "%");
  GetParam(kParamAmpRelease)->InitDouble("Synth Amp Release", 300., 1., 10000., 0.01, "ms",
                                         IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));

  GetParam(kParamPitchAttack)->InitDouble("Synth Pitch Attack", 10., 0.1, 10000., 0.01, "ms",
                                          IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamPitchDecay)->InitDouble("Synth Pitch Decay", 250., 1., 10000., 0.01, "ms",
                                         IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamPitchSustain)->InitDouble("Synth Pitch Sustain", 0., 0., 100., 0.1, "%");
  GetParam(kParamPitchRelease)->InitDouble("Synth Pitch Release", 300., 1., 10000., 0.01, "ms",
                                           IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamPitchAmount)->InitDouble("Synth Pitch Amount", 0., -24., 24., 0.01, "st");

  GetParam(kParamSynthMasterLevel)->InitDouble("Synth Master Level", 0., -60., 6., 0.01, "dB");

  GetParam(kParamFiltAttack)->InitDouble("Synth Filt Attack", 10., 0.1, 10000., 0.01, "ms",
                                         IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamFiltDecay)->InitDouble("Synth Filt Decay", 250., 1., 10000., 0.01, "ms",
                                        IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));
  GetParam(kParamFiltSustain)->InitDouble("Synth Filt Sustain", 40., 0., 100., 0.1, "%");
  GetParam(kParamFiltRelease)->InitDouble("Synth Filt Release", 300., 1., 10000., 0.01, "ms",
                                          IParam::kFlagsNone, "", IParam::ShapePowCurve(3.));

  // U9. Defaults are inert: sync/ring off, depths at 0, mod destinations off, legato retriggers.
  GetParam(kParamOscSync)->InitEnum("Synth Osc Sync", 0, {"Free", "Sync"});
  GetParam(kParamOscRing)->InitEnum("Synth Osc Ring", 0, {"Off", "On"});
  GetParam(kParamFilterKeyTrack)->InitDouble("Synth Filter Key Track", 0., 0., 200., 0.1, "%");
  GetParam(kParamFilterDrive)->InitDouble("Synth Filter Drive", 0., 0., 100., 0.1, "%");
  GetParam(kParamLfoRate)->InitDouble("Synth LFO Rate", 1., 0.01, 30., 0.01, "Hz",
                                      IParam::kFlagsNone, "", IParam::ShapeExp());
  GetParam(kParamLfoShape)->InitEnum("Synth LFO Shape", 0, {"Sine", "Tri", "Saw", "Square", "S&H"});
  GetParam(kParamLfoDest)->InitEnum("Synth LFO Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamLfoAmount)->InitDouble("Synth LFO Amount", 0., -100., 100., 0.1, "%");
  // U13: a second, independent LFO. Same shape/dest/amount surface as LFO 1.
  GetParam(kParamLfo2Rate)->InitDouble("Synth LFO2 Rate", 1., 0.01, 30., 0.01, "Hz",
                                      IParam::kFlagsNone, "", IParam::ShapeExp());
  GetParam(kParamLfo2Shape)->InitEnum("Synth LFO2 Shape", 0, {"Sine", "Tri", "Saw", "Square", "S&H"});
  GetParam(kParamLfo2Dest)->InitEnum("Synth LFO2 Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamLfo2Amount)->InitDouble("Synth LFO2 Amount", 0., -100., 100., 0.1, "%");
  GetParam(kParamModWheelDest)->InitEnum("Synth Mod Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamModWheelAmount)->InitDouble("Synth Mod Amount", 0., -100., 100., 0.1, "%");
  GetParam(kParamVelToAmp)->InitDouble("Synth Vel To Amp", 0., 0., 100., 0.1, "%");
  GetParam(kParamVelToFilter)->InitDouble("Synth Vel To Filter", 0., 0., 100., 0.1, "%");
  GetParam(kParamMonoLegato)->InitEnum("Synth Mono Legato", 0, {"Retrig", "Legato"});

  // U10. Off is the inert default. A preset delta turns a stage on.
  GetParam(kParamIns1Algo)->InitEnum("FX Insert1 Algo", 0, {"Off", "Drive", "Chorus", "Delay", "Crush"});
  GetParam(kParamIns1A)->InitDouble("FX Insert1 A", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns1B)->InitDouble("FX Insert1 B", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns1C)->InitDouble("FX Insert1 C", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns1Mix)->InitDouble("FX Insert1 Mix", 100., 0., 100., 0.1, "%");
  GetParam(kParamIns1Bypass)->InitEnum("FX Insert1 Bypass", 0, {"Active", "Bypass"});

  GetParam(kParamIns2Algo)->InitEnum("FX Insert2 Algo", 0, {"Off", "Drive", "Chorus", "Delay", "Crush"});
  GetParam(kParamIns2A)->InitDouble("FX Insert2 A", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns2B)->InitDouble("FX Insert2 B", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns2C)->InitDouble("FX Insert2 C", 50., 0., 100., 0.1, "%");
  GetParam(kParamIns2Mix)->InitDouble("FX Insert2 Mix", 100., 0., 100., 0.1, "%");
  GetParam(kParamIns2Bypass)->InitEnum("FX Insert2 Bypass", 0, {"Active", "Bypass"});

  GetParam(kParamMstAlgo)->InitEnum("FX Master Algo", 0, {"Off", "Comp", "Limit", "Width", "Tilt"});
  GetParam(kParamMstA)->InitDouble("FX Master A", 50., 0., 100., 0.1, "%");
  GetParam(kParamMstB)->InitDouble("FX Master B", 50., 0., 100., 0.1, "%");
  GetParam(kParamMstC)->InitDouble("FX Master C", 50., 0., 100., 0.1, "%");
  GetParam(kParamMstMix)->InitDouble("FX Master Mix", 40., 0., 100., 0.1, "%");
  GetParam(kParamMstBypass)->InitEnum("FX Master Bypass", 0, {"Active", "Bypass"});
  GetParam(kParamOutLevel)->InitDouble("FX Out Level", 0., -60., 12., 0.01, "dB");
  GetParam(kParamOutLevel)->SetDisplayText(-60., "-inf");

  // U11. These controls edit the selected slot. The 16 complete slot records live in state chunks.
  GetParam(kParamDrumSlotSelect)->InitInt("Drum Slot Select", 1, 1, 16);
  GetParam(kParamDrumSlotLevel)->InitDouble("Drum Slot Level", 0., -60., 6., 0.01, "dB");
  GetParam(kParamDrumSlotLevel)->SetDisplayText(-60., "-inf");
  GetParam(kParamDrumSlotPan)->InitDouble("Drum Slot Pan", 0., -100., 100., 0.1, "%");
  GetParam(kParamDrumSlotTune)->InitDouble("Drum Slot Tune", 0., -12., 12., 0.01, "st");
  GetParam(kParamDrumSlotDecay)->InitDouble("Drum Slot Decay", 70., 0., 100., 0.1, "%");
  GetParam(kParamDrumSlotFilter)->InitDouble("Drum Slot Filter", 12000., 20., 20000., 0.1, "Hz",
                                             IParam::kFlagsNone, "", IParam::ShapeExp(),
                                             IParam::kUnitFrequency);
  GetParam(kParamDrumSlotDrive)->InitDouble("Drum Slot Drive", 0., 0., 100., 0.1, "%");
  GetParam(kParamDrumSlotMute)->InitBool("Drum Slot Mute", false);
  GetParam(kParamDrumSlotSolo)->InitBool("Drum Slot Solo", false);
  GetParam(kParamDrumKitLevel)->InitDouble("Drum Kit Level", 0., -60., 6., 0.01, "dB");
  GetParam(kParamDrumKitLevel)->SetDisplayText(-60., "-inf");

  // U15. Off sources, Off destinations and zero amounts make every new route inert.
  GetParam(kParamMod1Source)->InitEnum(
    "Mod1 Src", 0, {"Off", "LFO 1", "LFO 2", "Mod Wheel", "Velocity", "Key"});
  GetParam(kParamMod1Dest)->InitEnum(
    "Mod1 Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamMod1Amount)->InitDouble("Mod1 Amt", 0., -100., 100., 0.1, "%");
  GetParam(kParamMod2Source)->InitEnum(
    "Mod2 Src", 0, {"Off", "LFO 1", "LFO 2", "Mod Wheel", "Velocity", "Key"});
  GetParam(kParamMod2Dest)->InitEnum(
    "Mod2 Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamMod2Amount)->InitDouble("Mod2 Amt", 0., -100., 100., 0.1, "%");
  GetParam(kParamMod3Source)->InitEnum(
    "Mod3 Src", 0, {"Off", "LFO 1", "LFO 2", "Mod Wheel", "Velocity", "Key"});
  GetParam(kParamMod3Dest)->InitEnum(
    "Mod3 Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamMod3Amount)->InitDouble("Mod3 Amt", 0., -100., 100., 0.1, "%");
  GetParam(kParamMod4Source)->InitEnum(
    "Mod4 Src", 0, {"Off", "LFO 1", "LFO 2", "Mod Wheel", "Velocity", "Key"});
  GetParam(kParamMod4Dest)->InitEnum(
    "Mod4 Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level", "FM"});
  GetParam(kParamMod4Amount)->InitDouble("Mod4 Amt", 0., -100., 100., 0.1, "%");

  // U17. Defaults are inert: no extra key scale, no stereo offset, no direct path.
  // Key and curve knobs are octaves of change per octave of pitch, scaled so ±100% is ±2.
  GetParam(kParamNoiseKey)->InitDouble("Synth Noise Key", 0., -100., 100., 0.1, "%");
  GetParam(kParamAmpAttackKey)->InitDouble("Synth Amp Attack Key", 0., -100., 100., 0.1, "%");
  GetParam(kParamAmpAttackCurve)->InitDouble("Synth Amp Attack Curve", 0., -100., 100., 0.1, "%");
  GetParam(kParamAmpDecayKey)->InitDouble("Synth Amp Decay Key", 0., -100., 100., 0.1, "%");
  GetParam(kParamAmpDecayCurve)->InitDouble("Synth Amp Decay Curve", 0., -100., 100., 0.1, "%");
  GetParam(kParamStereoDelay)->InitDouble("Synth Stereo Delay", 0., 0., 40., 0.01, "ms");
  GetParam(kParamStereoDelayKey)->InitDouble("Synth Stereo Delay Key", 0., -100., 100., 0.1, "%");
  GetParam(kParamStereoSpread)->InitDouble("Synth Stereo Spread", 0., -100., 100., 0.1, "%");
  GetParam(kParamDirectLevel)->InitDouble("Synth Direct Level", 0., 0., 100., 0.1, "%");
  GetParam(kParamFilterKeyCurve)->InitDouble("Synth Filter Key Curve", 0., -100., 100., 0.1, "%");

  // Baked factory presets, grouped Family 01-05, neutral names only.
  AddFactoryPresets();

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [this](IGraphics* g) { keith::ui::BuildEditorial(*this, g); };
#endif
}

#if IPLUG_EDITOR
void Keith::OnIdle()
{
  UpdateDrumUi();

  IGraphics* pGraphics = GetUI();

  if (!pGraphics)
    return;

  const int current = GetCurrentPresetIdx();
  if (current != mLastPreset)
  {
    mLastPreset = current;
    pGraphics->SetAllControlsDirty();
  }

  const int fxAlgo[3] = {
    GetParam(kParamIns1Algo)->Int(),
    GetParam(kParamIns2Algo)->Int(),
    GetParam(kParamMstAlgo)->Int()
  };
  for (int i = 0; i < 3; ++i)
  {
    if (fxAlgo[i] != mLastFxAlgo[i])
    {
      mLastFxAlgo[i] = fxAlgo[i];
      pGraphics->SetAllControlsDirty();
    }
  }
}
#endif

#if IPLUG_DSP
void Keith::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  mDSP.ProcessBlock(outputs, 2, nFrames);
}

void Keith::OnReset()
{
  mDSP.Reset(GetSampleRate(), GetBlockSize());
  SyncTapeParams();
  SyncSynthParams();
  SyncFxParams();
  SyncDrumParams();
  mDSP.SetGain(GetParam(kParamGain)->Value());
}

void Keith::ProcessMidiMsg(const IMidiMsg& msg)
{
  mDSP.ProcessMidiMsg(msg);
}

void Keith::OnParamChange(int paramIdx)
{
  if (paramIdx == kParamGain)
  {
    mDSP.SetGain(GetParam(kParamGain)->Value());
    return;
  }

  if (paramIdx >= kParamTapeSat && paramIdx <= kParamTapeBypass)
  {
    SyncTapeParams();
    return;
  }

  if (paramIdx >= kParamIns1Algo && paramIdx <= kParamOutLevel)
  {
    SyncFxParams();
    return;
  }

  if (paramIdx == kParamDrumSlotSelect)
  {
    const int slot = std::clamp(GetParam(paramIdx)->Int() - 1, 0,
                                keith::drums::kNumSlots - 1);
    LoadSelectedDrumSlotParams(slot);
    SyncDrumParams();
    mDrumUiDirty.store(true);
    return;
  }

  if (paramIdx >= kParamDrumSlotLevel && paramIdx <= kParamDrumSlotSolo)
  {
    StoreSelectedDrumSlotParam(paramIdx);
    SyncDrumParams();
    if (paramIdx == kParamDrumSlotMute || paramIdx == kParamDrumSlotSolo)
      mDrumUiDirty.store(true);
    return;
  }

  if (paramIdx == kParamDrumKitLevel)
  {
    mDrumKit.levelDb = static_cast<float>(GetParam(paramIdx)->Value());
    SyncDrumParams();
    return;
  }

  SyncSynthParams();
}

void Keith::SyncTapeParams()
{
  KeithDSP<sample>::TapeParams global {};
  global.saturation = static_cast<sample>(GetParam(kParamTapeSat)->Value() / 100.);
  global.headBump = static_cast<sample>(GetParam(kParamTapeBump)->Value() / 100.);
  global.wow = static_cast<sample>(GetParam(kParamTapeWow)->Value() / 100.);
  global.flutter = static_cast<sample>(GetParam(kParamTapeFlutter)->Value() / 100.);
  global.hiss = static_cast<sample>(GetParam(kParamTapeHiss)->Value() / 100.);
  global.printThrough = static_cast<sample>(GetParam(kParamTapePrint)->Value() / 100.);
  global.mix = static_cast<sample>(GetParam(kParamTapeMix)->Value() / 100.);
  global.bypass = GetParam(kParamTapeBypass)->Bool();

  mDSP.SetGlobalTapeParams(global);

  KeithDSP<sample>::TapeParams partBypass {};
  partBypass.bypass = true;
  partBypass.mix = 0.f;
  for (int part = 0; part < 4; ++part)
    mDSP.SetPartTapeParams(part, partBypass);
}

void Keith::SyncSynthParams()
{
  keith::synth::Params p {};
  p.engine = static_cast<keith::synth::EngineMode>(GetParam(kParamEngine)->Int());
  p.polyMode = static_cast<keith::synth::PolyMode>(GetParam(kParamPolyMode)->Int());
  p.portaMs = static_cast<float>(GetParam(kParamPorta)->Value());
  p.unisonVoices = GetParam(kParamUnisonVoices)->Int();
  p.unisonDetune = static_cast<float>(GetParam(kParamUnisonDetune)->Value() / 100.);

  p.osc1Shape = static_cast<float>(GetParam(kParamOsc1Shape)->Value() / 100.);
  p.osc1Pitch = static_cast<float>(GetParam(kParamOsc1Pitch)->Value());
  p.osc1Level = static_cast<float>(GetParam(kParamOsc1Level)->Value() / 100.);
  p.osc1PW = static_cast<float>(GetParam(kParamOsc1PW)->Value() / 100.);

  p.osc2Shape = static_cast<float>(GetParam(kParamOsc2Shape)->Value() / 100.);
  p.osc2Pitch = static_cast<float>(GetParam(kParamOsc2Pitch)->Value());
  p.osc2Level = static_cast<float>(GetParam(kParamOsc2Level)->Value() / 100.);
  p.osc2PW = static_cast<float>(GetParam(kParamOsc2PW)->Value() / 100.);

  p.subLevel = static_cast<float>(GetParam(kParamSubLevel)->Value() / 100.);
  p.noiseLevel = static_cast<float>(GetParam(kParamNoiseLevel)->Value() / 100.);
  p.fmAmount = static_cast<float>(GetParam(kParamFmAmount)->Value() / 100.);

  p.filterType = static_cast<keith::synth::FilterType>(GetParam(kParamFilterType)->Int());
  p.filterCutoffHz = static_cast<float>(GetParam(kParamFilterCutoff)->Value());
  p.filterReso = static_cast<float>(GetParam(kParamFilterReso)->Value() / 100.);
  p.filterEnvAmt = static_cast<float>(GetParam(kParamFilterEnv)->Value() / 100.);

  p.ampAttackMs = static_cast<float>(GetParam(kParamAmpAttack)->Value());
  p.ampDecayMs = static_cast<float>(GetParam(kParamAmpDecay)->Value());
  p.ampSustain = static_cast<float>(GetParam(kParamAmpSustain)->Value() / 100.);
  p.ampReleaseMs = static_cast<float>(GetParam(kParamAmpRelease)->Value());

  p.pitchAttackMs = static_cast<float>(GetParam(kParamPitchAttack)->Value());
  p.pitchDecayMs = static_cast<float>(GetParam(kParamPitchDecay)->Value());
  p.pitchSustain = static_cast<float>(GetParam(kParamPitchSustain)->Value() / 100.);
  p.pitchReleaseMs = static_cast<float>(GetParam(kParamPitchRelease)->Value());
  p.pitchAmount = static_cast<float>(GetParam(kParamPitchAmount)->Value());

  p.filtAttackMs = static_cast<float>(GetParam(kParamFiltAttack)->Value());
  p.filtDecayMs = static_cast<float>(GetParam(kParamFiltDecay)->Value());
  p.filtSustain = static_cast<float>(GetParam(kParamFiltSustain)->Value() / 100.);
  p.filtReleaseMs = static_cast<float>(GetParam(kParamFiltRelease)->Value());

  p.masterLevel = keith::synth::DbToLinear(static_cast<float>(GetParam(kParamSynthMasterLevel)->Value()));

  p.oscSync = GetParam(kParamOscSync)->Int() != 0;
  p.oscRing = GetParam(kParamOscRing)->Int() != 0;
  p.filterKeyTrack = static_cast<float>(GetParam(kParamFilterKeyTrack)->Value() / 100.);
  p.filterDrive = static_cast<float>(GetParam(kParamFilterDrive)->Value() / 100.);
  p.lfoRateHz = static_cast<float>(GetParam(kParamLfoRate)->Value());
  p.lfoShape = static_cast<keith::synth::LfoShape>(GetParam(kParamLfoShape)->Int());
  p.lfoDest = static_cast<keith::synth::ModDest>(GetParam(kParamLfoDest)->Int());
  p.lfoAmount = static_cast<float>(GetParam(kParamLfoAmount)->Value() / 100.);
  p.lfo2RateHz = static_cast<float>(GetParam(kParamLfo2Rate)->Value());
  p.lfo2Shape = static_cast<keith::synth::LfoShape>(GetParam(kParamLfo2Shape)->Int());
  p.lfo2Dest = static_cast<keith::synth::ModDest>(GetParam(kParamLfo2Dest)->Int());
  p.lfo2Amount = static_cast<float>(GetParam(kParamLfo2Amount)->Value() / 100.);
  p.modWheelDest = static_cast<keith::synth::ModDest>(GetParam(kParamModWheelDest)->Int());
  p.modWheelAmount = static_cast<float>(GetParam(kParamModWheelAmount)->Value() / 100.);
  p.velToAmp = static_cast<float>(GetParam(kParamVelToAmp)->Value() / 100.);
  p.velToFilter = static_cast<float>(GetParam(kParamVelToFilter)->Value() / 100.);
  p.monoLegato = GetParam(kParamMonoLegato)->Int() != 0;

  constexpr int modSources[keith::synth::kNumModSlots] = {
    kParamMod1Source, kParamMod2Source, kParamMod3Source, kParamMod4Source
  };
  constexpr int modDestinations[keith::synth::kNumModSlots] = {
    kParamMod1Dest, kParamMod2Dest, kParamMod3Dest, kParamMod4Dest
  };
  constexpr int modAmounts[keith::synth::kNumModSlots] = {
    kParamMod1Amount, kParamMod2Amount, kParamMod3Amount, kParamMod4Amount
  };
  for (int slot = 0; slot < keith::synth::kNumModSlots; ++slot)
  {
    auto& route = p.modMatrix[slot];
    route.source = static_cast<keith::synth::ModSource>(GetParam(modSources[slot])->Int());
    route.destination =
      static_cast<keith::synth::ModDest>(GetParam(modDestinations[slot])->Int());
    route.amount = static_cast<float>(GetParam(modAmounts[slot])->Value() / 100.);
  }

  constexpr float kKeyScale = 0.02f; // ±100% -> ±2 octaves of change per octave of pitch
  p.noiseKey = static_cast<float>(GetParam(kParamNoiseKey)->Value() * kKeyScale);
  p.ampAttackKey = static_cast<float>(GetParam(kParamAmpAttackKey)->Value() * kKeyScale);
  p.ampAttackCurve = static_cast<float>(GetParam(kParamAmpAttackCurve)->Value() * kKeyScale);
  p.ampDecayKey = static_cast<float>(GetParam(kParamAmpDecayKey)->Value() * kKeyScale);
  p.ampDecayCurve = static_cast<float>(GetParam(kParamAmpDecayCurve)->Value() * kKeyScale);
  p.stereoDelayMs = static_cast<float>(GetParam(kParamStereoDelay)->Value());
  p.stereoDelayKey = static_cast<float>(GetParam(kParamStereoDelayKey)->Value() * kKeyScale);
  p.stereoSpread = static_cast<float>(GetParam(kParamStereoSpread)->Value() / 100.);
  p.directLevel = static_cast<float>(GetParam(kParamDirectLevel)->Value() / 100.);
  p.filterKeyCurve = static_cast<float>(GetParam(kParamFilterKeyCurve)->Value() * kKeyScale);

  mDSP.SetSynthParams(p);
}

void Keith::SyncFxParams()
{
  KeithDSP<sample>::InsertParams insert1 {};
  insert1.algorithm = static_cast<keith::fx::InsertAlgorithm>(GetParam(kParamIns1Algo)->Int());
  insert1.a = static_cast<sample>(GetParam(kParamIns1A)->Value() / 100.);
  insert1.b = static_cast<sample>(GetParam(kParamIns1B)->Value() / 100.);
  insert1.c = static_cast<sample>(GetParam(kParamIns1C)->Value() / 100.);
  insert1.mix = static_cast<sample>(GetParam(kParamIns1Mix)->Value() / 100.);
  insert1.bypass = GetParam(kParamIns1Bypass)->Int() != 0;
  mDSP.SetInsert1Params(insert1);

  KeithDSP<sample>::InsertParams insert2 {};
  insert2.algorithm = static_cast<keith::fx::InsertAlgorithm>(GetParam(kParamIns2Algo)->Int());
  insert2.a = static_cast<sample>(GetParam(kParamIns2A)->Value() / 100.);
  insert2.b = static_cast<sample>(GetParam(kParamIns2B)->Value() / 100.);
  insert2.c = static_cast<sample>(GetParam(kParamIns2C)->Value() / 100.);
  insert2.mix = static_cast<sample>(GetParam(kParamIns2Mix)->Value() / 100.);
  insert2.bypass = GetParam(kParamIns2Bypass)->Int() != 0;
  mDSP.SetInsert2Params(insert2);

  KeithDSP<sample>::MasterParams master {};
  master.algorithm = static_cast<keith::fx::MasterAlgorithm>(GetParam(kParamMstAlgo)->Int());
  master.a = static_cast<sample>(GetParam(kParamMstA)->Value() / 100.);
  master.b = static_cast<sample>(GetParam(kParamMstB)->Value() / 100.);
  master.c = static_cast<sample>(GetParam(kParamMstC)->Value() / 100.);
  master.mix = static_cast<sample>(GetParam(kParamMstMix)->Value() / 100.);
  master.bypass = GetParam(kParamMstBypass)->Int() != 0;
  mDSP.SetMasterFxParams(master);

  const double outDb = GetParam(kParamOutLevel)->Value();
  mDSP.SetOutLevel(keith::synth::DbToLinear(static_cast<float>(outDb)));
}

void Keith::SyncDrumParams()
{
  mDSP.SetDrumParams(mDrumKit);
}
#endif
