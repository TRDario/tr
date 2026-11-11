/// @file
/// @brief Tests utility/enum.hpp.

#include <tr/utility/enum.hpp>
#include <tr/utility/integer.hpp>

//

namespace
{
	/// @brief Test bitmask enum.
	enum class test_enum : tr::u8
	{
		a = 1,
		b = 2,
		c = 4,
		ab = 3,
		ac = 5,
		bc = 6,
		na = 254
	};

	TR_DEFINE_ENUM_BITMASK_OPERATORS(test_enum);
} // namespace

//

/// @brief Tests AND on the bitmask enum.
static_assert((test_enum::ac & test_enum::a) == test_enum::a);

/// @brief Tests AND on the bitmask enum.
static_assert((test_enum::ac & test_enum::c) == test_enum::c);

/// @brief Tests OR on the bitmask enum.
static_assert((test_enum::a | test_enum::c) == test_enum::ac);

/// @brief Tests XOR on the bitmask enum.
static_assert((test_enum::a ^ test_enum::b) == test_enum::ab);

/// @brief Tests XOR on the bitmask enum.
static_assert((test_enum::a ^ test_enum::ab) == test_enum::b);

/// @brief Tests XOR on the bitmask enum.
static_assert(!(test_enum::a ^ test_enum::a));

/// @brief Tests NOT on the bitmask enum.
static_assert(~test_enum::a == test_enum::na);

/// @brief Tests the mutating bitmask operations on the bitmask enum.
static_assert([] {
	test_enum enm{test_enum::a};
	enm |= test_enum::b;
	enm &= test_enum::b;
	enm ^= test_enum::bc;
	return enm == test_enum::c;
}());

/// @brief Test class holding a private bitmask enumerator.
class test_class
{
  private:
	/// @brief Private test bitmask enum.
	enum class private_enum : tr::u8
	{
		a = 1,
		b = 2,
		c = 4,
		ab = 3,
		ac = 5,
		bc = 6,
		na = 254
	};

	TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(private_enum);

	//

	/// @brief Static asserts wrapped in a function.
	consteval void asserts()
	{
		/// @brief Tests AND on the bitmask enum.
		static_assert((private_enum::ac & private_enum::a) == private_enum::a);

		/// @brief Tests AND on the bitmask enum.
		static_assert((private_enum::ac & private_enum::c) == private_enum::c);

		/// @brief Tests OR on the bitmask enum.
		static_assert((private_enum::a | private_enum::c) == private_enum::ac);

		/// @brief Tests XOR on the bitmask enum.
		static_assert((private_enum::a ^ private_enum::b) == private_enum::ab);

		/// @brief Tests XOR on the bitmask enum.
		static_assert((private_enum::a ^ private_enum::ab) == private_enum::b);

		/// @brief Tests XOR on the bitmask enum.
		static_assert(!(private_enum::a ^ private_enum::a));

		/// @brief Tests NOT on the bitmask enum.
		static_assert(~private_enum::a == private_enum::na);
	};
};