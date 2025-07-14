#pragma once
#include "cursor.hpp"

struct SDL_Cursor;

namespace tr {
	struct mouse_motion_event;
	struct mouse_down_event;
	struct mouse_up_event;

	// Mouse buttons (may be ORed together).
	enum class mouse_button : std::uint8_t {
		NONE = 0x0,
		LEFT = 0x1,
		MIDDLE = 0x2,
		RIGHT = 0x4,
		X1 = 0x8,
		X2 = 0x10
	};
	DEFINE_BITMASK_OPERATORS(mouse_button);

	// Mouse functionality.
	struct mouse {
		// Sets whether relative mouse mode is enabled.
		static bool set_relative_mode(bool relative) noexcept;
		// Sets whether the mouse is captured.
		static bool set_captured(bool captured) noexcept;
		// Sets whether the mouse cursor is visible.
		static void show_cursor(bool show) noexcept;
		// Sets the mouse cursor.
		static void set_cursor(const cursor& cursor) noexcept;
	};
} // namespace tr
