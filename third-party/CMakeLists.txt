add_library(ThirdParty INTERFACE)
set(DEPENDENCY_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}")

# ALL DLLS

file(GLOB_RECURSE dlls CONFIGURE_DEPENDS "${DEPENDENCY_LIBRARY_PATH}/*.dll")

add_custom_target(DLL_LIBRARIES ALL
    COMMENT "Copying DLLs to Output"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${DEPENDENCY_LIBRARY_PATH}/SDL3.dll" 
        "${CMAKE_BINARY_DIR}"
)

# SDL

add_library(SDL3 INTERFACE)
target_include_directories(SDL3 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL")

target_link_libraries(SDL3 
    INTERFACE "${DEPENDENCY_LIBRARY_PATH}/SDL3.lib"
)

add_dependencies(SDL3 DLL_LIBRARIES)

if (BUILD_DEPENDENCIES)
    
    FetchContent_Declare(
        SDL_lib
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG preview-3.1.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )

    FetchContent_MakeAvailable(SDL_lib)

    target_output_directory(SDL3-shared "${DEPENDENCY_LIBRARY_PATH}")

    add_custom_target(COPY_INCLUDES ALL
        COMMENT "Copying include folders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
            "${sdl_lib_SOURCE_DIR}/include" 
            "${CMAKE_CURRENT_SOURCE_DIR}/SDL"
    )

    add_dependencies(COPY_INCLUDES SDL3-shared)
    add_dependencies(DLL_LIBRARIES SDL3-shared)
    add_dependencies(SDL3 COPY_INCLUDES)

endif()

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

set(BOOST_ROOT  "C:/Program Files/Boost/boost_1_86_0")

find_package(Boost 1.50 REQUIRED COMPONENTS)

target_compile_definitions(ThirdParty INTERFACE _WIN32_WINDOWS)
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
    INTERFACE SDL3
    INTERFACE glm::glm
    INTERFACE magic_enum::magic_enum
    #INTERFACE cereal::cereal
    ${Boost_LIBRARIES}
    )