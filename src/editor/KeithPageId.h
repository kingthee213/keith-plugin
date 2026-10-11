#pragma once

// Which editor page is showing.
//
// This enum lives in its own header, with no iPlug2 dependency, for one reason: Keith.h needs it
// for the page state member, and Keith.h is parsed before iPlug2's graphics headers. Putting the
// enum in the same header as the button control made Keith.h pull in IColor and IControl, which
// failed on macOS with "unknown type name 'IColor'".
//
// Splitting it means the plugin can include this, the button can include this, and neither has
// to care about what the other needs.

namespace keith {
namespace ui {

enum class KeithPage : int
{
  /** The normal panel view. */
  None = 0,
  Sampler,
  Wavetable
};

} // namespace ui
} // namespace keith
