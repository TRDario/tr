/// @file
/// @brief Tests utility/atlas_packer.hpp.

#include <gtest/gtest.h>
#include <tr/utility/atlas_packer.hpp>

//

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