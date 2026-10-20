/// @file
/// @brief Tests utility/stopwatch.hpp.

#include <gtest/gtest.h>
#include <tr/utility/stopwatch.hpp>

using namespace std::chrono_literals;

//

/// Interval of the stopwatch.
constexpr std::chrono::milliseconds interval{50};

//

/// Tests getting the elasped time from a stopwatch.
TEST(StopwatchTest, GetsElapsedTime)
{
	tr::stopwatch stopwatch;
	for (int i = 0; i < 3; ++i) {
		std::this_thread::sleep_for(interval);
		const auto elapsed{stopwatch.elapsed()};
		EXPECT_GE(elapsed, interval * (i + 1));
		EXPECT_LT(elapsed, interval * (i + 1) + 2s);
	}
}

/// Tests lapping a stopwatch.
TEST(StopwatchTest, Laps)
{
	tr::stopwatch stopwatch;
	std::chrono::steady_clock::duration first_lap{};
	for (int i = 0; i < 3; ++i) {
		std::this_thread::sleep_for(interval);
		const auto lap{stopwatch.lap()};
		EXPECT_GE(lap, interval);
		EXPECT_LT(lap, interval + 2s);
		if (i == 0) {
			first_lap = lap;
		}
	}
	EXPECT_LT(stopwatch.elapsed(), first_lap);
}