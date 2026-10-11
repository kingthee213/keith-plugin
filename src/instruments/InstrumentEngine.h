/**
 * A multisampled instrument played across the keyboard.
 *
 * ## Why this is not the SamplerEngine
 *
 * `SamplerEngine` holds ONE sample and plays it across the keyboard by repitching. A multisampled
 * instrument is the opposite: one FILE PER NOTE, each played at its own pitch, with repitching
 * only used to fill the gaps between the sampled notes. Repitching a single guitar note up and down
 * a keyboard is what makes a sampled instrument sound like a synth again -- the formants move with
 * the pitch and a chord stops sounding like six strings.
 *
 * ## Why this is not the drum engine either
 *
 * `DrumKitEngine` maps a MIDI note to a SLOT, and every slot plays at its recorded pitch. Here a
 * note selects the nearest recorded sample and is then transposed to the requested pitch, by
 * however many semitones away that sample sits.
 *
 * ## Zones
 *
 * A zone is one recorded note: a file, the MIDI note it was recorded at, and the pitch range it
 * covers. Ranges are half-open between neighbours -- `[lo, hi)` -- so exactly one zone owns any
 * note and there are no gaps and no double-triggers. The last zone's range is open-ended upward and
 * the first opens downward, so every note on the keyboard resolves to something.
 *
 * ## Ownership
 *
 * Same discipline as the other engines: loads happen on the message thread, the audio thread only
 * reads, and a new bank is published behind a `shared_ptr` so the outgoing one cannot be freed
 * while a voice is still reading it.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "sampler/SamplerVoice.h"   // SampleData
#include "synth/VoiceAllocator.h"   // kMaxVoices
#include <cstdlib>

namespace keith {
namespace instruments {

/** One recorded note. */
struct Zone
{
  std::shared_ptr<const sampler::SampleData> sample;
  int rootNote = 60;   ///< the MIDI note this file was recorded at
  int loNote = 0;      ///< lowest MIDI note this zone answers for, inclusive
  int hiNote = 127;    ///< highest, inclusive

  bool Loaded() const { return sample != nullptr && !sample->Empty(); }
  int Frames() const { return sample ? sample->frames : 0; }
  double Rate() const { return sample ? sample->sampleRate : 48000.; }
};

/** An immutable loaded bank. Publishing a new one is how a change reaches the audio thread. */
struct BankData
{
  std::string name;
  std::vector<Zone> zones;   ///< sorted by rootNote

  int LoadedZones() const
  {
    int n = 0;
    for (const Zone& z : zones)
      if (z.Loaded())
        ++n;
    return n;
  }

  int LowNote() const { return zones.empty() ? 0 : zones.front().loNote; }
  int HighNote() const { return zones.empty() ? 0 : zones.back().hiNote; }

  /** The zone that answers for a note, or nullptr. Linear scan: a bank has tens of zones, not
   *  thousands, and this is called on note-on rather than per sample. */
  const Zone* ZoneFor(int note) const
  {
    for (const Zone& z : zones)
      if (z.Loaded() && note >= z.loNote && note <= z.hiNote)
        return &z;
    return nullptr;
  }
};

/**
 * How many notes a bank is assumed to cover when a zone has no explicit range.
 *
 * 3 semitones either side is the usual spacing for a multisample with a note every minor third,
 * and it keeps the transposition within a range where formants have not visibly moved.
 */
constexpr int kDefaultZoneSpan = 3;

/**
 * Build the zone ranges from a sorted list of root notes.
 *
 * Split at the midpoint between neighbours. A note exactly between two roots goes to the lower one,
 * which is arbitrary but fixed -- what matters is that the boundary is somewhere and is the same on
 * every load.
 */
inline void AssignRanges(std::vector<Zone>& zones)
{
  if (zones.empty())
    return;
  for (std::size_t i = 0; i < zones.size(); ++i)
  {
    if (i == 0)
      zones[i].loNote = 0;
    else
      zones[i].loNote = (zones[i - 1].rootNote + zones[i].rootNote + 1) / 2;

    if (i + 1 == zones.size())
      zones[i].hiNote = 127;
    else
      zones[i].hiNote = (zones[i].rootNote + zones[i + 1].rootNote) / 2;
  }
}

/** A voice reading from a published bank. One per held note, with the usual voice pool. */
class InstrumentVoice
{
public:
  void SetSampleRate(double sr) { mSampleRate = sr > 1. ? sr : 48000.; }

  void Start(const std::shared_ptr<const BankData>& bank, int note, float velocity,
             float gain = 1.f, float pitchOffsetSemis = 0.f)
  {
    mBank = bank;
    mNote = note;
    mPos = 0.;
    mActive = false;
    mReleasing = false;
    mGain = 0.f;

    if (!mBank)
      return;
    const Zone* zone = mBank->ZoneFor(note);
    if (!zone || !zone->Loaded())
      return;

    mZone = zone;
    // The whole point: the file is repitched by the INTERVAL between its recorded note and the
    // requested one, so a note that has its own recording plays at unity.
    const double semis = static_cast<double>(note - zone->rootNote) + pitchOffsetSemis;
    mStep = (zone->Rate() / mSampleRate) * std::pow(2.0, semis / 12.0);

    // A little gain reduction for notes pushed far from their root, because repitching a recording
    // changes its energy and a zone stretched 3 semitones up is audibly louder than the one beside
    // it. Not a correction -- just enough that the seam between zones is not a step.
    const double stretch = std::abs(semis);
    const float trim = static_cast<float>(1.0 / (1.0 + 0.06 * stretch * stretch));
    mGain = gain * trim * std::max(0.f, std::min(1.f, velocity));

    mActive = true;
  }

  void Release()
  {
    if (mActive)
      mReleasing = true;
  }

  void Reset()
  {
    mActive = false;
    mReleasing = false;
    mPos = 0.;
    mZone = nullptr;
    mBank.reset();
  }

  bool Active() const { return mActive; }
  int Note() const { return mNote; }

  /**
   * How far into the sample the read head sits, in source frames.
   *
   * Used for voice stealing. It is an approximation of age -- two voices read at different rates
   * advance different distances in the same wall time -- but it needs no extra state and is right
   * in the common case where a stolen voice is one that has been ringing a while.
   */
  double Pos() const { return mPos; }

  /** One sample out. Mono, because the bank is a mono recording and the layer is added to the
   *  synth bus the way every other source is. */
  float Process()
  {
    if (!mActive || !mZone)
      return 0.f;

    const int frames = mZone->Frames();
    if (frames <= 0)
    {
      mActive = false;
      return 0.f;
    }
    if (mPos >= static_cast<double>(frames))
    {
      mActive = false;
      return 0.f;
    }

    const int i0 = static_cast<int>(mPos);
    const int i1 = std::min(i0 + 1, frames - 1);
    const double frac = mPos - static_cast<double>(i0);
    const float a = mZone->sample->MonoAt(i0);
    const float b = mZone->sample->MonoAt(i1);
    float out = a + (b - a) * static_cast<float>(frac);

    out *= ReleaseEnvelope(frames) * mGain;
    mPos += mStep;
    return out;
  }

private:
  /** A release, so a note lifted off the keyboard does not stop with a step and click. 30 ms from
   *  wherever the sample is to zero. A guitar note left to ring would otherwise never stop under a
   *  held chord. */
  float ReleaseEnvelope(int frames) const
  {
    if (!mReleasing)
      return 1.f;
    const double left = static_cast<double>(frames) - mPos;
    const double releaseFrames = 0.030 * mSampleRate;
    if (left <= 0.)
      return 0.f;
    return static_cast<float>(std::max(0., std::min(1., left / releaseFrames)));
  }

  std::shared_ptr<const BankData> mBank;
  const Zone* mZone = nullptr;
  double mPos = 0.;
  double mStep = 1.;
  double mSampleRate = 48000.;
  float mGain = 0.f;
  int mNote = -1;
  bool mActive = false;
  bool mReleasing = false;
};

/**
 * The instrument bank: a published bank and a voice pool.
 *
 * A POOL rather than a voice per zone, unlike the drum engine. A guitar is played as chords, so the
 * same zone is needed several times at once -- three notes of a triad can all land in one zone --
 * and slot-per-voice would cut the first off when the second arrived.
 */
class InstrumentEngine
{
public:
  static constexpr int kVoices = 16;

  InstrumentEngine()
  {
    for (auto& v : mVoices)
      v.SetSampleRate(48000.);
  }

  void SetSampleRate(double sr)
  {
    mSampleRate = sr > 1. ? sr : 48000.;
    for (auto& v : mVoices)
      v.SetSampleRate(mSampleRate);
  }

  void Reset()
  {
    for (auto& v : mVoices)
      v.Reset();
  }

  void Publish(std::shared_ptr<const BankData> bank) { mBank = std::move(bank); }

  const std::shared_ptr<const BankData>& Bank() const { return mBank; }

  bool HandlesNotes() const { return mBank && mBank->LoadedZones() > 0; }

  /** Audio thread. Takes a free voice, or steals the oldest if all are busy. */
  bool Trigger(int note, float velocity, float gain)
  {
    if (!HandlesNotes())
      return false;

    int slot = -1;
    for (int i = 0; i < kVoices; ++i)
      if (!mVoices[i].Active())
      {
        slot = i;
        break;
      }
    if (slot < 0)
    {
      // All busy: take the one that has been sounding longest. Stealing the first found is
      // deterministic and, for a 16-voice pool under a chord, indistinguishable from a smarter
      // policy -- and a smarter one would need allocation to track age.
      slot = 0;
      for (int i = 1; i < kVoices; ++i)
        if (mVoices[i].Pos() > mVoices[slot].Pos())
          slot = i;
    }

    mVoices[slot].Start(mBank, note, velocity, gain);
    return true;
  }

  void Release(int note)
  {
    for (auto& v : mVoices)
      if (v.Active() && v.Note() == note)
        v.Release();
  }

  void ReleaseAll()
  {
    for (auto& v : mVoices)
      v.Release();
  }

  float Process()
  {
    float acc = 0.f;
    for (auto& v : mVoices)
      if (v.Active())
        acc += v.Process();
    return acc;
  }

private:
  std::shared_ptr<const BankData> mBank;
  InstrumentVoice mVoices[kVoices];
  double mSampleRate = 48000.;
};

} // namespace instruments
} // namespace keith
