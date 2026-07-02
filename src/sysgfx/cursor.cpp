///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements cursor.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include <SDL3/SDL.h>

/////////////////////////////////////////////////////////////// CURSOR ERROR //////////////////////////////////////////////////////////////

tr::cursor_error::cursor_error(std::string_view description)
	: m_description{description}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::cursor_error::name() const
{
	return "Cursor error";
}

std::string_view tr::cursor_error::description() const
{
	return m_description;
}

std::string_view tr::cursor_error::details() const
{
	return m_details;
}

////////////////////////////////////////////////////////////////// CURSOR /////////////////////////////////////////////////////////////////

tr::cursor::cursor(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"cursor allocation"};
	}
	m_ptr.reset(ptr);
}

tr::cursor::cursor()
	: cursor{SDL_GetDefaultCursor()}
{
}

tr::cursor::cursor(sys_cursor icon)
	: cursor{SDL_CreateSystemCursor(SDL_SystemCursor(icon))}
{
}

tr::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(bitmap.m_ptr.get(), focus.x, focus.y)}
{
}

tr::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
	: cursor{SDL_CreateColorCursor(view.m_ptr.get(), focus.x, focus.y)}
{
}

void tr::cursor::deleter::operator()(SDL_Cursor* ptr) const
{
	SDL_DestroyCursor(ptr);
}

/////////////////////////////////////////////////////////////// CURSOR STATE //////////////////////////////////////////////////////////////

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
	if (!SDL_SetCursor(cursor.m_ptr.get())) {
		throw cursor_error("Failed to set mouse cursor.");
	}
}