/// @file
/// @brief Tests utility/angle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/angle.hpp>

using namespace tr::angle_literals;

//

/// Tests the angle representation type selected by `tr::rads()`.
static_assert(std::same_as<decltype(tr::rads(1.0f)), tr::angle<float>>);

/// Tests the angle representation type selected by `tr::rads()`.
static_assert(std::same_as<decltype(tr::rads(1.0)), tr::angle<double>>);

/// Tests the angle representation type selected by `tr::rads()`.
static_assert(std::same_as<decltype(tr::rads(1)), tr::angle<double>>);

/// Tests the angle representation type selected by `operator+()`.
static_assert(std::same_as<decltype(tr::angle<float>{1.0f} + tr::angle<double>{2.0}), tr::angle<double>>);

//

/// Tests constructing and converting angles between units.
TEST(AngleTest, ConstructsAndConvertsUnits)
{
	EXPECT_FLOAT_EQ(tr::rads(2.0).rads(), 2.0f);
	EXPECT_FLOAT_EQ(tr::degs(180).rads(), std::numbers::pi);
	EXPECT_FLOAT_EQ(tr::turns(0.5).rads(), std::numbers::pi);

	EXPECT_FLOAT_EQ(tr::degs(90).degs(), 90.0f);
	EXPECT_FLOAT_EQ(tr::turns(0.25).turns(), 0.25f);
	EXPECT_FLOAT_EQ(tr::rads(std::numbers::pi).turns(), 0.5f);
}

/// Tests converting angle values between representation types.
TEST(AngleTest, ConvertsRepresentationTypes)
{
	const tr::angle<double> precise{1.23456789};
	const tr::angle<float> approximate{static_cast<tr::angle<float>>(precise)};
	EXPECT_FLOAT_EQ(approximate.rads(), static_cast<float>(precise.rads()));

	const tr::angle<long double> extended{1.234567890123456789L};
	EXPECT_EQ(extended.rads(), 1.234567890123456789L);
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
	EXPECT_EQ(1_rad + 2_radf, 3_rad);
	EXPECT_NEAR((90_deg - 30_deg).rads(), (60_deg).rads(), 1e-6);
	EXPECT_EQ(3_rad - 2_radf, 1_rad);
	EXPECT_EQ(-45_deg, tr::degs(-45));
	EXPECT_EQ(30_deg * 3, 90_deg);
	EXPECT_EQ(3 * 30_deg, 90_deg);
	EXPECT_EQ(90_deg / 3, 30_deg);
	EXPECT_FLOAT_EQ(90_deg / 30_deg, 3);
	EXPECT_NEAR((100_deg % 30_deg).rads(), (10_deg).rads(), 1e-6);

	tr::angle<float> value{10_deg};
	value += 20_deg;
	EXPECT_NEAR(value.rads(), (30_deg).rads(), 1e-6);

	value -= 10_deg;
	EXPECT_NEAR(value.rads(), (20_deg).rads(), 1e-6);

	value *= 3;
	EXPECT_NEAR(value.rads(), (60_deg).rads(), 1e-6);

	value /= 2;
	EXPECT_NEAR(value.rads(), (30_deg).rads(), 1e-6);
}

/// Tests the trigonometric functions of an angle.
TEST(AngleTest, ComputesTrigonometricFunctions)
{
	EXPECT_NEAR((45_deg).sin(), std::sqrt(0.5), 1e-6);
	EXPECT_NEAR((45_deg).cos(), std::sqrt(0.5), 1e-6);
	EXPECT_NEAR((45_deg).tan(), 1.0f, 1e-6);
}

/// Tests inverse trigonometric functions producing an angle.
TEST(AngleTest, ComputesInverseTrigonometricFunctions)
{
	EXPECT_NEAR(tr::asin(1.0).rads(), std::numbers::pi / 2, 1e-6);
	EXPECT_NEAR(tr::acos(0.0).rads(), std::numbers::pi / 2, 1e-6);
	EXPECT_NEAR(tr::atan(1.0).rads(), std::numbers::pi / 4, 1e-6);
	EXPECT_NEAR(tr::atan2(1.0, 0.0).rads(), std::numbers::pi / 2, 1e-6);
	EXPECT_NEAR(tr::atan2(glm::vec2{0.0f, 1.0f}).rads(), std::numbers::pi_v<float> / 2, 1e-6f);
}

/// Tests absolute value and shortest-path angle interpolation.
TEST(AngleTest, ComputesAbsoluteValueAndLerp)
{
	EXPECT_EQ(tr::abs(-45_deg), 45_deg);
	EXPECT_EQ(tr::lerp(0_deg, 90_deg, 0.5f), 45_deg);
	EXPECT_NEAR(tr::lerp(350_deg, 10_degf, 0.5f).rads(), 0.0f, 1e-6f);
}

/// Tests formatting angles in each supported unit.
TEST(AngleTest, FormatsInRequestedUnit)
{
	EXPECT_EQ(std::format("{:r.3f}", 90_deg), "1.571rad");
	EXPECT_EQ(std::format("{:d}", 90_deg), "90deg");
	EXPECT_EQ(std::format("{:t}", 90_deg), "0.25tr");
}