#include "../../include/tr/sysgfx/keyboard.hpp"
#include "../../include/tr/sysgfx/keyboard_events.hpp"
#include <SDL3/SDL.h>

namespace tr::system {
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
} // namespace tr::system

constexpr int tr::system::key_state_index(scancode key)
{
	int index{int(key) - KEY_STATE_INDEX_OFFSET_1};
	if (index >= KEY_STATE_INDEX_OFFSET_2_START) {
		index -= KEY_STATE_INDEX_OFFSET_2;
	}
	return (index >= 0 && index <= KEY_STATE_MAX_INDEX) ? index : INVALID_KEY_STATE_INDEX;
}

tr::system::scancode::scancode(const char* name)
	: m_base{enum_t(SDL_GetScancodeFromName(name))}
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get scancode from name before initializing the application.");
}

const char* tr::system::scancode::name() const
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get scancode name before initializing the application.");

	return SDL_GetScancodeName(SDL_Scancode(m_base));
}

tr::system::keycode::keycode(const char* name)
	: m_base{enum_t(SDL_GetKeyFromName(name))}
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get keycode from name before initializing the application.");
}

std::string tr::system::keycode::name() const
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get keycode name before initializing the application.");

	return SDL_GetKeyName(SDL_Keycode(m_base));
}

bool tr::system::key_state::held(scancode key) const
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return false;
	}
	const std::byte& byte{buffer[index / 8]};
	return bool(byte & std::byte(1 << (index % 8)));
}

void tr::system::key_state::update(const key_down_event& event)
{
	force_down(event.scan);
}

void tr::system::key_state::update(const key_up_event& event)
{
	force_up(event.scan);
}

void tr::system::key_state::force_down(scancode key)
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte |= std::byte(1 << (index % 8));
}

void tr::system::key_state::force_up(scancode key)
{
	const int index{key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{buffer[index / 8]};
	byte &= ~std::byte(1 << (index % 8));
}

void tr::system::keyboard_state::update(const key_down_event& event)
{
	key_state::update(event);
	mods = event.mods;
}

void tr::system::keyboard_state::update(const key_up_event& event)
{
	key_state::update(event);
	mods = event.mods;
}

bool tr::system::clipboard_empty()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to query clipboard state before initializing the application.");

	return !SDL_HasClipboardText();
}

std::string tr::system::clipboard_text()
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get clipboard text before initializing the application.");

	std::unique_ptr<char, decltype(&SDL_free)> ptr{SDL_GetClipboardText(), SDL_free};
	return std::string{ptr != nullptr ? ptr.get() : std::string{}};
}

void tr::system::set_clipboard_text(const char* text)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to set clipboard text before initializing the application.");

	if (!SDL_SetClipboardText(text)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set clipboard text.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::system::set_clipboard_text(const std::string& text)
{
	set_clipboard_text(text.c_str());
}

tr::u64 std::hash<tr::system::scancode>::operator()(tr::system::scancode code) const
{
	return tr::u64(int(code));
}

tr::u64 std::hash<tr::system::keycode>::operator()(tr::system::keycode code) const
{
	return tr::u64(int(code));
}

tr::u64 std::hash<tr::system::scan_chord>::operator()(tr::system::scan_chord chord) const
{
	return (tr::u64(int(chord.scan)) << 32) | tr::u64(chord.mods);
}

tr::u64 std::hash<tr::system::key_chord>::operator()(tr::system::key_chord chord) const
{
	return (tr::u64(int(chord.key)) << 32) | tr::u64(chord.mods);
}