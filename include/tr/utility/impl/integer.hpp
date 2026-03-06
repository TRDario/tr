///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements integer.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../integer.hpp"

///////////////////////////////////////////////////////////// INTEGER LITERALS ////////////////////////////////////////////////////////////

consteval tr::u8 tr::literals::integer_literals::operator""_u8(unsigned long long v)
{
	return u8(v);
}

consteval tr::u16 tr::literals::integer_literals::operator""_u16(unsigned long long v)
{
	return u16(v);
}

consteval tr::u32 tr::literals::integer_literals::operator""_u32(unsigned long long v)
{
	return u32(v);
}

consteval tr::u64 tr::literals::integer_literals::operator""_u64(unsigned long long v)
{
	return u64(v);
}

consteval tr::usize tr::literals::integer_literals::operator""_uz(unsigned long long v)
{
	return usize(v);
}

consteval tr::i8 tr::literals::integer_literals::operator""_i8(unsigned long long v)
{
	return i8(v);
}

consteval tr::i16 tr::literals::integer_literals::operator""_i16(unsigned long long v)
{
	return i16(v);
}

consteval tr::i32 tr::literals::integer_literals::operator""_i32(unsigned long long v)
{
	return i32(v);
}

consteval tr::i64 tr::literals::integer_literals::operator""_i64(unsigned long long v)
{
	return i64(v);
}

consteval tr::ssize tr::literals::integer_literals::operator""_z(unsigned long long v)
{
	return ssize(v);
}