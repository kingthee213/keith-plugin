#!/usr/bin/env bash
# Build Keith VST3, AU and the standalone with the Xcode generator.
# iPlug2 is fetched by tools/bootstrap-deps at the pinned revision.
# This script has not been executed. There is no Mac in this workspace.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT}"

"${ROOT}/tools/bootstrap-deps"

cmake -S "${ROOT}" -B "${ROOT}/build" -G Xcode \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.7.10 \
  -DIPLUG2_UNIVERSAL=ON \
  -DIPLUG_DEPLOY_PLUGINS=OFF

cmake --build "${ROOT}/build" --config Release --target Keith-vst3 Keith-au Keith-app

echo "Requested deployment target: 11.7.10 (universal arm64 + x86_64)"
echo "Bundles under ${ROOT}/build/out:"
find "${ROOT}/build/out" -type d \( -name 'Keith.vst3' -o -name 'Keith.component' -o -name 'Keith.app' \) | sort
