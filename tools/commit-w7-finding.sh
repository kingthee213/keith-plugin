#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Record why the reference grid cannot hold our controls, and compose the face from his art

Measured on the reference's own coordinates: the LFO panel is 124 by 392 and carries four
controls there, ours needs eight plus a label row per block, and a full knob needs 73 px
of height, so two LFO blocks need 418 px of content in the 346 available. The amp
modifiers needed 134 px of width where the grid leaves 72. GLOBAL carries nine controls
where the reference shows three, EFFECTS twelve where it shows four.

So matching the reference exactly and keeping 111 controls visible at the reference's
control size are in conflict, and which one gives is a product call.

Also lands the composed face: the reference is the artwork source now, with the cityscape,
the hardware graphic, the wood rail and the frame edges cut from his render into
resources/reference/ and assembled by tools/compose_face.py rather than redrawn."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"
echo "dirty: $(git status --short | wc -l | tr -d ' ')"
echo "wip branch: $(git log --oneline -1 w7-composition)"
