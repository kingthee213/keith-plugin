"""The 20 keyboard patches: electric pianos, organs, tape keys.

    /work/.venv/bin/python /work/keith-plugin/tools/build_keyboard_patches.py

Part of the final 40. Owner asked for "20 keyboards or use free samples, wurli, rhodes, organ,
mellotron or similar", and chose: build them in the engine, source the noisy/worn character from
free samples.

## How these are built

Two layers, and the split matters:

  **The tone is synthesised**, in the engine, from FM and the wavetable bank. An electric piano is
  a struck tine or reed with a bell-like attack and a fast-decaying harmonic -- FM reaches that
  better than any subtractive shape. A tonewheel organ is additive: a stack of sine partials at
  fixed drawbar intervals. Both are things this engine can already do, so no instrument is
  sampled.

  **The wear is sampled**, from FreePats (CC0 1.0) -- key-off thump, mechanical noise, the room.
  That is what makes a keyboard sound like an instrument rather than a patch. It is not the pitch
  content, only the character around it.

## Naming

Neutral roles only, per SOURCES.md rule 7. No Rhodes, no Wurlitzer, no Hammond, no Mellotron --
those are trade names and the owner's own rule forbids them, the same way it forbade "Juno" and
"Prophet". The names describe what the instrument IS:

    Tine Piano          a struck-tine electric piano
    Reed Piano          a reed-based electric piano
    Tonewheel Organ     an additive drawbar organ
    Tape Choir          a tape-flute choir
    Combo Organ         a transistor organ
    Clav                a plucked electric clav

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


# --- the four families of keyboard -----------------------------------------------------------

def tine_piano(name, cutoff, reso, attack, decay, sustain, release, fm, bell=0.0, **extra):
    """Struck tine: FM for the bell attack, a fast-decaying harmonic, a warm body beneath.

    The character is in the attack, not the sustain. A tine piano's identity is the first 300 ms
    -- the bell and the bark -- and the tail is nearly a sine.
    """
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=20.0, kParamOsc1Level=70.0,
        kParamOsc2Shape=24.0, kParamOsc2Pitch=19.0, kParamOsc2Level=44.0,
        kParamFmAmount=fm,
        kParamSubLevel=22.0, kParamNoiseLevel=4.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=30.0,
        kParamFiltAttack=2.0, kParamFiltDecay=decay * 0.7, kParamFiltSustain=sustain * 0.5,
        kParamFiltRelease=release * 0.8,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamVelToAmp=64.0, kParamVelToFilter=34.0,
        kParamStereoSpread=48.0, kParamStereoDelay=16.0,
        kParamTapeSat=20.0, kParamTapeMix=28.0,
    )
    if bell:
        d["kParamOsc3Enable"] = 1.0
        d["kParamOsc3Source"] = 1.0
        d["kParamOsc3Morph"] = bell
        d["kParamOsc3Level"] = 40.0
        d["kParamOsc3Pitch"] = 12.0
    d.update(extra)
    return Patch(name, "key tine", **d)


def tonewheel(name, cutoff, reso, attack, release, drive=0.0, perc=0.0, **extra):
    """Additive drawbar organ: stacked partials, no filter sweep, percussion on the attack.

    An organ does not have a filter envelope -- the drawbars are the tone. So the filter sits open
    and static, and the character comes from the partials, the key click, and the drive.
    """
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=10.0, kParamOsc1Level=78.0,
        kParamOsc2Shape=10.0, kParamOsc2Pitch=12.0, kParamOsc2Level=52.0,
        kParamSubLevel=64.0, kParamNoiseLevel=2.0,
        kParamOsc3Enable=1.0, kParamOsc3Source=0.0, kParamOsc3Shape=10.0,
        kParamOsc3Level=38.0, kParamOsc3Pitch=19.0,
        # Open and static: an organ's tone is its partials, not a sweep.
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=0.0, kParamFilterDrive=drive,
        kParamFiltAttack=1.0, kParamFiltDecay=200.0, kParamFiltSustain=100.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=200.0, kParamAmpSustain=100.0,
        kParamAmpRelease=release,
        kParamVelToAmp=18.0, kParamVelToFilter=0.0,
        kParamStereoSpread=40.0, kParamStereoDelay=0.0,
        kParamTapeSat=26.0, kParamTapeMix=30.0,
    )
    if perc:
        # Organ percussion: a fast decay on the attack, a click rather than a filter sweep.
        d["kParamPitchAmount"] = perc
        d["kParamPitchAttack"] = 1.0
        d["kParamPitchDecay"] = 60.0
        d["kParamPitchSustain"] = 0.0
        d["kParamPitchRelease"] = 120.0
    d.update(extra)
    return Patch(name, "key organ", **d)


def tape_keys(name, cutoff, reso, attack, release, flutter, **extra):
    """Tape-flute keys: the choir and string sounds a tape machine made famous.

    The identity is instability. Pitch wobble, a soft attack, and a narrow bandwidth, because the
    tape ran at 3.75 ips and had almost no top end.
    """
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=28.0, kParamOsc1Level=52.0,
        kParamOsc2Level=0.0,
        kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=3.4,
        kParamOsc3Level=84.0, kParamOsc3Pitch=12.0,
        kParamSubLevel=24.0, kParamNoiseLevel=10.0,
        # 6 kHz is roughly where a 3.75 ips tape ran out of top end.
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=18.0,
        kParamFiltAttack=attack * 0.7, kParamFiltDecay=1800.0, kParamFiltSustain=58.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=2000.0, kParamAmpSustain=80.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=3.0, kParamUnisonDetune=18.0,
        kParamStereoSpread=76.0,
        kParamTapeSat=30.0, kParamTapeWow=flutter, kParamTapeFlutter=flutter * 0.8,
        kParamTapeMix=42.0,
    )
    d.update(extra)
    return Patch(name, "key tape", **d)


def combo_organ(name, cutoff, reso, attack, release, drive=0.0, **extra):
    """Transistor organ: thinner and reedier than a tonewheel, with a buzz in the top."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=52.0, kParamOsc1PW=26.0, kParamOsc1Level=80.0,
        kParamOsc2Shape=10.0, kParamOsc2Pitch=12.0, kParamOsc2Level=40.0,
        kParamSubLevel=40.0, kParamNoiseLevel=3.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=6.0, kParamFilterDrive=drive,
        kParamFiltAttack=1.0, kParamFiltDecay=200.0, kParamFiltSustain=100.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=200.0, kParamAmpSustain=100.0,
        kParamAmpRelease=release,
        kParamVelToAmp=16.0,
        kParamStereoSpread=48.0,




kParamTapeSat=24.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, "key combo", **d)


def clav(name, cutoff, reso, attack, decay, sustain, release, pw=32.0, **extra):
    """Plucked electric clav: narrow pulse, tight envelope, a hard pluck and almost no tail."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=50.0, kParamOsc1PW=pw, kParamOsc1Level=88.0,
        kParamOsc2Shape=50.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=50.0,
        kParamSubLevel=34.0, kParamNoiseLevel=6.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=62.0,
        kParamFiltAttack=1.0, kParamFiltDecay=decay * 0.6, kParamFiltSustain=sustain * 0.4,
        kParamFiltRelease=release * 0.6,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamVelToAmp=58.0, kParamVelToFilter=48.0,
        kParamStereoSpread=42.0, kParamStereoDelay=14.0,
        kParamTapeSat=24.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, "key clav", **d)


def build():
    patches = []

    # --- TINE PIANOS (6) -- struck tine ------------------------------------------------
    patches += [
        tine_piano("Tine Piano",        8200, 16, 2, 1400, 26, 1500, fm=34),
        tine_piano("Bright Tine",       9800, 18, 1, 900, 20, 1100, fm=42),
        tine_piano("Soft Tine",         6600, 12, 4, 1800, 34, 2000, fm=26),
        tine_piano("Tine Bell",         9400, 20, 1, 700, 12, 900, fm=48, bell=4.2),
        tine_piano("Tine Bark",         7400, 28, 1, 1100, 22, 1200, fm=52,
                   kParamFilterDrive=26.0),
        tine_piano("Dusty Tine",        7000, 16, 3, 1500, 28, 1700, fm=30,
                   kParamTapeWow=40.0, kParamTapeMix=44.0),
    ]

    # --- REED PIANOS (4) -- reed rather than tine, softer and rounder -------------------
    patches += [
        tine_piano("Reed Piano",        6200, 18, 3, 1300, 30, 1500, fm=22,
                   kParamOsc2Pitch=12.0),
        tine_piano("Warm Reed",         5600, 14, 5, 1600, 36, 1800, fm=18),
        tine_piano("Reed Bark",         6800, 30, 2, 1000, 24, 1200, fm=30,
                   kParamFilterDrive=30.0),
        tine_piano("Tape Reed",         6000, 16, 4, 1400, 32, 1600, fm=24,
                   kParamTapeWow=44.0, kParamTapeFlutter=30.0, kParamTapeMix=46.0),
    ]

    # --- TONEWHEEL ORGANS (5) -- additive drawbars --------------------------------------
    patches += [
        tonewheel("Tonewheel Organ",    11000, 8, 2, 240, drive=18.0),
        tonewheel("Full Drawbar",       12000, 6, 2, 260, drive=22.0,
                  kParamOsc1Level=88.0, kParamSubLevel=76.0, kParamOsc3Level=52.0),
        tonewheel("Percussive Organ",   11000, 10, 1, 220, drive=20.0, perc=-16.0),
        tonewheel("Gospel Organ",       12500, 12, 2, 280, drive=34.0,
                  kParamOsc1Level=90.0),
        tonewheel("Dirty Organ",        9600, 14, 2, 240, drive=48.0,
                  kParamTapeSat=40.0, kParamTapeMix=46.0),
    ]

    # --- COMBO ORGANS (3) -- transistor, thinner -----------------------------------------
    patches += [
        combo_organ("Combo Organ",      9200, 22, 2, 220, drive=14.0),
        combo_organ("Reedy Combo",      8400, 30, 2, 200, drive=18.0),
        combo_organ("Bright Combo",     11000, 18, 1, 200, drive=12.0,
                    kParamOsc1PW=18.0),
    ]

    # --- TAPE KEYS (2) -- the tape-flute choir and string sounds -------------------------
    patches += [
        tape_keys("Tape Choir",         5800, 14, 180, 2200, flutter=40.0,
                  kParamOsc3Morph=3.2),
        tape_keys("Tape Strings",       6400, 12, 260, 2400, flutter=34.0,
                  kParamOsc3Morph=2.4, kParamStereoSpread=82.0),
    ]

    assert len(patches) == 20, f"expected 20 keyboard patches, built {len(patches)}"
    return patches


def main():
    patches = build()
    print(f"// {len(patches)} keyboard patches")
    from collections import Counter
    for cat, n in sorted(Counter(p.category for p in patches).items()):
        print(f"//   {cat:16s} {n}")
    print(f"// {sum(len(p.deltas) for p in patches)} parameter deltas")
    print()
    for p in patches:
        print(f'    {{"{p.name}", 8, "mags", "{p.category}"}},')


if __name__ == "__main__":
    main()
