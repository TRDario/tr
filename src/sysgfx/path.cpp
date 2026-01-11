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
	const char* const developer{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING)};
	const char* const app{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cpath{SDL_GetPrefPath(developer, app)};
	if (cpath == nullptr) {
		throw init_error{"Failed to get user directory path."};
	}
	std::filesystem::path userdir{cpath.get()};
	return userdir;
}
