#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

//

tr::sys::init_error::init_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::sys::init_error::name() const
{
	return "System initialization error";
}

std::string_view tr::sys::init_error::description() const
{
	return m_description;
}

std::string_view tr::sys::init_error::details() const
{
	return SDL_GetError();
}

//

void tr::sys::set_app_information(const char* developer, const char* name, const char* version)
{
	app_developer = developer;
	app_name = name;
	SDL_SetAppMetadata(name, version, nullptr);
}

void tr::sys::set_iteration_frequency(float frequency)
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(frequency).c_str());
}

//

extern "C"
{
	SDL_AppResult SDL_AppInit(void**, int argc, char** argv)
	{
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			TR_LOG(tr::log, tr::severity::FATAL, "Failed to initialize SDL3.");
			TR_LOG_CONTINUE(tr::log, "{}", SDL_GetError());
			return SDL_APP_FAILURE;
		};
		TR_LOG(tr::log, tr::severity::INFO, "Initialized SDL3.");
		TR_LOG_CONTINUE(tr::log, "Platform: {}", SDL_GetPlatform());
		TR_LOG_CONTINUE(tr::log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
		TR_LOG_CONTINUE(tr::log, "RAM: {}mb", SDL_GetSystemRAM());

		try {
			return SDL_AppResult(::initialize({(const char**)argv, std::size_t(argc)}));
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
	{
		try {
			return SDL_AppResult(::handle_event((tr::sys::event&)*event));
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppIterate(void*)
	{
		try {
			return SDL_AppResult(::iterate());
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	void SDL_AppQuit(void*, SDL_AppResult)
	{
		try {
			::quit();
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
		}
		SDL_Quit();
	}
}
