"""Prove the surface guard can still fail.

Removes one control attach, runs the guard, restores the file, and reports whether the guard
noticed. A guard that cannot fail is not a guard, and this one already went blind once when the
layout switched from named attaches to numeric range loops.
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = ROOT / "src" / "editor" / "EditorialLayout.h"
ATTACH = "  freeKnob(kParamVelToAmp, ControlFor(kParamVelToAmp).label, kGreen, true);\n"
GUARD = ROOT / "tools" / "editor_surface_check.py"


def run_guard():
    out = subprocess.run([sys.executable, str(GUARD)], capture_output=True, text=True).stdout
    line = [l for l in out.splitlines() if "reachable from the editor source" in l]
    return line[0] if line else "(guard line not found)"


original = LAYOUT.read_text()
if ATTACH not in original:
    raise SystemExit("the velocity-to-amp attach is not in the layout, nothing to remove")

print("with the attach in place :", run_guard())

LAYOUT.write_text(original.replace(ATTACH, "", 1))
try:
    print("with the attach removed :", run_guard())
finally:
    LAYOUT.write_text(original)

restored = LAYOUT.read_text() == original
print("layout restored         :", restored)
if not restored:
    raise SystemExit("failed to restore the layout")
