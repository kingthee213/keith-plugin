// Main Keith iPlug2 plugin implementation: factory presets, parameter setup, state I/O, and host callbacks.
#include "Keith.h"
#include "IPlug_include_in_plug_src.h"

#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <limits>

#if IPLUG_EDITOR
#include "IControls.h"
#include "editor/EditorialLayout.h"
#endif

namespace
{
constexpr uint32_t kDrumStateMagic = 0x4B445231u; // "KDR1"
constexpr uint32_t kDrumStateVersion = 1u;

constexpr uint32_t kSamplerStateMagic = 0x4B535031u; // "KSP1"
constexpr uint32_t kSamplerStateVersion = 1u;
// 16-bit PCM payload limit on save and load. Above this we store the file path only so the
// project stays small; slices and warp still round-trip.
constexpr uint32_t kSampleEmbedCeilingBytes = 8u * 1024u * 1024u;

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
// --- BEGIN the MAGS library
  // polysynth
  {109, kParamEngine, 0},   {109, kParamPolyMode, 0},   {109, kParamOsc1Shape, 24},
  {109, kParamOsc1Level, 88},   {109, kParamOsc2Level, 0},   {109, kParamSubLevel, 30},
  {109, kParamNoiseLevel, 0},   {109, kParamFilterType, 0},   {109, kParamFilterCutoff, 11000},
  {109, kParamFilterReso, 10},   {109, kParamFilterEnv, 32},   {109, kParamFiltAttack, 6},
  {109, kParamFiltDecay, 900},   {109, kParamFiltSustain, 38},   {109, kParamFiltRelease, 1400},
  {109, kParamAmpAttack, 6},   {109, kParamAmpDecay, 1200},   {109, kParamAmpSustain, 62},
  {109, kParamAmpRelease, 1400},   {109, kParamUnisonVoices, 2},   {109, kParamUnisonDetune, 17},
  {109, kParamStereoSpread, 62},   {109, kParamTapeSat, 12},   {109, kParamTapeMix, 28},
  {109, kParamFilterKeyTrack, 52},   {109, kParamIns1Algo, 2},   {109, kParamIns1A, 20},
  {109, kParamIns1B, 28},   {109, kParamIns1C, 36},   {109, kParamIns1Mix, 20},
  {109, kParamMstAlgo, 2},   {109, kParamMstA, 24},   {109, kParamMstB, 18},
  {109, kParamMstC, 30},   {109, kParamMstMix, 10},
  {110, kParamEngine, 0},   {110, kParamPolyMode, 0},   {110, kParamOsc1Shape, 18},
  {110, kParamOsc1Level, 88},   {110, kParamOsc2Level, 0},   {110, kParamSubLevel, 45},
  {110, kParamNoiseLevel, 0},   {110, kParamFilterType, 0},   {110, kParamFilterCutoff, 9800},
  {110, kParamFilterReso, 14},   {110, kParamFilterEnv, 32},   {110, kParamFiltAttack, 6},
  {110, kParamFiltDecay, 900},   {110, kParamFiltSustain, 38},   {110, kParamFiltRelease, 900},
  {110, kParamAmpAttack, 4},   {110, kParamAmpDecay, 1200},   {110, kParamAmpSustain, 62},
  {110, kParamAmpRelease, 900},   {110, kParamUnisonVoices, 2},   {110, kParamUnisonDetune, 17},
  {110, kParamStereoSpread, 42},   {110, kParamTapeSat, 15},   {110, kParamTapeMix, 28},
  {110, kParamFmAmount, 22},   {110, kParamVelToAmp, 56},   {110, kParamIns1Algo, 2},
  {110, kParamIns1A, 14},   {110, kParamIns1B, 38},   {110, kParamIns1C, 44},
  {110, kParamIns1Mix, 26},   {110, kParamMstAlgo, 2},   {110, kParamMstA, 24},
  {110, kParamMstB, 18},   {110, kParamMstC, 30},   {110, kParamMstMix, 10},
  {111, kParamEngine, 0},   {111, kParamPolyMode, 0},   {111, kParamOsc1Shape, 44},
  {111, kParamOsc1Level, 84},   {111, kParamOsc2Shape, 46},   {111, kParamOsc2Pitch, -12},
  {111, kParamOsc2Level, 64},   {111, kParamSubLevel, 20},   {111, kParamFilterType, 0},
  {111, kParamFilterCutoff, 7400},   {111, kParamFilterReso, 22},   {111, kParamFilterEnv, 58},
  {111, kParamFiltAttack, 4},   {111, kParamFiltDecay, 208},   {111, kParamFiltSustain, 4.8},
  {111, kParamFiltRelease, 210},   {111, kParamAmpAttack, 3},   {111, kParamAmpDecay, 260},
  {111, kParamAmpSustain, 8},   {111, kParamAmpRelease, 300},   {111, kParamUnisonVoices, 1},
  {111, kParamUnisonDetune, 10},   {111, kParamStereoSpread, 30},   {111, kParamStereoDelay, 30},
  {111, kParamTapeSat, 20},   {111, kParamTapeMix, 32},   {111, kParamOsc1PW, 62},
  {111, kParamIns1Algo, 4},   {111, kParamIns1A, 22},   {111, kParamIns1B, 32},
  {111, kParamIns1C, 14},   {111, kParamIns1Mix, 14},   {111, kParamMstAlgo, 2},
  {111, kParamMstA, 24},   {111, kParamMstB, 18},   {111, kParamMstC, 30},
  {111, kParamMstMix, 10},
  {112, kParamEngine, 0},   {112, kParamPolyMode, 0},   {112, kParamOsc1Shape, 48},
  {112, kParamOsc1Level, 88},   {112, kParamOsc2Level, 0},   {112, kParamSubLevel, 45},
  {112, kParamNoiseLevel, 0},   {112, kParamFilterType, 0},   {112, kParamFilterCutoff, 8600},
  {112, kParamFilterReso, 18},   {112, kParamFilterEnv, 52},   {112, kParamFiltAttack, 6},
  {112, kParamFiltDecay, 900},   {112, kParamFiltSustain, 38},   {112, kParamFiltRelease, 420},
  {112, kParamAmpAttack, 2},   {112, kParamAmpDecay, 1200},   {112, kParamAmpSustain, 62},
  {112, kParamAmpRelease, 420},   {112, kParamUnisonVoices, 2},   {112, kParamUnisonDetune, 17},
  {112, kParamStereoSpread, 42},   {112, kParamTapeSat, 22},   {112, kParamTapeMix, 28},
  {112, kParamOsc1PW, 64},   {112, kParamIns1Algo, 2},   {112, kParamIns1A, 20},
  {112, kParamIns1B, 28},   {112, kParamIns1C, 36},   {112, kParamIns1Mix, 20},
  {112, kParamMstAlgo, 2},   {112, kParamMstA, 24},   {112, kParamMstB, 18},
  {112, kParamMstC, 30},   {112, kParamMstMix, 10},
  {113, kParamEngine, 0},   {113, kParamPolyMode, 0},   {113, kParamOsc1Shape, 30},
  {113, kParamOsc1Level, 58},   {113, kParamOsc2Level, 0},   {113, kParamOsc3Enable, 1},
  {113, kParamOsc3Source, 1},   {113, kParamOsc3Morph, 2.4},   {113, kParamOsc3Level, 86},
  {113, kParamOsc3Pitch, 12},   {113, kParamSubLevel, 26},   {113, kParamFilterType, 0},
  {113, kParamFilterCutoff, 10200},   {113, kParamFilterReso, 12},   {113, kParamFilterEnv, 28},
  {113, kParamFiltAttack, 10},   {113, kParamFiltDecay, 1400},   {113, kParamFiltSustain, 44},
  {113, kParamFiltRelease, 1800},   {113, kParamAmpAttack, 10},   {113, kParamAmpDecay, 1600},
  {113, kParamAmpSustain, 70},   {113, kParamAmpRelease, 1800},   {113, kParamUnisonVoices, 2},
  {113, kParamUnisonDetune, 12},   {113, kParamStereoSpread, 68},   {113, kParamTapeSat, 18},
  {113, kParamTapeMix, 30},   {113, kParamIns1Algo, 2},   {113, kParamIns1A, 14},
  {113, kParamIns1B, 38},   {113, kParamIns1C, 44},   {113, kParamIns1Mix, 26},
  {113, kParamMstAlgo, 2},   {113, kParamMstA, 24},   {113, kParamMstB, 18},
  {113, kParamMstC, 30},   {113, kParamMstMix, 10},
  {114, kParamEngine, 0},   {114, kParamPolyMode, 0},   {114, kParamOsc1Shape, 20},
  {114, kParamOsc1Level, 72},   {114, kParamOsc2Shape, 22},   {114, kParamOsc2Pitch, 24},
  {114, kParamOsc2Level, 48},   {114, kParamFmAmount, 34},   {114, kParamSubLevel, 18},
  {114, kParamNoiseLevel, 4},   {114, kParamFilterType, 0},   {114, kParamFilterCutoff, 8200},
  {114, kParamFilterReso, 16},   {114, kParamFilterEnv, 24},   {114, kParamFiltAttack, 3},
  {114, kParamFiltDecay, 1100},   {114, kParamFiltSustain, 11},   {114, kParamFiltRelease, 1200},
  {114, kParamAmpAttack, 2},   {114, kParamAmpDecay, 1100},   {114, kParamAmpSustain, 22},
  {114, kParamAmpRelease, 1200},   {114, kParamVelToAmp, 62},   {114, kParamVelToFilter, 30},
  {114, kParamStereoSpread, 44},   {114, kParamStereoDelay, 22},   {114, kParamTapeSat, 14},
  {114, kParamTapeMix, 26},   {114, kParamIns1Algo, 4},   {114, kParamIns1A, 22},
  {114, kParamIns1B, 32},   {114, kParamIns1C, 14},   {114, kParamIns1Mix, 14},
  {114, kParamMstAlgo, 2},   {114, kParamMstA, 24},   {114, kParamMstB, 18},
  {114, kParamMstC, 30},   {114, kParamMstMix, 10},
  {115, kParamEngine, 0},   {115, kParamPolyMode, 0},   {115, kParamOsc1Shape, 12},
  {115, kParamOsc1Level, 88},   {115, kParamOsc2Level, 0},   {115, kParamSubLevel, 22},
  {115, kParamNoiseLevel, 0},   {115, kParamFilterType, 0},   {115, kParamFilterCutoff, 12500},
  {115, kParamFilterReso, 8},   {115, kParamFilterEnv, 32},   {115, kParamFiltAttack, 6},
  {115, kParamFiltDecay, 900},   {115, kParamFiltSustain, 38},   {115, kParamFiltRelease, 2200},
  {115, kParamAmpAttack, 8},   {115, kParamAmpDecay, 1200},   {115, kParamAmpSustain, 62},
  {115, kParamAmpRelease, 2200},   {115, kParamUnisonVoices, 2},   {115, kParamUnisonDetune, 17},
  {115, kParamStereoSpread, 42},   {115, kParamTapeSat, 10},   {115, kParamTapeMix, 28},
  {115, kParamIns1Algo, 2},   {115, kParamIns1A, 20},   {115, kParamIns1B, 28},
  {115, kParamIns1C, 36},   {115, kParamIns1Mix, 20},   {115, kParamMstAlgo, 2},
  {115, kParamMstA, 24},   {115, kParamMstB, 18},   {115, kParamMstC, 30},
  {115, kParamMstMix, 10},
  {116, kParamEngine, 0},   {116, kParamPolyMode, 0},   {116, kParamOsc1Shape, 44},
  {116, kParamOsc1Level, 84},   {116, kParamOsc2Shape, 46},   {116, kParamOsc2Pitch, 7},
  {116, kParamOsc2Level, 64},   {116, kParamSubLevel, 20},   {116, kParamFilterType, 0},
  {116, kParamFilterCutoff, 6200},   {116, kParamFilterReso, 30},   {116, kParamFilterEnv, 64},
  {116, kParamFiltAttack, 4},   {116, kParamFiltDecay, 176},   {116, kParamFiltSustain, 3.5999999999999996},
  {116, kParamFiltRelease, 168},   {116, kParamAmpAttack, 2},   {116, kParamAmpDecay, 220},
  {116, kParamAmpSustain, 6},   {116, kParamAmpRelease, 240},   {116, kParamUnisonVoices, 1},
  {116, kParamUnisonDetune, 10},   {116, kParamStereoSpread, 30},   {116, kParamStereoDelay, 18},
  {116, kParamTapeSat, 20},   {116, kParamTapeMix, 32},   {116, kParamFmAmount, 14},
  {116, kParamIns1Algo, 2},   {116, kParamIns1A, 14},   {116, kParamIns1B, 38},
  {116, kParamIns1C, 44},   {116, kParamIns1Mix, 26},   {116, kParamMstAlgo, 2},
  {116, kParamMstA, 24},   {116, kParamMstB, 18},   {116, kParamMstC, 30},
  {116, kParamMstMix, 10},
  {117, kParamEngine, 0},   {117, kParamPolyMode, 0},   {117, kParamOsc1Shape, 16},
  {117, kParamOsc1Level, 88},   {117, kParamOsc2Level, 0},   {117, kParamSubLevel, 45},
  {117, kParamNoiseLevel, 0},   {117, kParamFilterType, 0},   {117, kParamFilterCutoff, 7000},
  {117, kParamFilterReso, 12},   {117, kParamFilterEnv, 32},   {117, kParamFiltAttack, 6},
  {117, kParamFiltDecay, 900},   {117, kParamFiltSustain, 38},   {117, kParamFiltRelease, 1600},
  {117, kParamAmpAttack, 6},   {117, kParamAmpDecay, 1200},   {117, kParamAmpSustain, 62},
  {117, kParamAmpRelease, 1600},   {117, kParamUnisonVoices, 2},   {117, kParamUnisonDetune, 17},
  {117, kParamStereoSpread, 42},   {117, kParamTapeSat, 15},   {117, kParamTapeMix, 28},
  {117, kParamFmAmount, 28},   {117, kParamVelToAmp, 64},   {117, kParamIns1Algo, 4},
  {117, kParamIns1A, 22},   {117, kParamIns1B, 32},   {117, kParamIns1C, 14},
  {117, kParamIns1Mix, 14},   {117, kParamMstAlgo, 2},   {117, kParamMstA, 24},
  {117, kParamMstB, 18},   {117, kParamMstC, 30},   {117, kParamMstMix, 10},
  {118, kParamEngine, 0},   {118, kParamPolyMode, 0},   {118, kParamOsc1Shape, 42},
  {118, kParamOsc1Level, 82},   {118, kParamOsc2Shape, 40},   {118, kParamOsc2Pitch, 0},
  {118, kParamOsc2Level, 76},   {118, kParamSubLevel, 32},   {118, kParamFilterType, 0},
  {118, kParamFilterCutoff, 6600},   {118, kParamFilterReso, 20},   {118, kParamFilterEnv, 40},
  {118, kParamFiltAttack, 8},   {118, kParamFiltDecay, 1100},   {118, kParamFiltSustain, 34},
  {118, kParamFiltRelease, 2400},   {118, kParamAmpAttack, 320},   {118, kParamAmpDecay, 1400},
  {118, kParamAmpSustain, 66},   {118, kParamAmpRelease, 2400},   {118, kParamUnisonVoices, 4},
  {118, kParamUnisonDetune, 30},   {118, kParamStereoSpread, 74},   {118, kParamStereoDelay, 0},
  {118, kParamTapeSat, 12},   {118, kParamTapeMix, 24},   {118, kParamLfoRate, 0.42},
  {118, kParamLfoAmount, 22},   {118, kParamLfoDest, 2},   {118, kParamIns1Algo, 2},
  {118, kParamIns1A, 20},   {118, kParamIns1B, 28},   {118, kParamIns1C, 36},
  {118, kParamIns1Mix, 20},   {118, kParamMstAlgo, 2},   {118, kParamMstA, 24},
  {118, kParamMstB, 18},   {118, kParamMstC, 30},   {118, kParamMstMix, 10},
  {119, kParamEngine, 0},   {119, kParamPolyMode, 0},   {119, kParamOsc1Shape, 42},
  {119, kParamOsc1Level, 82},   {119, kParamOsc2Shape, 40},   {119, kParamOsc2Pitch, 0},
  {119, kParamOsc2Level, 76},   {119, kParamSubLevel, 32},   {119, kParamFilterType, 0},
  {119, kParamFilterCutoff, 7800},   {119, kParamFilterReso, 26},   {119, kParamFilterEnv, 40},
  {119, kParamFiltAttack, 8},   {119, kParamFiltDecay, 1100},   {119, kParamFiltSustain, 34},
  {119, kParamFiltRelease, 900},   {119, kParamAmpAttack, 6},   {119, kParamAmpDecay, 1400},
  {119, kParamAmpSustain, 66},   {119, kParamAmpRelease, 900},   {119, kParamUnisonVoices, 4},
  {119, kParamUnisonDetune, 26},   {119, kParamStereoSpread, 58},   {119, kParamStereoDelay, 0},
  {119, kParamTapeSat, 12},   {119, kParamTapeMix, 24},   {119, kParamIns1Algo, 2},
  {119, kParamIns1A, 14},   {119, kParamIns1B, 38},   {119, kParamIns1C, 44},
  {119, kParamIns1Mix, 26},   {119, kParamMstAlgo, 2},   {119, kParamMstA, 24},
  {119, kParamMstB, 18},   {119, kParamMstC, 30},   {119, kParamMstMix, 10},
  {120, kParamEngine, 0},   {120, kParamPolyMode, 0},   {120, kParamOsc1Shape, 42},
  {120, kParamOsc1Level, 82},   {120, kParamOsc2Shape, 40},   {120, kParamOsc2Pitch, 0},
  {120, kParamOsc2Level, 76},   {120, kParamSubLevel, 32},   {120, kParamFilterType, 0},
  {120, kParamFilterCutoff, 3200},   {120, kParamFilterReso, 44},   {120, kParamFilterEnv, 72},
  {120, kParamFiltAttack, 8},   {120, kParamFiltDecay, 1500},   {120, kParamFiltSustain, 34},
  {120, kParamFiltRelease, 700},   {120, kParamAmpAttack, 4},   {120, kParamAmpDecay, 1400},
  {120, kParamAmpSustain, 66},   {120, kParamAmpRelease, 700},   {120, kParamUnisonVoices, 4},
  {120, kParamUnisonDetune, 22},   {120, kParamStereoSpread, 58},   {120, kParamStereoDelay, 0},
  {120, kParamTapeSat, 12},   {120, kParamTapeMix, 24},   {120, kParamLfoRate, 0.9},
  {120, kParamLfoDest, 1},   {120, kParamLfoAmount, 26},   {120, kParamIns1Algo, 4},
  {120, kParamIns1A, 22},   {120, kParamIns1B, 32},   {120, kParamIns1C, 14},
  {120, kParamIns1Mix, 14},   {120, kParamMstAlgo, 2},   {120, kParamMstA, 24},
  {120, kParamMstB, 18},   {120, kParamMstC, 30},   {120, kParamMstMix, 10},
  {121, kParamEngine, 0},   {121, kParamPolyMode, 0},   {121, kParamOsc1Shape, 44},
  {121, kParamOsc1Level, 84},   {121, kParamOsc2Shape, 46},   {121, kParamOsc2Pitch, -12},
  {121, kParamOsc2Level, 64},   {121, kParamSubLevel, 20},   {121, kParamFilterType, 0},
  {121, kParamFilterCutoff, 5400},   {121, kParamFilterReso, 38},   {121, kParamFilterEnv, 48},
  {121, kParamFiltAttack, 4},   {121, kParamFiltDecay, 240},   {121, kParamFiltSustain, 6},
  {121, kParamFiltRelease, 266},   {121, kParamAmpAttack, 2},   {121, kParamAmpDecay, 300},
  {121, kParamAmpSustain, 10},   {121, kParamAmpRelease, 380},   {121, kParamUnisonVoices, 1},
  {121, kParamUnisonDetune, 10},   {121, kParamStereoSpread, 30},   {121, kParamStereoDelay, 18},
  {121, kParamTapeSat, 36},   {121, kParamTapeMix, 32},   {121, kParamFilterDrive, 24},
  {121, kParamFmAmount, 10},   {121, kParamIns1Algo, 2},   {121, kParamIns1A, 20},
  {121, kParamIns1B, 28},   {121, kParamIns1C, 36},   {121, kParamIns1Mix, 20},
  {121, kParamMstAlgo, 2},   {121, kParamMstA, 24},   {121, kParamMstB, 18},
  {121, kParamMstC, 30},   {121, kParamMstMix, 10},
  {122, kParamEngine, 0},   {122, kParamPolyMode, 0},   {122, kParamOsc1Shape, 40},
  {122, kParamOsc1Level, 88},   {122, kParamOsc2Level, 0},   {122, kParamSubLevel, 45},
  {122, kParamNoiseLevel, 0},   {122, kParamFilterType, 0},   {122, kParamFilterCutoff, 8800},
  {122, kParamFilterReso, 16},   {122, kParamFilterEnv, 32},   {122, kParamFiltAttack, 6},
  {122, kParamFiltDecay, 900},   {122, kParamFiltSustain, 38},   {122, kParamFiltRelease, 1400},
  {122, kParamAmpAttack, 8},   {122, kParamAmpDecay, 1200},   {122, kParamAmpSustain, 62},
  {122, kParamAmpRelease, 1400},   {122, kParamUnisonVoices, 2},   {122, kParamUnisonDetune, 17},
  {122, kParamStereoSpread, 42},   {122, kParamTapeSat, 18},   {122, kParamTapeMix, 28},
  {122, kParamIns1Algo, 2},   {122, kParamIns1A, 14},   {122, kParamIns1B, 38},
  {122, kParamIns1C, 44},   {122, kParamIns1Mix, 26},   {122, kParamMstAlgo, 2},
  {122, kParamMstA, 24},   {122, kParamMstB, 18},   {122, kParamMstC, 30},
  {122, kParamMstMix, 10},
  {123, kParamEngine, 0},   {123, kParamPolyMode, 0},   {123, kParamOsc1Shape, 36},
  {123, kParamOsc1Level, 90},   {123, kParamOsc2Shape, 34},   {123, kParamOsc2Pitch, -12},
  {123, kParamOsc2Level, 58},   {123, kParamSubLevel, 38},   {123, kParamNoiseLevel, 6},
  {123, kParamFilterType, 0},   {123, kParamFilterCutoff, 1900},   {123, kParamFilterReso, 40},
  {123, kParamFilterDrive, 32},   {123, kParamFilterEnv, 44},   {123, kParamFiltAttack, 8},
  {123, kParamFiltDecay, 500},   {123, kParamFiltSustain, 21},   {123, kParamFiltRelease, 400},
  {123, kParamAmpAttack, 3},   {123, kParamAmpDecay, 500},   {123, kParamAmpSustain, 30},
  {123, kParamAmpRelease, 400},   {123, kParamUnisonVoices, 2},   {123, kParamUnisonDetune, 14},
  {123, kParamStereoSpread, 34},   {123, kParamTapeSat, 30},   {123, kParamTapeMix, 36},
  {123, kParamIns1Algo, 4},   {123, kParamIns1A, 22},   {123, kParamIns1B, 32},
  {123, kParamIns1C, 14},   {123, kParamIns1Mix, 14},   {123, kParamMstAlgo, 2},
  {123, kParamMstA, 24},   {123, kParamMstB, 18},   {123, kParamMstC, 30},
  {123, kParamMstMix, 10},
  {124, kParamEngine, 0},   {124, kParamPolyMode, 0},   {124, kParamOsc1Shape, 42},
  {124, kParamOsc1Level, 82},   {124, kParamOsc2Shape, 40},   {124, kParamOsc2Pitch, 0},
  {124, kParamOsc2Level, 76},   {124, kParamSubLevel, 32},   {124, kParamFilterType, 0},
  {124, kParamFilterCutoff, 6400},   {124, kParamFilterReso, 34},   {124, kParamFilterEnv, 40},
  {124, kParamFiltAttack, 8},   {124, kParamFiltDecay, 1100},   {124, kParamFiltSustain, 34},
  {124, kParamFiltRelease, 1200},   {124, kParamAmpAttack, 12},   {124, kParamAmpDecay, 1400},
  {124, kParamAmpSustain, 66},   {124, kParamAmpRelease, 1200},   {124, kParamUnisonVoices, 4},
  {124, kParamUnisonDetune, 34},   {124, kParamStereoSpread, 70},   {124, kParamStereoDelay, 0},
  {124, kParamTapeSat, 12},   {124, kParamTapeMix, 24},   {124, kParamIns1Algo, 2},
  {124, kParamIns1A, 20},   {124, kParamIns1B, 28},   {124, kParamIns1C, 36},
  {124, kParamIns1Mix, 20},   {124, kParamMstAlgo, 2},   {124, kParamMstA, 24},
  {124, kParamMstB, 18},   {124, kParamMstC, 30},   {124, kParamMstMix, 10},
  {125, kParamEngine, 0},   {125, kParamPolyMode, 0},   {125, kParamOsc1Shape, 30},
  {125, kParamOsc1Level, 58},   {125, kParamOsc2Level, 0},   {125, kParamOsc3Enable, 1},
  {125, kParamOsc3Source, 1},   {125, kParamOsc3Morph, 4.6},   {125, kParamOsc3Level, 86},
  {125, kParamOsc3Pitch, 0},   {125, kParamSubLevel, 26},   {125, kParamFilterType, 0},
  {125, kParamFilterCutoff, 7200},   {125, kParamFilterReso, 18},   {125, kParamFilterEnv, 28},
  {125, kParamFiltAttack, 10},   {125, kParamFiltDecay, 1400},   {125, kParamFiltSustain, 44},
  {125, kParamFiltRelease, 1800},   {125, kParamAmpAttack, 60},   {125, kParamAmpDecay, 1600},
  {125, kParamAmpSustain, 70},   {125, kParamAmpRelease, 1800},   {125, kParamUnisonVoices, 4},
  {125, kParamUnisonDetune, 20},   {125, kParamStereoSpread, 52},   {125, kParamTapeSat, 18},
  {125, kParamTapeMix, 30},   {125, kParamIns1Algo, 2},   {125, kParamIns1A, 14},
  {125, kParamIns1B, 38},   {125, kParamIns1C, 44},   {125, kParamIns1Mix, 26},
  {125, kParamMstAlgo, 2},   {125, kParamMstA, 24},   {125, kParamMstB, 18},
  {125, kParamMstC, 30},   {125, kParamMstMix, 10},
  {126, kParamEngine, 0},   {126, kParamPolyMode, 0},   {126, kParamOsc1Shape, 44},
  {126, kParamOsc1Level, 84},   {126, kParamOsc2Shape, 46},   {126, kParamOsc2Pitch, -12},
  {126, kParamOsc2Level, 64},   {126, kParamSubLevel, 20},   {126, kParamFilterType, 0},
  {126, kParamFilterCutoff, 9200},   {126, kParamFilterReso, 24},   {126, kParamFilterEnv, 54},
  {126, kParamFiltAttack, 4},   {126, kParamFiltDecay, 272},   {126, kParamFiltSustain, 7.199999999999999},
  {126, kParamFiltRelease, 294},   {126, kParamAmpAttack, 2},   {126, kParamAmpDecay, 340},
  {126, kParamAmpSustain, 12},   {126, kParamAmpRelease, 420},   {126, kParamUnisonVoices, 1},
  {126, kParamUnisonDetune, 10},   {126, kParamStereoSpread, 30},   {126, kParamStereoDelay, 18},
  {126, kParamTapeSat, 28},   {126, kParamTapeMix, 32},   {126, kParamOsc1PW, 55},
  {126, kParamIns1Algo, 4},   {126, kParamIns1A, 22},   {126, kParamIns1B, 32},
  {126, kParamIns1C, 14},   {126, kParamIns1Mix, 14},   {126, kParamMstAlgo, 2},
  {126, kParamMstA, 24},   {126, kParamMstB, 18},   {126, kParamMstC, 30},
  {126, kParamMstMix, 10},
  {127, kParamEngine, 0},   {127, kParamPolyMode, 0},   {127, kParamOsc1Shape, 20},
  {127, kParamOsc1Level, 72},   {127, kParamOsc2Shape, 22},   {127, kParamOsc2Pitch, 24},
  {127, kParamOsc2Level, 48},   {127, kParamFmAmount, 58},   {127, kParamSubLevel, 18},
  {127, kParamNoiseLevel, 4},   {127, kParamFilterType, 0},   {127, kParamFilterCutoff, 9600},
  {127, kParamFilterReso, 20},   {127, kParamFilterEnv, 24},   {127, kParamFiltAttack, 3},
  {127, kParamFiltDecay, 380},   {127, kParamFiltSustain, 2},   {127, kParamFiltRelease, 340},
  {127, kParamAmpAttack, 1},   {127, kParamAmpDecay, 380},   {127, kParamAmpSustain, 4},
  {127, kParamAmpRelease, 340},   {127, kParamVelToAmp, 70},   {127, kParamVelToFilter, 30},
  {127, kParamStereoSpread, 44},   {127, kParamStereoDelay, 0},   {127, kParamTapeSat, 14},
  {127, kParamTapeMix, 26},   {127, kParamIns1Algo, 2},   {127, kParamIns1A, 20},
  {127, kParamIns1B, 28},   {127, kParamIns1C, 36},   {127, kParamIns1Mix, 20},
  {127, kParamMstAlgo, 2},   {127, kParamMstA, 24},   {127, kParamMstB, 18},
  {127, kParamMstC, 30},   {127, kParamMstMix, 10},
  {128, kParamEngine, 0},   {128, kParamPolyMode, 0},   {128, kParamOsc1Shape, 20},
  {128, kParamOsc1Level, 72},   {128, kParamOsc2Shape, 22},   {128, kParamOsc2Pitch, 24},
  {128, kParamOsc2Level, 48},   {128, kParamFmAmount, 72},   {128, kParamSubLevel, 18},
  {128, kParamNoiseLevel, 4},   {128, kParamFilterType, 0},   {128, kParamFilterCutoff, 11400},
  {128, kParamFilterReso, 14},   {128, kParamFilterEnv, 24},   {128, kParamFiltAttack, 3},
  {128, kParamFiltDecay, 1800},   {128, kParamFiltSustain, 6},   {128, kParamFiltRelease, 2200},
  {128, kParamAmpAttack, 2},   {128, kParamAmpDecay, 1800},   {128, kParamAmpSustain, 12},
  {128, kParamAmpRelease, 2200},   {128, kParamVelToAmp, 48},   {128, kParamVelToFilter, 30},
  {128, kParamStereoSpread, 44},   {128, kParamStereoDelay, 34},   {128, kParamTapeSat, 14},
  {128, kParamTapeMix, 26},   {128, kParamIns1Algo, 2},   {128, kParamIns1A, 14},
  {128, kParamIns1B, 38},   {128, kParamIns1C, 44},   {128, kParamIns1Mix, 26},
  {128, kParamMstAlgo, 2},   {128, kParamMstA, 24},   {128, kParamMstB, 18},
  {128, kParamMstC, 30},   {128, kParamMstMix, 10},
  {129, kParamEngine, 0},   {129, kParamPolyMode, 0},   {129, kParamOsc1Shape, 52},
  {129, kParamOsc1Level, 88},   {129, kParamOsc2Level, 0},   {129, kParamSubLevel, 45},
  {129, kParamNoiseLevel, 0},   {129, kParamFilterType, 0},   {129, kParamFilterCutoff, 8400},
  {129, kParamFilterReso, 26},   {129, kParamFilterEnv, 62},   {129, kParamFiltAttack, 6},
  {129, kParamFiltDecay, 900},   {129, kParamFiltSustain, 38},   {129, kParamFiltRelease, 300},
  {129, kParamAmpAttack, 1},   {129, kParamAmpDecay, 1200},   {129, kParamAmpSustain, 62},
  {129, kParamAmpRelease, 300},   {129, kParamUnisonVoices, 2},   {129, kParamUnisonDetune, 17},
  {129, kParamStereoSpread, 42},   {129, kParamTapeSat, 15},   {129, kParamTapeMix, 28},
  {129, kParamOsc1PW, 70},   {129, kParamIns1Algo, 4},   {129, kParamIns1A, 22},
  {129, kParamIns1B, 32},   {129, kParamIns1C, 14},   {129, kParamIns1Mix, 14},
  {129, kParamMstAlgo, 2},   {129, kParamMstA, 24},   {129, kParamMstB, 18},
  {129, kParamMstC, 30},   {129, kParamMstMix, 10},
  {130, kParamEngine, 0},   {130, kParamPolyMode, 0},   {130, kParamOsc1Shape, 44},
  {130, kParamOsc1Level, 84},   {130, kParamOsc2Shape, 46},   {130, kParamOsc2Pitch, -12},
  {130, kParamOsc2Level, 64},   {130, kParamSubLevel, 20},   {130, kParamFilterType, 0},
  {130, kParamFilterCutoff, 5800},   {130, kParamFilterReso, 32},   {130, kParamFilterEnv, 70},
  {130, kParamFiltAttack, 4},   {130, kParamFiltDecay, 160},   {130, kParamFiltSustain, 2.4},
  {130, kParamFiltRelease, 154},   {130, kParamAmpAttack, 1},   {130, kParamAmpDecay, 200},
  {130, kParamAmpSustain, 4},   {130, kParamAmpRelease, 220},   {130, kParamUnisonVoices, 1},
  {130, kParamUnisonDetune, 10},   {130, kParamStereoSpread, 30},   {130, kParamStereoDelay, 0},
  {130, kParamTapeSat, 20},   {130, kParamTapeMix, 18},   {130, kParamIns1Algo, 2},
  {130, kParamIns1A, 20},   {130, kParamIns1B, 28},   {130, kParamIns1C, 36},
  {130, kParamIns1Mix, 20},   {130, kParamMstAlgo, 2},   {130, kParamMstA, 24},
  {130, kParamMstB, 18},   {130, kParamMstC, 30},   {130, kParamMstMix, 10},
  {131, kParamEngine, 0},   {131, kParamPolyMode, 0},   {131, kParamOsc1Shape, 30},
  {131, kParamOsc1Level, 58},   {131, kParamOsc2Level, 0},   {131, kParamOsc3Enable, 1},
  {131, kParamOsc3Source, 1},   {131, kParamOsc3Morph, 5.8},   {131, kParamOsc3Level, 86},
  {131, kParamOsc3Pitch, 19},   {131, kParamSubLevel, 26},   {131, kParamFilterType, 0},
  {131, kParamFilterCutoff, 9000},   {131, kParamFilterReso, 16},   {131, kParamFilterEnv, 28},
  {131, kParamFiltAttack, 10},   {131, kParamFiltDecay, 1400},   {131, kParamFiltSustain, 44},
  {131, kParamFiltRelease, 1600},   {131, kParamAmpAttack, 2},   {131, kParamAmpDecay, 1600},
  {131, kParamAmpSustain, 70},   {131, kParamAmpRelease, 1600},   {131, kParamUnisonVoices, 2},
  {131, kParamUnisonDetune, 12},   {131, kParamStereoSpread, 52},   {131, kParamTapeSat, 18},
  {131, kParamTapeMix, 30},   {131, kParamVelToAmp, 64},   {131, kParamIns1Algo, 2},
  {131, kParamIns1A, 14},   {131, kParamIns1B, 38},   {131, kParamIns1C, 44},
  {131, kParamIns1Mix, 26},   {131, kParamMstAlgo, 2},   {131, kParamMstA, 24},
  {131, kParamMstB, 18},   {131, kParamMstC, 30},   {131, kParamMstMix, 10},
  {132, kParamEngine, 0},   {132, kParamPolyMode, 0},   {132, kParamOsc1Shape, 20},
  {132, kParamOsc1Level, 72},   {132, kParamOsc2Shape, 22},   {132, kParamOsc2Pitch, 24},
  {132, kParamOsc2Level, 48},   {132, kParamFmAmount, 44},   {132, kParamSubLevel, 40},
  {132, kParamNoiseLevel, 4},   {132, kParamFilterType, 0},   {132, kParamFilterCutoff, 8000},
  {132, kParamFilterReso, 12},   {132, kParamFilterEnv, 24},   {132, kParamFiltAttack, 3},
  {132, kParamFiltDecay, 600},   {132, kParamFiltSustain, 0},   {132, kParamFiltRelease, 500},
  {132, kParamAmpAttack, 1},   {132, kParamAmpDecay, 600},   {132, kParamAmpSustain, 0},
  {132, kParamAmpRelease, 500},   {132, kParamVelToAmp, 48},   {132, kParamVelToFilter, 30},
  {132, kParamStereoSpread, 44},   {132, kParamStereoDelay, 22},   {132, kParamTapeSat, 14},
  {132, kParamTapeMix, 26},   {132, kParamIns1Algo, 4},   {132, kParamIns1A, 22},
  {132, kParamIns1B, 32},   {132, kParamIns1C, 14},   {132, kParamIns1Mix, 14},
  {132, kParamMstAlgo, 2},   {132, kParamMstA, 24},   {132, kParamMstB, 18},
  {132, kParamMstC, 30},   {132, kParamMstMix, 10},
  {133, kParamEngine, 0},   {133, kParamPolyMode, 0},   {133, kParamOsc1Shape, 22},
  {133, kParamOsc1Level, 88},   {133, kParamOsc2Level, 0},   {133, kParamSubLevel, 45},
  {133, kParamNoiseLevel, 0},   {133, kParamFilterType, 0},   {133, kParamFilterCutoff, 10400},
  {133, kParamFilterReso, 18},   {133, kParamFilterEnv, 56},   {133, kParamFiltAttack, 6},
  {133, kParamFiltDecay, 900},   {133, kParamFiltSustain, 38},   {133, kParamFiltRelease, 520},
  {133, kParamAmpAttack, 1},   {133, kParamAmpDecay, 1200},   {133, kParamAmpSustain, 62},
  {133, kParamAmpRelease, 520},   {133, kParamUnisonVoices, 2},   {133, kParamUnisonDetune, 17},
  {133, kParamStereoSpread, 42},   {133, kParamTapeSat, 15},   {133, kParamTapeMix, 28},
  {133, kParamFmAmount, 18},   {133, kParamIns1Algo, 2},   {133, kParamIns1A, 20},
  {133, kParamIns1B, 28},   {133, kParamIns1C, 36},   {133, kParamIns1Mix, 20},
  {133, kParamMstAlgo, 2},   {133, kParamMstA, 24},   {133, kParamMstB, 18},
  {133, kParamMstC, 30},   {133, kParamMstMix, 10},
  {134, kParamEngine, 0},   {134, kParamPolyMode, 0},   {134, kParamOsc1Shape, 42},
  {134, kParamOsc1Level, 82},   {134, kParamOsc2Shape, 40},   {134, kParamOsc2Pitch, 0},
  {134, kParamOsc2Level, 76},   {134, kParamSubLevel, 32},   {134, kParamFilterType, 0},
  {134, kParamFilterCutoff, 6800},   {134, kParamFilterReso, 42},   {134, kParamFilterEnv, 40},
  {134, kParamFiltAttack, 8},   {134, kParamFiltDecay, 1100},   {134, kParamFiltSustain, 34},
  {134, kParamFiltRelease, 600},   {134, kParamAmpAttack, 3},   {134, kParamAmpDecay, 1400},
  {134, kParamAmpSustain, 66},   {134, kParamAmpRelease, 600},   {134, kParamUnisonVoices, 4},
  {134, kParamUnisonDetune, 40},   {134, kParamStereoSpread, 58},   {134, kParamStereoDelay, 0},
  {134, kParamTapeSat, 44},   {134, kParamTapeMix, 48},   {134, kParamFilterDrive, 58},
  {134, kParamIns1Algo, 2},   {134, kParamIns1A, 14},   {134, kParamIns1B, 38},
  {134, kParamIns1C, 44},   {134, kParamIns1Mix, 26},   {134, kParamMstAlgo, 2},
  {134, kParamMstA, 24},   {134, kParamMstB, 18},   {134, kParamMstC, 30},
  {134, kParamMstMix, 10},
  {135, kParamEngine, 0},   {135, kParamPolyMode, 0},   {135, kParamOsc1Shape, 44},
  {135, kParamOsc1Level, 84},   {135, kParamOsc2Shape, 46},   {135, kParamOsc2Pitch, -12},
  {135, kParamOsc2Level, 64},   {135, kParamSubLevel, 20},   {135, kParamFilterType, 0},
  {135, kParamFilterCutoff, 5200},   {135, kParamFilterReso, 48},   {135, kParamFilterEnv, 48},
  {135, kParamFiltAttack, 4},   {135, kParamFiltDecay, 208},   {135, kParamFiltSustain, 4.8},
  {135, kParamFiltRelease, 210},   {135, kParamAmpAttack, 1},   {135, kParamAmpDecay, 260},
  {135, kParamAmpSustain, 8},   {135, kParamAmpRelease, 300},   {135, kParamUnisonVoices, 1},
  {135, kParamUnisonDetune, 10},   {135, kParamStereoSpread, 30},   {135, kParamStereoDelay, 18},
  {135, kParamTapeSat, 20},   {135, kParamTapeMix, 32},   {135, kParamFilterDrive, 62},
  {135, kParamIns1Algo, 4},   {135, kParamIns1A, 22},   {135, kParamIns1B, 32},
  {135, kParamIns1C, 14},   {135, kParamIns1Mix, 14},   {135, kParamMstAlgo, 2},
  {135, kParamMstA, 24},   {135, kParamMstB, 18},   {135, kParamMstC, 30},
  {135, kParamMstMix, 10},
  {136, kParamEngine, 0},   {136, kParamPolyMode, 0},   {136, kParamOsc1Shape, 42},
  {136, kParamOsc1Level, 82},   {136, kParamOsc2Shape, 40},   {136, kParamOsc2Pitch, 7},
  {136, kParamOsc2Level, 76},   {136, kParamSubLevel, 32},   {136, kParamFilterType, 0},
  {136, kParamFilterCutoff, 4600},   {136, kParamFilterReso, 52},   {136, kParamFilterEnv, 40},
  {136, kParamFiltAttack, 8},   {136, kParamFiltDecay, 1100},   {136, kParamFiltSustain, 34},
  {136, kParamFiltRelease, 500},   {136, kParamAmpAttack, 2},   {136, kParamAmpDecay, 1400},
  {136, kParamAmpSustain, 66},   {136, kParamAmpRelease, 500},   {136, kParamUnisonVoices, 4},
  {136, kParamUnisonDetune, 44},   {136, kParamStereoSpread, 58},   {136, kParamStereoDelay, 0},
  {136, kParamTapeSat, 12},   {136, kParamTapeMix, 24},   {136, kParamFilterDrive, 54},
  {136, kParamIns1Algo, 2},   {136, kParamIns1A, 20},   {136, kParamIns1B, 28},
  {136, kParamIns1C, 36},   {136, kParamIns1Mix, 20},   {136, kParamMstAlgo, 2},
  {136, kParamMstA, 24},   {136, kParamMstB, 18},   {136, kParamMstC, 30},
  {136, kParamMstMix, 10},
  {137, kParamEngine, 0},   {137, kParamPolyMode, 0},   {137, kParamOsc1Shape, 36},
  {137, kParamOsc1Level, 90},   {137, kParamOsc2Shape, 34},   {137, kParamOsc2Pitch, -12},
  {137, kParamOsc2Level, 58},   {137, kParamSubLevel, 38},   {137, kParamNoiseLevel, 6},
  {137, kParamFilterType, 0},   {137, kParamFilterCutoff, 2400},   {137, kParamFilterReso, 46},
  {137, kParamFilterDrive, 32},   {137, kParamFilterEnv, 44},   {137, kParamFiltAttack, 8},
  {137, kParamFiltDecay, 700},   {137, kParamFiltSustain, 28},   {137, kParamFiltRelease, 500},
  {137, kParamAmpAttack, 2},   {137, kParamAmpDecay, 700},   {137, kParamAmpSustain, 40},
  {137, kParamAmpRelease, 500},   {137, kParamUnisonVoices, 2},   {137, kParamUnisonDetune, 14},
  {137, kParamStereoSpread, 34},   {137, kParamTapeSat, 30},   {137, kParamTapeMix, 36},
  {137, kParamIns1Algo, 2},   {137, kParamIns1A, 14},   {137, kParamIns1B, 38},
  {137, kParamIns1C, 44},   {137, kParamIns1Mix, 26},   {137, kParamMstAlgo, 2},
  {137, kParamMstA, 24},   {137, kParamMstB, 18},   {137, kParamMstC, 30},
  {137, kParamMstMix, 10},
  {138, kParamEngine, 0},   {138, kParamPolyMode, 0},   {138, kParamOsc1Shape, 44},
  {138, kParamOsc1Level, 84},   {138, kParamOsc2Shape, 46},   {138, kParamOsc2Pitch, -12},
  {138, kParamOsc2Level, 64},   {138, kParamSubLevel, 20},   {138, kParamFilterType, 0},
  {138, kParamFilterCutoff, 7400},   {138, kParamFilterReso, 36},   {138, kParamFilterEnv, 48},
  {138, kParamFiltAttack, 4},   {138, kParamFiltDecay, 560},   {138, kParamFiltSustain, 12},
  {138, kParamFiltRelease, 489.99999999999994},   {138, kParamAmpAttack, 4},   {138, kParamAmpDecay, 700},
  {138, kParamAmpSustain, 20},   {138, kParamAmpRelease, 700},   {138, kParamUnisonVoices, 1},
  {138, kParamUnisonDetune, 10},   {138, kParamStereoSpread, 30},   {138, kParamStereoDelay, 18},
  {138, kParamTapeSat, 40},   {138, kParamTapeMix, 32},   {138, kParamFilterDrive, 50},
  {138, kParamIns1Algo, 4},   {138, kParamIns1A, 22},   {138, kParamIns1B, 32},
  {138, kParamIns1C, 14},   {138, kParamIns1Mix, 14},   {138, kParamMstAlgo, 2},
  {138, kParamMstA, 24},   {138, kParamMstB, 18},   {138, kParamMstC, 30},
  {138, kParamMstMix, 10},
  {139, kParamEngine, 0},   {139, kParamPolyMode, 0},   {139, kParamOsc1Shape, 30},
  {139, kParamOsc1Level, 58},   {139, kParamOsc2Level, 0},   {139, kParamOsc3Enable, 1},
  {139, kParamOsc3Source, 1},   {139, kParamOsc3Morph, 6.4},   {139, kParamOsc3Level, 86},
  {139, kParamOsc3Pitch, 0},   {139, kParamSubLevel, 26},   {139, kParamFilterType, 0},
  {139, kParamFilterCutoff, 6000},   {139, kParamFilterReso, 40},   {139, kParamFilterEnv, 28},
  {139, kParamFiltAttack, 10},   {139, kParamFiltDecay, 1400},   {139, kParamFiltSustain, 44},
  {139, kParamFiltRelease, 800},   {139, kParamAmpAttack, 3},   {139, kParamAmpDecay, 1600},
  {139, kParamAmpSustain, 70},   {139, kParamAmpRelease, 800},   {139, kParamUnisonVoices, 4},
  {139, kParamUnisonDetune, 12},   {139, kParamStereoSpread, 52},   {139, kParamTapeSat, 18},
  {139, kParamTapeMix, 30},   {139, kParamFilterDrive, 56},   {139, kParamIns1Algo, 2},
  {139, kParamIns1A, 20},   {139, kParamIns1B, 28},   {139, kParamIns1C, 36},
  {139, kParamIns1Mix, 20},   {139, kParamMstAlgo, 2},   {139, kParamMstA, 24},
  {139, kParamMstB, 18},   {139, kParamMstC, 30},   {139, kParamMstMix, 10},
  {140, kParamEngine, 0},   {140, kParamPolyMode, 0},   {140, kParamOsc1Shape, 42},
  {140, kParamOsc1Level, 82},   {140, kParamOsc2Shape, 40},   {140, kParamOsc2Pitch, 0},
  {140, kParamOsc2Level, 76},   {140, kParamSubLevel, 32},   {140, kParamFilterType, 0},
  {140, kParamFilterCutoff, 6200},   {140, kParamFilterReso, 14},   {140, kParamFilterEnv, 40},
  {140, kParamFiltAttack, 8},   {140, kParamFiltDecay, 1100},   {140, kParamFiltSustain, 34},
  {140, kParamFiltRelease, 2600},   {140, kParamAmpAttack, 420},   {140, kParamAmpDecay, 1400},
  {140, kParamAmpSustain, 66},   {140, kParamAmpRelease, 2600},   {140, kParamUnisonVoices, 4},
  {140, kParamUnisonDetune, 24},   {140, kParamStereoSpread, 84},   {140, kParamStereoDelay, 0},
  {140, kParamTapeSat, 12},   {140, kParamTapeMix, 24},   {140, kParamFilterKeyTrack, 40},
  {140, kParamIns1Algo, 2},   {140, kParamIns1A, 14},   {140, kParamIns1B, 38},
  {140, kParamIns1C, 44},   {140, kParamIns1Mix, 26},   {140, kParamMstAlgo, 2},
  {140, kParamMstA, 24},   {140, kParamMstB, 18},   {140, kParamMstC, 30},
  {140, kParamMstMix, 10},
  {141, kParamEngine, 0},   {141, kParamPolyMode, 0},   {141, kParamOsc1Shape, 40},
  {141, kParamOsc1Level, 82},   {141, kParamOsc2Shape, 38},   {141, kParamOsc2Pitch, 0},
  {141, kParamOsc2Level, 76},   {141, kParamSubLevel, 32},   {141, kParamFilterType, 0},
  {141, kParamFilterCutoff, 4400},   {141, kParamFilterReso, 24},   {141, kParamFilterEnv, 52},
  {141, kParamFiltAttack, 90},   {141, kParamFiltDecay, 1100},   {141, kParamFiltSustain, 34},
  {141, kParamFiltRelease, 1200},   {141, kParamAmpAttack, 60},   {141, kParamAmpDecay, 1400},
  {141, kParamAmpSustain, 66},   {141, kParamAmpRelease, 1200},   {141, kParamUnisonVoices, 4},
  {141, kParamUnisonDetune, 18},   {141, kParamStereoSpread, 58},   {141, kParamStereoDelay, 0},
  {141, kParamTapeSat, 12},   {141, kParamTapeMix, 24},   {141, kParamIns1Algo, 4},
  {141, kParamIns1A, 22},   {141, kParamIns1B, 32},   {141, kParamIns1C, 14},
  {141, kParamIns1Mix, 14},   {141, kParamMstAlgo, 2},   {141, kParamMstA, 24},
  {141, kParamMstB, 18},   {141, kParamMstC, 30},   {141, kParamMstMix, 10},
  {142, kParamEngine, 0},   {142, kParamPolyMode, 0},   {142, kParamOsc1Shape, 30},
  {142, kParamOsc1Level, 58},   {142, kParamOsc2Level, 0},   {142, kParamOsc3Enable, 1},
  {142, kParamOsc3Source, 1},   {142, kParamOsc3Morph, 3.4},   {142, kParamOsc3Level, 86},
  {142, kParamOsc3Pitch, 0},   {142, kParamSubLevel, 26},   {142, kParamFilterType, 0},
  {142, kParamFilterCutoff, 5600},   {142, kParamFilterReso, 16},   {142, kParamFilterEnv, 28},
  {142, kParamFiltAttack, 10},   {142, kParamFiltDecay, 1400},   {142, kParamFiltSustain, 44},
  {142, kParamFiltRelease, 3200},   {142, kParamAmpAttack, 520},   {142, kParamAmpDecay, 1600},
  {142, kParamAmpSustain, 70},   {142, kParamAmpRelease, 3200},   {142, kParamUnisonVoices, 4},
  {142, kParamUnisonDetune, 12},   {142, kParamStereoSpread, 88},   {142, kParamTapeSat, 18},
  {142, kParamTapeMix, 30},   {142, kParamIns1Algo, 2},   {142, kParamIns1A, 20},
  {142, kParamIns1B, 28},   {142, kParamIns1C, 36},   {142, kParamIns1Mix, 20},
  {142, kParamMstAlgo, 2},   {142, kParamMstA, 24},   {142, kParamMstB, 18},
  {142, kParamMstC, 30},   {142, kParamMstMix, 10},
  {143, kParamEngine, 0},   {143, kParamPolyMode, 0},   {143, kParamOsc1Shape, 36},
  {143, kParamOsc1Level, 88},   {143, kParamOsc2Level, 0},   {143, kParamSubLevel, 45},
  {143, kParamNoiseLevel, 0},   {143, kParamFilterType, 0},   {143, kParamFilterCutoff, 7000},
  {143, kParamFilterReso, 12},   {143, kParamFilterEnv, 32},   {143, kParamFiltAttack, 6},
  {143, kParamFiltDecay, 900},   {143, kParamFiltSustain, 38},   {143, kParamFiltRelease, 2800},
  {143, kParamAmpAttack, 380},   {143, kParamAmpDecay, 1200},   {143, kParamAmpSustain, 62},
  {143, kParamAmpRelease, 2800},   {143, kParamUnisonVoices, 2},   {143, kParamUnisonDetune, 17},
  {143, kParamStereoSpread, 42},   {143, kParamTapeSat, 14},   {143, kParamTapeMix, 28},
  {143, kParamIns1Algo, 2},   {143, kParamIns1A, 14},   {143, kParamIns1B, 38},
  {143, kParamIns1C, 44},   {143, kParamIns1Mix, 26},   {143, kParamMstAlgo, 2},
  {143, kParamMstA, 24},   {143, kParamMstB, 18},   {143, kParamMstC, 30},
  {143, kParamMstMix, 10},
  {144, kParamEngine, 0},   {144, kParamPolyMode, 0},   {144, kParamOsc1Shape, 42},
  {144, kParamOsc1Level, 82},   {144, kParamOsc2Shape, 40},   {144, kParamOsc2Pitch, 0},
  {144, kParamOsc2Level, 76},   {144, kParamSubLevel, 32},   {144, kParamFilterType, 0},
  {144, kParamFilterCutoff, 5200},   {144, kParamFilterReso, 18},   {144, kParamFilterEnv, 40},
  {144, kParamFiltAttack, 8},   {144, kParamFiltDecay, 1100},   {144, kParamFiltSustain, 34},
  {144, kParamFiltRelease, 3000},   {144, kParamAmpAttack, 460},   {144, kParamAmpDecay, 1400},
  {144, kParamAmpSustain, 66},   {144, kParamAmpRelease, 3000},   {144, kParamUnisonVoices, 4},
  {144, kParamUnisonDetune, 28},   {144, kParamStereoSpread, 90},   {144, kParamStereoDelay, 0},
  {144, kParamTapeSat, 12},   {144, kParamTapeMix, 24},   {144, kParamIns1Algo, 4},
  {144, kParamIns1A, 22},   {144, kParamIns1B, 32},   {144, kParamIns1C, 14},
  {144, kParamIns1Mix, 14},   {144, kParamMstAlgo, 2},   {144, kParamMstA, 24},
  {144, kParamMstB, 18},   {144, kParamMstC, 30},   {144, kParamMstMix, 10},
  {145, kParamEngine, 0},   {145, kParamPolyMode, 0},   {145, kParamOsc1Shape, 36},
  {145, kParamOsc1Level, 90},   {145, kParamOsc2Shape, 34},   {145, kParamOsc2Pitch, -12},
  {145, kParamOsc2Level, 58},   {145, kParamSubLevel, 38},   {145, kParamNoiseLevel, 6},
  {145, kParamFilterType, 0},   {145, kParamFilterCutoff, 3400},   {145, kParamFilterReso, 22},
  {145, kParamFilterDrive, 32},   {145, kParamFilterEnv, 44},   {145, kParamFiltAttack, 8},
  {145, kParamFiltDecay, 1400},   {145, kParamFiltSustain, 40.599999999999994},   {145, kParamFiltRelease, 900},
  {145, kParamAmpAttack, 80},   {145, kParamAmpDecay, 1400},   {145, kParamAmpSustain, 58},
  {145, kParamAmpRelease, 900},   {145, kParamUnisonVoices, 2},   {145, kParamUnisonDetune, 14},
  {145, kParamStereoSpread, 34},   {145, kParamTapeSat, 30},   {145, kParamTapeMix, 36},
  {145, kParamIns1Algo, 2},   {145, kParamIns1A, 20},   {145, kParamIns1B, 28},
  {145, kParamIns1C, 36},   {145, kParamIns1Mix, 20},   {145, kParamMstAlgo, 2},
  {145, kParamMstA, 24},   {145, kParamMstB, 18},   {145, kParamMstC, 30},
  {145, kParamMstMix, 10},
  {146, kParamEngine, 0},   {146, kParamPolyMode, 0},   {146, kParamOsc1Shape, 44},
  {146, kParamOsc1Level, 84},   {146, kParamOsc2Shape, 46},   {146, kParamOsc2Pitch, -12},
  {146, kParamOsc2Level, 64},   {146, kParamSubLevel, 20},   {146, kParamFilterType, 0},
  {146, kParamFilterCutoff, 5800},   {146, kParamFilterReso, 16},   {146, kParamFilterEnv, 48},
  {146, kParamFiltAttack, 4},   {146, kParamFiltDecay, 1760},   {146, kParamFiltSustain, 37.199999999999996},
  {146, kParamFiltRelease, 1680},   {146, kParamAmpAttack, 400},   {146, kParamAmpDecay, 2200},
  {146, kParamAmpSustain, 62},   {146, kParamAmpRelease, 2400},   {146, kParamUnisonVoices, 1},
  {146, kParamUnisonDetune, 10},   {146, kParamStereoSpread, 76},   {146, kParamStereoDelay, 18},
  {146, kParamTapeSat, 20},   {146, kParamTapeMix, 32},   {146, kParamPorta, 40},
  {146, kParamMonoLegato, 1},   {146, kParamIns1Algo, 2},   {146, kParamIns1A, 14},
  {146, kParamIns1B, 38},   {146, kParamIns1C, 44},   {146, kParamIns1Mix, 26},
  {146, kParamMstAlgo, 2},   {146, kParamMstA, 24},   {146, kParamMstB, 18},
  {146, kParamMstC, 30},   {146, kParamMstMix, 10},
  {147, kParamEngine, 0},   {147, kParamPolyMode, 0},   {147, kParamOsc1Shape, 38},
  {147, kParamOsc1Level, 88},   {147, kParamOsc2Level, 0},   {147, kParamSubLevel, 45},
  {147, kParamNoiseLevel, 0},   {147, kParamFilterType, 0},   {147, kParamFilterCutoff, 6400},
  {147, kParamFilterReso, 20},   {147, kParamFilterEnv, 32},   {147, kParamFiltAttack, 6},
  {147, kParamFiltDecay, 900},   {147, kParamFiltSustain, 38},   {147, kParamFiltRelease, 1800},
  {147, kParamAmpAttack, 12},   {147, kParamAmpDecay, 1200},   {147, kParamAmpSustain, 62},
  {147, kParamAmpRelease, 1800},   {147, kParamUnisonVoices, 2},   {147, kParamUnisonDetune, 17},
  {147, kParamStereoSpread, 42},   {147, kParamTapeSat, 34},   {147, kParamTapeMix, 44},
  {147, kParamTapeWow, 42},   {147, kParamTapeFlutter, 30},   {147, kParamTapeHiss, 12},
  {147, kParamIns1Algo, 4},   {147, kParamIns1A, 22},   {147, kParamIns1B, 32},
  {147, kParamIns1C, 14},   {147, kParamIns1Mix, 14},   {147, kParamMstAlgo, 2},
  {147, kParamMstA, 24},   {147, kParamMstB, 18},   {147, kParamMstC, 30},
  {147, kParamMstMix, 10},
  {148, kParamEngine, 0},   {148, kParamPolyMode, 0},   {148, kParamOsc1Shape, 42},
  {148, kParamOsc1Level, 82},   {148, kParamOsc2Shape, 40},   {148, kParamOsc2Pitch, 0},
  {148, kParamOsc2Level, 76},   {148, kParamSubLevel, 32},   {148, kParamFilterType, 0},
  {148, kParamFilterCutoff, 5200},   {148, kParamFilterReso, 24},   {148, kParamFilterEnv, 40},
  {148, kParamFiltAttack, 8},   {148, kParamFiltDecay, 1100},   {148, kParamFiltSustain, 34},
  {148, kParamFiltRelease, 2800},   {148, kParamAmpAttack, 340},   {148, kParamAmpDecay, 1400},
  {148, kParamAmpSustain, 66},   {148, kParamAmpRelease, 2800},   {148, kParamUnisonVoices, 4},
  {148, kParamUnisonDetune, 46},   {148, kParamStereoSpread, 80},   {148, kParamStereoDelay, 0},
  {148, kParamTapeSat, 12},   {148, kParamTapeMix, 24},   {148, kParamTapeWow, 48},
  {148, kParamTapeFlutter, 34},   {148, kParamIns1Algo, 2},   {148, kParamIns1A, 20},
  {148, kParamIns1B, 28},   {148, kParamIns1C, 36},   {148, kParamIns1Mix, 20},
  {148, kParamMstAlgo, 2},   {148, kParamMstA, 24},   {148, kParamMstB, 18},
  {148, kParamMstC, 30},   {148, kParamMstMix, 10},
  {149, kParamEngine, 0},   {149, kParamPolyMode, 0},   {149, kParamOsc1Shape, 44},
  {149, kParamOsc1Level, 84},   {149, kParamOsc2Shape, 46},   {149, kParamOsc2Pitch, -12},
  {149, kParamOsc2Level, 64},   {149, kParamSubLevel, 20},   {149, kParamFilterType, 0},
  {149, kParamFilterCutoff, 5600},   {149, kParamFilterReso, 30},   {149, kParamFilterEnv, 48},
  {149, kParamFiltAttack, 4},   {149, kParamFiltDecay, 320},   {149, kParamFiltSustain, 8.4},
  {149, kParamFiltRelease, 350},   {149, kParamAmpAttack, 4},   {149, kParamAmpDecay, 400},
  {149, kParamAmpSustain, 14},   {149, kParamAmpRelease, 500},   {149, kParamUnisonVoices, 1},
  {149, kParamUnisonDetune, 10},   {149, kParamStereoSpread, 30},   {149, kParamStereoDelay, 18},
  {149, kParamTapeSat, 20},   {149, kParamTapeMix, 40},   {149, kParamOsc1PW, 58},
  {149, kParamTapeWow, 38},   {149, kParamIns1Algo, 2},   {149, kParamIns1A, 14},
  {149, kParamIns1B, 38},   {149, kParamIns1C, 44},   {149, kParamIns1Mix, 26},
  {149, kParamMstAlgo, 2},   {149, kParamMstA, 24},   {149, kParamMstB, 18},
  {149, kParamMstC, 30},   {149, kParamMstMix, 10},
  {150, kParamEngine, 0},   {150, kParamPolyMode, 0},   {150, kParamOsc1Shape, 38},
  {150, kParamOsc1Level, 88},   {150, kParamOsc2Level, 0},   {150, kParamSubLevel, 45},
  {150, kParamNoiseLevel, 0},   {150, kParamFilterType, 0},   {150, kParamFilterCutoff, 6800},
  {150, kParamFilterReso, 14},   {150, kParamFilterEnv, 32},   {150, kParamFiltAttack, 6},
  {150, kParamFiltDecay, 900},   {150, kParamFiltSustain, 38},   {150, kParamFiltRelease, 2000},
  {150, kParamAmpAttack, 8},   {150, kParamAmpDecay, 1200},   {150, kParamAmpSustain, 62},
  {150, kParamAmpRelease, 2000},   {150, kParamUnisonVoices, 2},   {150, kParamUnisonDetune, 17},
  {150, kParamStereoSpread, 42},   {150, kParamTapeSat, 38},   {150, kParamTapeMix, 46},
  {150, kParamFmAmount, 30},   {150, kParamTapeFlutter, 36},   {150, kParamIns1Algo, 4},
  {150, kParamIns1A, 22},   {150, kParamIns1B, 32},   {150, kParamIns1C, 14},
  {150, kParamIns1Mix, 14},   {150, kParamMstAlgo, 2},   {150, kParamMstA, 24},
  {150, kParamMstB, 18},   {150, kParamMstC, 30},   {150, kParamMstMix, 10},
  {151, kParamEngine, 0},   {151, kParamPolyMode, 0},   {151, kParamOsc1Shape, 30},
  {151, kParamOsc1Level, 58},   {151, kParamOsc2Level, 0},   {151, kParamOsc3Enable, 1},
  {151, kParamOsc3Source, 1},   {151, kParamOsc3Morph, 2},   {151, kParamOsc3Level, 86},
  {151, kParamOsc3Pitch, 0},   {151, kParamSubLevel, 26},   {151, kParamFilterType, 0},
  {151, kParamFilterCutoff, 6000},   {151, kParamFilterReso, 22},   {151, kParamFilterEnv, 28},
  {151, kParamFiltAttack, 10},   {151, kParamFiltDecay, 1400},   {151, kParamFiltSustain, 44},
  {151, kParamFiltRelease, 2600},   {151, kParamAmpAttack, 220},   {151, kParamAmpDecay, 1600},
  {151, kParamAmpSustain, 70},   {151, kParamAmpRelease, 2600},   {151, kParamUnisonVoices, 2},
  {151, kParamUnisonDetune, 28},   {151, kParamStereoSpread, 52},   {151, kParamTapeSat, 18},
  {151, kParamTapeMix, 30},   {151, kParamTapeWow, 44},   {151, kParamIns1Algo, 2},
  {151, kParamIns1A, 20},   {151, kParamIns1B, 28},   {151, kParamIns1C, 36},
  {151, kParamIns1Mix, 20},   {151, kParamMstAlgo, 2},   {151, kParamMstA, 24},
  {151, kParamMstB, 18},   {151, kParamMstC, 30},   {151, kParamMstMix, 10},
  {152, kParamEngine, 0},   {152, kParamPolyMode, 0},   {152, kParamOsc1Shape, 36},
  {152, kParamOsc1Level, 90},   {152, kParamOsc2Shape, 34},   {152, kParamOsc2Pitch, -12},
  {152, kParamOsc2Level, 58},   {152, kParamSubLevel, 38},   {152, kParamNoiseLevel, 6},
  {152, kParamFilterType, 0},   {152, kParamFilterCutoff, 2600},   {152, kParamFilterReso, 28},
  {152, kParamFilterDrive, 32},   {152, kParamFilterEnv, 44},   {152, kParamFiltAttack, 8},
  {152, kParamFiltDecay, 1200},   {152, kParamFiltSustain, 33.599999999999994},   {152, kParamFiltRelease, 1000},
  {152, kParamAmpAttack, 10},   {152, kParamAmpDecay, 1200},   {152, kParamAmpSustain, 48},
  {152, kParamAmpRelease, 1000},   {152, kParamUnisonVoices, 2},   {152, kParamUnisonDetune, 14},
  {152, kParamStereoSpread, 34},   {152, kParamTapeSat, 30},   {152, kParamTapeMix, 36},
  {152, kParamIns1Algo, 2},   {152, kParamIns1A, 14},   {152, kParamIns1B, 38},
  {152, kParamIns1C, 44},   {152, kParamIns1Mix, 26},   {152, kParamMstAlgo, 2},
  {152, kParamMstA, 24},   {152, kParamMstB, 18},   {152, kParamMstC, 30},
  {152, kParamMstMix, 10},
  {153, kParamEngine, 0},   {153, kParamPolyMode, 0},   {153, kParamOsc1Shape, 20},
  {153, kParamOsc1Level, 72},   {153, kParamOsc2Shape, 22},   {153, kParamOsc2Pitch, 19},
  {153, kParamOsc2Level, 48},   {153, kParamFmAmount, 26},   {153, kParamSubLevel, 18},
  {153, kParamNoiseLevel, 4},   {153, kParamFilterType, 0},   {153, kParamFilterCutoff, 7200},
  {153, kParamFilterReso, 16},   {153, kParamFilterEnv, 24},   {153, kParamFiltAttack, 3},
  {153, kParamFiltDecay, 1500},   {153, kParamFiltSustain, 15},   {153, kParamFiltRelease, 1600},
  {153, kParamAmpAttack, 6},   {153, kParamAmpDecay, 1500},   {153, kParamAmpSustain, 30},
  {153, kParamAmpRelease, 1600},   {153, kParamVelToAmp, 48},   {153, kParamVelToFilter, 30},
  {153, kParamStereoSpread, 44},   {153, kParamStereoDelay, 22},   {153, kParamTapeSat, 14},
  {153, kParamTapeMix, 42},   {153, kParamTapeWow, 40},   {153, kParamTapeHiss, 10},
  {153, kParamIns1Algo, 4},   {153, kParamIns1A, 22},   {153, kParamIns1B, 32},
  {153, kParamIns1C, 14},   {153, kParamIns1Mix, 14},   {153, kParamMstAlgo, 2},
  {153, kParamMstA, 24},   {153, kParamMstB, 18},   {153, kParamMstC, 30},
  {153, kParamMstMix, 10},
  {154, kParamEngine, 0},   {154, kParamPolyMode, 0},   {154, kParamOsc1Shape, 42},
  {154, kParamOsc1Level, 82},   {154, kParamOsc2Shape, 40},   {154, kParamOsc2Pitch, 0},
  {154, kParamOsc2Level, 76},   {154, kParamSubLevel, 32},   {154, kParamFilterType, 0},
  {154, kParamFilterCutoff, 7200},   {154, kParamFilterReso, 28},   {154, kParamFilterEnv, 40},
  {154, kParamFiltAttack, 8},   {154, kParamFiltDecay, 1100},   {154, kParamFiltSustain, 34},
  {154, kParamFiltRelease, 1400},   {154, kParamAmpAttack, 20},   {154, kParamAmpDecay, 1400},
  {154, kParamAmpSustain, 66},   {154, kParamAmpRelease, 1400},   {154, kParamUnisonVoices, 6},
  {154, kParamUnisonDetune, 32},   {154, kParamStereoSpread, 88},   {154, kParamStereoDelay, 0},
  {154, kParamTapeSat, 12},   {154, kParamTapeMix, 24},   {154, kParamIns1Algo, 2},
  {154, kParamIns1A, 20},   {154, kParamIns1B, 28},   {154, kParamIns1C, 36},
  {154, kParamIns1Mix, 20},   {154, kParamMstAlgo, 2},   {154, kParamMstA, 24},
  {154, kParamMstB, 18},   {154, kParamMstC, 30},   {154, kParamMstMix, 10},
  {155, kParamEngine, 0},   {155, kParamPolyMode, 0},   {155, kParamOsc1Shape, 44},
  {155, kParamOsc1Level, 84},   {155, kParamOsc2Shape, 46},   {155, kParamOsc2Pitch, 0},
  {155, kParamOsc2Level, 64},   {155, kParamSubLevel, 20},   {155, kParamFilterType, 0},
  {155, kParamFilterCutoff, 4800},   {155, kParamFilterReso, 26},   {155, kParamFilterEnv, 56},
  {155, kParamFiltAttack, 50},   {155, kParamFiltDecay, 720},   {155, kParamFiltSustain, 32.4},
  {155, kParamFiltRelease, 560},   {155, kParamAmpAttack, 40},   {155, kParamAmpDecay, 900},
  {155, kParamAmpSustain, 54},   {155, kParamAmpRelease, 800},   {155, kParamUnisonVoices, 1},
  {155, kParamUnisonDetune, 10},   {155, kParamStereoSpread, 30},   {155, kParamStereoDelay, 18},
  {155, kParamTapeSat, 20},   {155, kParamTapeMix, 32},   {155, kParamIns1Algo, 2},
  {155, kParamIns1A, 14},   {155, kParamIns1B, 38},   {155, kParamIns1C, 44},
  {155, kParamIns1Mix, 26},   {155, kParamMstAlgo, 2},   {155, kParamMstA, 24},
  {155, kParamMstB, 18},   {155, kParamMstC, 30},   {155, kParamMstMix, 10},
  {156, kParamEngine, 0},   {156, kParamPolyMode, 0},   {156, kParamOsc1Shape, 38},
  {156, kParamOsc1Level, 88},   {156, kParamOsc2Level, 0},   {156, kParamSubLevel, 45},
  {156, kParamNoiseLevel, 0},   {156, kParamFilterType, 0},   {156, kParamFilterCutoff, 6000},
  {156, kParamFilterReso, 10},   {156, kParamFilterEnv, 32},   {156, kParamFiltAttack, 6},
  {156, kParamFiltDecay, 900},   {156, kParamFiltSustain, 38},   {156, kParamFiltRelease, 2400},
  {156, kParamAmpAttack, 300},   {156, kParamAmpDecay, 1200},   {156, kParamAmpSustain, 62},
  {156, kParamAmpRelease, 2400},   {156, kParamUnisonVoices, 1},   {156, kParamUnisonDetune, 17},
  {156, kParamStereoSpread, 42},   {156, kParamTapeSat, 16},   {156, kParamTapeMix, 28},
  {156, kParamIns1Algo, 4},   {156, kParamIns1A, 22},   {156, kParamIns1B, 32},
  {156, kParamIns1C, 14},   {156, kParamIns1Mix, 14},   {156, kParamMstAlgo, 2},
  {156, kParamMstA, 24},   {156, kParamMstB, 18},   {156, kParamMstC, 30},
  {156, kParamMstMix, 10},
  {157, kParamEngine, 0},   {157, kParamPolyMode, 0},   {157, kParamOsc1Shape, 36},
  {157, kParamOsc1Level, 90},   {157, kParamOsc2Shape, 34},   {157, kParamOsc2Pitch, -12},
  {157, kParamOsc2Level, 58},   {157, kParamSubLevel, 38},   {157, kParamNoiseLevel, 6},
  {157, kParamFilterType, 0},   {157, kParamFilterCutoff, 3000},   {157, kParamFilterReso, 38},
  {157, kParamFilterDrive, 32},   {157, kParamFilterEnv, 44},   {157, kParamFiltAttack, 8},
  {157, kParamFiltDecay, 1000},   {157, kParamFiltSustain, 36.4},   {157, kParamFiltRelease, 900},
  {157, kParamAmpAttack, 12},   {157, kParamAmpDecay, 1000},   {157, kParamAmpSustain, 52},
  {157, kParamAmpRelease, 900},   {157, kParamUnisonVoices, 2},   {157, kParamUnisonDetune, 14},
  {157, kParamStereoSpread, 34},   {157, kParamTapeSat, 30},   {157, kParamTapeMix, 36},
  {157, kParamIns1Algo, 2},   {157, kParamIns1A, 20},   {157, kParamIns1B, 28},
  {157, kParamIns1C, 36},   {157, kParamIns1Mix, 20},   {157, kParamMstAlgo, 2},
  {157, kParamMstA, 24},   {157, kParamMstB, 18},   {157, kParamMstC, 30},
  {157, kParamMstMix, 10},
  {158, kParamEngine, 0},   {158, kParamPolyMode, 0},   {158, kParamOsc1Shape, 30},
  {158, kParamOsc1Level, 58},   {158, kParamOsc2Level, 0},   {158, kParamOsc3Enable, 1},
  {158, kParamOsc3Source, 1},   {158, kParamOsc3Morph, 0.5},   {158, kParamOsc3Level, 86},
  {158, kParamOsc3Pitch, 0},   {158, kParamSubLevel, 26},   {158, kParamFilterType, 0},
  {158, kParamFilterCutoff, 7600},   {158, kParamFilterReso, 30},   {158, kParamFilterEnv, 28},
  {158, kParamFiltAttack, 10},   {158, kParamFiltDecay, 1400},   {158, kParamFiltSustain, 44},
  {158, kParamFiltRelease, 2200},   {158, kParamAmpAttack, 140},   {158, kParamAmpDecay, 1600},
  {158, kParamAmpSustain, 70},   {158, kParamAmpRelease, 2200},   {158, kParamUnisonVoices, 2},
  {158, kParamUnisonDetune, 12},   {158, kParamStereoSpread, 82},   {158, kParamTapeSat, 18},
  {158, kParamTapeMix, 30},   {158, kParamLfoRate, 0.28},   {158, kParamLfoDest, 16},
  {158, kParamLfoAmount, 64},   {158, kParamIns1Algo, 2},   {158, kParamIns1A, 14},
  {158, kParamIns1B, 38},   {158, kParamIns1C, 44},   {158, kParamIns1Mix, 26},
  {158, kParamMstAlgo, 2},   {158, kParamMstA, 24},   {158, kParamMstB, 18},
  {158, kParamMstC, 30},   {158, kParamMstMix, 10},
  // lead
  {159, kParamEngine, 0},   {159, kParamPolyMode, 0},   {159, kParamPorta, 8},
  {159, kParamOsc1Shape, 44},   {159, kParamOsc1Level, 86},   {159, kParamOsc2Shape, 42},
  {159, kParamOsc2Pitch, 7},   {159, kParamOsc2Level, 74},   {159, kParamSubLevel, 24},
  {159, kParamNoiseLevel, 0},   {159, kParamFilterType, 0},   {159, kParamFilterCutoff, 9600},
  {159, kParamFilterReso, 26},   {159, kParamFilterDrive, 10},   {159, kParamFilterEnv, 54},
  {159, kParamFiltAttack, 2},   {159, kParamFiltDecay, 700},   {159, kParamFiltSustain, 48},
  {159, kParamFiltRelease, 800},   {159, kParamAmpAttack, 3},   {159, kParamAmpDecay, 700},
  {159, kParamAmpSustain, 48},   {159, kParamAmpRelease, 800},   {159, kParamPitchAmount, -14},
  {159, kParamPitchAttack, 1},   {159, kParamPitchDecay, 90},   {159, kParamPitchSustain, 0},
  {159, kParamPitchRelease, 200},   {159, kParamUnisonVoices, 2},   {159, kParamUnisonDetune, 16},
  {159, kParamVelToAmp, 40},   {159, kParamVelToFilter, 34},   {159, kParamStereoSpread, 52},
  {159, kParamStereoDelay, 20},   {159, kParamTapeSat, 26},   {159, kParamTapeMix, 30},
  {159, kParamIns1Algo, 3},   {159, kParamIns1A, 34},   {159, kParamIns1B, 30},
  {159, kParamIns1C, 54},   {159, kParamIns1Mix, 18},   {159, kParamIns2Algo, 4},
  {159, kParamIns2A, 24},   {159, kParamIns2B, 34},   {159, kParamIns2C, 12},
  {159, kParamIns2Mix, 14},   {159, kParamMstAlgo, 2},   {159, kParamMstA, 24},
  {159, kParamMstB, 18},   {159, kParamMstC, 30},   {159, kParamMstMix, 10},
  {160, kParamEngine, 0},   {160, kParamPolyMode, 0},   {160, kParamPorta, 0},
  {160, kParamOsc1Shape, 52},   {160, kParamOsc1PW, 34},   {160, kParamOsc1Level, 88},
  {160, kParamOsc2Shape, 50},   {160, kParamOsc2Pitch, -12},   {160, kParamOsc2Level, 56},
  {160, kParamSubLevel, 30},   {160, kParamFilterType, 0},   {160, kParamFilterCutoff, 8400},
  {160, kParamFilterReso, 32},   {160, kParamFilterDrive, 0},   {160, kParamFilterEnv, 52},
  {160, kParamFiltAttack, 2},   {160, kParamFiltDecay, 600},   {160, kParamFiltSustain, 46},
  {160, kParamFiltRelease, 700},   {160, kParamAmpAttack, 3},   {160, kParamAmpDecay, 800},
  {160, kParamAmpSustain, 62},   {160, kParamAmpRelease, 700},   {160, kParamPitchAmount, -10},
  {160, kParamPitchDecay, 70},   {160, kParamUnisonVoices, 2},   {160, kParamUnisonDetune, 14},
  {160, kParamVelToAmp, 42},   {160, kParamVelToFilter, 32},   {160, kParamStereoSpread, 32},
  {160, kParamStereoDelay, 24},   {160, kParamTapeSat, 28},   {160, kParamTapeMix, 32},
  {160, kParamIns1Algo, 3},   {160, kParamIns1A, 44},   {160, kParamIns1B, 36},
  {160, kParamIns1C, 60},   {160, kParamIns1Mix, 24},   {160, kParamIns2Algo, 3},
  {160, kParamIns2A, 30},   {160, kParamIns2B, 28},   {160, kParamIns2C, 44},
  {160, kParamIns2Mix, 12},   {160, kParamMstAlgo, 2},   {160, kParamMstA, 24},
  {160, kParamMstB, 18},   {160, kParamMstC, 30},   {160, kParamMstMix, 10},
  {161, kParamEngine, 0},   {161, kParamPolyMode, 0},   {161, kParamPorta, 0},
  {161, kParamOsc1Shape, 30},   {161, kParamOsc1Level, 54},   {161, kParamOsc2Level, 0},
  {161, kParamOsc3Enable, 1},   {161, kParamOsc3Source, 1},   {161, kParamOsc3Morph, 2.6},
  {161, kParamOsc3Level, 88},   {161, kParamOsc3Pitch, 12},   {161, kParamSubLevel, 20},
  {161, kParamFilterType, 0},   {161, kParamFilterCutoff, 10200},   {161, kParamFilterReso, 22},
  {161, kParamFilterEnv, 44},   {161, kParamFiltAttack, 2},   {161, kParamFiltDecay, 900},
  {161, kParamFiltSustain, 48},   {161, kParamFiltRelease, 1200},   {161, kParamAmpAttack, 3},
  {161, kParamAmpDecay, 1000},   {161, kParamAmpSustain, 64},   {161, kParamAmpRelease, 1200},
  {161, kParamUnisonVoices, 2},   {161, kParamUnisonDetune, 13},   {161, kParamVelToAmp, 46},
  {161, kParamVelToFilter, 30},   {161, kParamStereoSpread, 44},   {161, kParamStereoDelay, 30},
  {161, kParamTapeSat, 24},   {161, kParamTapeMix, 32},   {161, kParamIns1Algo, 4},
  {161, kParamIns1A, 26},   {161, kParamIns1B, 36},   {161, kParamIns1C, 16},
  {161, kParamIns1Mix, 16},   {161, kParamIns2Algo, 4},   {161, kParamIns2A, 24},
  {161, kParamIns2B, 34},   {161, kParamIns2C, 12},   {161, kParamIns2Mix, 14},
  {161, kParamMstAlgo, 2},   {161, kParamMstA, 24},   {161, kParamMstB, 18},
  {161, kParamMstC, 30},   {161, kParamMstMix, 10},
  {162, kParamEngine, 0},   {162, kParamPolyMode, 0},   {162, kParamPorta, 0},
  {162, kParamOsc1Shape, 22},   {162, kParamOsc1Level, 78},   {162, kParamOsc2Shape, 24},
  {162, kParamOsc2Pitch, 12},   {162, kParamOsc2Level, 62},   {162, kParamFmAmount, 36},
  {162, kParamSubLevel, 16},   {162, kParamFilterType, 0},   {162, kParamFilterCutoff, 8800},
  {162, kParamFilterReso, 24},   {162, kParamFilterEnv, 46},   {162, kParamFiltAttack, 2},
  {162, kParamFiltDecay, 900},   {162, kParamFiltSustain, 26.4},   {162, kParamFiltRelease, 1100},
  {162, kParamAmpAttack, 2},   {162, kParamAmpDecay, 900},   {162, kParamAmpSustain, 44},
  {162, kParamAmpRelease, 1100},   {162, kParamVelToAmp, 52},   {162, kParamVelToFilter, 28},
  {162, kParamStereoSpread, 54},   {162, kParamStereoDelay, 26},   {162, kParamTapeSat, 22},
  {162, kParamTapeMix, 30},   {162, kParamIns1Algo, 3},   {162, kParamIns1A, 34},
  {162, kParamIns1B, 30},   {162, kParamIns1C, 54},   {162, kParamIns1Mix, 18},
  {162, kParamIns2Algo, 3},   {162, kParamIns2A, 30},   {162, kParamIns2B, 28},
  {162, kParamIns2C, 44},   {162, kParamIns2Mix, 12},   {162, kParamMstAlgo, 2},
  {162, kParamMstA, 24},   {162, kParamMstB, 18},   {162, kParamMstC, 30},
  {162, kParamMstMix, 10},
  {163, kParamEngine, 0},   {163, kParamPolyMode, 0},   {163, kParamPorta, 20},
  {163, kParamOsc1Shape, 44},   {163, kParamOsc1Level, 86},   {163, kParamOsc2Shape, 42},
  {163, kParamOsc2Pitch, 7},   {163, kParamOsc2Level, 74},   {163, kParamSubLevel, 24},
  {163, kParamNoiseLevel, 0},   {163, kParamFilterType, 0},   {163, kParamFilterCutoff, 7600},
  {163, kParamFilterReso, 30},   {163, kParamFilterDrive, 0},   {163, kParamFilterEnv, 54},
  {163, kParamFiltAttack, 2},   {163, kParamFiltDecay, 600},   {163, kParamFiltSustain, 52},
  {163, kParamFiltRelease, 900},   {163, kParamAmpAttack, 4},   {163, kParamAmpDecay, 600},
  {163, kParamAmpSustain, 52},   {163, kParamAmpRelease, 900},   {163, kParamPitchAmount, -14},
  {163, kParamPitchAttack, 1},   {163, kParamPitchDecay, 90},   {163, kParamPitchSustain, 0},
  {163, kParamPitchRelease, 200},   {163, kParamUnisonVoices, 2},   {163, kParamUnisonDetune, 16},
  {163, kParamVelToAmp, 40},   {163, kParamVelToFilter, 34},   {163, kParamStereoSpread, 52},
  {163, kParamStereoDelay, 20},   {163, kParamTapeSat, 26},   {163, kParamTapeMix, 30},
  {163, kParamIns1Algo, 3},   {163, kParamIns1A, 44},   {163, kParamIns1B, 36},
  {163, kParamIns1C, 60},   {163, kParamIns1Mix, 24},   {163, kParamIns2Algo, 4},
  {163, kParamIns2A, 24},   {163, kParamIns2B, 34},   {163, kParamIns2C, 12},
  {163, kParamIns2Mix, 14},   {163, kParamMstAlgo, 2},   {163, kParamMstA, 24},
  {163, kParamMstB, 18},   {163, kParamMstC, 30},   {163, kParamMstMix, 10},
  {164, kParamEngine, 0},   {164, kParamPolyMode, 0},   {164, kParamPorta, 0},
  {164, kParamOsc1Shape, 52},   {164, kParamOsc1PW, 28},   {164, kParamOsc1Level, 88},
  {164, kParamOsc2Shape, 50},   {164, kParamOsc2Pitch, -12},   {164, kParamOsc2Level, 56},
  {164, kParamSubLevel, 30},   {164, kParamFilterType, 0},   {164, kParamFilterCutoff, 10400},
  {164, kParamFilterReso, 38},   {164, kParamFilterDrive, 0},   {164, kParamFilterEnv, 52},
  {164, kParamFiltAttack, 2},   {164, kParamFiltDecay, 600},   {164, kParamFiltSustain, 46},
  {164, kParamFiltRelease, 600},   {164, kParamAmpAttack, 2},   {164, kParamAmpDecay, 800},
  {164, kParamAmpSustain, 62},   {164, kParamAmpRelease, 600},   {164, kParamPitchAmount, -18},
  {164, kParamPitchDecay, 70},   {164, kParamUnisonVoices, 2},   {164, kParamUnisonDetune, 14},
  {164, kParamVelToAmp, 42},   {164, kParamVelToFilter, 32},   {164, kParamStereoSpread, 32},
  {164, kParamStereoDelay, 24},   {164, kParamTapeSat, 28},   {164, kParamTapeMix, 32},
  {164, kParamIns1Algo, 4},   {164, kParamIns1A, 26},   {164, kParamIns1B, 36},
  {164, kParamIns1C, 16},   {164, kParamIns1Mix, 16},   {164, kParamIns2Algo, 3},
  {164, kParamIns2A, 30},   {164, kParamIns2B, 28},   {164, kParamIns2C, 44},
  {164, kParamIns2Mix, 12},   {164, kParamMstAlgo, 2},   {164, kParamMstA, 24},
  {164, kParamMstB, 18},   {164, kParamMstC, 30},   {164, kParamMstMix, 10},
  {165, kParamEngine, 0},   {165, kParamPolyMode, 0},   {165, kParamPorta, 0},
  {165, kParamOsc1Shape, 30},   {165, kParamOsc1Level, 54},   {165, kParamOsc2Level, 0},
  {165, kParamOsc3Enable, 1},   {165, kParamOsc3Source, 1},   {165, kParamOsc3Morph, 4.4},
  {165, kParamOsc3Level, 88},   {165, kParamOsc3Pitch, 0},   {165, kParamSubLevel, 20},
  {165, kParamFilterType, 0},   {165, kParamFilterCutoff, 11400},   {165, kParamFilterReso, 20},
  {165, kParamFilterEnv, 44},   {165, kParamFiltAttack, 2},   {165, kParamFiltDecay, 900},
  {165, kParamFiltSustain, 48},   {165, kParamFiltRelease, 1400},   {165, kParamAmpAttack, 2},
  {165, kParamAmpDecay, 1000},   {165, kParamAmpSustain, 64},   {165, kParamAmpRelease, 1400},
  {165, kParamUnisonVoices, 2},   {165, kParamUnisonDetune, 13},   {165, kParamVelToAmp, 46},
  {165, kParamVelToFilter, 30},   {165, kParamStereoSpread, 44},   {165, kParamStereoDelay, 36},
  {165, kParamTapeSat, 24},   {165, kParamTapeMix, 32},   {165, kParamIns1Algo, 3},
  {165, kParamIns1A, 34},   {165, kParamIns1B, 30},   {165, kParamIns1C, 54},
  {165, kParamIns1Mix, 18},   {165, kParamIns2Algo, 4},   {165, kParamIns2A, 24},
  {165, kParamIns2B, 34},   {165, kParamIns2C, 12},   {165, kParamIns2Mix, 14},
  {165, kParamMstAlgo, 2},   {165, kParamMstA, 24},   {165, kParamMstB, 18},
  {165, kParamMstC, 30},   {165, kParamMstMix, 10},
  {166, kParamEngine, 0},   {166, kParamPolyMode, 0},   {166, kParamPorta, 14},
  {166, kParamOsc1Shape, 44},   {166, kParamOsc1Level, 86},   {166, kParamOsc2Shape, 42},
  {166, kParamOsc2Pitch, 0},   {166, kParamOsc2Level, 74},   {166, kParamSubLevel, 24},
  {166, kParamNoiseLevel, 0},   {166, kParamFilterType, 0},   {166, kParamFilterCutoff, 7000},
  {166, kParamFilterReso, 22},   {166, kParamFilterDrive, 0},   {166, kParamFilterEnv, 54},
  {166, kParamFiltAttack, 2},   {166, kParamFiltDecay, 900},   {166, kParamFiltSustain, 54},
  {166, kParamFiltRelease, 1200},   {166, kParamAmpAttack, 8},   {166, kParamAmpDecay, 900},
  {166, kParamAmpSustain, 54},   {166, kParamAmpRelease, 1200},   {166, kParamPitchAmount, -14},
  {166, kParamPitchAttack, 1},   {166, kParamPitchDecay, 90},   {166, kParamPitchSustain, 0},
  {166, kParamPitchRelease, 200},   {166, kParamUnisonVoices, 2},   {166, kParamUnisonDetune, 16},
  {166, kParamVelToAmp, 40},   {166, kParamVelToFilter, 34},   {166, kParamStereoSpread, 52},
  {166, kParamStereoDelay, 20},   {166, kParamTapeSat, 26},   {166, kParamTapeMix, 30},
  {166, kParamIns1Algo, 3},   {166, kParamIns1A, 44},   {166, kParamIns1B, 36},
  {166, kParamIns1C, 60},   {166, kParamIns1Mix, 24},   {166, kParamIns2Algo, 3},
  {166, kParamIns2A, 30},   {166, kParamIns2B, 28},   {166, kParamIns2C, 44},
  {166, kParamIns2Mix, 12},   {166, kParamMstAlgo, 2},   {166, kParamMstA, 24},
  {166, kParamMstB, 18},   {166, kParamMstC, 30},   {166, kParamMstMix, 10},
  {167, kParamEngine, 0},   {167, kParamPolyMode, 0},   {167, kParamPorta, 12},
  {167, kParamOsc1Shape, 44},   {167, kParamOsc1Level, 86},   {167, kParamOsc2Shape, 42},
  {167, kParamOsc2Pitch, 0},   {167, kParamOsc2Level, 74},   {167, kParamSubLevel, 24},
  {167, kParamNoiseLevel, 0},   {167, kParamFilterType, 0},   {167, kParamFilterCutoff, 5200},
  {167, kParamFilterReso, 44},   {167, kParamFilterDrive, 26},   {167, kParamFilterEnv, 54},
  {167, kParamFiltAttack, 2},   {167, kParamFiltDecay, 500},   {167, kParamFiltSustain, 40},
  {167, kParamFiltRelease, 700},   {167, kParamAmpAttack, 3},   {167, kParamAmpDecay, 500},
  {167, kParamAmpSustain, 40},   {167, kParamAmpRelease, 700},   {167, kParamPitchAmount, -14},
  {167, kParamPitchAttack, 1},   {167, kParamPitchDecay, 90},   {167, kParamPitchSustain, 0},
  {167, kParamPitchRelease, 200},   {167, kParamUnisonVoices, 2},   {167, kParamUnisonDetune, 16},
  {167, kParamVelToAmp, 40},   {167, kParamVelToFilter, 34},   {167, kParamStereoSpread, 52},
  {167, kParamStereoDelay, 20},   {167, kParamTapeSat, 26},   {167, kParamTapeMix, 30},
  {167, kParamIns1Algo, 4},   {167, kParamIns1A, 26},   {167, kParamIns1B, 36},
  {167, kParamIns1C, 16},   {167, kParamIns1Mix, 16},   {167, kParamIns2Algo, 4},
  {167, kParamIns2A, 24},   {167, kParamIns2B, 34},   {167, kParamIns2C, 12},
  {167, kParamIns2Mix, 14},   {167, kParamMstAlgo, 2},   {167, kParamMstA, 24},
  {167, kParamMstB, 18},   {167, kParamMstC, 30},   {167, kParamMstMix, 10},
  {168, kParamEngine, 0},   {168, kParamPolyMode, 0},   {168, kParamPorta, 0},
  {168, kParamOsc1Shape, 44},   {168, kParamOsc1Level, 86},   {168, kParamOsc2Shape, 42},
  {168, kParamOsc2Pitch, 0},   {168, kParamOsc2Level, 74},   {168, kParamSubLevel, 24},
  {168, kParamNoiseLevel, 0},   {168, kParamFilterType, 0},   {168, kParamFilterCutoff, 6200},
  {168, kParamFilterReso, 40},   {168, kParamFilterDrive, 30},   {168, kParamFilterEnv, 54},
  {168, kParamFiltAttack, 2},   {168, kParamFiltDecay, 600},   {168, kParamFiltSustain, 46},
  {168, kParamFiltRelease, 800},   {168, kParamAmpAttack, 4},   {168, kParamAmpDecay, 600},
  {168, kParamAmpSustain, 46},   {168, kParamAmpRelease, 800},   {168, kParamPitchAmount, -14},
  {168, kParamPitchAttack, 1},   {168, kParamPitchDecay, 90},   {168, kParamPitchSustain, 0},
  {168, kParamPitchRelease, 200},   {168, kParamUnisonVoices, 2},   {168, kParamUnisonDetune, 16},
  {168, kParamVelToAmp, 40},   {168, kParamVelToFilter, 34},   {168, kParamStereoSpread, 46},
  {168, kParamStereoDelay, 20},   {168, kParamTapeSat, 26},   {168, kParamTapeMix, 30},
  {168, kParamIns1Algo, 3},   {168, kParamIns1A, 34},   {168, kParamIns1B, 30},
  {168, kParamIns1C, 54},   {168, kParamIns1Mix, 18},   {168, kParamIns2Algo, 3},
  {168, kParamIns2A, 30},   {168, kParamIns2B, 28},   {168, kParamIns2C, 44},
  {168, kParamIns2Mix, 12},   {168, kParamMstAlgo, 2},   {168, kParamMstA, 24},
  {168, kParamMstB, 18},   {168, kParamMstC, 30},   {168, kParamMstMix, 10},
  {169, kParamEngine, 0},   {169, kParamPolyMode, 0},   {169, kParamPorta, 0},
  {169, kParamOsc1Shape, 46},   {169, kParamOsc1Level, 88},   {169, kParamOsc2Shape, 48},
  {169, kParamOsc2Pitch, 19},   {169, kParamOsc2Level, 80},   {169, kParamOscSync, 1},
  {169, kParamSubLevel, 18},   {169, kParamFilterType, 0},   {169, kParamFilterCutoff, 6800},
  {169, kParamFilterReso, 42},   {169, kParamFilterDrive, 18},   {169, kParamFilterEnv, 58},
  {169, kParamFiltAttack, 2},   {169, kParamFiltDecay, 700},   {169, kParamFiltSustain, 42},
  {169, kParamFiltRelease, 700},   {169, kParamAmpAttack, 2},   {169, kParamAmpDecay, 900},
  {169, kParamAmpSustain, 58},   {169, kParamAmpRelease, 700},   {169, kParamUnisonVoices, 2},
  {169, kParamUnisonDetune, 12},   {169, kParamVelToAmp, 44},   {169, kParamVelToFilter, 30},
  {169, kParamStereoSpread, 48},   {169, kParamStereoDelay, 24},   {169, kParamTapeSat, 32},
  {169, kParamTapeMix, 34},   {169, kParamIns1Algo, 3},   {169, kParamIns1A, 44},
  {169, kParamIns1B, 36},   {169, kParamIns1C, 60},   {169, kParamIns1Mix, 24},
  {169, kParamIns2Algo, 4},   {169, kParamIns2A, 24},   {169, kParamIns2B, 34},
  {169, kParamIns2C, 12},   {169, kParamIns2Mix, 14},   {169, kParamMstAlgo, 2},
  {169, kParamMstA, 24},   {169, kParamMstB, 18},   {169, kParamMstC, 30},
  {169, kParamMstMix, 10},
  {170, kParamEngine, 0},   {170, kParamPolyMode, 0},   {170, kParamPorta, 0},
  {170, kParamOsc1Shape, 52},   {170, kParamOsc1PW, 40},   {170, kParamOsc1Level, 88},
  {170, kParamOsc2Shape, 50},   {170, kParamOsc2Pitch, -12},   {170, kParamOsc2Level, 56},
  {170, kParamSubLevel, 30},   {170, kParamFilterType, 0},   {170, kParamFilterCutoff, 5800},
  {170, kParamFilterReso, 36},   {170, kParamFilterDrive, 24},   {170, kParamFilterEnv, 52},
  {170, kParamFiltAttack, 2},   {170, kParamFiltDecay, 600},   {170, kParamFiltSustain, 46},
  {170, kParamFiltRelease, 650},   {170, kParamAmpAttack, 2},   {170, kParamAmpDecay, 800},
  {170, kParamAmpSustain, 62},   {170, kParamAmpRelease, 650},   {170, kParamPitchAmount, -10},
  {170, kParamPitchDecay, 70},   {170, kParamUnisonVoices, 2},   {170, kParamUnisonDetune, 14},
  {170, kParamVelToAmp, 42},   {170, kParamVelToFilter, 32},   {170, kParamStereoSpread, 32},
  {170, kParamStereoDelay, 24},   {170, kParamTapeSat, 28},   {170, kParamTapeMix, 32},
  {170, kParamIns1Algo, 4},   {170, kParamIns1A, 26},   {170, kParamIns1B, 36},
  {170, kParamIns1C, 16},   {170, kParamIns1Mix, 16},   {170, kParamIns2Algo, 3},
  {170, kParamIns2A, 30},   {170, kParamIns2B, 28},   {170, kParamIns2C, 44},
  {170, kParamIns2Mix, 12},   {170, kParamMstAlgo, 2},   {170, kParamMstA, 24},
  {170, kParamMstB, 18},   {170, kParamMstC, 30},   {170, kParamMstMix, 10},
  {171, kParamEngine, 0},   {171, kParamPolyMode, 0},   {171, kParamPorta, 0},
  {171, kParamOsc1Shape, 44},   {171, kParamOsc1Level, 86},   {171, kParamOsc2Shape, 42},
  {171, kParamOsc2Pitch, 0},   {171, kParamOsc2Level, 74},   {171, kParamSubLevel, 24},
  {171, kParamNoiseLevel, 0},   {171, kParamFilterType, 0},   {171, kParamFilterCutoff, 7400},
  {171, kParamFilterReso, 30},   {171, kParamFilterDrive, 18},   {171, kParamFilterEnv, 54},
  {171, kParamFiltAttack, 2},   {171, kParamFiltDecay, 700},   {171, kParamFiltSustain, 50},
  {171, kParamFiltRelease, 1000},   {171, kParamAmpAttack, 5},   {171, kParamAmpDecay, 700},
  {171, kParamAmpSustain, 50},   {171, kParamAmpRelease, 1000},   {171, kParamPitchAmount, -14},
  {171, kParamPitchAttack, 1},   {171, kParamPitchDecay, 90},   {171, kParamPitchSustain, 0},
  {171, kParamPitchRelease, 200},   {171, kParamUnisonVoices, 2},   {171, kParamUnisonDetune, 16},
  {171, kParamVelToAmp, 40},   {171, kParamVelToFilter, 34},   {171, kParamStereoSpread, 52},
  {171, kParamStereoDelay, 20},   {171, kParamTapeSat, 26},   {171, kParamTapeMix, 30},
  {171, kParamIns1Algo, 3},   {171, kParamIns1A, 34},   {171, kParamIns1B, 30},
  {171, kParamIns1C, 54},   {171, kParamIns1Mix, 18},   {171, kParamIns2Algo, 4},
  {171, kParamIns2A, 24},   {171, kParamIns2B, 34},   {171, kParamIns2C, 12},
  {171, kParamIns2Mix, 14},   {171, kParamMstAlgo, 2},   {171, kParamMstA, 24},
  {171, kParamMstB, 18},   {171, kParamMstC, 30},   {171, kParamMstMix, 10},
  {172, kParamEngine, 0},   {172, kParamPolyMode, 0},   {172, kParamPorta, 0},
  {172, kParamOsc1Shape, 30},   {172, kParamOsc1Level, 54},   {172, kParamOsc2Level, 0},
  {172, kParamOsc3Enable, 1},   {172, kParamOsc3Source, 1},   {172, kParamOsc3Morph, 3.6},
  {172, kParamOsc3Level, 88},   {172, kParamOsc3Pitch, 12},   {172, kParamSubLevel, 20},
  {172, kParamFilterType, 0},   {172, kParamFilterCutoff, 8000},   {172, kParamFilterReso, 28},
  {172, kParamFilterEnv, 44},   {172, kParamFiltAttack, 2},   {172, kParamFiltDecay, 900},
  {172, kParamFiltSustain, 48},   {172, kParamFiltRelease, 1100},   {172, kParamAmpAttack, 6},
  {172, kParamAmpDecay, 1000},   {172, kParamAmpSustain, 64},   {172, kParamAmpRelease, 1100},
  {172, kParamUnisonVoices, 2},   {172, kParamUnisonDetune, 13},   {172, kParamVelToAmp, 46},
  {172, kParamVelToFilter, 30},   {172, kParamStereoSpread, 44},   {172, kParamStereoDelay, 30},
  {172, kParamTapeSat, 24},   {172, kParamTapeMix, 32},   {172, kParamIns1Algo, 3},
  {172, kParamIns1A, 44},   {172, kParamIns1B, 36},   {172, kParamIns1C, 60},
  {172, kParamIns1Mix, 24},   {172, kParamIns2Algo, 3},   {172, kParamIns2A, 30},
  {172, kParamIns2B, 28},   {172, kParamIns2C, 44},   {172, kParamIns2Mix, 12},
  {172, kParamMstAlgo, 2},   {172, kParamMstA, 24},   {172, kParamMstB, 18},
  {172, kParamMstC, 30},   {172, kParamMstMix, 10},
  {173, kParamEngine, 0},   {173, kParamPolyMode, 0},   {173, kParamPorta, 0},
  {173, kParamOsc1Shape, 22},   {173, kParamOsc1Level, 78},   {173, kParamOsc2Shape, 24},
  {173, kParamOsc2Pitch, 12},   {173, kParamOsc2Level, 62},   {173, kParamFmAmount, 48},
  {173, kParamSubLevel, 16},   {173, kParamFilterType, 0},   {173, kParamFilterCutoff, 9200},
  {173, kParamFilterReso, 22},   {173, kParamFilterEnv, 46},   {173, kParamFiltAttack, 2},
  {173, kParamFiltDecay, 800},   {173, kParamFiltSustain, 22.8},   {173, kParamFiltRelease, 1100},
  {173, kParamAmpAttack, 2},   {173, kParamAmpDecay, 800},   {173, kParamAmpSustain, 38},
  {173, kParamAmpRelease, 1100},   {173, kParamVelToAmp, 52},   {173, kParamVelToFilter, 28},
  {173, kParamStereoSpread, 54},   {173, kParamStereoDelay, 26},   {173, kParamTapeSat, 22},
  {173, kParamTapeMix, 30},   {173, kParamIns1Algo, 4},   {173, kParamIns1A, 26},
  {173, kParamIns1B, 36},   {173, kParamIns1C, 16},   {173, kParamIns1Mix, 16},
  {173, kParamIns2Algo, 4},   {173, kParamIns2A, 24},   {173, kParamIns2B, 34},
  {173, kParamIns2C, 12},   {173, kParamIns2Mix, 14},   {173, kParamMstAlgo, 2},
  {173, kParamMstA, 24},   {173, kParamMstB, 18},   {173, kParamMstC, 30},
  {173, kParamMstMix, 10},
  {174, kParamEngine, 0},   {174, kParamPolyMode, 0},   {174, kParamPorta, 0},
  {174, kParamOsc1Shape, 44},   {174, kParamOsc1Level, 86},   {174, kParamOsc2Shape, 42},
  {174, kParamOsc2Pitch, 0},   {174, kParamOsc2Level, 74},   {174, kParamSubLevel, 24},
  {174, kParamNoiseLevel, 0},   {174, kParamFilterType, 0},   {174, kParamFilterCutoff, 4200},
  {174, kParamFilterReso, 48},   {174, kParamFilterDrive, 0},   {174, kParamFilterEnv, 54},
  {174, kParamFiltAttack, 2},   {174, kParamFiltDecay, 450},   {174, kParamFiltSustain, 44},
  {174, kParamFiltRelease, 700},   {174, kParamAmpAttack, 3},   {174, kParamAmpDecay, 450},
  {174, kParamAmpSustain, 44},   {174, kParamAmpRelease, 700},   {174, kParamPitchAmount, -14},
  {174, kParamPitchAttack, 1},   {174, kParamPitchDecay, 90},   {174, kParamPitchSustain, 0},
  {174, kParamPitchRelease, 200},   {174, kParamUnisonVoices, 2},   {174, kParamUnisonDetune, 16},
  {174, kParamVelToAmp, 40},   {174, kParamVelToFilter, 34},   {174, kParamStereoSpread, 52},
  {174, kParamStereoDelay, 20},   {174, kParamTapeSat, 26},   {174, kParamTapeMix, 30},
  {174, kParamLfoRate, 1.4},   {174, kParamLfoDest, 1},   {174, kParamLfoAmount, 34},
  {174, kParamIns1Algo, 3},   {174, kParamIns1A, 34},   {174, kParamIns1B, 30},
  {174, kParamIns1C, 54},   {174, kParamIns1Mix, 18},   {174, kParamIns2Algo, 3},
  {174, kParamIns2A, 30},   {174, kParamIns2B, 28},   {174, kParamIns2C, 44},
  {174, kParamIns2Mix, 12},   {174, kParamMstAlgo, 2},   {174, kParamMstA, 24},
  {174, kParamMstB, 18},   {174, kParamMstC, 30},   {174, kParamMstMix, 10},
  {175, kParamEngine, 0},   {175, kParamPolyMode, 0},   {175, kParamPorta, 0},
  {175, kParamOsc1Shape, 30},   {175, kParamOsc1Level, 54},   {175, kParamOsc2Level, 0},
  {175, kParamOsc3Enable, 1},   {175, kParamOsc3Source, 1},   {175, kParamOsc3Morph, 5.8},
  {175, kParamOsc3Level, 88},   {175, kParamOsc3Pitch, 19},   {175, kParamSubLevel, 20},
  {175, kParamFilterType, 0},   {175, kParamFilterCutoff, 11000},   {175, kParamFilterReso, 18},
  {175, kParamFilterEnv, 44},   {175, kParamFiltAttack, 2},   {175, kParamFiltDecay, 900},
  {175, kParamFiltSustain, 48},   {175, kParamFiltRelease, 1600},   {175, kParamAmpAttack, 2},
  {175, kParamAmpDecay, 1000},   {175, kParamAmpSustain, 64},   {175, kParamAmpRelease, 1600},
  {175, kParamUnisonVoices, 2},   {175, kParamUnisonDetune, 13},   {175, kParamVelToAmp, 46},
  {175, kParamVelToFilter, 30},   {175, kParamStereoSpread, 44},   {175, kParamStereoDelay, 38},
  {175, kParamTapeSat, 24},   {175, kParamTapeMix, 32},   {175, kParamIns1Algo, 3},
  {175, kParamIns1A, 44},   {175, kParamIns1B, 36},   {175, kParamIns1C, 60},
  {175, kParamIns1Mix, 24},   {175, kParamIns2Algo, 4},   {175, kParamIns2A, 24},
  {175, kParamIns2B, 34},   {175, kParamIns2C, 12},   {175, kParamIns2Mix, 14},
  {175, kParamMstAlgo, 2},   {175, kParamMstA, 24},   {175, kParamMstB, 18},
  {175, kParamMstC, 30},   {175, kParamMstMix, 10},
  {176, kParamEngine, 0},   {176, kParamPolyMode, 0},   {176, kParamPorta, 0},
  {176, kParamOsc1Shape, 22},   {176, kParamOsc1Level, 78},   {176, kParamOsc2Shape, 24},
  {176, kParamOsc2Pitch, 12},   {176, kParamOsc2Level, 62},   {176, kParamFmAmount, 62},
  {176, kParamSubLevel, 16},   {176, kParamFilterType, 0},   {176, kParamFilterCutoff, 9800},
  {176, kParamFilterReso, 26},   {176, kParamFilterEnv, 46},   {176, kParamFiltAttack, 2},
  {176, kParamFiltDecay, 600},   {176, kParamFiltSustain, 18},   {176, kParamFiltRelease, 800},
  {176, kParamAmpAttack, 1},   {176, kParamAmpDecay, 600},   {176, kParamAmpSustain, 30},
  {176, kParamAmpRelease, 800},   {176, kParamVelToAmp, 52},   {176, kParamVelToFilter, 28},
  {176, kParamStereoSpread, 54},   {176, kParamStereoDelay, 26},   {176, kParamTapeSat, 22},
  {176, kParamTapeMix, 30},   {176, kParamIns1Algo, 4},   {176, kParamIns1A, 26},
  {176, kParamIns1B, 36},   {176, kParamIns1C, 16},   {176, kParamIns1Mix, 16},
  {176, kParamIns2Algo, 3},   {176, kParamIns2A, 30},   {176, kParamIns2B, 28},
  {176, kParamIns2C, 44},   {176, kParamIns2Mix, 12},   {176, kParamMstAlgo, 2},
  {176, kParamMstA, 24},   {176, kParamMstB, 18},   {176, kParamMstC, 30},
  {176, kParamMstMix, 10},
  {177, kParamEngine, 0},   {177, kParamPolyMode, 0},   {177, kParamPorta, 0},
  {177, kParamOsc1Shape, 52},   {177, kParamOsc1PW, 22},   {177, kParamOsc1Level, 88},
  {177, kParamOsc2Shape, 50},   {177, kParamOsc2Pitch, -12},   {177, kParamOsc2Level, 56},
  {177, kParamSubLevel, 30},   {177, kParamFilterType, 0},   {177, kParamFilterCutoff, 8800},
  {177, kParamFilterReso, 34},   {177, kParamFilterDrive, 0},   {177, kParamFilterEnv, 52},
  {177, kParamFiltAttack, 2},   {177, kParamFiltDecay, 600},   {177, kParamFiltSustain, 46},
  {177, kParamFiltRelease, 500},   {177, kParamAmpAttack, 1},   {177, kParamAmpDecay, 800},
  {177, kParamAmpSustain, 62},   {177, kParamAmpRelease, 500},   {177, kParamPitchAmount, -10},
  {177, kParamPitchDecay, 70},   {177, kParamUnisonVoices, 2},   {177, kParamUnisonDetune, 14},
  {177, kParamVelToAmp, 42},   {177, kParamVelToFilter, 32},   {177, kParamStereoSpread, 32},
  {177, kParamStereoDelay, 0},   {177, kParamTapeSat, 28},   {177, kParamTapeMix, 32},
  {177, kParamIns1Algo, 3},   {177, kParamIns1A, 34},   {177, kParamIns1B, 30},
  {177, kParamIns1C, 54},   {177, kParamIns1Mix, 18},   {177, kParamIns2Algo, 4},
  {177, kParamIns2A, 24},   {177, kParamIns2B, 34},   {177, kParamIns2C, 12},
  {177, kParamIns2Mix, 14},   {177, kParamMstAlgo, 2},   {177, kParamMstA, 24},
  {177, kParamMstB, 18},   {177, kParamMstC, 30},   {177, kParamMstMix, 10},
  {178, kParamEngine, 0},   {178, kParamPolyMode, 0},   {178, kParamPorta, 0},
  {178, kParamOsc1Shape, 44},   {178, kParamOsc1Level, 86},   {178, kParamOsc2Shape, 42},
  {178, kParamOsc2Pitch, 0},   {178, kParamOsc2Level, 74},   {178, kParamSubLevel, 24},
  {178, kParamNoiseLevel, 0},   {178, kParamFilterType, 0},   {178, kParamFilterCutoff, 9400},
  {178, kParamFilterReso, 28},   {178, kParamFilterDrive, 0},   {178, kParamFilterEnv, 54},
  {178, kParamFiltAttack, 2},   {178, kParamFiltDecay, 400},   {178, kParamFiltSustain, 20},
  {178, kParamFiltRelease, 500},   {178, kParamAmpAttack, 1},   {178, kParamAmpDecay, 400},
  {178, kParamAmpSustain, 20},   {178, kParamAmpRelease, 500},   {178, kParamPitchAmount, -14},
  {178, kParamPitchAttack, 1},   {178, kParamPitchDecay, 90},   {178, kParamPitchSustain, 0},
  {178, kParamPitchRelease, 200},   {178, kParamUnisonVoices, 2},   {178, kParamUnisonDetune, 16},
  {178, kParamVelToAmp, 40},   {178, kParamVelToFilter, 34},   {178, kParamStereoSpread, 52},
  {178, kParamStereoDelay, 20},   {178, kParamTapeSat, 26},   {178, kParamTapeMix, 30},
  {178, kParamIns1Algo, 3},   {178, kParamIns1A, 44},   {178, kParamIns1B, 36},
  {178, kParamIns1C, 60},   {178, kParamIns1Mix, 24},   {178, kParamIns2Algo, 3},
  {178, kParamIns2A, 30},   {178, kParamIns2B, 28},   {178, kParamIns2C, 44},
  {178, kParamIns2Mix, 12},   {178, kParamMstAlgo, 2},   {178, kParamMstA, 24},
  {178, kParamMstB, 18},   {178, kParamMstC, 30},   {178, kParamMstMix, 10},
  {179, kParamEngine, 0},   {179, kParamPolyMode, 0},   {179, kParamPorta, 0},
  {179, kParamOsc1Shape, 30},   {179, kParamOsc1Level, 54},   {179, kParamOsc2Level, 0},
  {179, kParamOsc3Enable, 1},   {179, kParamOsc3Source, 1},   {179, kParamOsc3Morph, 6.2},
  {179, kParamOsc3Level, 88},   {179, kParamOsc3Pitch, 0},   {179, kParamSubLevel, 20},
  {179, kParamFilterType, 0},   {179, kParamFilterCutoff, 9600},   {179, kParamFilterReso, 22},
  {179, kParamFilterEnv, 44},   {179, kParamFiltAttack, 2},   {179, kParamFiltDecay, 900},
  {179, kParamFiltSustain, 48},   {179, kParamFiltRelease, 1300},   {179, kParamAmpAttack, 2},
  {179, kParamAmpDecay, 1000},   {179, kParamAmpSustain, 64},   {179, kParamAmpRelease, 1300},
  {179, kParamUnisonVoices, 2},   {179, kParamUnisonDetune, 13},   {179, kParamVelToAmp, 46},
  {179, kParamVelToFilter, 30},   {179, kParamStereoSpread, 44},   {179, kParamStereoDelay, 30},
  {179, kParamTapeSat, 24},   {179, kParamTapeMix, 32},   {179, kParamIns1Algo, 4},
  {179, kParamIns1A, 26},   {179, kParamIns1B, 36},   {179, kParamIns1C, 16},
  {179, kParamIns1Mix, 16},   {179, kParamIns2Algo, 4},   {179, kParamIns2A, 24},
  {179, kParamIns2B, 34},   {179, kParamIns2C, 12},   {179, kParamIns2Mix, 14},
  {179, kParamMstAlgo, 2},   {179, kParamMstA, 24},   {179, kParamMstB, 18},
  {179, kParamMstC, 30},   {179, kParamMstMix, 10},
  {180, kParamEngine, 0},   {180, kParamPolyMode, 0},   {180, kParamPorta, 0},
  {180, kParamOsc1Shape, 22},   {180, kParamOsc1Level, 78},   {180, kParamOsc2Shape, 24},
  {180, kParamOsc2Pitch, 12},   {180, kParamOsc2Level, 62},   {180, kParamFmAmount, 40},
  {180, kParamSubLevel, 16},   {180, kParamFilterType, 0},   {180, kParamFilterCutoff, 8400},
  {180, kParamFilterReso, 18},   {180, kParamFilterEnv, 46},   {180, kParamFiltAttack, 2},
  {180, kParamFiltDecay, 500},   {180, kParamFiltSustain, 8.4},   {180, kParamFiltRelease, 600},
  {180, kParamAmpAttack, 1},   {180, kParamAmpDecay, 500},   {180, kParamAmpSustain, 14},
  {180, kParamAmpRelease, 600},   {180, kParamVelToAmp, 52},   {180, kParamVelToFilter, 28},
  {180, kParamStereoSpread, 54},   {180, kParamStereoDelay, 26},   {180, kParamTapeSat, 22},
  {180, kParamTapeMix, 30},   {180, kParamIns1Algo, 3},   {180, kParamIns1A, 34},
  {180, kParamIns1B, 30},   {180, kParamIns1C, 54},   {180, kParamIns1Mix, 18},
  {180, kParamIns2Algo, 3},   {180, kParamIns2A, 30},   {180, kParamIns2B, 28},
  {180, kParamIns2C, 44},   {180, kParamIns2Mix, 12},   {180, kParamMstAlgo, 2},
  {180, kParamMstA, 24},   {180, kParamMstB, 18},   {180, kParamMstC, 30},
  {180, kParamMstMix, 10},
  {181, kParamEngine, 0},   {181, kParamPolyMode, 0},   {181, kParamPorta, 0},
  {181, kParamOsc1Shape, 46},   {181, kParamOsc1Level, 88},   {181, kParamOsc2Shape, 48},
  {181, kParamOsc2Pitch, 14},   {181, kParamOsc2Level, 80},   {181, kParamOscSync, 1},
  {181, kParamSubLevel, 18},   {181, kParamFilterType, 0},   {181, kParamFilterCutoff, 9000},
  {181, kParamFilterReso, 36},   {181, kParamFilterDrive, 18},   {181, kParamFilterEnv, 58},
  {181, kParamFiltAttack, 2},   {181, kParamFiltDecay, 700},   {181, kParamFiltSustain, 42},
  {181, kParamFiltRelease, 500},   {181, kParamAmpAttack, 1},   {181, kParamAmpDecay, 900},
  {181, kParamAmpSustain, 58},   {181, kParamAmpRelease, 500},   {181, kParamUnisonVoices, 2},
  {181, kParamUnisonDetune, 12},   {181, kParamVelToAmp, 44},   {181, kParamVelToFilter, 30},
  {181, kParamStereoSpread, 48},   {181, kParamStereoDelay, 24},   {181, kParamTapeSat, 32},
  {181, kParamTapeMix, 34},   {181, kParamIns1Algo, 3},   {181, kParamIns1A, 44},
  {181, kParamIns1B, 36},   {181, kParamIns1C, 60},   {181, kParamIns1Mix, 24},
  {181, kParamIns2Algo, 4},   {181, kParamIns2A, 24},   {181, kParamIns2B, 34},
  {181, kParamIns2C, 12},   {181, kParamIns2Mix, 14},   {181, kParamMstAlgo, 2},
  {181, kParamMstA, 24},   {181, kParamMstB, 18},   {181, kParamMstC, 30},
  {181, kParamMstMix, 10},
  {182, kParamEngine, 0},   {182, kParamPolyMode, 0},   {182, kParamPorta, 0},
  {182, kParamOsc1Shape, 44},   {182, kParamOsc1Level, 86},   {182, kParamOsc2Shape, 42},
  {182, kParamOsc2Pitch, 0},   {182, kParamOsc2Level, 74},   {182, kParamSubLevel, 24},
  {182, kParamNoiseLevel, 0},   {182, kParamFilterType, 0},   {182, kParamFilterCutoff, 7200},
  {182, kParamFilterReso, 46},   {182, kParamFilterDrive, 64},   {182, kParamFilterEnv, 54},
  {182, kParamFiltAttack, 2},   {182, kParamFiltDecay, 600},   {182, kParamFiltSustain, 50},
  {182, kParamFiltRelease, 700},   {182, kParamAmpAttack, 2},   {182, kParamAmpDecay, 600},
  {182, kParamAmpSustain, 50},   {182, kParamAmpRelease, 700},   {182, kParamPitchAmount, -14},
  {182, kParamPitchAttack, 1},   {182, kParamPitchDecay, 90},   {182, kParamPitchSustain, 0},
  {182, kParamPitchRelease, 200},   {182, kParamUnisonVoices, 2},   {182, kParamUnisonDetune, 16},
  {182, kParamVelToAmp, 40},   {182, kParamVelToFilter, 34},   {182, kParamStereoSpread, 52},
  {182, kParamStereoDelay, 20},   {182, kParamTapeSat, 46},   {182, kParamTapeMix, 48},
  {182, kParamIns1Algo, 4},   {182, kParamIns1A, 26},   {182, kParamIns1B, 36},
  {182, kParamIns1C, 16},   {182, kParamIns1Mix, 16},   {182, kParamIns2Algo, 3},
  {182, kParamIns2A, 30},   {182, kParamIns2B, 28},   {182, kParamIns2C, 44},
  {182, kParamIns2Mix, 12},   {182, kParamMstAlgo, 2},   {182, kParamMstA, 24},
  {182, kParamMstB, 18},   {182, kParamMstC, 30},   {182, kParamMstMix, 10},
  {183, kParamEngine, 0},   {183, kParamPolyMode, 0},   {183, kParamPorta, 0},
  {183, kParamOsc1Shape, 44},   {183, kParamOsc1Level, 86},   {183, kParamOsc2Shape, 42},
  {183, kParamOsc2Pitch, 7},   {183, kParamOsc2Level, 74},   {183, kParamSubLevel, 24},
  {183, kParamNoiseLevel, 0},   {183, kParamFilterType, 0},   {183, kParamFilterCutoff, 6400},
  {183, kParamFilterReso, 50},   {183, kParamFilterDrive, 70},   {183, kParamFilterEnv, 54},
  {183, kParamFiltAttack, 2},   {183, kParamFiltDecay, 550},   {183, kParamFiltSustain, 46},
  {183, kParamFiltRelease, 650},   {183, kParamAmpAttack, 2},   {183, kParamAmpDecay, 550},
  {183, kParamAmpSustain, 46},   {183, kParamAmpRelease, 650},   {183, kParamPitchAmount, -14},
  {183, kParamPitchAttack, 1},   {183, kParamPitchDecay, 90},   {183, kParamPitchSustain, 0},
  {183, kParamPitchRelease, 200},   {183, kParamUnisonVoices, 2},   {183, kParamUnisonDetune, 16},
  {183, kParamVelToAmp, 40},   {183, kParamVelToFilter, 34},   {183, kParamStereoSpread, 52},
  {183, kParamStereoDelay, 20},   {183, kParamTapeSat, 44},   {183, kParamTapeMix, 30},
  {183, kParamIns1Algo, 3},   {183, kParamIns1A, 34},   {183, kParamIns1B, 30},
  {183, kParamIns1C, 54},   {183, kParamIns1Mix, 18},   {183, kParamIns2Algo, 4},
  {183, kParamIns2A, 24},   {183, kParamIns2B, 34},   {183, kParamIns2C, 12},
  {183, kParamIns2Mix, 14},   {183, kParamMstAlgo, 2},   {183, kParamMstA, 24},
  {183, kParamMstB, 18},   {183, kParamMstC, 30},   {183, kParamMstMix, 10},
  {184, kParamEngine, 0},   {184, kParamPolyMode, 0},   {184, kParamPorta, 0},
  {184, kParamOsc1Shape, 52},   {184, kParamOsc1PW, 18},   {184, kParamOsc1Level, 88},
  {184, kParamOsc2Shape, 50},   {184, kParamOsc2Pitch, -12},   {184, kParamOsc2Level, 56},
  {184, kParamSubLevel, 30},   {184, kParamFilterType, 0},   {184, kParamFilterCutoff, 7800},
  {184, kParamFilterReso, 44},   {184, kParamFilterDrive, 60},   {184, kParamFilterEnv, 52},
  {184, kParamFiltAttack, 2},   {184, kParamFiltDecay, 600},   {184, kParamFiltSustain, 46},
  {184, kParamFiltRelease, 600},   {184, kParamAmpAttack, 1},   {184, kParamAmpDecay, 800},
  {184, kParamAmpSustain, 62},   {184, kParamAmpRelease, 600},   {184, kParamPitchAmount, -10},
  {184, kParamPitchDecay, 70},   {184, kParamUnisonVoices, 2},   {184, kParamUnisonDetune, 14},
  {184, kParamVelToAmp, 42},   {184, kParamVelToFilter, 32},   {184, kParamStereoSpread, 32},
  {184, kParamStereoDelay, 24},   {184, kParamTapeSat, 28},   {184, kParamTapeMix, 32},
  {184, kParamIns1Algo, 3},   {184, kParamIns1A, 44},   {184, kParamIns1B, 36},
  {184, kParamIns1C, 60},   {184, kParamIns1Mix, 24},   {184, kParamIns2Algo, 3},
  {184, kParamIns2A, 30},   {184, kParamIns2B, 28},   {184, kParamIns2C, 44},
  {184, kParamIns2Mix, 12},   {184, kParamMstAlgo, 2},   {184, kParamMstA, 24},
  {184, kParamMstB, 18},   {184, kParamMstC, 30},   {184, kParamMstMix, 10},
  {185, kParamEngine, 0},   {185, kParamPolyMode, 0},   {185, kParamPorta, 0},
  {185, kParamOsc1Shape, 46},   {185, kParamOsc1Level, 88},   {185, kParamOsc2Shape, 48},
  {185, kParamOsc2Pitch, 24},   {185, kParamOsc2Level, 80},   {185, kParamOscSync, 1},
  {185, kParamSubLevel, 18},   {185, kParamFilterType, 0},   {185, kParamFilterCutoff, 8200},
  {185, kParamFilterReso, 48},   {185, kParamFilterDrive, 56},   {185, kParamFilterEnv, 58},
  {185, kParamFiltAttack, 2},   {185, kParamFiltDecay, 700},   {185, kParamFiltSustain, 42},
  {185, kParamFiltRelease, 600},   {185, kParamAmpAttack, 1},   {185, kParamAmpDecay, 900},
  {185, kParamAmpSustain, 58},   {185, kParamAmpRelease, 600},   {185, kParamUnisonVoices, 2},
  {185, kParamUnisonDetune, 12},   {185, kParamVelToAmp, 44},   {185, kParamVelToFilter, 30},
  {185, kParamStereoSpread, 48},   {185, kParamStereoDelay, 24},   {185, kParamTapeSat, 32},
  {185, kParamTapeMix, 34},   {185, kParamIns1Algo, 4},   {185, kParamIns1A, 26},
  {185, kParamIns1B, 36},   {185, kParamIns1C, 16},   {185, kParamIns1Mix, 16},
  {185, kParamIns2Algo, 4},   {185, kParamIns2A, 24},   {185, kParamIns2B, 34},
  {185, kParamIns2C, 12},   {185, kParamIns2Mix, 14},   {185, kParamMstAlgo, 2},
  {185, kParamMstA, 24},   {185, kParamMstB, 18},   {185, kParamMstC, 30},
  {185, kParamMstMix, 10},
  {186, kParamEngine, 0},   {186, kParamPolyMode, 0},   {186, kParamPorta, 0},
  {186, kParamOsc1Shape, 44},   {186, kParamOsc1Level, 86},   {186, kParamOsc2Shape, 42},
  {186, kParamOsc2Pitch, 0},   {186, kParamOsc2Level, 74},   {186, kParamSubLevel, 24},
  {186, kParamNoiseLevel, 0},   {186, kParamFilterType, 0},   {186, kParamFilterCutoff, 6000},
  {186, kParamFilterReso, 52},   {186, kParamFilterDrive, 58},   {186, kParamFilterEnv, 54},
  {186, kParamFiltAttack, 2},   {186, kParamFiltDecay, 500},   {186, kParamFiltSustain, 48},
  {186, kParamFiltRelease, 600},   {186, kParamAmpAttack, 2},   {186, kParamAmpDecay, 500},
  {186, kParamAmpSustain, 48},   {186, kParamAmpRelease, 600},   {186, kParamPitchAmount, -14},
  {186, kParamPitchAttack, 1},   {186, kParamPitchDecay, 90},   {186, kParamPitchSustain, 0},
  {186, kParamPitchRelease, 200},   {186, kParamUnisonVoices, 4},   {186, kParamUnisonDetune, 22},
  {186, kParamVelToAmp, 40},   {186, kParamVelToFilter, 34},   {186, kParamStereoSpread, 52},
  {186, kParamStereoDelay, 20},   {186, kParamTapeSat, 26},   {186, kParamTapeMix, 30},
  {186, kParamIns1Algo, 3},   {186, kParamIns1A, 34},   {186, kParamIns1B, 30},
  {186, kParamIns1C, 54},   {186, kParamIns1Mix, 18},   {186, kParamIns2Algo, 3},
  {186, kParamIns2A, 30},   {186, kParamIns2B, 28},   {186, kParamIns2C, 44},
  {186, kParamIns2Mix, 12},   {186, kParamMstAlgo, 2},   {186, kParamMstA, 24},
  {186, kParamMstB, 18},   {186, kParamMstC, 30},   {186, kParamMstMix, 10},
  {187, kParamEngine, 0},   {187, kParamPolyMode, 0},   {187, kParamPorta, 0},
  {187, kParamOsc1Shape, 22},   {187, kParamOsc1Level, 78},   {187, kParamOsc2Shape, 24},
  {187, kParamOsc2Pitch, 12},   {187, kParamOsc2Level, 62},   {187, kParamFmAmount, 58},
  {187, kParamSubLevel, 16},   {187, kParamFilterType, 0},   {187, kParamFilterCutoff, 9000},
  {187, kParamFilterReso, 42},   {187, kParamFilterEnv, 46},   {187, kParamFiltAttack, 2},
  {187, kParamFiltDecay, 550},   {187, kParamFiltSustain, 20.4},   {187, kParamFiltRelease, 700},
  {187, kParamAmpAttack, 1},   {187, kParamAmpDecay, 550},   {187, kParamAmpSustain, 34},
  {187, kParamAmpRelease, 700},   {187, kParamVelToAmp, 52},   {187, kParamVelToFilter, 28},
  {187, kParamStereoSpread, 54},   {187, kParamStereoDelay, 26},   {187, kParamTapeSat, 22},
  {187, kParamTapeMix, 30},   {187, kParamFilterDrive, 48},   {187, kParamIns1Algo, 3},
  {187, kParamIns1A, 44},   {187, kParamIns1B, 36},   {187, kParamIns1C, 60},
  {187, kParamIns1Mix, 24},   {187, kParamIns2Algo, 4},   {187, kParamIns2A, 24},
  {187, kParamIns2B, 34},   {187, kParamIns2C, 12},   {187, kParamIns2Mix, 14},
  {187, kParamMstAlgo, 2},   {187, kParamMstA, 24},   {187, kParamMstB, 18},
  {187, kParamMstC, 30},   {187, kParamMstMix, 10},
  {188, kParamEngine, 0},   {188, kParamPolyMode, 0},   {188, kParamPorta, 0},
  {188, kParamOsc1Shape, 30},   {188, kParamOsc1Level, 54},   {188, kParamOsc2Level, 0},
  {188, kParamOsc3Enable, 1},   {188, kParamOsc3Source, 1},   {188, kParamOsc3Morph, 6.6},
  {188, kParamOsc3Level, 88},   {188, kParamOsc3Pitch, 0},   {188, kParamSubLevel, 20},
  {188, kParamFilterType, 0},   {188, kParamFilterCutoff, 7000},   {188, kParamFilterReso, 44},
  {188, kParamFilterEnv, 44},   {188, kParamFiltAttack, 2},   {188, kParamFiltDecay, 900},
  {188, kParamFiltSustain, 48},   {188, kParamFiltRelease, 800},   {188, kParamAmpAttack, 2},
  {188, kParamAmpDecay, 1000},   {188, kParamAmpSustain, 64},   {188, kParamAmpRelease, 800},
  {188, kParamUnisonVoices, 2},   {188, kParamUnisonDetune, 13},   {188, kParamVelToAmp, 46},
  {188, kParamVelToFilter, 30},   {188, kParamStereoSpread, 44},   {188, kParamStereoDelay, 30},
  {188, kParamTapeSat, 24},   {188, kParamTapeMix, 32},   {188, kParamFilterDrive, 52},
  {188, kParamIns1Algo, 4},   {188, kParamIns1A, 26},   {188, kParamIns1B, 36},
  {188, kParamIns1C, 16},   {188, kParamIns1Mix, 16},   {188, kParamIns2Algo, 3},
  {188, kParamIns2A, 30},   {188, kParamIns2B, 28},   {188, kParamIns2C, 44},
  {188, kParamIns2Mix, 12},   {188, kParamMstAlgo, 2},   {188, kParamMstA, 24},
  {188, kParamMstB, 18},   {188, kParamMstC, 30},   {188, kParamMstMix, 10},
  {189, kParamEngine, 0},   {189, kParamPolyMode, 0},   {189, kParamPorta, 22},
  {189, kParamOsc1Shape, 44},   {189, kParamOsc1Level, 86},   {189, kParamOsc2Shape, 42},
  {189, kParamOsc2Pitch, 0},   {189, kParamOsc2Level, 74},   {189, kParamSubLevel, 24},
  {189, kParamNoiseLevel, 0},   {189, kParamFilterType, 0},   {189, kParamFilterCutoff, 6600},
  {189, kParamFilterReso, 20},   {189, kParamFilterDrive, 0},   {189, kParamFilterEnv, 54},
  {189, kParamFiltAttack, 2},   {189, kParamFiltDecay, 900},   {189, kParamFiltSustain, 58},
  {189, kParamFiltRelease, 1400},   {189, kParamAmpAttack, 10},   {189, kParamAmpDecay, 900},
  {189, kParamAmpSustain, 58},   {189, kParamAmpRelease, 1400},   {189, kParamPitchAmount, -14},
  {189, kParamPitchAttack, 1},   {189, kParamPitchDecay, 90},   {189, kParamPitchSustain, 0},
  {189, kParamPitchRelease, 200},   {189, kParamUnisonVoices, 2},   {189, kParamUnisonDetune, 16},
  {189, kParamVelToAmp, 40},   {189, kParamVelToFilter, 34},   {189, kParamStereoSpread, 52},
  {189, kParamStereoDelay, 20},   {189, kParamTapeSat, 26},   {189, kParamTapeMix, 30},
  {189, kParamIns1Algo, 3},   {189, kParamIns1A, 34},   {189, kParamIns1B, 30},
  {189, kParamIns1C, 54},   {189, kParamIns1Mix, 18},   {189, kParamIns2Algo, 4},
  {189, kParamIns2A, 24},   {189, kParamIns2B, 34},   {189, kParamIns2C, 12},
  {189, kParamIns2Mix, 14},   {189, kParamMstAlgo, 2},   {189, kParamMstA, 24},
  {189, kParamMstB, 18},   {189, kParamMstC, 30},   {189, kParamMstMix, 10},
  {190, kParamEngine, 0},   {190, kParamPolyMode, 0},   {190, kParamPorta, 16},
  {190, kParamOsc1Shape, 40},   {190, kParamOsc1Level, 86},   {190, kParamOsc2Shape, 38},
  {190, kParamOsc2Pitch, 0},   {190, kParamOsc2Level, 74},   {190, kParamSubLevel, 24},
  {190, kParamNoiseLevel, 0},   {190, kParamFilterType, 0},   {190, kParamFilterCutoff, 5000},
  {190, kParamFilterReso, 28},   {190, kParamFilterDrive, 0},   {190, kParamFilterEnv, 54},
  {190, kParamFiltAttack, 60},   {190, kParamFiltDecay, 800},   {190, kParamFiltSustain, 56},
  {190, kParamFiltRelease, 1000},   {190, kParamAmpAttack, 40},   {190, kParamAmpDecay, 800},
  {190, kParamAmpSustain, 56},   {190, kParamAmpRelease, 1000},   {190, kParamPitchAmount, -14},
  {190, kParamPitchAttack, 1},   {190, kParamPitchDecay, 90},   {190, kParamPitchSustain, 0},
  {190, kParamPitchRelease, 200},   {190, kParamUnisonVoices, 2},   {190, kParamUnisonDetune, 16},
  {190, kParamVelToAmp, 40},   {190, kParamVelToFilter, 34},   {190, kParamStereoSpread, 52},
  {190, kParamStereoDelay, 20},   {190, kParamTapeSat, 26},   {190, kParamTapeMix, 30},
  {190, kParamIns1Algo, 3},   {190, kParamIns1A, 44},   {190, kParamIns1B, 36},
  {190, kParamIns1C, 60},   {190, kParamIns1Mix, 24},   {190, kParamIns2Algo, 3},
  {190, kParamIns2A, 30},   {190, kParamIns2B, 28},   {190, kParamIns2C, 44},
  {190, kParamIns2Mix, 12},   {190, kParamMstAlgo, 2},   {190, kParamMstA, 24},
  {190, kParamMstB, 18},   {190, kParamMstC, 30},   {190, kParamMstMix, 10},
  {191, kParamEngine, 0},   {191, kParamPolyMode, 0},   {191, kParamPorta, 0},
  {191, kParamOsc1Shape, 30},   {191, kParamOsc1Level, 54},   {191, kParamOsc2Level, 0},
  {191, kParamOsc3Enable, 1},   {191, kParamOsc3Source, 1},   {191, kParamOsc3Morph, 3.4},
  {191, kParamOsc3Level, 88},   {191, kParamOsc3Pitch, 12},   {191, kParamSubLevel, 20},
  {191, kParamFilterType, 0},   {191, kParamFilterCutoff, 6200},   {191, kParamFilterReso, 22},
  {191, kParamFilterEnv, 44},   {191, kParamFiltAttack, 2},   {191, kParamFiltDecay, 900},
  {191, kParamFiltSustain, 48},   {191, kParamFiltRelease, 1500},   {191, kParamAmpAttack, 70},
  {191, kParamAmpDecay, 1000},   {191, kParamAmpSustain, 64},   {191, kParamAmpRelease, 1500},
  {191, kParamUnisonVoices, 2},   {191, kParamUnisonDetune, 13},   {191, kParamVelToAmp, 46},
  {191, kParamVelToFilter, 30},   {191, kParamStereoSpread, 44},   {191, kParamStereoDelay, 30},
  {191, kParamTapeSat, 24},   {191, kParamTapeMix, 32},   {191, kParamIns1Algo, 4},
  {191, kParamIns1A, 26},   {191, kParamIns1B, 36},   {191, kParamIns1C, 16},
  {191, kParamIns1Mix, 16},   {191, kParamIns2Algo, 4},   {191, kParamIns2A, 24},
  {191, kParamIns2B, 34},   {191, kParamIns2C, 12},   {191, kParamIns2Mix, 14},
  {191, kParamMstAlgo, 2},   {191, kParamMstA, 24},   {191, kParamMstB, 18},
  {191, kParamMstC, 30},   {191, kParamMstMix, 10},
  {192, kParamEngine, 0},   {192, kParamPolyMode, 0},   {192, kParamPorta, 18},
  {192, kParamOsc1Shape, 44},   {192, kParamOsc1Level, 86},   {192, kParamOsc2Shape, 42},
  {192, kParamOsc2Pitch, 0},   {192, kParamOsc2Level, 74},   {192, kParamSubLevel, 24},
  {192, kParamNoiseLevel, 0},   {192, kParamFilterType, 0},   {192, kParamFilterCutoff, 7600},
  {192, kParamFilterReso, 18},   {192, kParamFilterDrive, 0},   {192, kParamFilterEnv, 54},
  {192, kParamFiltAttack, 2},   {192, kParamFiltDecay, 900},   {192, kParamFiltSustain, 60},
  {192, kParamFiltRelease, 1500},   {192, kParamAmpAttack, 12},   {192, kParamAmpDecay, 900},
  {192, kParamAmpSustain, 60},   {192, kParamAmpRelease, 1500},   {192, kParamPitchAmount, -14},
  {192, kParamPitchAttack, 1},   {192, kParamPitchDecay, 90},   {192, kParamPitchSustain, 0},
  {192, kParamPitchRelease, 200},   {192, kParamUnisonVoices, 2},   {192, kParamUnisonDetune, 16},
  {192, kParamVelToAmp, 40},   {192, kParamVelToFilter, 34},   {192, kParamStereoSpread, 56},
  {192, kParamStereoDelay, 20},   {192, kParamTapeSat, 26},   {192, kParamTapeMix, 30},
  {192, kParamIns1Algo, 3},   {192, kParamIns1A, 34},   {192, kParamIns1B, 30},
  {192, kParamIns1C, 54},   {192, kParamIns1Mix, 18},   {192, kParamIns2Algo, 3},
  {192, kParamIns2A, 30},   {192, kParamIns2B, 28},   {192, kParamIns2C, 44},
  {192, kParamIns2Mix, 12},   {192, kParamMstAlgo, 2},   {192, kParamMstA, 24},
  {192, kParamMstB, 18},   {192, kParamMstC, 30},   {192, kParamMstMix, 10},
  {193, kParamEngine, 0},   {193, kParamPolyMode, 0},   {193, kParamPorta, 0},
  {193, kParamOsc1Shape, 22},   {193, kParamOsc1Level, 78},   {193, kParamOsc2Shape, 24},
  {193, kParamOsc2Pitch, 12},   {193, kParamOsc2Level, 62},   {193, kParamFmAmount, 28},
  {193, kParamSubLevel, 16},   {193, kParamFilterType, 0},   {193, kParamFilterCutoff, 7400},
  {193, kParamFilterReso, 20},   {193, kParamFilterEnv, 46},   {193, kParamFiltAttack, 2},
  {193, kParamFiltDecay, 1000},   {193, kParamFiltSustain, 27.599999999999998},   {193, kParamFiltRelease, 1300},
  {193, kParamAmpAttack, 8},   {193, kParamAmpDecay, 1000},   {193, kParamAmpSustain, 46},
  {193, kParamAmpRelease, 1300},   {193, kParamVelToAmp, 52},   {193, kParamVelToFilter, 28},
  {193, kParamStereoSpread, 54},   {193, kParamStereoDelay, 26},   {193, kParamTapeSat, 22},
  {193, kParamTapeMix, 30},   {193, kParamIns1Algo, 3},   {193, kParamIns1A, 44},
  {193, kParamIns1B, 36},   {193, kParamIns1C, 60},   {193, kParamIns1Mix, 24},
  {193, kParamIns2Algo, 4},   {193, kParamIns2A, 24},   {193, kParamIns2B, 34},
  {193, kParamIns2C, 12},   {193, kParamIns2Mix, 14},   {193, kParamMstAlgo, 2},
  {193, kParamMstA, 24},   {193, kParamMstB, 18},   {193, kParamMstC, 30},
  {193, kParamMstMix, 10},
  {194, kParamEngine, 0},   {194, kParamPolyMode, 0},   {194, kParamPorta, 0},
  {194, kParamOsc1Shape, 52},   {194, kParamOsc1PW, 46},   {194, kParamOsc1Level, 88},
  {194, kParamOsc2Shape, 50},   {194, kParamOsc2Pitch, -12},   {194, kParamOsc2Level, 56},
  {194, kParamSubLevel, 30},   {194, kParamFilterType, 0},   {194, kParamFilterCutoff, 5400},
  {194, kParamFilterReso, 24},   {194, kParamFilterDrive, 0},   {194, kParamFilterEnv, 52},
  {194, kParamFiltAttack, 2},   {194, kParamFiltDecay, 600},   {194, kParamFiltSustain, 46},
  {194, kParamFiltRelease, 1000},   {194, kParamAmpAttack, 30},   {194, kParamAmpDecay, 800},
  {194, kParamAmpSustain, 62},   {194, kParamAmpRelease, 1000},   {194, kParamPitchAmount, -10},
  {194, kParamPitchDecay, 70},   {194, kParamUnisonVoices, 2},   {194, kParamUnisonDetune, 14},
  {194, kParamVelToAmp, 42},   {194, kParamVelToFilter, 32},   {194, kParamStereoSpread, 32},
  {194, kParamStereoDelay, 24},   {194, kParamTapeSat, 28},   {194, kParamTapeMix, 32},
  {194, kParamIns1Algo, 4},   {194, kParamIns1A, 26},   {194, kParamIns1B, 36},
  {194, kParamIns1C, 16},   {194, kParamIns1Mix, 16},   {194, kParamIns2Algo, 3},
  {194, kParamIns2A, 30},   {194, kParamIns2B, 28},   {194, kParamIns2C, 44},
  {194, kParamIns2Mix, 12},   {194, kParamMstAlgo, 2},   {194, kParamMstA, 24},
  {194, kParamMstB, 18},   {194, kParamMstC, 30},   {194, kParamMstMix, 10},
  {195, kParamEngine, 0},   {195, kParamPolyMode, 0},   {195, kParamPorta, 10},
  {195, kParamOsc1Shape, 44},   {195, kParamOsc1Level, 86},   {195, kParamOsc2Shape, 42},
  {195, kParamOsc2Pitch, 0},   {195, kParamOsc2Level, 74},   {195, kParamSubLevel, 24},
  {195, kParamNoiseLevel, 0},   {195, kParamFilterType, 0},   {195, kParamFilterCutoff, 6400},
  {195, kParamFilterReso, 26},   {195, kParamFilterDrive, 0},   {195, kParamFilterEnv, 54},
  {195, kParamFiltAttack, 2},   {195, kParamFiltDecay, 700},   {195, kParamFiltSustain, 50},
  {195, kParamFiltRelease, 1000},   {195, kParamAmpAttack, 6},   {195, kParamAmpDecay, 700},
  {195, kParamAmpSustain, 50},   {195, kParamAmpRelease, 1000},   {195, kParamPitchAmount, -14},
  {195, kParamPitchAttack, 1},   {195, kParamPitchDecay, 90},   {195, kParamPitchSustain, 0},
  {195, kParamPitchRelease, 200},   {195, kParamUnisonVoices, 2},   {195, kParamUnisonDetune, 16},
  {195, kParamVelToAmp, 40},   {195, kParamVelToFilter, 34},   {195, kParamStereoSpread, 52},
  {195, kParamStereoDelay, 20},   {195, kParamTapeSat, 26},   {195, kParamTapeMix, 44},
  {195, kParamTapeWow, 44},   {195, kParamTapeFlutter, 32},   {195, kParamIns1Algo, 3},
  {195, kParamIns1A, 34},   {195, kParamIns1B, 30},   {195, kParamIns1C, 54},
  {195, kParamIns1Mix, 18},   {195, kParamIns2Algo, 4},   {195, kParamIns2A, 24},
  {195, kParamIns2B, 34},   {195, kParamIns2C, 12},   {195, kParamIns2Mix, 14},
  {195, kParamMstAlgo, 2},   {195, kParamMstA, 24},   {195, kParamMstB, 18},
  {195, kParamMstC, 30},   {195, kParamMstMix, 10},
  {196, kParamEngine, 0},   {196, kParamPolyMode, 0},   {196, kParamPorta, 0},
  {196, kParamOsc1Shape, 52},   {196, kParamOsc1PW, 38},   {196, kParamOsc1Level, 88},
  {196, kParamOsc2Shape, 50},   {196, kParamOsc2Pitch, -12},   {196, kParamOsc2Level, 56},
  {196, kParamSubLevel, 30},   {196, kParamFilterType, 0},   {196, kParamFilterCutoff, 6000},
  {196, kParamFilterReso, 30},   {196, kParamFilterDrive, 0},   {196, kParamFilterEnv, 52},
  {196, kParamFiltAttack, 2},   {196, kParamFiltDecay, 600},   {196, kParamFiltSustain, 46},
  {196, kParamFiltRelease, 900},   {196, kParamAmpAttack, 6},   {196, kParamAmpDecay, 800},
  {196, kParamAmpSustain, 62},   {196, kParamAmpRelease, 900},   {196, kParamPitchAmount, -10},
  {196, kParamPitchDecay, 70},   {196, kParamUnisonVoices, 2},   {196, kParamUnisonDetune, 30},
  {196, kParamVelToAmp, 42},   {196, kParamVelToFilter, 32},   {196, kParamStereoSpread, 32},
  {196, kParamStereoDelay, 24},   {196, kParamTapeSat, 28},   {196, kParamTapeMix, 32},
  {196, kParamTapeWow, 40},   {196, kParamIns1Algo, 3},   {196, kParamIns1A, 44},
  {196, kParamIns1B, 36},   {196, kParamIns1C, 60},   {196, kParamIns1Mix, 24},
  {196, kParamIns2Algo, 3},   {196, kParamIns2A, 30},   {196, kParamIns2B, 28},
  {196, kParamIns2C, 44},   {196, kParamIns2Mix, 12},   {196, kParamMstAlgo, 2},
  {196, kParamMstA, 24},   {196, kParamMstB, 18},   {196, kParamMstC, 30},
  {196, kParamMstMix, 10},
  {197, kParamEngine, 0},   {197, kParamPolyMode, 0},   {197, kParamPorta, 0},
  {197, kParamOsc1Shape, 22},   {197, kParamOsc1Level, 78},   {197, kParamOsc2Shape, 24},
  {197, kParamOsc2Pitch, 12},   {197, kParamOsc2Level, 62},   {197, kParamFmAmount, 30},
  {197, kParamSubLevel, 16},   {197, kParamFilterType, 0},   {197, kParamFilterCutoff, 7000},
  {197, kParamFilterReso, 22},   {197, kParamFilterEnv, 46},   {197, kParamFiltAttack, 2},
  {197, kParamFiltDecay, 900},   {197, kParamFiltSustain, 25.2},   {197, kParamFiltRelease, 1200},
  {197, kParamAmpAttack, 6},   {197, kParamAmpDecay, 900},   {197, kParamAmpSustain, 42},
  {197, kParamAmpRelease, 1200},   {197, kParamVelToAmp, 52},   {197, kParamVelToFilter, 28},
  {197, kParamStereoSpread, 54},   {197, kParamStereoDelay, 26},   {197, kParamTapeSat, 22},
  {197, kParamTapeMix, 30},   {197, kParamTapeWow, 42},   {197, kParamTapeHiss, 12},
  {197, kParamIns1Algo, 4},   {197, kParamIns1A, 26},   {197, kParamIns1B, 36},
  {197, kParamIns1C, 16},   {197, kParamIns1Mix, 16},   {197, kParamIns2Algo, 4},
  {197, kParamIns2A, 24},   {197, kParamIns2B, 34},   {197, kParamIns2C, 12},
  {197, kParamIns2Mix, 14},   {197, kParamMstAlgo, 2},   {197, kParamMstA, 24},
  {197, kParamMstB, 18},   {197, kParamMstC, 30},   {197, kParamMstMix, 10},
  {198, kParamEngine, 0},   {198, kParamPolyMode, 0},   {198, kParamPorta, 16},
  {198, kParamOsc1Shape, 44},   {198, kParamOsc1Level, 86},   {198, kParamOsc2Shape, 42},
  {198, kParamOsc2Pitch, 0},   {198, kParamOsc2Level, 74},   {198, kParamSubLevel, 24},
  {198, kParamNoiseLevel, 0},   {198, kParamFilterType, 0},   {198, kParamFilterCutoff, 6800},
  {198, kParamFilterReso, 24},   {198, kParamFilterDrive, 0},   {198, kParamFilterEnv, 54},
  {198, kParamFiltAttack, 2},   {198, kParamFiltDecay, 800},   {198, kParamFiltSustain, 52},
  {198, kParamFiltRelease, 1100},   {198, kParamAmpAttack, 8},   {198, kParamAmpDecay, 800},
  {198, kParamAmpSustain, 52},   {198, kParamAmpRelease, 1100},   {198, kParamPitchAmount, -14},
  {198, kParamPitchAttack, 1},   {198, kParamPitchDecay, 90},   {198, kParamPitchSustain, 0},
  {198, kParamPitchRelease, 200},   {198, kParamUnisonVoices, 2},   {198, kParamUnisonDetune, 16},
  {198, kParamVelToAmp, 40},   {198, kParamVelToFilter, 34},   {198, kParamStereoSpread, 52},
  {198, kParamStereoDelay, 20},   {198, kParamTapeSat, 38},   {198, kParamTapeMix, 46},
  {198, kParamTapeFlutter, 36},   {198, kParamIns1Algo, 3},   {198, kParamIns1A, 34},
  {198, kParamIns1B, 30},   {198, kParamIns1C, 54},   {198, kParamIns1Mix, 18},
  {198, kParamIns2Algo, 3},   {198, kParamIns2A, 30},   {198, kParamIns2B, 28},
  {198, kParamIns2C, 44},   {198, kParamIns2Mix, 12},   {198, kParamMstAlgo, 2},
  {198, kParamMstA, 24},   {198, kParamMstB, 18},   {198, kParamMstC, 30},
  {198, kParamMstMix, 10},
  {199, kParamEngine, 0},   {199, kParamPolyMode, 0},   {199, kParamPorta, 0},
  {199, kParamOsc1Shape, 30},   {199, kParamOsc1Level, 54},   {199, kParamOsc2Level, 0},
  {199, kParamOsc3Enable, 1},   {199, kParamOsc3Source, 1},   {199, kParamOsc3Morph, 2.2},
  {199, kParamOsc3Level, 88},   {199, kParamOsc3Pitch, 0},   {199, kParamSubLevel, 20},
  {199, kParamFilterType, 0},   {199, kParamFilterCutoff, 6400},   {199, kParamFilterReso, 26},
  {199, kParamFilterEnv, 44},   {199, kParamFiltAttack, 2},   {199, kParamFiltDecay, 900},
  {199, kParamFiltSustain, 48},   {199, kParamFiltRelease, 1400},   {199, kParamAmpAttack, 40},
  {199, kParamAmpDecay, 1000},   {199, kParamAmpSustain, 64},   {199, kParamAmpRelease, 1400},
  {199, kParamUnisonVoices, 2},   {199, kParamUnisonDetune, 26},   {199, kParamVelToAmp, 46},
  {199, kParamVelToFilter, 30},   {199, kParamStereoSpread, 44},   {199, kParamStereoDelay, 30},
  {199, kParamTapeSat, 24},   {199, kParamTapeMix, 32},   {199, kParamTapeWow, 46},
  {199, kParamIns1Algo, 3},   {199, kParamIns1A, 44},   {199, kParamIns1B, 36},
  {199, kParamIns1C, 60},   {199, kParamIns1Mix, 24},   {199, kParamIns2Algo, 4},
  {199, kParamIns2A, 24},   {199, kParamIns2B, 34},   {199, kParamIns2C, 12},
  {199, kParamIns2Mix, 14},   {199, kParamMstAlgo, 2},   {199, kParamMstA, 24},
  {199, kParamMstB, 18},   {199, kParamMstC, 30},   {199, kParamMstMix, 10},
  {200, kParamEngine, 0},   {200, kParamPolyMode, 0},   {200, kParamPorta, 0},
  {200, kParamOsc1Shape, 52},   {200, kParamOsc1PW, 26},   {200, kParamOsc1Level, 88},
  {200, kParamOsc2Shape, 50},   {200, kParamOsc2Pitch, -12},   {200, kParamOsc2Level, 56},
  {200, kParamSubLevel, 30},   {200, kParamFilterType, 0},   {200, kParamFilterCutoff, 5600},
  {200, kParamFilterReso, 32},   {200, kParamFilterDrive, 0},   {200, kParamFilterEnv, 52},
  {200, kParamFiltAttack, 2},   {200, kParamFiltDecay, 600},   {200, kParamFiltSustain, 46},
  {200, kParamFiltRelease, 800},   {200, kParamAmpAttack, 4},   {200, kParamAmpDecay, 800},
  {200, kParamAmpSustain, 62},   {200, kParamAmpRelease, 800},   {200, kParamPitchAmount, -10},
  {200, kParamPitchDecay, 70},   {200, kParamUnisonVoices, 2},   {200, kParamUnisonDetune, 14},
  {200, kParamVelToAmp, 42},   {200, kParamVelToFilter, 32},   {200, kParamStereoSpread, 32},
  {200, kParamStereoDelay, 24},   {200, kParamTapeSat, 28},   {200, kParamTapeMix, 42},
  {200, kParamTapeWow, 48},   {200, kParamIns1Algo, 4},   {200, kParamIns1A, 26},
  {200, kParamIns1B, 36},   {200, kParamIns1C, 16},   {200, kParamIns1Mix, 16},
  {200, kParamIns2Algo, 3},   {200, kParamIns2A, 30},   {200, kParamIns2B, 28},
  {200, kParamIns2C, 44},   {200, kParamIns2Mix, 12},   {200, kParamMstAlgo, 2},
  {200, kParamMstA, 24},   {200, kParamMstB, 18},   {200, kParamMstC, 30},
  {200, kParamMstMix, 10},
  {201, kParamEngine, 0},   {201, kParamPolyMode, 0},   {201, kParamPorta, 20},
  {201, kParamOsc1Shape, 44},   {201, kParamOsc1Level, 86},   {201, kParamOsc2Shape, 42},
  {201, kParamOsc2Pitch, 0},   {201, kParamOsc2Level, 74},   {201, kParamSubLevel, 38},
  {201, kParamNoiseLevel, 0},   {201, kParamFilterType, 0},   {201, kParamFilterCutoff, 5200},
  {201, kParamFilterReso, 28},   {201, kParamFilterDrive, 0},   {201, kParamFilterEnv, 54},
  {201, kParamFiltAttack, 2},   {201, kParamFiltDecay, 900},   {201, kParamFiltSustain, 54},
  {201, kParamFiltRelease, 1200},   {201, kParamAmpAttack, 10},   {201, kParamAmpDecay, 900},
  {201, kParamAmpSustain, 54},   {201, kParamAmpRelease, 1200},   {201, kParamPitchAmount, -14},
  {201, kParamPitchAttack, 1},   {201, kParamPitchDecay, 90},   {201, kParamPitchSustain, 0},
  {201, kParamPitchRelease, 200},   {201, kParamUnisonVoices, 2},   {201, kParamUnisonDetune, 16},
  {201, kParamVelToAmp, 40},   {201, kParamVelToFilter, 34},   {201, kParamStereoSpread, 52},
  {201, kParamStereoDelay, 20},   {201, kParamTapeSat, 26},   {201, kParamTapeMix, 40},
  {201, kParamIns1Algo, 3},   {201, kParamIns1A, 34},   {201, kParamIns1B, 30},
  {201, kParamIns1C, 54},   {201, kParamIns1Mix, 18},   {201, kParamIns2Algo, 4},
  {201, kParamIns2A, 24},   {201, kParamIns2B, 34},   {201, kParamIns2C, 12},
  {201, kParamIns2Mix, 14},   {201, kParamMstAlgo, 2},   {201, kParamMstA, 24},
  {201, kParamMstB, 18},   {201, kParamMstC, 30},   {201, kParamMstMix, 10},
  {202, kParamEngine, 0},   {202, kParamPolyMode, 0},   {202, kParamPorta, 14},
  {202, kParamOsc1Shape, 44},   {202, kParamOsc1Level, 86},   {202, kParamOsc2Shape, 42},
  {202, kParamOsc2Pitch, 0},   {202, kParamOsc2Level, 74},   {202, kParamSubLevel, 24},
  {202, kParamNoiseLevel, 0},   {202, kParamFilterType, 0},   {202, kParamFilterCutoff, 7000},
  {202, kParamFilterReso, 32},   {202, kParamFilterDrive, 0},   {202, kParamFilterEnv, 54},
  {202, kParamFiltAttack, 2},   {202, kParamFiltDecay, 600},   {202, kParamFiltSustain, 50},
  {202, kParamFiltRelease, 800},   {202, kParamAmpAttack, 3},   {202, kParamAmpDecay, 600},
  {202, kParamAmpSustain, 50},   {202, kParamAmpRelease, 800},   {202, kParamPitchAmount, -14},
  {202, kParamPitchAttack, 1},   {202, kParamPitchDecay, 90},   {202, kParamPitchSustain, 0},
  {202, kParamPitchRelease, 200},   {202, kParamUnisonVoices, 6},   {202, kParamUnisonDetune, 24},
  {202, kParamVelToAmp, 40},   {202, kParamVelToFilter, 34},   {202, kParamStereoSpread, 72},
  {202, kParamStereoDelay, 20},   {202, kParamTapeSat, 26},   {202, kParamTapeMix, 30},
  {202, kParamIns1Algo, 3},   {202, kParamIns1A, 44},   {202, kParamIns1B, 36},
  {202, kParamIns1C, 60},   {202, kParamIns1Mix, 24},   {202, kParamIns2Algo, 3},
  {202, kParamIns2A, 30},   {202, kParamIns2B, 28},   {202, kParamIns2C, 44},
  {202, kParamIns2Mix, 12},   {202, kParamMstAlgo, 2},   {202, kParamMstA, 24},
  {202, kParamMstB, 18},   {202, kParamMstC, 30},   {202, kParamMstMix, 10},
  {203, kParamEngine, 0},   {203, kParamPolyMode, 0},   {203, kParamPorta, 0},
  {203, kParamOsc1Shape, 46},   {203, kParamOsc1Level, 88},   {203, kParamOsc2Shape, 48},
  {203, kParamOsc2Pitch, 12},   {203, kParamOsc2Level, 80},   {203, kParamOscSync, 1},
  {203, kParamSubLevel, 18},   {203, kParamFilterType, 0},   {203, kParamFilterCutoff, 7600},
  {203, kParamFilterReso, 40},   {203, kParamFilterDrive, 18},   {203, kParamFilterEnv, 58},
  {203, kParamFiltAttack, 2},   {203, kParamFiltDecay, 700},   {203, kParamFiltSustain, 42},
  {203, kParamFiltRelease, 700},   {203, kParamAmpAttack, 2},   {203, kParamAmpDecay, 900},
  {203, kParamAmpSustain, 58},   {203, kParamAmpRelease, 700},   {203, kParamUnisonVoices, 2},
  {203, kParamUnisonDetune, 12},   {203, kParamVelToAmp, 44},   {203, kParamVelToFilter, 30},
  {203, kParamStereoSpread, 48},   {203, kParamStereoDelay, 24},   {203, kParamTapeSat, 32},
  {203, kParamTapeMix, 34},   {203, kParamIns1Algo, 4},   {203, kParamIns1A, 26},
  {203, kParamIns1B, 36},   {203, kParamIns1C, 16},   {203, kParamIns1Mix, 16},
  {203, kParamIns2Algo, 4},   {203, kParamIns2A, 24},   {203, kParamIns2B, 34},
  {203, kParamIns2C, 12},   {203, kParamIns2Mix, 14},   {203, kParamMstAlgo, 2},
  {203, kParamMstA, 24},   {203, kParamMstB, 18},   {203, kParamMstC, 30},
  {203, kParamMstMix, 10},
  {204, kParamEngine, 0},   {204, kParamPolyMode, 0},   {204, kParamPorta, 0},
  {204, kParamOsc1Shape, 22},   {204, kParamOsc1Level, 78},   {204, kParamOsc2Shape, 24},
  {204, kParamOsc2Pitch, 12},   {204, kParamOsc2Level, 62},   {204, kParamFmAmount, 52},
  {204, kParamSubLevel, 16},   {204, kParamFilterType, 0},   {204, kParamFilterCutoff, 9200},
  {204, kParamFilterReso, 28},   {204, kParamFilterEnv, 46},   {204, kParamFiltAttack, 2},
  {204, kParamFiltDecay, 700},   {204, kParamFiltSustain, 25.2},   {204, kParamFiltRelease, 900},
  {204, kParamAmpAttack, 2},   {204, kParamAmpDecay, 700},   {204, kParamAmpSustain, 42},
  {204, kParamAmpRelease, 900},   {204, kParamVelToAmp, 52},   {204, kParamVelToFilter, 28},
  {204, kParamStereoSpread, 54},   {204, kParamStereoDelay, 26},   {204, kParamTapeSat, 22},
  {204, kParamTapeMix, 30},   {204, kParamIns1Algo, 3},   {204, kParamIns1A, 34},
  {204, kParamIns1B, 30},   {204, kParamIns1C, 54},   {204, kParamIns1Mix, 18},
  {204, kParamIns2Algo, 3},   {204, kParamIns2A, 30},   {204, kParamIns2B, 28},
  {204, kParamIns2C, 44},   {204, kParamIns2Mix, 12},   {204, kParamMstAlgo, 2},
  {204, kParamMstA, 24},   {204, kParamMstB, 18},   {204, kParamMstC, 30},
  {204, kParamMstMix, 10},
  {205, kParamEngine, 0},   {205, kParamPolyMode, 0},   {205, kParamPorta, 0},
  {205, kParamOsc1Shape, 52},   {205, kParamOsc1PW, 18},   {205, kParamOsc1Level, 88},
  {205, kParamOsc2Shape, 50},   {205, kParamOsc2Pitch, -12},   {205, kParamOsc2Level, 56},
  {205, kParamSubLevel, 30},   {205, kParamFilterType, 0},   {205, kParamFilterCutoff, 8600},
  {205, kParamFilterReso, 36},   {205, kParamFilterDrive, 0},   {205, kParamFilterEnv, 52},
  {205, kParamFiltAttack, 2},   {205, kParamFiltDecay, 600},   {205, kParamFiltSustain, 46},
  {205, kParamFiltRelease, 600},   {205, kParamAmpAttack, 2},   {205, kParamAmpDecay, 800},
  {205, kParamAmpSustain, 62},   {205, kParamAmpRelease, 600},   {205, kParamPitchAmount, -10},
  {205, kParamPitchDecay, 70},   {205, kParamUnisonVoices, 2},   {205, kParamUnisonDetune, 14},
  {205, kParamVelToAmp, 42},   {205, kParamVelToFilter, 32},   {205, kParamStereoSpread, 32},
  {205, kParamStereoDelay, 24},   {205, kParamTapeSat, 28},   {205, kParamTapeMix, 32},
  {205, kParamIns1Algo, 3},   {205, kParamIns1A, 44},   {205, kParamIns1B, 36},
  {205, kParamIns1C, 60},   {205, kParamIns1Mix, 24},   {205, kParamIns2Algo, 4},
  {205, kParamIns2A, 24},   {205, kParamIns2B, 34},   {205, kParamIns2C, 12},
  {205, kParamIns2Mix, 14},   {205, kParamMstAlgo, 2},   {205, kParamMstA, 24},
  {205, kParamMstB, 18},   {205, kParamMstC, 30},   {205, kParamMstMix, 10},
  {206, kParamEngine, 0},   {206, kParamPolyMode, 0},   {206, kParamPorta, 0},
  {206, kParamOsc1Shape, 30},   {206, kParamOsc1Level, 54},   {206, kParamOsc2Level, 0},
  {206, kParamOsc3Enable, 1},   {206, kParamOsc3Source, 1},   {206, kParamOsc3Morph, 1},
  {206, kParamOsc3Level, 88},   {206, kParamOsc3Pitch, 0},   {206, kParamSubLevel, 20},
  {206, kParamFilterType, 0},   {206, kParamFilterCutoff, 7800},   {206, kParamFilterReso, 34},
  {206, kParamFilterEnv, 44},   {206, kParamFiltAttack, 2},   {206, kParamFiltDecay, 900},
  {206, kParamFiltSustain, 48},   {206, kParamFiltRelease, 1200},   {206, kParamAmpAttack, 3},
  {206, kParamAmpDecay, 1000},   {206, kParamAmpSustain, 64},   {206, kParamAmpRelease, 1200},
  {206, kParamUnisonVoices, 2},   {206, kParamUnisonDetune, 13},   {206, kParamVelToAmp, 46},
  {206, kParamVelToFilter, 30},   {206, kParamStereoSpread, 44},   {206, kParamStereoDelay, 30},
  {206, kParamTapeSat, 24},   {206, kParamTapeMix, 32},   {206, kParamLfoRate, 0.5},
  {206, kParamLfoDest, 16},   {206, kParamLfoAmount, 58},   {206, kParamIns1Algo, 4},
  {206, kParamIns1A, 26},   {206, kParamIns1B, 36},   {206, kParamIns1C, 16},
  {206, kParamIns1Mix, 16},   {206, kParamIns2Algo, 3},   {206, kParamIns2A, 30},
  {206, kParamIns2B, 28},   {206, kParamIns2C, 44},   {206, kParamIns2Mix, 12},
  {206, kParamMstAlgo, 2},   {206, kParamMstA, 24},   {206, kParamMstB, 18},
  {206, kParamMstC, 30},   {206, kParamMstMix, 10},
  {207, kParamEngine, 0},   {207, kParamPolyMode, 0},   {207, kParamPorta, 0},
  {207, kParamOsc1Shape, 44},   {207, kParamOsc1Level, 86},   {207, kParamOsc2Shape, 42},
  {207, kParamOsc2Pitch, 0},   {207, kParamOsc2Level, 74},   {207, kParamSubLevel, 36},
  {207, kParamNoiseLevel, 0},   {207, kParamFilterType, 0},   {207, kParamFilterCutoff, 5400},
  {207, kParamFilterReso, 42},   {207, kParamFilterDrive, 44},   {207, kParamFilterEnv, 54},
  {207, kParamFiltAttack, 2},   {207, kParamFiltDecay, 600},   {207, kParamFiltSustain, 48},
  {207, kParamFiltRelease, 750},   {207, kParamAmpAttack, 3},   {207, kParamAmpDecay, 600},
  {207, kParamAmpSustain, 48},   {207, kParamAmpRelease, 750},   {207, kParamPitchAmount, -14},
  {207, kParamPitchAttack, 1},   {207, kParamPitchDecay, 90},   {207, kParamPitchSustain, 0},
  {207, kParamPitchRelease, 200},   {207, kParamUnisonVoices, 2},   {207, kParamUnisonDetune, 16},
  {207, kParamVelToAmp, 40},   {207, kParamVelToFilter, 34},   {207, kParamStereoSpread, 52},
  {207, kParamStereoDelay, 20},   {207, kParamTapeSat, 26},   {207, kParamTapeMix, 30},
  {207, kParamIns1Algo, 3},   {207, kParamIns1A, 34},   {207, kParamIns1B, 30},
  {207, kParamIns1C, 54},   {207, kParamIns1Mix, 18},   {207, kParamIns2Algo, 4},
  {207, kParamIns2A, 24},   {207, kParamIns2B, 34},   {207, kParamIns2C, 12},
  {207, kParamIns2Mix, 14},   {207, kParamMstAlgo, 2},   {207, kParamMstA, 24},
  {207, kParamMstB, 18},   {207, kParamMstC, 30},   {207, kParamMstMix, 10},
  {208, kParamEngine, 0},   {208, kParamPolyMode, 0},   {208, kParamPorta, 0},
  {208, kParamOsc1Shape, 44},   {208, kParamOsc1Level, 86},   {208, kParamOsc2Shape, 42},
  {208, kParamOsc2Pitch, 0},   {208, kParamOsc2Level, 74},   {208, kParamSubLevel, 24},
  {208, kParamNoiseLevel, 0},   {208, kParamFilterType, 0},   {208, kParamFilterCutoff, 6600},
  {208, kParamFilterReso, 34},   {208, kParamFilterDrive, 0},   {208, kParamFilterEnv, 54},
  {208, kParamFiltAttack, 2},   {208, kParamFiltDecay, 650},   {208, kParamFiltSustain, 50},
  {208, kParamFiltRelease, 850},   {208, kParamAmpAttack, 3},   {208, kParamAmpDecay, 650},
  {208, kParamAmpSustain, 50},   {208, kParamAmpRelease, 850},   {208, kParamPitchAmount, -14},
  {208, kParamPitchAttack, 1},   {208, kParamPitchDecay, 90},   {208, kParamPitchSustain, 0},
  {208, kParamPitchRelease, 200},   {208, kParamUnisonVoices, 4},   {208, kParamUnisonDetune, 20},
  {208, kParamVelToAmp, 40},   {208, kParamVelToFilter, 34},   {208, kParamStereoSpread, 58},
  {208, kParamStereoDelay, 34},   {208, kParamTapeSat, 26},   {208, kParamTapeMix, 30},
  {208, kParamIns1Algo, 3},   {208, kParamIns1A, 44},   {208, kParamIns1B, 36},
  {208, kParamIns1C, 60},   {208, kParamIns1Mix, 24},   {208, kParamIns2Algo, 3},
  {208, kParamIns2A, 30},   {208, kParamIns2B, 28},   {208, kParamIns2C, 44},
  {208, kParamIns2Mix, 12},   {208, kParamMstAlgo, 2},   {208, kParamMstA, 24},
  {208, kParamMstB, 18},   {208, kParamMstC, 30},   {208, kParamMstMix, 10},
  // keys
  {209, kParamEngine, 0},   {209, kParamPolyMode, 0},   {209, kParamOsc1Shape, 20},
  {209, kParamOsc1Level, 70},   {209, kParamOsc2Shape, 24},   {209, kParamOsc2Pitch, 19},
  {209, kParamOsc2Level, 44},   {209, kParamFmAmount, 34},   {209, kParamSubLevel, 22},
  {209, kParamNoiseLevel, 4},   {209, kParamFilterType, 0},   {209, kParamFilterCutoff, 8200},
  {209, kParamFilterReso, 16},   {209, kParamFilterEnv, 30},   {209, kParamFiltAttack, 2},
  {209, kParamFiltDecay, 979.9999999999999},   {209, kParamFiltSustain, 13},   {209, kParamFiltRelease, 1200},
  {209, kParamAmpAttack, 2},   {209, kParamAmpDecay, 1400},   {209, kParamAmpSustain, 26},
  {209, kParamAmpRelease, 1500},   {209, kParamVelToAmp, 64},   {209, kParamVelToFilter, 34},
  {209, kParamStereoSpread, 48},   {209, kParamStereoDelay, 16},   {209, kParamTapeSat, 20},
  {209, kParamTapeMix, 28},   {209, kParamIns1Algo, 2},   {209, kParamIns1A, 14},
  {209, kParamIns1B, 22},   {209, kParamIns1C, 26},   {209, kParamIns1Mix, 14},
  {209, kParamMstAlgo, 2},   {209, kParamMstA, 24},   {209, kParamMstB, 18},
  {209, kParamMstC, 30},   {209, kParamMstMix, 10},
  {210, kParamEngine, 0},   {210, kParamPolyMode, 0},   {210, kParamOsc1Shape, 20},
  {210, kParamOsc1Level, 70},   {210, kParamOsc2Shape, 24},   {210, kParamOsc2Pitch, 19},
  {210, kParamOsc2Level, 44},   {210, kParamFmAmount, 42},   {210, kParamSubLevel, 22},
  {210, kParamNoiseLevel, 4},   {210, kParamFilterType, 0},   {210, kParamFilterCutoff, 9800},
  {210, kParamFilterReso, 18},   {210, kParamFilterEnv, 30},   {210, kParamFiltAttack, 2},
  {210, kParamFiltDecay, 630},   {210, kParamFiltSustain, 10},   {210, kParamFiltRelease, 880},
  {210, kParamAmpAttack, 1},   {210, kParamAmpDecay, 900},   {210, kParamAmpSustain, 20},
  {210, kParamAmpRelease, 1100},   {210, kParamVelToAmp, 64},   {210, kParamVelToFilter, 34},
  {210, kParamStereoSpread, 48},   {210, kParamStereoDelay, 16},   {210, kParamTapeSat, 20},
  {210, kParamTapeMix, 28},   {210, kParamIns1Algo, 2},   {210, kParamIns1A, 20},
  {210, kParamIns1B, 26},   {210, kParamIns1C, 34},   {210, kParamIns1Mix, 18},
  {210, kParamMstAlgo, 2},   {210, kParamMstA, 24},   {210, kParamMstB, 18},
  {210, kParamMstC, 30},   {210, kParamMstMix, 10},
  {211, kParamEngine, 0},   {211, kParamPolyMode, 0},   {211, kParamOsc1Shape, 20},
  {211, kParamOsc1Level, 70},   {211, kParamOsc2Shape, 24},   {211, kParamOsc2Pitch, 19},
  {211, kParamOsc2Level, 44},   {211, kParamFmAmount, 26},   {211, kParamSubLevel, 22},
  {211, kParamNoiseLevel, 4},   {211, kParamFilterType, 0},   {211, kParamFilterCutoff, 6600},
  {211, kParamFilterReso, 12},   {211, kParamFilterEnv, 30},   {211, kParamFiltAttack, 2},
  {211, kParamFiltDecay, 1260},   {211, kParamFiltSustain, 17},   {211, kParamFiltRelease, 1600},
  {211, kParamAmpAttack, 4},   {211, kParamAmpDecay, 1800},   {211, kParamAmpSustain, 34},
  {211, kParamAmpRelease, 2000},   {211, kParamVelToAmp, 64},   {211, kParamVelToFilter, 34},
  {211, kParamStereoSpread, 48},   {211, kParamStereoDelay, 16},   {211, kParamTapeSat, 20},
  {211, kParamTapeMix, 28},   {211, kParamIns1Algo, 1},   {211, kParamIns1A, 22},
  {211, kParamIns1B, 38},   {211, kParamIns1C, 28},   {211, kParamIns1Mix, 14},
  {211, kParamMstAlgo, 2},   {211, kParamMstA, 24},   {211, kParamMstB, 18},
  {211, kParamMstC, 30},   {211, kParamMstMix, 10},
  {212, kParamEngine, 0},   {212, kParamPolyMode, 0},   {212, kParamOsc1Shape, 20},
  {212, kParamOsc1Level, 70},   {212, kParamOsc2Shape, 24},   {212, kParamOsc2Pitch, 19},
  {212, kParamOsc2Level, 44},   {212, kParamFmAmount, 48},   {212, kParamSubLevel, 22},
  {212, kParamNoiseLevel, 4},   {212, kParamFilterType, 0},   {212, kParamFilterCutoff, 9400},
  {212, kParamFilterReso, 20},   {212, kParamFilterEnv, 30},   {212, kParamFiltAttack, 2},
  {212, kParamFiltDecay, 489.99999999999994},   {212, kParamFiltSustain, 6},   {212, kParamFiltRelease, 720},
  {212, kParamAmpAttack, 1},   {212, kParamAmpDecay, 700},   {212, kParamAmpSustain, 12},
  {212, kParamAmpRelease, 900},   {212, kParamVelToAmp, 64},   {212, kParamVelToFilter, 34},
  {212, kParamStereoSpread, 48},   {212, kParamStereoDelay, 16},   {212, kParamTapeSat, 20},
  {212, kParamTapeMix, 28},   {212, kParamOsc3Enable, 1},   {212, kParamOsc3Source, 1},
  {212, kParamOsc3Morph, 4.2},   {212, kParamOsc3Level, 40},   {212, kParamOsc3Pitch, 12},
  {212, kParamIns1Algo, 2},   {212, kParamIns1A, 14},   {212, kParamIns1B, 22},
  {212, kParamIns1C, 26},   {212, kParamIns1Mix, 14},   {212, kParamMstAlgo, 2},
  {212, kParamMstA, 24},   {212, kParamMstB, 18},   {212, kParamMstC, 30},
  {212, kParamMstMix, 10},
  {213, kParamEngine, 0},   {213, kParamPolyMode, 0},   {213, kParamOsc1Shape, 20},
  {213, kParamOsc1Level, 70},   {213, kParamOsc2Shape, 24},   {213, kParamOsc2Pitch, 19},
  {213, kParamOsc2Level, 44},   {213, kParamFmAmount, 52},   {213, kParamSubLevel, 22},
  {213, kParamNoiseLevel, 4},   {213, kParamFilterType, 0},   {213, kParamFilterCutoff, 7400},
  {213, kParamFilterReso, 28},   {213, kParamFilterEnv, 30},   {213, kParamFiltAttack, 2},
  {213, kParamFiltDecay, 770},   {213, kParamFiltSustain, 11},   {213, kParamFiltRelease, 960},
  {213, kParamAmpAttack, 1},   {213, kParamAmpDecay, 1100},   {213, kParamAmpSustain, 22},
  {213, kParamAmpRelease, 1200},   {213, kParamVelToAmp, 64},   {213, kParamVelToFilter, 34},
  {213, kParamStereoSpread, 48},   {213, kParamStereoDelay, 16},   {213, kParamTapeSat, 20},
  {213, kParamTapeMix, 28},   {213, kParamFilterDrive, 26},   {213, kParamIns1Algo, 2},
  {213, kParamIns1A, 20},   {213, kParamIns1B, 26},   {213, kParamIns1C, 34},
  {213, kParamIns1Mix, 18},   {213, kParamMstAlgo, 2},   {213, kParamMstA, 24},
  {213, kParamMstB, 18},   {213, kParamMstC, 30},   {213, kParamMstMix, 10},
  {214, kParamEngine, 0},   {214, kParamPolyMode, 0},   {214, kParamOsc1Shape, 20},
  {214, kParamOsc1Level, 70},   {214, kParamOsc2Shape, 24},   {214, kParamOsc2Pitch, 19},
  {214, kParamOsc2Level, 44},   {214, kParamFmAmount, 30},   {214, kParamSubLevel, 22},
  {214, kParamNoiseLevel, 4},   {214, kParamFilterType, 0},   {214, kParamFilterCutoff, 7000},
  {214, kParamFilterReso, 16},   {214, kParamFilterEnv, 30},   {214, kParamFiltAttack, 2},
  {214, kParamFiltDecay, 1050},   {214, kParamFiltSustain, 14},   {214, kParamFiltRelease, 1360},
  {214, kParamAmpAttack, 3},   {214, kParamAmpDecay, 1500},   {214, kParamAmpSustain, 28},
  {214, kParamAmpRelease, 1700},   {214, kParamVelToAmp, 64},   {214, kParamVelToFilter, 34},
  {214, kParamStereoSpread, 48},   {214, kParamStereoDelay, 16},   {214, kParamTapeSat, 20},
  {214, kParamTapeMix, 44},   {214, kParamTapeWow, 40},   {214, kParamIns1Algo, 1},
  {214, kParamIns1A, 22},   {214, kParamIns1B, 38},   {214, kParamIns1C, 28},
  {214, kParamIns1Mix, 14},   {214, kParamMstAlgo, 2},   {214, kParamMstA, 24},
  {214, kParamMstB, 18},   {214, kParamMstC, 30},   {214, kParamMstMix, 10},
  {215, kParamEngine, 0},   {215, kParamPolyMode, 0},   {215, kParamOsc1Shape, 20},
  {215, kParamOsc1Level, 70},   {215, kParamOsc2Shape, 24},   {215, kParamOsc2Pitch, 12},
  {215, kParamOsc2Level, 44},   {215, kParamFmAmount, 22},   {215, kParamSubLevel, 22},
  {215, kParamNoiseLevel, 4},   {215, kParamFilterType, 0},   {215, kParamFilterCutoff, 6200},
  {215, kParamFilterReso, 18},   {215, kParamFilterEnv, 30},   {215, kParamFiltAttack, 2},
  {215, kParamFiltDecay, 909.9999999999999},   {215, kParamFiltSustain, 15},   {215, kParamFiltRelease, 1200},
  {215, kParamAmpAttack, 3},   {215, kParamAmpDecay, 1300},   {215, kParamAmpSustain, 30},
  {215, kParamAmpRelease, 1500},   {215, kParamVelToAmp, 64},   {215, kParamVelToFilter, 34},
  {215, kParamStereoSpread, 48},   {215, kParamStereoDelay, 16},   {215, kParamTapeSat, 20},
  {215, kParamTapeMix, 28},   {215, kParamIns1Algo, 2},   {215, kParamIns1A, 14},
  {215, kParamIns1B, 22},   {215, kParamIns1C, 26},   {215, kParamIns1Mix, 14},
  {215, kParamMstAlgo, 2},   {215, kParamMstA, 24},   {215, kParamMstB, 18},
  {215, kParamMstC, 30},   {215, kParamMstMix, 10},
  {216, kParamEngine, 0},   {216, kParamPolyMode, 0},   {216, kParamOsc1Shape, 20},
  {216, kParamOsc1Level, 70},   {216, kParamOsc2Shape, 24},   {216, kParamOsc2Pitch, 19},
  {216, kParamOsc2Level, 44},   {216, kParamFmAmount, 18},   {216, kParamSubLevel, 22},
  {216, kParamNoiseLevel, 4},   {216, kParamFilterType, 0},   {216, kParamFilterCutoff, 5600},
  {216, kParamFilterReso, 14},   {216, kParamFilterEnv, 30},   {216, kParamFiltAttack, 2},
  {216, kParamFiltDecay, 1120},   {216, kParamFiltSustain, 18},   {216, kParamFiltRelease, 1440},
  {216, kParamAmpAttack, 5},   {216, kParamAmpDecay, 1600},   {216, kParamAmpSustain, 36},
  {216, kParamAmpRelease, 1800},   {216, kParamVelToAmp, 64},   {216, kParamVelToFilter, 34},
  {216, kParamStereoSpread, 48},   {216, kParamStereoDelay, 16},   {216, kParamTapeSat, 20},
  {216, kParamTapeMix, 28},   {216, kParamIns1Algo, 2},   {216, kParamIns1A, 20},
  {216, kParamIns1B, 26},   {216, kParamIns1C, 34},   {216, kParamIns1Mix, 18},
  {216, kParamMstAlgo, 2},   {216, kParamMstA, 24},   {216, kParamMstB, 18},
  {216, kParamMstC, 30},   {216, kParamMstMix, 10},
  {217, kParamEngine, 0},   {217, kParamPolyMode, 0},   {217, kParamOsc1Shape, 20},
  {217, kParamOsc1Level, 70},   {217, kParamOsc2Shape, 24},   {217, kParamOsc2Pitch, 19},
  {217, kParamOsc2Level, 44},   {217, kParamFmAmount, 30},   {217, kParamSubLevel, 22},
  {217, kParamNoiseLevel, 4},   {217, kParamFilterType, 0},   {217, kParamFilterCutoff, 6800},
  {217, kParamFilterReso, 30},   {217, kParamFilterEnv, 30},   {217, kParamFiltAttack, 2},
  {217, kParamFiltDecay, 700},   {217, kParamFiltSustain, 12},   {217, kParamFiltRelease, 960},
  {217, kParamAmpAttack, 2},   {217, kParamAmpDecay, 1000},   {217, kParamAmpSustain, 24},
  {217, kParamAmpRelease, 1200},   {217, kParamVelToAmp, 64},   {217, kParamVelToFilter, 34},
  {217, kParamStereoSpread, 48},   {217, kParamStereoDelay, 16},   {217, kParamTapeSat, 20},
  {217, kParamTapeMix, 28},   {217, kParamFilterDrive, 30},   {217, kParamIns1Algo, 1},
  {217, kParamIns1A, 22},   {217, kParamIns1B, 38},   {217, kParamIns1C, 28},
  {217, kParamIns1Mix, 14},   {217, kParamMstAlgo, 2},   {217, kParamMstA, 24},
  {217, kParamMstB, 18},   {217, kParamMstC, 30},   {217, kParamMstMix, 10},
  {218, kParamEngine, 0},   {218, kParamPolyMode, 0},   {218, kParamOsc1Shape, 20},
  {218, kParamOsc1Level, 70},   {218, kParamOsc2Shape, 24},   {218, kParamOsc2Pitch, 19},
  {218, kParamOsc2Level, 44},   {218, kParamFmAmount, 24},   {218, kParamSubLevel, 22},
  {218, kParamNoiseLevel, 4},   {218, kParamFilterType, 0},   {218, kParamFilterCutoff, 6000},
  {218, kParamFilterReso, 16},   {218, kParamFilterEnv, 30},   {218, kParamFiltAttack, 2},
  {218, kParamFiltDecay, 979.9999999999999},   {218, kParamFiltSustain, 16},   {218, kParamFiltRelease, 1280},
  {218, kParamAmpAttack, 4},   {218, kParamAmpDecay, 1400},   {218, kParamAmpSustain, 32},
  {218, kParamAmpRelease, 1600},   {218, kParamVelToAmp, 64},   {218, kParamVelToFilter, 34},
  {218, kParamStereoSpread, 48},   {218, kParamStereoDelay, 16},   {218, kParamTapeSat, 20},
  {218, kParamTapeMix, 46},   {218, kParamTapeWow, 44},   {218, kParamTapeFlutter, 30},
  {218, kParamIns1Algo, 2},   {218, kParamIns1A, 14},   {218, kParamIns1B, 22},
  {218, kParamIns1C, 26},   {218, kParamIns1Mix, 14},   {218, kParamMstAlgo, 2},
  {218, kParamMstA, 24},   {218, kParamMstB, 18},   {218, kParamMstC, 30},
  {218, kParamMstMix, 10},
  {219, kParamEngine, 0},   {219, kParamPolyMode, 0},   {219, kParamOsc1Shape, 10},
  {219, kParamOsc1Level, 78},   {219, kParamOsc2Shape, 10},   {219, kParamOsc2Pitch, 12},
  {219, kParamOsc2Level, 52},   {219, kParamSubLevel, 64},   {219, kParamNoiseLevel, 2},
  {219, kParamOsc3Enable, 1},   {219, kParamOsc3Source, 0},   {219, kParamOsc3Shape, 10},
  {219, kParamOsc3Level, 38},   {219, kParamOsc3Pitch, 19},   {219, kParamFilterType, 0},
  {219, kParamFilterCutoff, 11000},   {219, kParamFilterReso, 8},   {219, kParamFilterEnv, 0},
  {219, kParamFilterDrive, 18},   {219, kParamFiltAttack, 1},   {219, kParamFiltDecay, 200},
  {219, kParamFiltSustain, 100},   {219, kParamFiltRelease, 240},   {219, kParamAmpAttack, 2},
  {219, kParamAmpDecay, 200},   {219, kParamAmpSustain, 100},   {219, kParamAmpRelease, 240},
  {219, kParamVelToAmp, 18},   {219, kParamVelToFilter, 0},   {219, kParamStereoSpread, 40},
  {219, kParamStereoDelay, 0},   {219, kParamTapeSat, 26},   {219, kParamTapeMix, 30},
  {219, kParamIns1Algo, 2},   {219, kParamIns1A, 20},   {219, kParamIns1B, 26},
  {219, kParamIns1C, 34},   {219, kParamIns1Mix, 18},   {219, kParamMstAlgo, 2},
  {219, kParamMstA, 24},   {219, kParamMstB, 18},   {219, kParamMstC, 30},
  {219, kParamMstMix, 10},
  {220, kParamEngine, 0},   {220, kParamPolyMode, 0},   {220, kParamOsc1Shape, 10},
  {220, kParamOsc1Level, 88},   {220, kParamOsc2Shape, 10},   {220, kParamOsc2Pitch, 12},
  {220, kParamOsc2Level, 52},   {220, kParamSubLevel, 76},   {220, kParamNoiseLevel, 2},
  {220, kParamOsc3Enable, 1},   {220, kParamOsc3Source, 0},   {220, kParamOsc3Shape, 10},
  {220, kParamOsc3Level, 52},   {220, kParamOsc3Pitch, 19},   {220, kParamFilterType, 0},
  {220, kParamFilterCutoff, 12000},   {220, kParamFilterReso, 6},   {220, kParamFilterEnv, 0},
  {220, kParamFilterDrive, 22},   {220, kParamFiltAttack, 1},   {220, kParamFiltDecay, 200},
  {220, kParamFiltSustain, 100},   {220, kParamFiltRelease, 260},   {220, kParamAmpAttack, 2},
  {220, kParamAmpDecay, 200},   {220, kParamAmpSustain, 100},   {220, kParamAmpRelease, 260},
  {220, kParamVelToAmp, 18},   {220, kParamVelToFilter, 0},   {220, kParamStereoSpread, 40},
  {220, kParamStereoDelay, 0},   {220, kParamTapeSat, 26},   {220, kParamTapeMix, 30},
  {220, kParamIns1Algo, 1},   {220, kParamIns1A, 22},   {220, kParamIns1B, 38},
  {220, kParamIns1C, 28},   {220, kParamIns1Mix, 14},   {220, kParamMstAlgo, 2},
  {220, kParamMstA, 24},   {220, kParamMstB, 18},   {220, kParamMstC, 30},
  {220, kParamMstMix, 10},
  {221, kParamEngine, 0},   {221, kParamPolyMode, 0},   {221, kParamOsc1Shape, 10},
  {221, kParamOsc1Level, 78},   {221, kParamOsc2Shape, 10},   {221, kParamOsc2Pitch, 12},
  {221, kParamOsc2Level, 52},   {221, kParamSubLevel, 64},   {221, kParamNoiseLevel, 2},
  {221, kParamOsc3Enable, 1},   {221, kParamOsc3Source, 0},   {221, kParamOsc3Shape, 10},
  {221, kParamOsc3Level, 38},   {221, kParamOsc3Pitch, 19},   {221, kParamFilterType, 0},
  {221, kParamFilterCutoff, 11000},   {221, kParamFilterReso, 10},   {221, kParamFilterEnv, 0},
  {221, kParamFilterDrive, 20},   {221, kParamFiltAttack, 1},   {221, kParamFiltDecay, 200},
  {221, kParamFiltSustain, 100},   {221, kParamFiltRelease, 220},   {221, kParamAmpAttack, 1},
  {221, kParamAmpDecay, 200},   {221, kParamAmpSustain, 100},   {221, kParamAmpRelease, 220},
  {221, kParamVelToAmp, 18},   {221, kParamVelToFilter, 0},   {221, kParamStereoSpread, 40},
  {221, kParamStereoDelay, 0},   {221, kParamTapeSat, 26},   {221, kParamTapeMix, 30},
  {221, kParamPitchAmount, -16},   {221, kParamPitchAttack, 1},   {221, kParamPitchDecay, 60},
  {221, kParamPitchSustain, 0},   {221, kParamPitchRelease, 120},   {221, kParamIns1Algo, 2},
  {221, kParamIns1A, 14},   {221, kParamIns1B, 22},   {221, kParamIns1C, 26},
  {221, kParamIns1Mix, 14},   {221, kParamMstAlgo, 2},   {221, kParamMstA, 24},
  {221, kParamMstB, 18},   {221, kParamMstC, 30},   {221, kParamMstMix, 10},
  {222, kParamEngine, 0},   {222, kParamPolyMode, 0},   {222, kParamOsc1Shape, 10},
  {222, kParamOsc1Level, 90},   {222, kParamOsc2Shape, 10},   {222, kParamOsc2Pitch, 12},
  {222, kParamOsc2Level, 52},   {222, kParamSubLevel, 64},   {222, kParamNoiseLevel, 2},
  {222, kParamOsc3Enable, 1},   {222, kParamOsc3Source, 0},   {222, kParamOsc3Shape, 10},
  {222, kParamOsc3Level, 38},   {222, kParamOsc3Pitch, 19},   {222, kParamFilterType, 0},
  {222, kParamFilterCutoff, 12500},   {222, kParamFilterReso, 12},   {222, kParamFilterEnv, 0},
  {222, kParamFilterDrive, 34},   {222, kParamFiltAttack, 1},   {222, kParamFiltDecay, 200},
  {222, kParamFiltSustain, 100},   {222, kParamFiltRelease, 280},   {222, kParamAmpAttack, 2},
  {222, kParamAmpDecay, 200},   {222, kParamAmpSustain, 100},   {222, kParamAmpRelease, 280},
  {222, kParamVelToAmp, 18},   {222, kParamVelToFilter, 0},   {222, kParamStereoSpread, 40},
  {222, kParamStereoDelay, 0},   {222, kParamTapeSat, 26},   {222, kParamTapeMix, 30},
  {222, kParamIns1Algo, 2},   {222, kParamIns1A, 20},   {222, kParamIns1B, 26},
  {222, kParamIns1C, 34},   {222, kParamIns1Mix, 18},   {222, kParamMstAlgo, 2},
  {222, kParamMstA, 24},   {222, kParamMstB, 18},   {222, kParamMstC, 30},
  {222, kParamMstMix, 10},
  {223, kParamEngine, 0},   {223, kParamPolyMode, 0},   {223, kParamOsc1Shape, 10},
  {223, kParamOsc1Level, 78},   {223, kParamOsc2Shape, 10},   {223, kParamOsc2Pitch, 12},
  {223, kParamOsc2Level, 52},   {223, kParamSubLevel, 64},   {223, kParamNoiseLevel, 2},
  {223, kParamOsc3Enable, 1},   {223, kParamOsc3Source, 0},   {223, kParamOsc3Shape, 10},
  {223, kParamOsc3Level, 38},   {223, kParamOsc3Pitch, 19},   {223, kParamFilterType, 0},
  {223, kParamFilterCutoff, 9600},   {223, kParamFilterReso, 14},   {223, kParamFilterEnv, 0},
  {223, kParamFilterDrive, 48},   {223, kParamFiltAttack, 1},   {223, kParamFiltDecay, 200},
  {223, kParamFiltSustain, 100},   {223, kParamFiltRelease, 240},   {223, kParamAmpAttack, 2},
  {223, kParamAmpDecay, 200},   {223, kParamAmpSustain, 100},   {223, kParamAmpRelease, 240},
  {223, kParamVelToAmp, 18},   {223, kParamVelToFilter, 0},   {223, kParamStereoSpread, 40},
  {223, kParamStereoDelay, 0},   {223, kParamTapeSat, 40},   {223, kParamTapeMix, 46},
  {223, kParamIns1Algo, 1},   {223, kParamIns1A, 22},   {223, kParamIns1B, 38},
  {223, kParamIns1C, 28},   {223, kParamIns1Mix, 14},   {223, kParamMstAlgo, 2},
  {223, kParamMstA, 24},   {223, kParamMstB, 18},   {223, kParamMstC, 30},
  {223, kParamMstMix, 10},
  {224, kParamEngine, 0},   {224, kParamPolyMode, 0},   {224, kParamOsc1Shape, 52},
  {224, kParamOsc1PW, 26},   {224, kParamOsc1Level, 80},   {224, kParamOsc2Shape, 10},
  {224, kParamOsc2Pitch, 12},   {224, kParamOsc2Level, 40},   {224, kParamSubLevel, 40},
  {224, kParamNoiseLevel, 3},   {224, kParamFilterType, 0},   {224, kParamFilterCutoff, 9200},
  {224, kParamFilterReso, 22},   {224, kParamFilterEnv, 6},   {224, kParamFilterDrive, 14},
  {224, kParamFiltAttack, 1},   {224, kParamFiltDecay, 200},   {224, kParamFiltSustain, 100},
  {224, kParamFiltRelease, 220},   {224, kParamAmpAttack, 2},   {224, kParamAmpDecay, 200},
  {224, kParamAmpSustain, 100},   {224, kParamAmpRelease, 220},   {224, kParamVelToAmp, 16},
  {224, kParamStereoSpread, 48},   {224, kParamTapeSat, 24},   {224, kParamTapeMix, 30},
  {224, kParamIns1Algo, 2},   {224, kParamIns1A, 14},   {224, kParamIns1B, 22},
  {224, kParamIns1C, 26},   {224, kParamIns1Mix, 14},   {224, kParamMstAlgo, 2},
  {224, kParamMstA, 24},   {224, kParamMstB, 18},   {224, kParamMstC, 30},
  {224, kParamMstMix, 10},
  {225, kParamEngine, 0},   {225, kParamPolyMode, 0},   {225, kParamOsc1Shape, 52},
  {225, kParamOsc1PW, 26},   {225, kParamOsc1Level, 80},   {225, kParamOsc2Shape, 10},
  {225, kParamOsc2Pitch, 12},   {225, kParamOsc2Level, 40},   {225, kParamSubLevel, 40},
  {225, kParamNoiseLevel, 3},   {225, kParamFilterType, 0},   {225, kParamFilterCutoff, 8400},
  {225, kParamFilterReso, 30},   {225, kParamFilterEnv, 6},   {225, kParamFilterDrive, 18},
  {225, kParamFiltAttack, 1},   {225, kParamFiltDecay, 200},   {225, kParamFiltSustain, 100},
  {225, kParamFiltRelease, 200},   {225, kParamAmpAttack, 2},   {225, kParamAmpDecay, 200},
  {225, kParamAmpSustain, 100},   {225, kParamAmpRelease, 200},   {225, kParamVelToAmp, 16},
  {225, kParamStereoSpread, 48},   {225, kParamTapeSat, 24},   {225, kParamTapeMix, 30},
  {225, kParamIns1Algo, 2},   {225, kParamIns1A, 20},   {225, kParamIns1B, 26},
  {225, kParamIns1C, 34},   {225, kParamIns1Mix, 18},   {225, kParamMstAlgo, 2},
  {225, kParamMstA, 24},   {225, kParamMstB, 18},   {225, kParamMstC, 30},
  {225, kParamMstMix, 10},
  {226, kParamEngine, 0},   {226, kParamPolyMode, 0},   {226, kParamOsc1Shape, 52},
  {226, kParamOsc1PW, 18},   {226, kParamOsc1Level, 80},   {226, kParamOsc2Shape, 10},
  {226, kParamOsc2Pitch, 12},   {226, kParamOsc2Level, 40},   {226, kParamSubLevel, 40},
  {226, kParamNoiseLevel, 3},   {226, kParamFilterType, 0},   {226, kParamFilterCutoff, 11000},
  {226, kParamFilterReso, 18},   {226, kParamFilterEnv, 6},   {226, kParamFilterDrive, 12},
  {226, kParamFiltAttack, 1},   {226, kParamFiltDecay, 200},   {226, kParamFiltSustain, 100},
  {226, kParamFiltRelease, 200},   {226, kParamAmpAttack, 1},   {226, kParamAmpDecay, 200},
  {226, kParamAmpSustain, 100},   {226, kParamAmpRelease, 200},   {226, kParamVelToAmp, 16},
  {226, kParamStereoSpread, 48},   {226, kParamTapeSat, 24},   {226, kParamTapeMix, 30},
  {226, kParamIns1Algo, 1},   {226, kParamIns1A, 22},   {226, kParamIns1B, 38},
  {226, kParamIns1C, 28},   {226, kParamIns1Mix, 14},   {226, kParamMstAlgo, 2},
  {226, kParamMstA, 24},   {226, kParamMstB, 18},   {226, kParamMstC, 30},
  {226, kParamMstMix, 10},
  {227, kParamEngine, 0},   {227, kParamPolyMode, 0},   {227, kParamOsc1Shape, 28},
  {227, kParamOsc1Level, 52},   {227, kParamOsc2Level, 0},   {227, kParamOsc3Enable, 1},
  {227, kParamOsc3Source, 1},   {227, kParamOsc3Morph, 3.2},   {227, kParamOsc3Level, 84},
  {227, kParamOsc3Pitch, 12},   {227, kParamSubLevel, 24},   {227, kParamNoiseLevel, 10},
  {227, kParamFilterType, 0},   {227, kParamFilterCutoff, 5800},   {227, kParamFilterReso, 14},
  {227, kParamFilterEnv, 18},   {227, kParamFiltAttack, 125.99999999999999},   {227, kParamFiltDecay, 1800},
  {227, kParamFiltSustain, 58},   {227, kParamFiltRelease, 2200},   {227, kParamAmpAttack, 180},
  {227, kParamAmpDecay, 2000},   {227, kParamAmpSustain, 80},   {227, kParamAmpRelease, 2200},
  {227, kParamUnisonVoices, 3},   {227, kParamUnisonDetune, 18},   {227, kParamStereoSpread, 76},
  {227, kParamTapeSat, 30},   {227, kParamTapeWow, 40},   {227, kParamTapeFlutter, 32},
  {227, kParamTapeMix, 42},   {227, kParamIns1Algo, 2},   {227, kParamIns1A, 14},
  {227, kParamIns1B, 22},   {227, kParamIns1C, 26},   {227, kParamIns1Mix, 14},
  {227, kParamMstAlgo, 2},   {227, kParamMstA, 24},   {227, kParamMstB, 18},
  {227, kParamMstC, 30},   {227, kParamMstMix, 10},
  {228, kParamEngine, 0},   {228, kParamPolyMode, 0},   {228, kParamOsc1Shape, 28},
  {228, kParamOsc1Level, 52},   {228, kParamOsc2Level, 0},   {228, kParamOsc3Enable, 1},
  {228, kParamOsc3Source, 1},   {228, kParamOsc3Morph, 2.4},   {228, kParamOsc3Level, 84},
  {228, kParamOsc3Pitch, 12},   {228, kParamSubLevel, 24},   {228, kParamNoiseLevel, 10},
  {228, kParamFilterType, 0},   {228, kParamFilterCutoff, 6400},   {228, kParamFilterReso, 12},
  {228, kParamFilterEnv, 18},   {228, kParamFiltAttack, 182},   {228, kParamFiltDecay, 1800},
  {228, kParamFiltSustain, 58},   {228, kParamFiltRelease, 2400},   {228, kParamAmpAttack, 260},
  {228, kParamAmpDecay, 2000},   {228, kParamAmpSustain, 80},   {228, kParamAmpRelease, 2400},
  {228, kParamUnisonVoices, 3},   {228, kParamUnisonDetune, 18},   {228, kParamStereoSpread, 82},
  {228, kParamTapeSat, 30},   {228, kParamTapeWow, 34},   {228, kParamTapeFlutter, 27.200000000000003},
  {228, kParamTapeMix, 42},   {228, kParamIns1Algo, 2},   {228, kParamIns1A, 20},
  {228, kParamIns1B, 26},   {228, kParamIns1C, 34},   {228, kParamIns1Mix, 18},
  {228, kParamMstAlgo, 2},   {228, kParamMstA, 24},   {228, kParamMstB, 18},
  {228, kParamMstC, 30},   {228, kParamMstMix, 10},
  // bass
  {229, kParamEngine, 0},   {229, kParamPolyMode, 0},   {229, kParamPorta, 0},
  {229, kParamOsc1Shape, 38},   {229, kParamOsc1Level, 88},   {229, kParamOsc2Shape, 36},
  {229, kParamOsc2Pitch, -12},   {229, kParamOsc2Level, 54},   {229, kParamSubLevel, 62},
  {229, kParamNoiseLevel, 2},   {229, kParamFilterType, 0},   {229, kParamFilterCutoff, 1800},
  {229, kParamFilterReso, 26},   {229, kParamFilterDrive, 0},   {229, kParamFilterEnv, 46},
  {229, kParamFiltAttack, 2},   {229, kParamFiltDecay, 700},   {229, kParamFiltSustain, 24},
  {229, kParamFiltRelease, 294},   {229, kParamAmpAttack, 3},   {229, kParamAmpDecay, 700},
  {229, kParamAmpSustain, 40},   {229, kParamAmpRelease, 420},   {229, kParamVelToAmp, 38},
  {229, kParamVelToFilter, 40},   {229, kParamStereoSpread, 0},   {229, kParamStereoDelay, 0},
  {229, kParamTapeSat, 26},   {229, kParamTapeMix, 32},   {229, kParamIns1Algo, 1},
  {229, kParamIns1A, 24},   {229, kParamIns1B, 40},   {229, kParamIns1C, 28},
  {229, kParamIns1Mix, 16},   {229, kParamMstAlgo, 2},   {229, kParamMstA, 24},
  {229, kParamMstB, 18},   {229, kParamMstC, 30},   {229, kParamMstMix, 10},
  {230, kParamEngine, 0},   {230, kParamPolyMode, 0},   {230, kParamPorta, 0},
  {230, kParamOsc1Shape, 38},   {230, kParamOsc1Level, 88},   {230, kParamOsc2Shape, 36},
  {230, kParamOsc2Pitch, -12},   {230, kParamOsc2Level, 54},   {230, kParamSubLevel, 62},
  {230, kParamNoiseLevel, 2},   {230, kParamFilterType, 0},   {230, kParamFilterCutoff, 3600},
  {230, kParamFilterReso, 34},   {230, kParamFilterDrive, 22},   {230, kParamFilterEnv, 46},
  {230, kParamFiltAttack, 2},   {230, kParamFiltDecay, 500},   {230, kParamFiltSustain, 20.4},
  {230, kParamFiltRelease, 266},   {230, kParamAmpAttack, 2},   {230, kParamAmpDecay, 500},
  {230, kParamAmpSustain, 34},   {230, kParamAmpRelease, 380},   {230, kParamVelToAmp, 38},
  {230, kParamVelToFilter, 40},   {230, kParamStereoSpread, 0},   {230, kParamStereoDelay, 0},
  {230, kParamTapeSat, 26},   {230, kParamTapeMix, 32},   {230, kParamIns1Algo, 1},
  {230, kParamIns1A, 32},   {230, kParamIns1B, 46},   {230, kParamIns1C, 34},
  {230, kParamIns1Mix, 22},   {230, kParamMstAlgo, 2},   {230, kParamMstA, 24},
  {230, kParamMstB, 18},   {230, kParamMstC, 30},   {230, kParamMstMix, 10},
  {231, kParamEngine, 0},   {231, kParamPolyMode, 0},   {231, kParamPorta, 0},
  {231, kParamOsc1Shape, 38},   {231, kParamOsc1Level, 88},   {231, kParamOsc2Shape, 36},
  {231, kParamOsc2Pitch, -12},   {231, kParamOsc2Level, 54},   {231, kParamSubLevel, 62},
  {231, kParamNoiseLevel, 2},   {231, kParamFilterType, 0},   {231, kParamFilterCutoff, 1200},
  {231, kParamFilterReso, 22},   {231, kParamFilterDrive, 0},   {231, kParamFilterEnv, 46},
  {231, kParamFiltAttack, 2},   {231, kParamFiltDecay, 900},   {231, kParamFiltSustain, 28.799999999999997},
  {231, kParamFiltRelease, 364},   {231, kParamAmpAttack, 5},   {231, kParamAmpDecay, 900},
  {231, kParamAmpSustain, 48},   {231, kParamAmpRelease, 520},   {231, kParamVelToAmp, 38},
  {231, kParamVelToFilter, 40},   {231, kParamStereoSpread, 0},   {231, kParamStereoDelay, 0},
  {231, kParamTapeSat, 26},   {231, kParamTapeMix, 32},   {231, kParamIns1Algo, 4},
  {231, kParamIns1A, 18},   {231, kParamIns1B, 28},   {231, kParamIns1C, 10},
  {231, kParamIns1Mix, 12},   {231, kParamMstAlgo, 2},   {231, kParamMstA, 24},
  {231, kParamMstB, 18},   {231, kParamMstC, 30},   {231, kParamMstMix, 10},
  {232, kParamEngine, 0},   {232, kParamPolyMode, 0},   {232, kParamPorta, 0},
  {232, kParamOsc1Shape, 38},   {232, kParamOsc1Level, 88},   {232, kParamOsc2Shape, 36},
  {232, kParamOsc2Pitch, -12},   {232, kParamOsc2Level, 54},   {232, kParamSubLevel, 62},
  {232, kParamNoiseLevel, 2},   {232, kParamFilterType, 0},   {232, kParamFilterCutoff, 2800},
  {232, kParamFilterReso, 44},   {232, kParamFilterDrive, 58},   {232, kParamFilterEnv, 46},
  {232, kParamFiltAttack, 2},   {232, kParamFiltDecay, 480},   {232, kParamFiltSustain, 18},
  {232, kParamFiltRelease, 251.99999999999997},   {232, kParamAmpAttack, 2},   {232, kParamAmpDecay, 480},
  {232, kParamAmpSustain, 30},   {232, kParamAmpRelease, 360},   {232, kParamVelToAmp, 38},
  {232, kParamVelToFilter, 40},   {232, kParamStereoSpread, 0},   {232, kParamStereoDelay, 0},
  {232, kParamTapeSat, 40},   {232, kParamTapeMix, 32},   {232, kParamIns1Algo, 1},
  {232, kParamIns1A, 24},   {232, kParamIns1B, 40},   {232, kParamIns1C, 28},
  {232, kParamIns1Mix, 16},   {232, kParamMstAlgo, 2},   {232, kParamMstA, 24},
  {232, kParamMstB, 18},   {232, kParamMstC, 30},   {232, kParamMstMix, 10},
  {233, kParamEngine, 0},   {233, kParamPolyMode, 0},   {233, kParamPorta, 0},
  {233, kParamOsc1Shape, 38},   {233, kParamOsc1Level, 88},   {233, kParamOsc2Shape, 36},
  {233, kParamOsc2Pitch, -12},   {233, kParamOsc2Level, 54},   {233, kParamSubLevel, 62},
  {233, kParamNoiseLevel, 2},   {233, kParamFilterType, 0},   {233, kParamFilterCutoff, 2200},
  {233, kParamFilterReso, 28},   {233, kParamFilterDrive, 0},   {233, kParamFilterEnv, 46},
  {233, kParamFiltAttack, 2},   {233, kParamFiltDecay, 650},   {233, kParamFiltSustain, 25.2},
  {233, kParamFiltRelease, 315},   {233, kParamAmpAttack, 4},   {233, kParamAmpDecay, 650},
  {233, kParamAmpSustain, 42},   {233, kParamAmpRelease, 450},   {233, kParamVelToAmp, 38},
  {233, kParamVelToFilter, 40},   {233, kParamStereoSpread, 0},   {233, kParamStereoDelay, 0},
  {233, kParamTapeSat, 26},   {233, kParamTapeMix, 44},   {233, kParamTapeWow, 40},
  {233, kParamTapeFlutter, 30},   {233, kParamIns1Algo, 1},   {233, kParamIns1A, 32},
  {233, kParamIns1B, 46},   {233, kParamIns1C, 34},   {233, kParamIns1Mix, 22},
  {233, kParamMstAlgo, 2},   {233, kParamMstA, 24},   {233, kParamMstB, 18},
  {233, kParamMstC, 30},   {233, kParamMstMix, 10},
  // guitars
  {234, kParamEngine, 0},   {234, kParamPolyMode, 0},   {234, kParamOsc1Shape, 44},
  {234, kParamOsc1PW, 34},   {234, kParamOsc1Level, 84},   {234, kParamOsc2Shape, 38},
  {234, kParamOsc2Pitch, 12},   {234, kParamOsc2Level, 38},   {234, kParamSubLevel, 20},
  {234, kParamNoiseLevel, 8},   {234, kParamFilterType, 0},   {234, kParamFilterCutoff, 7200},
  {234, kParamFilterReso, 12},   {234, kParamFilterDrive, 0},   {234, kParamFilterEnv, 62},
  {234, kParamFilterKeyTrack, 52},   {234, kParamFiltAttack, 1},   {234, kParamFiltDecay, 700},
  {234, kParamFiltSustain, 7.699999999999999},   {234, kParamFiltRelease, 800},   {234, kParamAmpAttack, 2},
  {234, kParamAmpDecay, 1400},   {234, kParamAmpSustain, 22},   {234, kParamAmpRelease, 1600},
  {234, kParamVelToAmp, 54},   {234, kParamVelToFilter, 46},   {234, kParamStereoSpread, 44},
  {234, kParamStereoDelay, 12},   {234, kParamTapeSat, 20},   {234, kParamTapeMix, 26},
  {234, kParamIns1Algo, 2},   {234, kParamIns1A, 14},   {234, kParamIns1B, 24},
  {234, kParamIns1C, 30},   {234, kParamIns1Mix, 16},   {234, kParamMstAlgo, 2},
  {234, kParamMstA, 24},   {234, kParamMstB, 18},   {234, kParamMstC, 30},
  {234, kParamMstMix, 10},
  {235, kParamEngine, 0},   {235, kParamPolyMode, 0},   {235, kParamOsc1Shape, 32},
  {235, kParamOsc1PW, 42},   {235, kParamOsc1Level, 84},   {235, kParamOsc2Shape, 38},
  {235, kParamOsc2Pitch, 12},   {235, kParamOsc2Level, 38},   {235, kParamSubLevel, 20},
  {235, kParamNoiseLevel, 8},   {235, kParamFilterType, 0},   {235, kParamFilterCutoff, 5400},
  {235, kParamFilterReso, 10},   {235, kParamFilterDrive, 0},   {235, kParamFilterEnv, 62},
  {235, kParamFilterKeyTrack, 52},   {235, kParamFiltAttack, 1},   {235, kParamFiltDecay, 600},
  {235, kParamFiltSustain, 9.1},   {235, kParamFiltRelease, 700},   {235, kParamAmpAttack, 3},
  {235, kParamAmpDecay, 1200},   {235, kParamAmpSustain, 26},   {235, kParamAmpRelease, 1400},
  {235, kParamVelToAmp, 54},   {235, kParamVelToFilter, 46},   {235, kParamStereoSpread, 44},
  {235, kParamStereoDelay, 12},   {235, kParamTapeSat, 20},   {235, kParamTapeMix, 26},
  {235, kParamIns1Algo, 2},   {235, kParamIns1A, 20},   {235, kParamIns1B, 30},
  {235, kParamIns1C, 38},   {235, kParamIns1Mix, 20},   {235, kParamMstAlgo, 2},
  {235, kParamMstA, 24},   {235, kParamMstB, 18},   {235, kParamMstC, 30},
  {235, kParamMstMix, 10},
  {236, kParamEngine, 0},   {236, kParamPolyMode, 0},   {236, kParamOsc1Shape, 46},
  {236, kParamOsc1PW, 28},   {236, kParamOsc1Level, 84},   {236, kParamOsc2Shape, 38},
  {236, kParamOsc2Pitch, 12},   {236, kParamOsc2Level, 38},   {236, kParamSubLevel, 20},
  {236, kParamNoiseLevel, 8},   {236, kParamFilterType, 0},   {236, kParamFilterCutoff, 8600},
  {236, kParamFilterReso, 14},   {236, kParamFilterDrive, 0},   {236, kParamFilterEnv, 62},
  {236, kParamFilterKeyTrack, 52},   {236, kParamFiltAttack, 1},   {236, kParamFiltDecay, 550},
  {236, kParamFiltSustain, 6.3},   {236, kParamFiltRelease, 650},   {236, kParamAmpAttack, 1},
  {236, kParamAmpDecay, 1100},   {236, kParamAmpSustain, 18},   {236, kParamAmpRelease, 1300},
  {236, kParamVelToAmp, 54},   {236, kParamVelToFilter, 46},   {236, kParamStereoSpread, 44},
  {236, kParamStereoDelay, 12},   {236, kParamTapeSat, 20},   {236, kParamTapeMix, 26},
  {236, kParamIns1Algo, 3},   {236, kParamIns1A, 26},   {236, kParamIns1B, 22},
  {236, kParamIns1C, 44},   {236, kParamIns1Mix, 12},   {236, kParamMstAlgo, 2},
  {236, kParamMstA, 24},   {236, kParamMstB, 18},   {236, kParamMstC, 30},
  {236, kParamMstMix, 10},
  {237, kParamEngine, 0},   {237, kParamPolyMode, 0},   {237, kParamOsc1Shape, 32},
  {237, kParamOsc1PW, 46},   {237, kParamOsc1Level, 84},   {237, kParamOsc2Shape, 38},
  {237, kParamOsc2Pitch, 12},   {237, kParamOsc2Level, 38},   {237, kParamSubLevel, 20},
  {237, kParamNoiseLevel, 8},   {237, kParamFilterType, 0},   {237, kParamFilterCutoff, 4600},
  {237, kParamFilterReso, 12},   {237, kParamFilterDrive, 0},   {237, kParamFilterEnv, 62},
  {237, kParamFilterKeyTrack, 52},   {237, kParamFiltAttack, 1},   {237, kParamFiltDecay, 800},
  {237, kParamFiltSustain, 10.5},   {237, kParamFiltRelease, 900},   {237, kParamAmpAttack, 4},
  {237, kParamAmpDecay, 1600},   {237, kParamAmpSustain, 30},   {237, kParamAmpRelease, 1800},
  {237, kParamVelToAmp, 54},   {237, kParamVelToFilter, 46},   {237, kParamStereoSpread, 44},
  {237, kParamStereoDelay, 12},   {237, kParamTapeSat, 20},   {237, kParamTapeMix, 26},
  {237, kParamIns1Algo, 2},   {237, kParamIns1A, 14},   {237, kParamIns1B, 24},
  {237, kParamIns1C, 30},   {237, kParamIns1Mix, 16},   {237, kParamMstAlgo, 2},
  {237, kParamMstA, 24},   {237, kParamMstB, 18},   {237, kParamMstC, 30},
  {237, kParamMstMix, 10},
  {238, kParamEngine, 0},   {238, kParamPolyMode, 0},   {238, kParamOsc1Shape, 38},
  {238, kParamOsc1PW, 38},   {238, kParamOsc1Level, 84},   {238, kParamOsc2Shape, 38},
  {238, kParamOsc2Pitch, 12},   {238, kParamOsc2Level, 38},   {238, kParamSubLevel, 20},
  {238, kParamNoiseLevel, 8},   {238, kParamFilterType, 0},   {238, kParamFilterCutoff, 5600},
  {238, kParamFilterReso, 12},   {238, kParamFilterDrive, 0},   {238, kParamFilterEnv, 62},
  {238, kParamFilterKeyTrack, 52},   {238, kParamFiltAttack, 1},   {238, kParamFiltDecay, 650},
  {238, kParamFiltSustain, 9.1},   {238, kParamFiltRelease, 750},   {238, kParamAmpAttack, 3},
  {238, kParamAmpDecay, 1300},   {238, kParamAmpSustain, 26},   {238, kParamAmpRelease, 1500},
  {238, kParamVelToAmp, 54},   {238, kParamVelToFilter, 46},   {238, kParamStereoSpread, 44},
  {238, kParamStereoDelay, 12},   {238, kParamTapeSat, 20},   {238, kParamTapeMix, 42},
  {238, kParamTapeWow, 38},   {238, kParamTapeFlutter, 26},   {238, kParamIns1Algo, 2},
  {238, kParamIns1A, 20},   {238, kParamIns1B, 30},   {238, kParamIns1C, 38},
  {238, kParamIns1Mix, 20},   {238, kParamMstAlgo, 2},   {238, kParamMstA, 24},
  {238, kParamMstB, 18},   {238, kParamMstC, 30},   {238, kParamMstMix, 10},
  {239, kParamEngine, 0},   {239, kParamPolyMode, 0},   {239, kParamOsc1Shape, 36},
  {239, kParamOsc1PW, 40},   {239, kParamOsc1Level, 84},   {239, kParamOsc2Shape, 38},
  {239, kParamOsc2Pitch, 12},   {239, kParamOsc2Level, 38},   {239, kParamSubLevel, 20},
  {239, kParamNoiseLevel, 8},   {239, kParamFilterType, 0},   {239, kParamFilterCutoff, 6200},
  {239, kParamFilterReso, 10},   {239, kParamFilterDrive, 0},   {239, kParamFilterEnv, 62},
  {239, kParamFilterKeyTrack, 52},   {239, kParamFiltAttack, 1},   {239, kParamFiltDecay, 750},
  {239, kParamFiltSustain, 9.799999999999999},   {239, kParamFiltRelease, 850},   {239, kParamAmpAttack, 3},
  {239, kParamAmpDecay, 1500},   {239, kParamAmpSustain, 28},   {239, kParamAmpRelease, 1700},
  {239, kParamVelToAmp, 54},   {239, kParamVelToFilter, 58},   {239, kParamStereoSpread, 44},
  {239, kParamStereoDelay, 12},   {239, kParamTapeSat, 20},   {239, kParamTapeMix, 26},
  {239, kParamIns1Algo, 3},   {239, kParamIns1A, 26},   {239, kParamIns1B, 22},
  {239, kParamIns1C, 44},   {239, kParamIns1Mix, 12},   {239, kParamMstAlgo, 2},
  {239, kParamMstA, 24},   {239, kParamMstB, 18},   {239, kParamMstC, 30},
  {239, kParamMstMix, 10},
  {240, kParamEngine, 0},   {240, kParamPolyMode, 0},   {240, kParamOsc1Shape, 44},
  {240, kParamOsc1PW, 30},   {240, kParamOsc1Level, 84},   {240, kParamOsc2Shape, 38},
  {240, kParamOsc2Pitch, 12},   {240, kParamOsc2Level, 56},   {240, kParamSubLevel, 20},
  {240, kParamNoiseLevel, 8},   {240, kParamFilterType, 0},   {240, kParamFilterCutoff, 7600},
  {240, kParamFilterReso, 16},   {240, kParamFilterDrive, 0},   {240, kParamFilterEnv, 62},
  {240, kParamFilterKeyTrack, 52},   {240, kParamFiltAttack, 1},   {240, kParamFiltDecay, 650},
  {240, kParamFiltSustain, 8.399999999999999},   {240, kParamFiltRelease, 750},   {240, kParamAmpAttack, 2},
  {240, kParamAmpDecay, 1300},   {240, kParamAmpSustain, 24},   {240, kParamAmpRelease, 1500},
  {240, kParamVelToAmp, 54},   {240, kParamVelToFilter, 46},   {240, kParamStereoSpread, 68},
  {240, kParamStereoDelay, 12},   {240, kParamTapeSat, 20},   {240, kParamTapeMix, 26},
  {240, kParamIns1Algo, 2},   {240, kParamIns1A, 14},   {240, kParamIns1B, 24},
  {240, kParamIns1C, 30},   {240, kParamIns1Mix, 16},   {240, kParamMstAlgo, 2},
  {240, kParamMstA, 24},   {240, kParamMstB, 18},   {240, kParamMstC, 30},
  {240, kParamMstMix, 10},
  {241, kParamEngine, 0},   {241, kParamPolyMode, 0},   {241, kParamOsc1Shape, 34},
  {241, kParamOsc1PW, 44},   {241, kParamOsc1Level, 84},   {241, kParamOsc2Shape, 38},
  {241, kParamOsc2Pitch, 12},   {241, kParamOsc2Level, 38},   {241, kParamSubLevel, 20},
  {241, kParamNoiseLevel, 8},   {241, kParamFilterType, 0},   {241, kParamFilterCutoff, 4200},
  {241, kParamFilterReso, 20},   {241, kParamFilterDrive, 0},   {241, kParamFilterEnv, 74},
  {241, kParamFilterKeyTrack, 52},   {241, kParamFiltAttack, 1},   {241, kParamFiltDecay, 350},
  {241, kParamFiltSustain, 4.199999999999999},   {241, kParamFiltRelease, 450},   {241, kParamAmpAttack, 1},
  {241, kParamAmpDecay, 700},   {241, kParamAmpSustain, 12},   {241, kParamAmpRelease, 700},
  {241, kParamVelToAmp, 54},   {241, kParamVelToFilter, 46},   {241, kParamStereoSpread, 44},
  {241, kParamStereoDelay, 12},   {241, kParamTapeSat, 20},   {241, kParamTapeMix, 26},
  {241, kParamIns1Algo, 2},   {241, kParamIns1A, 20},   {241, kParamIns1B, 30},
  {241, kParamIns1C, 38},   {241, kParamIns1Mix, 20},   {241, kParamMstAlgo, 2},
  {241, kParamMstA, 24},   {241, kParamMstB, 18},   {241, kParamMstC, 30},
  {241, kParamMstMix, 10},
  {242, kParamEngine, 0},   {242, kParamPolyMode, 0},   {242, kParamOsc1Shape, 44},
  {242, kParamOsc1PW, 32},   {242, kParamOsc1Level, 84},   {242, kParamOsc2Shape, 38},
  {242, kParamOsc2Pitch, 12},   {242, kParamOsc2Level, 38},   {242, kParamSubLevel, 20},
  {242, kParamNoiseLevel, 8},   {242, kParamFilterType, 0},   {242, kParamFilterCutoff, 6800},
  {242, kParamFilterReso, 16},   {242, kParamFilterDrive, 0},   {242, kParamFilterEnv, 62},
  {242, kParamFilterKeyTrack, 52},   {242, kParamFiltAttack, 1},   {242, kParamFiltDecay, 650},
  {242, kParamFiltSustain, 8.399999999999999},   {242, kParamFiltRelease, 750},   {242, kParamAmpAttack, 2},
  {242, kParamAmpDecay, 1300},   {242, kParamAmpSustain, 24},   {242, kParamAmpRelease, 1500},
  {242, kParamVelToAmp, 54},   {242, kParamVelToFilter, 46},   {242, kParamStereoSpread, 44},
  {242, kParamStereoDelay, 12},   {242, kParamTapeSat, 20},   {242, kParamTapeMix, 26},
  {242, kParamIns1Algo, 3},   {242, kParamIns1A, 26},   {242, kParamIns1B, 22},
  {242, kParamIns1C, 44},   {242, kParamIns1Mix, 12},   {242, kParamMstAlgo, 2},
  {242, kParamMstA, 24},   {242, kParamMstB, 18},   {242, kParamMstC, 30},
  {242, kParamMstMix, 10},
  {243, kParamEngine, 0},   {243, kParamPolyMode, 0},   {243, kParamOsc1Shape, 46},
  {243, kParamOsc1PW, 30},   {243, kParamOsc1Level, 84},   {243, kParamOsc2Shape, 38},
  {243, kParamOsc2Pitch, 12},   {243, kParamOsc2Level, 38},   {243, kParamSubLevel, 20},
  {243, kParamNoiseLevel, 8},   {243, kParamFilterType, 0},   {243, kParamFilterCutoff, 6200},
  {243, kParamFilterReso, 24},   {243, kParamFilterDrive, 34},   {243, kParamFilterEnv, 62},
  {243, kParamFilterKeyTrack, 52},   {243, kParamFiltAttack, 1},   {243, kParamFiltDecay, 600},
  {243, kParamFiltSustain, 9.1},   {243, kParamFiltRelease, 700},   {243, kParamAmpAttack, 2},
  {243, kParamAmpDecay, 1200},   {243, kParamAmpSustain, 26},   {243, kParamAmpRelease, 1400},
  {243, kParamVelToAmp, 54},   {243, kParamVelToFilter, 46},   {243, kParamStereoSpread, 44},
  {243, kParamStereoDelay, 12},   {243, kParamTapeSat, 20},   {243, kParamTapeMix, 26},
  {243, kParamIns1Algo, 2},   {243, kParamIns1A, 14},   {243, kParamIns1B, 24},
  {243, kParamIns1C, 30},   {243, kParamIns1Mix, 16},   {243, kParamMstAlgo, 2},
  {243, kParamMstA, 24},   {243, kParamMstB, 18},   {243, kParamMstC, 30},
  {243, kParamMstMix, 10},
  {244, kParamEngine, 0},   {244, kParamPolyMode, 0},   {244, kParamOsc1Shape, 48},
  {244, kParamOsc1PW, 26},   {244, kParamOsc1Level, 84},   {244, kParamOsc2Shape, 38},
  {244, kParamOsc2Pitch, 12},   {244, kParamOsc2Level, 38},   {244, kParamSubLevel, 20},
  {244, kParamNoiseLevel, 8},   {244, kParamFilterType, 0},   {244, kParamFilterCutoff, 5600},
  {244, kParamFilterReso, 30},   {244, kParamFilterDrive, 64},   {244, kParamFilterEnv, 48},
  {244, kParamFilterKeyTrack, 52},   {244, kParamFiltAttack, 1},   {244, kParamFiltDecay, 500},
  {244, kParamFiltSustain, 10.5},   {244, kParamFiltRelease, 650},   {244, kParamAmpAttack, 1},
  {244, kParamAmpDecay, 1000},   {244, kParamAmpSustain, 30},   {244, kParamAmpRelease, 1300},
  {244, kParamVelToAmp, 54},   {244, kParamVelToFilter, 46},   {244, kParamStereoSpread, 44},
  {244, kParamStereoDelay, 12},   {244, kParamTapeSat, 20},   {244, kParamTapeMix, 26},
  {244, kParamIns1Algo, 2},   {244, kParamIns1A, 20},   {244, kParamIns1B, 30},
  {244, kParamIns1C, 38},   {244, kParamIns1Mix, 20},   {244, kParamMstAlgo, 2},
  {244, kParamMstA, 24},   {244, kParamMstB, 18},   {244, kParamMstC, 30},
  {244, kParamMstMix, 10},
  {245, kParamEngine, 0},   {245, kParamPolyMode, 0},   {245, kParamOsc1Shape, 40},
  {245, kParamOsc1PW, 36},   {245, kParamOsc1Level, 84},   {245, kParamOsc2Shape, 38},
  {245, kParamOsc2Pitch, 12},   {245, kParamOsc2Level, 38},   {245, kParamSubLevel, 20},
  {245, kParamNoiseLevel, 8},   {245, kParamFilterType, 0},   {245, kParamFilterCutoff, 5200},
  {245, kParamFilterReso, 18},   {245, kParamFilterDrive, 18},   {245, kParamFilterEnv, 62},
  {245, kParamFilterKeyTrack, 52},   {245, kParamFiltAttack, 1},   {245, kParamFiltDecay, 750},
  {245, kParamFiltSustain, 9.799999999999999},   {245, kParamFiltRelease, 850},   {245, kParamAmpAttack, 3},
  {245, kParamAmpDecay, 1500},   {245, kParamAmpSustain, 28},   {245, kParamAmpRelease, 1700},
  {245, kParamVelToAmp, 54},   {245, kParamVelToFilter, 46},   {245, kParamStereoSpread, 44},
  {245, kParamStereoDelay, 12},   {245, kParamTapeSat, 20},   {245, kParamTapeMix, 26},
  {245, kParamIns1Algo, 3},   {245, kParamIns1A, 26},   {245, kParamIns1B, 22},
  {245, kParamIns1C, 44},   {245, kParamIns1Mix, 12},   {245, kParamMstAlgo, 2},
  {245, kParamMstA, 24},   {245, kParamMstB, 18},   {245, kParamMstC, 30},
  {245, kParamMstMix, 10},
  {246, kParamEngine, 0},   {246, kParamPolyMode, 0},   {246, kParamOsc1Shape, 42},
  {246, kParamOsc1PW, 34},   {246, kParamOsc1Level, 84},   {246, kParamOsc2Shape, 38},
  {246, kParamOsc2Pitch, 12},   {246, kParamOsc2Level, 38},   {246, kParamSubLevel, 20},
  {246, kParamNoiseLevel, 8},   {246, kParamFilterType, 0},   {246, kParamFilterCutoff, 4400},
  {246, kParamFilterReso, 26},   {246, kParamFilterDrive, 40},   {246, kParamFilterEnv, 78},
  {246, kParamFilterKeyTrack, 52},   {246, kParamFiltAttack, 1},   {246, kParamFiltDecay, 300},
  {246, kParamFiltSustain, 3.5},   {246, kParamFiltRelease, 400},   {246, kParamAmpAttack, 1},
  {246, kParamAmpDecay, 600},   {246, kParamAmpSustain, 10},   {246, kParamAmpRelease, 600},
  {246, kParamVelToAmp, 54},   {246, kParamVelToFilter, 46},   {246, kParamStereoSpread, 44},
  {246, kParamStereoDelay, 12},   {246, kParamTapeSat, 20},   {246, kParamTapeMix, 26},
  {246, kParamIns1Algo, 2},   {246, kParamIns1A, 14},   {246, kParamIns1B, 24},
  {246, kParamIns1C, 30},   {246, kParamIns1Mix, 16},   {246, kParamMstAlgo, 2},
  {246, kParamMstA, 24},   {246, kParamMstB, 18},   {246, kParamMstC, 30},
  {246, kParamMstMix, 10},
  {247, kParamEngine, 0},   {247, kParamPolyMode, 0},   {247, kParamOsc1Shape, 44},
  {247, kParamOsc1PW, 32},   {247, kParamOsc1Level, 84},   {247, kParamOsc2Shape, 38},
  {247, kParamOsc2Pitch, 12},   {247, kParamOsc2Level, 38},   {247, kParamSubLevel, 20},
  {247, kParamNoiseLevel, 8},   {247, kParamFilterType, 0},   {247, kParamFilterCutoff, 6400},
  {247, kParamFilterReso, 18},   {247, kParamFilterDrive, 24},   {247, kParamFilterEnv, 62},
  {247, kParamFilterKeyTrack, 52},   {247, kParamFiltAttack, 1},   {247, kParamFiltDecay, 650},
  {247, kParamFiltSustain, 8.399999999999999},   {247, kParamFiltRelease, 750},   {247, kParamAmpAttack, 2},
  {247, kParamAmpDecay, 1300},   {247, kParamAmpSustain, 24},   {247, kParamAmpRelease, 1500},
  {247, kParamVelToAmp, 54},   {247, kParamVelToFilter, 46},   {247, kParamStereoSpread, 44},
  {247, kParamStereoDelay, 12},   {247, kParamTapeSat, 20},   {247, kParamTapeMix, 26},
  {247, kParamLfoRate, 4.6},   {247, kParamLfoDest, 1},   {247, kParamLfoAmount, 42},
  {247, kParamIns1Algo, 2},   {247, kParamIns1A, 20},   {247, kParamIns1B, 30},
  {247, kParamIns1C, 38},   {247, kParamIns1Mix, 20},   {247, kParamMstAlgo, 2},
  {247, kParamMstA, 24},   {247, kParamMstB, 18},   {247, kParamMstC, 30},
  {247, kParamMstMix, 10},
  {248, kParamEngine, 0},   {248, kParamPolyMode, 0},   {248, kParamOsc1Shape, 44},
  {248, kParamOsc1PW, 32},   {248, kParamOsc1Level, 84},   {248, kParamOsc2Shape, 38},
  {248, kParamOsc2Pitch, 12},   {248, kParamOsc2Level, 38},   {248, kParamSubLevel, 20},
  {248, kParamNoiseLevel, 8},   {248, kParamFilterType, 0},   {248, kParamFilterCutoff, 6000},
  {248, kParamFilterReso, 20},   {248, kParamFilterDrive, 28},   {248, kParamFilterEnv, 62},
  {248, kParamFilterKeyTrack, 52},   {248, kParamFiltAttack, 1},   {248, kParamFiltDecay, 600},
  {248, kParamFiltSustain, 9.1},   {248, kParamFiltRelease, 700},   {248, kParamAmpAttack, 2},
  {248, kParamAmpDecay, 1200},   {248, kParamAmpSustain, 26},   {248, kParamAmpRelease, 1400},
  {248, kParamVelToAmp, 54},   {248, kParamVelToFilter, 46},   {248, kParamStereoSpread, 44},
  {248, kParamStereoDelay, 12},   {248, kParamTapeSat, 20},   {248, kParamTapeMix, 44},
  {248, kParamTapeWow, 40},   {248, kParamTapeFlutter, 30},   {248, kParamIns1Algo, 3},
  {248, kParamIns1A, 26},   {248, kParamIns1B, 22},   {248, kParamIns1C, 44},
  {248, kParamIns1Mix, 12},   {248, kParamMstAlgo, 2},   {248, kParamMstA, 24},
  {248, kParamMstB, 18},   {248, kParamMstC, 30},   {248, kParamMstMix, 10},
  // reeds/horn
  {249, kParamEngine, 0},   {249, kParamPolyMode, 0},   {249, kParamPorta, 0},
  {249, kParamOsc1Shape, 54},   {249, kParamOsc1PW, 28},   {249, kParamOsc1Level, 82},
  {249, kParamOsc2Shape, 52},   {249, kParamOsc2Pitch, -12},   {249, kParamOsc2Level, 44},
  {249, kParamSubLevel, 26},   {249, kParamNoiseLevel, 32},   {249, kParamFilterType, 0},
  {249, kParamFilterCutoff, 5600},   {249, kParamFilterReso, 26},   {249, kParamFilterEnv, 38},
  {249, kParamFiltAttack, 12},   {249, kParamFiltDecay, 1100},   {249, kParamFiltSustain, 52},
  {249, kParamFiltRelease, 900},   {249, kParamAmpAttack, 30},   {249, kParamAmpDecay, 1200},
  {249, kParamAmpSustain, 76},   {249, kParamAmpRelease, 900},   {249, kParamVelToAmp, 46},
  {249, kParamVelToFilter, 30},   {249, kParamStereoSpread, 44},   {249, kParamStereoDelay, 20},
  {249, kParamTapeSat, 18},   {249, kParamTapeMix, 26},   {249, kParamIns1Algo, 2},
  {249, kParamIns1A, 16},   {249, kParamIns1B, 20},   {249, kParamIns1C, 28},
  {249, kParamIns1Mix, 12},   {249, kParamMstAlgo, 2},   {249, kParamMstA, 24},
  {249, kParamMstB, 18},   {249, kParamMstC, 30},   {249, kParamMstMix, 10},
  {250, kParamEngine, 0},   {250, kParamPolyMode, 0},   {250, kParamPorta, 0},
  {250, kParamOsc1Shape, 54},   {250, kParamOsc1PW, 50},   {250, kParamOsc1Level, 82},
  {250, kParamOsc2Shape, 52},   {250, kParamOsc2Pitch, -12},   {250, kParamOsc2Level, 44},
  {250, kParamSubLevel, 26},   {250, kParamNoiseLevel, 52},   {250, kParamFilterType, 0},
  {250, kParamFilterCutoff, 7200},   {250, kParamFilterReso, 18},   {250, kParamFilterEnv, 38},
  {250, kParamFiltAttack, 12},   {250, kParamFiltDecay, 1100},   {250, kParamFiltSustain, 52},
  {250, kParamFiltRelease, 1000},   {250, kParamAmpAttack, 40},   {250, kParamAmpDecay, 1200},
  {250, kParamAmpSustain, 76},   {250, kParamAmpRelease, 1000},   {250, kParamVelToAmp, 46},
  {250, kParamVelToFilter, 30},   {250, kParamStereoSpread, 44},   {250, kParamStereoDelay, 20},
  {250, kParamTapeSat, 18},   {250, kParamTapeMix, 26},   {250, kParamIns1Algo, 2},
  {250, kParamIns1A, 22},   {250, kParamIns1B, 24},   {250, kParamIns1C, 34},
  {250, kParamIns1Mix, 18},   {250, kParamMstAlgo, 2},   {250, kParamMstA, 24},
  {250, kParamMstB, 18},   {250, kParamMstC, 30},   {250, kParamMstMix, 10},
  {251, kParamEngine, 0},   {251, kParamPolyMode, 0},   {251, kParamPorta, 0},
  {251, kParamOsc1Shape, 54},   {251, kParamOsc1PW, 18},   {251, kParamOsc1Level, 82},
  {251, kParamOsc2Shape, 52},   {251, kParamOsc2Pitch, -12},   {251, kParamOsc2Level, 44},
  {251, kParamSubLevel, 26},   {251, kParamNoiseLevel, 18},   {251, kParamFilterType, 0},
  {251, kParamFilterCutoff, 6400},   {251, kParamFilterReso, 32},   {251, kParamFilterEnv, 38},
  {251, kParamFiltAttack, 12},   {251, kParamFiltDecay, 1100},   {251, kParamFiltSustain, 52},
  {251, kParamFiltRelease, 850},   {251, kParamAmpAttack, 24},   {251, kParamAmpDecay, 1200},
  {251, kParamAmpSustain, 76},   {251, kParamAmpRelease, 850},   {251, kParamVelToAmp, 46},
  {251, kParamVelToFilter, 30},   {251, kParamStereoSpread, 44},   {251, kParamStereoDelay, 20},
  {251, kParamTapeSat, 18},   {251, kParamTapeMix, 26},   {251, kParamIns1Algo, 1},
  {251, kParamIns1A, 18},   {251, kParamIns1B, 34},   {251, kParamIns1C, 26},
  {251, kParamIns1Mix, 12},   {251, kParamMstAlgo, 2},   {251, kParamMstA, 24},
  {251, kParamMstB, 18},   {251, kParamMstC, 30},   {251, kParamMstMix, 10},
  {252, kParamEngine, 0},   {252, kParamPolyMode, 0},   {252, kParamPorta, 0},
  {252, kParamOsc1Shape, 54},   {252, kParamOsc1PW, 22},   {252, kParamOsc1Level, 82},
  {252, kParamOsc2Shape, 52},   {252, kParamOsc2Pitch, -12},   {252, kParamOsc2Level, 44},
  {252, kParamSubLevel, 26},   {252, kParamNoiseLevel, 14},   {252, kParamFilterType, 0},
  {252, kParamFilterCutoff, 5000},   {252, kParamFilterReso, 30},   {252, kParamFilterEnv, 38},
  {252, kParamFiltAttack, 12},   {252, kParamFiltDecay, 1100},   {252, kParamFiltSustain, 52},
  {252, kParamFiltRelease, 900},   {252, kParamAmpAttack, 28},   {252, kParamAmpDecay, 1200},
  {252, kParamAmpSustain, 76},   {252, kParamAmpRelease, 900},   {252, kParamVelToAmp, 46},
  {252, kParamVelToFilter, 30},   {252, kParamStereoSpread, 44},   {252, kParamStereoDelay, 20},
  {252, kParamTapeSat, 18},   {252, kParamTapeMix, 26},   {252, kParamIns1Algo, 2},
  {252, kParamIns1A, 16},   {252, kParamIns1B, 20},   {252, kParamIns1C, 28},
  {252, kParamIns1Mix, 12},   {252, kParamMstAlgo, 2},   {252, kParamMstA, 24},
  {252, kParamMstB, 18},   {252, kParamMstC, 30},   {252, kParamMstMix, 10},
  {253, kParamEngine, 0},   {253, kParamPolyMode, 0},   {253, kParamPorta, 0},
  {253, kParamOsc1Shape, 54},   {253, kParamOsc1PW, 50},   {253, kParamOsc1Level, 82},
  {253, kParamOsc2Shape, 52},   {253, kParamOsc2Pitch, -12},   {253, kParamOsc2Level, 44},
  {253, kParamSubLevel, 26},   {253, kParamNoiseLevel, 64},   {253, kParamFilterType, 0},
  {253, kParamFilterCutoff, 6800},   {253, kParamFilterReso, 20},   {253, kParamFilterEnv, 38},
  {253, kParamFiltAttack, 12},   {253, kParamFiltDecay, 1100},   {253, kParamFiltSustain, 52},
  {253, kParamFiltRelease, 1200},   {253, kParamAmpAttack, 60},   {253, kParamAmpDecay, 1200},
  {253, kParamAmpSustain, 76},   {253, kParamAmpRelease, 1200},   {253, kParamVelToAmp, 46},
  {253, kParamVelToFilter, 30},   {253, kParamStereoSpread, 44},   {253, kParamStereoDelay, 20},
  {253, kParamTapeSat, 18},   {253, kParamTapeMix, 26},   {253, kParamIns1Algo, 2},
  {253, kParamIns1A, 22},   {253, kParamIns1B, 24},   {253, kParamIns1C, 34},
  {253, kParamIns1Mix, 18},   {253, kParamMstAlgo, 2},   {253, kParamMstA, 24},
  {253, kParamMstB, 18},   {253, kParamMstC, 30},   {253, kParamMstMix, 10},
  {254, kParamEngine, 0},   {254, kParamPolyMode, 0},   {254, kParamPorta, 0},
  {254, kParamOsc1Shape, 54},   {254, kParamOsc1PW, 16},   {254, kParamOsc1Level, 82},
  {254, kParamOsc2Shape, 52},   {254, kParamOsc2Pitch, -12},   {254, kParamOsc2Level, 44},
  {254, kParamSubLevel, 26},   {254, kParamNoiseLevel, 24},   {254, kParamFilterType, 0},
  {254, kParamFilterCutoff, 7800},   {254, kParamFilterReso, 34},   {254, kParamFilterEnv, 38},
  {254, kParamFiltAttack, 12},   {254, kParamFiltDecay, 1100},   {254, kParamFiltSustain, 52},
  {254, kParamFiltRelease, 800},   {254, kParamAmpAttack, 18},   {254, kParamAmpDecay, 1200},
  {254, kParamAmpSustain, 76},   {254, kParamAmpRelease, 800},   {254, kParamVelToAmp, 46},
  {254, kParamVelToFilter, 30},   {254, kParamStereoSpread, 44},   {254, kParamStereoDelay, 20},
  {254, kParamTapeSat, 18},   {254, kParamTapeMix, 26},   {254, kParamIns1Algo, 1},
  {254, kParamIns1A, 18},   {254, kParamIns1B, 34},   {254, kParamIns1C, 26},
  {254, kParamIns1Mix, 12},   {254, kParamMstAlgo, 2},   {254, kParamMstA, 24},
  {254, kParamMstB, 18},   {254, kParamMstC, 30},   {254, kParamMstMix, 10},
  {255, kParamEngine, 0},   {255, kParamPolyMode, 0},   {255, kParamPorta, 0},
  {255, kParamOsc1Shape, 54},   {255, kParamOsc1PW, 20},   {255, kParamOsc1Level, 82},
  {255, kParamOsc2Shape, 52},   {255, kParamOsc2Pitch, -12},   {255, kParamOsc2Level, 44},
  {255, kParamSubLevel, 26},   {255, kParamNoiseLevel, 16},   {255, kParamFilterType, 0},
  {255, kParamFilterCutoff, 3800},   {255, kParamFilterReso, 28},   {255, kParamFilterEnv, 38},
  {255, kParamFiltAttack, 12},   {255, kParamFiltDecay, 1100},   {255, kParamFiltSustain, 52},
  {255, kParamFiltRelease, 1000},   {255, kParamAmpAttack, 36},   {255, kParamAmpDecay, 1200},
  {255, kParamAmpSustain, 76},   {255, kParamAmpRelease, 1000},   {255, kParamVelToAmp, 46},
  {255, kParamVelToFilter, 30},   {255, kParamStereoSpread, 44},   {255, kParamStereoDelay, 20},
  {255, kParamTapeSat, 18},   {255, kParamTapeMix, 26},   {255, kParamIns1Algo, 2},
  {255, kParamIns1A, 16},   {255, kParamIns1B, 20},   {255, kParamIns1C, 28},
  {255, kParamIns1Mix, 12},   {255, kParamMstAlgo, 2},   {255, kParamMstA, 24},
  {255, kParamMstB, 18},   {255, kParamMstC, 30},   {255, kParamMstMix, 10},
  {256, kParamEngine, 0},   {256, kParamPolyMode, 0},   {256, kParamPorta, 0},
  {256, kParamOsc1Shape, 54},   {256, kParamOsc1PW, 28},   {256, kParamOsc1Level, 82},
  {256, kParamOsc2Shape, 52},   {256, kParamOsc2Pitch, -12},   {256, kParamOsc2Level, 44},
  {256, kParamSubLevel, 26},   {256, kParamNoiseLevel, 30},   {256, kParamFilterType, 0},
  {256, kParamFilterCutoff, 5400},   {256, kParamFilterReso, 28},   {256, kParamFilterEnv, 38},
  {256, kParamFiltAttack, 12},   {256, kParamFiltDecay, 1100},   {256, kParamFiltSustain, 52},
  {256, kParamFiltRelease, 950},   {256, kParamAmpAttack, 32},   {256, kParamAmpDecay, 1200},
  {256, kParamAmpSustain, 76},   {256, kParamAmpRelease, 950},   {256, kParamVelToAmp, 46},
  {256, kParamVelToFilter, 30},   {256, kParamStereoSpread, 44},   {256, kParamStereoDelay, 20},
  {256, kParamTapeSat, 30},   {256, kParamTapeMix, 40},   {256, kParamTapeWow, 36},
  {256, kParamIns1Algo, 2},   {256, kParamIns1A, 22},   {256, kParamIns1B, 24},
  {256, kParamIns1C, 34},   {256, kParamIns1Mix, 18},   {256, kParamMstAlgo, 2},
  {256, kParamMstA, 24},   {256, kParamMstB, 18},   {256, kParamMstC, 30},
  {256, kParamMstMix, 10},
  {257, kParamEngine, 0},   {257, kParamPolyMode, 0},   {257, kParamPorta, 0},
  {257, kParamOsc1Shape, 54},   {257, kParamOsc1PW, 44},   {257, kParamOsc1Level, 82},
  {257, kParamOsc2Shape, 52},   {257, kParamOsc2Pitch, -12},   {257, kParamOsc2Level, 44},
  {257, kParamSubLevel, 26},   {257, kParamNoiseLevel, 44},   {257, kParamFilterType, 0},
  {257, kParamFilterCutoff, 8600},   {257, kParamFilterReso, 22},   {257, kParamFilterEnv, 38},
  {257, kParamFiltAttack, 12},   {257, kParamFiltDecay, 1100},   {257, kParamFiltSustain, 52},
  {257, kParamFiltRelease, 900},   {257, kParamAmpAttack, 26},   {257, kParamAmpDecay, 1200},
  {257, kParamAmpSustain, 76},   {257, kParamAmpRelease, 900},   {257, kParamVelToAmp, 46},
  {257, kParamVelToFilter, 30},   {257, kParamStereoSpread, 44},   {257, kParamStereoDelay, 20},
  {257, kParamTapeSat, 18},   {257, kParamTapeMix, 26},   {257, kParamIns1Algo, 1},
  {257, kParamIns1A, 18},   {257, kParamIns1B, 34},   {257, kParamIns1C, 26},
  {257, kParamIns1Mix, 12},   {257, kParamMstAlgo, 2},   {257, kParamMstA, 24},
  {257, kParamMstB, 18},   {257, kParamMstC, 30},   {257, kParamMstMix, 10},
  {258, kParamEngine, 0},   {258, kParamPolyMode, 0},   {258, kParamPorta, 0},
  {258, kParamOsc1Shape, 54},   {258, kParamOsc1PW, 28},   {258, kParamOsc1Level, 82},
  {258, kParamOsc2Shape, 52},   {258, kParamOsc2Pitch, -12},   {258, kParamOsc2Level, 44},
  {258, kParamSubLevel, 26},   {258, kParamNoiseLevel, 26},   {258, kParamFilterType, 0},
  {258, kParamFilterCutoff, 6000},   {258, kParamFilterReso, 24},   {258, kParamFilterEnv, 38},
  {258, kParamFiltAttack, 12},   {258, kParamFiltDecay, 1100},   {258, kParamFiltSustain, 52},
  {258, kParamFiltRelease, 1100},   {258, kParamAmpAttack, 44},   {258, kParamAmpDecay, 1200},
  {258, kParamAmpSustain, 76},   {258, kParamAmpRelease, 1100},   {258, kParamVelToAmp, 46},
  {258, kParamVelToFilter, 30},   {258, kParamStereoSpread, 44},   {258, kParamStereoDelay, 20},
  {258, kParamTapeSat, 18},   {258, kParamTapeMix, 26},   {258, kParamUnisonVoices, 3},
  {258, kParamUnisonDetune, 18},   {258, kParamIns1Algo, 2},   {258, kParamIns1A, 16},
  {258, kParamIns1B, 20},   {258, kParamIns1C, 28},   {258, kParamIns1Mix, 12},
  {258, kParamMstAlgo, 2},   {258, kParamMstA, 24},   {258, kParamMstB, 18},
  {258, kParamMstC, 30},   {258, kParamMstMix, 10},
  {259, kParamEngine, 0},   {259, kParamPolyMode, 0},   {259, kParamOsc1Shape, 40},
  {259, kParamOsc1Level, 84},   {259, kParamOsc2Shape, 38},   {259, kParamOsc2Pitch, 0},
  {259, kParamOsc2Level, 76},   {259, kParamSubLevel, 24},   {259, kParamNoiseLevel, 4},
  {259, kParamFilterType, 0},   {259, kParamFilterCutoff, 4600},   {259, kParamFilterReso, 24},
  {259, kParamFilterEnv, 58},   {259, kParamFiltAttack, 90},   {259, kParamFiltDecay, 900},
  {259, kParamFiltSustain, 46},   {259, kParamFiltRelease, 900},   {259, kParamAmpAttack, 30},
  {259, kParamAmpDecay, 1200},   {259, kParamAmpSustain, 74},   {259, kParamAmpRelease, 900},
  {259, kParamUnisonVoices, 2},   {259, kParamUnisonDetune, 14},   {259, kParamVelToAmp, 40},
  {259, kParamVelToFilter, 46},   {259, kParamStereoSpread, 56},   {259, kParamTapeSat, 24},
  {259, kParamTapeMix, 32},   {259, kParamIns1Algo, 2},   {259, kParamIns1A, 22},
  {259, kParamIns1B, 24},   {259, kParamIns1C, 34},   {259, kParamIns1Mix, 18},
  {259, kParamMstAlgo, 2},   {259, kParamMstA, 24},   {259, kParamMstB, 18},
  {259, kParamMstC, 30},   {259, kParamMstMix, 10},
  {260, kParamEngine, 0},   {260, kParamPolyMode, 0},   {260, kParamOsc1Shape, 40},
  {260, kParamOsc1Level, 84},   {260, kParamOsc2Shape, 38},   {260, kParamOsc2Pitch, 0},
  {260, kParamOsc2Level, 76},   {260, kParamSubLevel, 24},   {260, kParamNoiseLevel, 4},
  {260, kParamFilterType, 0},   {260, kParamFilterCutoff, 5800},   {260, kParamFilterReso, 22},
  {260, kParamFilterEnv, 58},   {260, kParamFiltAttack, 70},   {260, kParamFiltDecay, 900},
  {260, kParamFiltSustain, 46},   {260, kParamFiltRelease, 850},   {260, kParamAmpAttack, 24},
  {260, kParamAmpDecay, 1200},   {260, kParamAmpSustain, 74},   {260, kParamAmpRelease, 850},
  {260, kParamUnisonVoices, 3},   {260, kParamUnisonDetune, 16},   {260, kParamVelToAmp, 40},
  {260, kParamVelToFilter, 46},   {260, kParamStereoSpread, 56},   {260, kParamTapeSat, 24},
  {260, kParamTapeMix, 32},   {260, kParamIns1Algo, 1},   {260, kParamIns1A, 18},
  {260, kParamIns1B, 34},   {260, kParamIns1C, 26},   {260, kParamIns1Mix, 12},
  {260, kParamMstAlgo, 2},   {260, kParamMstA, 24},   {260, kParamMstB, 18},
  {260, kParamMstC, 30},   {260, kParamMstMix, 10},
  {261, kParamEngine, 0},   {261, kParamPolyMode, 0},   {261, kParamOsc1Shape, 40},
  {261, kParamOsc1Level, 84},   {261, kParamOsc2Shape, 38},   {261, kParamOsc2Pitch, 0},
  {261, kParamOsc2Level, 76},   {261, kParamSubLevel, 24},   {261, kParamNoiseLevel, 4},
  {261, kParamFilterType, 0},   {261, kParamFilterCutoff, 3600},   {261, kParamFilterReso, 28},
  {261, kParamFilterEnv, 58},   {261, kParamFiltAttack, 110},   {261, kParamFiltDecay, 900},
  {261, kParamFiltSustain, 46},   {261, kParamFiltRelease, 1000},   {261, kParamAmpAttack, 40},
  {261, kParamAmpDecay, 1200},   {261, kParamAmpSustain, 74},   {261, kParamAmpRelease, 1000},
  {261, kParamUnisonVoices, 2},   {261, kParamUnisonDetune, 14},   {261, kParamVelToAmp, 40},
  {261, kParamVelToFilter, 46},   {261, kParamStereoSpread, 56},   {261, kParamTapeSat, 24},
  {261, kParamTapeMix, 32},   {261, kParamIns1Algo, 2},   {261, kParamIns1A, 16},
  {261, kParamIns1B, 20},   {261, kParamIns1C, 28},   {261, kParamIns1Mix, 12},
  {261, kParamMstAlgo, 2},   {261, kParamMstA, 24},   {261, kParamMstB, 18},
  {261, kParamMstC, 30},   {261, kParamMstMix, 10},
  {262, kParamEngine, 0},   {262, kParamPolyMode, 0},   {262, kParamOsc1Shape, 40},
  {262, kParamOsc1Level, 84},   {262, kParamOsc2Shape, 38},   {262, kParamOsc2Pitch, 0},
  {262, kParamOsc2Level, 76},   {262, kParamSubLevel, 24},   {262, kParamNoiseLevel, 4},
  {262, kParamFilterType, 0},   {262, kParamFilterCutoff, 5200},   {262, kParamFilterReso, 30},
  {262, kParamFilterEnv, 72},   {262, kParamFiltAttack, 50},   {262, kParamFiltDecay, 900},
  {262, kParamFiltSustain, 46},   {262, kParamFiltRelease, 500},   {262, kParamAmpAttack, 6},
  {262, kParamAmpDecay, 1200},   {262, kParamAmpSustain, 74},   {262, kParamAmpRelease, 500},
  {262, kParamUnisonVoices, 2},   {262, kParamUnisonDetune, 14},   {262, kParamVelToAmp, 40},
  {262, kParamVelToFilter, 46},   {262, kParamStereoSpread, 56},   {262, kParamTapeSat, 24},
  {262, kParamTapeMix, 32},   {262, kParamIns1Algo, 2},   {262, kParamIns1A, 22},
  {262, kParamIns1B, 24},   {262, kParamIns1C, 34},   {262, kParamIns1Mix, 18},
  {262, kParamMstAlgo, 2},   {262, kParamMstA, 24},   {262, kParamMstB, 18},
  {262, kParamMstC, 30},   {262, kParamMstMix, 10},
  {263, kParamEngine, 0},   {263, kParamPolyMode, 0},   {263, kParamOsc1Shape, 40},
  {263, kParamOsc1Level, 84},   {263, kParamOsc2Shape, 38},   {263, kParamOsc2Pitch, 0},
  {263, kParamOsc2Level, 76},   {263, kParamSubLevel, 24},   {263, kParamNoiseLevel, 8},
  {263, kParamFilterType, 0},   {263, kParamFilterCutoff, 4200},   {263, kParamFilterReso, 20},
  {263, kParamFilterEnv, 58},   {263, kParamFiltAttack, 140},   {263, kParamFiltDecay, 900},
  {263, kParamFiltSustain, 46},   {263, kParamFiltRelease, 1200},   {263, kParamAmpAttack, 60},
  {263, kParamAmpDecay, 1200},   {263, kParamAmpSustain, 74},   {263, kParamAmpRelease, 1200},
  {263, kParamUnisonVoices, 2},   {263, kParamUnisonDetune, 14},   {263, kParamVelToAmp, 40},
  {263, kParamVelToFilter, 46},   {263, kParamStereoSpread, 56},   {263, kParamTapeSat, 24},
  {263, kParamTapeMix, 32},   {263, kParamIns1Algo, 1},   {263, kParamIns1A, 18},
  {263, kParamIns1B, 34},   {263, kParamIns1C, 26},   {263, kParamIns1Mix, 12},
  {263, kParamMstAlgo, 2},   {263, kParamMstA, 24},   {263, kParamMstB, 18},
  {263, kParamMstC, 30},   {263, kParamMstMix, 10},
  {264, kParamEngine, 0},   {264, kParamPolyMode, 0},   {264, kParamOsc1Shape, 40},
  {264, kParamOsc1Level, 84},   {264, kParamOsc2Shape, 38},   {264, kParamOsc2Pitch, 0},
  {264, kParamOsc2Level, 76},   {264, kParamSubLevel, 24},   {264, kParamNoiseLevel, 4},
  {264, kParamFilterType, 0},   {264, kParamFilterCutoff, 6200},   {264, kParamFilterReso, 34},
  {264, kParamFilterEnv, 58},   {264, kParamFiltAttack, 60},   {264, kParamFiltDecay, 900},
  {264, kParamFiltSustain, 46},   {264, kParamFiltRelease, 700},   {264, kParamAmpAttack, 14},
  {264, kParamAmpDecay, 1200},   {264, kParamAmpSustain, 74},   {264, kParamAmpRelease, 700},
  {264, kParamUnisonVoices, 2},   {264, kParamUnisonDetune, 14},   {264, kParamVelToAmp, 40},
  {264, kParamVelToFilter, 46},   {264, kParamStereoSpread, 56},   {264, kParamTapeSat, 24},
  {264, kParamTapeMix, 32},   {264, kParamFilterDrive, 30},   {264, kParamIns1Algo, 2},
  {264, kParamIns1A, 16},   {264, kParamIns1B, 20},   {264, kParamIns1C, 28},
  {264, kParamIns1Mix, 12},   {264, kParamMstAlgo, 2},   {264, kParamMstA, 24},
  {264, kParamMstB, 18},   {264, kParamMstC, 30},   {264, kParamMstMix, 10},
  {265, kParamEngine, 0},   {265, kParamPolyMode, 0},   {265, kParamOsc1Shape, 40},
  {265, kParamOsc1Level, 84},   {265, kParamOsc2Shape, 38},   {265, kParamOsc2Pitch, 0},
  {265, kParamOsc2Level, 76},   {265, kParamSubLevel, 24},   {265, kParamNoiseLevel, 4},
  {265, kParamFilterType, 0},   {265, kParamFilterCutoff, 4800},   {265, kParamFilterReso, 26},
  {265, kParamFilterEnv, 58},   {265, kParamFiltAttack, 95},   {265, kParamFiltDecay, 900},
  {265, kParamFiltSustain, 46},   {265, kParamFiltRelease, 900},   {265, kParamAmpAttack, 32},
  {265, kParamAmpDecay, 1200},   {265, kParamAmpSustain, 74},   {265, kParamAmpRelease, 900},
  {265, kParamUnisonVoices, 2},   {265, kParamUnisonDetune, 14},   {265, kParamVelToAmp, 40},
  {265, kParamVelToFilter, 46},   {265, kParamStereoSpread, 56},   {265, kParamTapeSat, 34},
  {265, kParamTapeMix, 42},   {265, kParamTapeWow, 32},   {265, kParamIns1Algo, 2},
  {265, kParamIns1A, 22},   {265, kParamIns1B, 24},   {265, kParamIns1C, 34},
  {265, kParamIns1Mix, 18},   {265, kParamMstAlgo, 2},   {265, kParamMstA, 24},
  {265, kParamMstB, 18},   {265, kParamMstC, 30},   {265, kParamMstMix, 10},
  {266, kParamEngine, 0},   {266, kParamPolyMode, 0},   {266, kParamOsc1Shape, 40},
  {266, kParamOsc1Level, 84},   {266, kParamOsc2Shape, 38},   {266, kParamOsc2Pitch, 0},
  {266, kParamOsc2Level, 76},   {266, kParamSubLevel, 24},   {266, kParamNoiseLevel, 4},
  {266, kParamFilterType, 0},   {266, kParamFilterCutoff, 3800},   {266, kParamFilterReso, 36},
  {266, kParamFilterEnv, 40},   {266, kParamFiltAttack, 80},   {266, kParamFiltDecay, 900},
  {266, kParamFiltSustain, 46},   {266, kParamFiltRelease, 800},   {266, kParamAmpAttack, 26},
  {266, kParamAmpDecay, 1200},   {266, kParamAmpSustain, 74},   {266, kParamAmpRelease, 800},
  {266, kParamUnisonVoices, 2},   {266, kParamUnisonDetune, 14},   {266, kParamVelToAmp, 40},
  {266, kParamVelToFilter, 46},   {266, kParamStereoSpread, 56},   {266, kParamTapeSat, 24},
  {266, kParamTapeMix, 32},   {266, kParamIns1Algo, 1},   {266, kParamIns1A, 18},
  {266, kParamIns1B, 34},   {266, kParamIns1C, 26},   {266, kParamIns1Mix, 12},
  {266, kParamMstAlgo, 2},   {266, kParamMstA, 24},   {266, kParamMstB, 18},
  {266, kParamMstC, 30},   {266, kParamMstMix, 10},
  {267, kParamEngine, 0},   {267, kParamPolyMode, 0},   {267, kParamOsc1Shape, 40},
  {267, kParamOsc1Level, 84},   {267, kParamOsc2Shape, 38},   {267, kParamOsc2Pitch, 0},
  {267, kParamOsc2Level, 76},   {267, kParamSubLevel, 24},   {267, kParamNoiseLevel, 4},
  {267, kParamFilterType, 0},   {267, kParamFilterCutoff, 5000},   {267, kParamFilterReso, 24},
  {267, kParamFilterEnv, 58},   {267, kParamFiltAttack, 180},   {267, kParamFiltDecay, 900},
  {267, kParamFiltSustain, 46},   {267, kParamFiltRelease, 1400},   {267, kParamAmpAttack, 90},
  {267, kParamAmpDecay, 1200},   {267, kParamAmpSustain, 74},   {267, kParamAmpRelease, 1400},
  {267, kParamUnisonVoices, 4},   {267, kParamUnisonDetune, 20},   {267, kParamVelToAmp, 40},
  {267, kParamVelToFilter, 46},   {267, kParamStereoSpread, 56},   {267, kParamTapeSat, 24},
  {267, kParamTapeMix, 32},   {267, kParamIns1Algo, 2},   {267, kParamIns1A, 16},
  {267, kParamIns1B, 20},   {267, kParamIns1C, 28},   {267, kParamIns1Mix, 12},
  {267, kParamMstAlgo, 2},   {267, kParamMstA, 24},   {267, kParamMstB, 18},
  {267, kParamMstC, 30},   {267, kParamMstMix, 10},
  {268, kParamEngine, 0},   {268, kParamPolyMode, 0},   {268, kParamOsc1Shape, 40},
  {268, kParamOsc1Level, 84},   {268, kParamOsc2Shape, 38},   {268, kParamOsc2Pitch, 0},
  {268, kParamOsc2Level, 76},   {268, kParamSubLevel, 24},   {268, kParamNoiseLevel, 4},
  {268, kParamFilterType, 0},   {268, kParamFilterCutoff, 6600},   {268, kParamFilterReso, 30},
  {268, kParamFilterEnv, 58},   {268, kParamFiltAttack, 44},   {268, kParamFiltDecay, 900},
  {268, kParamFiltSustain, 46},   {268, kParamFiltRelease, 650},   {268, kParamAmpAttack, 12},
  {268, kParamAmpDecay, 1200},   {268, kParamAmpSustain, 74},   {268, kParamAmpRelease, 650},
  {268, kParamUnisonVoices, 2},   {268, kParamUnisonDetune, 14},   {268, kParamVelToAmp, 40},
  {268, kParamVelToFilter, 54},   {268, kParamStereoSpread, 56},   {268, kParamTapeSat, 24},
  {268, kParamTapeMix, 32},   {268, kParamIns1Algo, 2},   {268, kParamIns1A, 22},
  {268, kParamIns1B, 24},   {268, kParamIns1C, 34},   {268, kParamIns1Mix, 18},
  {268, kParamMstAlgo, 2},   {268, kParamMstA, 24},   {268, kParamMstB, 18},
  {268, kParamMstC, 30},   {268, kParamMstMix, 10},
  // pad
  {269, kParamEngine, 0},   {269, kParamPolyMode, 0},   {269, kParamOsc1Shape, 40},
  {269, kParamOsc1Level, 76},   {269, kParamOsc2Shape, 38},   {269, kParamOsc2Pitch, -12},
  {269, kParamOsc2Level, 62},   {269, kParamSubLevel, 28},   {269, kParamNoiseLevel, 6},
  {269, kParamFilterType, 0},   {269, kParamFilterCutoff, 5200},   {269, kParamFilterReso, 20},
  {269, kParamFilterEnv, 26},   {269, kParamFiltAttack, 252},   {269, kParamFiltDecay, 2200},
  {269, kParamFiltSustain, 52},   {269, kParamFiltRelease, 2800},   {269, kParamAmpAttack, 420},
  {269, kParamAmpDecay, 2400},   {269, kParamAmpSustain, 76},   {269, kParamAmpRelease, 2800},
  {269, kParamUnisonVoices, 4},   {269, kParamUnisonDetune, 20},   {269, kParamLfoRate, 0.22},
  {269, kParamLfoDest, 1},   {269, kParamLfoAmount, 22},   {269, kParamStereoSpread, 78},
  {269, kParamTapeSat, 16},   {269, kParamTapeMix, 30},   {269, kParamIns1Algo, 2},
  {269, kParamIns1A, 12},   {269, kParamIns1B, 36},   {269, kParamIns1C, 44},
  {269, kParamIns1Mix, 28},   {269, kParamIns2Algo, 3},   {269, kParamIns2A, 62},
  {269, kParamIns2B, 44},   {269, kParamIns2C, 50},   {269, kParamIns2Mix, 18},
  {269, kParamMstAlgo, 2},   {269, kParamMstA, 24},   {269, kParamMstB, 18},
  {269, kParamMstC, 30},   {269, kParamMstMix, 10},
  {270, kParamEngine, 0},   {270, kParamPolyMode, 0},   {270, kParamOsc1Shape, 40},
  {270, kParamOsc1Level, 76},   {270, kParamOsc2Shape, 38},   {270, kParamOsc2Pitch, -12},
  {270, kParamOsc2Level, 62},   {270, kParamSubLevel, 28},   {270, kParamNoiseLevel, 6},
  {270, kParamFilterType, 0},   {270, kParamFilterCutoff, 4600},   {270, kParamFilterReso, 24},
  {270, kParamFilterEnv, 26},   {270, kParamFiltAttack, 228},   {270, kParamFiltDecay, 2200},
  {270, kParamFiltSustain, 52},   {270, kParamFiltRelease, 3000},   {270, kParamAmpAttack, 380},
  {270, kParamAmpDecay, 2400},   {270, kParamAmpSustain, 76},   {270, kParamAmpRelease, 3000},
  {270, kParamUnisonVoices, 4},   {270, kParamUnisonDetune, 20},   {270, kParamLfoRate, 0.16},
  {270, kParamLfoDest, 1},   {270, kParamLfoAmount, 22},   {270, kParamStereoSpread, 78},
  {270, kParamTapeSat, 16},   {270, kParamTapeMix, 30},   {270, kParamIns1Algo, 2},
  {270, kParamIns1A, 18},   {270, kParamIns1B, 44},   {270, kParamIns1C, 52},
  {270, kParamIns1Mix, 34},   {270, kParamIns2Algo, 2},   {270, kParamIns2A, 26},
  {270, kParamIns2B, 34},   {270, kParamIns2C, 40},   {270, kParamIns2Mix, 16},
  {270, kParamMstAlgo, 2},   {270, kParamMstA, 24},   {270, kParamMstB, 18},
  {270, kParamMstC, 30},   {270, kParamMstMix, 10},
  {271, kParamEngine, 0},   {271, kParamPolyMode, 0},   {271, kParamOsc1Shape, 40},
  {271, kParamOsc1Level, 76},   {271, kParamOsc2Shape, 38},   {271, kParamOsc2Pitch, -12},
  {271, kParamOsc2Level, 62},   {271, kParamSubLevel, 28},   {271, kParamNoiseLevel, 6},
  {271, kParamFilterType, 0},   {271, kParamFilterCutoff, 6000},   {271, kParamFilterReso, 18},
  {271, kParamFilterEnv, 26},   {271, kParamFiltAttack, 204},   {271, kParamFiltDecay, 2200},
  {271, kParamFiltSustain, 52},   {271, kParamFiltRelease, 2600},   {271, kParamAmpAttack, 340},
  {271, kParamAmpDecay, 2400},   {271, kParamAmpSustain, 76},   {271, kParamAmpRelease, 2600},
  {271, kParamUnisonVoices, 4},   {271, kParamUnisonDetune, 20},   {271, kParamLfoRate, 0.22},
  {271, kParamLfoDest, 1},   {271, kParamLfoAmount, 22},   {271, kParamStereoSpread, 78},
  {271, kParamTapeSat, 16},   {271, kParamTapeMix, 30},   {271, kParamIns1Algo, 3},
  {271, kParamIns1A, 58},   {271, kParamIns1B, 42},   {271, kParamIns1C, 48},
  {271, kParamIns1Mix, 20},   {271, kParamIns2Algo, 3},   {271, kParamIns2A, 62},
  {271, kParamIns2B, 44},   {271, kParamIns2C, 50},   {271, kParamIns2Mix, 18},
  {271, kParamMstAlgo, 2},   {271, kParamMstA, 24},   {271, kParamMstB, 18},
  {271, kParamMstC, 30},   {271, kParamMstMix, 10},
  {272, kParamEngine, 0},   {272, kParamPolyMode, 0},   {272, kParamOsc1Shape, 28},
  {272, kParamOsc1Level, 48},   {272, kParamOsc2Level, 0},   {272, kParamOsc3Enable, 1},
  {272, kParamOsc3Source, 1},   {272, kParamOsc3Morph, 3.2},   {272, kParamOsc3Level, 84},
  {272, kParamSubLevel, 22},   {272, kParamNoiseLevel, 4},   {272, kParamFilterType, 0},
  {272, kParamFilterCutoff, 5000},   {272, kParamFilterReso, 22},   {272, kParamFilterEnv, 26},
  {272, kParamFiltAttack, 276},   {272, kParamFiltDecay, 2200},   {272, kParamFiltSustain, 52},
  {272, kParamFiltRelease, 3200},   {272, kParamAmpAttack, 460},   {272, kParamAmpDecay, 2400},
  {272, kParamAmpSustain, 76},   {272, kParamAmpRelease, 3200},   {272, kParamUnisonVoices, 4},
  {272, kParamUnisonDetune, 20},   {272, kParamLfoRate, 0.22},   {272, kParamLfoDest, 1},
  {272, kParamLfoAmount, 22},   {272, kParamStereoSpread, 78},   {272, kParamTapeSat, 16},
  {272, kParamTapeMix, 30},   {272, kParamIns1Algo, 2},   {272, kParamIns1A, 12},
  {272, kParamIns1B, 36},   {272, kParamIns1C, 44},   {272, kParamIns1Mix, 28},
  {272, kParamIns2Algo, 2},   {272, kParamIns2A, 26},   {272, kParamIns2B, 34},
  {272, kParamIns2C, 40},   {272, kParamIns2Mix, 16},   {272, kParamMstAlgo, 2},
  {272, kParamMstA, 24},   {272, kParamMstB, 18},   {272, kParamMstC, 30},
  {272, kParamMstMix, 10},
  {273, kParamEngine, 0},   {273, kParamPolyMode, 0},   {273, kParamOsc1Shape, 28},
  {273, kParamOsc1Level, 48},   {273, kParamOsc2Level, 0},   {273, kParamOsc3Enable, 1},
  {273, kParamOsc3Source, 1},   {273, kParamOsc3Morph, 2.4},   {273, kParamOsc3Level, 84},
  {273, kParamSubLevel, 22},   {273, kParamNoiseLevel, 4},   {273, kParamFilterType, 0},
  {273, kParamFilterCutoff, 7200},   {273, kParamFilterReso, 16},   {273, kParamFilterEnv, 26},
  {273, kParamFiltAttack, 180},   {273, kParamFiltDecay, 2200},   {273, kParamFiltSustain, 52},
  {273, kParamFiltRelease, 2400},   {273, kParamAmpAttack, 300},   {273, kParamAmpDecay, 2400},
  {273, kParamAmpSustain, 76},   {273, kParamAmpRelease, 2400},   {273, kParamUnisonVoices, 4},
  {273, kParamUnisonDetune, 20},   {273, kParamLfoRate, 0.22},   {273, kParamLfoDest, 1},
  {273, kParamLfoAmount, 22},   {273, kParamStereoSpread, 78},   {273, kParamTapeSat, 16},
  {273, kParamTapeMix, 30},   {273, kParamIns1Algo, 2},   {273, kParamIns1A, 18},
  {273, kParamIns1B, 44},   {273, kParamIns1C, 52},   {273, kParamIns1Mix, 34},
  {273, kParamIns2Algo, 3},   {273, kParamIns2A, 62},   {273, kParamIns2B, 44},
  {273, kParamIns2C, 50},   {273, kParamIns2Mix, 18},   {273, kParamMstAlgo, 2},
  {273, kParamMstA, 24},   {273, kParamMstB, 18},   {273, kParamMstC, 30},
  {273, kParamMstMix, 10},
  {274, kParamEngine, 0},   {274, kParamPolyMode, 0},   {274, kParamOsc1Shape, 40},
  {274, kParamOsc1Level, 76},   {274, kParamOsc2Shape, 38},   {274, kParamOsc2Pitch, -12},
  {274, kParamOsc2Level, 62},   {274, kParamSubLevel, 44},   {274, kParamNoiseLevel, 6},
  {274, kParamFilterType, 0},   {274, kParamFilterCutoff, 3200},   {274, kParamFilterReso, 26},
  {274, kParamFilterEnv, 26},   {274, kParamFiltAttack, 300},   {274, kParamFiltDecay, 2200},
  {274, kParamFiltSustain, 52},   {274, kParamFiltRelease, 3400},   {274, kParamAmpAttack, 500},
  {274, kParamAmpDecay, 2400},   {274, kParamAmpSustain, 76},   {274, kParamAmpRelease, 3400},
  {274, kParamUnisonVoices, 4},   {274, kParamUnisonDetune, 20},   {274, kParamLfoRate, 0.12},
  {274, kParamLfoDest, 1},   {274, kParamLfoAmount, 22},   {274, kParamStereoSpread, 78},
  {274, kParamTapeSat, 16},   {274, kParamTapeMix, 30},   {274, kParamIns1Algo, 3},
  {274, kParamIns1A, 58},   {274, kParamIns1B, 42},   {274, kParamIns1C, 48},
  {274, kParamIns1Mix, 20},   {274, kParamIns2Algo, 2},   {274, kParamIns2A, 26},
  {274, kParamIns2B, 34},   {274, kParamIns2C, 40},   {274, kParamIns2Mix, 16},
  {274, kParamMstAlgo, 2},   {274, kParamMstA, 24},   {274, kParamMstB, 18},
  {274, kParamMstC, 30},   {274, kParamMstMix, 10},
  {275, kParamEngine, 0},   {275, kParamPolyMode, 0},   {275, kParamOsc1Shape, 40},
  {275, kParamOsc1Level, 76},   {275, kParamOsc2Shape, 38},   {275, kParamOsc2Pitch, -12},
  {275, kParamOsc2Level, 62},   {275, kParamSubLevel, 28},   {275, kParamNoiseLevel, 14},
  {275, kParamFilterType, 0},   {275, kParamFilterCutoff, 6400},   {275, kParamFilterReso, 20},
  {275, kParamFilterEnv, 26},   {275, kParamFiltAttack, 216},   {275, kParamFiltDecay, 2200},
  {275, kParamFiltSustain, 52},   {275, kParamFiltRelease, 2800},   {275, kParamAmpAttack, 360},
  {275, kParamAmpDecay, 2400},   {275, kParamAmpSustain, 76},   {275, kParamAmpRelease, 2800},
  {275, kParamUnisonVoices, 4},   {275, kParamUnisonDetune, 20},   {275, kParamLfoRate, 0.3},
  {275, kParamLfoDest, 1},   {275, kParamLfoAmount, 22},   {275, kParamStereoSpread, 78},
  {275, kParamTapeSat, 16},   {275, kParamTapeMix, 30},   {275, kParamIns1Algo, 2},
  {275, kParamIns1A, 12},   {275, kParamIns1B, 36},   {275, kParamIns1C, 44},
  {275, kParamIns1Mix, 28},   {275, kParamIns2Algo, 3},   {275, kParamIns2A, 62},
  {275, kParamIns2B, 44},   {275, kParamIns2C, 50},   {275, kParamIns2Mix, 18},
  {275, kParamMstAlgo, 2},   {275, kParamMstA, 24},   {275, kParamMstB, 18},
  {275, kParamMstC, 30},   {275, kParamMstMix, 10},
  {276, kParamEngine, 0},   {276, kParamPolyMode, 0},   {276, kParamOsc1Shape, 40},
  {276, kParamOsc1Level, 76},   {276, kParamOsc2Shape, 38},   {276, kParamOsc2Pitch, -12},
  {276, kParamOsc2Level, 62},   {276, kParamSubLevel, 28},   {276, kParamNoiseLevel, 6},
  {276, kParamFilterType, 0},   {276, kParamFilterCutoff, 4200},   {276, kParamFilterReso, 24},
  {276, kParamFilterEnv, 26},   {276, kParamFiltAttack, 240},   {276, kParamFiltDecay, 2200},
  {276, kParamFiltSustain, 52},   {276, kParamFiltRelease, 3000},   {276, kParamAmpAttack, 400},
  {276, kParamAmpDecay, 2400},   {276, kParamAmpSustain, 76},   {276, kParamAmpRelease, 3000},
  {276, kParamUnisonVoices, 4},   {276, kParamUnisonDetune, 20},   {276, kParamLfoRate, 0.22},
  {276, kParamLfoDest, 1},   {276, kParamLfoAmount, 22},   {276, kParamStereoSpread, 78},
  {276, kParamTapeSat, 34},   {276, kParamTapeMix, 44},   {276, kParamTapeWow, 40},
  {276, kParamIns1Algo, 2},   {276, kParamIns1A, 18},   {276, kParamIns1B, 44},
  {276, kParamIns1C, 52},   {276, kParamIns1Mix, 34},   {276, kParamIns2Algo, 2},
  {276, kParamIns2A, 26},   {276, kParamIns2B, 34},   {276, kParamIns2C, 40},
  {276, kParamIns2Mix, 16},   {276, kParamMstAlgo, 2},   {276, kParamMstA, 24},
  {276, kParamMstB, 18},   {276, kParamMstC, 30},   {276, kParamMstMix, 10},
  {277, kParamEngine, 0},   {277, kParamPolyMode, 0},   {277, kParamOsc1Shape, 40},
  {277, kParamOsc1Level, 76},   {277, kParamOsc2Shape, 38},   {277, kParamOsc2Pitch, -12},
  {277, kParamOsc2Level, 62},   {277, kParamSubLevel, 28},   {277, kParamNoiseLevel, 6},
  {277, kParamFilterType, 0},   {277, kParamFilterCutoff, 5600},   {277, kParamFilterReso, 34},
  {277, kParamFilterEnv, 26},   {277, kParamFiltAttack, 204},   {277, kParamFiltDecay, 2200},
  {277, kParamFiltSustain, 52},   {277, kParamFiltRelease, 2600},   {277, kParamAmpAttack, 340},
  {277, kParamAmpDecay, 2400},   {277, kParamAmpSustain, 76},   {277, kParamAmpRelease, 2600},
  {277, kParamUnisonVoices, 4},   {277, kParamUnisonDetune, 20},   {277, kParamLfoRate, 0.26},
  {277, kParamLfoDest, 1},   {277, kParamLfoAmount, 40},   {277, kParamStereoSpread, 78},
  {277, kParamTapeSat, 16},   {277, kParamTapeMix, 30},   {277, kParamIns1Algo, 3},
  {277, kParamIns1A, 58},   {277, kParamIns1B, 42},   {277, kParamIns1C, 48},
  {277, kParamIns1Mix, 20},   {277, kParamIns2Algo, 3},   {277, kParamIns2A, 62},
  {277, kParamIns2B, 44},   {277, kParamIns2C, 50},   {277, kParamIns2Mix, 18},
  {277, kParamMstAlgo, 2},   {277, kParamMstA, 24},   {277, kParamMstB, 18},
  {277, kParamMstC, 30},   {277, kParamMstMix, 10},
  {278, kParamEngine, 0},   {278, kParamPolyMode, 0},   {278, kParamOsc1Shape, 28},
  {278, kParamOsc1Level, 48},   {278, kParamOsc2Level, 0},   {278, kParamOsc3Enable, 1},
  {278, kParamOsc3Source, 1},   {278, kParamOsc3Morph, 5.2},   {278, kParamOsc3Level, 84},
  {278, kParamSubLevel, 16},   {278, kParamNoiseLevel, 4},   {278, kParamFilterType, 0},
  {278, kParamFilterCutoff, 5800},   {278, kParamFilterReso, 28},   {278, kParamFilterEnv, 26},
  {278, kParamFiltAttack, 264},   {278, kParamFiltDecay, 2200},   {278, kParamFiltSustain, 52},
  {278, kParamFiltRelease, 2900},   {278, kParamAmpAttack, 440},   {278, kParamAmpDecay, 2400},
  {278, kParamAmpSustain, 76},   {278, kParamAmpRelease, 2900},   {278, kParamUnisonVoices, 4},
  {278, kParamUnisonDetune, 20},   {278, kParamLfoRate, 0.22},   {278, kParamLfoDest, 1},
  {278, kParamLfoAmount, 22},   {278, kParamStereoSpread, 78},   {278, kParamTapeSat, 16},
  {278, kParamTapeMix, 30},   {278, kParamIns1Algo, 2},   {278, kParamIns1A, 12},
  {278, kParamIns1B, 36},   {278, kParamIns1C, 44},   {278, kParamIns1Mix, 28},
  {278, kParamIns2Algo, 2},   {278, kParamIns2A, 26},   {278, kParamIns2B, 34},
  {278, kParamIns2C, 40},   {278, kParamIns2Mix, 16},   {278, kParamMstAlgo, 2},
  {278, kParamMstA, 24},   {278, kParamMstB, 18},   {278, kParamMstC, 30},
  {278, kParamMstMix, 10},
  {279, kParamEngine, 0},   {279, kParamPolyMode, 0},   {279, kParamOsc1Shape, 40},
  {279, kParamOsc1Level, 74},   {279, kParamOsc2Shape, 38},   {279, kParamOsc2Pitch, -12},
  {279, kParamOsc2Level, 60},   {279, kParamSubLevel, 30},   {279, kParamNoiseLevel, 6},
  {279, kParamFilterType, 0},   {279, kParamFilterCutoff, 6800},   {279, kParamFilterReso, 18},
  {279, kParamFilterEnv, 26},   {279, kParamFiltAttack, 300},   {279, kParamFiltDecay, 2200},
  {279, kParamFiltSustain, 52},   {279, kParamFiltRelease, 3400},   {279, kParamAmpAttack, 500},
  {279, kParamAmpDecay, 2400},   {279, kParamAmpSustain, 76},   {279, kParamAmpRelease, 3400},
  {279, kParamUnisonVoices, 4},   {279, kParamUnisonDetune, 20},   {279, kParamLfoRate, 0.14},
  {279, kParamLfoDest, 1},   {279, kParamLfoAmount, 20},   {279, kParamStereoSpread, 80},
  {279, kParamTapeSat, 16},   {279, kParamTapeMix, 30},   {279, kParamIns1Algo, 2},
  {279, kParamIns1A, 18},   {279, kParamIns1B, 44},   {279, kParamIns1C, 52},
  {279, kParamIns1Mix, 34},   {279, kParamIns2Algo, 3},   {279, kParamIns2A, 62},
  {279, kParamIns2B, 44},   {279, kParamIns2C, 50},   {279, kParamIns2Mix, 18},
  {279, kParamMstAlgo, 2},   {279, kParamMstA, 24},   {279, kParamMstB, 18},
  {279, kParamMstC, 30},   {279, kParamMstMix, 10},
  {280, kParamEngine, 0},   {280, kParamPolyMode, 0},   {280, kParamOsc1Shape, 40},
  {280, kParamOsc1Level, 74},   {280, kParamOsc2Shape, 38},   {280, kParamOsc2Pitch, -12},
  {280, kParamOsc2Level, 60},   {280, kParamSubLevel, 30},   {280, kParamNoiseLevel, 6},
  {280, kParamFilterType, 0},   {280, kParamFilterCutoff, 5400},   {280, kParamFilterReso, 32},
  {280, kParamFilterEnv, 26},   {280, kParamFiltAttack, 252},   {280, kParamFiltDecay, 2200},
  {280, kParamFiltSustain, 52},   {280, kParamFiltRelease, 3000},   {280, kParamAmpAttack, 420},
  {280, kParamAmpDecay, 2400},   {280, kParamAmpSustain, 76},   {280, kParamAmpRelease, 3000},
  {280, kParamUnisonVoices, 4},   {280, kParamUnisonDetune, 20},   {280, kParamLfoRate, 0.28},
  {280, kParamLfoDest, 1},   {280, kParamLfoAmount, 38},   {280, kParamStereoSpread, 80},
  {280, kParamTapeSat, 16},   {280, kParamTapeMix, 30},   {280, kParamIns1Algo, 3},
  {280, kParamIns1A, 58},   {280, kParamIns1B, 42},   {280, kParamIns1C, 48},
  {280, kParamIns1Mix, 20},   {280, kParamIns2Algo, 2},   {280, kParamIns2A, 26},
  {280, kParamIns2B, 34},   {280, kParamIns2C, 40},   {280, kParamIns2Mix, 16},
  {280, kParamMstAlgo, 2},   {280, kParamMstA, 24},   {280, kParamMstB, 18},
  {280, kParamMstC, 30},   {280, kParamMstMix, 10},
  {281, kParamEngine, 0},   {281, kParamPolyMode, 0},   {281, kParamOsc1Shape, 40},
  {281, kParamOsc1Level, 74},   {281, kParamOsc2Shape, 38},   {281, kParamOsc2Pitch, -12},
  {281, kParamOsc2Level, 60},   {281, kParamSubLevel, 46},   {281, kParamNoiseLevel, 6},
  {281, kParamFilterType, 0},   {281, kParamFilterCutoff, 3800},   {281, kParamFilterReso, 24},
  {281, kParamFilterEnv, 26},   {281, kParamFiltAttack, 336},   {281, kParamFiltDecay, 2200},
  {281, kParamFiltSustain, 52},   {281, kParamFiltRelease, 3600},   {281, kParamAmpAttack, 560},
  {281, kParamAmpDecay, 2400},   {281, kParamAmpSustain, 76},   {281, kParamAmpRelease, 3600},
  {281, kParamUnisonVoices, 4},   {281, kParamUnisonDetune, 20},   {281, kParamLfoRate, 0.12},
  {281, kParamLfoDest, 1},   {281, kParamLfoAmount, 20},   {281, kParamStereoSpread, 80},
  {281, kParamTapeSat, 16},   {281, kParamTapeMix, 30},   {281, kParamIns1Algo, 2},
  {281, kParamIns1A, 12},   {281, kParamIns1B, 36},   {281, kParamIns1C, 44},
  {281, kParamIns1Mix, 28},   {281, kParamIns2Algo, 3},   {281, kParamIns2A, 62},
  {281, kParamIns2B, 44},   {281, kParamIns2C, 50},   {281, kParamIns2Mix, 18},
  {281, kParamMstAlgo, 2},   {281, kParamMstA, 24},   {281, kParamMstB, 18},
  {281, kParamMstC, 30},   {281, kParamMstMix, 10},
  {282, kParamEngine, 0},   {282, kParamPolyMode, 0},   {282, kParamOsc1Shape, 26},
  {282, kParamOsc1Level, 46},   {282, kParamOsc2Level, 0},   {282, kParamOsc3Enable, 1},
  {282, kParamOsc3Source, 1},   {282, kParamOsc3Morph, 4.8},   {282, kParamOsc3Level, 84},
  {282, kParamSubLevel, 20},   {282, kParamNoiseLevel, 4},   {282, kParamFilterType, 0},
  {282, kParamFilterCutoff, 6000},   {282, kParamFilterReso, 20},   {282, kParamFilterEnv, 26},
  {282, kParamFiltAttack, 264},   {282, kParamFiltDecay, 2200},   {282, kParamFiltSustain, 52},
  {282, kParamFiltRelease, 3200},   {282, kParamAmpAttack, 440},   {282, kParamAmpDecay, 2400},
  {282, kParamAmpSustain, 76},   {282, kParamAmpRelease, 3200},   {282, kParamUnisonVoices, 4},
  {282, kParamUnisonDetune, 20},   {282, kParamLfoRate, 0.22},   {282, kParamLfoDest, 1},
  {282, kParamLfoAmount, 20},   {282, kParamStereoSpread, 80},   {282, kParamTapeSat, 16},
  {282, kParamTapeMix, 30},   {282, kParamIns1Algo, 2},   {282, kParamIns1A, 18},
  {282, kParamIns1B, 44},   {282, kParamIns1C, 52},   {282, kParamIns1Mix, 34},
  {282, kParamIns2Algo, 2},   {282, kParamIns2A, 26},   {282, kParamIns2B, 34},
  {282, kParamIns2C, 40},   {282, kParamIns2Mix, 16},   {282, kParamMstAlgo, 2},
  {282, kParamMstA, 24},   {282, kParamMstB, 18},   {282, kParamMstC, 30},
  {282, kParamMstMix, 10},
  {283, kParamEngine, 0},   {283, kParamPolyMode, 0},   {283, kParamOsc1Shape, 40},
  {283, kParamOsc1Level, 74},   {283, kParamOsc2Shape, 38},   {283, kParamOsc2Pitch, -12},
  {283, kParamOsc2Level, 60},   {283, kParamSubLevel, 30},   {283, kParamNoiseLevel, 6},
  {283, kParamFilterType, 0},   {283, kParamFilterCutoff, 4800},   {283, kParamFilterReso, 22},
  {283, kParamFilterEnv, 26},   {283, kParamFiltAttack, 240},   {283, kParamFiltDecay, 2200},
  {283, kParamFiltSustain, 52},   {283, kParamFiltRelease, 3000},   {283, kParamAmpAttack, 400},
  {283, kParamAmpDecay, 2400},   {283, kParamAmpSustain, 76},   {283, kParamAmpRelease, 3000},
  {283, kParamUnisonVoices, 4},   {283, kParamUnisonDetune, 20},   {283, kParamLfoRate, 0.22},
  {283, kParamLfoDest, 1},   {283, kParamLfoAmount, 20},   {283, kParamStereoSpread, 80},
  {283, kParamTapeSat, 36},   {283, kParamTapeMix, 46},   {283, kParamTapeWow, 44},
  {283, kParamIns1Algo, 3},   {283, kParamIns1A, 58},   {283, kParamIns1B, 42},
  {283, kParamIns1C, 48},   {283, kParamIns1Mix, 20},   {283, kParamIns2Algo, 3},
  {283, kParamIns2A, 62},   {283, kParamIns2B, 44},   {283, kParamIns2C, 50},
  {283, kParamIns2Mix, 18},   {283, kParamMstAlgo, 2},   {283, kParamMstA, 24},
  {283, kParamMstB, 18},   {283, kParamMstC, 30},   {283, kParamMstMix, 10},
  {284, kParamEngine, 0},   {284, kParamPolyMode, 0},   {284, kParamOsc1Shape, 26},
  {284, kParamOsc1Level, 46},   {284, kParamOsc2Level, 0},   {284, kParamOsc3Enable, 1},
  {284, kParamOsc3Source, 1},   {284, kParamOsc3Morph, 2.6},   {284, kParamOsc3Level, 84},
  {284, kParamSubLevel, 20},   {284, kParamNoiseLevel, 4},   {284, kParamFilterType, 0},
  {284, kParamFilterCutoff, 8200},   {284, kParamFilterReso, 16},   {284, kParamFilterEnv, 26},
  {284, kParamFiltAttack, 228},   {284, kParamFiltDecay, 2200},   {284, kParamFiltSustain, 52},
  {284, kParamFiltRelease, 2800},   {284, kParamAmpAttack, 380},   {284, kParamAmpDecay, 2400},
  {284, kParamAmpSustain, 76},   {284, kParamAmpRelease, 2800},   {284, kParamUnisonVoices, 4},
  {284, kParamUnisonDetune, 20},   {284, kParamLfoRate, 0.24},   {284, kParamLfoDest, 1},
  {284, kParamLfoAmount, 20},   {284, kParamStereoSpread, 80},   {284, kParamTapeSat, 16},
  {284, kParamTapeMix, 30},   {284, kParamIns1Algo, 2},   {284, kParamIns1A, 12},
  {284, kParamIns1B, 36},   {284, kParamIns1C, 44},   {284, kParamIns1Mix, 28},
  {284, kParamIns2Algo, 2},   {284, kParamIns2A, 26},   {284, kParamIns2B, 34},
  {284, kParamIns2C, 40},   {284, kParamIns2Mix, 16},   {284, kParamMstAlgo, 2},
  {284, kParamMstA, 24},   {284, kParamMstB, 18},   {284, kParamMstC, 30},
  {284, kParamMstMix, 10},
  {285, kParamEngine, 0},   {285, kParamPolyMode, 0},   {285, kParamOsc1Shape, 26},
  {285, kParamOsc1Level, 40},   {285, kParamOsc2Level, 0},   {285, kParamOsc3Enable, 1},
  {285, kParamOsc3Source, 1},   {285, kParamOsc3Morph, 2.8},   {285, kParamOsc3Level, 88},
  {285, kParamOsc3Pitch, 12},   {285, kParamSubLevel, 16},   {285, kParamNoiseLevel, 8},
  {285, kParamFilterType, 0},   {285, kParamFilterCutoff, 6000},   {285, kParamFilterReso, 16},
  {285, kParamFilterEnv, 22},   {285, kParamFiltAttack, 154},   {285, kParamFiltDecay, 2000},
  {285, kParamFiltSustain, 58},   {285, kParamFiltRelease, 2400},   {285, kParamAmpAttack, 220},
  {285, kParamAmpDecay, 2200},   {285, kParamAmpSustain, 78},   {285, kParamAmpRelease, 2400},
  {285, kParamUnisonVoices, 4},   {285, kParamUnisonDetune, 16},   {285, kParamStereoSpread, 82},
  {285, kParamTapeSat, 18},   {285, kParamTapeMix, 30},   {285, kParamIns1Algo, 2},
  {285, kParamIns1A, 18},   {285, kParamIns1B, 44},   {285, kParamIns1C, 52},
  {285, kParamIns1Mix, 34},   {285, kParamIns2Algo, 3},   {285, kParamIns2A, 62},
  {285, kParamIns2B, 44},   {285, kParamIns2C, 50},   {285, kParamIns2Mix, 18},
  {285, kParamMstAlgo, 2},   {285, kParamMstA, 24},   {285, kParamMstB, 18},
  {285, kParamMstC, 30},   {285, kParamMstMix, 10},
  {286, kParamEngine, 0},   {286, kParamPolyMode, 0},   {286, kParamOsc1Shape, 26},
  {286, kParamOsc1Level, 40},   {286, kParamOsc2Level, 0},   {286, kParamOsc3Enable, 1},
  {286, kParamOsc3Source, 1},   {286, kParamOsc3Morph, 5.4},   {286, kParamOsc3Level, 88},
  {286, kParamOsc3Pitch, 12},   {286, kParamSubLevel, 16},   {286, kParamNoiseLevel, 8},
  {286, kParamFilterType, 0},   {286, kParamFilterCutoff, 5200},   {286, kParamFilterReso, 18},
  {286, kParamFilterEnv, 22},   {286, kParamFiltAttack, 182},   {286, kParamFiltDecay, 2000},
  {286, kParamFiltSustain, 58},   {286, kParamFiltRelease, 2600},   {286, kParamAmpAttack, 260},
  {286, kParamAmpDecay, 2200},   {286, kParamAmpSustain, 78},   {286, kParamAmpRelease, 2600},
  {286, kParamUnisonVoices, 4},   {286, kParamUnisonDetune, 16},   {286, kParamStereoSpread, 82},
  {286, kParamTapeSat, 18},   {286, kParamTapeMix, 30},   {286, kParamIns1Algo, 3},
  {286, kParamIns1A, 58},   {286, kParamIns1B, 42},   {286, kParamIns1C, 48},
  {286, kParamIns1Mix, 20},   {286, kParamIns2Algo, 2},   {286, kParamIns2A, 26},
  {286, kParamIns2B, 34},   {286, kParamIns2C, 40},   {286, kParamIns2Mix, 16},
  {286, kParamMstAlgo, 2},   {286, kParamMstA, 24},   {286, kParamMstB, 18},
  {286, kParamMstC, 30},   {286, kParamMstMix, 10},
  {287, kParamEngine, 0},   {287, kParamPolyMode, 0},   {287, kParamOsc1Shape, 26},
  {287, kParamOsc1Level, 40},   {287, kParamOsc2Level, 0},   {287, kParamOsc3Enable, 1},
  {287, kParamOsc3Source, 1},   {287, kParamOsc3Morph, 3},   {287, kParamOsc3Level, 88},
  {287, kParamOsc3Pitch, 12},   {287, kParamSubLevel, 16},   {287, kParamNoiseLevel, 8},
  {287, kParamFilterType, 0},   {287, kParamFilterCutoff, 5600},   {287, kParamFilterReso, 20},
  {287, kParamFilterEnv, 22},   {287, kParamFiltAttack, 210},   {287, kParamFiltDecay, 2000},
  {287, kParamFiltSustain, 58},   {287, kParamFiltRelease, 2800},   {287, kParamAmpAttack, 300},
  {287, kParamAmpDecay, 2200},   {287, kParamAmpSustain, 78},   {287, kParamAmpRelease, 2800},
  {287, kParamUnisonVoices, 6},   {287, kParamUnisonDetune, 16},   {287, kParamStereoSpread, 90},
  {287, kParamTapeSat, 18},   {287, kParamTapeMix, 30},   {287, kParamIns1Algo, 2},
  {287, kParamIns1A, 12},   {287, kParamIns1B, 36},   {287, kParamIns1C, 44},
  {287, kParamIns1Mix, 28},   {287, kParamIns2Algo, 3},   {287, kParamIns2A, 62},
  {287, kParamIns2B, 44},   {287, kParamIns2C, 50},   {287, kParamIns2Mix, 18},
  {287, kParamMstAlgo, 2},   {287, kParamMstA, 24},   {287, kParamMstB, 18},
  {287, kParamMstC, 30},   {287, kParamMstMix, 10},
  {288, kParamEngine, 0},   {288, kParamPolyMode, 0},   {288, kParamOsc1Shape, 26},
  {288, kParamOsc1Level, 40},   {288, kParamOsc2Level, 0},   {288, kParamOsc3Enable, 1},
  {288, kParamOsc3Source, 1},   {288, kParamOsc3Morph, 4.2},   {288, kParamOsc3Level, 88},
  {288, kParamOsc3Pitch, 12},   {288, kParamSubLevel, 16},   {288, kParamNoiseLevel, 8},
  {288, kParamFilterType, 0},   {288, kParamFilterCutoff, 5400},   {288, kParamFilterReso, 22},
  {288, kParamFilterEnv, 22},   {288, kParamFiltAttack, 168},   {288, kParamFiltDecay, 2000},
  {288, kParamFiltSustain, 58},   {288, kParamFiltRelease, 2500},   {288, kParamAmpAttack, 240},
  {288, kParamAmpDecay, 2200},   {288, kParamAmpSustain, 78},   {288, kParamAmpRelease, 2500},
  {288, kParamUnisonVoices, 4},   {288, kParamUnisonDetune, 16},   {288, kParamStereoSpread, 82},
  {288, kParamTapeSat, 18},   {288, kParamTapeMix, 52},   {288, kParamTapeWow, 54},
  {288, kParamTapeFlutter, 44},   {288, kParamIns1Algo, 2},   {288, kParamIns1A, 18},
  {288, kParamIns1B, 44},   {288, kParamIns1C, 52},   {288, kParamIns1Mix, 34},
  {288, kParamIns2Algo, 2},   {288, kParamIns2A, 26},   {288, kParamIns2B, 34},
  {288, kParamIns2C, 40},   {288, kParamIns2Mix, 16},   {288, kParamMstAlgo, 2},
  {288, kParamMstA, 24},   {288, kParamMstB, 18},   {288, kParamMstC, 30},
  {288, kParamMstMix, 10},
  // strings/fx
  {289, kParamEngine, 0},   {289, kParamPolyMode, 0},   {289, kParamOsc1Shape, 42},
  {289, kParamOsc1Level, 78},   {289, kParamOsc2Shape, 40},   {289, kParamOsc2Pitch, -12},
  {289, kParamOsc2Level, 66},   {289, kParamSubLevel, 18},   {289, kParamNoiseLevel, 8},
  {289, kParamFilterType, 0},   {289, kParamFilterCutoff, 6200},   {289, kParamFilterReso, 14},
  {289, kParamFilterEnv, 30},   {289, kParamFiltAttack, 112},   {289, kParamFiltDecay, 1600},
  {289, kParamFiltSustain, 54},   {289, kParamFiltRelease, 2200},   {289, kParamAmpAttack, 140},
  {289, kParamAmpDecay, 1800},   {289, kParamAmpSustain, 78},   {289, kParamAmpRelease, 2200},
  {289, kParamUnisonVoices, 4},   {289, kParamUnisonDetune, 18},   {289, kParamFilterKeyTrack, 36},
  {289, kParamStereoSpread, 72},   {289, kParamTapeSat, 14},   {289, kParamTapeMix, 26},
  {289, kParamIns1Algo, 3},   {289, kParamIns1A, 48},   {289, kParamIns1B, 38},
  {289, kParamIns1C, 58},   {289, kParamIns1Mix, 26},   {289, kParamIns2Algo, 4},
  {289, kParamIns2A, 30},   {289, kParamIns2B, 40},   {289, kParamIns2C, 20},
  {289, kParamIns2Mix, 20},   {289, kParamMstAlgo, 2},   {289, kParamMstA, 24},
  {289, kParamMstB, 18},   {289, kParamMstC, 30},   {289, kParamMstMix, 10},
  {290, kParamEngine, 0},   {290, kParamPolyMode, 0},   {290, kParamOsc1Shape, 42},
  {290, kParamOsc1Level, 78},   {290, kParamOsc2Shape, 40},   {290, kParamOsc2Pitch, -12},
  {290, kParamOsc2Level, 66},   {290, kParamSubLevel, 18},   {290, kParamNoiseLevel, 12},
  {290, kParamFilterType, 0},   {290, kParamFilterCutoff, 5400},   {290, kParamFilterReso, 16},
  {290, kParamFilterEnv, 30},   {290, kParamFiltAttack, 144},   {290, kParamFiltDecay, 1600},
  {290, kParamFiltSustain, 54},   {290, kParamFiltRelease, 2000},   {290, kParamAmpAttack, 180},
  {290, kParamAmpDecay, 1800},   {290, kParamAmpSustain, 78},   {290, kParamAmpRelease, 2000},
  {290, kParamUnisonVoices, 4},   {290, kParamUnisonDetune, 18},   {290, kParamFilterKeyTrack, 36},
  {290, kParamStereoSpread, 72},   {290, kParamTapeSat, 14},   {290, kParamTapeMix, 26},
  {290, kParamIns1Algo, 4},   {290, kParamIns1A, 34},   {290, kParamIns1B, 42},
  {290, kParamIns1C, 22},   {290, kParamIns1Mix, 22},   {290, kParamIns2Algo, 3},
  {290, kParamIns2A, 44},   {290, kParamIns2B, 36},   {290, kParamIns2C, 52},
  {290, kParamIns2Mix, 18},   {290, kParamMstAlgo, 2},   {290, kParamMstA, 24},
  {290, kParamMstB, 18},   {290, kParamMstC, 30},   {290, kParamMstMix, 10},
  {291, kParamEngine, 0},   {291, kParamPolyMode, 0},   {291, kParamOsc1Shape, 42},
  {291, kParamOsc1Level, 78},   {291, kParamOsc2Shape, 40},   {291, kParamOsc2Pitch, -12},
  {291, kParamOsc2Level, 66},   {291, kParamSubLevel, 18},   {291, kParamNoiseLevel, 8},
  {291, kParamFilterType, 0},   {291, kParamFilterCutoff, 7600},   {291, kParamFilterReso, 12},
  {291, kParamFilterEnv, 30},   {291, kParamFiltAttack, 88},   {291, kParamFiltDecay, 1600},
  {291, kParamFiltSustain, 54},   {291, kParamFiltRelease, 1800},   {291, kParamAmpAttack, 110},
  {291, kParamAmpDecay, 1800},   {291, kParamAmpSustain, 78},   {291, kParamAmpRelease, 1800},
  {291, kParamUnisonVoices, 4},   {291, kParamUnisonDetune, 18},   {291, kParamFilterKeyTrack, 36},
  {291, kParamStereoSpread, 72},   {291, kParamTapeSat, 14},   {291, kParamTapeMix, 26},
  {291, kParamIns1Algo, 2},   {291, kParamIns1A, 24},   {291, kParamIns1B, 46},
  {291, kParamIns1C, 56},   {291, kParamIns1Mix, 30},   {291, kParamIns2Algo, 4},
  {291, kParamIns2A, 30},   {291, kParamIns2B, 40},   {291, kParamIns2C, 20},
  {291, kParamIns2Mix, 20},   {291, kParamMstAlgo, 2},   {291, kParamMstA, 24},
  {291, kParamMstB, 18},   {291, kParamMstC, 30},   {291, kParamMstMix, 10},
  {292, kParamEngine, 0},   {292, kParamPolyMode, 0},   {292, kParamOsc1Shape, 42},
  {292, kParamOsc1Level, 78},   {292, kParamOsc2Shape, 40},   {292, kParamOsc2Pitch, -24},
  {292, kParamOsc2Level, 66},   {292, kParamSubLevel, 18},   {292, kParamNoiseLevel, 8},
  {292, kParamFilterType, 0},   {292, kParamFilterCutoff, 4200},   {292, kParamFilterReso, 18},
  {292, kParamFilterEnv, 30},   {292, kParamFiltAttack, 160},   {292, kParamFiltDecay, 1600},
  {292, kParamFiltSustain, 54},   {292, kParamFiltRelease, 2400},   {292, kParamAmpAttack, 200},
  {292, kParamAmpDecay, 1800},   {292, kParamAmpSustain, 78},   {292, kParamAmpRelease, 2400},
  {292, kParamUnisonVoices, 4},   {292, kParamUnisonDetune, 18},   {292, kParamFilterKeyTrack, 36},
  {292, kParamStereoSpread, 72},   {292, kParamTapeSat, 14},   {292, kParamTapeMix, 26},
  {292, kParamIns1Algo, 3},   {292, kParamIns1A, 48},   {292, kParamIns1B, 38},
  {292, kParamIns1C, 58},   {292, kParamIns1Mix, 26},   {292, kParamIns2Algo, 3},
  {292, kParamIns2A, 44},   {292, kParamIns2B, 36},   {292, kParamIns2C, 52},
  {292, kParamIns2Mix, 18},   {292, kParamMstAlgo, 2},   {292, kParamMstA, 24},
  {292, kParamMstB, 18},   {292, kParamMstC, 30},   {292, kParamMstMix, 10},
  {293, kParamEngine, 0},   {293, kParamPolyMode, 0},   {293, kParamOsc1Shape, 42},
  {293, kParamOsc1Level, 78},   {293, kParamOsc2Shape, 40},   {293, kParamOsc2Pitch, -12},
  {293, kParamOsc2Level, 66},   {293, kParamSubLevel, 18},   {293, kParamNoiseLevel, 8},
  {293, kParamFilterType, 0},   {293, kParamFilterCutoff, 5800},   {293, kParamFilterReso, 14},
  {293, kParamFilterEnv, 30},   {293, kParamFiltAttack, 240},   {293, kParamFiltDecay, 1600},
  {293, kParamFiltSustain, 54},   {293, kParamFiltRelease, 2600},   {293, kParamAmpAttack, 300},
  {293, kParamAmpDecay, 1800},   {293, kParamAmpSustain, 78},   {293, kParamAmpRelease, 2600},
  {293, kParamUnisonVoices, 4},   {293, kParamUnisonDetune, 18},   {293, kParamFilterKeyTrack, 36},
  {293, kParamStereoSpread, 72},   {293, kParamTapeSat, 14},   {293, kParamTapeMix, 26},
  {293, kParamIns1Algo, 4},   {293, kParamIns1A, 34},   {293, kParamIns1B, 42},
  {293, kParamIns1C, 22},   {293, kParamIns1Mix, 22},   {293, kParamIns2Algo, 4},
  {293, kParamIns2A, 30},   {293, kParamIns2B, 40},   {293, kParamIns2C, 20},
  {293, kParamIns2Mix, 20},   {293, kParamMstAlgo, 2},   {293, kParamMstA, 24},
  {293, kParamMstB, 18},   {293, kParamMstC, 30},   {293, kParamMstMix, 10},
  {294, kParamEngine, 0},   {294, kParamPolyMode, 0},   {294, kParamOsc1Shape, 42},
  {294, kParamOsc1Level, 78},   {294, kParamOsc2Shape, 40},   {294, kParamOsc2Pitch, -12},
  {294, kParamOsc2Level, 66},   {294, kParamSubLevel, 18},   {294, kParamNoiseLevel, 8},
  {294, kParamFilterType, 0},   {294, kParamFilterCutoff, 6800},   {294, kParamFilterReso, 16},
  {294, kParamFilterEnv, 30},   {294, kParamFiltAttack, 56},   {294, kParamFiltDecay, 1600},
  {294, kParamFiltSustain, 54},   {294, kParamFiltRelease, 1400},   {294, kParamAmpAttack, 70},
  {294, kParamAmpDecay, 1800},   {294, kParamAmpSustain, 78},   {294, kParamAmpRelease, 1400},
  {294, kParamUnisonVoices, 4},   {294, kParamUnisonDetune, 18},   {294, kParamFilterKeyTrack, 36},
  {294, kParamStereoSpread, 72},   {294, kParamTapeSat, 14},   {294, kParamTapeMix, 26},
  {294, kParamIns1Algo, 2},   {294, kParamIns1A, 24},   {294, kParamIns1B, 46},
  {294, kParamIns1C, 56},   {294, kParamIns1Mix, 30},   {294, kParamIns2Algo, 3},
  {294, kParamIns2A, 44},   {294, kParamIns2B, 36},   {294, kParamIns2C, 52},
  {294, kParamIns2Mix, 18},   {294, kParamMstAlgo, 2},   {294, kParamMstA, 24},
  {294, kParamMstB, 18},   {294, kParamMstC, 30},   {294, kParamMstMix, 10},
  {295, kParamEngine, 0},   {295, kParamPolyMode, 0},   {295, kParamOsc1Shape, 42},
  {295, kParamOsc1Level, 78},   {295, kParamOsc2Shape, 40},   {295, kParamOsc2Pitch, -12},
  {295, kParamOsc2Level, 66},   {295, kParamSubLevel, 18},   {295, kParamNoiseLevel, 8},
  {295, kParamFilterType, 0},   {295, kParamFilterCutoff, 5600},   {295, kParamFilterReso, 20},
  {295, kParamFilterEnv, 42},   {295, kParamFiltAttack, 208},   {295, kParamFiltDecay, 1600},
  {295, kParamFiltSustain, 54},   {295, kParamFiltRelease, 2800},   {295, kParamAmpAttack, 260},
  {295, kParamAmpDecay, 1800},   {295, kParamAmpSustain, 78},   {295, kParamAmpRelease, 2800},
  {295, kParamUnisonVoices, 4},   {295, kParamUnisonDetune, 18},   {295, kParamFilterKeyTrack, 36},
  {295, kParamStereoSpread, 72},   {295, kParamTapeSat, 14},   {295, kParamTapeMix, 26},
  {295, kParamIns1Algo, 3},   {295, kParamIns1A, 48},   {295, kParamIns1B, 38},
  {295, kParamIns1C, 58},   {295, kParamIns1Mix, 26},   {295, kParamIns2Algo, 4},
  {295, kParamIns2A, 30},   {295, kParamIns2B, 40},   {295, kParamIns2C, 20},
  {295, kParamIns2Mix, 20},   {295, kParamMstAlgo, 2},   {295, kParamMstA, 24},
  {295, kParamMstB, 18},   {295, kParamMstC, 30},   {295, kParamMstMix, 10},
  {296, kParamEngine, 0},   {296, kParamPolyMode, 0},   {296, kParamOsc1Shape, 42},
  {296, kParamOsc1Level, 78},   {296, kParamOsc2Shape, 40},   {296, kParamOsc2Pitch, -12},
  {296, kParamOsc2Level, 66},   {296, kParamSubLevel, 18},   {296, kParamNoiseLevel, 8},
  {296, kParamFilterType, 0},   {296, kParamFilterCutoff, 5200},   {296, kParamFilterReso, 18},
  {296, kParamFilterEnv, 30},   {296, kParamFiltAttack, 128},   {296, kParamFiltDecay, 1600},
  {296, kParamFiltSustain, 54},   {296, kParamFiltRelease, 2200},   {296, kParamAmpAttack, 160},
  {296, kParamAmpDecay, 1800},   {296, kParamAmpSustain, 78},   {296, kParamAmpRelease, 2200},
  {296, kParamUnisonVoices, 4},   {296, kParamUnisonDetune, 18},   {296, kParamFilterKeyTrack, 36},
  {296, kParamStereoSpread, 72},   {296, kParamTapeSat, 32},   {296, kParamTapeMix, 42},
  {296, kParamTapeWow, 38},   {296, kParamIns1Algo, 4},   {296, kParamIns1A, 34},
  {296, kParamIns1B, 42},   {296, kParamIns1C, 22},   {296, kParamIns1Mix, 22},
  {296, kParamIns2Algo, 3},   {296, kParamIns2A, 44},   {296, kParamIns2B, 36},
  {296, kParamIns2C, 52},   {296, kParamIns2Mix, 18},   {296, kParamMstAlgo, 2},
  {296, kParamMstA, 24},   {296, kParamMstB, 18},   {296, kParamMstC, 30},
  {296, kParamMstMix, 10},
  {297, kParamEngine, 0},   {297, kParamPolyMode, 0},   {297, kParamOsc1Shape, 42},
  {297, kParamOsc1Level, 78},   {297, kParamOsc2Shape, 40},   {297, kParamOsc2Pitch, 0},
  {297, kParamOsc2Level, 66},   {297, kParamSubLevel, 18},   {297, kParamNoiseLevel, 8},
  {297, kParamFilterType, 0},   {297, kParamFilterCutoff, 6400},   {297, kParamFilterReso, 14},
  {297, kParamFilterEnv, 30},   {297, kParamFiltAttack, 104},   {297, kParamFiltDecay, 1600},
  {297, kParamFiltSustain, 54},   {297, kParamFiltRelease, 2000},   {297, kParamAmpAttack, 130},
  {297, kParamAmpDecay, 1800},   {297, kParamAmpSustain, 78},   {297, kParamAmpRelease, 2000},
  {297, kParamUnisonVoices, 4},   {297, kParamUnisonDetune, 18},   {297, kParamFilterKeyTrack, 36},
  {297, kParamStereoSpread, 72},   {297, kParamTapeSat, 14},   {297, kParamTapeMix, 26},
  {297, kParamIns1Algo, 2},   {297, kParamIns1A, 24},   {297, kParamIns1B, 46},
  {297, kParamIns1C, 56},   {297, kParamIns1Mix, 30},   {297, kParamIns2Algo, 4},
  {297, kParamIns2A, 30},   {297, kParamIns2B, 40},   {297, kParamIns2C, 20},
  {297, kParamIns2Mix, 20},   {297, kParamMstAlgo, 2},   {297, kParamMstA, 24},
  {297, kParamMstB, 18},   {297, kParamMstC, 30},   {297, kParamMstMix, 10},
  {298, kParamEngine, 0},   {298, kParamPolyMode, 0},   {298, kParamOsc1Shape, 42},
  {298, kParamOsc1Level, 78},   {298, kParamOsc2Shape, 40},   {298, kParamOsc2Pitch, -12},
  {298, kParamOsc2Level, 66},   {298, kParamSubLevel, 8},   {298, kParamNoiseLevel, 8},
  {298, kParamFilterType, 0},   {298, kParamFilterCutoff, 8200},   {298, kParamFilterReso, 12},
  {298, kParamFilterEnv, 30},   {298, kParamFiltAttack, 96},   {298, kParamFiltDecay, 1600},
  {298, kParamFiltSustain, 54},   {298, kParamFiltRelease, 1800},   {298, kParamAmpAttack, 120},
  {298, kParamAmpDecay, 1800},   {298, kParamAmpSustain, 78},   {298, kParamAmpRelease, 1800},
  {298, kParamUnisonVoices, 4},   {298, kParamUnisonDetune, 18},   {298, kParamFilterKeyTrack, 36},
  {298, kParamStereoSpread, 72},   {298, kParamTapeSat, 14},   {298, kParamTapeMix, 26},
  {298, kParamIns1Algo, 3},   {298, kParamIns1A, 48},   {298, kParamIns1B, 38},
  {298, kParamIns1C, 58},   {298, kParamIns1Mix, 26},   {298, kParamIns2Algo, 3},
  {298, kParamIns2A, 44},   {298, kParamIns2B, 36},   {298, kParamIns2C, 52},
  {298, kParamIns2Mix, 18},   {298, kParamMstAlgo, 2},   {298, kParamMstA, 24},
  {298, kParamMstB, 18},   {298, kParamMstC, 30},   {298, kParamMstMix, 10},
  {299, kParamEngine, 0},   {299, kParamPolyMode, 0},   {299, kParamOsc1Shape, 56},
  {299, kParamOsc1Level, 80},   {299, kParamOsc2Shape, 58},   {299, kParamOsc2Pitch, 7},
  {299, kParamOsc2Level, 70},   {299, kParamOscRing, 1},   {299, kParamNoiseLevel, 34},
  {299, kParamSubLevel, 10},   {299, kParamFilterType, 0},   {299, kParamFilterCutoff, 4800},
  {299, kParamFilterReso, 44},   {299, kParamFilterEnv, 64},   {299, kParamFiltAttack, 20},
  {299, kParamFiltDecay, 2000},   {299, kParamFiltSustain, 40},   {299, kParamFiltRelease, 2400},
  {299, kParamAmpAttack, 20},   {299, kParamAmpDecay, 2400},   {299, kParamAmpSustain, 62},
  {299, kParamAmpRelease, 2400},   {299, kParamLfoRate, 0.6},   {299, kParamLfoDest, 16},
  {299, kParamLfoAmount, 72},   {299, kParamStereoSpread, 86},   {299, kParamStereoDelay, 40},
  {299, kParamTapeSat, 30},   {299, kParamTapeMix, 40},   {299, kParamIns1Algo, 4},
  {299, kParamIns1A, 34},   {299, kParamIns1B, 42},   {299, kParamIns1C, 22},
  {299, kParamIns1Mix, 22},   {299, kParamIns2Algo, 4},   {299, kParamIns2A, 30},
  {299, kParamIns2B, 40},   {299, kParamIns2C, 20},   {299, kParamIns2Mix, 20},
  {299, kParamMstAlgo, 2},   {299, kParamMstA, 24},   {299, kParamMstB, 18},
  {299, kParamMstC, 30},   {299, kParamMstMix, 10},
  {300, kParamEngine, 0},   {300, kParamPolyMode, 0},   {300, kParamOsc1Shape, 56},
  {300, kParamOsc1Level, 30},   {300, kParamOsc2Shape, 58},   {300, kParamOsc2Pitch, 7},
  {300, kParamOsc2Level, 70},   {300, kParamOscRing, 1},   {300, kParamNoiseLevel, 64},
  {300, kParamSubLevel, 10},   {300, kParamFilterType, 0},   {300, kParamFilterCutoff, 6200},
  {300, kParamFilterReso, 30},   {300, kParamFilterEnv, 64},   {300, kParamFiltAttack, 400},
  {300, kParamFiltDecay, 2000},   {300, kParamFiltSustain, 40},   {300, kParamFiltRelease, 3200},
  {300, kParamAmpAttack, 400},   {300, kParamAmpDecay, 2400},   {300, kParamAmpSustain, 62},
  {300, kParamAmpRelease, 3200},   {300, kParamLfoRate, 0.6},   {300, kParamLfoDest, 16},
  {300, kParamLfoAmount, 72},   {300, kParamStereoSpread, 86},   {300, kParamStereoDelay, 40},
  {300, kParamTapeSat, 30},   {300, kParamTapeMix, 40},   {300, kParamIns1Algo, 2},
  {300, kParamIns1A, 24},   {300, kParamIns1B, 46},   {300, kParamIns1C, 56},
  {300, kParamIns1Mix, 30},   {300, kParamIns2Algo, 3},   {300, kParamIns2A, 44},
  {300, kParamIns2B, 36},   {300, kParamIns2C, 52},   {300, kParamIns2Mix, 18},
  {300, kParamMstAlgo, 2},   {300, kParamMstA, 24},   {300, kParamMstB, 18},
  {300, kParamMstC, 30},   {300, kParamMstMix, 10},
  {301, kParamEngine, 0},   {301, kParamPolyMode, 0},   {301, kParamOsc1Shape, 56},
  {301, kParamOsc1Level, 80},   {301, kParamOsc2Shape, 58},   {301, kParamOsc2Pitch, 7},
  {301, kParamOsc2Level, 70},   {301, kParamOscRing, 1},   {301, kParamNoiseLevel, 34},
  {301, kParamSubLevel, 10},   {301, kParamFilterType, 0},   {301, kParamFilterCutoff, 3200},
  {301, kParamFilterReso, 52},   {301, kParamFilterEnv, 84},   {301, kParamFiltAttack, 200},
  {301, kParamFiltDecay, 2000},   {301, kParamFiltSustain, 40},   {301, kParamFiltRelease, 2800},
  {301, kParamAmpAttack, 200},   {301, kParamAmpDecay, 2400},   {301, kParamAmpSustain, 62},
  {301, kParamAmpRelease, 2800},   {301, kParamLfoRate, 1.6},   {301, kParamLfoDest, 16},
  {301, kParamLfoAmount, 72},   {301, kParamStereoSpread, 86},   {301, kParamStereoDelay, 40},
  {301, kParamTapeSat, 30},   {301, kParamTapeMix, 40},   {301, kParamIns1Algo, 3},
  {301, kParamIns1A, 48},   {301, kParamIns1B, 38},   {301, kParamIns1C, 58},
  {301, kParamIns1Mix, 26},   {301, kParamIns2Algo, 4},   {301, kParamIns2A, 30},
  {301, kParamIns2B, 40},   {301, kParamIns2C, 20},   {301, kParamIns2Mix, 20},
  {301, kParamMstAlgo, 2},   {301, kParamMstA, 24},   {301, kParamMstB, 18},
  {301, kParamMstC, 30},   {301, kParamMstMix, 10},
  {302, kParamEngine, 0},   {302, kParamPolyMode, 0},   {302, kParamOsc1Shape, 56},
  {302, kParamOsc1Level, 80},   {302, kParamOsc2Shape, 58},   {302, kParamOsc2Pitch, 7},
  {302, kParamOsc2Level, 70},   {302, kParamOscRing, 1},   {302, kParamNoiseLevel, 34},
  {302, kParamSubLevel, 10},   {302, kParamFilterType, 0},   {302, kParamFilterCutoff, 5400},
  {302, kParamFilterReso, 48},   {302, kParamFilterEnv, 64},   {302, kParamFiltAttack, 30},
  {302, kParamFiltDecay, 2000},   {302, kParamFiltSustain, 40},   {302, kParamFiltRelease, 2600},
  {302, kParamAmpAttack, 30},   {302, kParamAmpDecay, 2400},   {302, kParamAmpSustain, 62},
  {302, kParamAmpRelease, 2600},   {302, kParamLfoRate, 2.4},   {302, kParamLfoDest, 16},
  {302, kParamLfoAmount, 88},   {302, kParamStereoSpread, 86},   {302, kParamStereoDelay, 40},
  {302, kParamTapeSat, 30},   {302, kParamTapeMix, 40},   {302, kParamIns1Algo, 4},
  {302, kParamIns1A, 34},   {302, kParamIns1B, 42},   {302, kParamIns1C, 22},
  {302, kParamIns1Mix, 22},   {302, kParamIns2Algo, 3},   {302, kParamIns2A, 44},
  {302, kParamIns2B, 36},   {302, kParamIns2C, 52},   {302, kParamIns2Mix, 18},
  {302, kParamMstAlgo, 2},   {302, kParamMstA, 24},   {302, kParamMstB, 18},
  {302, kParamMstC, 30},   {302, kParamMstMix, 10},
  {303, kParamEngine, 0},   {303, kParamPolyMode, 0},   {303, kParamOsc1Shape, 56},
  {303, kParamOsc1Level, 80},   {303, kParamOsc2Shape, 58},   {303, kParamOsc2Pitch, 7},
  {303, kParamOsc2Level, 70},   {303, kParamOscRing, 1},   {303, kParamNoiseLevel, 34},
  {303, kParamSubLevel, 10},   {303, kParamFilterType, 0},   {303, kParamFilterCutoff, 5600},
  {303, kParamFilterReso, 36},   {303, kParamFilterEnv, 64},   {303, kParamFiltAttack, 60},
  {303, kParamFiltDecay, 2000},   {303, kParamFiltSustain, 40},   {303, kParamFiltRelease, 2400},
  {303, kParamAmpAttack, 60},   {303, kParamAmpDecay, 2400},   {303, kParamAmpSustain, 62},
  {303, kParamAmpRelease, 2400},   {303, kParamLfoRate, 0.6},   {303, kParamLfoDest, 16},
  {303, kParamLfoAmount, 72},   {303, kParamStereoSpread, 86},   {303, kParamStereoDelay, 40},
  {303, kParamTapeSat, 30},   {303, kParamTapeMix, 56},   {303, kParamTapeWow, 72},
  {303, kParamTapeFlutter, 64},   {303, kParamIns1Algo, 2},   {303, kParamIns1A, 24},
  {303, kParamIns1B, 46},   {303, kParamIns1C, 56},   {303, kParamIns1Mix, 30},
  {303, kParamIns2Algo, 4},   {303, kParamIns2A, 30},   {303, kParamIns2B, 40},
  {303, kParamIns2C, 20},   {303, kParamIns2Mix, 20},   {303, kParamMstAlgo, 2},
  {303, kParamMstA, 24},   {303, kParamMstB, 18},   {303, kParamMstC, 30},
  {303, kParamMstMix, 10},
  {304, kParamEngine, 0},   {304, kParamPolyMode, 0},   {304, kParamOsc1Shape, 56},
  {304, kParamOsc1Level, 80},   {304, kParamOsc2Shape, 58},   {304, kParamOsc2Pitch, 7},
  {304, kParamOsc2Level, 70},   {304, kParamOscRing, 1},   {304, kParamNoiseLevel, 34},
  {304, kParamSubLevel, 10},   {304, kParamFilterType, 0},   {304, kParamFilterCutoff, 5000},
  {304, kParamFilterReso, 40},   {304, kParamFilterEnv, 64},   {304, kParamFiltAttack, 320},
  {304, kParamFiltDecay, 2000},   {304, kParamFiltSustain, 40},   {304, kParamFiltRelease, 3400},
  {304, kParamAmpAttack, 320},   {304, kParamAmpDecay, 2400},   {304, kParamAmpSustain, 62},
  {304, kParamAmpRelease, 3400},   {304, kParamLfoRate, 0.6},   {304, kParamLfoDest, 16},
  {304, kParamLfoAmount, 72},   {304, kParamStereoSpread, 86},   {304, kParamStereoDelay, 40},
  {304, kParamTapeSat, 30},   {304, kParamTapeMix, 40},   {304, kParamIns1Algo, 3},
  {304, kParamIns1A, 48},   {304, kParamIns1B, 38},   {304, kParamIns1C, 58},
  {304, kParamIns1Mix, 26},   {304, kParamIns2Algo, 3},   {304, kParamIns2A, 44},
  {304, kParamIns2B, 36},   {304, kParamIns2C, 52},   {304, kParamIns2Mix, 18},
  {304, kParamMstAlgo, 2},   {304, kParamMstA, 24},   {304, kParamMstB, 18},
  {304, kParamMstC, 30},   {304, kParamMstMix, 10},
  {305, kParamEngine, 0},   {305, kParamPolyMode, 0},   {305, kParamOsc1Shape, 56},
  {305, kParamOsc1Level, 80},   {305, kParamOsc2Shape, 58},   {305, kParamOsc2Pitch, 7},
  {305, kParamOsc2Level, 70},   {305, kParamOscRing, 1},   {305, kParamNoiseLevel, 34},
  {305, kParamSubLevel, 10},   {305, kParamFilterType, 0},   {305, kParamFilterCutoff, 3800},
  {305, kParamFilterReso, 56},   {305, kParamFilterEnv, 64},   {305, kParamFiltAttack, 10},
  {305, kParamFiltDecay, 2000},   {305, kParamFiltSustain, 40},   {305, kParamFiltRelease, 2000},
  {305, kParamAmpAttack, 10},   {305, kParamAmpDecay, 2400},   {305, kParamAmpSustain, 62},
  {305, kParamAmpRelease, 2000},   {305, kParamLfoRate, 0.6},   {305, kParamLfoDest, 16},
  {305, kParamLfoAmount, 72},   {305, kParamStereoSpread, 86},   {305, kParamStereoDelay, 40},
  {305, kParamTapeSat, 30},   {305, kParamTapeMix, 52},   {305, kParamTapeHiss, 48},
  {305, kParamTapePrint, 40},   {305, kParamIns1Algo, 4},   {305, kParamIns1A, 34},
  {305, kParamIns1B, 42},   {305, kParamIns1C, 22},   {305, kParamIns1Mix, 22},
  {305, kParamIns2Algo, 4},   {305, kParamIns2A, 30},   {305, kParamIns2B, 40},
  {305, kParamIns2C, 20},   {305, kParamIns2Mix, 20},   {305, kParamMstAlgo, 2},
  {305, kParamMstA, 24},   {305, kParamMstB, 18},   {305, kParamMstC, 30},
  {305, kParamMstMix, 10},
  {306, kParamEngine, 0},   {306, kParamPolyMode, 0},   {306, kParamOsc1Shape, 56},
  {306, kParamOsc1Level, 80},   {306, kParamOsc2Shape, 58},   {306, kParamOsc2Pitch, 7},
  {306, kParamOsc2Level, 70},   {306, kParamOscRing, 0},   {306, kParamNoiseLevel, 34},
  {306, kParamSubLevel, 10},   {306, kParamFilterType, 0},   {306, kParamFilterCutoff, 2600},
  {306, kParamFilterReso, 46},   {306, kParamFilterEnv, 64},   {306, kParamFiltAttack, 4},
  {306, kParamFiltDecay, 2000},   {306, kParamFiltSustain, 40},   {306, kParamFiltRelease, 1800},
  {306, kParamAmpAttack, 4},   {306, kParamAmpDecay, 2400},   {306, kParamAmpSustain, 62},
  {306, kParamAmpRelease, 1800},   {306, kParamLfoRate, 0.6},   {306, kParamLfoDest, 16},
  {306, kParamLfoAmount, 72},   {306, kParamStereoSpread, 86},   {306, kParamStereoDelay, 40},
  {306, kParamTapeSat, 30},   {306, kParamTapeMix, 40},   {306, kParamPitchAmount, -24},
  {306, kParamPitchDecay, 600},   {306, kParamIns1Algo, 2},   {306, kParamIns1A, 24},
  {306, kParamIns1B, 46},   {306, kParamIns1C, 56},   {306, kParamIns1Mix, 30},
  {306, kParamIns2Algo, 3},   {306, kParamIns2A, 44},   {306, kParamIns2B, 36},
  {306, kParamIns2C, 52},   {306, kParamIns2Mix, 18},   {306, kParamMstAlgo, 2},
  {306, kParamMstA, 24},   {306, kParamMstB, 18},   {306, kParamMstC, 30},
  {306, kParamMstMix, 10},
  {307, kParamEngine, 0},   {307, kParamPolyMode, 0},   {307, kParamOsc1Shape, 56},
  {307, kParamOsc1Level, 80},   {307, kParamOsc2Shape, 58},   {307, kParamOsc2Pitch, 7},
  {307, kParamOsc2Level, 70},   {307, kParamOscRing, 0},   {307, kParamNoiseLevel, 22},
  {307, kParamSubLevel, 10},   {307, kParamFilterType, 0},   {307, kParamFilterCutoff, 7800},
  {307, kParamFilterReso, 38},   {307, kParamFilterEnv, 64},   {307, kParamFiltAttack, 240},
  {307, kParamFiltDecay, 2000},   {307, kParamFiltSustain, 40},   {307, kParamFiltRelease, 3000},
  {307, kParamAmpAttack, 240},   {307, kParamAmpDecay, 2400},   {307, kParamAmpSustain, 62},
  {307, kParamAmpRelease, 3000},   {307, kParamLfoRate, 0.6},   {307, kParamLfoDest, 16},
  {307, kParamLfoAmount, 72},   {307, kParamStereoSpread, 86},   {307, kParamStereoDelay, 40},
  {307, kParamTapeSat, 30},   {307, kParamTapeMix, 40},   {307, kParamIns1Algo, 3},
  {307, kParamIns1A, 48},   {307, kParamIns1B, 38},   {307, kParamIns1C, 58},
  {307, kParamIns1Mix, 26},   {307, kParamIns2Algo, 4},   {307, kParamIns2A, 30},
  {307, kParamIns2B, 40},   {307, kParamIns2C, 20},   {307, kParamIns2Mix, 20},
  {307, kParamMstAlgo, 2},   {307, kParamMstA, 24},   {307, kParamMstB, 18},
  {307, kParamMstC, 30},   {307, kParamMstMix, 10},
  {308, kParamEngine, 0},   {308, kParamPolyMode, 0},   {308, kParamOsc1Shape, 56},
  {308, kParamOsc1Level, 80},   {308, kParamOsc2Shape, 58},   {308, kParamOsc2Pitch, 7},
  {308, kParamOsc2Level, 70},   {308, kParamOscRing, 1},   {308, kParamNoiseLevel, 34},
  {308, kParamSubLevel, 10},   {308, kParamFilterType, 0},   {308, kParamFilterCutoff, 6800},
  {308, kParamFilterReso, 50},   {308, kParamFilterEnv, 64},   {308, kParamFiltAttack, 8},
  {308, kParamFiltDecay, 2000},   {308, kParamFiltSustain, 40},   {308, kParamFiltRelease, 2200},
  {308, kParamAmpAttack, 8},   {308, kParamAmpDecay, 2400},   {308, kParamAmpSustain, 62},
  {308, kParamAmpRelease, 2200},   {308, kParamLfoRate, 0.6},   {308, kParamLfoDest, 16},
  {308, kParamLfoAmount, 72},   {308, kParamStereoSpread, 86},   {308, kParamStereoDelay, 40},
  {308, kParamTapeSat, 30},   {308, kParamTapeMix, 40},   {308, kParamPitchAmount, -24},
  {308, kParamPitchDecay, 900},   {308, kParamIns1Algo, 4},   {308, kParamIns1A, 34},
  {308, kParamIns1B, 42},   {308, kParamIns1C, 22},   {308, kParamIns1Mix, 22},
  {308, kParamIns2Algo, 3},   {308, kParamIns2A, 44},   {308, kParamIns2B, 36},
  {308, kParamIns2C, 52},   {308, kParamIns2Mix, 18},   {308, kParamMstAlgo, 2},
  {308, kParamMstA, 24},   {308, kParamMstB, 18},   {308, kParamMstC, 30},
  {308, kParamMstMix, 10},
  {309, kParamEngine, 0},   {309, kParamPolyMode, 0},   {309, kParamOsc1Shape, 42},
  {309, kParamOsc1Level, 78},   {309, kParamOsc2Shape, 40},   {309, kParamOsc2Pitch, -12},
  {309, kParamOsc2Level, 66},   {309, kParamSubLevel, 18},   {309, kParamNoiseLevel, 8},
  {309, kParamFilterType, 0},   {309, kParamFilterCutoff, 7000},   {309, kParamFilterReso, 14},
  {309, kParamFilterEnv, 30},   {309, kParamFiltAttack, 120},   {309, kParamFiltDecay, 1600},
  {309, kParamFiltSustain, 54},   {309, kParamFiltRelease, 2000},   {309, kParamAmpAttack, 150},
  {309, kParamAmpDecay, 1800},   {309, kParamAmpSustain, 78},   {309, kParamAmpRelease, 2000},
  {309, kParamUnisonVoices, 4},   {309, kParamUnisonDetune, 18},   {309, kParamFilterKeyTrack, 36},
  {309, kParamStereoSpread, 74},   {309, kParamTapeSat, 14},   {309, kParamTapeMix, 26},
  {309, kParamIns1Algo, 2},   {309, kParamIns1A, 24},   {309, kParamIns1B, 46},
  {309, kParamIns1C, 56},   {309, kParamIns1Mix, 30},   {309, kParamIns2Algo, 4},
  {309, kParamIns2A, 30},   {309, kParamIns2B, 40},   {309, kParamIns2C, 20},
  {309, kParamIns2Mix, 20},   {309, kParamMstAlgo, 2},   {309, kParamMstA, 24},
  {309, kParamMstB, 18},   {309, kParamMstC, 30},   {309, kParamMstMix, 10},
  {310, kParamEngine, 0},   {310, kParamPolyMode, 0},   {310, kParamOsc1Shape, 42},
  {310, kParamOsc1Level, 78},   {310, kParamOsc2Shape, 40},   {310, kParamOsc2Pitch, -12},
  {310, kParamOsc2Level, 66},   {310, kParamSubLevel, 24},   {310, kParamNoiseLevel, 8},
  {310, kParamFilterType, 0},   {310, kParamFilterCutoff, 5800},   {310, kParamFilterReso, 16},
  {310, kParamFilterEnv, 30},   {310, kParamFiltAttack, 160},   {310, kParamFiltDecay, 1600},
  {310, kParamFiltSustain, 54},   {310, kParamFiltRelease, 2200},   {310, kParamAmpAttack, 200},
  {310, kParamAmpDecay, 1800},   {310, kParamAmpSustain, 78},   {310, kParamAmpRelease, 2200},
  {310, kParamUnisonVoices, 4},   {310, kParamUnisonDetune, 18},   {310, kParamFilterKeyTrack, 36},
  {310, kParamStereoSpread, 74},   {310, kParamTapeSat, 14},   {310, kParamTapeMix, 26},
  {310, kParamIns1Algo, 3},   {310, kParamIns1A, 48},   {310, kParamIns1B, 38},
  {310, kParamIns1C, 58},   {310, kParamIns1Mix, 26},   {310, kParamIns2Algo, 3},
  {310, kParamIns2A, 44},   {310, kParamIns2B, 36},   {310, kParamIns2C, 52},
  {310, kParamIns2Mix, 18},   {310, kParamMstAlgo, 2},   {310, kParamMstA, 24},
  {310, kParamMstB, 18},   {310, kParamMstC, 30},   {310, kParamMstMix, 10},
  {311, kParamEngine, 0},   {311, kParamPolyMode, 0},   {311, kParamOsc1Shape, 42},
  {311, kParamOsc1Level, 78},   {311, kParamOsc2Shape, 40},   {311, kParamOsc2Pitch, -12},
  {311, kParamOsc2Level, 66},   {311, kParamSubLevel, 8},   {311, kParamNoiseLevel, 8},
  {311, kParamFilterType, 0},   {311, kParamFilterCutoff, 8600},   {311, kParamFilterReso, 12},
  {311, kParamFilterEnv, 30},   {311, kParamFiltAttack, 104},   {311, kParamFiltDecay, 1600},
  {311, kParamFiltSustain, 54},   {311, kParamFiltRelease, 1900},   {311, kParamAmpAttack, 130},
  {311, kParamAmpDecay, 1800},   {311, kParamAmpSustain, 78},   {311, kParamAmpRelease, 1900},
  {311, kParamUnisonVoices, 4},   {311, kParamUnisonDetune, 18},   {311, kParamFilterKeyTrack, 36},
  {311, kParamStereoSpread, 74},   {311, kParamTapeSat, 14},   {311, kParamTapeMix, 26},
  {311, kParamIns1Algo, 4},   {311, kParamIns1A, 34},   {311, kParamIns1B, 42},
  {311, kParamIns1C, 22},   {311, kParamIns1Mix, 22},   {311, kParamIns2Algo, 4},
  {311, kParamIns2A, 30},   {311, kParamIns2B, 40},   {311, kParamIns2C, 20},
  {311, kParamIns2Mix, 20},   {311, kParamMstAlgo, 2},   {311, kParamMstA, 24},
  {311, kParamMstB, 18},   {311, kParamMstC, 30},   {311, kParamMstMix, 10},
  {312, kParamEngine, 0},   {312, kParamPolyMode, 0},   {312, kParamOsc1Shape, 42},
  {312, kParamOsc1Level, 78},   {312, kParamOsc2Shape, 40},   {312, kParamOsc2Pitch, -24},
  {312, kParamOsc2Level, 66},   {312, kParamSubLevel, 18},   {312, kParamNoiseLevel, 8},
  {312, kParamFilterType, 0},   {312, kParamFilterCutoff, 4600},   {312, kParamFilterReso, 18},
  {312, kParamFilterEnv, 30},   {312, kParamFiltAttack, 176},   {312, kParamFiltDecay, 1600},
  {312, kParamFiltSustain, 54},   {312, kParamFiltRelease, 2400},   {312, kParamAmpAttack, 220},
  {312, kParamAmpDecay, 1800},   {312, kParamAmpSustain, 78},   {312, kParamAmpRelease, 2400},
  {312, kParamUnisonVoices, 4},   {312, kParamUnisonDetune, 18},   {312, kParamFilterKeyTrack, 36},
  {312, kParamStereoSpread, 74},   {312, kParamTapeSat, 14},   {312, kParamTapeMix, 26},
  {312, kParamIns1Algo, 2},   {312, kParamIns1A, 24},   {312, kParamIns1B, 46},
  {312, kParamIns1C, 56},   {312, kParamIns1Mix, 30},   {312, kParamIns2Algo, 3},
  {312, kParamIns2A, 44},   {312, kParamIns2B, 36},   {312, kParamIns2C, 52},
  {312, kParamIns2Mix, 18},   {312, kParamMstAlgo, 2},   {312, kParamMstA, 24},
  {312, kParamMstB, 18},   {312, kParamMstC, 30},   {312, kParamMstMix, 10},
  {313, kParamEngine, 0},   {313, kParamPolyMode, 0},   {313, kParamOsc1Shape, 42},
  {313, kParamOsc1Level, 78},   {313, kParamOsc2Shape, 40},   {313, kParamOsc2Pitch, -12},
  {313, kParamOsc2Level, 66},   {313, kParamSubLevel, 18},   {313, kParamNoiseLevel, 8},
  {313, kParamFilterType, 0},   {313, kParamFilterCutoff, 5600},   {313, kParamFilterReso, 18},
  {313, kParamFilterEnv, 30},   {313, kParamFiltAttack, 144},   {313, kParamFiltDecay, 1600},
  {313, kParamFiltSustain, 54},   {313, kParamFiltRelease, 2200},   {313, kParamAmpAttack, 180},
  {313, kParamAmpDecay, 1800},   {313, kParamAmpSustain, 78},   {313, kParamAmpRelease, 2200},
  {313, kParamUnisonVoices, 4},   {313, kParamUnisonDetune, 18},   {313, kParamFilterKeyTrack, 36},
  {313, kParamStereoSpread, 74},   {313, kParamTapeSat, 34},   {313, kParamTapeMix, 44},
  {313, kParamTapeWow, 40},   {313, kParamIns1Algo, 3},   {313, kParamIns1A, 48},
  {313, kParamIns1B, 38},   {313, kParamIns1C, 58},   {313, kParamIns1Mix, 26},
  {313, kParamIns2Algo, 4},   {313, kParamIns2A, 30},   {313, kParamIns2B, 40},
  {313, kParamIns2C, 20},   {313, kParamIns2Mix, 20},   {313, kParamMstAlgo, 2},
  {313, kParamMstA, 24},   {313, kParamMstB, 18},   {313, kParamMstC, 30},
  {313, kParamMstMix, 10},
  // --- END the MAGS library
};

const keith::PresetSpec kPresetSpecs[kNumPresets] = {
  {"Family 01 / 001 Glass Keys", 1, "Triton", "keys"},
  {"Family 01 / 002 Sparse Keys", 1, "Triton", "keys"},
  {"Family 01 / 003 Soft Tines", 1, "Triton", "keys"},
  {"Family 01 / 004 Bright Clav", 1, "Triton", "keys"},
  {"Family 01 / 005 Drawbar Organ", 1, "Triton", "keys"},
  {"Family 01 / 006 Glass Bells", 1, "Triton", "keys"},
  {"Family 01 / 007 Crystal Pluck", 1, "Triton", "polysynth"},
  {"Family 01 / 008 Key Stab", 1, "Triton", "polysynth"},
  {"Family 01 / 009 Dry Bass", 1, "Triton", "bass"},
  {"Family 01 / 010 Smooth Lead", 1, "Triton", "lead"},
  {"Family 01 / 011 Warm Brass", 1, "Triton", "reeds/horn"},
  {"Family 01 / 012 Silk Strings", 1, "Triton", "strings/fx"},
  {"Family 01 / 013 Soft Choir", 1, "Triton", "pad"},
  {"Family 01 / 014 Bell Pad", 1, "Triton", "pad"},
  {"Family 01 / 015 Slow Motion", 1, "Triton", "pad"},
  {"Family 01 / 016 Shimmer Texture", 1, "Triton", "strings/fx"},
  {"Family 01 / 017 Low Drone", 1, "Triton", "pad"},
  {"Family 01 / 018 Glass Arp", 1, "Triton", "lead"},
  {"Family 01 / 019 Heavy Delay", 1, "Triton", "strings/fx"},
  {"Family 01 / 020 Room Kit", 1, "Triton", "drums"},
  {"Family 01 / 021 Upright Bass", 1, "Triton", "bass"},
  {"Family 01 / 022 Hammer Keys", 1, "Triton", "keys"},
  {"Family 01 / 023 Swell Choir", 1, "Triton", "pad"},
  {"Family 01 / 024 Bright Reed", 1, "Triton", "lead"},
  {"Family 01 / 025 Snap Bass", 1, "Triton", "bass"},
  {"Family 01 / 026 Short Bass", 1, "Triton", "bass"},
  {"Family 02 / 001 Neon Keys", 2, "Triton Extreme", "keys"},
  {"Family 02 / 002 Thin Piano", 2, "Triton Extreme", "keys"},
  {"Family 02 / 003 Thin Clav", 2, "Triton Extreme", "keys"},
  {"Family 02 / 004 Perc Organ", 2, "Triton Extreme", "keys"},
  {"Family 02 / 005 Metal Bells", 2, "Triton Extreme", "keys"},
  {"Family 02 / 006 Neon Pluck", 2, "Triton Extreme", "polysynth"},
  {"Family 02 / 007 Sparse Pluck", 2, "Triton Extreme", "polysynth"},
  {"Family 02 / 008 Thin Stab", 2, "Triton Extreme", "polysynth"},
  {"Family 02 / 009 Hard Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 010 Sparse Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 011 Sync Lead", 2, "Triton Extreme", "lead"},
  {"Family 02 / 012 Stab Brass", 2, "Triton Extreme", "reeds/horn"},
  {"Family 02 / 013 Tight Strings", 2, "Triton Extreme", "strings/fx"},
  {"Family 02 / 014 Breath Choir", 2, "Triton Extreme", "pad"},
  {"Family 02 / 015 Poly Pad", 2, "Triton Extreme", "pad"},
  {"Family 02 / 016 Sweep Motion", 2, "Triton Extreme", "pad"},
  {"Family 02 / 017 Skew Texture", 2, "Triton Extreme", "strings/fx"},
  {"Family 02 / 018 Pulse Drone", 2, "Triton Extreme", "pad"},
  {"Family 02 / 019 Wide Arp", 2, "Triton Extreme", "lead"},
  {"Family 02 / 020 Heavy Crush", 2, "Triton Extreme", "strings/fx"},
  {"Family 02 / 021 Punch Kit", 2, "Triton Extreme", "drums"},
  {"Family 03 / 001 Piano Stack", 3, "Korg M1", "keys"},
  {"Family 03 / 002 Soft EP", 3, "Korg M1", "keys"},
  {"Family 03 / 003 Soul Piano", 3, "Korg M1", "keys"},
  {"Family 03 / 004 Clav Stab", 3, "Korg M1", "keys"},
  {"Family 03 / 005 Dry Clav", 3, "Korg M1", "keys"},
  {"Family 03 / 006 Warm Organ", 3, "Korg M1", "keys"},
  {"Family 03 / 007 Bottle Bells", 3, "Korg M1", "keys"},
  {"Family 03 / 008 Muted Pluck", 3, "Korg M1", "polysynth"},
  {"Family 03 / 009 Reed Stab", 3, "Korg M1", "polysynth"},
  {"Family 03 / 010 Finger Bass", 3, "Korg M1", "bass"},
  {"Family 03 / 011 Solo Lead", 3, "Korg M1", "lead"},
  {"Family 03 / 012 Ensemble Brass", 3, "Korg M1", "reeds/horn"},
  {"Family 03 / 013 Section Strings", 3, "Korg M1", "strings/fx"},
  {"Family 03 / 014 Ooh Choir", 3, "Korg M1", "pad"},
  {"Family 03 / 015 Dream Pad", 3, "Korg M1", "pad"},
  {"Family 03 / 016 Slow Sweep", 3, "Korg M1", "pad"},
  {"Family 03 / 017 Hollow Texture", 3, "Korg M1", "strings/fx"},
  {"Family 03 / 018 Warm Drone", 3, "Korg M1", "pad"},
  {"Family 03 / 019 Mallet Arp", 3, "Korg M1", "lead"},
  {"Family 03 / 020 Slap Delay", 3, "Korg M1", "strings/fx"},
  {"Family 03 / 021 Dry Perc", 3, "Korg M1", "drums"},
  {"Family 04 / 001 Vector Keys", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 002 Phase Piano", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 003 Wave Clav", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 004 Drift Organ", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 005 Chime Bells", 4, "Korg Wavestation", "keys"},
  {"Family 04 / 006 Harp Pluck", 4, "Korg Wavestation", "polysynth"},
  {"Family 04 / 007 Soft Stab", 4, "Korg Wavestation", "polysynth"},
  {"Family 04 / 008 Sub Bass", 4, "Korg Wavestation", "bass"},
  {"Family 04 / 009 Air Lead", 4, "Korg Wavestation", "lead"},
  {"Family 04 / 010 Swell Brass", 4, "Korg Wavestation", "reeds/horn"},
  {"Family 04 / 011 Bowed Strings", 4, "Korg Wavestation", "strings/fx"},
  {"Family 04 / 012 Air Choir", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 013 Cloud Pad", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 014 Sparse Pad", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 015 Motion Pad", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 016 Tilt Texture", 4, "Korg Wavestation", "strings/fx"},
  {"Family 04 / 017 Slow Drone", 4, "Korg Wavestation", "pad"},
  {"Family 04 / 018 Seq Arp", 4, "Korg Wavestation", "lead"},
  {"Family 04 / 019 Wash Delay", 4, "Korg Wavestation", "strings/fx"},
  {"Family 04 / 020 Metal Kit", 4, "Korg Wavestation", "drums"},
  {"Family 05 / 001 Stage Keys", 5, "Korg TR", "keys"},
  {"Family 05 / 002 Reed Piano", 5, "Korg TR", "keys"},
  {"Family 05 / 003 Funk Clav", 5, "Korg TR", "keys"},
  {"Family 05 / 004 Rock Organ", 5, "Korg TR", "keys"},
  {"Family 05 / 005 Ping Bells", 5, "Korg TR", "keys"},
  {"Family 05 / 006 Mute Pluck", 5, "Korg TR", "polysynth"},
  {"Family 05 / 007 Short Stab", 5, "Korg TR", "polysynth"},
  {"Family 05 / 008 Round Bass", 5, "Korg TR", "bass"},
  {"Family 05 / 009 Saw Lead", 5, "Korg TR", "lead"},
  {"Family 05 / 010 Section Brass", 5, "Korg TR", "reeds/horn"},
  {"Family 05 / 011 Small Strings", 5, "Korg TR", "strings/fx"},
  {"Family 05 / 012 Close Choir", 5, "Korg TR", "pad"},
  {"Family 05 / 013 Tape Pad", 5, "Korg TR", "pad"},
  {"Family 05 / 014 Filter Motion", 5, "Korg TR", "pad"},
  {"Family 05 / 015 Broken Texture", 5, "Korg TR", "strings/fx"},
  {"Family 05 / 016 Dark Drone", 5, "Korg TR", "pad"},
  {"Family 05 / 017 Tight Arp", 5, "Korg TR", "lead"},
  {"Family 05 / 018 Room Crush", 5, "Korg TR", "strings/fx"},
  {"Family 05 / 019 Dry Kit", 5, "Korg TR", "drums"},
  {"Family 05 / 020 Hard Kit", 5, "Korg TR", "drums"},
  {"Family 05 / 021 Tape Kit", 5, "Korg TR", "drums"},
// --- BEGIN the MAGS library
  // The MAGS library: 205 patches, grouped by role rather than by
  // emulated family. Measured level trims for these are emitted below once
  // tools/measure_preset_levels.py has run against them.
  //
  {"Glass Field", 0, "mags", "polysynth"},
  {"Thin Bell Keys", 0, "mags", "polysynth"},
  {"Wide Interval Stab", 0, "mags", "polysynth"},
  {"Clav Ghost", 0, "mags", "polysynth"},
  {"Glass Morph Keys", 0, "mags", "polysynth"},
  {"Hollow Tine", 0, "mags", "polysynth"},
  {"Sparse Chime", 0, "mags", "polysynth"},
  {"Tight Saw Stab", 0, "mags", "polysynth"},
  {"Air Tines", 0, "mags", "polysynth"},
  {"Off-Kilter Pad", 0, "mags", "polysynth"},
  {"Disc Chorus", 0, "mags", "polysynth"},
  {"Filtered Sweep", 0, "mags", "polysynth"},
  {"French Touch Stab", 0, "mags", "polysynth"},
  {"Warm Chorus Keys", 0, "mags", "polysynth"},
  {"Driven Disco Bass", 0, "mags", "polysynth"},
  {"Talkbox Lead Pad", 0, "mags", "polysynth"},
  {"Robot Choir Keys", 0, "mags", "polysynth"},
  {"Neon Clav", 0, "mags", "polysynth"},
  {"Metal Pluck", 0, "mags", "polysynth"},
  {"Exotic Bell", 0, "mags", "polysynth"},
  {"Dry Clav Pluck", 0, "mags", "polysynth"},
  {"Hard Stab", 0, "mags", "polysynth"},
  {"Gamelan Keys", 0, "mags", "polysynth"},
  {"Short Marimba", 0, "mags", "polysynth"},
  {"Plucked Glass", 0, "mags", "polysynth"},
  {"Grit Saw Stack", 0, "mags", "polysynth"},
  {"Grit Comp Stab", 0, "mags", "polysynth"},
  {"Overdriven Fifths", 0, "mags", "polysynth"},
  {"Saturated Sub Stack", 0, "mags", "polysynth"},
  {"Hard Edge Lead Pad", 0, "mags", "polysynth"},
  {"Folded Saw Morph", 0, "mags", "polysynth"},
  {"Silk String Bed", 0, "mags", "polysynth"},
  {"Warm Brass Section", 0, "mags", "polysynth"},
  {"Lush Voice Bed", 0, "mags", "polysynth"},
  {"Sweetening Pad", 0, "mags", "polysynth"},
  {"Dense Choir Stack", 0, "mags", "polysynth"},
  {"Rich Low Brass", 0, "mags", "polysynth"},
  {"Glide Strings", 0, "mags", "polysynth"},
  {"Dusty Keys", 0, "mags", "polysynth"},
  {"Drifting Pad", 0, "mags", "polysynth"},
  {"Warm Detune Stab", 0, "mags", "polysynth"},
  {"Tape Tines", 0, "mags", "polysynth"},
  {"Warped Choir Keys", 0, "mags", "polysynth"},
  {"Round Low Keys", 0, "mags", "polysynth"},
  {"Soft Tine Drift", 0, "mags", "polysynth"},
  {"Wide Unison Stack", 0, "mags", "polysynth"},
  {"Resonant Poly Brass", 0, "mags", "polysynth"},
  {"Simple Chorus Pad", 0, "mags", "polysynth"},
  {"Ladder Filter Stack", 0, "mags", "polysynth"},
  {"Table Sweep Pad", 0, "mags", "polysynth"},
  {"Thin Glass Lead", 0, "mags", "lead"},
  {"Hollow Pulse Lead", 0, "mags", "lead"},
  {"Glass Morph Lead", 0, "mags", "lead"},
  {"Air Tine Lead", 0, "mags", "lead"},
  {"Wide Interval Lead", 0, "mags", "lead"},
  {"Nasal Whine", 0, "mags", "lead"},
  {"Chime Lead", 0, "mags", "lead"},
  {"Soft Glass Solo", 0, "mags", "lead"},
  {"Filtered Disco Lead", 0, "mags", "lead"},
  {"Talkbox Lead", 0, "mags", "lead"},
  {"Sweep Sync Lead", 0, "mags", "lead"},
  {"French Touch Lead", 0, "mags", "lead"},
  {"Chorus Solo", 0, "mags", "lead"},
  {"Robot Voice Lead", 0, "mags", "lead"},
  {"Disco Bell Lead", 0, "mags", "lead"},
  {"Filter Ride Lead", 0, "mags", "lead"},
  {"Exotic Bell Lead", 0, "mags", "lead"},
  {"Metal Lead", 0, "mags", "lead"},
  {"Hard Dry Lead", 0, "mags", "lead"},
  {"Sparse Pluck Lead", 0, "mags", "lead"},
  {"Gamelan Lead", 0, "mags", "lead"},
  {"Short Marimba Lead", 0, "mags", "lead"},
  {"Sharp Sync Lead", 0, "mags", "lead"},
  {"Distorted Saw Lead", 0, "mags", "lead"},
  {"Overdrive Lead", 0, "mags", "lead"},
  {"Grit Pulse Lead", 0, "mags", "lead"},
  {"Hard Edge Sync", 0, "mags", "lead"},
  {"Comp Stack Lead", 0, "mags", "lead"},
  {"Metal Edge Lead", 0, "mags", "lead"},
  {"Folded Saw Lead", 0, "mags", "lead"},
  {"Silk Solo Lead", 0, "mags", "lead"},
  {"Warm Brass Lead", 0, "mags", "lead"},
  {"Voice Lead", 0, "mags", "lead"},
  {"Sweetening Solo", 0, "mags", "lead"},
  {"Soft Tine Solo", 0, "mags", "lead"},
  {"Round Brass Lead", 0, "mags", "lead"},
  {"Dusty Lead", 0, "mags", "lead"},
  {"Detune Warm Lead", 0, "mags", "lead"},
  {"Soft Tine Drift Lead", 0, "mags", "lead"},
  {"Tape Solo", 0, "mags", "lead"},
  {"Warped Choir Lead", 0, "mags", "lead"},
  {"Lo-Fi Pulse Lead", 0, "mags", "lead"},
  {"Round Warm Solo", 0, "mags", "lead"},
  {"Wide Unison Lead", 0, "mags", "lead"},
  {"Classic Sync Lead", 0, "mags", "lead"},
  {"Bright FM Lead", 0, "mags", "lead"},
  {"Narrow Pulse Lead", 0, "mags", "lead"},
  {"Table Sweep Lead", 0, "mags", "lead"},
  {"Ladder Drive Lead", 0, "mags", "lead"},
  {"Stack Detune Lead", 0, "mags", "lead"},
  {"Tine Piano", 0, "mags", "keys"},
  {"Bright Tine", 0, "mags", "keys"},
  {"Soft Tine", 0, "mags", "keys"},
  {"Tine Bell", 0, "mags", "keys"},
  {"Tine Bark", 0, "mags", "keys"},
  {"Dusty Tine", 0, "mags", "keys"},
  {"Reed Piano", 0, "mags", "keys"},
  {"Warm Reed", 0, "mags", "keys"},
  {"Reed Bark", 0, "mags", "keys"},
  {"Tape Reed", 0, "mags", "keys"},
  {"Tonewheel Organ", 0, "mags", "keys"},
  {"Full Drawbar", 0, "mags", "keys"},
  {"Percussive Organ", 0, "mags", "keys"},
  {"Gospel Organ", 0, "mags", "keys"},
  {"Dirty Organ", 0, "mags", "keys"},
  {"Combo Organ", 0, "mags", "keys"},
  {"Reedy Combo", 0, "mags", "keys"},
  {"Bright Combo", 0, "mags", "keys"},
  {"Tape Choir", 0, "mags", "keys"},
  {"Tape Strings", 0, "mags", "keys"},
  {"Round Sub Bass", 0, "mags", "bass"},
  {"Bright Bass", 0, "mags", "bass"},
  {"Warm Bottom", 0, "mags", "bass"},
  {"Grit Bass", 0, "mags", "bass"},
  {"Tape Bass", 0, "mags", "bass"},
  {"Steel String", 0, "mags", "guitars"},
  {"Nylon String", 0, "mags", "guitars"},
  {"Bright Steel", 0, "mags", "guitars"},
  {"Warm Acoustic", 0, "mags", "guitars"},
  {"Tape Acoustic", 0, "mags", "guitars"},
  {"Fingerpick", 0, "mags", "guitars"},
  {"Twelve String", 0, "mags", "guitars"},
  {"Muted Acoustic", 0, "mags", "guitars"},
  {"Clean Electric", 0, "mags", "guitars"},
  {"Crunch Electric", 0, "mags", "guitars"},
  {"High Gain", 0, "mags", "guitars"},
  {"Hollow Body", 0, "mags", "guitars"},
  {"Muted Electric", 0, "mags", "guitars"},
  {"Tremolo Electric", 0, "mags", "guitars"},
  {"Tape Electric", 0, "mags", "guitars"},
  {"Hollow Reed", 0, "mags", "reeds/horn"},
  {"Breathy Flute", 0, "mags", "reeds/horn"},
  {"Narrow Oboe", 0, "mags", "reeds/horn"},
  {"Warm Clarinet", 0, "mags", "reeds/horn"},
  {"Airy Pan Flute", 0, "mags", "reeds/horn"},
  {"Bright Shawm", 0, "mags", "reeds/horn"},
  {"Soft Bassoon", 0, "mags", "reeds/horn"},
  {"Dusty Reed", 0, "mags", "reeds/horn"},
  {"Thin Whistle", 0, "mags", "reeds/horn"},
  {"Wide Reed Section", 0, "mags", "reeds/horn"},
  {"Warm Brass", 0, "mags", "reeds/horn"},
  {"Bright Brass Section", 0, "mags", "reeds/horn"},
  {"Low Brass Bed", 0, "mags", "reeds/horn"},
  {"Punch Brass Stab", 0, "mags", "reeds/horn"},
  {"Soft French Horn", 0, "mags", "reeds/horn"},
  {"Hard Brass Stack", 0, "mags", "reeds/horn"},
  {"Tape Brass", 0, "mags", "reeds/horn"},
  {"Muted Brass", 0, "mags", "reeds/horn"},
  {"Wide Brass Swell", 0, "mags", "reeds/horn"},
  {"Sharp Brass Lead", 0, "mags", "reeds/horn"},
  {"Slow Motion Pad", 0, "mags", "pad"},
  {"Drifting Warm Pad", 0, "mags", "pad"},
  {"Wide Chorus Pad", 0, "mags", "pad"},
  {"Voice Bed Pad", 0, "mags", "pad"},
  {"Glass Pad", 0, "mags", "pad"},
  {"Deep Low Pad", 0, "mags", "pad"},
  {"Air Motion Pad", 0, "mags", "pad"},
  {"Dark Tape Pad", 0, "mags", "pad"},
  {"Bright Sweep Pad", 0, "mags", "pad"},
  {"Sparse Hollow Pad", 0, "mags", "pad"},
  {"Frozen Pad", 0, "mags", "pad"},
  {"Hollow Sweep Pad", 0, "mags", "pad"},
  {"Low Bed Pad", 0, "mags", "pad"},
  {"Formant Pad", 0, "mags", "pad"},
  {"Worn Pad", 0, "mags", "pad"},
  {"Glass Drift Pad", 0, "mags", "pad"},
  {"Choir Ahh", 0, "mags", "pad"},
  {"Choir Ooh", 0, "mags", "pad"},
  {"Wide Choir Bed", 0, "mags", "pad"},
  {"Broken Choir", 0, "mags", "pad"},
  {"Silk Strings", 0, "mags", "strings/fx"},
  {"Warm Section Strings", 0, "mags", "strings/fx"},
  {"Bright Strings", 0, "mags", "strings/fx"},
  {"Low Cello Bed", 0, "mags", "strings/fx"},
  {"Slow Bow Strings", 0, "mags", "strings/fx"},
  {"Tight Strings", 0, "mags", "strings/fx"},
  {"Section Swell", 0, "mags", "strings/fx"},
  {"Dusty Strings", 0, "mags", "strings/fx"},
  {"Octave Strings", 0, "mags", "strings/fx"},
  {"Thin High Strings", 0, "mags", "strings/fx"},
  {"Ring Mod Sweep", 0, "mags", "strings/fx"},
  {"Noise Wash", 0, "mags", "strings/fx"},
  {"Metal Riser", 0, "mags", "strings/fx"},
  {"Detuned Siren", 0, "mags", "strings/fx"},
  {"Tape Warble FX", 0, "mags", "strings/fx"},
  {"Long Delay Drone", 0, "mags", "strings/fx"},
  {"Broken Radio", 0, "mags", "strings/fx"},
  {"Sub Drop FX", 0, "mags", "strings/fx"},
  {"Shimmer Texture", 0, "mags", "strings/fx"},
  {"Sweep Down FX", 0, "mags", "strings/fx"},
  {"Synthetic Strings", 0, "mags", "strings/fx"},
  {"Velvet Strings", 0, "mags", "strings/fx"},
  {"High Strings", 0, "mags", "strings/fx"},
  {"Deep Strings", 0, "mags", "strings/fx"},
  {"Tape Strings Bed", 0, "mags", "strings/fx"},
  // --- END the MAGS library
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

namespace keith::sampler {
namespace {

void PutString(IByteChunk& chunk, const std::string& text)
{
  const int32_t len = static_cast<int32_t>(text.size());
  chunk.Put(&len);
  for (char c : text)
  {
    const uint8_t byte = static_cast<uint8_t>(c);
    chunk.Put(&byte);
  }
}

int GetString(const IByteChunk& chunk, int pos, std::string& text)
{
  int32_t len = 0;
  pos = chunk.Get(&len, pos);
  if (pos < 0 || len < 0)
    return -1;
  if (len == 0)
  {
    text.clear();
    return pos;
  }
  // Bound the length against what is actually left BEFORE using it, and without adding, so the
  // check cannot itself overflow.
  //
  // The earlier form was `pos + len > chunk.Size()`, which adds two 32-bit ints. A hostile len of
  // 2^31-1 makes that sum wrap negative, and a negative number is comfortably `<= chunk.Size()`,
  // so the check passed and the following resize() attempted a 2 GB allocation. Same family as the
  // slice/marker count overflow: the guard was doing the arithmetic it meant to guard.
  //
  // Subtracting instead keeps every value in range. `len > remaining` is equivalent to
  // `pos + len > Size` for non-negative len, and cannot wrap.
  const int remaining = chunk.Size() - pos;
  if (remaining < 0 || len > remaining)
    return -1;
  text.resize(static_cast<std::size_t>(len));
  for (int32_t i = 0; i < len; ++i)
  {
    uint8_t byte = 0;
    pos = chunk.Get(&byte, pos);
    if (pos < 0)
      return -1;
    text[static_cast<std::size_t>(i)] = static_cast<char>(byte);
  }
  return pos;
}

int16_t FloatToPcm16(float sample)
{
  const float clamped = std::clamp(sample, -1.f, 1.f);
  return static_cast<int16_t>(std::lround(clamped * 32767.f));
}

bool ChunkHasBytes(const IByteChunk& chunk, int pos, int bytes)
{
  // Compare by subtraction, not `pos + bytes <= Size()`. That form adds two 32-bit ints, and a
  // large enough `bytes` makes the sum wrap negative -- which is <= Size(), so the guard would
  // pass for a byte count that does not exist. No current caller can reach that (the audio payload
  // is ceiling-checked first and the header check is a fixed size), but the helper is the wrong
  // shape for its job and the next caller would not know.
  if (pos < 0 || bytes < 0)
    return false;
  const int remaining = chunk.Size() - pos;
  return remaining >= 0 && bytes <= remaining;
}

} // namespace

void SamplerEngine::SerializeState(IByteChunk& chunk) const
{
  const uint32_t magic = kSamplerStateMagic;
  const uint32_t version = kSamplerStateVersion;
  chunk.Put(&magic);
  chunk.Put(&version);

  const bool hasSample = HasSample();
  // Full decoded length, not START/END playback bounds, so slice markers stay editable after reopen.
  const int frames = hasSample ? mSample->frames : 0;
  const int channelCount = hasSample ? mSample->ChannelCount() : 0;
  const std::size_t pcmBytes =
      hasSample ? static_cast<std::size_t>(frames) * static_cast<std::size_t>(channelCount) * 2u
                : 0u;
  // Sources are 16-bit PCM; storing float32 would double the chunk for no audible gain.
  const bool embed =
      hasSample && pcmBytes > 0u && pcmBytes <= static_cast<std::size_t>(kSampleEmbedCeilingBytes);
  const uint8_t embedded = embed ? 1u : 0u;
  chunk.Put(&embedded);

  const double sampleRate = hasSample ? mSample->sampleRate : 44100.;
  chunk.Put(&sampleRate);

  const uint8_t channels = static_cast<uint8_t>(std::clamp(channelCount, 0, 255));
  chunk.Put(&channels);
  const int32_t frameCount = static_cast<int32_t>(frames);
  chunk.Put(&frameCount);

  const std::string name = hasSample ? mSample->name : std::string {};
  PutString(chunk, name);
  // Always store the source path when we know it. When over the ceiling the path is the only copy
  // of the audio; when embedded it still helps the user re-link after moving the project.
  const std::string path = hasSample ? mLastPath : std::string {};
  PutString(chunk, path);

  if (embed)
  {
    for (int frame = 0; frame < frames; ++frame)
    {
      for (int ch = 0; ch < channelCount; ++ch)
      {
        const float sample = mSample->channels[static_cast<std::size_t>(ch)]
                                 [static_cast<std::size_t>(frame)];
        const int16_t pcm = FloatToPcm16(sample);
        chunk.Put(&pcm);
      }
    }
  }

  const std::shared_ptr<const std::vector<Slice>> slices = mSlices;
  const int32_t sliceCount =
      static_cast<int32_t>(slices ? static_cast<int>(slices->size()) : 0);
  chunk.Put(&sliceCount);
  if (slices)
  {
    for (const Slice& slice : *slices)
    {
      const int32_t startFrame = slice.startFrame;
      const int32_t midiNote = slice.midiNote;
      chunk.Put(&startFrame);
      chunk.Put(&midiNote);
    }
  }

  const uint8_t warpEnabled = mWarpMap.enabled ? 1u : 0u;
  chunk.Put(&warpEnabled);
  const int32_t markerCount = static_cast<int32_t>(mWarpMap.Markers().size());
  chunk.Put(&markerCount);
  for (const WarpMarker& marker : mWarpMap.Markers())
  {
    const int32_t frame = marker.frame;
    chunk.Put(&frame);
    chunk.Put(&marker.beats);
  }
}

int SamplerEngine::UnserializeState(const IByteChunk& chunk, int startPos)
{
  if (startPos < 0 || !ChunkHasBytes(chunk, startPos, static_cast<int>(sizeof(uint32_t) * 2u)))
    return startPos;

  uint32_t magic = 0;
  uint32_t version = 0;
  chunk.Get(&magic, startPos);
  chunk.Get(&version, startPos + static_cast<int>(sizeof(uint32_t)));
  if (magic != kSamplerStateMagic || version != kSamplerStateVersion)
    return startPos;

  int pos = startPos + static_cast<int>(sizeof(uint32_t) * 2u);

  uint8_t embedded = 0;
  pos = chunk.Get(&embedded, pos);
  double sampleRate = 44100.;
  pos = pos >= 0 ? chunk.Get(&sampleRate, pos) : -1;
  uint8_t channelCount = 0;
  pos = pos >= 0 ? chunk.Get(&channelCount, pos) : -1;
  int32_t frames = 0;
  pos = pos >= 0 ? chunk.Get(&frames, pos) : -1;
  if (pos < 0 || frames < 0 || channelCount == 0)
    return startPos;

  std::string name;
  std::string path;
  pos = GetString(chunk, pos, name);
  pos = pos >= 0 ? GetString(chunk, pos, path) : -1;
  if (pos < 0)
    return startPos;

  std::shared_ptr<const SampleData> sample;
  LoadError loadError = LoadError::None;
  bool embeddedInProject = embedded != 0;

  if (embedded != 0)
  {
    const int64_t sampleCount =
        static_cast<int64_t>(frames) * static_cast<int64_t>(channelCount);
    if (sampleCount < 0 || sampleCount > std::numeric_limits<int32_t>::max())
      return startPos;
    const std::size_t payloadBytes = static_cast<std::size_t>(sampleCount) * sizeof(int16_t);
    if (!ChunkHasBytes(chunk, pos, static_cast<int>(payloadBytes)))
      return startPos;

    if (payloadBytes > static_cast<std::size_t>(kSampleEmbedCeilingBytes))
    {
      // Reject oversized embedded audio (corrupt or hostile chunk) but keep parsing so slices
      // and warp still restore.
      loadError = LoadError::TooLarge;
      embeddedInProject = false;
      for (std::size_t i = 0; i < sampleCount && pos >= 0; ++i)
      {
        int16_t discard = 0;
        pos = chunk.Get(&discard, pos);
      }
    }
    else
    {
      auto restored = std::make_shared<SampleData>();
      restored->sampleRate = sampleRate;
      restored->frames = frames;
      restored->name = name;
      restored->channels.resize(static_cast<std::size_t>(channelCount));
      for (auto& ch : restored->channels)
        ch.resize(static_cast<std::size_t>(frames));

      for (int frame = 0; frame < frames && pos >= 0; ++frame)
      {
        for (int ch = 0; ch < channelCount && pos >= 0; ++ch)
        {
          int16_t pcm = 0;
          pos = chunk.Get(&pcm, pos);
          restored->channels[static_cast<std::size_t>(ch)][static_cast<std::size_t>(frame)] =
              static_cast<float>(pcm) / 32767.0f;
        }
      }
      if (pos < 0)
        return startPos;
      sample = std::move(restored);
    }
  }
  else if (!path.empty())
  {
    embeddedInProject = false;
    loadError = LoadError::FileNotFound;
    sample = LoadFile(path, &loadError);
    if (sample)
      loadError = LoadError::None;
  }

  int32_t sliceCount = 0;
  pos = chunk.Get(&sliceCount, pos);
  if (pos < 0 || sliceCount < 0)
    return startPos;
  // Bound the COUNT against the bytes actually remaining BEFORE multiplying.
  //
  // The previous form was `ChunkHasBytes(chunk, pos, sliceCount * 8)`, which multiplies in 32-bit
  // int. A corrupt chunk declaring sliceCount = 2^28 wraps that product to 0, so the size check
  // passes for any position and slices.resize(2^28) then tries to allocate 2 GB and throws. The
  // embedded-audio path above already computes in int64_t and cannot overflow; these two counts
  // did not, which was the inconsistency. Dividing here keeps the arithmetic inside int.
  const int remaining = chunk.Size() - pos;
  if (remaining < 0 || sliceCount > remaining / static_cast<int>(sizeof(int32_t) * 2u))
    return startPos;

  std::vector<Slice> slices;
  if (sliceCount > 0)
    slices.resize(static_cast<std::size_t>(sliceCount));
  for (int i = 0; i < sliceCount && pos >= 0; ++i)
  {
    int32_t startFrame = 0;
    int32_t midiNote = 0;
    pos = chunk.Get(&startFrame, pos);
    pos = pos >= 0 ? chunk.Get(&midiNote, pos) : -1;
    if (pos < 0)
      return startPos;
    slices[static_cast<std::size_t>(i)] = Slice {startFrame, midiNote};
  }

  uint8_t warpEnabled = 0;
  pos = chunk.Get(&warpEnabled, pos);
  if (pos < 0)
    return startPos;

  int32_t markerCount = 0;
  pos = chunk.Get(&markerCount, pos);
  if (pos < 0 || markerCount < 0)
    return startPos;
  // Same overflow as the slice count above: markerCount * 16 in 32-bit wraps, and a wrapped-to-0
  // product passes any bounds check before markers.resize allocates gigabytes and throws.
  {
    const int remaining = chunk.Size() - pos;
    if (remaining < 0
        || markerCount > remaining / static_cast<int>(sizeof(int32_t) + sizeof(double)))
      return startPos;
  }

  std::vector<WarpMarker> markers;
  if (markerCount > 0)
    markers.resize(static_cast<std::size_t>(markerCount));
  for (int i = 0; i < markerCount && pos >= 0; ++i)
  {
    int32_t frame = 0;
    double beats = 0.;
    pos = chunk.Get(&frame, pos);
    pos = pos >= 0 ? chunk.Get(&beats, pos) : -1;
    if (pos < 0)
      return startPos;
    markers[static_cast<std::size_t>(i)] = WarpMarker {frame, beats, false};
  }

  mSample = sample;
  mLastPath = !path.empty() ? path : name;
  mLastError = loadError;
  mSampleEmbeddedInProject = embeddedInProject;
  PublishSampleToVoices();
  PublishSlices(std::move(slices));
  mWarpMap.Clear();
  mWarpMap.enabled = warpEnabled != 0;
  for (const WarpMarker& marker : markers)
    mWarpMap.SetMarker(marker.frame, marker.beats, marker.userPlaced);

  return pos;
}

} // namespace keith::sampler

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
  mSampler.SerializeState(chunk);
  return SerializeParams(chunk);
}

int Keith::UnserializeState(const IByteChunk& chunk, int startPos)
{
  keith::drums::KitParams restored;
  const int paramsPos = UnserializeDrumKit(chunk, startPos, restored);
  if (paramsPos >= 0)
  {
    mDrumKit = restored;
    const int afterSampler = mSampler.UnserializeState(chunk, paramsPos);
    return UnserializeParams(chunk, afterSampler);
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
static_assert(kParamOsc3Shape == 111 && kParamOsc3Source == 117 && kParamSampleMode == 118
                && kParamSampleReverse == 119 && kNumParams == 120,
              "the third oscillator appends 111..117, the sampler controls 118..119");

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

  // What the keyboard does to a loaded sample. Enumerated so the two states are mutually
  // exclusive: PLAY transposes the whole sample, SLICE plays one slice per key. Stepped
  // parameter, so the editor can draw a button per value rather than a continuous control.
  GetParam(kParamSampleMode)->InitEnum("Sample Mode", 0, {"Play", "Slice"});
  // Reverse is a separate switch rather than a third mode, because it composes with both: a whole
  // sample can play backwards, and so can each slice. A mode would have forced a choice between
  // them.
  GetParam(kParamSampleReverse)->InitEnum("Sample Reverse", 0, {"Fwd", "Rev"});
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

  // Which of the two sampler behaviours the keyboard drives. Read once per block and pushed to
  // the engine, rather than the engine polling a parameter from the audio thread.
  mSampler.SetMode(GetParam(kParamSampleMode)->Int() == 1
                     ? keith::sampler::SampleMode::Slice
                     : keith::sampler::SampleMode::Play);

  // Reverse, read the same way. Applied per trigger, so switching it does not disturb a note
  // that is already sounding.
  mSampler.SetReversed(GetParam(kParamSampleReverse)->Int() == 1);

  // The host transport, read once per block. The warp map needs the tempo, and tempo sync needs
  // the musical position.
  //
  // These are IPlugProcessor's accessors onto its mTimeInfo member, which the host keeps filled.
  // Used rather than reading mTimeInfo directly -- they are the supported surface, and reaching
  // into the member would break if the field moved.
  {
    keith::sampler::Transport transport;
    transport.tempo = GetTempo();
    transport.ppqPosition = GetPPQPos();
    transport.running = GetTransportIsRunning();

    int numerator = 4, denominator = 4;
    GetTimeSig(numerator, denominator);
    // Guarded: a host that reports a zero denominator would divide by zero here.
    transport.beatsPerBar = (denominator > 0)
                              ? static_cast<int>(4.0 * numerator / denominator)
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
