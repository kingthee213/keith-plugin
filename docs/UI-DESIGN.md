# Keith — UI Design Language

Working product name: **Keith** (BRIEF §13 — confirm or replace).  
This document is the drawing brief for the editor. A second person should be able to rebuild the shell from it without inventing a new look.

Prototype reference: `design/editor-prototype.html` (offline, single file).  
Parameter contract: `docs/PARAM-MANIFEST.md`.  
Drum map for the manual: `docs/DRUM-MAP.md`.

---

## 1. School (original art only)

**School statement.** Original art in a cartoon / comic-panel school: thick ink outlines, flat saturated fills, high-energy panel framing, cosmic green / magenta / gold accents on a deep ink ground. Energy and contrast first; no photorealism, no soft UI glass.

**Hard exclusion.** Style reference only. Do **not** reproduce any character, costume, ring, lantern, corps symbol, logo, wordmark, or recognisable mark from either referenced property. Do not use those property names in the UI, file names, or metadata. If a mark, mascot, texture, or icon is drawn, it is new.

**Mascot / mark (proposed).** A simple original star-burst stamp (eight hard points, flat gold fill, black outline) used as a decorative corner tick only — not a character, not a logo claim. If the owner rejects it, drop it; the shell stands without it.

---

## 2. Palette (hex + role)

| Token | Hex | Role |
|---|---|---|
| `ink` | `#0B0B12` | Window chrome ground, deepest panel fill |
| `panel` | `#14141F` | Module panel face |
| `panel-alt` | `#1C1C2A` | Nested boxes, list rows, inactive pads |
| `line` | `#0A0A0A` | Thick comic outline (near-black ink) |
| `paper` | `#F4EFE4` | Label paper strips, readout backgrounds |
| `cosmic-green` | `#39FF14` | Primary accent — active pad, focus ring, armed state |
| `magenta` | `#FF2BD6` | Secondary accent — FX hot controls, selected list row |
| `gold` | `#FFC857` | Tertiary accent — brand wordmark, value highlights, star stamp |
| `cyan-ink` | `#2EE6FF` | Readouts / MIDI activity blink (sparingly) |
| `mute-red` | `#FF3B4A` | Mute / off / clip warning |
| `flat-fill-a` | `#2A1A4A` | Drum panel wash (flat purple-ink) |
| `flat-fill-b` | `#0F2A22` | Synth panel wash (flat green-ink) |
| `flat-fill-c` | `#2A1020` | FX panel wash (flat magenta-ink) |
| `flat-fill-d` | `#1A1A10` | Preset strip wash |

Saturations stay high. Gradients are forbidden except a single 2-stop panel wash (solid → slightly darker solid). No purple-on-white “AI default” look; the ground is ink, accents are neon comic flat.

---

## 3. Type

Every face below is either OFL (freely redistributable) or drawn by us. Licences recorded for bundling into the iPlug2 editor binary.

| Face | Use | Weights | Sizes (px at design width 1280) | Licence | Source |
|---|---|---|---|---|---|
| **Bangers** | Product wordmark, section titles, pad labels | Regular only | Wordmark 42; section 22; pad 13 | SIL Open Font Licence 1.1 | Google Fonts / github.com/googlefonts/bangers |
| **Space Mono** | Parameter labels, list rows, numeric readouts | Regular, Bold | Label 11; readout 12–14; list 12 | SIL Open Font Licence 1.1 | Google Fonts / github.com/googlefonts/spacemono |
| **Keith Stamp** (drawn by us) | Optional 8-point star corner mark | n/a (vector) | 16–20 square | Original — all rights owner | Inline SVG in prototype; port as NanoVG paths |

**Type rules**

- Section titles: Bangers, uppercase, black `line` outline feel via 2–3 px text stroke in green/magenta/gold fill.
- Parameter labels: Space Mono Regular, uppercase tracking +0.04em, colour `paper`.
- Values: Space Mono Bold, colour `gold` when changing, `paper` at rest.
- No Inter, Roboto, Arial, system-ui as the design faces. System fallbacks are allowed only while fonts load; the shipped editor embeds the OFL files.

---

## 4. Panel / box vocabulary

### 4.1 Window

- **One window**, resizable. **Proposed** default / design size: **1280 × 800** CSS px.
- **Stated minimum (U5):** **1024 × 700**. Below that the shell is not claimed to keep a complete no-scroll surface.
- At design size and above: **no column scrollbars**; every control block (Kit, Mod·Voice, Insert 2, Master, Gain) is fully in view without scrolling a column.
- Aspect not locked; layout reflows by flex / denser rhythm, not by tabs.
- Outer chrome: `ink` fill, 4 px `line` border, 0 radius (hard comic corners). Optional corner star stamps (see §1).
- No tab bar. Drum, synth, FX, and preset browser are simultaneous panels.

### 4.2 Module frame (“comic panel”)

- Rectangle with **5 px** outer `line` stroke.
- 8 px inner gutter of `panel`.
- Title strip: 28 px tall paper or flat colour bar along the top edge, title in Bangers.
- Panel washes: drum `flat-fill-a`, synth `flat-fill-b`, FX `flat-fill-c`, preset `flat-fill-d`.
- Drop “shadow”: a second hard rectangle offset +4,+4 in `line` (no blur).

### 4.3 Knob

- Circle diameter **34 px** at design size (compact **30 px** at minimum 1024×700).
- Fill `panel-alt`, stroke 2 px `line`.
- Indicator: thick radial tick in `cosmic-green` (synth/drum) or `magenta` (FX).
- Arc track under the tick: 270° from 7-o’clock to 5-o’clock.
- Label centred below, Space Mono ~9–10.
- Value readout under label at design size; hidden at minimum size to reclaim height.
- Interaction: drag vertical = change; double-click = default (**proposed** behaviour).

### 4.4 Switch

- Pill **40 × 18**, hard outline 2 px (compact vs earlier 48×22 to fit).
- Off: `panel-alt` fill, thumb left.
- On: `cosmic-green` or `magenta` fill (group colour), thumb right, black outline retained.
- No soft shadows.

### 4.5 Pad (drum)

- Square **44 × 44** at design size (grid gap 4); **36 × 36** at minimum. Caps prevent the old aspect-fill clip.
- Idle: `panel-alt`, 3 px `line`, flat.
- Hit / MIDI activity: flash `cosmic-green` fill for ~80 ms.
- Selected slot: `gold` outer ring outside the pad.
- Muted slot: `mute-red` diagonal hatch (SVG or two lines).
- Label: short slot name inside pad (Bangers); note name below at design size only.

### 4.6 List (preset / bank)

- Rows 28 px, alternating `panel` / `panel-alt`.
- Selected row: `magenta` left bar 6 px + `paper` text on `flat-fill-c`.
- Family chips: hard rectangles, selected = `gold` fill + black text.

### 4.7 State language

| State | How it reads |
|---|---|
| Idle | Flat panel colours, paper labels |
| Focus / hover | 2 px `cosmic-green` inset ring on the control |
| Active / playing | Pad or meter flashes green/cyan |
| Armed / selected | Gold ring (pads) or magenta bar (lists) |
| Muted / bypass | Mute-red hatch or dim to 40% opacity + “BYP” tag |
| Automation written | Small cyan-ink square at control top-right (**proposed**) |

---

## 5. Layout (no tab hunt)

All four zones visible at once. Wireframe at design size 1280x800 (U5 fit pass):

```
+-- chrome ------------------------------------------------------+
| [*] KEITH   PRESET STRIP (family / bank / patch / nav)  52px |
+----------------+-----------------------------+-----------------+
| DRUM KIT       | SYNTH / BASS / KEYS         | FX              |
| 4x4 pads 44px  | Engine / Poly               | Tape bus        |
| slot + M/S     | Osc                         | Insert 1 | 2    |
| kit knobs      | Filter | Amp Env (pair)     | Master + Gain   |
|                | Mod / Voice                 |                 |
+----------------+-----------------------------+-----------------+
      ~300px                flex grow                 ~340px
```

**Fit rules (U5)**

- **1280 x 800 (design):** complete surface, no column scroll, no clipped block bottoms. Pads capped at 44 px (not aspect-fill). Knobs 34 px. Preset strip single 52 px row. Filter|Amp and Insert1|Insert2 side-by-side pairs reclaim vertical budget.
- **1024 x 700 (minimum):** same complete surface via denser rhythm -- pads 36 px, knobs 30 px, value readouts under knobs hidden, note labels under pads hidden, brand subtitle dropped. Still no column scrollbars.
- Width between min and design: three columns stay; columns shrink with the denser media query under 1100 / 740.

**1:1 mapping.** Every visible control maps to exactly one row in `PARAM-MANIFEST.md`. Decorative art (star stamps, panel washes, title ink) is not a parameter. U1 live parameter `gain` appears in the FX Master out row.

---

## 6. Zones in detail

### 6.1 Preset strip (top)

- Product wordmark left.
- Family chips: `Family 01` … `Family 04` only (never reference-machine names).
- Bank list (scroll) + patch list (scroll) side by side or stacked on narrow widths.
- Prev / next patch switches; Save / Init (**proposed** — Init confirms).
- Star / favourite toggle (**proposed**).

### 6.2 Drum kit (left)

- 16 pads = 16 slots (see `DRUM-MAP.md`).
- Under grid: selected-slot knobs — Level, Pan, Tune, Decay, Filter, Drive (**proposed** set).
- Kit-level: Kit Level, Kit Swing feel amount if used (**proposed** — may drop if engine has no swing).

### 6.3 Synth / bass / keys (centre)

- Engine switch: **Hybrid** (Surge-style behaviour target) / **Analog** (OB-XF-style behaviour target). Labels are neutral — never “Surge” or “OB-XF” in the UI.
- Part / layer select if multi-part (**proposed** single part for v0.1).
- Blocks always visible: Osc, Filter, Amp Env, Mod (condensed matrix destinations).
- Unison, Porta, Poly/Mono.

### 6.4 FX (right)

- **Tape** global bus (BRIEF §3 / §7): Saturation, Head Bump, Wow, Flutter, Hiss, Print, Mix.
- Insert FX 1–2 and Master FX: algorithm list + 3–4 macros each (**proposed** algorithm catalogue pending content lane totals).
- Per-block bypass switches.

---

## 7. Motion (presence, not noise)

1. Pad hit: 80 ms green flash + 1 px scale punch.
2. Preset load: title strip ink wipe left→right ~200 ms.
3. Knob grab: indicator brightens to full neon.

No continuous idle animation. No glow bloom stacks.

---

## 8. Assets and licence checklist

| Asset | Origin | Licence |
|---|---|---|
| Bangers.ttf | googlefonts/bangers | OFL-1.1 |
| SpaceMono-Regular/Bold.ttf | googlefonts/spacemono | OFL-1.1 |
| Star stamp SVG | drawn for this project | original |
| Halftone / panel CSS | authored CSS | original |
| Prototype HTML | authored | original |

No Splice, Freakshow, Gorillaz, Green Lantern, or Korg-named assets.

---

## 9. Porting notes (composer lane)

- Port layout geometry and colours into iPlug2 / NanoVG (or chosen iPlug2 graphics stack).
- Bind every control id from `PARAM-MANIFEST.md` 1:1; do not invent UI-only parameters.
- Embed the two OFL families in the plugin resources; keep OFL notices in license notes.
- Prototype is the visual contract, not production code.

---

## 10. Proposed (undecided) items in this doc

Marked **proposed** above and listed again for the coordinator:

1. Default window size 1280×800; U5 states minimum **1024×700** (replaces earlier 960×640 claim until owner confirms).
2. Eight-point gold star corner stamp.
3. Knob double-click → default.
4. Automation-written cyan square badge.
5. Preset Init + favourite star.
6. v0.1 single synth part (no multi-part mixer yet).
7. Drum slot control set: Level, Pan, Tune, Decay, Filter, Drive.
8. Kit Swing control (drop if unused by engine).
9. Insert/Master FX algorithm lists and macro counts (await U2/U4).
10. Neutral engine labels **Hybrid** / **Analog**.
11. Whether U1 `gain` remains the long-term master level or is superseded by `synth_master_level` / `fx_out_level`.
