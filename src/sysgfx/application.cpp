/// @file
/// @brief Implements application.hpp.

#define SDL_MAIN_HANDLED 1
#include "internal/sdl_main_callbacks.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tr/sysgfx/application.hpp>
#include <tr/sysgfx/dialog.hpp>
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

void tr::application::shut_down(signal) {}

//

int tr::run_main_loop(application& application, int argc, const char** argv)
{
	TR_ASSERT(!internal::running_application.has_value(), "Tried to invoke the main loop while it is already ongoing.");

	internal::running_application = application;
	const int exit_code{SDL_RunApp(argc, const_cast<char**>(argv), internal::run_main_loop, nullptr)};
	internal::running_application = std::nullopt;
	return exit_code;
}

void tr::set_update_frequency(float frequency) noexcept
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, frequency == uncapped_update_frequency ? "0" : std::to_string(frequency).c_str());
}