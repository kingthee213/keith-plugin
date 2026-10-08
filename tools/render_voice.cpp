// Renders one OfflineEngine note to a stereo float file for measurement.
// Not part of the plugin. The recreation gate renders through the plugin itself.
#include "../src/synth/CoreSynth.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using keith::synth::EngineMode;
using keith::synth::FilterType;
using keith::synth::OfflineEngine;
using keith::synth::Params;

static bool SetField(Params& p, const std::string& key, float value)
{
  if (key == "engine") p.engine = value >= 0.5f ? EngineMode::Analog : EngineMode::Hybrid;
  else if (key == "osc1Shape") p.osc1Shape = value;
  else if (key == "osc1Pitch") p.osc1Pitch = value;
  else if (key == "osc1Level") p.osc1Level = value;
  else if (key == "osc1PW") p.osc1PW = value;
  else if (key == "osc2Shape") p.osc2Shape = value;
  else if (key == "osc2Pitch") p.osc2Pitch = value;
  else if (key == "osc2Level") p.osc2Level = value;
  else if (key == "osc2PW") p.osc2PW = value;
  else if (key == "subLevel") p.subLevel = value;
  else if (key == "noiseLevel") p.noiseLevel = value;
  else if (key == "fmAmount") p.fmAmount = value;
  else if (key == "filterType") p.filterType = static_cast<FilterType>(static_cast<int>(value));
  else if (key == "filterCutoffHz") p.filterCutoffHz = value;
  else if (key == "filterReso") p.filterReso = value;
  else if (key == "filterEnvAmt") p.filterEnvAmt = value;
  else if (key == "filterKeyTrack") p.filterKeyTrack = value;
  else if (key == "filterKeyCurve") p.filterKeyCurve = value;
  else if (key == "filterDrive") p.filterDrive = value;
  else if (key == "ampAttackMs") p.ampAttackMs = value;
  else if (key == "ampDecayMs") p.ampDecayMs = value;
  else if (key == "ampSustain") p.ampSustain = value;
  else if (key == "ampReleaseMs") p.ampReleaseMs = value;
  else if (key == "masterLevel") p.masterLevel = value;
  else if (key == "noiseKey") p.noiseKey = value;
  else if (key == "ampAttackKey") p.ampAttackKey = value;
  else if (key == "ampAttackCurve") p.ampAttackCurve = value;
  else if (key == "ampDecayKey") p.ampDecayKey = value;
  else if (key == "ampDecayCurve") p.ampDecayCurve = value;
  else if (key == "stereoDelayMs") p.stereoDelayMs = value;
  else if (key == "stereoDelayKey") p.stereoDelayKey = value;
  else if (key == "stereoSpread") p.stereoSpread = value;
  else if (key == "directLevel") p.directLevel = value;
  else if (key == "unisonVoices") p.unisonVoices = static_cast<int>(value);
  else if (key == "unisonDetune") p.unisonDetune = value;
  else return false;
  return true;
}

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::fprintf(stderr, "usage: render_voice <params.txt> <out.f32>\n");
    return 2;
  }

  std::ifstream in(argv[1]);
  if (!in)
  {
    std::fprintf(stderr, "cannot read %s\n", argv[1]);
    return 2;
  }

  int sr = 48000;
  int note = 60;
  float seconds = 4.f;
  float velocity = 0.8f;
  Params params {};
  params.osc2Level = 0.f;
  params.filterEnvAmt = 0.f;
  params.ampSustain = 0.f;
  params.masterLevel = 0.4f;

  std::string line;
  while (std::getline(in, line))
  {
    if (line.empty() || line[0] == '#')
      continue;
    std::istringstream row(line);
    std::string key;
    float value = 0.f;
    row >> key >> value;
    if (key == "sr") sr = static_cast<int>(value);
    else if (key == "note") note = static_cast<int>(value);
    else if (key == "seconds") seconds = value;
    else if (key == "velocity") velocity = value;
    else if (!SetField(params, key, value))
    {
      std::fprintf(stderr, "unknown field %s\n", key.c_str());
      return 2;
    }
  }

  OfflineEngine eng;
  eng.Reset(sr);
  eng.SetParams(params);
  eng.NoteOn(note, velocity);
  const int frames = std::max(1, static_cast<int>(seconds * static_cast<float>(sr)));
  std::vector<float> interleaved(static_cast<size_t>(frames) * 2);
  for (int i = 0; i < frames; ++i)
  {
    float left = 0.f, right = 0.f;
    eng.ProcessStereo(left, right);
    interleaved[static_cast<size_t>(i) * 2] = left;
    interleaved[static_cast<size_t>(i) * 2 + 1] = right;
  }

  std::ofstream out(argv[2], std::ios::binary);
  const int32_t header[2] = {sr, frames};
  out.write(reinterpret_cast<const char*>(header), sizeof(header));
  out.write(reinterpret_cast<const char*>(interleaved.data()),
            static_cast<std::streamsize>(interleaved.size() * sizeof(float)));
  return out ? 0 : 1;
}
