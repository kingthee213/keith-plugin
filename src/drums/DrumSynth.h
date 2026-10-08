#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

// Original synthesized percussion for Keith. This engine uses oscillators, deterministic
// noise, envelopes, a one-pole filter and soft clipping. It contains no sample playback.

namespace keith {
namespace drums {

constexpr int kNumSlots = 16;
constexpr int kFirstMidiNote = 36;
constexpr int kLastMidiNote = 51;
constexpr float kSilenceDb = -60.f;

inline bool IsDrumNote(int note)
{
  return note >= kFirstMidiNote && note <= kLastMidiNote;
}

inline int SlotForMidiNote(int note)
{
  return IsDrumNote(note) ? note - kFirstMidiNote : -1;
}

enum class MidiDestination
{
  Synth,
  Drum
};

inline MidiDestination DestinationForMidiNote(int note)
{
  return IsDrumNote(note) ? MidiDestination::Drum : MidiDestination::Synth;
}

struct SlotParams
{
  float levelDb = 0.f;
  float pan = 0.f;
  float tuneSemitones = 0.f;
  float decay = 0.70f;
  float filterHz = 12000.f;
  float drive = 0.f;
  bool mute = false;
  bool solo = false;
};

struct KitParams
{
  SlotParams slot[kNumSlots];
  float levelDb = 0.f;
};

inline float DbToLinear(float db)
{
  if (db <= kSilenceDb)
    return 0.f;
  return std::pow(10.f, db / 20.f);
}

inline SlotParams DefaultSlotParams(int slot)
{
  static constexpr float kLevel[kNumSlots] = {
     0.f, -1.f, -3.f, -2.f, -4.f, -4.f, -5.f, -2.f,
    -2.f, -2.f, -5.f, -4.f, -3.f, -6.f, -5.f, -4.f
  };
  static constexpr float kPan[kNumSlots] = {
     0.f,  0.f,  0.10f, -0.10f,  0.25f,  0.30f, -0.20f, -0.18f,
     0.f,  0.18f, 0.20f, -0.20f, -0.25f,  0.25f, -0.30f,  0.30f
  };
  static constexpr float kDecay[kNumSlots] = {
    0.70f, 0.55f, 0.25f, 0.42f, 0.18f, 0.72f, 0.16f, 0.48f,
    0.44f, 0.40f, 0.78f, 0.88f, 0.46f, 0.36f, 0.24f, 0.52f
  };
  static constexpr float kFilter[kNumSlots] = {
    12000.f, 11000.f, 15000.f, 13000.f, 18000.f, 18000.f, 15000.f, 9000.f,
    10000.f, 11000.f, 17000.f, 18000.f, 12000.f, 16000.f, 14000.f, 13000.f
  };

  const int i = std::clamp(slot, 0, kNumSlots - 1);
  SlotParams params;
  params.levelDb = kLevel[i];
  params.pan = kPan[i];
  params.decay = kDecay[i];
  params.filterHz = kFilter[i];
  return params;
}

inline KitParams DefaultKitParams()
{
  KitParams params;
  for (int i = 0; i < kNumSlots; ++i)
    params.slot[i] = DefaultSlotParams(i);
  return params;
}

inline const char* SlotName(int slot)
{
  static constexpr const char* kNames[kNumSlots] = {
    "Kick", "Snare", "Snare Alt / Rim", "Clap",
    "Closed Hat", "Open Hat", "Pedal Hat", "Tom Lo",
    "Tom Mid", "Tom Hi", "Ride", "Crash",
    "Perc 1", "Perc 2", "Perc 3", "Perc 4"
  };
  return kNames[std::clamp(slot, 0, kNumSlots - 1)];
}

inline int RoundRobinCount(int slot)
{
  static constexpr int kCounts[kNumSlots] = {
    3, 4, 2, 3, 4, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2
  };
  return kCounts[std::clamp(slot, 0, kNumSlots - 1)];
}

namespace detail {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 6.28318530717958647692f;

struct VoiceSpec
{
  float baseHz;
  float secondRatio;
  float thirdRatio;
  float tone;
  float noise;
  float metal;
  float transient;
  float decaySeconds;
  float pitchDropSemitones;
  float noiseTone;
};

inline VoiceSpec SpecForSlot(int slot)
{
  static constexpr VoiceSpec kSpecs[kNumSlots] = {
    { 55.f,  2.00f,  3.00f, 1.00f, 0.03f, 0.00f, 0.18f, 0.48f, 36.f, 0.20f},
    {185.f,  1.47f,  2.31f, 0.28f, 0.85f, 0.06f, 0.48f, 0.32f,  5.f, 0.48f},
    {510.f,  1.63f,  2.41f, 0.58f, 0.16f, 0.28f, 0.75f, 0.11f,  8.f, 0.30f},
    {240.f,  1.71f,  2.53f, 0.06f, 0.94f, 0.10f, 0.52f, 0.24f,  0.f, 0.58f},
    {5800.f, 1.37f,  1.73f, 0.00f, 0.36f, 0.88f, 0.32f, 0.10f,  0.f, 0.86f},
    {5100.f, 1.41f,  1.79f, 0.00f, 0.42f, 0.82f, 0.22f, 0.62f,  0.f, 0.82f},
    {4300.f, 1.47f,  1.83f, 0.00f, 0.34f, 0.78f, 0.30f, 0.08f,  0.f, 0.72f},
    { 92.f,  1.50f,  2.03f, 0.95f, 0.04f, 0.03f, 0.18f, 0.45f, 16.f, 0.24f},
    {132.f,  1.50f,  2.07f, 0.94f, 0.04f, 0.04f, 0.20f, 0.38f, 14.f, 0.25f},
    {188.f,  1.50f,  2.11f, 0.92f, 0.05f, 0.05f, 0.22f, 0.32f, 12.f, 0.27f},
    {3100.f, 1.337f, 1.821f,0.02f, 0.24f, 0.96f, 0.18f, 0.92f,  0.f, 0.76f},
    {2600.f, 1.419f, 1.937f,0.01f, 0.40f, 0.98f, 0.45f, 1.15f,  0.f, 0.90f},
    {246.f,  1.52f,  2.04f, 0.88f, 0.12f, 0.08f, 0.28f, 0.29f,  9.f, 0.30f},
    {3600.f, 1.39f,  1.89f, 0.00f, 0.92f, 0.18f, 0.22f, 0.20f,  0.f, 0.88f},
    {820.f,  1.62f,  2.36f, 0.42f, 0.20f, 0.36f, 0.82f, 0.09f,  4.f, 0.38f},
    {540.f,  1.48f,  2.00f, 0.58f, 0.03f, 0.72f, 0.42f, 0.26f,  2.f, 0.24f}
  };
  return kSpecs[std::clamp(slot, 0, kNumSlots - 1)];
}

inline float WrapPhase(float phase)
{
  return phase - std::floor(phase);
}

inline float Sanitize(float sample)
{
  if (!std::isfinite(sample))
    return 0.f;
  return std::clamp(sample, -8.f, 8.f);
}

} // namespace detail

class Voice
{
public:
  void Reset(double sampleRate, int slot)
  {
    mSampleRate = sampleRate > 1. ? sampleRate : 44100.;
    mSlot = std::clamp(slot, 0, kNumSlots - 1);
    mActive = false;
    mAge = 0;
    mTailSamples = 1;
    mPhase1 = mPhase2 = mPhase3 = 0.f;
    mFilterState = 0.f;
    mNoiseState = 0.f;
    mLast = 0.f;
    mRetriggerFrom = 0.f;
    mVelocity = 0.f;
    mPitchJitter = 0.f;
    mDecayJitter = 1.f;
    mLayer = 0;
    mNoise = 0x9E3779B9u ^ static_cast<uint32_t>(mSlot * 0x45D9F3Bu);
  }

  void Trigger(float velocity, int roundRobin)
  {
    static constexpr float kPitchJitter[4] = {-0.14f, 0.09f, -0.05f, 0.16f};
    static constexpr float kDecayJitter[4] = {0.96f, 1.035f, 0.985f, 1.055f};

    const int rr = std::clamp(roundRobin, 0, 3);
    mVelocity = std::clamp(velocity, 0.f, 1.f);
    mLayer = mVelocity <= (42.f / 127.f) ? 0 : (mVelocity <= (95.f / 127.f) ? 1 : 2);
    mPitchJitter = kPitchJitter[rr];
    mDecayJitter = kDecayJitter[rr];
    mRetriggerFrom = mLast;
    mAge = 0;
    mActive = mVelocity > 0.f;
    mFilterState = 0.f;
    mNoiseState = 0.f;
    mPhase1 = detail::WrapPhase(0.07f * static_cast<float>(mSlot + rr));
    mPhase2 = detail::WrapPhase(0.13f * static_cast<float>(mSlot + 2 * rr) + 0.17f);
    mPhase3 = detail::WrapPhase(0.19f * static_cast<float>(mSlot + 3 * rr) + 0.31f);
    mNoise = 0xA511E9B3u ^ static_cast<uint32_t>((mSlot + 1) * 0x9E3779B9u) ^
             static_cast<uint32_t>((rr + 1) * 0x85EBCA6Bu);
  }

  float Process(const SlotParams& params)
  {
    if (!mActive)
      return 0.f;

    const detail::VoiceSpec spec = detail::SpecForSlot(mSlot);
    const float decayScale = 0.18f + 2.35f * std::clamp(params.decay, 0.f, 1.f);
    const float tailSeconds = std::max(0.008f, spec.decaySeconds * decayScale * mDecayJitter);
    mTailSamples = std::max(1, static_cast<int>(tailSeconds * static_cast<float>(mSampleRate)));

    if (mAge >= mTailSamples)
    {
      mActive = false;
      mLast = 0.f;
      return 0.f;
    }

    const float time = static_cast<float>(mAge) / static_cast<float>(mSampleRate);
    const float progress = static_cast<float>(mAge) / static_cast<float>(mTailSamples);
    const float envelope = std::exp(-6.907755f * progress);
    const float pitchSweep = spec.pitchDropSemitones * std::exp(-time / 0.032f);
    const float layerPitch = static_cast<float>(mLayer - 1) * 0.06f;
    const float semitones = params.tuneSemitones + mPitchJitter + layerPitch + pitchSweep;
    const float frequency = spec.baseHz * std::pow(2.f, semitones / 12.f);

    mPhase1 = detail::WrapPhase(mPhase1 + frequency / static_cast<float>(mSampleRate));
    mPhase2 = detail::WrapPhase(mPhase2 + frequency * spec.secondRatio /
                                static_cast<float>(mSampleRate));
    mPhase3 = detail::WrapPhase(mPhase3 + frequency * spec.thirdRatio /
                                static_cast<float>(mSampleRate));

    const float tone1 = std::sin(detail::kTwoPi * mPhase1);
    const float tone2 = std::sin(detail::kTwoPi * mPhase2);
    const float tone3 = std::sin(detail::kTwoPi * mPhase3);
    const float metal = tone1 * tone2 + 0.55f * tone2 * tone3;

    mNoise = mNoise * 1664525u + 1013904223u;
    const float white = static_cast<float>(mNoise >> 8) / 8388608.f - 1.f;
    mNoiseState += spec.noiseTone * (white - mNoiseState);
    const float highNoise = white - mNoiseState;
    const float brightness = 0.82f + 0.18f * static_cast<float>(mLayer);
    const float transient = std::exp(-time / 0.006f) * (white * 0.7f + tone3 * 0.3f);

    float raw = spec.tone * (tone1 + 0.18f * tone2) +
                spec.noise * (mNoiseState * (2.f - brightness) + highNoise * brightness) +
                spec.metal * metal +
                spec.transient * transient * (0.78f + 0.22f * static_cast<float>(mLayer));

    if (mSlot == 3)
    {
      const float clapPulse = 0.72f + 0.28f * std::max(0.f,
        std::sin(detail::kTwoPi * 47.f * time));
      raw *= clapPulse;
    }
    else if (mSlot == 13)
    {
      const float shakerPulse = 0.65f + 0.35f * std::max(0.f,
        std::sin(detail::kTwoPi * 31.f * time));
      raw *= shakerPulse;
    }

    const float cutoff = std::clamp(params.filterHz, 20.f,
                                    static_cast<float>(mSampleRate * 0.45));
    const float coefficient = 1.f - std::exp(-detail::kTwoPi * cutoff /
                                             static_cast<float>(mSampleRate));
    mFilterState += coefficient * (raw - mFilterState);
    float shaped = mFilterState;

    const float drive = std::clamp(params.drive, 0.f, 1.f);
    if (drive > 0.f)
    {
      const float driveGain = 1.f + drive * 13.f;
      shaped = std::tanh(shaped * driveGain) / (1.f + drive * 1.8f);
    }

    const float velocityGain = std::pow(mVelocity, 0.72f);
    float output = shaped * envelope * velocityGain * DbToLinear(params.levelDb) * 0.42f;

    const int attackSamples = std::max(1, static_cast<int>(mSampleRate * 0.0005));
    const float attack = std::min(1.f, static_cast<float>(mAge) /
                                        static_cast<float>(attackSamples));
    output = mRetriggerFrom * (1.f - attack) + output * attack;
    output = detail::Sanitize(output);

    ++mAge;
    mLast = output;
    return output;
  }

  bool Active() const { return mActive; }

private:
  double mSampleRate = 44100.;
  int mSlot = 0;
  int mAge = 0;
  int mTailSamples = 1;
  int mLayer = 0;
  uint32_t mNoise = 0x9E3779B9u;
  float mPhase1 = 0.f;
  float mPhase2 = 0.f;
  float mPhase3 = 0.f;
  float mFilterState = 0.f;
  float mNoiseState = 0.f;
  float mLast = 0.f;
  float mRetriggerFrom = 0.f;
  float mVelocity = 0.f;
  float mPitchJitter = 0.f;
  float mDecayJitter = 1.f;
  bool mActive = false;
};

class Engine
{
public:
  void Reset(double sampleRate)
  {
    mSampleRate = sampleRate > 1. ? sampleRate : 44100.;
    for (int i = 0; i < kNumSlots; ++i)
    {
      mVoice[i].Reset(mSampleRate, i);
      mRoundRobin[i] = 0;
    }
  }

  void ResetRoundRobin()
  {
    for (int& index : mRoundRobin)
      index = 0;
  }

  void AllNotesOff()
  {
    for (int i = 0; i < kNumSlots; ++i)
      mVoice[i].Reset(mSampleRate, i);
  }

  void SetParams(const KitParams& params) { mParams = params; }
  const KitParams& GetParams() const { return mParams; }

  void SetSlotParams(int slot, const SlotParams& params)
  {
    if (slot >= 0 && slot < kNumSlots)
      mParams.slot[slot] = params;
  }

  void SetKitLevel(float db) { mParams.levelDb = db; }
  bool Audible() const { return mParams.levelDb > kSilenceDb; }

  void Trigger(int slot, float velocity)
  {
    if (slot < 0 || slot >= kNumSlots || velocity <= 0.f)
      return;
    const int count = RoundRobinCount(slot);
    const int rr = mRoundRobin[slot] % count;
    mRoundRobin[slot] = (rr + 1) % count;
    mVoice[slot].Trigger(velocity, rr);
  }

  void ProcessFrame(float& left, float& right)
  {
    left = 0.f;
    right = 0.f;

    bool anySolo = false;
    for (const SlotParams& slot : mParams.slot)
      anySolo = anySolo || slot.solo;

    for (int i = 0; i < kNumSlots; ++i)
    {
      const SlotParams& params = mParams.slot[i];
      const float sample = mVoice[i].Process(params);
      const bool silent = params.mute || (anySolo && !params.solo) ||
                          params.levelDb <= kSilenceDb;
      if (silent)
        continue;

      const float pan = std::clamp(params.pan, -1.f, 1.f);
      const float angle = (pan + 1.f) * detail::kPi * 0.25f;
      left += sample * std::cos(angle);
      right += sample * std::sin(angle);
    }

    const float kitGain = DbToLinear(mParams.levelDb);
    if (kitGain == 0.f)
    {
      left = right = 0.f;
      return;
    }

    left = detail::Sanitize(std::tanh(left * kitGain * 0.75f));
    right = detail::Sanitize(std::tanh(right * kitGain * 0.75f));
  }

  int ActiveVoiceCount() const
  {
    int count = 0;
    for (const Voice& voice : mVoice)
      count += voice.Active() ? 1 : 0;
    return count;
  }

private:
  double mSampleRate = 44100.;
  KitParams mParams = DefaultKitParams();
  Voice mVoice[kNumSlots];
  int mRoundRobin[kNumSlots] {};
};

} // namespace drums
} // namespace keith
