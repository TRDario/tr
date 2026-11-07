/// @file
/// @brief Tests utility/atlas_entries.hpp.

#include <gtest/gtest.h>
#include <tr/utility/atlas_entries.hpp>

//

/// @brief Tests that defaulted-constructed atlas entries are empty.
TEST(AtlasEntriesTest, DefaultConstructsEmpty)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.size(), 0);
}

/// @brief Tests successfully inserting into atlas entries.
TEST(AtlasEntriesTest, InsertsFitting)
{
	struct entry_with_extra
	{
		tr::rectangle<tr::u16> uv;
		float extra;
	};

	tr::atlas_entries<int> entries;
	tr::atlas_entries<int, entry_with_extra> entries_with_extra;

	const glm::u16vec2 size{127, 127};
	EXPECT_TRUE(entries.try_insert(0, size, glm::u16vec2{256, 256}));
	EXPECT_EQ(entries.size(), 1);
	ASSERT_TRUE(entries.contains(0));
	EXPECT_EQ(entries[0].size, size);

	EXPECT_TRUE(entries_with_extra.try_insert(0, size, glm::u16vec2{256, 256}, 10.0f));
	EXPECT_EQ(entries_with_extra.size(), 1);
	ASSERT_TRUE(entries_with_extra.contains(0));
	EXPECT_EQ(entries_with_extra[0].uv.size, size);
	EXPECT_EQ(entries_with_extra[0].extra, 10.0f);
}

/// @brief Tests failing to insert an entry that would not fit into atlas entries.
TEST(AtlasEntriesTest, FailsToInsertNonFitting)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.try_insert(0, glm::u16vec2{512, 512}, glm::u16vec2{256, 256}));
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.size(), 0);
}

/// @brief Tests inserting entries into atlas entries, erasing them, and inserting an entry that would have previously not fit into them.
TEST(AtlasEntriesTest, ClearsAndInsertsPreviouslyNonFitting)
{
	tr::atlas_entries<int> entries;
	EXPECT_TRUE(entries.try_insert(0, glm::u16vec2{255, 255}, glm::u16vec2{256, 256}));
	entries.clear();
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.size(), 0);
	EXPECT_TRUE(entries.try_insert(1, glm::u16vec2{255, 255}, glm::u16vec2{256, 256}));
	EXPECT_TRUE(entries.contains(1));
	EXPECT_EQ(entries.size(), 1);
}