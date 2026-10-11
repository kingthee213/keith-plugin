#pragma once

#include <algorithm>
#include <vector>

#include "sampler/SamplerEngine.h"

#include "MidiSynth.h"
#include "Smoothers.h"
#include "drums/DrumSynth.h"        // KitParams: the per-slot level, pan, tune, mute and solo
#include "drums/DrumKitEngine.h"    // the sampled kit engine that replaced the synthesis
#include "drums/DrumKitLoader.h"    // LoadKit: reads a kit directory into a KitData
#include "instruments/InstrumentEngine.h"   // the multisampled guitar layer
#include "instruments/InstrumentLoader.h"
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

    // Size the warped-sampler scratch here, once, from the host's maximum block size. Doing it in
    // the render path would allocate on the audio thread, which is not allowed -- and a host may
    // hand a larger block than any previous one, so the buffers must be grown somewhere that is
    // permitted. Reset is that place.
    const std::size_t scratch = static_cast<std::size_t>(std::max(1, blockSize));
    mWarpScratch.assign(scratch, 0.f);
    mWarpScratchR.assign(scratch, 0.f);
    mWarpBlockActive = false;

    mRuntime.modWheel = 0.f;
    mPerf.Clear();
    mDrums.SetSampleRate(sampleRate);
    mDrums.Reset();
    mPercussion.SetSampleRate(sampleRate);
    mPercussion.Reset();
    mInstrument.SetSampleRate(sampleRate);
    mInstrument.Reset();
    mVocal.SetSampleRate(sampleRate);
    mVocal.Reset();
    ApplyDrumSlotParams();
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
   * Render the sampler through the stretcher for one block, if the warp path is active.
   *
   * Sets mWarpBlockActive so the sample loop knows to add the scratch buffer rather than call
   * the plain sampler path. When warp is off or unavailable the buffers are left alone and the
   * loop falls through to plain playback.
   */
  void RenderWarpBlock(int nFrames)
  {
    mWarpBlockActive = false;
    if (mSampler == nullptr || nFrames <= 0)
      return;

    keith::sampler::WarpEngine& warp = mSampler->Warp();
    if (!warp.WarpAvailable() || !warp.Active())
      return;

    // No resize here: the buffers are sized in Reset() from the host's maximum block size, so
    // this path never allocates. If a block somehow arrives larger than that, the warp path
    // declines rather than growing a buffer on the audio thread.
    if (static_cast<int>(mWarpScratch.size()) < nFrames)
      return;

    std::fill_n(mWarpScratch.begin(), nFrames, 0.f);
    std::fill_n(mWarpScratchR.begin(), nFrames, 0.f);

    float* channels[2] = {mWarpScratch.data(), mWarpScratchR.data()};
    warp.ProcessWarped(channels, nFrames);
    mWarpBlockActive = true;
  }

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

  /**
   * Per-slot level and tune, applied against whatever kit is loaded.
   *
   * The old engine built its hit from these numbers, so they fed the synthesis. They now scale the
   * sampled kit instead: level is gain, tune is playback rate. Mute and solo are read by the
   * editor and by the trigger path, not here.
   */
  void SetDrumParams(const keith::drums::KitParams& p)
  {
    mDrumParams = p;
    mDrumGainsDirty = true;
  }

  /** Push level and tune for every slot onto the loaded kit. Called when either changes. */
  void ApplyDrumSlotParams()
  {
    for (int slot = 0; slot < keith::drums::kKitSlots; ++slot)
    {
      const keith::drums::SlotParams& sp = mDrumParams.slot[slot];
      // A muted or silent slot is turned down to zero rather than skipped, so the trigger path
      // stays one shape and a mute takes effect on the next hit.
      const bool audible = SlotAudible(static_cast<int>(slot));
      const float level = audible ? keith::drums::DbToLinear(sp.levelDb) : 0.f;
      mDrums.SetSlotParams(static_cast<int>(slot), level, sp.tuneSemitones);
    }
    mDrumGainsDirty = false;
  }

  /**
   * Point the drum engine at a loaded kit.
   *
   * Called from the message thread when the kit parameter changes or the plugin is set up. The
   * engine takes a shared_ptr, so a kit swap cannot free audio the audio thread is still reading.
   */
  void SetDrumKit(std::shared_ptr<const keith::drums::KitData> kit)
  {
    mDrums.Publish(std::move(kit));
    ApplyDrumSlotParams();
  }

  /** The loaded kit, so the editor and the serialiser can see it. */
  const std::shared_ptr<const keith::drums::KitData>& DrumKit() const { return mDrums.Kit(); }

  /**
   * Point the multisample instrument layer at a loaded guitar bank.
   *
   * Separate from the drums entirely. This plays across the MIDI range through the sampler path
   * and never touches the drum engine or the pads -- the owner was explicit that guitars belong in
   * the piano roll, not on the drum machine.
   */
  void SetInstrumentBank(std::shared_ptr<const keith::instruments::BankData> bank)
  {
    mInstrument.Publish(std::move(bank));
  }

  const std::shared_ptr<const keith::instruments::BankData>& InstrumentBank() const
  {
    return mInstrument.Bank();
  }

  /** Linear gain for the guitar layer, applied on trigger. */
  void SetInstrumentGain(float gain) { mInstrumentGain = gain; }

  bool InstrumentHandlesNotes() const { return mInstrument.HandlesNotes(); }

  /** Audio thread. */
  bool TriggerInstrument(int note, float velocity)
  {
    return mInstrument.Trigger(note, velocity, mInstrumentGain);
  }

  void ReleaseInstrument(int note) { mInstrument.Release(note); }
  float ProcessInstrument() { return mInstrument.Process(); }

  /**
   * The sampled vocal layer.
   *
   * A SECOND instrument engine, not the guitar's. Both are multisamples and share a class, but they
   * are separate layers that can sound at once -- a guitar patch with the choir behind it is an
   * ordinary thing to want -- and sharing one engine would mean loading a guitar evicted the
   * vocals and the two voices competed for the same 16-voice pool.
   *
   * The vocal bank is the same shape as a guitar bank (one file per recorded note, named by MIDI
   * number), which is why there is no third engine.
   */
  void SetVocalBank(std::shared_ptr<const keith::instruments::BankData> bank)
  {
    mVocal.Publish(std::move(bank));
    mVocalEnabled = mVocal.HandlesNotes();
  }

  void SetVocalEnabled(bool on) { mVocalEnabled = on && mVocal.HandlesNotes(); }
  bool VocalEnabled() const { return mVocalEnabled; }

  bool VocalHandlesNotes() const { return mVocalEnabled && mVocal.HandlesNotes(); }

  bool TriggerVocal(int note, float velocity)
  {
    return mVocal.Trigger(note, velocity, mVocalGain);
  }

  void ReleaseVocal(int note) { mVocal.Release(note); }
  float ProcessVocal() { return mVocalEnabled ? mVocal.Process() : 0.f; }

  /**
   * The percussion kit.
   *
   * A SECOND engine rather than more slots on the first, and it is triggered from notes in its own
   * range -- so a percussion kit can be swapped without disturbing the drum kit, and neither
   * engine needs to know the other exists. Offsetting slots inside one engine would put that
   * knowledge in the slot arithmetic, which is where it would be easy to get wrong.
   */
  void SetPercussionKit(std::shared_ptr<const keith::drums::KitData> kit, int firstSlot)
  {
    mPercussionFirstSlot = firstSlot;
    mPercussion.Publish(std::move(kit));
  }

  const std::shared_ptr<const keith::drums::KitData>& PercussionKit() const
  {
    return mPercussion.Kit();
  }

  /** True when a note lands in the percussion kit's range and that slot holds audio. */
  bool HandlesPercussionNote(int note) const
  {
    return mPercussionFirstSlot >= 0 && mPercussion.Kit()
           && mPercussion.Kit()->LoadedSlots() > 0
           && note >= (keith::drums::kFirstMidiNote + mPercussionFirstSlot);
  }

  /** Audio thread. Triggers the percussion engine. */
  bool TriggerPercussion(int note, float velocity)
  {
    return mPercussion.Trigger(note, velocity);
  }

  float ProcessPercussion() { return mPercussion.Process(); }

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

    // The warped sampler renders a whole block at a time, because Bungee works in grains rather
    // than per sample. Doing it here, before the sample loop, keeps the loop free of it.
    RenderWarpBlock(nFrames);

    int drumEvent = 0;
    for (int s = 0; s < nFrames; ++s)
    {
      while (drumEvent < mNumDrumEvents && mDrumEvents[drumEvent].offset <= s)
      {
        const int slot = mDrumEvents[drumEvent].slot;
        // The drum engine is keyed by MIDI note, like the sampler, so slot N is note 36+N.
        mDrums.Trigger(keith::drums::kFirstMidiNote + slot, mDrumEvents[drumEvent].velocity);
        // Record the hit so the editor can flash the pad. The pad lights for every trigger,
        // whether it came from MIDI or from a click in the editor.
        RecordPadHit(slot);
        ++drumEvent;
      }

      float drumLeft = 0.f;
      float drumRight = 0.f;
      // The sampled kit is mono, summed into both outputs. A recorded hit is mono more often
      // than not, and panning it would put the kit off-centre for no reason the player asked for.
      if (mDrumGainsDirty)
        ApplyDrumSlotParams();
      // The guitar layer sums with the drums -- both are sample sources added on top of the synth
      // bus, and both go through the same insert chain afterwards.
      const float drumMono = mDrums.Process() + mPercussion.Process() + mInstrument.Process()
                             + ProcessVocal();
      drumLeft += drumMono;
      drumRight += drumMono;
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
      if (noteMessage && keith::drums::IsDrumNote(msg.NoteNumber()) && (mDrums.Kit() && mDrums.Kit()->LoadedSlots() > 0))
      {
        if (status == IMidiMsg::kNoteOn && msg.Velocity() > 0)
          QueueDrumEvent(msg.mOffset, keith::drums::SlotForMidiNote(msg.NoteNumber()),
                         static_cast<float>(msg.Velocity()) / 127.f);
      }
      if (noteMessage)
        return;
    }

    // --- the multisampled instrument layer (guitars) --------------------------------------------
    //
    // Ahead of the synth and behind the sampler, and entirely separate from the drums: this is the
    // piano-roll path the owner asked for. When no bank is loaded this does nothing, so the synth
    // behaves exactly as before.
    if (noteMessage && mInstrument.HandlesNotes())
    {
      const int note = msg.NoteNumber();
      const bool on = status == IMidiMsg::kNoteOn && msg.Velocity() > 0;
      if (on)
      {
        // Triggered directly rather than queued. The drum path queues because a drum event has a
        // sample offset within the block; a played instrument note starts at the block boundary
        // like a synth or sampler note, and queueing it would add offset machinery for nothing.
        if (mInstrument.Trigger(note, static_cast<float>(msg.Velocity()) / 127.f, mInstrumentGain))
          return;
        // A bank is loaded but this note found no zone: fall through rather than swallow, because
        // a silently dead key is a worse failure than an unexpected synth note.
      }
      else if (status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && msg.Velocity() == 0))
      {
        mInstrument.Release(note);
        return;
      }
    }

    // --- the sampled choir ----------------------------------------------------------------------
    //
    // Behind the sampler and behind the guitar, so a loaded sample or a guitar patch wins. The
    // choir is a layer meant to sit under something else, not to take the keyboard away from it.
    if (noteMessage && VocalHandlesNotes())
    {
      const int note = msg.NoteNumber();
      const bool on = status == IMidiMsg::kNoteOn && msg.Velocity() > 0;
      if (on)
        TriggerVocal(note, static_cast<float>(msg.Velocity()) / 127.f);
      else if (status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && msg.Velocity() == 0))
        ReleaseVocal(note);
    }

    // --- the sampler takes the whole keyboard -------------------------------------------------
    //
    // Owner's rule: "if a sample is in sampler. drums and synth are overridden and only sampler
    // triggers."
    //
    // Placed here, AFTER the PAD MIDI branch and BEFORE the drums. The order is the fix, and
    // both ends of it matter:
    //
    //   * After PAD MIDI, because that switch is an explicit mode -- "only drum pads are
    //     triggered, nothing should come from synth". It outranks the sampler, so with the pads
    //     switch on the pads still play and a loaded sample stays silent.
    //   * Before the drums, because the drum branch RETURNS as soon as it sees a drum note. With
    //     it first, the sampler never received notes 36-51 and SLICE mode's first sixteen slices
    //     were unreachable whenever a kit was audible -- which is always, by default.
    //
    // With no sample loaded this does nothing, so the synth and drums behave exactly as before.
    if (mSampler && mSampler->HasSample())
    {
      if (noteMessage)
      {
        const int note = msg.NoteNumber();
        const bool on = status == IMidiMsg::kNoteOn && msg.Velocity() > 0;
        const float velocity = static_cast<float>(msg.Velocity()) / 127.f;

        if (on)
        {
          // The ratio and range come from the sampler's mode: PLAY transposes the whole sample,
          // SLICE plays the mapped slice at its recorded pitch. A key outside the slice range in
          // SLICE mode starts nothing -- deliberately, since a slicer that also played the synth
          // underneath would be two instruments at once.
          mSampler->TriggerNote(note, velocity);
        }

        // Note-off is swallowed: these are one-shots, so a slice plays to its end. That is what
        // makes a chopped break keep its rhythm rather than stuttering on key release.
        return;
      }

      // Modulation and pitch bend do nothing to a one-shot sample, so they are swallowed.
      //
      // All-notes-off is NOT swallowed. The host sends it when playback stops, and it is what
      // silences the drums -- swallowing it here would leave any ringing drum event sounding
      // after the transport stopped. So it falls through to the handling below. That is the
      // general rule for this block: swallow what a one-shot genuinely cannot use, and let
      // everything else through to the code that owns it.
      if (status == IMidiMsg::kPitchWheel)
        return;

      if (status == IMidiMsg::kControlChange)
      {
        const int cc = msg.ControlChangeIdx();
        // 123 is All Notes Off; 120 is All Sound Off. Hosts send either when the transport stops,
        // and both must reach the drum handling below, which is what actually silences ringing
        // drum events.
        //
        // (An earlier version of this comment said 120 was All Notes Off. It is not, and
        // IMidiMsg::kAllNotesOff is 123 -- so the code was right and the reasoning was wrong,
        // which is worse than the reverse. Both are named explicitly now.)
        const bool allNotesOff = (cc == IMidiMsg::kAllNotesOff || cc == 120);
        if (!allNotesOff)
          return;   // mod wheel, sustain and the rest: nothing to do to a one-shot sample
      }
    }

    // --- the percussion kit, which owns the notes above the drum range -------------------------
    //
    // Checked BEFORE the drum branch and before the synth. The percussion range starts above the
    // drums, so the two cannot both match a note -- but ordering it first makes that explicit
    // rather than something the reader has to verify from the ranges.
    if (noteMessage && HandlesPercussionNote(msg.NoteNumber()))
    {
      if (status == IMidiMsg::kNoteOn && msg.Velocity() > 0)
        TriggerPercussion(msg.NoteNumber(), static_cast<float>(msg.Velocity()) / 127.f);
      else if (status == IMidiMsg::kNoteOff || (status == IMidiMsg::kNoteOn && msg.Velocity() == 0))
        mPercussion.Release(msg.NoteNumber());
      return;
    }

    // --- the drum range ------------------------------------------------------------------------
    //
    // The drums claim notes 36-51 ONLY when PAD MIDI is on. That switch is the owner's way of
    // saying "the pads are what I am playing right now".
    //
    // This used to claim the range whenever ANY kit was loaded -- and a kit is always loaded,
    // because OnReset loads one. So on the default startup the drum range was taken over and a
    // drum kit was "in the drum pad" before the player had asked for anything, which is exactly
    // what the owner reported. Gating it on PAD MIDI is the fix: with the switch off, 36-51 fall
    // through to the instrument layer and then the synth like any other note.
    if (noteMessage && mPadMidi && keith::drums::IsDrumNote(msg.NoteNumber())
        && (mDrums.Kit() && mDrums.Kit()->LoadedSlots() > 0))
    {
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
        mDrums.Reset();
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
  bool mDrumGainsDirty = false;

  /**
   * Whether a slot should sound, given mute and solo.
   *
   * The rule is the usual one and worth stating because it is the thing that is easy to get
   * backwards: if ANY slot is soloed, only the soloed slots sound. Otherwise a muted slot is
   * silent and everything else sounds.
   */
  bool SlotAudible(int slot) const
  {
    if (slot < 0 || slot >= keith::drums::kNumSlots)
      return false;
    bool anySolo = false;
    for (int i = 0; i < keith::drums::kNumSlots; ++i)
      if (mDrumParams.slot[i].solo)
        anySolo = true;
    if (anySolo)
      return mDrumParams.slot[slot].solo;
    return !mDrumParams.slot[slot].mute;
  }
  /** Set from Keith::ProcessBlock; see SetPadMidi. */
  bool mPadMidi = true;
  keith::drums::DrumKitEngine mDrums;
  /** Percussion, on notes above the drum kit's own range. -1 until a kit is loaded. */
  keith::drums::DrumKitEngine mPercussion;
  int mPercussionFirstSlot = -1;
  /** The multisampled guitar layer, played across the keyboard. */
  keith::instruments::InstrumentEngine mInstrument;
  float mInstrumentGain = 1.f;
  /** The sampled choir, on its own engine so a guitar cannot evict it. */
  keith::instruments::InstrumentEngine mVocal;
  bool mVocalEnabled = false;
  float mVocalGain = 0.7f;
  DrumEvent mDrumEvents[kMaxDrumEvents] {};
  /** The plugin's sampler. Null until SetSamplerEngine is called; never dereferenced when null. */
  keith::sampler::SamplerEngine* mSampler = nullptr;
  /** Block-rate scratch for the warped sampler, and whether it was filled this block. */
  std::vector<float> mWarpScratch;
  std::vector<float> mWarpScratchR;
  bool mWarpBlockActive = false;
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
