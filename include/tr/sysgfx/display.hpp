#pragma once
#include "../utility/common.hpp"

namespace tr {
	// Gets the size of the display.
	glm::ivec2 display_size();
	// Gets the maximum supported MSAA factor.
	std::uint8_t max_msaa();
	// Gets the refresh rate of the display.
	float refresh_rate();
} // namespace tr
