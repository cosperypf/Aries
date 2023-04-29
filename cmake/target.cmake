macro(AddModuleSourceFromSubDir dir module list_suffix output_suffix)
  if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${dir}/CMakeLists.txt")
    set(${module}_${list_suffix})
    add_subdirectory(${dir})
    list(APPEND ${module}_${output_suffix} "${CMAKE_CURRENT_LIST_DIR}/${dir}/CMakeLists.txt")
    foreach(file ${${module}_${list_suffix}})
      list(APPEND ${module}_${output_suffix} "${CMAKE_CURRENT_LIST_DIR}/${dir}/${file}")
    endforeach()
  endif()
endmacro()

macro(LinkDependency target type dep)
  if (TARGET ${dep})
    target_link_libraries(${target} ${type} ${dep})
    if(dep MATCHES "^opencv_")
      # fix for opencv modules not exporting include dir
      target_include_directories(${target} ${type} SYSTEM "${OPENCV_MODULE_${dep}_LOCATION}/include" "${CMAKE_BINARY_DIR}")
      foreach (nested ${OPENCV_MODULE_${dep}_DEPS})
        target_include_directories(${target} ${type} SYSTEM "${OPENCV_MODULE_${nested}_LOCATION}/include")
      endforeach()
    endif()
  endif()
endmacro()

function(AddModule dir)
  string(TOUPPER ${dir} module)
  set(target juglans_${dir})
  option(JUGLANS_BUILD_${module} "Enable ${dir} module" ON)
  set(MODULE_ENABLED ON)
  if (NOT ${JUGLANS_BUILD_${module}})
    message(STATUS "Disabled ${dir} module")
    set(MODULE_ENABLED OFF)
  endif()

  cmake_parse_arguments(AddModule "" "SHARED" "DEPENDS;OPTIONAL;PRIVATE;PRIVATE_OPTIONAL" ${ARGN})
  foreach(dep ${AddModule_DEPENDS} ${AddModule_PRIVATE})
    if (${MODULE_ENABLED} AND NOT TARGET ${dep})
      message(STATUS "Disabling ${dir} module due to missing ${dep} target")
      set(MODULE_ENABLED OFF)
    endif()
  endforeach()
  set_property(GLOBAL APPEND PROPERTY JUGLANS_MODULE_LIST ${dir})

  #################### library target ####################
  set(${module}_LIBRARY_SOURCES "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
  if (AddModule_SHARED)
    AddModuleSourceFromSubDir(include ${module} HEADERS LIBRARY_SOURCES)
  else()
    AddModuleSourceFromSubDir(include/juglans/${dir} ${module} HEADERS LIBRARY_SOURCES)
  endif()
  AddModuleSourceFromSubDir(internal ${module} INTERNAL_HEADERS LIBRARY_SOURCES)
  AddModuleSourceFromSubDir(src ${module} SOURCES LIBRARY_SOURCES)

  if (MODULE_ENABLED)
    if (AddModule_SHARED)
      add_library(${target} SHARED ${${module}_LIBRARY_SOURCES})
      set_target_properties(${target} PROPERTIES OUTPUT_NAME ${AddModule_SHARED})
      if (ENABLE_OPTIMIZATION AND CMAKE_STRIP)
        add_custom_command(TARGET ${target} POST_BUILD
          COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${target}>
          COMMENT "Strip ${target} binary"
        )
      endif()
    else()
      add_library(${target} STATIC ${${module}_LIBRARY_SOURCES})
    endif()
    set_target_properties(${target} PROPERTIES FOLDER "modules")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/internal")
    foreach(dep ${AddModule_DEPENDS} ${AddModule_OPTIONAL})
      LinkDependency(${target} PUBLIC ${dep})
    endforeach()
    foreach(dep ${AddModule_PRIVATE} ${AddModule_PRIVATE_OPTIONAL})
      LinkDependency(${target} PRIVATE ${dep})
    endforeach()
  endif()

  #################### unit test target ####################
  set(${module}_UNITTEST_SOURCES)
  AddModuleSourceFromSubDir(unit ${module} UNIT_SOURCES UNITTEST_SOURCES)
  if (MODULE_ENABLED AND BUILD_TEST AND ${module}_UNITTEST_SOURCES)
    add_executable(${dir}_test ${${module}_UNITTEST_SOURCES})
    set_target_properties(${dir}_test PROPERTIES FOLDER "tests")

    target_include_directories(${dir}_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}/internal")
    target_link_libraries(${dir}_test PRIVATE ${target} Catch2::Catch2WithMain)
    foreach(dep ${AddModule_PRIVATE} ${AddModule_PRIVATE_OPTIONAL})
      LinkDependency(${dir}_test PRIVATE ${dep})
    endforeach()
    # BIN & FOLDER args for push target on android
    AddRunTarget(${dir}_test BIN FOLDER juglans_test ARGS --allow-running-no-tests)
  endif()

  #################### misc ####################
  AddFormatTarget(${dir} ${${module}_LIBRARY_SOURCES} ${${module}_UNITTEST_SOURCES})
  source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${${module}_LIBRARY_SOURCES} ${${module}_UNITTEST_SOURCES})
endfunction()

function(AddTool dir)
  string(TOUPPER ${dir} module)
  set(TOOL_ENABLED ON)

  cmake_parse_arguments(AddTool "LIBRARY" "NAME" "DEPENDS;OPTIONAL" ${ARGN})
  set(target ${dir})
  if (AddTool_NAME)
    set(target ${AddTool_NAME})
  endif()
  foreach(dep ${AddTool_DEPENDS})
    if (NOT TARGET ${dep})
      message(STATUS "Disabling ${target} tool due to missing ${dep} target")
      set(TOOL_ENABLED OFF)
    endif()
  endforeach()
  set_property(GLOBAL APPEND PROPERTY JUGLANS_TOOL_LIST ${target})

  set(${module}_TOOL_SOURCES)
  AddModuleSourceFromSubDir(${dir} ${module} SOURCES TOOL_SOURCES)

  if (TOOL_ENABLED AND BUILD_TOOL)
    if (${AddTool_LIBRARY})
      add_library(${target} STATIC ${${module}_TOOL_SOURCES})
    else()
      add_executable(${target} ${${module}_TOOL_SOURCES})
      PushToDevice(${target} BIN)
    endif()
    set_target_properties(${target} PROPERTIES FOLDER "tools")
    target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/${dir}")
    foreach(dep ${AddTool_DEPENDS} ${AddTool_OPTIONAL})
      LinkDependency(${target} PUBLIC ${dep})
    endforeach()
  endif()

  AddFormatTarget(${dir} ${${module}_TOOL_SOURCES})
  source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${dir} FILES ${${module}_TOOL_SOURCES})
endfunction()
