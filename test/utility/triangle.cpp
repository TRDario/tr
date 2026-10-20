/// @file
/// @brief Tests utility/triangle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/triangle.hpp>

//

/// Triangle test fixture.
class TriangleTest : public ::testing::Test
{
  protected:
	/// First triangle
	const tr::triangle tri0{{100, 100}, {200, 100}, {150, 150}};

	/// Second triangle.
	const tr::triangle tri1{{200, 100}, {100, 100}, {150, 150}};
};

//

/// Tests computing the winding orders of triangles.
TEST_F(TriangleTest, ComputesWindingOrder)
{
	EXPECT_EQ(tri0.winding_order(), tr::winding_order::cw);
	EXPECT_EQ(tri1.winding_order(), tr::winding_order::ccw);
}

/// Tests that a point inside the triangles is reported as contained.
TEST_F(TriangleTest, ContainsInternalPoint)
{
	EXPECT_TRUE(tri0.contains({150, 125}));
	EXPECT_TRUE(tri1.contains({150, 125}));
}

/// Tests that the vertices of the triangles are reported as contained.
TEST_F(TriangleTest, ContainsVertices)
{
	EXPECT_TRUE(tri0.contains(tri0.a));
	EXPECT_TRUE(tri0.contains(tri0.b));
	EXPECT_TRUE(tri0.contains(tri0.c));
	EXPECT_TRUE(tri1.contains(tri1.a));
	EXPECT_TRUE(tri1.contains(tri1.b));
	EXPECT_TRUE(tri1.contains(tri1.c));
}

/// Tests that points on the edges of the triangles are reported as contained.
TEST_F(TriangleTest, ContainsPointsOnEdges)
{
	EXPECT_TRUE(tri0.contains({150, 100}));
	EXPECT_TRUE(tri0.contains({175, 125}));
	EXPECT_TRUE(tri0.contains({125, 125}));
	EXPECT_FALSE(tri0.contains({50, 100}));
	EXPECT_FALSE(tri0.contains({185, 135}));
	EXPECT_FALSE(tri0.contains({115, 135}));
	EXPECT_TRUE(tri1.contains({150, 100}));
	EXPECT_TRUE(tri1.contains({175, 125}));
	EXPECT_TRUE(tri1.contains({125, 125}));
	EXPECT_FALSE(tri1.contains({50, 100}));
	EXPECT_FALSE(tri1.contains({185, 135}));
	EXPECT_FALSE(tri1.contains({115, 135}));
}