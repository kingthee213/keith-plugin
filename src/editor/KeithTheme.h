#pragma once

// Keith editor theme: palette, type styles and drawing helpers.
// Source of truth: docs/UI-DESIGN.md (§2 palette, §3 type, §4 panel vocabulary).

#include "IGraphics.h"

namespace keith {
namespace ui {

// ---------------------------------------------------------------------------
// Fonts loaded by the editor (OFL 1.1, embedded via resources/main.rc).
// ---------------------------------------------------------------------------
constexpr const char* kFontTitle = "Oswald";
constexpr const char* kFontMono = "SpaceMono";
constexpr const char* kFontMonoBold = "SpaceMonoBold";

// ---------------------------------------------------------------------------
// Palette (docs/UI-DESIGN.md §2)
// ---------------------------------------------------------------------------
// Editorial punk palette: tar-black ground, paper labels, acid paint and oxidised red.
// Native markings sit over original physically rendered materials (tools/render_materials.py).
const IColor kInk(255, 0x0A, 0x0A, 0x0A);
const IColor kWood(255, 0x6B, 0x4A, 0x2E);
const IColor kCharcoal(255, 0x2A, 0x2D, 0x30);
const IColor kTealPanel(255, 0x2F, 0x5C, 0x58);
const IColor kOlivePanel(255, 0x6B, 0x6A, 0x45);
const IColor kHotPink(255, 0xFF, 0x4F, 0xA3);
const IColor kPanel(255, 0x2A, 0x2D, 0x30);
const IColor kPanelAlt(255, 0x35, 0x38, 0x3C);
const IColor kLine(255, 0x02, 0x02, 0x02);
const IColor kPaper(255, 0xEA, 0xE1, 0xCD);
const IColor kGreen(255, 0xB7, 0xFF, 0x1A);
const IColor kMagenta(255, 0xE8, 0x4A, 0x2E);
const IColor kGold(255, 0xF2, 0xBE, 0x3E);
const IColor kCyan(255, 0x5C, 0xE1, 0xE6);
const IColor kMuteRed(255, 0xD9, 0x35, 0x35);

// Flat panel washes: inked print plates, not gradients.
const IColor kFillDrum(255, 0x2B, 0x20, 0x3A);
const IColor kFillSynth(255, 0x16, 0x2B, 0x20);
const IColor kFillFx(255, 0x36, 0x1D, 0x1B);
const IColor kFillPreset(255, 0x2B, 0x29, 0x1A);

// Comic outline weights and the hard drop shadow offset (§4.2)
constexpr float kOutline = 4.f;
constexpr float kInnerOutline = 2.f;
constexpr float kShadow = 4.f;

// ---------------------------------------------------------------------------
// Type styles (docs/UI-DESIGN.md §3)
// ---------------------------------------------------------------------------
inline IText Title(float size, const IColor& colour = kPaper)
{
  return IText(size, colour, kFontTitle, EAlign::Near, EVAlign::Middle);
}

inline IText Label(float size = 11.f, const IColor& colour = kPaper)
{
  return IText(size, colour, kFontMono, EAlign::Center, EVAlign::Middle);
}

inline IText Value(float size = 11.f, const IColor& colour = kGold)
{
  return IText(size, colour, kFontMonoBold, EAlign::Center, EVAlign::Middle);
}

inline IText Mono(float size, const IColor& colour, EAlign align = EAlign::Near,
                  EVAlign valign = EVAlign::Middle)
{
  return IText(size, colour, kFontMono, align, valign);
}

inline IText MonoBold(float size, const IColor& colour, EAlign align = EAlign::Near,
                      EVAlign valign = EVAlign::Middle)
{
  return IText(size, colour, kFontMonoBold, align, valign);
}

// ---------------------------------------------------------------------------
// IVStyle factory for the built-in vector controls (buttons, toggles, tab
// switches). Hard corners, no shadows, 2px ink frame — §4.2/§4.4.
// ---------------------------------------------------------------------------
inline IVStyle HardButton(const IColor& fill, const IColor& labelColour = kPaper,
                          float labelSize = 12.f, float roundness = 0.f,
                          const IColor& hover = IColor(40, 0xF4, 0xEF, 0xE4))
{
  IVColorSpec colours;
  colours.mColors[kBG] = kPanelAlt;
  colours.mColors[kFG] = fill;
  colours.mColors[kPR] = kGold;
  colours.mColors[kFR] = kLine;
  colours.mColors[kHL] = hover;
  colours.mColors[kSH] = kLine;
  colours.mColors[kX1] = kGreen;
  colours.mColors[kX2] = kMagenta;
  colours.mColors[kX3] = kGold;

  return IVStyle(true, true, colours)
    .WithRoundness(roundness)
    .WithDrawFrame(true)
    .WithFrameThickness(kInnerOutline)
    .WithDrawShadows(false)
    .WithEmboss(false)
    .WithLabelText(IText(labelSize, labelColour, kFontMonoBold, EAlign::Center, EVAlign::Middle))
    .WithValueText(Value(11.f, labelColour));
}

// ---------------------------------------------------------------------------
// Layout helpers — hard rect maths, no allocation.
// ---------------------------------------------------------------------------
inline IRECT ColCell(const IRECT& r, int col, int nCols, float gap = 0.f)
{
  const float w = (r.W() - gap * static_cast<float>(nCols - 1)) / static_cast<float>(nCols);
  const float l = r.L + static_cast<float>(col) * (w + gap);
  return IRECT(l, r.T, l + w, r.B);
}

inline IRECT RowCell(const IRECT& r, int row, int nRows, float gap = 0.f)
{
  const float h = (r.H() - gap * static_cast<float>(nRows - 1)) / static_cast<float>(nRows);
  const float t = r.T + static_cast<float>(row) * (h + gap);
  return IRECT(r.L, t, r.R, t + h);
}

// Split a rect into a row of `n` cells, taking cell `i`, with the first `fixed` cells at a fixed
// width. Used for "graph + knob column" blocks.
inline IRECT ColCellFixed(const IRECT& r, int i, int n, float fixedWidth, float gap = 0.f)
{
  IRECT head = r.GetFromLeft(fixedWidth);
  IRECT tail = r.GetReducedFromLeft(fixedWidth + gap);
  return (i < n - 1) ? ColCell(head, i, n - 1, gap) : tail;
}

// ---------------------------------------------------------------------------
// Drawing helpers
// ---------------------------------------------------------------------------

/** Draw text with a thick comic outline: 8 offset passes in the outline colour, then the fill.
 *  Zero allocation — the outline offsets are static. */
inline void DrawOutlinedText(IGraphics& g, const IText& text, const char* str, const IRECT& bounds,
                             const IColor& outline = kLine, float thickness = 2.f)
{
  static const float kOffsets[8][2] = {{-1.f, 0.f}, {1.f, 0.f},  {0.f, -1.f}, {0.f, 1.f},
                                       {-1.f, -1.f}, {1.f, -1.f}, {-1.f, 1.f}, {1.f, 1.f}};
  IText outlineText = text;
  outlineText.mFGColor = outline;

  for (int i = 0; i < 8; ++i)
    g.DrawText(outlineText, str,
               bounds.GetTranslated(kOffsets[i][0] * thickness, kOffsets[i][1] * thickness));

  g.DrawText(text, str, bounds);
}

/** Chipped paint at a panel edge: deterministic marks so each panel feels printed and handled,
 * without a texture asset or a soft effect. */
inline void DrawPaintChips(IGraphics& g, const IRECT& r, const IColor& accent)
{
  const float y = r.T + kOutline * 0.5f;
  const float x = r.L + kOutline * 0.5f;
  g.FillRect(accent, IRECT(x + 18.f, y, x + 44.f, y + 2.f));
  g.FillRect(kPaper.WithOpacity(0.75f), IRECT(r.R - 54.f, r.B - 2.f, r.R - 21.f, r.B));
  g.FillRect(accent.WithOpacity(0.75f), IRECT(r.L, r.T + 48.f, r.L + 2.f, r.T + 66.f));
  g.FillRect(kLine, IRECT(r.R - 2.f, r.T + 88.f, r.R, r.T + 104.f));
  g.DrawLine(kPaper.WithOpacity(0.22f), r.L + 7.f, r.B - 7.f, r.L + 36.f, r.B - 7.f, nullptr, 1.f);
}

/** A hard editorial panel: flat wash, ink border, hard offset shadow and chipped paint. */
inline void DrawPanelFrame(IGraphics& g, const IRECT& r, const IColor& wash, float border = kOutline,
                           bool shadow = true, const IColor& accent = kGreen)
{
  if (shadow)
    g.FillRect(kLine, r.GetTranslated(kShadow, kShadow));

  g.FillRect(wash, r);
  g.DrawRect(kLine, r, nullptr, border);
  DrawPaintChips(g, r, accent);
}

/** Title strip along the top edge of a panel. */
inline void DrawTitleStrip(IGraphics& g, const IRECT& r, const char* title, const IColor& fill,
                           const IColor& ink = kInk, float height = 26.f, float size = 19.f,
                           bool muted = false)
{
  const IRECT strip(r.L + kOutline, r.T + kOutline, r.R - kOutline, r.T + kOutline + height);
  g.FillRect(muted ? kPanelAlt : fill, strip);
  g.DrawRect(kLine, strip, nullptr, kInnerOutline);

  IText t = Title(size, ink);
  t.mAlign = EAlign::Near;
  const IRECT textBounds = strip.GetPadded(-8.f);

  if (muted)
  {
    // Outlined so a "next engine" title still reads on the dark strip.
    DrawOutlinedText(g, Title(size, kPaper), title, textBounds, kLine, 2.f);
  }
  else
  {
    g.DrawText(t, title, textBounds);
  }
}

/** Small hard badge, used for tags such as "NEXT ENGINE". */
inline void DrawBadge(IGraphics& g, const IRECT& r, const char* label, const IColor& fill,
                      const IColor& ink = kInk)
{
  g.FillRect(fill, r);
  g.DrawRect(kLine, r, nullptr, kInnerOutline);
  g.DrawText(Mono(11.f, ink, EAlign::Center), label, r);
}

// ---------------------------------------------------------------------------
// Family chip style: hard rectangle, gold when the family owns the current preset.
// ---------------------------------------------------------------------------
inline IVStyle ChipStyle(bool active, float labelSize = 13.f)
{
  return HardButton(active ? kGold : kPanelAlt, active ? kInk : kPaper, labelSize);
}

// ---------------------------------------------------------------------------
// Drum map (docs/DRUM-MAP.md §2 contiguous map, base note 36 = C1).
// ---------------------------------------------------------------------------
inline const char* const* DrumSlotNames()
{
  static const char* const kNames[16] = {
    "KICK", "SNARE", "SNARE ALT", "CLAP",
    "CLOSED HAT", "OPEN HAT", "PEDAL HAT", "TOM LO",
    "TOM MID", "TOM HI", "RIDE", "CRASH",
    "PERC 1", "PERC 2", "PERC 3", "PERC 4"
  };
  return kNames;
}

inline const char* const* DrumNoteNames()
{
  static const char* const kNotes[16] = {"C1",  "C#1", "D1",  "D#1", "E1", "F1",
                                         "F#1", "G1",  "G#1", "A1",  "A#1", "B1",
                                         "C2",  "C#2", "D2",  "D#2"};
  return kNotes;
}

} // namespace ui
} // namespace keith
