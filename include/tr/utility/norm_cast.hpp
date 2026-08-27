/// @file
/// @brief Provides `tr::norm_cast()`.

#pragma once

//

namespace tr
{
	/// @name Casts
	/// @{

	/// Converts a floating point number in the range [0-1] to another floating point number in the range [0-1].
	/// @tparam To A floating-point type.
	/// @tparam From A floating-point type.
	/// @param from A value in the range [0-1].
	/// @return A value in the range [0-1].
	template <std::floating_point To, std::floating_point From>
	constexpr To norm_cast(From from);

	/// Converts an integer representing a normalized value to a floating-point number in the range [0-1].
	/// @tparam To A floating-point type.
	/// @tparam From An integer type.
	/// @param from A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @return A value in the range [0-1].
	template <std::floating_point To, std::integral From>
	constexpr To norm_cast(From from);

	/// Converts a floating-point number in the range [0-1] to a normalized integer.
	/// @tparam To An integer type.
	/// @tparam From The value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @param from A value in the range [0-1].
	/// @return A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	template <std::integral To, std::floating_point From>
	constexpr To norm_cast(From from);

	/// Converts a normalized integer value to another normalized integer value.
	/// @tparam To An integer type.
	/// @tparam From An integer type.
	/// @param from A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @return A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	template <std::integral To, std::integral From>
	constexpr To norm_cast(From from);

	/// @}
} // namespace tr

#include "impl/norm_cast.hpp" // IWYU pragma: export