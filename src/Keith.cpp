// Main Keith iPlug2 plugin implementation: factory presets, parameter setup, state I/O, and host callbacks.
#include "Keith.h"
#include "KeithGuard.h"
#include "IPlug_include_in_plug_src.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>   // std::getenv, for the KEITH_SAMPLE_DIR override in SampleDir()
#include <exception>  // std::exception, caught by the sample loaders
#include <initializer_list>
#include <limits>

#if IPLUG_EDITOR
#include "IControls.h"
#include "editor/EditorialLayout.h"
#endif

// BundleResourcePath, which resolves a plugin's resource directory across the three formats.
// It lives in IPlugPaths.h, and neither the plugin base header nor IPlug_include_in_plug_src.h
// pulls it in -- hence the undeclared-identifier error when it was first called.
//
// OUTSIDE the IPLUG_EDITOR guard on purpose: SampleDir() is used by the DSP side to find the
// sample kits, and the offline renderer builds with IPLUG_EDITOR=0. Inside the guard it compiled
// for the plugin and failed for the renderer.
#include "IPlugPaths.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace
{
constexpr uint32_t kDrumStateMagic = 0x4B445231u; // "KDR1"
constexpr uint32_t kDrumStateVersion = 1u;

constexpr uint32_t kSamplerStateMagic = 0x4B535031u; // "KSP1"
constexpr uint32_t kSamplerStateVersion = 2u;
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
  // mono synths
  {109, kParamEngine, 0},   {109, kParamPolyMode, 0},   {109, kParamPorta, 8},
  {109, kParamOsc1Shape, 44},   {109, kParamOsc1Level, 86},   {109, kParamOsc2Shape, 42},
  {109, kParamOsc2Pitch, 7},   {109, kParamOsc2Level, 74},   {109, kParamSubLevel, 24},
  {109, kParamNoiseLevel, 0},   {109, kParamFilterType, 0},   {109, kParamFilterCutoff, 9600},
  {109, kParamFilterReso, 26},   {109, kParamFilterDrive, 10},   {109, kParamFilterEnv, 54},
  {109, kParamFiltAttack, 2},   {109, kParamFiltDecay, 700},   {109, kParamFiltSustain, 48},
  {109, kParamFiltRelease, 800},   {109, kParamAmpAttack, 3},   {109, kParamAmpDecay, 700},
  {109, kParamAmpSustain, 48},   {109, kParamAmpRelease, 800},   {109, kParamPitchAmount, -14},
  {109, kParamPitchAttack, 1},   {109, kParamPitchDecay, 90},   {109, kParamPitchSustain, 0},
  {109, kParamPitchRelease, 200},   {109, kParamUnisonVoices, 2},   {109, kParamUnisonDetune, 16},
  {109, kParamVelToAmp, 40},   {109, kParamVelToFilter, 34},   {109, kParamStereoSpread, 52},
  {109, kParamStereoDelay, 20},   {109, kParamTapeSat, 26},   {109, kParamTapeMix, 30},
  {109, kParamIns1Algo, 3},   {109, kParamIns1A, 34},   {109, kParamIns1B, 30},
  {109, kParamIns1C, 54},   {109, kParamIns1Mix, 18},   {109, kParamMstAlgo, 2},
  {109, kParamMstA, 24},   {109, kParamMstB, 18},   {109, kParamMstC, 30},
  {109, kParamMstMix, 10},
  {110, kParamEngine, 0},   {110, kParamPolyMode, 0},   {110, kParamPorta, 0},
  {110, kParamOsc1Shape, 52},   {110, kParamOsc1PW, 34},   {110, kParamOsc1Level, 88},
  {110, kParamOsc2Shape, 50},   {110, kParamOsc2Pitch, -12},   {110, kParamOsc2Level, 56},
  {110, kParamSubLevel, 30},   {110, kParamFilterType, 0},   {110, kParamFilterCutoff, 8400},
  {110, kParamFilterReso, 32},   {110, kParamFilterDrive, 0},   {110, kParamFilterEnv, 52},
  {110, kParamFiltAttack, 2},   {110, kParamFiltDecay, 600},   {110, kParamFiltSustain, 46},
  {110, kParamFiltRelease, 700},   {110, kParamAmpAttack, 3},   {110, kParamAmpDecay, 800},
  {110, kParamAmpSustain, 62},   {110, kParamAmpRelease, 700},   {110, kParamPitchAmount, -10},
  {110, kParamPitchDecay, 70},   {110, kParamUnisonVoices, 2},   {110, kParamUnisonDetune, 14},
  {110, kParamVelToAmp, 42},   {110, kParamVelToFilter, 32},   {110, kParamStereoSpread, 32},
  {110, kParamStereoDelay, 24},   {110, kParamTapeSat, 28},   {110, kParamTapeMix, 32},
  {110, kParamIns1Algo, 3},   {110, kParamIns1A, 44},   {110, kParamIns1B, 36},
  {110, kParamIns1C, 60},   {110, kParamIns1Mix, 24},   {110, kParamMstAlgo, 2},
  {110, kParamMstA, 24},   {110, kParamMstB, 18},   {110, kParamMstC, 30},
  {110, kParamMstMix, 10},
  {111, kParamEngine, 0},   {111, kParamPolyMode, 0},   {111, kParamPorta, 0},
  {111, kParamOsc1Shape, 30},   {111, kParamOsc1Level, 54},   {111, kParamOsc2Level, 0},
  {111, kParamOsc3Enable, 1},   {111, kParamOsc3Source, 1},   {111, kParamOsc3Morph, 2.6},
  {111, kParamOsc3Level, 88},   {111, kParamOsc3Pitch, 12},   {111, kParamSubLevel, 20},
  {111, kParamFilterType, 0},   {111, kParamFilterCutoff, 10200},   {111, kParamFilterReso, 22},
  {111, kParamFilterEnv, 44},   {111, kParamFiltAttack, 2},   {111, kParamFiltDecay, 900},
  {111, kParamFiltSustain, 48},   {111, kParamFiltRelease, 1200},   {111, kParamAmpAttack, 3},
  {111, kParamAmpDecay, 1000},   {111, kParamAmpSustain, 64},   {111, kParamAmpRelease, 1200},
  {111, kParamUnisonVoices, 2},   {111, kParamUnisonDetune, 13},   {111, kParamVelToAmp, 46},
  {111, kParamVelToFilter, 30},   {111, kParamStereoSpread, 44},   {111, kParamStereoDelay, 30},
  {111, kParamTapeSat, 24},   {111, kParamTapeMix, 32},   {111, kParamIns1Algo, 4},
  {111, kParamIns1A, 26},   {111, kParamIns1B, 36},   {111, kParamIns1C, 16},
  {111, kParamIns1Mix, 16},   {111, kParamMstAlgo, 2},   {111, kParamMstA, 24},
  {111, kParamMstB, 18},   {111, kParamMstC, 30},   {111, kParamMstMix, 10},
  {112, kParamEngine, 0},   {112, kParamPolyMode, 0},   {112, kParamPorta, 0},
  {112, kParamOsc1Shape, 22},   {112, kParamOsc1Level, 78},   {112, kParamOsc2Shape, 24},
  {112, kParamOsc2Pitch, 12},   {112, kParamOsc2Level, 62},   {112, kParamFmAmount, 36},
  {112, kParamSubLevel, 16},   {112, kParamFilterType, 0},   {112, kParamFilterCutoff, 8800},
  {112, kParamFilterReso, 24},   {112, kParamFilterEnv, 46},   {112, kParamFiltAttack, 2},
  {112, kParamFiltDecay, 900},   {112, kParamFiltSustain, 26.4},   {112, kParamFiltRelease, 1100},
  {112, kParamAmpAttack, 2},   {112, kParamAmpDecay, 900},   {112, kParamAmpSustain, 44},
  {112, kParamAmpRelease, 1100},   {112, kParamVelToAmp, 52},   {112, kParamVelToFilter, 28},
  {112, kParamStereoSpread, 54},   {112, kParamStereoDelay, 26},   {112, kParamTapeSat, 22},
  {112, kParamTapeMix, 30},   {112, kParamIns1Algo, 3},   {112, kParamIns1A, 34},
  {112, kParamIns1B, 30},   {112, kParamIns1C, 54},   {112, kParamIns1Mix, 18},
  {112, kParamMstAlgo, 2},   {112, kParamMstA, 24},   {112, kParamMstB, 18},
  {112, kParamMstC, 30},   {112, kParamMstMix, 10},
  {113, kParamEngine, 0},   {113, kParamPolyMode, 0},   {113, kParamPorta, 20},
  {113, kParamOsc1Shape, 44},   {113, kParamOsc1Level, 86},   {113, kParamOsc2Shape, 42},
  {113, kParamOsc2Pitch, 7},   {113, kParamOsc2Level, 74},   {113, kParamSubLevel, 24},
  {113, kParamNoiseLevel, 0},   {113, kParamFilterType, 0},   {113, kParamFilterCutoff, 7600},
  {113, kParamFilterReso, 30},   {113, kParamFilterDrive, 0},   {113, kParamFilterEnv, 54},
  {113, kParamFiltAttack, 2},   {113, kParamFiltDecay, 600},   {113, kParamFiltSustain, 52},
  {113, kParamFiltRelease, 900},   {113, kParamAmpAttack, 4},   {113, kParamAmpDecay, 600},
  {113, kParamAmpSustain, 52},   {113, kParamAmpRelease, 900},   {113, kParamPitchAmount, -14},
  {113, kParamPitchAttack, 1},   {113, kParamPitchDecay, 90},   {113, kParamPitchSustain, 0},
  {113, kParamPitchRelease, 200},   {113, kParamUnisonVoices, 2},   {113, kParamUnisonDetune, 16},
  {113, kParamVelToAmp, 40},   {113, kParamVelToFilter, 34},   {113, kParamStereoSpread, 52},
  {113, kParamStereoDelay, 20},   {113, kParamTapeSat, 26},   {113, kParamTapeMix, 30},
  {113, kParamIns1Algo, 3},   {113, kParamIns1A, 44},   {113, kParamIns1B, 36},
  {113, kParamIns1C, 60},   {113, kParamIns1Mix, 24},   {113, kParamMstAlgo, 2},
  {113, kParamMstA, 24},   {113, kParamMstB, 18},   {113, kParamMstC, 30},
  {113, kParamMstMix, 10},
  {114, kParamEngine, 0},   {114, kParamPolyMode, 0},   {114, kParamPorta, 0},
  {114, kParamOsc1Shape, 52},   {114, kParamOsc1PW, 28},   {114, kParamOsc1Level, 88},
  {114, kParamOsc2Shape, 50},   {114, kParamOsc2Pitch, -12},   {114, kParamOsc2Level, 56},
  {114, kParamSubLevel, 30},   {114, kParamFilterType, 0},   {114, kParamFilterCutoff, 10400},
  {114, kParamFilterReso, 38},   {114, kParamFilterDrive, 0},   {114, kParamFilterEnv, 52},
  {114, kParamFiltAttack, 2},   {114, kParamFiltDecay, 600},   {114, kParamFiltSustain, 46},
  {114, kParamFiltRelease, 600},   {114, kParamAmpAttack, 2},   {114, kParamAmpDecay, 800},
  {114, kParamAmpSustain, 62},   {114, kParamAmpRelease, 600},   {114, kParamPitchAmount, -18},
  {114, kParamPitchDecay, 70},   {114, kParamUnisonVoices, 2},   {114, kParamUnisonDetune, 14},
  {114, kParamVelToAmp, 42},   {114, kParamVelToFilter, 32},   {114, kParamStereoSpread, 32},
  {114, kParamStereoDelay, 24},   {114, kParamTapeSat, 28},   {114, kParamTapeMix, 32},
  {114, kParamIns1Algo, 4},   {114, kParamIns1A, 26},   {114, kParamIns1B, 36},
  {114, kParamIns1C, 16},   {114, kParamIns1Mix, 16},   {114, kParamMstAlgo, 2},
  {114, kParamMstA, 24},   {114, kParamMstB, 18},   {114, kParamMstC, 30},
  {114, kParamMstMix, 10},
  {115, kParamEngine, 0},   {115, kParamPolyMode, 0},   {115, kParamPorta, 0},
  {115, kParamOsc1Shape, 30},   {115, kParamOsc1Level, 54},   {115, kParamOsc2Level, 0},
  {115, kParamOsc3Enable, 1},   {115, kParamOsc3Source, 1},   {115, kParamOsc3Morph, 4.4},
  {115, kParamOsc3Level, 88},   {115, kParamOsc3Pitch, 0},   {115, kParamSubLevel, 20},
  {115, kParamFilterType, 0},   {115, kParamFilterCutoff, 11400},   {115, kParamFilterReso, 20},
  {115, kParamFilterEnv, 44},   {115, kParamFiltAttack, 2},   {115, kParamFiltDecay, 900},
  {115, kParamFiltSustain, 48},   {115, kParamFiltRelease, 1400},   {115, kParamAmpAttack, 2},
  {115, kParamAmpDecay, 1000},   {115, kParamAmpSustain, 64},   {115, kParamAmpRelease, 1400},
  {115, kParamUnisonVoices, 2},   {115, kParamUnisonDetune, 13},   {115, kParamVelToAmp, 46},
  {115, kParamVelToFilter, 30},   {115, kParamStereoSpread, 44},   {115, kParamStereoDelay, 36},
  {115, kParamTapeSat, 24},   {115, kParamTapeMix, 32},   {115, kParamIns1Algo, 3},
  {115, kParamIns1A, 34},   {115, kParamIns1B, 30},   {115, kParamIns1C, 54},
  {115, kParamIns1Mix, 18},   {115, kParamMstAlgo, 2},   {115, kParamMstA, 24},
  {115, kParamMstB, 18},   {115, kParamMstC, 30},   {115, kParamMstMix, 10},
  {116, kParamEngine, 0},   {116, kParamPolyMode, 0},   {116, kParamPorta, 14},
  {116, kParamOsc1Shape, 44},   {116, kParamOsc1Level, 86},   {116, kParamOsc2Shape, 42},
  {116, kParamOsc2Pitch, 0},   {116, kParamOsc2Level, 74},   {116, kParamSubLevel, 24},
  {116, kParamNoiseLevel, 0},   {116, kParamFilterType, 0},   {116, kParamFilterCutoff, 7000},
  {116, kParamFilterReso, 22},   {116, kParamFilterDrive, 0},   {116, kParamFilterEnv, 54},
  {116, kParamFiltAttack, 2},   {116, kParamFiltDecay, 900},   {116, kParamFiltSustain, 54},
  {116, kParamFiltRelease, 1200},   {116, kParamAmpAttack, 8},   {116, kParamAmpDecay, 900},
  {116, kParamAmpSustain, 54},   {116, kParamAmpRelease, 1200},   {116, kParamPitchAmount, -14},
  {116, kParamPitchAttack, 1},   {116, kParamPitchDecay, 90},   {116, kParamPitchSustain, 0},
  {116, kParamPitchRelease, 200},   {116, kParamUnisonVoices, 2},   {116, kParamUnisonDetune, 16},
  {116, kParamVelToAmp, 40},   {116, kParamVelToFilter, 34},   {116, kParamStereoSpread, 52},
  {116, kParamStereoDelay, 20},   {116, kParamTapeSat, 26},   {116, kParamTapeMix, 30},
  {116, kParamIns1Algo, 3},   {116, kParamIns1A, 44},   {116, kParamIns1B, 36},
  {116, kParamIns1C, 60},   {116, kParamIns1Mix, 24},   {116, kParamMstAlgo, 2},
  {116, kParamMstA, 24},   {116, kParamMstB, 18},   {116, kParamMstC, 30},
  {116, kParamMstMix, 10},
  {117, kParamEngine, 0},   {117, kParamPolyMode, 0},   {117, kParamPorta, 12},
  {117, kParamOsc1Shape, 44},   {117, kParamOsc1Level, 86},   {117, kParamOsc2Shape, 42},
  {117, kParamOsc2Pitch, 0},   {117, kParamOsc2Level, 74},   {117, kParamSubLevel, 24},
  {117, kParamNoiseLevel, 0},   {117, kParamFilterType, 0},   {117, kParamFilterCutoff, 5200},
  {117, kParamFilterReso, 44},   {117, kParamFilterDrive, 26},   {117, kParamFilterEnv, 54},
  {117, kParamFiltAttack, 2},   {117, kParamFiltDecay, 500},   {117, kParamFiltSustain, 40},
  {117, kParamFiltRelease, 700},   {117, kParamAmpAttack, 3},   {117, kParamAmpDecay, 500},
  {117, kParamAmpSustain, 40},   {117, kParamAmpRelease, 700},   {117, kParamPitchAmount, -14},
  {117, kParamPitchAttack, 1},   {117, kParamPitchDecay, 90},   {117, kParamPitchSustain, 0},
  {117, kParamPitchRelease, 200},   {117, kParamUnisonVoices, 2},   {117, kParamUnisonDetune, 16},
  {117, kParamVelToAmp, 40},   {117, kParamVelToFilter, 34},   {117, kParamStereoSpread, 52},
  {117, kParamStereoDelay, 20},   {117, kParamTapeSat, 26},   {117, kParamTapeMix, 30},
  {117, kParamIns1Algo, 4},   {117, kParamIns1A, 26},   {117, kParamIns1B, 36},
  {117, kParamIns1C, 16},   {117, kParamIns1Mix, 16},   {117, kParamMstAlgo, 2},
  {117, kParamMstA, 24},   {117, kParamMstB, 18},   {117, kParamMstC, 30},
  {117, kParamMstMix, 10},
  {118, kParamEngine, 0},   {118, kParamPolyMode, 0},   {118, kParamPorta, 0},
  {118, kParamOsc1Shape, 44},   {118, kParamOsc1Level, 86},   {118, kParamOsc2Shape, 42},
  {118, kParamOsc2Pitch, 0},   {118, kParamOsc2Level, 74},   {118, kParamSubLevel, 24},
  {118, kParamNoiseLevel, 0},   {118, kParamFilterType, 0},   {118, kParamFilterCutoff, 6200},
  {118, kParamFilterReso, 40},   {118, kParamFilterDrive, 30},   {118, kParamFilterEnv, 54},
  {118, kParamFiltAttack, 2},   {118, kParamFiltDecay, 600},   {118, kParamFiltSustain, 46},
  {118, kParamFiltRelease, 800},   {118, kParamAmpAttack, 4},   {118, kParamAmpDecay, 600},
  {118, kParamAmpSustain, 46},   {118, kParamAmpRelease, 800},   {118, kParamPitchAmount, -14},
  {118, kParamPitchAttack, 1},   {118, kParamPitchDecay, 90},   {118, kParamPitchSustain, 0},
  {118, kParamPitchRelease, 200},   {118, kParamUnisonVoices, 2},   {118, kParamUnisonDetune, 16},
  {118, kParamVelToAmp, 40},   {118, kParamVelToFilter, 34},   {118, kParamStereoSpread, 46},
  {118, kParamStereoDelay, 20},   {118, kParamTapeSat, 26},   {118, kParamTapeMix, 30},
  {118, kParamIns1Algo, 3},   {118, kParamIns1A, 34},   {118, kParamIns1B, 30},
  {118, kParamIns1C, 54},   {118, kParamIns1Mix, 18},   {118, kParamMstAlgo, 2},
  {118, kParamMstA, 24},   {118, kParamMstB, 18},   {118, kParamMstC, 30},
  {118, kParamMstMix, 10},
  {119, kParamEngine, 0},   {119, kParamPolyMode, 0},   {119, kParamPorta, 0},
  {119, kParamOsc1Shape, 46},   {119, kParamOsc1Level, 88},   {119, kParamOsc2Shape, 48},
  {119, kParamOsc2Pitch, 19},   {119, kParamOsc2Level, 80},   {119, kParamOscSync, 1},
  {119, kParamSubLevel, 18},   {119, kParamFilterType, 0},   {119, kParamFilterCutoff, 6800},
  {119, kParamFilterReso, 42},   {119, kParamFilterDrive, 18},   {119, kParamFilterEnv, 58},
  {119, kParamFiltAttack, 2},   {119, kParamFiltDecay, 700},   {119, kParamFiltSustain, 42},
  {119, kParamFiltRelease, 700},   {119, kParamAmpAttack, 2},   {119, kParamAmpDecay, 900},
  {119, kParamAmpSustain, 58},   {119, kParamAmpRelease, 700},   {119, kParamUnisonVoices, 2},
  {119, kParamUnisonDetune, 12},   {119, kParamVelToAmp, 44},   {119, kParamVelToFilter, 30},
  {119, kParamStereoSpread, 48},   {119, kParamStereoDelay, 24},   {119, kParamTapeSat, 32},
  {119, kParamTapeMix, 34},   {119, kParamIns1Algo, 3},   {119, kParamIns1A, 44},
  {119, kParamIns1B, 36},   {119, kParamIns1C, 60},   {119, kParamIns1Mix, 24},
  {119, kParamMstAlgo, 2},   {119, kParamMstA, 24},   {119, kParamMstB, 18},
  {119, kParamMstC, 30},   {119, kParamMstMix, 10},
  {120, kParamEngine, 0},   {120, kParamPolyMode, 0},   {120, kParamPorta, 0},
  {120, kParamOsc1Shape, 52},   {120, kParamOsc1PW, 40},   {120, kParamOsc1Level, 88},
  {120, kParamOsc2Shape, 50},   {120, kParamOsc2Pitch, -12},   {120, kParamOsc2Level, 56},
  {120, kParamSubLevel, 30},   {120, kParamFilterType, 0},   {120, kParamFilterCutoff, 5800},
  {120, kParamFilterReso, 36},   {120, kParamFilterDrive, 24},   {120, kParamFilterEnv, 52},
  {120, kParamFiltAttack, 2},   {120, kParamFiltDecay, 600},   {120, kParamFiltSustain, 46},
  {120, kParamFiltRelease, 650},   {120, kParamAmpAttack, 2},   {120, kParamAmpDecay, 800},
  {120, kParamAmpSustain, 62},   {120, kParamAmpRelease, 650},   {120, kParamPitchAmount, -10},
  {120, kParamPitchDecay, 70},   {120, kParamUnisonVoices, 2},   {120, kParamUnisonDetune, 14},
  {120, kParamVelToAmp, 42},   {120, kParamVelToFilter, 32},   {120, kParamStereoSpread, 32},
  {120, kParamStereoDelay, 24},   {120, kParamTapeSat, 28},   {120, kParamTapeMix, 32},
  {120, kParamIns1Algo, 4},   {120, kParamIns1A, 26},   {120, kParamIns1B, 36},
  {120, kParamIns1C, 16},   {120, kParamIns1Mix, 16},   {120, kParamMstAlgo, 2},
  {120, kParamMstA, 24},   {120, kParamMstB, 18},   {120, kParamMstC, 30},
  {120, kParamMstMix, 10},
  {121, kParamEngine, 0},   {121, kParamPolyMode, 0},   {121, kParamPorta, 0},
  {121, kParamOsc1Shape, 44},   {121, kParamOsc1Level, 86},   {121, kParamOsc2Shape, 42},
  {121, kParamOsc2Pitch, 0},   {121, kParamOsc2Level, 74},   {121, kParamSubLevel, 24},
  {121, kParamNoiseLevel, 0},   {121, kParamFilterType, 0},   {121, kParamFilterCutoff, 7400},
  {121, kParamFilterReso, 30},   {121, kParamFilterDrive, 18},   {121, kParamFilterEnv, 54},
  {121, kParamFiltAttack, 2},   {121, kParamFiltDecay, 700},   {121, kParamFiltSustain, 50},
  {121, kParamFiltRelease, 1000},   {121, kParamAmpAttack, 5},   {121, kParamAmpDecay, 700},
  {121, kParamAmpSustain, 50},   {121, kParamAmpRelease, 1000},   {121, kParamPitchAmount, -14},
  {121, kParamPitchAttack, 1},   {121, kParamPitchDecay, 90},   {121, kParamPitchSustain, 0},
  {121, kParamPitchRelease, 200},   {121, kParamUnisonVoices, 2},   {121, kParamUnisonDetune, 16},
  {121, kParamVelToAmp, 40},   {121, kParamVelToFilter, 34},   {121, kParamStereoSpread, 52},
  {121, kParamStereoDelay, 20},   {121, kParamTapeSat, 26},   {121, kParamTapeMix, 30},
  {121, kParamIns1Algo, 3},   {121, kParamIns1A, 34},   {121, kParamIns1B, 30},
  {121, kParamIns1C, 54},   {121, kParamIns1Mix, 18},   {121, kParamMstAlgo, 2},
  {121, kParamMstA, 24},   {121, kParamMstB, 18},   {121, kParamMstC, 30},
  {121, kParamMstMix, 10},
  {122, kParamEngine, 0},   {122, kParamPolyMode, 0},   {122, kParamPorta, 0},
  {122, kParamOsc1Shape, 30},   {122, kParamOsc1Level, 54},   {122, kParamOsc2Level, 0},
  {122, kParamOsc3Enable, 1},   {122, kParamOsc3Source, 1},   {122, kParamOsc3Morph, 3.6},
  {122, kParamOsc3Level, 88},   {122, kParamOsc3Pitch, 12},   {122, kParamSubLevel, 20},
  {122, kParamFilterType, 0},   {122, kParamFilterCutoff, 8000},   {122, kParamFilterReso, 28},
  {122, kParamFilterEnv, 44},   {122, kParamFiltAttack, 2},   {122, kParamFiltDecay, 900},
  {122, kParamFiltSustain, 48},   {122, kParamFiltRelease, 1100},   {122, kParamAmpAttack, 6},
  {122, kParamAmpDecay, 1000},   {122, kParamAmpSustain, 64},   {122, kParamAmpRelease, 1100},
  {122, kParamUnisonVoices, 2},   {122, kParamUnisonDetune, 13},   {122, kParamVelToAmp, 46},
  {122, kParamVelToFilter, 30},   {122, kParamStereoSpread, 44},   {122, kParamStereoDelay, 30},
  {122, kParamTapeSat, 24},   {122, kParamTapeMix, 32},   {122, kParamIns1Algo, 3},
  {122, kParamIns1A, 44},   {122, kParamIns1B, 36},   {122, kParamIns1C, 60},
  {122, kParamIns1Mix, 24},   {122, kParamMstAlgo, 2},   {122, kParamMstA, 24},
  {122, kParamMstB, 18},   {122, kParamMstC, 30},   {122, kParamMstMix, 10},
  {123, kParamEngine, 0},   {123, kParamPolyMode, 0},   {123, kParamPorta, 0},
  {123, kParamOsc1Shape, 22},   {123, kParamOsc1Level, 78},   {123, kParamOsc2Shape, 24},
  {123, kParamOsc2Pitch, 12},   {123, kParamOsc2Level, 62},   {123, kParamFmAmount, 48},
  {123, kParamSubLevel, 16},   {123, kParamFilterType, 0},   {123, kParamFilterCutoff, 9200},
  {123, kParamFilterReso, 22},   {123, kParamFilterEnv, 46},   {123, kParamFiltAttack, 2},
  {123, kParamFiltDecay, 800},   {123, kParamFiltSustain, 22.8},   {123, kParamFiltRelease, 1100},
  {123, kParamAmpAttack, 2},   {123, kParamAmpDecay, 800},   {123, kParamAmpSustain, 38},
  {123, kParamAmpRelease, 1100},   {123, kParamVelToAmp, 52},   {123, kParamVelToFilter, 28},
  {123, kParamStereoSpread, 54},   {123, kParamStereoDelay, 26},   {123, kParamTapeSat, 22},
  {123, kParamTapeMix, 30},   {123, kParamIns1Algo, 4},   {123, kParamIns1A, 26},
  {123, kParamIns1B, 36},   {123, kParamIns1C, 16},   {123, kParamIns1Mix, 16},
  {123, kParamMstAlgo, 2},   {123, kParamMstA, 24},   {123, kParamMstB, 18},
  {123, kParamMstC, 30},   {123, kParamMstMix, 10},
  {124, kParamEngine, 0},   {124, kParamPolyMode, 0},   {124, kParamPorta, 0},
  {124, kParamOsc1Shape, 44},   {124, kParamOsc1Level, 86},   {124, kParamOsc2Shape, 42},
  {124, kParamOsc2Pitch, 0},   {124, kParamOsc2Level, 74},   {124, kParamSubLevel, 24},
  {124, kParamNoiseLevel, 0},   {124, kParamFilterType, 0},   {124, kParamFilterCutoff, 4200},
  {124, kParamFilterReso, 48},   {124, kParamFilterDrive, 0},   {124, kParamFilterEnv, 54},
  {124, kParamFiltAttack, 2},   {124, kParamFiltDecay, 450},   {124, kParamFiltSustain, 44},
  {124, kParamFiltRelease, 700},   {124, kParamAmpAttack, 3},   {124, kParamAmpDecay, 450},
  {124, kParamAmpSustain, 44},   {124, kParamAmpRelease, 700},   {124, kParamPitchAmount, -14},
  {124, kParamPitchAttack, 1},   {124, kParamPitchDecay, 90},   {124, kParamPitchSustain, 0},
  {124, kParamPitchRelease, 200},   {124, kParamUnisonVoices, 2},   {124, kParamUnisonDetune, 16},
  {124, kParamVelToAmp, 40},   {124, kParamVelToFilter, 34},   {124, kParamStereoSpread, 52},
  {124, kParamStereoDelay, 20},   {124, kParamTapeSat, 26},   {124, kParamTapeMix, 30},
  {124, kParamLfoRate, 1.4},   {124, kParamLfoDest, 1},   {124, kParamLfoAmount, 34},
  {124, kParamIns1Algo, 3},   {124, kParamIns1A, 34},   {124, kParamIns1B, 30},
  {124, kParamIns1C, 54},   {124, kParamIns1Mix, 18},   {124, kParamMstAlgo, 2},
  {124, kParamMstA, 24},   {124, kParamMstB, 18},   {124, kParamMstC, 30},
  {124, kParamMstMix, 10},
  {125, kParamEngine, 0},   {125, kParamPolyMode, 0},   {125, kParamPorta, 0},
  {125, kParamOsc1Shape, 30},   {125, kParamOsc1Level, 54},   {125, kParamOsc2Level, 0},
  {125, kParamOsc3Enable, 1},   {125, kParamOsc3Source, 1},   {125, kParamOsc3Morph, 5.8},
  {125, kParamOsc3Level, 88},   {125, kParamOsc3Pitch, 19},   {125, kParamSubLevel, 20},
  {125, kParamFilterType, 0},   {125, kParamFilterCutoff, 11000},   {125, kParamFilterReso, 18},
  {125, kParamFilterEnv, 44},   {125, kParamFiltAttack, 2},   {125, kParamFiltDecay, 900},
  {125, kParamFiltSustain, 48},   {125, kParamFiltRelease, 1600},   {125, kParamAmpAttack, 2},
  {125, kParamAmpDecay, 1000},   {125, kParamAmpSustain, 64},   {125, kParamAmpRelease, 1600},
  {125, kParamUnisonVoices, 2},   {125, kParamUnisonDetune, 13},   {125, kParamVelToAmp, 46},
  {125, kParamVelToFilter, 30},   {125, kParamStereoSpread, 44},   {125, kParamStereoDelay, 38},
  {125, kParamTapeSat, 24},   {125, kParamTapeMix, 32},   {125, kParamIns1Algo, 3},
  {125, kParamIns1A, 44},   {125, kParamIns1B, 36},   {125, kParamIns1C, 60},
  {125, kParamIns1Mix, 24},   {125, kParamMstAlgo, 2},   {125, kParamMstA, 24},
  {125, kParamMstB, 18},   {125, kParamMstC, 30},   {125, kParamMstMix, 10},
  {126, kParamEngine, 0},   {126, kParamPolyMode, 0},   {126, kParamPorta, 0},
  {126, kParamOsc1Shape, 22},   {126, kParamOsc1Level, 78},   {126, kParamOsc2Shape, 24},
  {126, kParamOsc2Pitch, 12},   {126, kParamOsc2Level, 62},   {126, kParamFmAmount, 62},
  {126, kParamSubLevel, 16},   {126, kParamFilterType, 0},   {126, kParamFilterCutoff, 9800},
  {126, kParamFilterReso, 26},   {126, kParamFilterEnv, 46},   {126, kParamFiltAttack, 2},
  {126, kParamFiltDecay, 600},   {126, kParamFiltSustain, 18},   {126, kParamFiltRelease, 800},
  {126, kParamAmpAttack, 1},   {126, kParamAmpDecay, 600},   {126, kParamAmpSustain, 30},
  {126, kParamAmpRelease, 800},   {126, kParamVelToAmp, 52},   {126, kParamVelToFilter, 28},
  {126, kParamStereoSpread, 54},   {126, kParamStereoDelay, 26},   {126, kParamTapeSat, 22},
  {126, kParamTapeMix, 30},   {126, kParamIns1Algo, 4},   {126, kParamIns1A, 26},
  {126, kParamIns1B, 36},   {126, kParamIns1C, 16},   {126, kParamIns1Mix, 16},
  {126, kParamMstAlgo, 2},   {126, kParamMstA, 24},   {126, kParamMstB, 18},
  {126, kParamMstC, 30},   {126, kParamMstMix, 10},
  {127, kParamEngine, 0},   {127, kParamPolyMode, 0},   {127, kParamPorta, 0},
  {127, kParamOsc1Shape, 52},   {127, kParamOsc1PW, 22},   {127, kParamOsc1Level, 88},
  {127, kParamOsc2Shape, 50},   {127, kParamOsc2Pitch, -12},   {127, kParamOsc2Level, 56},
  {127, kParamSubLevel, 30},   {127, kParamFilterType, 0},   {127, kParamFilterCutoff, 8800},
  {127, kParamFilterReso, 34},   {127, kParamFilterDrive, 0},   {127, kParamFilterEnv, 52},
  {127, kParamFiltAttack, 2},   {127, kParamFiltDecay, 600},   {127, kParamFiltSustain, 46},
  {127, kParamFiltRelease, 500},   {127, kParamAmpAttack, 1},   {127, kParamAmpDecay, 800},
  {127, kParamAmpSustain, 62},   {127, kParamAmpRelease, 500},   {127, kParamPitchAmount, -10},
  {127, kParamPitchDecay, 70},   {127, kParamUnisonVoices, 2},   {127, kParamUnisonDetune, 14},
  {127, kParamVelToAmp, 42},   {127, kParamVelToFilter, 32},   {127, kParamStereoSpread, 32},
  {127, kParamStereoDelay, 0},   {127, kParamTapeSat, 28},   {127, kParamTapeMix, 32},
  {127, kParamIns1Algo, 3},   {127, kParamIns1A, 34},   {127, kParamIns1B, 30},
  {127, kParamIns1C, 54},   {127, kParamIns1Mix, 18},   {127, kParamMstAlgo, 2},
  {127, kParamMstA, 24},   {127, kParamMstB, 18},   {127, kParamMstC, 30},
  {127, kParamMstMix, 10},
  {128, kParamEngine, 0},   {128, kParamPolyMode, 0},   {128, kParamPorta, 0},
  {128, kParamOsc1Shape, 44},   {128, kParamOsc1Level, 86},   {128, kParamOsc2Shape, 42},
  {128, kParamOsc2Pitch, 0},   {128, kParamOsc2Level, 74},   {128, kParamSubLevel, 24},
  {128, kParamNoiseLevel, 0},   {128, kParamFilterType, 0},   {128, kParamFilterCutoff, 9400},
  {128, kParamFilterReso, 28},   {128, kParamFilterDrive, 0},   {128, kParamFilterEnv, 54},
  {128, kParamFiltAttack, 2},   {128, kParamFiltDecay, 400},   {128, kParamFiltSustain, 20},
  {128, kParamFiltRelease, 500},   {128, kParamAmpAttack, 1},   {128, kParamAmpDecay, 400},
  {128, kParamAmpSustain, 20},   {128, kParamAmpRelease, 500},   {128, kParamPitchAmount, -14},
  {128, kParamPitchAttack, 1},   {128, kParamPitchDecay, 90},   {128, kParamPitchSustain, 0},
  {128, kParamPitchRelease, 200},   {128, kParamUnisonVoices, 2},   {128, kParamUnisonDetune, 16},
  {128, kParamVelToAmp, 40},   {128, kParamVelToFilter, 34},   {128, kParamStereoSpread, 52},
  {128, kParamStereoDelay, 20},   {128, kParamTapeSat, 26},   {128, kParamTapeMix, 30},
  {128, kParamIns1Algo, 3},   {128, kParamIns1A, 44},   {128, kParamIns1B, 36},
  {128, kParamIns1C, 60},   {128, kParamIns1Mix, 24},   {128, kParamMstAlgo, 2},
  {128, kParamMstA, 24},   {128, kParamMstB, 18},   {128, kParamMstC, 30},
  {128, kParamMstMix, 10},
  {129, kParamEngine, 0},   {129, kParamPolyMode, 0},   {129, kParamPorta, 0},
  {129, kParamOsc1Shape, 30},   {129, kParamOsc1Level, 54},   {129, kParamOsc2Level, 0},
  {129, kParamOsc3Enable, 1},   {129, kParamOsc3Source, 1},   {129, kParamOsc3Morph, 6.2},
  {129, kParamOsc3Level, 88},   {129, kParamOsc3Pitch, 0},   {129, kParamSubLevel, 20},
  {129, kParamFilterType, 0},   {129, kParamFilterCutoff, 9600},   {129, kParamFilterReso, 22},
  {129, kParamFilterEnv, 44},   {129, kParamFiltAttack, 2},   {129, kParamFiltDecay, 900},
  {129, kParamFiltSustain, 48},   {129, kParamFiltRelease, 1300},   {129, kParamAmpAttack, 2},
  {129, kParamAmpDecay, 1000},   {129, kParamAmpSustain, 64},   {129, kParamAmpRelease, 1300},
  {129, kParamUnisonVoices, 2},   {129, kParamUnisonDetune, 13},   {129, kParamVelToAmp, 46},
  {129, kParamVelToFilter, 30},   {129, kParamStereoSpread, 44},   {129, kParamStereoDelay, 30},
  {129, kParamTapeSat, 24},   {129, kParamTapeMix, 32},   {129, kParamIns1Algo, 4},
  {129, kParamIns1A, 26},   {129, kParamIns1B, 36},   {129, kParamIns1C, 16},
  {129, kParamIns1Mix, 16},   {129, kParamMstAlgo, 2},   {129, kParamMstA, 24},
  {129, kParamMstB, 18},   {129, kParamMstC, 30},   {129, kParamMstMix, 10},
  {130, kParamEngine, 0},   {130, kParamPolyMode, 0},   {130, kParamPorta, 0},
  {130, kParamOsc1Shape, 22},   {130, kParamOsc1Level, 78},   {130, kParamOsc2Shape, 24},
  {130, kParamOsc2Pitch, 12},   {130, kParamOsc2Level, 62},   {130, kParamFmAmount, 40},
  {130, kParamSubLevel, 16},   {130, kParamFilterType, 0},   {130, kParamFilterCutoff, 8400},
  {130, kParamFilterReso, 18},   {130, kParamFilterEnv, 46},   {130, kParamFiltAttack, 2},
  {130, kParamFiltDecay, 500},   {130, kParamFiltSustain, 8.4},   {130, kParamFiltRelease, 600},
  {130, kParamAmpAttack, 1},   {130, kParamAmpDecay, 500},   {130, kParamAmpSustain, 14},
  {130, kParamAmpRelease, 600},   {130, kParamVelToAmp, 52},   {130, kParamVelToFilter, 28},
  {130, kParamStereoSpread, 54},   {130, kParamStereoDelay, 26},   {130, kParamTapeSat, 22},
  {130, kParamTapeMix, 30},   {130, kParamIns1Algo, 3},   {130, kParamIns1A, 34},
  {130, kParamIns1B, 30},   {130, kParamIns1C, 54},   {130, kParamIns1Mix, 18},
  {130, kParamMstAlgo, 2},   {130, kParamMstA, 24},   {130, kParamMstB, 18},
  {130, kParamMstC, 30},   {130, kParamMstMix, 10},
  {131, kParamEngine, 0},   {131, kParamPolyMode, 0},   {131, kParamPorta, 0},
  {131, kParamOsc1Shape, 46},   {131, kParamOsc1Level, 88},   {131, kParamOsc2Shape, 48},
  {131, kParamOsc2Pitch, 14},   {131, kParamOsc2Level, 80},   {131, kParamOscSync, 1},
  {131, kParamSubLevel, 18},   {131, kParamFilterType, 0},   {131, kParamFilterCutoff, 9000},
  {131, kParamFilterReso, 36},   {131, kParamFilterDrive, 18},   {131, kParamFilterEnv, 58},
  {131, kParamFiltAttack, 2},   {131, kParamFiltDecay, 700},   {131, kParamFiltSustain, 42},
  {131, kParamFiltRelease, 500},   {131, kParamAmpAttack, 1},   {131, kParamAmpDecay, 900},
  {131, kParamAmpSustain, 58},   {131, kParamAmpRelease, 500},   {131, kParamUnisonVoices, 2},
  {131, kParamUnisonDetune, 12},   {131, kParamVelToAmp, 44},   {131, kParamVelToFilter, 30},
  {131, kParamStereoSpread, 48},   {131, kParamStereoDelay, 24},   {131, kParamTapeSat, 32},
  {131, kParamTapeMix, 34},   {131, kParamIns1Algo, 3},   {131, kParamIns1A, 44},
  {131, kParamIns1B, 36},   {131, kParamIns1C, 60},   {131, kParamIns1Mix, 24},
  {131, kParamMstAlgo, 2},   {131, kParamMstA, 24},   {131, kParamMstB, 18},
  {131, kParamMstC, 30},   {131, kParamMstMix, 10},
  {132, kParamEngine, 0},   {132, kParamPolyMode, 0},   {132, kParamPorta, 0},
  {132, kParamOsc1Shape, 44},   {132, kParamOsc1Level, 86},   {132, kParamOsc2Shape, 42},
  {132, kParamOsc2Pitch, 0},   {132, kParamOsc2Level, 74},   {132, kParamSubLevel, 24},
  {132, kParamNoiseLevel, 0},   {132, kParamFilterType, 0},   {132, kParamFilterCutoff, 7200},
  {132, kParamFilterReso, 46},   {132, kParamFilterDrive, 64},   {132, kParamFilterEnv, 54},
  {132, kParamFiltAttack, 2},   {132, kParamFiltDecay, 600},   {132, kParamFiltSustain, 50},
  {132, kParamFiltRelease, 700},   {132, kParamAmpAttack, 2},   {132, kParamAmpDecay, 600},
  {132, kParamAmpSustain, 50},   {132, kParamAmpRelease, 700},   {132, kParamPitchAmount, -14},
  {132, kParamPitchAttack, 1},   {132, kParamPitchDecay, 90},   {132, kParamPitchSustain, 0},
  {132, kParamPitchRelease, 200},   {132, kParamUnisonVoices, 2},   {132, kParamUnisonDetune, 16},
  {132, kParamVelToAmp, 40},   {132, kParamVelToFilter, 34},   {132, kParamStereoSpread, 52},
  {132, kParamStereoDelay, 20},   {132, kParamTapeSat, 46},   {132, kParamTapeMix, 48},
  {132, kParamIns1Algo, 4},   {132, kParamIns1A, 26},   {132, kParamIns1B, 36},
  {132, kParamIns1C, 16},   {132, kParamIns1Mix, 16},   {132, kParamMstAlgo, 2},
  {132, kParamMstA, 24},   {132, kParamMstB, 18},   {132, kParamMstC, 30},
  {132, kParamMstMix, 10},
  {133, kParamEngine, 0},   {133, kParamPolyMode, 0},   {133, kParamPorta, 0},
  {133, kParamOsc1Shape, 44},   {133, kParamOsc1Level, 86},   {133, kParamOsc2Shape, 42},
  {133, kParamOsc2Pitch, 7},   {133, kParamOsc2Level, 74},   {133, kParamSubLevel, 24},
  {133, kParamNoiseLevel, 0},   {133, kParamFilterType, 0},   {133, kParamFilterCutoff, 6400},
  {133, kParamFilterReso, 50},   {133, kParamFilterDrive, 70},   {133, kParamFilterEnv, 54},
  {133, kParamFiltAttack, 2},   {133, kParamFiltDecay, 550},   {133, kParamFiltSustain, 46},
  {133, kParamFiltRelease, 650},   {133, kParamAmpAttack, 2},   {133, kParamAmpDecay, 550},
  {133, kParamAmpSustain, 46},   {133, kParamAmpRelease, 650},   {133, kParamPitchAmount, -14},
  {133, kParamPitchAttack, 1},   {133, kParamPitchDecay, 90},   {133, kParamPitchSustain, 0},
  {133, kParamPitchRelease, 200},   {133, kParamUnisonVoices, 2},   {133, kParamUnisonDetune, 16},
  {133, kParamVelToAmp, 40},   {133, kParamVelToFilter, 34},   {133, kParamStereoSpread, 52},
  {133, kParamStereoDelay, 20},   {133, kParamTapeSat, 44},   {133, kParamTapeMix, 30},
  {133, kParamIns1Algo, 3},   {133, kParamIns1A, 34},   {133, kParamIns1B, 30},
  {133, kParamIns1C, 54},   {133, kParamIns1Mix, 18},   {133, kParamMstAlgo, 2},
  {133, kParamMstA, 24},   {133, kParamMstB, 18},   {133, kParamMstC, 30},
  {133, kParamMstMix, 10},
  {134, kParamEngine, 0},   {134, kParamPolyMode, 0},   {134, kParamPorta, 0},
  {134, kParamOsc1Shape, 52},   {134, kParamOsc1PW, 18},   {134, kParamOsc1Level, 88},
  {134, kParamOsc2Shape, 50},   {134, kParamOsc2Pitch, -12},   {134, kParamOsc2Level, 56},
  {134, kParamSubLevel, 30},   {134, kParamFilterType, 0},   {134, kParamFilterCutoff, 7800},
  {134, kParamFilterReso, 44},   {134, kParamFilterDrive, 60},   {134, kParamFilterEnv, 52},
  {134, kParamFiltAttack, 2},   {134, kParamFiltDecay, 600},   {134, kParamFiltSustain, 46},
  {134, kParamFiltRelease, 600},   {134, kParamAmpAttack, 1},   {134, kParamAmpDecay, 800},
  {134, kParamAmpSustain, 62},   {134, kParamAmpRelease, 600},   {134, kParamPitchAmount, -10},
  {134, kParamPitchDecay, 70},   {134, kParamUnisonVoices, 2},   {134, kParamUnisonDetune, 14},
  {134, kParamVelToAmp, 42},   {134, kParamVelToFilter, 32},   {134, kParamStereoSpread, 32},
  {134, kParamStereoDelay, 24},   {134, kParamTapeSat, 28},   {134, kParamTapeMix, 32},
  {134, kParamIns1Algo, 3},   {134, kParamIns1A, 44},   {134, kParamIns1B, 36},
  {134, kParamIns1C, 60},   {134, kParamIns1Mix, 24},   {134, kParamMstAlgo, 2},
  {134, kParamMstA, 24},   {134, kParamMstB, 18},   {134, kParamMstC, 30},
  {134, kParamMstMix, 10},
  {135, kParamEngine, 0},   {135, kParamPolyMode, 0},   {135, kParamPorta, 0},
  {135, kParamOsc1Shape, 46},   {135, kParamOsc1Level, 88},   {135, kParamOsc2Shape, 48},
  {135, kParamOsc2Pitch, 24},   {135, kParamOsc2Level, 80},   {135, kParamOscSync, 1},
  {135, kParamSubLevel, 18},   {135, kParamFilterType, 0},   {135, kParamFilterCutoff, 8200},
  {135, kParamFilterReso, 48},   {135, kParamFilterDrive, 56},   {135, kParamFilterEnv, 58},
  {135, kParamFiltAttack, 2},   {135, kParamFiltDecay, 700},   {135, kParamFiltSustain, 42},
  {135, kParamFiltRelease, 600},   {135, kParamAmpAttack, 1},   {135, kParamAmpDecay, 900},
  {135, kParamAmpSustain, 58},   {135, kParamAmpRelease, 600},   {135, kParamUnisonVoices, 2},
  {135, kParamUnisonDetune, 12},   {135, kParamVelToAmp, 44},   {135, kParamVelToFilter, 30},
  {135, kParamStereoSpread, 48},   {135, kParamStereoDelay, 24},   {135, kParamTapeSat, 32},
  {135, kParamTapeMix, 34},   {135, kParamIns1Algo, 4},   {135, kParamIns1A, 26},
  {135, kParamIns1B, 36},   {135, kParamIns1C, 16},   {135, kParamIns1Mix, 16},
  {135, kParamMstAlgo, 2},   {135, kParamMstA, 24},   {135, kParamMstB, 18},
  {135, kParamMstC, 30},   {135, kParamMstMix, 10},
  {136, kParamEngine, 0},   {136, kParamPolyMode, 0},   {136, kParamPorta, 0},
  {136, kParamOsc1Shape, 44},   {136, kParamOsc1Level, 86},   {136, kParamOsc2Shape, 42},
  {136, kParamOsc2Pitch, 0},   {136, kParamOsc2Level, 74},   {136, kParamSubLevel, 24},
  {136, kParamNoiseLevel, 0},   {136, kParamFilterType, 0},   {136, kParamFilterCutoff, 6000},
  {136, kParamFilterReso, 52},   {136, kParamFilterDrive, 58},   {136, kParamFilterEnv, 54},
  {136, kParamFiltAttack, 2},   {136, kParamFiltDecay, 500},   {136, kParamFiltSustain, 48},
  {136, kParamFiltRelease, 600},   {136, kParamAmpAttack, 2},   {136, kParamAmpDecay, 500},
  {136, kParamAmpSustain, 48},   {136, kParamAmpRelease, 600},   {136, kParamPitchAmount, -14},
  {136, kParamPitchAttack, 1},   {136, kParamPitchDecay, 90},   {136, kParamPitchSustain, 0},
  {136, kParamPitchRelease, 200},   {136, kParamUnisonVoices, 4},   {136, kParamUnisonDetune, 22},
  {136, kParamVelToAmp, 40},   {136, kParamVelToFilter, 34},   {136, kParamStereoSpread, 52},
  {136, kParamStereoDelay, 20},   {136, kParamTapeSat, 26},   {136, kParamTapeMix, 30},
  {136, kParamIns1Algo, 3},   {136, kParamIns1A, 34},   {136, kParamIns1B, 30},
  {136, kParamIns1C, 54},   {136, kParamIns1Mix, 18},   {136, kParamMstAlgo, 2},
  {136, kParamMstA, 24},   {136, kParamMstB, 18},   {136, kParamMstC, 30},
  {136, kParamMstMix, 10},
  {137, kParamEngine, 0},   {137, kParamPolyMode, 0},   {137, kParamPorta, 0},
  {137, kParamOsc1Shape, 22},   {137, kParamOsc1Level, 78},   {137, kParamOsc2Shape, 24},
  {137, kParamOsc2Pitch, 12},   {137, kParamOsc2Level, 62},   {137, kParamFmAmount, 58},
  {137, kParamSubLevel, 16},   {137, kParamFilterType, 0},   {137, kParamFilterCutoff, 9000},
  {137, kParamFilterReso, 42},   {137, kParamFilterEnv, 46},   {137, kParamFiltAttack, 2},
  {137, kParamFiltDecay, 550},   {137, kParamFiltSustain, 20.4},   {137, kParamFiltRelease, 700},
  {137, kParamAmpAttack, 1},   {137, kParamAmpDecay, 550},   {137, kParamAmpSustain, 34},
  {137, kParamAmpRelease, 700},   {137, kParamVelToAmp, 52},   {137, kParamVelToFilter, 28},
  {137, kParamStereoSpread, 54},   {137, kParamStereoDelay, 26},   {137, kParamTapeSat, 22},
  {137, kParamTapeMix, 30},   {137, kParamFilterDrive, 48},   {137, kParamIns1Algo, 3},
  {137, kParamIns1A, 44},   {137, kParamIns1B, 36},   {137, kParamIns1C, 60},
  {137, kParamIns1Mix, 24},   {137, kParamMstAlgo, 2},   {137, kParamMstA, 24},
  {137, kParamMstB, 18},   {137, kParamMstC, 30},   {137, kParamMstMix, 10},
  {138, kParamEngine, 0},   {138, kParamPolyMode, 0},   {138, kParamPorta, 0},
  {138, kParamOsc1Shape, 30},   {138, kParamOsc1Level, 54},   {138, kParamOsc2Level, 0},
  {138, kParamOsc3Enable, 1},   {138, kParamOsc3Source, 1},   {138, kParamOsc3Morph, 6.6},
  {138, kParamOsc3Level, 88},   {138, kParamOsc3Pitch, 0},   {138, kParamSubLevel, 20},
  {138, kParamFilterType, 0},   {138, kParamFilterCutoff, 7000},   {138, kParamFilterReso, 44},
  {138, kParamFilterEnv, 44},   {138, kParamFiltAttack, 2},   {138, kParamFiltDecay, 900},
  {138, kParamFiltSustain, 48},   {138, kParamFiltRelease, 800},   {138, kParamAmpAttack, 2},
  {138, kParamAmpDecay, 1000},   {138, kParamAmpSustain, 64},   {138, kParamAmpRelease, 800},
  {138, kParamUnisonVoices, 2},   {138, kParamUnisonDetune, 13},   {138, kParamVelToAmp, 46},
  {138, kParamVelToFilter, 30},   {138, kParamStereoSpread, 44},   {138, kParamStereoDelay, 30},
  {138, kParamTapeSat, 24},   {138, kParamTapeMix, 32},   {138, kParamFilterDrive, 52},
  {138, kParamIns1Algo, 4},   {138, kParamIns1A, 26},   {138, kParamIns1B, 36},
  {138, kParamIns1C, 16},   {138, kParamIns1Mix, 16},   {138, kParamMstAlgo, 2},
  {138, kParamMstA, 24},   {138, kParamMstB, 18},   {138, kParamMstC, 30},
  {138, kParamMstMix, 10},
  {139, kParamEngine, 0},   {139, kParamPolyMode, 0},   {139, kParamPorta, 22},
  {139, kParamOsc1Shape, 44},   {139, kParamOsc1Level, 86},   {139, kParamOsc2Shape, 42},
  {139, kParamOsc2Pitch, 0},   {139, kParamOsc2Level, 74},   {139, kParamSubLevel, 24},
  {139, kParamNoiseLevel, 0},   {139, kParamFilterType, 0},   {139, kParamFilterCutoff, 6600},
  {139, kParamFilterReso, 20},   {139, kParamFilterDrive, 0},   {139, kParamFilterEnv, 54},
  {139, kParamFiltAttack, 2},   {139, kParamFiltDecay, 900},   {139, kParamFiltSustain, 58},
  {139, kParamFiltRelease, 1400},   {139, kParamAmpAttack, 10},   {139, kParamAmpDecay, 900},
  {139, kParamAmpSustain, 58},   {139, kParamAmpRelease, 1400},   {139, kParamPitchAmount, -14},
  {139, kParamPitchAttack, 1},   {139, kParamPitchDecay, 90},   {139, kParamPitchSustain, 0},
  {139, kParamPitchRelease, 200},   {139, kParamUnisonVoices, 2},   {139, kParamUnisonDetune, 16},
  {139, kParamVelToAmp, 40},   {139, kParamVelToFilter, 34},   {139, kParamStereoSpread, 52},
  {139, kParamStereoDelay, 20},   {139, kParamTapeSat, 26},   {139, kParamTapeMix, 30},
  {139, kParamIns1Algo, 3},   {139, kParamIns1A, 34},   {139, kParamIns1B, 30},
  {139, kParamIns1C, 54},   {139, kParamIns1Mix, 18},   {139, kParamMstAlgo, 2},
  {139, kParamMstA, 24},   {139, kParamMstB, 18},   {139, kParamMstC, 30},
  {139, kParamMstMix, 10},
  {140, kParamEngine, 0},   {140, kParamPolyMode, 0},   {140, kParamPorta, 16},
  {140, kParamOsc1Shape, 40},   {140, kParamOsc1Level, 86},   {140, kParamOsc2Shape, 38},
  {140, kParamOsc2Pitch, 0},   {140, kParamOsc2Level, 74},   {140, kParamSubLevel, 24},
  {140, kParamNoiseLevel, 0},   {140, kParamFilterType, 0},   {140, kParamFilterCutoff, 5000},
  {140, kParamFilterReso, 28},   {140, kParamFilterDrive, 0},   {140, kParamFilterEnv, 54},
  {140, kParamFiltAttack, 60},   {140, kParamFiltDecay, 800},   {140, kParamFiltSustain, 56},
  {140, kParamFiltRelease, 1000},   {140, kParamAmpAttack, 40},   {140, kParamAmpDecay, 800},
  {140, kParamAmpSustain, 56},   {140, kParamAmpRelease, 1000},   {140, kParamPitchAmount, -14},
  {140, kParamPitchAttack, 1},   {140, kParamPitchDecay, 90},   {140, kParamPitchSustain, 0},
  {140, kParamPitchRelease, 200},   {140, kParamUnisonVoices, 2},   {140, kParamUnisonDetune, 16},
  {140, kParamVelToAmp, 40},   {140, kParamVelToFilter, 34},   {140, kParamStereoSpread, 52},
  {140, kParamStereoDelay, 20},   {140, kParamTapeSat, 26},   {140, kParamTapeMix, 30},
  {140, kParamIns1Algo, 3},   {140, kParamIns1A, 44},   {140, kParamIns1B, 36},
  {140, kParamIns1C, 60},   {140, kParamIns1Mix, 24},   {140, kParamMstAlgo, 2},
  {140, kParamMstA, 24},   {140, kParamMstB, 18},   {140, kParamMstC, 30},
  {140, kParamMstMix, 10},
  {141, kParamEngine, 0},   {141, kParamPolyMode, 0},   {141, kParamPorta, 0},
  {141, kParamOsc1Shape, 30},   {141, kParamOsc1Level, 54},   {141, kParamOsc2Level, 0},
  {141, kParamOsc3Enable, 1},   {141, kParamOsc3Source, 1},   {141, kParamOsc3Morph, 3.4},
  {141, kParamOsc3Level, 88},   {141, kParamOsc3Pitch, 12},   {141, kParamSubLevel, 20},
  {141, kParamFilterType, 0},   {141, kParamFilterCutoff, 6200},   {141, kParamFilterReso, 22},
  {141, kParamFilterEnv, 44},   {141, kParamFiltAttack, 2},   {141, kParamFiltDecay, 900},
  {141, kParamFiltSustain, 48},   {141, kParamFiltRelease, 1500},   {141, kParamAmpAttack, 70},
  {141, kParamAmpDecay, 1000},   {141, kParamAmpSustain, 64},   {141, kParamAmpRelease, 1500},
  {141, kParamUnisonVoices, 2},   {141, kParamUnisonDetune, 13},   {141, kParamVelToAmp, 46},
  {141, kParamVelToFilter, 30},   {141, kParamStereoSpread, 44},   {141, kParamStereoDelay, 30},
  {141, kParamTapeSat, 24},   {141, kParamTapeMix, 32},   {141, kParamIns1Algo, 4},
  {141, kParamIns1A, 26},   {141, kParamIns1B, 36},   {141, kParamIns1C, 16},
  {141, kParamIns1Mix, 16},   {141, kParamMstAlgo, 2},   {141, kParamMstA, 24},
  {141, kParamMstB, 18},   {141, kParamMstC, 30},   {141, kParamMstMix, 10},
  {142, kParamEngine, 0},   {142, kParamPolyMode, 0},   {142, kParamPorta, 18},
  {142, kParamOsc1Shape, 44},   {142, kParamOsc1Level, 86},   {142, kParamOsc2Shape, 42},
  {142, kParamOsc2Pitch, 0},   {142, kParamOsc2Level, 74},   {142, kParamSubLevel, 24},
  {142, kParamNoiseLevel, 0},   {142, kParamFilterType, 0},   {142, kParamFilterCutoff, 7600},
  {142, kParamFilterReso, 18},   {142, kParamFilterDrive, 0},   {142, kParamFilterEnv, 54},
  {142, kParamFiltAttack, 2},   {142, kParamFiltDecay, 900},   {142, kParamFiltSustain, 60},
  {142, kParamFiltRelease, 1500},   {142, kParamAmpAttack, 12},   {142, kParamAmpDecay, 900},
  {142, kParamAmpSustain, 60},   {142, kParamAmpRelease, 1500},   {142, kParamPitchAmount, -14},
  {142, kParamPitchAttack, 1},   {142, kParamPitchDecay, 90},   {142, kParamPitchSustain, 0},
  {142, kParamPitchRelease, 200},   {142, kParamUnisonVoices, 2},   {142, kParamUnisonDetune, 16},
  {142, kParamVelToAmp, 40},   {142, kParamVelToFilter, 34},   {142, kParamStereoSpread, 56},
  {142, kParamStereoDelay, 20},   {142, kParamTapeSat, 26},   {142, kParamTapeMix, 30},
  {142, kParamIns1Algo, 3},   {142, kParamIns1A, 34},   {142, kParamIns1B, 30},
  {142, kParamIns1C, 54},   {142, kParamIns1Mix, 18},   {142, kParamMstAlgo, 2},
  {142, kParamMstA, 24},   {142, kParamMstB, 18},   {142, kParamMstC, 30},
  {142, kParamMstMix, 10},
  {143, kParamEngine, 0},   {143, kParamPolyMode, 0},   {143, kParamPorta, 0},
  {143, kParamOsc1Shape, 22},   {143, kParamOsc1Level, 78},   {143, kParamOsc2Shape, 24},
  {143, kParamOsc2Pitch, 12},   {143, kParamOsc2Level, 62},   {143, kParamFmAmount, 28},
  {143, kParamSubLevel, 16},   {143, kParamFilterType, 0},   {143, kParamFilterCutoff, 7400},
  {143, kParamFilterReso, 20},   {143, kParamFilterEnv, 46},   {143, kParamFiltAttack, 2},
  {143, kParamFiltDecay, 1000},   {143, kParamFiltSustain, 27.599999999999998},   {143, kParamFiltRelease, 1300},
  {143, kParamAmpAttack, 8},   {143, kParamAmpDecay, 1000},   {143, kParamAmpSustain, 46},
  {143, kParamAmpRelease, 1300},   {143, kParamVelToAmp, 52},   {143, kParamVelToFilter, 28},
  {143, kParamStereoSpread, 54},   {143, kParamStereoDelay, 26},   {143, kParamTapeSat, 22},
  {143, kParamTapeMix, 30},   {143, kParamIns1Algo, 3},   {143, kParamIns1A, 44},
  {143, kParamIns1B, 36},   {143, kParamIns1C, 60},   {143, kParamIns1Mix, 24},
  {143, kParamMstAlgo, 2},   {143, kParamMstA, 24},   {143, kParamMstB, 18},
  {143, kParamMstC, 30},   {143, kParamMstMix, 10},
  {144, kParamEngine, 0},   {144, kParamPolyMode, 0},   {144, kParamPorta, 0},
  {144, kParamOsc1Shape, 52},   {144, kParamOsc1PW, 46},   {144, kParamOsc1Level, 88},
  {144, kParamOsc2Shape, 50},   {144, kParamOsc2Pitch, -12},   {144, kParamOsc2Level, 56},
  {144, kParamSubLevel, 30},   {144, kParamFilterType, 0},   {144, kParamFilterCutoff, 5400},
  {144, kParamFilterReso, 24},   {144, kParamFilterDrive, 0},   {144, kParamFilterEnv, 52},
  {144, kParamFiltAttack, 2},   {144, kParamFiltDecay, 600},   {144, kParamFiltSustain, 46},
  {144, kParamFiltRelease, 1000},   {144, kParamAmpAttack, 30},   {144, kParamAmpDecay, 800},
  {144, kParamAmpSustain, 62},   {144, kParamAmpRelease, 1000},   {144, kParamPitchAmount, -10},
  {144, kParamPitchDecay, 70},   {144, kParamUnisonVoices, 2},   {144, kParamUnisonDetune, 14},
  {144, kParamVelToAmp, 42},   {144, kParamVelToFilter, 32},   {144, kParamStereoSpread, 32},
  {144, kParamStereoDelay, 24},   {144, kParamTapeSat, 28},   {144, kParamTapeMix, 32},
  {144, kParamIns1Algo, 4},   {144, kParamIns1A, 26},   {144, kParamIns1B, 36},
  {144, kParamIns1C, 16},   {144, kParamIns1Mix, 16},   {144, kParamMstAlgo, 2},
  {144, kParamMstA, 24},   {144, kParamMstB, 18},   {144, kParamMstC, 30},
  {144, kParamMstMix, 10},
  {145, kParamEngine, 0},   {145, kParamPolyMode, 0},   {145, kParamPorta, 10},
  {145, kParamOsc1Shape, 44},   {145, kParamOsc1Level, 86},   {145, kParamOsc2Shape, 42},
  {145, kParamOsc2Pitch, 0},   {145, kParamOsc2Level, 74},   {145, kParamSubLevel, 24},
  {145, kParamNoiseLevel, 0},   {145, kParamFilterType, 0},   {145, kParamFilterCutoff, 6400},
  {145, kParamFilterReso, 26},   {145, kParamFilterDrive, 0},   {145, kParamFilterEnv, 54},
  {145, kParamFiltAttack, 2},   {145, kParamFiltDecay, 700},   {145, kParamFiltSustain, 50},
  {145, kParamFiltRelease, 1000},   {145, kParamAmpAttack, 6},   {145, kParamAmpDecay, 700},
  {145, kParamAmpSustain, 50},   {145, kParamAmpRelease, 1000},   {145, kParamPitchAmount, -14},
  {145, kParamPitchAttack, 1},   {145, kParamPitchDecay, 90},   {145, kParamPitchSustain, 0},
  {145, kParamPitchRelease, 200},   {145, kParamUnisonVoices, 2},   {145, kParamUnisonDetune, 16},
  {145, kParamVelToAmp, 40},   {145, kParamVelToFilter, 34},   {145, kParamStereoSpread, 52},
  {145, kParamStereoDelay, 20},   {145, kParamTapeSat, 26},   {145, kParamTapeMix, 44},
  {145, kParamTapeWow, 44},   {145, kParamTapeFlutter, 32},   {145, kParamIns1Algo, 3},
  {145, kParamIns1A, 34},   {145, kParamIns1B, 30},   {145, kParamIns1C, 54},
  {145, kParamIns1Mix, 18},   {145, kParamMstAlgo, 2},   {145, kParamMstA, 24},
  {145, kParamMstB, 18},   {145, kParamMstC, 30},   {145, kParamMstMix, 10},
  {146, kParamEngine, 0},   {146, kParamPolyMode, 0},   {146, kParamPorta, 0},
  {146, kParamOsc1Shape, 52},   {146, kParamOsc1PW, 38},   {146, kParamOsc1Level, 88},
  {146, kParamOsc2Shape, 50},   {146, kParamOsc2Pitch, -12},   {146, kParamOsc2Level, 56},
  {146, kParamSubLevel, 30},   {146, kParamFilterType, 0},   {146, kParamFilterCutoff, 6000},
  {146, kParamFilterReso, 30},   {146, kParamFilterDrive, 0},   {146, kParamFilterEnv, 52},
  {146, kParamFiltAttack, 2},   {146, kParamFiltDecay, 600},   {146, kParamFiltSustain, 46},
  {146, kParamFiltRelease, 900},   {146, kParamAmpAttack, 6},   {146, kParamAmpDecay, 800},
  {146, kParamAmpSustain, 62},   {146, kParamAmpRelease, 900},   {146, kParamPitchAmount, -10},
  {146, kParamPitchDecay, 70},   {146, kParamUnisonVoices, 2},   {146, kParamUnisonDetune, 30},
  {146, kParamVelToAmp, 42},   {146, kParamVelToFilter, 32},   {146, kParamStereoSpread, 32},
  {146, kParamStereoDelay, 24},   {146, kParamTapeSat, 28},   {146, kParamTapeMix, 32},
  {146, kParamTapeWow, 40},   {146, kParamIns1Algo, 3},   {146, kParamIns1A, 44},
  {146, kParamIns1B, 36},   {146, kParamIns1C, 60},   {146, kParamIns1Mix, 24},
  {146, kParamMstAlgo, 2},   {146, kParamMstA, 24},   {146, kParamMstB, 18},
  {146, kParamMstC, 30},   {146, kParamMstMix, 10},
  {147, kParamEngine, 0},   {147, kParamPolyMode, 0},   {147, kParamPorta, 0},
  {147, kParamOsc1Shape, 22},   {147, kParamOsc1Level, 78},   {147, kParamOsc2Shape, 24},
  {147, kParamOsc2Pitch, 12},   {147, kParamOsc2Level, 62},   {147, kParamFmAmount, 30},
  {147, kParamSubLevel, 16},   {147, kParamFilterType, 0},   {147, kParamFilterCutoff, 7000},
  {147, kParamFilterReso, 22},   {147, kParamFilterEnv, 46},   {147, kParamFiltAttack, 2},
  {147, kParamFiltDecay, 900},   {147, kParamFiltSustain, 25.2},   {147, kParamFiltRelease, 1200},
  {147, kParamAmpAttack, 6},   {147, kParamAmpDecay, 900},   {147, kParamAmpSustain, 42},
  {147, kParamAmpRelease, 1200},   {147, kParamVelToAmp, 52},   {147, kParamVelToFilter, 28},
  {147, kParamStereoSpread, 54},   {147, kParamStereoDelay, 26},   {147, kParamTapeSat, 22},
  {147, kParamTapeMix, 30},   {147, kParamTapeWow, 42},   {147, kParamTapeHiss, 12},
  {147, kParamIns1Algo, 4},   {147, kParamIns1A, 26},   {147, kParamIns1B, 36},
  {147, kParamIns1C, 16},   {147, kParamIns1Mix, 16},   {147, kParamMstAlgo, 2},
  {147, kParamMstA, 24},   {147, kParamMstB, 18},   {147, kParamMstC, 30},
  {147, kParamMstMix, 10},
  {148, kParamEngine, 0},   {148, kParamPolyMode, 0},   {148, kParamPorta, 16},
  {148, kParamOsc1Shape, 44},   {148, kParamOsc1Level, 86},   {148, kParamOsc2Shape, 42},
  {148, kParamOsc2Pitch, 0},   {148, kParamOsc2Level, 74},   {148, kParamSubLevel, 24},
  {148, kParamNoiseLevel, 0},   {148, kParamFilterType, 0},   {148, kParamFilterCutoff, 6800},
  {148, kParamFilterReso, 24},   {148, kParamFilterDrive, 0},   {148, kParamFilterEnv, 54},
  {148, kParamFiltAttack, 2},   {148, kParamFiltDecay, 800},   {148, kParamFiltSustain, 52},
  {148, kParamFiltRelease, 1100},   {148, kParamAmpAttack, 8},   {148, kParamAmpDecay, 800},
  {148, kParamAmpSustain, 52},   {148, kParamAmpRelease, 1100},   {148, kParamPitchAmount, -14},
  {148, kParamPitchAttack, 1},   {148, kParamPitchDecay, 90},   {148, kParamPitchSustain, 0},
  {148, kParamPitchRelease, 200},   {148, kParamUnisonVoices, 2},   {148, kParamUnisonDetune, 16},
  {148, kParamVelToAmp, 40},   {148, kParamVelToFilter, 34},   {148, kParamStereoSpread, 52},
  {148, kParamStereoDelay, 20},   {148, kParamTapeSat, 38},   {148, kParamTapeMix, 46},
  {148, kParamTapeFlutter, 36},   {148, kParamIns1Algo, 3},   {148, kParamIns1A, 34},
  {148, kParamIns1B, 30},   {148, kParamIns1C, 54},   {148, kParamIns1Mix, 18},
  {148, kParamMstAlgo, 2},   {148, kParamMstA, 24},   {148, kParamMstB, 18},
  {148, kParamMstC, 30},   {148, kParamMstMix, 10},
  {149, kParamEngine, 0},   {149, kParamPolyMode, 0},   {149, kParamPorta, 0},
  {149, kParamOsc1Shape, 30},   {149, kParamOsc1Level, 54},   {149, kParamOsc2Level, 0},
  {149, kParamOsc3Enable, 1},   {149, kParamOsc3Source, 1},   {149, kParamOsc3Morph, 2.2},
  {149, kParamOsc3Level, 88},   {149, kParamOsc3Pitch, 0},   {149, kParamSubLevel, 20},
  {149, kParamFilterType, 0},   {149, kParamFilterCutoff, 6400},   {149, kParamFilterReso, 26},
  {149, kParamFilterEnv, 44},   {149, kParamFiltAttack, 2},   {149, kParamFiltDecay, 900},
  {149, kParamFiltSustain, 48},   {149, kParamFiltRelease, 1400},   {149, kParamAmpAttack, 40},
  {149, kParamAmpDecay, 1000},   {149, kParamAmpSustain, 64},   {149, kParamAmpRelease, 1400},
  {149, kParamUnisonVoices, 2},   {149, kParamUnisonDetune, 26},   {149, kParamVelToAmp, 46},
  {149, kParamVelToFilter, 30},   {149, kParamStereoSpread, 44},   {149, kParamStereoDelay, 30},
  {149, kParamTapeSat, 24},   {149, kParamTapeMix, 32},   {149, kParamTapeWow, 46},
  {149, kParamIns1Algo, 3},   {149, kParamIns1A, 44},   {149, kParamIns1B, 36},
  {149, kParamIns1C, 60},   {149, kParamIns1Mix, 24},   {149, kParamMstAlgo, 2},
  {149, kParamMstA, 24},   {149, kParamMstB, 18},   {149, kParamMstC, 30},
  {149, kParamMstMix, 10},
  {150, kParamEngine, 0},   {150, kParamPolyMode, 0},   {150, kParamPorta, 0},
  {150, kParamOsc1Shape, 52},   {150, kParamOsc1PW, 26},   {150, kParamOsc1Level, 88},
  {150, kParamOsc2Shape, 50},   {150, kParamOsc2Pitch, -12},   {150, kParamOsc2Level, 56},
  {150, kParamSubLevel, 30},   {150, kParamFilterType, 0},   {150, kParamFilterCutoff, 5600},
  {150, kParamFilterReso, 32},   {150, kParamFilterDrive, 0},   {150, kParamFilterEnv, 52},
  {150, kParamFiltAttack, 2},   {150, kParamFiltDecay, 600},   {150, kParamFiltSustain, 46},
  {150, kParamFiltRelease, 800},   {150, kParamAmpAttack, 4},   {150, kParamAmpDecay, 800},
  {150, kParamAmpSustain, 62},   {150, kParamAmpRelease, 800},   {150, kParamPitchAmount, -10},
  {150, kParamPitchDecay, 70},   {150, kParamUnisonVoices, 2},   {150, kParamUnisonDetune, 14},
  {150, kParamVelToAmp, 42},   {150, kParamVelToFilter, 32},   {150, kParamStereoSpread, 32},
  {150, kParamStereoDelay, 24},   {150, kParamTapeSat, 28},   {150, kParamTapeMix, 42},
  {150, kParamTapeWow, 48},   {150, kParamIns1Algo, 4},   {150, kParamIns1A, 26},
  {150, kParamIns1B, 36},   {150, kParamIns1C, 16},   {150, kParamIns1Mix, 16},
  {150, kParamMstAlgo, 2},   {150, kParamMstA, 24},   {150, kParamMstB, 18},
  {150, kParamMstC, 30},   {150, kParamMstMix, 10},
  {151, kParamEngine, 0},   {151, kParamPolyMode, 0},   {151, kParamPorta, 20},
  {151, kParamOsc1Shape, 44},   {151, kParamOsc1Level, 86},   {151, kParamOsc2Shape, 42},
  {151, kParamOsc2Pitch, 0},   {151, kParamOsc2Level, 74},   {151, kParamSubLevel, 38},
  {151, kParamNoiseLevel, 0},   {151, kParamFilterType, 0},   {151, kParamFilterCutoff, 5200},
  {151, kParamFilterReso, 28},   {151, kParamFilterDrive, 0},   {151, kParamFilterEnv, 54},
  {151, kParamFiltAttack, 2},   {151, kParamFiltDecay, 900},   {151, kParamFiltSustain, 54},
  {151, kParamFiltRelease, 1200},   {151, kParamAmpAttack, 10},   {151, kParamAmpDecay, 900},
  {151, kParamAmpSustain, 54},   {151, kParamAmpRelease, 1200},   {151, kParamPitchAmount, -14},
  {151, kParamPitchAttack, 1},   {151, kParamPitchDecay, 90},   {151, kParamPitchSustain, 0},
  {151, kParamPitchRelease, 200},   {151, kParamUnisonVoices, 2},   {151, kParamUnisonDetune, 16},
  {151, kParamVelToAmp, 40},   {151, kParamVelToFilter, 34},   {151, kParamStereoSpread, 52},
  {151, kParamStereoDelay, 20},   {151, kParamTapeSat, 26},   {151, kParamTapeMix, 40},
  {151, kParamIns1Algo, 3},   {151, kParamIns1A, 34},   {151, kParamIns1B, 30},
  {151, kParamIns1C, 54},   {151, kParamIns1Mix, 18},   {151, kParamMstAlgo, 2},
  {151, kParamMstA, 24},   {151, kParamMstB, 18},   {151, kParamMstC, 30},
  {151, kParamMstMix, 10},
  {152, kParamEngine, 0},   {152, kParamPolyMode, 0},   {152, kParamPorta, 14},
  {152, kParamOsc1Shape, 44},   {152, kParamOsc1Level, 86},   {152, kParamOsc2Shape, 42},
  {152, kParamOsc2Pitch, 0},   {152, kParamOsc2Level, 74},   {152, kParamSubLevel, 24},
  {152, kParamNoiseLevel, 0},   {152, kParamFilterType, 0},   {152, kParamFilterCutoff, 7000},
  {152, kParamFilterReso, 32},   {152, kParamFilterDrive, 0},   {152, kParamFilterEnv, 54},
  {152, kParamFiltAttack, 2},   {152, kParamFiltDecay, 600},   {152, kParamFiltSustain, 50},
  {152, kParamFiltRelease, 800},   {152, kParamAmpAttack, 3},   {152, kParamAmpDecay, 600},
  {152, kParamAmpSustain, 50},   {152, kParamAmpRelease, 800},   {152, kParamPitchAmount, -14},
  {152, kParamPitchAttack, 1},   {152, kParamPitchDecay, 90},   {152, kParamPitchSustain, 0},
  {152, kParamPitchRelease, 200},   {152, kParamUnisonVoices, 6},   {152, kParamUnisonDetune, 24},
  {152, kParamVelToAmp, 40},   {152, kParamVelToFilter, 34},   {152, kParamStereoSpread, 72},
  {152, kParamStereoDelay, 20},   {152, kParamTapeSat, 26},   {152, kParamTapeMix, 30},
  {152, kParamIns1Algo, 3},   {152, kParamIns1A, 44},   {152, kParamIns1B, 36},
  {152, kParamIns1C, 60},   {152, kParamIns1Mix, 24},   {152, kParamMstAlgo, 2},
  {152, kParamMstA, 24},   {152, kParamMstB, 18},   {152, kParamMstC, 30},
  {152, kParamMstMix, 10},
  {153, kParamEngine, 0},   {153, kParamPolyMode, 0},   {153, kParamPorta, 0},
  {153, kParamOsc1Shape, 46},   {153, kParamOsc1Level, 88},   {153, kParamOsc2Shape, 48},
  {153, kParamOsc2Pitch, 12},   {153, kParamOsc2Level, 80},   {153, kParamOscSync, 1},
  {153, kParamSubLevel, 18},   {153, kParamFilterType, 0},   {153, kParamFilterCutoff, 7600},
  {153, kParamFilterReso, 40},   {153, kParamFilterDrive, 18},   {153, kParamFilterEnv, 58},
  {153, kParamFiltAttack, 2},   {153, kParamFiltDecay, 700},   {153, kParamFiltSustain, 42},
  {153, kParamFiltRelease, 700},   {153, kParamAmpAttack, 2},   {153, kParamAmpDecay, 900},
  {153, kParamAmpSustain, 58},   {153, kParamAmpRelease, 700},   {153, kParamUnisonVoices, 2},
  {153, kParamUnisonDetune, 12},   {153, kParamVelToAmp, 44},   {153, kParamVelToFilter, 30},
  {153, kParamStereoSpread, 48},   {153, kParamStereoDelay, 24},   {153, kParamTapeSat, 32},
  {153, kParamTapeMix, 34},   {153, kParamIns1Algo, 4},   {153, kParamIns1A, 26},
  {153, kParamIns1B, 36},   {153, kParamIns1C, 16},   {153, kParamIns1Mix, 16},
  {153, kParamMstAlgo, 2},   {153, kParamMstA, 24},   {153, kParamMstB, 18},
  {153, kParamMstC, 30},   {153, kParamMstMix, 10},
  {154, kParamEngine, 0},   {154, kParamPolyMode, 0},   {154, kParamPorta, 0},
  {154, kParamOsc1Shape, 22},   {154, kParamOsc1Level, 78},   {154, kParamOsc2Shape, 24},
  {154, kParamOsc2Pitch, 12},   {154, kParamOsc2Level, 62},   {154, kParamFmAmount, 52},
  {154, kParamSubLevel, 16},   {154, kParamFilterType, 0},   {154, kParamFilterCutoff, 9200},
  {154, kParamFilterReso, 28},   {154, kParamFilterEnv, 46},   {154, kParamFiltAttack, 2},
  {154, kParamFiltDecay, 700},   {154, kParamFiltSustain, 25.2},   {154, kParamFiltRelease, 900},
  {154, kParamAmpAttack, 2},   {154, kParamAmpDecay, 700},   {154, kParamAmpSustain, 42},
  {154, kParamAmpRelease, 900},   {154, kParamVelToAmp, 52},   {154, kParamVelToFilter, 28},
  {154, kParamStereoSpread, 54},   {154, kParamStereoDelay, 26},   {154, kParamTapeSat, 22},
  {154, kParamTapeMix, 30},   {154, kParamIns1Algo, 3},   {154, kParamIns1A, 34},
  {154, kParamIns1B, 30},   {154, kParamIns1C, 54},   {154, kParamIns1Mix, 18},
  {154, kParamMstAlgo, 2},   {154, kParamMstA, 24},   {154, kParamMstB, 18},
  {154, kParamMstC, 30},   {154, kParamMstMix, 10},
  {155, kParamEngine, 0},   {155, kParamPolyMode, 0},   {155, kParamPorta, 0},
  {155, kParamOsc1Shape, 52},   {155, kParamOsc1PW, 18},   {155, kParamOsc1Level, 88},
  {155, kParamOsc2Shape, 50},   {155, kParamOsc2Pitch, -12},   {155, kParamOsc2Level, 56},
  {155, kParamSubLevel, 30},   {155, kParamFilterType, 0},   {155, kParamFilterCutoff, 8600},
  {155, kParamFilterReso, 36},   {155, kParamFilterDrive, 0},   {155, kParamFilterEnv, 52},
  {155, kParamFiltAttack, 2},   {155, kParamFiltDecay, 600},   {155, kParamFiltSustain, 46},
  {155, kParamFiltRelease, 600},   {155, kParamAmpAttack, 2},   {155, kParamAmpDecay, 800},
  {155, kParamAmpSustain, 62},   {155, kParamAmpRelease, 600},   {155, kParamPitchAmount, -10},
  {155, kParamPitchDecay, 70},   {155, kParamUnisonVoices, 2},   {155, kParamUnisonDetune, 14},
  {155, kParamVelToAmp, 42},   {155, kParamVelToFilter, 32},   {155, kParamStereoSpread, 32},
  {155, kParamStereoDelay, 24},   {155, kParamTapeSat, 28},   {155, kParamTapeMix, 32},
  {155, kParamIns1Algo, 3},   {155, kParamIns1A, 44},   {155, kParamIns1B, 36},
  {155, kParamIns1C, 60},   {155, kParamIns1Mix, 24},   {155, kParamMstAlgo, 2},
  {155, kParamMstA, 24},   {155, kParamMstB, 18},   {155, kParamMstC, 30},
  {155, kParamMstMix, 10},
  {156, kParamEngine, 0},   {156, kParamPolyMode, 0},   {156, kParamPorta, 0},
  {156, kParamOsc1Shape, 30},   {156, kParamOsc1Level, 54},   {156, kParamOsc2Level, 0},
  {156, kParamOsc3Enable, 1},   {156, kParamOsc3Source, 1},   {156, kParamOsc3Morph, 1},
  {156, kParamOsc3Level, 88},   {156, kParamOsc3Pitch, 0},   {156, kParamSubLevel, 20},
  {156, kParamFilterType, 0},   {156, kParamFilterCutoff, 7800},   {156, kParamFilterReso, 34},
  {156, kParamFilterEnv, 44},   {156, kParamFiltAttack, 2},   {156, kParamFiltDecay, 900},
  {156, kParamFiltSustain, 48},   {156, kParamFiltRelease, 1200},   {156, kParamAmpAttack, 3},
  {156, kParamAmpDecay, 1000},   {156, kParamAmpSustain, 64},   {156, kParamAmpRelease, 1200},
  {156, kParamUnisonVoices, 2},   {156, kParamUnisonDetune, 13},   {156, kParamVelToAmp, 46},
  {156, kParamVelToFilter, 30},   {156, kParamStereoSpread, 44},   {156, kParamStereoDelay, 30},
  {156, kParamTapeSat, 24},   {156, kParamTapeMix, 32},   {156, kParamLfoRate, 0.5},
  {156, kParamLfoDest, 16},   {156, kParamLfoAmount, 58},   {156, kParamIns1Algo, 4},
  {156, kParamIns1A, 26},   {156, kParamIns1B, 36},   {156, kParamIns1C, 16},
  {156, kParamIns1Mix, 16},   {156, kParamMstAlgo, 2},   {156, kParamMstA, 24},
  {156, kParamMstB, 18},   {156, kParamMstC, 30},   {156, kParamMstMix, 10},
  {157, kParamEngine, 0},   {157, kParamPolyMode, 0},   {157, kParamPorta, 0},
  {157, kParamOsc1Shape, 44},   {157, kParamOsc1Level, 86},   {157, kParamOsc2Shape, 42},
  {157, kParamOsc2Pitch, 0},   {157, kParamOsc2Level, 74},   {157, kParamSubLevel, 36},
  {157, kParamNoiseLevel, 0},   {157, kParamFilterType, 0},   {157, kParamFilterCutoff, 5400},
  {157, kParamFilterReso, 42},   {157, kParamFilterDrive, 44},   {157, kParamFilterEnv, 54},
  {157, kParamFiltAttack, 2},   {157, kParamFiltDecay, 600},   {157, kParamFiltSustain, 48},
  {157, kParamFiltRelease, 750},   {157, kParamAmpAttack, 3},   {157, kParamAmpDecay, 600},
  {157, kParamAmpSustain, 48},   {157, kParamAmpRelease, 750},   {157, kParamPitchAmount, -14},
  {157, kParamPitchAttack, 1},   {157, kParamPitchDecay, 90},   {157, kParamPitchSustain, 0},
  {157, kParamPitchRelease, 200},   {157, kParamUnisonVoices, 2},   {157, kParamUnisonDetune, 16},
  {157, kParamVelToAmp, 40},   {157, kParamVelToFilter, 34},   {157, kParamStereoSpread, 52},
  {157, kParamStereoDelay, 20},   {157, kParamTapeSat, 26},   {157, kParamTapeMix, 30},
  {157, kParamIns1Algo, 3},   {157, kParamIns1A, 34},   {157, kParamIns1B, 30},
  {157, kParamIns1C, 54},   {157, kParamIns1Mix, 18},   {157, kParamMstAlgo, 2},
  {157, kParamMstA, 24},   {157, kParamMstB, 18},   {157, kParamMstC, 30},
  {157, kParamMstMix, 10},
  {158, kParamEngine, 0},   {158, kParamPolyMode, 0},   {158, kParamPorta, 0},
  {158, kParamOsc1Shape, 44},   {158, kParamOsc1Level, 86},   {158, kParamOsc2Shape, 42},
  {158, kParamOsc2Pitch, 0},   {158, kParamOsc2Level, 74},   {158, kParamSubLevel, 24},
  {158, kParamNoiseLevel, 0},   {158, kParamFilterType, 0},   {158, kParamFilterCutoff, 6600},
  {158, kParamFilterReso, 34},   {158, kParamFilterDrive, 0},   {158, kParamFilterEnv, 54},
  {158, kParamFiltAttack, 2},   {158, kParamFiltDecay, 650},   {158, kParamFiltSustain, 50},
  {158, kParamFiltRelease, 850},   {158, kParamAmpAttack, 3},   {158, kParamAmpDecay, 650},
  {158, kParamAmpSustain, 50},   {158, kParamAmpRelease, 850},   {158, kParamPitchAmount, -14},
  {158, kParamPitchAttack, 1},   {158, kParamPitchDecay, 90},   {158, kParamPitchSustain, 0},
  {158, kParamPitchRelease, 200},   {158, kParamUnisonVoices, 4},   {158, kParamUnisonDetune, 20},
  {158, kParamVelToAmp, 40},   {158, kParamVelToFilter, 34},   {158, kParamStereoSpread, 58},
  {158, kParamStereoDelay, 34},   {158, kParamTapeSat, 26},   {158, kParamTapeMix, 30},
  {158, kParamIns1Algo, 3},   {158, kParamIns1A, 44},   {158, kParamIns1B, 36},
  {158, kParamIns1C, 60},   {158, kParamIns1Mix, 24},   {158, kParamMstAlgo, 2},
  {158, kParamMstA, 24},   {158, kParamMstB, 18},   {158, kParamMstC, 30},
  {158, kParamMstMix, 10},
  // poly synths
  {159, kParamEngine, 0},   {159, kParamPolyMode, 0},   {159, kParamOsc1Shape, 24},
  {159, kParamOsc1Level, 88},   {159, kParamOsc2Level, 0},   {159, kParamSubLevel, 30},
  {159, kParamNoiseLevel, 0},   {159, kParamFilterType, 0},   {159, kParamFilterCutoff, 11000},
  {159, kParamFilterReso, 10},   {159, kParamFilterEnv, 32},   {159, kParamFiltAttack, 6},
  {159, kParamFiltDecay, 900},   {159, kParamFiltSustain, 38},   {159, kParamFiltRelease, 1400},
  {159, kParamAmpAttack, 6},   {159, kParamAmpDecay, 1200},   {159, kParamAmpSustain, 62},
  {159, kParamAmpRelease, 1400},   {159, kParamUnisonVoices, 2},   {159, kParamUnisonDetune, 17},
  {159, kParamStereoSpread, 62},   {159, kParamTapeSat, 12},   {159, kParamTapeMix, 28},
  {159, kParamFilterKeyTrack, 52},   {159, kParamIns1Algo, 2},   {159, kParamIns1A, 20},
  {159, kParamIns1B, 28},   {159, kParamIns1C, 36},   {159, kParamIns1Mix, 20},
  {159, kParamMstAlgo, 2},   {159, kParamMstA, 24},   {159, kParamMstB, 18},
  {159, kParamMstC, 30},   {159, kParamMstMix, 10},
  {160, kParamEngine, 0},   {160, kParamPolyMode, 0},   {160, kParamOsc1Shape, 18},
  {160, kParamOsc1Level, 88},   {160, kParamOsc2Level, 0},   {160, kParamSubLevel, 45},
  {160, kParamNoiseLevel, 0},   {160, kParamFilterType, 0},   {160, kParamFilterCutoff, 9800},
  {160, kParamFilterReso, 14},   {160, kParamFilterEnv, 32},   {160, kParamFiltAttack, 6},
  {160, kParamFiltDecay, 900},   {160, kParamFiltSustain, 38},   {160, kParamFiltRelease, 900},
  {160, kParamAmpAttack, 4},   {160, kParamAmpDecay, 1200},   {160, kParamAmpSustain, 62},
  {160, kParamAmpRelease, 900},   {160, kParamUnisonVoices, 2},   {160, kParamUnisonDetune, 17},
  {160, kParamStereoSpread, 42},   {160, kParamTapeSat, 15},   {160, kParamTapeMix, 28},
  {160, kParamFmAmount, 22},   {160, kParamVelToAmp, 56},   {160, kParamIns1Algo, 2},
  {160, kParamIns1A, 14},   {160, kParamIns1B, 38},   {160, kParamIns1C, 44},
  {160, kParamIns1Mix, 26},   {160, kParamMstAlgo, 2},   {160, kParamMstA, 24},
  {160, kParamMstB, 18},   {160, kParamMstC, 30},   {160, kParamMstMix, 10},
  {161, kParamEngine, 0},   {161, kParamPolyMode, 0},   {161, kParamOsc1Shape, 44},
  {161, kParamOsc1Level, 84},   {161, kParamOsc2Shape, 46},   {161, kParamOsc2Pitch, -12},
  {161, kParamOsc2Level, 64},   {161, kParamSubLevel, 20},   {161, kParamFilterType, 0},
  {161, kParamFilterCutoff, 7400},   {161, kParamFilterReso, 22},   {161, kParamFilterEnv, 58},
  {161, kParamFiltAttack, 4},   {161, kParamFiltDecay, 208},   {161, kParamFiltSustain, 4.8},
  {161, kParamFiltRelease, 210},   {161, kParamAmpAttack, 3},   {161, kParamAmpDecay, 260},
  {161, kParamAmpSustain, 8},   {161, kParamAmpRelease, 300},   {161, kParamUnisonVoices, 1},
  {161, kParamUnisonDetune, 10},   {161, kParamStereoSpread, 30},   {161, kParamStereoDelay, 30},
  {161, kParamTapeSat, 20},   {161, kParamTapeMix, 32},   {161, kParamOsc1PW, 62},
  {161, kParamIns1Algo, 4},   {161, kParamIns1A, 22},   {161, kParamIns1B, 32},
  {161, kParamIns1C, 14},   {161, kParamIns1Mix, 14},   {161, kParamMstAlgo, 2},
  {161, kParamMstA, 24},   {161, kParamMstB, 18},   {161, kParamMstC, 30},
  {161, kParamMstMix, 10},
  {162, kParamEngine, 0},   {162, kParamPolyMode, 0},   {162, kParamOsc1Shape, 48},
  {162, kParamOsc1Level, 88},   {162, kParamOsc2Level, 0},   {162, kParamSubLevel, 45},
  {162, kParamNoiseLevel, 0},   {162, kParamFilterType, 0},   {162, kParamFilterCutoff, 8600},
  {162, kParamFilterReso, 18},   {162, kParamFilterEnv, 52},   {162, kParamFiltAttack, 6},
  {162, kParamFiltDecay, 900},   {162, kParamFiltSustain, 38},   {162, kParamFiltRelease, 420},
  {162, kParamAmpAttack, 2},   {162, kParamAmpDecay, 1200},   {162, kParamAmpSustain, 62},
  {162, kParamAmpRelease, 420},   {162, kParamUnisonVoices, 2},   {162, kParamUnisonDetune, 17},
  {162, kParamStereoSpread, 42},   {162, kParamTapeSat, 22},   {162, kParamTapeMix, 28},
  {162, kParamOsc1PW, 64},   {162, kParamIns1Algo, 2},   {162, kParamIns1A, 20},
  {162, kParamIns1B, 28},   {162, kParamIns1C, 36},   {162, kParamIns1Mix, 20},
  {162, kParamMstAlgo, 2},   {162, kParamMstA, 24},   {162, kParamMstB, 18},
  {162, kParamMstC, 30},   {162, kParamMstMix, 10},
  {163, kParamEngine, 0},   {163, kParamPolyMode, 0},   {163, kParamOsc1Shape, 30},
  {163, kParamOsc1Level, 58},   {163, kParamOsc2Level, 0},   {163, kParamOsc3Enable, 1},
  {163, kParamOsc3Source, 1},   {163, kParamOsc3Morph, 2.4},   {163, kParamOsc3Level, 86},
  {163, kParamOsc3Pitch, 12},   {163, kParamSubLevel, 26},   {163, kParamFilterType, 0},
  {163, kParamFilterCutoff, 10200},   {163, kParamFilterReso, 12},   {163, kParamFilterEnv, 28},
  {163, kParamFiltAttack, 10},   {163, kParamFiltDecay, 1400},   {163, kParamFiltSustain, 44},
  {163, kParamFiltRelease, 1800},   {163, kParamAmpAttack, 10},   {163, kParamAmpDecay, 1600},
  {163, kParamAmpSustain, 70},   {163, kParamAmpRelease, 1800},   {163, kParamUnisonVoices, 2},
  {163, kParamUnisonDetune, 12},   {163, kParamStereoSpread, 68},   {163, kParamTapeSat, 18},
  {163, kParamTapeMix, 30},   {163, kParamIns1Algo, 2},   {163, kParamIns1A, 14},
  {163, kParamIns1B, 38},   {163, kParamIns1C, 44},   {163, kParamIns1Mix, 26},
  {163, kParamMstAlgo, 2},   {163, kParamMstA, 24},   {163, kParamMstB, 18},
  {163, kParamMstC, 30},   {163, kParamMstMix, 10},
  {164, kParamEngine, 0},   {164, kParamPolyMode, 0},   {164, kParamOsc1Shape, 20},
  {164, kParamOsc1Level, 72},   {164, kParamOsc2Shape, 22},   {164, kParamOsc2Pitch, 24},
  {164, kParamOsc2Level, 48},   {164, kParamFmAmount, 34},   {164, kParamSubLevel, 18},
  {164, kParamNoiseLevel, 4},   {164, kParamFilterType, 0},   {164, kParamFilterCutoff, 8200},
  {164, kParamFilterReso, 16},   {164, kParamFilterEnv, 24},   {164, kParamFiltAttack, 3},
  {164, kParamFiltDecay, 1100},   {164, kParamFiltSustain, 11},   {164, kParamFiltRelease, 1200},
  {164, kParamAmpAttack, 2},   {164, kParamAmpDecay, 1100},   {164, kParamAmpSustain, 22},
  {164, kParamAmpRelease, 1200},   {164, kParamVelToAmp, 62},   {164, kParamVelToFilter, 30},
  {164, kParamStereoSpread, 44},   {164, kParamStereoDelay, 22},   {164, kParamTapeSat, 14},
  {164, kParamTapeMix, 26},   {164, kParamIns1Algo, 4},   {164, kParamIns1A, 22},
  {164, kParamIns1B, 32},   {164, kParamIns1C, 14},   {164, kParamIns1Mix, 14},
  {164, kParamMstAlgo, 2},   {164, kParamMstA, 24},   {164, kParamMstB, 18},
  {164, kParamMstC, 30},   {164, kParamMstMix, 10},
  {165, kParamEngine, 0},   {165, kParamPolyMode, 0},   {165, kParamOsc1Shape, 12},
  {165, kParamOsc1Level, 88},   {165, kParamOsc2Level, 0},   {165, kParamSubLevel, 22},
  {165, kParamNoiseLevel, 0},   {165, kParamFilterType, 0},   {165, kParamFilterCutoff, 12500},
  {165, kParamFilterReso, 8},   {165, kParamFilterEnv, 32},   {165, kParamFiltAttack, 6},
  {165, kParamFiltDecay, 900},   {165, kParamFiltSustain, 38},   {165, kParamFiltRelease, 2200},
  {165, kParamAmpAttack, 8},   {165, kParamAmpDecay, 1200},   {165, kParamAmpSustain, 62},
  {165, kParamAmpRelease, 2200},   {165, kParamUnisonVoices, 2},   {165, kParamUnisonDetune, 17},
  {165, kParamStereoSpread, 42},   {165, kParamTapeSat, 10},   {165, kParamTapeMix, 28},
  {165, kParamIns1Algo, 2},   {165, kParamIns1A, 20},   {165, kParamIns1B, 28},
  {165, kParamIns1C, 36},   {165, kParamIns1Mix, 20},   {165, kParamMstAlgo, 2},
  {165, kParamMstA, 24},   {165, kParamMstB, 18},   {165, kParamMstC, 30},
  {165, kParamMstMix, 10},
  {166, kParamEngine, 0},   {166, kParamPolyMode, 0},   {166, kParamOsc1Shape, 44},
  {166, kParamOsc1Level, 84},   {166, kParamOsc2Shape, 46},   {166, kParamOsc2Pitch, 7},
  {166, kParamOsc2Level, 64},   {166, kParamSubLevel, 20},   {166, kParamFilterType, 0},
  {166, kParamFilterCutoff, 6200},   {166, kParamFilterReso, 30},   {166, kParamFilterEnv, 64},
  {166, kParamFiltAttack, 4},   {166, kParamFiltDecay, 176},   {166, kParamFiltSustain, 3.5999999999999996},
  {166, kParamFiltRelease, 168},   {166, kParamAmpAttack, 2},   {166, kParamAmpDecay, 220},
  {166, kParamAmpSustain, 6},   {166, kParamAmpRelease, 240},   {166, kParamUnisonVoices, 1},
  {166, kParamUnisonDetune, 10},   {166, kParamStereoSpread, 30},   {166, kParamStereoDelay, 18},
  {166, kParamTapeSat, 20},   {166, kParamTapeMix, 32},   {166, kParamFmAmount, 14},
  {166, kParamIns1Algo, 2},   {166, kParamIns1A, 14},   {166, kParamIns1B, 38},
  {166, kParamIns1C, 44},   {166, kParamIns1Mix, 26},   {166, kParamMstAlgo, 2},
  {166, kParamMstA, 24},   {166, kParamMstB, 18},   {166, kParamMstC, 30},
  {166, kParamMstMix, 10},
  {167, kParamEngine, 0},   {167, kParamPolyMode, 0},   {167, kParamOsc1Shape, 16},
  {167, kParamOsc1Level, 88},   {167, kParamOsc2Level, 0},   {167, kParamSubLevel, 45},
  {167, kParamNoiseLevel, 0},   {167, kParamFilterType, 0},   {167, kParamFilterCutoff, 7000},
  {167, kParamFilterReso, 12},   {167, kParamFilterEnv, 32},   {167, kParamFiltAttack, 6},
  {167, kParamFiltDecay, 900},   {167, kParamFiltSustain, 38},   {167, kParamFiltRelease, 1600},
  {167, kParamAmpAttack, 6},   {167, kParamAmpDecay, 1200},   {167, kParamAmpSustain, 62},
  {167, kParamAmpRelease, 1600},   {167, kParamUnisonVoices, 2},   {167, kParamUnisonDetune, 17},
  {167, kParamStereoSpread, 42},   {167, kParamTapeSat, 15},   {167, kParamTapeMix, 28},
  {167, kParamFmAmount, 28},   {167, kParamVelToAmp, 64},   {167, kParamIns1Algo, 4},
  {167, kParamIns1A, 22},   {167, kParamIns1B, 32},   {167, kParamIns1C, 14},
  {167, kParamIns1Mix, 14},   {167, kParamMstAlgo, 2},   {167, kParamMstA, 24},
  {167, kParamMstB, 18},   {167, kParamMstC, 30},   {167, kParamMstMix, 10},
  {168, kParamEngine, 0},   {168, kParamPolyMode, 0},   {168, kParamOsc1Shape, 42},
  {168, kParamOsc1Level, 82},   {168, kParamOsc2Shape, 40},   {168, kParamOsc2Pitch, 0},
  {168, kParamOsc2Level, 76},   {168, kParamSubLevel, 32},   {168, kParamFilterType, 0},
  {168, kParamFilterCutoff, 6600},   {168, kParamFilterReso, 20},   {168, kParamFilterEnv, 40},
  {168, kParamFiltAttack, 8},   {168, kParamFiltDecay, 1100},   {168, kParamFiltSustain, 34},
  {168, kParamFiltRelease, 2400},   {168, kParamAmpAttack, 320},   {168, kParamAmpDecay, 1400},
  {168, kParamAmpSustain, 66},   {168, kParamAmpRelease, 2400},   {168, kParamUnisonVoices, 4},
  {168, kParamUnisonDetune, 30},   {168, kParamStereoSpread, 74},   {168, kParamStereoDelay, 0},
  {168, kParamTapeSat, 12},   {168, kParamTapeMix, 24},   {168, kParamLfoRate, 0.42},
  {168, kParamLfoAmount, 22},   {168, kParamLfoDest, 2},   {168, kParamIns1Algo, 2},
  {168, kParamIns1A, 20},   {168, kParamIns1B, 28},   {168, kParamIns1C, 36},
  {168, kParamIns1Mix, 20},   {168, kParamMstAlgo, 2},   {168, kParamMstA, 24},
  {168, kParamMstB, 18},   {168, kParamMstC, 30},   {168, kParamMstMix, 10},
  {169, kParamEngine, 0},   {169, kParamPolyMode, 0},   {169, kParamOsc1Shape, 42},
  {169, kParamOsc1Level, 82},   {169, kParamOsc2Shape, 40},   {169, kParamOsc2Pitch, 0},
  {169, kParamOsc2Level, 76},   {169, kParamSubLevel, 32},   {169, kParamFilterType, 0},
  {169, kParamFilterCutoff, 7800},   {169, kParamFilterReso, 26},   {169, kParamFilterEnv, 40},
  {169, kParamFiltAttack, 8},   {169, kParamFiltDecay, 1100},   {169, kParamFiltSustain, 34},
  {169, kParamFiltRelease, 900},   {169, kParamAmpAttack, 6},   {169, kParamAmpDecay, 1400},
  {169, kParamAmpSustain, 66},   {169, kParamAmpRelease, 900},   {169, kParamUnisonVoices, 4},
  {169, kParamUnisonDetune, 26},   {169, kParamStereoSpread, 58},   {169, kParamStereoDelay, 0},
  {169, kParamTapeSat, 12},   {169, kParamTapeMix, 24},   {169, kParamIns1Algo, 2},
  {169, kParamIns1A, 14},   {169, kParamIns1B, 38},   {169, kParamIns1C, 44},
  {169, kParamIns1Mix, 26},   {169, kParamMstAlgo, 2},   {169, kParamMstA, 24},
  {169, kParamMstB, 18},   {169, kParamMstC, 30},   {169, kParamMstMix, 10},
  {170, kParamEngine, 0},   {170, kParamPolyMode, 0},   {170, kParamOsc1Shape, 42},
  {170, kParamOsc1Level, 82},   {170, kParamOsc2Shape, 40},   {170, kParamOsc2Pitch, 0},
  {170, kParamOsc2Level, 76},   {170, kParamSubLevel, 32},   {170, kParamFilterType, 0},
  {170, kParamFilterCutoff, 3200},   {170, kParamFilterReso, 44},   {170, kParamFilterEnv, 72},
  {170, kParamFiltAttack, 8},   {170, kParamFiltDecay, 1500},   {170, kParamFiltSustain, 34},
  {170, kParamFiltRelease, 700},   {170, kParamAmpAttack, 4},   {170, kParamAmpDecay, 1400},
  {170, kParamAmpSustain, 66},   {170, kParamAmpRelease, 700},   {170, kParamUnisonVoices, 4},
  {170, kParamUnisonDetune, 22},   {170, kParamStereoSpread, 58},   {170, kParamStereoDelay, 0},
  {170, kParamTapeSat, 12},   {170, kParamTapeMix, 24},   {170, kParamLfoRate, 0.9},
  {170, kParamLfoDest, 1},   {170, kParamLfoAmount, 26},   {170, kParamIns1Algo, 4},
  {170, kParamIns1A, 22},   {170, kParamIns1B, 32},   {170, kParamIns1C, 14},
  {170, kParamIns1Mix, 14},   {170, kParamMstAlgo, 2},   {170, kParamMstA, 24},
  {170, kParamMstB, 18},   {170, kParamMstC, 30},   {170, kParamMstMix, 10},
  {171, kParamEngine, 0},   {171, kParamPolyMode, 0},   {171, kParamOsc1Shape, 44},
  {171, kParamOsc1Level, 84},   {171, kParamOsc2Shape, 46},   {171, kParamOsc2Pitch, -12},
  {171, kParamOsc2Level, 64},   {171, kParamSubLevel, 20},   {171, kParamFilterType, 0},
  {171, kParamFilterCutoff, 5400},   {171, kParamFilterReso, 38},   {171, kParamFilterEnv, 48},
  {171, kParamFiltAttack, 4},   {171, kParamFiltDecay, 240},   {171, kParamFiltSustain, 6},
  {171, kParamFiltRelease, 266},   {171, kParamAmpAttack, 2},   {171, kParamAmpDecay, 300},
  {171, kParamAmpSustain, 10},   {171, kParamAmpRelease, 380},   {171, kParamUnisonVoices, 1},
  {171, kParamUnisonDetune, 10},   {171, kParamStereoSpread, 30},   {171, kParamStereoDelay, 18},
  {171, kParamTapeSat, 36},   {171, kParamTapeMix, 32},   {171, kParamFilterDrive, 24},
  {171, kParamFmAmount, 10},   {171, kParamIns1Algo, 2},   {171, kParamIns1A, 20},
  {171, kParamIns1B, 28},   {171, kParamIns1C, 36},   {171, kParamIns1Mix, 20},
  {171, kParamMstAlgo, 2},   {171, kParamMstA, 24},   {171, kParamMstB, 18},
  {171, kParamMstC, 30},   {171, kParamMstMix, 10},
  {172, kParamEngine, 0},   {172, kParamPolyMode, 0},   {172, kParamOsc1Shape, 40},
  {172, kParamOsc1Level, 88},   {172, kParamOsc2Level, 0},   {172, kParamSubLevel, 45},
  {172, kParamNoiseLevel, 0},   {172, kParamFilterType, 0},   {172, kParamFilterCutoff, 8800},
  {172, kParamFilterReso, 16},   {172, kParamFilterEnv, 32},   {172, kParamFiltAttack, 6},
  {172, kParamFiltDecay, 900},   {172, kParamFiltSustain, 38},   {172, kParamFiltRelease, 1400},
  {172, kParamAmpAttack, 8},   {172, kParamAmpDecay, 1200},   {172, kParamAmpSustain, 62},
  {172, kParamAmpRelease, 1400},   {172, kParamUnisonVoices, 2},   {172, kParamUnisonDetune, 17},
  {172, kParamStereoSpread, 42},   {172, kParamTapeSat, 18},   {172, kParamTapeMix, 28},
  {172, kParamIns1Algo, 2},   {172, kParamIns1A, 14},   {172, kParamIns1B, 38},
  {172, kParamIns1C, 44},   {172, kParamIns1Mix, 26},   {172, kParamMstAlgo, 2},
  {172, kParamMstA, 24},   {172, kParamMstB, 18},   {172, kParamMstC, 30},
  {172, kParamMstMix, 10},
  {173, kParamEngine, 0},   {173, kParamPolyMode, 0},   {173, kParamOsc1Shape, 36},
  {173, kParamOsc1Level, 90},   {173, kParamOsc2Shape, 34},   {173, kParamOsc2Pitch, -12},
  {173, kParamOsc2Level, 58},   {173, kParamSubLevel, 38},   {173, kParamNoiseLevel, 6},
  {173, kParamFilterType, 0},   {173, kParamFilterCutoff, 1900},   {173, kParamFilterReso, 40},
  {173, kParamFilterDrive, 32},   {173, kParamFilterEnv, 44},   {173, kParamFiltAttack, 8},
  {173, kParamFiltDecay, 500},   {173, kParamFiltSustain, 21},   {173, kParamFiltRelease, 400},
  {173, kParamAmpAttack, 3},   {173, kParamAmpDecay, 500},   {173, kParamAmpSustain, 30},
  {173, kParamAmpRelease, 400},   {173, kParamUnisonVoices, 2},   {173, kParamUnisonDetune, 14},
  {173, kParamStereoSpread, 34},   {173, kParamTapeSat, 30},   {173, kParamTapeMix, 36},
  {173, kParamIns1Algo, 4},   {173, kParamIns1A, 22},   {173, kParamIns1B, 32},
  {173, kParamIns1C, 14},   {173, kParamIns1Mix, 14},   {173, kParamMstAlgo, 2},
  {173, kParamMstA, 24},   {173, kParamMstB, 18},   {173, kParamMstC, 30},
  {173, kParamMstMix, 10},
  {174, kParamEngine, 0},   {174, kParamPolyMode, 0},   {174, kParamOsc1Shape, 42},
  {174, kParamOsc1Level, 82},   {174, kParamOsc2Shape, 40},   {174, kParamOsc2Pitch, 0},
  {174, kParamOsc2Level, 76},   {174, kParamSubLevel, 32},   {174, kParamFilterType, 0},
  {174, kParamFilterCutoff, 6400},   {174, kParamFilterReso, 34},   {174, kParamFilterEnv, 40},
  {174, kParamFiltAttack, 8},   {174, kParamFiltDecay, 1100},   {174, kParamFiltSustain, 34},
  {174, kParamFiltRelease, 1200},   {174, kParamAmpAttack, 12},   {174, kParamAmpDecay, 1400},
  {174, kParamAmpSustain, 66},   {174, kParamAmpRelease, 1200},   {174, kParamUnisonVoices, 4},
  {174, kParamUnisonDetune, 34},   {174, kParamStereoSpread, 70},   {174, kParamStereoDelay, 0},
  {174, kParamTapeSat, 12},   {174, kParamTapeMix, 24},   {174, kParamIns1Algo, 2},
  {174, kParamIns1A, 20},   {174, kParamIns1B, 28},   {174, kParamIns1C, 36},
  {174, kParamIns1Mix, 20},   {174, kParamMstAlgo, 2},   {174, kParamMstA, 24},
  {174, kParamMstB, 18},   {174, kParamMstC, 30},   {174, kParamMstMix, 10},
  {175, kParamEngine, 0},   {175, kParamPolyMode, 0},   {175, kParamOsc1Shape, 30},
  {175, kParamOsc1Level, 58},   {175, kParamOsc2Level, 0},   {175, kParamOsc3Enable, 1},
  {175, kParamOsc3Source, 1},   {175, kParamOsc3Morph, 4.6},   {175, kParamOsc3Level, 86},
  {175, kParamOsc3Pitch, 0},   {175, kParamSubLevel, 26},   {175, kParamFilterType, 0},
  {175, kParamFilterCutoff, 7200},   {175, kParamFilterReso, 18},   {175, kParamFilterEnv, 28},
  {175, kParamFiltAttack, 10},   {175, kParamFiltDecay, 1400},   {175, kParamFiltSustain, 44},
  {175, kParamFiltRelease, 1800},   {175, kParamAmpAttack, 60},   {175, kParamAmpDecay, 1600},
  {175, kParamAmpSustain, 70},   {175, kParamAmpRelease, 1800},   {175, kParamUnisonVoices, 4},
  {175, kParamUnisonDetune, 20},   {175, kParamStereoSpread, 52},   {175, kParamTapeSat, 18},
  {175, kParamTapeMix, 30},   {175, kParamIns1Algo, 2},   {175, kParamIns1A, 14},
  {175, kParamIns1B, 38},   {175, kParamIns1C, 44},   {175, kParamIns1Mix, 26},
  {175, kParamMstAlgo, 2},   {175, kParamMstA, 24},   {175, kParamMstB, 18},
  {175, kParamMstC, 30},   {175, kParamMstMix, 10},
  {176, kParamEngine, 0},   {176, kParamPolyMode, 0},   {176, kParamOsc1Shape, 44},
  {176, kParamOsc1Level, 84},   {176, kParamOsc2Shape, 46},   {176, kParamOsc2Pitch, -12},
  {176, kParamOsc2Level, 64},   {176, kParamSubLevel, 20},   {176, kParamFilterType, 0},
  {176, kParamFilterCutoff, 9200},   {176, kParamFilterReso, 24},   {176, kParamFilterEnv, 54},
  {176, kParamFiltAttack, 4},   {176, kParamFiltDecay, 272},   {176, kParamFiltSustain, 7.199999999999999},
  {176, kParamFiltRelease, 294},   {176, kParamAmpAttack, 2},   {176, kParamAmpDecay, 340},
  {176, kParamAmpSustain, 12},   {176, kParamAmpRelease, 420},   {176, kParamUnisonVoices, 1},
  {176, kParamUnisonDetune, 10},   {176, kParamStereoSpread, 30},   {176, kParamStereoDelay, 18},
  {176, kParamTapeSat, 28},   {176, kParamTapeMix, 32},   {176, kParamOsc1PW, 55},
  {176, kParamIns1Algo, 4},   {176, kParamIns1A, 22},   {176, kParamIns1B, 32},
  {176, kParamIns1C, 14},   {176, kParamIns1Mix, 14},   {176, kParamMstAlgo, 2},
  {176, kParamMstA, 24},   {176, kParamMstB, 18},   {176, kParamMstC, 30},
  {176, kParamMstMix, 10},
  {177, kParamEngine, 0},   {177, kParamPolyMode, 0},   {177, kParamOsc1Shape, 20},
  {177, kParamOsc1Level, 72},   {177, kParamOsc2Shape, 22},   {177, kParamOsc2Pitch, 24},
  {177, kParamOsc2Level, 48},   {177, kParamFmAmount, 58},   {177, kParamSubLevel, 18},
  {177, kParamNoiseLevel, 4},   {177, kParamFilterType, 0},   {177, kParamFilterCutoff, 9600},
  {177, kParamFilterReso, 20},   {177, kParamFilterEnv, 24},   {177, kParamFiltAttack, 3},
  {177, kParamFiltDecay, 380},   {177, kParamFiltSustain, 2},   {177, kParamFiltRelease, 340},
  {177, kParamAmpAttack, 1},   {177, kParamAmpDecay, 380},   {177, kParamAmpSustain, 4},
  {177, kParamAmpRelease, 340},   {177, kParamVelToAmp, 70},   {177, kParamVelToFilter, 30},
  {177, kParamStereoSpread, 44},   {177, kParamStereoDelay, 0},   {177, kParamTapeSat, 14},
  {177, kParamTapeMix, 26},   {177, kParamIns1Algo, 2},   {177, kParamIns1A, 20},
  {177, kParamIns1B, 28},   {177, kParamIns1C, 36},   {177, kParamIns1Mix, 20},
  {177, kParamMstAlgo, 2},   {177, kParamMstA, 24},   {177, kParamMstB, 18},
  {177, kParamMstC, 30},   {177, kParamMstMix, 10},
  {178, kParamEngine, 0},   {178, kParamPolyMode, 0},   {178, kParamOsc1Shape, 20},
  {178, kParamOsc1Level, 72},   {178, kParamOsc2Shape, 22},   {178, kParamOsc2Pitch, 24},
  {178, kParamOsc2Level, 48},   {178, kParamFmAmount, 72},   {178, kParamSubLevel, 18},
  {178, kParamNoiseLevel, 4},   {178, kParamFilterType, 0},   {178, kParamFilterCutoff, 11400},
  {178, kParamFilterReso, 14},   {178, kParamFilterEnv, 24},   {178, kParamFiltAttack, 3},
  {178, kParamFiltDecay, 1800},   {178, kParamFiltSustain, 6},   {178, kParamFiltRelease, 2200},
  {178, kParamAmpAttack, 2},   {178, kParamAmpDecay, 1800},   {178, kParamAmpSustain, 12},
  {178, kParamAmpRelease, 2200},   {178, kParamVelToAmp, 48},   {178, kParamVelToFilter, 30},
  {178, kParamStereoSpread, 44},   {178, kParamStereoDelay, 34},   {178, kParamTapeSat, 14},
  {178, kParamTapeMix, 26},   {178, kParamIns1Algo, 2},   {178, kParamIns1A, 14},
  {178, kParamIns1B, 38},   {178, kParamIns1C, 44},   {178, kParamIns1Mix, 26},
  {178, kParamMstAlgo, 2},   {178, kParamMstA, 24},   {178, kParamMstB, 18},
  {178, kParamMstC, 30},   {178, kParamMstMix, 10},
  {179, kParamEngine, 0},   {179, kParamPolyMode, 0},   {179, kParamOsc1Shape, 52},
  {179, kParamOsc1Level, 88},   {179, kParamOsc2Level, 0},   {179, kParamSubLevel, 45},
  {179, kParamNoiseLevel, 0},   {179, kParamFilterType, 0},   {179, kParamFilterCutoff, 8400},
  {179, kParamFilterReso, 26},   {179, kParamFilterEnv, 62},   {179, kParamFiltAttack, 6},
  {179, kParamFiltDecay, 900},   {179, kParamFiltSustain, 38},   {179, kParamFiltRelease, 300},
  {179, kParamAmpAttack, 1},   {179, kParamAmpDecay, 1200},   {179, kParamAmpSustain, 62},
  {179, kParamAmpRelease, 300},   {179, kParamUnisonVoices, 2},   {179, kParamUnisonDetune, 17},
  {179, kParamStereoSpread, 42},   {179, kParamTapeSat, 15},   {179, kParamTapeMix, 28},
  {179, kParamOsc1PW, 70},   {179, kParamIns1Algo, 4},   {179, kParamIns1A, 22},
  {179, kParamIns1B, 32},   {179, kParamIns1C, 14},   {179, kParamIns1Mix, 14},
  {179, kParamMstAlgo, 2},   {179, kParamMstA, 24},   {179, kParamMstB, 18},
  {179, kParamMstC, 30},   {179, kParamMstMix, 10},
  {180, kParamEngine, 0},   {180, kParamPolyMode, 0},   {180, kParamOsc1Shape, 44},
  {180, kParamOsc1Level, 84},   {180, kParamOsc2Shape, 46},   {180, kParamOsc2Pitch, -12},
  {180, kParamOsc2Level, 64},   {180, kParamSubLevel, 20},   {180, kParamFilterType, 0},
  {180, kParamFilterCutoff, 5800},   {180, kParamFilterReso, 32},   {180, kParamFilterEnv, 70},
  {180, kParamFiltAttack, 4},   {180, kParamFiltDecay, 160},   {180, kParamFiltSustain, 2.4},
  {180, kParamFiltRelease, 154},   {180, kParamAmpAttack, 1},   {180, kParamAmpDecay, 200},
  {180, kParamAmpSustain, 4},   {180, kParamAmpRelease, 220},   {180, kParamUnisonVoices, 1},
  {180, kParamUnisonDetune, 10},   {180, kParamStereoSpread, 30},   {180, kParamStereoDelay, 0},
  {180, kParamTapeSat, 20},   {180, kParamTapeMix, 18},   {180, kParamIns1Algo, 2},
  {180, kParamIns1A, 20},   {180, kParamIns1B, 28},   {180, kParamIns1C, 36},
  {180, kParamIns1Mix, 20},   {180, kParamMstAlgo, 2},   {180, kParamMstA, 24},
  {180, kParamMstB, 18},   {180, kParamMstC, 30},   {180, kParamMstMix, 10},
  {181, kParamEngine, 0},   {181, kParamPolyMode, 0},   {181, kParamOsc1Shape, 30},
  {181, kParamOsc1Level, 58},   {181, kParamOsc2Level, 0},   {181, kParamOsc3Enable, 1},
  {181, kParamOsc3Source, 1},   {181, kParamOsc3Morph, 5.8},   {181, kParamOsc3Level, 86},
  {181, kParamOsc3Pitch, 19},   {181, kParamSubLevel, 26},   {181, kParamFilterType, 0},
  {181, kParamFilterCutoff, 9000},   {181, kParamFilterReso, 16},   {181, kParamFilterEnv, 28},
  {181, kParamFiltAttack, 10},   {181, kParamFiltDecay, 1400},   {181, kParamFiltSustain, 44},
  {181, kParamFiltRelease, 1600},   {181, kParamAmpAttack, 2},   {181, kParamAmpDecay, 1600},
  {181, kParamAmpSustain, 70},   {181, kParamAmpRelease, 1600},   {181, kParamUnisonVoices, 2},
  {181, kParamUnisonDetune, 12},   {181, kParamStereoSpread, 52},   {181, kParamTapeSat, 18},
  {181, kParamTapeMix, 30},   {181, kParamVelToAmp, 64},   {181, kParamIns1Algo, 2},
  {181, kParamIns1A, 14},   {181, kParamIns1B, 38},   {181, kParamIns1C, 44},
  {181, kParamIns1Mix, 26},   {181, kParamMstAlgo, 2},   {181, kParamMstA, 24},
  {181, kParamMstB, 18},   {181, kParamMstC, 30},   {181, kParamMstMix, 10},
  {182, kParamEngine, 0},   {182, kParamPolyMode, 0},   {182, kParamOsc1Shape, 20},
  {182, kParamOsc1Level, 72},   {182, kParamOsc2Shape, 22},   {182, kParamOsc2Pitch, 24},
  {182, kParamOsc2Level, 48},   {182, kParamFmAmount, 44},   {182, kParamSubLevel, 40},
  {182, kParamNoiseLevel, 4},   {182, kParamFilterType, 0},   {182, kParamFilterCutoff, 8000},
  {182, kParamFilterReso, 12},   {182, kParamFilterEnv, 24},   {182, kParamFiltAttack, 3},
  {182, kParamFiltDecay, 600},   {182, kParamFiltSustain, 0},   {182, kParamFiltRelease, 500},
  {182, kParamAmpAttack, 1},   {182, kParamAmpDecay, 600},   {182, kParamAmpSustain, 0},
  {182, kParamAmpRelease, 500},   {182, kParamVelToAmp, 48},   {182, kParamVelToFilter, 30},
  {182, kParamStereoSpread, 44},   {182, kParamStereoDelay, 22},   {182, kParamTapeSat, 14},
  {182, kParamTapeMix, 26},   {182, kParamIns1Algo, 4},   {182, kParamIns1A, 22},
  {182, kParamIns1B, 32},   {182, kParamIns1C, 14},   {182, kParamIns1Mix, 14},
  {182, kParamMstAlgo, 2},   {182, kParamMstA, 24},   {182, kParamMstB, 18},
  {182, kParamMstC, 30},   {182, kParamMstMix, 10},
  {183, kParamEngine, 0},   {183, kParamPolyMode, 0},   {183, kParamOsc1Shape, 22},
  {183, kParamOsc1Level, 88},   {183, kParamOsc2Level, 0},   {183, kParamSubLevel, 45},
  {183, kParamNoiseLevel, 0},   {183, kParamFilterType, 0},   {183, kParamFilterCutoff, 10400},
  {183, kParamFilterReso, 18},   {183, kParamFilterEnv, 56},   {183, kParamFiltAttack, 6},
  {183, kParamFiltDecay, 900},   {183, kParamFiltSustain, 38},   {183, kParamFiltRelease, 520},
  {183, kParamAmpAttack, 1},   {183, kParamAmpDecay, 1200},   {183, kParamAmpSustain, 62},
  {183, kParamAmpRelease, 520},   {183, kParamUnisonVoices, 2},   {183, kParamUnisonDetune, 17},
  {183, kParamStereoSpread, 42},   {183, kParamTapeSat, 15},   {183, kParamTapeMix, 28},
  {183, kParamFmAmount, 18},   {183, kParamIns1Algo, 2},   {183, kParamIns1A, 20},
  {183, kParamIns1B, 28},   {183, kParamIns1C, 36},   {183, kParamIns1Mix, 20},
  {183, kParamMstAlgo, 2},   {183, kParamMstA, 24},   {183, kParamMstB, 18},
  {183, kParamMstC, 30},   {183, kParamMstMix, 10},
  {184, kParamEngine, 0},   {184, kParamPolyMode, 0},   {184, kParamOsc1Shape, 42},
  {184, kParamOsc1Level, 82},   {184, kParamOsc2Shape, 40},   {184, kParamOsc2Pitch, 0},
  {184, kParamOsc2Level, 76},   {184, kParamSubLevel, 32},   {184, kParamFilterType, 0},
  {184, kParamFilterCutoff, 6800},   {184, kParamFilterReso, 42},   {184, kParamFilterEnv, 40},
  {184, kParamFiltAttack, 8},   {184, kParamFiltDecay, 1100},   {184, kParamFiltSustain, 34},
  {184, kParamFiltRelease, 600},   {184, kParamAmpAttack, 3},   {184, kParamAmpDecay, 1400},
  {184, kParamAmpSustain, 66},   {184, kParamAmpRelease, 600},   {184, kParamUnisonVoices, 4},
  {184, kParamUnisonDetune, 40},   {184, kParamStereoSpread, 58},   {184, kParamStereoDelay, 0},
  {184, kParamTapeSat, 44},   {184, kParamTapeMix, 48},   {184, kParamFilterDrive, 58},
  {184, kParamIns1Algo, 2},   {184, kParamIns1A, 14},   {184, kParamIns1B, 38},
  {184, kParamIns1C, 44},   {184, kParamIns1Mix, 26},   {184, kParamMstAlgo, 2},
  {184, kParamMstA, 24},   {184, kParamMstB, 18},   {184, kParamMstC, 30},
  {184, kParamMstMix, 10},
  {185, kParamEngine, 0},   {185, kParamPolyMode, 0},   {185, kParamOsc1Shape, 44},
  {185, kParamOsc1Level, 84},   {185, kParamOsc2Shape, 46},   {185, kParamOsc2Pitch, -12},
  {185, kParamOsc2Level, 64},   {185, kParamSubLevel, 20},   {185, kParamFilterType, 0},
  {185, kParamFilterCutoff, 5200},   {185, kParamFilterReso, 48},   {185, kParamFilterEnv, 48},
  {185, kParamFiltAttack, 4},   {185, kParamFiltDecay, 208},   {185, kParamFiltSustain, 4.8},
  {185, kParamFiltRelease, 210},   {185, kParamAmpAttack, 1},   {185, kParamAmpDecay, 260},
  {185, kParamAmpSustain, 8},   {185, kParamAmpRelease, 300},   {185, kParamUnisonVoices, 1},
  {185, kParamUnisonDetune, 10},   {185, kParamStereoSpread, 30},   {185, kParamStereoDelay, 18},
  {185, kParamTapeSat, 20},   {185, kParamTapeMix, 32},   {185, kParamFilterDrive, 62},
  {185, kParamIns1Algo, 4},   {185, kParamIns1A, 22},   {185, kParamIns1B, 32},
  {185, kParamIns1C, 14},   {185, kParamIns1Mix, 14},   {185, kParamMstAlgo, 2},
  {185, kParamMstA, 24},   {185, kParamMstB, 18},   {185, kParamMstC, 30},
  {185, kParamMstMix, 10},
  {186, kParamEngine, 0},   {186, kParamPolyMode, 0},   {186, kParamOsc1Shape, 42},
  {186, kParamOsc1Level, 82},   {186, kParamOsc2Shape, 40},   {186, kParamOsc2Pitch, 7},
  {186, kParamOsc2Level, 76},   {186, kParamSubLevel, 32},   {186, kParamFilterType, 0},
  {186, kParamFilterCutoff, 4600},   {186, kParamFilterReso, 52},   {186, kParamFilterEnv, 40},
  {186, kParamFiltAttack, 8},   {186, kParamFiltDecay, 1100},   {186, kParamFiltSustain, 34},
  {186, kParamFiltRelease, 500},   {186, kParamAmpAttack, 2},   {186, kParamAmpDecay, 1400},
  {186, kParamAmpSustain, 66},   {186, kParamAmpRelease, 500},   {186, kParamUnisonVoices, 4},
  {186, kParamUnisonDetune, 44},   {186, kParamStereoSpread, 58},   {186, kParamStereoDelay, 0},
  {186, kParamTapeSat, 12},   {186, kParamTapeMix, 24},   {186, kParamFilterDrive, 54},
  {186, kParamIns1Algo, 2},   {186, kParamIns1A, 20},   {186, kParamIns1B, 28},
  {186, kParamIns1C, 36},   {186, kParamIns1Mix, 20},   {186, kParamMstAlgo, 2},
  {186, kParamMstA, 24},   {186, kParamMstB, 18},   {186, kParamMstC, 30},
  {186, kParamMstMix, 10},
  {187, kParamEngine, 0},   {187, kParamPolyMode, 0},   {187, kParamOsc1Shape, 36},
  {187, kParamOsc1Level, 90},   {187, kParamOsc2Shape, 34},   {187, kParamOsc2Pitch, -12},
  {187, kParamOsc2Level, 58},   {187, kParamSubLevel, 38},   {187, kParamNoiseLevel, 6},
  {187, kParamFilterType, 0},   {187, kParamFilterCutoff, 2400},   {187, kParamFilterReso, 46},
  {187, kParamFilterDrive, 32},   {187, kParamFilterEnv, 44},   {187, kParamFiltAttack, 8},
  {187, kParamFiltDecay, 700},   {187, kParamFiltSustain, 28},   {187, kParamFiltRelease, 500},
  {187, kParamAmpAttack, 2},   {187, kParamAmpDecay, 700},   {187, kParamAmpSustain, 40},
  {187, kParamAmpRelease, 500},   {187, kParamUnisonVoices, 2},   {187, kParamUnisonDetune, 14},
  {187, kParamStereoSpread, 34},   {187, kParamTapeSat, 30},   {187, kParamTapeMix, 36},
  {187, kParamIns1Algo, 2},   {187, kParamIns1A, 14},   {187, kParamIns1B, 38},
  {187, kParamIns1C, 44},   {187, kParamIns1Mix, 26},   {187, kParamMstAlgo, 2},
  {187, kParamMstA, 24},   {187, kParamMstB, 18},   {187, kParamMstC, 30},
  {187, kParamMstMix, 10},
  {188, kParamEngine, 0},   {188, kParamPolyMode, 0},   {188, kParamOsc1Shape, 44},
  {188, kParamOsc1Level, 84},   {188, kParamOsc2Shape, 46},   {188, kParamOsc2Pitch, -12},
  {188, kParamOsc2Level, 64},   {188, kParamSubLevel, 20},   {188, kParamFilterType, 0},
  {188, kParamFilterCutoff, 7400},   {188, kParamFilterReso, 36},   {188, kParamFilterEnv, 48},
  {188, kParamFiltAttack, 4},   {188, kParamFiltDecay, 560},   {188, kParamFiltSustain, 12},
  {188, kParamFiltRelease, 489.99999999999994},   {188, kParamAmpAttack, 4},   {188, kParamAmpDecay, 700},
  {188, kParamAmpSustain, 20},   {188, kParamAmpRelease, 700},   {188, kParamUnisonVoices, 1},
  {188, kParamUnisonDetune, 10},   {188, kParamStereoSpread, 30},   {188, kParamStereoDelay, 18},
  {188, kParamTapeSat, 40},   {188, kParamTapeMix, 32},   {188, kParamFilterDrive, 50},
  {188, kParamIns1Algo, 4},   {188, kParamIns1A, 22},   {188, kParamIns1B, 32},
  {188, kParamIns1C, 14},   {188, kParamIns1Mix, 14},   {188, kParamMstAlgo, 2},
  {188, kParamMstA, 24},   {188, kParamMstB, 18},   {188, kParamMstC, 30},
  {188, kParamMstMix, 10},
  {189, kParamEngine, 0},   {189, kParamPolyMode, 0},   {189, kParamOsc1Shape, 30},
  {189, kParamOsc1Level, 58},   {189, kParamOsc2Level, 0},   {189, kParamOsc3Enable, 1},
  {189, kParamOsc3Source, 1},   {189, kParamOsc3Morph, 6.4},   {189, kParamOsc3Level, 86},
  {189, kParamOsc3Pitch, 0},   {189, kParamSubLevel, 26},   {189, kParamFilterType, 0},
  {189, kParamFilterCutoff, 6000},   {189, kParamFilterReso, 40},   {189, kParamFilterEnv, 28},
  {189, kParamFiltAttack, 10},   {189, kParamFiltDecay, 1400},   {189, kParamFiltSustain, 44},
  {189, kParamFiltRelease, 800},   {189, kParamAmpAttack, 3},   {189, kParamAmpDecay, 1600},
  {189, kParamAmpSustain, 70},   {189, kParamAmpRelease, 800},   {189, kParamUnisonVoices, 4},
  {189, kParamUnisonDetune, 12},   {189, kParamStereoSpread, 52},   {189, kParamTapeSat, 18},
  {189, kParamTapeMix, 30},   {189, kParamFilterDrive, 56},   {189, kParamIns1Algo, 2},
  {189, kParamIns1A, 20},   {189, kParamIns1B, 28},   {189, kParamIns1C, 36},
  {189, kParamIns1Mix, 20},   {189, kParamMstAlgo, 2},   {189, kParamMstA, 24},
  {189, kParamMstB, 18},   {189, kParamMstC, 30},   {189, kParamMstMix, 10},
  {190, kParamEngine, 0},   {190, kParamPolyMode, 0},   {190, kParamOsc1Shape, 38},
  {190, kParamOsc1Level, 88},   {190, kParamOsc2Level, 0},   {190, kParamSubLevel, 45},
  {190, kParamNoiseLevel, 0},   {190, kParamFilterType, 0},   {190, kParamFilterCutoff, 6400},
  {190, kParamFilterReso, 20},   {190, kParamFilterEnv, 32},   {190, kParamFiltAttack, 6},
  {190, kParamFiltDecay, 900},   {190, kParamFiltSustain, 38},   {190, kParamFiltRelease, 1800},
  {190, kParamAmpAttack, 12},   {190, kParamAmpDecay, 1200},   {190, kParamAmpSustain, 62},
  {190, kParamAmpRelease, 1800},   {190, kParamUnisonVoices, 2},   {190, kParamUnisonDetune, 17},
  {190, kParamStereoSpread, 42},   {190, kParamTapeSat, 34},   {190, kParamTapeMix, 44},
  {190, kParamTapeWow, 42},   {190, kParamTapeFlutter, 30},   {190, kParamTapeHiss, 12},
  {190, kParamIns1Algo, 2},   {190, kParamIns1A, 14},   {190, kParamIns1B, 38},
  {190, kParamIns1C, 44},   {190, kParamIns1Mix, 26},   {190, kParamMstAlgo, 2},
  {190, kParamMstA, 24},   {190, kParamMstB, 18},   {190, kParamMstC, 30},
  {190, kParamMstMix, 10},
  {191, kParamEngine, 0},   {191, kParamPolyMode, 0},   {191, kParamOsc1Shape, 42},
  {191, kParamOsc1Level, 82},   {191, kParamOsc2Shape, 40},   {191, kParamOsc2Pitch, 0},
  {191, kParamOsc2Level, 76},   {191, kParamSubLevel, 32},   {191, kParamFilterType, 0},
  {191, kParamFilterCutoff, 5200},   {191, kParamFilterReso, 24},   {191, kParamFilterEnv, 40},
  {191, kParamFiltAttack, 8},   {191, kParamFiltDecay, 1100},   {191, kParamFiltSustain, 34},
  {191, kParamFiltRelease, 2800},   {191, kParamAmpAttack, 340},   {191, kParamAmpDecay, 1400},
  {191, kParamAmpSustain, 66},   {191, kParamAmpRelease, 2800},   {191, kParamUnisonVoices, 4},
  {191, kParamUnisonDetune, 46},   {191, kParamStereoSpread, 80},   {191, kParamStereoDelay, 0},
  {191, kParamTapeSat, 12},   {191, kParamTapeMix, 24},   {191, kParamTapeWow, 48},
  {191, kParamTapeFlutter, 34},   {191, kParamIns1Algo, 4},   {191, kParamIns1A, 22},
  {191, kParamIns1B, 32},   {191, kParamIns1C, 14},   {191, kParamIns1Mix, 14},
  {191, kParamMstAlgo, 2},   {191, kParamMstA, 24},   {191, kParamMstB, 18},
  {191, kParamMstC, 30},   {191, kParamMstMix, 10},
  {192, kParamEngine, 0},   {192, kParamPolyMode, 0},   {192, kParamOsc1Shape, 44},
  {192, kParamOsc1Level, 84},   {192, kParamOsc2Shape, 46},   {192, kParamOsc2Pitch, -12},
  {192, kParamOsc2Level, 64},   {192, kParamSubLevel, 20},   {192, kParamFilterType, 0},
  {192, kParamFilterCutoff, 5600},   {192, kParamFilterReso, 30},   {192, kParamFilterEnv, 48},
  {192, kParamFiltAttack, 4},   {192, kParamFiltDecay, 320},   {192, kParamFiltSustain, 8.4},
  {192, kParamFiltRelease, 350},   {192, kParamAmpAttack, 4},   {192, kParamAmpDecay, 400},
  {192, kParamAmpSustain, 14},   {192, kParamAmpRelease, 500},   {192, kParamUnisonVoices, 1},
  {192, kParamUnisonDetune, 10},   {192, kParamStereoSpread, 30},   {192, kParamStereoDelay, 18},
  {192, kParamTapeSat, 20},   {192, kParamTapeMix, 40},   {192, kParamOsc1PW, 58},
  {192, kParamTapeWow, 38},   {192, kParamIns1Algo, 2},   {192, kParamIns1A, 20},
  {192, kParamIns1B, 28},   {192, kParamIns1C, 36},   {192, kParamIns1Mix, 20},
  {192, kParamMstAlgo, 2},   {192, kParamMstA, 24},   {192, kParamMstB, 18},
  {192, kParamMstC, 30},   {192, kParamMstMix, 10},
  {193, kParamEngine, 0},   {193, kParamPolyMode, 0},   {193, kParamOsc1Shape, 38},
  {193, kParamOsc1Level, 88},   {193, kParamOsc2Level, 0},   {193, kParamSubLevel, 45},
  {193, kParamNoiseLevel, 0},   {193, kParamFilterType, 0},   {193, kParamFilterCutoff, 6800},
  {193, kParamFilterReso, 14},   {193, kParamFilterEnv, 32},   {193, kParamFiltAttack, 6},
  {193, kParamFiltDecay, 900},   {193, kParamFiltSustain, 38},   {193, kParamFiltRelease, 2000},
  {193, kParamAmpAttack, 8},   {193, kParamAmpDecay, 1200},   {193, kParamAmpSustain, 62},
  {193, kParamAmpRelease, 2000},   {193, kParamUnisonVoices, 2},   {193, kParamUnisonDetune, 17},
  {193, kParamStereoSpread, 42},   {193, kParamTapeSat, 38},   {193, kParamTapeMix, 46},
  {193, kParamFmAmount, 30},   {193, kParamTapeFlutter, 36},   {193, kParamIns1Algo, 2},
  {193, kParamIns1A, 14},   {193, kParamIns1B, 38},   {193, kParamIns1C, 44},
  {193, kParamIns1Mix, 26},   {193, kParamMstAlgo, 2},   {193, kParamMstA, 24},
  {193, kParamMstB, 18},   {193, kParamMstC, 30},   {193, kParamMstMix, 10},
  {194, kParamEngine, 0},   {194, kParamPolyMode, 0},   {194, kParamOsc1Shape, 30},
  {194, kParamOsc1Level, 58},   {194, kParamOsc2Level, 0},   {194, kParamOsc3Enable, 1},
  {194, kParamOsc3Source, 1},   {194, kParamOsc3Morph, 2},   {194, kParamOsc3Level, 86},
  {194, kParamOsc3Pitch, 0},   {194, kParamSubLevel, 26},   {194, kParamFilterType, 0},
  {194, kParamFilterCutoff, 6000},   {194, kParamFilterReso, 22},   {194, kParamFilterEnv, 28},
  {194, kParamFiltAttack, 10},   {194, kParamFiltDecay, 1400},   {194, kParamFiltSustain, 44},
  {194, kParamFiltRelease, 2600},   {194, kParamAmpAttack, 220},   {194, kParamAmpDecay, 1600},
  {194, kParamAmpSustain, 70},   {194, kParamAmpRelease, 2600},   {194, kParamUnisonVoices, 2},
  {194, kParamUnisonDetune, 28},   {194, kParamStereoSpread, 52},   {194, kParamTapeSat, 18},
  {194, kParamTapeMix, 30},   {194, kParamTapeWow, 44},   {194, kParamIns1Algo, 4},
  {194, kParamIns1A, 22},   {194, kParamIns1B, 32},   {194, kParamIns1C, 14},
  {194, kParamIns1Mix, 14},   {194, kParamMstAlgo, 2},   {194, kParamMstA, 24},
  {194, kParamMstB, 18},   {194, kParamMstC, 30},   {194, kParamMstMix, 10},
  {195, kParamEngine, 0},   {195, kParamPolyMode, 0},   {195, kParamOsc1Shape, 36},
  {195, kParamOsc1Level, 90},   {195, kParamOsc2Shape, 34},   {195, kParamOsc2Pitch, -12},
  {195, kParamOsc2Level, 58},   {195, kParamSubLevel, 38},   {195, kParamNoiseLevel, 6},
  {195, kParamFilterType, 0},   {195, kParamFilterCutoff, 2600},   {195, kParamFilterReso, 28},
  {195, kParamFilterDrive, 32},   {195, kParamFilterEnv, 44},   {195, kParamFiltAttack, 8},
  {195, kParamFiltDecay, 1200},   {195, kParamFiltSustain, 33.599999999999994},   {195, kParamFiltRelease, 1000},
  {195, kParamAmpAttack, 10},   {195, kParamAmpDecay, 1200},   {195, kParamAmpSustain, 48},
  {195, kParamAmpRelease, 1000},   {195, kParamUnisonVoices, 2},   {195, kParamUnisonDetune, 14},
  {195, kParamStereoSpread, 34},   {195, kParamTapeSat, 30},   {195, kParamTapeMix, 36},
  {195, kParamIns1Algo, 2},   {195, kParamIns1A, 20},   {195, kParamIns1B, 28},
  {195, kParamIns1C, 36},   {195, kParamIns1Mix, 20},   {195, kParamMstAlgo, 2},
  {195, kParamMstA, 24},   {195, kParamMstB, 18},   {195, kParamMstC, 30},
  {195, kParamMstMix, 10},
  {196, kParamEngine, 0},   {196, kParamPolyMode, 0},   {196, kParamOsc1Shape, 20},
  {196, kParamOsc1Level, 72},   {196, kParamOsc2Shape, 22},   {196, kParamOsc2Pitch, 19},
  {196, kParamOsc2Level, 48},   {196, kParamFmAmount, 26},   {196, kParamSubLevel, 18},
  {196, kParamNoiseLevel, 4},   {196, kParamFilterType, 0},   {196, kParamFilterCutoff, 7200},
  {196, kParamFilterReso, 16},   {196, kParamFilterEnv, 24},   {196, kParamFiltAttack, 3},
  {196, kParamFiltDecay, 1500},   {196, kParamFiltSustain, 15},   {196, kParamFiltRelease, 1600},
  {196, kParamAmpAttack, 6},   {196, kParamAmpDecay, 1500},   {196, kParamAmpSustain, 30},
  {196, kParamAmpRelease, 1600},   {196, kParamVelToAmp, 48},   {196, kParamVelToFilter, 30},
  {196, kParamStereoSpread, 44},   {196, kParamStereoDelay, 22},   {196, kParamTapeSat, 14},
  {196, kParamTapeMix, 42},   {196, kParamTapeWow, 40},   {196, kParamTapeHiss, 10},
  {196, kParamIns1Algo, 2},   {196, kParamIns1A, 14},   {196, kParamIns1B, 38},
  {196, kParamIns1C, 44},   {196, kParamIns1Mix, 26},   {196, kParamMstAlgo, 2},
  {196, kParamMstA, 24},   {196, kParamMstB, 18},   {196, kParamMstC, 30},
  {196, kParamMstMix, 10},
  {197, kParamEngine, 0},   {197, kParamPolyMode, 0},   {197, kParamOsc1Shape, 42},
  {197, kParamOsc1Level, 82},   {197, kParamOsc2Shape, 40},   {197, kParamOsc2Pitch, 0},
  {197, kParamOsc2Level, 76},   {197, kParamSubLevel, 32},   {197, kParamFilterType, 0},
  {197, kParamFilterCutoff, 7200},   {197, kParamFilterReso, 28},   {197, kParamFilterEnv, 40},
  {197, kParamFiltAttack, 8},   {197, kParamFiltDecay, 1100},   {197, kParamFiltSustain, 34},
  {197, kParamFiltRelease, 1400},   {197, kParamAmpAttack, 20},   {197, kParamAmpDecay, 1400},
  {197, kParamAmpSustain, 66},   {197, kParamAmpRelease, 1400},   {197, kParamUnisonVoices, 6},
  {197, kParamUnisonDetune, 32},   {197, kParamStereoSpread, 88},   {197, kParamStereoDelay, 0},
  {197, kParamTapeSat, 12},   {197, kParamTapeMix, 24},   {197, kParamIns1Algo, 4},
  {197, kParamIns1A, 22},   {197, kParamIns1B, 32},   {197, kParamIns1C, 14},
  {197, kParamIns1Mix, 14},   {197, kParamMstAlgo, 2},   {197, kParamMstA, 24},
  {197, kParamMstB, 18},   {197, kParamMstC, 30},   {197, kParamMstMix, 10},
  {198, kParamEngine, 0},   {198, kParamPolyMode, 0},   {198, kParamOsc1Shape, 44},
  {198, kParamOsc1Level, 84},   {198, kParamOsc2Shape, 46},   {198, kParamOsc2Pitch, 0},
  {198, kParamOsc2Level, 64},   {198, kParamSubLevel, 20},   {198, kParamFilterType, 0},
  {198, kParamFilterCutoff, 4800},   {198, kParamFilterReso, 26},   {198, kParamFilterEnv, 56},
  {198, kParamFiltAttack, 50},   {198, kParamFiltDecay, 720},   {198, kParamFiltSustain, 32.4},
  {198, kParamFiltRelease, 560},   {198, kParamAmpAttack, 40},   {198, kParamAmpDecay, 900},
  {198, kParamAmpSustain, 54},   {198, kParamAmpRelease, 800},   {198, kParamUnisonVoices, 1},
  {198, kParamUnisonDetune, 10},   {198, kParamStereoSpread, 30},   {198, kParamStereoDelay, 18},
  {198, kParamTapeSat, 20},   {198, kParamTapeMix, 32},   {198, kParamIns1Algo, 2},
  {198, kParamIns1A, 20},   {198, kParamIns1B, 28},   {198, kParamIns1C, 36},
  {198, kParamIns1Mix, 20},   {198, kParamMstAlgo, 2},   {198, kParamMstA, 24},
  {198, kParamMstB, 18},   {198, kParamMstC, 30},   {198, kParamMstMix, 10},
  {199, kParamEngine, 0},   {199, kParamPolyMode, 0},   {199, kParamOsc1Shape, 38},
  {199, kParamOsc1Level, 88},   {199, kParamOsc2Level, 0},   {199, kParamSubLevel, 45},
  {199, kParamNoiseLevel, 0},   {199, kParamFilterType, 0},   {199, kParamFilterCutoff, 6000},
  {199, kParamFilterReso, 10},   {199, kParamFilterEnv, 32},   {199, kParamFiltAttack, 6},
  {199, kParamFiltDecay, 900},   {199, kParamFiltSustain, 38},   {199, kParamFiltRelease, 2400},
  {199, kParamAmpAttack, 300},   {199, kParamAmpDecay, 1200},   {199, kParamAmpSustain, 62},
  {199, kParamAmpRelease, 2400},   {199, kParamUnisonVoices, 1},   {199, kParamUnisonDetune, 17},
  {199, kParamStereoSpread, 42},   {199, kParamTapeSat, 16},   {199, kParamTapeMix, 28},
  {199, kParamIns1Algo, 2},   {199, kParamIns1A, 14},   {199, kParamIns1B, 38},
  {199, kParamIns1C, 44},   {199, kParamIns1Mix, 26},   {199, kParamMstAlgo, 2},
  {199, kParamMstA, 24},   {199, kParamMstB, 18},   {199, kParamMstC, 30},
  {199, kParamMstMix, 10},
  {200, kParamEngine, 0},   {200, kParamPolyMode, 0},   {200, kParamOsc1Shape, 36},
  {200, kParamOsc1Level, 90},   {200, kParamOsc2Shape, 34},   {200, kParamOsc2Pitch, -12},
  {200, kParamOsc2Level, 58},   {200, kParamSubLevel, 38},   {200, kParamNoiseLevel, 6},
  {200, kParamFilterType, 0},   {200, kParamFilterCutoff, 3000},   {200, kParamFilterReso, 38},
  {200, kParamFilterDrive, 32},   {200, kParamFilterEnv, 44},   {200, kParamFiltAttack, 8},
  {200, kParamFiltDecay, 1000},   {200, kParamFiltSustain, 36.4},   {200, kParamFiltRelease, 900},
  {200, kParamAmpAttack, 12},   {200, kParamAmpDecay, 1000},   {200, kParamAmpSustain, 52},
  {200, kParamAmpRelease, 900},   {200, kParamUnisonVoices, 2},   {200, kParamUnisonDetune, 14},
  {200, kParamStereoSpread, 34},   {200, kParamTapeSat, 30},   {200, kParamTapeMix, 36},
  {200, kParamIns1Algo, 4},   {200, kParamIns1A, 22},   {200, kParamIns1B, 32},
  {200, kParamIns1C, 14},   {200, kParamIns1Mix, 14},   {200, kParamMstAlgo, 2},
  {200, kParamMstA, 24},   {200, kParamMstB, 18},   {200, kParamMstC, 30},
  {200, kParamMstMix, 10},
  {201, kParamEngine, 0},   {201, kParamPolyMode, 0},   {201, kParamOsc1Shape, 30},
  {201, kParamOsc1Level, 58},   {201, kParamOsc2Level, 0},   {201, kParamOsc3Enable, 1},
  {201, kParamOsc3Source, 1},   {201, kParamOsc3Morph, 0.5},   {201, kParamOsc3Level, 86},
  {201, kParamOsc3Pitch, 0},   {201, kParamSubLevel, 26},   {201, kParamFilterType, 0},
  {201, kParamFilterCutoff, 7600},   {201, kParamFilterReso, 30},   {201, kParamFilterEnv, 28},
  {201, kParamFiltAttack, 10},   {201, kParamFiltDecay, 1400},   {201, kParamFiltSustain, 44},
  {201, kParamFiltRelease, 2200},   {201, kParamAmpAttack, 140},   {201, kParamAmpDecay, 1600},
  {201, kParamAmpSustain, 70},   {201, kParamAmpRelease, 2200},   {201, kParamUnisonVoices, 2},
  {201, kParamUnisonDetune, 12},   {201, kParamStereoSpread, 82},   {201, kParamTapeSat, 18},
  {201, kParamTapeMix, 30},   {201, kParamLfoRate, 0.28},   {201, kParamLfoDest, 16},
  {201, kParamLfoAmount, 64},   {201, kParamIns1Algo, 2},   {201, kParamIns1A, 20},
  {201, kParamIns1B, 28},   {201, kParamIns1C, 36},   {201, kParamIns1Mix, 20},
  {201, kParamMstAlgo, 2},   {201, kParamMstA, 24},   {201, kParamMstB, 18},
  {201, kParamMstC, 30},   {201, kParamMstMix, 10},
  {202, kParamEngine, 0},   {202, kParamPolyMode, 0},   {202, kParamOsc1Shape, 20},
  {202, kParamOsc1Level, 70},   {202, kParamOsc2Shape, 24},   {202, kParamOsc2Pitch, 19},
  {202, kParamOsc2Level, 44},   {202, kParamFmAmount, 34},   {202, kParamSubLevel, 22},
  {202, kParamNoiseLevel, 4},   {202, kParamFilterType, 0},   {202, kParamFilterCutoff, 8200},
  {202, kParamFilterReso, 16},   {202, kParamFilterEnv, 30},   {202, kParamFiltAttack, 2},
  {202, kParamFiltDecay, 979.9999999999999},   {202, kParamFiltSustain, 13},   {202, kParamFiltRelease, 1200},
  {202, kParamAmpAttack, 2},   {202, kParamAmpDecay, 1400},   {202, kParamAmpSustain, 26},
  {202, kParamAmpRelease, 1500},   {202, kParamVelToAmp, 64},   {202, kParamVelToFilter, 34},
  {202, kParamStereoSpread, 48},   {202, kParamStereoDelay, 16},   {202, kParamTapeSat, 20},
  {202, kParamTapeMix, 28},   {202, kParamIns1Algo, 2},   {202, kParamIns1A, 14},
  {202, kParamIns1B, 38},   {202, kParamIns1C, 44},   {202, kParamIns1Mix, 26},
  {202, kParamMstAlgo, 2},   {202, kParamMstA, 24},   {202, kParamMstB, 18},
  {202, kParamMstC, 30},   {202, kParamMstMix, 10},
  {203, kParamEngine, 0},   {203, kParamPolyMode, 0},   {203, kParamOsc1Shape, 20},
  {203, kParamOsc1Level, 70},   {203, kParamOsc2Shape, 24},   {203, kParamOsc2Pitch, 19},
  {203, kParamOsc2Level, 44},   {203, kParamFmAmount, 42},   {203, kParamSubLevel, 22},
  {203, kParamNoiseLevel, 4},   {203, kParamFilterType, 0},   {203, kParamFilterCutoff, 9800},
  {203, kParamFilterReso, 18},   {203, kParamFilterEnv, 30},   {203, kParamFiltAttack, 2},
  {203, kParamFiltDecay, 630},   {203, kParamFiltSustain, 10},   {203, kParamFiltRelease, 880},
  {203, kParamAmpAttack, 1},   {203, kParamAmpDecay, 900},   {203, kParamAmpSustain, 20},
  {203, kParamAmpRelease, 1100},   {203, kParamVelToAmp, 64},   {203, kParamVelToFilter, 34},
  {203, kParamStereoSpread, 48},   {203, kParamStereoDelay, 16},   {203, kParamTapeSat, 20},
  {203, kParamTapeMix, 28},   {203, kParamIns1Algo, 4},   {203, kParamIns1A, 22},
  {203, kParamIns1B, 32},   {203, kParamIns1C, 14},   {203, kParamIns1Mix, 14},
  {203, kParamMstAlgo, 2},   {203, kParamMstA, 24},   {203, kParamMstB, 18},
  {203, kParamMstC, 30},   {203, kParamMstMix, 10},
  {204, kParamEngine, 0},   {204, kParamPolyMode, 0},   {204, kParamOsc1Shape, 20},
  {204, kParamOsc1Level, 70},   {204, kParamOsc2Shape, 24},   {204, kParamOsc2Pitch, 19},
  {204, kParamOsc2Level, 44},   {204, kParamFmAmount, 26},   {204, kParamSubLevel, 22},
  {204, kParamNoiseLevel, 4},   {204, kParamFilterType, 0},   {204, kParamFilterCutoff, 6600},
  {204, kParamFilterReso, 12},   {204, kParamFilterEnv, 30},   {204, kParamFiltAttack, 2},
  {204, kParamFiltDecay, 1260},   {204, kParamFiltSustain, 17},   {204, kParamFiltRelease, 1600},
  {204, kParamAmpAttack, 4},   {204, kParamAmpDecay, 1800},   {204, kParamAmpSustain, 34},
  {204, kParamAmpRelease, 2000},   {204, kParamVelToAmp, 64},   {204, kParamVelToFilter, 34},
  {204, kParamStereoSpread, 48},   {204, kParamStereoDelay, 16},   {204, kParamTapeSat, 20},
  {204, kParamTapeMix, 28},   {204, kParamIns1Algo, 2},   {204, kParamIns1A, 20},
  {204, kParamIns1B, 28},   {204, kParamIns1C, 36},   {204, kParamIns1Mix, 20},
  {204, kParamMstAlgo, 2},   {204, kParamMstA, 24},   {204, kParamMstB, 18},
  {204, kParamMstC, 30},   {204, kParamMstMix, 10},
  {205, kParamEngine, 0},   {205, kParamPolyMode, 0},   {205, kParamOsc1Shape, 20},
  {205, kParamOsc1Level, 70},   {205, kParamOsc2Shape, 24},   {205, kParamOsc2Pitch, 19},
  {205, kParamOsc2Level, 44},   {205, kParamFmAmount, 48},   {205, kParamSubLevel, 22},
  {205, kParamNoiseLevel, 4},   {205, kParamFilterType, 0},   {205, kParamFilterCutoff, 9400},
  {205, kParamFilterReso, 20},   {205, kParamFilterEnv, 30},   {205, kParamFiltAttack, 2},
  {205, kParamFiltDecay, 489.99999999999994},   {205, kParamFiltSustain, 6},   {205, kParamFiltRelease, 720},
  {205, kParamAmpAttack, 1},   {205, kParamAmpDecay, 700},   {205, kParamAmpSustain, 12},
  {205, kParamAmpRelease, 900},   {205, kParamVelToAmp, 64},   {205, kParamVelToFilter, 34},
  {205, kParamStereoSpread, 48},   {205, kParamStereoDelay, 16},   {205, kParamTapeSat, 20},
  {205, kParamTapeMix, 28},   {205, kParamOsc3Enable, 1},   {205, kParamOsc3Source, 1},
  {205, kParamOsc3Morph, 4.2},   {205, kParamOsc3Level, 40},   {205, kParamOsc3Pitch, 12},
  {205, kParamIns1Algo, 2},   {205, kParamIns1A, 14},   {205, kParamIns1B, 38},
  {205, kParamIns1C, 44},   {205, kParamIns1Mix, 26},   {205, kParamMstAlgo, 2},
  {205, kParamMstA, 24},   {205, kParamMstB, 18},   {205, kParamMstC, 30},
  {205, kParamMstMix, 10},
  {206, kParamEngine, 0},   {206, kParamPolyMode, 0},   {206, kParamOsc1Shape, 20},
  {206, kParamOsc1Level, 70},   {206, kParamOsc2Shape, 24},   {206, kParamOsc2Pitch, 19},
  {206, kParamOsc2Level, 44},   {206, kParamFmAmount, 52},   {206, kParamSubLevel, 22},
  {206, kParamNoiseLevel, 4},   {206, kParamFilterType, 0},   {206, kParamFilterCutoff, 7400},
  {206, kParamFilterReso, 28},   {206, kParamFilterEnv, 30},   {206, kParamFiltAttack, 2},
  {206, kParamFiltDecay, 770},   {206, kParamFiltSustain, 11},   {206, kParamFiltRelease, 960},
  {206, kParamAmpAttack, 1},   {206, kParamAmpDecay, 1100},   {206, kParamAmpSustain, 22},
  {206, kParamAmpRelease, 1200},   {206, kParamVelToAmp, 64},   {206, kParamVelToFilter, 34},
  {206, kParamStereoSpread, 48},   {206, kParamStereoDelay, 16},   {206, kParamTapeSat, 20},
  {206, kParamTapeMix, 28},   {206, kParamFilterDrive, 26},   {206, kParamIns1Algo, 4},
  {206, kParamIns1A, 22},   {206, kParamIns1B, 32},   {206, kParamIns1C, 14},
  {206, kParamIns1Mix, 14},   {206, kParamMstAlgo, 2},   {206, kParamMstA, 24},
  {206, kParamMstB, 18},   {206, kParamMstC, 30},   {206, kParamMstMix, 10},
  {207, kParamEngine, 0},   {207, kParamPolyMode, 0},   {207, kParamOsc1Shape, 20},
  {207, kParamOsc1Level, 70},   {207, kParamOsc2Shape, 24},   {207, kParamOsc2Pitch, 19},
  {207, kParamOsc2Level, 44},   {207, kParamFmAmount, 30},   {207, kParamSubLevel, 22},
  {207, kParamNoiseLevel, 4},   {207, kParamFilterType, 0},   {207, kParamFilterCutoff, 7000},
  {207, kParamFilterReso, 16},   {207, kParamFilterEnv, 30},   {207, kParamFiltAttack, 2},
  {207, kParamFiltDecay, 1050},   {207, kParamFiltSustain, 14},   {207, kParamFiltRelease, 1360},
  {207, kParamAmpAttack, 3},   {207, kParamAmpDecay, 1500},   {207, kParamAmpSustain, 28},
  {207, kParamAmpRelease, 1700},   {207, kParamVelToAmp, 64},   {207, kParamVelToFilter, 34},
  {207, kParamStereoSpread, 48},   {207, kParamStereoDelay, 16},   {207, kParamTapeSat, 20},
  {207, kParamTapeMix, 44},   {207, kParamTapeWow, 40},   {207, kParamIns1Algo, 2},
  {207, kParamIns1A, 20},   {207, kParamIns1B, 28},   {207, kParamIns1C, 36},
  {207, kParamIns1Mix, 20},   {207, kParamMstAlgo, 2},   {207, kParamMstA, 24},
  {207, kParamMstB, 18},   {207, kParamMstC, 30},   {207, kParamMstMix, 10},
  {208, kParamEngine, 0},   {208, kParamPolyMode, 0},   {208, kParamOsc1Shape, 20},
  {208, kParamOsc1Level, 70},   {208, kParamOsc2Shape, 24},   {208, kParamOsc2Pitch, 12},
  {208, kParamOsc2Level, 44},   {208, kParamFmAmount, 22},   {208, kParamSubLevel, 22},
  {208, kParamNoiseLevel, 4},   {208, kParamFilterType, 0},   {208, kParamFilterCutoff, 6200},
  {208, kParamFilterReso, 18},   {208, kParamFilterEnv, 30},   {208, kParamFiltAttack, 2},
  {208, kParamFiltDecay, 909.9999999999999},   {208, kParamFiltSustain, 15},   {208, kParamFiltRelease, 1200},
  {208, kParamAmpAttack, 3},   {208, kParamAmpDecay, 1300},   {208, kParamAmpSustain, 30},
  {208, kParamAmpRelease, 1500},   {208, kParamVelToAmp, 64},   {208, kParamVelToFilter, 34},
  {208, kParamStereoSpread, 48},   {208, kParamStereoDelay, 16},   {208, kParamTapeSat, 20},
  {208, kParamTapeMix, 28},   {208, kParamIns1Algo, 2},   {208, kParamIns1A, 14},
  {208, kParamIns1B, 38},   {208, kParamIns1C, 44},   {208, kParamIns1Mix, 26},
  {208, kParamMstAlgo, 2},   {208, kParamMstA, 24},   {208, kParamMstB, 18},
  {208, kParamMstC, 30},   {208, kParamMstMix, 10},
  {209, kParamEngine, 0},   {209, kParamPolyMode, 0},   {209, kParamOsc1Shape, 20},
  {209, kParamOsc1Level, 70},   {209, kParamOsc2Shape, 24},   {209, kParamOsc2Pitch, 19},
  {209, kParamOsc2Level, 44},   {209, kParamFmAmount, 18},   {209, kParamSubLevel, 22},
  {209, kParamNoiseLevel, 4},   {209, kParamFilterType, 0},   {209, kParamFilterCutoff, 5600},
  {209, kParamFilterReso, 14},   {209, kParamFilterEnv, 30},   {209, kParamFiltAttack, 2},
  {209, kParamFiltDecay, 1120},   {209, kParamFiltSustain, 18},   {209, kParamFiltRelease, 1440},
  {209, kParamAmpAttack, 5},   {209, kParamAmpDecay, 1600},   {209, kParamAmpSustain, 36},
  {209, kParamAmpRelease, 1800},   {209, kParamVelToAmp, 64},   {209, kParamVelToFilter, 34},
  {209, kParamStereoSpread, 48},   {209, kParamStereoDelay, 16},   {209, kParamTapeSat, 20},
  {209, kParamTapeMix, 28},   {209, kParamIns1Algo, 4},   {209, kParamIns1A, 22},
  {209, kParamIns1B, 32},   {209, kParamIns1C, 14},   {209, kParamIns1Mix, 14},
  {209, kParamMstAlgo, 2},   {209, kParamMstA, 24},   {209, kParamMstB, 18},
  {209, kParamMstC, 30},   {209, kParamMstMix, 10},
  {210, kParamEngine, 0},   {210, kParamPolyMode, 0},   {210, kParamOsc1Shape, 20},
  {210, kParamOsc1Level, 70},   {210, kParamOsc2Shape, 24},   {210, kParamOsc2Pitch, 19},
  {210, kParamOsc2Level, 44},   {210, kParamFmAmount, 30},   {210, kParamSubLevel, 22},
  {210, kParamNoiseLevel, 4},   {210, kParamFilterType, 0},   {210, kParamFilterCutoff, 6800},
  {210, kParamFilterReso, 30},   {210, kParamFilterEnv, 30},   {210, kParamFiltAttack, 2},
  {210, kParamFiltDecay, 700},   {210, kParamFiltSustain, 12},   {210, kParamFiltRelease, 960},
  {210, kParamAmpAttack, 2},   {210, kParamAmpDecay, 1000},   {210, kParamAmpSustain, 24},
  {210, kParamAmpRelease, 1200},   {210, kParamVelToAmp, 64},   {210, kParamVelToFilter, 34},
  {210, kParamStereoSpread, 48},   {210, kParamStereoDelay, 16},   {210, kParamTapeSat, 20},
  {210, kParamTapeMix, 28},   {210, kParamFilterDrive, 30},   {210, kParamIns1Algo, 2},
  {210, kParamIns1A, 20},   {210, kParamIns1B, 28},   {210, kParamIns1C, 36},
  {210, kParamIns1Mix, 20},   {210, kParamMstAlgo, 2},   {210, kParamMstA, 24},
  {210, kParamMstB, 18},   {210, kParamMstC, 30},   {210, kParamMstMix, 10},
  {211, kParamEngine, 0},   {211, kParamPolyMode, 0},   {211, kParamOsc1Shape, 20},
  {211, kParamOsc1Level, 70},   {211, kParamOsc2Shape, 24},   {211, kParamOsc2Pitch, 19},
  {211, kParamOsc2Level, 44},   {211, kParamFmAmount, 24},   {211, kParamSubLevel, 22},
  {211, kParamNoiseLevel, 4},   {211, kParamFilterType, 0},   {211, kParamFilterCutoff, 6000},
  {211, kParamFilterReso, 16},   {211, kParamFilterEnv, 30},   {211, kParamFiltAttack, 2},
  {211, kParamFiltDecay, 979.9999999999999},   {211, kParamFiltSustain, 16},   {211, kParamFiltRelease, 1280},
  {211, kParamAmpAttack, 4},   {211, kParamAmpDecay, 1400},   {211, kParamAmpSustain, 32},
  {211, kParamAmpRelease, 1600},   {211, kParamVelToAmp, 64},   {211, kParamVelToFilter, 34},
  {211, kParamStereoSpread, 48},   {211, kParamStereoDelay, 16},   {211, kParamTapeSat, 20},
  {211, kParamTapeMix, 46},   {211, kParamTapeWow, 44},   {211, kParamTapeFlutter, 30},
  {211, kParamIns1Algo, 2},   {211, kParamIns1A, 14},   {211, kParamIns1B, 38},
  {211, kParamIns1C, 44},   {211, kParamIns1Mix, 26},   {211, kParamMstAlgo, 2},
  {211, kParamMstA, 24},   {211, kParamMstB, 18},   {211, kParamMstC, 30},
  {211, kParamMstMix, 10},
  {212, kParamEngine, 0},   {212, kParamPolyMode, 0},   {212, kParamOsc1Shape, 10},
  {212, kParamOsc1Level, 78},   {212, kParamOsc2Shape, 10},   {212, kParamOsc2Pitch, 12},
  {212, kParamOsc2Level, 52},   {212, kParamSubLevel, 64},   {212, kParamNoiseLevel, 2},
  {212, kParamOsc3Enable, 1},   {212, kParamOsc3Source, 0},   {212, kParamOsc3Shape, 10},
  {212, kParamOsc3Level, 38},   {212, kParamOsc3Pitch, 19},   {212, kParamFilterType, 0},
  {212, kParamFilterCutoff, 11000},   {212, kParamFilterReso, 8},   {212, kParamFilterEnv, 0},
  {212, kParamFilterDrive, 18},   {212, kParamFiltAttack, 1},   {212, kParamFiltDecay, 200},
  {212, kParamFiltSustain, 100},   {212, kParamFiltRelease, 240},   {212, kParamAmpAttack, 2},
  {212, kParamAmpDecay, 200},   {212, kParamAmpSustain, 100},   {212, kParamAmpRelease, 240},
  {212, kParamVelToAmp, 18},   {212, kParamVelToFilter, 0},   {212, kParamStereoSpread, 40},
  {212, kParamStereoDelay, 0},   {212, kParamTapeSat, 26},   {212, kParamTapeMix, 30},
  {212, kParamIns1Algo, 4},   {212, kParamIns1A, 22},   {212, kParamIns1B, 32},
  {212, kParamIns1C, 14},   {212, kParamIns1Mix, 14},   {212, kParamMstAlgo, 2},
  {212, kParamMstA, 24},   {212, kParamMstB, 18},   {212, kParamMstC, 30},
  {212, kParamMstMix, 10},
  {213, kParamEngine, 0},   {213, kParamPolyMode, 0},   {213, kParamOsc1Shape, 10},
  {213, kParamOsc1Level, 88},   {213, kParamOsc2Shape, 10},   {213, kParamOsc2Pitch, 12},
  {213, kParamOsc2Level, 52},   {213, kParamSubLevel, 76},   {213, kParamNoiseLevel, 2},
  {213, kParamOsc3Enable, 1},   {213, kParamOsc3Source, 0},   {213, kParamOsc3Shape, 10},
  {213, kParamOsc3Level, 52},   {213, kParamOsc3Pitch, 19},   {213, kParamFilterType, 0},
  {213, kParamFilterCutoff, 12000},   {213, kParamFilterReso, 6},   {213, kParamFilterEnv, 0},
  {213, kParamFilterDrive, 22},   {213, kParamFiltAttack, 1},   {213, kParamFiltDecay, 200},
  {213, kParamFiltSustain, 100},   {213, kParamFiltRelease, 260},   {213, kParamAmpAttack, 2},
  {213, kParamAmpDecay, 200},   {213, kParamAmpSustain, 100},   {213, kParamAmpRelease, 260},
  {213, kParamVelToAmp, 18},   {213, kParamVelToFilter, 0},   {213, kParamStereoSpread, 40},
  {213, kParamStereoDelay, 0},   {213, kParamTapeSat, 26},   {213, kParamTapeMix, 30},
  {213, kParamIns1Algo, 2},   {213, kParamIns1A, 20},   {213, kParamIns1B, 28},
  {213, kParamIns1C, 36},   {213, kParamIns1Mix, 20},   {213, kParamMstAlgo, 2},
  {213, kParamMstA, 24},   {213, kParamMstB, 18},   {213, kParamMstC, 30},
  {213, kParamMstMix, 10},
  {214, kParamEngine, 0},   {214, kParamPolyMode, 0},   {214, kParamOsc1Shape, 10},
  {214, kParamOsc1Level, 78},   {214, kParamOsc2Shape, 10},   {214, kParamOsc2Pitch, 12},
  {214, kParamOsc2Level, 52},   {214, kParamSubLevel, 64},   {214, kParamNoiseLevel, 2},
  {214, kParamOsc3Enable, 1},   {214, kParamOsc3Source, 0},   {214, kParamOsc3Shape, 10},
  {214, kParamOsc3Level, 38},   {214, kParamOsc3Pitch, 19},   {214, kParamFilterType, 0},
  {214, kParamFilterCutoff, 11000},   {214, kParamFilterReso, 10},   {214, kParamFilterEnv, 0},
  {214, kParamFilterDrive, 20},   {214, kParamFiltAttack, 1},   {214, kParamFiltDecay, 200},
  {214, kParamFiltSustain, 100},   {214, kParamFiltRelease, 220},   {214, kParamAmpAttack, 1},
  {214, kParamAmpDecay, 200},   {214, kParamAmpSustain, 100},   {214, kParamAmpRelease, 220},
  {214, kParamVelToAmp, 18},   {214, kParamVelToFilter, 0},   {214, kParamStereoSpread, 40},
  {214, kParamStereoDelay, 0},   {214, kParamTapeSat, 26},   {214, kParamTapeMix, 30},
  {214, kParamPitchAmount, -16},   {214, kParamPitchAttack, 1},   {214, kParamPitchDecay, 60},
  {214, kParamPitchSustain, 0},   {214, kParamPitchRelease, 120},   {214, kParamIns1Algo, 2},
  {214, kParamIns1A, 14},   {214, kParamIns1B, 38},   {214, kParamIns1C, 44},
  {214, kParamIns1Mix, 26},   {214, kParamMstAlgo, 2},   {214, kParamMstA, 24},
  {214, kParamMstB, 18},   {214, kParamMstC, 30},   {214, kParamMstMix, 10},
  {215, kParamEngine, 0},   {215, kParamPolyMode, 0},   {215, kParamOsc1Shape, 10},
  {215, kParamOsc1Level, 90},   {215, kParamOsc2Shape, 10},   {215, kParamOsc2Pitch, 12},
  {215, kParamOsc2Level, 52},   {215, kParamSubLevel, 64},   {215, kParamNoiseLevel, 2},
  {215, kParamOsc3Enable, 1},   {215, kParamOsc3Source, 0},   {215, kParamOsc3Shape, 10},
  {215, kParamOsc3Level, 38},   {215, kParamOsc3Pitch, 19},   {215, kParamFilterType, 0},
  {215, kParamFilterCutoff, 12500},   {215, kParamFilterReso, 12},   {215, kParamFilterEnv, 0},
  {215, kParamFilterDrive, 34},   {215, kParamFiltAttack, 1},   {215, kParamFiltDecay, 200},
  {215, kParamFiltSustain, 100},   {215, kParamFiltRelease, 280},   {215, kParamAmpAttack, 2},
  {215, kParamAmpDecay, 200},   {215, kParamAmpSustain, 100},   {215, kParamAmpRelease, 280},
  {215, kParamVelToAmp, 18},   {215, kParamVelToFilter, 0},   {215, kParamStereoSpread, 40},
  {215, kParamStereoDelay, 0},   {215, kParamTapeSat, 26},   {215, kParamTapeMix, 30},
  {215, kParamIns1Algo, 4},   {215, kParamIns1A, 22},   {215, kParamIns1B, 32},
  {215, kParamIns1C, 14},   {215, kParamIns1Mix, 14},   {215, kParamMstAlgo, 2},
  {215, kParamMstA, 24},   {215, kParamMstB, 18},   {215, kParamMstC, 30},
  {215, kParamMstMix, 10},
  {216, kParamEngine, 0},   {216, kParamPolyMode, 0},   {216, kParamOsc1Shape, 10},
  {216, kParamOsc1Level, 78},   {216, kParamOsc2Shape, 10},   {216, kParamOsc2Pitch, 12},
  {216, kParamOsc2Level, 52},   {216, kParamSubLevel, 64},   {216, kParamNoiseLevel, 2},
  {216, kParamOsc3Enable, 1},   {216, kParamOsc3Source, 0},   {216, kParamOsc3Shape, 10},
  {216, kParamOsc3Level, 38},   {216, kParamOsc3Pitch, 19},   {216, kParamFilterType, 0},
  {216, kParamFilterCutoff, 9600},   {216, kParamFilterReso, 14},   {216, kParamFilterEnv, 0},
  {216, kParamFilterDrive, 48},   {216, kParamFiltAttack, 1},   {216, kParamFiltDecay, 200},
  {216, kParamFiltSustain, 100},   {216, kParamFiltRelease, 240},   {216, kParamAmpAttack, 2},
  {216, kParamAmpDecay, 200},   {216, kParamAmpSustain, 100},   {216, kParamAmpRelease, 240},
  {216, kParamVelToAmp, 18},   {216, kParamVelToFilter, 0},   {216, kParamStereoSpread, 40},
  {216, kParamStereoDelay, 0},   {216, kParamTapeSat, 40},   {216, kParamTapeMix, 46},
  {216, kParamIns1Algo, 2},   {216, kParamIns1A, 20},   {216, kParamIns1B, 28},
  {216, kParamIns1C, 36},   {216, kParamIns1Mix, 20},   {216, kParamMstAlgo, 2},
  {216, kParamMstA, 24},   {216, kParamMstB, 18},   {216, kParamMstC, 30},
  {216, kParamMstMix, 10},
  {217, kParamEngine, 0},   {217, kParamPolyMode, 0},   {217, kParamOsc1Shape, 52},
  {217, kParamOsc1PW, 26},   {217, kParamOsc1Level, 80},   {217, kParamOsc2Shape, 10},
  {217, kParamOsc2Pitch, 12},   {217, kParamOsc2Level, 40},   {217, kParamSubLevel, 40},
  {217, kParamNoiseLevel, 3},   {217, kParamFilterType, 0},   {217, kParamFilterCutoff, 9200},
  {217, kParamFilterReso, 22},   {217, kParamFilterEnv, 6},   {217, kParamFilterDrive, 14},
  {217, kParamFiltAttack, 1},   {217, kParamFiltDecay, 200},   {217, kParamFiltSustain, 100},
  {217, kParamFiltRelease, 220},   {217, kParamAmpAttack, 2},   {217, kParamAmpDecay, 200},
  {217, kParamAmpSustain, 100},   {217, kParamAmpRelease, 220},   {217, kParamVelToAmp, 16},
  {217, kParamStereoSpread, 48},   {217, kParamTapeSat, 24},   {217, kParamTapeMix, 30},
  {217, kParamIns1Algo, 2},   {217, kParamIns1A, 14},   {217, kParamIns1B, 38},
  {217, kParamIns1C, 44},   {217, kParamIns1Mix, 26},   {217, kParamMstAlgo, 2},
  {217, kParamMstA, 24},   {217, kParamMstB, 18},   {217, kParamMstC, 30},
  {217, kParamMstMix, 10},
  {218, kParamEngine, 0},   {218, kParamPolyMode, 0},   {218, kParamOsc1Shape, 52},
  {218, kParamOsc1PW, 26},   {218, kParamOsc1Level, 80},   {218, kParamOsc2Shape, 10},
  {218, kParamOsc2Pitch, 12},   {218, kParamOsc2Level, 40},   {218, kParamSubLevel, 40},
  {218, kParamNoiseLevel, 3},   {218, kParamFilterType, 0},   {218, kParamFilterCutoff, 8400},
  {218, kParamFilterReso, 30},   {218, kParamFilterEnv, 6},   {218, kParamFilterDrive, 18},
  {218, kParamFiltAttack, 1},   {218, kParamFiltDecay, 200},   {218, kParamFiltSustain, 100},
  {218, kParamFiltRelease, 200},   {218, kParamAmpAttack, 2},   {218, kParamAmpDecay, 200},
  {218, kParamAmpSustain, 100},   {218, kParamAmpRelease, 200},   {218, kParamVelToAmp, 16},
  {218, kParamStereoSpread, 48},   {218, kParamTapeSat, 24},   {218, kParamTapeMix, 30},
  {218, kParamIns1Algo, 4},   {218, kParamIns1A, 22},   {218, kParamIns1B, 32},
  {218, kParamIns1C, 14},   {218, kParamIns1Mix, 14},   {218, kParamMstAlgo, 2},
  {218, kParamMstA, 24},   {218, kParamMstB, 18},   {218, kParamMstC, 30},
  {218, kParamMstMix, 10},
  {219, kParamEngine, 0},   {219, kParamPolyMode, 0},   {219, kParamOsc1Shape, 52},
  {219, kParamOsc1PW, 18},   {219, kParamOsc1Level, 80},   {219, kParamOsc2Shape, 10},
  {219, kParamOsc2Pitch, 12},   {219, kParamOsc2Level, 40},   {219, kParamSubLevel, 40},
  {219, kParamNoiseLevel, 3},   {219, kParamFilterType, 0},   {219, kParamFilterCutoff, 11000},
  {219, kParamFilterReso, 18},   {219, kParamFilterEnv, 6},   {219, kParamFilterDrive, 12},
  {219, kParamFiltAttack, 1},   {219, kParamFiltDecay, 200},   {219, kParamFiltSustain, 100},
  {219, kParamFiltRelease, 200},   {219, kParamAmpAttack, 1},   {219, kParamAmpDecay, 200},
  {219, kParamAmpSustain, 100},   {219, kParamAmpRelease, 200},   {219, kParamVelToAmp, 16},
  {219, kParamStereoSpread, 48},   {219, kParamTapeSat, 24},   {219, kParamTapeMix, 30},
  {219, kParamIns1Algo, 2},   {219, kParamIns1A, 20},   {219, kParamIns1B, 28},
  {219, kParamIns1C, 36},   {219, kParamIns1Mix, 20},   {219, kParamMstAlgo, 2},
  {219, kParamMstA, 24},   {219, kParamMstB, 18},   {219, kParamMstC, 30},
  {219, kParamMstMix, 10},
  {220, kParamEngine, 0},   {220, kParamPolyMode, 0},   {220, kParamOsc1Shape, 28},
  {220, kParamOsc1Level, 52},   {220, kParamOsc2Level, 0},   {220, kParamOsc3Enable, 1},
  {220, kParamOsc3Source, 1},   {220, kParamOsc3Morph, 3.2},   {220, kParamOsc3Level, 84},
  {220, kParamOsc3Pitch, 12},   {220, kParamSubLevel, 24},   {220, kParamNoiseLevel, 10},
  {220, kParamFilterType, 0},   {220, kParamFilterCutoff, 5800},   {220, kParamFilterReso, 14},
  {220, kParamFilterEnv, 18},   {220, kParamFiltAttack, 125.99999999999999},   {220, kParamFiltDecay, 1800},
  {220, kParamFiltSustain, 58},   {220, kParamFiltRelease, 2200},   {220, kParamAmpAttack, 180},
  {220, kParamAmpDecay, 2000},   {220, kParamAmpSustain, 80},   {220, kParamAmpRelease, 2200},
  {220, kParamUnisonVoices, 3},   {220, kParamUnisonDetune, 18},   {220, kParamStereoSpread, 76},
  {220, kParamTapeSat, 30},   {220, kParamTapeWow, 40},   {220, kParamTapeFlutter, 32},
  {220, kParamTapeMix, 42},   {220, kParamIns1Algo, 2},   {220, kParamIns1A, 14},
  {220, kParamIns1B, 38},   {220, kParamIns1C, 44},   {220, kParamIns1Mix, 26},
  {220, kParamMstAlgo, 2},   {220, kParamMstA, 24},   {220, kParamMstB, 18},
  {220, kParamMstC, 30},   {220, kParamMstMix, 10},
  {221, kParamEngine, 0},   {221, kParamPolyMode, 0},   {221, kParamOsc1Shape, 28},
  {221, kParamOsc1Level, 52},   {221, kParamOsc2Level, 0},   {221, kParamOsc3Enable, 1},
  {221, kParamOsc3Source, 1},   {221, kParamOsc3Morph, 2.4},   {221, kParamOsc3Level, 84},
  {221, kParamOsc3Pitch, 12},   {221, kParamSubLevel, 24},   {221, kParamNoiseLevel, 10},
  {221, kParamFilterType, 0},   {221, kParamFilterCutoff, 6400},   {221, kParamFilterReso, 12},
  {221, kParamFilterEnv, 18},   {221, kParamFiltAttack, 182},   {221, kParamFiltDecay, 1800},
  {221, kParamFiltSustain, 58},   {221, kParamFiltRelease, 2400},   {221, kParamAmpAttack, 260},
  {221, kParamAmpDecay, 2000},   {221, kParamAmpSustain, 80},   {221, kParamAmpRelease, 2400},
  {221, kParamUnisonVoices, 3},   {221, kParamUnisonDetune, 18},   {221, kParamStereoSpread, 82},
  {221, kParamTapeSat, 30},   {221, kParamTapeWow, 34},   {221, kParamTapeFlutter, 27.200000000000003},
  {221, kParamTapeMix, 42},   {221, kParamIns1Algo, 4},   {221, kParamIns1A, 22},
  {221, kParamIns1B, 32},   {221, kParamIns1C, 14},   {221, kParamIns1Mix, 14},
  {221, kParamMstAlgo, 2},   {221, kParamMstA, 24},   {221, kParamMstB, 18},
  {221, kParamMstC, 30},   {221, kParamMstMix, 10},
  // guitars
  {222, kParamEngine, 0},   {222, kParamPolyMode, 0},   {222, kParamGuitarBank, 0},
  {222, kParamGuitarLevel, -60},   {222, kParamOsc1Shape, 44},   {222, kParamOsc1PW, 34},
  {222, kParamOsc1Level, 84},   {222, kParamOsc2Shape, 38},   {222, kParamOsc2Pitch, 12},
  {222, kParamOsc2Level, 38},   {222, kParamSubLevel, 20},   {222, kParamNoiseLevel, 8},
  {222, kParamFilterType, 0},   {222, kParamFilterCutoff, 7200},   {222, kParamFilterReso, 12},
  {222, kParamFilterDrive, 0},   {222, kParamFilterEnv, 62},   {222, kParamFilterKeyTrack, 52},
  {222, kParamFiltAttack, 1},   {222, kParamFiltDecay, 700},   {222, kParamFiltSustain, 7.699999999999999},
  {222, kParamFiltRelease, 800},   {222, kParamAmpAttack, 2},   {222, kParamAmpDecay, 1400},
  {222, kParamAmpSustain, 22},   {222, kParamAmpRelease, 1600},   {222, kParamVelToAmp, 54},
  {222, kParamVelToFilter, 46},   {222, kParamStereoSpread, 44},   {222, kParamStereoDelay, 12},
  {222, kParamTapeSat, 20},   {222, kParamTapeMix, 26},   {222, kParamIns1Algo, 2},
  {222, kParamIns1A, 14},   {222, kParamIns1B, 24},   {222, kParamIns1C, 30},
  {222, kParamIns1Mix, 16},   {222, kParamMstAlgo, 2},   {222, kParamMstA, 24},
  {222, kParamMstB, 18},   {222, kParamMstC, 30},   {222, kParamMstMix, 10},
  {223, kParamEngine, 0},   {223, kParamPolyMode, 0},   {223, kParamGuitarBank, 5},
  {223, kParamGuitarLevel, -6},   {223, kParamOsc1Shape, 32},   {223, kParamOsc1PW, 42},
  {223, kParamOsc1Level, 84},   {223, kParamOsc2Shape, 38},   {223, kParamOsc2Pitch, 12},
  {223, kParamOsc2Level, 38},   {223, kParamSubLevel, 20},   {223, kParamNoiseLevel, 8},
  {223, kParamFilterType, 0},   {223, kParamFilterCutoff, 5400},   {223, kParamFilterReso, 10},
  {223, kParamFilterDrive, 0},   {223, kParamFilterEnv, 62},   {223, kParamFilterKeyTrack, 52},
  {223, kParamFiltAttack, 1},   {223, kParamFiltDecay, 600},   {223, kParamFiltSustain, 9.1},
  {223, kParamFiltRelease, 700},   {223, kParamAmpAttack, 3},   {223, kParamAmpDecay, 1200},
  {223, kParamAmpSustain, 26},   {223, kParamAmpRelease, 1400},   {223, kParamVelToAmp, 54},
  {223, kParamVelToFilter, 46},   {223, kParamStereoSpread, 44},   {223, kParamStereoDelay, 12},
  {223, kParamTapeSat, 20},   {223, kParamTapeMix, 26},   {223, kParamIns1Algo, 2},
  {223, kParamIns1A, 20},   {223, kParamIns1B, 30},   {223, kParamIns1C, 38},
  {223, kParamIns1Mix, 20},   {223, kParamMstAlgo, 2},   {223, kParamMstA, 24},
  {223, kParamMstB, 18},   {223, kParamMstC, 30},   {223, kParamMstMix, 10},
  {224, kParamEngine, 0},   {224, kParamPolyMode, 0},   {224, kParamGuitarBank, 0},
  {224, kParamGuitarLevel, -60},   {224, kParamOsc1Shape, 46},   {224, kParamOsc1PW, 28},
  {224, kParamOsc1Level, 84},   {224, kParamOsc2Shape, 38},   {224, kParamOsc2Pitch, 12},
  {224, kParamOsc2Level, 38},   {224, kParamSubLevel, 20},   {224, kParamNoiseLevel, 8},
  {224, kParamFilterType, 0},   {224, kParamFilterCutoff, 8600},   {224, kParamFilterReso, 14},
  {224, kParamFilterDrive, 0},   {224, kParamFilterEnv, 62},   {224, kParamFilterKeyTrack, 52},
  {224, kParamFiltAttack, 1},   {224, kParamFiltDecay, 550},   {224, kParamFiltSustain, 6.3},
  {224, kParamFiltRelease, 650},   {224, kParamAmpAttack, 1},   {224, kParamAmpDecay, 1100},
  {224, kParamAmpSustain, 18},   {224, kParamAmpRelease, 1300},   {224, kParamVelToAmp, 54},
  {224, kParamVelToFilter, 46},   {224, kParamStereoSpread, 44},   {224, kParamStereoDelay, 12},
  {224, kParamTapeSat, 20},   {224, kParamTapeMix, 26},   {224, kParamIns1Algo, 3},
  {224, kParamIns1A, 26},   {224, kParamIns1B, 22},   {224, kParamIns1C, 44},
  {224, kParamIns1Mix, 12},   {224, kParamMstAlgo, 2},   {224, kParamMstA, 24},
  {224, kParamMstB, 18},   {224, kParamMstC, 30},   {224, kParamMstMix, 10},
  {225, kParamEngine, 0},   {225, kParamPolyMode, 0},   {225, kParamGuitarBank, 0},
  {225, kParamGuitarLevel, -60},   {225, kParamOsc1Shape, 32},   {225, kParamOsc1PW, 46},
  {225, kParamOsc1Level, 84},   {225, kParamOsc2Shape, 38},   {225, kParamOsc2Pitch, 12},
  {225, kParamOsc2Level, 38},   {225, kParamSubLevel, 20},   {225, kParamNoiseLevel, 8},
  {225, kParamFilterType, 0},   {225, kParamFilterCutoff, 4600},   {225, kParamFilterReso, 12},
  {225, kParamFilterDrive, 0},   {225, kParamFilterEnv, 62},   {225, kParamFilterKeyTrack, 52},
  {225, kParamFiltAttack, 1},   {225, kParamFiltDecay, 800},   {225, kParamFiltSustain, 10.5},
  {225, kParamFiltRelease, 900},   {225, kParamAmpAttack, 4},   {225, kParamAmpDecay, 1600},
  {225, kParamAmpSustain, 30},   {225, kParamAmpRelease, 1800},   {225, kParamVelToAmp, 54},
  {225, kParamVelToFilter, 46},   {225, kParamStereoSpread, 44},   {225, kParamStereoDelay, 12},
  {225, kParamTapeSat, 20},   {225, kParamTapeMix, 26},   {225, kParamIns1Algo, 2},
  {225, kParamIns1A, 14},   {225, kParamIns1B, 24},   {225, kParamIns1C, 30},
  {225, kParamIns1Mix, 16},   {225, kParamMstAlgo, 2},   {225, kParamMstA, 24},
  {225, kParamMstB, 18},   {225, kParamMstC, 30},   {225, kParamMstMix, 10},
  {226, kParamEngine, 0},   {226, kParamPolyMode, 0},   {226, kParamGuitarBank, 0},
  {226, kParamGuitarLevel, -60},   {226, kParamOsc1Shape, 38},   {226, kParamOsc1PW, 38},
  {226, kParamOsc1Level, 84},   {226, kParamOsc2Shape, 38},   {226, kParamOsc2Pitch, 12},
  {226, kParamOsc2Level, 38},   {226, kParamSubLevel, 20},   {226, kParamNoiseLevel, 8},
  {226, kParamFilterType, 0},   {226, kParamFilterCutoff, 5600},   {226, kParamFilterReso, 12},
  {226, kParamFilterDrive, 0},   {226, kParamFilterEnv, 62},   {226, kParamFilterKeyTrack, 52},
  {226, kParamFiltAttack, 1},   {226, kParamFiltDecay, 650},   {226, kParamFiltSustain, 9.1},
  {226, kParamFiltRelease, 750},   {226, kParamAmpAttack, 3},   {226, kParamAmpDecay, 1300},
  {226, kParamAmpSustain, 26},   {226, kParamAmpRelease, 1500},   {226, kParamVelToAmp, 54},
  {226, kParamVelToFilter, 46},   {226, kParamStereoSpread, 44},   {226, kParamStereoDelay, 12},
  {226, kParamTapeSat, 20},   {226, kParamTapeMix, 42},   {226, kParamTapeWow, 38},
  {226, kParamTapeFlutter, 26},   {226, kParamIns1Algo, 2},   {226, kParamIns1A, 20},
  {226, kParamIns1B, 30},   {226, kParamIns1C, 38},   {226, kParamIns1Mix, 20},
  {226, kParamMstAlgo, 2},   {226, kParamMstA, 24},   {226, kParamMstB, 18},
  {226, kParamMstC, 30},   {226, kParamMstMix, 10},
  {227, kParamEngine, 0},   {227, kParamPolyMode, 0},   {227, kParamGuitarBank, 0},
  {227, kParamGuitarLevel, -60},   {227, kParamOsc1Shape, 36},   {227, kParamOsc1PW, 40},
  {227, kParamOsc1Level, 84},   {227, kParamOsc2Shape, 38},   {227, kParamOsc2Pitch, 12},
  {227, kParamOsc2Level, 38},   {227, kParamSubLevel, 20},   {227, kParamNoiseLevel, 8},
  {227, kParamFilterType, 0},   {227, kParamFilterCutoff, 6200},   {227, kParamFilterReso, 10},
  {227, kParamFilterDrive, 0},   {227, kParamFilterEnv, 62},   {227, kParamFilterKeyTrack, 52},
  {227, kParamFiltAttack, 1},   {227, kParamFiltDecay, 750},   {227, kParamFiltSustain, 9.799999999999999},
  {227, kParamFiltRelease, 850},   {227, kParamAmpAttack, 3},   {227, kParamAmpDecay, 1500},
  {227, kParamAmpSustain, 28},   {227, kParamAmpRelease, 1700},   {227, kParamVelToAmp, 54},
  {227, kParamVelToFilter, 58},   {227, kParamStereoSpread, 44},   {227, kParamStereoDelay, 12},
  {227, kParamTapeSat, 20},   {227, kParamTapeMix, 26},   {227, kParamIns1Algo, 3},
  {227, kParamIns1A, 26},   {227, kParamIns1B, 22},   {227, kParamIns1C, 44},
  {227, kParamIns1Mix, 12},   {227, kParamMstAlgo, 2},   {227, kParamMstA, 24},
  {227, kParamMstB, 18},   {227, kParamMstC, 30},   {227, kParamMstMix, 10},
  {228, kParamEngine, 0},   {228, kParamPolyMode, 0},   {228, kParamGuitarBank, 0},
  {228, kParamGuitarLevel, -60},   {228, kParamOsc1Shape, 44},   {228, kParamOsc1PW, 30},
  {228, kParamOsc1Level, 84},   {228, kParamOsc2Shape, 38},   {228, kParamOsc2Pitch, 12},
  {228, kParamOsc2Level, 56},   {228, kParamSubLevel, 20},   {228, kParamNoiseLevel, 8},
  {228, kParamFilterType, 0},   {228, kParamFilterCutoff, 7600},   {228, kParamFilterReso, 16},
  {228, kParamFilterDrive, 0},   {228, kParamFilterEnv, 62},   {228, kParamFilterKeyTrack, 52},
  {228, kParamFiltAttack, 1},   {228, kParamFiltDecay, 650},   {228, kParamFiltSustain, 8.399999999999999},
  {228, kParamFiltRelease, 750},   {228, kParamAmpAttack, 2},   {228, kParamAmpDecay, 1300},
  {228, kParamAmpSustain, 24},   {228, kParamAmpRelease, 1500},   {228, kParamVelToAmp, 54},
  {228, kParamVelToFilter, 46},   {228, kParamStereoSpread, 68},   {228, kParamStereoDelay, 12},
  {228, kParamTapeSat, 20},   {228, kParamTapeMix, 26},   {228, kParamIns1Algo, 2},
  {228, kParamIns1A, 14},   {228, kParamIns1B, 24},   {228, kParamIns1C, 30},
  {228, kParamIns1Mix, 16},   {228, kParamMstAlgo, 2},   {228, kParamMstA, 24},
  {228, kParamMstB, 18},   {228, kParamMstC, 30},   {228, kParamMstMix, 10},
  {229, kParamEngine, 0},   {229, kParamPolyMode, 0},   {229, kParamGuitarBank, 0},
  {229, kParamGuitarLevel, -60},   {229, kParamOsc1Shape, 34},   {229, kParamOsc1PW, 44},
  {229, kParamOsc1Level, 84},   {229, kParamOsc2Shape, 38},   {229, kParamOsc2Pitch, 12},
  {229, kParamOsc2Level, 38},   {229, kParamSubLevel, 20},   {229, kParamNoiseLevel, 8},
  {229, kParamFilterType, 0},   {229, kParamFilterCutoff, 4200},   {229, kParamFilterReso, 20},
  {229, kParamFilterDrive, 0},   {229, kParamFilterEnv, 74},   {229, kParamFilterKeyTrack, 52},
  {229, kParamFiltAttack, 1},   {229, kParamFiltDecay, 350},   {229, kParamFiltSustain, 4.199999999999999},
  {229, kParamFiltRelease, 450},   {229, kParamAmpAttack, 1},   {229, kParamAmpDecay, 700},
  {229, kParamAmpSustain, 12},   {229, kParamAmpRelease, 700},   {229, kParamVelToAmp, 54},
  {229, kParamVelToFilter, 46},   {229, kParamStereoSpread, 44},   {229, kParamStereoDelay, 12},
  {229, kParamTapeSat, 20},   {229, kParamTapeMix, 26},   {229, kParamIns1Algo, 2},
  {229, kParamIns1A, 20},   {229, kParamIns1B, 30},   {229, kParamIns1C, 38},
  {229, kParamIns1Mix, 20},   {229, kParamMstAlgo, 2},   {229, kParamMstA, 24},
  {229, kParamMstB, 18},   {229, kParamMstC, 30},   {229, kParamMstMix, 10},
  {230, kParamEngine, 0},   {230, kParamPolyMode, 0},   {230, kParamGuitarBank, 1},
  {230, kParamGuitarLevel, -6},   {230, kParamOsc1Shape, 44},   {230, kParamOsc1PW, 32},
  {230, kParamOsc1Level, 84},   {230, kParamOsc2Shape, 38},   {230, kParamOsc2Pitch, 12},
  {230, kParamOsc2Level, 38},   {230, kParamSubLevel, 20},   {230, kParamNoiseLevel, 8},
  {230, kParamFilterType, 0},   {230, kParamFilterCutoff, 6800},   {230, kParamFilterReso, 16},
  {230, kParamFilterDrive, 0},   {230, kParamFilterEnv, 62},   {230, kParamFilterKeyTrack, 52},
  {230, kParamFiltAttack, 1},   {230, kParamFiltDecay, 650},   {230, kParamFiltSustain, 8.399999999999999},
  {230, kParamFiltRelease, 750},   {230, kParamAmpAttack, 2},   {230, kParamAmpDecay, 1300},
  {230, kParamAmpSustain, 24},   {230, kParamAmpRelease, 1500},   {230, kParamVelToAmp, 54},
  {230, kParamVelToFilter, 46},   {230, kParamStereoSpread, 44},   {230, kParamStereoDelay, 12},
  {230, kParamTapeSat, 20},   {230, kParamTapeMix, 26},   {230, kParamIns1Algo, 3},
  {230, kParamIns1A, 26},   {230, kParamIns1B, 22},   {230, kParamIns1C, 44},
  {230, kParamIns1Mix, 12},   {230, kParamMstAlgo, 2},   {230, kParamMstA, 24},
  {230, kParamMstB, 18},   {230, kParamMstC, 30},   {230, kParamMstMix, 10},
  {231, kParamEngine, 0},   {231, kParamPolyMode, 0},   {231, kParamGuitarBank, 3},
  {231, kParamGuitarLevel, -6},   {231, kParamOsc1Shape, 46},   {231, kParamOsc1PW, 30},
  {231, kParamOsc1Level, 84},   {231, kParamOsc2Shape, 38},   {231, kParamOsc2Pitch, 12},
  {231, kParamOsc2Level, 38},   {231, kParamSubLevel, 20},   {231, kParamNoiseLevel, 8},
  {231, kParamFilterType, 0},   {231, kParamFilterCutoff, 6200},   {231, kParamFilterReso, 24},
  {231, kParamFilterDrive, 34},   {231, kParamFilterEnv, 62},   {231, kParamFilterKeyTrack, 52},
  {231, kParamFiltAttack, 1},   {231, kParamFiltDecay, 600},   {231, kParamFiltSustain, 9.1},
  {231, kParamFiltRelease, 700},   {231, kParamAmpAttack, 2},   {231, kParamAmpDecay, 1200},
  {231, kParamAmpSustain, 26},   {231, kParamAmpRelease, 1400},   {231, kParamVelToAmp, 54},
  {231, kParamVelToFilter, 46},   {231, kParamStereoSpread, 44},   {231, kParamStereoDelay, 12},
  {231, kParamTapeSat, 20},   {231, kParamTapeMix, 26},   {231, kParamIns1Algo, 2},
  {231, kParamIns1A, 14},   {231, kParamIns1B, 24},   {231, kParamIns1C, 30},
  {231, kParamIns1Mix, 16},   {231, kParamMstAlgo, 2},   {231, kParamMstA, 24},
  {231, kParamMstB, 18},   {231, kParamMstC, 30},   {231, kParamMstMix, 10},
  {232, kParamEngine, 0},   {232, kParamPolyMode, 0},   {232, kParamGuitarBank, 4},
  {232, kParamGuitarLevel, -6},   {232, kParamOsc1Shape, 48},   {232, kParamOsc1PW, 26},
  {232, kParamOsc1Level, 84},   {232, kParamOsc2Shape, 38},   {232, kParamOsc2Pitch, 12},
  {232, kParamOsc2Level, 38},   {232, kParamSubLevel, 20},   {232, kParamNoiseLevel, 8},
  {232, kParamFilterType, 0},   {232, kParamFilterCutoff, 5600},   {232, kParamFilterReso, 30},
  {232, kParamFilterDrive, 64},   {232, kParamFilterEnv, 48},   {232, kParamFilterKeyTrack, 52},
  {232, kParamFiltAttack, 1},   {232, kParamFiltDecay, 500},   {232, kParamFiltSustain, 10.5},
  {232, kParamFiltRelease, 650},   {232, kParamAmpAttack, 1},   {232, kParamAmpDecay, 1000},
  {232, kParamAmpSustain, 30},   {232, kParamAmpRelease, 1300},   {232, kParamVelToAmp, 54},
  {232, kParamVelToFilter, 46},   {232, kParamStereoSpread, 44},   {232, kParamStereoDelay, 12},
  {232, kParamTapeSat, 20},   {232, kParamTapeMix, 26},   {232, kParamIns1Algo, 2},
  {232, kParamIns1A, 20},   {232, kParamIns1B, 30},   {232, kParamIns1C, 38},
  {232, kParamIns1Mix, 20},   {232, kParamMstAlgo, 2},   {232, kParamMstA, 24},
  {232, kParamMstB, 18},   {232, kParamMstC, 30},   {232, kParamMstMix, 10},
  {233, kParamEngine, 0},   {233, kParamPolyMode, 0},   {233, kParamGuitarBank, 2},
  {233, kParamGuitarLevel, -6},   {233, kParamOsc1Shape, 40},   {233, kParamOsc1PW, 36},
  {233, kParamOsc1Level, 84},   {233, kParamOsc2Shape, 38},   {233, kParamOsc2Pitch, 12},
  {233, kParamOsc2Level, 38},   {233, kParamSubLevel, 20},   {233, kParamNoiseLevel, 8},
  {233, kParamFilterType, 0},   {233, kParamFilterCutoff, 5200},   {233, kParamFilterReso, 18},
  {233, kParamFilterDrive, 18},   {233, kParamFilterEnv, 62},   {233, kParamFilterKeyTrack, 52},
  {233, kParamFiltAttack, 1},   {233, kParamFiltDecay, 750},   {233, kParamFiltSustain, 9.799999999999999},
  {233, kParamFiltRelease, 850},   {233, kParamAmpAttack, 3},   {233, kParamAmpDecay, 1500},
  {233, kParamAmpSustain, 28},   {233, kParamAmpRelease, 1700},   {233, kParamVelToAmp, 54},
  {233, kParamVelToFilter, 46},   {233, kParamStereoSpread, 44},   {233, kParamStereoDelay, 12},
  {233, kParamTapeSat, 20},   {233, kParamTapeMix, 26},   {233, kParamIns1Algo, 3},
  {233, kParamIns1A, 26},   {233, kParamIns1B, 22},   {233, kParamIns1C, 44},
  {233, kParamIns1Mix, 12},   {233, kParamMstAlgo, 2},   {233, kParamMstA, 24},
  {233, kParamMstB, 18},   {233, kParamMstC, 30},   {233, kParamMstMix, 10},
  {234, kParamEngine, 0},   {234, kParamPolyMode, 0},   {234, kParamGuitarBank, 3},
  {234, kParamGuitarLevel, -6},   {234, kParamOsc1Shape, 42},   {234, kParamOsc1PW, 34},
  {234, kParamOsc1Level, 84},   {234, kParamOsc2Shape, 38},   {234, kParamOsc2Pitch, 12},
  {234, kParamOsc2Level, 38},   {234, kParamSubLevel, 20},   {234, kParamNoiseLevel, 8},
  {234, kParamFilterType, 0},   {234, kParamFilterCutoff, 4400},   {234, kParamFilterReso, 26},
  {234, kParamFilterDrive, 40},   {234, kParamFilterEnv, 78},   {234, kParamFilterKeyTrack, 52},
  {234, kParamFiltAttack, 1},   {234, kParamFiltDecay, 300},   {234, kParamFiltSustain, 3.5},
  {234, kParamFiltRelease, 400},   {234, kParamAmpAttack, 1},   {234, kParamAmpDecay, 600},
  {234, kParamAmpSustain, 10},   {234, kParamAmpRelease, 600},   {234, kParamVelToAmp, 54},
  {234, kParamVelToFilter, 46},   {234, kParamStereoSpread, 44},   {234, kParamStereoDelay, 12},
  {234, kParamTapeSat, 20},   {234, kParamTapeMix, 26},   {234, kParamIns1Algo, 2},
  {234, kParamIns1A, 14},   {234, kParamIns1B, 24},   {234, kParamIns1C, 30},
  {234, kParamIns1Mix, 16},   {234, kParamMstAlgo, 2},   {234, kParamMstA, 24},
  {234, kParamMstB, 18},   {234, kParamMstC, 30},   {234, kParamMstMix, 10},
  {235, kParamEngine, 0},   {235, kParamPolyMode, 0},   {235, kParamGuitarBank, 1},
  {235, kParamGuitarLevel, -6},   {235, kParamOsc1Shape, 44},   {235, kParamOsc1PW, 32},
  {235, kParamOsc1Level, 84},   {235, kParamOsc2Shape, 38},   {235, kParamOsc2Pitch, 12},
  {235, kParamOsc2Level, 38},   {235, kParamSubLevel, 20},   {235, kParamNoiseLevel, 8},
  {235, kParamFilterType, 0},   {235, kParamFilterCutoff, 6400},   {235, kParamFilterReso, 18},
  {235, kParamFilterDrive, 24},   {235, kParamFilterEnv, 62},   {235, kParamFilterKeyTrack, 52},
  {235, kParamFiltAttack, 1},   {235, kParamFiltDecay, 650},   {235, kParamFiltSustain, 8.399999999999999},
  {235, kParamFiltRelease, 750},   {235, kParamAmpAttack, 2},   {235, kParamAmpDecay, 1300},
  {235, kParamAmpSustain, 24},   {235, kParamAmpRelease, 1500},   {235, kParamVelToAmp, 54},
  {235, kParamVelToFilter, 46},   {235, kParamStereoSpread, 44},   {235, kParamStereoDelay, 12},
  {235, kParamTapeSat, 20},   {235, kParamTapeMix, 26},   {235, kParamLfoRate, 4.6},
  {235, kParamLfoDest, 1},   {235, kParamLfoAmount, 42},   {235, kParamIns1Algo, 2},
  {235, kParamIns1A, 20},   {235, kParamIns1B, 30},   {235, kParamIns1C, 38},
  {235, kParamIns1Mix, 20},   {235, kParamMstAlgo, 2},   {235, kParamMstA, 24},
  {235, kParamMstB, 18},   {235, kParamMstC, 30},   {235, kParamMstMix, 10},
  {236, kParamEngine, 0},   {236, kParamPolyMode, 0},   {236, kParamGuitarBank, 1},
  {236, kParamGuitarLevel, -6},   {236, kParamOsc1Shape, 44},   {236, kParamOsc1PW, 32},
  {236, kParamOsc1Level, 84},   {236, kParamOsc2Shape, 38},   {236, kParamOsc2Pitch, 12},
  {236, kParamOsc2Level, 38},   {236, kParamSubLevel, 20},   {236, kParamNoiseLevel, 8},
  {236, kParamFilterType, 0},   {236, kParamFilterCutoff, 6000},   {236, kParamFilterReso, 20},
  {236, kParamFilterDrive, 28},   {236, kParamFilterEnv, 62},   {236, kParamFilterKeyTrack, 52},
  {236, kParamFiltAttack, 1},   {236, kParamFiltDecay, 600},   {236, kParamFiltSustain, 9.1},
  {236, kParamFiltRelease, 700},   {236, kParamAmpAttack, 2},   {236, kParamAmpDecay, 1200},
  {236, kParamAmpSustain, 26},   {236, kParamAmpRelease, 1400},   {236, kParamVelToAmp, 54},
  {236, kParamVelToFilter, 46},   {236, kParamStereoSpread, 44},   {236, kParamStereoDelay, 12},
  {236, kParamTapeSat, 20},   {236, kParamTapeMix, 44},   {236, kParamTapeWow, 40},
  {236, kParamTapeFlutter, 30},   {236, kParamIns1Algo, 3},   {236, kParamIns1A, 26},
  {236, kParamIns1B, 22},   {236, kParamIns1C, 44},   {236, kParamIns1Mix, 12},
  {236, kParamMstAlgo, 2},   {236, kParamMstA, 24},   {236, kParamMstB, 18},
  {236, kParamMstC, 30},   {236, kParamMstMix, 10},
  // winds
  {237, kParamEngine, 0},   {237, kParamPolyMode, 0},   {237, kParamPorta, 0},
  {237, kParamOsc1Shape, 54},   {237, kParamOsc1PW, 28},   {237, kParamOsc1Level, 82},
  {237, kParamOsc2Shape, 52},   {237, kParamOsc2Pitch, -12},   {237, kParamOsc2Level, 44},
  {237, kParamSubLevel, 26},   {237, kParamNoiseLevel, 32},   {237, kParamFilterType, 0},
  {237, kParamFilterCutoff, 5600},   {237, kParamFilterReso, 26},   {237, kParamFilterEnv, 38},
  {237, kParamFiltAttack, 12},   {237, kParamFiltDecay, 1100},   {237, kParamFiltSustain, 52},
  {237, kParamFiltRelease, 900},   {237, kParamAmpAttack, 30},   {237, kParamAmpDecay, 1200},
  {237, kParamAmpSustain, 76},   {237, kParamAmpRelease, 900},   {237, kParamVelToAmp, 46},
  {237, kParamVelToFilter, 30},   {237, kParamStereoSpread, 44},   {237, kParamStereoDelay, 20},
  {237, kParamTapeSat, 18},   {237, kParamTapeMix, 26},   {237, kParamIns1Algo, 2},
  {237, kParamIns1A, 16},   {237, kParamIns1B, 20},   {237, kParamIns1C, 28},
  {237, kParamIns1Mix, 12},   {237, kParamMstAlgo, 2},   {237, kParamMstA, 24},
  {237, kParamMstB, 18},   {237, kParamMstC, 30},   {237, kParamMstMix, 10},
  {238, kParamEngine, 0},   {238, kParamPolyMode, 0},   {238, kParamPorta, 0},
  {238, kParamOsc1Shape, 54},   {238, kParamOsc1PW, 50},   {238, kParamOsc1Level, 82},
  {238, kParamOsc2Shape, 52},   {238, kParamOsc2Pitch, -12},   {238, kParamOsc2Level, 44},
  {238, kParamSubLevel, 26},   {238, kParamNoiseLevel, 52},   {238, kParamFilterType, 0},
  {238, kParamFilterCutoff, 7200},   {238, kParamFilterReso, 18},   {238, kParamFilterEnv, 38},
  {238, kParamFiltAttack, 12},   {238, kParamFiltDecay, 1100},   {238, kParamFiltSustain, 52},
  {238, kParamFiltRelease, 1000},   {238, kParamAmpAttack, 40},   {238, kParamAmpDecay, 1200},
  {238, kParamAmpSustain, 76},   {238, kParamAmpRelease, 1000},   {238, kParamVelToAmp, 46},
  {238, kParamVelToFilter, 30},   {238, kParamStereoSpread, 44},   {238, kParamStereoDelay, 20},
  {238, kParamTapeSat, 18},   {238, kParamTapeMix, 26},   {238, kParamIns1Algo, 2},
  {238, kParamIns1A, 22},   {238, kParamIns1B, 24},   {238, kParamIns1C, 34},
  {238, kParamIns1Mix, 18},   {238, kParamMstAlgo, 2},   {238, kParamMstA, 24},
  {238, kParamMstB, 18},   {238, kParamMstC, 30},   {238, kParamMstMix, 10},
  {239, kParamEngine, 0},   {239, kParamPolyMode, 0},   {239, kParamPorta, 0},
  {239, kParamOsc1Shape, 54},   {239, kParamOsc1PW, 18},   {239, kParamOsc1Level, 82},
  {239, kParamOsc2Shape, 52},   {239, kParamOsc2Pitch, -12},   {239, kParamOsc2Level, 44},
  {239, kParamSubLevel, 26},   {239, kParamNoiseLevel, 18},   {239, kParamFilterType, 0},
  {239, kParamFilterCutoff, 6400},   {239, kParamFilterReso, 32},   {239, kParamFilterEnv, 38},
  {239, kParamFiltAttack, 12},   {239, kParamFiltDecay, 1100},   {239, kParamFiltSustain, 52},
  {239, kParamFiltRelease, 850},   {239, kParamAmpAttack, 24},   {239, kParamAmpDecay, 1200},
  {239, kParamAmpSustain, 76},   {239, kParamAmpRelease, 850},   {239, kParamVelToAmp, 46},
  {239, kParamVelToFilter, 30},   {239, kParamStereoSpread, 44},   {239, kParamStereoDelay, 20},
  {239, kParamTapeSat, 18},   {239, kParamTapeMix, 26},   {239, kParamIns1Algo, 1},
  {239, kParamIns1A, 18},   {239, kParamIns1B, 34},   {239, kParamIns1C, 26},
  {239, kParamIns1Mix, 12},   {239, kParamMstAlgo, 2},   {239, kParamMstA, 24},
  {239, kParamMstB, 18},   {239, kParamMstC, 30},   {239, kParamMstMix, 10},
  {240, kParamEngine, 0},   {240, kParamPolyMode, 0},   {240, kParamPorta, 0},
  {240, kParamOsc1Shape, 54},   {240, kParamOsc1PW, 22},   {240, kParamOsc1Level, 82},
  {240, kParamOsc2Shape, 52},   {240, kParamOsc2Pitch, -12},   {240, kParamOsc2Level, 44},
  {240, kParamSubLevel, 26},   {240, kParamNoiseLevel, 14},   {240, kParamFilterType, 0},
  {240, kParamFilterCutoff, 5000},   {240, kParamFilterReso, 30},   {240, kParamFilterEnv, 38},
  {240, kParamFiltAttack, 12},   {240, kParamFiltDecay, 1100},   {240, kParamFiltSustain, 52},
  {240, kParamFiltRelease, 900},   {240, kParamAmpAttack, 28},   {240, kParamAmpDecay, 1200},
  {240, kParamAmpSustain, 76},   {240, kParamAmpRelease, 900},   {240, kParamVelToAmp, 46},
  {240, kParamVelToFilter, 30},   {240, kParamStereoSpread, 44},   {240, kParamStereoDelay, 20},
  {240, kParamTapeSat, 18},   {240, kParamTapeMix, 26},   {240, kParamIns1Algo, 2},
  {240, kParamIns1A, 16},   {240, kParamIns1B, 20},   {240, kParamIns1C, 28},
  {240, kParamIns1Mix, 12},   {240, kParamMstAlgo, 2},   {240, kParamMstA, 24},
  {240, kParamMstB, 18},   {240, kParamMstC, 30},   {240, kParamMstMix, 10},
  {241, kParamEngine, 0},   {241, kParamPolyMode, 0},   {241, kParamPorta, 0},
  {241, kParamOsc1Shape, 54},   {241, kParamOsc1PW, 50},   {241, kParamOsc1Level, 82},
  {241, kParamOsc2Shape, 52},   {241, kParamOsc2Pitch, -12},   {241, kParamOsc2Level, 44},
  {241, kParamSubLevel, 26},   {241, kParamNoiseLevel, 64},   {241, kParamFilterType, 0},
  {241, kParamFilterCutoff, 6800},   {241, kParamFilterReso, 20},   {241, kParamFilterEnv, 38},
  {241, kParamFiltAttack, 12},   {241, kParamFiltDecay, 1100},   {241, kParamFiltSustain, 52},
  {241, kParamFiltRelease, 1200},   {241, kParamAmpAttack, 60},   {241, kParamAmpDecay, 1200},
  {241, kParamAmpSustain, 76},   {241, kParamAmpRelease, 1200},   {241, kParamVelToAmp, 46},
  {241, kParamVelToFilter, 30},   {241, kParamStereoSpread, 44},   {241, kParamStereoDelay, 20},
  {241, kParamTapeSat, 18},   {241, kParamTapeMix, 26},   {241, kParamIns1Algo, 2},
  {241, kParamIns1A, 22},   {241, kParamIns1B, 24},   {241, kParamIns1C, 34},
  {241, kParamIns1Mix, 18},   {241, kParamMstAlgo, 2},   {241, kParamMstA, 24},
  {241, kParamMstB, 18},   {241, kParamMstC, 30},   {241, kParamMstMix, 10},
  {242, kParamEngine, 0},   {242, kParamPolyMode, 0},   {242, kParamPorta, 0},
  {242, kParamOsc1Shape, 54},   {242, kParamOsc1PW, 16},   {242, kParamOsc1Level, 82},
  {242, kParamOsc2Shape, 52},   {242, kParamOsc2Pitch, -12},   {242, kParamOsc2Level, 44},
  {242, kParamSubLevel, 26},   {242, kParamNoiseLevel, 24},   {242, kParamFilterType, 0},
  {242, kParamFilterCutoff, 7800},   {242, kParamFilterReso, 34},   {242, kParamFilterEnv, 38},
  {242, kParamFiltAttack, 12},   {242, kParamFiltDecay, 1100},   {242, kParamFiltSustain, 52},
  {242, kParamFiltRelease, 800},   {242, kParamAmpAttack, 18},   {242, kParamAmpDecay, 1200},
  {242, kParamAmpSustain, 76},   {242, kParamAmpRelease, 800},   {242, kParamVelToAmp, 46},
  {242, kParamVelToFilter, 30},   {242, kParamStereoSpread, 44},   {242, kParamStereoDelay, 20},
  {242, kParamTapeSat, 18},   {242, kParamTapeMix, 26},   {242, kParamIns1Algo, 1},
  {242, kParamIns1A, 18},   {242, kParamIns1B, 34},   {242, kParamIns1C, 26},
  {242, kParamIns1Mix, 12},   {242, kParamMstAlgo, 2},   {242, kParamMstA, 24},
  {242, kParamMstB, 18},   {242, kParamMstC, 30},   {242, kParamMstMix, 10},
  {243, kParamEngine, 0},   {243, kParamPolyMode, 0},   {243, kParamPorta, 0},
  {243, kParamOsc1Shape, 54},   {243, kParamOsc1PW, 20},   {243, kParamOsc1Level, 82},
  {243, kParamOsc2Shape, 52},   {243, kParamOsc2Pitch, -12},   {243, kParamOsc2Level, 44},
  {243, kParamSubLevel, 26},   {243, kParamNoiseLevel, 16},   {243, kParamFilterType, 0},
  {243, kParamFilterCutoff, 3800},   {243, kParamFilterReso, 28},   {243, kParamFilterEnv, 38},
  {243, kParamFiltAttack, 12},   {243, kParamFiltDecay, 1100},   {243, kParamFiltSustain, 52},
  {243, kParamFiltRelease, 1000},   {243, kParamAmpAttack, 36},   {243, kParamAmpDecay, 1200},
  {243, kParamAmpSustain, 76},   {243, kParamAmpRelease, 1000},   {243, kParamVelToAmp, 46},
  {243, kParamVelToFilter, 30},   {243, kParamStereoSpread, 44},   {243, kParamStereoDelay, 20},
  {243, kParamTapeSat, 18},   {243, kParamTapeMix, 26},   {243, kParamIns1Algo, 2},
  {243, kParamIns1A, 16},   {243, kParamIns1B, 20},   {243, kParamIns1C, 28},
  {243, kParamIns1Mix, 12},   {243, kParamMstAlgo, 2},   {243, kParamMstA, 24},
  {243, kParamMstB, 18},   {243, kParamMstC, 30},   {243, kParamMstMix, 10},
  {244, kParamEngine, 0},   {244, kParamPolyMode, 0},   {244, kParamPorta, 0},
  {244, kParamOsc1Shape, 54},   {244, kParamOsc1PW, 28},   {244, kParamOsc1Level, 82},
  {244, kParamOsc2Shape, 52},   {244, kParamOsc2Pitch, -12},   {244, kParamOsc2Level, 44},
  {244, kParamSubLevel, 26},   {244, kParamNoiseLevel, 30},   {244, kParamFilterType, 0},
  {244, kParamFilterCutoff, 5400},   {244, kParamFilterReso, 28},   {244, kParamFilterEnv, 38},
  {244, kParamFiltAttack, 12},   {244, kParamFiltDecay, 1100},   {244, kParamFiltSustain, 52},
  {244, kParamFiltRelease, 950},   {244, kParamAmpAttack, 32},   {244, kParamAmpDecay, 1200},
  {244, kParamAmpSustain, 76},   {244, kParamAmpRelease, 950},   {244, kParamVelToAmp, 46},
  {244, kParamVelToFilter, 30},   {244, kParamStereoSpread, 44},   {244, kParamStereoDelay, 20},
  {244, kParamTapeSat, 30},   {244, kParamTapeMix, 40},   {244, kParamTapeWow, 36},
  {244, kParamIns1Algo, 2},   {244, kParamIns1A, 22},   {244, kParamIns1B, 24},
  {244, kParamIns1C, 34},   {244, kParamIns1Mix, 18},   {244, kParamMstAlgo, 2},
  {244, kParamMstA, 24},   {244, kParamMstB, 18},   {244, kParamMstC, 30},
  {244, kParamMstMix, 10},
  {245, kParamEngine, 0},   {245, kParamPolyMode, 0},   {245, kParamPorta, 0},
  {245, kParamOsc1Shape, 54},   {245, kParamOsc1PW, 44},   {245, kParamOsc1Level, 82},
  {245, kParamOsc2Shape, 52},   {245, kParamOsc2Pitch, -12},   {245, kParamOsc2Level, 44},
  {245, kParamSubLevel, 26},   {245, kParamNoiseLevel, 44},   {245, kParamFilterType, 0},
  {245, kParamFilterCutoff, 8600},   {245, kParamFilterReso, 22},   {245, kParamFilterEnv, 38},
  {245, kParamFiltAttack, 12},   {245, kParamFiltDecay, 1100},   {245, kParamFiltSustain, 52},
  {245, kParamFiltRelease, 900},   {245, kParamAmpAttack, 26},   {245, kParamAmpDecay, 1200},
  {245, kParamAmpSustain, 76},   {245, kParamAmpRelease, 900},   {245, kParamVelToAmp, 46},
  {245, kParamVelToFilter, 30},   {245, kParamStereoSpread, 44},   {245, kParamStereoDelay, 20},
  {245, kParamTapeSat, 18},   {245, kParamTapeMix, 26},   {245, kParamIns1Algo, 1},
  {245, kParamIns1A, 18},   {245, kParamIns1B, 34},   {245, kParamIns1C, 26},
  {245, kParamIns1Mix, 12},   {245, kParamMstAlgo, 2},   {245, kParamMstA, 24},
  {245, kParamMstB, 18},   {245, kParamMstC, 30},   {245, kParamMstMix, 10},
  {246, kParamEngine, 0},   {246, kParamPolyMode, 0},   {246, kParamPorta, 0},
  {246, kParamOsc1Shape, 54},   {246, kParamOsc1PW, 28},   {246, kParamOsc1Level, 82},
  {246, kParamOsc2Shape, 52},   {246, kParamOsc2Pitch, -12},   {246, kParamOsc2Level, 44},
  {246, kParamSubLevel, 26},   {246, kParamNoiseLevel, 26},   {246, kParamFilterType, 0},
  {246, kParamFilterCutoff, 6000},   {246, kParamFilterReso, 24},   {246, kParamFilterEnv, 38},
  {246, kParamFiltAttack, 12},   {246, kParamFiltDecay, 1100},   {246, kParamFiltSustain, 52},
  {246, kParamFiltRelease, 1100},   {246, kParamAmpAttack, 44},   {246, kParamAmpDecay, 1200},
  {246, kParamAmpSustain, 76},   {246, kParamAmpRelease, 1100},   {246, kParamVelToAmp, 46},
  {246, kParamVelToFilter, 30},   {246, kParamStereoSpread, 44},   {246, kParamStereoDelay, 20},
  {246, kParamTapeSat, 18},   {246, kParamTapeMix, 26},   {246, kParamUnisonVoices, 3},
  {246, kParamUnisonDetune, 18},   {246, kParamIns1Algo, 2},   {246, kParamIns1A, 16},
  {246, kParamIns1B, 20},   {246, kParamIns1C, 28},   {246, kParamIns1Mix, 12},
  {246, kParamMstAlgo, 2},   {246, kParamMstA, 24},   {246, kParamMstB, 18},
  {246, kParamMstC, 30},   {246, kParamMstMix, 10},
  {247, kParamEngine, 0},   {247, kParamPolyMode, 0},   {247, kParamOsc1Shape, 40},
  {247, kParamOsc1Level, 84},   {247, kParamOsc2Shape, 38},   {247, kParamOsc2Pitch, 0},
  {247, kParamOsc2Level, 76},   {247, kParamSubLevel, 24},   {247, kParamNoiseLevel, 4},
  {247, kParamFilterType, 0},   {247, kParamFilterCutoff, 4600},   {247, kParamFilterReso, 24},
  {247, kParamFilterEnv, 58},   {247, kParamFiltAttack, 90},   {247, kParamFiltDecay, 900},
  {247, kParamFiltSustain, 46},   {247, kParamFiltRelease, 900},   {247, kParamAmpAttack, 30},
  {247, kParamAmpDecay, 1200},   {247, kParamAmpSustain, 74},   {247, kParamAmpRelease, 900},
  {247, kParamUnisonVoices, 2},   {247, kParamUnisonDetune, 14},   {247, kParamVelToAmp, 40},
  {247, kParamVelToFilter, 46},   {247, kParamStereoSpread, 56},   {247, kParamTapeSat, 24},
  {247, kParamTapeMix, 32},   {247, kParamIns1Algo, 2},   {247, kParamIns1A, 22},
  {247, kParamIns1B, 24},   {247, kParamIns1C, 34},   {247, kParamIns1Mix, 18},
  {247, kParamMstAlgo, 2},   {247, kParamMstA, 24},   {247, kParamMstB, 18},
  {247, kParamMstC, 30},   {247, kParamMstMix, 10},
  {248, kParamEngine, 0},   {248, kParamPolyMode, 0},   {248, kParamOsc1Shape, 40},
  {248, kParamOsc1Level, 84},   {248, kParamOsc2Shape, 38},   {248, kParamOsc2Pitch, 0},
  {248, kParamOsc2Level, 76},   {248, kParamSubLevel, 24},   {248, kParamNoiseLevel, 4},
  {248, kParamFilterType, 0},   {248, kParamFilterCutoff, 5800},   {248, kParamFilterReso, 22},
  {248, kParamFilterEnv, 58},   {248, kParamFiltAttack, 70},   {248, kParamFiltDecay, 900},
  {248, kParamFiltSustain, 46},   {248, kParamFiltRelease, 850},   {248, kParamAmpAttack, 24},
  {248, kParamAmpDecay, 1200},   {248, kParamAmpSustain, 74},   {248, kParamAmpRelease, 850},
  {248, kParamUnisonVoices, 3},   {248, kParamUnisonDetune, 16},   {248, kParamVelToAmp, 40},
  {248, kParamVelToFilter, 46},   {248, kParamStereoSpread, 56},   {248, kParamTapeSat, 24},
  {248, kParamTapeMix, 32},   {248, kParamIns1Algo, 1},   {248, kParamIns1A, 18},
  {248, kParamIns1B, 34},   {248, kParamIns1C, 26},   {248, kParamIns1Mix, 12},
  {248, kParamMstAlgo, 2},   {248, kParamMstA, 24},   {248, kParamMstB, 18},
  {248, kParamMstC, 30},   {248, kParamMstMix, 10},
  {249, kParamEngine, 0},   {249, kParamPolyMode, 0},   {249, kParamOsc1Shape, 40},
  {249, kParamOsc1Level, 84},   {249, kParamOsc2Shape, 38},   {249, kParamOsc2Pitch, 0},
  {249, kParamOsc2Level, 76},   {249, kParamSubLevel, 24},   {249, kParamNoiseLevel, 4},
  {249, kParamFilterType, 0},   {249, kParamFilterCutoff, 3600},   {249, kParamFilterReso, 28},
  {249, kParamFilterEnv, 58},   {249, kParamFiltAttack, 110},   {249, kParamFiltDecay, 900},
  {249, kParamFiltSustain, 46},   {249, kParamFiltRelease, 1000},   {249, kParamAmpAttack, 40},
  {249, kParamAmpDecay, 1200},   {249, kParamAmpSustain, 74},   {249, kParamAmpRelease, 1000},
  {249, kParamUnisonVoices, 2},   {249, kParamUnisonDetune, 14},   {249, kParamVelToAmp, 40},
  {249, kParamVelToFilter, 46},   {249, kParamStereoSpread, 56},   {249, kParamTapeSat, 24},
  {249, kParamTapeMix, 32},   {249, kParamIns1Algo, 2},   {249, kParamIns1A, 16},
  {249, kParamIns1B, 20},   {249, kParamIns1C, 28},   {249, kParamIns1Mix, 12},
  {249, kParamMstAlgo, 2},   {249, kParamMstA, 24},   {249, kParamMstB, 18},
  {249, kParamMstC, 30},   {249, kParamMstMix, 10},
  {250, kParamEngine, 0},   {250, kParamPolyMode, 0},   {250, kParamOsc1Shape, 40},
  {250, kParamOsc1Level, 84},   {250, kParamOsc2Shape, 38},   {250, kParamOsc2Pitch, 0},
  {250, kParamOsc2Level, 76},   {250, kParamSubLevel, 24},   {250, kParamNoiseLevel, 4},
  {250, kParamFilterType, 0},   {250, kParamFilterCutoff, 5200},   {250, kParamFilterReso, 30},
  {250, kParamFilterEnv, 72},   {250, kParamFiltAttack, 50},   {250, kParamFiltDecay, 900},
  {250, kParamFiltSustain, 46},   {250, kParamFiltRelease, 500},   {250, kParamAmpAttack, 6},
  {250, kParamAmpDecay, 1200},   {250, kParamAmpSustain, 74},   {250, kParamAmpRelease, 500},
  {250, kParamUnisonVoices, 2},   {250, kParamUnisonDetune, 14},   {250, kParamVelToAmp, 40},
  {250, kParamVelToFilter, 46},   {250, kParamStereoSpread, 56},   {250, kParamTapeSat, 24},
  {250, kParamTapeMix, 32},   {250, kParamIns1Algo, 2},   {250, kParamIns1A, 22},
  {250, kParamIns1B, 24},   {250, kParamIns1C, 34},   {250, kParamIns1Mix, 18},
  {250, kParamMstAlgo, 2},   {250, kParamMstA, 24},   {250, kParamMstB, 18},
  {250, kParamMstC, 30},   {250, kParamMstMix, 10},
  {251, kParamEngine, 0},   {251, kParamPolyMode, 0},   {251, kParamOsc1Shape, 40},
  {251, kParamOsc1Level, 84},   {251, kParamOsc2Shape, 38},   {251, kParamOsc2Pitch, 0},
  {251, kParamOsc2Level, 76},   {251, kParamSubLevel, 24},   {251, kParamNoiseLevel, 8},
  {251, kParamFilterType, 0},   {251, kParamFilterCutoff, 4200},   {251, kParamFilterReso, 20},
  {251, kParamFilterEnv, 58},   {251, kParamFiltAttack, 140},   {251, kParamFiltDecay, 900},
  {251, kParamFiltSustain, 46},   {251, kParamFiltRelease, 1200},   {251, kParamAmpAttack, 60},
  {251, kParamAmpDecay, 1200},   {251, kParamAmpSustain, 74},   {251, kParamAmpRelease, 1200},
  {251, kParamUnisonVoices, 2},   {251, kParamUnisonDetune, 14},   {251, kParamVelToAmp, 40},
  {251, kParamVelToFilter, 46},   {251, kParamStereoSpread, 56},   {251, kParamTapeSat, 24},
  {251, kParamTapeMix, 32},   {251, kParamIns1Algo, 1},   {251, kParamIns1A, 18},
  {251, kParamIns1B, 34},   {251, kParamIns1C, 26},   {251, kParamIns1Mix, 12},
  {251, kParamMstAlgo, 2},   {251, kParamMstA, 24},   {251, kParamMstB, 18},
  {251, kParamMstC, 30},   {251, kParamMstMix, 10},
  {252, kParamEngine, 0},   {252, kParamPolyMode, 0},   {252, kParamOsc1Shape, 40},
  {252, kParamOsc1Level, 84},   {252, kParamOsc2Shape, 38},   {252, kParamOsc2Pitch, 0},
  {252, kParamOsc2Level, 76},   {252, kParamSubLevel, 24},   {252, kParamNoiseLevel, 4},
  {252, kParamFilterType, 0},   {252, kParamFilterCutoff, 6200},   {252, kParamFilterReso, 34},
  {252, kParamFilterEnv, 58},   {252, kParamFiltAttack, 60},   {252, kParamFiltDecay, 900},
  {252, kParamFiltSustain, 46},   {252, kParamFiltRelease, 700},   {252, kParamAmpAttack, 14},
  {252, kParamAmpDecay, 1200},   {252, kParamAmpSustain, 74},   {252, kParamAmpRelease, 700},
  {252, kParamUnisonVoices, 2},   {252, kParamUnisonDetune, 14},   {252, kParamVelToAmp, 40},
  {252, kParamVelToFilter, 46},   {252, kParamStereoSpread, 56},   {252, kParamTapeSat, 24},
  {252, kParamTapeMix, 32},   {252, kParamFilterDrive, 30},   {252, kParamIns1Algo, 2},
  {252, kParamIns1A, 16},   {252, kParamIns1B, 20},   {252, kParamIns1C, 28},
  {252, kParamIns1Mix, 12},   {252, kParamMstAlgo, 2},   {252, kParamMstA, 24},
  {252, kParamMstB, 18},   {252, kParamMstC, 30},   {252, kParamMstMix, 10},
  {253, kParamEngine, 0},   {253, kParamPolyMode, 0},   {253, kParamOsc1Shape, 40},
  {253, kParamOsc1Level, 84},   {253, kParamOsc2Shape, 38},   {253, kParamOsc2Pitch, 0},
  {253, kParamOsc2Level, 76},   {253, kParamSubLevel, 24},   {253, kParamNoiseLevel, 4},
  {253, kParamFilterType, 0},   {253, kParamFilterCutoff, 4800},   {253, kParamFilterReso, 26},
  {253, kParamFilterEnv, 58},   {253, kParamFiltAttack, 95},   {253, kParamFiltDecay, 900},
  {253, kParamFiltSustain, 46},   {253, kParamFiltRelease, 900},   {253, kParamAmpAttack, 32},
  {253, kParamAmpDecay, 1200},   {253, kParamAmpSustain, 74},   {253, kParamAmpRelease, 900},
  {253, kParamUnisonVoices, 2},   {253, kParamUnisonDetune, 14},   {253, kParamVelToAmp, 40},
  {253, kParamVelToFilter, 46},   {253, kParamStereoSpread, 56},   {253, kParamTapeSat, 34},
  {253, kParamTapeMix, 42},   {253, kParamTapeWow, 32},   {253, kParamIns1Algo, 2},
  {253, kParamIns1A, 22},   {253, kParamIns1B, 24},   {253, kParamIns1C, 34},
  {253, kParamIns1Mix, 18},   {253, kParamMstAlgo, 2},   {253, kParamMstA, 24},
  {253, kParamMstB, 18},   {253, kParamMstC, 30},   {253, kParamMstMix, 10},
  {254, kParamEngine, 0},   {254, kParamPolyMode, 0},   {254, kParamOsc1Shape, 40},
  {254, kParamOsc1Level, 84},   {254, kParamOsc2Shape, 38},   {254, kParamOsc2Pitch, 0},
  {254, kParamOsc2Level, 76},   {254, kParamSubLevel, 24},   {254, kParamNoiseLevel, 4},
  {254, kParamFilterType, 0},   {254, kParamFilterCutoff, 3800},   {254, kParamFilterReso, 36},
  {254, kParamFilterEnv, 40},   {254, kParamFiltAttack, 80},   {254, kParamFiltDecay, 900},
  {254, kParamFiltSustain, 46},   {254, kParamFiltRelease, 800},   {254, kParamAmpAttack, 26},
  {254, kParamAmpDecay, 1200},   {254, kParamAmpSustain, 74},   {254, kParamAmpRelease, 800},
  {254, kParamUnisonVoices, 2},   {254, kParamUnisonDetune, 14},   {254, kParamVelToAmp, 40},
  {254, kParamVelToFilter, 46},   {254, kParamStereoSpread, 56},   {254, kParamTapeSat, 24},
  {254, kParamTapeMix, 32},   {254, kParamIns1Algo, 1},   {254, kParamIns1A, 18},
  {254, kParamIns1B, 34},   {254, kParamIns1C, 26},   {254, kParamIns1Mix, 12},
  {254, kParamMstAlgo, 2},   {254, kParamMstA, 24},   {254, kParamMstB, 18},
  {254, kParamMstC, 30},   {254, kParamMstMix, 10},
  {255, kParamEngine, 0},   {255, kParamPolyMode, 0},   {255, kParamOsc1Shape, 40},
  {255, kParamOsc1Level, 84},   {255, kParamOsc2Shape, 38},   {255, kParamOsc2Pitch, 0},
  {255, kParamOsc2Level, 76},   {255, kParamSubLevel, 24},   {255, kParamNoiseLevel, 4},
  {255, kParamFilterType, 0},   {255, kParamFilterCutoff, 5000},   {255, kParamFilterReso, 24},
  {255, kParamFilterEnv, 58},   {255, kParamFiltAttack, 180},   {255, kParamFiltDecay, 900},
  {255, kParamFiltSustain, 46},   {255, kParamFiltRelease, 1400},   {255, kParamAmpAttack, 90},
  {255, kParamAmpDecay, 1200},   {255, kParamAmpSustain, 74},   {255, kParamAmpRelease, 1400},
  {255, kParamUnisonVoices, 4},   {255, kParamUnisonDetune, 20},   {255, kParamVelToAmp, 40},
  {255, kParamVelToFilter, 46},   {255, kParamStereoSpread, 56},   {255, kParamTapeSat, 24},
  {255, kParamTapeMix, 32},   {255, kParamIns1Algo, 2},   {255, kParamIns1A, 16},
  {255, kParamIns1B, 20},   {255, kParamIns1C, 28},   {255, kParamIns1Mix, 12},
  {255, kParamMstAlgo, 2},   {255, kParamMstA, 24},   {255, kParamMstB, 18},
  {255, kParamMstC, 30},   {255, kParamMstMix, 10},
  {256, kParamEngine, 0},   {256, kParamPolyMode, 0},   {256, kParamOsc1Shape, 40},
  {256, kParamOsc1Level, 84},   {256, kParamOsc2Shape, 38},   {256, kParamOsc2Pitch, 0},
  {256, kParamOsc2Level, 76},   {256, kParamSubLevel, 24},   {256, kParamNoiseLevel, 4},
  {256, kParamFilterType, 0},   {256, kParamFilterCutoff, 6600},   {256, kParamFilterReso, 30},
  {256, kParamFilterEnv, 58},   {256, kParamFiltAttack, 44},   {256, kParamFiltDecay, 900},
  {256, kParamFiltSustain, 46},   {256, kParamFiltRelease, 650},   {256, kParamAmpAttack, 12},
  {256, kParamAmpDecay, 1200},   {256, kParamAmpSustain, 74},   {256, kParamAmpRelease, 650},
  {256, kParamUnisonVoices, 2},   {256, kParamUnisonDetune, 14},   {256, kParamVelToAmp, 40},
  {256, kParamVelToFilter, 54},   {256, kParamStereoSpread, 56},   {256, kParamTapeSat, 24},
  {256, kParamTapeMix, 32},   {256, kParamIns1Algo, 2},   {256, kParamIns1A, 22},
  {256, kParamIns1B, 24},   {256, kParamIns1C, 34},   {256, kParamIns1Mix, 18},
  {256, kParamMstAlgo, 2},   {256, kParamMstA, 24},   {256, kParamMstB, 18},
  {256, kParamMstC, 30},   {256, kParamMstMix, 10},
  // sfx
  {257, kParamEngine, 0},   {257, kParamPolyMode, 0},   {257, kParamOsc1Shape, 56},
  {257, kParamOsc1Level, 80},   {257, kParamOsc2Shape, 58},   {257, kParamOsc2Pitch, 7},
  {257, kParamOsc2Level, 70},   {257, kParamOscRing, 1},   {257, kParamNoiseLevel, 34},
  {257, kParamSubLevel, 10},   {257, kParamFilterType, 0},   {257, kParamFilterCutoff, 4800},
  {257, kParamFilterReso, 44},   {257, kParamFilterEnv, 64},   {257, kParamFiltAttack, 20},
  {257, kParamFiltDecay, 2000},   {257, kParamFiltSustain, 40},   {257, kParamFiltRelease, 2400},
  {257, kParamAmpAttack, 20},   {257, kParamAmpDecay, 2400},   {257, kParamAmpSustain, 62},
  {257, kParamAmpRelease, 2400},   {257, kParamLfoRate, 0.6},   {257, kParamLfoDest, 16},
  {257, kParamLfoAmount, 72},   {257, kParamStereoSpread, 86},   {257, kParamStereoDelay, 40},
  {257, kParamTapeSat, 30},   {257, kParamTapeMix, 40},   {257, kParamIns1Algo, 3},
  {257, kParamIns1A, 48},   {257, kParamIns1B, 38},   {257, kParamIns1C, 58},
  {257, kParamIns1Mix, 26},   {257, kParamMstAlgo, 2},   {257, kParamMstA, 24},
  {257, kParamMstB, 18},   {257, kParamMstC, 30},   {257, kParamMstMix, 10},
  {258, kParamEngine, 0},   {258, kParamPolyMode, 0},   {258, kParamOsc1Shape, 56},
  {258, kParamOsc1Level, 30},   {258, kParamOsc2Shape, 58},   {258, kParamOsc2Pitch, 7},
  {258, kParamOsc2Level, 70},   {258, kParamOscRing, 1},   {258, kParamNoiseLevel, 64},
  {258, kParamSubLevel, 10},   {258, kParamFilterType, 0},   {258, kParamFilterCutoff, 6200},
  {258, kParamFilterReso, 30},   {258, kParamFilterEnv, 64},   {258, kParamFiltAttack, 400},
  {258, kParamFiltDecay, 2000},   {258, kParamFiltSustain, 40},   {258, kParamFiltRelease, 3200},
  {258, kParamAmpAttack, 400},   {258, kParamAmpDecay, 2400},   {258, kParamAmpSustain, 62},
  {258, kParamAmpRelease, 3200},   {258, kParamLfoRate, 0.6},   {258, kParamLfoDest, 16},
  {258, kParamLfoAmount, 72},   {258, kParamStereoSpread, 86},   {258, kParamStereoDelay, 40},
  {258, kParamTapeSat, 30},   {258, kParamTapeMix, 40},   {258, kParamIns1Algo, 4},
  {258, kParamIns1A, 34},   {258, kParamIns1B, 42},   {258, kParamIns1C, 22},
  {258, kParamIns1Mix, 22},   {258, kParamMstAlgo, 2},   {258, kParamMstA, 24},
  {258, kParamMstB, 18},   {258, kParamMstC, 30},   {258, kParamMstMix, 10},
  {259, kParamEngine, 0},   {259, kParamPolyMode, 0},   {259, kParamOsc1Shape, 56},
  {259, kParamOsc1Level, 80},   {259, kParamOsc2Shape, 58},   {259, kParamOsc2Pitch, 7},
  {259, kParamOsc2Level, 70},   {259, kParamOscRing, 1},   {259, kParamNoiseLevel, 34},
  {259, kParamSubLevel, 10},   {259, kParamFilterType, 0},   {259, kParamFilterCutoff, 3200},
  {259, kParamFilterReso, 52},   {259, kParamFilterEnv, 84},   {259, kParamFiltAttack, 200},
  {259, kParamFiltDecay, 2000},   {259, kParamFiltSustain, 40},   {259, kParamFiltRelease, 2800},
  {259, kParamAmpAttack, 200},   {259, kParamAmpDecay, 2400},   {259, kParamAmpSustain, 62},
  {259, kParamAmpRelease, 2800},   {259, kParamLfoRate, 1.6},   {259, kParamLfoDest, 16},
  {259, kParamLfoAmount, 72},   {259, kParamStereoSpread, 86},   {259, kParamStereoDelay, 40},
  {259, kParamTapeSat, 30},   {259, kParamTapeMix, 40},   {259, kParamIns1Algo, 2},
  {259, kParamIns1A, 24},   {259, kParamIns1B, 46},   {259, kParamIns1C, 56},
  {259, kParamIns1Mix, 30},   {259, kParamMstAlgo, 2},   {259, kParamMstA, 24},
  {259, kParamMstB, 18},   {259, kParamMstC, 30},   {259, kParamMstMix, 10},
  {260, kParamEngine, 0},   {260, kParamPolyMode, 0},   {260, kParamOsc1Shape, 56},
  {260, kParamOsc1Level, 80},   {260, kParamOsc2Shape, 58},   {260, kParamOsc2Pitch, 7},
  {260, kParamOsc2Level, 70},   {260, kParamOscRing, 1},   {260, kParamNoiseLevel, 34},
  {260, kParamSubLevel, 10},   {260, kParamFilterType, 0},   {260, kParamFilterCutoff, 5400},
  {260, kParamFilterReso, 48},   {260, kParamFilterEnv, 64},   {260, kParamFiltAttack, 30},
  {260, kParamFiltDecay, 2000},   {260, kParamFiltSustain, 40},   {260, kParamFiltRelease, 2600},
  {260, kParamAmpAttack, 30},   {260, kParamAmpDecay, 2400},   {260, kParamAmpSustain, 62},
  {260, kParamAmpRelease, 2600},   {260, kParamLfoRate, 2.4},   {260, kParamLfoDest, 16},
  {260, kParamLfoAmount, 88},   {260, kParamStereoSpread, 86},   {260, kParamStereoDelay, 40},
  {260, kParamTapeSat, 30},   {260, kParamTapeMix, 40},   {260, kParamIns1Algo, 3},
  {260, kParamIns1A, 48},   {260, kParamIns1B, 38},   {260, kParamIns1C, 58},
  {260, kParamIns1Mix, 26},   {260, kParamMstAlgo, 2},   {260, kParamMstA, 24},
  {260, kParamMstB, 18},   {260, kParamMstC, 30},   {260, kParamMstMix, 10},
  {261, kParamEngine, 0},   {261, kParamPolyMode, 0},   {261, kParamOsc1Shape, 56},
  {261, kParamOsc1Level, 80},   {261, kParamOsc2Shape, 58},   {261, kParamOsc2Pitch, 7},
  {261, kParamOsc2Level, 70},   {261, kParamOscRing, 1},   {261, kParamNoiseLevel, 34},
  {261, kParamSubLevel, 10},   {261, kParamFilterType, 0},   {261, kParamFilterCutoff, 5600},
  {261, kParamFilterReso, 36},   {261, kParamFilterEnv, 64},   {261, kParamFiltAttack, 60},
  {261, kParamFiltDecay, 2000},   {261, kParamFiltSustain, 40},   {261, kParamFiltRelease, 2400},
  {261, kParamAmpAttack, 60},   {261, kParamAmpDecay, 2400},   {261, kParamAmpSustain, 62},
  {261, kParamAmpRelease, 2400},   {261, kParamLfoRate, 0.6},   {261, kParamLfoDest, 16},
  {261, kParamLfoAmount, 72},   {261, kParamStereoSpread, 86},   {261, kParamStereoDelay, 40},
  {261, kParamTapeSat, 30},   {261, kParamTapeMix, 56},   {261, kParamTapeWow, 72},
  {261, kParamTapeFlutter, 64},   {261, kParamIns1Algo, 4},   {261, kParamIns1A, 34},
  {261, kParamIns1B, 42},   {261, kParamIns1C, 22},   {261, kParamIns1Mix, 22},
  {261, kParamMstAlgo, 2},   {261, kParamMstA, 24},   {261, kParamMstB, 18},
  {261, kParamMstC, 30},   {261, kParamMstMix, 10},
  {262, kParamEngine, 0},   {262, kParamPolyMode, 0},   {262, kParamOsc1Shape, 56},
  {262, kParamOsc1Level, 80},   {262, kParamOsc2Shape, 58},   {262, kParamOsc2Pitch, 7},
  {262, kParamOsc2Level, 70},   {262, kParamOscRing, 1},   {262, kParamNoiseLevel, 34},
  {262, kParamSubLevel, 10},   {262, kParamFilterType, 0},   {262, kParamFilterCutoff, 5000},
  {262, kParamFilterReso, 40},   {262, kParamFilterEnv, 64},   {262, kParamFiltAttack, 320},
  {262, kParamFiltDecay, 2000},   {262, kParamFiltSustain, 40},   {262, kParamFiltRelease, 3400},
  {262, kParamAmpAttack, 320},   {262, kParamAmpDecay, 2400},   {262, kParamAmpSustain, 62},
  {262, kParamAmpRelease, 3400},   {262, kParamLfoRate, 0.6},   {262, kParamLfoDest, 16},
  {262, kParamLfoAmount, 72},   {262, kParamStereoSpread, 86},   {262, kParamStereoDelay, 40},
  {262, kParamTapeSat, 30},   {262, kParamTapeMix, 40},   {262, kParamIns1Algo, 2},
  {262, kParamIns1A, 24},   {262, kParamIns1B, 46},   {262, kParamIns1C, 56},
  {262, kParamIns1Mix, 30},   {262, kParamMstAlgo, 2},   {262, kParamMstA, 24},
  {262, kParamMstB, 18},   {262, kParamMstC, 30},   {262, kParamMstMix, 10},
  {263, kParamEngine, 0},   {263, kParamPolyMode, 0},   {263, kParamOsc1Shape, 56},
  {263, kParamOsc1Level, 80},   {263, kParamOsc2Shape, 58},   {263, kParamOsc2Pitch, 7},
  {263, kParamOsc2Level, 70},   {263, kParamOscRing, 1},   {263, kParamNoiseLevel, 34},
  {263, kParamSubLevel, 10},   {263, kParamFilterType, 0},   {263, kParamFilterCutoff, 3800},
  {263, kParamFilterReso, 56},   {263, kParamFilterEnv, 64},   {263, kParamFiltAttack, 10},
  {263, kParamFiltDecay, 2000},   {263, kParamFiltSustain, 40},   {263, kParamFiltRelease, 2000},
  {263, kParamAmpAttack, 10},   {263, kParamAmpDecay, 2400},   {263, kParamAmpSustain, 62},
  {263, kParamAmpRelease, 2000},   {263, kParamLfoRate, 0.6},   {263, kParamLfoDest, 16},
  {263, kParamLfoAmount, 72},   {263, kParamStereoSpread, 86},   {263, kParamStereoDelay, 40},
  {263, kParamTapeSat, 30},   {263, kParamTapeMix, 52},   {263, kParamTapeHiss, 48},
  {263, kParamTapePrint, 40},   {263, kParamIns1Algo, 3},   {263, kParamIns1A, 48},
  {263, kParamIns1B, 38},   {263, kParamIns1C, 58},   {263, kParamIns1Mix, 26},
  {263, kParamMstAlgo, 2},   {263, kParamMstA, 24},   {263, kParamMstB, 18},
  {263, kParamMstC, 30},   {263, kParamMstMix, 10},
  {264, kParamEngine, 0},   {264, kParamPolyMode, 0},   {264, kParamOsc1Shape, 56},
  {264, kParamOsc1Level, 80},   {264, kParamOsc2Shape, 58},   {264, kParamOsc2Pitch, 7},
  {264, kParamOsc2Level, 70},   {264, kParamOscRing, 0},   {264, kParamNoiseLevel, 34},
  {264, kParamSubLevel, 10},   {264, kParamFilterType, 0},   {264, kParamFilterCutoff, 2600},
  {264, kParamFilterReso, 46},   {264, kParamFilterEnv, 64},   {264, kParamFiltAttack, 4},
  {264, kParamFiltDecay, 2000},   {264, kParamFiltSustain, 40},   {264, kParamFiltRelease, 1800},
  {264, kParamAmpAttack, 4},   {264, kParamAmpDecay, 2400},   {264, kParamAmpSustain, 62},
  {264, kParamAmpRelease, 1800},   {264, kParamLfoRate, 0.6},   {264, kParamLfoDest, 16},
  {264, kParamLfoAmount, 72},   {264, kParamStereoSpread, 86},   {264, kParamStereoDelay, 40},
  {264, kParamTapeSat, 30},   {264, kParamTapeMix, 40},   {264, kParamPitchAmount, -24},
  {264, kParamPitchDecay, 600},   {264, kParamIns1Algo, 4},   {264, kParamIns1A, 34},
  {264, kParamIns1B, 42},   {264, kParamIns1C, 22},   {264, kParamIns1Mix, 22},
  {264, kParamMstAlgo, 2},   {264, kParamMstA, 24},   {264, kParamMstB, 18},
  {264, kParamMstC, 30},   {264, kParamMstMix, 10},
  {265, kParamEngine, 0},   {265, kParamPolyMode, 0},   {265, kParamOsc1Shape, 56},
  {265, kParamOsc1Level, 80},   {265, kParamOsc2Shape, 58},   {265, kParamOsc2Pitch, 7},
  {265, kParamOsc2Level, 70},   {265, kParamOscRing, 0},   {265, kParamNoiseLevel, 22},
  {265, kParamSubLevel, 10},   {265, kParamFilterType, 0},   {265, kParamFilterCutoff, 7800},
  {265, kParamFilterReso, 38},   {265, kParamFilterEnv, 64},   {265, kParamFiltAttack, 240},
  {265, kParamFiltDecay, 2000},   {265, kParamFiltSustain, 40},   {265, kParamFiltRelease, 3000},
  {265, kParamAmpAttack, 240},   {265, kParamAmpDecay, 2400},   {265, kParamAmpSustain, 62},
  {265, kParamAmpRelease, 3000},   {265, kParamLfoRate, 0.6},   {265, kParamLfoDest, 16},
  {265, kParamLfoAmount, 72},   {265, kParamStereoSpread, 86},   {265, kParamStereoDelay, 40},
  {265, kParamTapeSat, 30},   {265, kParamTapeMix, 40},   {265, kParamIns1Algo, 2},
  {265, kParamIns1A, 24},   {265, kParamIns1B, 46},   {265, kParamIns1C, 56},
  {265, kParamIns1Mix, 30},   {265, kParamMstAlgo, 2},   {265, kParamMstA, 24},
  {265, kParamMstB, 18},   {265, kParamMstC, 30},   {265, kParamMstMix, 10},
  {266, kParamEngine, 0},   {266, kParamPolyMode, 0},   {266, kParamOsc1Shape, 56},
  {266, kParamOsc1Level, 80},   {266, kParamOsc2Shape, 58},   {266, kParamOsc2Pitch, 7},
  {266, kParamOsc2Level, 70},   {266, kParamOscRing, 1},   {266, kParamNoiseLevel, 34},
  {266, kParamSubLevel, 10},   {266, kParamFilterType, 0},   {266, kParamFilterCutoff, 6800},
  {266, kParamFilterReso, 50},   {266, kParamFilterEnv, 64},   {266, kParamFiltAttack, 8},
  {266, kParamFiltDecay, 2000},   {266, kParamFiltSustain, 40},   {266, kParamFiltRelease, 2200},
  {266, kParamAmpAttack, 8},   {266, kParamAmpDecay, 2400},   {266, kParamAmpSustain, 62},
  {266, kParamAmpRelease, 2200},   {266, kParamLfoRate, 0.6},   {266, kParamLfoDest, 16},
  {266, kParamLfoAmount, 72},   {266, kParamStereoSpread, 86},   {266, kParamStereoDelay, 40},
  {266, kParamTapeSat, 30},   {266, kParamTapeMix, 40},   {266, kParamPitchAmount, -24},
  {266, kParamPitchDecay, 900},   {266, kParamIns1Algo, 3},   {266, kParamIns1A, 48},
  {266, kParamIns1B, 38},   {266, kParamIns1C, 58},   {266, kParamIns1Mix, 26},
  {266, kParamMstAlgo, 2},   {266, kParamMstA, 24},   {266, kParamMstB, 18},
  {266, kParamMstC, 30},   {266, kParamMstMix, 10},
  // vocal
  {267, kParamEngine, 0},   {267, kParamPolyMode, 0},   {267, kParamOsc1Shape, 26},
  {267, kParamOsc1Level, 40},   {267, kParamOsc2Level, 0},   {267, kParamOsc3Enable, 1},
  {267, kParamOsc3Source, 1},   {267, kParamOsc3Morph, 2.8},   {267, kParamOsc3Level, 88},
  {267, kParamOsc3Pitch, 12},   {267, kParamSubLevel, 16},   {267, kParamNoiseLevel, 8},
  {267, kParamFilterType, 0},   {267, kParamFilterCutoff, 6000},   {267, kParamFilterReso, 16},
  {267, kParamFilterEnv, 22},   {267, kParamFiltAttack, 154},   {267, kParamFiltDecay, 2000},
  {267, kParamFiltSustain, 58},   {267, kParamFiltRelease, 2400},   {267, kParamAmpAttack, 220},
  {267, kParamAmpDecay, 2200},   {267, kParamAmpSustain, 78},   {267, kParamAmpRelease, 2400},
  {267, kParamUnisonVoices, 4},   {267, kParamUnisonDetune, 16},   {267, kParamStereoSpread, 82},
  {267, kParamTapeSat, 18},   {267, kParamTapeMix, 30},   {267, kParamIns1Algo, 2},
  {267, kParamIns1A, 30},   {267, kParamIns1B, 20},   {267, kParamIns1C, 40},
  {267, kParamIns1Mix, 34},   {267, kParamMstAlgo, 2},   {267, kParamMstA, 24},
  {267, kParamMstB, 18},   {267, kParamMstC, 30},   {267, kParamMstMix, 10},
  {268, kParamEngine, 0},   {268, kParamPolyMode, 0},   {268, kParamOsc1Shape, 26},
  {268, kParamOsc1Level, 40},   {268, kParamOsc2Level, 0},   {268, kParamOsc3Enable, 1},
  {268, kParamOsc3Source, 1},   {268, kParamOsc3Morph, 5.4},   {268, kParamOsc3Level, 88},
  {268, kParamOsc3Pitch, 12},   {268, kParamSubLevel, 16},   {268, kParamNoiseLevel, 8},
  {268, kParamFilterType, 0},   {268, kParamFilterCutoff, 5200},   {268, kParamFilterReso, 18},
  {268, kParamFilterEnv, 22},   {268, kParamFiltAttack, 182},   {268, kParamFiltDecay, 2000},
  {268, kParamFiltSustain, 58},   {268, kParamFiltRelease, 2600},   {268, kParamAmpAttack, 260},
  {268, kParamAmpDecay, 2200},   {268, kParamAmpSustain, 78},   {268, kParamAmpRelease, 2600},
  {268, kParamUnisonVoices, 4},   {268, kParamUnisonDetune, 16},   {268, kParamStereoSpread, 82},
  {268, kParamTapeSat, 18},   {268, kParamTapeMix, 30},   {268, kParamIns1Algo, 3},
  {268, kParamIns1A, 42},   {268, kParamIns1B, 26},   {268, kParamIns1C, 34},
  {268, kParamIns1Mix, 30},   {268, kParamMstAlgo, 2},   {268, kParamMstA, 24},
  {268, kParamMstB, 18},   {268, kParamMstC, 30},   {268, kParamMstMix, 10},
  {269, kParamEngine, 0},   {269, kParamPolyMode, 0},   {269, kParamOsc1Shape, 26},
  {269, kParamOsc1Level, 40},   {269, kParamOsc2Level, 0},   {269, kParamOsc3Enable, 1},
  {269, kParamOsc3Source, 1},   {269, kParamOsc3Morph, 3},   {269, kParamOsc3Level, 88},
  {269, kParamOsc3Pitch, 12},   {269, kParamSubLevel, 16},   {269, kParamNoiseLevel, 8},
  {269, kParamFilterType, 0},   {269, kParamFilterCutoff, 5600},   {269, kParamFilterReso, 20},
  {269, kParamFilterEnv, 22},   {269, kParamFiltAttack, 210},   {269, kParamFiltDecay, 2000},
  {269, kParamFiltSustain, 58},   {269, kParamFiltRelease, 2800},   {269, kParamAmpAttack, 300},
  {269, kParamAmpDecay, 2200},   {269, kParamAmpSustain, 78},   {269, kParamAmpRelease, 2800},
  {269, kParamUnisonVoices, 6},   {269, kParamUnisonDetune, 16},   {269, kParamStereoSpread, 90},
  {269, kParamTapeSat, 18},   {269, kParamTapeMix, 30},   {269, kParamIns1Algo, 1},
  {269, kParamIns1A, 20},   {269, kParamIns1B, 30},   {269, kParamIns1C, 24},
  {269, kParamIns1Mix, 28},   {269, kParamMstAlgo, 2},   {269, kParamMstA, 24},
  {269, kParamMstB, 18},   {269, kParamMstC, 30},   {269, kParamMstMix, 10},
  {270, kParamEngine, 0},   {270, kParamPolyMode, 0},   {270, kParamOsc1Shape, 26},
  {270, kParamOsc1Level, 40},   {270, kParamOsc2Level, 0},   {270, kParamOsc3Enable, 1},
  {270, kParamOsc3Source, 1},   {270, kParamOsc3Morph, 4.2},   {270, kParamOsc3Level, 88},
  {270, kParamOsc3Pitch, 12},   {270, kParamSubLevel, 16},   {270, kParamNoiseLevel, 8},
  {270, kParamFilterType, 0},   {270, kParamFilterCutoff, 5400},   {270, kParamFilterReso, 22},
  {270, kParamFilterEnv, 22},   {270, kParamFiltAttack, 168},   {270, kParamFiltDecay, 2000},
  {270, kParamFiltSustain, 58},   {270, kParamFiltRelease, 2500},   {270, kParamAmpAttack, 240},
  {270, kParamAmpDecay, 2200},   {270, kParamAmpSustain, 78},   {270, kParamAmpRelease, 2500},
  {270, kParamUnisonVoices, 4},   {270, kParamUnisonDetune, 16},   {270, kParamStereoSpread, 82},
  {270, kParamTapeSat, 18},   {270, kParamTapeMix, 52},   {270, kParamTapeWow, 54},
  {270, kParamTapeFlutter, 44},   {270, kParamIns1Algo, 2},   {270, kParamIns1A, 30},
  {270, kParamIns1B, 20},   {270, kParamIns1C, 40},   {270, kParamIns1Mix, 34},
  {270, kParamMstAlgo, 2},   {270, kParamMstA, 24},   {270, kParamMstB, 18},
  {270, kParamMstC, 30},   {270, kParamMstMix, 10},
  // bass
  {271, kParamEngine, 0},   {271, kParamPolyMode, 0},   {271, kParamPorta, 0},
  {271, kParamOsc1Shape, 38},   {271, kParamOsc1Level, 88},   {271, kParamOsc2Shape, 36},
  {271, kParamOsc2Pitch, -12},   {271, kParamOsc2Level, 54},   {271, kParamSubLevel, 62},
  {271, kParamNoiseLevel, 2},   {271, kParamFilterType, 0},   {271, kParamFilterCutoff, 1800},
  {271, kParamFilterReso, 26},   {271, kParamFilterDrive, 0},   {271, kParamFilterEnv, 46},
  {271, kParamFiltAttack, 2},   {271, kParamFiltDecay, 700},   {271, kParamFiltSustain, 24},
  {271, kParamFiltRelease, 294},   {271, kParamAmpAttack, 3},   {271, kParamAmpDecay, 700},
  {271, kParamAmpSustain, 40},   {271, kParamAmpRelease, 420},   {271, kParamVelToAmp, 38},
  {271, kParamVelToFilter, 40},   {271, kParamStereoSpread, 0},   {271, kParamStereoDelay, 0},
  {271, kParamTapeSat, 26},   {271, kParamTapeMix, 32},   {271, kParamIns1Algo, 1},
  {271, kParamIns1A, 24},   {271, kParamIns1B, 40},   {271, kParamIns1C, 28},
  {271, kParamIns1Mix, 16},   {271, kParamMstAlgo, 2},   {271, kParamMstA, 24},
  {271, kParamMstB, 18},   {271, kParamMstC, 30},   {271, kParamMstMix, 10},
  {272, kParamEngine, 0},   {272, kParamPolyMode, 0},   {272, kParamPorta, 0},
  {272, kParamOsc1Shape, 38},   {272, kParamOsc1Level, 88},   {272, kParamOsc2Shape, 36},
  {272, kParamOsc2Pitch, -12},   {272, kParamOsc2Level, 54},   {272, kParamSubLevel, 62},
  {272, kParamNoiseLevel, 2},   {272, kParamFilterType, 0},   {272, kParamFilterCutoff, 3600},
  {272, kParamFilterReso, 34},   {272, kParamFilterDrive, 22},   {272, kParamFilterEnv, 46},
  {272, kParamFiltAttack, 2},   {272, kParamFiltDecay, 500},   {272, kParamFiltSustain, 20.4},
  {272, kParamFiltRelease, 266},   {272, kParamAmpAttack, 2},   {272, kParamAmpDecay, 500},
  {272, kParamAmpSustain, 34},   {272, kParamAmpRelease, 380},   {272, kParamVelToAmp, 38},
  {272, kParamVelToFilter, 40},   {272, kParamStereoSpread, 0},   {272, kParamStereoDelay, 0},
  {272, kParamTapeSat, 26},   {272, kParamTapeMix, 32},   {272, kParamIns1Algo, 1},
  {272, kParamIns1A, 32},   {272, kParamIns1B, 46},   {272, kParamIns1C, 34},
  {272, kParamIns1Mix, 22},   {272, kParamMstAlgo, 2},   {272, kParamMstA, 24},
  {272, kParamMstB, 18},   {272, kParamMstC, 30},   {272, kParamMstMix, 10},
  {273, kParamEngine, 0},   {273, kParamPolyMode, 0},   {273, kParamPorta, 0},
  {273, kParamOsc1Shape, 38},   {273, kParamOsc1Level, 88},   {273, kParamOsc2Shape, 36},
  {273, kParamOsc2Pitch, -12},   {273, kParamOsc2Level, 54},   {273, kParamSubLevel, 62},
  {273, kParamNoiseLevel, 2},   {273, kParamFilterType, 0},   {273, kParamFilterCutoff, 1200},
  {273, kParamFilterReso, 22},   {273, kParamFilterDrive, 0},   {273, kParamFilterEnv, 46},
  {273, kParamFiltAttack, 2},   {273, kParamFiltDecay, 900},   {273, kParamFiltSustain, 28.799999999999997},
  {273, kParamFiltRelease, 364},   {273, kParamAmpAttack, 5},   {273, kParamAmpDecay, 900},
  {273, kParamAmpSustain, 48},   {273, kParamAmpRelease, 520},   {273, kParamVelToAmp, 38},
  {273, kParamVelToFilter, 40},   {273, kParamStereoSpread, 0},   {273, kParamStereoDelay, 0},
  {273, kParamTapeSat, 26},   {273, kParamTapeMix, 32},   {273, kParamIns1Algo, 4},
  {273, kParamIns1A, 18},   {273, kParamIns1B, 28},   {273, kParamIns1C, 10},
  {273, kParamIns1Mix, 12},   {273, kParamMstAlgo, 2},   {273, kParamMstA, 24},
  {273, kParamMstB, 18},   {273, kParamMstC, 30},   {273, kParamMstMix, 10},
  {274, kParamEngine, 0},   {274, kParamPolyMode, 0},   {274, kParamPorta, 0},
  {274, kParamOsc1Shape, 38},   {274, kParamOsc1Level, 88},   {274, kParamOsc2Shape, 36},
  {274, kParamOsc2Pitch, -12},   {274, kParamOsc2Level, 54},   {274, kParamSubLevel, 62},
  {274, kParamNoiseLevel, 2},   {274, kParamFilterType, 0},   {274, kParamFilterCutoff, 2800},
  {274, kParamFilterReso, 44},   {274, kParamFilterDrive, 58},   {274, kParamFilterEnv, 46},
  {274, kParamFiltAttack, 2},   {274, kParamFiltDecay, 480},   {274, kParamFiltSustain, 18},
  {274, kParamFiltRelease, 251.99999999999997},   {274, kParamAmpAttack, 2},   {274, kParamAmpDecay, 480},
  {274, kParamAmpSustain, 30},   {274, kParamAmpRelease, 360},   {274, kParamVelToAmp, 38},
  {274, kParamVelToFilter, 40},   {274, kParamStereoSpread, 0},   {274, kParamStereoDelay, 0},
  {274, kParamTapeSat, 40},   {274, kParamTapeMix, 32},   {274, kParamIns1Algo, 1},
  {274, kParamIns1A, 24},   {274, kParamIns1B, 40},   {274, kParamIns1C, 28},
  {274, kParamIns1Mix, 16},   {274, kParamMstAlgo, 2},   {274, kParamMstA, 24},
  {274, kParamMstB, 18},   {274, kParamMstC, 30},   {274, kParamMstMix, 10},
  {275, kParamEngine, 0},   {275, kParamPolyMode, 0},   {275, kParamPorta, 0},
  {275, kParamOsc1Shape, 38},   {275, kParamOsc1Level, 88},   {275, kParamOsc2Shape, 36},
  {275, kParamOsc2Pitch, -12},   {275, kParamOsc2Level, 54},   {275, kParamSubLevel, 62},
  {275, kParamNoiseLevel, 2},   {275, kParamFilterType, 0},   {275, kParamFilterCutoff, 2200},
  {275, kParamFilterReso, 28},   {275, kParamFilterDrive, 0},   {275, kParamFilterEnv, 46},
  {275, kParamFiltAttack, 2},   {275, kParamFiltDecay, 650},   {275, kParamFiltSustain, 25.2},
  {275, kParamFiltRelease, 315},   {275, kParamAmpAttack, 4},   {275, kParamAmpDecay, 650},
  {275, kParamAmpSustain, 42},   {275, kParamAmpRelease, 450},   {275, kParamVelToAmp, 38},
  {275, kParamVelToFilter, 40},   {275, kParamStereoSpread, 0},   {275, kParamStereoDelay, 0},
  {275, kParamTapeSat, 26},   {275, kParamTapeMix, 44},   {275, kParamTapeWow, 40},
  {275, kParamTapeFlutter, 30},   {275, kParamIns1Algo, 1},   {275, kParamIns1A, 32},
  {275, kParamIns1B, 46},   {275, kParamIns1C, 34},   {275, kParamIns1Mix, 22},
  {275, kParamMstAlgo, 2},   {275, kParamMstA, 24},   {275, kParamMstB, 18},
  {275, kParamMstC, 30},   {275, kParamMstMix, 10},
  // strings
  {276, kParamEngine, 0},   {276, kParamPolyMode, 0},   {276, kParamOsc1Shape, 42},
  {276, kParamOsc1Level, 82},   {276, kParamOsc2Shape, 40},   {276, kParamOsc2Pitch, 0},
  {276, kParamOsc2Level, 76},   {276, kParamSubLevel, 32},   {276, kParamFilterType, 0},
  {276, kParamFilterCutoff, 6200},   {276, kParamFilterReso, 14},   {276, kParamFilterEnv, 40},
  {276, kParamFiltAttack, 8},   {276, kParamFiltDecay, 1100},   {276, kParamFiltSustain, 34},
  {276, kParamFiltRelease, 2600},   {276, kParamAmpAttack, 420},   {276, kParamAmpDecay, 1400},
  {276, kParamAmpSustain, 66},   {276, kParamAmpRelease, 2600},   {276, kParamUnisonVoices, 4},
  {276, kParamUnisonDetune, 24},   {276, kParamStereoSpread, 84},   {276, kParamStereoDelay, 0},
  {276, kParamTapeSat, 12},   {276, kParamTapeMix, 24},   {276, kParamFilterKeyTrack, 40},
  {276, kParamIns1Algo, 2},   {276, kParamIns1A, 12},   {276, kParamIns1B, 36},
  {276, kParamIns1C, 44},   {276, kParamIns1Mix, 28},   {276, kParamMstAlgo, 2},
  {276, kParamMstA, 24},   {276, kParamMstB, 18},   {276, kParamMstC, 30},
  {276, kParamMstMix, 10},
  {277, kParamEngine, 0},   {277, kParamPolyMode, 0},   {277, kParamOsc1Shape, 40},
  {277, kParamOsc1Level, 82},   {277, kParamOsc2Shape, 38},   {277, kParamOsc2Pitch, 0},
  {277, kParamOsc2Level, 76},   {277, kParamSubLevel, 32},   {277, kParamFilterType, 0},
  {277, kParamFilterCutoff, 4400},   {277, kParamFilterReso, 24},   {277, kParamFilterEnv, 52},
  {277, kParamFiltAttack, 90},   {277, kParamFiltDecay, 1100},   {277, kParamFiltSustain, 34},
  {277, kParamFiltRelease, 1200},   {277, kParamAmpAttack, 60},   {277, kParamAmpDecay, 1400},
  {277, kParamAmpSustain, 66},   {277, kParamAmpRelease, 1200},   {277, kParamUnisonVoices, 4},
  {277, kParamUnisonDetune, 18},   {277, kParamStereoSpread, 58},   {277, kParamStereoDelay, 0},
  {277, kParamTapeSat, 12},   {277, kParamTapeMix, 24},   {277, kParamIns1Algo, 2},
  {277, kParamIns1A, 18},   {277, kParamIns1B, 44},   {277, kParamIns1C, 52},
  {277, kParamIns1Mix, 34},   {277, kParamMstAlgo, 2},   {277, kParamMstA, 24},
  {277, kParamMstB, 18},   {277, kParamMstC, 30},   {277, kParamMstMix, 10},
  {278, kParamEngine, 0},   {278, kParamPolyMode, 0},   {278, kParamOsc1Shape, 30},
  {278, kParamOsc1Level, 58},   {278, kParamOsc2Level, 0},   {278, kParamOsc3Enable, 1},
  {278, kParamOsc3Source, 1},   {278, kParamOsc3Morph, 3.4},   {278, kParamOsc3Level, 86},
  {278, kParamOsc3Pitch, 0},   {278, kParamSubLevel, 26},   {278, kParamFilterType, 0},
  {278, kParamFilterCutoff, 5600},   {278, kParamFilterReso, 16},   {278, kParamFilterEnv, 28},
  {278, kParamFiltAttack, 10},   {278, kParamFiltDecay, 1400},   {278, kParamFiltSustain, 44},
  {278, kParamFiltRelease, 3200},   {278, kParamAmpAttack, 520},   {278, kParamAmpDecay, 1600},
  {278, kParamAmpSustain, 70},   {278, kParamAmpRelease, 3200},   {278, kParamUnisonVoices, 4},
  {278, kParamUnisonDetune, 12},   {278, kParamStereoSpread, 88},   {278, kParamTapeSat, 18},
  {278, kParamTapeMix, 30},   {278, kParamIns1Algo, 3},   {278, kParamIns1A, 58},
  {278, kParamIns1B, 42},   {278, kParamIns1C, 48},   {278, kParamIns1Mix, 20},
  {278, kParamMstAlgo, 2},   {278, kParamMstA, 24},   {278, kParamMstB, 18},
  {278, kParamMstC, 30},   {278, kParamMstMix, 10},
  {279, kParamEngine, 0},   {279, kParamPolyMode, 0},   {279, kParamOsc1Shape, 36},
  {279, kParamOsc1Level, 88},   {279, kParamOsc2Level, 0},   {279, kParamSubLevel, 45},
  {279, kParamNoiseLevel, 0},   {279, kParamFilterType, 0},   {279, kParamFilterCutoff, 7000},
  {279, kParamFilterReso, 12},   {279, kParamFilterEnv, 32},   {279, kParamFiltAttack, 6},
  {279, kParamFiltDecay, 900},   {279, kParamFiltSustain, 38},   {279, kParamFiltRelease, 2800},
  {279, kParamAmpAttack, 380},   {279, kParamAmpDecay, 1200},   {279, kParamAmpSustain, 62},
  {279, kParamAmpRelease, 2800},   {279, kParamUnisonVoices, 2},   {279, kParamUnisonDetune, 17},
  {279, kParamStereoSpread, 42},   {279, kParamTapeSat, 14},   {279, kParamTapeMix, 28},
  {279, kParamIns1Algo, 2},   {279, kParamIns1A, 12},   {279, kParamIns1B, 36},
  {279, kParamIns1C, 44},   {279, kParamIns1Mix, 28},   {279, kParamMstAlgo, 2},
  {279, kParamMstA, 24},   {279, kParamMstB, 18},   {279, kParamMstC, 30},
  {279, kParamMstMix, 10},
  {280, kParamEngine, 0},   {280, kParamPolyMode, 0},   {280, kParamOsc1Shape, 42},
  {280, kParamOsc1Level, 82},   {280, kParamOsc2Shape, 40},   {280, kParamOsc2Pitch, 0},
  {280, kParamOsc2Level, 76},   {280, kParamSubLevel, 32},   {280, kParamFilterType, 0},
  {280, kParamFilterCutoff, 5200},   {280, kParamFilterReso, 18},   {280, kParamFilterEnv, 40},
  {280, kParamFiltAttack, 8},   {280, kParamFiltDecay, 1100},   {280, kParamFiltSustain, 34},
  {280, kParamFiltRelease, 3000},   {280, kParamAmpAttack, 460},   {280, kParamAmpDecay, 1400},
  {280, kParamAmpSustain, 66},   {280, kParamAmpRelease, 3000},   {280, kParamUnisonVoices, 4},
  {280, kParamUnisonDetune, 28},   {280, kParamStereoSpread, 90},   {280, kParamStereoDelay, 0},
  {280, kParamTapeSat, 12},   {280, kParamTapeMix, 24},   {280, kParamIns1Algo, 2},
  {280, kParamIns1A, 18},   {280, kParamIns1B, 44},   {280, kParamIns1C, 52},
  {280, kParamIns1Mix, 34},   {280, kParamMstAlgo, 2},   {280, kParamMstA, 24},
  {280, kParamMstB, 18},   {280, kParamMstC, 30},   {280, kParamMstMix, 10},
  {281, kParamEngine, 0},   {281, kParamPolyMode, 0},   {281, kParamOsc1Shape, 36},
  {281, kParamOsc1Level, 90},   {281, kParamOsc2Shape, 34},   {281, kParamOsc2Pitch, -12},
  {281, kParamOsc2Level, 58},   {281, kParamSubLevel, 38},   {281, kParamNoiseLevel, 6},
  {281, kParamFilterType, 0},   {281, kParamFilterCutoff, 3400},   {281, kParamFilterReso, 22},
  {281, kParamFilterDrive, 32},   {281, kParamFilterEnv, 44},   {281, kParamFiltAttack, 8},
  {281, kParamFiltDecay, 1400},   {281, kParamFiltSustain, 40.599999999999994},   {281, kParamFiltRelease, 900},
  {281, kParamAmpAttack, 80},   {281, kParamAmpDecay, 1400},   {281, kParamAmpSustain, 58},
  {281, kParamAmpRelease, 900},   {281, kParamUnisonVoices, 2},   {281, kParamUnisonDetune, 14},
  {281, kParamStereoSpread, 34},   {281, kParamTapeSat, 30},   {281, kParamTapeMix, 36},
  {281, kParamIns1Algo, 3},   {281, kParamIns1A, 58},   {281, kParamIns1B, 42},
  {281, kParamIns1C, 48},   {281, kParamIns1Mix, 20},   {281, kParamMstAlgo, 2},
  {281, kParamMstA, 24},   {281, kParamMstB, 18},   {281, kParamMstC, 30},
  {281, kParamMstMix, 10},
  {282, kParamEngine, 0},   {282, kParamPolyMode, 0},   {282, kParamOsc1Shape, 44},
  {282, kParamOsc1Level, 84},   {282, kParamOsc2Shape, 46},   {282, kParamOsc2Pitch, -12},
  {282, kParamOsc2Level, 64},   {282, kParamSubLevel, 20},   {282, kParamFilterType, 0},
  {282, kParamFilterCutoff, 5800},   {282, kParamFilterReso, 16},   {282, kParamFilterEnv, 48},
  {282, kParamFiltAttack, 4},   {282, kParamFiltDecay, 1760},   {282, kParamFiltSustain, 37.199999999999996},
  {282, kParamFiltRelease, 1680},   {282, kParamAmpAttack, 400},   {282, kParamAmpDecay, 2200},
  {282, kParamAmpSustain, 62},   {282, kParamAmpRelease, 2400},   {282, kParamUnisonVoices, 1},
  {282, kParamUnisonDetune, 10},   {282, kParamStereoSpread, 76},   {282, kParamStereoDelay, 18},
  {282, kParamTapeSat, 20},   {282, kParamTapeMix, 32},   {282, kParamPorta, 40},
  {282, kParamMonoLegato, 1},   {282, kParamIns1Algo, 2},   {282, kParamIns1A, 12},
  {282, kParamIns1B, 36},   {282, kParamIns1C, 44},   {282, kParamIns1Mix, 28},
  {282, kParamMstAlgo, 2},   {282, kParamMstA, 24},   {282, kParamMstB, 18},
  {282, kParamMstC, 30},   {282, kParamMstMix, 10},
  {283, kParamEngine, 0},   {283, kParamPolyMode, 0},   {283, kParamOsc1Shape, 40},
  {283, kParamOsc1Level, 76},   {283, kParamOsc2Shape, 38},   {283, kParamOsc2Pitch, -12},
  {283, kParamOsc2Level, 62},   {283, kParamSubLevel, 28},   {283, kParamNoiseLevel, 6},
  {283, kParamFilterType, 0},   {283, kParamFilterCutoff, 5200},   {283, kParamFilterReso, 20},
  {283, kParamFilterEnv, 26},   {283, kParamFiltAttack, 252},   {283, kParamFiltDecay, 2200},
  {283, kParamFiltSustain, 52},   {283, kParamFiltRelease, 2800},   {283, kParamAmpAttack, 420},
  {283, kParamAmpDecay, 2400},   {283, kParamAmpSustain, 76},   {283, kParamAmpRelease, 2800},
  {283, kParamUnisonVoices, 4},   {283, kParamUnisonDetune, 20},   {283, kParamLfoRate, 0.22},
  {283, kParamLfoDest, 1},   {283, kParamLfoAmount, 22},   {283, kParamStereoSpread, 78},
  {283, kParamTapeSat, 16},   {283, kParamTapeMix, 30},   {283, kParamIns1Algo, 2},
  {283, kParamIns1A, 18},   {283, kParamIns1B, 44},   {283, kParamIns1C, 52},
  {283, kParamIns1Mix, 34},   {283, kParamMstAlgo, 2},   {283, kParamMstA, 24},
  {283, kParamMstB, 18},   {283, kParamMstC, 30},   {283, kParamMstMix, 10},
  {284, kParamEngine, 0},   {284, kParamPolyMode, 0},   {284, kParamOsc1Shape, 40},
  {284, kParamOsc1Level, 76},   {284, kParamOsc2Shape, 38},   {284, kParamOsc2Pitch, -12},
  {284, kParamOsc2Level, 62},   {284, kParamSubLevel, 28},   {284, kParamNoiseLevel, 6},
  {284, kParamFilterType, 0},   {284, kParamFilterCutoff, 4600},   {284, kParamFilterReso, 24},
  {284, kParamFilterEnv, 26},   {284, kParamFiltAttack, 228},   {284, kParamFiltDecay, 2200},
  {284, kParamFiltSustain, 52},   {284, kParamFiltRelease, 3000},   {284, kParamAmpAttack, 380},
  {284, kParamAmpDecay, 2400},   {284, kParamAmpSustain, 76},   {284, kParamAmpRelease, 3000},
  {284, kParamUnisonVoices, 4},   {284, kParamUnisonDetune, 20},   {284, kParamLfoRate, 0.16},
  {284, kParamLfoDest, 1},   {284, kParamLfoAmount, 22},   {284, kParamStereoSpread, 78},
  {284, kParamTapeSat, 16},   {284, kParamTapeMix, 30},   {284, kParamIns1Algo, 3},
  {284, kParamIns1A, 58},   {284, kParamIns1B, 42},   {284, kParamIns1C, 48},
  {284, kParamIns1Mix, 20},   {284, kParamMstAlgo, 2},   {284, kParamMstA, 24},
  {284, kParamMstB, 18},   {284, kParamMstC, 30},   {284, kParamMstMix, 10},
  {285, kParamEngine, 0},   {285, kParamPolyMode, 0},   {285, kParamOsc1Shape, 40},
  {285, kParamOsc1Level, 76},   {285, kParamOsc2Shape, 38},   {285, kParamOsc2Pitch, -12},
  {285, kParamOsc2Level, 62},   {285, kParamSubLevel, 28},   {285, kParamNoiseLevel, 6},
  {285, kParamFilterType, 0},   {285, kParamFilterCutoff, 6000},   {285, kParamFilterReso, 18},
  {285, kParamFilterEnv, 26},   {285, kParamFiltAttack, 204},   {285, kParamFiltDecay, 2200},
  {285, kParamFiltSustain, 52},   {285, kParamFiltRelease, 2600},   {285, kParamAmpAttack, 340},
  {285, kParamAmpDecay, 2400},   {285, kParamAmpSustain, 76},   {285, kParamAmpRelease, 2600},
  {285, kParamUnisonVoices, 4},   {285, kParamUnisonDetune, 20},   {285, kParamLfoRate, 0.22},
  {285, kParamLfoDest, 1},   {285, kParamLfoAmount, 22},   {285, kParamStereoSpread, 78},
  {285, kParamTapeSat, 16},   {285, kParamTapeMix, 30},   {285, kParamIns1Algo, 2},
  {285, kParamIns1A, 12},   {285, kParamIns1B, 36},   {285, kParamIns1C, 44},
  {285, kParamIns1Mix, 28},   {285, kParamMstAlgo, 2},   {285, kParamMstA, 24},
  {285, kParamMstB, 18},   {285, kParamMstC, 30},   {285, kParamMstMix, 10},
  {286, kParamEngine, 0},   {286, kParamPolyMode, 0},   {286, kParamOsc1Shape, 28},
  {286, kParamOsc1Level, 48},   {286, kParamOsc2Level, 0},   {286, kParamOsc3Enable, 1},
  {286, kParamOsc3Source, 1},   {286, kParamOsc3Morph, 3.2},   {286, kParamOsc3Level, 84},
  {286, kParamSubLevel, 22},   {286, kParamNoiseLevel, 4},   {286, kParamFilterType, 0},
  {286, kParamFilterCutoff, 5000},   {286, kParamFilterReso, 22},   {286, kParamFilterEnv, 26},
  {286, kParamFiltAttack, 276},   {286, kParamFiltDecay, 2200},   {286, kParamFiltSustain, 52},
  {286, kParamFiltRelease, 3200},   {286, kParamAmpAttack, 460},   {286, kParamAmpDecay, 2400},
  {286, kParamAmpSustain, 76},   {286, kParamAmpRelease, 3200},   {286, kParamUnisonVoices, 4},
  {286, kParamUnisonDetune, 20},   {286, kParamLfoRate, 0.22},   {286, kParamLfoDest, 1},
  {286, kParamLfoAmount, 22},   {286, kParamStereoSpread, 78},   {286, kParamTapeSat, 16},
  {286, kParamTapeMix, 30},   {286, kParamIns1Algo, 2},   {286, kParamIns1A, 18},
  {286, kParamIns1B, 44},   {286, kParamIns1C, 52},   {286, kParamIns1Mix, 34},
  {286, kParamMstAlgo, 2},   {286, kParamMstA, 24},   {286, kParamMstB, 18},
  {286, kParamMstC, 30},   {286, kParamMstMix, 10},
  {287, kParamEngine, 0},   {287, kParamPolyMode, 0},   {287, kParamOsc1Shape, 28},
  {287, kParamOsc1Level, 48},   {287, kParamOsc2Level, 0},   {287, kParamOsc3Enable, 1},
  {287, kParamOsc3Source, 1},   {287, kParamOsc3Morph, 2.4},   {287, kParamOsc3Level, 84},
  {287, kParamSubLevel, 22},   {287, kParamNoiseLevel, 4},   {287, kParamFilterType, 0},
  {287, kParamFilterCutoff, 7200},   {287, kParamFilterReso, 16},   {287, kParamFilterEnv, 26},
  {287, kParamFiltAttack, 180},   {287, kParamFiltDecay, 2200},   {287, kParamFiltSustain, 52},
  {287, kParamFiltRelease, 2400},   {287, kParamAmpAttack, 300},   {287, kParamAmpDecay, 2400},
  {287, kParamAmpSustain, 76},   {287, kParamAmpRelease, 2400},   {287, kParamUnisonVoices, 4},
  {287, kParamUnisonDetune, 20},   {287, kParamLfoRate, 0.22},   {287, kParamLfoDest, 1},
  {287, kParamLfoAmount, 22},   {287, kParamStereoSpread, 78},   {287, kParamTapeSat, 16},
  {287, kParamTapeMix, 30},   {287, kParamIns1Algo, 3},   {287, kParamIns1A, 58},
  {287, kParamIns1B, 42},   {287, kParamIns1C, 48},   {287, kParamIns1Mix, 20},
  {287, kParamMstAlgo, 2},   {287, kParamMstA, 24},   {287, kParamMstB, 18},
  {287, kParamMstC, 30},   {287, kParamMstMix, 10},
  {288, kParamEngine, 0},   {288, kParamPolyMode, 0},   {288, kParamOsc1Shape, 40},
  {288, kParamOsc1Level, 76},   {288, kParamOsc2Shape, 38},   {288, kParamOsc2Pitch, -12},
  {288, kParamOsc2Level, 62},   {288, kParamSubLevel, 44},   {288, kParamNoiseLevel, 6},
  {288, kParamFilterType, 0},   {288, kParamFilterCutoff, 3200},   {288, kParamFilterReso, 26},
  {288, kParamFilterEnv, 26},   {288, kParamFiltAttack, 300},   {288, kParamFiltDecay, 2200},
  {288, kParamFiltSustain, 52},   {288, kParamFiltRelease, 3400},   {288, kParamAmpAttack, 500},
  {288, kParamAmpDecay, 2400},   {288, kParamAmpSustain, 76},   {288, kParamAmpRelease, 3400},
  {288, kParamUnisonVoices, 4},   {288, kParamUnisonDetune, 20},   {288, kParamLfoRate, 0.12},
  {288, kParamLfoDest, 1},   {288, kParamLfoAmount, 22},   {288, kParamStereoSpread, 78},
  {288, kParamTapeSat, 16},   {288, kParamTapeMix, 30},   {288, kParamIns1Algo, 2},
  {288, kParamIns1A, 12},   {288, kParamIns1B, 36},   {288, kParamIns1C, 44},
  {288, kParamIns1Mix, 28},   {288, kParamMstAlgo, 2},   {288, kParamMstA, 24},
  {288, kParamMstB, 18},   {288, kParamMstC, 30},   {288, kParamMstMix, 10},
  {289, kParamEngine, 0},   {289, kParamPolyMode, 0},   {289, kParamOsc1Shape, 40},
  {289, kParamOsc1Level, 76},   {289, kParamOsc2Shape, 38},   {289, kParamOsc2Pitch, -12},
  {289, kParamOsc2Level, 62},   {289, kParamSubLevel, 28},   {289, kParamNoiseLevel, 14},
  {289, kParamFilterType, 0},   {289, kParamFilterCutoff, 6400},   {289, kParamFilterReso, 20},
  {289, kParamFilterEnv, 26},   {289, kParamFiltAttack, 216},   {289, kParamFiltDecay, 2200},
  {289, kParamFiltSustain, 52},   {289, kParamFiltRelease, 2800},   {289, kParamAmpAttack, 360},
  {289, kParamAmpDecay, 2400},   {289, kParamAmpSustain, 76},   {289, kParamAmpRelease, 2800},
  {289, kParamUnisonVoices, 4},   {289, kParamUnisonDetune, 20},   {289, kParamLfoRate, 0.3},
  {289, kParamLfoDest, 1},   {289, kParamLfoAmount, 22},   {289, kParamStereoSpread, 78},
  {289, kParamTapeSat, 16},   {289, kParamTapeMix, 30},   {289, kParamIns1Algo, 2},
  {289, kParamIns1A, 18},   {289, kParamIns1B, 44},   {289, kParamIns1C, 52},
  {289, kParamIns1Mix, 34},   {289, kParamMstAlgo, 2},   {289, kParamMstA, 24},
  {289, kParamMstB, 18},   {289, kParamMstC, 30},   {289, kParamMstMix, 10},
  {290, kParamEngine, 0},   {290, kParamPolyMode, 0},   {290, kParamOsc1Shape, 40},
  {290, kParamOsc1Level, 76},   {290, kParamOsc2Shape, 38},   {290, kParamOsc2Pitch, -12},
  {290, kParamOsc2Level, 62},   {290, kParamSubLevel, 28},   {290, kParamNoiseLevel, 6},
  {290, kParamFilterType, 0},   {290, kParamFilterCutoff, 4200},   {290, kParamFilterReso, 24},
  {290, kParamFilterEnv, 26},   {290, kParamFiltAttack, 240},   {290, kParamFiltDecay, 2200},
  {290, kParamFiltSustain, 52},   {290, kParamFiltRelease, 3000},   {290, kParamAmpAttack, 400},
  {290, kParamAmpDecay, 2400},   {290, kParamAmpSustain, 76},   {290, kParamAmpRelease, 3000},
  {290, kParamUnisonVoices, 4},   {290, kParamUnisonDetune, 20},   {290, kParamLfoRate, 0.22},
  {290, kParamLfoDest, 1},   {290, kParamLfoAmount, 22},   {290, kParamStereoSpread, 78},
  {290, kParamTapeSat, 34},   {290, kParamTapeMix, 44},   {290, kParamTapeWow, 40},
  {290, kParamIns1Algo, 3},   {290, kParamIns1A, 58},   {290, kParamIns1B, 42},
  {290, kParamIns1C, 48},   {290, kParamIns1Mix, 20},   {290, kParamMstAlgo, 2},
  {290, kParamMstA, 24},   {290, kParamMstB, 18},   {290, kParamMstC, 30},
  {290, kParamMstMix, 10},
  {291, kParamEngine, 0},   {291, kParamPolyMode, 0},   {291, kParamOsc1Shape, 40},
  {291, kParamOsc1Level, 76},   {291, kParamOsc2Shape, 38},   {291, kParamOsc2Pitch, -12},
  {291, kParamOsc2Level, 62},   {291, kParamSubLevel, 28},   {291, kParamNoiseLevel, 6},
  {291, kParamFilterType, 0},   {291, kParamFilterCutoff, 5600},   {291, kParamFilterReso, 34},
  {291, kParamFilterEnv, 26},   {291, kParamFiltAttack, 204},   {291, kParamFiltDecay, 2200},
  {291, kParamFiltSustain, 52},   {291, kParamFiltRelease, 2600},   {291, kParamAmpAttack, 340},
  {291, kParamAmpDecay, 2400},   {291, kParamAmpSustain, 76},   {291, kParamAmpRelease, 2600},
  {291, kParamUnisonVoices, 4},   {291, kParamUnisonDetune, 20},   {291, kParamLfoRate, 0.26},
  {291, kParamLfoDest, 1},   {291, kParamLfoAmount, 40},   {291, kParamStereoSpread, 78},
  {291, kParamTapeSat, 16},   {291, kParamTapeMix, 30},   {291, kParamIns1Algo, 2},
  {291, kParamIns1A, 12},   {291, kParamIns1B, 36},   {291, kParamIns1C, 44},
  {291, kParamIns1Mix, 28},   {291, kParamMstAlgo, 2},   {291, kParamMstA, 24},
  {291, kParamMstB, 18},   {291, kParamMstC, 30},   {291, kParamMstMix, 10},
  {292, kParamEngine, 0},   {292, kParamPolyMode, 0},   {292, kParamOsc1Shape, 28},
  {292, kParamOsc1Level, 48},   {292, kParamOsc2Level, 0},   {292, kParamOsc3Enable, 1},
  {292, kParamOsc3Source, 1},   {292, kParamOsc3Morph, 5.2},   {292, kParamOsc3Level, 84},
  {292, kParamSubLevel, 16},   {292, kParamNoiseLevel, 4},   {292, kParamFilterType, 0},
  {292, kParamFilterCutoff, 5800},   {292, kParamFilterReso, 28},   {292, kParamFilterEnv, 26},
  {292, kParamFiltAttack, 264},   {292, kParamFiltDecay, 2200},   {292, kParamFiltSustain, 52},
  {292, kParamFiltRelease, 2900},   {292, kParamAmpAttack, 440},   {292, kParamAmpDecay, 2400},
  {292, kParamAmpSustain, 76},   {292, kParamAmpRelease, 2900},   {292, kParamUnisonVoices, 4},
  {292, kParamUnisonDetune, 20},   {292, kParamLfoRate, 0.22},   {292, kParamLfoDest, 1},
  {292, kParamLfoAmount, 22},   {292, kParamStereoSpread, 78},   {292, kParamTapeSat, 16},
  {292, kParamTapeMix, 30},   {292, kParamIns1Algo, 2},   {292, kParamIns1A, 18},
  {292, kParamIns1B, 44},   {292, kParamIns1C, 52},   {292, kParamIns1Mix, 34},
  {292, kParamMstAlgo, 2},   {292, kParamMstA, 24},   {292, kParamMstB, 18},
  {292, kParamMstC, 30},   {292, kParamMstMix, 10},
  {293, kParamEngine, 0},   {293, kParamPolyMode, 0},   {293, kParamOsc1Shape, 42},
  {293, kParamOsc1Level, 78},   {293, kParamOsc2Shape, 40},   {293, kParamOsc2Pitch, -12},
  {293, kParamOsc2Level, 66},   {293, kParamSubLevel, 18},   {293, kParamNoiseLevel, 8},
  {293, kParamFilterType, 0},   {293, kParamFilterCutoff, 6200},   {293, kParamFilterReso, 14},
  {293, kParamFilterEnv, 30},   {293, kParamFiltAttack, 112},   {293, kParamFiltDecay, 1600},
  {293, kParamFiltSustain, 54},   {293, kParamFiltRelease, 2200},   {293, kParamAmpAttack, 140},
  {293, kParamAmpDecay, 1800},   {293, kParamAmpSustain, 78},   {293, kParamAmpRelease, 2200},
  {293, kParamUnisonVoices, 4},   {293, kParamUnisonDetune, 18},   {293, kParamFilterKeyTrack, 36},
  {293, kParamStereoSpread, 72},   {293, kParamTapeSat, 14},   {293, kParamTapeMix, 26},
  {293, kParamIns1Algo, 3},   {293, kParamIns1A, 58},   {293, kParamIns1B, 42},
  {293, kParamIns1C, 48},   {293, kParamIns1Mix, 20},   {293, kParamMstAlgo, 2},
  {293, kParamMstA, 24},   {293, kParamMstB, 18},   {293, kParamMstC, 30},
  {293, kParamMstMix, 10},
  {294, kParamEngine, 0},   {294, kParamPolyMode, 0},   {294, kParamOsc1Shape, 42},
  {294, kParamOsc1Level, 78},   {294, kParamOsc2Shape, 40},   {294, kParamOsc2Pitch, -12},
  {294, kParamOsc2Level, 66},   {294, kParamSubLevel, 18},   {294, kParamNoiseLevel, 12},
  {294, kParamFilterType, 0},   {294, kParamFilterCutoff, 5400},   {294, kParamFilterReso, 16},
  {294, kParamFilterEnv, 30},   {294, kParamFiltAttack, 144},   {294, kParamFiltDecay, 1600},
  {294, kParamFiltSustain, 54},   {294, kParamFiltRelease, 2000},   {294, kParamAmpAttack, 180},
  {294, kParamAmpDecay, 1800},   {294, kParamAmpSustain, 78},   {294, kParamAmpRelease, 2000},
  {294, kParamUnisonVoices, 4},   {294, kParamUnisonDetune, 18},   {294, kParamFilterKeyTrack, 36},
  {294, kParamStereoSpread, 72},   {294, kParamTapeSat, 14},   {294, kParamTapeMix, 26},
  {294, kParamIns1Algo, 2},   {294, kParamIns1A, 12},   {294, kParamIns1B, 36},
  {294, kParamIns1C, 44},   {294, kParamIns1Mix, 28},   {294, kParamMstAlgo, 2},
  {294, kParamMstA, 24},   {294, kParamMstB, 18},   {294, kParamMstC, 30},
  {294, kParamMstMix, 10},
  {295, kParamEngine, 0},   {295, kParamPolyMode, 0},   {295, kParamOsc1Shape, 42},
  {295, kParamOsc1Level, 78},   {295, kParamOsc2Shape, 40},   {295, kParamOsc2Pitch, -12},
  {295, kParamOsc2Level, 66},   {295, kParamSubLevel, 18},   {295, kParamNoiseLevel, 8},
  {295, kParamFilterType, 0},   {295, kParamFilterCutoff, 7600},   {295, kParamFilterReso, 12},
  {295, kParamFilterEnv, 30},   {295, kParamFiltAttack, 88},   {295, kParamFiltDecay, 1600},
  {295, kParamFiltSustain, 54},   {295, kParamFiltRelease, 1800},   {295, kParamAmpAttack, 110},
  {295, kParamAmpDecay, 1800},   {295, kParamAmpSustain, 78},   {295, kParamAmpRelease, 1800},
  {295, kParamUnisonVoices, 4},   {295, kParamUnisonDetune, 18},   {295, kParamFilterKeyTrack, 36},
  {295, kParamStereoSpread, 72},   {295, kParamTapeSat, 14},   {295, kParamTapeMix, 26},
  {295, kParamIns1Algo, 2},   {295, kParamIns1A, 18},   {295, kParamIns1B, 44},
  {295, kParamIns1C, 52},   {295, kParamIns1Mix, 34},   {295, kParamMstAlgo, 2},
  {295, kParamMstA, 24},   {295, kParamMstB, 18},   {295, kParamMstC, 30},
  {295, kParamMstMix, 10},
  {296, kParamEngine, 0},   {296, kParamPolyMode, 0},   {296, kParamOsc1Shape, 42},
  {296, kParamOsc1Level, 78},   {296, kParamOsc2Shape, 40},   {296, kParamOsc2Pitch, -24},
  {296, kParamOsc2Level, 66},   {296, kParamSubLevel, 18},   {296, kParamNoiseLevel, 8},
  {296, kParamFilterType, 0},   {296, kParamFilterCutoff, 4200},   {296, kParamFilterReso, 18},
  {296, kParamFilterEnv, 30},   {296, kParamFiltAttack, 160},   {296, kParamFiltDecay, 1600},
  {296, kParamFiltSustain, 54},   {296, kParamFiltRelease, 2400},   {296, kParamAmpAttack, 200},
  {296, kParamAmpDecay, 1800},   {296, kParamAmpSustain, 78},   {296, kParamAmpRelease, 2400},
  {296, kParamUnisonVoices, 4},   {296, kParamUnisonDetune, 18},   {296, kParamFilterKeyTrack, 36},
  {296, kParamStereoSpread, 72},   {296, kParamTapeSat, 14},   {296, kParamTapeMix, 26},
  {296, kParamIns1Algo, 3},   {296, kParamIns1A, 58},   {296, kParamIns1B, 42},
  {296, kParamIns1C, 48},   {296, kParamIns1Mix, 20},   {296, kParamMstAlgo, 2},
  {296, kParamMstA, 24},   {296, kParamMstB, 18},   {296, kParamMstC, 30},
  {296, kParamMstMix, 10},
  {297, kParamEngine, 0},   {297, kParamPolyMode, 0},   {297, kParamOsc1Shape, 42},
  {297, kParamOsc1Level, 78},   {297, kParamOsc2Shape, 40},   {297, kParamOsc2Pitch, -12},
  {297, kParamOsc2Level, 66},   {297, kParamSubLevel, 18},   {297, kParamNoiseLevel, 8},
  {297, kParamFilterType, 0},   {297, kParamFilterCutoff, 5800},   {297, kParamFilterReso, 14},
  {297, kParamFilterEnv, 30},   {297, kParamFiltAttack, 240},   {297, kParamFiltDecay, 1600},
  {297, kParamFiltSustain, 54},   {297, kParamFiltRelease, 2600},   {297, kParamAmpAttack, 300},
  {297, kParamAmpDecay, 1800},   {297, kParamAmpSustain, 78},   {297, kParamAmpRelease, 2600},
  {297, kParamUnisonVoices, 4},   {297, kParamUnisonDetune, 18},   {297, kParamFilterKeyTrack, 36},
  {297, kParamStereoSpread, 72},   {297, kParamTapeSat, 14},   {297, kParamTapeMix, 26},
  {297, kParamIns1Algo, 2},   {297, kParamIns1A, 12},   {297, kParamIns1B, 36},
  {297, kParamIns1C, 44},   {297, kParamIns1Mix, 28},   {297, kParamMstAlgo, 2},
  {297, kParamMstA, 24},   {297, kParamMstB, 18},   {297, kParamMstC, 30},
  {297, kParamMstMix, 10},
  {298, kParamEngine, 0},   {298, kParamPolyMode, 0},   {298, kParamOsc1Shape, 42},
  {298, kParamOsc1Level, 78},   {298, kParamOsc2Shape, 40},   {298, kParamOsc2Pitch, -12},
  {298, kParamOsc2Level, 66},   {298, kParamSubLevel, 18},   {298, kParamNoiseLevel, 8},
  {298, kParamFilterType, 0},   {298, kParamFilterCutoff, 6800},   {298, kParamFilterReso, 16},
  {298, kParamFilterEnv, 30},   {298, kParamFiltAttack, 56},   {298, kParamFiltDecay, 1600},
  {298, kParamFiltSustain, 54},   {298, kParamFiltRelease, 1400},   {298, kParamAmpAttack, 70},
  {298, kParamAmpDecay, 1800},   {298, kParamAmpSustain, 78},   {298, kParamAmpRelease, 1400},
  {298, kParamUnisonVoices, 4},   {298, kParamUnisonDetune, 18},   {298, kParamFilterKeyTrack, 36},
  {298, kParamStereoSpread, 72},   {298, kParamTapeSat, 14},   {298, kParamTapeMix, 26},
  {298, kParamIns1Algo, 2},   {298, kParamIns1A, 18},   {298, kParamIns1B, 44},
  {298, kParamIns1C, 52},   {298, kParamIns1Mix, 34},   {298, kParamMstAlgo, 2},
  {298, kParamMstA, 24},   {298, kParamMstB, 18},   {298, kParamMstC, 30},
  {298, kParamMstMix, 10},
  {299, kParamEngine, 0},   {299, kParamPolyMode, 0},   {299, kParamOsc1Shape, 42},
  {299, kParamOsc1Level, 78},   {299, kParamOsc2Shape, 40},   {299, kParamOsc2Pitch, -12},
  {299, kParamOsc2Level, 66},   {299, kParamSubLevel, 18},   {299, kParamNoiseLevel, 8},
  {299, kParamFilterType, 0},   {299, kParamFilterCutoff, 5600},   {299, kParamFilterReso, 20},
  {299, kParamFilterEnv, 42},   {299, kParamFiltAttack, 208},   {299, kParamFiltDecay, 1600},
  {299, kParamFiltSustain, 54},   {299, kParamFiltRelease, 2800},   {299, kParamAmpAttack, 260},
  {299, kParamAmpDecay, 1800},   {299, kParamAmpSustain, 78},   {299, kParamAmpRelease, 2800},
  {299, kParamUnisonVoices, 4},   {299, kParamUnisonDetune, 18},   {299, kParamFilterKeyTrack, 36},
  {299, kParamStereoSpread, 72},   {299, kParamTapeSat, 14},   {299, kParamTapeMix, 26},
  {299, kParamIns1Algo, 3},   {299, kParamIns1A, 58},   {299, kParamIns1B, 42},
  {299, kParamIns1C, 48},   {299, kParamIns1Mix, 20},   {299, kParamMstAlgo, 2},
  {299, kParamMstA, 24},   {299, kParamMstB, 18},   {299, kParamMstC, 30},
  {299, kParamMstMix, 10},
  {300, kParamEngine, 0},   {300, kParamPolyMode, 0},   {300, kParamOsc1Shape, 42},
  {300, kParamOsc1Level, 78},   {300, kParamOsc2Shape, 40},   {300, kParamOsc2Pitch, -12},
  {300, kParamOsc2Level, 66},   {300, kParamSubLevel, 18},   {300, kParamNoiseLevel, 8},
  {300, kParamFilterType, 0},   {300, kParamFilterCutoff, 5200},   {300, kParamFilterReso, 18},
  {300, kParamFilterEnv, 30},   {300, kParamFiltAttack, 128},   {300, kParamFiltDecay, 1600},
  {300, kParamFiltSustain, 54},   {300, kParamFiltRelease, 2200},   {300, kParamAmpAttack, 160},
  {300, kParamAmpDecay, 1800},   {300, kParamAmpSustain, 78},   {300, kParamAmpRelease, 2200},
  {300, kParamUnisonVoices, 4},   {300, kParamUnisonDetune, 18},   {300, kParamFilterKeyTrack, 36},
  {300, kParamStereoSpread, 72},   {300, kParamTapeSat, 32},   {300, kParamTapeMix, 42},
  {300, kParamTapeWow, 38},   {300, kParamIns1Algo, 2},   {300, kParamIns1A, 12},
  {300, kParamIns1B, 36},   {300, kParamIns1C, 44},   {300, kParamIns1Mix, 28},
  {300, kParamMstAlgo, 2},   {300, kParamMstA, 24},   {300, kParamMstB, 18},
  {300, kParamMstC, 30},   {300, kParamMstMix, 10},
  {301, kParamEngine, 0},   {301, kParamPolyMode, 0},   {301, kParamOsc1Shape, 42},
  {301, kParamOsc1Level, 78},   {301, kParamOsc2Shape, 40},   {301, kParamOsc2Pitch, 0},
  {301, kParamOsc2Level, 66},   {301, kParamSubLevel, 18},   {301, kParamNoiseLevel, 8},
  {301, kParamFilterType, 0},   {301, kParamFilterCutoff, 6400},   {301, kParamFilterReso, 14},
  {301, kParamFilterEnv, 30},   {301, kParamFiltAttack, 104},   {301, kParamFiltDecay, 1600},
  {301, kParamFiltSustain, 54},   {301, kParamFiltRelease, 2000},   {301, kParamAmpAttack, 130},
  {301, kParamAmpDecay, 1800},   {301, kParamAmpSustain, 78},   {301, kParamAmpRelease, 2000},
  {301, kParamUnisonVoices, 4},   {301, kParamUnisonDetune, 18},   {301, kParamFilterKeyTrack, 36},
  {301, kParamStereoSpread, 72},   {301, kParamTapeSat, 14},   {301, kParamTapeMix, 26},
  {301, kParamIns1Algo, 2},   {301, kParamIns1A, 18},   {301, kParamIns1B, 44},
  {301, kParamIns1C, 52},   {301, kParamIns1Mix, 34},   {301, kParamMstAlgo, 2},
  {301, kParamMstA, 24},   {301, kParamMstB, 18},   {301, kParamMstC, 30},
  {301, kParamMstMix, 10},
  {302, kParamEngine, 0},   {302, kParamPolyMode, 0},   {302, kParamOsc1Shape, 42},
  {302, kParamOsc1Level, 78},   {302, kParamOsc2Shape, 40},   {302, kParamOsc2Pitch, -12},
  {302, kParamOsc2Level, 66},   {302, kParamSubLevel, 8},   {302, kParamNoiseLevel, 8},
  {302, kParamFilterType, 0},   {302, kParamFilterCutoff, 8200},   {302, kParamFilterReso, 12},
  {302, kParamFilterEnv, 30},   {302, kParamFiltAttack, 96},   {302, kParamFiltDecay, 1600},
  {302, kParamFiltSustain, 54},   {302, kParamFiltRelease, 1800},   {302, kParamAmpAttack, 120},
  {302, kParamAmpDecay, 1800},   {302, kParamAmpSustain, 78},   {302, kParamAmpRelease, 1800},
  {302, kParamUnisonVoices, 4},   {302, kParamUnisonDetune, 18},   {302, kParamFilterKeyTrack, 36},
  {302, kParamStereoSpread, 72},   {302, kParamTapeSat, 14},   {302, kParamTapeMix, 26},
  {302, kParamIns1Algo, 3},   {302, kParamIns1A, 58},   {302, kParamIns1B, 42},
  {302, kParamIns1C, 48},   {302, kParamIns1Mix, 20},   {302, kParamMstAlgo, 2},
  {302, kParamMstA, 24},   {302, kParamMstB, 18},   {302, kParamMstC, 30},
  {302, kParamMstMix, 10},
  {303, kParamEngine, 0},   {303, kParamPolyMode, 0},   {303, kParamOsc1Shape, 40},
  {303, kParamOsc1Level, 74},   {303, kParamOsc2Shape, 38},   {303, kParamOsc2Pitch, -12},
  {303, kParamOsc2Level, 60},   {303, kParamSubLevel, 30},   {303, kParamNoiseLevel, 6},
  {303, kParamFilterType, 0},   {303, kParamFilterCutoff, 6800},   {303, kParamFilterReso, 18},
  {303, kParamFilterEnv, 26},   {303, kParamFiltAttack, 300},   {303, kParamFiltDecay, 2200},
  {303, kParamFiltSustain, 52},   {303, kParamFiltRelease, 3400},   {303, kParamAmpAttack, 500},
  {303, kParamAmpDecay, 2400},   {303, kParamAmpSustain, 76},   {303, kParamAmpRelease, 3400},
  {303, kParamUnisonVoices, 4},   {303, kParamUnisonDetune, 20},   {303, kParamLfoRate, 0.14},
  {303, kParamLfoDest, 1},   {303, kParamLfoAmount, 20},   {303, kParamStereoSpread, 80},
  {303, kParamTapeSat, 16},   {303, kParamTapeMix, 30},   {303, kParamIns1Algo, 2},
  {303, kParamIns1A, 12},   {303, kParamIns1B, 36},   {303, kParamIns1C, 44},
  {303, kParamIns1Mix, 28},   {303, kParamMstAlgo, 2},   {303, kParamMstA, 24},
  {303, kParamMstB, 18},   {303, kParamMstC, 30},   {303, kParamMstMix, 10},
  {304, kParamEngine, 0},   {304, kParamPolyMode, 0},   {304, kParamOsc1Shape, 40},
  {304, kParamOsc1Level, 74},   {304, kParamOsc2Shape, 38},   {304, kParamOsc2Pitch, -12},
  {304, kParamOsc2Level, 60},   {304, kParamSubLevel, 30},   {304, kParamNoiseLevel, 6},
  {304, kParamFilterType, 0},   {304, kParamFilterCutoff, 5400},   {304, kParamFilterReso, 32},
  {304, kParamFilterEnv, 26},   {304, kParamFiltAttack, 252},   {304, kParamFiltDecay, 2200},
  {304, kParamFiltSustain, 52},   {304, kParamFiltRelease, 3000},   {304, kParamAmpAttack, 420},
  {304, kParamAmpDecay, 2400},   {304, kParamAmpSustain, 76},   {304, kParamAmpRelease, 3000},
  {304, kParamUnisonVoices, 4},   {304, kParamUnisonDetune, 20},   {304, kParamLfoRate, 0.28},
  {304, kParamLfoDest, 1},   {304, kParamLfoAmount, 38},   {304, kParamStereoSpread, 80},
  {304, kParamTapeSat, 16},   {304, kParamTapeMix, 30},   {304, kParamIns1Algo, 2},
  {304, kParamIns1A, 18},   {304, kParamIns1B, 44},   {304, kParamIns1C, 52},
  {304, kParamIns1Mix, 34},   {304, kParamMstAlgo, 2},   {304, kParamMstA, 24},
  {304, kParamMstB, 18},   {304, kParamMstC, 30},   {304, kParamMstMix, 10},
  {305, kParamEngine, 0},   {305, kParamPolyMode, 0},   {305, kParamOsc1Shape, 40},
  {305, kParamOsc1Level, 74},   {305, kParamOsc2Shape, 38},   {305, kParamOsc2Pitch, -12},
  {305, kParamOsc2Level, 60},   {305, kParamSubLevel, 46},   {305, kParamNoiseLevel, 6},
  {305, kParamFilterType, 0},   {305, kParamFilterCutoff, 3800},   {305, kParamFilterReso, 24},
  {305, kParamFilterEnv, 26},   {305, kParamFiltAttack, 336},   {305, kParamFiltDecay, 2200},
  {305, kParamFiltSustain, 52},   {305, kParamFiltRelease, 3600},   {305, kParamAmpAttack, 560},
  {305, kParamAmpDecay, 2400},   {305, kParamAmpSustain, 76},   {305, kParamAmpRelease, 3600},
  {305, kParamUnisonVoices, 4},   {305, kParamUnisonDetune, 20},   {305, kParamLfoRate, 0.12},
  {305, kParamLfoDest, 1},   {305, kParamLfoAmount, 20},   {305, kParamStereoSpread, 80},
  {305, kParamTapeSat, 16},   {305, kParamTapeMix, 30},   {305, kParamIns1Algo, 3},
  {305, kParamIns1A, 58},   {305, kParamIns1B, 42},   {305, kParamIns1C, 48},
  {305, kParamIns1Mix, 20},   {305, kParamMstAlgo, 2},   {305, kParamMstA, 24},
  {305, kParamMstB, 18},   {305, kParamMstC, 30},   {305, kParamMstMix, 10},
  {306, kParamEngine, 0},   {306, kParamPolyMode, 0},   {306, kParamOsc1Shape, 26},
  {306, kParamOsc1Level, 46},   {306, kParamOsc2Level, 0},   {306, kParamOsc3Enable, 1},
  {306, kParamOsc3Source, 1},   {306, kParamOsc3Morph, 4.8},   {306, kParamOsc3Level, 84},
  {306, kParamSubLevel, 20},   {306, kParamNoiseLevel, 4},   {306, kParamFilterType, 0},
  {306, kParamFilterCutoff, 6000},   {306, kParamFilterReso, 20},   {306, kParamFilterEnv, 26},
  {306, kParamFiltAttack, 264},   {306, kParamFiltDecay, 2200},   {306, kParamFiltSustain, 52},
  {306, kParamFiltRelease, 3200},   {306, kParamAmpAttack, 440},   {306, kParamAmpDecay, 2400},
  {306, kParamAmpSustain, 76},   {306, kParamAmpRelease, 3200},   {306, kParamUnisonVoices, 4},
  {306, kParamUnisonDetune, 20},   {306, kParamLfoRate, 0.22},   {306, kParamLfoDest, 1},
  {306, kParamLfoAmount, 20},   {306, kParamStereoSpread, 80},   {306, kParamTapeSat, 16},
  {306, kParamTapeMix, 30},   {306, kParamIns1Algo, 2},   {306, kParamIns1A, 12},
  {306, kParamIns1B, 36},   {306, kParamIns1C, 44},   {306, kParamIns1Mix, 28},
  {306, kParamMstAlgo, 2},   {306, kParamMstA, 24},   {306, kParamMstB, 18},
  {306, kParamMstC, 30},   {306, kParamMstMix, 10},
  {307, kParamEngine, 0},   {307, kParamPolyMode, 0},   {307, kParamOsc1Shape, 40},
  {307, kParamOsc1Level, 74},   {307, kParamOsc2Shape, 38},   {307, kParamOsc2Pitch, -12},
  {307, kParamOsc2Level, 60},   {307, kParamSubLevel, 30},   {307, kParamNoiseLevel, 6},
  {307, kParamFilterType, 0},   {307, kParamFilterCutoff, 4800},   {307, kParamFilterReso, 22},
  {307, kParamFilterEnv, 26},   {307, kParamFiltAttack, 240},   {307, kParamFiltDecay, 2200},
  {307, kParamFiltSustain, 52},   {307, kParamFiltRelease, 3000},   {307, kParamAmpAttack, 400},
  {307, kParamAmpDecay, 2400},   {307, kParamAmpSustain, 76},   {307, kParamAmpRelease, 3000},
  {307, kParamUnisonVoices, 4},   {307, kParamUnisonDetune, 20},   {307, kParamLfoRate, 0.22},
  {307, kParamLfoDest, 1},   {307, kParamLfoAmount, 20},   {307, kParamStereoSpread, 80},
  {307, kParamTapeSat, 36},   {307, kParamTapeMix, 46},   {307, kParamTapeWow, 44},
  {307, kParamIns1Algo, 2},   {307, kParamIns1A, 18},   {307, kParamIns1B, 44},
  {307, kParamIns1C, 52},   {307, kParamIns1Mix, 34},   {307, kParamMstAlgo, 2},
  {307, kParamMstA, 24},   {307, kParamMstB, 18},   {307, kParamMstC, 30},
  {307, kParamMstMix, 10},
  {308, kParamEngine, 0},   {308, kParamPolyMode, 0},   {308, kParamOsc1Shape, 26},
  {308, kParamOsc1Level, 46},   {308, kParamOsc2Level, 0},   {308, kParamOsc3Enable, 1},
  {308, kParamOsc3Source, 1},   {308, kParamOsc3Morph, 2.6},   {308, kParamOsc3Level, 84},
  {308, kParamSubLevel, 20},   {308, kParamNoiseLevel, 4},   {308, kParamFilterType, 0},
  {308, kParamFilterCutoff, 8200},   {308, kParamFilterReso, 16},   {308, kParamFilterEnv, 26},
  {308, kParamFiltAttack, 228},   {308, kParamFiltDecay, 2200},   {308, kParamFiltSustain, 52},
  {308, kParamFiltRelease, 2800},   {308, kParamAmpAttack, 380},   {308, kParamAmpDecay, 2400},
  {308, kParamAmpSustain, 76},   {308, kParamAmpRelease, 2800},   {308, kParamUnisonVoices, 4},
  {308, kParamUnisonDetune, 20},   {308, kParamLfoRate, 0.24},   {308, kParamLfoDest, 1},
  {308, kParamLfoAmount, 20},   {308, kParamStereoSpread, 80},   {308, kParamTapeSat, 16},
  {308, kParamTapeMix, 30},   {308, kParamIns1Algo, 3},   {308, kParamIns1A, 58},
  {308, kParamIns1B, 42},   {308, kParamIns1C, 48},   {308, kParamIns1Mix, 20},
  {308, kParamMstAlgo, 2},   {308, kParamMstA, 24},   {308, kParamMstB, 18},
  {308, kParamMstC, 30},   {308, kParamMstMix, 10},
  {309, kParamEngine, 0},   {309, kParamPolyMode, 0},   {309, kParamOsc1Shape, 42},
  {309, kParamOsc1Level, 78},   {309, kParamOsc2Shape, 40},   {309, kParamOsc2Pitch, -12},
  {309, kParamOsc2Level, 66},   {309, kParamSubLevel, 18},   {309, kParamNoiseLevel, 8},
  {309, kParamFilterType, 0},   {309, kParamFilterCutoff, 7000},   {309, kParamFilterReso, 14},
  {309, kParamFilterEnv, 30},   {309, kParamFiltAttack, 120},   {309, kParamFiltDecay, 1600},
  {309, kParamFiltSustain, 54},   {309, kParamFiltRelease, 2000},   {309, kParamAmpAttack, 150},
  {309, kParamAmpDecay, 1800},   {309, kParamAmpSustain, 78},   {309, kParamAmpRelease, 2000},
  {309, kParamUnisonVoices, 4},   {309, kParamUnisonDetune, 18},   {309, kParamFilterKeyTrack, 36},
  {309, kParamStereoSpread, 74},   {309, kParamTapeSat, 14},   {309, kParamTapeMix, 26},
  {309, kParamIns1Algo, 2},   {309, kParamIns1A, 12},   {309, kParamIns1B, 36},
  {309, kParamIns1C, 44},   {309, kParamIns1Mix, 28},   {309, kParamMstAlgo, 2},
  {309, kParamMstA, 24},   {309, kParamMstB, 18},   {309, kParamMstC, 30},
  {309, kParamMstMix, 10},
  {310, kParamEngine, 0},   {310, kParamPolyMode, 0},   {310, kParamOsc1Shape, 42},
  {310, kParamOsc1Level, 78},   {310, kParamOsc2Shape, 40},   {310, kParamOsc2Pitch, -12},
  {310, kParamOsc2Level, 66},   {310, kParamSubLevel, 24},   {310, kParamNoiseLevel, 8},
  {310, kParamFilterType, 0},   {310, kParamFilterCutoff, 5800},   {310, kParamFilterReso, 16},
  {310, kParamFilterEnv, 30},   {310, kParamFiltAttack, 160},   {310, kParamFiltDecay, 1600},
  {310, kParamFiltSustain, 54},   {310, kParamFiltRelease, 2200},   {310, kParamAmpAttack, 200},
  {310, kParamAmpDecay, 1800},   {310, kParamAmpSustain, 78},   {310, kParamAmpRelease, 2200},
  {310, kParamUnisonVoices, 4},   {310, kParamUnisonDetune, 18},   {310, kParamFilterKeyTrack, 36},
  {310, kParamStereoSpread, 74},   {310, kParamTapeSat, 14},   {310, kParamTapeMix, 26},
  {310, kParamIns1Algo, 2},   {310, kParamIns1A, 18},   {310, kParamIns1B, 44},
  {310, kParamIns1C, 52},   {310, kParamIns1Mix, 34},   {310, kParamMstAlgo, 2},
  {310, kParamMstA, 24},   {310, kParamMstB, 18},   {310, kParamMstC, 30},
  {310, kParamMstMix, 10},
  {311, kParamEngine, 0},   {311, kParamPolyMode, 0},   {311, kParamOsc1Shape, 42},
  {311, kParamOsc1Level, 78},   {311, kParamOsc2Shape, 40},   {311, kParamOsc2Pitch, -12},
  {311, kParamOsc2Level, 66},   {311, kParamSubLevel, 8},   {311, kParamNoiseLevel, 8},
  {311, kParamFilterType, 0},   {311, kParamFilterCutoff, 8600},   {311, kParamFilterReso, 12},
  {311, kParamFilterEnv, 30},   {311, kParamFiltAttack, 104},   {311, kParamFiltDecay, 1600},
  {311, kParamFiltSustain, 54},   {311, kParamFiltRelease, 1900},   {311, kParamAmpAttack, 130},
  {311, kParamAmpDecay, 1800},   {311, kParamAmpSustain, 78},   {311, kParamAmpRelease, 1900},
  {311, kParamUnisonVoices, 4},   {311, kParamUnisonDetune, 18},   {311, kParamFilterKeyTrack, 36},
  {311, kParamStereoSpread, 74},   {311, kParamTapeSat, 14},   {311, kParamTapeMix, 26},
  {311, kParamIns1Algo, 3},   {311, kParamIns1A, 58},   {311, kParamIns1B, 42},
  {311, kParamIns1C, 48},   {311, kParamIns1Mix, 20},   {311, kParamMstAlgo, 2},
  {311, kParamMstA, 24},   {311, kParamMstB, 18},   {311, kParamMstC, 30},
  {311, kParamMstMix, 10},
  {312, kParamEngine, 0},   {312, kParamPolyMode, 0},   {312, kParamOsc1Shape, 42},
  {312, kParamOsc1Level, 78},   {312, kParamOsc2Shape, 40},   {312, kParamOsc2Pitch, -24},
  {312, kParamOsc2Level, 66},   {312, kParamSubLevel, 18},   {312, kParamNoiseLevel, 8},
  {312, kParamFilterType, 0},   {312, kParamFilterCutoff, 4600},   {312, kParamFilterReso, 18},
  {312, kParamFilterEnv, 30},   {312, kParamFiltAttack, 176},   {312, kParamFiltDecay, 1600},
  {312, kParamFiltSustain, 54},   {312, kParamFiltRelease, 2400},   {312, kParamAmpAttack, 220},
  {312, kParamAmpDecay, 1800},   {312, kParamAmpSustain, 78},   {312, kParamAmpRelease, 2400},
  {312, kParamUnisonVoices, 4},   {312, kParamUnisonDetune, 18},   {312, kParamFilterKeyTrack, 36},
  {312, kParamStereoSpread, 74},   {312, kParamTapeSat, 14},   {312, kParamTapeMix, 26},
  {312, kParamIns1Algo, 2},   {312, kParamIns1A, 12},   {312, kParamIns1B, 36},
  {312, kParamIns1C, 44},   {312, kParamIns1Mix, 28},   {312, kParamMstAlgo, 2},
  {312, kParamMstA, 24},   {312, kParamMstB, 18},   {312, kParamMstC, 30},
  {312, kParamMstMix, 10},
  {313, kParamEngine, 0},   {313, kParamPolyMode, 0},   {313, kParamOsc1Shape, 42},
  {313, kParamOsc1Level, 78},   {313, kParamOsc2Shape, 40},   {313, kParamOsc2Pitch, -12},
  {313, kParamOsc2Level, 66},   {313, kParamSubLevel, 18},   {313, kParamNoiseLevel, 8},
  {313, kParamFilterType, 0},   {313, kParamFilterCutoff, 5600},   {313, kParamFilterReso, 18},
  {313, kParamFilterEnv, 30},   {313, kParamFiltAttack, 144},   {313, kParamFiltDecay, 1600},
  {313, kParamFiltSustain, 54},   {313, kParamFiltRelease, 2200},   {313, kParamAmpAttack, 180},
  {313, kParamAmpDecay, 1800},   {313, kParamAmpSustain, 78},   {313, kParamAmpRelease, 2200},
  {313, kParamUnisonVoices, 4},   {313, kParamUnisonDetune, 18},   {313, kParamFilterKeyTrack, 36},
  {313, kParamStereoSpread, 74},   {313, kParamTapeSat, 34},   {313, kParamTapeMix, 44},
  {313, kParamTapeWow, 40},   {313, kParamIns1Algo, 2},   {313, kParamIns1A, 18},
  {313, kParamIns1B, 44},   {313, kParamIns1C, 52},   {313, kParamIns1Mix, 34},
  {313, kParamMstAlgo, 2},   {313, kParamMstA, 24},   {313, kParamMstB, 18},
  {313, kParamMstC, 30},   {313, kParamMstMix, 10},
  // --- END the MAGS library
};

const keith::PresetSpec kPresetSpecs[kNumPresets] = {
  {"Family 01 / 001 Glass Keys", 1, "Triton", "poly synths"},
  {"Family 01 / 002 Sparse Keys", 1, "Triton", "poly synths"},
  {"Family 01 / 003 Soft Tines", 1, "Triton", "poly synths"},
  {"Family 01 / 004 Bright Clav", 1, "Triton", "poly synths"},
  {"Family 01 / 005 Drawbar Organ", 1, "Triton", "poly synths"},
  {"Family 01 / 006 Glass Bells", 1, "Triton", "poly synths"},
  {"Family 01 / 007 Crystal Pluck", 1, "Triton", "poly synths"},
  {"Family 01 / 008 Key Stab", 1, "Triton", "poly synths"},
  {"Family 01 / 009 Dry Bass", 1, "Triton", "bass"},
  {"Family 01 / 010 Smooth Lead", 1, "Triton", "mono synths"},
  {"Family 01 / 011 Warm Brass", 1, "Triton", "winds"},
  {"Family 01 / 012 Silk Strings", 1, "Triton", "strings"},
  {"Family 01 / 013 Soft Choir", 1, "Triton", "strings"},
  {"Family 01 / 014 Bell Pad", 1, "Triton", "strings"},
  {"Family 01 / 015 Slow Motion", 1, "Triton", "sfx"},
  {"Family 01 / 016 Shimmer Texture", 1, "Triton", "sfx"},
  {"Family 01 / 017 Low Drone", 1, "Triton", "sfx"},
  {"Family 01 / 018 Glass Arp", 1, "Triton", "mono synths"},
  {"Family 01 / 019 Heavy Delay", 1, "Triton", "strings"},
  {"Family 01 / 020 Room Kit", 1, "Triton", "retired drums"},
  {"Family 01 / 021 Upright Bass", 1, "Triton", "bass"},
  {"Family 01 / 022 Hammer Keys", 1, "Triton", "poly synths"},
  {"Family 01 / 023 Swell Choir", 1, "Triton", "sfx"},
  {"Family 01 / 024 Bright Reed", 1, "Triton", "mono synths"},
  {"Family 01 / 025 Snap Bass", 1, "Triton", "bass"},
  {"Family 01 / 026 Short Bass", 1, "Triton", "bass"},
  {"Family 02 / 001 Neon Keys", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 002 Thin Piano", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 003 Thin Clav", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 004 Perc Organ", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 005 Metal Bells", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 006 Neon Pluck", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 007 Sparse Pluck", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 008 Thin Stab", 2, "Triton Extreme", "poly synths"},
  {"Family 02 / 009 Hard Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 010 Sparse Bass", 2, "Triton Extreme", "bass"},
  {"Family 02 / 011 Sync Lead", 2, "Triton Extreme", "mono synths"},
  {"Family 02 / 012 Stab Brass", 2, "Triton Extreme", "winds"},
  {"Family 02 / 013 Tight Strings", 2, "Triton Extreme", "strings"},
  {"Family 02 / 014 Breath Choir", 2, "Triton Extreme", "strings"},
  {"Family 02 / 015 Poly Pad", 2, "Triton Extreme", "strings"},
  {"Family 02 / 016 Sweep Motion", 2, "Triton Extreme", "sfx"},
  {"Family 02 / 017 Skew Texture", 2, "Triton Extreme", "sfx"},
  {"Family 02 / 018 Pulse Drone", 2, "Triton Extreme", "sfx"},
  {"Family 02 / 019 Wide Arp", 2, "Triton Extreme", "mono synths"},
  {"Family 02 / 020 Heavy Crush", 2, "Triton Extreme", "sfx"},
  {"Family 02 / 021 Punch Kit", 2, "Triton Extreme", "retired drums"},
  {"Family 03 / 001 Piano Stack", 3, "Korg M1", "poly synths"},
  {"Family 03 / 002 Soft EP", 3, "Korg M1", "poly synths"},
  {"Family 03 / 003 Soul Piano", 3, "Korg M1", "poly synths"},
  {"Family 03 / 004 Clav Stab", 3, "Korg M1", "poly synths"},
  {"Family 03 / 005 Dry Clav", 3, "Korg M1", "poly synths"},
  {"Family 03 / 006 Warm Organ", 3, "Korg M1", "poly synths"},
  {"Family 03 / 007 Bottle Bells", 3, "Korg M1", "poly synths"},
  {"Family 03 / 008 Muted Pluck", 3, "Korg M1", "poly synths"},
  {"Family 03 / 009 Reed Stab", 3, "Korg M1", "poly synths"},
  {"Family 03 / 010 Finger Bass", 3, "Korg M1", "bass"},
  {"Family 03 / 011 Solo Lead", 3, "Korg M1", "mono synths"},
  {"Family 03 / 012 Ensemble Brass", 3, "Korg M1", "winds"},
  {"Family 03 / 013 Section Strings", 3, "Korg M1", "strings"},
  {"Family 03 / 014 Ooh Choir", 3, "Korg M1", "strings"},
  {"Family 03 / 015 Dream Pad", 3, "Korg M1", "strings"},
  {"Family 03 / 016 Slow Sweep", 3, "Korg M1", "sfx"},
  {"Family 03 / 017 Hollow Texture", 3, "Korg M1", "sfx"},
  {"Family 03 / 018 Warm Drone", 3, "Korg M1", "sfx"},
  {"Family 03 / 019 Mallet Arp", 3, "Korg M1", "mono synths"},
  {"Family 03 / 020 Slap Delay", 3, "Korg M1", "strings"},
  {"Family 03 / 021 Dry Perc", 3, "Korg M1", "retired drums"},
  {"Family 04 / 001 Vector Keys", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 002 Phase Piano", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 003 Wave Clav", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 004 Drift Organ", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 005 Chime Bells", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 006 Harp Pluck", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 007 Soft Stab", 4, "Korg Wavestation", "poly synths"},
  {"Family 04 / 008 Sub Bass", 4, "Korg Wavestation", "bass"},
  {"Family 04 / 009 Air Lead", 4, "Korg Wavestation", "mono synths"},
  {"Family 04 / 010 Swell Brass", 4, "Korg Wavestation", "winds"},
  {"Family 04 / 011 Bowed Strings", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 012 Air Choir", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 013 Cloud Pad", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 014 Sparse Pad", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 015 Motion Pad", 4, "Korg Wavestation", "sfx"},
  {"Family 04 / 016 Tilt Texture", 4, "Korg Wavestation", "sfx"},
  {"Family 04 / 017 Slow Drone", 4, "Korg Wavestation", "sfx"},
  {"Family 04 / 018 Seq Arp", 4, "Korg Wavestation", "mono synths"},
  {"Family 04 / 019 Wash Delay", 4, "Korg Wavestation", "strings"},
  {"Family 04 / 020 Metal Kit", 4, "Korg Wavestation", "retired drums"},
  {"Family 05 / 001 Stage Keys", 5, "Korg TR", "poly synths"},
  {"Family 05 / 002 Reed Piano", 5, "Korg TR", "poly synths"},
  {"Family 05 / 003 Funk Clav", 5, "Korg TR", "poly synths"},
  {"Family 05 / 004 Rock Organ", 5, "Korg TR", "poly synths"},
  {"Family 05 / 005 Ping Bells", 5, "Korg TR", "poly synths"},
  {"Family 05 / 006 Mute Pluck", 5, "Korg TR", "poly synths"},
  {"Family 05 / 007 Short Stab", 5, "Korg TR", "poly synths"},
  {"Family 05 / 008 Round Bass", 5, "Korg TR", "bass"},
  {"Family 05 / 009 Saw Lead", 5, "Korg TR", "mono synths"},
  {"Family 05 / 010 Section Brass", 5, "Korg TR", "winds"},
  {"Family 05 / 011 Small Strings", 5, "Korg TR", "strings"},
  {"Family 05 / 012 Close Choir", 5, "Korg TR", "strings"},
  {"Family 05 / 013 Tape Pad", 5, "Korg TR", "strings"},
  {"Family 05 / 014 Filter Motion", 5, "Korg TR", "sfx"},
  {"Family 05 / 015 Broken Texture", 5, "Korg TR", "sfx"},
  {"Family 05 / 016 Dark Drone", 5, "Korg TR", "sfx"},
  {"Family 05 / 017 Tight Arp", 5, "Korg TR", "mono synths"},
  {"Family 05 / 018 Room Crush", 5, "Korg TR", "sfx"},
  {"Family 05 / 019 Dry Kit", 5, "Korg TR", "retired drums"},
  {"Family 05 / 020 Hard Kit", 5, "Korg TR", "retired drums"},
  {"Family 05 / 021 Tape Kit", 5, "Korg TR", "retired drums"},
// --- BEGIN the MAGS library
  // The MAGS library: 205 patches, grouped by role rather than by
  // emulated family. Measured level trims for these are emitted below once
  // tools/measure_preset_levels.py has run against them.
  //
  {"Thin Glass Lead", 0, "mags", "mono synths"},
  {"Hollow Pulse Lead", 0, "mags", "mono synths"},
  {"Glass Morph Lead", 0, "mags", "mono synths"},
  {"Air Tine Lead", 0, "mags", "mono synths"},
  {"Wide Interval Lead", 0, "mags", "mono synths"},
  {"Nasal Whine", 0, "mags", "mono synths"},
  {"Chime Lead", 0, "mags", "mono synths"},
  {"Soft Glass Solo", 0, "mags", "mono synths"},
  {"Filtered Disco Lead", 0, "mags", "mono synths"},
  {"Talkbox Lead", 0, "mags", "mono synths"},
  {"Sweep Sync Lead", 0, "mags", "mono synths"},
  {"French Touch Lead", 0, "mags", "mono synths"},
  {"Chorus Solo", 0, "mags", "mono synths"},
  {"Robot Voice Lead", 0, "mags", "mono synths"},
  {"Disco Bell Lead", 0, "mags", "mono synths"},
  {"Filter Ride Lead", 0, "mags", "mono synths"},
  {"Exotic Bell Lead", 0, "mags", "mono synths"},
  {"Metal Lead", 0, "mags", "mono synths"},
  {"Hard Dry Lead", 0, "mags", "mono synths"},
  {"Sparse Pluck Lead", 0, "mags", "mono synths"},
  {"Gamelan Lead", 0, "mags", "mono synths"},
  {"Short Marimba Lead", 0, "mags", "mono synths"},
  {"Sharp Sync Lead", 0, "mags", "mono synths"},
  {"Distorted Saw Lead", 0, "mags", "mono synths"},
  {"Overdrive Lead", 0, "mags", "mono synths"},
  {"Grit Pulse Lead", 0, "mags", "mono synths"},
  {"Hard Edge Sync", 0, "mags", "mono synths"},
  {"Comp Stack Lead", 0, "mags", "mono synths"},
  {"Metal Edge Lead", 0, "mags", "mono synths"},
  {"Folded Saw Lead", 0, "mags", "mono synths"},
  {"Silk Solo Lead", 0, "mags", "mono synths"},
  {"Warm Brass Lead", 0, "mags", "mono synths"},
  {"Voice Lead", 0, "mags", "mono synths"},
  {"Sweetening Solo", 0, "mags", "mono synths"},
  {"Soft Tine Solo", 0, "mags", "mono synths"},
  {"Round Brass Lead", 0, "mags", "mono synths"},
  {"Dusty Lead", 0, "mags", "mono synths"},
  {"Detune Warm Lead", 0, "mags", "mono synths"},
  {"Soft Tine Drift Lead", 0, "mags", "mono synths"},
  {"Tape Solo", 0, "mags", "mono synths"},
  {"Warped Choir Lead", 0, "mags", "mono synths"},
  {"Lo-Fi Pulse Lead", 0, "mags", "mono synths"},
  {"Round Warm Solo", 0, "mags", "mono synths"},
  {"Wide Unison Lead", 0, "mags", "mono synths"},
  {"Classic Sync Lead", 0, "mags", "mono synths"},
  {"Bright FM Lead", 0, "mags", "mono synths"},
  {"Narrow Pulse Lead", 0, "mags", "mono synths"},
  {"Table Sweep Lead", 0, "mags", "mono synths"},
  {"Ladder Drive Lead", 0, "mags", "mono synths"},
  {"Stack Detune Lead", 0, "mags", "mono synths"},
  {"Glass Field", 0, "mags", "poly synths"},
  {"Thin Bell Keys", 0, "mags", "poly synths"},
  {"Wide Interval Stab", 0, "mags", "poly synths"},
  {"Clav Ghost", 0, "mags", "poly synths"},
  {"Glass Morph Keys", 0, "mags", "poly synths"},
  {"Hollow Tine", 0, "mags", "poly synths"},
  {"Sparse Chime", 0, "mags", "poly synths"},
  {"Tight Saw Stab", 0, "mags", "poly synths"},
  {"Air Tines", 0, "mags", "poly synths"},
  {"Off-Kilter Pad", 0, "mags", "poly synths"},
  {"Disc Chorus", 0, "mags", "poly synths"},
  {"Filtered Sweep", 0, "mags", "poly synths"},
  {"French Touch Stab", 0, "mags", "poly synths"},
  {"Warm Chorus Keys", 0, "mags", "poly synths"},
  {"Driven Disco Bass", 0, "mags", "poly synths"},
  {"Talkbox Lead Pad", 0, "mags", "poly synths"},
  {"Robot Choir Keys", 0, "mags", "poly synths"},
  {"Neon Clav", 0, "mags", "poly synths"},
  {"Metal Pluck", 0, "mags", "poly synths"},
  {"Exotic Bell", 0, "mags", "poly synths"},
  {"Dry Clav Pluck", 0, "mags", "poly synths"},
  {"Hard Stab", 0, "mags", "poly synths"},
  {"Gamelan Keys", 0, "mags", "poly synths"},
  {"Short Marimba", 0, "mags", "poly synths"},
  {"Plucked Glass", 0, "mags", "poly synths"},
  {"Grit Saw Stack", 0, "mags", "poly synths"},
  {"Grit Comp Stab", 0, "mags", "poly synths"},
  {"Overdriven Fifths", 0, "mags", "poly synths"},
  {"Saturated Sub Stack", 0, "mags", "poly synths"},
  {"Hard Edge Lead Pad", 0, "mags", "poly synths"},
  {"Folded Saw Morph", 0, "mags", "poly synths"},
  {"Dusty Keys", 0, "mags", "poly synths"},
  {"Drifting Pad", 0, "mags", "poly synths"},
  {"Warm Detune Stab", 0, "mags", "poly synths"},
  {"Tape Tines", 0, "mags", "poly synths"},
  {"Warped Choir Keys", 0, "mags", "poly synths"},
  {"Round Low Keys", 0, "mags", "poly synths"},
  {"Soft Tine Drift", 0, "mags", "poly synths"},
  {"Wide Unison Stack", 0, "mags", "poly synths"},
  {"Resonant Poly Brass", 0, "mags", "poly synths"},
  {"Simple Chorus Pad", 0, "mags", "poly synths"},
  {"Ladder Filter Stack", 0, "mags", "poly synths"},
  {"Table Sweep Pad", 0, "mags", "poly synths"},
  {"Tine Piano", 0, "mags", "poly synths"},
  {"Bright Tine", 0, "mags", "poly synths"},
  {"Soft Tine", 0, "mags", "poly synths"},
  {"Tine Bell", 0, "mags", "poly synths"},
  {"Tine Bark", 0, "mags", "poly synths"},
  {"Dusty Tine", 0, "mags", "poly synths"},
  {"Reed Piano", 0, "mags", "poly synths"},
  {"Warm Reed", 0, "mags", "poly synths"},
  {"Reed Bark", 0, "mags", "poly synths"},
  {"Tape Reed", 0, "mags", "poly synths"},
  {"Tonewheel Organ", 0, "mags", "poly synths"},
  {"Full Drawbar", 0, "mags", "poly synths"},
  {"Percussive Organ", 0, "mags", "poly synths"},
  {"Gospel Organ", 0, "mags", "poly synths"},
  {"Dirty Organ", 0, "mags", "poly synths"},
  {"Combo Organ", 0, "mags", "poly synths"},
  {"Reedy Combo", 0, "mags", "poly synths"},
  {"Bright Combo", 0, "mags", "poly synths"},
  {"Tape Choir", 0, "mags", "poly synths"},
  {"Tape Strings", 0, "mags", "poly synths"},
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
  {"Hollow Reed", 0, "mags", "winds"},
  {"Breathy Flute", 0, "mags", "winds"},
  {"Narrow Oboe", 0, "mags", "winds"},
  {"Warm Clarinet", 0, "mags", "winds"},
  {"Airy Pan Flute", 0, "mags", "winds"},
  {"Bright Shawm", 0, "mags", "winds"},
  {"Soft Bassoon", 0, "mags", "winds"},
  {"Dusty Reed", 0, "mags", "winds"},
  {"Thin Whistle", 0, "mags", "winds"},
  {"Wide Reed Section", 0, "mags", "winds"},
  {"Warm Brass", 0, "mags", "winds"},
  {"Bright Brass Section", 0, "mags", "winds"},
  {"Low Brass Bed", 0, "mags", "winds"},
  {"Punch Brass Stab", 0, "mags", "winds"},
  {"Soft French Horn", 0, "mags", "winds"},
  {"Hard Brass Stack", 0, "mags", "winds"},
  {"Tape Brass", 0, "mags", "winds"},
  {"Muted Brass", 0, "mags", "winds"},
  {"Wide Brass Swell", 0, "mags", "winds"},
  {"Sharp Brass Lead", 0, "mags", "winds"},
  {"Ring Mod Sweep", 0, "mags", "sfx"},
  {"Noise Wash", 0, "mags", "sfx"},
  {"Metal Riser", 0, "mags", "sfx"},
  {"Detuned Siren", 0, "mags", "sfx"},
  {"Tape Warble FX", 0, "mags", "sfx"},
  {"Long Delay Drone", 0, "mags", "sfx"},
  {"Broken Radio", 0, "mags", "sfx"},
  {"Sub Drop FX", 0, "mags", "sfx"},
  {"Shimmer Texture", 0, "mags", "sfx"},
  {"Sweep Down FX", 0, "mags", "sfx"},
  {"Choir Ahh", 0, "mags", "vocal"},
  {"Choir Ooh", 0, "mags", "vocal"},
  {"Wide Choir Bed", 0, "mags", "vocal"},
  {"Broken Choir", 0, "mags", "vocal"},
  {"Round Sub Bass", 0, "mags", "bass"},
  {"Bright Bass", 0, "mags", "bass"},
  {"Warm Bottom", 0, "mags", "bass"},
  {"Grit Bass", 0, "mags", "bass"},
  {"Tape Bass", 0, "mags", "bass"},
  {"Silk String Bed", 0, "mags", "strings"},
  {"Warm Brass Section", 0, "mags", "strings"},
  {"Lush Voice Bed", 0, "mags", "strings"},
  {"Sweetening Pad", 0, "mags", "strings"},
  {"Dense Choir Stack", 0, "mags", "strings"},
  {"Rich Low Brass", 0, "mags", "strings"},
  {"Glide Strings", 0, "mags", "strings"},
  {"Slow Motion Pad", 0, "mags", "strings"},
  {"Drifting Warm Pad", 0, "mags", "strings"},
  {"Wide Chorus Pad", 0, "mags", "strings"},
  {"Voice Bed Pad", 0, "mags", "strings"},
  {"Glass Pad", 0, "mags", "strings"},
  {"Deep Low Pad", 0, "mags", "strings"},
  {"Air Motion Pad", 0, "mags", "strings"},
  {"Dark Tape Pad", 0, "mags", "strings"},
  {"Bright Sweep Pad", 0, "mags", "strings"},
  {"Sparse Hollow Pad", 0, "mags", "strings"},
  {"Silk Strings", 0, "mags", "strings"},
  {"Warm Section Strings", 0, "mags", "strings"},
  {"Bright Strings", 0, "mags", "strings"},
  {"Low Cello Bed", 0, "mags", "strings"},
  {"Slow Bow Strings", 0, "mags", "strings"},
  {"Tight Strings", 0, "mags", "strings"},
  {"Section Swell", 0, "mags", "strings"},
  {"Dusty Strings", 0, "mags", "strings"},
  {"Octave Strings", 0, "mags", "strings"},
  {"Thin High Strings", 0, "mags", "strings"},
  {"Frozen Pad", 0, "mags", "strings"},
  {"Hollow Sweep Pad", 0, "mags", "strings"},
  {"Low Bed Pad", 0, "mags", "strings"},
  {"Formant Pad", 0, "mags", "strings"},
  {"Worn Pad", 0, "mags", "strings"},
  {"Glass Drift Pad", 0, "mags", "strings"},
  {"Synthetic Strings", 0, "mags", "strings"},
  {"Velvet Strings", 0, "mags", "strings"},
  {"High Strings", 0, "mags", "strings"},
  {"Deep Strings", 0, "mags", "strings"},
  {"Tape Strings Bed", 0, "mags", "strings"},
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
  // A mono synth, so the palette spans the groups the library actually has. It was all keys,
  // pianos, plucks and textures, which meant a demo built from it had no lead line at all -- and
  // verify_preset_metadata checks that the palette covers a spread of groups and was right to flag
  // the gap. Chosen for the same dry character as the rest rather than added as a token.
  "Family 01 / 010 Smooth Lead",
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
      const uint8_t manual = slice.manual ? 1u : 0u;
      chunk.Put(&startFrame);
      chunk.Put(&midiNote);
      chunk.Put(&manual);
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
  if (magic != kSamplerStateMagic
      || (version != kSamplerStateVersion && version != kSamplerStateVersion - 1))
    return startPos;
  const bool sliceHasManual = version >= 2;

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
  const int bytesPerSlice =
      static_cast<int>(sizeof(int32_t) * 2u + (sliceHasManual ? sizeof(uint8_t) : 0));
  const int remaining = chunk.Size() - pos;
  if (remaining < 0 || sliceCount > remaining / bytesPerSlice)
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
    uint8_t manual = 0;
    if (sliceHasManual && pos >= 0)
      pos = chunk.Get(&manual, pos);
    if (pos < 0)
      return startPos;
    slices[static_cast<std::size_t>(i)] = Slice {startFrame, midiNote, manual != 0};
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

// These two live at FILE scope, not in an anonymous namespace. They were wrapped, and the wrap
// closed at line ~5121 while the member functions calling them are at ~5665 and ~5978 -- outside
// it, so the calls failed to compile with "no matching function". They take `Keith&` as a
// parameter, so they do not need internal linkage to stay out of the way.
/**
 * Push the TEMPO SYNC switch into the engine.
 *
 * The engine cannot read the parameter itself, and the switch has to reach it before any audio is
 * rendered. Called from the same places as PushSliceSettings so the two cannot drift.
 *
 * Turning it on adopts the current slices as the warp map; turning it off clears the map and the
 * sample plays at its recorded rate again. Both directions are driven from the one parameter, so
 * there is no separate state to keep in step.
 */
void PushTempoSync(Keith& plugin)
{
  keith::sampler::SamplerEngine& sampler = plugin.Sampler();
  const bool on = plugin.GetParam(kParamSampleTempoSync)->Int() != 0;
  // Guarded so this does not rebuild the warp map on every unrelated parameter change: adopting
  // the slices allocates, and re-adopting them while the user drags a knob would churn.
  if (on == sampler.TempoSyncEnabled())
    return;
  sampler.SetTempoSyncEnabled(on);
}

/**
 * Push the sampler page's slice settings into the engine.
 *
 * The engine needs these BEFORE a sample loads, because Load() now applies the current slice mode
 * itself. Without this push the engine would always detect at its own defaults, so the SENS,
 * REGIONS and SLICE BY controls would change what the editor shows but not what a load produces.
 *
 * Cheap enough to call on every sampler parameter change: it writes six scalars and allocates
 * nothing. It deliberately does NOT re-slice, so moving a knob does not discard a manual edit --
 * ApplySliceBy does that when the user asks for it.
 */
void PushSliceSettings(Keith& plugin)
{
  keith::sampler::SamplerEngine& sampler = plugin.Sampler();
  const int by = plugin.GetParam(kParamSliceBy)->Int();
  sampler.SetSliceMode(by == 0   ? keith::sampler::SamplerEngine::SliceBy::Transient
                       : by == 1 ? keith::sampler::SamplerEngine::SliceBy::Region
                                 : keith::sampler::SamplerEngine::SliceBy::Manual);
  sampler.SetSliceSensitivity(
      static_cast<float>(plugin.GetParam(kParamSliceSensitivity)->Value() / 100.0));
  sampler.SetSliceRegions(plugin.GetParam(kParamSliceRegions)->Int());
  sampler.SetSliceRegionNormalised(plugin.GetParam(kParamSampleRegionStart)->Value(),
                                  plugin.GetParam(kParamSampleRegionEnd)->Value());
}

void SyncSamplerPageParams(Keith& plugin)
{
  keith::sampler::SamplerEngine& sampler = plugin.Sampler();

  // Slice settings go in FIRST, before the no-sample early return below.
  //
  // They have to be in the engine before the first sample arrives: Load() applies the current
  // slice mode as it loads, so if the engine has not been told what mode is wanted it detects at
  // its own defaults and the SLICE BY / SENS / REGIONS controls do nothing on a fresh load. This
  // early return used to skip them along with the frame work, which needs a sample to exist.
  PushSliceSettings(plugin);
  PushTempoSync(plugin);

  if (!sampler.HasSample())
    return;
  const int frames = sampler.Sample()->frames;
  int r0 = static_cast<int>(std::lround(plugin.GetParam(kParamSampleRegionStart)->Value() / 100.0 * frames));
  int r1 = static_cast<int>(std::lround(plugin.GetParam(kParamSampleRegionEnd)->Value() / 100.0 * frames));
  r0 = std::clamp(r0, 0, std::max(0, frames - 1));
  r1 = std::clamp(r1, r0 + 1, frames);
  sampler.SetRegionFrames(r0, r1);
  const int span = r1 - r0;
  const int p0 =
      r0 + static_cast<int>(plugin.GetParam(kParamSamplePlayStart)->Value() / 100.0 * span);
  const int p1 =
      r0 + static_cast<int>(plugin.GetParam(kParamSamplePlayEnd)->Value() / 100.0 * span);
  sampler.SetPlayWindow(p0, std::max(p0, p1 - 1));
  sampler.SetSampleGain(static_cast<float>(plugin.GetParam(kParamSampleGain)->Value() / 100.0));
  sampler.SetSampleRoot(plugin.GetParam(kParamSampleRoot)->Int());
  sampler.SetSampleTuneSemitones(plugin.GetParam(kParamSampleTune)->Value());
}

void ApplySliceBy(Keith& plugin)
{
  keith::sampler::SamplerEngine& sampler = plugin.Sampler();
  if (!sampler.HasSample())
    return;
  // The engine acts on the same settings Load() would use, so what a knob change produces and
  // what a fresh load produces cannot diverge.
  PushSliceSettings(plugin);
  PushTempoSync(plugin);
  const int frames = sampler.Sample()->frames;
  int r0 = static_cast<int>(std::lround(plugin.GetParam(kParamSampleRegionStart)->Value() / 100.0 * frames));
  int r1 = static_cast<int>(std::lround(plugin.GetParam(kParamSampleRegionEnd)->Value() / 100.0 * frames));
  r1 = std::clamp(r1, r0 + 1, frames);
  const int by = plugin.GetParam(kParamSliceBy)->Int();
  if (by == 0)
  {
    keith::sampler::SliceSettings settings;
    settings.sensitivity =
        static_cast<float>(plugin.GetParam(kParamSliceSensitivity)->Value() / 100.0);
    sampler.DetectSlices(settings, true);
  }
  else if (by == 1)
  {
    sampler.SetSlices(keith::sampler::RegionSlices(r0, r1, plugin.GetParam(kParamSliceRegions)->Int(),
                                                   frames));
  }
}

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
  try
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
  catch (const std::exception& e)
  {
    DBGMSG("Keith: OnRestoreState threw: %s\n", e.what());
  }
  catch (...)
  {
    DBGMSG("Keith: OnRestoreState threw (unknown)\n");
  }
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
                && kParamSampleReverse == 119 && kParamDrumKit == 120 && kParamPercKit == 121
                && kParamVocalEnable == 122 && kParamGuitarBank == 123 && kParamGuitarLevel == 124
                && kParamSampleRegionStart == 125 && kParamSliceRegions == 134
                && kParamSampleTempoSync == 135 && kNumParams == 136,
              "U18-U21 appends through sampler page controls");

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

  // The ten drum kits, in the order the loader lists them. The names describe the machine
  // character rather than naming any artist or product -- see banks/SOURCES.md rule 7.
  GetParam(kParamDrumKit)->InitEnum("Drum Kit", 0,
    {"Smoke", "Velvet", "Stone", "Dust", "Paper", "Late", "Corner", "Crate", "Swing", "Clinic"});
  GetParam(kParamPercKit)->InitEnum("Percussion Kit", 0, {"Hands", "Metal"});
  GetParam(kParamVocalEnable)->InitBool("Vocal Layer", false);

  // The sampled guitar banks. "Off" first so the default claims nothing: a bank that loads on
  // startup takes the MIDI range over, which is what the owner reported happening with the drums.
  GetParam(kParamGuitarBank)->InitEnum("Guitar Bank", 0,
    {"Off", "Clean Electric", "Jazz Clean", "Distorted 1", "Distorted 2", "Nylon"});
  GetParam(kParamGuitarLevel)->InitDouble("Guitar Level", 0., -60., 12., 0.01, "dB");

  GetParam(kParamSampleRegionStart)->InitDouble("Sample Region Start", 0., 0., 100., 0.1, "%");
  GetParam(kParamSampleRegionEnd)->InitDouble("Sample Region End", 100., 0., 100., 0.1, "%");
  GetParam(kParamSamplePlayStart)->InitDouble("Sample Play Start", 0., 0., 100., 0.1, "%");
  GetParam(kParamSamplePlayEnd)->InitDouble("Sample Play End", 100., 0., 100., 0.1, "%");
  GetParam(kParamSampleRoot)->InitInt("Sample Root", 60, 0, 127);
  GetParam(kParamSampleTune)->InitDouble("Sample Tune", 0., -48., 48., 0.01, "st");
  GetParam(kParamSampleGain)->InitDouble("Sample Gain", 100., 0., 200., 0.1, "%");
  GetParam(kParamSliceBy)->InitEnum("Slice By", 0, {"Transient", "Region", "Manual"});
  GetParam(kParamSliceSensitivity)->InitDouble("Slice Sensitivity", 50., 0., 100., 0.1, "%");
  GetParam(kParamSliceRegions)->InitInt("Slice Regions", 8, 2, 64);
  // Off by default: tempo sync changes how the sample PLAYS, so it should be an explicit choice
  // rather than the state a fresh patch lands in.
  GetParam(kParamSampleTempoSync)->InitEnum("Sample Tempo Sync", 0, {"Off", "On"});

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
  try
  {
    // The deferred sample load, once, on the first idle tick after a reset -- by which point the
    // host has finished bringing the plugin up, so the resource-path call and the file reads are
    // both safe.
    if (mLoadRequested.exchange(false))
    {
      LoadDrumKit();
      LoadPercussionKit();
      LoadGuitarBank();
      LoadVocalBank();
    }

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
  catch (const std::exception& e)
  {
    DBGMSG("Keith: OnIdle threw: %s\n", e.what());
  }
  catch (...)
  {
    DBGMSG("Keith: OnIdle threw (unknown)\n");
  }
}
#endif

#if IPLUG_DSP
void Keith::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  try
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
    SyncSamplerPageParams(*this);

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
  catch (...)
  {
    keith::detail::NoteAudioFailureOnce("ProcessBlock");
  }
}

std::string Keith::SampleDir(const char* kind) const
{
  // The sample content is copied into the bundle's resources by CMake, and the three formats put
  // that in different places. BundleResourcePath is iPlug2's answer for all of them -- there is no
  // GetBundlePath(), which is what this first called and the compiler rejected.
  //
  // An environment override comes FIRST, and it is not a convenience. The offline preset renderer
  // is a plain command-line binary with no bundle, and BundleResourcePath on macOS hands the path
  // to +[NSString stringWithUTF8String:], which throws NSInvalidArgumentException on a null
  // cString -- the renderer died on the first preset. KEITH_SAMPLE_DIR lets the test harness and
  // the render point at the samples directly, with no bundle machinery involved.
  if (const char* override = std::getenv("KEITH_SAMPLE_DIR"))
    if (override[0] != 0)
      return std::string(override) + "/" + kind;

  // Resolved once and cached: BundleResourcePath goes into Cocoa, so calling it on every load is
  // needless risk and the answer cannot change while the plugin is running.
  if (mResourceRoot.empty())
  {
    WDL_String path;
    // The second argument is PLATFORM-DEPENDENT, and both forms matter:
    //
    //   macOS/iOS:  PluginIDType is `const char*`, the bundle identifier. Omitting it passes NULL,
    //               and IPlugPaths.mm then calls
    //                   [NSBundle bundleWithIdentifier:[NSString stringWithUTF8String:bundleID]]
    //               which RAISES NSInvalidArgumentException on a null cString. An Objective-C
    //               exception is not caught by our `catch (std::exception&)` or `catch (...)`
    //               guards, so it reached std::terminate and aborted the HOST -- the Live crash
    //               showed terminate() inside Live's own timer with no MagsSynth frame anywhere.
    //
    //   Windows:    PluginIDType is `HMODULE` -- a module handle, NOT a string. Passing BUNDLE_ID
    //               there is a compile error (C2664: cannot convert 'const char[1]' to HMODULE),
    //               which is how the first attempt at this fix failed the Windows build.
    //
    // So macOS passes the identifier and Windows passes nullptr, which its own implementation
    // accepts. BUNDLE_ID is derived by iPlug2 from BUNDLE_DOMAIN/BUNDLE_MFR/BUNDLE_NAME in
    // config.h and expands per format -- audio.Mags.audiounit.MagsSynth for the AU, which is
    // exactly the CFBundleIdentifier named in the crash report.
#if defined OS_MAC || defined OS_IOS
    BundleResourcePath(path, BUNDLE_ID);
#else
    BundleResourcePath(path, nullptr);
#endif
    if (path.GetLength() > 0)
      mResourceRoot = path.Get();
  }
  if (mResourceRoot.empty())
  {
    // No bundle to resolve against -- a standalone binary outside a .app, say. Return a path that
    // simply does not exist rather than one that crashes: a missing kit is a silent pad, which is
    // recoverable, and the loader reports it.
    DBGMSG("Keith: no bundle resource path; set KEITH_SAMPLE_DIR to locate the samples\n");
    return std::string("samples/") + kind;
  }
  return mResourceRoot + "/samples/" + kind;
}

void Keith::LoadDrumKit()
{
  try
  {
    // Wrapped because this runs during load: a filesystem error must degrade to a silent kit
    // rather than take the process down.
    static const char* const kKits[] = {
      "smoke", "velvet", "stone", "dust", "paper", "late", "corner", "crate", "swing", "clinic",
    };
    const int want =
        std::max(0, std::min<int>(9, static_cast<int>(GetParam(kParamDrumKit)->Value())));
    if (want == mLoadedKitIndex && mLoadedKit)
      return;   // already there; reloading on every parameter change would stutter

    const std::string dir = SampleDir("drums") + "/" + kKits[want];
    keith::drums::KitLoadReport report;
    auto kit = keith::drums::LoadKit(dir, report);

    // Report rather than fail silently: a kit with a missing slot is audible as a dead pad, and
    // guessing which one from the sound alone is slow.
    if (!report.Ok())
    {
      DBGMSG("drum kit %s: %d slots, %d missing, %d failed\n", kKits[want], report.slotsLoaded,
             static_cast<int>(report.missing.size()), static_cast<int>(report.failed.size()));
      for (const std::string& m : report.missing)
        DBGMSG("  missing slot: %s\n", m.c_str());
    }

    mLoadedKit = kit;
    mLoadedKitIndex = want;
    mDSP.SetDrumKit(kit);
  }
  catch (const std::exception& e) { DBGMSG("drum kit: load failed: %s\n", e.what()); }
  catch (...) { DBGMSG("drum kit: load failed (unknown)\n"); }
}

void Keith::LoadGuitarBank()
{
  try
  {
    // Wrapped because this runs during load. A filesystem error or an unreadable file must
    // degrade to silence rather than take the process down -- "Mags quit unexpectedly" on open was
    // this code path without the guard.
    static const char* const kBanks[] = {"", "clean", "jazz", "dist1", "dist2", "nylon"};
    const int want =
        std::max(0, std::min<int>(5, static_cast<int>(GetParam(kParamGuitarBank)->Value())));

    if (want == mLoadedGuitarBankIndex)
      return;   // the same guard the drum kit uses: OnParamChange fires on every automated move

    if (want == 0)
    {
      // Off: publish nothing rather than leaving the old bank in place, so the layer really stops
      // sounding instead of continuing to answer notes after the control says Off.
      mLoadedGuitarBankIndex = 0;
      mDSP.SetInstrumentBank(nullptr);
      return;
    }

    const std::string dir = SampleDir("guitars") + "/" + kBanks[want];
    keith::instruments::BankLoadReport report;
    auto bank = keith::instruments::LoadBank(dir, report);
    if (!report.Ok())
      DBGMSG("guitar bank %s: %d zones, %d failed -- %s\n", kBanks[want], report.zonesLoaded,
             static_cast<int>(report.failed.size()), report.error.c_str());
    else
      DBGMSG("guitar bank %s: %d zones, MIDI %d..%d\n", kBanks[want], report.zonesLoaded,
             report.lowNote, report.highNote);

    mLoadedGuitarBankIndex = want;
    mDSP.SetInstrumentBank(bank);
    SyncGuitarLevel();
  }
  catch (const std::exception& e) { DBGMSG("guitar bank: load failed: %s\n", e.what()); }
  catch (...) { DBGMSG("guitar bank: load failed (unknown)\n"); }
}

void Keith::LoadVocalBank()
{
  // The choir lives under the `a` vowel, which is the only vowel the CC0 source has sustains for.
  // An Ooh was left out rather than synthesised by filtering the Aah, which would be inventing a
  // recording. See resources/samples/vocal/README.md.
  try
  {
    // Wrapped because this runs during load. A filesystem error or an unreadable file must
    // degrade to silence rather than take the process down -- "Mags quit unexpectedly" on open was
    // this code path without the guard.
    const std::string dir = SampleDir("vocal") + "/a";
    keith::instruments::BankLoadReport report;
    auto bank = keith::instruments::LoadBank(dir, report);
    if (!report.Ok())
    {
      DBGMSG("vocal bank: %d zones -- %s\n", report.zonesLoaded, report.error.c_str());
      return;
    }
    DBGMSG("vocal bank: %d zones, MIDI %d..%d\n", report.zonesLoaded, report.lowNote,
           report.highNote);
    mDSP.SetVocalBank(bank);
  }
  catch (const std::exception& e) { DBGMSG("vocal bank: load failed: %s\n", e.what()); }
  catch (...) { DBGMSG("vocal bank: load failed (unknown)\n"); }
}

void Keith::SyncGuitarLevel()
{
  const double db = GetParam(kParamGuitarLevel)->Value();
  mDSP.SetInstrumentGain(static_cast<float>(db <= -60. ? 0. : std::pow(10., db / 20.)));
}

void Keith::LoadPercussionKit()
{
  try
  {
    static const char* const kKits[] = {"hands", "metal"};
    const int want =
        std::max(0, std::min<int>(1, static_cast<int>(GetParam(kParamPercKit)->Value())));
    const std::string dir = SampleDir("percussion") + "/" + kKits[want];

    keith::drums::KitLoadReport report;
    auto kit = keith::drums::LoadKit(dir, report);
    if (!report.Ok())
      DBGMSG("percussion kit %s: %d slots, %d missing\n", kKits[want], report.slotsLoaded,
             static_cast<int>(report.missing.size()));

    // Slid above the drum range, so a percussion kit never contends for a pad slot.
    mDSP.SetPercussionKit(kit, keith::drums::kPercussionFirstSlot);
  }
  catch (const std::exception& e) { DBGMSG("percussion kit: load failed: %s\n", e.what()); }
  catch (...) { DBGMSG("percussion kit: load failed (unknown)\n"); }
}

void Keith::OnReset()
{
  try
  {
    mDSP.Reset(GetSampleRate(), GetBlockSize());

    // The sample content is loaded on the FIRST IDLE TICK, not here. This is the fix for the plugin
    // crashing on open with "Mags quit unexpectedly".
    //
    // OnReset runs while the plugin is still being initialised. Loading a kit calls
    // BundleResourcePath, which on macOS goes into Cocoa, and calling Cocoa from inside plugin
    // initialisation is not safe -- the process died rather than reporting a failure. It also reads
    // several hundred files synchronously, which on its own can make a host time out and mark the
    // plugin as failed to load.
    //
    // Reset clears the engines, so the kit is re-requested here and picked up on that first tick.
    mLoadRequested.store(true);
    mLoadedKitIndex = -1;
    mLoadedGuitarBankIndex = -1;
    // The sampler needs the host rate so a file at 48 kHz plays at the right speed in a 44.1 kHz
    // host. It is set here rather than at construction because the rate is not known until now.
    mSampler.SetSampleRate(GetSampleRate());
    SyncTapeParams();
    SyncSynthParams();
    SyncFxParams();
    SyncDrumParams();
    mDSP.SetGain(GetParam(kParamGain)->Value());
  }
  catch (const std::exception& e)
  {
    DBGMSG("Keith: OnReset threw: %s\n", e.what());
  }
  catch (...)
  {
    DBGMSG("Keith: OnReset threw (unknown)\n");
  }
}

void Keith::ProcessMidiMsg(const IMidiMsg& msg)
{
  try
  {
    mDSP.ProcessMidiMsg(msg);
  }
  catch (const std::exception& e)
  {
    DBGMSG("Keith: ProcessMidiMsg threw: %s\n", e.what());
  }
  catch (...)
  {
    DBGMSG("Keith: ProcessMidiMsg threw (unknown)\n");
  }
}

void Keith::OnParamChange(int paramIdx)
{
  try
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

    if (paramIdx == kParamGuitarBank)
    {
      LoadGuitarBank();
      return;
    }

    if (paramIdx == kParamVocalEnable)
    {
      // The bank is loaded once at reset; this only flips the layer on and off. Loading on every
      // toggle would re-read 14 files each time the switch moved.
      mDSP.SetVocalEnabled(GetParam(paramIdx)->Bool());
      return;
    }

    if (paramIdx == kParamGuitarLevel)
    {
      SyncGuitarLevel();
      return;
    }

    if (paramIdx == kParamPercKit)
    {
      LoadPercussionKit();
      return;
    }

    if (paramIdx == kParamDrumKit)
    {
      // Reloads only when the index actually moved -- LoadDrumKit checks, because OnParamChange
      // fires for every automated change and loading ten WAVs per block would be a stutter.
      LoadDrumKit();
      mDrumUiDirty.store(true);
      return;
    }

    if (paramIdx >= kParamSampleRegionStart && paramIdx <= kParamSampleGain)
    {
      SyncSamplerPageParams(*this);
      return;
    }

    if (paramIdx == kParamSliceBy || paramIdx == kParamSliceSensitivity
        || paramIdx == kParamSliceRegions)
    {
      ApplySliceBy(*this);
      return;
    }

    // TEMPO SYNC is its own branch: switching it does not re-slice, it adopts the slices that
    // already exist as a warp map (or clears the map). Going through ApplySliceBy would redetect
    // and discard a manual edit as a side effect of pressing a tempo button.
    if (paramIdx == kParamSampleTempoSync)
    {
      PushTempoSync(*this);
      return;
    }

    SyncSynthParams();
  }
  catch (const std::exception& e)
  {
    // The host calls this for every parameter move, including automation. An exception escaping
    // into the host calls std::terminate and takes the whole session down, so it is absorbed and
    // reported here instead.
    DBGMSG("Keith: OnParamChange threw: %s\n", e.what());
  }
  catch (...)
  {
    DBGMSG("Keith: OnParamChange threw (unknown)\n");
  }
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
  // The per-slot params (level, pan, tune, mute, solo) are applied by the DSP against the loaded
  // kit. The kit itself is not passed here: it changes rarely and is published by LoadDrumKit,
  // which avoids handing a shared_ptr across on every knob move.
  mDSP.SetDrumParams(mDrumKit);
}
#endif
