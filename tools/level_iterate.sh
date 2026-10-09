#!/usr/bin/env bash
# Drive the preset level loop to convergence.
#
# The chain is not linear: the synth bus feeds the insert chain, the tape bus and the master
# block, so a gain change there does not produce a matching measured change. A first pass on
# the output stage lands most presets, and a second pass corrects whatever the peak ceiling
# pushed back. Each pass measures the state the previous pass built, so the loop corrects a
# real measurement instead of trusting an arithmetic prediction.
#
# The script resets the generated block first, so it is safe to re-run after any preset edit.
#
# Usage: bash tools/level_iterate.sh [passes]     (default 2)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

PY="${KEITH_MEASURE_PYTHON:-$HOME/.venvs/keith-measure/bin/python}"
export PATH="$HOME/cmakebin:$PATH"
PASSES="${1:-2}"
RENDER="build/Release/keith-recreation-render"

if [ ! -x "$PY" ]; then
  echo "measurement python not found at $PY" >&2
  exit 1
fi

echo "== resetting to the hand-written preset levels"
"$PY" tools/measure_preset_levels.py strip

for pass in $(seq 1 "$PASSES"); do
  echo "== pass $pass: building the renderer"
  cmake --build build --config Release --target keith-recreation-render >/dev/null
  echo "== pass $pass: measuring the current state"
  "$PY" tools/measure_preset_levels.py measure --render "$RENDER" \
      --out "build/level-pass${pass}-measured.tsv"
  echo "== pass $pass: writing and applying the correction"
  "$PY" tools/measure_preset_levels.py emit --tsv "build/level-pass${pass}-measured.tsv" \
      --out "build/level-pass${pass}-trims.txt"
  "$PY" tools/measure_preset_levels.py apply --trims "build/level-pass${pass}-trims.txt"
done

echo "== final build and measurement"
cmake --build build --config Release --target keith-recreation-render >/dev/null
"$PY" tools/measure_preset_levels.py measure --render "$RENDER" \
    --out docs/level-measurements-after.tsv
cp "build/level-pass1-measured.tsv" docs/level-measurements-before.tsv
echo "== done: before is docs/level-measurements-before.tsv, after is docs/level-measurements-after.tsv"
