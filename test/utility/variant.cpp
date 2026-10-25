/// @file
/// @brief Tests utility/variant.hpp.

#include <gtest/gtest.h>
#include <tr/utility/variant.hpp>

//

/// Tests checking a variant alternative.
TEST(VariantTest, ChecksAlternative)
{
	tr::variant<int, float, char> var{5.0f};
	EXPECT_FALSE(var.is<int>());
	EXPECT_TRUE(var.is<float>());
	EXPECT_FALSE(var.is<char>());
}

/// Tests optionally getting a variant alternative.
TEST(VariantTest, GetsOptionalAlternative)
{
	tr::variant<int, float, char> var1{5.0f};
	const tr::opt_ref<int> var1_int{var1.get_if<int>()};
	const tr::opt_ref<float> var1_float{var1.get_if<float>()};
	EXPECT_FALSE(var1_int.has_value());
	ASSERT_TRUE(var1_float.has_value());
	EXPECT_EQ(*var1_float, 5.0f);

	const tr::variant<int, float, char> var2{5};
	const tr::opt_ref<const int> var2_int{var2.get_if<int>()};
	const tr::opt_ref<const float> var2_float{var2.get_if<float>()};
	EXPECT_FALSE(var2_float.has_value());
	ASSERT_TRUE(var2_int.has_value());
	EXPECT_EQ(*var2_int, 5);
}

/// Tests converting a variant to an optional reference to an alternative.
TEST(VariantTest, ConvertsToAlternativeOptRef)
{
	tr::variant<int, float, char> var1{5.0f};
	const tr::opt_ref<int> var1_int{var1};
	const tr::opt_ref<float> var1_float{var1};
	EXPECT_FALSE(var1_int.has_value());
	ASSERT_TRUE(var1_float.has_value());
	EXPECT_EQ(*var1_float, 5.0f);

	const tr::variant<int, float, char> var2{5};
	const tr::opt_ref<const int> var2_int{var2};
	const tr::opt_ref<const float> var2_float{var2};
	EXPECT_FALSE(var2_float.has_value());
	ASSERT_TRUE(var2_int.has_value());
	EXPECT_EQ(*var2_int, 5);
}

/// Tests getting a variant alternative.
TEST(VariantTest, GetsAlternative)
{
	tr::variant<int, float, char> var1{5.0f};
	float& val1{var1.get<float>()};
	EXPECT_EQ(val1, 5.0f);
	val1 = 10.0f;
	EXPECT_EQ(var1.get<float>(), 10.0f);

	const tr::variant<int, float, char> var2{5};
	const int& val2{var2.get<int>()};
	EXPECT_EQ(val2, 5);

	tr::variant<int, float, std::string> var3{"string"};
	std::string val3{std::move(var3).get<std::string>()};
	EXPECT_EQ(val3, "string");
}

/// Tests visiting a variant.
TEST(VariantTest, Visits)
{
	constexpr auto visitor{[]<typename T>(const T& v) { return std::same_as<T, int>; }};

	const tr::variant<int, float, char> var1{5};
	EXPECT_TRUE(var1.visit(visitor));

	const tr::variant<int, float, char> var2{'a'};
	EXPECT_FALSE(var2.visit(visitor));

	const tr::variant<int, float, char> var3{2.5f};
	EXPECT_FALSE(var3.visit(visitor));
}