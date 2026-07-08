///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/triangle.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/triangle.hpp>

TEST(triangle_test, winding_order)
{
	const tr::triangle tri0{{100, 100}, {200, 100}, {150, 150}};
	EXPECT_EQ(tri0.winding_order(), tr::winding_order::cw);

	const tr::triangle tri1{{200, 100}, {100, 100}, {150, 150}};
	EXPECT_EQ(tri1.winding_order(), tr::winding_order::ccw);
}

TEST(triangle_test, contains)
{
	const tr::triangle tri{{100, 100}, {200, 100}, {150, 150}};
	EXPECT_TRUE(tri.contains({120, 110}));
	EXPECT_FALSE(tri.contains({150, 160}));
}