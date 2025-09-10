#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/initialization.hpp"
#include <SDL3/SDL.h>

std::filesystem::path tr::sys::executable_dir()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get executable directory before initializing the application.");

	return SDL_GetBasePath();
}

std::filesystem::path tr::sys::user_dir()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get user directory before initializing the application.");

	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cpath{SDL_GetPrefPath(app_developer, app_name)};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath.get()};
	return userdir;
}
