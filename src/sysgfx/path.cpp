///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements path.hpp.                                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////////////// STANDARD PATHS /////////////////////////////////////////////////////////////

std::filesystem::path tr::executable_directory()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::user_directory()
{
	char* cpath{SDL_GetPrefPath(app::metadata.developer.c_str(), app::metadata.name.c_str())};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath};
	SDL_free(cpath);
	return userdir;
}
