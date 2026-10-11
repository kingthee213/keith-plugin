#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>

// Sampler: load an audio file and play it back.
//
// Stage 1 of the sampler work -- load and play, nothing else. Pitch across the keyboard and
// loop points are the next stages and layer on top of this.
//
// ## The threading rule this file exists to respect
//
// The audio thread must never allocate, never lock, and never read from disk. So the split is:
//
//   - `LoadFile` runs on the message thread. It decodes into a `SampleData`, which is
//     immutable once built. It may allocate freely.
//   - `SamplerVoice` holds a `std::shared_ptr<const SampleData>` and reads it on the audio
//     thread. Handing a sample to a voice is a shared_ptr copy, which is the ONE allocation
//     that could happen on the audio thread -- so assignment is done on the message thread by
//     `SetSample`, never inside the render loop.
//   - Swapping which sample a voice plays is done by loading the new one first, then
//     publishing the pointer. An in-flight render keeps the old sample alive through its own
//     shared_ptr, so the swap cannot free memory out from under the audio thread.
//
// This is the standard "publish an immutable buffer" pattern, and it is why SampleData has no
// mutating methods at all.

namespace keith {
namespace sampler {

/** Decoded audio, in the file's own sample rate, deinterleaved to one vector per channel. */
struct SampleData
{
  std::vector<std::vector<float>> channels;  // channels[ch][frame]
  double sampleRate = 44100.;
  int frames = 0;
  /** The file this came from, for display. Not used for playback. */
  std::string name;

  int ChannelCount() const { return static_cast<int>(channels.size()); }
  bool Empty() const { return frames <= 0 || channels.empty(); }

  /** Mono-summed view, used when a mono voice reads a stereo file. */
  float MonoAt(int frame) const
  {
    if (frame < 0 || frame >= frames)
      return 0.f;
    float sum = 0.f;
    for (const auto& ch : channels)
      sum += ch[static_cast<std::size_t>(frame)];
    return channels.empty() ? 0.f : sum / static_cast<float>(channels.size());
  }
};

/** What a load attempt produced. Errors are value-typed so the caller can show a reason. */
enum class LoadError
{
  None = 0,
  FileNotFound,
  Unreadable,
  UnsupportedFormat,
  Empty,
  TooLarge
};

inline const char* LoadErrorText(LoadError e)
{
  switch (e)
  {
    case LoadError::None:              return "ok";
    case LoadError::FileNotFound:      return "file not found";
    case LoadError::Unreadable:        return "could not be read";
    case LoadError::UnsupportedFormat: return "unsupported format";
    case LoadError::Empty:             return "no audio in file";
    case LoadError::TooLarge:          return "file too large to embed";
  }
  return "unknown";
}

/**
 * A ceiling on decoded sample memory, per sample.
 *
 * The owner chose embedded samples so patches are self-contained. That is fine for a drum
 * hit and gets heavy fast for a long pad: an 8-second stereo file at 44.1 kHz is 1.4 MB, and
 * a four-voice layered pad would put several MB into every save. Hosts cap state size and
 * large states slow every project save, so this refuses the file rather than silently making
 * a patch that breaks a session.
 *
 * 8 MB decoded is about 45 seconds of stereo 44.1 kHz, or 90 seconds of mono. Generous for a
 * one-shot or a loop, and far below anything that would trouble a host.
 */
constexpr std::size_t kMaxDecodedBytes = 8u * 1024u * 1024u;

/**
 * A sampler voice: one playback position over one sample.
 *
 * Stage 1 plays at the file's own pitch. `SetPitchRatio` is present and used, but the keyboard
 * mapping that computes the ratio from a MIDI note is stage 2 -- this stage only needs the
 * playback path to be correct, including a fractional position and linear interpolation,
 * because pitch shifting without interpolation would sound broken the moment it is switched
 * on.
 */
class SamplerVoice
{
public:
  /** Publish a sample to this voice. Call from the message thread. */
  void SetSample(std::shared_ptr<const SampleData> sample)
  {
    mSample = std::move(sample);
    mPosition = 0.;
    mActive = false;
  }

  /** The sample currently held, or null. Safe to call from the message thread for display. */
  std::shared_ptr<const SampleData> Sample() const { return mSample; }

  void SetSampleRate(double sr) { mSampleRate = sr > 1. ? sr : 44100.; }

  /** Start playback from the beginning. */
  void Trigger(float velocity)
  {
    if (!mSample || mSample->Empty())
      return;
    mVelocity = std::clamp(velocity, 0.f, 1.f);

    // Reversed starts at the END of the range and walks back. The range is resolved here rather
    // than in Process because the start point has to be right from the first sample -- starting at
    // mStartFrame and correcting on the first Process call would emit one frame from the wrong end.
    // The frame counter, from which the read position is DERIVED. See PositionForFrame().
    mFrameIndex = 0;
    // The step and the frame count are derived ONCE here, at trigger time, and cached.
    //
    // Neither can change while the note sounds: pitch ratio, range and direction are all set
    // before Trigger() and never touched mid-note (Trigger() in SamplerEngine is the only caller
    // of SetPitchRatio/SetRange/SetReversed, and it sets all three before triggering). Computing
    // them per output sample instead would put two divisions and a std::ceil() in the audio loop
    // for no benefit.
    mCachedStep = StepPerFrame();
    mCachedFrames = FrameCount();
    mPosition = PositionForFrame(0);
    mActive = true;
  }

  void Stop() { mActive = false; }

  bool Active() const { return mActive; }

  /**
   * Playback rate against the file's own rate.
   *
   * 1.0 plays at the recorded pitch. Stage 2 sets this from the note being played and the
   * sample's root key; stage 1 leaves it at 1.
   */
  void SetPitchRatio(double ratio) { mPitchRatio = std::clamp(ratio, 0.01, 32.0); }

  /**
   * One mono sample.
   *
   * Returns 0 when inactive, so a caller can sum this unconditionally into a mix. Playback
   * stops at the end of the data -- stage 3 adds looping; without it, a one-shot just ends.
   */
  float Process()
  {
    if (!mActive || !mSample || mSample->Empty())
      return 0.f;

    // The playable range. For a whole-sample voice this is the entire file; for a slice it is
    // just that slice. mEndFrame is inclusive, so a slice from frame 1000 to 5000 stops at 5000
    // rather than running on to the end of the sample -- which is the difference between playing
    // a slice and playing the rest of the file from that point.
    const int lastFrame = ResolvedEndFrame();

    // The range is exhausted when the frame counter reaches its length. Because the counter is
    // direction-agnostic, this one test stops both forward and reversed playback -- they differ
    // only in how PositionForFrame() maps the counter to a position.
    if (mFrameIndex >= mCachedFrames)
    {
      mActive = false;
      return 0.f;
    }

    // Linear interpolation between the two frames either side of the position. Without this a
    // pitch-shifted sample has audible quantisation, and the aliasing gets worse the further
    // the ratio is from 1.
    const double x = mPosition;
    const int i0 = std::min(static_cast<int>(x), lastFrame);
    // Clamped to lastFrame, not the file end: interpolating past a slice's end would read the
    // first frame of the next slice and bleed it in.
    const int i1 = std::min(i0 + 1, lastFrame);
    const float frac = static_cast<float>(x - static_cast<double>(i0));

    const float a = mSample->MonoAt(i0);
    const float b = mSample->MonoAt(i1);
    const float value = a + (b - a) * frac;

    // Advance the frame counter and derive the next position from it, rather than accumulating a
    // position. Two reasons, and the second is the one that forced the change:
    //
    //   * Accumulation drifts. Adding a ratio like 1.059463 a few hundred thousand times
    //     accumulates rounding error, so a long sample ends a frame or two off where it should.
    //   * Forward and reversed must cover EXACTLY the same frames. Accumulating downward from the
    //     end and upward from the start does the same arithmetic in a different order, and the
    //     two sequences disagreed by one frame at non-unity ratios. Deriving both from one counter
    //     makes reversed the forward sequence reversed, by construction.
    ++mFrameIndex;
    mPosition = PositionForFrame(mFrameIndex);

    return value * mVelocity;
  }

  /** Restrict playback to [startFrame, endFrame], inclusive. endFrame < 0 means to the end. */
  void SetRange(int startFrame, int endFrame)
  {
    mStartFrame = std::max(0, startFrame);
    mEndFrame = endFrame;
    mPosition = static_cast<double>(mStartFrame);
  }

  /** The frames this voice will play, for a caller that wants to know its reach. */
  int StartFrame() const { return mStartFrame; }
  int EndFrame() const { return mEndFrame; }

  /**
   * Play the range backwards.
   *
   * A direction flag rather than a second reversed copy of the sample: no extra memory, no rebuild
   * on load, and it works per-voice so it can be switched while a note is still sounding. The
   * read is the same interpolation either way -- only the step and the stop condition change.
   */
  void SetReversed(bool reversed) { mReversed = reversed; }
  bool Reversed() const { return mReversed; }

private:
  /** How many frames this voice will emit over its range. */
  int FrameCount() const
  {
    const double step = StepPerFrame();
    const double span = (static_cast<double>(ResolvedEndFrame()) + 1.0)
                          - static_cast<double>(mStartFrame);
    if (step <= 0.0 || span <= 0.0)
      return 0;
    // Ceiling, because a partial final frame still sounds. The epsilon keeps a span that is an
    // exact multiple of the step from rounding up to one extra frame.
    const int n = static_cast<int>(std::ceil(span / step - 1e-9));
    return std::max(0, n);
  }

  /** Frames advanced per output sample: the pitch ratio against the file's own rate. */
  double StepPerFrame() const
  {
    if (!mSample || mSampleRate <= 0.0)
      return 0.0;
    return mPitchRatio * (mSample->sampleRate / mSampleRate);
  }

  /**
   * The read position for frame `n` of the range.
   *
   * Both directions use this one function, which is what guarantees they cover the same frames:
   * forward walks n upward, reversed walks it downward, and the arithmetic is identical either
   * way. Reversed is therefore the forward sequence in reverse, not an independent calculation
   * that has to be kept in step.
   */
  double PositionForFrame(int n) const
  {
    const int index = mReversed ? (mCachedFrames - 1 - n) : n;
    return static_cast<double>(mStartFrame) + static_cast<double>(index) * mCachedStep;
  }

  /** mEndFrame resolved against the sample, clamped so it can never point past the data. */
  int ResolvedEndFrame() const
  {
    const int fileLast = mSample ? mSample->frames - 1 : 0;
    return std::min(mEndFrame < 0 ? fileLast : mEndFrame, fileLast);
  }

  std::shared_ptr<const SampleData> mSample;
  int mStartFrame = 0;
  int mEndFrame = -1;   // -1: play to the end of the sample
  bool mReversed = false;
  int mFrameIndex = 0;
  // Derived at trigger time from the sample, the pitch ratio and the range; see Trigger().
  double mCachedStep = 1.0;
  int mCachedFrames = 0;
  double mSampleRate = 44100.;
  double mPosition = 0.;
  double mPitchRatio = 1.0;
  float mVelocity = 1.f;
  bool mActive = false;
};

} // namespace sampler
} // namespace keith
