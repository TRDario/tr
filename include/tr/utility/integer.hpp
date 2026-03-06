///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a set of integer aliases and fixed-width integer literals.                                                                   //
//                                                                                                                                       //
// Integer aliases are in the format iX/uX (for signed and unsigned respectively) alongside ssize/usize, and are in the namespace        //
// tr::integer_aliases, as well as tr::aliases.                                                                                          //
//                                                                                                                                       //
// Integer literals are in the format _iX/_uX (for signed and unsigned respectively) alongside _z/_uz, and are in the namespace          //
// tr::integer_literals, as well as tr::literals.                                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	inline namespace aliases {
		// Common integer aliases.
		inline namespace integer_aliases {
			using u8 = std::uint8_t;
			using u16 = std::uint16_t;
			using u32 = std::uint32_t;
			using u64 = std::uint64_t;
			using usize = std::size_t;
			using i8 = std::int8_t;
			using i16 = std::int16_t;
			using i32 = std::int32_t;
			using i64 = std::int64_t;
			using ssize = std::ptrdiff_t;
		} // namespace integer_aliases
	} // namespace aliases

	inline namespace literals {
		// Additional integer literals.
		inline namespace integer_literals {
			consteval u8 operator""_u8(unsigned long long v);
			consteval u16 operator""_u16(unsigned long long v);
			consteval u32 operator""_u32(unsigned long long v);
			consteval u64 operator""_u64(unsigned long long v);
			consteval usize operator""_uz(unsigned long long v);

			consteval i8 operator""_i8(unsigned long long v);
			consteval i16 operator""_i16(unsigned long long v);
			consteval i32 operator""_i32(unsigned long long v);
			consteval i64 operator""_i64(unsigned long long v);
			consteval ssize operator""_z(unsigned long long v);
		} // namespace integer_literals
	} // namespace literals
} // namespace tr

#include "impl/integer.hpp" // IWYU pragma: export