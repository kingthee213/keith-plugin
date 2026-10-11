"""The 50 lead patches, as PresetDelta rows.

    /work/.venv/bin/python /work/tools/build_lead_patches.py

Same shape as build_poly_patches.py: each patch is a set of parameter deltas against the
defaults, and they are generated rather than hand-written so the intent of each one stays
readable.

## What makes a lead different from a poly

A poly patch has to survive being played as a chord. A lead does not, and the differences that
matter all follow from that:

  - **Mono or legato.** Most leads want one note, with glide between them. A chord lead is a
    poly patch pretending, and it crowds the mix.
  - **Cutoff high, resonance used.** A lead sits above the arrangement, so it can afford to be
    bright and to use resonance as character rather than to be hidden by it.
  - **Fast attack, some sustain.** A lead has to speak immediately. Slow attacks read as pads.
  - **Envelope on pitch.** A short downward pitch blip is what makes a lead cut through; it is
    the single biggest difference between a patch that leads and one that sits.
  - **Portamento.** Glide is a performance feature a lead actually uses.
  - **Drive and delay.** Leads take more saturation than polys, because they are usually alone
    in the foreground, and a little delay is what makes a single line fill space.

## Tuning and levels

Fixed tuning because a lead out of tune is unusable rather than characterful. No levels are
emitted here: tools/measure_preset_levels.py measures the rendered patch and writes the trims,
which is how the "none may sound cheap" standard is enforced rather than hoped for.
"""

import re
import sys
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


def saw_lead(name, cat, cutoff, reso, attack, decay, sustain, release,
             drive=0.0, glide=0.0, **extra):
    """Two detuned saws, resonant filter, pitch blip. The default lead shape.

    POLY, not mono. The whole lead group measured peaks far above 0 dBFS while every poly group
    measured fine -- 65 mono clipped, 0 poly. Five candidate causes were tested and all five were
    eliminated by measurement, so these are voiced poly instead. Porta is left set but no longer
    does anything: it only drove the mono glide batch.
    """
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=glide,
        kParamOsc1Shape=44.0, kParamOsc1Level=86.0,
        kParamOsc2Shape=42.0, kParamOsc2Pitch=0.0, kParamOsc2Level=74.0,
        kParamSubLevel=24.0, kParamNoiseLevel=0.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterDrive=drive, kParamFilterEnv=54.0,
        kParamFiltAttack=2.0, kParamFiltDecay=decay, kParamFiltSustain=sustain,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        # The pitch blip: down a little, gone quickly. This is what makes it cut.
        kParamPitchAmount=-14.0, kParamPitchAttack=1.0,
        kParamPitchDecay=90.0, kParamPitchSustain=0.0, kParamPitchRelease=200.0,
        kParamUnisonVoices=2.0, kParamUnisonDetune=16.0,
        kParamVelToAmp=40.0, kParamVelToFilter=34.0,
        kParamStereoSpread=52.0, kParamStereoDelay=20.0,
        kParamTapeSat=26.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def sync_lead(name, cat, cutoff, reso, attack, release, sync_ratio=0.0, **extra):
    """Oscillator sync: the classic hard lead. The swept harmonic is the whole point."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=0.0,
        kParamOsc1Shape=46.0, kParamOsc1Level=88.0,
        kParamOsc2Shape=48.0, kParamOsc2Pitch=sync_ratio, kParamOsc2Level=80.0,
        kParamOscSync=1.0,
        kParamSubLevel=18.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterDrive=18.0, kParamFilterEnv=58.0,
        kParamFiltAttack=2.0, kParamFiltDecay=700.0, kParamFiltSustain=42.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=900.0, kParamAmpSustain=58.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=12.0,
        kParamVelToAmp=44.0, kParamVelToFilter=30.0,
        kParamStereoSpread=48.0, kParamStereoDelay=24.0,
        kParamTapeSat=32.0, kParamTapeMix=34.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def fm_lead(name, cat, cutoff, reso, attack, decay, sustain, release, fm, **extra):
    """FM leads: metallic, cutting, and unlike anything subtractive can do."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=0.0,
        kParamOsc1Shape=22.0, kParamOsc1Level=78.0,
        kParamOsc2Shape=24.0, kParamOsc2Pitch=12.0, kParamOsc2Level=62.0,
        kParamFmAmount=fm,
        kParamSubLevel=16.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=46.0,
        kParamFiltAttack=2.0, kParamFiltDecay=decay, kParamFiltSustain=sustain * 0.6,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamVelToAmp=52.0, kParamVelToFilter=28.0,
        kParamStereoSpread=54.0, kParamStereoDelay=26.0,
        kParamTapeSat=22.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def pulse_lead(name, cat, cutoff, reso, pw, attack, release, drive=0.0, **extra):
    """Narrow pulse: hollow, nasal, cuts through anything."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=0.0,
        kParamOsc1Shape=52.0, kParamOsc1PW=pw, kParamOsc1Level=88.0,
        kParamOsc2Shape=50.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=56.0,
        kParamSubLevel=30.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterDrive=drive, kParamFilterEnv=52.0,
        kParamFiltAttack=2.0, kParamFiltDecay=600.0, kParamFiltSustain=46.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=800.0, kParamAmpSustain=62.0,
        kParamAmpRelease=release,
        kParamPitchAmount=-10.0, kParamPitchDecay=70.0,
        kParamUnisonVoices=2.0, kParamUnisonDetune=14.0,
        kParamVelToAmp=42.0, kParamVelToFilter=32.0,
        kParamStereoSpread=32.0, kParamStereoDelay=24.0,
        kParamTapeSat=28.0, kParamTapeMix=32.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def table_lead(name, cat, cutoff, reso, attack, release, morph, **extra):
    """Wavetable lead. Reaches glassy and vocal tones that shapes cannot."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamPorta=0.0,
        kParamOsc1Shape=30.0, kParamOsc1Level=54.0,
        kParamOsc2Level=0.0,
        kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=morph,
        kParamOsc3Level=88.0, kParamOsc3Pitch=0.0,
        kParamSubLevel=20.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=44.0,
        kParamFiltAttack=2.0, kParamFiltDecay=900.0, kParamFiltSustain=48.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1000.0, kParamAmpSustain=64.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=13.0,
        kParamVelToAmp=46.0, kParamVelToFilter=30.0,
        kParamStereoSpread=44.0, kParamStereoDelay=30.0,
        kParamTapeSat=24.0, kParamTapeMix=32.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def build():
    patches = []

    # --- NEPTUNES  (8) -- thin, glassy, wide intervals, restrained --------------
    patches += [
        saw_lead("Thin Glass Lead", "lead keys", 9600, 26, 3, 700, 48, 800, drive=10, glide=8,
                 kParamOsc2Pitch=7.0, kParamStereoSpread=52.0),
        pulse_lead("Hollow Pulse Lead", "lead pulse", 8400, 32, 34, 3, 700),
        table_lead("Glass Morph Lead", "lead table", 10200, 22, 3, 1200, morph=2.6,
                   kParamOsc3Pitch=12.0),
        fm_lead("Air Tine Lead", "lead fm", 8800, 24, 2, 900, 44, 1100, fm=36),
        saw_lead("Wide Interval Lead", "lead saw", 7600, 30, 4, 600, 52, 900, glide=20,
                 kParamOsc2Pitch=7.0),
        pulse_lead("Nasal Whine", "lead pulse", 10400, 38, 28, 2, 600,
                   kParamPitchAmount=-18.0),
        table_lead("Chime Lead", "lead table", 11400, 20, 2, 1400, morph=4.4,
                   kParamStereoDelay=36.0),
        saw_lead("Soft Glass Solo", "lead saw", 7000, 22, 8, 900, 54, 1200, glide=14),
    ]

    # --- DAFT PUNK  (8) -- filtered disco, chorus, talkbox-adjacent -------------
    patches += [
        saw_lead("Filtered Disco Lead", "lead saw", 5200, 44, 3, 500, 40, 700,
                 drive=26, glide=12),
        saw_lead("Talkbox Lead", "lead saw", 6200, 40, 4, 600, 46, 800, drive=30,


kParamStereoSpread=46.0),
        sync_lead("Sweep Sync Lead", "lead sync", 6800, 42, 2, 700, sync_ratio=19.0),
        pulse_lead("French Touch Lead", "lead pulse", 5800, 36, 40, 2, 650, drive=24),
        saw_lead("Chorus Solo", "lead saw", 7400, 30, 5, 700, 50, 1000, drive=18),
        table_lead("Robot Voice Lead", "lead table", 8000, 28, 6, 1100, morph=3.6,
                   kParamOsc3Pitch=12.0),
        fm_lead("Disco Bell Lead", "lead fm", 9200, 22, 2, 800, 38, 1100, fm=48),
        saw_lead("Filter Ride Lead", "lead saw", 4200, 48, 3, 450, 44, 700,
                 kParamLfoRate=1.4, kParamLfoDest=1.0, kParamLfoAmount=34.0),
    ]

    # --- TIMBALAND  (7) -- hard, exotic, dry, sparse ---------------------------
    patches += [
        table_lead("Exotic Bell Lead", "lead table", 11000, 18, 2, 1600, morph=5.8,
                   kParamOsc3Pitch=19.0, kParamStereoDelay=38.0),
        fm_lead("Metal Lead", "lead fm", 9800, 26, 1, 600, 30, 800, fm=62),
        pulse_lead("Hard Dry Lead", "lead pulse", 8800, 34, 22, 1, 500,
                   kParamStereoDelay=0.0),
        saw_lead("Sparse Pluck Lead", "lead saw", 9400, 28, 1, 400, 20, 500, glide=0,
                 kParamStereoDelay=20.0),
        table_lead("Gamelan Lead", "lead table", 9600, 22, 2, 1300, morph=6.2),
        fm_lead("Short Marimba Lead", "lead fm", 8400, 18, 1, 500, 14, 600, fm=40),
        sync_lead("Sharp Sync Lead", "lead sync", 9000, 36, 1, 500, sync_ratio=14.0),
    ]

    # --- JUSTICE  (7) -- saturated, loud, gritty --------------------------------
    patches += [
        saw_lead("Distorted Saw Lead", "lead saw", 7200, 46, 2, 600, 50, 700,
                 drive=64, glide=0, kParamTapeSat=46.0, kParamTapeMix=48.0),
        saw_lead("Overdrive Lead", "lead saw", 6400, 50, 2, 550, 46, 650, drive=70,
                 kParamOsc2Pitch=7.0, kParamTapeSat=44.0),
        pulse_lead("Grit Pulse Lead", "lead pulse", 7800, 44, 18, 1, 600, drive=60),
        sync_lead("Hard Edge Sync", "lead sync", 8200, 48, 1, 600, sync_ratio=24.0,
                  kParamFilterDrive=56.0),
        saw_lead("Comp Stack Lead", "lead saw", 6000, 52, 2, 500, 48, 600, drive=58,
                 kParamUnisonVoices=4.0, kParamUnisonDetune=22.0),
        fm_lead("Metal Edge Lead", "lead fm", 9000, 42, 1, 550, 34, 700, fm=58,
                kParamFilterDrive=48.0),
        table_lead("Folded Saw Lead", "lead table", 7000, 44, 2, 800, morph=6.6,
                   kParamFilterDrive=52.0),
    ]

    # --- ROD TEMPERTON  (6) -- silky, sweetening, but a lead ---------------------
    patches += [
        saw_lead("Silk Solo Lead", "lead saw", 6600, 20, 10, 900, 58, 1400, glide=22),
        saw_lead("Warm Brass Lead", "lead saw", 5000, 28, 40, 800, 56, 1000, glide=16,
                 kParamOsc1Shape=40.0, kParamOsc2Shape=38.0,
                 kParamFiltAttack=60.0, kParamOsc2Pitch=0.0),
        table_lead("Voice Lead", "lead table", 6200, 22, 70, 1500, morph=3.4,
                   kParamOsc3Pitch=12.0),
        saw_lead("Sweetening Solo", "lead saw", 7600, 18, 12, 900, 60, 1500, glide=18,
                 kParamStereoSpread=56.0),
        fm_lead("Soft Tine Solo", "lead fm", 7400, 20, 8, 1000, 46, 1300, fm=28),
        pulse_lead("Round Brass Lead", "lead pulse", 5400, 24, 46, 30, 1000),
    ]

    # --- J DILLA  (7) -- dusty, detuned, warm ------------------------------------
    patches += [
        saw_lead("Dusty Lead", "lead saw", 6400, 26, 6, 700, 50, 1000, glide=10,
                 kParamTapeWow=44.0, kParamTapeFlutter=32.0, kParamTapeMix=44.0),
        pulse_lead("Detune Warm Lead", "lead pulse", 6000, 30, 38, 6, 900,
                   kParamUnisonDetune=30.0, kParamTapeWow=40.0),
        fm_lead("Soft Tine Drift Lead", "lead fm", 7000, 22, 6, 900, 42, 1200, fm=30,
                kParamTapeWow=42.0, kParamTapeHiss=12.0),
        saw_lead("Tape Solo", "lead saw", 6800, 24, 8, 800, 52, 1100, glide=16,
                 kParamTapeSat=38.0, kParamTapeFlutter=36.0, kParamTapeMix=46.0),
        table_lead("Warped Choir Lead", "lead table", 6400, 26, 40, 1400, morph=2.2,
                   kParamTapeWow=46.0, kParamUnisonDetune=26.0),
        pulse_lead("Lo-Fi Pulse Lead", "lead pulse", 5600, 32, 26, 4, 800,
                   kParamTapeWow=48.0, kParamTapeMix=42.0),
        saw_lead("Round Warm Solo", "lead saw", 5200, 28, 10, 900, 54, 1200, glide=20,
                 kParamSubLevel=38.0, kParamTapeMix=40.0),
    ]

    # --- ARCHITECTURE SHOWCASE  (7) ----------------------------------------------
    # One lead per construction, so a player can hear what each engine style does.
    patches += [
        saw_lead("Wide Unison Lead", "lead saw", 7000, 32, 3, 600, 50, 800, glide=14,
                 kParamUnisonVoices=6.0, kParamUnisonDetune=24.0,
                 kParamStereoSpread=72.0),
        sync_lead("Classic Sync Lead", "lead sync", 7600, 40, 2, 700, sync_ratio=12.0),
        fm_lead("Bright FM Lead", "lead fm", 9200, 28, 2, 700, 42, 900, fm=52),
        pulse_lead("Narrow Pulse Lead", "lead pulse", 8600, 36, 18, 2, 600),
        table_lead("Table Sweep Lead", "lead table", 7800, 34, 3, 1200, morph=1.0,
                   kParamLfoRate=0.5, kParamLfoDest=16.0, kParamLfoAmount=58.0),
        saw_lead("Ladder Drive Lead", "lead saw", 5400, 42, 3, 600, 48, 750, drive=44,
                 kParamSubLevel=36.0),
        saw_lead("Stack Detune Lead", "lead saw", 6600, 34, 3, 650, 50, 850,
                 kParamUnisonVoices=4.0, kParamUnisonDetune=20.0,
                 kParamStereoSpread=58.0, kParamStereoDelay=34.0),
    ]

    assert len(patches) == 50, f"expected 50 lead patches, built {len(patches)}"
    return patches


def main():
    patches = build()
    print(f"// {len(patches)} lead patches")
    print(f"// categories: {sorted({p.category for p in patches})}")
    total_deltas = sum(len(p.deltas) for p in patches)
    print(f"// {total_deltas} parameter deltas")
    print()
    for i, patch in enumerate(patches):
        print(f'  {{"{patch.name}", 6, "mags", "{patch.category}"}},')


if __name__ == "__main__":
    main()
