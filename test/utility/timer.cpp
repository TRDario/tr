///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/timer.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/timer.hpp>

using namespace std::chrono_literals;

TEST(timer_test, inactive)
{
	tr::timer timer;
	EXPECT_FALSE(timer.active());
}

TEST(timer_test, active)
{
	std::atomic_int i = 0;
	tr::timer timer{50ms, [&] { i++; }};

	std::this_thread::sleep_for(255ms);
	EXPECT_EQ(i, 5);
	std::this_thread::sleep_for(250ms);
	EXPECT_EQ(i, 10);
}

TEST(timer_test, move)
{
	std::atomic_int i = 0;
	tr::timer timer{50ms, [&] { i++; }};

	std::this_thread::sleep_for(255ms);
	EXPECT_EQ(i, 5);

	tr::timer timer2 = std::move(timer);

	std::this_thread::sleep_for(250ms);
	EXPECT_EQ(i, 10);

	timer = std::move(timer2);

	std::this_thread::sleep_for(250ms);
	EXPECT_EQ(i, 15);
}