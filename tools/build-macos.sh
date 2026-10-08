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
# The Xcode generator nests the bundles one level down (build/out/Release/...),
# so locating them by name is safer than assuming a fixed depth. The previous
# version hard-coded build/out/<name>, which meant the signing step and the
# printed install paths both pointed at directories that do not exist.
echo "==> Bundles produced"
bundle_path() { find "${OUT}" -type d -name "$1" -maxdepth 3 -print -quit 2>/dev/null; }
VST3="$(bundle_path 'Keith.vst3')"
AU="$(bundle_path 'Keith.component')"
APP="$(bundle_path 'Keith.app')"
for b in "$VST3" "$AU" "$APP"; do [[ -n "$b" ]] && echo "    $b"; done

echo "==> Ad-hoc signing (required for Apple Silicon hosts to load the plugin)"
# A downloaded, unsigned bundle is refused outright by most macOS hosts. Ad-hoc
# signing is enough for a demo on someone else's machine and needs no certificate.
sign() {
  local target="$1"
  if [[ -n "$target" && -e "$target" ]]; then
    codesign --force --deep --sign - "$target" 2>/dev/null \
      && echo "    signed  $(basename "$target")" \
      || echo "    WARNING: could not sign $(basename "$target") - a DAW may refuse it"
  fi
}
sign "$VST3"
sign "$AU"
sign "$APP"

echo "==> Verifying architectures (arm64 and x86_64 expected)"
for b in "$VST3" "$AU"; do
  [[ -n "$b" ]] || continue
  bin="$(find "$b/Contents/MacOS" -maxdepth 1 -type f -print -quit 2>/dev/null)"
  [[ -n "$bin" ]] && printf '    %-28s %s\n' "$(basename "$b")" "$(lipo -archs "$bin" 2>/dev/null || echo '(lipo unavailable)')"
done

cat <<EOF

Build complete.
  VST3 : ${VST3:-NOT BUILT}
  AU   : ${AU:-NOT BUILT}
  App  : ${APP:-NOT BUILT}

Install:
  mkdir -p ~/Library/Audio/Plug-Ins/VST3 ~/Library/Audio/Plug-Ins/Components
  cp -R "${VST3:-<vst3>}" ~/Library/Audio/Plug-Ins/VST3/ 2>/dev/null
  cp -R "${AU:-<au>}"    ~/Library/Audio/Plug-Ins/Components/ 2>/dev/null

Deployment target: 11.7.10. The standalone app is the fastest demo with no DAW.
EOF
