#define SDL_MAIN_USE_CALLBACKS 1
#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#undef main

#if defined _MSC_VER and not defined TR_ENABLE_ASSERTS
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#ifdef TR_HAS_AUDIO
#include "../../include/tr/audio/impl.hpp"
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
		using tr::sys::main::metadata;
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata.name);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata.version);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata.identifier);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata.developer);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata.copyright);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata.url);
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, metadata.type == tr::sys::app_type::game ? "game" : "application");
		if (!metadata.name.empty()) {
			if (!metadata.version.empty()) {
				TR_LOG(tr::log, tr::severity::info, "Launching {} {}.", metadata.name, metadata.version);
			}
			else {
				TR_LOG(tr::log, tr::severity::info, "Launching {}.", metadata.name);
			}
		}

		try {
			tr::sys::signal parse_result{tr::sys::main::parse_command_line({(tr::cstring_view*)argv, std::size_t(argc)})};
			if (parse_result != tr::sys::signal::proceed) {
				return SDL_AppResult(parse_result);
			}
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}

		if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
			TR_LOG(tr::log, tr::severity::fatal, "Failed to initialize SDL3.");
			TR_LOG_CONTINUE(tr::log, "{}", SDL_GetError());
			return SDL_APP_FAILURE;
		}
		else {
			TR_LOG(tr::log, tr::severity::info, "Initialized SDL3.");
			TR_LOG_CONTINUE(tr::log, "Platform: {}", SDL_GetPlatform());
			TR_LOG_CONTINUE(tr::log, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
			TR_LOG_CONTINUE(tr::log, "RAM: {}mb", SDL_GetSystemRAM());
		}
		tr::sys::set_draw_frequency(tr::sys::refresh_rate());

#ifdef TR_HAS_AUDIO
		if (!tr::audio::g_manager.initialize()) {
			TR_LOG(tr::log, tr::severity::fatal, "Failed to initialize audio system.");
			return SDL_APP_FAILURE;
		}
#endif

		try {
			return SDL_AppResult(tr::sys::main::initialize());
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
				return SDL_AppResult(tr::sys::main::tick());
			}
			else {
				return SDL_AppResult(tr::sys::main::handle_event((tr::sys::event&)*event));
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
			return SDL_AppResult(tr::sys::main::draw());
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	void SDL_AppQuit(void*, SDL_AppResult)
	{
		try {
			tr::sys::main::shut_down();
			TR_ASSERT(!tr::sys::g_window.is_open(), "Did not close window during main::shut_down.");
		}
		catch (std::exception& err) {
			tr::sys::show_fatal_error_message_box(err);
		}

#ifdef TR_HAS_AUDIO
		tr::audio::g_manager.shut_down();
#endif

		tr::sys::g_tick_timer.reset();
		TTF_Quit();
		SDL_Quit();
	}
}
