/// @file
/// @brief Tests utility/triangle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/triangle.hpp>

//

/// Tests `tr::triangle::winding_order()`.
TEST(TriangleTest, WindingOrder)
{
	const tr::triangle tri0{{100, 100}, {200, 100}, {150, 150}};
	EXPECT_EQ(tri0.winding_order(), tr::winding_order::cw);

	const tr::triangle tri1{{200, 100}, {100, 100}, {150, 150}};
	EXPECT_EQ(tri1.winding_order(), tr::winding_order::ccw);
}

/// Tests `tr::triangle::contains()`.
TEST(TriangleTest, Contains)
{
	const tr::triangle tri0{{100, 100}, {200, 100}, {150, 150}};
	EXPECT_TRUE(tri0.contains({150, 125}));
	EXPECT_TRUE(tri0.contains(tri0.a));
	EXPECT_TRUE(tri0.contains(tri0.b));
	EXPECT_TRUE(tri0.contains(tri0.c));
	EXPECT_TRUE(tri0.contains({150, 100}));
	EXPECT_TRUE(tri0.contains({175, 125}));
	EXPECT_TRUE(tri0.contains({125, 125}));
	EXPECT_FALSE(tri0.contains({50, 100}));
	EXPECT_FALSE(tri0.contains({185, 135}));
	EXPECT_FALSE(tri0.contains({115, 135}));

	const tr::triangle tri1{{200, 100}, {100, 100}, {150, 150}};
	EXPECT_TRUE(tri1.contains({150, 125}));
	EXPECT_TRUE(tri1.contains(tri1.a));
	EXPECT_TRUE(tri1.contains(tri1.b));
	EXPECT_TRUE(tri1.contains(tri1.c));
	EXPECT_TRUE(tri1.contains({150, 100}));
	EXPECT_TRUE(tri1.contains({175, 125}));
	EXPECT_TRUE(tri1.contains({125, 125}));
	EXPECT_FALSE(tri1.contains({50, 100}));
	EXPECT_FALSE(tri1.contains({185, 135}));
	EXPECT_FALSE(tri1.contains({115, 135}));
}