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

set(KEITH_DEFINES NO_IGRAPHICS)

if(APPLE)
  set(KEITH_FORMATS VST3 AU)
else()
  set(KEITH_FORMATS VST3)
endif()

iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    src/Keith.cpp
    src/Keith.h
    src/Keith_DSP.h
    resources/resource.h
  UI NONE
  FORMATS ${KEITH_FORMATS}
  LINK
    iPlug2::Extras::Synth
  DEFINES ${KEITH_DEFINES}
  EXCLUDE_FORMATS APP CLAP AAX AUV3 WAM WASM_DSP WASM_UI VST2
)
