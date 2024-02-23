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

## Add suffixient
FetchContent_Declare(
  suffixient
  GIT_REPOSITORY https://github.com/regindex/suffixient
  GIT_TAG main
)
if(NOT suffixient_POPULATED)
  FetchContent_Populate(suffixient)
  add_subdirectory(${suffixient_SOURCE_DIR} ${suffixient_BINARY_DIR})
endif()
