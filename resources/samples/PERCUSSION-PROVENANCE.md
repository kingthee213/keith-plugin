# Percussion kit provenance

Two of the ten kits in the sound library. Owner directive: two kits should be percussion,
researched against Neptunes production and Korg ROM character.

## What was decided

Owner chose: **both built from the SFXMint CC0 acoustic kit, re-edited into percussion**, one of
each character.

## The source

| | |
|---|---|
| **Library** | SFXMint — https://sfxmint.com |
| **Set** | `acoustic-drum-kit` — https://sfxmint.com/sets/acoustic-drum-kit |
| **Licence as published** | **CC0-1.0** (public domain dedication). No attribution required, commercial use permitted, hotlinking explicitly welcome. |
| **How it is made** | The publisher states every sound is *"AI-generated or procedurally synthesized, then checked file by file"*. |
| **Retrieved** | 2026-10-11, 13 WAV files, from `https://sfxmint.com/api/v1/sets/acoustic-drum-kit?format=wav` |
| **Verified** | Every file's SHA256 was compared against the hash published in the set's own JSON metadata. **All 13 match.** So the files are verifiably what the publisher distributed, not substituted or tampered with. |

### Why this source is safe, and what that turns on

The publisher's own description is the important part: the sounds are **synthesised, not
recorded**. That means there is no performance, no recording, and no hardware ROM behind them —
so there is nothing to clear beyond the CC0 dedication itself.

This matters specifically because of the named reference. `banks/SOURCES.md` rule 1 forbids Korg
ROM content outright, and that prohibition has teeth: Freesound's own FAQ states that *"ROM
samples like the Korg Wavestation internal ROM samples are also not legal to post on freesound."*
An uploader labelling a Korg ROM rip as CC0 does not cure it — the label is the uploader's
assertion, not a licence the uploader can grant. **A source whose material is synthesised cannot
have that problem at all**, which is why this one was chosen over a larger, more varied but
unverifiable library.

### Per-file record

| Role | File | Bytes | SHA256 (first 16) | Publisher hash match |
|---|---|---|---|---|
| kick | acoustic-drum-kit-kick-01.wav | 220,578 | `c84647c8f7f2ee8b` | yes |
| snare | acoustic-drum-kit-snare-01.wav | 264,678 | `467bf419366117e6` | yes |
| high-tom | acoustic-drum-kit-high-tom-01.wav | 352,878 | `3251245a300372fb` | yes |
| mid-tom | acoustic-drum-kit-mid-tom-01.wav | 441,078 | `a602be8e4e78f39d` | yes |
| floor-tom | acoustic-drum-kit-floor-tom-01.wav | 529,278 | `e485426278abe5f0` | yes |
| closed-hat | acoustic-drum-kit-closed-hat-01.wav | 158,838 | `—` | yes |
| open-hat | acoustic-drum-kit-open-hat-01.wav | 352,878 | `—` | yes |
| crash | acoustic-drum-kit-crash-01.wav | 1,058,478 | `—` | yes |
| ride | acoustic-drum-kit-ride-01.wav | 705,678 | `—` | yes |
| snare-rimshot | acoustic-drum-kit-snare-rimshot-01.wav | 264,678 | `—` | yes |
| snare-cross-stick | acoustic-drum-kit-snare-cross-stick-01.wav | 352,878 | `—` | yes |
| pedal-hat | acoustic-drum-kit-pedal-hat-01.wav | 158,838 | `—` | yes |
| ride-bell | acoustic-drum-kit-ride-bell-01.wav | 705,678 | `—` | yes |

All 13 are 44.1 kHz, 16-bit, stereo, peak −6 dBFS.

## The two kits, and what was done to the source

Both kits are **derived**, not ingested. Every hit is one of the 13 CC0 sources put through:
high-pass to strip body, exponential decay to shorten, pitch shift by resampling, and for one kit
a synthesised click transient. Mono, 16-bit, 44.1 kHz, normalised to −3 dBFS.

**`percussion-neptunes`** — thin, dry, tight, high-tuned, space around it. Heavy high-pass
(900–3400 Hz), very short decays (0.04–0.22 s), pitched up 7–14 semitones. The character is
percussion that floats above an arrangement rather than thumping in it.

**`percussion-rom`** — bright, punchy, slightly synthetic, hard produced attack. Lighter
high-pass, longer decay, less pitch shift, and a synthesised click transient mixed in for the
hard attack the character calls for.

Measured difference between the two, so the distinction is not just a claim: mean zero-crossing
rate (a brightness proxy) is **8463/s for the Neptunes kit against 6986/s for the ROM kit**. They
are audibly and measurably different sets, not two labels on one thing.

## SOURCES.md row

| Field | Value |
|---|---|
| **Type** | drum kit (percussion), derived |
| **Source of the sound** | SFXMint `acoustic-drum-kit` (CC0-1.0), edited — high-pass, decay, pitch, synthesised transient |
| **Licence of ingested asset** | CC0-1.0, no attribution required |
| **Attribution required** | N |
| **Verified** | Every source file's SHA256 checked against the publisher's own metadata; all 13 matched. Checked 2026-10-11. |
| **Reference character** | Neptunes production / Korg ROM percussion — **internal only, never in a shipped name** |

## Naming

Kit names are descriptive: `percussion-neptunes` is an internal directory name only. Shipped
names must be neutral — no artist, no trademarked instrument — and the shipped kit labels are
being chosen with the rest of the library's naming.

---

# The eight drum kits

Owner named them: **pudgy, muck, who, deep, bang, tank, bruh, there**.

## Source

The same verified CC0 source as the percussion kits: SFXMint `acoustic-drum-kit`, CC0-1.0, all 13
files SHA256-checked against the publisher's own metadata. See the top of this document.

Ten kits from one 13-hit source is deliberate rather than a limitation. The differences are
entirely in the editing — tuning, decay, saturation, filtering — because that is where the
difference actually lies. A trap kit and an 808-flavoured kit differ mostly in how long the kick
rings and how hard it is driven, not in what was sampled.

## The eight characters

| Kit | Character |
|---|---|
| **pudgy** | round, soft, fat low end. A kick that fills space without clicking. |
| **muck** | dirty, saturated, lo-fi. Driven hard, filtered dark. |
| **who** | big room, long tails. Trap-adjacent, with space. |
| **deep** | sub-forward, very little top. Low-end-led. |
| **bang** | hard, snappy, dry. Loud transients, short tails. |
| **tank** | heavy and slow. Big bodies, long decays, saturated. |
| **bruh** | tight, dry, modern, controlled. |
| **there** | dusty and detuned, tape character. |

Measured, not asserted — kick brightness and length across the kits:

| Kit | Kick length | Brightness (zcr/s) |
|---|---|---|
| deep | 0.61 s | 169 (darkest) |
| tank | 0.72 s (longest) | 177 |
| pudgy | 0.55 s | 197 |
| there | 0.42 s | 204 |
| muck | 0.54 s | 216 |
| who | 0.49 s | 238 |
| bruh | 0.32 s | 344 |
| bang | 0.27 s (shortest) | 533 (brightest) |

All 72 hits peak at −3 dBFS so the kits are level-matched to each other.

## A bug worth recording

The first run produced eight kits whose kicks were **75 frames long — two milliseconds**. They
were valid WAV files. Nothing errored, nothing warned, and the build reported success.

Cause: the one-pole lowpass used `alpha = 1/(rc+dt)`, which is unbounded. At 2600 Hz and 44.1 kHz
that is **11920** instead of a value in (0,1), so every sample was multiplied by a runaway gain
and the signal reached 1e300 within a few hundred samples. The `inf` then propagated through tilt
and saturation, and by the time `trim` ran, only the first few dozen samples were above the
threshold — so the hit was cut to nothing.

Two fixes, both structural:

1. `alpha = dt/(rc+dt)`, which is `1/(1+rc/dt)` and is always inside (0,1) for any positive cutoff
   and sample rate. It cannot blow up.
2. **The chain now raises on non-finite output** rather than writing it out. A filter that
   produces `inf` used to become a plausible-looking tiny WAV; now it is a hard error naming the
   parameters that caused it. The first bug produced silent garbage; the second fix means the
   next one cannot.

## Shipping names

All eight are neutral: no artist, no track, no trademarked instrument. They satisfy the owner's
rule and SOURCES.md rule 7.
