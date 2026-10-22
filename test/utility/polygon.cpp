/// @file
/// @brief Tests utility/polygon.hpp.

#include <gtest/gtest.h>
#include <tr/utility/polygon.hpp>

//

/// Tests determining a polygon's winding order.
TEST(PolygonTest, ComputesWindingOrder)
{
	const std::array<glm::vec2, 4> clockwise{{{0, 0}, {4, 0}, {4, 4}, {0, 4}}};
	const std::array<glm::vec2, 4> counterclockwise{{{0, 0}, {0, 4}, {4, 4}, {4, 0}}};

	EXPECT_EQ(tr::polygon_winding_order(clockwise), tr::winding_order::cw);
	EXPECT_EQ(tr::polygon_winding_order(counterclockwise), tr::winding_order::ccw);
}

/// Tests determining whether a polygon is simple.
TEST(PolygonTest, DetectsSimpleAndSelfIntersectingPolygons)
{
	const std::array<glm::vec2, 5> simple{{{0, 0}, {4, 0}, {4, 4}, {2, 2}, {0, 4}}};
	const std::array<glm::vec2, 4> self_intersecting{{{0, 0}, {4, 4}, {0, 4}, {4, 0}}};

	EXPECT_TRUE(tr::simple_polygon(simple));
	EXPECT_FALSE(tr::simple_polygon(self_intersecting));
}

/// Tests that an empty polygon contains no points.
TEST(PolygonTest, EmptyPolygonContainsNoPoints)
{
	EXPECT_FALSE(tr::point_in_polygon({0, 0}, {}));
}

/// Tests that a point inside a polygon is reported as contained.
TEST(PolygonTest, ContainsPointInside)
{
	const std::array<glm::vec2, 4> square{{{0, 0}, {4, 0}, {4, 4}, {0, 4}}};
	EXPECT_TRUE(tr::point_in_polygon({2, 2}, square));
}

/// Tests that the vertices of a polygon are reported as contained.
TEST(PolygonTest, ContainsVertices)
{
	const std::array<glm::vec2, 4> square{{{0, 0}, {4, 0}, {4, 4}, {0, 4}}};
	EXPECT_TRUE(std::ranges::all_of(square, [&](glm::vec2 p) { return tr::point_in_polygon(p, square); }));
}

/// Tests that points on the edges of a polygon are reported as contained.
TEST(PolygonTest, ContainsPointsOnEdges)
{
	const std::array<glm::vec2, 4> square{{{0, 0}, {4, 0}, {4, 4}, {0, 4}}};
	EXPECT_TRUE(tr::point_in_polygon({2, 0}, square));
	EXPECT_TRUE(tr::point_in_polygon({4, 2}, square));
	EXPECT_TRUE(tr::point_in_polygon({2, 4}, square));
	EXPECT_TRUE(tr::point_in_polygon({0, 2}, square));
}

/// Tests that points outside a polygon are not reported as contained.
TEST(PolygonTest, DoesNotContainPointOutside)
{
	const std::array<glm::vec2, 4> square{{{0, 0}, {4, 0}, {4, 4}, {0, 4}}};
	EXPECT_FALSE(tr::point_in_polygon({-2, 2}, square));
	EXPECT_FALSE(tr::point_in_polygon({6, 2}, square));
	EXPECT_FALSE(tr::point_in_polygon({2, -2}, square));
	EXPECT_FALSE(tr::point_in_polygon({2, 6}, square));
}

/// Tests that overlapping polygons are reported as intersecting.
TEST(PolygonTest, OverlappingPolygonsIntersect)
{
	const std::array<glm::vec2, 4> a{{{-4, -4}, {4, -4}, {4, 4}, {-4, 4}}};
	const std::array<glm::vec2, 4> b{{{-1, -1}, {1, -1}, {1, 1}, {-1, 1}}};
	EXPECT_TRUE(tr::intersecting(a, b));
	EXPECT_TRUE(tr::intersecting(b, a));
}

/// Tests that separated polygons are reported as not intersecting.
TEST(PolygonTest, SeparatedPolygonsDoNotIntersect)
{
	const std::array<glm::vec2, 4> a{{{-4, -4}, {4, -4}, {4, 4}, {-4, 4}}};
	const std::array<glm::vec2, 4> b{{{5, 5}, {7, 5}, {7, 7}, {5, 7}}};
	EXPECT_FALSE(tr::intersecting(a, b));
}
