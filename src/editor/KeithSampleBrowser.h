/**
 * The sample browser: a scrollable list of the samples that ship with the plugin.
 *
 * ## Why it reads the folder rather than a hardcoded list
 *
 * The drum kits and guitar banks are already `<slot>_<desc>.wav` and `<note>.flac` on disk. A
 * hardcoded list of names would go stale the moment a sample is added and would fail SILENTLY --
 * the file would be there and simply never appear, which is the worst way for this to break. The
 * list is built by reading the directory.
 *
 * ## What it lists
 *
 * The plugin's own shipped content: the 10 drum kits, 2 percussion kits, 5 guitar banks and the
 * vocal choir. Not an arbitrary file browser -- there is no cross-platform file dialog in iPlug2,
 * and a path field would mean typing paths, which is not what "pick a sample" should feel like.
 *
 * An external file can still be loaded by dragging it onto the drop zone; this list is for the
 * content that is already there.
 */

#pragma once

#include <algorithm>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "KeithPage.h"
#include <system_error>

namespace keith {
namespace ui {

/** One row: a sample, and where it came from. */
struct BrowserEntry
{
  std::string label;   // what the user reads
  std::string group;   // "drums", "guitars", ...
  std::string path;    // absolute, for loading
  bool isKit = false;  // a whole kit rather than a single file
};

/**
 * Build the list by walking the sample folder.
 *
 * Grouped in a fixed order rather than alphabetically, because the groups are not equivalent: the
 * kits are what a drum patch loads, the guitars are what a guitar patch loads.
 */
inline std::vector<BrowserEntry> BuildBrowserEntries(const std::string& root)
{
  std::vector<BrowserEntry> out;
  namespace fs = std::filesystem;

  if (root.empty())
    return out;

  const fs::path base(root);
  std::error_code ec;
  if (!fs::exists(base, ec))
    return out;

  // A group is a directory of either kits (directories of wavs) or banks (a directory of flacs).
  // Ordered so the drum kits come first, then percussion, then guitars, then the choir.
  static const char* const kGroupOrder[] = {"drums", "percussion", "guitars", "vocal"};

  for (const char* group : kGroupOrder)
  {
    const fs::path gdir = base / group;
    if (!fs::is_directory(gdir, ec))
      continue;

    std::vector<std::string> subs;
    for (const auto& e : fs::directory_iterator(gdir, ec))
    {
      if (e.is_directory())
        subs.push_back(e.path().filename().string());
    }
    std::sort(subs.begin(), subs.end());

    for (const std::string& sub : subs)
    {
      BrowserEntry entry;
      entry.group = group;
      entry.label = sub;
      entry.path = (gdir / sub).string();
      // A kit is a folder of one-shots; a bank is a folder of pitched multisamples. Either way the
      // loader takes the DIRECTORY, so a row points at the folder rather than a file.
      entry.isKit = true;
      out.push_back(entry);
    }
  }
  return out;
}

/**
 * The list.
 *
 * Scrolls rather than paginating: a wheel or a drag scrolls it, and the visible window is a slice
 * of the full list. Clicking a row reports the entry; the page decides what to do with it.
 */
class KeithSampleBrowser : public KeithPageElement
{
public:
  KeithSampleBrowser(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  void SetEntries(std::vector<BrowserEntry> entries)
  {
    mEntries = std::move(entries);
    mSelected = -1;
    mScroll = 0;
  }

  void SetOnPick(std::function<void(const BrowserEntry&)> fn) { mOnPick = std::move(fn); }

  int Selected() const { return mSelected; }

  void Draw(IGraphics& g) override;
  void OnMouseDown(float x, float y, const IMouseMod&) override;
  void OnMouseWheel(float x, float y, const IMouseMod&, float d) override;

private:
  static constexpr float kRowH = 16.f;
  static constexpr float kPad = 3.f;

  int RowsVisible() const;
  int RowAt(float y) const;
  void ClampScroll();

  std::vector<BrowserEntry> mEntries;
  int mSelected = -1;
  int mScroll = 0;
  std::function<void(const BrowserEntry&)> mOnPick;
};

}  // namespace ui
}  // namespace keith
