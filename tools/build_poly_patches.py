"""The 50 poly synth patches, as PresetDelta rows.

    /work/.venv/bin/python /work/tools/build_poly_patches.py          # print the C++ block
    /work/.venv/bin/python /work/tools/build_poly_patches.py --append # write into Keith.cpp

## Why this is a generator and not 50 hand-written blocks

Each patch is a set of parameter deltas, and there are 50 of them across 118 parameters. Written
by hand that is ~700 lines nobody can review. Generated, the *intent* of each patch is visible --
"wide Jupiter saw pad, slow filter" is a row of readable numbers rather than an assertion about
which parameter ids to touch.

## The eight reference characters, and what each contributes

    NEPTUNES / PHARRELL / CHAD HUGO   thin glassy keys, high cutoff, tight short stabs,
                                      unusual intervals (wide, not stacked thirds)
    DAFT PUNK                          filtered disco, chorus, medium-detune saw pairs
    TIMBALAND                          hard plucks, inharmonic/metallic FM, dry and close
    JUSTICE                            saturated saws, heavy drive, deliberately loud
    ROD TEMPERTON                      lush dense pads, warm brass, silky string beds
    J DILLA                            detuned and dusty, slight pitch drift, warm saturation

## The four hardware architectures

    JUNO      one osc + sub + chorus. Simple, warm, forgiving filter.
    JUPITER   two osc detuned, rich filter, big unison.
    PROPHET   two osc, resonant curtis-style filter, snappy envelopes.
    MOOG      ladder filter that thickens as it closes, plus saturation.

## Levels

No OutLevel values are emitted here. tools/measure_preset_levels.py measures each rendered patch
and writes the trims, which is how "none may sound cheap" is enforced rather than hoped for. A
hand-guessed level would fight that tool.
"""

import re
import sys
from pathlib import Path

ROOT = Path("/work/keith-plugin")
KEITH_CPP = ROOT / "src" / "Keith.cpp"

# --- parameter ids, read from the header so these cannot drift ---------------------------
_h = (ROOT / "src" / "Keith.h").read_text()
P = {name: int(idx) for name, idx in re.findall(r"(kParam\w+)\s*=\s*(\d+)", _h)}


def p(name):
    return P[name]


# --- patch construction -------------------------------------------------------------------

class Patch:
    """One patch: a name, a bank, a category, and its parameter deltas."""

    def __init__(self, name, category, **deltas):
        self.name = name
        self.category = category
        self.deltas = deltas

    def rows(self, preset_index):
        out = []
        for key, value in self.deltas.items():
            pid = P[key]
            v = value
            vs = f"{v:.0f}" if float(v).is_integer() else f"{v:g}"
            out.append((preset_index, pid, vs, f"kParam{key[3:]}"))
        return out


# Ranges, from the parameter table. Filters in Hz, envelopes in ms, levels 0..100.
def juno_poly(name, cat, cutoff, reso, attack, release, sat=15.0, **extra):
    """One oscillator plus a sub and a chorus. The Juno recipe: simple and warm."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=38.0, kParamOsc1Level=88.0,
        kParamOsc2Level=0.0,
        kParamSubLevel=45.0, kParamNoiseLevel=0.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=32.0,
        kParamFiltAttack=6.0, kParamFiltDecay=900.0, kParamFiltSustain=38.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1200.0, kParamAmpSustain=62.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=17.0,
        kParamStereoSpread=42.0,
        kParamTapeSat=sat, kParamTapeMix=28.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def jupiter_poly(name, cat, cutoff, reso, attack, release, detune=22.0, **extra):
    """Two oscillators detuned, big unison, rich filter."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=42.0, kParamOsc1Level=82.0,
        kParamOsc2Shape=40.0, kParamOsc2Pitch=0.0, kParamOsc2Level=76.0,
        kParamSubLevel=32.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=40.0,
        kParamFiltAttack=8.0, kParamFiltDecay=1100.0, kParamFiltSustain=34.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1400.0, kParamAmpSustain=66.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=4.0, kParamUnisonDetune=detune,
        kParamStereoSpread=58.0, kParamStereoDelay=0.0,
        kParamTapeSat=12.0, kParamTapeMix=24.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def prophet_poly(name, cat, cutoff, reso, attack, decay, sustain, release,
                 pulse=50.0, **extra):
    """Two oscillators, resonant filter, snappy envelopes."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=44.0, kParamOsc1Level=84.0,
        kParamOsc2Shape=46.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=64.0,
        kParamSubLevel=20.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=48.0,
        kParamFiltAttack=4.0, kParamFiltDecay=decay * 0.8, kParamFiltSustain=sustain * 0.6,
        kParamFiltRelease=release * 0.7,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamUnisonVoices=1.0, kParamUnisonDetune=10.0,
        kParamStereoSpread=30.0, kParamStereoDelay=18.0,
        kParamTapeSat=20.0, kParamTapeMix=32.0,
    )
    # Only when the patch actually wants a narrow pulse. Emitting the default would put a value
    # in the table that is out of the parameter's range for no reason.
    if pulse != 50.0:
        d["kParamOsc1PW"] = pulse
    d.update(extra)
    return Patch(name, cat, **d)


def moog_poly(name, cat, cutoff, reso, attack, decay, sustain, release, **extra):
    """Ladder filter that thickens as it closes, plus saturation."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=36.0, kParamOsc1Level=90.0,
        kParamOsc2Shape=34.0, kParamOsc2Pitch=-12.0, kParamOsc2Level=58.0,
        kParamSubLevel=38.0, kParamNoiseLevel=6.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterDrive=32.0, kParamFilterEnv=44.0,
        kParamFiltAttack=8.0, kParamFiltDecay=decay, kParamFiltSustain=sustain * 0.7,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=14.0,
        kParamStereoSpread=34.0,
        kParamTapeSat=30.0, kParamTapeMix=36.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def wavetable_poly(name, cat, cutoff, reso, attack, release, morph, frame=1.0, **extra):
    """Uses the third oscillator's wavetable bank, which is what reaches bell and voice tones."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=30.0, kParamOsc1Level=58.0,
        kParamOsc2Level=0.0,
        kParamOsc3Enable=1.0, kParamOsc3Source=1.0, kParamOsc3Morph=morph,
        kParamOsc3Level=86.0, kParamOsc3Pitch=0.0,
        kParamSubLevel=26.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=28.0,
        kParamFiltAttack=10.0, kParamFiltDecay=1400.0, kParamFiltSustain=44.0,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=1600.0, kParamAmpSustain=70.0,
        kParamAmpRelease=release,
        kParamUnisonVoices=2.0, kParamUnisonDetune=12.0,
        kParamStereoSpread=52.0,
        kParamTapeSat=18.0, kParamTapeMix=30.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def fm_poly(name, cat, cutoff, reso, attack, decay, sustain, release, fm, **extra):
    """FM for bells, tines and metallic tones, which subtractive shapes cannot reach."""
    d = dict(
        kParamEngine=0, kParamPolyMode=0,
        kParamOsc1Shape=20.0, kParamOsc1Level=72.0,
        kParamOsc2Shape=22.0, kParamOsc2Pitch=19.0, kParamOsc2Level=48.0,
        kParamFmAmount=fm,
        kParamSubLevel=18.0, kParamNoiseLevel=4.0,
        kParamFilterType=0, kParamFilterCutoff=cutoff, kParamFilterReso=reso,
        kParamFilterEnv=24.0,
        kParamFiltAttack=3.0, kParamFiltDecay=decay, kParamFiltSustain=sustain * 0.5,
        kParamFiltRelease=release,
        kParamAmpAttack=attack, kParamAmpDecay=decay, kParamAmpSustain=sustain,
        kParamAmpRelease=release,
        kParamVelToAmp=48.0, kParamVelToFilter=30.0,
        kParamStereoSpread=44.0, kParamStereoDelay=22.0,
        kParamTapeSat=14.0, kParamTapeMix=26.0,
    )
    d.update(extra)
    return Patch(name, cat, **d)


def build():
    """The 50, grouped by reference character."""
    patches = []

    # --- NEPTUNES / PHARRELL / CHAD HUGO  (10) ------------------------------------------
    # Thin and glassy, high cutoff, unusual intervals. The signature is space, not density.
    patches += [
        juno_poly("Glass Field", "poly keys", 11000, 10, 6, 1400, sat=12,
                  kParamOsc1Shape=24.0, kParamSubLevel=30.0,
                  kParamFilterKeyTrack=52.0, kParamStereoSpread=62.0),
        juno_poly("Thin Bell Keys", "poly keys", 9800, 14, 4, 900,
                  kParamOsc1Shape=18.0, kParamFmAmount=22.0, kParamVelToAmp=56.0),
        prophet_poly("Wide Interval Stab", "poly keys", 7400, 22, 3, 260, 8, 300, pulse=62,
                     kParamFilterEnv=58.0, kParamStereoDelay=30.0),
        juno_poly("Clav Ghost", "poly keys", 8600, 18, 2, 420, sat=22,
                  kParamOsc1Shape=48.0, kParamOsc1PW=64.0, kParamFilterEnv=52.0),
        wavetable_poly("Glass Morph Keys", "poly keys", 10200, 12, 10, 1800, morph=2.4,
                       kParamOsc3Pitch=12.0, kParamStereoSpread=68.0),
        fm_poly("Hollow Tine", "poly keys", 8200, 16, 2, 1100, 22, 1200, fm=34,
                kParamOsc2Pitch=24.0, kParamVelToAmp=62.0),
        juno_poly("Sparse Chime", "poly keys", 12500, 8, 8, 2200, sat=10,
                  kParamOsc1Shape=12.0, kParamSubLevel=22.0,
                  kParamIns1Algo=2.0, kParamIns1A=22.0, kParamIns1B=30.0,
                  kParamIns1C=48.0, kParamIns1Mix=24.0),
        prophet_poly("Tight Saw Stab", "poly keys", 6200, 30, 2, 220, 6, 240,
                     kParamFilterEnv=64.0, kParamOsc2Pitch=7.0, kParamFmAmount=14.0),
        juno_poly("Air Rhodes", "poly keys", 7000, 12, 6, 1600,
                  kParamFmAmount=28.0, kParamOsc1Shape=16.0, kParamVelToAmp=64.0),
        jupiter_poly("Off-Kilter Pad", "poly keys", 6600, 20, 320, 2400, detune=30,
                     kParamStereoSpread=74.0, kParamLfoRate=0.42, kParamLfoAmount=22.0,
                     kParamLfoDest=2.0),

    # --- DAFT PUNK  (8) ----------------------------------------------------------------
    # Filtered disco. Chorus on everything, medium detune, tidy bass underneath.
        jupiter_poly("Disc Chorus", "poly lead keys", 7800, 26, 6, 900, detune=26,
                     kParamIns1Algo=2.0, kParamIns1A=30.0, kParamIns1B=38.0,
                     kParamIns1C=64.0, kParamIns1Mix=34.0),
        jupiter_poly("Filtered Sweep", "poly lead keys", 3200, 44, 4, 700,
                     kParamFilterEnv=72.0, kParamFiltDecay=1500.0,
                     kParamLfoRate=0.9, kParamLfoDest=1.0, kParamLfoAmount=26.0),
        prophet_poly("French Touch Stab", "poly lead keys", 5400, 38, 2, 300, 10, 380,
                     kParamTapeSat=36.0, kParamFilterDrive=24.0, kParamFmAmount=10.0),
        juno_poly("Warm Chorus Keys", "poly lead keys", 8800, 16, 8, 1400, sat=18,
                  kParamOsc1Shape=40.0, kParamIns1Algo=2.0, kParamIns1C=72.0,
                  kParamIns1Mix=30.0),
        moog_poly("Driven Disco Bass", "poly lead keys", 1900, 40, 3, 500, 30, 400),
        jupiter_poly("Talkbox Lead Pad", "poly lead keys", 6400, 34, 12, 1200, detune=34,
                     kParamIns2Algo=2.0, kParamIns2C=70.0, kParamIns2Mix=28.0,
                     kParamStereoSpread=70.0),
        wavetable_poly("Robot Choir Keys", "poly lead keys", 7200, 18, 60, 1800, morph=4.6,
                       kParamUnisonVoices=4.0, kParamUnisonDetune=20.0),
        prophet_poly("Neon Clav", "poly lead keys", 9200, 24, 2, 340, 12, 420, pulse=55,
                     kParamFilterEnv=54.0, kParamTapeSat=28.0),

    # --- TIMBALAND  (7) ----------------------------------------------------------------
    # Hard plucks, inharmonic bells, dry and close. Space around the low end.
        fm_poly("Metal Pluck", "poly pluck", 9600, 20, 1, 380, 4, 340, fm=58,
                kParamOsc2Pitch=24.0, kParamVelToAmp=70.0, kParamStereoDelay=0.0),
        fm_poly("Exotic Bell", "poly pluck", 11400, 14, 2, 1800, 12, 2200, fm=72,
                kParamOsc2Pitch=24.0, kParamStereoDelay=34.0),
        juno_poly("Dry Clav Pluck", "poly pluck", 8400, 26, 1, 300,
                  kParamOsc1Shape=52.0, kParamOsc1PW=70.0, kParamFilterEnv=62.0),
        prophet_poly("Hard Stab", "poly pluck", 5800, 32, 1, 200, 4, 220,
                     kParamFilterEnv=70.0, kParamTapeMix=18.0, kParamStereoDelay=0.0),
        wavetable_poly("Gamelan Keys", "poly pluck", 9000, 16, 2, 1600, morph=5.8,
                       frame=6.0, kParamOsc3Pitch=19.0, kParamVelToAmp=64.0),
        fm_poly("Short Marimba", "poly pluck", 8000, 12, 1, 600, 0, 500, fm=44,
                kParamOsc2Pitch=24.0, kParamSubLevel=40.0),
        juno_poly("Plucked Glass", "poly pluck", 10400, 18, 1, 520,
                  kParamOsc1Shape=22.0, kParamFmAmount=18.0, kParamFilterEnv=56.0),

    # --- JUSTICE  (6) -------------------------------------------------------------------
    # Saturated saws, deliberately loud, gritty. Drive rather than finesse.
        jupiter_poly("Grit Saw Stack", "poly saw", 6800, 42, 3, 600, detune=40,
                     kParamFilterDrive=58.0, kParamTapeSat=44.0, kParamTapeMix=48.0),
        prophet_poly("Grit Comp Stab", "poly saw", 5200, 48, 1, 260, 8, 300,
                     kParamFilterDrive=62.0, kParamIns1Algo=3.0, kParamIns1A=30.0,
                     kParamIns1B=40.0, kParamIns1C=60.0, kParamIns1Mix=34.0),
        jupiter_poly("Overdriven Fifths", "poly saw", 4600, 52, 2, 500, detune=44,
                     kParamOsc2Pitch=7.0, kParamFilterDrive=54.0),
        moog_poly("Saturated Sub Stack", "poly saw", 2400, 46, 2, 700, 40, 500),
        prophet_poly("Hard Edge Lead Pad", "poly saw", 7400, 36, 4, 700, 20, 700,
                     kParamFilterDrive=50.0, kParamTapeSat=40.0),
        wavetable_poly("Folded Saw Morph", "poly saw", 6000, 40, 3, 800, morph=6.4,
                       kParamFilterDrive=56.0, kParamUnisonVoices=4.0),

    # --- ROD TEMPERTON  (7) -------------------------------------------------------------
    # Lush, dense, silky. Sweetening above the melody. Slow and wide.
        jupiter_poly("Silk String Bed", "poly pad", 6200, 14, 420, 2600, detune=24,
                     kParamUnisonVoices=4.0, kParamStereoSpread=84.0,
                     kParamFilterKeyTrack=40.0),
        jupiter_poly("Warm Brass Section", "poly pad", 4400, 24, 60, 1200, detune=18,
                     kParamFilterEnv=52.0, kParamFiltAttack=90.0,
                     kParamOsc1Shape=40.0, kParamOsc2Shape=38.0),
        wavetable_poly("Lush Voice Bed", "poly pad", 5600, 16, 520, 3200, morph=3.4,
                       kParamStereoSpread=88.0, kParamUnisonVoices=4.0),
        juno_poly("Sweetening Pad", "poly pad", 7000, 12, 380, 2800, sat=14,
                  kParamOsc1Shape=36.0, kParamIns1Algo=2.0, kParamIns1C=80.0,
                  kParamIns1Mix=38.0),
        jupiter_poly("Dense Choir Stack", "poly pad", 5200, 18, 460, 3000, detune=28,
                     kParamStereoSpread=90.0, kParamIns2Algo=2.0, kParamIns2C=76.0,
                     kParamIns2Mix=32.0),
        moog_poly("Rich Low Brass", "poly pad", 3400, 22, 80, 1400, 58, 900),
        prophet_poly("Glide Strings", "poly pad", 5800, 16, 400, 2200, 62, 2400,
                     kParamPorta=40.0, kParamMonoLegato=1.0, kParamStereoSpread=76.0),

    # --- J DILLA  (7) -------------------------------------------------------------------
    # Detuned and dusty. Drift, not pristine. Warmth from saturation, not bitcrushing.
        juno_poly("Dusty Keys", "poly warm keys", 6400, 20, 12, 1800, sat=34,
                  kParamTapeWow=42.0, kParamTapeFlutter=30.0, kParamTapeHiss=12.0,
                  kParamTapeMix=44.0),
        jupiter_poly("Drifting Pad", "poly warm keys", 5200, 24, 340, 2800, detune=46,
                     kParamTapeWow=48.0, kParamTapeFlutter=34.0, kParamStereoSpread=80.0),
        prophet_poly("Warm Detune Stab", "poly warm keys", 5600, 30, 4, 400, 14, 500, pulse=58,
                     kParamTapeWow=38.0, kParamTapeMix=40.0),
        juno_poly("Tape Rhodes", "poly warm keys", 6800, 14, 8, 2000, sat=38,
                  kParamFmAmount=30.0, kParamTapeFlutter=36.0, kParamTapeMix=46.0),
        wavetable_poly("Warped Choir Keys", "poly warm keys", 6000, 22, 220, 2600, morph=2.0,
                       kParamTapeWow=44.0, kParamUnisonDetune=28.0),
        moog_poly("Round Low Keys", "poly warm keys", 2600, 28, 10, 1200, 48, 1000),
        fm_poly("Soft Tine Drift", "poly warm keys", 7200, 16, 6, 1500, 30, 1600, fm=26,
                kParamTapeWow=40.0, kParamTapeHiss=10.0, kParamTapeMix=42.0),

    # --- CROSSOVER / ARCHITECTURE SHOWCASES  (5) ----------------------------------------
    # One patch per hardware architecture, built to show the engine's range rather than a
    # single reference. These are the ones a player reaches for first.
        jupiter_poly("Wide Unison Stack", "poly synth", 7200, 28, 20, 1400, detune=32,
                     kParamUnisonVoices=6.0, kParamStereoSpread=88.0,
                     kParamIns1Algo=2.0, kParamIns1C=66.0, kParamIns1Mix=32.0),
        prophet_poly("Resonant Poly Brass", "poly synth", 4800, 26, 40, 900, 54, 800,
                     kParamOsc2Pitch=0.0, kParamFilterEnv=56.0, kParamFiltAttack=50.0),
        juno_poly("Simple Chorus Pad", "poly synth", 6000, 10, 300, 2400, sat=16,
                  kParamUnisonVoices=1.0, kParamIns1Algo=2.0, kParamIns1C=74.0,
                  kParamIns1Mix=36.0),
        moog_poly("Ladder Filter Stack", "poly synth", 3000, 38, 12, 1000, 52, 900),
        wavetable_poly("Table Sweep Pad", "poly synth", 7600, 30, 140, 2200, morph=0.5,
                       kParamLfoRate=0.28, kParamLfoDest=16.0, kParamLfoAmount=64.0,
                       kParamStereoSpread=82.0),
    ]

    assert len(patches) == 50, f"expected 50 poly patches, built {len(patches)}"
    return patches


def format_block(patches, first_preset_index):
    """The C++ PresetSpec entries and the PresetDelta rows, ready to paste."""
    specs, deltas = [], []
    for i, patch in enumerate(patches):
        idx = first_preset_index + i
        specs.append(f'  {{"{patch.name}", 6, "mags", "{patch.category}"}},')
        deltas.append(f"  // Poly {i + 1:02d} / {patch.name}")
        rows = [f"{{{idx}, {pid}, {val}}}" for idx, pid, val, _ in patch.rows(idx)]
        # three per line, so it stays readable
        for k in range(0, len(rows), 3):
            deltas.append("  " + ", ".join(rows[k:k + 3]) + ",")
        deltas.append("")
    return "\n".join(specs), "\n".join(deltas)


def main():
    patches = build()
    specs, deltas = format_block(patches, 109)
    print(f"// {len(patches)} poly synth patches")
    print(f"// categories used: {sorted({p.category for p in patches})}")
    print()
    print("// ---- PresetSpec entries ----")
    print(specs)
    print()
    print("// ---- PresetDelta rows ----")
    print(deltas[:2000] + ("\n... (truncated)" if len(deltas) > 2000 else ""))

    if "--append" in sys.argv:
        # Deliberately NOT auto-editing Keith.cpp yet: the level trims have to be measured
        # after these exist, and writing them in before measuring would put unlevelled
        # patches in a build. This flag is a placeholder for that step.
        print()
        print("--append is not wired yet: the level trims must be measured first.")
        print("See tools/measure_preset_levels.py.")


if __name__ == "__main__":
    main()
