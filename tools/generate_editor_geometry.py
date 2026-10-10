#!/usr/bin/env python3
"""Generate EditorGeometry.h control rects for W7 laptop grid (1152x768, 0.75 of reference)."""
from __future__ import annotations

import re
from pathlib import Path

REF_SCALE = 0.75


def _r(v: float) -> int:
    return int(round(v * REF_SCALE))


def _rf(v: float) -> float:
    return round(v * REF_SCALE, 1)


LABEL_FONT_PX = _rf(11.0)
KNOB_H = _r(34 + 19 + 20)
KNOB_GAP = _rf(14)
TAB_H = _rf(28)
DROPDOWN_H = _rf(52)
DROPDOWN_H_COMPACT = _rf(36)
FADER_H = _rf(96)
FADER_W = _rf(54)
MIN_W = _rf(42)
PAD = _rf(18)
ROW_GAP = _rf(14)

ADVANCE_EM = 0.60
ROW_PITCH_GAP = 4.0


def _load_worst_values() -> list[str]:
    text = (Path(__file__).parent / "worst_values.inc").read_text()
    return re.findall(r'"([^"]*)"', text)


WORST_VALUES = _load_worst_values()


def _mono_advance(units: int, font_px: float) -> float:
    return units * font_px / 1000.0


def _text_width(text: str, font_px: float) -> float:
    return ADVANCE_EM * font_px * len(text)


def min_control_width(param: int) -> float:
    """At least label advance, value advance, and value advance + row-pitch padding."""
    label, _kind, units = LABELS[param]
    label_adv = _mono_advance(units, LABEL_FONT_PX)
    value_adv = _text_width(WORST_VALUES[param], LABEL_FONT_PX)
    return max(MIN_W, label_adv, value_adv, value_adv + ROW_PITCH_GAP)


def shared_knob_width(params: list[int], inner_w: float, cols: int, gap: float = ROW_GAP) -> float:
    """Width for a row of equal-width knobs filling inner_w.

    Always returns the exact equal share. It used to return max(share, min_control_width),
    which meant a row whose labels needed more than their share came out wider than the
    panel: the controls were then laid out from the left and the row ended short on the
    right, or overran. Every asymmetric row in the editor came from that. The share wins,
    and the label is made to fit its rectangle instead -- which is what the fit gate is for.
    """
    return (inner_w - (cols - 1) * gap) / cols

TITLE_BAND = _rf(28)
VISIBLE_ROW_GAP = _rf(14)
CANVAS = (0, 0, _r(1536), _r(1024))

_PANELS_REF = {
    "kPanelOsc": (250, 24, 552, 416),
    "kPanelFilter": (562, 24, 724, 416),
    "kPanelAmp": (728, 24, 928, 416),
    "kPanelLfo": (938, 24, 1240, 416),
    "kPanelMod": (1250, 24, 1500, 416),
    # The middle band. Vertical extent 322..533 design units: 312 is the top band's bottom
    # edge and 543 the bottom band's top edge, with clearance on both sides.
    # Sized so the three panels stop short of the right-hand wood frame.
    # The frame is 30 px of the 1152 canvas, so the usable right edge is 1122 -- the panels
    # used to run to 1125 and overlapped it by 3 px.
    #   AMP MODIFIERS  8 knobs, two rows of four
    #   LOADED SOUNDS  preset menu plus its knobs; widened because the menu was cramped
    #   DRUM PADS      4x4 grid plus three slot knobs; the rest of the width
    "kPanelAmpMods": (256, 430, 585, 711),
    "kPanelSample": (593, 430, 905, 711),
    "kPanelPads": (912, 430, 1496, 711),
    "kPanelVintage": (256, 724, 730, 996),
    "kPanelGlobal": (744, 724, 1066, 996),
    "kPanelFx": (1080, 724, 1496, 1000),
    "kLeftRail": (0, 0, 246, 1024),
    # The wordmark sits in the dark field to the right of the wood rail, vertically centred
    # and nudged right of the field's middle per the owner's direction. The box is tall enough
    # for the two words at the larger size set in compose_face.draw_wordmark, which draws them
    # into the finished 2x face so the glyphs stay crisp.
    "kWordmarkBox": (57, 23, 244, 223),
    # The keyboard runs from the vintage panel's left edge to the right-hand wood frame, so
    # it starts where the bottom band starts and stops before the frame, which is 30 px of
    # the canvas (usable right edge 1122 -> reference 1496).
    "kKeyboardBox": (256.0, 1002.0, 1496.0, 1020.0),
}

PANELS = {name: tuple(_r(v) for v in box) for name, box in _PANELS_REF.items()}
KEYBOARD = tuple(_rf(v) for v in _PANELS_REF["kKeyboardBox"])
PANELS["kKeyboardBox"] = tuple(int(round(v)) for v in KEYBOARD)

OVERLAYS: dict[str, tuple[float, float, float, float]] = {}

KINDS = {
    "knob": "ControlKind::Knob",
    "tab": "ControlKind::Tab",
    "dropdown": "ControlKind::Dropdown",
    "fader": "ControlKind::Fader",
    "pad": "ControlKind::Pad",
}

LABELS = {
    0: ("VOL", "knob", 1836), 1: ("DIST", "knob", 2448), 2: ("TONE", "knob", 2448),
    3: ("WOW", "knob", 1836), 4: ("FLUT", "knob", 2448), 5: ("HISS", "knob", 2448),
    6: ("PRINT", "knob", 3060), 7: ("MIX", "knob", 1836), 8: ("BYP", "tab", 1836),
    9: ("TYPE", "tab", 2448), 10: ("POLY", "dropdown", 2448), 11: ("GLIDE", "knob", 3060),
    12: ("UNI", "knob", 1836), 13: ("DET", "knob", 1836), 14: ("SHP", "knob", 1836),
    15: ("TUN", "knob", 2448), 16: ("LVL", "knob", 1836), 17: ("PW", "knob", 1224),
    18: ("SHP", "knob", 1836), 19: ("TUN", "knob", 2448), 20: ("LVL", "knob", 1836),
    21: ("PW", "knob", 1224), 22: ("SUB", "knob", 1836), 23: ("NOISE", "knob", 3060),
    24: ("FM", "knob", 1224), 25: ("TYPE", "tab", 2448), 26: ("CUT", "knob", 1836),
    27: ("RES", "knob", 1836), 28: ("ENV", "knob", 1836), 29: ("ATT", "knob", 1836),
    30: ("DEC", "knob", 1836), 31: ("SUS", "knob", 1836), 32: ("REL", "knob", 1836),
    33: ("PATK", "knob", 1836), 34: ("PDEC", "knob", 1836), 35: ("PSUS", "knob", 1836),
    36: ("PREL", "knob", 1836), 37: ("PAMT", "knob", 1836), 38: ("MASTER", "knob", 1836),
    39: ("FATT", "knob", 1836), 40: ("FDEC", "knob", 1836), 41: ("FSUS", "knob", 1836),
    42: ("FREL", "knob", 1836), 43: ("SYNC", "tab", 2448), 44: ("RING", "tab", 2448),
    45: ("KEY", "knob", 1836), 46: ("COMP", "knob", 2448), 47: ("RATE", "knob", 2448),
    48: ("SHP", "dropdown", 1836), 49: ("DST", "dropdown", 1836), 50: ("DEP", "knob", 1836),
    51: ("MWD", "dropdown", 1836), 52: ("MWA", "knob", 1836), 53: ("VEL", "knob", 1836),
    54: ("FILT", "knob", 1836), 55: ("LEG", "dropdown", 1836), 56: ("I1", "dropdown", 1224),
    57: ("A1", "knob", 1224), 58: ("B1", "knob", 1224), 59: ("C1", "knob", 1224),
    60: ("M1", "knob", 1224), 62: ("I2", "dropdown", 1224), 63: ("A2", "knob", 1224),
    64: ("B2", "knob", 1224), 65: ("C2", "knob", 1224), 66: ("M2", "knob", 1224),
    68: ("MS", "dropdown", 1224), 69: ("MA", "knob", 1224), 70: ("MB", "knob", 1224),
    71: ("MC", "knob", 1224), 72: ("MX", "knob", 1224), 74: ("OUT", "knob", 1836),
    75: ("PAD", "pad", 1836), 76: ("LVL", "knob", 1836), 77: ("PAN", "knob", 1836),
    78: ("TUN", "knob", 1836), 79: ("DEC", "knob", 1836), 80: ("FLT", "knob", 1836),
    81: ("DRV", "knob", 1836), 82: ("MUT", "tab", 1836), 83: ("SOL", "tab", 1836),
    84: ("KIT", "knob", 1836), 85: ("RT", "knob", 1224), 86: ("SHP", "dropdown", 1836),
    87: ("DST", "dropdown", 1836), 88: ("DEP", "knob", 1836), 89: ("S1", "dropdown", 1224),
    90: ("D1", "dropdown", 1224), 91: ("A1", "knob", 1224), 92: ("S2", "dropdown", 1224),
    93: ("D2", "dropdown", 1224), 94: ("A2", "knob", 1224), 95: ("S3", "dropdown", 1224),
    96: ("D3", "dropdown", 1224), 97: ("A3", "knob", 1224), 98: ("S4", "dropdown", 1224),
    99: ("D4", "dropdown", 1224), 100: ("A4", "knob", 1224), 101: ("NKEY", "knob", 1836),
    102: ("AKEY", "knob", 1836), 103: ("ABND", "knob", 1836), 104: ("DKEY", "knob", 1836),
    105: ("DBND", "knob", 1836), 106: ("SDLY", "knob", 1836), 107: ("STRK", "knob", 1836),
    108: ("SWID", "knob", 1836), 109: ("DIR", "knob", 1836), 110: ("CBND", "knob", 1836),
}

LABEL_ABBREVIATIONS = sorted({label for label, _kind, _units in LABELS.values()})


def label_abbrev_lines():
    lines = [
        "constexpr const char* kLabelAbbreviations[] = {",
    ]
    for label in LABEL_ABBREVIATIONS:
        lines.append(f'  "{label}",')
    lines.extend([
        "};",
        "constexpr int kNumLabelAbbreviations = static_cast<int>(sizeof(kLabelAbbreviations) / sizeof(const char*));",
        "inline bool LabelIsDeclaredAbbrev(const char* label) {",
        "  for (int i = 0; i < kNumLabelAbbreviations; ++i)",
        "    if (std::strcmp(label, kLabelAbbreviations[i]) == 0) return true;",
        "  return false;",
        "}",
        "inline bool LabelPassesAbbrevGate(const char* label) {",
        "  const std::size_t n = std::strlen(label);",
        "  if (n == 0) return false;",
        "  if (n == 1) return LabelIsDeclaredAbbrev(label);",
        "  if (n >= 4) return true;",
        "  return LabelIsDeclaredAbbrev(label);",
        "}",
    ])
    return lines

BYPASS_VIA_ALGO = {61, 67, 73}
rects: dict[int, tuple[float, float, float, float]] = {}


def box(l, t, r, b):
    return (l, t, r, b)


def content_top(panel_t: float) -> float:
    return panel_t + TITLE_BAND


def overlaps(a, b):
    return a[0] < b[2] and b[0] < a[2] and a[1] < b[3] and b[1] < a[3]


def title_band(panel_key: str) -> tuple[float, float, float, float]:
    l, t, r, b = PANELS[panel_key]
    return (l, t, r, t + TITLE_BAND)


def place_knobs_grid(items, l, t, r, b, cols, knob_w=None):
    n = len(items)
    rows = (n + cols - 1) // cols
    floor = max(min_control_width(p) for p in items)
    kw = knob_w or min(64, (r - l - KNOB_GAP * (cols - 1)) / cols)
    kw = max(kw, floor)
    row_step = KNOB_H + KNOB_GAP
    col_step = kw + KNOB_GAP
    total_h = rows * KNOB_H + (rows - 1) * KNOB_GAP
    y0 = t + max(0, (b - t - total_h) / 2)
    for i, param in enumerate(items):
        row, col = divmod(i, cols)
        x = l + col * col_step
        y = y0 + row * row_step
        rects[param] = box(x, y, x + kw, y + KNOB_H)


def place_row(items, l, t, w, h, gap=ROW_GAP):
    w = max(w, max(min_control_width(p) for p in items))
    x = l
    for param in items:
        rects[param] = box(x, t, x + w, t + h)
        x += w + gap



def place_overlay(oid: str, l, t, r, b):
    OVERLAYS[oid] = box(l, t, r, b)


# OSC
#
# Four equal columns with equal gaps, so the knob rows are symmetric inside the panel. The
# columns used to be laid out as two groups of two, which left a 0 px gap between the middle
# pair and a 10.5 px overhang on the right -- visibly unbalanced.
#
# Vertically: title band, a gap, the waveform, then two rows of knobs, then the tabs. The
# waveform used to start flush against the title band with no gap at all, which read as
# crowded. The slack below the title is given to the waveform so it is larger, not tighter.
osc_l, osc_t, osc_r, osc_b = PANELS["kPanelOsc"]
inner_l, inner_r = osc_l + PAD, osc_r - PAD
inner_osc = inner_r - inner_l

osc_cols = 4
kw = (inner_osc - (osc_cols - 1) * ROW_GAP) / osc_cols
col_x = [inner_l + i * (kw + ROW_GAP) for i in range(osc_cols)]

title_end = content_top(osc_t)
slack = (osc_b - PAD) - title_end
# Elements under the title: a gap, the waveform, then three knob rows and the tab row, each
# separated by a gap. The three rows carry their own two internal gaps, so only the four
# outer gaps are counted here -- counting five double-counted one and left the waveform 10 px
# shorter than it needed to be.
wave_h = slack - (4 * ROW_GAP) - (3 * KNOB_H + 2 * ROW_GAP) - TAB_H

y = title_end + ROW_GAP
half = (inner_osc - ROW_GAP) / 2
place_overlay("osc1_waveform", inner_l, y, inner_l + half, y + wave_h)
place_overlay("osc2_waveform", inner_l + half + ROW_GAP, y, inner_r, y + wave_h)
y += wave_h + ROW_GAP

# Osc 1 on the left two columns, osc 2 on the right two.
osc_layout = [(14, 0, 0), (15, 1, 0), (18, 2, 0), (19, 3, 0),
              (16, 0, 1), (17, 1, 1), (20, 2, 1), (21, 3, 1)]
for p, col, row in osc_layout:
    yy = y + row * (KNOB_H + ROW_GAP)
    rects[p] = box(col_x[col], yy, col_x[col] + kw, yy + KNOB_H)
y += 2 * (KNOB_H + ROW_GAP)

# Mixer: the three controls share the full width, evenly.
mix_w = (inner_osc - 2 * ROW_GAP) / 3
for i, p in enumerate([22, 23, 24]):
    x = inner_l + i * (mix_w + ROW_GAP)
    rects[p] = box(x, y, x + mix_w, y + KNOB_H)
y += KNOB_H + ROW_GAP

# SYNC and RING share the width as a symmetric pair.
half_t = (inner_osc - ROW_GAP) / 2
rects[43] = box(inner_l, y, inner_l + half_t, y + TAB_H)
rects[44] = box(inner_l + half_t + ROW_GAP, y, inner_r, y + TAB_H)

# FILTER — the type tab, then three rows of two.
#
# The panel has ~60 px of unused height and the tab used to sit flush against the title band
# with no gap, which read as crowded. The knobs move down and the slack is spread as even
# gaps, with a little more under the title so the type menu has air. Nothing outside this
# panel changes.
fl, ft, fr, fb = PANELS["kPanelFilter"]
inner_filt = (fr - fl) - 2 * PAD
kw_filt = (inner_filt - ROW_GAP) / 2
filt_title_end = content_top(ft)
filt_bottom = fb - PAD
FILT_TOP_GAP = _r(21)
filt_gap = (filt_bottom - filt_title_end - FILT_TOP_GAP - TAB_H - 3 * KNOB_H) / 4

y_filt = filt_title_end + FILT_TOP_GAP
rects[25] = box(fl + PAD, y_filt, fr - PAD, y_filt + TAB_H)
y_filt += TAB_H + filt_gap
for row_params in ([26, 27], [28, 45], [53, 54]):
    for i, p in enumerate(row_params):
        x = fl + PAD + i * (kw_filt + ROW_GAP)
        rects[p] = box(x, y_filt, x + kw_filt, y_filt + KNOB_H)
    y_filt += KNOB_H + filt_gap

# AMP — two blocks of four: amp ADSR then filter ADSR. Two equal columns filling the panel.
al, at, ar, ab = PANELS["kPanelAmp"]
inner_amp = (ar - al) - 2 * PAD
kw_amp = (inner_amp - ROW_GAP) / 2
y_amp = content_top(at) + ROW_GAP
for row, params in enumerate(([29, 30], [31, 32])):
    y = y_amp + row * (KNOB_H + ROW_GAP)
    for i, p in enumerate(params):
        x = al + PAD + i * (kw_amp + ROW_GAP)
        rects[p] = box(x, y, x + kw_amp, y + KNOB_H)
y_fa = y_amp + 2 * (KNOB_H + ROW_GAP) + ROW_GAP
for row, params in enumerate(([39, 40], [41, 42])):
    y = y_fa + row * (KNOB_H + ROW_GAP)
    for i, p in enumerate(params):
        x = al + PAD + i * (kw_amp + ROW_GAP)
        rects[p] = box(x, y, x + kw_amp, y + KNOB_H)

# AMP MODIFIERS — eight U17 modifiers at the plugin's standard knob size (KNOB_H, the same
# 55 px every other knob uses), two rows of four. Two rows is what the band's 190 px of
# content height allows; four rows of two would run 251 px and overlap Vintage below.
aml, amt, amr, amb = PANELS["kPanelAmpMods"]
amp_mods = [101, 102, 103, 104, 105, 106, 107, 108]
mod_cols = 4
mod_w = shared_knob_width(amp_mods, amr - aml - 2 * PAD, mod_cols)
row_y0 = content_top(amt) + ROW_GAP
for i, p in enumerate(amp_mods):
    row = i // mod_cols
    col = i % mod_cols
    y = row_y0 + row * (KNOB_H + ROW_GAP)
    x = aml + PAD + col * (mod_w + ROW_GAP)
    rects[p] = box(x, y, x + mod_w, y + KNOB_H)

# LFO — one row per block: two knobs and two compact dropdowns, all four sharing the width
# evenly so the row fills the panel. It used to size the dropdowns by their label minimum and
# the knobs from the remainder, which left 109 px of dead space on the right.
#
# Each block is label / gap / controls, with the same generous gap the FILTER panel uses under
# its title. The label used to sit a single ROW_GAP above the controls, which made the thin
# full-width label strip merge into the row beneath it.
ll, lt, lr, lb = PANELS["kPanelLfo"]
LFO_LABEL_H = _rf(14)
LFO_LABEL_GAP = 16.0

# The vertical slack is split so the pair sits balanced, with a little more air under the
# panel title than between the blocks. "LFO 1" sat directly beneath the "LFO" title with only
# a normal row gap, which read as the two titles crowding each other.
lfo_block_h = LFO_LABEL_H + LFO_LABEL_GAP + KNOB_H
lfo_slack = (lb - PAD) - content_top(lt) - 2 * lfo_block_h
lfo_title_gap = lfo_slack / 3 + _r(16)      # a bit more than the even share
lfo_gap = (lfo_slack - lfo_title_gap) / 2


def lfo_block(y_top, rate, shape, dest, amount, label_id):
    place_overlay(label_id, ll + PAD, y_top, lr - PAD, y_top + LFO_LABEL_H)
    cy = y_top + LFO_LABEL_H + LFO_LABEL_GAP
    inner = (lr - ll) - 2 * PAD
    # Four equal columns: rate, amount, shape, dest. The dropdowns sit in their own column
    # rather than taking space from the knobs.
    col = (inner - 3 * ROW_GAP) / 4
    x = ll + PAD
    rects[rate] = box(x, cy, x + col, cy + KNOB_H)
    x += col + ROW_GAP
    rects[amount] = box(x, cy, x + col, cy + KNOB_H)
    x += col + ROW_GAP
    dd_y = cy + (KNOB_H - DROPDOWN_H_COMPACT) / 2
    rects[shape] = box(x, dd_y, x + col, dd_y + DROPDOWN_H_COMPACT)
    x += col + ROW_GAP
    rects[dest] = box(x, dd_y, x + col, dd_y + DROPDOWN_H_COMPACT)


y1 = content_top(lt) + lfo_title_gap
lfo_block(y1, 47, 48, 49, 50, "lfo1_label")
lfo_block(y1 + lfo_block_h + lfo_gap, 85, 86, 87, 88, "lfo2_label")

# MOD — four slots, each a source dropdown, a destination dropdown and an amount knob.
# The three controls share the row evenly, so every slot fills the panel. The amount knob
# used to be pinned to the right edge at the label minimum, which left a 0 px gap after the
# destination dropdown and a mismatched right margin.
ml, mt, mr, mb = PANELS["kPanelMod"]
inner_mod = (mr - ml) - 2 * PAD
slot_rows = 4
mod_col = (inner_mod - 2 * ROW_GAP) / 3
slot_h = (mb - content_top(mt) - PAD - (slot_rows - 1) * ROW_GAP) / slot_rows
for slot, (src, dst, amt) in enumerate([(89, 90, 91), (92, 93, 94), (95, 96, 97), (98, 99, 100)]):
    y = content_top(mt) + slot * (slot_h + ROW_GAP)
    dd_y = y + (slot_h - DROPDOWN_H) / 2
    rects[src] = box(ml + PAD, dd_y, ml + PAD + mod_col, dd_y + DROPDOWN_H)
    rects[dst] = box(ml + PAD + mod_col + ROW_GAP, dd_y,
                     ml + PAD + 2 * mod_col + ROW_GAP, dd_y + DROPDOWN_H)
    ky = y + (slot_h - KNOB_H) / 2
    rects[amt] = box(mr - PAD - mod_col, ky, mr - PAD, ky + KNOB_H)

# SAMPLE — 3×2 knobs (value-sized width); MUT/SOL tabs share the preset row on the right.
sl, st, sr, sb = PANELS["kPanelSample"]
inner_w = sr - sl - 2 * PAD
sample_knobs = [33, 34, 35, 36, 37, 84]
kw3 = shared_knob_width(sample_knobs, inner_w - 2 * ROW_GAP, 3)
tab_w82 = min_control_width(82)
tab_w83 = min_control_width(83)
tabs_total = tab_w82 + ROW_GAP + tab_w83
preset_h = _rf(36)
y_knob_row2 = sb - PAD - KNOB_H
y_knob_row1 = y_knob_row2 - ROW_GAP - KNOB_H
y_preset = y_knob_row1 - ROW_GAP - preset_h
y0 = content_top(st)
wave_h = max(0.0, y_preset - y0 - ROW_GAP)
place_overlay("sample_waveform", sl + PAD, y0, sr - PAD, y0 + wave_h)
preset_w = max(MIN_W, inner_w - tabs_total - ROW_GAP)
place_overlay("preset_menu", sl + PAD, y_preset, sl + PAD + preset_w, y_preset + preset_h)
tab_y = y_preset + (preset_h - TAB_H) / 2
tab_x = sr - PAD - tabs_total
rects[82] = box(tab_x, tab_y, tab_x + tab_w82, tab_y + TAB_H)
rects[83] = box(tab_x + tab_w82 + ROW_GAP, tab_y, tab_x + tabs_total, tab_y + TAB_H)
for i, param in enumerate(sample_knobs):
    row, col = divmod(i, 3)
    y = y_knob_row1 if row == 0 else y_knob_row2
    x = sl + PAD + col * (kw3 + ROW_GAP)
    rects[param] = box(x, y, x + kw3, y + KNOB_H)

# PADS
#
# The pads get most of the panel width and its full height; the six slot controls sit in a
# single column of three beside the grid. The grid was previously given
# (pr - PAD - 3*40 - 2*ROW_GAP), which left the 4x4 pads too small to hit comfortably, and
# the slot knobs were laid out from min_control_width without checking against the panel
# edge, so the last one could run off the canvas.
pl, pt, pr, pb = PANELS["kPanelPads"]
slot_params = [76, 77, 78, 79, 80, 81]
inner_w = (pr - pl) - 2 * PAD
# The slot column and the grid have to SHARE inner_w. Sizing the grid first and taking the
# knobs from the remainder is what pushed the third knob past the panel edge -- twice.
# Fix a usable minimum for each, then split the slack: most of it to the grid.
slot_cols = 3
# Minimums for the pads panel: a usable grid and three comfortably hittable slot knobs.
# In reference units; the design-space minimums are 240 px for the grid and 46 px per knob.
GRID_MIN = _r(320)      # 240 design px
SLOT_MIN = _r(61)       # 46 design px
slack = inner_w - (GRID_MIN + ROW_GAP + (slot_cols * SLOT_MIN + (slot_cols - 1) * ROW_GAP))
if slack < 0:
    raise SystemExit(f"pads: grid + slot column need {GRID_MIN + ROW_GAP + slot_cols*SLOT_MIN:.0f} "
                     f"px but the panel inner is {inner_w:.0f}")
grid_w = GRID_MIN + slack * 0.60
slot_w = SLOT_MIN + (slack * 0.40) / slot_cols

grid_l = pl + PAD
grid_t = content_top(pt) + ROW_GAP
grid_b = pb - PAD
rects[75] = box(grid_l, grid_t, grid_l + grid_w, grid_b)

# Slot controls: two rows of three, top-aligned beside the grid.
trio_x = grid_l + grid_w + ROW_GAP
right_limit = pr - PAD
for row, trio in enumerate([(76, 77, 78), (79, 80, 81)]):
    y = grid_t + row * (KNOB_H + ROW_GAP)
    for i, p in enumerate(trio):
        x = trio_x + i * (slot_w + ROW_GAP)
        if x + slot_w > right_limit + 0.5:
            raise SystemExit(
                f"pads: slot knob {p} would end at {x + slot_w:.1f}, past the panel edge "
                f"{right_limit:.1f}")
        rects[p] = box(x, y, x + slot_w, y + KNOB_H)

# PAD MIDI — the switch that routes live MIDI to the pads. It sits in the pocket under the
# slot knobs, spanning the same width, so it reads as part of that column. Not a saved
# parameter: it is editor state, held on Keith as an atomic and read by the audio thread.
pad_sw_top = grid_t + 2 * (KNOB_H + ROW_GAP) + ROW_GAP
pad_sw_bottom = pb - PAD
place_overlay("pad_midi_switch", trio_x, pad_sw_top, right_limit, pad_sw_bottom)

# VINTAGE — the analog voice block. ENGINE and BYP on one switch row, then nine knobs:
# the three voice trims (TONE / COMP / DIST) and the six tape controls that used to live
# down the wood rail. Five columns, two rows.
vl, vt, vr, vb = PANELS["kPanelVintage"]
inner_v = vr - vl - 2 * PAD
# Space between the panel title band and the switch row. The owner asked for clearance here;
# the switch was sitting directly under the title.
VINTAGE_TITLE_GAP = _r(19)
sw_y = content_top(vt) + VINTAGE_TITLE_GAP
tab_w_engine = max(min_control_width(9), _r(150))
tab_w_byp = max(min_control_width(8), _r(110))
rects[9] = box(vl + PAD, sw_y, vl + PAD + tab_w_engine, sw_y + TAB_H)
byp_x = vl + PAD + tab_w_engine + ROW_GAP
rects[8] = box(byp_x, sw_y, byp_x + tab_w_byp, sw_y + TAB_H)

# Eleven knobs: the three voice trims (TONE/COMP/DIST), the six tape controls
# (WOW/FLUT/HISS/PRINT/MIX/OUT), and the synth voice's two trims (DIR/CBND) which used to sit
# on the wood rail. Six columns by two rows is what fits the 183 px of usable height; five
# columns would need a third row and overrun the panel by 59 px.
vintage_knobs = [2, 46, 1, 3, 4, 5, 6, 7, 74, 109, 110]
v_cols = 6
v_kw = shared_knob_width(vintage_knobs, inner_v, v_cols)
v_y0 = sw_y + TAB_H + ROW_GAP
for i, p in enumerate(vintage_knobs):
    row, col = divmod(i, v_cols)
    y = v_y0 + row * (KNOB_H + ROW_GAP)
    x = vl + PAD + col * (v_kw + ROW_GAP)
    rects[p] = box(x, y, x + v_kw, y + KNOB_H)

# GLOBAL
gl, gt, gr, gb = PANELS["kPanelGlobal"]
inner = gr - gl - 2 * PAD
y1 = content_top(gt) + ROW_GAP
right = gr - PAD
mode_w = _rf(56)
mode_y = y1 + (KNOB_H - DROPDOWN_H) / 2
rects[55] = box(right - mode_w, mode_y, right, mode_y + DROPDOWN_H)
rects[10] = box(right - 2 * mode_w - ROW_GAP, mode_y, right - mode_w - ROW_GAP, mode_y + DROPDOWN_H)
left = gl + PAD
knob_span = right - 2 * mode_w - 2 * ROW_GAP - left
kw_g = (knob_span - 2 * ROW_GAP) / 3
x = left
for p in (0, 11, 12):
    rects[p] = box(x, y1, x + kw_g, y1 + KNOB_H)
    x += kw_g + ROW_GAP
knob_y = gb - PAD - KNOB_H
drop_w = _rf(64)
bottom = [13, 52]
bw = (inner - drop_w - ROW_GAP - (len(bottom) - 1) * ROW_GAP) / len(bottom)
x = gl + PAD
for p in bottom:
    rects[p] = box(x, knob_y, x + bw, knob_y + KNOB_H)
    x += bw + ROW_GAP
rects[51] = box(x, knob_y, x + drop_w, knob_y + DROPDOWN_H)

# Left rail
lr_l, _lr_t, lr_r, _lr_b = PANELS["kLeftRail"]
# The rail carries the master output level, the MIDI hints, and nothing else interactive.
# MASTER is deliberately larger than a panel knob -- it is the one control on the rail and
# the last thing in the signal path, so it should read as the biggest.
MASTER_H = KNOB_H * 1.35
MASTER_W = MASTER_H * 0.8
master_cx = (lr_l + lr_r) / 2
master_y = _r(330)
rects[38] = box(master_cx - MASTER_W / 2, master_y,
                master_cx + MASTER_W / 2, master_y + MASTER_H)

place_overlay("midi_hint1", lr_l + PAD, _r(944), lr_r - PAD, _r(962))
place_overlay("midi_hint2", lr_l + PAD, _r(962), lr_r - PAD, _r(980))

# FX
fxl, fxt, fxr, fxb = PANELS["kPanelFx"]
inner_fx = fxr - fxl - 2 * PAD
menu_y = content_top(fxt) + ROW_GAP
third = (inner_fx - 2 * ROW_GAP) / 3
fx_menu_h = DROPDOWN_H_COMPACT
rects[56] = box(fxl + PAD, menu_y, fxl + PAD + third, menu_y + fx_menu_h)
rects[62] = box(fxl + PAD + third + ROW_GAP, menu_y, fxl + PAD + 2 * third + ROW_GAP, menu_y + fx_menu_h)
rects[68] = box(fxl + PAD + 2 * (third + ROW_GAP), menu_y, fxr - PAD, menu_y + fx_menu_h)
all_fx_knobs = [57, 58, 59, 60, 63, 64, 65, 66, 69, 70, 71, 72]
fx_row1_y = menu_y + fx_menu_h + ROW_GAP
fx_row2_y = fx_row1_y + KNOB_H + ROW_GAP
knob_w = shared_knob_width(all_fx_knobs, inner_fx - 5 * VISIBLE_ROW_GAP, 6, gap=VISIBLE_ROW_GAP)
for row_idx, knobs in enumerate((all_fx_knobs[:6], all_fx_knobs[6:])):
    y = fx_row1_y if row_idx == 0 else fx_row2_y
    kx = fxl + PAD
    for p in knobs:
        rects[p] = box(kx, y, kx + knob_w, y + KNOB_H)
        kx += knob_w + VISIBLE_ROW_GAP

overlay_conflicts = []
for oid, r in OVERLAYS.items():
    for p, cr in rects.items():
        if overlaps(r, cr):
            overlay_conflicts.append(f"overlay {oid} {r} overlaps param {p} {cr}")
if overlay_conflicts:
    print(f"{len(overlay_conflicts)} overlay conflicts:", flush=True)
    print("\n".join(overlay_conflicts), flush=True)
    raise SystemExit(1)

for p in range(111):
    if p in BYPASS_VIA_ALGO:
        continue
    if p not in rects:
        raise SystemExit(f"missing rect for param {p}")

for p, (l, t, r, b) in rects.items():
    _label, kind, _units = LABELS[p]
    w = r - l
    if kind == "fader" and w < MIN_W:
        raise SystemExit(f"param {p} fader width {w:.1f} < {MIN_W}")

panel_boxes = {k: v for k, v in PANELS.items() if k.startswith("kPanel")}


def panel_for_rect(r):
    cx, cy = (r[0] + r[2]) / 2, (r[1] + r[3]) / 2
    for name, (l, t, rr, b) in panel_boxes.items():
        if l <= cx <= rr and t <= cy <= b:
            return name
    return None


for p, r in rects.items():
    if r[2] > CANVAS[2] or r[3] > CANVAS[3] or r[0] < 0 or r[1] < 0:
        raise SystemExit(f"param {p} out of canvas: {r}")
    for pname, pb in panel_boxes.items():
        if overlaps(r, pb):
            band = title_band(pname)
            if overlaps(r, band):
                raise SystemExit(f"param {p} overlaps title band in {pname}: {r} vs {band}")

kb = PANELS["kKeyboardBox"]
for p, r in rects.items():
    if overlaps(r, kb):
        raise SystemExit(f"param {p} overlaps keyboard: {r}")

for name, (l, t, rr, b) in panel_boxes.items():
    if b > kb[1] and overlaps((l, t, rr, b), kb):
        raise SystemExit(f"panel {name} overlaps keyboard")

items = sorted(rects.items())
for i, (pa, ra) in enumerate(items):
    for pb, rb in items[i + 1:]:
        if overlaps(ra, rb):
            raise SystemExit(f"overlap {pa} vs {pb}: {ra} {rb}")

grouped = {k: [] for k in panel_boxes}
for p, r in rects.items():
    pname = panel_for_rect(r)
    if pname:
        grouped[pname].append(p)

for name, bucket in sorted(grouped.items()):
    if not bucket:
        continue
    l, t, r, b = panel_boxes[name]
    density = (r - l) * (b - t) / len(bucket)
    flag = "OK" if density >= 5000 else "LOW"
    print(f"# {name}: {len(bucket)} controls density={density:.0f} {flag}")



def overlay_lines():
    lines = [
        "struct OverlayRect { const char* id; Box rect; };",
        "inline constexpr OverlayRect kOverlayRects[] = {",
    ]
    for oid, (l, tt, r, b) in sorted(OVERLAYS.items()):
        lines.append(f'  {{"{oid}", {{{l:.1f}f, {tt:.1f}f, {r:.1f}f, {b:.1f}f}}}},')
    lines.extend([
        "};",
        "constexpr int kNumOverlayRects = static_cast<int>(sizeof(kOverlayRects) / sizeof(OverlayRect));",
        "inline const OverlayRect* OverlayFor(const char* id) {",
        "  for (int i = 0; i < kNumOverlayRects; ++i)",
        "    if (std::strcmp(id, kOverlayRects[i].id) == 0) return &kOverlayRects[i];",
        "  return nullptr;",
        "}",
    ])
    return lines



def panel_lines():
    lines = [
        f"constexpr float kPanelTitleBand = {TITLE_BAND}f;",
        f"constexpr Box kKeyboardBox {{{KEYBOARD[0]:.1f}f, {KEYBOARD[1]:.1f}f, "
        f"{KEYBOARD[2]:.1f}f, {KEYBOARD[3]:.1f}f}};",
        "inline Box PanelTitleBand(Box panel) { return {panel.l, panel.t, panel.r, panel.t + kPanelTitleBand}; }",
    ]
    lines.extend(
        f"constexpr Box {name} {{{l:.1f}f, {t:.1f}f, {r:.1f}f, {b:.1f}f}};"
        for name, (l, t, r, b) in PANELS.items()
        if name not in ("kKeyboardBox",)
    )
    return lines


def control_rect_lines():
    lines = ["inline constexpr ControlRect kControlRects[] = {"]
    for p in sorted(rects.keys()):
        l, t, r, b = rects[p]
        label, kind, units = LABELS[p]
        lines.append(
            f"  {{{p}, {{{l:.1f}f, {t:.1f}f, {r:.1f}f, {b:.1f}f}}, \"{label}\", {LABEL_FONT_PX:.1f}f, {units}, {KINDS[kind]}}},"
        )
    lines.append("};")
    return lines


def knob_placement_lines():
    lines = [
        "#pragma once",
        '#include "IControl.h"',
        '#include "EditorGeometry.h"',
        "#include <cassert>",
        "",
        "namespace keith { namespace ui {",
        "struct KnobPlacement { int parameter; const char* page; float left, top, right, bottom, radius; "
        "IRECT Bounds() const { return IRECT(left, top, right, bottom); } };",
        "inline const KnobPlacement kKnobPlacements[] = {",
    ]
    for p in sorted(rects.keys()):
        _label, kind, _units = LABELS[p]
        if kind != "knob":
            continue
        l, t, r, b = rects[p]
        radius = min(r - l, b - t) * 0.42
        lines.append(f'  {{{p}, "single", {l:.1f}f, {t:.1f}f, {r:.1f}f, {b:.1f}f, {radius:.1f}f}},')
    lines.extend([
        "};",
        "inline const KnobPlacement& PlacementFor(int parameter){for(const auto& item:kKnobPlacements)"
        "if(item.parameter==parameter)return item;assert(false&&\"Knob must have a physical placement\");"
        "return kKnobPlacements[0];}",
        "} }",
    ])
    return lines


def apply_to_repo():
    import re
    from pathlib import Path

    root = Path(__file__).resolve().parents[1]
    geom_path = root / "src/editor/EditorGeometry.h"
    geom = geom_path.read_text()
    panel_block = "\n".join(panel_lines())
    geom = re.sub(
        r"constexpr float kPanelTitleBand = .*?constexpr Box kWordmarkBox \{[^}]+\};",
        panel_block,
        geom,
        count=1,
        flags=re.S,
    )
    if "kPanelTitleBand" not in geom:
        geom = re.sub(
            r"constexpr Box kPanelOsc \{.*?constexpr Box kWordmarkBox \{[^}]+\};",
            panel_block,
            geom,
            count=1,
            flags=re.S,
        )
    overlay_block = "\n".join(overlay_lines())
    if "kOverlayRects" in geom:
        geom = re.sub(
            r"struct OverlayRect \{.*?inline const OverlayRect\* OverlayFor\(const char\* id\) \{.*?\n\}",
            overlay_block.rstrip(),
            geom,
            count=1,
            flags=re.S,
        )
    else:
        geom = geom.replace(
            "struct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };",
            overlay_block.rstrip() + "\nstruct ControlRect { int parameter; Box rect; const char* label; float labelFontPx; int labelUnits; ControlKind kind; };",
            1,
        )
    cw, ch = float(CANVAS[2]), float(CANVAS[3])
    lrw = float(PANELS["kLeftRail"][2])
    geom = re.sub(
        r"constexpr float kCanvasW = [\d.]+f;\nconstexpr float kCanvasH = [\d.]+f;\nconstexpr float kLeftRailW = [\d.]+f;",
        f"constexpr float kCanvasW = {cw:.1f}f;\nconstexpr float kCanvasH = {ch:.1f}f;\nconstexpr float kLeftRailW = {lrw:.1f}f;",
        geom,
        count=1,
    )
    geom = re.sub(
        r"constexpr float kPanelPadding = [\d.]+f;\nconstexpr float kControlGap = [\d.]+f;",
        f"constexpr float kPanelPadding = {PAD:.1f}f;\nconstexpr float kControlGap = {ROW_GAP:.1f}f;",
        geom,
        count=1,
    )
    knob_label = _rf(19)
    knob_value = _rf(20)
    knob_body = _rf(34)
    geom = re.sub(
        r"constexpr float kKnobLabelHeight = [\d.]+f;\nconstexpr float kKnobValueHeight = [\d.]+f;\nconstexpr float kKnobBodyMin = [\d.]+f;",
        f"constexpr float kKnobLabelHeight = {knob_label:.1f}f;\n"
        f"constexpr float kKnobValueHeight = {knob_value:.1f}f;\n"
        f"constexpr float kKnobBodyMin = {knob_body:.1f}f;",
        geom,
        count=1,
    )
    rect_block = "\n".join(control_rect_lines())
    geom = re.sub(
        r"inline constexpr ControlRect kControlRects\[\] = \{.*?\};",
        rect_block,
        geom,
        count=1,
        flags=re.S,
    )
    abbrev_block = "\n".join(label_abbrev_lines())
    if "LabelPassesAbbrevGate" in geom:
        geom = re.sub(
            r"constexpr const char\* kLabelAbbreviations\[\] = \{.*?\ninline bool LabelPassesAbbrevGate\(const char\* label\) \{.*?\n\}",
            abbrev_block,
            geom,
            count=1,
            flags=re.S,
        )
    else:
        geom = re.sub(
            r"(constexpr Box kLeftRail \{[^}]+\};)\n",
            "\1\n" + abbrev_block + "\n",
            geom,
            count=1,
        )
    if "LabelPassesAbbrevGate" not in geom:
        raise SystemExit("failed to inject abbreviation table")
    if "constexpr Box kWordmarkBox" not in geom:
        geom = geom.replace(
            "constexpr Box kLeftRail {0.0f, 0.0f, 210.0f, 800.0f};",
            "constexpr Box kLeftRail {0.0f, 0.0f, 210.0f, 800.0f};\nconstexpr Box kWordmarkBox {18.0f, 28.0f, 192.0f, 118.0f};",
            1,
        )
    if "#include <cstring>" not in geom:
        geom = geom.replace("#include <cmath>\n", "#include <cmath>\n#include <cstring>\n", 1)
    geom_path.write_text(geom)
    (root / "src/editor/ControlLayout.h").write_text("\n".join(knob_placement_lines()) + "\n")
    print(f"wrote {geom_path} and ControlLayout.h", flush=True)


print("\n// --- generated ---")
print("\n".join(panel_lines()))
print()
print("\n".join(control_rect_lines()))

if __name__ == "__main__":
    import sys

    if "--write" in sys.argv:
        apply_to_repo()
