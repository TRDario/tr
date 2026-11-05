/// @file
/// @brief Tests utility/out_handle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/out_handle.hpp>

//

namespace
{
	/// @brief Default-constructible, stateless deleter that keeps track of deletions.
	struct stateless_deleter
	{
		/// @brief Number of tracked deletions.
		inline static int deleted_count{0};

		/// @brief Last deleted value.
		inline static int last_deleted_handle{-1};

		//

		/// @brief Mock deletion.
		/// @param value Value to "delete".
		void operator()(int value) const noexcept
		{
			++deleted_count;
			last_deleted_handle = value;
		}
	};

	/// @brief Mock C function taking a handled value by pointer.
	/// @param out Handle output pointer.
	/// @param handle Handle to assign to the output.
	void set_c_handle(int* out, int handle)
	{
		*out = handle;
	}
} // namespace

//

/// @brief Tests setting a unique handle using `tr::out_handle`.
TEST(OutHandleTest, UniqueHandle)
{
	tr::unique_handle<int, 0, stateless_deleter> handle;

	set_c_handle(tr::out_handle{handle}, 10);
	ASSERT_TRUE(handle);
	EXPECT_EQ(handle.get(), 10);

	set_c_handle(tr::out_handle{handle}, 0);
	EXPECT_FALSE(handle);
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_handle, 10);
}