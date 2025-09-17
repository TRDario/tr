#pragma once
#include "mouse.hpp"

namespace tr::sys {
	class event;

	// Event emitted when the mouse is moved.
	struct mouse_motion_event {
		// A mask of the held mouse buttons.
		mouse_button buttons;
		// The position of the mouse.
		glm::vec2 pos;
		// The change in mouse position since the last event of this type.
		glm::vec2 delta;

		// Converts a generic event into a mouse motion event.
		explicit mouse_motion_event(const event& event);
	};

	// Event emitted when a mouse button is pressed.
	struct mouse_down_event {
		// The pressed mouse button.
		mouse_button button;
		// The number of consecutive clicks.
		u8 clicks;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse down event.
		explicit mouse_down_event(const event& event);
	};

	// Event emitted when a mouse button is released.
	struct mouse_up_event {
		// The released mouse button.
		mouse_button button;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse up event.
		explicit mouse_up_event(const event& event);
	};

	// Event emitted when the mouse wheel is moved.
	struct mouse_wheel_event {
		// The change in wheel value.
		glm::vec2 delta;
		// The position of the mouse.
		glm::vec2 mouse_pos;

		// Converts a generic event into a mouse wheel event.
		explicit mouse_wheel_event(const event& event);
	};
} // namespace tr::sys