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
	ASSERT_TRUE("test"_zsv == "test"_zsv);
	ASSERT_TRUE("test"_zsv == "test");
	ASSERT_TRUE("test"_zsv == "test"s);
	ASSERT_TRUE("test"_zsv == "test"sv);

	ASSERT_TRUE("tost"_zsv != "test"_zsv);
	ASSERT_TRUE("tast"_zsv < "test"_zsv);
	ASSERT_TRUE("tost"_zsv > "test"_zsv);
	ASSERT_TRUE("test1"_zsv > "test"_zsv);
	ASSERT_TRUE("test"_zsv < "test1"_zsv);
}

TEST(zstring_view_test, iteration)
{
	const tr::zstring_view view{"This is a test!"};

	ASSERT_NE(std::ranges::find(view, 's'), view.end());
	ASSERT_EQ(std::ranges::find(view, 'w'), view.end());
	ASSERT_EQ(std::ranges::count(view, ' '), 3);
	ASSERT_EQ(std::ranges::distance(view), 15);
}

TEST(zstring_view_test, empty)
{
	ASSERT_TRUE(tr::zstring_view{}.empty());
	ASSERT_TRUE(""_zsv.empty());
	ASSERT_FALSE("test"_zsv.empty());
}

TEST(zstring_view_test, remove_prefix)
{
	tr::zstring_view str{"testTEST"};
	str.remove_prefix(4);
	ASSERT_EQ(str, "TEST");
}

TEST(zstring_view_test, substr)
{
	ASSERT_EQ("testTEST"_zsv.substr(4), "TEST");
}

TEST(zstring_view_test, ostream)
{
	std::ostringstream str;
	str << "test"_zsv;
	ASSERT_EQ(str.str(), "test");
}

TEST(zstring_view_test, format)
{
	ASSERT_EQ(TR_FMT::format("{}", "test"_zsv), "test");
}