/// @file
/// @brief Provides integer aliases and fixed-width integer literals.

#pragma once
#include "common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr
{
	inline namespace aliases
	{
		/// Namespace containing integer aliases.
		inline namespace integer_aliases
		{
			/// 8-bit unsigned integer.
			using u8 = std::uint8_t;

			/// 16-bit unsigned integer.
			using u16 = std::uint16_t;

			/// 32-bit unsigned integer.
			using u32 = std::uint32_t;

			/// 64-bit unsigned integer.
			using u64 = std::uint64_t;

			/// Unsigned size integer.
			using usize = std::size_t;

			/// 8-bit signed integer.
			using i8 = std::int8_t;

			/// 16-bit signed integer.
			using i16 = std::int16_t;

			/// 32-bit signed integer.
			using i32 = std::int32_t;

			/// 64-bit signed integer.
			using i64 = std::int64_t;

			/// Signed size integer.
			using ssize = std::ptrdiff_t;
		} // namespace integer_aliases
	} // namespace aliases

	inline namespace literals
	{
		/// Namespace contaning integer literals.
		inline namespace integer_literals
		{
			/// 8-bit unsigned integer literal.
			/// @param[in] v Integer in the range [0, UINT8_MAX].
			/// @return An 8-bit unsigned integer.
			consteval u8 operator""_u8(unsigned long long v);

			/// 16-bit unsigned integer literal.
			/// @param[in] v Integer in the range [0, UINT16_MAX].
			/// @return A 16-bit unsigned integer.
			consteval u16 operator""_u16(unsigned long long v);

			/// 32-bit unsigned integer literal.
			/// @param[in] v Integer in the range [0, UINT32_MAX].
			/// @return A 32-bit unsigned integer.
			consteval u32 operator""_u32(unsigned long long v);

			/// 64-bit unsigned integer literal.
			/// @param[in] v Integer in the range [0, UINT64_MAX].
			/// @return A 64-bit unsigned integer.
			consteval u64 operator""_u64(unsigned long long v);

			/// 8-bit signed integer literal.
			/// @param[in] v Integer in the range [0, INT8_MAX].
			/// @return An 8-bit signed integer.
			consteval i8 operator""_i8(unsigned long long v);

			/// 16-bit signed integer literal.
			/// @param[in] v Integer in the range [0, INT16_MAX].
			/// @return A 16-bit signed integer.
			consteval i16 operator""_i16(unsigned long long v);

			/// 32-bit signed integer literal.
			/// @param[in] v Integer in the range [0, INT32_MAX].
			/// @return A 32-bit signed integer.
			consteval i32 operator""_i32(unsigned long long v);

			/// 64-bit signed integer literal.
			/// @param[in] v Integer in the range [0, INT64_MAX].
			/// @return A 64-bit signed integer.
			consteval i64 operator""_i64(unsigned long long v);
		} // namespace integer_literals
	} // namespace literals
} // namespace tr

#include "impl/integer.hpp" // IWYU pragma: export