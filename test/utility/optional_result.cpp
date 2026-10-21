/// @file
/// @brief Tests utility/optional_result.hpp.

#include <gtest/gtest.h>
#include <tr/utility/optional_result.hpp>

//

/// Test optional result type.
using test_optional_result = tr::optional_result<tr::result_values<int, float>, tr::result_errors<char, std::string>>;

//

/// Tests identifying value and error alternative types for the test optional result type.
TEST(OptionalResultTest, IdentifiesAlternativeTypes)
{
	static_assert(test_optional_result::is_value_type<int>);
	static_assert(test_optional_result::is_value_type<float>);
	static_assert(!test_optional_result::is_value_type<char>);
	static_assert(!test_optional_result::is_value_type<std::string>);

	static_assert(test_optional_result::is_error_type<char>);
	static_assert(test_optional_result::is_error_type<std::string>);
	static_assert(!test_optional_result::is_error_type<int>);
	static_assert(!test_optional_result::is_error_type<float>);
}

/// Tests the empty state and resetting an optional result.
TEST(OptionalResultTest, IdentifiesEmptyState)
{
	test_optional_result result;
	EXPECT_TRUE(result.is_empty());
	EXPECT_FALSE(result.is_value());
	EXPECT_FALSE(result.is_error());

	result.emplace<int>(42);
	EXPECT_FALSE(result.is_empty());

	result.reset();
	EXPECT_TRUE(result.is_empty());
	EXPECT_FALSE(result.is_value());
	EXPECT_FALSE(result.is_error());
}

/// Tests identifying stored value alternatives in an optional result.
TEST(OptionalResultTest, IdentifiesValues)
{
	const test_optional_result int_value{42};
	EXPECT_FALSE(int_value.is_empty());
	EXPECT_TRUE(int_value.is_value());
	EXPECT_FALSE(int_value.is_error());

	const test_optional_result float_value{2.5f};
	EXPECT_FALSE(float_value.is_empty());
	EXPECT_TRUE(float_value.is_value());
	EXPECT_FALSE(float_value.is_error());
}

/// Tests identifying stored error alternatives in an optional result.
TEST(OptionalResultTest, IdentifiesErrors)
{
	const test_optional_result char_error{'e'};
	EXPECT_FALSE(char_error.is_empty());
	EXPECT_FALSE(char_error.is_value());
	EXPECT_TRUE(char_error.is_error());

	const test_optional_result string_error{"error"};
	EXPECT_FALSE(string_error.is_empty());
	EXPECT_FALSE(string_error.is_value());
	EXPECT_TRUE(string_error.is_error());
}