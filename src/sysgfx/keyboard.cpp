#include "../../include/tr/sysgfx/keyboard.hpp"
#include "../../include/tr/sysgfx/keyboard_events.hpp"
#include <SDL3/SDL.h>

namespace tr::sys {
	// Basic offset of all key state indices compared to the scancodes.
	inline constexpr int KEY_STATE_INDEX_OFFSET_1{int(scancode::A)};
	// Offset of modifier key state indices compared to the scan
	inline constexpr int KEY_STATE_INDEX_OFFSET_2{int(scancode::LCTRL) - int(scancode::APPLICATION) - 1};
	// Marker for when to start applying the second index offset.
	inline constexpr int KEY_STATE_INDEX_OFFSET_2_START{int(scancode::APPLICATION) + 1};
	// The maximum allowed key state index.
	inline constexpr int KEY_STATE_MAX_INDEX{int(scancode::RWIN) - KEY_STATE_INDEX_OFFSET_2};
	// Sentinel for an invalid key state index.
	inline constexpr int INVALID_KEY_STATE_INDEX{-1};
	// Converts a scancode into a key state index.
	constexpr int key_state_index(scancode key);
} // namespace tr::sys

constexpr int tr::sys::key_state_index(scancode key)
{
	int index{int(key) - KEY_STATE_INDEX_OFFSET_1};
	if (index >= KEY_STATE_INDEX_OFFSET_2_START) {
		index -= KEY_STATE_INDEX_OFFSET_2;
	}
	return (index >= 0 && index <= KEY_STATE_MAX_INDEX) ? index : INVALID_KEY_STATE_INDEX;
}

tr::sys::scancode::scancode(cstring_view name)
	: m_base{enum_t(SDL_GetScancodeFromName(name))}
{
}

tr::cstring_view tr::sys::scancode::name() const
{
	return SDL_GetScancodeName(SDL_Scancode(m_base));
}

tr::sys::keycode::keycode(cstring_view name)
	: m_base{enum_t(SDL_GetKeyFromName(name))}
{
}

std::string tr::sys::keycode::name() const
{
	return SDL_GetKeyName(SDL_Keycode(m_base));
}

bool tr::sys::key_state::held(scancode key) const
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return false;
	}
	const std::byte& byte{buffer[index / 8]};
	return bool(byte & std::byte(1 << (index % 8)));
}

void tr::sys::key_state::update(const key_down_event& event)
{
	force_down(event.scan);
}

void tr::sys::key_state::update(const key_up_event& event)
{
	force_up(event.scan);
}

void tr::sys::key_state::force_down(scancode key)
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte |= std::byte(1 << (index % 8));
}

void tr::sys::key_state::force_up(scancode key)
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte &= ~std::byte(1 << (index % 8));
}

void tr::sys::keyboard_state::update(const key_down_event& event)
{
	key_state::update(event);
	mods = event.mods;
}

void tr::sys::keyboard_state::update(const key_up_event& event)
{
	key_state::update(event);
	mods = event.mods;
}

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