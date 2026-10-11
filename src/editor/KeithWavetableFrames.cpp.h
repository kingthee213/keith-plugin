#pragma once

#include <string>

#include "KeithWavetableFrames.h"
#include "IPlugPluginBase.h"
#include <algorithm>
#include <cmath>

namespace keith {
namespace ui {

inline float KeithWavetableFrames::CurrentMorph() const
{
  const auto* plugin = dynamic_cast<const iplug::IPluginBase*>(GetDelegate());
  if (plugin == nullptr || mMorphParam < 0 || plugin->GetParam(mMorphParam) == nullptr)
    return 0.f;
  return static_cast<float>(plugin->GetParam(mMorphParam)->Value() / 100.0);
}

inline void KeithWavetableFrames::WriteMorph(float morph01)
{
  auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
  if (plugin == nullptr || mMorphParam < 0)
    return;
  IParam* param = plugin->GetParam(mMorphParam);
  if (param == nullptr)
    return;
  param->SetNormalized(std::clamp(morph01, 0.f, 1.f));
  SetDirty(false);
  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

inline void KeithWavetableFrames::SetMorphFromPoint(float x, float y)
{
  const int count = static_cast<int>(mShapes.size());
  if (count < 1)
    return;

  if (mView == WavetableViewMode::Linear)
  {
    const float pad = 4.f;
    const float top = mRECT.T + pad;
    const float bottom = mRECT.B - pad;
    const float t = 1.f - (y - top) / std::max(1.f, bottom - top);
    WriteMorph(std::clamp(t, 0.f, 1.f));
    return;
  }

  const float cx = mRECT.MW();
  const float cy = mRECT.MH();
  const float dx = x - cx;
  const float dy = y - cy;
  const float dist = std::sqrt(dx * dx + dy * dy);
  const float maxR = std::min(mRECT.W(), mRECT.H()) * 0.48f;
  WriteMorph(std::clamp(dist / std::max(1.f, maxR), 0.f, 1.f));
}

inline void KeithWavetableFrames::StepMorph(int direction)
{
  const int count = static_cast<int>(mShapes.size());
  if (count < 2 || direction == 0)
    return;
  const float pos = CurrentMorph() * static_cast<float>(count - 1);
  int idx = static_cast<int>(std::lround(pos));
  idx = std::clamp(idx + direction, 0, count - 1);
  WriteMorph(static_cast<float>(idx) / static_cast<float>(count - 1));
}

inline void KeithWavetableFrames::Draw(IGraphics& g)
{
  if (!PageIsOpen())
    return;

  g.FillRect(kPanel, mRECT);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);

  if (mShapes.empty())
  {
    g.DrawText(Mono(9.f, kPaper, EAlign::Center), "No wavetable loaded", mRECT);
    return;
  }

  const int count = static_cast<int>(mShapes.size());
  const float morph = CurrentMorph();
  const float pad = 4.f;

  if (mView == WavetableViewMode::Polar)
  {
    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    const float maxR = std::min(mRECT.W(), mRECT.H()) * 0.48f;

    for (int f = 0; f < count; ++f)
    {
      const FrameShape& shape = mShapes[static_cast<size_t>(f)];
      const float r0 = maxR * (static_cast<float>(f) / static_cast<float>(count));
      const float r1 = maxR * (static_cast<float>(f + 1) / static_cast<float>(count));
      const int n = static_cast<int>(shape.points.size());
      for (int i = 0; i < n; ++i)
      {
        const float phase = static_cast<float>(i) / static_cast<float>(n);
        const float ang = phase * 6.2831853f;
        const float amp = shape.points[static_cast<size_t>(i)];
        const float r = r0 + (r1 - r0) * 0.5f + amp * (r1 - r0) * 0.42f;
        const float x0 = cx + std::cos(ang) * r;
        const float y0 = cy + std::sin(ang) * r;
        const float ang2 = (static_cast<float>(i + 1) / static_cast<float>(n)) * 6.2831853f;
        const float amp2 = shape.points[static_cast<size_t>((i + 1) % n)];
        const float r2 = r0 + (r1 - r0) * 0.5f + amp2 * (r1 - r0) * 0.42f;
        const float x1 = cx + std::cos(ang2) * r2;
        const float y1 = cy + std::sin(ang2) * r2;
        g.DrawLine(kPaper, x0, y0, x1, y1, nullptr, 1.f);
      }
    }

    const float mr = maxR * morph;
    g.DrawCircle(kHotPink, cx, cy, mr, nullptr, 1.5f);
  }
  else
  {
    const float rowH = (mRECT.H() - 2.f * pad) / count;
    const float left = mRECT.L + pad;
    const float width = (mRECT.R - pad) - left;

    for (int f = 0; f < count; ++f)
    {
      const int drawIndex = count - 1 - f;
      const FrameShape& shape = mShapes[static_cast<size_t>(drawIndex)];
      const float mid = mRECT.T + pad + rowH * (f + 0.5f);
      const float half = rowH * 0.42f;

      const IRECT row(left, mid - rowH * 0.5f, left + width, mid + rowH * 0.5f);
      if (drawIndex % 2 == 1)
        g.FillRect(kPanelAlt, row);

      g.DrawLine(kLine, left, mid, left + width, mid, nullptr, 1.f);

      const int n = static_cast<int>(shape.points.size());
      for (int i = 0; i + 1 < n; ++i)
      {
        const float x0 = left + width * (static_cast<float>(i) / (n - 1));
        const float x1 = left + width * (static_cast<float>(i + 1) / (n - 1));
        const float y0 = mid - shape.points[static_cast<size_t>(i)] * half;
        const float y1 = mid - shape.points[static_cast<size_t>(i + 1)] * half;
        g.DrawLine(kPaper, x0, y0, x1, y1, nullptr, 1.f);
      }

      const std::string label = std::to_string(drawIndex + 1);
      const IRECT tag(static_cast<int>(left) + 1, static_cast<int>(mid - rowH * 0.5f) + 1,
                      static_cast<int>(left) + 13, static_cast<int>(mid - rowH * 0.5f) + 11);
      g.DrawText(Mono(8.f, kPaper, EAlign::Near), label.c_str(), tag);
    }

    const float pos = morph * static_cast<float>(count - 1);
    const float y = mRECT.B - pad - rowH * (pos + 0.5f);
    g.DrawLine(kHotPink, mRECT.L + pad, y, mRECT.R - pad, y, nullptr, 2.f);
  }

  const int nearest = static_cast<int>(std::lround(morph * static_cast<float>(count - 1))) + 1;
  const std::string read =
      "TABLE " + std::to_string(count) + " FRAMES  " + std::to_string(nearest) + "/" + std::to_string(count);
  const IRECT tag(static_cast<int>(mRECT.R) - 200, static_cast<int>(mRECT.T) + 2,
                  static_cast<int>(mRECT.R) - 3, static_cast<int>(mRECT.T) + 14);
  g.DrawText(Mono(8.f, kHotPink, EAlign::Far), read.c_str(), tag);
}

inline void KeithWavetableFrames::OnMouseDown(float x, float y, const IMouseMod&)
{
  if (!PageIsOpen() || mShapes.empty())
    return;
  mDragging = true;
  SetMorphFromPoint(x, y);
}

inline void KeithWavetableFrames::OnMouseDrag(float x, float y, float, float, const IMouseMod&)
{
  if (!PageIsOpen() || !mDragging || mShapes.empty())
    return;
  SetMorphFromPoint(x, y);
}

inline void KeithWavetableViewSwitch::Draw(IGraphics& g)
{
  if (!PageIsOpen() || mTarget == nullptr)
    return;
  const bool polar = mTarget->ViewMode() == WavetableViewMode::Polar;
  const IRECT left(mRECT.L, mRECT.T, mRECT.MW(), mRECT.B);
  const IRECT right(mRECT.MW(), mRECT.T, mRECT.R, mRECT.B);
  g.FillRect(polar ? kPanelAlt : kHotPink, left);
  g.FillRect(polar ? kHotPink : kPanelAlt, right);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);
  g.DrawText(MonoBold(8.f, polar ? kPaper : kInk, EAlign::Center), "LINEAR", left);
  g.DrawText(MonoBold(8.f, polar ? kInk : kPaper, EAlign::Center), "POLAR", right);
}

inline void KeithWavetableViewSwitch::OnMouseDown(float x, float, const IMouseMod&)
{
  if (!PageIsOpen() || mTarget == nullptr)
    return;
  mTarget->SetViewMode(x < mRECT.MW() ? WavetableViewMode::Linear : WavetableViewMode::Polar);
  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

}  // namespace ui
}  // namespace keith
