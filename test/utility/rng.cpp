///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/rng.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/rng.hpp>

// RNG test fixture.
class rng_test : public testing::Test {
  protected:
	// The RNG being tested.
	tr::rng rng;
};

TEST_F(rng_test, advance)
{
	const tr::u64 first{rng.advance()};
	const tr::u64 second{rng.advance()};
	EXPECT_NE(first, second);
}

TEST_F(rng_test, bool_distribution)
{
	std::array<tr::i64, 2> results{};
	for (int i = 0; i < 10000000; ++i) {
		++results[rng.generate<bool>()];
	}
	EXPECT_LT(std::abs(results[1] - results[0]), 10000);
}

TEST_F(rng_test, sign_distribution)
{
	int value{0};
	for (int i = 0; i < 10000000; ++i) {
		value += rng.generate_sign();
	}
	EXPECT_LT(std::abs(value), 10000);
}

TEST_F(rng_test, integer_distribution)
{
	std::array<tr::i64, 11> results{};
	for (int i = 0; i < 10000000; ++i) {
		const int result{rng.generate(-5, 6)};
		EXPECT_GE(result, -5);
		EXPECT_LE(result, 5);
		++results[result + 5];
	}
	const auto minmax{std::ranges::minmax_element(results)};
	EXPECT_LT(minmax.max - minmax.min, 2500);
}

TEST_F(rng_test, floating_point_default_range_distribution)
{
	double sum{0};
	for (int i = 0; i < 10000000; ++i) {
		const double result{rng.generate<double>()};
		EXPECT_GE(result, 0.0);
		EXPECT_LE(result, 1.0);
		sum += result;
	}
	EXPECT_NEAR(sum / 10000000, 0.5, 1e-3);
}

TEST_F(rng_test, floating_point_custom_range_distribution)
{
	double sum{0};
	for (int i = 0; i < 10000000; ++i) {
		const double result{rng.generate(-2.0, 1.0)};
		EXPECT_GE(result, -2.0);
		EXPECT_LE(result, 1.0);
		sum += result;
	}
	EXPECT_NEAR(sum / 10000000, -0.5, 1e-3);
}

TEST_F(rng_test, angle_default_range_distribution)
{
	using namespace tr::angle_literals;

	tr::angle sum{};
	for (int i = 0; i < 100000; ++i) {
		const tr::angle result{rng.generate<tr::angle>()};
		EXPECT_GE(result, 0_tr);
		EXPECT_LE(result, 1_tr);
		sum += result;
	}
	EXPECT_NEAR((sum / 100000).turns(), 0.5, 0.01);
}

TEST_F(rng_test, angle_custom_range_distribution)
{
	using namespace tr::angle_literals;

	tr::angle sum{};
	for (int i = 0; i < 100000; ++i) {
		const tr::angle result{rng.generate(0_deg, 270_deg)};
		EXPECT_GE(result, 0_deg);
		EXPECT_LE(result, 270_deg);
		sum += result;
	}
	EXPECT_NEAR((sum / 100000).degs(), 135, 0.5);
}

TEST_F(rng_test, vector_magnitude_distribution)
{
	glm::vec2 sum{};
	for (int i = 0; i < 10000000; ++i) {
		const glm::vec2 result{rng.generate<glm::vec2>(1)};
		EXPECT_NEAR(glm::length(result), 1, 1e-6);
		sum += result;
	}
	sum /= 10000000;
	EXPECT_NEAR(glm::length(sum), 0, 1e-2);
}

TEST_F(rng_test, vector_rect_distribution)
{
	const tr::frect2 rect{{-100, -100}, {200, 200}};
	glm::vec2 sum{};
	for (int i = 0; i < 10000000; ++i) {
		const glm::vec2 result{rng.generate<glm::vec2>(rect)};
		EXPECT_TRUE(rect.contains(result));
		sum += result;
	}
	sum /= 10000000;
	EXPECT_NEAR(glm::length(sum), 0, 1);
}