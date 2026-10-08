#!/usr/bin/env bash
# Create the python environment the verification gates run in.
#
# The macOS system python ships no third-party packages, and PEP 668 blocks installing into
# it. Three tools need more than the standard library:
#
#   tools/measure_preset_levels.py   numpy, scipy   preset loudness and peak measurement
#   tools/recreation_gate.py         numpy         the recreation comparison gate
#   tools/render_faces.py            pillow        the painted editor face
#
# CMake points the recreation gate at this interpreter, and the other two are run from it by
# hand. Re-running this script is safe; pip skips what is already installed.
set -euo pipefail

VENV="${KEITH_VENV:-$HOME/.venvs/keith-measure}"

python3 -m venv "$VENV"
"$VENV/bin/pip" install --quiet --upgrade pip
"$VENV/bin/pip" install --quiet numpy scipy pillow

"$VENV/bin/python" - <<'CHECK'
import numpy, scipy, PIL
print(f"numpy {numpy.__version__}  scipy {scipy.__version__}  pillow {PIL.__version__}")
CHECK

echo "tools venv ready at $VENV"
echo "  $VENV/bin/python tools/measure_preset_levels.py measure --render build/Release/keith-recreation-render --out docs/level-measurements-after.tsv"
echo "  $VENV/bin/python tools/render_faces.py"
