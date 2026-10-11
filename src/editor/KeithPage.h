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

// Explicit, though EditorialControls.h also brings it in. KeithPageKnob and KeithPageTabSwitch
// derive from KeithKnobControl and IVTabSwitchControl, so this header genuinely depends on
// KeithControls.h -- and an inherited definition is not a guarantee, it is an accident waiting for
// the day EditorialControls.h stops including it.
#include "EditorialControls.h"
#include "KeithControls.h"
#include "KeithPageId.h"

namespace keith {
namespace ui {

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
      g.FillRect(kInk, mRECT);
      g.DrawRect(kInk, mRECT);
      g.DrawText(MonoBold(9.f, kPaper, EAlign::Center), Label(), mRECT);
    }
    else
    {
      g.FillRect(kPanel, mRECT);
      g.DrawRect(kPaper, mRECT);
      g.DrawText(MonoBold(9.f, kPaper, EAlign::Center), Label(), mRECT);
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


// -------------------------------------------------------------------------------------------
// The page surface and its contents
//
// The page is drawn as one surface rather than as a set of independent controls, because the
// panels underneath must be COVERED, not merely overlapped. A per-control draw would let panel
// controls show through the gaps between page elements.
//
// ## Visibility
//
// Every page control reads the shared KeithPage state in Draw() and draws nothing when its page is
// closed. That is deliberately simpler than attaching and detaching controls on every page change:
// attaching allocates, and it happens on the message thread while the editor may be mid-draw.
// Skipping the draw costs nothing and cannot be got wrong.
//
// Hit-testing is the one thing Draw() cannot suppress, so OnMouseDown on each page control returns
// early when its page is closed. Without that, a closed page would still swallow clicks meant for
// the panel underneath it.
// -------------------------------------------------------------------------------------------

/** Shared behaviour for anything living on a page: hidden when the page is closed. */
class KeithPageElement : public IControl
{
public:
  KeithPageElement(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : IControl(bounds)
  , mState(state)
  , mPage(page)
  {
  }

protected:
  /** Should this element draw and accept input right now? */
  bool PageIsOpen() const { return mState != nullptr && *mState == mPage; }

  const KeithPage* mState;
  KeithPage mPage;
};

/**
 * The page surface: a filled rectangle that hides the panels behind it.
 *
 * Uses the editor's panel colour rather than the paper background, so an open page reads as a
 * surface laid over the instrument rather than as a hole cut into it. The border matches the
 * panel borders, so the page looks like one of them.
 */
class KeithPageSurface : public KeithPageElement
{
public:
  KeithPageSurface(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  void Draw(IGraphics& g) override
  {
    if (!PageIsOpen())
      return;
    g.FillRect(kInk, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, 2.f);
  }

  void OnMouseDown(float, float, const IMouseMod&) override
  {
    // Swallows clicks so they cannot reach the panels behind the page. Without this, clicking a
    // page's empty area would operate a control the user cannot see.
  }
};

/** A caption on a page: the section title, or a label beside a control. */
class KeithPageLabel : public KeithPageElement
{
public:
  KeithPageLabel(const IRECT& bounds, const KeithPage* state, KeithPage page, const char* text,
                 float size = 11.f, EAlign align = EAlign::Near)
  : KeithPageElement(bounds, state, page)
  , mSize(size)
  , mAlign(align)
  {
    mText.Set(text);
    mIgnoreMouse = true;
  }

  void SetText(const char* t) { mText.Set(t); }

  void Draw(IGraphics& g) override
  {
    if (!PageIsOpen())
      return;
    g.DrawText(MonoBold(mSize, kPaper, mAlign), mText.Get(), mRECT);
  }

private:
  WDL_String mText;
  float mSize;
  EAlign mAlign;
};

/** A labelled empty area: the waveform view, the frame view, the browser lists. */
class KeithPagePanel : public KeithPageElement
{
public:
  KeithPagePanel(const IRECT& bounds, const KeithPage* state, KeithPage page)
  : KeithPageElement(bounds, state, page)
  {
  }

  void Draw(IGraphics& g) override
  {
    if (!PageIsOpen())
      return;
    g.FillRect(kPanel, mRECT);
    g.DrawRect(kLine, mRECT, nullptr, 1.f);
  }
};

// -------------------------------------------------------------------------------------------
// PLAY and SLICE: the sampler's mode, as two full-width buttons under the page title.
//
// Full width and stacked, because they are the page's primary choice rather than two of many
// controls -- the owner asked for exactly that. Bound to kParamSampleMode, so the mode is part of
// the patch: a sound saved in SLICE reloads in SLICE.
// -------------------------------------------------------------------------------------------
class KeithPageModeButton : public KeithPageElement
{
public:
  KeithPageModeButton(const IRECT& bounds, const KeithPage* state, KeithPage page,
                      int paramIdx, int modeValue, const char* title, const char* detail)
  : KeithPageElement(bounds, state, page)
  , mParamIdx(paramIdx)
  , mValue(modeValue)
  {
    mTitle.Set(title);
    mDetail.Set(detail);
  }

  void Draw(IGraphics& g) override
  {
    if (!PageIsOpen())
      return;

    // Read through the delegate, the same way the bypass toggle does, rather than IControl's
    // own GetParam -- this control is bound to a parameter index passed at construction, not to
    // IControl's single held parameter.
    const auto* plugin = dynamic_cast<const iplug::IPluginBase*>(GetDelegate());
    const int current = (plugin != nullptr && plugin->GetParam(mParamIdx) != nullptr)
                          ? plugin->GetParam(mParamIdx)->Int()
                          : 0;
    const bool active = current == mValue;

    // Active is the lit state: a filled bar with the detail text in ink. Inactive is the quiet
    // outlined bar. The detail line is what explains the difference between the two, so it stays
    // readable in both states -- a user should not have to press one to find out what it does.
    if (active)
    {
      g.FillRect(kHotPink, mRECT);
      g.DrawRect(kInk, mRECT, nullptr, 1.f);
      g.DrawText(MonoBold(12.f, kInk), mTitle.Get(),
                 mRECT.GetReducedFromRight(mRECT.W() * 0.42f).GetPadded(-14.f));
      g.DrawText(Mono(9.f, kInk, EAlign::Far), mDetail.Get(),
                 mRECT.GetReducedFromLeft(mRECT.W() * 0.30f).GetPadded(-14.f));
    }
    else
    {
      g.FillRect(kPanel, mRECT);
      g.DrawRect(kLine, mRECT, nullptr, 1.f);
      g.DrawText(MonoBold(12.f, kPaper), mTitle.Get(),
                 mRECT.GetReducedFromRight(mRECT.W() * 0.42f).GetPadded(-14.f));
      g.DrawText(Mono(9.f, kPanelAlt, EAlign::Far), mDetail.Get(),
                 mRECT.GetReducedFromLeft(mRECT.W() * 0.30f).GetPadded(-14.f));
    }
  }

  void OnMouseDown(float, float, const IMouseMod&) override
  {
    if (!PageIsOpen())
      return;

    // SetNormalized + SetDirty(false), which is how the editor's other parameter-bound controls
    // write. Set()/SetDirty() are not used anywhere in this codebase, so they are unproven here;
    // the enum has two values, so normalised 0.0 and 1.0 are exactly the two states.
    auto* plugin = dynamic_cast<iplug::IPluginBase*>(GetDelegate());
    if (plugin == nullptr)
      return;
    IParam* param = plugin->GetParam(mParamIdx);
    if (param == nullptr)
      return;

    param->SetNormalized(mValue == 1 ? 1.0 : 0.0);
    SetDirty(false);
    if (auto* ui = GetUI())
      ui->SetAllControlsDirty();
  }

private:
  int mParamIdx;
  int mValue;
  WDL_String mTitle;
  WDL_String mDetail;
};

// -------------------------------------------------------------------------------------------
// Page-aware versions of the two real controls the wavetable page needs.
//
// KeithKnobControl and IVTabSwitchControl are ordinary iPlug2 controls: they draw whenever they
// are attached, with no notion of a page. Attaching them directly to a page control area means
// they draw ALWAYS -- over the panels, whether the sampler/wavetable page is open or not.
//
// There is no Hide()/Show() in use anywhere in this codebase, so relying on it would be guessing
// at an unproven API. These derive from the real control and add the same guard the rest of the
// page uses: draw nothing and take no input when the page is closed.
//
// Deriving rather than wrapping keeps the behaviour identical to the panels -- same knob, same
// tab switch, same parameter binding -- with only the visibility rule added.
// -------------------------------------------------------------------------------------------

/** A panel-style knob that only draws while its page is open. */
class KeithPageKnob : public KeithKnobControl
{
public:
  KeithPageKnob(const IRECT& bounds, int paramIdx, const char* label, const IColor& accent,
                const KeithPage* state, KeithPage page)
  : KeithKnobControl(bounds, paramIdx, label, accent)
  , mState(state)
  , mPage(page)
  {
  }

  void Draw(IGraphics& g) override
  {
    if (!Open())
      return;
    KeithKnobControl::Draw(g);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    // Guarded as well as Draw: a closed page must not swallow clicks meant for the panel behind it.
    if (!Open())
      return;
    KeithKnobControl::OnMouseDown(x, y, mod);
  }

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    if (!Open())
      return;
    KeithKnobControl::OnMouseDrag(x, y, dX, dY, mod);
  }

  void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override
  {
    if (!Open())
      return;
    KeithKnobControl::OnMouseWheel(x, y, mod, d);
  }

private:
  bool Open() const { return mState != nullptr && *mState == mPage; }
  const KeithPage* mState;
  KeithPage mPage;
};

/** A tab switch that only draws while its page is open. */
class KeithPageTabSwitch : public IVTabSwitchControl
{
public:
  KeithPageTabSwitch(const IRECT& bounds, int paramIdx,
                     std::initializer_list<const char*> choices, const char* label,
                     const IVStyle& style, const KeithPage* state, KeithPage page)
  : IVTabSwitchControl(bounds, paramIdx, choices, label, style, EVShape::Rectangle)
  , mState(state)
  , mPage(page)
  {
  }

  void Draw(IGraphics& g) override
  {
    if (!Open())
      return;
    IVTabSwitchControl::Draw(g);
  }

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (!Open())
      return;
    IVTabSwitchControl::OnMouseDown(x, y, mod);
  }

private:
  bool Open() const { return mState != nullptr && *mState == mPage; }
  const KeithPage* mState;
  KeithPage mPage;
};

} // namespace ui
} // namespace keith
