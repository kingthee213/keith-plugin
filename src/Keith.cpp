#include "Keith.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

#include <initializer_list>

#include "editor/EditorialLayout.h"

namespace
{
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
  {9, kParamLfoAmount, 40},

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

    // Same layout the host serialises: one double per parameter, in parameter order.
    IByteChunk chunk;
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
static_assert(kParamOscSync == 43 && kParamMonoLegato == 55 && kNumParams == 56,
              "U9 appends 43..55");

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
  IGraphics* pGraphics = GetUI();

  if (!pGraphics)
    return;

  const int current = GetCurrentPresetIdx();
  if (current != mLastPreset)
  {
    mLastPreset = current;
    pGraphics->SetAllControlsDirty();
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
#endif
