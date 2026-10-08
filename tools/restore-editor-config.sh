#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== the gate still passes with the editor config in place ==="
cmake --build build --config Release --target keith-editor-tests 2>&1 | grep -E "BUILD SUCCEEDED|BUILD FAILED|error:" | head -3
./build/Release/keith-editor-tests > /tmp/g11.txt 2>&1
echo "exit=$? PASS=$(grep -c '^PASS' /tmp/g11.txt) FAIL=$(grep -c '^FAIL' /tmp/g11.txt)"

git add -A
git -c user.name=sss0lace -c user.email=noahissaabbas@gmail.com commit -q -m "Restore the editor config I deleted without reading it

The W6 worker created .clangd and tools/.clangd so clangd would read the C++17 the
build uses instead of defaulting and reporting std::clamp in EditorGeometry.h as
unknown. I removed both as scratch files before committing, which threw away a
deliberate fix without checking what it was for.

One file at the repo root replaces the pair. Verified the include directories exist
and the geometry gate still passes."
git push -q origin mags-rebuild-and-levels
echo "commit: $(git log --oneline -1)"
echo "dirty files: $(git status --short | wc -l | tr -d ' ')"
