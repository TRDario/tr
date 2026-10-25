/// @file
/// @brief Tests utility/color_cast.hpp.

#include <gtest/gtest.h>
#include <tr/utility/hsv.hpp>
#include <tr/utility/rgb.hpp>
#include <tr/utility/rgba.hpp>

//

/// Tests casting between RGB channel types.
TEST(ColorCastTest, CastsBetweenRgbChannelTypes)
{
	const tr::rgb<tr::u8> color{255, 128, 0};
	const tr::rgb<float> converted{tr::color_cast<tr::rgb<float>>(color)};
	EXPECT_FLOAT_EQ(converted.r, 1.0f);
	EXPECT_NEAR(converted.g, 128.0f / 255.0f, 1e-6f);
	EXPECT_FLOAT_EQ(converted.b, 0.0f);
	EXPECT_EQ(tr::color_cast<tr::rgb<tr::u8>>(color), color);
}

/// Tests casting between RGBA channel types.
TEST(ColorCastTest, CastsBetweenRgbaChannelTypes)
{
	const tr::rgba<tr::u8> color{255, 128, 0, 128};
	const tr::rgba<float> converted{tr::color_cast<tr::rgba<float>>(color)};
	EXPECT_FLOAT_EQ(converted.r, 1.0f);
	EXPECT_NEAR(converted.g, 128.0f / 255.0f, 1e-6f);
	EXPECT_FLOAT_EQ(converted.b, 0.0f);
	EXPECT_NEAR(converted.a, 128.0f / 255.0f, 1e-6f);
	EXPECT_EQ(tr::color_cast<tr::rgba<tr::u8>>(color), color);
}

/// Tests casts between RGB and RGBA color models.
TEST(ColorCastTest, CastsBetweenRgbAndRgba)
{
	const tr::rgb<tr::u8> rgb{255, 128, 0};
	const tr::rgba<float> rgba{tr::color_cast<tr::rgba<float>>(rgb)};
	EXPECT_FLOAT_EQ(rgba.r, 1.0f);
	EXPECT_NEAR(rgba.g, 128.0f / 255.0f, 1e-6f);
	EXPECT_FLOAT_EQ(rgba.b, 0.0f);
	EXPECT_FLOAT_EQ(rgba.a, 1.0f);
	EXPECT_EQ(tr::color_cast<tr::rgb<tr::u8>>(rgba), rgb);
}

/// Tests casts to and from HSV through the RGBA color model.
TEST(ColorCastTest, CastsThroughHsv)
{
	const tr::rgb<float> green{tr::color_cast<tr::rgb<float>>(tr::hsv{120.0f, 1.0f, 1.0f})};
	EXPECT_FLOAT_EQ(green.r, 0.0f);
	EXPECT_FLOAT_EQ(green.g, 1.0f);
	EXPECT_FLOAT_EQ(green.b, 0.0f);

	const tr::hsv hsv{tr::color_cast<tr::hsv>(tr::rgba<float>{1.0f, 0.0f, 0.0f, 0.25f})};
	EXPECT_NEAR(hsv.h, 0.0f, 1e-6f);
	EXPECT_NEAR(hsv.s, 1.0f, 1e-6f);
	EXPECT_NEAR(hsv.v, 1.0f, 1e-6f);
}
