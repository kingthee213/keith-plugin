#pragma once
#include "KeithControls.h"
#include "IPlugPluginBase.h"
#include "../synth/CoreSynth.h"
#include <functional>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

namespace keith { namespace ui {

// Painted chassis from tools/render_faces.py; live controls stay native on top.
class KeithArtwork : public IControl
{
public:
  explicit KeithArtwork(const IRECT& r) : IControl(r) { mIgnoreMouse = true; }
  void OnInit() override { mChassis = GetUI()->LoadBitmap(CHASSIS_FN); }
  void Draw(IGraphics& g) override
  {
    g.DrawFittedBitmap(mChassis, mRECT);
  }

private:
  IBitmap mChassis;
};

class KeithParamMenu : public IControl
{
public:
  KeithParamMenu(const IRECT& r, int paramIdx, const char* label,
                 std::initializer_list<const char*> options)
    : IControl(r, paramIdx), mLabel(label)
  {
    for (const char* option : options)
      mOptions.emplace_back(option);
  }

  void Draw(IGraphics& g) override
  {
    const int selected = SelectedIndex();
    const IRECT value(mRECT.L, mRECT.T + 16.f, mRECT.R, mRECT.B);
    g.DrawText(Mono(9, kPaper, EAlign::Near), mLabel.c_str(),
               IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + 14.f));
    g.FillRect(kPanelAlt, value);
    g.DrawRect(kLine, value, nullptr, 1.f);
    g.DrawText(MonoBold(10, kPaper, EAlign::Near), mOptions[static_cast<size_t>(selected)].c_str(),
               value.GetPadded(-7.f).GetReducedFromRight(16.f));
    g.FillTriangle(kHotPink, value.R - 13.f, value.MH() - 2.f, value.R - 5.f, value.MH() - 2.f,
                   value.R - 9.f, value.MH() + 3.f);
  }

  void OnMouseDown(float, float, const IMouseMod&) override
  {
    const int selected = SelectedIndex();
    mMenu.Clear();
    for (int i = 0; i < static_cast<int>(mOptions.size()); ++i)
    {
      const int flags = i == selected ? IPopupMenu::Item::kChecked : 0;
      mMenu.AddItem(new IPopupMenu::Item(mOptions[static_cast<size_t>(i)].c_str(), flags, i));
    }
    GetUI()->CreatePopupMenu(*this, mMenu, mRECT);
  }

  void OnPopupMenuSelection(IPopupMenu* menu, int) override
  {
    if (!menu || !menu->GetChosenItem() || mOptions.size() < 2)
      return;
    const int selected = menu->GetChosenItem()->GetTag();
    if (selected < 0 || selected >= static_cast<int>(mOptions.size()))
      return;
    SetValueFromUserInput(static_cast<double>(selected) /
                          static_cast<double>(mOptions.size() - 1), 0);
    GetUI()->SetAllControlsDirty();
  }

private:
  int SelectedIndex() const
  {
    const IParam* param = GetParam();
    return param ? std::clamp(param->Int(), 0, static_cast<int>(mOptions.size()) - 1) : 0;
  }

  std::string mLabel;
  std::vector<std::string> mOptions;
  IPopupMenu mMenu;
};

inline const char* FxKnobLabel(bool master, int algorithm, int knob)
{
  static const char* insertLabels[5][3] = {
    {"NOT USED", "NOT USED", "NOT USED"},
    {"DRIVE", "TONE", "BIAS"},
    {"RATE", "DEPTH", "SPREAD"},
    {"TIME", "FEEDBACK", "TONE"},
    {"BITS", "RATE", "JITTER"}
  };
  static const char* masterLabels[5][3] = {
    {"NOT USED", "NOT USED", "NOT USED"},
    {"THRESHOLD", "RATIO", "RELEASE"},
    {"CEILING", "RELEASE", "INPUT"},
    {"WIDTH", "MONO FREQ", "BALANCE"},
    {"TILT", "PIVOT", "DRIVE"}
  };
  const int safeAlgorithm = std::clamp(algorithm, 0, 4);
  const int safeKnob = std::clamp(knob, 0, 2);
  return master ? masterLabels[safeAlgorithm][safeKnob]
                : insertLabels[safeAlgorithm][safeKnob];
}

class KeithFxAlgoMenu : public IControl
{
public:
  KeithFxAlgoMenu(const IRECT& r, int algoParam, int bypassParam, const char* label,
                  std::initializer_list<const char*> options)
  : IControl(r, algoParam)
  , mBypassParam(bypassParam)
  , mLabel(label)
  {
    for (const char* option : options)
      mOptions.emplace_back(option);
  }

  void Draw(IGraphics& g) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    const int selected = SelectedIndex();
    const IRECT menu = MenuBounds();
    const IRECT bypass = BypassBounds();
    g.DrawText(Mono(9, kPaper, EAlign::Near), mLabel.c_str(),
                IRECT(mRECT.L, mRECT.T, mRECT.R, mRECT.T + 14.f));
    g.FillRect(kPanelAlt, menu);
    g.DrawRect(kLine, menu, nullptr, 1.f);
    g.DrawText(MonoBold(10, kPaper, EAlign::Near), mOptions[static_cast<size_t>(selected)].c_str(),
                menu.GetPadded(-7.f).GetReducedFromRight(16.f));
    g.FillTriangle(kHotPink, menu.R - 13.f, menu.MH() - 2.f, menu.R - 5.f, menu.MH() - 2.f,
                    menu.R - 9.f, menu.MH() + 3.f);

    const IParam* bypassParam = plugin ? plugin->GetParam(mBypassParam) : nullptr;
    const bool bypassed = bypassParam && bypassParam->Int() > 0;
    g.FillRect(bypassed ? kMuteRed.WithOpacity(0.65f) : kGreen, bypass);
    g.DrawRect(kLine, bypass, nullptr, 1.f);
    g.DrawText(MonoBold(9, kInk, EAlign::Center), bypassed ? "BYP" : "ACT", bypass);
  }

  void OnMouseDown(float x, float y, const IMouseMod&) override
  {
    if (BypassBounds().Contains(x, y))
    {
      auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
      if (!plugin)
        return;
      IParam* bypass = plugin->GetParam(mBypassParam);
      if (!bypass)
        return;
      const int bypassed = bypass->Int();
      bypass->SetNormalized(bypassed > 0 ? 0.0 : 1.0);
      SetDirty(false);
      return;
    }

    const int selected = SelectedIndex();
    mMenu.Clear();
    for (int i = 0; i < static_cast<int>(mOptions.size()); ++i)
    {
      const int flags = i == selected ? IPopupMenu::Item::kChecked : 0;
      mMenu.AddItem(new IPopupMenu::Item(mOptions[static_cast<size_t>(i)].c_str(), flags, i));
    }
    GetUI()->CreatePopupMenu(*this, mMenu, MenuBounds());
  }

  void OnPopupMenuSelection(IPopupMenu* menu, int) override
  {
    if (!menu || !menu->GetChosenItem() || mOptions.size() < 2)
      return;
    const int selected = menu->GetChosenItem()->GetTag();
    if (selected < 0 || selected >= static_cast<int>(mOptions.size()))
      return;
    SetValueFromUserInput(static_cast<double>(selected) /
                          static_cast<double>(mOptions.size() - 1), 0);
    GetUI()->SetAllControlsDirty();
  }

private:
  int SelectedIndex() const
  {
    const IParam* param = GetParam();
    return param ? std::clamp(param->Int(), 0, static_cast<int>(mOptions.size()) - 1) : 0;
  }

  IRECT MenuBounds() const
  {
    IRECT menu = mRECT.GetReducedFromTop(16.f);
    menu.R = mRECT.L + mRECT.W() * 0.72f;
    return menu;
  }

  IRECT BypassBounds() const
  {
    const IRECT menu = MenuBounds();
    return IRECT(menu.R + 4.f, menu.T, mRECT.R, menu.B);
  }

  int mBypassParam;
  std::string mLabel;
  std::vector<std::string> mOptions;
  IPopupMenu mMenu;
};

class KeithFxKnobControl : public KeithKnobControl
{
public:
  KeithFxKnobControl(const IRECT& bounds, int paramIdx, int algorithmParam, int knob, bool master,
                     const IColor& accent)
  : KeithKnobControl(bounds, paramIdx, "NOT USED", accent, true, false, 4.0, 24.f, true)
  , mAlgorithmParam(algorithmParam)
  , mKnob(knob)
  , mMaster(master)
  {
  }

  void Draw(IGraphics& g) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    const int algorithm = plugin ? plugin->GetParam(mAlgorithmParam)->Int() : 0;
    if (algorithm <= 0)
      return;
    const char* algoLabel = FxKnobLabel(mMaster, algorithm, mKnob);
    if (std::strcmp(algoLabel, "NOT USED") == 0)
      return;
    SetLabel(ControlFor(GetParamIdx()).label);
    KeithKnobControl::Draw(g);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    const int algorithm = plugin ? plugin->GetParam(mAlgorithmParam)->Int() : 0;
    const char* label = FxKnobLabel(mMaster, algorithm, mKnob);
    if (algorithm <= 0 || std::strcmp(label, "NOT USED") == 0)
      return;
    KeithKnobControl::OnMouseDown(x, y, mod);
  }

private:
  int mAlgorithmParam;
  int mKnob;
  bool mMaster;
};

class KeithFxMixKnobControl : public KeithKnobControl
{
public:
  KeithFxMixKnobControl(const IRECT& bounds, int paramIdx, int algorithmParam, const char* label,
                        const IColor& accent)
  : KeithKnobControl(bounds, paramIdx, label, accent, true, false, 4.0, 24.f, true)
  , mAlgorithmParam(algorithmParam)
  {
  }

  void Draw(IGraphics& g) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    const int algorithm = plugin ? plugin->GetParam(mAlgorithmParam)->Int() : 0;
    if (algorithm <= 0)
      return;
    KeithKnobControl::Draw(g);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    const int algorithm = plugin ? plugin->GetParam(mAlgorithmParam)->Int() : 0;
    if (algorithm <= 0)
      return;
    KeithKnobControl::OnMouseDown(x, y, mod);
  }

private:
  int mAlgorithmParam;
};

class KeithWaveform : public IControl
{
public:
  KeithWaveform(const IRECT& r, int shape, int pw, IColor accent)
    : IControl(r, {shape, pw}), mAccent(accent)
  {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    synth::Oscillator osc;
    osc.SetSampleRate(48000);
    osc.Reset();
    // GetParam returns null if the index is out of range. This control is attached twice and
    // draws on every repaint, so an unguarded deref here takes the host down rather than
    // just mis-drawing.
    const IParam* pwParam = GetParam(1);
    if (!pwParam)
      return;
    const float shape = static_cast<float>(GetValue(0));
    const float pw = static_cast<float>(pwParam->Value() / 100.);
    g.FillRect(kInk, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, 1.f);
    g.DrawLine(kPaper.WithOpacity(0.15f), mRECT.L, mRECT.MH(), mRECT.R, mRECT.MH(), nullptr, 1);
    for (int i = 0; i <= 320; ++i)
    {
      const float y = mRECT.MH() - osc.Process(300, shape, pw) * mRECT.H() * 0.36f;
      const float x = mRECT.L + mRECT.W() * i / 320.f;
      if (!i)
        g.PathMoveTo(x, y);
      else
        g.PathLineTo(x, y);
    }
    g.PathStroke(IPattern(mAccent), 2.5f);
  }

private:
  IColor mAccent;
};

class KeithPresetMenu : public IControl
{
public:
  explicit KeithPresetMenu(const IRECT& r) : IControl(r) {}
  void Draw(IGraphics& g) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    g.FillRect(kPaper, mRECT);
    g.DrawText(MonoBold(15, kInk), plugin->GetPresetName(plugin->GetCurrentPresetIdx()),
               mRECT.GetReducedFromRight(38).GetReducedFromLeft(16));
    g.FillTriangle(kInk, mRECT.R - 26, mRECT.MH() - 3, mRECT.R - 14, mRECT.MH() - 3,
                   mRECT.R - 20, mRECT.MH() + 4);
  }
  void OnMouseDown(float, float, const IMouseMod&) override
  {
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    mMenu.Clear();
    int count = 0;
    const auto* presets = BakedPresets(count);

    // Grouped by kind rather than by family.
    //
    // The family grouping was how the original 109 were organised: five emulated machines, each
    // with its own set. The 160-sound library is organised by what a sound IS -- polys, leads,
    // pads, strings, FX, reeds, brass -- because that is how a player looks for one. Someone
    // wanting a pad does not care which machine it came from.
    //
    // The order is the brief's order, so the menu reads the way the library was specified.
    // Anything unlisted lands under its own heading rather than being dropped, so a new category
    // cannot silently vanish from the menu.
    static const char* kKindOrder[] = {
      // The owner's eight, in the order the menu lists them. Drums is deliberately absent: a drum
      // kit is chosen with the Drum Kit parameter, not from the preset list.
      "mono synths", "poly synths", "guitars", "winds", "sfx", "vocal", "bass", "strings",
    };

    // The category IS the group now.
    //
    // The owner asked for the whole library grouped by role -- polysynth, lead, keys, bass, drums,
    // reeds/horn, pad, strings/fx -- and for the families to go. So src/Keith.cpp stores those
    // eight names directly in the category field and this mapping has nothing left to do. It
    // stays, rather than being deleted, only to catch a category that is not one of the eight:
    // anything unrecognised keeps its own heading, which is visible in the menu, instead of being
    // folded into a group it does not belong to.
    //
    // An earlier version derived a kind from the leading word ("poly keys" -> "poly", "key organ"
    // -> "keys"). That was necessary while categories carried a sub-type. It is not any more, and
    // leaving it in would silently mis-group anything whose first word happened to collide.
    auto kindOf = [](const char* category) -> std::string {
      const std::string cat = category ? category : "";
      if (cat.empty())
        return "other";
      for (const char* kind : kKindOrder)
        if (cat == kind)
          return cat;
      // Not one of the eight. Return it as-is so the menu shows the unexpected heading rather
      // than dropping the preset or hiding it inside a group.
      return cat;
    };

    // Only kinds that actually have presets get a heading, so an empty "Reeds" does not appear
    // in a build that predates those patches.
    std::vector<std::string> present;
    for (int i = 0; i < count; ++i)
    {
      const std::string k = kindOf(presets[i].category);
      if (std::find(present.begin(), present.end(), k) == present.end())
        present.push_back(k);
    }
    std::vector<std::string> ordered;
    for (const char* kind : kKindOrder)
      if (std::find(present.begin(), present.end(), kind) != present.end())
        ordered.push_back(kind);
    for (const auto& k : present)
      if (std::find(ordered.begin(), ordered.end(), k) == ordered.end())
        ordered.push_back(k);

    for (const auto& kind : ordered)
    {
      auto* sub = new IPopupMenu;
      int inKind = 0;
      for (int i = 0; i < count; ++i)
      {
        if (kindOf(presets[i].category) != kind)
          continue;
        const char* name = std::strstr(presets[i].name, " / ");
        const int flags = i == plugin->GetCurrentPresetIdx() ? IPopupMenu::Item::kChecked : 0;
        sub->AddItem(new IPopupMenu::Item(name ? name + 3 : presets[i].name, flags, i));
        ++inKind;
      }
      if (inKind == 0)
      {
        delete sub;
        continue;
      }
      // The count in the heading, so a kind's size is visible without opening it.
      char label[64];
      std::snprintf(label, sizeof(label), "%s (%d)", kind.c_str(), inKind);
      mMenu.AddItem(label, sub);
    }

    GetUI()->CreatePopupMenu(*this, mMenu, mRECT);
  }
  void OnPopupMenuSelection(IPopupMenu* menu, int) override
  {
    if (!menu || !menu->GetChosenItem())
      return;
    const int index = menu->GetChosenItem()->GetTag();
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    if (index >= 0 && index < plugin->NPresets())
      plugin->RestorePreset(index);
    GetUI()->SetAllControlsDirty();
  }

private:
  IPopupMenu mMenu;
};

} }
