/// @file
/// @brief Implements internal/sdl_main_callbacks.hpp.

#define SDL_MAIN_HANDLED 1
#include "internal/sdl_main_callbacks.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tr/sysgfx/application.hpp>
#include <tr/sysgfx/dialog.hpp>
#include <tr/sysgfx/logger.hpp>
#include <tr/utility/dynamic_ref_cast.hpp>
#include <tr/utility/exception.hpp>

//

void tr::internal::show_fatal_error_message_box(const std::exception& error)
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

SDL_AppResult tr::internal::initialize(void**, int, char**)
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
		return static_cast<SDL_AppResult>(running_application->initialize());
	}
	catch (std::exception& err) {
		show_fatal_error_message_box(err);
		return SDL_APP_FAILURE;
	}
}

SDL_AppResult tr::internal::handle_event(void*, SDL_Event* sdl_event)
{
	try {
		return static_cast<SDL_AppResult>(running_application->handle_event(reinterpret_cast<const event&>(*sdl_event)));
	}
	catch (std::exception& err) {
		internal::show_fatal_error_message_box(err);
		return SDL_APP_FAILURE;
	}
}

SDL_AppResult tr::internal::update(void*)
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

void tr::internal::shut_down(void*, SDL_AppResult result)
{
	try {
		running_application->shut_down(static_cast<application::signal>(result));
	}
	catch (std::exception& err) {
		internal::show_fatal_error_message_box(err);
	}

	TTF_Quit();
}

int tr::internal::run_main_loop(int argc, char** argv) noexcept
{
	return SDL_EnterAppMainCallbacks(argc, argv, initialize, update, handle_event, shut_down);
}