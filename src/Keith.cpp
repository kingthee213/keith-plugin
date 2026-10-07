#include "Keith.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <initializer_list>

#include "editor/EditorialLayout.h"

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
  {0, kParamEngine, 0},        {0, kParamOsc1Shape, 20},   {0, kParamOsc1Level, 90},
  {0, kParamOsc2Shape, 10},    {0, kParamOsc2Pitch, 12},   {0, kParamOsc2Level, 55},
  {0, kParamFilterCutoff, 9000}, {0, kParamFilterReso, 8}, {0, kParamFilterEnv, 30},
  {0, kParamAmpAttack, 8},     {0, kParamAmpDecay, 800},   {0, kParamAmpSustain, 35},
  {0, kParamAmpRelease, 900},  {0, kParamUnisonVoices, 2}, {0, kParamUnisonDetune, 18},
  {0, kParamTapeSat, 20},      {0, kParamTapeMix, 30},

  // Family 01 / 002 Dry Bass
  {1, kParamPolyMode, 1},      {1, kParamOsc1Shape, 62},   {1, kParamOsc1Pitch, -12},
  {1, kParamOsc1Level, 100},   {1, kParamOsc2Shape, 66},   {1, kParamOsc2Pitch, -12},
  {1, kParamOsc2Level, 70},    {1, kParamSubLevel, 70},    {1, kParamFilterCutoff, 1400},
  {1, kParamFilterReso, 22},   {1, kParamFilterEnv, 55},   {1, kParamAmpAttack, 2},
  {1, kParamAmpDecay, 300},    {1, kParamAmpSustain, 80},  {1, kParamAmpRelease, 180},
  {1, kParamTapeSat, 35},      {1, kParamTapeMix, 25},

  // Family 01 / 003 Bell Pad
  {2, kParamOsc1Shape, 8},     {2, kParamOsc2Shape, 88},   {2, kParamOsc2Pitch, 19},
  {2, kParamFmAmount, 25},     {2, kParamFilterCutoff, 6000}, {2, kParamAmpAttack, 400},
  {2, kParamAmpDecay, 2500},   {2, kParamAmpSustain, 60},  {2, kParamAmpRelease, 2500},
  {2, kParamUnisonVoices, 3},  {2, kParamUnisonDetune, 22}, {2, kParamSynthMasterLevel, -3},
  {2, kParamIns1Algo, 3},      {2, kParamIns1A, 45},       {2, kParamIns1B, 32},
  {2, kParamIns1C, 68},        {2, kParamIns1Mix, 24},

  // Family 02 / 001 Neon Pluck
  {3, kParamOsc1Shape, 70},    {3, kParamOsc2Shape, 40},   {3, kParamOsc2Pitch, 12},
  {3, kParamFilterCutoff, 3500}, {3, kParamFilterReso, 30}, {3, kParamFilterEnv, 75},
  {3, kParamFiltAttack, 1},    {3, kParamFiltDecay, 180},  {3, kParamFiltSustain, 0},
  {3, kParamFiltRelease, 200},   {3, kParamAmpAttack, 2},    {3, kParamAmpDecay, 220},
  {3, kParamAmpSustain, 0},    {3, kParamAmpRelease, 250},
  {3, kParamFilterKeyTrack, 75},

  // Family 02 / 002 Thin Stab
  {4, kParamPolyMode, 1},      {4, kParamOsc1Shape, 55},   {4, kParamOsc1Level, 85},
  {4, kParamOsc2Shape, 100},   {4, kParamOsc2PW, 20},      {4, kParamOsc2Level, 60},
  {4, kParamFilterCutoff, 5200}, {4, kParamFilterReso, 18}, {4, kParamFilterEnv, 50},
  {4, kParamAmpAttack, 1},     {4, kParamAmpDecay, 120},   {4, kParamAmpSustain, 0},
  {4, kParamAmpRelease, 120},  {4, kParamTapeSat, 40},

  // Family 02 / 003 Wide Arp
  {5, kParamOsc1Shape, 66},    {5, kParamOsc2Shape, 66},   {5, kParamOsc2Pitch, 7},
  {5, kParamOsc2Level, 70},    {5, kParamUnisonVoices, 4}, {5, kParamUnisonDetune, 40},
  {5, kParamFilterCutoff, 7000}, {5, kParamAmpAttack, 1},  {5, kParamAmpDecay, 90},
  {5, kParamAmpSustain, 0},    {5, kParamAmpRelease, 140},

  // Family 03 / 001 Soft EP
  {6, kParamOsc1Shape, 25},    {6, kParamOsc2Shape, 33},   {6, kParamOsc2Pitch, 12},
  {6, kParamOsc2Level, 45},    {6, kParamFmAmount, 18},    {6, kParamFilterCutoff, 4200},
  {6, kParamFilterReso, 12},   {6, kParamAmpAttack, 6},    {6, kParamAmpDecay, 1400},
  {6, kParamAmpSustain, 45},   {6, kParamAmpRelease, 800}, {6, kParamTapeMix, 35},

  // Family 03 / 002 Dry Clav
  {7, kParamOsc1Shape, 78},    {7, kParamOsc1PW, 30},      {7, kParamOsc2Shape, 90},
  {7, kParamOsc2Pitch, 12},    {7, kParamOsc2Level, 55},   {7, kParamFilterCutoff, 3000},
  {7, kParamFilterReso, 28},   {7, kParamFilterEnv, 60},   {7, kParamAmpAttack, 1},
  {7, kParamAmpDecay, 260},    {7, kParamAmpSustain, 0},   {7, kParamAmpRelease, 160},
  {7, kParamTapeSat, 30},      {7, kParamTapeMix, 20},

  // Family 03 / 003 Warm Organ
  {8, kParamOsc1Shape, 33},    {8, kParamOsc1Level, 90},   {8, kParamOsc2Shape, 33},
  {8, kParamOsc2Pitch, 12},    {8, kParamOsc2Level, 90},   {8, kParamSubLevel, 40},
  {8, kParamFilterCutoff, 8000}, {8, kParamFilterReso, 6}, {8, kParamAmpAttack, 3},
  {8, kParamAmpDecay, 60},     {8, kParamAmpSustain, 100}, {8, kParamAmpRelease, 90},

  // Family 04 / 001 Motion Pad
  {9, kParamOsc1Shape, 12},    {9, kParamOsc2Shape, 60},   {9, kParamOsc2Pitch, 7},
  {9, kParamUnisonVoices, 5},  {9, kParamUnisonDetune, 55}, {9, kParamFilterCutoff, 2600},
  {9, kParamFilterReso, 20},   {9, kParamFilterEnv, 65},   {9, kParamAmpAttack, 900},
  {9, kParamAmpDecay, 3000},   {9, kParamAmpSustain, 70},  {9, kParamAmpRelease, 3500},
  {9, kParamFiltAttack, 1200}, {9, kParamFiltDecay, 3000}, {9, kParamFiltSustain, 60},
  {9, kParamFiltRelease, 3000}, {9, kParamLfoRate, 0.25}, {9, kParamLfoDest, 1},
  {9, kParamLfoAmount, 40},     {9, kParamIns2Algo, 2},    {9, kParamIns2A, 24},
  {9, kParamIns2B, 58},         {9, kParamIns2C, 82},      {9, kParamIns2Mix, 35},
  {9, kParamMstAlgo, 3},        {9, kParamMstA, 72},       {9, kParamMstB, 28},
  {9, kParamMstC, 50},          {9, kParamMstMix, 55},

  // Family 04 / 002 Slow Drone
  {10, kParamOsc1Shape, 5},        {10, kParamOsc2Shape, 50},     {10, kParamOsc2Pitch, -12},
  {10, kParamSubLevel, 60},        {10, kParamNoiseLevel, 12},    {10, kParamFilterCutoff, 1200},
  {10, kParamFilterReso, 35},      {10, kParamFilterEnv, 20},     {10, kParamAmpAttack, 2000},
  {10, kParamAmpDecay, 4000},      {10, kParamAmpSustain, 95},    {10, kParamAmpRelease, 4000},

  // Family 04 / 003 Air Choir
  {11, kParamOsc1Shape, 28},       {11, kParamOsc2Shape, 45},     {11, kParamOsc2Pitch, 12},
  {11, kParamUnisonVoices, 6},     {11, kParamUnisonDetune, 30},  {11, kParamFilterCutoff, 5500},
  {11, kParamFilterReso, 14},      {11, kParamAmpAttack, 700},    {11, kParamAmpDecay, 2000},
  {11, kParamAmpSustain, 85},      {11, kParamAmpRelease, 2600},  {11, kParamTapeHiss, 18},
  {11, kParamTapeMix, 40},

  // Family 05 / 001 Hard Kit (pitch envelope does the drum-style drop)
  {12, kParamEngine, 1},           {12, kParamOsc1Shape, 100},    {12, kParamOsc1PW, 12},
  {12, kParamOsc1Level, 100},      {12, kParamOsc2Shape, 100},    {12, kParamOsc2Pitch, -12},
  {12, kParamOsc2Level, 85},       {12, kParamNoiseLevel, 45},    {12, kParamFilterCutoff, 12000},
  {12, kParamFilterReso, 10},      {12, kParamFilterEnv, 0},      {12, kParamAmpAttack, 0.5},
  {12, kParamAmpDecay, 160},       {12, kParamAmpSustain, 0},     {12, kParamAmpRelease, 120},
  {12, kParamPitchAmount, -18},    {12, kParamPitchAttack, 0.5},  {12, kParamPitchDecay, 90},
  {12, kParamPitchSustain, 0},     {12, kParamPitchRelease, 90},  {12, kParamTapeSat, 45},
  {12, kParamIns1Algo, 1},         {12, kParamIns1A, 62},         {12, kParamIns1B, 68},
  {12, kParamIns1C, 54},           {12, kParamIns1Mix, 42},       {12, kParamMstAlgo, 1},
  {12, kParamMstA, 38},            {12, kParamMstB, 65},          {12, kParamMstC, 32},
  {12, kParamMstMix, 70},          {12, kParamOutLevel, -1.5},

  // Family 05 / 002 Dry Kit
  {13, kParamOsc1Shape, 92},       {13, kParamOsc1PW, 40},        {13, kParamOsc2Shape, 88},
  {13, kParamOsc2Level, 60},       {13, kParamSubLevel, 35},      {13, kParamNoiseLevel, 30},
  {13, kParamFilterCutoff, 7000},  {13, kParamFilterReso, 15},    {13, kParamFilterEnv, 40},
  {13, kParamAmpAttack, 0.5},      {13, kParamAmpDecay, 110},     {13, kParamAmpSustain, 0},
  {13, kParamAmpRelease, 90},      {13, kParamPitchAmount, -12},  {13, kParamPitchDecay, 70},
  {13, kParamTapeSat, 15},         {13, kParamTapeMix, 15},

  // Family 05 / 003 Tape Kit
  {14, kParamOsc1Shape, 100},      {14, kParamOsc1PW, 50},        {14, kParamOsc2Shape, 55},
  {14, kParamOsc2Pitch, -24},      {14, kParamOsc2Level, 70},     {14, kParamNoiseLevel, 60},
  {14, kParamFilterCutoff, 5000},  {14, kParamFilterReso, 20},    {14, kParamFilterEnv, 60},
  {14, kParamAmpAttack, 0.5},      {14, kParamAmpDecay, 240},     {14, kParamAmpSustain, 0},
  {14, kParamAmpRelease, 260},     {14, kParamPitchAmount, -24},  {14, kParamPitchDecay, 140},
  {14, kParamTapeSat, 60},         {14, kParamTapeBump, 45},      {14, kParamTapeWow, 25},
  {14, kParamTapeFlutter, 30},     {14, kParamTapeMix, 55},       {14, kParamFilterDrive, 45},
};

const keith::PresetSpec kPresetSpecs[kNumPresets] = {
  {"Family 01 / 001 Glass Keys", 1, "Triton", "keys"},
  {"Family 01 / 002 Dry Bass", 1, "Triton", "bass"},
  {"Family 01 / 003 Bell Pad", 1, "Triton", "pad"},
  {"Family 02 / 001 Neon Pluck", 2, "Triton Extreme", "pluck"},
  {"Family 02 / 002 Thin Stab", 2, "Triton Extreme", "stab"},
  {"Family 02 / 003 Wide Arp", 2, "Triton Extreme", "arp"},
  {"Family 03 / 001 Soft EP", 3, "Korg M1", "electric piano"},
  {"Family 03 / 002 Dry Clav", 3, "Korg M1", "clav"},
  {"Family 03 / 003 Warm Organ", 3, "Korg M1", "organ"},
  {"Family 04 / 001 Motion Pad", 4, "Korg Wavestation", "motion pad"},
  {"Family 04 / 002 Slow Drone", 4, "Korg Wavestation", "drone"},
  {"Family 04 / 003 Air Choir", 4, "Korg Wavestation", "choir"},
  {"Family 05 / 001 Hard Kit", 5, "Korg TR", "kit"},
  {"Family 05 / 002 Dry Kit", 5, "Korg TR", "kit"},
  {"Family 05 / 003 Tape Kit", 5, "Korg TR", "kit"},
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

    // Factory chunks use the same custom-state layout as host save/restore.
    IByteChunk chunk;
    Keith::SerializeDrumKit(chunk, keith::drums::DefaultKitParams());
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
static_assert(kParamDrumSlotSelect == 75 && kParamDrumKitLevel == 84 && kNumParams == 85,
              "U11 appends 75..84");

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
  GetParam(kParamLfoDest)->InitEnum("Synth LFO Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level"});
  GetParam(kParamLfoAmount)->InitDouble("Synth LFO Amount", 0., -100., 100., 0.1, "%");
  GetParam(kParamModWheelDest)->InitEnum("Synth Mod Dest", 0, {"Off", "Cutoff", "Pitch", "Shape", "Level"});
  GetParam(kParamModWheelAmount)->InitDouble("Synth Mod Amount", 0., -100., 100., 0.1, "%");
  GetParam(kParamVelToAmp)->InitDouble("Synth Vel To Amp", 0., 0., 100., 0.1, "%");
  GetParam(kParamVelToFilter)->InitDouble("Synth Vel To Filter", 0., 0., 100., 0.1, "%");
  GetParam(kParamMonoLegato)->InitEnum("Synth Mono Legato", 0, {"Retrig", "Legato"});

  // U10. Off algorithms make every new default inert for the existing 15 presets.
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

  // Baked factory presets (15, grouped Family 01-05, neutral names only).
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
  p.modWheelDest = static_cast<keith::synth::ModDest>(GetParam(kParamModWheelDest)->Int());
  p.modWheelAmount = static_cast<float>(GetParam(kParamModWheelAmount)->Value() / 100.);
  p.velToAmp = static_cast<float>(GetParam(kParamVelToAmp)->Value() / 100.);
  p.velToFilter = static_cast<float>(GetParam(kParamVelToFilter)->Value() / 100.);
  p.monoLegato = GetParam(kParamMonoLegato)->Int() != 0;

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
