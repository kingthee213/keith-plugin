#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Cut the owner's artwork out of the reference and spec the composition rebuild

The reference is the spec and it already contains the cityscape, the hardware graphic,
the wood rail with the wordmark plaque and the wood frame edges. Re-deriving those in
code is what produced a recreation the owner called a mess. tools/extract_reference_assets.py
records the crops so they are reproducible, and resources/reference/ holds them.

Also records what the current build gets wrong: the canvas is 8:5 where the reference is
3:2, the background is wood where the reference is near black with wood only as the left
column and a thin frame, the cityscape is an 80 px column where the reference uses it as
the hero at 550 by 302, and the smallest text is 9 px against the reference's 11.
briefs/W7-composition.md carries the build spec with panel rectangles and sampled fills."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"
echo "dirty: $(git status --short | wc -l | tr -d ' ')"
ls resources/reference/
