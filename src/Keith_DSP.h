#pragma once

#include "sampler/SamplerEngine.h"

#include "MidiSynth.h"
#include "Smoothers.h"
#include "drums/DrumSynth.h"
#include "fx/PatchEffects.h"
#include "fx/TapeAnalog.h"
#include "synth/CoreSynth.h"

using namespace iplug;

enum EKeithModulations
{
  kModGain = 0,
  kNumKeithModulations
};

/** One glide-or-attack decision per allocator Trigger batch, in MIDI order. */
struct KeithGlideQueue
{
  bool item[64] {};
  int read = 0;
  int write = 0;
  int count = 0;

  void Clear() { read = write = count = 0; }

  void Push(bool glide)
  {
    if (count >= 64)
      return;
    item[write] = glide;
    write = (write + 1) & 63;
    ++count;
  }

  bool Pop()
  {
    if (count <= 0)
      return false;
    const bool glide = item[read];
    read = (read + 1) & 63;
    --count;
    return glide;
  }
};

/** Held-key mask shared by every synth voice. The allocator triggers all 16 voices on one
 *  mono note, so a batch consumes a single queued decision. */
struct KeithPerformance
{
  static constexpr int kVoices = 16;

  keith::synth::HeldKeyTracker held;
  KeithGlideQueue glides;
  bool batchOpen = false;
  int batchCount = 0;
  bool batchGlide = false;
  bool sounding = false;
  int soundingKey = -1;

  void ClearNotes()
  {
    held.Clear();
    sounding = false;
    soundingKey = -1;
  }

  void Clear()
  {
    ClearNotes();
    glides.Clear();
    batchOpen = false;
    batchCount = 0;
    batchGlide = false;
  }
};

template<typename T>
class KeithDSP
{
public:
  class Voice : public SynthVoice
  {
  public:
    Voice() = default;

    void SetSharedParams(const keith::synth::Params* p)
    {
      mParams = p;
    }

    void SetRuntime(keith::synth::RuntimeMod* r)
    {
      mRuntime = r;
    }

    void SetPerformance(KeithPerformance* perf)
    {
      mPerf = perf;
    }

    bool GetBusy() const override
    {
      return mVoice.Busy();
    }

    void Trigger(double level, bool /*isRetrigger*/) override
    {
      if (!mParams || !mRuntime || !mPerf)
        return;
      // MidiSynth pitch is 1v/oct relative to A440 (1.0 = 440 Hz).
      const double pitchOct = mInputs[kVoiceControlPitch].endValue;
      const float midiNote = static_cast<float>(69.0 + 12.0 * pitchOct);

      // Mono note-on starts every voice. Poly starts one. One queued decision covers the batch.
      // isRetrigger is never set by VoiceAllocator, so the decision comes from the held-key mask.
      const int batchSize = (mParams->polyMode == keith::synth::PolyMode::Mono)
                              ? KeithPerformance::kVoices : 1;
      if (!mPerf->batchOpen || mPerf->batchCount >= batchSize)
      {
        mPerf->batchGlide = mPerf->glides.Pop();
        mPerf->batchOpen = true;
        mPerf->batchCount = 0;
      }
      ++mPerf->batchCount;

      if (mPerf->batchGlide && mVoice.Busy())
        mVoice.GlideTo(midiNote);
      else
        mVoice.NoteOn(midiNote, static_cast<float>(level), *mParams);
    }

    /** CC1 (mod wheel) arrives here from VoiceAllocator; every other controller is ignored. */
    void SetControl(int controlNumber, float value) override
    {
      if (controlNumber == 1 && mRuntime)
        mRuntime->modWheel = std::clamp(value / 127.f, 0.f, 1.f);
    }

    void Release() override
    {
      mVoice.NoteOff();
    }

    void ProcessSamplesAccumulating(T** /*inputs*/, T** outputs, int /*nInputs*/, int nOutputs,
                                     int startIdx, int nFrames) override
    {
      if (!mParams || !mRuntime)
        return;

      const double pitchOct = mInputs[kVoiceControlPitch].endValue;
      const double bendOct = mInputs[kVoiceControlPitchBend].endValue;
      const float glideNote = static_cast<float>(69.0 + 12.0 * (pitchOct + bendOct));
      mVoice.SetBaseNote(glideNote);
      // NoteOn runs before this pitch is current, so key-scaled times are applied here,
      // once the voice actually knows which note it is sounding.
      mVoice.ApplyEnvTimes(*mParams);

      for (int i = startIdx; i < startIdx + nFrames; ++i)
      {
        float left = 0.f;
        float right = 0.f;
        mVoice.ProcessFrame(*mParams, *mRuntime, left, right);
        outputs[0][i] += static_cast<T>(left);
        if (nOutputs > 1)
          outputs[1][i] += static_cast<T>(right);
      }
    }

    void SetSampleRateAndBlockSize(double sampleRate, int /*blockSize*/) override
    {
      mVoice.SetSampleRate(sampleRate);
      mVoice.Reset();
    }

    void ApplyEnvTimes(const keith::synth::Params& p)
    {
      mVoice.ApplyEnvTimes(p);
    }

  private:
    keith::synth::Voice mVoice;
    const keith::synth::Params* mParams = nullptr;
    keith::synth::RuntimeMod* mRuntime = nullptr;
    KeithPerformance* mPerf = nullptr;
  };

  KeithDSP()
  {
    for (int i = 0; i < 16; ++i)
    {
      auto* v = new Voice();
      v->SetSharedParams(&mSynthParams);
      v->SetRuntime(&mRuntime);
      v->SetPerformance(&mPerf);
      mSynth.AddVoice(v, 0);
    }
  }

  void Reset(double sampleRate, int blockSize)
  {
    mSampleRate = sampleRate;
    mBlockSize = blockSize;
    mRuntime.modWheel = 0.f;
    mPerf.Clear();
    mDrums.Reset(sampleRate);
    mDrums.SetParams(mDrumParams);
    mNumDrumEvents = 0;
    mSynth.SetSampleRateAndBlockSize(sampleRate, blockSize);
    mSynth.Reset();
    mModulationsData.Resize(blockSize * kNumKeithModulations);
    mModulations.Empty();

    for (int i = 0; i < kNumKeithModulations; ++i)
      mModulations.Add(mModulationsData.Get() + (blockSize * i));

    mParamSmoother.SetSmoothTime(10., sampleRate);
    mParamSmoother.SetValue(static_cast<T>(mParamsToSmooth[kModGain]));

    mGlobalTape.Reset(sampleRate, blockSize);
    for (auto& tape : mPartTape)
      tape.Reset(sampleRate, blockSize);
    mInsert1.Reset(sampleRate, blockSize);
    mInsert2.Reset(sampleRate, blockSize);
    mMasterFx.Reset(sampleRate, blockSize);
    mOutLevel.Reset(sampleRate, static_cast<T>(1));

    mSynth.ForEachVoice([this](SynthVoice& voice) {
      static_cast<Voice&>(voice).SetSharedParams(&mSynthParams);
      static_cast<Voice&>(voice).SetRuntime(&mRuntime);
      static_cast<Voice&>(voice).SetPerformance(&mPerf);
    });
  }

  using TapeParams = typename keith::fx::TapeAnalog<T>::Params;
  using InsertParams = typename keith::fx::InsertEffect<T>::Params;
  using MasterParams = typename keith::fx::MasterEffect<T>::Params;

  void SetGlobalTapeParams(const TapeParams& p)
  {
    mGlobalTape.SetParams(p);
  }

  void SetPartTapeParams(int partIndex, const TapeParams& p)
  {
    if (partIndex >= 0 && partIndex < kNumParts)
      mPartTape[partIndex].SetParams(p);
  }

  void SetInsert1Params(const InsertParams& p) { mInsert1.SetParams(p); }
  void SetInsert2Params(const InsertParams& p) { mInsert2.SetParams(p); }
  void SetMasterFxParams(const MasterParams& p) { mMasterFx.SetParams(p); }
  void SetOutLevel(double linear) { mOutLevel.SetLinear(static_cast<T>(linear)); }
  /** PAD MIDI. Pushed in by Keith before each block so the audio thread does not have to
   *  reach back into the plugin. See Keith::PadMidiEnabled(). */
  void SetPadMidi(bool on) { mPadMidi = on; }

  /**
   * Point the DSP at the plugin's sampler.
   *
   * A pointer rather than a copy: the engine owns the loaded sample and the voices, and the
   * editor loads files into it. The DSP only reads. Set once at construction.
   */
  void SetSamplerEngine(keith::sampler::SamplerEngine* engine) { mSampler = engine; }

  /** Fire a pad now, from the editor. Played at the very start of the next block, so a
   *  clicked pad sounds immediately rather than at whatever offset the host happens to be
   *  at. Velocity is 0..1. */
  void TriggerPad(int slot, float velocity)
  {
    QueueDrumEvent(0, slot, velocity);
  }

  /**
   * How many times a pad has been hit since the plugin started.
   *
   * The editor polls this to light a pad for a moment after it sounds. Counting hits rather
   * than exposing a "busy" flag means the editor can see EVERY hit -- a pad struck twice in
   * quick succession flashes twice -- and it needs no lock or timer on the audio thread.
   */
  uint64_t PadHitCount(int slot) const
  {
    return (slot >= 0 && slot < keith::drums::kNumSlots) ? mPadHits[slot].load() : 0;
  }

  void SetDrumParams(const keith::drums::KitParams& p)
  {
    mDrumParams = p;
    mDrums.SetParams(p);
  }

  void SetSynthParams(const keith::synth::Params& p)
  {
    mSynthParams = p;

    const auto mode = (p.polyMode == keith::synth::PolyMode::Mono)
                        ? VoiceAllocator::kPolyModeMono
                        : VoiceAllocator::kPolyModePoly;
    mSynth.SetPolyMode(mode);
    mSynth.SetNoteGlideTime(static_cast<double>(p.portaMs) * 0.001);

    mSynth.ForEachVoice([&p](SynthVoice& voice) {
      static_cast<Voice&>(voice).ApplyEnvTimes(p);
    });
  }

  void ProcessBlock(T** outputs, int nOutputs, int nFrames)
  {
    for (int ch = 0; ch < nOutputs; ++ch)
      memset(outputs[ch], 0, nFrames * sizeof(T));

    mParamSmoother.ProcessBlock(mParamsToSmooth, mModulations.GetList(), nFrames);
    mSynth.ProcessBlock(mModulations.GetList(), outputs, 0, nOutputs, nFrames);

    int drumEvent = 0;
    for (int s = 0; s < nFrames; ++s)
    {
      while (drumEvent < mNumDrumEvents && mDrumEvents[drumEvent].offset <= s)
      {
        const int slot = mDrumEvents[drumEvent].slot;
        mDrums.Trigger(slot, mDrumEvents[drumEvent].velocity);
        // Record the hit so the editor can flash the pad. The pad lights for every trigger,
        // whether it came from MIDI or from a click in the editor.
        RecordPadHit(slot);
        ++drumEvent;
      }

      float drumLeft = 0.f;
      float drumRight = 0.f;
      mDrums.ProcessFrame(drumLeft, drumRight);
      outputs[0][s] += static_cast<T>(drumLeft);
      if (nOutputs > 1)
        outputs[1][s] += static_cast<T>(drumRight);

      // The sampler sums in last, alongside the drums and after the synth. It is a plain
      // additive layer at stage 1 -- how it routes relative to the synth, and whether the
      // drum pads can point at a sample instead of the synthesized voice, are later decisions.
      // Guarded on the pointer: the DSP is constructed before the link is made.
      if (mSampler != nullptr)
      {
        const float samplerOut = mSampler->Process();
        outputs[0][s] += static_cast<T>(samplerOut);
        if (nOutputs > 1)
          outputs[1][s] += static_cast<T>(samplerOut);
      }
    }
    RetainFutureDrumEvents(nFrames, drumEvent);

    // Synth voices plus the independent drum bus -> insert 1 -> insert 2 -> tape -> master FX ->
    // output level -> the existing gain parameter.
    mInsert1.ProcessBlock(outputs, nOutputs, nFrames);
    mInsert2.ProcessBlock(outputs, nOutputs, nFrames);
    mPartTape[0].ProcessBlock(outputs, nOutputs, nFrames);
    mGlobalTape.ProcessBlock(outputs, nOutputs, nFrames);
    mMasterFx.ProcessBlock(outputs, nOutputs, nFrames);
    mOutLevel.ProcessBlock(outputs, nOutputs, nFrames);

    for (int s = 0; s < nFrames; ++s)
    {
      const T gain = mModulations.GetList()[kModGain][s];
      outputs[0][s] *= gain;
      if (nOutputs > 1)
        outputs[1][s] *= gain;
    }

    mPerf.batchOpen = false;
    mPerf.batchCount = 0;
  }

  void ProcessMidiMsg(const IMidiMsg& msg)
  {
    const IMidiMsg::EStatusMsg status = msg.StatusMsg();
    const bool noteMessage = status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff;

    // PAD MIDI ON: the pads are the only thing that plays. Notes in the drum range trigger
    // them; every other note is swallowed rather than falling through to the synth. That is
    // the point of the switch -- with it on, only what lines up with a pad makes a sound.
    if (mPadMidi)
    {
      if (noteMessage && keith::drums::IsDrumNote(msg.NoteNumber()) && mDrums.Audible())
      {
        if (status == IMidiMsg::kNoteOn && msg.Velocity() > 0)
          QueueDrumEvent(msg.mOffset, keith::drums::SlotForMidiNote(msg.NoteNumber()),
                         static_cast<float>(msg.Velocity()) / 127.f);
      }
      if (noteMessage)
        return;
    }
    else if (noteMessage && keith::drums::IsDrumNote(msg.NoteNumber()) && mDrums.Audible())
    {
      // PAD MIDI OFF, audible kit: the drum range still plays the pads, as it always has.
      // Existing presets rely on this -- an audible kit owns 36-51.
      if (status == IMidiMsg::kNoteOn && msg.Velocity() > 0)
        QueueDrumEvent(msg.mOffset, keith::drums::SlotForMidiNote(msg.NoteNumber()),
                       static_cast<float>(msg.Velocity()) / 127.f);
      return;
    }

    if (status == IMidiMsg::kControlChange)
    {
      const int cc = msg.ControlChangeIdx();
      if (cc == IMidiMsg::kAllNotesOff || cc == 120)
      {
        mDrums.AllNotesOff();
        mNumDrumEvents = 0;
      }
    }

    TrackPerformanceMidi(msg);
    mSynth.AddMidiMsgToQueue(msg);
  }

  void SetGain(double gainPercent)
  {
    mParamsToSmooth[kModGain] = static_cast<T>(gainPercent / 100.);
  }

private:
  struct DrumEvent
  {
    int offset = 0;
    int slot = 0;
    float velocity = 0.f;
  };

  void QueueDrumEvent(int offset, int slot, float velocity)
  {
    if (mNumDrumEvents >= kMaxDrumEvents)
      return;

    DrumEvent event {std::max(0, offset), slot, velocity};
    int insertion = mNumDrumEvents;
    while (insertion > 0 && mDrumEvents[insertion - 1].offset > event.offset)
    {
      mDrumEvents[insertion] = mDrumEvents[insertion - 1];
      --insertion;
    }
    mDrumEvents[insertion] = event;
    ++mNumDrumEvents;
  }

  /** Note that a pad sounded, so the editor can flash it. Called from the audio thread. */
  void RecordPadHit(int slot)
  {
    if (slot >= 0 && slot < keith::drums::kNumSlots)
      mPadHits[slot].fetch_add(1, std::memory_order_relaxed);
  }

  void RetainFutureDrumEvents(int nFrames, int consumed)
  {
    int write = 0;
    for (int read = consumed; read < mNumDrumEvents; ++read)
    {
      DrumEvent event = mDrumEvents[read];
      event.offset = std::max(0, event.offset - nFrames);
      mDrumEvents[write++] = event;
    }
    mNumDrumEvents = write;
  }

  void TrackPerformanceMidi(const IMidiMsg& msg)
  {
    using keith::synth::LegatoAttack;
    using keith::synth::PolyMode;

    const IMidiMsg::EStatusMsg status = msg.StatusMsg();
    if (status == IMidiMsg::kControlChange)
    {
      const int cc = msg.ControlChangeIdx();
      if (cc == IMidiMsg::kModWheel)
      {
        // VoiceAllocator addresses a CC by its controller number, so SetControl often misses the
        // sounding note. The message itself is the source; store CC1 here as well.
        const float raw = static_cast<float>(msg.ControlChange(IMidiMsg::kModWheel));
        mRuntime.modWheel = std::clamp(raw / 127.f, 0.f, 1.f);
      }
      else if (cc == IMidiMsg::kAllNotesOff || cc == 120)
      {
        mPerf.ClearNotes();
      }
      return;
    }

    const bool noteOn = status == IMidiMsg::kNoteOn && msg.Velocity() > 0;
    const bool noteOff = status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && msg.Velocity() == 0);
    if (!noteOn && !noteOff)
      return;

    const int note = msg.NoteNumber();
    if (note < 0 || note > 127)
      return;

    if (noteOn)
    {
      const int others = mPerf.held.Others(note);
      mPerf.glides.Push(LegatoAttack(mSynthParams, others, mPerf.sounding));
      mPerf.held.Add(note, static_cast<float>(msg.Velocity()) / 127.f);
      mPerf.sounding = true;
      mPerf.soundingKey = note;
      return;
    }

    if (!mPerf.held.down[note])
      return;

    const bool wasSounding = mPerf.soundingKey == note;
    mPerf.held.Remove(note);
    if (mSynthParams.polyMode == PolyMode::Mono)
    {
      if (mPerf.held.count > 0 && wasSounding)
      {
        const int queued = mPerf.held.Newest();
        mPerf.glides.Push(mSynthParams.monoLegato && mPerf.sounding);
        mPerf.soundingKey = queued;
      }
      else if (mPerf.held.count == 0)
      {
        mPerf.sounding = false;
        mPerf.soundingKey = -1;
      }
    }
    else if (mPerf.held.count == 0)
    {
      mPerf.sounding = false;
      mPerf.soundingKey = -1;
    }
  }

private:
  static constexpr int kNumParts = 4;
  static constexpr int kMaxDrumEvents = 128;

  double mSampleRate = 44100.;
  int mBlockSize = 64;
  MidiSynth mSynth {VoiceAllocator::kPolyModePoly, MidiSynth::kDefaultBlockSize};
  WDL_TypedBuf<T> mModulationsData;
  WDL_PtrList<T> mModulations;
  LogParamSmooth<T, kNumKeithModulations> mParamSmoother;
  sample mParamsToSmooth[kNumKeithModulations] {};

  keith::synth::Params mSynthParams {};
  keith::synth::RuntimeMod mRuntime {};
  KeithPerformance mPerf {};
  keith::drums::KitParams mDrumParams = keith::drums::DefaultKitParams();
  /** Set from Keith::ProcessBlock; see SetPadMidi. */
  bool mPadMidi = true;
  keith::drums::Engine mDrums;
  DrumEvent mDrumEvents[kMaxDrumEvents] {};
  /** The plugin's sampler. Null until SetSamplerEngine is called; never dereferenced when null. */
  keith::sampler::SamplerEngine* mSampler = nullptr;
  int mNumDrumEvents = 0;
  /** Hits per pad since load. The editor diffs this to flash a pad; see PadHitCount(). */
  std::atomic<uint64_t> mPadHits[keith::drums::kNumSlots] {};
  keith::fx::InsertEffect<T> mInsert1;
  keith::fx::InsertEffect<T> mInsert2;
  keith::fx::TapeAnalog<T> mGlobalTape;
  keith::fx::TapeAnalog<T> mPartTape[kNumParts] {};
  keith::fx::MasterEffect<T> mMasterFx;
  keith::fx::SmoothedGain<T> mOutLevel;
};
