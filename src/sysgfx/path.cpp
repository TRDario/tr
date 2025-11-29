#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include <SDL3/SDL.h>

std::filesystem::path tr::sys::executable_dir()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::sys::user_dir()
{
	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cpath{SDL_GetPrefPath(g_app_developer, g_app_name)};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath.get()};
	return userdir;
}
