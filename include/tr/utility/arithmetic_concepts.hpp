/// @file
/// @brief Provides arithmetic concepts.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Built-in arithmetic type.
	template <typename Target>
	concept arithmetic = std::integral<Target> || std::floating_point<Target>;
} // namespace tr