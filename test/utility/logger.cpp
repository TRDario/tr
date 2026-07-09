///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/logger.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/logger.hpp>
#include <tr/utility/reference.hpp>

TEST(logger_test, backend)
{
	tr::logger logger;
	EXPECT_FALSE(logger.active());

	logger = tr::make_logger<tr::console_logger>("test");
	EXPECT_TRUE(logger.active());
	EXPECT_TRUE(tr::dynamic_ref_cast<tr::console_logger>(logger.backend()).has_ref());

	logger.clear_backend();
	EXPECT_FALSE(logger.active());

	logger.replace_backend_with<tr::console_logger>("test");
	EXPECT_TRUE(logger.active());
	EXPECT_TRUE(tr::dynamic_ref_cast<tr::console_logger>(logger.backend()).has_ref());
}

TEST(logger_test, log)
{
	struct logger_backend_mock : public tr::logger_backend {
		void log(const std::tm&, tr::severity, std::string_view string) override
		{
			EXPECT_EQ(string, "This is an integer: 420");
		}

		void log_continue(std::string_view string) override
		{
			EXPECT_EQ(string, "This is a float: 3.14");
		}
	};

	tr::logger logger{tr::make_logger<logger_backend_mock>()};
	logger.log(tr::severity::info, "This is an integer: {}", 420);
	logger.log_continue("This is a float: {:.2f}", std::numbers::pi);
}