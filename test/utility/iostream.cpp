/// @file
/// @brief Tests utility/iostream.hpp.

#include <gtest/gtest.h>
#include <tr/utility/iostream.hpp>
#include <tr/utility/temporary_file.hpp>

//

/// Tests writing to and reading from a temporary file.
TEST(IostreamTest, OpensFilesForWritingAndReading)
{
	tr::temporary_file file{"IostreamTest_OpensFilesForWritingAndReading", ".txt"};

	tr::open_file_w(file.path()) << "temporary file contents";
	ASSERT_TRUE(std::filesystem::exists(file.path()));

	std::ifstream input{tr::open_file_r(file.path())};
	std::string contents;
	std::getline(input, contents);
	EXPECT_EQ(contents, "temporary file contents");
}

/// Tests that opening a missing file for reading throws file_not_found.
TEST(IostreamTest, MissingFileThrows)
{
	tr::temporary_file file{"IostreamTest_MissingFileThrows"};
	ASSERT_FALSE(std::filesystem::exists(file.path()));

	EXPECT_THROW((void)tr::open_file_r(file.path()), tr::file_not_found);
}

/// Tests that a failed write open throws file_open_error.
TEST(IostreamTest, FailedWriteOpenThrows)
{
	tr::temporary_file file{"IostreamTest_FailedWriteOpenThrows"};
	EXPECT_THROW((void)tr::open_file_w(file.path().parent_path()), tr::file_open_error);
}

/// Tests checking EOF before and after reading a temporary file.
TEST(IostreamTest, ChecksEndOfFile)
{
	tr::temporary_file file{"IostreamTest_ChecksEndOfFile", ".txt"};

	tr::open_file_w(file.path()) << "contents";
	ASSERT_TRUE(std::filesystem::exists(file.path()));

	std::ifstream input{tr::open_file_r(file.path())};
	EXPECT_FALSE(tr::reached_eof(input));

	std::string contents;
	std::getline(input, contents);
	EXPECT_TRUE(tr::reached_eof(input));
}