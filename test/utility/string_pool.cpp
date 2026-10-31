/// @file
/// @brief Tests utility/string_pool.hpp.

#include <gtest/gtest.h>
#include <tr/utility/string_pool.hpp>

//

/// Tests that the string pool iterator satisfies `std::bidirectional_iterator`.
static_assert(std::bidirectional_iterator<tr::string_pool::iterator>);

//

/// Tests whether a default-constructed string pool is empty.
TEST(StringPoolTest, DefaultConstructsEmpty)
{
	const tr::string_pool pool;
	EXPECT_TRUE(pool.empty());
	EXPECT_EQ(pool.size_bytes(), 0);
	EXPECT_EQ(pool.capacity_bytes(), 0);
	EXPECT_EQ(pool.begin(), pool.end());
	EXPECT_EQ(std::ranges::distance(pool), 0);
}

/// Tests appending strings to a string pool.
TEST(StringPoolTest, AppendsStrings)
{
	tr::string_pool pool;
	pool.reserve_bytes(64);
	const tr::string_pool::iterator first{pool.append("first")};
	const tr::string_pool::iterator second{pool.append("")};
	const tr::string_pool::iterator third{pool.append("third")};
	EXPECT_FALSE(pool.empty());
	EXPECT_EQ(pool.size_bytes(), 13);
	EXPECT_EQ(*first, "first");
	EXPECT_EQ(*second, "");
	EXPECT_EQ(*third, "third");
}

/// Tests iterating forwards through a string pool.
TEST(StringPoolTest, IteratesForwards)
{
	tr::string_pool pool;
	pool.append("first");
	pool.append("second");
	pool.append("third");

	tr::string_pool::iterator it{pool.begin()};
	EXPECT_EQ(*it++, "first");
	EXPECT_EQ(*it++, "second");
	EXPECT_EQ(*it++, "third");
	EXPECT_EQ(it, pool.end());
}

/// Tests iterating backwards through a string pool.
TEST(StringPoolTest, IteratesBackwards)
{
	tr::string_pool pool;
	pool.append("first");
	pool.append("second");
	pool.append("third");

	tr::string_pool::iterator it{pool.end()};
	EXPECT_EQ(*--it, "third");
	EXPECT_EQ(*--it, "second");
	EXPECT_EQ(*--it, "first");
	EXPECT_EQ(it, pool.begin());
}

/// Tests reserving capacity in a string pool.
TEST(StringPoolTest, ReservesCapacity)
{
	tr::string_pool pool;
	pool.reserve_bytes(64);
	EXPECT_GE(pool.capacity_bytes(), 64);
}

/// Tests clearing a string pool.
TEST(StringPoolTest, ClearsStrings)
{
	tr::string_pool pool;
	pool.append("stored");
	pool.clear();
	EXPECT_TRUE(pool.empty());
	EXPECT_EQ(pool.begin(), pool.end());
}

/// Tests that a string pool string builder builds a string.
TEST(StringPoolTest, StringBuilderBuildsString)
{
	tr::string_pool pool;
	pool.append("before");
	/* String builder scope */ {
		tr::string_pool::string_builder_t string_builder{pool.string_builder()};
		std::ranges::copy(std::string_view{"built"}, string_builder.out());
	}
	pool.append("after");

	tr::string_pool::iterator it{pool.begin()};
	EXPECT_EQ(*it++, "before");
	EXPECT_EQ(*it++, "built");
	EXPECT_EQ(*it++, "after");
}

/// Tests erasing a string from a string pool.
TEST(StringPoolTest, ErasesString)
{
	tr::string_pool pool;
	pool.reserve_bytes(64);
	pool.append("first");
	const tr::string_pool::iterator erased{pool.append("erased")};
	pool.append("last");

	const tr::string_pool::iterator next{pool.erase(erased)};
	EXPECT_EQ(*next, "last");
	EXPECT_EQ(std::ranges::distance(pool), 2);
	EXPECT_EQ(*pool.begin(), "first");
}

/// Tests erasing a range of strings from a string pool.
TEST(StringPoolTest, ErasesStringRange)
{
	tr::string_pool pool;
	pool.reserve_bytes(64);
	pool.append("first");
	const tr::string_pool::iterator erase_begin{pool.append("second")};
	const tr::string_pool::iterator erase_end{pool.append("third")};
	pool.append("last");

	const tr::string_pool::iterator next{pool.erase(erase_begin, erase_end)};
	EXPECT_EQ(*next, "third");
	EXPECT_EQ(std::ranges::distance(pool), 3);
	EXPECT_EQ(*pool.begin(), "first");

	const tr::string_pool::iterator end{pool.erase(pool.begin(), pool.end())};
	EXPECT_EQ(end, pool.end());
	EXPECT_TRUE(pool.empty());
}