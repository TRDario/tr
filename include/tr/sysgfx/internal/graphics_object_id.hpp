/// @file
/// @brief Provides `tr::internal::graphics_object_id`.

#pragma once
#include <tr/utility/unique_handle.hpp>

//

namespace tr::internal
{
	/// @brief ID uniquely identifying a graphics object.
	enum class graphics_object_id : unsigned int
	{
		/// @brief Sentinel value for an invalid graphics object ID.
		invalid
	};

	//

	/// @brief Generates a new graphics object ID.
	/// @return ID uniquely identifying a new graphics object.
	[[nodiscard]] graphics_object_id generate_graphics_object_id() noexcept;
} // namespace tr::internal