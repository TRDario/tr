if(TR_USE_SYSTEM_LIBRARIES)
    find_package(GTest REQUIRED)
else()
    include(FetchContent)
    FetchContent_Declare(
        gtest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(gtest)
endif()

enable_testing()
add_subdirectory(utility)
if(TR_BUILD_AUDIO)
    add_subdirectory(audio)
endif()