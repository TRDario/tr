/// @file
/// @brief Tests utility/pointer_iterator.hpp.

#include <gtest/gtest.h>
#include <tr/utility/pointer_iterator.hpp>

//

/// Iterator wrapping an integer pointer.
struct test_pointer_iterator : tr::pointer_iterator<test_pointer_iterator, int*>
{
	using pointer_iterator<test_pointer_iterator, int*>::pointer_iterator;
};

//

/// Tests the pointer iterator's difference type.
static_assert(std::same_as<test_pointer_iterator::difference_type, std::ptrdiff_t>);

/// Tests the pointer iterator's value type.
static_assert(std::same_as<test_pointer_iterator::value_type, int>);

/// Tests the pointer iterator's pointer type.
static_assert(std::same_as<test_pointer_iterator::pointer, int*>);

/// Tests the pointer iterator's reference type.
static_assert(std::same_as<test_pointer_iterator::reference, int&>);

/// Tests that the derived iterator models a contiguous iterator.
static_assert(std::contiguous_iterator<test_pointer_iterator>);

//

/// Tests constructing a pointer iterator and accessing its wrapped pointer.
TEST(PointerIteratorTest, ConstructsAndAccessesBase)
{
	int values[]{1, 2, 3};
	const test_pointer_iterator iterator{values};

	EXPECT_EQ(iterator.base(), values);
	EXPECT_EQ(*iterator, 1);
	EXPECT_EQ(iterator.operator->(), values);
	EXPECT_EQ(iterator[1], 2);
}

/// Tests incrementing and decrementing a pointer iterator.
TEST(PointerIteratorTest, IncrementsAndDecrements)
{
	int values[]{1, 2, 3};
	test_pointer_iterator iterator{values};

	EXPECT_EQ((++iterator).base(), values + 1);
	EXPECT_EQ((iterator++).base(), values + 1);
	EXPECT_EQ(iterator.base(), values + 2);
	EXPECT_EQ((--iterator).base(), values + 1);
	EXPECT_EQ((iterator--).base(), values + 1);
	EXPECT_EQ(iterator.base(), values);
}

/// Tests advancing and subtracting from a pointer iterator.
TEST(PointerIteratorTest, SupportsArithmetic)
{
	int values[]{1, 2, 3, 4};
	const test_pointer_iterator iterator{values + 1};
	test_pointer_iterator advanced{iterator};

	EXPECT_EQ((advanced += 2).base(), values + 3);
	EXPECT_EQ((advanced -= 1).base(), values + 2);
	EXPECT_EQ((iterator + 2).base(), values + 3);
	EXPECT_EQ((2 + iterator).base(), values + 3);
	EXPECT_EQ((iterator - 1).base(), values);
	EXPECT_EQ(iterator.base(), values + 1);
}

/// Tests comparing pointer iterators and getting their distance.
TEST(PointerIteratorTest, ComparesAndGetsDistance)
{
	int values[]{1, 2, 3, 4};
	const test_pointer_iterator first{values};
	const test_pointer_iterator second{values + 2};
	const test_pointer_iterator same{values};

	EXPECT_EQ(first, same);
	EXPECT_NE(first, second);
	EXPECT_LT(first, second);
	EXPECT_GT(second, first);
	EXPECT_EQ(second - first, 2);
	EXPECT_EQ(first - second, -2);
}