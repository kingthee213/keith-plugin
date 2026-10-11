#pragma once

// The rail page buttons: SAMPLER and WAVETABLE.
//
// These open full-width pages that cover the panel area. They are controls rather than
// parameters: a page switch is not part of a patch, so it does not belong in the parameter list
// and must not be serialised -- reopening a project should not drop the user into the sampler.
//
// ## Why these are their own control type
//
// iPlug2's IVButtonControl takes a parameter and writes to it. A page switch has no parameter.
// `KeithPageButton` extends IControl directly and keeps its own on/off state, so it can toggle
// pages without inventing a parameter that would then need saving, loading, and a default.
//
// ## What a page is
//
// A page is a set of controls shown or hidden together, sitting over the panel area. The panels
// themselves are not moved: the page covers them. That means opening a page never disturbs the
// layout underneath, which is what keeps this change low-risk -- the symmetry work on the panels
// is untouched by anything here.

#include "EditorialControls.h"

namespace keith {
namespace ui {

/** Which page is showing. Only one at a time; None means the normal panel view. */
enum class KeithPage
{
  None = 0,
  Sampler,
  Wavetable
};

/**
 * A button that shows or hides a page.
 *
 * Draws as a bordered tab with a label, matching the editor's other tabs. Pressed state is
 * drawn inverted so the open page is identifiable at a glance.
 */
class KeithPageButton : public IControl
{
public:
  /**
   * @param bounds   Where the button sits.
   * @param page     Which page it opens.
   * @param state    Shared page state. A pointer rather than a value so both buttons agree on
   *                 which page is current without either owning it.
   */
  KeithPageButton(const IRECT& bounds, KeithPage page, KeithPage* state)
  : IControl(bounds)
  , mPage(page)
  , mState(state)
  {
  }

  void Draw(IGraphics& g) override
  {
    const bool open = (mState != nullptr && *mState == mPage);

    // Open reads as lit: a bright fill with dark text. Closed is the quiet bordered tab the
    // editor uses elsewhere, so the rail does not compete with the panel it opens.
    // FillRect and DrawRect rather than rounded rects: this codebase uses neither FillRoundRect
    // nor DrawRoundRect anywhere, so the rounded variants are unproven here. The editor's other
    // tabs are square-cornered, so this also matches them.
    if (open)
    {
      g.FillRect(kInk, mWidgetBounds);
      g.DrawRect(kInk, mWidgetBounds);
      g.DrawText(MonoBold(9.f, kPaper, EAlign::Center), Label(), mWidgetBounds);
    }
    else
    {
      g.FillRect(kPanel, mWidgetBounds);
      g.DrawRect(kPaper, mWidgetBounds);
      g.DrawText(MonoBold(9.f, kPaper, EAlign::Center), Label(), mWidgetBounds);
    }
  }

  void OnMouseDown(float x, float y, const IMouseMod&) override
  {
    (void) x;
    (void) y;
    if (mState == nullptr)
      return;

    // Clicking the open page's button closes it, so the same control both opens and dismisses.
    *mState = (*mState == mPage) ? KeithPage::None : mPage;
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
  }

private:
  const char* Label() const
  {
    return (mPage == KeithPage::Sampler) ? "SAMPLER" : "WAVETABLE";
  }

  KeithPage mPage;
  KeithPage* mState;
};

} // namespace ui
} // namespace keith
