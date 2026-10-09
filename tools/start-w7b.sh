#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"
COMMIT_NAME="-c user.name=sss0lace -c user.email=noahissaabbas@gmail.com"

git add -A
git $COMMIT_NAME commit -q -m "Settle the fitting question: the reference is a style guide, not a contract

The owner's call. Match the aesthetics as closely as possible and lay out whatever
obviously has to differ differently. Panel rectangles are a guide; where our control
count does not fit, the panel grows. Every one of the 111 controls stays visible and
nothing shrinks below the label-plus-value minimum."
git push -q origin mags-rebuild-and-levels
echo "mags head: $(git log --oneline -1)"

git checkout -q w7-composition
git checkout mags-rebuild-and-levels -- briefs/W7-composition.md
git $COMMIT_NAME commit -q -m "Take the settled fitting rule onto the composition branch"
echo "w7 head: $(git log --oneline -1)"

echo "=== state of the w7 tree ==="
git status --short | head -12
echo "=== does the generator write the header now? ==="
python3 tools/generate_editor_geometry.py --write 2>&1 | grep -vE "^//" | head -12
