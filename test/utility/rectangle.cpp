/// @file
/// @brief Tests utility/rectangle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rectangle.hpp>

//

/// @brief Tests default-constructing a rectangle.
TEST(RectangleTest, DefaultConstructs)
{
	const tr::rectangle<float> rect{};
	EXPECT_EQ(rect.tl.x, 0);
	EXPECT_EQ(rect.tl.y, 0);
	EXPECT_EQ(rect.size.x, 0);
	EXPECT_EQ(rect.size.y, 0);
}

/// @brief Tests converting a rectangle from a different-type rectangle.
TEST(RectangleTest, ConstructsFromDifferentRectangle)
{
	const tr::rectangle<int> original{50, 100, 150, 200};
	const tr::rectangle<float> converted{static_cast<tr::rectangle<float>>(original)};
	EXPECT_EQ(converted.tl.x, 50);
	EXPECT_EQ(converted.tl.y, 100);
	EXPECT_EQ(converted.size.x, 150);
	EXPECT_EQ(converted.size.y, 200);
}

/// @brief Tests comparing rectangles.
TEST(RectangleTest, Compares)
{
	const tr::rectangle<float> frect{50, 100, 150, 200};
	const tr::rectangle<int> irect{50, 100, 150, 200};
	const tr::rectangle<unsigned int> urect{49, 100, 150, 200};
	EXPECT_EQ(frect, irect);
	EXPECT_NE(frect, urect);
}

/// @brief Tests converting a rectangle to the set of its edges.
TEST(RectangleTest, ConvertsToEdges)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// @brief Tests converting a rectangle to the set of its edges when a rectangle has negative size.
TEST(RectangleTest, ConvertsToEdgesWithNegativeSize)
{
	const tr::rectangle<float> rectangle{300, 400, -200, -250};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// @brief Tests that a point inside a rectangle is reported as contained.
TEST(RectangleTest, ContainsPointInside)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 200}));
}

/// @brief Tests that points on the vertices of a rectangle are reported as contained.
TEST(RectangleTest, ContainsVertices)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.left, edges.top}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.right, edges.top}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.left, edges.bottom}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{edges.right, edges.bottom}));
}

/// @brief Tests that points on the edges of a rectangle are reported as contained.
TEST(RectangleTest, ContainsPointsOnEdge)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 150}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{300, 300}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{200, 350}));
	EXPECT_TRUE(rectangle.contains(glm::vec2{100, 300}));
}

/// @brief Tests that the the single point of a zero-size rectangle is reported contained.
TEST(RectangleTest, ZeroSizeContainsSinglePoint)
{
	const tr::rectangle<float> rectangle{100, 150, 0, 0};
	EXPECT_TRUE(rectangle.contains(rectangle.tl));
}

/// @brief Tests that a point outside a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainPointOutside)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_FALSE(rectangle.contains(glm::vec2{75, 200}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{325, 200}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{150, 100}));
	EXPECT_FALSE(rectangle.contains(glm::vec2{150, 450}));
}

/// @brief Tests that a rectangle inside a rectangle is reported as contained.
TEST(RectangleTest, ContainsInsideRectangle)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_TRUE(rectangle.contains(tr::rectangle<float>{125, 160, 40, 40}));
}

/// @brief Tests that a copy of a rectangle is reported as contained.
TEST(RectangleTest, ContainsIdenticalRectangle)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_TRUE(rectangle.contains(rectangle));
}

/// @brief Tests that a rectangle partially overlapping a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainOverlappingRectangle)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_FALSE(rectangle.contains(tr::rectangle<double>{150, 200, 200, 250}));
}

/// @brief Tests that a rectangle outside a rectangle is not reported as contained.
TEST(RectangleTest, DoesNotContainOutsideRectangle)
{
	const tr::rectangle<float> rectangle{100, 150, 200, 250};
	EXPECT_FALSE(rectangle.contains(tr::rectangle<float>{500, 500, 200, 250}));
}

/// @brief Tests that overlapping rectangles intersect.
TEST(RectangleTest, OverlappingRectanglesIntersect)
{
	const tr::rectangle<float> a{100, 100, 200, 100};
	const tr::rectangle<float> b{180, 175, 50, 50};
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

/// @brief Tests that touching rectangles do not intersect.
TEST(RectangleTest, TouchingRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{100, 100, 200, 100};
	const tr::rectangle<float> b{300, 150, 200, 100};
	const tr::rectangle<double> c{200, 200, 200, 100};

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

/// @brief Tests that separated rectangles do not intersect.
TEST(RectangleTest, SeparatedRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{100, 100, 200, 100};
	const tr::rectangle<float> b{150, 205, 100, 200};
	EXPECT_FALSE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_FALSE(ab.has_value());
}