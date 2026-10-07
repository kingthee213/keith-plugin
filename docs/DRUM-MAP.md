# Keith — Drum Map

Documented map for the preset/control manual (BRIEF §2, §12).  
**Status: proposed.** BRIEF requires a documented map with velocity layers and round-robin; it does not lock note numbers or pad order. This is the U3 proposed default for the friend to play from a keyboard or pad controller in Ableton Live 12.

Neutral slot names only. No reference-machine program names.

---

## 1. Overview

| Item | Value | Status |
|---|---|---|
| Slots per kit | 16 | proposed |
| Pad grid | 4×4, left→right, bottom→top row order matching Live Drum Rack habit | proposed |
| Base octave | MIDI note **36 (C1)** = Slot 01 Kick | proposed |
| Velocity layers | 3 per slot (Soft / Med / Hard) | proposed |
| Round-robin | 2–4 alternates per layer (slot-dependent) | proposed |
| Chromatic spill | Notes outside 36–51 ignored by drum section (synth may still voice if dual-path — engine decision) | proposed |

---

## 2. Pad grid ↔ slot ↔ MIDI

Pad numbering in the editor (visual):

```
 Row 3 (top):    P13  P14  P15  P16
 Row 2:          P09  P10  P11  P12
 Row 1:          P05  P06  P07  P08
 Row 0 (bottom): P01  P02  P03  P04
```

| Pad | Slot | Role | MIDI note | Note name | RR alts (proposed) |
|---|---|---|---|---|---|
| P01 | 01 | Kick | 36 | C1 | 3 |
| P02 | 02 | Snare | 38 | D1 | 4 |
| P03 | 03 | Snare Rim / Side | 40 | E1 | 2 |
| P04 | 04 | Clap | 39 | D#1 | 3 |
| P05 | 05 | Closed Hat | 42 | F#1 | 4 |
| P06 | 06 | Open Hat | 46 | A#1 | 2 |
| P07 | 07 | Pedal Hat | 44 | G#1 | 2 |
| P08 | 08 | Tom Lo | 45 | A1 | 2 |
| P09 | 09 | Tom Mid | 47 | B1 | 2 |
| P10 | 10 | Tom Hi | 50 | D2 | 2 |
| P11 | 11 | Ride | 51 | D#2 | 2 |
| P12 | 12 | Crash | 49 | C#2 | 2 |
| P13 | 13 | Perc 1 (Conga/Bongo) | 37 | C#1 | 3 |
| P14 | 14 | Perc 2 (Shaker) | 54 | F#2 | **note: outside 36–51 contiguous block** — see §4 |
| P15 | 15 | Perc 3 (Rim click) | 37 | C#1 alt role if Perc1 empty — **conflict risk** | proposed revise |
| P16 | 16 | Perc 4 (Cowbell/Stab) | 56 | G#2 | see §4 |

### Proposed contiguous map (preferred for v0.1)

To keep one unbroken MIDI range for the drum section and avoid note conflicts, **prefer this locked-intent table** over mixed GM outliers for P14–P16:

| Pad | Slot | Role | MIDI note | Note name | RR |
|---|---|---|---|---|---|
| P01 | 01 | Kick | 36 | C1 | 3 |
| P02 | 02 | Snare | 37 | C#1 | 4 |
| P03 | 03 | Snare Alt / Rim | 38 | D1 | 2 |
| P04 | 04 | Clap | 39 | D#1 | 3 |
| P05 | 05 | Closed Hat | 40 | E1 | 4 |
| P06 | 06 | Open Hat | 41 | F1 | 2 |
| P07 | 07 | Pedal Hat | 42 | F#1 | 2 |
| P08 | 08 | Tom Lo | 43 | G1 | 2 |
| P09 | 09 | Tom Mid | 44 | G#1 | 2 |
| P10 | 10 | Tom Hi | 45 | A1 | 2 |
| P11 | 11 | Ride | 46 | A#1 | 2 |
| P12 | 12 | Crash | 47 | B1 | 2 |
| P13 | 13 | Perc 1 | 48 | C2 | 3 |
| P14 | 14 | Perc 2 | 49 | C#2 | 3 |
| P15 | 15 | Perc 3 | 50 | D2 | 2 |
| P16 | 16 | Perc 4 | 51 | D#2 | 2 |

**Coordinator decision:** adopt **contiguous map (36–51)** as the printed manual map. The GM-flavoured variant above is research only and must not ship if it conflicts.

---

## 3. Velocity layers

| Layer | Velocity | Behaviour |
|---|---|---|
| Soft | 1–42 | Soft sample (or low gain + darker filter if single sample) |
| Med | 43–95 | Medium sample |
| Hard | 96–127 | Hard sample / transient accent |

Exact split points are **proposed**. Engine may retune after ear tests; document final splits in this file when locked.

---

## 4. Round-robin

- On each hit within a velocity layer, advance RR index: `next = (last + 1) % N`.
- N per slot as in the contiguous table (2–4).
- Reset RR on kit/preset load (**proposed**).
- Goal: kill machine-gun repeats on hats and snares (BRIEF §2).

---

## 5. Editor interaction

- Click pad: select slot (updates slot knobs) and audition at velocity 100 (**proposed**).
- MIDI in: lights the matching pad; does not steal keyboard focus from host.
- Mute / Solo: per-slot switches from `PARAM-MANIFEST.md`.

---

## 6. Manual blurb (draft copy)

> Keith’s drum kit is sixteen slots on pads **P01–P16**. MIDI notes **C1–D#2 (36–51)** trigger slots 01–16. Each slot has three velocity layers and round-robin alternates so hats and snares do not machine-gun. Map your pad controller to that range, or play it from the keys.

---

## 7. Proposed-but-undecided

1. Contiguous 36–51 map vs GM-flavoured note numbers.
2. Velocity split points 42 / 95.
3. RR counts per slot.
4. Click-pad audition velocity = 100.
5. Whether notes outside 36–51 are ignored or passed to the synth engine.
6. Pad bottom-left = Kick (Live-like) vs top-left origin.
