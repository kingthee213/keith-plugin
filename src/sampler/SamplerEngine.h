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
#include "SampleSlicer.h"
#include "WarpEngine.h"
#include "WarpMap.h"

#include <cmath>
#include <string>

namespace keith {
namespace sampler {

/** How many sampler voices the engine runs. */
constexpr int kSamplerVoices = 8;

/**
 * What the keyboard does to the loaded sample. Two plain buttons in the editor pick between them.
 *
 *   Play  -- the whole sample, chromatically. Middle C plays it at its recorded pitch and every
 *            other note transposes it, so the sample becomes an instrument across the keyboard.
 *            This is the sampler-as-synth behaviour.
 *
 *   Slice -- each key plays one slice, at its recorded pitch, and stops at that slice's end.
 *            This is the slicer behaviour: the keyboard becomes the pads.
 *
 * The two are genuinely different instruments rather than a setting, which is why the owner asked
 * for a button each instead of a toggle -- "a normal button (allows whole sample to play in
 * different pitch across keyboard) and a slice button to choose".
 */
enum class SampleMode
{
  Play = 0,   // whole sample, transposed across the keyboard
  Slice = 1   // one slice per key, recorded pitch, stops at the slice end
};

/** The note that plays a sample at its recorded pitch in Play mode. */
constexpr int kSampleRootNote = 60;   // middle C

/** The lowest key that maps to a slice. Slices are laid out upward from here. */
constexpr int kSliceLowNote = 36;     // matches the drum range, so the pads line up

/** How many slices the keyboard can reach. 36..96 is five octaves, far more than slicing makes. */
constexpr int kMaxSliceKeys = 60;

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
    // The stretcher is rebuilt here, on the message thread, because Bungee allocates in its
    // constructor. This is the one place a sample-rate change can safely reach it.
    mWarp.Prepare(2, static_cast<int>(mSampleRate));
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

    // A new sample invalidates the old slices and markers: their frames referred to the previous
    // file. Reset to one slice covering the whole thing, and let detection or the user fill it in.
    mSlices = SingleSlice();
    mWarpMap.Clear();
    mWarp.SetSample(mSample.get());
    mWarp.SetWarpMap(&mWarpMap);
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

  SampleMode Mode() const { return mMode; }
  void SetMode(SampleMode m) { mMode = m; }

  /** In Slice mode, which slice a key plays. -1 when the key is outside the slice range. */
  int SliceForNote(int note) const
  {
    const int index = note - kSliceLowNote;
    if (index < 0 || index >= static_cast<int>(mSlices.size()) || index >= kMaxSliceKeys)
      return -1;
    return index;
  }

  /**
   * A key was pressed. What it does depends on the mode, which is the whole point of the switch.
   *
   *   Play  -- transposes the whole sample. The ratio is the interval from middle C, so middle C
   *            plays at the recorded pitch and the sample behaves like an instrument.
   *   Slice -- plays the mapped slice at its RECORDED pitch (ratio 1) and stops at its end.
   *            Transposing it would defeat the purpose: a slice is played because of where it is,
   *            not to be re-pitched.
   *
   * @return true if a voice was started, so the caller knows the note was consumed.
   */
  bool TriggerNote(int note, float velocity)
  {
    if (!HasSample())
      return false;

    if (mMode == SampleMode::Play)
    {
      const double ratio = std::pow(2.0, (note - kSampleRootNote) / 12.0);
      Trigger(velocity, ratio, 0, -1);
      return true;
    }

    const int sliceIndex = SliceForNote(note);
    if (sliceIndex < 0)
      return false;

    const Slice& slice = mSlices[static_cast<std::size_t>(sliceIndex)];
    Trigger(velocity, 1.0, slice.startFrame,
            slice.EndFrame(mSlices, static_cast<std::size_t>(sliceIndex), mSample->frames));
    return true;
  }

  /**
   * Start the next free voice on the loaded sample.
   *
   * Round-robin over the voice pool rather than a free-list: at eight voices and with a
   * one-shot sample, stealing the oldest is indistinguishable from finding a free one, and it
   * avoids a scan on the audio thread.
   *
   * `endFrame` of -1 plays to the end of the sample, which is whole-sample playback.
   */
  void Trigger(float velocity, double pitchRatio = 1.0, int startFrame = 0, int endFrame = -1)
  {
    if (!HasSample())
      return;
    SamplerVoice& voice = mVoices[static_cast<std::size_t>(mNextVoice % kSamplerVoices)];
    voice.SetPitchRatio(pitchRatio);
    voice.SetRange(startFrame, endFrame);
    voice.Trigger(velocity);
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

  // --- warp and slicing ---------------------------------------------------------------
  //
  // Held here rather than in the DSP so the editor can edit them: the marker drags and the
  // detected slices both arrive from the message thread, and the audio thread only reads.

  WarpEngine& Warp() { return mWarp; }
  const WarpEngine& Warp() const { return mWarp; }

  WarpMap& Map() { return mWarpMap; }
  const WarpMap& Map() const { return mWarpMap; }

  const std::vector<Slice>& Slices() const { return mSlices; }

  /**
   * Run detection on the loaded sample and adopt the result.
   *
   * Message thread: detection allocates and reads the whole sample. The markers it produces are
   * then editable by the user.
   */
  SliceResult DetectSlices(const SliceSettings& settings)
  {
    mSlices = SingleSlice();
    if (!HasSample())
      return SliceResult {};
    SliceResult found = DetectTransients(*mSample, mSampleRate, settings);
    mSlices = found.slices;
    return found;
  }

  /** Move a slice boundary, as a drag on the waveform view does. */
  void MoveSliceBoundary(std::size_t index, int frame)
  {
    if (!HasSample())
      return;
    MoveBoundary(mSlices, index, frame, mSample->frames);
  }

  void SetSlices(std::vector<Slice> slices) { mSlices = std::move(slices); }

  /** Adopt the slices as an evenly spaced warp map, which is the tempo-sync case. */
  void TempoSyncFromSlices(double beatsPerSlice)
  {
    mWarpMap.enabled = true;
    mWarpMap.FromSlices(mSlices, beatsPerSlice);
  }

  /** Patch state that belongs here rather than in the DSP. */
  void SetTransport(const Transport& t)
  {
    mTransport = t;
    mWarpMap.SetTempo(t.tempo);
  }

  const Transport& CurrentTransport() const { return mTransport; }

private:
  std::shared_ptr<const SampleData> mSample;
  SamplerVoice mVoices[kSamplerVoices];
  std::vector<Slice> mSlices = SingleSlice();
  SampleMode mMode = SampleMode::Play;
  WarpMap mWarpMap;
  WarpEngine mWarp;
  Transport mTransport;
  std::string mLastPath;
  LoadError mLastError = LoadError::None;
  double mSampleRate = 44100.;
  int mNextVoice = 0;
};

} // namespace sampler
} // namespace keith
