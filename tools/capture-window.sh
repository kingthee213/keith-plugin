#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

rm -f /tmp/win-w7b.png design/screenshots/w7b-vs-reference.png

pkill -f MagsSynth
sleep 2
open build/out/Release/MagsSynth.app
sleep 8

osascript -e 'tell application "MagsSynth" to activate' \
         -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true'
sleep 3

BOUNDS=$(osascript -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1')
echo "window bounds: $BOUNDS"
FRONT=$(osascript -e 'tell application "System Events" to get name of first application process whose frontmost is true')
echo "frontmost app: $FRONT"

X=$(echo "$BOUNDS" | cut -d, -f1 | tr -d ' ')
Y=$(echo "$BOUNDS" | cut -d, -f2 | tr -d ' ')
W=$(echo "$BOUNDS" | cut -d, -f3 | tr -d ' ')
H=$(echo "$BOUNDS" | cut -d, -f4 | tr -d ' ')
screencapture -x -R"$X,$Y,$W,$H" /tmp/win-w7b.png
~/.venvs/keith-measure/bin/python -c "
from PIL import Image
im = Image.open('/tmp/win-w7b.png')
print('capture', im.size)
"
