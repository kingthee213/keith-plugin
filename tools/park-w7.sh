#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

git checkout -q -b w7-composition
git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "W7 work in progress: reference-matched grid, composed face, panel fitting unfinished

Keeps the canvas at 1536x1024 with the reference's own panel rectangles, composes the face
from the owner's artwork in resources/reference/ instead of redrawing it, and makes the
generator's validator report every conflict at once rather than dying on the first.

Unfinished: the reference's panel rectangles are too small for our control count and the
generator refuses to write until each overflow is resolved. See briefs/W7-composition.md."
echo "wip branch head: $(git log --oneline -1)"

git checkout -q mags-rebuild-and-levels
echo "=== back on the demo branch ==="
git log --oneline -1
echo "dirty: $(git status --short | wc -l | tr -d ' ')"

./build/Release/keith-editor-tests > /tmp/g15.txt 2>&1
echo "geometry exit=$? PASS=$(grep -c '^PASS' /tmp/g15.txt) FAIL=$(grep -c '^FAIL' /tmp/g15.txt)"
python3 tools/editor_surface_check.py 2>&1 | tail -1
echo "=== branches ==="
git branch | head -5
