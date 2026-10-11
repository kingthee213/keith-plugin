# Sample sourcing: what is actually usable

The owner asked for a library split roughly **50% organic (sampled) / 25% hybrid / 25% synth**, and
suggested Epidemic Sound, then Freesound. This records what each source permits, checked against
the licence text rather than the marketing pages.

## Epidemic Sound — NOT USABLE

Read from the Pro Subscription Music License, 2026-10-11.

The licence grants a **synchronisation licence only**: the right to put their works *"in video and
podcast productions"*. Placing their audio inside a plugin is not a synchronisation use — it is
redistribution of the audio as part of a product. And it closes the gap explicitly:

> *"You may only use the Licensed Works under and subject to the rights granted under the terms and
> conditions of this License. All rights not expressly granted hereunder are reserved by Epidemic
> Sound."*

Two further blockers:

- **Non-transferable.** A plugin is given or sold to other people, which the licence does not allow.
- **It lapses.** *"After expiration or termination of the Subscription Period you may not use the
  Licensed Work(s) to create any new Productions."* A plugin built on it would become unlawful for
  every user the day a subscription ended.

Using it would mean shipping a product that cannot legally ship. Not a grey area.

**If Epidemic is wanted**, it needs a negotiated business licence with explicit redistribution
rights. That is a conversation with them, not a subscription.

## Freesound — USABLE, with care

Read from the Freesound FAQ, 2026-10-11. CC0 and CC-BY both permit commercial redistribution;
**CC-BY-NC does not** and is excluded for a commercial product.

Their own wording on CC0: *"you can do pretty much what you want with the sound. You could even
sell the sound."* And the remix table is explicit: a CC0 or BY sound may be distributed in a
derivative, with attribution where the source was BY.

Freesound adds a per-sound uploader preference panel for generative-AI training. **That is about
training models, not about sampling**, so it does not bear on this use — but it is worth knowing
the field exists, because it can express a preference even on a CC0 sound.

### The MTG instrument packs — CC-BY 3.0

The best fit found so far. MTG is the Music Technology Group at Universitat Pompeu Fabra, the
group that runs Freesound itself, and these come from the **good-sounds.org** project: monophonic
instrument samples, one note per file, tagged with the note, mic and tuning reference.

| | |
|---|---|
| **Licence** | **CC-BY 3.0** — verified in the page markup for separate files (354452, 361080), not inferred from the uploader's name |
| **Attribution** | **REQUIRED** |
| **Commercial use** | permitted |
| **Format** | WAV, 48 kHz, 24-bit, mono, one note per file |
| **Covers** | flute, violin, and others in the same project — see the packs |

**`good-sounds.org` itself is now a squatted domain** serving an unrelated marketing blog. The
samples survive on Freesound with Freesound's licence attached, so the licence is verifiable at the
source of download; the original project site is not available as a cross-check and that is
recorded rather than glossed.

**This changes the attribution obligation.** The repo's `SOURCES.md` requires a filled licence per
asset. CC-BY means the plugin needs a real attribution string — the uploader, the work, the licence
and a link — in the manual and in the plugin's about screen, not just a line in a markdown file.

## What this means for the 50/25/25 target

Checked honestly against what can actually be sourced:

| Group | Wanted | Sourceable? |
|---|---|---|
| guitars | organic | **partly** — FreePats has electric guitar banks (CC0); acoustic guitar on Freesound is mixed-licence and needs checking file by file |
| reeds/horn | organic | **partly** — MTG has flute; oboe/clarinet/bassoon/brass need separate hunting |
| strings | organic | **yes** — MTG violin packs, CC-BY |
| keys | hybrid | **yes** — FreePats acoustic grand and honky-tonk (CC0), MTG for others |
| drums | organic | already done — SFXMint CC0 |
| pads, polysynth, leads, bass | synth | nothing needed |

**50% organic is not reachable from free sources alone** for a library this size. Flute and violin
exist; a full orchestral wind and brass section, and convincing multi-sampled acoustic guitar, do
not — not at CC0 or CC-BY across the board. The honest options are a smaller organic share, a paid
licence for the gaps, or instruments replaced by hybrids (a sampled attack over a synthesised
body), which needs far less source material for a much better result than pure synthesis.

## Rules this has to respect

From `banks/SOURCES.md`, unchanged:

1. No Korg ROM content, and no factory program data ripped from hardware or Korg's software.
2. No Splice samples anywhere.
5. Open-source code is not permission to reuse its sample content — code licence and content
   licence are separate questions. **Check both, record both.**
6. Every sample and every patch recipe gets a row.
7. Neutral labels only in shipped names and metadata.

**CC-BY-NC is excluded** — the plugin is a commercial product.
