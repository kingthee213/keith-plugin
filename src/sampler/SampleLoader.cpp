// Audio file decoding for the sampler: the implementation.
//
// This is the ONLY translation unit that defines the dr_libs implementation macros. Defining
// them anywhere else produces duplicate symbols at link time -- which is what broke the first
// attempt at this, because the macros lived in a header that several units included. The
// declarations are in SampleLoader.h; dr_libs itself is compiled here and nowhere else.

#define DR_WAV_IMPLEMENTATION
#define DR_FLAC_IMPLEMENTATION
#define DR_MP3_IMPLEMENTATION

// dr_libs emits unused-parameter and sign-compare noise in strict builds. It is third-party and
// not ours to fix, so the warnings are suppressed around it rather than patched.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wsign-compare"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include "../../third_party/dr_libs/dr_wav.h"
#include "../../third_party/dr_libs/dr_flac.h"
#include "../../third_party/dr_libs/dr_mp3.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "SampleLoader.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>

namespace keith {
namespace sampler {

namespace detail {

/** Appended into one file at a time, deinterleaving into per-channel vectors as it goes. */
struct DecodeTarget
{
  SampleData data;
  std::size_t bytes = 0;

  void Prepare(int channelCount, int frameCount, double rate, const char* name)
  {
    const int ch = channelCount > 0 ? channelCount : 1;
    data.channels.assign(static_cast<std::size_t>(ch), std::vector<float>());
    for (auto& c : data.channels)
      c.reserve(static_cast<std::size_t>(frameCount));
    data.sampleRate = rate > 0. ? rate : 44100.;
    data.name = name ? name : "";
  }

  void Append(const float* interleaved, unsigned int frames, unsigned int channels)
  {
    if (frames == 0 || channels == 0)
      return;
    for (unsigned int f = 0; f < frames; ++f)
      for (unsigned int c = 0; c < channels; ++c)
        data.channels[c].push_back(interleaved[static_cast<std::size_t>(f) * channels + c]);
    bytes += static_cast<std::size_t>(frames) * channels * sizeof(float);
  }

  void Finish()
  {
    data.frames = static_cast<int>(data.channels.empty() ? 0 : data.channels[0].size());
  }
};

// --- per-format decoders -----------------------------------------------------------------

LoadError DecodeWav(const std::string& path, DecodeTarget& out)
{
  drwav wav;
  if (!drwav_init_file(&wav, path.c_str(), nullptr))
    return LoadError::Unreadable;

  const std::size_t total = static_cast<std::size_t>(wav.totalPCMFrameCount);
  if (total == 0)
  {
    drwav_uninit(&wav);
    return LoadError::Empty;
  }
  if (total * wav.channels * sizeof(float) > kMaxDecodedBytes)
  {
    drwav_uninit(&wav);
    return LoadError::TooLarge;
  }

  out.Prepare(static_cast<int>(wav.channels), static_cast<int>(total),
              static_cast<double>(wav.sampleRate), path.c_str());

  // Decode in blocks rather than one giant buffer -- a long stereo file would otherwise need a
  // second full-size allocation just for the interleaved staging copy.
  constexpr drwav_uint64 kBlockFrames = 4096;
  std::vector<float> block(static_cast<std::size_t>(kBlockFrames) * wav.channels);
  drwav_uint64 done = 0;
  while (done < total)
  {
    const drwav_uint64 want = std::min<drwav_uint64>(kBlockFrames, total - done);
    const drwav_uint64 got = drwav_read_pcm_frames_f32(&wav, want, block.data());
    if (got == 0)
      break;
    out.Append(block.data(), static_cast<unsigned int>(got), wav.channels);
    done += got;
  }

  drwav_uninit(&wav);
  out.Finish();
  return out.data.Empty() ? LoadError::Empty : LoadError::None;
}

LoadError DecodeFlac(const std::string& path, DecodeTarget& out)
{
  unsigned int channels = 0, rate = 0;
  drflac_uint64 total = 0;
  float* decoded = drflac_open_file_and_read_pcm_frames_f32(path.c_str(), &channels, &rate,
                                                            &total, nullptr);
  if (!decoded)
    return LoadError::Unreadable;
  if (total > 0 && total * channels * sizeof(float) > kMaxDecodedBytes)
  {
    drflac_free(decoded, nullptr);
    return LoadError::TooLarge;
  }
  if (total == 0)
  {
    drflac_free(decoded, nullptr);
    return LoadError::Empty;
  }

  out.Prepare(static_cast<int>(channels), static_cast<int>(total), static_cast<double>(rate),
              path.c_str());
  out.Append(decoded, static_cast<unsigned int>(total), channels);
  drflac_free(decoded, nullptr);
  out.Finish();
  return LoadError::None;
}

LoadError DecodeMp3(const std::string& path, DecodeTarget& out)
{
  drmp3_config config {};
  drmp3_uint64 total = 0;
  float* decoded = drmp3_open_file_and_read_pcm_frames_f32(path.c_str(), &config, &total, nullptr);
  if (!decoded)
    return LoadError::Unreadable;
  if (total > 0 && total * config.channels * sizeof(float) > kMaxDecodedBytes)
  {
    drmp3_free(decoded, nullptr);
    return LoadError::TooLarge;
  }
  if (total == 0)
  {
    drmp3_free(decoded, nullptr);
    return LoadError::Empty;
  }

  out.Prepare(static_cast<int>(config.channels), static_cast<int>(total),
              static_cast<double>(config.sampleRate), path.c_str());
  out.Append(decoded, static_cast<unsigned int>(total), config.channels);
  drmp3_free(decoded, nullptr);
  out.Finish();
  return LoadError::None;
}

/**
 * AIFF.
 *
 * dr_libs covers WAV, FLAC and MP3 but not AIFF, so this is parsed here. AIFF is a simple
 * big-endian container and only COMM and SSND matter for our purposes: COMM gives channels,
 * frame count, bit depth and rate, SSND holds the PCM. Compressed AIFF-C variants are refused
 * rather than mis-read -- a wrong interpretation of the samples is worse than a clear failure.
 */
LoadError DecodeAiff(const std::string& path, DecodeTarget& out)
{
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f)
    return LoadError::FileNotFound;

  auto readBE32 = [](const unsigned char* p) -> uint32_t {
    return (static_cast<uint32_t>(p[0]) << 24) | (static_cast<uint32_t>(p[1]) << 16)
         | (static_cast<uint32_t>(p[2]) << 8)  |  static_cast<uint32_t>(p[3]);
  };
  auto readBE16 = [](const unsigned char* p) -> uint16_t {
    return static_cast<uint16_t>((static_cast<uint16_t>(p[0]) << 8) | p[1]);
  };

  unsigned char hdr[12];
  if (std::fread(hdr, 1, 12, f) != 12 || std::memcmp(hdr, "FORM", 4) != 0
      || (std::memcmp(hdr + 8, "AIFF", 4) != 0 && std::memcmp(hdr + 8, "AIFC", 4) != 0))
  {
    std::fclose(f);
    return LoadError::UnsupportedFormat;
  }
  if (std::memcmp(hdr + 8, "AIFC", 4) == 0)
  {
    std::fclose(f);
    return LoadError::UnsupportedFormat;
  }

  int channels = 0, bits = 0, frames = 0;
  double rate = 0.;
  bool haveComm = false, haveSsnd = false;
  long ssndOffset = 0;

  unsigned char chunk[8];
  while (std::fread(chunk, 1, 8, f) == 8)
  {
    const uint32_t size = readBE32(chunk + 4);
    if (std::memcmp(chunk, "COMM", 4) == 0 && size >= 18)
    {
      unsigned char comm[18];
      if (std::fread(comm, 1, 18, f) != 18)
        break;
      channels = readBE16(comm);
      frames = static_cast<int>(readBE32(comm + 2));
      bits = readBE16(comm + 6);
      // 80-bit IEEE 754 extended, as AIFF's sample rate. Rebuilt by hand: the exponent is
      // biased by 16383 and the mantissa's leading bit is explicit.
      const int exp = (static_cast<int>(readBE16(comm + 8)) & 0x7FFF) - 16383;
      const uint32_t hi = readBE32(comm + 10), lo = readBE32(comm + 14);
      const double mant = static_cast<double>(hi) * 4294967296.0 + static_cast<double>(lo);
      rate = mant * std::pow(2.0, exp - 63);
      haveComm = true;
      if (size > 18)
        std::fseek(f, static_cast<long>(size - 18), SEEK_CUR);
    }
    else if (std::memcmp(chunk, "SSND", 4) == 0 && size >= 8)
    {
      unsigned char ssnd[8];
      if (std::fread(ssnd, 1, 8, f) != 8)
        break;
      const uint32_t offset = readBE32(ssnd);
      ssndOffset = std::ftell(f) + static_cast<long>(offset);
      haveSsnd = true;
      break;  // SSND is the audio; nothing after it matters.
    }
    else
    {
      // Chunk sizes are even-padded; round up before seeking.
      const uint32_t skip = (size + 1u) & ~1u;
      if (std::fseek(f, static_cast<long>(skip), SEEK_CUR) != 0)
        break;
    }
  }

  if (!haveComm || !haveSsnd || channels <= 0 || frames <= 0)
  {
    std::fclose(f);
    return haveComm ? LoadError::Empty : LoadError::UnsupportedFormat;
  }
  if (bits != 8 && bits != 16 && bits != 24 && bits != 32)
  {
    std::fclose(f);
    return LoadError::UnsupportedFormat;
  }
  if (static_cast<std::size_t>(frames) * channels * sizeof(float) > kMaxDecodedBytes)
  {
    std::fclose(f);
    return LoadError::TooLarge;
  }

  out.Prepare(channels, frames, rate, path.c_str());
  if (std::fseek(f, ssndOffset, SEEK_SET) != 0)
  {
    std::fclose(f);
    return LoadError::Unreadable;
  }

  const int bytesPerSample = bits / 8;
  std::vector<unsigned char> raw(static_cast<std::size_t>(frames) * channels * bytesPerSample);
  const std::size_t want = raw.size();
  const std::size_t got = std::fread(raw.data(), 1, want, f);
  std::fclose(f);
  if (got < want)
    return LoadError::Unreadable;

  // AIFF PCM is signed big-endian two's complement.
  std::vector<float> block(static_cast<std::size_t>(frames) * channels);
  for (std::size_t i = 0; i < block.size(); ++i)
  {
    const unsigned char* smp = raw.data() + i * static_cast<std::size_t>(bytesPerSample);
    float v = 0.f;
    if (bits == 8)
      v = static_cast<float>(static_cast<int8_t>(smp[0])) / 128.f;
    else if (bits == 16)
      v = static_cast<float>(static_cast<int16_t>(readBE16(smp))) / 32768.f;
    else if (bits == 24)
    {
      int32_t x = (static_cast<int32_t>(smp[0]) << 16) | (static_cast<int32_t>(smp[1]) << 8)
                |  static_cast<int32_t>(smp[2]);
      if (x & 0x800000)
        x |= ~0xFFFFFF;  // sign-extend
      v = static_cast<float>(x) / 8388608.f;
    }
    else
      v = static_cast<float>(static_cast<int32_t>(readBE32(smp))) / 2147483648.f;
    block[i] = v;
  }
  out.Append(block.data(), static_cast<unsigned int>(frames),
             static_cast<unsigned int>(channels));
  out.Finish();
  return LoadError::None;
}

} // namespace detail

std::shared_ptr<const SampleData> LoadFile(const std::string& path, LoadError* error)
{
  auto fail = [error](LoadError e) -> std::shared_ptr<const SampleData> {
    if (error)
      *error = e;
    return nullptr;
  };

  const std::string ext = detail::ExtensionOf(path);
  detail::DecodeTarget target;
  LoadError err = LoadError::UnsupportedFormat;

  if (ext == "wav" || ext == "wave")
    err = detail::DecodeWav(path, target);
  else if (ext == "flac")
    err = detail::DecodeFlac(path, target);
  else if (ext == "mp3")
    err = detail::DecodeMp3(path, target);
  else if (ext == "aif" || ext == "aiff" || ext == "aifc")
    err = detail::DecodeAiff(path, target);
  else
    return fail(LoadError::UnsupportedFormat);

  if (err != LoadError::None)
    return fail(err);

  return std::make_shared<SampleData>(std::move(target.data));
}

} // namespace sampler
} // namespace keith
