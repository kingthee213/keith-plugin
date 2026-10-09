#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:/opt/homebrew/bin:/usr/local/bin:$PATH"

PV="$(command -v pluginval || find /opt/homebrew /usr/local "$HOME" -maxdepth 4 -name pluginval -type f 2>/dev/null | head -1)"
echo "pluginval: ${PV:-not found}"

echo "=== gates ==="
python3 tools/editor_surface_check.py 2>&1 | tail -1
python3 tools/guard_self_test.py
./build/Release/keith-editor-tests > /tmp/g13.txt 2>&1
echo "geometry exit=$? PASS=$(grep -c '^PASS' /tmp/g13.txt) FAIL=$(grep -c '^FAIL' /tmp/g13.txt)"
./build/Release/keith-synth-tests 2>&1 | tail -1
ctest --test-dir build -C Release 2>&1 | grep -E "tests passed|tests failed" | head -2

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Bind the orphaned velocity-to-amp parameter and teach the guard to read a range

The geometry already reserved a rectangle for kParamVelToAmp (id 53), paired with
kParamVelToFilter in the filter panel's third row, but nothing attached a control to
it, so that row drew one knob where two were planned. It is a VEL knob now and the
panel reads as three rows of two.

The guard's two reachability tests searched the editor text for parameter names and
could not see the three numeric range loops that bind the amp, filter and modifier
strips, so they reported ten reachable parameters as missing. That is the wrong
answer and it would have hidden a real orphan beside them. One helper now expands a
range against the enum and both tests use it.

tools/guard_self_test.py removes the attach, runs the guard, and restores the file,
proving the guard still fails by name when a parameter loses its control.

surface check 17 passed 0 failed, geometry 636 passed 0 failed, synth 137 passed 0
failed."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"

cmake --build build --config Release --target MagsSynth-vst3 MagsSynth-au 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -3

rm -rf ~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3 ~/Library/Audio/Plug-Ins/Components/MagsSynth.component
cp -R build/out/Release/MagsSynth.vst3 ~/Library/Audio/Plug-Ins/VST3/
cp -R build/out/Release/MagsSynth.component ~/Library/Audio/Plug-Ins/Components/
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3 2>/dev/null
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/Components/MagsSynth.component 2>/dev/null

echo "=== installed and validated ==="
ls ~/Library/Audio/Plug-Ins/VST3/ ~/Library/Audio/Plug-Ins/Components/
auval -v aumu MgS1 Mags 2>&1 | tail -3
if [ -n "$PV" ]; then
  "$PV" --strictness-level 10 --validate-in-process --skip-gui-tests ~/Library/Audio/Plug-Ins/Components/MagsSynth.component 2>&1 | tail -2
fi

echo "dirty files: $(git status --short | wc -l | tr -d ' ')"
