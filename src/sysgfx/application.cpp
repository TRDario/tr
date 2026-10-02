/// @file
/// @brief Implements application.hpp.

#define SDL_MAIN_HANDLED 1
#include "internal/sdl_main_callbacks.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tr/sysgfx/application.hpp>
#include <tr/sysgfx/dialog.hpp>
#include <tr/sysgfx/logger.hpp>
#include <tr/utility/dynamic_ref_cast.hpp>
#include <tr/utility/opt_ref.hpp>

//

tr::application::signal tr::application::initialize(std::span<const zstring_view>)
{
	return signal::proceed;
}

tr::application::signal tr::application::handle_event(const event&)
{
	return signal::proceed;
}

tr::application::signal tr::application::update(duration)
{
	return signal::proceed;
}

//

void tr::internal::set_application_metadata(const application_metadata& metadata)
{
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata.name.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata.version.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata.identifier.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata.developer.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata.copyright.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata.url.c_str());
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,
							   metadata.type == application_metadata::application_type::game ? "game" : "application");
	if (!metadata.name.empty()) {
		if (!metadata.version.empty()) {
			std::println("Launching {} {}.", metadata.name, metadata.version);
		}
		else {
			std::println("Launching {}.", metadata.name);
		}
	}
}

//

//

void tr::internal::show_fatal_error_message_box(const std::exception& error)
{
	if (dynamic_ref_cast<const std::bad_alloc>(error).has_value() || dynamic_ref_cast<const out_of_memory>(error).has_value()) {
		emergency_buffer.reset();
	}

	const char* const application_name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
	const std::string title{std::format("{} - Fatal Error", application_name)};

	opt_ref<const exception> tr_exception{dynamic_ref_cast<const exception>(error)};
	std::string message;
	if (tr_exception.has_value()) {
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

SDL_AppResult tr::internal::initialize_application(void**, int argc, char** argv)
{
	if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
		std::println(stderr, "Failed to initialize SDL3: {}", SDL_GetError());

		const char* const application_name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
		const std::string title{std::format("{} - Fatal Error", application_name)};
		const std::string message{
			std::format("A fatal error has occured (Failed to initialize SDL3).\n{}\nPress OK to exit the application.", SDL_GetError()),
		};
		show_message_box(message_box_type::error, tr::message_box_layout::ok, title, message);
		return SDL_APP_FAILURE;
	}

	try {
		const std::span<const zstring_view> args{reinterpret_cast<const zstring_view*>(argv), static_cast<usize>(argc)};
		return static_cast<SDL_AppResult>(running_application->initialize(args));
	}
	catch (std::exception& err) {
		show_fatal_error_message_box(err);
		return SDL_APP_FAILURE;
	}
}

SDL_AppResult tr::internal::handle_application_event(void*, SDL_Event* sdl_event)
{
	try {
		return static_cast<SDL_AppResult>(running_application->handle_event(reinterpret_cast<const event&>(*sdl_event)));
	}
	catch (std::exception& err) {
		internal::show_fatal_error_message_box(err);
		return SDL_APP_FAILURE;
	}
}

SDL_AppResult tr::internal::update_application(void*)
{
	try {
		static std::chrono::steady_clock::time_point prev{std::chrono::steady_clock::now()};
		const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
		const tr::duration delta{now - prev};
		prev = now;
		return static_cast<SDL_AppResult>(running_application->update(delta));
	}
	catch (std::exception& err) {
		internal::show_fatal_error_message_box(err);
		return SDL_APP_FAILURE;
	}
}

void tr::internal::shut_down_application(void*, SDL_AppResult)
{
	try {
		running_application.reset();
	}
	catch (std::exception& err) {
		internal::show_fatal_error_message_box(err);
	}

	TTF_Quit();
}

int tr::internal::run_application_main_loop(int argc, char** argv) noexcept
{
	return SDL_EnterAppMainCallbacks(argc, argv, initialize_application, update_application, handle_application_event,
									 shut_down_application);
}

int tr::internal::run_application(std::unique_ptr<application> application, int argc, const char** argv)
{
	TR_ASSERT(running_application == nullptr, "Tried to invoke the main loop while it is already ongoing.");

	running_application = std::move(application);
	const int exit_code{SDL_RunApp(argc, const_cast<char**>(argv), run_application_main_loop, nullptr)};
	return exit_code;
}

void tr::set_update_frequency(float frequency) noexcept
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, frequency == uncapped_update_frequency ? "0" : std::to_string(frequency).c_str());
}