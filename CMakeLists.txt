option(ENABLE_ASAN "Build with AddressSanitizer" OFF)
cmake_minimum_required(VERSION 3.16)
project(aoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
# endif()

set(SOURCE_DIR "src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${SOURCE_DIR}/*.cpp
    ${SOURCE_DIR}/*/*.cpp
)

#target_compile_options(aoc PRIVATE -Wall -Wextra -Wpedantic)

if (ENABLE_ASAN)
    message(STATUS "ASan enabled")
    #target_compile_options(aoc PRIVATE -g -fsanitize=address)
    add_compile_options(-g -fsanitize=address)
    #target_link_options(aoc PRIVATE -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# IMPORTANT: This needs to be AFTER setting the compile options
add_executable(aoc ${SOURCES})
# Add files from "include/"
target_include_directories(aoc PRIVATE ${CMAKE_SOURCE_DIR}/include)