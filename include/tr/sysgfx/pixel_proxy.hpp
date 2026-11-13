/// @file
/// @brief Provides `tr::const_pixel_proxy` and `tr::pixel_proxy`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/integer.hpp>
#include <tr/utility/rgba.hpp>

//

namespace tr
{
	/// @brief Proxy object providing read-only access to a pixel of a bitmap.
	class const_pixel_proxy
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a pixel proxy.
		/// @param data Pointer to the data of the pixel.
		/// @param pixel_format Format of the pixel.
		[[nodiscard]] const_pixel_proxy(const std::byte* data, pixel_format format) noexcept;

		/// @}
		/// @endcond
		/// @name Element access
		/// @{

		/// @brief Returns the color of the pixel.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba<u8>() const noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns a pointer to the data of the pixel.
		/// @return Pointer to the data of the pixel.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// @brief Returns the format of the pixel.
		/// @return Format of the pixel.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @}

	  private:
		/// @brief Pointer to the data of the pixel.
		const std::byte* m_data;

		/// @brief Format of the pixel.
		pixel_format m_format;
	};

	/// @brief Proxy object providing read-write access to a pixel of a bitmap.
	class pixel_proxy
	{
	  public:
		/// @cond implementation_details
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a pixel proxy.
		/// @param data Pointer to the data of the pixel.
		/// @param format Format of the pixel.
		[[nodiscard]] pixel_proxy(std::byte* data, pixel_format format) noexcept;

		/// @}
		/// @endcond
		/// @name Element access
		/// @{

		/// @brief Returns the color of the pixel.
		/// @return Color of the pixel.
		[[nodiscard]] operator rgba<u8>() const noexcept;

		/// @brief Sets the color of the pixel.
		/// @param color Color to set.
		/// @return Reference to `*this`.
		pixel_proxy& operator=(rgba<u8> color) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns a pointer to the data of the pixel.
		/// @return Pointer to the data of the pixel.
		[[nodiscard]] std::byte* data() const noexcept;

		/// @brief Returns the format of the pixel.
		/// @return Format of the pixel.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @}

	  private:
		/// @brief Pointer to the data of the pixel.
		std::byte* m_data;

		/// @brief Format of the pixel.
		pixel_format m_format;
	};
} // namespace tr