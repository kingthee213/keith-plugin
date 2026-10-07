#pragma once

#include "MidiSynth.h"
#include "Oscillator.h"
#include "Smoothers.h"

using namespace iplug;

enum EKeithModulations
{
  kModGain = 0,
  kNumKeithModulations
};

template<typename T>
class KeithDSP
{
public:
  class Voice : public SynthVoice
  {
  public:
    bool GetBusy() const override
    {
      return mActive;
    }

    void Trigger(double level, bool isRetrigger) override
    {
      mActive = true;
      mLevel = level;
      mOSC.Reset();
    }

    void Release() override
    {
      mActive = false;
    }

    void ProcessSamplesAccumulating(T** inputs, T** outputs, int nInputs, int nOutputs, int startIdx, int nFrames) override
    {
      const double pitch = mInputs[kVoiceControlPitch].endValue;
      const double freq = 440. * pow(2., pitch);

      for (int i = startIdx; i < startIdx + nFrames; ++i)
      {
        if (!mActive)
          continue;

        const T sample = static_cast<T>(mOSC.Process(freq) * mLevel);
        outputs[0][i] += sample;
        if (nOutputs > 1)
          outputs[1][i] += sample;
      }
    }

    void SetSampleRateAndBlockSize(double sampleRate, int blockSize) override
    {
      mOSC.SetSampleRate(sampleRate);
    }

  private:
    FastSinOscillator<T> mOSC;
    bool mActive = false;
    double mLevel = 0.;
  };

  KeithDSP()
  {
    for (int i = 0; i < 16; ++i)
      mSynth.AddVoice(new Voice(), 0);
  }

  void Reset(double sampleRate, int blockSize)
  {
    mSynth.SetSampleRateAndBlockSize(sampleRate, blockSize);
    mSynth.Reset();
    mModulationsData.Resize(blockSize * kNumKeithModulations);
    mModulations.Empty();

    for (int i = 0; i < kNumKeithModulations; ++i)
      mModulations.Add(mModulationsData.Get() + (blockSize * i));

    mParamSmoother.SetSmoothTime(10., sampleRate);
    mParamSmoother.SetValue(static_cast<T>(mParamsToSmooth[kModGain]));
  }

  void ProcessBlock(T** outputs, int nOutputs, int nFrames)
  {
    for (int ch = 0; ch < nOutputs; ++ch)
      memset(outputs[ch], 0, nFrames * sizeof(T));

    mParamSmoother.ProcessBlock(mParamsToSmooth, mModulations.GetList(), nFrames);
    mSynth.ProcessBlock(mModulations.GetList(), outputs, 0, nOutputs, nFrames);

    for (int s = 0; s < nFrames; ++s)
    {
      const T gain = mModulations.GetList()[kModGain][s];
      outputs[0][s] *= gain;
      if (nOutputs > 1)
        outputs[1][s] *= gain;
    }
  }

  void ProcessMidiMsg(const IMidiMsg& msg)
  {
    mSynth.AddMidiMsgToQueue(msg);
  }

  void SetGain(double gainPercent)
  {
    mParamsToSmooth[kModGain] = static_cast<T>(gainPercent / 100.);
  }

private:
  MidiSynth mSynth {VoiceAllocator::kPolyModePoly, MidiSynth::kDefaultBlockSize};
  WDL_TypedBuf<T> mModulationsData;
  WDL_PtrList<T> mModulations;
  LogParamSmooth<T, kNumKeithModulations> mParamSmoother;
  sample mParamsToSmooth[kNumKeithModulations] {};
};
