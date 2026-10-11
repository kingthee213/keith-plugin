# Guitar samples

All five banks are **CC0 1.0** (public domain dedication, no attribution required),
verified from the licence file shipped inside each FreePats archive.

| bank | source | instrument |
|---|---|---|
| clean | FreePats FSBS Clean Electric | direct-sampled electric guitar |
| jazz | FreePats FSBS Clean Electric (Jazz) | the same, warmer |
| dist1 | FreePats FSBS Distorted Electric 1 | the same, driven |
| dist2 | FreePats FSBS Distorted Electric 2 | the same, driven |
| nylon | FreePats Spanish Classical Guitar | mic'd nylon-string |

Files are `<midi note>.flac`, copied unaltered. FLAC because the plugin's loader decodes it
through dr_flac, so no re-encode is needed and the audio that ships is the audio published.

## What is not here

**Steel-string acoustic.** The only FreePats steel-string bank is GPL-3.0-with-exception,
and that exception is written for mixing samples into a composition -- it does not clearly
grant redistributing the sample files inside a product that other people play. The licence
is also deprecated by FreePats. One ambiguous grant is not worth mixing into fourteen clean
ones, so that patch stays synthesised until a CC0 steel-string turns up.

## Velocity and round-robin

The banks are velocity-layered and carry four round-robin takes per note. One layer (the
plain, non-soft) and one take are kept, because the plugin's sampler holds one sample per
note. The soft layer is a separate recording rather than a volume change, so this is a
small real loss of dynamics, not something velocity scaling could have reproduced.
