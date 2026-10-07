#include "../src/editor/EditorGeometry.h"
#include <cstdio>

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
      if (i) ok &= x > keith::ui::EnvelopeTimeX(segment, (i - 1) / 1000.);
    }
    std::printf("%s envelope segment %d: 1001 forward/inverse and monotonic checks\n", ok ? "PASS" : "FAIL", segment);
    failed += !ok;
  }
  using keith::ui::ScrollOffset;
  const bool scroll = ScrollOffset(0, 1, 660, 380) == 0 &&
    ScrollOffset(0, -1, 660, 380) == 32 && ScrollOffset(0, -100, 660, 380) == 280 &&
    ScrollOffset(280, 100, 660, 380) == 0 && ScrollOffset(0, -20, 40, 380) == 0;
  std::printf("%s scroll bounds, direction, short content\n", scroll ? "PASS" : "FAIL");
  failed += !scroll;

  using keith::ui::Box;
  const Box page = keith::ui::kPageWell;
  auto checkGroup = [&](const char* name, const Box* boxes, int count) {
    bool inside = true;
    bool clear = true;
    for (int i = 0; i < count; ++i)
    {
      inside = inside && page.Contains(boxes[i]);
      for (int j = i + 1; j < count; ++j)
        clear = clear && !boxes[i].Overlaps(boxes[j]);
    }
    std::printf("%s %s inside the page well\n", inside ? "PASS" : "FAIL", name);
    std::printf("%s %s controls do not overlap\n", clear ? "PASS" : "FAIL", name);
    failed += !inside + !clear;
  };
  checkGroup("modulation", keith::ui::kModulationBoxes,
             static_cast<int>(sizeof(keith::ui::kModulationBoxes) / sizeof(Box)));
  checkGroup("mod matrix", keith::ui::kModMatrixBoxes,
             static_cast<int>(sizeof(keith::ui::kModMatrixBoxes) / sizeof(Box)));
  checkGroup("filter", keith::ui::kFilterBoxes,
             static_cast<int>(sizeof(keith::ui::kFilterBoxes) / sizeof(Box)));
  checkGroup("FX insert 1", keith::ui::kFxInsert1Boxes,
             static_cast<int>(sizeof(keith::ui::kFxInsert1Boxes) / sizeof(Box)));
  checkGroup("FX insert 2", keith::ui::kFxInsert2Boxes,
             static_cast<int>(sizeof(keith::ui::kFxInsert2Boxes) / sizeof(Box)));
  checkGroup("FX master", keith::ui::kFxMasterBoxes,
             static_cast<int>(sizeof(keith::ui::kFxMasterBoxes) / sizeof(Box)));
  checkGroup("source switches", keith::ui::kSourceSwitchBoxes,
             static_cast<int>(sizeof(keith::ui::kSourceSwitchBoxes) / sizeof(Box)));

  bool mixerClear = true;
  for (Box sw : keith::ui::kSourceSwitchBoxes)
    for (Box knob : keith::ui::kMixerKnobBoxes)
      mixerClear = mixerClear && !sw.Overlaps(knob);
  std::printf("%s source switches clear of the baked mixer knobs\n", mixerClear ? "PASS" : "FAIL");
  failed += !mixerClear;

  bool nav = true;
  float previousRight = keith::ui::kNavBar.l;
  const float innerNeed = 14.f * keith::ui::kNavFontPx * keith::ui::kSpaceMonoBoldEm;
  for (int i = 0; i < keith::ui::kNavCount; ++i)
  {
    const Box cell = keith::ui::NavCell(i);
    nav = nav && keith::ui::kNavBar.Contains(cell);
    nav = nav && cell.l >= previousRight - 0.01f;
    const float inner = cell.W() - 2.f * keith::ui::kNavTextPad;
    const float label = static_cast<float>(keith::ui::kNavLabelChars[i]) * keith::ui::kNavFontPx * keith::ui::kSpaceMonoBoldEm;
    nav = nav && label <= inner;
    previousRight = cell.r + keith::ui::kNavGap;
  }
  nav = nav && innerNeed <= keith::ui::NavCell(1).W() - 2.f * keith::ui::kNavTextPad;
  const float headingMetric = keith::ui::OswaldMetricHeight(keith::ui::kHeadingPx);
  const float wordmarkMetric = keith::ui::OswaldMetricHeight(keith::ui::kWordmarkPx);
  const float keithAdvance = keith::ui::OswaldAdvance(keith::ui::kOswaldKeithUnits, keith::ui::kWordmarkPx);
  const bool headings =
    keith::ui::kModLfoHeader.H() >= headingMetric &&
    keith::ui::kModWheelHeader.H() >= headingMetric &&
    keith::ui::kVelHeader.H() >= headingMetric &&
    keith::ui::kFilterHeader.H() >= headingMetric &&
    keith::ui::OswaldAdvance(keith::ui::kOswaldLfoUnits, keith::ui::kHeadingPx) <= keith::ui::kModLfoHeader.W() &&
    keith::ui::OswaldAdvance(keith::ui::kOswaldModWheelUnits, keith::ui::kHeadingPx) <= keith::ui::kModWheelHeader.W() &&
    keith::ui::OswaldAdvance(keith::ui::kOswaldVelocityUnits, keith::ui::kHeadingPx) <= keith::ui::kVelHeader.W() &&
    keith::ui::OswaldAdvance(keith::ui::kOswaldResponseUnits, keith::ui::kHeadingPx) <= keith::ui::kFilterHeader.W() &&
    keith::ui::kWordmarkBox.H() >= wordmarkMetric &&
    keithAdvance <= keith::ui::kWordmarkBox.W();
  std::printf("%s seven nav labels fit at 15px Space Mono Bold\n", nav ? "PASS" : "FAIL");
  std::printf("%s Oswald headings fit (heading metric %.2f in %.0f, wordmark metric %.2f in %.0f, KEITH advance %.2f in %.0f)\n",
              headings ? "PASS" : "FAIL", headingMetric, keith::ui::kModLfoHeader.H(),
              wordmarkMetric, keith::ui::kWordmarkBox.H(), keithAdvance, keith::ui::kWordmarkBox.W());
  failed += !nav + !headings;

  return failed ? 1 : 0;
}
