///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements path.hpp.                                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////////////// STANDARD PATHS /////////////////////////////////////////////////////////////

std::filesystem::path tr::sys::executable_dir()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::sys::user_dir()
{
	std::unique_ptr<char[], decltype([](void* p) { SDL_free(p); })> cpath{SDL_GetPrefPath(main::metadata.developer, main::metadata.name)};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath.get()};
	return userdir;
}
