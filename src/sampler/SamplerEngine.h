// Drop target for sample files, and the engine-side state that holds them.
//
// Two things live here:
//
//   - `SamplerEngine`: the plugin-level sampler state. Holds the loaded sample, owns the
//     voices, and is the only place a sample pointer is published. Lives on `Keith`.
//   - `KeithSampleDropZone`: a transparent control covering the editor that accepts a dragged
//     audio file and asks the engine to load it.
//
// ## Why the drop is a control rather than the plugin class
//
// iPlug2 routes a file drop to whichever `IControl` is under the cursor, calling the control's
// `OnDrop(const char*)`. There is a `IGraphics::OnDrop(str, x, y)` that does the routing. A
// transparent control spanning the editor is therefore the simplest way to accept a drop
// anywhere in the window, and it keeps the load path out of `Keith` itself.

#pragma once

#include "SampleLoader.h"
#include "SamplerVoice.h"

#include <string>

namespace keith {
namespace sampler {

/** How many sampler voices the engine runs. */
constexpr int kSamplerVoices = 8;

/**
 * The plugin's sampler state.
 *
 * Owned by `Keith`. The audio thread reaches it through a pointer, so the rules in
 * SamplerVoice.h apply: nothing here allocates or touches disk once a sample is loaded.
 */
class SamplerEngine
{
public:
  SamplerEngine()
  {
    for (auto& v : mVoices)
      v.SetSampleRate(44100.);
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
    for (auto& v : mVoices)
      v.SetSampleRate(mSampleRate);
  }

  /**
   * Load a file and publish it to every voice.
   *
   * Called from the message thread only -- this reads from disk and allocates. The voices take
   * their own shared_ptr, so a render already in flight keeps the previous sample alive until
   * it completes and nothing is freed underneath the audio thread.
   *
   * Returns None on success; the error otherwise, for the caller to show.
   */
  LoadError Load(const std::string& path)
  {
    LoadError err = LoadError::None;
    auto sample = LoadFile(path, &err);
    if (!sample)
    {
      mLastError = err;
      mLastPath = path;
      return err;
    }

    mSample = sample;
    mLastPath = path;
    mLastError = LoadError::None;
    for (auto& v : mVoices)
      v.SetSample(std::move(sample));
    return LoadError::None;
  }

  /** True when a sample is loaded and ready to play. */
  bool HasSample() const { return mSample && !mSample->Empty(); }

  /** The loaded sample, for the editor to draw. Null when nothing is loaded. */
  const std::shared_ptr<const SampleData>& Sample() const { return mSample; }

  /** The last path asked for, whether or not it loaded. */
  const std::string& LastPath() const { return mLastPath; }

  /** Why the last load failed, if it did. */
  LoadError LastError() const { return mLastError; }

  /**
   * Start the next free voice on the loaded sample.
   *
   * Round-robin over the voice pool rather than a free-list: at eight voices and with a
   * one-shot sample, stealing the oldest is indistinguishable from finding a free one, and it
   * avoids a scan on the audio thread.
   */
  void Trigger(float velocity)
  {
    if (!HasSample())
      return;
    mVoices[static_cast<std::size_t>(mNextVoice % kSamplerVoices)].Trigger(velocity);
    ++mNextVoice;
  }

  void StopAll()
  {
    for (auto& v : mVoices)
      v.Stop();
  }

  /** Sum the voices. One mono sample. Called from the audio thread. */
  float Process()
  {
    float sum = 0.f;
    for (auto& v : mVoices)
      sum += v.Process();
    return sum;
  }

  /** Any voice still sounding? Used to decide whether the UI needs repainting. */
  bool AnyActive() const
  {
    for (const auto& v : mVoices)
      if (v.Active())
        return true;
    return false;
  }

private:
  std::shared_ptr<const SampleData> mSample;
  SamplerVoice mVoices[kSamplerVoices];
  std::string mLastPath;
  LoadError mLastError = LoadError::None;
  double mSampleRate = 44100.;
  int mNextVoice = 0;
};

} // namespace sampler
} // namespace keith
