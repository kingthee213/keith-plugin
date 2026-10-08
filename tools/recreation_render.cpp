// Renders one factory preset through the plugin, not OfflineEngine.
// The recreation gate measures the file with tools/measure_signal.py.
#include "Keith.h"

#include "pluginterfaces/vst/ivstaudioprocessor.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace {

constexpr double kRate = 48000.0;
constexpr int kBlock = 512;
// The output gain smoother settles during this silence. It is not part of the file.
constexpr int kWarmupBlocks = 24;

using iplug::sample;

void Prepare(Keith& plugin)
{
  Steinberg::Vst::ProcessSetup setup {};
  setup.processMode = Steinberg::Vst::kRealtime;
  setup.symbolicSampleSize = sizeof(sample) == sizeof(double) ? Steinberg::Vst::kSample64
                                                             : Steinberg::Vst::kSample32;
  setup.maxSamplesPerBlock = kBlock;
  setup.sampleRate = kRate;
  plugin.setupProcessing(setup);
}

void ProcessSilence(Keith& plugin, int blocks)
{
  std::vector<sample> left(static_cast<size_t>(kBlock));
  std::vector<sample> right(static_cast<size_t>(kBlock));
  sample* outputs[2] {left.data(), right.data()};
  for (int block = 0; block < blocks; ++block)
  {
    std::fill(left.begin(), left.end(), sample(0));
    std::fill(right.begin(), right.end(), sample(0));
    plugin.ProcessBlock(nullptr, outputs, kBlock);
  }
}

bool RenderNote(Keith& plugin, int note, double seconds, const std::string& path)
{
  ProcessSilence(plugin, kWarmupBlocks);

  iplug::IMidiMsg noteOn;
  noteOn.MakeNoteOnMsg(note, 102, 0);
  plugin.ProcessMidiMsg(noteOn);

  const int frames = std::max(kBlock, static_cast<int>(seconds * kRate));
  const int blocks = (frames + kBlock - 1) / kBlock;
  std::vector<float> interleaved(static_cast<size_t>(blocks * kBlock) * 2);

  std::vector<sample> blockLeft(static_cast<size_t>(kBlock));
  std::vector<sample> blockRight(static_cast<size_t>(kBlock));
  sample* outputs[2] {blockLeft.data(), blockRight.data()};

  for (int block = 0; block < blocks; ++block)
  {
    std::fill(blockLeft.begin(), blockLeft.end(), sample(0));
    std::fill(blockRight.begin(), blockRight.end(), sample(0));
    plugin.ProcessBlock(nullptr, outputs, kBlock);
    for (int i = 0; i < kBlock; ++i)
    {
      const size_t at = (static_cast<size_t>(block) * kBlock + static_cast<size_t>(i)) * 2;
      interleaved[at] = static_cast<float>(blockLeft[static_cast<size_t>(i)]);
      interleaved[at + 1] = static_cast<float>(blockRight[static_cast<size_t>(i)]);
    }
  }

  std::ofstream out(path, std::ios::binary);
  const int32_t header[2] = {static_cast<int32_t>(kRate), frames};
  out.write(reinterpret_cast<const char*>(header), sizeof(header));
  out.write(reinterpret_cast<const char*>(interleaved.data()),
            static_cast<std::streamsize>(static_cast<size_t>(frames) * 2 * sizeof(float)));
  if (!out)
  {
    std::fprintf(stderr, "could not write %s\n", path.c_str());
    return false;
  }
  return true;
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 5)
  {
    std::fprintf(stderr, "usage: recreation_render <preset name> <midi note> <seconds> <out.f32>\n");
    return 2;
  }

  const std::string name = argv[1];
  const int note = std::atoi(argv[2]);
  const double seconds = std::atof(argv[3]);
  if (note < 0 || note > 127 || seconds <= 0.0)
  {
    std::fprintf(stderr, "bad note or length\n");
    return 2;
  }

  iplug::InstanceInfo info {};
  const std::unique_ptr<Keith> plugin(new Keith(info));

  int preset = -1;
  for (int i = 0; i < plugin->NPresets(); ++i)
  {
    if (name == plugin->GetPresetName(i))
      preset = i;
  }
  if (preset < 0 || !plugin->RestorePreset(preset))
  {
    std::fprintf(stderr, "preset not found: %s\n", name.c_str());
    return 2;
  }

  Prepare(*plugin);
  if (!RenderNote(*plugin, note, seconds, argv[4]))
    return 1;
  return 0;
}
