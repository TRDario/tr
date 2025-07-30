#pragma once
#include "keyboard.hpp"

namespace tr {
	class event;

	// Event emitted when a key is pressed.
	struct key_down_event {
		// The event ID of key down events.
		static inline constexpr std::uint32_t ID{0x300};

		// Whether the event is a repeat.
		bool repeat;
		// The physical keycode of the pressed key.
		scancode scan;
		// The virtual keycode of the pressed key.
		keycode key;
		// A bitmask of held modifiers when the key was pressed.
		keymod mods;

		// Converts a generic event into a key down event.
		key_down_event(const event& event);
	};
	// Checks if a key chord was pressed.
	bool operator==(const key_down_event& event, const key_chord& chord);
	// Checks if a scan chord was pressed.
	bool operator==(const key_down_event& event, const scan_chord& chord);
	// Checks if a key chord was pressed.
	bool operator==(const key_chord& chord, const key_down_event& event);
	// Checks if a scan chord was pressed.
	bool operator==(const scan_chord& chord, const key_down_event& event);

	// Event emitted when a key is release.
	struct key_up_event {
		// The event ID of key up events.
		static inline constexpr std::uint32_t ID{0x301};

		// The physical keycode of the released key.
		scancode scan;
		// The virtual keycode of the released key.
		keycode key;
		// A bitmask of held modifiers when the key was released.
		keymod mods;

		// Converts a generic event into a key up event.
		key_up_event(const event& event);
	};

	// Event emitted when text is inputted.
	struct text_input_event {
		// The event ID of text input events.
		static inline constexpr std::uint32_t ID{0x303};

		// A view over the inputted text string.
		std::string_view text;

		// Converts a generic event into a text input event.
		text_input_event(const event& event);
	};
} // namespace tr