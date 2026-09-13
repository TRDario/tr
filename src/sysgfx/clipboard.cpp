/// @file
/// @brief Implements clipboard.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/clipboard.hpp>

//

tr::set_clipboard_error::set_clipboard_error() noexcept
	: m_description{SDL_GetError()}
{
}

std::string_view tr::set_clipboard_error::name() const noexcept
{
	return "Clipboard setting error";
}

std::string_view tr::set_clipboard_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::set_clipboard_error::details() const noexcept
{
	return {};
}

//

bool tr::clipboard_empty() noexcept
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