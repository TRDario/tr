/// @file
/// @brief Provides mathematical utilities.

#pragma once
#include <tr/utility/concepts.hpp>

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
	[[nodiscard]] constexpr To round_cast(From from) noexcept
	{
		return static_cast<To>(std::round(from));
	}

	/// Casts a floating point number to an integer by flooring the value.
	/// @tparam To Target integer type.
	/// @tparam From Source floating-point type.
	/// @param from Source floating-point value.
	/// @return `from` floored to an integer.
	template <std::integral To, std::floating_point From>
	[[nodiscard]] constexpr To floor_cast(From from) noexcept
	{
		return static_cast<To>(std::floor(from));
	}

	/// Casts a floating point number to an integer by rounding the value up.
	/// @tparam To Target integer type.
	/// @tparam From Source floating-point type.
	/// @param from Source floating-point value.
	/// @return `from` rounded up to an integer.
	template <std::integral To, std::floating_point From>
	[[nodiscard]] constexpr To ceil_cast(From from) noexcept
	{
		return static_cast<To>(std::ceil(from));
	}

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Sign operation.
	/// @tparam Value type.
	/// @param v Value to get the sign of.
	/// @return `1` if the value is positive, `-1` if the value is negative, or `0`.
	template <arithmetic T>
	[[nodiscard]] constexpr T sgn(T v) noexcept
	{
		if constexpr (std::unsigned_integral<T>) {
			return v > T{0} ? T{1} : T{0};
		}
		else {
			return v > T{0} ? T{1} : v < T{0} ? T{-1} : T{0};
		}
	}

	/// Generic modulo operation.
	/// @tparam T1 Dividend type.
	/// @tparam T2 Divisor type.
	/// @param v Dividend value.
	/// @param mod Divisor value.
	/// @return `v % mod`.
	template <typename T1, typename T2>
	[[nodiscard]] constexpr auto mod(T1 v, T2 mod)
	{
		if constexpr ((std::floating_point<T1> || std::floating_point<T2>) && arithmetic<T1> && arithmetic<T2>) {
			return static_cast<std::common_type_t<T1, T2>>(v) - static_cast<i64>(v / mod) * mod;
		}
		else {
			return v % mod;
		}
	}

	/// Euclidian modulo operation (always returns in the range [0, `mod`)).
	/// @tparam T1 Dividend type.
	/// @tparam T2 Divisor type.
	/// @param v Dividend value.
	/// @param mod Divisor value.
	/// @return Euclidian `v % mod`.
	template <typename T1, typename T2>
	[[nodiscard]] constexpr auto eucmod(T1 v, T2 mod)
	{
		const auto normal_mod{tr::mod(v, mod)};
		return normal_mod >= decltype(normal_mod){} ? normal_mod : normal_mod + mod;
	}

	/// Linearly interpolates between two values.
	/// @tparam T Value type.
	/// @tparam Ratio Ratio type.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <typename T, std::floating_point Ratio>
	[[nodiscard]] constexpr T lerp(const T& x, const T& y, Ratio a) noexcept(noexcept(x * (1 - a) + y * a))
	{
		return x * (1 - a) + y * a;
	}

	/// Squaring operation.
	/// @tparam T Value type.
	/// @param v Value to square.
	/// @return `v²`.
	template <typename T>
	[[nodiscard]] constexpr auto sqr(const T& v) noexcept(noexcept(v * v))
	{
		return v * v;
	}

	/// @}
} // namespace tr