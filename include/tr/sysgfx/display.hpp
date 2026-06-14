///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides display querying functions.                                                                                                  //
//                                                                                                                                       //
// The display size, maximum supported level of MSAA, and the refresh rate of the display are all queryable:                             //
//     - tr::display_size() -> {1920, 1080}                                                                                              //
//     - tr::max_msaa() -> 8                                                                                                             //
//     - tr::refresh_rate() -> 60.0f                                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/integer.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Gets the size of the display.
	glm::ivec2 display_size();
	// Gets the maximum supported MSAA factor.
	u8 max_msaa();
	// Gets the refresh rate of the display.
	float refresh_rate();
} // namespace tr
