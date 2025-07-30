#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

glm::ivec2 tr::display_size() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get display size before initializing the application.");

	const SDL_DisplayMode& mode{*SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())};
	return {mode.w, mode.h};
}

std::uint8_t tr::max_msaa() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get max MSAA before initializing the application.");

	constexpr std::uint8_t UNKNOWN{255};
	using wrapped_window = std::unique_ptr<SDL_Window, decltype([](SDL_Window* w) { SDL_DestroyWindow(w); })>;
	using wrapped_context = std::unique_ptr<SDL_GLContextState, decltype([](SDL_GLContext c) { SDL_GL_DestroyContext(c); })>;

	static std::uint8_t max{UNKNOWN};
	if (max == UNKNOWN) {
		max = 32;
		while (max > 0) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, max);
			wrapped_window window{SDL_CreateWindow("", 500, 500, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL)};
			if (window == nullptr) {
				max = static_cast<std::uint8_t>(max / 2);
				continue;
			}
			wrapped_context context{SDL_GL_CreateContext(window.get())};
			if (context == nullptr) {
				max = static_cast<std::uint8_t>(max / 2);
				continue;
			}

			int real_samples;
			SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &real_samples);
			if (static_cast<std::uint8_t>(real_samples) != max) {
				max = static_cast<std::uint8_t>(max / 2);
				continue;
			}
			else {
				break;
			}
		}

		if (ogl_context != nullptr) {
			SDL_GL_MakeCurrent(sdl_window, ogl_context);
		}
	}
	return max;
}

float tr::refresh_rate() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get refresh rate before initializing the application.");

	return SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())->refresh_rate;
}