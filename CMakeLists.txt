cmake_minimum_required(VERSION 3.5)
project(cppbase)

# enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX ../${CMAKE_BUILD_TYPE}-install)
endif()

option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# only build examples if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL "cppbase")
  option(CPPBASE_BUILD_EXAMPLES "build cppbse examples" ON)
endif()

set(CMAKE_CXX_STANDARD 20)

# g++ cpp_flags
# set(CXX_FLAGS
#  -g
#  -DVALGRIND
#  -DCHECK_PTHREAD_RETURN_VALUE
#  -D_FILE_OFFSET_BITS=64
# -Wall
# -Wextra
# -Werror
# -Wconversion
# -Wno-unused-parameter
# -Wold-style-cast
# -Woverloaded-virtual
# -Wpointer-arith
# -Wshadow
# -Wwrite-strings
# -march=native
# -MMD
# -std=c++2a
# -rdynamic
# )

# clang cpp_flags
# if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#   list(APPEND CXX_FLAGS "-Wno-null-dereference")
#   list(APPEND CXX_FLAGS "-Wno-sign-conversion")
#   list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
#   list(APPEND CXX_FLAGS "-Wthread-safety")
#   list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
# endif()
# string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_VERBOSE_MAKEFILE off)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/base)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog/CMakeLists.txt)
    message(FATAL_ERROR "spdlog submodule not found. Run: git submodule update --init")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/CMakeLists.txt)
    message(FATAL_ERROR "googletest submodule not found. Run: git submodule update --init")
endif()

set(BOOST_ROOT "C:/local/boost_1_90_0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(deps/spdlog)
add_subdirectory(deps/googletest)
add_subdirectory(base)
add_subdirectory(test)
