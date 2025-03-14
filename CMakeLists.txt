# PROJECT

cmake_minimum_required(VERSION 3.15)
project(TankOn)

# INCLUDES
include(FetchContent)
include(cmake/utility.cmake)

# OPTIONS
set(BUILD_SHARED_LIBS OFF)
option(PROFILE "Enables Tracy as a profiler" OFF)

# MAIN EXECUTABLE

add_subdirectory(third-party)
add_subdirectory(src)

