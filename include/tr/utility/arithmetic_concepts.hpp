/// @file
/// @brief Provides arithmetic concepts.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Concept checking for built-in arithmetic type.
	/// @tparam Target Type to check for being a built-in arithmetic type.
	template <typename Target>
	concept arithmetic = std::integral<Target> || std::floating_point<Target>;
} // namespace tr