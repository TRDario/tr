/// @file
/// @brief Tests utility/atlas_packer.hpp.

#include <gtest/gtest.h>
#include <tr/utility/atlas_packer.hpp>

//

/// Tests inserting a fitting entry into an atlas packer.
TEST(AtlasPackerTest, InsertsFitting)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({127, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({127, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({63, 63}, {256, 256}).has_value());
}

/// Tests failing to insert an entry that would not fit into an atlas packer.
TEST(AtlasPackerTest, FailsToInsertNonFitting)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_FALSE(packer.try_insert({255, 127}, {256, 256}).has_value());
}

/// Tests inserting entries into an atlas packer, erasing them, and inserting an entry that would have previously not fit into it.
TEST(AtlasPackerTest, ClearsAndInsertsPreviouslyNonFitting)
{
	tr::atlas_packer packer;
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
	packer.clear();
	EXPECT_TRUE(packer.try_insert({255, 127}, {256, 256}).has_value());
}