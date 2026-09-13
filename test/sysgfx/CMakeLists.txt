add_executable(
	sysgfx_test
    logger.cpp
)
target_link_libraries(
	sysgfx_test
	tr::sysgfx
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sysgfx_test)