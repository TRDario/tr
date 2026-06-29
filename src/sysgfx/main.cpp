///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements sysgfx/main.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SDL_MAIN_USE_CALLBACKS 1
#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/utility/logger.hpp"
#include "../../include/tr/utility/reference.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#undef main

#if defined _MSC_VER and not defined TR_ENABLE_ASSERTS
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

///////////////////////////////////////////////////////////// INTERNAL HELPERS ////////////////////////////////////////////////////////////

// Buffer allocated to be freed in case of an out-of-memory error.
static std::unique_ptr<char[]> g_emergency_buffer{new char[16384]};

// Shows an "Fatal exception" message box.
// In case of an out-of-memory error, it frees an emergency buffer to allow for clean-up and logging.
static void show_fatal_error_message_box(const std::exception& exception)
{
	if (tr::dynamic_ref_cast<const std::bad_alloc>(exception).has_ref() ||
		tr::dynamic_ref_cast<const tr::out_of_memory>(exception).has_ref()) {
		g_emergency_buffer.reset();
	}

	const std::string title{TR_FMT::format("{} - Fatal Error", app::metadata.name)};

	tr::opt_ref<const tr::exception> tr_exception{tr::dynamic_ref_cast<const tr::exception>(exception)};
	std::string message;
	if (tr_exception.has_ref()) {
		message = TR_FMT::format("A fatal error has occurred ({}).", tr_exception->name());
		const std::string_view description{tr_exception->description()};
		if (!description.empty()) {
			message.push_back('\n');
			message.append(description);
		}
		const std::string_view details{tr_exception->details()};
		if (!details.empty()) {
			message.push_back('\n');
			message.append(details);
		}
		TR_LOG(tr::error_logger, tr::severity::fatal, *tr_exception);
	}
	else {
		message = TR_FMT::format("A fatal error has occurred ({}).", exception.what());
		TR_LOG(tr::error_logger, tr::severity::fatal, exception);
	}
	message.append("\nPress OK to exit the application.");

	tr::show_message_box(tr::message_box_type::error, tr::message_box_layout::ok, title, message);
}

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
				TR_LOG(tr::error_logger, tr::severity::info, "Launching {} {}.", app::metadata.name, app::metadata.version);
			}
			else {
				TR_LOG(tr::error_logger, tr::severity::info, "Launching {}.", app::metadata.name);
			}
		}

		try {
			tr::signal parse_result{app::parse_command_line({reinterpret_cast<tr::zstring_view*>(argv), static_cast<std::size_t>(argc)})};
			if (parse_result != tr::signal::proceed) {
				return static_cast<SDL_AppResult>(parse_result);
			}
		}
		catch (std::exception& err) {
			show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}

		if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
			TR_LOG(tr::error_logger, tr::severity::fatal, "Failed to initialize SDL3.");
			TR_LOG_CONTINUE(tr::error_logger, "{}", SDL_GetError());
			return SDL_APP_FAILURE;
		}
		else {
			TR_LOG(tr::error_logger, tr::severity::info, "Initialized SDL3.");
			TR_LOG_CONTINUE(tr::error_logger, "Platform: {}", SDL_GetPlatform());
			TR_LOG_CONTINUE(tr::error_logger, "CPU cores: {}", SDL_GetNumLogicalCPUCores());
			TR_LOG_CONTINUE(tr::error_logger, "RAM: {}mb", SDL_GetSystemRAM());
		}

		try {
			return static_cast<SDL_AppResult>(app::initialize());
		}
		catch (std::exception& err) {
			show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
	{
		try {
			return static_cast<SDL_AppResult>(app::handle_event(reinterpret_cast<tr::event&>(*event)));
		}
		catch (std::exception& err) {
			show_fatal_error_message_box(err);
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
			return static_cast<SDL_AppResult>(app::update(delta));
		}
		catch (std::exception& err) {
			show_fatal_error_message_box(err);
			return SDL_APP_FAILURE;
		}
	}

	void SDL_AppQuit(void*, SDL_AppResult)
	{
		try {
			app::shut_down();
		}
		catch (std::exception& err) {
			show_fatal_error_message_box(err);
		}

		TTF_Quit();
	}
}
