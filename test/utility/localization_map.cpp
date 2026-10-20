/// @file
/// @brief Tests utility/localization_map.hpp.

#include <gtest/gtest.h>
#include <tr/utility/localization_map.hpp>

//

/// Tests that a localization map default-constructs empty.
TEST(LocalizationMapTest, DefaultConstructsEmpty)
{
	tr::localization_map map;
	EXPECT_EQ(map.size(), 0);
}

/// Tests constructing a localization map from an iterator pair.
TEST(LocalizationMapTest, ConstructsFromIteratorPair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"second", "Second"}}};

	const tr::localization_map map{data.begin(), data.end()};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

/// Tests constructing a localization map from a range.
TEST(LocalizationMapTest, ConstructsFromRange)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

#if defined(TR_ENABLE_ASSERTS) && defined(GTEST_HAS_DEATH_TEST)
/// Tests that constructing a localization map with duplicate keys triggers an assertion.
TEST(LocalizationMapTest, ConstructingWithDuplicateKeysTriggersAssertion)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"first", "Second"}}};

	EXPECT_DEATH({ const tr::localization_map map{data}; }, "duplicate keys");
}
#endif

/// Tests that copy-constructing a localization map copies values from the original map.
TEST(LocalizationMapTest, CopyConstructorCopiesValues)
{
	const tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map copy{original};
	EXPECT_EQ(original.size(), copy.size());
	EXPECT_EQ(original["first"], copy["first"]);
	EXPECT_NE(original["first"].data(), copy["first"].data());
}

/// Tests that move-constructing a localization map transfers values from the original map.
TEST(LocalizationMapTest, MoveConstructorTransfersValues)
{
	tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map moved{std::move(original)};
	EXPECT_EQ(original.size(), 0);
	EXPECT_EQ(moved.size(), 2);
	EXPECT_TRUE(moved.contains("first"));
	EXPECT_TRUE(moved.contains("second"));
}

/// Tests that copy-assigning a localization map replaces its contents with copied values from the original map.
TEST(LocalizationMapTest, CopyAssignmentReplacesValuesWithCopied)
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

/// Tests that copy-assigning a localization map replaces its contents with transferred values from the original map.
TEST(LocalizationMapTest, MoveAssignmentReplacesValuesWithTransferred)
{
	tr::localization_map map1{{{"first", "First"}, {"second", "Second"}}};
	tr::localization_map map2{{{"third", "Third"}, {"fourth", "Fourth"}}};
	map1 = std::move(map2);
	EXPECT_EQ(map1.size(), 2);
	EXPECT_EQ(map2.size(), 0);
	EXPECT_FALSE(map1.contains("first"));
	EXPECT_TRUE(map1.contains("third"));
}

/// Tests that accessing a localization map with present keys returns associated values.
TEST(LocalizationMapTest, AccessWithPresentKeyReturnsValue)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map["first"], "First");
	EXPECT_EQ(map["second"], "Second");
}

/// Tests that accessing a localization map with a missing key returns the passed key.
TEST(LocalizationMapTest, AccessWithMissingKeyReturnsKey)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	const std::string_view missing_key{"third"};
	EXPECT_EQ(map[missing_key].data(), missing_key.data());
}

/// Tests that clearing a localization map erases its values.
TEST(LocalizationMapTest, ClearEmptiesMap)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	map.clear();
	EXPECT_EQ(map.size(), 0);
}

/// Tests that updating a localization map with a new key adds a new value.
TEST(LocalizationMapTest, UpdateWithNewKeyAddsValue)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update("third", "Third");
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["third"], "Third");
}

/// Tests that updating a localization map with a present key replaces the associated value.
TEST(LocalizationMapTest, UpdateWithPresentKeyReplacesValue)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update("first", "1st");
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map["first"], "1st");
}

/// Tests updating a localization map with an iterator pair to a range of key-value pairs.
TEST(LocalizationMapTest, UpdatesWithIteratorPair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> extra{{{"third", "Third"}, {"first", "1st"}}};

	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update(extra.begin(), extra.end());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
	EXPECT_EQ(map["third"], "Third");
}

/// Tests updating a localization map with a range of key-value pairs.
TEST(LocalizationMapTest, UpdatesWithRange)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update({{"third", "Third"}, {"first", "1st"}});
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
	EXPECT_EQ(map["third"], "Third");
}