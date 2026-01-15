///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements cursor.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////////////////// CURSOR /////////////////////////////////////////////////////////////////

tr::sys::cursor::cursor(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"cursor allocation"};
	}
	m_ptr.reset(ptr);
}

tr::sys::cursor::cursor()
	: cursor{SDL_GetDefaultCursor()}
{
}

tr::sys::cursor::cursor(sys_cursor icon)
	: cursor{SDL_CreateSystemCursor(SDL_SystemCursor(icon))}
{
}

tr::sys::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(bitmap.m_ptr.get(), focus.x, focus.y)}
{
}

tr::sys::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(view.m_ptr.get(), focus.x, focus.y)}
{
}

void tr::sys::cursor::deleter::operator()(SDL_Cursor* ptr) const
{
	SDL_DestroyCursor(ptr);
}

/////////////////////////////////////////////////////////////// CURSOR STATE //////////////////////////////////////////////////////////////

void tr::sys::show_cursor()
{
	if (!SDL_ShowCursor()) {
		TR_LOG_SDL_ERROR("Failed to show mouse cursor.");
	}
}

void tr::sys::hide_cursor()
{
	if (!SDL_HideCursor()) {
		TR_LOG_SDL_ERROR("Failed to hide mouse cursor.");
	}
}

void tr::sys::set_cursor(const cursor& cursor)
{
	if (!SDL_SetCursor(cursor.m_ptr.get())) {
		TR_LOG_SDL_ERROR("Failed to set mouse cursor.");
	}
}