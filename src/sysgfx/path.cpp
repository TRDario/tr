/// @file
/// @brief Implements path.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/path.hpp>

//

tr::path_error::path_error(std::string_view description) noexcept
	: m_description{description}
{
}

std::string_view tr::path_error::name() const noexcept
{
	return "Path error";
}

std::string_view tr::path_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::path_error::details() const noexcept
{
	return {};
}

//

std::filesystem::path tr::executable_directory()
{
	return SDL_GetBasePath();
}

std::filesystem::path tr::user_directory()
{
	const char* const developer{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING)};
	const char* const name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
	char* const cpath{SDL_GetPrefPath(developer, name)};
	if (cpath == nullptr) {
		throw path_error{"Failed to get user directory path."};
	}
	std::filesystem::path user_directory_path{cpath};
	SDL_free(cpath);
	return user_directory_path;
}
