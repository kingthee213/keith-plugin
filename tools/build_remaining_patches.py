"""The remaining melodic patches: pads, strings, FX, reeds/flutes, brass.

    /work/.venv/bin/python /work/tools/build_remaining_patches.py

10 of each, 50 total, completing the 160 melodic sounds (with the 50 polys and 50 leads).

Same shape as the other generators: each patch is a set of parameter deltas, generated rather
than hand-written so the intent stays readable.

## What distinguishes each kind

A pad, a string and a brass patch are all "sustained", but they are not built the same way, and
the differences are what make the five categories worth having:

  PADS      slow attack, long release, heavy stereo, movement from an LFO on the filter. The
            point is the space they occupy, so width and drift matter more than brightness.
  STRINGS   a bow has a slower, less clicky attack than a pluck, and the body sits in the upper
            mids. Slight detune and a gentle filter so a section reads as several players.
  FX        deliberately not musical: extreme pitch, ring modulation, noise, long delay. An FX
            patch is allowed to be unusable as a melody.
  REEDS     a narrow pulse for the hollow body, a little breath noise, a fast but soft attack.
            The noise is what makes a reed read as wind rather than as a synth.
  BRASS     a filter attack that opens *after* the note starts, which is the brass "blat". No
            other category wants that, and it is the whole character.

## Ranges

The FX patches want extreme values -- a long delay, a deep pitch drop -- and the first version
used 76% delay and -48 semitones. Both are outside their parameters' registered ranges, so the
host would have clamped them silently: the patch loads, sounds wrong, and nothing says why.
kParamStereoDelay is 0..40 and kParamPitchAmount is -24..+24, so 40 and -24 are the real extremes.
Caught by tools/check_patch_ranges.py, which is why that gate exists.

## Levels

None emitted. tools/measure_preset_levels.py measures each rendered patch and writes the trims.
"""

import re
from pathlib import Path

ROOT = Path("/work/keith-plugin")
_h = (ROOT / "src" / "Keith.h").read_text()
P = {name: int(idx) for name, idx in re.findall(r"(kParam\w+)\s*=\s*(\d+)", _h)}


class Patch:
    def __init__(self, name, category, **deltas):
        self.name = name
        self.category = category
        self.deltas = deltas

    def rows(self, preset_index):
        out = []
        for key, value in self.deltas.items():
            v = value
            vs = f"{v:.0f}" if float(v).is_integer() else f"{v:g}"
            out.append((preset_index, P[key], vs))
        return out


def pad(name, cat, cutoff, reso, attack, release, morph=None, drift=0.22, **extra):
    """Slow, wide, drifting. Movement from an LFO on the cutoff."""
    if morph is None:
        d = dict(
            kParamEngine=0, kParamPolyMode=0,
            kParamOsc1Shape=40.0, kParamOsc1Level=76.0,
            kParamOsc2Shape=38.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=62.0,
            kParamSubLevel=28.0, kParamNoiseLevel=6.0,
        )
    else:
        d = dict(
            kParamEngine=0, kParamPolyMode=0,
            kParamOsc1Shape=28.0, kParamOsc1Level=48.0,
            kParamOsc2Level=0.0,
            kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=morph,
            kParamOsc3Level=84.0,
            kParamSubLevel=22.0, kParamNoiseLevel=4.0,
        )
    d.update(dict(
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=26.0,
        kParamFiltAttack=attack * 0.6, kParamFiltDecay=2200.0, kParamFiltSustain=52.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=2400.0, kParamAmpSustain=76.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=4.0, kParamUnisonDetune=20.0,
        kParamLfoRate=drift, kParamLfoDest=1.0, kParamLfoAmount=22.0,
        kParamStereoSpread=78.0,
        kParamTapeSat=16.0, kParamTapeMix=30.0,


))
    d.update(extra)
    return Patch(name, cat, **d)


def strings(name, cat, cutoff, reso, attack, release, **extra):
    """A section, not a soloist. Detune and a gentle filter so it reads as several players."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=42.0, kParamOsc1Level=78.0,
        kParamOsc2Shape=40.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=66.0,
        kParamSubLevel=18.0, kParamNoiseLevel=8.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=30.0,
        kParamFiltAttack=attack * 0.8, kParamFiltDecay=1600.0, kParamFiltSustain=54.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1800.0, kParamAmpSustain=78.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=4.0, kParamUnisonDetune=18.0,
        kParamFilterKeyTrack=36.0,
        kParamStereoSpread=72.0,
        kParamTapeSat=14.0, kParamTapeMix=26.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def fx(name, cat, cutoff, reso, attack, release, **extra):
    """Allowed to be unusable as a melody. That is what the category is for."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=56.0, kParamOsc1Level=80.0,
        kParamOsc2Shape=58.0, kParamOsc2Pitch=7.0, kParamOsc2Level=70.0,
        kParamOscRing=1.0,
        kParamNoiseLevel=34.0, kParamSubLevel=10.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=64.0,
        kParamFiltAttack=attack, kParamFiltDecay=2000.0, kParamFiltSustain=40.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=2400.0, kParamAmpSustain=62.0,
        kParamAmpRelease=release,
        kParamLfoRate=0.6, kParamLfoDest=16.0, kParamLfoAmount=72.0,
        kParamStereoSpread=86.0, kParamStereoDelay=40.0,



kParamTapeSat=30.0, kParamTapeMix=40.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def reed(name, cat, cutoff, reso, attack, release, breath=32.0, pw=28.0, **extra):
    """Narrow pulse plus breath. The noise is what makes it read as wind.

    POLY, not mono -- see the note in build_lead_patches.py. Every mono patch in the library
    measured a peak above 0 dBFS and no poly patch did, and the cause was not identified after
    five candidates were eliminated. These are voiced poly to sidestep it.
    """
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=0.0,
        kParamOsc1Shape=54.0, kParamOsc1PW=pw, kParamOsc1Level=82.0,
        kParamOsc2Shape=52.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=44.0,
        kParamSubLevel=26.0, kParamNoiseLevel=breath,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=38.0,
        kParamFiltAttack=12.0, kParamFiltDecay=1100.0, kParamFiltSustain=52.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1200.0, kParamAmpSustain=76.0,
        kParamAmpRelease=release,
        kParamVelToAmp=46.0, kParamVelToFilter=30.0,
        kParamStereoSpread=44.0, kParamStereoDelay=20.0,
        kParamTapeSat=18.0, kParamTapeMix=26.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def brass(name, cat, cutoff, reso, attack, release, blat=90.0, **extra):
    """The filter opens AFTER the note starts. That is the brass 'blat' and the whole character."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=40.0, kParamOsc1Level=84.0,
        kParamOsc2Shape=38.0, kParamOsc2Pitch=0.0, kParamOsc2Level=76.0,
        kParamSubLevel=24.0, kParamNoiseLevel=4.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=58.0,
        # The delayed filter attack is the point: a fast amp attack with a slower filter one.
        kParamFiltAttack=blat, kParamFiltDecay=900.0, kParamFiltSustain=46.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1200.0, kParamAmpSustain=74.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=14.0,
        kParamVelToAmp=40.0, kParamVelToFilter=46.0,
        kParamStereoSpread=56.0,
        kParamTapeSat=24.0, kParamTapeMix=32.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def build():
    patches = []

    # --- PADS (10) -----------------------------------------------------------------
    patches += [
        pad("Slow Motion Pad", "pad", 5200, 20, 420, 2800),
        pad("Drifting Warm Pad", "pad", 4600, 24, 380, 3000, drift=0.16),
        pad("Wide Chorus Pad", "pad", 6000, 18, 340, 2600),
        pad("Voice Bed Pad", "pad", 5000, 22, 460, 3200, morph=3.2),
        pad("Glass Pad", "pad", 7200, 16, 300, 2400, morph=2.4),
        pad("Deep Low Pad", "pad", 3200, 26, 500, 3400, drift=0.12,
            kParamSubLevel=44.0),
        pad("Air Motion Pad", "pad", 6400, 20, 360, 2800, drift=0.30,
            kParamNoiseLevel=14.0),
        pad("Dark Tape Pad", "pad", 4200, 24, 400, 3000,
            kParamTapeSat=34.0, kParamTapeWow=40.0, kParamTapeMix=44.0),
        pad("Bright Sweep Pad", "pad", 5600, 34, 340, 2600, drift=0.26,
            kParamLfoAmount=40.0),
        pad("Sparse Hollow Pad", "pad", 5800, 28, 440, 2900, morph=5.2,
            kParamSubLevel=16.0),
    ]

    # --- STRINGS (10) --------------------------------------------------------------
    patches += [
        strings("Silk Strings", "strings", 6200, 14, 140, 2200),
        strings("Warm Section Strings", "strings", 5400, 16, 180, 2000, kParamNoiseLevel=12.0),
        strings("Bright Strings", "strings", 7600, 12, 110, 1800),
        strings("Low Cello Bed", "strings", 4200, 18, 200, 2400, kParamOsc2Pitch=-24.0),
        strings("Slow Bow Strings", "strings", 5800, 14, 300, 2600),
        strings("Tight Strings", "strings", 6800, 16, 70, 1400),
        strings("Section Swell", "strings", 5600, 20, 260, 2800, kParamFilterEnv=42.0),
        strings("Dusty Strings", "strings", 5200, 18, 160, 2200,
                kParamTapeSat=32.0, kParamTapeWow=38.0, kParamTapeMix=42.0),
        strings("Octave Strings", "strings", 6400, 14, 130, 2000, kParamOsc2Pitch=0.0),
        strings("Thin High Strings", "strings", 8200, 12, 120, 1800, kParamSubLevel=8.0),
    ]

    # --- FX (10) -------------------------------------------------------------------
    patches += [
        fx("Ring Mod Sweep", "fx", 4800, 44, 20, 2400),
        fx("Noise Wash", "fx", 6200, 30, 400, 3200, kParamNoiseLevel=64.0,
           kParamOsc1Level=30.0),
        fx("Metal Riser", "fx", 3200, 52, 200, 2800, kParamLfoRate=1.6,
           kParamFilterEnv=84.0),
        fx("Detuned Siren", "fx", 5400, 48, 30, 2600, kParamLfoRate=2.4,
           kParamLfoAmount=88.0),
        fx("Tape Warble FX", "fx", 5600, 36, 60, 2400,
           kParamTapeWow=72.0, kParamTapeFlutter=64.0, kParamTapeMix=56.0),
        fx("Long Delay Drone", "fx", 5000, 40, 320, 3400, kParamStereoDelay=40.0),
        fx("Broken Radio", "fx", 3800, 56, 10, 2000, kParamTapeHiss=48.0,
           kParamTapePrint=40.0, kParamTapeMix=52.0),
        fx("Sub Drop FX", "fx", 2600, 46, 4, 1800, kParamPitchAmount=-24.0,
           kParamPitchDecay=600.0, kParamOscRing=0.0),
        fx("Shimmer Texture", "fx", 7800, 38, 240, 3000, kParamOscRing=0.0,
           kParamNoiseLevel=22.0),
        fx("Sweep Down FX", "fx", 6800, 50, 8, 2200, kParamPitchAmount=-24.0,
           kParamPitchDecay=900.0),
    ]

    # --- REEDS / FLUTES (10) --------------------------------------------------------
    patches += [
        reed("Hollow Reed", "reeds", 5600, 26, 30, 900),
        reed("Breathy Flute", "reeds", 7200, 18, 40, 1000, breath=52.0, pw=50.0),
        reed("Narrow Oboe", "reeds", 6400, 32, 24, 850, breath=18.0, pw=18.0),
        reed("Warm Clarinet", "reeds", 5000, 30, 28, 900, breath=14.0, pw=22.0),
        reed("Airy Pan Flute", "reeds", 6800, 20, 60, 1200, breath=64.0, pw=50.0),
        reed("Bright Shawm", "reeds", 7800, 34, 18, 800, breath=24.0, pw=16.0),
        reed("Soft Bassoon", "reeds", 3800, 28, 36, 1000, breath=16.0, pw=20.0),
        reed("Dusty Reed", "reeds", 5400, 28, 32, 950, breath=30.0,
             kParamTapeSat=30.0, kParamTapeWow=36.0, kParamTapeMix=40.0),
        reed("Thin Whistle", "reeds", 8600, 22, 26, 900, breath=44.0, pw=44.0),
        reed("Wide Reed Section", "reeds", 6000, 24, 44, 1100, breath=26.0,
             kParamUnisonVoices=3.0, kParamUnisonDetune=18.0),
    ]

    # --- BRASS (10) -----------------------------------------------------------------
    patches += [
        brass("Warm Brass", "brass", 4600, 24, 30, 900),
        brass("Bright Brass Section", "brass", 5800, 22, 24, 850, blat=70.0,
              kParamUnisonVoices=3.0, kParamUnisonDetune=16.0),
        brass("Low Brass Bed", "brass", 3600, 28, 40, 1000, blat=110.0),
        brass("Punch Brass Stab", "brass", 5200, 30, 6, 500, blat=50.0,
              kParamFilterEnv=72.0),
        brass("Soft French Horn", "brass", 4200, 20, 60, 1200, blat=140.0,
              kParamNoiseLevel=8.0),
        brass("Hard Brass Stack", "brass", 6200, 34, 14, 700, blat=60.0,
              kParamFilterDrive=30.0),
        brass("Tape Brass", "brass", 4800, 26, 32, 900, blat=95.0,
              kParamTapeSat=34.0, kParamTapeWow=32.0, kParamTapeMix=42.0),
        brass("Muted Brass", "brass", 3800, 36, 26, 800, blat=80.0, kParamFilterEnv=40.0),
        brass("Wide Brass Swell", "brass", 5000, 24, 90, 1400, blat=180.0,
              kParamUnisonVoices=4.0, kParamUnisonDetune=20.0),
        brass("Sharp Brass Lead", "brass", 6600, 30, 12, 650, blat=44.0,
              kParamVelToFilter=54.0),
    ]

    assert len(patches) == 50, f"expected 50, built {len(patches)}"
    return patches


def main():
    patches = build()
    by_cat = {}
    for p in patches:
        by_cat.setdefault(p.category, []).append(p.name)
    print(f"// {len(patches)} patches")
    for cat, names in by_cat.items():
        print(f"// {cat}: {len(names)}")
    print(f"// {sum(len(p.deltas) for p in patches)} parameter deltas")
    print()
    for p in patches:
        print(f'  {{"{p.name}", 7, "mags", "{p.category}"}},')


if __name__ == "__main__":
    main()
