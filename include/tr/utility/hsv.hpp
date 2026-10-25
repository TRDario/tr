/// @file
/// @brief Provides `tr::hsv`.

#pragma once
#include <tr/utility/color_cast.hpp>
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/math.hpp>
#include <tr/utility/rgba.hpp>

//

namespace tr
{
	/// Color stored in the HSV color model.
	struct hsv
	{
		/// Hue channel.
		float h;

		/// Saturation channel.
		float s;

		/// Value channel.
		float v;

		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(hsv lhs, hsv rhs) noexcept = default;

		/// @}
	};
} // namespace tr

//

/// Enables default binary IO for `tr::hsv`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::hsv>{true};

//

/// Color caster from `tr::hsv` to `tr::rgba<double>`.
template <>
struct tr::color_caster<tr::hsv, tr::rgba<double>>
{
	/// Casts a color from `tr::hsv` to an equivalent value of type `tr::rgba<double>`.
	/// @param from Color to cast.
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

/// Color caster from `tr::rgba<double>` to `tr::hsv`.
template <>
struct tr::color_caster<tr::rgba<double>, tr::hsv>
{
	/// Casts a color from `tr::rgba<double>` to an equivalent value of type `tr::hsv`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::hsv`.
	[[nodiscard]] static constexpr hsv operator()(const rgba<double>& rgba) noexcept
	{
		const std::pair<double, double> minmax{std::minmax({rgba.r, rgba.g, rgba.b})};
		const double delta{minmax.second - minmax.first};

		hsv hsv;
		hsv.v = minmax.second;
		if (delta == 0) {
			hsv.h = 0;
		}
		else if (minmax.second == rgba.r) {
			hsv.h = 60.0 * eucmod((rgba.g - rgba.b) / delta, 6.0);
		}
		else if (minmax.second == rgba.g) {
			hsv.h = 60.0 * ((rgba.b - rgba.r) / delta + 2.0);
		}
		else {
			hsv.h = 60.0 * ((rgba.r - rgba.g) / delta + 4.0);
		}
		hsv.s = minmax.second != 0 ? delta / minmax.second : 0;
		return hsv;
	}
};