/// @file
/// @brief Tests utility/iterator_interface.hpp.

#include <gtest/gtest.h>
#include <tr/utility/iterator_interface.hpp>

//

/// @brief Iterator used to exercise the operators synthesized by iterator_interface.
struct synthesized_iterator : tr::iterator_interface<synthesized_iterator>
{
	/// @brief Position pointer of the iterator.
	int* ptr;

	//

	/// @brief Constructs an iterator.
	/// @param ptr Position pointer of the iterator.
	[[nodiscard]] constexpr explicit synthesized_iterator(int* ptr) noexcept
		: ptr{ptr}
	{
	}

	//

	/// @brief Dereferences the iterator.
	/// @return Reference to the pointed-to value.
	[[nodiscard]] constexpr int& operator*() const noexcept
	{
		return *ptr;
	}

	/// @brief Increments the iterator.
	/// @return Reference to `*this`.
	constexpr synthesized_iterator& operator++() noexcept
	{
		++ptr;
		return *this;
	}

	/// @brief Decrements the iterator.
	/// @return Reference to `*this`.
	constexpr synthesized_iterator& operator--() noexcept
	{
		--ptr;
		return *this;
	}

	/// @brief Advances the iterator.
	/// @param difference Amount to advance the iterator by.
	/// @return Reference to `*this`.
	constexpr synthesized_iterator& operator+=(std::ptrdiff_t difference) noexcept
	{
		ptr += difference;
		return *this;
	}
};

/// @brief Iterator exposing only dereferencing, used to check unavailable operations.
struct dereference_only_iterator : tr::iterator_interface<dereference_only_iterator>
{
	/// @brief Position pointer of the iterator.
	int* ptr;

	//

	/// @brief Constructs an iterator.
	/// @param ptr Position pointer of the iterator.
	[[nodiscard]] constexpr explicit dereference_only_iterator(int* ptr) noexcept
		: ptr{ptr}
	{
	}

	//

	/// @brief Dereferences the iterator.
	/// @return Reference to the pointed-to value.
	[[nodiscard]] constexpr int& operator*() const noexcept
	{
		return *ptr;
	}
};

/// @brief Iterator whose dereference returns a value rather than a reference.
struct value_dereference_only_iterator : tr::iterator_interface<value_dereference_only_iterator>
{
	/// @brief Value of the iterator.
	int value;

	//

	/// @brief Constructs an iterator.
	/// @param value Value of the iterator.
	[[nodiscard]] constexpr explicit value_dereference_only_iterator(int value) noexcept
		: value{value}
	{
	}

	//

	/// @brief Dereferences the iterator.
	/// @return Value of the iterator.
	[[nodiscard]] constexpr int operator*() const noexcept
	{
		return value;
	}
};

//

/// @brief Concept checking for an iterator with defined post-incrementation.
/// @tparam Iterator Iterator type to check for post-increment.
template <typename Iterator>
concept has_post_increment = requires(Iterator& it) { it++; };

/// @brief Concept checking for an iterator with defined post-incrementation.
/// @tparam Iterator Iterator type to check for post-decrement.
template <typename Iterator>
concept has_post_decrement = requires(Iterator& it) { it--; };

/// @brief Concept checking for an iterator with defined addition.
/// @tparam Iterator Iterator type to check for addition.
template <typename Iterator>
concept has_addition = requires(const Iterator& it) {
	it + 2;
	2 + it;
};

/// @brief Concept checking for an iterator with defined subtraction.
/// @tparam Iterator Iterator type to check for subtraction.
template <typename Iterator>
concept has_subtraction = requires(Iterator& it) {
	it -= 2;
	it - 2;
};

/// @brief Concept checking for an iterator with defined subscript dereferencing.
/// @tparam Iterator Iterator type to check for subscript dereference.
template <typename Iterator>
concept has_subscript_dereference = requires(const Iterator& it) { it[2]; };

/// @brief Concept checking for an iterator with defined pointer dereferencing.
/// @tparam Iterator Iterator type to check for pointer dereference.
template <typename Iterator>
concept has_pointer_dereference = requires(const Iterator& it) { it.operator->(); };

//
// clang-format off

/// @brief Tests that `tr::iterator_interface` synthesizes post-incrementation for `synthesized_iterator`.
static_assert(
	has_post_increment<synthesized_iterator>,
	"tr::iterator_interface should have synthesized post-incrementation, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes post-decrementation for `synthesized_iterator`..
static_assert(
	has_post_decrement<synthesized_iterator>,
	"tr::iterator_interface should have synthesized post-decrementation, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes addition for `synthesized_iterator`..
static_assert(
	has_addition<synthesized_iterator>,
	"tr::iterator_interface should have synthesized addition, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes subtraction for `synthesized_iterator`.
static_assert(
	has_subtraction<synthesized_iterator>,
	"tr::iterator_interface should have synthesized subtraction, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes subscript dereferencing for `synthesized_iterator`..
static_assert(
	has_subscript_dereference<synthesized_iterator>,
	"tr::iterator_interface should have synthesized subscripting, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `synthesized_iterator`..
static_assert(
	has_pointer_dereference<synthesized_iterator>,
	"tr::iterator_interface should have synthesized pointer dereferencing, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` does not synthesize post-incrementation for `dereference_only_iterator`.
static_assert(
	!has_post_increment<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized post-incrementation, but did!"
);

/// @brief Tests that `tr::iterator_interface` does not synthesize post-decrementation for `dereference_only_iterator`.
static_assert(
	!has_post_decrement<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized post-decrementation, but did!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes addition for `dereference_only_iterator`..
static_assert(
	!has_addition<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized addition, but did!"
);

/// @brief Tests that `tr::iterator_interface` does not synthesize subtraction for `dereference_only_iterator`.
static_assert(
	!has_subtraction<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized subtraction, but did!"
);

/// @brief Tests that `tr::iterator_interface` does not synthesize subscript dereferencing for `dereference_only_iterator`.
static_assert(
	!has_subscript_dereference<dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized subscripting, but did!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `dereference_only_iterator`.
static_assert(
	has_pointer_dereference<dereference_only_iterator>,
	"tr::iterator_interface should have synthesized pointer dereferencing, but didn't!"
);

/// @brief Tests that `tr::iterator_interface` synthesizes pointer dereferencing for `value_dereference_only_iterator`.
static_assert(
	!has_pointer_dereference<value_dereference_only_iterator>,
	"tr::iterator_interface should not have synthesized pointer dereferencing, but did!"
);

// clang-format on
//

/// @brief Tests that post-incrementation is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPostIncrement)
{
	int values[]{1, 2, 3};
	synthesized_iterator iterator{values};
	const synthesized_iterator previous{iterator++};
	EXPECT_EQ(previous.ptr, values);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// @brief Tests that post-decrementation is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPostDecrement)
{
	int values[]{1, 2, 3};
	synthesized_iterator iterator{values + 2};
	const synthesized_iterator previous{iterator--};
	EXPECT_EQ(previous.ptr, values + 2);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// @brief Tests that subtraction assignment is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesSubtractionAssignment)
{
	int values[]{1, 2, 3, 4};
	synthesized_iterator iterator{values + 3};
	synthesized_iterator& result{(iterator -= 2)};
	EXPECT_EQ(&result, &iterator);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// @brief Tests that arithmetic operators are synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesArithmeticOperators)
{
	int values[]{1, 2, 3, 4};
	const synthesized_iterator iterator{values + 1};
	EXPECT_EQ((iterator + 2).ptr, values + 3);
	EXPECT_EQ((2 + iterator).ptr, values + 3);
	EXPECT_EQ((iterator - 1).ptr, values);
	EXPECT_EQ(iterator.ptr, values + 1);
}

/// @brief Tests that pointer dereferencing is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesPointerDereference)
{
	int value{42};
	const synthesized_iterator iterator{&value};
	EXPECT_EQ(iterator.operator->(), &value);
	EXPECT_EQ(*iterator.operator->(), 42);
}

/// @brief Tests that subscript dereferencing is synthesized correctly.
TEST(IteratorInterfaceTest, SynthesizesSubscriptDereference)
{
	int values[]{1, 2, 3};
	const synthesized_iterator iterator{values};
	iterator[1] = 42;
	EXPECT_EQ(values[1], 42);
}