#pragma once
#include "EditorialControls.h"
#include "ControlLayout.h"

namespace keith { namespace ui {
inline void BuildEditorial(Keith& plugin, IGraphics* g)
{
  g->AttachCornerResizer(EUIResizerMode::Scale, false);
  g->EnableMouseOver(true);
  g->AttachPanelBackground(kInk);
  g->LoadFont("Bangers", BANGERS_FN);
  g->LoadFont(kFontTitle, OSWALD_FN);
  g->LoadFont(kFontMono, SPACEMONO_FN);
  g->LoadFont(kFontMonoBold, SPACEMONO_BOLD_FN);
  g->AttachControl(new KeithArtwork(g->GetBounds()));

  auto* pages = new KeithPageDeck(IRECT(kPageWell.l, kPageWell.t, kPageWell.r, kPageWell.b));
  g->AttachControl(pages);
  auto* source = pages->AddPage();
  auto* modulation = pages->AddPage();
  auto* filterPage = pages->AddPage();
  auto* shape = pages->AddPage();
  auto* tape = pages->AddPage();
  auto* library = pages->AddPage();
  source->AddChildControl(new KeithWorkspaceMaterial(SOURCE_MATERIAL_FN));
  tape->AddChildControl(new KeithWorkspaceMaterial(TAPE_MATERIAL_FN));
  const char* pageNames[] = {
    "01  SOURCE", "02  MODULATION", "03  FILTER", "04  ENVELOPES", "05  TAPE", "06  LIBRARY"
  };
  auto box = [](Box b) { return IRECT(b.l, b.t, b.r, b.b); };
  for (int i = 0; i < kNavCount; ++i)
    g->AttachControl(new KeithNavButton(box(NavCell(i)), pageNames[i], pages, i, false, kNavTextPad));

  g->AttachControl(new KeithTextControl(IRECT(334, 30, 870, 53), "PATCH / FAMILY 01-05", Mono(12, kPaper)));
  g->AttachControl(new KeithPresetMenu(IRECT(334, 58, 938, 109)), kCtrlTagPresetManager);
  auto step = [&plugin, g](int direction) {
    plugin.RestorePreset((plugin.GetCurrentPresetIdx() + direction + plugin.NPresets()) % plugin.NPresets());
    g->SetAllControlsDirty();
  };
  const IVStyle button = HardButton(kPanelAlt, kPaper, 18);
  g->AttachControl(new IVButtonControl(IRECT(950, 58, 995, 109), [step](IControl*) { step(-1); }, "<", button));
  g->AttachControl(new IVButtonControl(IRECT(1006, 58, 1051, 109), [step](IControl*) { step(1); }, ">", button));
  const int outputParam = kParamGain;
  g->AttachControl(new KeithKnobControl(PlacementFor(outputParam).Bounds(), outputParam, "OUTPUT", kPaper));

  auto text = [](IContainerBase* page, const IRECT& r, const char* s, IText style) {
    page->AddChildControl(new KeithTextControl(r, s, style));
  };
  auto knob = [](IContainerBase* page, int param, const char* label,
                 IColor color = kGreen, bool bipolar = false) {
    const auto& position = PlacementFor(param);
    page->AddChildControl(new KeithKnobControl(position.Bounds(), param, label, color, true, bipolar, 4.0, position.radius));
  };
  auto tabs = [](IContainerBase* page, const IRECT& r, int param, std::initializer_list<const char*> choices) {
    auto style = HardButton(kPanel, kPaper, 12).WithValueText(MonoBold(12, kPaper));
    style.colorSpec.mColors[kPR] = kMagenta;
    page->AddChildControl(new IVTabSwitchControl(r, param, choices, "", style, EVShape::Rectangle));
  };
  auto freeKnob = [](IContainerBase* page, Box b, int param, const char* label,
                     IColor color = kGreen, bool bipolar = false) {
    page->AddChildControl(new KeithKnobControl(IRECT(b.l, b.t, b.r, b.b), param, label, color,
                                               true, bipolar, 4.0, 24.f, true));
  };

  // A continuous source workspace, with oversized morph controls and a side mixer.
  auto oscillator = [&](float y, const char* number, int morph, int pitch, int level, int width, IColor color) {
    text(source, IRECT(58, y, 210, y+30), number, MonoBold(22, kPaper));
    knob(source, morph, "MORPH", color, false);
    source->AddChildControl(new KeithWaveform(IRECT(252, y+30, 712, y+107), morph, width, color));
    source->AddChildControl(new KeithShapeReadoutControl(IRECT(528, y, 712, y+24), morph,
                                                       Mono(12, kPaper, EAlign::Far)));
    knob(source, pitch, "TUNE", color, true);
    knob(source, level, "LEVEL", color);
    knob(source, width, "PULSE WIDTH", color);
  };
  oscillator(224, "OSC / 01", kParamOsc1Shape, kParamOsc1Pitch, kParamOsc1Level, kParamOsc1PW, kGreen);
  oscillator(460, "OSC / 02", kParamOsc2Shape, kParamOsc2Pitch, kParamOsc2Level, kParamOsc2PW, kMagenta);
  text(source, IRECT(786, 224, 974, 254), "SOURCE MIX", MonoBold(16,kPaper));
  knob(source, kParamSubLevel, "SUB");
  knob(source, kParamNoiseLevel, "NOISE");
  knob(source, kParamFmAmount, "FM", kMagenta);
  knob(source, kParamSynthMasterLevel, "SYNTH LEVEL", kPaper, true);
  text(source, box(kSyncLabel), "OSC SYNC", Mono(11, kPaper));
  tabs(source, box(kSyncTabs), kParamOscSync, {"FREE", "SYNC"});
  text(source, box(kRingLabel), "RING", Mono(11, kPaper));
  tabs(source, box(kRingTabs), kParamOscRing, {"OFF", "ON"});

  text(modulation, box(kModLfoHeader), "LFO", Title(kHeadingPx, kPaper));
  freeKnob(modulation, kModRate, kParamLfoRate, "RATE");
  freeKnob(modulation, kModLfoAmount, kParamLfoAmount, "AMOUNT", kGreen, true);
  text(modulation, box(kModShapeLabel), "SHAPE", Mono(12, kPaper));
  tabs(modulation, box(kModShapeTabs), kParamLfoShape, {"SINE", "TRI", "SAW", "SQUARE", "S&H"});
  text(modulation, box(kModLfoDestLabel), "DEST", Mono(12, kPaper));
  tabs(modulation, box(kModLfoDestTabs), kParamLfoDest, {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL"});
  text(modulation, box(kModWheelHeader), "MOD WHEEL", Title(kHeadingPx, kPaper));
  text(modulation, box(kModWheelCc), "CC1", Mono(16, kPaper, EAlign::Far));
  freeKnob(modulation, kModWheelAmount, kParamModWheelAmount, "AMOUNT", kMagenta, true);
  text(modulation, box(kModWheelDestLabel), "DEST", Mono(12, kPaper));
  tabs(modulation, box(kModWheelDestTabs), kParamModWheelDest, {"OFF", "CUTOFF", "PITCH", "SHAPE", "LEVEL"});
  text(modulation, box(kVelHeader), "VELOCITY", Title(kHeadingPx, kPaper));
  freeKnob(modulation, kVelAmp, kParamVelToAmp, "TO AMP");
  freeKnob(modulation, kVelFilter, kParamVelToFilter, "TO FILTER", kGreen);
  text(modulation, box(kLegatoLabel), "MONO LEGATO", Mono(11, kPaper));
  tabs(modulation, box(kLegatoTabs), kParamMonoLegato, {"RETRIG", "LEGATO"});

  text(filterPage, box(kFilterHeader), "RESPONSE", Title(kHeadingPx, kPaper));
  filterPage->AddChildControl(new KeithFilterResponse(box(kFilterPlot)));
  freeKnob(filterPage, kKeyTrack, kParamFilterKeyTrack, "KEY TRACK");
  freeKnob(filterPage, kFilterDrive, kParamFilterDrive, "DRIVE", kMagenta);

  // One large envelope desk, rather than three miniature plots competing for room.
  auto* envelopes = new KeithPageDeck(IRECT(50, 275, 988, 704));
  shape->AddChildControl(envelopes);
  auto* amp = envelopes->AddPage();
  auto* pitch = envelopes->AddPage();
  pitch->AddChildControl(new KeithWorkspaceMaterial(PITCH_MATERIAL_FN));
  auto* filter = envelopes->AddPage();
  shape->AddChildControl(new KeithNavButton(IRECT(58,225,340,262), "VOLUME", envelopes,0));
  shape->AddChildControl(new KeithNavButton(IRECT(358,225,640,262), "PITCH", envelopes,1));
  shape->AddChildControl(new KeithNavButton(IRECT(658,225,940,262), "FILTER", envelopes,2));
  const IRECT plot(74, 318, 800, 658);
  amp->AddChildControl(new KeithEnvelopeControl(plot, kParamAmpAttack, kParamAmpDecay,
    kParamAmpSustain, kParamAmpRelease, kNoParameter, kGreen));
  pitch->AddChildControl(new KeithEnvelopeControl(plot, kParamPitchAttack, kParamPitchDecay,
    kParamPitchSustain, kParamPitchRelease, kParamPitchAmount, kMagenta));
  filter->AddChildControl(new KeithEnvelopeControl(plot, kParamFiltAttack, kParamFiltDecay,
    kParamFiltSustain, kParamFiltRelease, kNoParameter, kGreen));
  text(amp, IRECT(828,328,980,358), "AMPLITUDE", MonoBold(13,kPaper));
  text(amp, IRECT(828,370,980,400), "A / D / S / R", Mono(12,kPaper));
  text(amp, IRECT(828,418,980,450), "DRAG HANDLES", Mono(12,kPaper));
  text(amp, IRECT(828,454,980,486), "CLICK VALUES", Mono(12,kPaper));
  knob(pitch, kParamPitchAmount, "PITCH DEPTH", kMagenta, true);
  text(filter, IRECT(828,328,980,358), "FILTER CONTOUR", MonoBold(12,kPaper));
  text(filter, IRECT(828,388,980,418), "DEPTH AT RIGHT", Mono(12,kPaper));
  text(shape, IRECT(74,674,978,699), "TIME HANDLES / NORMALIZED SHAPE / CLICK A VALUE FOR PRECISE ENTRY", Mono(11,kPaper));

  // Tape follows its audible path, with large drive and mix at the two ends.
  text(tape, IRECT(62,228,974,268), "TAPE / GLOBAL BUS", MonoBold(24,kPaper));
  knob(tape, kParamTapeSat, "DRIVE", kMagenta, false);
  knob(tape, kParamTapeMix, "DRY / WET", kPaper, false);
  knob(tape, kParamTapeBump, "HEAD BUMP", kMagenta);
  knob(tape, kParamTapeWow, "WOW", kMagenta);
  knob(tape, kParamTapeFlutter, "FLUTTER", kMagenta);
  knob(tape, kParamTapeHiss, "HISS", kMagenta);
  knob(tape, kParamTapePrint, "PRINT", kMagenta);
  tabs(tape, IRECT(72,626,244,670), kParamTapeBypass, {"ACTIVE", "BYPASS"});
  text(tape, IRECT(788,626,982,670), "POST SYNTH", MonoBold(13,kPaper));

  text(library, IRECT(60,226,620,260), "PATCH LIBRARY", MonoBold(22,kPaper));
  text(library, IRECT(658,226,976,260), "CONTROL NOTES", MonoBold(16,kPaper));
  std::vector<KeithArchive::Row> rows;
  int count;
  const auto* presets = BakedPresets(count);
  for (int i=0;i<count;++i) rows.push_back({presets[i].name,i});
  library->AddChildControl(new KeithArchive(IRECT(58,284,620,686),std::move(rows)));
  library->AddChildControl(new KeithArchive(IRECT(644,284,982,686), {
    {"SOURCE"},{"Morph: sine through pulse."},{"Drag up to increase."},{"Ctrl-drag: fine control."},
    {"Double-click: default."},{"ENVELOPES"},{"Drag the four handles."},{"Click a value to type."},
    {"TAPE"},{"Dry/wet blends the bus."},{"MODULATION"},{"One LFO, CC1, velocity."},
    {"CONTENT"},{"15 original synth patches."},
    {"Factory ROMs not included."},{"Drum sampler not built."},{"Insert FX not built."},{"Mod matrix not built."}
  }));

  // The channel strip is always reachable while navigating source, shape or tape.
  auto* channel = new IContainerBase(IRECT(1030,204,1252,718));
  channel->SetIgnoreMouse(true);
  g->AttachControl(channel);
  text(channel, IRECT(1046,220,1240,249), "FILTER / VOICE", MonoBold(15,kPaper));
  tabs(channel, IRECT(1046,265,1240,298), kParamFilterType, {"LP24","LP12","BP","HP"});
  knob(channel, kParamFilterCutoff, "CUTOFF", kGreen, false);
  knob(channel, kParamFilterReso, "RESONANCE");
  knob(channel, kParamFilterEnv, "ENV DEPTH", kGreen, true);
  tabs(channel, IRECT(1046,568,1240,599), kParamEngine, {"HYBRID","ANALOG"});
  tabs(channel, IRECT(1046,606,1240,635), kParamPolyMode, {"POLY","MONO"});
  knob(channel, kParamPorta, "GLIDE", kPaper);
  knob(channel, kParamUnisonVoices, "UNISON", kPaper);
  knob(channel, kParamUnisonDetune, "DETUNE", kPaper);

  g->AttachControl(new KeithTextControl(IRECT(42,746,338,770), "MIDI IN / A-L PLAY", Mono(12,kPaper)));
  g->AttachControl(new KeithTextControl(IRECT(42,771,338,790), "Z/X OCTAVE", Mono(11,kPaper)));
  g->AttachControl(new IVKeyboardControl(IRECT(380,744,1248,789),36,84,false,
    kPaper,kInk,kMagenta,kPanelAlt,kGreen),kCtrlTagKeyboard);
  g->SetQwertyMidiKeyHandlerFunc([g](const IMidiMsg& msg) {
    auto* keys=g->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>();
    keys->SetNoteFromMidi(msg.NoteNumber(),msg.StatusMsg()==IMidiMsg::kNoteOn && msg.Velocity()>0);
  });
  envelopes->Select(0);
  pages->Select(0);
}
} }
