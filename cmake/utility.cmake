
# Changes the directory where the target is built
function(target_output_directory target directory)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${directory}
        ARCHIVE_OUTPUT_DIRECTORY ${directory}
    )
endfunction()

    
function(target_copy_directory custom target source_directory destination_directory)
add_custom_target(${custom} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_directory} ${destination_directory}
    DEPENDS ${target})
endfunction()
