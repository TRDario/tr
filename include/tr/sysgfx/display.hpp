/// @file
/// @brief Provides display querying functions.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @name Display
	/// @{

	/// Gets the size of the display.
	/// @return Size of the display in pixels.
	[[nodiscard]] glm::ivec2 display_size() noexcept;

	/// Gets the maximum supported MSAA factor.
	/// @return Maximum supposed MSAA factor.
	[[nodiscard]] u8 max_msaa() noexcept;

	/// Gets the refresh rate of the display.
	/// @return Refresh rate of the display.
	[[nodiscard]] float refresh_rate() noexcept;

	/// @}
} // namespace tr
