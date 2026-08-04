/// @file
/// @brief Provides mathematical utilities.

#pragma once
#include "concepts.hpp"
#include <concepts>

//

namespace tr
{
	/// @name Casts
	/// @{

	/// Casts a floating point number to an integer by rounding the value.
	/// @tparam To Target integer type.
	/// @tparam From Source floating-point type.
	/// @param from Source floating-point value.
	/// @return `from` rounded to an integer.
	template <std::integral To, std::floating_point From>
	constexpr To round_cast(From from);

	/// Casts a floating point number to an integer by flooring the value.
	/// @tparam To Target integer type.
	/// @tparam From Source floating-point type.
	/// @param from Source floating-point value.
	/// @return `from` floored to an integer.
	template <std::integral To, std::floating_point From>
	constexpr To floor_cast(From from);

	/// Casts a floating point number to an integer by rounding the value up.
	/// @tparam To Target integer type.
	/// @tparam From Source floating-point type.
	/// @param from Source floating-point value.
	/// @return `from` rounded up to an integer.
	template <std::integral To, std::floating_point From>
	constexpr To ceil_cast(From from);

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Sign operation.
	/// @tparam Value type.
	/// @param v Value to get the sign of.
	/// @return `1` if the value is positive, `-1` if the value is negative, or `0`.
	template <arithmetic T>
	constexpr T sgn(T v);

	/// Generic modulo operation.
	/// @tparam T1 Dividend type.
	/// @tparam T2 Divisor type.
	/// @param v Dividend value.
	/// @param mod Divisor value.
	/// @return `v % mod`.
	template <typename T1, typename T2>
	constexpr auto mod(T1 v, T2 mod);

	/// Euclidian modulo operation (always returns in the range [0, `mod`)).
	/// @tparam T1 Dividend type.
	/// @tparam T2 Divisor type.
	/// @param v Dividend value.
	/// @param mod Divisor value.
	/// @return Euclidian `v % mod`.
	template <typename T1, typename T2>
	constexpr auto eucmod(T1 v, T2 mod);

	/// Linearly interpolates between two values.
	/// @tparam T Value type.
	/// @tparam Ratio Ratio type.
	/// @param x First sample value.
	/// @param y Second sample value.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <typename T, std::floating_point Ratio>
	constexpr T lerp(const T& x, const T& y, Ratio a);

	// Squaring operation.
	/// @tparam T Value type.
	/// @param v Value to square.
	/// @return `v²`.
	template <typename T>
	constexpr auto sqr(const T& v);

	/// @}
} // namespace tr

#include "impl/math.hpp" // IWYU pragma: export