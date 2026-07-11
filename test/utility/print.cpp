///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/print.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/print.hpp>

TEST(print_test, print_to)
{
	std::ostringstream os;
	tr::print_to(os, "PI = {:.2f}", std::numbers::pi);
	EXPECT_EQ(os.str(), "PI = 3.14");
}

TEST(print_test, println_to)
{
	std::ostringstream os;
	tr::println_to(os, "PI = {:.2f}", std::numbers::pi);
	EXPECT_EQ(os.str(), "PI = 3.14\n");
}