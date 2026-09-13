/// @file
/// @brief Provides `tr::graphics_buffer_map_access`.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Graphics buffer map access type.
	enum class graphics_buffer_map_access : u32
	{
		/// Read-only map.
		read_only = 1,

		/// Write-only map.
		write_only = 2,

		/// Read-write map.
		read_write = 3
	};
}; // namespace tr