#include <SDL3/SDL.h>
#include <tr/sysgfx/internal/sdl_surface_deleter.hpp>

//

void tr::internal::sdl_surface_deleter::operator()(SDL_Surface* ptr) noexcept
{
	SDL_DestroySurface(ptr);
}