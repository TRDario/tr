###########################################################################################################################################
##                                                                                                                                       ##
## Provides functions for generating headers for embedding data within C++ source files.                                                 ##
##                                                                                                                                       ##
###########################################################################################################################################

# Processes text from INPUT_FILE into an embeddable string named VARIABLE_NAME and output to OUTPUT_FILE.
function(tr_generate_embeddable_string INPUT_FILE OUTPUT_FILE VARIABLE_NAME)
    file(READ ${INPUT_FILE} CONTENT)
    set(DELIMITER "for_c++_include")
    set(CONTENT "constexpr const char* ${VARIABLE_NAME}{R\"${DELIMITER}(\n${CONTENT})${DELIMITER}\"};")
    file(WRITE ${OUTPUT_FILE} "${CONTENT}")
endfunction()

# Processes binary data from INPUT_FILE into an embeddable array named VARIABLE_NAME and output to OUTPUT_FILE.
function(tr_generate_embeddable_binary INPUT_FILE OUTPUT_FILE VARIABLE_NAME)
    file(READ ${INPUT_FILE} HEX_CONTENT HEX)
    string(REPEAT "[0-9a-f]" 16 COLUMN_PATTERN)
    string(REGEX REPLACE "(${COLUMN_PATTERN})" "\\1\n" CONTENT "${HEX_CONTENT}")
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "std::byte{0x\\1}, " CONTENT "${CONTENT}")
    string(REGEX REPLACE ", $" "" CONTENT "${CONTENT}")
    file(WRITE "${OUTPUT_FILE}" "constexpr std::byte ${VARIABLE_NAME}[] =\n{\n${CONTENT}\n};")
endfunction()