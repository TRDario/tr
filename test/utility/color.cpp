/// @file
/// @brief Tests utility/color.hpp.

#include <gtest/gtest.h>
#include <tr/utility/color.hpp>

using namespace tr::color_literals;

//

/// Tests constructing an rgb8 color from channels.
TEST(ColorTest, Rgb8ConstructsFromChannels)
{
	const tr::rgb8 color{40, 80, 120};
	EXPECT_EQ(color.r, 40);
	EXPECT_EQ(color.g, 80);
	EXPECT_EQ(color.b, 120);
}

/// Tests constructing an rgb8 color from an rgbf color.
TEST(ColorTest, Rgb8ConstructsFromRgbf)
{
	const tr::rgb8 color{tr::rgbf{0, 0.5f, 1.0f}};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 127);
	EXPECT_EQ(color.b, 255);
}

/// Tests constructing an rgb8 color from an rgba8 color.
TEST(ColorTest, Rgb8ConstructsFromRgba8)
{
	const tr::rgb8 color{tr::rgba8{0, 64, 128, 192}};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 64);
	EXPECT_EQ(color.b, 128);
}

/// Tests constructing an rgb8 color from an rgbaf color.
TEST(ColorTest, Rgb8ConstructsFromRgbaf)
{
	const tr::rgb8 color{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 127);
	EXPECT_EQ(color.b, 255);
}

/// Tests constructing an rgb8 color from an hsv color.
TEST(ColorTest, Rgb8ConstructsFromHsv)
{
	const tr::rgb8 color{tr::hsv{0, 1, 1}};
	EXPECT_EQ(color.r, 255);
	EXPECT_EQ(color.g, 0);
	EXPECT_EQ(color.b, 0);
}

/// Tests constructing an rgbf color from channels.
TEST(ColorTest, RgbfConstructsFromChannels)
{
	const tr::rgbf color{0, 0.5f, 1.0f};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 0.5f);
	EXPECT_EQ(color.b, 1.0f);
}

/// Tests constructing an rgbf color from an rgb8 color.
TEST(ColorTest, RgbfConstructsFromRgb8)
{
	const tr::rgbf color{tr::rgb8{0, 63, 127}};
	EXPECT_EQ(color.r, 0.0f);
	EXPECT_NEAR(color.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(color.b, 0.5f, 1 / 255.0f + 1e-3f);
}

/// Tests constructing an rgbf color from an rgba8 color.
TEST(ColorTest, RgbfConstructsFromRgba8)
{
	const tr::rgbf color{tr::rgba8{0, 63, 127, 192}};
	EXPECT_EQ(color.r, 0.0f);
	EXPECT_NEAR(color.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(color.b, 0.5f, 1 / 255.0f + 1e-3f);
}

/// Tests constructing an rgbf color from an rgbaf color.
TEST(ColorTest, RgbfConstructsFromRgbaf)
{
	const tr::rgbf color{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 0.5f);
	EXPECT_EQ(color.b, 1.0f);
}

/// Tests constructing an rgbf color from an hsv color.
TEST(ColorTest, RgbfConstructsFromHsv)
{
	const tr::rgbf color{tr::hsv{0, 1, 1}};
	EXPECT_EQ(color.r, 1.0f);
	EXPECT_EQ(color.g, 0.0f);
	EXPECT_EQ(color.b, 0.0f);
}

/// Tests constructing an rgba8 color from channels.
TEST(ColorTest, Rgba8ConstructsFromChannels)
{
	const tr::rgba8 color{40, 80, 120, 160};
	EXPECT_EQ(color.r, 40);
	EXPECT_EQ(color.g, 80);
	EXPECT_EQ(color.b, 120);
	EXPECT_EQ(color.a, 160);
}

/// Tests constructing an rgba8 color from an rgb8 color.
TEST(ColorTest, Rgba8ConstructsFromRgb8)
{
	const tr::rgba8 color{tr::rgb8{0, 64, 128}, 192};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 64);
	EXPECT_EQ(color.b, 128);
	EXPECT_EQ(color.a, 192);
}

/// Tests constructing an rgba8 color from an rgbf color.
TEST(ColorTest, Rgba8ConstructsFromRgbf)
{
	const tr::rgba8 color{tr::rgbf{0, 0.5f, 1.0f}, 63};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 127);
	EXPECT_EQ(color.b, 255);
	EXPECT_EQ(color.a, 63);
}

/// Tests constructing an rgba8 color from an rgbaf color.
TEST(ColorTest, Rgba8ConstructsFromRgbaf)
{
	const tr::rgba8 color{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 127);
	EXPECT_EQ(color.b, 255);
	EXPECT_EQ(color.a, 191);
}

/// Tests constructing an rgba8 color from an hsv color.
TEST(ColorTest, Rgba8ConstructsFromHsv)
{
	const tr::rgba8 color{tr::hsv{0, 1, 1}, 127};
	EXPECT_EQ(color.r, 255);
	EXPECT_EQ(color.g, 0);
	EXPECT_EQ(color.b, 0);
	EXPECT_EQ(color.a, 127);
}

/// Tests constructing an rgbaf color from channels.
TEST(ColorTest, RgbafConstructsFromChannels)
{
	const tr::rgbaf color{0, 0.5f, 1.0f, 0.5f};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 0.5f);
	EXPECT_EQ(color.b, 1.0f);
	EXPECT_EQ(color.a, 0.5f);
}

/// Tests constructing an rgbaf color from an rgb8 color.
TEST(ColorTest, RgbafConstructsFromRgb8)
{
	const tr::rgbaf color{tr::rgb8{0, 63, 127}, 0.75f};
	EXPECT_EQ(color.r, 0.0f);
	EXPECT_NEAR(color.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(color.b, 0.5f, 1 / 255.0f + 1e-3f);
	EXPECT_EQ(color.a, 0.75f);
}

/// Tests constructing an rgbaf color from an rgbf color.
TEST(ColorTest, RgbafConstructsFromRgbf)
{
	const tr::rgbaf color{tr::rgbf{0, 0.5f, 1.0f}, 0.4f};
	EXPECT_EQ(color.r, 0);
	EXPECT_EQ(color.g, 0.5f);
	EXPECT_EQ(color.b, 1.0f);
	EXPECT_EQ(color.a, 0.4f);
}

/// Tests constructing an rgbaf color from an rgba8 color.
TEST(ColorTest, RgbafConstructsFromRgba8)
{
	const tr::rgbaf color{tr::rgba8{0, 63, 127, 191}};
	EXPECT_EQ(color.r, 0.0f);
	EXPECT_NEAR(color.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(color.b, 0.5f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(color.a, 0.75f, 1 / 255.0f + 1e-3f);
}

/// Tests constructing an rgbaf color from an hsv color.
TEST(ColorTest, RgbafConstructsFromHsv)
{
	const tr::rgbaf color{tr::hsv{0, 1, 1}, 0.25f};
	EXPECT_EQ(color.r, 1.0f);
	EXPECT_EQ(color.g, 0.0f);
	EXPECT_EQ(color.b, 0.0f);
	EXPECT_EQ(color.a, 0.25f);
}

/// Tests constructing colors from literals.
TEST(ColorTests, LiteralsConstruct)
{
	EXPECT_EQ("#404040"_rgb8, (tr::rgb8{64, 64, 64}));
	EXPECT_EQ("#0080FF"_rgbf, (tr::rgbf{0, 128 / 255.0f, 1.0f}));
	EXPECT_EQ("#404040"_rgba8, (tr::rgba8{64, 64, 64, 255}));
	EXPECT_EQ("#40404080"_rgba8, (tr::rgba8{64, 64, 64, 128}));
	EXPECT_EQ("#0080FF"_rgbaf, (tr::rgbaf{0, 128 / 255.0f, 1.0f, 1.0f}));
	EXPECT_EQ("#0080FF40"_rgbaf, (tr::rgbaf{0, 128 / 255.0f, 1.0f, 64 / 255.0f}));
}

/// Tests scalar arithmetic with rgb8.
TEST(ColorTests, Rgb8ScalarArithmetic)
{
	tr::rgb8 color{150, 150, 150};
	tr::rgb8 copy{color};

	copy = color + 10;
	color += 10;
	EXPECT_EQ(color, "#A0A0A0"_rgb8);
	EXPECT_EQ(copy, "#A0A0A0"_rgb8);

	copy = color - 10;
	color -= 10;
	EXPECT_EQ(color, "#969696"_rgb8);
	EXPECT_EQ(copy, "#969696"_rgb8);

	copy = color / 2;
	color /= 2.0f;
	EXPECT_EQ(color, "#4B4B4B"_rgb8);
	EXPECT_EQ(copy, "#4B4B4B"_rgb8);

	copy = color * 2.0f;
	color *= 2.0f;
	EXPECT_EQ(color, "#969696"_rgb8);
	EXPECT_EQ(copy, "#969696"_rgb8);
}

/// Tests color arithmetic with rgb8.
TEST(ColorTests, Rgb8ColorArithmetic)
{
	tr::rgb8 color{"#969696"_rgb8};
	tr::rgb8 copy{color};

	copy = color + 10;
	color += 10;
	EXPECT_EQ(color, "#A0A0A0"_rgb8);
	EXPECT_EQ(copy, "#A0A0A0"_rgb8);

	copy = color - 10;
	color -= 10;
	EXPECT_EQ(color, "#969696"_rgb8);
	EXPECT_EQ(copy, "#969696"_rgb8);

	copy = color / 2;
	color /= 2.0f;
	EXPECT_EQ(color, "#4B4B4B"_rgb8);
	EXPECT_EQ(copy, "#4B4B4B"_rgb8);

	copy = color * 2.0f;
	color *= 2.0f;
	EXPECT_EQ(color, "#969696"_rgb8);
	EXPECT_EQ(copy, "#969696"_rgb8);
}

/// Tests scalar arithmetic with rgbf.
TEST(ColorTest, RgbfScalarArithmetic)
{
	tr::rgbf color{0.75f, 0.75f, 0.75f};
	tr::rgbf copy{color};

	copy = color + 0.25f;
	color += 0.25f;
	EXPECT_EQ(color, "#FFFFFF"_rgbf);
	EXPECT_EQ(copy, "#FFFFFF"_rgbf);

	const tr::rgbf sub_result{0.75f, 0.75f, 0.75f};
	copy = color - 0.25f;
	color -= 0.25f;
	EXPECT_EQ(color, sub_result);
	EXPECT_EQ(copy, sub_result);

	const tr::rgbf div_result{0.375f, 0.375f, 0.375f};
	copy = color / 2;
	color /= 2.0f;
	EXPECT_EQ(color, div_result);
	EXPECT_EQ(copy, div_result);

	const tr::rgbf mul_result{0.75f, 0.75f, 0.75f};
	copy = color * 2.0f;
	color *= 2.0f;
	EXPECT_EQ(color, mul_result);
	EXPECT_EQ(copy, mul_result);
}

/// Tests color arithmetic with rgbf.
TEST(ColorTest, RgbfColorArithmetic)
{
	tr::rgbf color{0.75f, 0.75f, 0.75f};
	tr::rgbf copy{color};

	const tr::rgbf addc_result{1.0f, 0.75f, 0.75f};
	copy = color + tr::rgbf{0.25f, 0.0f, 0.0f};
	color += tr::rgbf{0.25f, 0.0f, 0.0f};
	EXPECT_EQ(color, addc_result);
	EXPECT_EQ(copy, addc_result);

	const tr::rgbf subc_result{0.5f, 0.75f, 0.75f};
	copy = color - tr::rgbf{0.5f, 0.0f, 0.0f};
	color -= tr::rgbf{0.5f, 0.0f, 0.0f};
	EXPECT_EQ(color, subc_result);
	EXPECT_EQ(copy, subc_result);

	const tr::rgbf mulc_result{0, 0, 0.75f};
	copy = color * "#0000FF"_rgbf;
	color *= "#0000FF"_rgbf;
	EXPECT_EQ(color, mulc_result);
	EXPECT_EQ(copy, mulc_result);
}

/// Tests scalar arithmetic with rgba8.
TEST(ColorTest, Rgba8ScalarArithmetic)
{
	tr::rgba8 color{150, 150, 150, 150};
	tr::rgba8 copy{color};

	copy = color + 10;
	color += 10;
	EXPECT_EQ(color, "#A0A0A0A0"_rgba8);
	EXPECT_EQ(copy, "#A0A0A0A0"_rgba8);

	copy = color - 10;
	color -= 10;
	EXPECT_EQ(color, "#96969696"_rgba8);
	EXPECT_EQ(copy, "#96969696"_rgba8);

	copy = color / 2;
	color /= 2.0f;
	EXPECT_EQ(color, "#4B4B4B4B"_rgba8);
	EXPECT_EQ(copy, "#4B4B4B4B"_rgba8);

	copy = color * 2.0f;
	color *= 2.0f;
	EXPECT_EQ(color, "#96969696"_rgba8);
	EXPECT_EQ(copy, "#96969696"_rgba8);
}

/// Tests color arithmetic with rgba8.
TEST(ColorTest, Rgba8ColorArithmetic)
{
	tr::rgba8 color{"#96969696"_rgba8};
	tr::rgba8 copy{color};

	copy = color + "#10000000"_rgba8;
	color += "#10000000"_rgba8;
	EXPECT_EQ(color, "#A6969696"_rgba8);
	EXPECT_EQ(copy, "#A6969696"_rgba8);

	copy = color - "#10000000"_rgba8;
	color -= "#10000000"_rgba8;
	EXPECT_EQ(color, "#96969696"_rgba8);
	EXPECT_EQ(copy, "#96969696"_rgba8);

	copy = color * "#0000FFFF"_rgba8;
	color *= "#0000FFFF"_rgba8;
	EXPECT_EQ(color, "#00009696"_rgba8);
	EXPECT_EQ(copy, "#00009696"_rgba8);
}

/// Tests scalar arithmetic with rgbaf.
TEST(ColorTest, RgbafScalarArithmetic)
{
	tr::rgbaf color{0.75f, 0.75f, 0.75f, 0.75f};
	tr::rgbaf copy{color};

	copy = color + 0.25f;
	color += 0.25f;
	EXPECT_EQ(color, "#FFFFFFFF"_rgbaf);
	EXPECT_EQ(copy, "#FFFFFFFF"_rgbaf);

	const tr::rgbaf sub_result{0.75f, 0.75f, 0.75f, 0.75f};
	copy = color - 0.25f;
	color -= 0.25f;
	EXPECT_EQ(color, sub_result);
	EXPECT_EQ(copy, sub_result);

	const tr::rgbaf div_result{0.375f, 0.375f, 0.375f, 0.375f};
	copy = color / 2;
	color /= 2.0f;
	EXPECT_EQ(color, div_result);
	EXPECT_EQ(copy, div_result);

	const tr::rgbaf mul_result{0.75f, 0.75f, 0.75f, 0.75f};
	copy = color * 2.0f;
	color *= 2.0f;
	EXPECT_EQ(color, mul_result);
	EXPECT_EQ(copy, mul_result);
}

/// Tests color arithmetic with rgbaf.
TEST(ColorTest, RgbafColorArithmetic)
{
	tr::rgbaf color{0.75f, 0.75f, 0.75f, 0.75f};
	tr::rgbaf copy{color};

	const tr::rgbaf addc_result{1.0f, 0.75f, 0.75f, 0.75f};
	copy = color + tr::rgbaf{0.25f, 0.0f, 0.0f, 0.0f};
	color += tr::rgbaf{0.25f, 0.0f, 0.0f, 0.0f};
	EXPECT_EQ(color, addc_result);
	EXPECT_EQ(copy, addc_result);

	const tr::rgbaf subc_result{0.5f, 0.75f, 0.75f, 0.75f};
	copy = color - tr::rgbaf{0.5f, 0.0f, 0.0f, 0.0f};
	color -= tr::rgbaf{0.5f, 0.0f, 0.0f, 0.0f};
	EXPECT_EQ(color, subc_result);
	EXPECT_EQ(copy, subc_result);

	const tr::rgbaf mulc_result{0, 0, 0.75f, 0.75f};
	copy = color * "#0000FFFF"_rgbaf;
	color *= "#0000FFFF"_rgbaf;
	EXPECT_EQ(color, mulc_result);
	EXPECT_EQ(copy, mulc_result);
}