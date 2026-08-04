/// @file
/// @brief Provides a function to obtain the human-readable name of a type.

#pragma once
#include "common.hpp"

//

namespace tr
{
	/// Gets a human-readable name string for a type.
	/// @note May not return the exact same string on all compilers.
	/// @tparam T Type to get the name of.
	/// @return String view to the name of the type.
	template <typename T>
	consteval std::string_view type_name();

} // namespace tr

#include "impl/type_name.hpp" // IWYU pragma: export