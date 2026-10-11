#pragma once

#include "SamplerVoice.h"

// Audio file decoding for the sampler: declarations only.
//
// The dr_libs IMPLEMENTATIONS live in SampleLoader.cpp, not here. This header is included by
// SamplerEngine.h, which Keith.h includes, and both the plugin and the compile-check unit
// compile that chain -- so putting the implementations here would define every dr_libs symbol
// once per translation unit and the link would fail with hundreds of duplicate symbols. That is
// exactly what happened the first time this was wired up.
//
// Files in this header are header-only on purpose but none of them touch dr_libs, so they can
// be inlined freely.

#include <memory>
#include <string>

namespace keith {
namespace sampler {

namespace detail {

/** Case-insensitive extension of a path, without the dot. */
inline std::string ExtensionOf(const std::string& path)
{
  const std::size_t slash = path.find_last_of("/\\");
  const std::size_t dot = path.find_last_of('.');
  if (dot == std::string::npos || (slash != std::string::npos && dot < slash))
    return "";
  std::string ext = path.substr(dot + 1);
  for (char& c : ext)
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  return ext;
}

} // namespace detail

/**
 * Decode an audio file into an immutable SampleData.
 *
 * Defined in SampleLoader.cpp. Format is chosen by extension; a file whose extension lies about
 * its contents fails with Unreadable rather than being mis-parsed, which is the honest outcome.
 *
 * On success returns a shared_ptr to a SampleData that is never mutated afterwards, so the
 * audio thread can read it without a lock. On failure returns null and writes the reason to
 * `error` when provided.
 *
 * Message thread only: this reads from disk and allocates.
 */
std::shared_ptr<const SampleData> LoadFile(const std::string& path,
                                          LoadError* error = nullptr);

} // namespace sampler
} // namespace keith
