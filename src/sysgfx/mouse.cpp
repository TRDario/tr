#include "../../include/tr/sysgfx/mouse.hpp"
#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

void tr::sys::set_mouse_relative_mode(bool relative)
{
	TR_ASSERT(g_sdl_window != nullptr, "Tried to set mouse relative mode state before opening the window.");

	if (!SDL_SetWindowRelativeMouseMode(g_sdl_window, relative)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set relative mouse mode.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}

#ifdef _WIN32
	if (relative && !g_cursor_reset_timer.has_value()) {
		// Workaround for an SDL bug where the cursor does not properly disappear sometimes in relative mode.
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
	else if (!relative) {
		g_cursor_reset_timer.reset();
	}
#endif
}

void tr::sys::set_mouse_captured(bool captured)
{
	if (!SDL_CaptureMouse(captured)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set mouse capture state.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::set_cursor(const cursor& cursor)
{
	if (!SDL_SetCursor(cursor.m_ptr.get())) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set mouse cursor.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::show_cursor(bool show)
{
	if (show) {
		if (!SDL_ShowCursor()) {
			TR_LOG(log, tr::severity::ERROR, "Failed to show mouse cursor.");
			TR_LOG_CONTINUE(log, "{}", SDL_GetError());
		}
	}
	else {
		if (!SDL_HideCursor()) {
			TR_LOG(log, tr::severity::ERROR, "Failed to hide mouse cursor.");
			TR_LOG_CONTINUE(log, "{}", SDL_GetError());
		}
	}
}
