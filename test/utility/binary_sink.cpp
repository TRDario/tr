/// @file
/// @brief Tests utility/binary_sink.hpp

#include <gtest/gtest.h>
#include <tr/utility/binary_sink.hpp>
#include <tr/utility/binary_source.hpp>
#include <tr/utility/temporary_file.hpp>

//

/// @brief Tests that `tr::span_binary_sink` fulfills `tr::binary_sink`.
static_assert(tr::binary_sink<tr::span_binary_sink>, "tr::span_binary_sink should fulfill tr::binary_sink, but doesn't!");

/// @brief Tests that `tr::vector_binary_sink` fulfills `tr::binary_sink`.
static_assert(tr::binary_sink<tr::vector_binary_sink>, "tr::vector_binary_sink should fulfill tr::binary_sink, but doesn't!");

/// @brief Tests that `tr::file_binary_sink` fulfills `tr::binary_sink`.
static_assert(tr::binary_sink<tr::file_binary_sink>, "tr::file_binary_sink should fulfill tr::binary_sink, but doesn't!");

//

/// @brief Tests that a span binary sink writes bytes properly.
TEST(BinarySinkTest, SpanBinarySinkWrites)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	std::array<std::byte, 10> destination{};

	tr::span_binary_sink binary_sink{destination};
	EXPECT_EQ(binary_sink.write(source), source.size());
	EXPECT_EQ(binary_sink.position(), source.size());
	EXPECT_EQ(source, destination);
}

/// @brief Tests that a span binary sink only writes up to the end of the span and no more.
TEST(BinarySinkTest, SpanBinarySinkStopsAtSpanEnd)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};
	std::array<std::byte, 5> destination{};

	tr::span_binary_sink binary_sink{destination};
	EXPECT_EQ(binary_sink.write(source), destination.size());
	EXPECT_EQ(binary_sink.position(), destination.size());
	EXPECT_TRUE(std::ranges::equal(source.begin(), source.begin() + 5, destination.begin(), destination.end()));
	EXPECT_EQ(binary_sink.write(source), 0);
}

//

/// @brief Tests that a vector binary sink writes bytes properly.
TEST(BinarySinkTest, VectorBinarySinkWrites)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};

	tr::vector_binary_sink binary_sink;
	EXPECT_EQ(binary_sink.write(source), source.size());
	EXPECT_EQ(binary_sink.position(), source.size());
	EXPECT_EQ(binary_sink.vector().size(), source.size());
	EXPECT_TRUE(std::ranges::equal(source, binary_sink.vector()));
}

//

/// @brief Tests that a file binary sink writes bytes properly.
TEST(BinarySinkTest, FileBinarySinkWrites)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};

	const tr::temporary_file temp_file{"BinarySinkTest_FileBinarySinkWrites", ".dat"};
	tr::file_binary_sink binary_sink{temp_file.path()};
	ASSERT_TRUE(binary_sink);
	EXPECT_EQ(binary_sink.write(source), source.size());
	binary_sink.close();

	tr::file_binary_source binary_source{temp_file.path()};
	ASSERT_TRUE(binary_source);
	std::array<std::byte, 10> destination;
	EXPECT_EQ(binary_source.read(destination), destination.size());
	EXPECT_TRUE(std::ranges::equal(source, destination));
}

/// @brief Tests that a closed file binary sink does not write anything.
TEST(BinarySinkTest, ClosedFileBinarySinkDoesNotWrite)
{
	constexpr std::array<std::byte, 10> source{
		std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
		std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9},
	};

	const tr::temporary_file temp_file{"BinarySinkTest_ClosedFileBinarySinkDoesNotWrite", ".dat"};
	tr::file_binary_sink binary_sink{temp_file.path()};
	binary_sink.close();
	ASSERT_FALSE(binary_sink);
	EXPECT_EQ(binary_sink.write(source), 0);
}

/// @brief Tests closing and re-opening a file binary sink.
TEST(BinarySinkTest, FileBinarySinkClosesAndOpens)
{
	const tr::temporary_file temp_file{"BinarySinkTest_FileBinarySinkClosesAndOpens", ".dat"};
	tr::file_binary_sink binary_sink{temp_file.path()};
	ASSERT_TRUE(binary_sink);
	ASSERT_TRUE(binary_sink.is_open());

	binary_sink.close();
	EXPECT_FALSE(binary_sink);
	EXPECT_FALSE(binary_sink.is_open());

	binary_sink.open(temp_file.path());
	EXPECT_TRUE(binary_sink);
	EXPECT_TRUE(binary_sink.is_open());
}

/// @brief Tests that moving a file binary sink transfers the opened file and position when moving.
TEST(BinarySinkTest, FileBinarySinkTransfersFileOwnershipAndPositionOnMove)
{
	constexpr std::array<std::byte, 2> source1{std::byte{0}, std::byte{1}};
	constexpr std::array<std::byte, 2> source2{std::byte{2}, std::byte{3}};
	constexpr std::array<std::byte, 2> source3{std::byte{4}, std::byte{5}};
	const tr::temporary_file temp_file{"BinarySinkTest_FileBinarySinkTransfersFileOwnershipAndPositionOnMove", ".dat"};

	tr::file_binary_sink binary_sink1{temp_file.path()};
	ASSERT_TRUE(binary_sink1.is_open());
	ASSERT_EQ(binary_sink1.write(source1), source1.size());
	ASSERT_EQ(binary_sink1.position(), source1.size());

	tr::file_binary_sink binary_sink2{std::move(binary_sink1)};
	EXPECT_FALSE(binary_sink1.is_open());
	EXPECT_TRUE(binary_sink2.is_open());
	EXPECT_EQ(binary_sink2.position(), source1.size());
	EXPECT_EQ(binary_sink2.write(source2), source2.size());
	EXPECT_EQ(binary_sink2.position(), source1.size() + source2.size());

	binary_sink1 = std::move(binary_sink2);
	EXPECT_TRUE(binary_sink1.is_open());
	EXPECT_FALSE(binary_sink2.is_open());
	EXPECT_EQ(binary_sink1.position(), source1.size() + source2.size());
	EXPECT_EQ(binary_sink1.write(source3), source3.size());
	EXPECT_EQ(binary_sink1.position(), source1.size() + source2.size() + source3.size());

	binary_sink1.close();
	ASSERT_FALSE(binary_sink1);
	tr::file_binary_source binary_source{temp_file.path()};
	ASSERT_TRUE(binary_source);
	std::array<std::byte, 6> destination;
	EXPECT_EQ(binary_source.read(destination), destination.size());
	EXPECT_TRUE(std::ranges::equal(destination, std::views::join(std::array{source1, source2, source3})));
}