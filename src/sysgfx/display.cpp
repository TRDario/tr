#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

glm::ivec2 tr::sys::display_size()
{
	const SDL_DisplayMode& mode{*SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())};
	return {mode.w, mode.h};
}

tr::u8 tr::sys::max_msaa()
{
	constexpr u8 UNKNOWN{255};
	using wrapped_window = std::unique_ptr<SDL_Window, decltype([](SDL_Window* w) { SDL_DestroyWindow(w); })>;
	using wrapped_context = std::unique_ptr<SDL_GLContextState, decltype([](SDL_GLContext c) { SDL_GL_DestroyContext(c); })>;

	static u8 max{UNKNOWN};
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
				max /= 2;
				continue;
			}
			wrapped_context context{SDL_GL_CreateContext(window.get())};
			if (context == nullptr) {
				max /= 2;
				continue;
			}

			int real_samples;
			SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &real_samples);
			if (u8(real_samples) != max) {
				max /= 2;
				continue;
			}
			else {
				break;
			}
		}

		if (gfx::g_ogl_context != nullptr) {
			SDL_GL_MakeCurrent(g_sdl_window, gfx::g_ogl_context);
		}
	}
	return max;
}

float tr::sys::refresh_rate()
{
	return SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())->refresh_rate;
}