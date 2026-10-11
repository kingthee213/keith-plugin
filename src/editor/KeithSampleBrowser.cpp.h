/**
 * KeithSampleBrowser implementation.
 *
 * Scroll and hit-testing share RowAt() and the same scroll offset, so a click cannot land on a
 * different row than the one drawn under the cursor -- the standard failure for a scrolling list.
 */

#pragma once

#include "KeithSampleBrowser.h"
#include <algorithm>
#include <string>

namespace keith {
namespace ui {

inline int KeithSampleBrowser::RowsVisible() const
{
  const int h = static_cast<int>(mRECT.H() - 2.f * kPad);
  return std::max(0, static_cast<int>(h / kRowH));
}

inline int KeithSampleBrowser::RowAt(float y) const
{
  const float top = mRECT.T + kPad;
  if (y < top)
    return -1;
  const int row = static_cast<int>((y - top) / kRowH);
  if (row < 0 || row >= RowsVisible())
    return -1;
  const int idx = mScroll + row;
  if (idx < 0 || idx >= static_cast<int>(mEntries.size()))
    return -1;
  return idx;
}

inline void KeithSampleBrowser::ClampScroll()
{
  const int maxScroll = std::max(0, static_cast<int>(mEntries.size()) - RowsVisible());
  mScroll = std::min(std::max(0, mScroll), maxScroll);
}

inline void KeithSampleBrowser::Draw(IGraphics& g)
{
  if (!PageIsOpen())
    return;

  g.FillRect(kPanel, mRECT);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);

  ClampScroll();

  if (mEntries.empty())
  {
    // An explicit empty state, because a blank box is indistinguishable from a broken one -- which
    // is precisely what was reported about these panels when they drew nothing.
    g.DrawText(Mono(9.f, kPaper, EAlign::Center), "No samples found", mRECT);
    return;
  }

  const int rows = RowsVisible();
  const int last = std::min(static_cast<int>(mEntries.size()), mScroll + rows);

  for (int i = mScroll; i < last; ++i)
  {
    const int row = i - mScroll;
    const IRECT r(mRECT.L + kPad, mRECT.T + kPad + row * kRowH, mRECT.R - kPad,
                  mRECT.T + kPad + (row + 1) * kRowH - 1.f);

    const bool sel = (i == mSelected);
    if (sel)
      g.FillRect(kHotPink, r);
    else if (row % 2 == 1)
      g.FillRect(kPanelAlt, r);   // banding, so a long list is countable at a glance

    // The group is shown as a dim prefix rather than a separate column: with four groups and a
    // narrow panel, a column would leave too little room for the name.
    const std::string text = mEntries[static_cast<size_t>(i)].label;
    g.DrawText(MonoBold(9.f, sel ? kInk : kPaper, EAlign::Near), text, r.GetPadded(4.f, 0.f, 0.f, 0.f));
  }

  // A scrollbar only when there is somewhere to scroll, so its presence means something.
  if (static_cast<int>(mEntries.size()) > rows && rows > 0)
  {
    const float trackH = static_cast<float>(rows) * kRowH;
    const float frac = static_cast<float>(rows) / static_cast<float>(mEntries.size());
    const float thumbH = std::max(12.f, trackH * frac);
    const float maxScroll = static_cast<float>(mEntries.size() - rows);
    const float t = maxScroll > 0.f ? static_cast<float>(mScroll) / maxScroll : 0.f;
    const float y = mRECT.T + kPad + t * (trackH - thumbH);
    const IRECT thumb(static_cast<int>(mRECT.R) - 4, static_cast<int>(y),
                      static_cast<int>(mRECT.R) - 1, static_cast<int>(y + thumbH));
    g.FillRect(kCyan, thumb);
  }
}

inline void KeithSampleBrowser::OnMouseDown(float, float y, const IMouseMod&)
{
  if (!PageIsOpen())
    return;

  const int idx = RowAt(y);
  if (idx < 0)
    return;

  mSelected = idx;
  if (mOnPick)
    mOnPick(mEntries[static_cast<size_t>(idx)]);

  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

inline void KeithSampleBrowser::OnMouseWheel(float, float, const IMouseMod&, float d)
{
  if (!PageIsOpen())
    return;

  // One row per notch, negated because a wheel reports positive for up and the list moves down.
  mScroll -= static_cast<int>(d);
  ClampScroll();

  if (auto* ui = GetUI())
    ui->SetAllControlsDirty();
}

}  // namespace ui
}  // namespace keith
