///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements math.hpp.                                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../math.hpp"

////////////////////////////////////////////////////////////////// CASTS //////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////// MISCELLANEOUS //////////////////////////////////////////////////////////////

template <tr::arithmetic T> constexpr T tr::abs(T v)
{
	return v >= 0 ? v : -v;
}

template <tr::arithmetic T> constexpr T tr::sgn(T v)
{
	if constexpr (std::unsigned_integral<T>) {
		return v > T(0) ? T(1) : T(0);
	}
	else {
		return v > T(0) ? T(1) : v < T(0) ? T(-1) : T(0);
	}
}

template <typename T1, typename T2> constexpr auto tr::mod(T1 v, T2 mod)
{
	if constexpr ((std::floating_point<T1> || std::floating_point<T2>) && arithmetic<T1> && arithmetic<T2>) {
		return std::common_type_t<T1, T2>(v) - i64(v / mod) * mod;
	}
	else {
		return v % mod;
	}
}

template <typename T1, typename T2> constexpr auto tr::eucmod(T1 v, T2 mod)
{
	const auto normal_mod{tr::mod(v, mod)};
	return normal_mod >= decltype(normal_mod){} ? normal_mod : normal_mod + mod;
}

template <typename T, std::floating_point Ratio> constexpr T tr::lerp(const T& x, const T& y, Ratio a)
{
	return x * (1 - a) + y * a;
}

template <typename T> constexpr auto tr::sqr(const T& v)
{
	return v * v;
}