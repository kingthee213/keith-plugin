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
    for (int i = 0; i < mSize; ++i)
    {
      const int r = Reverse(i, levels);
      if (r > i)
      {
        std::swap(mRe[i], mRe[r]);
        std::swap(mIm[i], mIm[r]);
      }
    }
    for (int half = 1; half < mSize; half <<= 1)
    {
      const double step = -3.14159265358979323846 / half;
      for (int i = 0; i < half; ++i)
      {
        mCos[i] = static_cast<float>(std::cos(step * i));
        mSin[i] = static_cast<float>(std::sin(step * i));
      }
      mHalf[half] = half;
    }
  }

  int Size() const { return mSize; }

  /** Transforms mRe in place; mIm must already be zeroed by the caller. */
  void Forward(float* re, float* im) const
  {
    for (int i = 0; i < mSize; ++i)
      mScratchRe[i] = re[i];

    for (int size = 2; size <= mSize; size <<= 1)
    {
      const int half = size >> 1;
      for (int i = 0; i < mSize; i += size)
      {
        for (int j = i, k = 0; j < i + half; ++j, ++k)
        {
          const float tr = mScratchRe[j + half] * mCos[k] - im[j + half] * mSin[k];
          const float ti = mScratchRe[j + half] * mSin[k] + im[j + half] * mCos[k];
          mScratchRe[j + half] = mScratchRe[j] - tr;
          im[j + half] = im[j] - ti;
          mScratchRe[j] += tr;
          im[j] += ti;
        }
      }
    }

    for (int i = 0; i < mSize; ++i)
      re[i] = mScratchRe[i];
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
  int mHalf[kMaxSize + 1] {};
  float mScratchRe[kMaxSize] {};
};

/** Shared state between the audio thread and the analyzer control. */
class AnalyzerFeed
{
public:
  static constexpr int kRingSize = 8192;
  static constexpr int kBinCount = 96;
  static constexpr int kWaterfallRows = 28;
  static constexpr double kLowHz = 40.0;
  static constexpr double kHighHz = 16000.0;
  static constexpr float kFloorDisplayDb = -120.f;

  void Reset(double sampleRate)
  {
    mSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    std::memset(mRing, 0, sizeof(mRing));
    std::memset(mBins, 0, sizeof(mBins));
    std::memset(mWaterfall, 0, sizeof(mWaterfall));
    mWrite = 0;
    mFilled = 0;
    mTriggerCounter = 0;
    mArmed = 0;
    mPeakDb = -120.f;
    mStarted = false;
    mFft.Reset(512);
    for (int i = 0; i < 512; ++i)
      mWindow[i] = 0.5f - 0.5f * std::cos(2.f * 3.14159265f * i / 511.f);
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

  /** MIDI thread / audio thread. Counts note-ons so the UI can arm the display. */
  void NoteOn() noexcept
  {
    if (mTriggerCounter < 1000000)
      ++mTriggerCounter;
  }

  /** UI THREAD. Reads the ring, transforms, fills bins and the waterfall history. */
  void Snapshot(int view)
  {
    const int n = mFft.Size();
    if (mFilled < n)
      return;

    for (int i = 0; i < n; ++i)
    {
      const int idx = (mWrite - n + i) & (kRingSize - 1);
      const float mono = 0.5f * (mRing[idx][0] + mRing[idx][1]);
      const float w = mWindow[i];
      mRe[i] = mono * w;
      mIm[i] = 0.f;
    }
    mFft.Forward(mRe, mIm);

    float peak = 0.f;
    for (int b = 0; b < kBinCount; ++b)
    {
      mAcc[b] = 0.0;
      mCount[b] = 0;
    }

    // Assign each FFT bin to exactly one log band, then average within the band.
    //
    // The first cut looped over a band's frequency edges and summed whatever FFT bins fell
    // between them. That double-counts: below about 2 kHz a log band is narrower than one
    // FFT bin (93.75 Hz at 48 kHz with a 512 transform), so consecutive bands all swept the
    // same high-energy bin and smeared energy upward. A 1 kHz tone read as 2.5 kHz. This
    // assigns each FFT bin once, so the mapping cannot drift.
    const double binHz = mSampleRate / static_cast<double>(n);
    const int half = n / 2;
    for (int k = 1; k <= half; ++k)
    {
      const double hz = k * binHz;
      if (hz < kLowHz)
        continue;
      if (hz >= kHighHz)
        break;
      int b = static_cast<int>(kBinCount * std::log(hz / kLowHz) / std::log(kHighHz / kLowHz));
      b = b < 0 ? 0 : (b >= kBinCount ? kBinCount - 1 : b);
      mAcc[b] += static_cast<double>(mRe[k]) * mRe[k] +
                 static_cast<double>(mIm[k]) * mIm[k];
      ++mCount[b];
    }

    for (int b = 0; b < kBinCount; ++b)
    {
      double energy = 0.0;
      if (mCount[b] > 0)
      {
        energy = mAcc[b] / mCount[b];
      }
      else
      {
        // A band too narrow to own an FFT bin borrows the nearest populated one, so the
        // bottom of the display still reads rather than sitting at the floor.
        int nb = b;
        while (nb < kBinCount && mCount[nb] == 0)
          ++nb;
        energy = (nb < kBinCount && mCount[nb] > 0) ? mAcc[nb] / mCount[nb] : 0.0;
      }
      const double rms = std::sqrt(energy);
      const double norm = rms / (0.5 * n);
      mBins[b] = norm > 1e-9 ? static_cast<float>(20.0 * std::log10(norm)) : kFloorDisplayDb;
      peak = std::max(peak, mBins[b]);
    }

    // Smooth so the display does not flicker frame to frame.
    if (mStarted)
    {
      for (int b = 0; b < kBinCount; ++b)
        mSmoothed[b] = mSmoothed[b] * 0.65f + mBins[b] * 0.35f;
    }
    else
    {
      for (int b = 0; b < kBinCount; ++b)
        mSmoothed[b] = mBins[b];
      mStarted = true;
    }
    mPeakDb = peak;

    // Waterfall: shift the history down one row and write the newest spectrum at the top.
    if (view == 1)
    {
      for (int r = kWaterfallRows - 1; r > 0; --r)
        std::memcpy(mWaterfall[r], mWaterfall[r - 1], sizeof(mWaterfall[0]));
      std::memcpy(mWaterfall[0], mSmoothed, sizeof(mSmoothed));
    }

    // Arm on a note-on and hold, so a short hit stays visible across several frames.
    if (mTriggerSeen != mTriggerCounter)
    {
      mTriggerSeen = mTriggerCounter;
      mArmed = 48;
    }
    else if (mArmed > 0)
    {
      --mArmed;
    }
  }

  bool Armed() const { return mArmed > 0; }
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
  float mWindow[2048] {};
  float mRe[2048] {};
  float mIm[2048] {};
  float mBins[kBinCount] {};
  float mSmoothed[kBinCount] {};
  float mWaterfall[kWaterfallRows][kBinCount] {};
  double mAcc[kBinCount] {};
  int mCount[kBinCount] {};
  float mPeakDb = -120.f;
  bool mStarted = false;

  unsigned mTriggerCounter = 0;
  unsigned mTriggerSeen = 0;
  int mArmed = 0;
};

}  // namespace ui
}  // namespace keith
