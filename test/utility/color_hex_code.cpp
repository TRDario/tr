/// @file
/// @brief Tests utility/color_hex_code.hpp.

#include <gtest/gtest.h>
#include <tr/utility/color_hex_code.hpp>

using namespace tr::color_literals;

//

/// Tests parsing RGB and RGBA hex codes.
TEST(ColorHexCodeTest, ParsesRgbAndRgba)
{
	EXPECT_EQ(tr::from_hex_code<tr::rgb<tr::u8>>("#12aBcF"), (tr::rgb<tr::u8>{0x12, 0xAB, 0xCF}));
	EXPECT_EQ(tr::from_hex_code<tr::rgba<tr::u8>>("#123456"), (tr::rgba<tr::u8>{0x12, 0x34, 0x56, 0xFF}));
	EXPECT_EQ(tr::from_hex_code<tr::rgba<tr::u8>>("#12345678"), (tr::rgba<tr::u8>{0x12, 0x34, 0x56, 0x78}));
	EXPECT_FLOAT_EQ(tr::from_hex_code<tr::rgb<float>>("#FF8000").g, 128.0f / 255.0f);
}

/// Tests rejecting malformed hex codes.
TEST(ColorHexCodeTest, RejectsInvalidCodes)
{
	EXPECT_THROW((void)tr::from_hex_code("#12345"), tr::invalid_hex_code);
	EXPECT_THROW((void)tr::from_hex_code("123456"), tr::invalid_hex_code);
	EXPECT_THROW((void)tr::from_hex_code("#12G456"), tr::invalid_hex_code);
	EXPECT_THROW((void)tr::from_hex_code("#1234567Z"), tr::invalid_hex_code);
}

/// Tests the supported RGB and RGBA hex code literals.
TEST(ColorHexCodeTest, ConstructsFromLiterals)
{
	EXPECT_EQ("#123456"_rgb8, (tr::rgb<tr::u8>{0x12, 0x34, 0x56}));
	EXPECT_FLOAT_EQ("#FF8000"_rgbf.g, 128.0f / 255.0f);
	EXPECT_NEAR("#FF8000"_rgb.r, 1.0, 1e-12);
	EXPECT_EQ("#123456"_rgba8, (tr::rgba<tr::u8>{0x12, 0x34, 0x56, 0xFF}));
	EXPECT_FLOAT_EQ("#12345680"_rgbaf.a, 128.0f / 255.0f);
	EXPECT_NEAR("#12345680"_rgba.a, 128.0 / 255.0, 1e-12);
}
