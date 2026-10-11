// Main Keith iPlug2 plugin implementation: factory presets, parameter setup, state I/O, and host callbacks.
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


  // Family 01 / 024 Bright Reed. Analysis match of the harmonica reference.
  {23, kParamOsc1Shape, 73.88},   {23, kParamOsc1PW, 33.68},  {23, kParamOsc1Level, 72.32},
  {23, kParamOsc2Shape, 61.27},   {23, kParamOsc2Pitch, 12},  {23, kParamOsc2Level, 17.79},
  {23, kParamNoiseLevel, 0.77},   {23, kParamFmAmount, 5.34},
  {23, kParamFilterCutoff, 15137}, {23, kParamFilterReso, 22.34}, {23, kParamFilterEnv, 0},
  {23, kParamFilterDrive, 14.12}, {23, kParamDirectLevel, 30.71},
  {23, kParamAmpAttack, 67.49},   {23, kParamAmpDecay, 4346},
  {23, kParamAmpSustain, 0},      {23, kParamAmpRelease, 40},
  {23, kParamStereoDelay, 2.94},  {23, kParamStereoSpread, 5.54},
  {23, kParamSynthMasterLevel, -12}, {23, kParamTapeBypass, 1},
  {23, kParamDrumKitLevel, -60},
  {23, kParamIns1Algo, 1},        {23, kParamIns1A, 14},      {23, kParamIns1B, 10},
  {23, kParamIns1C, 12},          {23, kParamIns1Mix, 5},
  {23, kParamMstAlgo, 4},         {23, kParamMstA, 40},       {23, kParamMstB, 18},
  {23, kParamMstC, 8},            {23, kParamMstMix, 12},

  // Family 01 / 025 Snap Bass. Analysis match of the short bass reference at C2.
  {24, kParamOsc1Shape, 75},      {24, kParamOsc1Level, 75},  {24, kParamOsc2Level, 0},
  {24, kParamNoiseLevel, 36},
  {24, kParamFilterCutoff, 600},  {24, kParamFilterReso, 4},  {24, kParamFilterEnv, 0},
  {24, kParamAmpAttack, 8},       {24, kParamAmpDecay, 3400},
  {24, kParamAmpSustain, 0},      {24, kParamAmpRelease, 30},
  {24, kParamStereoDelay, 1.8},   {24, kParamStereoSpread, -100},
  {24, kParamSynthMasterLevel, -10}, {24, kParamTapeBypass, 1},
  {24, kParamDrumKitLevel, -60},
  {24, kParamIns1Algo, 2},        {24, kParamIns1A, 8},       {24, kParamIns1B, 12},
  {24, kParamIns1C, 16},          {24, kParamIns1Mix, 5},
  {24, kParamMstAlgo, 1},         {24, kParamMstA, 28},       {24, kParamMstB, 16},
  {24, kParamMstC, 10},           {24, kParamMstMix, 12},

  // Family 01 / 026 Short Bass. Analysis match of the short bass reference at C3.
  {25, kParamOsc1Shape, 26},      {25, kParamOsc1Level, 89},  {25, kParamOsc2Level, 0},
  {25, kParamSubLevel, 15},       {25, kParamNoiseLevel, 42}, {25, kParamDirectLevel, 20},
  {25, kParamFilterType, 1},      {25, kParamFilterCutoff, 430}, {25, kParamFilterReso, 18},
  {25, kParamFilterEnv, 0},
  {25, kParamAmpAttack, 8},       {25, kParamAmpDecay, 2500},
  {25, kParamAmpSustain, 0},      {25, kParamAmpRelease, 30},
  {25, kParamStereoDelay, 2.5},   {25, kParamStereoSpread, 60},
  {25, kParamSynthMasterLevel, -10}, {25, kParamTapeBypass, 1},
  {25, kParamDrumKitLevel, -60},
  {25, kParamIns1Algo, 1},        {25, kParamIns1A, 22},      {25, kParamIns1B, 18},
  {25, kParamIns1C, 14},          {25, kParamIns1Mix, 5},
  {25, kParamMstAlgo, 1},         {25, kParamMstA, 36},       {25, kParamMstB, 22},
  {25, kParamMstC, 14},           {25, kParamMstMix, 12},

  // Family 02 / 001 Neon Keys
  {26, kParamEngine, 1},      {26, kParamOsc1Shape, 36},  {26, kParamOsc1Level, 88},
  {26, kParamOsc2Shape, 58},  {26, kParamOsc2Pitch, 12},  {26, kParamOsc2Level, 62},
  {26, kParamFilterCutoff, 7800}, {26, kParamFilterReso, 16}, {26, kParamFilterEnv, 28},
  {26, kParamFilterDrive, 24}, {26, kParamAmpAttack, 4},   {26, kParamAmpDecay, 700},
  {26, kParamAmpSustain, 42}, {26, kParamAmpRelease, 420}, {26, kParamUnisonVoices, 2},
  {26, kParamUnisonDetune, 26}, {26, kParamVelToAmp, 55},   {26, kParamTapeSat, 38},
  {26, kParamTapeMix, 22},
  {26, kParamIns1Algo, 1},    {26, kParamIns1A, 48},      {26, kParamIns1B, 42},
  {26, kParamIns1C, 38},      {26, kParamIns1Mix, 30},    {26, kParamMstAlgo, 1},
  {26, kParamMstA, 56},       {26, kParamMstB, 36},       {26, kParamMstC, 32},
  {26, kParamMstMix, 26},

  // Family 02 / 002 Thin Piano
  {27, kParamEngine, 1},      {27, kParamOsc1Shape, 16},  {27, kParamOsc2Shape, 28},
  {27, kParamOsc2Pitch, 12},  {27, kParamOsc2Level, 18},  {27, kParamFmAmount, 8},
  {27, kParamFilterCutoff, 2600}, {27, kParamFilterReso, 8},  {27, kParamFilterEnv, 18},
  {27, kParamAmpAttack, 4},   {27, kParamAmpDecay, 700},  {27, kParamAmpSustain, 22},
  {27, kParamAmpRelease, 480}, {27, kParamVelToAmp, 62},   {27, kParamVelToFilter, 20},
  {27, kParamTapeSat, 14},    {27, kParamTapeMix, 16},    {27, kParamSynthMasterLevel, -2},
  {27, kParamIns1Algo, 2},    {27, kParamIns1A, 12},      {27, kParamIns1B, 18},
  {27, kParamIns1C, 35},      {27, kParamIns1Mix, 12},    {27, kParamMstAlgo, 4},
  {27, kParamMstA, 58},       {27, kParamMstB, 46},       {27, kParamMstC, 10},
  {27, kParamMstMix, 16},

  // Family 02 / 003 Thin Clav
  {28, kParamEngine, 1},      {28, kParamOsc1Shape, 82},  {28, kParamOsc1PW, 18},
  {28, kParamOsc2Shape, 94},  {28, kParamOsc2Pitch, 12},  {28, kParamOsc2Level, 28},
  {28, kParamOsc2PW, 16},     {28, kParamFilterCutoff, 4100}, {28, kParamFilterReso, 38},
  {28, kParamFilterEnv, 70},  {28, kParamFilterDrive, 30}, {28, kParamFilterKeyTrack, 48},
  {28, kParamFiltAttack, 1},  {28, kParamFiltDecay, 120}, {28, kParamFiltSustain, 0},
  {28, kParamFiltRelease, 100}, {28, kParamAmpAttack, 1},   {28, kParamAmpDecay, 180},
  {28, kParamAmpSustain, 0},  {28, kParamAmpRelease, 130}, {28, kParamVelToAmp, 70},
  {28, kParamIns1Algo, 1},    {28, kParamIns1A, 55},      {28, kParamIns1B, 40},
  {28, kParamIns1C, 32},      {28, kParamIns1Mix, 34},    {28, kParamMstAlgo, 1},
  {28, kParamMstA, 54},       {28, kParamMstB, 46},       {28, kParamMstC, 22},
  {28, kParamMstMix, 28},

  // Family 02 / 004 Perc Organ
  {29, kParamEngine, 1},      {29, kParamOsc1Shape, 40},  {29, kParamOsc1Level, 95},
  {29, kParamOsc2Shape, 33},  {29, kParamOsc2Pitch, 19},  {29, kParamOsc2Level, 55},
  {29, kParamSubLevel, 25},   {29, kParamFilterCutoff, 6200}, {29, kParamFilterReso, 18},
  {29, kParamFilterEnv, 45},  {29, kParamAmpAttack, 1},   {29, kParamAmpDecay, 180},
  {29, kParamAmpSustain, 85}, {29, kParamAmpRelease, 70}, {29, kParamFiltAttack, 1},
  {29, kParamFiltDecay, 140}, {29, kParamFiltSustain, 40}, {29, kParamFiltRelease, 120},
  {29, kParamVelToAmp, 35},   {29, kParamLfoRate, 6.5},   {29, kParamLfoDest, 1},
  {29, kParamLfoAmount, 12},
  {29, kParamIns1Algo, 1},    {29, kParamIns1A, 36},      {29, kParamIns1B, 48},
  {29, kParamIns1C, 42},      {29, kParamIns1Mix, 22},    {29, kParamMstAlgo, 1},
  {29, kParamMstA, 52},       {29, kParamMstB, 38},       {29, kParamMstC, 30},
  {29, kParamMstMix, 24},

  // Family 02 / 005 Metal Bells
  {30, kParamEngine, 1},      {30, kParamOsc1Shape, 10},  {30, kParamOsc2Shape, 80},
  {30, kParamOsc2Pitch, 19},  {30, kParamOsc2Level, 70},  {30, kParamFmAmount, 40},
  {30, kParamOscRing, 1},     {30, kParamFilterCutoff, 9800}, {30, kParamFilterReso, 20},
  {30, kParamFilterKeyTrack, 60}, {30, kParamAmpAttack, 1},   {30, kParamAmpDecay, 1600},
  {30, kParamAmpSustain, 0},  {30, kParamAmpRelease, 1800}, {30, kParamFilterDrive, 18},
  {30, kParamSynthMasterLevel, -5},
  {30, kParamIns1Algo, 3},    {30, kParamIns1A, 24},      {30, kParamIns1B, 26},
  {30, kParamIns1C, 58},      {30, kParamIns1Mix, 20},    {30, kParamIns2Algo, 1},
  {30, kParamIns2A, 42},      {30, kParamIns2B, 56},      {30, kParamIns2C, 40},
  {30, kParamIns2Mix, 18},    {30, kParamMstAlgo, 3},     {30, kParamMstA, 62},
  {30, kParamMstB, 30},       {30, kParamMstC, 50},       {30, kParamMstMix, 24},

  // Family 02 / 006 Neon Pluck
  {31, kParamOsc1Shape, 70},  {31, kParamOsc2Shape, 40},  {31, kParamOsc2Pitch, 12},
  {31, kParamFilterCutoff, 3500}, {31, kParamFilterReso, 30}, {31, kParamFilterEnv, 75},
  {31, kParamFiltAttack, 1},  {31, kParamFiltDecay, 180}, {31, kParamFiltSustain, 0},
  {31, kParamFiltRelease, 200}, {31, kParamAmpAttack, 2},   {31, kParamAmpDecay, 220},
  {31, kParamAmpSustain, 0},  {31, kParamAmpRelease, 250}, {31, kParamFilterKeyTrack, 75},
  {31, kParamIns1Algo, 1},    {31, kParamIns1A, 44},      {31, kParamIns1B, 50},
  {31, kParamIns1C, 36},      {31, kParamIns1Mix, 24},    {31, kParamMstAlgo, 1},
  {31, kParamMstA, 55},       {31, kParamMstB, 34},       {31, kParamMstC, 26},
  {31, kParamMstMix, 22},

  // Family 02 / 007 Sparse Pluck
  {32, kParamEngine, 1},      {32, kParamOsc1Shape, 64},  {32, kParamOsc1Level, 80},
  {32, kParamOsc2Level, 12},  {32, kParamFilterCutoff, 4800}, {32, kParamFilterReso, 14},
  {32, kParamFilterEnv, 60},  {32, kParamFilterKeyTrack, 85}, {32, kParamFiltAttack, 1},
  {32, kParamFiltDecay, 90},  {32, kParamFiltSustain, 0}, {32, kParamFiltRelease, 80},
  {32, kParamAmpAttack, 1},   {32, kParamAmpDecay, 140},  {32, kParamAmpSustain, 0},
  {32, kParamAmpRelease, 160}, {32, kParamTapeMix, 12},    {32, kParamSynthMasterLevel, -3},
  {32, kParamIns1Algo, 3},    {32, kParamIns1A, 14},      {32, kParamIns1B, 10},
  {32, kParamIns1C, 72},      {32, kParamIns1Mix, 8},     {32, kParamMstAlgo, 1},
  {32, kParamMstA, 58},       {32, kParamMstB, 28},       {32, kParamMstC, 24},
  {32, kParamMstMix, 14},

  // Family 02 / 008 Thin Stab
  {33, kParamPolyMode, 1},    {33, kParamOsc1Shape, 55},  {33, kParamOsc1Level, 85},
  {33, kParamOsc2Shape, 100}, {33, kParamOsc2PW, 20},     {33, kParamOsc2Level, 60},
  {33, kParamFilterCutoff, 5200}, {33, kParamFilterReso, 18}, {33, kParamFilterEnv, 50},
  {33, kParamAmpAttack, 1},   {33, kParamAmpDecay, 120},  {33, kParamAmpSustain, 0},
  {33, kParamAmpRelease, 120}, {33, kParamTapeSat, 40},
  {33, kParamIns1Algo, 1},    {33, kParamIns1A, 52},      {33, kParamIns1B, 46},
  {33, kParamIns1C, 34},      {33, kParamIns1Mix, 30},    {33, kParamMstAlgo, 1},
  {33, kParamMstA, 50},       {33, kParamMstB, 48},       {33, kParamMstC, 20},
  {33, kParamMstMix, 30},

  // Family 02 / 009 Hard Bass
  {34, kParamEngine, 1},      {34, kParamPolyMode, 1},    {34, kParamOscSync, 1},
  {34, kParamOsc1Shape, 74},  {34, kParamOsc1Pitch, -12}, {34, kParamOsc2Shape, 80},
  {34, kParamOsc2Pitch, -12}, {34, kParamOsc2Level, 85},  {34, kParamSubLevel, 80},
  {34, kParamFilterCutoff, 900}, {34, kParamFilterReso, 28}, {34, kParamFilterEnv, 65},
  {34, kParamFilterDrive, 42}, {34, kParamFilterKeyTrack, 25}, {34, kParamAmpAttack, 1},
  {34, kParamAmpDecay, 260},  {34, kParamAmpSustain, 70}, {34, kParamAmpRelease, 140},
  {34, kParamVelToAmp, 45},   {34, kParamTapeSat, 48},    {34, kParamTapeMix, 20},
  {34, kParamIns1Algo, 1},    {34, kParamIns1A, 70},      {34, kParamIns1B, 35},
  {34, kParamIns1C, 28},      {34, kParamIns1Mix, 40},    {34, kParamMstAlgo, 2},
  {34, kParamMstA, 36},       {34, kParamMstB, 30},       {34, kParamMstC, 18},
  {34, kParamMstMix, 55},

  // Family 02 / 010 Sparse Bass
  {35, kParamEngine, 1},      {35, kParamPolyMode, 1},    {35, kParamMonoLegato, 1},
  {35, kParamOsc1Shape, 60},  {35, kParamOsc1Pitch, -12}, {35, kParamOsc2Level, 16},
  {35, kParamSubLevel, 85},   {35, kParamFilterCutoff, 700}, {35, kParamFilterReso, 12},
  {35, kParamFilterEnv, 30},  {35, kParamAmpAttack, 2},   {35, kParamAmpDecay, 220},
  {35, kParamAmpSustain, 55}, {35, kParamAmpRelease, 160}, {35, kParamVelToAmp, 30},
  {35, kParamTapeSat, 8},     {35, kParamTapeMix, 8},     {35, kParamSynthMasterLevel, -2},
  {35, kParamIns1Algo, 1},    {35, kParamIns1A, 22},      {35, kParamIns1B, 34},
  {35, kParamIns1C, 48},      {35, kParamIns1Mix, 14},    {35, kParamMstAlgo, 1},
  {35, kParamMstA, 60},       {35, kParamMstB, 30},       {35, kParamMstC, 24},
  {35, kParamMstMix, 18},

  // Family 02 / 011 Sync Lead
  {36, kParamEngine, 1},      {36, kParamPolyMode, 1},    {36, kParamOscSync, 1},
  {36, kParamPorta, 140},     {36, kParamOsc1Shape, 68},  {36, kParamOsc2Shape, 72},
  {36, kParamOsc2Pitch, 7},   {36, kParamOsc2Level, 75},  {36, kParamFilterCutoff, 2400},
  {36, kParamFilterReso, 36}, {36, kParamFilterEnv, 48},  {36, kParamFilterDrive, 35},
  {36, kParamFilterKeyTrack, 50}, {36, kParamAmpAttack, 6},   {36, kParamAmpDecay, 280},
  {36, kParamAmpSustain, 80}, {36, kParamAmpRelease, 200}, {36, kParamModWheelDest, 1},
  {36, kParamModWheelAmount, 55}, {36, kParamVelToFilter, 40},
  {36, kParamIns1Algo, 1},    {36, kParamIns1A, 58},      {36, kParamIns1B, 44},
  {36, kParamIns1C, 36},      {36, kParamIns1Mix, 32},    {36, kParamIns2Algo, 3},
  {36, kParamIns2A, 28},      {36, kParamIns2B, 24},      {36, kParamIns2C, 60},
  {36, kParamIns2Mix, 16},    {36, kParamMstAlgo, 1},     {36, kParamMstA, 54},
  {36, kParamMstB, 40},       {36, kParamMstC, 30},       {36, kParamMstMix, 26},

  // Family 02 / 012 Stab Brass
  {37, kParamEngine, 1},      {37, kParamOsc1Shape, 64},  {37, kParamOsc2Shape, 58},
  {37, kParamOsc2Pitch, 7},   {37, kParamOsc2Level, 80},  {37, kParamUnisonVoices, 2},
  {37, kParamUnisonDetune, 18}, {37, kParamFilterCutoff, 1800}, {37, kParamFilterReso, 26},
  {37, kParamFilterEnv, 78},  {37, kParamFilterDrive, 20}, {37, kParamFiltAttack, 8},
  {37, kParamFiltDecay, 160}, {37, kParamFiltSustain, 20}, {37, kParamFiltRelease, 180},
  {37, kParamAmpAttack, 8},   {37, kParamAmpDecay, 220},  {37, kParamAmpSustain, 35},
  {37, kParamAmpRelease, 180}, {37, kParamVelToAmp, 65},   {37, kParamVelToFilter, 70},
  {37, kParamIns1Algo, 1},    {37, kParamIns1A, 44},      {37, kParamIns1B, 50},
  {37, kParamIns1C, 40},      {37, kParamIns1Mix, 26},    {37, kParamMstAlgo, 1},
  {37, kParamMstA, 50},       {37, kParamMstB, 46},       {37, kParamMstC, 22},
  {37, kParamMstMix, 30},

  // Family 02 / 013 Tight Strings
  {38, kParamEngine, 1},      {38, kParamOsc1Shape, 48},  {38, kParamOsc2Shape, 44},
  {38, kParamOsc2Pitch, 12},  {38, kParamOsc2Level, 60},  {38, kParamUnisonVoices, 3},
  {38, kParamUnisonDetune, 12}, {38, kParamFilterCutoff, 3600}, {38, kParamFilterReso, 10},
  {38, kParamFilterEnv, 20},  {38, kParamAmpAttack, 180}, {38, kParamAmpDecay, 900},
  {38, kParamAmpSustain, 70}, {38, kParamAmpRelease, 700}, {38, kParamVelToAmp, 25},
  {38, kParamIns1Algo, 2},    {38, kParamIns1A, 22},      {38, kParamIns1B, 20},
  {38, kParamIns1C, 42},      {38, kParamIns1Mix, 16},    {38, kParamMstAlgo, 1},
  {38, kParamMstA, 52},       {38, kParamMstB, 48},       {38, kParamMstC, 34},
  {38, kParamMstMix, 30},

  // Family 02 / 014 Breath Choir
  {39, kParamEngine, 1},      {39, kParamOsc1Shape, 24},  {39, kParamOsc2Shape, 50},
  {39, kParamOsc2Pitch, 12},  {39, kParamOsc2Level, 45},  {39, kParamNoiseLevel, 16},
  {39, kParamUnisonVoices, 4}, {39, kParamUnisonDetune, 28}, {39, kParamFilterType, 2},
  {39, kParamFilterCutoff, 1400}, {39, kParamFilterReso, 22}, {39, kParamAmpAttack, 500},
  {39, kParamAmpDecay, 1600}, {39, kParamAmpSustain, 75}, {39, kParamAmpRelease, 1800},
  {39, kParamLfoRate, 0.4},   {39, kParamLfoDest, 4},     {39, kParamLfoAmount, 18},
  {39, kParamSynthMasterLevel, -4},
  {39, kParamIns1Algo, 2},    {39, kParamIns1A, 18},      {39, kParamIns1B, 32},
  {39, kParamIns1C, 58},      {39, kParamIns1Mix, 24},    {39, kParamIns2Algo, 3},
  {39, kParamIns2A, 40},      {39, kParamIns2B, 24},      {39, kParamIns2C, 50},
  {39, kParamIns2Mix, 16},    {39, kParamMstAlgo, 3},     {39, kParamMstA, 60},
  {39, kParamMstB, 28},       {39, kParamMstC, 50},       {39, kParamMstMix, 24},

  // Family 02 / 015 Poly Pad
  {40, kParamEngine, 1},      {40, kParamOsc1Shape, 44},  {40, kParamOsc2Shape, 66},
  {40, kParamOsc2Pitch, 7},   {40, kParamOsc2Level, 70},  {40, kParamUnisonVoices, 3},
  {40, kParamUnisonDetune, 24}, {40, kParamFilterCutoff, 2400}, {40, kParamFilterReso, 18},
  {40, kParamFilterEnv, 35},  {40, kParamFilterDrive, 12}, {40, kParamAmpAttack, 350},
  {40, kParamAmpDecay, 2000}, {40, kParamAmpSustain, 68}, {40, kParamAmpRelease, 2200},
  {40, kParamLfoRate, 0.3},   {40, kParamLfoDest, 1},     {40, kParamLfoAmount, 22},
  {40, kParamIns1Algo, 2},    {40, kParamIns1A, 26},      {40, kParamIns1B, 34},
  {40, kParamIns1C, 56},      {40, kParamIns1Mix, 28},    {40, kParamIns2Algo, 3},
  {40, kParamIns2A, 40},      {40, kParamIns2B, 30},      {40, kParamIns2C, 54},
  {40, kParamIns2Mix, 18},    {40, kParamMstAlgo, 3},     {40, kParamMstA, 64},
  {40, kParamMstB, 26},       {40, kParamMstC, 50},       {40, kParamMstMix, 26},

  // Family 02 / 016 Sweep Motion
  {41, kParamEngine, 1},      {41, kParamOsc1Shape, 50},  {41, kParamOsc2Shape, 62},
  {41, kParamOsc2Pitch, -5},  {41, kParamOsc2Level, 65},  {41, kParamUnisonVoices, 4},
  {41, kParamUnisonDetune, 44}, {41, kParamFilterCutoff, 800}, {41, kParamFilterReso, 32},
  {41, kParamFilterEnv, 80},  {41, kParamFiltAttack, 1800}, {41, kParamFiltDecay, 2400},
  {41, kParamFiltSustain, 30}, {41, kParamFiltRelease, 2000}, {41, kParamAmpAttack, 200},
  {41, kParamAmpDecay, 2600}, {41, kParamAmpSustain, 60}, {41, kParamAmpRelease, 2400},
  {41, kParamLfoRate, 0.12},  {41, kParamLfoShape, 2},    {41, kParamLfoDest, 1},
  {41, kParamLfoAmount, 55},
  {41, kParamIns1Algo, 2},    {41, kParamIns1A, 10},      {41, kParamIns1B, 38},
  {41, kParamIns1C, 60},      {41, kParamIns1Mix, 22},    {41, kParamIns2Algo, 3},
  {41, kParamIns2A, 62},      {41, kParamIns2B, 48},      {41, kParamIns2C, 46},
  {41, kParamIns2Mix, 36},    {41, kParamMstAlgo, 3},     {41, kParamMstA, 66},
  {41, kParamMstB, 24},       {41, kParamMstC, 50},       {41, kParamMstMix, 32},

  // Family 02 / 017 Skew Texture
  {42, kParamEngine, 1},      {42, kParamOsc1Shape, 58},  {42, kParamOsc2Shape, 90},
  {42, kParamOsc2Pitch, 6},   {42, kParamOsc2Level, 55},  {42, kParamOscRing, 1},
  {42, kParamNoiseLevel, 20}, {42, kParamFilterType, 2},  {42, kParamFilterCutoff, 900},
  {42, kParamFilterReso, 48}, {42, kParamFilterDrive, 28}, {42, kParamUnisonVoices, 2},
  {42, kParamUnisonDetune, 50}, {42, kParamAmpAttack, 80},  {42, kParamAmpDecay, 1800},
  {42, kParamAmpSustain, 55}, {42, kParamAmpRelease, 1600}, {42, kParamLfoRate, 3.5},
  {42, kParamLfoShape, 4},    {42, kParamLfoDest, 1},     {42, kParamLfoAmount, 60},
  {42, kParamIns1Algo, 4},    {42, kParamIns1A, 55},      {42, kParamIns1B, 40},
  {42, kParamIns1C, 35},      {42, kParamIns1Mix, 45},    {42, kParamIns2Algo, 1},
  {42, kParamIns2A, 48},      {42, kParamIns2B, 38},      {42, kParamIns2C, 44},
  {42, kParamIns2Mix, 24},    {42, kParamMstAlgo, 4},     {42, kParamMstA, 30},
  {42, kParamMstB, 55},       {42, kParamMstC, 18},       {42, kParamMstMix, 40},

  // Family 02 / 018 Pulse Drone
  {43, kParamEngine, 1},      {43, kParamOsc1Shape, 70},  {43, kParamOsc1PW, 30},
  {43, kParamOsc2Shape, 66},  {43, kParamOsc2Pitch, -12}, {43, kParamOsc2Level, 50},
  {43, kParamSubLevel, 70},   {43, kParamFilterCutoff, 600}, {43, kParamFilterReso, 42},
  {43, kParamFilterEnv, 15},  {43, kParamFilterDrive, 25}, {43, kParamAmpAttack, 900},
  {43, kParamAmpDecay, 3000}, {43, kParamAmpSustain, 90}, {43, kParamAmpRelease, 3200},
  {43, kParamLfoRate, 0.2},   {43, kParamLfoShape, 3},    {43, kParamLfoDest, 4},
  {43, kParamLfoAmount, 30},  {43, kParamSynthMasterLevel, -6},
  {43, kParamIns1Algo, 1},    {43, kParamIns1A, 30},      {43, kParamIns1B, 28},
  {43, kParamIns1C, 50},      {43, kParamIns1Mix, 20},    {43, kParamIns2Algo, 3},
  {43, kParamIns2A, 58},      {43, kParamIns2B, 34},      {43, kParamIns2C, 34},
  {43, kParamIns2Mix, 16},    {43, kParamMstAlgo, 4},     {43, kParamMstA, 38},
  {43, kParamMstB, 30},       {43, kParamMstC, 12},       {43, kParamMstMix, 24},

  // Family 02 / 019 Wide Arp
  {44, kParamOsc1Shape, 66},  {44, kParamOsc2Shape, 66},  {44, kParamOsc2Pitch, 7},
  {44, kParamOsc2Level, 70},  {44, kParamUnisonVoices, 4}, {44, kParamUnisonDetune, 40},
  {44, kParamFilterCutoff, 7000}, {44, kParamAmpAttack, 1},   {44, kParamAmpDecay, 90},
  {44, kParamAmpSustain, 0},  {44, kParamAmpRelease, 140},
  {44, kParamIns1Algo, 3},    {44, kParamIns1A, 20},      {44, kParamIns1B, 22},
  {44, kParamIns1C, 66},      {44, kParamIns1Mix, 18},    {44, kParamMstAlgo, 3},
  {44, kParamMstA, 66},       {44, kParamMstB, 28},       {44, kParamMstC, 50},
  {44, kParamMstMix, 24},

  // Family 02 / 020 Heavy Crush
  {45, kParamEngine, 1},      {45, kParamOsc1Shape, 78},  {45, kParamOsc2Shape, 84},
  {45, kParamOsc2Pitch, 12},  {45, kParamOsc2Level, 70},  {45, kParamFilterCutoff, 3200},
  {45, kParamFilterReso, 24}, {45, kParamFilterDrive, 55}, {45, kParamAmpAttack, 5},
  {45, kParamAmpDecay, 600},  {45, kParamAmpSustain, 40}, {45, kParamAmpRelease, 400},
  {45, kParamOutLevel, -4},
  {45, kParamIns1Algo, 4},    {45, kParamIns1A, 72},      {45, kParamIns1B, 64},
  {45, kParamIns1C, 48},      {45, kParamIns1Mix, 80},    {45, kParamIns2Algo, 1},
  {45, kParamIns2A, 66},      {45, kParamIns2B, 30},      {45, kParamIns2C, 44},
  {45, kParamIns2Mix, 50},    {45, kParamMstAlgo, 2},     {45, kParamMstA, 28},
  {45, kParamMstB, 55},       {45, kParamMstC, 40},       {45, kParamMstMix, 70},

  // Family 02 / 021 Punch Kit
  {46, kParamEngine, 1},      {46, kParamOsc1Shape, 100}, {46, kParamOsc1PW, 15},
  {46, kParamOsc2Shape, 100}, {46, kParamOsc2Pitch, -12}, {46, kParamOsc2Level, 90},
  {46, kParamNoiseLevel, 25}, {46, kParamSubLevel, 40},   {46, kParamFilterCutoff, 11000},
  {46, kParamFilterReso, 6},  {46, kParamFilterEnv, 0},   {46, kParamFilterDrive, 48},
  {46, kParamAmpAttack, 0.5}, {46, kParamAmpDecay, 90},   {46, kParamAmpSustain, 0},
  {46, kParamAmpRelease, 70}, {46, kParamPitchAmount, -20}, {46, kParamPitchAttack, 0.5},
  {46, kParamPitchDecay, 50}, {46, kParamPitchRelease, 50}, {46, kParamTapeSat, 30},
  {46, kParamTapeMix, 12},    {46, kParamDrumSlotTune, 2}, {46, kParamDrumSlotDecay, 28},
  {46, kParamDrumSlotDrive, 70}, {46, kParamDrumSlotFilter, 15000}, {46, kParamDrumKitLevel, -2},
  {46, kParamIns1Algo, 1},    {46, kParamIns1A, 62},      {46, kParamIns1B, 45},
  {46, kParamIns1C, 40},      {46, kParamIns1Mix, 36},    {46, kParamIns2Algo, 4},
  {46, kParamIns2A, 66},      {46, kParamIns2B, 50},      {46, kParamIns2C, 30},
  {46, kParamIns2Mix, 20},    {46, kParamMstAlgo, 2},     {46, kParamMstA, 24},
  {46, kParamMstB, 46},       {46, kParamMstC, 18},       {46, kParamMstMix, 60},

  // Family 03 / 001 Piano Stack
  {47, kParamOsc1Shape, 12},  {47, kParamOsc1Level, 95},  {47, kParamOsc2Shape, 18},
  {47, kParamOsc2Pitch, 12},  {47, kParamOsc2Level, 48},  {47, kParamFmAmount, 6},
  {47, kParamFilterCutoff, 6400}, {47, kParamFilterReso, 4},  {47, kParamFilterEnv, 12},
  {47, kParamAmpAttack, 6},   {47, kParamAmpDecay, 1100}, {47, kParamAmpSustain, 38},
  {47, kParamAmpRelease, 900}, {47, kParamVelToAmp, 35},   {47, kParamTapeSat, 18},
  {47, kParamTapeMix, 28},
  {47, kParamIns1Algo, 2},    {47, kParamIns1A, 10},      {47, kParamIns1B, 16},
  {47, kParamIns1C, 38},      {47, kParamIns1Mix, 12},    {47, kParamIns2Algo, 3},
  {47, kParamIns2A, 30},      {47, kParamIns2B, 18},      {47, kParamIns2C, 50},
  {47, kParamIns2Mix, 12},    {47, kParamMstAlgo, 4},     {47, kParamMstA, 56},
  {47, kParamMstB, 44},       {47, kParamMstC, 10},       {47, kParamMstMix, 16},

  // Family 03 / 002 Soft EP
  {48, kParamOsc1Shape, 25},  {48, kParamOsc2Shape, 33},  {48, kParamOsc2Pitch, 12},
  {48, kParamOsc2Level, 45},  {48, kParamFmAmount, 18},   {48, kParamFilterCutoff, 4200},
  {48, kParamFilterReso, 12}, {48, kParamAmpAttack, 6},   {48, kParamAmpDecay, 1400},
  {48, kParamAmpSustain, 45}, {48, kParamAmpRelease, 800}, {48, kParamTapeMix, 35},
  {48, kParamIns1Algo, 2},    {48, kParamIns1A, 16},      {48, kParamIns1B, 24},
  {48, kParamIns1C, 46},      {48, kParamIns1Mix, 18},    {48, kParamMstAlgo, 4},
  {48, kParamMstA, 58},       {48, kParamMstB, 40},       {48, kParamMstC, 12},
  {48, kParamMstMix, 18},

  // Family 03 / 003 Soul Piano
  {49, kParamOsc1Shape, 20},  {49, kParamOsc2Shape, 36},  {49, kParamOsc2Pitch, 12},
  {49, kParamOsc2Level, 52},  {49, kParamFmAmount, 22},   {49, kParamFilterCutoff, 3400},
  {49, kParamFilterReso, 14}, {49, kParamFilterEnv, 22},  {49, kParamAmpAttack, 8},
  {49, kParamAmpDecay, 1600}, {49, kParamAmpSustain, 50}, {49, kParamAmpRelease, 1100},
  {49, kParamVelToAmp, 58},   {49, kParamVelToFilter, 30}, {49, kParamTapeSat, 30},
  {49, kParamTapeMix, 38},    {49, kParamTapeHiss, 6},
  {49, kParamIns1Algo, 2},    {49, kParamIns1A, 14},      {49, kParamIns1B, 28},
  {49, kParamIns1C, 42},      {49, kParamIns1Mix, 26},    {49, kParamIns2Algo, 3},
  {49, kParamIns2A, 26},      {49, kParamIns2B, 20},      {49, kParamIns2C, 46},
  {49, kParamIns2Mix, 14},    {49, kParamMstAlgo, 4},     {49, kParamMstA, 58},
  {49, kParamMstB, 45},       {49, kParamMstC, 14},       {49, kParamMstMix, 22},

  // Family 03 / 004 Clav Stab
  {50, kParamOsc1Shape, 84},  {50, kParamOsc1PW, 24},     {50, kParamOsc2Shape, 92},
  {50, kParamOsc2Pitch, 12},  {50, kParamOsc2Level, 62},  {50, kParamOsc2PW, 18},
  {50, kParamFilterCutoff, 2800}, {50, kParamFilterReso, 45}, {50, kParamFilterEnv, 80},
  {50, kParamFilterDrive, 26}, {50, kParamFilterKeyTrack, 40}, {50, kParamFiltAttack, 1},
  {50, kParamFiltDecay, 100}, {50, kParamFiltSustain, 0}, {50, kParamFiltRelease, 90},
  {50, kParamAmpAttack, 1},   {50, kParamAmpDecay, 140},  {50, kParamAmpSustain, 0},
  {50, kParamAmpRelease, 110}, {50, kParamVelToAmp, 78},   {50, kParamVelToFilter, 60},
  {50, kParamTapeSat, 22},    {50, kParamTapeMix, 16},
  {50, kParamIns1Algo, 1},    {50, kParamIns1A, 38},      {50, kParamIns1B, 50},
  {50, kParamIns1C, 44},      {50, kParamIns1Mix, 24},    {50, kParamMstAlgo, 1},
  {50, kParamMstA, 54},       {50, kParamMstB, 44},       {50, kParamMstC, 24},
  {50, kParamMstMix, 26},

  // Family 03 / 005 Dry Clav
  {51, kParamOsc1Shape, 78},  {51, kParamOsc1PW, 30},     {51, kParamOsc2Shape, 90},
  {51, kParamOsc2Pitch, 12},  {51, kParamOsc2Level, 55},  {51, kParamFilterCutoff, 3000},
  {51, kParamFilterReso, 28}, {51, kParamFilterEnv, 60},  {51, kParamAmpAttack, 1},
  {51, kParamAmpDecay, 260},  {51, kParamAmpSustain, 0},  {51, kParamAmpRelease, 160},
  {51, kParamTapeSat, 30},    {51, kParamTapeMix, 20},
  {51, kParamIns1Algo, 1},    {51, kParamIns1A, 28},      {51, kParamIns1B, 46},
  {51, kParamIns1C, 48},      {51, kParamIns1Mix, 18},    {51, kParamMstAlgo, 1},
  {51, kParamMstA, 58},       {51, kParamMstB, 36},       {51, kParamMstC, 22},
  {51, kParamMstMix, 20},

  // Family 03 / 006 Warm Organ
  {52, kParamOsc1Shape, 33},  {52, kParamOsc1Level, 90},  {52, kParamOsc2Shape, 33},
  {52, kParamOsc2Pitch, 12},  {52, kParamOsc2Level, 90},  {52, kParamSubLevel, 40},
  {52, kParamFilterCutoff, 8000}, {52, kParamFilterReso, 6},  {52, kParamAmpAttack, 3},
  {52, kParamAmpDecay, 60},   {52, kParamAmpSustain, 100}, {52, kParamAmpRelease, 90},
  {52, kParamIns1Algo, 2},    {52, kParamIns1A, 14},      {52, kParamIns1B, 26},
  {52, kParamIns1C, 48},      {52, kParamIns1Mix, 20},    {52, kParamMstAlgo, 1},
  {52, kParamMstA, 56},       {52, kParamMstB, 32},       {52, kParamMstC, 38},
  {52, kParamMstMix, 22},

  // Family 03 / 007 Bottle Bells
  {53, kParamOsc1Shape, 4},   {53, kParamOsc2Shape, 8},   {53, kParamOsc2Pitch, 12},
  {53, kParamOsc2Level, 40},  {53, kParamFmAmount, 14},   {53, kParamFilterCutoff, 7000},
  {53, kParamFilterReso, 4},  {53, kParamFilterKeyTrack, 35}, {53, kParamAmpAttack, 3},
  {53, kParamAmpDecay, 1800}, {53, kParamAmpSustain, 10}, {53, kParamAmpRelease, 1600},
  {53, kParamTapeMix, 24},
  {53, kParamIns1Algo, 3},    {53, kParamIns1A, 30},      {53, kParamIns1B, 18},
  {53, kParamIns1C, 75},      {53, kParamIns1Mix, 14},    {53, kParamIns2Algo, 2},
  {53, kParamIns2A, 12},      {53, kParamIns2B, 22},      {53, kParamIns2C, 62},
  {53, kParamIns2Mix, 14},    {53, kParamMstAlgo, 3},     {53, kParamMstA, 62},
  {53, kParamMstB, 28},       {53, kParamMstC, 50},       {53, kParamMstMix, 22},

  // Family 03 / 008 Muted Pluck
  {54, kParamOsc1Shape, 46},  {54, kParamOsc1PW, 35},     {54, kParamOsc2Shape, 30},
  {54, kParamOsc2Pitch, 12},  {54, kParamOsc2Level, 35},  {54, kParamFilterCutoff, 2200},
  {54, kParamFilterReso, 16}, {54, kParamFilterEnv, 40},  {54, kParamFilterKeyTrack, 50},
  {54, kParamFiltAttack, 2},  {54, kParamFiltDecay, 200}, {54, kParamFiltSustain, 0},
  {54, kParamFiltRelease, 180}, {54, kParamAmpAttack, 2},   {54, kParamAmpDecay, 280},
  {54, kParamAmpSustain, 0},  {54, kParamAmpRelease, 200}, {54, kParamTapeSat, 20},
  {54, kParamTapeMix, 22},
  {54, kParamIns1Algo, 3},    {54, kParamIns1A, 12},      {54, kParamIns1B, 12},
  {54, kParamIns1C, 52},      {54, kParamIns1Mix, 10},    {54, kParamMstAlgo, 1},
  {54, kParamMstA, 56},       {54, kParamMstB, 30},       {54, kParamMstC, 24},
  {54, kParamMstMix, 18},

  // Family 03 / 009 Reed Stab
  {55, kParamOsc1Shape, 38},  {55, kParamOsc2Shape, 42},  {55, kParamOsc2Pitch, 7},
  {55, kParamOsc2Level, 55},  {55, kParamFilterCutoff, 2600}, {55, kParamFilterReso, 24},
  {55, kParamFilterEnv, 55},  {55, kParamFiltAttack, 4},  {55, kParamFiltDecay, 150},
  {55, kParamFiltSustain, 0}, {55, kParamFiltRelease, 140}, {55, kParamAmpAttack, 4},
  {55, kParamAmpDecay, 200},  {55, kParamAmpSustain, 0},  {55, kParamAmpRelease, 160},
  {55, kParamVelToAmp, 50},   {55, kParamVelToFilter, 40},
  {55, kParamIns1Algo, 2},    {55, kParamIns1A, 16},      {55, kParamIns1B, 22},
  {55, kParamIns1C, 44},      {55, kParamIns1Mix, 18},    {55, kParamIns2Algo, 3},
  {55, kParamIns2A, 20},      {55, kParamIns2B, 16},      {55, kParamIns2C, 48},
  {55, kParamIns2Mix, 12},    {55, kParamMstAlgo, 1},     {55, kParamMstA, 52},
  {55, kParamMstB, 44},       {55, kParamMstC, 22},       {55, kParamMstMix, 26},

  // Family 03 / 010 Finger Bass
  {56, kParamPolyMode, 1},    {56, kParamMonoLegato, 1},  {56, kParamOsc1Shape, 28},
  {56, kParamOsc1Pitch, -12}, {56, kParamOsc2Shape, 22},  {56, kParamOsc2Pitch, -12},
  {56, kParamOsc2Level, 40},  {56, kParamSubLevel, 55},   {56, kParamFilterCutoff, 1100},
  {56, kParamFilterReso, 14}, {56, kParamFilterEnv, 35},  {56, kParamAmpAttack, 4},
  {56, kParamAmpDecay, 400},  {56, kParamAmpSustain, 72}, {56, kParamAmpRelease, 220},
  {56, kParamVelToAmp, 40},   {56, kParamTapeSat, 24},    {56, kParamTapeMix, 30},
  {56, kParamIns1Algo, 1},    {56, kParamIns1A, 26},      {56, kParamIns1B, 38},
  {56, kParamIns1C, 46},      {56, kParamIns1Mix, 20},    {56, kParamMstAlgo, 1},
  {56, kParamMstA, 60},       {56, kParamMstB, 40},       {56, kParamMstC, 22},
  {56, kParamMstMix, 24},

  // Family 03 / 011 Solo Lead
  {57, kParamPolyMode, 1},    {57, kParamPorta, 60},      {57, kParamOsc1Shape, 34},
  {57, kParamOsc2Shape, 40},  {57, kParamOsc2Pitch, 12},  {57, kParamOsc2Level, 40},
  {57, kParamFmAmount, 10},   {57, kParamFilterCutoff, 4500}, {57, kParamFilterReso, 12},
  {57, kParamFilterEnv, 28},  {57, kParamFilterKeyTrack, 30}, {57, kParamAmpAttack, 18},
  {57, kParamAmpDecay, 500},  {57, kParamAmpSustain, 76}, {57, kParamAmpRelease, 320},
  {57, kParamModWheelDest, 2}, {57, kParamModWheelAmount, 18}, {57, kParamVelToAmp, 28},
  {57, kParamIns1Algo, 3},    {57, kParamIns1A, 28},      {57, kParamIns1B, 22},
  {57, kParamIns1C, 58},      {57, kParamIns1Mix, 16},    {57, kParamIns2Algo, 2},
  {57, kParamIns2A, 14},      {57, kParamIns2B, 18},      {57, kParamIns2C, 54},
  {57, kParamIns2Mix, 12},    {57, kParamMstAlgo, 1},     {57, kParamMstA, 56},
  {57, kParamMstB, 36},       {57, kParamMstC, 32},       {57, kParamMstMix, 22},

  // Family 03 / 012 Ensemble Brass
  {58, kParamOsc1Shape, 46},  {58, kParamOsc2Shape, 50},  {58, kParamOsc2Pitch, 7},
  {58, kParamOsc2Level, 75},  {58, kParamUnisonVoices, 3}, {58, kParamUnisonDetune, 14},
  {58, kParamFilterCutoff, 2000}, {58, kParamFilterReso, 10}, {58, kParamFilterEnv, 48},
  {58, kParamFiltAttack, 40}, {58, kParamFiltDecay, 400}, {58, kParamFiltSustain, 60},
  {58, kParamFiltRelease, 500}, {58, kParamAmpAttack, 45},  {58, kParamAmpDecay, 700},
  {58, kParamAmpSustain, 80}, {58, kParamAmpRelease, 400}, {58, kParamVelToFilter, 45},
  {58, kParamTapeSat, 20},    {58, kParamTapeMix, 32},
  {58, kParamIns1Algo, 2},    {58, kParamIns1A, 12},      {58, kParamIns1B, 24},
  {58, kParamIns1C, 52},      {58, kParamIns1Mix, 20},    {58, kParamMstAlgo, 1},
  {58, kParamMstA, 56},       {58, kParamMstB, 42},       {58, kParamMstC, 42},
  {58, kParamMstMix, 30},

  // Family 03 / 013 Section Strings
  {59, kParamOsc1Shape, 36},  {59, kParamOsc2Shape, 32},  {59, kParamOsc2Pitch, 12},
  {59, kParamOsc2Level, 80},  {59, kParamUnisonVoices, 4}, {59, kParamUnisonDetune, 18},
  {59, kParamFilterCutoff, 2800}, {59, kParamFilterReso, 6},  {59, kParamFilterEnv, 18},
  {59, kParamAmpAttack, 700}, {59, kParamAmpDecay, 2000}, {59, kParamAmpSustain, 85},
  {59, kParamAmpRelease, 2000}, {59, kParamFiltAttack, 800}, {59, kParamFiltDecay, 1800},
  {59, kParamFiltSustain, 70}, {59, kParamFiltRelease, 1800}, {59, kParamTapeMix, 36},
  {59, kParamSynthMasterLevel, -2},
  {59, kParamIns1Algo, 2},    {59, kParamIns1A, 8},       {59, kParamIns1B, 30},
  {59, kParamIns1C, 50},      {59, kParamIns1Mix, 24},    {59, kParamIns2Algo, 3},
  {59, kParamIns2A, 44},      {59, kParamIns2B, 24},      {59, kParamIns2C, 46},
  {59, kParamIns2Mix, 16},    {59, kParamMstAlgo, 3},     {59, kParamMstA, 60},
  {59, kParamMstB, 30},       {59, kParamMstC, 50},       {59, kParamMstMix, 24},

  // Family 03 / 014 Ooh Choir
  {60, kParamOsc1Shape, 18},  {60, kParamOsc2Shape, 22},  {60, kParamOsc2Pitch, 12},
  {60, kParamOsc2Level, 70},  {60, kParamNoiseLevel, 6},  {60, kParamUnisonVoices, 5},
  {60, kParamUnisonDetune, 16}, {60, kParamFilterCutoff, 3600}, {60, kParamFilterReso, 8},
  {60, kParamAmpAttack, 800}, {60, kParamAmpDecay, 2200}, {60, kParamAmpSustain, 88},
  {60, kParamAmpRelease, 2400}, {60, kParamLfoRate, 0.15},  {60, kParamLfoDest, 3},
  {60, kParamLfoAmount, 8},   {60, kParamTapeHiss, 8},    {60, kParamTapeMix, 30},
  {60, kParamIns1Algo, 2},    {60, kParamIns1A, 10},      {60, kParamIns1B, 26},
  {60, kParamIns1C, 56},      {60, kParamIns1Mix, 22},    {60, kParamIns2Algo, 3},
  {60, kParamIns2A, 48},      {60, kParamIns2B, 20},      {60, kParamIns2C, 44},
  {60, kParamIns2Mix, 16},    {60, kParamMstAlgo, 3},     {60, kParamMstA, 62},
  {60, kParamMstB, 28},       {60, kParamMstC, 50},       {60, kParamMstMix, 22},

  // Family 03 / 015 Dream Pad
  {61, kParamOsc1Shape, 14},  {61, kParamOsc2Shape, 26},  {61, kParamOsc2Pitch, 7},
  {61, kParamOsc2Level, 55},  {61, kParamUnisonVoices, 3}, {61, kParamUnisonDetune, 20},
  {61, kParamFilterCutoff, 3000}, {61, kParamFilterReso, 12}, {61, kParamFilterEnv, 20},
  {61, kParamAmpAttack, 900}, {61, kParamAmpDecay, 2400}, {61, kParamAmpSustain, 70},
  {61, kParamAmpRelease, 2800}, {61, kParamLfoRate, 0.1},   {61, kParamLfoDest, 1},
  {61, kParamLfoAmount, 16},  {61, kParamSynthMasterLevel, -3},
  {61, kParamIns1Algo, 2},    {61, kParamIns1A, 12},      {61, kParamIns1B, 34},
  {61, kParamIns1C, 58},      {61, kParamIns1Mix, 28},    {61, kParamIns2Algo, 3},
  {61, kParamIns2A, 48},      {61, kParamIns2B, 26},      {61, kParamIns2C, 52},
  {61, kParamIns2Mix, 16},    {61, kParamMstAlgo, 3},     {61, kParamMstA, 64},
  {61, kParamMstB, 26},       {61, kParamMstC, 50},       {61, kParamMstMix, 26},

  // Family 03 / 016 Slow Sweep
  {62, kParamOsc1Shape, 10},  {62, kParamOsc2Shape, 48},  {62, kParamOsc2Pitch, 5},
  {62, kParamOsc2Level, 45},  {62, kParamFilterCutoff, 1400}, {62, kParamFilterReso, 18},
  {62, kParamFilterEnv, 40},  {62, kParamFiltAttack, 2000}, {62, kParamFiltDecay, 3000},
  {62, kParamFiltSustain, 45}, {62, kParamFiltRelease, 2500}, {62, kParamAmpAttack, 1400},
  {62, kParamAmpDecay, 3200}, {62, kParamAmpSustain, 75}, {62, kParamAmpRelease, 3000},
  {62, kParamLfoRate, 0.07},  {62, kParamLfoShape, 1},    {62, kParamLfoDest, 1},
  {62, kParamLfoAmount, 28},
  {62, kParamIns1Algo, 2},    {62, kParamIns1A, 8},       {62, kParamIns1B, 40},
  {62, kParamIns1C, 54},      {62, kParamIns1Mix, 22},    {62, kParamIns2Algo, 3},
  {62, kParamIns2A, 56},      {62, kParamIns2B, 34},      {62, kParamIns2C, 44},
  {62, kParamIns2Mix, 20},    {62, kParamMstAlgo, 4},     {62, kParamMstA, 46},
  {62, kParamMstB, 32},       {62, kParamMstC, 12},       {62, kParamMstMix, 22},

  // Family 03 / 017 Hollow Texture
  {63, kParamOsc1Shape, 22},  {63, kParamOsc2Shape, 55},  {63, kParamOsc2Pitch, 7},
  {63, kParamOsc2Level, 35},  {63, kParamNoiseLevel, 12}, {63, kParamFilterType, 2},
  {63, kParamFilterCutoff, 1600}, {63, kParamFilterReso, 30}, {63, kParamUnisonVoices, 2},
  {63, kParamUnisonDetune, 30}, {63, kParamAmpAttack, 600}, {63, kParamAmpDecay, 2400},
  {63, kParamAmpSustain, 60}, {63, kParamAmpRelease, 2200}, {63, kParamLfoRate, 0.55},
  {63, kParamLfoShape, 4},    {63, kParamLfoDest, 1},     {63, kParamLfoAmount, 24},
  {63, kParamIns1Algo, 4},    {63, kParamIns1A, 20},      {63, kParamIns1B, 18},
  {63, kParamIns1C, 26},      {63, kParamIns1Mix, 12},    {63, kParamIns2Algo, 3},
  {63, kParamIns2A, 36},      {63, kParamIns2B, 28},      {63, kParamIns2C, 42},
  {63, kParamIns2Mix, 16},    {63, kParamMstAlgo, 4},     {63, kParamMstA, 40},
  {63, kParamMstB, 40},       {63, kParamMstC, 14},       {63, kParamMstMix, 18},

  // Family 03 / 018 Warm Drone
  {64, kParamOsc1Shape, 6},   {64, kParamOsc2Shape, 14},  {64, kParamOsc2Pitch, -12},
  {64, kParamOsc2Level, 60},  {64, kParamSubLevel, 45},   {64, kParamFilterCutoff, 700},
  {64, kParamFilterReso, 22}, {64, kParamFilterEnv, 8},   {64, kParamAmpAttack, 2200},
  {64, kParamAmpDecay, 4000}, {64, kParamAmpSustain, 92}, {64, kParamAmpRelease, 3800},
  {64, kParamLfoRate, 0.05},  {64, kParamLfoDest, 1},     {64, kParamLfoAmount, 14},
  {64, kParamTapeSat, 28},    {64, kParamTapeMix, 40},    {64, kParamSynthMasterLevel, -5},
  {64, kParamIns1Algo, 2},    {64, kParamIns1A, 6},       {64, kParamIns1B, 28},
  {64, kParamIns1C, 42},      {64, kParamIns1Mix, 18},    {64, kParamIns2Algo, 3},
  {64, kParamIns2A, 60},      {64, kParamIns2B, 32},      {64, kParamIns2C, 28},
  {64, kParamIns2Mix, 14},    {64, kParamMstAlgo, 4},     {64, kParamMstA, 38},
  {64, kParamMstB, 26},       {64, kParamMstC, 10},       {64, kParamMstMix, 20},

  // Family 03 / 019 Mallet Arp
  {65, kParamOsc1Shape, 8},   {65, kParamOsc2Shape, 16},  {65, kParamOsc2Pitch, 12},
  {65, kParamOsc2Level, 50},  {65, kParamFmAmount, 8},    {65, kParamFilterCutoff, 5500},
  {65, kParamFilterReso, 10}, {65, kParamFilterEnv, 35},  {65, kParamFilterKeyTrack, 40},
  {65, kParamAmpAttack, 2},   {65, kParamAmpDecay, 160},  {65, kParamAmpSustain, 0},
  {65, kParamAmpRelease, 180}, {65, kParamFiltAttack, 1},  {65, kParamFiltDecay, 120},
  {65, kParamFiltSustain, 0}, {65, kParamFiltRelease, 140},
  {65, kParamIns1Algo, 3},    {65, kParamIns1A, 20},      {65, kParamIns1B, 12},
  {65, kParamIns1C, 60},      {65, kParamIns1Mix, 10},    {65, kParamMstAlgo, 1},
  {65, kParamMstA, 54},       {65, kParamMstB, 28},       {65, kParamMstC, 24},
  {65, kParamMstMix, 18},

  // Family 03 / 020 Slap Delay
  {66, kParamOsc1Shape, 70},  {66, kParamOsc1PW, 40},     {66, kParamOsc2Shape, 60},
  {66, kParamOsc2Pitch, 12},  {66, kParamOsc2Level, 40},  {66, kParamFilterCutoff, 3600},
  {66, kParamFilterReso, 20}, {66, kParamFilterEnv, 45},  {66, kParamAmpAttack, 1},
  {66, kParamAmpDecay, 180},  {66, kParamAmpSustain, 0},  {66, kParamAmpRelease, 140},
  {66, kParamTapeSat, 26},
  {66, kParamIns1Algo, 3},    {66, kParamIns1A, 18},      {66, kParamIns1B, 42},
  {66, kParamIns1C, 60},      {66, kParamIns1Mix, 48},    {66, kParamIns2Algo, 4},
  {66, kParamIns2A, 52},      {66, kParamIns2B, 44},      {66, kParamIns2C, 34},
  {66, kParamIns2Mix, 26},    {66, kParamMstAlgo, 3},     {66, kParamMstA, 64},
  {66, kParamMstB, 30},       {66, kParamMstC, 50},       {66, kParamMstMix, 30},

  // Family 03 / 021 Dry Perc
  {67, kParamEngine, 1},      {67, kParamOsc1Shape, 88},  {67, kParamOsc1PW, 45},
  {67, kParamOsc2Shape, 70},  {67, kParamOsc2Level, 40},  {67, kParamNoiseLevel, 22},
  {67, kParamFilterCutoff, 8000}, {67, kParamFilterReso, 8},  {67, kParamFilterEnv, 15},
  {67, kParamAmpAttack, 0.5}, {67, kParamAmpDecay, 100},  {67, kParamAmpSustain, 0},
  {67, kParamAmpRelease, 80}, {67, kParamPitchAmount, -8}, {67, kParamPitchDecay, 60},
  {67, kParamPitchRelease, 60}, {67, kParamTapeSat, 10},    {67, kParamTapeMix, 10},
  {67, kParamDrumSlotDecay, 36}, {67, kParamDrumSlotDrive, 18}, {67, kParamDrumSlotFilter, 11000},
  {67, kParamDrumKitLevel, -3}, {67, kParamFilterDrive, 12},
  {67, kParamIns1Algo, 1},    {67, kParamIns1A, 30},      {67, kParamIns1B, 52},
  {67, kParamIns1C, 44},      {67, kParamIns1Mix, 22},    {67, kParamIns2Algo, 4},
  {67, kParamIns2A, 58},      {67, kParamIns2B, 46},      {67, kParamIns2C, 28},
  {67, kParamIns2Mix, 16},    {67, kParamMstAlgo, 2},     {67, kParamMstA, 34},
  {67, kParamMstB, 42},       {67, kParamMstC, 16},       {67, kParamMstMix, 46},

  // Family 04 / 001 Vector Keys
  {68, kParamOsc1Shape, 28},  {68, kParamOsc2Shape, 64},  {68, kParamOsc2Pitch, 7},
  {68, kParamOsc2Level, 58},  {68, kParamUnisonVoices, 3}, {68, kParamUnisonDetune, 32},
  {68, kParamFilterCutoff, 5000}, {68, kParamFilterReso, 14}, {68, kParamFilterEnv, 22},
  {68, kParamAmpAttack, 20},  {68, kParamAmpDecay, 900},  {68, kParamAmpSustain, 48},
  {68, kParamAmpRelease, 1100}, {68, kParamLfoRate, 0.18},  {68, kParamLfoDest, 1},
  {68, kParamLfoAmount, 18},  {68, kParamVelToAmp, 30},
  {68, kParamIns1Algo, 2},    {68, kParamIns1A, 16},      {68, kParamIns1B, 48},
  {68, kParamIns1C, 62},      {68, kParamIns1Mix, 26},    {68, kParamIns2Algo, 3},
  {68, kParamIns2A, 34},      {68, kParamIns2B, 22},      {68, kParamIns2C, 54},
  {68, kParamIns2Mix, 14},    {68, kParamMstAlgo, 3},     {68, kParamMstA, 62},
  {68, kParamMstB, 30},       {68, kParamMstC, 50},       {68, kParamMstMix, 24},

  // Family 04 / 002 Phase Piano
  {69, kParamOsc1Shape, 18},  {69, kParamOsc2Shape, 44},  {69, kParamOsc2Pitch, 12},
  {69, kParamOsc2Level, 36},  {69, kParamFmAmount, 12},   {69, kParamFilterCutoff, 3900},
  {69, kParamFilterReso, 16}, {69, kParamAmpAttack, 10},  {69, kParamAmpDecay, 1300},
  {69, kParamAmpSustain, 36}, {69, kParamAmpRelease, 900}, {69, kParamLfoRate, 0.45},
  {69, kParamLfoDest, 3},     {69, kParamLfoAmount, 22},  {69, kParamVelToAmp, 42},
  {69, kParamIns1Algo, 2},    {69, kParamIns1A, 28},      {69, kParamIns1B, 55},
  {69, kParamIns1C, 66},      {69, kParamIns1Mix, 34},    {69, kParamIns2Algo, 3},
  {69, kParamIns2A, 28},      {69, kParamIns2B, 18},      {69, kParamIns2C, 50},
  {69, kParamIns2Mix, 12},    {69, kParamMstAlgo, 3},     {69, kParamMstA, 60},
  {69, kParamMstB, 32},       {69, kParamMstC, 50},       {69, kParamMstMix, 22},

  // Family 04 / 003 Wave Clav
  {70, kParamOsc1Shape, 72},  {70, kParamOsc1PW, 42},     {70, kParamOsc2Shape, 80},
  {70, kParamOsc2Pitch, 12},  {70, kParamOsc2Level, 48},  {70, kParamFilterCutoff, 2600},
  {70, kParamFilterReso, 26}, {70, kParamFilterEnv, 50},  {70, kParamFilterKeyTrack, 35},
  {70, kParamAmpAttack, 2},   {70, kParamAmpDecay, 220},  {70, kParamAmpSustain, 0},
  {70, kParamAmpRelease, 180}, {70, kParamLfoRate, 0.7},   {70, kParamLfoDest, 1},
  {70, kParamLfoAmount, 16},  {70, kParamVelToAmp, 48},
  {70, kParamIns1Algo, 2},    {70, kParamIns1A, 20},      {70, kParamIns1B, 36},
  {70, kParamIns1C, 48},      {70, kParamIns1Mix, 20},    {70, kParamIns2Algo, 1},
  {70, kParamIns2A, 34},      {70, kParamIns2B, 48},      {70, kParamIns2C, 42},
  {70, kParamIns2Mix, 18},    {70, kParamMstAlgo, 1},     {70, kParamMstA, 54},
  {70, kParamMstB, 42},       {70, kParamMstC, 24},       {70, kParamMstMix, 24},

  // Family 04 / 004 Drift Organ
  {71, kParamOsc1Shape, 33},  {71, kParamOsc1Level, 80},  {71, kParamOsc2Shape, 40},
  {71, kParamOsc2Pitch, 7},   {71, kParamOsc2Level, 70},  {71, kParamSubLevel, 30},
  {71, kParamUnisonVoices, 2}, {71, kParamUnisonDetune, 18}, {71, kParamFilterCutoff, 5400},
  {71, kParamFilterReso, 12}, {71, kParamAmpAttack, 8},   {71, kParamAmpSustain, 100},
  {71, kParamAmpRelease, 200}, {71, kParamLfoRate, 0.25},  {71, kParamLfoDest, 1},
  {71, kParamLfoAmount, 20},
  {71, kParamIns1Algo, 2},    {71, kParamIns1A, 14},      {71, kParamIns1B, 60},
  {71, kParamIns1C, 70},      {71, kParamIns1Mix, 32},    {71, kParamIns2Algo, 3},
  {71, kParamIns2A, 24},      {71, kParamIns2B, 16},      {71, kParamIns2C, 46},
  {71, kParamIns2Mix, 12},    {71, kParamMstAlgo, 3},     {71, kParamMstA, 62},
  {71, kParamMstB, 28},       {71, kParamMstC, 50},       {71, kParamMstMix, 24},

  // Family 04 / 005 Chime Bells
  {72, kParamOsc1Shape, 8},   {72, kParamOsc2Shape, 20},  {72, kParamOsc2Pitch, 19},
  {72, kParamOsc2Level, 55},  {72, kParamFmAmount, 18},   {72, kParamFilterCutoff, 8600},
  {72, kParamFilterKeyTrack, 45}, {72, kParamAmpAttack, 4},   {72, kParamAmpDecay, 2600},
  {72, kParamAmpSustain, 15}, {72, kParamAmpRelease, 2800}, {72, kParamLfoRate, 0.12},
  {72, kParamLfoDest, 2},     {72, kParamLfoAmount, 8},   {72, kParamSynthMasterLevel, -4},
  {72, kParamIns1Algo, 3},    {72, kParamIns1A, 36},      {72, kParamIns1B, 28},
  {72, kParamIns1C, 72},      {72, kParamIns1Mix, 22},    {72, kParamIns2Algo, 2},
  {72, kParamIns2A, 12},      {72, kParamIns2B, 30},      {72, kParamIns2C, 74},
  {72, kParamIns2Mix, 20},    {72, kParamMstAlgo, 3},     {72, kParamMstA, 66},
  {72, kParamMstB, 26},       {72, kParamMstC, 50},       {72, kParamMstMix, 26},

  // Family 04 / 006 Harp Pluck
  {73, kParamOsc1Shape, 12},  {73, kParamOsc2Shape, 18},  {73, kParamOsc2Pitch, 12},
  {73, kParamOsc2Level, 42},  {73, kParamFmAmount, 6},    {73, kParamFilterCutoff, 6400},
  {73, kParamFilterReso, 8},  {73, kParamFilterEnv, 30},  {73, kParamFilterKeyTrack, 65},
  {73, kParamAmpAttack, 1},   {73, kParamAmpDecay, 500},  {73, kParamAmpSustain, 0},
  {73, kParamAmpRelease, 700},
  {73, kParamIns1Algo, 2},    {73, kParamIns1A, 18},      {73, kParamIns1B, 24},
  {73, kParamIns1C, 58},      {73, kParamIns1Mix, 18},    {73, kParamIns2Algo, 3},
  {73, kParamIns2A, 26},      {73, kParamIns2B, 16},      {73, kParamIns2C, 62},
  {73, kParamIns2Mix, 14},    {73, kParamMstAlgo, 3},     {73, kParamMstA, 60},
  {73, kParamMstB, 30},       {73, kParamMstC, 50},       {73, kParamMstMix, 22},

  // Family 04 / 007 Soft Stab
  {74, kParamOsc1Shape, 30},  {74, kParamOsc2Shape, 48},  {74, kParamOsc2Pitch, 7},
  {74, kParamOsc2Level, 50},  {74, kParamFilterCutoff, 4200}, {74, kParamFilterReso, 14},
  {74, kParamFilterEnv, 40},  {74, kParamAmpAttack, 6},   {74, kParamAmpDecay, 240},
  {74, kParamAmpSustain, 0},  {74, kParamAmpRelease, 280}, {74, kParamUnisonVoices, 2},
  {74, kParamUnisonDetune, 16}, {74, kParamVelToAmp, 40},
  {74, kParamIns1Algo, 2},    {74, kParamIns1A, 12},      {74, kParamIns1B, 30},
  {74, kParamIns1C, 52},      {74, kParamIns1Mix, 16},    {74, kParamIns2Algo, 3},
  {74, kParamIns2A, 18},      {74, kParamIns2B, 14},      {74, kParamIns2C, 50},
  {74, kParamIns2Mix, 12},    {74, kParamMstAlgo, 1},     {74, kParamMstA, 52},
  {74, kParamMstB, 40},       {74, kParamMstC, 22},       {74, kParamMstMix, 24},

  // Family 04 / 008 Sub Bass
  {75, kParamPolyMode, 1},    {75, kParamOsc1Shape, 8},   {75, kParamOsc1Pitch, -12},
  {75, kParamOsc2Shape, 14},  {75, kParamOsc2Pitch, -12}, {75, kParamOsc2Level, 30},
  {75, kParamSubLevel, 90},   {75, kParamFilterCutoff, 480}, {75, kParamFilterReso, 18},
  {75, kParamFilterEnv, 25},  {75, kParamAmpAttack, 8},   {75, kParamAmpDecay, 500},
  {75, kParamAmpSustain, 85}, {75, kParamAmpRelease, 300}, {75, kParamLfoRate, 0.2},
  {75, kParamLfoDest, 1},     {75, kParamLfoAmount, 10},  {75, kParamFilterDrive, 8},
  {75, kParamIns1Algo, 1},    {75, kParamIns1A, 24},      {75, kParamIns1B, 30},
  {75, kParamIns1C, 48},      {75, kParamIns1Mix, 16},    {75, kParamMstAlgo, 1},
  {75, kParamMstA, 62},       {75, kParamMstB, 44},       {75, kParamMstC, 20},
  {75, kParamMstMix, 26},

  // Family 04 / 009 Air Lead
  {76, kParamPolyMode, 1},    {76, kParamPorta, 80},      {76, kParamOsc1Shape, 22},
  {76, kParamOsc2Shape, 36},  {76, kParamOsc2Pitch, 12},  {76, kParamOsc2Level, 48},
  {76, kParamNoiseLevel, 8},  {76, kParamUnisonVoices, 2}, {76, kParamUnisonDetune, 14},
  {76, kParamFilterCutoff, 6200}, {76, kParamFilterReso, 10}, {76, kParamFilterKeyTrack, 25},
  {76, kParamAmpAttack, 30},  {76, kParamAmpDecay, 600},  {76, kParamAmpSustain, 70},
  {76, kParamAmpRelease, 500}, {76, kParamLfoRate, 0.35},  {76, kParamLfoDest, 2},
  {76, kParamLfoAmount, 12},  {76, kParamModWheelDest, 1}, {76, kParamModWheelAmount, 28},
  {76, kParamIns1Algo, 3},    {76, kParamIns1A, 32},      {76, kParamIns1B, 24},
  {76, kParamIns1C, 60},      {76, kParamIns1Mix, 20},    {76, kParamIns2Algo, 2},
  {76, kParamIns2A, 16},      {76, kParamIns2B, 26},      {76, kParamIns2C, 66},
  {76, kParamIns2Mix, 16},    {76, kParamMstAlgo, 3},     {76, kParamMstA, 58},
  {76, kParamMstB, 32},       {76, kParamMstC, 50},       {76, kParamMstMix, 22},

  // Family 04 / 010 Swell Brass
  {77, kParamOsc1Shape, 40},  {77, kParamOsc2Shape, 52},  {77, kParamOsc2Pitch, 7},
  {77, kParamOsc2Level, 72},  {77, kParamUnisonVoices, 4}, {77, kParamUnisonDetune, 26},
  {77, kParamFilterCutoff, 1600}, {77, kParamFilterReso, 16}, {77, kParamFilterEnv, 60},
  {77, kParamFiltAttack, 500}, {77, kParamFiltDecay, 1200}, {77, kParamFiltSustain, 55},
  {77, kParamFiltRelease, 1400}, {77, kParamAmpAttack, 280}, {77, kParamAmpDecay, 1400},
  {77, kParamAmpSustain, 78}, {77, kParamAmpRelease, 900}, {77, kParamLfoRate, 0.15},
  {77, kParamLfoDest, 1},     {77, kParamLfoAmount, 14},  {77, kParamVelToFilter, 35},
  {77, kParamIns1Algo, 2},    {77, kParamIns1A, 10},      {77, kParamIns1B, 40},
  {77, kParamIns1C, 56},      {77, kParamIns1Mix, 24},    {77, kParamIns2Algo, 3},
  {77, kParamIns2A, 40},      {77, kParamIns2B, 22},      {77, kParamIns2C, 48},
  {77, kParamIns2Mix, 14},    {77, kParamMstAlgo, 3},     {77, kParamMstA, 60},
  {77, kParamMstB, 30},       {77, kParamMstC, 50},       {77, kParamMstMix, 24},

  // Family 04 / 011 Bowed Strings
  {78, kParamOsc1Shape, 34},  {78, kParamOsc2Shape, 28},  {78, kParamOsc2Pitch, 12},
  {78, kParamOsc2Level, 78},  {78, kParamUnisonVoices, 6}, {78, kParamUnisonDetune, 34},
  {78, kParamFilterCutoff, 2400}, {78, kParamFilterReso, 8},  {78, kParamFilterEnv, 15},
  {78, kParamAmpAttack, 1100}, {78, kParamAmpDecay, 2600}, {78, kParamAmpSustain, 86},
  {78, kParamAmpRelease, 2600}, {78, kParamLfoRate, 0.08},  {78, kParamLfoDest, 1},
  {78, kParamLfoAmount, 10},  {78, kParamSynthMasterLevel, -3},
  {78, kParamIns1Algo, 2},    {78, kParamIns1A, 8},       {78, kParamIns1B, 50},
  {78, kParamIns1C, 60},      {78, kParamIns1Mix, 30},    {78, kParamIns2Algo, 3},
  {78, kParamIns2A, 46},      {78, kParamIns2B, 26},      {78, kParamIns2C, 46},
  {78, kParamIns2Mix, 16},    {78, kParamMstAlgo, 3},     {78, kParamMstA, 68},
  {78, kParamMstB, 28},       {78, kParamMstC, 50},       {78, kParamMstMix, 24},

  // Family 04 / 012 Air Choir
  {79, kParamOsc1Shape, 28},  {79, kParamOsc2Shape, 45},  {79, kParamOsc2Pitch, 12},
  {79, kParamUnisonVoices, 6}, {79, kParamUnisonDetune, 30}, {79, kParamFilterCutoff, 5500},
  {79, kParamFilterReso, 14}, {79, kParamAmpAttack, 700}, {79, kParamAmpDecay, 2000},
  {79, kParamAmpSustain, 85}, {79, kParamAmpRelease, 2600}, {79, kParamTapeHiss, 18},
  {79, kParamTapeMix, 40},
  {79, kParamIns1Algo, 2},    {79, kParamIns1A, 12},      {79, kParamIns1B, 44},
  {79, kParamIns1C, 68},      {79, kParamIns1Mix, 28},    {79, kParamIns2Algo, 3},
  {79, kParamIns2A, 50},      {79, kParamIns2B, 24},      {79, kParamIns2C, 44},
  {79, kParamIns2Mix, 18},    {79, kParamMstAlgo, 3},     {79, kParamMstA, 66},
  {79, kParamMstB, 26},       {79, kParamMstC, 50},       {79, kParamMstMix, 26},

  // Family 04 / 013 Cloud Pad
  {80, kParamOsc1Shape, 16},  {80, kParamOsc2Shape, 58},  {80, kParamOsc2Pitch, 5},
  {80, kParamOsc2Level, 62},  {80, kParamUnisonVoices, 5}, {80, kParamUnisonDetune, 42},
  {80, kParamFilterCutoff, 2800}, {80, kParamFilterReso, 18}, {80, kParamFilterEnv, 30},
  {80, kParamAmpAttack, 1600}, {80, kParamAmpDecay, 3200}, {80, kParamAmpSustain, 74},
  {80, kParamAmpRelease, 3600}, {80, kParamLfoRate, 0.06},  {80, kParamLfoShape, 1},
  {80, kParamLfoDest, 1},     {80, kParamLfoAmount, 26},  {80, kParamSynthMasterLevel, -4},
  {80, kParamIns1Algo, 2},    {80, kParamIns1A, 12},      {80, kParamIns1B, 62},
  {80, kParamIns1C, 72},      {80, kParamIns1Mix, 36},    {80, kParamIns2Algo, 3},
  {80, kParamIns2A, 58},      {80, kParamIns2B, 34},      {80, kParamIns2C, 52},
  {80, kParamIns2Mix, 22},    {80, kParamMstAlgo, 3},     {80, kParamMstA, 70},
  {80, kParamMstB, 24},       {80, kParamMstC, 50},       {80, kParamMstMix, 30},

  // Family 04 / 014 Sparse Pad
  {81, kParamOsc1Shape, 12},  {81, kParamOsc1Level, 70},  {81, kParamOsc2Shape, 20},
  {81, kParamOsc2Pitch, 12},  {81, kParamOsc2Level, 18},  {81, kParamUnisonVoices, 2},
  {81, kParamUnisonDetune, 10}, {81, kParamFilterCutoff, 4000}, {81, kParamFilterReso, 6},
  {81, kParamAmpAttack, 1500}, {81, kParamAmpDecay, 2000}, {81, kParamAmpSustain, 45},
  {81, kParamAmpRelease, 2400}, {81, kParamTapeMix, 14},    {81, kParamSynthMasterLevel, -6},
  {81, kParamIns1Algo, 2},    {81, kParamIns1A, 8},       {81, kParamIns1B, 18},
  {81, kParamIns1C, 46},      {81, kParamIns1Mix, 12},    {81, kParamIns2Algo, 3},
  {81, kParamIns2A, 42},      {81, kParamIns2B, 16},      {81, kParamIns2C, 40},
  {81, kParamIns2Mix, 10},    {81, kParamMstAlgo, 3},     {81, kParamMstA, 56},
  {81, kParamMstB, 30},       {81, kParamMstC, 50},       {81, kParamMstMix, 16},

  // Family 04 / 015 Motion Pad
  {82, kParamOsc1Shape, 12},  {82, kParamOsc2Shape, 60},  {82, kParamOsc2Pitch, 7},
  {82, kParamUnisonVoices, 5}, {82, kParamUnisonDetune, 55}, {82, kParamFilterCutoff, 2600},
  {82, kParamFilterReso, 20}, {82, kParamFilterEnv, 65},  {82, kParamAmpAttack, 900},
  {82, kParamAmpDecay, 3000}, {82, kParamAmpSustain, 70}, {82, kParamAmpRelease, 3500},
  {82, kParamFiltAttack, 1200}, {82, kParamFiltDecay, 3000}, {82, kParamFiltSustain, 60},
  {82, kParamFiltRelease, 3000}, {82, kParamLfoRate, 0.25},  {82, kParamLfoDest, 1},
  {82, kParamLfoAmount, 40},
  {82, kParamIns1Algo, 2},    {82, kParamIns1A, 24},      {82, kParamIns1B, 58},
  {82, kParamIns1C, 82},      {82, kParamIns1Mix, 35},    {82, kParamIns2Algo, 3},
  {82, kParamIns2A, 60},      {82, kParamIns2B, 38},      {82, kParamIns2C, 48},
  {82, kParamIns2Mix, 24},    {82, kParamMstAlgo, 3},     {82, kParamMstA, 72},
  {82, kParamMstB, 28},       {82, kParamMstC, 50},       {82, kParamMstMix, 55},

  // Family 04 / 016 Tilt Texture
  {83, kParamOsc1Shape, 46},  {83, kParamOsc2Shape, 74},  {83, kParamOsc2Pitch, 7},
  {83, kParamOsc2Level, 58},  {83, kParamOscRing, 1},     {83, kParamNoiseLevel, 10},
  {83, kParamFilterType, 2},  {83, kParamFilterCutoff, 1100}, {83, kParamFilterReso, 36},
  {83, kParamUnisonVoices, 3}, {83, kParamUnisonDetune, 38}, {83, kParamAmpAttack, 400},
  {83, kParamAmpDecay, 2200}, {83, kParamAmpSustain, 58}, {83, kParamAmpRelease, 2000},
  {83, kParamLfoRate, 0.9},   {83, kParamLfoShape, 2},    {83, kParamLfoDest, 2},
  {83, kParamLfoAmount, 18},
  {83, kParamIns1Algo, 4},    {83, kParamIns1A, 28},      {83, kParamIns1B, 22},
  {83, kParamIns1C, 30},      {83, kParamIns1Mix, 16},    {83, kParamIns2Algo, 3},
  {83, kParamIns2A, 40},      {83, kParamIns2B, 30},      {83, kParamIns2C, 44},
  {83, kParamIns2Mix, 18},    {83, kParamMstAlgo, 4},     {83, kParamMstA, 72},
  {83, kParamMstB, 35},       {83, kParamMstC, 20},       {83, kParamMstMix, 48},

  // Family 04 / 017 Slow Drone
  {84, kParamOsc1Shape, 5},   {84, kParamOsc2Shape, 50},  {84, kParamOsc2Pitch, -12},
  {84, kParamSubLevel, 60},   {84, kParamNoiseLevel, 12}, {84, kParamFilterCutoff, 1200},
  {84, kParamFilterReso, 35}, {84, kParamFilterEnv, 20},  {84, kParamAmpAttack, 2000},
  {84, kParamAmpDecay, 4000}, {84, kParamAmpSustain, 95}, {84, kParamAmpRelease, 4000},
  {84, kParamIns1Algo, 2},    {84, kParamIns1A, 6},       {84, kParamIns1B, 34},
  {84, kParamIns1C, 50},      {84, kParamIns1Mix, 20},    {84, kParamIns2Algo, 3},
  {84, kParamIns2A, 64},      {84, kParamIns2B, 36},      {84, kParamIns2C, 30},
  {84, kParamIns2Mix, 16},    {84, kParamMstAlgo, 4},     {84, kParamMstA, 42},
  {84, kParamMstB, 28},       {84, kParamMstC, 10},       {84, kParamMstMix, 22},

  // Family 04 / 018 Seq Arp
  {85, kParamOsc1Shape, 54},  {85, kParamOsc2Shape, 60},  {85, kParamOsc2Pitch, 12},
  {85, kParamOsc2Level, 45},  {85, kParamUnisonVoices, 2}, {85, kParamUnisonDetune, 20},
  {85, kParamFilterCutoff, 5200}, {85, kParamFilterReso, 22}, {85, kParamFilterEnv, 55},
  {85, kParamFilterKeyTrack, 55}, {85, kParamAmpAttack, 1},   {85, kParamAmpDecay, 70},
  {85, kParamAmpSustain, 0},  {85, kParamAmpRelease, 100}, {85, kParamLfoRate, 4},
  {85, kParamLfoShape, 3},    {85, kParamLfoDest, 1},     {85, kParamLfoAmount, 20},
  {85, kParamIns1Algo, 3},    {85, kParamIns1A, 16},      {85, kParamIns1B, 18},
  {85, kParamIns1C, 62},      {85, kParamIns1Mix, 12},    {85, kParamIns2Algo, 2},
  {85, kParamIns2A, 20},      {85, kParamIns2B, 20},      {85, kParamIns2C, 60},
  {85, kParamIns2Mix, 12},    {85, kParamMstAlgo, 3},     {85, kParamMstA, 58},
  {85, kParamMstB, 30},       {85, kParamMstC, 50},       {85, kParamMstMix, 20},

  // Family 04 / 019 Wash Delay
  {86, kParamOsc1Shape, 20},  {86, kParamOsc2Shape, 40},  {86, kParamOsc2Pitch, 7},
  {86, kParamOsc2Level, 55},  {86, kParamFilterCutoff, 3600}, {86, kParamFilterReso, 12},
  {86, kParamAmpAttack, 200}, {86, kParamAmpDecay, 1600}, {86, kParamAmpSustain, 40},
  {86, kParamAmpRelease, 2000}, {86, kParamSynthMasterLevel, -3},
  {86, kParamIns1Algo, 3},    {86, kParamIns1A, 68},      {86, kParamIns1B, 55},
  {86, kParamIns1C, 50},      {86, kParamIns1Mix, 62},    {86, kParamIns2Algo, 2},
  {86, kParamIns2A, 14},      {86, kParamIns2B, 46},      {86, kParamIns2C, 70},
  {86, kParamIns2Mix, 28},    {86, kParamMstAlgo, 3},     {86, kParamMstA, 74},
  {86, kParamMstB, 28},       {86, kParamMstC, 50},       {86, kParamMstMix, 36},

  // Family 04 / 020 Metal Kit
  {87, kParamEngine, 1},      {87, kParamOsc1Shape, 96},  {87, kParamOsc1PW, 22},
  {87, kParamOsc2Shape, 88},  {87, kParamOsc2Pitch, 7},   {87, kParamOsc2Level, 55},
  {87, kParamNoiseLevel, 50}, {87, kParamFilterType, 3},  {87, kParamFilterCutoff, 4000},
  {87, kParamFilterReso, 18}, {87, kParamAmpAttack, 0.5}, {87, kParamAmpDecay, 200},
  {87, kParamAmpSustain, 0},  {87, kParamAmpRelease, 180}, {87, kParamPitchAmount, -6},
  {87, kParamPitchDecay, 40}, {87, kParamPitchRelease, 40}, {87, kParamDrumSlotTune, -3},
  {87, kParamDrumSlotDecay, 62}, {87, kParamDrumSlotDrive, 35}, {87, kParamDrumSlotFilter, 16000},
  {87, kParamDrumKitLevel, -2}, {87, kParamFilterDrive, 22},
  {87, kParamIns1Algo, 4},    {87, kParamIns1A, 24},      {87, kParamIns1B, 20},
  {87, kParamIns1C, 34},      {87, kParamIns1Mix, 18},    {87, kParamIns2Algo, 1},
  {87, kParamIns2A, 46},      {87, kParamIns2B, 56},      {87, kParamIns2C, 40},
  {87, kParamIns2Mix, 22},    {87, kParamMstAlgo, 2},     {87, kParamMstA, 32},
  {87, kParamMstB, 44},       {87, kParamMstC, 18},       {87, kParamMstMix, 48},

  // Family 05 / 001 Stage Keys
  {88, kParamOsc1Shape, 24},  {88, kParamOsc2Shape, 16},  {88, kParamOsc2Pitch, 12},
  {88, kParamOsc2Level, 42},  {88, kParamFilterCutoff, 7000}, {88, kParamFilterReso, 6},
  {88, kParamFilterEnv, 16},  {88, kParamAmpAttack, 5},   {88, kParamAmpDecay, 850},
  {88, kParamAmpSustain, 36}, {88, kParamAmpRelease, 550}, {88, kParamVelToAmp, 32},
  {88, kParamTapeSat, 16},    {88, kParamTapeMix, 18},
  {88, kParamIns1Algo, 2},    {88, kParamIns1A, 11},      {88, kParamIns1B, 18},
  {88, kParamIns1C, 40},      {88, kParamIns1Mix, 14},    {88, kParamMstAlgo, 1},
  {88, kParamMstA, 60},       {88, kParamMstB, 36},       {88, kParamMstC, 34},
  {88, kParamMstMix, 28},

  // Family 05 / 002 Reed Piano
  {89, kParamOsc1Shape, 30},  {89, kParamOsc2Shape, 24},  {89, kParamOsc2Pitch, 19},
  {89, kParamOsc2Level, 28},  {89, kParamFmAmount, 14},   {89, kParamFilterCutoff, 3000},
  {89, kParamFilterReso, 9},  {89, kParamFilterEnv, 16},  {89, kParamAmpAttack, 5},
  {89, kParamAmpDecay, 1000}, {89, kParamAmpSustain, 32}, {89, kParamAmpRelease, 640},
  {89, kParamVelToAmp, 48},   {89, kParamVelToFilter, 18}, {89, kParamTapeMix, 20},
  {89, kParamIns1Algo, 2},    {89, kParamIns1A, 9},       {89, kParamIns1B, 14},
  {89, kParamIns1C, 36},      {89, kParamIns1Mix, 10},    {89, kParamMstAlgo, 4},
  {89, kParamMstA, 58},       {89, kParamMstB, 42},       {89, kParamMstC, 12},
  {89, kParamMstMix, 18},

  // Family 05 / 003 Funk Clav
  {90, kParamOsc1Shape, 80},  {90, kParamOsc1PW, 26},     {90, kParamOsc2Shape, 86},
  {90, kParamOsc2Pitch, 12},  {90, kParamOsc2Level, 58},  {90, kParamOsc2PW, 20},
  {90, kParamFilterCutoff, 2500}, {90, kParamFilterReso, 36}, {90, kParamFilterEnv, 66},
  {90, kParamFilterDrive, 16}, {90, kParamFilterKeyTrack, 28}, {90, kParamFiltAttack, 1},
  {90, kParamFiltDecay, 130}, {90, kParamFiltSustain, 0}, {90, kParamFiltRelease, 110},
  {90, kParamAmpAttack, 1},   {90, kParamAmpDecay, 190},  {90, kParamAmpSustain, 0},
  {90, kParamAmpRelease, 140}, {90, kParamVelToAmp, 72},   {90, kParamTapeSat, 18},
  {90, kParamTapeMix, 14},
  {90, kParamIns1Algo, 1},    {90, kParamIns1A, 34},      {90, kParamIns1B, 46},
  {90, kParamIns1C, 42},      {90, kParamIns1Mix, 22},    {90, kParamMstAlgo, 1},
  {90, kParamMstA, 56},       {90, kParamMstB, 44},       {90, kParamMstC, 24},
  {90, kParamMstMix, 26},

  // Family 05 / 004 Rock Organ
  {91, kParamOsc1Shape, 36},  {91, kParamOsc1Level, 100}, {91, kParamOsc2Shape, 33},
  {91, kParamOsc2Pitch, 12},  {91, kParamOsc2Level, 78},  {91, kParamSubLevel, 28},
  {91, kParamFilterCutoff, 6800}, {91, kParamFilterReso, 14}, {91, kParamFilterEnv, 35},
  {91, kParamFiltAttack, 1},  {91, kParamFiltDecay, 90},  {91, kParamFiltSustain, 30},
  {91, kParamFiltRelease, 80}, {91, kParamAmpAttack, 2},   {91, kParamAmpDecay, 40},
  {91, kParamAmpSustain, 100}, {91, kParamAmpRelease, 70}, {91, kParamFilterDrive, 18},
  {91, kParamLfoRate, 6},     {91, kParamLfoDest, 1},     {91, kParamLfoAmount, 6},
  {91, kParamIns1Algo, 1},    {91, kParamIns1A, 42},      {91, kParamIns1B, 38},
  {91, kParamIns1C, 46},      {91, kParamIns1Mix, 20},    {91, kParamMstAlgo, 1},
  {91, kParamMstA, 54},       {91, kParamMstB, 40},       {91, kParamMstC, 32},
  {91, kParamMstMix, 26},

  // Family 05 / 005 Ping Bells
  {92, kParamOsc1Shape, 9},   {92, kParamOsc2Shape, 14},  {92, kParamOsc2Pitch, 24},
  {92, kParamOsc2Level, 36},  {92, kParamFmAmount, 20},   {92, kParamFilterCutoff, 12000},
  {92, kParamFilterKeyTrack, 70}, {92, kParamAmpAttack, 1},   {92, kParamAmpDecay, 900},
  {92, kParamAmpSustain, 0},  {92, kParamAmpRelease, 1100}, {92, kParamSynthMasterLevel, -3},
  {92, kParamIns1Algo, 3},    {92, kParamIns1A, 12},      {92, kParamIns1B, 8},
  {92, kParamIns1C, 80},      {92, kParamIns1Mix, 8},     {92, kParamMstAlgo, 3},
  {92, kParamMstA, 60},       {92, kParamMstB, 28},       {92, kParamMstC, 50},
  {92, kParamMstMix, 20},

  // Family 05 / 006 Mute Pluck
  {93, kParamOsc1Shape, 52},  {93, kParamOsc1PW, 38},     {93, kParamOsc2Level, 10},
  {93, kParamFilterCutoff, 1800}, {93, kParamFilterReso, 12}, {93, kParamFilterEnv, 48},
  {93, kParamFilterKeyTrack, 80}, {93, kParamFiltAttack, 1},  {93, kParamFiltDecay, 60},
  {93, kParamFiltSustain, 0}, {93, kParamFiltRelease, 50}, {93, kParamAmpAttack, 1},
  {93, kParamAmpDecay, 90},   {93, kParamAmpSustain, 0},  {93, kParamAmpRelease, 70},
  {93, kParamTapeMix, 8},
  {93, kParamIns1Algo, 1},    {93, kParamIns1A, 20},      {93, kParamIns1B, 44},
  {93, kParamIns1C, 50},      {93, kParamIns1Mix, 10},    {93, kParamMstAlgo, 1},
  {93, kParamMstA, 58},       {93, kParamMstB, 30},       {93, kParamMstC, 22},
  {93, kParamMstMix, 18},

  // Family 05 / 007 Short Stab
  {94, kParamOsc1Shape, 58},  {94, kParamOsc2Shape, 62},  {94, kParamOsc2Pitch, 12},
  {94, kParamOsc2Level, 48},  {94, kParamFilterCutoff, 4600}, {94, kParamFilterReso, 20},
  {94, kParamFilterEnv, 58},  {94, kParamAmpAttack, 1},   {94, kParamAmpDecay, 80},
  {94, kParamAmpSustain, 0},  {94, kParamAmpRelease, 60}, {94, kParamVelToAmp, 68},
  {94, kParamVelToFilter, 50}, {94, kParamFilterDrive, 14}, {94, kParamTapeSat, 12},
  {94, kParamTapeMix, 10},
  {94, kParamIns1Algo, 1},    {94, kParamIns1A, 28},      {94, kParamIns1B, 48},
  {94, kParamIns1C, 44},      {94, kParamIns1Mix, 16},    {94, kParamMstAlgo, 1},
  {94, kParamMstA, 52},       {94, kParamMstB, 46},       {94, kParamMstC, 20},
  {94, kParamMstMix, 26},

  // Family 05 / 008 Round Bass
  {95, kParamPolyMode, 1},    {95, kParamMonoLegato, 1},  {95, kParamOsc1Shape, 18},
  {95, kParamOsc1Pitch, -12}, {95, kParamOsc2Shape, 12},  {95, kParamOsc2Pitch, -12},
  {95, kParamOsc2Level, 35},  {95, kParamSubLevel, 65},   {95, kParamFilterCutoff, 980},
  {95, kParamFilterReso, 16}, {95, kParamFilterEnv, 32},  {95, kParamAmpAttack, 3},
  {95, kParamAmpDecay, 340},  {95, kParamAmpSustain, 78}, {95, kParamAmpRelease, 200},
  {95, kParamVelToAmp, 36},   {95, kParamTapeSat, 14},    {95, kParamTapeMix, 12},
  {95, kParamIns1Algo, 1},    {95, kParamIns1A, 24},      {95, kParamIns1B, 55},
  {95, kParamIns1C, 48},      {95, kParamIns1Mix, 18},    {95, kParamMstAlgo, 1},
  {95, kParamMstA, 62},       {95, kParamMstB, 40},       {95, kParamMstC, 22},
  {95, kParamMstMix, 26},

  // Family 05 / 009 Saw Lead
  {96, kParamEngine, 1},      {96, kParamPolyMode, 1},    {96, kParamPorta, 40},
  {96, kParamOsc1Shape, 70},  {96, kParamOsc2Shape, 66},  {96, kParamOsc2Pitch, 0.01},
  {96, kParamOsc2Level, 60},  {96, kParamUnisonVoices, 2}, {96, kParamUnisonDetune, 8},
  {96, kParamFilterCutoff, 3900}, {96, kParamFilterReso, 22}, {96, kParamFilterEnv, 30},
  {96, kParamFilterDrive, 20}, {96, kParamFilterKeyTrack, 35}, {96, kParamAmpAttack, 8},
  {96, kParamAmpDecay, 240},  {96, kParamAmpSustain, 82}, {96, kParamAmpRelease, 180},
  {96, kParamModWheelDest, 1}, {96, kParamModWheelAmount, 32},
  {96, kParamIns1Algo, 1},    {96, kParamIns1A, 46},      {96, kParamIns1B, 40},
  {96, kParamIns1C, 38},      {96, kParamIns1Mix, 24},    {96, kParamMstAlgo, 1},
  {96, kParamMstA, 56},       {96, kParamMstB, 38},       {96, kParamMstC, 30},
  {96, kParamMstMix, 24},

  // Family 05 / 010 Section Brass
  {97, kParamOsc1Shape, 50},  {97, kParamOsc2Shape, 54},  {97, kParamOsc2Pitch, 12},
  {97, kParamOsc2Level, 64},  {97, kParamUnisonVoices, 2}, {97, kParamUnisonDetune, 10},
  {97, kParamFilterCutoff, 2200}, {97, kParamFilterReso, 12}, {97, kParamFilterEnv, 52},
  {97, kParamFiltAttack, 25}, {97, kParamFiltDecay, 220}, {97, kParamFiltSustain, 50},
  {97, kParamFiltRelease, 300}, {97, kParamAmpAttack, 22},  {97, kParamAmpDecay, 400},
  {97, kParamAmpSustain, 74}, {97, kParamAmpRelease, 280}, {97, kParamVelToFilter, 42},
  {97, kParamIns1Algo, 2},    {97, kParamIns1A, 8},       {97, kParamIns1B, 16},
  {97, kParamIns1C, 44},      {97, kParamIns1Mix, 12},    {97, kParamMstAlgo, 1},
  {97, kParamMstA, 56},       {97, kParamMstB, 42},       {97, kParamMstC, 38},
  {97, kParamMstMix, 24},

  // Family 05 / 011 Small Strings
  {98, kParamOsc1Shape, 38},  {98, kParamOsc2Shape, 34},  {98, kParamOsc2Pitch, 12},
  {98, kParamOsc2Level, 55},  {98, kParamUnisonVoices, 2}, {98, kParamUnisonDetune, 9},
  {98, kParamFilterCutoff, 3400}, {98, kParamFilterReso, 5},  {98, kParamAmpAttack, 240},
  {98, kParamAmpDecay, 900},  {98, kParamAmpSustain, 72}, {98, kParamAmpRelease, 800},
  {98, kParamTapeMix, 16},    {98, kParamSynthMasterLevel, -2},
  {98, kParamIns1Algo, 2},    {98, kParamIns1A, 7},       {98, kParamIns1B, 20},
  {98, kParamIns1C, 42},      {98, kParamIns1Mix, 14},    {98, kParamMstAlgo, 3},
  {98, kParamMstA, 58},       {98, kParamMstB, 30},       {98, kParamMstC, 50},
  {98, kParamMstMix, 20},

  // Family 05 / 012 Close Choir
  {99, kParamOsc1Shape, 21},  {99, kParamOsc2Shape, 27},  {99, kParamOsc2Pitch, 12},
  {99, kParamOsc2Level, 40},  {99, kParamNoiseLevel, 4},  {99, kParamUnisonVoices, 2},
  {99, kParamUnisonDetune, 8}, {99, kParamFilterCutoff, 4200}, {99, kParamFilterReso, 7},
  {99, kParamAmpAttack, 280}, {99, kParamAmpDecay, 1200}, {99, kParamAmpSustain, 80},
  {99, kParamAmpRelease, 900}, {99, kParamTapeHiss, 4},    {99, kParamTapeMix, 18},
  {99, kParamIns1Algo, 2},    {99, kParamIns1A, 6},       {99, kParamIns1B, 14},
  {99, kParamIns1C, 40},      {99, kParamIns1Mix, 10},    {99, kParamMstAlgo, 3},
  {99, kParamMstA, 56},       {99, kParamMstB, 30},       {99, kParamMstC, 50},
  {99, kParamMstMix, 18},

  // Family 05 / 013 Tape Pad
  {100, kParamOsc1Shape, 26},  {100, kParamOsc2Shape, 32},  {100, kParamOsc2Pitch, 7},
  {100, kParamOsc2Level, 50},  {100, kParamUnisonVoices, 2}, {100, kParamUnisonDetune, 12},
  {100, kParamFilterCutoff, 2600}, {100, kParamFilterReso, 10}, {100, kParamAmpAttack, 600},
  {100, kParamAmpDecay, 1800}, {100, kParamAmpSustain, 66}, {100, kParamAmpRelease, 2000},
  {100, kParamTapeSat, 32},    {100, kParamTapeBump, 28},   {100, kParamTapeWow, 18},
  {100, kParamTapeFlutter, 14}, {100, kParamTapeMix, 42},
  {100, kParamIns1Algo, 3},    {100, kParamIns1A, 30},      {100, kParamIns1B, 20},
  {100, kParamIns1C, 46},      {100, kParamIns1Mix, 14},    {100, kParamMstAlgo, 3},
  {100, kParamMstA, 62},       {100, kParamMstB, 28},       {100, kParamMstC, 50},
  {100, kParamMstMix, 22},

  // Family 05 / 014 Filter Motion
  {101, kParamOsc1Shape, 48},  {101, kParamOsc2Shape, 56},  {101, kParamOsc2Pitch, 5},
  {101, kParamOsc2Level, 60},  {101, kParamFilterCutoff, 700}, {101, kParamFilterReso, 28},
  {101, kParamFilterEnv, 70},  {101, kParamFiltAttack, 800}, {101, kParamFiltDecay, 1600},
  {101, kParamFiltSustain, 20}, {101, kParamFiltRelease, 1400}, {101, kParamAmpAttack, 120},
  {101, kParamAmpDecay, 2000}, {101, kParamAmpSustain, 64}, {101, kParamAmpRelease, 1800},
  {101, kParamLfoRate, 0.16},  {101, kParamLfoShape, 2},    {101, kParamLfoDest, 1},
  {101, kParamLfoAmount, 36},
  {101, kParamIns1Algo, 2},    {101, kParamIns1A, 18},      {101, kParamIns1B, 28},
  {101, kParamIns1C, 52},      {101, kParamIns1Mix, 18},    {101, kParamMstAlgo, 3},
  {101, kParamMstA, 60},       {101, kParamMstB, 30},       {101, kParamMstC, 50},
  {101, kParamMstMix, 24},

  // Family 05 / 015 Broken Texture
  {102, kParamEngine, 1},      {102, kParamOsc1Shape, 62},  {102, kParamOsc2Shape, 96},
  {102, kParamOsc2Pitch, 6},   {102, kParamOsc2Level, 44},  {102, kParamOscRing, 1},
  {102, kParamNoiseLevel, 18}, {102, kParamFilterType, 3},  {102, kParamFilterCutoff, 2400},
  {102, kParamFilterReso, 24}, {102, kParamAmpAttack, 40},  {102, kParamAmpDecay, 900},
  {102, kParamAmpSustain, 40}, {102, kParamAmpRelease, 700}, {102, kParamLfoRate, 7},
  {102, kParamLfoShape, 4},    {102, kParamLfoDest, 3},     {102, kParamLfoAmount, 28},
  {102, kParamIns1Algo, 4},    {102, kParamIns1A, 40},      {102, kParamIns1B, 30},
  {102, kParamIns1C, 36},      {102, kParamIns1Mix, 22},    {102, kParamMstAlgo, 4},
  {102, kParamMstA, 22},       {102, kParamMstB, 55},       {102, kParamMstC, 16},
  {102, kParamMstMix, 26},

  // Family 05 / 016 Dark Drone
  {103, kParamOsc1Shape, 4},   {103, kParamOsc2Shape, 10},  {103, kParamOsc2Pitch, -24},
  {103, kParamOsc2Level, 55},  {103, kParamSubLevel, 75},   {103, kParamNoiseLevel, 8},
  {103, kParamFilterCutoff, 420}, {103, kParamFilterReso, 30}, {103, kParamFilterEnv, 6},
  {103, kParamAmpAttack, 2400}, {103, kParamAmpDecay, 4000}, {103, kParamAmpSustain, 98},
  {103, kParamAmpRelease, 4000}, {103, kParamLfoRate, 0.04},  {103, kParamLfoDest, 1},
  {103, kParamLfoAmount, 12},  {103, kParamFilterDrive, 10}, {103, kParamSynthMasterLevel, -8},
  {103, kParamTapeSat, 20},    {103, kParamTapeMix, 24},
  {103, kParamIns1Algo, 2},    {103, kParamIns1A, 5},       {103, kParamIns1B, 24},
  {103, kParamIns1C, 38},      {103, kParamIns1Mix, 16},    {103, kParamMstAlgo, 4},
  {103, kParamMstA, 34},       {103, kParamMstB, 24},       {103, kParamMstC, 10},
  {103, kParamMstMix, 20},

  // Family 05 / 017 Tight Arp
  {104, kParamOsc1Shape, 60},  {104, kParamOsc2Shape, 48},  {104, kParamOsc2Pitch, 12},
  {104, kParamOsc2Level, 30},  {104, kParamFilterCutoff, 4400}, {104, kParamFilterReso, 18},
  {104, kParamFilterEnv, 62},  {104, kParamFilterKeyTrack, 72}, {104, kParamAmpAttack, 1},
  {104, kParamAmpDecay, 55},   {104, kParamAmpSustain, 0},  {104, kParamAmpRelease, 70},
  {104, kParamFiltAttack, 1},  {104, kParamFiltDecay, 40},  {104, kParamFiltSustain, 0},
  {104, kParamFiltRelease, 40}, {104, kParamTapeMix, 8},
  {104, kParamIns1Algo, 3},    {104, kParamIns1A, 10},      {104, kParamIns1B, 8},
  {104, kParamIns1C, 56},      {104, kParamIns1Mix, 6},     {104, kParamMstAlgo, 1},
  {104, kParamMstA, 52},       {104, kParamMstB, 30},       {104, kParamMstC, 20},
  {104, kParamMstMix, 16},

  // Family 05 / 018 Room Crush
  {105, kParamEngine, 1},      {105, kParamOsc1Shape, 66},  {105, kParamOsc2Shape, 74},
  {105, kParamOsc2Pitch, -12}, {105, kParamOsc2Level, 50},  {105, kParamFilterCutoff, 2800},
  {105, kParamFilterReso, 18}, {105, kParamFilterDrive, 36}, {105, kParamAmpAttack, 2},
  {105, kParamAmpDecay, 320},  {105, kParamAmpSustain, 25}, {105, kParamAmpRelease, 240},
  {105, kParamOutLevel, -2},   {105, kParamTapeSat, 22},    {105, kParamTapeMix, 12},
  {105, kParamIns1Algo, 4},    {105, kParamIns1A, 58},      {105, kParamIns1B, 46},
  {105, kParamIns1C, 32},      {105, kParamIns1Mix, 55},    {105, kParamMstAlgo, 2},
  {105, kParamMstA, 32},       {105, kParamMstB, 48},       {105, kParamMstC, 22},
  {105, kParamMstMix, 50},

  // Family 05 / 019 Dry Kit
  {106, kParamOsc1Shape, 92}, {106, kParamOsc1PW, 40},    {106, kParamOsc2Shape, 88},
  {106, kParamOsc2Level, 60}, {106, kParamSubLevel, 35},  {106, kParamNoiseLevel, 30},
  {106, kParamFilterCutoff, 7000}, {106, kParamFilterReso, 15}, {106, kParamFilterEnv, 40},
  {106, kParamAmpAttack, 0.5}, {106, kParamAmpDecay, 110}, {106, kParamAmpSustain, 0},
  {106, kParamAmpRelease, 90}, {106, kParamPitchAmount, -12}, {106, kParamPitchDecay, 70},
  {106, kParamTapeSat, 15},   {106, kParamTapeMix, 15},   {106, kParamDrumSlotDecay, 42},
  {106, kParamDrumSlotDrive, 22}, {106, kParamDrumSlotFilter, 10000}, {106, kParamDrumKitLevel, -1},
  {106, kParamIns1Algo, 1},   {106, kParamIns1A, 28},     {106, kParamIns1B, 50},
  {106, kParamIns1C, 46},     {106, kParamIns1Mix, 20},   {106, kParamMstAlgo, 2},
  {106, kParamMstA, 38},      {106, kParamMstB, 40},      {106, kParamMstC, 14},
  {106, kParamMstMix, 42},

  // Family 05 / 020 Hard Kit
  {107, kParamEngine, 1},     {107, kParamOsc1Shape, 100}, {107, kParamOsc1PW, 12},
  {107, kParamOsc1Level, 100}, {107, kParamOsc2Shape, 100}, {107, kParamOsc2Pitch, -12},
  {107, kParamOsc2Level, 85}, {107, kParamNoiseLevel, 45}, {107, kParamFilterCutoff, 12000},
  {107, kParamFilterReso, 10}, {107, kParamFilterEnv, 0},  {107, kParamAmpAttack, 0.5},
  {107, kParamAmpDecay, 160}, {107, kParamAmpSustain, 0}, {107, kParamAmpRelease, 120},
  {107, kParamPitchAmount, -18}, {107, kParamPitchAttack, 0.5}, {107, kParamPitchDecay, 90},
  {107, kParamPitchSustain, 0}, {107, kParamPitchRelease, 90}, {107, kParamTapeSat, 45},
  {107, kParamTapeMix, 12},   {107, kParamOutLevel, -1.5}, {107, kParamDrumSlotTune, -2},
  {107, kParamDrumSlotDecay, 26}, {107, kParamDrumSlotDrive, 78}, {107, kParamDrumSlotFilter, 17000},
  {107, kParamIns1Algo, 1},   {107, kParamIns1A, 62},     {107, kParamIns1B, 68},
  {107, kParamIns1C, 54},     {107, kParamIns1Mix, 42},   {107, kParamMstAlgo, 1},
  {107, kParamMstA, 38},      {107, kParamMstB, 65},      {107, kParamMstC, 32},
  {107, kParamMstMix, 70},

  // Family 05 / 021 Tape Kit
  {108, kParamOsc1Shape, 100}, {108, kParamOsc1PW, 50},    {108, kParamOsc2Shape, 55},
  {108, kParamOsc2Pitch, -24}, {108, kParamOsc2Level, 70}, {108, kParamNoiseLevel, 60},
  {108, kParamFilterCutoff, 5000}, {108, kParamFilterReso, 20}, {108, kParamFilterEnv, 60},
  {108, kParamAmpAttack, 0.5}, {108, kParamAmpDecay, 240}, {108, kParamAmpSustain, 0},
  {108, kParamAmpRelease, 260}, {108, kParamPitchAmount, -24}, {108, kParamPitchDecay, 140},
  {108, kParamTapeSat, 60},   {108, kParamTapeBump, 45},  {108, kParamTapeWow, 25},
  {108, kParamTapeFlutter, 30}, {108, kParamTapeMix, 55},   {108, kParamFilterDrive, 45},
  {108, kParamDrumSlotDecay, 75}, {108, kParamDrumSlotDrive, 50}, {108, kParamDrumSlotFilter, 5500},
  {108, kParamDrumKitLevel, -3},
  {108, kParamIns1Algo, 1},   {108, kParamIns1A, 48},     {108, kParamIns1B, 36},
  {108, kParamIns1C, 52},     {108, kParamIns1Mix, 30},   {108, kParamMstAlgo, 2},
  {108, kParamMstA, 30},      {108, kParamMstB, 44},      {108, kParamMstC, 20},
  {108, kParamMstMix, 45},



  // --- BEGIN measured level trims (tools/measure_preset_levels.py)
  // Target -12 LUFS momentary loudness, sample peak at or below -1.5 dBFS.
  // These sit last in kPresetDeltas on purpose: a later delta wins, so these
  // override the hand-written level values above.
  // Values are absolute, not offsets, because a preset delta replaces the value.
  {0, kParamOutLevel, +8.01}, {0, kParamSynthMasterLevel, +0.00},  // Family 01 / 001 Glass Keys  measured -12.01 LUFS, applied +0.01 dB
  {1, kParamOutLevel, +9.81}, {1, kParamSynthMasterLevel, -4.00},  // Family 01 / 002 Sparse Keys  measured -11.99 LUFS, applied -0.01 dB
  {2, kParamOutLevel, +1.45}, {2, kParamSynthMasterLevel, +0.00},  // Family 01 / 003 Soft Tines  measured -12.00 LUFS, applied +0.00 dB
  {3, kParamOutLevel, +3.80}, {3, kParamSynthMasterLevel, -0.00},  // Family 01 / 004 Bright Clav  measured -13.38 LUFS, applied +0.00 dB
  {4, kParamOutLevel, +3.79}, {4, kParamSynthMasterLevel, +0.00},  // Family 01 / 005 Drawbar Organ  measured -12.00 LUFS, applied -0.00 dB
  {5, kParamOutLevel, +8.43}, {5, kParamSynthMasterLevel, -4.00},  // Family 01 / 006 Glass Bells  measured -12.00 LUFS, applied -0.00 dB
  {6, kParamOutLevel, +8.96}, {6, kParamSynthMasterLevel, -0.00},  // Family 01 / 007 Crystal Pluck  measured -15.05 LUFS, applied -0.00 dB
  {7, kParamOutLevel, +4.36}, {7, kParamSynthMasterLevel, +0.00},  // Family 01 / 008 Key Stab  measured -16.79 LUFS, applied -0.00 dB
  {8, kParamOutLevel, -19.24}, {8, kParamSynthMasterLevel, -0.00},  // Family 01 / 009 Dry Bass  measured -13.71 LUFS, applied -0.00 dB
  {9, kParamOutLevel, -17.98}, {9, kParamSynthMasterLevel, +0.00},  // Family 01 / 010 Smooth Lead  measured -12.00 LUFS, applied -0.00 dB
  {10, kParamOutLevel, +5.67}, {10, kParamSynthMasterLevel, +0.00},  // Family 01 / 011 Warm Brass  measured -12.00 LUFS, applied +0.00 dB
  {11, kParamOutLevel, +8.88}, {11, kParamSynthMasterLevel, -3.00},  // Family 01 / 012 Silk Strings  measured -12.00 LUFS, applied +0.00 dB
  {12, kParamOutLevel, +9.86}, {12, kParamSynthMasterLevel, -3.00},  // Family 01 / 013 Soft Choir  measured -12.00 LUFS, applied -0.00 dB
  {13, kParamOutLevel, +5.96}, {13, kParamSynthMasterLevel, -3.00},  // Family 01 / 014 Bell Pad  measured -12.00 LUFS, applied +0.00 dB
  {14, kParamOutLevel, +10.81}, {14, kParamSynthMasterLevel, -4.00},  // Family 01 / 015 Slow Motion  measured -12.00 LUFS, applied +0.00 dB
  {15, kParamOutLevel, +11.60}, {15, kParamSynthMasterLevel, +0.00},  // Family 01 / 016 Shimmer Texture  measured -12.00 LUFS, applied +0.00 dB
  {16, kParamOutLevel, +4.13}, {16, kParamSynthMasterLevel, -6.00},  // Family 01 / 017 Low Drone  measured -12.00 LUFS, applied +0.00 dB
  {17, kParamOutLevel, +10.40}, {17, kParamSynthMasterLevel, +0.00},  // Family 01 / 018 Glass Arp  measured -16.14 LUFS, applied +0.00 dB
  {18, kParamOutLevel, +8.57}, {18, kParamSynthMasterLevel, +0.00},  // Family 01 / 019 Heavy Delay  measured -12.00 LUFS, applied +0.00 dB
  {19, kParamOutLevel, +7.84}, {19, kParamSynthMasterLevel, +0.00},  // Family 01 / 020 Room Kit  measured -14.86 LUFS, applied +0.00 dB
  {20, kParamOutLevel, +9.20}, {20, kParamSynthMasterLevel, -11.06},  // Family 01 / 021 Upright Bass  measured -12.00 LUFS, applied -0.00 dB
  {21, kParamOutLevel, +12.00}, {21, kParamSynthMasterLevel, -12.55},  // Family 01 / 022 Hammer Keys  measured -12.00 LUFS, applied -0.00 dB
  {22, kParamOutLevel, +12.00}, {22, kParamSynthMasterLevel, -6.29},  // Family 01 / 023 Swell Choir  measured -12.01 LUFS, applied +0.01 dB
  {23, kParamOutLevel, +12.00}, {23, kParamSynthMasterLevel, -11.69},  // Family 01 / 024 Bright Reed  measured -12.01 LUFS, applied +0.01 dB
  {24, kParamOutLevel, +12.00}, {24, kParamSynthMasterLevel, +2.07},  // Family 01 / 025 Snap Bass  measured -12.44 LUFS, applied +0.44 dB
  {25, kParamOutLevel, +10.54}, {25, kParamSynthMasterLevel, -10.00},  // Family 01 / 026 Short Bass  measured -12.00 LUFS, applied +0.00 dB
  {26, kParamOutLevel, +1.08}, {26, kParamSynthMasterLevel, +0.00},  // Family 02 / 001 Neon Keys  measured -12.00 LUFS, applied +0.00 dB
  {27, kParamOutLevel, +1.30}, {27, kParamSynthMasterLevel, -2.00},  // Family 02 / 002 Thin Piano  measured -12.00 LUFS, applied +0.00 dB
  {28, kParamOutLevel, +8.38}, {28, kParamSynthMasterLevel, -0.00},  // Family 02 / 003 Thin Clav  measured -13.09 LUFS, applied -0.00 dB
  {29, kParamOutLevel, +3.27}, {29, kParamSynthMasterLevel, +0.00},  // Family 02 / 004 Perc Organ  measured -11.99 LUFS, applied -0.01 dB
  {30, kParamOutLevel, +6.39}, {30, kParamSynthMasterLevel, -5.00},  // Family 02 / 005 Metal Bells  measured -12.00 LUFS, applied -0.00 dB
  {31, kParamOutLevel, +11.11}, {31, kParamSynthMasterLevel, +0.00},  // Family 02 / 006 Neon Pluck  measured -12.51 LUFS, applied +0.00 dB
  {32, kParamOutLevel, +10.15}, {32, kParamSynthMasterLevel, -3.00},  // Family 02 / 007 Sparse Pluck  measured -13.94 LUFS, applied +0.00 dB
  {33, kParamOutLevel, -17.47}, {33, kParamSynthMasterLevel, +0.00},  // Family 02 / 008 Thin Stab  measured -19.91 LUFS, applied -0.00 dB
  {34, kParamOutLevel, -12.93}, {34, kParamSynthMasterLevel, +0.00},  // Family 02 / 009 Hard Bass  measured -12.00 LUFS, applied -0.00 dB
  {35, kParamOutLevel, -20.94}, {35, kParamSynthMasterLevel, -2.00},  // Family 02 / 010 Sparse Bass  measured -12.00 LUFS, applied -0.00 dB
  {36, kParamOutLevel, -13.13}, {36, kParamSynthMasterLevel, +0.00},  // Family 02 / 011 Sync Lead  measured -12.00 LUFS, applied -0.00 dB
  {37, kParamOutLevel, +5.77}, {37, kParamSynthMasterLevel, +0.00},  // Family 02 / 012 Stab Brass  measured -12.00 LUFS, applied -0.00 dB
  {38, kParamOutLevel, +6.22}, {38, kParamSynthMasterLevel, +0.00},  // Family 02 / 013 Tight Strings  measured -12.01 LUFS, applied +0.01 dB
  {39, kParamOutLevel, +12.00}, {39, kParamSynthMasterLevel, -1.57},  // Family 02 / 014 Breath Choir  measured -12.25 LUFS, applied +0.25 dB
  {40, kParamOutLevel, +6.00}, {40, kParamSynthMasterLevel, +0.00},  // Family 02 / 015 Poly Pad  measured -12.00 LUFS, applied -0.00 dB
  {41, kParamOutLevel, +7.96}, {41, kParamSynthMasterLevel, +0.00},  // Family 02 / 016 Sweep Motion  measured -12.00 LUFS, applied +0.00 dB
  {42, kParamOutLevel, +0.52}, {42, kParamSynthMasterLevel, +0.00},  // Family 02 / 017 Skew Texture  measured -12.00 LUFS, applied +0.00 dB
  {43, kParamOutLevel, +3.49}, {43, kParamSynthMasterLevel, -6.00},  // Family 02 / 018 Pulse Drone  measured -12.00 LUFS, applied +0.00 dB
  {44, kParamOutLevel, +12.00}, {44, kParamSynthMasterLevel, +2.27},  // Family 02 / 019 Wide Arp  measured -14.48 LUFS, applied +0.00 dB
  {45, kParamOutLevel, +6.50}, {45, kParamSynthMasterLevel, +0.00},  // Family 02 / 020 Heavy Crush  measured -12.00 LUFS, applied +0.00 dB
  {46, kParamOutLevel, +9.54}, {46, kParamSynthMasterLevel, +0.00},  // Family 02 / 021 Punch Kit  measured -14.84 LUFS, applied +0.00 dB
  {47, kParamOutLevel, +3.89}, {47, kParamSynthMasterLevel, +0.00},  // Family 03 / 001 Piano Stack  measured -12.00 LUFS, applied -0.00 dB
  {48, kParamOutLevel, +3.11}, {48, kParamSynthMasterLevel, +0.00},  // Family 03 / 002 Soft EP  measured -12.00 LUFS, applied -0.00 dB
  {49, kParamOutLevel, +3.25}, {49, kParamSynthMasterLevel, +0.00},  // Family 03 / 003 Soul Piano  measured -12.01 LUFS, applied +0.01 dB
  {50, kParamOutLevel, +2.13}, {50, kParamSynthMasterLevel, -0.00},  // Family 03 / 004 Clav Stab  measured -17.10 LUFS, applied -0.00 dB
  {51, kParamOutLevel, +2.12}, {51, kParamSynthMasterLevel, +0.00},  // Family 03 / 005 Dry Clav  measured -16.14 LUFS, applied -0.00 dB
  {52, kParamOutLevel, +2.48}, {52, kParamSynthMasterLevel, +0.00},  // Family 03 / 006 Warm Organ  measured -12.00 LUFS, applied -0.00 dB
  {53, kParamOutLevel, +4.23}, {53, kParamSynthMasterLevel, +0.00},  // Family 03 / 007 Bottle Bells  measured -12.00 LUFS, applied +0.00 dB
  {54, kParamOutLevel, +5.03}, {54, kParamSynthMasterLevel, -0.00},  // Family 03 / 008 Muted Pluck  measured -17.50 LUFS, applied -0.00 dB
  {55, kParamOutLevel, +8.06}, {55, kParamSynthMasterLevel, -0.00},  // Family 03 / 009 Reed Stab  measured -15.18 LUFS, applied +0.00 dB
  {56, kParamOutLevel, -20.50}, {56, kParamSynthMasterLevel, +0.00},  // Family 03 / 010 Finger Bass  measured -12.00 LUFS, applied -0.00 dB
  {57, kParamOutLevel, -16.70}, {57, kParamSynthMasterLevel, +0.00},  // Family 03 / 011 Solo Lead  measured -12.00 LUFS, applied -0.00 dB
  {58, kParamOutLevel, +5.90}, {58, kParamSynthMasterLevel, +0.00},  // Family 03 / 012 Ensemble Brass  measured -12.00 LUFS, applied +0.00 dB
  {59, kParamOutLevel, +5.07}, {59, kParamSynthMasterLevel, -2.00},  // Family 03 / 013 Section Strings  measured -12.00 LUFS, applied +0.00 dB
  {60, kParamOutLevel, +5.37}, {60, kParamSynthMasterLevel, +0.00},  // Family 03 / 014 Ooh Choir  measured -12.00 LUFS, applied +0.00 dB
  {61, kParamOutLevel, +6.77}, {61, kParamSynthMasterLevel, -3.00},  // Family 03 / 015 Dream Pad  measured -12.00 LUFS, applied -0.00 dB
  {62, kParamOutLevel, +4.06}, {62, kParamSynthMasterLevel, +0.00},  // Family 03 / 016 Slow Sweep  measured -12.01 LUFS, applied +0.01 dB
  {63, kParamOutLevel, +12.00}, {63, kParamSynthMasterLevel, +2.01},  // Family 03 / 017 Hollow Texture  measured -12.37 LUFS, applied +0.37 dB
  {64, kParamOutLevel, +6.00}, {64, kParamSynthMasterLevel, -5.00},  // Family 03 / 018 Warm Drone  measured -12.00 LUFS, applied +0.00 dB
  {65, kParamOutLevel, +10.01}, {65, kParamSynthMasterLevel, -0.00},  // Family 03 / 019 Mallet Arp  measured -14.24 LUFS, applied +0.00 dB
  {66, kParamOutLevel, +11.35}, {66, kParamSynthMasterLevel, +0.00},  // Family 03 / 020 Slap Delay  measured -14.10 LUFS, applied -0.00 dB
  {67, kParamOutLevel, +7.28}, {67, kParamSynthMasterLevel, +0.00},  // Family 03 / 021 Dry Perc  measured -14.61 LUFS, applied +0.00 dB
  {68, kParamOutLevel, +7.21}, {68, kParamSynthMasterLevel, +0.00},  // Family 04 / 001 Vector Keys  measured -12.00 LUFS, applied +0.00 dB
  {69, kParamOutLevel, +5.78}, {69, kParamSynthMasterLevel, +0.00},  // Family 04 / 002 Phase Piano  measured -12.00 LUFS, applied +0.00 dB
  {70, kParamOutLevel, +10.41}, {70, kParamSynthMasterLevel, -0.00},  // Family 04 / 003 Wave Clav  measured -13.44 LUFS, applied +0.00 dB
  {71, kParamOutLevel, +3.32}, {71, kParamSynthMasterLevel, +0.00},  // Family 04 / 004 Drift Organ  measured -12.00 LUFS, applied -0.00 dB
  {72, kParamOutLevel, +9.19}, {72, kParamSynthMasterLevel, -4.00},  // Family 04 / 005 Chime Bells  measured -12.00 LUFS, applied +0.00 dB
  {73, kParamOutLevel, +10.47}, {73, kParamSynthMasterLevel, +0.00},  // Family 04 / 006 Harp Pluck  measured -12.01 LUFS, applied +0.01 dB
  {74, kParamOutLevel, +9.27}, {74, kParamSynthMasterLevel, +0.00},  // Family 04 / 007 Soft Stab  measured -13.29 LUFS, applied +0.00 dB
  {75, kParamOutLevel, -22.56}, {75, kParamSynthMasterLevel, -0.00},  // Family 04 / 008 Sub Bass  measured -12.00 LUFS, applied +0.00 dB
  {76, kParamOutLevel, -15.71}, {76, kParamSynthMasterLevel, +0.00},  // Family 04 / 009 Air Lead  measured -12.01 LUFS, applied +0.01 dB
  {77, kParamOutLevel, +6.55}, {77, kParamSynthMasterLevel, +0.00},  // Family 04 / 010 Swell Brass  measured -12.00 LUFS, applied -0.00 dB
  {78, kParamOutLevel, +8.66}, {78, kParamSynthMasterLevel, -3.00},  // Family 04 / 011 Bowed Strings  measured -12.00 LUFS, applied +0.00 dB
  {79, kParamOutLevel, +6.35}, {79, kParamSynthMasterLevel, +0.00},  // Family 04 / 012 Air Choir  measured -12.00 LUFS, applied +0.00 dB
  {80, kParamOutLevel, +8.99}, {80, kParamSynthMasterLevel, -4.00},  // Family 04 / 013 Cloud Pad  measured -12.00 LUFS, applied +0.00 dB
  {81, kParamOutLevel, +11.99}, {81, kParamSynthMasterLevel, -4.61},  // Family 04 / 014 Sparse Pad  measured -11.99 LUFS, applied -0.01 dB
  {82, kParamOutLevel, +6.40}, {82, kParamSynthMasterLevel, +0.00},  // Family 04 / 015 Motion Pad  measured -11.99 LUFS, applied -0.01 dB
  {83, kParamOutLevel, +7.74}, {83, kParamSynthMasterLevel, +0.00},  // Family 04 / 016 Tilt Texture  measured -12.00 LUFS, applied -0.00 dB
  {84, kParamOutLevel, +2.24}, {84, kParamSynthMasterLevel, +0.00},  // Family 04 / 017 Slow Drone  measured -12.00 LUFS, applied -0.00 dB
  {85, kParamOutLevel, +10.39}, {85, kParamSynthMasterLevel, -0.00},  // Family 04 / 018 Seq Arp  measured -16.61 LUFS, applied +0.00 dB
  {86, kParamOutLevel, +10.57}, {86, kParamSynthMasterLevel, -3.00},  // Family 04 / 019 Wash Delay  measured -12.00 LUFS, applied +0.00 dB
  {87, kParamOutLevel, +12.00}, {87, kParamSynthMasterLevel, +0.74},  // Family 04 / 020 Metal Kit  measured -12.37 LUFS, applied +0.37 dB
  {88, kParamOutLevel, +3.50}, {88, kParamSynthMasterLevel, +0.00},  // Family 05 / 001 Stage Keys  measured -12.00 LUFS, applied -0.00 dB
  {89, kParamOutLevel, +0.20}, {89, kParamSynthMasterLevel, +0.00},  // Family 05 / 002 Reed Piano  measured -12.00 LUFS, applied +0.00 dB
  {90, kParamOutLevel, +0.08}, {90, kParamSynthMasterLevel, +0.00},  // Family 05 / 003 Funk Clav  measured -17.02 LUFS, applied +0.00 dB
  {91, kParamOutLevel, +1.81}, {91, kParamSynthMasterLevel, +0.00},  // Family 05 / 004 Rock Organ  measured -12.00 LUFS, applied +0.00 dB
  {92, kParamOutLevel, +9.13}, {92, kParamSynthMasterLevel, -3.00},  // Family 05 / 005 Ping Bells  measured -12.00 LUFS, applied +0.00 dB
  {93, kParamOutLevel, +4.51}, {93, kParamSynthMasterLevel, +0.00},  // Family 05 / 006 Mute Pluck  measured -18.36 LUFS, applied +0.00 dB
  {94, kParamOutLevel, -1.12}, {94, kParamSynthMasterLevel, +0.00},  // Family 05 / 007 Short Stab  measured -19.30 LUFS, applied -0.00 dB
  {95, kParamOutLevel, -23.23}, {95, kParamSynthMasterLevel, +0.00},  // Family 05 / 008 Round Bass  measured -12.00 LUFS, applied -0.00 dB
  {96, kParamOutLevel, -16.30}, {96, kParamSynthMasterLevel, +0.00},  // Family 05 / 009 Saw Lead  measured -12.00 LUFS, applied +0.00 dB
  {97, kParamOutLevel, +7.02}, {97, kParamSynthMasterLevel, +0.00},  // Family 05 / 010 Section Brass  measured -12.00 LUFS, applied +0.00 dB
  {98, kParamOutLevel, +6.59}, {98, kParamSynthMasterLevel, -2.00},  // Family 05 / 011 Small Strings  measured -12.00 LUFS, applied -0.00 dB
  {99, kParamOutLevel, +3.53}, {99, kParamSynthMasterLevel, +0.00},  // Family 05 / 012 Close Choir  measured -12.00 LUFS, applied -0.00 dB
  {100, kParamOutLevel, +3.50}, {100, kParamSynthMasterLevel, +0.00},  // Family 05 / 013 Tape Pad  measured -12.00 LUFS, applied +0.00 dB
  {101, kParamOutLevel, -0.90}, {101, kParamSynthMasterLevel, +0.00},  // Family 05 / 014 Filter Motion  measured -12.00 LUFS, applied +0.00 dB
  {102, kParamOutLevel, +11.12}, {102, kParamSynthMasterLevel, +0.00},  // Family 05 / 015 Broken Texture  measured -12.00 LUFS, applied -0.00 dB
  {103, kParamOutLevel, -0.85}, {103, kParamSynthMasterLevel, -8.00},  // Family 05 / 016 Dark Drone  measured -12.00 LUFS, applied +0.00 dB
  {104, kParamOutLevel, +4.01}, {104, kParamSynthMasterLevel, -0.00},  // Family 05 / 017 Tight Arp  measured -19.78 LUFS, applied -0.00 dB
  {105, kParamOutLevel, +4.90}, {105, kParamSynthMasterLevel, +0.00},  // Family 05 / 018 Room Crush  measured -12.00 LUFS, applied +0.00 dB
  {106, kParamOutLevel, +4.93}, {106, kParamSynthMasterLevel, -0.00},  // Family 05 / 019 Dry Kit  measured -14.58 LUFS, applied -0.00 dB
  {107, kParamOutLevel, +7.79}, {107, kParamSynthMasterLevel, +0.00},  // Family 05 / 020 Hard Kit  measured -16.59 LUFS, applied +0.00 dB
  {108, kParamOutLevel, +6.81}, {108, kParamSynthMasterLevel, +0.00},  // Family 05 / 021 Tape Kit  measured -12.00 LUFS, applied -0.00 dB
  // --- END measured level trims
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
  {"Family 01 / 024 Bright Reed", 1, "Triton", "lead"},
  {"Family 01 / 025 Snap Bass", 1, "Triton", "bass"},
  {"Family 01 / 026 Short Bass", 1, "Triton", "bass"},
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
static_assert(kParamNoiseKey == 101 && kParamFilterKeyCurve == 110,
              "U17 appends 101..110");
static_assert(kParamOsc3Shape == 111 && kParamOsc3Source == 117 && kNumParams == 118,
              "the third oscillator appends 111..117");

Keith::Keith(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
#if IPLUG_DSP
  // Hand the sampler to the DSP before anything renders. The DSP is constructed as a member,
  // so this is the first point at which both objects exist.
  mDSP.SetSamplerEngine(&mSampler);
#endif

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

  // Third oscillator. Level and enable both default to silent/off: every existing preset was
  // written before osc 3 existed, so it has to add nothing to their sound. Shape, pitch and
  // PW match the other two so a patch that switches it on starts from the familiar place.
  GetParam(kParamOsc3Shape)->InitDouble("Synth Osc3 Shape", 0., 0., 100., 0.1, "%");
  GetParam(kParamOsc3Pitch)->InitDouble("Synth Osc3 Pitch", 0., -24., 24., 0.01, "st");
  GetParam(kParamOsc3Level)->InitDouble("Synth Osc3 Level", 0., 0., 100., 0.1, "%");
  GetParam(kParamOsc3PW)->InitDouble("Synth Osc3 PW", 50., 5., 95., 0.1, "%");
  GetParam(kParamOsc3Source)->InitDouble("Synth Osc3 Source", 0., 0., 1., 1., "");
  GetParam(kParamOsc3Morph)->InitDouble("Synth Osc3 Morph", 0., 0., 100., 0.1, "%");
  GetParam(kParamOsc3Enable)->InitBool("Synth Osc3 Enable", false);

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

  GetParam(kParamSynthMasterLevel)->InitDouble("Synth Master Level", 0., -60., 18., 0.01, "dB");

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
  GetParam(kParamDrumKitLevel)->InitDouble("Drum Kit Level", -60., -60., 6., 0.01, "dB");
  GetParam(kParamDrumKitLevel)->SetDisplayText(-60., "-- OFF");
  GetParam(kParamDrumKitLevel)->SetDisplayText(0., "0 dB");

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

  // Drum pad flashes. A pad lights when it sounds -- from MIDI or from a click -- and the
  // flash fades, so the pads are repainted while any of them is still lit. The hit counters
  // live on the audio thread; comparing them here is what tells the editor a pad just fired.
  // When nothing has fired for the length of the fade, the repaints stop and the UI idles.
  for (int i = 0; i < keith::drums::kNumSlots; ++i)
  {
    const uint64_t hits = mDSP.PadHitCount(i);
    if (hits != mLastPadHits[i])
    {
      mLastPadHits[i] = hits;
      mPadFlashFrames = kPadFlashFrames;
    }
  }
  if (mPadFlashFrames > 0)
  {
    pGraphics->SetAllControlsDirty();
    --mPadFlashFrames;
  }
}
#endif

#if IPLUG_DSP
void Keith::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // PAD MIDI is editor-only state rather than a parameter, so it is handed to the DSP each
  // block instead of being read from the parameter list.
  mDSP.SetPadMidi(mPadMidiEnabled.load());

  // The host transport, read once per block. The warp map needs the tempo, and tempo sync needs
  // the musical position. GetTimeInfo is cheap and must be called on the audio thread here --
  // the object it returns is only valid for the duration of this call, so nothing may be stored
  // except copies of the fields.
  {
    const ITimeInfo& timeInfo = GetTimeInfo();
    keith::sampler::Transport transport;
    transport.tempo = timeInfo.mTempo;
    transport.ppqPosition = timeInfo.mPPQPos;
    transport.running = timeInfo.mTransportIsRunning;
    transport.beatsPerBar = (timeInfo.mDenominator > 0)
                              ? static_cast<int>(4.0 * timeInfo.mNumerator / timeInfo.mDenominator)
                              : 4;
    mSampler.SetTransport(transport);
  }

  // A pad clicked in the editor is played at the start of this block.
  float padVelocity = 1.f;
  const int padSlot = TakePendingDrumPad(padVelocity);
  if (padSlot >= 0)
    mDSP.TriggerPad(padSlot, padVelocity);
  mDSP.ProcessBlock(outputs, 2, nFrames);
}

void Keith::OnReset()
{
  mDSP.Reset(GetSampleRate(), GetBlockSize());
  // The sampler needs the host rate so a file at 48 kHz plays at the right speed in a 44.1 kHz
  // host. It is set here rather than at construction because the rate is not known until now.
  mSampler.SetSampleRate(GetSampleRate());
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

  // Osc 3. The enable check is applied here as well as in the voice, so a disabled oscillator
  // reports zero level and the render loop can skip it without re-reading the flag.
  const bool osc3On = GetParam(kParamOsc3Enable)->Bool();
  p.osc3Enabled = osc3On;
  p.osc3Shape = static_cast<float>(GetParam(kParamOsc3Shape)->Value() / 100.);
  p.osc3Pitch = static_cast<float>(GetParam(kParamOsc3Pitch)->Value());
  p.osc3Level = osc3On ? static_cast<float>(GetParam(kParamOsc3Level)->Value() / 100.) : 0.f;
  p.osc3PW = static_cast<float>(GetParam(kParamOsc3PW)->Value() / 100.);
  p.osc3Source = GetParam(kParamOsc3Source)->Int();
  p.osc3Morph = static_cast<float>(GetParam(kParamOsc3Morph)->Value() / 100.);

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
