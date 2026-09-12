/// @file
/// @brief Tests utility/opt_ref.hpp.

#include <gtest/gtest.h>
#include <tr/utility/opt_ref.hpp>
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

TEST(opt_ref_test, empty)
{
	tr::opt_ref<int> empty1;
	tr::opt_ref<int> empty2(std::nullopt);

	EXPECT_FALSE(empty1.has_ref());
	EXPECT_FALSE(empty2.has_ref());
	EXPECT_EQ(empty1.as_ptr(), nullptr);
	EXPECT_EQ(empty1, empty2);
}

TEST(opt_ref_test, construction)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};

	EXPECT_TRUE(wrapped_ref.has_ref());
	EXPECT_EQ(wrapped_ref.as_ptr(), &data);
	EXPECT_EQ(*wrapped_ref, 10);
}

TEST(opt_ref_test, const_qualification)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};
	tr::opt_ref<const int> wrapped_cref{data};

	EXPECT_TRUE(wrapped_cref.has_ref());
	EXPECT_EQ(*wrapped_cref, 10);
}

TEST(opt_ref_test, base_class_conversion)
{
	derived data{};
	tr::opt_ref<derived> wrapped_ref{data};
	tr::opt_ref<base> wrapped_base_ref{data};

	EXPECT_TRUE(wrapped_base_ref.has_ref());
	EXPECT_EQ((base*)&data, wrapped_base_ref.as_ptr());
}

TEST(opt_ref_test, equality)
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

TEST(opt_ref_test, make_opt_ref)
{
	int data{10};
	auto empty_ref{tr::make_opt_ref<int>(nullptr)};
	auto wrapped_ref{tr::make_opt_ref(&data)};
	EXPECT_FALSE(empty_ref.has_ref());
	EXPECT_EQ(empty_ref, std::nullopt);
	EXPECT_TRUE(wrapped_ref.has_ref());
	EXPECT_EQ(wrapped_ref, data);
}