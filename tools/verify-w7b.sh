#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== generator writes cleanly? ==="
python3 tools/generate_editor_geometry.py --write 2>&1 | grep -vE "^//" | head -8
echo "exit=$?"
echo "dirty after regenerate: $(git status --short | wc -l | tr -d ' ')"

echo "=== face ==="
~/.venvs/keith-measure/bin/python tools/compose_face.py

echo "=== gates ==="
cmake --build build --config Release --target keith-editor-tests MagsSynth-app 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -3
./build/Release/keith-editor-tests > /tmp/g16.txt 2>&1
echo "geometry exit=$? PASS=$(grep -c '^PASS' /tmp/g16.txt) FAIL=$(grep -c '^FAIL' /tmp/g16.txt)"
grep '^FAIL' /tmp/g16.txt | head -4
python3 tools/editor_surface_check.py 2>&1 | tail -1
python3 tools/fit_budget.py 2>&1 | tail -2

pkill -f MagsSynth
sleep 1
open build/out/Release/MagsSynth.app
sleep 10
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' \
         -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
screencapture -x -R200,80,1536,1024 /tmp/win-w7b.png
ls -la /tmp/win-w7b.png
~/.venvs/keith-measure/bin/python -c "
from PIL import Image
ref = Image.open('/Users/solace/TYH_M1/dev/mags-modulator/inbox/mags-synth-final-ui-reference.png').convert('RGB')
shot = Image.open('/tmp/win-w7b.png').convert('RGB').resize((1536,1024), Image.LANCZOS)
sheet = Image.new('RGB', (1536, 2070), (30,30,30))
sheet.paste(ref, (0,0)); sheet.paste(shot, (0,1046))
sheet.save('design/screenshots/w7b-vs-reference.png')
print('side by side written')
"
