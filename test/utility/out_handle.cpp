/// @file
/// @brief Tests utility/out_handle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/out_handle.hpp>

//

/// Default-constructible, stateless deleter that keeps track of deletions.
struct stateless_deleter
{
	/// Number of tracked deletions.
	inline static int deleted_count{0};

	/// Last deleted value.
	inline static int last_deleted_value{0};

	//

	/// Mock deletion.
	void operator()(int value) const noexcept
	{
		++deleted_count;
		last_deleted_value = value;
	}
};

/// Mock C function taking a handled value by pointer.
static void set_c_handle(int* handle, int value)
{
	*handle = value;
}

//

TEST(out_handle_test, out_handle)
{
	tr::handle<int, 0, stateless_deleter> handle;

	set_c_handle(tr::out_handle(handle), 10);
	EXPECT_TRUE(handle.has_value());
	EXPECT_EQ(handle.get(), 10);

	set_c_handle(tr::out_handle(handle, tr::maybe_empty), 0);
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_value, 10);
}