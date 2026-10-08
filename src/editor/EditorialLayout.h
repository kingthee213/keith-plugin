#pragma once
#include "EditorialControls.h"
#include "ControlLayout.h"

namespace keith { namespace ui {

inline IRECT CRect(int param)
{
  const auto& c = ControlFor(param);
  return IRECT(c.rect.l, c.rect.t, c.rect.r, c.rect.b);
}

inline void BuildEditorial(Keith& plugin, IGraphics* g)
{
  g->AttachCornerResizer(EUIResizerMode::Scale, false);
  g->EnableMouseOver(true);
  g->AttachPanelBackground(kWood);
  g->LoadFont("Bangers", BANGERS_FN);
  g->LoadFont(kFontTitle, OSWALD_FN);
  g->LoadFont(kFontMono, SPACEMONO_FN);
  g->LoadFont(kFontMonoBold, SPACEMONO_BOLD_FN);
  g->AttachControl(new KeithArtwork(g->GetBounds()));

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
    auto style = HardButton(kPanel, kPaper, 10).WithValueText(MonoBold(10, kPaper));
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
  g->AttachControl(new KeithTextControl(IRECT(kPanelOsc.l + 8, kPanelOsc.t + 4, kPanelOsc.r - 8,
                                               kPanelOsc.t + 22),
                                        "OSCILLATORS", MonoBold(11, kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(222, 28, 280, 44), "OSC 1", MonoBold(10, kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(278, 28, 336, 44), "OSC 2", MonoBold(10, kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(334, 28, 406, 44), "MIX", MonoBold(10, kPaper)));
  g->AttachControl(new KeithWaveform(IRECT(222, 52, 278, 92), kParamOsc1Shape, kParamOsc1PW,
                                     kGreen));
  g->AttachControl(new KeithWaveform(IRECT(278, 52, 336, 92), kParamOsc2Shape, kParamOsc2PW,
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
  g->AttachControl(new KeithTextControl(IRECT(kPanelFilter.l + 8, kPanelFilter.t + 4,
                                               kPanelFilter.r - 8, kPanelFilter.t + 22),
                                        "FILTER", MonoBold(11, kPaper)));
  tabs(kParamFilterType, {"LP24", "LP12", "BP", "HP"});
  freeKnob(kParamFilterCutoff, "CUTOFF", kGreen);
  freeKnob(kParamFilterReso, "RESO");
  freeKnob(kParamFilterEnv, "ENV", kGreen, true);
  freeKnob(kParamFilterKeyTrack, "KEY");
  freeKnob(kParamFiltAttack, "F ATK");
  freeKnob(kParamFiltDecay, "F DEC");
  freeKnob(kParamFiltSustain, "F SUS");
  freeKnob(kParamFiltRelease, "F REL");

  // AMP — ADSR faders plus compact modifier row.
  g->AttachControl(new KeithTextControl(IRECT(kPanelAmp.l + 8, kPanelAmp.t + 4, kPanelAmp.r - 8,
                                               kPanelAmp.t + 22),
                                        "AMP", MonoBold(11, kPaper)));
  fader(kParamAmpAttack, "A");
  fader(kParamAmpDecay, "D");
  fader(kParamAmpSustain, "S");
  fader(kParamAmpRelease, "R");
  freeKnob(kParamNoiseKey, "N KEY", kGreen, true);
  freeKnob(kParamAmpAttackKey, "A KEY", kGreen, true);
  freeKnob(kParamAmpAttackCurve, "A BND", kGreen, true);
  freeKnob(kParamAmpDecayKey, "D KEY", kGreen, true);
  freeKnob(kParamAmpDecayCurve, "D BND", kGreen, true);
  freeKnob(kParamStereoDelay, "ST DL");
  freeKnob(kParamStereoDelayKey, "ST TR", kGreen, true);
  freeKnob(kParamStereoSpread, "ST WD", kGreen, true);
  freeKnob(kParamDirectLevel, "DIRECT");
  freeKnob(kParamFilterKeyCurve, "C BND", kGreen, true);

  // LFO 1 + LFO 2 stacked.
  g->AttachControl(new KeithTextControl(IRECT(kPanelLfo.l + 8, kPanelLfo.t + 4, kPanelLfo.r - 8,
                                               kPanelLfo.t + 22),
                                        "LFO", MonoBold(11, kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(776, 24, 868, 38), "LFO 1", MonoBold(9, kHotPink)));
  g->AttachControl(new KeithTextControl(IRECT(776, 134, 868, 148), "LFO 2",
                                        MonoBold(9, kHotPink)));
  g->AttachControl(new KeithWaveform(IRECT(872, 60, 950, 100), kParamLfoRate, kParamLfoShape,
                                     kHotPink));
  freeKnob(kParamLfoRate, "RATE");
  freeKnob(kParamLfoAmount, "DEPTH", kGreen, true);
  menu(kParamLfoShape, "SHAPE", {"SINE", "TRI", "SAW", "SQUARE", "S&H"});
  menu(kParamLfoDest, "DEST", {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});
  freeKnob(kParamLfo2Rate, "RATE");
  freeKnob(kParamLfo2Amount, "DEPTH", kGreen, true);
  menu(kParamLfo2Shape, "SHAPE", {"SINE", "TRI", "SAW", "SQUARE", "S&H"});
  menu(kParamLfo2Dest, "DEST", {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});
  freeKnob(kParamModWheelAmount, "MW AMT", kMagenta, true);
  menu(kParamModWheelDest, "MW DST", {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL", "FM"});

  // MOD MATRIX
  g->AttachControl(new KeithTextControl(IRECT(kPanelMod.l + 8, kPanelMod.t + 4, kPanelMod.r - 8,
                                               kPanelMod.t + 22),
                                        "MOD MATRIX", MonoBold(11, kPaper)));
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
  freeKnob(kParamVelToAmp, "V AMP");
  freeKnob(kParamVelToFilter, "V FLT");

  // Middle row: artwork plate (painted in chassis), sampler + drums.
  g->AttachControl(new KeithTextControl(IRECT(kPanelSample.l + 8, kPanelSample.t + 4,
                                               kPanelSample.r - 8, kPanelSample.t + 22),
                                        "ANALOG REPLICA / DRUMS / LOADED SOUNDS",
                                        MonoBold(10, kPaper)));
  g->AttachControl(new KeithPresetMenu(IRECT(710, 300, 988, 340)));
  constexpr int pitchParams[] = {kParamPitchAttack, kParamPitchDecay, kParamPitchSustain,
                                 kParamPitchRelease, kParamPitchAmount};
  for (int p : pitchParams)
    freeKnob(p, "P", kMagenta, p == kParamPitchAmount);
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
  g->AttachControl(new KeithTextControl(IRECT(kPanelVintage.l + 8, kPanelVintage.t + 4,
                                               kPanelVintage.r - 8, kPanelVintage.t + 22),
                                        "VINTAGE ANALOG MODELING", MonoBold(10, kPaper)));
  tabs(kParamEngine, {"HYBRID", "ANALOG"});
  freeKnob(kParamTapeBump, "TONE", kMagenta);
  freeKnob(kParamFilterDrive, "COMP", kMagenta);
  freeKnob(kParamTapeSat, "DIST", kMagenta);

  // GLOBAL
  g->AttachControl(new KeithTextControl(IRECT(kPanelGlobal.l + 8, kPanelGlobal.t + 4,
                                               kPanelGlobal.r - 8, kPanelGlobal.t + 22),
                                        "GLOBAL", MonoBold(11, kPaper)));
  freeKnob(kParamGain, "VOLUME", kPaper, true);
  tabs(kParamPolyMode, {"POLY", "MONO"});
  tabs(kParamMonoLegato, {"RETRIG", "LEGATO"});
  freeKnob(kParamPorta, "GLIDE", kPaper);
  freeKnob(kParamUnisonVoices, "UNISON", kPaper);
  freeKnob(kParamUnisonDetune, "DETUNE", kPaper);
  freeKnob(kParamSynthMasterLevel, "SYNTH", kPaper, true);
  g->AttachControl(new KeithMeterControl(IRECT(852, 708, 898, 776), kParamGain));

  // EFFECTS — inserts, master bus and remaining tape controls.
  g->AttachControl(new KeithTextControl(IRECT(kPanelFx.l + 8, kPanelFx.t + 4, kPanelFx.r - 8,
                                               kPanelFx.t + 22),
                                        "EFFECTS", MonoBold(11, kPaper)));
  freeKnob(kParamTapeWow, "WOW", kMagenta);
  freeKnob(kParamTapeFlutter, "FLUT", kMagenta);
  freeKnob(kParamTapeHiss, "HISS", kMagenta);
  freeKnob(kParamTapePrint, "PRINT", kMagenta);
  freeKnob(kParamTapeMix, "MIX", kPaper);
  tabs(kParamTapeBypass, {"ACTIVE", "BYPASS"});
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
  freeKnob(kParamOutLevel, "OUT", kMagenta, true);

  g->AttachControl(new KeithTextControl(IRECT(42, 746, 338, 770), "MIDI IN / A-L PLAY",
                                        Mono(12, kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(42, 771, 338, 790), "Z/X OCTAVE", Mono(11, kPaper)));
  g->AttachControl(new IVKeyboardControl(IRECT(380, 744, 1248, 789), 36, 84, false, kPaper, kInk,
                                         kHotPink, kPanelAlt, kGreen),
                   kCtrlTagKeyboard);
  g->SetQwertyMidiKeyHandlerFunc([g](const IMidiMsg& msg) {
    auto* keys = g->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>();
    keys->SetNoteFromMidi(msg.NoteNumber(),
                          msg.StatusMsg() == IMidiMsg::kNoteOn && msg.Velocity() > 0);
  });
}

} }
