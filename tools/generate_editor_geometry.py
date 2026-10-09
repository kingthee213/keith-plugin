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
    share = (inner_w - (cols - 1) * gap) / cols
    return max(share, max(min_control_width(p) for p in params))

TITLE_BAND = _rf(28)
VISIBLE_ROW_GAP = _rf(14)
CANVAS = (0, 0, _r(1536), _r(1024))

_PANELS_REF = {
    "kPanelOsc": (250, 24, 552, 416),
    "kPanelFilter": (562, 24, 724, 416),
    "kPanelAmp": (728, 24, 928, 416),
    "kPanelLfo": (938, 24, 1240, 416),
    "kPanelMod": (1250, 24, 1500, 416),
    "kPanelArt": (258, 430, 808, 732),
    "kPanelAmpMods": (734, 432, 980, 646),
    "kPanelSample": (988, 426, 1256, 700),
    "kPanelPads": (1264, 426, 1500, 736),
    "kPanelVintage": (256, 724, 730, 996),
    "kPanelGlobal": (744, 724, 1066, 996),
    "kPanelFx": (1080, 724, 1500, 1000),
    "kLeftRail": (0, 0, 246, 1024),
    "kWordmarkBox": (18, 28, 228, 118),
    "kKeyboardBox": (358.0, 1002.0, 1492.0, 1020.0),
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
    15: ("TUNE", "knob", 2448), 16: ("LVL", "knob", 1836), 17: ("PW", "knob", 1224),
    18: ("SHP", "knob", 1836), 19: ("TUNE", "knob", 2448), 20: ("LVL", "knob", 1836),
    21: ("PW", "knob", 1224), 22: ("SUB", "knob", 1836), 23: ("NOISE", "knob", 3060),
    24: ("FM", "knob", 1224), 25: ("TYPE", "tab", 2448), 26: ("CUT", "knob", 1836),
    27: ("RES", "knob", 1836), 28: ("ENV", "knob", 1836), 29: ("ATT", "knob", 1836),
    30: ("DEC", "knob", 1836), 31: ("SUS", "knob", 1836), 32: ("REL", "knob", 1836),
    33: ("PATK", "knob", 1836), 34: ("PDEC", "knob", 1836), 35: ("PSUS", "knob", 1836),
    36: ("PREL", "knob", 1836), 37: ("PAMT", "knob", 1836), 38: ("SYN", "knob", 1836),
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
osc_l, osc_t, osc_r, osc_b = PANELS["kPanelOsc"]
ix, iy = osc_l + PAD, content_top(osc_t)
wave_h = _r(44)
place_overlay("osc1_waveform", osc_l + PAD, iy, osc_l + PAD + _r(112), iy + wave_h)
place_overlay("osc2_waveform", osc_l + PAD + _r(118), iy, osc_l + PAD + _r(230), iy + wave_h)
iy += wave_h + ROW_GAP
kw = shared_knob_width([14, 15, 16, 17, 18, 19, 20, 21], osc_r - osc_l - 2 * PAD - ROW_GAP, 4)
for col, params in enumerate([(14, 15, 16, 17), (18, 19, 20, 21)]):
    cx = ix + col * (kw * 2 + ROW_GAP)
    for row, pair in enumerate([(params[0], params[1]), (params[2], params[3])]):
        y = iy + row * (KNOB_H + ROW_GAP)
        rects[pair[0]] = box(cx, y, cx + kw, y + KNOB_H)
        rects[pair[1]] = box(cx + kw + ROW_GAP, y, cx + 2 * kw + ROW_GAP, y + KNOB_H)
mix_y = iy + 2 * (KNOB_H + ROW_GAP)
mix_w = (osc_r - osc_l - 2 * PAD - 2 * ROW_GAP) / 3
for i, p in enumerate([22, 23, 24]):
    rects[p] = box(ix + i * (mix_w + ROW_GAP), mix_y, ix + i * (mix_w + ROW_GAP) + mix_w, mix_y + KNOB_H)
rects[43] = box(ix, osc_b - PAD - TAB_H, ix + _rf(76), osc_b - PAD)
rects[44] = box(ix + _rf(84), osc_b - PAD - TAB_H, ix + _rf(160), osc_b - PAD)

# FILTER
fl, ft, fr, fb = PANELS["kPanelFilter"]
kw_filt = shared_knob_width([26, 27, 28, 45, 53, 54], fr - fl - 2 * PAD - ROW_GAP, 2)
rects[25] = box(fl + PAD, content_top(ft), fr - PAD, content_top(ft) + TAB_H)
y_filt = content_top(ft) + TAB_H + ROW_GAP
for row_params in ([26, 27], [28, 45], [53, 54]):
    for i, p in enumerate(row_params):
        x = fl + PAD + i * (kw_filt + ROW_GAP)
        rects[p] = box(x, y_filt, x + kw_filt, y_filt + KNOB_H)
    y_filt += KNOB_H + ROW_GAP

# AMP — 2x2 amp ADSR and 2x2 filter ADSR so values fit at MIN_W.
al, at, ar, ab = PANELS["kPanelAmp"]
y_amp = content_top(at) + ROW_GAP
inner_amp = ar - al - 2 * PAD
kw_amp = shared_knob_width([29, 30, 31, 32, 39, 40, 41, 42], inner_amp - ROW_GAP, 2)
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

# AMP MODIFIERS — two columns; compact height fits the strip beside the hero.
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

# LFO — one horizontal row per block (label, rate, depth, shape, dest).
ll, lt, lr, lb = PANELS["kPanelLfo"]
LFO_LABEL_H = _rf(14)
LFO_DD_W = _rf(52)


def lfo_block(y_top, rate, shape, dest, amount, label_id):
    place_overlay(label_id, ll + PAD, y_top, lr - PAD, y_top + LFO_LABEL_H)
    cy = y_top + LFO_LABEL_H + ROW_GAP
    inner = lr - ll - 2 * PAD
    dd_w = max(MIN_W, LFO_DD_W, min_control_width(48), min_control_width(49), min_control_width(86), min_control_width(87))
    knob_w = shared_knob_width([47, 50, 85, 88], inner - 2 * dd_w - 3 * ROW_GAP, 2)
    x = ll + PAD
    rects[rate] = box(x, cy, x + knob_w, cy + KNOB_H)
    x += knob_w + ROW_GAP
    rects[amount] = box(x, cy, x + knob_w, cy + KNOB_H)
    x += knob_w + ROW_GAP
    dd_y = cy + (KNOB_H - DROPDOWN_H_COMPACT) / 2
    rects[shape] = box(x, dd_y, x + dd_w, dd_y + DROPDOWN_H_COMPACT)
    x += dd_w + ROW_GAP
    rects[dest] = box(x, dd_y, x + dd_w, dd_y + DROPDOWN_H_COMPACT)


lfo_row_h = LFO_LABEL_H + ROW_GAP + KNOB_H
y1 = content_top(lt)
lfo_block(y1, 47, 48, 49, 50, "lfo1_label")
y2 = y1 + lfo_row_h + ROW_GAP
lfo_block(y2, 85, 86, 87, 88, "lfo2_label")

# MOD
ml, mt, mr, mb = PANELS["kPanelMod"]
row_w = mr - ml - 2 * PAD
slot_h = (mb - content_top(mt) - PAD) / 4
for slot, (src, dst, amt) in enumerate([(89, 90, 91), (92, 93, 94), (95, 96, 97), (98, 99, 100)]):
    y = content_top(mt) + slot * slot_h + PAD
    dw = (row_w - MIN_W - ROW_GAP) / 2
    rects[src] = box(ml + PAD, y, ml + PAD + dw, y + DROPDOWN_H)
    rects[dst] = box(ml + PAD + dw + ROW_GAP, y, ml + PAD + 2 * dw + ROW_GAP, y + DROPDOWN_H)
    rects[amt] = box(mr - PAD - MIN_W, y, mr - PAD, y + KNOB_H)

# SAMPLE — knobs anchor from the panel bottom so they never spill into the global row.
sl, st, sr, sb = PANELS["kPanelSample"]
inner_w = sr - sl - 2 * PAD
sample_knobs = [33, 34, 35, 36, 37, 84]
kw3 = shared_knob_width(sample_knobs, inner_w - 2 * ROW_GAP, 3)
knob_rows = 2
tab_row_h = TAB_H + ROW_GAP
y2 = sb - PAD - KNOB_H
y1 = y2 - ROW_GAP - KNOB_H
preset_h = _rf(36)
y0 = content_top(st)
wave_h = max(_rf(20), y1 - y0 - ROW_GAP * 2 - preset_h - tab_row_h)
place_overlay("sample_waveform", sl + PAD, y0, sr - PAD, y0 + wave_h)
y_preset = y0 + wave_h + ROW_GAP
place_overlay("preset_menu", sl + PAD, y_preset, sr - PAD, y_preset + preset_h)
y1 = max(y1, y_preset + preset_h + ROW_GAP)
y2 = y1 + KNOB_H + ROW_GAP
for i, param in enumerate(sample_knobs):
    row, col = divmod(i, 3)
    y = y1 + row * (KNOB_H + ROW_GAP)
    x = sl + PAD + col * (kw3 + ROW_GAP)
    rects[param] = box(x, y, x + kw3, y + KNOB_H)
tab_y = y1 + knob_rows * (KNOB_H + ROW_GAP)
tab_w = min(_rf(72), kw3)
rects[82] = box(sl + PAD, tab_y, sl + PAD + tab_w, tab_y + TAB_H)
rects[83] = box(sl + PAD + tab_w + ROW_GAP, tab_y, sl + PAD + 2 * tab_w + ROW_GAP, tab_y + TAB_H)

# PADS
pl, pt, pr, pb = PANELS["kPanelPads"]
grid_r = pr - PAD - 3 * _rf(40) - 2 * ROW_GAP
rects[75] = box(pl + PAD, content_top(pt), grid_r - ROW_GAP, pb - PAD)
kw = max(MIN_W, max(min_control_width(p) for p in [76, 77, 78, 79, 80, 81]))
dy = content_top(pt) + ROW_GAP
for row, trio in enumerate([(76, 77, 78), (79, 80, 81)]):
    y = dy + row * (KNOB_H + ROW_GAP)
    for i, p in enumerate(trio):
        rects[p] = box(grid_r + i * (kw + ROW_GAP), y, grid_r + i * (kw + ROW_GAP) + kw, y + KNOB_H)

# VINTAGE
vl, vt, vr, vb = PANELS["kPanelVintage"]
rects[9] = box(vl + PAD, content_top(vt), vl + PAD + _r(220), content_top(vt) + TAB_H)
knob_x = vl + _r(250)
knob_w_v = shared_knob_width([2, 46, 1], vr - knob_x - PAD - 2 * ROW_GAP, 3)
y_v = content_top(vt) + TAB_H + ROW_GAP
for i, p in enumerate([2, 46, 1]):
    rects[p] = box(knob_x + i * (knob_w_v + ROW_GAP), y_v, knob_x + i * (knob_w_v + ROW_GAP) + knob_w_v, y_v + KNOB_H)

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
bottom = [13, 38, 52]
bw = (inner - drop_w - ROW_GAP - (len(bottom) - 1) * ROW_GAP) / len(bottom)
x = gl + PAD
for p in bottom:
    rects[p] = box(x, knob_y, x + bw, knob_y + KNOB_H)
    x += bw + ROW_GAP
rects[51] = box(x, knob_y, x + drop_w, knob_y + DROPDOWN_H)

# Left rail
lr_l, _lr_t, lr_r, _lr_b = PANELS["kLeftRail"]
tape_items = [3, 4, 5, 6, 7, 74]
y = _r(168)
for p in tape_items:
    rects[p] = box(lr_l + PAD, y, lr_r - PAD, y + KNOB_H)
    y += KNOB_H + ROW_GAP
place_overlay("bypass_label", lr_l + PAD, y, lr_r - PAD, y + _rf(16))
bypass_y = y + _rf(18)
rects[8] = box(lr_l + PAD, bypass_y, lr_r - PAD, bypass_y + TAB_H)
y = bypass_y + TAB_H + ROW_GAP
rw = (lr_r - lr_l - 2 * PAD - ROW_GAP) / 2
rects[109] = box(lr_l + PAD, y, lr_l + PAD + rw, y + KNOB_H)
rects[110] = box(lr_l + PAD + rw + ROW_GAP, y, lr_r - PAD, y + KNOB_H)
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
