/// @file
/// @brief Tests utility/alignment.hpp.

#include <gtest/gtest.h>
#include <tr/utility/alignment.hpp>

//

/// Data about an alignment case.
struct alignment_case
{
	/// Vertical alignment.
	tr::valign valign;

	/// Horizontal alignment.
	tr::halign halign;

	/// Combined alignment.
	tr::align align;

	/// Position of the top-left corner with origin=(20, 50), size=(30, 10).
	glm::vec2 tl;
};

/// List of alignment cases.
constexpr std::array<alignment_case, 9> alignment_cases{
	alignment_case{tr::valign::top, tr::halign::left, tr::align::tl, {20, 50}},
	alignment_case{tr::valign::top, tr::halign::center, tr::align::tc, {5, 50}},
	alignment_case{tr::valign::top, tr::halign::right, tr::align::tr, {-10, 50}},
	alignment_case{tr::valign::center, tr::halign::left, tr::align::cl, {20, 45}},
	alignment_case{tr::valign::center, tr::halign::center, tr::align::cc, {5, 45}},
	alignment_case{tr::valign::center, tr::halign::right, tr::align::cr, {-10, 45}},
	alignment_case{tr::valign::bottom, tr::halign::left, tr::align::bl, {20, 40}},
	alignment_case{tr::valign::bottom, tr::halign::center, tr::align::bc, {5, 40}},
	alignment_case{tr::valign::bottom, tr::halign::right, tr::align::br, {-10, 40}},
};

//

/// Tests composing and decomposing horizontal and vertical alignment.
TEST(AlignmentTest, ComposesAndDecomposes)
{
	for (const alignment_case& alignment_case : alignment_cases) {
		EXPECT_EQ(alignment_case.valign | alignment_case.halign, alignment_case.align);
		EXPECT_EQ(alignment_case.halign | alignment_case.valign, alignment_case.align);
		EXPECT_EQ(tr::to_halign(alignment_case.align), alignment_case.halign);
		EXPECT_EQ(tr::to_valign(alignment_case.align), alignment_case.valign);
	}
}

/// Tests computing the top-left corner of a rectangle.
TEST(AlignmentTest, ComputesTopLeft)
{
	for (const alignment_case& alignment_case : alignment_cases) {
		EXPECT_EQ(tr::tl(glm::vec2{20, 50}, glm::vec2{30, 10}, alignment_case.align), alignment_case.tl);
	}
}