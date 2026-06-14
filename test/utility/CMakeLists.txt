enable_testing()

add_executable(
	utility_test
	binary_io.cpp
	encryption.cpp
	math.cpp
	mstream.cpp
	norm_cast.cpp
	optional.cpp
	stopwatch.cpp
	utf8.cpp
	zstring_view.cpp
)
target_link_libraries(
	utility_test
	tr::utility
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utility_test)