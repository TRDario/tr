/// @file
/// @brief Implements application.hpp.

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tr/sysgfx/application.hpp>
#include <tr/sysgfx/dialog.hpp>
#include <tr/sysgfx/logger.hpp>
#include <tr/utility/dynamic_ref_cast.hpp>
#include <tr/utility/opt_ref.hpp>

//

namespace tr
{
	namespace
	{
		/// Optional reference to the application currently invoked in the main loop.
		opt_ref<application> current_application{};

		/// Buffer allocated to be freed in case of an out-of-memory error.
		std::unique_ptr<char[]> emergency_buffer{new char[16384]};

		//

		/// Shows an "Fatal exception" message box.
		/// @details In case of an out-of-memory error, it frees an emergency buffer to allow for clean-up and logging.
		/// @param error Error to display.
		void show_fatal_error_message_box(const std::exception& error)
		{
			if (dynamic_ref_cast<const std::bad_alloc>(error).has_ref() || dynamic_ref_cast<const out_of_memory>(error).has_ref()) {
				emergency_buffer.reset();
			}

			const char* const application_name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
			const std::string title{std::format("{} - Fatal Error", application_name)};

			opt_ref<const exception> tr_exception{dynamic_ref_cast<const exception>(error)};
			std::string message;
			if (tr_exception.has_ref()) {
				message = std::format("A fatal error has occurred ({}).", tr_exception->name());
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
			}
			else {
				message = std::format("A fatal error has occurred ({}).", error.what());
			}
			message.append("\nPress OK to exit the application.");

			TR_LOG_FATAL("tr", "{}", error.what());
			show_message_box(message_box_type::error, message_box_layout::ok, title, message);
		}

		//

		/// Shim around `current_application->initialize()` to make it compatible with SDL.
		/// @return SDL app result.
		[[nodiscard]] SDL_AppResult sdl_initialize(void**, int, char**)
		{
			if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
				std::println(stderr, "Failed to initialize SDL3: {}", SDL_GetError());

				const char* const application_name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
				const std::string title{std::format("{} - Fatal Error", application_name)};
				const std::string message{
					std::format("A fatal error has occured (Failed to initialize SDL3).\n{}\nPress OK to exit the application.",
								SDL_GetError()),
				};
				show_message_box(message_box_type::error, tr::message_box_layout::ok, title, message);
				return SDL_APP_FAILURE;
			}

			try {
				return static_cast<SDL_AppResult>(current_application->initialize());
			}
			catch (std::exception& err) {
				show_fatal_error_message_box(err);
				return SDL_APP_FAILURE;
			}
		}

		/// Shim around `current_application->handle_event()` to make it compatible with SDL.
		/// @param sdl_event SDL event to handle.
		/// @return SDL app result.
		[[nodiscard]] SDL_AppResult sdl_handle_event(void*, SDL_Event* sdl_event)
		{
			try {
				return static_cast<SDL_AppResult>(current_application->handle_event(reinterpret_cast<const event&>(*sdl_event)));
			}
			catch (std::exception& err) {
				show_fatal_error_message_box(err);
				return SDL_APP_FAILURE;
			}
		}

		/// Shim around `current_application->update()` to make it compatible with SDL.
		/// @return SDL app result.
		[[nodiscard]] SDL_AppResult sdl_update(void*)
		{
			try {
				static std::chrono::steady_clock::time_point prev{std::chrono::steady_clock::now()};
				const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
				const tr::duration delta{now - prev};
				prev = now;
				return static_cast<SDL_AppResult>(current_application->update(delta));
			}
			catch (std::exception& err) {
				show_fatal_error_message_box(err);
				return SDL_APP_FAILURE;
			}
		}

		/// Shim around `current_application->shut_down()` to make it compatible with SDL.
		/// @param sdl_signal SDL app result.
		void sdl_shut_down(void*, SDL_AppResult result)
		{
			try {
				current_application->shut_down(static_cast<application::signal>(result));
			}
			catch (std::exception& err) {
				show_fatal_error_message_box(err);
			}

			TTF_Quit();
		}

		/// Shim around `SDL_EnterAppMainCallbacks()` to call it through SDL_RunApp.
		/// @param argc Command-line argument count.
		/// @param argv Command-line argument values.
		/// @return Exit code of the application.
		[[nodiscard]] int sdl_run_main_loop(int argc, char** argv) noexcept
		{
			return SDL_EnterAppMainCallbacks(argc, argv, sdl_initialize, sdl_update, sdl_handle_event, sdl_shut_down);
		}
	} // namespace
} // namespace tr

//

void tr::set_application_metadata(const application::metadata& metadata)
{
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata.name.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata.version.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata.identifier.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata.developer.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata.copyright.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata.url.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, metadata.type == application::type::game ? "game" : "application");
	if (!metadata.name.empty()) {
		if (!metadata.version.empty()) {
			std::println("Launching {} {}.", metadata.name, metadata.version);
		}
		else {
			std::println("Launching {}.", metadata.name);
		}
	}
}

int tr::run_main_loop(application& application, int argc, const char** argv)
{
	TR_ASSERT(!current_application.has_ref(), "Tried to invoke the main loop while it is already ongoing.");

	current_application = application;
	const int exit_code{SDL_RunApp(argc, const_cast<char**>(argv), sdl_run_main_loop, nullptr)};
	current_application = std::nullopt;
	return exit_code;
}

void tr::set_update_frequency(float frequency) noexcept
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, frequency == uncapped_update_frequency ? "0" : std::to_string(frequency).c_str());
}