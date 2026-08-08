/// @file
/// @brief Tests utility/hash_map.hpp.

#include <gtest/gtest.h>
#include <tr/utility/hash_map.hpp>

//

/// Hash map test fixture.
class hash_map_test : public ::testing::Test
{
  protected:
	/// First test map.
	tr::string_flat_map<int> map1{{"one", 1}, {"two", 2}};

	/// Second test map.
	tr::string_node_map<int> map2{{"four", 4}, {"five", 5}};
};

//

TEST_F(hash_map_test, get)
{
	EXPECT_EQ(tr::get(map1, "one"), 1);
	EXPECT_EQ(tr::get(std::as_const(map1), "two"), 2);
	EXPECT_EQ(tr::get(map2, "four"), 4);
	EXPECT_EQ(tr::get(std::as_const(map2), "five"), 5);

	tr::get(map1, "one") = 10;
	tr::get(map2, "four") = 40;
	EXPECT_EQ(tr::get(map1, "one"), 10);
	EXPECT_EQ(tr::get(map2, "four"), 40);
}

TEST_F(hash_map_test, try_get)
{
	EXPECT_EQ(*tr::try_get(map1, "one"), 1);
	EXPECT_EQ(*tr::try_get(std::as_const(map1), "two"), 2);
	EXPECT_EQ(*tr::try_get(map2, "four"), 4);
	EXPECT_EQ(*tr::try_get(std::as_const(map2), "five"), 5);
	EXPECT_FALSE(tr::try_get(map1, "three").has_ref());
	EXPECT_FALSE(tr::try_get(std::as_const(map1), "three").has_ref());
	EXPECT_FALSE(tr::try_get(map2, "three").has_ref());
	EXPECT_FALSE(tr::try_get(std::as_const(map2), "three").has_ref());
}