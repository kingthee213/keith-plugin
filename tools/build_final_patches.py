"""The final 20 patches, completing the library.

    /work/.venv/bin/python /work/keith-plugin/tools/build_final_patches.py

The owner's 200 breaks down as 160 specified plus 40 to be named. The 20 keyboards are done
(build_keyboard_patches.py); this is the other 20.

## Why these 20 and not something else

They fill the gaps left by the 160, rather than inventing new territory. The 160 gave the library
50 polys, 50 leads and only 10 each of pads, strings, FX, reeds and brass -- so the thin
categories are where more is worth having. The split:

    6  pads        the joint-thinnest category, and the one most libraries are short of
    5  strings     orchestral and synthetic, which the engine can now reach
    5  basses       the 160 had no bass at all, which is a real hole
    4  vocal/choir  the tape-flute and formant work makes these possible now

## Naming

Names describe the sound only. No manufacturer, no product, no artist, no trademark -- the same
rule that cost "Juno Simple Pad", "Prophet Poly Brass" and "Tape Rhodes" their original names.

## Levels

None emitted. tools/measure_preset_levels.py measures and writes the trims.
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


def pad(name, cutoff, reso, attack, release, morph=None, drift=0.22, **extra):
    if morph is None:
        d = dict(
            kParamEngine=0, kParamPolyMode=0,
            kParamOsc1Shape=40.0, kParamOsc1Level=74.0,
            kParamOsc2Shape=38.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=60.0,
            kParamSubLevel=30.0, kParamNoiseLevel=6.0,
        )
    else:
        d = dict(
            kParamEngine=0, kParamPolyMode=0,
            kParamOsc1Shape=26.0, kParamOsc1Level=46.0,
            kParamOsc2Level=0.0,
            kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=morph,
            kParamOsc3Level=84.0,
            kParamSubLevel=20.0, kParamNoiseLevel=4.0,
        )
    d.update(dict(
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=26.0,
        kParamFiltAttack=attack * 0.6, kParamFiltDecay=2200.0, kParamFiltSustain=52.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=2400.0, kParamAmpSustain=76.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=4.0, kParamUnisonDetune=20.0,
        kParamLfoRate=drift, kParamLfoDest=1.0, kParamLfoAmount=20.0,
        kParamStereoSpread=80.0,
        kParamTapeSat=16.0, kParamTapeMix=30.0,
        kParamIns1Algo=2.0, kParamIns1C=64.0, kParamIns1Mix=26.0,
    ))
    d.update(extra)
    return Patch(name, "pad", **d)


def strings(name, cutoff, reso, attack, release, **extra):
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
        kParamFilterKeyTrack=36.0, kParamStereoSpread=74.0,
        kParamTapeSat=14.0, kParamTapeMix=26.0,
    )
    d.update(extra)
    return Patch(name, "strings", **d)


def bass(name, cutoff, reso, attack, decay, sustain, release, drive=0.0, **extra):
    """A bass patch. Mono, sub-forward, with the filter doing the work rather than the oscillator."""
    d = dict(
        kParamEngine=0, kParamPolyMode=1, kParamMonoLegato=1,
        kParamPorta=0.0,
        kParamOsc1Shape=38.0, kParamOsc1Level=88.0,
        kParamOsc2Shape=36.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=54.0,
        kParamSubLevel=62.0, kParamNoiseLevel=2.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterDrive=drive, kParamFilterEnv=46.0,
        kParamFiltAttack=2.0, kParamFiltDecay=decay, kParamFiltSustain=sustain * 0.6,
        kParamFiltRelease=release * 0.7,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamVelToAmp=38.0, kParamVelToFilter=40.0,
        kParamStereoSpread=0.0, kParamStereoDelay=0.0,
        kParamTapeSat=26.0, kParamTapeMix=32.0,
    )
    d.update(extra)
    return Patch(name, "bass", **d)


def choir(name, cutoff, reso, attack, release, morph, **extra):
    """A vocal patch: the wavetable bank's formant frames carry the vowel."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=26.0, kParamOsc1Level=40.0,
        kParamOsc2Level=0.0,
        kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=morph,
        kParamOsc3Level=88.0, kParamOsc3Pitch=12.0,
        kParamSubLevel=16.0, kParamNoiseLevel=8.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=22.0,
        kParamFiltAttack=attack * 0.7, kParamFiltDecay=2000.0, kParamFiltSustain=58.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=2200.0, kParamAmpSustain=78.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=4.0, kParamUnisonDetune=16.0,
        kParamStereoSpread=82.0,
        kParamTapeSat=18.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, "choir", **d)


def build():
    patches = []

    # --- PADS (6) ----------------------------------------------------------------------
    patches += [
        pad("Frozen Pad",        6800, 18, 500, 3400, drift=0.14),
        pad("Hollow Sweep Pad",  5400, 32, 420, 3000, drift=0.28, kParamLfoAmount=38.0),
        pad("Low Bed Pad",       3800, 24, 560, 3600, drift=0.12, kParamSubLevel=46.0),
        pad("Formant Pad",       6000, 20, 440, 3200, morph=4.8),
        pad("Worn Pad",          4800, 22, 400, 3000,
            kParamTapeSat=36.0, kParamTapeWow=44.0, kParamTapeMix=46.0),
        pad("Glass Drift Pad",   8200, 16, 380, 2800, morph=2.6, drift=0.24),
    ]

    # --- STRINGS (5) -------------------------------------------------------------------
    patches += [
        strings("Synthetic Strings",  7000, 14, 150, 2000),
        strings("Velvet Strings",     5800, 16, 200, 2200, kParamSubLevel=24.0),
        strings("High Strings",       8600, 12, 130, 1900, kParamSubLevel=8.0),
        strings("Deep Strings",       4600, 18, 220, 2400, kParamOsc2Pitch=-24.0),
        strings("Tape Strings Bed",   5600, 18, 180, 2200,
                kParamTapeSat=34.0, kParamTapeWow=40.0, kParamTapeMix=44.0),
    ]

    # --- BASSES (5) -- the 160 had none ------------------------------------------------
    patches += [
        bass("Round Sub Bass",    1800, 26, 3, 700, 40, 420),
        bass("Bright Bass",       3600, 34, 2, 500, 34, 380, drive=22.0),
        bass("Warm Bottom",       1200, 22, 5, 900, 48, 520),
        bass("Grit Bass",         2800, 44, 2, 480, 30, 360, drive=58.0,
             kParamTapeSat=40.0),
        bass("Tape Bass",         2200, 28, 4, 650, 42, 450,
             kParamTapeWow=40.0, kParamTapeFlutter=30.0, kParamTapeMix=44.0),
    ]

    # --- CHOIR / VOCAL (4) -------------------------------------------------------------
    patches += [
        choir("Choir Ahh",        6000, 16, 220, 2400, morph=2.8),
        choir("Choir Ooh",        5200, 18, 260, 2600, morph=5.4),
        choir("Wide Choir Bed",   5600, 20, 300, 2800, morph=3.0,
              kParamUnisonVoices=6.0, kParamStereoSpread=90.0),
        choir("Broken Choir",     5400, 22, 240, 2500, morph=4.2,
              kParamTapeWow=54.0, kParamTapeFlutter=44.0, kParamTapeMix=52.0),
    ]

    assert len(patches) == 20, f"expected 20 final patches, built {len(patches)}"
    return patches


def main():
    patches = build()
    from collections import Counter
    print(f"// {len(patches)} final patches")
    for cat, n in sorted(Counter(p.category for p in patches).items()):
        print(f"//   {cat:12s} {n}")
    print(f"// {sum(len(p.deltas) for p in patches)} parameter deltas")
    print()
    for p in patches:
        print(f'    {{"{p.name}", 8, "mags", "{p.category}"}},')


if __name__ == "__main__":
    main()
