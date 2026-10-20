/// @file
/// @brief Tests utility/mstream.hpp.

#include <gtest/gtest.h>
#include <tr/utility/mstream.hpp>

//

/// Tests seeking and reading from a memory stream.
TEST(MstreamTest, SeeksAndReads)
{
	const std::array<char, 8> buffer{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	tr::imstream stream{std::span<const char>{buffer}};
	std::array<char, 3> result{};

	stream.seekg(2, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.read(result.data(), result.size());
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 3>{'c', 'd', 'e'}));

	stream.seekg(-2, std::ios::end);
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(stream.get(), 'g');
	EXPECT_TRUE(stream.good());

	stream.seekg(-1, std::ios::cur);
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(stream.get(), 'g');
}

/// Tests an overlong read from a memory stream failing.
TEST(MstreamTest, OverlongReadFails)
{
	const std::array<char, 3> buffer{'a', 'b', 'c'};
	tr::imstream stream{std::span<const char>{buffer}};
	std::array<char, 4> result{};

	stream.read(result.data(), result.size());
	EXPECT_EQ(stream.gcount(), 3);
	EXPECT_EQ((std::array<char, 3>{result[0], result[1], result[2]}), buffer);
	EXPECT_TRUE(stream.eof());
	EXPECT_TRUE(stream.fail());
}

/// Tests seeking and writing to a memory stream.
TEST(MstreamTest, SeeksAndWrites)
{
	std::array<char, 8> buffer;
	buffer.fill('_');
	tr::omstream stream{buffer};

	stream.seekp(2, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.write("abc", 3);
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(buffer, (std::array<char, 8>{'_', '_', 'a', 'b', 'c', '_', '_', '_'}));

	stream.seekp(-1, std::ios::end);
	ASSERT_TRUE(stream.good());
	stream.put('z');
	EXPECT_TRUE(stream.good());
	EXPECT_EQ(buffer.back(), 'z');
}

/// Tests a truncated write to a memory stream failing.
TEST(MstreamTest, TruncatedWriteFails)
{
	std::array<char, 3> buffer{'_', '_', '_'};
	tr::omstream stream{std::span<char>{buffer}};

	stream.write("abcd", 4);
	EXPECT_EQ(buffer, (std::array<char, 3>{'a', 'b', 'c'}));
	EXPECT_TRUE(stream.fail());
}

/// Tests seeking to the end of and past the end of a memory stream.
TEST(MstreamTest, SeeksToEnd)
{
	std::array<std::byte, 4> buffer{};
	tr::omstream stream{std::span<std::byte>{buffer}};

	stream.seekp(0, std::ios::end);
	EXPECT_TRUE(stream.good());
	EXPECT_EQ(stream.tellp(), std::streampos{4});

	stream.seekp(1, std::ios::end);
	EXPECT_TRUE(stream.fail());
}

/// Tests an unsigned char buffer with an output memory stream.
TEST(MstreamTest, OmstreamConstructsWithUnsignedCharBuffer)
{
	std::array<unsigned char, 4> buffer{};
	tr::omstream stream{buffer};

	stream.write("test", 4);
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(buffer, (std::array<unsigned char, 4>{'t', 'e', 's', 't'}));
}

/// Tests a byte buffer with an output memory stream.
TEST(MstreamTest, OmstreamConstructsWithByteBuffer)
{
	std::array<std::byte, 4> buffer{};
	tr::omstream stream{std::span<std::byte>{buffer}};

	stream.write("test", 4);
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(buffer, (std::array{std::byte{'t'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}}));
}

/// Tests an unsigned char buffer with an input memory stream.
TEST(MstreamTest, ImstreamConstructsWithUnsignedCharBuffer)
{
	const std::array<unsigned char, 4> buffer{'t', 'e', 's', 't'};
	tr::imstream stream{std::span<const unsigned char>{buffer}};
	std::array<char, 4> result{};

	stream.read(result.data(), result.size());
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 4>{'t', 'e', 's', 't'}));
}

/// Tests a byte buffer with an input memory stream.
TEST(MstreamTest, ImstreamConstructsWithByteBuffer)
{
	const std::array buffer{std::byte{'t'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}};
	tr::imstream stream{std::span<const std::byte>{buffer}};
	std::array<char, 4> result{};

	stream.read(result.data(), result.size());
	ASSERT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 4>{'t', 'e', 's', 't'}));
}

/// Tests reading and writing to a memory stream.
TEST(MstreamTest, ReadsAndWrites)
{
	std::array<char, 6> buffer{'a', 'b', 'c', 'd', 'e', 'f'};
	tr::mstream stream{std::span<char>{buffer}};

	stream.seekg(1, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.seekp(3, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.put('X');
	ASSERT_TRUE(stream.good());

	std::array<char, 2> result{};
	stream.read(result.data(), result.size());
	EXPECT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 2>{'b', 'c'}));
	EXPECT_EQ(buffer, (std::array<char, 6>{'a', 'b', 'c', 'X', 'e', 'f'}));
}

/// Tests an unsigned char buffer with an output memory stream.
TEST(MstreamTest, MstreamConstructsWithUnsignedCharBuffer)
{
	std::array<unsigned char, 3> buffer{'a', 'b', 'c'};
	tr::mstream stream{std::span<unsigned char>{buffer}};
	stream.seekp(1, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.put('X');
	ASSERT_TRUE(stream.good());

	std::array<char, 3> result{};
	stream.read(result.data(), result.size());
	EXPECT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 3>{'a', 'X', 'c'}));
	EXPECT_EQ(buffer, (std::array<unsigned char, 3>{'a', 'X', 'c'}));
}

/// Tests a byte buffer with an output memory stream.
TEST(MstreamTest, MstreamConstructsWithByteBuffer)
{
	std::array buffer{std::byte{'a'}, std::byte{'b'}, std::byte{'c'}};
	tr::mstream stream{std::span<std::byte>{buffer}};
	stream.seekp(1, std::ios::beg);
	ASSERT_TRUE(stream.good());
	stream.put('X');
	ASSERT_TRUE(stream.good());

	std::array<char, 3> result{};
	stream.read(result.data(), result.size());
	EXPECT_TRUE(stream.good());
	EXPECT_EQ(result, (std::array<char, 3>{'a', 'X', 'c'}));
	EXPECT_EQ(buffer, (std::array{std::byte{'a'}, std::byte{'X'}, std::byte{'c'}}));
}