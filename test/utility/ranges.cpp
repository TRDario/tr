///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/ranges.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/ranges.hpp>

TEST(ranges_test, reinterpret_span)
{
	constexpr std::array<tr::u16, 2> data{0x4080, 0x4080};
	const auto reinterpreted{tr::reinterpret_span<const tr::u8>(std::span{data})};
	EXPECT_EQ(reinterpreted.size(), 4);
	EXPECT_EQ(reinterpreted[0], 0x80);
	EXPECT_EQ(reinterpreted[1], 0x40);
	EXPECT_EQ(reinterpreted[2], 0x80);
	EXPECT_EQ(reinterpreted[3], 0x40);
}

TEST(ranges_test, object_bytes_round_trip)
{
	std::pair<int, float> vs{1, 2.0f};

	const auto vs_bytes{tr::as_bytes(vs)};
	EXPECT_EQ(tr::as_object<decltype(vs)>(vs_bytes), vs);

	const auto vs_mut_bytes{tr::as_mut_bytes(vs)};
	tr::as_mut_object<decltype(vs)>(vs_mut_bytes).first = 5;
	EXPECT_EQ(vs.first, 5);
}

TEST(ranges_test, range_bytes_round_trip)
{
	std::vector<int> vs{1, 2, 3, 4, 5};

	const auto vs_bytes{tr::range_bytes(vs)};
	EXPECT_TRUE(std::ranges::equal(tr::as_objects<const int>(vs_bytes), vs));

	const auto vs_mut_bytes{tr::range_mut_bytes(vs)};
	tr::as_mut_objects<int>(vs_mut_bytes)[0] = 10;
	EXPECT_EQ(vs[0], 10);
}

TEST(ranges_test, contains)
{
	const std::vector<int> vs{1, 2, 3, 4, 5};
	EXPECT_TRUE(tr::contains(vs, 1));
	EXPECT_FALSE(tr::contains(vs, 6));
}

TEST(ranges_test, find)
{
	const std::vector<int> vs{1, 1, 1, 1, 1, 2, 3, 4, 5, 1, 2, 3, 2, 4, 5, 4, 5, 4, 5};
	constexpr std::array<int, 2> pattern0{1, 3};
	constexpr std::array<int, 2> pattern1{4, 5};

	const auto last_of_it{tr::find_last_of(vs, pattern0)};
	EXPECT_EQ(last_of_it, vs.begin() + 11);

	const auto first_not_of_it{tr::find_first_not_of(vs, pattern0)};
	EXPECT_EQ(first_not_of_it, vs.begin() + 5);

	const auto last_not_of_it{tr::find_last_not_of(vs, pattern1)};
	EXPECT_EQ(last_not_of_it, vs.begin() + 12);
}

TEST(ranges_test, left_fold)
{
	constexpr std::array<int, 5> vs{1, 2, 3, 4, 5};
	const int sum{tr::sum(vs, 0)};
	const int product{tr::fold_left(vs, 1, [](int product, int v) { return product * v; })};
	EXPECT_EQ(sum, 15);
	EXPECT_EQ(product, 120);
}

TEST(ranges_test, deref)
{
	std::array<std::unique_ptr<int>, 5> vs{std::make_unique<int>(1), std::make_unique<int>(2), std::make_unique<int>(3),
										   std::make_unique<int>(4), std::make_unique<int>(5)};
	int sum{tr::sum(tr::deref(vs), 0)};
	EXPECT_EQ(sum, 15);

	std::ranges::for_each(tr::deref(vs), [](int& v) { v += 1; });
	sum = tr::sum(tr::deref(vs), 0);
	EXPECT_EQ(sum, 20);
}

TEST(ranges_test, unstable_erase)
{
	std::vector<int> vs{1, 2, 3, 4, 5};
	tr::unstable_erase(vs, std::ranges::find(vs, 3));
	EXPECT_TRUE(tr::contains(vs, 1));
	EXPECT_TRUE(tr::contains(vs, 2));
	EXPECT_FALSE(tr::contains(vs, 3));
	EXPECT_TRUE(tr::contains(vs, 4));
	EXPECT_TRUE(tr::contains(vs, 5));
}