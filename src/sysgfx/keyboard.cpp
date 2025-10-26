#include "../../include/tr/sysgfx/keyboard.hpp"
#include <SDL3/SDL.h>

std::string tr::sys::name(keycode key)
{
	return SDL_GetKeyName(SDL_Keycode(key));
}

std::string tr::sys::name(key_chord chord)
{
	std::string str;
	if (chord.mods & keymod::CTRL) {
		str.append("Ctrl+");
	}
	if (chord.mods & keymod::ALT) {
		str.append("Alt+");
	}
	if (chord.mods & keymod::SHIFT) {
		str.append("Shift+");
	}
	str.append(name(chord.key));
	return str;
}

tr::sys::keycode tr::sys::impl::to_keycode(cstring_view str)
{
	return keycode(SDL_GetKeyFromName(str));
}

//

bool tr::sys::clipboard_empty()
{
	return !SDL_HasClipboardText();
}

std::string tr::sys::clipboard_text()
{
	std::unique_ptr<char, decltype(&SDL_free)> ptr{SDL_GetClipboardText(), SDL_free};
	return std::string{ptr != nullptr ? ptr.get() : std::string{}};
}

void tr::sys::set_clipboard_text(cstring_view text)
{
	if (!SDL_SetClipboardText(text)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set clipboard text.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

//

tr::u64 std::hash<tr::sys::scancode>::operator()(tr::sys::scancode code) const
{
	return tr::u64(int(code));
}

tr::u64 std::hash<tr::sys::keycode>::operator()(tr::sys::keycode code) const
{
	return tr::u64(int(code));
}

tr::u64 std::hash<tr::sys::scan_chord>::operator()(tr::sys::scan_chord chord) const
{
	return (tr::u64(int(chord.scan)) << 32) | tr::u64(chord.mods);
}

tr::u64 std::hash<tr::sys::key_chord>::operator()(tr::sys::key_chord chord) const
{
	return (tr::u64(int(chord.key)) << 32) | tr::u64(chord.mods);
}