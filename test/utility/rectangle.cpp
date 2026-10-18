/// @file
/// @brief Tests utility/rectangle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/rectangle.hpp>

//

/// Tests a default-constructed rectangle.
TEST(RectangleTest, DefaultConstructed)
{
	const tr::rectangle<float> default_constructed{};
	EXPECT_EQ(default_constructed.tl.x, 0);
	EXPECT_EQ(default_constructed.tl.y, 0);
	EXPECT_EQ(default_constructed.size.x, 0);
	EXPECT_EQ(default_constructed.size.y, 0);
}

/// Tests a rectangle constructed from a top-left corner and size.
TEST(RectangleTest, FromTopLeftAndSize)
{
	const tr::rectangle<float> tl_and_size{{100, 150}, {200, 250}};
	EXPECT_EQ(tl_and_size.tl.x, 100);
	EXPECT_EQ(tl_and_size.tl.y, 150);
	EXPECT_EQ(tl_and_size.size.x, 200);
	EXPECT_EQ(tl_and_size.size.y, 250);
}

/// Tests a rectangle constructed from a size.
TEST(RectangleTest, FromSize)
{
	const tr::rectangle<float> size_only{{100, 150}};
	EXPECT_EQ(size_only.tl.x, 0);
	EXPECT_EQ(size_only.tl.y, 0);
	EXPECT_EQ(size_only.size.x, 100);
	EXPECT_EQ(size_only.size.y, 150);
}

/// Tests a rectangle converted from a different-type rectangle.
TEST(RectangleTest, FromDifferentTypeRectangle)
{
	const tr::rectangle<float> converted{tr::rectangle<int>{{50, 100}, {150, 200}}};
	EXPECT_EQ(converted.tl.x, 50);
	EXPECT_EQ(converted.tl.y, 100);
	EXPECT_EQ(converted.size.x, 150);
	EXPECT_EQ(converted.size.y, 200);
}

/// Tests the equality comparison of rectangles.
TEST(RectangleTest, EqualityComparison)
{
	const tr::rectangle<float> frect{{50, 100}, {150, 200}};
	const tr::rectangle<int> irect{{50, 100}, {150, 200}};
	const tr::rectangle<unsigned int> urect{{49, 100}, {150, 200}};
	EXPECT_EQ(frect, irect);
	EXPECT_NE(frect, urect);
}

/// Tests `tr::rectangle::edges()`.
TEST(RectangleTest, ConversionToEdges)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// Tests `tr::rectangle::edges()` when the rectangle has negative size.
TEST(RectangleTest, ConversionToEdgesWithNegativeSize)
{
	const tr::rectangle<float> rectangle{{300, 400}, {-200, -250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_EQ(edges.left, 100);
	EXPECT_EQ(edges.top, 150);
	EXPECT_EQ(edges.right, 300);
	EXPECT_EQ(edges.bottom, 400);
}

/// Tests whether `tr::rectangle::contains()` flags an inside point as contained.
TEST(RectangleTest, ContainsPointInside)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_TRUE(rectangle.contains({200, 200}));
}

/// Tests whether `tr::rectangle::contains()` flags a point on a vertex of the rectangle as contained.
TEST(RectangleTest, ContainsPointOnVertex)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	const tr::rectangle_edges<float> edges{rectangle.edges()};
	EXPECT_TRUE(rectangle.contains({edges.left, edges.top}));
	EXPECT_TRUE(rectangle.contains({edges.right, edges.top}));
	EXPECT_TRUE(rectangle.contains({edges.left, edges.bottom}));
	EXPECT_TRUE(rectangle.contains({edges.right, edges.bottom}));
}

/// Tests whether `tr::rectangle::contains()` flags a point on an edge of the rectangle as contained.
TEST(RectangleTest, ContainsPointOnEdge)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_TRUE(rectangle.contains({200, 150}));
	EXPECT_TRUE(rectangle.contains({300, 300}));
	EXPECT_TRUE(rectangle.contains({200, 350}));
	EXPECT_TRUE(rectangle.contains({100, 300}));
}

/// Tests whether `tr::rectangle::contains()` flags the single point of a zero-size rectangle as contained.
TEST(RectangleTest, ZeroSizeContainsSinglePoint)
{
	const tr::rectangle<float> rectangle{{100, 150}, {0, 0}};
	EXPECT_TRUE(rectangle.contains(rectangle.tl));
}

/// Tests whether `tr::rectangle::contains()` flags an outside point as not contained.
TEST(RectangleTest, DoesNotContainPointOutside)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_FALSE(rectangle.contains({75, 200}));
	EXPECT_FALSE(rectangle.contains({325, 200}));
	EXPECT_FALSE(rectangle.contains({150, 100}));
	EXPECT_FALSE(rectangle.contains({150, 450}));
}

/// Tests whether `tr::rectangle::contains()` flags an inside rectangle as contained.
TEST(RectangleTest, ContainsInsideRectangle)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_TRUE(rectangle.contains({{125, 160}, {40, 40}}));
}

/// Tests whether `tr::rectangle::contains()` flags an identical rectangle as contained.
TEST(RectangleTest, ContainsIdenticalRectangle)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_TRUE(rectangle.contains(rectangle));
}

/// Tests whether `tr::rectangle::contains()` flags an overlapping rectangle as not contained.
TEST(RectangleTest, DoesNotContainOverlappingRectangle)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_FALSE(rectangle.contains({{150, 200}, {200, 250}}));
}

/// Tests whether `tr::rectangle::contains()` flags an outside rectangle as not contained.
TEST(RectangleTest, DoesNotContainOutsideRectangle)
{
	const tr::rectangle<float> rectangle{{100, 150}, {200, 250}};
	EXPECT_FALSE(rectangle.contains({{500, 500}, {200, 250}}));
}

/// Tests that overlapping rectangles are deemed to intersect.
TEST(RectangleTest, OverlappingRectanglesIntersect)
{
	const tr::rectangle<float> a{{100, 100}, {200, 100}};
	const tr::rectangle<float> b{{180, 175}, {50, 50}};
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

/// Tests that touching rectangles are deemed to not intersect.
TEST(RectangleTest, TouchingRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{{100, 100}, {200, 100}};
	const tr::rectangle<float> b{{300, 150}, {200, 100}};
	const tr::rectangle<float> c{{200, 200}, {200, 100}};

	EXPECT_FALSE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_FALSE(ab.has_value());

	EXPECT_FALSE(tr::intersecting(a, c));
	const std::optional<tr::rectangle<float>> ac{tr::intersection(a, c)};
	const std::optional<tr::rectangle<float>> ca{tr::intersection(c, a)};
	EXPECT_EQ(ac, ca);
	EXPECT_FALSE(ac.has_value());
}

/// Tests that separated rectangles are deemed to not intersect.
TEST(RectangleTest, SeparatedRectanglesDoNotIntersect)
{
	const tr::rectangle<float> a{{100, 100}, {200, 100}};
	const tr::rectangle<float> b{{150, 205}, {100, 200}};
	EXPECT_FALSE(tr::intersecting(a, b));
	const std::optional<tr::rectangle<float>> ab{tr::intersection(a, b)};
	const std::optional<tr::rectangle<float>> ba{tr::intersection(b, a)};
	EXPECT_EQ(ab, ba);
	EXPECT_FALSE(ab.has_value());
}