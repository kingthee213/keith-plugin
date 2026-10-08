#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

cmake --build build --config Release --target keith-editor-tests MagsSynth-app 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -4

./build/Release/keith-editor-tests > /tmp/g8.txt 2>&1
echo "gate exit=$?"
echo "PASS=$(grep -c '^PASS' /tmp/g8.txt) FAIL=$(grep -c '^FAIL' /tmp/g8.txt)"
grep '^FAIL' /tmp/g8.txt | head -4

pkill -f MagsSynth
sleep 1
open build/out/Release/MagsSynth.app
sleep 10
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true' \
         -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1'
screencapture -x -R200,93,1280,832 /tmp/win8.png
ls -la /tmp/win8.png
