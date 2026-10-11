#pragma once

// Tempo sync and warp markers.
//
// ## The two features this supports
//
// 1. **Tempo sync.** Slices fire on the beat, so a sliced break plays in time with the host
//    rather than at its recorded speed.
//
// 2. **Warp markers.** Each marker says "this moment in the audio should land at this moment in
//    musical time". The engine then stretches the audio between markers so each segment occupies
//    the musical time between its markers. This is the Ableton model, and it is what lets a
//    sampled loop be pulled onto a grid without fitting perfectly in the first place.
//
// They are one mechanism with a smaller and a larger use: a slice marker is a warp marker whose
// destination is simply the next beat. Implementing warp markers properly therefore covers tempo
// sync as a special case.
//
// ## How the stretch maths works
//
// Audio runs on a frame clock; music runs on a beat clock. A marker is a correspondence between
// the two:
//
//     marker  |  audio frame  |  beats from the start
//     --------+---------------+-----------------------
//        0    |       0       |        0.0
//        1    |    13230      |        1.0
//        2    |    27600      |        2.0
//
// Between two markers the relationship is linear: the audio from frame A to frame B must cover
// the beats from beat A to beat B. That gives a stretch ratio per segment:
//
// The playback rate for a segment is how much audio to read per host sample:
//
//     hostSamples = (beatB - beatA) * (60 / tempo) * sampleRate
//     rate        = (frameB - frameA) / hostSamples
//
// A rate of 1.0 plays the segment untouched. One segment may need a different rate from the
// next, which is what makes this a warp rather than a global tempo change.
//
// Note the sample rate is part of the formula and not optional: leave it out and the units are
// frames-per-beat against samples-per-beat, and the answer is wrong by the sample rate.
//
// ## Why the maths lives apart from the stretcher
//
// Computing "what ratio applies here, and what frame should be playing at this beat" is exact
// arithmetic and can be tested on its own. Whether the stretcher sounds good doing it is a
// separate and much harder question. Keeping them apart means the mapping can be proven correct
// before any audio runs through it -- which matters, because a subtly wrong mapping sounds like
// a rhythm that drifts rather than like an obvious bug.

#include "SampleSlicer.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace keith {
namespace sampler {

/** Tempo and transport, as read from the host each block. */
struct Transport
{
  double tempo = 120.;
  /** Musical position in quarter notes since the start. -1 when unknown. */
  double ppqPosition = -1.;
  bool running = false;
  /** Time signature, for converting beats to bars in the UI. */
  int beatsPerBar = 4;

  /** True when the host is giving us usable tempo and position. */
  bool Valid() const { return tempo > 1. && ppqPosition >= 0.; }

  /** Seconds per beat at the current tempo. */
  double SecondsPerBeat() const { return 60. / std::max(1., tempo); }
};

/** What a warp marker pins together. */
struct WarpMarker
{
  /** Where in the audio, in frames. */
  int frame = 0;
  /** Where in the music, in quarter notes from the clip's start. */
  double beats = 0.;
  /** True when the user placed this by hand rather than it coming from detection. */
  bool userPlaced = false;
};

/**
 * The warp map: markers, and the arithmetic that turns a musical position into a frame.
 *
 * Markers are kept sorted by frame. Nothing here allocates on the audio thread as long as the
 * marker set is not being edited -- editing happens on the message thread and the audio thread
 * reads a snapshot.
 */
class WarpMap
{
public:
  /** Enable warp/stretch. When off, the sample plays at its own rate and markers only slice. */
  bool enabled = false;
  /** Original tempo of the audio, in BPM. A loop recorded at 90 and played at 120 is stretched. */
  double sourceTempo = 120.;

  const std::vector<WarpMarker>& Markers() const { return mMarkers; }
  std::size_t Count() const { return mMarkers.size(); }

  /** True when there is enough to warp: at least two markers and a source tempo. */
  bool Usable() const { return enabled && mMarkers.size() >= 2 && sourceTempo > 1.; }

  void Clear() { mMarkers.clear(); }

  /**
   * Replace the markers from a set of slices, spacing them evenly on the beat grid.
   *
   * This is the "tempo sync" case: each slice lands on its own beat, so a detected break plays
   * in time. The spacing comes from the caller rather than being assumed to be one beat per
   * slice, because a break may have more slices than bars.
   */
  void FromSlices(const std::vector<Slice>& slices, double beatsPerSlice)
  {
    mMarkers.clear();
    mMarkers.reserve(slices.size());
    for (std::size_t i = 0; i < slices.size(); ++i)
      mMarkers.push_back(WarpMarker {slices[i].startFrame,
                                     static_cast<double>(i) * beatsPerSlice, false});
  }

  /** Add or replace a marker at a frame. Keeps the set sorted. */
  void SetMarker(int frame, double beats, bool userPlaced)
  {
    for (auto& m : mMarkers)
    {
      if (m.frame == frame)
      {
        m.beats = beats;
        m.userPlaced = userPlaced;
        return;
      }
    }
    mMarkers.push_back(WarpMarker {frame, beats, userPlaced});
    Sort();
  }

  /** Move a marker's musical position, as a drag on the warp view does. */
  void MoveMarkerBeat(std::size_t index, double beats)
  {
    if (index >= mMarkers.size())
      return;
    // Keep the musical order the same as the audio order; markers may not cross in beat space
    // any more than they may in frames, or a segment would have negative length.
    const double lower = (index == 0) ? -1e9 : mMarkers[index - 1].beats + 1e-6;
    const double upper = (index + 1 < mMarkers.size()) ? mMarkers[index + 1].beats - 1e-6 : 1e9;
    if (upper < lower)
      return;
    mMarkers[index].beats = std::clamp(beats, lower, upper);
    mMarkers[index].userPlaced = true;
  }

  /** Remove a marker. The first is kept: it anchors the map at the sample start. */
  void RemoveMarker(std::size_t index)
  {
    if (index == 0 || index >= mMarkers.size())
      return;
    mMarkers.erase(mMarkers.begin() + static_cast<long>(index));
  }

  /**
   * The stretch ratio in force at a musical position: beats per audio frame, inverted.
   *
   * Returns 1.0 when the map is not usable, which makes the caller's job simple -- a disabled
   * or empty map means plain playback with no branching at the call site.
   */
  double StretchAtBeat(double beats) const
  {
    if (!Usable() || mHostSampleRate <= 1.)
      return 1.0;
    const std::size_t seg = SegmentForBeat(beats);
    const WarpMarker& a = mMarkers[seg];
    const WarpMarker& b = mMarkers[seg + 1];
    const double audioFrames = static_cast<double>(b.frame - a.frame);
    const double beatSpan = b.beats - a.beats;
    if (audioFrames <= 0. || beatSpan <= 0.)
      return 1.0;

    // The segment holds `audioFrames` frames of audio, which must fill `beatSpan` quarter notes
    // of host time. Host time per beat is 60/tempo seconds, so the segment has
    //
    //     hostSamples = beatSpan * (60 / tempo) * sampleRate
    //
    // host samples to fill. The playback rate is how much audio to read per host sample, so
    //
    //     rate = audioFrames / hostSamples
    //
    // A rate of 1 plays the segment untouched. Below 1 stretches it out over more time; above 1
    // compresses it. The sample rate is not optional here: without it the units are frames per
    // beat against samples per beat and the result is out by a factor of the sample rate.
    const double hostSamples = beatSpan * (60.0 / std::max(1., mTempo)) * mHostSampleRate;
    if (hostSamples <= 0.)
      return 1.0;
    return audioFrames / hostSamples;
  }

  /** The host tempo the stretch is computed against. Set each block from the transport. */
  void SetTempo(double tempo) { mTempo = tempo > 1. ? tempo : 120.; }

  /** The output sample rate. Needed to turn a musical duration into a sample count. */
  void SetHostSampleRate(double sr) { mHostSampleRate = sr > 1. ? sr : 44100.; }

  /**
   * Which audio frame should be playing at a musical position.
   *
   * This is the inverse mapping and the one playback actually needs when syncing to the host:
   * the host says where the song is in beats, and this says where in the sample that lands.
   * Interpolates linearly inside the segment.
   */
  double FrameAtBeat(double beats) const
  {
    if (mMarkers.empty())
      return 0.;
    if (mMarkers.size() == 1 || beats <= mMarkers.front().beats)
      return static_cast<double>(mMarkers.front().frame);

    const std::size_t seg = SegmentForBeat(beats);
    const WarpMarker& a = mMarkers[seg];
    const WarpMarker& b = mMarkers[seg + 1];
    const double beatSpan = b.beats - a.beats;
    if (beatSpan <= 0.)
      return static_cast<double>(a.frame);
    const double t = (beats - a.beats) / beatSpan;
    return static_cast<double>(a.frame) + t * static_cast<double>(b.frame - a.frame);
  }

  /** Musical position of an audio frame -- the forward direction, for display. */
  double BeatAtFrame(double frame) const
  {
    if (mMarkers.empty())
      return 0.;
    if (mMarkers.size() == 1 || frame <= static_cast<double>(mMarkers.front().frame))
      return mMarkers.front().beats;

    std::size_t seg = mMarkers.size() - 2;
    for (std::size_t i = 0; i + 1 < mMarkers.size(); ++i)
    {
      if (frame >= static_cast<double>(mMarkers[i].frame)
          && frame <= static_cast<double>(mMarkers[i + 1].frame))
      {
        seg = i;
        break;
      }
    }
    const WarpMarker& a = mMarkers[seg];
    const WarpMarker& b = mMarkers[seg + 1];
    const double frameSpan = static_cast<double>(b.frame - a.frame);
    if (frameSpan <= 0.)
      return a.beats;
    const double t = (frame - static_cast<double>(a.frame)) / frameSpan;
    return a.beats + t * (b.beats - a.beats);
  }

  /** The musical length the whole map covers. */
  double TotalBeats() const
  {
    return mMarkers.empty() ? 0. : mMarkers.back().beats - mMarkers.front().beats;
  }

private:
  void Sort()
  {
    std::sort(mMarkers.begin(), mMarkers.end(),
              [](const WarpMarker& a, const WarpMarker& b) { return a.frame < b.frame; });
  }

  /** Index of the segment containing a beat position. Never returns the last marker. */
  std::size_t SegmentForBeat(double beats) const
  {
    std::size_t seg = 0;
    for (std::size_t i = 0; i + 1 < mMarkers.size(); ++i)
    {
      if (beats >= mMarkers[i].beats && beats < mMarkers[i + 1].beats)
      {
        seg = i;
        break;
      }
      seg = i;  // past the end: clamp to the last segment
    }
    return seg;
  }

  std::vector<WarpMarker> mMarkers;
  double mTempo = 120.;
  double mHostSampleRate = 44100.;
};

/**
 * Where a slice should fire, musically, when tempo-syncing.
 *
 * Slices are evenly spaced in beats unless a warp map overrides them, which is the whole point of
 * taking the map as an argument rather than computing beats from the slice index here.
 */
inline double SliceBeat(const WarpMap& warp, const std::vector<Slice>& slices, std::size_t index,
                        double beatsPerSlice)
{
  if (warp.Usable() && index < warp.Count())
    return warp.Markers()[index].beats;
  return static_cast<double>(index) * beatsPerSlice;
}

} // namespace sampler
} // namespace keith
