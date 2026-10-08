#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

// Original Keith tape/analog degradation block.
// Algorithm ideas seeded from Airwindows MIT modules (Tape, ToTape7, Flutter, Flutter2, Noise,
// TapeDust) — reimplemented here; no Airwindows source is included.

namespace keith {
namespace fx {

namespace detail {
constexpr double kPi = 3.14159265358979323846;
}

template<typename T>
class TapeAnalog
{
public:
  struct Params
  {
    T saturation = 0.25f;  // 0..1
    T headBump = 0.20f;
    T wow = 0.10f;
    T flutter = 0.10f;
    T hiss = 0.05f;
    T printThrough = 0.0f;
    T mix = 0.40f;
    bool bypass = false;
  };

  void Reset(double sampleRate, int /*blockSize*/)
  {
    mSampleRate = sampleRate > 0. ? sampleRate : 44100.;
    mWowPhase = 0.;
    mFlutterPhase = 0.;
    mHissSeed = 0x12345678u;
    mBumpZ1L = mBumpZ2L = mBumpZ1R = mBumpZ2R = static_cast<T>(0);
    mHissLpL = mHissLpR = static_cast<T>(0);
    mPrintWrite = 0;
    std::memset(mPrintBuffer, 0, sizeof(mPrintBuffer));
    std::memset(mDelayBuffer, 0, sizeof(mDelayBuffer));
    mDelayWrite = 0;
  }

  void SetParams(const Params& p)
  {
    mParams = p;
  }

  void ProcessBlock(T** outputs, int nChannels, int nFrames)
  {
    if (!outputs || nChannels <= 0 || nFrames <= 0)
      return;

    const bool bypass = mParams.bypass || mParams.mix <= 0.0001f;
    const T mix = static_cast<T>(mParams.mix);
    const int channels = std::min(nChannels, 2);

    for (int i = 0; i < nFrames; ++i)
    {
      T dry[2] = {outputs[0][i], channels > 1 ? outputs[1][i] : outputs[0][i]};
      T wet[2] = {dry[0], dry[1]};

      if (!bypass)
        ProcessFrame(dry, wet, channels);

      for (int ch = 0; ch < channels; ++ch)
        outputs[ch][i] = dry[ch] + (wet[ch] - dry[ch]) * mix;
    }
  }

private:
  static T Clamp(T x, T lo, T hi)
  {
    return std::min(hi, std::max(lo, x));
  }

  // Spiral-inspired soft saturation (seed: Airwindows Tape drive curve).
  T Saturate(T x, T amount) const
  {
    if (amount <= 0.0001f)
      return x;

    const T drive = 1.f + amount * 8.f;
    const T biased = x * drive;
    const T absB = std::fabs(biased);
    const T shaped = biased / (1.f + absB + absB * absB * 0.5f);
    return shaped / std::sqrt(drive);
  }

  // Head bump: resonant low shelf (seed: Airwindows Tape / ToTape7 head bump).
  T HeadBump(T x, int ch, T amount)
  {
    if (amount <= 0.0001f)
      return x;

    const T freq = static_cast<T>(80.);
    const T q = static_cast<T>(0.7 + amount * 1.5);
    const T w = static_cast<T>(2. * detail::kPi * freq / mSampleRate);
    const T alpha = std::sin(w) / (2.f * q);
    const T cosw = std::cos(w);
    const T a0 = 1.f + alpha;
    const T b0 = (1.f - cosw) * 0.5f / a0;
    const T b1 = (1.f - cosw) / a0;
    const T b2 = b0;
    const T a1 = (-2.f * cosw) / a0;
    const T a2 = (1.f - alpha) / a0;
    const T gain = 1.f + amount * 2.5f;

    T& z1 = (ch == 0) ? mBumpZ1L : mBumpZ1R;
    T& z2 = (ch == 0) ? mBumpZ2L : mBumpZ2R;
    const T y = b0 * x + b1 * z1 + b2 * z2 - a1 * z1 - a2 * z2;
    z2 = z1;
    z1 = x;
    return x + (y - x) * gain * amount;
  }

  void AdvanceModulationPhases()
  {
    const double wowHz = 0.35 + mParams.wow * 1.2;
    const double flutterHz = 6.0 + mParams.flutter * 9.0;

    mWowPhase += wowHz / mSampleRate;
    mFlutterPhase += flutterHz / mSampleRate;
    if (mWowPhase >= 1.)
      mWowPhase -= 1.;
    if (mFlutterPhase >= 1.)
      mFlutterPhase -= 1.;
  }

  T PitchModulationDelay(T x, int ch)
  {
    const T wowDepth = mParams.wow * static_cast<T>(0.003);
    const T flutterDepth = mParams.flutter * static_cast<T>(0.0015);

    const T wow = std::sin(static_cast<T>(mWowPhase * 2. * detail::kPi));
    const T flutter = std::sin(static_cast<T>(mFlutterPhase * 2. * detail::kPi + ch * 0.7));
    const T mod = wow * wowDepth + flutter * flutterDepth;

    const int baseDelay = static_cast<int>(mSampleRate * 0.02); // 20 ms base
    const int modSamples = static_cast<int>(mod * mSampleRate);
    const int delaySamples = std::max(1, baseDelay + modSamples);

    mDelayBuffer[ch][mDelayWrite] = x;
    int readPos = mDelayWrite - delaySamples;
    if (readPos < 0)
      readPos += kDelaySize;
    const T out = mDelayBuffer[ch][readPos];
    return out;
  }

  T AddHiss(T x, int ch)
  {
    if (mParams.hiss <= 0.0001f)
      return x;

    mHissSeed = mHissSeed * 1664525u + 1013904223u + static_cast<uint32_t>(ch * 17);
    const T white = static_cast<T>((mHissSeed >> 8) & 0xFFFF) / static_cast<T>(32768.) - 1.f;

    T& lp = (ch == 0) ? mHissLpL : mHissLpR;
    const T cutoff = static_cast<T>(0.02 + mParams.hiss * 0.15);
    lp = lp + cutoff * (white - lp);

    const T level = mParams.hiss * static_cast<T>(0.003) * (static_cast<T>(1.) + std::fabs(x));
    return x + lp * level;
  }

  T AddPrintThrough(T x, int ch)
  {
    if (mParams.printThrough <= 0.0001f)
      return x;

    const int delaySamples = static_cast<int>(mSampleRate * 1.1);
    const int readPos = (mPrintWrite - delaySamples + kPrintSize) % kPrintSize;
    const T ghost = mPrintBuffer[ch][readPos] * mParams.printThrough * static_cast<T>(0.35);
    mPrintBuffer[ch][mPrintWrite] = x;
    return x + ghost;
  }

  void ProcessFrame(const T dry[], T wet[], int nChannels)
  {
    if (mParams.wow > 0.0001f || mParams.flutter > 0.0001f)
      AdvanceModulationPhases();

    for (int ch = 0; ch < nChannels; ++ch)
    {
      T y = dry[ch];

      if (mParams.wow > 0.0001f || mParams.flutter > 0.0001f)
        y = PitchModulationDelay(y, ch);

      y = HeadBump(y, ch, mParams.headBump);
      y = Saturate(y, mParams.saturation);
      y = AddHiss(y, ch);
      wet[ch] = AddPrintThrough(y, ch);
    }

    mDelayWrite = (mDelayWrite + 1) % kDelaySize;
    mPrintWrite = (mPrintWrite + 1) % kPrintSize;
  }

  static constexpr int kDelaySize = 8192;
  static constexpr int kPrintSize = 48000; // ~1 s at 48 kHz

  Params mParams {};
  double mSampleRate = 44100.;
  double mWowPhase = 0.;
  double mFlutterPhase = 0.;
  uint32_t mHissSeed = 0x12345678u;

  T mBumpZ1L = 0., mBumpZ2L = 0., mBumpZ1R = 0., mBumpZ2R = 0.;
  T mHissLpL = 0., mHissLpR = 0.;

  T mDelayBuffer[2][kDelaySize] {};
  int mDelayWrite = 0;

  T mPrintBuffer[2][kPrintSize] {};
  int mPrintWrite = 0;
};

} // namespace fx
} // namespace keith
