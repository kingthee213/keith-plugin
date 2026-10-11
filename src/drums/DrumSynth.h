/**
 * Drum kit parameters.
 *
 * ## What is left here
 *
 * The **parameters only**: per-slot level, pan, tune, decay, mute and solo, plus the kit level.
 * These are serialised, are bound to the drum panel's knobs, and are unrelated to how a hit is
 * produced.
 *
 * ## What was removed
 *
 * The synthesised voice and engine -- `VoiceSpec`, `detail::`, `Voice` and `Engine`. Every hit used
 * to be generated from oscillators and noise, which is why the kits did not sound like records. The
 * sound now comes from the CC0 sample kits in `resources/samples/drums` via `DrumKitEngine`.
 *
 * Anything that still needs the old synthesis is a bug, not a feature: nothing outside this file
 * referenced those types, and the engine that replaced them is `drums/DrumKitEngine.h`.
 *
 * This file is named DrumSynth.h for the sake of the history, not the contents.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

// Original synthesized percussion for Keith. This engine uses oscillators, deterministic
// noise, envelopes, a one-pole filter and soft clipping. It contains no sample playback.

namespace keith {
namespace drums {

constexpr int kNumSlots = 16;
constexpr int kFirstMidiNote = 36;
constexpr int kLastMidiNote = 51;
constexpr float kSilenceDb = -60.f;

inline bool IsDrumNote(int note)
{
  return note >= kFirstMidiNote && note <= kLastMidiNote;
}

inline int SlotForMidiNote(int note)
{
  return IsDrumNote(note) ? note - kFirstMidiNote : -1;
}

enum class MidiDestination
{
  Synth,
  Drum
};

inline MidiDestination DestinationForMidiNote(int note)
{
  return IsDrumNote(note) ? MidiDestination::Drum : MidiDestination::Synth;
}

struct SlotParams
{
  float levelDb = 0.f;
  float pan = 0.f;
  float tuneSemitones = 0.f;
  float decay = 0.70f;
  float filterHz = 12000.f;
  float drive = 0.f;
  bool mute = false;
  bool solo = false;
};

struct KitParams
{
  SlotParams slot[kNumSlots];
  float levelDb = 0.f;
};

inline float DbToLinear(float db)
{
  if (db <= kSilenceDb)
    return 0.f;
  return std::pow(10.f, db / 20.f);
}

inline SlotParams DefaultSlotParams(int slot)
{
  static constexpr float kLevel[kNumSlots] = {
     0.f, -1.f, -3.f, -2.f, -4.f, -4.f, -5.f, -2.f,
    -2.f, -2.f, -5.f, -4.f, -3.f, -6.f, -5.f, -4.f
  };
  static constexpr float kPan[kNumSlots] = {
     0.f,  0.f,  0.10f, -0.10f,  0.25f,  0.30f, -0.20f, -0.18f,
     0.f,  0.18f, 0.20f, -0.20f, -0.25f,  0.25f, -0.30f,  0.30f
  };
  static constexpr float kDecay[kNumSlots] = {
    0.70f, 0.55f, 0.25f, 0.42f, 0.18f, 0.72f, 0.16f, 0.48f,
    0.44f, 0.40f, 0.78f, 0.88f, 0.46f, 0.36f, 0.24f, 0.52f
  };
  static constexpr float kFilter[kNumSlots] = {
    12000.f, 11000.f, 15000.f, 13000.f, 18000.f, 18000.f, 15000.f, 9000.f,
    10000.f, 11000.f, 17000.f, 18000.f, 12000.f, 16000.f, 14000.f, 13000.f
  };

  const int i = std::clamp(slot, 0, kNumSlots - 1);
  SlotParams params;
  params.levelDb = kLevel[i];
  params.pan = kPan[i];
  params.decay = kDecay[i];
  params.filterHz = kFilter[i];
  return params;
}

inline KitParams DefaultKitParams()
{
  KitParams params;
  for (int i = 0; i < kNumSlots; ++i)
    params.slot[i] = DefaultSlotParams(i);
  return params;
}

} // namespace drums
} // namespace keith
