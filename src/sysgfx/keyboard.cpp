///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of keyboard.hpp.                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/keyboard.hpp"
#include "../../include/tr/sysgfx/event.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

/////////////////////////////////////////////////////////// SCANCODE AND KEYCODE //////////////////////////////////////////////////////////

std::string tr::sys::name(keycode key)
{
	return SDL_GetKeyName(SDL_Keycode(key));
}

tr::sys::keycode tr::sys::to_keycode_fallback(cstring_view str)
{
	return keycode(SDL_GetKeyFromName(str));
}

////////////////////////////////////////////////////////////////// CHORDS /////////////////////////////////////////////////////////////////

std::string tr::sys::key_chord::name() const
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
	str.append(sys::name(key));
	return str;
}

////////////////////////////////////////////////////////////// KEYBOARD STATE /////////////////////////////////////////////////////////////

constexpr int tr::sys::scan_state::to_index(scancode key)
{
	int index{int(key) - 4};
	if (index >= 102) {
		index -= 122;
	}
	return (index >= 0 && index <= 109) ? index : invalid_key_state_index;
}

bool tr::sys::scan_state::held(scancode key) const
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return false;
	}
	const std::byte& byte{buffer[index / 8]};
	return bool(byte & std::byte(1 << (index % 8)));
}

void tr::sys::scan_state::handle_event(const event& event)
{
	if (event.is<key_down_event>()) {
		handle_event(event.as<key_down_event>());
	}
	else if (event.is<key_up_event>()) {
		handle_event(event.as<key_up_event>());
	}
}

void tr::sys::scan_state::handle_event(const key_down_event& event)
{
	force_down(event.scan);
}

void tr::sys::scan_state::handle_event(const key_up_event& event)
{
	force_up(event.scan);
}

void tr::sys::scan_state::force_down(scancode key)
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte |= std::byte(1 << (index % 8));
}

void tr::sys::scan_state::force_up(scancode key)
{
	const int index{to_index(key)};
	if (index == invalid_key_state_index) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte &= ~std::byte(1 << (index % 8));
}

bool tr::sys::keyboard_state::held(keymod kmods) const
{
	return (mods & kmods) == kmods;
}

bool tr::sys::keyboard_state::held(scan_chord chord) const
{
	return held(chord.mods) && held(chord.scan);
}

void tr::sys::keyboard_state::handle_event(const event& event)
{
	if (event.is<key_down_event>()) {
		handle_event(event.as<key_down_event>());
	}
	else if (event.is<key_up_event>()) {
		handle_event(event.as<key_up_event>());
	}
}

void tr::sys::keyboard_state::handle_event(const key_down_event& event)
{
	scan_state::handle_event(event);
	mods = event.mods;
}

void tr::sys::keyboard_state::handle_event(const key_up_event& event)
{
	scan_state::handle_event(event);
	mods = event.mods;
}

//////////////////////////////////////////////////////////////// CLIPBOARD ////////////////////////////////////////////////////////////////

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
		TR_LOG_SDL_ERROR("Failed to set clipboard text.");
	}
}

////////////////////////////////////////////////////////////////// HASHES /////////////////////////////////////////////////////////////////

std::size_t std::hash<tr::sys::scancode>::operator()(tr::sys::scancode code) const
{
	return std::size_t(code);
}

std::size_t std::hash<tr::sys::keycode>::operator()(tr::sys::keycode code) const
{
	return std::size_t(code);
}

std::size_t std::hash<tr::sys::scan_chord>::operator()(tr::sys::scan_chord chord) const
{
	return (std::size_t(chord.scan) << 32) | std::size_t(chord.mods);
}

std::size_t std::hash<tr::sys::key_chord>::operator()(tr::sys::key_chord chord) const
{
	return (std::size_t(chord.key) << 32) | std::size_t(chord.mods);
}

//////////////////////////////////////////////////////////////// BINARY IO ////////////////////////////////////////////////////////////////

void tr::binary_reader<tr::sys::scan_chord>::operator()(std::istream& is, tr::sys::scan_chord& out) const
{
	read_binary(is, out.mods);
	read_binary(is, out.scan);
}

void tr::binary_writer<tr::sys::scan_chord>::operator()(std::ostream& os, const tr::sys::scan_chord& in) const
{
	write_binary(os, in.mods);
	write_binary(os, in.scan);
}

void tr::binary_reader<tr::sys::key_chord>::operator()(std::istream& is, tr::sys::key_chord& out) const
{
	read_binary(is, out.mods);
	read_binary(is, out.key);
}

void tr::binary_writer<tr::sys::key_chord>::operator()(std::ostream& os, const tr::sys::key_chord& in) const
{
	write_binary(os, in.mods);
	write_binary(os, in.key);
}