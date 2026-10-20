/// @file
/// @brief Tests utility/zstring_view.hpp.

#include <gtest/gtest.h>
#include <tr/utility/zstring_view.hpp>

using namespace std::literals;
using namespace tr::literals;

//

/// Tests that a NUL-terminated string view is trivially copyable.
static_assert(std::is_trivially_copyable_v<tr::zstring_view>, "tr::zstring_view should be trivially copyable, but it is not!");

/// Tests that a NUL-terminated string view is trivially destructible.
static_assert(std::is_trivially_destructible_v<tr::zstring_view>, "tr::zstring_view should be trivially destructible, but it is not!");

//

/// Tests that a NUL-terminated string view default constructs empty.
TEST(ZstringViewTest, DefaultConstructsEmpty)
{
	const tr::zstring_view empty{};
	EXPECT_TRUE(empty.empty());
	EXPECT_EQ(empty.begin(), empty.end());
	EXPECT_EQ(empty.data(), nullptr);
	EXPECT_EQ(empty.c_str(), nullptr);
}

/// Tests constructing a NUL-terminated string view from a string literal.
TEST(ZstringViewTest, ConstructsFromStringLiteral)
{
	const tr::zstring_view literal{"literal"};
	EXPECT_FALSE(literal.empty());
	EXPECT_STREQ(literal.c_str(), "literal");
}

/// Tests constructing a NUL-terminated string view from a standard string.
TEST(ZstringViewTest, ConstructsFromString)
{
	const std::string source{"content"};
	const tr::zstring_view string{source};
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.c_str(), source.c_str());
}

/// Tests converting a NUL-terminated string view to a standard string view.
TEST(ZstringViewTest, ConvertsToStringView)
{
	const tr::zstring_view view{"test"};
	EXPECT_EQ(std::string_view{view}, "test");
}

/// Tests converting a NUL-terminated string view to a path.
TEST(ZstringViewTest, ConvertsToPath)
{
	const tr::zstring_view view{"/usr/bin/foo"};
	EXPECT_EQ(std::filesystem::path{view}.string(), "/usr/bin/foo");
}

/// Tests comparing NUL-terminated string views.
TEST(ZstringViewTest, Compares)
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

/// Tests iterating through a NUL-terminated string view.
TEST(ZstringViewTest, IsIterable)
{
	const tr::zstring_view view{"This is a test!"};

	EXPECT_NE(std::ranges::find(view, 's'), view.end());
	EXPECT_EQ(std::ranges::find(view, 'w'), view.end());
	EXPECT_EQ(std::ranges::count(view, ' '), 3);
	EXPECT_EQ(std::ranges::distance(view), 15);
}

/// Tests checking whether a NUL-terminated string view is empty.
TEST(ZstringViewTest, ChecksEmpty)
{
	EXPECT_TRUE(tr::zstring_view{}.empty());
	EXPECT_TRUE(""_zsv.empty());
	EXPECT_FALSE("test"_zsv.empty());
}

/// Tests creating a substring from a NUL-terminated string view.
TEST(ZstringViewTest, CreatesSubstring)
{
	EXPECT_EQ("testTEST"_zsv.substr(4), "TEST");
}

/// Tests removing a prefix from a NUL-terminated string view.
TEST(ZstringViewTest, RemovesPrefix)
{
	tr::zstring_view view{"testTEST"};
	view.remove_prefix(4);
	EXPECT_EQ(view, "TEST");
}

/// Tests outputting a NUL-terminated string view to a stream.
TEST(ZstringViewTest, OutputsToStream)
{
	std::ostringstream stream;
	stream << "test"_zsv;
	EXPECT_EQ(stream.str(), "test");
}

/// Tests appending and concatenating NUL-terminated string views to a path.
TEST(ZstringviewTest, AppendsAndConcatenatesToPath)
{
	const std::filesystem::path root{"a"};
	std::filesystem::path path{root / "b"_zsv};
	EXPECT_EQ(path, root / "b");
	path /= "c"_zsv;
	EXPECT_EQ(path, root / "b" / "c");
	path += "d"_zsv;
	EXPECT_EQ(path, root / "b" / "cd");
}

/// Tests formatting a NUL-terminated string view.
TEST(ZstringViewTest, Formats)
{
	EXPECT_EQ(std::format("{}", "test"_zsv), "test");
}

/// Tests binary-writing a NUL-terminated string view.
TEST(ZstringViewTest, IsBinaryWritable)
{
	std::stringstream stream;
	tr::write_binary(stream, "test"_zsv);
	EXPECT_EQ(tr::read_binary<std::string>(stream), "test");
}