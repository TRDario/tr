/// @file
/// @brief Tests utility/rgb.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rgb.hpp>
#include <tr/utility/rgba.hpp>

//

/// Tests constructing and comparing RGB colors.
TEST(RgbTest, ConstructsAndCompares)
{
	const tr::rgb<int> color{10, 20, 30};
	EXPECT_EQ(color.r, 10);
	EXPECT_EQ(color.g, 20);
	EXPECT_EQ(color.b, 30);
	EXPECT_EQ(color, (tr::rgb<double>{10.0, 20.0, 30.0}));
	EXPECT_NE(color, (tr::rgb<int>{10, 20, 31}));
}

/// Tests conversion to a vector and adding an alpha channel.
TEST(RgbTest, ConvertsToVectorAndAddsAlpha)
{
	const tr::rgb<float> color{0.1f, 0.2f, 0.3f};
	const glm::vec3 vector{static_cast<glm::vec3>(color)};
	EXPECT_FLOAT_EQ(vector.r, 0.1f);
	EXPECT_FLOAT_EQ(vector.g, 0.2f);
	EXPECT_FLOAT_EQ(vector.b, 0.3f);
	EXPECT_EQ(color.with_alpha(0.4f), (tr::rgba<float>{0.1f, 0.2f, 0.3f, 0.4f}));
}

/// Tests scalar and component-wise RGB arithmetic.
TEST(RgbTest, PerformsArithmetic)
{
	const tr::rgb<int> color{12, 18, 24};
	EXPECT_EQ(color + 2, (tr::rgb<int>{14, 20, 26}));
	EXPECT_EQ(color - 2, (tr::rgb<int>{10, 16, 22}));
	EXPECT_EQ(color * 2, (tr::rgb<int>{24, 36, 48}));
	EXPECT_EQ(color / 2, (tr::rgb<int>{6, 9, 12}));
	EXPECT_EQ((color + tr::rgb<int>{1, 2, 3}), (tr::rgb<int>{13, 20, 27}));
	EXPECT_EQ((color - tr::rgb<int>{1, 2, 3}), (tr::rgb<int>{11, 16, 21}));
	EXPECT_EQ((color * tr::rgb<int>{1, 2, 3}), (tr::rgb<int>{12, 36, 72}));
	EXPECT_EQ((color / tr::rgb<int>{2, 3, 4}), (tr::rgb<int>{6, 6, 6}));

	tr::rgb<int> value{12, 18, 24};
	value += 2;
	EXPECT_EQ(value, (tr::rgb<int>{14, 20, 26}));
	value -= tr::rgb<int>{2, 4, 6};
	EXPECT_EQ(value, (tr::rgb<int>{12, 16, 20}));
	value *= 2;
	EXPECT_EQ(value, (tr::rgb<int>{24, 32, 40}));
	value /= tr::rgb<int>{2, 4, 5};
	EXPECT_EQ(value, (tr::rgb<int>{12, 8, 8}));
}
