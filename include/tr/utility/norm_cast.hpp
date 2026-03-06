///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a cast between normalized values.                                                                                            //
//                                                                                                                                       //
// For integers, their entire range of values can be interpreted as representing fractional values between 0 and 1. tr::norm_cast allows //
// casting between different types of integers in this format, or converting to or from a floating point value in the range [0, 1]:      //
//     - tr::norm_cast<u8>(i16(0)) -> 128                                                                                                //
//     - tr::norm_cast<u8>(0.75) -> 191                                                                                                  //
//     - tr::norm_cast<float>(u8(255)) -> 1.0f                                                                                           //
//     - tr::norm_cast<float>(0.5) -> 0.5f                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Converts a floating point number in the range [0-1] to another floating point number in the range [0-1].
	template <std::floating_point To, std::floating_point From> constexpr To norm_cast(From from);
	// Converts a normalized integer value to a floating point number in the range [0-1].
	template <std::floating_point To, std::integral From> constexpr To norm_cast(From from);
	// Converts a floating point number in the range [0-1] to a normalized integer.
	template <std::integral To, std::floating_point From> constexpr To norm_cast(From from);
	// Converts a normalized integer value to another normalied integer value.
	template <std::integral To, std::integral From> constexpr To norm_cast(From from);
} // namespace tr

#include "impl/norm_cast.hpp"