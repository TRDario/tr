function(tr_write_git_hash GIT_HASH)
    file(WRITE ${CMAKE_BINARY_DIR}/git-state.txt ${GIT_HASH})
endfunction()

function(tr_read_git_hash GIT_HASH)
    if (EXISTS ${CMAKE_BINARY_DIR}/git-state.txt)
        file(STRINGS ${CMAKE_BINARY_DIR}/git-state.txt CONTENT)
        LIST(GET CONTENT 0 var)

        set(${GIT_HASH} ${var} PARENT_SCOPE)
    endif()
endfunction()

function(tr_check_git_hash)
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    tr_read_git_hash(GIT_HASH_CACHE)
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/git_hash)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/git_hash)
        file(WRITE ${CMAKE_BINARY_DIR}/git_hash/git_hash.hpp.in "#pragma once\n#define GIT_HASH \"@GIT_HASH@\"")
    endif()

    if (NOT DEFINED GIT_HASH_CACHE)
        set(GIT_HASH_CACHE "INVALID")
    endif()

    if (NOT ${GIT_HASH} STREQUAL ${GIT_HASH_CACHE} OR NOT EXISTS ${CMAKE_BINARY_DIR}/git_hash/git_hash.hpp)
        tr_write_git_hash(${GIT_HASH})
        configure_file(${CMAKE_BINARY_DIR}/git_hash/git_hash.hpp.in ${CMAKE_BINARY_DIR}/git_hash/git_hash.hpp @ONLY)
    endif()
endfunction()

function(tr_git_hash_setup)
    add_custom_target(always_check_git COMMAND ${CMAKE_COMMAND}
        -DRUN_CHECK_GIT_HASH=1
        -DGIT_HASH_CACHE=${GIT_HASH_CACHE}
        -P ${tr_SOURCE_DIR}/git_hash.cmake
        BYPRODUCTS ${CMAKE_BINARY_DIR}/git_hash/git_hash.hpp
        COMMENT "Checking git hash for changes"
    )

    add_library(git_hash INTERFACE)
    target_include_directories(git_hash INTERFACE ${CMAKE_BINARY_DIR}/git_hash)
    add_dependencies(git_hash always_check_git)

    tr_check_git_hash()
endfunction()

if (RUN_CHECK_GIT_HASH)
    tr_check_git_hash()
endif ()