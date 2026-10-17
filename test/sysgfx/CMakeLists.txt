add_executable(
	sysgfx_test
	file_dialog.cpp
    logger.cpp
)
target_link_libraries(
	sysgfx_test
	tr::sysgfx
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sysgfx_test)