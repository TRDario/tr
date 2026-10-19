/// @file
/// @brief Tests utility/utf8.hpp.

#include <gtest/gtest.h>
#include <tr/utility/inplace_string.hpp>
#include <tr/utility/inplace_vector.hpp>
#include <tr/utility/utf8.hpp>

//

/// Sample UTF-8-encoded string.
constexpr tr::inplace_string<10> characters{"ač東😳"};

/// List of codepoints in `characters`.
constexpr std::array<tr::codepoint, 4> codepoints{0x61, 0x10D, 0x6771, 0x1F633};

/// List of offsets to the beginnings of the character sequences in `characters`.
constexpr std::array<tr::usize, 5> offsets{0, 1, 3, 6, 10};

//

/// Tests that a UTF-8 iterator is classed as bidirectional.
static_assert(std::bidirectional_iterator<tr::utf8::iterator>, "tr::utf8::iterator should be bidirectional, but is not!");

/// Tests that an indexed UTF-8 iterator is classed as bidirectional.
static_assert(std::bidirectional_iterator<tr::utf8::indexed_iterator>, "tr::utf8::indexed_iterator should be bidirectional, but is not!");

//

/// Tests converting a UTF-8 string to codepoints.
TEST(Utf8Test, ConvertsUtf8ToCodepoint)
{
	for (int i = 0; i < 4; ++i) {
		EXPECT_EQ(tr::utf8::to_cp(characters.begin() + offsets[i]), codepoints[i]);
	}
}

/// Tests iterating forwards through a UTF-8 string.
TEST(Utf8Test, IteratesForwards)
{
	for (auto [it, i] = std::pair{characters.begin(), 0}; i <= 4; it = tr::utf8::next(it), ++i) {
		EXPECT_EQ(tr::usize(std::distance(characters.begin(), it)), offsets[i]);
		EXPECT_EQ(tr::utf8::next(characters.begin(), i), it);
	}
}

/// Tests iterating backwards through a UTF-8 string.
TEST(Utf8Test, IteratesBackwards)
{
	for (auto [it, i] = std::pair{characters.end(), 4}; i >= 0; it = tr::utf8::prev(it), --i) {
		EXPECT_EQ(tr::usize(std::distance(characters.begin(), it)), offsets[i]);
		EXPECT_EQ(tr::utf8::prev(characters.end(), 4 - i), it);
	}
}

/// Tests inserting into a UTF-8 string.
TEST(Utf8Test, Inserts)
{
	tr::inplace_string<10> insert;
	for (tr::codepoint cp : codepoints) {
		tr::utf8::insert(insert, insert.begin(), cp);
	}
	EXPECT_EQ(insert, "😳東ča");
}

/// Tests deleting from a UTF-8 string.
TEST(Utf8Test, Deletes)
{
	tr::inplace_string<10> erase{"😳東ča"};
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "東ča");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "ča");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "a");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_TRUE(erase.empty());
}

/// Tests popping back from a UTF-8 string.
TEST(Utf8Test, PopsBack)
{
	tr::inplace_string<10> pop_back{"😳東ča"};
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳東č");
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳東");
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳");
	tr::utf8::pop_back(pop_back);
	EXPECT_TRUE(pop_back.empty());
}

/// Tests iterating forwards with the UTF-8 string iterator.
TEST(Utf8Test, IteratesForwardsWithIterator)
{
	tr::inplace_vector<tr::codepoint, 4> range_codepoints;
	for (tr::utf8::iterator it = tr::utf8::begin(characters); it != tr::utf8::end(characters); ++it) {
		range_codepoints.emplace_back(*it);
	}
	EXPECT_TRUE(std::ranges::equal(range_codepoints, codepoints));
}

/// Tests iterating backwards with the UTF-8 string iterator.
TEST(Utf8Test, IteratesBackwardsWithIterator)
{
	tr::inplace_vector<tr::codepoint, 4> range_codepoints;
	for (tr::utf8::iterator it = tr::utf8::end(characters); it != tr::utf8::begin(characters); --it) {
		range_codepoints.emplace_back(*std::prev(it));
	}
	EXPECT_TRUE(std::ranges::equal(range_codepoints, std::views::reverse(codepoints)));
}

/// Tests iterating forwards with the indexed UTF-8 string iterator.
TEST(Utf8Test, IteratesForwardsWithIndexedIterator)
{
	tr::inplace_vector<tr::codepoint, 4> range_codepoints;
	for (tr::utf8::indexed_iterator it = tr::utf8::ibegin(characters); it != tr::utf8::iend(characters); ++it) {
		range_codepoints.emplace_back(*it);
		EXPECT_EQ(std::ranges::distance(tr::utf8::ibegin(characters), it), it.index());
	}
	EXPECT_TRUE(std::ranges::equal(range_codepoints, codepoints));
}

/// Tests iterating backwards with the indexed UTF-8 string iterator.
TEST(Utf8Test, IteratesBackwardsWithIndexedIterator)
{
	tr::inplace_vector<tr::codepoint, 4> range_codepoints;
	for (tr::utf8::indexed_iterator it = tr::utf8::iend(characters); it != tr::utf8::ibegin(characters); --it) {
		const tr::utf8::indexed_iterator prev_it{std::prev(it)};
		range_codepoints.emplace_back(*prev_it);
		EXPECT_EQ(std::ranges::distance(tr::utf8::ibegin(characters), prev_it), prev_it.index());
	}
	EXPECT_TRUE(std::ranges::equal(range_codepoints, std::views::reverse(codepoints)));
}

/// Tests iterating through a Unicode codepoint view over a UTF-8 string.
TEST(Utf8Test, IteratesThroughRange)
{
	tr::inplace_vector<tr::codepoint, 4> range_codepoints;
	for (tr::codepoint cp : tr::utf8::range(characters)) {
		range_codepoints.emplace_back(cp);
	}
	EXPECT_TRUE(std::ranges::equal(range_codepoints, codepoints));
}

/// Tests calculating the length of a UTF-8 string.
TEST(Utf8Test, CalculatesLength)
{
	EXPECT_EQ(tr::utf8::length(characters), 4);
}