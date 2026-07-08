///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Defines the tr::enable_default_binary_io customization point.                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// If specialized to true, a default binary reader and writer are used (the object's bytes are directly read/written).
	template <cv_unqualified_object T> inline constexpr bool enable_default_binary_io{false};
} // namespace tr