///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/atlas_packer.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/atlas_packer.hpp>

TEST(atlas_packer_test, try_insert_success)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({127, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({127, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
}

TEST(atlas_packer_test, try_insert_failure)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_FALSE(packer.try_insert({255, 127}, {256, 256}).has_value());
}

TEST(atlas_packer_test, clear)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	packer.clear();
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
}

//

TEST(atlas_entries_test, empty)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 0);
}

TEST(atlas_entries_test, try_insert_success)
{
	struct entry_with_extra {
		tr::rectangle<tr::u16> uv;
		float extra;
	};

	tr::atlas_entries<int> entries;
	tr::atlas_entries<int, entry_with_extra> entries_with_extra;

	const glm::u16vec2 size{127, 127};
	EXPECT_TRUE(entries.try_insert(0, size, {256, 256}).has_value());
	EXPECT_TRUE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 1);
	EXPECT_EQ(entries[0].size, size);

	EXPECT_TRUE(entries_with_extra.try_insert(0, size, {256, 256}, 10).has_value());
	EXPECT_TRUE(entries_with_extra.contains(0));
	EXPECT_EQ(entries_with_extra.entries(), 1);
	EXPECT_EQ(entries_with_extra[0].uv.size, size);
	EXPECT_EQ(entries_with_extra[0].extra, 10);
}

TEST(atlas_entries_test, try_insert_failure)
{
	tr::atlas_entries<int> entries;
	EXPECT_FALSE(entries.try_insert(0, {512, 512}, {256, 256}).has_value());
	EXPECT_FALSE(entries.contains(0));
	EXPECT_EQ(entries.entries(), 0);
}

TEST(atlas_entries_test, clear)
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