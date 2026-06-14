///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of keyboard.hpp.                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/keyboard.hpp"
#include "../../include/tr/sysgfx/event.hpp"
#include "../../include/tr/sysgfx/log_sdl_error.hpp"
#include <SDL3/SDL.h>

/////////////////////////////////////////////////////////// SCANCODE AND KEYCODE //////////////////////////////////////////////////////////

std::string tr::name(keycode key)
{
	return SDL_GetKeyName(SDL_Keycode(key));
}

tr::keycode tr::to_keycode_fallback(zstring_view str)
{
	return keycode(SDL_GetKeyFromName(str.c_str()));
}

////////////////////////////////////////////////////////////////// CHORDS /////////////////////////////////////////////////////////////////

std::string tr::key_chord::name() const
{
	std::string str;
	if (mods & keymod::ctrl) {
		str.append("Ctrl+");
	}
	if (mods & keymod::alt) {
		str.append("Alt+");
	}
	if (mods & keymod::shift) {
		str.append("Shift+");
	}
	str.append(tr::name(key));
	return str;
}

////////////////////////////////////////////////////////////// KEYBOARD STATE /////////////////////////////////////////////////////////////

constexpr int tr::scan_state::to_index(scancode key)
{
	int index{int(key) - 4};
	if (index >= 102) {
		index -= 122;
	}
	return (index >= 0 && index <= 109) ? index : invalid_key_state_index;
}

bool tr::scan_state::held(scancode key) const
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return false;
	}
	const std::byte& byte{buffer[index / 8]};
	return bool(byte & std::byte(1 << (index % 8)));
}

void tr::scan_state::handle_event(const event& event)
{
	if (event.is<key_down_event>()) {
		handle_event(event.as<key_down_event>());
	}
	else if (event.is<key_up_event>()) {
		handle_event(event.as<key_up_event>());
	}
}

void tr::scan_state::handle_event(const key_down_event& event)
{
	force_down(event.scan);
}

void tr::scan_state::handle_event(const key_up_event& event)
{
	force_up(event.scan);
}

void tr::scan_state::force_down(scancode key)
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte |= std::byte(1 << (index % 8));
}

void tr::scan_state::force_up(scancode key)
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte &= ~std::byte(1 << (index % 8));
}

bool tr::keyboard_state::held(keymod kmods) const
{
	return (mods & kmods) == kmods;
}

bool tr::keyboard_state::held(scan_chord chord) const
{
	return held(chord.mods) && held(chord.scan);
}

void tr::keyboard_state::handle_event(const event& event)
{
	if (event.is<key_down_event>()) {
		handle_event(event.as<key_down_event>());
	}
	else if (event.is<key_up_event>()) {
		handle_event(event.as<key_up_event>());
	}
}

void tr::keyboard_state::handle_event(const key_down_event& event)
{
	scan_state::handle_event(event);
	mods = event.mods;
}

void tr::keyboard_state::handle_event(const key_up_event& event)
{
	scan_state::handle_event(event);
	mods = event.mods;
}

//////////////////////////////////////////////////////////////// CLIPBOARD ////////////////////////////////////////////////////////////////

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
		TR_LOG_SDL_ERROR("Failed to set clipboard text.");
	}
}

////////////////////////////////////////////////////////////////// HASHES /////////////////////////////////////////////////////////////////

std::size_t boost::hash<tr::scancode>::operator()(tr::scancode code) const
{
	return std::size_t(code);
}

std::size_t boost::hash<tr::keycode>::operator()(tr::keycode code) const
{
	return std::size_t(code);
}

std::size_t boost::hash<tr::scan_chord>::operator()(tr::scan_chord chord) const
{
	return (std::size_t(chord.scan) << 32) | std::size_t(chord.mods);
}

std::size_t boost::hash<tr::key_chord>::operator()(tr::key_chord chord) const
{
	return (std::size_t(chord.key) << 32) | std::size_t(chord.mods);
}

//////////////////////////////////////////////////////////////// BINARY IO ////////////////////////////////////////////////////////////////

void tr::binary_reader<tr::scan_chord>::operator()(std::istream& is, tr::scan_chord& out) const
{
	read_binary(is, out.mods, out.scan);
}

void tr::binary_writer<tr::scan_chord>::operator()(std::ostream& os, const tr::scan_chord& in) const
{
	write_binary(os, in.mods, in.scan);
}

void tr::binary_reader<tr::key_chord>::operator()(std::istream& is, tr::key_chord& out) const
{
	read_binary(is, out.mods, out.key);
}

void tr::binary_writer<tr::key_chord>::operator()(std::ostream& os, const tr::key_chord& in) const
{
	write_binary(os, in.mods, in.key);
}