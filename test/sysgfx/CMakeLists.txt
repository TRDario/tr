add_executable(
	sysgfx_test
	file_dialog.cpp
    logger.cpp
)
tr_target_template(sysgfx_test)
target_link_libraries(sysgfx_test PUBLIC GTest::gtest_main)
gtest_discover_tests(sysgfx_test)