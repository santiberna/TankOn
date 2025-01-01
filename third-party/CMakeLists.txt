add_library(ThirdParty INTERFACE)

FetchContent_Declare(
    SDL_lib
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG preview-3.1.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL_lib)
target_output_directory(SDL3-shared "${CMAKE_BINARY_DIR}")

# GLM

FetchContent_Declare(
    GLM_lib
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(GLM_lib)

# MagicEnum

FetchContent_Declare(
    MagicEnum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.6
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(MagicEnum)

# Boost

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if (WIN32)
    set(BOOST_ROOT  "C:/Program Files/Boost/boost_1_86_0")
    target_compile_definitions(ThirdParty INTERFACE _WIN32_WINDOWS)
endif ()

find_package(Boost 1.50 REQUIRED COMPONENTS)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost library dir: ${Boost_LIBRARY_DIRS}")

# Cereal

FetchContent_Declare(
    Cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.2
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
set(SKIP_PERFORMANCE_COMPARISON ON)

FetchContent_MakeAvailable(Cereal)

# LINK DEPENDENCIES

target_include_directories(ThirdParty
    INTERFACE ${Boost_INCLUDE_DIRS}
)

target_link_directories(ThirdParty
    INTERFACE ${Boost_LIBRARY_DIRS}
)

target_link_libraries(ThirdParty 
    #INTERFACE SDL3-shared
    INTERFACE glm::glm
    INTERFACE magic_enum::magic_enum
    #INTERFACE cereal::cereal
    ${Boost_LIBRARIES}
    )