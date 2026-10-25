/// @file
/// @brief Tests utility/line.hpp.

#include <gtest/gtest.h>
#include <tr/utility/line.hpp>

//

/// Tests line segment lengths.
TEST(LineTest, MeasuresSegments)
{
	const tr::line_segment<float> float_segment{{0.0f, 0.0f}, {3.0f, 4.0f}};
	EXPECT_FLOAT_EQ(float_segment.length(), 5.0f);
	EXPECT_FLOAT_EQ(float_segment.length2(), 25.0f);

	const tr::line_segment<double> double_segment{{0.0, 0.0}, {3.0, 4.0}};
	EXPECT_DOUBLE_EQ(double_segment.length(), 5.0);
	EXPECT_DOUBLE_EQ(double_segment.length2(), 25.0);
}

/// Tests line segment closest-point projection.
TEST(LineTest, ProjectsPoints)
{
	const tr::line_segment<float> float_segment{{0.0f, 0.0f}, {3.0f, 4.0f}};
	const glm::vec2 float_point{float_segment.closest_point(glm::vec2{1.0f, 3.0f})};
	EXPECT_NEAR(float_point.x, 1.8f, 1e-6f);
	EXPECT_NEAR(float_point.y, 2.4f, 1e-6f);

	const glm::dvec2 mixed_point{float_segment.closest_point(glm::dvec2{1.0, 3.0})};
	EXPECT_NEAR(mixed_point.x, 1.8, 1e-12);
	EXPECT_NEAR(mixed_point.y, 2.4, 1e-12);

	const tr::line_segment<double> double_segment{{0.0, 0.0}, {3.0, 4.0}};
	const glm::dvec2 double_point{double_segment.closest_point(glm::dvec2{1.0, 3.0})};
	EXPECT_NEAR(double_point.x, 1.8, 1e-12);
	EXPECT_NEAR(double_point.y, 2.4, 1e-12);
}

/// Tests detection of collinear points.
TEST(LineTest, DetectsCollinear)
{
	EXPECT_TRUE(tr::collinear(glm::dvec2{0.0, 0.0}, glm::vec2{1.0f, 1.0f}, glm::dvec2{2.0, 2.0}));
	EXPECT_FALSE(tr::collinear(glm::dvec2{0.0, 0.0}, glm::vec2{1.0f, 1.0f}, glm::dvec2{2.0, 3.0}));
}

/// Tests detection and computation of intersections.
TEST(LineTest, ComputesIntersections)
{
	const tr::line_segment<double> horizontal{{0.0, 1.0}, {2.0, 1.0}};
	const tr::line_segment<double> vertical{{1.0, 0.0}, {1.0, 2.0}};
	EXPECT_TRUE(tr::intersecting(horizontal, vertical));
	const std::optional<glm::dvec2> segments_point{tr::intersection(horizontal, vertical)};
	EXPECT_TRUE(tr::intersecting(horizontal, vertical));
	ASSERT_TRUE(segments_point.has_value());
	EXPECT_DOUBLE_EQ(segments_point->x, 1.0);
	EXPECT_DOUBLE_EQ(segments_point->y, 1.0);
	EXPECT_FALSE(tr::intersection(horizontal, tr::line_segment<double>{{3.0, 0.0}, {3.0, 2.0}}));

	const tr::line_segment<float> mixed_horizontal{{0.0f, 1.0f}, {2.0f, 1.0f}};
	const std::optional<glm::dvec2> mixed_segments_points{tr::intersection(mixed_horizontal, vertical)};
	EXPECT_TRUE(tr::intersecting(mixed_horizontal, vertical));
	ASSERT_TRUE(mixed_segments_points.has_value());
	EXPECT_DOUBLE_EQ(mixed_segments_points->x, 1.0);
	EXPECT_DOUBLE_EQ(mixed_segments_points->y, 1.0);

	const std::optional<glm::dvec2> angles_point{
		tr::intersection(glm::dvec2{0.0, 0.0}, tr::degs(0.0), glm::dvec2{1.0, -1.0}, tr::degs(90.0))};
	ASSERT_TRUE(angles_point.has_value());
	EXPECT_DOUBLE_EQ(angles_point->x, 1.0);
	EXPECT_DOUBLE_EQ(angles_point->y, 0.0);

	const std::optional<glm::dvec2> angle_segment_point{
		tr::intersection(glm::dvec2{0.0, 0.0}, tr::degs(0.0), tr::line_segment<double>{{1.0, -1.0}, {1.0, 1.0}})};
	ASSERT_TRUE(angle_segment_point.has_value());
	EXPECT_DOUBLE_EQ(angle_segment_point->x, 1.0);
	EXPECT_DOUBLE_EQ(angle_segment_point->y, 0.0);

	const std::optional<glm::dvec2> mixed_angles_point{
		tr::intersection(glm::vec2{0.0f, 0.0f}, tr::degs(0.0f), glm::dvec2{1.0, -1.0}, tr::degs(90.0))};
	ASSERT_TRUE(mixed_angles_point.has_value());
	EXPECT_DOUBLE_EQ(mixed_angles_point->x, 1.0);
	EXPECT_DOUBLE_EQ(mixed_angles_point->y, 0.0);

	const std::optional<glm::dvec2> mixed_angle_segment_point{
		tr::intersection(glm::vec2{0.0f, 0.0f}, tr::degs(0.0f), tr::line_segment<double>{{1.0, -1.0}, {1.0, 1.0}})};
	ASSERT_TRUE(mixed_angle_segment_point.has_value());
	EXPECT_DOUBLE_EQ(mixed_angle_segment_point->x, 1.0);
	EXPECT_DOUBLE_EQ(mixed_angle_segment_point->y, 0.0);

	const std::optional<glm::dvec2> mixed_point_angle_point{
		tr::intersection(glm::vec2{0.0f, 0.0f}, tr::degs(0.0f), glm::dvec2{1.0, -1.0}, glm::vec2{1.0f, 1.0f})};
	ASSERT_TRUE(mixed_point_angle_point.has_value());
	EXPECT_DOUBLE_EQ(mixed_point_angle_point->x, 1.0);
	EXPECT_DOUBLE_EQ(mixed_point_angle_point->y, 0.0);

	const std::optional<glm::dvec2> mixed_points_point{
		tr::insersection(glm::vec2{0.0f, 0.0f}, glm::vec2{2.0f, 0.0f}, glm::dvec2{1.0, -1.0}, glm::dvec2{1.0, 1.0})};
	ASSERT_TRUE(mixed_points_point.has_value());
	EXPECT_DOUBLE_EQ(mixed_points_point->x, 1.0);
	EXPECT_DOUBLE_EQ(mixed_points_point->y, 0.0);
}