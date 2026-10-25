/// @file
/// @brief Tests utility/binary_io.hpp.

#include <gtest/gtest.h>
#include <tr/utility/binary_io.hpp>
#include <tr/utility/binary_io_specializations.hpp>
#include <tr/utility/rng.hpp>

//

/// Helper template that checks for a binary round-trip of a value.
/// @tparam Value Type to round-trip.
/// @param expected Value to round-trip.
template <typename Value>
void expect_binary_round_trip(const Value& expected)
{
	std::stringstream stream;
	tr::write_binary(stream, expected);
	EXPECT_EQ(tr::read_binary<Value>(stream), expected);
}

/// Helper template that checks for a binary round-trip of an unordered container.
/// @tparam Container Type to round-trip.
/// @param expected Value to round-trip.
template <typename Container>
void expect_unordered_container_binary_round_trip(const Container& expected)
{
	std::stringstream stream;
	tr::write_binary(stream, expected);
	Container round_tripped{tr::read_binary<Container>(stream)};
	EXPECT_EQ(std::ranges::size(expected), std::ranges::size(round_tripped));
	for (const auto& value : round_tripped) {
		EXPECT_TRUE(std::ranges::contains(expected, value));
	}
}

//

/// Tests binary IO round-tripping of primitives.
TEST(BinaryIoTest, RoundTripsPrimitives)
{
	using namespace tr::integer_literals;

	/// Test enumerator.
	enum class test_enum : bool
	{
		no,
		yes
	};

	expect_binary_round_trip(0_i8);
	expect_binary_round_trip(1_i16);
	expect_binary_round_trip(-1_i32);
	expect_binary_round_trip(257_i64);
	expect_binary_round_trip(0_u8);
	expect_binary_round_trip(1_u16);
	expect_binary_round_trip(~0_u32);
	expect_binary_round_trip(257_u64);
	expect_binary_round_trip(0.5f);
	expect_binary_round_trip(std::numeric_limits<float>::infinity());
	expect_binary_round_trip(-3e-6);
	expect_binary_round_trip(std::numeric_limits<double>::denorm_min());
	expect_binary_round_trip(glm::vec2{-50, 90});
	expect_binary_round_trip(glm::ortho(0.0f, 1000.0f, 1000.0f, 0.0f));
	expect_binary_round_trip(std::pair{-3, 4.2e6f});
	expect_binary_round_trip(test_enum::yes);
}

/// Tests binary IO round-tripping of strings.
TEST(BinaryIoTest, RoundTripsStrings)
{
	const char* cstr1{"test1"};
	const char cstr2[]{"test2"};
	std::string_view strview{"test3"};
	std::string str{"test4"};
	std::string_view empty{};
	std::string with_nul{"\0\0\0\0TEST\0\0\0\0"};

	std::stringstream ios;
	tr::write_binary(ios, cstr1, cstr2, strview, str, empty, with_nul);
	EXPECT_EQ(tr::read_binary<std::string>(ios), cstr1);
	EXPECT_EQ(tr::read_binary<std::string>(ios), cstr2);
	EXPECT_EQ(tr::read_binary<std::string>(ios), strview);
	EXPECT_EQ(tr::read_binary<std::string>(ios), str);
	EXPECT_EQ(tr::read_binary<std::string>(ios), empty);
	EXPECT_EQ(tr::read_binary<std::string>(ios), with_nul);
}

/// Tests binary IO round-tripping of containers.
TEST(BinaryIoTest, RoundTripsContainers)
{
	expect_binary_round_trip(std::array<int, 5>{1, 2, 3, 4, 5});
	expect_binary_round_trip(std::vector<float>{});
	expect_binary_round_trip(std::vector<int>{1, 2, 3, 4, 5});
	expect_binary_round_trip(std::set<int>{1, 2, 3, 4, 5});
	expect_unordered_container_binary_round_trip(boost::unordered_flat_set<int>{1, 2, 3, 4, 5});
	expect_unordered_container_binary_round_trip(boost::unordered_node_set<int>{1, 2, 3, 4, 5});
	expect_binary_round_trip(std::map<int, std::string>{});
	expect_binary_round_trip(std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}});
	expect_unordered_container_binary_round_trip(boost::unordered_flat_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}});
	expect_unordered_container_binary_round_trip(boost::unordered_node_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}});
}

/// Tests binary IO round-tripping of magic bytes.
TEST(BinaryIoTest, RoundTripsMagic)
{
	std::stringstream ios;
	tr::write_binary_magic(ios, "tr");
	EXPECT_TRUE(tr::read_binary_magic(ios, "tr"));
}

/// Tests the detection of invalid magic bytes.
TEST(BinaryIoTest, DetectsInvalidMagic)
{
	std::stringstream ios;
	tr::write_binary_magic(ios, "t");
	EXPECT_FALSE(tr::read_binary_magic(ios, "tr"));
}

/// Tests flushing the contents of a stream as binary data.
TEST(BinaryIoTest, FlushesBinary)
{
	std::stringstream ios;
	ios.write("0123456789", 11);
	(void)tr::read_binary<char>(ios);
	(void)tr::read_binary<char>(ios);
	(void)tr::read_binary<char>(ios);
	const std::vector<std::byte> flushed{tr::flush_binary(ios)};
	EXPECT_STREQ(reinterpret_cast<const char*>(flushed.data()), "3456789");
}