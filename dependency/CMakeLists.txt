# Set option for sub project
macro(SetOption name value)
  set(${name} ${value} CACHE INTERNAL "" FORCE)
endmacro()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dependency)

# disable all warning for dependency target
if (MSVC)
  # NOTE: don't disable all warnings for c, as libjpeg-turbo explicitly add /W3 warning
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4819")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

# NOTE: sort dependency in alphabat order unless dependency has include orders

##### CLI11 #####
if (BUILD_TOOL)
  add_subdirectory(CLI11 EXCLUDE_FROM_ALL)
  if (ANDROID)
    # don't use c++17 filesystem for android
    target_compile_definitions(CLI11 INTERFACE -DCLI11_HAS_FILESYSTEM=0)
  endif()
endif()

##### spdlog #####
SetOption(SPDLOG_NO_ATOMIC_LEVELS ON)
SetOption(SPDLOG_DISABLE_DEFAULT_LOGGER ON)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
set_target_properties(spdlog PROPERTIES FOLDER "dependency")

##### ghc_filesystem #####
if (ANDROID)
  SetOption(GHC_FILESYSTEM_WITH_INSTALL OFF)
  add_subdirectory(ghc_filesystem EXCLUDE_FROM_ALL)
  target_compile_definitions(ghc_filesystem INTERFACE -DGHC_FS_API= -DGHC_FS_API_CLASS=)
endif()

##### Catch2 #####
if (BUILD_TEST)
  SetOption(CATCH_CONFIG_CPP11_TO_STRING ON)
  add_subdirectory(catch2 EXCLUDE_FROM_ALL)
  set_target_properties(Catch2 PROPERTIES FOLDER "dependency")
  set_target_properties(Catch2WithMain PROPERTIES FOLDER "dependency")
endif()

