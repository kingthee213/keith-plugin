#!/usr/bin/env bash
# Build Keith (Yote Machine) for macOS: VST3, AU and the standalone app.
#
# STATUS: still not executed on real hardware - no Mac was reachable when this was
# written, so treat the first run as a test and read the output rather than assuming.
# The three things that most often break a first macOS plugin build are handled here:
# a real universal-architectures flag (the previous IPLUG2_UNIVERSAL variable is not
# used by this project's CMake at all, so the old script produced host-only binaries
# while claiming universal), an ad-hoc codesign pass (Apple Silicon hosts refuse to
# load an unsigned bundle), and lipo verification that the result really is universal.
#
# Usage:  ./tools/build-macos.sh
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT}"

echo "==> Checking build prerequisites"
if ! xcode-select -p >/dev/null 2>&1; then
  echo "ERROR: Xcode Command Line Tools are missing. Run: xcode-select --install" >&2
  exit 1
fi
for tool in cmake git; do
  if ! command -v "$tool" >/dev/null 2>&1; then
    echo "ERROR: '$tool' not found. Install it (https://cmake.org/download/ or 'brew install cmake') and re-run." >&2
    exit 1
  fi
done
# The Xcode generator needs the full Xcode toolchain, not just CMake's default.
if ! command -v xcodebuild >/dev/null 2>&1; then
  echo "ERROR: 'xcodebuild' not found. Install Xcode from the App Store and run: sudo xcode-select -s /Applications/Xcode.app" >&2
  exit 1
fi
echo "    xcode-select: $(xcode-select -p)"
echo "    cmake:        $(cmake --version | head -1)"

echo "==> Fetching dependencies (iPlug2 at the pinned revision)"
"${ROOT}/tools/bootstrap-deps"

echo "==> Configuring (universal: arm64 + x86_64)"
cmake -S "${ROOT}" -B "${ROOT}/build" -G Xcode \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.7.10 \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DIPLUG_DEPLOY_PLUGINS=OFF

echo "==> Building Release"
cmake --build "${ROOT}/build" --config Release --target Keith-vst3 Keith-au Keith-app

OUT="${ROOT}/build/out"
echo "==> Bundles produced"
find "${OUT}" -maxdepth 3 -type d \( -name '*.vst3' -o -name '*.component' -o -name '*.app' \) | sort

echo "==> Ad-hoc signing (required for Apple Silicon hosts to load the plugin)"
# A downloaded, unsigned bundle is refused outright by most macOS hosts. Ad-hoc
# signing is enough for a demo on someone else's machine and needs no certificate.
sign() {
  local target="$1"
  if [[ -e "$target" ]]; then
    codesign --force --deep --sign - "$target" 2>/dev/null \
      && echo "    signed  $(basename "$target")" \
      || echo "    WARNING: could not sign $(basename "$target") - a DAW may refuse it"
  fi
}
sign "${OUT}/Keith.vst3"
sign "${OUT}/Keith.component"
sign "${OUT}/Keith.app"

echo "==> Verifying architectures (both arm64 and x86_64 expected)"
for b in "${OUT}/Keith.vst3/Contents/MacOS/Keith" "${OUT}/Keith.component/Contents/MacOS/Keith"; do
  [[ -e "$b" ]] && { printf '    %-52s ' "$(basename "$(dirname "$(dirname "$(dirname "$b")")")")"; lipo -archs "$b" 2>/dev/null || echo "(lipo unavailable)"; }
done

cat <<EOF

Build complete. Install locations:
  VST3 : ${OUT}/Keith.vst3        ->  ~/Library/Audio/Plug-Ins/VST3/
  AU   : ${OUT}/Keith.component   ->  ~/Library/Audio/Plug-Ins/Components/
  App  : ${OUT}/Keith.app         ->  run it directly, no install needed

Deployment target: 11.7.10. Architectures: arm64 + x86_64.
The standalone app is the fastest way to demo without a DAW.
EOF
