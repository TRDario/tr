#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include <SDL3/SDL.h>

namespace tr::system {
	// Checks that a pointer is not null.
	SDL_Cursor* check_not_null(SDL_Cursor* ptr);
} // namespace tr::system

SDL_Cursor* tr::system::check_not_null(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"cursor allocation"};
	}
	return ptr;
}

tr::system::cursor::cursor()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_GetDefaultCursor()));
}

tr::system::cursor::cursor(sys_cursor icon)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(icon))));
}

tr::system::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateColorCursor(bitmap.m_ptr.get(), focus.x, focus.y)));
}

tr::system::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	m_ptr.reset(check_not_null(SDL_CreateColorCursor(view.m_ptr.get(), focus.x, focus.y)));
}

tr::system::cursor::cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");
	TR_ASSERT(color.size() == mask.size(), "Mismatched cursor color and mask data: {} bytes for color vs {} bytes for mask", color.size(),
			  mask.size());
	TR_ASSERT(color.size() == size.x * size.y / 64ull, "Mismatched cursor data and size: {} bytes for color/mask vs {}x{} size ({} bytes)",
			  color.size(), size.x, size.y, size.x * size.y / 64);

	const std::uint8_t* ccolor{reinterpret_cast<const std::uint8_t*>(color.data())};
	const std::uint8_t* cmask{reinterpret_cast<const std::uint8_t*>(mask.data())};
	m_ptr.reset(check_not_null(SDL_CreateCursor(ccolor, cmask, size.x, size.y, focus.x, focus.y)));
}

void tr::system::cursor::deleter::operator()(SDL_Cursor* ptr) const
{
	SDL_DestroyCursor(ptr);
}