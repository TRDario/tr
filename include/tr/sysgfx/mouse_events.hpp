#pragma once
#include "mouse.hpp"

namespace tr::sys {
	class event;

	// Event emitted when the mouse is moved.
	struct mouse_motion_event {
		// The event ID of mouse motion events.
		static inline constexpr u32 ID{0x400};

		// A mask of the held mouse buttons.
		mouse_button buttons;
		// The position of the mouse.
		glm::vec2 pos;
		// The change in mouse position since the last event of this type.
		glm::vec2 delta;

		// Converts a generic event into a mouse motion event.
		mouse_motion_event(const event& event);
	};

	// Event emitted when a mouse button is pressed.
	struct mouse_down_event {
		// The event ID of mouse down events.
		static inline constexpr u32 ID{0x401};

		// The pressed mouse button.
		mouse_button button;
		// The number of consecutive clicks.
		u8 clicks;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse down event.
		mouse_down_event(const event& event);
	};

	// Event emitted when a mouse button is released.
	struct mouse_up_event {
		// The event ID of mouse up events.
		static inline constexpr u32 ID{0x402};

		// The released mouse button.
		mouse_button button;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse up event.
		mouse_up_event(const event& event);
	};

	// Event emitted when the mouse wheel is moved.
	struct mouse_wheel_event {
		// The event ID of mouse wheel events.
		static inline constexpr u32 ID{0x403};

		// The change in wheel value.
		glm::vec2 delta;
		// The position of the mouse.
		glm::vec2 mouse_pos;

		// Converts a generic event into a mouse wheel event.
		mouse_wheel_event(const event& event);
	};
} // namespace tr::sys