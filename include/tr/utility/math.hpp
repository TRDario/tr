#pragma once
#include "concepts.hpp"

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
	template <arithmetic T1, arithmetic T2> constexpr std::common_type_t<T1, T2> mod(T1 v, T2 mod);
	// Euclidian modulo operation.
	template <arithmetic T1, arithmetic T2> constexpr std::common_type_t<T1, T2> eucmod(T1 v, T2 mod);
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::integral To, std::floating_point From> constexpr To tr::round_cast(From from)
{
	return static_cast<To>(std::round(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::floor_cast(From from)
{
	return static_cast<To>(std::floor(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::ceil_cast(From from)
{
	return static_cast<To>(std::ceil(from));
}

template <tr::arithmetic T> constexpr T tr::abs(T v)
{
	return v >= 0 ? v : -v;
}

template <tr::arithmetic T1, tr::arithmetic T2> constexpr std::common_type_t<T1, T2> tr::mod(T1 v, T2 mod)
{
	if constexpr (std::floating_point<T1> || std::floating_point<T2>) {
		return static_cast<std::common_type_t<T1, T2>>(v) - static_cast<std::int64_t>(v / mod);
	}
	else {
		return static_cast<std::common_type_t<T1, T2>>(v % mod);
	}
}

template <tr::arithmetic T1, tr::arithmetic T2> constexpr std::common_type_t<T1, T2> tr::eucmod(T1 v, T2 mod)
{
	const std::common_type_t<T1, T2> normal_mod{tr::mod(v, mod)};
	return mod >= 0 ? normal_mod : normal_mod + mod;
}