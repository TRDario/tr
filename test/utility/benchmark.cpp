///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/benchmark.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/benchmark.hpp>

using namespace std::chrono_literals;

TEST(benchmark_test, single_measurement)
{
	tr::benchmark benchmark;
	benchmark.start();
	std::this_thread::sleep_for(100ms);
	benchmark.stop();
	EXPECT_EQ(benchmark.measurements().size(), 1);
	EXPECT_GE(benchmark.latest(), 100ms);
}

TEST(benchmark_test, multiple_measurements)
{
	tr::benchmark benchmark;
	benchmark.start();
	std::this_thread::sleep_for(50ms);
	benchmark.stop();
	for (int i = 0; i < 5; ++i) {
		benchmark.start();
		std::this_thread::sleep_for(100ms);
		benchmark.stop();
	}
	benchmark.start();
	std::this_thread::sleep_for(150ms);
	benchmark.stop();
	EXPECT_NEAR(tr::dmsecs{benchmark.min()}.count(), 50.0, 10.0);
	EXPECT_NEAR(tr::dmsecs{benchmark.avg()}.count(), 100.0, 10.0);
	EXPECT_NEAR(tr::dmsecs{benchmark.max()}.count(), 150.0, 10.0);
}

TEST(benchmark_test, reset)
{
	tr::benchmark benchmark;
	benchmark.start();
	std::this_thread::sleep_for(100ms);
	benchmark.stop();
	benchmark.clear();
	EXPECT_TRUE(benchmark.measurements().empty());
	EXPECT_EQ(benchmark.min().count(), 0);
	EXPECT_EQ(benchmark.avg().count(), 0);
	EXPECT_EQ(benchmark.max().count(), 0);
	EXPECT_EQ(benchmark.fps(), 0);
}