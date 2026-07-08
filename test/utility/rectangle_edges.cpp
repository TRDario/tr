///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/rectangle_edges.hpp.                                                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/rectangle_edges.hpp>

TEST(rectangle_edges_test, construction)
{
	const tr::rectangle_edges<float> edges0{};
	EXPECT_EQ(edges0.left, 0);
	EXPECT_EQ(edges0.top, 0);
	EXPECT_EQ(edges0.right, 0);
	EXPECT_EQ(edges0.bottom, 0);

	const tr::rectangle_edges<float> edges1{2.5f};
	EXPECT_EQ(edges1.left, 2.5f);
	EXPECT_EQ(edges1.top, 2.5f);
	EXPECT_EQ(edges1.right, 2.5f);
	EXPECT_EQ(edges1.bottom, 2.5f);

	const tr::rectangle_edges<float> edges2{2.5f, 4.0f};
	EXPECT_EQ(edges2.left, 2.5f);
	EXPECT_EQ(edges2.top, 4.0f);
	EXPECT_EQ(edges2.right, 2.5f);
	EXPECT_EQ(edges2.bottom, 4.0f);

	const tr::rectangle_edges<float> edges3{2.5f, 4.0f, 3.5f};
	EXPECT_EQ(edges3.left, 2.5f);
	EXPECT_EQ(edges3.top, 4.0f);
	EXPECT_EQ(edges3.right, 3.5f);
	EXPECT_EQ(edges3.bottom, 4.0f);

	const tr::rectangle_edges<float> edges4{2.5f, 4.0f, 3.5f, 4.5f};
	EXPECT_EQ(edges4.left, 2.5f);
	EXPECT_EQ(edges4.top, 4.0f);
	EXPECT_EQ(edges4.right, 3.5f);
	EXPECT_EQ(edges4.bottom, 4.5f);
}