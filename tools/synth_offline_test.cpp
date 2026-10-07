// Offline core-synth verification — not linked into the plugin binary.
#include "../src/drums/DrumSynth.h"
#include "../src/fx/PatchEffects.h"
#include "../src/synth/CoreSynth.h"

#include <cmath>
#include <cstdio>
#include <set>
#include <vector>

using keith::synth::EngineMode;
using keith::synth::FilterType;
using keith::synth::LfoShape;
using keith::synth::ModDest;
using keith::synth::ModRoute;
using keith::synth::ModSignals;
using keith::synth::ModSource;
using keith::synth::OfflineEngine;
using keith::synth::Params;
using keith::synth::PolyMode;
using keith::fx::InsertAlgorithm;
using keith::fx::InsertEffect;
using keith::fx::MasterAlgorithm;
using keith::fx::MasterEffect;
using keith::fx::SmoothedGain;
using keith::drums::Engine;
using keith::drums::KitParams;
using keith::drums::SlotParams;

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

struct StereoBuffer
{
  explicit StereoBuffer(size_t frames = 0) : left(frames), right(frames) {}
  std::vector<float> left;
  std::vector<float> right;
};

static StereoBuffer RenderDrumHit(int sampleRate, const KitParams& params, int slot,
                                  float velocity, int frames)
{
  Engine engine;
  engine.Reset(sampleRate);
  engine.SetParams(params);
  engine.Trigger(slot, velocity);
  StereoBuffer output(static_cast<size_t>(frames));
  for (int i = 0; i < frames; ++i)
    engine.ProcessFrame(output.left[static_cast<size_t>(i)], output.right[static_cast<size_t>(i)]);
  return output;
}

static bool AllFinite(const StereoBuffer& x)
{
  return AllFinite(x.left) && AllFinite(x.right);
}

static double StereoPeak(const StereoBuffer& x)
{
  return std::max(PeakAbs(x.left), PeakAbs(x.right));
}

static double DifferenceRms(const std::vector<float>& x, size_t start, size_t count)
{
  if (count < 2)
    return 0.;
  double sum = 0.;
  for (size_t i = start + 1; i < start + count; ++i)
  {
    const double difference = static_cast<double>(x[i]) - static_cast<double>(x[i - 1]);
    sum += difference * difference;
  }
  return std::sqrt(sum / static_cast<double>(count - 1));
}

template<typename Effect>
static StereoBuffer ProcessEffect(Effect& effect, const typename Effect::Params& params,
                                  const StereoBuffer& input, double sampleRate)
{
  StereoBuffer output = input;
  effect.SetParams(params);
  effect.Reset(sampleRate, 64);
  float* channels[2] = {output.left.data(), output.right.data()};
  effect.ProcessBlock(channels, 2, static_cast<int>(output.left.size()));
  return output;
}

static StereoBuffer SineStereo(int frames, double sampleRate, double leftHz, double rightHz,
                               float amplitude = 0.5f)
{
  StereoBuffer out(static_cast<size_t>(frames));
  for (int i = 0; i < frames; ++i)
  {
    out.left[static_cast<size_t>(i)] =
      amplitude * std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * leftHz * i / sampleRate));
    out.right[static_cast<size_t>(i)] =
      amplitude * std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * rightHz * i / sampleRate));
  }
  return out;
}

static double Mean(const std::vector<float>& x, size_t start, size_t count)
{
  double sum = 0.;
  for (size_t i = 0; i < count; ++i)
    sum += x[start + i];
  return count ? sum / static_cast<double>(count) : 0.;
}

static double StereoDifferenceRms(const StereoBuffer& x, size_t start, size_t count)
{
  double sum = 0.;
  for (size_t i = 0; i < count; ++i)
  {
    const double difference = x.left[start + i] - x.right[start + i];
    sum += difference * difference;
  }
  return count ? std::sqrt(sum / static_cast<double>(count)) : 0.;
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

  // U13: a second, independent LFO. Driving LFO2 while LFO1 is Off must produce the same
  // kind of modulation, and an LFO2 routed to the FM depth must move the timbre.
  {
    auto lfo2Std = [&](const std::vector<float>& buf) {
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
    auto lfo2Modulated = [&](ModDest dest, float amount) {
      Params q = SteadyPatch();
      q.lfo2RateHz = 6.f;
      q.lfo2Dest = dest;
      q.lfo2Amount = amount;
      q.osc1Shape = 0.15f;
      q.filterCutoffHz = 4000.f;
      eng.Reset(sr);
      eng.SetParams(q);
      return RenderHold(eng, 60, 0.8f, sr / 3);
    };
    const auto l2Off = lfo2Modulated(ModDest::Off, 1.f);
    const auto l2Level = lfo2Modulated(ModDest::Level, 0.8f);
    const auto l2Cutoff = lfo2Modulated(ModDest::Cutoff, 1.f);
    const auto l2Zero = lfo2Modulated(ModDest::Cutoff, 0.f);
    Expect(Identical(l2Off, l2Zero), "lfo2_off_and_zero_amount_match");
    Expect(lfo2Std(l2Level) > lfo2Std(l2Off) * 3.0, "lfo2_level_tremolos");
    Expect(MeanAbsDiff(l2Cutoff, l2Off, static_cast<size_t>(sr / 20)) > 0.01,
           "lfo2_cutoff_moves_filter");
    Expect(AllFinite(l2Level) && AllFinite(l2Cutoff), "lfo2_routes_stay_finite");

    // LFO2 must be independent of LFO1: with only LFO2 routed, the output moves; with LFO1
    // routed to the same dest, LFO2 Off, it also moves, and the two are not the same signal.
    Params only1 = SteadyPatch();
    only1.lfoRateHz = 3.f;
    only1.lfoDest = ModDest::Level;
    only1.lfoAmount = 0.8f;
    Params only2 = only1;
    only2.lfoDest = ModDest::Off;
    only2.lfo2RateHz = 3.f;
    only2.lfo2Dest = ModDest::Level;
    only2.lfo2Amount = 0.8f;
    eng.Reset(sr);
    eng.SetParams(only1);
    auto r1 = RenderHold(eng, 60, 0.8f, sr / 3);
    eng.Reset(sr);
    eng.SetParams(only2);
    auto r2 = RenderHold(eng, 60, 0.8f, sr / 3);
    Expect(lfo2Std(r1) > 0.01 && lfo2Std(r2) > 0.01, "lfo1_and_lfo2_both_modulate");

    // FM destination: an LFO2 routed to the FM depth moves the oscillator's timbre.
    Params fmBase = SteadyPatch();
    fmBase.osc1Shape = 0.5f;
    fmBase.osc2Level = 0.8f;
    fmBase.fmAmount = 0.5f;
    fmBase.filterCutoffHz = 14000.f;
    eng.Reset(sr);
    eng.SetParams(fmBase);
    auto fmPlain = RenderHold(eng, 60, 0.8f, sr / 3);
    Params fmMod = fmBase;
    fmMod.lfo2RateHz = 0.2f;
    fmMod.lfo2Dest = ModDest::Fm;
    fmMod.lfo2Amount = 0.9f;
    eng.Reset(sr);
    eng.SetParams(fmMod);
    auto fmSwing = RenderHold(eng, 60, 0.8f, sr / 3);
    Expect(MeanAbsDiff(fmPlain, fmSwing, static_cast<size_t>(sr / 20)) > 0.001,
           "lfo2_fm_moves_timbre");
    Expect(AllFinite(fmSwing), "lfo2_fm_stays_finite");
  }

  // U15 modulation matrix. These assertions exercise the same ApplyModMatrix helper used by
  // Voice::Process, so each slot must move only the destination selected by the stored ModDest.
  {
    const ModSignals signals {0.5f, -0.75f, 0.8f, 0.6f, 0.4f};
    const ModSource sources[4] = {
      ModSource::Lfo1, ModSource::Lfo2, ModSource::ModWheel, ModSource::Velocity
    };
    const ModDest destinations[4] = {
      ModDest::Cutoff, ModDest::Pitch, ModDest::Level, ModDest::Shape
    };

    for (int slot = 0; slot < 4; ++slot)
    {
      ModRoute routes[keith::synth::kNumModSlots] {};
      routes[slot].source = sources[slot];
      routes[slot].destination = destinations[slot];
      routes[slot].amount = 0.75f;
      float cutoff = 0.f, pitch = 0.f, shape = 0.f, level = 0.f, fm = 0.f;
      const bool active = keith::synth::ApplyModMatrix(
        routes, signals, cutoff, pitch, shape, level, fm);
      const float values[5] = {cutoff, pitch, shape, level, fm};
      const int target = destinations[slot] == ModDest::Cutoff ? 0 :
                         destinations[slot] == ModDest::Pitch ? 1 :
                         destinations[slot] == ModDest::Shape ? 2 : 3;
      char positive[64];
      char negative[64];
      std::snprintf(positive, sizeof(positive), "mod_slot_%d_destination_moves", slot + 1);
      std::snprintf(negative, sizeof(negative),
                    "mod_slot_%d_unrelated_destination_unchanged", slot + 1);
      Expect(active && std::fabs(values[target]) > 0.01f, positive);
      Expect(values[(target + 1) % 5] == 0.f, negative);
    }

    ModRoute keyRoute[keith::synth::kNumModSlots] {};
    keyRoute[0] = {ModSource::Key, ModDest::Fm, 1.f};
    float cutoff = 0.f, pitch = 0.f, shape = 0.f, level = 0.f, fm = 0.f;
    keith::synth::ApplyModMatrix(keyRoute, signals, cutoff, pitch, shape, level, fm);
    Expect(fm == signals.key && cutoff == 0.f && pitch == 0.f &&
           shape == 0.f && level == 0.f, "mod_key_source_routes_to_fm_only");
  }

  {
    ModRoute routes[keith::synth::kNumModSlots] {};
    routes[0] = {ModSource::Lfo1, ModDest::Cutoff, 1.f};
    routes[1] = {ModSource::Velocity, ModDest::Cutoff, 0.5f};
    const ModSignals signals {0.25f, 0.f, 0.f, 0.5f, 0.f};
    float cutoff = 0.f, pitch = 0.f, shape = 0.f, level = 0.f, fm = 0.f;
    keith::synth::ApplyModMatrix(routes, signals, cutoff, pitch, shape, level, fm);
    Expect(std::fabs(cutoff - 2.f) < 1.0e-6f && pitch == 0.f && shape == 0.f &&
           level == 0.f && fm == 0.f, "mod_slots_sum_on_shared_destination");

    for (ModRoute& route : routes)
      route = {ModSource::Lfo1, ModDest::Cutoff, 1.f};
    cutoff = pitch = shape = level = fm = 0.f;
    keith::synth::ApplyModMatrix(
      routes, ModSignals {1.f, 1.f, 1.f, 1.f, 1.f},
      cutoff, pitch, shape, level, fm);
    Expect(cutoff == 4.f, "mod_shared_destination_sum_is_bounded");
  }

  {
    ModRoute routes[keith::synth::kNumModSlots] {};
    routes[0] = {ModSource::Off, ModDest::Cutoff, 1.f};
    routes[1] = {ModSource::Lfo1, ModDest::Pitch, 0.f};
    float cutoff = 0.25f, pitch = -0.5f, shape = 0.1f, level = 0.2f, fm = -0.3f;
    const bool active = keith::synth::ApplyModMatrix(
      routes, ModSignals {1.f, 1.f, 1.f, 1.f, 1.f},
      cutoff, pitch, shape, level, fm);
    Expect(!active && cutoff == 0.25f && pitch == -0.5f && shape == 0.1f &&
           level == 0.2f && fm == -0.3f, "mod_off_and_zero_amount_are_inert");
  }

  {
    Params maximum = SteadyPatch();
    maximum.osc1Shape = 0.6f;
    maximum.osc2Level = 0.8f;
    maximum.fmAmount = 1.f;
    maximum.filterCutoffHz = 1200.f;
    maximum.filterReso = 1.f;
    maximum.lfoRateHz = 30.f;
    maximum.lfo2RateHz = 30.f;
    maximum.modMatrix[0] = {ModSource::Lfo1, ModDest::Cutoff, 1.f};
    maximum.modMatrix[1] = {ModSource::Lfo2, ModDest::Pitch, 1.f};
    maximum.modMatrix[2] = {ModSource::ModWheel, ModDest::Level, 1.f};
    maximum.modMatrix[3] = {ModSource::Key, ModDest::Fm, 1.f};
    eng.Reset(sr);
    eng.SetModWheel(1.f);
    eng.SetParams(maximum);
    const auto output = RenderHold(eng, 127, 1.f, sr);
    Expect(AllFinite(output), "mod_max_all_slots_is_finite");
  }

  {
    Params legacy = SteadyPatch();
    legacy.osc1Shape = 0.25f;
    legacy.osc2Level = 0.7f;
    legacy.filterCutoffHz = 1800.f;
    legacy.lfoRateHz = 3.f;
    legacy.lfoDest = ModDest::Pitch;
    legacy.lfoAmount = 0.35f;
    legacy.lfo2RateHz = 5.f;
    legacy.lfo2Dest = ModDest::Cutoff;
    legacy.lfo2Amount = 0.4f;
    legacy.modWheelDest = ModDest::Level;
    legacy.modWheelAmount = 0.25f;

    auto renderLegacy = [&](const Params& params) {
      eng.Reset(sr);
      eng.SetModWheel(0.8f);
      eng.SetParams(params);
      return RenderHold(eng, 60, 0.9f, sr / 2);
    };
    const auto before = renderLegacy(legacy);
    Params matrixOff = legacy;
    for (int slot = 0; slot < keith::synth::kNumModSlots; ++slot)
    {
      matrixOff.modMatrix[slot].source = ModSource::Off;
      matrixOff.modMatrix[slot].destination = static_cast<ModDest>(1 + slot);
      matrixOff.modMatrix[slot].amount = 1.f;
    }
    const auto after = renderLegacy(matrixOff);
    Expect(Identical(before, after) && AllFinite(after),
           "mod_matrix_does_not_disturb_existing_routings");
  }

  {
    bool preserved = true;
    for (int preset = 0; preset < 103; ++preset)
    {
      Params existing = SteadyPatch();
      existing.osc1Shape = static_cast<float>(preset % 5) * 0.2f;
      existing.osc2Level = static_cast<float>(preset % 4) * 0.15f;
      existing.filterCutoffHz = 900.f + static_cast<float>(preset) * 120.f;
      existing.masterLevel = 0.35f + static_cast<float>(preset % 3) * 0.1f;
      existing.lfoDest = static_cast<ModDest>(preset % 6);
      existing.lfoAmount = static_cast<float>((preset % 7) - 3) / 5.f;
      existing.lfo2Dest = static_cast<ModDest>((preset + 2) % 6);
      existing.lfo2Amount = static_cast<float>((preset % 5) - 2) / 4.f;
      existing.modWheelDest = static_cast<ModDest>((preset + 4) % 6);
      existing.modWheelAmount = 0.3f;

      Params zeroAmounts = existing;
      for (int slot = 0; slot < keith::synth::kNumModSlots; ++slot)
      {
        zeroAmounts.modMatrix[slot].source = static_cast<ModSource>(1 + (slot % 5));
        zeroAmounts.modMatrix[slot].destination = static_cast<ModDest>(1 + (slot % 5));
        zeroAmounts.modMatrix[slot].amount = 0.f;
      }

      auto renderPreset = [&](const Params& params) {
        OfflineEngine presetEngine;
        presetEngine.Reset(sr);
        presetEngine.SetModWheel(0.7f);
        presetEngine.SetParams(params);
        return RenderHold(presetEngine, 48 + preset % 36, 0.8f, 2048);
      };
      const auto oldSound = renderPreset(existing);
      const auto newSound = renderPreset(zeroAmounts);
      preserved = preserved && Identical(oldSound, newSound) && AllFinite(newSound);
    }
    Expect(preserved, "mod_new_defaults_are_inert");
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

  // U10 insert and master effects. Every algorithm gets an audible measurement and a
  // negative assertion that rules out an unrelated or no-op implementation.
  {
    const StereoBuffer input = SineStereo(sr, sr, 1000., 1000., 0.18f);
    InsertEffect<float> effect;
    InsertEffect<float>::Params drive {};
    drive.algorithm = InsertAlgorithm::Drive;
    drive.a = 1.f;
    drive.b = 1.f;
    drive.c = 0.5f;
    drive.mix = 1.f;
    const StereoBuffer output = ProcessEffect(effect, drive, input, sr);
    const size_t start = static_cast<size_t>(sr / 10);
    const size_t count = static_cast<size_t>(sr / 2);
    const double dryFundamental = GoertzelMag(input.left, start, count, 1000., sr);
    const double dryThird = GoertzelMag(input.left, start, count, 3000., sr);
    const double wetFundamental = GoertzelMag(output.left, start, count, 1000., sr);
    const double wetThird = GoertzelMag(output.left, start, count, 3000., sr);
    const double dryRatio = dryThird / std::max(1.0e-12, dryFundamental);
    const double wetRatio = wetThird / std::max(1.0e-12, wetFundamental);
    Expect(wetRatio > dryRatio + 0.01, "drive_adds_harmonics");

    const int dryCrossings = CountZeroCrossings(input.left, start, count);
    const int wetCrossings = CountZeroCrossings(output.left, start, count);
    Expect(std::abs(wetCrossings - dryCrossings) < 4, "drive_does_not_retune");
  }

  {
    const StereoBuffer input = SineStereo(sr, sr, 440., 440., 0.35f);
    InsertEffect<float> effect;
    InsertEffect<float>::Params chorus {};
    chorus.algorithm = InsertAlgorithm::Chorus;
    chorus.a = 0.8f;
    chorus.b = 1.f;
    chorus.c = 1.f;
    chorus.mix = 1.f;
    const StereoBuffer output = ProcessEffect(effect, chorus, input, sr);
    const size_t start = static_cast<size_t>(sr / 10);
    const size_t count = static_cast<size_t>(sr / 2);
    Expect(StereoDifferenceRms(output, start, count) > 0.05, "chorus_modulates_delay");
    Expect(std::fabs(Mean(output.left, start, count)) < 0.002 &&
           std::fabs(Mean(output.right, start, count)) < 0.002,
           "chorus_does_not_add_dc");
  }

  {
    StereoBuffer impulse(static_cast<size_t>(sr));
    impulse.left[0] = 1.f;
    impulse.right[0] = 1.f;
    InsertEffect<float> effect;
    InsertEffect<float>::Params delay {};
    delay.algorithm = InsertAlgorithm::Delay;
    delay.a = 0.5f;
    delay.b = 0.f;
    delay.c = 1.f;
    delay.mix = 1.f;
    const StereoBuffer output = ProcessEffect(effect, delay, impulse, sr);
    const int expected = static_cast<int>((0.02 + 0.25 * 0.98) * sr);
    const size_t around = static_cast<size_t>(expected);
    const double repeatPeak = PeakAbs(std::vector<float>(
      output.left.begin() + static_cast<ptrdiff_t>(around - 2),
      output.left.begin() + static_cast<ptrdiff_t>(around + 3)));
    Expect(repeatPeak > 0.5, "delay_repeats_at_set_time");
    Expect(PeakAbs(std::vector<float>(output.left.begin() + 1,
                                     output.left.begin() + static_cast<ptrdiff_t>(around - 4))) < 1.0e-7,
           "delay_has_no_early_repeat");
  }

  {
    StereoBuffer ramp(1024);
    for (size_t i = 0; i < ramp.left.size(); ++i)
    {
      const float value = static_cast<float>(i) / static_cast<float>(ramp.left.size() - 1) * 2.f - 1.f;
      ramp.left[i] = value;
      ramp.right[i] = value;
    }

    InsertEffect<float> effect;
    InsertEffect<float>::Params crush {};
    crush.algorithm = InsertAlgorithm::Crush;
    crush.a = 0.f;
    crush.b = 1.f;
    crush.c = 0.f;
    crush.mix = 1.f;
    const StereoBuffer lowBits = ProcessEffect(effect, crush, ramp, sr);
    std::set<int> levels;
    for (float value : lowBits.left)
      levels.insert(static_cast<int>(std::lround(value * 100000.f)));
    Expect(levels.size() <= 32 && levels.size() >= 12, "crush_quantises_levels");

    crush.a = 1.f;
    const StereoBuffer highBits = ProcessEffect(effect, crush, ramp, sr);
    const double lowError = MeanAbsDiff(lowBits.left, ramp.left, 0);
    const double highError = MeanAbsDiff(highBits.left, ramp.left, 0);
    Expect(highError < lowError * 0.05, "crush_bits_control_reduces_error");
  }

  {
    StereoBuffer input(static_cast<size_t>(sr));
    for (int i = 0; i < sr; ++i)
    {
      const float amplitude = i < sr / 2 ? 0.1f : 0.9f;
      const float value = amplitude *
        std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * 500.0 * i / sr));
      input.left[static_cast<size_t>(i)] = value;
      input.right[static_cast<size_t>(i)] = value;
    }

    MasterEffect<float> effect;
    MasterEffect<float>::Params compressor {};
    compressor.algorithm = MasterAlgorithm::Compressor;
    compressor.a = 0.5f;
    compressor.b = 1.f;
    compressor.c = 0.4f;
    compressor.mix = 1.f;
    const StereoBuffer output = ProcessEffect(effect, compressor, input, sr);
    const size_t window = static_cast<size_t>(sr / 5);
    const double dryRange = Rms(input.left, static_cast<size_t>(sr * 3 / 4), window) /
                            Rms(input.left, static_cast<size_t>(sr / 4), window);
    const double wetRange = Rms(output.left, static_cast<size_t>(sr * 3 / 4), window) /
                            Rms(output.left, static_cast<size_t>(sr / 4), window);
    Expect(wetRange < dryRange * 0.45, "comp_reduces_dynamic_range");

    const StereoBuffer quiet = SineStereo(sr / 4, sr, 300., 300., 0.2f);
    compressor.a = 1.f;
    const StereoBuffer untouched = ProcessEffect(effect, compressor, quiet, sr);
    Expect(Identical(quiet.left, untouched.left) && Identical(quiet.right, untouched.right),
           "comp_below_threshold_is_unchanged");
  }

  {
    const StereoBuffer hot = SineStereo(sr / 2, sr, 220., 220., 2.f);
    MasterEffect<float> effect;
    MasterEffect<float>::Params limiter {};
    limiter.algorithm = MasterAlgorithm::Limiter;
    limiter.a = 0.5f;
    limiter.b = 0.5f;
    limiter.c = 0.f;
    limiter.mix = 1.f;
    const StereoBuffer bounded = ProcessEffect(effect, limiter, hot, sr);
    const double ceiling = std::pow(10.0, -9.0 / 20.0);
    Expect(PeakAbs(bounded.left) <= ceiling + 1.0e-6, "limit_bounds_peak");

    const StereoBuffer quiet = SineStereo(sr / 4, sr, 220., 220., 0.2f);
    limiter.a = 1.f;
    const StereoBuffer untouched = ProcessEffect(effect, limiter, quiet, sr);
    Expect(Identical(quiet.left, untouched.left) && Identical(quiet.right, untouched.right),
           "limit_below_ceiling_is_unchanged");
  }

  {
    StereoBuffer input(static_cast<size_t>(sr / 2));
    for (size_t i = 0; i < input.left.size(); ++i)
    {
      const float common = 0.35f *
        std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * 400.0 * i / sr));
      const float side = 0.15f *
        std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * 900.0 * i / sr));
      input.left[i] = common + side;
      input.right[i] = common - side;
    }

    MasterEffect<float> effect;
    MasterEffect<float>::Params width {};
    width.algorithm = MasterAlgorithm::Width;
    width.a = 1.f;
    width.b = 0.f;
    width.c = 0.5f;
    width.mix = 1.f;
    const StereoBuffer widened = ProcessEffect(effect, width, input, sr);
    const size_t start = static_cast<size_t>(sr / 20);
    const size_t count = widened.left.size() - start;
    Expect(StereoDifferenceRms(widened, start, count) >
             StereoDifferenceRms(input, start, count) * 1.7,
           "width_changes_stereo_correlation");

    const StereoBuffer mono = SineStereo(sr / 4, sr, 500., 500., 0.3f);
    const StereoBuffer monoOut = ProcessEffect(effect, width, mono, sr);
    Expect(Identical(mono.left, monoOut.left) && Identical(mono.right, monoOut.right),
           "width_does_not_invent_side_from_mono");
  }

  {
    StereoBuffer input(static_cast<size_t>(sr));
    for (int i = 0; i < sr; ++i)
    {
      const float value =
        0.2f * std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * 100.0 * i / sr)) +
        0.2f * std::sin(static_cast<float>(2.0 * 3.14159265358979323846 * 5000.0 * i / sr));
      input.left[static_cast<size_t>(i)] = value;
      input.right[static_cast<size_t>(i)] = value;
    }

    MasterEffect<float> effect;
    MasterEffect<float>::Params tilt {};
    tilt.algorithm = MasterAlgorithm::Tilt;
    tilt.a = 0.5f;
    tilt.b = 0.5f;
    tilt.c = 0.f;
    tilt.mix = 1.f;
    const StereoBuffer flat = ProcessEffect(effect, tilt, input, sr);
    tilt.a = 1.f;
    const StereoBuffer bright = ProcessEffect(effect, tilt, input, sr);
    const size_t start = static_cast<size_t>(sr / 10);
    const size_t count = static_cast<size_t>(sr / 2);
    const double flatRatio = GoertzelMag(flat.left, start, count, 5000., sr) /
                             GoertzelMag(flat.left, start, count, 100., sr);
    const double brightRatio = GoertzelMag(bright.left, start, count, 5000., sr) /
                               GoertzelMag(bright.left, start, count, 100., sr);
    if (!(brightRatio > flatRatio * 3.0))
      std::printf("  tilt high/low ratio flat %g bright %g\n", flatRatio, brightRatio);
    Expect(brightRatio > flatRatio * 3.0, "tilt_changes_spectral_balance");
    Expect(MeanAbsDiff(flat.left, input.left, start) < 1.0e-6,
           "tilt_center_without_drive_is_neutral");
  }

  {
    const StereoBuffer input = SineStereo(sr / 4, sr, 321., 654., 0.4f);
    InsertEffect<float> insert;
    InsertEffect<float>::Params insertOff {};
    insertOff.algorithm = InsertAlgorithm::Off;
    insertOff.a = insertOff.b = insertOff.c = 1.f;
    insertOff.mix = 1.f;
    const StereoBuffer insertOutput = ProcessEffect(insert, insertOff, input, sr);

    MasterEffect<float> master;
    MasterEffect<float>::Params masterOff {};
    masterOff.algorithm = MasterAlgorithm::Off;
    masterOff.a = masterOff.b = masterOff.c = 1.f;
    masterOff.mix = 1.f;
    const StereoBuffer masterOutput = ProcessEffect(master, masterOff, input, sr);
    Expect(Identical(input.left, insertOutput.left) && Identical(input.right, insertOutput.right) &&
           Identical(input.left, masterOutput.left) && Identical(input.right, masterOutput.right),
           "fx_off_is_exact_passthrough");

    insertOff.algorithm = InsertAlgorithm::Drive;
    insertOff.bypass = true;
    const StereoBuffer insertBypass = ProcessEffect(insert, insertOff, input, sr);
    masterOff.algorithm = MasterAlgorithm::Limiter;
    masterOff.bypass = true;
    const StereoBuffer masterBypass = ProcessEffect(master, masterOff, input, sr);
    Expect(Identical(input.left, insertBypass.left) && Identical(input.right, insertBypass.right) &&
           Identical(input.left, masterBypass.left) && Identical(input.right, masterBypass.right),
           "fx_bypass_is_exact_passthrough");
  }

  {
    const StereoBuffer input = SineStereo(sr / 4, sr, 271., 419., 0.25f);
    InsertEffect<float> effect;
    InsertEffect<float>::Params drive {};
    drive.algorithm = InsertAlgorithm::Drive;
    drive.a = 0.8f;
    drive.b = 0.7f;
    drive.c = 0.6f;
    drive.mix = 0.f;
    const StereoBuffer dry = ProcessEffect(effect, drive, input, sr);
    Expect(Identical(input.left, dry.left) && Identical(input.right, dry.right),
           "fx_mix_zero_is_exact_passthrough");

    drive.mix = 1.f;
    const StereoBuffer wet = ProcessEffect(effect, drive, input, sr);
    drive.mix = 0.5f;
    const StereoBuffer half = ProcessEffect(effect, drive, input, sr);
    bool trueBlend = true;
    for (size_t i = 0; i < input.left.size(); ++i)
    {
      const float expectedL = input.left[i] + (wet.left[i] - input.left[i]) * 0.5f;
      const float expectedR = input.right[i] + (wet.right[i] - input.right[i]) * 0.5f;
      trueBlend = trueBlend && std::fabs(half.left[i] - expectedL) < 1.0e-7f &&
                  std::fabs(half.right[i] - expectedR) < 1.0e-7f;
    }
    Expect(trueBlend, "fx_mix_is_true_dry_wet");
  }

  {
    StereoBuffer unity = SineStereo(sr / 8, sr, 440., 660., 0.2f);
    const StereoBuffer input = unity;
    SmoothedGain<float> outLevel;
    outLevel.Reset(sr, 1.f);
    float* unityChannels[2] = {unity.left.data(), unity.right.data()};
    outLevel.ProcessBlock(unityChannels, 2, static_cast<int>(unity.left.size()));
    Expect(Identical(input.left, unity.left) && Identical(input.right, unity.right),
           "fx_out_level_zero_db_is_inert");

    StereoBuffer boosted = input;
    const float plusTwelve = static_cast<float>(std::pow(10.0, 12.0 / 20.0));
    outLevel.Reset(sr, plusTwelve);
    float* boostChannels[2] = {boosted.left.data(), boosted.right.data()};
    outLevel.ProcessBlock(boostChannels, 2, static_cast<int>(boosted.left.size()));
    const double ratio = Rms(boosted.left, 0, boosted.left.size()) /
                         Rms(input.left, 0, input.left.size());
    Expect(std::fabs(ratio - plusTwelve) < 1.0e-5, "fx_out_level_applies_db_gain");
  }

  {
    InsertEffect<float>::Params insert1 {};
    InsertEffect<float>::Params insert2 {};
    MasterEffect<float>::Params master {};
    const double outLevelDb = 0.;
    const bool fields =
      insert1.algorithm == InsertAlgorithm::Off && insert1.a == 0.5f && insert1.b == 0.5f &&
      insert1.c == 0.5f && insert1.mix == 1.f && !insert1.bypass &&
      insert2.algorithm == InsertAlgorithm::Off && insert2.a == 0.5f && insert2.b == 0.5f &&
      insert2.c == 0.5f && insert2.mix == 1.f && !insert2.bypass &&
      master.algorithm == MasterAlgorithm::Off && master.a == 0.5f && master.b == 0.5f &&
      master.c == 0.5f && master.mix == 0.4f && !master.bypass && outLevelDb == 0.;
    Expect(fields, "fx_new_defaults_match_inert_values");

    const StereoBuffer input = SineStereo(sr / 3, sr, 333., 777., 0.4f);
    StereoBuffer output = input;
    InsertEffect<float> first;
    InsertEffect<float> second;
    MasterEffect<float> final;
    first.SetParams(insert1);
    second.SetParams(insert2);
    final.SetParams(master);
    first.Reset(sr, 64);
    second.Reset(sr, 64);
    final.Reset(sr, 64);
    SmoothedGain<float> outLevel;
    outLevel.Reset(sr, 1.f);
    float* channels[2] = {output.left.data(), output.right.data()};
    const int frames = static_cast<int>(output.left.size());
    first.ProcessBlock(channels, 2, frames);
    second.ProcessBlock(channels, 2, frames);
    final.ProcessBlock(channels, 2, frames);
    outLevel.ProcessBlock(channels, 2, frames);
    Expect(Identical(input.left, output.left) && Identical(input.right, output.right),
           "fx_new_defaults_are_inert");
  }

  {
    StereoBuffer output = SineStereo(sr * 2, sr, 97., 613., 4.f);
    InsertEffect<float> first;
    InsertEffect<float>::Params delay {};
    delay.algorithm = InsertAlgorithm::Delay;
    delay.a = delay.b = delay.c = delay.mix = 1.f;
    first.SetParams(delay);
    first.Reset(sr, 64);

    InsertEffect<float> second;
    InsertEffect<float>::Params crush {};
    crush.algorithm = InsertAlgorithm::Crush;
    crush.a = crush.b = crush.c = crush.mix = 1.f;
    second.SetParams(crush);
    second.Reset(sr, 64);

    MasterEffect<float> master;
    MasterEffect<float>::Params limiter {};
    limiter.algorithm = MasterAlgorithm::Limiter;
    limiter.a = limiter.b = limiter.c = limiter.mix = 1.f;
    master.SetParams(limiter);
    master.Reset(sr, 64);

    SmoothedGain<float> outLevel;
    outLevel.Reset(sr, static_cast<float>(std::pow(10.0, 12.0 / 20.0)));
    float* channels[2] = {output.left.data(), output.right.data()};
    const int frames = static_cast<int>(output.left.size());
    first.ProcessBlock(channels, 2, frames);
    second.ProcessBlock(channels, 2, frames);
    master.ProcessBlock(channels, 2, frames);
    outLevel.ProcessBlock(channels, 2, frames);
    Expect(AllFinite(output.left) && AllFinite(output.right) &&
             PeakAbs(output.left) <= 64. && PeakAbs(output.right) <= 64.,
           "fx_chain_all_max_is_finite");
  }

  {
    InsertEffect<float> effect;
    InsertEffect<float>::Params drive {};
    drive.algorithm = InsertAlgorithm::Drive;
    drive.a = 0.f;
    drive.b = 1.f;
    drive.c = 0.5f;
    drive.mix = 1.f;
    effect.SetParams(drive);
    effect.Reset(sr, 64);
    std::vector<float> left(2048, 0.2f);
    std::vector<float> right(2048, 0.2f);
    float* channels[2] = {left.data(), right.data()};
    effect.ProcessBlock(channels, 2, 1024);
    drive.a = 1.f;
    effect.SetParams(drive);
    float* changedChannels[2] = {left.data() + 1024, right.data() + 1024};
    effect.ProcessBlock(changedChannels, 2, 1024);
    const double firstChanged = std::fabs(static_cast<double>(left[1024]) - left[1023]);
    Expect(firstChanged < 0.01, "fx_parameter_changes_are_smoothed");
  }

  // U11 synthesized percussion.
  {
    const KitParams kit = keith::drums::DefaultKitParams();
    for (int slot = 0; slot < keith::drums::kNumSlots; ++slot)
    {
      const StereoBuffer output = RenderDrumHit(sr, kit, slot, 0.9f, sr * 3);
      const size_t tailStart = output.left.size() - static_cast<size_t>(sr / 20);
      const bool tailSilent = PeakAbs(std::vector<float>(output.left.begin() +
        static_cast<ptrdiff_t>(tailStart), output.left.end())) == 0. &&
        PeakAbs(std::vector<float>(output.right.begin() +
        static_cast<ptrdiff_t>(tailStart), output.right.end())) == 0.;
      char name[64];
      std::snprintf(name, sizeof(name), "drum_voice_%02d_finite_nonzero_tail", slot + 1);
      Expect(AllFinite(output) && StereoPeak(output) > 1.0e-5 && tailSilent, name);
    }
  }

  {
    KitParams kit = keith::drums::DefaultKitParams();
    const StereoBuffer soft = RenderDrumHit(sr, kit, 1, 0.2f, sr / 2);
    const StereoBuffer hard = RenderDrumHit(sr, kit, 1, 1.f, sr / 2);
    const size_t start = static_cast<size_t>(sr / 200);
    const size_t count = static_cast<size_t>(sr / 8);
    const double softRms = Rms(soft.left, start, count) + Rms(soft.right, start, count);
    const double hardRms = Rms(hard.left, start, count) + Rms(hard.right, start, count);
    Expect(hardRms > softRms * 2.0, "drum_velocity_changes_level");
  }

  {
    KitParams shortKit = keith::drums::DefaultKitParams();
    shortKit.slot[5].decay = 0.f;
    KitParams longKit = shortKit;
    longKit.slot[5].decay = 1.f;
    const StereoBuffer shortHit = RenderDrumHit(sr, shortKit, 5, 0.9f, sr * 2);
    const StereoBuffer longHit = RenderDrumHit(sr, longKit, 5, 0.9f, sr * 2);
    const size_t start = static_cast<size_t>(sr * 2 / 5);
    const size_t count = static_cast<size_t>(sr / 5);
    const double shortTail = Rms(shortHit.left, start, count) + Rms(shortHit.right, start, count);
    const double longTail = Rms(longHit.left, start, count) + Rms(longHit.right, start, count);
    Expect(longTail > std::max(1.0e-8, shortTail * 8.0), "drum_slot_decay_changes_length");
  }

  {
    KitParams lowKit = keith::drums::DefaultKitParams();
    lowKit.slot[9].tuneSemitones = -12.f;
    KitParams highKit = lowKit;
    highKit.slot[9].tuneSemitones = 12.f;
    const StereoBuffer low = RenderDrumHit(sr, lowKit, 9, 0.9f, sr / 2);
    const StereoBuffer high = RenderDrumHit(sr, highKit, 9, 0.9f, sr / 2);
    const size_t start = static_cast<size_t>(sr / 50);
    const size_t count = static_cast<size_t>(sr / 8);
    const int lowCrossings = CountZeroCrossings(low.left, start, count);
    const int highCrossings = CountZeroCrossings(high.left, start, count);
    if (!(highCrossings > lowCrossings * 1.4))
      std::printf("  drum tune crossings low %d high %d\n", lowCrossings, highCrossings);
    Expect(highCrossings > lowCrossings * 1.4, "drum_slot_tune_changes_pitch");
  }

  {
    KitParams darkKit = keith::drums::DefaultKitParams();
    darkKit.slot[1].filterHz = 600.f;
    KitParams brightKit = darkKit;
    brightKit.slot[1].filterHz = 18000.f;
    const StereoBuffer dark = RenderDrumHit(sr, darkKit, 1, 0.9f, sr / 2);
    const StereoBuffer bright = RenderDrumHit(sr, brightKit, 1, 0.9f, sr / 2);
    const size_t start = static_cast<size_t>(sr / 100);
    const size_t count = static_cast<size_t>(sr / 8);
    const double darkDifference = DifferenceRms(dark.left, start, count);
    const double brightDifference = DifferenceRms(bright.left, start, count);
    Expect(brightDifference > darkDifference * 2.0, "drum_slot_filter_changes_brightness");
  }

  {
    KitParams cleanKit = keith::drums::DefaultKitParams();
    cleanKit.slot[12].drive = 0.f;
    cleanKit.slot[12].filterHz = 18000.f;
    KitParams drivenKit = cleanKit;
    drivenKit.slot[12].drive = 1.f;
    const StereoBuffer clean = RenderDrumHit(sr, cleanKit, 12, 1.f, sr / 2);
    const StereoBuffer driven = RenderDrumHit(sr, drivenKit, 12, 1.f, sr / 2);
    const size_t start = static_cast<size_t>(sr / 50);
    const size_t count = static_cast<size_t>(sr / 8);
    const double cleanRoughness = DifferenceRms(clean.left, start, count) /
      std::max(1.0e-9, Rms(clean.left, start, count));
    const double drivenRoughness = DifferenceRms(driven.left, start, count) /
      std::max(1.0e-9, Rms(driven.left, start, count));
    const double waveformDifference = MeanAbsDiff(clean.left, driven.left, start);
    if (!(drivenRoughness > cleanRoughness * 1.08 && waveformDifference > 0.002))
      std::printf("  drum drive roughness clean %g driven %g diff %g\n",
                  cleanRoughness, drivenRoughness, waveformDifference);
    Expect(drivenRoughness > cleanRoughness * 1.08 &&
           waveformDifference > 0.002,
           "drum_slot_drive_adds_harmonics");
  }

  {
    KitParams kit = keith::drums::DefaultKitParams();
    kit.slot[0].mute = true;
    const StereoBuffer muted = RenderDrumHit(sr, kit, 0, 1.f, sr);
    Expect(StereoPeak(muted) == 0., "drum_slot_mute_is_silent");
  }

  {
    KitParams kit = keith::drums::DefaultKitParams();
    kit.slot[0].solo = true;
    Engine soloOnly;
    soloOnly.Reset(sr);
    soloOnly.SetParams(kit);
    soloOnly.Trigger(0, 0.9f);
    Engine withOther;
    withOther.Reset(sr);
    withOther.SetParams(kit);
    withOther.Trigger(0, 0.9f);
    withOther.Trigger(1, 0.9f);
    bool identical = true;
    for (int i = 0; i < sr; ++i)
    {
      float aL = 0.f, aR = 0.f, bL = 0.f, bR = 0.f;
      soloOnly.ProcessFrame(aL, aR);
      withOther.ProcessFrame(bL, bR);
      identical = identical && aL == bL && aR == bR;
    }
    Expect(identical, "drum_solo_mutes_others");
  }

  {
    KitParams leftKit = keith::drums::DefaultKitParams();
    leftKit.slot[0].pan = -1.f;
    KitParams rightKit = leftKit;
    rightKit.slot[0].pan = 1.f;
    const StereoBuffer left = RenderDrumHit(sr, leftKit, 0, 0.9f, sr / 2);
    const StereoBuffer right = RenderDrumHit(sr, rightKit, 0, 0.9f, sr / 2);
    const double leftL = Rms(left.left, 0, left.left.size());
    const double leftR = Rms(left.right, 0, left.right.size());
    const double rightL = Rms(right.left, 0, right.left.size());
    const double rightR = Rms(right.right, 0, right.right.size());
    Expect(leftL > leftR * 1000. && rightR > rightL * 1000., "drum_pan_shifts_balance");
  }

  {
    Params synthParams = SteadyPatch();
    synthParams.ampSustain = 1.f;
    OfflineEngine baseline;
    OfflineEngine routed;
    baseline.Reset(sr);
    routed.Reset(sr);
    baseline.SetParams(synthParams);
    routed.SetParams(synthParams);
    for (int note = 60; note < 76; ++note)
    {
      baseline.NoteOn(note, 0.8f);
      routed.NoteOn(note, 0.8f);
    }
    for (int i = 0; i < 1024; ++i)
    {
      baseline.Process();
      routed.Process();
    }

    Engine drums;
    drums.Reset(sr);
    drums.SetParams(keith::drums::DefaultKitParams());
    const int drumNote = 36;
    if (keith::drums::DestinationForMidiNote(drumNote) == keith::drums::MidiDestination::Drum)
      drums.Trigger(keith::drums::SlotForMidiNote(drumNote), 1.f);
    else
      routed.NoteOn(drumNote, 1.f);

    bool synthIdentical = true;
    double drumPeak = 0.;
    for (int i = 0; i < sr / 4; ++i)
    {
      synthIdentical = synthIdentical && baseline.Process() == routed.Process();
      float left = 0.f, right = 0.f;
      drums.ProcessFrame(left, right);
      drumPeak = std::max(drumPeak, std::max(std::fabs(static_cast<double>(left)),
                                            std::fabs(static_cast<double>(right))));
    }
    Expect(synthIdentical && routed.ActiveVoiceCount() == 16 && drumPeak > 0.001,
           "drum_notes_do_not_steal_synth_voices");
  }

  {
    auto sequence = [&](std::vector<float>& output) {
      Engine engine;
      engine.Reset(sr);
      engine.SetParams(keith::drums::DefaultKitParams());
      const int hitFrames = sr / 3;
      output.assign(static_cast<size_t>(hitFrames * 4), 0.f);
      for (int hit = 0; hit < 4; ++hit)
      {
        engine.Trigger(1, 0.9f);
        for (int i = 0; i < hitFrames; ++i)
        {
          float left = 0.f, right = 0.f;
          engine.ProcessFrame(left, right);
          output[static_cast<size_t>(hit * hitFrames + i)] = left;
        }
      }
    };
    std::vector<float> first;
    std::vector<float> second;
    sequence(first);
    sequence(second);
    const size_t hitFrames = static_cast<size_t>(sr / 3);
    std::vector<float> hitOne(first.begin(), first.begin() + static_cast<ptrdiff_t>(hitFrames));
    std::vector<float> hitTwo(first.begin() + static_cast<ptrdiff_t>(hitFrames),
                              first.begin() + static_cast<ptrdiff_t>(hitFrames * 2));
    Expect(Identical(first, second) && MeanAbsDiff(hitOne, hitTwo, 0) > 1.0e-4,
           "drum_round_robin_varies_and_stays_deterministic");
  }

  {
    KitParams kit = keith::drums::DefaultKitParams();
    kit.levelDb = 6.f;
    for (int i = 0; i < keith::drums::kNumSlots; ++i)
    {
      kit.slot[i].levelDb = 6.f;
      kit.slot[i].pan = (i & 1) ? 1.f : -1.f;
      kit.slot[i].tuneSemitones = 12.f;
      kit.slot[i].decay = 1.f;
      kit.slot[i].filterHz = 20000.f;
      kit.slot[i].drive = 1.f;
    }
    Engine engine;
    engine.Reset(sr);
    engine.SetParams(kit);
    for (int i = 0; i < keith::drums::kNumSlots; ++i)
      engine.Trigger(i, 1.f);
    bool finite = true;
    double peak = 0.;
    for (int i = 0; i < sr * 4; ++i)
    {
      float left = 0.f, right = 0.f;
      engine.ProcessFrame(left, right);
      finite = finite && std::isfinite(left) && std::isfinite(right);
      peak = std::max(peak, std::max(std::fabs(static_cast<double>(left)),
                                    std::fabs(static_cast<double>(right))));
    }
    Expect(finite && peak <= 1.000001, "drum_all_slots_max_is_finite");
  }

  {
    bool preserved = true;
    for (int preset = 0; preset < 15; ++preset)
    {
      Params patch = SteadyPatch();
      patch.osc1Shape = static_cast<float>(preset % 5) * 0.2f;
      patch.osc2Level = static_cast<float>(preset % 4) * 0.15f;
      patch.filterCutoffHz = 1800.f + static_cast<float>(preset) * 650.f;
      patch.masterLevel = 0.35f + static_cast<float>(preset % 3) * 0.1f;
      OfflineEngine before;
      OfflineEngine after;
      before.Reset(sr);
      after.Reset(sr);
      before.SetParams(patch);
      after.SetParams(patch);
      before.NoteOn(60 + preset % 12, 0.8f);
      after.NoteOn(60 + preset % 12, 0.8f);
      Engine drums;
      drums.Reset(sr);
      drums.SetParams(keith::drums::DefaultKitParams());
      for (int i = 0; i < 4096; ++i)
      {
        float drumLeft = 0.f, drumRight = 0.f;
        drums.ProcessFrame(drumLeft, drumRight);
        const float expected = before.Process();
        const float actual = after.Process() + drumLeft + drumRight;
        preserved = preserved && expected == actual && drumLeft == 0.f && drumRight == 0.f;
      }
    }
    Expect(preserved, "drum_new_defaults_preserve_presets");
  }

  std::printf("\nsynth_offline_test: %d passed, %d failed (total %d)\n", gPass, gFail, gPass + gFail);
  return gFail == 0 ? 0 : 1;
}
