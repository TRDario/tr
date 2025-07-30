#pragma once
#include "color_cast.hpp"
#include "math.hpp"
#include "norm_cast.hpp"

namespace tr {
	consteval std::size_t umax(std::uint8_t bits);
}

consteval std::size_t tr::umax(std::uint8_t bits)
{
	return (std::size_t{1} << bits) - 1;
}

template <tr::built_in_color To, tr::specialization_of<tr::r> From> constexpr To tr::color_cast(const From& from)
{
	if constexpr (specialization_of<To, g> || specialization_of<To, b>) {
		return {0};
	}
	else {
		using T = decltype(To::r);
		constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

		if constexpr (specialization_of<To, r>) {
			return {norm_cast<T>(from.r)};
		}
		else if constexpr (specialization_of<To, rg>) {
			return {norm_cast<T>(from.r), 0};
		}
		else if constexpr (specialization_of<To, rgb>) {
			return {norm_cast<T>(from.r), 0, 0};
		}
		else if constexpr (specialization_of<To, bgr>) {
			return {0, 0, norm_cast<T>(from.r)};
		}
		else if constexpr (specialization_of<To, rgba>) {
			return {norm_cast<T>(from.r), 0, 0, norm_cast<T>(1.0)};
		}
		else if constexpr (specialization_of<To, bgra>) {
			return {0, 0, norm_cast<T>(from.r), norm_cast<T>(1.0)};
		}
		else if constexpr (std::same_as<To, rgb8_223>) {
			return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX), 0, 0};
		}
		else if constexpr (std::same_as<To, rgb16_565>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0};
		}
		else if constexpr (std::same_as<To, bgr8_332>) {
			return {0, 0, static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, bgr16_565>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, rgba16_4444>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), 0, 0,
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, rgba16_5551>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0,
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, rgba32_1010102>) {
			return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), 0, 0,
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, abgr16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), 0, 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, abgr16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), 0, 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, abgr32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), 0, 0,
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, bgra16_4444>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, bgra16_5551>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, bgra32_1010102>) {
			return {0, 0, static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, argb16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					0, 0};
		}
		else if constexpr (std::same_as<To, argb16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					0, 0};
		}
		else if constexpr (std::same_as<To, argb32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					0, 0};
		}
	}
}

template <tr::built_in_color To, tr::specialization_of<tr::g> From> constexpr To tr::color_cast(const From& from)
{
	if constexpr (specialization_of<To, r> || specialization_of<To, b>) {
		return {0};
	}
	else {
		using T = decltype(To::g);
		constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

		if constexpr (specialization_of<To, g>) {
			return {norm_cast<T>(from.g)};
		}
		else if constexpr (specialization_of<To, rg>) {
			return {0, norm_cast<T>(from.g)};
		}
		else if constexpr (specialization_of<To, rgb> || specialization_of<To, bgr>) {
			return {0, norm_cast<T>(from.g), 0};
		}
		else if constexpr (specialization_of<To, rgba>) {
			return {0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0)};
		}
		else if constexpr (specialization_of<To, bgra>) {
			return {0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0)};
		}
		else if constexpr (std::same_as<To, rgb8_223>) {
			return {0, static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, rgb16_565>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, bgr8_332>) {
			return {0, static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, bgr16_565>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, rgba16_4444>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, rgba16_5551>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, rgba32_1010102>) {
			return {0, static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0,
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, abgr16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, abgr16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, abgr32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), 0,
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, bgra16_4444>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, bgra16_5551>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, bgra32_1010102>) {
			return {0, static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0,
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, argb16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, argb16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, argb32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), 0,
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0};
		}
	}
}

template <tr::built_in_color To, tr::specialization_of<tr::b> From> constexpr To tr::color_cast(const From& from)
{
	if constexpr (specialization_of<To, r> || specialization_of<To, g> || specialization_of<To, rg>) {
		return {0};
	}
	else {
		using T = decltype(To::b);
		constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

		if constexpr (specialization_of<To, b>) {
			return {norm_cast<T>(from.b)};
		}
		else if constexpr (specialization_of<To, rgb>) {
			return {0, 0, norm_cast<T>(from.b)};
		}
		else if constexpr (specialization_of<To, bgr>) {
			return {norm_cast<T>(from.b), 0, 0};
		}
		else if constexpr (specialization_of<To, rgba>) {
			return {0, 0, norm_cast<T>(from.b), norm_cast<T>(1.0)};
		}
		else if constexpr (specialization_of<To, bgra>) {
			return {norm_cast<T>(from.b), 0, 0, norm_cast<T>(1.0)};
		}
		else if constexpr (std::same_as<To, rgb8_223>) {
			return {0, 0, static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, rgb16_565>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, bgr8_332>) {
			return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX), 0, 0};
		}
		else if constexpr (std::same_as<To, bgr16_565>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0};
		}
		else if constexpr (std::same_as<To, rgba16_4444>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, rgba16_5551>) {
			return {0, 0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, rgba32_1010102>) {
			return {0, 0, static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, abgr16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
					0, 0};
		}
		else if constexpr (std::same_as<To, abgr16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
					0, 0};
		}
		else if constexpr (std::same_as<To, abgr32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
					0, 0};
		}
		else if constexpr (std::same_as<To, bgra16_4444>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), 0, 0,
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, bgra16_5551>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0,
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, bgra32_1010102>) {
			return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), 0, 0,
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, argb16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), 0, 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, argb16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), 0, 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, argb32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), 0, 0,
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)};
		}
	}
}

template <tr::built_in_color To, tr::specialization_of<tr::rg> From> constexpr To tr::color_cast(const From& from)
{
	if constexpr (specialization_of<To, b>) {
		return {0};
	}
	else {
		using T = decltype(To::r);
		constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

		if constexpr (specialization_of<To, r>) {
			return {norm_cast<T>(from.r)};
		}
		else if constexpr (specialization_of<To, rg>) {
			return {norm_cast<T>(from.r), norm_cast<T>(from.g)};
		}
		else if constexpr (specialization_of<To, rgb>) {
			return {norm_cast<T>(from.r), norm_cast<T>(from.g), 0};
		}
		else if constexpr (specialization_of<To, bgr>) {
			return {0, norm_cast<T>(from.g), norm_cast<T>(from.r)};
		}
		else if constexpr (specialization_of<To, rgba>) {
			return {norm_cast<T>(from.r), norm_cast<T>(from.g), 0, norm_cast<T>(1.0)};
		}
		else if constexpr (specialization_of<To, bgra>) {
			return {0, norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0)};
		}
		else if constexpr (std::same_as<To, rgb8_223>) {
			return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
					static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, rgb16_565>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, bgr8_332>) {
			return {0, static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
					static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, bgr16_565>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, rgba16_4444>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, rgba16_5551>) {
			return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0,
					static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, rgba32_1010102>) {
			return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0,
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, abgr16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, abgr16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), 0,
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, abgr32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), 0,
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)};
		}
		else if constexpr (std::same_as<To, bgra16_4444>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), static_cast<std::uint16_t>(umax(4))};
		}
		else if constexpr (std::same_as<To, bgra16_5551>) {
			return {0, static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), static_cast<std::uint16_t>(umax(1))};
		}
		else if constexpr (std::same_as<To, bgra32_1010102>) {
			return {0, static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(umax(2))};
		}
		else if constexpr (std::same_as<To, argb16_4444>) {
			return {static_cast<std::uint16_t>(umax(4)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, argb16_1555>) {
			return {static_cast<std::uint16_t>(umax(1)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX), 0};
		}
		else if constexpr (std::same_as<To, argb32_2101010>) {
			return {static_cast<std::uint32_t>(umax(2)), static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX), 0};
		}
	}
}

template <tr::built_in_color To, class From>
	requires tr::specialization_of<From, tr::rgb> || tr::specialization_of<From, tr::bgr>
constexpr To tr::color_cast(const From& from)
{
	using T = decltype(To::r);
	constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

	if constexpr (specialization_of<To, r>) {
		return {norm_cast<T>(from.r)};
	}
	else if constexpr (specialization_of<To, rg>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g)};
	}
	else if constexpr (specialization_of<To, rgb>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b)};
	}
	else if constexpr (specialization_of<To, bgr>) {
		return {norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r)};
	}
	else if constexpr (specialization_of<To, rgba>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(1.0)};
	}
	else if constexpr (specialization_of<To, bgra>) {
		return {norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0)};
	}
	else if constexpr (std::same_as<To, rgb8_223>) {
		return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgb16_565>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgr8_332>) {
		return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgr16_565>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgba16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), static_cast<std::uint16_t>(umax(4))};
	}
	else if constexpr (std::same_as<To, rgba16_5551>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), static_cast<std::uint16_t>(umax(1))};
	}
	else if constexpr (std::same_as<To, rgba32_1010102>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), static_cast<std::uint32_t>(umax(2))};
	}
	else if constexpr (std::same_as<To, abgr16_4444>) {
		return {static_cast<std::uint16_t>(umax(4)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, abgr16_1555>) {
		return {static_cast<std::uint16_t>(umax(1)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, abgr32_2101010>) {
		return {static_cast<std::uint32_t>(umax(2)), static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgra16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), static_cast<std::uint16_t>(umax(4))};
	}
	else if constexpr (std::same_as<To, bgra16_5551>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), static_cast<std::uint16_t>(umax(1))};
	}
	else if constexpr (std::same_as<To, bgra32_1010102>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), static_cast<std::uint32_t>(umax(2))};
	}
	else if constexpr (std::same_as<To, argb16_4444>) {
		return {static_cast<std::uint16_t>(umax(4)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, argb16_1555>) {
		return {static_cast<std::uint16_t>(umax(1)), static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, argb32_2101010>) {
		return {static_cast<std::uint32_t>(umax(2)), static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)};
	}
}

template <tr::built_in_color To, class From>
	requires tr::specialization_of<From, tr::rgba> || tr::specialization_of<From, tr::bgra>
constexpr To tr::color_cast(const From& from)
{
	using T = decltype(To::r);
	constexpr std::uint32_t UI32_MAX{std::numeric_limits<std::uint32_t>::max()};

	if constexpr (specialization_of<To, r>) {
		return {norm_cast<T>(from.r)};
	}
	else if constexpr (specialization_of<To, rg>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g)};
	}
	else if constexpr (specialization_of<To, rgb>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b)};
	}
	else if constexpr (specialization_of<To, bgr>) {
		return {norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r)};
	}
	else if constexpr (specialization_of<To, rgba>) {
		return {norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(from.a)};
	}
	else if constexpr (specialization_of<To, bgra>) {
		return {norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(from.a)};
	}
	else if constexpr (std::same_as<To, rgb8_223>) {
		return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgb16_565>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgr8_332>) {
		return {static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				static_cast<std::uint8_t>(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgr16_565>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgba16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgba16_5551>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, rgba32_1010102>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, abgr16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, abgr16_1555>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, abgr32_2101010>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgra16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint16_t>(from.a) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgra16_5551>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, bgra32_1010102>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, argb16_4444>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, argb16_1555>) {
		return {static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				static_cast<std::uint16_t>(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)};
	}
	else if constexpr (std::same_as<To, argb32_2101010>) {
		return {static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				static_cast<std::uint32_t>(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)};
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const rgb8_223& from)
{
	if constexpr (std::same_as<To, rgb8>) {
		return {
			static_cast<std::uint8_t>(from.r * 255 / umax(2)),
			static_cast<std::uint8_t>(from.g * 255 / umax(3)),
			static_cast<std::uint8_t>(from.b * 255 / umax(3)),
		};
	}
	else {
		return color_cast<To>(color_cast<rgb8>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const rgb16_565& from)
{
	if constexpr (std::same_as<To, rgb8>) {
		return {
			static_cast<std::uint8_t>(from.r * 255 / umax(5)),
			static_cast<std::uint8_t>(from.g * 255 / umax(6)),
			static_cast<std::uint8_t>(from.b * 255 / umax(5)),
		};
	}
	else {
		return color_cast<To>(color_cast<rgb8>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const bgr8_332& from)
{
	if constexpr (std::same_as<To, rgb8>) {
		return {
			static_cast<std::uint8_t>(from.r * 255 / umax(2)),
			static_cast<std::uint8_t>(from.g * 255 / umax(3)),
			static_cast<std::uint8_t>(from.b * 255 / umax(3)),
		};
	}
	else {
		return color_cast<To>(color_cast<rgb8>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const bgr16_565& from)
{
	if constexpr (std::same_as<To, rgb8>) {
		return {
			static_cast<std::uint8_t>(from.r * 255 / umax(5)),
			static_cast<std::uint8_t>(from.g * 255 / umax(6)),
			static_cast<std::uint8_t>(from.b * 255 / umax(5)),
		};
	}
	else {
		return color_cast<To>(color_cast<rgb8>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const rgba16_4444& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(4)), static_cast<std::uint8_t>(from.g * 255 / umax(4)),
				static_cast<std::uint8_t>(from.b * 255 / umax(4)), static_cast<std::uint8_t>(from.a * 255 / umax(4))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const rgba16_5551& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(5)), static_cast<std::uint8_t>(from.g * 255 / umax(5)),
				static_cast<std::uint8_t>(from.b * 255 / umax(5)), static_cast<std::uint8_t>(from.a * 255 / umax(1))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const rgba32_1010102& from)
{
	if constexpr (std::same_as<To, rgba<std::uint16_t>>) {
		return {static_cast<std::uint16_t>(from.r * 65535 / umax(10)), static_cast<std::uint16_t>(from.g * 65535 / umax(10)),
				static_cast<std::uint16_t>(from.b * 65535 / umax(10)), static_cast<std::uint16_t>(from.a * 65535 / umax(2))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint16_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const abgr16_4444& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(4)), static_cast<std::uint8_t>(from.g * 255 / umax(4)),
				static_cast<std::uint8_t>(from.b * 255 / umax(4)), static_cast<std::uint8_t>(from.a * 255 / umax(4))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const abgr16_1555& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(5)), static_cast<std::uint8_t>(from.g * 255 / umax(5)),
				static_cast<std::uint8_t>(from.b * 255 / umax(5)), static_cast<std::uint8_t>(from.a * 255 / umax(1))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const abgr32_2101010& from)
{
	if constexpr (std::same_as<To, rgba<std::uint16_t>>) {
		return {static_cast<std::uint16_t>(from.r * 65535 / umax(10)), static_cast<std::uint16_t>(from.g * 65535 / umax(10)),
				static_cast<std::uint16_t>(from.b * 65535 / umax(10)), static_cast<std::uint16_t>(from.a * 65535 / umax(2))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint16_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const bgra16_4444& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(4)), static_cast<std::uint8_t>(from.g * 255 / umax(4)),
				static_cast<std::uint8_t>(from.b * 255 / umax(4)), static_cast<std::uint8_t>(from.a * 255 / umax(4))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const bgra16_5551& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(5)), static_cast<std::uint8_t>(from.g * 255 / umax(5)),
				static_cast<std::uint8_t>(from.b * 255 / umax(5)), static_cast<std::uint8_t>(from.a * 255 / umax(1))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const bgra32_1010102& from)
{
	if constexpr (std::same_as<To, rgba<std::uint16_t>>) {
		return {static_cast<std::uint16_t>(from.r * 65535 / umax(10)), static_cast<std::uint16_t>(from.g * 65535 / umax(10)),
				static_cast<std::uint16_t>(from.b * 65535 / umax(10)), static_cast<std::uint16_t>(from.a * 65535 / umax(2))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint16_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const argb16_4444& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(4)), static_cast<std::uint8_t>(from.g * 255 / umax(4)),
				static_cast<std::uint8_t>(from.b * 255 / umax(4)), static_cast<std::uint8_t>(from.a * 255 / umax(4))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const argb16_1555& from)
{
	if constexpr (std::same_as<To, rgba<std::uint8_t>>) {
		return {static_cast<std::uint8_t>(from.r * 255 / umax(5)), static_cast<std::uint8_t>(from.g * 255 / umax(5)),
				static_cast<std::uint8_t>(from.b * 255 / umax(5)), static_cast<std::uint8_t>(from.a * 255 / umax(1))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint8_t>>(from));
	}
}

template <tr::built_in_color To> constexpr To tr::color_cast(const argb32_2101010& from)
{
	if constexpr (std::same_as<To, rgba<std::uint16_t>>) {
		return {static_cast<std::uint16_t>(from.r * 65535 / umax(10)), static_cast<std::uint16_t>(from.g * 65535 / umax(10)),
				static_cast<std::uint16_t>(from.b * 65535 / umax(10)), static_cast<std::uint16_t>(from.a * 65535 / umax(2))};
	}
	else {
		return color_cast<To>(color_cast<rgba<std::uint16_t>>(from));
	}
}

template <tr::built_in_color To, tr::color_castable_from From> constexpr To tr::color_cast(const From& from)
{
	if constexpr (std::same_as<std::remove_cvref_t<return_type_t<decltype(color_caster<From>::to_built_in)>>, To>) {
		return color_caster<From>::to_built_in(from);
	}
	else {
		return color_cast<To>(color_caster<From>::to_built_in(from));
	}
}

template <tr::color_castable_to To, tr::built_in_color From> constexpr To tr::color_cast(const From& from)
{
	using NaturalBuiltin = std::remove_cvref_t<arg_type_t<decltype(color_caster<To>::from_built_in)>>;

	if constexpr (std::same_as<NaturalBuiltin, From>) {
		return color_caster<To>::from_built_in(from);
	}
	else {
		return color_caster<To>::from_built_in(color_cast<NaturalBuiltin>(from));
	}
}

template <tr::color_castable_to To, tr::color_castable_from From> constexpr To tr::color_cast(const From& from)
{
	return color_cast<To>(color_cast<std::remove_cvref_t<arg_type_t<decltype(color_caster<To>::from_built_in)>>>(from));
}

constexpr tr::rgbaf tr::color_caster<tr::hsv>::to_built_in(const hsv& from)
{
	constexpr auto constexpr_abs{[](auto v) { return v > 0 ? v : -v; }};

	const float h{eucmod(from.h, 360.0f)};
	const float c{from.v * from.s};
	const float m{from.v - c};
	const float x{c * (1 - constexpr_abs(eucmod((h / 60.0f), 2.0f) - 1)) + m};

	if (h < 60) {
		return {from.v, x, m, 1.0f};
	}
	else if (h < 120) {
		return {x, from.v, m, 1.0f};
	}
	else if (h < 180) {
		return {m, from.v, x, 1.0f};
	}
	else if (h < 240) {
		return {m, x, from.v, 1.0f};
	}
	else if (h < 300) {
		return {x, m, from.v, 1.0f};
	}
	else {
		return {from.v, m, x, 1.0f};
	}
}

constexpr tr::hsv tr::color_caster<tr::hsv>::from_built_in(const rgbaf& from)
{
	hsv hsv{};
	hsv.v = std::max({from.r, from.g, from.b});
	const float delta{hsv.v - std::min({from.r, from.g, from.b})};

	if (delta == 0) {
		hsv.h = 0;
	}
	else if (hsv.v == from.r) {
		hsv.h = 60.0f * eucmod((from.g - from.b) / delta, 6.0f);
	}
	else if (hsv.v == from.g) {
		hsv.h = 60.0f * ((from.b - from.r) / delta + 2.0f);
	}
	else {
		hsv.h = 60.0f * ((from.r - from.g) / delta + 4.0f);
	}

	hsv.s = hsv.v != 0 ? delta / hsv.v : 0;

	return hsv;
}