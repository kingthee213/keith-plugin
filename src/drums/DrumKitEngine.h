/**
 * A drum kit played from samples.
 *
 * Replaces the synthesised DrumSynth. The old engine generated every hit from oscillators and
 * noise, which is why the kits did not sound like records: a drum break's character is the
 * recorded hit, the converter it passed through and the way it was chopped, and none of that can
 * be synthesised into existence.
 *
 * ## Shape
 *
 * One kit is 16 slots holding one sample each, which is the opposite of SamplerEngine -- that
 * holds ONE sample across 16 voices. So this is its own engine rather than a reuse.
 *
 * ## Ownership
 *
 * Loads are on the message thread. The audio thread only reads. A load publishes a new
 * shared_ptr<const KitData> and never frees the old one while a voice may still be reading it --
 * the same discipline SamplerEngine uses for slices, and for the same reason: a raw swap races
 * with the audio thread.
 *
 * ## Slot shape
 *
 * A slot is either ONESHOT or SUSTAINED. Percussion and drums are one-shots: the whole sample
 * plays and a note-off does nothing, because choking a conga mid-ring is not what a drummer does.
 * A sustained slot (a stab, a riser) follows the note length like a synth voice. The kit declares
 * which, so the behaviour is data rather than a special case per instrument.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "sampler/SamplerVoice.h"   // SampleData lives here
#include "sampler/SampleLoader.h"

namespace keith {
namespace drums {

/** Slots, matching the MIDI drum range the editor draws. */
constexpr int kKitSlots = 16;

// kFirstMidiNote and kLastMidiNote come from drums/DrumSynth.h, which owns the drum note range.
// Redefining them here collided -- and a second definition of a range is the kind of thing that
// drifts out of step silently, so there is one definition and this file uses it.
constexpr int kFirstMidiNote = 36;

/**
 * Where the percussion kit starts, in MIDI notes ABOVE the whole drum range.
 *
 * This matters and was got wrong once. The drum range is notes 36..51 (slots 0..15). Percussion
 * originally started at slot 9 -- note 45 -- which is INSIDE that range, and because the
 * percussion branch is checked first, notes 45..51 would have played percussion instead of drum
 * pads 9..15. Half the pad grid would have gone silent the moment a percussion kit was loaded.
 *
 * Starting above the range removes the overlap rather than relying on branch order to arbitrate it.
 */
constexpr int kPercussionFirstSlot = 16;   // note 52

enum class SlotMode
{
  OneShot,    ///< plays to the end regardless of note length
  Sustained,  ///< follows note on/off, with a release
};

/** One slot's decoded audio plus how to play it. */
struct Slot
{
  std::shared_ptr<const sampler::SampleData> sample;
  SlotMode mode = SlotMode::OneShot;
  float gain = 1.f;          ///< linear, applied on top of the sample's own level
  float pitchSemis = 0.f;    ///< per-slot tune, for kits that need a slot moved

  bool Loaded() const { return sample != nullptr && !sample->Empty(); }
  int Frames() const { return sample ? sample->frames : 0; }
  int Channels() const { return sample ? sample->ChannelCount() : 1; }
  double Rate() const { return sample ? sample->sampleRate : 48000.; }
};

/** An immutable loaded kit. Publishing a new one is how a kit change reaches the audio thread. */
struct KitData
{
  std::string name;
  Slot slots[kKitSlots];

  /** How many slots hold audio. A kit with holes is allowed; the holes are silent. */
  int LoadedSlots() const
  {
    int n = 0;
    for (const Slot& s : slots)
      if (s.Loaded())
        ++n;
    return n;
  }
};

/** A voice reading from a published kit. 16 of these, one per slot, so a slot can still retrigger
 *  itself without cutting off its own previous hit -- which is what makes fast hi-hats work. */
class KitVoice
{
public:
  void SetSampleRate(double sr) { mSampleRate = sr > 1. ? sr : 48000.; }

  void Start(const std::shared_ptr<const KitData>& kit, int slot, float velocity,
             float slotGain = 1.f, float slotRate = 1.f)
  {
    mKit = kit;
    mSlot = slot;
    mPos = 0.;
    mActive = false;
    mGain = 0.f;

    if (!mKit || slot < 0 || slot >= kKitSlots)
      return;
    const Slot& s = mKit->slots[slot];
    if (!s.Loaded())
      return;

    // The sample's own rate against the host's. A 48k sample in a 44.1k host must play slower or
    // it comes out sharp, and drum hits are short enough that the error is obvious.
    mStep = (s.Rate() / mSampleRate) * slotRate * std::pow(2.0, s.pitchSemis / 12.0);
    mGain = s.gain * slotGain * std::max(0.f, std::min(1.f, velocity));
    mActive = true;
    mReleasing = false;
  }

  /** Note-off. Only Sustained slots respond; a one-shot rings out, as a drum does. */
  void Release()
  {
    if (!mActive)
      return;
    if (mKit && mSlot >= 0 && mKit->slots[mSlot].mode == SlotMode::Sustained)
      mReleasing = true;
  }

  void Reset()
  {
    mActive = false;
    mReleasing = false;
    mPos = 0.;
    mKit.reset();
  }

  bool Active() const { return mActive; }

  /** One sample out. Returns 0 when idle, so the caller can skip the slot entirely. */
  float Process()
  {
    if (!mActive || !mKit)
      return 0.f;

    const Slot& s = mKit->slots[mSlot];
    const int frames = s.Frames();
    if (frames <= 0)
    {
      mActive = false;
      return 0.f;
    }

    // Past the end: done. This is the normal end for a one-shot.
    if (mPos >= static_cast<double>(frames))
    {
      mActive = false;
      return 0.f;
    }

    const int i0 = static_cast<int>(mPos);
    const int i1 = std::min(i0 + 1, frames - 1);
    const double frac = mPos - static_cast<double>(i0);

    // Linear interpolation between the two neighbouring frames. Crude next to a proper resampler,
    // but a drum hit is short and the alternative costs a filter per voice.
    float a = SampleAt(s, i0);
    float b = SampleAt(s, i1);
    float out = a + (b - a) * static_cast<float>(frac);

    const float env = Envelope(s, frames);
    out *= env * mGain;

    mPos += mStep;
    return out;
  }

private:
  /** Mono-summed frame, because the kit is played through the mono drum bus. SampleData is
   *  channels[ch][frame]; MonoAt already does exactly this and is used rather than reimplemented. */
  static float SampleAt(const Slot& s, int frame)
  {
    if (!s.sample || frame < 0 || frame >= s.Frames())
      return 0.f;
    return s.sample->MonoAt(frame);
  }

  float Envelope(const Slot& s, int frames) const
  {
    if (s.mode != SlotMode::Sustained)
      return 1.f;   // a one-shot's shape is in the recording

    // A sustained slot needs a release, or the note stops with a step discontinuity and clicks.
    // 8 ms from the current level to zero.
    if (!mReleasing)
      return 1.f;
    const double remaining = static_cast<double>(frames) - mPos;
    const double releaseFrames = 0.008 * mSampleRate;
    if (remaining <= 0.)
      return 0.f;
    return static_cast<float>(std::max(0., std::min(1., remaining / releaseFrames)));
  }

  std::shared_ptr<const KitData> mKit;
  int mSlot = -1;
  double mPos = 0.;
  double mStep = 1.;
  double mSampleRate = 48000.;
  float mGain = 0.f;
  bool mActive = false;
  bool mReleasing = false;
};

/**
 * The kit bank: holds the published kit and drives the voices.
 *
 * Slot-per-voice rather than a shared pool. A pool would be cheaper for long tails, but 16 short
 * drum samples are trivial next to the synth, and slot-per-voice means a hi-hat roll never steals
 * the kick's voice -- the failure mode a pool gets wrong most audibly.
 */
class DrumKitEngine
{
public:
  DrumKitEngine()
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

  /** Message thread. Publishes a new kit; the audio thread picks it up on its next trigger. */
  void Publish(std::shared_ptr<const KitData> kit) { mKit = std::move(kit); }

  /**
   * Per-slot gain and tune, set by the host's slot parameters.
   *
   * Held here rather than written into the published KitData, because KitData is shared with the
   * audio thread and mutating it under a reader is exactly the race Publish exists to avoid. These
   * two arrays are written on the message thread and read on the audio thread as plain floats, so
   * a torn read at worst gives one hit the previous gain -- inaudible, and cheaper than a lock on
   * a per-sample path.
   */
  void SetSlotParams(int slot, float gain, float tuneSemis)
  {
    if (slot < 0 || slot >= kKitSlots)
      return;
    mSlotGain[slot] = gain;
    mSlotRate[slot] = std::pow(2.0, tuneSemis / 12.0);
  }

  float SlotGain(int slot) const
  {
    return (slot >= 0 && slot < kKitSlots) ? mSlotGain[slot] : 0.f;
  }

  float SlotRate(int slot) const
  {
    return (slot >= 0 && slot < kKitSlots) ? mSlotRate[slot] : 1.f;
  }

  const std::shared_ptr<const KitData>& Kit() const { return mKit; }

  /** True when a note belongs to the drum range AND that slot holds audio. */
  bool HandlesNote(int note) const
  {
    return mKit && SlotFor(note) >= 0;
  }

  static int SlotFor(int note)
  {
    const int slot = note - kFirstMidiNote;
    return (slot >= 0 && slot < kKitSlots) ? slot : -1;
  }

  /** Audio thread. Starts the slot's voice. Returns false if the slot is empty. */
  bool Trigger(int note, float velocity)
  {
    const int slot = SlotFor(note);
    if (!mKit || slot < 0 || !mKit->slots[slot].Loaded())
      return false;
    if (mSlotGain[slot] <= 0.f)
      return false;   // muted or turned down: do not start a voice that would be silent
    mVoices[slot].Start(mKit, slot, velocity, mSlotGain[slot], mSlotRate[slot]);
    return true;
  }

  void Release(int note)
  {
    const int slot = SlotFor(note);
    if (slot >= 0)
      mVoices[slot].Release();
  }

  /** Audio thread. Sums every active slot. Nothing is allocated here. */
  float Process()
  {
    float acc = 0.f;
    for (int i = 0; i < kKitSlots; ++i)
      if (mVoices[i].Active())
        acc += mVoices[i].Process();
    return acc;
  }

  /** Copies a slot's samples for the editor waveform. Message thread; walks frames, no deref. */
  int CopySlotAudio(int slot, std::vector<float>& out) const
  {
    out.clear();
    if (!mKit || slot < 0 || slot >= kKitSlots)
      return 0;
    const Slot& s = mKit->slots[slot];
    if (!s.Loaded())
      return 0;
    const int frames = s.Frames();
    out.reserve(static_cast<std::size_t>(frames));
    for (int i = 0; i < frames; ++i)
      out.push_back(SampleAt(s, i));
    return frames;
  }

private:
  static float SampleAt(const Slot& s, int frame)
  {
    if (!s.sample || frame < 0 || frame >= s.Frames())
      return 0.f;
    return s.sample->MonoAt(frame);
  }

  std::shared_ptr<const KitData> mKit;
  KitVoice mVoices[kKitSlots];
  double mSampleRate = 48000.;
  float mSlotGain[kKitSlots] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  float mSlotRate[kKitSlots] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
};

} // namespace drums
} // namespace keith
