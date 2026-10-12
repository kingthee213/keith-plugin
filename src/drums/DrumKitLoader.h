#pragma once

/**
 * Load a drum kit from the samples on disk.
 *
 * A kit is a directory of WAVs named `<slot>_<description>.wav`, slot 00..15. The slot number is
 * the MIDI note minus 36, so the filename carries the mapping and nothing else has to agree.
 *
 * ## Why the folder is the interface
 *
 * The alternative is a manifest listing every file and its slot. That is more explicit, but it is
 * a second place to keep in step with the audio, and a mismatch shows up as a kit with a silent
 * kick. Deriving the slot from the filename means a dropped-in file plays, and the loader's report
 * says what it could not place rather than failing silently.
 *
 * ## Embedded resources
 *
 * The kits ship inside the plugin bundle. `ResourceDir()` resolves to the bundle's Resources
 * directory, where CMake copies `resources/samples/`. The path is different in a standalone app,
 * a VST3 and an AU, which is why the caller passes the root in rather than this code guessing.
 */

#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <system_error>
#include <string>
#include <vector>

#include "drums/DrumKitEngine.h"

namespace keith {
namespace drums {

/** What a load produced, so the caller can report rather than assume. */
struct KitLoadReport
{
  std::string kit;
  int slotsLoaded = 0;
  std::vector<std::string> missing;   ///< slot names with no file
  std::vector<std::string> failed;    ///< files that would not decode
  std::string error;                  ///< empty on success

  bool Ok() const { return error.empty() && slotsLoaded > 0; }
};

/** Slot names in index order. These are what the builder writes and what this reads back. */
inline const char* const* SlotNames()
{
  static const char* names[kKitSlots] = {
    "kick", "snare", "clap", "rim", "hat_closed", "hat_open", "tom_lo", "tom_hi",
    "perc", "slot9", "slot10", "slot11", "slot12", "slot13", "slot14", "slot15",
  };
  return names;
}

/**
 * Slot 8 (perc) and above are one-shots like the rest of a drum kit. Sustained slots are for kits
 * that hold pads or risers, which this bank does not ship, so nothing is sustained by default and
 * the field stays settable for a kit that needs it.
 */
inline SlotMode ModeForSlot(int) { return SlotMode::OneShot; }

/**
 * Read every `NN_name.wav` in `dir` into a KitData.
 *
 * Not real-time safe and must not be called from the audio thread: it opens files and allocates.
 * The caller builds a whole KitData, then publishes it with DrumKitEngine::Publish, which is the
 * handover point.
 */
inline std::shared_ptr<const KitData> LoadKit(const std::string& dir, KitLoadReport& report)
{
  report = KitLoadReport();
  report.kit = dir;

  auto kit = std::make_shared<KitData>();
  const std::size_t slash = dir.find_last_of("/\\");
  kit->name = (slash == std::string::npos) ? dir : dir.substr(slash + 1);

  const char* const* names = SlotNames();

  for (int slot = 0; slot < kKitSlots; ++slot)
  {
    // Files are `NN_<description>.wav`; the leading number is the slot and the rest is
    // descriptive. So the slot comes from the filename prefix and the description is free text --
    // no manifest to keep in step, and a file renamed by hand still loads.
    //
    // The directory is read properly with std::filesystem rather than a list of guessed names. A
    // guessed list has to be updated whenever a kit gains a slot, and the failure is silent.
    char prefix[8];
    std::snprintf(prefix, sizeof(prefix), "%02d_", slot);
    const std::string prefixStr(prefix);

    std::string found;
    std::error_code ec;
    if (std::filesystem::is_directory(dir, ec))
    {
      for (const auto& entry : std::filesystem::directory_iterator(dir, ec))
      {
        if (!entry.is_regular_file())
          continue;
        const std::string filename = entry.path().filename().string();
        if (filename.size() > prefixStr.size() && filename.compare(0, prefixStr.size(), prefixStr) == 0)
        {
          const std::string ext = entry.path().extension().string();
          if (ext == ".wav" || ext == ".WAV" || ext == ".aif" || ext == ".aiff" || ext == ".flac")
          {
            found = entry.path().string();
            break;
          }
        }
      }
    }

    if (found.empty())
    {
      report.missing.push_back(names[slot]);
      continue;
    }

    std::string why;
    auto data = sampler::LoadFile(found, nullptr);
    if (!data || data->Empty())
    {
      report.failed.push_back(names[slot]);
      continue;
    }

    kit->slots[slot].sample = std::move(data);
    kit->slots[slot].mode = ModeForSlot(slot);
    kit->slots[slot].gain = 1.f;
    ++report.slotsLoaded;
  }

  if (report.slotsLoaded == 0)
    report.error = "no slots loaded from " + dir;

  return kit;
}

} // namespace drums
} // namespace keith
