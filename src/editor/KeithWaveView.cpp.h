#pragma once

#include "KeithWaveView.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace keith {
namespace ui {

namespace {
constexpr float kEdgeInset = 2.f;
constexpr float kMarkerGrab = 6.f;
constexpr float kFlagW = 4.f;
const IColor kAutoSlice = kCyan;
const IColor kManualSlice = kPaper;
}  // namespace

inline KeithWaveView::SlicePtr KeithWaveView::CurrentSlices() const
{
  return mSliceSource ? mSliceSource() : SlicePtr {};
}

inline float KeithWaveView::FracForFrame(int frame) const
{
  if (mEnv.frames <= 0)
    return 0.f;
  return std::min(1.f, std::max(0.f, static_cast<float>(frame) / static_cast<float>(mEnv.frames)));
}

inline int KeithWaveView::FrameForFrac(float frac) const
{
  if (mEnv.frames <= 0)
    return 0;
  return static_cast<int>(std::lround(std::min(1.f, std::max(0.f, frac)) * mEnv.frames));
}

inline float KeithWaveView::XForFrac(float frac) const
{
  const float l = mRECT.L + kEdgeInset;
  const float r = mRECT.R - kEdgeInset;
  const float span = std::max(1e-6f, mVisEnd - mVisStart);
  const float t = (frac - mVisStart) / span;
  return l + t * (r - l);
}

inline float KeithWaveView::FracForX(float x) const
{
  const float l = mRECT.L + kEdgeInset;
  const float r = mRECT.R - kEdgeInset;
  if (r <= l)
    return 0.f;
  const float t = (x - l) / (r - l);
  const float span = std::max(1e-6f, mVisEnd - mVisStart);
  return std::min(1.f, std::max(0.f, mVisStart + t * span));
}

inline void KeithWaveView::Rebuild()
{
  const int width = static_cast<int>(mRECT.W() - 2.f * kEdgeInset);
  mEnv = WaveEnvelope::Build(mSample, width);
}

inline void KeithWaveView::ZoomAt(float centerFrac, float factor)
{
  const float span = mVisEnd - mVisStart;
  float newSpan = span * factor;
  newSpan = std::clamp(newSpan, 0.01f, 1.f);
  float s0 = centerFrac - (centerFrac - mVisStart) * (newSpan / span);
  float s1 = s0 + newSpan;
  if (s0 < 0.f)
  {
    s1 -= s0;
    s0 = 0.f;
  }
  if (s1 > 1.f)
  {
    s0 -= (s1 - 1.f);
    s1 = 1.f;
  }
  mVisStart = std::max(0.f, s0);
  mVisEnd = std::min(1.f, s1);
}

inline int KeithWaveView::HitRegionFlag(float x) const
{
  int regionStart = 0;
  int regionEnd = mEnv.frames;
  if (mRegionSource)
    mRegionSource(regionStart, regionEnd);
  const float f0 = FracForFrame(regionStart);
  const float f1 = FracForFrame(regionEnd);
  if (std::fabs(XForFrac(f0) - x) < kMarkerGrab + kFlagW)
    return 0;
  if (std::fabs(XForFrac(f1) - x) < kMarkerGrab + kFlagW)
    return 1;
  return -1;
}

inline int KeithWaveView::HitSliceBoundary(float x, bool* onStart) const
{
  const SlicePtr slices = CurrentSlices();
  if (!slices || slices->empty())
    return -1;

  int best = -1;
  float bestDist = kMarkerGrab;
  bool start = true;

  for (size_t i = 0; i < slices->size(); ++i)
  {
    const float ds = std::fabs(XForFrac(FracForFrame((*slices)[i].startFrame)) - x);
    if (ds < bestDist)
    {
      bestDist = ds;
      best = static_cast<int>(i);
      start = true;
    }
    if (i + 1 == slices->size())
    {
      const int e = (*slices)[i].EndFrame(*slices, i, mEnv.frames) + 1;
      const float de = std::fabs(XForFrac(FracForFrame(e)) - x);
      if (de < bestDist)
      {
        bestDist = de;
        best = static_cast<int>(i);
        start = false;
      }
    }
  }
  if (onStart)
    *onStart = start;
  return best;
}

inline void KeithWaveView::Draw(IGraphics& g)
{
  if (!PageIsOpen())
    return;

  g.FillRect(kPanel, mRECT);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);

  if (mEnv.Empty())
  {
    g.DrawText(Mono(9.f, kPaper, EAlign::Center), "No sample loaded — click a name on the left",
               mRECT);
    return;
  }

  int regionStart = 0;
  int regionEnd = mEnv.frames;
  if (mRegionSource)
    mRegionSource(regionStart, regionEnd);

  const float mid = mRECT.MH();
  const float half = (mRECT.H() * 0.5f) - 4.f;
  const float l = mRECT.L + kEdgeInset;
  const float r = mRECT.R - kEdgeInset;

  const int n = static_cast<int>(mEnv.columns.size());
  for (int i = 0; i < n; ++i)
  {
    const float frac = static_cast<float>(i) / static_cast<float>(std::max(1, n - 1));
    if (frac < mVisStart - 0.001f || frac > mVisEnd + 0.001f)
      continue;
    const WaveEnvelope::Column& c = mEnv.columns[static_cast<size_t>(i)];
    if (!c.valid)
      continue;
    const float x = XForFrac(frac);
    const float yLo = mid + std::min(0.f, c.lo) * half;
    const float yHi = mid + std::max(0.f, c.hi) * half;
    if (std::fabs(yHi - yLo) < 0.75f)
      g.DrawLine(kPaper, x, mid - 0.5f, x, mid + 0.5f, nullptr, 1.f);
    else
      g.DrawLine(kPaper, x, yHi, x, yLo, nullptr, 1.f);
  }

  g.DrawLine(kLine, l, mid, r, mid, nullptr, 1.f);

  // Region shading outside the flags.
  const float xReg0 = XForFrac(FracForFrame(regionStart));
  const float xReg1 = XForFrac(FracForFrame(regionEnd));
  if (xReg0 > l)
    g.FillRect(kInk.WithOpacity(0.35f), IRECT(l, mRECT.T, xReg0, mRECT.B));
  if (xReg1 < r)
    g.FillRect(kInk.WithOpacity(0.35f), IRECT(xReg1, mRECT.T, r, mRECT.B));

  const SlicePtr slices = CurrentSlices();
  if (slices)
  {
    for (size_t i = 0; i < slices->size(); ++i)
    {
      const sampler::Slice& s = (*slices)[i];
      const IColor col = s.manual ? kManualSlice : kAutoSlice;
      const float x0 = XForFrac(FracForFrame(s.startFrame));
      if (x0 < l - 1.f || x0 > r + 1.f)
        continue;
      g.DrawLine(col, x0, mRECT.T + 2.f, x0, mRECT.B - 2.f, nullptr, s.manual ? 1.5f : 1.f);
    }
    if (!slices->empty())
    {
      const int last = static_cast<int>(slices->size()) - 1;
      const int e = (*slices)[static_cast<size_t>(last)].EndFrame(*slices,
                                                                 static_cast<size_t>(last),
                                                                 mEnv.frames)
                    + 1;
      const float xe = XForFrac(FracForFrame(e));
      if (xe >= l - 1.f && xe <= r + 1.f)
        g.DrawLine(kAutoSlice, xe, mRECT.T + 2.f, xe, mRECT.B - 2.f, nullptr, 1.f);
    }
  }

  // Draggable region flags.
  for (int which = 0; which < 2; ++which)
  {
    const float xf = (which == 0) ? xReg0 : xReg1;
    g.FillRect(kHotPink, IRECT(xf - kFlagW, mRECT.T + 2.f, xf + kFlagW, mRECT.B - 2.f));
  }

  // Hot-swap button, lower-right of the waveform.
  const IRECT swap(mRECT.R - 52.f, mRECT.B - 18.f, mRECT.R - 4.f, mRECT.B - 4.f);
  g.FillRect(kPanelAlt, swap);
  g.DrawRect(kLine, swap, nullptr, 1.f);
  g.DrawText(MonoBold(8.f, kPaper, EAlign::Center), "SWAP", swap);

  const int pctStart = mEnv.frames > 0
                         ? static_cast<int>(100.f * FracForFrame(regionStart))
                         : 0;
  const int pctLen = mEnv.frames > 0
                       ? static_cast<int>(100.f * (FracForFrame(regionEnd) - FracForFrame(regionStart)))
                       : 0;
  const std::string read = "start " + std::to_string(pctStart) + "%  len " + std::to_string(pctLen) + "%";
  const IRECT tag(static_cast<int>(mRECT.L) + 4, static_cast<int>(mRECT.T) + 2,
                  static_cast<int>(mRECT.L) + 160, static_cast<int>(mRECT.T) + 14);
  g.DrawText(Mono(8.f, kPaper, EAlign::Near), read.c_str(), tag);
}

inline void KeithWaveView::OnMouseDown(float x, float y, const IMouseMod& mod)
{
  if (!PageIsOpen() || mEnv.Empty())
    return;

  const IRECT swap(mRECT.R - 52.f, mRECT.B - 18.f, mRECT.R - 4.f, mRECT.B - 4.f);
  if (swap.Contains(x, y))
  {
    if (mOnHotSwap)
      mOnHotSwap();
    return;
  }

  const int flag = HitRegionFlag(x);
  if (flag >= 0 && mOnRegion)
  {
    mGrab = flag == 0 ? Grab::LeftFlag : Grab::RightFlag;
    mGrabRegion = flag;
    return;
  }

  bool onStart = true;
  const int slice = HitSliceBoundary(x, &onStart);
  if (slice >= 0 && mod.A && mOnToggleManual)
  {
    mOnToggleManual(slice);
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
    return;
  }

  if (slice >= 0 && mOnMove)
  {
    mGrabIndex = slice;
    mGrab = onStart ? Grab::SliceStart : Grab::SliceEnd;
    return;
  }

  mGrab = Grab::Pan;
  mPanAnchorX = x;
  mPanAnchorFrac = FracForX(x);
  (void) y;
}

inline void KeithWaveView::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod)
{
  if (!PageIsOpen() || mGrab == Grab::None || mEnv.Empty())
    return;

  if (mGrab == Grab::Pan)
  {
    if (mod.C || mod.S)
    {
      ZoomAt(FracForX(x), std::pow(1.002f, -dY));
    }
    else
    {
      const float l = mRECT.L + kEdgeInset;
      const float r = mRECT.R - kEdgeInset;
      const float span = std::max(1e-6f, mVisEnd - mVisStart);
      const float pxSpan = std::max(1.f, r - l);
      const float deltaFrac = -(dX / pxSpan) * span;
      float s0 = mVisStart + deltaFrac;
      float s1 = mVisEnd + deltaFrac;
      if (s0 < 0.f)
      {
        s1 -= s0;
        s0 = 0.f;
      }
      if (s1 > 1.f)
      {
        s0 -= (s1 - 1.f);
        s1 = 1.f;
      }
      mVisStart = s0;
      mVisEnd = s1;
    }
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
    return;
  }

  const int frame = FrameForFrac(FracForX(x));

  if ((mGrab == Grab::LeftFlag || mGrab == Grab::RightFlag) && mOnRegion)
  {
    mOnRegion(mGrabRegion, frame);
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
    return;
  }

  if ((mGrab == Grab::SliceStart || mGrab == Grab::SliceEnd) && mOnMove && mGrabIndex >= 0)
  {
    const int idx = mGrab == Grab::SliceStart ? mGrabIndex : mGrabIndex + 1;
    mOnMove(idx, frame);
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
    return;
  }

  (void) y;
}

inline void KeithWaveView::OnMouseUp(float, float, const IMouseMod&)
{
  mGrab = Grab::None;
  mGrabIndex = -1;
  mGrabRegion = -1;
}

inline void KeithWaveView::OnMouseDblClick(float x, float, const IMouseMod&)
{
  if (!PageIsOpen() || mEnv.Empty())
    return;

  bool onStart = true;
  const int slice = HitSliceBoundary(x, &onStart);
  if (slice > 0 && onStart && mOnRemove)
  {
    mOnRemove(slice);
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
    return;
  }

  if (slice < 0 && mOnAdd)
  {
    mOnAdd(FrameForFrac(FracForX(x)));
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
  }
}

inline void KeithWaveView::OnMouseWheel(float x, float, const IMouseMod& mod, float d)
{
  if (!PageIsOpen() || mEnv.Empty())
    return;
  if (!mod.C && !mod.S)
    return;
  const float factor = d > 0.f ? 0.9f : 1.1f;
  ZoomAt(FracForX(x), factor);
  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

}  // namespace ui
}  // namespace keith
