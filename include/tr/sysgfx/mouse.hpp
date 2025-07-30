#pragma once
#include "../utility/common.hpp"

namespace tr {
	class cursor;

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
	namespace mouse {
		// Sets whether relative mouse mode is enabled.
		void set_relative_mode(bool relative);
		// Sets whether the mouse is captured.
		void set_captured(bool captured);
		// Sets whether the mouse cursor is visible.
		void show_cursor(bool show);
		// Sets the mouse cursor.
		void set_cursor(const cursor& cursor);
	}; // namespace mouse
} // namespace tr
