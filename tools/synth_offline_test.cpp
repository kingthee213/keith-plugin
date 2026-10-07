// Offline core-synth verification — not linked into the plugin binary.
#include "../src/synth/CoreSynth.h"

#include <cmath>
#include <cstdio>
#include <vector>

using keith::synth::EngineMode;
using keith::synth::FilterType;
using keith::synth::LfoShape;
using keith::synth::ModDest;
using keith::synth::OfflineEngine;
using keith::synth::Params;
using keith::synth::PolyMode;

static int gPass = 0;
static int gFail = 0;

static void Expect(bool cond, const char* name)
{
  if (cond)
  {
    ++gPass;
    std::printf("PASS  %s\n", name);
  }
  else
  {
    ++gFail;
    std::printf("FAIL  %s\n", name);
  }
}

static bool AllFinite(const std::vector<float>& x)
{
  for (float v : x)
  {
    if (!std::isfinite(v))
      return false;
  }
  return true;
}

static double PeakAbs(const std::vector<float>& x)
{
  double p = 0.;
  for (float v : x)
    p = std::max(p, std::fabs(static_cast<double>(v)));
  return p;
}

static double Rms(const std::vector<float>& x, size_t start, size_t count)
{
  if (count == 0)
    return 0.;
  double sum = 0.;
  for (size_t i = 0; i < count; ++i)
  {
    const double v = x[start + i];
    sum += v * v;
  }
  return std::sqrt(sum / static_cast<double>(count));
}

static int CountZeroCrossings(const std::vector<float>& x, size_t start, size_t count)
{
  int zc = 0;
  for (size_t i = start + 1; i < start + count; ++i)
  {
    if ((x[i - 1] < 0.f && x[i] >= 0.f) || (x[i - 1] >= 0.f && x[i] < 0.f))
      ++zc;
  }
  return zc;
}

static std::vector<float> RenderNote(OfflineEngine& eng, int note, int onFrames, int offFrames)
{
  std::vector<float> out(static_cast<size_t>(onFrames + offFrames), 0.f);
  eng.NoteOn(note, 0.9f);
  for (int i = 0; i < onFrames; ++i)
    out[static_cast<size_t>(i)] = eng.Process();
  eng.NoteOff(note);
  for (int i = 0; i < offFrames; ++i)
    out[static_cast<size_t>(onFrames + i)] = eng.Process();
  return out;
}

static std::vector<float> RenderHold(OfflineEngine& eng, int note, float velocity, int frames)
{
  std::vector<float> out(static_cast<size_t>(frames), 0.f);
  eng.NoteOn(note, velocity);
  for (int i = 0; i < frames; ++i)
    out[static_cast<size_t>(i)] = eng.Process();
  return out;
}

static bool Identical(const std::vector<float>& a, const std::vector<float>& b)
{
  if (a.size() != b.size())
    return false;
  for (size_t i = 0; i < a.size(); ++i)
    if (a[i] != b[i])
      return false;
  return true;
}

static double MeanAbsDiff(const std::vector<float>& a, const std::vector<float>& b, size_t start)
{
  if (a.size() != b.size() || start >= a.size())
    return 1.0e9;
  double sum = 0.;
  const size_t n = a.size() - start;
  for (size_t i = start; i < a.size(); ++i)
    sum += std::fabs(static_cast<double>(a[i]) - static_cast<double>(b[i]));
  return sum / static_cast<double>(n);
}

static double GoertzelMag(const std::vector<float>& x, size_t start, size_t count, double freq, double sr)
{
  if (count < 8 || freq <= 0.)
    return 0.;
  const double w = 2.0 * 3.14159265358979323846 * freq / sr;
  const double coeff = 2.0 * std::cos(w);
  double s1 = 0.;
  double s2 = 0.;
  for (size_t i = 0; i < count; ++i)
  {
    const double s0 = static_cast<double>(x[start + i]) + coeff * s1 - s2;
    s2 = s1;
    s1 = s0;
  }
  const double power = s1 * s1 + s2 * s2 - coeff * s1 * s2;
  return power > 0. ? std::sqrt(power) : 0.;
}

static Params SteadyPatch()
{
  Params p {};
  p.engine = EngineMode::Hybrid;
  p.osc1Shape = 0.f;
  p.osc1Level = 1.f;
  p.osc2Level = 0.f;
  p.subLevel = 0.f;
  p.noiseLevel = 0.f;
  p.fmAmount = 0.f;
  p.filterType = FilterType::LP24;
  p.filterCutoffHz = 12000.f;
  p.filterReso = 0.05f;
  p.filterEnvAmt = 0.f;
  p.ampAttackMs = 1.f;
  p.ampDecayMs = 50.f;
  p.ampSustain = 1.f;
  p.ampReleaseMs = 200.f;
  p.pitchAmount = 0.f;
  p.masterLevel = 0.6f;
  return p;
}

int main()
{
  const int sr = 48000;
  OfflineEngine eng;
  eng.Reset(sr);

  Params p {};
  p.engine = EngineMode::Analog;
  p.osc1Shape = 0.7f; // saw-ish
  p.osc1Level = 1.f;
  p.osc2Level = 0.f;
  p.subLevel = 0.f;
  p.noiseLevel = 0.f;
  p.fmAmount = 0.f;
  p.filterType = FilterType::LP24;
  p.filterCutoffHz = 12000.f;
  p.filterReso = 0.1f;
  p.filterEnvAmt = 0.f;
  p.ampAttackMs = 5.f;
  p.ampDecayMs = 100.f;
  p.ampSustain = 0.8f;
  p.ampReleaseMs = 200.f;
  p.pitchAmount = 0.f;
  p.masterLevel = 0.8f;
  eng.SetParams(p);

  // 1) Note-on creates non-zero audio
  {
    eng.Reset(sr);
    eng.SetParams(p);
    auto buf = RenderNote(eng, 60, sr / 4, sr / 4);
    Expect(PeakAbs(buf) > 0.01, "note_on_nonzero");
    Expect(AllFinite(buf), "note_on_finite");
  }

  // 2) Note-off decays toward silence
  {
    eng.Reset(sr);
    Params slow = p;
    slow.ampReleaseMs = 80.f;
    slow.ampSustain = 1.f;
    eng.SetParams(slow);
    auto buf = RenderNote(eng, 60, sr / 5, sr / 2);
    const size_t mid = buf.size() / 2;
    const double holdRms = Rms(buf, mid / 4, mid / 4);
    const double tailRms = Rms(buf, buf.size() - static_cast<size_t>(sr / 20), static_cast<size_t>(sr / 20));
    Expect(holdRms > 0.02, "note_off_had_signal");
    Expect(tailRms < holdRms * 0.15, "note_off_decays_to_silence");
  }

  // 3) Amp attack/release change measured envelope
  {
    eng.Reset(sr);
    Params fastA = p;
    fastA.ampAttackMs = 1.f;
    fastA.ampReleaseMs = 50.f;
    eng.SetParams(fastA);
    auto fastBuf = RenderNote(eng, 60, sr / 4, sr / 4);

    eng.Reset(sr);
    Params slowA = p;
    slowA.ampAttackMs = 200.f;
    slowA.ampReleaseMs = 400.f;
    eng.SetParams(slowA);
    auto slowBuf = RenderNote(eng, 60, sr / 4, sr / 4);

    const double fastEarly = Rms(fastBuf, 0, static_cast<size_t>(sr / 100));
    const double slowEarly = Rms(slowBuf, 0, static_cast<size_t>(sr / 100));
    Expect(fastEarly > slowEarly * 1.5, "amp_attack_changes_envelope");

    const double fastTail = Rms(fastBuf, fastBuf.size() - static_cast<size_t>(sr / 10), static_cast<size_t>(sr / 20));
    const double slowTail = Rms(slowBuf, slowBuf.size() - static_cast<size_t>(sr / 10), static_cast<size_t>(sr / 20));
    Expect(slowTail > fastTail * 1.2, "amp_release_changes_envelope");
  }

  // 4) Pitch envelope changes zero-crossing / frequency
  {
    eng.Reset(sr);
    Params flat = p;
    flat.pitchAmount = 0.f;
    flat.ampSustain = 1.f;
    flat.ampAttackMs = 1.f;
    flat.filterCutoffHz = 18000.f;
    eng.SetParams(flat);
    auto flatBuf = RenderNote(eng, 60, sr / 3, 1000);

    eng.Reset(sr);
    Params pitched = flat;
    pitched.pitchAmount = 12.f; // +1 octave at env peak
    pitched.pitchAttackMs = 1.f;
    pitched.pitchDecayMs = 5000.f;
    pitched.pitchSustain = 1.f;
    pitched.pitchReleaseMs = 50.f;
    eng.SetParams(pitched);
    auto pitchedBuf = RenderNote(eng, 60, sr / 3, 1000);

    const size_t win = static_cast<size_t>(sr / 5);
    const size_t start = static_cast<size_t>(sr / 20);
    const int zcFlat = CountZeroCrossings(flatBuf, start, win);
    const int zcPitch = CountZeroCrossings(pitchedBuf, start, win);
    Expect(zcPitch > zcFlat * 1.4, "pitch_env_raises_zero_crossings");
  }

  // 5) All filter modes remain finite
  {
    const FilterType modes[] = { FilterType::LP24, FilterType::LP12, FilterType::BP, FilterType::HP };
    bool ok = true;
    for (auto mode : modes)
    {
      eng.Reset(sr);
      Params fp = p;
      fp.filterType = mode;
      fp.filterCutoffHz = 1000.f;
      fp.filterReso = 0.7f;
      fp.filterEnvAmt = 0.5f;
      eng.SetParams(fp);
      auto buf = RenderNote(eng, 64, sr / 5, sr / 5);
      if (!AllFinite(buf) || PeakAbs(buf) > 20.)
        ok = false;
    }
    Expect(ok, "filter_modes_finite");
  }

  // 6) Parameter sweep has no NaN/Inf
  {
    eng.Reset(sr);
    Params sp = p;
    eng.SetParams(sp);
    eng.NoteOn(67, 0.8f);
    bool ok = true;
    for (int step = 0; step < 200; ++step)
    {
      const float t = static_cast<float>(step) / 199.f;
      sp.osc1Shape = t;
      sp.osc2Shape = 1.f - t;
      sp.osc2Level = t;
      sp.fmAmount = t * 0.5f;
      sp.filterCutoffHz = 80.f + t * 12000.f;
      sp.filterReso = t * 0.9f;
      sp.filterType = static_cast<FilterType>(step % 4);
      sp.subLevel = t * 0.3f;
      sp.noiseLevel = t * 0.1f;
      sp.unisonVoices = 1 + (step % 8);
      sp.unisonDetune = t;
      sp.engine = (step & 1) ? EngineMode::Hybrid : EngineMode::Analog;
      eng.SetParams(sp);
      for (int i = 0; i < 256; ++i)
      {
        const float s = eng.Process();
        if (!std::isfinite(s))
        {
          ok = false;
          break;
        }
      }
      if (!ok)
        break;
    }
    eng.NoteOff(67);
    for (int i = 0; i < sr / 4; ++i)
    {
      if (!std::isfinite(eng.Process()))
        ok = false;
    }
    Expect(ok, "param_sweep_no_nan_inf");
  }

  // U9. Each new control has one audible assertion and one negative that leaves an unrelated
  // destination alone. Defaults are compared both as fields and as rendered audio.
  {
    Params onlyOld = SteadyPatch();
    onlyOld.polyMode = PolyMode::Mono;
    onlyOld.osc1Shape = 0.7f;
    onlyOld.osc2Level = 0.45f;
    onlyOld.osc2Pitch = 7.f;
    onlyOld.filterCutoffHz = 2500.f;

    Params documented = onlyOld;
    documented.oscSync = false;
    documented.oscRing = false;
    documented.filterKeyTrack = 0.f;
    documented.filterDrive = 0.f;
    documented.lfoRateHz = 1.f;
    documented.lfoShape = LfoShape::Sine;
    documented.lfoDest = ModDest::Off;
    documented.lfoAmount = 0.f;
    documented.modWheelDest = ModDest::Off;
    documented.modWheelAmount = 0.f;
    documented.velToAmp = 0.f;
    documented.velToFilter = 0.f;
    documented.monoLegato = false;

    const bool fields =
      onlyOld.oscSync == documented.oscSync && onlyOld.oscRing == documented.oscRing &&
      onlyOld.filterKeyTrack == documented.filterKeyTrack && onlyOld.filterDrive == documented.filterDrive &&
      onlyOld.lfoRateHz == documented.lfoRateHz && onlyOld.lfoShape == documented.lfoShape &&
      onlyOld.lfoDest == documented.lfoDest && onlyOld.lfoAmount == documented.lfoAmount &&
      onlyOld.modWheelDest == documented.modWheelDest && onlyOld.modWheelAmount == documented.modWheelAmount &&
      onlyOld.velToAmp == documented.velToAmp && onlyOld.velToFilter == documented.velToFilter &&
      onlyOld.monoLegato == documented.monoLegato;
    Expect(fields, "new_defaults_match_documented_inert_values");

    auto play = [&](const Params& patch) {
      eng.Reset(sr);
      eng.SetParams(patch);
      eng.SetModWheel(0.85f);
      eng.NoteOn(60, 0.85f);
      std::vector<float> out(static_cast<size_t>(sr / 5), 0.f);
      for (int i = 0; i < sr / 5; ++i)
        out[static_cast<size_t>(i)] = eng.Process();
      eng.NoteOn(67, 0.4f);
      out.resize(out.size() + static_cast<size_t>(sr / 5));
      for (int i = 0; i < sr / 5; ++i)
        out[static_cast<size_t>(sr / 5 + i)] = eng.Process();
      return out;
    };
    const auto a = play(onlyOld);
    const auto b = play(documented);
    Expect(Identical(a, b) && AllFinite(a) && PeakAbs(a) > 0.01, "new_defaults_are_inert");
  }

  {
    Params clean = SteadyPatch();
    clean.osc1Shape = 0.85f;
    clean.osc2Shape = 0.85f;
    clean.osc2Level = 0.8f;
    clean.osc2Pitch = 12.f;
    clean.filterCutoffHz = 14000.f;
    eng.Reset(sr);
    eng.SetParams(clean);
    auto off = RenderHold(eng, 48, 0.9f, sr / 6);

    Params synced = clean;
    synced.oscSync = true;
    eng.Reset(sr);
    eng.SetParams(synced);
    auto on = RenderHold(eng, 48, 0.9f, sr / 6);
    const double diff = MeanAbsDiff(off, on, static_cast<size_t>(sr / 40));
    Expect(AllFinite(on) && diff > 0.02, "osc_sync_changes_timbre");

    Params sub = SteadyPatch();
    sub.osc1Level = 0.f;
    sub.osc2Level = 0.f;
    sub.subLevel = 0.8f;
    sub.oscSync = false;
    eng.Reset(sr);
    eng.SetParams(sub);
    auto subOff = RenderHold(eng, 48, 0.9f, sr / 8);
    sub.oscSync = true;
    eng.Reset(sr);
    eng.SetParams(sub);
    auto subOn = RenderHold(eng, 48, 0.9f, sr / 8);
    Expect(Identical(subOff, subOn), "osc_sync_does_not_change_sub_only");
  }

  {
    Params bare = SteadyPatch();
    bare.osc2Level = 0.f;
    bare.osc2Pitch = 0.f;
    bare.oscRing = false;
    eng.Reset(sr);
    eng.SetParams(bare);
    auto low = RenderHold(eng, 60, 0.9f, sr / 8);
    bare.osc2Pitch = 12.f;
    eng.Reset(sr);
    eng.SetParams(bare);
    auto high = RenderHold(eng, 60, 0.9f, sr / 8);
    Expect(Identical(low, high), "osc_ring_off_ignores_silent_osc2_pitch");

    bare.oscRing = true;
    bare.osc2Pitch = 0.f;
    eng.Reset(sr);
    eng.SetParams(bare);
    auto ringSame = RenderHold(eng, 60, 0.9f, sr / 8);
    bare.osc2Pitch = 12.f;
    eng.Reset(sr);
    eng.SetParams(bare);
    auto ringOct = RenderHold(eng, 60, 0.9f, sr / 8);
    Expect(AllFinite(ringOct) && MeanAbsDiff(ringSame, ringOct, static_cast<size_t>(sr / 40)) > 0.02,
           "osc_ring_changes_spectrum");
  }

  {
    Params kt = SteadyPatch();
    kt.osc1Shape = 0.7f;
    kt.filterCutoffHz = 1800.f;
    kt.filterKeyTrack = 0.f;
    eng.Reset(sr);
    eng.SetParams(kt);
    auto midOff = RenderHold(eng, 60, 0.9f, sr / 8);
    kt.filterKeyTrack = 1.f;
    eng.Reset(sr);
    eng.SetParams(kt);
    auto midOn = RenderHold(eng, 60, 0.9f, sr / 8);
    Expect(Identical(midOff, midOn), "key_track_does_not_move_middle_c");

    kt.filterKeyTrack = 0.f;
    eng.Reset(sr);
    eng.SetParams(kt);
    auto highOff = RenderHold(eng, 84, 0.9f, sr / 6);
    kt.filterKeyTrack = 1.f;
    eng.Reset(sr);
    eng.SetParams(kt);
    auto highOn = RenderHold(eng, 84, 0.9f, sr / 6);
    const size_t skip = static_cast<size_t>(sr / 40);
    const double dull = Rms(highOff, skip, highOff.size() - skip);
    const double open = Rms(highOn, skip, highOn.size() - skip);
    if (!(open > dull * 1.4))
      std::printf("  key track rms off %g on %g\n", dull, open);
    Expect(AllFinite(highOn) && open > dull * 1.4, "key_track_tracks_note");
  }

  {
    Params dry = SteadyPatch();
    dry.osc1Shape = 0.f;
    dry.filterDrive = 0.f;
    dry.filterCutoffHz = 16000.f;
    eng.Reset(sr);
    eng.SetParams(dry);
    auto clean = RenderHold(eng, 60, 0.9f, sr / 5);
    Params wet = dry;
    wet.filterDrive = 1.f;
    eng.Reset(sr);
    eng.SetParams(wet);
    auto driven = RenderHold(eng, 60, 0.9f, sr / 5);
    const size_t start = static_cast<size_t>(sr / 20);
    const size_t n = static_cast<size_t>(sr / 10);
    const double f0 = 261.6256;
    const double h1Clean = GoertzelMag(clean, start, n, f0, sr);
    const double h3Clean = GoertzelMag(clean, start, n, f0 * 3.0, sr);
    const double h1Wet = GoertzelMag(driven, start, n, f0, sr);
    const double h3Wet = GoertzelMag(driven, start, n, f0 * 3.0, sr);
    const double ratioClean = h3Clean / std::max(1.0e-9, h1Clean);
    const double ratioWet = h3Wet / std::max(1.0e-9, h1Wet);
    if (!(ratioWet > ratioClean * 2.0))
      std::printf("  drive harmonic ratio clean %g driven %g\n", ratioClean, ratioWet);
    Expect(AllFinite(driven) && ratioWet > ratioClean * 2.0, "filter_drive_adds_harmonics");

    const int zcClean = CountZeroCrossings(clean, start, n);
    const int zcWet = CountZeroCrossings(driven, start, n);
    Expect(std::abs(zcWet - zcClean) < zcClean / 5, "filter_drive_does_not_retune");
  }

  {
    auto tremoloStd = [&](const std::vector<float>& buf) {
      const int win = sr / 200;
      std::vector<double> env;
      for (size_t i = static_cast<size_t>(sr / 20); i + static_cast<size_t>(win) < buf.size(); i += static_cast<size_t>(win))
        env.push_back(Rms(buf, i, static_cast<size_t>(win)));
      double mean = 0.;
      for (double v : env)
        mean += v;
      mean /= static_cast<double>(env.size());
      double var = 0.;
      for (double v : env)
        var += (v - mean) * (v - mean);
      return std::sqrt(var / static_cast<double>(env.size()));
    };

    Params flat = SteadyPatch();
    flat.lfoDest = ModDest::Level;
    flat.lfoAmount = 0.f;
    flat.lfoRateHz = 5.f;
    eng.Reset(sr);
    eng.SetParams(flat);
    auto zero = RenderHold(eng, 60, 0.8f, sr / 2);
    flat.lfoAmount = 0.9f;
    eng.Reset(sr);
    eng.SetParams(flat);
    auto deep = RenderHold(eng, 60, 0.8f, sr / 2);
    const double std0 = tremoloStd(zero);
    const double std1 = tremoloStd(deep);
    if (!(std1 > std0 * 4.0))
      std::printf("  lfo amount std zero %g deep %g\n", std0, std1);
    Expect(AllFinite(deep) && std1 > std0 * 4.0, "lfo_amount_sets_depth");

    const size_t zcStart = static_cast<size_t>(sr / 10);
    const size_t zcN = static_cast<size_t>(sr / 5);
    Expect(std::abs(CountZeroCrossings(zero, zcStart, zcN) - CountZeroCrossings(deep, zcStart, zcN)) < 8,
           "lfo_level_does_not_change_pitch");

    auto cycleCount = [&](float rate) {
      Params q = SteadyPatch();
      q.lfoDest = ModDest::Level;
      q.lfoAmount = 0.95f;
      q.lfoRateHz = rate;
      q.lfoShape = LfoShape::Sine;
      eng.Reset(sr);
      eng.SetParams(q);
      auto buf = RenderHold(eng, 60, 0.8f, sr);
      const int win = sr / 100;
      std::vector<double> env;
      for (size_t i = static_cast<size_t>(sr / 10); i + static_cast<size_t>(win) < buf.size(); i += static_cast<size_t>(win))
        env.push_back(Rms(buf, i, static_cast<size_t>(win)));
      int peaks = 0;
      for (size_t i = 1; i + 1 < env.size(); ++i)
        if (env[i] > env[i - 1] && env[i] >= env[i + 1] && env[i] > 0.01)
          ++peaks;
      return peaks;
    };
    const int slow = cycleCount(2.f);
    const int fast = cycleCount(8.f);
    if (!(fast > slow * 2))
      std::printf("  lfo peaks 2Hz %d 8Hz %d\n", slow, fast);
    Expect(fast > slow * 2, "lfo_rate_sets_period");

    auto shaped = [&](LfoShape shape) {
      Params q = SteadyPatch();
      q.lfoDest = ModDest::Level;
      q.lfoAmount = 1.f;
      q.lfoRateHz = 4.f;
      q.lfoShape = shape;
      eng.Reset(sr);
      eng.SetParams(q);
      return RenderHold(eng, 60, 0.8f, sr / 2);
    };
    const auto sine = shaped(LfoShape::Sine);
    const auto square = shaped(LfoShape::Square);
    const auto sampleHold = shaped(LfoShape::SampleHold);
    Expect(MeanAbsDiff(sine, square, static_cast<size_t>(sr / 20)) > 0.01, "lfo_shape_changes_waveform");
    Expect(MeanAbsDiff(sine, sampleHold, static_cast<size_t>(sr / 20)) > 0.01, "lfo_sample_hold_differs_from_sine");

    auto routed = [&](ModDest dest, float amount) {
      Params q = SteadyPatch();
      q.osc1Shape = 0.15f;
      q.lfoRateHz = 6.f;
      q.lfoAmount = amount;
      q.lfoDest = dest;
      q.filterCutoffHz = 4000.f;
      eng.Reset(sr);
      eng.SetParams(q);
      return RenderHold(eng, 60, 0.8f, sr / 3);
    };
    const auto destOff = routed(ModDest::Off, 1.f);
    const auto destPitch = routed(ModDest::Pitch, 0.8f);
    const auto destLevel = routed(ModDest::Level, 0.8f);
    const auto destShape = routed(ModDest::Shape, 1.f);
    const auto destCutoff = routed(ModDest::Cutoff, 1.f);
    const auto amountZero = routed(ModDest::Cutoff, 0.f);
    Expect(Identical(destOff, amountZero), "lfo_off_and_zero_amount_match");

    const size_t rs = static_cast<size_t>(sr / 15);
    const size_t rn = static_cast<size_t>(sr / 6);
    // A bipolar pitch LFO spends as much time sharp as flat, so a long window's
    // zero-crossing total stays near the centre note. The short-window spread does not.
    auto zcSpread = [&](const std::vector<float>& buf) {
      const size_t win = static_cast<size_t>(sr / 40);
      int low = 100000;
      int high = 0;
      for (size_t i = rs; i + win <= rs + rn; i += win / 2)
      {
        const int zc = CountZeroCrossings(buf, i, win);
        low = std::min(low, zc);
        high = std::max(high, zc);
      }
      return high - low;
    };
    const int spreadOff = zcSpread(destOff);
    const int spreadPitch = zcSpread(destPitch);
    if (!(spreadPitch > spreadOff + 2))
      std::printf("  lfo pitch zc spread off %d pitch %d\n", spreadOff, spreadPitch);
    Expect(spreadPitch > spreadOff + 2, "lfo_pitch_moves_frequency");
    Expect(std::fabs(Rms(destPitch, rs, rn) - Rms(destOff, rs, rn)) < Rms(destOff, rs, rn) * 0.35,
           "lfo_pitch_does_not_tremolo");
    Expect(tremoloStd(destLevel) > tremoloStd(destOff) * 3.0, "lfo_level_tremolos");
    Expect(MeanAbsDiff(destShape, destOff, rs) > 0.01, "lfo_shape_moves_morph");

    Params subOnly = SteadyPatch();
    subOnly.osc1Level = 0.f;
    subOnly.osc2Level = 0.f;
    subOnly.subLevel = 0.7f;
    subOnly.lfoDest = ModDest::Shape;
    subOnly.lfoAmount = 1.f;
    subOnly.lfoRateHz = 7.f;
    eng.Reset(sr);
    eng.SetParams(subOnly);
    auto shapeSub = RenderHold(eng, 60, 0.8f, sr / 5);
    subOnly.lfoDest = ModDest::Off;
    eng.Reset(sr);
    eng.SetParams(subOnly);
    auto offSub = RenderHold(eng, 60, 0.8f, sr / 5);
    Expect(Identical(shapeSub, offSub), "lfo_shape_does_not_move_sub");

    Params cut = SteadyPatch();
    cut.osc1Shape = 0.75f;
    cut.filterCutoffHz = 900.f;
    cut.lfoDest = ModDest::Cutoff;
    cut.lfoAmount = 0.85f;
    cut.lfoRateHz = 3.f;
    eng.Reset(sr);
    eng.SetParams(cut);
    auto cutOn = RenderHold(eng, 60, 0.8f, sr / 2);
    cut.lfoDest = ModDest::Off;
    eng.Reset(sr);
    eng.SetParams(cut);
    auto cutOff = RenderHold(eng, 60, 0.8f, sr / 2);
    Expect(MeanAbsDiff(cutOn, cutOff, static_cast<size_t>(sr / 20)) > 0.01, "lfo_cutoff_moves_filter");
    Expect(AllFinite(destCutoff), "lfo_routes_stay_finite");
  }

  {
    Params mw = SteadyPatch();
    mw.modWheelDest = ModDest::Pitch;
    mw.modWheelAmount = 1.f;
    eng.Reset(sr);
    eng.SetModWheel(0.f);
    eng.SetParams(mw);
    auto wheelDown = RenderHold(eng, 60, 0.85f, sr / 5);
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto wheelUp = RenderHold(eng, 60, 0.85f, sr / 5);
    const size_t s = static_cast<size_t>(sr / 20);
    const size_t n = static_cast<size_t>(sr / 10);
    const int zcDown = CountZeroCrossings(wheelDown, s, n);
    const int zcUp = CountZeroCrossings(wheelUp, s, n);
    if (!(zcUp > zcDown * 1.7))
      std::printf("  mod wheel zc down %d up %d\n", zcDown, zcUp);
    Expect(zcUp > zcDown * 1.7, "mod_wheel_amount_sets_depth");

    mw.modWheelAmount = 0.f;
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto zeroAmt = RenderHold(eng, 60, 0.85f, sr / 5);
    Expect(Identical(wheelDown, zeroAmt), "mod_wheel_zero_amount_ignores_cc1");

    mw.modWheelAmount = 1.f;
    mw.modWheelDest = ModDest::Off;
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto destOff = RenderHold(eng, 60, 0.85f, sr / 5);
    Expect(Identical(wheelDown, destOff), "mod_wheel_off_ignores_cc1");

    mw.modWheelDest = ModDest::Level;
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto level = RenderHold(eng, 60, 0.85f, sr / 5);
    Expect(Rms(level, s, n) > Rms(wheelDown, s, n) * 1.4, "mod_wheel_level_changes_amplitude");
    Expect(std::abs(CountZeroCrossings(level, s, n) - zcDown) < 6, "mod_wheel_level_does_not_change_pitch");

    mw.modWheelDest = ModDest::Cutoff;
    mw.osc1Shape = 0.8f;
    mw.filterCutoffHz = 700.f;
    eng.Reset(sr);
    eng.SetModWheel(0.f);
    eng.SetParams(mw);
    auto cutDown = RenderHold(eng, 60, 0.85f, sr / 5);
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto cutUp = RenderHold(eng, 60, 0.85f, sr / 5);
    Expect(Rms(cutUp, s, n) > Rms(cutDown, s, n) * 1.3, "mod_wheel_cutoff_opens_filter");

    mw.modWheelDest = ModDest::Shape;
    mw.osc1Shape = 0.2f;
    mw.filterCutoffHz = 14000.f;
    eng.Reset(sr);
    eng.SetModWheel(0.f);
    eng.SetParams(mw);
    auto shapeDown = RenderHold(eng, 60, 0.85f, sr / 6);
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto shapeUp = RenderHold(eng, 60, 0.85f, sr / 6);
    Expect(MeanAbsDiff(shapeDown, shapeUp, s) > 0.01, "mod_wheel_shape_moves_morph");
    mw.osc1Level = 0.f;
    mw.subLevel = 0.8f;
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto shapeSub = RenderHold(eng, 60, 0.85f, sr / 8);
    mw.modWheelDest = ModDest::Off;
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(mw);
    auto shapeSubOff = RenderHold(eng, 60, 0.85f, sr / 8);
    Expect(Identical(shapeSub, shapeSubOff), "mod_wheel_shape_does_not_move_sub");
  }

  {
    Params vel = SteadyPatch();
    vel.velToAmp = 0.f;
    eng.Reset(sr);
    eng.SetParams(vel);
    auto plain = RenderHold(eng, 60, 0.15f, sr / 5);
    vel.velToAmp = 1.f;
    eng.Reset(sr);
    eng.SetParams(vel);
    auto dipped = RenderHold(eng, 60, 0.15f, sr / 5);
    const size_t s = static_cast<size_t>(sr / 20);
    const size_t n = static_cast<size_t>(sr / 10);
    const double plainR = Rms(plain, s, n);
    const double dippedR = Rms(dipped, s, n);
    if (!(dippedR < plainR * 0.55))
      std::printf("  vel amp rms plain %g dipped %g\n", plainR, dippedR);
    Expect(dippedR < plainR * 0.55 && dippedR > plainR * 0.1, "vel_to_amp_changes_level");
    Expect(std::abs(CountZeroCrossings(plain, s, n) - CountZeroCrossings(dipped, s, n)) < 6,
           "vel_to_amp_does_not_change_pitch");

    vel.velToAmp = 0.f;
    eng.Reset(sr);
    eng.SetParams(vel);
    auto mid0 = RenderHold(eng, 60, 0.5f, sr / 8);
    vel.velToAmp = 1.f;
    eng.Reset(sr);
    eng.SetParams(vel);
    auto mid1 = RenderHold(eng, 60, 0.5f, sr / 8);
    Expect(Identical(mid0, mid1), "vel_to_amp_is_inert_at_half_velocity");

    Params vf = SteadyPatch();
    vf.osc1Shape = 0.75f;
    vf.filterCutoffHz = 600.f;
    vf.velToFilter = 0.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto dark = RenderHold(eng, 60, 0.95f, sr / 5);
    vf.velToFilter = 1.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto bright = RenderHold(eng, 60, 0.95f, sr / 5);
    Expect(Rms(bright, s, n) > Rms(dark, s, n) * 1.25, "vel_to_filter_changes_cutoff");

    vf.velToFilter = 0.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto half0 = RenderHold(eng, 60, 0.5f, sr / 8);
    vf.velToFilter = 1.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto half1 = RenderHold(eng, 60, 0.5f, sr / 8);
    Expect(Identical(half0, half1), "vel_to_filter_is_inert_at_half_velocity");

    vf.osc1Shape = 0.f;
    vf.filterCutoffHz = 16000.f;
    vf.velToFilter = 0.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto open0 = RenderHold(eng, 60, 0.95f, sr / 6);
    vf.velToFilter = 1.f;
    eng.Reset(sr);
    eng.SetParams(vf);
    auto open1 = RenderHold(eng, 60, 0.95f, sr / 6);
    Expect(std::abs(CountZeroCrossings(open0, s, n) - CountZeroCrossings(open1, s, n)) < 6,
           "vel_to_filter_does_not_retune_open_sine");
  }

  {
    Params leg = SteadyPatch();
    leg.polyMode = PolyMode::Mono;
    leg.monoLegato = false;
    leg.ampAttackMs = 4.f;
    leg.ampDecayMs = 600.f;
    leg.ampSustain = 0.15f;
    leg.filterCutoffHz = 14000.f;

    auto phrase = [&](bool legato) {
      Params q = leg;
      q.monoLegato = legato;
      eng.Reset(sr);
      eng.SetParams(q);
      const int hold = sr / 4;
      const int tail = sr / 8;
      std::vector<float> out(static_cast<size_t>(hold + tail), 0.f);
      eng.NoteOn(60, 0.9f);
      for (int i = 0; i < hold; ++i)
        out[static_cast<size_t>(i)] = eng.Process();
      eng.NoteOn(67, 0.9f);
      for (int i = 0; i < tail; ++i)
        out[static_cast<size_t>(hold + i)] = eng.Process();
      return out;
    };

    const auto retrig = phrase(false);
    const auto glide = phrase(true);
    const size_t after = static_cast<size_t>(sr / 4 + sr / 50);
    const size_t len = static_cast<size_t>(sr / 20);
    const double retrigR = Rms(retrig, after, len);
    const double glideR = Rms(glide, after, len);
    if (!(retrigR > glideR * 2.5))
      std::printf("  legato rms retrig %g glide %g\n", retrigR, glideR);
    Expect(retrigR > glideR * 2.5, "legato_skips_reattack");

    const int zcGlide = CountZeroCrossings(glide, after, len);
    const int zcBefore = CountZeroCrossings(glide, static_cast<size_t>(sr / 10), len);
    Expect(zcGlide > zcBefore * 1.2, "legato_still_changes_pitch");

    leg.polyMode = PolyMode::Poly;
    leg.monoLegato = true;
    eng.Reset(sr);
    eng.SetParams(leg);
    auto polyOn = RenderHold(eng, 60, 0.9f, sr / 6);
    leg.monoLegato = false;
    eng.Reset(sr);
    eng.SetParams(leg);
    auto polyOff = RenderHold(eng, 60, 0.9f, sr / 6);
    Expect(Identical(polyOn, polyOff), "legato_does_not_change_poly");
  }

  {
    eng.Reset(sr);
    Params sp = SteadyPatch();
    sp.osc2Level = 0.4f;
    eng.SetParams(sp);
    eng.SetModWheel(0.4f);
    eng.NoteOn(60, 0.7f);
    bool ok = true;
    for (int step = 0; step < 80; ++step)
    {
      const float t = static_cast<float>(step) / 79.f;
      sp.oscSync = (step % 2) == 0;
      sp.oscRing = (step % 3) == 0;
      sp.filterKeyTrack = t;
      sp.filterDrive = t;
      sp.lfoRateHz = 0.05f + t * 20.f;
      sp.lfoShape = static_cast<LfoShape>(step % 5);
      sp.lfoDest = static_cast<ModDest>(step % 5);
      sp.lfoAmount = t * 2.f - 1.f;
      sp.modWheelDest = static_cast<ModDest>((step / 3) % 5);
      sp.modWheelAmount = 0.5f - t;
      sp.velToAmp = t;
      sp.velToFilter = 1.f - t;
      sp.monoLegato = (step % 2) == 1;
      eng.SetParams(sp);
      eng.SetModWheel(t);
      for (int i = 0; i < 128; ++i)
      {
        const float s = eng.Process();
        if (!std::isfinite(s) || std::fabs(s) > 8.f)
        {
          ok = false;
          break;
        }
      }
      if (!ok)
        break;
    }
    Expect(ok, "u9_param_sweep_finite");
  }

  std::printf("\nsynth_offline_test: %d passed, %d failed (total %d)\n", gPass, gFail, gPass + gFail);
  return gFail == 0 ? 0 : 1;
}
