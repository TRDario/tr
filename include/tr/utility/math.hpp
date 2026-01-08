///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous useful mathematical operations.                                                                                //
//                                                                                                                                       //
// The rounding casts are equivalent to applying the corresponding rounding function before casting:                                     //
//     - tr::round_cast<int>(4.6) -> 5                                                                                                   //
//     - tr::floor_cast<u8>(10.9) -> u8(10)                                                                                              //
//     - tr::ceil_cast<i16>(10.1) -> i16(11)                                                                                             //
//                                                                                                                                       //
// tr::abs is a constexpr alternative to std::abs.                                                                                       //
//     - tr::abs(-4) -> 4                                                                                                                //
//                                                                                                                                       //
// tr::mod is a generic modulo operation (defaults to the % operator, but has custom overloads for floating-point numbers).              //
//     - tr::mod(9.5f, 5) -> 4.5f                                                                                                        //
//                                                                                                                                       //
// tr::eucmod is a euclidian modulo operation (unlike regular modulo cannot return a negative result).                                   //
//     - tr::eucmod(-1.5f, 5) -> 3.5f                                                                                                    //
//                                                                                                                                       //
// tr::lerp linearly interpolates between two values.                                                                                    //
//     - tr::lerp(1, 5, 0.5) -> 3                                                                                                        //
//                                                                                                                                       //
// tr::sqr returns the square of a value.                                                                                                //
//     - tr::sqr(3) -> 9                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"
#include <concepts>

namespace tr {
	// Casts a floating point number to an integer by rounding the value.
	template <std::integral To, std::floating_point From> constexpr To round_cast(From from);
	// Casts a floating point number to an integer by flooring the value.
	template <std::integral To, std::floating_point From> constexpr To floor_cast(From from);
	// Casts a floating point number to an integer by getting its ceiling.
	template <std::integral To, std::floating_point From> constexpr To ceil_cast(From from);

	// Constexpr abs function.
	template <arithmetic T> constexpr T abs(T v);
	// Generic modulo operation.
	template <class T1, class T2> constexpr auto mod(T1 v, T2 mod);
	// Euclidian modulo operation.
	template <class T1, class T2> constexpr auto eucmod(T1 v, T2 mod);
	// Linearly interpolates between two values.
	template <class T1, std::floating_point T2> constexpr T1 lerp(const T1& x, const T1& y, T2 a);
	// Square operation.
	template <class T> constexpr auto sqr(const T& v);
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::integral To, std::floating_point From> constexpr To tr::round_cast(From from)
{
	return To(std::round(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::floor_cast(From from)
{
	return To(std::floor(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::ceil_cast(From from)
{
	return To(std::ceil(from));
}

//

template <tr::arithmetic T> constexpr T tr::abs(T v)
{
	return v >= 0 ? v : -v;
}

template <class T1, class T2> constexpr auto tr::mod(T1 v, T2 mod)
{
	if constexpr ((std::floating_point<T1> || std::floating_point<T2>) && arithmetic<T1> && arithmetic<T2>) {
		return std::common_type_t<T1, T2>(v) - i64(v / mod) * mod;
	}
	else {
		return v % mod;
	}
}

template <class T1, class T2> constexpr auto tr::eucmod(T1 v, T2 mod)
{
	const auto normal_mod{tr::mod(v, mod)};
	return normal_mod >= decltype(normal_mod){} ? normal_mod : normal_mod + mod;
}

template <class T1, std::floating_point T2> constexpr T1 tr::lerp(const T1& x, const T1& y, T2 a)
{
	return x * (1 - a) + y * a;
}

template <class T> constexpr auto tr::sqr(const T& v)
{
	return v * v;
}