/// @file
/// @brief Implements pixel_format.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/pixel_format.hpp>

//

int tr::pixel_bytes(pixel_format format) noexcept
{
	return SDL_BYTESPERPIXEL(std::to_underlying(format));
}