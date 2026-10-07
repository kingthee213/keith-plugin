#pragma once
#include "KeithControls.h"
#include "IPlugPluginBase.h"
#include "../synth/CoreSynth.h"
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace keith { namespace ui {

// The chassis is an original Cycles render; live controls stay native.
class KeithArtwork : public IControl
{
public:
  explicit KeithArtwork(const IRECT& r) : IControl(r) { mIgnoreMouse = true; }
  void OnInit() override { mChassis = GetUI()->LoadBitmap(CHASSIS_FN); }
  void Draw(IGraphics& g) override
  {
    g.DrawFittedBitmap(mChassis, mRECT);
    g.DrawText(Title(kWordmarkPx, kPaper), "YOTE MACHINE",
               IRECT(kWordmarkBox.l, kWordmarkBox.t, kWordmarkBox.r, kWordmarkBox.b));
    // One stamped service plate carries brand and provenance. It sits in the band
    // between the wordmark and the nav bar, which has room for the plate only:
    // stacking a separate subtitle above it clipped the subtitle's descenders.
    // Nav bar starts at y=144, so the plate must end above that.
    const IRECT plate(48, 114, 344, 142);
    g.FillRect(kMagenta.WithOpacity(0.92f), plate);
    g.DrawRect(kInk, plate, nullptr, 2.f);
    g.DrawText(MonoBold(11, kInk, EAlign::Near), "THE YOUTH HOSTEL DSP", plate.GetReducedFromLeft(10));
    g.DrawText(Mono(10, kInk, EAlign::Far), "FIVE SOURCE FAMILIES", plate.GetReducedFromRight(10));
    g.FillCircle(kInk, plate.L + 5.f, plate.MH(), 1.5f);
    g.FillCircle(kInk, plate.R - 5.f, plate.MH(), 1.5f);
  }
private:
  IBitmap mChassis;
};

class KeithPageDeck : public IContainerBase
{
public:
  explicit KeithPageDeck(const IRECT& r) : IContainerBase(r) { mIgnoreMouse = true; }
  IContainerBase* AddPage()
  {
    auto* page = new IContainerBase(mRECT);
    page->SetIgnoreMouse(true);
    AddChildControl(page);
    page->Hide(NChildren() - 1 != mSelected);
    return page;
  }
  int Selected() const { return mSelected; }
  void Select(int page)
  {
    if (page < 0 || page >= NChildren()) return;
    mSelected = page;
    Hide(IsHidden());
    GetUI()->SetAllControlsDirty();
  }
  void Hide(bool hidden) override
  {
    IControl::Hide(hidden);
    ForAllChildrenFunc([=](int i, IControl* c) { c->Hide(hidden || i != mSelected); });
  }
private:
  int mSelected = 0;
};

class KeithNavButton : public IControl
{
public:
  KeithNavButton(const IRECT& r, const char* label, KeithPageDeck* deck, int page, bool paper = false,
                 float textPad = 12.f)
    : IControl(r), mDeck(deck), mPage(page), mPaper(paper), mLabel(label), mTextPad(textPad) {}
  void Draw(IGraphics& g) override
  {
    const bool active = mDeck->Selected() == mPage;
    const IColor ground = kPanel;
    const IColor ink = kPaper;
    g.FillRect(active ? ink : ground, mRECT);
    g.DrawText(MonoBold(kNavFontPx, active ? ground : ink), mLabel.c_str(), mRECT.GetPadded(-mTextPad));
    if (active) g.FillRect(kMagenta, mRECT.GetFromLeft(4));
    else g.DrawLine(ink.WithOpacity(0.25f), mRECT.L, mRECT.B, mRECT.R, mRECT.B, nullptr, 1);
  }
  void OnMouseDown(float, float, const IMouseMod&) override { mDeck->Select(mPage); }
private:
  KeithPageDeck* mDeck;
  int mPage;
  bool mPaper;
  std::string mLabel;
  float mTextPad;
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
    g.FillTriangle(kMagenta, value.R - 13.f, value.MH() - 2.f,
                   value.R - 5.f, value.MH() - 2.f, value.R - 9.f, value.MH() + 3.f);
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

class KeithFxKnobControl : public KeithKnobControl
{
public:
  KeithFxKnobControl(const IRECT& bounds, int paramIdx, int algorithmParam, int knob,
                     bool master, const IColor& accent)
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
    SetLabel(FxKnobLabel(mMaster, algorithm, mKnob));
    KeithKnobControl::Draw(g);
  }

private:
  int mAlgorithmParam;
  int mKnob;
  bool mMaster;
};

class KeithWorkspaceMaterial : public IControl
{
public:
  explicit KeithWorkspaceMaterial(const char* resource)
    : IControl(IRECT(34, 204, 1004, 718)), mResource(resource) { mIgnoreMouse = true; }
  void OnInit() override { mBitmap = GetUI()->LoadBitmap(mResource); }
  void Draw(IGraphics& g) override { g.DrawFittedBitmap(mBitmap, mRECT); }
private:
  const char* mResource;
  IBitmap mBitmap;
};

class KeithWaveform : public IControl
{
public:
  KeithWaveform(const IRECT& r, int shape, int pw, IColor accent)
    : IControl(r, {shape, pw}), mAccent(accent) { mIgnoreMouse = true; }
  void Draw(IGraphics& g) override
  {
    synth::Oscillator osc;
    osc.SetSampleRate(48000);
    osc.Reset();
    const float shape = static_cast<float>(GetValue(0));
    const float pw = static_cast<float>(GetParam(1)->Value() / 100.);
    g.DrawLine(kPaper.WithOpacity(0.15f), mRECT.L, mRECT.MH(), mRECT.R, mRECT.MH(), nullptr, 1);
    for (int i = 0; i <= 320; ++i)
    {
      const float y = mRECT.MH() - osc.Process(300, shape, pw) * mRECT.H() * 0.36f;
      const float x = mRECT.L + mRECT.W() * i / 320.f;
      if (!i) g.PathMoveTo(x, y); else g.PathLineTo(x, y);
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
    for (int family = 1; family <= kNumFamilies; ++family)
    {
      auto* sub = new IPopupMenu;
      for (int i = 0; i < count; ++i)
      {
        if (presets[i].family != family) continue;
        const char* name = std::strstr(presets[i].name, " / ");
        const int flags = i == plugin->GetCurrentPresetIdx() ? IPopupMenu::Item::kChecked : 0;
        sub->AddItem(new IPopupMenu::Item(name ? name + 3 : presets[i].name, flags, i));
      }
      char label[24]; std::snprintf(label, sizeof(label), "Family %02d", family);
      mMenu.AddItem(label, sub);
    }
    GetUI()->CreatePopupMenu(*this, mMenu, mRECT);
  }
  void OnPopupMenuSelection(IPopupMenu* menu, int) override
  {
    if (!menu || !menu->GetChosenItem()) return;
    const int index = menu->GetChosenItem()->GetTag();
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    if (index >= 0 && index < plugin->NPresets()) plugin->RestorePreset(index);
    GetUI()->SetAllControlsDirty();
  }
private:
  IPopupMenu mMenu;
};

// Each archive owns its drawing and hit test. Scrolled-off rows cannot intercept
// another window's input; unlike translating independent controls past a parent.
class KeithArchive : public IControl
{
public:
  struct Row { std::string text; int preset = -1; };
  KeithArchive(const IRECT& r, std::vector<Row> rows) : IControl(r), mRows(std::move(rows)) {}
  void Draw(IGraphics& g) override
  {
    g.FillRect(kPanel, mRECT);
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    for (size_t i = 0; i < mRows.size(); ++i)
    {
      IRECT row(mRECT.L, mRECT.T + i * kRowHeight - mOffset, mRECT.R,
                mRECT.T + (i + 1) * kRowHeight - mOffset);
      if (!row.Intersects(mRECT)) continue;
      const bool active = mRows[i].preset >= 0 && mRows[i].preset == plugin->GetCurrentPresetIdx();
      if (active) g.FillRect(kGreen, row);
      g.DrawText(Mono(14, active ? kInk : kPaper), mRows[i].text.c_str(), row.GetReducedFromLeft(16));
      g.DrawLine(kPaper.WithOpacity(0.13f), row.L + 16, row.B, row.R - 16, row.B, nullptr, 1);
    }
  }
  void OnMouseWheel(float, float, const IMouseMod&, float d) override
  {
    mOffset = ScrollOffset(mOffset, d, static_cast<float>(mRows.size()) * kRowHeight, mRECT.H());
    SetDirty(false);
  }
  void OnMouseDown(float x, float y, const IMouseMod&) override
  {
    if (!mRECT.Contains(x, y)) return;
    const int row = static_cast<int>((y - mRECT.T + mOffset) / kRowHeight);
    if (row < 0 || row >= static_cast<int>(mRows.size()) || mRows[row].preset < 0) return;
    dynamic_cast<iplug::IPluginBase*>(GetDelegate())->RestorePreset(mRows[row].preset);
    GetUI()->SetAllControlsDirty();
  }
private:
  static constexpr float kRowHeight = 44.f;
  std::vector<Row> mRows;
  float mOffset = 0;
};

// Live magnitude of MultiFilter. Param ids are the stable host ids (cutoff 26, reso 27, type 25).
class KeithFilterResponse : public IControl
{
public:
  KeithFilterResponse(const IRECT& r)
  : IControl(r, {26, 27, 25})
  {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    const float cutoff = static_cast<float>(GetParam(0)->Value());
    const float reso = static_cast<float>(GetParam(1)->Value() / 100.);
    const int type = GetParam(2)->Int();
    if (cutoff != mCutoff || reso != mReso || type != mType)
      Rebuild(cutoff, reso, type);

    g.FillRect(kPanel, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, 2.f);
    g.DrawLine(kPaper.WithOpacity(0.12f), mRECT.L, mRECT.MH(), mRECT.R, mRECT.MH(), nullptr, 1.f);

    const float tCut = std::log(std::max(20.f, cutoff) / 20.f) / std::log(1000.f);
    const float xCut = mRECT.L + mRECT.W() * std::clamp(tCut, 0.f, 1.f);
    g.DrawLine(kPaper.WithOpacity(0.35f), xCut, mRECT.T + 6.f, xCut, mRECT.B - 6.f, nullptr, 1.f);

    for (int i = 0; i < kPoints; ++i)
    {
      const float x = mRECT.L + mRECT.W() * static_cast<float>(i) / static_cast<float>(kPoints - 1);
      const float y = mRECT.B - 10.f - mMag[i] * (mRECT.H() - 24.f);
      if (i == 0)
        g.PathMoveTo(x, y);
      else
        g.PathLineTo(x, y);
    }
    g.PathStroke(IPattern(kGreen), 2.f);
  }

private:
  static constexpr int kPoints = 48;
  static constexpr int kIR = 1024;

  void Rebuild(float cutoff, float reso, int type)
  {
    mCutoff = cutoff;
    mReso = reso;
    mType = type;

    synth::MultiFilter filter;
    filter.SetSampleRate(48000.);
    filter.Reset();
    const auto mode = static_cast<synth::FilterType>(std::clamp(type, 0, 3));
    float ir[kIR];
    ir[0] = filter.Process(1.f, cutoff, reso, mode);
    for (int i = 1; i < kIR; ++i)
      ir[i] = filter.Process(0.f, cutoff, reso, mode);

    float peak = 1.0e-6f;
    for (int i = 0; i < kPoints; ++i)
    {
      const float t = static_cast<float>(i) / static_cast<float>(kPoints - 1);
      const float freq = 20.f * std::pow(1000.f, t);
      mMag[i] = Magnitude(ir, freq);
      peak = std::max(peak, mMag[i]);
    }
    for (int i = 0; i < kPoints; ++i)
      mMag[i] = std::min(1.f, mMag[i] / peak);
  }

  static float Magnitude(const float* ir, float freq)
  {
    const float w = 2.f * static_cast<float>(synth::detail::kPi) * freq / 48000.f;
    const float coeff = 2.f * std::cos(w);
    float s1 = 0.f;
    float s2 = 0.f;
    for (int i = 0; i < kIR; ++i)
    {
      const float s0 = ir[i] + coeff * s1 - s2;
      s2 = s1;
      s1 = s0;
    }
    const float power = s1 * s1 + s2 * s2 - coeff * s1 * s2;
    return power > 0.f ? std::sqrt(power) : 0.f;
  }

  float mMag[kPoints] {};
  float mCutoff = -1.f;
  float mReso = -1.f;
  int mType = -1;
};
} }
