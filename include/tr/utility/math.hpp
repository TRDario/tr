/// @file
/// @brief Provides mathematical utilities.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/integer.hpp>

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
	/// @tparam X Value type.
	/// @param x Value to get the sign of.
	/// @return `1` if the value is positive, `-1` if the value is negative, or `0`.
	template <arithmetic X>
	[[nodiscard]] constexpr X sgn(X x) noexcept
	{
		if constexpr (std::unsigned_integral<X>) {
			return x > X{0} ? X{1} : X{0};
		}
		else {
			return x > X{0} ? X{1} : x < X{0} ? X{-1} : X{0};
		}
	}

	/// Generic modulo operation.
	/// @tparam X, Y Value types.
	/// @param x Dividend value.
	/// @param y Divisor value.
	/// @return `x % y`.
	template <typename X, typename Y>
	[[nodiscard]] constexpr auto mod(X x, Y y)
		requires requires { x % y; } || (arithmetic<X> && arithmetic<Y>)
	{
		if constexpr ((std::floating_point<X> || std::floating_point<Y>) && arithmetic<X> && arithmetic<Y>) {
			return static_cast<std::common_type_t<X, Y>>(x) - static_cast<i64>(x / y) * y;
		}
		else {
			return x % y;
		}
	}

	/// Euclidian modulo operation (always returns in the range [0, `mod`)).
	/// @tparam X, Y Value types.
	/// @param x Dividend value.
	/// @param y Divisor value.
	/// @return Euclidian `x % y`.
	template <typename X, typename Y>
	[[nodiscard]] constexpr auto eucmod(X x, Y y)
		requires requires { x % y; } || (arithmetic<X> && arithmetic<Y>)
	{
		const auto normal_mod{tr::mod(x, y)};
		return normal_mod >= decltype(normal_mod){} ? normal_mod : normal_mod + y;
	}

	/// Linearly interpolates between two values.
	/// @tparam X, Y, A Value types.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <typename X, typename Y, typename A>
	[[nodiscard]] constexpr auto lerp(const X& x, const Y& y, A a) noexcept(noexcept(x * (1 - a) + y * a))
		requires requires { x * (1 - a) + y * a; }
	{
		return x * (1 - a) + y * a;
	}

	/// Squaring operation.
	/// @tparam X Value type.
	/// @param x Value to square.
	/// @return `x²`.
	template <typename X>
	[[nodiscard]] constexpr auto sqr(const X& x) noexcept(noexcept(x * x))
		requires requires { x * x; }
	{
		return x * x;
	}

	/// @}
} // namespace tr