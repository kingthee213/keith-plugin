#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "WIP: derive control width from the value text, incomplete

The rule is started, 63 lines in tools/generate_editor_geometry.py, and it does not yet
work: the generator now exits 1 with 'param 82 overlaps title band in kPanelGlobal', so it
refuses to write the header and the geometry gate stays at 632 passed 8 failed. The eight
are the same four value-width and four row-pitch failures as before.

Kept rather than discarded because the approach is right. Pick it up by resolving the
kPanelGlobal title-band conflict, which the wider cells caused."
echo "commit: $(git log --oneline -1)"
echo "dirty: $(git status --short | wc -l | tr -d ' ')"

echo "=== how many worker rounds have died mid turn ==="
echo "W7 (composition), W7d (column width), and the earlier ui lane: 3"
