/// @file
/// @brief Provides functions for converting `tr::pixel_format` to OpenGL texture formats.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>

//

namespace tr::internal
{
	/// Converts a pixel format to an OpenGL texture format.
	/// @param format Pixel format.
	/// @return Equivalent OpenGL texture format.
	[[nodiscard]] unsigned int opengl_texture_format(pixel_format format) noexcept;

	/// Converts a pixel format to an OpenGL texture format layout.
	/// @param format Pixel format.
	/// @return Equivalent OpenGL texture format layout.
	[[nodiscard]] unsigned int opengl_texture_format_layout(pixel_format format) noexcept;

	/// Converts a pixel format to an OpenGL texture format type.
	/// @param format Pixel format.
	/// @return Equivalent OpenGL texture format type.
	[[nodiscard]] unsigned int opengl_texture_format_type(pixel_format format) noexcept;

	//

	/// Converts a pixel format to an OpenGL image format.
	/// @param format Pixel format.
	/// @return Equivalent OpenGL image format.
	[[nodiscard]] unsigned int opengl_image_format(pixel_format format) noexcept;
} // namespace tr::internal