///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous useful mathematical operations.                                                                                //
//                                                                                                                                       //
// The rounding casts are equivalent to applying the corresponding rounding function before casting:                                     //
//     - tr::round_cast<int>(4.6) -> 5                                                                                                   //
//     - tr::floor_cast<u8>(10.9) -> u8(10)                                                                                              //
//     - tr::ceil_cast<i16>(10.1) -> i16(11)                                                                                             //
//                                                                                                                                       //
// tr::abs is a constexpr alternative to std::abs:                                                                                       //
//     - tr::abs(-4) -> 4                                                                                                                //
//                                                                                                                                       //
// tr::mod is a generic modulo operation (defaults to the % operator, but has custom overloads for floating-point numbers):              //
//     - tr::mod(9.5f, 5) -> 4.5f                                                                                                        //
//                                                                                                                                       //
// tr::eucmod is a euclidian modulo operation (unlike regular modulo cannot return a negative result):                                   //
//     - tr::eucmod(-1.5f, 5) -> 3.5f                                                                                                    //
//                                                                                                                                       //
// tr::lerp linearly interpolates between two values:                                                                                    //
//     - tr::lerp(1, 5, 0.5) -> 3                                                                                                        //
//                                                                                                                                       //
// tr::sqr returns the square of a value:                                                                                                //
//     - tr::sqr(3) -> 9                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"
#include <concepts>

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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
	template <typename T1, typename T2> constexpr auto mod(T1 v, T2 mod);
	// Euclidian modulo operation.
	template <typename T1, typename T2> constexpr auto eucmod(T1 v, T2 mod);
	// Linearly interpolates between two values.
	template <typename T, std::floating_point Ratio> constexpr T lerp(const T& x, const T& y, Ratio a);
	// Square operation.
	template <typename T> constexpr auto sqr(const T& v);
} // namespace tr

#include "impl/math.hpp" // IWYU pragma: export