add_library(ThirdParty INTERFACE)

# SDL

FetchContent_Declare(
    SDL_lib
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG preview-3.1.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL_lib)
target_output_directory(SDL3-shared "${CMAKE_BINARY_DIR}")

# SDL_Mixer

FetchContent_Declare(
    SDL_mixer_lib
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main 
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL_mixer_lib)
target_output_directory(SDL3_mixer-shared "${CMAKE_BINARY_DIR}")

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
    set(BOOST_ROOT "C:/Program Files/Boost/boost_1_86_0")
    target_compile_definitions(ThirdParty 
        INTERFACE _WIN32_WINNT=0x0602)
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

# ImGui

file(GLOB_RECURSE imgui_source CONFIGURE_DEPENDS 
    "imgui/imgui.h" "imgui/*.h"
    "imgui/backends/*.cpp" "imgui/backends/*.h"
    "imgui/misc/*.cpp" "imgui/misc/.h"
    )

add_library(ImGui STATIC)
target_include_directories(ImGui PUBLIC "imgui")
target_link_libraries(ImGui PRIVATE SDL3-shared)

target_sources(ImGui PRIVATE 
    "imgui/imgui.cpp" 
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "imgui/backends/imgui_impl_sdl3.cpp"
    "imgui/backends/imgui_impl_sdlrenderer3.cpp"
    )

# STB

add_subdirectory(stb)

# LINK DEPENDENCIES

target_include_directories(ThirdParty
    INTERFACE ${Boost_INCLUDE_DIRS}
)

target_link_directories(ThirdParty
    INTERFACE ${Boost_LIBRARY_DIRS}
)

target_link_libraries(ThirdParty 
    INTERFACE SDL3-shared
    INTERFACE SDL3_mixer-shared 
    INTERFACE glm::glm
    INTERFACE cereal::cereal
    INTERFACE ImGui
    INTERFACE STB
    ${Boost_LIBRARIES}
    )