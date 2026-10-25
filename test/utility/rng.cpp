/// @file
/// @brief Tests utility/rng.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rng.hpp>

//

/// RNG test fixture.
class RngTest : public testing::Test
{
  protected:
	/// RNG being tested (static seed).
	tr::rng rng{1337};
};

//

/// Tests advancing the RNG.
TEST_F(RngTest, Advances)
{
	const tr::u64 first{rng.advance()};
	const tr::u64 second{rng.advance()};
	EXPECT_NE(first, second);
}

/// Tests for a balanced boolean distribution.
TEST_F(RngTest, BoolDistributionIsBalanced)
{
	std::array<tr::i64, 2> results{};
	for (int i = 0; i < 1000000; ++i) {
		++results[rng.generate<bool>()];
	}
	EXPECT_LT(std::abs(results[1] - results[0]), 1000);
}

/// Tests for a balanced sign distribution.
TEST_F(RngTest, SignDistributionIsBalanced)
{
	std::array<tr::i64, 2> results{};
	for (int i = 0; i < 1000000; ++i) {
		const int value{rng.generate_sign()};
		EXPECT_TRUE(value == 1 || value == -1);
		++results[(value + 1) / 2];
	}
	EXPECT_LT(std::abs(results[1] - results[0]), 1000);
}

/// Tests for a balanced integer distribution in the range [0, MAX).
TEST_F(RngTest, IntMaxDistributionIsBalanced)
{
	std::array<tr::i64, 10> results{};
	for (int i = 0; i < 1000000; ++i) {
		const int result{rng.generate(10)};
		EXPECT_GE(result, 0);
		EXPECT_LT(result, 10);
		++results[result];
	}
	const auto minmax{std::ranges::minmax_element(results)};
	EXPECT_GT(results.front(), 0);
	EXPECT_GT(results.back(), 0);
	EXPECT_LT(minmax.max - minmax.min, 250);
}

/// Tests for a balanced integer distribution in the range [MIN, MAX).
TEST_F(RngTest, IntMinMaxDistributionIsBalanced)
{
	std::array<tr::i64, 11> results{};
	for (int i = 0; i < 1000000; ++i) {
		const int result{rng.generate(-5, 6)};
		EXPECT_GE(result, -5);
		EXPECT_LE(result, 5);
		++results[result + 5];
	}
	const auto minmax{std::ranges::minmax_element(results)};
	EXPECT_GT(results.front(), 0);
	EXPECT_GT(results.back(), 0);
	EXPECT_LT(minmax.max - minmax.min, 250);
}

/// Tests for a balanced float distribution in the range [0, 1).
TEST_F(RngTest, FloatDistributionIsBalanced)
{
	double sum{0};
	for (int i = 0; i < 1000000; ++i) {
		const double result{rng.generate<double>()};
		EXPECT_GE(result, 0.0);
		EXPECT_LT(result, 1.0);
		sum += result;
	}
	EXPECT_NEAR(sum / 1000000, 0.5, 1e-2);
}

/// Tests for a balanced float distribution in the range [0, MAX).
TEST_F(RngTest, FloatMaxDistributionIsBalanced)
{
	double sum{0};
	for (int i = 0; i < 1000000; ++i) {
		const double result{rng.generate(5.0)};
		EXPECT_GE(result, 0.0);
		EXPECT_LT(result, 5.0);
		sum += result;
	}
	EXPECT_NEAR(sum / 1000000, 2.5, 1e-2);
}

/// Tests for a balanced float distribution in the range [MIN, MAX).
TEST_F(RngTest, FloatMinMaxDistributionIsBalanced)
{
	double sum{0};
	for (int i = 0; i < 1000000; ++i) {
		const double result{rng.generate(-2.0, 1.0)};
		EXPECT_GE(result, -2.0);
		EXPECT_LT(result, 1.0);
		sum += result;
	}
	EXPECT_NEAR(sum / 1000000, -0.5, 1e-2);
}

/// Tests for a balanced angle distribution in the range [0tr, MAX).
TEST_F(RngTest, AngleMaxDistributionIsBalanced)
{
	using namespace tr::angle_literals;

	tr::angle<double> sum{};
	for (int i = 0; i < 100000; ++i) {
		const tr::angle result{rng.generate(180_deg)};
		EXPECT_GE(result, 0_deg);
		EXPECT_LT(result, 180_deg);
		sum += result;
	}
	EXPECT_NEAR((sum / 100000).degs(), 90, 0.75f);
}

/// Tests for a balanced angle distribution in the range [MIN, MAX).
TEST_F(RngTest, AngleMinMaxDistributionIsBalanced)
{
	using namespace tr::angle_literals;

	tr::angle<double> sum{};
	for (int i = 0; i < 100000; ++i) {
		const tr::angle result{rng.generate(0_deg, 270_deg)};
		EXPECT_GE(result, 0_deg);
		EXPECT_LT(result, 270_deg);
		sum += result;
	}
	EXPECT_NEAR((sum / 100000).degs(), 135, 0.75f);
}

/// Tests for a balanced vector distribution with a fixed magnitude.
TEST_F(RngTest, VectorMagnitudeDistributionIsBalanced)
{
	glm::vec2 sum{};
	for (int i = 0; i < 1000000; ++i) {
		const glm::vec2 result{rng.generate<glm::vec2>(2.5)};
		EXPECT_NEAR(glm::length(result), 2.5, 1e-6);
		sum += result;
	}
	sum /= 1000000;
	EXPECT_NEAR(glm::length(sum), 0, 1e-2);
}

/// Tests for a balanced vector distribution within a rectangle.
TEST_F(RngTest, VectorRectDistributionIsBalanced)
{
	const tr::rectangle<float> region{{-100, -100}, {200, 200}};
	glm::vec2 sum{};
	for (int i = 0; i < 1000000; ++i) {
		const glm::vec2 result{rng.generate<glm::vec2>(region)};
		EXPECT_TRUE(region.contains(result));
		sum += result;
	}
	sum /= 1000000;
	EXPECT_NEAR(glm::length(sum), 0, 1);
}