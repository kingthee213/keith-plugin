#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

cp /tmp/win10.png design/screenshots/w10-editor-fit.png
git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Remove two decorations that covered the controls underneath them

The GLOBAL panel drew kParamGain as an unlabelled bar, so it sat pegged at 100
percent whenever the synth was idle and duplicated the labelled VOL knob beside it.
The LFO panel drew a waveform box inside the rate knob's own rectangle, so the knob's
label and value were covered by the squiggle. Both were hand-placed IRECT decorations,
which is why the geometry gate could not see either one.

Removed both. The oscillator waveform squiggles stay; they are clear of their knobs.

Verified from a window capture: LFO rate and depth read cleanly in both sections, no
decoration sits over a control, geometry gate 636 passed 0 failed."
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
