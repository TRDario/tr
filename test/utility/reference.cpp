///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/reference.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/reference.hpp>

struct base {
	virtual ~base() = default;
};
struct derived : base {};

TEST(reference_test, ref_construction)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	int& unwrapped_ref{data};
	EXPECT_EQ(&data, &unwrapped_ref);
}

TEST(reference_test, ref_const_qualification)
{
	int data{10};
	tr::ref<int> wrapped_ref{data};
	tr::ref<const int> wrapped_cref{data};
	const int& unwrapped_cref{data};
	EXPECT_EQ(&data, &unwrapped_cref);
}

TEST(reference_test, ref_base_class_conversion)
{
	derived data{};
	tr::ref<derived> wrapped_ref{data};
	tr::ref<base> wrapped_base_ref{data};
	base& unwrapped_base_ref{data};
	EXPECT_EQ((base*)&data, &unwrapped_base_ref);
}

TEST(reference_test, ref_equality)
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

TEST(reference_test, ref_access)
{
	struct data_t {
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

//

TEST(reference_test, opt_ref_empty)
{
	tr::opt_ref<int> empty1;
	tr::opt_ref<int> empty2(std::nullopt);

	EXPECT_FALSE(empty1.has_ref());
	EXPECT_FALSE(empty2.has_ref());
	EXPECT_EQ(empty1.as_ptr(), nullptr);
	EXPECT_EQ(empty1, empty2);
}

TEST(reference_test, opt_ref_construction)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};

	EXPECT_TRUE(wrapped_ref.has_ref());
	EXPECT_EQ(wrapped_ref.as_ptr(), &data);
	EXPECT_EQ(*wrapped_ref, 10);
}

TEST(reference_test, opt_ref_const_qualification)
{
	int data{10};
	tr::opt_ref<int> wrapped_ref{data};
	tr::opt_ref<const int> wrapped_cref{data};

	EXPECT_TRUE(wrapped_cref.has_ref());
	EXPECT_EQ(*wrapped_cref, 10);
}

TEST(reference_test, opt_ref_base_class_conversion)
{
	derived data{};
	tr::opt_ref<derived> wrapped_ref{data};
	tr::opt_ref<base> wrapped_base_ref{data};

	EXPECT_TRUE(wrapped_base_ref.has_ref());
	EXPECT_EQ((base*)&data, wrapped_base_ref.as_ptr());
}

TEST(reference_test, opt_ref_equality)
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

TEST(reference_test, make_opt_ref)
{
	int data{10};
	auto empty_ref{tr::make_opt_ref<int>(nullptr)};
	auto wrapped_ref{tr::make_opt_ref(&data)};
	EXPECT_FALSE(empty_ref.has_ref());
	EXPECT_EQ(empty_ref, std::nullopt);
	EXPECT_TRUE(wrapped_ref.has_ref());
	EXPECT_EQ(wrapped_ref, data);
}

//

TEST(reference_test, dynamic_ref_cast_lvalue)
{
	base data1{};
	derived data2{};
	base& data2_ref{data2};

	tr::opt_ref<derived> wrapped_data2_ref{tr::dynamic_ref_cast<derived>(data2_ref)};
	EXPECT_EQ(wrapped_data2_ref, data2);

	tr::opt_ref<derived> wrapped_data1_ref{tr::dynamic_ref_cast<derived>(data1)};
	EXPECT_EQ(wrapped_data1_ref, std::nullopt);
}

TEST(reference_test, dynamic_ref_cast_tr)
{
	base data1{};
	derived data2{};
	tr::ref<base> data1_ref{data1};
	tr::ref<base> data2_ref{data2};

	tr::opt_ref<derived> wrapped_data2_ref{tr::dynamic_ref_cast<derived>(data2_ref)};
	EXPECT_EQ(wrapped_data2_ref, data2);

	tr::opt_ref<derived> wrapped_data1_ref{tr::dynamic_ref_cast<derived>(data1_ref)};
	EXPECT_EQ(wrapped_data1_ref, std::nullopt);
}

TEST(reference_test, dynamic_ref_cast_opt_tr)
{
	base data1{};
	derived data2{};
	tr::opt_ref<base> empty_ref{};
	tr::opt_ref<base> data1_ref{data1};
	tr::opt_ref<base> data2_ref{data2};

	tr::opt_ref<derived> wrapped_data2_ref{tr::dynamic_ref_cast<derived>(data2_ref)};
	EXPECT_EQ(wrapped_data2_ref, data2);

	tr::opt_ref<derived> wrapped_data1_ref{tr::dynamic_ref_cast<derived>(data1_ref)};
	EXPECT_EQ(wrapped_data1_ref, std::nullopt);

	tr::opt_ref<derived> wrapped_empty_ref{tr::dynamic_ref_cast<derived>(data1_ref)};
	EXPECT_EQ(wrapped_empty_ref, std::nullopt);
}