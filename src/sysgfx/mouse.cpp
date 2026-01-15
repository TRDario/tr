///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements mouse.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/mouse.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

//////////////////////////////////////////////////////////////// MOUSE MODE ///////////////////////////////////////////////////////////////

void tr::sys::set_mouse_mode(mouse_mode mode)
{
	TR_ASSERT(g_sdl_window != nullptr, "Tried to set mouse mode before opening the window.");

	if (!SDL_SetWindowRelativeMouseMode(g_sdl_window, bool(mode))) {
		TR_LOG_SDL_ERROR("Failed to set mouse mode.");
	}

	// Workaround for an SDL bug where the cursor does not properly disappear sometimes in relative mode on Windows.
#ifdef _WIN32
	switch (mode) {
	case mouse_mode::absolute:
		g_cursor_reset_timer.reset();
		break;
	case mouse_mode::relative:
		if (!g_cursor_reset_timer.has_value()) {
			g_cursor_reset_timer.emplace(std::chrono::seconds{1}, [] {
				float x, y;
				SDL_GetMouseState(&x, &y);
				SDL_SetWindowRelativeMouseMode(g_sdl_window, false);
				const bool cursor_visible{SDL_CursorVisible()};
				if (cursor_visible) {
					SDL_HideCursor();
				}
				SDL_WarpMouseInWindow(g_sdl_window, x, y);
				if (cursor_visible) {
					SDL_ShowCursor();
				}
				SDL_SetWindowRelativeMouseMode(g_sdl_window, true);
			});
		}
		break;
	}
#endif
}