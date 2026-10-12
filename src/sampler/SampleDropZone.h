#pragma once

// Drop target for sample files.
//
// Separate from SamplerEngine.h because this needs `IControl`, which only exists in an editor
// build and only after iPlug2's control headers have been included. SamplerEngine.h is
// included from Keith.h, which the DSP and the editor both read and which is parsed before
// those headers -- so keeping the IControl-dependent part here avoids depending on include
// order.
//
// iPlug2 routes a file drop to whichever `IControl` is under the cursor, via
// `IGraphics::OnDrop(const char*, float, float)`. A transparent control spanning the editor is
// therefore the simplest way to accept a drop anywhere in the window.

#pragma once

#include "SamplerEngine.h"
#include <string>
#include <vector>

namespace keith {
namespace ui {

/**
 * Transparent control covering the editor that accepts a dragged audio file.
 *
 * Nothing is painted -- `Draw` is deliberately empty, so the drop area does not alter the face.
 * It exists purely to receive `OnDrop`, which iPlug2 calls on the message thread, which is what
 * makes decoding safe here.
 */
class KeithSampleDropZone : public IControl
{
public:
  KeithSampleDropZone(const IRECT& bounds, keith::sampler::SamplerEngine& engine)
  : IControl(bounds)
  , mEngine(engine)
  {
  }

  /** A drop target, not a visual element. */
  void Draw(IGraphics&) override {}

  void OnDrop(const char* str) override
  {
    if (!str)
      return;
    mEngine.Load(std::string(str));
    // Repaint so anything showing the sample's name or waveform updates.
    if (auto* g = GetUI())
      g->SetAllControlsDirty();
  }

  void OnDropMultiple(const std::vector<const char*>& paths) override
  {
    // Only the first file loads. A slot holds one sample, so accepting a batch would silently
    // discard all but one; loading the first and ignoring the rest is honest about that.
    if (!paths.empty())
      OnDrop(paths[0]);
  }

private:
  keith::sampler::SamplerEngine& mEngine;
};

} // namespace ui
} // namespace keith
