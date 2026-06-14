#include <gtest/gtest.h>
#include <tr/utility/math.hpp>
#include <tr/utility/rng.hpp>

TEST(math_test, round_cast)
{
	ASSERT_EQ(tr::round_cast<int>(4.00), 4);
	ASSERT_EQ(tr::round_cast<int>(4.43), 4);
	ASSERT_EQ(tr::round_cast<int>(4.50), 5);
	ASSERT_EQ(tr::round_cast<int>(4.63), 5);
	ASSERT_EQ(tr::round_cast<int>(5.00), 5);
}

TEST(math_test, floor_cast)
{
	ASSERT_EQ(tr::floor_cast<int>(4.00), 4);
	ASSERT_EQ(tr::floor_cast<int>(4.43), 4);
	ASSERT_EQ(tr::floor_cast<int>(4.50), 4);
	ASSERT_EQ(tr::floor_cast<int>(4.63), 4);
	ASSERT_EQ(tr::floor_cast<int>(5.00), 5);
}

TEST(math_test, ceil_cast)
{
	ASSERT_EQ(tr::ceil_cast<int>(4.00), 4);
	ASSERT_EQ(tr::ceil_cast<int>(4.43), 5);
	ASSERT_EQ(tr::ceil_cast<int>(4.50), 5);
	ASSERT_EQ(tr::ceil_cast<int>(4.63), 5);
	ASSERT_EQ(tr::ceil_cast<int>(5.00), 5);
}

TEST(math_test, abs)
{
	tr::rng rng;
	for (int i = 0; i < 1000; ++i) {
		float value{rng.generate(-100.0f, 100.0f)};
		ASSERT_EQ(tr::abs(value), std::abs(value));
	}
}

TEST(math_test, sgn)
{
	ASSERT_EQ(tr::sgn(-INFINITY), -1);
	ASSERT_EQ(tr::sgn(-10), -1);
	ASSERT_EQ(tr::sgn(0), 0);
	ASSERT_EQ(tr::sgn(+0.0), 0);
	ASSERT_EQ(tr::sgn(-0.0), 0);
	ASSERT_EQ(tr::sgn(10), 1);
	ASSERT_EQ(tr::sgn(+INFINITY), 1);
}

TEST(math_test, mod)
{
	ASSERT_EQ(tr::mod(50, 3), 2);
	ASSERT_EQ(tr::mod(-50, 3), -2);
	ASSERT_EQ(tr::mod(50.0, 4.25), 3.25);
	ASSERT_EQ(tr::mod(-50.0, 4.25), -3.25);
}

TEST(math_test, eucmod)
{
	ASSERT_EQ(tr::eucmod(50, 3), 2);
	ASSERT_EQ(tr::eucmod(-50, 3), 1);
	ASSERT_EQ(tr::eucmod(50.0, 4.25), 3.25);
	ASSERT_EQ(tr::eucmod(-50.0, 4.25), 1);
}

TEST(math_test, lerp)
{
	ASSERT_EQ(tr::lerp(0, 100, 0.65f), 65);
	ASSERT_EQ(tr::lerp(50.0f, -50.0f, 0.5), 0);
	ASSERT_EQ(tr::lerp(20.0f, 50.0f, 0.0f), 20.0f);
	ASSERT_EQ(tr::lerp(20.0f, 50.0f, 1.0f), 50.0f);
}

TEST(math_test, sqr)
{
	ASSERT_EQ(tr::sqr(-3), 9);
	ASSERT_EQ(tr::sqr(0), 0);
	ASSERT_EQ(tr::sqr(2.5), 6.25);
	ASSERT_EQ(tr::sqr(4), 16);
	ASSERT_EQ(tr::sqr(-INFINITY), INFINITY);
}