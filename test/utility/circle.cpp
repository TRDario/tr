///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/circle.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/circle.hpp>
#include <tr/utility/rng.hpp>

TEST(circle_test, contains)
{
	tr::rng rng;
	const tr::circle circle{rng.generate<glm::vec2>(100000), rng.generate(2.0f, 1000.0f)};

	for (int i = 0; i < 1000000; ++i) {
		EXPECT_TRUE(circle.contains(circle.center + rng.generate<glm::vec2>(rng.generate(0.0f, circle.radius - 0.1f))));
	}
	for (int i = 0; i < 1000000; ++i) {
		EXPECT_FALSE(circle.contains(circle.center + rng.generate<glm::vec2>(rng.generate(circle.radius + 0.1f, 1e12f))));
	}
}

TEST(circle_test, intersecting)
{
	const tr::circle circle0{{100, 100}, 200};
	const tr::circle circle1{{250, 100}, 100};
	const tr::circle circle2{{425, 100}, 100};
	EXPECT_TRUE(tr::intersecting(circle0, circle1));
	EXPECT_TRUE(tr::intersecting(circle1, circle2));
	EXPECT_FALSE(tr::intersecting(circle0, circle2));
}