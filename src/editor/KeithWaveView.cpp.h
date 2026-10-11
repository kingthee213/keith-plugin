/**
 * KeithWaveView implementation: the waveform and its slice markers.
 *
 * Split from the header so the drawing and hit-testing code sits together, where the shared
 * mapping between them is visible. They MUST agree; separating them across files is how a marker
 * ends up drawn where it cannot be grabbed.
 */

#pragma once

#include <functional>

#include "KeithWaveView.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace keith {
namespace ui {

// --- mapping --------------------------------------------------------------------------------
//
// One function each way, used by both Draw and the mouse handlers. The band is inset so a marker
// at frame 0 or at the very end is still fully draggable rather than half off the edge.

namespace {
constexpr float kEdgeInset = 2.f;
constexpr float kMarkerGrab = 5.f;   // px either side of a marker line that counts as grabbing it
}  // namespace

inline float KeithWaveView::FracForFrame(int frame) const
{
  if (mEnv.frames <= 0)
    return 0.f;
  return std::min(1.f, std::max(0.f, static_cast<float>(frame) / mEnv.frames));
}

inline int KeithWaveView::FrameForFrac(float frac) const
{
  if (mEnv.frames <= 0)
    return 0;
  return static_cast<int>(std::lround(std::min(1.f, std::max(0.f, frac)) * mEnv.frames));
}

inline float KeithWaveView::XForFrame(int frame) const
{
  const float l = mRECT.L + kEdgeInset;
  const float r = mRECT.R - kEdgeInset;
  return l + FracForFrame(frame) * (r - l);
}

inline void KeithWaveView::Rebuild()
{
  // Built at the panel's pixel width, so one envelope column is one pixel and the draw loop is a
  // straight line. This is the only place the sample is scanned.
  const int width = static_cast<int>(mRECT.W() - 2.f * kEdgeInset);
  mEnv = WaveEnvelope::Build(mSample, width);
}

// --- drawing --------------------------------------------------------------------------------

inline void KeithWaveView::Draw(IGraphics& g)
{
  if (!PageIsOpen())
    return;

  g.FillRect(kPanel, mRECT);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);

  if (mEnv.Empty())
  {
    // An explicit empty state, because an empty box is indistinguishable from a broken one -- which
    // is exactly what was reported when these panels drew nothing.
    g.DrawText(Mono(9.f, kPaper, EAlign::Center), "No sample loaded — click a name on the left",
               mRECT);
    return;
  }

  const float mid = mRECT.MH();
  const float half = (mRECT.H() * 0.5f) - 4.f;

  // One vertical line per envelope column. At this resolution that is what a waveform looks like,
  // and it costs one draw per pixel rather than one per sample.
  const int n = static_cast<int>(mEnv.columns.size());
  for (int i = 0; i < n; ++i)
  {
    const WaveEnvelope::Column& c = mEnv.columns[static_cast<size_t>(i)];
    if (!c.valid)
      continue;
    const float x = mRECT.L + kEdgeInset + static_cast<float>(i);
    const float yLo = mid + std::min(0.f, c.lo) * half;
    const float yHi = mid + std::max(0.f, c.hi) * half;
    // A line of at least a pixel, so a quiet passage still reads as audio rather than a gap.
    if (std::fabs(yHi - yLo) < 0.75f)
      g.DrawLine(kPaper, x, mid - 0.5f, x, mid + 0.5f, nullptr, 1.f);
    else
      g.DrawLine(kPaper, x, yHi, x, yLo, nullptr, 1.f);
  }

  // The centre line, so the zero crossing is visible.
  g.DrawLine(kLine, mRECT.L + kEdgeInset, mid, mRECT.R - kEdgeInset, mid, nullptr, 1.f);

  if (!mSlices)
    return;

  // Slice boundaries. Alternating tint by slice index so adjacent slices are distinguishable --
  // two markers of the same colour next to each other read as one line.
  const auto& slices = *mSlices;
  for (size_t i = 0; i < slices.size(); ++i)
  {
    const Slice& s = slices[i];
    const IColor col = (i % 2 == 0) ? kHotPink : kCyan;

    const float x0 = XForFrame(s.startFrame);
    g.DrawLine(col, x0, mRECT.T + 2.f, x0, mRECT.B - 2.f, nullptr, 1.f);

    // The number, so the pad that plays this slice is identifiable without playing it.
    if (static_cast<int>(i) < kMaxSliceKeys)
    {
      IRECT tag(static_cast<int>(x0) + 2, static_cast<int>(mRECT.T) + 2,
                static_cast<int>(x0) + 18, static_cast<int>(mRECT.T) + 14);
      if (tag.R < mRECT.R)
      {
        g.FillRect(kInk, tag);
        g.DrawText(MonoBold(8.f, col, EAlign::Center), std::to_string(i + 1), tag);
      }
    }
  }

  // The final slice's end. Derived through Slice::EndFrame rather than read from a member, because
  // a slice deliberately stores no end -- it derives one, so a marker drag cannot leave a start and
  // an end disagreeing.
  {
    const int last = static_cast<int>(slices.size()) - 1;
    const float xe = XForFrame(slices[static_cast<size_t>(last)].EndFrame(slices,
                                                                        static_cast<size_t>(last),
                                                                        mEnv.frames) + 1);
    g.DrawLine(kCyan, xe, mRECT.T + 2.f, xe, mRECT.B - 2.f, nullptr, 1.f);
  }
}

// --- interaction ----------------------------------------------------------------------------

inline void KeithWaveView::OnMouseDown(float x, float y, const IMouseMod&)
{
  if (!PageIsOpen() || mEnv.Empty() || !mSlices || mSlices->empty())
    return;

  // Nearest boundary within the grab band wins, so a click near a marker catches it rather than
  // doing nothing because it missed by a pixel.
  const auto& slices = *mSlices;
  int best = -1;
  float bestDist = kMarkerGrab;
  bool onStart = true;

  for (size_t i = 0; i < slices.size(); ++i)
  {
    const float ds = std::fabs(XForFrame(slices[i].startFrame) - x);
    if (ds < bestDist)
    {
      bestDist = ds;
      best = static_cast<int>(i);
      onStart = true;
    }
    // The final slice's end is grabbable too, but only for the last slice: an end that is also the
    // next slice's start would otherwise be two markers in the same place.
    if (i + 1 == slices.size())
    {
      const int e = slices[i].EndFrame(slices, i, mEnv.frames) + 1;
      const float de = std::fabs(XForFrame(e) - x);
      if (de < bestDist)
      {
        bestDist = de;
        best = static_cast<int>(i);
        onStart = false;
      }
    }
  }

  (void) y;
  if (best < 0)
  {
    mGrab = Grab::None;
    return;
  }

  mGrabIndex = best;
  mGrab = onStart ? Grab::Left : Grab::Right;
  const size_t bi = static_cast<size_t>(best);
  mGrabStartFrac = FracForFrame(slices[bi].startFrame);
  mGrabEndFrac = FracForFrame(slices[bi].EndFrame(slices, bi, mEnv.frames) + 1);
  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

inline void KeithWaveView::OnMouseDrag(float x, float, float, float, const IMouseMod&)
{
  if (!PageIsOpen() || mGrab == Grab::None || mGrabIndex < 0 || !mOnEdit)
    return;

  const float l = mRECT.L + kEdgeInset;
  const float r = mRECT.R - kEdgeInset;
  if (r <= l)
    return;
  float frac = (x - l) / (r - l);
  frac = std::min(1.f, std::max(0.f, frac));

  float start = mGrabStartFrac;
  float end = mGrabEndFrac;

  // A minimum gap, so a slice can never be zero-length: an empty slice plays nothing and cannot be
  // grabbed again, which loses it.
  const float minFrac = mEnv.frames > 0 ? (1.f / mEnv.frames) : 0.f;

  if (mGrab == Grab::Left)
    start = std::min(frac, end - minFrac);
  else
    end = std::max(frac, start + minFrac);

  mGrabStartFrac = std::min(1.f, std::max(0.f, start));
  mGrabEndFrac = std::min(1.f, std::max(0.f, end));

  // Report in FRAMES, converted here because this is the only place that knows the length.
  // The boundary being dragged is this slice's start, or the NEXT slice's start when the right
  // edge is held -- a slice stores no end, so its right edge IS the next slice's start.
  const int frame = FrameForFrac(mGrab == Grab::Left ? mGrabStartFrac : mGrabEndFrac);
  mOnEdit(mGrabIndex, frame);

  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

inline void KeithWaveView::OnMouseUp(float, float, const IMouseMod&)
{
  mGrab = Grab::None;
  mGrabIndex = -1;
}

}  // namespace ui
}  // namespace keith
