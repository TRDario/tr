#pragma once
#include "../utility/common.hpp"

namespace tr {
	// Gets the size of the display.
	glm::ivec2 display_size() noexcept;
	// Gets the maximum supported MSAA factor.
	std::uint8_t max_msaa() noexcept;
	// Gets the refresh rate of the display.
	float refresh_rate() noexcept;
} // namespace tr
