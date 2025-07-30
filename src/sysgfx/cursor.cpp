#include "../../include/tr/sysgfx/cursor.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include <SDL3/SDL.h>

namespace tr {
	// Checks that a pointer is not null.
	SDL_Cursor* check_not_null(SDL_Cursor* ptr);
} // namespace tr

SDL_Cursor* tr::check_not_null(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"cursor allocation"};
	}
	return ptr;
}

tr::cursor::cursor()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	ptr.reset(check_not_null(SDL_GetDefaultCursor()));
}

tr::cursor::cursor(sys_cursor icon)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	ptr.reset(check_not_null(SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(icon))));
}

tr::cursor::cursor(const bitmap& bitmap, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	ptr.reset(check_not_null(SDL_CreateColorCursor(bitmap.ptr.get(), focus.x, focus.y)));
}

tr::cursor::cursor(const bitmap_view& view, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");

	ptr.reset(check_not_null(SDL_CreateColorCursor(view.ptr.get(), focus.x, focus.y)));
}

tr::cursor::cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to create cursor before initializing the application.");
	TR_ASSERT(color.size() == mask.size(), "Mismatched cursor color and mask data: {} bytes for color vs {} bytes for mask", color.size(),
			  mask.size());
	TR_ASSERT(color.size() == size.x * size.y / 64ull, "Mismatched cursor data and size: {} bytes for color/mask vs {}x{} size ({} bytes)",
			  color.size(), size.x, size.y, size.x * size.y / 64);

	const std::uint8_t* ccolor{reinterpret_cast<const std::uint8_t*>(color.data())};
	const std::uint8_t* cmask{reinterpret_cast<const std::uint8_t*>(mask.data())};
	ptr.reset(check_not_null(SDL_CreateCursor(ccolor, cmask, size.x, size.y, focus.x, focus.y)));
}

void tr::cursor::deleter::operator()(SDL_Cursor* ptr) const
{
	SDL_DestroyCursor(ptr);
}