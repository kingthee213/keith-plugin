#pragma once
#include "AnalyzerControl.h"
#include "EditorialControls.h"
#include "ControlLayout.h"
#include <cassert>

namespace keith { namespace ui {

inline IRECT CRect(int param)
{
  const auto& c = ControlFor(param);
  return IRECT(c.rect.l, c.rect.t, c.rect.r, c.rect.b);
}

inline IRECT OverlayIRECT(const char* id)
{
  const OverlayRect* overlay = OverlayFor(id);
  assert(overlay);
  return IRECT(overlay->rect.l, overlay->rect.t, overlay->rect.r, overlay->rect.b);
}

inline void BuildEditorial(Keith& plugin, IGraphics* g)
{
  g->AttachCornerResizer(EUIResizerMode::Scale, false);
  g->EnableMouseOver(true);
  g->AttachPanelBackground(kWood);
  g->LoadFont(kFontTitle, OSWALD_FN);
  g->LoadFont(kFontMono, SPACEMONO_FN);
  g->LoadFont(kFontMonoBold, SPACEMONO_BOLD_FN);
  g->AttachControl(new KeithArtwork(g->GetBounds()));
  // The analyzer sits in the panel the city plate used to fill. It is attached after the
  // artwork so it draws on top of the face, and before the controls so nothing native is
  // hidden behind it.
  g->AttachControl(new KeithAnalyzerControl(
      IRECT(kPanelAnalyzer.l, kPanelAnalyzer.t, kPanelAnalyzer.r, kPanelAnalyzer.b),
      plugin.Analyzer()));

  auto knob = [&](int param, const char* label, IColor color = kGreen, bool bipolar = false,
                  bool native = true) {
    const auto& pos = PlacementFor(param);
    g->AttachControl(new KeithKnobControl(pos.Bounds(), param, label, color, true, bipolar, 4.0,
                                         pos.radius, native));
  };

  auto freeKnob = [&](int param, const char* label, IColor color = kGreen, bool bipolar = false) {
    const auto& c = ControlFor(param);
    const float r = std::min(c.rect.W(), c.rect.H()) * 0.42f;
    g->AttachControl(new KeithKnobControl(CRect(param), param, label, color, true, bipolar, 4.0, r,
                                         true));
  };

  auto tabs = [&](int param, std::initializer_list<const char*> choices) {
    auto style = HardButton(kPaper, kInk, 11).WithValueText(MonoBold(10, kInk));
    style.colorSpec.mColors[kPR] = kHotPink;
    g->AttachControl(new IVTabSwitchControl(CRect(param), param, choices, "", style,
                                             EVShape::Rectangle));
  };

  auto menu = [&](int param, const char* label, std::initializer_list<const char*> options) {
    g->AttachControl(new KeithParamMenu(CRect(param), param, label, options));
  };

  auto fader = [&](int param, const char* label) {
    g->AttachControl(new KeithFaderControl(CRect(param), param, label, kHotPink));
  };

  auto fxKnob = [&](int param, int algoParam, int knobIndex, bool master, IColor color) {
    g->AttachControl(new KeithFxKnobControl(CRect(param), param, algoParam, knobIndex, master,
                                            color));
  };

  auto fxAlgo = [&](int algoParam, int bypassParam, const char* label,
                    std::initializer_list<const char*> options) {
    g->AttachControl(new KeithFxAlgoMenu(CRect(algoParam), algoParam, bypassParam, label, options));
  };

  auto fxMix = [&](int param, int algoParam, const char* label, IColor color = kPaper) {
    g->AttachControl(new KeithFxMixKnobControl(CRect(param), param, algoParam, label, color));
  };

  // OSCILLATORS — two oscillators plus a mixer column (not a third osc).
  g->AttachControl(new KeithWaveform(OverlayIRECT("osc1_waveform"), kParamOsc1Shape, kParamOsc1PW,
                                     kGreen));
  g->AttachControl(new KeithWaveform(OverlayIRECT("osc2_waveform"), kParamOsc2Shape, kParamOsc2PW,
                                     kMagenta));
  freeKnob(kParamOsc1Shape, "SHAPE", kGreen);
  freeKnob(kParamOsc1Pitch, "TUNE", kGreen, true);
  freeKnob(kParamOsc1Level, "LEVEL", kGreen);
  freeKnob(kParamOsc1PW, "PW", kGreen);
  tabs(kParamOscSync, {"FREE", "SYNC"});
  freeKnob(kParamOsc2Shape, "SHAPE", kMagenta);
  freeKnob(kParamOsc2Pitch, "TUNE", kMagenta, true);
  freeKnob(kParamOsc2Level, "LEVEL", kMagenta);
  freeKnob(kParamOsc2PW, "PW", kMagenta);
  tabs(kParamOscRing, {"OFF", "ON"});
  freeKnob(kParamSubLevel, "SUB");
  freeKnob(kParamNoiseLevel, "NOISE");
  freeKnob(kParamFmAmount, "FM", kMagenta);

  // FILTER
  tabs(kParamFilterType, {"LP24", "LP12", "BP", "HP"});
  freeKnob(kParamFilterCutoff, "CUTOFF", kGreen);
  freeKnob(kParamFilterReso, "RESO");
  freeKnob(kParamFilterEnv, "ENV", kGreen, true);
  freeKnob(kParamFilterKeyTrack, "KEY");
  freeKnob(kParamVelToFilter, ControlFor(kParamVelToFilter).label, kGreen, true);
  freeKnob(kParamVelToAmp, ControlFor(kParamVelToAmp).label, kGreen, true);

  // AMP — ADSR knobs plus amp-modifier knobs in the strip beside the skyline.
  for (int p = kParamAmpAttack; p <= kParamAmpRelease; ++p)
    freeKnob(p, ControlFor(p).label, kMagenta);
  for (int p = kParamFiltAttack; p <= kParamFiltRelease; ++p)
    freeKnob(p, ControlFor(p).label, kGreen);
  for (int p = kParamNoiseKey; p <= kParamStereoSpread; ++p)
    freeKnob(p, ControlFor(p).label, kGreen, true);

  // LFO 1 + LFO 2 stacked.
  g->AttachControl(new KeithTextControl(OverlayIRECT("lfo1_label"), "LFO 1", MonoBold(11, kHotPink)));
  g->AttachControl(new KeithTextControl(OverlayIRECT("lfo2_label"), "LFO 2",
                                        MonoBold(11, kHotPink)));
  freeKnob(kParamLfoRate, "RATE");
  freeKnob(kParamLfoAmount, "DEPTH", kGreen, true);
  menu(kParamLfoShape, "SHAPE", {"SINE", "TRI", "SAW", "SQUARE", "S&H"});
  menu(kParamLfoDest, "DEST", {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});
  freeKnob(kParamLfo2Rate, "RATE");
  freeKnob(kParamLfo2Amount, "DEPTH", kGreen, true);
  menu(kParamLfo2Shape, "SHAPE", {"SINE", "TRI", "SAW", "SQUARE", "S&H"});
  menu(kParamLfo2Dest, "DEST", {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});

  // MOD MATRIX
  constexpr int matrixSources[] = {kParamMod1Source, kParamMod2Source, kParamMod3Source,
                                   kParamMod4Source};
  constexpr int matrixDestinations[] = {kParamMod1Dest, kParamMod2Dest, kParamMod3Dest,
                                        kParamMod4Dest};
  constexpr int matrixAmounts[] = {kParamMod1Amount, kParamMod2Amount, kParamMod3Amount,
                                   kParamMod4Amount};
  for (int slot = 0; slot < keith::synth::kNumModSlots; ++slot)
  {
    menu(matrixSources[slot], "SOURCE",
         {"OFF", "LFO 1", "LFO 2", "MOD WHEEL", "VELOCITY", "KEY"});
    menu(matrixDestinations[slot], "DEST",
         {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});
    freeKnob(matrixAmounts[slot], "AMT", kGreen, true);
  }

  // Middle row: artwork plate (painted in chassis), sampler + drums.
  g->AttachControl(new KeithPresetMenu(OverlayIRECT("preset_menu")));
  constexpr int pitchParams[] = {kParamPitchAttack, kParamPitchDecay, kParamPitchSustain,
                                 kParamPitchRelease, kParamPitchAmount};
  for (int p : pitchParams)
    freeKnob(p, ControlFor(p).label, kMagenta, p == kParamPitchAmount);
  freeKnob(kParamDrumSlotLevel, "LVL", kGreen, true);
  freeKnob(kParamDrumSlotPan, "PAN", kGreen, true);
  freeKnob(kParamDrumSlotTune, "TUNE", kMagenta, true);
  freeKnob(kParamDrumSlotDecay, "DECAY");
  freeKnob(kParamDrumSlotFilter, "FILT");
  freeKnob(kParamDrumSlotDrive, "DRIVE", kMagenta);
  tabs(kParamDrumSlotMute, {"OFF", "MUTE"});
  tabs(kParamDrumSlotSolo, {"OFF", "SOLO"});
  freeKnob(kParamDrumKitLevel, "KIT", kPaper, true);
  g->AttachControl(new KeithPadGridControl(CRect(kParamDrumSlotSelect), kParamDrumSlotSelect));

  // VINTAGE ANALOG MODELING
  tabs(kParamEngine, {"HYBRID", "ANALOG"});
  freeKnob(kParamTapeBump, "TONE", kMagenta);
  freeKnob(kParamFilterDrive, "COMP", kMagenta);
  freeKnob(kParamTapeSat, "DIST", kMagenta);

  // GLOBAL
  freeKnob(kParamGain, ControlFor(kParamGain).label, kPaper, true);
  menu(kParamPolyMode, ControlFor(kParamPolyMode).label, {"POLY", "MONO"});
  menu(kParamMonoLegato, ControlFor(kParamMonoLegato).label, {"RETRIG", "LEGATO"});
  freeKnob(kParamPorta, ControlFor(kParamPorta).label, kPaper);
  freeKnob(kParamUnisonVoices, ControlFor(kParamUnisonVoices).label, kPaper);
  freeKnob(kParamUnisonDetune, ControlFor(kParamUnisonDetune).label, kPaper);
  freeKnob(kParamSynthMasterLevel, ControlFor(kParamSynthMasterLevel).label, kPaper, true);
  menu(kParamModWheelDest, ControlFor(kParamModWheelDest).label,
       {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});
  freeKnob(kParamModWheelAmount, ControlFor(kParamModWheelAmount).label, kMagenta, true);

  // LEFT RAIL — tape bus and direct/curve trims.
  freeKnob(kParamTapeWow, "WOW", kMagenta);
  freeKnob(kParamTapeFlutter, "FLUT", kMagenta);
  freeKnob(kParamTapeHiss, "HISS", kMagenta);
  freeKnob(kParamTapePrint, "PRINT", kMagenta);
  freeKnob(kParamTapeMix, "MIX", kPaper);
  g->AttachControl(new KeithTextControl(OverlayIRECT("bypass_label"), "BYPASS", MonoBold(11, kPaper)));
  tabs(kParamTapeBypass, {"ACTIVE", "BYPASS"});
  freeKnob(kParamOutLevel, "OUT", kMagenta, true);
  freeKnob(kParamDirectLevel, ControlFor(kParamDirectLevel).label, kGreen, true);
  freeKnob(kParamFilterKeyCurve, ControlFor(kParamFilterKeyCurve).label, kGreen, true);

  // EFFECTS — inserts and master bus.
  fxAlgo(kParamIns1Algo, kParamIns1Bypass, "INS1", {"OFF", "DRIVE", "CHORUS", "DELAY", "CRUSH"});
  fxKnob(kParamIns1A, kParamIns1Algo, 0, false, kGreen);
  fxKnob(kParamIns1B, kParamIns1Algo, 1, false, kGreen);
  fxKnob(kParamIns1C, kParamIns1Algo, 2, false, kGreen);
  fxMix(kParamIns1Mix, kParamIns1Algo, "MIX1", kPaper);
  fxAlgo(kParamIns2Algo, kParamIns2Bypass, "INS2", {"OFF", "DRIVE", "CHORUS", "DELAY", "CRUSH"});
  fxKnob(kParamIns2A, kParamIns2Algo, 0, false, kMagenta);
  fxKnob(kParamIns2B, kParamIns2Algo, 1, false, kMagenta);
  fxKnob(kParamIns2C, kParamIns2Algo, 2, false, kMagenta);
  fxMix(kParamIns2Mix, kParamIns2Algo, "MIX2", kPaper);
  fxAlgo(kParamMstAlgo, kParamMstBypass, "MST", {"OFF", "COMP", "LIMIT", "WIDTH", "TILT"});
  fxKnob(kParamMstA, kParamMstAlgo, 0, true, kPaper);
  fxKnob(kParamMstB, kParamMstAlgo, 1, true, kPaper);
  fxKnob(kParamMstC, kParamMstAlgo, 2, true, kPaper);
  fxMix(kParamMstMix, kParamMstAlgo, "MMIX", kGreen);

  g->AttachControl(new KeithTextControl(OverlayIRECT("midi_hint1"), "MIDI / A-L PLAY", Mono(10, kPaper)));
  g->AttachControl(new KeithTextControl(OverlayIRECT("midi_hint2"), "Z/X OCTAVE", Mono(10, kPaper)));
  g->AttachControl(new IVKeyboardControl(IRECT(kKeyboardBox.l, kKeyboardBox.t, kKeyboardBox.r, kKeyboardBox.b), 36, 84, false, kPaper, kInk,
                                         kHotPink, kPanelAlt, kGreen),
                   kCtrlTagKeyboard);
  g->SetQwertyMidiKeyHandlerFunc([g](const IMidiMsg& msg) {
    auto* keys = g->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>();
    keys->SetNoteFromMidi(msg.NoteNumber(),
                          msg.StatusMsg() == IMidiMsg::kNoteOn && msg.Velocity() > 0);
  });
}

} }
