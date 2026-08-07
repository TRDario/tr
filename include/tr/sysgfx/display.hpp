/// @file
/// @brief Provides display querying functions.

#pragma once
#include "../utility/integer.hpp"

//

namespace tr
{
	/// @name Display
	/// @{

	/// Gets the size of the display.
	/// @return Size of the display in pixels.
	glm::ivec2 display_size();

	/// Gets the maximum supported MSAA factor.
	/// @return Maximum supposed MSAA factor.
	u8 max_msaa();

	/// Gets the refresh rate of the display.
	/// @return Refresh rate of the display.
	float refresh_rate();

	/// @}
} // namespace tr
