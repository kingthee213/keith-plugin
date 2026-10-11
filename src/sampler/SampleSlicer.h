#pragma once

// Transient detection and slice markers for the sampler.
//
// A "slice" is a region of the sample between two markers. Markers are detected automatically
// from the audio and then moved by the user, so a slice boundary can be nudged to sit exactly
// on the hit rather than where the algorithm guessed.
//
// ## How the detection works, and why this method
//
// The detector is spectral-flux-like but computed on a cheap time-domain proxy: a short-window
// envelope, differenced between frames, with a threshold that adapts to how loud the material
// is. Onset detection on percussive material is mostly "where does the energy jump", and a
// half-wave-rectified envelope difference finds that without an FFT per frame.
//
// The three constants that matter, all exposed as parameters rather than buried:
//
//   - `sensitivity` moves the threshold against the material's own peak, so a quiet break and a
//     loud one both segment sensibly without retuning.
//   - `minSpacingMs` stops a single hit being reported as three onsets a few samples apart.
//     Real percussive attacks have a short noisy rise that looks like several onsets.
//   - `windowMs` sets how much audio each energy reading covers. Too short and it tracks noise;
//     too long and two fast hits merge into one onset.
//
// Detection runs on the message thread when a sample is loaded and when the user asks for it.
// The results are a plain vector of frame indices, which the audio thread can read as a plain
// array -- there is no detection on the audio thread.

#include "SamplerVoice.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace keith {
namespace sampler {

/** A slice: a start frame, and where on the keyboard it is triggered from. */
struct Slice
{
  int startFrame = 0;
  /** MIDI note that triggers this slice. Assigned from the root key upward. */
  int midiNote = 36;
  /** True when the user placed or edited this boundary; auto-detected slices are false. */
  bool manual = false;

  /**
   * The last frame this slice sounds, given the whole slice list.
   *
   * A slice has no stored end: it ends one frame before the next slice starts, and the final
   * slice ends with the sample. Storing an end would let the two disagree after a marker drag,
   * so it is derived instead -- and derived in one place, rather than at each caller.
   *
   * @param slices     the full list this slice belongs to
   * @param index      this slice's position in that list
   * @param totalFrames the sample length, for the last slice
   */
  int EndFrame(const std::vector<Slice>& slices, std::size_t index, int totalFrames) const
  {
    const std::size_t next = index + 1;
    const int end = (next < slices.size()) ? slices[next].startFrame - 1 : totalFrames - 1;
    // Never before the start: a corrupt or mid-drag pair must not invert the range.
    return std::max(startFrame, end);
  }
};

/** Detection settings. Defaults are tuned for drum breaks and vocal chops, not for drones. */
struct SliceSettings
{
  /** 0..1. Higher finds more onsets. 0.5 is a sensible middle. */
  float sensitivity = 0.5f;
  /** Minimum gap between onsets. Below about 20 ms a single hit reports as several. */
  float minSpacingMs = 40.f;
  /** How quickly the envelope follows a rise. Short, so an attack is not smoothed away. */
  float attackMs = 1.f;
  /** How quickly it falls. Long enough that a decay does not read as a new onset. */
  float releaseMs = 20.f;
  /** Span of the moving reference level. */
  float localMs = 250.f;
  /** Frame hop between energy readings. */
  float hopMs = 5.f;
};

struct SliceResult
{
  std::vector<Slice> slices;
  bool ok = false;
  /** Onsets found before min-spacing thinning, for an honest report. */
  int rawOnsets = 0;
};

namespace detail {

/** One-pole envelope follower over the rectified mono signal. */
inline std::vector<float> FollowEnvelope(const SampleData& sample, double sampleRate,
                                        float attackMs, float releaseMs)
{
  const double at = std::exp(-1.0 / std::max(1.0, sampleRate * attackMs / 1000.0));
  const double rt = std::exp(-1.0 / std::max(1.0, sampleRate * releaseMs / 1000.0));
  std::vector<float> env(static_cast<std::size_t>(sample.frames), 0.f);
  double prev = 0.;
  for (int i = 0; i < sample.frames; ++i)
  {
    const double x = std::fabs(static_cast<double>(sample.MonoAt(i)));
    const double coef = (x > prev) ? at : rt;
    prev = x + coef * (prev - x);
    env[static_cast<std::size_t>(i)] = static_cast<float>(prev);
  }
  return env;
}

} // namespace detail

/**
 * Find onsets in the sample.
 *
 * ## Why an envelope follower rather than a windowed energy
 *
 * The first attempt measured mean amplitude over a fixed 10 ms window and differenced it. That
 * fails on steady tones: the window is not an integer number of cycles, so for a 220 Hz sine the
 * per-window mean ripples by 8% at the beat between window length and signal period, and every
 * ripple looks like an onset. A 2-second tone produced 40 of them.
 *
 * A one-pole follower has no window to misalign, so a steady tone gives a steady envelope and
 * the flux is genuinely flat.
 *
 * ## Why the threshold needs both an absolute floor and a local reference
 *
 * A single threshold relative to the largest rise makes a quiet hit invisible -- an 8% hit
 * cannot clear a bar drawn at 35% of the loudest transient. But a purely local reference is
 * worse in the other direction: against a flat envelope the flux is near zero, the local level
 * is also near zero, and the comparison turns floating-point ripple into onsets. A steady tone
 * then reports 35 slices.
 *
 * So an onset must clear BOTH: an absolute floor (below it, nothing is happening) and a
 * multiple of the local average (so a quiet hit is judged against its own neighbourhood rather
 * than against the loudest thing in the sample).
 *
 * Detection runs on the message thread, at load time and on demand. Results are a plain vector
 * of frame indices, safe for the audio thread to read.
 */
inline SliceResult DetectTransients(const SampleData& sample, double sampleRate,
                                    const SliceSettings& settings)
{
  SliceResult result;

  if (sample.Empty() || sample.frames < 2 || sampleRate <= 0.)
  {
    result.slices.push_back(Slice {0, 36});
    return result;
  }

  const std::vector<float> env = detail::FollowEnvelope(sample, sampleRate, settings.attackMs,
                                                       settings.releaseMs);
  const int hop = std::max(1, static_cast<int>(sampleRate * settings.hopMs / 1000.0));
  const int blocks = sample.frames / hop;
  if (blocks < 3)
  {
    result.slices.push_back(Slice {0, 36});
    return result;
  }

  // Block-average the envelope at the hop. The follower's own spikiness is not useful, and
  // this also decouples the onset grid from the sample rate.
  std::vector<float> level(static_cast<std::size_t>(blocks), 0.f);
  for (int b = 0; b < blocks; ++b)
  {
    float sum = 0.f;
    const int begin = b * hop;
    for (int i = begin; i < begin + hop; ++i)
      sum += env[static_cast<std::size_t>(i)];
    level[static_cast<std::size_t>(b)] = sum / static_cast<float>(hop);
  }

  // Half-wave rectified difference: an onset is a rise, so a falling edge must not register.
  std::vector<float> flux(static_cast<std::size_t>(blocks), 0.f);
  float peakFlux = 0.f;
  for (int b = 1; b < blocks; ++b)
  {
    const float d = level[static_cast<std::size_t>(b)] - level[static_cast<std::size_t>(b - 1)];
    flux[static_cast<std::size_t>(b)] = std::max(0.f, d);
    peakFlux = std::max(peakFlux, flux[static_cast<std::size_t>(b)]);
  }

  if (peakFlux <= 0.f)
  {
    result.slices.push_back(Slice {0, 36});
    return result;
  }

  // Moving average of the flux: the local reference level.
  const int win = std::max(1, static_cast<int>(sampleRate * settings.localMs / 1000.0 / hop));
  std::vector<float> local(static_cast<std::size_t>(blocks), 0.f);
  float run = 0.f;
  for (int b = 0; b < blocks; ++b)
  {
    run += flux[static_cast<std::size_t>(b)];
    if (b >= win)
      run -= flux[static_cast<std::size_t>(b - win)];
    local[static_cast<std::size_t>(b)] = run / static_cast<float>(std::min(b + 1, win));
  }

  const float s = std::clamp(settings.sensitivity, 0.f, 1.f);
  const float relative = 2.4f - 1.7f * s;
  const float floorLevel = peakFlux * 0.06f;

  const int minSpacing = std::max(1, static_cast<int>(sampleRate * settings.minSpacingMs
                                                     / 1000.0));
  std::vector<int> onsets;
  onsets.push_back(0);  // the first slice always begins at the sample start
  int lastOnset = 0;

  for (int b = 1; b < blocks; ++b)
  {
    const float f = flux[static_cast<std::size_t>(b)];
    if (f < floorLevel)
      continue;
    if (f <= std::max(local[static_cast<std::size_t>(b)] * relative, floorLevel))
      continue;

    // A local maximum only: across a rise every frame qualifies, and without this one hit is
    // reported repeatedly.
    const float next = (b + 1 < blocks) ? flux[static_cast<std::size_t>(b + 1)] : 0.f;
    if (f < next)
      continue;

    const int frame = b * hop;
    if (frame - lastOnset < minSpacing)
      continue;

    onsets.push_back(frame);
    lastOnset = frame;
  }

  result.rawOnsets = static_cast<int>(onsets.size());
  for (std::size_t i = 0; i < onsets.size(); ++i)
    result.slices.push_back(Slice {onsets[i], 36 + static_cast<int>(i)});

  result.ok = result.slices.size() > 1;
  return result;
}

/**
 * The default slice layout: one slice covering the whole sample.
 *
 * Used before detection has run, so the sampler always has at least one triggerable region.
 */
inline std::vector<Slice> SingleSlice()
{
  return {Slice {0, 36}};
}

/**
 * Frame index where a slice ends -- the next slice's start, or the end of the sample.
 *
 * Slices are defined only by their start points, so the end is derived. Storing one number per
 * boundary rather than a start and an end is what makes a marker drag a single edit: moving a
 * boundary moves the end of the preceding slice and the start of the next in one motion, and
 * the two can never drift apart or overlap.
 */
inline int SliceEndFrame(const std::vector<Slice>& slices, std::size_t index, int totalFrames)
{
  if (index + 1 < slices.size())
    return slices[index + 1].startFrame;
  return totalFrames;
}

/** Find the slice containing a frame, or -1. */
inline int SliceAtFrame(const std::vector<Slice>& slices, int frame, int totalFrames)
{
  for (std::size_t i = 0; i < slices.size(); ++i)
  {
    const int end = SliceEndFrame(slices, i, totalFrames);
    if (frame >= slices[i].startFrame && frame < end)
      return static_cast<int>(i);
  }
  // A frame past the last boundary still belongs to the last slice.
  return slices.empty() ? -1 : static_cast<int>(slices.size()) - 1;
}

/**
 * Move one slice boundary by the user, keeping the set valid.
 *
 * A boundary may not cross its neighbours or leave the sample, and it must stay at least a few
 * frames from them so a slice cannot be zero-length. Clamped rather than rejected, so a drag
 * that goes too far stops at the limit instead of failing.
 */
inline void MoveBoundary(std::vector<Slice>& slices, std::size_t index, int newFrame,
                         int totalFrames, int minSliceFrames = 16)
{
  if (index == 0 || index >= slices.size())
    return;  // the first boundary is always frame 0

  const int lower = slices[index - 1].startFrame + minSliceFrames;
  const int upper = (index + 1 < slices.size())
                      ? slices[index + 1].startFrame - minSliceFrames
                      : totalFrames - 1;
  if (upper < lower)
    return;  // no room between the neighbours; leave it alone rather than corrupt the set
  slices[index].startFrame = std::clamp(newFrame, lower, upper);
}

/** Remove a boundary. Refuses the first one, which defines where the sample begins playing. */
inline void RemoveBoundary(std::vector<Slice>& slices, std::size_t index)
{
  if (index == 0 || index >= slices.size())
    return;
  slices.erase(slices.begin() + static_cast<long>(index));
}

/** Add a boundary at a frame, keeping the set sorted. Returns the new slice's index. */
inline int AddBoundary(std::vector<Slice>& slices, int frame, int totalFrames, int midiNote,
                       bool manual = true)
{
  if (slices.size() >= static_cast<std::size_t>(kMaxSliceKeys))
    return -1;
  frame = std::clamp(frame, 0, std::max(0, totalFrames - 1));
  auto it = std::lower_bound(slices.begin(), slices.end(), frame,
                             [](const Slice& s, int f) { return s.startFrame < f; });
  const int index = static_cast<int>(it - slices.begin());
  if (index > 0 && slices[static_cast<std::size_t>(index - 1)].startFrame == frame)
    return index - 1;
  if (index < static_cast<int>(slices.size()) && slices[static_cast<size_t>(index)].startFrame == frame)
    return index;
  slices.insert(it, Slice {frame, midiNote, manual});
  // Re-number MIDI notes from the root upward after any edit.
  for (std::size_t i = 0; i < slices.size(); ++i)
    slices[i].midiNote = kSliceLowNote + static_cast<int>(i);
  return index;
}

/** Evenly spaced boundaries across [regionStart, regionEnd), including regionStart at index 0. */
inline std::vector<Slice> RegionSlices(int regionStart, int regionEnd, int count, int totalFrames)
{
  count = std::clamp(count, 2, kMaxSliceKeys);
  regionStart = std::clamp(regionStart, 0, std::max(0, totalFrames - 1));
  regionEnd = std::clamp(regionEnd, regionStart + 1, totalFrames);
  const int span = std::max(1, regionEnd - regionStart);
  std::vector<Slice> out;
  out.reserve(static_cast<std::size_t>(count));
  for (int i = 0; i < count; ++i)
  {
    const int frame = regionStart + (i * span) / count;
    out.push_back(Slice {frame, kSliceLowNote + i, false});
  }
  return out;
}

/**
 * Re-run auto detection but keep manual boundaries.
 *
 * Manual slices survive a sensitivity change, which is why the editor colours them differently.
 */
inline std::vector<Slice> MergeDetectedWithManual(const std::vector<Slice>& detected,
                                                const std::vector<Slice>& previous, int totalFrames)
{
  std::vector<Slice> out = detected;
  for (std::size_t i = 1; i < previous.size(); ++i)
  {
    if (!previous[i].manual)
      continue;
    const int frame = previous[i].startFrame;
    bool found = false;
    for (const Slice& s : out)
    {
      if (s.startFrame == frame)
      {
        found = true;
        break;
      }
    }
    if (!found)
      AddBoundary(out, frame, totalFrames, kSliceLowNote, true);
  }
  return out;
}

} // namespace sampler
} // namespace keith
