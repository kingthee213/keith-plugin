#include "../src/editor/EditorGeometry.h"
#include <cstdio>
#include <cstring>

constexpr int kNumParams = 111;

int main()
{
  int failed = 0;

  for (int segment : {0, 1, 3})
  {
    bool ok = true;
    for (int i = 0; i <= 1000; ++i)
    {
      const double v = i / 1000.;
      const float x = keith::ui::EnvelopeTimeX(segment, v);
      const double recovered = keith::ui::EnvelopeTimeValue(segment, x);
      ok &= std::fabs(v - recovered) < 0.000001;
      if (i)
        ok &= x > keith::ui::EnvelopeTimeX(segment, (i - 1) / 1000.);
    }
    std::printf("%s envelope segment %d: forward/inverse and monotonic checks\n",
                ok ? "PASS" : "FAIL", segment);
    failed += !ok;
  }

  using keith::ui::ScrollOffset;
  const bool scroll = ScrollOffset(0, 1, 660, 380) == 0 &&
    ScrollOffset(0, -1, 660, 380) == 32 && ScrollOffset(0, -100, 660, 380) == 280 &&
    ScrollOffset(280, 100, 660, 380) == 0 && ScrollOffset(0, -20, 40, 380) == 0;
  std::printf("%s scroll bounds, direction, short content\n", scroll ? "PASS" : "FAIL");
  failed += !scroll;

  if (!keith::ui::kSingleWindowEditor)
  {
    std::printf("FAIL single-window editor flag is false\n");
    failed += 1;
  }
  else
  {
    std::printf("PASS single-window editor flag\n");
  }

  using keith::ui::Box;
  using keith::ui::ControlRect;
  const Box canvas = keith::ui::kCanvas;

  int seen[kNumParams] {};
  for (const ControlRect& item : keith::ui::kControlRects)
  {
    if (item.parameter < 0 || item.parameter >= kNumParams)
    {
      std::printf("FAIL control rect parameter %d out of range\n", item.parameter);
      failed += 1;
      continue;
    }
    seen[item.parameter] += 1;
    const bool inside = canvas.Contains(item.rect);
    std::printf("%s parameter %d inside canvas\n", inside ? "PASS" : "FAIL", item.parameter);
    failed += !inside;

    const float advance = keith::ui::MonoAdvance(item.labelUnits, item.labelFontPx);
    const bool labelFits = advance <= item.rect.W() && item.labelFontPx <= item.rect.H();
    std::printf("%s parameter %d label \"%s\" fits (advance %.1f in %.0f)\n",
                labelFits ? "PASS" : "FAIL", item.parameter, item.label,
                advance, item.rect.W());
    failed += !labelFits;
  }

  for (int param = 0; param < kNumParams; ++param)
  {
    const bool one = seen[param] == 1;
    std::printf("%s parameter %d has exactly one control rect (count %d)\n",
                one ? "PASS" : "FAIL", param, seen[param]);
    failed += !one;
  }

  bool clear = true;
  for (int i = 0; i < keith::ui::kNumControlRects; ++i)
  {
    for (int j = i + 1; j < keith::ui::kNumControlRects; ++j)
    {
      if (keith::ui::kControlRects[i].rect.Overlaps(keith::ui::kControlRects[j].rect))
      {
        std::printf("FAIL overlap parameter %d vs %d\n", keith::ui::kControlRects[i].parameter,
                    keith::ui::kControlRects[j].parameter);
        clear = false;
        failed += 1;
      }
    }
  }
  if (clear)
    std::printf("PASS all control rectangles are disjoint\n");

  const float wordmarkMetric = keith::ui::OswaldMetricHeight(keith::ui::kWordmarkPx);
  const bool wordmark = keith::ui::kWordmarkBox.H() >= wordmarkMetric;
  std::printf("%s wordmark box fits spray metric %.1f in %.0f\n", wordmark ? "PASS" : "FAIL",
              wordmarkMetric, keith::ui::kWordmarkBox.H());
  failed += !wordmark;

  return failed ? 1 : 0;
}
