"""Measure the loudness of every baked factory preset and emit a level trim per preset.

The problem: only some presets set Synth Master Level, so switching patches jumps in
volume. This measures the actual rendered level of each preset and proposes the trim
that lands each one on the target.

How it measures. Each preset renders three sustained notes through the plugin itself
(keith-recreation-render, the same path the recreation gate uses) at C2, C4 and C5.
For each render it computes

  - sample peak, in dBFS
  - the loudest 100 ms K-weighted window, in LUFS (ITU-R BS.1770-4 weighting)

and takes the loudest of the three notes as that preset's level. Two deliberate choices:

  * A 100 ms window, not the standard 400 ms. BS.1770's 400 ms window is for programme
    loudness. A single held note is not programme material, and a 400 ms window averages
    a short percussive hit away: a kick that peaks at -9 dBFS reads 22 dB quieter through
    a 400 ms window than through a 100 ms one. Shortening the window is what lets a
    percussive preset and a pad land on the same number.
  * Peak as a ceiling rather than a target. Each trim is the smaller of what loudness
    wants and what peak allows, so no trim can push a preset into clipping.

Targets, and why. -12 LUFS momentary with sample peak under -1 dBFS. The target is a
tolerance, not an exact value: a pad and a pluck are not equally loud at the same
number, so the check is that no preset sits more than TOLERANCE_DB from the target.

Usage:
  python3 tools/measure_preset_levels.py measure --render build/Release/keith-recreation-render \\
      --out docs/level-measurements.tsv
  python3 tools/measure_preset_levels.py emit --tsv docs/level-measurements.tsv \\
      --out build/level-trims.txt
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

import numpy as np
from scipy import signal

ROOT = Path(__file__).resolve().parent.parent
RATE = 48000.0
PROBE_SECONDS = 3.0
PROBE_NOTES = (36, 60, 72)
TARGET_LUFS = -12.0
TOLERANCE_DB = 3.0
PEAK_CEILING_DBFS = -1.5
BLOCK_S = 0.100
HOP_S = 0.025

# ITU-R BS.1770-4 K-weighting, 48 kHz.
SHELF_B = np.array([1.53512485958697, -2.69169618940638, 1.19839281085285])
SHELF_A = np.array([1.0, -1.69065929318241, 0.73248077421585])
HPF_B = np.array([1.0, -2.0, 1.0])
HPF_A = np.array([1.0, -1.99004745483398, 0.99007225036621])


def biquad(x, b, a):
    return signal.lfilter(b, a, x)


def read_f32(path):
    raw = np.fromfile(path, dtype=np.uint8)
    header = raw[:8].view(np.int32)
    rate, frames = int(header[0]), int(header[1])
    audio = raw[8:8 + frames * 2 * 4].view(np.float32).reshape(-1, 2)
    if rate != int(RATE):
        raise SystemExit(f"{path}: expected {RATE:.0f} Hz, got {rate}")
    return audio.astype(np.float64)


def momentary_max_lufs(audio):
    left = biquad(biquad(audio[:, 0], SHELF_B, SHELF_A), HPF_B, HPF_A)
    right = biquad(biquad(audio[:, 1], SHELF_B, SHELF_A), HPF_B, HPF_A)
    block = int(BLOCK_S * RATE)
    hop = int(HOP_S * RATE)
    if audio.shape[0] < block:
        block = audio.shape[0]
        hop = max(1, block // 4)
    best = -np.inf
    for start in range(0, audio.shape[0] - block + 1, hop):
        window = slice(start, start + block)
        power = (np.mean(left[window] ** 2) + np.mean(right[window] ** 2)) / 2.0
        if power <= 0.0:
            continue
        best = max(best, -0.691 + 10.0 * np.log10(power))
    return best


def preset_names():
    text = (ROOT / "src" / "Keith.cpp").read_text(errors="replace")
    block = text.split("kPresetSpecs[kNumPresets] = {", 1)[1].split("\n};", 1)[0]
    return re.findall(r'\{\s*"([^"]+)"', block)


def measure_preset(render, name, workdir):
    best_lufs = -np.inf
    best_peak = -np.inf
    for note in PROBE_NOTES:
        out = workdir / "probe.f32"
        result = subprocess.run([str(render), name, str(note), str(PROBE_SECONDS), str(out)],
                                capture_output=True, text=True)
        if result.returncode != 0:
            raise SystemExit(f"render failed for {name} note {note}: {result.stderr.strip()}")
        audio = read_f32(out)
        peak = 20.0 * np.log10(max(float(np.max(np.abs(audio))), 1e-9))
        lufs = momentary_max_lufs(audio)
        best_lufs = max(best_lufs, lufs)
        best_peak = max(best_peak, peak)
    return best_lufs, best_peak


def cmd_measure(args):
    names = preset_names()
    render = Path(args.render).resolve()
    if not render.exists():
        sys.exit(f"render binary not found: {render}")

    rows = []
    with tempfile.TemporaryDirectory() as tmp:
        workdir = Path(tmp)
        for index, name in enumerate(names):
            lufs, peak = measure_preset(render, name, workdir)
            trim = TARGET_LUFS - lufs
            rows.append((index, name, lufs, peak, trim))
            print(f"{index:3d} {name:34s} {lufs:7.2f} LUFS  peak {peak:6.2f} dBFS  "
                  f"trim {trim:+6.2f} dB")

    spread = max(r[2] for r in rows) - min(r[2] for r in rows)
    print(f"\nlevel spread before trim: {spread:.2f} dB across {len(rows)} presets")
    print(f"target {TARGET_LUFS:+.1f} LUFS momentary, tolerance +/-{TOLERANCE_DB:.1f} dB, "
          f"peak ceiling {PEAK_CEILING_DBFS:+.1f} dBFS")
    print(f"presets outside tolerance: "
          f"{sum(1 for r in rows if abs(r[4]) > TOLERANCE_DB)}")
    print(f"presets peaking above ceiling: "
          f"{sum(1 for r in rows if r[3] > PEAK_CEILING_DBFS + 0.05)}")

    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("w") as handle:
        handle.write("index\tpreset\tlufs_momentary\tpeak_dbfs\ttrim_db\n")
        for index, name, lufs, peak, trim in rows:
            handle.write(f"{index}\t{name}\t{lufs:.3f}\t{peak:.3f}\t{trim:.3f}\n")
    print(f"wrote {out}")
    return 0


OUT_SYMBOL = "kParamOutLevel"
MASTER_SYMBOL = "kParamSynthMasterLevel"
# FX Out Level sits after the insert chain, the tape bus and the master block, so a change
# there is a pure level change. Synth Master Level sits on the synth bus, before all of
# that, so a change there also changes how hard the tape and master stages are driven. The
# first trim pass used the synth bus and did not land: the chain is not linear, and a
# measured error came back as a different measured error. So the trim goes on the output
# stage and the synth bus only carries whatever overflow the output range cannot hold.
RANGES = {OUT_SYMBOL: (-60.0, 12.0), MASTER_SYMBOL: (-60.0, 18.0)}

DELTA = re.compile(r"\{\s*(\d+)\s*,\s*(kParam\w+)\s*,\s*([+-]?[\d.]+)\s*\}")


def current_levels():
    """Level each preset already sets, per stage, so a correction can be absolute."""
    text = (ROOT / "src" / "Keith.cpp").read_text(errors="replace")
    block = text.split("kPresetDeltas[] = {", 1)[1].split("\n};", 1)[0]
    levels = {OUT_SYMBOL: {}, MASTER_SYMBOL: {}}
    for match in DELTA.finditer(block):
        symbol = match.group(2)
        if symbol in levels:
            levels[symbol][int(match.group(1))] = float(match.group(3))
    return levels


def cmd_emit(args):
    tsv = Path(args.tsv)
    rows = []
    for line in tsv.read_text().splitlines()[1:]:
        index, name, lufs, peak, trim = line.split("\t")
        rows.append((int(index), name, float(lufs), float(peak), float(trim)))

    base = current_levels()

    lines = [BEGIN_MARK + " (tools/measure_preset_levels.py)",
             f"  // Target {TARGET_LUFS:.0f} LUFS momentary loudness, sample peak at or below "
             f"{PEAK_CEILING_DBFS:.1f} dBFS.",
             "  // These sit last in kPresetDeltas on purpose: a later delta wins, so these",
             "  // override the hand-written level values above.",
             "  // Values are absolute, not offsets, because a preset delta replaces the value."]
    peak_limited = []
    clamped = []
    predicted = []
    for index, name, lufs, peak, _ in rows:
        trim = min(TARGET_LUFS - lufs, PEAK_CEILING_DBFS - peak)
        out_start = base[OUT_SYMBOL].get(index, 0.0)
        master_start = base[MASTER_SYMBOL].get(index, 0.0)

        out_value = min(max(out_start + trim, RANGES[OUT_SYMBOL][0]), RANGES[OUT_SYMBOL][1])
        applied_out = out_value - out_start
        master_value = min(max(master_start + (trim - applied_out), RANGES[MASTER_SYMBOL][0]),
                           RANGES[MASTER_SYMBOL][1])
        applied = applied_out + (master_value - master_start)

        if trim < (TARGET_LUFS - lufs) - 0.01:
            peak_limited.append((name, peak))
        if abs(applied - trim) > 0.01:
            clamped.append((name, trim, applied))
        predicted.append((index, name, lufs + applied, peak + applied))
        lines.append(f"  {{{index}, {OUT_SYMBOL}, {out_value:+.2f}}},"
                     f" {{{index}, {MASTER_SYMBOL}, {master_value:+.2f}}},"
                     f"  // {name}  measured {lufs:.2f} LUFS, applied {applied:+.2f} dB")
    lines.append(END_MARK)

    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines) + "\n")

    loudness = [row[2] for row in predicted]
    spread = max(loudness) - min(loudness)
    outside = [row for row in predicted if abs(row[2] - TARGET_LUFS) > TOLERANCE_DB]
    hot = [row for row in predicted if row[3] > PEAK_CEILING_DBFS + 0.05]
    print(f"wrote {out} ({len(rows)} trims)")
    print(f"predicted spread after trim: {spread:.2f} dB")
    print(f"predicted outside +/-{TOLERANCE_DB:.0f} dB of target: {len(outside)}")
    print(f"predicted still above the peak ceiling: {len(hot)}")
    print(f"limited by peak rather than by loudness: {len(peak_limited)}")
    for name, peak in peak_limited:
        print(f"  peak {peak:+6.2f} dBFS  {name}")
    if clamped:
        print(f"clamped at the parameter range: {len(clamped)}")
        for name, wanted, got in clamped:
            print(f"  wanted {wanted:+6.2f} -> {got:+6.2f}  {name}")
    if outside:
        print("cannot reach the target inside the parameter range:")
        for index, name, lufs, peak in sorted(outside, key=lambda r: r[2]):
            print(f"  {lufs:7.2f} LUFS  {name}")
    return 0


BEGIN_MARK = "  // --- BEGIN measured level trims"
END_MARK = "  // --- END measured level trims"


def cmd_apply(args):
    """Write the trims into src/Keith.cpp, replacing any block a previous run left."""
    trims = Path(args.trims).read_text().rstrip("\n")
    if not trims.startswith(BEGIN_MARK):
        sys.exit(f"{args.trims} does not start with the generated block marker")

    source_path = ROOT / "src" / "Keith.cpp"
    source = source_path.read_text()
    if BEGIN_MARK in source:
        start = source.index(BEGIN_MARK)
        end = source.index(END_MARK) + len(END_MARK)
        source = source[:start] + trims + source[end:]
        action = "replaced"
    else:
        anchor = source.index("\n};\n\nconst keith::PresetSpec kPresetSpecs")
        source = source[:anchor] + "\n\n" + trims + source[anchor:]
        action = "inserted"

    source_path.write_text(source)
    count = trims.count("kParamSynthMasterLevel")
    print(f"{action} the measured level block in src/Keith.cpp ({count} presets)")
    return 0


def cmd_strip(args):
    """Remove the generated block so a run starts from the hand-written preset values."""
    source_path = ROOT / "src" / "Keith.cpp"
    source = source_path.read_text()
    if BEGIN_MARK not in source:
        print("no measured level block to strip")
        return 0
    start = source.index(BEGIN_MARK)
    end = source.index(END_MARK) + len(END_MARK)
    # Take the newline that followed the block with it, so the array keeps its shape.
    while end < len(source) and source[end] in "\r\n":
        end += 1
    source_path.write_text(source[:start] + source[end:])
    print("stripped the measured level block from src/Keith.cpp")
    return 0


def main():
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="command", required=True)
    measure = sub.add_parser("measure")
    measure.add_argument("--render", required=True)
    measure.add_argument("--out", required=True)
    measure.set_defaults(func=cmd_measure)
    emit = sub.add_parser("emit")
    emit.add_argument("--tsv", required=True)
    emit.add_argument("--out", required=True)
    emit.set_defaults(func=cmd_emit)
    apply_trims = sub.add_parser("apply")
    apply_trims.add_argument("--trims", required=True)
    apply_trims.set_defaults(func=cmd_apply)
    strip = sub.add_parser("strip")
    strip.set_defaults(func=cmd_strip)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
