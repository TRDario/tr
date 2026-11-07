/// @file
/// @brief Tests utility/binary_source.hpp

#include <gtest/gtest.h>
#include <tr/utility/binary_source.hpp>
#include <tr/utility/temporary_file.hpp>

//

/// @brief Tests that `tr::span_binary_source` fulfills `tr::binary_source`.
static_assert(tr::binary_source<tr::span_binary_source>, "tr::span_binary_source should fulfill tr::binary_source, but doesn't!");

/// @brief Tests that `tr::file_binary_source` fulfills `tr::binary_source`.
static_assert(tr::binary_source<tr::file_binary_source>, "tr::file_binary_source should fulfill tr::binary_source, but doesn't!");

//

/// @brief Tests that a span binary source reads bytes properly.
TEST(BinarySourceTest, SpanBinarySourceReads)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	std::array<std::byte, 10> destination{};

	tr::span_binary_source binary_source{source};
	EXPECT_EQ(binary_source.read(destination), destination.size());
	EXPECT_EQ(source, destination);
	EXPECT_EQ(binary_source.position(), source.size());
}

/// @brief Tests that a span binary source only reads up to the end of the span and no more.
TEST(BinarySourceTest, SpanBinarySourceStopsAtSpanEnd)
{
	constexpr std::array<std::byte, 5> source{std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}};
	std::array<std::byte, 10> destination{};
	tr::span_binary_source binary_source{source};
	EXPECT_EQ(binary_source.read(destination), source.size());
	EXPECT_TRUE(std::ranges::equal(source.begin(), source.end(), destination.begin(), destination.begin() + source.size()));
	EXPECT_EQ(binary_source.position(), source.size());
	EXPECT_EQ(binary_source.read(destination), 0);
}

//

/// @brief Tests that a span binary source reads bytes properly.
TEST(BinarySourceTest, FileBinarySourceReads)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	const tr::temporary_file temp_file{"BinarySourceTest_FileBinarySourceReads", ".dat"};
	/* File stream scope */ {
		std::ofstream file{temp_file.path()};
		file.write(reinterpret_cast<const char*>(source.data()), source.size());
	}

	std::array<std::byte, 10> destination{};
	tr::file_binary_source binary_source{temp_file.path()};
	ASSERT_TRUE(binary_source);
	ASSERT_TRUE(binary_source.is_open());
	EXPECT_EQ(binary_source.read(destination), source.size());
	EXPECT_TRUE(std::ranges::equal(source.begin(), source.end(), destination.begin(), destination.begin() + source.size()));
	EXPECT_EQ(binary_source.position(), source.size());
}

/// @brief Tests that a closed span binary source does not read anything.
TEST(BinarySourceTest, ClosedFileBinarySourceDoesNotWrite)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	const tr::temporary_file temp_file{"BinarySourceTest_FileBinarySourceReads", ".dat"};
	/* File stream scope */ {
		std::ofstream file{temp_file.path()};
		file.write(reinterpret_cast<const char*>(source.data()), source.size());
	}

	std::array<std::byte, 10> destination{};
	tr::file_binary_source binary_source{temp_file.path()};
	binary_source.close();
	ASSERT_FALSE(binary_source);
	EXPECT_EQ(binary_source.read(destination), 0);
}

/// @brief Tests that a file binary source only reads up to the end of the file and no more.
TEST(BinarySourceTest, FileBinarySourceStopsAtFileEnd)
{
	constexpr std::array<std::byte, 5> source{std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}};
	const tr::temporary_file temp_file{"BinarySourceTest_FileBinarySourceStopsAtFileEnd", ".dat"};
	/* File stream scope */ {
		std::ofstream file{temp_file.path()};
		file.write(reinterpret_cast<const char*>(source.data()), source.size());
	}

	std::array<std::byte, 10> destination{};
	tr::file_binary_source binary_source{temp_file.path()};
	ASSERT_TRUE(binary_source);
	ASSERT_TRUE(binary_source.is_open());
	EXPECT_EQ(binary_source.read(destination), source.size());
	EXPECT_TRUE(std::ranges::equal(source.begin(), source.end(), destination.begin(), destination.begin() + source.size()));
	EXPECT_EQ(binary_source.position(), source.size());
}

/// @brief Tests closing and re-opening a file binary source.
TEST(BinarySourceTest, FileBinarySourceClosesAndOpens)
{
	const tr::temporary_file temp_file{"BinarySourceTest_FileBinarySourceThrowsOnMissingFile", ".dat"};
	/* File stream scope */ {
		std::ofstream file{temp_file.path()};
	}
	ASSERT_TRUE(std::filesystem::exists(temp_file.path()));
	tr::file_binary_source binary_source{temp_file.path()};
	ASSERT_TRUE(binary_source);
	ASSERT_TRUE(binary_source.is_open());

	binary_source.close();
	EXPECT_FALSE(binary_source);
	EXPECT_FALSE(binary_source.is_open());

	binary_source.open(temp_file.path());
	EXPECT_TRUE(binary_source);
	EXPECT_TRUE(binary_source.is_open());
}

/// @brief Tests that moving a file binary source transfers the opened file and position when moving.
TEST(BinarySourceTest, FileBinarySourceTransfersFileOwnershipAndPositionOnMove)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	const tr::temporary_file temp_file{"BinarySourceTest_FileBinarySourceTransfersFileOwnershipAndPositionOnMove", ".dat"};
	/* File stream scope */ {
		std::ofstream file{temp_file.path()};
		file.write(reinterpret_cast<const char*>(source.data()), source.size());
	}
	std::array<std::byte, 2> destination{};

	ASSERT_TRUE(std::filesystem::exists(temp_file.path()));
	tr::file_binary_source binary_source1{temp_file.path()};
	ASSERT_TRUE(binary_source1.is_open());
	ASSERT_EQ(binary_source1.read(destination), destination.size());
	ASSERT_EQ(binary_source1.position(), destination.size());
	ASSERT_TRUE(std::ranges::equal(destination.begin(), destination.end(), source.begin(), source.begin() + destination.size()));

	tr::file_binary_source binary_source2{std::move(binary_source1)};
	EXPECT_FALSE(binary_source1.is_open());
	EXPECT_TRUE(binary_source2.is_open());
	EXPECT_EQ(binary_source2.position(), destination.size());
	EXPECT_EQ(binary_source2.read(destination), destination.size());
	EXPECT_EQ(binary_source2.position(), 2 * destination.size());
	EXPECT_TRUE(std::ranges::equal(destination.begin(), destination.end(), source.begin() + destination.size(),
								   source.begin() + 2 * destination.size()));

	binary_source1 = std::move(binary_source2);
	EXPECT_TRUE(binary_source1.is_open());
	EXPECT_FALSE(binary_source2.is_open());
	EXPECT_EQ(binary_source1.position(), 2 * destination.size());
	EXPECT_EQ(binary_source1.read(destination), destination.size());
	EXPECT_EQ(binary_source1.position(), 3 * destination.size());
	EXPECT_TRUE(std::ranges::equal(destination.begin(), destination.end(), source.begin() + 2 * destination.size(),
								   source.begin() + 3 * destination.size()));
}

/// @brief Tests that a file binary source throws when its source file is not found.
TEST(BinarySourceTest, FileBinarySourceThrowsOnMissingFile)
{
	const tr::temporary_file temp_file1{"BinarySourceTest_FileBinarySourceThrowsOnMissingFile1", ".dat"};
	ASSERT_FALSE(std::filesystem::exists(temp_file1.path()));
	EXPECT_THROW((void)tr::file_binary_source{temp_file1.path()}, tr::file_not_found);

	/* File stream scope */ {
		std::ofstream file{temp_file1.path()};
	}
	ASSERT_TRUE(std::filesystem::exists(temp_file1.path()));
	tr::file_binary_source binary_source{temp_file1.path()};
	const tr::temporary_file temp_file2{"BinarySourceTest_FileBinarySourceThrowsOnMissingFile2", ".dat"};
	ASSERT_FALSE(std::filesystem::exists(temp_file2.path()));
	EXPECT_THROW(binary_source.open(temp_file2.path()), tr::file_not_found);
}