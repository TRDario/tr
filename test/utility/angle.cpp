/// @file
/// @brief Tests utility/angle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/angle.hpp>

using namespace tr::angle_literals;

//

/// Tests constructing and converting angles between units.
TEST(AngleTest, ConstructsAndConvertsUnits)
{
	EXPECT_FLOAT_EQ(tr::rads(2.0).rads(), 2.0f);
	EXPECT_FLOAT_EQ(tr::degs(180).rads(), std::numbers::pi_v<float>);
	EXPECT_FLOAT_EQ(tr::turns(0.5).rads(), std::numbers::pi_v<float>);

	EXPECT_FLOAT_EQ(tr::degs(90).degs(), 90.0f);
	EXPECT_FLOAT_EQ(tr::turns(0.25).turns(), 0.25f);
	EXPECT_FLOAT_EQ(tr::rads(std::numbers::pi_v<float>).turns(), 0.5f);
}

/// Tests the degree, radian, and turn literals.
TEST(AngleTest, ConstructsFromLiterals)
{
	EXPECT_EQ(180_deg, tr::degs(180));
	EXPECT_EQ(2_rad, tr::rads(2));
	EXPECT_EQ(0.5_tr, tr::turns(0.5));
}

/// Tests comparing angle values.
TEST(AngleTest, ComparesAngles)
{
	EXPECT_EQ(90_deg, 0.25_tr);
	EXPECT_LT(45_deg, 90_deg);
	EXPECT_GT(180_deg, 90_deg);
	EXPECT_NE(90_deg, 45_deg);
}

/// Tests angle and scalar arithmetic.
TEST(AngleTest, PerformsArithmetic)
{
	EXPECT_EQ(30_deg + 60_deg, 90_deg);
	EXPECT_EQ(90_deg - 30_deg, 60_deg);
	EXPECT_EQ(-45_deg, -45_deg);
	EXPECT_EQ(30_deg * 3, 90_deg);
	EXPECT_EQ(3 * 30_deg, 90_deg);
	EXPECT_EQ(90_deg / 3, 30_deg);
	EXPECT_FLOAT_EQ(90_deg / 30_deg, 3.0f);
	EXPECT_NEAR((100_deg % 30_deg).rads(), (10_deg).rads(), 1e-6f);

	tr::angle value{10_deg};
	value += 20_deg;
	EXPECT_NEAR(value.rads(), (30_deg).rads(), 1e-6f);

	value -= 10_deg;
	EXPECT_NEAR(value.rads(), (20_deg).rads(), 1e-6f);

	value *= 3;
	EXPECT_NEAR(value.rads(), (60_deg).rads(), 1e-6f);

	value /= 2;
	EXPECT_NEAR(value.rads(), (30_deg).rads(), 1e-6f);
}

/// Tests the trigonometric functions of an angle.
TEST(AngleTest, ComputesTrigonometricFunctions)
{
	EXPECT_NEAR((45_deg).sin(), std::sqrt(0.5f), 1e-6f);
	EXPECT_NEAR((45_deg).cos(), std::sqrt(0.5f), 1e-6f);
	EXPECT_NEAR((45_deg).tan(), 1.0f, 1e-6f);
}

/// Tests inverse trigonometric functions producing an angle.
TEST(AngleTest, ComputesInverseTrigonometricFunctions)
{
	EXPECT_NEAR(tr::asin(1.0f).rads(), std::numbers::pi_v<float> / 2, 1e-6f);
	EXPECT_NEAR(tr::acos(0.0f).rads(), std::numbers::pi_v<float> / 2, 1e-6f);
	EXPECT_NEAR(tr::atan(1.0f).rads(), std::numbers::pi_v<float> / 4, 1e-6f);
	EXPECT_NEAR(tr::atan2(1.0f, 0.0f).rads(), std::numbers::pi_v<float> / 2, 1e-6f);
	EXPECT_NEAR(tr::atan2(glm::vec2{0.0f, 1.0f}).rads(), std::numbers::pi_v<float> / 2, 1e-6f);
}

/// Tests absolute value and shortest-path angle interpolation.
TEST(AngleTest, ComputesAbsoluteValueAndLerp)
{
	EXPECT_EQ(tr::abs(-45_deg), 45_deg);
	EXPECT_EQ(tr::lerp(0_deg, 90_deg, 0.5f), 45_deg);
	EXPECT_NEAR(tr::lerp(350_deg, 10_deg, 0.5f).rads(), 0.0f, 1e-6f);
}

/// Tests formatting angles in each supported unit.
TEST(AngleTest, FormatsInRequestedUnit)
{
	EXPECT_EQ(std::format("{:r}", 90_deg), "1.5707964rad");
	EXPECT_EQ(std::format("{:d}", 90_deg), "90deg");
	EXPECT_EQ(std::format("{:t}", 90_deg), "0.25tr");
}