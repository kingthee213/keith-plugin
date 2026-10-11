#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "drums/DrumSynth.h"

#include <atomic>

const int kNumPresets = 109;

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

  // U10 appends. IDs 0–55 stay put.
  kParamIns1Algo = 56,
  kParamIns1A = 57,
  kParamIns1B = 58,
  kParamIns1C = 59,
  kParamIns1Mix = 60,
  kParamIns1Bypass = 61,

  kParamIns2Algo = 62,
  kParamIns2A = 63,
  kParamIns2B = 64,
  kParamIns2C = 65,
  kParamIns2Mix = 66,
  kParamIns2Bypass = 67,

  kParamMstAlgo = 68,
  kParamMstA = 69,
  kParamMstB = 70,
  kParamMstC = 71,
  kParamMstMix = 72,
  kParamMstBypass = 73,
  kParamOutLevel = 74,

  // U11 appends. IDs 0-74 stay put.
  kParamDrumSlotSelect = 75,
  kParamDrumSlotLevel = 76,
  kParamDrumSlotPan = 77,
  kParamDrumSlotTune = 78,
  kParamDrumSlotDecay = 79,
  kParamDrumSlotFilter = 80,
  kParamDrumSlotDrive = 81,
  kParamDrumSlotMute = 82,
  kParamDrumSlotSolo = 83,
  kParamDrumKitLevel = 84,

  // U13 appends. IDs 0-84 stay put.
  kParamLfo2Rate = 85,
  kParamLfo2Shape = 86,
  kParamLfo2Dest = 87,
  kParamLfo2Amount = 88,

  // U15 appends. IDs 0-88 stay put.
  kParamMod1Source = 89,
  kParamMod1Dest = 90,
  kParamMod1Amount = 91,
  kParamMod2Source = 92,
  kParamMod2Dest = 93,
  kParamMod2Amount = 94,
  kParamMod3Source = 95,
  kParamMod3Dest = 96,
  kParamMod3Amount = 97,
  kParamMod4Source = 98,
  kParamMod4Dest = 99,
  kParamMod4Amount = 100,

  // U17 appends. IDs 0-100 stay put.
  kParamNoiseKey = 101,
  kParamAmpAttackKey = 102,
  kParamAmpAttackCurve = 103,
  kParamAmpDecayKey = 104,
  kParamAmpDecayCurve = 105,
  kParamStereoDelay = 106,
  kParamStereoDelayKey = 107,
  kParamStereoSpread = 108,
  kParamDirectLevel = 109,
  kParamFilterKeyCurve = 110,

  // Third oscillator. IDs 0-110 stay put, as every append block before this one has.
  //
  // Four controls match osc 1 and 2 -- shape, pitch, level, pulse width -- so the voice can
  // treat all three identically rather than special-casing the new one.
  //
  // Osc 3 carries three extra controls the first two do not need:
  //
  //   Source  picks the built-in wave shapes or the wavetable bank. A separate switch rather
  //           than overloading shape, so the existing shape range keeps its meaning and the
  //           wavetable can be reached without shrinking it.
  //   Morph   walks the wavetable frames. One control, not one per frame.
  //   Enable  because the existing two oscillators are always on, and level-at-zero is the
  //           only way to silence them. A third oscillator that always summed into the mix
  //           would change the sound of every existing preset, so it defaults to OFF.
  kParamOsc3Shape = 111,
  kParamOsc3Pitch = 112,
  kParamOsc3Level = 113,
  kParamOsc3PW = 114,
  kParamOsc3Enable = 115,
  /** Morph position across the wavetable frames. Only meaningful when the bank is loaded. */
  kParamOsc3Morph = 116,
  /** Which source osc 3 draws from: the built-in shapes, or the wavetable bank. */
  kParamOsc3Source = 117,

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
  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
  void OnRestoreState() override;

  int SelectedDrumSlot() const { return mSelectedDrumSlot.load(); }

  /**
   * PAD MIDI: whether MIDI in the drum range plays the drum pads.
   *
   * ON  - notes 36-51 trigger the pads. Nothing else is affected.
   * OFF - those notes do not reach the pads; they fall through to the synth like any
   *       other note.
   *
   * Editor-only state, deliberately not a saved parameter: it is how you are playing right
   * now, not part of the sound, so a patch or project does not recall it. Held as an atomic
   * because the editor sets it and the audio thread reads it.
   */
  bool PadMidiEnabled() const { return mPadMidiEnabled.load(); }
  void SetPadMidiEnabled(bool on) { mPadMidiEnabled.store(on); }

  /**
   * Fire a drum pad from the editor, and move the selected slot to it.
   *
   * The editor cannot reach the DSP's drum queue -- it lives on the audio thread -- so it
   * records the slot and velocity here and Keith::ProcessBlock hands them over. A slot of -1
   * means nothing is pending.
   */
  void TriggerDrumPad(int slot, float velocity)
  {
    mPendingPadVelocity.store(velocity);
    mPendingPadSlot.store(slot);        // written last: the DSP reads this to decide
  }

  int TakePendingDrumPad(float& velocity)
  {
    const int slot = mPendingPadSlot.exchange(-1);
    if (slot >= 0)
      velocity = mPendingPadVelocity.load();
    return slot;
  }

  /**
   * Hits on a drum pad since the plugin loaded, for the editor to flash the pad.
   *
   * A running count rather than a flag: the editor keeps the last value it saw and lights the
   * pad when it changes, so a pad struck twice in quick succession flashes twice. Reading it
   * needs no lock, and the pad does not have to be the selected slot to light up.
   */
  uint64_t DrumPadHitCount(int slot) const { return mDSP.PadHitCount(slot); }
  bool DrumSlotMuted(int slot) const
  {
    return slot >= 0 && slot < keith::drums::kNumSlots && mDrumKit.slot[slot].mute;
  }
  bool DrumSlotSoloed(int slot) const
  {
    return slot >= 0 && slot < keith::drums::kNumSlots && mDrumKit.slot[slot].solo;
  }

private:
  /** Builds the baked factory presets and adopts preset 0 as the init state. */
  void AddFactoryPresets();
  void LoadSelectedDrumSlotParams(int slot);
  void StoreSelectedDrumSlotParam(int paramIdx);
  void ResetDrumKitToDefaults();
  void UpdateDrumUi();
  static void SerializeDrumKit(IByteChunk& chunk, const keith::drums::KitParams& kit);
  static int UnserializeDrumKit(const IByteChunk& chunk, int startPos,
                               keith::drums::KitParams& kit);

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
  void SyncFxParams();
  void SyncDrumParams();
  KeithDSP<sample> mDSP;
#endif

#if IPLUG_EDITOR
private:
  int mLastPreset = -1;
  int mLastFxAlgo[3] {-1, -1, -1};
#endif

  keith::drums::KitParams mDrumKit = keith::drums::DefaultKitParams();
  std::atomic<int> mSelectedDrumSlot {0};
  /** PAD MIDI. Not serialised -- see PadMidiEnabled(). */
  std::atomic<bool> mPadMidiEnabled {true};
  /** A pad fired from the editor, handed to the DSP on the next block. -1 = nothing. */
  std::atomic<int> mPendingPadSlot {-1};
  std::atomic<float> mPendingPadVelocity {1.f};
  /**
   * Drum pad flash bookkeeping. The editor polls the DSP's hit counters and repaints for
   * this many UI frames after a hit, so the lit pad fades instead of staying lit. At the
   * usual 30 fps idle this is about a fifth of a second.
   */
  static constexpr int kPadFlashFrames = 7;
  uint64_t mLastPadHits[keith::drums::kNumSlots] {};
  int mPadFlashFrames = 0;
  std::atomic<bool> mDrumUiDirty {false};
};
