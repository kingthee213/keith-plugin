#pragma once

// Keith editor controls: physical instrument framing, knobs, shape readout, envelope editor, pad grid.
//
// Everything here binds 1:1 to host parameters (docs/PARAM-MANIFEST.md). No control in this file
// draws a fake or inert parameter, and no Draw() implementation allocates: text is formatted into
// fixed stack buffers and the envelope maths is pure arithmetic. UI strings are ASCII on purpose -
// the iPlug2 CMake project does not compile with /utf-8.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

#include "IControl.h"

#include "KeithTheme.h"
#include "EditorGeometry.h"

using namespace iplug;
using namespace igraphics;

namespace keith {
namespace ui {

// ---------------------------------------------------------------------------
// Panel frame + title strip. Children are placed by the layout inside Content().
// ---------------------------------------------------------------------------
class KeithPanelControl : public IControl
{
public:
  static constexpr float kTitleHeight = 26.f;

  KeithPanelControl(const IRECT& bounds, const char* title, const IColor& wash,
                    const IColor& titleFill, bool muted = false, bool shadow = true)
  : IControl(bounds)
  , mWash(wash)
  , mTitleFill(titleFill)
  , mMuted(muted)
  , mShadow(shadow)
  {
    mTitle.Set(title);
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    DrawPanelFrame(g, mRECT, mWash, kOutline, mShadow, mTitleFill);

    if (mTitle.GetLength())
      DrawTitleStrip(g, mRECT, mTitle.Get(), mTitleFill, kInk, kTitleHeight, 19.f, mMuted);
  }

  /** Inner area for child controls, below the title strip. */
  IRECT Content(float pad = 8.f) const
  {
    IRECT r = mRECT;
    if (mTitle.GetLength())
      r.ReduceFromTop(kOutline + kTitleHeight + pad);
    else
      r.ReduceFromTop(kOutline + pad);

    r.ReduceFromLeft(kOutline + pad);
    r.ReduceFromRight(kOutline + pad);
    r.ReduceFromBottom(kOutline + pad);
    return r;
  }

  /** The title strip area, for controls that sit in the header (e.g. the filter type tabs). */
  IRECT TitleArea() const
  {
    return IRECT(mRECT.L + kOutline, mRECT.T + kOutline, mRECT.R - kOutline,
                 mRECT.T + kOutline + kTitleHeight);
  }

private:
  IColor mWash;
  IColor mTitleFill;
  WDL_String mTitle;
  bool mMuted;
  bool mShadow;
};

// ---------------------------------------------------------------------------
// Static text (wordmark, hints, status). Alignment comes from the IText.
// ---------------------------------------------------------------------------
class KeithTextControl : public IControl
{
public:
  KeithTextControl(const IRECT& bounds, const char* str, const IText& text, bool outlined = false,
                   const IColor& outline = kLine, float outlineThickness = 2.f)
  : IControl(bounds)
  , mText(text)
  , mOutline(outline)
  , mOutlined(outlined)
  , mOutlineThickness(outlineThickness)
  {
    mStr.Set(str);
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    if (mOutlined)
      DrawOutlinedText(g, mText, mStr.Get(), mRECT, mOutline, mOutlineThickness);
    else
      g.DrawText(mText, mStr.Get(), mRECT);
  }

private:
  IText mText;
  IColor mOutline;
  WDL_String mStr;
  bool mOutlined;
  float mOutlineThickness;
};

// ---------------------------------------------------------------------------
// Small hard badge, used for editorial state tags.
// ---------------------------------------------------------------------------
class KeithBadgeControl : public IControl
{
public:
  KeithBadgeControl(const IRECT& bounds, const char* label, const IColor& fill,
                    const IColor& ink = kInk, float size = 11.f)
  : IControl(bounds)
  , mFill(fill)
  , mInk(ink)
  , mSize(size)
  {
    mLabel.Set(label);
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    g.FillRect(mFill, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, kInnerOutline);
    g.DrawText(Mono(mSize, mInk, EAlign::Center), mLabel.Get(), mRECT);
  }

private:
  IColor mFill;
  IColor mInk;
  WDL_String mLabel;
  float mSize;
};

// ---------------------------------------------------------------------------
// Knob: hard circle, 270 deg track, thick neon tick (docs/UI-DESIGN.md §4.3).
// ---------------------------------------------------------------------------
class KeithKnobControl : public IKnobControlBase
{
public:
  static constexpr float kLabelHeight = 19.f;
  static constexpr float kValueHeight = 20.f;
  static constexpr float kAngle1 = -135.f;
  static constexpr float kAngle2 = 135.f;
  /** Cap so wide cells never balloon the dial past the design language (34px at design size). */
  static constexpr float kMaxBodyRadius = 24.f;

  KeithKnobControl(const IRECT& bounds, int paramIdx, const char* label, const IColor& accent,
                   bool showValue = true, bool bipolar = false, double gearing = 4.0, float radius = 24.f,
                   bool nativeBody = false)
  : IKnobControlBase(bounds, paramIdx, EDirection::Vertical, gearing)
  , mAccent(accent)
  , mShowValue(showValue)
  , mBipolar(bipolar)
  , mMaxRadius(radius)
  , mNativeBody(nativeBody)
  {
    DisablePrompt(false);
    SetText(Mono(12.f, kPaper, EAlign::Center));
    mLabel.Set(label);
  }

  void OnInit() override { OnResize(); }

  void OnResize() override
  {
    IRECT r = mRECT;
    mValueBounds = IRECT();
    if (mShowValue)
    {
      mValueBounds = r.GetFromBottom(kValueHeight);
      r.ReduceFromBottom(kValueHeight);
    }
    mLabelBounds = r.GetFromBottom(kLabelHeight);
    r.ReduceFromBottom(kLabelHeight);
    mWidgetBounds = r;
    SetTargetRECT(mRECT);
    SetDirty(false);
  }

  void SetDirty(bool push, int valIdx = kNoValIdx) override
  {
    IKnobControlBase::SetDirty(push, valIdx);
    UpdateValueStr();
  }

  void Draw(IGraphics& g) override
  {
    const float cx = mWidgetBounds.MW();
    const float cy = mWidgetBounds.MH();
    const float r = std::min(mWidgetBounds.W(), mWidgetBounds.H()) * 0.5f;
    const float body = std::min(r * 0.80f, mMaxRadius);

    if (mNativeBody)
    {
      g.FillCircle(kPanelAlt, cx, cy, body);
      g.DrawCircle(kLine, cx, cy, body, nullptr, 2.f);
    }

    const float value = static_cast<float>(std::clamp(GetValue(), 0., 1.));
    const float angle = kAngle1 + value * (kAngle2 - kAngle1);
    const float arcFrom = mBipolar ? 0.f : kAngle1;
    g.DrawArc(kPaper.WithOpacity(0.22f), cx, cy, r, kAngle1, kAngle2, nullptr, 1.f);
    if (std::fabs(angle - arcFrom) > 0.5f)
      g.DrawArc(mAccent, cx, cy, r, std::min(arcFrom, angle), std::max(arcFrom, angle), nullptr, 1.5f);
    // Physical knob and its shadow are baked in place; only the pointer moves.
    const IColor tick = mMouseIsOver ? mAccent : kPaper;
    g.DrawRadialLine(kInk, cx+0.5f, cy+0.5f, angle, body*0.49f, body*0.81f, nullptr, 3.f);
    g.DrawRadialLine(tick, cx, cy, angle, body*0.49f, body*0.81f, nullptr, 2.f);

    if (mLabelBounds.H() > 0.f)
      g.DrawText(Label(13.f, kPaper), mLabel.Get(), mLabelBounds);

    if (mValueBounds.H() > 0.f)
      g.DrawText(Value(mMaxRadius >= 38.f ? 18.f : 13.f, mMouseDown ? kCyan : kPaper), mValueStr.Get(), mValueBounds);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (mShowValue && mValueBounds.Contains(x, y))
      PromptUserInput(mValueBounds);
    else
      IKnobControlBase::OnMouseDown(x, y, mod);
  }

  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override { SetValueToDefault(); }

  /** The current display string, e.g. "25.0 %" or "LP24". Filled outside Draw. */
  const char* GetValueStr() const { return mValueStr.Get(); }
  void SetLabel(const char* label) { mLabel.Set(label); }

protected:
  IRECT GetKnobDragBounds() override { return mWidgetBounds; }

private:
  void UpdateValueStr()
  {
    const IParam* pParam = GetParam();
    if (!pParam)
      return;

    pParam->GetDisplay(pParam->Value(), false, mValueStr, true);

    // Enum/display-text params already read as their option name; numeric params get their unit.
    if (pParam->Type() == IParam::kTypeDouble && CStringHasContents(pParam->GetLabel()))
    {
      mValueStr.Append(" ");
      mValueStr.Append(pParam->GetLabel());
    }
  }

  IColor mAccent;
  WDL_String mLabel;
  WDL_String mValueStr;
  IRECT mWidgetBounds;
  IRECT mLabelBounds;
  IRECT mValueBounds;
  bool mShowValue;
  bool mBipolar;
  float mMaxRadius;
  bool mNativeBody;
};

// ---------------------------------------------------------------------------
// Oscillator shape readout. The DSP morph is sine -> triangle -> saw -> pulse
// (src/synth/CoreSynth.h), so the text names the two stages the value sits between.
// ---------------------------------------------------------------------------
class KeithShapeReadoutControl : public IControl
{
public:
  KeithShapeReadoutControl(const IRECT& bounds, int shapeParamIdx, const IText& text)
  : IControl(bounds, shapeParamIdx)
  , mText(text)
  {
    mIgnoreMouse = true;
  }

  void Draw(IGraphics& g) override
  {
    const IParam* pParam = GetParam();
    if (!pParam)
      return;

    static const char* kStages[4] = {"SINE", "TRI", "SAW", "PULSE"};

    const double shape = std::min(std::max(pParam->Value() / 100.0, 0.0), 1.0);
    const int idx = std::min(static_cast<int>(shape * 3.0), 2);
    const double local = (shape - idx / 3.0) * 3.0;

    char buf[32];

    if (local <= 0.02)
      std::snprintf(buf, sizeof(buf), "%s", kStages[idx]);
    else if (local >= 0.98)
      std::snprintf(buf, sizeof(buf), "%s", kStages[idx + 1]);
    else
      std::snprintf(buf, sizeof(buf), "%s>%s", kStages[idx], kStages[idx + 1]);

    g.DrawText(mText, buf, mRECT);
  }

private:
  IText mText;
};

// ---------------------------------------------------------------------------
// Envelope editor: draggable A/D/S/R handles over a real curve. Bipolar mode
// (pitch envelope) scales and flips the curve with the amount parameter.
// ---------------------------------------------------------------------------
class KeithEnvelopeControl : public IControl
{
public:
  static constexpr float kReadoutHeight = 46.f;
  static constexpr float kPad = 4.f;

  /** @param attack/decay/sustain/release parameter indices, in that value order
   *  @param amountParam amount index for a bipolar envelope, or kNoParameter */
  KeithEnvelopeControl(const IRECT& bounds, int attack, int decay, int sustain, int release,
                       int amountParam, const IColor& accent)
  : IControl(bounds, amountParam > kNoParameter
                        ? std::initializer_list<int>{attack, decay, sustain, release, amountParam}
                        : std::initializer_list<int>{attack, decay, sustain, release})
  , mAccent(accent)
  , mAmountIdx(amountParam)
  {
    DisablePrompt(false);
    SetText(Mono(12.f, kPaper, EAlign::Center));
  }

  void Draw(IGraphics& g) override
  {
    const IRECT plot = PlotBounds();
    g.FillRect(PlotGround(), plot);
    g.DrawRect(kLine, plot, nullptr, kInnerOutline);

    const Geometry geo = Measure(plot);

    // Flat grid: sustain level and segment boundaries.
    g.DrawLine(kPanelAlt, plot.L, geo.ySustain, plot.R, geo.ySustain, nullptr, 1.f);
    g.DrawLine(kPanelAlt, geo.xAttack, plot.T, geo.xAttack, plot.B, nullptr, 1.f);
    g.DrawLine(kPanelAlt, geo.xDecay, plot.T, geo.xDecay, plot.B, nullptr, 1.f);
    g.DrawLine(kPanelAlt, geo.xSustain, plot.T, geo.xSustain, plot.B, nullptr, 1.f);

    if (mAmountIdx > kNoParameter)
      g.DrawLine(kCyan.WithOpacity(0.55f), plot.L, plot.MH(), plot.R, plot.MH(), nullptr, 1.f);

    // Curve: ink outline pass then the neon pass.
    DrawCurve(g, geo, kLine, 4.f);
    DrawCurve(g, geo, mAccent, 2.f);

    DrawHandle(g, geo.xAttack, geo.yPeak);
    DrawHandle(g, geo.xDecay, geo.ySustain);
    DrawHandle(g, geo.xSustain, geo.ySustain);
    DrawHandle(g, geo.xRelease, geo.yEnd);

    DrawReadouts(g, plot);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (y >= PlotBounds().B)
    {
      const int row = y >= PlotBounds().B + kReadoutHeight * 0.5f ? 1 : 0;
      const int col = x >= mRECT.MW() ? 1 : 0;
      const IRECT values(mRECT.L, PlotBounds().B, mRECT.R, mRECT.B);
      PromptUserInput(ColCell(RowCell(values, row, 2), col, 2), row * 2 + col);
      mDrag = kNone;
    }
    else mDrag = HitTest(x, y);
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    if (mDrag < 0)
      return;

    if (mDrag == kSustain)
      SetValueFromUserInput(SolveSustain(y), 2);
    else
      SetValueFromUserInput(SolveSegment(mDrag, x), mDrag);
  }

  void OnMouseUp(float x, float y, const IMouseMod& mod) override { mDrag = kNone; }

  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override
  {
    const int handle = HitTest(x, y);
    if (handle >= 0 && handle <= 3)
      SetValueToDefault(handle);
  }

private:
  enum EHandle
  {
    kAttack = 0,
    kDecay = 1,
    kSustain = 2,
    kRelease = 3,
    kNone = -1
  };

  struct Geometry
  {
    float xAttack, xDecay, xSustain, xRelease;
    float yPeak, ySustain, yEnd;
  };

  static IColor PlotGround() { return IColor(255, 0x08, 0x08, 0x0E); }

  IRECT PlotBounds() const { return mRECT.GetReducedFromBottom(kReadoutHeight); }

  float Span(const IRECT& plot) const { return plot.H() * 0.5f - kPad - 1.f; }

  // Display the normalized contour so sustain remains editable at zero depth.
  // The separate amount parameter controls actual pitch modulation in the DSP.
  float Amplitude(const IRECT& plot) const { return Span(plot); }

  float CurveSign() const
  {
    if (mAmountIdx <= kNoParameter)
      return 1.f;

    const IParam* pParam = GetParam(4);
    return (pParam && pParam->Value() < 0.0) ? -1.f : 1.f;
  }

  Geometry Measure(const IRECT& plot) const
  {
    Geometry geo;
    geo.xAttack = plot.L + plot.W() * EnvelopeTimeX(0, GetValue(0));
    geo.xDecay = plot.L + plot.W() * EnvelopeTimeX(1, GetValue(1));
    geo.xSustain = plot.L + plot.W() * 0.61f;
    geo.xRelease = plot.L + plot.W() * EnvelopeTimeX(3, GetValue(3));

    const double sustain = std::min(std::max(GetValue(2), 0.), 1.);

    if (mAmountIdx > kNoParameter)
    {
      const float amp = Amplitude(plot);
      geo.yPeak = plot.MH() - CurveSign() * amp;
      geo.ySustain = plot.MH() - CurveSign() * amp * static_cast<float>(sustain);
      geo.yEnd = plot.MH();
    }
    else
    {
      geo.yPeak = plot.T + kPad;
      geo.ySustain = plot.B - kPad - static_cast<float>(sustain) * (plot.H() - 2.f * kPad);
      geo.yEnd = plot.B - kPad;
    }

    return geo;
  }

  void DrawCurve(IGraphics& g, const Geometry& geo, const IColor& colour, float thickness) const
  {
    IPattern pattern(colour);
    IStrokeOptions options;
    options.mJoinOption = ELineJoin::Miter;
    options.mCapOption = ELineCap::Butt;

    g.PathMoveTo(geo.xRelease, geo.yEnd);
    g.PathLineTo(geo.xSustain, geo.ySustain);
    g.PathLineTo(geo.xDecay, geo.ySustain);
    g.PathLineTo(geo.xAttack, geo.yPeak);
    g.PathLineTo(PlotBounds().L, geo.yEnd);
    g.PathStroke(pattern, thickness, options);
  }

  void DrawHandle(IGraphics& g, float x, float y) const
  {
    const float r = 5.5f;
    g.FillCircle(kLine, x, y, r + 1.5f);
    g.FillCircle(mAccent, x, y, r - 0.5f);
  }

  void DrawReadouts(IGraphics& g, const IRECT& plot) const
  {
    const IParam* pA = GetParam(0);
    const IParam* pD = GetParam(1);
    const IParam* pS = GetParam(2);
    const IParam* pR = GetParam(3);

    if (!pA || !pD || !pS || !pR)
      return;

    const char* names[] = {"A", "D", "S", "R"};
    const IParam* params[] = {pA, pD, pS, pR};
    for (int i = 0; i < 4; ++i)
    {
      char value[24], line[36];
      if (i == 2) FormatPercent(params[i]->Value(), value, sizeof(value));
      else FormatMs(params[i]->Value(), value, sizeof(value));
      std::snprintf(line, sizeof(line), "%s %s", names[i], value);
      const IRECT readouts(plot.L, plot.B, plot.R, mRECT.B);
      const IRECT cell = ColCell(RowCell(readouts, i / 2, 2), i % 2, 2);
      g.DrawText(Mono(13.f, kPaper), line, cell);
    }
  }

  double SolveSustain(float y) const
  {
    const IRECT plot = PlotBounds();

    if (mAmountIdx <= kNoParameter)
      return std::min(std::max((plot.B - kPad - y) / (plot.H() - 2.0 * kPad), 0.0), 1.0);

    const float amp = Amplitude(plot);
    if (amp < 1.f)
      return GetValue(2); // curve is flat at this amount; leave the level alone

    const double level = (plot.MH() - y) / (CurveSign() * amp);
    return std::min(std::max(level, 0.0), 1.0);
  }

  float SolveSegment(int segment, float targetX) const
  {
    const IRECT plot = PlotBounds();
    return static_cast<float>(EnvelopeTimeValue(segment, (targetX - plot.L) / plot.W()));
  }

  int HitTest(float x, float y) const
  {
    const IRECT plot = PlotBounds();
    const Geometry geo = Measure(plot);
    const float points[4][2] = {{geo.xAttack, geo.yPeak},
                                {geo.xDecay, geo.ySustain},
                                {geo.xSustain, geo.ySustain},
                                {geo.xRelease, geo.yEnd}};

    int best = kNone;
    float bestDist = 1.0e30f;

    for (int i = 0; i < 4; ++i)
    {
      const float dx = x - points[i][0];
      const float dy = y - points[i][1];
      const float dist = dx * dx + dy * dy;
      if (dist < bestDist)
      {
        bestDist = dist;
        best = i;
      }
    }

    return bestDist <= 20.f * 20.f ? best : kNone;
  }

  static void FormatMs(double ms, char* buf, int n)
  {
    if (ms >= 1000.0)
      std::snprintf(buf, n, "%.2fs", ms / 1000.0);
    else if (ms >= 100.0)
      std::snprintf(buf, n, "%.0fms", ms);
    else
      std::snprintf(buf, n, "%.1fms", ms);
  }

  static void FormatPercent(double pct, char* buf, int n) { std::snprintf(buf, n, "%.0f%%", pct); }

  IColor mAccent;
  int mAmountIdx = kNoParameter;
  int mDrag = kNone;
};


// ---------------------------------------------------------------------------
// Vertical ADSR fader with a painted cap (AMP panel).
// ---------------------------------------------------------------------------
class KeithFaderControl : public ISliderControlBase
{
public:
  KeithFaderControl(const IRECT& bounds, int paramIdx, const char* label, const IColor& accent = kMagenta)
  : ISliderControlBase(bounds, paramIdx, EDirection::Vertical, 4.0, 12.f)
  , mAccent(accent)
  {
    mLabel.Set(label);
    DisablePrompt(false);
    SetText(Mono(10.f, kPaper, EAlign::Center));
  }

  void OnInit() override { OnResize(); }

  void OnResize() override
  {
    mValueBounds = mRECT.GetFromBottom(16.f);
    mLabelBounds = mRECT.GetFromTop(14.f);
    SetTargetRECT(mRECT);
  }

  void Draw(IGraphics& g) override
  {
    const IRECT track = TrackBounds();
    g.FillRect(kPanelAlt, track);
    g.DrawRect(kLine, track, nullptr, kInnerOutline);
    const float value = static_cast<float>(std::clamp(GetValue(), 0., 1.));
    const float y = track.B - value * track.H();
    const IRECT cap(track.L - 2.f, y - 6.f, track.R + 2.f, y + 6.f);
    g.FillRect(kMuteRed, cap);
    g.DrawRect(kLine, cap, nullptr, 2.f);
    g.DrawLine(mAccent, track.MW(), track.T, track.MW(), cap.MH(), nullptr, 2.f);
    if (mLabelBounds.H() > 0.f)
      g.DrawText(Label(9.f, kPaper), mLabel.Get(), mLabelBounds);
    if (mValueBounds.H() > 0.f)
    {
      const IParam* pParam = GetParam();
      if (pParam)
      {
        WDL_String valueStr;
        pParam->GetDisplay(pParam->Value(), false, valueStr, true);
        g.DrawText(Value(10.f, kGold), valueStr.Get(), mValueBounds);
      }
    }
  }

private:
  IRECT TrackBounds() const
  {
    IRECT r = mRECT;
    r.ReduceFromTop(16.f);
    r.ReduceFromBottom(20.f);
    r.ReduceFromLeft(r.W() * 0.35f);
    r.ReduceFromRight(r.W() * 0.35f);
    return r;
  }

  IColor mAccent;
  WDL_String mLabel;
  IRECT mLabelBounds;
  IRECT mValueBounds;
};

class KeithMeterControl : public IControl
{
public:
  KeithMeterControl(const IRECT& bounds, int gainParam) : IControl(bounds, gainParam) { mIgnoreMouse = true; }
  void Draw(IGraphics& g) override
  {
    g.FillRect(kInk, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, kInnerOutline);
    const IParam* p = GetParam();
    const float level = p ? static_cast<float>(p->Value() / 100.) : 0.5f;
    for (int bar = 0; bar < 2; ++bar)
    {
      const IRECT col = ColCell(mRECT, bar, 2, 4.f).GetPadded(-4.f);
      const float h = col.H() * std::clamp(level * 1.1f, 0.05f, 1.f);
      g.FillRect(kGreen, IRECT(col.L, col.B - h, col.R, col.B));
    }
  }
};

// ---------------------------------------------------------------------------
// Live synthesized-drum pad grid. A pad click selects its slot. MIDI notes 36-51
// trigger sound, so selecting a pad never pretends to audition it.
// ---------------------------------------------------------------------------
class KeithPadGridControl : public IControl
{
public:
  KeithPadGridControl(const IRECT& bounds, int selectParam)
  : IControl(bounds, selectParam)
  {
  }

  void Draw(IGraphics& g) override
  {
    const float gap = 4.f;
    const IRECT grid = mRECT;
    const int selected = std::clamp(static_cast<int>(std::lround(GetValue(0) * 15.)), 0, 15);
    const auto* plugin = dynamic_cast<const Keith*>(GetDelegate());

    for (int i = 0; i < 16; ++i)
    {
      const int col = i % 4;
      const int row = 3 - (i / 4); // slot 01 bottom-left, like the documented map
      const IRECT cell = RowCell(ColCell(grid, col, 4, gap), row, 4, gap).GetPadded(-1.f);
      const bool muted = plugin && plugin->DrumSlotMuted(i);
      const bool soloed = plugin && plugin->DrumSlotSoloed(i);
      const bool active = i == selected;

      IColor fill = kPanelAlt;
      if (muted)
        fill = kMuteRed.WithOpacity(0.55f);
      if (soloed)
        fill = kGold;
      if (active)
        fill = kGreen;
      g.FillRect(fill, cell);
      g.DrawRect(active ? kPaper : kLine, cell, nullptr, active ? 4.f : 3.f);

      const IRECT nameBounds(cell.L, cell.T + 2.f, cell.R, cell.MH() + 2.f);
      const IRECT noteBounds(cell.L, cell.MH() + 2.f, cell.R, cell.B - 2.f);
      const IColor ink = (active || soloed) ? kInk : kPaper;

      g.DrawText(MonoBold(12.f, ink, EAlign::Center),
                 DrumSlotNames()[i], nameBounds);
      g.DrawText(Mono(9.f, active ? kInk : kCyan, EAlign::Center), DrumNoteNames()[i], noteBounds);
      if (muted)
        g.DrawText(MonoBold(9.f, kPaper, EAlign::Near), "M", cell.GetPadded(-5.f));
      if (soloed)
        g.DrawText(MonoBold(9.f, kInk, EAlign::Far), "S", cell.GetPadded(-5.f));
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod&) override
  {
    const float gap = 4.f;
    for (int i = 0; i < 16; ++i)
    {
      const int col = i % 4;
      const int row = 3 - (i / 4);
      const IRECT cell = RowCell(ColCell(mRECT, col, 4, gap), row, 4, gap).GetPadded(-1.f);
      if (cell.Contains(x, y))
      {
        SetValueFromUserInput(static_cast<double>(i) / 15., 0);
        return;
      }
    }
  }
};

} // namespace ui
} // namespace keith
