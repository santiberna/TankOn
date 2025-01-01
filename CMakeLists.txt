# PROJECT

cmake_minimum_required(VERSION 3.15)
project(NetworkingDemo)

# INCLUDES
include(FetchContent)
include(cmake/utility.cmake)

# OPTIONS

option(ENABLE_TESTS "Compile Unit Tests" OFF)

# MAIN EXECUTABLE

add_subdirectory(third-party)
add_subdirectory(src)

