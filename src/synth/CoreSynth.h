#pragma once

#include "WavetableOsc.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

// Subtractive / hybrid voice core for Keith.
// Hybrid path: TPT SVF multimode filter (unchanged).
// Analog path: OB-Xf-derived 4-pole ladder (Filter.h apply4Pole), sub square osc,
// pitch drift (OscillatorBlock.h oscPitchNoise + Voice.h slop), HF rolloff (Voice.h brightness).

namespace keith {
namespace synth {

namespace detail {
constexpr double kPi = 3.14159265358979323846;
constexpr double kTwoPi = 6.28318530717958647692;
constexpr float kDenorm = 1.0e-20f;
}

enum class EngineMode : int
{
  Hybrid = 0,
  Analog = 1
};

enum class FilterType : int
{
  LP24 = 0,
  LP12 = 1,
  BP = 2,
  HP = 3
};

enum class PolyMode : int
{
  Poly = 0,
  Mono = 1
};

// U9: one real modulation source set and one named destination set. Every destination is
// applied to its own DSP point only; Off is inert.
enum class LfoShape : int
{
  Sine = 0,
  Tri = 1,
  Saw = 2,
  Square = 3,
  SampleHold = 4
};

enum class ModDest : int
{
  Off = 0,
  Cutoff = 1,
  Pitch = 2,
  Shape = 3,
  Level = 4,
  Fm = 5   // U13: drive the FM oscillator depth
};

enum class ModSource : int
{
  Off = 0,
  Lfo1 = 1,
  Lfo2 = 2,
  ModWheel = 3,
  Velocity = 4,
  Key = 5
};

constexpr int kNumModSlots = 4;

struct ModRoute
{
  ModSource source = ModSource::Off;
  ModDest destination = ModDest::Off;
  float amount = 0.f;
};

struct ModSignals
{
  float lfo1 = 0.f;
  float lfo2 = 0.f;
  float modWheel = 0.f;
  float velocity = 0.f;
  float key = 0.f;
};

/** Live performance state that is not a stored parameter: the mod wheel arrives from CC1
 *  through the framework (VoiceAllocator forwards unhandled controllers to SynthVoice::SetControl). */
struct RuntimeMod
{
  float modWheel = 0.f; // 0..1
};

/** One destination of a modulation source, in octaves of cutoff / semitones of pitch / morph
 *  fraction / linear level. Returns nothing; accumulates into the caller's values. */
inline void ApplyModRoute(ModDest dest, float amount, float& cutoffOct, float& pitchSemi,
                          float& shapeMod, float& levelMod, float* fmMod = nullptr)
{
  switch (dest)
  {
    case ModDest::Cutoff: cutoffOct += amount * 4.f; break; // +/- 4 octaves at full amount
    case ModDest::Pitch: pitchSemi += amount * 12.f; break; // +/- 1 octave at full amount
    case ModDest::Shape: shapeMod += amount * 0.5f; break; // half a morph stage
    case ModDest::Level: levelMod += amount; break; // tremolo
    case ModDest::Fm: if (fmMod) *fmMod += amount; break; // drive the FM depth
    case ModDest::Off:
    default: break;
  }
}

inline float ModSourceValue(ModSource source, const ModSignals& signals)
{
  switch (source)
  {
    case ModSource::Lfo1: return signals.lfo1;
    case ModSource::Lfo2: return signals.lfo2;
    case ModSource::ModWheel: return signals.modWheel;
    case ModSource::Velocity: return signals.velocity;
    case ModSource::Key: return signals.key;
    case ModSource::Off:
    default: return 0.f;
  }
}

/** Sum the four matrix slots through the existing destination router. The matrix contribution is
 *  limited to one full-scale route per destination before it is added to the fixed routings. */
inline bool ApplyModMatrix(const ModRoute (&routes)[kNumModSlots], const ModSignals& signals,
                           float& cutoffOct, float& pitchSemi, float& shapeMod,
                           float& levelMod, float& fmMod)
{
  float matrixCutoffOct = 0.f;
  float matrixPitchSemi = 0.f;
  float matrixShape = 0.f;
  float matrixLevel = 0.f;
  float matrixFm = 0.f;
  bool active = false;

  for (const ModRoute& route : routes)
  {
    if (route.source == ModSource::Off || route.destination == ModDest::Off || route.amount == 0.f)
      continue;

    const float source = std::clamp(ModSourceValue(route.source, signals), -1.f, 1.f);
    if (source == 0.f)
      continue;

    ApplyModRoute(route.destination, source * std::clamp(route.amount, -1.f, 1.f),
                  matrixCutoffOct, matrixPitchSemi, matrixShape, matrixLevel, &matrixFm);
    active = true;
  }

  if (!active)
    return false;

  cutoffOct += std::clamp(matrixCutoffOct, -4.f, 4.f);
  pitchSemi += std::clamp(matrixPitchSemi, -12.f, 12.f);
  shapeMod += std::clamp(matrixShape, -0.5f, 0.5f);
  levelMod += std::clamp(matrixLevel, -1.f, 1.f);
  fmMod += std::clamp(matrixFm, -1.f, 1.f);
  return true;
}

struct Params
{
  EngineMode engine = EngineMode::Hybrid;
  PolyMode polyMode = PolyMode::Poly;
  float portaMs = 0.f;
  int unisonVoices = 1;
  float unisonDetune = 0.25f; // 0..1

  float osc1Shape = 0.f;   // 0..1 morph
  float osc1Pitch = 0.f;   // semitones
  float osc1Level = 1.f;   // 0..1
  float osc1PW = 0.5f;     // 0.05..0.95

  float osc2Shape = 0.f;
  float osc2Pitch = 0.f;
  float osc2Level = 0.8f;
  float osc2PW = 0.5f;

  // --- Third oscillator. Off by default so no pre-existing patch changes. ---
  // `osc3Source` picks the built-in wave morph or the wavetable bank; `osc3Morph` walks the
  // wavetable frames when the bank is the source.
  bool osc3Enabled = false;
  float osc3Shape = 0.f;
  float osc3Pitch = 0.f;
  float osc3Level = 0.f;
  float osc3PW = 0.5f;
  int osc3Source = 0;       // 0 = built-in shapes, 1 = wavetable bank
  float osc3Morph = 0.f;    // 0..1 across the frames

  float subLevel = 0.f;
  float noiseLevel = 0.f;
  float fmAmount = 0.f;

  FilterType filterType = FilterType::LP24;
  float filterCutoffHz = 8000.f;
  float filterReso = 0.10f; // 0..1
  float filterEnvAmt = 0.40f; // -1..1

  float ampAttackMs = 5.f;
  float ampDecayMs = 200.f;
  float ampSustain = 0.70f;
  float ampReleaseMs = 300.f;

  float pitchAttackMs = 10.f;
  float pitchDecayMs = 250.f;
  float pitchSustain = 0.f;
  float pitchReleaseMs = 300.f;
  float pitchAmount = 0.f; // semitones, -24..24

  float filtAttackMs = 10.f;
  float filtDecayMs = 250.f;
  float filtSustain = 0.40f;
  float filtReleaseMs = 300.f;

  float masterLevel = 1.f; // linear 0..~2

  // --- U9 appends. Every default is inert, so pre-U9 presets are unchanged. ---
  bool oscSync = false;     // hard sync: osc2 wrap restarts osc1
  bool oscRing = false;     // ring modulation added to the mixer
  float filterKeyTrack = 0.f; // octaves of cutoff per octave from MIDI 60. 1 = 1:1
  float filterKeyCurve = 0.f; // same unit times octaves squared. 0 keeps the linear track
  float filterDrive = 0.f;    // 0..1: pre-filter saturation

  float lfoRateHz = 1.f;      // 0.01..30
  LfoShape lfoShape = LfoShape::Sine;
  ModDest lfoDest = ModDest::Off;
  float lfoAmount = 0.f;      // -1..1

  // --- U13 append: a second, independent LFO. Default Off, so pre-U13 presets are unchanged. ---
  float lfo2RateHz = 0.3f;    // 0.01..30
  LfoShape lfo2Shape = LfoShape::Sine;
  ModDest lfo2Dest = ModDest::Off;
  float lfo2Amount = 0.f;     // -1..1

  ModDest modWheelDest = ModDest::Off;
  float modWheelAmount = 0.f; // -1..1

  float velToAmp = 0.f;    // 0..1: extra velocity depth on top of the envelope's own response
  float velToFilter = 0.f; // 0..1: velocity offsets cutoff, +/-2 octaves at full amount

  bool monoLegato = false; // mono only: a new key while another is held glides without re-attack

  // --- U15 append: four additive source -> destination -> amount routes. Defaults are inert. ---
  ModRoute modMatrix[kNumModSlots] {};

  // U17 append. Every default is inert, so earlier presets stay bit-identical.
  // Key amounts are octaves of change per octave of pitch away from MIDI 60.
  // Curve amounts are the same unit, times octaves-squared, so a decay can peak mid-keyboard.
  float noiseKey = 0.f;
  float ampAttackKey = 0.f;
  float ampAttackCurve = 0.f;
  float ampDecayKey = 0.f;
  float ampDecayCurve = 0.f;
  float stereoDelayMs = 0.f;
  float stereoDelayKey = 0.f;
  float stereoSpread = 0.f; // -1..1. Negative inverts the delayed right channel.
  float directLevel = 0.f;  // post-filter copy of the oscillators, before noise
};

class Envelope
{
public:
  enum class Stage
  {
    Idle = 0,
    Attack,
    Decay,
    Sustain,
    Release
  };

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
  }

  void SetTimes(float attackMs, float decayMs, float releaseMs)
  {
    mAttackInc = 1.f / std::max(1.f, attackMs * 0.001f * static_cast<float>(mSampleRate));
    const float decaySamples = std::max(1.f, decayMs * 0.001f * static_cast<float>(mSampleRate));
    const float releaseSamples = std::max(1.f, releaseMs * 0.001f * static_cast<float>(mSampleRate));
    mDecayCoeff = std::exp(-6.907755f / decaySamples);   // ~-60 dB
    mReleaseCoeff = std::exp(-6.907755f / releaseSamples);
  }

  void SetSustain(float s)
  {
    mSustain = std::clamp(s, 0.f, 1.f);
  }

  void NoteOn(float level)
  {
    mLevel = std::clamp(level, 0.f, 1.f);
    mStage = Stage::Attack;
    // Soft start from current value to avoid clicks on steal/retrigger.
  }

  void NoteOff()
  {
    if (mStage == Stage::Idle)
      return;
    mStage = Stage::Release;
  }

  void HardReset()
  {
    mStage = Stage::Idle;
    mValue = 0.f;
    mLevel = 0.f;
  }

  bool Busy() const
  {
    return mStage != Stage::Idle;
  }

  float Process()
  {
    switch (mStage)
    {
      case Stage::Attack:
        mValue += mAttackInc;
        if (mValue >= 1.f)
        {
          mValue = 1.f;
          mStage = Stage::Decay;
        }
        break;
      case Stage::Decay:
        mValue = mSustain + (mValue - mSustain) * mDecayCoeff;
        if (std::fabs(mValue - mSustain) < 1.0e-4f)
        {
          mValue = mSustain;
          mStage = Stage::Sustain;
        }
        break;
      case Stage::Sustain:
        mValue = mSustain;
        break;
      case Stage::Release:
        mValue *= mReleaseCoeff;
        if (mValue < 1.0e-5f)
        {
          mValue = 0.f;
          mStage = Stage::Idle;
        }
        break;
      case Stage::Idle:
      default:
        mValue = 0.f;
        break;
    }
    return mValue * mLevel;
  }

  float Value() const { return mValue * mLevel; }

private:
  double mSampleRate = 44100.;
  float mValue = 0.f;
  float mLevel = 0.f;
  float mSustain = 0.7f;
  float mAttackInc = 0.01f;
  float mDecayCoeff = 0.999f;
  float mReleaseCoeff = 0.999f;
  Stage mStage = Stage::Idle;
};

class Oscillator
{
public:
  void Reset(float phase = 0.f)
  {
    mPhase = phase - std::floor(phase);
    mPrev = 0.f;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
  }

  // shape 0..1: sine -> triangle -> saw -> pulse; pw 0..1 for pulse width.
  float Process(float freqHz, float shape, float pulseWidth)
  {
    freqHz = std::clamp(freqHz, 1.f, static_cast<float>(mSampleRate * 0.45));
    const float incr = freqHz / static_cast<float>(mSampleRate);
    mPhase += incr;
    mWrapped = false;
    if (mPhase >= 1.f)
    {
      mPhase -= std::floor(mPhase);
      mWrapped = true;
    }

    const float pw = std::clamp(pulseWidth, 0.05f, 0.95f);
    const float s = std::clamp(shape, 0.f, 1.f);

    const float sine = std::sin(static_cast<float>(detail::kTwoPi) * mPhase);
    const float tri = 1.f - 4.f * std::fabs(mPhase - 0.5f); // -1..1
    float saw = 2.f * mPhase - 1.f;
    saw -= PolyBlep(mPhase, incr);

    float pulse = mPhase < pw ? 1.f : -1.f;
    pulse += PolyBlep(mPhase, incr);
    float t2 = mPhase - pw;
    if (t2 < 0.f)
      t2 += 1.f;
    pulse -= PolyBlep(t2, incr);

    float out;
    if (s < 0.333333f)
    {
      const float t = s / 0.333333f;
      out = sine * (1.f - t) + tri * t;
    }
    else if (s < 0.666666f)
    {
      const float t = (s - 0.333333f) / 0.333333f;
      out = tri * (1.f - t) + saw * t;
    }
    else
    {
      const float t = (s - 0.666666f) / 0.333334f;
      out = saw * (1.f - t) + pulse * t;
    }

    // One-pole softener in Analog mode is applied by caller via mild LP if needed.
    mPrev = out;
    return out;
  }

  /** True when the last Process() crossed the phase wrap; the hard-sync master signal. */
  bool Wrapped() const { return mWrapped; }

  /** Restart the cycle at a known phase; used by hard sync and by note-on. */
  void SyncReset() { mPhase = 0.f; }

private:
  static float PolyBlep(float t, float dt)
  {
    if (t < dt)
    {
      t /= dt;
      return t + t - t * t - 1.f;
    }
    if (t > 1.f - dt)
    {
      t = (t - 1.f) / dt;
      return t * t + t + t + 1.f;
    }
    return 0.f;
  }

  double mSampleRate = 44100.;
  float mPhase = 0.f;
  float mPrev = 0.f;
  bool mWrapped = false;
};

class MultiFilter
{
public:
  void Reset()
  {
    mZ1 = mZ2 = mZ3 = mZ4 = 0.f;
    mLp = mBp = mHp = 0.f;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
  }

  float Process(float in, float cutoffHz, float reso01, FilterType type)
  {
    cutoffHz = std::clamp(cutoffHz, 20.f, static_cast<float>(mSampleRate * 0.45));
    const float g = std::tan(static_cast<float>(detail::kPi) * cutoffHz / static_cast<float>(mSampleRate));
    const float q = 0.5f + std::clamp(reso01, 0.f, 1.f) * 9.5f; // 0.5..10
    const float k = 1.f / q;

    // TPT SVF (Zavalishin) — one 12 dB section.
    const float v1 = (in - (2.f * k + g) * mZ1 - mZ2) / (1.f + 2.f * k * g + g * g);
    const float v2 = g * v1 + mZ1;
    const float v3 = g * v2 + mZ2;
    mZ1 = g * v1 + v2;
    mZ2 = g * v2 + v3;

    mHp = v1;
    mBp = v2;
    mLp = v3;

    float out12 = mLp;
    switch (type)
    {
      case FilterType::HP: out12 = mHp; break;
      case FilterType::BP: out12 = mBp; break;
      case FilterType::LP12:
      case FilterType::LP24:
      default: out12 = mLp; break;
    }

    if (type != FilterType::LP24)
      return Sanitize(out12);

    // Second cascaded section for ~24 dB/oct low-pass.
    const float u1 = (out12 - (2.f * k + g) * mZ3 - mZ4) / (1.f + 2.f * k * g + g * g);
    const float u2 = g * u1 + mZ3;
    const float u3 = g * u2 + mZ4;
    mZ3 = g * u1 + u2;
    mZ4 = g * u2 + u3;
    return Sanitize(u3);
  }

private:
  static float Sanitize(float x)
  {
    if (!std::isfinite(x))
      return 0.f;
    return x + detail::kDenorm;
  }

  double mSampleRate = 44100.;
  float mZ1 = 0.f, mZ2 = 0.f, mZ3 = 0.f, mZ4 = 0.f;
  float mLp = 0.f, mBp = 0.f, mHp = 0.f;
};
/** 4-pole transistor-ladder low-pass adapted from OB-Xf Filter::apply4Pole (Filter.h). */
class LadderFilter
{
public:
  void Reset()
  {
    mPole1 = mPole2 = mPole3 = mPole4 = 0.f;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
    const float rcRate = std::sqrt(44000.f / static_cast<float>(mSampleRate));
    mResCorrection = (970.f / 44000.f) * rcRate;
    mResCorrectionInv = 1.f / mResCorrection;
  }

  float Process(float sample, float cutoffHz, float reso01)
  {
    cutoffHz = std::clamp(cutoffHz, 20.f, static_cast<float>(mSampleRate * 0.45));
    const float res = std::clamp(reso01, 0.f, 1.f);
    const float res4Pole = 3.5f * res;

    float g = std::tan(cutoffHz / static_cast<float>(mSampleRate) *
                         static_cast<float>(detail::kPi));
    const float lpc = g / (1.f + g);
    const float y0 = ResolveFeedback4Pole(sample, g, lpc, res4Pole);

    double v = (y0 - mPole1) * lpc;
    double resPole = v + mPole1;
    mPole1 = resPole + v;
    mPole1 = std::atan(static_cast<float>(mPole1) * mResCorrection) * mResCorrectionInv;

    const float goveroneplusg = g / (1.f + g);
    const float y1 = static_cast<float>(resPole);
    const float y4 = TptPole(mPole2, y1, goveroneplusg, mPole3, mPole4);

    return y4 * (1.f + res4Pole * 0.45f);
  }

private:
  static float TptPole(float& state, float input, float gOver1pG, float& state2, float& state3)
  {
    const float y2 = TptProcessScaled(state, input, gOver1pG);
    const float y3 = TptProcessScaled(state2, y2, gOver1pG);
    return TptProcessScaled(state3, y3, gOver1pG);
  }

  static float TptProcessScaled(float& state, float input, float gOver1pG)
  {
    const double v = (input - state) * gOver1pG;
    const double res = v + state;
    state = static_cast<float>(res + v);
    return static_cast<float>(res);
  }

  float ResolveFeedback4Pole(float sample, float g, float lpc, float res4Pole) const
  {
    const float ml = 1.f / (1.f + g);
    const float S = (lpc * (lpc * (lpc * mPole1 + mPole2) + mPole3) + mPole4) * ml;
    const float G = lpc * lpc * lpc * lpc;
    return (sample - res4Pole * S) / (1.f + res4Pole * G);
  }

  double mSampleRate = 44100.;
  float mPole1 = 0.f;
  float mPole2 = 0.f;
  float mPole3 = 0.f;
  float mPole4 = 0.f;
  float mResCorrection = 1.f;
  float mResCorrectionInv = 1.f;
};

/** Square sub one octave down — distinct band under the fundamental, not a level bump. */
class SubSquareOsc
{
public:
  void Reset(float phase = 0.f)
  {
    mPhase = phase - std::floor(phase);
    mLp = 0.f;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
    mLpCoef = std::exp(-2.f * static_cast<float>(detail::kPi) * 140.f /
                       static_cast<float>(mSampleRate));
  }

  float Process(float freqHz)
  {
    freqHz = std::clamp(freqHz, 20.f, static_cast<float>(mSampleRate * 0.45));
    mPhase += freqHz / static_cast<float>(mSampleRate);
    if (mPhase >= 1.f)
      mPhase -= std::floor(mPhase);
    const float square = mPhase < 0.5f ? 1.f : -1.f;
    mLp += mLpCoef * (square - mLp);
    return mLp;
  }

private:
  double mSampleRate = 44100.;
  float mPhase = 0.f;
  float mLp = 0.f;
  float mLpCoef = 0.99f;
};


inline float MidiNoteToHz(float note)
{
  return 440.f * std::pow(2.f, (note - 69.f) / 12.f);
}

/** One tempo-independent low-frequency oscillator, per voice, phase-restarted on note-on.
 *  Output is -1..1 for every shape; Sample & Hold draws a new value once per cycle. */
class Lfo
{
public:
  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
  }

  void Reset()
  {
    mPhase = 0.f;
    mRandom = 0.f;
    mSeed = 0x2545F491u;
  }

  float Process(float rateHz, LfoShape shape)
  {
    const float rate = std::clamp(rateHz, 0.01f, 30.f);
    mPhase += rate / static_cast<float>(mSampleRate);
    if (mPhase >= 1.f)
    {
      mPhase -= std::floor(mPhase);
      mSeed = mSeed * 1664525u + 1013904223u;
      mRandom = (static_cast<float>(mSeed >> 8) / 8388608.f) - 1.f;
    }

    switch (shape)
    {
      case LfoShape::Tri: return 1.f - 4.f * std::fabs(mPhase - 0.5f);
      case LfoShape::Saw: return 2.f * mPhase - 1.f;
      case LfoShape::Square: return mPhase < 0.5f ? 1.f : -1.f;
      case LfoShape::SampleHold: return mRandom;
      case LfoShape::Sine:
      default: return std::sin(static_cast<float>(detail::kTwoPi) * mPhase);
    }
  }

private:
  double mSampleRate = 44100.;
  float mPhase = 0.f;
  float mRandom = 0.f;
  uint32_t mSeed = 0x2545F491u;
};

inline float DbToLinear(float db)
{
  if (db <= -60.f)
    return 0.f;
  return std::pow(10.f, db / 20.f);
}

/** Keys currently held, oldest at the front. Cleared on note-off, all-notes-off and reset.
 *  OfflineEngine and KeithDSP both decide legato from this count. */
struct HeldKeyTracker
{
  bool down[128] {};
  int order[128] {};
  float velocity[128] {};
  int count = 0;

  void Clear()
  {
    std::memset(down, 0, sizeof(down));
    std::memset(velocity, 0, sizeof(velocity));
    count = 0;
  }

  int Others(int note) const
  {
    if (note < 0 || note > 127)
      return count;
    return count - (down[note] ? 1 : 0);
  }

  void Add(int note, float vel)
  {
    if (note < 0 || note > 127)
      return;
    velocity[note] = vel;
    if (down[note])
      return;
    down[note] = true;
    order[count++] = note;
  }

  void Remove(int note)
  {
    if (note < 0 || note > 127 || !down[note])
      return;
    down[note] = false;
    int write = 0;
    for (int read = 0; read < count; ++read)
      if (order[read] != note)
        order[write++] = order[read];
    count = write;
  }

  int Newest() const { return count > 0 ? order[count - 1] : -1; }
};

/** Mono legato: a new key while another is held, and the voice is already open, glides.
 *  Retrigger (the default) always re-attacks. Poly ignores the flag. */
inline bool LegatoAttack(const Params& p, int otherKeysHeld, bool voiceBusy)
{
  return p.polyMode == PolyMode::Mono && p.monoLegato && otherKeysHeld > 0 && voiceBusy;
}

class Voice
{
public:
  /**
   * The wavetable bank.
   *
   * Exposed so the editor can DRAW the frames. The bank is built once per sample-rate change and
   * never mutated afterwards, so a pointer to it is stable for as long as it is asked for -- which
   * is what lets the page hold one and rebuild its picture only when it changes.
   */
  const WavetableBank* Wavetable() const { return &mWavetableBank; }

  /**
   * The built-in wavetable bank, available without loading anything from disk.
   *
   * Eight frames walking from a pure sine to a bright, slightly hollow harmonic stack. The
   * morph parameter sweeps across them. This exists so osc 3 in wavetable mode makes a sound
   * with no file loading -- the derived tables from the analysis pipeline replace this bank
   * later, but a sampler that is silent until a file arrives cannot be tested at all.
   */
  static WavetableBank MakeStarterBank()
  {
    WavetableBank bank;
    std::vector<WavetableFrame> frames;

    for (int f = 0; f < 8; ++f)
    {
      WavetableFrame frame;
      const float t = static_cast<float>(f) / 7.f;
      // Frame 0 is a sine. Each later frame adds more upper harmonics and pulls the odd ones
      // back, which opens a hollow, squarer tone -- the classic sine-to-square sweep.
      for (int h = 0; h < 48; ++h)
      {
        const int n = h + 1;
        const float rolloff = 1.f / static_cast<float>(n);
        const float odd = (n % 2 == 1) ? 1.f : (1.f - t);
        const float top = std::min(1.f, 48.f / static_cast<float>(n));
        frame.harmonic[h] = t * rolloff * odd * top;
      }
      // Keep the fundamental at unity so the frame is never quiet.
      frame.harmonic[0] = 1.f;
      frames.push_back(frame);
    }

    bank.SetFrames(frames);
    return bank;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
    mSub.SetSampleRate(mSampleRate);
    mAnalogSub.SetSampleRate(mSampleRate);
    for (auto& o : mUniOsc3)
      o.SetSampleRate(mSampleRate);
    // The starter bank is built once per sample-rate change. Building it is a few hundred
    // sine evaluations per frame, which is a message-thread cost, not an audio-thread one --
    // SetSampleRate is called from OnReset.
    mWavetableBank = MakeStarterBank();
    for (auto& o : mUniOsc3Wt)
    {
      o.SetSampleRate(mSampleRate);
      o.SetBank(&mWavetableBank);
    }
    mFilter.SetSampleRate(mSampleRate);
    mLadder.SetSampleRate(mSampleRate);
    mAnalogBrightHz = std::min(4200.f, static_cast<float>(mSampleRate * 0.5f) - 10.f);
    mAnalogBrightCoef = std::tan(mAnalogBrightHz * static_cast<float>(detail::kPi) /
                                 static_cast<float>(mSampleRate));
    mAmpEnv.SetSampleRate(mSampleRate);
    mPitchEnv.SetSampleRate(mSampleRate);
    mFiltEnv.SetSampleRate(mSampleRate);
    mLfo.SetSampleRate(mSampleRate);
    mLfo2.SetSampleRate(mSampleRate);
    const int delaySamples = std::clamp(static_cast<int>(mSampleRate * 0.042) + 8, 64, 8192);
    mStereoDelay.assign(static_cast<size_t>(delaySamples), 0.f);
    mStereoWrite = 0;
    for (auto& o : mUniOsc1)
      o.SetSampleRate(mSampleRate);
    for (auto& o : mUniOsc2)
      o.SetSampleRate(mSampleRate);
  }

  void Reset()
  {
    mSub.Reset(0.11f);
    mAnalogSub.Reset(0.11f);
    mFilter.Reset();
    mLadder.Reset();
    mVoiceDetuneSemi = 0.f;
    mPitchDriftSemi = 0.f;
    mPitchDriftTarget = 0.f;
    mAnalogBrightState = 0.f;
    mAnalogFloor = 0.f;
    mAmpEnv.HardReset();
    mPitchEnv.HardReset();
    mFiltEnv.HardReset();
    mNoise = 0x12345678u;
    mActive = false;
    mNote = 60.f;
    mVelocity = 0.f;
    mAnalogSoft = 0.f;
    mLfo.Reset();
    mLfo2.Reset();
    ClearStereoDelay();
    for (int i = 0; i < kMaxUnison; ++i)
    {
      mUniOsc1[i].Reset(static_cast<float>(i) * 0.07f);
      mUniOsc2[i].Reset(static_cast<float>(i) * 0.13f + 0.2f);
      mUniOsc3[i].Reset(static_cast<float>(i) * 0.19f + 0.41f);
      mUniOsc3Wt[i].Reset(static_cast<float>(i) * 0.19f + 0.41f);
    }
  }

  bool Busy() const { return mAmpEnv.Busy(); }

  void NoteOn(float note, float velocity, const Params& p)
  {
    mNote = note;
    mVelocity = std::clamp(velocity, 0.f, 1.f);
    mActive = true;
    ApplyEnvTimes(p);
    mAmpEnv.NoteOn(mVelocity);
    mPitchEnv.NoteOn(1.f);
    mFiltEnv.NoteOn(1.f);
    mSub.Reset(0.11f);
    mAnalogSub.Reset(0.11f);
    mFilter.Reset();
    mLadder.Reset();
    mVoiceDetuneSemi = VoiceDetuneForIndex(mNote);
    mPitchDriftSemi = 0.f;
    mPitchDriftTarget = 0.f;
    mAnalogBrightState = 0.f;
    mLfo.Reset();
    mLfo2.Reset();
    ClearStereoDelay();
    for (int i = 0; i < kMaxUnison; ++i)
    {
      mUniOsc1[i].Reset(static_cast<float>(i) * 0.07f);
      mUniOsc2[i].Reset(static_cast<float>(i) * 0.13f + 0.2f);
      mUniOsc3[i].Reset(static_cast<float>(i) * 0.19f + 0.41f);
      mUniOsc3Wt[i].Reset(static_cast<float>(i) * 0.19f + 0.41f);
    }
  }

  /** Mono legato: take the new note without restarting any envelope. */
  void GlideTo(float note)
  {
    mNote = note;
    mActive = true;
  }

  void NoteOff()
  {
    mAmpEnv.NoteOff();
    mPitchEnv.NoteOff();
    mFiltEnv.NoteOff();
  }

  // Portamento / pitch-bend: MidiSynth updates this each block via the pitch ramp.
  void SetBaseNote(float note)
  {
    mNote = note;
  }

  void ApplyEnvTimes(const Params& p)
  {
    float attack = p.ampAttackMs;
    float decay = p.ampDecayMs;
    if (p.ampAttackKey != 0.f || p.ampAttackCurve != 0.f ||
        p.ampDecayKey != 0.f || p.ampDecayCurve != 0.f)
    {
      const float oct = (mNote - 60.f) / 12.f;
      const float oct2 = oct * oct;
      if (p.ampAttackKey != 0.f || p.ampAttackCurve != 0.f)
        attack *= std::pow(2.f, p.ampAttackKey * oct + p.ampAttackCurve * oct2);
      if (p.ampDecayKey != 0.f || p.ampDecayCurve != 0.f)
        decay *= std::pow(2.f, p.ampDecayKey * oct + p.ampDecayCurve * oct2);
      attack = std::clamp(attack, 0.1f, 30000.f);
      decay = std::clamp(decay, 1.f, 80000.f);
    }
    mAmpEnv.SetTimes(attack, decay, p.ampReleaseMs);
    mAmpEnv.SetSustain(p.ampSustain);
    mPitchEnv.SetTimes(p.pitchAttackMs, p.pitchDecayMs, p.pitchReleaseMs);
    mPitchEnv.SetSustain(p.pitchSustain);
    mFiltEnv.SetTimes(p.filtAttackMs, p.filtDecayMs, p.filtReleaseMs);
    mFiltEnv.SetSustain(p.filtSustain);
  }

  float Process(const Params& p, const RuntimeMod& mod)
  {
    float left = 0.f;
    float right = 0.f;
    ProcessFrame(p, mod, left, right);
    return left;
  }

  void ProcessFrame(const Params& p, const RuntimeMod& mod, float& left, float& right)
  {
    if (p.engine == EngineMode::Analog)
    {
      ProcessFrameAnalog(p, mod, left, right);
      return;
    }

    if (!mActive && !mAmpEnv.Busy())
    {
      left = 0.f;
      right = 0.f;
      return;
    }

    const float amp = mAmpEnv.Process();
    if (!mAmpEnv.Busy())
    {
      mActive = false;
      left = 0.f;
      right = 0.f;
      return;
    }

    const float pitchEnv = mPitchEnv.Process();
    const float filtEnv = mFiltEnv.Process();

    // --- U9 modulation: one LFO (per-voice, retriggered on note-on) and the mod wheel (CC1).
    // U13 adds an independent second LFO. Each route feeds exactly one DSP point; an Off
    // destination contributes nothing. ---
    const float lfoValue = mLfo.Process(p.lfoRateHz, p.lfoShape);
    const float lfo2Value = mLfo2.Process(p.lfo2RateHz, p.lfo2Shape);
    const float lfo = lfoValue * p.lfoAmount;   // -1..1 * amount
    const float lfo2 = lfo2Value * p.lfo2Amount; // -1..1 * amount
    const float mw = mod.modWheel * p.modWheelAmount;                        // 0..1 * amount

    float cutoffOct = 0.f, pitchMod = 0.f, shapeMod = 0.f, levelMod = 0.f, fmMod = 0.f;
    ApplyModRoute(p.lfoDest, lfo, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    ApplyModRoute(p.lfo2Dest, lfo2, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    ApplyModRoute(p.modWheelDest, mw, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    const ModSignals matrixSignals {
      lfoValue,
      lfo2Value,
      mod.modWheel,
      mVelocity,
      std::clamp((mNote - 60.f) / 60.f, -1.f, 1.f)
    };
    ApplyModMatrix(p.modMatrix, matrixSignals, cutoffOct, pitchMod, shapeMod, levelMod, fmMod);

    const float pitchSemis = p.pitchAmount * pitchEnv + pitchMod;
    const float baseNote = mNote + pitchSemis;

    const float fmScale = 1.25f;
    const float shapeBias = 0.05f;

    const int unison = std::clamp(p.unisonVoices, 1, kMaxUnison);
    const float detuneCents = p.unisonDetune * 35.f; // up to ~35 cents spread

    float mix = 0.f;
    const float norm = 1.f / static_cast<float>(unison);

    for (int u = 0; u < unison; ++u)
    {
      float detuneSemi = 0.f;
      if (unison > 1)
      {
        const float t = (static_cast<float>(u) / static_cast<float>(unison - 1)) * 2.f - 1.f;
        detuneSemi = (t * detuneCents) / 100.f;
      }

      const float note1 = baseNote + p.osc1Pitch + detuneSemi;
      const float note2 = baseNote + p.osc2Pitch - detuneSemi * 0.85f;
      float f1 = MidiNoteToHz(note1);
      const float f2 = MidiNoteToHz(note2);

      const float osc1Shape = std::clamp(p.osc1Shape + shapeBias + shapeMod, 0.f, 1.f);
      const float osc2Shape = std::clamp(p.osc2Shape + shapeBias + shapeMod, 0.f, 1.f);

      const float osc2 = mUniOsc2[u].Process(f2, osc2Shape, p.osc2PW);

      // Hard sync: osc2 is the master; its wrap restarts the osc1 cycle.
      if (p.oscSync && mUniOsc2[u].Wrapped())
        mUniOsc1[u].SyncReset();

      // Simple phase-modulation FM: osc2 -> osc1 frequency.
      const float fmEff = p.fmAmount + fmMod; // LFO/mod-wheel drive the FM depth
      f1 *= (1.f + osc2 * fmEff * fmScale * 4.f);
      const float osc1 = mUniOsc1[u].Process(f1, osc1Shape, p.osc1PW);

      float voice = osc1 * p.osc1Level + osc2 * p.osc2Level;
      if (p.oscRing)
        voice += osc1 * osc2; // ring modulation: a distinct algorithm from the FM path

      // Third oscillator. Detuned between the other two so a three-oscillator stack spreads
      // rather than doubling one pitch, and taken OUTSIDE the FM/sync path -- it is a layer,
      // not a modulator, so it must not alter how osc 1 and 2 interact.
      if (p.osc3Enabled && p.osc3Level > 0.f)
      {
        const float note3 = baseNote + p.osc3Pitch + detuneSemi * 0.5f;
        const float f3 = MidiNoteToHz(note3);
        const float osc3Shape = std::clamp(p.osc3Shape + shapeBias + shapeMod, 0.f, 1.f);
        const float osc3 = p.osc3Source == 1
                             ? mUniOsc3Wt[u].Process(f3, p.osc3Morph)
                             : mUniOsc3[u].Process(f3, osc3Shape, p.osc3PW);
        voice += osc3 * p.osc3Level;
      }

      mix += voice * norm;
    }

    const float subFreq = MidiNoteToHz(baseNote - 12.f);
    const float sub = mSub.Process(subFreq, 0.05f, 0.5f); // near-sine
    mix += sub * p.subLevel;
    const float tonal = mix;

    // Noise. Key amount 0 keeps the old multiply.
    mNoise = mNoise * 1664525u + 1013904223u;
    const float noise = (static_cast<float>(mNoise >> 8) / 8388608.f) - 1.f;
    float noiseGain = p.noiseLevel;
    if (p.noiseKey != 0.f)
    {
      const float oct = (mNote - 60.f) / 12.f;
      noiseGain *= std::pow(2.f, std::clamp(p.noiseKey * oct, -6.f, 6.f));
      if (noiseGain > 8.f)
        noiseGain = 8.f;
    }
    mix += noise * noiseGain;

    // Soft clip mix headroom. Drive adds pre-filter saturation and backs the level off again,
    // so drive 0 is bit-identical to the pre-U9 path.
    const float driveGain = 1.f + 7.f * p.filterDrive;
    mix = std::tanh(mix * 0.85f * driveGain) * (1.f / (1.f + 1.6f * p.filterDrive));

    // Cutoff: key tracking, velocity offset, envelope and modulation all land on the same cut.
    // Curve 0 uses the original multiply, so existing patches stay bit-identical.
    const float keyOct = p.filterKeyCurve == 0.f
      ? p.filterKeyTrack * (mNote - 60.f) / 12.f
      : p.filterKeyTrack * (mNote - 60.f) / 12.f
        + p.filterKeyCurve * ((mNote - 60.f) / 12.f) * ((mNote - 60.f) / 12.f);
    const float velOct = p.velToFilter * (mVelocity - 0.5f) * 4.f; // +/- 2 octaves at 100 %
    float cutoff = p.filterCutoffHz * std::pow(2.f, keyOct + velOct + cutoffOct);
    // Env amount: bipolar, ±4 octaves around cutoff at full amount.
    const float envOct = p.filterEnvAmt * filtEnv * 4.f;
    cutoff *= std::pow(2.f, envOct);
    cutoff = std::clamp(cutoff, 20.f, static_cast<float>(mSampleRate * 0.45));

    float filtered = mFilter.Process(mix, cutoff, p.filterReso, p.filterType);
    if (p.directLevel > 0.f)
      filtered += tonal * p.directLevel;

    const float velAmp = 1.f + p.velToAmp * (mVelocity - 0.5f) * 2.f;
    const float tremolo = std::max(0.f, 1.f + levelMod);
    const float mono = filtered * amp * velAmp * tremolo * p.masterLevel;
    left = mono;
    if (p.stereoSpread == 0.f)
    {
      right = mono;
      return;
    }

    const float spread = std::clamp(p.stereoSpread, -1.f, 1.f);
    float delayMs = std::max(0.f, p.stereoDelayMs);
    if (p.stereoDelayKey != 0.f)
    {
      const float oct = (mNote - 60.f) / 12.f;
      delayMs *= std::pow(2.f, std::clamp(p.stereoDelayKey * oct, -4.f, 4.f));
    }
    delayMs = std::min(delayMs, 40.f);
    const float delayed = WriteStereoDelay(mono, delayMs * 0.001f * static_cast<float>(mSampleRate));
    const float mag = std::fabs(spread);
    const float side = spread >= 0.f ? delayed : -delayed;
    right = mono * (1.f - mag) + side * mag;
  }


  void ProcessFrameAnalog(const Params& p, const RuntimeMod& mod, float& left, float& right)
  {
    AdvancePitchDrift();

    if (!mActive && !mAmpEnv.Busy())
    {
      const float floor = AnalogNoiseSample() * AnalogFloorGain();
      left = floor;
      right = floor;
      return;
    }

    const float amp = mAmpEnv.Process();
    if (!mAmpEnv.Busy())
    {
      mActive = false;
      const float floor = AnalogNoiseSample() * AnalogFloorGain();
      left = floor;
      right = floor;
      return;
    }

    const float pitchEnv = mPitchEnv.Process();
    const float filtEnv = mFiltEnv.Process();

    const float lfoValue = mLfo.Process(p.lfoRateHz, p.lfoShape);
    const float lfo2Value = mLfo2.Process(p.lfo2RateHz, p.lfo2Shape);
    const float lfo = lfoValue * p.lfoAmount;
    const float lfo2 = lfo2Value * p.lfo2Amount;
    const float mw = mod.modWheel * p.modWheelAmount;

    float cutoffOct = 0.f, pitchMod = 0.f, shapeMod = 0.f, levelMod = 0.f, fmMod = 0.f;
    ApplyModRoute(p.lfoDest, lfo, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    ApplyModRoute(p.lfo2Dest, lfo2, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    ApplyModRoute(p.modWheelDest, mw, cutoffOct, pitchMod, shapeMod, levelMod, &fmMod);
    const ModSignals matrixSignals {
      lfoValue,
      lfo2Value,
      mod.modWheel,
      mVelocity,
      std::clamp((mNote - 60.f) / 60.f, -1.f, 1.f)
    };
    ApplyModMatrix(p.modMatrix, matrixSignals, cutoffOct, pitchMod, shapeMod, levelMod, fmMod);

    const float pitchSemis = p.pitchAmount * pitchEnv + pitchMod;
    const float driftSemi = mVoiceDetuneSemi + mPitchDriftSemi;
    const float baseNote = mNote + pitchSemis + driftSemi;

    const int unison = std::clamp(p.unisonVoices, 1, kMaxUnison);
    const float detuneCents = p.unisonDetune * 28.f;

    float mix = 0.f;
    const float norm = 1.f / static_cast<float>(unison);

    for (int u = 0; u < unison; ++u)
    {
      float detuneSemi = 0.f;
      if (unison > 1)
      {
        const float t = (static_cast<float>(u) / static_cast<float>(unison - 1)) * 2.f - 1.f;
        detuneSemi = (t * detuneCents) / 100.f;
      }

      const float note1 = baseNote + p.osc1Pitch + detuneSemi;
      const float note2 = baseNote + p.osc2Pitch - detuneSemi * 0.85f;
      float f1 = MidiNoteToHz(note1);
      const float f2 = MidiNoteToHz(note2);

      const float osc1Shape = std::clamp(p.osc1Shape - 0.14f + shapeMod, 0.f, 1.f);
      const float osc2Shape = std::clamp(p.osc2Shape - 0.14f + shapeMod, 0.f, 1.f);

      const float osc2 = mUniOsc2[u].Process(f2, osc2Shape, p.osc2PW);

      if (p.oscSync && mUniOsc2[u].Wrapped())
        mUniOsc1[u].SyncReset();

      const float fmEff = p.fmAmount + fmMod;
      f1 *= (1.f + osc2 * fmEff * 0.85f * 4.f);
      const float osc1 = mUniOsc1[u].Process(f1, osc1Shape, p.osc1PW);

      float voice = osc1 * p.osc1Level + osc2 * p.osc2Level;
      if (p.oscRing)
        voice += osc1 * osc2;

      if (p.osc3Enabled && p.osc3Level > 0.f)
      {
        const float note3 = baseNote + p.osc3Pitch + detuneSemi * 0.5f;
        const float f3 = MidiNoteToHz(note3);
        const float osc3Shape = std::clamp(p.osc3Shape - 0.14f + shapeMod, 0.f, 1.f);
        const float osc3 = p.osc3Source == 1
                             ? mUniOsc3Wt[u].Process(f3, p.osc3Morph)
                             : mUniOsc3[u].Process(f3, osc3Shape, p.osc3PW);
        voice += osc3 * p.osc3Level;
      }

      mix += voice * norm;
    }

    const float subFreq = MidiNoteToHz(baseNote - 12.f);
    const float sub = mAnalogSub.Process(subFreq);
    const float subGain = p.subLevel > 0.f ? p.subLevel : 0.35f;
    mix += sub * subGain * 1.55f;

    mNoise = mNoise * 1664525u + 1013904223u;
    const float noise = (static_cast<float>(mNoise >> 8) / 8388608.f) - 1.f;
    float noiseGain = p.noiseLevel + 0.018f;
    if (p.noiseKey != 0.f)
    {
      const float oct = (mNote - 60.f) / 12.f;
      noiseGain *= std::pow(2.f, std::clamp(p.noiseKey * oct, -6.f, 6.f));
      if (noiseGain > 8.f)
        noiseGain = 8.f;
    }
    mix += noise * noiseGain;

    const float driveGain = 1.f + 7.f * p.filterDrive;
    mix = std::tanh(mix * 0.9f * driveGain) * (1.f / (1.f + 1.6f * p.filterDrive));

    const float keyOct = p.filterKeyCurve == 0.f
      ? p.filterKeyTrack * (mNote - 60.f) / 12.f
      : p.filterKeyTrack * (mNote - 60.f) / 12.f
        + p.filterKeyCurve * ((mNote - 60.f) / 12.f) * ((mNote - 60.f) / 12.f);
    const float velOct = p.velToFilter * (mVelocity - 0.5f) * 4.f;
    float cutoff = p.filterCutoffHz * std::pow(2.f, keyOct + velOct + cutoffOct);
    const float envOct = p.filterEnvAmt * filtEnv * 4.f;
    cutoff *= std::pow(2.f, envOct);
    cutoff = std::clamp(cutoff, 20.f, static_cast<float>(mSampleRate * 0.45));

    float filtered = 0.f;
    if (p.filterType == FilterType::BP || p.filterType == FilterType::HP)
      filtered = mFilter.Process(mix, cutoff, p.filterReso, p.filterType);
    else
      filtered = mLadder.Process(mix, cutoff, p.filterReso);

    if (p.directLevel > 0.f)
      filtered += mix * p.directLevel;

    filtered = AnalogBrightness(filtered);
    filtered = AnalogBrightness(filtered);
    filtered = filtered * 0.93f + mAnalogSoft * 0.07f;
    mAnalogSoft = filtered;

    const float velAmp = 1.f + p.velToAmp * (mVelocity - 0.5f) * 2.f;
    const float tremolo = std::max(0.f, 1.f + levelMod);
    const float mono = filtered * amp * velAmp * tremolo * p.masterLevel;
    left = mono;
    if (p.stereoSpread == 0.f)
    {
      right = mono;
      return;
    }

    const float spread = std::clamp(p.stereoSpread, -1.f, 1.f);
    float delayMs = std::max(0.f, p.stereoDelayMs);
    if (p.stereoDelayKey != 0.f)
    {
      const float oct = (mNote - 60.f) / 12.f;
      delayMs *= std::pow(2.f, std::clamp(p.stereoDelayKey * oct, -4.f, 4.f));
    }
    delayMs = std::min(delayMs, 40.f);
    const float delayed = WriteStereoDelay(mono, delayMs * 0.001f * static_cast<float>(mSampleRate));
    const float mag = std::fabs(spread);
    const float side = spread >= 0.f ? delayed : -delayed;
    right = mono * (1.f - mag) + side * mag;
  }

  static float VoiceDetuneForIndex(float note)
  {
    const uint32_t seed = static_cast<uint32_t>(note * 17.31f) * 1664525u + 1013904223u;
    const float white = (static_cast<float>(seed >> 8) / 8388608.f) - 1.f;
    return white * 0.11f;
  }

  void AdvancePitchDrift()
  {
    mDriftNoise = mDriftNoise * 1664525u + 1013904223u;
    const float white = (static_cast<float>(mDriftNoise >> 8) / 8388608.f) - 1.f;
    mPitchDriftTarget += white * (1.2f / static_cast<float>(mSampleRate));
    mPitchDriftTarget = std::clamp(mPitchDriftTarget, -0.18f, 0.18f);
    const float smooth = 1.f - std::exp(-0.35f / static_cast<float>(mSampleRate));
    mPitchDriftSemi += (mPitchDriftTarget - mPitchDriftSemi) * smooth;
  }

  float AnalogNoiseSample()
  {
    mFloorNoise = mFloorNoise * 1664525u + 1013904223u;
    return (static_cast<float>(mFloorNoise >> 8) / 8388608.f) - 1.f;
  }

  static float AnalogFloorGain() { return 0.00045f; }

  float AnalogBrightness(float sample)
  {
    const double v = (sample - mAnalogBrightState) * mAnalogBrightCoef / (1.f + mAnalogBrightCoef);
    const double res = v + mAnalogBrightState;
    mAnalogBrightState = static_cast<float>(res + v);
    return static_cast<float>(res);
  }


private:
  static constexpr int kMaxUnison = 8;
  double mSampleRate = 44100.;
  Oscillator mSub;
  SubSquareOsc mAnalogSub;
  Oscillator mUniOsc1[kMaxUnison];
  Oscillator mUniOsc2[kMaxUnison];
  /** Third oscillator, built-in wave morph. Only rendered when osc3Enabled. */
  Oscillator mUniOsc3[kMaxUnison];
  /** Third oscillator reading the wavetable bank. */
  WavetableOscillator mUniOsc3Wt[kMaxUnison];
  /**
   * The wavetable bank osc 3 reads. Held by the Voice rather than passed in per sample so a
   * patch change does not have to thread a pointer through the render loop; the engine sets
   * it when the patch loads, which is a message-thread operation.
   */
  WavetableBank mWavetableBank;
  MultiFilter mFilter;
  LadderFilter mLadder;
  float mVoiceDetuneSemi = 0.f;
  float mPitchDriftSemi = 0.f;
  float mPitchDriftTarget = 0.f;
  uint32_t mDriftNoise = 0x9e3779b9u;
  uint32_t mFloorNoise = 0x85ebca6bu;
  float mAnalogBrightState = 0.f;
  float mAnalogBrightCoef = 0.f;
  float mAnalogBrightHz = 7200.f;
  float mAnalogFloor = 0.f;
  Envelope mAmpEnv;
  Envelope mPitchEnv;
  Envelope mFiltEnv;
  Lfo mLfo;
  Lfo mLfo2;
  void ClearStereoDelay()
  {
    std::fill(mStereoDelay.begin(), mStereoDelay.end(), 0.f);
    mStereoWrite = 0;
  }

  float WriteStereoDelay(float sample, float delaySamples)
  {
    if (mStereoDelay.size() < 4)
    {
      mStereoDelay.assign(64, 0.f);
      mStereoWrite = 0;
    }
    const int length = static_cast<int>(mStereoDelay.size());
    mStereoDelay[static_cast<size_t>(mStereoWrite)] = sample;
    const float limited = std::clamp(delaySamples, 0.f, static_cast<float>(length - 2));
    float read = static_cast<float>(mStereoWrite) - limited;
    if (read < 0.f)
      read += static_cast<float>(length);
    const int i0 = static_cast<int>(read) % length;
    const float frac = read - static_cast<float>(static_cast<int>(read));
    const int i1 = (i0 + 1) % length;
    const float out = mStereoDelay[static_cast<size_t>(i0)] +
                      (mStereoDelay[static_cast<size_t>(i1)] - mStereoDelay[static_cast<size_t>(i0)]) * frac;
    mStereoWrite = (mStereoWrite + 1) % length;
    return out;
  }

  uint32_t mNoise = 0x12345678u;
  float mNote = 60.f;
  float mVelocity = 0.f;
  float mAnalogSoft = 0.f;
  bool mActive = false;
  std::vector<float> mStereoDelay;
  int mStereoWrite = 0;
};

// Standalone poly engine for offline tests (no iPlug2 MidiSynth dependency).
class OfflineEngine
{
public:
  static constexpr int kMaxVoices = 16;

  void Reset(double sampleRate)
  {
    mSampleRate = sampleRate;
    mHeld.Clear();
    mSounding = false;
    mSoundingKey = -1;
    for (int i = 0; i < kMaxVoices; ++i)
    {
      mVoices[i].SetSampleRate(sampleRate);
      mVoices[i].Reset();
      mNoteForVoice[i] = -1;
    }
  }

  void SetParams(const Params& p) { mParams = p; }

  /** Mod wheel (CC1) feed; the offline mirror of the runtime value KeithDSP stores from CC1. */
  void SetModWheel(float value01) { mRuntime.modWheel = std::clamp(value01, 0.f, 1.f); }

  /** Test hook: release everything and clear held keys, as all-notes-off must. */
  void AllNotesOff()
  {
    for (int i = 0; i < kMaxVoices; ++i)
    {
      mVoices[i].NoteOff();
      mNoteForVoice[i] = -1;
    }
    mHeld.Clear();
    mSounding = false;
    mSoundingKey = -1;
  }

  void NoteOn(int note, float velocity)
  {
    if (note < 0 || note > 127)
      return;

    const int others = mHeld.Others(note);
    const bool busy = mSounding && mVoices[0].Busy();
    mHeld.Add(note, velocity);

    if (mParams.polyMode == PolyMode::Mono)
    {
      if (LegatoAttack(mParams, others, busy))
        mVoices[0].GlideTo(static_cast<float>(note));
      else
        mVoices[0].NoteOn(static_cast<float>(note), velocity, mParams);

      mSounding = true;
      mSoundingKey = note;
      mNoteForVoice[0] = note;
      for (int i = 1; i < kMaxVoices; ++i)
        mNoteForVoice[i] = -1;
      return;
    }

    int slot = -1;
    for (int i = 0; i < kMaxVoices; ++i)
    {
      if (mNoteForVoice[i] == note)
      {
        slot = i;
        break;
      }
    }
    if (slot < 0)
    {
      for (int i = 0; i < kMaxVoices; ++i)
      {
        if (!mVoices[i].Busy())
        {
          slot = i;
          break;
        }
      }
    }
    if (slot < 0)
      slot = 0; // steal oldest-ish

    mVoices[slot].NoteOn(static_cast<float>(note), velocity, mParams);
    mNoteForVoice[slot] = note;
    mSounding = true;
    mSoundingKey = note;
  }

  void NoteOff(int note)
  {
    if (note < 0 || note > 127 || !mHeld.down[note])
      return;

    const bool wasSounding = mSoundingKey == note;
    mHeld.Remove(note);

    if (mParams.polyMode == PolyMode::Mono)
    {
      if (mHeld.count > 0 && wasSounding)
      {
        const int next = mHeld.Newest();
        if (mParams.monoLegato && mVoices[0].Busy())
          mVoices[0].GlideTo(static_cast<float>(next));
        else
          mVoices[0].NoteOn(static_cast<float>(next), mHeld.velocity[next], mParams);
        mSoundingKey = next;
        mNoteForVoice[0] = next;
      }
      else if (mHeld.count == 0)
      {
        mVoices[0].NoteOff();
        mNoteForVoice[0] = -1;
        mSounding = false;
        mSoundingKey = -1;
      }
      return;
    }

    for (int i = 0; i < kMaxVoices; ++i)
    {
      if (mNoteForVoice[i] == note)
      {
        mVoices[i].NoteOff();
        mNoteForVoice[i] = -1;
      }
    }
    if (mHeld.count == 0)
    {
      mSounding = false;
      mSoundingKey = -1;
    }
  }

  float Process()
  {
    float sum = 0.f;
    for (int i = 0; i < kMaxVoices; ++i)
      sum += mVoices[i].Process(mParams, mRuntime);
    return sum;
  }

  void ProcessStereo(float& left, float& right)
  {
    left = 0.f;
    right = 0.f;
    for (int i = 0; i < kMaxVoices; ++i)
    {
      float voiceLeft = 0.f;
      float voiceRight = 0.f;
      mVoices[i].ProcessFrame(mParams, mRuntime, voiceLeft, voiceRight);
      left += voiceLeft;
      right += voiceRight;
    }
  }

  int ActiveVoiceCount() const
  {
    int count = 0;
    for (const Voice& voice : mVoices)
      count += voice.Busy() ? 1 : 0;
    return count;
  }

  Params& GetParams() { return mParams; }

private:
  Params mParams {};
  RuntimeMod mRuntime {};
  Voice mVoices[kMaxVoices];
  int mNoteForVoice[kMaxVoices] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1
  };
  HeldKeyTracker mHeld;
  bool mSounding = false;
  int mSoundingKey = -1;
  double mSampleRate = 44100.;
};

} // namespace synth
} // namespace keith
