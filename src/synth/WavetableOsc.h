#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

// Wavetable oscillator for Keith.
//
// A wavetable is a set of frames, each a single cycle of a waveform, stored as harmonic
// amplitudes. Storing harmonics rather than samples is what makes the morph work: between
// two frames we can move each harmonic independently and the result is still a valid
// single-cycle waveform that loops perfectly.
//
// Frames are held as harmonic spectra because that is also what the derivation pipeline
// produces -- an analysis of a source recording yields the harmonic content per frame -- so
// nothing has to be converted between what we measure and what we play.
//
// Antialiasing: a naive table read aliases badly above a few hundred Hz because the table's
// upper harmonics fold back. Each frame therefore carries enough harmonics for its intended
// pitch range, and the read picks the band-limited set for the note being played. This is
// the standard mip-map approach; it costs one lookup and removes the worst of the fold-back
// without a full oversampled oscillator.

namespace keith {
namespace synth {

/** Highest number of harmonics any frame may carry. */
constexpr int kWavetableMaxHarmonics = 256;

/** Samples per single cycle of the rendered table. Power of two so the read can mask. */
constexpr int kWavetableSize = 2048;
constexpr int kWavetableMask = kWavetableSize - 1;

/**
 * One frame of a wavetable: the harmonic amplitudes of a single cycle.
 *
 * `harmonic[0]` is the fundamental. Amplitudes are relative, so a frame is normalised when
 * it is rendered rather than when it is authored.
 */
struct WavetableFrame
{
  float harmonic[kWavetableMaxHarmonics] {};

  /** Set the frame from a list of amplitudes, zeroing everything above `count`. */
  void SetHarmonics(const float* amplitude, int count)
  {
    const int n = std::clamp(count, 0, kWavetableMaxHarmonics);
    for (int i = 0; i < kWavetableMaxHarmonics; ++i)
      harmonic[i] = i < n ? amplitude[i] : 0.f;
  }

  /** A single sine at the fundamental -- the neutral frame, and the default. */
  static WavetableFrame Sine()
  {
    WavetableFrame frame;
    frame.harmonic[0] = 1.f;
    return frame;
  }
};

/**
 * A bank of frames, morphing across them by a continuous position.
 *
 * The table renders each frame into a band-limited set of cycle buffers once, on load, so
 * playback is an interpolated table read rather than a sum of sines per sample. Rebuilding
 * happens on the message thread; the audio thread only reads.
 */
class WavetableBank
{
public:
  WavetableBank() = default;

  /** How many frames this bank holds. */
  int FrameCount() const { return static_cast<int>(mFrames.size()); }

  /** Replace the contents wholesale. Not real-time safe -- call from the message thread. */
  void SetFrames(const std::vector<WavetableFrame>& frames)
  {
    mFrames = frames;
    Rebuild();
  }

  /** The frames currently held, for serialisation. */
  const std::vector<WavetableFrame>& Frames() const { return mFrames; }

  /**
   * A sample from the bank at a given phase and morph position.
   *
   * `phase` is 0..1 within the cycle. `morph` is 0..frameCount-1: whole numbers land on a
   * frame, fractions cross-fade between two adjacent frames. `bandLimit` selects how many
   * harmonics are used, which the caller sets from the note's pitch.
   */
  float Read(float phase, float morph, int bandLimit) const
  {
    if (mRendered.empty())
      return 0.f;

    const int last = static_cast<int>(mRendered.size()) - 1;
    const float clamped = std::clamp(morph, 0.f, static_cast<float>(last));
    const int indexA = static_cast<int>(clamped);
    const int indexB = std::min(indexA + 1, last);
    const float blend = clamped - static_cast<float>(indexA);

    const int band = std::clamp(bandLimit, 1, kMipLevels);
    const float a = SampleRendered(indexA, band, phase);
    const float b = SampleRendered(indexB, band, phase);
    return a + (b - a) * blend;
  }

  /**
   * How many harmonics to use for a given fundamental, so the top harmonic stays under
   * Nyquist. The caller passes the note frequency and the sample rate.
   */
  static int BandLimitFor(float freqHz, double sampleRate)
  {
    if (freqHz <= 0.f || sampleRate <= 0.)
      return kMipLevels;
    const float nyquist = static_cast<float>(sampleRate * 0.5);
    const int allowed = static_cast<int>(nyquist / freqHz);
    // Map the allowed harmonic count onto a mip level. Each level halves the harmonic count,
    // so level 0 is the full set and the last level is one harmonic (a pure sine).
    int level = 0;
    int cap = kWavetableMaxHarmonics;
    while (level < kMipLevels - 1 && allowed < cap)
    {
      cap /= 2;
      ++level;
    }
    return level;
  }

private:
  // Number of band-limited variants. Level 0 keeps all harmonics, each later level halves
  // the count, so a very high note falls back to a sine rather than aliasing.
  static constexpr int kMipLevels = 9;

  /** Render one frame at one mip level into a cycle buffer. */
  void RenderFrame(int frameIndex, int level, std::vector<float>& out) const
  {
    out.assign(kWavetableSize, 0.f);
    const int harmonics = std::max(1, kWavetableMaxHarmonics >> level);
    const WavetableFrame& frame = mFrames[static_cast<std::size_t>(frameIndex)];

    for (int h = 0; h < harmonics; ++h)
    {
      const float amplitude = frame.harmonic[h];
      if (amplitude == 0.f)
        continue;
      const float step = static_cast<float>(detail_twoPi()) * static_cast<float>(h + 1) /
                         static_cast<float>(kWavetableSize);
      for (int i = 0; i < kWavetableSize; ++i)
        out[static_cast<std::size_t>(i)] += amplitude * std::sin(step * static_cast<float>(i));
    }

    // Normalise to unit peak so morphing between frames does not jump in level. Without this
    // a frame with many strong harmonics would be far louder than a near-sine frame and the
    // morph would sound like a volume sweep.
    float peak = 0.f;
    for (float v : out)
      peak = std::max(peak, std::fabs(v));
    if (peak > 1e-6f)
    {
      const float gain = 1.f / peak;
      for (float& v : out)
        v *= gain;
    }
  }

  /** Rendered buffers, indexed [frame][level]. */
  void Rebuild()
  {
    mRendered.assign(mFrames.size(), std::vector<std::vector<float>>(kMipLevels));
    for (int f = 0; f < static_cast<int>(mFrames.size()); ++f)
      for (int l = 0; l < kMipLevels; ++l)
        RenderFrame(f, l, mRendered[static_cast<std::size_t>(f)][static_cast<std::size_t>(l)]);
  }

  /** Linear-interpolated read from a rendered buffer. */
  float SampleRendered(int frameIndex, int level, float phase) const
  {
    const std::vector<float>& buffer = mRendered[static_cast<std::size_t>(frameIndex)]
                                                [static_cast<std::size_t>(level)];
    const float pos = phase * static_cast<float>(kWavetableSize);
    const int i0 = static_cast<int>(pos) & kWavetableMask;
    const int i1 = (i0 + 1) & kWavetableMask;
    const float frac = pos - std::floor(pos);
    const float a = buffer[static_cast<std::size_t>(i0)];
    const float b = buffer[static_cast<std::size_t>(i1)];
    return a + (b - a) * frac;
  }

  static constexpr double detail_twoPi() { return 6.28318530717958647692; }

  std::vector<WavetableFrame> mFrames;
  std::vector<std::vector<std::vector<float>>> mRendered;
};

/**
 * A wavetable oscillator voice: phase accumulation against a WavetableBank.
 *
 * Kept deliberately close to `Oscillator` -- same Reset / SetSampleRate / Process / Wrapped /
 * SyncReset surface -- so the two are interchangeable at the call site and a Voice can choose
 * between them without special cases.
 *
 * Hard sync needs care: the table is read at an arbitrary phase rather than the phase being
 * reset to zero, which is why SyncReset() is separate from Reset(). A wavetable read at a
 * non-zero phase is still continuous, so sync works, but it does not have the click of a
 * hard-reset saw.
 */
class WavetableOscillator
{
public:
  void Reset(float phase = 0.f)
  {
    mPhase = phase - std::floor(phase);
    mWrapped = false;
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 44100.;
  }

  /** Point the oscillator at a bank. Not real-time safe; call when the patch changes. */
  void SetBank(const WavetableBank* bank) { mBank = bank; }

  /**
   * One sample.
   *
   * `freqHz` is the note pitch. `morph` walks the frames. The band limit is derived from the
   * pitch each sample, which is cheap (a compare and a shift) and stays correct under pitch
   * modulation -- a fixed limit set at note-on would alias as soon as an envelope swept the
   * pitch up.
   */
  float Process(float freqHz, float morph)
  {
    if (!mBank || mBank->FrameCount() == 0)
      return 0.f;

    freqHz = std::clamp(freqHz, 1.f, static_cast<float>(mSampleRate * 0.45));
    const float incr = freqHz / static_cast<float>(mSampleRate);
    mPhase += incr;
    mWrapped = false;
    if (mPhase >= 1.f)
    {
      mPhase -= std::floor(mPhase);
      mWrapped = true;
    }

    const int band = WavetableBank::BandLimitFor(freqHz, mSampleRate);
    return mBank->Read(mPhase, morph, band);
  }

  /** True when the last Process() crossed the phase wrap; the hard-sync master signal. */
  bool Wrapped() const { return mWrapped; }

  /** Restart the cycle; used by note-on. */
  void SyncReset() { mPhase = 0.f; }

private:
  double mSampleRate = 44100.;
  float mPhase = 0.f;
  bool mWrapped = false;
  const WavetableBank* mBank = nullptr;
};

} // namespace synth
} // namespace keith
