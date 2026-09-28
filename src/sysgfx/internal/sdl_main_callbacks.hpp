/// @file
/// @brief Provides SDL main callbacks.

#pragma once
#include <SDL3/SDL.h>
#include <tr/utility/opt_ref.hpp>

namespace tr
{
	class application;
}

//

namespace tr::internal
{
	/// Optional reference to the running application.
	inline opt_ref<application> running_application{};

	/// Global buffer allocated to be freed in case of an out-of-memory error.
	inline std::unique_ptr<char[]> emergency_buffer{new char[16384]};

	//

	/// Shows an "Fatal exception" message box.
	/// @details In case of an out-of-memory error, it frees an emergency buffer to allow for clean-up and logging.
	/// @param error Error to display.
	void show_fatal_error_message_box(const std::exception& error);

	//

	/// Shim around `running_application->initialize()` to make it compatible with SDL.
	/// @param argc, argv Command-line arguments.
	/// @return SDL app result.
	[[nodiscard]] SDL_AppResult initialize(void**, int argc, char** argv);

	/// Shim around `running_application->handle_event()` to make it compatible with SDL.
	/// @param sdl_event SDL event to handle.
	/// @return SDL app result.
	[[nodiscard]] SDL_AppResult handle_event(void*, SDL_Event* sdl_event);

	/// Shim around `running_application->update()` to make it compatible with SDL.
	/// @return SDL app result.
	[[nodiscard]] SDL_AppResult update(void*);

	/// Shim around `running_application->shut_down()` to make it compatible with SDL.
	/// @param sdl_signal SDL app result.
	void shut_down(void*, SDL_AppResult result);

	/// Shim around `SDL_EnterAppMainCallbacks()` to call it through SDL_RunApp.
	/// @param argc, argv Command-line arguments.
	/// @return Exit code of the application.
	[[nodiscard]] int run_main_loop(int argc, char** argv) noexcept;
} // namespace tr::internal