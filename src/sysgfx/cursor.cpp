/// @file
/// @brief Implements cursor.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/bitmap.hpp>
#include <tr/sysgfx/bitmap_view.hpp>
#include <tr/sysgfx/cursor.hpp>
#include <tr/sysgfx/exception.hpp>

//

tr::cursor::cursor()
	: cursor{SDL_GetDefaultCursor()}
{
}

tr::cursor::cursor(SDL_Cursor* ptr)
{
	if (!ptr) {
		throw out_of_memory{"cursor allocation"};
	}
	m_ptr.reset(ptr);
}

tr::cursor::cursor(sys_cursor icon)
	: cursor{SDL_CreateSystemCursor(SDL_SystemCursor(icon))}
{
}

tr::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(bitmap.pointer(), focus.x, focus.y)}
{
}

tr::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(view.pointer(), focus.x, focus.y)}
{
}

void tr::cursor::sdl_cursor_deleter::operator()(SDL_Cursor* ptr) noexcept
{
	SDL_DestroyCursor(ptr);
}

//

tr::cursor::operator bool() const noexcept
{
	return static_cast<bool>(m_ptr);
}

bool tr::cursor::valid() const noexcept
{
	return static_cast<bool>(m_ptr);
}

SDL_Cursor* tr::cursor::pointer() const noexcept
{
	return m_ptr.get();
}

//

void tr::show_cursor()
{
	if (!SDL_ShowCursor()) {
		throw cursor_error("Failed to show mouse cursor.");
	}
}

void tr::hide_cursor()
{
	if (!SDL_HideCursor()) {
		throw cursor_error("Failed to hide mouse cursor.");
	}
}

void tr::set_cursor(const cursor& cursor)
{
	TR_ASSERT(cursor.valid(), "Tried to set a cursor in an invalid state.");

	if (!SDL_SetCursor(cursor.pointer())) {
		throw cursor_error("Failed to set mouse cursor.");
	}
}