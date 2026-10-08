#include "../src/editor/EditorGeometry.h"
#include <cstdio>
#include <cstring>

constexpr int kNumParams = 111;
#include "worst_values.inc"
constexpr float kAdvanceEm = 0.60f;

static float TextWidth(const char* text, float fontPx)
{
  return kAdvanceEm * fontPx * static_cast<float>(std::strlen(text));
}

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
  using keith::ui::ControlKind;
  using keith::ui::ControlRect;
  using keith::ui::PanelSpec;
  using keith::ui::PanelTitleBand;
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

    const float labelAdvance = keith::ui::MonoAdvance(item.labelUnits, item.labelFontPx);
    const bool labelFits = labelAdvance <= item.rect.W();
    std::printf("%s parameter %d label \"%s\" fits width (advance %.1f in %.0f)\n",
                labelFits ? "PASS" : "FAIL", item.parameter, item.label,
                labelAdvance, item.rect.W());
    failed += !labelFits;

    if (item.kind == ControlKind::Knob)
    {
      const bool knobTall = item.rect.H() >= keith::ui::kKnobMinHeight;
      std::printf("%s parameter %d knob height %.0f >= %.0f\n",
                  knobTall ? "PASS" : "FAIL", item.parameter, item.rect.H(),
                  keith::ui::kKnobMinHeight);
      failed += !knobTall;
    }

    const char* worstValue = kWorstValueStr[item.parameter];
    const float valueAdvance = TextWidth(worstValue, item.labelFontPx);
    const float need = std::max(labelAdvance, valueAdvance);
    const bool valueFits = need <= item.rect.W();
    std::printf("%s parameter %d value \"%s\" fits width (advance %.1f in %.0f)\n",
                valueFits ? "PASS" : "FAIL", item.parameter, worstValue,
                valueAdvance, item.rect.W());
    failed += !valueFits;
  }

  for (int param = 0; param < kNumParams; ++param)
  {
    if (keith::ui::BypassViaAlgoMenu(param))
    {
      const bool none = seen[param] == 0;
      std::printf("%s parameter %d bypass reachable via algo menu (rect count %d)\n",
                  none ? "PASS" : "FAIL", param, seen[param]);
      failed += !none;
      continue;
    }

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

  bool titleBandsClear = true;
  for (const ControlRect& item : keith::ui::kControlRects)
  {
    for (const PanelSpec& panel : keith::ui::kPanels)
    {
      if (!item.rect.Overlaps(panel.bounds))
        continue;
      const Box band = PanelTitleBand(panel.bounds);
      if (item.rect.Overlaps(band))
      {
        std::printf("FAIL parameter %d overlaps title band in panel \"%s\"\n", item.parameter,
                    panel.title);
        titleBandsClear = false;
        failed += 1;
      }
    }
  }
  if (titleBandsClear)
    std::printf("PASS controls clear every panel title band\n");

  bool keyboardClear = true;
  for (const PanelSpec& panel : keith::ui::kPanels)
  {
    if (panel.bounds.Overlaps(keith::ui::kKeyboardBox) && panel.bounds.b > keith::ui::kKeyboardBox.t)
    {
      std::printf("FAIL panel \"%s\" extends into keyboard strip (bottom %.0f > keyboard top %.0f)\n",
                  panel.title, panel.bounds.b, keith::ui::kKeyboardBox.t);
      keyboardClear = false;
      failed += 1;
    }
  }
  for (const ControlRect& item : keith::ui::kControlRects)
  {
    if (item.rect.Overlaps(keith::ui::kKeyboardBox))
    {
      std::printf("FAIL parameter %d overlaps keyboard strip\n", item.parameter);
      keyboardClear = false;
      failed += 1;
    }
  }
  if (keyboardClear)
    std::printf("PASS keyboard strip is clear of panels and controls\n");

  constexpr float kRowPitchGap = 4.f;
  bool rowPitchClear = true;
  for (int i = 0; i < keith::ui::kNumControlRects; ++i)
  {
    for (int j = i + 1; j < keith::ui::kNumControlRects; ++j)
    {
      const ControlRect& a = keith::ui::kControlRects[i];
      const ControlRect& b = keith::ui::kControlRects[j];
      const float rowA = (a.rect.t + a.rect.b) * 0.5f;
      const float rowB = (b.rect.t + b.rect.b) * 0.5f;
      if (std::fabs(rowA - rowB) > 6.f)
        continue;
      const ControlRect* left = a.rect.l <= b.rect.l ? &a : &b;
      const ControlRect* right = left == &a ? &b : &a;
      if (right->rect.l <= left->rect.l)
        continue;
      const float pitch = right->rect.l - left->rect.l;
      const char* worstValue = kWorstValueStr[left->parameter];
      const float labelAdvance = keith::ui::MonoAdvance(left->labelUnits, left->labelFontPx);
      const float valueAdvance = TextWidth(worstValue, left->labelFontPx);
      const float need = std::max(labelAdvance, valueAdvance) + kRowPitchGap;
      if (need > pitch)
      {
        std::printf("FAIL row pitch parameter %d -> %d needs %.1f has %.1f\n", left->parameter,
                    right->parameter, need, pitch);
        rowPitchClear = false;
        failed += 1;
      }
    }
  }
  if (rowPitchClear)
    std::printf("PASS row pitch fits label and value between neighbours\n");

  const float wordmarkMetric = keith::ui::OswaldMetricHeight(keith::ui::kWordmarkPx);
  const bool wordmark = keith::ui::kWordmarkBox.H() >= wordmarkMetric;
  std::printf("%s wordmark box fits spray metric %.1f in %.0f\n", wordmark ? "PASS" : "FAIL",
              wordmarkMetric, keith::ui::kWordmarkBox.H());
  failed += !wordmark;

  return failed ? 1 : 0;
}
