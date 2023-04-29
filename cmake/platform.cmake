set(IDE_GENERATOR OFF)
if(CMAKE_GENERATOR MATCHES "^Visual Studio" OR CMAKE_GENERATOR STREQUAL "Xcode")
  set(IDE_GENERATOR ON)
endif()

function(PushToDevice target)
  if (NOT ANDROID)
    return()
  endif()
  if (NOT TARGET push)
    add_custom_target(push)
    set_target_properties(push PROPERTIES FOLDER "utility")
  endif()

  cmake_parse_arguments(PushToDevice "BIN" "FOLDER" "FILES" ${ARGN})
  set(DEST_FOLDER /data/local/tmp)
  if (PushToDevice_FOLDER)
    set(DEST_FOLDER ${DEST_FOLDER}/${PushToDevice_FOLDER})
  elseif (NOT PushToDevice_BIN)
    set(DEST_FOLDER /data/local/tmp/libs)
  endif()

  set(EXTRA_OPTIONS)
  if (PushToDevice_FILES)
    set(EXTRA_OPTIONS DEPENDS ${PushToDevice_FILES})
  else()
    set(PushToDevice_FILES $<TARGET_FILE:${target}>)
    if (PushToDevice_BIN)
      set(EXTRA_OPTIONS COMMAND adb shell chmod u+x ${DEST_FOLDER}/$<TARGET_FILE_NAME:${target}>)
    endif()
    set(EXTRA_OPTIONS ${EXTRA_OPTIONS} DEPENDS ${target})
  endif()

  add_custom_target(push_${target}
    COMMAND adb shell mkdir -p ${DEST_FOLDER}
    COMMAND adb push --sync ${PushToDevice_FILES} ${DEST_FOLDER}
    ${EXTRA_OPTIONS}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Push ${target} target to ${DEST_FOLDER} on device"
    COMMAND_EXPAND_LISTS USES_TERMINAL VERBATIM
  )
  add_dependencies(push push_${target})
  set_target_properties(push_${target} PROPERTIES FOLDER "utility")
endfunction()

function(AddRunTarget target)
  if (IDE_GENERATOR AND NOT CMAKE_CROSSCOMPILING)
    return()
  endif()
  if (NOT TARGET run_all)
    add_custom_target(run_all)
  endif()
  cmake_parse_arguments(AddRunTarget "" "" "ARGS" ${ARGN})
  if (ANDROID)
    if (NOT TARGET push_${target})
      PushToDevice(${target} ${AddRunTarget_UNPARSED_ARGUMENTS})
    endif()
    cmake_parse_arguments(AddRunTarget_Push "" "FOLDER" "" ${AddRunTarget_UNPARSED_ARGUMENTS})
    if (AddRunTarget_Push_FOLDER)
      set(EXE_NAME ${AddRunTarget_Push_FOLDER}/$<TARGET_FILE_NAME:${target}>)
    else()
      set(EXE_NAME $<TARGET_FILE_NAME:${target}>)
    endif()
    set(EXE_NAME adb shell LD_LIBRARY_PATH=/data/local/tmp/libs /data/local/tmp/${EXE_NAME})
    set(DEP_NAME push_${target})
  else()
    set(EXE_NAME ${target})
    set(DEP_NAME ${target})
  endif()
  add_custom_target(run_${target}
    COMMAND ${EXE_NAME} ${AddRunTarget_ARGS}
    DEPENDS ${DEP_NAME}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Run target ${target}"
    COMMAND_EXPAND_LISTS USES_TERMINAL VERBATIM
  )
  add_dependencies(run_all run_${target})
endfunction()
