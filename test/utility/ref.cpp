/// @file
/// @brief Tests utility/ref.hpp.

#include <gtest/gtest.h>
#include <tr/utility/ref.hpp>

//

struct base
{
	virtual ~base() = default;
};

struct derived : base
{
};

//

TEST(ref_test, construction)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	int& unwrapped_ref{data};
	EXPECT_EQ(&data, &unwrapped_ref);
}

TEST(ref_test, const_qualification)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	tr::ref<const int> wrapped_cref{data};
	const int& unwrapped_cref{data};
	EXPECT_EQ(&data, &unwrapped_cref);
}

TEST(ref_test, base_class_conversion)
{
	derived data{};
	tr::ref<derived> wrapped_ref{data};
	tr::ref<base> wrapped_base_ref{data};
	base& unwrapped_base_ref{data};
	EXPECT_EQ((base*)&data, &unwrapped_base_ref);
}

TEST(ref_test, equality)
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

TEST(ref_test, access)
{
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