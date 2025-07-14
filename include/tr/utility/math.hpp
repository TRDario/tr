#pragma once
#include "common.hpp"

namespace tr {
	/******************************************************************************************************************
	 * Casts a floating point number to an integer by rounding the value.
	 *
	 * @param[in] from The number to cast.
	 *
	 * @return An integer holding the rounded number.
	 *****************************************************************************************************************/
	template <std::integral To, std::floating_point From> constexpr To round_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Casts a floating point number to an integer by flooring the value.
	 *
	 * @param[in] from The number to cast.
	 *
	 * @return An integer holding the floored number.
	 *****************************************************************************************************************/
	template <std::integral To, std::floating_point From> constexpr To floor_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Casts a floating point number to an integer by getting its ceiling.
	 *
	 * @param[in] from The number to cast.
	 *
	 * @return An integer holding the ceiling of the floating point value.
	 *****************************************************************************************************************/
	template <std::integral To, std::floating_point From> constexpr To ceil_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Generic modulo operation.
	 *
	 * @param[in] v The value to modulo.
	 * @param[in] mod The modulus.
	 *
	 * @return The modulo of v by mod, standard promotion rules apply.
	 ******************************************************************************************************************/
	template <arithmetic T1, arithmetic T2> std::common_type_t<T1, T2> mod(T1 v, T2 mod) noexcept;

	/******************************************************************************************************************
	 * Euclidian modulo operation.
	 *
	 * @param[in] v The value to modulo.
	 * @param[in] mod The modulus.
	 *
	 * @return The euclidian modulo of v by mod, standard promotion rules apply.
	 ******************************************************************************************************************/
	template <arithmetic T1, arithmetic T2> constexpr std::common_type_t<T1, T2> eucmod(T1 v, T2 mod) noexcept;
} // namespace tr

/// @cond IMPLEMENTATION

template <std::integral To, std::floating_point From> constexpr To tr::round_cast(From from) noexcept
{
	return static_cast<To>(std::round(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::floor_cast(From from) noexcept
{
	return static_cast<To>(std::floor(from));
}

template <std::integral To, std::floating_point From> constexpr To tr::ceil_cast(From from) noexcept
{
	return static_cast<To>(std::ceil(from));
}

template <tr::arithmetic T1, tr::arithmetic T2> std::common_type_t<T1, T2> tr::mod(T1 v, T2 mod) noexcept
{
	if constexpr (std::floating_point<T1> || std::floating_point<T2>) {
		return std::fmod(static_cast<std::common_type_t<T1, T2>>(v), static_cast<std::common_type_t<T1, T2>>(mod));
	}
	else {
		return std::common_type_t<T1, T2>(v % mod);
	}
}

template <tr::arithmetic T1, tr::arithmetic T2> constexpr std::common_type_t<T1, T2> tr::eucmod(T1 v, T2 mod) noexcept
{
	const std::common_type_t<T1, T2> normal_mod{tr::mod(v, mod)};
	return mod >= 0 ? normal_mod : normal_mod + mod;
}

/// @endcond
