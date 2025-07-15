#pragma once
#include "macro.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup norm_cast Normalized Cast
	 *  Functions for casting between normalized forms.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Converts a floating point number in the range [0-1] to another floating point number in the range [0-1].
	 *
	 * @param[in] from A floating point number in the range [0-1].
	 *
	 * @return A floating point number in the range [0-1].
	 ******************************************************************************************************************/
	template <std::floating_point To, std::floating_point From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a normalized integer value to a floating point number in the range [0-1].
	 *
	 * @param[in] from A normalized integer value.
	 *
	 * @return A floating point number in the range [0-1].
	 ******************************************************************************************************************/
	template <std::floating_point To, std::integral From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a floating point number in the range [0-1] to a normalized integer.
	 *
	 * @param[in] from A floating point number in the range [0-1].
	 *
	 * @return A normalized integer.
	 ******************************************************************************************************************/
	template <std::integral To, std::floating_point From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a normalized integer value to another normalied integer value.
	 *
	 * @param[in] from A normalized integer value.
	 *
	 * @return A normalized integer value.
	 ******************************************************************************************************************/
	template <std::integral To, std::integral From> constexpr To norm_cast(From from) noexcept;

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <std::floating_point To, std::floating_point From> constexpr To tr::norm_cast(From from) noexcept
{
	TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

	return static_cast<To>(from);
}

template <std::floating_point To, std::integral From> constexpr To tr::norm_cast(From from) noexcept
{
	using UFrom = std::make_unsigned_t<From>;

	return static_cast<To>(static_cast<double>(norm_cast<UFrom>(from)) / static_cast<double>(std::numeric_limits<UFrom>::max()));
}

template <std::integral To, std::floating_point From> constexpr To tr::norm_cast(From from) noexcept
{
	TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

	using UTo = std::make_unsigned_t<To>;

	const double result{static_cast<double>(from) * static_cast<double>(std::numeric_limits<UTo>::max())};
	if constexpr (sizeof(To) == 8) {
		// Safety checks for upper limit rounding is necessary for 64-bit.
		return norm_cast<To>(result == static_cast<double>(std::numeric_limits<UTo>::max()) ? std::numeric_limits<UTo>::max()
																							: static_cast<UTo>(result));
	}
	else {
		return norm_cast<To>(static_cast<UTo>(result));
	}
}

template <std::integral To, std::integral From> constexpr To tr::norm_cast(From from) noexcept
{
	using UFrom = std::make_unsigned_t<From>;
	using UTo = std::make_unsigned_t<To>;

	if constexpr (std::same_as<From, To>) {
		return from;
	}
	else if constexpr (sizeof(From) == sizeof(To)) {
		return static_cast<To>(static_cast<UFrom>(from) + 1 + std::numeric_limits<UFrom>::max() / 2);
	}
	else if constexpr (sizeof(From) > sizeof(To)) {
		return norm_cast<To>(static_cast<UTo>(norm_cast<UFrom>(from) >> ((sizeof(From) - sizeof(To)) * 8ULL)));
	}
	else {
		if constexpr (sizeof(To) != 8) {
			return norm_cast<To>(
				static_cast<UTo>(norm_cast<UFrom>(from) * std::numeric_limits<UTo>::max() / std::numeric_limits<UFrom>::max()));
		}
		else {
			return norm_cast<To>(norm_cast<double>(from));
		}
	}
}

/// @endcond
