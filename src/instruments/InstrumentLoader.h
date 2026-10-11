/**
 * Load a multisampled instrument bank from disk.
 *
 * A bank is a directory of `<midi note>.flac` (or .wav), one file per recorded note. The FILENAME
 * is the root note, which is what makes this self-describing: the builder writes the note into the
 * name, so nothing else has to agree and a dropped-in file plays.
 *
 * ## Why the filename carries the pitch
 *
 * The alternative is a manifest or an SFZ parse. A manifest is a second thing to keep in step, and
 * an SFZ parser is a large amount of surface for the benefit of reading a number that the builder
 * already knows. Reading `061.flac` as "MIDI 61, recorded at 61" is the whole interface.
 *
 * ## Ranges
 *
 * The zone ranges are derived from the notes present -- see AssignRanges in InstrumentEngine.h.
 * A sparse bank covers wide stretches by transposition, which is a real quality limit rather than
 * a bug: the nylon bank has 48 notes and stays close to pitch, the electric banks have 19 and
 * stretch further.
 */

#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

#include "instruments/InstrumentEngine.h"
#include <algorithm>

namespace keith {
namespace instruments {

struct BankLoadReport
{
  std::string bank;
  int zonesLoaded = 0;
  std::vector<std::string> failed;
  int lowNote = 0;
  int highNote = 0;
  std::string error;

  bool Ok() const { return error.empty() && zonesLoaded > 0; }
};

/** The three-letter root of a filename, parsed as a MIDI note. -1 when it is not a note. */
inline int NoteFromFilename(const std::string& stem)
{
  // Accepts `061` and `61`; rejects anything with a non-digit.
  if (stem.empty() || stem.size() > 3)
    return -1;
  for (char c : stem)
    if (c < '0' || c > '9')
      return -1;
  const int note = std::stoi(stem);
  return (note >= 0 && note <= 127) ? note : -1;
}

inline std::shared_ptr<const BankData> LoadBank(const std::string& dir, BankLoadReport& report)
{
  report = BankLoadReport();
  report.bank = dir;

  auto bank = std::make_shared<BankData>();
  const std::size_t slash = dir.find_last_of("/\\");
  bank->name = (slash == std::string::npos) ? dir : dir.substr(slash + 1);

  std::error_code ec;
  if (!std::filesystem::is_directory(dir, ec))
  {
    report.error = "not a directory: " + dir;
    return bank;
  }

  std::vector<Zone> zones;
  for (const auto& entry : std::filesystem::directory_iterator(dir, ec))
  {
    if (!entry.is_regular_file())
      continue;
    const std::string ext = entry.path().extension().string();
    if (ext != ".flac" && ext != ".FLAC" && ext != ".wav" && ext != ".WAV")
      continue;

    const int note = NoteFromFilename(entry.path().stem().string());
    if (note < 0)
      continue;

    std::shared_ptr<const sampler::SampleData> data;
    try
    {
      data = sampler::LoadFile(entry.path().string(), nullptr);
    }
    catch (...)
    {
      report.failed.push_back(entry.path().filename().string());
      continue;
    }
    if (!data || data->Empty())
    {
      report.failed.push_back(entry.path().filename().string());
      continue;
    }

    Zone zone;
    zone.sample = std::move(data);
    zone.rootNote = note;
    zones.push_back(std::move(zone));
  }

  if (zones.empty())
  {
    report.error = "no note samples in " + dir;
    return bank;
  }

  // Sorted by root note, because AssignRanges walks neighbours and depends on the order.
  std::sort(zones.begin(), zones.end(),
            [](const Zone& a, const Zone& b) { return a.rootNote < b.rootNote; });

  // A bank with duplicate root notes would make AssignRanges produce a reversed range for one of
  // them. Keeping the first and reporting the rest is better than a silent overlap.
  for (std::size_t i = zones.size(); i-- > 1;)
  {
    if (zones[i].rootNote == zones[i - 1].rootNote)
    {
      report.failed.push_back("duplicate root note " + std::to_string(zones[i].rootNote));
      zones.erase(zones.begin() + static_cast<std::ptrdiff_t>(i));
    }
  }

  AssignRanges(zones);

  bank->zones = std::move(zones);
  report.zonesLoaded = bank->LoadedZones();
  report.lowNote = bank->LowNote();
  report.highNote = bank->HighNote();
  return bank;
}

} // namespace instruments
} // namespace keith
