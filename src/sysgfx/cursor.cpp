#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include <SDL3/SDL.h>

namespace tr::sys {
	// Checks that a pointer is not null.
	SDL_Cursor* check_not_null(SDL_Cursor* ptr);
} // namespace tr::sys

SDL_Cursor* tr::sys::check_not_null(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"cursor allocation"};
	}
	return ptr;
}

tr::sys::cursor::cursor()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_GetDefaultCursor()));
}

tr::sys::cursor::cursor(sys_cursor icon)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateSystemCursor(SDL_SystemCursor(icon))));
}

tr::sys::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateColorCursor(bitmap.m_ptr.get(), focus.x, focus.y)));
}

tr::sys::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateColorCursor(view.m_ptr.get(), focus.x, focus.y)));
}

tr::sys::cursor::cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");
	TR_ASSERT(color.size() == mask.size(), "Mismatched cursor color and mask data: {} bytes for color vs {} bytes for mask", color.size(),
			  mask.size());
	TR_ASSERT(color.size() == size.x * size.y / 64ull, "Mismatched cursor data and size: {} bytes for color/mask vs {}x{} size ({} bytes)",
			  color.size(), size.x, size.y, size.x * size.y / 64);

	const u8* ccolor{(const u8*)color.data()};
	const u8* cmask{(const u8*)mask.data()};
	m_ptr.reset(check_not_null(SDL_CreateCursor(ccolor, cmask, size.x, size.y, focus.x, focus.y)));
}

void tr::sys::cursor::deleter::operator()(SDL_Cursor* ptr) const
{
	SDL_DestroyCursor(ptr);
}