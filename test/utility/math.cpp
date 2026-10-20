/// @file
/// @brief Tests utility/math.hpp.

#include <gtest/gtest.h>
#include <tr/utility/math.hpp>

//

/// Tests rounding and casting a value.
TEST(MathTest, RoundCasts)
{
	EXPECT_EQ(tr::round_cast<int>(4.00), 4);
	EXPECT_EQ(tr::round_cast<int>(4.43), 4);
	EXPECT_EQ(tr::round_cast<int>(4.50), 5);
	EXPECT_EQ(tr::round_cast<int>(4.63), 5);
	EXPECT_EQ(tr::round_cast<int>(5.00), 5);
}

/// Tests flooring and casting a value.
TEST(MathTest, FloorCasts)
{
	EXPECT_EQ(tr::floor_cast<int>(4.00), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.43), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.50), 4);
	EXPECT_EQ(tr::floor_cast<int>(4.63), 4);
	EXPECT_EQ(tr::floor_cast<int>(5.00), 5);
}

/// Tests ceiling and casting a value.
TEST(MathTest, CeilingCasts)
{
	EXPECT_EQ(tr::ceil_cast<int>(4.00), 4);
	EXPECT_EQ(tr::ceil_cast<int>(4.43), 5);
	EXPECT_EQ(tr::ceil_cast<int>(4.50), 5);
	EXPECT_EQ(tr::ceil_cast<int>(4.63), 5);
	EXPECT_EQ(tr::ceil_cast<int>(5.00), 5);
}

/// Tests calculating the sign of a value.
TEST(MathTest, CalculatesSign)
{
	EXPECT_EQ(tr::sgn(-INFINITY), -1);
	EXPECT_EQ(tr::sgn(-10), -1);
	EXPECT_EQ(tr::sgn(0), 0);
	EXPECT_EQ(tr::sgn(+0.0), 0);
	EXPECT_EQ(tr::sgn(-0.0), 0);
	EXPECT_EQ(tr::sgn(NAN), 0);
	EXPECT_EQ(tr::sgn(10), 1);
	EXPECT_EQ(tr::sgn(+INFINITY), 1);
}

/// Tests calculating the modulo of a value.
TEST(MathTest, CalculatesModulo)
{
	EXPECT_EQ(tr::mod(50, 3), 2);
	EXPECT_EQ(tr::mod(-50, 3), -2);
	EXPECT_EQ(tr::mod(50.0, 4.25), 3.25);
	EXPECT_EQ(tr::mod(-50.0, 4.25), -3.25);
}

/// Tests calculating the euclidian modulo of a value.
TEST(MathTest, CalculatesEuclidianModulo)
{
	EXPECT_EQ(tr::eucmod(50, 3), 2);
	EXPECT_EQ(tr::eucmod(-50, 3), 1);
	EXPECT_EQ(tr::eucmod(50.0, 4.25), 3.25);
	EXPECT_EQ(tr::eucmod(-50.0, 4.25), 1);
}

/// Tests lerping between two values.
TEST(MathTest, Lerps)
{
	EXPECT_EQ(tr::lerp(0, 100, 0.65f), 65);
	EXPECT_EQ(tr::lerp(50.0f, -50.0f, 0.5), 0);
	EXPECT_EQ(tr::lerp(20.0f, 50.0f, 0.0f), 20.0f);
	EXPECT_EQ(tr::lerp(20.0f, 50.0f, 1.0f), 50.0f);
}

/// Tests calculating the square of a value.
TEST(MathTest, CalculatesSquare)
{
	EXPECT_EQ(tr::sqr(-3), 9);
	EXPECT_EQ(tr::sqr(0), 0);
	EXPECT_EQ(tr::sqr(2.5), 6.25);
	EXPECT_EQ(tr::sqr(4), 16);
	EXPECT_EQ(tr::sqr(-INFINITY), INFINITY);
}