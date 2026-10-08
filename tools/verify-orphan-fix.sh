#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== surface check (was 15 passed, 2 failed) ==="
python3 tools/editor_surface_check.py 2>&1 | tail -3

echo "=== guard self test ==="
python3 tools/guard_self_test.py

echo "=== geometry gate ==="
cmake --build build --config Release --target keith-editor-tests MagsSynth-app 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -3
./build/Release/keith-editor-tests > /tmp/g12.txt 2>&1
echo "exit=$? PASS=$(grep -c '^PASS' /tmp/g12.txt) FAIL=$(grep -c '^FAIL' /tmp/g12.txt)"
grep '^FAIL' /tmp/g12.txt | head -4

echo "=== panel occupancy ==="
python3 tools/fit_budget.py 2>&1 | tail -2

echo "=== capture ==="
pkill -f MagsSynth
sleep 1
open build/out/Release/MagsSynth.app
sleep 10
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' \
         -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
screencapture -x -R200,93,1280,832 /tmp/win11.png
ls -la /tmp/win11.png
