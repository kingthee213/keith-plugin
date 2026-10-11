// Forces the sampler's headers to be compiled.
//
// The headers are header-only and nothing else includes some of them, and a header no
// translation unit includes is never type-checked.
//
// NOTE: this unit deliberately does NOT define the dr_libs implementation macros -- those live
// in src/sampler/SampleLoader.cpp and must exist in exactly one translation unit. Including
// SampleLoader.h here is fine because it now only declares LoadFile; the decoders and dr_libs
// itself are compiled in the .cpp. The first attempt had the macros in the header, which put
// every dr_libs symbol in both this unit and Keith.o and the link failed with hundreds of
// duplicate symbols.

#include "../src/sampler/SampleLoader.h"
#include <memory>
#include <vector>
#include "../src/sampler/SamplerEngine.h"
#include "../src/sampler/SamplerVoice.h"
#include "../src/sampler/SampleSlicer.h"
#include "../src/sampler/WarpMap.h"
#include "../src/sampler/WarpStretcher.h"
#include "../src/sampler/WarpVoice.h"
#include "../src/sampler/WarpEngine.h"

namespace {

// Touch the public surface so the signatures are checked, not merely parsed.
void InstantiateForTypeChecking()
{
  keith::sampler::LoadError err = keith::sampler::LoadError::None;

  std::shared_ptr<const keith::sampler::SampleData> sample =
      keith::sampler::LoadFile("/nonexistent/probe.wav", &err);
  (void) sample;
  (void) keith::sampler::LoadErrorText(err);

  keith::sampler::SamplerVoice voice;
  voice.SetSampleRate(44100.0);
  voice.SetSample(nullptr);
  voice.SetPitchRatio(1.0);
  voice.Trigger(1.f);
  voice.Stop();
  (void) voice.Active();
  (void) voice.Sample();
  (void) voice.Process();

  (void) keith::sampler::kMaxDecodedBytes;

  // The engine that owns the voices, which the plugin holds and the DSP reads.
  keith::sampler::SamplerEngine engine;
  engine.SetSampleRate(44100.0);
  (void) engine.HasSample();
  (void) engine.Sample();
  (void) engine.LastPath();
  (void) engine.LastError();
  engine.Trigger(1.f);
  (void) engine.Process();
  (void) engine.AnyActive();
  engine.StopAll();

  // Slicing: detection and the boundary edits the UI will drive.
  keith::sampler::SliceSettings settings;
  keith::sampler::SampleData empty;
  keith::sampler::SliceResult found =
      keith::sampler::DetectTransients(empty, 44100.0, settings);
  (void) found.ok;
  (void) found.rawOnsets;

  std::vector<keith::sampler::Slice> slices = keith::sampler::SingleSlice();
  keith::sampler::MoveBoundary(slices, 0, 100, 1000);
  keith::sampler::AddBoundary(slices, 500, 1000, 40);
  (void) keith::sampler::SliceEndFrame(slices, 0, 1000);
  (void) keith::sampler::SliceAtFrame(slices, 10, 1000);
  keith::sampler::RemoveBoundary(slices, 0);

  // Warp: tempo, the marker map, and the mapping the engine will read.
  keith::sampler::Transport transport;
  (void) transport.Valid();
  (void) transport.SecondsPerBeat();

  keith::sampler::WarpMap warp;
  warp.SetTempo(transport.tempo);
  warp.SetHostSampleRate(44100.0);
  warp.enabled = true;
  warp.sourceTempo = 120.0;
  warp.FromSlices(slices, 1.0);
  warp.SetMarker(0, 0.0, false);
  warp.SetMarker(1000, 1.0, true);
  warp.MoveMarkerBeat(1, 2.0);
  (void) warp.Usable();
  (void) warp.StretchAtBeat(0.5);
  (void) warp.FrameAtBeat(0.5);
  (void) warp.BeatAtFrame(500.0);
  (void) warp.TotalBeats();
  (void) keith::sampler::SliceBeat(warp, slices, 1, 1.0);
  warp.RemoveMarker(0);
  warp.Clear();

  // The Bungee boundary and the voice that drives it. Constructing the voice allocates, which is
  // exactly why it happens here rather than in a render path.
  (void) keith::sampler::WarpStretcherAvailable();
  (void) keith::sampler::WarpStretcherLatency(44100);

  keith::sampler::WarpVoice warpVoice;
  warpVoice.Prepare(2, 44100, 44100);
  (void) warpVoice.Prepared();
  (void) warpVoice.MaxInputFrameCount();
  (void) warpVoice.Active();
  (void) warpVoice.FramesRendered();
  warpVoice.Stop();

  // The engine that decides between plain and warped, and holds the latency compensation.
  keith::sampler::WarpEngine warpEngine;
  warpEngine.Prepare(2, 44100);
  (void) warpEngine.WarpAvailable();
  (void) warpEngine.LatencyCompensationFrames();
  warpEngine.SetLatencyCompensationFrames(0);
  warpEngine.SetSample(&empty);
  warpEngine.SetWarpMap(&warp);
  warpEngine.TriggerSlice(0.0);
  (void) warpEngine.Active();
  (void) warpEngine.ModeFor(warp);
  (void) keith::sampler::WarpEngine::BeatInSample(transport, 0.0, 0.0);
  warpEngine.Stop();

  // --- the keyboard-to-slice path, and the published slice set -------------------------------
  //
  // Type-checking only. This unit is compiled into an object library and never run, so there are
  // no error codes here: the assertions that must actually EXECUTE live in tools/slicer_check.py
  // and the mirrored simulations. What matters here is that every call below compiles and that
  // the signatures agree.
  {
    keith::sampler::SamplerEngine engine;
    engine.SetSampleRate(44100.);

    // Modes and reverse.
    engine.SetMode(keith::sampler::SampleMode::Play);
    engine.SetMode(keith::sampler::SampleMode::Slice);
    (void) engine.Mode();
    engine.SetReversed(true);
    engine.SetReversed(false);
    (void) engine.Reversed();

    // A snapshot of the slices. The return is a shared_ptr, not a reference, so a caller holds
    // whatever it is reading alive across an edit.
    std::shared_ptr<const std::vector<keith::sampler::Slice>> snapshot = engine.Slices();
    (void) snapshot;

    // Which key maps to which slice.
    (void) engine.SliceForNote(36);
    (void) engine.SliceForNote(60);

    // The trigger the DSP calls on a note-on.
    (void) engine.TriggerNote(60, 0.8f);

    // The three writers. Each publishes a new set rather than editing the live one.
    std::vector<keith::sampler::Slice> three(3);
    three[0].startFrame = 0;
    three[1].startFrame = 1000;
    three[2].startFrame = 5000;
    engine.SetSlices(three);
    engine.MoveSliceBoundary(1, 1200);
    (void) engine.DetectSlices(keith::sampler::SliceSettings {});

    // A fresh snapshot still describes a consistent set.
    std::shared_ptr<const std::vector<keith::sampler::Slice>> after = engine.Slices();
    (void) after;
  }

  // --- reverse on the voice ------------------------------------------------------------------
  {
    keith::sampler::SamplerVoice voice;
    voice.SetSampleRate(44100.);

    keith::sampler::SampleData data;
    data.sampleRate = 44100.;
    data.frames = 2000;
    data.channels.assign(1, std::vector<float>(static_cast<std::size_t>(data.frames), 0.25f));

    const int frames = data.frames;                       // captured before the move below
    voice.SetSample(std::make_shared<const keith::sampler::SampleData>(std::move(data)));

    voice.SetReversed(true);
    (void) voice.Reversed();
    voice.SetRange(0, frames - 1);
    voice.Trigger(1.f);
    (void) voice.Active();
    (void) voice.Process();
    voice.Stop();

    // A one-frame range is the boundary both directions have to survive.
    voice.SetReversed(false);
    voice.SetRange(500, 500);
    voice.Trigger(1.f);
    (void) voice.Process();

    voice.SetReversed(true);
    voice.SetRange(500, 500);
    voice.Trigger(1.f);
    (void) voice.Process();
  }
}

} // namespace
