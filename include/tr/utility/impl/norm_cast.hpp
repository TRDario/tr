///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements norm_cast.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../norm_cast.hpp"

//////////////////////////////////////////////////////////////// NORM CAST ////////////////////////////////////////////////////////////////

template <std::floating_point To, std::floating_point From> constexpr To tr::norm_cast(From from)
{
	TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

	return static_cast<To>(from);
}

template <std::floating_point To, std::integral From> constexpr To tr::norm_cast(From from)
{
	using UFrom = std::make_unsigned_t<From>;

	return static_cast<To>(static_cast<double>(norm_cast<UFrom>(from)) / static_cast<double>(std::numeric_limits<UFrom>::max()));
}

template <std::integral To, std::floating_point From> constexpr To tr::norm_cast(From from)
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

template <std::integral To, std::integral From> constexpr To tr::norm_cast(From from)
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