///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/utf8.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/static_vector.hpp>
#include <tr/utility/utf8.hpp>

constexpr tr::static_string<10> characters{"ač東😳"};
constexpr std::array<tr::codepoint, 4> codepoints{0x61, 0x10D, 0x6771, 0x1F633};
constexpr std::array<tr::usize, 5> offsets{0, 1, 3, 6, 10};

TEST(utf8_test, utf8_to_cp)
{
	for (int i = 0; i < 4; ++i) {
		EXPECT_EQ(tr::utf8::to_cp(characters.begin() + offsets[i]), codepoints[i]);
	}
}

TEST(utf8_test, forward_iteration)
{
	for (auto [it, i] = std::pair{characters.begin(), 0}; i <= 4; it = tr::utf8::next(it), ++i) {
		EXPECT_EQ(tr::usize(std::distance(characters.begin(), it)), offsets[i]);
		EXPECT_EQ(tr::utf8::next(characters.begin(), i), it);
	}
}

TEST(utf8_test, backward_iteration)
{
	for (auto [it, i] = std::pair{characters.end(), 4}; i >= 0; it = tr::utf8::prev(it), --i) {
		EXPECT_EQ(tr::usize(std::distance(characters.begin(), it)), offsets[i]);
		EXPECT_EQ(tr::utf8::prev(characters.end(), 4 - i), it);
	}
}

TEST(utf8_test, insertion)
{
	tr::static_string<10> insert;
	for (tr::codepoint cp : codepoints) {
		tr::utf8::insert(insert, insert.begin(), cp);
	}
	EXPECT_EQ(insert, "😳東ča");
}

TEST(utf8_test, deletion)
{
	tr::static_string<10> erase{"😳東ča"};
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "東ča");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "ča");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_EQ(erase, "a");
	tr::utf8::erase(erase, erase.begin());
	EXPECT_TRUE(erase.empty());
}

TEST(utf8_test, pop_back)
{
	tr::static_string<10> pop_back{"😳東ča"};
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳東č");
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳東");
	tr::utf8::pop_back(pop_back);
	EXPECT_EQ(pop_back, "😳");
	tr::utf8::pop_back(pop_back);
	EXPECT_TRUE(pop_back.empty());
}

TEST(utf8_test, iterator)
{
	tr::static_vector<tr::codepoint, 4> range_codepoints;
	for (tr::codepoint cp : tr::utf8::range(characters)) {
		range_codepoints.emplace_back(cp);
	}

	EXPECT_TRUE(std::ranges::equal(range_codepoints, codepoints));
	for (tr::utf8::indexed_iterator it = tr::utf8::ibegin(characters); it != tr::utf8::iend(characters); ++it) {
		EXPECT_EQ(std::ranges::distance(tr::utf8::ibegin(characters), it), it.index());
	}
}

TEST(utf8_test, length)
{
	EXPECT_EQ(tr::utf8::length(characters), 4);
}