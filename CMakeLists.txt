# 3.21.0 for VS2022 support
cmake_minimum_required(VERSION 3.21.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

macro(SetPolicy number value)
  if (POLICY CMP${number})
    cmake_policy(SET CMP${number} ${value})
    # set default policy for subproject
    set(CMAKE_POLICY_DEFAULT_CMP${number} ${value})
  endif()
endmacro()

include(setup)
SetPolicy(0092 NEW)

project(juglans CXX)

include(CMakeDependentOption)

include(common)
include(platform)
include(compiler)
include(clangformat)
include(target)
include(summary)

if (ANDROID)
  set(JUGLANS_SHARED_STL ${JUGLANS_ANDROID_NDK}/sources/cxx-stl/llvm-libc++/libs/arm64-v8a/libc++_shared.so)
  PushToDevice(android_stl FILES ${JUGLANS_SHARED_STL})
endif()

option(BUILD_TOOL "Enable tools" ${JUGLANS_IS_TOP_LEVEL})
option(BUILD_TEST "Enable tests" ${JUGLANS_IS_TOP_LEVEL})
option(ENABLE_OPTIMIZATION "Enable optimization, useful for final products" OFF)

cmake_dependent_option(USE_QXRCORE "Use qxr core to get vr camera stream" OFF "ANDROID" OFF)

option(INTERFACE_UNITY "Build unity test interface" ON)

add_subdirectory(dependency)
SetJuglansCompileFlags()
add_subdirectory(modules)
add_subdirectory(tools)

ShowSummary()
