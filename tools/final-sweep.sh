#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== editor geometry gate ==="
./build/Release/keith-editor-tests > /tmp/final-geom.txt 2>&1
echo "exit=$? PASS=$(grep -c '^PASS' /tmp/final-geom.txt) FAIL=$(grep -c '^FAIL' /tmp/final-geom.txt)"

echo "=== synth tests ==="
./build/Release/keith-synth-tests 2>&1 | tail -1

echo "=== editor surface check ==="
python3 tools/editor_surface_check.py 2>&1 | tail -1

echo "=== panel occupancy ==="
python3 tools/fit_budget.py 2>&1 | tail -2

echo "=== ctest ==="
ctest --test-dir build -C Release 2>&1 | tail -3

echo "=== pluginval VST3 ==="
pluginval --strictness-level 10 --validate-in-process --skip-gui-tests \
  ~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3 2>&1 | tail -2

echo "=== pluginval AU ==="
pluginval --strictness-level 10 --validate-in-process --skip-gui-tests \
  ~/Library/Audio/Plug-Ins/Components/MagsSynth.component 2>&1 | tail -2

echo "=== auval ==="
auval -v aumu MgS1 Mags 2>&1 | tail -2

echo "=== repo state ==="
git status --short | wc -l
git log --oneline -6
git log --oneline -1 origin/mags-rebuild-and-levels
