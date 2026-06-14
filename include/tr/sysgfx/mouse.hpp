///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides mouse datatypes.                                                                                                             //
//                                                                                                                                       //
// Mouse buttons are represented with a bitmask. A mouse state structure containing the position and held buttons of the mouse is also   //
// provided:                                                                                                                             //
//     - tr::mouse_button buttons{tr::mouse_button::left | tr::mouse_button::right} -> left and right mouse buttons held                 //
//     - tr::mouse_state state{.pos = {500, 500}, .held_buttonstr::mouse_button::left} -> mouse at (500, 500), holding LMB               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/macro.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Mouse buttons (may be ORed together).
	enum class mouse_button : u8 {
		none = 0x0,
		left = 0x1,
		middle = 0x2,
		right = 0x4,
		x1 = 0x8,
		x2 = 0x10
	};
	TR_DEFINE_ENUM_BITMASK_OPERATORS(mouse_button);

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
} // namespace tr
