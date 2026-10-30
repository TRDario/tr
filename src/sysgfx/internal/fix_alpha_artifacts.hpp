/// @file
/// @brief Provides `tr::internal::fix_alpha_artifacts()`.

#pragma once
#include <tr/sysgfx/bitmap.hpp>

//

namespace tr::internal
{
	/// Fixes alpha artifacts in transparent text.
	/// @param bitmap Rendered text bitmap.
	/// @param max_alpah Maximum allowed alpha.
	/// @return Bitmap with fixed alpha artifacts.
	[[nodiscard]] inline bitmap fix_alpha_artifacts(bitmap&& bitmap, u8 max_alpha) noexcept
	{
		// We know the bitmap is ARGB_8888.
		u8* row_it{reinterpret_cast<u8*>(bitmap.data())};
		for (int y = 0; y < bitmap.size().y; ++y) {
			for (int x = 0; x < bitmap.size().x; ++x) {
				row_it[x * 4 + 3] = std::min(row_it[x * 4 + 3], max_alpha);
			}
			row_it += bitmap.pitch();
		}
		return std::move(bitmap);
	}
}