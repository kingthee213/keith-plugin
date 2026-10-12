#!/usr/bin/env python3
"""Generate EditorGeometry.h control rects for the editor grid (1400x768, 0.75 of reference)."""
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
# The reference grid the panels are authored in. 1536 reference units was the 1152 px canvas;
# the sampler page needs more width, so the grid grows with it. 1867 units converts to exactly
# 1400.25 design px, and every coordinate here goes through _r() so the odd ratio is consistent
# rather than accumulating rounding.
CANVAS = (0, 0, _r(1867), _r(1024))

_PANELS_REF = {
    # Top band, left to right. The LFO is narrow and stacked (knobs above their menus), which
    # frees the width the third oscillator needs in OSC. Every other panel keeps the exact
    # width it had -- FILTER 121 and AMP 150 are unchanged, so no knob is resized to make
    # room. The freed width moves as a block: OSC grows into the space, and FILTER, AMP and
    # MOD slide rather than being squeezed.
    "kPanelOsc": (251, 24, 692, 416),
    "kPanelFilter": (700, 24, 861, 416),
    "kPanelAmp": (869, 24, 1069, 416),
    "kPanelLfo": (1077, 24, 1239, 416),
    "kPanelMod": (1247, 24, 1496, 416),
    # The middle band. Vertical extent 322..533 design units: 312 is the top band's bottom
    # edge and 543 the bottom band's top edge, with clearance on both sides.
    # Sized so the three panels stop short of the right-hand wood frame.
    # The frame is 30 px of the 1152 canvas, so the usable right edge is 1122 -- the panels
    # used to run to 1125 and overlapped it by 3 px.
    #   AMP MODIFIERS  8 knobs, two rows of four
    #   LOADED SOUNDS  preset menu plus its knobs; widened because the menu was cramped
    #   DRUMS          4x4 grid plus three slot knobs; the rest of the width
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
    # Third oscillator. Labels follow osc 1 and 2 so the columns read alike; SRCE and MRPH
    # are four-letter abbreviations, which the gate requires for anything under five.
    111: ("SHP", "knob", 1836), 112: ("TUN", "knob", 2448), 113: ("LVL", "knob", 1836),
    114: ("PW", "knob", 1224), 115: ("ON", "tab", 1836), 116: ("MRPH", "knob", 2448),
    117: ("SRCE", "tab", 2448),
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


# OSC — three oscillators, each a waveform over two rows of two knobs.
#
# Six equal columns: shape/pitch on the top row, level/pulse-width on the bottom, two columns
# per oscillator. The panel was widened from four columns to six when the third oscillator
# was added; see the panel table for where the width came from. Equal gaps and matched margins
# throughout, so no oscillator pair looks more tightly packed than another.
#
# Osc 3 also carries a source switch, a morph knob and an on/off tab. Those sit underneath
# osc 3's own columns rather than in the shared rows, so osc 1 and 2 are unaffected by its
# extra controls.
osc_l, osc_t, osc_r, osc_b = PANELS["kPanelOsc"]
inner_l, inner_r = osc_l + PAD, osc_r - PAD
inner_osc = inner_r - inner_l

osc_cols = 6
kw = (inner_osc - (osc_cols - 1) * ROW_GAP) / osc_cols
col_x = [inner_l + i * (kw + ROW_GAP) for i in range(osc_cols)]

title_end = content_top(osc_t)
# Six elements under the title -- waveform, two knob rows, osc 3's extras row, the mixer row
# and the tab row -- with seven gaps around and between them. The waveform takes two
# gap-units, so the slack divides into nine. The fixed heights are three knob rows (the two
# oscillator rows and the mixer) plus two tab-height rows (the extras and the tabs).
# Three knob rows -- osc 1, osc 2, and the mixer row, which now carries MORPH as a fourth
# column -- plus one tab row. Four knob rows would not fit: it would leave 3 px for the
# waveform, which is not a waveform. Folding MORPH into the mixer row is what keeps it a
# proper knob (68 px wide, wider than a standard 42) instead of a cramped tab.
osc_fixed_h = 3 * KNOB_H + TAB_H
# The title gap is weighted heavier than the gaps between rows, so the whole stack sits down
# off the heading instead of starting immediately under it. At the even split the first
# waveform sat 9 px below the title band, which is the tightest of any panel -- FILTER gives
# 16, VINTAGE 14, GLOBAL 28. The extra comes out of the waveform and the interior gaps, both
# of which have room, rather than out of the knobs.
OSC_TITLE_EXTRA = 16.0
osc_unit = ((osc_b - PAD) - title_end - osc_fixed_h - OSC_TITLE_EXTRA) / 8
osc_gap = osc_unit
wave_h = osc_unit * 1.5
osc_title_gap = osc_unit + OSC_TITLE_EXTRA

y = title_end + osc_title_gap
third = (inner_osc - 2 * ROW_GAP) / 3
for i, oid in enumerate(("osc1_waveform", "osc2_waveform", "osc3_waveform")):
    x = inner_l + i * (third + ROW_GAP)
    place_overlay(oid, x, y, x + third, y + wave_h)
y += wave_h + osc_gap

# Osc 1 in columns 0-1, osc 2 in 2-3, osc 3 in 4-5.
osc_layout = [(14, 0, 0), (15, 1, 0), (18, 2, 0), (19, 3, 0), (111, 4, 0), (112, 5, 0),
              (16, 0, 1), (17, 1, 1), (20, 2, 1), (21, 3, 1), (113, 4, 1), (114, 5, 1)]
for p, col, row in osc_layout:
    yy = y + row * (KNOB_H + ROW_GAP)
    rects[p] = box(col_x[col], yy, col_x[col] + kw, yy + KNOB_H)
y += 2 * (KNOB_H + osc_gap)

# Mixer row, four equal columns: SUB, NOISE, FM and osc 3's MORPH. MORPH belongs here because
# it is a level-like control on the same row and there is no height for a row of its own.
mix_w = (inner_osc - 3 * ROW_GAP) / 4
for i, p in enumerate([22, 23, 24, 116]):
    x = inner_l + i * (mix_w + ROW_GAP)
    rects[p] = box(x, y, x + mix_w, y + KNOB_H)
y += KNOB_H + osc_gap

# Tab row, four equal columns: SYNC, RING, and osc 3's source switch and on/off.
tab_w = (inner_osc - 3 * ROW_GAP) / 4
for i, p in enumerate([43, 44, 117, 115]):
    x = inner_l + i * (tab_w + ROW_GAP)
    rects[p] = box(x, y, x + tab_w, y + TAB_H)


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

# AMP — two blocks of four: amp ADSR then filter ADSR, two equal columns filling the panel.
# The four rows and the three gaps between them are spread over the panel's content height so
# the space above the first row matches the space below the last.
al, at, ar, ab = PANELS["kPanelAmp"]
inner_amp = (ar - al) - 2 * PAD
kw_amp = (inner_amp - ROW_GAP) / 2
amp_top = content_top(at)
amp_rows = [[29, 30], [31, 32], [39, 40], [41, 42]]
amp_gap = ((ab - PAD) - amp_top - 4 * KNOB_H) / 5
for row, params in enumerate(amp_rows):
    y = amp_top + amp_gap + row * (KNOB_H + amp_gap)
    for i, p in enumerate(params):
        x = al + PAD + i * (kw_amp + ROW_GAP)
        rects[p] = box(x, y, x + kw_amp, y + KNOB_H)

# AMP MODIFIERS — eight U17 modifiers at the plugin's standard knob size (KNOB_H, the same
# 55 px every other knob uses), two rows of four. Two rows is what the band's 190 px of
# content height allows; four rows of two would run 251 px and overlap Vintage below.
aml, amt, amr, amb = PANELS["kPanelAmpMods"]
amp_mods = [101, 102, 103, 104, 105, 106, 107, 108]
mod_cols = 4
inner_mod = (amr - aml) - 2 * PAD
mod_w = (inner_mod - (mod_cols - 1) * ROW_GAP) / mod_cols
mod_top = content_top(amt)
# Two rows: spread the slack so the gap under the title and the gap at the base match.
mod_gap = ((amb - PAD) - mod_top - 2 * KNOB_H) / 3
for i, p in enumerate(amp_mods):
    row = i // mod_cols
    col = i % mod_cols
    y = mod_top + mod_gap + row * (KNOB_H + mod_gap)
    x = aml + PAD + col * (mod_w + ROW_GAP)
    rects[p] = box(x, y, x + mod_w, y + KNOB_H)

# LFO — each block is two knobs side by side with their two menus directly UNDERNEATH.
#
# The panel is deliberately narrow (see the panel table): stacking the menus under the knobs
# makes each block taller but only two columns wide, and that width is what pays for the third
# oscillator in OSC. Width is the currency here, not height -- there is vertical room to stack,
# there was not horizontal room to keep everything on one row.
#
# Each block is label / gap / knob row / menu row. The label keeps its clearance from the panel
# title, which is the owner's original ask -- "LFO 1" sitting flush under "LFO" read as two
# titles crowding.
ll, lt, lr, lb = PANELS["kPanelLfo"]
LFO_LABEL_H = _rf(14)
LFO_LABEL_GAP = 12.0
LFO_ROW_GAP = 8.0

# Block height: label, gap, a knob row, then a menu row under it.
lfo_block_h = LFO_LABEL_H + LFO_LABEL_GAP + KNOB_H + LFO_ROW_GAP + DROPDOWN_H_COMPACT
# Three gaps: under the title, between the two blocks, and above the base. The title gap
# carries extra weight so the pair sits low enough to clear the heading.
lfo_slack = (lb - PAD) - content_top(lt) - 2 * lfo_block_h
LFO_TITLE_EXTRA = _r(14)
lfo_title_gap = (lfo_slack + 2 * LFO_TITLE_EXTRA) / 3
lfo_gap = (lfo_slack - lfo_title_gap) / 2


def lfo_block(y_top, rate, shape, dest, amount, label_id):
    place_overlay(label_id, ll + PAD, y_top, lr - PAD, y_top + LFO_LABEL_H)
    inner = (lr - ll) - 2 * PAD
    # Two equal columns, used twice: knobs on the first row, menus on the second.
    col = (inner - ROW_GAP) / 2
    ax = ll + PAD
    bx = ll + PAD + col + ROW_GAP

    knob_y = y_top + LFO_LABEL_H + LFO_LABEL_GAP
    rects[rate] = box(ax, knob_y, ax + col, knob_y + KNOB_H)
    rects[amount] = box(bx, knob_y, bx + col, knob_y + KNOB_H)

    menu_y = knob_y + KNOB_H + LFO_ROW_GAP
    rects[shape] = box(ax, menu_y, ax + col, menu_y + DROPDOWN_H_COMPACT)
    rects[dest] = box(bx, menu_y, bx + col, menu_y + DROPDOWN_H_COMPACT)


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

# SAMPLE — the waveform, the preset menu, then two rows of three knobs. The mute and solo
# tabs used to live in this panel's title band; they have moved to DRUMS, so the menu keeps
# the full panel width on its own row.
sl, st, sr, sb = PANELS["kPanelSample"]
inner_w = sr - sl - 2 * PAD
sample_knobs = [33, 34, 35, 36, 37, 84]
kw3 = (inner_w - 2 * ROW_GAP) / 3

preset_h = _rf(36)
# Four elements: the waveform, the preset menu, and two knob rows. Spread the slack so the
# gap under the title equals the gap at the base.
s_top = content_top(st)
s_bottom = sb - PAD
# Elements: the waveform, the preset menu, and two knob rows -- four of them, with five
# gaps around and between. The waveform counts as one gap-unit, so the slack divides into six.
s_gap = (s_bottom - s_top - 2 * KNOB_H - preset_h) / 6
y0 = s_top + s_gap
wave_h = s_gap
y_preset = y0 + wave_h + s_gap
y_knob_row1 = y_preset + preset_h + s_gap
y_knob_row2 = y_knob_row1 + KNOB_H + s_gap
place_overlay("sample_waveform", sl + PAD, y0, sr - PAD, y0 + wave_h)
# The menu takes the whole inner width now.
place_overlay("preset_menu", sl + PAD, y_preset, sr - PAD, y_preset + preset_h)
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
# The grid takes the panel's content height, inset by the same gap at the top and the
# bottom, so it sits centred in the panel rather than hard against the base.
pads_gap = ((pb - PAD) - content_top(pt)) * 0.04 + ROW_GAP * 0.5
grid_t = content_top(pt) + pads_gap
grid_b = pb - PAD - pads_gap
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

# Mute and solo sit in the DRUMS title band, above the fourth pad column and right-aligned on
# the panel. They were in the LOADED SOUNDS title band; the owner wants them with the pads
# they act on. Placing them over the last column keeps them clear of the "DRUMS" heading.
tab_w82 = min_control_width(82)
tab_w83 = min_control_width(83)
tabs_total = tab_w82 + ROW_GAP + tab_w83
tab_h_ui = TAB_H
tab_y = pt + (TITLE_BAND - tab_h_ui) / 2
tab_x = pr - PAD - tabs_total
rects[82] = box(tab_x, tab_y, tab_x + tab_w82, tab_y + tab_h_ui)
rects[83] = box(tab_x + tab_w82 + ROW_GAP, tab_y, tab_x + tabs_total, tab_y + tab_h_ui)

# PAD MIDI — the switch that routes live MIDI to the pads. It sits in the pocket under the
# slot knobs, spanning the same width, so it reads as part of that column. Not a saved
# parameter: it is editor state, held on Keith as an atomic and read by the audio thread.
# It stops short of the panel base by the same margin the grid above it uses, so the switch
# is not flush against the bottom edge.
pad_sw_top = grid_t + 2 * (KNOB_H + ROW_GAP) + ROW_GAP
pad_sw_bottom = pb - PAD - pads_gap
place_overlay("pad_midi_switch", trio_x, pad_sw_top, right_limit, pad_sw_bottom)

# VINTAGE — the analog voice block. ENGINE and BYP on one switch row, then nine knobs:
# the three voice trims (TONE / COMP / DIST) and the six tape controls that used to live
# down the wood rail. Five columns, two rows.
vl, vt, vr, vb = PANELS["kPanelVintage"]
inner_v = vr - vl - 2 * PAD
# Space between the panel title band and the switch row. The owner asked for clearance here;
# the switch was sitting directly under the title.
VINTAGE_TITLE_GAP = _r(19)
# The switch row spans the panel: ENGINE and BYPASS are equal halves rather than being sized
# to their labels, which left 124 px of dead space to their right.
sw_y = content_top(vt) + VINTAGE_TITLE_GAP
sw_w = (inner_v - ROW_GAP) / 2
rects[9] = box(vl + PAD, sw_y, vl + PAD + sw_w, sw_y + TAB_H)
rects[8] = box(vl + PAD + sw_w + ROW_GAP, sw_y, vr - PAD, sw_y + TAB_H)

# Eleven knobs: the three voice trims (TONE/COMP/DIST), the six tape controls
# (WOW/FLUT/HISS/PRINT/MIX/OUT), and the synth voice's two trims (DIR/CBND) which used to sit
# on the wood rail. Six columns by two rows is what fits the 183 px of usable height; five
# columns would need a third row and overrun the panel by 59 px.
# Eleven knobs cannot divide evenly into six columns, so each row is spread across the full
# panel width on its own: six on the first row, five on the second, each row using the same
# inner span with its own gap. That keeps both margins equal on both rows instead of leaving
# a 56 px hole at the end of the second.
vintage_knobs = [2, 46, 1, 3, 4, 5, 6, 7, 74, 109, 110]
v_row1 = vintage_knobs[:6]
v_row2 = vintage_knobs[6:]
v_rows_top = sw_y + TAB_H
v_gap = ((vb - PAD) - v_rows_top - 2 * KNOB_H) / 3.35
for row, params in enumerate((v_row1, v_row2)):
    cols = len(params)
    kw = (inner_v - (cols - 1) * ROW_GAP) / cols
    y = v_rows_top + v_gap + row * (KNOB_H + v_gap)
    for i, p in enumerate(params):
        x = vl + PAD + i * (kw + ROW_GAP)
        rects[p] = box(x, y, x + kw, y + KNOB_H)

# GLOBAL — two rows: the three voice knobs with POLY and LEG beside them, then DET, MWA and
# the MWD menu.
#
# Both rows divide their own span into EQUAL columns. The first row used to size its knobs from
# whatever was left after the two menus took their label minimum, which pinched the knobs to
# 29.7 px against the menus' 42 px, and the second row's knobs came out 73 px -- more than
# double the first row's, so the two rows read as unrelated grids. Equal columns per row make
# each row internally even and both rows edge-aligned.
#
# The title gap is weighted heavier than the other gaps: a section title with its first
# control crowding it reads as two titles on top of each other.
gl, gt, gr, gb = PANELS["kPanelGlobal"]
inner = (gr - gl) - 2 * PAD
left = gl + PAD
glob_top = content_top(gt)
glob_bottom = gb - PAD
GLOB_TITLE_WEIGHT = 1.8
glob_unit = (glob_bottom - glob_top - 2 * KNOB_H) / (2 + GLOB_TITLE_WEIGHT)
y1 = glob_top + glob_unit * GLOB_TITLE_WEIGHT
y2 = y1 + KNOB_H + glob_unit

# Row 1: five equal columns -- VOL, GLIDE, UNI, then the POLY and LEG menus.
row1 = (0, 11, 12, 10, 55)
cw1 = (inner - (len(row1) - 1) * ROW_GAP) / len(row1)
for i, p in enumerate(row1):
    x = left + i * (cw1 + ROW_GAP)
    if p in (10, 55):
        # Menus are shorter than knobs; centre them on the row so their tops and bottoms match.
        my = y1 + (KNOB_H - DROPDOWN_H) / 2
        rects[p] = box(x, my, x + cw1, my + DROPDOWN_H)
    else:
        rects[p] = box(x, y1, x + cw1, y1 + KNOB_H)

# Row 2: three equal columns -- DET, MWA, MWD.
row2 = (13, 52, 51)
cw2 = (inner - (len(row2) - 1) * ROW_GAP) / len(row2)
for i, p in enumerate(row2):
    x = left + i * (cw2 + ROW_GAP)
    if p == 51:
        my = y2 + (KNOB_H - DROPDOWN_H) / 2
        rects[p] = box(x, my, x + cw2, my + DROPDOWN_H)
    else:
        rects[p] = box(x, y2, x + cw2, y2 + KNOB_H)

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

# The two page buttons, directly under MASTER and sharing its width so they read as a stack:
# MASTER, then SAMPLER, then WAVETABLE. They open full-width pages that slide in over the
# panels; see KeithPageButton and the page overlay in the editor.
#
# Sized from MASTER rather than from a fixed number so that if MASTER is ever resized these
# follow it, and the column stays aligned.
BUTTON_H = TAB_H
place_overlay("sampler_button", master_cx - MASTER_W / 2, master_y + MASTER_H + _r(18),
              master_cx + MASTER_W / 2, master_y + MASTER_H + _r(18) + BUTTON_H)
place_overlay("wavetable_button", master_cx - MASTER_W / 2,
              master_y + MASTER_H + _r(18) + BUTTON_H + _r(12),
              master_cx + MASTER_W / 2,
              master_y + MASTER_H + _r(18) + BUTTON_H * 2 + _r(12))



# ---------------------------------------------------------------------------------------
# THE SAMPLER / WAVETABLE PAGE
#
# One page covering the panel area, opened by the SAMPLER or WAVETABLE button on the rail. The
# page is split in two: the sampler above, the wavetable below. The panels underneath are NOT
# moved or hidden by this geometry -- the page draws over them, which is what keeps the panel
# symmetry work untouched by anything here.
#
# The page uses the width added when the canvas went to 1400 (x to 1370 rather than 1122), so it
# has room for a browser strip beside a wide waveform.
# ---------------------------------------------------------------------------------------
PAGE_L = _r(188)
PAGE_R = _r(1370)
PAGE_T = _r(18)
PAGE_B = _r(750)
PAGE_PAD = _r(14)

# Split. The sampler takes the taller half because it carries the waveform, the slice row, the
# knob row and the mode buttons. The split is measured from content rather than guessed: at
# 361/357 the two halves hold their rows with clearance, which is what the mock showed.
PAGE_SPLIT_GAP = _r(14)
SAMPLER_TOP = PAGE_T

# The sampler half's height is DERIVED from the rows it must hold, not guessed. An earlier version
# used a round number and the mode row fell 106 units past the bottom of its own page -- it escaped
# into the wavetable half, and the page-overlap check is what caught it.
#
# Each entry is (name, height). Kept as data so the total and the layout below cannot drift apart:
# if a row's height changes here, the split follows automatically.
SL_TITLE_H_D = 30
MODE_BTN_H_D = 22
MODE_BTN_GAP_D = 8
SL_AFTER_MODES_GAP_D = 14
WAVE_H_D = 150
SL_AFTER_WAVE_GAP_D = 10
SLICE_ROW_H_D = 15
SL_AFTER_SLICES_GAP_D = 14
# The knob row is the knob's own height PLUS the label row beneath it. The label is drawn by the
# editor, not by the geometry, so it has to be budgeted here or the next row lands on top of it.
# That is exactly what happened: at 64 the budget fitted the knobs but not their labels, and the
# warp row overlapped START and END by 8 units.
PAGE_KNOB_H_D = 48
PAGE_KNOB_LABEL_H_D = 14
KNOB_ROW_H_D = PAGE_KNOB_H_D + PAGE_KNOB_LABEL_H_D
SL_AFTER_KNOBS_GAP_D = 10
MODE_ROW_H_D = 16
SL_BOTTOM_MARGIN_D = 8

SL_CONTENT_H_D = (
    SL_TITLE_H_D + MODE_BTN_H_D * 2 + MODE_BTN_GAP_D + SL_AFTER_MODES_GAP_D
    + WAVE_H_D + SL_AFTER_WAVE_GAP_D
    + SLICE_ROW_H_D + SL_AFTER_SLICES_GAP_D
    + KNOB_ROW_H_D + SL_AFTER_KNOBS_GAP_D
    + MODE_ROW_H_D + SL_BOTTOM_MARGIN_D
)

SAMPLER_BOTTOM = SAMPLER_TOP + _r(SL_CONTENT_H_D)
WAVETABLE_TOP = SAMPLER_BOTTOM + PAGE_SPLIT_GAP
WAVETABLE_BOTTOM = PAGE_B

# Fail here rather than emitting a page that spills. The page-overlap check would also catch it,
# but it reports the symptom (an escaping row) and this reports the cause (the split is too small).
if WAVETABLE_TOP >= WAVETABLE_BOTTOM:
    raise SystemExit(
        f"the sampler half ({SL_CONTENT_H_D} units of content) leaves no room for the wavetable "
        f"half; the page is {PAGE_B - PAGE_T} units tall")

place_overlay("page_sampler", PAGE_L, SAMPLER_TOP, PAGE_R, SAMPLER_BOTTOM)
place_overlay("page_wavetable", PAGE_L, WAVETABLE_TOP, PAGE_R, WAVETABLE_BOTTOM)

# --- the page band must not sit on top of the main view's controls ----------------------------
#
# The pages are drawn LAST, so anything they overlap is painted over. page_wt_frames was placed at
# y 332..392 while preset_menu sits at y 362..390 -- the page covered the sound-bank dropdown
# completely, and the owner reported that dropdown as unclickable. Nothing compared the page band
# against the controls already living in that region, so the generator emitted the collision.
#
# The main view's controls are placed from y0 down to a known row. Rather than guess that extent,
# check the page's top against the lowest control the main view places, named explicitly so the
# failure says which control is buried.
MAIN_VIEW_CONTROLS = ("preset_menu", "pad_midi_switch", "osc1_waveform", "osc2_waveform",
                      "osc3_waveform", "lfo1_label", "lfo2_label")

for name in MAIN_VIEW_CONTROLS:
    rect = OVERLAYS.get(name)
    if rect is None:
        continue
    l, t, r, b = rect
    for page in ("page_sampler", "page_wavetable"):
        pr = OVERLAYS[page]
        # A page fully covering a control is the bug. Partial x-overlap alone is fine; it is the
        # vertical band that matters, because the page spans the full content width.
        if pr[0] <= l and pr[2] >= r and pr[1] <= t and pr[3] >= b:
            raise SystemExit(
                f"{page} ({pr[0]:.0f},{pr[1]:.0f},{pr[2]:.0f},{pr[3]:.0f}) completely covers "
                f"{name} ({l:.0f},{t:.0f},{r:.0f},{b:.0f}). The pages are painted last, so "
                f"{name} would be invisible and unclickable. Move the page's top below "
                f"{b:.0f}, or move {name} out of the page band.")

# --- inside the sampler half -----------------------------------------------------------------
SL_TITLE_H = _r(SL_TITLE_H_D)
# PLAY and SLICE: directly under the SAMPLER title, full width, stacked. The owner asked for
# exactly this -- "both be long directly under sampler on sampler page" -- so they are the page's
# first controls and they span it edge to edge rather than sitting in a column.
MODE_BTN_H = _r(MODE_BTN_H_D)
MODE_BTN_GAP = _r(MODE_BTN_GAP_D)
mode_l = PAGE_L + PAGE_PAD
mode_r = PAGE_R - PAGE_PAD

place_overlay("page_sample_play", mode_l, SAMPLER_TOP + SL_TITLE_H,
              mode_r, SAMPLER_TOP + SL_TITLE_H + MODE_BTN_H)
place_overlay("page_sample_slice", mode_l, SAMPLER_TOP + SL_TITLE_H + MODE_BTN_H + MODE_BTN_GAP,
              mode_r, SAMPLER_TOP + SL_TITLE_H + MODE_BTN_H * 2 + MODE_BTN_GAP)

# The rest of the sampler half, laid out from the mode buttons downward.
SL_BODY_TOP = SAMPLER_TOP + SL_TITLE_H + MODE_BTN_H * 2 + MODE_BTN_GAP + _r(SL_AFTER_MODES_GAP_D)
SL_BROWSER_W = _r(240)

place_overlay("page_sample_browser", mode_l, SL_BODY_TOP, mode_l + SL_BROWSER_W, SAMPLER_BOTTOM - PAGE_PAD)
place_overlay("page_sample_wave", mode_l + SL_BROWSER_W + PAGE_PAD, SL_BODY_TOP,
              mode_r, SL_BODY_TOP + _r(WAVE_H_D))
place_overlay("page_sample_slices", mode_l + SL_BROWSER_W + PAGE_PAD,
              SL_BODY_TOP + _r(WAVE_H_D + SL_AFTER_WAVE_GAP_D),
              mode_r, SL_BODY_TOP + _r(WAVE_H_D + SL_AFTER_WAVE_GAP_D + SLICE_ROW_H_D))

# The control row: start, end, length, root, tune, gain. Six equal columns, which is the shape
# the panel knobs use, so the page reads as part of the same instrument.
knob_top = SL_BODY_TOP + _r(WAVE_H_D + SL_AFTER_WAVE_GAP_D + SLICE_ROW_H_D + SL_AFTER_SLICES_GAP_D)
page_knob_gap = _r(10)
avail = (mode_r - (mode_l + SL_BROWSER_W + PAGE_PAD)) - page_knob_gap * 5
page_knob_w = avail / 6
for idx, name in enumerate(("start", "end", "length", "root", "tune", "gain")):
    kx = mode_l + SL_BROWSER_W + PAGE_PAD + idx * (page_knob_w + page_knob_gap)
    # The rect is the KNOB only. Its label is drawn 14 units below by the editor, and that band is
    # budgeted separately in KNOB_ROW_H_D. This was +64, which overflowed the knob by 16 units and
    # swallowed the label band -- so the arithmetic looked right while the warp row below landed on
    # top of the knob labels.
    place_overlay(f"page_knob_{name}", kx, knob_top, kx + page_knob_w,
                  knob_top + _r(PAGE_KNOB_H_D))

# The mode row sits under the knobs: warp and tempo sync.
mode_row_top = knob_top + _r(KNOB_ROW_H_D + SL_AFTER_KNOBS_GAP_D)
place_overlay("page_sample_warp", mode_l + SL_BROWSER_W + PAGE_PAD, mode_row_top,
              mode_l + SL_BROWSER_W + PAGE_PAD + _r(120), mode_row_top + _r(MODE_ROW_H_D))
place_overlay("page_sample_temposync", mode_l + SL_BROWSER_W + PAGE_PAD + _r(130), mode_row_top,
              mode_l + SL_BROWSER_W + PAGE_PAD + _r(270), mode_row_top + _r(MODE_ROW_H_D))

# REVERSE, beside TEMPO SYNC. Same row, same height, so the three read as one group of sampler
# switches rather than as separate features.
place_overlay("page_sample_reverse", mode_l + SL_BROWSER_W + PAGE_PAD + _r(286), mode_row_top,
              mode_l + SL_BROWSER_W + PAGE_PAD + _r(402), mode_row_top + _r(MODE_ROW_H_D))

# --- inside the wavetable half ---------------------------------------------------------------
WT_TITLE_H = _r(30)
WT_BODY_TOP = WAVETABLE_TOP + WT_TITLE_H
WT_BANKS_W = _r(240)

place_overlay("page_wt_banks", PAGE_L + PAGE_PAD, WT_BODY_TOP,
              PAGE_L + PAGE_PAD + WT_BANKS_W, WAVETABLE_BOTTOM - PAGE_PAD)

# The wavetable half is banks | frames+view | controls, three columns. The controls get their own
# column at the right rather than being laid over the frame view -- an earlier version put MORPH
# and the OSC 3 SOURCE switch ON TOP of the large frame display, because the view spanned the
# whole width and the controls were placed after it in the same space.
WT_COL_L = PAGE_L + PAGE_PAD + WT_BANKS_W + PAGE_PAD
WT_CTRL_W = _r(210)
WT_RIGHT = PAGE_R - PAGE_PAD
WT_VIEW_R = WT_RIGHT - WT_CTRL_W - PAGE_PAD

place_overlay("page_wt_frames", WT_COL_L, WT_BODY_TOP, WT_VIEW_R, WT_BODY_TOP + _r(80))
place_overlay("page_wt_view", WT_COL_L, WT_BODY_TOP + _r(90), WT_VIEW_R,
              WAVETABLE_BOTTOM - PAGE_PAD)
place_overlay("page_wt_controls", WT_RIGHT - WT_CTRL_W, WT_BODY_TOP,
              WT_RIGHT, WAVETABLE_BOTTOM - PAGE_PAD)

# The controls column, laid out INSIDE its own rect rather than at offsets from the page.
#
# The column is 210 units wide and holds a knob and a pair of tabs. Two tabs side by side need
# twice a tab's width, so the tab width is DERIVED as half the available width minus the gap --
# not a fixed number. A fixed 84 units per tab was 174 units total in a 158-unit column, which put
# the OSC 3 SOURCE tabs off the right edge of the page. Deriving the width means the tabs cannot
# exceed the column no matter how the column is resized.
WT_COL_L = WT_RIGHT - WT_CTRL_W
WT_COL_R = WT_RIGHT
WT_COL_PAD = _r(12)
WT_INNER_L = WT_COL_L + WT_COL_PAD
WT_INNER_R = WT_COL_R - WT_COL_PAD
WT_INNER_W = WT_INNER_R - WT_INNER_L

WT_ROW_GAP = _r(14)
WT_TITLE_H = _r(14)

# Morph: the knob that walks the frames. The one control in this column that is a real parameter.
wt_morph_title_y = WT_BODY_TOP + _r(10)
place_overlay("page_wt_morph_label", WT_INNER_L, wt_morph_title_y,
              WT_INNER_R, wt_morph_title_y + WT_TITLE_H)
WT_MORPH_KNOB_H = _r(62)
wt_morph_y = wt_morph_title_y + WT_TITLE_H + _r(4)
place_overlay("page_wt_morph", WT_INNER_L, wt_morph_y, WT_INNER_R, wt_morph_y + WT_MORPH_KNOB_H)

# Osc 3 source: SHAPES or TABLE. Two tabs, width derived from the column.
wt_src_title_y = wt_morph_y + WT_MORPH_KNOB_H + WT_ROW_GAP
place_overlay("page_wt_src_label", WT_INNER_L, wt_src_title_y,
              WT_INNER_R, wt_src_title_y + WT_TITLE_H)
WT_TAB_H = _r(22)
WT_TAB_GAP = _r(8)
WT_TAB_W = (WT_INNER_W - WT_TAB_GAP) / 2
wt_tabs_y = wt_src_title_y + WT_TITLE_H + _r(4)
place_overlay("page_wt_tab_shapes", WT_INNER_L, wt_tabs_y,
              WT_INNER_L + WT_TAB_W, wt_tabs_y + WT_TAB_H)
place_overlay("page_wt_tab_table", WT_INNER_L + WT_TAB_W + WT_TAB_GAP, wt_tabs_y,
              WT_INNER_R, wt_tabs_y + WT_TAB_H)

# A guard, so a future column resize cannot silently push the tabs past the page edge. The
# overlap check would catch the spill, but it reports the symptom; this names the cause.
if WT_INNER_L + WT_TAB_W + WT_TAB_GAP + WT_TAB_W > WT_COL_R + 0.001:
    raise SystemExit(
        f"the OSC 3 SOURCE tabs do not fit the controls column: "
        f"2 x {WT_TAB_W:.1f} + {WT_TAB_GAP:.1f} gap needs "
        f"{WT_TAB_W * 2 + WT_TAB_GAP:.1f} but the column is {WT_COL_R - WT_COL_L:.1f} wide")

place_overlay("midi_hint1", lr_l + PAD, _r(944), lr_r - PAD, _r(962))
place_overlay("midi_hint2", lr_l + PAD, _r(962), lr_r - PAD, _r(980))

# FX — three slot menus on top, then two rows of six knobs.
#
# Both the menus and the knobs are sized from the panel inner width and laid out across all
# of it, so the left and right margins match and the columns line up: the menus are three
# equal thirds and each knob row is six equal sixths of the same span. Previously the knobs
# took their width from min_control_width, which capped them at 30 px and left 52.5 px of
# dead space on the right of every row.
fxl, fxt, fxr, fxb = PANELS["kPanelFx"]
inner_fx = (fxr - fxl) - 2 * PAD
left_fx = fxl + PAD
right_fx = fxr - PAD
fx_menu_h = DROPDOWN_H_COMPACT
# Spread the panel's slack evenly: the gaps above the menus, between the menus and the first
# knob row, and between the two knob rows are all the same height, so the block sits centred
# rather than pushed against the title.
fx_usable = (fxb - PAD) - content_top(fxt)
fx_gap = (fx_usable - fx_menu_h - 2 * KNOB_H) / 4
menu_y = content_top(fxt) + fx_gap
menu_w = (inner_fx - 2 * fx_gap) / 3
for i, p in enumerate((56, 62, 68)):
    x = left_fx + i * (menu_w + fx_gap)
    rects[p] = box(x, menu_y, x + menu_w, menu_y + fx_menu_h)

all_fx_knobs = [57, 58, 59, 60, 63, 64, 65, 66, 69, 70, 71, 72]
fx_row1_y = menu_y + fx_menu_h + fx_gap
fx_row2_y = fx_row1_y + KNOB_H + fx_gap
knob_w = (inner_fx - 5 * fx_gap) / 6
for row_idx, knobs in enumerate((all_fx_knobs[:6], all_fx_knobs[6:])):
    y = fx_row1_y if row_idx == 0 else fx_row2_y
    for i, p in enumerate(knobs):
        x = left_fx + i * (knob_w + fx_gap)
        rects[p] = box(x, y, x + knob_w, y + KNOB_H)

# Overlay vs control overlaps.
#
# A page overlay is EXEMPT, and deliberately so: the sampler/wavetable page is a full-area surface
# that draws OVER the panels, so overlapping them is the whole point rather than a bug. Without
# this exemption the check fires 183 times on a correct layout and aborts before writing anything,
# which is how the page geometry silently failed to reach the header.
#
# The exemption is by name prefix, not by size: "page_" is the convention for a page surface. An
# unrelated overlay that happens to be large is still checked.
PAGE_OVERLAY_PREFIX = "page_"

overlay_conflicts = []
page_overlays = []
for oid, r in OVERLAYS.items():
    if oid.startswith(PAGE_OVERLAY_PREFIX):
        page_overlays.append(oid)
        continue
    for p, cr in rects.items():
        if overlaps(r, cr):
            overlay_conflicts.append(f"overlay {oid} {r} overlaps param {p} {cr}")
if overlay_conflicts:
    print(f"{len(overlay_conflicts)} overlay conflicts:", flush=True)
    print("\n".join(overlay_conflicts), flush=True)
    raise SystemExit(1)

# Page overlays nest: the sampler page contains its waveform, its browser, its mode buttons. So
# the rule is not "pages must not overlap" -- it is "a page surface must not cross OUTSIDE its
# parent page". That is what actually breaks: a control that spills past its page would be drawn
# over the wrong half of the window, or clipped, depending on the drawing order.
#
# The parent of each element is the SMALLEST page that contains it. An element that no page
# contains is a top-level page, and top-level pages must not overlap each other.
page_boxes = {oid: OVERLAYS[oid] for oid in page_overlays}

def contains(outer, inner):
    return (outer[0] <= inner[0] and outer[1] <= inner[1]
            and outer[2] >= inner[2] and outer[3] >= inner[3])

tops = []
for oid in page_overlays:
    r = page_boxes[oid]
    parents = [o for o, pr in page_boxes.items() if o != oid and contains(pr, r)]
    if not parents:
        tops.append(oid)
        continue
    # The smallest containing page is the direct parent. Equality is impossible here because a
    # page cannot properly contain an identical twin, and an identical pair would be a real bug.
    parent = min(parents, key=lambda o: (page_boxes[o][2] - page_boxes[o][0])
                 * (page_boxes[o][3] - page_boxes[o][1]))
    if not contains(page_boxes[parent], r):
        print(f"page overlay {oid} escapes its parent {parent}", flush=True)
        raise SystemExit(1)

for i, a in enumerate(tops):
    for b in tops[i + 1:]:
        if overlaps(page_boxes[a], page_boxes[b]):
            print(f"top-level pages overlap: {a} {page_boxes[a]} and {b} {page_boxes[b]}",
                  flush=True)
            raise SystemExit(1)

print(f"{len(page_overlays)} page overlays: {len(tops)} top-level ({', '.join(tops)}), "
      f"{len(page_overlays) - len(tops)} nested", flush=True)
if page_overlays:
    print(f"{len(page_overlays)} page overlays, exempt from the control-overlap check: "
          f"{', '.join(page_overlays)}", flush=True)

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


# Controls that are deliberately drawn in a panel's title band, on the right, rather than
# below it. LOADED SOUNDS puts its mute and solo tabs up there so the preset menu can use the
# full panel width. Anything listed here is exempt from the title-band rule.
TITLE_BAND_TABS = {82, 83}

for p, r in rects.items():
    if r[2] > CANVAS[2] or r[3] > CANVAS[3] or r[0] < 0 or r[1] < 0:
        raise SystemExit(f"param {p} out of canvas: {r}")
    for pname, pb in panel_boxes.items():
        if overlaps(r, pb):
            band = title_band(pname)
            if overlaps(r, band) and p not in TITLE_BAND_TABS:
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
