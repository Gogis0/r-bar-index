include(FetchContent)

## Add BigRepair
FetchContent_Declare(
  bigrepair
  GIT_REPOSITORY https://gitlab.com/manzai/bigrepair
)
if(NOT bigrepair_POPULATED)
  FetchContent_Populate(bigrepair)
  # add_subdirectory(${bigrepair_SOURCE_DIR} ${bigrepair_BINARY_DIR})

  execute_process(COMMAND make
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${bigrepair_SOURCE_DIR} )
  if(result)
      message(FATAL_ERROR "CMake step for bigrepair failed: ${result}")
  endif()
  
endif()

## Add ShapedSlp
FetchContent_Declare(
  shaped_slp
  GIT_REPOSITORY https://github.com/Gogis0/ShapedSlp
  GIT_TAG lcp
)
if(NOT shaped_slp_POPULATED)
  FetchContent_Populate(shaped_slp)
  add_subdirectory(${shaped_slp_SOURCE_DIR} ${shaped_slp_BINARY_DIR})
endif()
