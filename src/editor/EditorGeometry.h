#pragma once
#include <algorithm>
#include <cmath>

namespace keith { namespace ui {
// Fixed time lanes keep one handle's drag from moving every other handle.
inline float EnvelopeTimeX(int segment, double value)
{
  const float v = static_cast<float>(std::clamp(value, 0., 1.));
  if (segment == 0) return 0.04f + 0.22f * v;
  if (segment == 1) return 0.30f + 0.22f * v;
  return 0.70f + 0.27f * v;
}
inline double EnvelopeTimeValue(int segment, float x)
{
  if (segment == 0) return std::clamp((x - 0.04) / 0.22, 0., 1.);
  if (segment == 1) return std::clamp((x - 0.30) / 0.22, 0., 1.);
  return std::clamp((x - 0.70) / 0.27, 0., 1.);
}
inline float ScrollOffset(float offset, float delta, float content, float viewport)
{
  return std::clamp(offset - delta * 32.f, 0.f, std::max(0.f, content - viewport));
}

struct Box
{
  float l, t, r, b;
  constexpr float W() const { return r - l; }
  constexpr float H() const { return b - t; }
  constexpr bool Contains(Box o) const
  {
    return o.l >= l && o.t >= t && o.r <= r && o.b <= b;
  }
  constexpr bool Overlaps(Box o) const
  {
    return l < o.r && o.l < r && t < o.b && o.t < b;
  }
};

// Page well and the six-button nav. Space Mono Bold's advance is 612/1000 em
// (resources/fonts/SpaceMono-Bold.ttf), so a 15px glyph is 9.18px wide.
constexpr Box kPageWell {34.f, 204.f, 1004.f, 718.f};
constexpr Box kNavBar {34.f, 144.f, 1022.f, 185.f};
constexpr int kNavCount = 6;
constexpr float kNavGap = 8.f;
constexpr float kNavTextPad = 6.f;
constexpr float kNavFontPx = 15.f;
constexpr float kSpaceMonoBoldEm = 0.612f;
constexpr int kNavLabelChars[] = {10, 14, 10, 13, 8, 11}; // "01  SOURCE" .. "06  LIBRARY"

inline Box NavCell(int index)
{
  const float span = kNavBar.W() - kNavGap * static_cast<float>(kNavCount - 1);
  const float w = span / static_cast<float>(kNavCount);
  const float l = kNavBar.l + static_cast<float>(index) * (w + kNavGap);
  return {l, kNavBar.t, l + w, kNavBar.b};
}

// Modulation page. Knobs carry a flat body because this page has no baked material.
constexpr Box kModLfoHeader {58.f, 218.f, 250.f, 264.f};
constexpr Box kModRate {58.f, 268.f, 210.f, 430.f};
constexpr Box kModLfoAmount {220.f, 268.f, 372.f, 430.f};
constexpr Box kModShapeLabel {58.f, 444.f, 148.f, 484.f};
constexpr Box kModShapeTabs {150.f, 440.f, 500.f, 484.f};
constexpr Box kModLfoDestLabel {58.f, 500.f, 148.f, 540.f};
constexpr Box kModLfoDestTabs {150.f, 496.f, 500.f, 540.f};
constexpr Box kModWheelHeader {530.f, 218.f, 820.f, 264.f};
constexpr Box kModWheelCc {830.f, 218.f, 980.f, 264.f};
constexpr Box kModWheelAmount {530.f, 268.f, 682.f, 430.f};
constexpr Box kModWheelDestLabel {700.f, 300.f, 980.f, 324.f};
constexpr Box kModWheelDestTabs {700.f, 328.f, 980.f, 372.f};
constexpr Box kVelHeader {530.f, 460.f, 780.f, 506.f};
constexpr Box kVelAmp {530.f, 508.f, 680.f, 680.f};
constexpr Box kVelFilter {690.f, 508.f, 840.f, 680.f};
constexpr Box kLegatoLabel {850.f, 520.f, 980.f, 544.f};
constexpr Box kLegatoTabs {850.f, 548.f, 980.f, 592.f};

// Filter page. Cutoff, resonance and type stay on the permanent channel.
constexpr Box kFilterHeader {58.f, 218.f, 360.f, 264.f};
constexpr Box kFilterPlot {58.f, 268.f, 700.f, 680.f};
constexpr Box kKeyTrack {740.f, 280.f, 960.f, 460.f};
constexpr Box kFilterDrive {740.f, 490.f, 960.f, 680.f};

// Source-page switches in the open band between the two mixer rows.
// Mixer knob rects are the baked placements; these switches must not cover them.
constexpr Box kSyncLabel {786.f, 418.f, 980.f, 434.f};
constexpr Box kSyncTabs {786.f, 434.f, 980.f, 466.f};
constexpr Box kRingLabel {786.f, 468.f, 980.f, 484.f};
constexpr Box kRingTabs {786.f, 484.f, 980.f, 502.f};
constexpr Box kMixerSub {784.f, 292.f, 876.f, 414.f};
constexpr Box kMixerNoise {888.f, 292.f, 980.f, 414.f};
constexpr Box kMixerFm {784.f, 505.f, 876.f, 627.f};
constexpr Box kMixerLevel {888.f, 505.f, 980.f, 627.f};

// Static Oswald Bold hhea metrics (resources/fonts/Oswald-Bold.ttf, no fvar table).
// NanoVG middle-aligns
// on this box, and each control is clipped to its rect, so the metric box has to
// fit. Advances are hmtx sums for the live heading strings.
constexpr float kOswaldUnits = 1000.f;
constexpr float kOswaldAscender = 1193.f;
constexpr float kOswaldDescender = -289.f;
constexpr int kOswaldKeithUnits = 2370;
constexpr int kOswaldLfoUnits = 1463;
constexpr int kOswaldModWheelUnits = 4776;
constexpr int kOswaldVelocityUnits = 3804;
constexpr int kOswaldResponseUnits = 4240;
constexpr float kHeadingPx = 28.f;
constexpr float kWordmarkPx = 48.f;
constexpr Box kWordmarkBox {49.f, 39.f, 299.f, 112.f};

inline float OswaldMetricHeight(float px)
{
  return px * (kOswaldAscender - kOswaldDescender) / kOswaldUnits;
}
inline float OswaldAdvance(int units, float px)
{
  return static_cast<float>(units) * px / kOswaldUnits;
}

inline constexpr Box kModulationBoxes[] = {
  kModLfoHeader, kModRate, kModLfoAmount, kModShapeLabel, kModShapeTabs,
  kModLfoDestLabel, kModLfoDestTabs, kModWheelHeader, kModWheelCc, kModWheelAmount,
  kModWheelDestLabel, kModWheelDestTabs, kVelHeader, kVelAmp, kVelFilter,
  kLegatoLabel, kLegatoTabs
};
inline constexpr Box kFilterBoxes[] = {kFilterHeader, kFilterPlot, kKeyTrack, kFilterDrive};
inline constexpr Box kSourceSwitchBoxes[] = {kSyncLabel, kSyncTabs, kRingLabel, kRingTabs};
inline constexpr Box kMixerKnobBoxes[] = {kMixerSub, kMixerNoise, kMixerFm, kMixerLevel};
} }
