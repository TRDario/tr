/// @file
/// @brief Provides `tr::orientation`.

#pragma once
#include <tr/utility/common.hpp>

namespace tr
{
	/// Listener orientation.
	struct orientation
	{
		/// View vector.
		glm::vec3 view;

		/// Up vector.
		glm::vec3 up;

		//

		/// Compares orientations for equality.
		/// @return Whether the orientations are identical.
		[[nodiscard]] friend bool operator==(const orientation&, const orientation&) noexcept = default;
	};
} // namespace tr