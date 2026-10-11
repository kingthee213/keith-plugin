/**
 * The wavetable page's frame view: every frame in the bank, and where the morph sits.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "KeithPage.h"
#include "synth/WavetableOsc.h"

namespace keith {
namespace ui {

struct FrameShape
{
  static constexpr int kPoints = 128;
  std::vector<float> points;

  bool Empty() const { return points.empty(); }
};

inline std::vector<FrameShape> BuildFrameShapes(const synth::WavetableBank* bank, int harmonics = 24)
{
  std::vector<FrameShape> out;
  if (!bank || bank->FrameCount() == 0)
    return out;

  const auto& frames = bank->Frames();
  out.reserve(frames.size());

  for (const synth::WavetableFrame& frame : frames)
  {
    FrameShape shape;
    shape.points.assign(FrameShape::kPoints, 0.f);

    const int n = std::min(harmonics, synth::kWavetableMaxHarmonics);
    float peak = 0.f;

    for (int i = 0; i < FrameShape::kPoints; ++i)
    {
      const double phase = 2.0 * 3.14159265358979 * i / FrameShape::kPoints;
      float v = 0.f;
      for (int h = 0; h < n; ++h)
        v += frame.harmonic[h] * static_cast<float>(std::sin(phase * (h + 1)));
      shape.points[static_cast<size_t>(i)] = v;
      peak = std::max(peak, std::fabs(v));
    }

    if (peak > 1e-6f)
      for (float& v : shape.points)
        v /= peak;

    out.push_back(std::move(shape));
  }
  return out;
}

enum class WavetableViewMode
{
  Linear = 0,
  Polar = 1,
};

class KeithWavetableFrames : public KeithPageElement
{
public:
  KeithWavetableFrames(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  void SetBank(const synth::WavetableBank* bank)
  {
    mShapes = BuildFrameShapes(bank);
    mBank = bank;
  }

  void SetMorphParam(int paramIdx) { mMorphParam = paramIdx; }

  void SetViewMode(WavetableViewMode mode) { mView = mode; }
  WavetableViewMode ViewMode() const { return mView; }

  void Draw(IGraphics& g) override;

  void OnMouseDown(float x, float y, const IMouseMod&) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod&) override;

  void StepMorph(int direction);

private:
  void SetMorphFromPoint(float x, float y);
  float CurrentMorph();
  void WriteMorph(float morph01);

  const synth::WavetableBank* mBank = nullptr;
  std::vector<FrameShape> mShapes;
  int mMorphParam = -1;
  WavetableViewMode mView = WavetableViewMode::Linear;
  bool mDragging = false;
};

/** Toggle linear / polar on the frame view. */
class KeithWavetableViewSwitch : public KeithPageElement
{
public:
  KeithWavetableViewSwitch(const IRECT& bounds, const KeithPage* state, KeithPage page,
                           KeithWavetableFrames* target)
  : KeithPageElement(bounds, state, page)
  , mTarget(target)
  {
  }

  void Draw(IGraphics& g) override;
  void OnMouseDown(float x, float, const IMouseMod&) override;

private:
  KeithWavetableFrames* mTarget = nullptr;
};

}  // namespace ui
}  // namespace keith
