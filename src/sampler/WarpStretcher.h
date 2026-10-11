#pragma once

// The warp stretcher: Bungee behind the plugin's own interface.
//
// Bungee does the stretching. Everything here is about driving it correctly, and about the one
// thing that genuinely constrains the design: Bungee allocates, so it cannot be constructed on
// the audio thread.
//
// ## How Bungee is driven
//
// It is pull-based, in overlapping grains:
//
//   1. `specifyGrain(request)` returns the input frame range that grain needs.
//   2. `analyseGrain(data, ...)` is handed those frames.
//   3. `synthesiseGrain(output)` produces output and returns the request describing it.
//
// `request.position` is the centre of the grain **in input frames**, and `request.pitch` is a
// frequency multiplier. Both are set per grain, which is the reason Bungee was chosen: a warp map
// computes a different ratio per segment, so the stretcher has to accept a rate that changes
// continuously rather than one global tempo.
//
// `preroll` runs the request backwards a few grains so the first edges are not weak, and `next`
// advances the request for a constant-rate granular loop.
//
// ## The threading rule
//
// A Bungee stretcher is created with `create()` and destroyed with `destroy()`; both allocate.
// So a stretcher is built on the message thread and lives for as long as the sample does, and the
// audio thread only ever calls `specifyGrain` / `analyseGrain` / `synthesiseGrain` on one that
// already exists. Loading a new file rebuilds them, on the message thread.
//
// ## What is not here yet
//
// The engine that decides *when* to render through the stretcher, and that turns a warp map into
// a sequence of grain positions, is the next step. This file is the boundary, kept small on
// purpose so the part which is hard to verify (the mapping) stays in WarpMap.h, where it is
// tested exactly.

#include "WarpMap.h"

// Bungee's C API and a thin C++ wrapper. Bungee's headers are safe to include from C++17: the
// function table beside this uses only plain C types, and the C++ Stretcher<> wrapper above it is
// a small inline shim.
#include "bungee/Bungee.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace keith {
namespace sampler {

/**
 * Which Bungee edition to use.
 *
 * Basic is the open-source implementation in this repository, under MPL-2.0. Pro is a commercial
 * enhancement that is not included and would not resolve at link time, so Basic is used.
 */
using BungeeEdition = Bungee::Basic;

/** Bungee's sample rates, in Hz. */
inline Bungee::SampleRates WarpRates(int inputRate, int outputRate)
{
  Bungee::SampleRates r {};
  r.input = inputRate;
  r.output = outputRate;
  return r;
}

/**
 * True when this build has a stretcher at all.
 *
 * The plugin is built without one when bootstrap-deps has not fetched Eigen, so callers must ask
 * rather than assume. A warp switch that silently does nothing is worse than one that reports
 * itself unavailable.
 *
 * Checked by asking Bungee for its edition string, which is a cheap call into the real function
 * table rather than a compile-time guess.
 */
inline bool WarpStretcherAvailable()
{
  return BungeeEdition::getFunctions()->edition() != nullptr;
}

/** Bungee's own reported latency in frames, at a given sample rate. */
inline int WarpStretcherLatency(int sampleRate)
{
  // Bungee exposes no latency query in the C table. The figure that matters -- how many frames of
  // run-in the pipeline needs before output is valid -- is bounded by the grain pipeline depth,
  // which Bungee reports indirectly through maxInputFrameCount(). Measured empirically later;
  // until then this reports nothing rather than inventing a number.
  (void) sampleRate;
  return 0;
}

} // namespace sampler
} // namespace keith
