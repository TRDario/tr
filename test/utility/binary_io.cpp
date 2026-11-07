/// @file
/// @brief Tests utility/binary_io.hpp.

#include <gtest/gtest.h>
#include <tr/utility/binary_io.hpp>
#include <tr/utility/binary_io_specializations.hpp>
#include <tr/utility/circle.hpp>
#include <tr/utility/rng.hpp>

//

/// @brief Tests writing, then reading a series of objects.
TEST(BinaryIOTest, RoundTrips)
{
	using namespace tr::integer_literals;

	/// Test enumerator.
	enum class test_enum : bool
	{
		no,
		yes
	};

	tr::vector_binary_sink sink;
	// clang-format off
	tr::write_binary(
		sink,
		false,
		test_enum::yes,
		0_i8,
		1_i16,
		2_i32,
		3_i64,
		4_u8,
		5_u16,
		6_u32,
		7_u64,
		std::byte{8},
		9.0f,
		10.0,
		glm::u8vec2{11, 12},
		glm::vec2{13, 14},
		glm::ortho(15.0f, 16.0f, 17.0f, 18.0f),
		std::pair{19, 20.0},
		std::tuple{21, 22.0f, 23.0},
		std::array<int, 5>{24, 25, 26, 27, 28},
		"string",
		std::string_view{"streng"},
		std::string{"strang"},
		tr::inplace_string<32>{"strung"},
		std::vector<int>{1, 2, 3, 4, 5},
		std::set<int>{1, 2, 3, 4, 5},
		boost::unordered_flat_set<int>{1, 2, 3, 4, 5},
		boost::unordered_node_set<int>{1, 2, 3, 4, 5},
		std::map<int, std::string>{},
		std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}},
		boost::unordered_flat_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}},
		boost::unordered_node_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}},
		tr::rectangle<float>{30, 31, 32, 33}
	);
	// clang-format on

	tr::span_binary_source source{sink.vector()};
	EXPECT_EQ(tr::read_binary<bool>(source), false);
	EXPECT_EQ(tr::read_binary<test_enum>(source), test_enum::yes);
	EXPECT_EQ(tr::read_binary<tr::i8>(source), 0_i8);
	EXPECT_EQ(tr::read_binary<tr::i16>(source), 1_i16);
	EXPECT_EQ(tr::read_binary<tr::i32>(source), 2_i32);
	EXPECT_EQ(tr::read_binary<tr::i64>(source), 3_i64);
	EXPECT_EQ(tr::read_binary<tr::u8>(source), 4_u8);
	EXPECT_EQ(tr::read_binary<tr::u16>(source), 5_u16);
	EXPECT_EQ(tr::read_binary<tr::u32>(source), 6_u32);
	EXPECT_EQ(tr::read_binary<tr::u64>(source), 7_u64);
	EXPECT_EQ(tr::read_binary<std::byte>(source), std::byte{8});
	EXPECT_EQ(tr::read_binary<float>(source), 9.0f);
	EXPECT_EQ(tr::read_binary<double>(source), 10.0);
	EXPECT_EQ(tr::read_binary<glm::u8vec2>(source), (glm::u8vec2{11, 12}));
	EXPECT_EQ(tr::read_binary<glm::vec2>(source), (glm::vec2{13, 14}));
	EXPECT_EQ(tr::read_binary<glm::mat4>(source), glm::ortho(15.0f, 16.0f, 17.0f, 18.0f));
	EXPECT_EQ((tr::read_binary<std::pair<int, double>>(source)), (std::pair{19, 20.0}));
	EXPECT_EQ((tr::read_binary<std::tuple<int, float, double>>(source)), (std::tuple{21, 22.0f, 23.0}));
	EXPECT_EQ((tr::read_binary<std::array<int, 5>>(source)), (std::array<int, 5>{24, 25, 26, 27, 28}));
	EXPECT_EQ(tr::read_binary<std::string>(source), "string");
	EXPECT_EQ(tr::read_binary<std::string>(source), "streng");
	EXPECT_EQ(tr::read_binary<tr::inplace_string<32>>(source), "strang");
	EXPECT_EQ(tr::read_binary<tr::inplace_string<32>>(source), "strung");
	EXPECT_EQ(tr::read_binary<std::vector<int>>(source), (std::vector<int>{1, 2, 3, 4, 5}));
	EXPECT_EQ(tr::read_binary<std::set<int>>(source), (std::set<int>{1, 2, 3, 4, 5}));
	EXPECT_EQ(tr::read_binary<boost::unordered_flat_set<int>>(source), (boost::unordered_flat_set<int>{1, 2, 3, 4, 5}));
	EXPECT_EQ(tr::read_binary<boost::unordered_node_set<int>>(source), (boost::unordered_node_set<int>{1, 2, 3, 4, 5}));
	EXPECT_EQ((tr::read_binary<std::map<int, std::string>>(source)), (std::map<int, std::string>{}));
	EXPECT_EQ((tr::read_binary<std::map<int, std::string>>(source)), (std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));
	EXPECT_EQ((tr::read_binary<boost::unordered_flat_map<int, std::string>>(source)),
			  (boost::unordered_flat_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));
	EXPECT_EQ((tr::read_binary<boost::unordered_node_map<int, std::string>>(source)),
			  (boost::unordered_node_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));
	EXPECT_EQ(tr::read_binary<tr::rectangle<float>>(source), (tr::rectangle<float>{30, 31, 32, 33}));
}

/// @brief Tests throwing an exception when binary reading fails.
TEST(BinaryIOTest, ThrowsOnReadFail)
{
	constexpr std::array<std::byte, 5> source{};
	std::array<std::byte, 10> destination{};
	tr::span_binary_source binary_source{source};
	EXPECT_THROW(tr::read_binary(binary_source, destination), tr::binary_read_error);
}

/// @brief Tests throwing an exception when binary writing fails.
TEST(BinaryIOTest, ThrowsOnWriteFail)
{
	constexpr std::array<std::byte, 10> source{};
	std::array<std::byte, 5> destination{};
	tr::span_binary_sink binary_sink{destination};
	EXPECT_THROW(tr::write_binary(binary_sink, source), tr::binary_write_error);
}

//

/// @brief Tests writing and checking magic bytes.
TEST(BinaryIOTest, ChecksMagicBytes)
{
	tr::vector_binary_sink sink;
	tr::write_binary_magic(sink, "tr");
	tr::span_binary_source source{sink.vector()};
	EXPECT_TRUE(tr::check_binary_magic(source, "tr"));
}

/// @brief Tests writing and checking invalid magic bytes.
TEST(BinaryIOTest, DetectsInvalidMagicBytes)
{
	tr::vector_binary_sink sink;
	tr::write_binary_magic(sink, "tr");
	tr::span_binary_source source{sink.vector()};
	EXPECT_FALSE(tr::check_binary_magic(source, "tl"));
}