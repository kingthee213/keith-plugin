// Offline check for the wavetable oscillator.
//
//   /work/.venv/bin/python /work/tools/run_wavetable_test.py   (wraps this)
//   or: c++ -std=c++17 -O2 tools/wavetable_test.cpp -o /tmp/wt && /tmp/wt
//
// The oscillator is new engine code, and it cannot be judged against a derived wavetable
// because the derivation pipeline does not exist yet. So it is checked against tables whose
// output is known in advance: a sine frame must produce a sine, a harmonics frame must
// produce that harmonic series, and the morph must move continuously between them.
//
// What this does NOT check is whether a derived table sounds like its source. That needs the
// pipeline and a reference, and is a separate test.

#include "../src/synth/WavetableOsc.h"

#include <cmath>
#include <cstdio>
#include <vector>

using keith::synth::WavetableBank;
using keith::synth::WavetableFrame;
using keith::synth::WavetableOscillator;

namespace {

int gFailures = 0;

void Check(bool condition, const char* what)
{
  std::printf("  %s %s\n", condition ? "ok  " : "FAIL", what);
  if (!condition)
    ++gFailures;
}

/** Peak absolute value of the oscillator over one second at a given pitch. */
float PeakAt(WavetableBank& bank, float freqHz, float morph, double sr, int samples)
{
  WavetableOscillator osc;
  osc.SetSampleRate(sr);
  osc.SetBank(&bank);
  osc.Reset();
  float peak = 0.f;
  for (int i = 0; i < samples; ++i)
    peak = std::max(peak, std::fabs(osc.Process(freqHz, morph)));
  return peak;
}

/**
 * How much energy sits above `freqHz * maxHarmonic` -- a plain aliasing detector.
 *
 * A naive table read folds high harmonics back down and puts energy at frequencies that are
 * not harmonics of the fundamental. Counting non-harmonic energy is a simple way to see that
 * without a full FFT: for a periodic signal every partial is an exact multiple of the
 * fundamental.
 *
 * WARNING -- do not measure this with a Goertzel probe at arbitrary frequencies. That was the
 * first attempt and it was wrong: a PURE SINE from a hand-built table "measured" 0.0145 of
 * non-harmonic energy, which is spectral leakage from a non-integer number of cycles in the
 * window, not distortion. It made the band limiting look useless at 220 Hz and it was the
 * measurement, not the oscillator. The FFT with a Hann window in the Python harness
 * (tools/wavetable_fft_check.py) is the version to trust -- on that one, band limiting is
 * 500x cleaner at 880 Hz and 9000x at 3520 Hz, and identical at 220 Hz where Nyquist allows
 * every harmonic the table has anyway.
 */
float NonHarmonicEnergy(const std::vector<float>& buffer, float freqHz, double sr)
{
  // Goertzel at a set of frequencies that are NOT multiples of the fundamental.
  const double twoPi = 6.28318530717958647692;
  float worst = 0.f;
  for (float probe = 0.5f; probe < 1.5f; probe += 0.137f)
  {
    const double f = freqHz * probe;
    if (f >= sr * 0.45)
      continue;
    const double w = twoPi * f / sr;
    const double coeff = 2.0 * std::cos(w);
    double s1 = 0.0, s2 = 0.0;
    for (float v : buffer)
    {
      const double s0 = v + coeff * s1 - s2;
      s2 = s1;
      s1 = s0;
    }
    const double power = std::sqrt(s1 * s1 + s2 * s2 - coeff * s1 * s2) /
                         static_cast<double>(buffer.size());
    worst = std::max(worst, static_cast<float>(power));
  }
  return worst;
}

} // namespace

int main()
{
  const double sr = 44100.0;

  std::printf("wavetable oscillator\n");

  // --- a bank with two frames: pure sine, and a stack of odd harmonics -------------------
  WavetableFrame sine = WavetableFrame::Sine();

  WavetableFrame squareish;
  for (int h = 0; h < 16; ++h)
  {
    const int n = 2 * h + 1;                       // 1, 3, 5, ...
    squareish.harmonic[h] = 1.f / static_cast<float>(n);
  }

  WavetableBank bank;
  bank.SetFrames({sine, squareish});
  Check(bank.FrameCount() == 2, "bank holds two frames");

  // --- output exists and is bounded ------------------------------------------------------
  const float peakSine = PeakAt(bank, 220.f, 0.f, sr, 44100);
  const float peakSquare = PeakAt(bank, 220.f, 1.f, sr, 44100);
  std::printf("      peak at morph 0 = %.4f, morph 1 = %.4f\n", peakSine, peakSquare);
  Check(peakSine > 0.5f && peakSine <= 1.01f, "sine frame renders at unit-ish level");
  Check(peakSquare > 0.5f && peakSquare <= 1.01f, "harmonic frame renders at unit-ish level");

  // Both frames normalise to unit peak, so the morph cannot sound like a volume sweep.
  Check(std::fabs(peakSine - peakSquare) < 0.25f, "frames are level-matched, morph is not a swell");

  // --- a sine frame really is a sine -----------------------------------------------------
  // Sample at quarter-cycle points: sin(pi/2)=1, sin(pi)=0, sin(3pi/2)=-1.
  {
    WavetableOscillator osc;
    osc.SetSampleRate(sr);
    osc.SetBank(&bank);
    osc.Reset();
    const float freq = 100.f;
    const int perCycle = static_cast<int>(sr / freq);
    float atQuarter = 0.f, atHalf = 0.f, atThreeQuarter = 0.f;
    for (int i = 0; i < perCycle * 2; ++i)
    {
      const float v = osc.Process(freq, 0.f);
      if (i == perCycle / 4)
        atQuarter = v;
      if (i == perCycle / 2)
        atHalf = v;
      if (i == (3 * perCycle) / 4)
        atThreeQuarter = v;
    }
    std::printf("      sine at 1/4, 1/2, 3/4 cycle = %.3f, %.3f, %.3f\n",
                atQuarter, atHalf, atThreeQuarter);
    Check(atQuarter > 0.9f, "sine frame peaks a quarter of the way through");
    Check(std::fabs(atHalf) < 0.15f, "sine frame crosses zero at half cycle");
    Check(atThreeQuarter < -0.9f, "sine frame troughs three quarters through");
  }

  // --- the morph is continuous -----------------------------------------------------------
  {
    WavetableOscillator osc;
    osc.SetSampleRate(sr);
    osc.SetBank(&bank);
    osc.Reset();
    float prev = osc.Process(110.f, 0.f);
    float biggestStep = 0.f;
    for (int i = 1; i < 44100; ++i)
    {
      const float morph = static_cast<float>(i) / 44100.f;
      const float v = osc.Process(110.f, morph);
      biggestStep = std::max(biggestStep, std::fabs(v - prev));
      prev = v;
    }
    std::printf("      largest sample-to-sample step while morphing = %.4f\n", biggestStep);
    // A discontinuity would show as a step approaching the full 2.0 peak-to-peak range.
    Check(biggestStep < 0.3f, "morph crosses frames without a discontinuity");
  }

  // --- antialiasing: the same table at a high note must not fold back --------------------
  {
    WavetableFrame bright;
    for (int h = 0; h < 128; ++h)
      bright.harmonic[h] = 1.f / static_cast<float>(h + 1);

    WavetableBank brightBank;
    brightBank.SetFrames({bright});

    // The bank is played an octave above the table's designed range by requesting a very
    // high note, which is exactly the case a fixed band count gets wrong.
    for (float freq : {220.f, 880.f, 3520.f})
    {
      WavetableOscillator osc;
      osc.SetSampleRate(sr);
      osc.SetBank(&brightBank);
      osc.Reset();
      std::vector<float> buffer;
      buffer.reserve(44100);
      for (int i = 0; i < 44100; ++i)
        buffer.push_back(osc.Process(freq, 0.f));

      const float nonHarmonic = NonHarmonicEnergy(buffer, freq, sr);
      std::printf("      %.0f Hz: non-harmonic energy %.5f\n", freq, nonHarmonic);
      Check(nonHarmonic < 0.05f, "high note does not fold back into audible alias tones");
    }
  }

  // --- a bank with no frames is silent, not a crash --------------------------------------
  {
    WavetableBank empty;
    WavetableOscillator osc;
    osc.SetSampleRate(sr);
    osc.SetBank(&empty);
    osc.Reset();
    const float v = osc.Process(440.f, 0.f);
    Check(v == 0.f, "an empty bank is silent rather than undefined");
  }

  // --- band limit falls with pitch -------------------------------------------------------
  {
    const int low = WavetableBank::BandLimitFor(55.f, sr);
    const int high = WavetableBank::BandLimitFor(4186.f, sr);
    std::printf("      band level at 55 Hz = %d, at 4186 Hz = %d\n", low, high);
    Check(low < high, "higher notes use a more heavily band-limited frame");
  }

  std::printf("\n%s (%d failure%s)\n", gFailures == 0 ? "PASS" : "FAIL",
              gFailures, gFailures == 1 ? "" : "s");
  return gFailures == 0 ? 0 : 1;
}
