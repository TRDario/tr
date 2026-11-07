/// @file
/// @brief Tests utility/zstring_view.hpp.

#include <gtest/gtest.h>
#include <tr/utility/zstring_view.hpp>

using namespace std::literals;
using namespace tr::literals;

//

/// @brief Tests that a NUL-terminated string view is trivially copyable.
static_assert(std::is_trivially_copyable_v<tr::zstring_view>, "tr::zstring_view should be trivially copyable, but it is not!");

/// @brief Tests that a NUL-terminated string view is trivially destructible.
static_assert(std::is_trivially_destructible_v<tr::zstring_view>, "tr::zstring_view should be trivially destructible, but it is not!");

//

/// @brief Tests that a NUL-terminated string view default constructs empty.
TEST(ZstringViewTest, DefaultConstructsEmpty)
{
	const tr::zstring_view empty{};
	EXPECT_TRUE(empty.empty());
	EXPECT_EQ(empty.begin(), empty.end());
	EXPECT_EQ(empty.data(), nullptr);
	EXPECT_EQ(empty.c_str(), nullptr);
}

/// @brief Tests constructing a NUL-terminated string view from a string literal.
TEST(ZstringViewTest, ConstructsFromStringLiteral)
{
	const tr::zstring_view literal{"literal"};
	EXPECT_FALSE(literal.empty());
	EXPECT_EQ(literal.length(), 7);
	EXPECT_STREQ(literal.c_str(), "literal");
}

/// @brief Tests constructing a NUL-terminated string view from a standard string.
TEST(ZstringViewTest, ConstructsFromString)
{
	const std::string source{"content"};
	const tr::zstring_view string{source};
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.length(), 7);
	EXPECT_EQ(string.c_str(), source.c_str());
}

/// @brief Tests constructing a NUL-terminated string view from an iterator to another NUL-terminated string view.
TEST(ZStringViewTest, ConstructsFromIterator)
{
	const tr::zstring_view source{"literal"};
	const tr::zstring_view iterator{source.begin() + 3};
	EXPECT_FALSE(iterator.empty());
	EXPECT_EQ(iterator.length(), 4);
	EXPECT_STREQ(iterator.c_str(), "eral");
}

/// @brief Tests converting a NUL-terminated string view to a standard string view.
TEST(ZstringViewTest, ConvertsToStringView)
{
	const tr::zstring_view view{"test"};
	EXPECT_EQ(std::string_view{view}, "test");
}

/// @brief Tests converting a NUL-terminated string view to a path.
TEST(ZstringViewTest, ConvertsToPath)
{
	const tr::zstring_view view{"/usr/bin/foo"};
	EXPECT_EQ(std::filesystem::path{view}.string(), "/usr/bin/foo");
}

/// @brief Tests comparing NUL-terminated string views.
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

/// @brief Tests iterating through a NUL-terminated string view.
TEST(ZstringViewTest, Iterates)
{
	const tr::zstring_view view{"This is a test!"};

	EXPECT_NE(std::ranges::find(view, 's'), view.end());
	EXPECT_EQ(std::ranges::find(view, 'w'), view.end());
	EXPECT_EQ(std::ranges::count(view, ' '), 3);
	EXPECT_EQ(std::ranges::distance(view), 15);
}

/// @brief Tests checking whether a NUL-terminated string view is empty.
TEST(ZstringViewTest, ChecksEmpty)
{
	EXPECT_TRUE(tr::zstring_view{}.empty());
	EXPECT_TRUE(""_zsv.empty());
	EXPECT_FALSE("test"_zsv.empty());
}

/// @brief Tests creating a substring from a NUL-terminated string view.
TEST(ZstringViewTest, CreatesSubstring)
{
	EXPECT_EQ("testTEST"_zsv.substr(4), "TEST");
}

/// @brief Tests removing a prefix from a NUL-terminated string view.
TEST(ZstringViewTest, RemovesPrefix)
{
	tr::zstring_view view{"testTEST"};
	view.remove_prefix(4);
	EXPECT_EQ(view, "TEST");
}

/// @brief Tests outputting a NUL-terminated string view to a stream.
TEST(ZstringViewTest, OutputsToStream)
{
	std::ostringstream stream;
	stream << "test"_zsv;
	EXPECT_EQ(stream.str(), "test");
}

/// @brief Tests appending and concatenating NUL-terminated string views to a path.
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

/// @brief Tests formatting a NUL-terminated string view.
TEST(ZstringViewTest, Formats)
{
	EXPECT_EQ(std::format("{}", "test"_zsv), "test");
}

/// @brief Tests binary-writing a NUL-terminated string view.
TEST(ZstringViewTest, WritesBinary)
{
	tr::vector_binary_sink sink;
	tr::write_binary(sink, "test"_zsv);
	EXPECT_EQ(tr::read_binary<std::string>(tr::span_binary_source{sink.vector()}), "test");
}