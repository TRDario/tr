/// @file
/// @brief Implements clipboard.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/clipboard.hpp>
#include <tr/sysgfx/exception.hpp>

//

bool tr::clipboard_empty() noexcept
{
	return !SDL_HasClipboardText();
}

std::string tr::clipboard_text()
{
	char* const raw_result{SDL_GetClipboardText()};
	std::string result;
	if (raw_result) {
		result = raw_result;
		SDL_free(raw_result);
	}
	return result;
}

void tr::set_clipboard_text(zstring_view text)
{
	if (!SDL_SetClipboardText(text.c_str())) {
		throw set_clipboard_error{};
	}
}