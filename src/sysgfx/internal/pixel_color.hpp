/// @file
/// @brief Provides `tr::internal::pixel_color()`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/color.hpp>

//

namespace tr::internal
{
	/// Extracts an RGBA8 color value from a pixel.
	/// @param data Pointer to the pixel data.
	/// @param format Format of the pixel.
	/// @return Color of the pixel.
	[[nodiscard]] rgba8 pixel_color(const std::byte* data, pixel_format format) noexcept;
} // namespace tr::internal