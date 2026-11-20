/// @file
/// @brief Provides `tr::hsv`.

/// @ingroup color
/// @defgroup hsv HSV
/// @brief Contains `tr::hsv` and related functionality.

#pragma once
#include <tr/utility/color_cast.hpp>
#include <tr/utility/math.hpp>
#include <tr/utility/rgba.hpp>

//

namespace tr
{
	/// @ingroup hsv
	/// @brief Color stored in the HSV color model.
	struct hsv
	{
		/// @brief Hue channel.
		float h;

		/// @brief Saturation channel.
		float s;

		/// @brief Value channel.
		float v;

		/// @name Comparison
		/// @{

		/// @brief Checks two colors for memberwise equality.
		/// @param lhs, rhs Colors to compare memberwise.
		/// @return `true` if the colors are equal memberwise, `false` otherwise.
		[[nodiscard]] friend constexpr bool operator==(hsv lhs, hsv rhs) noexcept = default;

		/// @}
	};
} // namespace tr

//

/// @ingroup hsv
/// @brief Specialization of `std::tuple_size` for `tr::hsv`.
template <>
struct std::tuple_size<tr::hsv> : std::integral_constant<std::size_t, 3>
{
};

/// @ingroup hsv
/// @brief Specialization of `std::tuple_element` for `tr::hsv`.
/// @tparam Index Index of the element.
template <std::size_t Index>
struct std::tuple_element<Index, tr::hsv> : std::tuple_element<Index, std::tuple<float, float, float>>
{
};

namespace tr
{
	/// @ingroup hsv
	/// @brief Returns an element from an HSV color.
	/// @tparam Index Index of the element to return.
	/// @tparam Hsv Potentially cvref-qualified instance of `tr::hsv`.
	/// @param hsv Color to return an element from.
	/// @return Element from `hsv`.
	template <usize Index, typename Hsv>
		requires std::same_as<std::remove_cvref_t<Hsv>, hsv>
	[[nodiscard]] decltype(auto) get(Hsv&& hsv) noexcept
	{
		if constexpr (Index == 0) {
			return std::forward_like<Hsv>(hsv.h);
		}
		else if constexpr (Index == 1) {
			return std::forward_like<Hsv>(hsv.s);
		}
		else if constexpr (Index == 2) {
			return std::forward_like<Hsv>(hsv.v);
		}
		else {
			TR_UNREACHABLE;
		}
	}
} // namespace tr

//

/// @ingroup hsv
/// @brief Color caster from `tr::hsv` to `tr::rgba<double>`.
template <>
struct tr::color_caster<tr::hsv, tr::rgba<double>>
{
	/// @brief Casts a color from `tr::hsv` to an equivalent value of type `tr::rgba<double>`.
	/// @param hsv Color to cast.
	/// @return Equivalent color value of type `tr::rgba<double>`.
	[[nodiscard]] static constexpr rgba<double> operator()(const hsv& hsv) noexcept
	{
		rgba<double> rgba;
		const double h{eucmod(hsv.h, 360.0)};
		const double c{static_cast<double>(hsv.v) * hsv.s};
		const double m{hsv.v - c};
		const double x{c * (1 - std::abs(mod(h / 60, 2) - 1)) + m};

		if (h < 60) {
			rgba.r = hsv.v;
			rgba.g = x;
			rgba.b = m;
		}
		else if (h < 120) {
			rgba.r = x;
			rgba.g = hsv.v;
			rgba.b = m;
		}
		else if (h < 180) {
			rgba.r = m;
			rgba.g = hsv.v;
			rgba.b = x;
		}
		else if (h < 240) {
			rgba.r = m;
			rgba.g = x;
			rgba.b = hsv.v;
		}
		else if (h < 300) {
			rgba.r = x;
			rgba.g = m;
			rgba.b = hsv.v;
		}
		else {
			rgba.r = hsv.v;
			rgba.g = m;
			rgba.b = x;
		}
		return rgba;
	}
};

/// @ingroup hsv
/// @brief Color caster from `tr::rgba<double>` to `tr::hsv`.
template <>
struct tr::color_caster<tr::rgba<double>, tr::hsv>
{
	/// @brief Casts a color from `tr::rgba<double>` to an equivalent value of type `tr::hsv`.
	/// @param rgba Color to cast.
	/// @return Equivalent color value of type `tr::hsv`.
	[[nodiscard]] static constexpr hsv operator()(const rgba<double>& rgba) noexcept
	{
		const std::pair<double, double> minmax{std::minmax({rgba.r, rgba.g, rgba.b})};
		const double delta{minmax.second - minmax.first};

		hsv hsv;
		hsv.v = static_cast<float>(minmax.second);
		if (delta == 0) {
			hsv.h = 0;
		}
		else if (minmax.second == rgba.r) {
			hsv.h = static_cast<float>(60.0 * eucmod((rgba.g - rgba.b) / delta, 6.0));
		}
		else if (minmax.second == rgba.g) {
			hsv.h = static_cast<float>(60.0 * ((rgba.b - rgba.r) / delta + 2.0));
		}
		else {
			hsv.h = static_cast<float>(60.0 * ((rgba.r - rgba.g) / delta + 4.0));
		}
		hsv.s = minmax.second != 0 ? static_cast<float>(delta / minmax.second) : 0;
		return hsv;
	}
};