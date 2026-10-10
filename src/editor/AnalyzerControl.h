#pragma once

/**
 * The SPECTRAL ANALYZER panel: three views, stepped by an arrow pager.
 *
 *   BARS       - the current spectrum as level-coloured bars, with peak caps
 *   WATERFALL  - the last kWaterfallRows spectra, newest at the bottom
 *   CURVE      - one filled trace with peak-hold dots riding it
 *
 * It moves on note-on, not continuously: the footer reads TRIG IDLE / -inf dB until a note
 * arrives, then TRIG ARMED and the measured peak. The feed arms for a fixed number of
 * frames so a short percussive hit stays visible between frames.
 *
 * The view index is held on the control, not as a host parameter. It is a display
 * preference, not something a saved patch should carry, and a parameter would need a
 * manifest row and a surface for no gain.
 */

#include "AnalyzerFeed.h"
#include "EditorGeometry.h"
#include "KeithTheme.h"

namespace keith {
namespace ui {

class KeithAnalyzerControl : public IControl
{
public:
  KeithAnalyzerControl(const IRECT& r, AnalyzerFeed& feed)
    : IControl(r), mFeed(feed)
  {
    mIgnoreMouse = false;
  }

  void Draw(IGraphics& g) override
  {
    mFeed.Snapshot(mView);
    const bool armed = mFeed.Armed();

    const float pad = 7.f;
    const IRECT title(mRECT.L + pad, mRECT.T + 5.f, mRECT.R - pad, mRECT.T + 26.f);
    mPager = IRECT(mRECT.L + pad, mRECT.T + 27.f, mRECT.R - pad, mRECT.T + 57.f);
    const IRECT foot(mRECT.L + 2.f, mRECT.B - 21.f, mRECT.R - 3.f, mRECT.B - 3.f);
    mPlot = IRECT(mRECT.L + pad, mPager.B + 4.f, mRECT.R - pad, foot.T - 20.f);

    g.FillRect(kInk, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, 1.f);

    g.DrawText(MonoBold(11.f, kPaper), "SPECTRAL ANALYZER", title);
    g.DrawText(Mono(9.f, kLine, EAlign::Far), ViewCounter(), title);

    DrawPager(g);
    DrawPlot(g);
    DrawAxis(g);
    DrawFooter(g, armed, foot);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    (void)y;
    (void)mod;
    if (mView > 0 && x < mPager.L + kArrowW)
    {
      --mView;
      return;
    }
    if (mView < kViews - 1 && x > mPager.R - kArrowW)
      ++mView;
  }

private:
  static constexpr int kViews = 3;
  static constexpr float kArrowW = 26.f;
  static constexpr float kFloorDb = -96.f;
  static constexpr float kCeilDb = 0.f;

  static const char* ViewName(int v)
  {
    static const char* kNames[kViews] = {"BARS", "WATERFALL", "CURVE"};
    return kNames[v < 0 ? 0 : (v >= kViews ? kViews - 1 : v)];
  }

  WDL_String ViewCounter() const
  {
    WDL_String s;
    s.SetFormatted(32, "VIEW %d/%d", mView + 1, kViews);
    return s;
  }

  /** Maps a dB reading onto 0..1 across the well. */
  static float Level(float db)
  {
    const float t = (db - kFloorDb) / (kCeilDb - kFloorDb);
    return t < 0.f ? 0.f : (t > 1.f ? 1.f : t);
  }

  /** Magenta at the floor, violet mid, cyan at the top. */
  static IColor Ramp(float level)
  {
    const float t = level < 0.f ? 0.f : (level > 1.f ? 1.f : level);
    if (t < 0.55f)
    {
      const float k = t / 0.55f;
      return IColor(255,
                    static_cast<int>(255 + (196 - 255) * k),
                    static_cast<int>(79 + (96 - 79) * k),
                    static_cast<int>(163 + (224 - 163) * k));
    }
    const float k = (t - 0.55f) / 0.45f;
    return IColor(255,
                  static_cast<int>(196 + (110 - 196) * k),
                  static_cast<int>(96 + (200 - 96) * k),
                  static_cast<int>(224 + (214 - 224) * k));
  }

  void DrawPager(IGraphics& g)
  {
    g.FillRect(kPanelAlt, mPager);
    const IRECT left(mPager.L, mPager.T, mPager.L + kArrowW, mPager.B);
    const IRECT right(mPager.R - kArrowW, mPager.T, mPager.R, mPager.B);
    g.FillRect(mView > 0 ? kPanel : kInk, left);
    g.FillRect(mView < kViews - 1 ? kPanel : kInk, right);

    const IColor leftInk = mView > 0 ? kPaper : kLine;
    g.FillTriangle(leftInk, left.MH() + 4.f, left.MH() - 5.f,
                   left.MH() + 4.f, left.MH() + 5.f, left.MH() - 4.f, left.MH());
    const IColor rightInk = mView < kViews - 1 ? kPaper : kLine;
    g.FillTriangle(rightInk, right.MH() - 4.f, right.MH() - 5.f,
                   right.MH() - 4.f, right.MH() + 5.f, right.MH() + 4.f, right.MH());

    const IRECT name(left.R + 5.f, mPager.T, right.L - 5.f, mPager.B);
    g.FillRect(kInk, name);
    g.DrawText(MonoBold(12.f, kPaper, EAlign::Center), ViewName(mView), name);

    // Position dots, so the order is readable and not just the counter in the corner.
    const float span = kViews * 10.f - 6.f;
    float x = name.MW() - span * 0.5f;
    const float y = mPager.B - 6.f;
    for (int i = 0; i < kViews; ++i)
    {
      g.FillEllipse(i == mView ? kHotPink : kPanelAlt, x, y - 3.f, x + 6.f, y + 3.f);
      x += 10.f;
    }
  }

  void DrawPlot(IGraphics& g)
  {
    g.FillRect(kInk, mPlot);
    for (int i = 1; i < 5; ++i)
    {
      const float y = mPlot.T + mPlot.H() * i / 5.f;
      g.DrawLine(kLine.WithOpacity(0.55f), mPlot.L, y, mPlot.R, y, nullptr, 1.f);
    }

    if (!mFeed.Armed())
    {
      DrawIdle(g);
      return;
    }

    switch (mView)
    {
      case 1: DrawWaterfall(g); break;
      case 2: DrawCurve(g); break;
      default: DrawBars(g); break;
    }
  }

  /** No note yet: the panel reads its own name rather than sitting blank. */
  void DrawIdle(IGraphics& g)
  {
    const float y = mPlot.MH();
    g.DrawLine(kPanelAlt, mPlot.L, y, mPlot.R, y, nullptr, 2.f);
    g.DrawText(MonoBold(40.f, kPanel, EAlign::Center), "MAGS", mPlot);
  }

  void DrawBars(IGraphics& g)
  {
    const float* bins = mFeed.Smoothed();
    const int n = mFeed.Bins();
    const float bw = mPlot.W() / n;
    for (int i = 0; i < n; ++i)
    {
      const float lv = Level(bins[i]);
      const float h = mPlot.H() * lv;
      if (h < 2.f)
        continue;
      const float x0 = mPlot.L + i * bw + 1.f;
      const float x1 = mPlot.L + (i + 1) * bw - 1.f;
      g.FillRect(Ramp(lv), x0, mPlot.B - h, x1, mPlot.B);
      if (lv > 0.80f)
        g.FillRect(kPaper, x0, mPlot.B - h - 2.f, x1, mPlot.B - h);
    }
  }

  void DrawWaterfall(IGraphics& g)
  {
    const int rows = mFeed.WaterfallRows();
    const int n = mFeed.Bins();
    const float rh = mPlot.H() / rows;
    g.FillRect(kInk, mPlot);
    for (int r = 0; r < rows; ++r)
    {
      // Newest row at the bottom, history scrolling up.
      const float fade = 0.22f + 0.78f * std::pow(1.f - static_cast<float>(r) / rows, 1.6f);
      const float* bins = mFeed.WaterfallRow(r);
      for (int i = 0; i < n; ++i)
      {
        const float lv = Level(bins[i]) * fade;
        if (lv <= 0.10f)
          continue;
        const float x0 = mPlot.L + i * (mPlot.W() / n);
        const float y0 = mPlot.T + r * rh;
        g.FillRect(Ramp(lv), x0, y0, x0 + mPlot.W() / n - 0.5f, y0 + rh - 0.4f);
      }
    }
    g.DrawLine(kHotPink, mPlot.L, mPlot.B - rh - 1.f, mPlot.R, mPlot.B - rh - 1.f,
               nullptr, 2.f);
  }

  void DrawCurve(IGraphics& g)
  {
    const float* bins = mFeed.Smoothed();
    const int n = mFeed.Bins();
    float prevX = 0.f;
    float prevY = 0.f;
    for (int i = 0; i < n; ++i)
    {
      const float lv = Level(bins[i]);
      const float x = mPlot.L + mPlot.W() * i / (n - 1);
      const float y = mPlot.B - mPlot.H() * lv * 0.92f;
      if (i)
        g.DrawLine(kHotPink, prevX, prevY, x, y, nullptr, 2.f);
      prevX = x;
      prevY = y;
      if ((i % 8) == 0)
        g.FillEllipse(kPaper, x - 1.8f, y - 1.8f, x + 1.8f, y + 1.8f);
    }
  }

  void DrawAxis(IGraphics& g)
  {
    const float y = mPlot.B + 2.f;
    g.DrawLine(kPanelAlt, mPlot.L, y, mPlot.R, y, nullptr, 1.f);
    static const char* kLabels[5] = {"60", "250", "1k", "4k", "16k"};
    static const float kFrac[5] = {0.02f, 0.24f, 0.50f, 0.76f, 0.96f};
    for (int i = 0; i < 5; ++i)
    {
      const float x = mPlot.L + mPlot.W() * kFrac[i];
      g.DrawText(Mono(8.f, kPanelAlt), kLabels[i],
                 IRECT(x, y + 1.f, x + 40.f, y + 14.f));
    }
  }

  void DrawFooter(IGraphics& g, bool armed, const IRECT& foot)
  {
    g.FillRect(kPanelAlt, foot);
    const IRECT row(foot.L + 10.f, foot.T, foot.R - 10.f, foot.B);
    g.DrawText(Mono(9.f, kPaper), "NOTE-ON", row);
    const IRECT state(row.L + 78.f, row.T, row.R - 62.f, row.B);
    if (!armed)
    {
      g.DrawText(Mono(9.f, kPaper), "TRIG IDLE", state);
      g.DrawText(MonoBold(9.f, kPaper, EAlign::Far), "-inf dB", row);
      return;
    }
    g.DrawText(MonoBold(9.f, kHotPink), "TRIG ARMED", state);
    WDL_String peak;
    peak.SetFormatted(24, "%.1f dB", mFeed.PeakDb());
    g.DrawText(MonoBold(9.f, kPaper, EAlign::Far), peak, row);
  }

  AnalyzerFeed& mFeed;
  int mView = 2;  // CURVE by default: it fills the well and reads at a glance.
  IRECT mPager, mPlot;
};

}  // namespace ui
}  // namespace keith
