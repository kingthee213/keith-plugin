#include "Keith.h"

#include <cmath>
#include <cstdio>
#include <memory>
#include <vector>

// Constructs the plugin and recalls every baked preset through RestorePreset,
// the same path the host uses. Consecutive parameter states must differ.
// The plugin object is large, so it lives on the heap the way a host allocates it.
int main()
{
  iplug::InstanceInfo info{};
  const std::unique_ptr<Keith> plugin(new Keith(info));

  if (plugin->NPresets() != kNumPresets)
  {
    std::fprintf(stderr, "expected %d presets, plugin has %d\n", kNumPresets, plugin->NPresets());
    return 1;
  }

  std::vector<std::vector<double>> states;
  states.reserve(static_cast<size_t>(kNumPresets));

  for (int preset = 0; preset < plugin->NPresets(); ++preset)
  {
    if (!plugin->RestorePreset(preset) || plugin->GetCurrentPresetIdx() != preset)
    {
      std::fprintf(stderr, "RestorePreset failed for index %d (%s)\n", preset,
                   plugin->GetPresetName(preset));
      return 1;
    }

    std::vector<double> state(static_cast<size_t>(kNumParams));
    for (int param = 0; param < kNumParams; ++param)
      state[static_cast<size_t>(param)] = plugin->GetParam(param)->Value();
    for (int param = kParamMod1Source; param <= kParamMod4Amount; ++param)
    {
      if (state[static_cast<size_t>(param)] != 0.)
      {
        std::fprintf(stderr, "preset %d (%s) has active modulation-matrix default at parameter %d\n",
                     preset, plugin->GetPresetName(preset), param);
        return 1;
      }
    }
    states.push_back(std::move(state));
  }

  for (int preset = 1; preset < kNumPresets; ++preset)
  {
    bool differs = false;
    for (int param = 0; param < kNumParams; ++param)
    {
      if (std::fabs(states[static_cast<size_t>(preset)][static_cast<size_t>(param)] -
                    states[static_cast<size_t>(preset - 1)][static_cast<size_t>(param)]) > 1e-6)
      {
        differs = true;
        break;
      }
    }
    if (!differs)
    {
      std::fprintf(stderr, "preset %d (%s) matches the previous preset after RestorePreset\n",
                   preset, plugin->GetPresetName(preset));
      return 1;
    }
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
                     left, plugin->GetPresetName(left), right, plugin->GetPresetName(right));
        return 1;
      }
    }
  }

  std::printf("PASS %d presets restored through RestorePreset\n", kNumPresets);
  return 0;
}
