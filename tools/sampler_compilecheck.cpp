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
