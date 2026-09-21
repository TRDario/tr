/// @file
/// @brief Tests utility/localization_map.hpp.

#include <gtest/gtest.h>
#include <tr/utility/localization_map.hpp>

//

TEST(localization_map_test, default_constructor)
{
	tr::localization_map map;
	EXPECT_EQ(map.size(), 0);
}

TEST(localization_map_test, iterator_pair_constructor)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"second", "Second"}}};

	const tr::localization_map map{data.begin(), data.end()};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

TEST(localization_map_test, range_constructor)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

#if defined(TR_ENABLE_ASSERTS) && defined(GTEST_HAS_DEATH_TEST)
TEST(localization_map_test, constructor_duplicate_keys)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"first", "Second"}}};

	EXPECT_DEATH({ const tr::localization_map map{data}; }, "duplicate keys");
}
#endif

TEST(localization_map_test, copy_constructor)
{
	const tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map copy{original};
	EXPECT_EQ(original.size(), copy.size());
	EXPECT_EQ(original["first"], copy["first"]);
	EXPECT_NE(original["first"].data(), copy["first"].data());
}

TEST(localization_map_test, move_constructor)
{
	tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map moved{std::move(original)};
	EXPECT_EQ(original.size(), 0);
	EXPECT_EQ(moved.size(), 2);
	EXPECT_TRUE(moved.contains("first"));
	EXPECT_TRUE(moved.contains("second"));
}

TEST(localization_map_test, copy_assignment)
{
	tr::localization_map map1{{{"first", "First"}, {"second", "Second"}}};
	tr::localization_map map2{{{"third", "Third"}, {"fourth", "Fourth"}}};
	map1 = map2;
	EXPECT_EQ(map1.size(), map2.size());
	EXPECT_FALSE(map1.contains("first"));
	EXPECT_TRUE(map1.contains("third"));
	EXPECT_EQ(map1["third"], map2["third"]);
	EXPECT_NE(map1["third"].data(), map2["third"].data());
}

TEST(localization_map_test, move_assignment)
{
	tr::localization_map map1{{{"first", "First"}, {"second", "Second"}}};
	tr::localization_map map2{{{"third", "Third"}, {"fourth", "Fourth"}}};
	map1 = std::move(map2);
	EXPECT_EQ(map1.size(), 2);
	EXPECT_EQ(map2.size(), 0);
	EXPECT_FALSE(map1.contains("first"));
	EXPECT_TRUE(map1.contains("third"));
}

TEST(localization_map_test, subscript)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	const std::string_view missing_key{"third"};
	EXPECT_EQ(map["first"], "First");
	EXPECT_EQ(map["second"], "Second");
	EXPECT_EQ(map[missing_key].data(), missing_key.data());
}

TEST(localization_map_test, clear)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	map.clear();
	EXPECT_EQ(map.size(), 0);
}

TEST(localization_map_test, update_single)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update("third", "Third");
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["third"], "Third");
	map.update("first", "1st");
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
}

TEST(localization_map_test, update_iterator_pair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> extra{{{"third", "Third"}, {"first", "1st"}}};

	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update(extra.begin(), extra.end());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["third"], "Third");
	map.update("first", "1st");
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
}

TEST(localization_map_test, update_range)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update({{"third", "Third"}, {"first", "1st"}});
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["third"], "Third");
}