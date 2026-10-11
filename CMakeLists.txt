cmake_minimum_required(VERSION 3.14)

# Ignored when the host is not Apple. Set before project() so a fresh Xcode
# configure picks up the BRIEF §11 floor (macOS 11.7.10) in the toolchain.
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.7.10" CACHE STRING "Minimum macOS version")

project(MagsSynth VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(KeithPlatform)

if(NOT DEFINED IPLUG2_DIR)
  set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/iPlug2" CACHE PATH "iPlug2 root directory")
endif()

if(NOT EXISTS "${IPLUG2_DIR}/iPlug2.cmake")
  message(FATAL_ERROR
    "iPlug2 not found at ${IPLUG2_DIR}. Run tools/bootstrap-deps (or bootstrap-deps.ps1 on Windows).")
endif()

set(IPLUG_DEPLOY_PLUGINS OFF CACHE BOOL "Keep build artefacts in build/out" FORCE)

# C++20. iPlug2 defaults to 17 and applies its choice globally, so this must be set before
# iPlug2.cmake is included.
#
# The reason is Bungee, the time-stretcher behind the sampler's warp feature: it uses designated
# initialisers, which are C++20. Raising the whole project rather than building Bungee as a
# separate C++20 target is the owner's call, and it is the simpler arrangement to reason about --
# one language standard everywhere, no mixed-standard boundary to maintain.
#
# iPlug2's own check only rejects a standard BELOW 17, so 20 passes it.
set(IPLUG2_CXX_STANDARD 20 CACHE STRING "C++ standard for iPlug2" FORCE)

include(${IPLUG2_DIR}/iPlug2.cmake)
find_package(iPlug2 REQUIRED)

include_directories(src)
enable_testing()

# The python gates need numpy, which the macOS system python does not ship. Point the tests
# at the shared tools venv when it exists, and let a builder override it.
# Setup: bash tools/setup-tools-venv.sh
if(NOT DEFINED KEITH_PYTHON)
  if(EXISTS "$ENV{HOME}/.venvs/keith-measure/bin/python")
    set(KEITH_PYTHON "$ENV{HOME}/.venvs/keith-measure/bin/python" CACHE FILEPATH "Python for the verification gates")
  else()
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    set(KEITH_PYTHON ${Python3_EXECUTABLE} CACHE FILEPATH "Python for the verification gates")
  endif()
endif()
add_executable(keith-synth-tests tools/synth_offline_test.cpp)
target_compile_features(keith-synth-tests PRIVATE cxx_std_20)
add_test(NAME synth-offline COMMAND keith-synth-tests)
add_executable(keith-render-voice tools/render_voice.cpp)
target_compile_features(keith-render-voice PRIVATE cxx_std_20)
add_executable(keith-editor-tests tools/editor_geometry_test.cpp)
target_compile_features(keith-editor-tests PRIVATE cxx_std_20)
add_test(NAME editor-geometry COMMAND keith-editor-tests)

# Measures Bungee's pipeline latency. Runs on the build machine because there is no compiler on
# the development box, and because the target machine is the one whose number matters.
add_executable(keith-warp-latency tools/warp_latency_test.cpp)
target_compile_features(keith-warp-latency PRIVATE cxx_std_20)

# Ship an editor: hosts that get a UI-less VST3 show an empty wrapper, and the
# standalone target needs a window to be usable at all.
if(APPLE)
  set(KEITH_FORMATS VST3 AU APP)
else()
  set(KEITH_FORMATS VST3 APP)
endif()

# --- Bungee: the time-stretcher behind the sampler's warp feature ------------------------
#
# Vendored in third_party/bungee (MPL-2.0). It is a SEPARATE static library rather than extra
# sources in the plugin target, for two reasons:
#
#   1. Bungee is third-party code under its own licence. Keeping it a distinct target keeps the
#      boundary visible, and the plugin and Bungee are separate compilation units.
#   2. The plugin talks to it through Bungee's C API (extern "C" function table, plain C types
#      across), so nothing about Bungee's internals reaches our headers.
#
# pffft is vendored alongside it (BSD/NCAR licence, third_party/pffft) and is Bungee's only FFT:
# Fourier.cpp has an #if BUNGEE_USE_PFFFT with no alternative implementation, so it is required.
#
# Eigen is fetched by bootstrap-deps rather than committed -- see the note there.
set(BUNGEE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bungee")
set(EIGEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/eigen")

if(EXISTS "${BUNGEE_DIR}/src/Stretcher.cpp" AND EXISTS "${EIGEN_DIR}/Eigen/Core")
  add_library(keith-pffft STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/pffft/pffft.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/pffft/fftpack.c")
  target_include_directories(keith-pffft PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/pffft")
  # Bungee's own CMake uses these two on pffft; -ffast-math is safe for an FFT, and the guard
  # against finite-math-only stops it optimising away NaN checks.
  target_compile_options(keith-pffft PRIVATE -ffast-math -fno-finite-math-only)

  file(GLOB BUNGEE_SOURCES "${BUNGEE_DIR}/src/*.cpp")
  add_library(keith-bungee STATIC ${BUNGEE_SOURCES})
  target_include_directories(keith-bungee PUBLIC
    "${BUNGEE_DIR}"
    "${EIGEN_DIR}")
  target_link_libraries(keith-bungee PUBLIC keith-pffft)
  target_compile_features(keith-bungee PUBLIC cxx_std_20)
  target_compile_definitions(keith-bungee PRIVATE
    eigen_assert=BUNGEE_ASSERT1
    EIGEN_DONT_PARALLELIZE=1)
  # Bungee sets this on itself: signed overflow is relied upon to wrap in the grain arithmetic.
  target_compile_options(keith-bungee PRIVATE -fwrapv)
  set_target_properties(keith-bungee PROPERTIES POSITION_INDEPENDENT_CODE ON)

  # version.cpp is just `const char* versionDescription = BUNGEE_VERSION;` -- Bungee's own CMake
  # sets the macro on that one file from its version variable. Set here on the whole target,
  # which is simpler and equivalent: it uses the pinned commit rather than a release number,
  # which is more accurate for a vendored copy anyway.
  target_compile_definitions(keith-bungee PRIVATE
    BUNGEE_VERSION="bungee-052451e")
else()
  # Not fetched. The plugin builds without warp rather than failing: bootstrap-deps missing a
  # step should not stop the rest of the work.
  message(STATUS "Bungee or Eigen not present -- building without the warp stretcher")
endif()

iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    src/Keith.cpp
    # Header-only additions that nothing includes yet still need type-checking. A header no
    # translation unit includes is never compiled, so a build would pass while proving nothing
    # about it. This pulls in src/synth/WavetableOsc.h for real.
    tools/wavetable_compilecheck.cpp
    # The sampler's decoders. This is the ONE translation unit that defines the dr_libs
    # implementation macros -- anywhere else and the symbols collide at link time, which is
    # what the first attempt did.
    src/sampler/SampleLoader.cpp
    # Header-only sampler types, pulled in so they are type-checked.
    tools/sampler_compilecheck.cpp
    src/Keith.h
    src/Keith_DSP.h
    src/editor/KeithControls.h
    src/editor/KeithTheme.h
    src/synth/CoreSynth.h
    src/drums/DrumSynth.h
    src/fx/PatchEffects.h
    src/fx/TapeAnalog.h
    resources/resource.h
  RESOURCES
    resources/images/chassis.png
    resources/fonts/Bangers-Regular.ttf
    resources/fonts/Oswald-Bold.ttf
    resources/fonts/PirataOne-Regular.ttf
    resources/fonts/SpaceMono-Regular.ttf
    resources/fonts/SpaceMono-Bold.ttf
  UI IGRAPHICS
    FORMATS ${KEITH_FORMATS}
  LINK
    iPlug2::Extras::Synth
  EXCLUDE_FORMATS CLAP AAX AUV3 WAM WASM_DSP WASM_UI VST2
)

# Link Bungee into every plugin format. Done by target name rather than through iplug_add_plugin's
# LINK block because the formats are separate targets (VST3, AU, APP) and the guard matters: if
# bootstrap-deps has not fetched Eigen, keith-bungee does not exist and linking must not be
# attempted.
if(TARGET keith-bungee)
  foreach(_fmt IN ITEMS MagsSynth-vst3 MagsSynth-au MagsSynth-app)
    if(TARGET ${_fmt})
      target_link_libraries(${_fmt} PRIVATE keith-bungee)
    endif()
  endforeach()
endif()

# The offline test binaries include the sampler headers, and WarpVoice.h constructs a Bungee
# stretcher. Without this they fail to link on the Bungee symbols.
#
# keith-recreation-render is NOT in this list, and cannot be: this loop runs at line ~179 and that
# target is not created until line ~316, so `if(TARGET ...)` is false and the link is silently
# skipped. It is linked after its add_executable instead. That mistake cost two CI runs.
foreach(_t IN ITEMS keith-synth-tests keith-render-voice keith-editor-tests keith-warp-latency)
  if(TARGET ${_t} AND TARGET keith-bungee)
    target_link_libraries(${_t} PRIVATE keith-bungee)
    add_test(NAME ${_t} COMMAND ${_t})
  endif()
endforeach()

# Editor faces. Windows embeds them with resources/main.rc. macOS LoadFont looks in
# the bundle's Contents/Resources (NSBundle resourcePath), so the same files are
# copied there for VST3, AU and the APP, and also beside the .app.
set(KEITH_FONT_RESOURCES
  fonts/Bangers-Regular.ttf
  fonts/Oswald-Bold.ttf
  fonts/PirataOne-Regular.ttf
  fonts/SpaceMono-Regular.ttf
  fonts/SpaceMono-Bold.ttf)
set(KEITH_IMAGE_RESOURCES
  images/chassis.png)

if(APPLE)
  foreach(_target IN ITEMS MagsSynth-vst3 MagsSynth-au MagsSynth-app)
    if(TARGET ${_target})
      set_target_properties(${_target} PROPERTIES
        MACOSX_DEPLOYMENT_TARGET "11.7.10"
        XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "11.7.10"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
    endif()
  endforeach()
endif()

# resources/main.rc is a Windows resource script. iPlug2 already adds it to the
# APP target. The VST3 target does not get it, so attach it here on Windows only.
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/main.rc" AND TARGET MagsSynth-vst3)
  target_sources(MagsSynth-vst3 PRIVATE resources/main.rc)
  set_source_files_properties(resources/main.rc PROPERTIES
    COMPILE_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts\" /I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"")
endif()

# Copy named files into destination, flattening to the file name.
function(keith_stage_files target destination)
  if(NOT TARGET ${target})
    return()
  endif()
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${destination}"
    COMMENT "Preparing ${destination}")
  foreach(_rel IN LISTS ARGN)
    get_filename_component(_name "${_rel}" NAME)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/${_rel}"
        "${destination}/${_name}"
      COMMENT "Copying ${_name} for ${target}")
  endforeach()
endfunction()

# iPlug2's CMake does not copy the RESOURCES list next to the Windows binaries.
# The .rc embeds the faces. These copies keep them on disk as well.
if(WIN32 AND TARGET MagsSynth-vst3)
  keith_stage_files(MagsSynth-vst3 "${CMAKE_BINARY_DIR}/out/MagsSynth.vst3/Contents/Resources"
    ${KEITH_FONT_RESOURCES} fonts/OFL.txt)
endif()

# Headless proof that every baked preset survives Keith::RestorePreset.
# IPLUG_EDITOR stays off so this target does not pull IGraphics. Building
# keith-synth-tests builds this too, because the verification line names that target.
set(_keith_vst3_sdk "${IPLUG2_DIR}/Dependencies/IPlug/VST3_SDK")
if(WIN32)
  set(_keith_vst3_entry "${_keith_vst3_sdk}/public.sdk/source/main/dllmain.cpp")
elseif(APPLE)
  set(_keith_vst3_entry "${_keith_vst3_sdk}/public.sdk/source/main/macmain.cpp")
endif()
add_executable(keith-preset-restore-tests
  tools/preset_restore_test.cpp
  src/Keith.cpp
  "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3.cpp"
  "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3_ProcessorBase.cpp"
  "${_keith_vst3_sdk}/base/source/baseiids.cpp"
  "${_keith_vst3_sdk}/base/source/fdebug.cpp"
  "${_keith_vst3_sdk}/base/source/fobject.cpp"
  "${_keith_vst3_sdk}/base/source/fstring.cpp"
  "${_keith_vst3_sdk}/base/source/updatehandler.cpp"
  "${_keith_vst3_sdk}/base/thread/source/flock.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/funknown.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/ustring.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/coreiids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/common/pluginview.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/common/commoniids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstbus.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstcomponent.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstcomponentbase.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstinitiids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstparameters.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstsinglecomponenteffect.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/main/pluginfactory.cpp"
  ${_keith_vst3_entry}
)
target_compile_features(keith-preset-restore-tests PRIVATE cxx_std_20)
target_include_directories(keith-preset-restore-tests PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/resources"
  "${IPLUG2_DIR}/IPlug/VST3"
  "${_keith_vst3_sdk}"
  "${_keith_vst3_sdk}/pluginterfaces"
  "${_keith_vst3_sdk}/public.sdk"
  "${_keith_vst3_sdk}/public.sdk/source"
  "${_keith_vst3_sdk}/base"
)
target_compile_definitions(keith-preset-restore-tests PRIVATE
  VST3_API
  IPLUG_DSP=1
  IPLUG_EDITOR=0
  NO_IGRAPHICS
  $<$<CONFIG:Debug>:DEVELOPMENT>
  $<$<NOT:$<CONFIG:Debug>>:RELEASE>
)
target_link_libraries(keith-preset-restore-tests PRIVATE iPlug2::IPlug iPlug2::Extras::Synth)
if(APPLE)
  set_source_files_properties(
    "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3.cpp"
    "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3_ProcessorBase.cpp"
    TARGET_DIRECTORY keith-preset-restore-tests
    PROPERTIES LANGUAGE OBJCXX)
  target_link_libraries(keith-preset-restore-tests PRIVATE
    "-framework CoreFoundation" "-framework Cocoa")
endif()
add_test(NAME preset-restore COMMAND keith-preset-restore-tests)
add_dependencies(keith-synth-tests keith-preset-restore-tests)

# Plugin-path render for the recreation gate. The assertion lives in Python so
# it calls the same measure_signal.py that analyse_references.py imports.
add_executable(keith-recreation-render
  tools/recreation_render.cpp
  src/Keith.cpp
  # SampleLoader.cpp defines LoadFile, which the sampler state calls for the path fallback
  # when a sample is too large to embed. Keith.cpp references it, so the renderer must compile it
  # too -- without this the link fails on an undefined LoadFile from SamplerEngine::UnserializeState.
  src/sampler/SampleLoader.cpp
  "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3.cpp"
  "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3_ProcessorBase.cpp"
  "${_keith_vst3_sdk}/base/source/baseiids.cpp"
  "${_keith_vst3_sdk}/base/source/fdebug.cpp"
  "${_keith_vst3_sdk}/base/source/fobject.cpp"
  "${_keith_vst3_sdk}/base/source/fstring.cpp"
  "${_keith_vst3_sdk}/base/source/updatehandler.cpp"
  "${_keith_vst3_sdk}/base/thread/source/flock.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/funknown.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/ustring.cpp"
  "${_keith_vst3_sdk}/pluginterfaces/base/coreiids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/common/pluginview.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/common/commoniids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstbus.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstcomponent.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstcomponentbase.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstinitiids.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstparameters.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/vst/vstsinglecomponenteffect.cpp"
  "${_keith_vst3_sdk}/public.sdk/source/main/pluginfactory.cpp"
  ${_keith_vst3_entry}
)
target_compile_features(keith-recreation-render PRIVATE cxx_std_20)

# Bungee, linked HERE rather than in the loop further up: this target does not exist until this
# point, so an `if(TARGET ...)` test earlier is false and the link is skipped without a word. The
# renderer includes src/Keith.cpp, which pulls in WarpStretcher.h, which needs bungee/Bungee.h.
if(TARGET keith-bungee)
  target_link_libraries(keith-recreation-render PRIVATE keith-bungee)
endif()
target_include_directories(keith-recreation-render PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/resources"
  "${IPLUG2_DIR}/IPlug/VST3"
  "${_keith_vst3_sdk}"
  "${_keith_vst3_sdk}/pluginterfaces"
  "${_keith_vst3_sdk}/public.sdk"
  "${_keith_vst3_sdk}/public.sdk/source"
  "${_keith_vst3_sdk}/base"
)
target_compile_definitions(keith-recreation-render PRIVATE
  VST3_API
  IPLUG_DSP=1
  IPLUG_EDITOR=0
  NO_IGRAPHICS
  $<$<CONFIG:Debug>:DEVELOPMENT>
  $<$<NOT:$<CONFIG:Debug>>:RELEASE>
)
target_link_libraries(keith-recreation-render PRIVATE iPlug2::IPlug iPlug2::Extras::Synth)
if(APPLE)
  set_source_files_properties(
    "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3.cpp"
    "${IPLUG2_DIR}/IPlug/VST3/IPlugVST3_ProcessorBase.cpp"
    TARGET_DIRECTORY keith-recreation-render
    PROPERTIES LANGUAGE OBJCXX)
  target_link_libraries(keith-recreation-render PRIVATE
    "-framework CoreFoundation" "-framework Cocoa")
endif()
add_test(NAME recreation-gate
  COMMAND "${KEITH_PYTHON}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/recreation_gate.py" $<TARGET_FILE:keith-recreation-render>)
set_tests_properties(recreation-gate PROPERTIES TIMEOUT 1800)
add_dependencies(keith-synth-tests keith-recreation-render)

if(WIN32 AND TARGET MagsSynth-app)
  keith_stage_files(MagsSynth-app "${CMAKE_BINARY_DIR}/out"
    ${KEITH_FONT_RESOURCES} fonts/OFL.txt)
endif()

# macOS LoadFont resolves a TTF from the bundle resource directory. Copy the
# faces there for each bundle. The APP also gets them beside the .app, matching
# the standalone layout used on Windows.
if(APPLE)
  foreach(_target IN ITEMS MagsSynth-vst3 MagsSynth-au MagsSynth-app)
    if(TARGET ${_target})
      keith_stage_files(${_target} "$<TARGET_BUNDLE_DIR:${_target}>/Contents/Resources"
        ${KEITH_FONT_RESOURCES} ${KEITH_IMAGE_RESOURCES} fonts/OFL.txt)
    endif()
  endforeach()
  if(TARGET MagsSynth-app)
    keith_stage_files(MagsSynth-app "$<TARGET_BUNDLE_DIR:MagsSynth-app>/.."
      ${KEITH_FONT_RESOURCES} fonts/OFL.txt)
  endif()
endif()
