/// @file
/// @brief Tests utility/optional_variant.hpp.

#include <gtest/gtest.h>
#include <tr/utility/optional_variant.hpp>

//

/// Tests the empty state and resetting an optional variant.
TEST(OptionalVariantTest, IdentifiesEmptyState)
{
	tr::optional_variant<int, float> var;
	EXPECT_FALSE(var.has_value());
	EXPECT_TRUE(var.is<tr::empty_variant_t>());

	var.emplace<int>(42);
	EXPECT_TRUE(var.has_value());
	EXPECT_FALSE(var.is<tr::empty_variant_t>());

	var.reset();
	EXPECT_FALSE(var.has_value());
	EXPECT_TRUE(var.is<tr::empty_variant_t>());
}

/// Tests storing and accessing alternatives in an optional variant.
TEST(OptionalVariantTest, StoresAlternatives)
{
	tr::optional_variant<int, float> int_var{42};
	EXPECT_TRUE(int_var.has_value());
	EXPECT_TRUE(int_var.is<int>());
	EXPECT_EQ(int_var.get<int>(), 42);

	const tr::optional_variant<int, float> float_var{2.5f};
	EXPECT_TRUE(float_var.has_value());
	EXPECT_TRUE(float_var.is<float>());
	EXPECT_EQ(float_var.get<float>(), 2.5f);
}