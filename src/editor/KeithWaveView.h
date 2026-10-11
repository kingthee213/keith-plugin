/**
 * The two controls the sampler page was missing: the waveform, and the sample browser.
 *
 * ## Why this file exists
 *
 * The page shipped with titles, mode buttons and seven EMPTY panels -- KeithPagePanel draws a
 * filled rectangle and a border and nothing else. There was no waveform display, no slice markers
 * and no browser list anywhere in the editor; grepping for DrawWaveform returned nothing. The owner
 * opened it and reported exactly that: "theres literally no buttons, no graph for the sample to be
 * seen and edited".
 *
 * So these are new controls, not fixes to existing ones.
 *
 * ## The two performance rules that shape the waveform drawing
 *
 * Draw() runs on the message thread every time the editor repaints. A waveform is thousands of
 * samples wide and the visible area is a few hundred pixels, so:
 *
 *   1. The peak envelope is computed ONCE per sample load, into a fixed-resolution table, not on
 *      every paint. Scanning 500,000 frames per frame of animation would make the editor crawl.
 *
 *   2. Drawing uses line segments per pixel column, not per sample. Even with the envelope, a loop
 *      over every frame would be wasteful; one vertical line per column is what a waveform actually
 *      looks like at this resolution anyway.
 *
 * The envelope is min/max per column, so a transient that occupies one sample of a large file still
 * shows up rather than being averaged into nothing.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <functional>   // std::function, the slice-drag callback
#include <memory>
#include <string>
#include <vector>

#include "KeithPage.h"
// Needed for SampleData and Slice: the view draws both, and neither arrives through KeithPage.h.
#include "sampler/SampleSlicer.h"
#include "sampler/SamplerVoice.h"

namespace keith {
namespace ui {

/** Build a min/max envelope at a fixed width, once, for the editor to draw repeatedly. */
struct WaveEnvelope
{
  struct Column
  {
    float lo = 0.f;
    float hi = 0.f;
    bool valid = false;
  };

  std::vector<Column> columns;
  int frames = 0;          // the sample length this was built from, for mapping to x
  double sampleRate = 44100.;
  std::string name;

  /** True when there is nothing to draw, so the view can show its empty state. */
  bool Empty() const { return frames <= 0 || columns.empty(); }

  double Seconds() const { return sampleRate > 0. ? static_cast<double>(frames) / sampleRate : 0.; }

  /**
   * Build from the mono sum of a sample.
   *
   * `width` is the number of columns. It is the panel's pixel width, so one column maps to one
   * pixel and the drawing loop is trivial. Rebuilt on load and on resize, never per paint.
   */
  static WaveEnvelope Build(const std::shared_ptr<const SampleData>& sample, int width)
  {
    WaveEnvelope env;
    if (!sample || sample->Empty() || width <= 0)
      return env;

    env.frames = sample->frames;
    env.sampleRate = sample->sampleRate;
    env.name = sample->name;
    env.columns.assign(static_cast<size_t>(width), Column {});

    // One pass over the frames, not one per column: framesPerColumn can be fractional, so a
    // column-by-column scan would walk the sample width times.
    const double per = static_cast<double>(sample->frames) / width;
    for (int c = 0; c < width; ++c)
    {
      const int from = static_cast<int>(c * per);
      int to = static_cast<int>((c + 1) * per);
      if (to <= from)
        to = from + 1;
      to = std::min(to, sample->frames);

      float lo = 0.f;
      float hi = 0.f;
      bool any = false;
      for (int f = from; f < to; ++f)
      {
        const float v = sample->MonoAt(f);
        lo = any ? std::min(lo, v) : v;
        hi = any ? std::max(hi, v) : v;
        any = true;
      }
      env.columns[static_cast<size_t>(c)] = { lo, hi, any };
    }
    return env;
  }
};

/**
 * The waveform, with slice markers.
 *
 * Drawing and hit-testing share one mapping function, so a marker cannot be drawn in one place and
 * be draggable in another -- which is the usual way a view like this goes wrong.
 */
class KeithWaveView : public KeithPageElement
{
public:
  KeithWaveView(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  /** Called from the plugin when a sample is loaded or the panel is resized. */
  void SetSample(const std::shared_ptr<const SampleData>& sample, double sampleRate)
  {
    mSample = sample;
    mRate = sampleRate > 0. ? sampleRate : 44100.;
    Rebuild();
  }

  void SetSlices(std::shared_ptr<const std::vector<Slice>> slices)
  {
    mSlices = std::move(slices);
  }

  /**
   * The drag target. Named rather than indexed, because the marker set is rebuilt on every
   * detection and an index would then refer to a different marker.
   */
  enum class Grab
  {
    None,
    Left,
    Right,
    Interior,
  };

  void Draw(IGraphics& g) override;

  void OnMouseDown(float x, float y, const IMouseMod&) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod&) override;
  void OnMouseUp(float x, float y, const IMouseMod&) override;

  /**
   * Set by the page so a drag can move a slice boundary.
   *
   * The callback takes FRAMES, not fractions. The view is the only thing that knows the sample
   * length, so it does the conversion; handing out fractions would make every caller repeat the
   * mapping and get it subtly wrong. Frames are also what the sampler's MoveSliceBoundary takes.
   */
  void SetSliceEdit(std::function<void(int idx, int frame)> fn) { mOnEdit = std::move(fn); }

  /** The sample's frame count, which the last slice needs to find its own end. */
  int TotalFrames() const { return mEnv.frames; }

private:
  void Rebuild();
  /** Frame -> normalised 0..1 across the view. One mapping, shared by draw and hit-test. */
  float FracForFrame(int frame) const;
  int FrameForFrac(float frac) const;
  /** Frame -> x in pixels. */
  float XForFrame(int frame) const;

  std::shared_ptr<const SampleData> mSample;
  std::shared_ptr<const std::vector<Slice>> mSlices;
  WaveEnvelope mEnv;
  double mRate = 44100.;
  Grab mGrab = Grab::None;
  int mGrabIndex = -1;
  float mGrabStartFrac = 0.f;
  float mGrabEndFrac = 0.f;
  std::function<void(int, int)> mOnEdit;
};

}  // namespace ui
}  // namespace keith
