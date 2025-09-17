#pragma once
#include "../utility/common.hpp"

namespace tr::sys {
	class event;

	// Event emitted when a window is shown.
	struct window_show_event {};
	// Event emitted when a window is hidden.
	struct window_hide_event {};
	// Event emitted when the window backbuffer changes size.
	struct backbuffer_resize_event {
		// The new size of the backbuffer.
		glm::ivec2 size;

		// Converts a generic event into a window backbuffer resizing event.
		explicit backbuffer_resize_event(const event& event);
	};
	// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event {};
	// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event {};
	// Event emitted when a window gains focus.
	struct window_gain_focus_event {};
	// Event emitted when a window loses focus.
	struct window_lose_focus_event {};
} // namespace tr::sys