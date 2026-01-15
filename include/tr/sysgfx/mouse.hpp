///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides mouse datatypes and functionality.                                                                                           //
//                                                                                                                                       //
// Mouse buttons are represented with a bitmask. A mouse state structure containing the position and held buttons of the mouse is also   //
// provided:                                                                                                                             //
//     - tr::sys::mouse_button buttons{tr::sys::mouse_button::left | tr::sys::mouse_button::right} -> left and right mouse buttons held  //
//     - tr::sys::mouse_state state{.pos = {500, 500}, .held_buttonstr::sys::mouse_button::left} -> mouse at (500, 500), holding LMB     //
//                                                                                                                                       //
// The mouse mode can be changed between absolute and relative:                                                                          //
//     - tr::sys::set_mouse_mode(tr::sys::mouse_mode::absolute) -> absolute mouse controls: normal operation                             //
//     - tr::sys::set_mouse_mode(tr::sys::mouse_mode::relative) -> relative mouse controls: cursor is hidden and only reports deltas     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
	// Mouse buttons (may be ORed together).
	enum class mouse_button : u8 {
		none = 0x0,
		left = 0x1,
		middle = 0x2,
		right = 0x4,
		x1 = 0x8,
		x2 = 0x10
	};
	DEFINE_BITMASK_OPERATORS(mouse_button);

	// Mouse state.
	struct mouse_state {
		// The current position of the mouse.
		glm::vec2 pos;
		// The currently-held mouse buttons.
		mouse_button held_buttons;
	};

	// Mouse modes.
	enum class mouse_mode : bool {
		absolute, // Absolute mouse controls: normal operation.
		relative  // Relative mouse controls: cursor is hidden, stuck to the center, and only reports deltas.
	};
	// Sets the mouse mode.
	void set_mouse_mode(mouse_mode mode);
} // namespace tr::sys
