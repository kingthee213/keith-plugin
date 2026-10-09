#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Point the build script at the bundles that exist

The script searched for Keith.vst3 and Keith.app, which resolve to nothing now that the
bundles are MagsSynth.*, so the install paths it printed were dead. Verified both: the
three MagsSynth names resolve under build/out and the two Keith names return empty. Also
drops the stale claim in the header that the script had never been run on a Mac.

Corrects my own note about the bottom rail help text. I called it overlapping after
reading a downscaled capture; at 1:1 it is two clean lines. The LOADED SOUNDS preset row
is a real collision and now carries the arithmetic that shows the panel is over-subscribed."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"
echo "dirty: $(git status --short | wc -l | tr -d ' ')"
echo "remote: $(git log --oneline -1 origin/mags-rebuild-and-levels)"
