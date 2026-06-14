///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements display.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/utility/defer.hpp"
#include <SDL3/SDL.h>

///////////////////////////////////////////////////////////////// DISPLAY /////////////////////////////////////////////////////////////////

glm::ivec2 tr::display_size()
{
	const SDL_DisplayMode& mode{*SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())};
	return {mode.w, mode.h};
}

tr::u8 tr::max_msaa()
{
	constexpr u8 unknown{255};

	static u8 max{unknown};
	if (max == unknown) {
		max = 32;
		while (max > 0) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, max);
			SDL_Window* window{SDL_CreateWindow("", 500, 500, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL)};
			if (window == nullptr) {
				max /= 2;
				continue;
			}
			TR_DEFER(SDL_DestroyWindow(window));
			SDL_GLContext context{SDL_GL_CreateContext(window)};
			if (context == nullptr) {
				max /= 2;
				continue;
			}
			TR_DEFER(SDL_GL_DestroyContext(context));

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
	}
	return max;
}

float tr::refresh_rate()
{
	return SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())->refresh_rate;
}