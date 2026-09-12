/// @file
/// @brief Provides `tr::norm_cast()`.

#pragma once
#include <tr/utility/macro.hpp>
#include <tr/utility/norm_cast.hpp>

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
	[[nodiscard]] constexpr To norm_cast(From from) noexcept
	{
		TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

		return static_cast<To>(from);
	}

	/// Converts a normalized integer value to another normalized integer value.
	/// @tparam To An integer type.
	/// @tparam From An integer type.
	/// @param from A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @return A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	template <std::integral To, std::integral From>
	[[nodiscard]] constexpr To norm_cast(From from) noexcept
	{
		using UFrom = std::make_unsigned_t<From>;
		using UTo = std::make_unsigned_t<To>;

		// Identity.
		if constexpr (std::same_as<From, To>) {
			return from;
		}
		// Same-width conversion.
		else if constexpr (sizeof(From) == sizeof(To)) {
			return static_cast<To>(static_cast<UFrom>(from) + 1 + std::numeric_limits<UFrom>::max() / 2);
		}
		// Narrowing conversion.
		else if constexpr (sizeof(From) > sizeof(To)) {
			return norm_cast<To>(static_cast<UTo>(norm_cast<UFrom>(from) >> ((sizeof(From) - sizeof(To)) * 8ULL)));
		}
		// Widening conversion.
		else {
			if constexpr (sizeof(To) != 8) {
				constexpr auto factor{std::numeric_limits<UTo>::max() / std::numeric_limits<UFrom>::max()};
				return norm_cast<To>(static_cast<UTo>(norm_cast<UFrom>(from) * factor));
			}
			else {
				return norm_cast<To>(norm_cast<double>(from));
			}
		}
	}

	/// Converts an integer representing a normalized value to a floating-point number in the range [0-1].
	/// @tparam To A floating-point type.
	/// @tparam From An integer type.
	/// @param from A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @return A value in the range [0-1].
	template <std::floating_point To, std::integral From>
	[[nodiscard]] constexpr To norm_cast(From from) noexcept
	{
		using UFrom = std::make_unsigned_t<From>;

		return static_cast<To>(static_cast<double>(norm_cast<UFrom>(from)) / static_cast<double>(std::numeric_limits<UFrom>::max()));
	}

	/// Converts a floating-point number in the range [0-1] to a normalized integer.
	/// @tparam To An integer type.
	/// @tparam From The value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	/// @param from A value in the range [0-1].
	/// @return A value mapped such that 0 is the minimum, and 1 the maximum value storable by the integer.
	template <std::integral To, std::floating_point From>
	[[nodiscard]] constexpr To norm_cast(From from) noexcept
	{
		TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

		using UTo = std::make_unsigned_t<To>;

		const double result{static_cast<double>(from) * static_cast<double>(std::numeric_limits<UTo>::max())};
		if constexpr (sizeof(To) == 8) {
			// Safety checks for upper limit rounding are necessary for 64-bit.
			return norm_cast<To>(result == static_cast<double>(std::numeric_limits<UTo>::max()) ? std::numeric_limits<UTo>::max()
																								: static_cast<UTo>(result));
		}
		else {
			return norm_cast<To>(static_cast<UTo>(result));
		}
	}

	/// @}
} // namespace tr