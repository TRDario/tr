/// @file
/// @brief Tests utility/ranges.hpp.

#include <gtest/gtest.h>
#include <tr/utility/ranges.hpp>

//

/// Tests span reinterpreting.
TEST(RangesTest, ReinterpretsSpan)
{
	constexpr std::array<tr::u16, 2> data{0x4080, 0x4080};
	const std::span<const tr::u8> reinterpreted{tr::reinterpret_span<const tr::u8>(std::span{data})};
	ASSERT_EQ(reinterpreted.size(), 4);
	EXPECT_EQ(reinterpreted[0], 0x80);
	EXPECT_EQ(reinterpreted[1], 0x40);
	EXPECT_EQ(reinterpreted[2], 0x80);
	EXPECT_EQ(reinterpreted[3], 0x40);
}

/// Tests `tr::as_object()` roundtripping.
TEST(RangesTest, ObjectBytesRoundTrips)
{
	std::pair<int, float> vs{1, 2.0f};

	const std::span<const std::byte, 8> vs_bytes{tr::as_bytes(vs)};
	EXPECT_EQ((tr::as_object<std::pair<int, float>>(vs_bytes)), vs);

	const std::span<std::byte, 8> vs_mut_bytes{tr::as_mut_bytes(vs)};
	tr::as_mut_object<std::pair<int, float>>(vs_mut_bytes).first = 5;
	EXPECT_EQ(vs.first, 5);
}

/// Tests `tr::range_bytes()` roundtripping.
TEST(RangesTest, RangeBytesRoundTrips)
{
	std::vector<int> vs{1, 2, 3, 4, 5};

	const std::span<const std::byte> vs_bytes{tr::range_bytes(vs)};
	EXPECT_TRUE(std::ranges::equal(tr::as_objects<const int>(vs_bytes), vs));

	const std::span<std::byte> vs_mut_bytes{tr::range_mut_bytes(vs)};
	tr::as_mut_objects<int>(vs_mut_bytes)[0] = 10;
	EXPECT_EQ(vs[0], 10);
}

/// Tests finding the last matching value out of a set.
TEST(RangesTest, FindsLastOf)
{
	const std::vector<int> vs{1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 2, 3, 2, 4, 5, 4, 5, 4, 5};
	constexpr std::array<int, 2> pattern{1, 3};
	const std::vector<int>::const_iterator last_of_it{tr::find_last_of(vs, pattern)};
	EXPECT_EQ(last_of_it, vs.begin() + 11);
}

/// Tests finding the first matching value not out of a set.
TEST(RangesTest, FindsFirstNotOf)
{
	const std::vector<int> vs{1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 2, 3, 2, 4, 5, 4, 5, 4, 5};
	constexpr std::array<int, 2> pattern{1, 3};
	const std::vector<int>::const_iterator first_not_of_it{tr::find_first_not_of(vs, pattern)};
	EXPECT_EQ(first_not_of_it, vs.begin() + 5);
}

/// Tests finding the last matching value not out of a set.
TEST(RangesTest, FindsLastNotOf)
{
	const std::vector<int> vs{1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 2, 3, 2, 4, 5, 4, 5, 4, 5};
	constexpr std::array<int, 2> pattern{4, 5};
	const std::vector<int>::const_iterator first_not_of_it{tr::find_last_not_of(vs, pattern)};
	EXPECT_EQ(first_not_of_it, vs.begin() + 12);
}

/// Tests summing values.
TEST(RangesTest, Sums)
{
	constexpr std::array<int, 5> vs{1, 2, 3, 4, 5};
	const int sum{tr::sum(vs, 0)};
	EXPECT_EQ(sum, 15);
}

/// Tests creating a dereference view.
TEST(RangesTest, CreatesDereferenceView)
{
	std::array<std::unique_ptr<int>, 5> vs{std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<int>(3),
										   std::make_unique<int>(4), std::make_unique<int>(5)};
	int sum{tr::sum(tr::deref(vs), 0)};
	EXPECT_EQ(sum, 15);

	std::ranges::for_each(tr::deref(vs), [](int& v) { v += 1; });
	sum = tr::sum(tr::deref(vs), 0);
	EXPECT_EQ(sum, 20);
}

/// Tests unstable erasing.
TEST(RangesTest, UnstableErases)
{
	std::vector<int> vs{1, 2, 3, 4, 5};
	tr::unstable_erase(vs, std::ranges::find(vs, 3));
	EXPECT_TRUE(std::ranges::contains(vs, 1));
	EXPECT_TRUE(std::ranges::contains(vs, 2));
	EXPECT_FALSE(std::ranges::contains(vs, 3));
	EXPECT_TRUE(std::ranges::contains(vs, 4));
	EXPECT_TRUE(std::ranges::contains(vs, 5));
}