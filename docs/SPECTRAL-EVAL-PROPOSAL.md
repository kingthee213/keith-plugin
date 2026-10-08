# Proposal — a spectral evaluation on top of the scalar gate

Owner's question, verbatim:

> "tolerances on this gate table run might be too high, can we also somehow analyze spectrograms of
> these sounds to see the % difference and get a tighter overall eval? lmk if this is overkill or if
> it could work"

Short answer: **not overkill, right instinct — and the first probe already paid for itself by falsifying
something I had implied.** But the number it produces is not yet fit to be a gate, and this records why,
with the measurements behind that claim rather than an opinion.

## What the current gate actually proves

`tools/recreation_gate.py` compares four scalars — fundamental, attack, decay to −20 dB, spectral
centroid — at 1.5 % on pitch and 8 % on the rest. Those are tight on **pitch and envelope**. They say
almost nothing about **timbre**: two sounds can share a fundamental, an envelope and a centroid and be
completely different instruments, because the metric never looks at how energy is distributed across
frequency over time.

So the honest position is that "the recreation passes the gate" means "the recreation has the right
pitch and envelope". It was never evidence that it *sounds* like the reference. My earlier phrasing
leaned on the gate too hard and this corrects it.

## The probe

`tools/spectral_compare.py` (probe, not a gate). Renders each recreation through the plugin's own
engine, then compares it to its reference as a time-frequency image:

- Onset-trim both, so the comparison measures timbre and not the delay between them.
- Normalise both to equal RMS, so it measures spectral shape and not gain.
- Log-mel spectrogram, 64 bands, 2048-pt FFT, hop 512.
- Spectral convergence `||A−B||_F / ||A||_F` as a %, plus log-mel RMSE in dB.

The plugin renders at 48 kHz and the references are 44.1 kHz, so references are resampled up by
spectrum zero-padding. Linear interpolation would have lowpassed exactly the top octave under
examination.

## Results

| Recreation | Note | Spectral convergence | log-mel RMSE |
|---|---|---|---|
| Family 01 / 021 Upright Bass | 36 | **49.0 %** | 43.1 dB |
| Family 01 / 022 Hammer Keys | 24 | 63.5 % | 19.8 dB |
| Family 01 / 025 Snap Bass | 36 | 72.0 % | 28.4 dB |
| Family 01 / 024 Bright Reed | 72 | 72.3 % | 12.6 dB |
| Family 01 / 023 Swell Choir | 60 | 110.2 % | 12.9 dB |
| Family 01 / 026 Short Bass | 48 | 110.7 % | 22.6 dB |

Reference-to-reference, cross-instrument: **75.9 %** (Stein Bass C2 vs C3) up to **177.8 %**
(acoustic bass vs harmonica).

## The honest read

**Upright Bass is genuinely good.** At 49.0 % it is closer to its own reference than any
reference-to-reference pair scores, including the same instrument one octave apart. That is a real
result.

**The rest are not clearly distinguished from unrelated instruments.** Swell Choir (110.2 %) and Short
Bass (110.7 %) sit in the same band as *deliberately mismatched* pairs. By this measure they are about
as far from their reference as a bass is from a piano. They pass the scalar gate at 8 % and are
spectrally a different sound. That is the gap the owner suspected.

**But this metric is too crude to gate on yet**, and it must not be adopted as-is:

1. **It is dominated by pitch content, not timbre.** Same instrument, different note scores 80–133 %
   (bass C2 vs C3 is 126.7 %), because the harmonic series simply lands in different mel bands. So the
   scale above is comparing across different questions.
2. **We have no same-note calibration baseline.** Two recordings of the *same* note from the same
   instrument would give the "this is the same sound" anchor. Without it, no threshold can be set
   defensibly, and picking one anyway would be exactly the guessing this is meant to replace.
3. **Decay-length mismatch contaminates it.** The window runs from onset, so a recreation with a
   slightly longer decay accumulates error that has nothing to do with its timbre.
4. **Linear-magnitude Frobenius norm lets one loud low harmonic dominate**, which is why the metric
   exceeds 100 % and why log-mel RMSE sometimes disagrees with it (Hammer Keys: worst SC but a
   middling 19.8 dB).

## Recommended path

**Keep the scalar gate and add a spectral gate beside it.** Do not replace: the scalars are cheap,
interpretable, and prove specific claims (this preset really is at 523 Hz with a 70 ms attack). The
spectral gate proves a different claim (this preset's spectral evolution resembles the reference), and
neither substitutes for the other.

Two changes make the metric fit to gate on:

1. **Compare timbre independently of level and note.** Log-mel, then per-frame cosine distance rather
   than a raw norm, and/or MFCC with DTW so time-stretched decays are not counted as timbre error.
   Smooth across frequency to compare spectral *envelope*.
2. **Calibrate before thresholding.** Measure the same-note anchor, then set the threshold from the
   measured scale rather than by taste. A defensible form: a recreation must be closer to its own
   reference than the reference is to a different instrument at the same note.

Then the failures become actionable: Swell Choir and Short Bass need real spectral work, and the
library-wide version of this measurement would finally quantify "the presets feel samey" instead of
leaving it as an impression.

**Cost:** small. The probe runs in seconds on numpy alone; no new dependency.
