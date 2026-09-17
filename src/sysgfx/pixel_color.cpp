/// @file
/// @brief Implements internal/pixel_color.hpp.

#include "internal/pixel_color.hpp"
#include <SDL3/SDL.h>

//

tr::rgba8 tr::internal::pixel_color(const std::byte* data, pixel_format format) noexcept
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
	SDL_GetRGBA(value, SDL_GetPixelFormatDetails(static_cast<SDL_PixelFormat>(format)), nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
}