#pragma once
#include "common.hpp"

namespace tr {
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
	}; // namespace integer_literals
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

consteval tr::u8 tr::integer_literals::operator""_u8(unsigned long long v)
{
	return u8(v);
}

consteval tr::u16 tr::integer_literals::operator""_u16(unsigned long long v)
{
	return u16(v);
}

consteval tr::u32 tr::integer_literals::operator""_u32(unsigned long long v)
{
	return u32(v);
}

consteval tr::u64 tr::integer_literals::operator""_u64(unsigned long long v)
{
	return u64(v);
}

consteval tr::usize tr::integer_literals::operator""_uz(unsigned long long v)
{
	return usize(v);
}

consteval tr::i8 tr::integer_literals::operator""_i8(unsigned long long v)
{
	return i8(v);
}

consteval tr::i16 tr::integer_literals::operator""_i16(unsigned long long v)
{
	return i16(v);
}

consteval tr::i32 tr::integer_literals::operator""_i32(unsigned long long v)
{
	return i32(v);
}

consteval tr::i64 tr::integer_literals::operator""_i64(unsigned long long v)
{
	return i64(v);
}

consteval tr::ssize tr::integer_literals::operator""_z(unsigned long long v)
{
	return ssize(v);
}