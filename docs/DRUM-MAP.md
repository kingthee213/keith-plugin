# Keith - synthesized drum map

Keith has a 16-voice synthesized percussion engine. It does not load or play audio files. Each
voice is generated from oscillators, deterministic noise, envelopes, filtering and drive.

## MIDI split

MIDI notes 36-51 trigger drum slots 01-16 and do not reach the synth voice allocator. Drum
note-offs are consumed because the percussion envelopes are one-shot. Notes 0-35 and 52-127 go to
the synth only.

This split keeps drum hits from taking or retriggering any of the 16 synth voices. It also gives
controllers one contiguous 4x4 pad range:

```
top:     P13  P14  P15  P16
         P09  P10  P11  P12
         P05  P06  P07  P08
bottom:  P01  P02  P03  P04
```

## Slots and defaults

| Pad | Slot | Synthesized voice | MIDI | Note | RR | Level | Pan |
|---|---:|---|---:|---|---:|---:|---:|
| P01 | 01 | Kick | 36 | C1 | 3 | 0 dB | C |
| P02 | 02 | Snare | 37 | C#1 | 4 | -1 dB | C |
| P03 | 03 | Snare Alt / Rim | 38 | D1 | 2 | -3 dB | R10 |
| P04 | 04 | Clap | 39 | D#1 | 3 | -2 dB | L10 |
| P05 | 05 | Closed Hat | 40 | E1 | 4 | -4 dB | R25 |
| P06 | 06 | Open Hat | 41 | F1 | 2 | -4 dB | R30 |
| P07 | 07 | Pedal Hat | 42 | F#1 | 2 | -5 dB | L20 |
| P08 | 08 | Tom Lo | 43 | G1 | 2 | -2 dB | L18 |
| P09 | 09 | Tom Mid | 44 | G#1 | 2 | -2 dB | C |
| P10 | 10 | Tom Hi | 45 | A1 | 2 | -2 dB | R18 |
| P11 | 11 | Ride | 46 | A#1 | 2 | -5 dB | R20 |
| P12 | 12 | Crash | 47 | B1 | 2 | -4 dB | L20 |
| P13 | 13 | Perc 1 | 48 | C2 | 3 | -3 dB | L25 |
| P14 | 14 | Perc 2 | 49 | C#2 | 3 | -6 dB | R25 |
| P15 | 15 | Perc 3 | 50 | D2 | 2 | -5 dB | L30 |
| P16 | 16 | Perc 4 | 51 | D#2 | 2 | -4 dB | R30 |

No default slot is silent. The level and pan spread keeps a new kit balanced without edits. Kit
level defaults to 0 dB.

## Velocity and round-robin

Velocity always controls output level. The engine also uses three synthesis bands:

| Band | Velocity | Synthesis change |
|---|---:|---|
| Soft | 1-42 | Lower transient and darker noise balance |
| Medium | 43-95 | Nominal voice |
| Hard | 96-127 | Stronger transient and brighter noise balance |

Each slot advances through the RR count in the table. The alternates use small fixed pitch and decay
offsets plus a deterministic noise seed. Reset or preset recall restarts the sequence, so the same
MIDI phrase produces the same output while repeated hits still vary.

## Slot controls

The selected slot stores tune, decay, filter cutoff, drive, level, pan, mute and solo. If any slot
is soloed, every non-soloed slot is silent. Mute and a slot level of `-inf` produce exact silence.
The drum bus is stereo and joins the synth before Insert 1, Insert 2, tape and master processing.

The editor pads select slots only. They do not audition. Play MIDI notes 36-51 to hear the voices.

## Why there is no swing control

Swing changes the timing of generated steps. Keith has no step sequencer or pattern player, so it
has no steps to move. `drum_kit_swing` is not applicable, not pending.
