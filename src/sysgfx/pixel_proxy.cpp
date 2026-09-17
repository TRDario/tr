/// @file
/// @brief Implements pixel_proxy.hpp.

#include "internal/pixel_color.hpp"
#include <SDL3/SDL.h>
#include <tr/sysgfx/pixel_proxy.hpp>
#include <tr/utility/color.hpp>

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
	return internal::pixel_color(m_data, m_format);
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
	return internal::pixel_color(m_data, m_format);
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
		internal::u24& arr{*reinterpret_cast<internal::u24*>(m_data)};
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