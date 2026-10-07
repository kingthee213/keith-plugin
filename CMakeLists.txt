cmake_minimum_required(VERSION 3.14)
project(Keith VERSION 0.1.0)

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

include(${IPLUG2_DIR}/iPlug2.cmake)
find_package(iPlug2 REQUIRED)

include_directories(src)
enable_testing()
add_executable(keith-synth-tests tools/synth_offline_test.cpp)
target_compile_features(keith-synth-tests PRIVATE cxx_std_17)
add_test(NAME synth-offline COMMAND keith-synth-tests)
add_executable(keith-editor-tests tools/editor_geometry_test.cpp)
target_compile_features(keith-editor-tests PRIVATE cxx_std_17)
add_test(NAME editor-geometry COMMAND keith-editor-tests)

# Ship an editor: hosts that get a UI-less VST3 show an empty wrapper, and the
# standalone target needs a window to be usable at all.
if(APPLE)
  set(KEITH_FORMATS VST3 AU APP)
else()
  set(KEITH_FORMATS VST3 APP)
endif()

iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    src/Keith.cpp
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
    resources/images/workspace-source.png
    resources/images/workspace-pitch.png
    resources/images/workspace-tape.png
    resources/fonts/Bangers-Regular.ttf
    resources/fonts/Oswald-Bold.ttf
    resources/fonts/SpaceMono-Regular.ttf
    resources/fonts/SpaceMono-Bold.ttf
  UI IGRAPHICS
    FORMATS ${KEITH_FORMATS}
  LINK
    iPlug2::Extras::Synth
  EXCLUDE_FORMATS CLAP AAX AUV3 WAM WASM_DSP WASM_UI VST2
)

# Windows resource handling for the editor faces (docs/UI-DESIGN.md §3).
#
# The standalone target compiles resources/main.rc (iPlug2 adds it for APP), which embeds the
# OFL TTFs into the .exe. The VST3 target gets no .rc from iPlug2, so on Windows its fonts
# resolve only from the working directory — embed the same .rc there too so the VST3 carries its
# own faces wherever a host loads it from.
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/main.rc")
  set(KEITH_FONT_RESOURCES
    fonts/Bangers-Regular.ttf
    fonts/Oswald-Bold.ttf
    fonts/SpaceMono-Regular.ttf
    fonts/SpaceMono-Bold.ttf)

  if(TARGET Keith-vst3)
    target_sources(Keith-vst3 PRIVATE resources/main.rc)
    set_source_files_properties(resources/main.rc PROPERTIES
      COMPILE_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts\" /I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"")
  endif()
endif()

# iPlug2's CMake does not copy the RESOURCES list next to the Windows binaries; do it here so the
# VST3 bundle and the standalone both ship their faces. (The .exe and the .vst3 also embed them.)
if(WIN32 AND TARGET Keith-vst3)
  add_custom_command(TARGET Keith-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/OFL.txt"
      "${CMAKE_BINARY_DIR}/out/Keith.vst3/Contents/Resources/OFL.txt"
    COMMENT "Copying Oswald OFL notice into Keith.vst3")
  foreach(_font IN LISTS KEITH_FONT_RESOURCES)
    get_filename_component(_font_name "${_font}" NAME)
    add_custom_command(TARGET Keith-vst3 POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/${_font}"
        "${CMAKE_BINARY_DIR}/out/Keith.vst3/Contents/Resources/${_font_name}"
      COMMENT "Copying editor font ${_font_name} into Keith.vst3")
  endforeach()
endif()

if(WIN32 AND TARGET Keith-app)
  add_custom_command(TARGET Keith-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/OFL.txt"
      "${CMAKE_BINARY_DIR}/out/OFL.txt"
    COMMENT "Copying Oswald OFL notice next to Keith.exe")
  foreach(_font IN LISTS KEITH_FONT_RESOURCES)
    get_filename_component(_font_name "${_font}" NAME)
    add_custom_command(TARGET Keith-app POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/${_font}"
        "${CMAKE_BINARY_DIR}/out/${_font_name}"
      COMMENT "Copying editor font ${_font_name} next to Keith.exe")
  endforeach()
endif()
