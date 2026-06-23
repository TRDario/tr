///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides enumerator utilities.                                                                                                        //
//                                                                                                                                       //
// tr::to_underlying is an implementation of C++23 std::to_underlying.                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

namespace tr {
	// Implements C++23 std::to_underlying.
	template <enumerator Enum> constexpr std::underlying_type_t<Enum> to_underlying(Enum enumerator);
} // namespace tr

#include "impl/enum.hpp" // IWYU pragma: export