/// @file
/// @brief Implements path.hpp.

#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include <SDL3/SDL.h>

//

std::filesystem::path tr::executable_directory()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::user_directory()
{
	char* const cpath{SDL_GetPrefPath(tr_app::metadata.developer.c_str(), tr_app::metadata.name.c_str())};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath};
	SDL_free(cpath);
	return userdir;
}
