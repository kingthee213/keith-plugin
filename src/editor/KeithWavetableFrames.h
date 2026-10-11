/**
 * The wavetable page's frame view: every frame in the bank, and where the morph sits.
 *
 * ## Why a frame view rather than a single waveform
 *
 * The engine has ONE bank of 8 frames, not a library of banks. `CoreSynth::MakeStarterBank()`
 * builds them: frame 0 is a sine, and each later frame adds upper harmonics while pulling the odd
 * ones back, opening a hollow squarer tone. The morph parameter walks across those 8 frames.
 *
 * So the thing worth DRAWING is all the frames at once, with the morph position marked. A single
 * large waveform would show one frame and hide the fact that morphing is what the bank is for, and
 * a "bank list" would show a list with exactly one row in it.
 *
 * The frames are held as harmonic spectra, so each one is rendered here by summing its harmonics
 * into a cycle. That is the same maths the oscillator does, written out rather than read from the
 * table, because the table is 9 mip levels deep and this only needs one picture of each frame.
 *
 * ## Cost
 *
 * One cycle of 128 samples per frame, 8 frames, rebuilt only when the bank changes -- not per
 * paint. Drawing is then a polyline over 128 points, 8 times, which is trivial.
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

/** One rendered cycle, for drawing. */
struct FrameShape
{
  static constexpr int kPoints = 128;
  std::vector<float> points;   // -1..1, one full cycle

  bool Empty() const { return points.empty(); }
};

/**
 * Render a bank's frames into drawable cycles.
 *
 * The harmonics are summed directly rather than read from the band-limited table: the table exists
 * so PLAYBACK is cheap, and it is 9 levels deep because playback picks a level per note. A picture
 * needs one level and one cycle, so the sum is simpler and is done once.
 */
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

    // Only the first `harmonics` are summed. The frames carry up to 256, but above about two dozen
    // they are inaudible and would make the picture a solid block rather than a readable shape.
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

    // Normalised to 1, so every frame draws at the same height and the SHAPE is what varies. Without
    // this a quiet frame would look small rather than different.
    if (peak > 1e-6f)
      for (float& v : shape.points)
        v /= peak;

    out.push_back(std::move(shape));
  }
  return out;
}

/**
 * All the frames, stacked, with the current morph position marked.
 *
 * The frames are drawn as a waterfall: one row per frame, top to bottom, in bank order. That makes
 * the morph direction visible -- the reader can see the sound opening up as the row number rises --
 * which a single overlaid waveform would hide.
 */
class KeithWavetableFrames : public KeithPageElement
{
public:
  KeithWavetableFrames(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  /** Rebuilt when the bank changes. Not per paint. */
  void SetBank(const synth::WavetableBank* bank)
  {
    mShapes = BuildFrameShapes(bank);
    mBank = bank;
  }

  /** The morph position, 0..1 across the frames, for the marker. */
  void SetMorph(float morph)
  {
    mMorph = std::min(1.f, std::max(0.f, morph));
  }

  void Draw(IGraphics& g) override;

private:
  const synth::WavetableBank* mBank = nullptr;
  std::vector<FrameShape> mShapes;
  float mMorph = 0.f;
};

}  // namespace ui
}  // namespace keith
