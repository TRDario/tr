/// @file
/// @brief Tests utility/rgba.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rgba.hpp>

//

/// Tests constructing RGBA colors and defaulting alpha to opaque.
TEST(RgbaTest, ConstructsWithOpaqueDefaultAlpha)
{
	const tr::rgba<float> color{0.1f, 0.2f, 0.3f};
	EXPECT_FLOAT_EQ(color.r, 0.1f);
	EXPECT_FLOAT_EQ(color.g, 0.2f);
	EXPECT_FLOAT_EQ(color.b, 0.3f);
	EXPECT_FLOAT_EQ(color.a, 1.0f);
	EXPECT_EQ(color, (tr::rgba<float>{0.1f, 0.2f, 0.3f, 1.0f}));
	EXPECT_NE(color, (tr::rgba<float>{0.1f, 0.2f, 0.3f, 0.5f}));
}

/// Tests conversion to a vector.
TEST(RgbaTest, ConvertsToVector)
{
	const tr::rgba<float> color{0.1f, 0.2f, 0.3f, 0.4f};
	const glm::vec4 vector{static_cast<glm::vec4>(color)};
	EXPECT_FLOAT_EQ(vector.r, 0.1f);
	EXPECT_FLOAT_EQ(vector.g, 0.2f);
	EXPECT_FLOAT_EQ(vector.b, 0.3f);
	EXPECT_FLOAT_EQ(vector.a, 0.4f);
}

/// Tests scalar and component-wise RGBA arithmetic.
TEST(RgbaTest, PerformsArithmetic)
{
	const tr::rgba<int> color{12, 18, 24, 30};
	EXPECT_EQ(color + 2, (tr::rgba<int>{14, 20, 26, 32}));
	EXPECT_EQ(color - 2, (tr::rgba<int>{10, 16, 22, 28}));
	EXPECT_EQ(color * 2, (tr::rgba<int>{24, 36, 48, 60}));
	EXPECT_EQ(color / 2, (tr::rgba<int>{6, 9, 12, 15}));
	EXPECT_EQ((color + tr::rgba<int>{1, 2, 3, 4}), (tr::rgba<int>{13, 20, 27, 34}));
	EXPECT_EQ((color - tr::rgba<int>{1, 2, 3, 4}), (tr::rgba<int>{11, 16, 21, 26}));
	EXPECT_EQ((color * tr::rgba<int>{1, 2, 3, 4}), (tr::rgba<int>{12, 36, 72, 120}));
	EXPECT_EQ((color / tr::rgba<int>{2, 3, 4, 5}), (tr::rgba<int>{6, 6, 6, 6}));

	tr::rgba<int> value{12, 18, 24, 30};
	value += 2;
	EXPECT_EQ(value, (tr::rgba<int>{14, 20, 26, 32}));
	value -= tr::rgba<int>{2, 4, 6, 8};
	EXPECT_EQ(value, (tr::rgba<int>{12, 16, 20, 24}));
	value *= 2;
	EXPECT_EQ(value, (tr::rgba<int>{24, 32, 40, 48}));
	value /= tr::rgba<int>{2, 4, 5, 6};
	EXPECT_EQ(value, (tr::rgba<int>{12, 8, 8, 8}));
}
