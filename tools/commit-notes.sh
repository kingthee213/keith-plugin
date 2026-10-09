#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Record the orphaned velocity-to-amp parameter the final sweep found

editor_surface_check drops to 15 passed, 2 failed. Ten of the eleven parameters it
names are attached by the numeric range loops in EditorialLayout.h and the guard
simply cannot see a range. kParamVelToAmp (id 53) is the exception: no range covers
it and it is named nowhere, so it has no surface at all.

Adds the consolidated final-sweep script."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"
echo "dirty files: $(git status --short | wc -l | tr -d ' ')"
echo "remote head: $(git log --oneline -1 origin/mags-rebuild-and-levels)"
