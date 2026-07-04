///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/color.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/color.hpp>

using namespace tr::color_literals;

TEST(color_test, rgb8_constructor)
{
	const tr::rgb8 channels{40, 80, 120};
	EXPECT_EQ(channels.r, 40);
	EXPECT_EQ(channels.g, 80);
	EXPECT_EQ(channels.b, 120);

	const tr::rgb8 from_rgbf{tr::rgbf{0, 0.5f, 1.0f}};
	EXPECT_EQ(from_rgbf.r, 0);
	EXPECT_EQ(from_rgbf.g, 127);
	EXPECT_EQ(from_rgbf.b, 255);

	const tr::rgb8 from_rgba8{tr::rgba8{0, 64, 128, 192}};
	EXPECT_EQ(from_rgba8.r, 0);
	EXPECT_EQ(from_rgba8.g, 64);
	EXPECT_EQ(from_rgba8.b, 128);

	const tr::rgb8 from_rgbaf{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(from_rgbaf.r, 0);
	EXPECT_EQ(from_rgbaf.g, 127);
	EXPECT_EQ(from_rgbaf.b, 255);

	constexpr tr::rgb8 from_hsv{tr::hsv{0, 1, 1}};
	EXPECT_EQ(from_hsv.r, 255);
	EXPECT_EQ(from_hsv.g, 0);
	EXPECT_EQ(from_hsv.b, 0);
}

TEST(color_test, rgbf_constructor)
{
	const tr::rgbf channels{0, 0.5f, 1.0f};
	EXPECT_EQ(channels.r, 0);
	EXPECT_EQ(channels.g, 0.5f);
	EXPECT_EQ(channels.b, 1.0f);

	const tr::rgbf from_rgb8{tr::rgb8{0, 63, 127}};
	EXPECT_EQ(from_rgb8.r, 0.0f);
	EXPECT_NEAR(from_rgb8.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(from_rgb8.b, 0.5f, 1 / 255.0f + 1e-3f);

	const tr::rgbf from_rgba8{tr::rgba8{0, 63, 127, 192}};
	EXPECT_EQ(from_rgba8.r, 0.0f);
	EXPECT_NEAR(from_rgba8.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(from_rgba8.b, 0.5f, 1 / 255.0f + 1e-3f);

	const tr::rgbf from_rgbaf{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(from_rgbaf.r, 0);
	EXPECT_EQ(from_rgbaf.g, 0.5f);
	EXPECT_EQ(from_rgbaf.b, 1.0f);

	constexpr tr::rgbf from_hsv{tr::hsv{0, 1, 1}};
	EXPECT_EQ(from_hsv.r, 1.0f);
	EXPECT_EQ(from_hsv.g, 0.0f);
	EXPECT_EQ(from_hsv.b, 0.0f);
}

TEST(color_test, rgba8_constructor)
{
	const tr::rgba8 channels{40, 80, 120, 160};
	EXPECT_EQ(channels.r, 40);
	EXPECT_EQ(channels.g, 80);
	EXPECT_EQ(channels.b, 120);
	EXPECT_EQ(channels.a, 160);

	const tr::rgba8 from_rgb8{tr::rgb8{0, 64, 128}, 192};
	EXPECT_EQ(from_rgb8.r, 0);
	EXPECT_EQ(from_rgb8.g, 64);
	EXPECT_EQ(from_rgb8.b, 128);
	EXPECT_EQ(from_rgb8.a, 192);

	const tr::rgba8 from_rgbf{tr::rgbf{0, 0.5f, 1.0f}, 63};
	EXPECT_EQ(from_rgbf.r, 0);
	EXPECT_EQ(from_rgbf.g, 127);
	EXPECT_EQ(from_rgbf.b, 255);
	EXPECT_EQ(from_rgbf.a, 63);

	const tr::rgba8 from_rgbaf{tr::rgbaf{0, 0.5f, 1.0f, 0.75f}};
	EXPECT_EQ(from_rgbaf.r, 0);
	EXPECT_EQ(from_rgbaf.g, 127);
	EXPECT_EQ(from_rgbaf.b, 255);
	EXPECT_EQ(from_rgbaf.a, 191);

	constexpr tr::rgba8 from_hsv{tr::hsv{0, 1, 1}, 127};
	EXPECT_EQ(from_hsv.r, 255);
	EXPECT_EQ(from_hsv.g, 0);
	EXPECT_EQ(from_hsv.b, 0);
	EXPECT_EQ(from_hsv.a, 127);
}

TEST(color_test, rgbaf_constructor)
{
	const tr::rgbaf channels{0, 0.5f, 1.0f, 0.5f};
	EXPECT_EQ(channels.r, 0);
	EXPECT_EQ(channels.g, 0.5f);
	EXPECT_EQ(channels.b, 1.0f);
	EXPECT_EQ(channels.a, 0.5f);

	const tr::rgbaf from_rgb8{tr::rgb8{0, 63, 127}, 0.75f};
	EXPECT_EQ(from_rgb8.r, 0.0f);
	EXPECT_NEAR(from_rgb8.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(from_rgb8.b, 0.5f, 1 / 255.0f + 1e-3f);
	EXPECT_EQ(from_rgb8.a, 0.75f);

	const tr::rgbaf from_rgbf{tr::rgbf{0, 0.5f, 1.0f}, 0.4f};
	EXPECT_EQ(from_rgbf.r, 0);
	EXPECT_EQ(from_rgbf.g, 0.5f);
	EXPECT_EQ(from_rgbf.b, 1.0f);
	EXPECT_EQ(from_rgbf.a, 0.4f);

	const tr::rgbaf from_rgba8{tr::rgba8{0, 63, 127, 191}};
	EXPECT_EQ(from_rgba8.r, 0.0f);
	EXPECT_NEAR(from_rgba8.g, 0.25f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(from_rgba8.b, 0.5f, 1 / 255.0f + 1e-3f);
	EXPECT_NEAR(from_rgba8.a, 0.75f, 1 / 255.0f + 1e-3f);

	constexpr tr::rgbaf from_hsv{tr::hsv{0, 1, 1}, 0.25f};
	EXPECT_EQ(from_hsv.r, 1.0f);
	EXPECT_EQ(from_hsv.g, 0.0f);
	EXPECT_EQ(from_hsv.b, 0.0f);
	EXPECT_EQ(from_hsv.a, 0.25f);
}

TEST(color_test, literals)
{
	const tr::rgb8 expected_rgb8{64, 64, 64};
	EXPECT_EQ("#404040"_rgb8, expected_rgb8);

	const tr::rgbf expected_rgbf{0, 128 / 255.0f, 1.0f};
	EXPECT_EQ("#0080FF"_rgbf, expected_rgbf);

	const tr::rgba8 expected_rgba8_0{64, 64, 64, 255};
	const tr::rgba8 expected_rgba8_1{64, 64, 64, 128};
	EXPECT_EQ("#404040"_rgba8, expected_rgba8_0);
	EXPECT_EQ("#40404080"_rgba8, expected_rgba8_1);

	const tr::rgbaf expected_rgbaf_0{0, 128 / 255.0f, 1.0f, 1.0f};
	const tr::rgbaf expected_rgbaf_1{0, 128 / 255.0f, 1.0f, 64 / 255.0f};
	EXPECT_EQ("#0080FF"_rgbaf, expected_rgbaf_0);
	EXPECT_EQ("#0080FF40"_rgbaf, expected_rgbaf_1);
}

TEST(color_test, rgb8_arithmetic)
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

	copy = color + "#100000"_rgb8;
	color += "#100000"_rgb8;
	EXPECT_EQ(color, "#A69696"_rgb8);
	EXPECT_EQ(copy, "#A69696"_rgb8);

	copy = color - "#100000"_rgb8;
	color -= "#100000"_rgb8;
	EXPECT_EQ(color, "#969696"_rgb8);
	EXPECT_EQ(copy, "#969696"_rgb8);

	copy = color * "#0000FF"_rgb8;
	color *= "#0000FF"_rgb8;
	EXPECT_EQ(color, "#000096"_rgb8);
	EXPECT_EQ(copy, "#000096"_rgb8);
}

TEST(color_test, rgbf_arithmetic)
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

TEST(color_test, rgba8_arithmetic)
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

TEST(color_test, rgbaf_arithmetic)
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