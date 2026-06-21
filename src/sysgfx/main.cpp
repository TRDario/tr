///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements sysgfx/main.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SDL_MAIN_USE_CALLBACKS 1
#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#undef main

#if defined _MSC_VER and not defined TR_ENABLE_ASSERTS
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//////////////////////////////////////////////////////////////// INIT ERROR ///////////////////////////////////////////////////////////////

tr::init_error::init_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::init_error::name() const
{
	return "System initialization error";
}

std::string_view tr::init_error::description() const
{
	return m_description;
}

std::string_view tr::init_error::details() const
{
	return SDL_GetError();
}

///////////////////////////////////////////////////////////// FREQUENCY SETTER ////////////////////////////////////////////////////////////

void tr::set_update_frequency(float frequency)
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, frequency == uncapped_update_frequency ? "0" : std::to_string(frequency).c_str());
}

////////////////////////////////////////////////////////////// MAIN CALLBACKS /////////////////////////////////////////////////////////////

extern "C"
{
	SDL_AppResult SDL_AppInit(void**, int argc, char** argv)
	{
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, app::metadata.name.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, app::metadata.version.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, app::metadata.identifier.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, app::metadata.developer.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, app::metadata.copyright.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, app::metadata.url.c_str());
		SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, app::metadata.type == tr::app_type::game ? "game" : "application");
		if (!app::metadata.name.empty()) {
			if (!app::metadata.version.empty()) {
				TR_LOG(tr::log, tr::severity::info, "Launching {} {}.", app::metadata.name, app::metadata.version);
			}
			else {
				TR_LOG(tr::log, tr::severity::info, "Launching {}.", app::metadata.name);
			}
		}

		try {
			tr::signal parse_result{app::parse_command_line({(tr::zstring_view*)argv, std::size_t(argc)})};
			if (parse_result != tr::signal::proceed) {
				return SDL_AppResult(parse_result);
			}
		}
		catch (std::exception& err) {
			tr::show_fatal_error_message_box(err);
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

		try {
			return SDL_AppResult(app::initialize());
		}
		catch (std::exception& err) {
			tr::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
	{
		try {
			return SDL_AppResult(app::handle_event((tr::event&)*event));
		}
		catch (std::exception& err) {
			tr::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppIterate(void*)
	{
		try {
			static std::chrono::steady_clock::time_point prev{std::chrono::steady_clock::now()};
			const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
			const tr::duration delta{now - prev};
			prev = now;
			return SDL_AppResult(app::update(delta));
		}
		catch (std::exception& err) {
			tr::show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	void SDL_AppQuit(void*, SDL_AppResult)
	{
		try {
			app::shut_down();
		}
		catch (std::exception& err) {
			tr::show_fatal_error_message_box(err);
		}

		TTF_Quit();
		SDL_Quit();
	}
}
