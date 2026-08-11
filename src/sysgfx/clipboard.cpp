/// @file
/// @brief Implements clipboard.hpp.

#include "../../include/tr/sysgfx/clipboard.hpp"
#include <SDL3/SDL.h>

//

tr::set_clipboard_error::set_clipboard_error()
	: m_description{SDL_GetError()}
{
}

std::string_view tr::set_clipboard_error::name() const
{
	return "Clipboard setting error";
}

std::string_view tr::set_clipboard_error::description() const
{
	return m_description;
}

std::string_view tr::set_clipboard_error::details() const
{
	return {};
}

//

bool tr::clipboard_empty()
{
	return !SDL_HasClipboardText();
}

std::string tr::clipboard_text()
{
	char* const raw{SDL_GetClipboardText()};
	std::string result;
	if (raw != nullptr) {
		result = raw;
		SDL_free(raw);
	}
	return result;
}

void tr::set_clipboard_text(zstring_view text)
{
	if (!SDL_SetClipboardText(text.c_str())) {
		throw set_clipboard_error{};
	}
}