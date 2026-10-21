/// @file
/// @brief Tests utility/chrono.hpp.

#include <gtest/gtest.h>
#include <tr/utility/chrono.hpp>

using namespace std::chrono_literals;
using namespace tr::chrono_literals;

//

/// Tests creating durations from integral frequencies.
TEST(ChronoTest, CreatesDurationsFromIntegralFrequencies)
{
	EXPECT_EQ(1_hz, 1s);
	EXPECT_EQ(2_hz, 500ms);
	EXPECT_EQ(60_hz, tr::duration{1s} / 60);
}

/// Tests creating durations from fractional frequencies.
TEST(ChronoTest, CreatesDurationsFromFractionalFrequencies)
{
	EXPECT_EQ(2.5_hz, 400ms);
	EXPECT_EQ(0.5_hz, 2s);
}

/// Tests computing ratios between durations.
TEST(ChronoTest, ComputesDurationRatios)
{
	EXPECT_FLOAT_EQ(tr::ratio(2s, 500ms), 4.0f);
	EXPECT_FLOAT_EQ(tr::ratio(250ms, 1s), 0.25f);
}

/// Tests converting a timestamp to local time.
TEST(ChronoTest, ConvertsToLocalTime)
{
	const std::time_t timestamp{1700000000};
	const std::tm actual{tr::localtime(timestamp)};
	const std::tm* expected{std::localtime(&timestamp)};
	ASSERT_NE(expected, nullptr);

	EXPECT_EQ(actual.tm_sec, expected->tm_sec);
	EXPECT_EQ(actual.tm_min, expected->tm_min);
	EXPECT_EQ(actual.tm_hour, expected->tm_hour);
	EXPECT_EQ(actual.tm_mday, expected->tm_mday);
	EXPECT_EQ(actual.tm_mon, expected->tm_mon);
	EXPECT_EQ(actual.tm_year, expected->tm_year);
	EXPECT_EQ(actual.tm_wday, expected->tm_wday);
	EXPECT_EQ(actual.tm_yday, expected->tm_yday);
	EXPECT_EQ(actual.tm_isdst, expected->tm_isdst);
}