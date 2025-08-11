#include "../../include/tr/sysgfx/initialization.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

tr::system::init_error::init_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::system::init_error::name() const
{
	return "System initialization error";
}

std::string_view tr::system::init_error::description() const
{
	return m_description;
}

std::string_view tr::system::init_error::details() const
{
	return SDL_GetError();
}

void tr::system::initialize(const char* developer, const char* name)
{
	TR_ASSERT(!SDL_WasInit(SDL_INIT_VIDEO), "Tried to initialize the problem more than once.");

	app_developer = developer;
	app_name = name;
	SDL_SetHint(SDL_HINT_APP_NAME, name);
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw init_error{"Failed to initialize SDL3."};
	};
	TR_LOG(log, severity::INFO, "Initialized SDL3.");
	TR_LOG_CONTINUE(log, "Platform: {}", SDL_GetPlatform());
	TR_LOG_CONTINUE(log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
	TR_LOG_CONTINUE(log, "RAM: {}mb", SDL_GetSystemRAM());
	atexit(SDL_Quit);
}