/// @file
/// @brief Tests utility/defer.hpp.

#include <gtest/gtest.h>
#include <tr/utility/defer.hpp>

//

/// Tests that a deferred operation executes on scope end.
TEST(DeferTest, ExecutesOnScopeEnd)
{
	int i{0};
	{
		TR_DEFER(++i);
		EXPECT_EQ(i, 0);
	}
	EXPECT_EQ(i, 1);
}