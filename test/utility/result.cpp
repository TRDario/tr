/// @file
/// @brief Tests utility/result.hpp.

#include <gtest/gtest.h>
#include <tr/utility/result.hpp>

//

/// Test result type.
using test_result = tr::result<tr::result_values<int, float>, tr::result_errors<char, std::string>>;

//

/// Tests identifying value and error alternative types for the test result type.
TEST(ResultTest, IdentifiesAlternativeTypes)
{
	static_assert(test_result::is_value_type<int>);
	static_assert(test_result::is_value_type<float>);
	static_assert(!test_result::is_value_type<char>);
	static_assert(!test_result::is_value_type<std::string>);

	static_assert(test_result::is_error_type<char>);
	static_assert(test_result::is_error_type<std::string>);
	static_assert(!test_result::is_error_type<int>);
	static_assert(!test_result::is_error_type<float>);
}

/// Tests identifying stored value alternatives in a result.
TEST(ResultTest, IdentifiesValues)
{
	const test_result int_value{42};
	EXPECT_TRUE(int_value.is_value());
	EXPECT_FALSE(int_value.is_error());

	const test_result float_value{2.5f};
	EXPECT_TRUE(float_value.is_value());
	EXPECT_FALSE(float_value.is_error());
}

/// Tests identifying stored error alternatives in a result.
TEST(ResultTest, IdentifiesErrors)
{
	const test_result char_error{'e'};
	EXPECT_FALSE(char_error.is_value());
	EXPECT_TRUE(char_error.is_error());

	const test_result string_error{"error"};
	EXPECT_FALSE(string_error.is_value());
	EXPECT_TRUE(string_error.is_error());
}