// Offline tape/analog block verification — not linked into the plugin binary.
#include "../src/fx/TapeAnalog.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

static void WriteWav(const char* path, const std::vector<float>& samples, int sampleRate)
{
  FILE* f = std::fopen(path, "wb");
  if (!f)
    return;

  const int numSamples = static_cast<int>(samples.size());
  const int dataBytes = numSamples * static_cast<int>(sizeof(int16_t));
  const int16_t channels = 1;
  const int16_t bits = 16;
  const int32_t byteRate = sampleRate * channels * bits / 8;
  const int16_t blockAlign = channels * bits / 8;

  std::fwrite("RIFF", 1, 4, f);
  int32_t chunkSize = 36 + dataBytes;
  std::fwrite(&chunkSize, 4, 1, f);
  std::fwrite("WAVE", 1, 4, f);
  std::fwrite("fmt ", 1, 4, f);
  int32_t fmtSize = 16;
  std::fwrite(&fmtSize, 4, 1, f);
  int16_t audioFormat = 1;
  std::fwrite(&audioFormat, 2, 1, f);
  std::fwrite(&channels, 2, 1, f);
  std::fwrite(&sampleRate, 4, 1, f);
  std::fwrite(&byteRate, 4, 1, f);
  std::fwrite(&blockAlign, 2, 1, f);
  std::fwrite(&bits, 2, 1, f);
  std::fwrite("data", 1, 4, f);
  std::fwrite(&dataBytes, 4, 1, f);

  for (float s : samples)
  {
    float clamped = s;
    if (clamped > 1.f)
      clamped = 1.f;
    if (clamped < -1.f)
      clamped = -1.f;
    int16_t pcm = static_cast<int16_t>(clamped * 32767.f);
    std::fwrite(&pcm, 2, 1, f);
  }
  std::fclose(f);
}

static double Rms(const std::vector<float>& x)
{
  double sum = 0.;
  for (float v : x)
    sum += static_cast<double>(v) * static_cast<double>(v);
  return std::sqrt(sum / static_cast<double>(x.size()));
}

static void Render(keith::fx::TapeAnalog<float>& tape, const keith::fx::TapeAnalog<float>::Params& p,
                   std::vector<float>& out, int sampleRate, int nFrames)
{
  tape.Reset(sampleRate, nFrames);
  tape.SetParams(p);

  out.resize(static_cast<size_t>(nFrames));
  std::vector<float> buf(static_cast<size_t>(nFrames));
  float* chans[1] = {buf.data()};

  for (int i = 0; i < nFrames; ++i)
    buf[static_cast<size_t>(i)] = 0.25f * std::sin(2.f * 3.14159265f * 440.f * static_cast<float>(i) / static_cast<float>(sampleRate));

  tape.ProcessBlock(chans, 1, nFrames);
  out = buf;
}

int main()
{
  const int sampleRate = 48000;
  const int nFrames = sampleRate; // 1 second

  keith::fx::TapeAnalog<float> tape;

  keith::fx::TapeAnalog<float>::Params subtle {};
  subtle.saturation = 0.10f;
  subtle.headBump = 0.10f;
  subtle.mix = 0.20f;
  subtle.bypass = false;

  keith::fx::TapeAnalog<float>::Params destroy {};
  destroy.saturation = 1.0f;
  destroy.headBump = 1.0f;
  destroy.wow = 1.0f;
  destroy.flutter = 1.0f;
  destroy.hiss = 1.0f;
  destroy.printThrough = 0.80f;
  destroy.mix = 1.0f;
  destroy.bypass = false;

  std::vector<float> dry, subtleOut, destroyOut;
  {
    keith::fx::TapeAnalog<float>::Params bypass {};
    bypass.bypass = true;
    bypass.mix = 1.f;
    Render(tape, bypass, dry, sampleRate, nFrames);
  }
  Render(tape, subtle, subtleOut, sampleRate, nFrames);
  Render(tape, destroy, destroyOut, sampleRate, nFrames);

  WriteWav("build/tape_subtle.wav", subtleOut, sampleRate);
  WriteWav("build/tape_destroy.wav", destroyOut, sampleRate);

  double diffSubtle = 0.;
  double diffDestroy = 0.;
  for (int i = 0; i < nFrames; ++i)
  {
    diffSubtle += std::fabs(subtleOut[static_cast<size_t>(i)] - dry[static_cast<size_t>(i)]);
    diffDestroy += std::fabs(destroyOut[static_cast<size_t>(i)] - dry[static_cast<size_t>(i)]);
  }
  diffSubtle /= nFrames;
  diffDestroy /= nFrames;

  std::printf("tape_offline_test sampleRate=%d frames=%d\n", sampleRate, nFrames);
  std::printf("dry rms=%.6f\n", Rms(dry));
  std::printf("subtle mean_abs_diff=%.6f rms=%.6f\n", diffSubtle, Rms(subtleOut));
  std::printf("destroy mean_abs_diff=%.6f rms=%.6f\n", diffDestroy, Rms(destroyOut));
  std::printf("wrote build/tape_subtle.wav and build/tape_destroy.wav\n");

  return (diffSubtle > 1e-6 && diffDestroy > diffSubtle * 5.) ? 0 : 1;
}
