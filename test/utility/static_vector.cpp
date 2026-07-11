///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/static_vector.hpp.                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/static_vector.hpp>

// Tracked integer value.
struct tracked_int {
	// Number of constructed tracked integers.
	inline static int constructed{0};
	// Number of copied tracked integers.
	inline static int copy_constructed{0};
	// Number of move-constructed tracked integers.
	inline static int move_constructed{0};
	// Number of destroyed tracked integers.
	inline static int destroyed{0};

	// Resets the tracked counts.
	static void reset_counts()
	{
		constructed = 0;
		copy_constructed = 0;
		move_constructed = 0;
		destroyed = 0;
	}

	// Base integer.
	int value;

	// Constructs a tracked integer.
	tracked_int(int value = 0)
		: value{value}
	{
		++constructed;
	}
	// Copied a tracked integer.
	tracked_int(const tracked_int& copy)
		: value{copy.value}
	{
		++copy_constructed;
	}
	// Moves a tracked integer.
	tracked_int(tracked_int&& move)
		: value{move.value}
	{
		++move_constructed;
	}
	// Destroys a tracked integer.
	~tracked_int()
	{
		++destroyed;
	}

	// Copies a tracked integer.
	tracked_int& operator=(const tracked_int& copy) = default;
	// Moves a tracked integer
	tracked_int& operator=(tracked_int&& move) = default;

	// Equality comparison.
	friend bool operator==(const tracked_int& l, const tracked_int& r) = default;
	// Equality comparison.
	friend bool operator==(const tracked_int& l, int r)
	{
		return l.value == r;
	}
};

TEST(static_vector_test, default_construction)
{
	tracked_int::reset_counts();
	const tr::static_vector<tracked_int, 256> vector;
	EXPECT_TRUE(vector.empty());
	EXPECT_EQ(vector.size(), 0);
	EXPECT_EQ(vector.max_size(), 256);
	EXPECT_EQ(tracked_int::constructed, 0);
}

TEST(static_vector_test, sized_default_construction)
{
	tracked_int::reset_counts();
	const tr::static_vector<tracked_int, 256> vector(3);
	EXPECT_FALSE(vector.empty());
	EXPECT_EQ(vector.size(), 3);
	EXPECT_EQ(vector.max_size(), 256);
	EXPECT_EQ(tracked_int::constructed, 3);
	EXPECT_EQ(vector[0], 0);
	EXPECT_EQ(vector[1], 0);
	EXPECT_EQ(vector[2], 0);
}

TEST(static_vector_test, sized_construction)
{
	tracked_int::reset_counts();
	const tr::static_vector<tracked_int, 256> vector(3, 5);
	EXPECT_FALSE(vector.empty());
	EXPECT_EQ(vector.size(), 3);
	EXPECT_EQ(vector.max_size(), 256);
	EXPECT_EQ(tracked_int::copy_constructed, 3);
	EXPECT_EQ(vector[0], 5);
	EXPECT_EQ(vector[1], 5);
	EXPECT_EQ(vector[2], 5);
}

TEST(static_vector_test, iterator_construction)
{
	std::array<tracked_int, 5> source{1, 2, 3, 4, 5};
	tracked_int::reset_counts();

	const tr::static_vector<tracked_int, 256> copied{source.begin() + 1, source.end() - 1};
	EXPECT_EQ(tracked_int::constructed, 0);
	EXPECT_EQ(tracked_int::copy_constructed, 3);
	EXPECT_EQ(tracked_int::move_constructed, 0);
	EXPECT_EQ(copied.size(), 3);
	EXPECT_EQ(copied[0], 2);
	EXPECT_EQ(copied[1], 3);
	EXPECT_EQ(copied[2], 4);

	const tr::static_vector<tracked_int, 256> moved{std::make_move_iterator(source.begin() + 2), std::make_move_iterator(source.end())};
	EXPECT_EQ(tracked_int::constructed, 0);
	EXPECT_EQ(tracked_int::copy_constructed, 3);
	EXPECT_EQ(tracked_int::move_constructed, 3);
	EXPECT_EQ(moved.size(), 3);
	EXPECT_EQ(moved[0], 3);
	EXPECT_EQ(moved[1], 4);
	EXPECT_EQ(moved[2], 5);
}

TEST(static_vector_test, range_construction)
{
	std::array<tracked_int, 5> source{1, 2, 3, 4, 5};
	tracked_int::reset_counts();

	const tr::static_vector<tracked_int, 256> vector{source};
	EXPECT_EQ(tracked_int::constructed, 0);
	EXPECT_EQ(tracked_int::copy_constructed, 5);
	EXPECT_EQ(tracked_int::move_constructed, 0);
	EXPECT_EQ(vector.size(), 5);
	EXPECT_EQ(vector[0], 1);
	EXPECT_EQ(vector[1], 2);
	EXPECT_EQ(vector[2], 3);
	EXPECT_EQ(vector[3], 4);
	EXPECT_EQ(vector[4], 5);
}

TEST(static_vector_test, initializer_list_construction)
{
	const tr::static_vector<tracked_int, 256> vector{1, 2, 3};
	EXPECT_EQ(vector.size(), 3);
	EXPECT_EQ(vector[0], 1);
	EXPECT_EQ(vector[1], 2);
	EXPECT_EQ(vector[2], 3);
}

TEST(static_vector_test, copy_construction)
{
	const tr::static_vector<tracked_int, 256> original{1, 2, 3, 4, 5};
	tracked_int::reset_counts();

	const tr::static_vector<tracked_int, 256> copy{original};
	EXPECT_EQ(tracked_int::constructed, 0);
	EXPECT_EQ(tracked_int::copy_constructed, 5);
	EXPECT_EQ(tracked_int::move_constructed, 0);
	EXPECT_EQ(copy.size(), 5);
	EXPECT_EQ(copy[0], 1);
	EXPECT_EQ(copy[1], 2);
	EXPECT_EQ(copy[2], 3);
	EXPECT_EQ(copy[3], 4);
	EXPECT_EQ(copy[4], 5);
}

TEST(static_vector_test, move_construction)
{
	tr::static_vector<tracked_int, 256> original{1, 2, 3, 4, 5};
	tracked_int::reset_counts();

	const tr::static_vector<tracked_int, 256> copy{std::move(original)};
	EXPECT_EQ(tracked_int::constructed, 0);
	EXPECT_EQ(tracked_int::copy_constructed, 0);
	EXPECT_EQ(tracked_int::move_constructed, 5);
	EXPECT_EQ(copy.size(), 5);
	EXPECT_EQ(copy[0], 1);
	EXPECT_EQ(copy[1], 2);
	EXPECT_EQ(copy[2], 3);
	EXPECT_EQ(copy[3], 4);
	EXPECT_EQ(copy[4], 5);
}

TEST(static_vector_test, destruction)
{
	tracked_int::reset_counts();
	{
		tr::static_vector<tracked_int, 256> vector;
		vector.emplace_back(1);
		vector.emplace_back(2);
		vector.emplace_back(3);
		vector.emplace_back(4);
		vector.emplace_back(5);
		EXPECT_EQ(tracked_int::constructed, 5);
		EXPECT_EQ(tracked_int::destroyed, 0);
	}
	EXPECT_EQ(tracked_int::destroyed, 5);
}

TEST(static_vector_test, access)
{
	tr::static_vector<int, 256> vector{1, 2, 3, 4, 5};
	const tr::static_vector<int, 256> cvector{1, 2, 3, 4, 5};
	EXPECT_EQ(vector[0], 1);
	EXPECT_EQ(cvector[0], 1);
	EXPECT_EQ(vector.at(2), 3);
	EXPECT_EQ(cvector.at(2), 3);
	EXPECT_EQ(vector.front(), 1);
	EXPECT_EQ(cvector.front(), 1);
	EXPECT_EQ(vector.back(), 5);
	EXPECT_EQ(cvector.back(), 5);
	EXPECT_EQ(*vector.data(), 1);
	EXPECT_EQ(*cvector.data(), 1);

	vector.front() = 10;
	vector.back() = 50;
	EXPECT_EQ(vector[0], 10);
	EXPECT_EQ(vector[4], 50);
}

TEST(static_vector_test, range)
{
	tr::static_vector<int, 256> vector{1, 2, 3, 4, 5};
	const tr::static_vector<int, 256> cvector{1, 2, 3, 4, 5};
	EXPECT_EQ(*vector.begin(), 1);
	EXPECT_EQ(*vector.cbegin(), 1);
	EXPECT_EQ(*cvector.begin(), 1);
	EXPECT_EQ(*vector.rbegin(), 5);
	EXPECT_EQ(*vector.crbegin(), 5);
	EXPECT_EQ(*cvector.rbegin(), 5);

	const int sum{std::accumulate(vector.begin(), vector.end(), 0)};
	const int csum{std::accumulate(vector.begin(), vector.end(), 0)};
	EXPECT_EQ(sum, 15);
	EXPECT_EQ(csum, 15);

	for (int& v : vector) {
		v *= 10;
	}
	EXPECT_EQ(vector.front(), 10);
	EXPECT_EQ(vector.back(), 50);

	constexpr std::array<int, 5> comp{50, 40, 30, 20, 10};
	EXPECT_TRUE(std::ranges::equal(comp.begin(), comp.end(), vector.rbegin(), vector.rend()));
	EXPECT_TRUE(std::ranges::equal(comp.begin(), comp.end(), vector.crbegin(), vector.crend()));
}

TEST(static_vector_test, insert)
{
	const std::array<tracked_int, 5> source{1, 2, 3, 4, 5};
	const std::array<tracked_int, 6> comp1{2, 1, 2, 3, 4, 5};
	const std::array<tracked_int, 7> comp2{6, 2, 1, 2, 3, 4, 5};
	const std::array<tracked_int, 10> comp3{6, 2, 1, 2, 3, 1, 2, 3, 4, 5};
	const std::array<tracked_int, 15> comp4{6, 2, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5};
	const std::array<tracked_int, 18> comp5{6, 2, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8};
	const std::array<tracked_int, 19> comp6{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8};
	const std::array<tracked_int, 20> comp7{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1};
	const std::array<tracked_int, 21> comp8{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1, 2};
	const std::array<tracked_int, 22> comp9{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1, 2, 3};
	const std::array<tracked_int, 24> comp10{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1, 2, 3, 4, 5};
	const std::array<tracked_int, 29> comp11{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
	const std::array<tracked_int, 31> comp12{6, 2, 1, 7, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 5, 10, 9, 8, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7};

	tr::static_vector<tracked_int, 256> vector{1, 2, 3, 4, 5};
	vector.insert(vector.begin(), source[1]);
	EXPECT_EQ(vector.size(), 6);
	EXPECT_TRUE(std::ranges::equal(vector, comp1));
	for (auto& v : vector) {
		std::cout << v.value << "\n";
	}

	vector.insert(vector.begin(), tracked_int{6});
	EXPECT_EQ(vector.size(), 7);
	EXPECT_TRUE(std::ranges::equal(vector, comp2));

	vector.insert(vector.begin() + 2, source.begin(), source.begin() + 3);
	EXPECT_EQ(vector.size(), 10);
	EXPECT_TRUE(std::ranges::equal(vector, comp3));

	vector.insert(vector.end() - 1, source);
	EXPECT_EQ(vector.size(), 15);
	EXPECT_TRUE(std::ranges::equal(vector, comp4));

	vector.insert(vector.end(), {10, 9, 8});
	EXPECT_EQ(vector.size(), 18);
	EXPECT_TRUE(std::ranges::equal(vector, comp5));

	vector.emplace(vector.begin() + 3, 7);
	EXPECT_EQ(vector.size(), 19);
	EXPECT_TRUE(std::ranges::equal(vector, comp6));

	vector.push_back(source[0]);
	EXPECT_EQ(vector.size(), 20);
	EXPECT_TRUE(std::ranges::equal(vector, comp7));

	vector.push_back(tracked_int{2});
	EXPECT_EQ(vector.size(), 21);
	EXPECT_TRUE(std::ranges::equal(vector, comp8));

	vector.emplace_back(3);
	EXPECT_EQ(vector.size(), 22);
	EXPECT_TRUE(std::ranges::equal(vector, comp9));

	vector.append(source.begin() + 3, source.end());
	EXPECT_EQ(vector.size(), 24);
	EXPECT_TRUE(std::ranges::equal(vector, comp10));

	vector.append(source);
	EXPECT_EQ(vector.size(), 29);
	EXPECT_TRUE(std::ranges::equal(vector, comp11));

	vector.append({6, 7});
	EXPECT_EQ(vector.size(), 31);
	EXPECT_TRUE(std::ranges::equal(vector, comp12));
}

TEST(static_vector_test, erase)
{
	tr::static_vector<tracked_int, 256> vector{1, 2, 3, 4, 5};
	const std::array<tracked_int, 4> comp1{1, 2, 4, 5};
	const std::array<tracked_int, 2> comp2{4, 5};
	tracked_int::reset_counts();

	vector.erase(vector.begin() + 2);
	EXPECT_EQ(tracked_int::destroyed, 1);
	EXPECT_EQ(vector.size(), 4);
	EXPECT_TRUE(std::ranges::equal(vector, comp1));

	vector.erase(vector.begin(), vector.begin() + 2);
	EXPECT_EQ(tracked_int::destroyed, 3);
	EXPECT_EQ(vector.size(), 2);
	EXPECT_TRUE(std::ranges::equal(vector, comp2));

	vector.pop_back();
	EXPECT_EQ(tracked_int::destroyed, 4);
	EXPECT_EQ(vector.size(), 1);
	EXPECT_EQ(vector.front(), 4);

	vector.clear();
	EXPECT_EQ(tracked_int::destroyed, 5);
	EXPECT_TRUE(vector.empty());
	EXPECT_EQ(vector.size(), 0);
}

TEST(static_vector_test, resize)
{
	const tracked_int copy{1};
	tr::static_vector<tracked_int, 256> vector;
	tracked_int::reset_counts();

	vector.resize(5);
	EXPECT_EQ(tracked_int::constructed, 5);
	EXPECT_EQ(vector.size(), 5);
	for (const tracked_int& v : vector) {
		EXPECT_EQ(v, 0);
	}

	vector.resize(10, copy);
	EXPECT_EQ(tracked_int::copy_constructed, 5);
	EXPECT_EQ(vector.size(), 10);
	EXPECT_EQ(vector[4], 0);
	EXPECT_EQ(vector[5], 1);

	vector.resize(3);
	EXPECT_EQ(tracked_int::destroyed, 7);
	EXPECT_EQ(vector.size(), 3);
}

TEST(static_vector_test, lifetime)
{
	{
		tr::static_vector<tracked_int, 256> vector{};
		vector.emplace_back(1);
		vector.emplace(vector.begin(), 1);
		vector.insert(vector.begin() + 1, {1, 2, 3});
		vector.emplace(vector.begin() + 1, 4);
		vector.insert(vector.begin(), {1, 2});
		vector.erase(vector.begin() + 2);
		vector.pop_back();
	}
	EXPECT_EQ(tracked_int::constructed + tracked_int::copy_constructed + tracked_int::move_constructed, tracked_int::destroyed);
}

TEST(static_vector_test, binary_io_roundtrip)
{
	std::stringstream io;

	const tr::static_vector<int, 5> original{1, 2, 3, 4};
	tr::write_binary(io, original);

	tr::static_vector<int, 5> copy;
	tr::read_binary(io, copy);
	EXPECT_TRUE(std::ranges::equal(original, copy));
}