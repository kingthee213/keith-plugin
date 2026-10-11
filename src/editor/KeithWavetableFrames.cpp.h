/**
 * KeithWavetableFrames implementation.
 *
 * An unnormalised sine summed over 24 harmonics of 1/n is a sawtooth, which is why the shapes are
 * normalised to 1 in BuildFrameShapes -- otherwise later frames would draw taller merely because
 * they have more harmonics, and the picture would show loudness where it should show shape.
 */

#pragma once

#include <string>

#include "KeithWavetableFrames.h"

namespace keith {
namespace ui {

inline void KeithWavetableFrames::Draw(IGraphics& g)
{
  if (!PageIsOpen())
    return;

  g.FillRect(kPanel, mRECT);
  g.DrawRect(kLine, mRECT, nullptr, 1.f);

  if (mShapes.empty())
  {
    // An explicit empty state. A blank box is what the owner reported and what this replaces; an
    // empty box and a broken one look the same, so the view says which it is.
    g.DrawText(Mono(9.f, kPaper, EAlign::Center), "No wavetable loaded", mRECT);
    return;
  }

  const int count = static_cast<int>(mShapes.size());
  const float pad = 4.f;
  const float rowH = (mRECT.H() - 2.f * pad) / count;
  const float left = mRECT.L + pad;
  const float width = (mRECT.R - pad) - left;

  for (int f = 0; f < count; ++f)
  {
    const FrameShape& shape = mShapes[static_cast<size_t>(f)];
    const float mid = mRECT.T + pad + rowH * (f + 0.5f);
    const float half = rowH * 0.42f;   // leaves a gap between rows so they read as separate

    // The frame's own row background, alternating, so 8 rows are countable without markers.
    const IRECT row(left, mid - rowH * 0.5f, left + width, mid + rowH * 0.5f);
    if (f % 2 == 1)
      g.FillRect(kPanelAlt, row);

    // The zero line for this row.
    g.DrawLine(kLine, left, mid, left + width, mid, nullptr, 1.f);

    // The shape. One line segment per point; 128 points across the row's width.
    const int n = static_cast<int>(shape.points.size());
    for (int i = 0; i + 1 < n; ++i)
    {
      const float x0 = left + width * (static_cast<float>(i) / (n - 1));
      const float x1 = left + width * (static_cast<float>(i + 1) / (n - 1));
      const float y0 = mid - shape.points[static_cast<size_t>(i)] * half;
      const float y1 = mid - shape.points[static_cast<size_t>(i + 1)] * half;
      g.DrawLine(kPaper, x0, y0, x1, y1, nullptr, 1.f);
    }

    // The frame number, so a morph position can be referred to.
    const std::string label = std::to_string(f + 1);
    const IRECT tag(static_cast<int>(left) + 1, static_cast<int>(mid - rowH * 0.5f) + 1,
                    static_cast<int>(left) + 13, static_cast<int>(mid - rowH * 0.5f) + 11);
    g.DrawText(Mono(8.f, kPaper, EAlign::Near), label.c_str(), tag);
  }

  // Where the morph currently sits. A horizontal line across the whole view at the interpolated
  // row, because the morph is continuous -- it can sit between two frames, and a marker snapped to
  // a row would misreport that.
  const float pos = mMorph * static_cast<float>(count - 1);
  const float y = mRECT.T + pad + rowH * (pos + 0.5f);
  g.DrawLine(kHotPink, mRECT.L + pad, y, mRECT.R - pad, y, nullptr, 2.f);

  // The frame it is nearest, and how far between, so the readout and the picture agree.
  const int nearest = static_cast<int>(std::lround(pos)) + 1;
  const std::string read = "morph " + std::to_string(nearest) + "/" + std::to_string(count);
  const IRECT tag(static_cast<int>(mRECT.R) - 74, static_cast<int>(mRECT.T) + 2,
                  static_cast<int>(mRECT.R) - 3, static_cast<int>(mRECT.T) + 14);
  g.DrawText(Mono(8.f, kHotPink, EAlign::Far), read.c_str(), tag);
}

}  // namespace ui
}  // namespace keith
