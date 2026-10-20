/// @file
/// @brief Tests utility/circle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/circle.hpp>

//

/// Tests that a point inside a circle is reported as contained.
TEST(CircleTest, ContainsPointInside)
{
	const tr::circle circle{{-50, 100}, 25};
	EXPECT_TRUE(circle.contains(glm::vec2{-60, 90}));
}

/// Tests that a point on the edge of a circle is reported as contained.
TEST(CircleTest, ContainsPointOnEdge)
{
	const tr::circle circle{{-50, 100}, 25};
	EXPECT_TRUE(circle.contains(glm::vec2{-50, 75}));
}

/// Tests that a point outside of a circle is reported as not contained.
TEST(CircleTest, DoesNotContainPointOutside)
{
	const tr::circle circle{{-50, 100}, 25};
	EXPECT_FALSE(circle.contains(glm::vec2{-25, 75}));
}

/// Tests that overlapping circles are reported as intersecting.
TEST(CircleTest, OverlappingCirclesIntersect)
{
	const tr::circle circle0{{100, 100}, 200};
	const tr::circle circle1{{250, 100}, 100};
	EXPECT_TRUE(tr::intersecting(circle0, circle1));
}

/// Tests that tangential circles are reported as intersecting.
TEST(CircleTest, TangentialCirclesIntersect)
{
	const tr::circle circle0{{100, 100}, 200};
	const tr::circle circle1{{400, 100}, 100};
	EXPECT_TRUE(tr::intersecting(circle0, circle1));
}

/// Tests that separated circles are reported as not intersecting.
TEST(CircleTest, SeparatedCirclesDoNotIntersect)
{
	const tr::circle circle0{{100, 100}, 200};
	const tr::circle circle1{{450, 100}, 100};
	EXPECT_FALSE(tr::intersecting(circle0, circle1));
}