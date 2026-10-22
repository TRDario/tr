/// @file
/// @brief Tests utility/iterator_interface.hpp.

#include <gtest/gtest.h>
#include <tr/utility/iterator_interface.hpp>

//

/// Iterator used to exercise the operators synthesized by iterator_interface.
struct synthesized_iterator : tr::iterator_interface<synthesized_iterator>
{
	/// Position pointer of the iterator.
	int* ptr;

	//

	/// Constructs an iterator.
	constexpr explicit synthesized_iterator(int* ptr) noexcept
		: ptr{ptr}
	{
	}

	//

	/// Dereferences the iterator.
	constexpr int& operator*() const noexcept
	{
		return *ptr;
	}

	/// Increments the iterator.
	constexpr synthesized_iterator& operator++() noexcept
	{
		++ptr;
		return *this;
	}

	/// Decrements the iterator.
	constexpr synthesized_iterator& operator--() noexcept
	{
		--ptr;
		return *this;
	}

	/// Advances the iterator.
	constexpr synthesized_iterator& operator+=(std::ptrdiff_t difference) noexcept
	{
		ptr += difference;
		return *this;
	}
};

/// Iterator exposing only dereferencing, used to check unavailable operations.
struct dereference_only_iterator : tr::iterator_interface<dereference_only_iterator>
{
	/// Position pointer of the iterator.
	int* ptr;

	//

	/// Constructs an iterator.
	constexpr explicit dereference_only_iterator(int* ptr) noexcept
		: ptr{ptr}
	{
	}

	//

	/// Dereferences the iterator.
	constexpr int& operator*() const noexcept
	{
		return *ptr;
	}
};

/// Iterator whose dereference returns a value rather than a reference.
struct value_dereference_only_iterator : tr::iterator_interface<value_dereference_only_iterator>
{
	/// Value of the iterator.
	int value;

	//

	/// Constructs an iterator.
	constexpr explicit value_dereference_only_iterator(int value) noexcept
		: value{value}
	{
	}

	//

	/// Dereferences the iterator.
	constexpr int operator*() const noexcept
	{
		return value;
	}
};

//

/// Iterator with defined post-incrementation.
template <typename Iterator>
concept has_post_increment = requires(Iterator& it) { it++; };

/// Iterator with defined post-incrementation.
template <typename Iterator>
concept has_post_decrement = requires(Iterator& it) { it--; };

/// Iterator with defined addition.
template <typename Iterator>
concept has_addition = requires(const Iterator& it) {
	it + 2;
	2 + it;
};

/// Iterator with defined subtraction.
template <typename Iterator>
concept has_subtraction = requires(Iterator& it) {
	it -= 2;
	it - 2;
};

/// Iterator with defined subscript dereferencing.
template <typename Iterator>
concept has_subscript_dereference = requires(const Iterator& it) { it[2]; };

/// Iterator with defined pointer dereferencing.
template <typename Iterator>
concept has_pointer_dereference = requires(const Iterator& it) { it.operator->(); };

//
// clang-format off

/// Tests that `tr::iterator_interface` synthesizes post-incrementation for `synthesized_iterator`.
static_assert(
	has_post_increment<synthesized_iterator>,
	"tr::iterator_interface should have synthesized post-incrementation, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes post-decrementation for `synthesized_iterator`..
static_assert(
	has_post_decrement<synthesized_iterator>,
	"tr::iterator_interface should have synthesized post-decrementation, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes addition for `synthesized_iterator`..
static_assert(
	has_addition<synthesized_iterator>,
	"tr::iterator_interface should have synthesized addition, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes subtraction for `synthesized_iterator`.
static_assert(
	has_subtraction<synthesized_iterator>,
	"tr::iterator_interface should have synthesized subtraction, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes subscript dereferencing for `synthesized_iterator`..
static_assert(
	has_subscript_dereference<synthesized_iterator>,
	"tr::iterator_interface should have synthesized subscripting, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `synthesized_iterator`..
static_assert(
	has_pointer_dereference<synthesized_iterator>,
	"tr::iterator_interface should have synthesized pointer dereferencing, but didn't!"
);

/// Tests that `tr::iterator_interface` does not synthesize post-incrementation for `dereference_only_iterator`.
static_assert(
	!has_post_increment<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized post-incrementation, but did!"
);

/// Tests that `tr::iterator_interface` does not synthesize post-decrementation for `dereference_only_iterator`.
static_assert(
	!has_post_decrement<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized post-decrementation, but did!"
);

/// Tests that `tr::iterator_interface` synthesizes addition for `dereference_only_iterator`..
static_assert(
	!has_addition<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized addition, but did!"
);

/// Tests that `tr::iterator_interface` does not synthesize subtraction for `dereference_only_iterator`.
static_assert(
	!has_subtraction<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized subtraction, but did!"
);

/// Tests that `tr::iterator_interface` does not synthesize subscript dereferencing for `dereference_only_iterator`.
static_assert(
	!has_subscript_dereference<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized subscripting, but did!"
);

/// Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `dereference_only_iterator`.
static_assert(
	has_pointer_dereference<dereference_only_iterator>,
	"tr::iterator_interface should have synthesized pointer dereferencing, but didn't!"
);

/// Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `value_dereference_only_iterator`.
static_assert(
	!has_pointer_dereference<value_dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized pointer dereferencing, but did!"
);

// clang-format on
//

/// Tests that post-incrementation is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPostIncrement)
{
	int values[]{1, 2, 3};
	synthesized_iterator iterator{values};
	const synthesized_iterator previous{iterator++};
	EXPECT_EQ(previous.ptr, values);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// Tests that post-decrementation is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPostDecrement)
{
	int values[]{1, 2, 3};
	synthesized_iterator iterator{values + 2};
	const synthesized_iterator previous{iterator--};
	EXPECT_EQ(previous.ptr, values + 2);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// Tests that subtraction assignment is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesSubtractionAssignment)
{
	int values[]{1, 2, 3, 4};
	synthesized_iterator iterator{values + 3};
	synthesized_iterator& result{(iterator -= 2)};
	EXPECT_EQ(&result, &iterator);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// Tests that arithmetic operators are synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesArithmeticOperators)
{
	int values[]{1, 2, 3, 4};
	const synthesized_iterator iterator{values + 1};
	EXPECT_EQ((iterator + 2).ptr, values + 3);
	EXPECT_EQ((2 + iterator).ptr, values + 3);
	EXPECT_EQ((iterator - 1).ptr, values);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// Tests that pointer dereferencing is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPointerDereference)
{
	int value{42};
	const synthesized_iterator iterator{&value};
	EXPECT_EQ(iterator.operator->(), &value);
	EXPECT_EQ(*iterator.operator->(), 42);
}

/// Tests that subscript dereferencing is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesSubscriptDereference)
{
	int values[]{1, 2, 3};
	const synthesized_iterator iterator{values};
	iterator[1] = 42;
	EXPECT_EQ(values[1], 42);
}