/// @file
/// @brief Provides types and definitions pertaining to audio classes.

#pragma once
#include "../utility/integer.hpp"

//

namespace tr
{
	/// Audio class identifier in the range [0, `audio_class_count`).
	using audio_class_id = u8;

	/// Maximum number of audio classes.
	constexpr usize audio_class_count{32};

	//

	/// Bitmask of audio class IDs.
	using audio_class_mask = std::bitset<audio_class_count>;
} // namespace tr