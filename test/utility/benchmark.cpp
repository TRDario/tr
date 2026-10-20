/// @file
/// @brief Tests utility/benchmark.hpp.

#include <gtest/gtest.h>
#include <tr/utility/benchmark.hpp>

using namespace std::chrono_literals;

//

/// Tests default-constructing an empty benchmark.
TEST(BenchmarkTest, DefaultConstructsEmpty)
{
	tr::benchmark benchmark;
	EXPECT_TRUE(benchmark.measurements().empty());
	EXPECT_EQ(benchmark.latest(), tr::duration{0});
	EXPECT_EQ(benchmark.min(), tr::duration{0});
	EXPECT_EQ(benchmark.max(), tr::duration{0});
	EXPECT_EQ(benchmark.avg(), tr::duration{0});
	EXPECT_EQ(benchmark.fps(), 0);
}

/// Tests recording a measurement on a benchmark.
TEST(BenchmarkTest, RecordsMeasurement)
{
	tr::benchmark benchmark;
	benchmark.start();
	std::this_thread::sleep_for(25ms);
	benchmark.stop();
	EXPECT_EQ(benchmark.measurements().size(), 1);
	EXPECT_EQ(benchmark.measurements().back().duration, benchmark.latest());
	EXPECT_GE(benchmark.latest(), 25ms);
}

/// Tests aggregating multiple measurements on a benchmark.
TEST(BenchmarkTest, AggregatesMeasurements)
{
	tr::benchmark benchmark;
	for (int i = 0; i < 5; ++i) {
		benchmark.start();
		std::this_thread::sleep_for(10ms);
		benchmark.stop();
	}
	EXPECT_EQ(benchmark.measurements().size(), 5);
	EXPECT_EQ(benchmark.measurements().back().duration, benchmark.latest());
	EXPECT_LE(benchmark.min(), benchmark.avg());
	EXPECT_LE(benchmark.avg(), benchmark.max());
	EXPECT_GE(benchmark.min(), 10ms);
}

/// Tests clearing a benchmark to reset its measurements.
TEST(BenchmarkTest, ClearResetsMeasurements)
{
	tr::benchmark benchmark;
	benchmark.start();
	benchmark.stop();
	EXPECT_EQ(benchmark.measurements().size(), 1);

	benchmark.clear();
	EXPECT_TRUE(benchmark.measurements().empty());
	EXPECT_EQ(benchmark.latest(), tr::duration{0});
	EXPECT_EQ(benchmark.min(), tr::duration{0});
	EXPECT_EQ(benchmark.max(), tr::duration{0});
	EXPECT_EQ(benchmark.avg(), tr::duration{0});
	EXPECT_EQ(benchmark.fps(), 0);
}