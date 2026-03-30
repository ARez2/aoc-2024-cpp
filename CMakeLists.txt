cmake_minimum_required(VERSION 3.16)
project(aoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(SOURCE_DIR "src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*/*.cpp
)

add_executable(aoc ${SOURCES})
target_compile_options(aoc PRIVATE -Wall -Wextra -Wpedantic)
