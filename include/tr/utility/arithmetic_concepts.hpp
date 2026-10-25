/// @file
/// @brief Provides arithmetic concepts.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Built-in arithmetic type.
	template <typename T>
	concept arithmetic = std::integral<T> || std::floating_point<T>;
} // namespace tr