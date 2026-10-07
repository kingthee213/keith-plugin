#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace keith {
namespace fx {

enum class InsertAlgorithm : int
{
  Off = 0,
  Drive = 1,
  Chorus = 2,
  Delay = 3,
  Crush = 4
};

enum class MasterAlgorithm : int
{
  Off = 0,
  Compressor = 1,
  Limiter = 2,
  Width = 3,
  Tilt = 4
};

namespace patch_detail {

constexpr double kPi = 3.14159265358979323846;

template<typename T>
inline T Clamp01(T value)
{
  return std::clamp(value, static_cast<T>(0), static_cast<T>(1));
}

template<typename T>
inline T DbToLinear(T db)
{
  return std::pow(static_cast<T>(10), db / static_cast<T>(20));
}

template<typename T>
inline T Sanitize(T value)
{
  if (!std::isfinite(value))
    return static_cast<T>(0);
  return std::clamp(value, static_cast<T>(-64), static_cast<T>(64));
}

template<typename T>
class SmoothValue
{
public:
  void Reset(double sampleRate, T value)
  {
    const double sr = sampleRate > 1. ? sampleRate : 44100.;
    mCoeff = static_cast<T>(std::exp(-1. / (0.005 * sr)));
    mCurrent = value;
    mTarget = value;
  }

  void SetTarget(T value) { mTarget = value; }

  T Next()
  {
    mCurrent = mTarget + mCoeff * (mCurrent - mTarget);
    if (std::fabs(mCurrent - mTarget) < static_cast<T>(1.0e-5))
      mCurrent = mTarget;
    return mCurrent;
  }

  T Current() const { return mCurrent; }
  T Target() const { return mTarget; }

private:
  T mCurrent = static_cast<T>(0);
  T mTarget = static_cast<T>(0);
  T mCoeff = static_cast<T>(0);
};

template<typename T>
inline T ReadDelay(const std::vector<T>& buffer, int writePosition, T delaySamples)
{
  if (buffer.empty())
    return static_cast<T>(0);

  const T maximum = static_cast<T>(buffer.size() - 2);
  const T delay = std::clamp(delaySamples, static_cast<T>(1), maximum);
  T read = static_cast<T>(writePosition) - delay;
  while (read < static_cast<T>(0))
    read += static_cast<T>(buffer.size());

  const int i0 = static_cast<int>(read) % static_cast<int>(buffer.size());
  const int i1 = (i0 + 1) % static_cast<int>(buffer.size());
  const T fraction = read - static_cast<T>(static_cast<int>(read));
  return buffer[static_cast<size_t>(i0)] +
         (buffer[static_cast<size_t>(i1)] - buffer[static_cast<size_t>(i0)]) * fraction;
}

inline float RandomBipolar(uint32_t& seed)
{
  seed = seed * 1664525u + 1013904223u;
  return static_cast<float>(seed >> 8) / 8388608.f - 1.f;
}

} // namespace patch_detail

template<typename T>
class InsertEffect
{
public:
  struct Params
  {
    InsertAlgorithm algorithm = InsertAlgorithm::Off;
    T a = static_cast<T>(0.5);
    T b = static_cast<T>(0.5);
    T c = static_cast<T>(0.5);
    T mix = static_cast<T>(1);
    bool bypass = false;
  };

  void Reset(double sampleRate, int /*blockSize*/)
  {
    mSampleRate = sampleRate > 1. ? sampleRate : 44100.;
    const size_t chorusSize = static_cast<size_t>(std::ceil(mSampleRate * 0.08)) + 4;
    const size_t delaySize = static_cast<size_t>(std::ceil(mSampleRate * 1.1)) + 4;
    mChorusL.assign(chorusSize, static_cast<T>(0));
    mChorusR.assign(chorusSize, static_cast<T>(0));
    mDelayL.assign(delaySize, static_cast<T>(0));
    mDelayR.assign(delaySize, static_cast<T>(0));
    mChorusWrite = 0;
    mDelayWrite = 0;
    mChorusPhase = 0.;
    mDriveTone[0] = mDriveTone[1] = static_cast<T>(0);
    mDelayTone[0] = mDelayTone[1] = static_cast<T>(0);
    mCrushPhase[0] = mCrushPhase[1] = static_cast<T>(1);
    mCrushHeld[0] = mCrushHeld[1] = static_cast<T>(0);
    mCrushJitter[0] = mCrushJitter[1] = static_cast<T>(0);
    mCrushSeed[0] = 0x3f29a17du;
    mCrushSeed[1] = 0xa173f29du;

    mA.Reset(mSampleRate, patch_detail::Clamp01(mParams.a));
    mB.Reset(mSampleRate, patch_detail::Clamp01(mParams.b));
    mC.Reset(mSampleRate, patch_detail::Clamp01(mParams.c));
    mMix.Reset(mSampleRate, patch_detail::Clamp01(mParams.mix));
    mCurrentAlgorithm = mParams.algorithm;
    mPendingAlgorithm = mParams.algorithm;
    mSwitchPending = false;
    mRoute.Reset(mSampleRate, IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
  }

  void SetParams(const Params& params)
  {
    mParams = params;
    mParams.a = patch_detail::Clamp01(mParams.a);
    mParams.b = patch_detail::Clamp01(mParams.b);
    mParams.c = patch_detail::Clamp01(mParams.c);
    mParams.mix = patch_detail::Clamp01(mParams.mix);
    mA.SetTarget(mParams.a);
    mB.SetTarget(mParams.b);
    mC.SetTarget(mParams.c);
    mMix.SetTarget(mParams.mix);

    mPendingAlgorithm = mParams.algorithm;
    mSwitchPending = mPendingAlgorithm != mCurrentAlgorithm;
    if (mSwitchPending)
      mRoute.SetTarget(static_cast<T>(0));
    else
      mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
  }

  void ProcessBlock(T** outputs, int nChannels, int nFrames)
  {
    if (!outputs || nChannels <= 0 || nFrames <= 0)
      return;

    const int channels = std::min(nChannels, 2);
    for (int i = 0; i < nFrames; ++i)
    {
      T dry[2] = {outputs[0][i], channels > 1 ? outputs[1][i] : outputs[0][i]};
      T wet[2] = {dry[0], dry[1]};
      const T a = mA.Next();
      const T b = mB.Next();
      const T c = mC.Next();
      const T amount = UpdateRoute() * mMix.Next();

      if (amount > static_cast<T>(0) && mCurrentAlgorithm != InsertAlgorithm::Off)
        ProcessWet(dry, wet, a, b, c);

      for (int ch = 0; ch < channels; ++ch)
      {
        const T mixed = dry[ch] + (wet[ch] - dry[ch]) * amount;
        outputs[ch][i] = patch_detail::Sanitize(mixed);
      }
    }
  }

private:
  bool IsRequestedActive() const
  {
    return !mParams.bypass && mParams.algorithm != InsertAlgorithm::Off;
  }

  T UpdateRoute()
  {
    if (mSwitchPending)
    {
      mRoute.SetTarget(static_cast<T>(0));
      const T value = mRoute.Next();
      if (value == static_cast<T>(0))
      {
        mCurrentAlgorithm = mPendingAlgorithm;
        mSwitchPending = false;
        mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
      }
      return value;
    }

    mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
    return mRoute.Next();
  }

  void ProcessWet(const T dry[2], T wet[2], T a, T b, T c)
  {
    switch (mCurrentAlgorithm)
    {
      case InsertAlgorithm::Drive: ProcessDrive(dry, wet, a, b, c); break;
      case InsertAlgorithm::Chorus: ProcessChorus(dry, wet, a, b, c); break;
      case InsertAlgorithm::Delay: ProcessDelay(dry, wet, a, b, c); break;
      case InsertAlgorithm::Crush: ProcessCrush(dry, wet, a, b, c); break;
      case InsertAlgorithm::Off:
      default: break;
    }
  }

  void ProcessDrive(const T dry[2], T wet[2], T driveControl, T toneControl, T biasControl)
  {
    const T drive = static_cast<T>(1) + static_cast<T>(24) * driveControl * driveControl;
    const T bias = (biasControl * static_cast<T>(2) - static_cast<T>(1)) * static_cast<T>(0.35);
    const T cutoff = static_cast<T>(800) *
                     std::pow(static_cast<T>(22.5), toneControl);
    const T alpha = static_cast<T>(1) -
                    std::exp(static_cast<T>(-2. * patch_detail::kPi) * cutoff /
                             static_cast<T>(mSampleRate));
    const T dc = std::tanh(bias * drive);
    const T makeup = static_cast<T>(1) / std::sqrt(drive);

    for (int ch = 0; ch < 2; ++ch)
    {
      const T shaped = (std::tanh((dry[ch] + bias) * drive) - dc) * makeup;
      mDriveTone[ch] += alpha * (shaped - mDriveTone[ch]);
      wet[ch] = mDriveTone[ch];
    }
  }

  void ProcessChorus(const T dry[2], T wet[2], T rateControl, T depthControl, T spreadControl)
  {
    if (mChorusL.empty())
      return;

    const double rateHz = 0.05 + 4.95 * static_cast<double>(rateControl * rateControl);
    const T depthSamples = static_cast<T>(mSampleRate) *
                           (static_cast<T>(0.0003) + depthControl * static_cast<T>(0.0117));
    const T baseSamples = static_cast<T>(mSampleRate * 0.016);
    const double spread = static_cast<double>(spreadControl) * patch_detail::kPi;
    const T modL = static_cast<T>(std::sin(mChorusPhase * 2. * patch_detail::kPi));
    const T modR = static_cast<T>(std::sin(mChorusPhase * 2. * patch_detail::kPi + spread));

    mChorusL[static_cast<size_t>(mChorusWrite)] = dry[0];
    mChorusR[static_cast<size_t>(mChorusWrite)] = dry[1];
    wet[0] = patch_detail::ReadDelay(mChorusL, mChorusWrite,
                                     baseSamples + depthSamples * (static_cast<T>(0.5) +
                                                                   static_cast<T>(0.5) * modL));
    wet[1] = patch_detail::ReadDelay(mChorusR, mChorusWrite,
                                     baseSamples + depthSamples * (static_cast<T>(0.5) +
                                                                   static_cast<T>(0.5) * modR));

    mChorusWrite = (mChorusWrite + 1) % static_cast<int>(mChorusL.size());
    mChorusPhase += rateHz / mSampleRate;
    if (mChorusPhase >= 1.)
      mChorusPhase -= std::floor(mChorusPhase);
  }

  void ProcessDelay(const T dry[2], T wet[2], T timeControl, T feedbackControl, T toneControl)
  {
    if (mDelayL.empty())
      return;

    const T seconds = static_cast<T>(0.02) + timeControl * timeControl * static_cast<T>(0.98);
    const T delaySamples = seconds * static_cast<T>(mSampleRate);
    const T feedback = feedbackControl * static_cast<T>(0.92);
    const T cutoff = static_cast<T>(500) * std::pow(static_cast<T>(36), toneControl);
    const T alpha = static_cast<T>(1) -
                    std::exp(static_cast<T>(-2. * patch_detail::kPi) * cutoff /
                             static_cast<T>(mSampleRate));

    const T tapL = patch_detail::ReadDelay(mDelayL, mDelayWrite, delaySamples);
    const T tapR = patch_detail::ReadDelay(mDelayR, mDelayWrite, delaySamples);
    mDelayTone[0] += alpha * (tapL - mDelayTone[0]);
    mDelayTone[1] += alpha * (tapR - mDelayTone[1]);
    wet[0] = mDelayTone[0];
    wet[1] = mDelayTone[1];

    const T writeL = dry[0] + std::tanh(mDelayTone[1] * feedback);
    const T writeR = dry[1] + std::tanh(mDelayTone[0] * feedback);
    mDelayL[static_cast<size_t>(mDelayWrite)] = patch_detail::Sanitize(writeL);
    mDelayR[static_cast<size_t>(mDelayWrite)] = patch_detail::Sanitize(writeR);
    mDelayWrite = (mDelayWrite + 1) % static_cast<int>(mDelayL.size());
  }

  void ProcessCrush(const T dry[2], T wet[2], T bitsControl, T rateControl, T jitterControl)
  {
    const T effectiveBits = static_cast<T>(4) + bitsControl * static_cast<T>(12);
    const T levels = std::pow(static_cast<T>(2), effectiveBits) - static_cast<T>(1);
    const T baseRate = static_cast<T>(0.02) +
                       rateControl * rateControl * static_cast<T>(0.98);

    for (int ch = 0; ch < 2; ++ch)
    {
      const T jittered = std::clamp(baseRate * (static_cast<T>(1) +
                                                 mCrushJitter[ch] * jitterControl *
                                                   static_cast<T>(0.45)),
                                    static_cast<T>(0.001), static_cast<T>(1));
      mCrushPhase[ch] += jittered;
      if (mCrushPhase[ch] >= static_cast<T>(1))
      {
        mCrushPhase[ch] -= std::floor(mCrushPhase[ch]);
        const T bounded = std::clamp(dry[ch], static_cast<T>(-4), static_cast<T>(4));
        mCrushHeld[ch] = std::round(bounded * levels) / levels;
        mCrushJitter[ch] = static_cast<T>(patch_detail::RandomBipolar(mCrushSeed[ch]));
      }
      wet[ch] = mCrushHeld[ch];
    }
  }

  Params mParams {};
  double mSampleRate = 44100.;
  patch_detail::SmoothValue<T> mA;
  patch_detail::SmoothValue<T> mB;
  patch_detail::SmoothValue<T> mC;
  patch_detail::SmoothValue<T> mMix;
  patch_detail::SmoothValue<T> mRoute;
  InsertAlgorithm mCurrentAlgorithm = InsertAlgorithm::Off;
  InsertAlgorithm mPendingAlgorithm = InsertAlgorithm::Off;
  bool mSwitchPending = false;

  T mDriveTone[2] {};
  std::vector<T> mChorusL;
  std::vector<T> mChorusR;
  int mChorusWrite = 0;
  double mChorusPhase = 0.;
  std::vector<T> mDelayL;
  std::vector<T> mDelayR;
  int mDelayWrite = 0;
  T mDelayTone[2] {};
  T mCrushPhase[2] {};
  T mCrushHeld[2] {};
  T mCrushJitter[2] {};
  uint32_t mCrushSeed[2] {};
};

template<typename T>
class MasterEffect
{
public:
  struct Params
  {
    MasterAlgorithm algorithm = MasterAlgorithm::Off;
    T a = static_cast<T>(0.5);
    T b = static_cast<T>(0.5);
    T c = static_cast<T>(0.5);
    T mix = static_cast<T>(0.4);
    bool bypass = false;
  };

  void Reset(double sampleRate, int /*blockSize*/)
  {
    mSampleRate = sampleRate > 1. ? sampleRate : 44100.;
    mA.Reset(mSampleRate, patch_detail::Clamp01(mParams.a));
    mB.Reset(mSampleRate, patch_detail::Clamp01(mParams.b));
    mC.Reset(mSampleRate, patch_detail::Clamp01(mParams.c));
    mMix.Reset(mSampleRate, patch_detail::Clamp01(mParams.mix));
    mCurrentAlgorithm = mParams.algorithm;
    mPendingAlgorithm = mParams.algorithm;
    mSwitchPending = false;
    mRoute.Reset(mSampleRate, IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
    mCompressorGain = static_cast<T>(1);
    mLimiterGain = static_cast<T>(1);
    mWidthSideLow = static_cast<T>(0);
    mTiltLow[0] = mTiltLow[1] = static_cast<T>(0);
  }

  void SetParams(const Params& params)
  {
    mParams = params;
    mParams.a = patch_detail::Clamp01(mParams.a);
    mParams.b = patch_detail::Clamp01(mParams.b);
    mParams.c = patch_detail::Clamp01(mParams.c);
    mParams.mix = patch_detail::Clamp01(mParams.mix);
    mA.SetTarget(mParams.a);
    mB.SetTarget(mParams.b);
    mC.SetTarget(mParams.c);
    mMix.SetTarget(mParams.mix);

    mPendingAlgorithm = mParams.algorithm;
    mSwitchPending = mPendingAlgorithm != mCurrentAlgorithm;
    if (mSwitchPending)
      mRoute.SetTarget(static_cast<T>(0));
    else
      mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
  }

  void ProcessBlock(T** outputs, int nChannels, int nFrames)
  {
    if (!outputs || nChannels <= 0 || nFrames <= 0)
      return;

    const int channels = std::min(nChannels, 2);
    for (int i = 0; i < nFrames; ++i)
    {
      T dry[2] = {outputs[0][i], channels > 1 ? outputs[1][i] : outputs[0][i]};
      T wet[2] = {dry[0], dry[1]};
      const T a = mA.Next();
      const T b = mB.Next();
      const T c = mC.Next();
      const T amount = UpdateRoute() * mMix.Next();

      if (amount > static_cast<T>(0) && mCurrentAlgorithm != MasterAlgorithm::Off)
        ProcessWet(dry, wet, a, b, c);

      for (int ch = 0; ch < channels; ++ch)
      {
        const T mixed = dry[ch] + (wet[ch] - dry[ch]) * amount;
        outputs[ch][i] = patch_detail::Sanitize(mixed);
      }
    }
  }

private:
  bool IsRequestedActive() const
  {
    return !mParams.bypass && mParams.algorithm != MasterAlgorithm::Off;
  }

  T UpdateRoute()
  {
    if (mSwitchPending)
    {
      mRoute.SetTarget(static_cast<T>(0));
      const T value = mRoute.Next();
      if (value == static_cast<T>(0))
      {
        mCurrentAlgorithm = mPendingAlgorithm;
        mSwitchPending = false;
        mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
      }
      return value;
    }

    mRoute.SetTarget(IsRequestedActive() ? static_cast<T>(1) : static_cast<T>(0));
    return mRoute.Next();
  }

  void ProcessWet(const T dry[2], T wet[2], T a, T b, T c)
  {
    switch (mCurrentAlgorithm)
    {
      case MasterAlgorithm::Compressor: ProcessCompressor(dry, wet, a, b, c); break;
      case MasterAlgorithm::Limiter: ProcessLimiter(dry, wet, a, b, c); break;
      case MasterAlgorithm::Width: ProcessWidth(dry, wet, a, b, c); break;
      case MasterAlgorithm::Tilt: ProcessTilt(dry, wet, a, b, c); break;
      case MasterAlgorithm::Off:
      default: break;
    }
  }

  void ProcessCompressor(const T dry[2], T wet[2], T thresholdControl, T ratioControl,
                         T releaseControl)
  {
    const T thresholdDb = static_cast<T>(-48) + thresholdControl * static_cast<T>(48);
    const T ratio = static_cast<T>(1) + ratioControl * static_cast<T>(19);
    const T releaseSeconds = static_cast<T>(0.02) + releaseControl * releaseControl *
                                                       static_cast<T>(0.98);
    const T peak = std::max(std::fabs(dry[0]), std::fabs(dry[1]));
    T target = static_cast<T>(1);
    if (peak > static_cast<T>(1.0e-9))
    {
      const T inputDb = static_cast<T>(20) * std::log10(peak);
      if (inputDb > thresholdDb)
      {
        const T reductionDb = (inputDb - thresholdDb) *
                              (static_cast<T>(1) - static_cast<T>(1) / ratio);
        target = patch_detail::DbToLinear(-reductionDb);
      }
    }

    const T attackCoeff = std::exp(static_cast<T>(-1) /
                                   (static_cast<T>(0.005) * static_cast<T>(mSampleRate)));
    const T releaseCoeff = std::exp(static_cast<T>(-1) /
                                    (releaseSeconds * static_cast<T>(mSampleRate)));
    const T coeff = target < mCompressorGain ? attackCoeff : releaseCoeff;
    mCompressorGain = target + coeff * (mCompressorGain - target);
    wet[0] = dry[0] * mCompressorGain;
    wet[1] = dry[1] * mCompressorGain;
  }

  void ProcessLimiter(const T dry[2], T wet[2], T ceilingControl, T releaseControl, T inputControl)
  {
    const T ceiling = patch_detail::DbToLinear(static_cast<T>(-18) +
                                                ceilingControl * static_cast<T>(18));
    const T inputGain = patch_detail::DbToLinear(inputControl * static_cast<T>(24));
    const T drivenL = dry[0] * inputGain;
    const T drivenR = dry[1] * inputGain;
    const T peak = std::max(std::fabs(drivenL), std::fabs(drivenR));
    const T target = peak > ceiling ? ceiling / std::max(peak, static_cast<T>(1.0e-12))
                                    : static_cast<T>(1);

    if (target < mLimiterGain)
      mLimiterGain = target;
    else
    {
      const T releaseSeconds = static_cast<T>(0.01) +
                               releaseControl * releaseControl * static_cast<T>(0.49);
      const T coeff = std::exp(static_cast<T>(-1) /
                               (releaseSeconds * static_cast<T>(mSampleRate)));
      mLimiterGain = target + coeff * (mLimiterGain - target);
    }

    wet[0] = std::clamp(drivenL * mLimiterGain, -ceiling, ceiling);
    wet[1] = std::clamp(drivenR * mLimiterGain, -ceiling, ceiling);
  }

  void ProcessWidth(const T dry[2], T wet[2], T widthControl, T monoFreqControl, T balanceControl)
  {
    const T mid = (dry[0] + dry[1]) * static_cast<T>(0.5);
    const T side = (dry[0] - dry[1]) * static_cast<T>(0.5);
    const T cutoff = static_cast<T>(40) *
                     std::pow(static_cast<T>(12.5), monoFreqControl);
    const T alpha = static_cast<T>(1) -
                    std::exp(static_cast<T>(-2. * patch_detail::kPi) * cutoff /
                             static_cast<T>(mSampleRate));
    mWidthSideLow += alpha * (side - mWidthSideLow);
    const T width = widthControl * static_cast<T>(2);
    const T widenedSide = (side - mWidthSideLow) * width;
    const T balance = balanceControl * static_cast<T>(2) - static_cast<T>(1);
    const T leftGain = balance > static_cast<T>(0) ? static_cast<T>(1) - balance
                                                    : static_cast<T>(1);
    const T rightGain = balance < static_cast<T>(0) ? static_cast<T>(1) + balance
                                                     : static_cast<T>(1);
    wet[0] = (mid + widenedSide) * leftGain;
    wet[1] = (mid - widenedSide) * rightGain;
  }

  void ProcessTilt(const T dry[2], T wet[2], T tiltControl, T pivotControl, T driveControl)
  {
    const T tiltDb = (tiltControl * static_cast<T>(2) - static_cast<T>(1)) *
                     static_cast<T>(12);
    const T pivot = static_cast<T>(200) * std::pow(static_cast<T>(25), pivotControl);
    const T alpha = static_cast<T>(1) -
                    std::exp(static_cast<T>(-2. * patch_detail::kPi) * pivot /
                             static_cast<T>(mSampleRate));
    const T lowGain = patch_detail::DbToLinear(-tiltDb * static_cast<T>(0.5));
    const T highGain = patch_detail::DbToLinear(tiltDb * static_cast<T>(0.5));
    const T driveGain = patch_detail::DbToLinear(driveControl * static_cast<T>(6));

    for (int ch = 0; ch < 2; ++ch)
    {
      const T driven = dry[ch] * driveGain;
      mTiltLow[ch] += alpha * (driven - mTiltLow[ch]);
      T shaped = mTiltLow[ch] * lowGain + (driven - mTiltLow[ch]) * highGain;
      if (driveControl > static_cast<T>(1.0e-5))
        shaped = std::tanh(shaped) / std::tanh(driveGain);
      wet[ch] = shaped;
    }
  }

  Params mParams {};
  double mSampleRate = 44100.;
  patch_detail::SmoothValue<T> mA;
  patch_detail::SmoothValue<T> mB;
  patch_detail::SmoothValue<T> mC;
  patch_detail::SmoothValue<T> mMix;
  patch_detail::SmoothValue<T> mRoute;
  MasterAlgorithm mCurrentAlgorithm = MasterAlgorithm::Off;
  MasterAlgorithm mPendingAlgorithm = MasterAlgorithm::Off;
  bool mSwitchPending = false;
  T mCompressorGain = static_cast<T>(1);
  T mLimiterGain = static_cast<T>(1);
  T mWidthSideLow = static_cast<T>(0);
  T mTiltLow[2] {};
};

template<typename T>
class SmoothedGain
{
public:
  void Reset(double sampleRate, T linear)
  {
    mGain.Reset(sampleRate, std::max(static_cast<T>(0), linear));
  }

  void SetLinear(T linear)
  {
    mGain.SetTarget(std::max(static_cast<T>(0), linear));
  }

  void ProcessBlock(T** outputs, int nChannels, int nFrames)
  {
    if (!outputs || nChannels <= 0 || nFrames <= 0)
      return;

    const int channels = std::min(nChannels, 2);
    for (int i = 0; i < nFrames; ++i)
    {
      const T gain = mGain.Next();
      for (int ch = 0; ch < channels; ++ch)
        outputs[ch][i] = patch_detail::Sanitize(outputs[ch][i] * gain);
    }
  }

private:
  patch_detail::SmoothValue<T> mGain;
};

} // namespace fx
} // namespace keith
