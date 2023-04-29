option(USE_CLANG_FORMAT "Enable clang format" ${JUGLANS_IS_TOP_LEVEL})

# find and check clang-format is 14.0.5
# add NO_CMAKE_PATH so we don't select clang-format in android ndk, which is too old
find_program(CLANG_FORMAT_BINARY NAMES clang-format-14 clang-format REQUIRED NO_CMAKE_PATH)
if (NOT CLANG_FORMAT_14_BINARY)
  execute_process(
    COMMAND ${CLANG_FORMAT_BINARY} --version
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE CLANG_FORMAT_VERSION_OUTPUT
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
  )
  string(FIND ${CLANG_FORMAT_VERSION_OUTPUT} "14.0" CLANG_FORMAT_14_VERSION_CHECK)
  if (CLANG_FORMAT_14_VERSION_CHECK LESS 0)
    message(FATAL_ERROR "Expected ${CLANG_FORMAT_BINARY} version to be 14.0.5, got ${CLANG_FORMAT_VERSION_OUTPUT}")
  endif()
  set(CLANG_FORMAT_14_BINARY ${CLANG_FORMAT_BINARY} CACHE INTERNAL "verified clang-format 14.0.5 path" FORCE)
endif()

add_custom_target(clangformat)
set_target_properties(clangformat PROPERTIES FOLDER "utility")

function(AddFormatTarget target)
  if (NOT CLANG_FORMAT_14_BINARY)
    return()
  endif()

  set(FILES)
  foreach(file ${ARGN})
    if (file MATCHES "\\.(cpp|c|cc|cs|cu|hpp|h|java|proto)$")
      list(APPEND FILES "${file}")
    endif()
  endforeach()
  if (NOT FILES)
    return()
  endif()

  add_custom_target(clangformat_${target}
    COMMAND ${CLANG_FORMAT_14_BINARY} --Werror -i ${FILES}
    DEPENDS ${CLANG_FORMAT_14_BINARY}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Format files in ${target}"
    COMMAND_EXPAND_LISTS USES_TERMINAL VERBATIM
  )
  add_dependencies(clangformat clangformat_${target})
  set_target_properties(clangformat_${target} PROPERTIES FOLDER "utility")
endfunction()
