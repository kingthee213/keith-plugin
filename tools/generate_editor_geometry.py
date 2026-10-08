#!/usr/bin/env python3
"""Generate EditorGeometry.h control rects for W7 reference grid (1536x1024)."""
from __future__ import annotations

LABEL_FONT_PX = 11.0
KNOB_H = 34 + 19 + 20
KNOB_H_COMPACT = 47
KNOB_GAP = 14
TAB_H = 28
DROPDOWN_H = 52
DROPDOWN_H_COMPACT = 36
FADER_H = 96
FADER_W = 54
MIN_W = 42
PAD = 18
ROW_GAP = 14
TITLE_BAND = 28
VISIBLE_ROW_GAP = 14
CANVAS = (0, 0, 1536, 1024)
KEYBOARD = (358.0, 1002.0, 1492.0, 1020.0)

PANELS = {
    "kPanelOsc": (250, 24, 654, 416),
    "kPanelFilter": (664, 24, 896, 416),
    "kPanelAmp": (906, 24, 1106, 416),
    "kPanelLfo": (1116, 24, 1240, 416),
    "kPanelMod": (1250, 24, 1500, 416),
    "kPanelAmpMods": (734, 432, 868, 728),
    "kPanelSample": (884, 426, 1256, 736),
    "kPanelPads": (1264, 426, 1500, 736),
    "kPanelVintage": (256, 756, 730, 996),
    "kPanelGlobal": (744, 756, 1066, 996),
    "kPanelFx": (1080, 756, 1500, 996),
    "kLeftRail": (0, 0, 246, 1024),
    "kWordmarkBox": (18, 28, 228, 118),
    "kKeyboardBox": KEYBOARD,
}

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
    kw = knob_w or min(64, (r - l - KNOB_GAP * (cols - 1)) / cols)
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
    x = l
    for param in items:
        rects[param] = box(x, t, x + w, t + h)
        x += w + gap



def place_overlay(oid: str, l, t, r, b):
    OVERLAYS[oid] = box(l, t, r, b)


# OSC
osc_l, osc_t, osc_r, osc_b = PANELS["kPanelOsc"]
ix, iy = osc_l + PAD, content_top(osc_t)
wave_h = 44
place_overlay("osc1_waveform", osc_l + PAD, iy, osc_l + PAD + 112, iy + wave_h)
place_overlay("osc2_waveform", osc_l + PAD + 118, iy, osc_l + PAD + 230, iy + wave_h)
iy += wave_h + ROW_GAP
kw = max(MIN_W, (osc_r - osc_l - 2 * PAD - ROW_GAP) / 4)
for col, params in enumerate([(14, 15, 16, 17), (18, 19, 20, 21)]):
    cx = ix + col * (kw * 2 + ROW_GAP)
    for row, pair in enumerate([(params[0], params[1]), (params[2], params[3])]):
        y = iy + row * (KNOB_H + ROW_GAP)
        rects[pair[0]] = box(cx, y, cx + kw, y + KNOB_H)
        rects[pair[1]] = box(cx + kw + ROW_GAP, y, cx + 2 * kw + ROW_GAP, y + KNOB_H)
mix_y = iy + 2 * (KNOB_H + ROW_GAP) + ROW_GAP
mix_w = (osc_r - osc_l - 2 * PAD - 2 * ROW_GAP) / 3
for i, p in enumerate([22, 23, 24]):
    rects[p] = box(ix + i * (mix_w + ROW_GAP), mix_y, ix + i * (mix_w + ROW_GAP) + mix_w, mix_y + KNOB_H)
rects[43] = box(ix, osc_b - PAD - TAB_H, ix + 76, osc_b - PAD)
rects[44] = box(ix + 84, osc_b - PAD - TAB_H, ix + 160, osc_b - PAD)

# FILTER
fl, ft, fr, fb = PANELS["kPanelFilter"]
kw_filt = max(MIN_W, (fr - fl - 2 * PAD - ROW_GAP) / 2)
rects[25] = box(fl + PAD, content_top(ft), fr - PAD, content_top(ft) + TAB_H)
y_filt = content_top(ft) + TAB_H + ROW_GAP
for row_params in ([26, 27], [28, 45], [53, 54]):
    for i, p in enumerate(row_params):
        x = fl + PAD + i * (kw_filt + ROW_GAP)
        rects[p] = box(x, y_filt, x + kw_filt, y_filt + KNOB_H)
    y_filt += KNOB_H + ROW_GAP

# AMP
al, at, ar, ab = PANELS["kPanelAmp"]
y_amp = content_top(at) + ROW_GAP
kw_amp = max(MIN_W, (ar - al - 2 * PAD - 3 * ROW_GAP) / 4)
for i, p in enumerate([29, 30, 31, 32]):
    rects[p] = box(al + PAD + i * (kw_amp + ROW_GAP), y_amp, al + PAD + i * (kw_amp + ROW_GAP) + kw_amp, y_amp + KNOB_H)
y_fa = y_amp + KNOB_H + ROW_GAP
for i, p in enumerate([39, 40, 41, 42]):
    rects[p] = box(al + PAD + i * (kw_amp + ROW_GAP), y_fa, al + PAD + i * (kw_amp + ROW_GAP) + kw_amp, y_fa + KNOB_H)

# AMP MODIFIERS
aml, amt, amr, amb = PANELS["kPanelAmpMods"]
amp_mods = [101, 102, 103, 104, 105, 106, 107, 108]
inner = amr - aml - 2 * PAD
mod_w = max(MIN_W, (inner - ROW_GAP) / 2)
row_y0 = content_top(amt)
for i, p in enumerate(amp_mods):
    row = i // 2
    col = i % 2
    y = row_y0 + row * (KNOB_H_COMPACT + ROW_GAP)
    x = aml + PAD + col * (mod_w + ROW_GAP)
    rects[p] = box(x, y, x + mod_w, y + KNOB_H_COMPACT)

# LFO
ll, lt, lr, lb = PANELS["kPanelLfo"]
inner_w = lr - ll - 2 * PAD


LFO_KNOB_H = 40
LFO_DD_H = 28


def lfo_block(y, rate, shape, dest, amount, label_id, label_y):
    place_overlay(label_id, ll + PAD, label_y, lr - PAD, label_y + 14)
    rects[rate] = box(ll + PAD, y, lr - PAD, y + LFO_KNOB_H)
    rects[amount] = box(ll + PAD, y + LFO_KNOB_H + ROW_GAP, lr - PAD, y + 2 * LFO_KNOB_H + ROW_GAP)
    dy = y + 2 * (LFO_KNOB_H + ROW_GAP)
    rects[shape] = box(ll + PAD, dy, lr - PAD, dy + LFO_DD_H)
    rects[dest] = box(ll + PAD, dy + LFO_DD_H + ROW_GAP, lr - PAD, dy + 2 * LFO_DD_H + ROW_GAP)


block_h = 2 * LFO_KNOB_H + 2 * LFO_DD_H + 3 * ROW_GAP
y1 = content_top(lt) + 16
lfo_block(y1, 47, 48, 49, 50, "lfo1_label", content_top(lt) + 2)
y2 = y1 + block_h + ROW_GAP
lfo_block(y2, 85, 86, 87, 88, "lfo2_label", y2 - ROW_GAP)

# MOD
ml, mt, mr, mb = PANELS["kPanelMod"]
row_w = mr - ml - 2 * PAD
slot_h = (mb - content_top(mt) - PAD) / 4
for slot, (src, dst, amt) in enumerate([(89, 90, 91), (92, 93, 94), (95, 96, 97), (98, 99, 100)]):
    y = content_top(mt) + slot * slot_h + 4
    dw = (row_w - MIN_W - ROW_GAP) / 2
    rects[src] = box(ml + PAD, y, ml + PAD + dw, y + DROPDOWN_H)
    rects[dst] = box(ml + PAD + dw + ROW_GAP, y, ml + PAD + 2 * dw + ROW_GAP, y + DROPDOWN_H)
    rects[amt] = box(mr - PAD - MIN_W, y, mr - PAD, y + KNOB_H)

# SAMPLE
sl, st, sr, sb = PANELS["kPanelSample"]
inner_w = sr - sl - 2 * PAD
y0 = content_top(st)
place_overlay("sample_waveform", sl + PAD, y0, sr - PAD, y0 + 40)
y_preset = y0 + 40 + ROW_GAP
place_overlay("preset_menu", sl + PAD, y_preset, sr - PAD, y_preset + 44)
y1 = y_preset + 44 + ROW_GAP
kw4 = max(MIN_W, (inner_w - 3 * ROW_GAP) / 4)
for i, p in enumerate([33, 34, 35, 36]):
    rects[p] = box(sl + PAD + i * (kw4 + ROW_GAP), y1, sl + PAD + i * (kw4 + ROW_GAP) + kw4, y1 + KNOB_H)
y2 = y1 + KNOB_H + ROW_GAP
for i, p in enumerate([37, 84]):
    rects[p] = box(sl + PAD + i * (kw4 + ROW_GAP), y2, sl + PAD + i * (kw4 + ROW_GAP) + kw4, y2 + KNOB_H)
tab_y = y2 + (KNOB_H - TAB_H) / 2
tab_w = min(72, kw4)
rects[82] = box(sl + PAD + 2 * (kw4 + ROW_GAP), tab_y, sl + PAD + 2 * (kw4 + ROW_GAP) + tab_w, tab_y + TAB_H)
rects[83] = box(sl + PAD + 3 * (kw4 + ROW_GAP), tab_y, sl + PAD + 3 * (kw4 + ROW_GAP) + tab_w, tab_y + TAB_H)

# PADS
pl, pt, pr, pb = PANELS["kPanelPads"]
grid_r = pr - PAD - 3 * MIN_W - 2 * ROW_GAP
rects[75] = box(pl + PAD, content_top(pt), grid_r - ROW_GAP, pb - PAD)
kw = MIN_W
dy = content_top(pt) + ROW_GAP
for row, trio in enumerate([(76, 77, 78), (79, 80, 81)]):
    y = dy + row * (KNOB_H + ROW_GAP)
    for i, p in enumerate(trio):
        rects[p] = box(grid_r + i * (kw + ROW_GAP), y, grid_r + i * (kw + ROW_GAP) + kw, y + KNOB_H)

# VINTAGE
vl, vt, vr, vb = PANELS["kPanelVintage"]
rects[9] = box(vl + PAD, content_top(vt), vl + PAD + 220, content_top(vt) + TAB_H)
knob_x = vl + 250
knob_w_v = max(MIN_W, (vr - knob_x - PAD - 2 * ROW_GAP) / 3)
y_v = content_top(vt) + TAB_H + ROW_GAP
for i, p in enumerate([2, 46, 1]):
    rects[p] = box(knob_x + i * (knob_w_v + ROW_GAP), y_v, knob_x + i * (knob_w_v + ROW_GAP) + knob_w_v, y_v + KNOB_H)

# GLOBAL
gl, gt, gr, gb = PANELS["kPanelGlobal"]
inner = gr - gl - 2 * PAD
y1 = content_top(gt) + ROW_GAP
right = gr - PAD
mode_w = 56
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
drop_w = 80
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
y = 168
for p in tape_items:
    rects[p] = box(lr_l + PAD, y, lr_r - PAD, y + KNOB_H)
    y += KNOB_H + ROW_GAP
place_overlay("bypass_label", lr_l + PAD, y, lr_r - PAD, y + 16)
bypass_y = y + 18
rects[8] = box(lr_l + PAD, bypass_y, lr_r - PAD, bypass_y + TAB_H)
y = bypass_y + TAB_H + ROW_GAP
rw = (lr_r - lr_l - 2 * PAD - ROW_GAP) / 2
rects[109] = box(lr_l + PAD, y, lr_l + PAD + rw, y + KNOB_H)
rects[110] = box(lr_l + PAD + rw + ROW_GAP, y, lr_r - PAD, y + KNOB_H)
place_overlay("midi_hint1", lr_l + PAD, 944, lr_r - PAD, 962)
place_overlay("midi_hint2", lr_l + PAD, 962, lr_r - PAD, 980)

# FX
fxl, fxt, fxr, fxb = PANELS["kPanelFx"]
inner_fx = fxr - fxl - 2 * PAD
knob_y = fxb - PAD - KNOB_H
menu_y = content_top(fxt) + ROW_GAP
third = (inner_fx - 2 * ROW_GAP) / 3
rects[56] = box(fxl + PAD, menu_y, fxl + PAD + third, menu_y + DROPDOWN_H)
rects[62] = box(fxl + PAD + third + ROW_GAP, menu_y, fxl + PAD + 2 * third + ROW_GAP, menu_y + DROPDOWN_H)
rects[68] = box(fxl + PAD + 2 * (third + ROW_GAP), menu_y, fxr - PAD, menu_y + DROPDOWN_H)
all_fx_knobs = [57, 58, 59, 60, 63, 64, 65, 66, 69, 70, 71, 72]
knob_w = (inner_fx - (len(all_fx_knobs) - 1) * VISIBLE_ROW_GAP) / len(all_fx_knobs)
kx = fxl + PAD
for p in all_fx_knobs:
    rects[p] = box(kx, knob_y, kx + knob_w, knob_y + KNOB_H)
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
        f"constexpr float kPanelTitleBand = {TITLE_BAND}.0f;",
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
            r"struct OverlayRect \{.*?constexpr int kNumOverlayRects = static_cast<int>\(sizeof\(kOverlayRects\) / sizeof\(OverlayRect\)\);",
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
    geom = re.sub(
        r"constexpr float kCanvasW = [\d.]+f;\nconstexpr float kCanvasH = [\d.]+f;\nconstexpr float kLeftRailW = [\d.]+f;",
        "constexpr float kCanvasW = 1536.0f;\nconstexpr float kCanvasH = 1024.0f;\nconstexpr float kLeftRailW = 246.0f;",
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
