#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

rm -f design/screenshots/w7d-vs-reference.png /tmp/win-w7b.png /tmp/face-w7d-1x.png

osascript -e 'tell application "MagsSynth" to activate'
sleep 4
osascript -e 'tell application "System Events" to tell process "MagsSynth" to set frontmost to true'
sleep 4

BOUNDS=$(osascript -e 'tell application "System Events" to tell process "MagsSynth" to get {position, size} of window 1')
FRONT=$(osascript -e 'tell application "System Events" to get name of first application process whose frontmost is true')
echo "bounds: $BOUNDS"
echo "frontmost: $FRONT"

if [ "$FRONT" != "MagsSynth" ]; then
  echo "NOT FRONT, not capturing into a pile of other windows"
  exit 1
fi

X=$(echo "$BOUNDS" | cut -d, -f1 | tr -d ' ')
Y=$(echo "$BOUNDS" | cut -d, -f2 | tr -d ' ')
W=$(echo "$BOUNDS" | cut -d, -f3 | tr -d ' ')
H=$(echo "$BOUNDS" | cut -d, -f4 | tr -d ' ')
screencapture -x -R"$X,$Y,$W,$H" /tmp/win-w7d.png
~/.venvs/keith-measure/bin/python -c "
from PIL import Image
im = Image.open('/tmp/win-w7d.png').convert('RGB')
print('capture', im.size)
# the plugin canvas is dark; a terminal is dark too, so sample the wood rail: it must be brown
px = im.getpixel((int(im.width*0.02), int(im.height*0.5)))
print('left edge pixel', px, '-> wood' if px[0] > px[2] + 10 else '-> NOT wood, wrong window')
"
