// Forces the wavetable header to be compiled.
//
// WavetableOsc.h is header-only and nothing includes it yet -- the voice does not use it
// until the third oscillator lands. A header that nothing includes is never compiled, so a
// build would pass while proving nothing about it. This translation unit exists purely so the
// header is type-checked by the normal build.

#include "../src/synth/WavetableOsc.h"

namespace {

// Touch the public surface so the signatures are checked, not just parsed.
void InstantiateForTypeChecking()
{
  keith::synth::WavetableBank bank;
  keith::synth::WavetableFrame frame = keith::synth::WavetableFrame::Sine();
  bank.SetFrames({frame});

  keith::synth::WavetableOscillator osc;
  osc.SetSampleRate(44100.0);
  osc.SetBank(&bank);
  osc.Reset();
  (void) osc.Process(220.f, 0.f);
  (void) osc.Wrapped();
  osc.SyncReset();

  (void) keith::synth::WavetableBank::BandLimitFor(440.f, 44100.0);
}

} // namespace
