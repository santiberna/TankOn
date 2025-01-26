
# Changes the directory where the target is built
function(target_output_directory target directory)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${directory}
        ARCHIVE_OUTPUT_DIRECTORY ${directory}
    )
endfunction()