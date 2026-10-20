/// @file
/// @brief Tests utility/exception.hpp.

#include <gtest/gtest.h>
#include <tr/utility/exception.hpp>

//

/// Tests that the 'what()' string builds correctly in a class inherited from `tr::exception`.
TEST(ExceptionTest, BuildsWhatStringCorrectly)
{
	struct test_error : public tr::exception
	{
		std::string_view name() const noexcept override
		{
			return "Test error";
		}

		std::string_view description() const noexcept override
		{
			return "this is a test";
		}

		std::string_view details() const noexcept override
		{
			return "testing errors";
		}
	};

	EXPECT_STREQ(test_error{}.what(), "Test error: this is a test (testing errors)");
}