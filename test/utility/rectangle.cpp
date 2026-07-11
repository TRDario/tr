///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/rectangle.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/rectangle.hpp>

TEST(rectangle_test, construction)
{
	const tr::rectangle<float> default_constructed{};
	EXPECT_EQ(default_constructed.tl.x, 0);
	EXPECT_EQ(default_constructed.tl.y, 0);
	EXPECT_EQ(default_constructed.size.x, 0);
	EXPECT_EQ(default_constructed.size.y, 0);

	const tr::rectangle<float> tl_and_size{{100, 150}, {200, 250}};
	EXPECT_EQ(tl_and_size.tl.x, 100);
	EXPECT_EQ(tl_and_size.tl.y, 150);
	EXPECT_EQ(tl_and_size.size.x, 200);
	EXPECT_EQ(tl_and_size.size.y, 250);

	const tr::rectangle<float> size_only{{100, 150}};
	EXPECT_EQ(size_only.tl.x, 0);
	EXPECT_EQ(size_only.tl.y, 0);
	EXPECT_EQ(size_only.size.x, 100);
	EXPECT_EQ(size_only.size.y, 150);

	const tr::rectangle<float> converted{tr::rectangle<int>{{50, 100}, {150, 200}}};
	EXPECT_EQ(converted.tl.x, 50);
	EXPECT_EQ(converted.tl.y, 100);
	EXPECT_EQ(converted.size.x, 150);
	EXPECT_EQ(converted.size.y, 200);
}

TEST(rectangle_test, comparison)
{
	const tr::rectangle<float> frect{{50, 100}, {150, 200}};
	const tr::rectangle<int> irect{{50, 100}, {150, 200}};
	const tr::rectangle<unsigned int> urect{{49, 100}, {150, 200}};
	EXPECT_EQ(frect, irect);
	EXPECT_NE(frect, urect);
}

TEST(rectangle_test, contains)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_TRUE(rectangle.contains({200, 200}));
	EXPECT_FALSE(rectangle.contains({100, 100}));
	EXPECT_TRUE(rectangle.contains({{150, 150}, {25, 25}}));
	EXPECT_FALSE(rectangle.contains({{100, 100}, {25, 25}}));
}

TEST(rectangle_test, edges)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

TEST(rectangle_test, intersection)
{
	const tr::rectangle<float> a{{100, 100}, {200, 100}};
	const tr::rectangle<float> b{{180, 175}, {50, 50}};
	const tr::rectangle<float> c{{150, 205}, {100, 200}};

	EXPECT_TRUE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_TRUE(ab.has_value());
	EXPECT_EQ(ab->tl.x, 180);
	EXPECT_EQ(ab->tl.y, 175);
	EXPECT_EQ(ab->size.x, 50);
	EXPECT_EQ(ab->size.y, 25);

	EXPECT_FALSE(tr::intersecting(a, c));
	const std::optional<tr::rectangle<float>> ac{tr::intersection(a, c)};
	const std::optional<tr::rectangle<float>> ca{tr::intersection(c, a)};
	EXPECT_EQ(ac, ca);
	EXPECT_FALSE(ac.has_value());

	EXPECT_TRUE(tr::intersecting(b, c));
	const std::optional<tr::rectangle<float>> bc{tr::intersection(b, c)};
	const std::optional<tr::rectangle<float>> cb{tr::intersection(c, b)};
	EXPECT_EQ(bc, cb);
	EXPECT_TRUE(bc.has_value());
	EXPECT_EQ(bc->tl.x, 180);
	EXPECT_EQ(bc->tl.y, 205);
	EXPECT_EQ(bc->size.x, 50);
	EXPECT_EQ(bc->size.y, 20);
}