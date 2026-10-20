/// @file
/// @brief Tests utility/dynamic_ref_cast.hpp.

#include <gtest/gtest.h>
#include <tr/utility/dynamic_ref_cast.hpp>

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

/// Tests `tr::dynamic_ref_cast()` casting plain references.
TEST(DynamicRefCastTest, CastsReference)
{
	base data1{};
	derived data2{};
	base& data2_ref{data2};

	tr::opt_ref<derived> wrapped_data2_ref{tr::dynamic_ref_cast<derived>(data2_ref)};
	EXPECT_EQ(wrapped_data2_ref, data2);

	tr::opt_ref<derived> wrapped_data1_ref{tr::dynamic_ref_cast<derived>(data1)};
	EXPECT_EQ(wrapped_data1_ref, std::nullopt);
}

/// Tests `tr::dynamic_ref_cast()` casting `tr::ref`.
TEST(DynamicRefCastTest, CastsTrReference)
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

/// Tests `tr::dynamic_ref_cast()` casting `tr::opt_ref`.
TEST(DynamicRefCastTest, CastsOptTrReference)
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