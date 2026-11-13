/// @file
/// @brief Provides `tr::access`.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @brief Access types.
	enum class access : u32
	{
		/// @brief Read-only access.
		/// @hideinitializer
		read_only = 1,

		/// @brief Write-only access.
		/// @hideinitializer
		write_only = 2,

		/// @brief Read-write access.
		/// @hideinitializer
		read_write = 3
	};
}; // namespace tr