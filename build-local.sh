#!/usr/bin/env bash
#
# Build MagsSynth on your Mac.
#
#   ./build-local.sh
#
# This does what the CI workflow does, without CI -- GitHub Actions is billing-blocked on
# this repo, which is why every run stopped after four seconds with zero steps.
#
# It stops at the first failure and prints the error, rather than carrying on and reporting
# something misleading. Building over a previous build is fine: CMake reuses what it can.

set -euo pipefail

cd "$(dirname "$0")"

say() { printf '\n\033[1m== %s\033[0m\n' "$1"; }
fail() { printf '\n\033[31mFAILED: %s\033[0m\n' "$1" >&2; exit 1; }

# --- what is on this machine ----------------------------------------------------------------
say "Checking the tools"

command -v cmake >/dev/null 2>&1 || fail "cmake is not installed.
  Install it with:  brew install cmake
  (or get it from https://cmake.org/download/ -- the .dmg installer is fine)"

command -v git >/dev/null 2>&1 || fail "git is not installed.
  Install it with:  xcode-select --install"

# Xcode or the command line tools, because the build uses the Xcode generator.
if ! xcode-select -p >/dev/null 2>&1; then
  fail "Xcode command line tools are missing.
  Install them with:  xcode-select --install
  Then run this script again."
fi

printf '  cmake   %s\n' "$(cmake --version | head -1)"
printf '  xcode   %s\n' "$(xcode-select -p)"

# --- dependencies ---------------------------------------------------------------------------
say "Fetching dependencies (iPlug2, Eigen)"

# bootstrap-deps downloads iPlug2 at its pinned revision and Eigen at 3.4.0. If they are
# already there it is a no-op.
./tools/bootstrap-deps || fail "bootstrap-deps could not fetch the dependencies.
  It needs network access. If you are behind a proxy, set https_proxy and retry."

[ -d third_party/iPlug2/IPlug ] || fail "iPlug2 did not download to third_party/iPlug2"

# --- configure ------------------------------------------------------------------------------
say "Configuring"

# Both architectures, so the plugin runs on Intel and Apple Silicon alike. This is why the
# build produces a universal binary.
#
# IPLUG_DEPLOY_PLUGINS=OFF stops iPlug2 trying to install the plugin into your plug-in
# folders during the build; the script copies them at the end instead, so you can see where
# they went.
cmake -S . -B build -G Xcode \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.7.10 \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DIPLUG_DEPLOY_PLUGINS=OFF \
  || fail "cmake could not configure. The error above says why."

# --- build ----------------------------------------------------------------------------------
say "Building the plugin (this takes a few minutes the first time)"

cmake --build build --config Release \
  --target MagsSynth-vst3 MagsSynth-au MagsSynth-app \
  || fail "the build failed. The error is above -- copy the first line that says 'error:' and
  send it over."

# The offline renderer, used to measure preset levels. Built because the level trims depend on
# it and a build that skips it cannot measure anything.
printf '\n  (also building the offline preset renderer)\n'
cmake --build build --config Release --target keith-recreation-render \
  || printf '  WARNING: the renderer did not build. The plugin is fine; level measurement will not run.\n'

# --- check what came out --------------------------------------------------------------------
say "Checking the result"

found=()

# iPlug2 puts these in different places depending on the generator. Search rather than assume.
for name in MagsSynth.vst3 MagsSynth.component MagsSynth.app; do
  p="$(find build -maxdepth 6 -name "$name" -print -quit 2>/dev/null || true)"
  if [ -n "$p" ]; then
    found+=("$p")
    printf '  built  %s\n' "$p"
  else
    printf '  MISSING  %s\n' "$name"
  fi
done

[ ${#found[@]} -gt 0 ] || fail "nothing was produced. The build reported success but there is no
  MagsSynth.vst3, .component or .app under the build directory."

# The samples must be inside each bundle or the plugin loads with silent drums. This is the
# check that the earlier zip passed and the earlier crash would have hidden.
say "Checking the samples shipped inside the bundles"

for p in "${found[@]}"; do
  if [ -d "$p/Contents/Resources/samples" ]; then
    n=$(find "$p/Contents/Resources/samples" -type f \( -name '*.wav' -o -name '*.flac' \) | wc -l | tr -d ' ')
    printf '  %-28s %s sample files\n' "$(basename "$p")" "$n"
    [ "$n" -gt 0 ] || fail "$(basename "$p") has a samples folder but no audio in it."
  else
    case "$p" in
      *.vst3|*.component|*.app)
        fail "$(basename "$p") has no Contents/Resources/samples -- the drums, guitars and choir
  would all be silent. Tell Sam: the sample staging step did not run." ;;
    esac
  fi
done

# --- done -----------------------------------------------------------------------------------
say "Done"

cat <<EOF

Your plugin is here:

$(printf '  %s\n' "${found[@]}")

To install it, copy the VST3 and the component into your plug-in folders:

  mkdir -p ~/Library/Audio/Plug-Ins/VST3 ~/Library/Audio/Plug-Ins/Components
  cp -R "$(cd "$(dirname "${found[0]}")" && pwd)"/MagsSynth.vst3 ~/Library/Audio/Plug-Ins/VST3/
  cp -R "$(cd "$(dirname "${found[0]}")" && pwd)"/MagsSynth.component ~/Library/Audio/Plug-Ins/Components/

Or just double-click the .app to run it standalone -- no DAW, no installation.

If macOS refuses to open the .app because it is from an unidentified developer:

  right-click the app -> Open -> Open

That is expected: the build is ad-hoc signed, not notarised by Apple.

EOF
