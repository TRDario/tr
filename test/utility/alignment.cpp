///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/alignment.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/alignment.hpp>

TEST(alignment_test, composition)
{
	EXPECT_EQ(tr::halign::left | tr::valign::center, tr::align::cl);
	EXPECT_EQ(tr::valign::bottom | tr::halign::right, tr::align::br);
}

TEST(alignment_test, decomposition)
{
	EXPECT_EQ(tr::to_halign(tr::align::tl), tr::halign::left);
	EXPECT_EQ(tr::to_halign(tr::align::cr), tr::halign::right);
	EXPECT_EQ(tr::to_valign(tr::align::tc), tr::valign::top);
	EXPECT_EQ(tr::to_valign(tr::align::bl), tr::valign::bottom);
}

TEST(alignment_test, tl)
{
	const glm::vec2 origin{20, 50};
	const glm::vec2 size{30, 30};

	const glm::vec2 tl_result{20, 50};
	EXPECT_EQ(tr::tl(origin, size, tr::align::tl), tl_result);

	const glm::vec2 cc_result{5, 35};
	EXPECT_EQ(tr::tl(origin, size, tr::align::cc), cc_result);

	const glm::vec2 br_result{-10, 20};
	EXPECT_EQ(tr::tl(origin, size, tr::align::br), br_result);
}