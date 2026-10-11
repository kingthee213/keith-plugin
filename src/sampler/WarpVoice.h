#pragma once

#include "WarpStretcher.h"

// A sampler voice that plays through Bungee, following a warp map.
//
// ## The shape of a render
//
// One output block is produced by repeating a grain step until enough frames exist:
//
//     1. ask the warp map where in the sample we should be, for the current beat position
//     2. set request.position to that frame and request.speed to the segment's rate
//     3. specifyGrain -> the input frame range Bungee needs
//     4. hand those frames over, with mute counts where they run past the sample
//     5. synthesiseGrain -> output frames
//
// Position comes from the warp map rather than from accumulating a rate, which is what keeps the
// audio locked to the beat grid: drift cannot accumulate because every grain is placed by the
// map, not by integrating the previous grain's rate.
//
// ## Why the input is clipped rather than clamped
//
// Near the sample's start and end, Bungee asks for frames that do not exist. It handles that:
// `analyseGrain` takes `muteFrameCountHead` and `muteFrameCountTail`, and replaces those frames
// with silence. So the buffer is passed with the real frames plus counts, rather than the
// pointer being slid to a valid range -- sliding would move where the audio sits in time.

#include "SampleSlicer.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace keith {
namespace sampler {

/**
 * One warp voice.
 *
 * Constructed on the message thread (Bungee allocates in its constructor), rendered on the audio
 * thread. Never copied or moved once in use, because the audio thread holds a pointer to it.
 */
class WarpVoice
{
public:
  WarpVoice() = default;

  ~WarpVoice() = default;

  WarpVoice(const WarpVoice&) = delete;
  WarpVoice& operator=(const WarpVoice&) = delete;

  /**
   * Build the stretcher. Message thread only -- Bungee allocates here.
   *
   * Safe to call repeatedly: an existing stretcher is replaced, which is what happens when a
   * sample is reloaded or the sample rate changes.
   */
  void Prepare(int channelCount, int inputRate, int outputRate)
  {
    mChannels = std::clamp(channelCount, 1, kMaxChannels);
    mInputRate = inputRate > 0 ? inputRate : 44100;
    mOutputRate = outputRate > 0 ? outputRate : 44100;
    mStretcher = std::make_unique<Bungee::Stretcher<BungeeEdition>>(
        WarpRates(mInputRate, mOutputRate), mChannels);
    mPrepared = mStretcher && mStretcher->state != nullptr;
    mRequest = Bungee::Request {};
    mStarted = false;
  }

  bool Prepared() const { return mPrepared; }

  /** The output frames the stretcher can produce in one call, for buffer sizing. */
  int MaxInputFrameCount() const
  {
    return mPrepared ? mStretcher->maxInputFrameCount() : 0;
  }

  /**
   * Begin playback at a musical position.
   *
   * Runs `preroll` so the pipeline has a few grains of run-in before the requested position.
   * Without it the first milliseconds are weak, which on a sliced break reads as a missing
   * transient rather than as a bug.
   */
  void Start(const WarpMap& warp, double startBeat, const SampleData& sample)
  {
    if (!mPrepared)
      return;
    mWarp = &warp;
    mSample = &sample;
    mBeat = startBeat;
    mFramesRendered = 0;
    mRequest = Bungee::Request {};
    mRequest.position = warp.FrameAtBeat(startBeat);
    mRequest.speed = 1.0;
    mRequest.pitch = 1.0;
    mRequest.reset = true;
    mRequest.resampleMode = resampleMode_autoOut;
    mStretcher->preroll(mRequest);
    mStarted = true;
  }

  void Stop() { mStarted = false; }

  bool Active() const { return mStarted; }

  /** Frames produced since Start. Used to decide when the requested span has been covered. */
  intptr_t FramesRendered() const { return mFramesRendered; }

  /**
   * Render `frameCount` frames into per-channel output buffers.
   *
   * Message thread never calls this. Allocation-free apart from the scratch vectors, which are
   * sized on the first call and reused -- see ReserveScratch.
   */
  void Process(float* const* output, int frameCount)
  {
    if (!mPrepared || !mStarted || mWarp == nullptr || mSample == nullptr || frameCount <= 0)
      return;

    int written = 0;
    while (written < frameCount)
    {
      // Where the map says this grain should sit, in audio frames. Taken from the map each time
      // rather than accumulated, so timing cannot drift.
      const double frame = mWarp->FrameAtBeat(mBeat);
      mRequest.position = frame;
      mRequest.speed = mWarp->StretchAtBeat(mBeat);
      mRequest.pitch = 1.0;   // warp holds pitch; pitch shifting is a separate control
      mRequest.reset = false;

      const Bungee::InputChunk chunk = mStretcher->specifyGrain(mRequest);
      const int chunkFrames = chunk.end - chunk.begin;
      if (chunkFrames <= 0)
      {
        mStarted = false;
        break;
      }

      ReserveScratch(chunkFrames, frameCount);

      // Copy the chunk out of the sample. Frames outside the sample are counted as mute so
      // Bungee zeroes them, rather than being clipped away.
      const int total = mSample->frames;
      const int head = std::max(0, -chunk.begin);
      const int tail = std::max(0, chunk.end - total);
      const int firstReal = std::max(0, chunk.begin);
      const int lastReal = std::min(total, chunk.end);

      for (int ch = 0; ch < mChannels; ++ch)
      {
        float* dst = mScratch[static_cast<std::size_t>(ch)].data();
        // Silence the muted head.
        for (int i = 0; i < head && i < chunkFrames; ++i)
          dst[i] = 0.f;
        // The real frames.
        const auto& src = mSample->channels[static_cast<std::size_t>(
            std::min<std::size_t>(static_cast<std::size_t>(ch), mSample->channels.size() - 1))];
        for (int i = firstReal; i < lastReal; ++i)
        {
          const int out = i - chunk.begin;
          if (out >= 0 && out < chunkFrames)
            dst[out] = src[static_cast<std::size_t>(i)];
        }
        // Silence the muted tail.
        for (int i = chunkFrames - tail; i < chunkFrames; ++i)
          if (i >= 0)
            dst[i] = 0.f;
      }

      // analyseGrain takes per-channel input as channel c at data[c * channelStride], so the
      // scratch channels have to be one flat block too. They are separate vectors, so the
      // stride is the chunk length and channel 0's pointer is the base.
      mInputBlock.resize(static_cast<std::size_t>(chunkFrames)
                         * static_cast<std::size_t>(mChannels));
      for (int ch = 0; ch < mChannels; ++ch)
        std::copy_n(mScratch[static_cast<std::size_t>(ch)].data(), chunkFrames,
                    mInputBlock.begin() + static_cast<long>(ch) * chunkFrames);

      mStretcher->analyseGrain(mInputBlock.data(), static_cast<intptr_t>(chunkFrames),
                               head, tail);

      // Bungee writes into a flat block, with channel `n` starting at data[n * channelStride].
      // So the output must be presented as one contiguous region with the stride set to the
      // total frame capacity -- not as a per-channel pointer array.
      const int remaining = frameCount - written;
      mOutput.data = mOutputBlock.data() + written;
      mOutput.frameCount = remaining;
      mOutput.channelStride = static_cast<intptr_t>(frameCount);
      mStretcher->synthesiseGrain(mOutput);

      const int produced = mOutput.frameCount;
      if (produced <= 0)
      {
        mStarted = false;
        break;
      }

      // Bungee wrote into mOutputBlock; copy out to the caller's per-channel buffers.
      for (int ch = 0; ch < mChannels; ++ch)
      {
        const float* src = mOutputBlock.data() + static_cast<long>(ch) * frameCount + written;
        float* dst = output[ch] + written;
        std::copy_n(src, produced, dst);
      }

      written += produced;
      mFramesRendered += produced;

      // Advance musical position by however much host time the produced frames represent.
      mBeat += mWarp->BeatAtFrame(frame + produced) - mWarp->BeatAtFrame(frame);
    }
  }

private:
  static constexpr int kMaxChannels = 2;

  /** Size the scratch buffers for a chunk, once. */
  void ReserveScratch(int frames, int outputFrames)
  {
    const std::size_t want = static_cast<std::size_t>(std::max(1, frames));
    if (mScratch.size() != static_cast<std::size_t>(mChannels))
      mScratch.assign(static_cast<std::size_t>(mChannels), std::vector<float>());
    for (auto& c : mScratch)
      if (c.size() < want)
        c.resize(want);

    // The output block is one flat buffer holding every channel back to back, strided by the
    // frame capacity -- see the note where mOutput is filled.
    const std::size_t outWant =
        static_cast<std::size_t>(std::max(1, outputFrames)) * static_cast<std::size_t>(mChannels);
    if (mOutputBlock.size() < outWant)
      mOutputBlock.resize(outWant);
  }

  std::unique_ptr<Bungee::Stretcher<BungeeEdition>> mStretcher;
  Bungee::Request mRequest {};
  Bungee::OutputChunk mOutput {};

  const WarpMap* mWarp = nullptr;
  const SampleData* mSample = nullptr;

  std::vector<std::vector<float>> mScratch;
  /** Flat output block; channel n lives at [n * frameCapacity]. */
  std::vector<float> mOutputBlock;
  /** Flat input block handed to analyseGrain; channel c at [c * chunkFrames]. */
  std::vector<float> mInputBlock;

  double mBeat = 0.;
  int mChannels = 1;
  int mInputRate = 44100;
  int mOutputRate = 44100;
  intptr_t mFramesRendered = 0;
  bool mPrepared = false;
  bool mStarted = false;
};

} // namespace sampler
} // namespace keith
