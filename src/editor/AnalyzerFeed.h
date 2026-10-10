#pragma once

/**
 * Output spectrum for the editor's SPECTRAL ANALYZER panel.
 *
 * Threading contract, which is the whole reason this is its own header:
 *
 *   - The audio thread only calls PushFrame() once per output frame. It copies two floats
 *     into a fixed ring buffer and advances a write cursor. No allocation, no locks, no
 *     branches that depend on the UI, and no FFT. The FFT runs on whichever thread calls
 *     Snapshot().
 *   - The UI thread calls Snapshot() from the editor draw path. That hops the ring by a
 *     fixed window, windows it, runs the transform, and writes the magnitudes into a
 *     second buffer that only the UI touches.
 *
 * Snapshot() reads the ring behind the writer. It never takes a lock and never resizes
 * anything, so the worst case against the audio thread is that it reads a window the
 * writer is part-way through updating. The result is one frame of a slightly torn
 * spectrum, which is invisible at display rates and is the correct trade against ever
 * blocking or allocating on the audio thread.
 *
 * Trigger: the panel is meant to move on note-on, not to track the output continuously.
 * NoteOn()/NoteOff() set a counter the UI reads; the UI holds "armed" for a fixed decay so
 * a short percussive hit stays visible rather than vanishing between frames.
 */

#include <algorithm>
#include <cmath>
#include <cstring>

namespace keith {
namespace ui {

/** radix-2 in-place FFT. n must be a power of two. */
class Fft
{
public:
  static constexpr int kMaxSize = 2048;

  void Reset(int size)
  {
    mSize = std::min(std::max(size, 64), kMaxSize);
    int levels = 0;
    while ((1 << levels) < mSize)
      ++levels;
    mLevels = levels;
    // Twiddle tables only. The bit-reversal permutation is applied to the caller's buffers
    // in Forward(), because the Fft owns no signal storage of its own.
    for (int half = 1; half < mSize; half <<= 1)
    {
      const double step = -3.14159265358979323846 / half;
      for (int i = 0; i < half; ++i)
      {
        mCos[i] = static_cast<float>(std::cos(step * i));
        mSin[i] = static_cast<float>(std::sin(step * i));
      }
    }
  }

  int Size() const { return mSize; }

  /** Transforms re in place; im must already be zeroed by the caller. */
  void Forward(float* re, float* im)
  {
    // Bit-reversal permutation, in place on the caller's buffers.
    for (int i = 0; i < mSize; ++i)
    {
      const int r = Reverse(i, mLevels);
      if (r > i)
      {
        std::swap(re[i], re[r]);
        std::swap(im[i], im[r]);
      }
    }

    for (int size = 2; size <= mSize; size <<= 1)
    {
      const int half = size >> 1;
      for (int i = 0; i < mSize; i += size)
      {
        for (int j = i, k = 0; j < i + half; ++j, ++k)
        {
          const float tr = re[j + half] * mCos[k] - im[j + half] * mSin[k];
          const float ti = re[j + half] * mSin[k] + im[j + half] * mCos[k];
          re[j + half] = re[j] - tr;
          im[j + half] = im[j] - ti;
          re[j] += tr;
          im[j] += ti;
        }
      }
    }
  }

private:
  static int Reverse(int value, int bits)
  {
    int out = 0;
    for (int i = 0; i < bits; ++i)
    {
      out = (out << 1) | (value & 1);
      value >>= 1;
    }
    return out;
  }

  int mSize = 512;
  int mLevels = 9;
  float mCos[kMaxSize / 2] {};
  float mSin[kMaxSize / 2] {};
};

/** Shared state between the audio thread and the analyzer control. */
class AnalyzerFeed
{
public:
  static constexpr int kRingSize = 8192;
  static constexpr int kFftSize = 2048;
  static constexpr int kBinCount = 96;
  static constexpr int kWaterfallRows = 28;
  static constexpr double kLowHz = 40.0;
  static constexpr double kHighHz = 16000.0;
  static constexpr float kFloorDisplayDb = -120.f;
  /** How long the display holds after the signal drops, in seconds. Wall-clock, not frames:
   *  a frame count would make the hold depend on the editor's redraw rate. Without a hold a
   *  short percussive hit would vanish between two repaints. */
  static constexpr double kHoldSeconds = 0.6;
  /** The level the output has to exceed to count as signal. -60 dBFS is well below anything
   *  audible and well above the idle noise floor. */
  static constexpr float kActivityDb = -60.f;
  /** Waterfall rows are advanced on this cadence so the history covers real time rather
   *  than one row per repaint. */
  static constexpr double kWaterfallRowSeconds = 0.045;

  void Reset(double sampleRate)
  {
    mSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    std::memset(mRing, 0, sizeof(mRing));
    std::memset(mBins, 0, sizeof(mBins));
    std::memset(mSmoothed, 0, sizeof(mSmoothed));
    std::memset(mWaterfall, 0, sizeof(mWaterfall));
    mWrite = 0;
    mFilled = 0;
    mTriggerCounter = 0;
    mActiveUntil = 0.0;
    mPeakDb = -120.f;
    mStarted = false;
    mWaterfallDue = 0.0;
    mNow = 0.0;
    mFft.Reset(kFftSize);
    for (int i = 0; i < kFftSize; ++i)
      mWindow[i] = 0.5f - 0.5f * std::cos(2.f * 3.14159265358979f * i / (kFftSize - 1));
    // Coherent gain of the Hann window: sum(w)/n. A bin-centred sine of amplitude A has an
    // FFT magnitude of A*sum(w)/2, so dividing the magnitude by sum(w)/2 recovers A.
    // Dividing by n/2 instead reads 6 dB high, which squashes the whole display.
    double sum = 0.0;
    for (int i = 0; i < kFftSize; ++i)
      sum += mWindow[i];
    mCoherentHalf = sum * 0.5;
  }

  /** AUDIO THREAD. Two float writes and a cursor bump. Nothing else. */
  void PushFrame(float left, float right) noexcept
  {
    mRing[mWrite][0] = left;
    mRing[mWrite][1] = right;
    mWrite = (mWrite + 1) & (kRingSize - 1);
    if (mFilled < kRingSize)
      ++mFilled;
  }

  /** MIDI thread / audio thread. Counts note-ons for diagnostics. The display itself
   *  follows the output level, so this is not on the trigger path. */
  void NoteOn() noexcept
  {
    if (mTriggerCounter < 1000000)
      ++mTriggerCounter;
  }

  /** UI THREAD. Reads the ring, transforms, fills bins and the waterfall history.
   *
   *  dtSeconds is the time since the previous call. The control passes the real elapsed
   *  time, so the arm hold and the waterfall cadence are wall-clock rather than one unit
   *  per repaint. A frame-counted hold made the panel behave differently at 15 fps than
   *  at 60. */
  void Snapshot(int view, double dtSeconds)
  {
    const double dt = dtSeconds > 0.0 && dtSeconds < 1.0 ? dtSeconds : (1.0 / 30.0);
    mNow += dt;

    const int n = mFft.Size();
    if (mFilled < n)
      return;

    for (int i = 0; i < n; ++i)
    {
      const int idx = (mWrite - n + i) & (kRingSize - 1);
      const float mono = 0.5f * (mRing[idx][0] + mRing[idx][1]);
      mRe[i] = mono * mWindow[i];
      mIm[i] = 0.f;
    }
    mFft.Forward(mRe, mIm);

    for (int b = 0; b < kBinCount; ++b)
    {
      mAcc[b] = 0.0;
      mCount[b] = 0;
    }

    // Band the spectrum by integrating the FFT bins that fall in each log band.
    //
    // A band is defined by its two edges in Hz. Every FFT bin whose centre lies inside gets
    // assigned to exactly one band. Bands narrower than one FFT bin own no bin, and those
    // are resolved below by interpolation between the neighbouring populated bands -- not
    // by copying the nearest one, which painted flat plateaus across the bottom of the
    // display and made it read as a staircase rather than a spectrum.
    const double binHz = mSampleRate / static_cast<double>(n);
    const int half = n / 2;
    const double logSpan = std::log(kHighHz / kLowHz);
    for (int k = 1; k <= half; ++k)
    {
      const double hz = k * binHz;
      if (hz < kLowHz)
        continue;
      if (hz >= kHighHz)
        break;
      int b = static_cast<int>(kBinCount * std::log(hz / kLowHz) / logSpan);
      b = b < 0 ? 0 : (b >= kBinCount ? kBinCount - 1 : b);
      mAcc[b] += static_cast<double>(mRe[k]) * mRe[k] +
                 static_cast<double>(mIm[k]) * mIm[k];
      ++mCount[b];
    }

    // Resolve empty bands by linear interpolation in dB between the nearest populated
    // bands on either side. Ends clamp to the nearest populated band, which is why the
    // very bottom of the display tracks the lowest real data rather than inventing it.
    int firstHit = -1;
    int lastHit = -1;
    for (int b = 0; b < kBinCount; ++b)
    {
      if (mCount[b] > 0)
      {
        if (firstHit < 0)
          firstHit = b;
        lastHit = b;
        mBins[b] = NormToDb(mAcc[b] / mCount[b], n);
      }
    }

    if (firstHit < 0)
    {
      // No usable FFT data at all this frame. Hold the floor rather than stale values.
      for (int b = 0; b < kBinCount; ++b)
        mBins[b] = kFloorDisplayDb;
    }
    else
    {
      for (int b = 0; b < firstHit; ++b)
        mBins[b] = mBins[firstHit];
      for (int b = lastHit + 1; b < kBinCount; ++b)
        mBins[b] = mBins[lastHit];

      int prev = firstHit;
      for (int b = firstHit + 1; b <= lastHit; ++b)
      {
        if (mCount[b] > 0)
        {
          const int gap = b - prev - 1;
          if (gap > 0)
          {
            for (int g = 1; g <= gap; ++g)
            {
              const double t = static_cast<double>(g) / (gap + 1);
              mBins[prev + g] = static_cast<float>(mBins[prev] * (1.0 - t) + mBins[b] * t);
            }
          }
          prev = b;
        }
      }
    }

    float peak = kFloorDisplayDb;
    for (int b = 0; b < kBinCount; ++b)
      peak = std::max(peak, mBins[b]);

    // Smooth so the display does not flicker frame to frame. The smoothing coefficient is
    // scaled to the frame time so the response feels the same at any redraw rate.
    const float alpha = static_cast<float>(std::clamp(dt / 0.06, 0.05, 0.9));
    if (mStarted)
    {
      for (int b = 0; b < kBinCount; ++b)
        mSmoothed[b] = mSmoothed[b] * (1.f - alpha) + mBins[b] * alpha;
    }
    else
    {
      for (int b = 0; b < kBinCount; ++b)
        mSmoothed[b] = mBins[b];
      mStarted = true;
    }
    mPeakDb = peak;

    // Waterfall advances on its own cadence, independent of which view is open, so the
    // history is real time rather than however long you happened to be looking at it.
    if (mNow >= mWaterfallDue)
    {
      mWaterfallDue = mNow + kWaterfallRowSeconds;
      for (int r = kWaterfallRows - 1; r > 0; --r)
        std::memcpy(mWaterfall[r], mWaterfall[r - 1], sizeof(mWaterfall[0]));
      std::memcpy(mWaterfall[0], mSmoothed, sizeof(mSmoothed));
    }

    // Activity follows the output itself, not note events.
    //
    // This used to arm on NoteOn(). That only works when MIDI reaches the plugin, and the
    // editor's own on-screen keyboard and QWERTY keys paint their state without ever
    // sending MIDI to the delegate -- so playing them left the panel dark. Watching the
    // output level needs no event path at all: whatever makes sound lights the panel,
    // whether that came from the host, the plugin's keyboard, or a drum pad.
    if (peak > kActivityDb)
      mActiveUntil = mNow + kHoldSeconds;

    mTriggerSeen = mTriggerCounter;
  }

  /** FFT magnitude-squared to dB, normalised by the window's coherent gain. */
  float NormToDb(double energy, int n) const
  {
    const double rms = std::sqrt(energy);
    const double norm = mCoherentHalf > 1e-9 ? rms / mCoherentHalf : 0.0;
    if (norm <= 1e-9)
      return kFloorDisplayDb;
    return static_cast<float>(20.0 * std::log10(norm));
  }

  /** True while the output has been above kActivityDb recently. The panel draws its
   *  spectrum while active and its idle MAGS face while not. */
  bool Active() const { return mNow < mActiveUntil; }
  float PeakDb() const { return mPeakDb; }
  const float* Smoothed() const { return mSmoothed; }
  const float* WaterfallRow(int row) const { return mWaterfall[row]; }
  int WaterfallRows() const { return kWaterfallRows; }
  int Bins() const { return kBinCount; }
  bool HasSignal() const { return mStarted; }

private:
  double mSampleRate = 44100.0;
  float mRing[kRingSize][2] {};
  int mWrite = 0;
  int mFilled = 0;

  Fft mFft;
  float mWindow[kFftSize] {};
  float mRe[kFftSize] {};
  float mIm[kFftSize] {};
  double mCoherentHalf = 1.0;
  float mBins[kBinCount] {};
  float mSmoothed[kBinCount] {};
  float mWaterfall[kWaterfallRows][kBinCount] {};
  double mAcc[kBinCount] {};
  int mCount[kBinCount] {};
  float mPeakDb = -120.f;
  bool mStarted = false;

  unsigned mTriggerCounter = 0;
  unsigned mTriggerSeen = 0;
  double mActiveUntil = 0.0;
  double mNow = 0.0;
  double mWaterfallDue = 0.0;
};

}  // namespace ui
}  // namespace keith
