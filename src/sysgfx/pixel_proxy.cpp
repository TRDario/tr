/// @file
/// @brief Implements pixel_proxy.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/pixel_proxy.hpp>
#include <tr/utility/color.hpp>

//

namespace tr
{
	namespace
	{
		/// 24-bit integer.
		using u24 = tr::u8[3];

		//

		/// Extracts an RGBA8 color value from a pixel.
		/// @param data Pointer to the pixel data.
		/// @param format Format of the pixel.
		/// @return Color of the pixel.
		[[nodiscard]] rgba8 pixel_color(const std::byte* data, pixel_format format) noexcept
		{
			u32 value{};
			switch (pixel_bytes(format)) {
			case 1:
				value = *reinterpret_cast<const u8*>(data);
				break;
			case 2:
				value = *reinterpret_cast<const u16*>(data);
				break;
			case 3: {
				const u24& arr{*reinterpret_cast<const u24*>(data)};
				value = arr[0] << 16 | arr[1] << 8 | arr[2];
				break;
			}
			case 4:
				value = *reinterpret_cast<const u32*>(data);
				break;
			}

			rgba8 color;
			SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(format)), nullptr, &color.r, &color.g, &color.b,
						&color.a);
			return color;
		}
	} // namespace
} // namespace tr

//

tr::const_pixel_proxy::const_pixel_proxy(const std::byte* data, pixel_format format) noexcept
	: m_data{data}
	, m_format{format}
{
}

const std::byte* tr::const_pixel_proxy::data() const noexcept
{
	return m_data;
}

tr::pixel_format tr::const_pixel_proxy::format() const noexcept
{
	return m_format;
}

tr::const_pixel_proxy::operator tr::rgba8() const noexcept
{
	return pixel_color(m_data, m_format);
}

//

tr::pixel_proxy::pixel_proxy(std::byte* data, pixel_format format) noexcept
	: m_data{data}
	, m_format{format}
{
}

std::byte* tr::pixel_proxy::data() const noexcept
{
	return m_data;
}

tr::pixel_format tr::pixel_proxy::format() const noexcept
{
	return m_format;
}

tr::pixel_proxy::operator tr::rgba8() const noexcept
{
	return pixel_color(m_data, m_format);
}

tr::pixel_proxy& tr::pixel_proxy::operator=(rgba8 color) noexcept
{
	const SDL_PixelFormatDetails* format_details{SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(m_format))};
	const u32 formatted{SDL_MapRGBA(format_details, nullptr, color.r, color.g, color.b, color.a)};
	switch (pixel_bytes(m_format)) {
	case 1:
		*reinterpret_cast<u8*>(m_data) = formatted;
		break;
	case 2:
		*reinterpret_cast<u16*>(m_data) = formatted;
		break;
	case 3: {
		u24& arr{*reinterpret_cast<u24*>(m_data)};
		arr[0] = formatted >> 16;
		arr[1] = formatted >> 8;
		arr[2] = formatted;
		break;
	}
	case 4:
		*reinterpret_cast<u32*>(m_data) = formatted;
	}
	return *this;
}