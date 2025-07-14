#include "../../include/tr/sysgfx/mouse.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

bool tr::mouse::set_relative_mode(bool relative) noexcept
{
	TR_ASSERT(_window != nullptr, "Tried to set mouse relative mode state before opening the window.");

	return !SDL_SetWindowRelativeMouseMode(_window, relative);
}

bool tr::mouse::set_captured(bool captured) noexcept
{
	return !SDL_CaptureMouse(captured);
}

void tr::mouse::set_cursor(const cursor& cursor) noexcept
{
	SDL_SetCursor(cursor._impl.get());
}

void tr::mouse::show_cursor(bool show) noexcept
{
	show ? SDL_ShowCursor() : SDL_HideCursor();
}
