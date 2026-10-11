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
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "KeithPage.h"

#include "sampler/SampleSlicer.h"
#include "sampler/SamplerVoice.h"

namespace keith {
namespace ui {

struct WaveEnvelope
{
  struct Column
  {
    float lo = 0.f;
    float hi = 0.f;
    bool valid = false;
  };

  std::vector<Column> columns;
  int frames = 0;
  double sampleRate = 44100.;
  std::string name;

  bool Empty() const { return frames <= 0 || columns.empty(); }

  double Seconds() const { return sampleRate > 0. ? static_cast<double>(frames) / sampleRate : 0.; }

  static WaveEnvelope Build(const std::shared_ptr<const sampler::SampleData>& sample, int width)
  {
    WaveEnvelope env;
    if (!sample || sample->Empty() || width <= 0)
      return env;

    env.frames = sample->frames;
    env.sampleRate = sample->sampleRate;
    env.name = sample->name;
    env.columns.assign(static_cast<size_t>(width), Column {});

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

class KeithWaveView : public KeithPageElement
{
public:
  KeithWaveView(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  void SetSample(const std::shared_ptr<const sampler::SampleData>& sample, double sampleRate)
  {
    mSample = sample;
    mRate = sampleRate > 0. ? sampleRate : 44100.;
    mVisStart = 0.f;
    mVisEnd = 1.f;
    Rebuild();
  }

  using SlicePtr = std::shared_ptr<const std::vector<sampler::Slice>>;
  void SetSliceSource(std::function<SlicePtr()> fn) { mSliceSource = std::move(fn); }

  void SetRegionSource(std::function<void(int& start, int& endExclusive)> fn)
  {
    mRegionSource = std::move(fn);
  }

  void SetSliceEdit(std::function<void(int idx, int frame)> fn) { mOnMove = std::move(fn); }
  void SetSliceAdd(std::function<void(int frame)> fn) { mOnAdd = std::move(fn); }
  void SetSliceRemove(std::function<void(int idx)> fn) { mOnRemove = std::move(fn); }
  void SetSliceToggleManual(std::function<void(int idx)> fn) { mOnToggleManual = std::move(fn); }
  void SetRegionEdit(std::function<void(int which, int frame)> fn) { mOnRegion = std::move(fn); }
  void SetOnHotSwap(std::function<void()> fn) { mOnHotSwap = std::move(fn); }

  enum class Grab
  {
    None,
    LeftFlag,
    RightFlag,
    SliceStart,
    SliceEnd,
    Pan,
  };

  void Draw(IGraphics& g) override;

  void OnMouseDown(float x, float y, const IMouseMod&) override;
  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod&) override;
  void OnMouseUp(float x, float y, const IMouseMod&) override;
  void OnMouseDblClick(float x, float y, const IMouseMod&) override;
  void OnMouseWheel(float x, float y, const IMouseMod&, float d) override;

  int TotalFrames() const { return mEnv.frames; }

private:
  void Rebuild();
  float FracForFrame(int frame) const;
  int FrameForFrac(float frac) const;
  float XForFrac(float frac) const;
  float FracForX(float x) const;
  int HitSliceBoundary(float x, bool* onStart) const;
  int HitRegionFlag(float x) const;
  void ZoomAt(float centerFrac, float factor);
  SlicePtr CurrentSlices() const;

  std::shared_ptr<const sampler::SampleData> mSample;
  std::function<SlicePtr()> mSliceSource;
  std::function<void(int&, int&)> mRegionSource;
  WaveEnvelope mEnv;
  double mRate = 44100.;
  float mVisStart = 0.f;
  float mVisEnd = 1.f;
  Grab mGrab = Grab::None;
  int mGrabIndex = -1;
  int mGrabRegion = -1;
  float mPanAnchorFrac = 0.f;
  float mPanAnchorX = 0.f;
  std::function<void(int, int)> mOnMove;
  std::function<void(int)> mOnAdd;
  std::function<void(int)> mOnRemove;
  std::function<void(int)> mOnToggleManual;
  std::function<void(int, int)> mOnRegion;
  std::function<void()> mOnHotSwap;
};

}  // namespace ui
}  // namespace keith
