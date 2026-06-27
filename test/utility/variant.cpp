///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/variant.hpp.                                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/variant.hpp>

TEST(variant_test, get_if)
{
	std::variant<int, float, char> var1{5.0f};
	const tr::opt_ref<int> var1_int{tr::get_if<int>(var1)};
	const tr::opt_ref<float> var1_float{tr::get_if<float>(var1)};
	EXPECT_FALSE(var1_int.has_ref());
	EXPECT_TRUE(var1_float.has_ref());
	EXPECT_EQ(*var1_float, 5.0f);

	const std::variant<int, float, char> var2{5};
	const tr::opt_ref<const int> var2_int{tr::get_if<int>(var2)};
	const tr::opt_ref<const float> var2_float{tr::get_if<float>(var2)};
	EXPECT_FALSE(var2_float.has_ref());
	EXPECT_TRUE(var2_int.has_ref());
	EXPECT_EQ(*var2_int, 5);

	std::variant<int, float, char> var3{'a'};
	const std::optional<int> var3_int{tr::get_if<int>(std::move(var3))};
	const std::optional<float> var3_char{tr::get_if<char>(std::move(var3))};
	EXPECT_FALSE(var3_int.has_value());
	EXPECT_TRUE(var3_char.has_value());
	EXPECT_EQ(*var3_char, 'a');

	const std::variant<int, float, char> var4{'b'};
	const std::optional<const int> var4_int{tr::get_if<int>(std::move(var4))};
	const std::optional<const char> var4_char{tr::get_if<char>(std::move(var4))};
	EXPECT_FALSE(var4_int.has_value());
	EXPECT_TRUE(var4_char.has_value());
	EXPECT_EQ(*var4_char, 'b');
}

TEST(variant_test, if_is)
{
	bool flag{false};
	const std::variant<int, float, char> var{5.0f};

	tr::if_is<int>(var, [&flag](int a) { flag = true; });
	EXPECT_FALSE(flag);
	tr::if_is<float>(var, [&flag](float a) { flag = true; });
	EXPECT_TRUE(flag);
}