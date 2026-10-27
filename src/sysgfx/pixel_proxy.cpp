/// @file
/// @brief Implements pixel_proxy.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/pixel_proxy.hpp>

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

tr::const_pixel_proxy::operator tr::rgba<u8>() const noexcept
{
	u32 value{0};
	std::memcpy(&value, m_data, pixel_bytes(m_format));

	rgba<u8> color;
	SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(m_format)), nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
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

tr::pixel_proxy::operator tr::rgba<tr::u8>() const noexcept
{
	u32 value{0};
	std::memcpy(&value, m_data, pixel_bytes(m_format));

	rgba<u8> color;
	SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(m_format)), nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
}

tr::pixel_proxy& tr::pixel_proxy::operator=(rgba<u8> color) noexcept
{
	const SDL_PixelFormatDetails* format_details{SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(m_format))};
	const u32 formatted{SDL_MapRGBA(format_details, nullptr, color.r, color.g, color.b, color.a)};
	std::memcpy(m_data, &formatted, pixel_bytes(m_format));
	return *this;
}