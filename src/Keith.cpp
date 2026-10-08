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

  // Family 02 / 001 Neon Keys
  {20, kParamEngine, 1},      {20, kParamOsc1Shape, 36},  {20, kParamOsc1Level, 88},
  {20, kParamOsc2Shape, 58},  {20, kParamOsc2Pitch, 12},  {20, kParamOsc2Level, 62},
  {20, kParamFilterCutoff, 7800}, {20, kParamFilterReso, 16}, {20, kParamFilterEnv, 28},
  {20, kParamFilterDrive, 24}, {20, kParamAmpAttack, 4},   {20, kParamAmpDecay, 700},
  {20, kParamAmpSustain, 42}, {20, kParamAmpRelease, 420}, {20, kParamUnisonVoices, 2},
  {20, kParamUnisonDetune, 26}, {20, kParamVelToAmp, 55},   {20, kParamTapeSat, 38},
  {20, kParamTapeMix, 22},
  {20, kParamIns1Algo, 1},    {20, kParamIns1A, 48},      {20, kParamIns1B, 42},
  {20, kParamIns1C, 38},      {20, kParamIns1Mix, 30},    {20, kParamMstAlgo, 1},
  {20, kParamMstA, 56},       {20, kParamMstB, 36},       {20, kParamMstC, 32},
  {20, kParamMstMix, 26},

  // Family 02 / 002 Thin Piano
  {21, kParamEngine, 1},      {21, kParamOsc1Shape, 16},  {21, kParamOsc2Shape, 28},
  {21, kParamOsc2Pitch, 12},  {21, kParamOsc2Level, 18},  {21, kParamFmAmount, 8},
  {21, kParamFilterCutoff, 2600}, {21, kParamFilterReso, 8},  {21, kParamFilterEnv, 18},
  {21, kParamAmpAttack, 4},   {21, kParamAmpDecay, 700},  {21, kParamAmpSustain, 22},
  {21, kParamAmpRelease, 480}, {21, kParamVelToAmp, 62},   {21, kParamVelToFilter, 20},
  {21, kParamTapeSat, 14},    {21, kParamTapeMix, 16},    {21, kParamSynthMasterLevel, -2},
  {21, kParamIns1Algo, 2},    {21, kParamIns1A, 12},      {21, kParamIns1B, 18},
  {21, kParamIns1C, 35},      {21, kParamIns1Mix, 12},    {21, kParamMstAlgo, 4},
  {21, kParamMstA, 58},       {21, kParamMstB, 46},       {21, kParamMstC, 10},
  {21, kParamMstMix, 16},

  // Family 02 / 003 Thin Clav
  {22, kParamEngine, 1},      {22, kParamOsc1Shape, 82},  {22, kParamOsc1PW, 18},
  {22, kParamOsc2Shape, 94},  {22, kParamOsc2Pitch, 12},  {22, kParamOsc2Level, 28},
  {22, kParamOsc2PW, 16},     {22, kParamFilterCutoff, 4100}, {22, kParamFilterReso, 38},
  {22, kParamFilterEnv, 70},  {22, kParamFilterDrive, 30}, {22, kParamFilterKeyTrack, 48},
  {22, kParamFiltAttack, 1},  {22, kParamFiltDecay, 120}, {22, kParamFiltSustain, 0},
  {22, kParamFiltRelease, 100}, {22, kParamAmpAttack, 1},   {22, kParamAmpDecay, 180},
  {22, kParamAmpSustain, 0},  {22, kParamAmpRelease, 130}, {22, kParamVelToAmp, 70},
  {22, kParamIns1Algo, 1},    {22, kParamIns1A, 55},      {22, kParamIns1B, 40},
  {22, kParamIns1C, 32},      {22, kParamIns1Mix, 34},    {22, kParamMstAlgo, 1},
  {22, kParamMstA, 54},       {22, kParamMstB, 46},       {22, kParamMstC, 22},
  {22, kParamMstMix, 28},

  // Family 02 / 004 Perc Organ
  {23, kParamEngine, 1},      {23, kParamOsc1Shape, 40},  {23, kParamOsc1Level, 95},
  {23, kParamOsc2Shape, 33},  {23, kParamOsc2Pitch, 19},  {23, kParamOsc2Level, 55},
  {23, kParamSubLevel, 25},   {23, kParamFilterCutoff, 6200}, {23, kParamFilterReso, 18},
  {23, kParamFilterEnv, 45},  {23, kParamAmpAttack, 1},   {23, kParamAmpDecay, 180},
  {23, kParamAmpSustain, 85}, {23, kParamAmpRelease, 70}, {23, kParamFiltAttack, 1},
  {23, kParamFiltDecay, 140}, {23, kParamFiltSustain, 40}, {23, kParamFiltRelease, 120},
  {23, kParamVelToAmp, 35},   {23, kParamLfoRate, 6.5},   {23, kParamLfoDest, 1},
  {23, kParamLfoAmount, 12},
  {23, kParamIns1Algo, 1},    {23, kParamIns1A, 36},      {23, kParamIns1B, 48},
  {23, kParamIns1C, 42},      {23, kParamIns1Mix, 22},    {23, kParamMstAlgo, 1},
  {23, kParamMstA, 52},       {23, kParamMstB, 38},       {23, kParamMstC, 30},
  {23, kParamMstMix, 24},

  // Family 02 / 005 Metal Bells
  {24, kParamEngine, 1},      {24, kParamOsc1Shape, 10},  {24, kParamOsc2Shape, 80},
  {24, kParamOsc2Pitch, 19},  {24, kParamOsc2Level, 70},  {24, kParamFmAmount, 40},
  {24, kParamOscRing, 1},     {24, kParamFilterCutoff, 9800}, {24, kParamFilterReso, 20},
  {24, kParamFilterKeyTrack, 60}, {24, kParamAmpAttack, 1},   {24, kParamAmpDecay, 1600},
  {24, kParamAmpSustain, 0},  {24, kParamAmpRelease, 1800}, {24, kParamFilterDrive, 18},
  {24, kParamSynthMasterLevel, -5},
  {24, kParamIns1Algo, 3},    {24, kParamIns1A, 24},      {24, kParamIns1B, 26},
  {24, kParamIns1C, 58},      {24, kParamIns1Mix, 20},    {24, kParamIns2Algo, 1},
  {24, kParamIns2A, 42},      {24, kParamIns2B, 56},      {24, kParamIns2C, 40},
  {24, kParamIns2Mix, 18},    {24, kParamMstAlgo, 3},     {24, kParamMstA, 62},
  {24, kParamMstB, 30},       {24, kParamMstC, 50},       {24, kParamMstMix, 24},

  // Family 02 / 006 Neon Pluck
  {25, kParamOsc1Shape, 70},  {25, kParamOsc2Shape, 40},  {25, kParamOsc2Pitch, 12},
  {25, kParamFilterCutoff, 3500}, {25, kParamFilterReso, 30}, {25, kParamFilterEnv, 75},
  {25, kParamFiltAttack, 1},  {25, kParamFiltDecay, 180}, {25, kParamFiltSustain, 0},
  {25, kParamFiltRelease, 200}, {25, kParamAmpAttack, 2},   {25, kParamAmpDecay, 220},
  {25, kParamAmpSustain, 0},  {25, kParamAmpRelease, 250}, {25, kParamFilterKeyTrack, 75},
  {25, kParamIns1Algo, 1},    {25, kParamIns1A, 44},      {25, kParamIns1B, 50},
  {25, kParamIns1C, 36},      {25, kParamIns1Mix, 24},    {25, kParamMstAlgo, 1},
  {25, kParamMstA, 55},       {25, kParamMstB, 34},       {25, kParamMstC, 26},
  {25, kParamMstMix, 22},

  // Family 02 / 007 Sparse Pluck
  {26, kParamEngine, 1},      {26, kParamOsc1Shape, 64},  {26, kParamOsc1Level, 80},
  {26, kParamOsc2Level, 12},  {26, kParamFilterCutoff, 4800}, {26, kParamFilterReso, 14},
  {26, kParamFilterEnv, 60},  {26, kParamFilterKeyTrack, 85}, {26, kParamFiltAttack, 1},
  {26, kParamFiltDecay, 90},  {26, kParamFiltSustain, 0}, {26, kParamFiltRelease, 80},
  {26, kParamAmpAttack, 1},   {26, kParamAmpDecay, 140},  {26, kParamAmpSustain, 0},
  {26, kParamAmpRelease, 160}, {26, kParamTapeMix, 12},    {26, kParamSynthMasterLevel, -3},
  {26, kParamIns1Algo, 3},    {26, kParamIns1A, 14},      {26, kParamIns1B, 10},
  {26, kParamIns1C, 72},      {26, kParamIns1Mix, 8},     {26, kParamMstAlgo, 1},
  {26, kParamMstA, 58},       {26, kParamMstB, 28},       {26, kParamMstC, 24},
  {26, kParamMstMix, 14},

  // Family 02 / 008 Thin Stab
  {27, kParamPolyMode, 1},    {27, kParamOsc1Shape, 55},  {27, kParamOsc1Level, 85},
  {27, kParamOsc2Shape, 100}, {27, kParamOsc2PW, 20},     {27, kParamOsc2Level, 60},
  {27, kParamFilterCutoff, 5200}, {27, kParamFilterReso, 18}, {27, kParamFilterEnv, 50},
  {27, kParamAmpAttack, 1},   {27, kParamAmpDecay, 120},  {27, kParamAmpSustain, 0},
  {27, kParamAmpRelease, 120}, {27, kParamTapeSat, 40},
  {27, kParamIns1Algo, 1},    {27, kParamIns1A, 52},      {27, kParamIns1B, 46},
  {27, kParamIns1C, 34},      {27, kParamIns1Mix, 30},    {27, kParamMstAlgo, 1},
  {27, kParamMstA, 50},       {27, kParamMstB, 48},       {27, kParamMstC, 20},
  {27, kParamMstMix, 30},

  // Family 02 / 009 Hard Bass
  {28, kParamEngine, 1},      {28, kParamPolyMode, 1},    {28, kParamOscSync, 1},
  {28, kParamOsc1Shape, 74},  {28, kParamOsc1Pitch, -12}, {28, kParamOsc2Shape, 80},
  {28, kParamOsc2Pitch, -12}, {28, kParamOsc2Level, 85},  {28, kParamSubLevel, 80},
  {28, kParamFilterCutoff, 900}, {28, kParamFilterReso, 28}, {28, kParamFilterEnv, 65},
  {28, kParamFilterDrive, 42}, {28, kParamFilterKeyTrack, 25}, {28, kParamAmpAttack, 1},
  {28, kParamAmpDecay, 260},  {28, kParamAmpSustain, 70}, {28, kParamAmpRelease, 140},
  {28, kParamVelToAmp, 45},   {28, kParamTapeSat, 48},    {28, kParamTapeMix, 20},
  {28, kParamIns1Algo, 1},    {28, kParamIns1A, 70},      {28, kParamIns1B, 35},
  {28, kParamIns1C, 28},      {28, kParamIns1Mix, 40},    {28, kParamMstAlgo, 2},
  {28, kParamMstA, 36},       {28, kParamMstB, 30},       {28, kParamMstC, 18},
  {28, kParamMstMix, 55},

  // Family 02 / 010 Sparse Bass
  {29, kParamEngine, 1},      {29, kParamPolyMode, 1},    {29, kParamMonoLegato, 1},
  {29, kParamOsc1Shape, 60},  {29, kParamOsc1Pitch, -12}, {29, kParamOsc2Level, 16},
  {29, kParamSubLevel, 85},   {29, kParamFilterCutoff, 700}, {29, kParamFilterReso, 12},
  {29, kParamFilterEnv, 30},  {29, kParamAmpAttack, 2},   {29, kParamAmpDecay, 220},
  {29, kParamAmpSustain, 55}, {29, kParamAmpRelease, 160}, {29, kParamVelToAmp, 30},
  {29, kParamTapeSat, 8},     {29, kParamTapeMix, 8},     {29, kParamSynthMasterLevel, -2},
  {29, kParamIns1Algo, 1},    {29, kParamIns1A, 22},      {29, kParamIns1B, 34},
  {29, kParamIns1C, 48},      {29, kParamIns1Mix, 14},    {29, kParamMstAlgo, 1},
  {29, kParamMstA, 60},       {29, kParamMstB, 30},       {29, kParamMstC, 24},
  {29, kParamMstMix, 18},

  // Family 02 / 011 Sync Lead
  {30, kParamEngine, 1},      {30, kParamPolyMode, 1},    {30, kParamOscSync, 1},
  {30, kParamPorta, 140},     {30, kParamOsc1Shape, 68},  {30, kParamOsc2Shape, 72},
  {30, kParamOsc2Pitch, 7},   {30, kParamOsc2Level, 75},  {30, kParamFilterCutoff, 2400},
  {30, kParamFilterReso, 36}, {30, kParamFilterEnv, 48},  {30, kParamFilterDrive, 35},
  {30, kParamFilterKeyTrack, 50}, {30, kParamAmpAttack, 6},   {30, kParamAmpDecay, 280},
  {30, kParamAmpSustain, 80}, {30, kParamAmpRelease, 200}, {30, kParamModWheelDest, 1},
  {30, kParamModWheelAmount, 55}, {30, kParamVelToFilter, 40},
  {30, kParamIns1Algo, 1},    {30, kParamIns1A, 58},      {30, kParamIns1B, 44},
  {30, kParamIns1C, 36},      {30, kParamIns1Mix, 32},    {30, kParamIns2Algo, 3},
  {30, kParamIns2A, 28},      {30, kParamIns2B, 24},      {30, kParamIns2C, 60},
  {30, kParamIns2Mix, 16},    {30, kParamMstAlgo, 1},     {30, kParamMstA, 54},
  {30, kParamMstB, 40},       {30, kParamMstC, 30},       {30, kParamMstMix, 26},

  // Family 02 / 012 Stab Brass
  {31, kParamEngine, 1},      {31, kParamOsc1Shape, 64},  {31, kParamOsc2Shape, 58},
  {31, kParamOsc2Pitch, 7},   {31, kParamOsc2Level, 80},  {31, kParamUnisonVoices, 2},
  {31, kParamUnisonDetune, 18}, {31, kParamFilterCutoff, 1800}, {31, kParamFilterReso, 26},
  {31, kParamFilterEnv, 78},  {31, kParamFilterDrive, 20}, {31, kParamFiltAttack, 8},
  {31, kParamFiltDecay, 160}, {31, kParamFiltSustain, 20}, {31, kParamFiltRelease, 180},
  {31, kParamAmpAttack, 8},   {31, kParamAmpDecay, 220},  {31, kParamAmpSustain, 35},
  {31, kParamAmpRelease, 180}, {31, kParamVelToAmp, 65},   {31, kParamVelToFilter, 70},
  {31, kParamIns1Algo, 1},    {31, kParamIns1A, 44},      {31, kParamIns1B, 50},
  {31, kParamIns1C, 40},      {31, kParamIns1Mix, 26},    {31, kParamMstAlgo, 1},
  {31, kParamMstA, 50},       {31, kParamMstB, 46},       {31, kParamMstC, 22},
  {31, kParamMstMix, 30},

  // Family 02 / 013 Tight Strings
  {32, kParamEngine, 1},      {32, kParamOsc1Shape, 48},  {32, kParamOsc2Shape, 44},
  {32, kParamOsc2Pitch, 12},  {32, kParamOsc2Level, 60},  {32, kParamUnisonVoices, 3},
  {32, kParamUnisonDetune, 12}, {32, kParamFilterCutoff, 3600}, {32, kParamFilterReso, 10},
  {32, kParamFilterEnv, 20},  {32, kParamAmpAttack, 180}, {32, kParamAmpDecay, 900},
  {32, kParamAmpSustain, 70}, {32, kParamAmpRelease, 700}, {32, kParamVelToAmp, 25},
  {32, kParamIns1Algo, 2},    {32, kParamIns1A, 22},      {32, kParamIns1B, 20},
  {32, kParamIns1C, 42},      {32, kParamIns1Mix, 16},    {32, kParamMstAlgo, 1},
  {32, kParamMstA, 52},       {32, kParamMstB, 48},       {32, kParamMstC, 34},
  {32, kParamMstMix, 30},

  // Family 02 / 014 Breath Choir
  {33, kParamEngine, 1},      {33, kParamOsc1Shape, 24},  {33, kParamOsc2Shape, 50},
  {33, kParamOsc2Pitch, 12},  {33, kParamOsc2Level, 45},  {33, kParamNoiseLevel, 16},
  {33, kParamUnisonVoices, 4}, {33, kParamUnisonDetune, 28}, {33, kParamFilterType, 2},
  {33, kParamFilterCutoff, 1400}, {33, kParamFilterReso, 22}, {33, kParamAmpAttack, 500},
  {33, kParamAmpDecay, 1600}, {33, kParamAmpSustain, 75}, {33, kParamAmpRelease, 1800},
  {33, kParamLfoRate, 0.4},   {33, kParamLfoDest, 4},     {33, kParamLfoAmount, 18},
  {33, kParamSynthMasterLevel, -4},
  {33, kParamIns1Algo, 2},    {33, kParamIns1A, 18},      {33, kParamIns1B, 32},
  {33, kParamIns1C, 58},      {33, kParamIns1Mix, 24},    {33, kParamIns2Algo, 3},
  {33, kParamIns2A, 40},      {33, kParamIns2B, 24},      {33, kParamIns2C, 50},
  {33, kParamIns2Mix, 16},    {33, kParamMstAlgo, 3},     {33, kParamMstA, 60},
  {33, kParamMstB, 28},       {33, kParamMstC, 50},       {33, kParamMstMix, 24},

  // Family 02 / 015 Poly Pad
  {34, kParamEngine, 1},      {34, kParamOsc1Shape, 44},  {34, kParamOsc2Shape, 66},
  {34, kParamOsc2Pitch, 7},   {34, kParamOsc2Level, 70},  {34, kParamUnisonVoices, 3},
  {34, kParamUnisonDetune, 24}, {34, kParamFilterCutoff, 2400}, {34, kParamFilterReso, 18},
  {34, kParamFilterEnv, 35},  {34, kParamFilterDrive, 12}, {34, kParamAmpAttack, 350},
  {34, kParamAmpDecay, 2000}, {34, kParamAmpSustain, 68}, {34, kParamAmpRelease, 2200},
  {34, kParamLfoRate, 0.3},   {34, kParamLfoDest, 1},     {34, kParamLfoAmount, 22},
  {34, kParamIns1Algo, 2},    {34, kParamIns1A, 26},      {34, kParamIns1B, 34},
  {34, kParamIns1C, 56},      {34, kParamIns1Mix, 28},    {34, kParamIns2Algo, 3},
  {34, kParamIns2A, 40},      {34, kParamIns2B, 30},      {34, kParamIns2C, 54},
  {34, kParamIns2Mix, 18},    {34, kParamMstAlgo, 3},     {34, kParamMstA, 64},
  {34, kParamMstB, 26},       {34, kParamMstC, 50},       {34, kParamMstMix, 26},

  // Family 02 / 016 Sweep Motion
  {35, kParamEngine, 1},      {35, kParamOsc1Shape, 50},  {35, kParamOsc2Shape, 62},
  {35, kParamOsc2Pitch, -5},  {35, kParamOsc2Level, 65},  {35, kParamUnisonVoices, 4},
  {35, kParamUnisonDetune, 44}, {35, kParamFilterCutoff, 800}, {35, kParamFilterReso, 32},
  {35, kParamFilterEnv, 80},  {35, kParamFiltAttack, 1800}, {35, kParamFiltDecay, 2400},
  {35, kParamFiltSustain, 30}, {35, kParamFiltRelease, 2000}, {35, kParamAmpAttack, 200},
  {35, kParamAmpDecay, 2600}, {35, kParamAmpSustain, 60}, {35, kParamAmpRelease, 2400},
  {35, kParamLfoRate, 0.12},  {35, kParamLfoShape, 2},    {35, kParamLfoDest, 1},
  {35, kParamLfoAmount, 55},
  {35, kParamIns1Algo, 2},    {35, kParamIns1A, 10},      {35, kParamIns1B, 38},
  {35, kParamIns1C, 60},      {35, kParamIns1Mix, 22},    {35, kParamIns2Algo, 3},
  {35, kParamIns2A, 62},      {35, kParamIns2B, 48},      {35, kParamIns2C, 46},
  {35, kParamIns2Mix, 36},    {35, kParamMstAlgo, 3},     {35, kParamMstA, 66},
  {35, kParamMstB, 24},       {35, kParamMstC, 50},       {35, kParamMstMix, 32},

  // Family 02 / 017 Skew Texture
  {36, kParamEngine, 1},      {36, kParamOsc1Shape, 58},  {36, kParamOsc2Shape, 90},
  {36, kParamOsc2Pitch, 6},   {36, kParamOsc2Level, 55},  {36, kParamOscRing, 1},
  {36, kParamNoiseLevel, 20}, {36, kParamFilterType, 2},  {36, kParamFilterCutoff, 900},
  {36, kParamFilterReso, 48}, {36, kParamFilterDrive, 28}, {36, kParamUnisonVoices, 2},
  {36, kParamUnisonDetune, 50}, {36, kParamAmpAttack, 80},  {36, kParamAmpDecay, 1800},
  {36, kParamAmpSustain, 55}, {36, kParamAmpRelease, 1600}, {36, kParamLfoRate, 3.5},
  {36, kParamLfoShape, 4},    {36, kParamLfoDest, 1},     {36, kParamLfoAmount, 60},
  {36, kParamIns1Algo, 4},    {36, kParamIns1A, 55},      {36, kParamIns1B, 40},
  {36, kParamIns1C, 35},      {36, kParamIns1Mix, 45},    {36, kParamIns2Algo, 1},
  {36, kParamIns2A, 48},      {36, kParamIns2B, 38},      {36, kParamIns2C, 44},
  {36, kParamIns2Mix, 24},    {36, kParamMstAlgo, 4},     {36, kParamMstA, 30},
  {36, kParamMstB, 55},       {36, kParamMstC, 18},       {36, kParamMstMix, 40},

  // Family 02 / 018 Pulse Drone
  {37, kParamEngine, 1},      {37, kParamOsc1Shape, 70},  {37, kParamOsc1PW, 30},
  {37, kParamOsc2Shape, 66},  {37, kParamOsc2Pitch, -12}, {37, kParamOsc2Level, 50},
  {37, kParamSubLevel, 70},   {37, kParamFilterCutoff, 600}, {37, kParamFilterReso, 42},
  {37, kParamFilterEnv, 15},  {37, kParamFilterDrive, 25}, {37, kParamAmpAttack, 900},
  {37, kParamAmpDecay, 3000}, {37, kParamAmpSustain, 90}, {37, kParamAmpRelease, 3200},
  {37, kParamLfoRate, 0.2},   {37, kParamLfoShape, 3},    {37, kParamLfoDest, 4},
  {37, kParamLfoAmount, 30},  {37, kParamSynthMasterLevel, -6},
  {37, kParamIns1Algo, 1},    {37, kParamIns1A, 30},      {37, kParamIns1B, 28},
  {37, kParamIns1C, 50},      {37, kParamIns1Mix, 20},    {37, kParamIns2Algo, 3},
  {37, kParamIns2A, 58},      {37, kParamIns2B, 34},      {37, kParamIns2C, 34},
  {37, kParamIns2Mix, 16},    {37, kParamMstAlgo, 4},     {37, kParamMstA, 38},
  {37, kParamMstB, 30},       {37, kParamMstC, 12},       {37, kParamMstMix, 24},

  // Family 02 / 019 Wide Arp
  {38, kParamOsc1Shape, 66},  {38, kParamOsc2Shape, 66},  {38, kParamOsc2Pitch, 7},
  {38, kParamOsc2Level, 70},  {38, kParamUnisonVoices, 4}, {38, kParamUnisonDetune, 40},
  {38, kParamFilterCutoff, 7000}, {38, kParamAmpAttack, 1},   {38, kParamAmpDecay, 90},
  {38, kParamAmpSustain, 0},  {38, kParamAmpRelease, 140},
  {38, kParamIns1Algo, 3},    {38, kParamIns1A, 20},      {38, kParamIns1B, 22},
  {38, kParamIns1C, 66},      {38, kParamIns1Mix, 18},    {38, kParamMstAlgo, 3},
  {38, kParamMstA, 66},       {38, kParamMstB, 28},       {38, kParamMstC, 50},
  {38, kParamMstMix, 24},

  // Family 02 / 020 Heavy Crush
  {39, kParamEngine, 1},      {39, kParamOsc1Shape, 78},  {39, kParamOsc2Shape, 84},
  {39, kParamOsc2Pitch, 12},  {39, kParamOsc2Level, 70},  {39, kParamFilterCutoff, 3200},
  {39, kParamFilterReso, 24}, {39, kParamFilterDrive, 55}, {39, kParamAmpAttack, 5},
  {39, kParamAmpDecay, 600},  {39, kParamAmpSustain, 40}, {39, kParamAmpRelease, 400},
  {39, kParamOutLevel, -4},
  {39, kParamIns1Algo, 4},    {39, kParamIns1A, 72},      {39, kParamIns1B, 64},
  {39, kParamIns1C, 48},      {39, kParamIns1Mix, 80},    {39, kParamIns2Algo, 1},
  {39, kParamIns2A, 66},      {39, kParamIns2B, 30},      {39, kParamIns2C, 44},
  {39, kParamIns2Mix, 50},    {39, kParamMstAlgo, 2},     {39, kParamMstA, 28},
  {39, kParamMstB, 55},       {39, kParamMstC, 40},       {39, kParamMstMix, 70},

  // Family 02 / 021 Punch Kit
  {40, kParamEngine, 1},      {40, kParamOsc1Shape, 100}, {40, kParamOsc1PW, 15},
  {40, kParamOsc2Shape, 100}, {40, kParamOsc2Pitch, -12}, {40, kParamOsc2Level, 90},
  {40, kParamNoiseLevel, 25}, {40, kParamSubLevel, 40},   {40, kParamFilterCutoff, 11000},
  {40, kParamFilterReso, 6},  {40, kParamFilterEnv, 0},   {40, kParamFilterDrive, 48},
  {40, kParamAmpAttack, 0.5}, {40, kParamAmpDecay, 90},   {40, kParamAmpSustain, 0},
  {40, kParamAmpRelease, 70}, {40, kParamPitchAmount, -20}, {40, kParamPitchAttack, 0.5},
  {40, kParamPitchDecay, 50}, {40, kParamPitchRelease, 50}, {40, kParamTapeSat, 30},
  {40, kParamTapeMix, 12},    {40, kParamDrumSlotTune, 2}, {40, kParamDrumSlotDecay, 28},
  {40, kParamDrumSlotDrive, 70}, {40, kParamDrumSlotFilter, 15000}, {40, kParamDrumKitLevel, -2},
  {40, kParamIns1Algo, 1},    {40, kParamIns1A, 62},      {40, kParamIns1B, 45},
  {40, kParamIns1C, 40},      {40, kParamIns1Mix, 36},    {40, kParamIns2Algo, 4},
  {40, kParamIns2A, 66},      {40, kParamIns2B, 50},      {40, kParamIns2C, 30},
  {40, kParamIns2Mix, 20},    {40, kParamMstAlgo, 2},     {40, kParamMstA, 24},
  {40, kParamMstB, 46},       {40, kParamMstC, 18},       {40, kParamMstMix, 60},

  // Family 03 / 001 Piano Stack
  {41, kParamOsc1Shape, 12},  {41, kParamOsc1Level, 95},  {41, kParamOsc2Shape, 18},
  {41, kParamOsc2Pitch, 12},  {41, kParamOsc2Level, 48},  {41, kParamFmAmount, 6},
  {41, kParamFilterCutoff, 6400}, {41, kParamFilterReso, 4},  {41, kParamFilterEnv, 12},
  {41, kParamAmpAttack, 6},   {41, kParamAmpDecay, 1100}, {41, kParamAmpSustain, 38},
  {41, kParamAmpRelease, 900}, {41, kParamVelToAmp, 35},   {41, kParamTapeSat, 18},
  {41, kParamTapeMix, 28},
  {41, kParamIns1Algo, 2},    {41, kParamIns1A, 10},      {41, kParamIns1B, 16},
  {41, kParamIns1C, 38},      {41, kParamIns1Mix, 12},    {41, kParamIns2Algo, 3},
  {41, kParamIns2A, 30},      {41, kParamIns2B, 18},      {41, kParamIns2C, 50},
  {41, kParamIns2Mix, 12},    {41, kParamMstAlgo, 4},     {41, kParamMstA, 56},
  {41, kParamMstB, 44},       {41, kParamMstC, 10},       {41, kParamMstMix, 16},

  // Family 03 / 002 Soft EP
  {42, kParamOsc1Shape, 25},  {42, kParamOsc2Shape, 33},  {42, kParamOsc2Pitch, 12},
  {42, kParamOsc2Level, 45},  {42, kParamFmAmount, 18},   {42, kParamFilterCutoff, 4200},
  {42, kParamFilterReso, 12}, {42, kParamAmpAttack, 6},   {42, kParamAmpDecay, 1400},
  {42, kParamAmpSustain, 45}, {42, kParamAmpRelease, 800}, {42, kParamTapeMix, 35},
  {42, kParamIns1Algo, 2},    {42, kParamIns1A, 16},      {42, kParamIns1B, 24},
  {42, kParamIns1C, 46},      {42, kParamIns1Mix, 18},    {42, kParamMstAlgo, 4},
  {42, kParamMstA, 58},       {42, kParamMstB, 40},       {42, kParamMstC, 12},
  {42, kParamMstMix, 18},

  // Family 03 / 003 Soul Piano
  {43, kParamOsc1Shape, 20},  {43, kParamOsc2Shape, 36},  {43, kParamOsc2Pitch, 12},
  {43, kParamOsc2Level, 52},  {43, kParamFmAmount, 22},   {43, kParamFilterCutoff, 3400},
  {43, kParamFilterReso, 14}, {43, kParamFilterEnv, 22},  {43, kParamAmpAttack, 8},
  {43, kParamAmpDecay, 1600}, {43, kParamAmpSustain, 50}, {43, kParamAmpRelease, 1100},
  {43, kParamVelToAmp, 58},   {43, kParamVelToFilter, 30}, {43, kParamTapeSat, 30},
  {43, kParamTapeMix, 38},    {43, kParamTapeHiss, 6},
  {43, kParamIns1Algo, 2},    {43, kParamIns1A, 14},      {43, kParamIns1B, 28},
  {43, kParamIns1C, 42},      {43, kParamIns1Mix, 26},    {43, kParamIns2Algo, 3},
  {43, kParamIns2A, 26},      {43, kParamIns2B, 20},      {43, kParamIns2C, 46},
  {43, kParamIns2Mix, 14},    {43, kParamMstAlgo, 4},     {43, kParamMstA, 58},
  {43, kParamMstB, 45},       {43, kParamMstC, 14},       {43, kParamMstMix, 22},

  // Family 03 / 004 Clav Stab
  {44, kParamOsc1Shape, 84},  {44, kParamOsc1PW, 24},     {44, kParamOsc2Shape, 92},
  {44, kParamOsc2Pitch, 12},  {44, kParamOsc2Level, 62},  {44, kParamOsc2PW, 18},
  {44, kParamFilterCutoff, 2800}, {44, kParamFilterReso, 45}, {44, kParamFilterEnv, 80},
  {44, kParamFilterDrive, 26}, {44, kParamFilterKeyTrack, 40}, {44, kParamFiltAttack, 1},
  {44, kParamFiltDecay, 100}, {44, kParamFiltSustain, 0}, {44, kParamFiltRelease, 90},
  {44, kParamAmpAttack, 1},   {44, kParamAmpDecay, 140},  {44, kParamAmpSustain, 0},
  {44, kParamAmpRelease, 110}, {44, kParamVelToAmp, 78},   {44, kParamVelToFilter, 60},
  {44, kParamTapeSat, 22},    {44, kParamTapeMix, 16},
  {44, kParamIns1Algo, 1},    {44, kParamIns1A, 38},      {44, kParamIns1B, 50},
  {44, kParamIns1C, 44},      {44, kParamIns1Mix, 24},    {44, kParamMstAlgo, 1},
  {44, kParamMstA, 54},       {44, kParamMstB, 44},       {44, kParamMstC, 24},
  {44, kParamMstMix, 26},

  // Family 03 / 005 Dry Clav
  {45, kParamOsc1Shape, 78},  {45, kParamOsc1PW, 30},     {45, kParamOsc2Shape, 90},
  {45, kParamOsc2Pitch, 12},  {45, kParamOsc2Level, 55},  {45, kParamFilterCutoff, 3000},
  {45, kParamFilterReso, 28}, {45, kParamFilterEnv, 60},  {45, kParamAmpAttack, 1},
  {45, kParamAmpDecay, 260},  {45, kParamAmpSustain, 0},  {45, kParamAmpRelease, 160},
  {45, kParamTapeSat, 30},    {45, kParamTapeMix, 20},
  {45, kParamIns1Algo, 1},    {45, kParamIns1A, 28},      {45, kParamIns1B, 46},
  {45, kParamIns1C, 48},      {45, kParamIns1Mix, 18},    {45, kParamMstAlgo, 1},
  {45, kParamMstA, 58},       {45, kParamMstB, 36},       {45, kParamMstC, 22},
  {45, kParamMstMix, 20},

  // Family 03 / 006 Warm Organ
  {46, kParamOsc1Shape, 33},  {46, kParamOsc1Level, 90},  {46, kParamOsc2Shape, 33},
  {46, kParamOsc2Pitch, 12},  {46, kParamOsc2Level, 90},  {46, kParamSubLevel, 40},
  {46, kParamFilterCutoff, 8000}, {46, kParamFilterReso, 6},  {46, kParamAmpAttack, 3},
  {46, kParamAmpDecay, 60},   {46, kParamAmpSustain, 100}, {46, kParamAmpRelease, 90},
  {46, kParamIns1Algo, 2},    {46, kParamIns1A, 14},      {46, kParamIns1B, 26},
  {46, kParamIns1C, 48},      {46, kParamIns1Mix, 20},    {46, kParamMstAlgo, 1},
  {46, kParamMstA, 56},       {46, kParamMstB, 32},       {46, kParamMstC, 38},
  {46, kParamMstMix, 22},

  // Family 03 / 007 Bottle Bells
  {47, kParamOsc1Shape, 4},   {47, kParamOsc2Shape, 8},   {47, kParamOsc2Pitch, 12},
  {47, kParamOsc2Level, 40},  {47, kParamFmAmount, 14},   {47, kParamFilterCutoff, 7000},
  {47, kParamFilterReso, 4},  {47, kParamFilterKeyTrack, 35}, {47, kParamAmpAttack, 3},
  {47, kParamAmpDecay, 1800}, {47, kParamAmpSustain, 10}, {47, kParamAmpRelease, 1600},
  {47, kParamTapeMix, 24},
  {47, kParamIns1Algo, 3},    {47, kParamIns1A, 30},      {47, kParamIns1B, 18},
  {47, kParamIns1C, 75},      {47, kParamIns1Mix, 14},    {47, kParamIns2Algo, 2},
  {47, kParamIns2A, 12},      {47, kParamIns2B, 22},      {47, kParamIns2C, 62},
  {47, kParamIns2Mix, 14},    {47, kParamMstAlgo, 3},     {47, kParamMstA, 62},
  {47, kParamMstB, 28},       {47, kParamMstC, 50},       {47, kParamMstMix, 22},

  // Family 03 / 008 Muted Pluck
  {48, kParamOsc1Shape, 46},  {48, kParamOsc1PW, 35},     {48, kParamOsc2Shape, 30},
  {48, kParamOsc2Pitch, 12},  {48, kParamOsc2Level, 35},  {48, kParamFilterCutoff, 2200},
  {48, kParamFilterReso, 16}, {48, kParamFilterEnv, 40},  {48, kParamFilterKeyTrack, 50},
  {48, kParamFiltAttack, 2},  {48, kParamFiltDecay, 200}, {48, kParamFiltSustain, 0},
  {48, kParamFiltRelease, 180}, {48, kParamAmpAttack, 2},   {48, kParamAmpDecay, 280},
  {48, kParamAmpSustain, 0},  {48, kParamAmpRelease, 200}, {48, kParamTapeSat, 20},
  {48, kParamTapeMix, 22},
  {48, kParamIns1Algo, 3},    {48, kParamIns1A, 12},      {48, kParamIns1B, 12},
  {48, kParamIns1C, 52},      {48, kParamIns1Mix, 10},    {48, kParamMstAlgo, 1},
  {48, kParamMstA, 56},       {48, kParamMstB, 30},       {48, kParamMstC, 24},
  {48, kParamMstMix, 18},

  // Family 03 / 009 Reed Stab
  {49, kParamOsc1Shape, 38},  {49, kParamOsc2Shape, 42},  {49, kParamOsc2Pitch, 7},
  {49, kParamOsc2Level, 55},  {49, kParamFilterCutoff, 2600}, {49, kParamFilterReso, 24},
  {49, kParamFilterEnv, 55},  {49, kParamFiltAttack, 4},  {49, kParamFiltDecay, 150},
  {49, kParamFiltSustain, 0}, {49, kParamFiltRelease, 140}, {49, kParamAmpAttack, 4},
  {49, kParamAmpDecay, 200},  {49, kParamAmpSustain, 0},  {49, kParamAmpRelease, 160},
  {49, kParamVelToAmp, 50},   {49, kParamVelToFilter, 40},
  {49, kParamIns1Algo, 2},    {49, kParamIns1A, 16},      {49, kParamIns1B, 22},
  {49, kParamIns1C, 44},      {49, kParamIns1Mix, 18},    {49, kParamIns2Algo, 3},
  {49, kParamIns2A, 20},      {49, kParamIns2B, 16},      {49, kParamIns2C, 48},
  {49, kParamIns2Mix, 12},    {49, kParamMstAlgo, 1},     {49, kParamMstA, 52},
  {49, kParamMstB, 44},       {49, kParamMstC, 22},       {49, kParamMstMix, 26},

  // Family 03 / 010 Finger Bass
  {50, kParamPolyMode, 1},    {50, kParamMonoLegato, 1},  {50, kParamOsc1Shape, 28},
  {50, kParamOsc1Pitch, -12}, {50, kParamOsc2Shape, 22},  {50, kParamOsc2Pitch, -12},
  {50, kParamOsc2Level, 40},  {50, kParamSubLevel, 55},   {50, kParamFilterCutoff, 1100},
  {50, kParamFilterReso, 14}, {50, kParamFilterEnv, 35},  {50, kParamAmpAttack, 4},
  {50, kParamAmpDecay, 400},  {50, kParamAmpSustain, 72}, {50, kParamAmpRelease, 220},
  {50, kParamVelToAmp, 40},   {50, kParamTapeSat, 24},    {50, kParamTapeMix, 30},
  {50, kParamIns1Algo, 1},    {50, kParamIns1A, 26},      {50, kParamIns1B, 38},
  {50, kParamIns1C, 46},      {50, kParamIns1Mix, 20},    {50, kParamMstAlgo, 1},
  {50, kParamMstA, 60},       {50, kParamMstB, 40},       {50, kParamMstC, 22},
  {50, kParamMstMix, 24},

  // Family 03 / 011 Solo Lead
  {51, kParamPolyMode, 1},    {51, kParamPorta, 60},      {51, kParamOsc1Shape, 34},
  {51, kParamOsc2Shape, 40},  {51, kParamOsc2Pitch, 12},  {51, kParamOsc2Level, 40},
  {51, kParamFmAmount, 10},   {51, kParamFilterCutoff, 4500}, {51, kParamFilterReso, 12},
  {51, kParamFilterEnv, 28},  {51, kParamFilterKeyTrack, 30}, {51, kParamAmpAttack, 18},
  {51, kParamAmpDecay, 500},  {51, kParamAmpSustain, 76}, {51, kParamAmpRelease, 320},
  {51, kParamModWheelDest, 2}, {51, kParamModWheelAmount, 18}, {51, kParamVelToAmp, 28},
  {51, kParamIns1Algo, 3},    {51, kParamIns1A, 28},      {51, kParamIns1B, 22},
  {51, kParamIns1C, 58},      {51, kParamIns1Mix, 16},    {51, kParamIns2Algo, 2},
  {51, kParamIns2A, 14},      {51, kParamIns2B, 18},      {51, kParamIns2C, 54},
  {51, kParamIns2Mix, 12},    {51, kParamMstAlgo, 1},     {51, kParamMstA, 56},
  {51, kParamMstB, 36},       {51, kParamMstC, 32},       {51, kParamMstMix, 22},

  // Family 03 / 012 Ensemble Brass
  {52, kParamOsc1Shape, 46},  {52, kParamOsc2Shape, 50},  {52, kParamOsc2Pitch, 7},
  {52, kParamOsc2Level, 75},  {52, kParamUnisonVoices, 3}, {52, kParamUnisonDetune, 14},
  {52, kParamFilterCutoff, 2000}, {52, kParamFilterReso, 10}, {52, kParamFilterEnv, 48},
  {52, kParamFiltAttack, 40}, {52, kParamFiltDecay, 400}, {52, kParamFiltSustain, 60},
  {52, kParamFiltRelease, 500}, {52, kParamAmpAttack, 45},  {52, kParamAmpDecay, 700},
  {52, kParamAmpSustain, 80}, {52, kParamAmpRelease, 400}, {52, kParamVelToFilter, 45},
  {52, kParamTapeSat, 20},    {52, kParamTapeMix, 32},
  {52, kParamIns1Algo, 2},    {52, kParamIns1A, 12},      {52, kParamIns1B, 24},
  {52, kParamIns1C, 52},      {52, kParamIns1Mix, 20},    {52, kParamMstAlgo, 1},
  {52, kParamMstA, 56},       {52, kParamMstB, 42},       {52, kParamMstC, 42},
  {52, kParamMstMix, 30},

  // Family 03 / 013 Section Strings
  {53, kParamOsc1Shape, 36},  {53, kParamOsc2Shape, 32},  {53, kParamOsc2Pitch, 12},
  {53, kParamOsc2Level, 80},  {53, kParamUnisonVoices, 4}, {53, kParamUnisonDetune, 18},
  {53, kParamFilterCutoff, 2800}, {53, kParamFilterReso, 6},  {53, kParamFilterEnv, 18},
  {53, kParamAmpAttack, 700}, {53, kParamAmpDecay, 2000}, {53, kParamAmpSustain, 85},
  {53, kParamAmpRelease, 2000}, {53, kParamFiltAttack, 800}, {53, kParamFiltDecay, 1800},
  {53, kParamFiltSustain, 70}, {53, kParamFiltRelease, 1800}, {53, kParamTapeMix, 36},
  {53, kParamSynthMasterLevel, -2},
  {53, kParamIns1Algo, 2},    {53, kParamIns1A, 8},       {53, kParamIns1B, 30},
  {53, kParamIns1C, 50},      {53, kParamIns1Mix, 24},    {53, kParamIns2Algo, 3},
  {53, kParamIns2A, 44},      {53, kParamIns2B, 24},      {53, kParamIns2C, 46},
  {53, kParamIns2Mix, 16},    {53, kParamMstAlgo, 3},     {53, kParamMstA, 60},
  {53, kParamMstB, 30},       {53, kParamMstC, 50},       {53, kParamMstMix, 24},

  // Family 03 / 014 Ooh Choir
  {54, kParamOsc1Shape, 18},  {54, kParamOsc2Shape, 22},  {54, kParamOsc2Pitch, 12},
  {54, kParamOsc2Level, 70},  {54, kParamNoiseLevel, 6},  {54, kParamUnisonVoices, 5},
  {54, kParamUnisonDetune, 16}, {54, kParamFilterCutoff, 3600}, {54, kParamFilterReso, 8},
  {54, kParamAmpAttack, 800}, {54, kParamAmpDecay, 2200}, {54, kParamAmpSustain, 88},
  {54, kParamAmpRelease, 2400}, {54, kParamLfoRate, 0.15},  {54, kParamLfoDest, 3},
  {54, kParamLfoAmount, 8},   {54, kParamTapeHiss, 8},    {54, kParamTapeMix, 30},
  {54, kParamIns1Algo, 2},    {54, kParamIns1A, 10},      {54, kParamIns1B, 26},
  {54, kParamIns1C, 56},      {54, kParamIns1Mix, 22},    {54, kParamIns2Algo, 3},
  {54, kParamIns2A, 48},      {54, kParamIns2B, 20},      {54, kParamIns2C, 44},
  {54, kParamIns2Mix, 16},    {54, kParamMstAlgo, 3},     {54, kParamMstA, 62},
  {54, kParamMstB, 28},       {54, kParamMstC, 50},       {54, kParamMstMix, 22},

  // Family 03 / 015 Dream Pad
  {55, kParamOsc1Shape, 14},  {55, kParamOsc2Shape, 26},  {55, kParamOsc2Pitch, 7},
  {55, kParamOsc2Level, 55},  {55, kParamUnisonVoices, 3}, {55, kParamUnisonDetune, 20},
  {55, kParamFilterCutoff, 3000}, {55, kParamFilterReso, 12}, {55, kParamFilterEnv, 20},
  {55, kParamAmpAttack, 900}, {55, kParamAmpDecay, 2400}, {55, kParamAmpSustain, 70},
  {55, kParamAmpRelease, 2800}, {55, kParamLfoRate, 0.1},   {55, kParamLfoDest, 1},
  {55, kParamLfoAmount, 16},  {55, kParamSynthMasterLevel, -3},
  {55, kParamIns1Algo, 2},    {55, kParamIns1A, 12},      {55, kParamIns1B, 34},
  {55, kParamIns1C, 58},      {55, kParamIns1Mix, 28},    {55, kParamIns2Algo, 3},
  {55, kParamIns2A, 48},      {55, kParamIns2B, 26},      {55, kParamIns2C, 52},
  {55, kParamIns2Mix, 16},    {55, kParamMstAlgo, 3},     {55, kParamMstA, 64},
  {55, kParamMstB, 26},       {55, kParamMstC, 50},       {55, kParamMstMix, 26},

  // Family 03 / 016 Slow Sweep
  {56, kParamOsc1Shape, 10},  {56, kParamOsc2Shape, 48},  {56, kParamOsc2Pitch, 5},
  {56, kParamOsc2Level, 45},  {56, kParamFilterCutoff, 1400}, {56, kParamFilterReso, 18},
  {56, kParamFilterEnv, 40},  {56, kParamFiltAttack, 2000}, {56, kParamFiltDecay, 3000},
  {56, kParamFiltSustain, 45}, {56, kParamFiltRelease, 2500}, {56, kParamAmpAttack, 1400},
  {56, kParamAmpDecay, 3200}, {56, kParamAmpSustain, 75}, {56, kParamAmpRelease, 3000},
  {56, kParamLfoRate, 0.07},  {56, kParamLfoShape, 1},    {56, kParamLfoDest, 1},
  {56, kParamLfoAmount, 28},
  {56, kParamIns1Algo, 2},    {56, kParamIns1A, 8},       {56, kParamIns1B, 40},
  {56, kParamIns1C, 54},      {56, kParamIns1Mix, 22},    {56, kParamIns2Algo, 3},
  {56, kParamIns2A, 56},      {56, kParamIns2B, 34},      {56, kParamIns2C, 44},
  {56, kParamIns2Mix, 20},    {56, kParamMstAlgo, 4},     {56, kParamMstA, 46},
  {56, kParamMstB, 32},       {56, kParamMstC, 12},       {56, kParamMstMix, 22},

  // Family 03 / 017 Hollow Texture
  {57, kParamOsc1Shape, 22},  {57, kParamOsc2Shape, 55},  {57, kParamOsc2Pitch, 7},
  {57, kParamOsc2Level, 35},  {57, kParamNoiseLevel, 12}, {57, kParamFilterType, 2},
  {57, kParamFilterCutoff, 1600}, {57, kParamFilterReso, 30}, {57, kParamUnisonVoices, 2},
  {57, kParamUnisonDetune, 30}, {57, kParamAmpAttack, 600}, {57, kParamAmpDecay, 2400},
  {57, kParamAmpSustain, 60}, {57, kParamAmpRelease, 2200}, {57, kParamLfoRate, 0.55},
  {57, kParamLfoShape, 4},    {57, kParamLfoDest, 1},     {57, kParamLfoAmount, 24},
  {57, kParamIns1Algo, 4},    {57, kParamIns1A, 20},      {57, kParamIns1B, 18},
  {57, kParamIns1C, 26},      {57, kParamIns1Mix, 12},    {57, kParamIns2Algo, 3},
  {57, kParamIns2A, 36},      {57, kParamIns2B, 28},      {57, kParamIns2C, 42},
  {57, kParamIns2Mix, 16},    {57, kParamMstAlgo, 4},     {57, kParamMstA, 40},
  {57, kParamMstB, 40},       {57, kParamMstC, 14},       {57, kParamMstMix, 18},

  // Family 03 / 018 Warm Drone
  {58, kParamOsc1Shape, 6},   {58, kParamOsc2Shape, 14},  {58, kParamOsc2Pitch, -12},
  {58, kParamOsc2Level, 60},  {58, kParamSubLevel, 45},   {58, kParamFilterCutoff, 700},
  {58, kParamFilterReso, 22}, {58, kParamFilterEnv, 8},   {58, kParamAmpAttack, 2200},
  {58, kParamAmpDecay, 4000}, {58, kParamAmpSustain, 92}, {58, kParamAmpRelease, 3800},
  {58, kParamLfoRate, 0.05},  {58, kParamLfoDest, 1},     {58, kParamLfoAmount, 14},
  {58, kParamTapeSat, 28},    {58, kParamTapeMix, 40},    {58, kParamSynthMasterLevel, -5},
  {58, kParamIns1Algo, 2},    {58, kParamIns1A, 6},       {58, kParamIns1B, 28},
  {58, kParamIns1C, 42},      {58, kParamIns1Mix, 18},    {58, kParamIns2Algo, 3},
  {58, kParamIns2A, 60},      {58, kParamIns2B, 32},      {58, kParamIns2C, 28},
  {58, kParamIns2Mix, 14},    {58, kParamMstAlgo, 4},     {58, kParamMstA, 38},
  {58, kParamMstB, 26},       {58, kParamMstC, 10},       {58, kParamMstMix, 20},

  // Family 03 / 019 Mallet Arp
  {59, kParamOsc1Shape, 8},   {59, kParamOsc2Shape, 16},  {59, kParamOsc2Pitch, 12},
  {59, kParamOsc2Level, 50},  {59, kParamFmAmount, 8},    {59, kParamFilterCutoff, 5500},
  {59, kParamFilterReso, 10}, {59, kParamFilterEnv, 35},  {59, kParamFilterKeyTrack, 40},
  {59, kParamAmpAttack, 2},   {59, kParamAmpDecay, 160},  {59, kParamAmpSustain, 0},
  {59, kParamAmpRelease, 180}, {59, kParamFiltAttack, 1},  {59, kParamFiltDecay, 120},
  {59, kParamFiltSustain, 0}, {59, kParamFiltRelease, 140},
  {59, kParamIns1Algo, 3},    {59, kParamIns1A, 20},      {59, kParamIns1B, 12},
  {59, kParamIns1C, 60},      {59, kParamIns1Mix, 10},    {59, kParamMstAlgo, 1},
  {59, kParamMstA, 54},       {59, kParamMstB, 28},       {59, kParamMstC, 24},
  {59, kParamMstMix, 18},

  // Family 03 / 020 Slap Delay
  {60, kParamOsc1Shape, 70},  {60, kParamOsc1PW, 40},     {60, kParamOsc2Shape, 60},
  {60, kParamOsc2Pitch, 12},  {60, kParamOsc2Level, 40},  {60, kParamFilterCutoff, 3600},
  {60, kParamFilterReso, 20}, {60, kParamFilterEnv, 45},  {60, kParamAmpAttack, 1},
  {60, kParamAmpDecay, 180},  {60, kParamAmpSustain, 0},  {60, kParamAmpRelease, 140},
  {60, kParamTapeSat, 26},
  {60, kParamIns1Algo, 3},    {60, kParamIns1A, 18},      {60, kParamIns1B, 42},
  {60, kParamIns1C, 60},      {60, kParamIns1Mix, 48},    {60, kParamIns2Algo, 4},
  {60, kParamIns2A, 52},      {60, kParamIns2B, 44},      {60, kParamIns2C, 34},
  {60, kParamIns2Mix, 26},    {60, kParamMstAlgo, 3},     {60, kParamMstA, 64},
  {60, kParamMstB, 30},       {60, kParamMstC, 50},       {60, kParamMstMix, 30},

  // Family 03 / 021 Dry Perc
  {61, kParamEngine, 1},      {61, kParamOsc1Shape, 88},  {61, kParamOsc1PW, 45},
  {61, kParamOsc2Shape, 70},  {61, kParamOsc2Level, 40},  {61, kParamNoiseLevel, 22},
  {61, kParamFilterCutoff, 8000}, {61, kParamFilterReso, 8},  {61, kParamFilterEnv, 15},
  {61, kParamAmpAttack, 0.5}, {61, kParamAmpDecay, 100},  {61, kParamAmpSustain, 0},
  {61, kParamAmpRelease, 80}, {61, kParamPitchAmount, -8}, {61, kParamPitchDecay, 60},
  {61, kParamPitchRelease, 60}, {61, kParamTapeSat, 10},    {61, kParamTapeMix, 10},
  {61, kParamDrumSlotDecay, 36}, {61, kParamDrumSlotDrive, 18}, {61, kParamDrumSlotFilter, 11000},
  {61, kParamDrumKitLevel, -3}, {61, kParamFilterDrive, 12},
  {61, kParamIns1Algo, 1},    {61, kParamIns1A, 30},      {61, kParamIns1B, 52},
  {61, kParamIns1C, 44},      {61, kParamIns1Mix, 22},    {61, kParamIns2Algo, 4},
  {61, kParamIns2A, 58},      {61, kParamIns2B, 46},      {61, kParamIns2C, 28},
  {61, kParamIns2Mix, 16},    {61, kParamMstAlgo, 2},     {61, kParamMstA, 34},
  {61, kParamMstB, 42},       {61, kParamMstC, 16},       {61, kParamMstMix, 46},

  // Family 04 / 001 Vector Keys
  {62, kParamOsc1Shape, 28},  {62, kParamOsc2Shape, 64},  {62, kParamOsc2Pitch, 7},
  {62, kParamOsc2Level, 58},  {62, kParamUnisonVoices, 3}, {62, kParamUnisonDetune, 32},
  {62, kParamFilterCutoff, 5000}, {62, kParamFilterReso, 14}, {62, kParamFilterEnv, 22},
  {62, kParamAmpAttack, 20},  {62, kParamAmpDecay, 900},  {62, kParamAmpSustain, 48},
  {62, kParamAmpRelease, 1100}, {62, kParamLfoRate, 0.18},  {62, kParamLfoDest, 1},
  {62, kParamLfoAmount, 18},  {62, kParamVelToAmp, 30},
  {62, kParamIns1Algo, 2},    {62, kParamIns1A, 16},      {62, kParamIns1B, 48},
  {62, kParamIns1C, 62},      {62, kParamIns1Mix, 26},    {62, kParamIns2Algo, 3},
  {62, kParamIns2A, 34},      {62, kParamIns2B, 22},      {62, kParamIns2C, 54},
  {62, kParamIns2Mix, 14},    {62, kParamMstAlgo, 3},     {62, kParamMstA, 62},
  {62, kParamMstB, 30},       {62, kParamMstC, 50},       {62, kParamMstMix, 24},

  // Family 04 / 002 Phase Piano
  {63, kParamOsc1Shape, 18},  {63, kParamOsc2Shape, 44},  {63, kParamOsc2Pitch, 12},
  {63, kParamOsc2Level, 36},  {63, kParamFmAmount, 12},   {63, kParamFilterCutoff, 3900},
  {63, kParamFilterReso, 16}, {63, kParamAmpAttack, 10},  {63, kParamAmpDecay, 1300},
  {63, kParamAmpSustain, 36}, {63, kParamAmpRelease, 900}, {63, kParamLfoRate, 0.45},
  {63, kParamLfoDest, 3},     {63, kParamLfoAmount, 22},  {63, kParamVelToAmp, 42},
  {63, kParamIns1Algo, 2},    {63, kParamIns1A, 28},      {63, kParamIns1B, 55},
  {63, kParamIns1C, 66},      {63, kParamIns1Mix, 34},    {63, kParamIns2Algo, 3},
  {63, kParamIns2A, 28},      {63, kParamIns2B, 18},      {63, kParamIns2C, 50},
  {63, kParamIns2Mix, 12},    {63, kParamMstAlgo, 3},     {63, kParamMstA, 60},
  {63, kParamMstB, 32},       {63, kParamMstC, 50},       {63, kParamMstMix, 22},

  // Family 04 / 003 Wave Clav
  {64, kParamOsc1Shape, 72},  {64, kParamOsc1PW, 42},     {64, kParamOsc2Shape, 80},
  {64, kParamOsc2Pitch, 12},  {64, kParamOsc2Level, 48},  {64, kParamFilterCutoff, 2600},
  {64, kParamFilterReso, 26}, {64, kParamFilterEnv, 50},  {64, kParamFilterKeyTrack, 35},
  {64, kParamAmpAttack, 2},   {64, kParamAmpDecay, 220},  {64, kParamAmpSustain, 0},
  {64, kParamAmpRelease, 180}, {64, kParamLfoRate, 0.7},   {64, kParamLfoDest, 1},
  {64, kParamLfoAmount, 16},  {64, kParamVelToAmp, 48},
  {64, kParamIns1Algo, 2},    {64, kParamIns1A, 20},      {64, kParamIns1B, 36},
  {64, kParamIns1C, 48},      {64, kParamIns1Mix, 20},    {64, kParamIns2Algo, 1},
  {64, kParamIns2A, 34},      {64, kParamIns2B, 48},      {64, kParamIns2C, 42},
  {64, kParamIns2Mix, 18},    {64, kParamMstAlgo, 1},     {64, kParamMstA, 54},
  {64, kParamMstB, 42},       {64, kParamMstC, 24},       {64, kParamMstMix, 24},

  // Family 04 / 004 Drift Organ
  {65, kParamOsc1Shape, 33},  {65, kParamOsc1Level, 80},  {65, kParamOsc2Shape, 40},
  {65, kParamOsc2Pitch, 7},   {65, kParamOsc2Level, 70},  {65, kParamSubLevel, 30},
  {65, kParamUnisonVoices, 2}, {65, kParamUnisonDetune, 18}, {65, kParamFilterCutoff, 5400},
  {65, kParamFilterReso, 12}, {65, kParamAmpAttack, 8},   {65, kParamAmpSustain, 100},
  {65, kParamAmpRelease, 200}, {65, kParamLfoRate, 0.25},  {65, kParamLfoDest, 1},
  {65, kParamLfoAmount, 20},
  {65, kParamIns1Algo, 2},    {65, kParamIns1A, 14},      {65, kParamIns1B, 60},
  {65, kParamIns1C, 70},      {65, kParamIns1Mix, 32},    {65, kParamIns2Algo, 3},
  {65, kParamIns2A, 24},      {65, kParamIns2B, 16},      {65, kParamIns2C, 46},
  {65, kParamIns2Mix, 12},    {65, kParamMstAlgo, 3},     {65, kParamMstA, 62},
  {65, kParamMstB, 28},       {65, kParamMstC, 50},       {65, kParamMstMix, 24},

  // Family 04 / 005 Chime Bells
  {66, kParamOsc1Shape, 8},   {66, kParamOsc2Shape, 20},  {66, kParamOsc2Pitch, 19},
  {66, kParamOsc2Level, 55},  {66, kParamFmAmount, 18},   {66, kParamFilterCutoff, 8600},
  {66, kParamFilterKeyTrack, 45}, {66, kParamAmpAttack, 4},   {66, kParamAmpDecay, 2600},
  {66, kParamAmpSustain, 15}, {66, kParamAmpRelease, 2800}, {66, kParamLfoRate, 0.12},
  {66, kParamLfoDest, 2},     {66, kParamLfoAmount, 8},   {66, kParamSynthMasterLevel, -4},
  {66, kParamIns1Algo, 3},    {66, kParamIns1A, 36},      {66, kParamIns1B, 28},
  {66, kParamIns1C, 72},      {66, kParamIns1Mix, 22},    {66, kParamIns2Algo, 2},
  {66, kParamIns2A, 12},      {66, kParamIns2B, 30},      {66, kParamIns2C, 74},
  {66, kParamIns2Mix, 20},    {66, kParamMstAlgo, 3},     {66, kParamMstA, 66},
  {66, kParamMstB, 26},       {66, kParamMstC, 50},       {66, kParamMstMix, 26},

  // Family 04 / 006 Harp Pluck
  {67, kParamOsc1Shape, 12},  {67, kParamOsc2Shape, 18},  {67, kParamOsc2Pitch, 12},
  {67, kParamOsc2Level, 42},  {67, kParamFmAmount, 6},    {67, kParamFilterCutoff, 6400},
  {67, kParamFilterReso, 8},  {67, kParamFilterEnv, 30},  {67, kParamFilterKeyTrack, 65},
  {67, kParamAmpAttack, 1},   {67, kParamAmpDecay, 500},  {67, kParamAmpSustain, 0},
  {67, kParamAmpRelease, 700},
  {67, kParamIns1Algo, 2},    {67, kParamIns1A, 18},      {67, kParamIns1B, 24},
  {67, kParamIns1C, 58},      {67, kParamIns1Mix, 18},    {67, kParamIns2Algo, 3},
  {67, kParamIns2A, 26},      {67, kParamIns2B, 16},      {67, kParamIns2C, 62},
  {67, kParamIns2Mix, 14},    {67, kParamMstAlgo, 3},     {67, kParamMstA, 60},
  {67, kParamMstB, 30},       {67, kParamMstC, 50},       {67, kParamMstMix, 22},

  // Family 04 / 007 Soft Stab
  {68, kParamOsc1Shape, 30},  {68, kParamOsc2Shape, 48},  {68, kParamOsc2Pitch, 7},
  {68, kParamOsc2Level, 50},  {68, kParamFilterCutoff, 4200}, {68, kParamFilterReso, 14},
  {68, kParamFilterEnv, 40},  {68, kParamAmpAttack, 6},   {68, kParamAmpDecay, 240},
  {68, kParamAmpSustain, 0},  {68, kParamAmpRelease, 280}, {68, kParamUnisonVoices, 2},
  {68, kParamUnisonDetune, 16}, {68, kParamVelToAmp, 40},
  {68, kParamIns1Algo, 2},    {68, kParamIns1A, 12},      {68, kParamIns1B, 30},
  {68, kParamIns1C, 52},      {68, kParamIns1Mix, 16},    {68, kParamIns2Algo, 3},
  {68, kParamIns2A, 18},      {68, kParamIns2B, 14},      {68, kParamIns2C, 50},
  {68, kParamIns2Mix, 12},    {68, kParamMstAlgo, 1},     {68, kParamMstA, 52},
  {68, kParamMstB, 40},       {68, kParamMstC, 22},       {68, kParamMstMix, 24},

  // Family 04 / 008 Sub Bass
  {69, kParamPolyMode, 1},    {69, kParamOsc1Shape, 8},   {69, kParamOsc1Pitch, -12},
  {69, kParamOsc2Shape, 14},  {69, kParamOsc2Pitch, -12}, {69, kParamOsc2Level, 30},
  {69, kParamSubLevel, 90},   {69, kParamFilterCutoff, 480}, {69, kParamFilterReso, 18},
  {69, kParamFilterEnv, 25},  {69, kParamAmpAttack, 8},   {69, kParamAmpDecay, 500},
  {69, kParamAmpSustain, 85}, {69, kParamAmpRelease, 300}, {69, kParamLfoRate, 0.2},
  {69, kParamLfoDest, 1},     {69, kParamLfoAmount, 10},  {69, kParamFilterDrive, 8},
  {69, kParamIns1Algo, 1},    {69, kParamIns1A, 24},      {69, kParamIns1B, 30},
  {69, kParamIns1C, 48},      {69, kParamIns1Mix, 16},    {69, kParamMstAlgo, 1},
  {69, kParamMstA, 62},       {69, kParamMstB, 44},       {69, kParamMstC, 20},
  {69, kParamMstMix, 26},

  // Family 04 / 009 Air Lead
  {70, kParamPolyMode, 1},    {70, kParamPorta, 80},      {70, kParamOsc1Shape, 22},
  {70, kParamOsc2Shape, 36},  {70, kParamOsc2Pitch, 12},  {70, kParamOsc2Level, 48},
  {70, kParamNoiseLevel, 8},  {70, kParamUnisonVoices, 2}, {70, kParamUnisonDetune, 14},
  {70, kParamFilterCutoff, 6200}, {70, kParamFilterReso, 10}, {70, kParamFilterKeyTrack, 25},
  {70, kParamAmpAttack, 30},  {70, kParamAmpDecay, 600},  {70, kParamAmpSustain, 70},
  {70, kParamAmpRelease, 500}, {70, kParamLfoRate, 0.35},  {70, kParamLfoDest, 2},
  {70, kParamLfoAmount, 12},  {70, kParamModWheelDest, 1}, {70, kParamModWheelAmount, 28},
  {70, kParamIns1Algo, 3},    {70, kParamIns1A, 32},      {70, kParamIns1B, 24},
  {70, kParamIns1C, 60},      {70, kParamIns1Mix, 20},    {70, kParamIns2Algo, 2},
  {70, kParamIns2A, 16},      {70, kParamIns2B, 26},      {70, kParamIns2C, 66},
  {70, kParamIns2Mix, 16},    {70, kParamMstAlgo, 3},     {70, kParamMstA, 58},
  {70, kParamMstB, 32},       {70, kParamMstC, 50},       {70, kParamMstMix, 22},

  // Family 04 / 010 Swell Brass
  {71, kParamOsc1Shape, 40},  {71, kParamOsc2Shape, 52},  {71, kParamOsc2Pitch, 7},
  {71, kParamOsc2Level, 72},  {71, kParamUnisonVoices, 4}, {71, kParamUnisonDetune, 26},
  {71, kParamFilterCutoff, 1600}, {71, kParamFilterReso, 16}, {71, kParamFilterEnv, 60},
  {71, kParamFiltAttack, 500}, {71, kParamFiltDecay, 1200}, {71, kParamFiltSustain, 55},
  {71, kParamFiltRelease, 1400}, {71, kParamAmpAttack, 280}, {71, kParamAmpDecay, 1400},
  {71, kParamAmpSustain, 78}, {71, kParamAmpRelease, 900}, {71, kParamLfoRate, 0.15},
  {71, kParamLfoDest, 1},     {71, kParamLfoAmount, 14},  {71, kParamVelToFilter, 35},
  {71, kParamIns1Algo, 2},    {71, kParamIns1A, 10},      {71, kParamIns1B, 40},
  {71, kParamIns1C, 56},      {71, kParamIns1Mix, 24},    {71, kParamIns2Algo, 3},
  {71, kParamIns2A, 40},      {71, kParamIns2B, 22},      {71, kParamIns2C, 48},
  {71, kParamIns2Mix, 14},    {71, kParamMstAlgo, 3},     {71, kParamMstA, 60},
  {71, kParamMstB, 30},       {71, kParamMstC, 50},       {71, kParamMstMix, 24},

  // Family 04 / 011 Bowed Strings
  {72, kParamOsc1Shape, 34},  {72, kParamOsc2Shape, 28},  {72, kParamOsc2Pitch, 12},
  {72, kParamOsc2Level, 78},  {72, kParamUnisonVoices, 6}, {72, kParamUnisonDetune, 34},
  {72, kParamFilterCutoff, 2400}, {72, kParamFilterReso, 8},  {72, kParamFilterEnv, 15},
  {72, kParamAmpAttack, 1100}, {72, kParamAmpDecay, 2600}, {72, kParamAmpSustain, 86},
  {72, kParamAmpRelease, 2600}, {72, kParamLfoRate, 0.08},  {72, kParamLfoDest, 1},
  {72, kParamLfoAmount, 10},  {72, kParamSynthMasterLevel, -3},
  {72, kParamIns1Algo, 2},    {72, kParamIns1A, 8},       {72, kParamIns1B, 50},
  {72, kParamIns1C, 60},      {72, kParamIns1Mix, 30},    {72, kParamIns2Algo, 3},
  {72, kParamIns2A, 46},      {72, kParamIns2B, 26},      {72, kParamIns2C, 46},
  {72, kParamIns2Mix, 16},    {72, kParamMstAlgo, 3},     {72, kParamMstA, 68},
  {72, kParamMstB, 28},       {72, kParamMstC, 50},       {72, kParamMstMix, 24},

  // Family 04 / 012 Air Choir
  {73, kParamOsc1Shape, 28},  {73, kParamOsc2Shape, 45},  {73, kParamOsc2Pitch, 12},
  {73, kParamUnisonVoices, 6}, {73, kParamUnisonDetune, 30}, {73, kParamFilterCutoff, 5500},
  {73, kParamFilterReso, 14}, {73, kParamAmpAttack, 700}, {73, kParamAmpDecay, 2000},
  {73, kParamAmpSustain, 85}, {73, kParamAmpRelease, 2600}, {73, kParamTapeHiss, 18},
  {73, kParamTapeMix, 40},
  {73, kParamIns1Algo, 2},    {73, kParamIns1A, 12},      {73, kParamIns1B, 44},
  {73, kParamIns1C, 68},      {73, kParamIns1Mix, 28},    {73, kParamIns2Algo, 3},
  {73, kParamIns2A, 50},      {73, kParamIns2B, 24},      {73, kParamIns2C, 44},
  {73, kParamIns2Mix, 18},    {73, kParamMstAlgo, 3},     {73, kParamMstA, 66},
  {73, kParamMstB, 26},       {73, kParamMstC, 50},       {73, kParamMstMix, 26},

  // Family 04 / 013 Cloud Pad
  {74, kParamOsc1Shape, 16},  {74, kParamOsc2Shape, 58},  {74, kParamOsc2Pitch, 5},
  {74, kParamOsc2Level, 62},  {74, kParamUnisonVoices, 5}, {74, kParamUnisonDetune, 42},
  {74, kParamFilterCutoff, 2800}, {74, kParamFilterReso, 18}, {74, kParamFilterEnv, 30},
  {74, kParamAmpAttack, 1600}, {74, kParamAmpDecay, 3200}, {74, kParamAmpSustain, 74},
  {74, kParamAmpRelease, 3600}, {74, kParamLfoRate, 0.06},  {74, kParamLfoShape, 1},
  {74, kParamLfoDest, 1},     {74, kParamLfoAmount, 26},  {74, kParamSynthMasterLevel, -4},
  {74, kParamIns1Algo, 2},    {74, kParamIns1A, 12},      {74, kParamIns1B, 62},
  {74, kParamIns1C, 72},      {74, kParamIns1Mix, 36},    {74, kParamIns2Algo, 3},
  {74, kParamIns2A, 58},      {74, kParamIns2B, 34},      {74, kParamIns2C, 52},
  {74, kParamIns2Mix, 22},    {74, kParamMstAlgo, 3},     {74, kParamMstA, 70},
  {74, kParamMstB, 24},       {74, kParamMstC, 50},       {74, kParamMstMix, 30},

  // Family 04 / 014 Sparse Pad
  {75, kParamOsc1Shape, 12},  {75, kParamOsc1Level, 70},  {75, kParamOsc2Shape, 20},
  {75, kParamOsc2Pitch, 12},  {75, kParamOsc2Level, 18},  {75, kParamUnisonVoices, 2},
  {75, kParamUnisonDetune, 10}, {75, kParamFilterCutoff, 4000}, {75, kParamFilterReso, 6},
  {75, kParamAmpAttack, 1500}, {75, kParamAmpDecay, 2000}, {75, kParamAmpSustain, 45},
  {75, kParamAmpRelease, 2400}, {75, kParamTapeMix, 14},    {75, kParamSynthMasterLevel, -6},
  {75, kParamIns1Algo, 2},    {75, kParamIns1A, 8},       {75, kParamIns1B, 18},
  {75, kParamIns1C, 46},      {75, kParamIns1Mix, 12},    {75, kParamIns2Algo, 3},
  {75, kParamIns2A, 42},      {75, kParamIns2B, 16},      {75, kParamIns2C, 40},
  {75, kParamIns2Mix, 10},    {75, kParamMstAlgo, 3},     {75, kParamMstA, 56},
  {75, kParamMstB, 30},       {75, kParamMstC, 50},       {75, kParamMstMix, 16},

  // Family 04 / 015 Motion Pad
  {76, kParamOsc1Shape, 12},  {76, kParamOsc2Shape, 60},  {76, kParamOsc2Pitch, 7},
  {76, kParamUnisonVoices, 5}, {76, kParamUnisonDetune, 55}, {76, kParamFilterCutoff, 2600},
  {76, kParamFilterReso, 20}, {76, kParamFilterEnv, 65},  {76, kParamAmpAttack, 900},
  {76, kParamAmpDecay, 3000}, {76, kParamAmpSustain, 70}, {76, kParamAmpRelease, 3500},
  {76, kParamFiltAttack, 1200}, {76, kParamFiltDecay, 3000}, {76, kParamFiltSustain, 60},
  {76, kParamFiltRelease, 3000}, {76, kParamLfoRate, 0.25},  {76, kParamLfoDest, 1},
  {76, kParamLfoAmount, 40},
  {76, kParamIns1Algo, 2},    {76, kParamIns1A, 24},      {76, kParamIns1B, 58},
  {76, kParamIns1C, 82},      {76, kParamIns1Mix, 35},    {76, kParamIns2Algo, 3},
  {76, kParamIns2A, 60},      {76, kParamIns2B, 38},      {76, kParamIns2C, 48},
  {76, kParamIns2Mix, 24},    {76, kParamMstAlgo, 3},     {76, kParamMstA, 72},
  {76, kParamMstB, 28},       {76, kParamMstC, 50},       {76, kParamMstMix, 55},

  // Family 04 / 016 Tilt Texture
  {77, kParamOsc1Shape, 46},  {77, kParamOsc2Shape, 74},  {77, kParamOsc2Pitch, 7},
  {77, kParamOsc2Level, 58},  {77, kParamOscRing, 1},     {77, kParamNoiseLevel, 10},
  {77, kParamFilterType, 2},  {77, kParamFilterCutoff, 1100}, {77, kParamFilterReso, 36},
  {77, kParamUnisonVoices, 3}, {77, kParamUnisonDetune, 38}, {77, kParamAmpAttack, 400},
  {77, kParamAmpDecay, 2200}, {77, kParamAmpSustain, 58}, {77, kParamAmpRelease, 2000},
  {77, kParamLfoRate, 0.9},   {77, kParamLfoShape, 2},    {77, kParamLfoDest, 2},
  {77, kParamLfoAmount, 18},
  {77, kParamIns1Algo, 4},    {77, kParamIns1A, 28},      {77, kParamIns1B, 22},
  {77, kParamIns1C, 30},      {77, kParamIns1Mix, 16},    {77, kParamIns2Algo, 3},
  {77, kParamIns2A, 40},      {77, kParamIns2B, 30},      {77, kParamIns2C, 44},
  {77, kParamIns2Mix, 18},    {77, kParamMstAlgo, 4},     {77, kParamMstA, 72},
  {77, kParamMstB, 35},       {77, kParamMstC, 20},       {77, kParamMstMix, 48},

  // Family 04 / 017 Slow Drone
  {78, kParamOsc1Shape, 5},   {78, kParamOsc2Shape, 50},  {78, kParamOsc2Pitch, -12},
  {78, kParamSubLevel, 60},   {78, kParamNoiseLevel, 12}, {78, kParamFilterCutoff, 1200},
  {78, kParamFilterReso, 35}, {78, kParamFilterEnv, 20},  {78, kParamAmpAttack, 2000},
  {78, kParamAmpDecay, 4000}, {78, kParamAmpSustain, 95}, {78, kParamAmpRelease, 4000},
  {78, kParamIns1Algo, 2},    {78, kParamIns1A, 6},       {78, kParamIns1B, 34},
  {78, kParamIns1C, 50},      {78, kParamIns1Mix, 20},    {78, kParamIns2Algo, 3},
  {78, kParamIns2A, 64},      {78, kParamIns2B, 36},      {78, kParamIns2C, 30},
  {78, kParamIns2Mix, 16},    {78, kParamMstAlgo, 4},     {78, kParamMstA, 42},
  {78, kParamMstB, 28},       {78, kParamMstC, 10},       {78, kParamMstMix, 22},

  // Family 04 / 018 Seq Arp
  {79, kParamOsc1Shape, 54},  {79, kParamOsc2Shape, 60},  {79, kParamOsc2Pitch, 12},
  {79, kParamOsc2Level, 45},  {79, kParamUnisonVoices, 2}, {79, kParamUnisonDetune, 20},
  {79, kParamFilterCutoff, 5200}, {79, kParamFilterReso, 22}, {79, kParamFilterEnv, 55},
  {79, kParamFilterKeyTrack, 55}, {79, kParamAmpAttack, 1},   {79, kParamAmpDecay, 70},
  {79, kParamAmpSustain, 0},  {79, kParamAmpRelease, 100}, {79, kParamLfoRate, 4},
  {79, kParamLfoShape, 3},    {79, kParamLfoDest, 1},     {79, kParamLfoAmount, 20},
  {79, kParamIns1Algo, 3},    {79, kParamIns1A, 16},      {79, kParamIns1B, 18},
  {79, kParamIns1C, 62},      {79, kParamIns1Mix, 12},    {79, kParamIns2Algo, 2},
  {79, kParamIns2A, 20},      {79, kParamIns2B, 20},      {79, kParamIns2C, 60},
  {79, kParamIns2Mix, 12},    {79, kParamMstAlgo, 3},     {79, kParamMstA, 58},
  {79, kParamMstB, 30},       {79, kParamMstC, 50},       {79, kParamMstMix, 20},

  // Family 04 / 019 Wash Delay
  {80, kParamOsc1Shape, 20},  {80, kParamOsc2Shape, 40},  {80, kParamOsc2Pitch, 7},
  {80, kParamOsc2Level, 55},  {80, kParamFilterCutoff, 3600}, {80, kParamFilterReso, 12},
  {80, kParamAmpAttack, 200}, {80, kParamAmpDecay, 1600}, {80, kParamAmpSustain, 40},
  {80, kParamAmpRelease, 2000}, {80, kParamSynthMasterLevel, -3},
  {80, kParamIns1Algo, 3},    {80, kParamIns1A, 68},      {80, kParamIns1B, 55},
  {80, kParamIns1C, 50},      {80, kParamIns1Mix, 62},    {80, kParamIns2Algo, 2},
  {80, kParamIns2A, 14},      {80, kParamIns2B, 46},      {80, kParamIns2C, 70},
  {80, kParamIns2Mix, 28},    {80, kParamMstAlgo, 3},     {80, kParamMstA, 74},
  {80, kParamMstB, 28},       {80, kParamMstC, 50},       {80, kParamMstMix, 36},

  // Family 04 / 020 Metal Kit
  {81, kParamEngine, 1},      {81, kParamOsc1Shape, 96},  {81, kParamOsc1PW, 22},
  {81, kParamOsc2Shape, 88},  {81, kParamOsc2Pitch, 7},   {81, kParamOsc2Level, 55},
  {81, kParamNoiseLevel, 50}, {81, kParamFilterType, 3},  {81, kParamFilterCutoff, 4000},
  {81, kParamFilterReso, 18}, {81, kParamAmpAttack, 0.5}, {81, kParamAmpDecay, 200},
  {81, kParamAmpSustain, 0},  {81, kParamAmpRelease, 180}, {81, kParamPitchAmount, -6},
  {81, kParamPitchDecay, 40}, {81, kParamPitchRelease, 40}, {81, kParamDrumSlotTune, -3},
  {81, kParamDrumSlotDecay, 62}, {81, kParamDrumSlotDrive, 35}, {81, kParamDrumSlotFilter, 16000},
  {81, kParamDrumKitLevel, -2}, {81, kParamFilterDrive, 22},
  {81, kParamIns1Algo, 4},    {81, kParamIns1A, 24},      {81, kParamIns1B, 20},
  {81, kParamIns1C, 34},      {81, kParamIns1Mix, 18},    {81, kParamIns2Algo, 1},
  {81, kParamIns2A, 46},      {81, kParamIns2B, 56},      {81, kParamIns2C, 40},
  {81, kParamIns2Mix, 22},    {81, kParamMstAlgo, 2},     {81, kParamMstA, 32},
  {81, kParamMstB, 44},       {81, kParamMstC, 18},       {81, kParamMstMix, 48},

  // Family 05 / 001 Stage Keys
  {82, kParamOsc1Shape, 24},  {82, kParamOsc2Shape, 16},  {82, kParamOsc2Pitch, 12},
  {82, kParamOsc2Level, 42},  {82, kParamFilterCutoff, 7000}, {82, kParamFilterReso, 6},
  {82, kParamFilterEnv, 16},  {82, kParamAmpAttack, 5},   {82, kParamAmpDecay, 850},
  {82, kParamAmpSustain, 36}, {82, kParamAmpRelease, 550}, {82, kParamVelToAmp, 32},
  {82, kParamTapeSat, 16},    {82, kParamTapeMix, 18},
  {82, kParamIns1Algo, 2},    {82, kParamIns1A, 11},      {82, kParamIns1B, 18},
  {82, kParamIns1C, 40},      {82, kParamIns1Mix, 14},    {82, kParamMstAlgo, 1},
  {82, kParamMstA, 60},       {82, kParamMstB, 36},       {82, kParamMstC, 34},
  {82, kParamMstMix, 28},

  // Family 05 / 002 Reed Piano
  {83, kParamOsc1Shape, 30},  {83, kParamOsc2Shape, 24},  {83, kParamOsc2Pitch, 19},
  {83, kParamOsc2Level, 28},  {83, kParamFmAmount, 14},   {83, kParamFilterCutoff, 3000},
  {83, kParamFilterReso, 9},  {83, kParamFilterEnv, 16},  {83, kParamAmpAttack, 5},
  {83, kParamAmpDecay, 1000}, {83, kParamAmpSustain, 32}, {83, kParamAmpRelease, 640},
  {83, kParamVelToAmp, 48},   {83, kParamVelToFilter, 18}, {83, kParamTapeMix, 20},
  {83, kParamIns1Algo, 2},    {83, kParamIns1A, 9},       {83, kParamIns1B, 14},
  {83, kParamIns1C, 36},      {83, kParamIns1Mix, 10},    {83, kParamMstAlgo, 4},
  {83, kParamMstA, 58},       {83, kParamMstB, 42},       {83, kParamMstC, 12},
  {83, kParamMstMix, 18},

  // Family 05 / 003 Funk Clav
  {84, kParamOsc1Shape, 80},  {84, kParamOsc1PW, 26},     {84, kParamOsc2Shape, 86},
  {84, kParamOsc2Pitch, 12},  {84, kParamOsc2Level, 58},  {84, kParamOsc2PW, 20},
  {84, kParamFilterCutoff, 2500}, {84, kParamFilterReso, 36}, {84, kParamFilterEnv, 66},
  {84, kParamFilterDrive, 16}, {84, kParamFilterKeyTrack, 28}, {84, kParamFiltAttack, 1},
  {84, kParamFiltDecay, 130}, {84, kParamFiltSustain, 0}, {84, kParamFiltRelease, 110},
  {84, kParamAmpAttack, 1},   {84, kParamAmpDecay, 190},  {84, kParamAmpSustain, 0},
  {84, kParamAmpRelease, 140}, {84, kParamVelToAmp, 72},   {84, kParamTapeSat, 18},
  {84, kParamTapeMix, 14},
  {84, kParamIns1Algo, 1},    {84, kParamIns1A, 34},      {84, kParamIns1B, 46},
  {84, kParamIns1C, 42},      {84, kParamIns1Mix, 22},    {84, kParamMstAlgo, 1},
  {84, kParamMstA, 56},       {84, kParamMstB, 44},       {84, kParamMstC, 24},
  {84, kParamMstMix, 26},

  // Family 05 / 004 Rock Organ
  {85, kParamOsc1Shape, 36},  {85, kParamOsc1Level, 100}, {85, kParamOsc2Shape, 33},
  {85, kParamOsc2Pitch, 12},  {85, kParamOsc2Level, 78},  {85, kParamSubLevel, 28},
  {85, kParamFilterCutoff, 6800}, {85, kParamFilterReso, 14}, {85, kParamFilterEnv, 35},
  {85, kParamFiltAttack, 1},  {85, kParamFiltDecay, 90},  {85, kParamFiltSustain, 30},
  {85, kParamFiltRelease, 80}, {85, kParamAmpAttack, 2},   {85, kParamAmpDecay, 40},
  {85, kParamAmpSustain, 100}, {85, kParamAmpRelease, 70}, {85, kParamFilterDrive, 18},
  {85, kParamLfoRate, 6},     {85, kParamLfoDest, 1},     {85, kParamLfoAmount, 6},
  {85, kParamIns1Algo, 1},    {85, kParamIns1A, 42},      {85, kParamIns1B, 38},
  {85, kParamIns1C, 46},      {85, kParamIns1Mix, 20},    {85, kParamMstAlgo, 1},
  {85, kParamMstA, 54},       {85, kParamMstB, 40},       {85, kParamMstC, 32},
  {85, kParamMstMix, 26},

  // Family 05 / 005 Ping Bells
  {86, kParamOsc1Shape, 9},   {86, kParamOsc2Shape, 14},  {86, kParamOsc2Pitch, 24},
  {86, kParamOsc2Level, 36},  {86, kParamFmAmount, 20},   {86, kParamFilterCutoff, 12000},
  {86, kParamFilterKeyTrack, 70}, {86, kParamAmpAttack, 1},   {86, kParamAmpDecay, 900},
  {86, kParamAmpSustain, 0},  {86, kParamAmpRelease, 1100}, {86, kParamSynthMasterLevel, -3},
  {86, kParamIns1Algo, 3},    {86, kParamIns1A, 12},      {86, kParamIns1B, 8},
  {86, kParamIns1C, 80},      {86, kParamIns1Mix, 8},     {86, kParamMstAlgo, 3},
  {86, kParamMstA, 60},       {86, kParamMstB, 28},       {86, kParamMstC, 50},
  {86, kParamMstMix, 20},

  // Family 05 / 006 Mute Pluck
  {87, kParamOsc1Shape, 52},  {87, kParamOsc1PW, 38},     {87, kParamOsc2Level, 10},
  {87, kParamFilterCutoff, 1800}, {87, kParamFilterReso, 12}, {87, kParamFilterEnv, 48},
  {87, kParamFilterKeyTrack, 80}, {87, kParamFiltAttack, 1},  {87, kParamFiltDecay, 60},
  {87, kParamFiltSustain, 0}, {87, kParamFiltRelease, 50}, {87, kParamAmpAttack, 1},
  {87, kParamAmpDecay, 90},   {87, kParamAmpSustain, 0},  {87, kParamAmpRelease, 70},
  {87, kParamTapeMix, 8},
  {87, kParamIns1Algo, 1},    {87, kParamIns1A, 20},      {87, kParamIns1B, 44},
  {87, kParamIns1C, 50},      {87, kParamIns1Mix, 10},    {87, kParamMstAlgo, 1},
  {87, kParamMstA, 58},       {87, kParamMstB, 30},       {87, kParamMstC, 22},
  {87, kParamMstMix, 18},

  // Family 05 / 007 Short Stab
  {88, kParamOsc1Shape, 58},  {88, kParamOsc2Shape, 62},  {88, kParamOsc2Pitch, 12},
  {88, kParamOsc2Level, 48},  {88, kParamFilterCutoff, 4600}, {88, kParamFilterReso, 20},
  {88, kParamFilterEnv, 58},  {88, kParamAmpAttack, 1},   {88, kParamAmpDecay, 80},
  {88, kParamAmpSustain, 0},  {88, kParamAmpRelease, 60}, {88, kParamVelToAmp, 68},
  {88, kParamVelToFilter, 50}, {88, kParamFilterDrive, 14}, {88, kParamTapeSat, 12},
  {88, kParamTapeMix, 10},
  {88, kParamIns1Algo, 1},    {88, kParamIns1A, 28},      {88, kParamIns1B, 48},
  {88, kParamIns1C, 44},      {88, kParamIns1Mix, 16},    {88, kParamMstAlgo, 1},
  {88, kParamMstA, 52},       {88, kParamMstB, 46},       {88, kParamMstC, 20},
  {88, kParamMstMix, 26},

  // Family 05 / 008 Round Bass
  {89, kParamPolyMode, 1},    {89, kParamMonoLegato, 1},  {89, kParamOsc1Shape, 18},
  {89, kParamOsc1Pitch, -12}, {89, kParamOsc2Shape, 12},  {89, kParamOsc2Pitch, -12},
  {89, kParamOsc2Level, 35},  {89, kParamSubLevel, 65},   {89, kParamFilterCutoff, 980},
  {89, kParamFilterReso, 16}, {89, kParamFilterEnv, 32},  {89, kParamAmpAttack, 3},
  {89, kParamAmpDecay, 340},  {89, kParamAmpSustain, 78}, {89, kParamAmpRelease, 200},
  {89, kParamVelToAmp, 36},   {89, kParamTapeSat, 14},    {89, kParamTapeMix, 12},
  {89, kParamIns1Algo, 1},    {89, kParamIns1A, 24},      {89, kParamIns1B, 55},
  {89, kParamIns1C, 48},      {89, kParamIns1Mix, 18},    {89, kParamMstAlgo, 1},
  {89, kParamMstA, 62},       {89, kParamMstB, 40},       {89, kParamMstC, 22},
  {89, kParamMstMix, 26},

  // Family 05 / 009 Saw Lead
  {90, kParamEngine, 1},      {90, kParamPolyMode, 1},    {90, kParamPorta, 40},
  {90, kParamOsc1Shape, 70},  {90, kParamOsc2Shape, 66},  {90, kParamOsc2Pitch, 0.01},
  {90, kParamOsc2Level, 60},  {90, kParamUnisonVoices, 2}, {90, kParamUnisonDetune, 8},
  {90, kParamFilterCutoff, 3900}, {90, kParamFilterReso, 22}, {90, kParamFilterEnv, 30},
  {90, kParamFilterDrive, 20}, {90, kParamFilterKeyTrack, 35}, {90, kParamAmpAttack, 8},
  {90, kParamAmpDecay, 240},  {90, kParamAmpSustain, 82}, {90, kParamAmpRelease, 180},
  {90, kParamModWheelDest, 1}, {90, kParamModWheelAmount, 32},
  {90, kParamIns1Algo, 1},    {90, kParamIns1A, 46},      {90, kParamIns1B, 40},
  {90, kParamIns1C, 38},      {90, kParamIns1Mix, 24},    {90, kParamMstAlgo, 1},
  {90, kParamMstA, 56},       {90, kParamMstB, 38},       {90, kParamMstC, 30},
  {90, kParamMstMix, 24},

  // Family 05 / 010 Section Brass
  {91, kParamOsc1Shape, 50},  {91, kParamOsc2Shape, 54},  {91, kParamOsc2Pitch, 12},
  {91, kParamOsc2Level, 64},  {91, kParamUnisonVoices, 2}, {91, kParamUnisonDetune, 10},
  {91, kParamFilterCutoff, 2200}, {91, kParamFilterReso, 12}, {91, kParamFilterEnv, 52},
  {91, kParamFiltAttack, 25}, {91, kParamFiltDecay, 220}, {91, kParamFiltSustain, 50},
  {91, kParamFiltRelease, 300}, {91, kParamAmpAttack, 22},  {91, kParamAmpDecay, 400},
  {91, kParamAmpSustain, 74}, {91, kParamAmpRelease, 280}, {91, kParamVelToFilter, 42},
  {91, kParamIns1Algo, 2},    {91, kParamIns1A, 8},       {91, kParamIns1B, 16},
  {91, kParamIns1C, 44},      {91, kParamIns1Mix, 12},    {91, kParamMstAlgo, 1},
  {91, kParamMstA, 56},       {91, kParamMstB, 42},       {91, kParamMstC, 38},
  {91, kParamMstMix, 24},

  // Family 05 / 011 Small Strings
  {92, kParamOsc1Shape, 38},  {92, kParamOsc2Shape, 34},  {92, kParamOsc2Pitch, 12},
  {92, kParamOsc2Level, 55},  {92, kParamUnisonVoices, 2}, {92, kParamUnisonDetune, 9},
  {92, kParamFilterCutoff, 3400}, {92, kParamFilterReso, 5},  {92, kParamAmpAttack, 240},
  {92, kParamAmpDecay, 900},  {92, kParamAmpSustain, 72}, {92, kParamAmpRelease, 800},
  {92, kParamTapeMix, 16},    {92, kParamSynthMasterLevel, -2},
  {92, kParamIns1Algo, 2},    {92, kParamIns1A, 7},       {92, kParamIns1B, 20},
  {92, kParamIns1C, 42},      {92, kParamIns1Mix, 14},    {92, kParamMstAlgo, 3},
  {92, kParamMstA, 58},       {92, kParamMstB, 30},       {92, kParamMstC, 50},
  {92, kParamMstMix, 20},

  // Family 05 / 012 Close Choir
  {93, kParamOsc1Shape, 21},  {93, kParamOsc2Shape, 27},  {93, kParamOsc2Pitch, 12},
  {93, kParamOsc2Level, 40},  {93, kParamNoiseLevel, 4},  {93, kParamUnisonVoices, 2},
  {93, kParamUnisonDetune, 8}, {93, kParamFilterCutoff, 4200}, {93, kParamFilterReso, 7},
  {93, kParamAmpAttack, 280}, {93, kParamAmpDecay, 1200}, {93, kParamAmpSustain, 80},
  {93, kParamAmpRelease, 900}, {93, kParamTapeHiss, 4},    {93, kParamTapeMix, 18},
  {93, kParamIns1Algo, 2},    {93, kParamIns1A, 6},       {93, kParamIns1B, 14},
  {93, kParamIns1C, 40},      {93, kParamIns1Mix, 10},    {93, kParamMstAlgo, 3},
  {93, kParamMstA, 56},       {93, kParamMstB, 30},       {93, kParamMstC, 50},
  {93, kParamMstMix, 18},

  // Family 05 / 013 Tape Pad
  {94, kParamOsc1Shape, 26},  {94, kParamOsc2Shape, 32},  {94, kParamOsc2Pitch, 7},
  {94, kParamOsc2Level, 50},  {94, kParamUnisonVoices, 2}, {94, kParamUnisonDetune, 12},
  {94, kParamFilterCutoff, 2600}, {94, kParamFilterReso, 10}, {94, kParamAmpAttack, 600},
  {94, kParamAmpDecay, 1800}, {94, kParamAmpSustain, 66}, {94, kParamAmpRelease, 2000},
  {94, kParamTapeSat, 32},    {94, kParamTapeBump, 28},   {94, kParamTapeWow, 18},
  {94, kParamTapeFlutter, 14}, {94, kParamTapeMix, 42},
  {94, kParamIns1Algo, 3},    {94, kParamIns1A, 30},      {94, kParamIns1B, 20},
  {94, kParamIns1C, 46},      {94, kParamIns1Mix, 14},    {94, kParamMstAlgo, 3},
  {94, kParamMstA, 62},       {94, kParamMstB, 28},       {94, kParamMstC, 50},
  {94, kParamMstMix, 22},

  // Family 05 / 014 Filter Motion
  {95, kParamOsc1Shape, 48},  {95, kParamOsc2Shape, 56},  {95, kParamOsc2Pitch, 5},
  {95, kParamOsc2Level, 60},  {95, kParamFilterCutoff, 700}, {95, kParamFilterReso, 28},
  {95, kParamFilterEnv, 70},  {95, kParamFiltAttack, 800}, {95, kParamFiltDecay, 1600},
  {95, kParamFiltSustain, 20}, {95, kParamFiltRelease, 1400}, {95, kParamAmpAttack, 120},
  {95, kParamAmpDecay, 2000}, {95, kParamAmpSustain, 64}, {95, kParamAmpRelease, 1800},
  {95, kParamLfoRate, 0.16},  {95, kParamLfoShape, 2},    {95, kParamLfoDest, 1},
  {95, kParamLfoAmount, 36},
  {95, kParamIns1Algo, 2},    {95, kParamIns1A, 18},      {95, kParamIns1B, 28},
  {95, kParamIns1C, 52},      {95, kParamIns1Mix, 18},    {95, kParamMstAlgo, 3},
  {95, kParamMstA, 60},       {95, kParamMstB, 30},       {95, kParamMstC, 50},
  {95, kParamMstMix, 24},

  // Family 05 / 015 Broken Texture
  {96, kParamEngine, 1},      {96, kParamOsc1Shape, 62},  {96, kParamOsc2Shape, 96},
  {96, kParamOsc2Pitch, 6},   {96, kParamOsc2Level, 44},  {96, kParamOscRing, 1},
  {96, kParamNoiseLevel, 18}, {96, kParamFilterType, 3},  {96, kParamFilterCutoff, 2400},
  {96, kParamFilterReso, 24}, {96, kParamAmpAttack, 40},  {96, kParamAmpDecay, 900},
  {96, kParamAmpSustain, 40}, {96, kParamAmpRelease, 700}, {96, kParamLfoRate, 7},
  {96, kParamLfoShape, 4},    {96, kParamLfoDest, 3},     {96, kParamLfoAmount, 28},
  {96, kParamIns1Algo, 4},    {96, kParamIns1A, 40},      {96, kParamIns1B, 30},
  {96, kParamIns1C, 36},      {96, kParamIns1Mix, 22},    {96, kParamMstAlgo, 4},
  {96, kParamMstA, 22},       {96, kParamMstB, 55},       {96, kParamMstC, 16},
  {96, kParamMstMix, 26},

  // Family 05 / 016 Dark Drone
  {97, kParamOsc1Shape, 4},   {97, kParamOsc2Shape, 10},  {97, kParamOsc2Pitch, -24},
  {97, kParamOsc2Level, 55},  {97, kParamSubLevel, 75},   {97, kParamNoiseLevel, 8},
  {97, kParamFilterCutoff, 420}, {97, kParamFilterReso, 30}, {97, kParamFilterEnv, 6},
  {97, kParamAmpAttack, 2400}, {97, kParamAmpDecay, 4000}, {97, kParamAmpSustain, 98},
  {97, kParamAmpRelease, 4000}, {97, kParamLfoRate, 0.04},  {97, kParamLfoDest, 1},
  {97, kParamLfoAmount, 12},  {97, kParamFilterDrive, 10}, {97, kParamSynthMasterLevel, -8},
  {97, kParamTapeSat, 20},    {97, kParamTapeMix, 24},
  {97, kParamIns1Algo, 2},    {97, kParamIns1A, 5},       {97, kParamIns1B, 24},
  {97, kParamIns1C, 38},      {97, kParamIns1Mix, 16},    {97, kParamMstAlgo, 4},
  {97, kParamMstA, 34},       {97, kParamMstB, 24},       {97, kParamMstC, 10},
  {97, kParamMstMix, 20},

  // Family 05 / 017 Tight Arp
  {98, kParamOsc1Shape, 60},  {98, kParamOsc2Shape, 48},  {98, kParamOsc2Pitch, 12},
  {98, kParamOsc2Level, 30},  {98, kParamFilterCutoff, 4400}, {98, kParamFilterReso, 18},
  {98, kParamFilterEnv, 62},  {98, kParamFilterKeyTrack, 72}, {98, kParamAmpAttack, 1},
  {98, kParamAmpDecay, 55},   {98, kParamAmpSustain, 0},  {98, kParamAmpRelease, 70},
  {98, kParamFiltAttack, 1},  {98, kParamFiltDecay, 40},  {98, kParamFiltSustain, 0},
  {98, kParamFiltRelease, 40}, {98, kParamTapeMix, 8},
  {98, kParamIns1Algo, 3},    {98, kParamIns1A, 10},      {98, kParamIns1B, 8},
  {98, kParamIns1C, 56},      {98, kParamIns1Mix, 6},     {98, kParamMstAlgo, 1},
  {98, kParamMstA, 52},       {98, kParamMstB, 30},       {98, kParamMstC, 20},
  {98, kParamMstMix, 16},

  // Family 05 / 018 Room Crush
  {99, kParamEngine, 1},      {99, kParamOsc1Shape, 66},  {99, kParamOsc2Shape, 74},
  {99, kParamOsc2Pitch, -12}, {99, kParamOsc2Level, 50},  {99, kParamFilterCutoff, 2800},
  {99, kParamFilterReso, 18}, {99, kParamFilterDrive, 36}, {99, kParamAmpAttack, 2},
  {99, kParamAmpDecay, 320},  {99, kParamAmpSustain, 25}, {99, kParamAmpRelease, 240},
  {99, kParamOutLevel, -2},   {99, kParamTapeSat, 22},    {99, kParamTapeMix, 12},
  {99, kParamIns1Algo, 4},    {99, kParamIns1A, 58},      {99, kParamIns1B, 46},
  {99, kParamIns1C, 32},      {99, kParamIns1Mix, 55},    {99, kParamMstAlgo, 2},
  {99, kParamMstA, 32},       {99, kParamMstB, 48},       {99, kParamMstC, 22},
  {99, kParamMstMix, 50},

  // Family 05 / 019 Dry Kit
  {100, kParamOsc1Shape, 92}, {100, kParamOsc1PW, 40},    {100, kParamOsc2Shape, 88},
  {100, kParamOsc2Level, 60}, {100, kParamSubLevel, 35},  {100, kParamNoiseLevel, 30},
  {100, kParamFilterCutoff, 7000}, {100, kParamFilterReso, 15}, {100, kParamFilterEnv, 40},
  {100, kParamAmpAttack, 0.5}, {100, kParamAmpDecay, 110}, {100, kParamAmpSustain, 0},
  {100, kParamAmpRelease, 90}, {100, kParamPitchAmount, -12}, {100, kParamPitchDecay, 70},
  {100, kParamTapeSat, 15},   {100, kParamTapeMix, 15},   {100, kParamDrumSlotDecay, 42},
  {100, kParamDrumSlotDrive, 22}, {100, kParamDrumSlotFilter, 10000}, {100, kParamDrumKitLevel, -1},
  {100, kParamIns1Algo, 1},   {100, kParamIns1A, 28},     {100, kParamIns1B, 50},
  {100, kParamIns1C, 46},     {100, kParamIns1Mix, 20},   {100, kParamMstAlgo, 2},
  {100, kParamMstA, 38},      {100, kParamMstB, 40},      {100, kParamMstC, 14},
  {100, kParamMstMix, 42},

  // Family 05 / 020 Hard Kit
  {101, kParamEngine, 1},     {101, kParamOsc1Shape, 100}, {101, kParamOsc1PW, 12},
  {101, kParamOsc1Level, 100}, {101, kParamOsc2Shape, 100}, {101, kParamOsc2Pitch, -12},
  {101, kParamOsc2Level, 85}, {101, kParamNoiseLevel, 45}, {101, kParamFilterCutoff, 12000},
  {101, kParamFilterReso, 10}, {101, kParamFilterEnv, 0},  {101, kParamAmpAttack, 0.5},
  {101, kParamAmpDecay, 160}, {101, kParamAmpSustain, 0}, {101, kParamAmpRelease, 120},
  {101, kParamPitchAmount, -18}, {101, kParamPitchAttack, 0.5}, {101, kParamPitchDecay, 90},
  {101, kParamPitchSustain, 0}, {101, kParamPitchRelease, 90}, {101, kParamTapeSat, 45},
  {101, kParamTapeMix, 12},   {101, kParamOutLevel, -1.5}, {101, kParamDrumSlotTune, -2},
  {101, kParamDrumSlotDecay, 26}, {101, kParamDrumSlotDrive, 78}, {101, kParamDrumSlotFilter, 17000},
  {101, kParamIns1Algo, 1},   {101, kParamIns1A, 62},     {101, kParamIns1B, 68},
  {101, kParamIns1C, 54},     {101, kParamIns1Mix, 42},   {101, kParamMstAlgo, 1},
  {101, kParamMstA, 38},      {101, kParamMstB, 65},      {101, kParamMstC, 32},
  {101, kParamMstMix, 70},

  // Family 05 / 021 Tape Kit
  {102, kParamOsc1Shape, 100}, {102, kParamOsc1PW, 50},    {102, kParamOsc2Shape, 55},
  {102, kParamOsc2Pitch, -24}, {102, kParamOsc2Level, 70}, {102, kParamNoiseLevel, 60},
  {102, kParamFilterCutoff, 5000}, {102, kParamFilterReso, 20}, {102, kParamFilterEnv, 60},
  {102, kParamAmpAttack, 0.5}, {102, kParamAmpDecay, 240}, {102, kParamAmpSustain, 0},
  {102, kParamAmpRelease, 260}, {102, kParamPitchAmount, -24}, {102, kParamPitchDecay, 140},
  {102, kParamTapeSat, 60},   {102, kParamTapeBump, 45},  {102, kParamTapeWow, 25},
  {102, kParamTapeFlutter, 30}, {102, kParamTapeMix, 55},   {102, kParamFilterDrive, 45},
  {102, kParamDrumSlotDecay, 75}, {102, kParamDrumSlotDrive, 50}, {102, kParamDrumSlotFilter, 5500},
  {102, kParamDrumKitLevel, -3},
  {102, kParamIns1Algo, 1},   {102, kParamIns1A, 48},     {102, kParamIns1B, 36},
  {102, kParamIns1C, 52},     {102, kParamIns1Mix, 30},   {102, kParamMstAlgo, 2},
  {102, kParamMstA, 30},      {102, kParamMstB, 44},      {102, kParamMstC, 20},
  {102, kParamMstMix, 45},
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
static_assert(kParamMod1Source == 89 && kParamMod4Amount == 100 && kNumParams == 101,
              "U15 appends 89..100");

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
  GetParam(kParamAmpDecay)->InitDouble("Synth Amp Decay", 200., 1., 10000., 0.01, "ms",
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
  GetParam(kParamFilterKeyTrack)->InitDouble("Synth Filter Key Track", 0., 0., 100., 0.1, "%");
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
