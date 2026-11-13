/// @file
/// @brief Implements clipboard.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/clipboard.hpp>
#include <tr/sysgfx/exception.hpp>
#include <tr/utility/defer.hpp>

//

bool tr::clipboard_empty() noexcept
{
	return !SDL_HasClipboardText();
}

std::string tr::clipboard_text()
{
	char* const raw_result{SDL_GetClipboardText()};
	TR_DEFER(SDL_free(raw_result));

	std::string result;
	if (raw_result) {
		result.assign(raw_result);
	}
	return result;
}

void tr::set_clipboard_text(zstring_view text)
{
	if (!SDL_SetClipboardText(text.c_str())) {
		throw set_clipboard_error{};
	}
}