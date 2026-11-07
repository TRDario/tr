/// @file
/// @brief Implements path.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/exception.hpp>
#include <tr/sysgfx/path.hpp>

//

std::filesystem::path tr::executable_directory()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::user_directory()
{
	const char* const developer{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING)};
	const char* const name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
	char* const user_directory_cpath{SDL_GetPrefPath(developer, name)};
	if (!user_directory_cpath) {
		throw path_error{"Failed to get user directory path."};
	}
	std::filesystem::path user_directory_path{user_directory_cpath};
	SDL_free(user_directory_cpath);
	return user_directory_path;
}
