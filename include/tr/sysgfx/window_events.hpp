#pragma once
#include "../utility/common.hpp"

namespace tr::sys {
	class event;

	// Event emitted when a window is shown.
	struct window_show_event {
		// The event ID of window showing events.
		static inline constexpr u32 ID{0x202};
	};

	// Event emitted when a window is hidden.
	struct window_hide_event {
		// The event ID of window hiding events.
		static inline constexpr u32 ID{0x203};
	};

	// Event emitted when the window backbuffer changes size.
	struct backbuffer_resize_event {
		// The event ID of window backbuffer resizing events.
		static inline constexpr u32 ID{0x207};

		// The new size of the backbuffer.
		glm::ivec2 size;

		// Converts a generic event into a window backbuffer resizing event.
		backbuffer_resize_event(const event& event);
	};

	// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event {
		// The event ID of mouse entering events.
		static inline constexpr u32 ID{0x20C};
	};

	// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event {
		// The event ID of mouse leaving events.
		static inline constexpr u32 ID{0x20D};
	};

	// Event emitted when a window gains focus.
	struct window_gain_focus_event {
		// The event ID of window faining focus events.
		static inline constexpr u32 ID{0x20E};
	};

	// Event emitted when a window loses focus.
	struct window_lose_focus_event {
		// The event ID of window losing focus events.
		static inline constexpr u32 ID{0x20F};
	};
} // namespace tr::sys