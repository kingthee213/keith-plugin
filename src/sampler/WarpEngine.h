#pragma once

#include "SampleSlicer.h"
#include "WarpMap.h"
#include "WarpVoice.h"

// The warp engine: decides when to render through the stretcher, and where.
//
// ## Two playback paths, chosen by one switch
//
//   - **Plain.** Read the sample at its own rate. Cheap, exact, no latency. What a one-shot and
//     an unwarped loop want.
//   - **Warped.** Render through Bungee, positioned by the warp map. Stretch and pitch become
//     independent, and slices land on the beat grid.
//
// The switch is `warp.enabled`. The engine picks the path per block, so toggling it does not
// rebuild anything.
//
// ## Tempo sync
//
// When the host is running, a slice fires when the transport reaches that slice's beat. The
// engine converts the transport's musical position into a beat within the sample:
//
//     beatInSample = transportBeat - sliceStartBeat + slice.beat
//
// and asks the warp map which frame that is. So the slice plays at the host's tempo rather than
// its recorded one, and it stays there as the tempo changes.
//
// ## Latency, and why it is a parameter rather than a constant
//
// Bungee's pipeline delays its output by some number of frames. Left uncompensated, every slice
// fires late by that amount -- a drift with no visible cause in the code.
//
// The number is NOT known yet. Bungee's C table has no latency query, the measurement is written
// but has not run, and this machine cannot compile C++ to run it. So the value lives in
// `SetLatencyCompensationFrames` with a default of zero, and the measurement in
// tools/warp_latency_test.cpp is what fills it in.
//
// Zero is the honest default: it means "not compensated", which is wrong by a known amount
// rather than wrong by a made-up amount. Do not replace it with a guess -- run the test.

#include <algorithm>
#include <cmath>
#include <vector>

namespace keith {
namespace sampler {

/** Which path a block used, for the editor to report. */
enum class WarpMode
{
  Plain = 0,
  Warped
};

class WarpEngine
{
public:
  WarpEngine() = default;

  /** Message thread. Builds the stretcher, which allocates. */
  void Prepare(int channelCount, int sampleRate)
  {
    mChannels = std::clamp(channelCount, 1, 2);
    mSampleRate = sampleRate > 0 ? sampleRate : 44100;
    mVoice.Prepare(mChannels, mSampleRate, mSampleRate);
    mPrepared = true;
  }

  /**
   * The pipeline delay, in frames, to subtract when placing grains.
   *
   * Fill this from tools/warp_latency_test.cpp. Left at zero it is not compensated, which is a
   * known wrong answer rather than a guess pretending to be right.
   */
  void SetLatencyCompensationFrames(int frames)
  {
    mLatencyFrames = std::max(0, frames);
  }

  int LatencyCompensationFrames() const { return mLatencyFrames; }

  /** True when a stretcher exists and can be rendered through. */
  bool WarpAvailable() const { return mPrepared && mVoice.Prepared(); }

  /** The map the engine follows. Owned by the caller. */
  void SetWarpMap(const WarpMap* warp) { mWarp = warp; }
  void SetSample(const SampleData* sample) { mSample = sample; }

  /** Start a slice at a musical position within the sample. */
  void TriggerSlice(double beatInSample)
  {
    if (!WarpAvailable() || mWarp == nullptr || mSample == nullptr)
      return;
    mVoice.Start(*mWarp, beatInSample, *mSample);
  }

  void Stop()
  {
    mVoice.Stop();
  }

  bool Active() const { return mVoice.Active(); }

  /**
   * Which path a block will use.
   *
   * Warped needs a usable map: two markers and an enabled switch. Anything less falls back to
   * plain, which is the safe direction -- plain playback always works.
   */
  WarpMode ModeFor(const WarpMap& warp) const
  {
    return (WarpAvailable() && warp.Usable()) ? WarpMode::Warped : WarpMode::Plain;
  }

  /**
   * The beat within the sample that the host transport maps to.
   *
   * Returns -1 when the transport is not running, which the caller reads as "do not tempo-lock".
   */
  static double BeatInSample(const Transport& transport, double sliceStartBeat,
                             double sliceBeat)
  {
    if (!transport.Valid() || !transport.running)
      return -1.;
    return transport.ppqPosition - sliceStartBeat + sliceBeat;
  }

  /**
   * Render one block through the warped path.
   *
   * Writes into per-channel `output` buffers, `frameCount` frames. `output` must have two
   * channels allocated even when the sample is mono; the second is filled with the first.
   */
  void ProcessWarped(float* const* output, int frameCount)
  {
    if (frameCount <= 0 || output == nullptr)
      return;

    if (!Active())
      return;

    mVoice.Process(output, frameCount);

    // A mono sample gets written to both outputs so the engine's contract is the same either way.
    if (mChannels == 1)
    {
      std::copy_n(output[0], frameCount, output[1]);
    }
  }

private:
  WarpVoice mVoice;
  const WarpMap* mWarp = nullptr;
  const SampleData* mSample = nullptr;
  int mChannels = 2;
  int mSampleRate = 44100;
  int mLatencyFrames = 0;
  bool mPrepared = false;
};

} // namespace sampler
} // namespace keith
