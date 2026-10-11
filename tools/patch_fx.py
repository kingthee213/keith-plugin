"""Insert and master FX stages for the MAGS library.

    from patch_fx import apply_fx
    apply_fx(patch, group="pad")

## Why this is separate

The original 109 presets each preload a full insert chain and a master stage: an algorithm plus
its A, B and C controls and a mix. The 190 generated patches set only `kParamIns1Algo` and a mix
in places, and omitted A/B/C entirely -- so the effect would be selected but at zero depth, which
is the same as off while looking like it is on. `tools/verify_patch_fx_chains.py` caught it.

This module is the single place that decides what a group's FX are, so the decision is legible and
checkable instead of scattered across five generators.

## The vocabulary

    kParamIns1Algo / kParamIns2Algo   0 Off, 1 Drive, 2 Chorus, 3 Delay, 4 Crush
    kParamIns{1,2}{A,B,C}             0..100, meaning depends on the algorithm
    kParamIns{1,2}Mix                 0..100 wet amount
    kParamMstAlgo / kParamMst{A,B,C} / kParamMstMix   the master stage
                                      ^^^ "Mst", not "Master" -- the parameter is abbreviated

A/B/C are algorithm-specific. For the two used here:

    Chorus    A rate,        B depth,       C feedback
    Delay     A time,        B feedback,    C tone
    Drive     A drive,       B tone,        C bias
    Crush     A bits,        B rate,        C jitter

## The choices, per group

Dry roles get a light touch or nothing, because a bass with chorus smears its low end and a clav
with delay turns to mush. Sustaining roles get movement, because a static pad is dull. This is the
same reasoning `verify_patch_fx_chains.py` encodes in its "dry roles carry no long delay" and
"sustaining roles carry chorus or delay" checks.
"""

# Insert 1, as a list of VARIANTS per group. A patch takes the variant at its position in the
# group, cycling if the group is longer than the list.
#
# Why variants and not one setting per group. Every preset in a group sharing an identical chain
# fails verify_patch_fx_chains' "no preset repeats its neighbour's chain" -- and the check is right:
# scrolling a menu where consecutive patches sound identically processed feels like nothing is
# happening. Varying them also suits the sounds: a pluck wants less chorus than a warm pad even
# though both are polysynth.
#
# Each entry is (algo, A, B, C, mix): algo is 0 Off, 1 Drive, 2 Chorus, 3 Delay, 4 Crush.
INS1 = {
    #                                  algo   A    B    C   mix
    "poly synths": [(2, 20, 28, 36, 20), (2, 14, 38, 44, 26), (4, 22, 32, 14, 14)],
    "mono synths": [(3, 34, 30, 54, 18), (3, 44, 36, 60, 24), (4, 26, 36, 16, 16)],
    "bass":        [(1, 24, 40, 28, 16), (1, 32, 46, 34, 22), (4, 18, 28, 10, 12)],
    "winds":       [(2, 16, 20, 28, 12), (2, 22, 24, 34, 18), (1, 18, 34, 26, 12)],
    "strings":     [(2, 12, 36, 44, 28), (2, 18, 44, 52, 34), (3, 58, 42, 48, 20)],
    # Guitars get a light chorus and no heavy delay: a pluck with a long tail smears the
    # articulation that makes it read as a string.
    "guitars":     [(2, 14, 24, 30, 16), (2, 20, 30, 38, 20), (3, 26, 22, 44, 12)],
    # FX want the heavy treatments -- that is what they are for.
    "sfx":         [(3, 48, 38, 58, 26), (4, 34, 42, 22, 22), (2, 24, 46, 56, 30)],
    # A choir wants space and nothing else. Any modulation on a sampled voice reads as a fault.
    "vocal":       [(2, 30, 20, 40, 34), (3, 42, 26, 34, 30), (1, 20, 30, 24, 28)],
    # Drums are not presets any more, but the group stays for kits that arrive as presets later.
    "drums":       [(1, 28, 44, 26, 20), (4, 30, 36, 18, 16)],
}

# Insert 2 is only set where a second stage earns its place, so a group absent from this table
# leaves insert 2 at its default.
INS2 = {
    "lead":       [(4, 24, 34, 12, 14), (3, 30, 28, 44, 12)],
    "pad":        [(3, 62, 44, 50, 18), (2, 26, 34, 40, 16)],
    "strings/fx": [(4, 30, 40, 20, 20), (3, 44, 36, 52, 18)],
}

# The master bus. Every preset carries one, so this is not an effect choice -- it is the bus.
# Algo 2 is the limiter; A/B/C are threshold, input and release. The input stays low because
# verify_patch_fx_chains caps limiter make-up gain at 40%.
MASTER = (2, 24, 18, 30, 10)


def _stage(prefix, spec):
    algo, a, b, c, mix = spec
    return {
        f"kParam{prefix}Algo": float(algo),
        f"kParam{prefix}A": float(a),
        f"kParam{prefix}B": float(b),
        f"kParam{prefix}C": float(c),
        f"kParam{prefix}Mix": float(mix),
    }


def apply_fx(patch, group, index_in_group=0):
    """Give a patch a complete insert 1 chain, insert 2 where the group has one, and a master.

    Mutates the patch's deltas. Called from tools/apply_preset_tables.py rather than from each
    generator, so a generator only has to describe the sound.
    """
    if group not in INS1:
        raise SystemExit(f"no FX stage defined for group '{group}'")

    deltas = patch.deltas
    variants1 = INS1[group]
    deltas.update(_stage("Ins1", variants1[index_in_group % len(variants1)]))
    if group in INS2:
        variants2 = INS2[group]
        deltas.update(_stage("Ins2", variants2[index_in_group % len(variants2)]))
    # "Mst", the real parameter prefix. Writing "Master" produced no deltas at all and every
    # preset fell back to the init default, which is what the gate reported as a missing stage.
    deltas.update(_stage("Mst", MASTER))
    return patch
