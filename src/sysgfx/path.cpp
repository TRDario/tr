#include "../../include/tr/sysgfx/path.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

void tr::initialize_application(const char* developer, const char* name) noexcept
{
	_app_developer = developer;
	_app_name = name;
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			terminate("SDL3 initialization failure", SDL_GetError());
		};
		TR_LOG(log, severity::INFO, "Initialized SDL3.");
		TR_LOG_CONTINUE(log, "Platform: {}", SDL_GetPlatform());
		TR_LOG_CONTINUE(log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
		TR_LOG_CONTINUE(log, "RAM: {}mb", SDL_GetSystemRAM());
		atexit(SDL_Quit);
	}
}

std::filesystem::path tr::executable_dir() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get executable directory before initializing the application.");

	try {
		return SDL_GetBasePath();
	}
	catch (std::bad_alloc&) {
		terminate("Executable directory fetching failure", "Out of memory.");
	}
}

std::filesystem::path tr::user_dir() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get user directory before initializing the application.");

	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cpath{SDL_GetPrefPath(_app_developer, _app_name)};
	if (cpath == nullptr) {
		terminate("User directory fetching failure", SDL_GetError());
	}
	try {
		std::filesystem::path userdir{cpath.get()};
		return userdir;
	}
	catch (std::bad_alloc&) {
		terminate("User directory fetching failure", "Out of memory.");
	}
}
