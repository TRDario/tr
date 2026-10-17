/// @file
/// @brief Tests utility/temporary_file.hpp.

#include <gtest/gtest.h>
#include <tr/utility/temporary_file.hpp>

//

/// Tests that a temporary file handle is not copyable.
static_assert(!std::copyable<tr::temporary_file>, "tr::temporary_file should not be copyable but is!");

/// Tests that a temporary file handle is movable.
static_assert(std::movable<tr::temporary_file>, "tr::temporary_file should be movable but is not!");

//

/// Tests that a registered but not created temporary file is handled gracefully on destruction of the handle.
TEST(TemporaryFileTest, IgnoresMissingFile)
{
	tr::temporary_file file{"TemporaryFileTest_IgnoresMissingFile"};
	ASSERT_FALSE(std::filesystem::exists(file.path()));
}

/// Tests that a registered and created file is removed on destruction of the handle.
TEST(TemporaryFileTest, RemovesCreatedFile)
{
	std::filesystem::path file_path;
	/* Temporary file scope */ {
		tr::temporary_file file{"TemporaryFileTest_RemovesCreatedFile"};
		file_path = file.path();
		std::ofstream file_stream{file.path()};
		ASSERT_TRUE(std::filesystem::exists(file.path()));
	}
	ASSERT_FALSE(std::filesystem::exists(file_path));
}

/// Tests that the move constructor transfers ownership over a temporary file.
TEST(TemporaryFileTest, MoveConstructorTransfersOwnership)
{
	std::unique_ptr<tr::temporary_file> file1{std::make_unique<tr::temporary_file>("TemporaryFileTest_MoveConstructorTransfersOwnership")};
	const std::filesystem::path file_path{file1->path()};
	(void)std::ofstream{file_path};
	ASSERT_TRUE(std::filesystem::exists(file_path));

	std::unique_ptr<tr::temporary_file> file2{std::make_unique<tr::temporary_file>(std::move(*file1))};
	file1.reset();
	ASSERT_TRUE(std::filesystem::exists(file_path));

	file2.reset();
	ASSERT_FALSE(std::filesystem::exists(file_path));
}

/// Tests that the move assignment operator removes the previously owned file and transfers ownership over a temporary file.
TEST(TemporaryFileTest, MoveAssignmentRemovesFileAndTransfersOwnership)
{
	std::unique_ptr<tr::temporary_file> file1{
		std::make_unique<tr::temporary_file>("TemporaryFileTest_MoveAssignmentRemovesFileAndTransfersOwnership_1"),
	};
	const std::filesystem::path file1_path{file1->path()};
	(void)std::ofstream{file1_path};
	ASSERT_TRUE(std::filesystem::exists(file1_path));

	std::unique_ptr<tr::temporary_file> file2{
		std::make_unique<tr::temporary_file>("TemporaryFileTest_MoveAssignmentRemovesFileAndTransfersOwnership_2"),
	};
	const std::filesystem::path file2_path{file2->path()};
	(void)std::ofstream{file2_path};
	ASSERT_TRUE(std::filesystem::exists(file2_path));

	*file1 = std::move(*file2);
	EXPECT_FALSE(std::filesystem::exists(file1_path));
	ASSERT_TRUE(std::filesystem::exists(file2_path));

	file2.reset();
	ASSERT_TRUE(std::filesystem::exists(file2_path));

	file1.reset();
	ASSERT_FALSE(std::filesystem::exists(file2_path));
}