// The single translation unit that instantiates the sampler's file decoders.
//
// SampleLoader.h defines the dr_libs implementation macros, which must happen in exactly one
// translation unit -- defining them in more than one produces duplicate symbols at link time.
// This is that unit.
//
// It also pulls in SamplerVoice.h, and both are header-only, so this file is what gets them
// compiled at all. A header no translation unit includes is never type-checked.

#include "../src/sampler/SampleLoader.h"
#include "../src/sampler/SamplerEngine.h"
#include "../src/sampler/SamplerVoice.h"

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
  (void) engine.Load("/nonexistent/probe.wav");
  (void) engine.HasSample();
  (void) engine.Sample();
  (void) engine.LastPath();
  (void) engine.LastError();
  engine.Trigger(1.f);
  (void) engine.Process();
  (void) engine.AnyActive();
  engine.StopAll();
}

} // namespace
