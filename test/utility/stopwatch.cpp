///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/stopwatch.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/stopwatch.hpp>

constexpr std::chrono::milliseconds interval{100};

TEST(stopwatch_test, elapsed)
{
	tr::stopwatch stopwatch;
	for (int i = 0; i < 20; ++i) {
		std::this_thread::sleep_for(interval);
		EXPECT_GE(stopwatch.elapsed(), interval * (i + 1));
	}
}

TEST(stopwatch_test, lap)
{
	tr::stopwatch stopwatch;
	for (int i = 0; i < 20; ++i) {
		std::this_thread::sleep_for(interval);
		EXPECT_GE(stopwatch.lap(), interval);
	}
}