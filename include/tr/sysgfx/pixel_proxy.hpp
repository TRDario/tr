/// @file
/// @brief Provides `tr::const_pixel_proxy` and `tr::pixel_proxy`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>

namespace tr
{
	struct rgba8;
}

//

namespace tr
{
	/// Read-only proxy to a bitmap pixel.
	class const_pixel_proxy
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors
		/// @{

		/// Undefined, required to default-construct iterators.
		[[nodiscard]] const_pixel_proxy() noexcept = default;

		/// Wraps a pointer to the pixel data.
		/// @param data Pointer to the pixel data.
		/// @param pixel_format Format of the pixel.
		[[nodiscard]] const_pixel_proxy(const std::byte* data, pixel_format format) noexcept;

		/// @}
		/// @endcond
		/// @name Access
		/// @{

		/// Gets a pointer to the data of the pixel.
		/// @return Pointer to the data of the pixel.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// Gets the format of the pixel.
		/// @return Format of the pixel.
		[[nodiscard]] pixel_format format() const noexcept;

		/// Gets the pixel's color value.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba8() const noexcept;

		/// @}

	  private:
		/// Pointer to the pixel data.
		const std::byte* m_data;

		/// Format of the pixel.
		pixel_format m_format;
	};

	/// Mutable proxy to a bitmap pixel.
	class pixel_proxy
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors
		/// @{

		/// Undefined, required to default-construct iterators.
		[[nodiscard]] pixel_proxy() noexcept = default;

		/// Wraps a pointer to the pixel data.
		/// @param data Pointer to the pixel data.
		/// @param format Format of the pixel.
		[[nodiscard]] pixel_proxy(std::byte* data, pixel_format format) noexcept;

		/// @}
		/// @endcond
		/// @name Access
		/// @{

		/// Gets a pointer to the data of the pixel.
		/// @return Pointer to the data of the pixel.
		[[nodiscard]] std::byte* data() const noexcept;

		/// Gets the format of the pixel.
		/// @return Format of the pixel.
		[[nodiscard]] pixel_format format() const noexcept;

		/// Gets the pixel's color value.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba8() const noexcept;

		/// Sets the pixel's color value.
		/// @param color Color to set.
		/// @return Reference to the pixel proxy.
		pixel_proxy& operator=(rgba8 color) noexcept;

		/// @}

	  private:
		/// Pointer to the pixel data.
		std::byte* m_data;

		/// Format of the pixel.
		pixel_format m_format;
	};
} // namespace tr