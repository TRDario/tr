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