# Deployment floors: macOS 11.7.10 (Big Sur), Windows 10 22H2.
# Include this before iPlug2.cmake.

if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "11.7.10" CACHE STRING "Minimum macOS version" FORCE)
  set(IPLUG2_UNIVERSAL ON CACHE BOOL "Build universal arm64+x86_64 binaries" FORCE)
endif()

if(WIN32)
  # NTDDI_WIN10_22H2 — Windows 10 22H2 minimum
  add_compile_definitions(
    _WIN32_WINNT=0x0A00
    WINVER=0x0A00
    NTDDI_VERSION=0x0A00000C
  )
endif()
