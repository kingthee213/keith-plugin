#!/usr/bin/env bash
set -uo pipefail
cd /Users/solace/YoteMachine-build
export PATH="$HOME/cmakebin:$PATH"

echo "=== generator diff size ==="
git diff --stat tools/generate_editor_geometry.py

echo "=== does it still write? ==="
python3 tools/generate_editor_geometry.py --write 2>&1 | grep -vE "^//" | head -6
echo "exit=$?"

echo "=== gate ==="
cmake --build build --config Release --target keith-editor-tests 2>&1 | grep -E "BUILD SUCCEEDED|error:" | head -2
./build/Release/keith-editor-tests > /tmp/g18.txt 2>&1
echo "gate exit=$? PASS=$(grep -c '^PASS' /tmp/g18.txt) FAIL=$(grep -c '^FAIL' /tmp/g18.txt)"
grep '^FAIL' /tmp/g18.txt | head -6
