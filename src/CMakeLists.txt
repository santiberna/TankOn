# MAIN PROJECT

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ThirdParty)

target_compile_options(${PROJECT_NAME} 
    PRIVATE -Wall 
    PRIVATE -Wextra)

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} 
    PRIVATE cxx_std_20)

file(GLOB_RECURSE source CONFIGURE_DEPENDS "*.cpp" "*.hpp")
target_sources(${PROJECT_NAME} PRIVATE ${source})

target_output_directory(${PROJECT_NAME} ${CMAKE_BINARY_DIR})