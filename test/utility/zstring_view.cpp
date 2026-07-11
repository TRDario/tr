///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/zstring_view.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/zstring_view.hpp>

using namespace std::literals;
using namespace tr::literals;

TEST(zstring_view_test, construction)
{
	const tr::zstring_view empty{};
	const tr::zstring_view literal{"literal"};

	const std::string source{"content"};
	const tr::zstring_view string{source};
}

TEST(zstring_view_test, comparison)
{
	EXPECT_TRUE("test"_zsv == "test"_zsv);
	EXPECT_TRUE("test"_zsv == "test");
	EXPECT_TRUE("test"_zsv == "test"s);
	EXPECT_TRUE("test"_zsv == "test"sv);

	EXPECT_TRUE("tost"_zsv != "test"_zsv);
	EXPECT_TRUE("tast"_zsv < "test"_zsv);
	EXPECT_TRUE("tost"_zsv > "test"_zsv);
	EXPECT_TRUE("test1"_zsv > "test"_zsv);
	EXPECT_TRUE("test"_zsv < "test1"_zsv);
}

TEST(zstring_view_test, iteration)
{
	const tr::zstring_view view{"This is a test!"};

	EXPECT_NE(std::ranges::find(view, 's'), view.end());
	EXPECT_EQ(std::ranges::find(view, 'w'), view.end());
	EXPECT_EQ(std::ranges::count(view, ' '), 3);
	EXPECT_EQ(std::ranges::distance(view), 15);
}

TEST(zstring_view_test, empty)
{
	EXPECT_TRUE(tr::zstring_view{}.empty());
	EXPECT_TRUE(""_zsv.empty());
	EXPECT_FALSE("test"_zsv.empty());
}

TEST(zstring_view_test, remove_prefix)
{
	tr::zstring_view str{"testTEST"};
	str.remove_prefix(4);
	EXPECT_EQ(str, "TEST");
}

TEST(zstring_view_test, substr)
{
	EXPECT_EQ("testTEST"_zsv.substr(4), "TEST");
}

TEST(zstring_view_test, ostream)
{
	std::ostringstream str;
	str << "test"_zsv;
	EXPECT_EQ(str.str(), "test");
}

TEST(zstring_view_test, format)
{
	EXPECT_EQ(TR_FMT::format("{}", "test"_zsv), "test");
}