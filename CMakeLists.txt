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
    resources/resource.h
  RESOURCES
    resources/fonts/Roboto-Regular.ttf
  UI IGRAPHICS
  FORMATS ${KEITH_FORMATS}
  LINK
    iPlug2::Extras::Synth
  EXCLUDE_FORMATS CLAP AAX AUV3 WAM WASM_DSP WASM_UI VST2
)

# On Windows iPlug2 resolves resources at <bundle>/Contents/Resources (VST3)
# and next to the executable (APP), but its CMake does not copy the RESOURCES
# list there. Do it here, or the editor ships without its font.
if(WIN32 AND TARGET Keith-vst3)
  add_custom_command(TARGET Keith-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/Roboto-Regular.ttf"
      "${CMAKE_BINARY_DIR}/out/Keith.vst3/Contents/Resources/Roboto-Regular.ttf"
    COMMENT "Copying editor resources into Keith.vst3")
endif()

if(WIN32 AND TARGET Keith-app)
  add_custom_command(TARGET Keith-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/Roboto-Regular.ttf"
      "${CMAKE_BINARY_DIR}/out/Roboto-Regular.ttf"
    COMMENT "Copying editor resources next to Keith.exe")
endif()
