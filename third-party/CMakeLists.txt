add_library(ThirdParty INTERFACE)
set(DEPENDENCY_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}")

# SDL

add_library(SDL3 INTERFACE)
target_include_directories(SDL3 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL")
target_link_libraries(SDL3 INTERFACE "${DEPENDENCY_LIBRARY_PATH}/SDL3.lib")

add_custom_target(COPY_SDL_DLL ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DEPENDENCY_LIBRARY_PATH}/SDL3.dll" "${CMAKE_BINARY_DIR}"
    COMMENT "Copying SDL DLL to Output")

add_dependencies(SDL3 COPY_SDL_DLL)

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

    add_custom_target(COPY_SDL_INCLUDES ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${sdl_lib_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/SDL"
        COMMENT "Copying SDL include folder")

    add_dependencies(COPY_SDL_INCLUDES SDL3-shared)
    add_dependencies(COPY_SDL_DLL SDL3-shared)

endif()

# STB

add_subdirectory(stb)

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


# LINK DEPENDENCIES

target_link_libraries(ThirdParty 
    INTERFACE SDL3
    INTERFACE STB
    INTERFACE glm::glm
    INTERFACE magic_enum::magic_enum
    )