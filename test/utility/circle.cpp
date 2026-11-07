/// @file
/// @brief Tests utility/circle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/circle.hpp>

//

/// @brief Tests that a point inside a circle is reported as contained.
TEST(CircleTest, ContainsPointInside)
{
	const tr::circle<float> circle{{-50, 100}, 25};
	EXPECT_TRUE(circle.contains(glm::vec2{-60, 90}));
}

/// @brief Tests that a point on the edge of a circle is reported as contained.
TEST(CircleTest, ContainsPointOnEdge)
{
	const tr::circle<float> circle{{-50, 100}, 25};
	EXPECT_TRUE(circle.contains(glm::ivec2{-50, 75}));
}

/// @brief Tests that a point outside of a circle is reported as not contained.
TEST(CircleTest, DoesNotContainPointOutside)
{
	const tr::circle<float> circle{{-50, 100}, 25};
	EXPECT_FALSE(circle.contains(glm::dvec2{-25, 75}));
}

/// @brief Tests that overlapping circles are reported as intersecting.
TEST(CircleTest, OverlappingCirclesIntersect)
{
	const tr::circle<float> circle0{{100, 100}, 200};
	const tr::circle<float> circle1{{250, 100}, 100};
	EXPECT_TRUE(circle0.intersects(circle1));
}

/// @brief Tests that tangential circles are reported as intersecting.
TEST(CircleTest, TangentialCirclesIntersect)
{
	const tr::circle<double> circle0{{100, 100}, 200};
	const tr::circle<double> circle1{{400, 100}, 100};
	EXPECT_TRUE(circle0.intersects(circle1));
}

/// @brief Tests that separated circles are reported as not intersecting.
TEST(CircleTest, SeparatedCirclesDoNotIntersect)
{
	const tr::circle<double> circle0{{450, 100}, 100};
	const tr::circle<float> circle1{};
	const tr::circle<float> circle2{200};
	EXPECT_FALSE(circle0.intersects(circle1));
	EXPECT_FALSE(circle0.intersects(circle2));
}