/// @file
/// @brief Implements integer.hpp.

#pragma once
#include "../integer.hpp"

//

consteval tr::u8 tr::literals::integer_literals::operator""_u8(unsigned long long v)
{
	return static_cast<u8>(v);
}

consteval tr::u16 tr::literals::integer_literals::operator""_u16(unsigned long long v)
{
	return static_cast<u16>(v);
}

consteval tr::u32 tr::literals::integer_literals::operator""_u32(unsigned long long v)
{
	return static_cast<u32>(v);
}

consteval tr::u64 tr::literals::integer_literals::operator""_u64(unsigned long long v)
{
	return static_cast<u64>(v);
}

consteval tr::i8 tr::literals::integer_literals::operator""_i8(unsigned long long v)
{
	return static_cast<i8>(v);
}

consteval tr::i16 tr::literals::integer_literals::operator""_i16(unsigned long long v)
{
	return static_cast<i16>(v);
}

consteval tr::i32 tr::literals::integer_literals::operator""_i32(unsigned long long v)
{
	return static_cast<i32>(v);
}

consteval tr::i64 tr::literals::integer_literals::operator""_i64(unsigned long long v)
{
	return static_cast<i64>(v);
}

//

namespace tr
{
	/// Specialization of `tr::size_type` for integers (S > UINT32_MAX).
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
		requires(S > UINT32_MAX)
	struct size_type<S>
	{
		/// Integer type that can store an integer in the range [0, `S`].
		using type = u64;
	};

	/// Specialization of `tr::size_type` for integers (UINT16_MAX < S <= UINT32_MAX).
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
		requires(S > UINT16_MAX && S <= UINT32_MAX)
	struct size_type<S>
	{
		/// Integer type that can store an integer in the range [0, `S`].
		using type = u32;
	};

	/// Specialization of `tr::size_type` for integers (UINT8_MAX < S <= UINT16_MAX).
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
		requires(S > UINT8_MAX && S <= UINT16_MAX)
	struct size_type<S>
	{
		/// Integer type that can store an integer in the range [0, `S`].
		using type = u16;
	};

	/// Specialization of `tr::size_type` for integers (S < UINT8_MAX).
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
		requires(S <= UINT8_MAX)
	struct size_type<S>
	{
		/// Integer type that can store an integer in the range [0, `S`].
		using type = u8;
	};
} // namespace tr