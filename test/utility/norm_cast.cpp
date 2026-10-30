/// @file
/// @brief Tests utility/norm_cast.hpp.

#include <gtest/gtest.h>
#include <tr/utility/norm_cast.hpp>
#include <tr/utility/rng.hpp>

//

/// Tests a float-to-float normalized cast.
TEST(NormCastTest, FloatToFloat)
{
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(0.0), 0.0f);
	EXPECT_DOUBLE_EQ(tr::norm_cast<double>(0.0f), 0.0);
	EXPECT_NEAR(tr::norm_cast<float>(0.2), 0.2f, std::numeric_limits<float>::epsilon());
	EXPECT_NEAR(tr::norm_cast<double>(0.2f), 0.2, std::numeric_limits<float>::epsilon());
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(1.0), 1.0f);
	EXPECT_DOUBLE_EQ(tr::norm_cast<double>(1.0f), 1.0);
}

/// Tests an int-to-int sign conversion normalized cast.
TEST(NormCastTest, IntTointSignConversion)
{
	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::i8>::min()), std::numeric_limits<tr::u8>::min());
	EXPECT_EQ(tr::norm_cast<tr::u8>(tr::i8{-1}), 127);
	EXPECT_EQ(tr::norm_cast<tr::u8>(tr::i8{0}), 128);
	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::i8>::max()), std::numeric_limits<tr::u8>::max());

	EXPECT_EQ(tr::norm_cast<tr::i8>(std::numeric_limits<tr::u8>::min()), std::numeric_limits<tr::i8>::min());
	EXPECT_EQ(tr::norm_cast<tr::i8>(tr::u8{127}), -1);
	EXPECT_EQ(tr::norm_cast<tr::i8>(tr::u8{128}), 0);
	EXPECT_EQ(tr::norm_cast<tr::i8>(std::numeric_limits<tr::u8>::max()), std::numeric_limits<tr::i8>::max());
}

/// Tests an int-to-int widening normalized cast.
TEST(NormCastTest, IntTointWidening)
{
	using namespace tr::integer_literals;

	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u8>::min()), std::numeric_limits<tr::u16>::min());
	EXPECT_EQ(tr::norm_cast<tr::u16>(1_u8), 257);
	EXPECT_EQ(tr::norm_cast<tr::u16>(128_u8), 32896);
	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u8>::max()), std::numeric_limits<tr::u16>::max());

	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u8>::min()), std::numeric_limits<tr::u32>::min());
	EXPECT_EQ(tr::norm_cast<tr::u32>(1_u8), 16843009);
	EXPECT_EQ(tr::norm_cast<tr::u32>(128_u8), 2155905152);
	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u8>::max()), std::numeric_limits<tr::u32>::max());

	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u16>::min()), std::numeric_limits<tr::u32>::min());
	EXPECT_EQ(tr::norm_cast<tr::u32>(1_u16), 65537);
	EXPECT_EQ(tr::norm_cast<tr::u32>(32768_u16), 2147516416);
	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u16>::max()), std::numeric_limits<tr::u32>::max());

	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u8>::min()), std::numeric_limits<tr::u64>::min());
	EXPECT_EQ(tr::norm_cast<tr::u64>(1_u8), 72340172838076673_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(128_u8), 9259542123273814144_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u8>::max()), std::numeric_limits<tr::u64>::max());

	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u16>::min()), std::numeric_limits<tr::u64>::min());
	EXPECT_EQ(tr::norm_cast<tr::u64>(1_u16), 281479271743489_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(32768_u16), 9223512776490647552_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u16>::max()), std::numeric_limits<tr::u64>::max());

	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u32>::min()), std::numeric_limits<tr::u64>::min());
	EXPECT_EQ(tr::norm_cast<tr::u64>(1_u32), 4294967297_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(2147483648_u32), 9223372039002259456_u64);
	EXPECT_EQ(tr::norm_cast<tr::u64>(std::numeric_limits<tr::u32>::max()), std::numeric_limits<tr::u64>::max());
}

/// Tests an int-to-int narrowing normalized cast.
TEST(NormCastTest, IntToIntNarrowing)
{
	using namespace tr::integer_literals;

	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u16>::min()), std::numeric_limits<tr::u8>::min());
	EXPECT_EQ(tr::norm_cast<tr::u8>(257_u16), 1);
	EXPECT_EQ(tr::norm_cast<tr::u8>(32896_u16), 128);
	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u16>::max()), std::numeric_limits<tr::u8>::max());

	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u32>::min()), std::numeric_limits<tr::u8>::min());
	EXPECT_EQ(tr::norm_cast<tr::u8>(16843009_u32), 1);
	EXPECT_EQ(tr::norm_cast<tr::u8>(2155905152_u32), 128);
	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u32>::max()), std::numeric_limits<tr::u8>::max());

	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u64>::min()), std::numeric_limits<tr::u8>::min());
	EXPECT_EQ(tr::norm_cast<tr::u8>(72340172838076673_u64), 1);
	EXPECT_EQ(tr::norm_cast<tr::u8>(9259542123273814144_u64), 128);
	EXPECT_EQ(tr::norm_cast<tr::u8>(std::numeric_limits<tr::u64>::max()), std::numeric_limits<tr::u8>::max());

	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u32>::min()), std::numeric_limits<tr::u16>::min());
	EXPECT_EQ(tr::norm_cast<tr::u16>(65537_u32), 1);
	EXPECT_EQ(tr::norm_cast<tr::u16>(2147516416_u32), 32768);
	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u32>::max()), std::numeric_limits<tr::u16>::max());

	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u64>::min()), std::numeric_limits<tr::u16>::min());
	EXPECT_EQ(tr::norm_cast<tr::u16>(281479271743489_u64), 1);
	EXPECT_EQ(tr::norm_cast<tr::u16>(9223512776490647552_u64), 32768_u16);
	EXPECT_EQ(tr::norm_cast<tr::u16>(std::numeric_limits<tr::u64>::max()), std::numeric_limits<tr::u16>::max());

	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u64>::min()), std::numeric_limits<tr::u32>::min());
	EXPECT_EQ(tr::norm_cast<tr::u32>(4294967297_u64), 1);
	EXPECT_EQ(tr::norm_cast<tr::u32>(9223372039002259456_u64), 2147483648_u32);
	EXPECT_EQ(tr::norm_cast<tr::u32>(std::numeric_limits<tr::u64>::max()), std::numeric_limits<tr::u32>::max());
}

/// Tests an int-to-float normalized cast.
TEST(NormCastTest, IntToFloat)
{
	using namespace tr::integer_literals;

	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::u8>::min()), 0.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::i16>::min()), 0.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::u32>::min()), 0.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::i64>::min()), 0.0f);

	EXPECT_NEAR(tr::norm_cast<float>(128_u8), 0.5f, 1 / 127.0);
	EXPECT_NEAR(tr::norm_cast<float>(0_i16), 0.5f, 1 / 32767.0);
	EXPECT_NEAR(tr::norm_cast<float>(2147483648_u32), 0.5f, 1 / 2147483647.0);
	EXPECT_NEAR(tr::norm_cast<float>(0_i64), 0.5f, 1 / 9.22337204e18);

	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::u8>::max()), 1.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::i16>::max()), 1.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::u32>::max()), 1.0f);
	EXPECT_FLOAT_EQ(tr::norm_cast<float>(std::numeric_limits<tr::i64>::max()), 1.0f);
}

/// Tests a float-to-int normalized cast.
TEST(NormCastTest, FloatToInt)
{
	EXPECT_EQ(tr::norm_cast<tr::u8>(0.0f), std::numeric_limits<tr::u8>::min());
	EXPECT_EQ(tr::norm_cast<tr::i16>(0.0f), std::numeric_limits<tr::i16>::min());
	EXPECT_EQ(tr::norm_cast<tr::u32>(0.0f), std::numeric_limits<tr::u32>::min());
	EXPECT_EQ(tr::norm_cast<tr::i64>(0.0f), std::numeric_limits<tr::i64>::min());

	EXPECT_EQ(tr::norm_cast<tr::u8>(0.5f), 127);
	EXPECT_EQ(tr::norm_cast<tr::i16>(0.5f), -1);
	EXPECT_EQ(tr::norm_cast<tr::u32>(0.5f), 2147483647);
	EXPECT_NEAR(tr::norm_cast<double>(tr::norm_cast<tr::i64>(0.5f)), 0.5f, std::numeric_limits<double>::epsilon());

	EXPECT_EQ(tr::norm_cast<tr::u8>(1.0f), std::numeric_limits<tr::u8>::max());
	EXPECT_EQ(tr::norm_cast<tr::i16>(1.0f), std::numeric_limits<tr::i16>::max());
	EXPECT_EQ(tr::norm_cast<tr::u32>(1.0f), std::numeric_limits<tr::u32>::max());
	EXPECT_EQ(tr::norm_cast<tr::i64>(1.0f), std::numeric_limits<tr::i64>::max());
}