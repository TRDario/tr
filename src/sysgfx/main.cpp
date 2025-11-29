#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#if defined _MSC_VER and not defined TR_ENABLE_ASSERTS
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//

namespace tr::sys {
	// Timer used to emit ticks.
	std::optional<timer> g_tick_timer;
} // namespace tr::sys

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

void tr::sys::set_app_information(cstring_view developer, cstring_view name, cstring_view version)
{
	g_app_developer = developer;
	g_app_name = name;
	SDL_SetAppMetadata(name, version, nullptr);
}

void tr::sys::set_tick_frequency(float frequency)
{
	g_tick_timer.emplace(dsecs{1} / frequency, [] {
		SDL_Event event{};
		event.type = 0x8000;
		SDL_PushEvent(&event);
	});
}

void tr::sys::set_draw_frequency(float frequency)
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(frequency).c_str());
}

//

extern "C"
{
	SDL_AppResult SDL_AppInit(void**, int argc, char** argv)
	{
		try {
			tr::sys::signal parse_result{::parse_command_line({(tr::cstring_view*)argv, std::size_t(argc)})};
			if (parse_result != tr::sys::signal::CONTINUE) {
				return SDL_AppResult(parse_result);
			}
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}

		if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
			TR_LOG(tr::log, tr::severity::FATAL, "Failed to initialize SDL3.");
			TR_LOG_CONTINUE(tr::log, "{}", SDL_GetError());
			return SDL_APP_FAILURE;
		};
		TR_LOG(tr::log, tr::severity::INFO, "Initialized SDL3.");
		TR_LOG_CONTINUE(tr::log, "Platform: {}", SDL_GetPlatform());
		TR_LOG_CONTINUE(tr::log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
		TR_LOG_CONTINUE(tr::log, "RAM: {}mb", SDL_GetSystemRAM());
		tr::sys::set_draw_frequency(tr::sys::refresh_rate());

		try {
			return SDL_AppResult(::initialize());
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
	{
		try {
			if (event->type == 0x8000) {
				return SDL_AppResult(::tick());
			}
			else {
				return SDL_AppResult(::handle_event((tr::sys::event&)*event));
			}
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppIterate(void*)
	{
		try {
			return SDL_AppResult(::draw());
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	void SDL_AppQuit(void*, SDL_AppResult)
	{
		try {
			::shut_down();
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
		}
		TTF_Quit();
		SDL_Quit();
	}
}
