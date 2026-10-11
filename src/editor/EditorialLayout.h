#pragma once
#include "EditorialControls.h"
#include "ControlLayout.h"
#include "../sampler/SampleDropZone.h"
#include "KeithPage.h"
#include <cassert>

// KeithPadMidiButton is declared in KeithControls.h (inside keith::ui) with only a forward
// declaration of Keith, so its body lives here where the full type is available. This header
// is included by Keith.cpp, after Keith.h.
namespace keith { namespace ui {

inline KeithPadMidiButton::KeithPadMidiButton(const IRECT& bounds, Keith& plugin)
: IControl(bounds)
, mPlugin(plugin)
{
}

inline void KeithPadMidiButton::Draw(IGraphics& g)
{
  const bool on = mPlugin.PadMidiEnabled();
  // Lit reads as ON, dim reads as OFF, so the state is visible at a glance.
  const IColor fill = on ? kGreen : kPanelAlt;
  const IColor ink = on ? kInk : kPaper.WithOpacity(0.55f);
  g.FillRect(fill, mRECT);
  g.DrawRect(on ? kPaper : kLine, mRECT, nullptr, on ? 3.f : 2.f);
  g.DrawText(MonoBold(11.f, ink, EAlign::Center, EVAlign::Middle),
             on ? "PAD MIDI ON" : "PAD MIDI OFF", mRECT);
}

inline void KeithPadMidiButton::OnMouseDown(float, float, const IMouseMod&)
{
  mPlugin.SetPadMidiEnabled(!mPlugin.PadMidiEnabled());
  SetDirty(false);
}

} } // namespace keith::ui

namespace keith { namespace ui {

inline IRECT CRect(int param)
{
  const auto& c = ControlFor(param);
  return IRECT(c.rect.l, c.rect.t, c.rect.r, c.rect.b);
}

inline IRECT OverlayIRECT(const char* id)
{
  const OverlayRect* overlay = OverlayFor(id);
  // A missing overlay used to be an assert followed by a null deref. asserts compile out in
  // release, so this became a silent segfault the moment the editor was built -- which is
  // what took a host down. A missing id is a programming error that must never deref, so
  // fall back to a zero rect and make the failure visible.
  if (!overlay)
  {
    assert(false && "OverlayIRECT: no such overlay id");
    return IRECT(0.f, 0.f, 0.f, 0.f);
  }
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

  // The whole editor accepts a dropped audio file. Attached last of the background layers and
  // before the controls, so a drop on top of a knob still reaches the zone while the knob
  // keeps the mouse for clicks -- iPlug2 routes the drop to the control under the cursor that
  // accepts it, and a control that never overrides OnDrop ignores it.
  g->AttachControl(new keith::ui::KeithSampleDropZone(g->GetBounds(), plugin.Sampler()));

  // The rail page buttons. They share one page state, which lives on the plugin so it survives
  // while the editor rebuilds and so both buttons agree on which page is open. Deliberately not
  // a parameter: a page switch is not part of a patch and must not be saved.
  g->AttachControl(new keith::ui::KeithPageButton(OverlayIRECT("sampler_button"),
                                                  keith::ui::KeithPage::Sampler,
                                                  plugin.PageState()));
  g->AttachControl(new keith::ui::KeithPageButton(OverlayIRECT("wavetable_button"),
                                                  keith::ui::KeithPage::Wavetable,
                                                  plugin.PageState()));


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

  // OSCILLATORS -- three oscillators plus a mixer column.
  //
  // Osc 3 gets its own waveform and knobs in the same shape as the other two, so the three
  // read as one family across the panel. Its source switch, morph knob and ON tab are extra
  // because it can read a wavetable rather than only the built-in shapes.
  g->AttachControl(new KeithWaveform(OverlayIRECT("osc1_waveform"), kParamOsc1Shape, kParamOsc1PW,
                                     kGreen));
  g->AttachControl(new KeithWaveform(OverlayIRECT("osc2_waveform"), kParamOsc2Shape, kParamOsc2PW,
                                     kMagenta));
  g->AttachControl(new KeithWaveform(OverlayIRECT("osc3_waveform"), kParamOsc3Shape, kParamOsc3PW,
                                     kCyan));
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
  freeKnob(kParamOsc3Shape, "SHAPE", kCyan);
  freeKnob(kParamOsc3Pitch, "TUNE", kCyan, true);
  freeKnob(kParamOsc3Level, "LEVEL", kCyan);
  freeKnob(kParamOsc3PW, "PW", kCyan);
  // The wavetable oscillator's own two controls, then its on/off. ON rather than OFF/ON
  // because the tab is the switch itself -- it reads as the state, not a choice between two.
  tabs(kParamOsc3Source, {"SHAPES", "TABLE"});
  freeKnob(kParamOsc3Morph, "MORPH", kCyan);
  tabs(kParamOsc3Enable, {"OFF", "ON"});
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
  // PAD MIDI sits under the drum pad slot knobs. Editor-only: see KeithPadMidiButton.
  g->AttachControl(new KeithPadMidiButton(OverlayIRECT("pad_midi_switch"), plugin));
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
  // BYPASS itself moved into VINTAGE ANALOG MODELING, and its rail label went with it. The
  // KeithTextControl that used to sit here looked up the "bypass_label" overlay, which no
  // longer exists -- a null deref at editor build. The label is now drawn by the vintage
  // panel's own switch row.
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

  // -----------------------------------------------------------------------------------------
  // The sampler / wavetable page
  //
  // Attached LAST, after every panel control, so the surfaces draw over them.
  //
  // This is the whole reason the block sits at the end of the function rather than here at the
  // top: iPlug2 draws attached controls in attachment order, so a page surface attached before
  // the panels is painted UNDER them and the main page shows through. It was attached early, the
  // comment above claimed otherwise, and that is exactly the bug the owner reported -- the
  // sampler and wavetable pages appearing half-drawn with the main page's controls on top.
  //
  // Every page control also skips drawing when its page is closed. See KeithPage.h for why
  // visibility works that way rather than by attaching and detaching on each page change.
  //
  // The page is one surface per half, with the contents on top. That ordering matters: a surface
  // covers the panels, and anything drawn under it would be invisible.
  // -----------------------------------------------------------------------------------------
  {
    // Non-const, matching PageState()'s return. The page controls take a const pointer, so
    // this converts down without ambiguity.
    keith::ui::KeithPage* pageState = plugin.PageState();

    // --- the two page surfaces, drawn over the panels ---------------------------------------
    g->AttachControl(new keith::ui::KeithPageSurface(OverlayIRECT("page_sampler"), pageState,
                                                     keith::ui::KeithPage::Sampler));
    g->AttachControl(new keith::ui::KeithPageSurface(OverlayIRECT("page_wavetable"), pageState,
                                                     keith::ui::KeithPage::Wavetable));

    // --- SAMPLER ------------------------------------------------------------------------------
    const keith::ui::KeithPage kSampler = keith::ui::KeithPage::Sampler;

    // The page title, inside the surface so it reads as the page's own heading.
    {
      IRECT r = OverlayIRECT("page_sampler");
      r = IRECT(r.L + 14.f, r.T + 4.f, r.R - 14.f, r.T + 30.f);
      g->AttachControl(new keith::ui::KeithPageLabel(r, pageState, kSampler, "SAMPLER", 15.f));
    }

    // PLAY and SLICE: full width, stacked, directly under the title. The detail text is what
    // tells a user which one does what without having to press them.
    g->AttachControl(new keith::ui::KeithPageModeButton(
        OverlayIRECT("page_sample_play"), pageState, kSampler, kParamSampleMode, 0,
        "PLAY", "whole sample, pitched across the keyboard"));
    g->AttachControl(new keith::ui::KeithPageModeButton(
        OverlayIRECT("page_sample_slice"), pageState, kSampler, kParamSampleMode, 1,
        "SLICE", "one slice per key, each at its recorded pitch"));

    // The browser, the waveform and the slice row.
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_sample_browser"), pageState,
                                                   kSampler));
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_sample_wave"), pageState,
                                                   kSampler));
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_sample_slices"), pageState,
                                                   kSampler));

    {
      IRECT r = OverlayIRECT("page_sample_browser");
      g->AttachControl(new keith::ui::KeithPageLabel(
          IRECT(r.L + 8.f, r.T + 6.f, r.R - 8.f, r.T + 22.f), pageState, kSampler, "SAMPLES", 10.f));
    }
    {
      IRECT r = OverlayIRECT("page_sample_slices");
      g->AttachControl(new keith::ui::KeithPageLabel(
          IRECT(r.L, r.T - 2.f, r.L + 52.f, r.T + 13.f), pageState, kSampler, "SLICES", 8.f));
    }

    // The six sampler knobs, each a real parameter-bound knob like the panels use.
    {
      // Labels only. The sampler has no parameters for start, end, length, root, tune or gain
      // yet, so these are captions on empty cells rather than knobs -- drawing a knob here would
      // show a control that cannot be moved. There is deliberately no param field: an earlier
      // version carried a -1 for each, which read as though six parameters existed.
      static const struct { const char* id; const char* label; } kPageKnobs[] = {
        {"page_knob_start",  "START"},
        {"page_knob_end",    "END"},
        {"page_knob_length", "LENGTH"},
        {"page_knob_root",   "ROOT"},
        {"page_knob_tune",   "TUNE"},
        {"page_knob_gain",   "GAIN"},
      };
      for (const auto& k : kPageKnobs)
        g->AttachControl(new keith::ui::KeithPageLabel(
            IRECT(OverlayIRECT(k.id).L, OverlayIRECT(k.id).B, OverlayIRECT(k.id).R,
                  OverlayIRECT(k.id).B + 14.f),
            pageState, kSampler, k.label, 8.f, EAlign::Center));
    }

    // WARP and TEMPO SYNC are captions only: neither has a parameter behind it yet, so drawing a
    // tab would show a control that cannot be pressed.
    g->AttachControl(new keith::ui::KeithPageLabel(OverlayIRECT("page_sample_warp"), pageState,
                                                   kSampler, "WARP", 9.f, EAlign::Center));
    g->AttachControl(new keith::ui::KeithPageLabel(OverlayIRECT("page_sample_temposync"), pageState,
                                                   kSampler, "TEMPO SYNC", 9.f, EAlign::Center));

    // REVERSE is real. A tab switch on kParamSampleReverse, page-aware so it only appears while
    // the sampler page is open, and bound to a parameter so it is saved with the patch.
    {
      const IRECT r = OverlayIRECT("page_sample_reverse");
      auto style = HardButton(kPaper, kInk, 11).WithValueText(MonoBold(10, kInk));
      style.colorSpec.mColors[kPR] = kHotPink;
      g->AttachControl(new keith::ui::KeithPageTabSwitch(r, kParamSampleReverse, {"FWD", "REV"},
                                                         "", style, pageState, kSampler));
    }

    // --- WAVETABLE ----------------------------------------------------------------------------
    const keith::ui::KeithPage kWavetable = keith::ui::KeithPage::Wavetable;
    {
      IRECT r = OverlayIRECT("page_wavetable");
      r = IRECT(r.L + 14.f, r.T + 4.f, r.R - 14.f, r.T + 30.f);
      g->AttachControl(new keith::ui::KeithPageLabel(r, pageState, kWavetable, "WAVETABLE", 15.f));
    }
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_wt_banks"), pageState,
                                                   kWavetable));
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_wt_frames"), pageState,
                                                   kWavetable));
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_wt_view"), pageState,
                                                   kWavetable));
    // The controls column gets a panel too, so it reads as a group the way the mock draws it.
    // Without this the morph knob and the source tabs float on the page background while the mock
    // shows them boxed -- a mock that disagrees with the build is worse than no mock.
    g->AttachControl(new keith::ui::KeithPagePanel(OverlayIRECT("page_wt_controls"), pageState,
                                                   kWavetable));
    {
      IRECT r = OverlayIRECT("page_wt_banks");
      g->AttachControl(new keith::ui::KeithPageLabel(
          IRECT(r.L + 8.f, r.T + 6.f, r.R - 8.f, r.T + 22.f), pageState, kWavetable, "BANKS", 10.f));
    }
    // The controls column: the morph knob and the osc 3 source tabs.
    //
    // Both parameters already have controls on the oscillator panels. These are SECOND controls on
    // the same parameters, which is deliberate: the panel is where you set osc 3 while patching,
    // the page is where you set it while looking at the bank it reads. iPlug2 supports multiple
    // controls per parameter and keeps them in step, so the two cannot disagree.
    g->AttachControl(new keith::ui::KeithPageLabel(OverlayIRECT("page_wt_morph_label"), pageState,
                                                   kWavetable, "MORPH", 9.f));

    // A real knob on kParamOsc3Morph, using the same control type the panels use so it looks and
    // behaves identically. KeithPageKnob's signature is (bounds, param, label, accent, pageState,
    // page); it forwards to KeithKnobControl(bounds, paramIdx, label, accent) and takes that
    // class's defaults for showValue, bipolar, gearing, radius and nativeBody. Do not mistake the
    // seventh KeithKnobControl parameter for a taper -- it is GEARING, the drag sensitivity, and
    // the page has no opinion on it, so it takes the default rather than inventing a value.
    {
      IRECT r = OverlayIRECT("page_wt_morph");
      // KeithPageKnob, not KeithKnobControl: the plain control draws whenever it is attached,
      // with no notion of a page, so it would sit over the panels permanently. The page-aware
      // version draws only while the wavetable page is open.
      g->AttachControl(new keith::ui::KeithPageKnob(r, kParamOsc3Morph, "", kCyan,
                                                    pageState, kWavetable));
    }

    g->AttachControl(new keith::ui::KeithPageLabel(OverlayIRECT("page_wt_src_label"), pageState,
                                                   kWavetable, "OSC 3 SOURCE", 9.f));

    // The SHAPES / TABLE pair. Built exactly as the tabs lambda does, but with the page's rects
    // instead of the panel's -- IVTabSwitchControl takes the whole pair's area and splits it, so
    // it spans both rects at once.
    {
      const IRECT shapes = OverlayIRECT("page_wt_tab_shapes");
      const IRECT table = OverlayIRECT("page_wt_tab_table");
      const IRECT span(shapes.L, shapes.T, table.R, table.B);
      auto style = HardButton(kPaper, kInk, 11).WithValueText(MonoBold(10, kInk));
      style.colorSpec.mColors[kPR] = kHotPink;
      g->AttachControl(new keith::ui::KeithPageTabSwitch(span, kParamOsc3Source,
                                                         {"SHAPES", "TABLE"}, "", style,
                                                         pageState, kWavetable));
    }
  }
}

} }
