#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

cp /tmp/win8.png design/screenshots/w8-editor-fit.png
git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Make the toggle labels readable

IVTabSwitchControl draws one label colour for every tab, so the light selected fill
under a cream label could never read on FREE, OFF, ACT or any of the effects
indicators. The tab style now fills light in both states with dark ink text and marks
the selected tab in hot pink. The effects ACT indicator draws dark ink on green.

kHotPink had 0x4FA3 in a byte position, which resolved to a pale pink; corrected to
0xFF, 0x4F, 0xA3.

Verified from a window capture read at 1:1 and as a whole: every toggle label reads,
geometry gate 636 passed 0 failed."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"

cmake --build build --config Release --target MagsSynth-vst3 MagsSynth-au 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -4

rm -rf ~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3 ~/Library/Audio/Plug-Ins/Components/MagsSynth.component
cp -R build/out/Release/MagsSynth.vst3 ~/Library/Audio/Plug-Ins/VST3/
cp -R build/out/Release/MagsSynth.component ~/Library/Audio/Plug-Ins/Components/
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/VST3/MagsSynth.vst3 2>/dev/null
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/Components/MagsSynth.component 2>/dev/null

echo "installed:"
ls ~/Library/Audio/Plug-Ins/VST3/ ~/Library/Audio/Plug-Ins/Components/
auval -v aumu MgS1 Mags 2>&1 | tail -4
