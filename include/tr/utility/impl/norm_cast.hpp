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

	return To(from);
}

template <std::floating_point To, std::integral From> constexpr To tr::norm_cast(From from)
{
	using UFrom = std::make_unsigned_t<From>;

	return To(double(norm_cast<UFrom>(from)) / double(std::numeric_limits<UFrom>::max()));
}

template <std::integral To, std::floating_point From> constexpr To tr::norm_cast(From from)
{
	TR_ASSERT(from >= 0 && from <= 1, "Cannot perform a normalizing cast on value {} outside the range [0, 1].", from);

	using UTo = std::make_unsigned_t<To>;

	const double result{double(from) * double(std::numeric_limits<UTo>::max())};
	if constexpr (sizeof(To) == 8) {
		// Safety checks for upper limit rounding is necessary for 64-bit.
		return norm_cast<To>(result == double(std::numeric_limits<UTo>::max()) ? std::numeric_limits<UTo>::max() : UTo(result));
	}
	else {
		return norm_cast<To>(UTo(result));
	}
}

template <std::integral To, std::integral From> constexpr To tr::norm_cast(From from)
{
	using UFrom = std::make_unsigned_t<From>;
	using UTo = std::make_unsigned_t<To>;

	if constexpr (std::same_as<From, To>) {
		return from;
	}
	else if constexpr (sizeof(From) == sizeof(To)) {
		return To(UFrom(from) + 1 + std::numeric_limits<UFrom>::max() / 2);
	}
	else if constexpr (sizeof(From) > sizeof(To)) {
		return norm_cast<To>(UTo(norm_cast<UFrom>(from) >> ((sizeof(From) - sizeof(To)) * 8ULL)));
	}
	else {
		if constexpr (sizeof(To) != 8) {
			return norm_cast<To>(UTo(norm_cast<UFrom>(from) * std::numeric_limits<UTo>::max() / std::numeric_limits<UFrom>::max()));
		}
		else {
			return norm_cast<To>(norm_cast<double>(from));
		}
	}
}