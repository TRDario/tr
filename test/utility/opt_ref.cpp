/// @file
/// @brief Tests utility/opt_ref.hpp.

#include <gtest/gtest.h>
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/ref.hpp>

//

/// Dummy base class.
struct base
{
	virtual ~base() = default;
};

/// Dummy derived class.
struct derived : base
{
};

//

/// Tests constructing an empty optional reference.
TEST(OptRefTest, ConstructsEmpty)
{
	tr::opt_ref<int> empty1;
	tr::opt_ref<int> empty2(std::nullopt);

	EXPECT_FALSE(empty1.has_value());
	EXPECT_FALSE(empty2.has_value());
	EXPECT_EQ(empty1.as_ptr(), nullptr);
	EXPECT_EQ(empty1, empty2);
}

/// Tests constructing an optional reference from a reference.
TEST(OptRefTest, ConstructsFromReference)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};

	ASSERT_TRUE(wrapped_ref.has_value());
	EXPECT_EQ(wrapped_ref.as_ptr(), &data);
	EXPECT_EQ(*wrapped_ref, 10);
}

/// Tests constructing an optional reference to const from an optional reference to non-const.
TEST(OptRefTest, ConstructsOptRefToConstFromNonConst)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};
	tr::opt_ref<const int> wrapped_cref{data};

	ASSERT_TRUE(wrapped_cref.has_value());
	EXPECT_EQ(*wrapped_cref, 10);
}

/// Tests constructing an optional reference to base from an optional reference to derived.
TEST(OptRefTest, ConstructsOptRefToBaseFromDerived)
{
	derived data{};
	tr::opt_ref<derived> wrapped_ref{data};
	tr::opt_ref<base> wrapped_base_ref{data};

	ASSERT_TRUE(wrapped_base_ref.has_value());
	EXPECT_EQ((base*)&data, wrapped_base_ref.as_ptr());
}

/// Tests comparing optional references.
TEST(OptRefTest, Compares)
{
	int data1{10};
	int data2{10};
	tr::ref<int> wrapped_ref1{data1};
	tr::opt_ref<int> wrapped_ref1_opt{data1};
	tr::opt_ref<int> wrapped_ref1_copy{wrapped_ref1};
	tr::opt_ref<int> wrapped_ref2{data2};
	tr::opt_ref<int> empty_ref{};
	EXPECT_EQ(wrapped_ref1, wrapped_ref1_opt);
	EXPECT_EQ(wrapped_ref1_opt, wrapped_ref1_copy);
	EXPECT_EQ(data1, wrapped_ref1);
	EXPECT_EQ(wrapped_ref1_opt, data1);
	EXPECT_EQ(empty_ref, std::nullopt);
	EXPECT_NE(wrapped_ref1_opt, wrapped_ref2);
	EXPECT_NE(wrapped_ref1_opt, empty_ref);
}

/// Tests accessing the value an optional reference points to.
TEST(OptRefTest, ProvidesAccess)
{
	/// Dummy value struct.
	struct data_t
	{
		int value;
	};

	data_t data{10};
	tr::opt_ref<data_t> wrapped_ref{data};
	ASSERT_TRUE(wrapped_ref.has_value());
	EXPECT_EQ((*wrapped_ref).value, 10);
	EXPECT_EQ(wrapped_ref->value, 10);
	EXPECT_EQ(wrapped_ref.as_ptr(), &data);
	(*wrapped_ref).value = 15;
	EXPECT_EQ(data.value, 15);
	wrapped_ref->value = 20;
	EXPECT_EQ(data.value, 20);
}

/// Tests making an optional reference from a pointer.
TEST(OptRefTest, MakeOptRef)
{
	int data{10};
	auto empty_ref{tr::make_opt_ref<int>(nullptr)};
	auto wrapped_ref{tr::make_opt_ref(&data)};
	EXPECT_FALSE(empty_ref.has_value());
	EXPECT_EQ(empty_ref, std::nullopt);
	ASSERT_TRUE(wrapped_ref.has_value());
	EXPECT_EQ(wrapped_ref, data);
}