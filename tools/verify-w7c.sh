#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== carve-out gone? ==="
grep -c "compactStrip" tools/editor_geometry_test.cpp || echo "0 (gone)"

echo "=== design size / window ==="
grep -nE "PLUG_WIDTH|PLUG_HEIGHT|PLUG_MIN|PLUG_MAX" config.h
grep -nE "kCanvasW|kCanvasH|kKnobMinHeight|kKnobBodyMin|kKnobLabelHeight|kKnobValueHeight" src/editor/EditorGeometry.h | head -6

echo "=== generator + face ==="
python3 tools/generate_editor_geometry.py --write 2>&1 | grep -vE "^//" | tail -4
~/.venvs/keith-measure/bin/python tools/compose_face.py

echo "=== gates ==="
cmake --build build --config Release --target keith-editor-tests MagsSynth-app 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -3
./build/Release/keith-editor-tests > /tmp/g17.txt 2>&1
echo "geometry exit=$? PASS=$(grep -c '^PASS' /tmp/g17.txt) FAIL=$(grep -c '^FAIL' /tmp/g17.txt)"
grep '^FAIL' /tmp/g17.txt | head -3
python3 tools/editor_surface_check.py 2>&1 | tail -1
python3 tools/fit_budget.py 2>&1 | tail -2

echo "=== smallest control height vs the rule ==="
python3 -c "
import re
t = open('src/editor/EditorGeometry.h').read()
m = re.search(r'inline constexpr ControlRect kControlRects\[\] = \{(.*?)\n\};', t, re.S)
rows = re.findall(r'\{\s*(\d+),\s*\{([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f,\s*([\d.]+)f\},\s*\"([^\"]*)\"', m.group(1))
hs = sorted((float(r[4]) - float(r[2]), int(r[0]), r[5]) for r in rows)
print('total controls:', len(hs))
for h, p, lab in hs[:6]:
    print(f'  shortest: param {p} {lab} {h:.1f}px')
"

echo "=== window ==="
pkill -f MagsSynth; sleep 2
open build/out/Release/MagsSynth.app; sleep 8
osascript -e 'tell application "MagsSynth" to activate' \
         -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true'
sleep 3
osascript -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
