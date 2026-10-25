/// @file
/// @brief Tests utility/rectangle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rectangle.hpp>

//

/// Tests converting a rectangle from a different-type rectangle.
TEST(RectangleTest, ConvertsFromDifferentRectangle)
{
	const tr::rectangle<int> original{.tl{50, 100}, .size{150, 200}};
	const tr::rectangle<float> converted{static_cast<tr::rectangle<float>>(original)};
	EXPECT_EQ(converted.tl.x, 50);
	EXPECT_EQ(converted.tl.y, 100);
	EXPECT_EQ(converted.size.x, 150);
	EXPECT_EQ(converted.size.y, 200);
}

/// Tests comparing rectangles.
TEST(RectangleTest, Compares)
{
	const tr::rectangle<float> frect{.tl{50, 100}, .size{150, 200}};
	const tr::rectangle<int> irect{.tl{50, 100}, .size{150, 200}};
	const tr::rectangle<unsigned int> urect{.tl{49, 100}, .size{150, 200}};
	EXPECT_EQ(frect, irect);
	EXPECT_NE(frect, urect);
}

/// Tests converting a rectangle to the set of its edges.
TEST(RectangleTest, ConvertsToEdges)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// Tests converting a rectangle to the set of its edges when a rectangle has negative size.
TEST(RectangleTest, ConvertsToEdgesWithNegativeSize)
{
	const tr::rectangle<float> rectangle{.tl{300, 400}, .size{-200, -250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// Tests that a point inside a rectangle is reported as contained.
TEST(RectangleTest, ContainsPointInside)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 200}));
}

/// Tests that points on the vertices of a rectangle are reported as contained.
TEST(RectangleTest, ContainsVertices)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.left, edges.top}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.right, edges.top}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.left, edges.bottom}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.right, edges.bottom}));
}

/// Tests that points on the edges of a rectangle are reported as contained.
TEST(RectangleTest, ContainsPointsOnEdge)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 150}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{300, 300}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 350}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{100, 300}));
}

/// Tests that the the single point of a zero-size rectangle is reported contained.
TEST(RectangleTest, ZeroSizeContainsSinglePoint)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{0, 0}};
	EXPECT_TRUE(rectangle.contains(rectangle.tl));
}

/// Tests that a point outside a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainPointOutside)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_FALSE(rectangle.contains(glm::vec2{75, 200}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{325, 200}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{150, 100}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{150, 450}));
}

/// Tests that a rectangle inside a rectangle is reported as contained.
TEST(RectangleTest, ContainsInsideRectangle)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_TRUE(rectangle.contains(tr::rectangle<float>{.tl{125, 160}, .size{40, 40}}));
}

/// Tests that a copy of a rectangle is reported as contained.
TEST(RectangleTest, ContainsIdenticalRectangle)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_TRUE(rectangle.contains(rectangle));
}

/// Tests that a rectangle partially overlapping a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainOverlappingRectangle)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_FALSE(rectangle.contains(tr::rectangle<double>{.tl{150, 200}, .size{200, 250}}));
}

/// Tests that a rectangle outside a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainOutsideRectangle)
{
	const tr::rectangle<float> rectangle{.tl{100, 150}, .size{200, 250}};
	EXPECT_FALSE(rectangle.contains(tr::rectangle<float>{.tl{500, 500}, .size{200, 250}}));
}

/// Tests that overlapping rectangles intersect.
TEST(RectangleTest, OverlappingRectanglesIntersect)
{
	const tr::rectangle<float> a{.tl{100, 100}, .size{200, 100}};
	const tr::rectangle<float> b{.tl{180, 175}, .size{50, 50}};
	EXPECT_TRUE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	ASSERT_TRUE(ab.has_value());
	EXPECT_EQ(ab->tl.x, 180);
	EXPECT_EQ(ab->tl.y, 175);
	EXPECT_EQ(ab->size.x, 50);
	EXPECT_EQ(ab->size.y, 25);
}

/// Tests that touching rectangles do not intersect.
TEST(RectangleTest, TouchingRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{.tl{100, 100}, .size{200, 100}};
	const tr::rectangle<float> b{.tl{300, 150}, .size{200, 100}};
	const tr::rectangle<double> c{.tl{200, 200}, .size{200, 100}};

	EXPECT_FALSE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_FALSE(ab.has_value());

	EXPECT_FALSE(tr::intersecting(a, c));
	const std::optional<tr::rectangle<double>> ac{tr::intersection(a, c)};
	const std::optional<tr::rectangle<double>> ca{tr::intersection(c, a)};
	EXPECT_EQ(ac, ca);
	EXPECT_FALSE(ac.has_value());
}

/// Tests that separated rectangles do not intersect.
TEST(RectangleTest, SeparatedRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{.tl{100, 100}, .size{200, 100}};
	const tr::rectangle<float> b{.tl{150, 205}, .size{100, 200}};
	EXPECT_FALSE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_FALSE(ab.has_value());
}