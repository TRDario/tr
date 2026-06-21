///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides types and definitions pertaining to audio classes.                                                                           //
//                                                                                                                                       //
// Audio sources may be marked with any combination of tr::audio_class_count audio classes (passed in the form of the audio_class_mask   //
// bitset), each of which has its own gain multiplier which is multiplied with the base gain set on the source.                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/integer.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Audio class identifier in the range [0, audio_class_count).
	using audio_class_id = u8;
	// Maximum number of audio classes.
	constexpr usize audio_class_count{32};

	// Bitmask of audio class IDs.
	using audio_class_mask = std::bitset<audio_class_count>;
} // namespace tr