///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/enum.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility/enum.hpp>

enum class test_enum : tr::u8 {
	a = 1,
	b = 2,
	c = 4,
	ab = 3,
	ac = 5,
	bc = 6,
	na = 254
};
TR_DEFINE_ENUM_BITMASK_OPERATORS(test_enum);

static_assert(test_enum::ac == test_enum{tr::to_underlying(test_enum::ac)});

static_assert((test_enum::ac & test_enum::a) == test_enum::a);
static_assert((test_enum::ac & test_enum::c) == test_enum::c);
static_assert((test_enum::a | test_enum::c) == test_enum::ac);
static_assert((test_enum::a ^ test_enum::b) == test_enum::ab);
static_assert((test_enum::a ^ test_enum::ab) == test_enum::b);
static_assert(!(test_enum::a ^ test_enum::a));
static_assert(~test_enum::a == test_enum::na);
static_assert([] {
	test_enum enm{test_enum::a};
	enm |= test_enum::b;
	enm &= test_enum::b;
	enm ^= test_enum::bc;
	return enm == test_enum::c;
}());

class test_class {
  private:
	enum class private_enum : tr::u8 {
		a = 1,
		b = 2,
		c = 4,
		ab = 3,
		ac = 5,
		bc = 6,
		na = 254
	};
	TR_DEFINE_PRIVATE_ENUM_BITMASK_OPERATORS(private_enum);

	consteval void asserts()
	{
		static_assert((private_enum::ac & private_enum::a) == private_enum::a);
		static_assert((private_enum::ac & private_enum::c) == private_enum::c);
		static_assert((private_enum::a | private_enum::c) == private_enum::ac);
		static_assert((private_enum::a ^ private_enum::b) == private_enum::ab);
		static_assert((private_enum::a ^ private_enum::ab) == private_enum::b);
		static_assert(!(private_enum::a ^ private_enum::a));
		static_assert(~private_enum::a == private_enum::na);
	};
};