# Keyboard wear: provenance

The "worn character" layer for the 20 keyboard patches. Owner's choice: build the tone in the
engine, source the noisy/worn character from free samples.

## The source

| | |
|---|---|
| **Project** | FreePats — https://freepats.zenvoid.org |
| **Licence** | **CC0 1.0** (public domain dedication), stated on each instrument page |
| **Used here** | `Organ/electric-organ.html` — three banks |
| **Retrieved** | 2026-10-11 |

### What was downloaded

| Bank | Archive | Bytes |
|---|---|---|
| Drawbar organ emulation | `DrawbarOrganEmulation-SFZ-20190712.tar.xz` | 6,042,972 |
| Percussive organ emulation | `PercussiveOrganEmulation-SFZ-20190715.tar.xz` | 12,423,412 |
| Rock organ emulation | `RockOrganEmulation-SFZ-20190715.tar.xz` | 12,576,272 |

Each contains a folder of note-named WAVs (`G#5.wav`, `C5L.wav`/`C5R.wav` for stereo pairs) plus
an SFZ mapping file. Verified as real audio: 44.1 kHz, 16-bit, mono, peak 0.999, sustained and
marked `loop_mode=loop_continuous` in the SFZ, which is right for an organ.

## Why this source is safe, and it turns on one thing

**The samples are recordings of `setBfree`, a free software synthesiser** — stated on the
instrument page: *"Recorded from setBfree software synthesizer."*

That matters for exactly the reason the percussion source did. `banks/SOURCES.md` rule 1 forbids
reusing hardware ROM content, and the prohibition is real: Freesound's own FAQ says ROM samples
like the Korg Wavestation's are *"not legal to post"*, and an uploader's label does not cure a bad
origin, because the label is the uploader's assertion rather than a licence they can grant.

**A recording of a free software synthesiser cannot have that problem.** There is no hardware ROM
and no commercial instrument behind it — only a GPL program's output, and output of a program is
not covered by the program's licence. Add FreePats' own CC0 dedication and the chain is short and
clean.

### Note on the source's wording

The instrument pages describe the sound as an imitation of *"an electromechanical organ, known as
a Hammond organ due to its inventor and original manufacturer"*. That wording is the **source's**
description of what the instrument imitates. It is internal reference only and appears in **no
shipped name or metadata** — see the naming section of this repo's brief.

## How the wear is used

The tone of every keyboard patch is **synthesised in the engine** — FM for the struck tines, the
wavetable bank for the tape-flute choir, stacked partials for the drawbars. **No instrument is
sampled for its pitch content.**

What is taken from FreePats is the *mechanical character*: the key noise, the thump, the room.
That is the layer that makes a keyboard read as an instrument rather than a patch, and it carries
no melodic information — it cannot be recognised as a particular record or product.

## SOURCES.md row

| Field | Value |
|---|---|
| **Type** | mechanical noise / key character, sampled |
| **Source of the sound** | FreePats `electric-organ` banks (recordings of setBfree, a free software synthesiser) |
| **Licence of ingested asset** | CC0 1.0, no attribution required |
| **Attribution required** | N |
| **Verified** | Download URLs taken from the instrument page; archives opened and contents inspected; licence read on the page itself. Checked 2026-10-11. |
| **Reference character** | Electromechanical organ family — **internal only, never in a shipped name** |

## Tone, for the record

Every keyboard patch's pitch content is engine-generated. The 20 keyboards carry the categories
`key tine`, `key organ`, `key combo` and `key tape`, and their names describe the instrument type
only: Tine Piano, Reed Piano, Tonewheel Organ, Combo Organ, Tape Choir. None of them names a
manufacturer, a product, or a trademark.
