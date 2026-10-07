#include "Keith.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

Keith::Keith(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");

  GetParam(kParamTapeSat)->InitDouble("FX Tape Saturation", 25., 0., 100., 0.1, "%");
  GetParam(kParamTapeBump)->InitDouble("FX Tape Head Bump", 20., 0., 100., 0.1, "%");
  GetParam(kParamTapeWow)->InitDouble("FX Tape Wow", 10., 0., 100., 0.1, "%");
  GetParam(kParamTapeFlutter)->InitDouble("FX Tape Flutter", 10., 0., 100., 0.1, "%");
  GetParam(kParamTapeHiss)->InitDouble("FX Tape Hiss", 5., 0., 100., 0.1, "%");
  GetParam(kParamTapePrint)->InitDouble("FX Tape Print", 0., 0., 100., 0.1, "%");
  GetParam(kParamTapeMix)->InitDouble("FX Tape Mix", 40., 0., 100., 0.1, "%");
  GetParam(kParamTapeBypass)->InitBool("FX Tape Bypass", false);

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(IColor(255, 22, 23, 27));
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    const IRECT bounds = pGraphics->GetBounds();
    const IRECT inner = bounds.GetPadded(-14.f);
    const IRECT header = inner.GetFromTop(58.f);
    const IRECT footer = inner.GetFromBottom(20.f);
    const IRECT mid = inner.GetReducedFromTop(58.f).GetReducedFromBottom(20.f);

    const IRECT keyboard = mid.GetFromBottom(150.f).GetPadded(-4.f);
    const IRECT upper = mid.GetReducedFromBottom(158.f);

    pGraphics->AttachControl(new ITextControl(header.GetFromLeft(300.f), "KEITH",
      IText(32.f, IColor(255, 236, 236, 242), "Roboto-Regular", EAlign::Near, EVAlign::Middle)));
    pGraphics->AttachControl(new ITextControl(header.GetFromRight(400.f),
      "tape / analog engine - preview build",
      IText(14.f, IColor(255, 150, 152, 164), "Roboto-Regular", EAlign::Far, EVAlign::Middle)));

    const IRECT knobs = upper.GetFromLeft(upper.W() - 104.f);
    const IRECT switches = upper.GetFromRight(104.f);

    // Parameter binding: every visible control is a real parameter (BRIEF 10).
    const int ids[8] = { kParamGain, kParamTapeSat, kParamTapeBump, kParamTapeWow,
                         kParamTapeFlutter, kParamTapeHiss, kParamTapePrint, kParamTapeMix };
    const char* labels[8] = { "Gain", "Sat", "Bump", "Wow", "Flutter", "Hiss", "Print", "Mix" };

    for (int i = 0; i < 8; ++i)
      pGraphics->AttachControl(new IVKnobControl(knobs.GetGridCell(i / 4, i % 4, 2, 4).GetPadded(-6.f), ids[i], labels[i]));

    pGraphics->AttachControl(new IVToggleControl(switches.GetCentredInside(88.f, 44.f), kParamTapeBypass, "Bypass"));

    // On-screen keyboard: click the keys, or play from the computer keyboard.
    pGraphics->AttachControl(new IVKeyboardControl(keyboard, 36, 84, true), kCtrlTagKeyboard);
    pGraphics->SetQwertyMidiKeyHandlerFunc([pGraphics](const IMidiMsg& msg) {
      if (auto* pKeyboard = pGraphics->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>())
        pKeyboard->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
    });

    WDL_String buildInfoStr;
    GetBuildInfoStr(buildInfoStr, __DATE__, __TIME__);
    pGraphics->AttachControl(new ITextControl(footer.GetFromLeft(560.f),
      "Typing keys: A W S E D F T G Y H U J K O L play; Z / X shift octave. Or send MIDI from the host.",
      IText(12.f, IColor(255, 140, 142, 152), "Roboto-Regular", EAlign::Near, EVAlign::Middle)));
    pGraphics->AttachControl(new ITextControl(footer.GetFromRight(200.f), buildInfoStr.Get(),
      IText(11.f, IColor(255, 120, 122, 132), "Roboto-Regular", EAlign::Far, EVAlign::Middle)));
  };
#endif
}

#if IPLUG_DSP
void Keith::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  mDSP.ProcessBlock(outputs, 2, nFrames);
}

void Keith::OnReset()
{
  mDSP.Reset(GetSampleRate(), GetBlockSize());
  SyncTapeParams();
  mDSP.SetGain(GetParam(kParamGain)->Value());
}

void Keith::ProcessMidiMsg(const IMidiMsg& msg)
{
  mDSP.ProcessMidiMsg(msg);
}

void Keith::OnParamChange(int paramIdx)
{
  if (paramIdx == kParamGain)
    mDSP.SetGain(GetParam(kParamGain)->Value());
  else
    SyncTapeParams();
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

  // Per-part instances are wired but bypassed until part-scoped fx_tape_* ids land.
  KeithDSP<sample>::TapeParams partBypass {};
  partBypass.bypass = true;
  partBypass.mix = 0.f;
  for (int part = 0; part < 4; ++part)
    mDSP.SetPartTapeParams(part, partBypass);
}
#endif
