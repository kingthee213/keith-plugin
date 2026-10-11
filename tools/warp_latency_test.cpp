// Measures Bungee's latency and reports it, so the audio path has a real number rather than a
// guess. Built and run by the macOS CI job, because this machine has no C++ compiler.
//
// ## Why this exists
//
// Bungee's C function table has no latency query. The warp voice needs one: if the stretcher
// emits its first valid output N frames late and the engine does not account for it, every slice
// fires N frames after its beat. That is a drift nobody can see in the code and everybody hears
// in the rhythm.
//
// ## How latency is measured
//
// Impulse in, find the impulse out.
//
//   1. Build a stretcher at a known rate.
//   2. Feed it a sample that is silent except for a single impulse at a known frame.
//   3. Run grains until output appears.
//   4. Find the frame index of the peak in the output.
//
// The difference between where the impulse was asked for and where it came out is the pipeline
// delay. Run at speed 1.0 so resampling does not confound it.
//
// This measures the *reported* latency through the API the plugin actually uses, not an internal
// figure, so what it prints is what the engine has to compensate for.

#include "../src/sampler/WarpStretcher.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

namespace {

constexpr int kSampleRate = 44100;
constexpr int kImpulseFrame = 5000;
constexpr int kSampleFrames = 44100;      // one second of source
constexpr int kOutputFrames = 44100;      // and one second of output to search

/** A one-second source: silence with a single impulse. */
std::vector<float> MakeImpulseSource()
{
  std::vector<float> src(static_cast<std::size_t>(kSampleFrames), 0.f);
  src[static_cast<std::size_t>(kImpulseFrame)] = 1.f;
  return src;
}

/**
 * Render `frames` of output at a constant speed, and report the peak position.
 *
 * Mirrors what WarpVoice::Process does, but with a fixed position instead of a warp map, so the
 * measurement is of the stretcher alone.
 */
struct RenderResult
{
  std::vector<float> output;
  int peakFrame = -1;
  float peakValue = 0.f;
  int grains = 0;
  bool ok = false;
};

RenderResult RenderConstantSpeed(double speed, int frames)
{
  RenderResult result;

  Bungee::Stretcher<BungeeEdition> stretcher(
      WarpRates(kSampleRate, kSampleRate), 1);
  if (stretcher.state == nullptr)
    return result;

  const std::vector<float> source = MakeImpulseSource();

  Bungee::Request request {};
  request.position = 0.;
  request.speed = speed;
  request.pitch = 1.;
  request.reset = true;
  request.resampleMode = resampleMode_autoOut;

  stretcher.preroll(request);

  result.output.assign(static_cast<std::size_t>(frames), 0.f);
  std::vector<float> inputScratch;

  int written = 0;
  int guard = 0;
  const int maxGrains = frames * 4;   // a runaway guard, not a tuning value

  while (written < frames && guard++ < maxGrains)
  {
    const Bungee::InputChunk chunk = stretcher.specifyGrain(request);
    const int chunkFrames = chunk.end - chunk.begin;
    if (chunkFrames <= 0)
      break;

    // Copy the requested range, counting anything outside the source as muted.
    const int head = std::max(0, -chunk.begin);
    const int tail = std::max(0, chunk.end - kSampleFrames);
    const int first = std::max(0, chunk.begin);
    const int last = std::min(kSampleFrames, chunk.end);

    inputScratch.assign(static_cast<std::size_t>(chunkFrames), 0.f);
    for (int i = first; i < last; ++i)
    {
      const int out = i - chunk.begin;
      if (out >= 0 && out < chunkFrames)
        inputScratch[static_cast<std::size_t>(out)] = source[static_cast<std::size_t>(i)];
    }

    stretcher.analyseGrain(inputScratch.data(), static_cast<intptr_t>(chunkFrames), head, tail);

    Bungee::OutputChunk out {};
    out.data = result.output.data() + written;
    out.frameCount = frames - written;
    out.channelStride = static_cast<intptr_t>(frames);
    stretcher.synthesiseGrain(out);

    const int produced = out.frameCount;
    if (produced <= 0)
      break;
    written += produced;
    ++result.grains;
  }

  for (int i = 0; i < frames; ++i)
  {
    const float v = std::fabs(result.output[static_cast<std::size_t>(i)]);
    if (v > result.peakValue)
    {
      result.peakValue = v;
      result.peakFrame = i;
    }
  }
  result.ok = result.peakFrame >= 0 && result.peakValue > 0.01f;
  return result;
}

} // namespace

int main()
{
  std::printf("Bungee latency measurement\n");
  std::printf("  available: %s\n", keith::sampler::WarpStretcherAvailable() ? "yes" : "no");
  std::printf("  edition:   %s\n", BungeeEdition::getFunctions()->edition());
  std::printf("  version:   %s\n", BungeeEdition::getFunctions()->version());
  std::printf("  sample rate %d, impulse at frame %d\n\n", kSampleRate, kImpulseFrame);

  int failures = 0;

  // Speed 1.0 only. At other speeds the output is a different length from the input, so an
  // impulse offset would conflate pipeline delay with the rate change.
  const RenderResult r = RenderConstantSpeed(1.0, kOutputFrames);

  if (!r.ok)
  {
    std::printf("  FAIL  no impulse found in the output (peak %.5f at frame %d)\n",
                r.peakValue, r.peakFrame);
    ++failures;
  }
  else
  {
    const int latency = r.peakFrame - kImpulseFrame;
    std::printf("  peak at output frame %d, source frame %d\n", r.peakFrame, kImpulseFrame);
    std::printf("  latency = %d frames (%.2f ms at %d Hz)\n",
                latency, 1000.0 * latency / kSampleRate, kSampleRate);
    std::printf("  grains rendered: %d\n", r.grains);
    std::printf("\n  VERDICT: Bungee's pipeline delay at speed 1.0 is %d frames.\n", latency);
    std::printf("  The engine must subtract this when it places grains, or every slice\n");
    std::printf("  fires this many frames after its beat.\n");
  }

  std::printf("\n%s\n", failures == 0 ? "PASS" : "FAIL");
  return failures == 0 ? 0 : 1;
}
