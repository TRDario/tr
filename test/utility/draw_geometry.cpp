/// @file
/// @brief Tests utility/draw_geometry.hpp.

#include <gtest/gtest.h>
#include <tr/utility/draw_geometry.hpp>

using namespace tr::angle_literals;

//

/// Tests calculating smooth circle and arc vertex counts.
TEST(DrawGeometryTest, CalculatesSmoothVertexCounts)
{
	EXPECT_EQ(tr::smooth_polygon_vertices(0.1), 3);
	EXPECT_EQ(tr::smooth_polygon_vertices(1.0), 7);
	EXPECT_EQ(tr::smooth_arc_vertices(1.0, 1_tr), 7);
	EXPECT_GT(tr::smooth_arc_vertices(10.0, 0.5_tr), tr::smooth_arc_vertices(10.0, 1_tr));
}

/// Tests calculating geometry index counts.
TEST(DrawGeometryTest, CalculatesIndexCounts)
{
	EXPECT_EQ(tr::line_strip_indices(4), 6);
	EXPECT_EQ(tr::line_loop_indices(4), 8);
	EXPECT_EQ(tr::polygon_indices(5), 9);
	EXPECT_EQ(tr::polygon_outline_indices(5), 30);
}

/// Tests emitting line strip indices.
TEST(DrawGeometryTest, FillsLineStripIndices)
{
	constexpr std::array<tr::u16, 6> expected{5, 6, 6, 7, 7, 8};

	std::array<tr::u16, 6> strip;
	const std::array<tr::u16, 6>::iterator strip_end{tr::fill_line_strip_indices(strip.begin(), 4, 5)};
	EXPECT_EQ(strip_end, strip.end());
	EXPECT_EQ(strip, expected);
}

/// Tests emitting line loop indices.
TEST(DrawGeometryTest, FillsLineIndices)
{
	constexpr std::array<tr::u16, 8> expected{5, 6, 6, 7, 7, 8, 8, 5};

	std::array<tr::u16, 8> loop;
	const std::array<tr::u16, 8>::iterator loop_end{tr::fill_line_loop_indices(loop.begin(), 4, 5)};
	EXPECT_EQ(loop_end, loop.end());
	EXPECT_EQ(loop, expected);
}

/// Tests emitting convex polygon fill indices.
TEST(DrawGeometryTest, FillsConvexPolygonIndices)
{
	constexpr std::array<tr::u16, 6> expected{5, 6, 7, 5, 7, 8};

	std::array<tr::u16, 6> fill;
	const std::array<tr::u16, 6>::iterator fill_end{tr::fill_convex_polygon_indices(fill.begin(), 4, 5)};
	EXPECT_EQ(fill_end, fill.end());
	EXPECT_EQ(fill, expected);
}

/// Tests emitting convex polygon outline indices.
TEST(DrawGeometryTest, FillsConvexPolygonOutlineIndices)
{
	constexpr std::array<tr::u16, 24> expected{5, 6, 9, 6, 9, 10, 6, 7, 10, 7, 10, 11, 7, 8, 11, 8, 11, 12, 8, 5, 12, 5, 12, 9};

	std::array<tr::u16, 24> outline;
	const std::array<tr::u16, 24>::iterator outline_end{tr::fill_convex_polygon_outline_indices(outline.begin(), 4, 5)};
	EXPECT_EQ(outline_end, outline.end());
	EXPECT_EQ(outline, expected);
}

/// Tests triangulating a simple polygon with a base offset.
TEST(DrawGeometryTest, FillsSimplePolygonIndices)
{
	constexpr std::array<tr::u16, 6> expected{8, 5, 6, 8, 6, 7};

	const std::array<glm::vec2, 4> square{glm::vec2{0, 0}, glm::vec2{4, 0}, glm::vec2{4, 4}, glm::vec2{0, 4}};
	std::array<tr::u16, 6> polygon;
	const std::array<tr::u16, 6>::iterator polygon_end{tr::fill_simple_polygon_indices(polygon.begin(), square, 5)};

	EXPECT_EQ(polygon_end, polygon.end());
	EXPECT_EQ(polygon, expected);
}

/// Tests emitting basic rectangle vertices.
TEST(DrawGeometryTest, FillsBasicRectangleVertices)
{
	constexpr std::array<glm::vec2, 4> expected{glm::vec2{10, 20}, glm::vec2{10, 60}, glm::vec2{40, 60}, glm::vec2{40, 20}};
	constexpr tr::rectangle<float> rectangle{{10, 20}, {30, 40}};

	std::array<glm::vec2, 4> filled_by_range;
	tr::fill_rectangle_vertices(filled_by_range, rectangle);
	EXPECT_EQ(filled_by_range, expected);

	std::array<glm::vec2, 4> filled_by_iterator;
	tr::fill_rectangle_vertices(filled_by_iterator.begin(), rectangle);
	EXPECT_EQ(filled_by_iterator, expected);
}

/// Tests emitting transformed rectangle vertices.
TEST(DrawGeometryTest, FillsTransformedRectangleVertices)
{
	constexpr std::array<glm::vec2, 4> expected{glm::vec2{20, 40}, glm::vec2{20, 120}, glm::vec2{80, 120}, glm::vec2{80, 40}};
	constexpr tr::rectangle<float> rectangle{{10, 20}, {30, 40}};
	const glm::mat4 transform{glm::scale(glm::mat4{1.0f}, glm::vec3{2.0f, 2.0f, 1.0f})};

	std::array<glm::vec2, 4> filled_by_range;
	tr::fill_rectangle_vertices(filled_by_range, rectangle, transform);
	EXPECT_EQ(filled_by_range, expected);

	std::array<glm::vec2, 4> filled_by_iterator;
	tr::fill_rectangle_vertices(filled_by_iterator.begin(), rectangle, transform);
	EXPECT_EQ(filled_by_iterator, expected);
}

/// Tests emitting rotated rectangle vertices.
TEST(DrawGeometryTest, FillsRotatedRectangleVertices)
{
	std::array<glm::vec2, 4> filled_by_range;
	tr::fill_rectangle_vertices(filled_by_range, glm::vec2{10, 20}, glm::vec2{5, 10}, glm::vec2{20, 10}, 90_deg);
	EXPECT_NEAR(filled_by_range[0].x, 20, 1e-5f);
	EXPECT_NEAR(filled_by_range[0].y, 15, 1e-5f);
	EXPECT_NEAR(filled_by_range[2].x, 10, 1e-5f);
	EXPECT_NEAR(filled_by_range[2].y, 35, 1e-5f);

	std::array<glm::vec2, 4> filled_by_iterator;
	const std::array<glm::vec2, 4>::iterator filled_by_iterator_end{
		tr::fill_rectangle_vertices(filled_by_iterator.begin(), glm::vec2{10, 20}, glm::vec2{5, 10}, glm::vec2{20, 10}, 90_deg),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_range, filled_by_iterator);
}

/// Tests emitting basic rectangle outline vertices.
TEST(DrawGeometryTest, FillsBasicRectangleOutlineVertices)
{
	constexpr std::array<glm::vec2, 8> expected{glm::vec2{9, 19},  glm::vec2{9, 61},  glm::vec2{41, 61}, glm::vec2{41, 19},
												glm::vec2{11, 21}, glm::vec2{11, 59}, glm::vec2{39, 59}, glm::vec2{39, 21}};
	constexpr tr::rectangle<float> rectangle{.tl{10, 20}, .size{30, 40}};

	std::array<glm::vec2, 8> filled_by_range;
	tr::fill_rectangle_outline_vertices(filled_by_range, rectangle, 2.0f);
	EXPECT_EQ(filled_by_range, expected);

	std::array<glm::vec2, 8> filled_by_iterator;
	const std::array<glm::vec2, 8>::iterator filled_by_iterator_end{
		tr::fill_rectangle_outline_vertices(filled_by_iterator.begin(), rectangle, 2.0f),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_iterator, expected);
}

/// Tests emitting transformed rectangle outline vertices.
TEST(DrawGeometryTest, FillsTransformedRectangleOutlineVertices)
{
	constexpr std::array<glm::vec2, 8> expected{glm::vec2{18, 38}, glm::vec2{18, 122}, glm::vec2{82, 122}, glm::vec2{82, 38},
												glm::vec2{22, 42}, glm::vec2{22, 118}, glm::vec2{78, 118}, glm::vec2{78, 42}};
	constexpr tr::rectangle<float> rectangle{.tl{10, 20}, .size{30, 40}};
	const glm::mat4 transform{glm::scale(glm::mat4{1.0f}, glm::vec3{2.0f, 2.0f, 1.0f})};

	std::array<glm::vec2, 8> filled_by_range;
	tr::fill_rectangle_outline_vertices(filled_by_range, rectangle, 2.0f, transform);
	EXPECT_EQ(filled_by_range, expected);

	std::array<glm::vec2, 8> filled_by_iterator;
	const std::array<glm::vec2, 8>::iterator filled_by_iterator_end{
		tr::fill_rectangle_outline_vertices(filled_by_iterator.begin(), rectangle, 2.0f, transform),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_iterator, expected);
}

/// Tests emitting rotated rectangle outline vertices.
TEST(DrawGeometryTest, FillsRotatedRectangleOutlineVertices)
{
	constexpr std::array<glm::vec2, 8> expected{glm::vec2{21, 14}, glm::vec2{9, 14},  glm::vec2{9, 36},  glm::vec2{21, 36},
												glm::vec2{19, 16}, glm::vec2{11, 16}, glm::vec2{11, 34}, glm::vec2{19, 34}};
	const glm::vec2 position{10, 20};
	const glm::vec2 anchor{5, 10};
	const glm::vec2 size{20, 10};

	std::array<glm::vec2, 8> filled_by_range;
	tr::fill_rectangle_outline_vertices(filled_by_range, position, anchor, size, 90_deg, 2.0f);
	for (tr::usize i = 0; i < filled_by_range.size(); ++i) {
		EXPECT_NEAR(filled_by_range[i].x, expected[i].x, 1e-5f);
		EXPECT_NEAR(filled_by_range[i].y, expected[i].y, 1e-5f);
	}

	std::array<glm::vec2, 8> filled_by_iterator;
	const std::array<glm::vec2, 8>::iterator filled_by_iterator_end{
		tr::fill_rectangle_outline_vertices(filled_by_iterator.begin(), position, anchor, size, 90_deg, 2.0f),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_range, filled_by_iterator);
}

/// Tests emitting arc vertices.
TEST(DrawGeometryTest, FillsArcVertices)
{
	constexpr tr::circle<float> circle{.center{2, 3}, .radius = 4};

	std::array<glm::vec2, 3> filled_by_range;
	tr::fill_arc_vertices(filled_by_range, circle, 0_deg, 90_deg);
	EXPECT_NEAR(filled_by_range[0].x, 6, 1e-5f);
	EXPECT_NEAR(filled_by_range[0].y, 3, 1e-5f);
	EXPECT_NEAR(filled_by_range[1].x, 2 + 2 * std::sqrt(3.0f), 1e-5f);
	EXPECT_NEAR(filled_by_range[1].y, 5, 1e-5f);
	EXPECT_NEAR(filled_by_range[2].x, 4, 1e-5f);
	EXPECT_NEAR(filled_by_range[2].y, 3 + 2 * std::sqrt(3.0f), 1e-5f);

	std::array<glm::vec2, 3> filled_by_iterator;
	const std::array<glm::vec2, 3>::iterator filled_by_iterator_end{
		tr::fill_arc_vertices(filled_by_iterator.begin(), filled_by_iterator.size(), circle, 0_deg, 90_deg),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_range, filled_by_iterator);
}

/// Tests emitting regular polygon vertices.
TEST(DrawGeometryTest, FillsRegularPolygonVertices)
{
	constexpr tr::circle<float> circle{.center{0, 0}, .radius = 2};

	std::array<glm::vec2, 4> filled_by_range;
	tr::fill_regular_polygon_vertices(filled_by_range, circle, 0_deg);
	EXPECT_NEAR(filled_by_range[0].x, 2, 1e-5f);
	EXPECT_NEAR(filled_by_range[0].y, 0, 1e-5f);
	EXPECT_NEAR(filled_by_range[1].x, 0, 1e-5f);
	EXPECT_NEAR(filled_by_range[1].y, 2, 1e-5f);

	std::array<glm::vec2, 4> filled_by_iterator;
	const std::array<glm::vec2, 4>::iterator filled_by_iterator_end{
		tr::fill_regular_polygon_vertices(filled_by_iterator.begin(), filled_by_iterator.size(), circle, 0_deg),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_range, filled_by_iterator);
}

/// Tests emitting circle vertices.
TEST(DrawGeometryTest, FillsCircleVertices)
{
	constexpr tr::circle<float> circle{.center{0, 0}, .radius = 2};

	std::array<glm::vec2, 4> filled_by_range;
	tr::fill_circle_vertices(filled_by_range, circle);
	EXPECT_NEAR(filled_by_range[0].x, 2, 1e-5f);
	EXPECT_NEAR(filled_by_range[0].y, 0, 1e-5f);
	EXPECT_NEAR(filled_by_range[1].x, 0, 1e-5f);
	EXPECT_NEAR(filled_by_range[1].y, 2, 1e-5f);

	std::array<glm::vec2, 4> filled_by_iterator;
	const std::array<glm::vec2, 4>::iterator filled_by_iterator_end{
		tr::fill_circle_vertices(filled_by_iterator.begin(), filled_by_iterator.size(), circle),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_range, filled_by_iterator);
}

/// Tests emitting regular polygon outline vertices.
TEST(DrawGeometryTest, FillsRegularPolygonOutlineVertices)
{
	constexpr tr::circle<float> circle{.center{1, 2}, .radius = 5.0f};

	std::array<glm::vec2, 8> filled_by_range;
	tr::fill_regular_polygon_outline_vertices(filled_by_range, circle, 0_deg, 2);
	EXPECT_NEAR(glm::distance(filled_by_range[0], glm::vec2{1, 2}), 6, 1e-5f);
	EXPECT_NEAR(glm::distance(filled_by_range[4], glm::vec2{1, 2}), 4, 1e-5f);

	std::array<glm::vec2, 8> filled_by_iterator;
	const std::array<glm::vec2, 8>::iterator filled_by_iterator_end{
		tr::fill_regular_polygon_outline_vertices(filled_by_iterator.begin(), filled_by_iterator.size() / 2, circle, 0_deg, 2),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_NEAR(glm::distance(filled_by_range[0], glm::vec2{1, 2}), 6, 1e-5f);
	EXPECT_NEAR(glm::distance(filled_by_range[4], glm::vec2{1, 2}), 4, 1e-5f);
}

/// Tests emitting circle outline vertices.
TEST(DrawGeometryTest, FillsCircleOutlineVertices)
{
	constexpr tr::circle<float> circle{.center{1, 2}, .radius = 5.0f};

	std::array<glm::vec2, 8> filled_by_range;
	tr::fill_circle_outline_vertices(filled_by_range, circle, 2);
	EXPECT_NEAR(glm::distance(filled_by_range[0], glm::vec2{1, 2}), 6, 1e-5f);
	EXPECT_NEAR(glm::distance(filled_by_range[4], glm::vec2{1, 2}), 4, 1e-5f);

	std::array<glm::vec2, 8> filled_by_iterator;
	const std::array<glm::vec2, 8>::iterator filled_by_iterator_end{
		tr::fill_circle_outline_vertices(filled_by_iterator.begin(), filled_by_iterator.size() / 2, circle, 2),
	};
	EXPECT_EQ(filled_by_iterator_end, filled_by_iterator.end());
	EXPECT_EQ(filled_by_iterator, filled_by_range);
}