/**
 * Stop an exception from escaping a plugin callback into the host.
 *
 * ## Why this exists
 *
 * A crash report from Ableton Live showed:
 *
 *     libc++abi  std::__terminate()
 *     libc++abi  std::terminate()
 *     abort()
 *     ...under AppKit's _routeMouseUpEvent
 *
 * That is an uncaught C++ exception. When one escapes a plugin's callback, the C++ runtime calls
 * terminate() and the HOST dies -- the whole session, not just the plugin. A plugin that throws
 * across the host boundary takes down a user's work, and it must not.
 *
 * Every entry point the host can call from outside this code -- OnParamChange, OnRestoreState,
 * OnReset, OnIdle, ProcessBlock, ProcessMidiMsg -- goes through Guard().
 *
 * ## What it does NOT do
 *
 * It does not pretend the failure did not happen. The exception is logged with DBGMSG, so a
 * development build still shows it, and the plugin continues in a degraded state rather than
 * killing the host. Silently swallowing errors would be worse than crashing: a crash is visible,
 * a swallowed error leaves the plugin in a state nobody can explain later.
 *
 * ## Re-entrancy
 *
 * The audio thread must not log, and must not throw either. GuardAudio() reports once per session
 * rather than per block, because ProcessBlock runs thousands of times a second and a log call per
 * block would itself be a failure.
 */

#pragma once

#include <exception>

#include "IPlug_include_in_plug_src.h"

namespace keith {
namespace detail {

/**
 * Run a host callback, absorbing anything it throws.
 *
 * Returns true when the body completed. Callers that return a value should use GuardOr with a
 * fallback, or check the result.
 */
template <typename Fn>
inline bool Guard(const char* what, Fn&& fn) noexcept
{
  try
  {
    fn();
    return true;
  }
  catch (const std::exception& e)
  {
    // DBGMSG is a debug-only write; in release it compiles out, so this costs a catch clause and
    // nothing else on the hot path.
    DBGMSG("Keith: %s threw: %s\n", what, e.what());
    return false;
  }
  catch (...)
  {
    DBGMSG("Keith: %s threw (unknown)\n", what);
    return false;
  }
}

/** Guard for a callback that must return a value: the fallback is returned on a throw. */
template <typename R, typename Fn>
inline R GuardOr(const char* what, R fallback, Fn&& fn) noexcept
{
  try
  {
    return fn();
  }
  catch (const std::exception& e)
  {
    DBGMSG("Keith: %s threw: %s\n", what, e.what());
    return fallback;
  }
  catch (...)
  {
    DBGMSG("Keith: %s threw (unknown)\n", what);
    return fallback;
  }
}

/**
 * Guard for the audio thread.
 *
 * Reports at most once per session. A log call per block would be its own problem, and the
 * condition that made the audio path throw is not going to fix itself between one block and the
 * next -- the useful signal is "this happened", not how many times.
 */
inline bool NoteAudioFailureOnce(const char* what) noexcept
{
  static bool reported = false;
  if (!reported)
  {
    reported = true;
    DBGMSG("Keith: audio path failed in %s (further reports suppressed)\n", what);
    return true;
  }
  return false;
}

}  // namespace detail
}  // namespace keith
