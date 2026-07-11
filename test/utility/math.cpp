///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/math.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/math.hpp>
#include <tr/utility/rng.hpp>

TEST(math_test, round_cast)
{
	EXPECT_EQ(tr::round_cast<int>(4.00), 4);
	EXPECT_EQ(tr::round_cast<int>(4.43), 4);
	EXPECT_EQ(tr::round_cast<int>(4.50), 5);
	EXPECT_EQ(tr::round_cast<int>(4.63), 5);
	EXPECT_EQ(tr::round_cast<int>(5.00), 5);
}

TEST(math_test, floor_cast)
{
	EXPECT_EQ(tr::floor_cast<int>(4.00), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.43), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.50), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.63), 4);
	EXPECT_EQ(tr::floor_cast<int>(5.00), 5);
}

TEST(math_test, ceil_cast)
{
	EXPECT_EQ(tr::ceil_cast<int>(4.00), 4);
	EXPECT_EQ(tr::ceil_cast<int>(4.43), 5);
	EXPECT_EQ(tr::ceil_cast<int>(4.50), 5);
	EXPECT_EQ(tr::ceil_cast<int>(4.63), 5);
	EXPECT_EQ(tr::ceil_cast<int>(5.00), 5);
}

TEST(math_test, abs)
{
	tr::rng rng;
	for (int i = 0; i < 1000; ++i) {
		float value{rng.generate(-100.0f, 100.0f)};
		EXPECT_EQ(tr::abs(value), std::abs(value));
	}
}

TEST(math_test, sgn)
{
	EXPECT_EQ(tr::sgn(-INFINITY), -1);
	EXPECT_EQ(tr::sgn(-10), -1);
	EXPECT_EQ(tr::sgn(0), 0);
	EXPECT_EQ(tr::sgn(+0.0), 0);
	EXPECT_EQ(tr::sgn(-0.0), 0);
	EXPECT_EQ(tr::sgn(10), 1);
	EXPECT_EQ(tr::sgn(+INFINITY), 1);
}

TEST(math_test, mod)
{
	EXPECT_EQ(tr::mod(50, 3), 2);
	EXPECT_EQ(tr::mod(-50, 3), -2);
	EXPECT_EQ(tr::mod(50.0, 4.25), 3.25);
	EXPECT_EQ(tr::mod(-50.0, 4.25), -3.25);
}

TEST(math_test, eucmod)
{
	EXPECT_EQ(tr::eucmod(50, 3), 2);
	EXPECT_EQ(tr::eucmod(-50, 3), 1);
	EXPECT_EQ(tr::eucmod(50.0, 4.25), 3.25);
	EXPECT_EQ(tr::eucmod(-50.0, 4.25), 1);
}

TEST(math_test, lerp)
{
	EXPECT_EQ(tr::lerp(0, 100, 0.65f), 65);
	EXPECT_EQ(tr::lerp(50.0f, -50.0f, 0.5), 0);
	EXPECT_EQ(tr::lerp(20.0f, 50.0f, 0.0f), 20.0f);
	EXPECT_EQ(tr::lerp(20.0f, 50.0f, 1.0f), 50.0f);
}

TEST(math_test, sqr)
{
	EXPECT_EQ(tr::sqr(-3), 9);
	EXPECT_EQ(tr::sqr(0), 0);
	EXPECT_EQ(tr::sqr(2.5), 6.25);
	EXPECT_EQ(tr::sqr(4), 16);
	EXPECT_EQ(tr::sqr(-INFINITY), INFINITY);
}