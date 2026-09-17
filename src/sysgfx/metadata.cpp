/// @file
/// @brief Implements metadata.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/metadata.hpp>

//

void tr::set_metadata(const metadata& metadata)
{
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata.name.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata.version.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata.identifier.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata.developer.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata.copyright.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata.url.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,
							   metadata.type == metadata::application_type::game ? "game" : "application");
	if (!metadata.name.empty()) {
		if (!metadata.version.empty()) {
			std::println("Launching {} {}.", metadata.name, metadata.version);
		}
		else {
			std::println("Launching {}.", metadata.name);
		}
	}
}