###########################################################################################################################################
##                                                                                                                                       ##
## Provides functions for generating headers for embedding data within C++ source files.                                                 ##
##                                                                                                                                       ##
###########################################################################################################################################

add_executable(cpp_embed_string tools/cpp_embed_string.c)
add_executable(cpp_embed_binary tools/cpp_embed_binary.c)

# Processes text from INPUT_FILE into an embeddable string named VARIABLE_NAME and output to OUTPUT_FILE.
function(tr_generate_embeddable_string TARGET INPUT_FILE OUTPUT_FILE VARIABLE_NAME)
    set(RELATIVE_OUTPUT_PATH ${OUTPUT_FILE})
    cmake_path(ABSOLUTE_PATH INPUT_FILE BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    cmake_path(ABSOLUTE_PATH OUTPUT_FILE BASE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND $<TARGET_FILE:cpp_embed_string> "${INPUT_FILE}" "${OUTPUT_FILE}" "${VARIABLE_NAME}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEPENDS cpp_embed_string "${INPUT_FILE}"
        COMMENT "Generating embedded string header ${RELATIVE_OUTPUT_PATH}"
        VERBATIM
    )
    target_sources(${TARGET} PRIVATE "${OUTPUT_FILE}")
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

# Processes binary data from INPUT_FILE into an embeddable array named VARIABLE_NAME and output to OUTPUT_FILE.
function(tr_generate_embeddable_binary TARGET INPUT_FILE OUTPUT_FILE VARIABLE_NAME)
    set(RELATIVE_OUTPUT_PATH ${OUTPUT_FILE})
    cmake_path(ABSOLUTE_PATH INPUT_FILE BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    cmake_path(ABSOLUTE_PATH OUTPUT_FILE BASE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND $<TARGET_FILE:cpp_embed_binary> "${INPUT_FILE}" "${OUTPUT_FILE}" "${VARIABLE_NAME}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEPENDS cpp_embed_binary "${INPUT_FILE}"
        COMMENT "Generating embedded binary header ${RELATIVE_OUTPUT_PATH}"
        VERBATIM
    )
    target_sources(${TARGET} PRIVATE "${OUTPUT_FILE}")
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endfunction()