set(
	TR_UTILITY_TEST_SOURCES
	binary_io.cpp
	encryption.cpp
	mstream.cpp
	norm_cast.cpp
	utf8.cpp
)
create_test_sourcelist(TR_UTILITY_TESTS tests.cpp ${TR_UTILITY_TEST_SOURCES})
add_executable(tr-utility-tests ${TR_UTILITY_TESTS})
tr_target_template(tr-utility-tests)
foreach(TEST ${TR_UTILITY_TEST_SOURCES})
	get_filename_component(TEST_NAME ${TEST} NAME_WE)
	add_test(NAME ${TEST_NAME} COMMAND tr-utility-tests ${TEST_NAME})
endforeach()