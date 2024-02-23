cmake_minimum_required(VERSION 2.6)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_FLAGS "--std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fstrict-aliasing -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -ggdb -Ofast -fstrict-aliasing -march=native")
#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.. )

add_subdirectory(thirdparty)

project (rho-index)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(~/include)  #SDSL headers
LINK_DIRECTORIES(~/lib)         #SDSL libs

message("Building in ${CMAKE_BUILD_TYPE} mode")

find_library(SDSL_LIB sdsl)
#add_executable(lcp_test test.c grammar.c)
#TARGET_LINK_LIBRARIES(lcp_test sdsl criterion)


add_executable(build_index hashing.c build_index.c)
