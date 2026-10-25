/// @file
/// @brief Tests utility/inplace_string.hpp

#include <gtest/gtest.h>
#include <tr/utility/inplace_string.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

//

/// Tests default-constructing an empty inplace string.
TEST(InplaceStringTest, DefaultConstructsEmpty)
{
	const tr::inplace_string<32> empty;
	EXPECT_TRUE(empty.empty());
	EXPECT_EQ(empty.size(), 0);
	EXPECT_EQ(empty.begin(), empty.end());
	EXPECT_EQ(empty.max_size(), 32);
}

/// Tests constructing inplace strings sized, with and without a fill character.
TEST(InplaceStringTest, ConstructsWithSize)
{
	const tr::inplace_string<32> nul(12);
	EXPECT_FALSE(nul.empty());
	EXPECT_EQ(nul.size(), 12);
	EXPECT_TRUE(std::ranges::all_of(nul, [](char c) { return c == '\0'; }));

	const tr::inplace_string<32> a(12, 'a');
	EXPECT_FALSE(a.empty());
	EXPECT_EQ(a.size(), 12);
	EXPECT_TRUE(std::ranges::all_of(a, [](char c) { return c == 'a'; }));
}

/// Tests constructing inplace strings from string literals.
TEST(InplaceStringTest, ConstructsWithLiteral)
{
	const tr::inplace_string<32> literal{"literal"};
	EXPECT_FALSE(literal.empty());
	EXPECT_EQ(literal.size(), 7);
	EXPECT_EQ(literal, "literal");
}

/// Tests constructing inplace strings from string views.
TEST(InplaceStringTest, ConstructsWithStringView)
{
	const tr::inplace_string<32> view{"string view"sv};
	EXPECT_FALSE(view.empty());
	EXPECT_EQ(view.size(), 11);
	EXPECT_EQ(view, "string view");

	const tr::inplace_string<32> string{"string"s};
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.size(), 6);
	EXPECT_EQ(string, "string");
}

/// Tests converting an inplace string into a string view.
TEST(InplaceStringTest, ConvertsToStringView)
{
	const tr::inplace_string<32> string{"literal"};
	const std::string_view view{string};
	EXPECT_FALSE(view.empty());
	EXPECT_EQ(view.size(), 7);
	EXPECT_EQ(view, "literal");
}

/// Tests converting an inplace string into a standard string.
TEST(InplaceStringTest, ConvertsToString)
{
	const tr::inplace_string<32> string{"literal"};
	const std::string view{string};
	EXPECT_FALSE(view.empty());
	EXPECT_EQ(view.size(), 7);
	EXPECT_EQ(view, "literal");
}

/// Tests comparing inplace strings.
TEST(InplaceStringTest, Compares)
{
	constexpr tr::inplace_string<4> test{"test"};
	constexpr tr::inplace_string<4> tost{"tost"};
	constexpr tr::inplace_string<4> tast{"tast"};
	constexpr tr::inplace_string<5> test1{"test1"};

	EXPECT_TRUE(test == test);
	EXPECT_TRUE(test == "test");
	EXPECT_TRUE(test == "test"s);
	EXPECT_TRUE(test == "test"sv);

	EXPECT_TRUE(tost != test);
	EXPECT_TRUE(tast < test);
	EXPECT_TRUE(tost > test);
	EXPECT_TRUE(test1 > test);
	EXPECT_TRUE(test < test1);
}

/// Tests character and pointer access through an inplace string.
TEST(InplaceStringTest, ProvidesAccess)
{
	tr::inplace_string<32> string{"01234"};
	const tr::inplace_string<32> cstring{"01234"};
	EXPECT_EQ(string[0], '0');
	EXPECT_EQ(cstring[0], '0');
	EXPECT_EQ(string.at(2), '2');
	EXPECT_EQ(cstring.at(2), '2');
	EXPECT_EQ(string.front(), '0');
	EXPECT_EQ(cstring.front(), '0');
	EXPECT_EQ(string.back(), '4');
	EXPECT_EQ(cstring.back(), '4');
	EXPECT_EQ(*string.data(), '0');
	EXPECT_EQ(*cstring.data(), '0');

	string.front() = 'A';
	string.back() = 'Z';
	EXPECT_EQ(string[0], 'A');
	EXPECT_EQ(string[4], 'Z');
}

/// Tests iterating through an inplace string.
TEST(InplaceStringTest, Iterates)
{
	const tr::inplace_string<32> string{"This is a test!"};
	EXPECT_NE(std::ranges::find(string, 's'), string.end());
	EXPECT_EQ(std::ranges::find(string, 'w'), string.end());
	EXPECT_EQ(std::ranges::count(string, ' '), 3);
	EXPECT_EQ(std::ranges::distance(string), 15);
}

/// Tests clearing an inplace string.
TEST(InplaceStringTest, Clears)
{
	tr::inplace_string<32> string{"test"};
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.size(), 4);
	EXPECT_NE(string.begin(), string.end());

	string.clear();
	EXPECT_TRUE(string.empty());
	EXPECT_EQ(string.size(), 0);
	EXPECT_EQ(string.begin(), string.end());
}

/// Tests appending characters to an inplace string.
TEST(InplaceStringTest, AppendsCharacter)
{
	tr::inplace_string<32> string{"test"};
	ASSERT_EQ(string.size(), 4);

	string.push_back('!');
	EXPECT_EQ(string.back(), '!');
	EXPECT_EQ(string.size(), 5);

	string.push_back('@');
	EXPECT_EQ(string.back(), '@');
	EXPECT_EQ(string.size(), 6);

	string += '#';
	EXPECT_EQ(string.back(), '#');
	EXPECT_EQ(string.size(), 7);

	EXPECT_EQ(string, "test!@#");
}

/// Tests appending strings to an inplace string.
TEST(InplaceStringTest, AppendsString)
{
	tr::inplace_string<32> string{"test"};
	ASSERT_EQ(string.size(), 4);

	string.append("!!!");
	EXPECT_EQ(string.size(), 7);
	EXPECT_EQ(string, "test!!!");

	const std::string_view view{"@@@"};
	string.append(view.begin(), view.end());
	EXPECT_EQ(string.size(), 10);
	EXPECT_EQ(string, "test!!!@@@");

	string += "###";
	EXPECT_EQ(string.size(), 13);
	EXPECT_EQ(string, "test!!!@@@###");
}

/// Tests inserting into an inplace string.
TEST(InplaceStringTest, Inserts)
{
	tr::inplace_string<32> string{"test"};
	ASSERT_EQ(string.size(), 4);

	string.insert(string.begin(), "TEST");
	EXPECT_EQ(string.size(), 8);
	EXPECT_EQ(string, "TESTtest");

	string.insert(string.begin() + 4, '!');
	EXPECT_EQ(string.size(), 9);
	EXPECT_EQ(string, "TEST!test");

	const std::string_view view{"@#$"};
	string.insert(string.begin() + 5, view.begin(), view.end());
	EXPECT_EQ(string.size(), 12);
	EXPECT_EQ(string, "TEST!@#$test");
}

/// Tests popping back a character from an inplace string.
TEST(InplaceStringTest, PopsBack)
{
	tr::inplace_string<32> string{"test"};
	ASSERT_EQ(string.size(), 4);

	string.pop_back();
	EXPECT_EQ(string.size(), 3);
	EXPECT_EQ(string, "tes");
}

/// Tests erasing from an inplace string.
TEST(InplaceStringTest, Erases)
{
	tr::inplace_string<32> string{"test!TEST"};
	ASSERT_EQ(string.size(), 9);

	string.erase(string.begin() + 4);
	EXPECT_EQ(string.size(), 8);
	EXPECT_EQ(string, "testTEST");

	string.erase(string.begin() + 4, string.end());
	EXPECT_EQ(string.size(), 4);
	EXPECT_EQ(string, "test");
}

/// Tests resizing an inplace string.
TEST(InplaceStringTest, Resizes)
{
	tr::inplace_string<32> string{"test"};
	ASSERT_EQ(string.size(), 4);

	string.resize(8, '!');
	EXPECT_EQ(string.size(), 8);
	EXPECT_EQ(string, "test!!!!");

	string.resize(2);
	EXPECT_EQ(string.size(), 2);
	EXPECT_EQ(string, "te");
}

/// Tests outputting an inplace string to a stream.
TEST(InplaceStringTest, OutputsToStream)
{
	std::ostringstream stream;
	stream << tr::inplace_string<4>{"test"};
	EXPECT_EQ(stream.str(), "test");
}

/// Tests formatting an inplace string.
TEST(InplaceStringTest, Formats)
{
	EXPECT_EQ(std::format("{}", tr::inplace_string<4>{"test"}), "test");
}

/// Tests binary-writing an inplace string.
TEST(InplaceStringTest, WritesBinary)
{
	std::stringstream stream;
	tr::write_binary(stream, tr::inplace_string<4>{"test"});
	EXPECT_EQ(tr::read_binary<tr::inplace_string<4>>(stream), "test");
}