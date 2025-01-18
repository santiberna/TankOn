# MAIN PROJECT

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC ThirdParty)

target_compile_options(${PROJECT_NAME} 
    PUBLIC -Wall 
    PUBLIC -Wextra)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} 
    PUBLIC cxx_std_20)

file(GLOB_RECURSE source CONFIGURE_DEPENDS
    "game/*.cpp" "game/*.hpp"
    "math/*.cpp" "math*.hpp"
    "network/*.cpp" "network/*.hpp"
    "resources/*.cpp" "resources/*.hpp"
    "utility/*.cpp" "utility/*.hpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${source})

add_executable(${PROJECT_NAME}Game "main.cpp")
target_link_libraries(${PROJECT_NAME}Game PRIVATE ${PROJECT_NAME})
target_output_directory(${PROJECT_NAME}Game ${CMAKE_BINARY_DIR})