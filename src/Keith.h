#pragma once

#include "IPlug_include_in_plug_hdr.h"

const int kNumPresets = 1;

enum EParams
{
  kParamGain = 0,
  kNumParams
};

#if IPLUG_DSP
#include "Keith_DSP.h"
#endif

using namespace iplug;

class Keith final : public Plugin
{
public:
  Keith(const InstanceInfo& info);

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;

private:
  KeithDSP<sample> mDSP;
#endif
};
