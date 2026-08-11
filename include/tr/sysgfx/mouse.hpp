/// @file
/// @brief Provides mouse datatypes.

#pragma once
#include "../utility/enum.hpp"

//

namespace tr
{
	/// Mouse buttons (may be ORed together).
	enum class mouse_button : u8
	{
		/// No mouse buttons.
		none = 0x0,
		/// Left mouse button.
		left = 0x1,
		/// Middle mouse button.
		middle = 0x2,
		/// Right mouse button.
		right = 0x4,
		/// First additional mouse button.
		x1 = 0x8,
		/// Second additional mouse button.
		x2 = 0x10
	};

	/// @cond __hidden
	TR_DEFINE_ENUM_BITMASK_OPERATORS(mouse_button);
	/// @endcond

	//

	/// Mouse state.
	struct mouse_state
	{
		// Current position of the mouse.
		glm::vec2 pos;

		// Currently-held mouse buttons.
		mouse_button held_buttons;
	};

	//

	/// Mouse modes.
	enum class mouse_mode : bool
	{
		/// Absolute mouse controls: normal operation.
		absolute,
		/// Relative mouse controls: cursor is hidden, stuck to the center, and only reports deltas.
		relative
	};
} // namespace tr
