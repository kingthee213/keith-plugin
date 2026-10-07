#pragma once

#include "IPlug_include_in_plug_hdr.h"

const int kNumPresets = 15;

enum EControlTags
{
  kCtrlTagKeyboard = 0,
  kCtrlTagPresetManager = 1,
  kCtrlTagStatusText = 2,
  kCtrlTagFamilyChipBase = 10 // +0 .. +4 for Family 01..05
};

/** Baked factory presets. Neutral family labels only (BRIEF §6.5) - no reference-machine names. */
namespace keith
{
constexpr int kNumFamilies = 5;

struct PresetSpec
{
  const char* name;
  int family; // 1..kNumFamilies
  const char* emulatedFamily; // internal provenance; never shown in the UI
  const char* category; // neutral sound role within the emulated family
};

/** @return the baked preset table; @param count receives kNumPresets.
 * Rows are ordered by emulatedFamily first, then neutral sound category. */
const PresetSpec* BakedPresets(int& count);

/** @return the first preset index of a family, or -1 if the family has none */
int FirstPresetInFamily(int family);

/** @return the family (1..5) of a preset index */
int FamilyForPreset(int presetIdx);
} // namespace keith

// Stable host parameter IDs. U1/U4 IDs 0–8 preserved (Gain + tape bus).
// Synth surface appended from 9. See docs/PARAM-MANIFEST.md and U7 report for migration.
enum EParams
{
  kParamGain = 0,

  // Global tape bus (PARAM-MANIFEST fx_tape_*) — IDs 1–8 unchanged from U4
  kParamTapeSat = 1,
  kParamTapeBump = 2,
  kParamTapeWow = 3,
  kParamTapeFlutter = 4,
  kParamTapeHiss = 5,
  kParamTapePrint = 6,
  kParamTapeMix = 7,
  kParamTapeBypass = 8,

  // Synth voice (U7)
  kParamEngine = 9,
  kParamPolyMode = 10,
  kParamPorta = 11,
  kParamUnisonVoices = 12,
  kParamUnisonDetune = 13,

  kParamOsc1Shape = 14,
  kParamOsc1Pitch = 15,
  kParamOsc1Level = 16,
  kParamOsc1PW = 17,

  kParamOsc2Shape = 18,
  kParamOsc2Pitch = 19,
  kParamOsc2Level = 20,
  kParamOsc2PW = 21,

  kParamSubLevel = 22,
  kParamNoiseLevel = 23,
  kParamFmAmount = 24,

  kParamFilterType = 25,
  kParamFilterCutoff = 26,
  kParamFilterReso = 27,
  kParamFilterEnv = 28,

  kParamAmpAttack = 29,
  kParamAmpDecay = 30,
  kParamAmpSustain = 31,
  kParamAmpRelease = 32,

  kParamPitchAttack = 33,
  kParamPitchDecay = 34,
  kParamPitchSustain = 35,
  kParamPitchRelease = 36,
  kParamPitchAmount = 37,

  kParamSynthMasterLevel = 38,

  kParamFiltAttack = 39,
  kParamFiltDecay = 40,
  kParamFiltSustain = 41,
  kParamFiltRelease = 42,

  // U9 appends. IDs 0–42 stay put.
  kParamOscSync = 43,
  kParamOscRing = 44,
  kParamFilterKeyTrack = 45,
  kParamFilterDrive = 46,
  kParamLfoRate = 47,
  kParamLfoShape = 48,
  kParamLfoDest = 49,
  kParamLfoAmount = 50,
  kParamModWheelDest = 51,
  kParamModWheelAmount = 52,
  kParamVelToAmp = 53,
  kParamVelToFilter = 54,
  kParamMonoLegato = 55,

  kNumParams
};

#if IPLUG_DSP
#include "Keith_DSP.h"
#endif

using namespace iplug;

#if IPLUG_EDITOR
using namespace igraphics;
#endif

class Keith final : public Plugin
{
public:
  Keith(const InstanceInfo& info);

private:
  /** Builds the baked factory presets and adopts preset 0 as the init state. */
  void AddFactoryPresets();

#if IPLUG_EDITOR
public:
  void OnIdle() override;
#endif

#if IPLUG_DSP
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;

private:
  void SyncTapeParams();
  void SyncSynthParams();
  KeithDSP<sample> mDSP;
#endif

#if IPLUG_EDITOR
private:
  int mLastPreset = -1;
#endif
};
