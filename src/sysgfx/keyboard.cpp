#include "../../include/tr/sysgfx/keyboard.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/keyboard_events.hpp"
#include <SDL3/SDL.h>

namespace tr {
	// Basic offset of all key state indices compared to the scancodes.
	inline constexpr int KEY_STATE_INDEX_OFFSET_1{static_cast<int>(scancode::A)};
	// Offset of modifier key state indices compared to the scan
	inline constexpr int KEY_STATE_INDEX_OFFSET_2{static_cast<int>(scancode::LCTRL) - static_cast<int>(scancode::APPLICATION) - 1};
	// Marker for when to start applying the second index offset.
	inline constexpr int KEY_STATE_INDEX_OFFSET_2_START{static_cast<int>(scancode::APPLICATION) + 1};
	// The maximum allowed key state index.
	inline constexpr int KEY_STATE_MAX_INDEX{static_cast<int>(scancode::RWIN) - KEY_STATE_INDEX_OFFSET_2};
	// Sentinel for an invalid key state index.
	inline constexpr int INVALID_KEY_STATE_INDEX{-1};
	// Converts a scancode into a key state index.
	constexpr int _key_state_index(scancode key) noexcept;
} // namespace tr

constexpr int tr::_key_state_index(scancode key) noexcept
{
	int index{static_cast<int>(key) - KEY_STATE_INDEX_OFFSET_1};
	if (index >= KEY_STATE_INDEX_OFFSET_2_START) {
		index -= KEY_STATE_INDEX_OFFSET_2;
	}
	return (index >= 0 && index <= KEY_STATE_MAX_INDEX) ? index : INVALID_KEY_STATE_INDEX;
}

tr::scancode::scancode(const char* name) noexcept
	: _enum{static_cast<enum_t>(SDL_GetScancodeFromName(name))}
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get scancode from name before initializing the application.");
}

const char* tr::scancode::name() const noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get scancode name before initializing the application.");

	return SDL_GetScancodeName(static_cast<SDL_Scancode>(_enum));
}

tr::keycode::keycode(const char* name) noexcept
	: _enum{static_cast<enum_t>(SDL_GetKeyFromName(name))}
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get keycode from name before initializing the application.");
}

std::string tr::keycode::name() const noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get keycode name before initializing the application.");

	return SDL_GetKeyName(static_cast<SDL_Keycode>(_enum));
}

bool tr::key_state::held(scancode key) const noexcept
{
	const int index{_key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return false;
	}
	const std::byte& byte{_data[index / 8]};
	return static_cast<bool>(byte & static_cast<std::byte>(1 << (index % 8)));
}

void tr::key_state::update(const key_down_event& event) noexcept
{
	force_down(event.scan);
}

void tr::key_state::update(const key_up_event& event) noexcept
{
	force_up(event.scan);
}

void tr::key_state::force_down(scancode key) noexcept
{
	const int index{_key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{_data[index / 8]};
	byte |= static_cast<std::byte>(1 << (index % 8));
}

void tr::key_state::force_up(scancode key) noexcept
{
	const int index{_key_state_index(key)};
	if (index == INVALID_KEY_STATE_INDEX) {
		return;
	}
	std::byte& byte{_data[index / 8]};
	byte &= ~static_cast<std::byte>(1 << (index % 8));
}

void tr::keyboard_state::update(const key_down_event& event) noexcept
{
	key_state::update(event);
	mods = event.mods;
}

void tr::keyboard_state::update(const key_up_event& event) noexcept
{
	key_state::update(event);
	mods = event.mods;
}

bool tr::clipboard::empty() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to query clipboard state before initializing the application.");

	return !SDL_HasClipboardText();
}

std::string tr::clipboard::get() noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to get clipboard text before initializing the application.");

	try {
		std::unique_ptr<char, decltype(&SDL_free)> ptr{SDL_GetClipboardText(), SDL_free};
		return std::string{ptr != nullptr ? ptr.get() : std::string{}};
	}
	catch (std::bad_alloc&) {
		return std::string{};
	}
}

void tr::clipboard::set(const char* text) noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to set clipboard text before initializing the application.");

	SDL_SetClipboardText(text);
}

void tr::clipboard::set(const std::string& text) noexcept
{
	set(text.c_str());
}