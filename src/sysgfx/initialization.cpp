#include "../../include/tr/sysgfx/initialization.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

tr::system_initialization_error::system_initialization_error(std::string_view description)
	: description_str{description}
{
}

std::string_view tr::system_initialization_error::name() const
{
	return "System initialization error";
}

std::string_view tr::system_initialization_error::description() const
{
	return description_str;
}

std::string_view tr::system_initialization_error::details() const
{
	return SDL_GetError();
}

void tr::initialize_application(const char* developer, const char* name)
{
	app_developer = developer;
	app_name = name;
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			throw system_initialization_error{"Failed to initialize SDL3."};
		};
		TR_LOG(log, severity::INFO, "Initialized SDL3.");
		TR_LOG_CONTINUE(log, "Platform: {}", SDL_GetPlatform());
		TR_LOG_CONTINUE(log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
		TR_LOG_CONTINUE(log, "RAM: {}mb", SDL_GetSystemRAM());
		atexit(SDL_Quit);
	}
}