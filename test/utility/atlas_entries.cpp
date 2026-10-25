/// @file
/// @brief Tests utility/atlas_entries.hpp.

#include <gtest/gtest.h>
#include <tr/utility/atlas_entries.hpp>

//

/// Tests that defaulted-constructed atlas entries are empty.
TEST(AtlasEntriesTest, DefaultConstructsEmpty)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 0);
}

/// Tests successfully inserting into atlas entries.
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
	EXPECT_TRUE(entries.try_insert(0, size, {256, 256}).has_value());
	EXPECT_EQ(entries.entries(), 1);
	ASSERT_TRUE(entries.contains(0));
	EXPECT_EQ(entries[0].size, size);

	EXPECT_TRUE(entries_with_extra.try_insert(0, size, {256, 256}, 10).has_value());
	EXPECT_EQ(entries_with_extra.entries(), 1);
	ASSERT_TRUE(entries_with_extra.contains(0));
	EXPECT_EQ(entries_with_extra[0].uv.size, size);
	EXPECT_EQ(entries_with_extra[0].extra, 10);
}

/// Tests failing to insert an entry that would not fit into atlas entries.
TEST(AtlasEntriesTest, FailsToInsertNonFitting)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.try_insert(0, {512, 512}, {256, 256}).has_value());
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 0);
}

/// Tests inserting entries into atlas entries, erasing them, and inserting an entry that would have previously not fit into them.
TEST(AtlasEntriesTest, ClearsAndInsertsPreviouslyNonFitting)
{
	tr::atlas_entries<int> entries;
	EXPECT_TRUE(entries.try_insert(0, {255, 255}, {256, 256}).has_value());
	entries.clear();
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 0);
	EXPECT_TRUE(entries.try_insert(1, {255, 255}, {256, 256}).has_value());
	EXPECT_TRUE(entries.contains(1));
	EXPECT_EQ(entries.entries(), 1);
}