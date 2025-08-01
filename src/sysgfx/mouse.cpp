#include "../../include/tr/sysgfx/mouse.hpp"
#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

void tr::system::set_mouse_relative_mode(bool relative)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set mouse relative mode state before opening the window.");

	if (!SDL_SetWindowRelativeMouseMode(sdl_window, relative)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set relative mouse mode.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::system::set_mouse_captured(bool captured)
{
	if (!SDL_CaptureMouse(captured)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set mouse capture state.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::system::set_cursor(const cursor& cursor)
{
	if (!SDL_SetCursor(cursor.m_ptr.get())) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set mouse cursor.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::system::show_cursor(bool show)
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
