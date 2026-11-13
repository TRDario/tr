/// @file
/// @brief Provides mouse datatypes.

#pragma once
#include <tr/utility/enum.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @brief Mouse buttons (may be ORed together).
	enum class mouse_button : u8
	{
		/// @brief No mouse buttons.
		none,

		/// @brief Left mouse button.
		left,

		/// @brief Middle mouse button.
		middle,

		/// @brief Right mouse button.
		/// @hideinitializer
		right = 0x4,

		/// @brief First additional mouse button.
		/// @hideinitializer
		x1 = 0x8,

		/// @brief Second additional mouse button.
		/// @hideinitializer
		x2 = 0x10
	};

	/// @cond implementation_details
	TR_DEFINE_ENUM_BITMASK_OPERATORS(mouse_button);
	/// @endcond

	//

	/// @brief Mouse modes.
	enum class mouse_mode : bool
	{
		/// @brief Absolute mouse controls: normal operation.
		absolute,

		/// @brief Relative mouse controls: cursor is hidden, stuck to the center, and only reports deltas.
		relative
	};
} // namespace tr
