#include "Keith.h"

#include "pluginterfaces/vst/ivstaudioprocessor.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <memory>
#include <vector>

// Three properties of the shipped state layer, each driven through the plugin's own
// host-facing entry points rather than a hand-rolled copy of them:
//
//   1. every baked preset recalls through RestorePreset and resolves to a distinct state
//   2. the insert and master FX chain each preset preloads is actually in the audio path
//   3. SerializeState / UnserializeState round-trip every parameter and every drum slot
//
// The plugin object is large, so it lives on the heap the way a host allocates it.

namespace {

constexpr double kRenderRate = 48000.;
constexpr int kRenderBlock = 512;
// 0.75 s is long enough for the slowest preloaded insert delay to return a tap.
constexpr int kRenderBlocks = 70;
constexpr int kProbeNote = 60;
constexpr int kProbeVelocity = 100;

// Engaging a preset's chain must change the output by at least this fraction of the
// signal's RMS. The floor sits well above the noise sources' own render-to-render
// variation, which kMaxRenderDrift pins down, and well below the weakest chain in the
// library. The test prints both margins so a reader can see the gap has not closed.
constexpr double kMinChainChange = 1e-2;
constexpr double kMaxRenderDrift = 5e-3;
constexpr double kMinAudible = 1e-6;

// The first drum slot whose parameters are driven to their extremes, so parameters
// 76-83 provably differ from their defaults in the probe state.
constexpr int kProbeDrumSlot = 7;

using iplug::sample;

bool IsSelectedSlotParam(int paramIdx)
{
  return paramIdx >= kParamDrumSlotLevel && paramIdx <= kParamDrumSlotSolo;
}

/** The legal value furthest from a parameter's default, so a parameter that silently
 * falls back to its default cannot survive a round-trip unnoticed. */
double FurthestFromDefault(const iplug::IParam* param)
{
  const double fromMin = std::fabs(param->GetDefault() - param->GetMin());
  const double fromMax = std::fabs(param->GetMax() - param->GetDefault());
  return fromMax >= fromMin ? param->GetMax() : param->GetMin();
}

void SetThroughHostPath(Keith& plugin, int paramIdx, double value)
{
  plugin.GetParam(paramIdx)->Set(value);
  plugin.OnParamChange(paramIdx);
}

void SelectDrumSlot(Keith& plugin, int slot)
{
  SetThroughHostPath(plugin, kParamDrumSlotSelect, static_cast<double>(slot));
}

/** Tells the plugin its sample rate and block size over the VST3 processor interface,
 * the same call a host makes before it asks for audio. It ends in OnReset(), so every
 * render below starts from a cleared tape, delay line and voice pool. */
void PrepareProcessing(Keith& plugin)
{
  Steinberg::Vst::ProcessSetup setup {};
  setup.processMode = Steinberg::Vst::kRealtime;
  setup.symbolicSampleSize = sizeof(sample) == sizeof(double) ? Steinberg::Vst::kSample64
                                                             : Steinberg::Vst::kSample32;
  setup.maxSamplesPerBlock = kRenderBlock;
  setup.sampleRate = kRenderRate;
  plugin.setupProcessing(setup);
}

struct Render
{
  std::vector<sample> left;
  std::vector<sample> right;
};

void RenderHeldNote(Keith& plugin, Render& out)
{
  PrepareProcessing(plugin);

  iplug::IMidiMsg noteOn;
  noteOn.MakeNoteOnMsg(kProbeNote, kProbeVelocity, 0);
  plugin.ProcessMidiMsg(noteOn);

  const size_t frames = static_cast<size_t>(kRenderBlock) * kRenderBlocks;
  out.left.assign(frames, sample(0));
  out.right.assign(frames, sample(0));

  std::vector<sample> blockLeft(kRenderBlock), blockRight(kRenderBlock);
  sample* outputs[2] {blockLeft.data(), blockRight.data()};

  for (int block = 0; block < kRenderBlocks; ++block)
  {
    std::fill(blockLeft.begin(), blockLeft.end(), sample(0));
    std::fill(blockRight.begin(), blockRight.end(), sample(0));
    plugin.ProcessBlock(nullptr, outputs, kRenderBlock);

    const size_t offset = static_cast<size_t>(block) * kRenderBlock;
    std::copy(blockLeft.begin(), blockLeft.end(), out.left.begin() + offset);
    std::copy(blockRight.begin(), blockRight.end(), out.right.begin() + offset);
  }
}

double Rms(const Render& render)
{
  double sum = 0.;
  for (size_t i = 0; i < render.left.size(); ++i)
  {
    const double left = static_cast<double>(render.left[i]);
    const double right = static_cast<double>(render.right[i]);
    sum += left * left + right * right;
  }
  return std::sqrt(sum / static_cast<double>(render.left.size() * 2));
}

double RmsDifference(const Render& a, const Render& b)
{
  double sum = 0.;
  for (size_t i = 0; i < a.left.size(); ++i)
  {
    const double left = static_cast<double>(a.left[i]) - static_cast<double>(b.left[i]);
    const double right = static_cast<double>(a.right[i]) - static_cast<double>(b.right[i]);
    sum += left * left + right * right;
  }
  return std::sqrt(sum / static_cast<double>(a.left.size() * 2));
}

bool Finite(const Render& render)
{
  for (size_t i = 0; i < render.left.size(); ++i)
  {
    if (!std::isfinite(static_cast<double>(render.left[i])) ||
        !std::isfinite(static_cast<double>(render.right[i])))
      return false;
  }
  return true;
}

std::vector<double> CaptureParams(Keith& plugin)
{
  std::vector<double> state(static_cast<size_t>(kNumParams));
  for (int param = 0; param < kNumParams; ++param)
    state[static_cast<size_t>(param)] = plugin.GetParam(param)->Value();
  return state;
}

/** Reads back every drum slot by selecting it, which is the only path the plugin
 * exposes to the 15 slots the host parameters are not currently pointing at. */
std::vector<double> CaptureDrumKit(Keith& plugin)
{
  std::vector<double> kit;
  const int restore = plugin.GetParam(kParamDrumSlotSelect)->Int();
  for (int slot = 1; slot <= keith::drums::kNumSlots; ++slot)
  {
    SelectDrumSlot(plugin, slot);
    for (int param = kParamDrumSlotLevel; param <= kParamDrumSlotSolo; ++param)
      kit.push_back(plugin.GetParam(param)->Value());
  }
  SelectDrumSlot(plugin, restore);
  return kit;
}

// ---------------------------------------------------------------- test 1

bool CheckPresetsRecallDistinctly(Keith& plugin)
{
  if (plugin.NPresets() != kNumPresets)
  {
    std::fprintf(stderr, "expected %d presets, plugin has %d\n", kNumPresets, plugin.NPresets());
    return false;
  }

  std::vector<std::vector<double>> states;
  states.reserve(static_cast<size_t>(kNumPresets));

  for (int preset = 0; preset < plugin.NPresets(); ++preset)
  {
    if (!plugin.RestorePreset(preset) || plugin.GetCurrentPresetIdx() != preset)
    {
      std::fprintf(stderr, "RestorePreset failed for index %d (%s)\n", preset,
                   plugin.GetPresetName(preset));
      return false;
    }

    std::vector<double> state = CaptureParams(plugin);
    for (int param = kParamMod1Source; param <= kParamMod4Amount; ++param)
    {
      if (state[static_cast<size_t>(param)] != 0.)
      {
        std::fprintf(stderr, "preset %d (%s) has active modulation-matrix default at parameter %d\n",
                     preset, plugin.GetPresetName(preset), param);
        return false;
      }
    }
    states.push_back(std::move(state));
  }

  for (int left = 0; left < kNumPresets; ++left)
  {
    for (int right = left + 1; right < kNumPresets; ++right)
    {
      bool same = true;
      for (int param = 0; param < kNumParams; ++param)
      {
        if (std::fabs(states[static_cast<size_t>(left)][static_cast<size_t>(param)] -
                      states[static_cast<size_t>(right)][static_cast<size_t>(param)]) > 1e-6)
        {
          same = false;
          break;
        }
      }
      if (same)
      {
        std::fprintf(stderr, "preset %d (%s) matches preset %d (%s) after RestorePreset\n",
                     left, plugin.GetPresetName(left), right, plugin.GetPresetName(right));
        return false;
      }
    }
  }

  std::printf("PASS %d presets recall through RestorePreset into distinct states\n", kNumPresets);
  return true;
}

// ---------------------------------------------------------------- test 2

/** Renders a held note twice per preset, once as the preset ships and once with the
 * three FX bypass switches thrown, and requires the two to differ. A preset whose
 * chain was never written into kPresetDeltas, or whose chain is not actually wired
 * into ProcessBlock, produces identical audio both ways and fails here. */
bool CheckPreloadedChainsAreInThePath(Keith& plugin)
{
  constexpr int kBypasses[] {kParamIns1Bypass, kParamIns2Bypass, kParamMstBypass};

  double worstChange = 1e9;
  int worstPreset = -1;
  double worstDrift = 0.;

  for (int preset = 0; preset < kNumPresets; ++preset)
  {
    if (!plugin.RestorePreset(preset))
    {
      std::fprintf(stderr, "RestorePreset failed for index %d\n", preset);
      return false;
    }

    const bool anyChain = plugin.GetParam(kParamIns1Algo)->Int() != 0 ||
                          plugin.GetParam(kParamIns2Algo)->Int() != 0 ||
                          plugin.GetParam(kParamMstAlgo)->Int() != 0;
    if (!anyChain)
    {
      std::fprintf(stderr, "preset %d (%s) preloads no FX chain at all\n", preset,
                   plugin.GetPresetName(preset));
      return false;
    }

    Render engaged;
    RenderHeldNote(plugin, engaged);

    // The engines use free-running noise, so two renders of the same preset are close
    // but not identical. Bound that drift, otherwise the change measured below could
    // come from the noise rather than from the bypass switches. Spot-checked across the
    // families rather than run 103 times, because the renders are what cost time here.
    if (preset % 20 == 0)
    {
      Render repeat;
      RenderHeldNote(plugin, repeat);
      const double drift = RmsDifference(engaged, repeat) / std::max(Rms(engaged), kMinAudible);
      worstDrift = std::max(worstDrift, drift);
      if (drift > kMaxRenderDrift)
      {
        std::fprintf(stderr,
                     "preset %d (%s) drifts %.3g between identical renders, over the %.3g "
                     "ceiling the routing measurement relies on\n",
                     preset, plugin.GetPresetName(preset), drift, kMaxRenderDrift);
        return false;
      }
    }

    for (const int bypass : kBypasses)
      SetThroughHostPath(plugin, bypass, 1.);
    Render bypassed;
    RenderHeldNote(plugin, bypassed);

    if (!Finite(engaged) || !Finite(bypassed))
    {
      std::fprintf(stderr, "preset %d (%s) rendered a non-finite sample\n", preset,
                   plugin.GetPresetName(preset));
      return false;
    }

    const double engagedRms = Rms(engaged);
    const double bypassedRms = Rms(bypassed);
    if (engagedRms < kMinAudible && bypassedRms < kMinAudible)
    {
      std::fprintf(stderr, "preset %d (%s) is silent, so routing cannot be observed\n", preset,
                   plugin.GetPresetName(preset));
      return false;
    }

    const double change =
      RmsDifference(engaged, bypassed) / std::max(engagedRms, bypassedRms);
    if (change < worstChange)
    {
      worstChange = change;
      worstPreset = preset;
    }
    if (change < kMinChainChange)
    {
      std::fprintf(stderr,
                   "preset %d (%s) sounds the same with its chain engaged and bypassed "
                   "(relative change %.3g, floor %.3g)\n",
                   preset, plugin.GetPresetName(preset), change, kMinChainChange);
      return false;
    }
  }

  std::printf("PASS %d preloaded FX chains alter the rendered audio "
              "(weakest %s at %.1f%% of signal RMS, floor %.1f%%, render drift %.2f%%)\n",
              kNumPresets, plugin.GetPresetName(worstPreset), worstChange * 100.,
              kMinChainChange * 100., worstDrift * 100.);
  return true;
}

// ---------------------------------------------------------------- test 3

/** Drives every parameter away from its default and every drum slot to its own value,
 * saves through SerializeState, and reloads into an instance that has never seen the
 * state. Covers all kNumParams host parameters plus the 16 x 8 drum slot records that
 * live in the chunk alongside them. */
bool CheckProbeStateRoundTrips(Keith& source)
{
  if (!source.RestorePreset(0))
  {
    std::fprintf(stderr, "RestorePreset(0) failed before the round-trip probe\n");
    return false;
  }

  for (int param = 0; param < kNumParams; ++param)
  {
    if (param == kParamDrumSlotSelect || IsSelectedSlotParam(param))
      continue;
    SetThroughHostPath(source, param, FurthestFromDefault(source.GetParam(param)));
  }

  // Each slot gets its own record, so a restore that reloads one slot over all 16 fails.
  for (int slot = 1; slot <= keith::drums::kNumSlots; ++slot)
  {
    SelectDrumSlot(source, slot);
    for (int param = kParamDrumSlotLevel; param <= kParamDrumSlotSolo; ++param)
    {
      if (slot == kProbeDrumSlot)
      {
        SetThroughHostPath(source, param, FurthestFromDefault(source.GetParam(param)));
        continue;
      }
      const double spread = 0.06 + 0.055 * static_cast<double>(slot - 1);
      source.GetParam(param)->SetNormalized(spread);
      source.OnParamChange(param);
    }
  }
  // Leave the host parameters pointing at the slot driven to its extremes.
  SelectDrumSlot(source, kProbeDrumSlot);

  int nonDefault = 0;
  for (int param = 0; param < kNumParams; ++param)
  {
    if (std::fabs(source.GetParam(param)->Value() - source.GetParam(param)->GetDefault()) > 1e-9)
      ++nonDefault;
  }
  if (nonDefault != kNumParams)
  {
    std::fprintf(stderr, "probe state only moved %d of %d parameters off their defaults\n",
                 nonDefault, kNumParams);
    return false;
  }

  const std::vector<double> expectedParams = CaptureParams(source);
  const std::vector<double> expectedKit = CaptureDrumKit(source);

  iplug::IByteChunk chunk;
  if (!source.SerializeState(chunk) || chunk.Size() <= 0)
  {
    std::fprintf(stderr, "SerializeState refused the probe state\n");
    return false;
  }

  iplug::InstanceInfo info {};
  const std::unique_ptr<Keith> target(new Keith(info));
  const int consumed = target->UnserializeState(chunk, 0);
  if (consumed != chunk.Size())
  {
    std::fprintf(stderr, "UnserializeState consumed %d of %d chunk bytes\n", consumed,
                 chunk.Size());
    return false;
  }
  target->OnRestoreState();

  const std::vector<double> restoredParams = CaptureParams(*target);
  for (int param = 0; param < kNumParams; ++param)
  {
    const size_t index = static_cast<size_t>(param);
    if (expectedParams[index] != restoredParams[index])
    {
      std::fprintf(stderr, "parameter %d (%s) saved %.17g but restored %.17g\n", param,
                   source.GetParam(param)->GetName(), expectedParams[index],
                   restoredParams[index]);
      return false;
    }
  }

  const std::vector<double> restoredKit = CaptureDrumKit(*target);
  if (restoredKit.size() != expectedKit.size())
  {
    std::fprintf(stderr, "drum kit readback changed size across the round-trip\n");
    return false;
  }
  for (size_t i = 0; i < expectedKit.size(); ++i)
  {
    if (expectedKit[i] != restoredKit[i])
    {
      const int slot = static_cast<int>(i / 8) + 1;
      const int param = kParamDrumSlotLevel + static_cast<int>(i % 8);
      std::fprintf(stderr, "drum slot %d parameter %d saved %.17g but restored %.17g\n", slot,
                   param, expectedKit[i], restoredKit[i]);
      return false;
    }
  }

  std::printf("PASS probe state round-trips %d parameters and %d drum slot values "
              "exactly through SerializeState/UnserializeState (%d byte chunk)\n",
              kNumParams, static_cast<int>(expectedKit.size()), chunk.Size());
  return true;
}

/** Saves each factory preset out of one instance and loads it into a second instance
 * that is holding a different preset, so the restore has to overwrite real state
 * rather than agree with whatever happened to be there. */
bool CheckFactoryPresetsRoundTrip(Keith& source, Keith& target)
{
  for (int preset = 0; preset < kNumPresets; ++preset)
  {
    if (!target.RestorePreset((preset + 1) % kNumPresets) || !source.RestorePreset(preset))
    {
      std::fprintf(stderr, "RestorePreset failed while round-tripping preset %d\n", preset);
      return false;
    }

    const std::vector<double> expected = CaptureParams(source);

    iplug::IByteChunk chunk;
    if (!source.SerializeState(chunk))
    {
      std::fprintf(stderr, "SerializeState failed for preset %d (%s)\n", preset,
                   source.GetPresetName(preset));
      return false;
    }
    if (target.UnserializeState(chunk, 0) != chunk.Size())
    {
      std::fprintf(stderr, "UnserializeState did not consume the chunk for preset %d (%s)\n",
                   preset, source.GetPresetName(preset));
      return false;
    }
    target.OnRestoreState();

    const std::vector<double> restored = CaptureParams(target);
    for (int param = 0; param < kNumParams; ++param)
    {
      const size_t index = static_cast<size_t>(param);
      if (expected[index] != restored[index])
      {
        std::fprintf(stderr,
                     "preset %d (%s) parameter %d (%s) saved %.17g but restored %.17g\n", preset,
                     source.GetPresetName(preset), param, source.GetParam(param)->GetName(),
                     expected[index], restored[index]);
        return false;
      }
    }
  }

  std::printf("PASS %d factory presets round-trip all %d parameters into a second instance\n",
              kNumPresets, kNumParams);
  return true;
}

} // namespace

int main()
{
  iplug::InstanceInfo info {};
  const std::unique_ptr<Keith> plugin(new Keith(info));
  const std::unique_ptr<Keith> second(new Keith(info));

  if (!CheckPresetsRecallDistinctly(*plugin))
    return 1;
  if (!CheckPreloadedChainsAreInThePath(*plugin))
    return 1;
  if (!CheckFactoryPresetsRoundTrip(*plugin, *second))
    return 1;
  if (!CheckProbeStateRoundTrips(*plugin))
    return 1;

  return 0;
}
