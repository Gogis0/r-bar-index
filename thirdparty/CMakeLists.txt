cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Set the directory where external projects will be placed
set(EXTERNAL_PROJECTS_DIR ${CMAKE_BINARY_DIR}/thirdparty)

# Include the FetchContent module
include(FetchContent)

# Add BigRepair
FetchContent_Declare(
  bigrepair
  GIT_REPOSITORY https://gitlab.com/manzai/bigrepair
  SOURCE_DIR ${EXTERNAL_PROJECTS_DIR}/bigrepair
)
if(NOT bigrepair_POPULATED)
  FetchContent_Populate(bigrepair)
  execute_process(
    COMMAND make
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${bigrepair_SOURCE_DIR}
  )
  if(result)
    message(FATAL_ERROR "CMake step for bigrepair failed: ${result}")
  endif()
endif()

# Add Suffixient
FetchContent_Declare(
  suffixient
  GIT_REPOSITORY https://github.com/regindex/suffixient
  GIT_TAG main
  SOURCE_DIR ${EXTERNAL_PROJECTS_DIR}/suffixient
)
if(NOT suffixient_POPULATED)
  FetchContent_Populate(suffixient)
  add_subdirectory(${suffixient_SOURCE_DIR} ${suffixient_BINARY_DIR})
endif()

# Add ShapedSlp from the 'lcp' branch
FetchContent_Declare(
  shapedslp
  GIT_REPOSITORY https://github.com/DepuydtLore/ShapedSlp
  GIT_TAG lcp
  SOURCE_DIR ${EXTERNAL_PROJECTS_DIR}/shapedslp
)
if(NOT shapedslp_POPULATED)
  FetchContent_Populate(shapedslp)
  # Include the CMakeLists.txt from ShapedSlp
  add_subdirectory(${shapedslp_SOURCE_DIR} ${shapedslp_BINARY_DIR})
endif()


# Add your project's targets and configurations below
