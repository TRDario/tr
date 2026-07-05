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

TEST(atlas_rects_test, empty)
{
	tr::atlas_rects<int> rects;
	EXPECT_FALSE(rects.contains(0));
	EXPECT_EQ(rects.entries(), 0);
}

TEST(atlas_rects_test, try_insert_success)
{
	struct rect_with_extra {
		tr::rect2<tr::u16> rect;
		float extra;
	};

	tr::atlas_rects<int> rects;
	tr::atlas_rects<int, rect_with_extra> rects_with_extra;

	const glm::u16vec2 size{127, 127};
	EXPECT_TRUE(rects.try_insert(0, size, {256, 256}).has_value());
	EXPECT_TRUE(rects.contains(0));
	EXPECT_EQ(rects.entries(), 1);
	EXPECT_EQ(rects[0].size, size);

	EXPECT_TRUE(rects_with_extra.try_insert(0, size, {256, 256}, 10).has_value());
	EXPECT_TRUE(rects_with_extra.contains(0));
	EXPECT_EQ(rects_with_extra.entries(), 1);
	EXPECT_EQ(rects_with_extra[0].rect.size, size);
	EXPECT_EQ(rects_with_extra[0].extra, 10);
}

TEST(atlas_rects_test, try_insert_failure)
{
	tr::atlas_rects<int> rects;
	EXPECT_FALSE(rects.try_insert(0, {512, 512}, {256, 256}).has_value());
	EXPECT_FALSE(rects.contains(0));
	EXPECT_EQ(rects.entries(), 0);
}

TEST(atlas_rects_test, clear)
{
	tr::atlas_rects<int> rects;
	EXPECT_TRUE(rects.try_insert(0, {255, 255}, {256, 256}).has_value());
	rects.clear();
	EXPECT_FALSE(rects.contains(0));
	EXPECT_EQ(rects.entries(), 0);
	EXPECT_TRUE(rects.try_insert(1, {255, 255}, {256, 256}).has_value());
	EXPECT_TRUE(rects.contains(1));
	EXPECT_EQ(rects.entries(), 1);
}