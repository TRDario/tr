/// @file
/// @brief Tests utility/ref.hpp.

#include <gtest/gtest.h>
#include <tr/utility/ref.hpp>

//

/// @brief Dummy base class.
struct base
{
	/// @brief Dummy virtual destructor.
	virtual ~base() = default;
};

/// @brief Dummy derived class.
struct derived : base
{
};

//

/// @brief Tests constructing a reference from a raw reference.
TEST(RefTest, ConstructsFromReference)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	int& unwrapped_ref{data};
	EXPECT_EQ(&data, &unwrapped_ref);
}

/// @brief Tests constructing a reference to const from a reference to non-const.
TEST(RefTest, ConstructsOptRefToConstFromNonConst)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	tr::ref<const int> wrapped_cref{data};
	const int& unwrapped_cref{data};
	EXPECT_EQ(&data, &unwrapped_cref);
}

/// @brief Tests constructing a reference to base from a reference to derived.
TEST(RefTest, ConstructsOptRefToBaseFromDerived)
{
	derived data{};
	tr::ref<derived> wrapped_ref{data};
	tr::ref<base> wrapped_base_ref{data};
	base& unwrapped_base_ref{data};
	EXPECT_EQ((base*)&data, &unwrapped_base_ref);
}

/// @brief Tests comparing references.
TEST(RefTest, Compares)
{
	int data1{10};
	int data2{10};
	tr::ref<int> wrapped_ref1{data1};
	tr::ref<int> wrapped_ref1_copy{wrapped_ref1};
	tr::ref<int> wrapped_ref2{data2};
	EXPECT_EQ(wrapped_ref1, wrapped_ref1_copy);
	EXPECT_EQ(data1, wrapped_ref1);
	EXPECT_EQ(wrapped_ref1, data1);
	EXPECT_NE(wrapped_ref1, wrapped_ref2);
}

/// @brief Tests accessing the value a reference points to.
TEST(RefTest, ProvidesAccess)
{
	/// Dummy value struct.
	struct data_t
	{
		int value;
	};

	data_t data{10};
	tr::ref<data_t> wrapped_ref{data};
	EXPECT_EQ((*wrapped_ref).value, 10);
	EXPECT_EQ(wrapped_ref->value, 10);
	EXPECT_EQ(wrapped_ref.as_ptr(), &data);
	(*wrapped_ref).value = 15;
	EXPECT_EQ(data.value, 15);
	wrapped_ref->value = 20;
	EXPECT_EQ(data.value, 20);
}