/// @file
/// @brief Provides `tr::access`.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Access types.
	enum class access : u32
	{
		/// Read-only access.
		read_only = 1,

		/// Write-only access.
		write_only = 2,

		/// Read-write access.
		read_write = 3
	};
}; // namespace tr