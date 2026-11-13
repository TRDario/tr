/// @file
/// @brief Provides `tr::pixel_format`.

#pragma once

//

namespace tr
{
	/// @brief Bitmap/texture pixel format.
	enum class pixel_format
	{
		/// @brief Single-channel color stored as an 8-bit integer.
		r8 = 318769153,

		/// @brief Single-channel color stored as an 8-bit integer.
		index8 = r8,

		/// @brief RGB color stored as an 8-bit integer.
		rgb_p332 = 336660481,

		/// @brief XRGB color stored as a 16-bit integer (4-bit channels).
		xrgb_p4444 = 353504258,

		/// @brief XBGR color stored as a 16-bit integer (4-bit channels).
		xbgr_p4444 = 357698562,

		/// @brief XRGB color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		xrgb_p1555 = 353570562,

		/// @brief XBGR color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		xbgr_p1555 = 357764866,

		/// @brief ARGB color stored as a 16-bit integer (4-bit channels).
		argb_p4444 = 355602434,

		/// @brief RGBA color stored as a 16-bit integer (4-bit channels).
		rgba_p4444 = 356651010,

		/// @brief ABGR color stored as a 16-bit integer (4-bit channels).
		abgr_p4444 = 359796738,

		/// @brief BGRA color stored as a 16-bit integer (4-bit channels).
		bgra_p4444 = 360845314,

		/// @brief ARGB color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		argb_p1555 = 355667970,

		/// @brief RGBA color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		rgba_p5551 = 356782082,

		/// @brief ABGR color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		abgr_p1555 = 359862274,

		/// @brief BGRA color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		bgra_p5551 = 360976386,

		/// @brief RGB color stored as a 16-bit integer.
		rgb_p565 = 353701890,

		/// @brief BGR color stored as a 16-bit integer.
		bgr_p565 = 357896194,

		/// @brief RGB color stored as an array.
		rgb24 = 386930691,

		/// @brief BGR color stored as an array.
		bgr24 = 390076419,

		/// @brief BGRX color stored as a 32-bit integer.
		bgrx32 = 370546692,

		/// @brief XBGR color stored as a 32-bit integer.
		xbgr32 = 371595268,

		/// @brief RGBX color stored as a 32-bit integer.
		rgbx32 = 374740996,

		/// @brief XRGB color stored as a 32-bit integer.
		xrgb32 = 375789572,

		/// @brief BGRA color stored as a 32-bit integer.
		bgra32 = 372645892,

		/// @brief ABGR color stored as a 32-bit integer.
		abgr32 = 373694468,

		/// @brief RGBA color stored as a 32-bit integer.
		rgba32 = 376840196,

		/// @brief ARGB color stored as a 32-bit integer.
		argb32 = 377888772,
	};

	//

	/// @brief Returns the number of bytes per pixel for a given format.
	/// @param format Pixel format type.
	/// @return Number of bytes per pixel for `format`.
	[[nodiscard]] int pixel_bytes(pixel_format format) noexcept;
} // namespace tr