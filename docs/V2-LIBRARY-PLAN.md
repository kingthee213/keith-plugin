# Yote Machine v2 — seeding a real library

What the owner asked for, verbatim:

> "spec a plan to seed the real plugin library rather than 10 random sounds (this is not the outcome I
> was looking for, we need a massive selection of real presets from the listed hardware/software). two
> of the synth sounds being further off is an understatement. anything over 1.5% difference in spectral
> imaging is not a successful recreation. we need to go until we can seed a full library of accurate
> sounds. at least 300 real presets made from recreations of sourced samples found online. surge repo is
> available for reference on how to add variety to sounds where necessary, and a third oscillator is
> also more than welcome if it assists in this pursuit."

Agreed on the diagnosis. Ten hand-picked references was a sample, not a library, and "passes the gate"
was never the same claim as "sounds like the source". This document is the plan, and it starts with the
one thing that has to be settled before any of the rest is worth building.

## 1. The 1.5% standard: right instinct, undefined number

The standard is the correct ambition and I am adopting it. But **it cannot be applied to the current
metric, because on that metric 1.5% is below the noise floor of the recording itself.**

Measured, from `docs/SPECTRAL-EVAL-PROPOSAL.md`: the present metric scores 49% for our best recreation,
and **80–133% between the same instrument at a different note**. Two performances of the *same* note by
the *same* instrument would not score 1.5% on it either — the window content, decay length and noise
floor differ enough on their own.

So "1.5%" has to be pinned to a defined metric before it means anything. Three things fix that:

1. **Normalise the metric.** Per-frame cosine distance on log-mel, or MFCC with DTW, so level,
   duration mismatch and one loud low harmonic stop dominating. (The current linear-magnitude norm is
   why it overshoots 100%.)
2. **Measure the floor.** Establish the distance between two things that are *the same sound* — the
   reference against itself under a small perturbation (1–2% detune, a little EQ). Whatever that
   scores is the "indistinguishable" anchor. A target tighter than the floor is unachievable by
   definition, not by effort.
3. **Then set the bar relative to it.** Recommended form: *a recreation must score no more than the
   floor plus a stated margin, and must be closer to its own reference than any other reference is.*
   That is a real 1.5%-style standard — tight, measurable, and not arbitrary.

**Deliverable:** a calibrated spectral gate. Everything below is judged by it, so it is Phase 0.

## 2. The honest architectural limit

Here is the part I would be failing you by leaving out.

**Pure subtractive synthesis cannot reach a 1.5%-class spectral match to a recording.** A recording
carries the specific harmonic content, noise, room and microphone of that take. Synthesis approximates
the envelope and evolution of that; the residual in image space stays large even when it sounds close.
That is exactly why the ear and the spectrogram disagree, and it is why a sampler trivially wins this
comparison — the sampler *is* the recording.

So there are two architectures, and the target decides which is required:

**A — pure synthesis (today's)**. Oscillators, filters, envelopes. Gets a recognisable, playable,
characterful instrument. Will land in the tens of percent on a normalised spectral metric. Good for
synthetic families: pads, plucks, basses, leads, drums.

**B — sample-derived wavetable resynthesis (recommended for the "real instrument" families)**. We do
not ship anyone's recordings and we do not build a sampler. We analyse a source recording offline and
ship a **derived wavetable / multi-sample waveform set** — our own data, generated from the analysis,
tuned to the measured spectral evolution. This is the Surge lesson: most of its range comes from
**oscillator variety plus wavetables**, not from a bigger subtractive engine.

B is the only path that plausibly reaches a 1.5%-class match on pianos, basses, choirs and reeds while
shipping no third-party audio. It also stays inside the decision already recorded in `docs/DECISIONS.md`
D2, because we generate derived data rather than redistributing recordings.

**Decision needed:** confirm B for the real-instrument families. Without it, the 1.5% standard and
"recreate algorithmically" are in direct conflict and no amount of iteration resolves it.

## 3. Engine work

- **Third oscillator** — accepted, and genuinely needed. Three sources is what makes layered real
  instruments (body + attack transient + air) reachable.
- **Wavetable oscillator** — the core of path B. Loads our derived tables, morphs between frames.
- **More filter character** — current set is a single family; Surge's variety comes substantially from
  filter types. Add at minimum a ladder-style and a comb/character filter.
- **Per-oscillator envelopes and routing** — needed so oscillator 3 can be a transient layer.
- Reference the Surge source for oscillator taxonomy and wavetable handling. **Reference, not copy** —
  Surge is GPL, and taking its code into a distributed plugin is a licensing problem. Ideas are free;
  its source is not.

## 4. Sourcing

`freewavesamples.com` alone carries thousands of free recordings across many instruments, and already
gave us the ten. Other free libraries extend coverage. The library plan:

- **Target 300+ presets**, organised by emulated family first (the existing `SOURCES.md` rule), then
  by role.
- **Source in batches**, not one at a time: pick an instrument group, gather N references, run the
  pipeline, gate them, then move on.
- **Every preset gets a provenance row** — source, licence, and whether it was synthesised or derived.
  `banks/SOURCES.md` already enforces this and it stays.

## 5. The pipeline — the actual answer to "300"

300 accurate recreations cannot be hand-tuned, and hand-tuning ten already produced two poor results.
So the work is to build a **pipeline**, and then run it:

```
source recording
  -> trim, onset-align, measure  (exists: tools/analyse_references.py)
  -> fit synthesis / derive wavetable   (new: the hard part)
  -> render through the plugin engine
  -> spectral + scalar gate against that source
  -> pass: emit preset + provenance row
     fail: report measured numbers, iterate or reject the source
```

The value is that step 4 is already built and the gate is objective. A human reviews the failures,
never the passes. That is how 300 gets done at a quality bar instead of at a rate.

## 6. Phases

| Phase | Work | Done when |
|---|---|---|
| **0** | Calibrate the spectral gate: normalised metric, measured floor, threshold derived from it | The gate reports a number whose 1.5%-class bar is defensible |
| **1** | Engine: third oscillator, wavetable oscillator, filter variety | A hand-built preset proves each new source is audible and gated |
| **2** | Pipeline: automate fit/derive → render → gate → emit | One source runs end to end unattended |
| **3** | Source and seed, in batches | 300+ presets, each with a passing gate row and provenance |
| **4** | Library capacity: the archive list must handle 300+ entries (scroll/paging as needed) | The UI browses 300 presets cleanly |
| **5** | macOS verified on real hardware, then a real DAW | A build exists that someone other than me has loaded and played |

Phase 5 is the one still blocked today: no Mac was reachable, so `tools/build-macos.sh` remains
unexecuted and the shipped zip is source-plus-script rather than a binary.

## 7. What I need from the owner

1. **Confirm path B** (sample-derived wavetables) for real-instrument families — or accept that 1.5% is
   not reachable by pure synthesis and re-scope the standard for those sounds.
2. **Confirm the sources.** Which free libraries may be harvested, and whether derived wavetables from
   them are acceptable given D2.
3. **Mac access.** Either wake `solacemacbook` on the tailnet so it can be built and verified, or run
   one command on it — that closes Phase 5 and turns the zip into a tested artifact.

## What does not change

The existing engine, 109 presets, tests and the scalar gate all stand. This is additive: a calibrated
spectral gate, wavetable/third-oscillator capability, and a pipeline that scales the same rigour from
ten sounds to three hundred.
